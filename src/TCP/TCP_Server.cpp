/**
 * "TCP_Server.cpp"
 **/



#include "TCP_Server.hpp"



TCP::CTcpServer::CTcpServer()
{
    m_serverSocketAddrSize						= 0;
    m_serverIpAddress 							= TCP_SERVER_IP_ADDRESS;
    m_serverSocketPort							= TCP_SERVER_PORT;
    m_serverClientCounter 						= 0;
	m_serverSocket								= -1;
	SPositionMsg 			l_positionMsg 		= {.hd={MSG_ID_POSITION, 		sizeof(SPositionMsg)}, 			.body={0, 0}};
	SPathMsg 				l_pathMsg 			= {.hd={MSG_ID_PATH, 			sizeof(SPathMsg)}, 				.body={0, {0,0,0,0,0,0,0,0,0,0}}};
	SPathCorrectionMsg		l_pathCorrectionMsg	= {.hd={MSG_ID_PATH_CORRECTION, sizeof(SPathCorrectionMsg)}, 	.body={0, 0, {0,0,0,0,0,0,0,0,0,0}}};
	SWorkShopOrderMsg 		l_workShopOrderMsg	= {.hd={MSG_ID_WORKSHOP_ORDER, 	sizeof(SWorkShopOrderMsg)}, 	.body={0}};
	SStopMsg 				l_stopMsg			= {.hd={MSG_ID_STOP, 			sizeof(SStopMsg)}, 				.body={0}};
	SWorkShopReportMsg 		l_workShopReportMsg	= {.hd={MSG_ID_WORKSHOP_REPORT, sizeof(SWorkShopReportMsg)}, 	.body={0}};
	SBitReportMsg 			l_bitReportMsg		= {.hd={MSG_ID_BIT_REPORT, 		sizeof(SBitReportMsg)}, 		.body={0}};
	SErrorMsg 				l_errorMsg			= {.hd={MSG_ID_ERROR, 			sizeof(SErrorMsg)}, 			.body={0}};
	m_positionMsg 								= l_positionMsg;
	m_pathMsg 									= l_pathMsg;
	m_pathCorrectionMsg 						= l_pathCorrectionMsg;
	m_workShopOrderMsg							= l_workShopOrderMsg;
	m_stopMsg									= l_stopMsg;
	m_workShopReportMsg							= l_workShopReportMsg;
	m_bitReportMsg								= l_bitReportMsg;
	m_errorMsg									= l_errorMsg;
}



TCP::CTcpServer::CTcpServer(int p_serverSocketPort, string p_serverSocketIpAddr)
{
    m_serverSocketAddrSize						= 0;
    m_serverIpAddress 							= p_serverSocketIpAddr;
    m_serverSocketPort							= p_serverSocketPort;
    m_serverClientCounter 						= 0;
	m_serverSocket								= -1;
	SPositionMsg 			l_positionMsg 		= {.hd={MSG_ID_POSITION, 		sizeof(SPositionMsg)}, 			.body={0, 0}};
	SPathMsg 				l_pathMsg 			= {.hd={MSG_ID_PATH, 			sizeof(SPathMsg)}, 				.body={0, {0,0,0,0,0,0,0,0,0,0}}};
	SPathCorrectionMsg		l_pathCorrectionMsg	= {.hd={MSG_ID_PATH_CORRECTION, sizeof(SPathCorrectionMsg)}, 	.body={0, 0, {0,0,0,0,0,0,0,0,0,0}}};
	SWorkShopOrderMsg 		l_workShopOrderMsg	= {.hd={MSG_ID_WORKSHOP_ORDER, 	sizeof(SWorkShopOrderMsg)}, 	.body={0}};
	SStopMsg 				l_stopMsg			= {.hd={MSG_ID_STOP, 			sizeof(SStopMsg)}, 				.body={0}};
	SWorkShopReportMsg 		l_workShopReportMsg	= {.hd={MSG_ID_WORKSHOP_REPORT, sizeof(SWorkShopReportMsg)}, 	.body={0}};
	SBitReportMsg 			l_bitReportMsg		= {.hd={MSG_ID_BIT_REPORT, 		sizeof(SBitReportMsg)}, 		.body={0}};
	SErrorMsg 				l_errorMsg			= {.hd={MSG_ID_ERROR, 			sizeof(SErrorMsg)}, 			.body={0}};
	m_positionMsg 								= l_positionMsg;
	m_pathMsg 									= l_pathMsg;
	m_pathCorrectionMsg 						= l_pathCorrectionMsg;
	m_workShopOrderMsg							= l_workShopOrderMsg;
	m_stopMsg									= l_stopMsg;
	m_workShopReportMsg							= l_workShopReportMsg;
	m_bitReportMsg								= l_bitReportMsg;
	m_errorMsg									= l_errorMsg;
}



