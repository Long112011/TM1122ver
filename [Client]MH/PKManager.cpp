// PKManager.cpp: implementation of the CPKManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "PKManager.h"
#include "ObjectManager.h"
#include "GameIn.h"
#include "CharStateDialog.h"
#include "ChatManager.h"
#include "./Interface/cWindowManager.h"
#include "WindowIdEnum.h"
#include "cMsgBox.h"
#include "VimuManager.h"
#include "PKLootingDialog.h"
#include "ItemManager.h"
#include "InventoryExDialog.h"
#include "QuickManager.h"
#include "GameEventManager.h"
#include "MugongManager.h"
#include "ObjectStateManager.h"
#include "SkillManager_Client.h"
#include "PartyWar.h"
#include "AIManager.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

GLOBALTON(CPKManager)

CPKManager::CPKManager()
{
	m_dwPKContinueTime	= PKMODETIME;
	m_htLootingDlg.Initialize( 3 );
}

CPKManager::~CPKManager()
{
}

void CPKManager::Init()
{
	m_IndexGenerator.Init(1000, IG_PKLOOTINGDLG_START);	
	m_bSyncing			= FALSE;
	m_bLooted			= FALSE;
	m_bLooting			= FALSE;
	m_bPKAllow			= FALSE;
}

void CPKManager::Release()
{
	m_htLootingDlg.RemoveAll();	//window new한건 windowmanager가 지워준�?
	m_IndexGenerator.Release();
}

BOOL CPKManager::HEROPKModeOn()
{
	if( !IsPKAllow() )
	{
		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg( 145 ) );
		return FALSE;
	}		
	if( PARTYWAR->IsPartyWar() )
	{
		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg( 550 ) );
		return FALSE;
	}
	if( m_bSyncing )			return FALSE;
	if( !HERO )					return FALSE;
	if( HERO->IsPKMode() )		return FALSE;
	if( WINDOWMGR->GetWindowForID( MBI_PKMODE ) ) return FALSE;

	cMsgBox* pMsgBox = WINDOWMGR->MsgBox( MBI_PKMODE, MBT_YESNO, CHATMGR->GetChatMsg( 149 ) );		
	if( !pMsgBox ) return FALSE;

	GAMEEVENTMGR->AddEvent( eGameEvent_PKInterface );

	return TRUE;
}

void CPKManager::SendMsgPKOn()
{
/*	
	if( HERO->GetState() != eObjectState_None && HERO->GetState() != eObjectState_Move )
	{
		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg( 557 ) );
		return;			//보통상태에서�?가�?
	}
*/

	if( HERO->GetState() == eObjectState_Die )
	{
		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg( 557 ) );
		return;			//죽은상태�?아니�?
	}

	if( VIMUMGR->IsVimuing() )	//비무중엔 안된�?
	{
		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg( 550 ) );
		return;
	}

	////////////////////////////////////////////////////////////////////////////////
	// 06. 06. 2�?전직 - 이영준
	// 은�?/ 혜안
	// 은�?패널�?
	if( HERO->GetSingleSpecialState(eSingleSpecialState_Hide) )
	{
		CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(1329));
		return;
	}
	////////////////////////////////////////////////////////////////////////////////

	MSGBASE msg;
	msg.Category	= MP_PK;
	msg.Protocol	= MP_PK_PKON_SYN;
	msg.dwObjectID	= HERO->GetID();
	NETWORK->Send( &msg, sizeof(msg) );

	m_bSyncing		= TRUE;
}

BOOL CPKManager::HEROPKModeOff()
{
	if( m_bSyncing )		return FALSE;
	if( !HERO )				return FALSE;
	if( !HERO->IsPKMode() )	return FALSE;	//이미 off상태이다.

	if( gCurTime - m_dwPKModeStartTime >= m_dwPKContinueTime )
	{
		MSGBASE msg;
		msg.Category	= MP_PK;
		msg.Protocol	= MP_PK_PKOFF_SYN;
		msg.dwObjectID	= HERO->GetID();
		NETWORK->Send( &msg, sizeof(msg) );

		m_bSyncing		= TRUE;
		
		return TRUE;
	}
	else
	{
		int nRemain = (int)( m_dwPKContinueTime - (gCurTime - m_dwPKModeStartTime) ) / 1000;
		if( nRemain < 0 ) nRemain = 0;				
//		else if( nRemain > 60 ) nRemain = 60;//나중�?상태에따�?초가 늘어나야한다.
		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(146), nRemain + 1); // 올림+1
	}

	return FALSE;
}

