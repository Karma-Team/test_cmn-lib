/**
 * "TCP_Server.cpp"
 **/



#include "TCP_Server.hpp"



TCP::CTcpServer::CTcpServer()
{
    m_serverSocketAddrSize			= 0;
    m_serverIpAddress 				= TCP_SERVER_IP_ADDRESS;
    m_serverSocketPort				= TCP_SERVER_PORT;
    m_serverClientNb 				= 0;
	m_serverSocket					= -1;
}



TCP::CTcpServer::CTcpServer(int p_serverSocketPort, string p_serverSocketIpAddr)
{
    m_serverSocketAddrSize			= 0;
    m_serverIpAddress 				= p_serverSocketIpAddr;
    m_serverSocketPort				= p_serverSocketPort;
    m_serverClientNb 				= 0;
	m_serverSocket					= -1;
}



TCP::CTcpServer::~CTcpServer()
{
    // Close the server socket
    	close(m_serverSocket);
		cout << "> TCP server socket closed" << endl;
}



int TCP::CTcpServer::initTcpServer()
{
	cout << "> Initialize the TCP server" << endl;

	// Create the TCP server socket
		m_serverSocket = socket(AF_INET, SOCK_STREAM, 0);
		if(m_serverSocket == -1)
		{
			cerr << "> Can't create the server socket! Quitting" << endl;
			return -1;
		}
		cout << "> TCP server socket created" << endl;

    // Assign an address and a port to the TCP server socket
		m_serverSocketAddr.sin_family	= AF_INET;
		m_serverSocketAddr.sin_port 	= htons(m_serverSocketPort);
		if(inet_pton(AF_INET, m_serverIpAddress.c_str(), &m_serverSocketAddr.sin_addr) != 1)
		{
			cerr << "> Can't convert the Internet address! Quitting" << endl;
			return -1;
		}

		m_serverSocketAddrSize = sizeof(m_serverSocketAddr);
		if(bind(m_serverSocket, (sockaddr*)&m_serverSocketAddr, m_serverSocketAddrSize) == -1)
		{
			cerr << "> Can't assign an address and a port to the server socket! Quitting" << endl;
			return -1;
		}
		cout << "> Address and port assigned to the TCP server socket" << endl;

    // Enable the TCP server socket to accept client connections
		if(listen(m_serverSocket, SOMAXCONN) == -1)
		{
			cerr << "> Can't enable the server socket to accept connections! Quitting" << endl;
			return -1;
		}
		cout << "> Client connections enabled to the TCP server socket" << endl;

	// Start the TCP server
		m_startThread = thread(&TCP::CTcpServer::startTcpServer, this);

	return m_serverSocket;
}


void TCP::CTcpServer::startTcpServer()

{
	cout << "> Start the TCP server" << endl;

    socklen_t 	l_clientSocketAddrSize;

	if(m_serverSocket != -1)
	{
		while(m_serverClientNb < SOMAXCONN)
		{
			// Wait for a client connection
				l_clientSocketAddrSize				= sizeof(sockaddr_in);
				m_clientSocket[m_serverClientNb] 	= accept(m_serverSocket, (sockaddr*)&m_clientSocketAddr[m_serverClientNb], &l_clientSocketAddrSize);

			// Create a client thread
				m_clientThread[m_serverClientNb] = thread(&TCP::CTcpServer::clientThread, this, m_serverClientNb);
				m_serverClientNb++;
		}
	}
	else
	{
		cerr << "> TCP server is not initialized! Quitting" << endl;
	}
}


