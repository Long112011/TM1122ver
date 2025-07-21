








// ItemShopGridDialog.cpp: implementation of the CItemShopGridDialog class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ItemShopGridDialog.h"

#include "ObjectManager.h"
#include "ObjectStateManager.h"
#include "ItemManager.h"
#include "./Interface/cScriptManager.h"
#include "./Interface/cWindowManager.h"
#include "ChatManager.h"
#include "PetManager.h"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CItemShopGridDialog::CItemShopGridDialog()
{
	m_type		= WT_ICONGRIDDIALOG;
	m_nIconType = WT_ITEM;	//ÀÌ ±×¸®µå´ÙÀÌ¾ó·Î±×¿¡¼­ ´Ù·ê ¾ÆÀÌÅÛ Á¾·ù
	m_TabNumber = 0;
}

CItemShopGridDialog::~CItemShopGridDialog()
{
	m_IndexGenerator.Release();
}



void CItemShopGridDialog::Init()
{
	//

	m_IndexGenerator.Init( 999, IG_SHOPITEM_STARTINDEX );
	m_type = WT_ICONGRIDDIALOG;
}


BOOL CItemShopGridDialog::DeleteItem(POSTYPE Pos,CItem** ppItem)
{
	if(!IsAddable((WORD)(Pos - (TP_SHOPITEM_START+(TABCELL_SHOPITEM_NUM*m_TabNumber)))))
		return DeleteIcon((WORD)(Pos-(TP_SHOPITEM_START+(TABCELL_SHOPITEM_NUM*m_TabNumber))), (cIcon **)ppItem);

	else
		return FALSE;
}


BOOL CItemShopGridDialog::AddItem(ITEMBASE* pItemBase)
{
	CItem * newItem = NULL;
	if( pItemBase->dwDBIdx == 0 )

	{

		//DEBUGMSG( 0, "Item DB idx == 0" );
		return FALSE;
	}

	// ÀÌ¹Ì Àü¿¡ ºÒ·¯¿Ô´ø ¾ÆÀÌÅÛ
	if( newItem = ITEMMGR->GetItem( pItemBase->dwDBIdx ) )
	{
		ITEMBASE* pIb = (ITEMBASE*)newItem->GetItemBaseInfo();
		pIb->Position = pItemBase->Position;
		int pos = GetRelativePosition(newItem->GetPosition());
		return AddIcon(pos, newItem);
	}


	// ItemManager¿¡ ¾ÆÀÌÅÛ Á¤º¸Ãß°¡
	newItem = ITEMMGR->MakeNewItem( pItemBase, "CItemShopGridDialog::AddItem" );
	if( newItem == NULL)
		return FALSE;

	newItem->SetMovable(TRUE);


	//»ö º¯È¯

	ITEMMGR->RefreshItem( newItem );


	POSTYPE relPos = GetRelativePosition(newItem->GetPosition());
	return AddIcon(relPos, newItem);
}


POSTYPE CItemShopGridDialog::GetRelativePosition( POSTYPE absPos)


{
	return ( absPos - TP_SHOPITEM_START ) %	TABCELL_SHOPITEM_NUM;
}


void CItemShopGridDialog::ShopItemDelete(DWORD dwDBIdx, POSTYPE absPos, DWORD dwCount)
{
}


BOOL CItemShopGridDialog::FakeMoveItem(LONG MouseX, LONG MouseY, CItem* pSrcItem)
{
	WORD ToPos=0;
	
	if( !GetPositionForXYRef(MouseX, MouseY, ToPos) )

		return FALSE;	
	
	ToPos = (WORD)(ToPos+TP_SHOPITEM_START+(TABCELL_SHOPITEM_NUM*m_TabNumber));

	CItem * pToItem = GetItemForPos( ToPos );
	
	if(pToItem && pToItem->IsLocked() == TRUE)
		return FALSE;


	if( pSrcItem->GetUseParam() == 1 )
	{
		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(748) );
		return FALSE;
	}

	if( PETMGR->IsCurPetSummonItem(pSrcItem->GetDBIdx()) )
	{
		return FALSE;
	}

	if(pSrcItem->GetItemBaseInfo()->ItemStatic == ITEM_STATIC_NEWBIE)
	{
		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(2657) );
		return FALSE;
	}

	return FakeGeneralItemMove( ToPos, pSrcItem, pToItem );
}



