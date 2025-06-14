





#ifndef __DISTRIBUTEDBMSGPARSER_H__
#define __DISTRIBUTEDBMSGPARSER_H__

#include "db.h"

#include "CommonDBMsgParser.h"

//-----------------------------------------------------------------------
// DBÄõ¸® ¸®ÅÏ ÇÔ¼ö°¡ ´Ã¾î³¯¶§¸¶´Ù Ãß°¡
// °è¼Ó Ãß°¡µÇ´Â ºÎºÐ

enum 
{

	eNull = 0,
	eLoginCheckQuery,
	eIpCheckQuery,
	eLoginCheckQueryTest,
	eCheckDynamicPW,
	eLoginCheckQueryForJP,

	eHackToolUser,

	eRegister,
	eRestpwd,
};

void LoginCheckInit();

/*
BOOL LoginCheckQuery(char* id, char* pw, DWORD AuthKey, DWORD dwConnectionIndex);
void LoginCheckDelete(DWORD UserID);
*/

BOOL IPCheckQuery(char* ip, DWORD AuthKey, DWORD dwConnectionIndex);
void RIpCheckQuery(LPQUERY pData, LPDBMESSAGE pMessage);
/// 060911 PKH 홍콩IP체크
BOOL IPCheckQueryHK(char* ip, DWORD AuthKey, DWORD dwConnectionIndex);

BOOL LoginCheckQueryTest(char* id, char* pw, DWORD AuthKey, DWORD dwConnectionIndex);
void RLoginCheckQueryTest(LPQUERY pData, LPDBMESSAGE pMessage);

BOOL LoginGetDynamicPWQuery(char* id, DWORD dwConnectionIndex );
void RLoginGetDynamicPWQuery(LPMIDDLEQUERY pData, LPDBMESSAGE pMessage);

BOOL CheckDynamicPW( char* id, char* pw, char* ip, WORD ServerNo, DWORD AuthKey, DWORD dwConnectionIndex, DWORD dwMaxUser, WORD ServerNum );

void RCheckDynamicPW( LPQUERY pData, LPDBMESSAGE pMessage );

void LoginError(DWORD dwConnectionIdx, DWORD AuthKey,DWORD ErrorCode,DWORD dwParam = 0);

//* MemberDB ±³Ã¼ ÀÛ¾÷ 2004.03.22
BOOL LoginCheckQuery(char* id, char* pw, char* ip, WORD ServerNo, DWORD AuthKey, DWORD dwConnectionIndex, DWORD dwMaxUser, WORD ServerNum);
#ifdef _KOR_LOCAL_
BOOL LoginCheckQueryforMD5(char* id, char* pw, char* ip, WORD ServerNo, DWORD AuthKey, DWORD dwConnectionIndex, DWORD dwMaxUser, WORD ServerNum, char* pPN);
BOOL LoginCheckQueryEx(char* id, char* pw, char* ip, WORD ServerNo, DWORD AuthKey, DWORD dwConnectionIndex, DWORD dwMaxUser, WORD ServerNum, char* pPN,char*Mac, DWORD dwMaxClient);// 2015-02-08 ��½��������!
#endif
void LoginCheckDelete(DWORD UserID);
void RLoginCheckQuery(LPQUERY pData, LPDBMESSAGE pMessage);
void RLoginCheckQueryforMD5(LPMIDDLEQUERY pData, LPDBMESSAGE pMessage);
//*/


// for Japan

void LoginCheckQueryForJP( char* id, char* pw, char* ip, WORD ServerNo, DWORD AuthKey, DWORD dwConnectionIndex, DWORD dwMaxUser, WORD ServerNum );
void RLoginCheckQueryForJP( LPQUERY pData, LPDBMESSAGE pMessage );



#ifdef _NPROTECT_
//---for Hacktool User Check
void HackToolUser( DWORD UserIdx );
#endif



void RegQueryEx(DWORD dwConnectionIndex);
void RregQuery(LPQUERY pData, LPDBMESSAGE pMessage);

void RestPwd(DWORD dwConnectionIndex);
void Rrestpwd(LPQUERY pData, LPDBMESSAGE pMessage);



#endif //__DISTRIBUTEDBMSGPARSER_H__