TCP::CTcpServer::~CTcpServer()
{
    // Close the server socket
    	close(m_serverSocket);
}



int TCP::CTcpServer::initTcpServer()
{
	// Create the TCP server socket
		m_serverSocket = socket(AF_INET, SOCK_STREAM, 0);
		if(m_serverSocket == -1)
		{
			return -1;
		}

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
			return -1;
		}

    // Enable the TCP server socket to accept client connections
		if(listen(m_serverSocket, SOMAXCONN) == -1)
		{
			return -1;
		}

	// Start the TCP server
		srand((unsigned) time(0));
		m_startThread = thread(&TCP::CTcpServer::startTcpServer, this);

	return m_serverSocket;
}


void TCP::CTcpServer::startTcpServer()
{
    socklen_t 	l_clientSocketAddrSize;

	if(m_serverSocket != -1)
	{
		while(m_serverClientCounter < SOMAXCONN)
		{
			// Wait for a client connection
				l_clientSocketAddrSize				= sizeof(sockaddr_in);
				m_clientSocket[m_serverClientCounter] 	= accept(m_serverSocket, (sockaddr*)&m_clientSocketAddr[m_serverClientCounter], &l_clientSocketAddrSize);

			// Translate the client socket address to a location and a service name
				memset(m_clientName[m_serverClientCounter], 0, NI_MAXHOST);
				memset(m_clientPort[m_serverClientCounter], 0, NI_MAXSERV);
				if(getnameinfo((sockaddr*)&m_clientSocketAddr[m_serverClientCounter], sizeof(m_clientSocketAddr[m_serverClientCounter]), m_clientName[m_serverClientCounter], NI_MAXHOST, m_clientPort[m_serverClientCounter], NI_MAXSERV, 0) != 0)
				{
					inet_ntop(AF_INET, &m_clientSocketAddr[m_serverClientCounter].sin_addr, m_clientName[m_serverClientCounter], NI_MAXHOST);
				}

			// Create the client threads
				// Launch the thread sending periodic information to the client
					m_sendPeriodicMsgToClientThread[m_serverClientCounter] = thread(&TCP::CTcpServer::sendPeriodicMsgToClientThread, this, m_serverClientCounter);

				// Launch the thread waiting for a client request
					m_treatClientRequestThread[m_serverClientCounter] = thread(&TCP::CTcpServer::treatClientRequestThread, this, m_serverClientCounter);

			// Update the client counter
				m_serverClientCounter++;
		}
	}
	else
	{
		cerr << "> TCP server is not initialized! Quitting" << endl;
	}
}



void TCP::CTcpServer::sendPeriodicMsgToClientThread(uint32_t p_clientId)
{
	EMsgId		l_periodicMsgId = MSG_ID_POSITION;
	uint32_t 	l_sleepTimeUs 	= 1000000;

	// Send a periodic message to the client
		while(true)
		{
			this->sendMsgToClient(l_periodicMsgId, p_clientId);

			// AHU : mise a jour aleatoire temporaire (normalement mis a jour apres traitement d'image du mat sur le plan de jeu)
			this->tmpUpdatePositionMsg();

			usleep(l_sleepTimeUs);
		}
}



