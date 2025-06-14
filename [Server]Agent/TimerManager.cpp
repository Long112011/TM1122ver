#include "StdAfx.h"
#include "TimerManager.h"
#include "mmSystem.h"
#include "ServerSystem.h"
#include "Network.h"
//extern DWORD TIMING2;
//extern DWORD ChatMsg1;
//extern DWORD ChatMsg2;
GLOBALTON(TimerManager)
TimerManager::TimerManager()
{
	//m_CurTime=timeGetTime();
}
TimerManager::~TimerManager()
{
	//m_CurTime=0;
}
void TimerManager::Process()
{
	/*DWORD Currtime=timeGetTime();

	if(Currtime-m_CurTime>TIMING2)
	{
		SendMessageToClient();

		m_CurTime=timeGetTime();
	}*/
}
void TimerManager::SendMessageToClient()
{
	MSG_TIMER msg;
	msg.Category=MP_CHAR;
	msg.Protocol=MP_CHAR_TIMER_ACK;
	msg.ChatMsgId1=2616;
	msg.ChatMsgId2=2617;
	USERINFO* pInfo = NULL;
	g_pUserTable->SetPositionUserHead();
	while( pInfo = (USERINFO *)g_pUserTable->GetUserData() )
	{	
		MSG_TIMER msgTemp = msg;
		g_Network.Send2User( pInfo->dwConnectionIndex, (char*)&msgTemp,sizeof(msgTemp));
	}
}

void TimerManager::SendMessageToClientItemLink(DWORD dwCharIdx)
{
	MSG_DWORD3 msg;
	msg.Category=MP_ITEMEXT;
	msg.Protocol=MP_ITEMEXT_SELECT_NACK;
	msg.dwObjectID	= dwCharIdx;
	msg.dwData1		= 1;
	//msg.ChatMsgId1=2616;
	//msg.ChatMsgId2=2617;
	USERINFO* pInfo = NULL;
	g_pUserTable->SetPositionUserHead();
	while( pInfo = (USERINFO *)g_pUserTable->GetUserData() )
	{	
		MSG_DWORD3 msgTemp = msg;
		g_Network.Send2User( pInfo->dwConnectionIndex, (char*)&msgTemp,sizeof(msgTemp));
	}
}