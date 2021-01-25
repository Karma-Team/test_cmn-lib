/**
 * "TCP_Common.cpp"
 **/



#include "TCP_Common.hpp"



#if TCP_DEBUG
void displayMsgInfoKeepAlive(SMsgInfoKeepAlive* p_msgInfoKeepAlive)
{
	cout << "> MSG_INFO_KEEP_ALIVE : ";
	cout << "[[hd | id=" << p_msgInfoKeepAlive->hd.id << " ; size=" << p_msgInfoKeepAlive->hd.size << "][body | ";
	cout << "tmp=" << p_msgInfoKeepAlive->body.isAlive << "]]" << endl;
}



void displayMsgInfoPosition(SMsgInfoPosition* p_msgInfoPosition)
{
	cout << "> MSG_INFO_POSITION : ";
	cout << "[[hd | id=" << p_msgInfoPosition->hd.id << " ; size=" << p_msgInfoPosition->hd.size << "][body | ";
	cout << "angle=" << p_msgInfoPosition->body.angle << " ; coord=(x=" << p_msgInfoPosition->body.coordinates.x << ", y=" << p_msgInfoPosition->body.coordinates.y << ")]] \n";

}



void displayMsgOrderBit(SMsgOrderBit* p_msgOrderBit)
{
	cout << "> MSG_ORDER_BIT : ";
	cout << "[[hd | id=" << p_msgOrderBit->hd.id << " ; size=" << p_msgOrderBit->hd.size << "][body | ";
	cout << "tmp=" << p_msgOrderBit->body.tmp << "]] \n";
}



void displayMsgOrderPath(SMsgOrderPath* p_msgOrderPath)
{
	cout << "> MSG_ORDER_PATH : ";
	cout << "[[hd | id=" << p_msgOrderPath->hd.id << " ; size=" << p_msgOrderPath->hd.size << "][body | ";
	cout << "pointsNb=" << p_msgOrderPath->body.pointsNb << " ; points=(";
	for(uint32_t i = 0 ; i < p_msgOrderPath->body.pointsNb ; i++)
	{
		cout << "(x : " << p_msgOrderPath->body.points[i].x << " | ";
		cout << "y : " 	<< p_msgOrderPath->body.points[i].y << ")";
	}
	cout << ")]] \n";
}



void displayMsgOrderPathCorr(SMsgOrderPathCorr* p_msgOrderPathCorr)
{
	cout << "> MSG_ORDER_PATH_CORR : ";
	cout << "[[hd | id=" << p_msgOrderPathCorr->hd.id << " ; size=" << p_msgOrderPathCorr->hd.size << "][body | ";
	cout << "lastIdValid=" << p_msgOrderPathCorr->body.lastIdValid << " ; pointsNb=" << p_msgOrderPathCorr->body.pointsNb << " ; points=(";
	for(uint32_t i = 0 ; i < p_msgOrderPathCorr->body.pointsNb ; i++)
	{
		cout << "(x : " << p_msgOrderPathCorr->body.points[i].x << " | ";
		cout << "y : " 	<< p_msgOrderPathCorr->body.points[i].y << ")";
	}
	cout << ")]] \n";
}



void displayMsgOrderWorkShop(SMsgOrderWorkShop* p_msgOrderWorkShop)
{
	cout << "> MSG_ORDER_WORKSHOP : ";
	cout << "[[hd | id=" << p_msgOrderWorkShop->hd.id << " ; size=" << p_msgOrderWorkShop->hd.size << "][body | ";
	cout << "tmp=" << p_msgOrderWorkShop->body.tmp << "]] \n";
}



void displayMsgOrderStop(SMsgOrderStop* p_msgOrderStop)
{
	cout << "> MSG_ORDER_STOP : ";
	cout << "[[hd | id=" << p_msgOrderStop->hd.id << " ; size=" << p_msgOrderStop->hd.size << "][body | ";
	cout << "tmp=" << p_msgOrderStop->body.tmp << "]] \n";
}



void displayMsgReportBit(SMsgReportBit* p_msgReportBit)
{
	cout << "> MSG_REPORT_BIT : ";
	cout << "[[hd | id=" << p_msgReportBit->hd.id << " ; size=" << p_msgReportBit->hd.size << "][body | ";
	cout << "tmp=" << p_msgReportBit->body.tmp << "]] \n";
}



void displayMsgReportWorkShop(SMsgReportWorkShop* p_msgReportWorkShop)
{
	cout << "> MSG_REPORT_WORKSHOP : ";
	cout << "[[hd | id=" << p_msgReportWorkShop->hd.id << " ; size=" << p_msgReportWorkShop->hd.size << "][body | ";
	cout << "tmp=" << p_msgReportWorkShop->body.tmp << "]] \n";
}
#endif
