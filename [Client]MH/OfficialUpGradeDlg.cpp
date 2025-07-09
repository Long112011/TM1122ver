#include "stdafx.h"
#include "OfficialUpGradeDlg.h"
#include "GameIn.h"
#include "ChatManager.h"
#include "InventoryExDialog.h"
#include "ItemManager.h"
#include "DealItem.h"
#include "cWindowManager.h"
#include "cScriptManager.h"
#include "ObjectManager.h"
#include "ObjectStateManager.h"
#include <math.h>
#include <vector>
COfficialUpGradeDlg::COfficialUpGradeDlg()
{
	cImage imgToolTip;
	SCRIPTMGR->GetImage(63, &imgToolTip, PFT_HARDPATH);
	m_MainItem.SetToolTip("", RGB_HALF(255, 255, 255), &imgToolTip);
	m_MainItem.SetMovable(FALSE);

	m_MainItem_ProTect.SetToolTip("", RGB_HALF(255, 255, 255), &imgToolTip);
	m_MainItem_ProTect.SetMovable(FALSE);
}
COfficialUpGradeDlg::~COfficialUpGradeDlg()
{
}
void COfficialUpGradeDlg::Linking()
{
	MainItem = (cIconGridDialog*)GetWindowForID(OfficialUpGradeDlg_MainItem);
	VirTual = (cIconGridDialog*)GetWindowForID(OfficialUpGradeDlg_VirTual);

	MainItem_ProTect = (cIconGridDialog*)GetWindowForID(OfficialUpGradeDlg_MainItem_ProTect);
	ClearMainItem_ProTect = (cIconGridDialog*)GetWindowForID(OfficialUpGradeDlg_MainItem_ProTect_Clear);
	ClearMainItem = (cIconGridDialog*)GetWindowForID(ITEM_STEP_REINFORCE_BTN_CLEAR_ITEM);
	ClearMainItemDEC_1 = (cIconGridDialog*)GetWindowForID(ITEM_STEP_REINFORCE_BTN_MATERIAL_DEC_1_ALL);
	ClearMainItemDEC_2 = (cIconGridDialog*)GetWindowForID(ITEM_STEP_REINFORCE_BTN_MATERIAL_DEC_2_ALL);
	ClearMainItemDEC_3 = (cIconGridDialog*)GetWindowForID(ITEM_STEP_REINFORCE_BTN_MATERIAL_DEC_3_ALL);
	ClearMainItemDEC_4 = (cIconGridDialog*)GetWindowForID(ITEM_STEP_REINFORCE_BTN_MATERIAL_DEC_4_ALL);



	ItemName = (cStatic*)GetWindowForID(OfficialUpGradeDlg_Static_ItemName);
	BgEffectItem = (cStatic*)GetWindowForID(OfficialUpGradeDlg_Static_BgEffectItem);
	GradeColor = (cStatic*)GetWindowForID(OfficialUpGradeDlg_Static_GradeColor);
	SmallGrade = (cStatic*)GetWindowForID(OfficialUpGradeDlg_Static_SmallGrade);

	for (size_t i = 0; i < 4; i++)
	{
		Btn_Plus[i] = (cButton*)GetWindowForID(OfficialUpGradeDlg_Btn_Plus_1 + i);
		Btn_Minus[i] = (cButton*)GetWindowForID(OfficialUpGradeDlg_Btn_Minus_1 + i);
		Btn_Max[i] = (cButton*)GetWindowForID(OfficialUpGradeDlg_Btn_Max_1 + i);
		Matirial_Dur[i] = (cStatic*)GetWindowForID(OfficialUpGradeDlg_Static_Dur_1 + i);
		ImgItem[i] = (cIconGridDialog*)GetWindowForID(OfficialUpGradeDlg_Static_ItemImage_1 + i);
	}
	MoneyStatic = (cStatic*)GetWindowForID(OfficialUpGradeDlg_Static_MoneyUse);
	CheckType = (cCheckBox*)GetWindowForID(OfficialUpGradeDlg_CheckBox_Type);
	BarPercent = (CObjectGuagen*)GetWindowForID(OfficialUpGradeDlg_Bar_PerCent);
	Int_Percent = (cStatic*)GetWindowForID(OfficialUpGradeDlg_TXT_INT_PerCent);
	Tod_Percent = (cStatic*)GetWindowForID(OfficialUpGradeDlg_TXT_TOD_PerCent); 


	BgState[0] = (cStatic*)GetWindowForID(OfficialUpGradeDlg_Bg_Stage_Success);
	BgState[1] = (cStatic*)GetWindowForID(OfficialUpGradeDlg_Bg_Stage_Fail);
	BgState[2] = (cStatic*)GetWindowForID(OfficialUpGradeDlg_Bg_Stage_BigFail);
	BarPercent->SetValue(0, 0);

	MoneyUse = 0;
	MoneyStatic->SetStaticText("0");
	MaxPercent = 0;
	Int_Percent->SetStaticText("0");
	Tod_Percent->SetStaticText(".0000");
	nowPercent = 0.0;
	CollectCountMat_1 = 0;
	CollectCountMat_2 = 0;
	CollectCountMat_3 = 0;
	CollectCountMat_4 = 0;
	for (size_t i = 0; i < 4; i++)
	{
		Matirial_Dur[i]->SetStaticText("0");
	}
}

