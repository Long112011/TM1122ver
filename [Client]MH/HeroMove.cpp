#include "stdafx.h"
#ifdef _CHEATENABLE_
#include "CheatMsgParser.h"
#endif	
#include "HeroMove.h"
#include "SkillManager_Client.h"
#include "MoveManager.h"
#include "ObjectManager.h"
#include "ChatDialog.h"
#include "InventoryExDialog.h"
#include "GameIn.h"
#include "MousePointer.h"
#include "Interface/cEditBox.h"
#include "GameResourceManager.h"
#include "ExchangeManager.h"
#include "chatmanager.h"
#include "NpcScriptManager.h"
#include "BattleSystem_Client.h"
#include "Input/UserInput.h"
#include "MouseCursor.h"
#include "Interface/cWindowManager.h"
#include "PartyManager.h"
#include "GuildManager.h"  
#include "GuildUnion.h"    
#include "ObjectStateManager.h"   
#include "PetBase.h"
#include "MiniMarryDialog.h"
#include "MiniBaishiDialog.h"

#include "JackFlashRoar.h"
#include "MovePoint.h"
#include "CharacterRBTN.h"
#include "ChannelDialog.h"
#include "GameState.h"
#include "AIManager.h"
#include "AutoPath.h"
#include "FBTimeDialog.h"
#include "UserInfoManager.h"
CHeroMove::CHeroMove()
{
}
CHeroMove::~CHeroMove()
{
}
void CHeroMove::OnMouseOver( CObject* pObject )
{
	if( !pObject )	return;
	OBJECTMGR->OnMouseOver( pObject );
	if( HERO )
	{
		eCURSOR cursor = eCURSOR_DEFAULT;

		switch (pObject->GetObjectKind())
		{
		case eObjectKind_Npc:
			{
				if (((CNpc*)pObject)->GetNpcKind() != 30 &&
					((CNpc*)pObject)->GetNpcKind() != 31)
					cursor = eCURSOR_DEALER;
			}
			break;	
		default:
			{
				CBattle* pBattle = BATTLESYSTEM->GetBattle(HERO);
				if (pBattle->IsEnemy(HERO, pObject))
					cursor = eCURSOR_ATTACK;
			}
			break;
		}

		CURSOR->SetCursor(cursor);

		/*CBattle* pBattle = BATTLESYSTEM->GetBattle( HERO );
		//CBattle* pBattle = BATTLESYSTEM->GetBattle( HERO->GetBattleID() );//kiv
		if( pBattle->IsEnemy( HERO, pObject ) )
		{
			BYTE bObjectKind = pObject->GetObjectKind();
			if( bObjectKind == eObjectKind_Mining )
			{
				CURSOR->SetCursor( eCURSOR_MINING );
			}
			else if( bObjectKind == eObjectKind_Collection )
			{
				CURSOR->SetCursor( eCURSOR_COLLECTION );
			}
			else if( bObjectKind == eObjectKind_Hunt )
			{
				CURSOR->SetCursor( eCURSOR_HUNT );
			}
			else
			{
				CURSOR->SetCursor( eCURSOR_ATTACK );
			}
		}
		else
		{
			if( pObject->GetObjectKind() == eObjectKind_Npc )
			{
				if( CURSOR->GetCursor() != eCURSOR_TITANREPAIR )
				{
					if( ((CNpc*)pObject)->GetNpcKind() != 30 && ((CNpc*)pObject)->GetNpcKind() != 31 )
						CURSOR->SetCursor( eCURSOR_DEALER );
				}
			}
			else if( CURSOR->GetCursor() != eCURSOR_TITANREPAIR )
                CURSOR->SetCursor( eCURSOR_DEFAULT );
		}*/
	}
}
void CHeroMove::OnMouseLeave( CObject* pObject )
{
	if( !pObject )	return;
	OBJECTMGR->OnMouseLeave( pObject );
}
BOOL saveClick=false;
void CHeroMove::OnClickPlayer(DWORD MouseEvent,CPlayer* pPlayer)
{
	if (pPlayer == NULL) return;

	HERO->GetEngineObject()->EnablePick(); //HERO->GetEngineObject()->DisablePick();

	DWORD dwPlayerID = pPlayer->GetID();

	if (HEROID == dwPlayerID)//if (HEROID == pPlayer->GetID())
	{
		/*if (MouseEvent & MOUSEEVENT_RCLICK)
		{
			HERO->GetEngineObject()->DisablePick();
			return;
		}*/
		if (MouseEvent & MOUSEEVENT_RCLICK)
		{
			HERO->GetEngineObject()->EnablePick();
			saveClick = true;
		}
		else if (MouseEvent & MOUSEEVENT_RBUTTONUP)
		{
			HERO->GetEngineObject()->EnablePick();
			if (saveClick)
			{
				saveClick = false;
				OBJECTMGR->SetSelectedObject(NULL);
				//CHATMGR->AddMsg(CTC_SYSMSG, "TEST");
				GAMEIN->GetCharacterRBTNDlg()->ShowSelfMenu(HERO);
				//return;
				HERO->GetEngineObject()->DisablePick();
			}
		}
		HERO->GetEngineObject()->DisablePick();
		
	}

	static DWORD dwDownPlayerID = 0;	
	if( MouseEvent & MOUSEEVENT_RCLICK )
	{		
		dwDownPlayerID = dwPlayerID;
	}
	else if( MouseEvent & MOUSEEVENT_RBUTTONUP)
	{
		if (dwDownPlayerID == dwPlayerID)
		{
			OBJECTMGR->SetSelectedObject(pPlayer);
			GAMEIN->GetCharacterRBTNDlg()->ShowOtherPlayerMenu(pPlayer);			
		}
	}
	else
	{
		OnAttack(pPlayer, MouseEvent);
	}
	if ( GAMEIN->GetMiniMaryDialog()->IsActive())
	{
		GAMEIN->GetMiniMaryDialog()->SetName(pPlayer->GetObjectName());
	}
	if (GAMEIN->GetMiniBaishiDialog()->IsActive())
	{
		GAMEIN->GetMiniBaishiDialog()->SetName(pPlayer->GetObjectName());
	}
}
void CHeroMove::OnClickMonster(DWORD MouseEvent,CMonster* pMonster)
{
#ifdef _TESTCLIENT_
	if(KEYBOARD->GetKeyPressed(KEY_CONTROL))
	{
		if(MouseEvent & MOUSEEVENT_LCLICK)
		{
			TARGETSET Target;
			Target.pTarget = pMonster;
			memset(&Target.ResultInfo,0,sizeof(RESULTINFO));
			Target.ResultInfo.bCritical = rand()%2 ? TRUE : FALSE;
			Target.ResultInfo.RealDamage = rand() % 50;
			Target.ResultInfo.ShieldDamage = rand() % 30;
			Target.ResultInfo.CounterDamage = 0;
			EFFECTMGR->StartEffectProcess(GAMERESRCMNGR->m_TestClientInfo.Effect,HERO,&Target,1,pMonster->GetID(),EFFECT_FLAG_HEROATTACK);
		}
		else if(MouseEvent == MOUSEEVENT_RCLICK)
		{
			TARGETSET Target;
			Target.pTarget = HERO;
			memset(&Target.ResultInfo,0,sizeof(RESULTINFO));
			Target.ResultInfo.bCritical = rand()%2 ? TRUE : FALSE;
			Target.ResultInfo.RealDamage = rand() % 50;
			Target.ResultInfo.ShieldDamage = rand() % 30;
			Target.ResultInfo.CounterDamage = 0;
			EFFECTMGR->StartEffectProcess(GAMERESRCMNGR->m_TestClientInfo.MonsterEffect,pMonster,&Target,1,HERO->GetID(),EFFECT_FLAG_HEROATTACK);
		}
		return;
	}
	else
	{
		CActionTarget Target;
		Target.InitActionTarget(pMonster,NULL);
		BOOL bMugong = FALSE;
		if(MouseEvent & MOUSEEVENT_RCLICK)
		{
			bMugong = TRUE;
			SKILLMGR->OnSkillCommand(HERO, &Target, bMugong);
		}
	}
#endif

	//if (0 == OBJECTMGR->GetSelectedObject() &&
	//	FALSE == HERO->GetBaseMoveInfo()->bMoving)
	//{
	//if (sCPlayerAI.IsOpen() && HERO->GetBaseMoveInfo()->bMoving)
	{
	//	MOVEMGR->MoveStop(HERO);
		//VECTOR3 position = pMonster->GetCurPosition() - HERO->GetCurPosition();
		//HERO->GetEngineObject()->SetDirection(&position);
		//OBJECTMGR->SetSelectedObject(pMonster);
	}
	//}

	//CHATMGR->AddMsg(CTC_SYSMSG, "%d", pMonster->GetGetID());
	OnAttack( pMonster, MouseEvent );
}
void CHeroMove::OnClickNpc(DWORD MouseEvent, CNpc* pNpc)
{
	if( HERO->GetState() != eObjectState_None &&
		HERO->GetState() != eObjectState_Move &&
		HERO->GetState() != eObjectState_Immortal )
		return;
	BYTE BattleKind = BATTLESYSTEM->GetBattle(HERO)->GetBattleKind();
	if( BattleKind != eBATTLE_KIND_NONE && BattleKind != eBATTLE_KIND_SIEGEWAR )	
		return;
	if( MouseEvent & MOUSEEVENT_DBLCLICK )
	{
		if( HERO->GetSingleSpecialState(eSingleSpecialState_Hide) )
		{
			CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(1329));
			return;
		}
		CActionTarget Target;
		Target.InitActionTarget( pNpc, NULL );
		CAction MoveAction;
		float fDistance = NPC_TALKING_DISTANCE;
		if( pNpc->GetNpcJob() == SYMBOL_ROLE )
			fDistance = NPC_TALKING_DISTANCE/2;
		MoveAction.InitAction( eActionKind_TalkToNpc, NULL, fDistance, &Target );
		MOVEMGR->SetHeroActionMove( &Target, &MoveAction );		
	}	
	else if( MouseEvent & MOUSEEVENT_LCLICK )
	{
		if( OBJECTMGR->GetSelectedObjectID() == pNpc->GetID() )
		{
			//CHATMGR->AddMsg(CTC_SYSMSG, "pNpc->GetID() %d", pNpc->GetNpcUniqueIdx());
			if( HERO->GetSingleSpecialState(eSingleSpecialState_Hide) )
			{
				CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(1329));
				return;
			}
				CActionTarget Target;
				Target.InitActionTarget( pNpc, NULL );
				CAction MoveAction;
				float fDistance = NPC_TALKING_DISTANCE;
				if( pNpc->GetNpcJob() == SYMBOL_ROLE )
					fDistance = NPC_TALKING_DISTANCE/2;
				MoveAction.InitAction( eActionKind_TalkToNpc, NULL, fDistance, &Target );
				MOVEMGR->SetHeroActionMove( &Target, &MoveAction );
		}
	}
}
void CHeroMove::OnClickSkillObject(DWORD MouseEvent,CSkillObject* pObj)
{
	//OnAttack( pObj, MouseEvent );
}
void CHeroMove::OnClickGround(DWORD MouseEvent,VECTOR3* pPos)
{
	if(KEYBOARD->GetKeyPressed(KEY_X))
	{
		if(MouseEvent & MOUSEEVENT_LCLICK)
		{
			MOVE_POS msg;
			msg.Category = MP_CHEAT;
			msg.Protocol = MP_CHEAT_MOVE_SYN;
			msg.dwObjectID = HEROID;
			msg.dwMoverID = HEROID;
			msg.cpos.Compress(pPos);
			NETWORK->Send(&msg,sizeof(msg));

			if(MOVEMGR->IsMoving(HERO))
			{
				MOVEMGR->HeroMoveStop();
			}
			return;
		}		
	}
	else
	{
		if(HERO)
		{
			if( MouseEvent & MOUSEEVENT_LCLICK )
			{
				MOVEMGR->SetHeroTarget(pPos,1.f);
				//MOVEMGR->SetHeroTarget(pPos,TRUE);
				MOVEPOINT->SetPoint(pPos);
				HERO->m_WayPointManager->SetFlagDestination(*pPos);
			}
			else
			{
				MOVEMGR->SetHeroTarget(pPos,FALSE);
			}

			HERO->DisableAutoAttack();
			HERO->SetCurComboNum(0);
			HERO->SetMovingAction(NULL);

			if(sCPlayerAI.IsOpen())
			{
				CHATMGR->AddMsgFade( CTC_SYSMSG, CHATMGR->GetChatMsg(2321));
			}
		}
	}
	/*if(KEYBOARD->GetKeyPressed(KEY_X) )
	{		
		MOVE_POS msg;
		msg.Category = MP_CHEAT;
		msg.Protocol = MP_CHEAT_MOVE_SYN;
		msg.dwObjectID = HEROID;
		msg.dwMoverID = HEROID;
		msg.cpos.Compress(pPos);
		NETWORK->Send(&msg,sizeof(msg));
		//HERO->DisableAutoAttack();
		//HERO->SetCurComboNum(0);
		//HERO->SetMovingAction(NULL);
	}	
	else
	{
		if(HERO)
		{
			if( MouseEvent & MOUSEEVENT_LCLICK )
			{
				MOVEMGR->SetHeroTarget(pPos,1.f);
				//MOVEMGR->SetHeroTarget(pPos,TRUE);
				MOVEPOINT->SetPoint(pPos);
			}
			else
				MOVEMGR->SetHeroTarget(pPos,FALSE);
			HERO->DisableAutoAttack();
			HERO->SetCurComboNum(0);
			HERO->SetMovingAction(NULL);
			if(sCPlayerAI.IsOpen())
			{
				CHATMGR->AddMsgFade( CTC_SYSMSG, CHATMGR->GetChatMsg(2321));
			}
		}
	}*/
}
void CHeroMove::OnClickPet(DWORD MouseEvent, CPetBase* pPet)
{
	//CHATMGR->AddMsg(CTC_SYSMSG, "%d", pPet->GetID());
	/*if( HERO->GetState() != eObjectState_None &&
		HERO->GetState() != eObjectState_Move &&
		HERO->GetState() != eObjectState_Immortal )
		return;
	if( BATTLESYSTEM->GetBattle(HERO)->GetBattleKind() != eBATTLE_KIND_NONE )	
		return;
	if( MouseEvent & MOUSEEVENT_DBLCLICK )
	{
		CActionTarget Target;
		Target.InitActionTarget( pPet, NULL );
		CAction MoveAction;
		MoveAction.InitAction( eActionKind_TalkToPet, NULL, NPC_TALKING_DISTANCE, &Target );
		MOVEMGR->SetHeroActionMove( &Target, &MoveAction );		
	}	
	else if( MouseEvent & MOUSEEVENT_LCLICK )
	{
		if( OBJECTMGR->GetSelectedObjectID() == pPet->GetID() )
		{
			CActionTarget Target;
			Target.InitActionTarget( pPet, NULL );
			CAction MoveAction;
			MoveAction.InitAction( eActionKind_TalkToPet, NULL, NPC_TALKING_DISTANCE, &Target );
			MOVEMGR->SetHeroActionMove( &Target, &MoveAction );
		}
	}*/
}
void CHeroMove::OnClickMapObject(DWORD MouseEvent, CObject* pObj)
{
	/*if (pObj)
	{
		int obj = pObj->GetID();

		CHATMGR->AddMsg(CTC_SYSMSG, "this is mapobject %d", obj);
	}*/
}
#define ATTACK_TICK	100  
BOOL IsEventAttackable(CObject* pTargetObj)
{
	if( HERO->GetWeaponEquipType() == WP_EVENT )
	{
		CItem* pItem = GAMEIN->GetInventoryDialog()->GetPriorityItemForCristmasEvent();
		if( !pItem )
		{
			CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(583) );
			return	FALSE;
		}
	}
	else
	{
		if( pTargetObj->GetObjectKind() & eObjectKind_Monster )
		{
			CMonster* pMonster = (CMonster*)pTargetObj;
			if( pMonster->GetMonsterKind() == EVENT_MONSTER_SANTA1 
				|| pMonster->GetMonsterKind() == EVENT_MONSTER_SANTA2
				|| pMonster->GetMonsterKind() == EVENT_FIELDBOSS_SNOWMAN_SM 
				|| pMonster->GetMonsterKind() == EVENT_FIELDSUB_SNOWMAN_SM
				|| pMonster->GetMonsterKind() == EVENT_FIELDBOSS_SNOWMAN_MD 
				|| pMonster->GetMonsterKind() == EVENT_FIELDSUB_SNOWMAN_MD
				|| pMonster->GetMonsterKind() == EVENT_FIELDBOSS_SNOWMAN_LG 
				|| pMonster->GetMonsterKind() == EVENT_FIELDSUB_SNOWMAN_LG
				)
			{
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(584) );
				return FALSE;
			}
		}
	}
	return TRUE;
}
void CHeroMove::OnAttack(CObject* pTargetObj,DWORD MouseEvent)
{	
	static CObject* pLastObj		= NULL;
	static DWORD	dwAttackTime	= 0;

	if( HERO && pTargetObj )
	{
		if( HERO->GetCharacterTotalInfo()->bVisible )
		{
			if( pLastObj != pTargetObj || gCurTime - dwAttackTime > ATTACK_TICK )
			{
				if( pTargetObj->GetState() == eObjectState_Die )
					return;

				if( MouseEvent & MOUSEEVENT_LCLICK )
				{
					if( OBJECTMGR->GetSelectedObjectID() == pTargetObj->GetID() )
					{
						if (!IsEventAttackable(pTargetObj))
							return;
						if( HERO->GetSingleSpecialState(eSingleSpecialState_Hide) )
						{
							CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(1329));
							return;
						}
						if (HERO->IsPKMode())
						if (PARTYMGR->IsPartyMember(pTargetObj->GetID()))
						{
							CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(630));
							return;
						}

						CActionTarget Target;
						Target.InitActionTarget(pTargetObj, NULL);

						HERO->EnableAutoAttack(&Target);
						SKILLMGR->OnSkillCommand(HERO, &Target, FALSE);
						pLastObj = pTargetObj;
						dwAttackTime = gCurTime;
						/*if( !IsEventAttackable(pTargetObj) )
							return;
						if( HERO->IsPKMode())
						{
							if( PARTYMGR->IsPartyMember( pTargetObj->GetID() ) )						
							{
								CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(630) );
								return;
							}
							if(GUILDMGR->IsSameGuild( (CPlayer*)HERO, (CPlayer*)pTargetObj) )
							{
								CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(932) );
								return ;
							}
							if( GUILDUNION->IsSameUnion( (CPlayer*)HERO, (CPlayer*)pTargetObj) )
							{
								CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(932) );
								return ;
							}
						}
						pTargetObj->ShowObjectName( TRUE, RGB_HALF( 255, 141, 39 ) );
						CActionTarget Target;
						Target.InitActionTarget(pTargetObj,NULL);						
						HERO->EnableAutoAttack(&Target);		
						SKILLMGR->OnSkillCommand(HERO, &Target, FALSE);
						pLastObj		= pTargetObj;
						dwAttackTime	= gCurTime;*/
					}
				}
				else if( MouseEvent & MOUSEEVENT_DBLCLICK )
				{
					if (!IsEventAttackable(pTargetObj))
						return;
					if( HERO->GetSingleSpecialState(eSingleSpecialState_Hide) )
					{
						CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(1329));
						return;
					}

					if (HERO->IsPKMode())
					if (PARTYMGR->IsPartyMember(pTargetObj->GetID()))
					{
						CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(630));
						return;
					}
					CActionTarget Target;
					Target.InitActionTarget(pTargetObj, NULL);

					HERO->EnableAutoAttack(&Target);
					SKILLMGR->OnSkillCommand(HERO, &Target, FALSE);
					pLastObj = pTargetObj;
					dwAttackTime = gCurTime;

					/*if( !IsEventAttackable(pTargetObj) )
						return;
					if( HERO->IsPKMode())
					{
						if( PARTYMGR->IsPartyMember( pTargetObj->GetID() ) )						
						{
							CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(630) );
							return;
						}
						if( GUILDMGR->IsSameGuild( (CPlayer*)HERO, (CPlayer*)pTargetObj) )
						{
							CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(932) );
							return ;
						}
						if( GUILDUNION->IsSameUnion( (CPlayer*)HERO, (CPlayer*)pTargetObj) )
						{
							CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(932) );
							return ;
						}
					}
					pTargetObj->ShowObjectName( TRUE, RGB_HALF( 255, 141, 39 ) );
					CActionTarget Target;
					Target.InitActionTarget(pTargetObj,NULL);
					HERO->EnableAutoAttack(&Target);		
					SKILLMGR->OnSkillCommand(HERO, &Target, FALSE);	
					pLastObj		= pTargetObj;
					dwAttackTime	= gCurTime;*/
				}
			}
		}
	}
}
void CHeroMove::OnDbClickPlayer(DWORD MouseEvent,CPlayer* pPlayer)
{
	if( HERO->GetState() != eObjectState_None &&
		HERO->GetState() != eObjectState_Move &&
		HERO->GetState() != eObjectState_Immortal )
		return;
	if( OBJECTMGR->GetSelectedObjectID() )
	{
		if( OBJECTMGR->GetSelectedObjectID() == pPlayer->GetID() )
		{
			if( pPlayer->GetState() == eObjectState_StreetStall_Owner )
			{
				CActionTarget Target;
				Target.InitActionTarget( pPlayer, NULL );
				CAction MoveAction;
				MoveAction.InitStallAction( STREETSTALL_ENTER_DISTANCE, &Target );
				MOVEMGR->SetHeroActionMove( &Target, &MoveAction );
			}
			else
			{
				OnAttack( pPlayer, MOUSEEVENT_DBLCLICK );
			}
		}
	}
}
void CHeroMove::OnDbClickMonster(DWORD MouseEvent,CMonster* pMonster)
{
	OnAttack( pMonster, MouseEvent );
}
void CHeroMove::OnDbClickNpc(DWORD MouseEvent, CNpc* pNpc)
{
	if( HERO->GetState() != eObjectState_None &&
		HERO->GetState() != eObjectState_Move &&
		HERO->GetState() != eObjectState_Immortal )
		return;
	if( HERO->GetSingleSpecialState(eSingleSpecialState_Hide) )
	{
		CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(1329));
		return;
	}	
	CActionTarget Target;
	Target.InitActionTarget( pNpc, NULL );
	CAction MoveAction;
	float fDistance = NPC_TALKING_DISTANCE;
	if( pNpc->GetNpcJob() == SYMBOL_ROLE )
		fDistance = NPC_TALKING_DISTANCE/2;
	MoveAction.InitAction( eActionKind_TalkToNpc, NULL, fDistance, &Target );
	MOVEMGR->SetHeroActionMove( &Target, &MoveAction );
}
#include "PathManager.h"
void CHeroMove::AttackSelectedObject()
{
	CObject* pTargetObj = OBJECTMGR->GetSelectedObject();
	if( !pTargetObj ) return;
	//if(MOVEMGR->IsMoving(HERO) && sCPlayerAI.IsOpen())return;

	VECTOR3 TempPos;
	VECTOR3 SelPos;
	pTargetObj->GetPosition(&SelPos);
	HERO->GetPosition(&TempPos);
	if(MOVEMGR->IsMoving(HERO) & PATHMGR->CheckCollisionLine(&TempPos,&SelPos,HERO)
		)
	{
		//CHATMGR->AddMsg( CTC_SYSMSG, "blocked" );
		return;
	}
	//else
		//CHATMGR->AddMsg( CTC_SYSMSG, "not blocked" );

	OnAttack(pTargetObj,MOUSEEVENT_LCLICK);
}
void CHeroMove::SelectLoginPoint( WORD LoginPointIndex )
{
	MSG_DWORD msg;
	msg.Category = MP_USERCONN;
	msg.Protocol = MP_USERCONN_SAVEPOINT_SYN;
	msg.dwObjectID = HEROID;
	msg.dwData = LoginPointIndex;
	NETWORK->Send(&msg, sizeof(msg));
	CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(161) );
}