void CPKManager::SetPlayerPKMode( CPlayer* pPlayer, BOOL bPKMode )
{
	pPlayer->SetPKMode( bPKMode );
}

BOOL CPKManager::ToggleHeroPKMode()
{
	if( HERO )
	if( HERO->IsPKMode() )
	{
		return HEROPKModeOff();
	}
	else
	{
		return HEROPKModeOn();
	}

	return FALSE;

}

void CPKManager::NetworkMsgParse( WORD Protocol, void* pMsg )
{
	switch(Protocol)
	{
	case MP_PK_PKON_ACK:
		{
			MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;

			CPlayer* pPlayer = (CPlayer*)OBJECTMGR->GetObject( pmsg->dwData );
			if( !pPlayer ) break;

			if( HERO )
			if( pmsg->dwData == HEROID )
			{
				m_bSyncing			= FALSE;
				m_dwPKModeStartTime	= gCurTime;
				m_dwPKContinueTime	= HERO->GetBadFame()/200 + PKMODETIME; //계산식을 넣자.
				if( m_dwPKContinueTime > 3600000 )
					m_dwPKContinueTime = 3600000;

				HERO->DisableAutoAttack();	//자동 공격 취소
				HERO->SetNextAction(NULL);	//스킬 취소
				if( HERO->GetState() == eObjectState_Immortal )
					OBJECTSTATEMGR->EndObjectState( HERO, eObjectState_Immortal );

				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(147) );
			}
			
			pPlayer->SetPKMode( TRUE );

			//캐릭�?오버인포 바꾸�?
			OBJECTMGR->ApplyOverInfoOption( pPlayer );
			if( pPlayer->InTitan() )
			{
				OBJECTEFFECTDESC desc(FindEffectNum("t_maintain_PK_S.beff"));
				pPlayer->AddObjectEffect( PK_EFFECT_ID, &desc, 1, HERO);

			}
			else
			{
				OBJECTEFFECTDESC desc(FindEffectNum("maintain_PK_S.beff"));
				//SW050915 살기�?이펙�?옵션�?상관없이 항상 보이�?
				//pPlayer->AddObjectEffect( PK_EFFECT_ID, &desc, 1 );
				pPlayer->AddObjectEffect( PK_EFFECT_ID, &desc, 1, HERO);	//HERO �?MakeEffectUnit( 에서 보여주기위한 하드코딩
			}
		}
		break;

	case MP_PK_PKON_NACK:
		{
			MSG_BYTE* pmsg = (MSG_BYTE*)pMsg;
			m_bSyncing		= FALSE;

			switch( pmsg->bData )
			{
			case ePKCODE_VIMUING:
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg( 550 ) );
				break;
			case ePKCODE_STATECONFLICT:
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg( 557 ) );
				break;
			case ePKCODE_NOTALLAW:
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg( 145 ) );
				break;
			}
		}
		break;

	case MP_PK_PKOFF_ACK:
		{
			MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;

			CPlayer* pPlayer = (CPlayer*)OBJECTMGR->GetObject( pmsg->dwData );
			if( !pPlayer ) break;

			pPlayer->SetPKMode( FALSE );
			
			if( HERO )
			if( pmsg->dwData == HERO->GetID() )
			{
				m_bSyncing		= FALSE;
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(148) );
			}
			else
			{
				//현재 공격타겟이 pk�?�?유저라면 타겟을 없애�?
				HERO->ClearTarget( pPlayer );				
			}

			//캐릭�?오버인포 바꾸�?
			OBJECTMGR->ApplyOverInfoOption( pPlayer );
			pPlayer->RemoveObjectEffect( PK_EFFECT_ID );


		}
		break;

	case MP_PK_DISAPPEAR:	//PK_OFF�?메세지�?다르�?..--;
		{
			MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;

			CPlayer* pPlayer = (CPlayer*)OBJECTMGR->GetObject( pmsg->dwData );
			if( !pPlayer ) break;

			pPlayer->SetPKMode( FALSE );
			
			if( HERO )
			if( pmsg->dwData == HERO->GetID() )
			{
				//m_bSyncing		= FALSE;
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(628) );
			}
			else
			{
				//현재 공격타겟이 pk�?�?유저라면 타겟을 없애�?
				HERO->ClearTarget( pPlayer );
			}

			//캐릭�?오버인포 바꾸�?
			OBJECTMGR->ApplyOverInfoOption( pPlayer );
			pPlayer->RemoveObjectEffect( PK_EFFECT_ID );
		}
		break;

	case MP_PK_PKOFF_NACK:
		{
			//시간이안되었�?
			m_bSyncing		= FALSE;
		}
		break;

	case MP_PK_LOOTING_START:
		{
			MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;
			if(AISETMGR->GetGameAI()->sPage8.isAutoCloseGetItemWin)
			{
				MSG_DWORD	msg;
				msg.Category	= MP_PK;
				msg.Protocol	= MP_PK_LOOTING_ENDLOOTING;
				msg.dwObjectID	= HEROID;
				msg.dwData		= pmsg->dwData;
				NETWORK->Send( &msg, sizeof(msg) );
				break;
			}
			CreateLootingDlg( pmsg->dwData );
			SetPKLooting( TRUE );
			GAMEEVENTMGR->AddEvent( eGameEvent_KillPK );

		}
		break;

	case MP_PK_LOOTING_BEINGLOOTED:
		{
			MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;
			CPlayer* pAttacker = (CPlayer*)OBJECTMGR->GetObject( pmsg->dwData );
			if( pAttacker )
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg( 562 ), pAttacker->GetObjectName() );

			SetPKLooted( TRUE );

			GAMEEVENTMGR->AddEvent( eGameEvent_DieFromPK );
			//부활이 안되도록 하자. 서버에서 보내주는 방식으로 바꾸는것 검�?
			//종료�?안되도록 하자.
			//아이�?조작�?안되도록하자
			//기존�?아이템을 조작하고 있었다면 모두 최소시킨�?(버리�?사기 팔기�?
		}
		break;

	case MP_PK_LOOTING_ITEMLOOTING:
		{
			//SW070127 타이탄
			//루팅 예외처리�?정리 //SW070308 아이템부가옵션�?정리 참조
			/*
			MSG_LOOTINGIEM* pmsg = (MSG_LOOTINGIEM*)pMsg;
			if( pmsg->bOptionItem )
				ITEMMGR->AddItemOption(&pmsg->sOptionInfo);
			if( pmsg->bRareOptionItem )
				ITEMMGR->AddItemRareOption(&pmsg->sRareOptionInfo);
			
			CItem* pItem = ITEMMGR->MakeNewItem(&pmsg->ItemInfo,"MP_PK_LOOTING_ITEMLOOTING");
			GAMEIN->GetInventoryDialog()->AddItem(pItem);
			QUICKMGR->RefreshQickItem();

			CPKLootingDialog* pDlg = m_htLootingDlg.GetData( pmsg->dwDiePlayerIdx );*/
			MSG_LOOTINGITEM* pmsg = (MSG_LOOTINGITEM*)pMsg;
			CItem* pItem = ITEMMGR->MakeNewItem(&pmsg->ItemInfo,"MP_PK_LOOTING_ITEMLOOTING");
			ITEMMGR->GetItemOptionsAndToolTipFromInfoMsg(pItem, (MSG_LINKITEMOPTIONS*)pmsg);

			GAMEIN->GetInventoryDialog()->AddItem(pItem);
			QUICKMGR->RefreshQickItem();

			CPKLootingDialog* pDlg = m_htLootingDlg.GetData( pmsg->dwMoney );
////

			if( pDlg )
			{
				pDlg->ChangeIconImage( pmsg->wAbsPosition, eLIK_ITEM, pmsg->ItemInfo.wIconIdx );
				pDlg->AddLootingItemNum();
				pDlg->SetMsgSync( FALSE );
			}

			CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg( 563 ), 
				ITEMMGR->GetItemInfo(pmsg->ItemInfo.wIconIdx)->ItemName );

			ITEMMGR->ItemDropEffect( pmsg->ItemInfo.wIconIdx );
		}
		break;

	case MP_PK_LOOTING_ITEMLOOTED:
		{
			MSG_EXCHANGE_REMOVEITEM* pmsg = (MSG_EXCHANGE_REMOVEITEM*)pMsg;
			WORD wTableIdx = ITEMMGR->GetTableIdxForAbsPos( pmsg->wAbsPosition );
			const ITEMBASE* pItemInfo = ITEMMGR->GetItemInfoAbsIn( HERO, pmsg->wAbsPosition );
			ITEMMGR->DeleteItemofTable( wTableIdx, pmsg->wAbsPosition );
			QUICKMGR->RefreshQickItem();
			
			CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg( 564 ), 
				ITEMMGR->GetItemInfo(pItemInfo->wIconIdx)->ItemName );
		}
		break;

	case MP_PK_LOOTING_MOENYLOOTING:
		{
			MSG_DWORD3* pmsg = (MSG_DWORD3*)pMsg;
			CPKLootingDialog* pDlg = m_htLootingDlg.GetData( pmsg->dwData1 );
			if( pDlg )
			{
				pDlg->ChangeIconImage( (WORD)pmsg->dwData2, eLIK_MONEY );
				pDlg->AddLootingItemNum();
				pDlg->SetMsgSync( FALSE );
			}
			CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg( 565 ), AddComma( pmsg->dwData3 ) );
			ITEMMGR->MoneyDropEffect();
		}
		break;

	case MP_PK_LOOTING_MOENYLOOTED:
		{
			MSG_DWORD3* pmsg = (MSG_DWORD3*)pMsg;

			CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg( 566 ), AddComma( pmsg->dwData3 ) );
		}
		break;

	case MP_PK_LOOTING_EXPLOOTING:
		{
			MSG_DWORD3* pmsg = (MSG_DWORD3*)pMsg;
			CPKLootingDialog* pDlg = m_htLootingDlg.GetData( pmsg->dwData1 );
			if( pDlg )
			{
				pDlg->ChangeIconImage( (WORD)pmsg->dwData2, eLIK_EXP );
				pDlg->AddLootingItemNum();
				pDlg->SetMsgSync( FALSE );
			}
			CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg( 567 ), pmsg->dwData3 );

			// magi82(4) - Titan(071022) 타이탄일때 이펙�?처리(pk 경험�?룻팅)
			if( HERO->InTitan() == TRUE )
                EFFECTMGR->StartHeroEffectProcess(eEffect_Titan_PK_LootExp);
			else
				EFFECTMGR->StartHeroEffectProcess(eEffect_PK_LootExp);
		}
		break;

	case MP_PK_LOOTING_EXPLOOTED:
		{
			MSG_DWORD3* pmsg = (MSG_DWORD3*)pMsg;
			CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg( 568 ), pmsg->dwData3 );
		}
		break;

	case MP_PK_LOOTING_NOLOOTING:
		{
			MSG_DWORD2* pmsg = (MSG_DWORD2*)pMsg;
			CPKLootingDialog* pDlg = m_htLootingDlg.GetData( pmsg->dwData1 );
			if( pDlg )
			{
				pDlg->ChangeIconImage( (WORD)pmsg->dwData2, eLIK_NONE );	//빈것으로
				pDlg->SetMsgSync( FALSE );
			}
		}
		break;
		
	case MP_PK_LOOTING_NOINVENSPACE:
		{
			MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;
			CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg( 569 ) );
			CPKLootingDialog* pDlg = m_htLootingDlg.GetData( pmsg->dwData );
			if( pDlg )
				pDlg->SetMsgSync( FALSE );
		}
		break;

	case MP_PK_LOOTING_ENDLOOTING:
		{
			MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;
			if( pmsg->dwData == HEROID )
			{
				SetPKLooted( FALSE );
			}
			else
			{
				CPKLootingDialog* pDlg = m_htLootingDlg.GetData( pmsg->dwData );
				if( pDlg )
				{
					pDlg->SetLootingEnd( TRUE );
					CloseLootingDialog( pDlg, FALSE );
				}
			}
		}
		break;

	case MP_PK_DESTROY_ITEM:
		{
			MSG_ITEM_DESTROY* pmsg = (MSG_ITEM_DESTROY*)pMsg;

			WORD wTableIdx = ITEMMGR->GetTableIdxForAbsPos( pmsg->wAbsPosition );
			const ITEMBASE* pItemInfo = ITEMMGR->GetItemInfoAbsIn( HERO, pmsg->wAbsPosition );

			if( pItemInfo == NULL ) return;
			ITEMMGR->DeleteItemofTable( wTableIdx, pmsg->wAbsPosition );
			QUICKMGR->RefreshQickItem();
			
			ITEM_INFO* pInfo = ITEMMGR->GetItemInfo( pItemInfo->wIconIdx );
			if( pInfo )
			{
				if( pmsg->cbReason == 0 )
					CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg( 569 ), pInfo->ItemName );
				else
					CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg( 688 ), pInfo->ItemName );
			}
		}
		break;
	case MP_PK_LOOTING_ERROR:
		{
			MSG_DWORD2* pmsg = (MSG_DWORD2*)pMsg;
			CPKLootingDialog* pDlg = m_htLootingDlg.GetData( pmsg->dwData1 );
			if( pDlg )
				pDlg->SetMsgSync( FALSE );

			//에러
			switch( pmsg->dwData2 )
			{
			case eLOOTINGERROR_OVER_DISTANCE:
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg( 561 ) ); //거리가 너무 멀�?
				break;
			default:
				break;
			}
		}
		break;
	case MP_PK_PKALLOW_CHANGED:
		{
			MSG_BYTE* pmsg = (MSG_BYTE*)pMsg;
			PKMGR->SetPKAllow( (BOOL)pmsg->bData );

			//모든 플레이어�?PK모드 끄기.
			if( PKMGR->IsPKAllow() )
			{
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(627) );
			}
			else
			{
				OBJECTMGR->AllPlayerPKOff();
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(626) );
			}
		}
		break;
	}
}


