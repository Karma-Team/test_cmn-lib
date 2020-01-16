/*
 * COM_TcpCommon.hpp
 *
 *  Created on: 13 janv. 2020
 *      Author: ahu
 */



#ifndef INC_COMTCPCOMMON_HPP_
#define INC_COMTCPCOMMON_HPP_



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
#define MAX_PATH_POINTS			10



using namespace std;



enum EMsgId
{
	MSG_ID_PATH				= 0x01,
	MSG_ID_PATH_CORRECTION	= 0x02,
	MSG_ID_WORKSHOP_ORDER	= 0x03,
	MSG_ID_STOP				= 0x04,
	MSG_ID_WORKSHOP_REPORT	= 0x05,
	MSG_ID_BIT_REPORT		= 0x06,
	MSG_ID_ERROR			= 0x07
};



struct SMsgHeader
{
	uint32_t id;
	uint32_t size;
};



struct SPathMsgBody
{
	SMsgHeader 	hd;
	uint32_t	pointsNb;
	int32_t		xyPointsArray[MAX_PATH_POINTS];
};



struct SPathCorrectionMsgBody
{
	SMsgHeader 	hd;
	uint32_t	lastIdValid;
	uint32_t	pointsNb;
	int32_t		xyCorrectionPointsArray[MAX_PATH_POINTS];
};



struct SWorkShopOrderMsgBody
{
	SMsgHeader 	hd;
	// ...
};



struct SStopMsgBody
{
	SMsgHeader 	hd;
	// ...
};



struct SWorkShopReportMsgBody
{
	SMsgHeader 	hd;
	// ...
};



struct SBitReportMsgBody
{
	SMsgHeader 	hd;
	// ...
};



struct SErrorMsgBody
{
	SMsgHeader 	hd;
	// ...
};



#endif /* INC_COMTCPCOMMON_HPP_ */
