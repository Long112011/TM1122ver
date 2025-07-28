#include "stdafx.h"
#include "ItemQualityDlg.h"
#include "WindowIDEnum.h"
#include "./Interface/cIconDialog.h"
#include "./Interface/cWindowManager.h"
#include "./Interface/cStatic.h"
#include "./Interface/cIcon.h"
#include "ChatManager.h"
#include "ObjectStateManager.h"
#include "ObjectManager.h"
#include "ItemManager.h"
CItemQualityDlg::CItemQualityDlg()
{
	pDlg = NULL;
	m_pIconDlgEquip = NULL;
	m_pIconDlgExtraEquip = NULL;
	m_pIconDlgEquipExt = NULL;
	m_ItemRand = NULL;
}

CItemQualityDlg::~CItemQualityDlg()
{

}

void CItemQualityDlg::Linking()
{
	pDlg = WINDOWMGR->GetWindowForID( ITEM_QUALITYDLG );
	m_pIconDlgEquip  = (cIconDialog*)GetWindowForID(ITEM_QUALITY_EQUIPICON);
	m_pIconDlgExtraEquip = (cIconDialog*)GetWindowForID(ITEM_QUALITY_EXTRAEQUIPICON);
	m_pIconDlgEquipExt  = (cIconDialog*)GetWindowForID(ITEM_QUALITY_EQUIPICONEXT);
	m_ItemRand = (cStatic*)GetWindowForID(ITEM_QUALITY_RAND);
	SetItemQualityRand(0);
}
void CItemQualityDlg::SetItemQualityRand(WORD val)
{
	char line[128];
	sprintf(line,"成功率：%.1f%%",(float)(val*1.0f));
	m_ItemRand->SetStaticText(line);
}
BOOL CItemQualityDlg::FakeMoveIcon(LONG x, LONG y, cIcon* pIcon)
{
	if (pIcon->GetType() != WT_ITEM || m_bDisable)
		return FALSE;

	CItem* pOrigItem = (CItem*)pIcon;
	if (pOrigItem->IsLocked()) return FALSE;

	ITEM_INFO* pItemInfo = ITEMMGR->GetItemInfo(pOrigItem->GetItemIdx());
	if (!pItemInfo) return FALSE;
	if (!ITEMMGR->IsEqualTableIdxForPos(eItemTable_Inventory, pOrigItem->GetPosition()))
		return FALSE;

	WORD materialID = pOrigItem->GetItemIdx();
	WORD quality = pOrigItem->GetQuality();

	// 主装备放入
	if (m_VirtualItemEquip.GetData() == 0)
	{
		if (!(pOrigItem->GetItemKind() >= eEQUIP_ITEM_WEAPON && pOrigItem->GetItemKind() <= eEQUIP_ITEM_UNIQUE))
		{
			CHATMGR->AddMsg(CTC_SYSMSG, "只有装备物品才能觉醒/洗练！");
			return FALSE;
		}
		if (quality != 4)
		{
			CHATMGR->AddMsg(CTC_SYSMSG, "主装备必须为品质4（·神话）！");
			return FALSE;
		}
		AddVirtualItem(pOrigItem);
		pOrigItem->SetLock(TRUE);
	}
	else if (m_VirtualItemExtraEquip.GetData() == 0)
	{
		// 1）先判断是否为合法材料
		bool bIsValidStone = (materialID == STONEJUEXING || materialID == STONECHANGE100 || materialID == STONEENTRY1 || materialID == STONEENTRY2);
		bool bIsSameEquip = (pOrigItem->GetItemKind() >= eEQUIP_ITEM_WEAPON && pOrigItem->GetItemKind() <= eEQUIP_ITEM_UNIQUE &&
			m_VirtualItemEquip.GetSrcItemIdx() == pOrigItem->GetItemIdx() && quality >= 2);

		// 2）必须限制材料数量为1
		if (pOrigItem->GetDurability() > 1)
		{
			CHATMGR->AddMsg(CTC_SYSMSG, "只能放入 1 个材料，无法使用堆叠物品！");
			return FALSE;
		}

		if (bIsValidStone)
		{
			AddVirtualItem(pOrigItem);
			pOrigItem->SetLock(TRUE);
			SetItemQualityRand(100.0f);
		}
		else if (bIsSameEquip)
		{
			AddVirtualItem(pOrigItem);
			pOrigItem->SetLock(TRUE);

			if (quality == 4)
				SetItemQualityRand(100.0f);
			else if (quality == 3)
				SetItemQualityRand(60.0f);
			else if (quality == 2)
				SetItemQualityRand(30.0f);
		}
		else
		{
			CHATMGR->AddMsg(CTC_SYSMSG, "无效的材料，请使用觉醒石、洗练石或同类武器！");
			return FALSE;
		}
	}
	else
	{
		CHATMGR->AddMsg(CTC_SYSMSG, "已有材料，请先移除！");
		return FALSE;
	}


	// 锁定其他面板
	ITEMMGR->SetDisableDialog(TRUE, eItemTable_Pyoguk);
	ITEMMGR->SetDisableDialog(TRUE, eItemTable_GuildWarehouse);
	ITEMMGR->SetDisableDialog(TRUE, eItemTable_Shop);
	ITEMMGR->SetDisableDialog(TRUE, eItemTable_PetInven);

	return FALSE;
}

