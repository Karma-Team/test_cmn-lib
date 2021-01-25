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
	SMsgInfoKeepAlive 		l_msgInfoKeepAlive	= {.hd={ID_MSG_INFO_KEEP_ALIVE,	sizeof(SMsgInfoKeepAlive)}, 	.body={true}};
	SMsgInfoPosition 		l_msgInfoPosition 	= {.hd={ID_MSG_INFO_POSITION, 	sizeof(SMsgInfoPosition)}, 		.body={0, 0}};
	SMsgOrderBit			l_msgOrderBit		= {.hd={ID_MSG_ORDER_BIT, 		sizeof(SMsgOrderBit)}, 			.body={0}};
	SMsgOrderPath 			l_msgOrderPath 		= {.hd={ID_MSG_ORDER_PATH,		sizeof(SMsgOrderPath)}, 		.body={0, {0,0,0,0,0,0,0,0,0,0}}};
	SMsgOrderPathCorr		l_msgOrderPathCorr	= {.hd={ID_MSG_ORDER_PATH_CORR, sizeof(SMsgOrderPathCorr)}, 	.body={0, 0, {0,0,0,0,0,0,0,0,0,0}}};
	SMsgOrderWorkShop 		l_msgOrderWorkShop	= {.hd={ID_MSG_ORDER_WORKSHOP, 	sizeof(SMsgOrderWorkShop)}, 	.body={0}};
	SMsgOrderStop 			l_msgOrderStop		= {.hd={ID_MSG_ORDER_STOP,		sizeof(SMsgOrderStop)}, 		.body={0}};
	SMsgReportBit 			l_msgReportBit		= {.hd={ID_MSG_REPORT_BIT, 		sizeof(SMsgReportBit)}, 		.body={0}};
	SMsgReportWorkShop 		l_msgReportWorkShop	= {.hd={ID_MSG_REPORT_WORKSHOP, sizeof(SMsgReportWorkShop)},	.body={0}};
	m_msgInfoKeepAlive 							= l_msgInfoKeepAlive;
	m_msgInfoPosition 							= l_msgInfoPosition;
	m_msgOrderBit 								= l_msgOrderBit;
	m_msgOrderPath 								= l_msgOrderPath;
	m_msgOrderPathCorr 							= l_msgOrderPathCorr;
	m_msgOrderWorkShop							= l_msgOrderWorkShop;
	m_msgOrderStop								= l_msgOrderStop;
	for(uint32_t iClient=0 ; iClient<SOMAXCONN ; iClient++)
	{
		m_msgReportBit[iClient]					= l_msgReportBit;
		m_msgReportWorkShop[iClient]			= l_msgReportWorkShop;
		m_serverClientStatus[iClient]			= 0;
	}
}



TCP::CTcpServer::CTcpServer(int p_serverSocketPort, string p_serverSocketIpAddr)
{
    m_serverSocketAddrSize						= 0;
    m_serverIpAddress 							= p_serverSocketIpAddr;
    m_serverSocketPort							= p_serverSocketPort;
    m_serverClientCounter 						= 0;
	m_serverSocket								= -1;
	SMsgInfoKeepAlive 		l_msgInfoKeepAlive	= {.hd={ID_MSG_INFO_KEEP_ALIVE,	sizeof(SMsgInfoKeepAlive)}, 	.body={true}};
	SMsgInfoPosition 		l_msgInfoPosition 	= {.hd={ID_MSG_INFO_POSITION, 	sizeof(SMsgInfoPosition)}, 		.body={0, 0}};
	SMsgOrderBit			l_msgOrderBit		= {.hd={ID_MSG_ORDER_BIT, 		sizeof(SMsgOrderBit)}, 			.body={0}};
	SMsgOrderPath 			l_msgOrderPath 		= {.hd={ID_MSG_ORDER_PATH,		sizeof(SMsgOrderPath)}, 		.body={0, {0,0,0,0,0,0,0,0,0,0}}};
	SMsgOrderPathCorr		l_msgOrderPathCorr	= {.hd={ID_MSG_ORDER_PATH_CORR, sizeof(SMsgOrderPathCorr)}, 	.body={0, 0, {0,0,0,0,0,0,0,0,0,0}}};
	SMsgOrderWorkShop 		l_msgOrderWorkShop	= {.hd={ID_MSG_ORDER_WORKSHOP, 	sizeof(SMsgOrderWorkShop)}, 	.body={0}};
	SMsgOrderStop 			l_msgOrderStop		= {.hd={ID_MSG_ORDER_STOP,		sizeof(SMsgOrderStop)}, 		.body={0}};
	SMsgReportWorkShop 		l_msgReportWorkShop	= {.hd={ID_MSG_REPORT_WORKSHOP, sizeof(SMsgReportWorkShop)},	.body={0}};
	SMsgReportBit 			l_msgReportBit		= {.hd={ID_MSG_REPORT_BIT, 		sizeof(SMsgReportBit)}, 		.body={0}};
	m_msgInfoKeepAlive 							= l_msgInfoKeepAlive;
	m_msgInfoPosition 							= l_msgInfoPosition;
	m_msgOrderBit 								= l_msgOrderBit;
	m_msgOrderPath 								= l_msgOrderPath;
	m_msgOrderPathCorr 							= l_msgOrderPathCorr;
	m_msgOrderWorkShop							= l_msgOrderWorkShop;
	m_msgOrderStop								= l_msgOrderStop;
	for(uint32_t iClient=0 ; iClient<SOMAXCONN ; iClient++)
	{
		m_msgReportBit[iClient]					= l_msgReportBit;
		m_msgReportWorkShop[iClient]			= l_msgReportWorkShop;
	}
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
		m_threadStart = thread(&TCP::CTcpServer::threadStartTcpServer, this);

	return m_serverSocket;
}