//--------------
//인터페이�?관�?
CPKLootingDialog* CPKManager::CreateLootingDlg( DWORD dwDiePlayerIdx )
{
	//매번 파일에서 읽어온다...?
#ifdef _FILE_BIN_	
//	CPKLootingDialog* pLootDlg = (CPKLootingDialog*)WINDOWMGR->GetDlgInfoFromFile( "./image/Looting.bin", "rb" );
	CPKLootingDialog* pLootDlg = (CPKLootingDialog*)WINDOWMGR->GetDlgInfoFromFile( "./image/InterfaceScript/Looting.bin", "rb" );
#else
	CPKLootingDialog* pLootDlg = (CPKLootingDialog*)WINDOWMGR->GetDlgInfoFromFile( "./image/Looting.txt" );
#endif
	pLootDlg->Linking();

	//늪뇹뚤PK溝固롸길�?쏵契뇹잿 2016-08-20
	LONG x = ( (float)GAMERESRCMNGR->m_GameDesc.dispInfo.dwWidth - 251 ) / 2;
	LONG y = ( (float)GAMERESRCMNGR->m_GameDesc.dispInfo.dwHeight - 275 ) / 2;

	//표지션이 겹치�?dlg가 있으�?위치조정
	PositioningDlg( x, y );

	pLootDlg->InitPKLootDlg( AllocWindowId(), x, y, dwDiePlayerIdx );
	WINDOWMGR->AddWindow( pLootDlg );
	
	pLootDlg->SetActive(TRUE);


//이미 존재하는가 한번 보자	//반드�?이위�? 위에�?먼저 검사하�?안된�? 윈도�?아이디문�?
	CPKLootingDialog* pLootTempDlg = m_htLootingDlg.GetData( dwDiePlayerIdx );
	if( pLootTempDlg )
		CloseLootingDialog( pLootTempDlg, FALSE );

	m_htLootingDlg.Add( pLootDlg, dwDiePlayerIdx );

	return pLootDlg;
}

