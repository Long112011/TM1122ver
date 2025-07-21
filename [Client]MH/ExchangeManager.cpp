







#include "stdafx.h"
#include "ExchangeManager.h"
#include "Item.h"
#include "GameIn.h"
#include "ExchangeItem.h"
#include "./interface/cWindowManager.h"
#include "WindowIdEnum.h"
#include "cMsgBox.h"
#include "ChatManager.h"
#include "ObjectManager.h"
#include "ItemManager.h"
#include "MoveManager.h"
#include "VimuManager.h"

#include "ExchangeDialog.h"
#include "InventoryExDialog.h"
#include "MainBarDialog.h"
#include "ObjectStateManager.h"
#include "QuickManager.h"
#include "PetManager.h"
#include "TitanManager.h"

#include "Battle.h"
#include "BattleSystem_Client.h"

#include "MHMap.h"

GLOBALTON(CExchangeManager)

CExchangeManager::CExchangeManager()
{
	m_IconIndexCreator.Init(MAX_ITEMICON_NUM, IG_EXCHANGEITEM_STARTINDEX);
	
	m_nLinkedItemNum	= 0;
	m_bLockSyncing		= FALSE;
	m_bExchangeSyncing	= FALSE;
	m_bInventoryActive	= FALSE;
//	m_nExchangeState	= eEXSTT_NONE;
}

CExchangeManager::~CExchangeManager()
{
	m_IconIndexCreator.Release();
}

void CExchangeManager::Init()	//exchangedialog¿¡¼­ active false ÀÏ¶§ È£ÃâÇÑ´Ù. CONFIRM
{
	m_nLinkedItemNum	= 0;
	m_bLockSyncing		= FALSE;
	m_bExchangeSyncing	= FALSE;
	m_IsExchanging		= FALSE;
}

void CExchangeManager::ExchangeStart()
{
//---±³È¯½ÅÃ»»óÅÂ·Î ¸¸µé±â
	HERO->DisableAutoAttack();					//ÀÚµ¿ °ø°İ Ãë¼Ò
	HERO->SetNextAction(NULL);					//½ºÅ³ Ãë¼Ò
	MOVEMGR->HeroMoveStop();					//Á¦ÀÚ¸® ¸ØÃã ¸Ş¼¼Áö

	OBJECTSTATEMGR->StartObjectState(HERO, eObjectState_Exchange);

	m_IsExchanging = TRUE;
//------------------------
}

void CExchangeManager::ExchangeEnd()
{
	if( HERO->GetState() == eObjectState_Exchange )
		OBJECTSTATEMGR->EndObjectState(HERO, eObjectState_Exchange);

	m_IsExchanging = FALSE;
}

BOOL CExchangeManager::ApplyExchange()
{
#ifdef _TESTCLIENT_
	return FALSE;
#endif
//---±³È¯½ÅÃ» °¡´É»óÅÂÀÎÁö Ã¼Å©
	if( m_IsExchanging  )
	{
		//"ÀÌ¹Ì ±³È¯ ½ÅÃ»Áß ÀÔ´Ï´Ù."
		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(335) );
		return FALSE;
	}

	if( HERO->GetState() != eObjectState_None && HERO->GetState() != eObjectState_Move &&
		HERO->GetState() != eObjectState_Immortal )
	{
		//"±³È¯ ÇÒ ¼ö ¾ø´Â »óÅÂÀÔ´Ï´Ù."
		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(336) );
		return FALSE;
	}

	if( VIMUMGR->IsVimuing() )
	{
		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(336) );
		return FALSE;
	}

	if( HERO->InTitan() )
	{
		TITANMGR->CheckNotifyMsg(TNM_CANNOT_DO);
		return FALSE;
	}

	//SW061019 ¼­¹ÙÀÌ¹ú¸ğµå °ü·Ã
	//if( MAP->CheckMapKindIs(eSurvival) )
	if( MAP->IsMapKind(eSurvivalMap) )
	{
		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(336) );
		return FALSE;
	}

	CObject* pOpPlayer = OBJECTMGR->GetSelectedObject();
	if( pOpPlayer == NULL ) 
	{
		//"±³È¯À» ÇÏ½Ã·Á¸é ¸ÕÀú ¿øÇÏ´Â »ó´ë¸¦ ¼±ÅÃÇØ ÁÖ¼¼¿ä."
		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(338) );
		return FALSE;
	}

	if( pOpPlayer->GetObjectKind() != eObjectKind_Player || pOpPlayer == HERO ) 
	{
		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(338) );
		return FALSE;
	}

	////////////////////////////////////////////////////////////////////////////////
	// 06. 06. 2Â÷ ÀüÁ÷ - ÀÌ¿µÁØ
	// Àº½Å / Çı¾È
	// Àº½Å ÆĞ³ÎÆ¼
	if( HERO->GetSingleSpecialState(eSingleSpecialState_Hide) )
	{
		CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(1329));
		return FALSE;
	}
	if( pOpPlayer->GetSingleSpecialState(eSingleSpecialState_Hide) )

	{
		CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(1329));
		return FALSE;
	}
	////////////////////////////////////////////////////////////////////////////////

	// Guild Tournament
	CBattle* pBattle = BATTLESYSTEM->GetBattle( HERO );

	if( pBattle && pBattle->GetBattleKind() == eBATTLE_KIND_GTOURNAMENT )
	if( HERO->GetBattleTeam() == 2 )
		return FALSE;

