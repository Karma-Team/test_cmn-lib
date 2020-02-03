/**
 * "TCP_Common.hpp"
 **/



#ifndef INC_TCPCOMMON_HPP_
#define INC_TCPCOMMON_HPP_



#include <iostream>
#include <sys/types.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdlib.h>
#include <string>
#include <thread>



#define TCP_SERVER_IP_ADDRESS	"127.0.0.1"
#define TCP_SERVER_PORT			54000
#define MAX_PATH_POINTS			10



using namespace std;



enum EMsgId
{
	MSG_ID_UNKNOWN			= 0x00,
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
	uint32_t 				id;
	uint32_t 				size;
};



struct SPathMsgBody
{
	uint32_t				pointsNb;
	int32_t					xyPointsArray[MAX_PATH_POINTS];
};



struct SPathMsg
{
	SMsgHeader 				hd;
	SPathMsgBody			body;
};



struct SPathCorrectionMsgBody
{
	uint32_t				lastIdValid;
	uint32_t				pointsNb;
	int32_t					xyCorrectionPointsArray[MAX_PATH_POINTS];
};



struct SPathCorrectionMsg
{
	SMsgHeader 				hd;
	SPathCorrectionMsgBody 	body;
};



struct SWorkShopOrderMsgBody
{
	uint32_t 				tmp;
};



struct SWorkShopOrderMsg
{
	SMsgHeader 				hd;
	SWorkShopOrderMsgBody 	body;
};



struct SStopMsgBody
{
	uint32_t 				tmp;
};



struct SStopMsg
{
	SMsgHeader 				hd;
	SStopMsgBody 			body;
};



struct SWorkShopReportMsgBody
{
	uint32_t 				tmp;
};



struct SWorkShopReportMsg
{
	SMsgHeader 				hd;
	SWorkShopReportMsgBody 	body;
};



struct SBitReportMsgBody
{
	uint32_t 				tmp;
};



struct SBitReportMsg
{
	SMsgHeader 				hd;
	SBitReportMsgBody 		body;
};



struct SErrorMsgBody
{
	uint32_t 				tmp;
};



struct SErrorMsg
{
	SMsgHeader 				hd;
	SErrorMsgBody 			body;
};



#endif /* INC_TCPCOMMON_HPP_ */
