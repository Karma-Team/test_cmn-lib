/**
 * "TCP_Client.cpp"
 **/

#include "TCP_Client.hpp"

using namespace std;

TCP::CTcpClient::CTcpClient() {
    m_serverIpAddress 							= TCP_SERVER_IP_ADDRESS;
	m_clientSocket								= -1;
    m_serverSocketPort							= TCP_SERVER_PORT;
	SMsgInfoKeepAlive 		l_msgInfoKeepAlive	= {.hd={ID_MSG_INFO_KEEP_ALIVE,	sizeof( SMsgInfoKeepAlive )}, 	.body={false}};
	SMsgInfoPosition 		l_msgInfoPosition 	= {.hd={ID_MSG_INFO_POSITION, 	sizeof( SMsgInfoPosition )}, 	.body={0, 0}};
	SMsgOrderBit			l_msgOrderBit		= {.hd={ID_MSG_ORDER_BIT, 		sizeof( SMsgOrderBit )}, 		.body={0}};
	SMsgOrderPath 			l_msgOrderPath 		= {.hd={ID_MSG_ORDER_PATH,		sizeof( SMsgOrderPath )}, 		.body={0, {0,0,0,0,0,0,0,0,0,0}}};
	SMsgOrderPathCorr		l_msgOrderPathCorr	= {.hd={ID_MSG_ORDER_PATH_CORR, sizeof( SMsgOrderPathCorr )}, 	.body={0, 0, {0,0,0,0,0,0,0,0,0,0}}};
	SMsgOrderWorkShop 		l_msgOrderWorkShop	= {.hd={ID_MSG_ORDER_WORKSHOP, 	sizeof( SMsgOrderWorkShop )}, 	.body={0}};
	SMsgOrderStop 			l_msgOrderStop		= {.hd={ID_MSG_ORDER_STOP,		sizeof( SMsgOrderStop )}, 		.body={0}};
	SMsgReportBit 			l_msgReportBit		= {.hd={ID_MSG_REPORT_BIT, 		sizeof( SMsgReportBit )}, 		.body={0}};
	SMsgReportWorkShop 		l_msgReportWorkShop	= {.hd={ID_MSG_REPORT_WORKSHOP, sizeof( SMsgReportWorkShop )},	.body={0}};
	m_msgInfoKeepAlive 							= l_msgInfoKeepAlive;
	m_msgInfoPosition 							= l_msgInfoPosition;
	m_msgOrderBit 								= l_msgOrderBit;
	m_msgOrderPath 								= l_msgOrderPath;
	m_msgOrderPathCorr 							= l_msgOrderPathCorr;
	m_msgOrderWorkShop							= l_msgOrderWorkShop;
	m_msgOrderStop								= l_msgOrderStop;
	m_msgReportBit								= l_msgReportBit;
	m_msgReportWorkShop							= l_msgReportWorkShop;
	m_isAliveServer								= false;
}