void COfficialUpGradeDlg::SetActive(BOOL val)
{
	if (!val)
	{
		cIcon* pItem = MainItem->GetIconForIdx(0);
		if (pItem)
		{
			CVirtualItem* pVItem = NULL;
			MainItem->DeleteIcon(0, (cIcon**)&pVItem);
			if (pVItem)
				pVItem->GetLinkItem()->SetLock(FALSE);

			pVItem = NULL;

			m_MainItem.SetLinkItem(nullptr);
			VirTual->DeleteIcon(0, (cIcon**)&pVItem);
		}
		cIcon* pItem_Pr = MainItem_ProTect->GetIconForIdx(0);
		if (pItem_Pr)
		{
			CVirtualItem* pVItem = NULL;
			MainItem_ProTect->DeleteIcon(0, (cIcon**)&pVItem);
			if (pVItem)
				pVItem->GetLinkItem()->SetLock(FALSE);

			pVItem = NULL;

			m_MainItem_ProTect.SetLinkItem(nullptr);
		}
		ItemNeed_1 = 0;
		ItemNeed_2 = 0;
		ItemNeed_3 = 0;
		ItemNeed_4 = 0;
		for (size_t i = 0; i < 80; i++)
		{
			CItem* pItem = NULL;
			pItem = GAMEIN->GetInventoryDialog()->GetItemForPos(CollectItemPos_1[i]);
			pItem = GAMEIN->GetInventoryDialog()->GetItemForPos(CollectItemPos_2[i]);
			pItem = GAMEIN->GetInventoryDialog()->GetItemForPos(CollectItemPos_3[i]);
			pItem = GAMEIN->GetInventoryDialog()->GetItemForPos(CollectItemPos_4[i]);
			CollectItemIdx_1[i] = 0;
			CollectItemPos_1[i] = 0;

			CollectItemIdx_2[i] = 0;
			CollectItemPos_2[i] = 0;

			CollectItemIdx_3[i] = 0;
			CollectItemPos_3[i] = 0;

			CollectItemIdx_4[i] = 0;
			CollectItemPos_4[i] = 0;
		}
		for (size_t i = 0; i < 4; i++)
		{
			Matirial_Dur[i]->SetStaticText("0");
		}
		ItemName->SetActive(false);
		BgEffectItem->SetActive(false);
		GradeColor->SetActive(false);
		SmallGrade->SetActive(false);
		MoneyUse = 0;
		MoneyStatic->SetStaticText("0");
		MaxPercent = 0;
		Int_Percent->SetStaticText("0");
		Tod_Percent->SetStaticText(".0000");
		BarPercent->SetValue(0, 0);
		nowPercent = 0.0;
		CollectCountMat_1 = 0;
		CollectCountMat_2 = 0;
		CollectCountMat_3 = 0;
		CollectCountMat_4 = 0;
		SetStateBg(4);

	}
	cDialog::SetActive(val);
}

BOOL COfficialUpGradeDlg::FakeMoveIcon(LONG x, LONG y, cIcon* pOrigIcon)
{
	if (pOrigIcon->GetType() != WT_ITEM) return FALSE;

	//Daily X = ��	Y = ��
	WORD PosX = -1;
	WORD PosY = -1;

	MainItem->GetCellPosition(x, y, PosX, PosY);
	if (PosX == 0)
	{
		CItem* pItem = (CItem*)pOrigIcon;

		CDealItem* pItemCheck = (CDealItem*)MainItem->GetIconForIdx(0);
		if (pItemCheck)
			return FALSE;

		eITEM_KINDBIT bits = GetItemKind(pItem->GetItemIdx());
		if (bits == eEQUIP_ITEM_WEAPON || bits == eEQUIP_ITEM_DRESS || bits == eEQUIP_ITEM_HAT || bits == eEQUIP_ITEM_BELT
			|| bits == eEQUIP_ITEM_SHOES || bits == eEQUIP_ITEM_RING || bits == eEQUIP_ITEM_CAPE || bits == eEQUIP_ITEM_NECKLACE || bits == eEQUIP_ITEM_ARMLET )
		{

			if (pItem->IsLocked()) return FALSE;

			if (!ITEMMGR->IsEqualTableIdxForPos(eItemTable_Inventory, pItem->GetPosition())) return FALSE;

			ITEM_INFO* pinfo = ITEMMGR->GetItemInfo(pItem->GetItemBaseInfo()->wIconIdx);

			if (pItem->GetGrade() >= 30)
			{
				return false;
			}

			// 获取通用升级数据，这里不再根据 ItemIdx 获取
			UpGradeDataList* pData = GAMERESRCMNGR->GetDataUpGrade(0); // 获取通用数据
			if (!pData)
			{
				return false;
			}
			char line[128];
			pItem->SetLock(TRUE);
			MaxPercent = pData->Data_Sub[pItem->GetGrade()].Percent;
			m_MainItem.SetData(pItem->GetItemIdx());
			m_MainItem.SetLinkItem(pItem);
			MainItem->AddIcon(0, (cIcon*)&m_MainItem);
			if (ITEMMGR->IsOptionItem(pItem->GetItemIdx(), pItem->GetDurability()))
			{
				ITEMMGR->SetToolTipIcon((cIcon*)&m_MainItem, ITEMMGR->GetItemOption(pItem->GetDurability()), ITEMMGR->GetItemRareOption(pItem->GetRareness()), 0, ITEMMGR->GetItemStoneOption(pItem->GetStoneIdx()), false, pItem->GetGrade()); // ������ 2014-11-14
			}
			else
			{
				ITEMMGR->SetToolTipIcon((cIcon*)&m_MainItem, NULL, ITEMMGR->GetItemRareOption(pItem->GetRareness()), 0, ITEMMGR->GetItemStoneOption(pItem->GetStoneIdx()), false, pItem->GetGrade());  // �����ǿ������,����!
			}
			WORD Option_v = 0;
			WORD Rare_v = 0;
			WORD Grow_v = 0;
			if (ITEMMGR->IsOptionItem(pItem->GetItemIdx(), pItem->GetDurability()))
			{
				Option_v = 1;
			}
			if (ITEMMGR->IsRareOptionItem(pItem->GetItemIdx(), pItem->GetRareness()))
			{
				Rare_v = 1;
			}
			m_VirTual.Init(pItem->GetItemIdx(), 0, pItem->GetGrade() + 1, Option_v, Rare_v, Grow_v);
			m_VirTual.SetData(pItem->GetItemIdx());
			VirTual->AddIcon(0, (cIcon*)&m_VirTual);
			if (ITEMMGR->IsOptionItem(pItem->GetItemIdx(), pItem->GetDurability()))
			{
				ITEMMGR->SetToolTipIcon((cIcon*)&m_VirTual, ITEMMGR->GetItemOption(pItem->GetDurability()), ITEMMGR->GetItemRareOption(pItem->GetRareness()), 0, ITEMMGR->GetItemStoneOption(pItem->GetStoneIdx()), false, pItem->GetGrade() + 1); // ������ 2014-11-14
			}
			else
			{
				ITEMMGR->SetToolTipIcon((cIcon*)&m_VirTual, NULL, ITEMMGR->GetItemRareOption(pItem->GetRareness()), 0, ITEMMGR->GetItemStoneOption(pItem->GetStoneIdx()), false, pItem->GetGrade() + 1);  // �����ǿ������,����!
			}
			sprintf(line, "%s", pinfo->ItemName);
			ItemName->SetFGColor(RGB(255, 255, 255));
			ItemName->SetStaticText(line);
			ItemName->SetActive(true);
			BgEffectItem->SetActive(true);
			sprintf(line, "+%d", (pItem->GetGrade() + 1));
			GradeColor->SetStaticText(line);
			GradeColor->SetActive(true);
			sprintf(line, "+%d", pItem->GetGrade());
			SmallGrade->SetStaticText(line);
			if (pItem->GetGrade() > 0)
			{
				SmallGrade->SetActive(true);
			}
			SetDataMatirial();
		}
	}
	PosX = -1;
	PosY = -1;

	MainItem_ProTect->GetCellPosition(x, y, PosX, PosY);
	if (PosX == 0)
	{
		CItem* pItem = (CItem*)pOrigIcon;

		if (!m_MainItem.GetLinkItem())
		{
			return false;
		}

		if (pItem->GetItemIdx() != 55607)//防爆石头 天墨团队
		{
			return false;
		}
		pItem->SetLock(true);
		m_MainItem_ProTect.SetData(pItem->GetItemIdx());
		m_MainItem_ProTect.SetLinkItem(pItem);
		MainItem_ProTect->AddIcon(0, (cIcon*)&m_MainItem_ProTect);
		ITEMMGR->SetToolTipIcon((cIcon*)&m_MainItem_ProTect);
	}
	return FALSE;
}


