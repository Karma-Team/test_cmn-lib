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
			//void clientConnectionThread(uint32_t p_clientId);

			/**
				@brief thread-method to send periodic messages to the client
			 **/
			void sendPeriodicMsgToClientThread(uint32_t p_clientId);

			/**
				@brief thread-method to treat clients requests
			 **/
			void treatClientRequestThread(uint32_t p_clientId);

			/**
				@brief update a buffer message (set)
				@return -1 if failed
			 **/
			int updateMsg(uint32_t p_updateMsgId, void* p_updateMsgBuffer);
			void tmpUpdatePositionMsg();

			/**
				@brief get a buffer message (get)
				@return -1 if failed
			 **/
			int getMsg(uint32_t p_getMsgId, void* p_getMsgBuffer);

			/**
				@brief methods to send a message to TCP client (send)
				@return -1 if failed
			 **/
			int sendMsgToClient(uint32_t p_sendMsgId, uint32_t p_clientId);

		private:
			thread					m_startThread;								//< TCP server start thread for waiting clients connection
			thread					m_clientConnexionThread[SOMAXCONN];			//< TCP server connexion thread
			thread					m_sendPeriodicMsgToClientThread[SOMAXCONN];	//< TCP server periodic thread
			thread					m_treatClientRequestThread[SOMAXCONN];		//< TCP server waiting thread
			mutex 					m_positionMsgMutex;							//< TCP server mutex for position message
			mutex 					m_pathMsgMutex;								//< TCP server mutex for path message
			mutex 					m_pathCorrectionMsgMutex;					//< TCP server mutex for path correction message
			mutex 					m_workShopOrderMsgMutex;					//< TCP server mutex for workshop order message
			mutex 					m_stopMsgMutex;								//< TCP server mutex for stop message
			mutex 					m_workShopReportMsgMutex;					//< TCP server mutex for workshop report message
			mutex 					m_bitReportMsgMutex;						//< TCP server mutex for bit report message
			mutex 					m_errorMsgMutex;							//< TCP server mutex for error message
			SPositionMsg 			m_positionMsg;								//< TCP server send buffer for position message to client
			SPathMsg 				m_pathMsg;									//< TCP server send buffer for path message to client
			SPathCorrectionMsg 		m_pathCorrectionMsg;						//< TCP server send buffer for path correction message to client
			SWorkShopOrderMsg 		m_workShopOrderMsg;							//< TCP server send buffer for workshop order message to client
			SStopMsg		 		m_stopMsg;									//< TCP server send buffer for stop message to client
			SWorkShopReportMsg		m_workShopReportMsg;						//< TCP server send buffer for workshop order report message to client
			SBitReportMsg			m_bitReportMsg;								//< TCP server send buffer for bit report message from server to client
			SErrorMsg	 			m_errorMsg;									//< TCP server send buffer for error message to client
			sockaddr_in 			m_serverSocketAddr;							//< TCP server socket address
			sockaddr_in 			m_clientSocketAddr[SOMAXCONN];				//< TCP clients sockets address
			socklen_t 				m_serverSocketAddrSize;						//< TCP client socket address size
			string 					m_serverIpAddress;							//< TCP server IP address
			uint32_t				m_serverClientCounter;						//< TCP server clientNb
			int 					m_serverSocketPort;							//< TCP server socket port
			int 					m_serverSocket;								//< TCP server socket
			int 					m_clientSocket[SOMAXCONN];					//< TCP clients sockets
			char 					m_clientName[SOMAXCONN][NI_MAXHOST];		//< TCP server clients name
			char 					m_clientPort[SOMAXCONN][NI_MAXSERV];		//< TCP server clients port
	};
}



#endif /* INC_TCPSERVER_HPP_ */
