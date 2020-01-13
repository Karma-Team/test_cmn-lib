/*
 * TcpCommunication.hpp
 *
 *  Created on: 13 janv. 2020
 *      Author: ahu
 */



#ifndef INC_TCPCOMMUNICATION_HPP_
#define INC_TCPCOMMUNICATION_HPP_



#include <iostream>
#include <sys/types.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <string.h>
#include <string>



#define TCP_SERVER_IP_ADDRESS	"127.0.0.1"
#define TCP_SERVER_PORT			54000
#define BUFFER_SIZE				4096



using namespace std;



#endif /* INC_TCPCOMMUNICATION_HPP_ */
