/**
 * "TCP_Client.hpp"
 **/



#ifndef INC_TCPCLIENT_HPP_
#define INC_TCPCLIENT_HPP_



#include "TCP_Common.hpp"



namespace TCP
{
	/**
		@class CTcpClient
		@brief TCP communication client class
	 **/
	class CTcpClient
	{
		public:
			/**
				@brief constructor
			 **/
			CTcpClient();

			/**
				@brief constructor
				@param[in] p_serverSocketPort : TCP server port
				@param[in] p_serverSocketIpAddr : TCP server IP address
			 **/
			CTcpClient(int p_serverSocketPort, string p_serverSocketIpAddr);

			/**
				@brief destructor
			 **/
			virtual ~CTcpClient();

			/**
				@brief method to initialize the TCP client socket
				@return the TCP client socket
			 **/
			int initTcpClient();

			/**
				@brief method to request a message to TCP server
				@param[in] p_RequestedMsgId : client requested message ID
				@return -1 if failed
			 **/
			int requestMsgToServer(uint32_t p_requestedMsgId, void * p_buffer);

		private:
			sockaddr_in 			m_serverSocketAddr;						//< TCP server socket address
			string 					m_serverIpAddress;						//< TCP server IP address
			int 					m_clientSocket;							//< TCP client socket
			int 					m_serverSocketPort;						//< TCP server socket port
	};
}



#endif /* INC_TCPCLIENT_HPP_ */
