/**
 * "TCP_Client.hpp"
 **/



#ifndef INC_TCPCLIENT_HPP_
#define INC_TCPCLIENT_HPP_



#include "TCP_Common.hpp"



/*
shutdown to end read/write.
close to releases data.
 */
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
				@brief method to initialize the TCP client
				@return the TCP client socket
			 **/
			int initTcpClient();

			/**
				@brief method to close the TCP client
			 **/
			void closeTcpClient();

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
			void updateMsg(uint32_t p_updatEIdMsg, void* p_updateMsgBuffer);

			/**
				@brief methods to access private parameters
			 **/
			void getMsg(uint32_t p_getMsgId, void* p_getMsgBuffer);

			/**
				@brief method to request a message to TCP server
				@param[in] p_RequestedMsgId : client requested message ID
			 **/
			void sendMsgToServer(uint32_t p_msgId);

		private:
			thread				m_threadReceiveMsgFromServer;	//< TCP client reception thread
			mutex 				m_mutexMsgInfoKeepAlive;		//< TCP client mutex for info message : keep alive
			mutex 				m_mutexMsgInfoPosition;			//< TCP client mutex for info message : position
			mutex 				m_mutexMsgOrderBit;				//< TCP client mutex for order message : bit
			mutex 				m_mutexMsgOrderPath;			//< TCP client mutex for order message : path
			mutex 				m_mutexMsgOrderPathCorr;		//< TCP client mutex for order message : path correction
			mutex 				m_mutexMsgOrderWorkShop;		//< TCP client mutex for order message : workshop
			mutex 				m_mutexMsgOrderStop;			//< TCP client mutex for order message : stop
			mutex 				m_mutexMsgReportWorkShop;		//< TCP client mutex for report message : workshop
			mutex 				m_mutexMsgReportBit;			//< TCP client mutex for report message : bit
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
			string 				m_serverIpAddress;				//< TCP server IP address
			int 				m_clientSocket;					//< TCP client socket
			int 				m_serverSocketPort;				//< TCP server socket port
	};
}



#endif /* INC_TCPCLIENT_HPP_ */