TCP::CTcpClient::CTcpClient( int p_serverSocketPort, string p_serverSocketIpAddr ) {
	m_serverIpAddress							= p_serverSocketIpAddr;
	m_clientSocket								= -1;
	m_serverSocketPort							= p_serverSocketPort;
	SMsgInfoKeepAlive 		l_msgInfoKeepAlive	= {.hd={ID_MSG_INFO_KEEP_ALIVE,	sizeof( SMsgInfoKeepAlive )}, 	.body={false}};
	SMsgInfoPosition 		l_msgInfoPosition 	= {.hd={ID_MSG_INFO_POSITION, 	sizeof( SMsgInfoPosition )}, 	.body={0, 0}};
	SMsgOrderBit			l_msgOrderBit		= {.hd={ID_MSG_ORDER_BIT, 		sizeof( SMsgOrderBit )}, 		.body={0}};
	SMsgOrderPath 			l_msgOrderPath 		= {.hd={ID_MSG_ORDER_PATH,		sizeof( SMsgOrderPath )}, 		.body={0, {0,0,0,0,0,0,0,0,0,0}}};
	SMsgOrderPathCorr		l_msgOrderPathCorr	= {.hd={ID_MSG_ORDER_PATH_CORR, sizeof( SMsgOrderPathCorr )}, 	.body={0, 0, {0,0,0,0,0,0,0,0,0,0}}};
	SMsgOrderWorkShop 		l_msgOrderWorkShop	= {.hd={ID_MSG_ORDER_WORKSHOP, 	sizeof( SMsgOrderWorkShop )}, 	.body={0}};
	SMsgOrderStop 			l_msgOrderStop		= {.hd={ID_MSG_ORDER_STOP,		sizeof( SMsgOrderStop )}, 		.body={0}};
	SMsgReportBit 			l_msgReportBit		= {.hd={ID_MSG_REPORT_BIT, 		sizeof( SMsgReportBit )}, 		.body={0}};
	SMsgReportWorkShop 		l_msgReportWorkShop	= {.hd={ID_MSG_REPORT_WORKSHOP, sizeof( SMsgReportWorkShop )},	.body={0}};
	m_msgInfoKeepAlive 							= l_msgInfoKeepAlive;
	m_msgInfoPosition 							= l_msgInfoPosition;
	m_msgOrderBit 								= l_msgOrderBit;
	m_msgOrderPath 								= l_msgOrderPath;
	m_msgOrderPathCorr 							= l_msgOrderPathCorr;
	m_msgOrderWorkShop							= l_msgOrderWorkShop;
	m_msgOrderStop								= l_msgOrderStop;
	m_msgReportBit								= l_msgReportBit;
	m_msgReportWorkShop							= l_msgReportWorkShop;
	m_isAliveServer								= false;
}

TCP::CTcpClient::~CTcpClient() {
	uint32_t l_sleepTimeUs = 1000000;	// 1 sec
	logDebug( "Start the TCP client closing process" );

	// Disable sends and receives and close the client socket
	{
		std::scoped_lock lock( m_mutexClientSocket );
		shutdown( m_clientSocket, SHUT_RDWR );
		close( m_clientSocket );
	}

	// Wait for threads to terminate
	usleep( l_sleepTimeUs );
	logDebug( "TCP client closed" );
}

int TCP::CTcpClient::initTcpClient() {
	socklen_t 	l_serverSocketAddrSize;
	int 		l_ret					= -1;

    // Create the client socket
	m_clientSocket = socket( AF_INET, SOCK_STREAM, 0 );
	if( m_clientSocket != -1 ) {
		// Convert an IP address from textual to binary format
		m_serverSocketAddr.sin_family 	= AF_INET;
		m_serverSocketAddr.sin_port 	= htons( m_serverSocketPort );
		if( inet_pton( AF_INET, m_serverIpAddress.c_str(), &m_serverSocketAddr.sin_addr ) != 1 ) {
			cerr << "> Can't convert the Internet address! Quitting" << endl;
		} else {
			// Connect the client socket to the server one
			l_serverSocketAddrSize = sizeof( m_serverSocketAddr );
			if( connect( m_clientSocket, (sockaddr*)&m_serverSocketAddr, l_serverSocketAddrSize ) != -1 ) {
				// Launch the reception thead
				{
					std::scoped_lock lock( m_mutexIsAliveServer );
					m_isAliveServer				= true;
				}
				l_ret 							= m_clientSocket;
				m_threadReceiveMsgFromServer 	= thread( &TCP::CTcpClient::threadReceiveMsgFromServer, this );
				m_threadReceiveMsgFromServer.detach();
			}
		}
	}

	return l_ret;
}