void COfficialUpGradeDlg::AddVirtualItem(CItem* pItem, DWORD num, WORD Grade)
{

}

BOOL COfficialUpGradeDlg::CheckPercent()
{
	if (nowPercent < 100.0)
	{
		return true;
	}
	return false;
}

void COfficialUpGradeDlg::OnActionEvent(LONG lId, void* p, DWORD we)
{
	char line[128];
	//CHATMGR->AddMsg(CTC_SYSMSG,"%f",nowPercent);
	//------------------------------------------------------------------
	if (m_MainItem.GetLinkItem())
	{
		if (lId == OfficialUpGradeDlg_Btn_Plus_1)
		{
			if (nowPercent >= 100.0)
			{
				return;
			}
			if ((ItemNeed_1 + 1) > ItemMatirial_Have[0])
			{
				return;
			}
			ItemNeed_1++;
			sprintf(line, "%d", ItemNeed_1);
			Matirial_Dur[0]->SetStaticText(line);
			if (CheckType->IsChecked())
			{
				FindItem(ITEM_STONE_1, ITEM_STONE_1_RED, ItemNeed_1, 1);
			}
			else {
				FindItem(ITEM_STONE_1, 0, ItemNeed_1, 1);
			}


		}
		if (lId == OfficialUpGradeDlg_Btn_Minus_1)
		{
			if (ItemNeed_1 <= 0)
			{
				return;
			}
			ItemNeed_1--;
			sprintf(line, "%d", ItemNeed_1);
			Matirial_Dur[0]->SetStaticText(line);
			if (CheckType->IsChecked())
			{
				FindItem(ITEM_STONE_1, ITEM_STONE_1_RED, ItemNeed_1, 1);
			}
			else {
				FindItem(ITEM_STONE_1, 0, ItemNeed_1, 1);
			}
		}
		//-------------------------------------------------------------------
		if (lId == OfficialUpGradeDlg_Btn_Plus_2)
		{
			if (nowPercent >= 100.0)
			{
				return;
			}
			if ((ItemNeed_2 + 1) > ItemMatirial_Have[1])
			{
				return;
			}
			ItemNeed_2++;
			sprintf(line, "%d", ItemNeed_2);
			Matirial_Dur[1]->SetStaticText(line);
			if (CheckType->IsChecked())
			{
				FindItem(ITEM_STONE_2, ITEM_STONE_2_RED, ItemNeed_2, 2);
			}
			else {
				FindItem(ITEM_STONE_2, 0, ItemNeed_2, 2);
			}
		}
		if (lId == OfficialUpGradeDlg_Btn_Minus_2)
		{
			if (ItemNeed_2 <= 0)
			{
				return;
			}
			ItemNeed_2--;
			sprintf(line, "%d", ItemNeed_2);
			Matirial_Dur[1]->SetStaticText(line);
			if (CheckType->IsChecked())
			{
				FindItem(ITEM_STONE_2, ITEM_STONE_2_RED, ItemNeed_2, 2);
			}
			else {
				FindItem(ITEM_STONE_2, 0, ItemNeed_2, 2);
			}
		}
		//-------------------------------------------------------------------
		if (lId == OfficialUpGradeDlg_Btn_Plus_3)
		{
			if (nowPercent >= 100.0)
			{
				return;
			}
			if ((ItemNeed_3 + 1) > ItemMatirial_Have[2])
			{
				return;
			}
			ItemNeed_3++;
			sprintf(line, "%d", ItemNeed_3);
			Matirial_Dur[2]->SetStaticText(line);
			if (CheckType->IsChecked())
			{
				FindItem(ITEM_STONE_3, ITEM_STONE_3_RED, ItemNeed_3, 3);
			}
			else {
				FindItem(ITEM_STONE_3, 0, ItemNeed_3, 3);
			}

		}
		if (lId == OfficialUpGradeDlg_Btn_Minus_3)
		{
			if (ItemNeed_3 <= 0)
			{
				return;
			}
			ItemNeed_3--;
			sprintf(line, "%d", ItemNeed_3);
			Matirial_Dur[2]->SetStaticText(line);
			if (CheckType->IsChecked())
			{
				FindItem(ITEM_STONE_3, ITEM_STONE_3_RED, ItemNeed_3, 3);
			}
			else {
				FindItem(ITEM_STONE_3, 0, ItemNeed_3, 3);
			}
		}
		//-------------------------------------------------------------------
		if (lId == OfficialUpGradeDlg_Btn_Plus_4)
		{
			if (nowPercent >= 100.0)
			{
				return;
			}
			if ((ItemNeed_4 + 1) > ItemMatirial_Have[3])
			{
				return;
			}

			ItemNeed_4++;
			sprintf(line, "%d", ItemNeed_4);
			Matirial_Dur[3]->SetStaticText(line);
			if (CheckType->IsChecked())
			{
				FindItem(ITEM_STONE_4, ITEM_STONE_4_RED, ItemNeed_4, 4);
			}
			else {
				FindItem(ITEM_STONE_4, 0, ItemNeed_4, 4);
			}

		}
		if (lId == OfficialUpGradeDlg_Btn_Minus_4)
		{
			if (ItemNeed_4 <= 0)
			{
				return;
			}
			ItemNeed_4--;
			sprintf(line, "%d", ItemNeed_4);
			Matirial_Dur[3]->SetStaticText(line);
			if (CheckType->IsChecked())
			{
				FindItem(ITEM_STONE_4, ITEM_STONE_4_RED, ItemNeed_4, 4);
			}
			else {
				FindItem(ITEM_STONE_4, 0, ItemNeed_4, 4);
			}
		}
		if (lId == OfficialUpGradeDlg_Btn_Max_1)
		{
			if (CheckType->IsChecked())
			{
				FindMaxBtn(ITEM_STONE_1, ITEM_STONE_1_RED, 1);
			}
			else {
				FindMaxBtn(ITEM_STONE_1, 0, 1);
			}
		}
		if (lId == OfficialUpGradeDlg_Btn_Max_2)
		{
			if (CheckType->IsChecked())
			{
				FindMaxBtn(ITEM_STONE_2, ITEM_STONE_2_RED, 2);
			}
			else {
				FindMaxBtn(ITEM_STONE_2, 0, 2);
			}
		}
		if (lId == OfficialUpGradeDlg_Btn_Max_3)
		{
			if (CheckType->IsChecked())
			{
				FindMaxBtn(ITEM_STONE_3, ITEM_STONE_3_RED, 3);
			}
			else {
				FindMaxBtn(ITEM_STONE_3, 0, 3);
			}
		}

		if (lId == OfficialUpGradeDlg_Btn_Max_4)
		{
			if (CheckType->IsChecked())
			{
				FindMaxBtn(ITEM_STONE_4, ITEM_STONE_4_RED, 4);
			}
			else {
				FindMaxBtn(ITEM_STONE_4, 0, 4);
			}
		}



		if (lId == OfficialUpGradeDlg_Btn_Open)
		{
			for (size_t i = 0; i < 104; i++)
			{
				MatInfoBg[i]->SetActive(true);
			}
			Btn[0]->SetActive(false);
			Btn[1]->SetActive(true);
			if (m_MainItem.GetLinkItem())
			{
				MatCheck[m_MainItem.GetLinkItem()->GetGrade()]->SetActive(true);
			}
		}
		if (lId == OfficialUpGradeDlg_Btn_Off)
		{
			for (size_t i = 0; i < 104; i++)
			{
				MatInfoBg[i]->SetActive(false);
			}
			Btn[0]->SetActive(true);
			Btn[1]->SetActive(false);
			for (size_t i = 0; i < 30; i++)
			{
				MatCheck[i]->SetActive(false);
			}

		}

	}
	else {

	}
	if (lId == ITEM_STEP_REINFORCE_BTN_CLEAR_ITEM)
	{
		clear_VirtualItemEquip();
	}
	if (lId == OfficialUpGradeDlg_MainItem_ProTect_Clear)
	{
		clear_VirtualItemStone();
	}
	if (lId == ITEM_STEP_REINFORCE_BTN_MATERIAL_DEC_1_ALL)
	{
		ClearMaterial1();
	}
	if (lId == ITEM_STEP_REINFORCE_BTN_MATERIAL_DEC_2_ALL)
	{
		ClearMaterial2();
	}
	if (lId == ITEM_STEP_REINFORCE_BTN_MATERIAL_DEC_3_ALL)
	{
		ClearMaterial3();
	}
	if (lId == ITEM_STEP_REINFORCE_BTN_MATERIAL_DEC_4_ALL)
	{
		ClearMaterial4();
	}

	if (lId == OfficialUpGradeDlg_BTN_OK)
	{
		WORD remainingTime = 0;
		if (IsCanUsePack(remainingTime))
		{
			OkBtn(); // 开始升级操作
			m_CanUseTimeCount = gCurTime; // 记录当前时间
		}
		else
		{
			CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(2738), 3 - remainingTime);
			return;
		}
	}






	//-------------------------------------------------------------------
	if (!m_MainItem.GetLinkItem())
	{
		return;
	}
	if (we == WE_CHECKED)
	{
		if (lId == OfficialUpGradeDlg_CheckBox_Type)
		{
			ResetMatirial();
			CheckStatusItem();
		}
	}
	if (we == WE_NOTCHECKED)
	{
		if (lId == OfficialUpGradeDlg_CheckBox_Type)
		{
			ResetMatirial();
			CheckStatusItem();
		}
	}


}
BOOL COfficialUpGradeDlg::IsCanUsePack(WORD& Time)
{


	if (gCurTime - m_CanUseTimeCount >= 3000)
	{
		Time = 0;
		return TRUE;
	}
	else
	{
		Time = (gCurTime - m_CanUseTimeCount) / 1000;
		return FALSE;
	}
}
void COfficialUpGradeDlg::FindItem(DWORD ItemIdx, DWORD ItemIdx2, DWORD Count, WORD Type)
{
	for (size_t i = 0; i < 80; i++)
	{
		CItem* pItem = NULL;
		if (Type == 1)
		{
			pItem = GAMEIN->GetInventoryDialog()->GetItemForPos(CollectItemPos_1[i]);
			CollectItemPos_1[i] = 0;
			CollectItemIdx_1[i] = 0;
		}
		else if (Type == 2)
		{
			pItem = GAMEIN->GetInventoryDialog()->GetItemForPos(CollectItemPos_2[i]);
			CollectItemPos_2[i] = 0;
			CollectItemIdx_2[i] = 0;
		}
		else if (Type == 3)
		{
			pItem = GAMEIN->GetInventoryDialog()->GetItemForPos(CollectItemPos_3[i]);
			CollectItemPos_3[i] = 0;
			CollectItemIdx_3[i] = 0;
		}
		else if (Type == 4)
		{
			pItem = GAMEIN->GetInventoryDialog()->GetItemForPos(CollectItemPos_4[i]);
			CollectItemPos_4[i] = 0;
			CollectItemIdx_4[i] = 0;
		}

	}


	int CurItem = 0;//Count;
	CurItem = Count;

	int CurCount_1 = 0;
	int CurCount_2 = 0;
	int CurCount_3 = 0;
	int CurCount_4 = 0;
	for (size_t i = TP_INVENTORY_START; i < TP_INVENTORY_END; i++)
	{
		CItem* pItem = GAMEIN->GetInventoryDialog()->GetItemForPos(i);
		if (pItem && pItem->GetItemIdx() == ItemIdx || pItem && pItem->GetItemIdx() == ItemIdx2)
		{
			if (pItem->IsLocked())
			{
				continue;
			}
			CurItem -= pItem->GetDurability();
			if (Count <= 0)
			{
				break;
			}
			//CHATMGR->AddMsg(CTC_SYSMSG,"ItemIdx %d Cur %d Count %d",pItem->GetPosition(),CurItem,Count);
			if (Type == 1)
			{
				CollectItemPos_1[CurCount_1] = pItem->GetPosition();
				CollectItemIdx_1[CurCount_1] = pItem->GetItemIdx();
				if (CurItem >= 0)
				{
					CollectItemDur_1[CurCount_1] = pItem->GetDurability();
				}
				else {
					CollectItemDur_1[CurCount_1] = pItem->GetDurability() + CurItem;
				}
				CurCount_1++;
			}
			else if (Type == 2)
			{
				CollectItemPos_2[CurCount_2] = pItem->GetPosition();
				CollectItemIdx_2[CurCount_2] = pItem->GetItemIdx();
				if (CurItem >= 0)
				{
					CollectItemDur_2[CurCount_2] = pItem->GetDurability();
				}
				else {
					CollectItemDur_2[CurCount_2] = pItem->GetDurability() + CurItem;
				}
				CurCount_2++;
			}
			else if (Type == 3)
			{
				CollectItemPos_3[CurCount_3] = pItem->GetPosition();
				CollectItemIdx_3[CurCount_3] = pItem->GetItemIdx();
				if (CurItem >= 0)
				{
					CollectItemDur_3[CurCount_3] = pItem->GetDurability();
				}
				else {
					CollectItemDur_3[CurCount_3] = pItem->GetDurability() + CurItem;
				}
				CurCount_3++;
			}
			else if (Type == 4)
			{
				CollectItemPos_4[CurCount_4] = pItem->GetPosition();
				CollectItemIdx_4[CurCount_4] = pItem->GetItemIdx();
				if (CurItem >= 0)
				{
					CollectItemDur_4[CurCount_4] = pItem->GetDurability();
				}
				else {
					CollectItemDur_4[CurCount_4] = pItem->GetDurability() + CurItem;
				}
				CurCount_4++;
			}
			if (CurItem <= 0)
			{
				break;
			}

		}
	}
	SetStateBg(4);
	AllAfterAllFunction();
}
void COfficialUpGradeDlg::CheckStatusItem()
{
	for (size_t i = 0; i < 4; i++)
	{
		ItemMatirial_Have[i] = 0;
	}

	for (size_t i = 0; i < 80; i++)
	{
		CItem* pItem = GAMEIN->GetInventoryDialog()->GetItemForPos(i);
		if (CheckType->IsChecked())
		{
			if (pItem && pItem->GetItemIdx() == ITEM_STONE_1 || pItem && pItem->GetItemIdx() == ITEM_STONE_1_RED)
			{
				ItemMatirial_Have[0] += pItem->GetDurability();
			}
			if (pItem && pItem->GetItemIdx() == ITEM_STONE_2 || pItem && pItem->GetItemIdx() == ITEM_STONE_2_RED)
			{
				ItemMatirial_Have[1] += pItem->GetDurability();
			}
			if (pItem && pItem->GetItemIdx() == ITEM_STONE_3 || pItem && pItem->GetItemIdx() == ITEM_STONE_3_RED)
			{
				ItemMatirial_Have[2] += pItem->GetDurability();
			}
			if (pItem && pItem->GetItemIdx() == ITEM_STONE_4 || pItem && pItem->GetItemIdx() == ITEM_STONE_4_RED)
			{
				ItemMatirial_Have[3] += pItem->GetDurability();
			}
		}
		else {
			if (pItem && pItem->GetItemIdx() == ITEM_STONE_1)
			{
				ItemMatirial_Have[0] += pItem->GetDurability();
			}
			if (pItem && pItem->GetItemIdx() == ITEM_STONE_2)
			{
				ItemMatirial_Have[1] += pItem->GetDurability();
			}
			if (pItem && pItem->GetItemIdx() == ITEM_STONE_3)
			{
				ItemMatirial_Have[2] += pItem->GetDurability();
			}
			if (pItem && pItem->GetItemIdx() == ITEM_STONE_4)
			{
				ItemMatirial_Have[3] += pItem->GetDurability();
			}
		}
	}



	cImage ImageItem;
	SCRIPTMGR->GetImage(209, &ImageItem, PFT_HARDPATH);
	ImgItem[0]->SetBasicImage(&ImageItem);

	SCRIPTMGR->GetImage(210, &ImageItem, PFT_HARDPATH);
	ImgItem[1]->SetBasicImage(&ImageItem);

	SCRIPTMGR->GetImage(211, &ImageItem, PFT_HARDPATH);
	ImgItem[2]->SetBasicImage(&ImageItem);

	SCRIPTMGR->GetImage(212, &ImageItem, PFT_HARDPATH);
	ImgItem[3]->SetBasicImage(&ImageItem);


	if (CheckType->IsChecked())
	{
		SCRIPTMGR->GetImage(213, &ImageItem, PFT_HARDPATH);
		ImgItem[0]->SetBasicImage(&ImageItem);


		SCRIPTMGR->GetImage(214, &ImageItem, PFT_HARDPATH);
		ImgItem[1]->SetBasicImage(&ImageItem);


		SCRIPTMGR->GetImage(215, &ImageItem, PFT_HARDPATH);
		ImgItem[2]->SetBasicImage(&ImageItem);


		SCRIPTMGR->GetImage(216, &ImageItem, PFT_HARDPATH);
		ImgItem[3]->SetBasicImage(&ImageItem);

	}
	if (CheckType->IsChecked() == false)
	{
		SCRIPTMGR->GetImage(205, &ImageItem, PFT_HARDPATH);
		ImgItem[0]->SetBasicImage(&ImageItem);

		SCRIPTMGR->GetImage(206, &ImageItem, PFT_HARDPATH);
		ImgItem[1]->SetBasicImage(&ImageItem);    


		SCRIPTMGR->GetImage(207, &ImageItem, PFT_HARDPATH); 
		ImgItem[2]->SetBasicImage(&ImageItem);


		SCRIPTMGR->GetImage(208, &ImageItem, PFT_HARDPATH);
		ImgItem[3]->SetBasicImage(&ImageItem);

	}


	//ImgItem[]
}

