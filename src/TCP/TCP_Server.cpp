/**
 * "TCP_Server.cpp"
 **/



#include "TCP_Server.hpp"



using namespace std;



TCP::CTcpServer::CTcpServer()
{
    m_serverSocketAddrSize						= 0;
    m_serverIpAddress 							= TCP_SERVER_IP_ADDRESS;
    m_serverSocketPort							= TCP_SERVER_PORT;
    m_clientCounter 							= 0;
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
	m_serverSocketStatus 						= 1;
	for(uint32_t iClient=0 ; iClient<SOMAXCONN ; iClient++)
	{
		m_msgReportBit[iClient]					= l_msgReportBit;
		m_msgReportWorkShop[iClient]			= l_msgReportWorkShop;
		m_clientSocketStatus[iClient]			= 0;
		m_clientJoinThreadsStatus[iClient] 		= 1;
	}
	m_allClientsJoinThreadsStatus 				= 1;
}



TCP::CTcpServer::CTcpServer(int p_serverSocketPort, std::string p_serverSocketIpAddr)
{
    m_serverSocketAddrSize						= 0;
    m_serverIpAddress 							= p_serverSocketIpAddr;
    m_serverSocketPort							= p_serverSocketPort;
    m_clientCounter 							= 0;
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
	m_serverSocketStatus 						= 1;
	for(uint32_t iClient=0 ; iClient<SOMAXCONN ; iClient++)
	{
		m_msgReportBit[iClient]					= l_msgReportBit;
		m_msgReportWorkShop[iClient]			= l_msgReportWorkShop;
		m_clientSocketStatus[iClient]			= 0;
		m_clientJoinThreadsStatus[iClient] 		= 1;
	}
	m_allClientsJoinThreadsStatus 				= 1;
}



TCP::CTcpServer::~CTcpServer()
{
	uint32_t 	l_allClientsJoinThreadsStatus	= 0;
	uint32_t 	l_sleepTimeUs 					= 1000000;	// 1 sec

	if(m_serverSocketStatus != 0)
	{
		logDebug("Start the TCP server closing process");
	    // Disable sends and receives and close the client socket
		for(uint32_t iClient=0 ; iClient<SOMAXCONN ; iClient++)
		{
			if (m_clientSocketStatus[iClient] != 0)
			{
			    m_clientSocketStatus[iClient] = 0;
				shutdown(m_clientSocket[iClient], SHUT_RDWR);
			    close(m_clientSocket[iClient]);
			}
		}

		// Disable sends and receives and close the server socket
		shutdown(m_serverSocket, SHUT_RDWR);
	    close(m_serverSocket);
		m_serverSocketStatus = 0;

		// Wait for threads to terminate
		std::scoped_lock lock(m_mutexAllClientsJoinThreadsStatus);
		{
			l_allClientsJoinThreadsStatus = m_allClientsJoinThreadsStatus;
		}
		while(l_allClientsJoinThreadsStatus == 0)
		{
			std::scoped_lock lock(m_mutexAllClientsJoinThreadsStatus);
			{
				l_allClientsJoinThreadsStatus = m_allClientsJoinThreadsStatus;
			}
		}
		usleep(l_sleepTimeUs);

		logDebug("TCP server closed");
	}
}



int TCP::CTcpServer::initTcpServer()
{
	int l_ret = -1;

	// Create the TCP server socket
	m_serverSocket = socket(AF_INET, SOCK_STREAM, 0);
	if(m_serverSocket != -1)
	{
		// Convert an IP address from textual to binary format
		m_serverSocketAddr.sin_family	= AF_INET;
		m_serverSocketAddr.sin_port 	= htons(m_serverSocketPort);
		if(inet_pton(AF_INET, m_serverIpAddress.c_str(), &m_serverSocketAddr.sin_addr) != 1)
		{
			cerr << "> Can't convert the Internet address! Quitting" << endl;
		}
		else
		{
			// Assign an address and a port to the TCP server socket
			m_serverSocketAddrSize = sizeof(m_serverSocketAddr);
			if(bind(m_serverSocket, (sockaddr*)&m_serverSocketAddr, m_serverSocketAddrSize) != -1)
			{
				// Enable the TCP server socket to accept client connections
				if(listen(m_serverSocket, SOMAXCONN) != -1)
				{
					// Start the TCP server
					l_ret = m_serverSocket;
					srand((unsigned) time(0));
					m_threadStart = thread(&TCP::CTcpServer::threadStartTcpServer, this);
					m_threadStart.detach();
				}
			}
		}
	}

	return l_ret;
}



