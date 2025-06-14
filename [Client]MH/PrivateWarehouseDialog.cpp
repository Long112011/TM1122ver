








// PrivateWarehouseDialog.cpp: implementation of the CPrivateWarehouseDialog class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "PrivateWarehouseDialog.h"
#include "ItemManager.h"
#include "WindowIDEnum.h"
#include "./Interface/cWindowManager.h"
#include "GameIn.h"
#include "cDivideBox.h"
#include "ObjectManager.h"
#include "ObjectStateManager.h"
#include "./Input/UserInput.h"
#include "PyogukDialog.h"
#include "InventoryExDialog.h"
#include "ChatManager.h"
#include "TitanManager.h"

#include "PetManager.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPrivateWarehouseDialog::CPrivateWarehouseDialog()
{
	m_type		= WT_ICONGRIDDIALOG;
	m_nIconType = WT_ITEM;	//捞 弊府靛促捞倔肺弊俊辑 促逢 酒捞袍 辆幅
}


CPrivateWarehouseDialog::~CPrivateWarehouseDialog()
{
	
}


void CPrivateWarehouseDialog::Init(LONG x, LONG y, WORD wid, WORD hei, cImage * basicImage, LONG ID)
{
	cDialog::Init(x,y,wid,hei,basicImage,ID);
	m_type = WT_ICONGRIDDIALOG;

}

BOOL CPrivateWarehouseDialog::AddItem(ITEMBASE* pItemInfo)
{
	if( pItemInfo->dwDBIdx == 0 )
	{
		DEBUGMSG( 0, "Item DB idx == 0" );
		return FALSE;
	}

	CItem * newItem = ITEMMGR->MakeNewItem( pItemInfo,"CPrivateWarehouseDialog::AddItem" );
	if(newItem == NULL)
		return FALSE;
	
	return AddItem( newItem );
}


BOOL CPrivateWarehouseDialog::AddItem(CItem* pItem)
{

	ASSERT(pItem);
	//祸 函券
	ITEMMGR->RefreshItem( pItem );

	POSTYPE relPos = GetRelativePosition(pItem->GetPosition());

	return AddIcon(relPos, pItem);
}

BOOL CPrivateWarehouseDialog::DeleteItem(POSTYPE Pos,CItem** ppItem)
{
	BYTE num = GAMEIN->GetPyogukDialog()->GetSelectedPyoguk();
	WORD A = TP_PYOGUK_START;
	WORD B = TABCELL_PYOGUK_NUM*(num);
	if(!IsAddable(Pos - A-B))
		return DeleteIcon(Pos-TP_PYOGUK_START-TABCELL_PYOGUK_NUM*(num), (cIcon **)ppItem);
	else
		return FALSE;
}

BOOL CPrivateWarehouseDialog::FakeMoveIcon(LONG x, LONG y, cIcon * icon)
{	
	ASSERT(icon);
	
	if(icon->GetType() == WT_ITEM)
	{
		ITEM_INFO* pItemInfo = ITEMMGR->GetItemInfo( ((CItem*)icon)->GetItemIdx() );
		if( !pItemInfo || pItemInfo->ItemKind & eSHOP_ITEM )
			return FALSE;

		FakeMoveItem(x, y, (CItem *)icon);
	}
	
	return FALSE;
}

