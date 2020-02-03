/**
 * "TCP_Server.cpp"
 **/



#include "TCP_Server.hpp"



TCP::CTcpServer::CTcpServer()
{
    m_serverSocketAddrSize							= 0;
    m_serverIpAddress 								= TCP_SERVER_IP_ADDRESS;
    m_serverSocketPort								= TCP_SERVER_PORT;
    m_serverClientNb 								= 0;
	m_serverSocket									= -1;
	struct SPathMsg 			l_pathMsg 			= {.hd={MSG_ID_PATH, 			sizeof(SPathMsg)}, 				.body={0, {0,0,0,0,0,0,0,0,0,0}}};
	struct SPathCorrectionMsg	l_pathCorrectionMsg	= {.hd={MSG_ID_PATH_CORRECTION, sizeof(SPathCorrectionMsg)}, 	.body={0, 0, {0,0,0,0,0,0,0,0,0,0}}};
	struct SWorkShopOrderMsg 	l_workShopOrderMsg	= {.hd={MSG_ID_WORKSHOP_ORDER, 	sizeof(SWorkShopOrderMsg)}, 	.body={0}};
	struct SStopMsg 			l_stopMsg			= {.hd={MSG_ID_STOP, 			sizeof(SStopMsg)}, 				.body={0}};
	struct SWorkShopReportMsg 	l_workShopReportMsg	= {.hd={MSG_ID_WORKSHOP_REPORT, sizeof(SWorkShopReportMsg)}, 	.body={0}};
	struct SBitReportMsg 		l_bitReportMsg		= {.hd={MSG_ID_BIT_REPORT, 		sizeof(SBitReportMsg)}, 		.body={0}};
	struct SErrorMsg 			l_errorMsg			= {.hd={MSG_ID_ERROR, 			sizeof(SErrorMsg)}, 			.body={0}};
	m_pathMsg 										= l_pathMsg;
	m_pathCorrectionMsg 							= l_pathCorrectionMsg;
	m_workShopOrderMsg								= l_workShopOrderMsg;
	m_stopMsg										= l_stopMsg;
	m_workShopReportMsg								= l_workShopReportMsg;
	m_bitReportMsg									= l_bitReportMsg;
	m_errorMsg										= l_errorMsg;
}



TCP::CTcpServer::CTcpServer(int p_serverSocketPort, string p_serverSocketIpAddr)
{
    m_serverSocketAddrSize							= 0;
    m_serverIpAddress 								= p_serverSocketIpAddr;
    m_serverSocketPort								= p_serverSocketPort;
    m_serverClientNb 								= 0;
	m_serverSocket									= -1;
	struct SPathMsg 			l_pathMsg 			= {.hd={MSG_ID_PATH, 			sizeof(SPathMsg)}, 				.body={0, {0,0,0,0,0,0,0,0,0,0}}};
	struct SPathCorrectionMsg	l_pathCorrectionMsg	= {.hd={MSG_ID_PATH_CORRECTION, sizeof(SPathCorrectionMsg)}, 	.body={0, 0, {0,0,0,0,0,0,0,0,0,0}}};
	struct SWorkShopOrderMsg 	l_workShopOrderMsg	= {.hd={MSG_ID_WORKSHOP_ORDER, 	sizeof(SWorkShopOrderMsg)}, 	.body={0}};
	struct SStopMsg 			l_stopMsg			= {.hd={MSG_ID_STOP, 			sizeof(SStopMsg)}, 				.body={0}};
	struct SWorkShopReportMsg 	l_workShopReportMsg	= {.hd={MSG_ID_WORKSHOP_REPORT, sizeof(SWorkShopReportMsg)}, 	.body={0}};
	struct SBitReportMsg 		l_bitReportMsg		= {.hd={MSG_ID_BIT_REPORT, 		sizeof(SBitReportMsg)}, 		.body={0}};
	struct SErrorMsg 			l_errorMsg			= {.hd={MSG_ID_ERROR, 			sizeof(SErrorMsg)}, 			.body={0}};
	m_pathMsg 										= l_pathMsg;
	m_pathCorrectionMsg 							= l_pathCorrectionMsg;
	m_workShopOrderMsg								= l_workShopOrderMsg;
	m_stopMsg										= l_stopMsg;
	m_workShopReportMsg								= l_workShopReportMsg;
	m_bitReportMsg									= l_bitReportMsg;
	m_errorMsg										= l_errorMsg;
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
	m_pathMsg.hd.id 	= MSG_ID_PATH;
	m_pathMsg.hd.size 	= sizeof(SPathMsg);

	if(send(m_clientSocket[p_clientId], &m_pathMsg, sizeof(SPathMsg), 0) == -1)
	{
		cout << "> Can't send path message to client! Quitting " << endl;
		return -1;
	}

	cout << "> Path message sent to client" << "\n";

	return 1;
}