void TCP::CTcpServer::threadStartTcpServer()
{
	logDebug("threadStartTcpServer (begin)");

    socklen_t 	l_clientSocketAddrSize;

    // Accept any client connection
	while((m_clientCounter < SOMAXCONN) && (m_serverSocket != -1))
	{
		// Wait for a client connection
			l_clientSocketAddrSize				= sizeof(sockaddr_in);
			m_clientSocket[m_clientCounter] 	= accept(m_serverSocket, (sockaddr*)&m_clientSocketAddr[m_clientCounter], &l_clientSocketAddrSize);

		// Treat a client connection
			if(m_clientSocket[m_clientCounter] != -1)
			{
				#if LOG_ON
				cout << "> Client connected : socketValue = " << m_clientSocket[m_clientCounter] << endl;
				#endif

				// Translate the client socket address to a location and a service name
					memset(m_clientName[m_clientCounter], 0, NI_MAXHOST);
					memset(m_clientPort[m_clientCounter], 0, NI_MAXSERV);
					if(getnameinfo((sockaddr*)&m_clientSocketAddr[m_clientCounter], sizeof(m_clientSocketAddr[m_clientCounter]), m_clientName[m_clientCounter], NI_MAXHOST, m_clientPort[m_clientCounter], NI_MAXSERV, 0) != 0)
					{
						inet_ntop(AF_INET, &m_clientSocketAddr[m_clientCounter].sin_addr, m_clientName[m_clientCounter], NI_MAXHOST);
					}

				// Create the client threads
					// Launch the thread sending periodic information to the client
						m_clientSocketStatus[m_clientCounter] 		= 1;
						m_clientJoinThreadsStatus[m_clientCounter] 	= 0;
						{
							std::scoped_lock lock(m_mutexAllClientsJoinThreadsStatus);
							m_allClientsJoinThreadsStatus 			= 0;
						}

						m_threadPeriodicSendToClientMsgInfoKeepAlive[m_clientCounter]	= thread(&TCP::CTcpServer::threadPeriodicSendToClientMsgInfoKeepAlive, 	this, m_clientCounter);
						m_threadPeriodicSendToClientMsgInfoPosition[m_clientCounter] 	= thread(&TCP::CTcpServer::threadPeriodicSendToClientMsgInfoPosition, 	this, m_clientCounter);

					// Launch the thread waiting for a client request
						m_threadTreatClientMsg[m_clientCounter] = thread(&TCP::CTcpServer::threadTreatClientMsg, this, m_clientCounter);

				// Update the client counter
					m_clientCounter++;
			}
			else
			{
				logDebug("> Server socket is closed (disable any client connection)");

				break;
			}
	}

	// Join all thread
	logDebug("Wait for all clients threads to join");
	for(uint32_t iClient=0 ; iClient<SOMAXCONN ; iClient++)
	{
		if(m_clientJoinThreadsStatus[iClient] != 1)
		{
			#if LOG_ON
			cout << "> Threads joigning for client " << iClient << endl;
			#endif
			m_threadPeriodicSendToClientMsgInfoKeepAlive[iClient].join();
			m_threadPeriodicSendToClientMsgInfoPosition[iClient].join();
			m_threadTreatClientMsg[iClient].join();
			m_clientJoinThreadsStatus[m_clientCounter] = 1;
			#if LOG_ON
			cout << "> Thread joigned for client " << iClient << endl;
			#endif
		}
	}
	{
		std::scoped_lock lock(m_mutexAllClientsJoinThreadsStatus);
		m_allClientsJoinThreadsStatus = 1;
	}
	logDebug("All clients threads joigned");
	logDebug("threadStartTcpServer (end)");
}



