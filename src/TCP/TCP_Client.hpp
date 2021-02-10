/**
 * "TCP_Client.hpp"
 **/

#ifndef INC_TCPCLIENT_HPP_
#define INC_TCPCLIENT_HPP_

#include "TCP_Common.hpp"

namespace TCP {
	/**
		@class CTcpClient
		@brief TCP communication client class
	 **/
	class CTcpClient {
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
			CTcpClient( int p_serverSocketPort, std::string p_serverSocketIpAddr );

			/**
				@brief destructor
			 **/
			virtual ~CTcpClient();

			/**
				@brief method to initialize the TCP client
				@return the TCP client socket
			 **/
			int initTcpClient();

			/**
				@brief thread-method to receive periodic messages from the server
			 **/
			void threadReceiveMsgFromServer();

			/**
				@brief monitor received messages from the server
			 **/
			void monitorMsgOrderBit();

			/**
				@brief update a buffer message (set)
			 **/
			void updateMsg( uint32_t p_updatEIdMsg, void* p_updateMsgBuffer );

			/**
				@brief methods to access private parameters
			 **/
			void getMsg( uint32_t p_getMsgId, void* p_getMsgBuffer );

			/**
				@brief method to request a message to TCP server
				@param[in] p_msgId : message to send to the client
				@return the transmitted bytes number
			 **/
			int sendMsgToServer( uint32_t p_msgId );

		private:
			std::thread			m_threadReceiveMsgFromServer;	//< TCP client reception thread
			std::mutex 			m_mutexMsgInfoKeepAlive;		//< TCP client mutex for info message : keep alive
			std::mutex 			m_mutexMsgInfoPosition;			//< TCP client mutex for info message : position
			std::mutex 			m_mutexMsgOrderBit;				//< TCP client mutex for order message : bit
			std::mutex 			m_mutexMsgOrderPath;			//< TCP client mutex for order message : path
			std::mutex 			m_mutexMsgOrderPathCorr;		//< TCP client mutex for order message : path correction
			std::mutex 			m_mutexMsgOrderWorkShop;		//< TCP client mutex for order message : workshop
			std::mutex 			m_mutexMsgOrderStop;			//< TCP client mutex for order message : stop
			std::mutex 			m_mutexMsgReportWorkShop;		//< TCP client mutex for report message : workshop
			std::mutex 			m_mutexMsgReportBit;			//< TCP client mutex for report message : bit
			std::mutex 			m_mutexIsAliveServer;			//< TCP client mutex for checking server status
			std::mutex 			m_mutexClientSocket;			//< TCP client mutex for socket
			SMsgInfoKeepAlive 	m_msgInfoKeepAlive;				//< TCP client receive buffer for info message : keep alive
			SMsgInfoPosition 	m_msgInfoPosition;				//< TCP client receive buffer for info message : position
			SMsgOrderBit 		m_msgOrderBit;					//< TCP client receive buffer for order message : bit
			SMsgOrderPath 		m_msgOrderPath;					//< TCP client receive buffer for order message : path
			SMsgOrderPathCorr 	m_msgOrderPathCorr;				//< TCP client receive buffer for order message : path correction
			SMsgOrderWorkShop 	m_msgOrderWorkShop;				//< TCP client receive buffer for order message : workshop
			SMsgOrderStop		m_msgOrderStop;					//< TCP client receive buffer for order message : stop
			SMsgReportWorkShop	m_msgReportWorkShop;			//< TCP client receive buffer for report message : workshop
			SMsgReportBit		m_msgReportBit;					//< TCP client receive buffer for report message : bit
			sockaddr_in 		m_serverSocketAddr;				//< TCP server socket address
			std::string 		m_serverIpAddress;				//< TCP server IP address
			int 				m_clientSocket;					//< TCP client socket
			int 				m_serverSocketPort;				//< TCP server socket port
			bool 				m_isAliveServer;				//< TCP server status
	};
}

#endif /* INC_TCPCLIENT_HPP_ */
