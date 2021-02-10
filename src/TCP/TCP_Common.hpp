/**
 * "TCP_Common.hpp"
 **/

#ifndef INC_TCPCOMMON_HPP_
#define INC_TCPCOMMON_HPP_

#include <iostream>
#include <string>
#include <cstdint>
#include <thread>
#include <mutex>
#include <sys/types.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>
#include <common.hpp>

#define TCP_SERVER_IP_ADDRESS	"127.0.0.1"
#define TCP_SERVER_PORT			54000

enum EIdMsg {
	// MAT vers ROBOT
		// INFO
		ID_MSG_INFO_KEEP_ALIVE	= 0x001,
		ID_MSG_INFO_POSITION	= 0x002,

		// ORDER
		ID_MSG_ORDER_BIT		= 0x013,
		ID_MSG_ORDER_PATH		= 0x014,
		ID_MSG_ORDER_PATH_CORR	= 0x015,
		ID_MSG_ORDER_WORKSHOP	= 0x016,
		ID_MSG_ORDER_STOP		= 0x017,

	// ROBOT vers MAT
		// REPORT
		ID_MSG_REPORT_BIT		= 0x101,
		ID_MSG_REPORT_WORKSHOP	= 0x102
};

struct SMsgHeader {
	uint32_t 				id;
	uint32_t 				size;
};

struct SMsgInfoKeepAliveBody {
	bool	 				isAlive;
};

struct SMsgInfoKeepAlive {
	SMsgHeader 				hd;
	SMsgInfoKeepAliveBody	body;
};

struct SMsgInfoPositionBody {
	SPoint					coordinates;
	int16_t 				angle;
};

struct SMsgInfoPosition {
	SMsgHeader 				hd;
	SMsgInfoPositionBody	body;
};

struct SMsgOrderBitBody {
	uint32_t				tmp;
};

struct SMsgOrderBit {
	SMsgHeader 				hd;
	SMsgOrderBitBody		body;
};

struct SMsgOrderPathBody {
	uint32_t				pointsNb;
	SPoint					points[MAX_PATH_POINTS];
};

struct SMsgOrderPath {
	SMsgHeader 				hd;
	SMsgOrderPathBody		body;
};

struct SMsgOrderPathCorrBody {
	uint32_t				lastIdValid;
	uint32_t				pointsNb;
	SPoint					points[MAX_PATH_POINTS];
};

struct SMsgOrderPathCorr {
	SMsgHeader 				hd;
	SMsgOrderPathCorrBody 	body;
};

struct SMsgOrderWorkShopBody {
	uint32_t 				tmp;
};

struct SMsgOrderWorkShop {
	SMsgHeader 				hd;
	SMsgOrderWorkShopBody 	body;
};

struct SMsgOrderStopBody {
	uint32_t 				tmp;
};

struct SMsgOrderStop {
	SMsgHeader 				hd;
	SMsgOrderStopBody 		body;
};

struct SMsgReportWorkShopBody {
	uint32_t 				tmp;
};

struct SMsgReportWorkShop {
	SMsgHeader 				hd;
	SMsgReportWorkShopBody 	body;
};

struct SMsgReportBitBody {
	uint32_t 				tmp;
};

struct SMsgReportBit {
	SMsgHeader 				hd;
	SMsgReportBitBody 		body;
};

struct SMsgErrorBody {
	uint32_t 				tmp;
};

void displayMsgInfoKeepAlive( SMsgInfoKeepAlive* p_msgInfoKeepAlive );
void displayMsgInfoPosition( SMsgInfoPosition* p_msgInfoPosition );
void displayMsgOrderBit( SMsgOrderBit* p_msgOrderBit );
void displayMsgOrderPath( SMsgOrderPath* p_msgOrderPath );
void displayMsgOrderPathCorr( SMsgOrderPathCorr* p_msgOrderPathCorr );
void displayMsgOrderWorkShop( SMsgOrderWorkShop* p_msgOrderWorkShop );
void displayMsgOrderStop( SMsgOrderStop* p_msgOrderStop );
void displayMsgReportBit( SMsgReportBit* p_msgReportBit );
void displayMsgReportWorkShop( SMsgReportWorkShop* p_msgReportWorkShop );

#endif /* INC_TCPCOMMON_HPP_ */