void CItemQualityDlg::AddVirtualItem(CItem * pItem)
{
	if (m_VirtualItemEquipExt.GetData()!=0)
	{
		ReleaseExt();
	}
	if(m_VirtualItemEquip.GetData()==0)
	{
		cIcon* pIcon=(cIcon*)pItem;
		m_VirtualItemEquip.SetData( pItem->GetItemIdx());
		m_VirtualItemEquip.SetLinkItem(pItem);
		m_pIconDlgEquip->AddIcon( 0, (cIcon*)&m_VirtualItemEquip );
		ITEMMGR->SetToolTipIcon((cIcon*)&m_VirtualItemEquip,ITEMMGR->GetItemOption(pItem->GetDurability()),ITEMMGR->GetItemRareOption(pItem->GetRareness()),0);
		m_pIconDlgEquip->SetDisable(TRUE);
	}
	else
	{
		if(m_VirtualItemExtraEquip.GetData()==0)//材料装备栏没有物品
		{
			cIcon* pIcon=(cIcon*)pItem;
			m_VirtualItemExtraEquip.SetData( pItem->GetItemIdx());
			m_VirtualItemExtraEquip.SetLinkItem(pItem);
			m_pIconDlgExtraEquip->AddIcon( 0, (cIcon*)&m_VirtualItemExtraEquip );
			m_pIconDlgExtraEquip->SetDepend(TRUE);
			ITEMMGR->SetToolTipIcon((cIcon*)&m_VirtualItemExtraEquip,ITEMMGR->GetItemOption(pItem->GetDurability()),ITEMMGR->GetItemRareOption(pItem->GetRareness()),0);
			m_pIconDlgExtraEquip->SetDisable(TRUE);

			if (pItem->GetQuality()==4)
				SetItemQualityRand(100);
			if (pItem->GetQuality()==3)
				SetItemQualityRand(30);
		}
	}
}
void CItemQualityDlg::AddVirtualItemExt(CItem * pItem)
{
	if(m_VirtualItemEquipExt.GetData()==0)
	{
		cIcon* pIcon=(cIcon*)pItem;
		m_VirtualItemEquipExt.SetData( pItem->GetItemIdx());
		m_VirtualItemEquipExt.SetLinkItem(pItem);
		m_pIconDlgEquipExt->AddIcon( 0, (cIcon*)&m_VirtualItemEquipExt );
		m_pIconDlgEquipExt->SetDisable(TRUE);
		ITEMMGR->SetToolTipIcon((cIcon*)&m_VirtualItemEquipExt,ITEMMGR->GetItemOption(pItem->GetDurability()),ITEMMGR->GetItemRareOption(pItem->GetRareness()),0);
	}
	else
	{
		ReleaseExt();
		cIcon* pIcon=(cIcon*)pItem;
		m_VirtualItemEquipExt.SetData( pItem->GetItemIdx());
		m_VirtualItemEquipExt.SetLinkItem(pItem);
		m_pIconDlgEquipExt->AddIcon( 0, (cIcon*)&m_VirtualItemEquipExt );
		m_pIconDlgEquipExt->SetDisable(TRUE);
		ITEMMGR->SetToolTipIcon((cIcon*)&m_VirtualItemEquipExt,ITEMMGR->GetItemOption(pItem->GetDurability()),ITEMMGR->GetItemRareOption(pItem->GetRareness()),0);
	}
}
void CItemQualityDlg::SendItemQualityMsg()
{
	if(!m_VirtualItemEquip.GetLinkItem())	return;
	if(!m_VirtualItemExtraEquip.GetLinkItem()) return;

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
	msg.Protocol = MP_ITEMEXT_QUALITY_SYN;
	msg.dwObjectID = HEROID;
	msg.ItemQualityIdx= m_ItemIdx;//觉醒石
	msg.ItemQualityPos= m_ItemPos;//觉醒石位置
	msg.ItemTargetIdx = m_VirtualItemEquip.GetLinkItem()->GetItemIdx();//主装备物品
	msg.ItemTargetPos = m_VirtualItemEquip.GetLinkItem()->GetPosition();//主装备位置

	msg.ItemExtraTargetIdx = m_VirtualItemExtraEquip.GetLinkItem()->GetItemIdx();//材料装备备物品
	msg.ItemExtraTargetPos =m_VirtualItemExtraEquip.GetLinkItem()->GetPosition();//材料装备位置
	NETWORK->Send( &msg, sizeof(msg) );
	//SetActive( FALSE );
}
void CItemQualityDlg::ReleaseExt()
{
	CVirtualItem* pVItem = NULL;
	if( m_pIconDlgEquipExt )
	{
		m_pIconDlgEquipExt->DeleteIcon( 0, (cIcon**)&pVItem );
		if( pVItem )
			pVItem->GetLinkItem()->SetLock( FALSE );

		m_VirtualItemEquipExt.SetLinkItem( NULL );
		m_VirtualItemEquipExt.SetData(0);
	}
}
void CItemQualityDlg::Release(int val)
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
	if(m_pIconDlgExtraEquip)
	{
		if(m_VirtualItemExtraEquip.GetData()!=0)
		{
			m_pIconDlgExtraEquip->DeleteIcon( 0, (cIcon**)&pVItem );
			if( pVItem )
				pVItem->GetLinkItem()->SetLock( FALSE );
			m_VirtualItemExtraEquip.SetLinkItem( NULL );
			m_VirtualItemExtraEquip.SetData(0);
		}
	}
	CItem* pOriItem = ITEMMGR->GetItemofTable( eItemTable_ShopInven, (POSTYPE)m_ItemPos );
	if (val==0)
	{
		if( pOriItem )
		{
			pOriItem->SetLock( TRUE );
		}
		else
		{
			ReleaseExt();
			SetActive(FALSE);
		}
	}
	else
	{
		pOriItem->SetLock( FALSE );
	}
	ITEMMGR->SetDisableDialog(FALSE, eItemTable_Inventory);
	ITEMMGR->SetDisableDialog(FALSE, eItemTable_Pyoguk);
	ITEMMGR->SetDisableDialog(FALSE, eItemTable_GuildWarehouse);
	ITEMMGR->SetDisableDialog(FALSE, eItemTable_Shop);
	ITEMMGR->SetDisableDialog(FALSE, eItemTable_PetInven);

	OBJECTSTATEMGR->EndObjectState( HERO, eObjectState_Deal );

	SetItemQualityRand(0);
	//SetActive( FALSE );
}
