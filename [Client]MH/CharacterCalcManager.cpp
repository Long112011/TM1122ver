#include "stdafx.h"
#include "CharacterCalcManager.h"
#include "GameIn.h"
#include "ObjectManager.h"
#include "ChatManager.h"
#include "GameResourceManager.h"
#include "./Audio/MHAudioManager.h"
#include "AbilityManager.h"
#include "CharacterDialog.h"
#include "GameEventManager.h"
#include "ObjectStateManager.h"
#include "ObjectActionManager.h"
#include "ExitManager.h"
#include "GameEventManager.h"
#include "QuestManager.h"
#include "SuryunDialog.h"
#include "JournalManager.h"
#include "MainBarDialog.h"
#include "SkillPointRedist.h"
#include "AppearanceManager.h"
#include "ItemManager.h"
#include "MussangManager.h"
#include "SkillPointResetDlg.h"
#include "MHMap.h"
#include "cMsgBox.h"
#include "MiniMapPing.h"     
#include "TopDialog.h"       
#include "TopMunpDialog.h"   
#include "FBTimeDialog.h"    
#include "..\WindowIDEnum.h" 
#include "cWindowManager.h"  
#include "FameGoldDialog.h"  
#include "EventPlayTimeDialog.h"
#include "FameManager.h"
#include "AIManager.h"
#include "RankButtonDialog.h"

