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
	m_pItemQualityText = (cStatic*)GetWindowForID(ITEM_QUALITY_TEXT_STATIC); //  ��Ҫ�ӵ� Static �ؼ� ID

}

BOOL CItemQualityChangeDlg::FakeMoveIcon( LONG x,LONG y,cIcon * pIcon )
{
	if(pIcon->GetType() != WT_ITEM) return FALSE;
	if( m_bDisable ) return FALSE;

	CItem* pOrigItem = (CItem*)pIcon;

	if( pOrigItem->IsLocked() )	return FALSE;//�����Ʒ������

	ITEM_INFO* pItemInfo = ITEMMGR->GetItemInfo( pOrigItem->GetItemIdx() );
	
	if( !pItemInfo )			return FALSE;//�����Ʒ��Ϣ��ȡʧ��

	if(!ITEMMGR->IsEqualTableIdxForPos(eItemTable_Inventory, pOrigItem->GetPosition()))//�ж���Ʒλ��
		return FALSE;
	
	ITEM_INFO* pShopItem = ITEMMGR->GetItemInfo( m_ItemIdx );
	if( !pShopItem )
		return FALSE;

	eITEM_KINDBIT bits = pOrigItem->GetItemKind();
	WORD quality = pOrigItem->GetQuality();
	if(m_VirtualItemEquip.GetData()==0)
	{
		if (!(bits >= eEQUIP_ITEM_WEAPON&&bits <= eEQUIP_ITEM_UNIQUE))
		{//��ӵ���Ʒ�Ƿ���װ������
			CHATMGR->AddMsg(CTC_SYSMSG, "ֻ��װ����Ʒ����ת��Ʒ��");
			return FALSE;
		}
		if( pItemInfo->LimitLevel < pShopItem->GenGol || pItemInfo->LimitLevel > pShopItem->MinChub )
		{//��װ����ת��ʯ�ȼ����
			CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1082) );
			return FALSE;
		}
		if(pItemInfo->ItemKind!=pShopItem->SellPrice||pItemInfo->ItemKind==2100&&pShopItem->BuyPrice!=2100)
		{//��װ����ת��ʯ���ͼ��
			CHATMGR->AddMsg( CTC_SYSMSG, "ת��װ����ת��ʯ���Ͳ���");
			return FALSE;
		}
		if (quality == 4)
		{
			CHATMGR->AddMsg(CTC_SYSMSG, "��װ��������Ʒ�ʣ��޷��ٽ��о��ѣ�");
			return FALSE;
		}


		AddVirtualItem( pOrigItem );
		pOrigItem->SetLock(TRUE);
	}
	else
	{
		CHATMGR->AddMsg( CTC_SYSMSG,"����װ���������ظ����ã�" );
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
	msg.ItemQualityIdx= m_ItemIdx;//ת��ʯ
	msg.ItemQualityPos= m_ItemPos;//ת��ʯλ��
	msg.ItemTargetIdx = m_VirtualItemEquip.GetLinkItem()->GetItemIdx();//��װ����Ʒ
	msg.ItemTargetPos = m_VirtualItemEquip.GetLinkItem()->GetPosition();//��װ��λ��

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

	// ���� tooltip ��Ϣ��ˢ��Ʒ��/���Ե���ʾ��
	ITEMMGR->SetToolTipIcon((cIcon*)&m_VirtualItemEquip,
		ITEMMGR->GetItemOption(pItem->GetDurability()),
		ITEMMGR->GetItemRareOption(pItem->GetRareness()), 0);

	// ��������ͼ��
	m_pIconDlgEquip->SetDisable(FALSE);

	//  ������������ֹ�������϶�
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
	// ���� UI �е�������Ʒ��ָ��ȷ�� tooltip ��Ʒ���ı��ܶ�����������
	m_VirtualItemEquip.SetLinkItem(pItem);

	// ˢ����ʾ��tooltip ��Ʒ���ı���
	RefreshVirtualEquipIcon();
	UpdateItemQualityText(pItem->GetQuality());
}
void CItemQualityChangeDlg::UpdateItemQualityText(WORD quality)
{
	if (!m_pItemQualityText) return;

	const char* szText = "";

	switch (quality)
	{
	case 0: szText = "��ͨ"; break;
	case 1: szText = "����"; break;
	case 2: szText = "����"; break;
	case 3: szText = "��˵"; break;
	case 4: szText = "��"; break;
	default: szText = "δ֪"; break;
	}
	char line[128];
	sprintf(line, "��ǰƷ�ʣ�%s ", szText);
	m_pItemQualityText->SetStaticText(line);
}
