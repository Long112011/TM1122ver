







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

void CExchangeManager::Init()	//exchangedialog���� active false �϶� ȣ���Ѵ�. CONFIRM
{
	m_nLinkedItemNum	= 0;
	m_bLockSyncing		= FALSE;
	m_bExchangeSyncing	= FALSE;
	m_IsExchanging		= FALSE;
}

void CExchangeManager::ExchangeStart()
{
//---��ȯ��û���·� �����
	HERO->DisableAutoAttack();					//�ڵ� ���� ���
	HERO->SetNextAction(NULL);					//��ų ���
	MOVEMGR->HeroMoveStop();					//���ڸ� ���� �޼���

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
//---��ȯ��û ���ɻ������� üũ
	if( m_IsExchanging  )
	{
		//"�̹� ��ȯ ��û�� �Դϴ�."
		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(335) );
		return FALSE;
	}

	if( HERO->GetState() != eObjectState_None && HERO->GetState() != eObjectState_Move &&
		HERO->GetState() != eObjectState_Immortal )
	{
		//"��ȯ �� �� ���� �����Դϴ�."
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

	//SW061019 �����̹���� ����
	//if( MAP->CheckMapKindIs(eSurvival) )
	if( MAP->IsMapKind(eSurvivalMap) )
	{
		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(336) );
		return FALSE;
	}

	CObject* pOpPlayer = OBJECTMGR->GetSelectedObject();
	if( pOpPlayer == NULL ) 
	{
		//"��ȯ�� �Ͻ÷��� ���� ���ϴ� ��븦 ������ �ּ���."
		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(338) );
		return FALSE;
	}

	if( pOpPlayer->GetObjectKind() != eObjectKind_Player || pOpPlayer == HERO ) 
	{
		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(338) );
		return FALSE;
	}

	////////////////////////////////////////////////////////////////////////////////
	// 06. 06. 2�� ���� - �̿���
	// ���� / ����
	// ���� �г�Ƽ
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

//---��뿡�� ��ȯ��û �޼��� ������
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
			ExchangeStart();	//nack���� ����Ȯ��
			
			MSGBASE msgAccept;
			msgAccept.Category		= MP_EXCHANGE;
			msgAccept.dwObjectID	= HERO->GetID();

			msgAccept.Protocol		= MP_EXCHANGE_ACCEPT_SYN;
			NETWORK->Send( &msgAccept, sizeof( msgAccept ) );

			CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(334) );//��ȯ�� �����ϼ̽��ϴ�.

		}
		else
		{
			MSGBASE msgError;		//���� ����üũ�� �߰�����.
			msgError.Category		= MP_EXCHANGE;
			msgError.dwObjectID		= HERO->GetID();
			msgError.Protocol		= MP_EXCHANGE_CANTAPPLY;
			NETWORK->Send( &msgError, sizeof(MSGBASE) );

			m_IsExchanging			 = FALSE;

			CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(336) );//��ȯ�� �� ���»����Դϴ�.
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
	if( pItem->GetType() != WT_ITEM ) return FALSE;		//�κ��丮 �������� �ƴϸ� return
	//�����Ǿ� �ִ� ��������....�ȵǴ°� �߰�..		//confirm

	if( pItem->GetLinkItem() != NULL ) return FALSE;		//�̹� ��ũ�� �������̸� return
	//��ũ�� �Ǿ����� �ȵǾ������� �˸� ���� ������?

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
	
	//KES 040801 ��ȯ�߿� �κ��丮 ��� ����
	ITEMMGR->SetDisableDialog(TRUE, eItemTable_Inventory);

	m_bExchangeSyncing = TRUE;
}