void CPrivateWarehouseDialog::FakeMoveItem( LONG mouseX, LONG mouseY, CItem * pFromItem )
{
	WORD ToPos=0;

	const ITEMBASE* pInfo = pFromItem->GetItemBaseInfo();

	//!!! 烙矫 鸥捞藕 厘厚 捞悼 阜扁	-> magi82 - Titan(071015) 鸥捞藕 厘馒袍捞 芒绊捞悼 登档废 贸府啊 救登菌扁 锭巩俊 烙矫肺 持菌瘤父.. 贸府肯丰秦辑 林籍贸府窃
	//if( ITEMMGR->IsTitanEquipItem(pInfo->wIconIdx) )
	//{
	//	CHATMGR->AddMsg(CTC_SYSMSG, "捞悼 且 荐 绝绰 酒捞袍 涝聪促.");
	//	return;
	//}


	// magi82(20) - Titan(071105) 拌距等 鸥捞藕 刘辑 酒捞袍篮 芒绊俊 甸绢啊瘤 臼绰促.
	if(TITANMGR->IsRegistedTitan(pFromItem->GetDBIdx()))

	{
		CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(1578));
		return;
	}

	if( PETMGR->IsCurPetSummonItem(pInfo->dwDBIdx) )
		return;

	if( !GetPositionForXYRef(mouseX, mouseY, ToPos ) )	
		return;

	BYTE num = GAMEIN->GetPyogukDialog()->GetSelectedPyoguk();
	ToPos = ToPos+TP_PYOGUK_START + TABCELL_PYOGUK_NUM*(num); // 例措困摹 逞绢 咳

	CItem * pToItem = GetItemForPos( ToPos );

	
/*
	if( ITEMMGR->IsDupItem( pFromItem->GetItemIdx() ) )
	{
		if( KEYBOARD->GetKeyPressed(KEY_MENU) && !( pToItem ) )
		{
			FakeItemDivide( ToPos, pFromItem, pToItem );

			return;
		}
		else if( ( pToItem && pFromItem->GetItemIdx() == pToItem->GetItemIdx() ) )
		{
			FakeItemCombine( ToPos, pFromItem, pToItem );
			return;
		}
	}
*/
	if(pToItem)
	if(pToItem->IsLocked() == TRUE)
		return;
	FakeGeneralItemMove( ToPos, pFromItem, pToItem );
}

POSTYPE CPrivateWarehouseDialog::GetRelativePosition( POSTYPE absPos)

{
	return ( absPos - TP_PYOGUK_START ) %	TABCELL_PYOGUK_NUM;
}
 

CItem * CPrivateWarehouseDialog::GetItemForPos(POSTYPE absPos)
{
	if(TP_PYOGUK_START <= absPos && absPos < TP_PYOGUK_END)
	{


		int num  = GAMEIN->GetPyogukDialog()->GetPyogukNum(absPos);
		return (CItem *)GetIconForIdx(absPos-TP_PYOGUK_START-TABCELL_PYOGUK_NUM*num);
	}

	return NULL;
}