void TCP::CTcpServer::threadPeriodicSendToClientMsgInfoKeepAlive(uint32_t p_clientId)
{
	logDebug("threadPeriodicSendToClientMsgInfoKeepAlive (begin)");

	EIdMsg		l_periodicMsgId = ID_MSG_INFO_KEEP_ALIVE;
	uint32_t 	l_sleepTimeUs 	= 5000000;					// 2 sec
	int32_t 	l_ret 			= 1;

	// Send a periodic message to the client
	while((m_clientSocketStatus[p_clientId] == 1) && (l_ret == 1))
	{
		l_ret = this->sendMsgToClient(l_periodicMsgId, p_clientId);
		usleep(l_sleepTimeUs);
	}

	logDebug("threadPeriodicSendToClientMsgInfoKeepAlive (end)");
}



void TCP::CTcpServer::threadPeriodicSendToClientMsgInfoPosition(uint32_t p_clientId)
{
	logDebug("threadPeriodicSendToClientMsgInfoPosition (begin)");

	EIdMsg		l_periodicMsgId = ID_MSG_INFO_POSITION;
	uint32_t 	l_sleepTimeUs 	= 2000000;					// 1 sec
	int32_t 	l_ret 			= 1;

	// Send a periodic message to the client
	while((m_clientSocketStatus[p_clientId] == 1) && (l_ret == 1))
	{
		l_ret = this->sendMsgToClient(l_periodicMsgId, p_clientId);
		this->tmpUpdateMsgInfoPosition();					// AHU : mise a jour aleatoire temporaire (normalement mis a jour apres traitement d'image du mat sur le plan de jeu)
		usleep(l_sleepTimeUs);
	}

	logDebug("threadPeriodicSendToClientMsgInfoPosition (end)");
}



void TCP::CTcpServer::threadTreatClientMsg(uint32_t p_clientId)
{
	logDebug("threadTreatClientMsg (begin)");

	SMsgHeader 	l_msgHeader;
	int			l_readBytesSize;

	while(m_clientSocketStatus[p_clientId] == 1)
	{
		// Read the message header
		l_readBytesSize = recv(m_clientSocket[p_clientId], &l_msgHeader, sizeof(SMsgHeader), 0);
		if(l_readBytesSize != -1)
		{
			// Read the message content
			switch(l_msgHeader.id)
			{
				case ID_MSG_REPORT_BIT:
					{
						std::scoped_lock lock(m_mutexMsgReportBit[p_clientId]);
						l_readBytesSize = recv(m_clientSocket[p_clientId], &m_msgReportBit[p_clientId].body, sizeof(SMsgReportBitBody), 0);
						displayMsgReportBit(&m_msgReportBit[p_clientId]);
					}
					break;

				case ID_MSG_REPORT_WORKSHOP:
					{
						std::scoped_lock lock(m_mutexMsgReportWorkShop[p_clientId]);
						l_readBytesSize = recv(m_clientSocket[p_clientId], &m_msgReportWorkShop[p_clientId].body, sizeof(SMsgReportWorkShopBody), 0);
						displayMsgReportWorkShop(&m_msgReportWorkShop[p_clientId]);
					}
					break;

				default:
					break;
			}
		}
	}

	logDebug("threadTreatClientMsg (end)");
}



