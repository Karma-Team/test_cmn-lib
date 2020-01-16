/*
 * COM_TcpClient.hpp
 *
 *  Created on: 12 janv. 2020
 *      Author: ahu
 */



#ifndef INC_COMTCPCLIENT_HPP_
#define INC_COMTCPCLIENT_HPP_



#include "COM_TcpCommon.hpp"



namespace COM
{
	class CTcpClient
	{
		public:
			CTcpClient();
			virtual ~CTcpClient();
			int initTcpClient();
			int startTcpClient();

		private:
		    sockaddr_in m_serverSocketAddr;
		    socklen_t	m_serverSocketAddrSize;
		    string 		m_serverIpAddress;
		    string 		m_clientInput;
			int 		m_clientRequestedMsgId;
		    int 		m_clientSocket;
		    int 		m_serverPort;
			int 		m_receivedBytesNb;
		    char 		m_buffer[BUFFER_SIZE];
	};
}



#endif /* INC_COMTCPCLIENT_HPP_ */