int TCP::CTcpServer::sendPathCorrectionMsgToClient(uint32_t p_clientId)
{
	m_pathCorrectionMsg.hd.id 	= MSG_ID_PATH_CORRECTION;
	m_pathCorrectionMsg.hd.size	= sizeof(SPathCorrectionMsg);

	if(send(m_clientSocket[p_clientId], &m_pathCorrectionMsg, sizeof(SPathCorrectionMsg), 0) == -1)
	{
		cout << "> Can't send path correction message to client! Quitting " << endl;
		return -1;
	}

	cout << "> Path correction message sent to client" << "\n";

	return 1;
}



int TCP::CTcpServer::sendWorkShopOrderMsgToClient(uint32_t p_clientId)
{
	m_workShopOrderMsg.hd.id 	= MSG_ID_WORKSHOP_ORDER;
	m_workShopOrderMsg.hd.size 	= sizeof(SWorkShopOrderMsg);

	if(send(m_clientSocket[p_clientId], &m_workShopOrderMsg, sizeof(SWorkShopOrderMsg), 0) == -1)
	{
		cout << "> Can't send workshop order message to client! Quitting " << endl;
		return -1;
	}

	cout << "> Workshop order message sent to client" << "\n";

	return 1;
}



int TCP::CTcpServer::sendStopMsgToClient(uint32_t p_clientId)
{
	m_stopMsg.hd.id 	= MSG_ID_STOP;
	m_stopMsg.hd.size 	= sizeof(SStopMsg);

	if(send(m_clientSocket[p_clientId], &m_stopMsg, sizeof(SStopMsg), 0) == -1)
	{
		cout << "> Can't send stop message to client! Quitting " << endl;
		return -1;
	}

	cout << "> Stop message sent to client" << "\n";

	return 1;
}



int TCP::CTcpServer::sendWorkShopReportMsgToClient(uint32_t p_clientId)
{
	m_workShopReportMsg.hd.id 	= MSG_ID_WORKSHOP_REPORT;
	m_workShopReportMsg.hd.size = sizeof(SWorkShopReportMsg);

	if(send(m_clientSocket[p_clientId], &m_workShopReportMsg, sizeof(SWorkShopReportMsg), 0) == -1)
	{
		cout << "> Can't send workshop report message to client! Quitting " << endl;
		return -1;
	}

	cout << "> Workshop report message sent to client" << "\n";

	return 1;
}



int TCP::CTcpServer::sendBitReportMsgToClient(uint32_t p_clientId)
{
	m_bitReportMsg.hd.id 	= MSG_ID_BIT_REPORT;
	m_bitReportMsg.hd.size 	= sizeof(SBitReportMsg);

	if(send(m_clientSocket[p_clientId], &m_bitReportMsg, sizeof(SBitReportMsg), 0) == -1)
	{
		cout << "> Can't send bit report message to client! Quitting " << endl;
		return -1;
	}

	cout << "> Bit report message sent to client" << "\n";

	return 1;
}