void TCP::CTcpServer::closeTcpServer()
{
	// AHU : stop the threads : "m_threadPeriodicSendToClientMsgInfoKeepAlive", "m_threadPeriodicSendToClientMsgInfoPosition", "m_threadTreatClientMsg" and "m_threadCientConnexion"

    // Disable sends and receives and close the client socket
	for(uint32_t iClient=0 ; iClient<SOMAXCONN ; iClient++)
	{
		if(m_serverClientStatus[iClient] != 0)
		{
			shutdown(m_clientSocket[iClient], SHUT_RDWR);
		    close(m_clientSocket[iClient]);
		}
	}

	shutdown(m_serverSocket, SHUT_RDWR);
    close(m_serverSocket);
}



void TCP::CTcpServer::threadStartTcpServer()
{
	#if TCP_DEBUG
	//cout << "> threadStartTcpServer (begin)" << endl;
	#endif

    socklen_t 	l_clientSocketAddrSize;

	if(m_serverSocket != -1)
	{
		while(m_serverClientCounter < SOMAXCONN)
		{
			// Wait for a client connection
				l_clientSocketAddrSize					= sizeof(sockaddr_in);
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
					m_serverClientStatus[m_serverClientCounter] 						= 1;
					m_threadPeriodicSendToClientMsgInfoKeepAlive[m_serverClientCounter]	= thread(&TCP::CTcpServer::threadPeriodicSendToClientMsgInfoKeepAlive, 	this, m_serverClientCounter);
					m_threadPeriodicSendToClientMsgInfoPosition[m_serverClientCounter] 	= thread(&TCP::CTcpServer::threadPeriodicSendToClientMsgInfoPosition, 	this, m_serverClientCounter);

				// Launch the thread waiting for a client request
					m_threadTreatClientMsg[m_serverClientCounter] = thread(&TCP::CTcpServer::threadTreatClientMsg, this, m_serverClientCounter);

			// Update the client counter
				m_serverClientCounter++;
		}
	}
	else
	{
		cerr << "> TCP server is not initialized! Quitting" << endl;
	}

	#if TCP_DEBUG
	cout << "> threadStartTcpServer (end)" << endl;
	#endif
}



void TCP::CTcpServer::threadPeriodicSendToClientMsgInfoKeepAlive(uint32_t p_clientId)
{
	#if TCP_DEBUG
	//cout << "> threadPeriodicSendToClientMsgInfoKeepAlive (begin)" << endl;
	#endif

	EIdMsg		l_periodicMsgId = ID_MSG_INFO_KEEP_ALIVE;
	uint32_t 	l_sleepTimeUs 	= 5000000;					// 5 sec

	// Send a periodic message to the client
	while(m_serverClientStatus[p_clientId] == 1)
	{
		this->sendMsgToClient(l_periodicMsgId, p_clientId);
		usleep(l_sleepTimeUs);
	}

	#if TCP_DEBUG
	cout << "> threadPeriodicSendToClientMsgInfoKeepAlive (end)" << endl;
	#endif
}



