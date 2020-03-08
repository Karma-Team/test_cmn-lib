/**
 * "TCP_Client.cpp"
 **/



#include "TCP_Client.hpp"



TCP::CTcpClient::CTcpClient()
{
    m_serverIpAddress 				= TCP_SERVER_IP_ADDRESS;
	m_clientSocket					= -1;
    m_serverSocketPort				= TCP_SERVER_PORT;
}



TCP::CTcpClient::CTcpClient(int p_serverSocketPort, string p_serverSocketIpAddr)
{
	m_serverIpAddress				= p_serverSocketIpAddr;
	m_clientSocket					= -1;
	m_serverSocketPort				= p_serverSocketPort;
}



TCP::CTcpClient::~CTcpClient()
{
    // Close the client socket
    	close(m_clientSocket);
}



int TCP::CTcpClient::initTcpClient()
{
	socklen_t 	l_serverSocketAddrSize;

    // Create the client socket
		m_clientSocket = socket(AF_INET, SOCK_STREAM, 0);
		if (m_clientSocket == -1)
		{
			return 1;
		}

	// Connect the client socket to the server one
		m_serverSocketAddr.sin_family 	= AF_INET;
		m_serverSocketAddr.sin_port 	= htons(m_serverSocketPort);
		if(inet_pton(AF_INET, m_serverIpAddress.c_str(), &m_serverSocketAddr.sin_addr) != 1)
		{
			return -1;
		}

		l_serverSocketAddrSize = sizeof(m_serverSocketAddr);
		if(connect(m_clientSocket, (sockaddr*)&m_serverSocketAddr, l_serverSocketAddrSize) == -1)
		{
			return -1;
		}

	return m_clientSocket;
}



int TCP::CTcpClient::requestMsgToServer(uint32_t p_requestedMsgId, void* p_requestedMsgBuffer)
{
	uint32_t 	l_requestedMsgBufferBytesSize;
	int			l_receivedMsgBufferBytesSize;

	// Send the requested message ID to TCP server
		if(send(m_clientSocket, &p_requestedMsgId, sizeof(uint32_t), 0) == -1)
		{
			return -1;
		}

	// Wait for the TCP server response
		switch(p_requestedMsgId)
		{
			case MSG_ID_PATH:
				l_requestedMsgBufferBytesSize = sizeof(SPathMsg);
				break;

			case MSG_ID_PATH_CORRECTION:
				l_requestedMsgBufferBytesSize = sizeof(SPathCorrectionMsg);
				break;

			case MSG_ID_WORKSHOP_ORDER:
				l_requestedMsgBufferBytesSize = sizeof(SWorkShopOrderMsg);
				break;

			case MSG_ID_STOP:
				l_requestedMsgBufferBytesSize = sizeof(SStopMsg);
				break;

			case MSG_ID_WORKSHOP_REPORT:
				l_requestedMsgBufferBytesSize = sizeof(SWorkShopReportMsg);
				break;

			case MSG_ID_BIT_REPORT:
				l_requestedMsgBufferBytesSize = sizeof(SBitReportMsg);
				break;

			case MSG_ID_ERROR:
				l_requestedMsgBufferBytesSize = sizeof(SErrorMsg);
				break;

			default:
				return -1;
		}
		l_receivedMsgBufferBytesSize = recv(m_clientSocket, p_requestedMsgBuffer, l_requestedMsgBufferBytesSize, 0);
		if (l_receivedMsgBufferBytesSize == -1)
		{

			return -1;
		}

	return 1;
}