int TCP::CTcpServer::sendErrorMsgToClient(uint32_t p_clientId)
{
	m_errorMsg.hd.id 	= MSG_ID_STOP;
	m_errorMsg.hd.size 	= sizeof(SErrorMsg);

	if(send(m_clientSocket[p_clientId], &m_errorMsg, sizeof(SErrorMsg), 0) == -1)
	{
		cout << "> Can't send Error message to client! Quitting " << endl;
		return -1;
	}

	cout << "> Error message sent to client" << "\n";

	return 1;
}



int TCP::CTcpServer::updateMsg(uint32_t p_updateMsgId, void* p_updateMsgBuffer)
{
	// Update message
		switch(p_updateMsgId)
		{
			case MSG_ID_PATH:
				cout << "> Update PATH message\n";
				memcpy(&m_pathMsg.body, p_updateMsgBuffer, sizeof(SPathMsgBody));
				break;

			case MSG_ID_PATH_CORRECTION:
				cout << "> Update PATH_CORRECTION message\n";
				memcpy(&m_pathCorrectionMsg.body, p_updateMsgBuffer, sizeof(SPathCorrectionMsgBody));
				break;

			case MSG_ID_WORKSHOP_ORDER:
				cout << "> Update WORKSHOP_ORDER message\n";
				memcpy(&m_workShopOrderMsg.body, p_updateMsgBuffer, sizeof(SWorkShopOrderMsgBody));
				break;

			case MSG_ID_STOP:
				cout << "> Update STOP message\n";
				memcpy(&m_stopMsg.body, p_updateMsgBuffer, sizeof(SStopMsgBody));
				break;

			case MSG_ID_WORKSHOP_REPORT:
				cout << "> Update WORKSHOP_REPORT message\n";
				memcpy(&m_workShopReportMsg.body, p_updateMsgBuffer, sizeof(SWorkShopReportMsgBody));
				break;

			case MSG_ID_BIT_REPORT:
				cout << "> Update BIT_REPORT message\n";
				memcpy(&m_bitReportMsg.body, p_updateMsgBuffer, sizeof(SBitReportMsgBody));
				break;

			case MSG_ID_ERROR:
				cout << "> Update ERROR message\n";
				memcpy(&m_errorMsg.body, p_updateMsgBuffer, sizeof(SErrorMsgBody));
				break;

			default:
				cout << "> Unknown message ID\n";
		}

	return 1;
}



/*
int TCP::CTcpServer::updatePathMsg(SPathMsgBody p_pathMsgBody)
{
	m_pathMsg.body = p_pathMsgBody;

	return 1;
}



int TCP::CTcpServer::updatePathCorrectionMsg(SPathCorrectionMsgBody p_pathCorrectionMsgBody)
{
	m_pathCorrectionMsg.body = p_pathCorrectionMsgBody;

	return 1;
}



int TCP::CTcpServer::updateWorkShopOrderMsg(SWorkShopOrderMsgBody p_workShopOrderMsgBody)
{
	m_workShopOrderMsg.body = p_workShopOrderMsgBody;

	return 1;
}



int TCP::CTcpServer::updateStopMsg(SStopMsgBody p_stopMsgBody)
{
	m_stopMsg.body = p_stopMsgBody;

	return 1;
}



int TCP::CTcpServer::updateWorkShopReportMsg(SWorkShopReportMsgBody p_workShopReportMsgBody)
{
	m_workShopReportMsg.body = p_workShopReportMsgBody;

	return 1;
}



int TCP::CTcpServer::updateBitReportMsg(SBitReportMsgBody p_bitReportMsgBody)
{
	m_bitReportMsg.body = p_bitReportMsgBody;

	return 1;
}



int TCP::CTcpServer::updateErrorMsg(SErrorMsgBody p_errorMsgBody)
{
	m_errorMsg.body = p_errorMsgBody;

	return 1;
}
*/