void TCP::CTcpServer::threadPeriodicSendToClientMsgInfoPosition(uint32_t p_clientId)
{
	#if TCP_DEBUG
	//cout << "> threadPeriodicSendToClientMsgInfoPosition (begin)" << endl;
	#endif

	EIdMsg		l_periodicMsgId = ID_MSG_INFO_POSITION;
	uint32_t 	l_sleepTimeUs 	= 2000000;					// 2 sec

	// Send a periodic message to the client
	while(m_serverClientStatus[p_clientId] == 1)
	{
		this->sendMsgToClient(l_periodicMsgId, p_clientId);
		this->tmpUpdateMsgInfoPosition();					// AHU : mise a jour aleatoire temporaire (normalement mis a jour apres traitement d'image du mat sur le plan de jeu)
		usleep(l_sleepTimeUs);
	}

	#if TCP_DEBUG
	cout << "> threadPeriodicSendToClientMsgInfoPosition (end)" << endl;
	#endif
}



void TCP::CTcpServer::threadTreatClientMsg(uint32_t p_clientId)
{
	#if TCP_DEBUG
	//cout << "> threadTreatClientMsg (begin)" << endl;
	#endif

	SMsgHeader 	l_msgHeader;
	int			l_readBytesSize;

	while(m_serverClientStatus[p_clientId] == 1)
	{
		// Read the message header
		l_readBytesSize = recv(m_clientSocket[p_clientId], &l_msgHeader, sizeof(SMsgHeader), 0);
		if(l_readBytesSize != -1)
		{
			// Read the message content
			switch(l_msgHeader.id)
			{
				case ID_MSG_REPORT_BIT:
					m_mutexMsgReportBit[p_clientId].lock();
					l_readBytesSize = recv(m_clientSocket[p_clientId], &m_msgReportBit[p_clientId].body, sizeof(SMsgReportBitBody), 0);
					#if TCP_DEBUG
					displayMsgReportBit(&m_msgReportBit[p_clientId]);
					#endif
					m_mutexMsgReportBit[p_clientId].unlock();
					break;

				case ID_MSG_REPORT_WORKSHOP:
					m_mutexMsgReportWorkShop[p_clientId].lock();
					l_readBytesSize = recv(m_clientSocket[p_clientId], &m_msgReportWorkShop[p_clientId].body, sizeof(SMsgReportWorkShopBody), 0);
					#if TCP_DEBUG
					displayMsgReportWorkShop(&m_msgReportWorkShop[p_clientId]);
					#endif
					m_mutexMsgReportWorkShop[p_clientId].unlock();
					break;

				default:
					break;
			}
		}
	}

	#if TCP_DEBUG
	cout << "> threadTreatClientMsg (end)" << endl;
	#endif
}



