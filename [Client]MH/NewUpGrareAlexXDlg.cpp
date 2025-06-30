#include "Stdafx.h"
#include "NewUpGrareAlexXDlg.h"
#include "WindowIDEnum.h"
#include "./Interface/cIconDialog.h"
#include "./Interface/cWindowManager.h"
#include "./Interface/cStatic.h"
#include "./Interface/cIcon.h"
#include "./Interface/cGuage.h"
#include "./Interface/cButton.h"
#include "ObjectManager.h"
#include "ChatManager.h"
#include "cCheckBox.h"
#include "ItemManager.h"
#include "ObjectStateManager.h"
#include "ObjectManager.h"
#include "GameIn.h"
#include "InventoryExDialog.h"
#include "MHFile.h"
#include "./ObjectGuagen.h"
#include "QuickManager.h"
#include "./Interface/cScriptManager.h"
#include "StatsCalcManager.h"
#include "CharacterDialog.h"
#include "AppearanceManager.h"
#include "NewUpGrareAlexXC_MGR.h"

/*
1.5
20
225
2500
*/

CNewUpGrareAlexXDlg::CNewUpGrareAlexXDlg()
{
	m_Dlg = NULL;
	m_UseRedStone = NULL;
	m_pIconDlgEquip = NULL;
	m_pIconDlgEquip2 = NULL;
	REAL_MATERIAL_COUNT_01 = 0;
	REAL_MATERIAL_COUNT_02 = 0;
	REAL_MATERIAL_COUNT_03 = 0;
	REAL_MATERIAL_COUNT_04 = 0;
	real_lv_item = 0;
	real_fullPercen = 0;
	real_percent_total = 0;
	real_r_img = FALSE;
	realFuck_WTF_GradeAlexX = false;
	real_money_total = 0;
}

CNewUpGrareAlexXDlg::~CNewUpGrareAlexXDlg()
{
	m_Dlg = NULL;
	m_UseRedStone = NULL;
	m_pIconDlgEquip = NULL;
	m_pIconDlgEquip2 = NULL;
	REAL_MATERIAL_COUNT_01 = 0;
	REAL_MATERIAL_COUNT_02 = 0;
	REAL_MATERIAL_COUNT_03 = 0;
	REAL_MATERIAL_COUNT_04 = 0;
	real_lv_item = 0;
	real_fullPercen = 0;
	real_percent_total = 0;
	real_r_img = FALSE;
	realFuck_WTF_GradeAlexX = false;
	real_money_total = 0;
}