void COfficialUpGradeDlg::AllAfterAllFunction()
{
	MoneyUse = (ItemNeed_1 * 3000000) + (ItemNeed_2 * 300000) + (ItemNeed_3 * 30000) + (ItemNeed_4 * 3000);
	MoneyStatic->SetStaticText(AddComma(MoneyUse));

	DWORD	ItemUseCount = (ItemNeed_1 * 1000) + (ItemNeed_2 * 100) + (ItemNeed_3 * 10) + (ItemNeed_4 * 1);

	float Pbar = ((float)ItemUseCount / (float)MaxPercent);
	//CHATMGR->AddMsg(CTC_SYSMSG,"pBar %.1f",Pbar);
	BarPercent->SetValue(Pbar, 0);

	nowPercent = ((float)ItemUseCount / (float)MaxPercent) * 100;

	if (nowPercent > 100.0)
	{
		nowPercent = 100.0;
	}


	float number = nowPercent;

	// แปลงเลขเป็น char
	int decimalPart = (int)((number - (int)number) * 10000);  // 56.0230 -> 230.0 -> 2300

	// แยกตัวเลขเป็น digits
	char resultChar[5];
	resultChar[0] = '0' + decimalPart / 1000 % 10;
	resultChar[1] = '0' + decimalPart / 100 % 10;
	resultChar[2] = '0' + decimalPart / 10 % 10;
	resultChar[3] = '0' + decimalPart % 10;
	resultChar[4] = '\0'; // ใส่ null terminator เพื่อให้เป็นสตริงถูกต้อง
	char line[128];
	sprintf(line, ".%s", resultChar);
	Tod_Percent->SetStaticText(line);
	sprintf(line, "%d", (int)nowPercent);
	Int_Percent->SetStaticText(line);
}