void CPKManager::PositioningDlg( LONG& rX, LONG& rY )
{
	int nCount = 0;
	m_htLootingDlg.SetPositionHead();
	CPKLootingDialog* pLootDlg = NULL;
	while( pLootDlg = m_htLootingDlg.GetData() )
	{
		if( (LONG)pLootDlg->GetAbsX() == rX )
		if( (LONG)pLootDlg->GetAbsY() == rY )
		{
			rX += 20;
			rY += 20;
			if( ++nCount >= 10 )//한없�?내려가는것 방지
			{
				rX -= 200;
				rY -= 200;
				nCount = 0;
			}
		}
	}
}


void CPKManager::CloseLootingDialog( CPKLootingDialog* pLootingDlg, BOOL bMsgToServer )
{
	pLootingDlg->SetDisable( FALSE );		//^^ disable인건 acitve가 안먹�?
	pLootingDlg->SetActiveRecursive( FALSE );
	pLootingDlg->ReleaseAllIcon();
	pLootingDlg->SetDisable( TRUE );
	m_htLootingDlg.Remove( pLootingDlg->GetDiePlayerIdx() );
	FreeWindowId( pLootingDlg->GetID() );
	WINDOWMGR->AddListDestroyWindow( pLootingDlg );

	if( m_htLootingDlg.GetDataNum() == 0 )
	{
		SetPKLooting( FALSE );	//서버와 맞추어야 할텐�?
	}

	if( bMsgToServer )
	{
		MSG_DWORD	msg;
		msg.Category	= MP_PK;
		msg.Protocol	= MP_PK_LOOTING_ENDLOOTING;
		msg.dwObjectID	= HEROID;
		msg.dwData		= pLootingDlg->GetDiePlayerIdx();

		NETWORK->Send( &msg, sizeof(msg) );
	}
}