void CNewUpGrareAlexXDlg::Linking()
{
	m_Dlg = WINDOWMGR->GetWindowForID(NEWUPGRARDALEXX_DLG);
	m_UseRedStone = (cCheckBox*)GetWindowForID(ITEM_STEP_REINFORCE_CHECK_BOX_USE_RED_MIX_STONE);
	m_pIconDlgEquip = (cIconDialog*)GetWindowForID(ITEM_STEP_REINFORCE_ICON_BASE);
	m_pIconDlgEquip2 = (cIconDialog*)GetWindowForID(ITEM_STEP_REINFORCE_ICON_BASE_2);
	m_pNameIconDlgEquip = (cStatic *)GetWindowForID(ITEM_STEP_REINFORCE_STC_BASE_ITEM_NAME);
	m_pIconDlgProtecion = (cIconDialog*)GetWindowForID(ITEM_STEP_REINFORCE_ICON_PROTECTION);
	m_pGuagepercent = (CObjectGuagen*)GetWindowForID(ITEM_STEP_REINFORCE_GAUGE_PROBABILITY);
	m_pMATERIAL_COUNT_01 = (cStatic *)GetWindowForID(ITEM_STEP_REINFORCE_STC_MATERIAL_COUNT_01);
	m_pMATERIAL_COUNT_02 = (cStatic *)GetWindowForID(ITEM_STEP_REINFORCE_STC_MATERIAL_COUNT_02);
	m_pMATERIAL_COUNT_03 = (cStatic *)GetWindowForID(ITEM_STEP_REINFORCE_STC_MATERIAL_COUNT_03);
	m_pMATERIAL_COUNT_04 = (cStatic *)GetWindowForID(ITEM_STEP_REINFORCE_STC_MATERIAL_COUNT_04);
	m_percent_total = (cStatic *)GetWindowForID(ITEM_STEP_REINFORCE_TXT_PERCENT);
	m_percent_max = (cStatic *)GetWindowForID(ITEM_STEP_REINFORCE_TXT_MAX_PERCENT);
	m_r_img = (cStatic *)GetWindowForID(ITEM_STEP_REINFORCE_R_IMG);
	m_money_total = (cStatic *)GetWindowForID(ITEM_STEP_REINFORCE_STC_COST);
	m_next_grade_text = (cStatic *)GetWindowForID(ITEM_STEP_REINFORCE_GARDE_NEXT);
	m_grade_text = (cStatic *)GetWindowForID(ITEM_STEP_REINFORCE_GARDE_OLD);

	/*m_pMATERIAL_COUNT_01->SetStaticText(0);
	m_pMATERIAL_COUNT_02->SetStaticText(0);
	m_pMATERIAL_COUNT_03->SetStaticText(0);
	m_pMATERIAL_COUNT_04->SetStaticText(0);*/
}
BOOL CNewUpGrareAlexXDlg::FakeMoveIcon(LONG x, LONG y, cIcon * icon)
{
	if (icon->GetType() != WT_ITEM)	return FALSE;
	if (m_bDisable)	return FALSE;
	CItem* pOrigItem = (CItem*)icon;
	if (pOrigItem->IsLocked())	return FALSE;
	ITEM_INFO* pItemInfo = ITEMMGR->GetItemInfo(pOrigItem->GetItemIdx());
	if (!pItemInfo)	return FALSE;
	eITEM_KINDBIT bits = pOrigItem->GetItemKind();
	DWORD Grade = pOrigItem->GetGradeAlexX();
	if (Grade >= MAX_UPGRADE_LEVEL_NUM) return FALSE;
	if (pOrigItem->GetItemIdx() == eAlexXItem_ItemProtectUpGrade30 || pOrigItem->GetItemIdx() == eAlexXItem_ItemProtectUpGrade50 || pOrigItem->GetItemIdx() == eAlexXItem_ItemProtectUpGrade100)
	{
		if (m_VirtualItemStone.GetData() == 0)
		{
			AddVirtualItemStone(pOrigItem);
			pOrigItem->SetLock(TRUE);
		}
		else
		{
			return FALSE;
		}
	}
	else if (m_VirtualItemEquip.GetData() == 0)
	{
		if (IsCanUpGrareItem(pOrigItem->GetItemIdx(), bits))
		{
			pOrigItem->SetLock(TRUE);
			AddVirtualItem(pOrigItem);
			AddVirtualItem2(pOrigItem);

			//////////////////
			UpdateNameItem();
			SetUpNew();
		}
		else
		{
			return FALSE;
		}
	}

	ITEMMGR->SetDisableDialog(TRUE, eItemTable_Pyoguk);
	ITEMMGR->SetDisableDialog(TRUE, eItemTable_GuildWarehouse);
	ITEMMGR->SetDisableDialog(TRUE, eItemTable_Shop);
	ITEMMGR->SetDisableDialog(TRUE, eItemTable_PetInven);
	return FALSE;
}
BOOL CNewUpGrareAlexXDlg::OnActionEvent(LONG lId, void * p, DWORD we)
{
	if (we & WE_BTNCLICK)
	{
		//เพิ่มเต็?100%
		if (lId == ITEM_STEP_REINFORCE_BTN_MATERIAL_MAX_01)
		{
			AddMAX_MATERIAL_COUNT(1);
		}
		if (lId == ITEM_STEP_REINFORCE_BTN_MATERIAL_MAX_02)
		{
			AddMAX_MATERIAL_COUNT(2);
		}
		if (lId == ITEM_STEP_REINFORCE_BTN_MATERIAL_MAX_03)
		{
			AddMAX_MATERIAL_COUNT(3);
		}
		if (lId == ITEM_STEP_REINFORCE_BTN_MATERIAL_MAX_04)
		{
			AddMAX_MATERIAL_COUNT(4);
		}
		//เพิ่มหิน + 1
		if (lId == ITEM_STEP_REINFORCE_BTN_MATERIAL_INC_01)
		{
			Add_MATERIAL_COUNT(1);
		}
		if (lId == ITEM_STEP_REINFORCE_BTN_MATERIAL_INC_02)
		{
			Add_MATERIAL_COUNT(2);
		}
		if (lId == ITEM_STEP_REINFORCE_BTN_MATERIAL_INC_03)
		{
			Add_MATERIAL_COUNT(3);
		}
		if (lId == ITEM_STEP_REINFORCE_BTN_MATERIAL_INC_04)
		{
			Add_MATERIAL_COUNT(4);
		}
		//ลบหิ?- 1
		if (lId == ITEM_STEP_REINFORCE_BTN_MATERIAL_DEC_01)
		{
			Del_MATERIAL_COUNT(1);
		}
		if (lId == ITEM_STEP_REINFORCE_BTN_MATERIAL_DEC_02)
		{
			Del_MATERIAL_COUNT(2);
		}
		if (lId == ITEM_STEP_REINFORCE_BTN_MATERIAL_DEC_03)
		{
			Del_MATERIAL_COUNT(3);
		}
		if (lId == ITEM_STEP_REINFORCE_BTN_MATERIAL_DEC_04)
		{
			Del_MATERIAL_COUNT(4);
		}
		//ตีบว?
		if (lId == ITEM_STEP_REINFORCE_BTN_REINFORCE)
		{
			SendItemUpGrareAlexXMsg();
		}
		//
		if (lId == ITEM_STEP_REINFORCE_CLOSEBTN)
		{
			close_thisdlg();
		}
		if (lId == ITEM_STEP_REINFORCE_BTN_CLEAR_ITEM)
		{
			clear_VirtualItemEquip();
		}
		if (lId == ITEM_STEP_REINFORCE_BTN_MATERIAL_DEC_1_ALL)
		{
			ClearMAX_MATERIAL_COUNT(1);
		}
		if (lId == ITEM_STEP_REINFORCE_BTN_MATERIAL_DEC_2_ALL)
		{
			ClearMAX_MATERIAL_COUNT(2);
		}
		if (lId == ITEM_STEP_REINFORCE_BTN_MATERIAL_DEC_3_ALL)
		{
			ClearMAX_MATERIAL_COUNT(3);
		}
		if (lId == ITEM_STEP_REINFORCE_BTN_MATERIAL_DEC_4_ALL)
		{
			ClearMAX_MATERIAL_COUNT(4);
		}
		if (lId == ITEM_STEP_REINFORCE_BTN_STONE_DEL)
		{
			clear_VirtualItemStone();
		}
	}
	return TRUE;
}
void CNewUpGrareAlexXDlg::AddVirtualItem(CItem * pItem)
{
	cIcon* pIcon = (cIcon*)pItem;
	m_VirtualItemEquip.SetData(pItem->GetItemIdx());
	m_VirtualItemEquip.SetLinkItem(pItem);
	m_pIconDlgEquip->AddIcon(0, (cIcon*)&m_VirtualItemEquip);
	CItem * pTargetItem = ITEMMGR->GetItem(m_VirtualItemEquip.GetLinkItem()->GetDBIdx());
	//pTargetItem->SetGradeAlexX(pItem->GetGradeAlexX());
	//m_VirtualItemEquip.SetIsWTF(TRUE);
	ITEMMGR->SetToolTipIcon((cIcon*)&m_VirtualItemEquip, ITEMMGR->GetItemOption(pTargetItem->GetDurability()), ITEMMGR->GetItemRareOption(pTargetItem->GetRareness()), 0/*, ITEMMGR->GetItemStoneOption(pTargetItem->GetStoneIdx())*/, 0, 0, pTargetItem->GetGradeAlexX());
	//m_pIconDlgEquip->SetDisable(TRUE);
	m_VirtualItemEquip.SetMovable(FALSE);
}
void CNewUpGrareAlexXDlg::AddVirtualItem2(CItem * pItem)
{
	cIcon* pIcon = (cIcon*)pItem;
	m_VirtualItemEquip2.SetData(pItem->GetItemIdx());
	m_VirtualItemEquip2.SetLinkItem(pItem);
	m_pIconDlgEquip2->AddIcon(0, (cIcon*)&m_VirtualItemEquip2);
	CItem * pTargetItem = ITEMMGR->GetItem(m_VirtualItemEquip2.GetLinkItem()->GetDBIdx());
	//pTargetItem->SetGradeAlexX(val);
	//pItem->SetGradeAlexX(val);
	m_VirtualItemEquip2.SetIsWTF(TRUE);
	if (pTargetItem->GetGradeAlexX() == 0)
	{
		SetrealFuck_WTF_GradeAlexX(true);
		m_VirtualItemEquip2.SetGradeAlexX(pTargetItem->GetGradeAlexX() + 1);
	}
	DWORD TWT_WTF = pTargetItem->GetGradeAlexX() + 1;
	//ITEMMGR->SetToolTipIcon((cIcon*)&m_VirtualItemEquip2, ITEMMGR->GetItemOption(pTargetItem->GetDurability()), ITEMMGR->GetItemRareOption(pTargetItem->GetRareness()), 0/*, ITEMMGR->GetItemStoneOption(pTargetItem->GetStoneIdx())*/, TWT_WTF);
	ITEMMGR->SetToolTipIcon((cIcon*)&m_VirtualItemEquip2, ITEMMGR->GetItemOption(pTargetItem->GetDurability()), ITEMMGR->GetItemRareOption(pTargetItem->GetRareness()), 0/*, ITEMMGR->GetItemStoneOption(pTargetItem->GetStoneIdx())*/, 0, 0, TWT_WTF);
	m_VirtualItemEquip2.SetMovable(FALSE);
}
void CNewUpGrareAlexXDlg::AddVirtualItemStone(CItem * pItem)
{
	cIcon* pIcon = (cIcon*)pItem;
	m_VirtualItemStone.SetData(pItem->GetItemIdx());
	m_VirtualItemStone.SetLinkItem(pItem);
	m_pIconDlgProtecion->AddIcon(0, (cIcon*)&m_VirtualItemStone);
	CItem * pTargetItem = ITEMMGR->GetItem(m_VirtualItemStone.GetLinkItem()->GetDBIdx());
	ITEMMGR->SetToolTipIcon((cIcon*)&m_VirtualItemStone, ITEMMGR->GetItemOption(pTargetItem->GetDurability()), ITEMMGR->GetItemRareOption(pTargetItem->GetRareness()), 0/*, ITEMMGR->GetItemStoneOption(pTargetItem->GetStoneIdx())*/);
	m_VirtualItemStone.SetMovable(FALSE);
}
void CNewUpGrareAlexXDlg::SetNameItemStaticText(char name[33],DWORD grade,WORD color)
{
	DWORD m_color = 0;
	if (color == 1)
	{
		m_color = TTTC_DEFAULT;
	}
	else if (color == 2)
	{
		m_color = TTTC_DEFAULT;
	}
	else if (color == 3)
	{
		m_color = TTTC_DEFAULT;
	}
	else if (color == 4)
	{
		m_color = TTTC_DEFAULT;
	}
	else if (color == 5)
	{
		m_color = TTTC_DEFAULT;
	}
	else if (color == 6)
	{
		m_color = TTTC_DEFAULT;
	}
	else
	{
		m_color = TTTC_DEFAULT;
	}
	char line[128];
	sprintf(line, "%s+%d", name, grade);
	m_pNameIconDlgEquip->SetStaticText(line);
	m_pNameIconDlgEquip->SetFGColor(m_color);
}
void CNewUpGrareAlexXDlg::SendItemUpGrareAlexXMsg()
{
	if (!m_VirtualItemEquip.GetLinkItem())
	{
		CHATMGR->AddMsg(CTC_SYSMSG, "!m_VirtualItemEquip.GetLinkItem");
		return;
	}

	CItem* pItem = ITEMMGR->GetItem(m_VirtualItemEquip.GetLinkItem()->GetDBIdx());

	if (!pItem)
	{
		if (m_pIconDlgEquip)
		{
			CVirtualItem* pVItem = NULL;
			m_pIconDlgEquip->DeleteIcon(0, (cIcon**)&pVItem);
			if (pVItem)
				pVItem->GetLinkItem()->SetLock(FALSE);
		}
		ITEMMGR->SetDisableDialog(FALSE, eItemTable_Inventory);
		ITEMMGR->SetDisableDialog(FALSE, eItemTable_Pyoguk);
		ITEMMGR->SetDisableDialog(FALSE, eItemTable_GuildWarehouse);
		ITEMMGR->SetDisableDialog(FALSE, eItemTable_Shop);
		ITEMMGR->SetDisableDialog(FALSE, eItemTable_PetInven);
		//OBJECTSTATEMGR->EndObjectState(HERO, eObjectState_Deal);
		return;
	}

	MSG_NEWYPGRARE_ALEXX msg;
	msg.Category = MP_NEWUPGRARE_ALEXX;
	msg.Protocol = MP_NEWUPGRARE_ALEXX_NEWUPGRARE_SYN;
	msg.dwObjectID = HEROID;
	msg.ItemDBIdx = pItem->GetItemIdx();
	msg.ItemPosition = pItem->GetPosition();

	DWORD dwMoney;
	dwMoney = calculateUpGradeItemMoeny();
	if (dwMoney > HERO->GetMoney())
	{
		CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(117));
		return;
	}
	else
	{
		msg.money = dwMoney;
	}

	if (!REAL_MATERIAL_COUNT_01 == 0)
	{
		msg.Material_count_1 = REAL_MATERIAL_COUNT_01;
		msg.Material_count_1_DB = m_VirtualItem_MATERIAL_1.GetLinkItem()->GetItemIdx();
		msg.Material_count_1_POS = m_VirtualItem_MATERIAL_1.GetLinkItem()->GetPosition();
	}
	else
	{
		msg.Material_count_1 = 0;
		msg.Material_count_1_DB = 0;
		msg.Material_count_1_POS = 0;
	}

	if (!REAL_MATERIAL_COUNT_02 == 0)
	{
		msg.Material_count_2 = REAL_MATERIAL_COUNT_02;
		msg.Material_count_2_DB = m_VirtualItem_MATERIAL_2.GetLinkItem()->GetItemIdx();
		msg.Material_count_2_POS = m_VirtualItem_MATERIAL_2.GetLinkItem()->GetPosition();
	}
	else
	{
		msg.Material_count_2 = 0;
		msg.Material_count_2_DB = 0;
		msg.Material_count_2_POS = 0;
	}

	if (!REAL_MATERIAL_COUNT_03 == 0)
	{
		msg.Material_count_3 = REAL_MATERIAL_COUNT_03;
		msg.Material_count_3_DB = m_VirtualItem_MATERIAL_3.GetLinkItem()->GetItemIdx();
		msg.Material_count_3_POS = m_VirtualItem_MATERIAL_3.GetLinkItem()->GetPosition();
	}
	else
	{
		msg.Material_count_3 = 0;
		msg.Material_count_3_DB = 0;
		msg.Material_count_3_POS = 0;
	}

	if (!REAL_MATERIAL_COUNT_04 == 0)
	{
		msg.Material_count_4 = REAL_MATERIAL_COUNT_04;
		msg.Material_count_4_DB = m_VirtualItem_MATERIAL_4.GetLinkItem()->GetItemIdx();
		msg.Material_count_4_POS = m_VirtualItem_MATERIAL_4.GetLinkItem()->GetPosition();
	}
	else
	{
		msg.Material_count_4 = 0;
		msg.Material_count_4_DB = 0;
		msg.Material_count_4_POS = 0;
	}

	if (m_VirtualItemStone.GetLinkItem())
	{
		if (m_VirtualItemStone.GetLinkItem()->GetItemIdx() == eAlexXItem_ItemProtectUpGrade30
			|| m_VirtualItemStone.GetLinkItem()->GetItemIdx() == eAlexXItem_ItemProtectUpGrade50
			|| m_VirtualItemStone.GetLinkItem()->GetItemIdx() == eAlexXItem_ItemProtectUpGrade100)
		{
			msg.Stone_count = 1;
			msg.Stone_DB = m_VirtualItemStone.GetLinkItem()->GetItemIdx();
			msg.Stone_POS = m_VirtualItemStone.GetLinkItem()->GetPosition();
		}
	}
	else
	{
		msg.Stone_count = 0;
		msg.Stone_DB = 0;
		msg.Stone_POS = 0;
	}

	/*char line[128];
	sprintf_s(line, sizeof(line), "%d", REAL_MATERIAL_COUNT_01);
	CHATMGR->AddMsg(CTC_SYSMSG, line);
	sprintf_s(line, sizeof(line), "%d", REAL_MATERIAL_COUNT_02);
	CHATMGR->AddMsg(CTC_SYSMSG, line);
	sprintf_s(line, sizeof(line), "%d", REAL_MATERIAL_COUNT_03);
	CHATMGR->AddMsg(CTC_SYSMSG, line);
	sprintf_s(line, sizeof(line), "%d", REAL_MATERIAL_COUNT_04);
	CHATMGR->AddMsg(CTC_SYSMSG, line);*/

	//msg.dwData3 = m_StoneItem->GetItemIdx(); 
	//msg.dwData4 = m_StoneItem->GetPosition();

	//for test
	/*MSG_WORD2 msg;
	msg.Category = MP_NEWUPGRARE_ALEXX;
	msg.Protocol = MP_NEWUPGRARE_ALEXX_NEWUPGRARE_SYN;
	msg.dwObjectID = HEROID;
	msg.wData1 = 1;*/


	NETWORK->Send(&msg, sizeof(msg));
	CHATMGR->AddMsg(CTC_SYSMSG, "Send");
}
BOOL CNewUpGrareAlexXDlg::IsCanUpGrareItem(DWORD wItemIdx, eITEM_KINDBIT bits)
{
	if (ITEMMGR->IsDupItem(wItemIdx))
	{
		return FALSE;
	}
	switch (bits)
	{
	case eEQUIP_ITEM_DRESS:
	{
		return TRUE;
	}
	break;
	case eEQUIP_ITEM_HAT:
	{
		return TRUE;
	}
	break;
	case eEQUIP_ITEM_SHOES:
	{
		return TRUE;
	}
	break;
	case eEQUIP_ITEM_WEAPON:
	{
		return TRUE;
	}
	break;
	case eEQUIP_ITEM_RING:
	{
		return TRUE;
	}
	break;
	case eEQUIP_ITEM_CAPE:
	{
		return TRUE;
	}
	break;
	case eEQUIP_ITEM_NECKLACE:
	{
		return TRUE;
	}
	break;
	case eEQUIP_ITEM_ARMLET:
	{
		return TRUE;
	}
	break;
	case eEQUIP_ITEM_BELT:
	{
		return TRUE;
	}
	//case eEQUIP_ITEM_EAR:
	//{
	//	return TRUE;
	//}
	//case eEQUIP_ITEM_BAR:
	//{
	//	return TRUE;
	//}
	}
	return FALSE;
}
void CNewUpGrareAlexXDlg::Add_MATERIAL_COUNT(DWORD lv)
{
	if (lv == 0) return;
	if (!m_VirtualItemEquip.GetLinkItem()) return;
	CItem * item = NULL;
	WORD Pos;
	DWORD ItemIdx = WhatLvMATERIAL(lv);

	item = GAMEIN->GetInventoryDialog()->GetItemLike30(ItemIdx, Pos);
	if (item == NULL) return;
	DURTYPE MaxitemDur = item->GetDurability();

	char line[64];

	if (lv == 1)
	{
		m_VirtualItem_MATERIAL_1.SetData(item->GetItemIdx());
		m_VirtualItem_MATERIAL_1.SetLinkItem(item);
		if (REAL_MATERIAL_COUNT_01 < MaxitemDur)
		{
			REAL_MATERIAL_COUNT_01 = REAL_MATERIAL_COUNT_01 + 1;
			sprintf_s(line, sizeof(line), "%d", REAL_MATERIAL_COUNT_01);
			m_pMATERIAL_COUNT_01->SetStaticText(line);
		}
		else
		{
			CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(2402));
		}
	}
	else if (lv == 2)
	{
		m_VirtualItem_MATERIAL_2.SetData(item->GetItemIdx());
		m_VirtualItem_MATERIAL_2.SetLinkItem(item);
		if (REAL_MATERIAL_COUNT_02 < MaxitemDur)
		{
			REAL_MATERIAL_COUNT_02 = REAL_MATERIAL_COUNT_02 + 1;
			sprintf_s(line, sizeof(line), "%d", REAL_MATERIAL_COUNT_02);
			m_pMATERIAL_COUNT_02->SetStaticText(line);
		}
		else
		{
			CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(2402));
		}
	}
	else if (lv == 3)
	{
		m_VirtualItem_MATERIAL_3.SetData(item->GetItemIdx());
		m_VirtualItem_MATERIAL_3.SetLinkItem(item);
		if (REAL_MATERIAL_COUNT_03 < MaxitemDur)
		{
			REAL_MATERIAL_COUNT_03 = REAL_MATERIAL_COUNT_03 + 1;
			sprintf_s(line, sizeof(line), "%d", REAL_MATERIAL_COUNT_03);
			m_pMATERIAL_COUNT_03->SetStaticText(line);
		}
		else
		{
			CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(2402));
		}
	}
	else if (lv == 4)
	{
		m_VirtualItem_MATERIAL_4.SetData(item->GetItemIdx());
		m_VirtualItem_MATERIAL_4.SetLinkItem(item);
		if (REAL_MATERIAL_COUNT_04 < MaxitemDur)
		{
			REAL_MATERIAL_COUNT_04 = REAL_MATERIAL_COUNT_04 + 1;
			sprintf_s(line, sizeof(line), "%d", REAL_MATERIAL_COUNT_04);
			m_pMATERIAL_COUNT_04->SetStaticText(line);
		}
		else
		{
			CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(2402));
		}
	}
	//item->SetLock(TRUE);
	Re_UpdatePercent();
	m_r_img->SetActive(FALSE);
}
void CNewUpGrareAlexXDlg::Del_MATERIAL_COUNT(DWORD lv)
{
	if (lv == 0) return;
	if (!m_VirtualItemEquip.GetLinkItem()) return;
	CItem * item = NULL;
	WORD Pos;
	DWORD ItemIdx = WhatLvMATERIAL(lv);

	item = GAMEIN->GetInventoryDialog()->GetItemLike30(ItemIdx, Pos);
	if (item == NULL) return;
	DURTYPE MaxitemDur = item->GetDurability();

	char line[64];

	if (lv == 1)
	{
		m_VirtualItem_MATERIAL_1.SetData(item->GetItemIdx());
		m_VirtualItem_MATERIAL_1.SetLinkItem(item);
		if (REAL_MATERIAL_COUNT_01 > 0)
		{
			REAL_MATERIAL_COUNT_01 = REAL_MATERIAL_COUNT_01 - 1;
			sprintf_s(line, sizeof(line), "%d", REAL_MATERIAL_COUNT_01);
			m_pMATERIAL_COUNT_01->SetStaticText(line);
		}
	}
	else if (lv == 2)
	{
		m_VirtualItem_MATERIAL_2.SetData(item->GetItemIdx());
		m_VirtualItem_MATERIAL_2.SetLinkItem(item);
		if (REAL_MATERIAL_COUNT_02 > 0)
		{
			//if (REAL_MATERIAL_COUNT_02 == 0) return;
			REAL_MATERIAL_COUNT_02 = REAL_MATERIAL_COUNT_02 - 1;
			sprintf_s(line, sizeof(line), "%d", REAL_MATERIAL_COUNT_02);
			m_pMATERIAL_COUNT_02->SetStaticText(line);
		}
	}
	else if (lv == 3)
	{
		m_VirtualItem_MATERIAL_3.SetData(item->GetItemIdx());
		m_VirtualItem_MATERIAL_3.SetLinkItem(item);
		if (REAL_MATERIAL_COUNT_03 > 0)
		{
			//if (REAL_MATERIAL_COUNT_03 == 0) return;
			REAL_MATERIAL_COUNT_03 = REAL_MATERIAL_COUNT_03 - 1;
			sprintf_s(line, sizeof(line), "%d", REAL_MATERIAL_COUNT_03);
			m_pMATERIAL_COUNT_03->SetStaticText(line);
		}
	}
	else if (lv == 4)
	{
		m_VirtualItem_MATERIAL_4.SetData(item->GetItemIdx());
		m_VirtualItem_MATERIAL_4.SetLinkItem(item);
		if (REAL_MATERIAL_COUNT_04 > 0)
		{
			//if (REAL_MATERIAL_COUNT_04 == 0) return;
			REAL_MATERIAL_COUNT_04 = REAL_MATERIAL_COUNT_04 - 1;
			sprintf_s(line, sizeof(line), "%d", REAL_MATERIAL_COUNT_04);
			m_pMATERIAL_COUNT_04->SetStaticText(line);
		}
	}
	//item->SetLock(TRUE);
	Re_UpdatePercent();
	m_r_img->SetActive(FALSE);
}
void CNewUpGrareAlexXDlg::AddMAX_MATERIAL_COUNT(DWORD lv)
{
	if (lv == 0) return;
	if (!m_VirtualItemEquip.GetLinkItem()) return;

	CItem * item = NULL;
	WORD Pos;
	DWORD ItemIdx = WhatLvMATERIAL(lv);
	item = GAMEIN->GetInventoryDialog()->GetItemLike30(ItemIdx, Pos);
	if (item == NULL) return;
	DURTYPE MaxitemDur = item->GetDurability();
	DWORD real_MaxitemDur;

	float Xreal_fullPercen = calculateUpGradeItemPercent();
	float result = calculateMaterialCount(Xreal_fullPercen, lv);

	char line[64];
	if (MaxitemDur >= (DWORD)result)
	{
		sprintf_s(line, sizeof(line), "%d", (DWORD)result);
		real_MaxitemDur = (DWORD)result;
	}
	else
	{
		sprintf_s(line, sizeof(line), "%d", MaxitemDur);
		real_MaxitemDur = MaxitemDur;
	}

	if (lv == 1)
	{
		m_pMATERIAL_COUNT_01->SetStaticText(line);
		m_VirtualItem_MATERIAL_1.SetData(item->GetItemIdx());
		m_VirtualItem_MATERIAL_1.SetLinkItem(item);
		REAL_MATERIAL_COUNT_01 = real_MaxitemDur;
	}
	else if (lv == 2)
	{
		m_pMATERIAL_COUNT_02->SetStaticText(line);
		m_VirtualItem_MATERIAL_2.SetData(item->GetItemIdx());
		m_VirtualItem_MATERIAL_2.SetLinkItem(item);
		REAL_MATERIAL_COUNT_02 = real_MaxitemDur;
	}
	else if (lv == 3)
	{
		m_pMATERIAL_COUNT_03->SetStaticText(line);
		m_VirtualItem_MATERIAL_3.SetData(item->GetItemIdx());
		m_VirtualItem_MATERIAL_3.SetLinkItem(item);
		REAL_MATERIAL_COUNT_03 = real_MaxitemDur;
	}
	else if (lv == 4)
	{
		m_pMATERIAL_COUNT_04->SetStaticText(line);
		m_VirtualItem_MATERIAL_4.SetData(item->GetItemIdx());
		m_VirtualItem_MATERIAL_4.SetLinkItem(item);
		REAL_MATERIAL_COUNT_04 = real_MaxitemDur;
	}
	//item->SetLock(TRUE);
	Re_UpdatePercent();
	m_r_img->SetActive(FALSE);
}
DWORD CNewUpGrareAlexXDlg::WhatLvMATERIAL(DWORD lv)
{
	DWORD ItemIdx;
	switch (lv)
	{
	case 1:
	{
		ItemIdx = eAlexXItem_ItemUpGradeMaterial_lv1;
	}
	break;
	case 2:
	{
		ItemIdx = eAlexXItem_ItemUpGradeMaterial_lv2;
	}
	break;
	case 3:
	{
		ItemIdx = eAlexXItem_ItemUpGradeMaterial_lv3;
	}
	break;
	case 4:
	{
		ItemIdx = eAlexXItem_ItemUpGradeMaterial_lv4;
	}
	break;
	}
	return ItemIdx;
}
void CNewUpGrareAlexXDlg::UpdateGuagePercent(float curVal)
{
	/*real_fullPercen = ITEMMGR->GetUpGradeItemPercentList(real_lv_item)->MaxPercent;
	float value = (float)curVal / (float)real_fullPercen;
	m_pGuagepercent->SetValue(value, 0);
	real_percent_total = (curVal / real_fullPercen) * 100;
	char line[128];
	sprintf_s(line, sizeof(line), "%.4f%%", real_percent_total);
	m_percent_total->SetStaticText(line);*/

	/*float increase_factor = 1.20;
	CItem* pItem = ITEMMGR->GetItem(m_VirtualItemEquip.GetLinkItem()->GetDBIdx());

	real_fullPercen = ITEMMGR->GetUpGradeItemPercentList(real_lv_item)->MaxPercent;
	DWORD pluGrade = pItem->GetGradeAlexX() + 1;
	float Xreal_fullPercen = real_fullPercen * pow(increase_factor, pluGrade);*/

	if (!m_VirtualItemEquip.GetLinkItem()) return;

	float Xreal_fullPercen = calculateUpGradeItemPercent();
	
	char line_test[128];
	sprintf_s(line_test, sizeof(line_test), "%.4f%%", Xreal_fullPercen);
	CHATMGR->AddMsg(CTC_SYSMSG, line_test);

	float value = (float)curVal / (float)Xreal_fullPercen;
	m_pGuagepercent->SetValue(value, 0);

	real_percent_total = (curVal / Xreal_fullPercen) * 100;
	char line[128];
	if (real_percent_total >= 100)
	{
		m_percent_max->SetActive(TRUE);
		sprintf_s(line, sizeof(line), "100.0000%%");
	}
	else
	{
		m_percent_max->SetActive(FALSE);
		sprintf_s(line, sizeof(line), "%.4f%%", real_percent_total);
	}
	m_percent_total->SetStaticText(line);
}
void CNewUpGrareAlexXDlg::SetUpNew()
{
	UpdateGuagePercent(0);
	m_r_img->SetActive(FALSE);
	UpdateMoney();
}
void CNewUpGrareAlexXDlg::Re_UpdatePercent()
{
	float full_naterial_1 = NEWUPGRAREALEXX_CMGR->AlexXUpGradeConfig.SetupGrade_AlexX[ITEM_MATERIAL_LV_01_PERCENT] * REAL_MATERIAL_COUNT_01;
	float full_naterial_2 = NEWUPGRAREALEXX_CMGR->AlexXUpGradeConfig.SetupGrade_AlexX[ITEM_MATERIAL_LV_02_PERCENT] * REAL_MATERIAL_COUNT_02;
	float full_naterial_3 = NEWUPGRAREALEXX_CMGR->AlexXUpGradeConfig.SetupGrade_AlexX[ITEM_MATERIAL_LV_03_PERCENT] * REAL_MATERIAL_COUNT_03;
	float full_naterial_4 = NEWUPGRAREALEXX_CMGR->AlexXUpGradeConfig.SetupGrade_AlexX[ITEM_MATERIAL_LV_04_PERCENT] * REAL_MATERIAL_COUNT_04;

	float full_naterial_X = full_naterial_1 + full_naterial_2 + full_naterial_3 + full_naterial_4;

	UpdateGuagePercent(full_naterial_X);
}
void CNewUpGrareAlexXDlg::MP_BakcGame_Msg(MSG_NEWYPGRARE_ALEXX_BACKGAME* msg)
{
	if (!msg->result == 0)
	{
		if (msg->result == 1)
		{
			ReLoad_MATERIAL_COUNT();
			UpdateGuagePercent(0);
			SetImg_R(1);
			CHATMGR->AddMsg(CTC_SYSMSG, "ตีบวกสำเร็?");

			Reload_VirtualItem(true);
			UpdateMoney();
			UpdateNameItem();
		}
		else if (msg->result == 2)
		{
			ReLoad_MATERIAL_COUNT();
			UpdateGuagePercent(0);
			SetImg_R(2);
			CHATMGR->AddMsg(CTC_SYSMSG, "ตีบวกล้มเหลว");

			//Reload_VirtualItem();
			UpdateMoney();
		}
		else if (msg->result == 3)
		{
			UpdateGuagePercent(0);
			ReLoad_MATERIAL_COUNT();
			SetImg_R(3);

			Reload_VirtualItem(false);
			UpdateMoney();
			UpdateNameItem();
		}
		STATSMGR->CalcItemStats(HERO);
		GAMEIN->GetCharacterDialog()->SetAttackRate();
		GAMEIN->GetCharacterDialog()->SetDefenseRate();
		GAMEIN->GetCharacterDialog()->SetGenGol();
		GAMEIN->GetCharacterDialog()->SetMinChub();
		GAMEIN->GetCharacterDialog()->SetCheRyuk();
		GAMEIN->GetCharacterDialog()->SetSimMek();
		GAMEIN->GetCharacterDialog()->SetCritical();
		GAMEIN->GetCharacterDialog()->UpdateData();
		APPEARANCEMGR->InitAppearance(HERO);
	}
	else if (!msg->WhatError == 0)
	{
		if (msg->WhatError == 1)
		{

		}
		else if (msg->WhatError == 2)
		{

		}
		else if (msg->WhatError == 3)
		{

		}
		else if (msg->WhatError == 3)
		{

		}
		else if (msg->WhatError == 4)
		{

		}
		else if (msg->WhatError == 5)
		{

		}
	}
}
void CNewUpGrareAlexXDlg::MP_BakcGame_DelItem_Msg(MSG_NEWYPGRARE_ALEXX_DELITEM* msg)
{
	if (msg->Pos == 0) return;
	CItem * item = GAMEIN->GetInventoryDialog()->GetItemForPos(msg->Pos);

	if (item->GetDurability() > 1)
	{
		GAMEIN->GetInventoryDialog()->UpdateItemDurabilityAdd(msg->Pos, -msg->count);
		if (item->GetDurability() == 0)
		{
			CItem* pOutItem = NULL;
			ITEMMGR->DeleteItem(msg->Pos, &pOutItem);
			if (pOutItem)
				ITEMMGR->ReLinkQuickPosition(pOutItem);

			if (item->GetItemIdx() == eAlexXItem_ItemProtectUpGrade30 || item->GetItemIdx() == eAlexXItem_ItemProtectUpGrade50 || item->GetItemIdx() == eAlexXItem_ItemProtectUpGrade100)
			{
				if (m_pIconDlgProtecion)
				{
					CVirtualItem* pVItem = NULL;

					m_pIconDlgProtecion->DeleteIcon(0, (cIcon**)&pVItem);
					if (pVItem)
						pVItem->GetLinkItem()->SetLock(FALSE);

					m_VirtualItemStone.SetLinkItem(NULL);
					m_VirtualItemStone.SetData(NULL);
					m_VirtualItemStone.SetIsWTF(FALSE);
				}
			}
		}
	}
	else
	{
		CItem* pOutItem = NULL;
		ITEMMGR->DeleteItem(msg->Pos, &pOutItem);
		if (pOutItem)
			ITEMMGR->ReLinkQuickPosition(pOutItem);

		if (pOutItem->GetItemIdx() == eAlexXItem_ItemProtectUpGrade30 || pOutItem->GetItemIdx() == eAlexXItem_ItemProtectUpGrade50 || pOutItem->GetItemIdx() == eAlexXItem_ItemProtectUpGrade100)
		{
			if (m_pIconDlgProtecion)
			{
				CVirtualItem* pVItem = NULL;

				m_pIconDlgProtecion->DeleteIcon(0, (cIcon**)&pVItem);
				if (pVItem)
					pVItem->GetLinkItem()->SetLock(FALSE);

				m_VirtualItemStone.SetLinkItem(NULL);
				m_VirtualItemStone.SetData(NULL);
				m_VirtualItemStone.SetIsWTF(FALSE);
			}
		}
	}
}
void CNewUpGrareAlexXDlg::ReLoad_MATERIAL_COUNT()
{
	REAL_MATERIAL_COUNT_01 = 0;
	REAL_MATERIAL_COUNT_02 = 0;
	REAL_MATERIAL_COUNT_03 = 0;
	REAL_MATERIAL_COUNT_04 = 0;

	char line[128];
	sprintf_s(line, sizeof(line), "%d", REAL_MATERIAL_COUNT_01);
	m_pMATERIAL_COUNT_01->SetStaticText(line);
	sprintf_s(line, sizeof(line), "%d", REAL_MATERIAL_COUNT_02);
	m_pMATERIAL_COUNT_02->SetStaticText(line);
	sprintf_s(line, sizeof(line), "%d", REAL_MATERIAL_COUNT_03);
	m_pMATERIAL_COUNT_03->SetStaticText(line);
	sprintf_s(line, sizeof(line), "%d", REAL_MATERIAL_COUNT_04);
	m_pMATERIAL_COUNT_04->SetStaticText(line);
}
void CNewUpGrareAlexXDlg::SetImg_R(DWORD val)
{
	if (val == 0) return;
	else
	{ 
		m_r_img->SetActive(TRUE);
	}
	cImage Img;
	if (val == 1)
	{
		SCRIPTMGR->GetImage(345, &Img, PFT_HARDPATH);
	}
	else if (val == 2)
	{
		SCRIPTMGR->GetImage(346, &Img, PFT_HARDPATH);
	}
	else if (val == 3)
	{
		SCRIPTMGR->GetImage(347, &Img, PFT_HARDPATH);
	}
	m_r_img->SetBasicImage(&Img);

}
void CNewUpGrareAlexXDlg::close_thisdlg()
{
	clear_VirtualItemEquip();
	UpdateGuagePercent(0);
	ReLoad_MATERIAL_COUNT();
	clear_VirtualItemStone();
	UpdateNameItem();
	
	if (m_r_img->IsActive())
		m_r_img->SetActive(FALSE);

	ITEMMGR->SetDisableDialog(FALSE, eItemTable_Inventory);
	ITEMMGR->SetDisableDialog(FALSE, eItemTable_Pyoguk);
	ITEMMGR->SetDisableDialog(FALSE, eItemTable_GuildWarehouse);
	ITEMMGR->SetDisableDialog(FALSE, eItemTable_Shop);
	ITEMMGR->SetDisableDialog(FALSE, eItemTable_PetInven);

	SetActive(FALSE);
}
float CNewUpGrareAlexXDlg::calculateMaterialCount(float max_naterial,DWORD lv) 
{
	if (max_naterial == 0) return 0.0;  // ถ้?max_naterial เป็น 0 ให้คืนค่?0.0
	float x_naterial = 0.0;  // กำหนดค่าเริ่มต้นให?x_naterial เป็น 0.0

	if (lv == 1)
		x_naterial = max_naterial / 1.5;
	else if (lv == 2)
		x_naterial = max_naterial / 20;
	else if (lv == 3)
		x_naterial = max_naterial / 225;
	else if (lv == 4)
		x_naterial = max_naterial / 2500;

	return x_naterial;
}

