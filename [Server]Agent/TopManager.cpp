#include "stdafx.h"
#include "TopManager.h"
#include "ServerSystem.h"
#include "Network.h"
#include "mmSystem.h"
#include "AgentDBMsgParser.h"
#include "TimerManager.h"
#include "../[CC]JACK/JackJack.h"
//DWORD TIMING;
//DWORD TIMING2;
//DWORD ChatMsg1;
//DWORD ChatMsg2;
GLOBALTON(TopManager)
TopManager::TopManager()
{
#ifndef _OZMSOTHROW_
   m_LastTime=timeGetTime();
#endif
   wTodayDate=0;
}
TopManager::~TopManager()
{
}
void TopManager::InitTopList()
{
	GetHeroTopList(MAX_TOP_NUM);
	GetMunpTopList(MAX_TOP_NUM);
	GetDungeonTopList(MAX_TOP_NUM);

	SYSTEMTIME sysTime;
	GetLocalTime( &sysTime );
	g_Console.LOG(4, "Rank Manager INIT : [%04d-%02d-%02d %02d:%02d:%02d]",
		sysTime.wYear, sysTime.wMonth, sysTime.wDay, sysTime.wHour, sysTime.wMinute, sysTime.wSecond);

	



#ifndef _OZMSOTHROW_
	g_Console.LOG(4, "Rank Manager Update every 10 minutes");
	m_LastTime=timeGetTime();
#else
	g_Console.LOG(4, "Rank Manager Update every midnight");
	wTodayDate=TodayDate();
#endif
}
SYSTEMTIME TopManager::TodayFullDate()
{
	stTIME Time;
	SYSTEMTIME sysTime;
	GetLocalTime(&sysTime);
	Time.SetTime(sysTime.wYear,sysTime.wMonth,sysTime.wDay,sysTime.wHour,sysTime.wMinute,sysTime.wSecond);
	return sysTime;
}
WORD TopManager::TodayDate()
{
	stTIME Time;
	SYSTEMTIME sysTime;
	GetLocalTime(&sysTime);
	Time.SetTime(sysTime.wYear,sysTime.wMonth,sysTime.wDay,sysTime.wHour,sysTime.wMinute,sysTime.wSecond);
	return sysTime.wDay;
}
void TopManager::Process()
{
	DWORD Currtime=timeGetTime();
#ifndef _OZMSOTHROW_
	if(Currtime-m_LastTime>600000)
#else
	if(wTodayDate!=TodayDate())
#endif
	{
		GetHeroTopList(MAX_TOP_NUM);
		GetMunpTopList(MAX_TOP_NUM);
		GetDungeonTopList(MAX_TOP_NUM);

		SYSTEMTIME sysTime;
		GetLocalTime( &sysTime );
		g_Console.LOG(4, "Rank Updated : [%04d-%02d-%02d %02d:%02d:%02d]",
			sysTime.wYear, sysTime.wMonth, sysTime.wDay, sysTime.wHour, sysTime.wMinute, sysTime.wSecond);

		TIMERMGR->SendMessageToClient();
#ifndef _OZMSOTHROW_		
		m_LastTime=timeGetTime();
#endif
	}
}
void TopManager::SetHeroTopList(SEND_TOPLIST * m_HeroList)
{
	memset(&m_HeroTopList,0,sizeof(SEND_TOPLIST));
	memcpy(&m_HeroTopList,m_HeroList,sizeof(SEND_TOPLIST));
}
void TopManager::SetMunpTopList(SEND_TOPLIST * m_MunpList)
{
	memset(&m_MunpTopList,0,sizeof(SEND_TOPLIST));
	memcpy(&m_MunpTopList,m_MunpList,sizeof(SEND_TOPLIST));
}
void TopManager::SetDungeonTopList(SEND_TOPLIST * m_DungeonList)
{
	memset(&m_DungeonTopList, 0, sizeof(SEND_TOPLIST));
	memcpy(&m_DungeonTopList,m_DungeonList,sizeof(SEND_TOPLIST));
}
void TopManager::SendAllHeroTopListInfo()
{
#ifdef _OZMSOTHROW_
	wTodayDate=TodayDate();
#endif
	USERINFO* pInfo = NULL;
	g_pUserTable->SetPositionUserHead();
	while( pInfo = (USERINFO *)g_pUserTable->GetUserData() )
	{	
		SEND_TOPLIST msgTemp = m_HeroTopList;
		g_Network.Send2User( pInfo->dwConnectionIndex, (char*)&msgTemp, msgTemp.GetSize());
	}
}
void TopManager::SendHeroTopListInfo(DWORD dwConnectionIdx)
{
	SEND_TOPLIST msgTemp = m_HeroTopList;
	g_Network.Send2User(dwConnectionIdx, (char*)&msgTemp, msgTemp.GetSize());
}
void TopManager::SendAllMunpTopListInfo()
{
	USERINFO* pInfo = NULL;
	g_pUserTable->SetPositionUserHead();
	while( pInfo = (USERINFO *)g_pUserTable->GetUserData() )
	{	
		SEND_TOPLIST msgTemp = m_MunpTopList;
		g_Network.Send2User( pInfo->dwConnectionIndex, (char*)&msgTemp, msgTemp.GetSize());
	}
}
void TopManager::SendMunpTopListInfo(DWORD dwConnectionIdx)
{
	SEND_TOPLIST msgTemp = m_MunpTopList;
	g_Network.Send2User(dwConnectionIdx, (char*)&msgTemp, msgTemp.GetSize());
}

void TopManager::SendAllDungeonTopListInfo()
{
	USERINFO* pInfo = NULL;
	g_pUserTable->SetPositionUserHead();
	while (pInfo = (USERINFO *)g_pUserTable->GetUserData())
	{
		SEND_TOPLIST msgTemp = m_DungeonTopList;
		g_Network.Send2User(pInfo->dwConnectionIndex, (char*)&msgTemp, msgTemp.GetSize());
	}
}
void TopManager::SendDungeonTopListInfo(DWORD dwConnectionIdx)
{
	SEND_TOPLIST msgTemp = m_DungeonTopList;
	g_Network.Send2User(dwConnectionIdx, (char*)&msgTemp, msgTemp.GetSize());
}
void TopManager::LoadFlushTimeConfig()
{
 	/*m_FlushTime=::GetPrivateProfileInt("RankingManager","RefreshTime1",0,".\\GameConfig.ini");
	TIMING=m_FlushTime*60000;
	g_Console.LOC(0x999933,4, "Rank update every : [%d minutes], (%dms)",m_FlushTime,TIMING);
	m_FlushTime2=::GetPrivateProfileInt("RankingManager","RefreshTime2",0,".\\GameConfig.ini");
	TIMING2=m_FlushTime2*60000;
	g_Console.LOC(0x999933,4, "Rank update every : [%d minutes], (%dms)",m_FlushTime2,TIMING2);
	m_ChatMsg1=::GetPrivateProfileInt("RankingManager","ChatMsg1",0,".\\GameConfig.ini");
	ChatMsg1=m_ChatMsg1;
	m_ChatMsg2=::GetPrivateProfileInt("RankingManager","ChatMsg2",0,".\\GameConfig.ini");
	ChatMsg2=m_ChatMsg2;*/
}