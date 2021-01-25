/**
 * "TCP_Server.hpp"
 **/



#ifndef INC_TCPSERVER_HPP_
#define INC_TCPSERVER_HPP_



#include "TCP_Common.hpp"



/*
shutdown to end read/write.
close to releases data.
 */
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
				@brief method to initialize the TCP server
				@return the TCP server socket
			 **/
			int initTcpServer();

			/**
				@brief method to close the TCP server
			 **/
			void closeTcpServer();

			/**
				@brief thread-method to start the TCP server socket and wait for clients connection
			 **/
			void threadStartTcpServer();

			/**
				@brief thread-method to send a periodic keep alive info message to the client
			 **/
			void threadPeriodicSendToClientMsgInfoKeepAlive(uint32_t p_clientId);

			/**
				@brief thread-method to send a periodic position info message to the client
			 **/
			void threadPeriodicSendToClientMsgInfoPosition(uint32_t p_clientId);

			/**
				@brief thread-method to treat clients messages
			 **/
			void threadTreatClientMsg(uint32_t p_clientId);

			/**
				@brief update a buffer message (set)
			 **/
			void updateMsg(uint32_t p_updatEIdMsg, void* p_updateMsgBuffer);

			/**
				@brief get a buffer message (get)
			 **/
			void getMsg(uint32_t p_getMsgId, void* p_getMsgBuffer);

			/**
				@brief methods to send a message to TCP client (send)
				@return -1 if failed
			 **/
			int sendMsgToClient(uint32_t p_msgId, uint32_t p_clientId);

			// AHU : TMP
			void tmpUpdateMsgInfoPosition();

		private:
			thread				m_threadStart;												//< TCP server start thread for waiting clients connection
			thread				m_threadCientConnexion[SOMAXCONN];							//< TCP server connexion thread
			thread				m_threadTreatClientMsg[SOMAXCONN];							//< TCP server waiting thread
			thread				m_threadPeriodicSendToClientMsgInfoKeepAlive[SOMAXCONN];	//< TCP server periodic thread
			thread				m_threadPeriodicSendToClientMsgInfoPosition[SOMAXCONN];		//< TCP server periodic thread
			mutex 				m_mutexMsgInfoKeepAlive;									//< TCP client mutex for info message : keep alive
			mutex 				m_mutexMsgInfoPosition;										//< TCP client mutex for info message : position
			mutex 				m_mutexMsgOrderBit;											//< TCP client mutex for order message : bit
			mutex 				m_mutexMsgOrderPath;										//< TCP client mutex for order message : path
			mutex 				m_mutexMsgOrderPathCorr;									//< TCP client mutex for order message : path correction
			mutex 				m_mutexMsgOrderWorkShop;									//< TCP client mutex for order message : workshop
			mutex 				m_mutexMsgOrderStop;										//< TCP client mutex for order message : stop
			mutex 				m_mutexMsgReportWorkShop[SOMAXCONN];						//< TCP client mutex for report message : workshop
			mutex 				m_mutexMsgReportBit[SOMAXCONN];								//< TCP client mutex for report message : bit
			SMsgInfoKeepAlive 	m_msgInfoKeepAlive;											//< TCP client receive buffer for info message : keep alive
			SMsgInfoPosition 	m_msgInfoPosition;											//< TCP client receive buffer for info message : position
			SMsgOrderBit 		m_msgOrderBit;												//< TCP client receive buffer for order message : bit
			SMsgOrderPath 		m_msgOrderPath;												//< TCP client receive buffer for order message : path
			SMsgOrderPathCorr 	m_msgOrderPathCorr;											//< TCP client receive buffer for order message : path correction
			SMsgOrderWorkShop 	m_msgOrderWorkShop;											//< TCP client receive buffer for order message : workshop
			SMsgOrderStop		m_msgOrderStop;												//< TCP client receive buffer for order message : stop
			SMsgReportWorkShop	m_msgReportWorkShop[SOMAXCONN];								//< TCP client receive buffer for report message : workshop
			SMsgReportBit		m_msgReportBit[SOMAXCONN];									//< TCP client receive buffer for report message : bit
			sockaddr_in 		m_serverSocketAddr;											//< TCP server socket address
			sockaddr_in 		m_clientSocketAddr[SOMAXCONN];								//< TCP clients sockets address
			socklen_t 			m_serverSocketAddrSize;										//< TCP client socket address size
			string 				m_serverIpAddress;											//< TCP server IP address
			uint32_t			m_serverClientCounter;										//< TCP server clientNb
			uint32_t 			m_serverClientStatus[SOMAXCONN];							//< TCP server client status
			int 				m_serverSocketPort;											//< TCP server socket port
			int 				m_serverSocket;												//< TCP server socket
			int 				m_clientSocket[SOMAXCONN];									//< TCP clients sockets
			char 				m_clientName[SOMAXCONN][NI_MAXHOST];						//< TCP server clients name
			char 				m_clientPort[SOMAXCONN][NI_MAXSERV];						//< TCP server clients port
	};
}



#endif /* INC_TCPSERVER_HPP_ */
