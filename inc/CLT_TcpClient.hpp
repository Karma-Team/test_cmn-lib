/*
 * CLT_TcpClient.hpp
 *
 *  Created on: 12 janv. 2020
 *      Author: ahu
 */



#ifndef INC_CLT_TCPCLIENT_HPP_
#define INC_CLT_TCPCLIENT_HPP_



#include "TcpCommunication.hpp"



using namespace std;



namespace CLT
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



#endif /* INC_CLT_TCPCLIENT_HPP_ */
