/**
 * "TCP_Server.hpp"
 **/



#ifndef INC_TCPSERVER_HPP_
#define INC_TCPSERVER_HPP_



#include "TCP_Common.hpp"



namespace TCP
{
	/**
		@class CTcpClient
		@brief TCP communication server class
	 **/
	class CTcpServer
	{
		public:
			/**
				@brief constructor
			 **/
			CTcpServer();

			/**
				@brief constructor
				@param[in] p_serverSocketPort : TCP server port
				@param[in] p_serverSocketIpAddr : TCP server IP address
			 **/
			CTcpServer(int p_serverSocketPort, string p_serverSocketIpAddr);

			/**
				@brief destructor
			 **/
			virtual ~CTcpServer();

			/**
				@brief method to initialize the TCP server socket
				@return the TCP server socket
			 **/
			int initTcpServer();

			/**
				@brief thread-method to start the TCP server socket and wait for clients connection
			 **/
			void startTcpServer();

			/**
				@brief thread-method to treat clients requests
			 **/
			void clientThread(uint32_t p_clientId);

			/**
				@brief update a buffer message
				@return -1 if failed
			 **/
			int updateMsg(uint32_t p_updateMsgId, void* p_updateMsgBuffer);
			/*
			int updatePathMsg(SPathMsgBody p_pathMsgBody);
			int updatePathCorrectionMsg(SPathCorrectionMsgBody p_pathCorrectionMsgBody);
			int updateWorkShopOrderMsg(SWorkShopOrderMsgBody p_workShopOrderMsgBody);
			int updateStopMsg(SStopMsgBody p_stopMsgBody);
			int updateWorkShopReportMsg(SWorkShopReportMsgBody p_workShopReportMsgBody);
			int updateBitReportMsg(SBitReportMsgBody p_bitReportMsgBody);
			int updateErrorMsg(SErrorMsgBody p_errorMsgBody);
			*/

			/**
				@brief methods to send a message to TCP client
				@return -1 if failed
			 **/
			int sendPathMsgToClient(uint32_t p_clientId);
			int sendPathCorrectionMsgToClient(uint32_t p_clientId);
			int sendWorkShopOrderMsgToClient(uint32_t p_clientId);
			int sendStopMsgToClient(uint32_t p_clientId);
			int sendWorkShopReportMsgToClient(uint32_t p_clientId);
			int sendBitReportMsgToClient(uint32_t p_clientId);
			int sendErrorMsgToClient(uint32_t p_clientId);

		private:
			thread					m_startThread;							//< TCP server start thread for waiting clients connection
			thread					m_clientThread[SOMAXCONN];				//< TCP server reception thread for message from client
			SPathMsg 				m_pathMsg;								//< TCP server send buffer for path message to client
			SPathCorrectionMsg 		m_pathCorrectionMsg;					//< TCP server send buffer for path correction message to client
			SWorkShopOrderMsg 		m_workShopOrderMsg;						//< TCP server send buffer for workshop order message to client
			SStopMsg		 		m_stopMsg;								//< TCP server send buffer for stop message to client
			SWorkShopReportMsg		m_workShopReportMsg;					//< TCP server send buffer for workshop order report message to client
			SBitReportMsg			m_bitReportMsg;							//< TCP server send buffer for bit report message from server to client
			SErrorMsg	 			m_errorMsg;								//< TCP server send buffer for error message to client
			sockaddr_in 			m_serverSocketAddr;						//< TCP server socket address
			sockaddr_in 			m_clientSocketAddr[SOMAXCONN];			//< TCP clients sockets address
			socklen_t 				m_serverSocketAddrSize;					//< TCP client socket address size
			string 					m_serverIpAddress;						//< TCP server IP address
			uint32_t				m_serverClientNb;						//< TCP server clientNb
			int 					m_serverSocketPort;						//< TCP server socket port
			int 					m_serverSocket;							//< TCP server socket
			int 					m_clientSocket[SOMAXCONN];				//< TCP clients sockets
			char 					m_clientName[SOMAXCONN][NI_MAXHOST];	//< TCP server clients name
			char 					m_clientPort[SOMAXCONN][NI_MAXSERV];	//< TCP server clients port
	};
}



#endif /* INC_TCPSERVER_HPP_ */
