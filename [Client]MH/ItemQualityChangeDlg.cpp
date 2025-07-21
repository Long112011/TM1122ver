#include "stdafx.h"
#include "ItemQualityChangeDlg.h"
#include "WindowIDEnum.h"
#include "./Interface/cIconDialog.h"
#include "./Interface/cWindowManager.h"
#include "./Interface/cStatic.h"
#include "./Interface/cIcon.h"
#include "ChatManager.h"
#include "ObjectStateManager.h"
#include "ObjectManager.h"
#include "ItemManager.h"
CItemQualityChangeDlg::CItemQualityChangeDlg()
{
	pDlg = NULL;
	m_pIconDlgEquip = NULL;
	m_pItemQualityText= NULL;
}

CItemQualityChangeDlg::~CItemQualityChangeDlg()
{

}

void CItemQualityChangeDlg::Linking()
{
	pDlg = WINDOWMGR->GetWindowForID( ITEM_QUALITYCHANGEDLG );
	m_pIconDlgEquip  = (cIconDialog*)GetWindowForID(ITEM_QUALITY_EQUIPICONCHANGE);
	m_pItemQualityText = (cStatic*)GetWindowForID(ITEM_QUALITY_TEXT_STATIC); //  你要加的 Static 控件 ID

}

BOOL CItemQualityChangeDlg::FakeMoveIcon( LONG x,LONG y,cIcon * pIcon )
{
	if(pIcon->GetType() != WT_ITEM) return FALSE;
	if( m_bDisable ) return FALSE;

	CItem* pOrigItem = (CItem*)pIcon;

	if( pOrigItem->IsLocked() )	return FALSE;//添加物品已锁定

	ITEM_INFO* pItemInfo = ITEMMGR->GetItemInfo( pOrigItem->GetItemIdx() );
	
	if( !pItemInfo )			return FALSE;//添加物品信息获取失败

	if(!ITEMMGR->IsEqualTableIdxForPos(eItemTable_Inventory, pOrigItem->GetPosition()))//判断物品位置
		return FALSE;
	
	ITEM_INFO* pShopItem = ITEMMGR->GetItemInfo( m_ItemIdx );
	if( !pShopItem )
		return FALSE;

	eITEM_KINDBIT bits = pOrigItem->GetItemKind();
	WORD quality = pOrigItem->GetQuality();
	if(m_VirtualItemEquip.GetData()==0)
	{
		if (!(bits >= eEQUIP_ITEM_WEAPON&&bits <= eEQUIP_ITEM_UNIQUE))
		{//添加的物品是否是装备类型
			CHATMGR->AddMsg(CTC_SYSMSG, "只有装备物品才能转换品质");
			return FALSE;
		}
		if( pItemInfo->LimitLevel < pShopItem->GenGol || pItemInfo->LimitLevel > pShopItem->MinChub )
		{//主装备和转换石等级检查
			CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1082) );
			return FALSE;
		}
		if(pItemInfo->ItemKind!=pShopItem->SellPrice||pItemInfo->ItemKind==2100&&pShopItem->BuyPrice!=2100)
		{//主装备和转换石类型检查
			CHATMGR->AddMsg( CTC_SYSMSG, "转换装备与转换石类型不符");
			return FALSE;
		}
		if (quality == 4)
		{
			CHATMGR->AddMsg(CTC_SYSMSG, "此装备已是神话品质，无法再进行觉醒！");
			return FALSE;
		}


		AddVirtualItem( pOrigItem );
		pOrigItem->SetLock(TRUE);
	}
	else
	{
		CHATMGR->AddMsg( CTC_SYSMSG,"已有装备，不能重复放置！" );
		return FALSE;
	}
	ITEMMGR->SetDisableDialog(TRUE, eItemTable_Inventory);
	ITEMMGR->SetDisableDialog(TRUE, eItemTable_Pyoguk);
	ITEMMGR->SetDisableDialog(TRUE, eItemTable_GuildWarehouse);
	ITEMMGR->SetDisableDialog(TRUE, eItemTable_Shop);
	ITEMMGR->SetDisableDialog(TRUE, eItemTable_PetInven);

	return FALSE;
}
void CItemQualityChangeDlg::AddVirtualItem(CItem * pItem)
{
	cIcon* pIcon=(cIcon*)pItem;
	m_VirtualItemEquip.SetData( pItem->GetItemIdx());
	m_VirtualItemEquip.SetLinkItem(pItem);
	m_pIconDlgEquip->AddIcon( 0, (cIcon*)&m_VirtualItemEquip );
	ITEMMGR->SetToolTipIcon((cIcon*)&m_VirtualItemEquip,ITEMMGR->GetItemOption(pItem->GetDurability()),ITEMMGR->GetItemRareOption(pItem->GetRareness()),0);
	m_pIconDlgEquip->SetDisable(TRUE);
	UpdateItemQualityText(pItem->GetQuality());
}
void CItemQualityChangeDlg::SendItemQualityMsg()
{
	if(!m_VirtualItemEquip.GetLinkItem())	return;

	CItem* pItem = ITEMMGR->GetItem( m_VirtualItemEquip.GetLinkItem()->GetDBIdx());
	if(!pItem)
	{
		if( m_pIconDlgEquip )
		{
			CVirtualItem* pVItem = NULL;
			m_pIconDlgEquip->DeleteIcon( 0, (cIcon**)&pVItem );
			if( pVItem )
				pVItem->GetLinkItem()->SetLock( FALSE );
		}
		m_ItemIdx = 0;
		m_ItemPos = 0;
		ITEMMGR->SetDisableDialog(FALSE, eItemTable_Inventory);
		ITEMMGR->SetDisableDialog(FALSE, eItemTable_Pyoguk);
		ITEMMGR->SetDisableDialog(FALSE, eItemTable_GuildWarehouse);
		ITEMMGR->SetDisableDialog(FALSE, eItemTable_Shop);
		ITEMMGR->SetDisableDialog(FALSE, eItemTable_PetInven);
		OBJECTSTATEMGR->EndObjectState( HERO, eObjectState_Deal );
		return;
	}
	MSG_ITEM_QUALITY_MSG	msg;
	msg.Category = MP_ITEMEXT;
	msg.Protocol = MP_ITEMEXT_QUALITYCHANGE_SYN;
	msg.dwObjectID = HEROID;
	msg.ItemQualityIdx= m_ItemIdx;//转换石
	msg.ItemQualityPos= m_ItemPos;//转换石位置
	msg.ItemTargetIdx = m_VirtualItemEquip.GetLinkItem()->GetItemIdx();//主装备物品
	msg.ItemTargetPos = m_VirtualItemEquip.GetLinkItem()->GetPosition();//主装备位置

	NETWORK->Send( &msg, sizeof(msg) );
	//SetActive( FALSE );
}
void CItemQualityChangeDlg::RefreshVirtualEquipIcon()
{
	CBaseItem* pBase = m_VirtualItemEquip.GetLinkItem();
	if (!pBase)
		return;

	CItem* pItem = dynamic_cast<CItem*>(pBase);
	if (!pItem)
		return;

	// 更新 tooltip 信息（刷新品质/属性等显示）
	ITEMMGR->SetToolTipIcon((cIcon*)&m_VirtualItemEquip,
		ITEMMGR->GetItemOption(pItem->GetDurability()),
		ITEMMGR->GetItemRareOption(pItem->GetRareness()), 0);

	// 重新启用图标
	m_pIconDlgEquip->SetDisable(FALSE);

	//  重新锁定，防止操作中拖动
	pItem->SetLock(TRUE);
}