void CHeroMove::MapChange( WORD MapIndex, DWORD ItemIdx, DWORD ItemPos,DWORD state,BYTE bFlagType )
{
//	AUTOPATH->SetFindPath( false );
//	Sleep(15);
//	sCPlayerAI.SetOpen(FALSE);
//	AISETMGR->GetGameAI()->sPage5.pbAutoLeveling==false;
	GAMEIN->GetJackFlashDlg()->SetMapChange(true);
//	sCPlayerAI.IsRun=FALSE;

	sCPlayerAI.BeforeMapChange();

	//g_UserInput.SetInputFocus(FALSE);
	GAMEIN->UpdataGoldMoney();

	if(bFlagType==0)
	{
		MSG_DWORD5 msg;
		msg.Category = MP_USERCONN;
		msg.Protocol = MP_USERCONN_CHANGEMAP_SYN;
		msg.dwObjectID = HEROID;
		msg.dwData1 = MapIndex;
		msg.dwData2 = ItemIdx;
		msg.dwData3 = ItemPos;
		msg.dwData4 = state;
		msg.dwData5 = gChannelNum+1;		
		
		CHANNEL->SetMoveMsg(msg);   
		CHANNEL->SetMove(TRUE);
	}
	if(bFlagType==1)
	{
		MSG_NAME_DWORD2 msg;
		msg.Category	= MP_MOVE;
		msg.Protocol	= MP_MOVE_RECONN_SYN;
		msg.dwObjectID	= HEROID;
		msg.dwData1		= MapIndex;
		msg.dwData2		= gChannelNum+1;
		msg.Name[0]		= 0;	
		NETWORK->Send( &msg, sizeof(msg) );
	}
}
#include "Reconnect.h"
void CHeroMove::MapPosition(CObject*pObject,VECTOR3*vSavedPos)
{
	MOVE_POS_USER msg;
	msg.Category	= MP_MOVE;
	msg.Protocol	= MP_MOVE_RECONN_POS_SYN;
	msg.dwObjectID	= HEROID;

	if(REDISCONN->GetStart())
		msg.CyptKey		= 0;
	else
		msg.CyptKey		= 1;
	SafeStrCpy( msg.Name, pObject->GetObjectName(), MAX_NAME_LENGTH + 1 );
	msg.cpos.Compress(vSavedPos);
	NETWORK->Send(&msg,sizeof(msg));
}