int CPKManager::GetLootingChance( DWORD dwBadFame )
{
#ifdef _HK_LOCAL_
#ifndef _TW_LOCAL_
	if( dwBadFame < 1000 )
		return 3;
	else if( dwBadFame < 2000 )
		return 4;
	else if( dwBadFame < 5000 )
		return 5;
	else if( dwBadFame < 10000 )
		return 6;
	else if( dwBadFame < 20000 )
		return 7;
	else if( dwBadFame < 50000 )
		return 8;
	else if( dwBadFame < 100000 )
		return 9;
	else
		return 10;
#else

	if( dwBadFame < 100000 )
		return 3;
	else if( dwBadFame < 500000 )
		return 4;
	else if( dwBadFame < 1000000 )
		return 5;
	else if( dwBadFame < 5000000 )
		return 6;
	else if( dwBadFame < 10000000 )
		return 7;
	else if( dwBadFame < 50000000 )
		return 8;
	else if( dwBadFame < 100000000 )
		return 9;
	else
		return 10;
#endif
#else

	if( dwBadFame < 100000 )
		return 3;
	else if( dwBadFame < 500000 )
		return 4;
	else if( dwBadFame < 1000000 )
		return 5;
	else if( dwBadFame < 5000000 )
		return 6;
	else if( dwBadFame < 10000000 )
		return 7;
	else if( dwBadFame < 50000000 )
		return 8;
	else if( dwBadFame < 100000000 )
		return 9;
	else
		return 10;
#endif
}