int TCP::CTcpServer::sendMsgToClient(uint32_t p_msgId, uint32_t p_clientId)
{
	if(m_clientSocketStatus[p_clientId] == 1)
	{
		switch(p_msgId)
		{
			case ID_MSG_INFO_KEEP_ALIVE:
				{
					std::scoped_lock lock(m_mutexMsgInfoKeepAlive);
					if(send(m_clientSocket[p_clientId], &m_msgInfoKeepAlive, sizeof(SMsgInfoKeepAlive), 0) == -1)
					{
						return -1;
					}
					displayMsgInfoKeepAlive(&m_msgInfoKeepAlive);
				}
				break;

			case ID_MSG_INFO_POSITION:
				{
					std::scoped_lock lock(m_mutexMsgInfoPosition);
					if(send(m_clientSocket[p_clientId], &m_msgInfoPosition, sizeof(SMsgInfoPosition), 0) == -1)
					{
						return -1;
					}
					displayMsgInfoPosition(&m_msgInfoPosition);
				}
				break;

			case ID_MSG_ORDER_BIT:
				{
					std::scoped_lock lock(m_mutexMsgOrderBit);
					if(send(m_clientSocket[p_clientId], &m_msgOrderBit, sizeof(SMsgOrderBit), 0) == -1)
					{
						return -1;
					}
					displayMsgOrderBit(&m_msgOrderBit);
				}
				break;

			case ID_MSG_ORDER_PATH:
				{
					std::scoped_lock lock(m_mutexMsgOrderPath);
					if(send(m_clientSocket[p_clientId], &m_msgOrderPath, sizeof(SMsgOrderPath), 0) == -1)
					{
						return -1;
					}
					displayMsgOrderPath(&m_msgOrderPath);
				}
				break;

			case ID_MSG_ORDER_PATH_CORR:
				{
					std::scoped_lock lock(m_mutexMsgOrderPathCorr);
					if(send(m_clientSocket[p_clientId], &m_msgOrderPathCorr, sizeof(SMsgOrderPathCorr), 0) == -1)
					{
						return -1;
					}
					displayMsgOrderPathCorr(&m_msgOrderPathCorr);
				}
				break;

			case ID_MSG_ORDER_WORKSHOP:
				{
					std::scoped_lock lock(m_mutexMsgOrderWorkShop);
					if(send(m_clientSocket[p_clientId], &m_msgOrderWorkShop, sizeof(SMsgOrderWorkShop), 0) == -1)
					{
						return -1;
					}
					displayMsgOrderWorkShop(&m_msgOrderWorkShop);
				}
				break;

			case ID_MSG_ORDER_STOP:
				{
					std::scoped_lock lock(m_mutexMsgOrderStop);
					if(send(m_clientSocket[p_clientId], &m_msgOrderStop, sizeof(SMsgOrderStop), 0) == -1)
					{
						return -1;
					}
					displayMsgOrderStop(&m_msgOrderStop);
				}
				break;

			default:
				break;
		}
	}
	else
	{
		return -1;
	}

	return 1;
}