void COfficialUpGradeDlg::ResetMatirial()
{
	char line[128];
	ItemNeed_1 = 0;
	sprintf(line, "%d", ItemNeed_1);
	Matirial_Dur[0]->SetStaticText(line);
	ItemNeed_2 = 0;
	sprintf(line, "%d", ItemNeed_2);
	Matirial_Dur[1]->SetStaticText(line);
	ItemNeed_3 = 0;
	sprintf(line, "%d", ItemNeed_3);
	Matirial_Dur[2]->SetStaticText(line);
	ItemNeed_4 = 0;
	sprintf(line, "%d", ItemNeed_4);
	Matirial_Dur[3]->SetStaticText(line);
	FindItem(ITEM_STONE_1, ITEM_STONE_1_RED, 0, 1);
	FindItem(ITEM_STONE_2, ITEM_STONE_2_RED, 0, 2);
	FindItem(ITEM_STONE_3, ITEM_STONE_3_RED, 0, 3);
	FindItem(ITEM_STONE_4, ITEM_STONE_4_RED, 0, 4);
}
bool COfficialUpGradeDlg::ValidateMaterialAvailability()//检测身上物品是否足够
{
	int requiredMaterials[4] = { ItemNeed_1, ItemNeed_2, ItemNeed_3, ItemNeed_4 };
	int availableMaterials[4] = { 0, 0, 0, 0 };

	// 遍历背包中每个物品槽，统计各物品的数量
	for (size_t i = TP_INVENTORY_START; i < TP_INVENTORY_END; i++)
	{
		CItem* pItem = GAMEIN->GetInventoryDialog()->GetItemForPos(i);
		if (pItem == nullptr || pItem->IsLocked())
		{
			continue;
		}

		DWORD itemIdx = pItem->GetItemIdx();

		if (CheckType->IsChecked())
		{
			// 如果选择了红色物品的类型，统计红色类型的物品
			if (itemIdx == ITEM_STONE_1 || itemIdx == ITEM_STONE_1_RED)
				availableMaterials[0] += pItem->GetDurability();
			else if (itemIdx == ITEM_STONE_2 || itemIdx == ITEM_STONE_2_RED)
				availableMaterials[1] += pItem->GetDurability();
			else if (itemIdx == ITEM_STONE_3 || itemIdx == ITEM_STONE_3_RED)
				availableMaterials[2] += pItem->GetDurability();
			else if (itemIdx == ITEM_STONE_4 || itemIdx == ITEM_STONE_4_RED)
				availableMaterials[3] += pItem->GetDurability();
		}
		else
		{
			// 统计普通物品
			if (itemIdx == ITEM_STONE_1)
				availableMaterials[0] += pItem->GetDurability();
			else if (itemIdx == ITEM_STONE_2)
				availableMaterials[1] += pItem->GetDurability();
			else if (itemIdx == ITEM_STONE_3)
				availableMaterials[2] += pItem->GetDurability();
			else if (itemIdx == ITEM_STONE_4)
				availableMaterials[3] += pItem->GetDurability();
		}
	}

	// 检查是否满足所有需求
	for (int i = 0; i < 4; i++)
	{
		if (availableMaterials[i] < requiredMaterials[i])
		{
			CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(2743));
			return false;
		}
	}
	return true;
}