void TCP::CTcpClient::threadReceiveMsgFromServer() {
	logDebug( "Client connected to server : socketValue = " );

	SMsgHeader 	l_msgHeader;
	int			l_clientSocket		= -1;
	int			l_receivedBytesNb;

	{
		std::scoped_lock lock( m_mutexClientSocket );
		l_clientSocket = m_clientSocket;
	}

	while( l_clientSocket != -1 ) {
		// Read the message header
		l_receivedBytesNb = recv( l_clientSocket, &l_msgHeader, sizeof( SMsgHeader ), 0 );
		if( l_receivedBytesNb == 0 ) {
			{
				std::scoped_lock lock( m_mutexIsAliveServer );
				m_isAliveServer	= false;
			}
		} else if( l_receivedBytesNb != -1 ) {
			// Read the message content
			switch( l_msgHeader.id ) {
				case ID_MSG_INFO_KEEP_ALIVE:
					{
						std::scoped_lock lock( m_mutexMsgInfoKeepAlive );
						l_receivedBytesNb = recv( l_clientSocket, &m_msgInfoKeepAlive.body, sizeof( SMsgInfoKeepAliveBody ), 0 );
						displayMsgInfoKeepAlive( &m_msgInfoKeepAlive );
					}
					break;

				case ID_MSG_INFO_POSITION:
					{
						std::scoped_lock lock( m_mutexMsgInfoPosition );
						l_receivedBytesNb = recv( l_clientSocket, &m_msgInfoPosition.body, sizeof( SMsgInfoPositionBody ), 0 );
						displayMsgInfoPosition( &m_msgInfoPosition );
					}
					break;

				case ID_MSG_ORDER_BIT:
					{
						std::scoped_lock lock( m_mutexMsgOrderBit );
						l_receivedBytesNb = recv( l_clientSocket, &m_msgOrderBit.body, sizeof( SMsgOrderBitBody ), 0 );
						displayMsgOrderBit( &m_msgOrderBit );
					}
					monitorMsgOrderBit();
					break;

				case ID_MSG_ORDER_PATH:
					{
						std::scoped_lock lock( m_mutexMsgOrderPath );
						l_receivedBytesNb = recv( l_clientSocket, &m_msgOrderPath.body, sizeof( SMsgOrderPathBody ), 0 );
						displayMsgOrderPath( &m_msgOrderPath );
					}
					break;

				case ID_MSG_ORDER_PATH_CORR:
					{
						std::scoped_lock lock( m_mutexMsgOrderPathCorr );
						l_receivedBytesNb = recv( l_clientSocket, &m_msgOrderPathCorr.body, sizeof( SMsgOrderPathCorrBody ), 0 );
						displayMsgOrderPathCorr( &m_msgOrderPathCorr );
					}
					break;

				case ID_MSG_ORDER_WORKSHOP:
					{
						std::scoped_lock lock( m_mutexMsgOrderWorkShop );
						l_receivedBytesNb = recv( l_clientSocket, &m_msgOrderWorkShop.body, sizeof( SMsgOrderWorkShopBody ), 0 );
						displayMsgOrderWorkShop( &m_msgOrderWorkShop );
					}
					break;

				case ID_MSG_ORDER_STOP:
					{
						std::scoped_lock lock( m_mutexMsgOrderStop );
						l_receivedBytesNb = recv( l_clientSocket, &m_msgOrderStop.body, sizeof( SMsgOrderStopBody ), 0 );
						displayMsgOrderStop( &m_msgOrderStop );
					}
					break;

				default:
					break;
			}
		}

		{
			std::scoped_lock lock( m_mutexClientSocket );
			l_clientSocket = m_clientSocket;
		}
	}

	logDebug( "threadReceiveMsgFromServer (end)" );
}

void TCP::CTcpClient::monitorMsgOrderBit() {
	sendMsgToServer( ID_MSG_REPORT_BIT );
}

int TCP::CTcpClient::sendMsgToServer( uint32_t p_msgId ) {
	int		l_clientSocket			= -1;
	int 	l_transmittedBytesNb 	= 0;
	bool 	l_isAliveServer;

	{
		std::scoped_lock lock( m_mutexClientSocket, m_mutexIsAliveServer );
		l_clientSocket = m_clientSocket;
		l_isAliveServer = m_isAliveServer;
	}

	if( l_isAliveServer ) {
		switch( p_msgId ) {
			case ID_MSG_REPORT_BIT:
				{
					std::scoped_lock lock( m_mutexMsgReportBit );
					l_transmittedBytesNb = send( l_clientSocket, &m_msgReportBit, sizeof( SMsgReportBit ), 0 );
					displayMsgReportBit( &m_msgReportBit );
				}
				break;

			case ID_MSG_REPORT_WORKSHOP:
				{
					std::scoped_lock lock( m_mutexMsgReportWorkShop );
					l_transmittedBytesNb = send( l_clientSocket, &m_msgReportWorkShop, sizeof( SMsgReportWorkShop ), 0 );
					displayMsgReportWorkShop( &m_msgReportWorkShop );
				}
				break;

			default:
				break;
		}
	}

	if( ( l_transmittedBytesNb == 0 ) || ( l_transmittedBytesNb == -1 )) {
		logDebug( "Failed to send msg to server" );
	}

	return l_transmittedBytesNb;
}