void CItemQualityChangeDlg::Release()
{
	CVirtualItem* pVItem = NULL;
	if( m_pIconDlgEquip )
	{

		m_pIconDlgEquip->DeleteIcon( 0, (cIcon**)&pVItem );
		if( pVItem )
			pVItem->GetLinkItem()->SetLock( FALSE );

		m_VirtualItemEquip.SetLinkItem( NULL );
		m_VirtualItemEquip.SetData(0);

	}
	CItem* pOriItem = ITEMMGR->GetItemofTable( eItemTable_ShopInven, (POSTYPE)m_ItemPos );
	if( pOriItem )
		pOriItem->SetLock( FALSE );

	m_ItemIdx = 0;
	m_ItemPos = 0;
	ITEMMGR->SetDisableDialog(FALSE, eItemTable_Inventory);
	ITEMMGR->SetDisableDialog(FALSE, eItemTable_Pyoguk);
	ITEMMGR->SetDisableDialog(FALSE, eItemTable_GuildWarehouse);
	ITEMMGR->SetDisableDialog(FALSE, eItemTable_Shop);
	ITEMMGR->SetDisableDialog(FALSE, eItemTable_PetInven);

	OBJECTSTATEMGR->EndObjectState( HERO, eObjectState_Deal );
	m_pItemQualityText = NULL;
	SetActive( FALSE );
}
void CItemQualityChangeDlg::OnItemQualityChanged(CItem* pItem)
{
	// 更新 UI 中的虚拟物品的指向，确保 tooltip 和品质文本能读到最新数据
	m_VirtualItemEquip.SetLinkItem(pItem);

	// 刷新显示（tooltip 和品质文本）
	RefreshVirtualEquipIcon();
	UpdateItemQualityText(pItem->GetQuality());
}
void CItemQualityChangeDlg::UpdateItemQualityText(WORD quality)
{
	if (!m_pItemQualityText) return;

	const char* szText = "";

	switch (quality)
	{
	case 0: szText = "普通"; break;
	case 1: szText = "优秀"; break;
	case 2: szText = "精良"; break;
	case 3: szText = "传说"; break;
	case 4: szText = "神话"; break;
	default: szText = "未知"; break;
	}
	char line[128];
	sprintf(line, "当前品质：%s ", szText);
	m_pItemQualityText->SetStaticText(line);
}
