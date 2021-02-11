/**
 * "TCP_Server.hpp"
 **/

#ifndef INC_TCPSERVER_HPP_
#define INC_TCPSERVER_HPP_

#include "TCP_Common.hpp"

namespace TCP {
	/**
		@class CTcpClient
		@brief TCP communication server class
	 **/
	class CTcpServer {
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
			CTcpServer( int p_serverSocketPort, std::string p_serverSocketIpAddr );

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
				@brief thread-method to start the TCP server socket and wait for clients connection
			 **/
			void threadStartTcpServer();

			/**
				@brief thread-method to send a periodic keep alive info message to the client
			 **/
			void threadPeriodicSendToClientMsgInfoKeepAlive( uint32_t p_clientId );

			/**
				@brief thread-method to send a periodic position info message to the client
			 **/
			void threadPeriodicSendToClientMsgInfoPosition( uint32_t p_clientId );

			/**
				@brief thread-method to treat clients messages
			 **/
			void threadTreatClientMsg( uint32_t p_clientId );

			/**
				@brief update a buffer message (set)
			 **/
			void updateMsg( uint32_t p_updatEIdMsg, void* p_updateMsgBuffer, uint32_t p_clientId );

			/**
				@brief get a buffer message (get)
			 **/
			void getMsg( uint32_t p_getMsgId, void* p_getMsgBuffer, uint32_t p_clientId );

			/**
				@brief methods to send a message to TCP client (send)
				@return -1 if failed
			 **/
			int sendMsgToClient( uint32_t p_msgId, uint32_t p_clientId );

			// AHU : mise a jour aleatoire temporaire (normalement mis a jour apres traitement d'image du mat sur le plan de jeu)
			void tmpUpdateMsgInfoPosition( uint32_t p_clientId );

		private:
			std::thread			m_threadStart;												//< TCP server start thread for waiting clients connection
			std::thread			m_threadTreatClientMsg[SOMAXCONN];							//< TCP server waiting thread
			std::thread			m_threadPeriodicSendToClientMsgInfoKeepAlive[SOMAXCONN];	//< TCP server periodic thread
			std::thread			m_threadPeriodicSendToClientMsgInfoPosition[SOMAXCONN];		//< TCP server periodic thread
			std::mutex 			m_mutexMsgInfoKeepAlive;									//< TCP client mutex for info message : keep alive
			std::mutex 			m_mutexMsgInfoPosition[SOMAXCONN];							//< TCP client mutex for info message : position
			std::mutex 			m_mutexMsgOrderBit[SOMAXCONN];								//< TCP client mutex for order message : bit
			std::mutex 			m_mutexMsgOrderPath[SOMAXCONN];								//< TCP client mutex for order message : path
			std::mutex 			m_mutexMsgOrderPathCorr[SOMAXCONN];							//< TCP client mutex for order message : path correction
			std::mutex 			m_mutexMsgOrderWorkShop[SOMAXCONN];							//< TCP client mutex for order message : workshop
			std::mutex 			m_mutexMsgOrderStop[SOMAXCONN];								//< TCP client mutex for order message : stop
			std::mutex 			m_mutexMsgReportWorkShop[SOMAXCONN];						//< TCP client mutex for report message : workshop
			std::mutex 			m_mutexMsgReportBit[SOMAXCONN];								//< TCP client mutex for report message : bit
			std::mutex 			m_mutexIsAliveClient[SOMAXCONN];							//< TCP server mutex for checking clients status
			std::mutex 			m_mutexClientSocket[SOMAXCONN];								//< TCP server mutex for clients socket
			std::mutex 			m_mutexAllClientsJoinThreadsStatus;							//< TCP client mutex for all clients join threads status
			SMsgInfoKeepAlive 	m_msgInfoKeepAlive;											//< TCP client receive buffer for info message : keep alive
			SMsgInfoPosition 	m_msgInfoPosition[SOMAXCONN];								//< TCP client receive buffer for info message : position
			SMsgOrderBit 		m_msgOrderBit[SOMAXCONN];									//< TCP client receive buffer for order message : bit
			SMsgOrderPath 		m_msgOrderPath[SOMAXCONN];									//< TCP client receive buffer for order message : path
			SMsgOrderPathCorr 	m_msgOrderPathCorr[SOMAXCONN];								//< TCP client receive buffer for order message : path correction
			SMsgOrderWorkShop 	m_msgOrderWorkShop[SOMAXCONN];								//< TCP client receive buffer for order message : workshop
			SMsgOrderStop		m_msgOrderStop[SOMAXCONN];									//< TCP client receive buffer for order message : stop
			SMsgReportWorkShop	m_msgReportWorkShop[SOMAXCONN];								//< TCP client receive buffer for report message : workshop
			SMsgReportBit		m_msgReportBit[SOMAXCONN];									//< TCP client receive buffer for report message : bit
			sockaddr_in 		m_serverSocketAddr;											//< TCP server socket address
			sockaddr_in 		m_clientSocketAddr[SOMAXCONN];								//< TCP clients sockets address
			socklen_t 			m_serverSocketAddrSize;										//< TCP client socket address size
			std::string 		m_serverIpAddress;											//< TCP server IP address
			uint32_t			m_clientCounter;											//< TCP server clientNb
			uint32_t 			m_allClientsJoinThreadsStatus; 								//< TCP server all clients join threads status
			int 				m_serverSocketPort;											//< TCP server socket port
			int 				m_serverSocket;												//< TCP server socket
			int 				m_clientSocket[SOMAXCONN];									//< TCP clients sockets
			char 				m_clientName[SOMAXCONN][NI_MAXHOST];						//< TCP server clients name
			char 				m_clientPort[SOMAXCONN][NI_MAXSERV];						//< TCP server clients port
			bool 				m_isAliveClient[SOMAXCONN];									//< TCP clients status
	};
}

#endif /* INC_TCPSERVER_HPP_ */
