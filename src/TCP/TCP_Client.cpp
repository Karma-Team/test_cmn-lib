/**
 * "TCP_Client.cpp"
 **/



#include "TCP_Client.hpp"



TCP::CTcpClient::CTcpClient()
{
    m_serverIpAddress 							= TCP_SERVER_IP_ADDRESS;
	m_clientSocket								= -1;
    m_serverSocketPort							= TCP_SERVER_PORT;
	SMsgInfoKeepAlive 		l_msgInfoKeepAlive	= {.hd={ID_MSG_INFO_KEEP_ALIVE,	sizeof(SMsgInfoKeepAlive)}, 	.body={false}};
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
	m_msgReportBit								= l_msgReportBit;
	m_msgReportWorkShop							= l_msgReportWorkShop;
}



TCP::CTcpClient::CTcpClient(int p_serverSocketPort, string p_serverSocketIpAddr)
{
	m_serverIpAddress							= p_serverSocketIpAddr;
	m_clientSocket								= -1;
	m_serverSocketPort							= p_serverSocketPort;
	SMsgInfoKeepAlive 		l_msgInfoKeepAlive	= {.hd={ID_MSG_INFO_KEEP_ALIVE,	sizeof(SMsgInfoKeepAlive)}, 	.body={false}};
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
	m_msgReportBit								= l_msgReportBit;
	m_msgReportWorkShop							= l_msgReportWorkShop;
}



TCP::CTcpClient::~CTcpClient()
{
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
		m_threadReceiveMsgFromServer = thread(&TCP::CTcpClient::threadReceiveMsgFromServer, this);

	return m_clientSocket;
}



void TCP::CTcpClient::closeTcpClient()
{
	// AHU : stop the thread "threadReceiveMsgFromServer"

    // Disable sends and receives and close the client socket
	shutdown(m_clientSocket, SHUT_RDWR);
    close(m_clientSocket);
}


void TCP::CTcpClient::threadReceiveMsgFromServer()
{
	#if TCP_DEBUG
	//cout << "> threadReceiveMsgFromServer (begin)" << endl;
	#endif

	SMsgHeader 	l_msgHeader;
	int			l_readBytesSize;
	int			l_closeSocket		= 0;

	while(l_closeSocket == 0)
	{
		// Read the message header
		l_readBytesSize = recv(m_clientSocket, &l_msgHeader, sizeof(SMsgHeader), 0);
		if (l_readBytesSize != -1)
		{
			// Read the message content
			switch(l_msgHeader.id)
			{
				case ID_MSG_INFO_KEEP_ALIVE:
					m_mutexMsgInfoKeepAlive.lock();
					l_readBytesSize = recv(m_clientSocket, &m_msgInfoKeepAlive.body, sizeof(SMsgInfoKeepAliveBody), 0);
					#if TCP_DEBUG
					displayMsgInfoKeepAlive(&m_msgInfoKeepAlive);
					#endif
					m_mutexMsgInfoKeepAlive.unlock();
					break;

				case ID_MSG_INFO_POSITION:
					m_mutexMsgInfoPosition.lock();
					l_readBytesSize = recv(m_clientSocket, &m_msgInfoPosition.body, sizeof(SMsgInfoPositionBody), 0);
					#if TCP_DEBUG
					displayMsgInfoPosition(&m_msgInfoPosition);
					#endif
					m_mutexMsgInfoPosition.unlock();
					break;

				case ID_MSG_ORDER_BIT:
					m_mutexMsgOrderBit.lock();
					l_readBytesSize = recv(m_clientSocket, &m_msgOrderBit.body, sizeof(SMsgOrderBitBody), 0);
					#if TCP_DEBUG
					displayMsgOrderBit(&m_msgOrderBit);
					#endif
					m_mutexMsgOrderBit.unlock();
					monitorMsgOrderBit();
					break;

				case ID_MSG_ORDER_PATH:
					m_mutexMsgOrderPath.lock();
					l_readBytesSize = recv(m_clientSocket, &m_msgOrderPath.body, sizeof(SMsgOrderPathBody), 0);
					#if TCP_DEBUG
					displayMsgOrderPath(&m_msgOrderPath);
					#endif
					m_mutexMsgOrderPath.unlock();
					break;

				case ID_MSG_ORDER_PATH_CORR:
					m_mutexMsgOrderPathCorr.lock();
					l_readBytesSize = recv(m_clientSocket, &m_msgOrderPathCorr.body, sizeof(SMsgOrderPathCorrBody), 0);
					#if TCP_DEBUG
					displayMsgOrderPathCorr(&m_msgOrderPathCorr);
					#endif
					m_mutexMsgOrderPathCorr.unlock();
					break;

				case ID_MSG_ORDER_WORKSHOP:
					m_mutexMsgOrderWorkShop.lock();
					l_readBytesSize = recv(m_clientSocket, &m_msgOrderWorkShop.body, sizeof(SMsgOrderWorkShopBody), 0);
					#if TCP_DEBUG
					displayMsgOrderWorkShop(&m_msgOrderWorkShop);
					#endif
					m_mutexMsgOrderWorkShop.unlock();
					break;

				case ID_MSG_ORDER_STOP:
					m_mutexMsgOrderStop.lock();
					l_readBytesSize = recv(m_clientSocket, &m_msgOrderStop.body, sizeof(SMsgOrderStopBody), 0);
					#if TCP_DEBUG
					displayMsgOrderStop(&m_msgOrderStop);
					#endif
					m_mutexMsgOrderStop.unlock();
					break;

				default:
					break;
			}
		}
	}

	#if TCP_DEBUG
	cout << "> threadReceiveMsgFromServer (end)" << endl;
	#endif
}