float CNewUpGrareAlexXDlg::calculateUpGradeItemPercent()
{
	//float increase_factor = 1.20;
	float increase_factor = NEWUPGRAREALEXX_CMGR->AlexXUpGradeConfig.SetupGrade_AlexX[UpGradeRate];
	CItem* pItem = ITEMMGR->GetItem(m_VirtualItemEquip.GetLinkItem()->GetDBIdx());

	real_fullPercen = ITEMMGR->GetUpGradeItemPercentList(real_lv_item)->MaxPercent;
	DWORD pluGrade = pItem->GetGradeAlexX() + 1;
	float Xreal_fullPercen = real_fullPercen * pow(increase_factor, pluGrade);

	return Xreal_fullPercen;
}
void CNewUpGrareAlexXDlg::clear_VirtualItemEquip()
{
	if (!m_VirtualItemEquip.GetLinkItem()) return;
	if (m_pIconDlgEquip)
	{
		CVirtualItem* pVItem = NULL;

		m_pIconDlgEquip->DeleteIcon(0, (cIcon**)&pVItem);
		if (pVItem)
			pVItem->GetLinkItem()->SetLock(FALSE);

		m_VirtualItemEquip.SetLinkItem(NULL);
		m_VirtualItemEquip.SetData(NULL);
		m_VirtualItemEquip.SetIsWTF(FALSE);
	}

	if (m_pIconDlgEquip2)
	{
		CVirtualItem* pVItem = NULL;

		m_pIconDlgEquip2->DeleteIcon(0, (cIcon**)&pVItem);
		if (pVItem)
			pVItem->GetLinkItem()->SetLock(FALSE);

		m_VirtualItemEquip2.SetLinkItem(NULL);
		m_VirtualItemEquip2.SetData(NULL);
	}

	m_pNameIconDlgEquip->SetStaticText("");
	UpdateGuagePercent(0);
	ReLoad_MATERIAL_COUNT();

	if (m_r_img->IsActive())
		m_r_img->SetActive(FALSE);

	m_money_total->SetStaticText("");
	UpdateNameItem();
}
void CNewUpGrareAlexXDlg::ClearMAX_MATERIAL_COUNT(DWORD lv)
{
	if (lv == 0) return;

	char line[64];

	if (lv == 1)
	{
		REAL_MATERIAL_COUNT_01 = 0;
		sprintf_s(line, sizeof(line), "%d", REAL_MATERIAL_COUNT_01);
		m_pMATERIAL_COUNT_01->SetStaticText(line);
	}
	else if (lv == 2)
	{
		REAL_MATERIAL_COUNT_02 = 0;
		sprintf_s(line, sizeof(line), "%d", REAL_MATERIAL_COUNT_02);
		m_pMATERIAL_COUNT_02->SetStaticText(line);
	}
	else if (lv == 3)
	{
		REAL_MATERIAL_COUNT_03 = 0;
		sprintf_s(line, sizeof(line), "%d", REAL_MATERIAL_COUNT_03);
		m_pMATERIAL_COUNT_03->SetStaticText(line);
	}
	else if (lv == 4)
	{
		REAL_MATERIAL_COUNT_04 = 0;
		sprintf_s(line, sizeof(line), "%d", REAL_MATERIAL_COUNT_04);
		m_pMATERIAL_COUNT_04->SetStaticText(line);
	}
	//item->SetLock(TRUE);
	Re_UpdatePercent();

	if (m_r_img->IsActive())
		m_r_img->SetActive(FALSE);
}
void CNewUpGrareAlexXDlg::Reload_VirtualItem(bool what)
{
	CItem * pTItem = NULL;
	CVirtualItem * pVItem = (CVirtualItem *)m_pIconDlgEquip->GetIconForIdx(0);
	pTItem = GAMEIN->GetInventoryDialog()->GetItemForPos(pVItem->GetSrcPosition());
	if (pTItem)
	{
		if (what)
		{
			pTItem->SetGradeAlexX(pVItem->GetGradeAlexX() + 1);
		}
		else
		{
			pTItem->SetGradeAlexX(pVItem->GetGradeAlexX() - 1);
		}
		ITEMMGR->SetToolTipIcon(pTItem, ITEMMGR->GetItemOption(pTItem->GetDurability()), ITEMMGR->GetItemRareOption(pTItem->GetRareness()), 0/*, ITEMMGR->GetItemStoneOption(pTItem->GetStoneIdx())*/,0,0, pTItem->GetGradeAlexX());
		AddVirtualItem(pTItem);
		AddVirtualItem2(pTItem);
	}
	
	char line[64];
	sprintf_s(line, sizeof(line), "%d", pVItem->GetGradeAlexX());
	CHATMGR->AddMsg(CTC_SYSMSG, line);
}
DWORD CNewUpGrareAlexXDlg::calculateUpGradeItemMoeny()
{
	//float increase_factor = 1.20;
	float increase_factor = NEWUPGRAREALEXX_CMGR->AlexXUpGradeConfig.SetupGrade_AlexX[MoneyRate];
	CItem* pItem = ITEMMGR->GetItem(m_VirtualItemEquip.GetLinkItem()->GetDBIdx());

	real_money_total = ITEMMGR->GetUpGradeItemPercentList(real_lv_item)->Money;
	DWORD pluGrade = pItem->GetGradeAlexX() + 1;
	DWORD Xreal_fullMoney = real_money_total * pow(increase_factor, pluGrade);

	return Xreal_fullMoney;
}
void CNewUpGrareAlexXDlg::UpdateMoney()
{
	if (!m_VirtualItemEquip.GetLinkItem())
	{
		m_money_total->SetStaticText("");
		return;
	}
	char line[128];
	DWORD Xreal_fullMoney = calculateUpGradeItemMoeny();
	wsprintf(line, "%s", AddComma(Xreal_fullMoney));
	m_money_total->SetStaticText(line);
}
void CNewUpGrareAlexXDlg::clear_VirtualItemStone()
{
	if (!m_VirtualItemStone.GetLinkItem()) return;
	if (m_pIconDlgProtecion)
	{
		CVirtualItem* pVItem = NULL;

		m_pIconDlgProtecion->DeleteIcon(0, (cIcon**)&pVItem);
		if (pVItem)
			pVItem->GetLinkItem()->SetLock(FALSE);

		m_VirtualItemStone.SetLinkItem(NULL);
		m_VirtualItemStone.SetData(NULL);
	}
}
void CNewUpGrareAlexXDlg::UpdateNameItem()
{
	if (!m_VirtualItemEquip.GetLinkItem())
	{
		m_pNameIconDlgEquip->SetStaticText("");
		m_next_grade_text->SetStaticText("");
		m_grade_text->SetStaticText("");
		return;
	}
	else
	{
		//if (ITEMMGR->GetItemFrame(m_VirtualItemEquip.GetLinkItem()->GetItemIdx()))
		//{
		//	switch (ITEMMGR->GetItemFrame(m_VirtualItemEquip.GetLinkItem()->GetItemIdx())->Image2DNumFrame)
		//	{
		//	case 1:
		//		m_pNameIconDlgEquip->SetFGColor(TTTC_ALEXXITEMG1);
		//		break;
		//	case 2:
		//		m_pNameIconDlgEquip->SetFGColor(TTTC_ALEXXITEMG2);
		//		break;
		//	case 3:
		//		m_pNameIconDlgEquip->SetFGColor(TTTC_ALEXXITEMG3);
		//		break;
		//	case 4:
		//		m_pNameIconDlgEquip->SetFGColor(TTTC_ALEXXITEMG4);
		//		break;
		//	case 5:
		//		m_pNameIconDlgEquip->SetFGColor(TTTC_ALEXXITEMG5);
		//		break;
		//	case 6:
		//		m_pNameIconDlgEquip->SetFGColor(TTTC_ALEXXITEMG6);
		//		break;
		//	}
		//}
		char line[128];
		DWORD Grade = m_VirtualItemEquip.GetLinkItem()->GetGradeAlexX();
		ITEM_INFO* pItemInfo = ITEMMGR->GetItemInfo(m_VirtualItemEquip.GetLinkItem()->GetItemIdx());
		if (Grade > 0)
		{
			sprintf_s(line, sizeof(line), "%s+%d", pItemInfo->ItemName, Grade);
		}
		else
		{
			sprintf_s(line, sizeof(line), "%s", pItemInfo->ItemName);
		}
		m_pNameIconDlgEquip->SetStaticText(line);
		real_lv_item = pItemInfo->LimitLevel;
		sprintf_s(line, sizeof(line), "+%d", Grade+1);
		m_next_grade_text->SetStaticText(line);
		sprintf_s(line, sizeof(line), "+%d", Grade);
		m_grade_text->SetStaticText(line);
	}
}