int TCP::CTcpServer::sendMsgToClient(uint32_t p_msgId, uint32_t p_clientId)
{
	if(m_serverClientStatus[p_clientId] == 1)
	{
		switch(p_msgId)
		{
			case ID_MSG_INFO_KEEP_ALIVE:
				m_mutexMsgInfoKeepAlive.lock();
				if(send(m_clientSocket[p_clientId], &m_msgInfoKeepAlive, sizeof(SMsgInfoKeepAlive), 0) == -1)
				{
					return -1;
				}
				#if TCP_DEBUG
				displayMsgInfoKeepAlive(&m_msgInfoKeepAlive);
				#endif
				m_mutexMsgInfoKeepAlive.unlock();
				break;

			case ID_MSG_INFO_POSITION:
				m_mutexMsgInfoPosition.lock();
				if(send(m_clientSocket[p_clientId], &m_msgInfoPosition, sizeof(SMsgInfoPosition), 0) == -1)
				{
					return -1;
				}
				#if TCP_DEBUG
				displayMsgInfoPosition(&m_msgInfoPosition);
				#endif
				m_mutexMsgInfoPosition.unlock();
				break;

			case ID_MSG_ORDER_BIT:
				m_mutexMsgOrderBit.lock();
				if(send(m_clientSocket[p_clientId], &m_msgOrderBit, sizeof(SMsgOrderBit), 0) == -1)
				{
					return -1;
				}
				#if TCP_DEBUG
				displayMsgOrderBit(&m_msgOrderBit);
				#endif
				m_mutexMsgOrderBit.unlock();
				break;

			case ID_MSG_ORDER_PATH:
				m_mutexMsgOrderPath.lock();
				if(send(m_clientSocket[p_clientId], &m_msgOrderPath, sizeof(SMsgOrderPath), 0) == -1)
				{
					return -1;
				}
				#if TCP_DEBUG
				displayMsgOrderPath(&m_msgOrderPath);
				#endif
				m_mutexMsgOrderPath.unlock();
				break;

			case ID_MSG_ORDER_PATH_CORR:
				m_mutexMsgOrderPathCorr.lock();
				if(send(m_clientSocket[p_clientId], &m_msgOrderPathCorr, sizeof(SMsgOrderPathCorr), 0) == -1)
				{
					return -1;
				}
				#if TCP_DEBUG
				displayMsgOrderPathCorr(&m_msgOrderPathCorr);
				#endif
				m_mutexMsgOrderPathCorr.unlock();
				break;

			case ID_MSG_ORDER_WORKSHOP:
				m_mutexMsgOrderWorkShop.lock();
				if(send(m_clientSocket[p_clientId], &m_msgOrderWorkShop, sizeof(SMsgOrderWorkShop), 0) == -1)
				{
					return -1;
				}
				#if TCP_DEBUG
				displayMsgOrderWorkShop(&m_msgOrderWorkShop);
				#endif
				m_mutexMsgOrderWorkShop.unlock();
				break;

			case ID_MSG_ORDER_STOP:
				m_mutexMsgOrderStop.lock();
				if(send(m_clientSocket[p_clientId], &m_msgOrderStop, sizeof(SMsgOrderStop), 0) == -1)
				{
					return -1;
				}
				#if TCP_DEBUG
				displayMsgOrderStop(&m_msgOrderStop);
				#endif
				m_mutexMsgOrderStop.unlock();
				break;

			default:
				break;
		}
	}

	return 1;
}



void TCP::CTcpServer::updateMsg(uint32_t p_updateIdMsg, void* p_updateMsgBuffer)
{
	#if TCP_DEBUG
	cout << "> Update message : " << (EIdMsg) p_updateIdMsg << endl;
	#endif

	switch(p_updateIdMsg)
	{
		case ID_MSG_INFO_KEEP_ALIVE:
			m_mutexMsgInfoKeepAlive.lock();
			memcpy(&m_msgInfoKeepAlive.body, p_updateMsgBuffer, sizeof(SMsgInfoKeepAliveBody));
			#if TCP_DEBUG
			displayMsgInfoKeepAlive(&m_msgInfoKeepAlive);
			#endif
			m_mutexMsgInfoKeepAlive.unlock();
			break;

		case ID_MSG_INFO_POSITION:
			m_mutexMsgInfoPosition.lock();
			memcpy(&m_msgInfoPosition.body, p_updateMsgBuffer, sizeof(SMsgInfoPositionBody));
			#if TCP_DEBUG
			displayMsgInfoPosition(&m_msgInfoPosition);
			#endif
			m_mutexMsgInfoPosition.unlock();
			break;

		/*
		 * Message with no content to be updated
		case ID_MSG_ORDER_BIT:
			m_mutexMsgOrderBit.lock();
			memcpy(&m_msgOrderBit.body, p_updateMsgBuffer, sizeof(SMsgOrderBit));
			#if TCP_DEBUG
			displayMsgOrderBit(&m_msgOrderBit);
			#endif
			m_mutexMsgOrderBit.unlock();
			break;
		*/

		case ID_MSG_ORDER_PATH:
			m_mutexMsgOrderPath.lock();
			memcpy(&m_msgOrderPath.body, p_updateMsgBuffer, sizeof(SMsgOrderPathBody));
			#if TCP_DEBUG
			displayMsgOrderPath(&m_msgOrderPath);
			#endif
			m_mutexMsgOrderPath.unlock();
			break;

		case ID_MSG_ORDER_PATH_CORR:
			m_mutexMsgOrderPathCorr.lock();
			memcpy(&m_msgOrderPathCorr.body, p_updateMsgBuffer, sizeof(SMsgOrderPathCorrBody));
			#if TCP_DEBUG
			displayMsgOrderPathCorr(&m_msgOrderPathCorr);
			#endif
			m_mutexMsgOrderPathCorr.unlock();
			break;

		case ID_MSG_ORDER_WORKSHOP:
			m_mutexMsgOrderWorkShop.lock();
			memcpy(&m_msgOrderWorkShop.body, p_updateMsgBuffer, sizeof(SMsgOrderWorkShopBody));
			#if TCP_DEBUG
			displayMsgOrderWorkShop(&m_msgOrderWorkShop);
			#endif
			m_mutexMsgOrderWorkShop.unlock();
			break;

		/*
		 * Message with no content to be updated
		case ID_MSG_ORDER_STOP:
			m_mutexMsgOrderStop.lock();
			memcpy(&m_msgOrderStop.body, p_updateMsgBuffer, sizeof(SMsgOrderStopBody));
			#if TCP_DEBUG
			displayMsgOrderStop(&m_msgOrderStop);
			#endif
			m_mutexMsgOrderStop.unlock();
			break;
		*/

		default:
			break;
	}
}