void TCP::CTcpClient::monitorMsgOrderBit()
{
	sendMsgToServer(ID_MSG_REPORT_BIT);
}



void TCP::CTcpClient::sendMsgToServer(uint32_t p_msgId)
{
	int l_ret = 0;

	switch(p_msgId)
	{
		case ID_MSG_REPORT_BIT:
			m_mutexMsgReportBit.lock();
			l_ret = send(m_clientSocket, &m_msgReportBit, sizeof(SMsgReportBit), 0);
			#if TCP_DEBUG
			displayMsgReportBit(&m_msgReportBit);
			#endif
			m_mutexMsgReportBit.unlock();
			break;

		case ID_MSG_REPORT_WORKSHOP:
			m_mutexMsgReportWorkShop.lock();
			l_ret = send(m_clientSocket, &m_msgReportWorkShop, sizeof(SMsgReportWorkShop), 0);
			#if TCP_DEBUG
			displayMsgReportWorkShop(&m_msgReportWorkShop);
			#endif
			m_mutexMsgReportWorkShop.unlock();
			break;

		default:
			break;
	}

	if(l_ret == -1)
	{
		closeTcpClient();
	}
}



void TCP::CTcpClient::updateMsg(uint32_t p_updateIdMsg, void* p_updateMsgBuffer)
{
	switch(p_updateIdMsg)
	{
		case ID_MSG_REPORT_BIT:
			m_mutexMsgReportBit.lock();
			memcpy(&m_msgReportBit.body, p_updateMsgBuffer, sizeof(SMsgReportBitBody));
			#if TCP_DEBUG
			displayMsgReportBit(&m_msgReportBit);
			#endif
			m_mutexMsgReportBit.unlock();
			break;

		case ID_MSG_REPORT_WORKSHOP:
			m_mutexMsgReportWorkShop.lock();
			memcpy(&m_msgReportWorkShop.body, p_updateMsgBuffer, sizeof(SMsgReportWorkShopBody));
			#if TCP_DEBUG
			displayMsgReportWorkShop(&m_msgReportWorkShop);
			#endif
			m_mutexMsgReportWorkShop.unlock();
			break;

		default:
			break;
	}
}



void TCP::CTcpClient::getMsg(uint32_t p_getMsgId, void* p_getMsgBuffer)
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
			m_mutexMsgReportWorkShop.lock();
			memcpy(p_getMsgBuffer, &m_msgReportWorkShop, sizeof(SMsgReportWorkShop));
			m_mutexMsgReportWorkShop.unlock();
			break;

		case ID_MSG_REPORT_BIT:
			m_mutexMsgReportBit.lock();
			memcpy(p_getMsgBuffer, &m_msgReportBit, sizeof(SMsgReportBit));
			m_mutexMsgReportBit.unlock();
			break;

		default:
			break;
	}
}