void CExchangeManager::NetworkMsgParse( BYTE Protocol, void* pMsg )
{
	switch(Protocol)
	{
	case MP_EXCHANGE_APPLY:	//��밡 ������ ��û�� �ؿԴ�
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


				//�����޼���
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(336) );	//��ȯ�� �� ���»����Դϴ�.
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
			if( pPlayer == NULL )	//CHECK ��ȯ��븦 ã�� �� ����?
			{
				//��ȯ ��û�� �������� ���Ͽ����ϴ�.
				ExchangeEnd();
				break;
			}
			ASSERT(pPlayer->GetObjectKind() == eObjectKind_Player);

			CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(322), pPlayer->GetObjectName() );

			cMsgBox* pMsgBox = WINDOWMGR->MsgBox( MBI_EXCHANGEWAITING, MBT_CANCEL, CHATMGR->GetChatMsg(337) );
			if( pMsgBox )
				pMsgBox->SetDefaultBtn(-1);	//����Ʈ ��ư ����.
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
			//315 �Ÿ��� �ʹ� �־� ��ȯ�� �� �����ϴ�.
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

			GAMEIN->GetExchangeDialog()->SetDisable( FALSE );		//FAKE MOVE���� TRUE
			CItem* pItem = (CItem*)GAMEIN->GetInventoryDialog()->GetItemForPos( msg->ItemInfo.Position );
			ASSERT( pItem );
			if( !pItem ) break;

			CExchangeItem* pExItem = new CExchangeItem;
								
			//weiye ���׷�����Ʒ��ӣ�ע��˴���ӳɳ� 2018-04-17

			pExItem->SetDBId(msg->ItemInfo.dwDBIdx);
			pExItem->SetType( WT_EXCHANGEITEM );  //weiye ����������  ��Ϊ��ʼ���ɳ�ֵ��Ҫ  �����ڴ���� 2018-04-16
			pExItem->SetGrow(msg->ItemInfo.ItemGrow);       // 2015-01-16 ��
			pExItem->InitItem( pItem, msg->wAbsPosition, m_IconIndexCreator.GenerateIndex() );
			pExItem->SetDurability( msg->ItemInfo.Durability );
			pExItem->SetRareness( msg->ItemInfo.RareIdx );
			pExItem->SetStoneIdx(msg->ItemInfo.StoneIdx);   // 2014-12-15 ����
			pExItem->SetGrade(msg->ItemInfo.Grade30);
			pExItem->SetQuality(msg->ItemInfo.ItemQuality);
			pExItem->SetEntry1(msg->ItemInfo.ItemEntry1);
			pExItem->SetEntry2(msg->ItemInfo.ItemEntry2);
			pExItem->SetEntry3(msg->ItemInfo.ItemEntry3);
			WINDOWMGR->AddWindow( pExItem );

			GAMEIN->GetExchangeDialog()->AddItem( 0, msg->wAbsPosition, pExItem );

			//�κ��丮 ������ ����
			pItem->SetLinkPosition( m_nLinkedItemNum );
			pItem->SetLinkItem( pExItem );

			//�κ��丮 üũ
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
			GAMEIN->GetExchangeDialog()->SetDisable( FALSE );		//FAKE MOVE���� TRUE

			--m_nLinkedItemNum;

			CItem* pItem = GAMEIN->GetInventoryDialog()->GetItemForPos( msg->ItemInfo.Position );
			if( pItem )
				pItem->SetLock( FALSE );			
		}
		break;

	case MP_EXCHANGE_ADDITEM:	//��밡 ����ߴ�.
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
			pItem->SetStoneIdx(msg->ItemInfo.StoneIdx);   // 2014-12-15 ��
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

	case MP_EXCHANGE_DELITEM://��밡 ������.
		{

			MSG_WORD* msg = (MSG_WORD*)pMsg;

			//�ɼ��� ������ �����ش�.
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

	case MP_EXCHANGE_INPUTGOLD_ACK: // 2014-10-29 �������˷��غ�
		{
			MSG_DWORD* msg = (MSG_DWORD*)pMsg;
			DWORD dwGold = GAMEIN->GetExchangeDialog()->InputGold( 0, msg->dwData );
			GAMEIN->GetExchangeDialog()->SetDisable( FALSE );
		}
		break;

	case MP_EXCHANGE_INPUTMONEY_NACK:
		{
			//"���Է¿� �����Ͽ����ϴ�."
			GAMEIN->GetExchangeDialog()->SetDisable( FALSE );
		}
		break;
	case MP_EXCHANGE_INPUTGOLD_NACK: // 2014-10-29 �������˴��󷵻�
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

	case MP_EXCHANGE_INPUTGOLD:     // 2014-10-29 �������˷���
		{
			MSG_DWORD* msg = (MSG_DWORD*)pMsg;
			GAMEIN->GetExchangeDialog()->InputGold( 1, msg->dwData );
		}
		break;

	case MP_EXCHANGE_LOCK_ACK:
		{
			m_bLockSyncing = FALSE;
			GAMEIN->GetExchangeDialog()->MyLock( TRUE );	//�̹� ������ �ѹ��� Ȯ��
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

	case MP_EXCHANGE_UNLOCK:	//��밡 ��������ÿ�.
		{
			GAMEIN->GetExchangeDialog()->OpLock( FALSE );
			CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(332) );

			if( m_bExchangeSyncing )//��ȯ���̾��ٸ� 
			{
				m_bExchangeSyncing = FALSE;
				//�κ��丮�𽺿��̺� Ǯ��
				GAMEIN->GetInventoryDialog()->SetDisable( FALSE );				
			}			
		}
		break;

	case MP_EXCHANGE_EXCHANGE_ACK:
		{
			//�Ѵ� exchange�� ������. ��ȯ�� �����Ѵ�!
			//���⼭ �ɼ��� �������...
		}
		break;

	case MP_EXCHANGE_EXCHANGE:
		{
			GAMEIN->GetExchangeDialog()->SetDisable( FALSE );
			GAMEIN->GetExchangeDialog()->SetActive( FALSE );			
			ITEMMGR->SetDisableDialog( FALSE, eItemTable_Inventory );


			//���Է¹�ư �����.	CONFIRM
			cDialog* pDlg = WINDOWMGR->GetWindowForID( DBOX_EXCHANGE_MONEY );
			if( pDlg )
				WINDOWMGR->AddListDestroyWindow( pDlg );


			CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(325) );

			ExchangeEnd();
		}
		break;

	case MP_EXCHANGE_EXCHANGE_NACK:		//��ȯ��ư�� �����´� ��밡 ����Ǯ�� ���� �����Ǿ����� NACK��.
		{
//			GAMEIN->GetExchangeDialog()->SetDisable( FALSE );
//			GAMEIN->GetExchangeDialog()->SetActive( FALSE );
//			GAMEIN->GetInventoryDialog()->SetDisable( FALSE );
//			if( !m_bInventoryActive )
//				GAMEIN->GetInventoryDialog()->SetActive( FALSE );

			//���Է¹�ư �����.	CONFIRM
//			cDialog* pDlg = WINDOWMGR->GetWindowForID( DBOX_EXCHANGE_MONEY );
//			if( pDlg )
//				WINDOWMGR->AddListDestroyWindow( pDlg );


//			CHATMGR->AddMsg( CTC_SYSMSG, "���� �ɸ��� ���� ���¶� ��ȯ�� ��ҵǾ����ϴ�." );
//			ExchangeEnd();

			ITEMMGR->SetDisableDialog( FALSE, eItemTable_Inventory );
		}
		break;

	case MP_EXCHANGE_CANCEL_ACK:
		{
			////////������ �ɼ� ���� Active False ���� �Ұ�
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

			GAMEIN->GetExchangeDialog()->SetDisable( FALSE );	//üũ..�ӽ�.

			GAMEIN->GetInventoryDialog()->SetDisable( FALSE );

			//���Է¹�ư �����.	CONFIRM
			cDialog* pDlg = WINDOWMGR->GetWindowForID( DBOX_EXCHANGE_MONEY );
			if( pDlg )
				WINDOWMGR->AddListDestroyWindow( pDlg );
			

			////////������ �ɼ� ���� : Active False ���� �Ұ�.
			GAMEIN->GetExchangeDialog()->DelItemOptionInfoAll();
			GAMEIN->GetExchangeDialog()->DelItemRareOptionInfoAll();
			GAMEIN->GetExchangeDialog()->DelItemStoneOptionAll();// 2014-12-15 !

			//��ȯ�� ��ҵǾ����� ��ȯ����� ���� �ʾ�����...
			GAMEIN->GetExchangeDialog()->SetActive( FALSE );
			if( !m_bInventoryActive )
				GAMEIN->GetInventoryDialog()->SetActive( FALSE );
			
			//��û�޼����� �����....
			
			pDlg = WINDOWMGR->GetWindowForID( MBI_EXCHANGEAPPLY);
			if( pDlg )
				WINDOWMGR->AddListDestroyWindow( pDlg );

			pDlg = WINDOWMGR->GetWindowForID( MBI_EXCHANGEWAITING );
			if( pDlg )
				WINDOWMGR->AddListDestroyWindow( pDlg );			

//error msg ���
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
			HERO->SetMoney( msg->dwData );	//+�κ��丮
		}
		break;
		
	case MP_EXCHANGE_INSERT:
		{
			MSG_ITEM* msg = (MSG_ITEM*)pMsg;
			BOOL rt = GAMEIN->GetInventoryDialog()->AddItem(&msg->ItemInfo);
			ASSERT(rt);
			//�������� ������ �ٽ� �ɼ��� �ִ´�.?

			QUICKMGR->RefreshQickItem();
		}
		break;

	case MP_EXCHANGE_REMOVE:
		{
			MSG_EXCHANGE_REMOVEITEM* msg = (MSG_EXCHANGE_REMOVEITEM*)pMsg;
			CItem* pOutItem = NULL;
			ITEMMGR->DeleteItem( msg->wAbsPosition, &pOutItem );

			//�ɼ��� ���� �������̾��ٸ� �ɼ��� ��������. �ڵ�����.
		}
		break;
	}
}