void TCP::CTcpServer::getMsg(uint32_t p_getMsgId, void* p_getMsgBuffer)
{
	switch(p_getMsgId)
	{
		case ID_MSG_INFO_KEEP_ALIVE:
			m_mutexMsgInfoKeepAlive.lock();
			memcpy(p_getMsgBuffer, &m_msgInfoKeepAlive, sizeof(SMsgInfoKeepAlive));
			m_mutexMsgInfoKeepAlive.unlock();
			break;

		case ID_MSG_INFO_POSITION:
			m_mutexMsgInfoPosition.lock();
			memcpy(p_getMsgBuffer, &m_msgInfoPosition, sizeof(SMsgInfoPosition));
			m_mutexMsgInfoPosition.unlock();
			break;

		case ID_MSG_ORDER_BIT:
			m_mutexMsgOrderBit.lock();
			memcpy(p_getMsgBuffer, &m_msgOrderBit, sizeof(SMsgOrderBit));
			m_mutexMsgOrderBit.unlock();
			break;

		case ID_MSG_ORDER_PATH:
			m_mutexMsgOrderPath.lock();
			memcpy(p_getMsgBuffer, &m_msgOrderPath, sizeof(SMsgOrderPath));
			m_mutexMsgOrderPath.unlock();
			break;

		case ID_MSG_ORDER_PATH_CORR:
			m_mutexMsgOrderPathCorr.lock();
			memcpy(p_getMsgBuffer, &m_msgOrderPathCorr, sizeof(SMsgOrderPathCorr));
			m_mutexMsgOrderPathCorr.unlock();
			break;

		case ID_MSG_ORDER_WORKSHOP:
			m_mutexMsgOrderWorkShop.lock();
			memcpy(p_getMsgBuffer, &m_msgOrderWorkShop, sizeof(SMsgOrderWorkShop));
			m_mutexMsgOrderWorkShop.unlock();
			break;

		case ID_MSG_ORDER_STOP:
			m_mutexMsgOrderStop.lock();
			memcpy(p_getMsgBuffer, &m_msgOrderStop, sizeof(SMsgOrderStop));
			m_mutexMsgOrderStop.unlock();
			break;

		case ID_MSG_REPORT_WORKSHOP:
			m_mutexMsgReportWorkShop[0].lock();
			memcpy(p_getMsgBuffer, &m_msgReportWorkShop[0], sizeof(SMsgReportWorkShop));
			m_mutexMsgReportWorkShop[0].unlock();
			break;

		case ID_MSG_REPORT_BIT:
			m_mutexMsgReportBit[0].lock();
			memcpy(p_getMsgBuffer, &m_msgReportBit[0], sizeof(SMsgReportBit));
			m_mutexMsgReportBit[0].unlock();
			break;

		default:
			break;
	}
}



void TCP::CTcpServer::tmpUpdateMsgInfoPosition()
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

	m_mutexMsgInfoPosition.lock();
	m_msgInfoPosition.body.coordinates.x 	= xRand;
	m_msgInfoPosition.body.coordinates.y 	= yRand;
	m_msgInfoPosition.body.angle 			= angleRand;
	m_mutexMsgInfoPosition.unlock();
}
