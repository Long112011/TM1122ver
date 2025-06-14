







// ChaseDialog.cpp: implementation of the CChaseDialog class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "GuildWarDialog.h"
#include "WindowIDEnum.h"
#include "MHMap.h"
#include "MHFile.h"
#include "GameResourceManager.h"
#include "./Interface/cStatic.h"
#include "./Interface/cTextArea.h"
#include "./Interface/cScriptManager.h"
#include "ChatManager.h"
#include "MHMap.h"
#include "GuildWarManager.h"
#include "ObjectManager.h"
#include "GameIn.h"
#include "MHTimeManager.h"
CGuildWarDialog::CGuildWarDialog()
{
	pPushTime = gCurTime;
	pLostTime=0;
	pLostTimeTick=0;
	pGuildInfoTable.Initialize(1024);
	pIsClose = FALSE;
}

CGuildWarDialog::~CGuildWarDialog()
{
	GUILD_WAR_GUILD_VALUE_CLIENT* pInfo=NULL;
	pGuildInfoTable.SetPositionHead();
	while(pInfo = pGuildInfoTable.GetData())
		delete pInfo;
	pGuildInfoTable.RemoveAll();
}


void CGuildWarDialog::Linking()
{
	for(int i= 0;i<3;i++)
	{
		pGuildListSta[i] = (cStatic* )GetWindowForID(GU_GUILD_WAR_GUILD_STA1+i);
	}
	pCharacterKillSta = (cStatic* )GetWindowForID(GU_GUILD_WAR_PLAYER_KILL_STA);
	pCharacterDieSta = (cStatic* )GetWindowForID(GU_GUILD_WAR_PLAYER_DIE_STA);

	pLostTimeSta = (cStatic* )GetWindowForID(GW_LOSTTIME_STA);
	GUILDWARMGR->SetWarMap(FALSE);
}

void CGuildWarDialog::InitGuildInfo(MSG_GUILD_WAR_GUILD_INFO* pInfo)
{
	pLostTime = pInfo->pLostTime;
	GUILD_WAR_GUILD_VALUE_CLIENT* pGuildInfo=NULL;
	for (int i =0;i<pInfo->pCount;i++)
	{
		pGuildInfo = pGuildInfoTable.GetData(pInfo->pGuildInfo[i].pGuildIDX);
		if(!pGuildInfo)
		{
			pGuildInfo = new GUILD_WAR_GUILD_VALUE_CLIENT;
			pGuildInfo->pGuildIDX = pInfo->pGuildInfo[i].pGuildIDX;
			SafeStrCpy(pGuildInfo->pGuildName,pInfo->pGuildInfo[i].pGuildName,MAX_GUILD_NAME+1);
			pGuildInfo->pKillCount = pInfo->pGuildInfo[i].pKillCount;
			pGuildInfo->pDieCount = pInfo->pGuildInfo[i].pDieCount;
			pGuildInfo->pRead = FALSE;
			pGuildInfoTable.Add(pGuildInfo,pGuildInfo->pGuildIDX);
		}
		else
		{
			pGuildInfo->pKillCount = pInfo->pGuildInfo[i].pKillCount;
			pGuildInfo->pDieCount = pInfo->pGuildInfo[i].pDieCount;
		}
	}

	if(pInfo->pState == eGuildState_WarIng)
	{
		/*if(!IsActive())
			SetActive(TRUE);*/
	}
	else
		SetActive(FALSE);

	if(!GUILDWARMGR->IsWarMap())
	{
		GUILDWARMGR->SetWarMap(TRUE);
		SetActive(TRUE);
	}

	GAMEIN->GetMiniMapDialog()->ShwoGuildWarBtn(TRUE);
}

