#include "stdafx.h"
#include "ItemUnLockManager.h"
#include "WindowIDEnum.h"
#include "./Interface/cIconDialog.h"
#include "./Interface/cWindowManager.h"
#include "./Interface/cStatic.h"
#include "ChatManager.h"
#include "ObjectStateManager.h"
#include "ObjectManager.h"

CItemUnLockDialog::CItemUnLockDialog()
{
   pDlg=NULL;

   m_pIconDlg = NULL;

   m_LockItem=NULL;
}

CItemUnLockDialog::~CItemUnLockDialog()
{
   pDlg=NULL;

   m_pIconDlg = NULL;

   m_LockItem=NULL;
}

void CItemUnLockDialog::Linking()
{
    pDlg = WINDOWMGR->GetWindowForID( ITEM_UNLOCKDLGEX );

	m_pIconDlg = (cIconDialog*)GetWindowForID(IT_UNLOCKICONEX);
}

BOOL CItemUnLockDialog::FakeMoveIcon(LONG x, LONG y, cIcon * icon)
{
	if( icon->GetType() != WT_ITEM )	
		
	return FALSE;

	if( m_bDisable )	return FALSE;

	CItem* pOrigItem = (CItem*)icon;

	if( pOrigItem->IsLocked() )	return FALSE;

	ITEM_INFO* pItemInfo = ITEMMGR->GetItemInfo( pOrigItem->GetItemIdx());

	if(!pItemInfo)	return FALSE;

	if(ITEMMGR->IsDupItem(pOrigItem->GetItemIdx()))  // 叠, 叠禁止 2014-12-05
	{
		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(2125));

		return FALSE;
	}

	if( !ITEMMGR->IsEqualTableIdxForPos(eItemTable_Inventory, pOrigItem->GetPosition()) && !ITEMMGR->IsEqualTableIdxForPos(eItemTable_ShopInven, pOrigItem->GetPosition()))
	{
		return FALSE;
	}

	if(pOrigItem->GetStatic()!=ITEM_STATIC_LUCK)  // 2014-12-05  3表示被   泡点购买1保持一致！锁的禁止解锁!
	{
		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(2121));

		return FALSE;
	}

	pOrigItem->SetLock(TRUE);

	AddVirtualItem( pOrigItem );

	ITEMMGR->SetDisableDialog(TRUE, eItemTable_Pyoguk);
	ITEMMGR->SetDisableDialog(TRUE, eItemTable_GuildWarehouse);
	ITEMMGR->SetDisableDialog(TRUE, eItemTable_Shop);
	ITEMMGR->SetDisableDialog(TRUE, eItemTable_PetInven);

	return FALSE;
}

void CItemUnLockDialog::AddVirtualItem(CItem * pItem)
{
	cIcon* pIcon=(cIcon*)pItem;
	m_VirtualItem.SetData( pItem->GetItemIdx());
	m_VirtualItem.SetLinkItem(pItem);
	m_pIconDlg->AddIcon( 0, (cIcon*)&m_VirtualItem);

	if( ITEMMGR->IsOptionItem(pItem->GetItemIdx(), pItem->GetDurability()) )
	{
       ITEMMGR->SetToolTipIcon((cIcon*)&m_VirtualItem, ITEMMGR->GetItemOption(pItem->GetDurability()),ITEMMGR->GetItemRareOption( pItem->GetRareness()),0); // 将到被 2014-11-14
	}
	else
	{
		ITEMMGR->SetToolTipIcon( (cIcon*)&m_VirtualItem, NULL, ITEMMGR->GetItemRareOption(pItem->GetRareness()),0);  // 如果是强化过的,则添!
	}
}

BOOL CItemUnLockDialog::VirtualItemIsNull()
{
	if(m_VirtualItem.GetLinkItem())
	{
		return FALSE;
	}
	else 
	{
		return TRUE;
	}
}

void CItemUnLockDialog::SetLockItem(CItem * pItem)
{
      m_LockItem=pItem;
}

CItem * CItemUnLockDialog::GetLockItem()
{
      return m_LockItem;
}
void CItemUnLockDialog::SendSynMsg()
{
	MSG_DWORD4 msg;
	msg.Category = MP_ITEM;
	msg.Protocol = MP_ITEM_UNLOCKEX_SYN;
	msg.dwObjectID = HERO->GetID();
	msg.dwData1	   = m_VirtualItem.GetLinkItem()->GetItemIdx();
	msg.dwData2	   = m_VirtualItem.GetLinkItem()->GetPosition();
	msg.dwData3    = m_LockItem->GetItemIdx();
	msg.dwData4	   = m_LockItem->GetPosition();
	NETWORK->Send(&msg, sizeof(msg));
	pDlg->SetActive(TRUE);
}

void CItemUnLockDialog::Release()
{
	if( m_pIconDlg )
	{
		CVirtualItem* pVItem = NULL;

		m_pIconDlg->DeleteIcon( 0, (cIcon**)&pVItem );
		if( pVItem )
			pVItem->GetLinkItem()->SetLock( FALSE );

		m_VirtualItem.SetLinkItem( NULL );
	}

	ITEMMGR->SetDisableDialog(FALSE, eItemTable_Inventory);
	ITEMMGR->SetDisableDialog(FALSE, eItemTable_Pyoguk);
	ITEMMGR->SetDisableDialog(FALSE, eItemTable_GuildWarehouse);
	ITEMMGR->SetDisableDialog(FALSE, eItemTable_Shop);
	ITEMMGR->SetDisableDialog(FALSE, eItemTable_PetInven);

	OBJECTSTATEMGR->EndObjectState( HERO, eObjectState_Deal );

	SetActive( FALSE );

    if(m_LockItem)
	{
		m_LockItem->SetLock(FALSE);
	}

	pDlg->SetActive(FALSE);
}