//------------------------------	
	
	ExchangeStart();

//---»ó´ë¿¡°Ô ±³È¯½ÅÃ» ¸Ş¼¼Áö º¸³»±â
	MSG_DWORD msgApply;
	msgApply.Category	= MP_EXCHANGE;
	msgApply.Protocol	= MP_EXCHANGE_APPLY_SYN;
	msgApply.dwObjectID	= HERO->GetID();
	msgApply.dwData		= pOpPlayer->GetID();

	NETWORK->Send( &msgApply, sizeof( msgApply ) );
//-----------------------------------

	return TRUE;
}

BOOL CExchangeManager::CanAcceptExchange( CPlayer* pAccepter )
{
	if( pAccepter == NULL )					return FALSE;
	if( VIMUMGR->IsVimuing() )				return FALSE;

	if( pAccepter->GetState() != eObjectState_None && pAccepter->GetState() != eObjectState_Move
		&& pAccepter->GetState() != eObjectState_Immortal )
		return FALSE;
	
	return TRUE;	
}

void CExchangeManager::AcceptExchange( BOOL bAccept )
{
	if( bAccept )
	{
		if( CanAcceptExchange( HERO ) )
		{
			ExchangeStart();	//nack¿¡¼­ ÇØÁ¦È®ÀÎ
			
			MSGBASE msgAccept;
			msgAccept.Category		= MP_EXCHANGE;
			msgAccept.dwObjectID	= HERO->GetID();

			msgAccept.Protocol		= MP_EXCHANGE_ACCEPT_SYN;
			NETWORK->Send( &msgAccept, sizeof( msgAccept ) );

			CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(334) );//±³È¯À» ¼ö¶ôÇÏ¼Ì½À´Ï´Ù.

		}
		else
		{
			MSGBASE msgError;		//Â÷ÈÄ ¿¡·¯Ã¼Å©¸¦ Ãß°¡ÇÏÀÚ.
			msgError.Category		= MP_EXCHANGE;
			msgError.dwObjectID		= HERO->GetID();
			msgError.Protocol		= MP_EXCHANGE_CANTAPPLY;
			NETWORK->Send( &msgError, sizeof(MSGBASE) );

			m_IsExchanging			 = FALSE;

			CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(336) );//±³È¯ÇÒ ¼ö ¾ø´Â»óÅÂÀÔ´Ï´Ù.
		}
	}
	else
	{
		MSGBASE msgAccept;
		msgAccept.Category		= MP_EXCHANGE;
		msgAccept.dwObjectID	= HERO->GetID();
		msgAccept.Protocol		= MP_EXCHANGE_REJECT_SYN;
		NETWORK->Send( &msgAccept, sizeof( msgAccept ) );

		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(327) );

		ExchangeEnd();
	}	
}

void CExchangeManager::CancelApply()
{
	MSGBASE msg;

	msg.Category	= MP_EXCHANGE;
	msg.Protocol	= MP_EXCHANGE_WAITING_CANCEL_SYN;
	msg.dwObjectID	= HERO->GetID();

	NETWORK->Send( &msg, sizeof(MSGBASE) );
}