int CPKManager::GetLootingItemNum( DWORD dwBadFame )
{
#ifdef _HK_LOCAL_
#ifndef _TW_LOCAL	//홍콩�?일단 바꿔�?적용
	if( dwBadFame < 100000 )
		return 1;
	else if( dwBadFame < 200000 )
		return 2;
	else if( dwBadFame < 500000 )
		return 3;
	else if( dwBadFame < 1000000 )
		return 4;
	else
		return 5;
#else
	if( dwBadFame < 100000000 )
		return 1;
	else if( dwBadFame < 400000000 )
		return 2;
	else if( dwBadFame < 700000000 )
		return 3;
	else if( dwBadFame < 1000000000 )
		return 4;
	else
		return 5;
#endif
#else
	if( dwBadFame < 100000000 )
		return 1;
	else if( dwBadFame < 400000000 )
		return 2;
	else if( dwBadFame < 700000000 )
		return 3;
	else if( dwBadFame < 1000000000 )
		return 4;
	else
		return 5;
#endif
}

/*
void CPKManager::RemoveAllLootingDlg()
{
	m_htLootingDlg.SetPositionHead();

	CPKLootingDialog* pLootDlg = NULL;
	while( pLootDlg = m_htLootingDlg.GetData() )
	{
		WINDOWMGR->AddListDestroyWindow( pLootDlg );	//delete�?매니져가 해준�?
		
	}
}
*/
