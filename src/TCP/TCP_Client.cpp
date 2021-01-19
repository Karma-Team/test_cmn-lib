/**
 * "TCP_Client.cpp"
 **/



#include "TCP_Client.hpp"



TCP::CTcpClient::CTcpClient()
{
    m_serverIpAddress 				= TCP_SERVER_IP_ADDRESS;
	m_clientSocket					= -1;
    m_serverSocketPort				= TCP_SERVER_PORT;
}



TCP::CTcpClient::CTcpClient(int p_serverSocketPort, string p_serverSocketIpAddr)
{
	m_serverIpAddress				= p_serverSocketIpAddr;
	m_clientSocket					= -1;
	m_serverSocketPort				= p_serverSocketPort;
}



TCP::CTcpClient::~CTcpClient()
{
    // Close the client socket
    	close(m_clientSocket);
}



int TCP::CTcpClient::initTcpClient()
{
	socklen_t 	l_serverSocketAddrSize;

    // Create the client socket
		m_clientSocket = socket(AF_INET, SOCK_STREAM, 0);
		if (m_clientSocket == -1)
		{
			return 1;
		}

	// Connect the client socket to the server one
		m_serverSocketAddr.sin_family 	= AF_INET;
		m_serverSocketAddr.sin_port 	= htons(m_serverSocketPort);
		if(inet_pton(AF_INET, m_serverIpAddress.c_str(), &m_serverSocketAddr.sin_addr) != 1)
		{
			return -1;
		}

		l_serverSocketAddrSize = sizeof(m_serverSocketAddr);
		if(connect(m_clientSocket, (sockaddr*)&m_serverSocketAddr, l_serverSocketAddrSize) == -1)
		{
			return -1;
		}

	// Launch the reception thead
		m_receiveMsgFromServerThread = thread(&TCP::CTcpClient::receiveMsgFromServerThread, this);

	return m_clientSocket;
}



void TCP::CTcpClient::receiveMsgFromServerThread()
{
	SMsgHeader 	l_msgHeader;
	int			l_readBytesSize;

	while(true)
	{
		// Read the message header
		l_readBytesSize = recv(m_clientSocket, &l_msgHeader, sizeof(SMsgHeader), 0);
		if (l_readBytesSize != -1)
		{
			// Read the message content
			switch(l_msgHeader.id)
			{
				case MSG_ID_POSITION:
					m_positionMsgMutex.lock();
					memcpy(&m_positionMsg.hd, &l_msgHeader, sizeof(SMsgHeader));
					l_readBytesSize = recv(m_clientSocket, &m_positionMsg.body, sizeof(SPositionMsgBody), 0);

					// TMP
					cout << "> Position message : \n";
					cout << "	[hd]\n";
					cout << "		id : " 		<< m_positionMsg.hd.id << "\n";
					cout << "		size : " 	<< m_positionMsg.hd.size << "\n";
					cout << "	[body]\n";
					cout << "		angle : " 	<< m_positionMsg.body.angle << "\n";
					cout << "		coordinates : [";
					cout << "(x : " 			<< m_positionMsg.body.coordinates.x << " | ";
					cout << "y : " 				<< m_positionMsg.body.coordinates.y << ")]\n";

					m_positionMsgMutex.unlock();
					break;

				case MSG_ID_PATH:
					m_pathMsgMutex.lock();
					l_readBytesSize = recv(m_clientSocket, &m_pathMsg, sizeof(SPathMsg), 0);
					m_pathMsgMutex.unlock();
					break;

				case MSG_ID_PATH_CORRECTION:
					m_pathCorrectionMsgMutex.lock();
					l_readBytesSize = recv(m_clientSocket, &m_pathCorrectionMsg, sizeof(SPathCorrectionMsg), 0);
					m_pathCorrectionMsgMutex.unlock();
					break;

				case MSG_ID_WORKSHOP_ORDER:
					m_workShopOrderMsgMutex.lock();
					l_readBytesSize = recv(m_clientSocket, &m_workShopOrderMsg, sizeof(SWorkShopOrderMsg), 0);
					m_workShopOrderMsgMutex.unlock();
					break;

				case MSG_ID_STOP:
					m_stopMsgMutex.lock();
					l_readBytesSize = recv(m_clientSocket, &m_stopMsg, sizeof(SStopMsg), 0);
					m_stopMsgMutex.unlock();
					break;

				case MSG_ID_WORKSHOP_REPORT:
					m_workShopReportMsgMutex.lock();
					l_readBytesSize = recv(m_clientSocket, &m_workShopReportMsg, sizeof(SWorkShopReportMsg), 0);
					m_workShopReportMsgMutex.unlock();
					break;

				case MSG_ID_BIT_REPORT:
					m_bitReportMsgMutex.lock();
					l_readBytesSize = recv(m_clientSocket, &m_bitReportMsg, sizeof(SBitReportMsg), 0);
					m_bitReportMsgMutex.unlock();
					break;

				case MSG_ID_ERROR:
					m_errorMsgMutex.lock();
					l_readBytesSize = recv(m_clientSocket, &m_errorMsg, sizeof(SErrorMsg), 0);
					m_errorMsgMutex.unlock();
					break;

				default:
					break;
			}
		}
	}
}



int TCP::CTcpClient::requestMsgToServer(uint32_t p_requestedMsgId)
{
	// Send the requested message ID to TCP server
		if(send(m_clientSocket, &p_requestedMsgId, sizeof(uint32_t), 0) == -1)
		{
			return -1;
		}

	return 1;
}



void TCP::CTcpClient::getPositionMsg(SPositionMsg* p_positionMsg)
{
	m_positionMsgMutex.lock();
	memcpy(p_positionMsg, &m_positionMsg, sizeof(SPositionMsg));
	m_positionMsgMutex.unlock();
}
