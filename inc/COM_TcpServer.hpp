/*
 * COM_TcpServer.hpp
 *
 *  Created on: 12 janv. 2020
 *      Author: ahu
 */



#ifndef INC_COMTCPSERVER_HPP_
#define INC_COMTCPSERVER_HPP_



#include "COM_TcpCommon.hpp"



namespace COM
{
	class CTcpServer
	{
		public:
			CTcpServer();
			virtual ~CTcpServer();
			int initTcpServer();
			int startTcpServer();

		private:
		    sockaddr_in m_serverSocketAddr;
		    sockaddr_in m_clientSocketAddr;
		    socklen_t 	m_serverSocketAddrSize;
		    socklen_t 	m_clientSocketAddrSize;
		    string 		m_serverIpAddress;
		    int 		m_clientRequestedMsgId;
		    int 		m_serverPort;
			int 		m_receivedBytesNb;
			int 		m_serverSocket;
		    int 		m_clientSocket;
		    char 		m_buffer[BUFFER_SIZE];
			char 		m_clientName[NI_MAXHOST];
			char 		m_clientPort[NI_MAXSERV];
	};
}



#endif /* INC_COMTCPSERVER_HPP_ */
