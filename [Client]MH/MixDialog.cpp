#include "stdafx.h"
#include "MixDialog.h"
#include "ChatManager.h"
#include "..\[CC]MixManager\MixManager.h"
#include "Item.h"
#include "INTERFACE\cIconDialog.h"
#include "./Interface/cListDialog.h"		
#include "INTERFACE\cIconGridDialog.h"
#include "WindowIDEnum.h"
#include "ItemShow.h"
#include "Gamein.h"
#include "ItemManager.h"
#include "InventoryExDialog.h"
#include "interface/cStatic.h"
#include "./Interface/cScriptManager.h"
#include "ObjectManager.h"
#include "./Interface/cWindowManager.h"
#include "objectstatemanager.h"
#include "StreetStallManager.h"
CMixDialog::CMixDialog()
{
	m_pMixDlg = NULL;
	m_pMixGridDlg = NULL;
	cImage imgToolTip;
	SCRIPTMGR->GetImage( 63, &imgToolTip, PFT_HARDPATH );
	for(int i = 0 ; i < eMixPos_Max ; ++i)
	{
		m_VirtualItem[i].SetToolTip( "", RGB_HALF( 255, 255, 255), &imgToolTip );
		m_VirtualItem[i].SetMovable(FALSE);
	}
	for(int i = 0 ; i < MAX_MIXGRID_NUM ; ++i)
	{
		m_ViewShowItem[i].SetToolTip( "", RGB_HALF( 255, 255, 255), &imgToolTip );
		m_ViewShowItem[i].SetMovable(FALSE);
	}
	m_bShopItemUse	= FALSE;
	m_ShopItemIdx	= 0;
	m_ShopItemPos	= 0;
	m_pTitle		= NULL;
	memset( m_Title, 0, sizeof(char)*128 );
	memset( m_Ratio, 0, sizeof(char)*128 );
	SCRIPTMGR->GetImage(68, &m_ScrollImg, PFT_JACKPATH);
	mScale.x = 1.3 ;
	mScale.y = 1.2 ;			 
}
CMixDialog::~CMixDialog()
{
}
void CMixDialog::Release(eMixReleaseOpt op, BOOL bMsg)
{
	ResetCurSelCellPos();
	CVirtualItem * pVItem;
	if(!m_pMixDlg->IsAddable(eMixPos_Basic) && (op & eMixBasicRelease) )
	{
		m_pMixDlg->DeleteIcon(eMixPos_Basic, (cIcon **)&pVItem);
		pVItem->GetLinkItem()->SetLock(FALSE);
		if( bMsg )
		{
			MSG_WORD msg;
			msg.Category	= MP_ITEM;
			msg.Protocol	= MP_ITEM_MIX_RELEASEITEM;
			msg.dwObjectID	= HEROID;
			msg.wData		= pVItem->GetLinkItem()->GetPosition();
			NETWORK->Send( &msg, sizeof(msg) );
		}
		pVItem->SetLinkItem(NULL);
		DeleteViewResultItemAll();
		m_pMaterialsStatic->SetStaticText(" ");
		m_pSuccessRatio->SetStaticText(" ");
		m_pMsgListDlg->RemoveAll();				 
	}
	if(!m_pMixDlg->IsAddable(eMixPos_Result) && (op & eMixResultRelease) )
	{
		m_pMixDlg->DeleteIcon(eMixPos_Result, (cIcon **)&pVItem);
		pVItem->GetLinkItem()->SetLock(FALSE);
		if( bMsg )
		{
			MSG_WORD msg;
			msg.Category	= MP_ITEM;
			msg.Protocol	= MP_ITEM_MIX_RELEASEITEM;
			msg.dwObjectID	= HEROID;
			msg.wData		= pVItem->GetLinkItem()->GetPosition();
			NETWORK->Send( &msg, sizeof(msg) );
		}
		pVItem->SetLinkItem(NULL);
	}
}
void CMixDialog::SetActiveRecursive(BOOL val)
{
	if( (STREETSTALLMGR->GetStallKind() == eSK_BUY && OBJECTSTATEMGR->GetObjectState(HERO) == eObjectState_StreetStall_Owner) )
	{
		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(798) );
		return;
	}
	cDialog::SetActiveRecursive( val );
	if( val )
	{
		if( m_bShopItemUse )
		{
			char buf [256];
			sprintf( buf, "%s - (%s)", m_Title, CHATMGR->GetChatMsg( 1092) );
			m_pTitle->SetStaticText( buf );
		}
		else
			m_pTitle->SetStaticText( m_Title );
	}
	if( !val && m_bShopItemUse )
	{
		CItem* pItem = ITEMMGR->GetItemofTable( eItemTable_ShopInven, (POSTYPE)m_ShopItemPos );
		if( pItem )
			pItem->SetLock( FALSE );
		m_bShopItemUse	= FALSE;
		m_ShopItemIdx	= 0;
		m_ShopItemPos	= 0;
	}
}
BOOL CMixDialog::IsEmpty(eMixPos pos)
{
	if( pos >= eMixPos_Max )
		return FALSE;
	return m_pMixDlg->IsAddable(pos);
}
void CMixDialog::Linking()
{
	m_pMaterialsStatic = (cStatic *)GetWindowForID(ITMD_MIXMATERIALS);
	m_pSuccessRatio = (cStatic *)GetWindowForID(ITMD_MIXSUCCESSRATE);
	m_pMsgListDlg = (cListDialog*)GetWindowForID(ITMD_MIXMATERIALSLIST);																 
	m_pMaterialsStatic->SetMultiLine(TRUE);
	m_pMaterialsStatic->InitMultiLine();
	m_pMixDlg = (cIconDialog *)GetWindowForID(ITMD_MIXICONDLG);
	m_pMixGridDlg = (cIconGridDialog *)GetWindowForID(ITMD_MIXICONGRIDDLG);
	m_pTitle = (cStatic*)GetWindowForID(ITMD_MIXICONTITLE);
	m_pArrowStatic = new cStatic; 
	if( m_pArrowStatic )
	{
		m_pArrowStatic->Init( 119,47,44,14, NULL, -1 );
		m_pArrowStatic->SetScale(&mScale);
		Add( m_pArrowStatic );
	}
	if( m_pTitle )
		strcpy( m_Title, m_pTitle->GetStaticText() );
	m_bMixing = FALSE;
}
BOOL CMixDialog::FakeMoveIcon(LONG x, LONG y, cIcon * pOrigIcon)
{
	if( pOrigIcon == NULL )
	{
		CHATMGR->AddMsg(CTC_SYSMSG, "return");
		return false;
	}
	if(pOrigIcon->GetType() != WT_ITEM) return FALSE;
	if( m_bDisable ) return FALSE;
	CItem * pOrigItem = (CItem *)pOrigIcon;
	BOOL bIsCanMix = FALSE;
	bIsCanMix= ITEMMGR->CanMixItem(pOrigItem->GetItemIdx());
	if( pOrigItem->IsLocked() ) return FALSE;
	if(!ITEMMGR->IsEqualTableIdxForPos(eItemTable_Inventory, pOrigItem->GetPosition(),TRUE))
	{
		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg( 612 ) );
		return FALSE;
	}
	// 检查物品是否有 grade
	if (pOrigItem->GetGrade() > 0) // 假设 GetGrade() 方法返回物品的等级
	{
		CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(2745)); // 设置一个新的提示ID，比如“物品有等级，无法进行此操作”
		return FALSE;
	}
	if(ITEMMGR->IsOptionItem(pOrigItem->GetItemIdx(), pOrigItem->GetDurability()))
	{
		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(202) );
		return FALSE;
	}
	eITEM_KINDBIT bits = pOrigItem->GetItemKind();
	if( !(bits & eYOUNGYAK_ITEM) && !(bits & eMUGONG_ITEM)
		&& !(bits & eEQUIP_ITEM) && !(bits & eEXTRA_ITEM) 
		&& !(bits & eCHANGE_ITEM) && !(bits & eQUEST_ITEM_START)&&
		!bIsCanMix)
	{
		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(202) );
		return FALSE;
	}
	if(bits == eEQUIP_ITEM_UNIQUE && GAMERESRCMNGR->GetUniqueItemOptionList(pOrigItem->GetItemIdx())->MixFlag == 0)
	{
		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1625) );
		return FALSE;
	}
	const ITEM_MIX_INFO * pMixItemInfo = MIXMGR->GetMixItemInfo(pOrigItem->GetItemIdx());
	if(!pMixItemInfo)
	{
		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(202) );
		return FALSE;
	}
	if( GetItemKind(pOrigItem->GetItemIdx() ) != eYOUNGYAK_ITEM && 
		GetItemKind(pOrigItem->GetItemIdx() ) != eEXTRA_ITEM_JEWEL && 
#ifdef _ITEMFUSE
		GetItemKind(pOrigItem->GetItemIdx() ) != eEXTRA_ITEM_FUSE && 
#endif
		!bIsCanMix&&
		(pOrigItem->GetDurability() != 1&&!IsEquipItem(GetItemKind(pOrigItem->GetItemIdx()))))
	{
		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(629) );
		return FALSE;
	}
	if( (bits & eEQUIP_ITEM) && m_bShopItemUse)
	{
		ITEM_INFO* pShopItem = ITEMMGR->GetItemInfo( (WORD)m_ShopItemIdx );
		if( !pShopItem )				return FALSE;
		if( !(pShopItem->ItemKind & eSHOP_ITEM) )		return FALSE;
		ITEM_INFO* pBaseItemInfo = ITEMMGR->GetItemInfo( pOrigItem->GetItemIdx() );
		if( !pBaseItemInfo )			return FALSE;
		if( pBaseItemInfo->LimitLevel < pShopItem->GenGol || pBaseItemInfo->LimitLevel > pShopItem->MinChub )
		{
			CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1082) );
			return FALSE;
		}
	}
	pOrigItem->SetLock(TRUE);
	MSG_ITEM	msgItem;
	msgItem.Category	= MP_ITEM;
	msgItem.Protocol	= MP_ITEM_MIX_ADDITEM_SYN;
	msgItem.dwObjectID	= HEROID;
	msgItem.ItemInfo	= *pOrigItem->GetItemBaseInfo();
	NETWORK->Send( &msgItem, sizeof(msgItem) );
	ITEMMGR->SetDisableDialog(TRUE, eItemTable_Inventory);
	ITEMMGR->SetDisableDialog(TRUE, eItemTable_Pyoguk);
	ITEMMGR->SetDisableDialog(TRUE, eItemTable_MunpaWarehouse);
	if(!IsEmpty(eMixPos_Basic))
	{
		Release(eMixBasicRelease);
	}
	Release(eMixResultRelease);
	AddVirtualItemWrap(eMixPos_Basic, pOrigItem);
	ViewResultItemList(pMixItemInfo);
	m_pMaterialsStatic->SetStaticText( CHATMGR->GetChatMsg(613) );
	return FALSE;
}
void CMixDialog::ViewMaterialsDesc(ITEM_MIX_RES * pResultItem)
{
	if( pResultItem == NULL )
	{
		CHATMGR->AddMsg(CTC_SYSMSG, "return");
		return;
	}
	char line[64] = {0,};
	char szDescText[384] = {0,};
	char buf[61] = { 0, };				   
	CVirtualItem * pBasicItem = (CVirtualItem *)m_pMixDlg->GetIconForIdx(eMixPos_Basic);
	m_pMsgListDlg->RemoveAll();
	m_pMaterialsStatic->SetStaticText(" ");
	if( pBasicItem == NULL )
	{
		CHATMGR->AddMsg(CTC_SYSMSG, "return");
		return;
	}
	if(pBasicItem->GetSrcDurability() >1 )
	{		
		wsprintf( buf, "[%s] X %d", ITEMMGR->GetItemInfo(pResultItem->wResItemIdx)->ItemName, pBasicItem->GetSrcDurability() );
		m_pMsgListDlg->AddItem( buf, RGBA_MAKE(255, 255, 255, 255),-1,FALSE,0);
	}
	else
	{
		wsprintf( buf, "[%s]", ITEMMGR->GetItemInfo(pResultItem->wResItemIdx)->ItemName );
		m_pMsgListDlg->AddItem( buf, RGBA_MAKE(255, 255, 255, 255),-1,FALSE,0);
	}
	
	DWORD dwMoney;
	if( ITEMMGR->IsDupItem( pBasicItem->GetSrcItemIdx() ) )
	{
		dwMoney = pResultItem->Money * pBasicItem->GetSrcDurability();
	}
	else
	{
		dwMoney = pResultItem->Money;
	}
	wsprintf(line, CHATMGR->GetChatMsg(229), AddComma( dwMoney ) );
	m_pMsgListDlg->AddItem( line, RGBA_MAKE(255, 255, 255, 255),-1,FALSE,0);
	wsprintf(line, CHATMGR->GetChatMsg(2540) );	
	m_pMsgListDlg->AddItem( line, RGBA_MAKE(255, 255, 255, 255),-1,FALSE,0);
	for(int i = 0 ; i < pResultItem->wMaterialItemNum ; ++i)
	{																																		   
		wsprintf(line, CHATMGR->GetChatMsg(228), ITEMMGR->GetItemInfo(pResultItem->psMaterialItemInfo[i].wMatItemIdx)->ItemName, pResultItem->psMaterialItemInfo[i].wMatItemNum);
		if(pBasicItem->GetSrcDurability() >1 )
		{
			wsprintf(line, CHATMGR->GetChatMsg(228), ITEMMGR->GetItemInfo(pResultItem->psMaterialItemInfo[i].wMatItemIdx)->ItemName, pResultItem->psMaterialItemInfo[i].wMatItemNum*pBasicItem->GetSrcDurability());												 
		}
		m_pMsgListDlg->AddItem( line, RGBA_MAKE(255, 255, 255, 255),-1,FALSE,0);
	}
	float fSuccessRate=pResultItem->SuccessRatio;
	sprintf(m_Ratio,CHATMGR->GetChatMsg(2538),(float)fSuccessRate/100);
	if(fSuccessRate==10000)
	{
		m_pSuccessRatio->SetFGColor(0xFFFFFF);
	}
	else
	{
		m_pSuccessRatio->SetFGColor(0xFFFF00);
	}
	m_pArrowStatic->	SetBasicImage(&m_ScrollImg);
	m_pSuccessRatio->	SetStaticText(m_Ratio);
	m_pMaterialsStatic->SetStaticText(szDescText);
}
void CMixDialog::ViewResultItemList(const ITEM_MIX_INFO * pMixItemInfo)
{//select which item you want
	if(!pMixItemInfo) return;
	DeleteViewResultItemAll();

	if( pMixItemInfo == NULL )
	{
		CHATMGR->AddMsg(CTC_SYSMSG, "return");
		return;
	}
	
	DURTYPE Dur = m_VirtualItem[eMixPos_Basic].GetLinkItem()->GetDurability();

	for(int i = 0 ; i < pMixItemInfo->wResultItemNum ; ++i)
	{
		m_ViewShowItem[i].SetData(pMixItemInfo->psResultItemInfo[i].wResItemIdx);
		m_ViewShowItem[i].Init(pMixItemInfo->psResultItemInfo[i].wResItemIdx, Dur);
		//ITEMMGR->SetToolTipIconMix( (cIcon*)&m_ViewShowItem[i], NULL, ITEMMGR->GetItemRareOption(m_VirtualItem[eMixPos_Basic].GetRareness()),0 );

		ITEMMGR->SetToolTipIcon( (cIcon*)&m_ViewShowItem[i],
			NULL,
			ITEMMGR->GetItemRareOption(m_VirtualItem[eMixPos_Basic].GetRareness()),
			0,
			NULL,
			0,
			1
			);

		m_pMixGridDlg->AddIcon(i, (cIcon*)&m_ViewShowItem[i]);
	}
}
void CMixDialog::DeleteViewResultItemAll()
{
	for(int i = 0 ; i < m_pMixGridDlg->GetCellNum() ; ++i )
	{
		CItemShow * pSItemOut = NULL;
		if(m_pMixGridDlg->DeleteIcon(i, (cIcon**)&pSItemOut))
		{
		}
	}
}
BOOL CMixDialog::AddVirtualItemWrap(POSTYPE relPos, CItem * pItem)
{
	if( pItem == NULL )
	{
		CHATMGR->AddMsg(CTC_SYSMSG, "return");
		return false;
	}
	m_VirtualItem[relPos].SetData(pItem->GetItemIdx());
	m_VirtualItem[relPos].SetLinkItem(pItem);
	if( pItem->GetDurability() != 0 && !ITEMMGR->IsDupItem(pItem->GetItemIdx()) )
	{
		ITEMMGR->SetToolTipIcon( (cIcon*)&m_VirtualItem[relPos],
			ITEMMGR->GetItemOption(pItem->GetDurability()),
			ITEMMGR->GetItemRareOption(pItem->GetRareness()),
			0,
			NULL,
			0,
			1
			);

		//ITEMMGR->SetToolTipIconMix((cIcon*)&m_VirtualItem[relPos], 
		//	ITEMMGR->GetItemOption(pItem->GetDurability()),
		//	ITEMMGR->GetItemRareOption(pItem->GetRareness()),
		//	0
		//	);
	}	
	else
	{
		//ITEMMGR->SetToolTipIconMix((cIcon*)&m_VirtualItem[relPos], 
		//	NULL,
		//	ITEMMGR->GetItemRareOption(pItem->GetRareness()),
		//	0
		//	);

		ITEMMGR->SetToolTipIcon( (cIcon*)&m_VirtualItem[relPos],
			NULL,
			ITEMMGR->GetItemRareOption(pItem->GetRareness()),
			0,
			0,
			0,
			1
			);
	}
	return AddVirtualItem(relPos, &m_VirtualItem[relPos]);
}
BOOL CMixDialog::AddVirtualItem(POSTYPE relPos, CVirtualItem * pItem)
{
	if(m_pMixDlg->IsAddable(relPos))
	{
		m_pMixDlg->AddIcon(relPos, (cIcon *)pItem);
		return TRUE;
	}
	return FALSE;
}
CVirtualItem * CMixDialog::GetVirtualItem(POSTYPE relPos)
{
	return (CVirtualItem *)m_pMixDlg->GetIconForIdx(relPos);
}
void CMixDialog::MixOkBtn(CMixDialog * pThis)
{
	if( pThis->IsNowMixing() == TRUE )
		return;
	CVirtualItem * pBasicItem = (CVirtualItem *)pThis->m_pMixDlg->GetIconForIdx(eMixPos_Basic);
	if(NULL == pBasicItem)
	{
		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(618) );
		return ;
	}
	if(-1 == pThis->m_pMixGridDlg->GetCurSelCellPos())
	{
		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(203) );
		return;
	}
	CItemShow * pSelItemShow = (CItemShow *)pThis->m_pMixGridDlg->GetIconForIdx((WORD)pThis->m_pMixGridDlg->GetCurSelCellPos());
	if(NULL == pSelItemShow)
	{
		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(203) );
		return ;
	}
	const ITEM_MIX_INFO * pMixInfo = MIXMGR->GetMixItemInfo(pBasicItem->GetSrcItemIdx());
	for(int i = 0 ; i < pMixInfo->wResultItemNum ; ++i)
	{
		if(pMixInfo->psResultItemInfo[i].wResItemIdx == pSelItemShow->GetItemIdx())
		{
			MSG_ITEM_MIX_SYN msg;
			msg.Init();
			msg.Category			= MP_ITEM;
			msg.Protocol			= MP_ITEM_MIX_SYN;
			msg.dwObjectID			= HEROID;
			msg.wBasicItemIdx		= pBasicItem->GetSrcItemIdx();
			msg.BasicItemPos		= pBasicItem->GetSrcPosition();
			msg.dwRareIdx			= pBasicItem->GetRareness();
			msg.wResultItemIdx		= pSelItemShow->GetItemIdx();	
			msg.ResultIndex			= i;
			if( pThis->m_bShopItemUse )
			{
				msg.ShopItemIdx = (WORD)pThis->m_ShopItemIdx;
				msg.ShopItemPos = (WORD)pThis->m_ShopItemPos;
			}
			DWORD dwMoney;
			if(ITEMMGR->IsDupItem(pBasicItem->GetSrcItemIdx()))
			{
				dwMoney = pMixInfo->psResultItemInfo[i].Money * pBasicItem->GetSrcDurability();
			}
			else
			{
				dwMoney = pMixInfo->psResultItemInfo[i].Money;
			}
			if( dwMoney > HERO->GetMoney() )
			{
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(117) );
				return ;
			}
			MATERIAL_ARRAY ItemOut[TP_INVENTORY_END-TP_INVENTORY_START];
			
			for(int j = 0 ; j < pMixInfo->psResultItemInfo[i].wMaterialItemNum ; ++j)
			{
				WORD ItemPosNum;
				WORD needItemIdx;
				DURTYPE needItemNum;

				needItemIdx = pMixInfo->psResultItemInfo[i].psMaterialItemInfo[j].wMatItemIdx;
				needItemNum = pMixInfo->psResultItemInfo[i].psMaterialItemInfo[j].wMatItemNum;

				if(ITEMMGR->IsDupItem(pBasicItem->GetSrcItemIdx()))
				{
					needItemNum *= pBasicItem->GetSrcDurability();
				}

				if(!pThis->EnoughMaterial(needItemIdx, (WORD)needItemNum, ItemOut, ItemPosNum) && needItemIdx)
				{
					CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(205) );
					return;
				}
				for(int k = 0 ; k < ItemPosNum ; ++k)
				{
					if(ITEMMGR->IsDupItem(needItemIdx))
						msg.AddMaterial(needItemIdx, ItemOut[k].ItemPos, ItemOut[k].Dur);
					else
						msg.AddMaterial(needItemIdx, ItemOut[k].ItemPos, 0);
				}	
			}
			NETWORK->Send(&msg, msg.GetSize());
			pThis->SetNowMixing( TRUE );
			return;
		}
	}
}
BOOL CMixDialog::EnoughMaterial(WORD wItemIdx, WORD ItemNum, MATERIAL_ARRAY * ItemOut, WORD & ItemPosNum)
{
	ItemPosNum = 0;
	DURTYPE CurRemainNum = ItemNum;
	for(int i = TP_INVENTORY_START ; i < TP_INVENTORY_END && CurRemainNum != 0 ; ++i)
	{
		CItem * pItem = GAMEIN->GetInventoryDialog()->GetItemForPos(i);
		if(pItem && pItem->GetItemIdx() == wItemIdx)
		{
			if(pItem->IsLocked())
				continue;
			if(ITEMMGR->IsDupItem(wItemIdx))
			{
				if(CurRemainNum > pItem->GetDurability())
				{
					ItemOut[ItemPosNum].Dur	= pItem->GetDurability();
					CurRemainNum -= pItem->GetDurability();
				}
				else
				{
					ItemOut[ItemPosNum].Dur	= CurRemainNum;
					CurRemainNum = 0;
				}
				ItemOut[ItemPosNum++].ItemPos	= i;
			}
			else
			{
				ItemOut[ItemPosNum].Dur = 1;
				ItemOut[ItemPosNum++].ItemPos = i;
				--CurRemainNum;
			}
		}
	}
	if(CurRemainNum == 0)
		return TRUE;
	else
		return FALSE;
}
void CMixDialog::MixCancelBtn(CMixDialog * pThis)
{
	pThis->Release();
	pThis->SetActiveRecursive(FALSE);
	CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(206));
}
BOOL CMixDialog::OnActionEvent(LONG lId, void * p, DWORD we)
{
	switch(we)
	{
	case WE_CLOSEWINDOW:
		{
			CMixDialog * pMixDlg = (CMixDialog *)WINDOWMGR->GetWindowForID(lId);
			if( pMixDlg->m_bShopItemUse )
			{
				CItem* pItem = ITEMMGR->GetItemofTable( eItemTable_ShopInven, (POSTYPE)pMixDlg->m_ShopItemPos );
				if( pItem )
					pItem->SetLock( FALSE );
				pMixDlg->m_bShopItemUse	= FALSE;
				pMixDlg->m_ShopItemIdx	= 0;
				pMixDlg->m_ShopItemPos	= 0;
			}
			pMixDlg->Release();

			pMixDlg->SetActiveRecursive(FALSE);
			CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(206));

			//CMixDialog * pMixDlg2 = (CMixDialog *)p;
			//CMixDialog::MixCancelBtn(pMixDlg2);
			return TRUE;
		}
		break;
	}
	switch( lId )
	{
	case ITMD_MIXICONGRIDDLG:
		{
			cIconGridDialog* pMixGridDlg = (cIconGridDialog *)((cDialog*)p)->GetWindowForID(lId);	
			if(we == WE_LBTNCLICK)
			{
				CMixDialog * pPar = (CMixDialog *)pMixGridDlg->GetParent();
				LONG SelectPos = pMixGridDlg->GetCurSelCellPos();
				CItemShow * pSelItemShow = (CItemShow *)pMixGridDlg->GetIconForIdx((WORD)SelectPos);
				if(NULL == pSelItemShow)
				{
					pPar->m_pMaterialsStatic->SetStaticText(" ");
					pPar->m_pSuccessRatio->SetStaticText(" ");
					pPar->m_pMsgListDlg->RemoveAll();
					pPar->m_pMixGridDlg->SetCurSelCellPos(-1);

					//pPar->ResetCurSelCellPos();
					return FALSE;
				}
				CVirtualItem * pBasicItem = (CVirtualItem *)pPar->m_pMixDlg->GetIconForIdx(eMixPos_Basic);
				const ITEM_MIX_INFO * pMixInfo = MIXMGR->GetMixItemInfo(pBasicItem->GetSrcItemIdx());
				if(!pMixInfo) 
				{
					//pPar->ResetCurSelCellPos();
					return FALSE;
				}
				pPar->ViewMaterialsDesc(&pMixInfo->psResultItemInfo[SelectPos]);

				/*CHATMGR->AddMsg( CTC_SYSMSG, "MIX:%d wResItemIdx:%d   pMixInfo1:%d pMixInfo2:%d",
					pBasicItem->GetSrcItemIdx(),
					pMixInfo->psResultItemInfo[SelectPos].wResItemIdx,
					pMixInfo->psResultItemInfo[SelectPos].psMaterialItemInfo[0].wMatItemIdx ,
					pMixInfo->psResultItemInfo[SelectPos].psMaterialItemInfo[1].wMatItemIdx);*/
			}
		}
		break;
	case ITMD_MIXOKBTN:
		{
			CMixDialog * pMixDlg = (CMixDialog *)p;
			CMixDialog::MixOkBtn(pMixDlg);
		}
		break;
	case ITMD_MIXCANCELBTN:
		{
			CMixDialog * pMixDlg = (CMixDialog *)p;
			CMixDialog::MixCancelBtn(pMixDlg);
		}
		break;
	}
	return TRUE;
}
DWORD CMixDialog::ActionEvent(CMouse * mouseInfo)
{
	DWORD we = WE_NULL;
	if( !m_bActive ) return we;
	we |= cDialog::ActionEvent(mouseInfo);
	for(int i = 0 ; i < eMixPos_Max ; ++i)
	{
		m_VirtualItem[i].ActionEvent(mouseInfo);
	}
	return we;
}
void CMixDialog::ResetCurSelCellPos()
{
	m_pMixGridDlg->SetCurSelCellPos(-1);
	m_bMixing = FALSE;
}
BOOL CMixDialog::AddRealItem( CItem *pOrigItem )
{
	return TRUE;
}
void CMixDialog::DieCheck()
{
	if( HERO->GetState() == eObjectState_Die )
	{
		Release();
		SetActiveRecursive( FALSE );
	}
}
void CMixDialog::SetShopItemInfo( DWORD ShopItemIdx, DWORD ShopItemPos )
{
	m_ShopItemIdx = ShopItemIdx;
	m_ShopItemPos = ShopItemPos;
	m_bShopItemUse = TRUE;
}