CItem* CItemShopGridDialog::GetItemForPos(POSTYPE absPos)
{

	if(TP_SHOPITEM_START <= absPos && absPos < TP_SHOPITEM_END)
	{
		return (CItem*)GetIconForIdx((WORD)(absPos-(TP_SHOPITEM_START+(TABCELL_SHOPITEM_NUM*m_TabNumber))));
	}

	return NULL;
}


BOOL CItemShopGridDialog::FakeGeneralItemMove(POSTYPE ToPos, CItem * pFromItem, CItem * pToItem)
{
	if(CanBeMoved(pFromItem, ToPos) == FALSE)
		return FALSE;
	BYTE FromIdx = ITEMMGR->GetTableIdxForAbsPos( pFromItem->GetPosition() );
	if(FromIdx >= eItemTable_Max) return FALSE;

	BYTE ToIdx = ITEMMGR->GetTableIdxForAbsPos( ToPos );
	if (ToIdx >= eItemTable_Max) return FALSE;


	if(pFromItem->GetPosition() == ToPos)
		return FALSE;
	if(pToItem && CanBeMoved( pToItem, pFromItem->GetPosition() ) == FALSE )
	{
		return FALSE;
	}
	


	MSG_ITEM_MOVE_SYN msg;
	msg.Category			= MP_ITEM;
	msg.Protocol			= MP_ITEM_MOVE_SYN;
	msg.dwObjectID			= HEROID;

	msg.FromPos				= pFromItem->GetPosition();
	msg.wFromItemIdx		= pFromItem->GetItemIdx();
	msg.ToPos				= ToPos;
	msg.wToItemIdx			= (pToItem?pToItem->GetItemIdx():0);

	NETWORK->Send( &msg, sizeof(msg) );
	return TRUE;
}


BOOL CItemShopGridDialog::CanBeMoved(CItem* pItem,POSTYPE pos)
{
	ASSERT(pItem);
	/*if( pItem->GetUseParam() == 1 )
	{
		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(748) );
		return FALSE;
	}*/
	if( TP_SHOPITEM_START <= pos && pos < TP_SHOPINVEN_END/*-40*/ )
	{
		// ¾ÆÀÌÅÛ¼¥ ¾ÆÀÌÅÛÀº ¾ÆÀÌÅÛ¸ôÀÎº¥¿¡¼­¸¸ ³ÖÀ» ¼ö ÀÖ´Ù.
		POSTYPE fromPos = pItem->GetPosition();
		if( TP_SHOPITEM_START <= fromPos && fromPos < TP_SHOPINVEN_END/*-40*/ )
			return TRUE;
		else
			return FALSE;
	}
	else

		return FALSE;

}





void CItemShopGridDialog::MoveItemToItemShopDlg(CItem* pFromItem,WORD num)
{//[ÓÒ¼ü²Ö´¢][BY:Ê®ÀïÆÂ½£´«Ææ][QQ:112582793][2019-4-16][19:38:19]
	CItem * pToItem = NULL;
	if( pFromItem->GetUseParam() == 1 )
	{
		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(748) );
		return ;
	}
	for (int i = TP_SHOPITEM_START + num; i< TP_SHOPITEM_START + num + TABCELL_SHOPITEM_NUM; i++)
	{
		if(pFromItem->GetItemBaseInfo()->ItemStatic == ITEM_STATIC_NEWBIE)
		{
			CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(2657) );
			return ;
		}
		pToItem = GetItemForPos(i);
		if(!pToItem)
		{
			FakeGeneralItemMove(i,pFromItem,pToItem);
			return;
		}
	}
	CHATMGR->AddMsg(CTC_SYSMSG,CHATMGR->GetChatMsg(2348));
}
#include "GameIn.h"
#include "InventoryExDialog.h"
void CItemShopGridDialog::OnActionEvnet(LONG lId, void * p, DWORD we)
{//[ÓÒ¼ü²Ö´¢][BY:Ê®ÀïÆÂ½£´«Ææ][QQ:112582793][2019-4-16][21:30:28]
	if(we == WE_RBTNCLICK)
	{
		CItem * pItem = NULL;
		pItem = (CItem*)this->GetIconForIdx((WORD)this->GetCurSelCellPos());

		if( !pItem )
			return;

		GAMEIN->GetInventoryDialog()->GetItemFromItemShop(pItem);
	}
}