void TCP::CTcpServer::clientThread(uint32_t p_clientId)
{
	cout << "> Client thread launched\n";

	uint32_t 	l_clientRequestedMsgId;
	int			l_receivedBytesNb;

	// Translate the client socket address to a location and a service name
		memset(m_clientName[p_clientId], 0, NI_MAXHOST);
		memset(m_clientPort[p_clientId], 0, NI_MAXSERV);
		if(getnameinfo((sockaddr*)&m_clientSocketAddr[p_clientId], sizeof(m_clientSocketAddr[p_clientId]), m_clientName[p_clientId], NI_MAXHOST, m_clientPort[p_clientId], NI_MAXSERV, 0) == 0)
		{
			cout << "> " << m_clientName[p_clientId] << " connected on port " << m_clientPort[p_clientId] << endl;
		}
		else
		{
			inet_ntop(AF_INET, &this->m_clientSocketAddr[p_clientId].sin_addr, this->m_clientName[p_clientId], NI_MAXHOST);
			cout << "> " << this->m_clientName[p_clientId] << " connected on port " << ntohs(this->m_clientSocketAddr[p_clientId].sin_port) << endl;
		}

	// Wait requested message ID from client
		while(true)
		{
			// Receive requested message ID from client
				l_receivedBytesNb = recv(m_clientSocket[p_clientId], &l_clientRequestedMsgId, sizeof(uint32_t), 0);
				if(l_receivedBytesNb == -1)
				{
					cerr << "> Error in recv()! Quitting" << endl;
					break;
				}
				if(l_receivedBytesNb == 0)
				{
					cout << "> Client disconnected! Quitting " << endl;
					break;
				}

			// Send requested message to client
				switch(l_clientRequestedMsgId)
				{
					case MSG_ID_PATH:
						cout << "> Requested message from client : MSG_ID_PATH\n";
						this->sendPathMsgToClient(p_clientId);
						break;

					case MSG_ID_PATH_CORRECTION:
						cout << "> Requested message from client : MSG_ID_PATH_CORRECTION\n";
						this->sendPathCorrectionMsgToClient(p_clientId);
						break;

					case MSG_ID_WORKSHOP_ORDER:
						cout << "> Requested message from client : MSG_ID_WORKSHOP_ORDER\n";
						this->sendWorkShopOrderMsgToClient(p_clientId);
						break;

					case MSG_ID_STOP:
						cout << "> Requested message from client : MSG_ID_STOP\n";
						sendStopMsgToClient(p_clientId);
						break;

					case MSG_ID_WORKSHOP_REPORT:
						cout << "> Requested message from client : MSG_ID_WORKSHOP_REPORT\n";
						sendWorkShopReportMsgToClient(p_clientId);
						break;

					case MSG_ID_BIT_REPORT:
						cout << "> Requested message from client : MSG_ID_BIT_REPORT\n";
						sendBitReportMsgToClient(p_clientId);
						break;

					case MSG_ID_ERROR:
						cout << "> Requested message from client : MSG_ID_ERROR\n";
						sendErrorMsgToClient(p_clientId);
						break;

					default:
						cout << "> Unknown message ID\n";
				}
		}
}



int TCP::CTcpServer::sendPathMsgToClient(uint32_t p_clientId)
{
	if(send(m_clientSocket[p_clientId], &m_pathMsgBody, sizeof(SPathMsgBody), 0) == -1)
	{
		cout << "> Can't send path message to client! Quitting " << endl;
		return -1;
	}
	cout << "> Path message sent to client" << "\n";

	return 1;
}



int TCP::CTcpServer::sendPathCorrectionMsgToClient(uint32_t p_clientId)
{
	if(send(m_clientSocket[p_clientId], &m_pathCorrectionMsgBody, sizeof(SPathCorrectionMsgBody), 0) == -1)
	{
		cout << "> Can't send path correction message to client! Quitting " << endl;
		return -1;
	}
	cout << "> Path correction message sent to client" << "\n";

	return 1;
}



int TCP::CTcpServer::sendWorkShopOrderMsgToClient(uint32_t p_clientId)
{
	if(send(m_clientSocket[p_clientId], &m_workShopOrderMsgBody, sizeof(SWorkShopOrderMsgBody), 0) == -1)
	{
		cout << "> Can't send workshop order message to client! Quitting " << endl;
		return -1;
	}
	cout << "> Workshop order message sent to client" << "\n";

	return 1;
}



int TCP::CTcpServer::sendStopMsgToClient(uint32_t p_clientId)
{
	if(send(m_clientSocket[p_clientId], &m_stopMsgBody, sizeof(SStopMsgBody), 0) == -1)
	{
		cout << "> Can't send stop message to client! Quitting " << endl;
		return -1;
	}
	cout << "> Stop message sent to client" << "\n";

	return 1;
}



int TCP::CTcpServer::sendWorkShopReportMsgToClient(uint32_t p_clientId)
{
	if(send(m_clientSocket[p_clientId], &m_workShopReportMsgBody, sizeof(SWorkShopReportMsgBody), 0) == -1)
	{
		cout << "> Can't send workshop report message to client! Quitting " << endl;
		return -1;
	}
	cout << "> Workshop report message sent to client" << "\n";

	return 1;
}



int TCP::CTcpServer::sendBitReportMsgToClient(uint32_t p_clientId)
{
	if(send(m_clientSocket[p_clientId], &m_bitReportMsgBody, sizeof(SBitReportMsgBody), 0) == -1)
	{
		cout << "> Can't send bit report message to client! Quitting " << endl;
		return -1;
	}
	cout << "> Bit report message sent to client" << "\n";

	return 1;
}



int TCP::CTcpServer::sendErrorMsgToClient(uint32_t p_clientId)
{
	if(send(m_clientSocket[p_clientId], &m_errorMsgBody, sizeof(SErrorMsgBody), 0) == -1)
	{
		cout << "> Can't send Error message to client! Quitting " << endl;
		return -1;
	}
	cout << "> Error message sent to client" << "\n";

	return 1;
}
