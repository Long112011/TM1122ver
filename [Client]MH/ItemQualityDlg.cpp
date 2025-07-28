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
	sprintf(line,"�ɹ��ʣ�%.1f%%",(float)(val*1.0f));
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

	// ��װ������
	if (m_VirtualItemEquip.GetData() == 0)
	{
		if (!(pOrigItem->GetItemKind() >= eEQUIP_ITEM_WEAPON && pOrigItem->GetItemKind() <= eEQUIP_ITEM_UNIQUE))
		{
			CHATMGR->AddMsg(CTC_SYSMSG, "ֻ��װ����Ʒ���ܾ���/ϴ����");
			return FALSE;
		}
		if (quality != 4)
		{
			CHATMGR->AddMsg(CTC_SYSMSG, "��װ������ΪƷ��4�����񻰣���");
			return FALSE;
		}
		AddVirtualItem(pOrigItem);
		pOrigItem->SetLock(TRUE);
	}
	else if (m_VirtualItemExtraEquip.GetData() == 0)
	{
		// 1�����ж��Ƿ�Ϊ�Ϸ�����
		bool bIsValidStone = (materialID == STONEJUEXING || materialID == STONECHANGE100 || materialID == STONEENTRY1 || materialID == STONEENTRY2);
		bool bIsSameEquip = (pOrigItem->GetItemKind() >= eEQUIP_ITEM_WEAPON && pOrigItem->GetItemKind() <= eEQUIP_ITEM_UNIQUE &&
			m_VirtualItemEquip.GetSrcItemIdx() == pOrigItem->GetItemIdx() && quality >= 2);

		// 2���������Ʋ�������Ϊ1
		if (pOrigItem->GetDurability() > 1)
		{
			CHATMGR->AddMsg(CTC_SYSMSG, "ֻ�ܷ��� 1 �����ϣ��޷�ʹ�öѵ���Ʒ��");
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
			CHATMGR->AddMsg(CTC_SYSMSG, "��Ч�Ĳ��ϣ���ʹ�þ���ʯ��ϴ��ʯ��ͬ��������");
			return FALSE;
		}
	}
	else
	{
		CHATMGR->AddMsg(CTC_SYSMSG, "���в��ϣ������Ƴ���");
		return FALSE;
	}


	// �����������
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
		if(m_VirtualItemExtraEquip.GetData()==0)//����װ����û����Ʒ
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
	msg.ItemQualityIdx= m_ItemIdx;//����ʯ
	msg.ItemQualityPos= m_ItemPos;//����ʯλ��
	msg.ItemTargetIdx = m_VirtualItemEquip.GetLinkItem()->GetItemIdx();//��װ����Ʒ
	msg.ItemTargetPos = m_VirtualItemEquip.GetLinkItem()->GetPosition();//��װ��λ��

	msg.ItemExtraTargetIdx = m_VirtualItemExtraEquip.GetLinkItem()->GetItemIdx();//����װ������Ʒ
	msg.ItemExtraTargetPos =m_VirtualItemExtraEquip.GetLinkItem()->GetPosition();//����װ��λ��
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
