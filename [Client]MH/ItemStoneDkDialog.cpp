#include "stdafx.h"
#include "ItemStoneDkDialog.h"
#include "WindowIDEnum.h"
#include "./Interface/cIconDialog.h"
#include "./Interface/cWindowManager.h"
#include "./Interface/cStatic.h"
#include "ChatManager.h"
#include "ObjectStateManager.h"
#include "ObjectManager.h"



CItemStoneDkDialog::CItemStoneDkDialog()
{
   pDlg=NULL;

   pRate=NULL;

   pRateValue=0;

   m_pIconDlg = NULL;

   m_StoneItem=NULL;

}

CItemStoneDkDialog::~CItemStoneDkDialog()
{
   pDlg=NULL;

   pRate=NULL;

   m_pIconDlg = NULL;

   m_StoneItem=NULL;

}

void CItemStoneDkDialog::Release()
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

    if(m_StoneItem)
	{
		m_StoneItem->SetLock(FALSE);
	}
}

void CItemStoneDkDialog::Linking()
{
  	pDlg = WINDOWMGR->GetWindowForID( ITEMSTONEDK_DLG );
	pRate= (cStatic*)GetWindowForID(ITEMSTONEDK_RATE);
	m_pIconDlg = (cIconDialog*)GetWindowForID(ITEMSTONEDK_ICON);
}
void CItemStoneDkDialog::SetItemStoneRate(DWORD Val)
{
	pRateValue=Val;

	if(pDlg)
	{
		if(pRate)
		{
			char RateMsg[128];
              
            sprintf(RateMsg,CHATMGR->GetChatMsg(2113),pRateValue);

			pRate->SetStaticText(RateMsg);
		}
	}
}

void CItemStoneDkDialog::SetItemStone(CItem * pItem)
{
    m_StoneItem=pItem;
}

CItem * CItemStoneDkDialog::GetItemStone()
{
	return  m_StoneItem;
}

BOOL CItemStoneDkDialog::FakeMoveIcon(LONG x,LONG y,cIcon * icon)
{
	if( icon->GetType() != WT_ITEM )	return FALSE;

	if( m_bDisable )	return FALSE;

	CItem* pOrigItem = (CItem*)icon;

	if( pOrigItem->IsLocked() )	return FALSE;

	ITEM_INFO* pItemInfo = ITEMMGR->GetItemInfo( pOrigItem->GetItemIdx());

	if(!pItemInfo)	return FALSE;

	eITEM_KINDBIT bits = pOrigItem->GetItemKind();

	if(!ITEMMGR->IsStoneOptionItem(pOrigItem->GetItemIdx(),bits))  // 可以!
	{
		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1911));

		return FALSE;
	}

	if( !ITEMMGR->IsEqualTableIdxForPos(eItemTable_Inventory, pOrigItem->GetPosition()) && !ITEMMGR->IsEqualTableIdxForPos(eItemTable_ShopInven, pOrigItem->GetPosition()))
	{
		return FALSE;
	}

	ITEM_STONE_OPTION_INFO * pStoneInfo= ITEMMGR->GetItemStoneOption(pOrigItem->GetStoneIdx());

	if(GetStoneDkCount(pStoneInfo)==5)  // 2014-01-25 最大打5孔
	{
		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1918));

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

void CItemStoneDkDialog::AddVirtualItem(CItem * pItem)
{
	cIcon* pIcon=(cIcon*)pItem;
	m_VirtualItem.SetData( pItem->GetItemIdx());
	m_VirtualItem.SetLinkItem(pItem);
	m_pIconDlg->AddIcon( 0, (cIcon*)&m_VirtualItem);

	if( ITEMMGR->IsOptionItem(pItem->GetItemIdx(), pItem->GetDurability()) )
	{
       ITEMMGR->SetToolTipIcon((cIcon*)&m_VirtualItem, ITEMMGR->GetItemOption(pItem->GetDurability()),ITEMMGR->GetItemRareOption( pItem->GetRareness()),0,ITEMMGR->GetItemStoneOption(pItem->GetStoneIdx())); // 将到被 2014-11-14
	}
	else
	{
		ITEMMGR->SetToolTipIcon( (cIcon*)&m_VirtualItem, NULL, ITEMMGR->GetItemRareOption(pItem->GetRareness()),0,ITEMMGR->GetItemStoneOption(pItem->GetStoneIdx()));  // 如果是强化过的,则添!
	}

}

void CItemStoneDkDialog::SendItemStoneDkMsg()
{
	if( !m_VirtualItem.GetLinkItem() )	return;

	CItem* pItem = ITEMMGR->GetItem( m_VirtualItem.GetLinkItem()->GetDBIdx());

	if(!pItem)
	{
		if( m_pIconDlg )
		{
			CVirtualItem* pVItem = NULL;
			m_pIconDlg->DeleteIcon( 0, (cIcon**)&pVItem );
			if( pVItem )
				pVItem->GetLinkItem()->SetLock( FALSE );
		}
		ITEMMGR->SetDisableDialog(FALSE, eItemTable_Inventory);
		ITEMMGR->SetDisableDialog(FALSE, eItemTable_Pyoguk);
		ITEMMGR->SetDisableDialog(FALSE, eItemTable_GuildWarehouse);
		ITEMMGR->SetDisableDialog(FALSE, eItemTable_Shop);
		ITEMMGR->SetDisableDialog(FALSE, eItemTable_PetInven);
		OBJECTSTATEMGR->EndObjectState( HERO, eObjectState_Deal );
		return;
	}

	MSG_DWORD4	msg;
	msg.Category = MP_ITEM;
	msg.Protocol = MP_ITEM_STONE_DK_SYN;
	msg.dwObjectID = HEROID;
	msg.dwData1	   = pItem->GetItemIdx();        // 被
	msg.dwData2	   = pItem->GetPosition();       // 被位置
	msg.dwData3    = m_StoneItem->GetItemIdx();  // 石头
	msg.dwData4    = m_StoneItem->GetPosition(); // 石头位置

	NETWORK->Send( &msg, sizeof(msg) );
	SetActive( FALSE );
}

int  CItemStoneDkDialog::GetStoneDkCount(ITEM_STONE_OPTION_INFO * pStoneInfo)
{
	int Count=0;

	if(pStoneInfo)
	{
		if(pStoneInfo->dwItemStone0!=0)
		{
			Count=Count+1;
		}
		if(pStoneInfo->dwItemStone1!=0)
		{
			Count=Count+1;
		}
		if(pStoneInfo->dwItemStone2!=0)
		{
			Count=Count+1;
		}
		if(pStoneInfo->dwItemStone3!=0)
		{
			Count=Count+1;
		}
		if(pStoneInfo->dwItemStone4!=0)
		{
			Count=Count+1;
		}
	}
	return Count;
}