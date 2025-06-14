// AbilityUse_Job.cpp: implementation of the CAbilityUse_Job class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "AbilityUse_Job.h"

#include "UngiJosikManager.h"
#ifdef _MHCLIENT_
#include "GameIn.h"
#include "MainBarDialog.h"
#include "UpgradeDlg.h"
#include "MixDialog.h"
#include "ReinforceDlg.h"
#include "DissolutionDialog.h"

#include "AbilityManager.h"
#include "ObjectManager.h"
#include "ExchangeManager.h"
#include "ChatManager.h"
#include "VimuManager.h"
#include "StreetStallManager.h"
#include "StallFindDlg.h"

#include "Battle.h"
#include "BattleSystem_Client.h"
#include "./input/UserInput.h"
#include "WindowIDEnum.h"
#include "cWindowManager.h"
#include "cMsgBox.h"
#include "StreetStall.h"
#include "StreetBuyStall.h"
#include "AutoNoteDlg.h"
#include "PartyMatchingDlg.h"
#endif


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CAbilityUse_Job::CAbilityUse_Job()
{

}

CAbilityUse_Job::~CAbilityUse_Job()
{

}


void CAbilityUse_Job::Use(BYTE Level,CAbilityInfo* pAbilityInfo)
{
#ifdef _MHCLIENT_
	ySWITCH(pAbilityInfo->GetInfo()->Ability_effect_Param1)
		yCASE(eAUKJOB_Ungijosik) // ¿î±âÁ¶½Ä
			UNGIMGR->ToggleHero_UngiMode();
		yCASE(eAUKJOB_Vimu) 	 // ºñ¹«½ÅÃ»
			if(GAMEIN->GetGameInInitKind() == eGameInInitKind_SuryunEnter || 
				GAMEIN->GetGameInInitKind() == eGameInInitKind_EventMapEnter)
				return;
			if(ABILITYMGR->CheckAbilityLevel(ABILITYINDEX_VIMU,HERO->GetAbilityGroup(),1) == FALSE)
			{
				CHATMGR->AddMsg( CTC_ABILITYUPGRADE_FAILED, CHATMGR->GetChatMsg( 139 ));
			}
			else
			{
				VIMUMGR->ApplyVimu();
			}
		yCASE(eAUKJOB_Party) 	 // ¹æÆÄÃ¢¼³
			//¹æÆÄ°¡ ¾øÀ»½Ã¿¡¸¸ÀÎ°¡? ¾Æ´Ï¸é ´ÙÀÌ¾ó·Î±× ²ô°í Å°´Â°Íµµ µÇ´Â°Ç°¡? 
			// -- ¹æÆÄ°¡ ¾øÀ» ¶§¸¸ÀÔ´Ï´Ù by hs
			if(GAMEIN->GetGameInInitKind() == eGameInInitKind_SuryunEnter || 
				GAMEIN->GetGameInInitKind() == eGameInInitKind_EventMapEnter)
				return;
			CPartyDialog* pDlg = GAMEIN->GetPartyDialog();

			if( !pDlg ) return;
			pDlg->SetActive( TRUE );

			
		yCASE(eAUKJOB_Guild) 	 // ¹®ÆÄÃ¢¼³		//passive skillÀÎµí.
			if(GAMEIN->GetGameInInitKind() == eGameInInitKind_SuryunEnter || 
				GAMEIN->GetGameInInitKind() == eGameInInitKind_EventMapEnter)
				return;
		yCASE(eAUKJOB_StreetStall)// ³ëÁ¡»ó°³¼³
			// 현재 노점 검색중이면 노점�?열기 불가 by Stiner(8)
			if( GAMEIN->GetStallFindDlg()->IsActive() == TRUE )
			{
				WINDOWMGR->MsgBox( MBI_OK, MBT_OK, CHATMGR->GetChatMsg(1697) );
				return;
			}
			
			if(GAMEIN->GetGameInInitKind() == eGameInInitKind_SuryunEnter || 
				GAMEIN->GetGameInInitKind() == eGameInInitKind_EventMapEnter)
				return;
			
			////////////////////////////////////////////////////////////////////////////////
			// 06. 06. 2�?전직 - 이영준
			// 은�?/ 혜안
			// 은�?패널�?

			if( HERO->GetSingleSpecialState(eSingleSpecialState_Hide) )
			{
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1329) );
				return;
			}
			////////////////////////////////////////////////////////////////////////////////
			if(ABILITYMGR->CheckAbilityLevel(ABILITYINDEX_STALL,HERO->GetAbilityGroup(),1) == FALSE)
			{
				CHATMGR->AddMsg( CTC_ABILITYUPGRADE_FAILED, CHATMGR->GetChatMsg( 139 ));
			}
			else
			{


//				CHATMGR->AddMsg( CTC_SYSMSG, "³ëÁ¡»ó ±â´ÉÀÌ ÀÏ½ÃÀûÀ¸·Î Áß´ÜµÇ¾ú½À´Ï´Ù. ÀÌ¿ë¿¡ ºÒÆíÀ» µå·Á ÁË¼ÛÇÕ´Ï´Ù." );


				if( ( HERO->GetState() == eObjectState_None || HERO->GetState() == eObjectState_Immortal )
					&& !VIMUMGR->IsVimuing() )
				{
					STREETSTALLMGR->ChangeDialogState();
				}

			}
 
		yCASE(eAUKJOB_Deal) 	 // ±³È¯	
			if(GAMEIN->GetGameInInitKind() == eGameInInitKind_SuryunEnter || 
				GAMEIN->GetGameInInitKind() == eGameInInitKind_EventMapEnter)
				return;
			if(ABILITYMGR->CheckAbilityLevel(ABILITYINDEX_EXCHANGE,HERO->GetAbilityGroup(),1) == FALSE)
			{
				CHATMGR->AddMsg( CTC_ABILITYUPGRADE_FAILED, CHATMGR->GetChatMsg( 139 ));
			}
			else
			{
				EXCHANGEMGR->ApplyExchange();
			}

		yCASE(eAUKJOB_Upgrade) 	 // µî±Þ¾÷	
			if(GAMEIN->GetGameInInitKind() == eGameInInitKind_SuryunEnter || 
				GAMEIN->GetGameInInitKind() == eGameInInitKind_EventMapEnter)
				return;
			if( HERO->GetState() != eObjectState_Die )
			{
				if( GAMEIN->GetUpgradeDialog()->IsActive() && 
					!GAMEIN->GetUpgradeDialog()->IsDisable() )
				{
					GAMEIN->GetUpgradeDialog()->Release();
					GAMEIN->GetUpgradeDialog()->SetActiveRecursive( FALSE );					
				}
				else
				{
					GAMEIN->GetUpgradeDialog()->SetActiveRecursive(TRUE);
				}
			}
		yCASE(eAUKJOB_Mix) 	 // ��???	
			if(GAMEIN->GetGameInInitKind() == eGameInInitKind_SuryunEnter || 
				GAMEIN->GetGameInInitKind() == eGameInInitKind_EventMapEnter)
				return;
			if( HERO->GetState() != eObjectState_Die )
			{
				if( GAMEIN->GetMixDialog()->IsActive() &&
					!GAMEIN->GetMixDialog()->IsDisable() )
				{
					GAMEIN->GetMixDialog()->Release();
					GAMEIN->GetMixDialog()->SetActiveRecursive( FALSE );
				}
				else
				{
					GAMEIN->GetMixDialog()->SetActiveRecursive(TRUE);
				}
			}
		yCASE(eAUKJOB_Reinforce) 	 // °­È­	

			if(GAMEIN->GetGameInInitKind() == eGameInInitKind_SuryunEnter || 
				GAMEIN->GetGameInInitKind() == eGameInInitKind_EventMapEnter)
				return;
			if( HERO->GetState() != eObjectState_Die )
			{
				if( GAMEIN->GetReinforceDialog()->IsActive() &&
					!GAMEIN->GetReinforceDialog()->IsDisable() )
				{
					GAMEIN->GetReinforceDialog()->Release();
					GAMEIN->GetReinforceDialog()->SetActiveRecursive( FALSE );
				}
				else
				{
					GAMEIN->GetReinforceDialog()->SetActiveRecursive(TRUE);
				}				
			}
		yCASE(eAUKJOB_Dissolve) 	 // ºÐÇØ
			if(GAMEIN->GetGameInInitKind() == eGameInInitKind_SuryunEnter || 
				GAMEIN->GetGameInInitKind() == eGameInInitKind_EventMapEnter)
				return;
			
			if( HERO->GetState() != eObjectState_Die )
			{
				if( GAMEIN->GetDissolutionDialog()->IsActive() &&
					!GAMEIN->GetDissolutionDialog()->IsDisable() )
				{
					GAMEIN->GetDissolutionDialog()->SetActive( FALSE );
				}
				else
				{
					GAMEIN->GetDissolutionDialog()->SetActive(TRUE);
				}				
			}
		yCASE(eAUKJOB_Restraint)
			if( BATTLESYSTEM->GetBattle(HERO)->GetBattleKind() != eBATTLE_KIND_SIEGEWAR )
				return;

			MSGBASE msg;
			SetProtocol( &msg, MP_SIEGEWAR, MP_SIEGEWAR_RESTRAINT_SYN );
			msg.dwObjectID = HEROID;
			NETWORK->Send( &msg, sizeof(msg) );
		yCASE(eAUKJOB_Attack)
			g_UserInput.GetHeroMove()->AttackSelectedObject();

		yCASE(eAUKJOB_StallFind)
			// 노점상이 열려있을때는 검�?불가 by Stiner(8)
			if( GAMEIN->GetStreetStallDialog()->IsActive() == TRUE 
				|| GAMEIN->GetStreetBuyStallDialog()->IsActive() == TRUE
				|| GAMEIN->GetStallKindSelectDialog()->IsActive() == TRUE )
			{
				WINDOWMGR->MsgBox( MBI_OK, MBT_OK, CHATMGR->GetChatMsg(1696) );
				return;
			}

			// 노점�?검색창 띄우�?by Stiner(8)
			GAMEIN->GetStallFindDlg()->SetActive(TRUE);
		yCASE(eAUKJOB_PartyFind)	//2008. 6. 4. CBH - 방파 찾기 추가)
			if( HERO->GetState() != eObjectState_Die )

			{
				GAMEIN->GetPartyMatchingDlg()->SetActive(TRUE);
			}
		yCASE(eAUKJOB_AutoNote)	// AutoNote
			GAMEIN->GetAutoNoteDlg()->SetActive(TRUE);

		yENDSWITCH
#endif
}