BOOL CExchangeManager::LinkItem( CItem* pItem )
{
	if( pItem->GetType() != WT_ITEM ) return FALSE;		//ÀÎº¥Åä¸® ¾ÆÀÌÅÛÀÌ ¾Æ´Ï¸é return
	//ÀåÂøµÇ¾î ÀÖ´Â ¾ÆÀÌÅÛÀº....¾ÈµÇ´Â°Í Ãß°¡..		//confirm

	if( pItem->GetLinkItem() != NULL ) return FALSE;		//ÀÌ¹Ì ¸µÅ©µÈ ¾ÆÀÌÅÛÀÌ¸é return
	//¸µÅ©°¡ µÇ¾ú´ÂÁö ¾ÈµÇ¾ú´ÂÁö¸¸ ¾Ë¸é µÇÁö ¾ÊÀ»±î?

	if( m_nLinkedItemNum >= MAX_EXCHANGEITEM ) return FALSE;

	++m_nLinkedItemNum;
	
	pItem->SetLock( TRUE );


	MSG_LINKITEM msg;

	msg.Category	= MP_EXCHANGE;
	msg.Protocol	= MP_EXCHANGE_ADDITEM_SYN;

	msg.dwObjectID	= HERO->GetID();
	msg.ItemInfo	= *pItem->GetItemBaseInfo();
	msg.wAbsPosition = m_nLinkedItemNum;

	NETWORK->Send( &msg, sizeof( MSG_LINKITEM ) );

	return TRUE;
}

void CExchangeManager::UnLinkItem( int nTypeIndex, cIcon* pItem )
{
	if( nTypeIndex == 0 )
	{
		((CExchangeItem*)pItem)->GetLinkItem()->SetLinkPosition( 0 );
		((CExchangeItem*)pItem)->GetLinkItem()->SetLinkItem( NULL );
		((CExchangeItem*)pItem)->GetLinkItem()->SetLock( FALSE );		
	}

	m_IconIndexCreator.ReleaseIndex( pItem->GetID() );
	WINDOWMGR->AddListDestroyWindow( pItem );
}

void CExchangeManager::Lock( BOOL bLock )
{
	MSGBASE msg;
	msg.Category	= MP_EXCHANGE;
	msg.dwObjectID	= HERO->GetID();

	if( bLock )
	{
		msg.Protocol = MP_EXCHANGE_LOCK_SYN;
	}
	else
	{
		msg.Protocol = MP_EXCHANGE_UNLOCK_SYN;
	}

	NETWORK->Send( &msg, sizeof( MSGBASE ) );

	m_bLockSyncing = TRUE;
}


void CExchangeManager::Exchange()
{
	MSGBASE msg;
	msg.Category	= MP_EXCHANGE;
	msg.dwObjectID	= HERO->GetID();
	msg.Protocol	= MP_EXCHANGE_EXCHANGE_SYN;

	NETWORK->Send( &msg, sizeof( MSGBASE ) );
	
	//KES 040801 ±³È¯Áß¿¡ ÀÎº¥Åä¸® »ç¿ë ¸·±â
	ITEMMGR->SetDisableDialog(TRUE, eItemTable_Inventory);

	m_bExchangeSyncing = TRUE;
}


