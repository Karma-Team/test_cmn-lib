/**
 * "COM_TcpServer.hpp"
 **/



#ifndef INC_COMTCPSERVER_HPP_
#define INC_COMTCPSERVER_HPP_



#include "COM_TcpCommon.hpp"



namespace COM
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
			SMsgHeader 				m_msgHeader;							//< TCP server send buffer for message header to client
			SPathMsgBody 			m_pathMsgBody;							//< TCP server send buffer for path message to client
			SPathCorrectionMsgBody 	m_pathCorrectionMsgBody;				//< TCP server send buffer for path correction message to client
			SWorkShopOrderMsgBody 	m_workShopOrderMsgBody;					//< TCP server send buffer for workshop order message to client
			SStopMsgBody		 	m_stopMsgBody;							//< TCP server send buffer for stop message to client
			SWorkShopReportMsgBody	m_workShopReportMsgBody;				//< TCP server send buffer for workshop order report message to client
			SBitReportMsgBody		m_bitReportMsgBody;						//< TCP server send buffer for bit report message from server to client
			SErrorMsgBody 			m_errorMsgBody;							//< TCP server send buffer for error message to client
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



#endif /* INC_COMTCPSERVER_HPP_ */