void TCP::CTcpClient::updateMsg( uint32_t p_updateIdMsg, void* p_updateMsgBuffer ) {
	switch( p_updateIdMsg ) {
		case ID_MSG_REPORT_BIT:
			{
				std::scoped_lock lock( m_mutexMsgReportBit );
				memcpy( &m_msgReportBit.body, p_updateMsgBuffer, sizeof( SMsgReportBitBody) );
				displayMsgReportBit( &m_msgReportBit );
			}
			break;

		case ID_MSG_REPORT_WORKSHOP:
			{
				std::scoped_lock lock( m_mutexMsgReportWorkShop );
				memcpy( &m_msgReportWorkShop.body, p_updateMsgBuffer, sizeof( SMsgReportWorkShopBody) );
				displayMsgReportWorkShop( &m_msgReportWorkShop );
			}
			break;

		default:
			break;
	}
}

void TCP::CTcpClient::getMsg( uint32_t p_getMsgId, void* p_getMsgBuffer ) {
	switch( p_getMsgId ) {
		case ID_MSG_INFO_KEEP_ALIVE:
			{
				std::scoped_lock lock( m_mutexMsgInfoKeepAlive );
				memcpy( p_getMsgBuffer, &m_msgInfoKeepAlive, sizeof( SMsgInfoKeepAlive) );
			}
			break;

		case ID_MSG_INFO_POSITION:
			{
				std::scoped_lock lock( m_mutexMsgInfoPosition );
				memcpy( p_getMsgBuffer, &m_msgInfoPosition, sizeof( SMsgInfoPosition) );
			}
			break;

		case ID_MSG_ORDER_BIT:
			{
				std::scoped_lock lock( m_mutexMsgOrderBit );
				memcpy( p_getMsgBuffer, &m_msgOrderBit, sizeof( SMsgOrderBit) );
			}
			break;

		case ID_MSG_ORDER_PATH:
			{
				std::scoped_lock lock( m_mutexMsgOrderPath );
				memcpy( p_getMsgBuffer, &m_msgOrderPath, sizeof( SMsgOrderPath) );
			}
			break;

		case ID_MSG_ORDER_PATH_CORR:
			{
				std::scoped_lock lock( m_mutexMsgOrderPathCorr );
				memcpy( p_getMsgBuffer, &m_msgOrderPathCorr, sizeof( SMsgOrderPathCorr) );
			}
			break;

		case ID_MSG_ORDER_WORKSHOP:
			{
				std::scoped_lock lock( m_mutexMsgOrderWorkShop );
				memcpy( p_getMsgBuffer, &m_msgOrderWorkShop, sizeof( SMsgOrderWorkShop) );
			}
			break;

		case ID_MSG_ORDER_STOP:
			{
				std::scoped_lock lock( m_mutexMsgOrderStop );
				memcpy( p_getMsgBuffer, &m_msgOrderStop, sizeof( SMsgOrderStop) );
			}
			break;

		case ID_MSG_REPORT_WORKSHOP:
			{
				std::scoped_lock lock( m_mutexMsgReportWorkShop );
				memcpy( p_getMsgBuffer, &m_msgReportWorkShop, sizeof( SMsgReportWorkShop) );
			}
			break;

		case ID_MSG_REPORT_BIT:
			{
				std::scoped_lock lock( m_mutexMsgReportBit );
				memcpy( p_getMsgBuffer, &m_msgReportBit, sizeof( SMsgReportBit) );
			}
			break;

		default:
			break;
	}
}
