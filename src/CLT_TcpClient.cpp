/*
 * CLT_TcpClient.cpp
 *
 *  Created on: 12 janv. 2020
 *      Author: ahu
 */



#include <CLT_TcpClient.hpp>



CLT::CTcpClient::CTcpClient()
{
    m_serverSocketAddrSize	= 0;
    m_serverIpAddress		= TCP_SERVER_IP_ADDRESS;
    m_userInput				= "";
    m_serverPort	 		= TCP_SERVER_PORT;
    m_receivedBytesNb 		= 0;
	m_clientSocket			= -1;
    strcpy(m_buffer, 		"");
}



CLT::CTcpClient::~CTcpClient()
{
    // Close the client socket
    	close(m_clientSocket);
		cout << "TCP client socket closed" << endl;
}




int CLT::CTcpClient::initTcpClient()
{
	cout << "Initialize the TCP client" << endl;

    // Create the client socket
		m_clientSocket = socket(AF_INET, SOCK_STREAM, 0);
		if (m_clientSocket == -1)
		{
			cerr << "Can't create the client socket! Quitting" << endl;
			return 1;
		}

	// Connect the client socket to the server one
		m_serverSocketAddr.sin_family 	= AF_INET;
		m_serverSocketAddr.sin_port 	= htons(m_serverPort);
		if(inet_pton(AF_INET, m_serverIpAddress.c_str(), &m_serverSocketAddr.sin_addr) != 1)
		{
			cerr << "Can't convert the Internet address! Quitting" << endl;
			return -1;
		}

		m_serverSocketAddrSize = sizeof(m_serverSocketAddr);
		if(connect(m_clientSocket, (sockaddr*)&m_serverSocketAddr, m_serverSocketAddrSize) == -1)
		{
			cerr << "Can't connect the client socket to the server one! Quitting" << endl;
			return -1;
		}

	return m_clientSocket;
}



int CLT::CTcpClient::startTcpClient()
{
	cout << "Start the TCP client" << endl;

	if(m_clientSocket != -1)
	{
	    //	While loop: send and receive message back from server
			do
			{
				// Enter lines of text
					cout << "> ";
					getline(cin, m_userInput);

				// Send data to server
					if(send(m_clientSocket, m_userInput.c_str(), m_userInput.size() + 1, 0) == -1)
					{
						cout << "Can't send data to server! Please try again \r\n";
						continue;
					}

				// Wait for response from server and then display it
					memset(m_buffer, 0, 4096);
					m_receivedBytesNb = recv(m_clientSocket, m_buffer, 4096, 0);
					if (m_receivedBytesNb == -1)
					{
						cerr << "Error in recv()!" << endl;
					}
					else
					{
						cout << "SERVER > " << string(m_buffer, m_receivedBytesNb) << "\r\n";
					}
			} while(true);

	    //	Close the client socket
	    	close(m_clientSocket);
	}
	else
	{
		cerr << "TCP client is not initialized! Quitting" << endl;
		return -1;
	}

	return 1;
}