void CPrivateWarehouseDialog::FakeGeneralItemMove( POSTYPE ToPos, CItem * pFromItem, CItem * pToItem)
{
	if(CanBeMoved(pFromItem, ToPos) == FALSE)
		return;
	BYTE FromIdx = ITEMMGR->GetTableIdxForAbsPos( pFromItem->GetPosition() );
	if(FromIdx >= eItemTable_Max) return;


	BYTE ToIdx = ITEMMGR->GetTableIdxForAbsPos( ToPos );
	if (ToIdx >= eItemTable_Max) return;

	if(pFromItem->GetPosition() == ToPos)
		return;
	if(pToItem && CanBeMoved( pToItem, pFromItem->GetPosition() ) == FALSE )
	{
		return;
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
}

void CPrivateWarehouseDialog::FakeItemDivide( POSTYPE ToPos, CItem * pFromItem, CItem * pToItem )
{

	if( !CanBeMoved( pFromItem, ToPos ) )
		return;
	if( pFromItem->GetPosition() == ToPos )
		return;

	GAMEIN->GetPyogukDialog()->SetDividMsg( pFromItem, ToPos );
	// Locking
//	pFromItem->SetMovable( FALSE );
	
	BYTE FromIdx = ITEMMGR->GetTableIdxForAbsPos( pFromItem->GetPosition() );
	if(FromIdx >= eItemTable_Max) return;

	GAMEIN->GetInventoryDialog()->SetDisable(TRUE);

	CPyogukDialog* pPyogukDlg = GAMEIN->GetPyogukDialog();
	pPyogukDlg->SetDisable(TRUE);

	cDivideBox * pDivideBox = WINDOWMGR->DivideBox( DBOX_DIVIDE_INV, (LONG)pFromItem->GetAbsX(), (LONG)pFromItem->GetAbsY(), OnFakePyogukItemDivideOk, OnFakePyogukItemDivideCancel, pPyogukDlg, pFromItem, CHATMGR->GetChatMsg(185) );
	pDivideBox->SetValue( 1 );
	pDivideBox->SetMaxValue( MAX_YOUNGYAKITEM_DUPNUM );
}


void CPrivateWarehouseDialog::OnFakePyogukItemDivideCancel( LONG iId, void* p, DWORD param1, void * vData1, void * vData2 )
{
	cDialog * ToDlg = ( cDialog * )vData1;

	ToDlg->SetDisable(FALSE);
	GAMEIN->GetInventoryDialog()->SetDisable(FALSE);
}

void CPrivateWarehouseDialog::OnFakePyogukItemDivideOk( LONG iId, void* p, DWORD param1, void * vData1, void * vData2 )
{
	CPyogukDialog * ptDlg = ( CPyogukDialog * )vData1;
	cDialog * pfDlg = (cDialog *)vData2;
	if( param1 == 0 ) 
	{
		ptDlg->SetDisable(FALSE);
		GAMEIN->GetInventoryDialog()->SetDisable(FALSE);
		return;
	}

	if( ptDlg->m_divideMsg.FromDur > param1 )
	{
		ptDlg->m_divideMsg.ToDur			= param1;
		ptDlg->m_divideMsg.FromDur		= ptDlg->m_divideMsg.FromDur - param1;
		NETWORK->Send( &ptDlg->m_divideMsg, sizeof(ptDlg->m_divideMsg) );
	}
	else
	{
		//酒捞袍 捞悼
		ptDlg->FakeGeneralItemMove(ptDlg->m_divideMsg.ToPos, (CItem *)vData2, NULL);

		ptDlg->SetDisable(FALSE);
		GAMEIN->GetInventoryDialog()->SetDisable(FALSE);
	}
}

void CPrivateWarehouseDialog::FakeItemCombine( POSTYPE ToPos, CItem * pFromItem, CItem * pToItem )
{
	if( pFromItem->GetPosition() == pToItem->GetPosition() )
		return;
	if( pFromItem->GetItemIdx() != pToItem->GetItemIdx() )
		return;

	if( CanBeMoved( pFromItem, ToPos ) == FALSE )
		return;


	if( pToItem && CanBeMoved( pToItem, pFromItem->GetPosition() ) == FALSE )
		return;

	BYTE FromIdx = ITEMMGR->GetTableIdxForAbsPos( pFromItem->GetPosition() );
	if(FromIdx >= eItemTable_Max) return;

	BYTE ToIdx = ITEMMGR->GetTableIdxForAbsPos( ToPos );
	if(ToIdx >= eItemTable_Max) return;

	MSG_ITEM_COMBINE_SYN msg;
	msg.Category			= MP_ITEM;
	msg.Protocol			= MP_ITEM_COMBINE_SYN;
	msg.dwObjectID			= HEROID;
	msg.FromPos				= pFromItem->GetPosition();
	msg.ToPos				= pToItem->GetPosition();
	msg.wItemIdx			= pFromItem->GetItemIdx();
	msg.FromDur				= pFromItem->GetDurability();
	msg.ToDur				= pToItem->GetDurability();

	NETWORK->Send( &msg, sizeof(msg) );
}


BOOL CPrivateWarehouseDialog::CanBeMoved(CItem* pItem,POSTYPE pos)
{
	ASSERT(pItem);
	if(pItem)
	{
		POSTYPE fromPos = pItem->GetPosition();
		POSTYPE toPos = pos;
		if( (TP_WEAR_START <= fromPos && fromPos < TP_WEAR_END) ||
			(TP_WEAR_START <= toPos && toPos < TP_WEAR_END) )
		{
			if( OBJECTSTATEMGR->CheckAvailableState(HERO,eCAS_EQUIPITEM) == FALSE )
			{
				CAction act;
				act.InitAction(eActionKind_EquipItem,(void*)pItem->GetDBIdx(),0,NULL);
				HERO->SetNextAction(&act);
				return FALSE;
			}
		}

		if( (TP_PETWEAR_START <= fromPos && fromPos < TP_PETWEAR_END ) ||
			(TP_PETINVEN_START <= fromPos && fromPos < TP_PETINVEN_END ) )
			return FALSE;


	}
	if(TP_PYOGUK_START <= pos && pos < TP_PYOGUK_END)
	{
		// 公炼扒 TRUE
		return TRUE;
	}
/*	else if(TP_MUNPAWAREHOUSE_START <= pos && pos < TP_MUNPAWAREHOUSE_END)
	{
		// 公炼扒 TRUE
		return TRUE;
	}*/
	else if(TP_INVENTORY_START <= pos && pos < TP_INVENTORY_END)
	{
		// 公炼扒 TRUE
		return TRUE;
	}
	else if(TP_WEAR_START <= pos && pos < TP_WEAR_END)
	{
		// 厘馒窍绰 酒捞牌捞 酒聪搁 FALSE
		if( !( pItem->GetItemKind() & eEQUIP_ITEM ) )
			return FALSE;

		// 甸绢啊具 且磊府啊 酒聪搁 FALSE
		ITEM_INFO * pInfo = ITEMMGR->GetItemInfo(pItem->GetItemIdx());
		if(!pInfo) return FALSE;
		if(pInfo->EquipKind == eWearedItem_Ring1)
		{	
			if(pos-TP_WEAR_START != eWearedItem_Ring1 && pos-TP_WEAR_START != eWearedItem_Ring2)
				return FALSE;
		}
		else
		{
			if(pos-TP_WEAR_START != pInfo->EquipKind)
				return FALSE;
		}
		
		return ITEMMGR->CanEquip(pItem->GetItemIdx());
	}
	else
		return FALSE;
}
	




void CPrivateWarehouseDialog::OnActionEvnet(LONG lId, void * p, DWORD we)
{//[塘숩꾑뇨][BY:枷쟁팃숲 ?[QQ:112582793][2019-4-16][21:04:03]
	if(we == WE_RBTNCLICK)
	{
		CItem * pItem = NULL;
		pItem = (CItem*)this->GetIconForIdx((WORD)this->GetCurSelCellPos());

		if( !pItem )
			return;
		GAMEIN->GetInventoryDialog()->GetItemFromPyoguk(pItem);
	}
}
void CPrivateWarehouseDialog::MoveItemToPyoguk(CItem* pFromItem,WORD num)
{//[塘숩꾑뇨][BY:枷쟁팃숲 ?[QQ:112582793][2019-4-16][18:10:50]
	CItem * pToItem = NULL;
	if(ITEMMGR->IsDupItem(pFromItem->GetItemIdx()))
	{
		for (int i = TP_PYOGUK_START + num; i< TP_PYOGUK_START + num + TABCELL_PYOGUK_NUM; i++)
		{
			pToItem = GetItemForPos(i);
			if(pToItem)
			{
				if(pToItem->GetItemIdx()==pFromItem->GetItemIdx()&&
					(pToItem->GetDurability()<MAX_YOUNGYAKITEM_DUPNUM)&&
					pFromItem->GetDurability()<MAX_YOUNGYAKITEM_DUPNUM)
				{
					FakeItemCombine(i,pFromItem,pToItem);
					return;
				}
			}
		}
	}
	for (int i = TP_PYOGUK_START + num; i< TP_PYOGUK_START + num + TABCELL_PYOGUK_NUM; i++)
	{
		pToItem = GetItemForPos(i);
		if(!pToItem)
		{
			FakeGeneralItemMove(i,pFromItem,pToItem);
			return;
		}
	}
	CHATMGR->AddMsg(CTC_SYSMSG,CHATMGR->GetChatMsg(2348));
}