void CGuildWarDialog::InitHeroInfo(DWORD pKillCount,DWORD pDieCount,WORD pState)
{
	char pTemp[MAX_PATH] = {0};
	sprintf(pTemp,CHATMGR->GetChatMsg(2620),pKillCount);
	pCharacterKillSta->SetStaticText(pTemp);

	sprintf(pTemp,CHATMGR->GetChatMsg(2621),pDieCount);
	pCharacterDieSta->SetStaticText(pTemp);

	if(pState == eGuildState_WarIng)
	{
		if(!IsActive() && !pIsClose)
			SetActive(TRUE);
	}
	else
		SetActive(FALSE);
	
}

void CGuildWarDialog::Process()
{
	if(!IsActive())
		return;

	if(pLostTime >= 0 && gCurTime - pLostTimeTick >= 1000)
	{
		pLostTimeTick = gCurTime;
		char pTemp[MAX_PATH] = {0};
		sprintf(pTemp,"%02d:%02d",(pLostTime%3600)/60,pLostTime%3600%60);
		pLostTimeSta->SetStaticText(pTemp);
		pLostTime--;

		if (HERO->GetState() ==  eObjectState_Die)
		{
			if(gCurTime - GUILDWARMGR->GetDieTime() >= 20000)
			{
				CHATMGR->AddMsg(CTC_SYSMSG,CHATMGR->GetChatMsg(2631),60 - (WORD)((gCurTime - GUILDWARMGR->GetDieTime()) / 1000) );
			}
		}
	}

	if(gCurTime - pPushTime <1000)//5000
		return;
	pPushTime = gCurTime;

	GUILD_WAR_GUILD_VALUE_CLIENT* pMaxKillGuildInfo[3];

	WORD pMaxKill = 0;
	GUILD_WAR_GUILD_VALUE_CLIENT* pGuildInfo = NULL;
	GUILD_WAR_GUILD_VALUE_CLIENT* pMaxKillGuild = NULL;
	for(int i =0;i<3;i++)
	{
		pMaxKillGuildInfo[i] = NULL;

		pGuildInfoTable.SetPositionHead();
		while(pGuildInfo = pGuildInfoTable.GetData())
		{
			if(pGuildInfo->pKillCount >  pMaxKill && !pGuildInfo->pRead)
			{
				pMaxKillGuild = pGuildInfo;
				pMaxKill = pGuildInfo->pKillCount;
				//pGuildInfo->pRead = TRUE;
			}
		}
		if(pMaxKill && pMaxKillGuild)
		{
			pMaxKillGuildInfo[i] = pMaxKillGuild;
			pMaxKillGuildInfo[i]->pRead = TRUE;
		}

		pMaxKillGuild=NULL;
		pMaxKill = 0;		
	}


	





	for (int i =0;i<3;i++)
	{
		if(pMaxKillGuildInfo[i])
		{
			char pTemp[MAX_PATH] = {0};
			sprintf(pTemp,CHATMGR->GetChatMsg(2622),i+1,pMaxKillGuildInfo[i]->pGuildName,pMaxKillGuildInfo[i]->pKillCount);
			pGuildListSta[i]->SetStaticText(pTemp);
			pGuildListSta[i]->SetActive(TRUE);

			//JACK
			if(strcmp(pTempJack,pMaxKillGuildInfo[0]->pGuildName)==0)
			{				
				DWORD rgb=RGB_HALF(0,255,0);
				pGuildListSta[0]->SetFGColor(rgb);	
			}
			else
			{
				SafeStrCpy(pTempJack,pMaxKillGuildInfo[0]->pGuildName,MAX_GUILD_NAME+1);
				pGuildListSta[0]->SetFlickerRoar(true);
				CHATMGR->AddMsg(CTC_ALERT_RED,CHATMGR->GetChatMsg(2633));		
			}
		}
		else
			pGuildListSta[i]->SetActive(FALSE);
	}

	

	


	pGuildInfoTable.SetPositionHead();
	while(pGuildInfo = pGuildInfoTable.GetData())
	{
		pGuildInfo->pRead = FALSE;
	}
}