void CExchangeManager::NetworkMsgParse( BYTE Protocol, void* pMsg )
{
	switch(Protocol)
	{
	case MP_EXCHANGE_APPLY:	//»ó´ë°¡ ³ª¿¡°Ô ½ÅÃ»À» ÇØ¿Ô´Ù
		{
			MSG_DWORD* msg = (MSG_DWORD*)pMsg;
			CPlayer* pPlayer = (CPlayer*)OBJECTMGR->GetObject( msg->dwData );
			
			ASSERT(pPlayer->GetObjectKind() == eObjectKind_Player);

			if( pPlayer == NULL || m_IsExchanging == TRUE )
			{
				MSGBASE msgReject;
				msgReject.Category		= MP_EXCHANGE;
				msgReject.dwObjectID	= HERO->GetID();
				msgReject.Protocol		= MP_EXCHANGE_CANTAPPLY;
				NETWORK->Send( &msgReject, sizeof(MSGBASE) );


				//¿¡·¯¸Ş¼¼Áö
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(336) );	//±³È¯ÇÒ ¼ö ¾ø´Â»óÅÂÀÔ´Ï´Ù.
				break;
			}

			WINDOWMGR->MsgBox( MBI_EXCHANGEAPPLY, MBT_YESNO,
						CHATMGR->GetChatMsg(321), pPlayer->GetObjectName() );

			if( GAMEIN->GetExchangeDialog() )
				GAMEIN->GetExchangeDialog()->SetPlayerName( HERO->GetObjectName(), pPlayer->GetObjectName() );

			m_IsExchanging = TRUE;
		}
		break;

	case MP_EXCHANGE_APPLY_ACK:
		{
			MSG_DWORD* msg = (MSG_DWORD*)pMsg;
			CPlayer* pPlayer = (CPlayer*)OBJECTMGR->GetObject( msg->dwData );
			if( pPlayer == NULL )	//CHECK ±³È¯»ó´ë¸¦ Ã£À» ¼ö ¾ø´Ù?
			{
				//±³È¯ ½ÅÃ»À» ¼öÇàÇÏÁö ¸øÇÏ¿´½À´Ï´Ù.
				ExchangeEnd();
				break;
			}
			ASSERT(pPlayer->GetObjectKind() == eObjectKind_Player);

			CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(322), pPlayer->GetObjectName() );

			cMsgBox* pMsgBox = WINDOWMGR->MsgBox( MBI_EXCHANGEWAITING, MBT_CANCEL, CHATMGR->GetChatMsg(337) );
			if( pMsgBox )
				pMsgBox->SetDefaultBtn(-1);	//µğÆúÆ® ¹öÆ° ¾ø´Ù.
		}
		break;

	case MP_EXCHANGE_APPLY_NACK:
		{			
			CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(323) );
			ExchangeEnd();
		}
		break;

	case MP_EXCHANGE_ACCEPT_NACK:
		{
			MSGBASE* msg = (MSGBASE*)pMsg;

			CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(326) );			
			ExchangeEnd();
		}
		break;

	case MP_EXCHANGE_ACCEPT:
		{
			MSG_DWORD* msg = (MSG_DWORD*)pMsg;
			CPlayer* pPlayer = (CPlayer*)OBJECTMGR->GetObject( msg->dwData );
			if( pPlayer == NULL ) return;

			ASSERT(pPlayer->GetObjectKind() == eObjectKind_Player);

			CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(324), pPlayer->GetObjectName() );

			if( GAMEIN->GetExchangeDialog() )
				GAMEIN->GetExchangeDialog()->SetPlayerName( HERO->GetObjectName(), pPlayer->GetObjectName() );

			cDialog* pDlg = WINDOWMGR->GetWindowForID( MBI_EXCHANGEWAITING );
			if( pDlg )
				WINDOWMGR->AddListDestroyWindow( pDlg );
		}
		break;

	case MP_EXCHANGE_REJECT:
		{
			MSGBASE* msg = (MSGBASE*)pMsg;

			ExchangeEnd();
			
			cDialog* pDlg = WINDOWMGR->GetWindowForID( MBI_EXCHANGEWAITING );
			if( pDlg )
				WINDOWMGR->AddListDestroyWindow( pDlg );

			CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(326) );
		}
		break;

	case MP_EXCHANGE_CANTAPPLY:
		{
			MSGBASE* msg = (MSGBASE*)pMsg;

			ExchangeEnd();

			cDialog* pDlg = WINDOWMGR->GetWindowForID( MBI_EXCHANGEWAITING );
			if( pDlg )
				WINDOWMGR->AddListDestroyWindow( pDlg );

			CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(323) );
			//315 °Å¸®°¡ ³Ê¹« ¸Ö¾î ±³È¯ÇÒ ¼ö ¾ø½À´Ï´Ù.
		}
		break;

	case MP_EXCHANGE_WAITING_CANCEL:
		{
			ExchangeEnd();
			
			cDialog* pDlg = WINDOWMGR->GetWindowForID( MBI_EXCHANGEAPPLY );
			if( pDlg )
				WINDOWMGR->AddListDestroyWindow( pDlg );
			
			CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(326) );
		}
		break;

	case MP_EXCHANGE_WAITING_CANCEL_ACK:
		{
			ExchangeEnd();


			CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(329) );
		}
		break;

	case MP_EXCHANGE_WAITING_CANCEL_NACK:
		{
			CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(316) );
		}
		break;	

	case MP_EXCHANGE_START:
		{
			GAMEIN->GetExchangeDialog()->SetActive( TRUE );

			if( !GAMEIN->GetInventoryDialog()->IsActive() )
			{
				GAMEIN->GetInventoryDialog()->SetActive( TRUE );
				m_bInventoryActive = FALSE;
			}
			else
			{
				m_bInventoryActive = TRUE;
			}
		}
		break;

	case MP_EXCHANGE_ADDITEM_ACK:
		{
			MSG_LINKITEM* msg = (MSG_LINKITEM*)pMsg;

			GAMEIN->GetExchangeDialog()->SetDisable( FALSE );		//FAKE MOVE¿¡¼­ TRUE
			CItem* pItem = (CItem*)GAMEIN->GetInventoryDialog()->GetItemForPos( msg->ItemInfo.Position );
			ASSERT( pItem );
			if( !pItem ) break;

			CExchangeItem* pExItem = new CExchangeItem;
								
			//weiye ½»Ò×·µ»ØÎïÆ·Ìí¼Ó£¬×¢Òâ´Ë´¦Ìí¼Ó³É³¤ 2018-04-17

			pExItem->SetDBId(msg->ItemInfo.dwDBIdx);
			pExItem->SetType( WT_EXCHANGEITEM );  //weiye ÏÈÉèÖÃÀàĞÍ  ÒòÎª³õÊ¼»¯³É³¤ÖµĞèÒª  ·ñÔòÄÚ´æÒç³ö 2018-04-16
			pExItem->SetGrow(msg->ItemInfo.ItemGrow);       // 2015-01-16 µ½
			pExItem->InitItem( pItem, msg->wAbsPosition, m_IconIndexCreator.GenerateIndex() );
			pExItem->SetDurability( msg->ItemInfo.Durability );
			pExItem->SetRareness( msg->ItemInfo.RareIdx );
			pExItem->SetStoneIdx(msg->ItemInfo.StoneIdx);   // 2014-12-15 ¸½µ½
			pExItem->SetGrade(msg->ItemInfo.Grade30);
			pExItem->SetQuality(msg->ItemInfo.ItemQuality);
			pExItem->SetEntry1(msg->ItemInfo.ItemEntry1);
			pExItem->SetEntry2(msg->ItemInfo.ItemEntry2);
			pExItem->SetEntry3(msg->ItemInfo.ItemEntry3);
			WINDOWMGR->AddWindow( pExItem );

			GAMEIN->GetExchangeDialog()->AddItem( 0, msg->wAbsPosition, pExItem );

			//ÀÎº¥Åä¸® ¾ÆÀÌÅÛ ¼¼ÆÃ
			pItem->SetLinkPosition( m_nLinkedItemNum );
			pItem->SetLinkItem( pExItem );

			//ÀÎº¥Åä¸® Ã¼Å©
			int nBlankNum = GAMEIN->GetInventoryDialog()->GetBlankNum();

			if( nBlankNum < (int)msg->wAbsPosition - EXCHANGEMGR->GetLinkedItemNum() )
			{
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(354) );
		//		m_bExchangable = FALSE;
			}
		}
		break;

	case MP_EXCHANGE_ADDITEM_NACK:
		{
			MSG_LINKITEM* msg = (MSG_LINKITEM*)pMsg;
			GAMEIN->GetExchangeDialog()->SetDisable( FALSE );		//FAKE MOVE¿¡¼­ TRUE

			--m_nLinkedItemNum;

			CItem* pItem = GAMEIN->GetInventoryDialog()->GetItemForPos( msg->ItemInfo.Position );
			if( pItem )
				pItem->SetLock( FALSE );			
		}
		break;

	case MP_EXCHANGE_ADDITEM:	//»ó´ë°¡ µî·ÏÇß´Ù.
		{
//			MSG_LINKITEM* msg = (MSG_LINKITEM*)pMsg;
			//MSG_LINKITEM_ADD* msg = (MSG_LINKITEM_ADD*)pMsg;
			MSG_LINKITEMOPTIONS* msg = (MSG_LINKITEMOPTIONS*)pMsg;

			CExchangeItem* pItem = new CExchangeItem;
			cImage tmpImage;
			pItem->Init( 0, 0, DEFAULT_ICONSIZE, DEFAULT_ICONSIZE, 
						 ITEMMGR->GetIconImage( msg->ItemInfo.wIconIdx, &tmpImage ), m_IconIndexCreator.GenerateIndex() );

			pItem->SetType( WT_EXCHANGEITEM );
			pItem->SetDurability( msg->ItemInfo.Durability );
			pItem->SetRareness( msg->ItemInfo.RareIdx );
			pItem->SetMovable( FALSE );
			pItem->SetData( msg->ItemInfo.wIconIdx );
			pItem->SetItemParam( msg->ItemInfo.ItemParam );
			pItem->SetDBId( msg->ItemInfo.dwDBIdx );
			pItem->SetStoneIdx(msg->ItemInfo.StoneIdx);   // 2014-12-15 µ½
			pItem->SetGrow(msg->ItemInfo.ItemGrow);
			pItem->SetGrade(msg->ItemInfo.Grade30);
			pItem->SetQuality(msg->ItemInfo.ItemQuality);
			pItem->SetEntry1(msg->ItemInfo.ItemEntry1);
			pItem->SetEntry2(msg->ItemInfo.ItemEntry2);
			pItem->SetEntry3(msg->ItemInfo.ItemEntry3);

			ITEMMGR->GetItemOptionsAndToolTipFromInfoMsg(pItem, msg);

			WINDOWMGR->AddWindow( pItem );

			GAMEIN->GetExchangeDialog()->AddItem( 1, msg->wAbsPosition, pItem );
		}
		break;
		
	case MP_EXCHANGE_DELITEM_ACK:
		{
			MSG_WORD* msg = (MSG_WORD*)pMsg;
			GAMEIN->GetExchangeDialog()->SetDisable( FALSE );
			GAMEIN->GetExchangeDialog()->DelItem( 0, msg->wData );
			--m_nLinkedItemNum;
		}
		break;

	case MP_EXCHANGE_DELITEM_NACK:
		{

			MSG_WORD* msg = (MSG_WORD*)pMsg;
			GAMEIN->GetExchangeDialog()->SetDisable( FALSE );
		}
		break;

	case MP_EXCHANGE_DELITEM://»ó´ë°¡ Áö¿ü´Ù.
		{

			MSG_WORD* msg = (MSG_WORD*)pMsg;

			//¿É¼ÇÀÌ ÀÖÀ¸¸é Áö¿öÁØ´Ù.
			GAMEIN->GetExchangeDialog()->DelItemOptionInfo( msg->wData );
			GAMEIN->GetExchangeDialog()->DelItemRareOptionInfo( msg->wData );
			GAMEIN->GetExchangeDialog()->DelPetItemInfo( msg->wData );
			GAMEIN->GetExchangeDialog()->DelItemStoneOptionInfo(msg->wData); // 2014-12-15 !
			GAMEIN->GetExchangeDialog()->DelItem( 1, msg->wData );			
			
		}
		break;
		
	case MP_EXCHANGE_INPUTMONEY_ACK:
		{
			MSG_DWORD* msg = (MSG_DWORD*)pMsg;
			DWORD dwMoney = GAMEIN->GetExchangeDialog()->InputMoney( 0, msg->dwData );
			GAMEIN->GetExchangeDialog()->SetDisable( FALSE );
		}
		break;

	case MP_EXCHANGE_INPUTGOLD_ACK: // 2014-10-29 ÊäÈë·şÎñ¶Ë·µ»Øºó
		{
			MSG_DWORD* msg = (MSG_DWORD*)pMsg;
			DWORD dwGold = GAMEIN->GetExchangeDialog()->InputGold( 0, msg->dwData );
			GAMEIN->GetExchangeDialog()->SetDisable( FALSE );
		}
		break;

	case MP_EXCHANGE_INPUTMONEY_NACK:
		{
			//"µ·ÀÔ·Â¿¡ ½ÇÆĞÇÏ¿´½À´Ï´Ù."
			GAMEIN->GetExchangeDialog()->SetDisable( FALSE );
		}
		break;
	case MP_EXCHANGE_INPUTGOLD_NACK: // 2014-10-29 ÊäÈë·şÎñ¶Ë´íÎó·µ»Ø
		{
			GAMEIN->GetExchangeDialog()->SetDisable( FALSE );
		}
		break;

	case MP_EXCHANGE_INPUTMONEY:
		{

			MSG_DWORD* msg = (MSG_DWORD*)pMsg;
			GAMEIN->GetExchangeDialog()->InputMoney( 1, msg->dwData );
		}
		break;

	case MP_EXCHANGE_INPUTGOLD:     // 2014-10-29 ÊäÈë·şÎñ¶Ë·µ»Ø
		{
			MSG_DWORD* msg = (MSG_DWORD*)pMsg;
			GAMEIN->GetExchangeDialog()->InputGold( 1, msg->dwData );
		}
		break;

	case MP_EXCHANGE_LOCK_ACK:
		{
			m_bLockSyncing = FALSE;
			GAMEIN->GetExchangeDialog()->MyLock( TRUE );	//ÀÌ¹Ì ÇßÀ¸³ª ÇÑ¹ø´õ È®ÀÎ
		}
		break;


	case MP_EXCHANGE_LOCK_NACK:
		{
			m_bLockSyncing = FALSE;
			GAMEIN->GetExchangeDialog()->MyLock( FALSE );
		}
		break;

	case MP_EXCHANGE_LOCK:
		{
			GAMEIN->GetExchangeDialog()->OpLock( TRUE );			
		}
		break;

	case MP_EXCHANGE_UNLOCK_ACK:
		{
			m_bLockSyncing = FALSE;			
			GAMEIN->GetExchangeDialog()->MyLock( FALSE );
		}
		break;

	case MP_EXCHANGE_UNLOCK_NACK:
		{
			m_bLockSyncing = FALSE;
			GAMEIN->GetExchangeDialog()->MyLock( TRUE );
		}
		break;

	case MP_EXCHANGE_UNLOCK:	//»ó´ë°¡ ¾ğ¶ôÇßÀ»½Ã¿¡.
		{
			GAMEIN->GetExchangeDialog()->OpLock( FALSE );
			CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(332) );

			if( m_bExchangeSyncing )//±³È¯ÁßÀÌ¾ú´Ù¸é 
			{
				m_bExchangeSyncing = FALSE;
				//ÀÎº£Åä¸®µğ½º¿¡ÀÌºí Ç®±â
				GAMEIN->GetInventoryDialog()->SetDisable( FALSE );				
			}			
		}
		break;

	case MP_EXCHANGE_EXCHANGE_ACK:
		{
			//µÑ´Ù exchange¸¦ ´­·¶´Ù. ±³È¯À» ½ÃÀÛÇÑ´Ù!
			//¿©±â¼­ ¿É¼ÇÀ» ´ÙÁö¿ì°í...
		}
		break;

	case MP_EXCHANGE_EXCHANGE:
		{
			GAMEIN->GetExchangeDialog()->SetDisable( FALSE );
			GAMEIN->GetExchangeDialog()->SetActive( FALSE );			
			ITEMMGR->SetDisableDialog( FALSE, eItemTable_Inventory );


			//µ·ÀÔ·Â¹öÆ° Áö¿ì±â.	CONFIRM
			cDialog* pDlg = WINDOWMGR->GetWindowForID( DBOX_EXCHANGE_MONEY );
			if( pDlg )
				WINDOWMGR->AddListDestroyWindow( pDlg );


			CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(325) );

			ExchangeEnd();
		}
		break;

	case MP_EXCHANGE_EXCHANGE_NACK:		//±³È¯¹öÆ°À» ´­·¶´Â´ë »ó´ë°¡ ¶ôÀ»Ç®¾î ¶ôÀÌ ÇØÁ¦µÇ¾úÀ»¶§ NACK´Ù.
		{
//			GAMEIN->GetExchangeDialog()->SetDisable( FALSE );
//			GAMEIN->GetExchangeDialog()->SetActive( FALSE );
//			GAMEIN->GetInventoryDialog()->SetDisable( FALSE );
//			if( !m_bInventoryActive )
//				GAMEIN->GetInventoryDialog()->SetActive( FALSE );

			//µ·ÀÔ·Â¹öÆ° Áö¿ì±â.	CONFIRM
//			cDialog* pDlg = WINDOWMGR->GetWindowForID( DBOX_EXCHANGE_MONEY );
//			if( pDlg )
//				WINDOWMGR->AddListDestroyWindow( pDlg );


//			CHATMGR->AddMsg( CTC_SYSMSG, "¶ôÀÌ °É¸®Áö ¾ÊÀº »óÅÂ¶ó ±³È¯ÀÌ Ãë¼ÒµÇ¾ú½À´Ï´Ù." );
//			ExchangeEnd();

			ITEMMGR->SetDisableDialog( FALSE, eItemTable_Inventory );
		}
		break;

	case MP_EXCHANGE_CANCEL_ACK:
		{
			////////¾ÆÀÌÅÛ ¿É¼Ç Á¦°Å Active False Àü¿¡ ÇÒ°Í
			GAMEIN->GetExchangeDialog()->DelItemOptionInfoAll();
			GAMEIN->GetExchangeDialog()->DelItemRareOptionInfoAll();
			GAMEIN->GetExchangeDialog()->DelPetItemInfoAll();
			GAMEIN->GetExchangeDialog()->DelItemStoneOptionAll();// 2014-12-15 !

			GAMEIN->GetExchangeDialog()->SetDisable( FALSE );
			GAMEIN->GetExchangeDialog()->SetActive( FALSE );

			if( !m_bInventoryActive )
			{
				ITEMMGR->SetDisableDialog( FALSE, eItemTable_Inventory );
				GAMEIN->GetInventoryDialog()->SetActive( FALSE );
			}

			cDialog* pDlg = WINDOWMGR->GetWindowForID( DBOX_EXCHANGE_MONEY );
			if( pDlg )
				WINDOWMGR->AddListDestroyWindow( pDlg );

			CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(329) );

			ExchangeEnd();
		}
		break;
	
	case MP_EXCHANGE_CANCEL_NACK:
		{
			GAMEIN->GetExchangeDialog()->SetDisable( FALSE );
		}
		break;

	case MP_EXCHANGE_CANCEL:
		{
			MSG_BYTE* msg = (MSG_BYTE*)pMsg;

			GAMEIN->GetExchangeDialog()->SetDisable( FALSE );	//Ã¼Å©..ÀÓ½Ã.

			GAMEIN->GetInventoryDialog()->SetDisable( FALSE );

			//µ·ÀÔ·Â¹öÆ° Áö¿ì±â.	CONFIRM
			cDialog* pDlg = WINDOWMGR->GetWindowForID( DBOX_EXCHANGE_MONEY );
			if( pDlg )
				WINDOWMGR->AddListDestroyWindow( pDlg );
			

			////////¾ÆÀÌÅÛ ¿É¼Ç Á¦°Å : Active False Àü¿¡ ÇÒ°Í.
			GAMEIN->GetExchangeDialog()->DelItemOptionInfoAll();
			GAMEIN->GetExchangeDialog()->DelItemRareOptionInfoAll();
			GAMEIN->GetExchangeDialog()->DelItemStoneOptionAll();// 2014-12-15 !

			//±³È¯ÀÌ Ãë¼ÒµÇ¾ú¶§¿Í ±³È¯Çã¶ôÀ» ÇÏÁö ¾Ê¾ÒÀ»½Ã...
			GAMEIN->GetExchangeDialog()->SetActive( FALSE );
			if( !m_bInventoryActive )
				GAMEIN->GetInventoryDialog()->SetActive( FALSE );
			
			//½ÅÃ»¸Ş¼¼Áöµé Áö¿ì±â....
			
			pDlg = WINDOWMGR->GetWindowForID( MBI_EXCHANGEAPPLY);
			if( pDlg )
				WINDOWMGR->AddListDestroyWindow( pDlg );

			pDlg = WINDOWMGR->GetWindowForID( MBI_EXCHANGEWAITING );
			if( pDlg )
				WINDOWMGR->AddListDestroyWindow( pDlg );			

//error msg Ãâ·Â
			switch( msg->bData )
			{
				case eEE_USERCANCEL:
					CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(326) );

					break;
				case eEE_USERLOGOUT:
					CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(314) );
					break;
				case eEE_USERDIE:
					CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(313) );
					break;
				case eEE_DIE:
					CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(312) );
					break;
				case eEE_NOTENOUGHMONEY:
					CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(311) );
					break;
				case eEE_NOTENOUGHSPACE:
					CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(310) );
					break;
				case eEE_MAXMONEY:
					CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(309) );
					break;
				default:
					break;
			}

			ExchangeEnd();
		}
		break;

	case MP_EXCHANGE_SETMONEY:
		{
			MSG_DWORD* msg = (MSG_DWORD*)pMsg;
			HERO->SetMoney( msg->dwData );	//+ÀÎº¥Åä¸®
		}
		break;
		
	case MP_EXCHANGE_INSERT:
		{
			MSG_ITEM* msg = (MSG_ITEM*)pMsg;
			BOOL rt = GAMEIN->GetInventoryDialog()->AddItem(&msg->ItemInfo);
			ASSERT(rt);
			//¾ÆÀÌÅÛÀ» ³ÖÀ»¶§ ´Ù½Ã ¿É¼ÇÀ» ³Ö´Â´Ù.?

			QUICKMGR->RefreshQickItem();
		}
		break;

	case MP_EXCHANGE_REMOVE:
		{
			MSG_EXCHANGE_REMOVEITEM* msg = (MSG_EXCHANGE_REMOVEITEM*)pMsg;
			CItem* pOutItem = NULL;
			ITEMMGR->DeleteItem( msg->wAbsPosition, &pOutItem );

			//¿É¼ÇÀ» °¡Áø ¾ÆÀÌÅÛÀÌ¾ú´Ù¸é ¿É¼ÇÀÌ Áö¿öÁø´Ù. ÀÚµ¿À¸·Î.
		}
		break;
	}
}





