/*
 * SRV_TcpServer.cpp
 *
 *  Created on: 12 janv. 2020
 *      Author: ahu
 */



#include <SRV_TcpServer.hpp>



SRV::CTcpServer::CTcpServer()
{
    m_serverSocketAddrSize	= 0;
    m_clientSocketAddrSize	= 0;
    m_serverIpAddress		= TCP_SERVER_IP_ADDRESS;
    m_serverPort			= TCP_SERVER_PORT;
    m_receivedBytesNb 		= 0;
	m_serverSocket			= -1;
	m_clientSocket			= -1;
    strcpy(m_buffer, 		"");
    strcpy(m_clientName, 	"");
    strcpy(m_clientPort, 	"");
}



SRV::CTcpServer::~CTcpServer()
{
    // Close the server socket
    	close(m_serverSocket);
		cout << "TCP server socket closed" << endl;
}



int SRV::CTcpServer::initTcpServer()
{
	cout << "Initialize the TCP server" << endl;

	// Create the TCP server socket
		m_serverSocket = socket(AF_INET, SOCK_STREAM, 0);
		if(m_serverSocket == -1)
		{
			cerr << "Can't create the server socket! Quitting" << endl;
			return -1;
		}
		cout << "TCP server socket created" << endl;

    // Assign an address and a port to the TCP server socket
		m_serverSocketAddr.sin_family	= AF_INET;
		m_serverSocketAddr.sin_port 	= htons(m_serverPort);
		if(inet_pton(AF_INET, m_serverIpAddress.c_str(), &m_serverSocketAddr.sin_addr) != 1)
		{
			cerr << "Can't convert the Internet address! Quitting" << endl;
			return -1;
		}

		m_serverSocketAddrSize = sizeof(m_serverSocketAddr);
		if(bind(m_serverSocket, (sockaddr*)&m_serverSocketAddr, m_serverSocketAddrSize) == -1)
		{
			cerr << "Can't assign an address and a port to the server socket! Quitting" << endl;
			return -1;
		}
		cout << "Address and port assigned to the TCP server socket" << endl;

    // Enable the TCP server socket to accept client connections
		if(listen(m_serverSocket, SOMAXCONN) == -1)
		{
			cerr << "Can't enable the server socket to accept connections! Quitting" << endl;
			return -1;
		}
		cout << "Client connections enabled to the TCP server socket" << endl;

	return m_serverSocket;
}



int SRV::CTcpServer::startTcpServer()
{
	cout << "Start the TCP server" << endl;

	if(m_serverSocket != -1)
	{
		// Wait for a client connection
			m_clientSocketAddrSize 	= sizeof(m_clientSocketAddr);
			m_clientSocket 			= accept(m_serverSocket, (sockaddr*)&m_clientSocketAddr, &m_clientSocketAddrSize);

		// Translate the client socket address to a location and a service name
			memset(m_clientName, 0, NI_MAXHOST);
			memset(m_clientPort, 0, NI_MAXSERV);
			if(getnameinfo((sockaddr*)&m_clientSocketAddr, sizeof(m_clientSocketAddr), m_clientName, NI_MAXHOST, m_clientPort, NI_MAXSERV, 0) == 0)
			{
				cout << m_clientName << " connected on port " << m_clientPort << endl;
			}
			else
			{
				inet_ntop(AF_INET, &m_clientSocketAddr.sin_addr, m_clientName, NI_MAXHOST);
				cout << m_clientName << " connected on port " << ntohs(m_clientSocketAddr.sin_port) << endl;
			}

		// While loop: accept and echo message back to client
			while(true)
			{
				// Initialize the buffer
					memset(m_buffer, 0, BUFFER_SIZE);

				// Wait for the client to send data
					m_receivedBytesNb = recv(m_clientSocket, m_buffer, BUFFER_SIZE, 0);
					if(m_receivedBytesNb == -1)
					{
						cerr << "Error in recv()! Quitting" << endl;
						break;
					}
					if(m_receivedBytesNb == 0)
					{
						cout << "Client disconnected! Quitting " << endl;
						break;
					}
					cout << "> " << string(m_buffer, 0, m_receivedBytesNb) << endl;

				// Echo message back to client
					if(send(m_clientSocket, m_buffer, m_receivedBytesNb + 1, 0) == -1)
					{
						cout << "Can't send data back to client! Quitting " << endl;
					}
			}

		// Close the client socket
			close(m_clientSocket);
	}
	else
	{
		cerr << "TCP server is not initialized! Quitting" << endl;
		return -1;
	}

	return 1;
}
