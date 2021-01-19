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
				@brief thread-method to receive periodic messages from the server
			 **/
			void receiveMsgFromServerThread();

			/**
				@brief method to request a message to TCP server
				@param[in] p_RequestedMsgId : client requested message ID
				@return -1 if failed
			 **/
			int requestMsgToServer(uint32_t p_requestedMsgId);

			/**
				@brief methods to access private parameters
			 **/
			void getPositionMsg(SPositionMsg* p_positionMsg);
			// AHU : accesseur a creer pour tous les messages + y faire appel avant de display dans le fichier de test client (idem pour serveur)

		private:
			thread					m_receiveMsgFromServerThread;	//< TCP client reception thread
			mutex 					m_positionMsgMutex;				//< TCP client mutex for position message
			mutex 					m_pathMsgMutex;					//< TCP client mutex for path message
			mutex 					m_pathCorrectionMsgMutex;		//< TCP client mutex for path correction message
			mutex 					m_workShopOrderMsgMutex;		//< TCP client mutex for workshop order message
			mutex 					m_stopMsgMutex;					//< TCP client mutex for stop message
			mutex 					m_workShopReportMsgMutex;		//< TCP client mutex for workshop report message
			mutex 					m_bitReportMsgMutex;			//< TCP client mutex for bit report message
			mutex 					m_errorMsgMutex;				//< TCP client mutex for error message
			SPositionMsg 			m_positionMsg;					//< TCP client receive buffer for position message from server
			SPathMsg 				m_pathMsg;						//< TCP client receive buffer for path message from server
			SPathCorrectionMsg 		m_pathCorrectionMsg;			//< TCP client receive buffer for path correction message from server
			SWorkShopOrderMsg 		m_workShopOrderMsg;				//< TCP client receive buffer for workshop order message from server
			SStopMsg		 		m_stopMsg;						//< TCP client receive buffer for stop message from server
			SWorkShopReportMsg		m_workShopReportMsg;			//< TCP client receive buffer for workshop order report message from server
			SBitReportMsg			m_bitReportMsg;					//< TCP client receive buffer for bit report message from server to client
			SErrorMsg	 			m_errorMsg;						//< TCP client receive buffer for error message from server
			sockaddr_in 			m_serverSocketAddr;				//< TCP server socket address
			string 					m_serverIpAddress;				//< TCP server IP address
			int 					m_clientSocket;					//< TCP client socket
			int 					m_serverSocketPort;				//< TCP server socket port
	};
}



#endif /* INC_TCPCLIENT_HPP_ */