void COfficialUpGradeDlg::OkBtn()
{

	if (!m_MainItem.GetLinkItem())
	{

		CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(2746));
		return;
	}

	if (nowPercent <= 0.0)
	{
		CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(2747));
		return;
	}
	if (!ValidateMaterialAvailability())
	{
		// 如果材料不足，直接返回，不进行升级操作
		return;
	}
	// 获取物品索引和位置
	int itemIdx = m_MainItem.GetLinkItem()->GetItemIdx();
	int itemPos = m_MainItem.GetLinkItem()->GetPosition();

	// 打印物品索引和位置
//	CHATMGR->AddMsg(CTC_SYSMSG, "client Main Item Index: %d, Main Item Position: %d", itemIdx, itemPos);


	//CHATMGR->AddMsg(CTC_SYSMSG,"Max Percent %d",MaxPercent);
	MSG_OFFICIAL_ITEM_SYN msg;
	msg.init();
	msg.Category = MP_ITEMEXT;
	msg.Protocol = MP_OFFICIAL_UPGRADE_SYN;
	msg.dwObjectID = HERO->GetID();
	msg.MainItem_Idx = m_MainItem.GetLinkItem()->GetItemIdx();
	msg.MainItem_Pos = m_MainItem.GetLinkItem()->GetPosition();

	CollectCountMat_1 = CollectCountMat_2 = CollectCountMat_3 = CollectCountMat_4 = 0;
	for (size_t i = 0; i < 80; i++)
	{
		if (CollectItemIdx_1[i] == ITEM_STONE_1 || CollectItemIdx_1[i] == ITEM_STONE_1_RED)
		{
			CollectCountMat_1++;
		}
		if (CollectItemIdx_2[i] == ITEM_STONE_2 || CollectItemIdx_2[i] == ITEM_STONE_2_RED)
		{
			CollectCountMat_2++;
		}
		if (CollectItemIdx_3[i] == ITEM_STONE_3 || CollectItemIdx_3[i] == ITEM_STONE_3_RED)
		{
			CollectCountMat_3++;
		}
		if (CollectItemIdx_4[i] == ITEM_STONE_4 || CollectItemIdx_4[i] == ITEM_STONE_4_RED)
		{
			CollectCountMat_4++;
		}
	}
	msg.CountCollect = (CollectCountMat_1 + CollectCountMat_2 + CollectCountMat_3 + CollectCountMat_4);
	for (size_t i = 0; i < CollectCountMat_1; i++)
	{
		msg.AddMaterial(CollectItemIdx_1[i], CollectItemPos_1[i], CollectItemDur_1[i]);
	}
	for (size_t i = 0; i < CollectCountMat_2; i++)
	{
		msg.AddMaterial(CollectItemIdx_2[i], CollectItemPos_2[i], CollectItemDur_2[i]);
	}
	for (size_t i = 0; i < CollectCountMat_3; i++)
	{
		msg.AddMaterial(CollectItemIdx_3[i], CollectItemPos_3[i], CollectItemDur_3[i]);
	}
	for (size_t i = 0; i < CollectCountMat_4; i++)
	{
		msg.AddMaterial(CollectItemIdx_4[i], CollectItemPos_4[i], CollectItemDur_4[i]);
	}

	if (m_MainItem_ProTect.GetLinkItem())
	{
		msg.Pr_ItemIdx = m_MainItem_ProTect.GetLinkItem()->GetItemIdx();
		msg.pr_Pos = m_MainItem_ProTect.GetLinkItem()->GetPosition();
		msg.Pr_Dur = m_MainItem_ProTect.GetLinkItem()->GetDurability();
	}
	//CHATMGR->AddMsg(CTC_SYSMSG, "Protect Item Index: %d, Position: %d", msg.Pr_ItemIdx, msg.pr_Pos);
	NETWORK->Send(&msg, sizeof(msg));
}