void TCP::CTcpServer::updateMsg(uint32_t p_updateIdMsg, void* p_updateMsgBuffer)
{
	#if LOG_ON
	cout << "> Update message : " << (EIdMsg) p_updateIdMsg << endl;
	#endif

	switch(p_updateIdMsg)
	{
		case ID_MSG_INFO_KEEP_ALIVE:
			{
				std::scoped_lock lock(m_mutexMsgInfoKeepAlive);
				memcpy(&m_msgInfoKeepAlive.body, p_updateMsgBuffer, sizeof(SMsgInfoKeepAliveBody));
				displayMsgInfoKeepAlive(&m_msgInfoKeepAlive);
			}
			break;

		case ID_MSG_INFO_POSITION:
			{
				std::scoped_lock lock(m_mutexMsgInfoPosition);
				memcpy(&m_msgInfoPosition.body, p_updateMsgBuffer, sizeof(SMsgInfoPositionBody));
				displayMsgInfoPosition(&m_msgInfoPosition);
			}
			break;

		/*
		 * Message with no content to be updated
		case ID_MSG_ORDER_BIT:
			{
				std::scoped_lock lock(m_mutexMsgOrderBit);
				memcpy(&m_msgOrderBit.body, p_updateMsgBuffer, sizeof(SMsgOrderBit));
				displayMsgOrderBit(&m_msgOrderBit);
			}
			break;
		*/

		case ID_MSG_ORDER_PATH:
			{
				std::scoped_lock lock(m_mutexMsgOrderPath);
				memcpy(&m_msgOrderPath.body, p_updateMsgBuffer, sizeof(SMsgOrderPathBody));
				displayMsgOrderPath(&m_msgOrderPath);
			}
			break;

		case ID_MSG_ORDER_PATH_CORR:
			{
				std::scoped_lock lock(m_mutexMsgOrderPathCorr);
				memcpy(&m_msgOrderPathCorr.body, p_updateMsgBuffer, sizeof(SMsgOrderPathCorrBody));
				displayMsgOrderPathCorr(&m_msgOrderPathCorr);
			}
			break;

		case ID_MSG_ORDER_WORKSHOP:
			{
				std::scoped_lock lock(m_mutexMsgOrderWorkShop);
				memcpy(&m_msgOrderWorkShop.body, p_updateMsgBuffer, sizeof(SMsgOrderWorkShopBody));
				displayMsgOrderWorkShop(&m_msgOrderWorkShop);
			}
			break;

		/*
		 * Message with no content to be updated
		case ID_MSG_ORDER_STOP:
			{
				std::scoped_lock lock(m_mutexMsgOrderStop);
				memcpy(&m_msgOrderStop.body, p_updateMsgBuffer, sizeof(SMsgOrderStopBody));
				displayMsgOrderStop(&m_msgOrderStop);
			}
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
			{
				std::scoped_lock lock(m_mutexMsgInfoKeepAlive);
				memcpy(p_getMsgBuffer, &m_msgInfoKeepAlive, sizeof(SMsgInfoKeepAlive));
			}
			break;

		case ID_MSG_INFO_POSITION:
			{
				std::scoped_lock lock(m_mutexMsgInfoPosition);
				memcpy(p_getMsgBuffer, &m_msgInfoPosition, sizeof(SMsgInfoPosition));
			}
			break;

		case ID_MSG_ORDER_BIT:
			{
				std::scoped_lock lock(m_mutexMsgOrderBit);
				memcpy(p_getMsgBuffer, &m_msgOrderBit, sizeof(SMsgOrderBit));
			}
			break;

		case ID_MSG_ORDER_PATH:
			{
				std::scoped_lock lock(m_mutexMsgOrderPath);
				memcpy(p_getMsgBuffer, &m_msgOrderPath, sizeof(SMsgOrderPath));
			}
			break;

		case ID_MSG_ORDER_PATH_CORR:
			{
				std::scoped_lock lock(m_mutexMsgOrderPathCorr);
				memcpy(p_getMsgBuffer, &m_msgOrderPathCorr, sizeof(SMsgOrderPathCorr));
			}
			break;

		case ID_MSG_ORDER_WORKSHOP:
			{
				std::scoped_lock lock(m_mutexMsgOrderWorkShop);
				memcpy(p_getMsgBuffer, &m_msgOrderWorkShop, sizeof(SMsgOrderWorkShop));
			}
			break;

		case ID_MSG_ORDER_STOP:
			{
				std::scoped_lock lock(m_mutexMsgOrderStop);
				memcpy(p_getMsgBuffer, &m_msgOrderStop, sizeof(SMsgOrderStop));
			}
			break;

		case ID_MSG_REPORT_WORKSHOP:
			{
				std::scoped_lock lock(m_mutexMsgReportWorkShop[0]);
				memcpy(p_getMsgBuffer, &m_msgReportWorkShop[0], sizeof(SMsgReportWorkShop));
			}
			break;

		case ID_MSG_REPORT_BIT:
			{
				std::scoped_lock lock(m_mutexMsgReportBit[0]);
				memcpy(p_getMsgBuffer, &m_msgReportBit[0], sizeof(SMsgReportBit));
			}
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

	{
		std::scoped_lock lock(m_mutexMsgInfoPosition);
		m_msgInfoPosition.body.coordinates.x 	= xRand;
		m_msgInfoPosition.body.coordinates.y 	= yRand;
		m_msgInfoPosition.body.angle 			= angleRand;
	}
}