void TCP::CTcpServer::tmpUpdatePositionMsg()
{
	int coordinatesMin 		= 0;
	int coordinatesMax 		= 1000;
	int coordinatesRange	= coordinatesMax - coordinatesMin + 1;
	int angleMin 			= 0;
	int angleMax 			= 360;
	int angleRange 			= angleMax - angleMin + 1;

	int xRand 		= rand() % coordinatesRange + coordinatesMin;
	int yRand 		= rand() % coordinatesRange + coordinatesMin;
	int angleRand 	= rand() % angleRange + angleMin;

	m_positionMsgMutex.lock();
	m_positionMsg.body.coordinates.x 	= xRand;
	m_positionMsg.body.coordinates.y 	= yRand;
	m_positionMsg.body.angle 			= angleRand;
	/*
	cout << "> Position message : \n";
	cout << "	[hd]\n";
	cout << "		id : " 		<< m_positionMsg.hd.id << "\n";
	cout << "		size : " 	<< m_positionMsg.hd.size << "\n";
	cout << "	[body]\n";
	cout << "		angle : " 	<< m_positionMsg.body.angle << "\n";
	cout << "		coordinates : [";
	cout << "(x : " 			<< m_positionMsg.body.coordinates.x << " | ";
	cout << "y : " 				<< m_positionMsg.body.coordinates.y << ")]\n";
	*/
	m_positionMsgMutex.unlock();
}



void TCP::CTcpServer::treatClientRequestThread(uint32_t p_clientId)
{
	uint32_t 	l_clientRequestedMsgId;
	int			l_receivedBytesNb;

	// Wait a requested message ID from the client
		while(true)
		{
			// Receive requested message ID from client
				l_receivedBytesNb = recv(m_clientSocket[p_clientId], &l_clientRequestedMsgId, sizeof(uint32_t), 0);
				if((l_receivedBytesNb == -1) || (l_receivedBytesNb == 0))
				{
					break;
				}

			// Send requested message to client
				this->sendMsgToClient(l_clientRequestedMsgId, p_clientId);
		}
}



int TCP::CTcpServer::sendMsgToClient(uint32_t p_sendMsgId, uint32_t p_clientId)
{
	switch(p_sendMsgId)
	{
		case MSG_ID_POSITION:
			m_positionMsgMutex.lock();
			if(send(m_clientSocket[p_clientId], &m_positionMsg, sizeof(SPositionMsg), 0) == -1)
			{
				return -1;
			}
			m_positionMsgMutex.unlock();
			break;

		case MSG_ID_PATH:
			m_pathMsgMutex.lock();
			if(send(m_clientSocket[p_clientId], &m_pathMsg, sizeof(SPathMsg), 0) == -1)
			{
				return -1;
			}
			m_pathMsgMutex.unlock();
			break;

		case MSG_ID_PATH_CORRECTION:
			m_pathCorrectionMsgMutex.lock();
			if(send(m_clientSocket[p_clientId], &m_pathCorrectionMsg, sizeof(SPathCorrectionMsg), 0) == -1)
			{
				return -1;
			}
			m_pathCorrectionMsgMutex.unlock();
			break;

		case MSG_ID_WORKSHOP_ORDER:
			m_workShopOrderMsgMutex.lock();
			if(send(m_clientSocket[p_clientId], &m_workShopOrderMsg, sizeof(SWorkShopOrderMsg), 0) == -1)
			{
				return -1;
			}
			m_workShopOrderMsgMutex.unlock();
			break;

		case MSG_ID_STOP:
			m_stopMsgMutex.lock();
			if(send(m_clientSocket[p_clientId], &m_stopMsg, sizeof(SStopMsg), 0) == -1)
			{
				return -1;
			}
			m_stopMsgMutex.unlock();
			break;

		case MSG_ID_WORKSHOP_REPORT:
			m_workShopReportMsgMutex.lock();
			if(send(m_clientSocket[p_clientId], &m_workShopReportMsg, sizeof(SWorkShopReportMsg), 0) == -1)
			{
				return -1;
			}
			m_workShopReportMsgMutex.unlock();
			break;

		case MSG_ID_BIT_REPORT:
			m_bitReportMsgMutex.lock();
			if(send(m_clientSocket[p_clientId], &m_bitReportMsg, sizeof(SBitReportMsg), 0) == -1)
			{
				return -1;
			}
			m_bitReportMsgMutex.unlock();
			break;

		case MSG_ID_ERROR:
			m_errorMsgMutex.lock();
			if(send(m_clientSocket[p_clientId], &m_errorMsg, sizeof(SErrorMsg), 0) == -1)
			{
				return -1;
			}
			m_errorMsgMutex.unlock();
			break;

		default:
			break;
	}

	return 1;
}