#include "TopDungeon.h"
extern bool b_WaitFirstEvent;
GLOBALTON(CCharacterCalcManager)
CCharacterCalcManager::CCharacterCalcManager()
{
}
CCharacterCalcManager::~CCharacterCalcManager()
{
}
void CCharacterCalcManager::NetworkMsgParse(BYTE Protocol,void* pMsg)
{
	switch(Protocol) 
	{
	case MP_CHAR_LIFE_ACK:
		{
			MSG_INT* msg = (MSG_INT*)pMsg;
			if(HERO)
			{
				HERO->ChangeLife(msg->nData);
			}
		}
		break;
	case MP_CHAR_LIFE_NACK:
		{
			ASSERT(0);
		}
		break;
	case MP_CHAR_SHIELD_ACK:
		{
			MSG_INT* msg = (MSG_INT*)pMsg;
			if(HERO)
			{
				HERO->ChangeShield(msg->nData);
			}
		}
		break;
	case MP_CHAR_SHIELD_NACK:
		{
			ASSERT(0);
		}
		break;
	case MP_CHAR_NAERYUK_ACK:
		{
			MSG_DWORD * msg = (MSG_DWORD *)pMsg;
			if(HERO)
			{
				HERO->SetNaeRyuk(msg->dwData);
			}		
		}
		break;
	case MP_CHAR_NAERYUK_NACK:
		{
			ASSERT(0);
		}
		break;
	case MP_CHAR_EXPPOINT_ACK:
		{
			MSG_EXPPOINT * msg = (MSG_EXPPOINT *)pMsg;	
			if(!HERO) return;
			if(msg->ExpKind == MSG_EXPPOINT::EXPPOINTKIND_ACQUIRE)
			{
				EXPTYPE diff = msg->ExpPoint - HERO->GetExpPoint();
				if(diff<0)
				{
					diff = GAMERESRCMNGR->GetMaxExpPoint(HERO->GetLevel()) - HERO->GetExpPoint() + msg->ExpPoint;
					HERO->SetExpPoint(0);
				}
				if(HERO->GetLevel() == GAMERESRCMNGR->GetMaxLevelControl())   
				{
					msg->ExpPoint = 0;
					diff = 0;
					return;
				}
				HERO->SetExpPoint(msg->ExpPoint);
				CHATMGR->AddMsg( CTC_GETEXP, CHATMGR->GetChatMsg( 132 ), diff );
			}
			else if(msg->ExpKind == MSG_EXPPOINT::EXPPOINTKIND_DIE)
			{
				EXPTYPE diff = HERO->GetExpPoint() - msg->ExpPoint;
				if(diff == 0)
					return;
				if(HERO->GetLevel() == GAMERESRCMNGR->GetMaxLevelControl())
				{
					return;
				}
				if(diff<0)	
				{
					diff = GAMERESRCMNGR->GetMaxExpPoint(HERO->GetLevel()) - msg->ExpPoint + HERO->GetExpPoint();
				}
				HERO->SetExpPoint(msg->ExpPoint);
				CHATMGR->AddMsg( CTC_GETEXP, CHATMGR->GetChatMsg( 134 ), diff );
			}
		}
		break;
	case MP_CHAR_EXPPOINT_NACK:
		{
			ASSERT(0);
		}
		break;		
	case MP_CHAR_MAXLIFE_NOTIFY:
		{
			MSG_DWORD * msg = (MSG_DWORD *)pMsg;
			if(HERO)
			{
				HERO->SetMaxLife(msg->dwData);
			}
		}
		break;
	case MP_CHAR_MAXSHIELD_NOTIFY:
		{
			MSG_DWORD * msg = (MSG_DWORD *)pMsg;
			if(HERO)
			{
				HERO->SetMaxShield(msg->dwData);
			}
		}
		break;
	case MP_CHAR_MAXNAERYUK_NOTIFY:
		{
			MSG_DWORD * msg = (MSG_DWORD *)pMsg;
			if(HERO)
			{
				HERO->SetMaxNaeRyuk(msg->dwData);
			}
		}
		break;
	case MP_CHAR_PLAYERLEVELUP_NOTIFY:
		{
			MSG_DWORD * msg = (MSG_DWORD *)pMsg;
			CPlayer * pPlayer = (CPlayer *)OBJECTMGR->GetObject(msg->dwData);
			if(!pPlayer) return;
			if(pPlayer->InTitan() == TRUE)
				EFFECTMGR->StartEffectProcess(eEffect_TitanLevelUpSentence,pPlayer,NULL,0,0);
			else
				EFFECTMGR->StartEffectProcess(eEffect_LevelUpSentence,pPlayer,NULL,0,0);
		}
		break;
	case MP_CHAR_LEVEL_NOTIFY:
		{
			MSG_LEVEL * msg = (MSG_LEVEL*)pMsg;
			if(HERO)
			{
				EXPTYPE diff = GAMERESRCMNGR->GetMaxExpPoint(HERO->GetLevel()) - HERO->GetExpPoint();
				diff += msg->CurExpPoint;				
				if(msg->Level > HERO->GetLevel())
					CHATMGR->AddMsg( CTC_GETEXP, CHATMGR->GetChatMsg( 132 ), diff );
				HERO->SetLevel(msg->Level);
				HERO->SetMaxExpPoint(msg->MaxExpPoint);
				HERO->SetExpPoint(msg->CurExpPoint);
				if( msg->Level > HERO->GetMaxLevel() )
				{
					HERO->SetMaxLevel( msg->Level );
				}
				ABILITYMGR->UpdateAbilityState(ABILITYUPDATE_CHARACTERLEVEL_CHANGED,HERO->GetLevel(),HERO->GetAbilityGroup());
				GAMEEVENTMGR->AddEvent( eGameEvent_LevelUp, HERO->GetLevel() );
				QUESTMGR->ProcessNpcMark();
			}
		}
		break;
	case MP_CHAR_LEVELUPPOINT_NOTIFY:
		{
			MSG_WORD * msg = (MSG_WORD *)pMsg;
			if(HERO)
			{
				if(msg->wData != 0)
				{				
					GAMEIN->GetCharacterDialog()->SetPointLeveling(TRUE, msg->wData);		
					if( !GAMEIN->GetCharacterDialog()->IsActive() )
						GAMEIN->GetMainInterfaceDialog()->SetAlram( OPT_CHARACTERDLGICON, TRUE );
				}
				else
 				{
 					GAMEIN->GetCharacterDialog()->SetPointLevelingHide();
 				}
			}
		}
		break;
	case MP_CHAR_GENGOL_NOTIFY:
		{
			MSG_DWORD * msg = (MSG_DWORD *)pMsg;
			if(HERO)
			{
				HERO->SetGenGol((WORD)msg->dwData);
			}
		}
		break;
	case MP_CHAR_MINCHUB_NOTIFY:
		{
			MSG_DWORD * msg = (MSG_DWORD *)pMsg;
			if(HERO)
			{
				HERO->SetMinChub((WORD)msg->dwData);
			}
		}
		break;
	case MP_CHAR_CHERYUK_NOTIFY:
		{
			MSG_DWORD * msg = (MSG_DWORD *)pMsg;
			if(HERO)
			{
				HERO->SetCheRyuk((WORD)msg->dwData);
			}
		}
		break;
	case MP_CHAR_SIMMEK_NOTIFY:
		{
			MSG_DWORD * msg = (MSG_DWORD *)pMsg;
			if(HERO)
			{
				HERO->SetSimMek((WORD)msg->dwData);
			}
		}
		break;
	case MP_CHAR_FAME_NOTIFY:
		{
			MSG_FAME * pmsg = (MSG_FAME*)pMsg;
			CHAR balloon[MAX_MASTERNAME_LENGTH + 1];
			if( pmsg->dwObjectID == HEROID )
			{
				HERO->SetFame(pmsg->Fame);

				switch(HERO->GetStage())
				{
				case 0			:	sprintf(balloon, "");	break;
				case eStage_Hwa	:
				case eStage_Hyun:	sprintf(balloon, "@%d", FAMEMGR->GetFameLogoChange(HERO->GetFame()));	break;
				case eStage_Geuk:
				case eStage_Tal	:	sprintf(balloon, "&%d", FAMEMGR->GetFameLogoChange(HERO->GetFame()));	break;
				}
				
				HERO->SetStageLogoBalloon(balloon);

				//std::string s = std::to_string(pmsg->Fame);
				//char* c = strcpy(new char[s.length() + 1], s.c_str());
				//HERO->SetKillCountBalloon(c);
			}
			else
			{
				CPlayer* pPlayer = (CPlayer*)OBJECTMGR->GetObject( pmsg->dwObjectID );
				if( !pPlayer )	return;

				pPlayer->SetFame(pmsg->Fame);				

				switch(pPlayer->GetStage())
				{
				case 0			:	sprintf(balloon, "");	break;
				case eStage_Hwa	:
				case eStage_Hyun:	sprintf(balloon, "@%d", FAMEMGR->GetFameLogoChange(pPlayer->GetFame()));	break;
				case eStage_Geuk:
				case eStage_Tal	:	sprintf(balloon, "&%d", FAMEMGR->GetFameLogoChange(pPlayer->GetFame()));	break;
				}

				pPlayer->SetStageLogoBalloon(balloon);


				//std::string s = std::to_string(pmsg->Fame);
				//char* c = strcpy(new char[s.length() + 1], s.c_str());
				//pPlayer->SetKillCountBalloon(c);
			}
		}
		break;
	case MP_CHAR_KILLCOUNT_NOTIFY:
	{
		MSG_KILLCOUNT * pmsg = (MSG_KILLCOUNT*)pMsg;
		//CHAR balloon[MAX_MASTERNAME_LENGTH + 1];
		if (pmsg->dwObjectID == HEROID)
		{
			HERO->SetKillCount(pmsg->KillCount);

			std::string s = std::to_string(pmsg->KillCount);
			char* c = strcpy(new char[s.length() + 1], s.c_str());
			HERO->SetKillCountBalloon(c);
		}
		else
		{
			CPlayer* pPlayer = (CPlayer*)OBJECTMGR->GetObject(pmsg->dwObjectID);
			if (!pPlayer)	return;

			pPlayer->SetKillCount(pmsg->KillCount);

			std::string s = std::to_string(pmsg->KillCount);
			char* c = strcpy(new char[s.length() + 1], s.c_str());
			pPlayer->SetKillCountBalloon(c);
		}
	}
	break;
	case MP_CHAR_BADFAME_NOTIFY:
		{
			MSG_FAME * pmsg = (MSG_FAME*)pMsg;
			CPlayer* pPlayer = (CPlayer*)OBJECTMGR->GetObject( pmsg->dwObjectID );
			if(pPlayer)
			{
				pPlayer->SetBadFame(pmsg->Fame);
			}
		}
		break;
	case MP_CHAR_BADFAME_ACK:
		{
			MSG_FAME* pmsg = (MSG_FAME*)pMsg;
			char badfame[16] = { 0, };
			char money[16] = { 0, };
			strcpy(badfame, AddComma(pmsg->Fame));
			strcpy(money, AddComma(pmsg->Fame*BAIL_PRICE));
			CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(646), money, badfame);
		}
		break;
	case MP_CHAR_BADFAME_NACK:
		{
			MSGBASE* pmsg = (MSGBASE*)pMsg;
			CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(647));
		}
		break;
	case MP_CHAR_BADFAME_CHANGED:
		{
			MSG_FAME* pmsg = (MSG_FAME*)pMsg;
			if(HERO)
			{
				HERO->SetFame(pmsg->Fame);
			}
		}
		break;
	case MP_CHAR_PLAYTIME_ACK:
		{
			MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;
			if(HERO)
			{
			}
		}
		break;
	case MP_CHAR_PLAYTIME_NACK:
		{
			MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;
			if(HERO)
			{
				HERO->SetPlayTime(pmsg->dwData);
			}
		}
		break;
	case MP_CHAR_ABILITYEXPPOINT_ACK:
		{
			MSG_ABILLITY_EXPPOINT * msg = (MSG_ABILLITY_EXPPOINT *)pMsg;	
			if(!HERO) return;
			if(msg->ExpKind == MSG_ABILLITY_EXPPOINT::EXPPOINTKIND_ACQUIRE)
			{
				int diff = msg->ExpPoint - HERO->GetAbilityExp();
				HERO->SetAbilityExp(msg->ExpPoint);
				CHATMGR->AddMsg( CTC_GETABILITYEXP, CHATMGR->GetChatMsg( 135 ), diff );
				ABILITYMGR->UpdateAbilityState(ABILITYUPDATE_ABILITYEXP_CHANGED,0,HERO->GetAbilityGroup());
			}
			else if(msg->ExpKind == MSG_ABILLITY_EXPPOINT::EXPPOINTKIND_ABILITYUSE)
			{
				ASSERT(0);
			}
		}
		break;
	case MP_CHAR_ABILITY_UPGRADE_ACK:
		{
			GAMEIN->GetSuryunDialog()->SetDisable(FALSE);
			GAMEIN->GetSkPointDlg()->SetDisable(FALSE);
			MSG_WORD* pmsg = (MSG_WORD*)pMsg;
			CAbilityInfo* pInfo = ABILITYMGR->GetAbilityInfo(pmsg->wData);
			GAMEIN->GetSuryunDialog()->SetSelectAbilityIDX( pInfo->GetIdx() );
			ABILITYMGR->UpgradeAbility(pmsg->wData,HERO->GetAbilityGroup());			
			CHATMGR->AddMsg( CTC_ABILITYUPGRADE_SUCCESS, CHATMGR->GetChatMsg( 137 ), pInfo->GetInfo()->Ability_name );
			GAMEEVENTMGR->AddEvent( eGameEvent_Ability, pInfo->GetInfo()->Ability_index );
			GAMEIN->GetSkPointDlg()->RefreshAbilityPoint();
			GAMEIN->GetSkPointDlg()->RefreshAbilityIcons();
			GAMEIN->GetSkillPointResetDlg()->RefreshAbilityIcons();
		}
		break;
	case MP_CHAR_ABILITY_UPGRADE_NACK:
		{
			GAMEIN->GetSuryunDialog()->SetDisable(FALSE);
			GAMEIN->GetSkPointDlg()->SetDisable(FALSE);
			MSG_WORD* pmsg = (MSG_WORD*)pMsg;
			CAbilityInfo* pInfo = ABILITYMGR->GetAbilityInfo(pmsg->wData);
			CHATMGR->AddMsg( CTC_ABILITYUPGRADE_FAILED, CHATMGR->GetChatMsg( 138 ), pInfo->GetInfo()->Ability_name );
		}
		break;
	case MP_CHAR_ABILITY_UPGRADE_SKPOINT_ACK:
		{
			GAMEIN->GetSuryunDialog()->SetDisable(FALSE);
			GAMEIN->GetSkPointDlg()->SetDisable(FALSE);
			MSG_WORD* pmsg = (MSG_WORD*)pMsg;
			CAbilityInfo* pInfo = ABILITYMGR->GetAbilityInfo(pmsg->wData);
			GAMEIN->GetSuryunDialog()->SetSelectAbilityIDX( pInfo->GetIdx() );
			ABILITYMGR->UpgradeAbilitySkPoint(pmsg->wData,HERO->GetAbilityGroup());			
			CHATMGR->AddMsg( CTC_ABILITYUPGRADE_SUCCESS, CHATMGR->GetChatMsg( 137 ), pInfo->GetInfo()->Ability_name );
			GAMEEVENTMGR->AddEvent( eGameEvent_Ability, pInfo->GetInfo()->Ability_index );
			GAMEIN->GetSkPointDlg()->RefreshAbilityPoint();
			GAMEIN->GetSkPointDlg()->RefreshAbilityIcons();
		}
		break;
	case MP_CHAR_ABILITY_UPGRADE_SKPOINT_NACK:
		{
			GAMEIN->GetSuryunDialog()->SetDisable(FALSE);
			GAMEIN->GetSkPointDlg()->SetDisable(FALSE);
			MSG_WORD* pmsg = (MSG_WORD*)pMsg;
			CAbilityInfo* pInfo = ABILITYMGR->GetAbilityInfo(pmsg->wData);
			CHATMGR->AddMsg( CTC_ABILITYUPGRADE_FAILED, CHATMGR->GetChatMsg( 138 ), pInfo->GetInfo()->Ability_name );
		}
		break;
	case MP_CHAR_ABILITY_DOWNGRADE_SKPOINT_ACK:
		{
			GAMEIN->GetSuryunDialog()->SetDisable(FALSE);
			GAMEIN->GetSkPointDlg()->SetDisable(FALSE);
			MSG_WORD* pmsg = (MSG_WORD*)pMsg;
			CAbilityInfo* pInfo = ABILITYMGR->GetAbilityInfo(pmsg->wData);
			GAMEIN->GetSuryunDialog()->SetSelectAbilityIDX( pInfo->GetIdx() );
			ABILITYMGR->DowngradeAbilitySkPoint(pmsg->wData,HERO->GetAbilityGroup());
			CHATMGR->AddMsg( CTC_ABILITYUPGRADE_SUCCESS, CHATMGR->GetChatMsg( 740 ), pInfo->GetInfo()->Ability_name );
			GAMEEVENTMGR->AddEvent( eGameEvent_Ability, pInfo->GetInfo()->Ability_index );
			GAMEIN->GetSkPointDlg()->RefreshAbilityPoint();
			GAMEIN->GetSkPointDlg()->RefreshAbilityIcons();

			if(pmsg->wData == ABILITYINDEX_TACTIC_MUSSANG)
				MUSSANGMGR->SetMussangPoint(0);

		}
		break;
	case MP_CHAR_ABILITY_DOWNGRADE_SKPOINT_NACK:
		{
			GAMEIN->GetSuryunDialog()->SetDisable(FALSE);
			GAMEIN->GetSkPointDlg()->SetDisable(FALSE);
			MSG_WORD* pmsg = (MSG_WORD*)pMsg;
			CAbilityInfo* pInfo = ABILITYMGR->GetAbilityInfo(pmsg->wData);
			CHATMGR->AddMsg( CTC_ABILITYUPGRADE_FAILED, CHATMGR->GetChatMsg( 741 ), pInfo->GetInfo()->Ability_name );
		}
		break;
	case MP_CHAR_ABILITY_RESET_SKPOINT_ACK:
		{
			MSG_WORD* pmsg = (MSG_WORD*)pMsg;
			CAbilityInfo* pInfo = ABILITYMGR->GetAbilityInfo(pmsg->wData);
			ABILITYMGR->ResetAbilitySkPoint(pmsg->wData,HERO->GetAbilityGroup());
			GAMEIN->GetSkillPointResetDlg()->SetActive(FALSE);

			if(pmsg->wData == ABILITYINDEX_TACTIC_MUSSANG)
				MUSSANGMGR->SetMussangPoint(0);
		}
		break;
	case MP_CHAR_EXITSTART_ACK:
		{
			MSGBASE* pmsg = (MSGBASE*)pMsg;
			EXITMGR->StartExit();			
		}
		break;
	case MP_CHAR_EXITSTART_NACK:
		{
			MSG_BYTE* pmsg = (MSG_BYTE*)pMsg;			
			EXITMGR->RejectExit( pmsg->bData );
		}
		break;
	case MP_CHAR_EXIT_ACK:
		{
			EXITMGR->Exit();
		}
		break;
	case MP_CHAR_EXIT_NACK:
		{
			MSG_BYTE* pmsg = (MSG_BYTE*)pMsg;			
			EXITMGR->CancelExit( pmsg->bData );
		}
		break;
	case MP_CHAR_YOUAREDIED:
		{
			if( HERO->GetState() != eObjectState_Die )
			{
				ASSERT(HERO->IsDied());
				if(gCurTime - HERO->GetDiedTime() < 10000)
					return;
				OBJECTACTIONMGR->Die(HERO,HERO,FALSE,FALSE,FALSE);
				GAMEIN->GetSkPointDlg()->SetActive( FALSE );
			}
		}
		break;
	case MP_CHAR_POINTMINUS_ACK:
		{
			MSG_WORD* pmsg = (MSG_WORD*)pMsg;
			--HERO->GetShopItemStats()->StatePoint;
			++HERO->GetShopItemStats()->UseStatePoint;
			GAMEIN->GetCharacterDialog()->RefreshPointInfo();
		}
		break;
	case MP_CHAR_POINTMINUS_NACK:
		{
			MSG_WORD* pmsg = (MSG_WORD*)pMsg;
			CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(756) );
		}
		break;
	case MP_CHAR_STAGE_NOTIFY:
		{
			MSG_BYTE* pmsg = (MSG_BYTE*)pMsg;
			if( pmsg->dwObjectID == HEROID )
			{
				HERO->SetStage( pmsg->bData );	
				char temp[64];
				switch( pmsg->bData )
				{
				case eStage_Hwa:
					{
						sprintf( temp, CHATMGR->GetChatMsg(890) );
						EFFECTMGR->StartEffectProcess( eEffect_ChangeStage_Hwa, HERO, NULL, 0, 0 );
					}
					break;
				case eStage_Hyun:
					{
						sprintf( temp, CHATMGR->GetChatMsg(891) );
						EFFECTMGR->StartEffectProcess( eEffect_ChangeStage_Hwa, HERO, NULL, 0, 0 );
					}
					break;
				case eStage_Geuk:
					{
						sprintf( temp, CHATMGR->GetChatMsg(892) );
						EFFECTMGR->StartEffectProcess( eEffect_ChangeStage_Geuk, HERO, NULL, 0, 0 );
					}
					break;
				case eStage_Tal:
					{
						sprintf( temp, CHATMGR->GetChatMsg(893) );
						EFFECTMGR->StartEffectProcess( eEffect_ChangeStage_Geuk, HERO, NULL, 0, 0 );
					}
					break;
				}
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(900), temp );
				APPEARANCEMGR->AddCharacterPartChange( HEROID );
			}
			else
			{
				CPlayer* pPlayer = (CPlayer*)OBJECTMGR->GetObject( pmsg->dwObjectID );
				if( !pPlayer )	return;
				pPlayer->SetStage( pmsg->bData );
				switch( pmsg->bData )
				{
				case eStage_Hwa:
				case eStage_Hyun:
					{
						EFFECTMGR->StartEffectProcess( eEffect_ChangeStage_Hwa, pPlayer, NULL, 0, 0 );
					}
					break;
				case eStage_Geuk:
				case eStage_Tal:
					{
						EFFECTMGR->StartEffectProcess( eEffect_ChangeStage_Geuk, pPlayer, NULL, 0, 0 );
					}
					break;
				}
				APPEARANCEMGR->AddCharacterPartChange( pmsg->dwObjectID );
			}						
		}
		break;
	case MP_CHAR_MUSSANG_INFO:
		{
			MSG_DWORDBYTE* pmsg = (MSG_DWORDBYTE*)pMsg;
			if( !HERO ) return;
			if( pmsg->bData & eMSS_LevelUp )
			{
				MUSSANGMGR->SetMussangMaxPoint(pmsg->dwData);
				break;
			}
			MUSSANGMGR->SetMussangPoint(pmsg->dwData);
			if( pmsg->bData & eMSS_Ready )
			{
				if( !MUSSANGMGR->IsMussangReady() )
				{
					MUSSANGMGR->SetMussangReady(TRUE);	
					MUSSANGMGR->SetValidMsg(TRUE);
				}
			}
		}
		break;
	case MP_CHAR_MUSSANG_ACK:
		{
			MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;
			CPlayer* pPlayer = (CPlayer*)OBJECTMGR->GetObject(pmsg->dwObjectID);
			if(!pPlayer) return;
			if( HERO == pPlayer )
			{
				MUSSANGMGR->SetMussangReady(FALSE);
				MUSSANGMGR->SetMussangMode(TRUE, pmsg->dwData);	
			}
			if((pPlayer->GetCharacterTotalInfo()->bVisible == FALSE)
				||(!pPlayer->GetSingleSpecialState(eSingleSpecialState_Hide)))
			{
				switch(pPlayer->GetStage())
				{
				case eStage_Normal:	
					{
						OBJECTEFFECTDESC desc(1035);
						pPlayer->AddObjectEffect( MUSSANG_EFFECT_ID1, &desc, 1, pPlayer);
					}
					break;
				case eStage_Hwa:		
				case eStage_Hyun:
					{
						OBJECTEFFECTDESC desc(888);
						pPlayer->AddObjectEffect( MUSSANG_EFFECT_ID2, &desc, 1, pPlayer);
					}
					break;
				case eStage_Geuk:
				case eStage_Tal:
					{
						OBJECTEFFECTDESC desc(889);
						pPlayer->AddObjectEffect( MUSSANG_EFFECT_ID3, &desc, 1, pPlayer);
					}
					break;
				}
			}
		}
		break;
	case MP_CHAR_MUSSANG_NACK:
		{
			MUSSANGMGR->SetValidMsg(TRUE);
		}
		break;
	case MP_CHAR_MUSSANG_END:
		{
			MSGBASE* pmsg = (MSGBASE*)pMsg;
			CPlayer* pPlayer = (CPlayer*)OBJECTMGR->GetObject(pmsg->dwObjectID);
			if(!pPlayer) return;

			switch(pPlayer->GetStage())
			{
			case eStage_Normal:	
				{
					pPlayer->RemoveObjectEffect( MUSSANG_EFFECT_ID1 );
				}
				break;
			case eStage_Hwa:		
			case eStage_Hyun:
				{
					pPlayer->RemoveObjectEffect( MUSSANG_EFFECT_ID2 );
				}
				break;
			case eStage_Geuk:
			case eStage_Tal:
				{
					pPlayer->RemoveObjectEffect( MUSSANG_EFFECT_ID3 );
				}
				break;
			}
			if( HERO == pPlayer )
			{
				MUSSANGMGR->SetMussangMode(FALSE);	
			}
		}
		break;
	case MP_CHAR_SINGLE_SPECIAL_STATE_NOTIFY:
		{
			MSG_SINGLE_SPECIAL_STATE* pmsg = (MSG_SINGLE_SPECIAL_STATE*)pMsg;
			CPlayer* pPlayer = (CPlayer*)OBJECTMGR->GetObject(pmsg->Operator);
			OBJECTMGR->ApplySingleSpecialState(pPlayer, pmsg->State, pmsg->bVal);
		}
		break;
	case MP_CHAR_SINGLE_SPECIAL_STATE_ACK:
		{
			MSG_SINGLE_SPECIAL_STATE* pmsg = (MSG_SINGLE_SPECIAL_STATE*)pMsg;
			CPlayer* pPlayer = (CPlayer*)OBJECTMGR->GetObject(pmsg->Operator);
			switch(pmsg->State)
			{
			case eSingleSpecialState_Hide:
				{
					if(pmsg->bVal)
						CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(1327));
					else
					{
						if(pmsg->Forced)
							CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(1328));
						else
							CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(1331));
					}
				}
				break;
			case eSingleSpecialState_Detect:
				{
					if(pmsg->bVal)
						CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(1337));
					else
						CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(1334));
				}
				break;
			case eSingleSpecialState_DetectItem:
				{
					if(pmsg->bVal)
						CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(1333));
					else
						CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(1334));
				}
				break;
			}
			OBJECTMGR->ApplySingleSpecialState(pPlayer, pmsg->State, pmsg->bVal);
		}
		break;
	case MP_CHAR_SINGLE_SPECIAL_STATE_NACK:
		{
			MSG_SINGLE_SPECIAL_STATE_NACK* pmsg = (MSG_SINGLE_SPECIAL_STATE_NACK*)pMsg;
			switch(pmsg->ErrCode)
			{
			case eSingleSpecialState_Error_NaeRyuk:
				{
					CHATMGR->AddMsgFade(CTC_SYSMSG, CHATMGR->GetChatMsg(401));
				}
				break;
			case eSingleSpecialState_Error_Delay:
				{
					switch(pmsg->State)
					{
					case eSingleSpecialState_Hide:
						CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(1330));
						break;
					case eSingleSpecialState_Detect:
						CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(1332));
						break;
					}				
				}
				break;
			}
		}
		break;
	case MP_CHAR_FULLMOONEVENT_CHANGE:
		{
			MSG_BYTE* pmsg = (MSG_BYTE*)pMsg;
			CPlayer* pPlayer = (CPlayer*)OBJECTMGR->GetObject( pmsg->dwObjectID );
			if( !pPlayer )		return;
			pPlayer->SetFullMoonEventIndex( pmsg->bData );
			APPEARANCEMGR->InitAppearance( pPlayer );
			if( pPlayer->GetID() == HEROID )
			{
				if( pmsg->bData == eFULLMOONEVENT_BUN )
					CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(1453));
				else if( pmsg->bData == eFULLMOONEVENT_CHINESECAKE )
					CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(1458));
				else
					CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(1454));
			}
		}		
		break;
	case MP_CHAR_NOACTIONPANELTY_NOTIFY:
		{
            CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1606) );
			HERO->SetNoActionPenalty();
		}
		break;
	case MP_CHAR_POINTADD_ACK:
		{
			if( HERO->GetShopItemStats()->UseStatePoint )
				--HERO->GetShopItemStats()->UseStatePoint;
			GAMEIN->GetCharacterDialog()->RefreshPointInfo();
		}
		break;
	case MP_CHAR_POINTADD_NACK:
		{
		}
		break;
	case MP_CHAR_CHANGESTAGE_NACK:  
		{
			MSG_DWORD2* pmsg = (MSG_DWORD2*)pMsg;
			if(pmsg->dwObjectID==HEROID)
			 {
				 if(pmsg->dwData2==1)
				 {
					  CHATMGR->AddMsg( CTC_SYSMSG,CHATMGR->GetChatMsg(2076),pmsg->dwData1);
				      return;
				 }
		         if(pmsg->dwData2==2)
				 {
   			          CHATMGR->AddMsg( CTC_SYSMSG,CHATMGR->GetChatMsg(2077));
				      return;
				 }
				 if(pmsg->dwData2==3)
				 {
   			          CHATMGR->AddMsg( CTC_SYSMSG,CHATMGR->GetChatMsg(2137));   
				      return;
				 }
			 }
		}
		break;
	case MP_CHAR_CHANGELIFE_ACK:   
		{
			MSG_DWORD2* pmsg = (MSG_DWORD2*)pMsg;
			if(pmsg->dwObjectID==HEROID)
			{
				if(pmsg->dwData2==1)
				{
					EFFECTMGR->StartEffectProcess( eEffect_ChangeStage_Hwa, HERO, NULL, 0, 0 );
					HERO->SetReset(pmsg->dwData1);
					GAMEIN->GetCharacterDialog()->SetReSet();
					CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(2073), pmsg->dwData1);
					sCPlayerAI.DufualtUsekyungGong2();
				}
			}
		}
		break;
	case MP_CHAR_CHANGELIFE_NACK:
		{
			MSG_DWORD2* pmsg = (MSG_DWORD2*)pMsg;
			if(pmsg->dwObjectID == HEROID)
			{
				int chat=0;
				switch(pmsg->dwData2)
				{				  
				case eTurnOnOff:		chat=2069;	break;
				case eTurnMax:			chat=2070;	break;
				case eTurnMinResetLv:	chat=2071;	break;
				case eTurnMoneyError:	chat=2072;	break;
				case eTurnLevelError:	chat=2434;	break;
				case eTurnEQTakeOff:	chat=2438;	break;
				}
				CHATMGR->AddMsg( CTC_SYSMSG,CHATMGR->GetChatMsg(chat),pmsg->dwData1);
			}
		}
		break;
	case MP_CHAR_PING_ACK:  
		{
			MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;
			if(pmsg->dwObjectID == HEROID)
			{
				if(pmsg->dwData==1234)
				{
					MINIPINGMGR->SetPingShow(/*FALSE*/);
				}
			}
		}
		break;
	case MP_CHAR_HEROTOPLIST_ACK:  
		{
			CTopDialog * pDlg = GAMEIN->GetTopDialog();
			if(pDlg)
			{
			    SEND_TOPLIST* pmsg = (SEND_TOPLIST*)pMsg;
				pDlg->SetHeroTopListInfo(pmsg);
				pDlg->SetHeroPage(0);
			}

			
		}
		break;
	case MP_CHAR_MUNPTOPLIST_ACK:  
		{
			CTopMunpDialog * pDlg = GAMEIN->GetTopMunpDialog();
			if(pDlg)
			{
			    SEND_TOPLIST* pmsg = (SEND_TOPLIST*)pMsg;
				pDlg->SetMunpTopListInfo(pmsg);
				pDlg->SetMunpPage(0);
			}
		}
		break;
	case MP_CHAR_DUNGEONTOPLIST_ACK:
		{
			CTopDungeon * pDlg2 = GAMEIN->GetTopDungeon();
			if (pDlg2)
			{
				SEND_TOPLIST* pmsg = (SEND_TOPLIST*)pMsg;
				pDlg2->SetHeroTopListInfo(pmsg);
				pDlg2->SetHeroPage(0);
			}
		}
		break;
	case MP_CHAR_FB_TIME_ACK:    
		{
             MSG_DWORD  * pmsg =( MSG_DWORD*)pMsg;
			 if( MAP->IsMapKind(eInsDungeon))
			 {
				GAMEIN->GetFBTimeDialog()->SetFBTime(pmsg->dwData);
			 }
 		}
		break;
	case  MP_CHAR_FB_TIME_NACK:
		{
			MSG_DWORD2  * pmsg =( MSG_DWORD2*)pMsg;
		}
		break;
	case MP_CHAR_TIMER_ACK:
		{
			MSG_TIMER  * pmsg =(MSG_TIMER*)pMsg;
			RANKBTNDLG->SetFlicker(true , HRANK);
			RANKBTNDLG->SetFlicker(true , GRANK);
		}
		break;
	case MP_CHAR_BADFAME_GOLD_ACK:  
		{      
              MSG_DWORD  * pmsg =( MSG_DWORD*)pMsg;
			  if(HERO)
			  {
				  GAMEIN->GetFameGoldDialog()->SetFame();
				  CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(2187),pmsg->dwData);
				  return;
			  }
		}
		break;
	case MP_CHAR_BADFAME_GOLD_NACK:  
		{
              MSGBASE  * pmsg =( MSGBASE*)pMsg;
			  CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(2188));
			  return;
		}
		break;
	case  MP_CHAR_LOGIN_EVENT_ACK:
		{
			MSG_SIGNEVENT  * pmsg =( MSG_SIGNEVENT*)pMsg;
			if(pmsg->dwObjectID==HEROID)
			{
				switch(pmsg->wParse)
				{
				case 0:
					GAMEIN->GetEventPlayTimeDlg()->Close();
					GAMEIN->GetEventPlayTimeDlg()->SetEventTime(pmsg->Time,0,pmsg->ItemIdx,pmsg->cTextSend,pmsg->CyptKey,pmsg->dwTotalTime);
				break;
				case 1:
					GAMEIN->GetEventPlayTimeDlg()->Close();
					b_WaitFirstEvent=false;
				break;
				case 2:
					GAMEIN->GetEventPlayTimeDlg()->Close();
					GAMEIN->GetEventPlayTimeDlg()->SetEventTime(pmsg->Time,1,pmsg->ItemIdx,pmsg->cTextSend,pmsg->CyptKey,pmsg->dwTotalTime);
				break;
				}
			
				/*if(pmsg->wParse==0)
				{
					GAMEIN->GetEventPlayTimeDlg()->Close();
					GAMEIN->GetEventPlayTimeDlg()->SetEventTime(pmsg->Time,0,pmsg->ItemIdx,pmsg->cTextSend,pmsg->CyptKey,pmsg->dwTotalTime);
				}
				if(pmsg->wParse==2)
				{
					GAMEIN->GetEventPlayTimeDlg()->Close();
					GAMEIN->GetEventPlayTimeDlg()->SetEventTime(pmsg->Time,1,pmsg->ItemIdx,pmsg->cTextSend,pmsg->CyptKey,pmsg->dwTotalTime);
				}
				if(pmsg->wParse==1)
				{
					GAMEIN->GetEventPlayTimeDlg()->Close();
					b_WaitFirstEvent=false;
				}*/
			}
		}
		break;
	}
}