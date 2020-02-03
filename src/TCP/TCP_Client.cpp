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
		cout << "> TCP client socket closed" << endl;
}



int TCP::CTcpClient::initTcpClient()
{
	cout << "> Initialize the TCP client" << endl;

	socklen_t 	l_serverSocketAddrSize;

    // Create the client socket
		m_clientSocket = socket(AF_INET, SOCK_STREAM, 0);
		if (m_clientSocket == -1)
		{
			cerr << "> Can't create the client socket! Quitting" << endl;
			return 1;
		}

	// Connect the client socket to the server one
		m_serverSocketAddr.sin_family 	= AF_INET;
		m_serverSocketAddr.sin_port 	= htons(m_serverSocketPort);
		if(inet_pton(AF_INET, m_serverIpAddress.c_str(), &m_serverSocketAddr.sin_addr) != 1)
		{
			cerr << "> Can't convert the Internet address! Quitting" << endl;
			return -1;
		}

		l_serverSocketAddrSize = sizeof(m_serverSocketAddr);
		if(connect(m_clientSocket, (sockaddr*)&m_serverSocketAddr, l_serverSocketAddrSize) == -1)
		{
			cerr << "> Can't connect the client socket to the server one! Quitting" << endl;
			return -1;
		}

	return m_clientSocket;
}



int TCP::CTcpClient::requestedMsgToServer(uint32_t p_requestedMsgId, void* p_requestedMsgBuffer)
{
	uint32_t 	l_requestedMsgBufferBytesSize;
	int			l_receivedMsgBufferBytesSize;

	// Send the requested message ID to TCP server
		if(send(m_clientSocket, &p_requestedMsgId, sizeof(uint32_t), 0) == -1)
		{
			cout << "> Can't send requested message ID to server! Please try again\n";
			return -1;
		}
		cout << "> Requested message ID sent to server\n";

	// Wait for the TCP server response
		switch(p_requestedMsgId)
		{
			case MSG_ID_PATH:
				cout << "> Wait for response from server : MSG_ID_PATH\n";
				l_requestedMsgBufferBytesSize = sizeof(SPathMsg);
				break;

			case MSG_ID_PATH_CORRECTION:
				cout << "> Wait for response from server : MSG_ID_PATH_CORRECTION\n";
				l_requestedMsgBufferBytesSize = sizeof(SPathCorrectionMsg);
				break;

			case MSG_ID_WORKSHOP_ORDER:
				cout << "> Wait for response from server : MSG_ID_WORKSHOP_ORDER\n";
				l_requestedMsgBufferBytesSize = sizeof(SWorkShopOrderMsg);
				break;

			case MSG_ID_STOP:
				cout << "> Wait for response from server : MSG_ID_STOP\n";
				l_requestedMsgBufferBytesSize = sizeof(SStopMsg);
				break;

			case MSG_ID_WORKSHOP_REPORT:
				cout << "> Wait for response from server : MSG_ID_WORKSHOP_REPORT\n";
				l_requestedMsgBufferBytesSize = sizeof(SWorkShopReportMsg);
				break;

			case MSG_ID_BIT_REPORT:
				cout << "> Wait for response from server : MSG_ID_BIT_REPORT\n";
				l_requestedMsgBufferBytesSize = sizeof(SBitReportMsg);
				break;

			case MSG_ID_ERROR:
				cout << "> Wait for response from server : MSG_ID_ERROR\n";
				l_requestedMsgBufferBytesSize = sizeof(SErrorMsg);
				break;

			default:
				cout << "> Unknown message ID\n";
				return -1;
		}
		l_receivedMsgBufferBytesSize = recv(m_clientSocket, p_requestedMsgBuffer, l_requestedMsgBufferBytesSize, 0);
		if (l_receivedMsgBufferBytesSize == -1)
		{
			cerr << "> Error in recv()!" << endl;
			return -1;
		}

	return 1;
}