void COfficialUpGradeDlg::RefreshAlldata(CItem* pItem)
{
	char line[128];
	ITEM_INFO* pinfo = ITEMMGR->GetItemInfo(pItem->GetItemBaseInfo()->wIconIdx);
	if (!pinfo)
	{
		return;
	}
	UpGradeDataList* pData = GAMERESRCMNGR->GetDataUpGrade(0); // 获取通用数据
	if (!pData)
	{
		return;
	}

	if (ITEMMGR->IsOptionItem(pItem->GetItemIdx(), pItem->GetDurability()))
	{
		ITEMMGR->SetToolTipIcon((cIcon*)&m_MainItem, ITEMMGR->GetItemOption(pItem->GetDurability()), ITEMMGR->GetItemRareOption(pItem->GetRareness()), 0, ITEMMGR->GetItemStoneOption(pItem->GetStoneIdx()), false, pItem->GetGrade()); // ������ 2014-11-14
	}
	else
	{
		ITEMMGR->SetToolTipIcon((cIcon*)&m_MainItem, NULL, ITEMMGR->GetItemRareOption(pItem->GetRareness()), 0, ITEMMGR->GetItemStoneOption(pItem->GetStoneIdx()), false, pItem->GetGrade());  // �����ǿ������,����!
	}
	WORD Option_v = 0;
	WORD Rare_v = 0;
	WORD Grow_v = 0;
	m_VirTual.Init(pItem->GetItemIdx(), 0, pItem->GetGrade() + 1, Option_v, Rare_v, Grow_v);
	m_VirTual.SetData(pItem->GetItemIdx());
	VirTual->AddIcon(0, (cIcon*)&m_VirTual);
	if (ITEMMGR->IsOptionItem(pItem->GetItemIdx(), pItem->GetDurability()))
	{
		Option_v = 1;
	}
	if (ITEMMGR->IsRareOptionItem(pItem->GetItemIdx(), pItem->GetRareness()))
	{
		Rare_v = 1;
	}
	if (ITEMMGR->IsOptionItem(pItem->GetItemIdx(), pItem->GetDurability()))
	{
		ITEMMGR->SetToolTipIcon((cIcon*)&m_VirTual, ITEMMGR->GetItemOption(pItem->GetDurability()), ITEMMGR->GetItemRareOption(pItem->GetRareness()), 0, ITEMMGR->GetItemStoneOption(pItem->GetStoneIdx()), false, pItem->GetGrade() + 1); // ������ 2014-11-14
	}
	else
	{
		ITEMMGR->SetToolTipIcon((cIcon*)&m_VirTual, NULL, ITEMMGR->GetItemRareOption(pItem->GetRareness()), 0, ITEMMGR->GetItemStoneOption(pItem->GetStoneIdx()), false, pItem->GetGrade() + 1);  // �����ǿ������,����!
	}
	if (pItem->GetGrade() > 0)
	{
		sprintf(line, "%s + %d", pinfo->ItemName, pItem->GetGrade());
	}
	else {
		sprintf(line, "%s", pinfo->ItemName);
	}
	for (size_t i = 0; i < 4; i++)
	{
		Matirial_Dur[i]->SetActive(false);
	}

	ItemName->SetFGColor(RGB(255, 255, 255));
	ItemName->SetStaticText(line);
	ItemName->SetActive(true);
	BgEffectItem->SetActive(true);
	sprintf(line, "+%d", (pItem->GetGrade() + 1));
	GradeColor->SetStaticText(line);
	GradeColor->SetActive(true);
	sprintf(line, "+%d", pItem->GetGrade());
	SmallGrade->SetStaticText(line);
	MaxPercent = pData->Data_Sub[pItem->GetGrade()].Percent;
	if (pItem->GetGrade() > 0)
	{
		SmallGrade->SetActive(true);
	}
	ItemNeed_1 = 0;
	sprintf(line, "%d", ItemNeed_1);
	Matirial_Dur[0]->SetStaticText(line);
	if (CheckType->IsChecked())
	{
		FindItem(ITEM_STONE_1, ITEM_STONE_1_RED, ItemNeed_1, 1);
	}
	else {
		FindItem(ITEM_STONE_1, 0, ItemNeed_1, 1);
	}
	//---------------
	ItemNeed_2 = 0;
	sprintf(line, "%d", ItemNeed_2);
	Matirial_Dur[1]->SetStaticText(line);
	if (CheckType->IsChecked())
	{
		FindItem(ITEM_STONE_2, ITEM_STONE_2_RED, ItemNeed_2, 2);
	}
	else {
		FindItem(ITEM_STONE_2, 0, ItemNeed_2, 2);
	}
	//---------------
	ItemNeed_3 = 0;
	sprintf(line, "%d", ItemNeed_3);
	Matirial_Dur[2]->SetStaticText(line);
	if (CheckType->IsChecked())
	{
		FindItem(ITEM_STONE_3, ITEM_STONE_3_RED, ItemNeed_3, 3);
	}
	else {
		FindItem(ITEM_STONE_3, 0, ItemNeed_3, 3);
	}
	//---------------
	ItemNeed_4 = 0;
	sprintf(line, "%d", ItemNeed_4);
	Matirial_Dur[3]->SetStaticText(line);
	if (CheckType->IsChecked())
	{
		FindItem(ITEM_STONE_4, ITEM_STONE_4_RED, ItemNeed_4, 4);
	}
	else {
		FindItem(ITEM_STONE_4, 0, ItemNeed_4, 4);
	}
	for (size_t i = 0; i < 4; i++)
	{
		Matirial_Dur[i]->SetActive(true);
	}
}

