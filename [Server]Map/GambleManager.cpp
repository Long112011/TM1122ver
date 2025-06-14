
#include "stdafx.h"
#include <time.h>
#include "GambleManager.h"
#include "MapDBMsgParser.h"
#include "Player.h"


GambleManager::GambleManager()
{
   Number1=0;
   
   Number2=0;

   Number3=0;

}
GambleManager::~GambleManager()
{

}

void GambleManager::SetUserGambleInfo(CPlayer * pPlayer,int Number1,int Number2,int Number3)
{
	SYSTEMTIME st;

	GetLocalTime( &st );

	GAMBLE_USER_INFO * pGambInfo = pPlayer->GetGambleInfo();


	if(pPlayer->GetGoldMoney()<250)
	{
		SendGambleUserInfoErrorMsg(pPlayer,4); // 投注不足!！

		return;
	}

	if(pGambInfo)
	{
		if(pGambInfo->Gamble_Year == st.wYear && pGambInfo->Gamble_Month ==st.wMonth && pGambInfo->Gamble_Day ==st.wDay)
		{
            SendGambleUserInfoErrorMsg(pPlayer,1); // 投注!

			return;
		}
		
	}
	
	if(CheckGambleNumber(Number1) && CheckGambleNumber(Number2) && CheckGambleNumber(Number3))
	{
		GambleUserInfoInsert(pPlayer->GetID(),st.wYear,st.wMonth,st.wDay,Number1,Number2,Number3);

		return;
	}
	else
	{
		SendGambleUserInfoErrorMsg(pPlayer,0); // 投注数字不合法！

		return;
	}

}

BOOL GambleManager::CheckGambleNumber(int Number)
{

	if(Number<=6 && Number>0)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}

}

void GambleManager::SendGamblePublicInfoMsg(CPlayer * pPlayer,int Number1,int Number2,int Number3)
{
  	MSG_DWORD3 msg;
	msg.Category = MP_ITEM;
	msg.Protocol = MP_ITEM_GAMBLE_PUBLIC_ACK;
	msg.dwObjectID= pPlayer->GetID();
	msg.dwData1  = Number1;
	msg.dwData2  = Number2;
	msg.dwData3  = Number3;
	pPlayer->SendMsg(&msg, sizeof(msg));

}
void GambleManager::SendGambleUserInfoSucceedMsg(CPlayer * pPlayer,int Number1,int Number2,int Number3)
{
	MSG_DWORD3 msg;
	msg.Category = MP_ITEM;
	msg.Protocol = MP_ITEM_GAMBLE_SEND_ACK;
	msg.dwObjectID= pPlayer->GetID();
	msg.dwData1  = Number1;
	msg.dwData2  = Number2;
	msg.dwData3  = Number3;
	pPlayer->SendMsg(&msg, sizeof(msg));
}

void GambleManager::SendGambleUserInfoMapMsg(CPlayer * pPlayer,int Number1,int Number2,int Number3)
{
	MSG_DWORD3 msg;
	msg.Category = MP_ITEM;
	msg.Protocol = MP_ITEM_GAMBLE_MAP_ACK;
	msg.dwObjectID= pPlayer->GetID();
	msg.dwData1  = Number1;
	msg.dwData2  = Number2;
	msg.dwData3  = Number3;
	pPlayer->SendMsg(&msg, sizeof(msg));
}

void GambleManager::SendGambleUserInfoOpenMsg(CPlayer * pPlayer,int Level,DWORD Gold)
{
	MSG_DWORD2 msg;
	msg.Category = MP_ITEM;
	msg.Protocol = MP_ITEM_GAMBLE_OPEN_ACK;
	msg.dwObjectID= pPlayer->GetID();
	msg.dwData1  = Level;
	msg.dwData2  = Gold;
	pPlayer->SendMsg(&msg, sizeof(msg));
	pPlayer->SetGoldMoney(Gold,4);
}
void GambleManager::SendGambleUserInfoErrorMsg(CPlayer * pPlayer, int ErrorCode)
{
  	MSG_DWORD msg;
	msg.Category = MP_ITEM;
	msg.Protocol = MP_ITEM_GAMBLE_SEND_NACK;
	msg.dwObjectID= pPlayer->GetID();
	msg.dwData    =ErrorCode;
	pPlayer->SendMsg(&msg, sizeof(msg));
}