int TCP::CTcpServer::updateMsg(uint32_t p_updateMsgId, void* p_updateMsgBuffer)
{
	switch(p_updateMsgId)
	{
		case MSG_ID_POSITION:
			m_positionMsgMutex.lock();
			memcpy(&m_positionMsg.body, p_updateMsgBuffer, sizeof(SPositionMsgBody));
			m_positionMsgMutex.unlock();
			break;

		case MSG_ID_PATH:
			m_pathMsgMutex.lock();
			memcpy(&m_pathMsg.body, p_updateMsgBuffer, sizeof(SPathMsgBody));
			m_pathMsgMutex.unlock();
			break;

		case MSG_ID_PATH_CORRECTION:
			m_pathCorrectionMsgMutex.lock();
			memcpy(&m_pathCorrectionMsg.body, p_updateMsgBuffer, sizeof(SPathCorrectionMsgBody));
			m_pathCorrectionMsgMutex.unlock();
			break;

		case MSG_ID_WORKSHOP_ORDER:
			m_workShopOrderMsgMutex.lock();
			memcpy(&m_workShopOrderMsg.body, p_updateMsgBuffer, sizeof(SWorkShopOrderMsgBody));
			m_workShopOrderMsgMutex.unlock();
			break;

		case MSG_ID_STOP:
			m_stopMsgMutex.lock();
			memcpy(&m_stopMsg.body, p_updateMsgBuffer, sizeof(SStopMsgBody));
			m_stopMsgMutex.unlock();
			break;

		case MSG_ID_WORKSHOP_REPORT:
			m_workShopReportMsgMutex.lock();
			memcpy(&m_workShopReportMsg.body, p_updateMsgBuffer, sizeof(SWorkShopReportMsgBody));
			m_workShopReportMsgMutex.unlock();
			break;

		case MSG_ID_BIT_REPORT:
			m_bitReportMsgMutex.lock();
			memcpy(&m_bitReportMsg.body, p_updateMsgBuffer, sizeof(SBitReportMsgBody));
			m_bitReportMsgMutex.unlock();
			break;

		case MSG_ID_ERROR:
			m_errorMsgMutex.lock();
			memcpy(&m_errorMsg.body, p_updateMsgBuffer, sizeof(SErrorMsgBody));
			m_errorMsgMutex.unlock();
			break;

		default:
			break;
	}

	return 1;
}



int TCP::CTcpServer::getMsg(uint32_t p_getMsgId, void* p_getMsgBuffer)
{
	switch(p_getMsgId)
	{
		case MSG_ID_POSITION:
			m_positionMsgMutex.lock();
			memcpy(p_getMsgBuffer, &m_positionMsg, sizeof(SPositionMsg));
			m_positionMsgMutex.unlock();
			break;

		case MSG_ID_PATH:
			m_pathMsgMutex.lock();
			memcpy(p_getMsgBuffer, &m_pathMsg, sizeof(SPathMsg));
			m_pathMsgMutex.unlock();
			break;

		case MSG_ID_PATH_CORRECTION:
			m_pathCorrectionMsgMutex.lock();
			memcpy(p_getMsgBuffer, &m_pathCorrectionMsg, sizeof(SPathCorrectionMsg));
			m_pathCorrectionMsgMutex.unlock();
			break;

		case MSG_ID_WORKSHOP_ORDER:
			m_workShopOrderMsgMutex.lock();
			memcpy(p_getMsgBuffer, &m_workShopOrderMsg, sizeof(SWorkShopOrderMsg));
			m_workShopOrderMsgMutex.unlock();
			break;

		case MSG_ID_STOP:
			m_stopMsgMutex.lock();
			memcpy(p_getMsgBuffer, &m_stopMsg, sizeof(SStopMsg));
			m_stopMsgMutex.unlock();
			break;

		case MSG_ID_WORKSHOP_REPORT:
			m_workShopReportMsgMutex.lock();
			memcpy(p_getMsgBuffer, &m_workShopReportMsg, sizeof(SWorkShopReportMsg));
			m_workShopReportMsgMutex.unlock();
			break;

		case MSG_ID_BIT_REPORT:
			m_bitReportMsgMutex.lock();
			memcpy(p_getMsgBuffer, &m_bitReportMsg, sizeof(SBitReportMsg));
			m_bitReportMsgMutex.unlock();
			break;

		case MSG_ID_ERROR:
			m_errorMsgMutex.lock();
			memcpy(p_getMsgBuffer, &m_errorMsg, sizeof(SErrorMsg));
			m_errorMsgMutex.unlock();
			break;

		default:
			break;
	}

	return 1;
}