void COfficialUpGradeDlg::SetStateBg(WORD State)
{
	if (State == 1)
	{
		BgState[0]->SetActive(true);
		BgState[1]->SetActive(false);
		BgState[2]->SetActive(false);
	}
	else if (State == 2)
	{
		BgState[0]->SetActive(false);
		BgState[1]->SetActive(true);
		BgState[2]->SetActive(false);
	}
	else if (State == 3)
	{
		BgState[0]->SetActive(false);
		BgState[1]->SetActive(false);
		BgState[2]->SetActive(true);
	}
	else if (State == 4)
	{
		BgState[0]->SetActive(false);
		BgState[1]->SetActive(false);
		BgState[2]->SetActive(false);
	}

}

void COfficialUpGradeDlg::SetDataMatirial()
{

}
void COfficialUpGradeDlg::clear_VirtualItemEquip()
{
	cIcon* pItem = MainItem->GetIconForIdx(0);
	if (pItem)
	{
		CVirtualItem* pVItem = NULL;
		MainItem->DeleteIcon(0, (cIcon**)&pVItem);
		if (pVItem)
			pVItem->GetLinkItem()->SetLock(FALSE);

		pVItem = NULL;

		m_MainItem.SetLinkItem(nullptr);
		VirTual->DeleteIcon(0, (cIcon**)&pVItem);
	}
	ClearMaterial1();
	ClearMaterial2();
	ClearMaterial3();
	ClearMaterial4();
	clear_VirtualItemStone();
	SetStateBg(4);
	AllAfterAllFunction();
	MaxPercent = 0;
	Int_Percent->SetStaticText("0");
	Tod_Percent->SetStaticText(".0000");
	BarPercent->SetValue(0, 0);
	nowPercent = 0.0;
	ItemName->SetActive(false);
	BgEffectItem->SetActive(false);
	GradeColor->SetActive(false);
	SmallGrade->SetActive(false);



}

void COfficialUpGradeDlg::clear_VirtualItemStone()
{
	if (!m_MainItem_ProTect.GetLinkItem()) return;
	if (MainItem_ProTect)
	{
		CVirtualItem* pVItem = NULL;

		MainItem_ProTect->DeleteIcon(0, (cIcon**)&pVItem);
		if (pVItem)
			pVItem->GetLinkItem()->SetLock(FALSE);

		m_MainItem_ProTect.SetLinkItem(NULL);
		m_MainItem_ProTect.SetData(NULL);
	}
}
void COfficialUpGradeDlg::ClearMaterial1()
{
	ItemNeed_1 = 0;

	// 清除 CollectItemIdx_1 和 CollectItemPos_1
	for (size_t i = 0; i < 80; i++)
	{
		CollectItemIdx_1[i] = 0;
		CollectItemPos_1[i] = 0;
	}

	// 重置对应的 Material_Dur 文本

	AllAfterAllFunction();
	Matirial_Dur[0]->SetStaticText("0");
	// 其他和材料 1 相关的 UI 或变量重置（如果有）
}

void COfficialUpGradeDlg::ClearMaterial2()
{
	ItemNeed_2 = 0;

	for (size_t i = 0; i < 80; i++)
	{
		CollectItemIdx_2[i] = 0;
		CollectItemPos_2[i] = 0;
	}


	AllAfterAllFunction();
	Matirial_Dur[1]->SetStaticText("0");
}

void COfficialUpGradeDlg::ClearMaterial3()
{
	ItemNeed_3 = 0;

	for (size_t i = 0; i < 80; i++)
	{
		CollectItemIdx_3[i] = 0;
		CollectItemPos_3[i] = 0;
	}


	AllAfterAllFunction();
	Matirial_Dur[2]->SetStaticText("0");
}

void COfficialUpGradeDlg::ClearMaterial4()
{
	ItemNeed_4 = 0;

	for (size_t i = 0; i < 80; i++)
	{
		CollectItemIdx_4[i] = 0;
		CollectItemPos_4[i] = 0;
	}


	AllAfterAllFunction();
	Matirial_Dur[3]->SetStaticText("0");
}

void COfficialUpGradeDlg::FindMaxBtn(WORD ItemIdx1, WORD ItemIDx2, WORD Type)
{
	WORD CountItemNeed = 0;
	for (size_t i = 0; i < 80; i++)
	{
		CItem* pItem = GAMEIN->GetInventoryDialog()->GetItemForPos(i);
		if (pItem && pItem->GetItemIdx() == ItemIdx1 || pItem && pItem->GetItemIdx() == ItemIDx2)
		{
			CountItemNeed += pItem->GetDurability();
		}

	}

	if (Type == 1)
	{
		ItemNeed_1 = 0;
	}
	else if (Type == 2)
	{
		ItemNeed_2 = 0;
	}
	else if (Type == 3)
	{
		ItemNeed_3 = 0;
	}
	else if (Type == 4)
	{
		ItemNeed_4 = 0;
	}


	DWORD	ItemUseCount = (ItemNeed_1 * 1000) + (ItemNeed_2 * 100) + (ItemNeed_3 * 10) + (ItemNeed_4 * 1);

	//nowPercent = ((float)ItemUseCount/ (float)MaxPercent)*100;

	DWORD Value = 0;
	if (Type == 1)
	{
		Value = 1000;
	}
	else if (Type == 2)
	{
		Value = 100;
	}
	else if (Type == 3)
	{
		Value = 10;
	}
	else if (Type == 4)
	{
		Value = 1;
	}

	if (nowPercent >= 100.0)
	{
		return;
	}


	WORD simpleUse1 = 0;

	for (;;)
	{
		if (simpleUse1 + 1 > CountItemNeed)
		{
			break;
		}
		simpleUse1++;
		if (Type == 1)
		{
			ItemNeed_1 = simpleUse1;
		}
		if (Type == 2)
		{
			ItemNeed_2 = simpleUse1;
		}
		if (Type == 3)
		{
			ItemNeed_3 = simpleUse1;
		}
		if (Type == 4)
		{
			ItemNeed_4 = simpleUse1;
		}
		ItemUseCount = (ItemNeed_1 * 1000) + (ItemNeed_2 * 100) + (ItemNeed_3 * 10) + (ItemNeed_4 * 1);
		nowPercent = ((float)ItemUseCount / (float)MaxPercent) * 100;
		if (nowPercent >= 100.0)
		{
			break;
		}

	}
	char line[128];
	sprintf(line, "%d", simpleUse1);
	Matirial_Dur[Type - 1]->SetStaticText(line);


	FindItem(ItemIdx1, ItemIDx2, simpleUse1, Type);

}

