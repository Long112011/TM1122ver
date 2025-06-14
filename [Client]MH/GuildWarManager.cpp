







// MussangManager.cpp: implementation of the CMussangManager class.

//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "GuildWarManager.h"
#include "GameIn.h"
#include "ObjectManager.h"

#include "ChatManager.h"
#include "GuageDialog.h"
#include "ObjectGuagen.h"
#include "WindowIDEnum.h"
#include "StatusIconDlg.h"
#include "StatsCalcManager.h"
#include "CharacterDialog.h"
#include "ChatManager.h"
#include "cWindowManager.h"
#include "ObjectStateManager.h"
#include "input\UserInput.h"
#include "cMsgBox.h"
#include "MHMap.h"
GLOBALTON(CGuildWarManager)

CGuildWarManager::CGuildWarManager()
{
	pWarMapNum = 0;
	pLoginHeroID = 0;
	pIsWarMap=0;
}


CGuildWarManager::~CGuildWarManager()
{

}

void CGuildWarManager::NetworkMsgParse(WORD Protocol,void* pMsg)
{
	switch(Protocol)
	{
	case MP_GUILDWAR_DAY_INFO_SYN:
		{	
			if(HERO)
			{
				MSG_DWORD2* pmsg = (MSG_DWORD2*)pMsg;
				if(HEROID != pLoginHeroID)
				{
					WINDOWMGR->MsgBox( MBI_NOBTNMSGBOX, 1, CHATMGR->GetChatMsg(2630),pmsg->dwData1,pmsg->dwData2);
					SetLoginHeroID(HEROID);
				}
			}
		}
		break;
	case MP_GUILDWAR_NOTE_USER:
		{
			MSG_DWORD3* pmsg = (MSG_DWORD3*)pMsg;
			if(pmsg->dwData1 == eGuildState_WarIng)
			{
				pWarMapNum = pmsg->dwData2;
				pChannelNum = pmsg->dwData3-1;

				DWORD dwCheckBit = eEventMap | eTournament | eQuestRoom | eSurvivalMap | eInsDungeon;
				if(GAMEIN->GetGameInInitKind() == eGameInInitKind_SuryunEnter )
					break;
				if(MAP->IsMapKind(dwCheckBit))
					break;
				
					WINDOWMGR->MsgBox( MBI_GUILDWAR_MOVE_YESNO, MBT_YESNO, CHATMGR->GetChatMsg(2618) );
				
			}
			else if (pmsg->dwData1 == eGuildState_End)
			{
				CHATMGR->AddMsg(CTC_GTOURNAMENT,CHATMGR->GetChatMsg(2619));
			}
			else if (pmsg->dwData1 == 300)
			{
				CHATMGR->AddMsg(CTC_GTOURNAMENT,CHATMGR->GetChatMsg(2629),5);
			}
			else if (pmsg->dwData1 == 900)
			{
				CHATMGR->AddMsg(CTC_GTOURNAMENT,CHATMGR->GetChatMsg(2629),15);
			}
			else if (pmsg->dwData1 == 1800)
			{
				CHATMGR->AddMsg(CTC_GTOURNAMENT,CHATMGR->GetChatMsg(2629),30);
			}
			else if (GAMEIN->GetGuildWarDialog())
			{
				GAMEIN->GetGuildWarDialog()->SetActive(FALSE);
			}

		}
		break;
	case MP_GUILDWAR_GUILDWARINFO:
		{//门派积分排行
			MSG_GUILD_WAR_GUILD_INFO* pmsg = (MSG_GUILD_WAR_GUILD_INFO*)pMsg;
			if(GAMEIN->GetGuildWarDialog())
				GAMEIN->GetGuildWarDialog()->InitGuildInfo(pmsg);
		}
		break;
	case MP_GUILDWAR_CPLAYERWARINFO:
		{//个人击杀分数
			MSG_DWORD3* pmsg = (MSG_DWORD3*)pMsg;
			if(GAMEIN->GetGuildWarDialog())
				GAMEIN->GetGuildWarDialog()->InitHeroInfo(pmsg->dwData1,pmsg->dwData2,pmsg->dwData3);
		}
		break;
	case MP_GUILDWAR_PLAYERRANDLIST:
		{//角色奖励击杀排行
			MSG_GUILD_WAR_PLAYER_INFO* pmsg = (MSG_GUILD_WAR_PLAYER_INFO*)pMsg;
			for (int i =0;i<pmsg->pCount;i++)
			{
				CHATMGR->AddMsg(CTC_GTOURNAMENT,CHATMGR->GetChatMsg(2623),pmsg->pPlayerInfo[i].pPlayerName,pmsg->pPlayerInfo[i].pKillCount,i +1);
			}
		}
		break;
	case MP_GUILDWAR_GUILDRANDLIST:
		{//门派奖励积分排行
			MSG_GUILD_WAR_GUILD_INFO* pmsg = (MSG_GUILD_WAR_GUILD_INFO*)pMsg;
			for (int i =0;i<pmsg->pCount;i++)
			{
				CHATMGR->AddMsg(CTC_GTOURNAMENT,CHATMGR->GetChatMsg(2624),pmsg->pGuildInfo[i].pGuildName,pmsg->pGuildInfo[i].pKillCount,i+1);
			}
		}
		break;
	case  MP_GUILDWAR_MOVE_MAP_NACK:
		{
			MSG_DWORD2* pmsg = (MSG_DWORD2*)pMsg;
			switch(pmsg->dwData1)
			{
			case 1:
				{
					CHATMGR->AddMsg(CTC_SYSMSG,CHATMGR->GetChatMsg(1103));
				}
				break;
			case 2:
				{
					CHATMGR->AddMsg(CTC_SYSMSG,CHATMGR->GetChatMsg(2625),pmsg->dwData2);
				}
				break;
			case 3:
				{
					CHATMGR->AddMsg(CTC_SYSMSG,CHATMGR->GetChatMsg(2626),pmsg->dwData2);
				}
				break;
			case 4:
				{
					CHATMGR->AddMsg(CTC_SYSMSG,CHATMGR->GetChatMsg(2627));
				}
				break;
			case 5:
				{
					CHATMGR->AddMsg(CTC_SYSMSG,CHATMGR->GetChatMsg(2628));
				}
				break;
			}

			if( HERO->GetState() == eObjectState_Deal )
				OBJECTSTATEMGR->EndObjectState(HERO, eObjectState_Deal);
			g_UserInput.SetInputFocus(TRUE);
		}
		break;
	}
}





