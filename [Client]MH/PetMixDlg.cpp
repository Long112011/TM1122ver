#include "stdafx.h"
#include "PetMixDlg.h"
#include "./interface/cStatic.h"
#include "./interface/cButton.h"
#include "WindowIDEnum.h"
#include "Pet.h"
#include "PetManager.h"
#include "GameIn.h"
#include "ObjectManager.h"
#include "ItemManager.h"
#include "ChatManager.h"
#include "ObjectGuagen.h"
#include "cIconDialog.h"
#include "GameIn.h"
#include "InventoryExDialog.h"
#include "QuickManager.h"
//#include "cFont.h"
PetMixDlg::PetMixDlg()
{
	m_ShopItemIdx = 0;
	m_ShopItemPos = 0;
	m_pOddsGuage = NULL;
	ResultPetIcon = nullptr;
	cImage imgToolTip;
	SCRIPTMGR->GetImage(63, &imgToolTip, PFT_HARDPATH);
	for (int i = 0; i < 3; i++)
	{
		VirtualItem[i].SetData(0);
		VirtualItem[i].SetLinkItem(NULL);
		VirtualItem[i].SetMovable(FALSE);
		VirtualItem[i].SetToolTip("", RGB_HALF(255, 255, 255), &imgToolTip);
	}
	for (int i = 0; i < 5; i++)
	{
		m_ImageDigit[i] = eImageDigit;
	}
	ResultPetShow.SetData(0);
	ResultPetShow.SetToolTip("", RGB_HALF(255, 255, 255), &imgToolTip);
	ResultPetShow.SetMovable(FALSE);

}


PetMixDlg::~PetMixDlg()
{
	m_ShopItemIdx = 0;
	m_ShopItemPos = 0;
	// 	for (int i = 0; i < 3; i++)
	// 	{
	// 		RemovePetItemIcon(i);
	// 	}
	// 	RemoveResultPetIcon();
}
void PetMixDlg::Linking()
{
	m_pOddsGuage = (cGuagen*)GetWindowForID(PMD_GAUGE_PROBABILITY);
	for (int i = 0; i < 3; i++)
	{
		ItemIcon[i] = (cIconDialog*)GetWindowForID(PMD_ICON_PETMAIN + i);
	}
	ResultPetIcon = (cIconDialog*)GetWindowForID(PMD_ICON_PETRESULT);
	((cStatic*)GetWindowForID(PMD_STATIC_MAINWAY))->SetActive(FALSE);
	((cStatic*)GetWindowForID(PMD_STATIC_MATERIALWAY))->SetActive(FALSE);
	((CObjectGuagen*)m_pOddsGuage)->SetValue(0, 0);
}

void PetMixDlg::OnActionEvent(LONG lId, void* p, DWORD we)
{
	if (PMD_CANCELBTN == lId)
	{
		Show(FALSE);
	}
	else if (PMD_OKBTN == lId)
	{
		MixSyn();
	}
}

void PetMixDlg::Show(BOOL val)
{
	SetActive(val);
	if (val)
	{

	}
	else
	{
		m_ShopItemIdx = 0;
		m_ShopItemPos = 0;
		for (int i = 0; i < 3; i++)
		{
			RemovePetItemIcon(i);
		}
		RemoveResultPetIcon();
	}
}

void  PetMixDlg::Render()
{
	if (!m_bActive) return;
	cDialog::Render();
	cImage ImageDigit[5];

	VECTOR2 vScale = { 1.0f, 1.0f };
	VECTOR2 pos = { m_absPos.x, m_absPos.y + 350 };

	// 	char buf1[10];
	// 	sprintf(buf1, "%.2f", 99.991);
	// 	RECT rect = { (LONG)m_absPos.x + 70, (LONG)m_absPos.y + 350, 1, 1 };
	// 	CFONT_OBJ->RenderFont(3, buf1, strlen(buf1), &rect, RGBA_MAKE(255, 0, 255, 255));
	for (int i = 0; i < 5; ++i)
	{
		if (i < 3)
			pos = { m_absPos.x + 70 + i * 25, pos.y };
		else
			pos = { m_absPos.x + 110 + i * 15, m_absPos.y + 349 + 15 };
		if (m_ImageDigit[i])
		{
			SCRIPTMGR->GetImage(m_ImageDigit[i], &ImageDigit[i], PFT_HARDPATH);
			ImageDigit[i].RenderSprite(&vScale, 0, 0, &pos, 0xffffffff);
		}
	}

}

BOOL PetMixDlg::FakeMoveIcon(LONG x, LONG y, cIcon* pOrigIcon)
{
	if (pOrigIcon->GetType() != WT_ITEM) return FALSE;
	if (m_bDisable) return FALSE;

	if (m_ShopItemIdx != eIncantation_PetMix)
		return FALSE;
	CItem* pOrigItem = (CItem*)pOrigIcon;
	if (pOrigItem->IsLocked()) return FALSE;
	eITEM_KINDBIT bits = pOrigItem->GetItemKind();
	if (!(bits & eSHOP_ITEM))
		return FALSE;
	ITEM_INFO* pItemInfo = ITEMMGR->GetItemInfo(pOrigItem->GetItemIdx());
	if (!pItemInfo) return FALSE;
	DWORD ItemParam = pOrigItem->GetItemParam();
	if ((GetTableIdxPosition(((CItem*)pOrigIcon)->GetPosition()) != eItemTable_ShopInven))
	{
		return FALSE;
	}
	if (pOrigItem->GetItemParam() != 0)
	{
		CHATMGR->AddMsg(CTC_SYSMSG, "解除封印后使用。");
		return FALSE;
	}
	if (bits == eSHOP_ITEM_PET)
	{

		PET_TOTALINFO* pPetInfo = PETMGR->GetPetInfo(pOrigItem->GetDBIdx());

		if (!pPetInfo)
		{
			CHATMGR->AddMsg(CTC_SYSMSG, "错误。");
			return FALSE;
		}
		if (!pItemInfo->GenGol)
		{
			CHATMGR->AddMsg(CTC_SYSMSG, "该宠物已经最大星级。");
			return FALSE;
		}
		if (PETMGR->IsCurPetSummonItem(pOrigItem->GetDBIdx()))
		{
			CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(1252));
			return FALSE;
		}
		if (VirtualItem[0].GetLinkItem() == NULL)
		{
			if (!IsCanMix(pOrigItem))
				return FALSE;
			if (pPetInfo->PetGrade < 3)
			{
				CHATMGR->AddMsg(CTC_SYSMSG, "只有三阶段宠物可以合成。");
				return FALSE;
			}
			if (pPetInfo->PetFriendly < GAMERESRCMNGR->GetPetRule()->MaxFriendship)
			{
				CHATMGR->AddMsg(CTC_SYSMSG, "只有三阶段亲密度100%的宠物可以合成。");
				return FALSE;
			}
			AddPetItemIcon(0, pOrigItem);
			RemoveResultPetIcon();
		}
		else
		{
			PET_TOTALINFO* pBasePetInfo = PETMGR->GetPetInfo(VirtualItem[0].GetLinkItem()->GetDBIdx());
			PET_TOTALINFO* pPetInfo = PETMGR->GetPetInfo(pOrigItem->GetDBIdx());
			ITEM_INFO* pMItemInfo = ITEMMGR->GetItemInfo(VirtualItem[0].GetData());;
			ITEM_INFO* pRItemInfo = ITEMMGR->GetItemInfo(pMItemInfo->GenGol);
			if (!pPetInfo || !pRItemInfo || !pMItemInfo || !pBasePetInfo)
			{
				return FALSE;
			}

			if (pPetInfo->PetGrade < 3 || pRItemInfo->CheRyuk - 1 != pMItemInfo->CheRyuk)
			{
				CHATMGR->AddMsg(CTC_SYSMSG, "只有相同星级的三阶段宠物可以做合成材料。");
				return FALSE;
			}
			if (pPetInfo->PetFriendly < GAMERESRCMNGR->GetPetRule()->MaxFriendship)
			{
				CHATMGR->AddMsg(CTC_SYSMSG, "只有三阶段亲密度100%的宠物可以做完材料。");
				//return FALSE;
			}
			AddPetItemIcon(1, pOrigItem);
			//添加结果宠物
			ResultPetAddIcon();
			//计算成功率显示
			CalculateSuccessRate();
		}

	}
	if (pItemInfo->ItemIdx == eIncantation_PetMixDrug)
	{
		if (!VirtualItem[0].GetLinkItem() || !VirtualItem[1].GetLinkItem())
		{
			return FALSE;
		}
		if (VirtualItem[2].GetLinkItem())
			return FALSE;
		AddPetItemIcon(2, pOrigItem);
		CalculateSuccessRate();
	}
	return FALSE;
}

void PetMixDlg::AddPetItemIcon(BYTE i, CItem* pOrigItem)
{
	VirtualItem[i].SetData(pOrigItem->GetItemIdx());
	VirtualItem[i].SetLinkItem(pOrigItem);
	ItemIcon[i]->AddIcon(0, (cIcon*)&VirtualItem[i]);
	pOrigItem->SetLock(TRUE);
	if (i == 0)
	{
		((cStatic*)GetWindowForID(PMD_STATIC_MAINWAY))->SetActive(TRUE);
	}
	if (i == 1)
	{
		((cStatic*)GetWindowForID(PMD_STATIC_MATERIALWAY))->SetActive(TRUE);
	}
}
void PetMixDlg::RemovePetItemIcon(BYTE i)
{
	if (!ItemIcon[i]->IsAddable(0))
	{
		cIcon* Icon;
		ItemIcon[i]->DeleteIcon(0, &Icon);
		// 		if (Icon)
		// 		   ((CItem*)Icon)->SetLock(FALSE);
	}
	if (VirtualItem[i].GetLinkItem())
		VirtualItem[i].GetLinkItem()->SetLock(FALSE);
	VirtualItem[i].SetData(0);
	VirtualItem[i].SetLinkItem(NULL);
	VirtualItem[i].SetMovable(FALSE);
	if (i == 0)
	{
		((cStatic*)GetWindowForID(PMD_STATIC_MAINWAY))->SetActive(FALSE);
	}
	if (i == 1)
	{
		((cStatic*)GetWindowForID(PMD_STATIC_MATERIALWAY))->SetActive(FALSE);
	}

}

void PetMixDlg::ResultPetAddIcon()
{
	if (VirtualItem[0].GetLinkItem() == FALSE)
		return;
	if (!IsCanMix((CItem*)VirtualItem[0].GetLinkItem()))
		return;
	ITEM_INFO* pItemInfo = ITEMMGR->GetItemInfo(VirtualItem[0].GetLinkItem()->GetItemIdx());
	if (!pItemInfo)
		return;

	ITEM_INFO* pRItemInfo = ITEMMGR->GetItemInfo(pItemInfo->GenGol);
	if (!pRItemInfo)
		return;
	ResultPetShow.SetData(pRItemInfo->ItemIdx);
	ResultPetShow.Init(pRItemInfo->ItemIdx, 0);
	ITEMMGR->SetToolTipIcon(&ResultPetShow);
	ResultPetIcon->AddIcon(0, (cIcon*)&ResultPetShow);

}

void PetMixDlg::RemoveResultPetIcon()
{
	if (!ResultPetIcon->IsAddable(0))
	{
		cIcon* Icon;
		ResultPetIcon->DeleteIcon(0, &Icon);
	}
	ResultPetShow.SetData(0);
	ResultPetShow.SetMovable(FALSE);
	ResultPetShow.Init(0, 0);

	for (int i = 0; i < 5; i++)
	{
		m_ImageDigit[i] = eImageDigit;
	}
	((CObjectGuagen*)m_pOddsGuage)->SetValue(0, 0);
}


BOOL PetMixDlg::IsCanMix(CItem* Pet)
{
	if (!Pet) return FALSE;
	ITEM_INFO* pMItemInfo = ITEMMGR->GetItemInfo(VirtualItem[0].GetData());;
	ITEM_INFO* pRItemInfo = ITEMMGR->GetItemInfo(pMItemInfo->GenGol);
	if (!pMItemInfo) return FALSE;

	if (!pRItemInfo) return FALSE;

	if (Pet->GetItemInfo()->GenGol == 0)
		return FALSE;
	if (pMItemInfo->GenGol != pRItemInfo->ItemIdx)
		return FALSE;
	return TRUE;
}
void PetMixDlg::CalculateSuccessRate()
{
	if (!VirtualItem[0].GetLinkItem()) return;
	ITEM_INFO* pItemInfo = ITEMMGR->GetItemInfo(VirtualItem[0].GetLinkItem()->GetItemIdx());
	ITEM_INFO* pRItemInfo = ITEMMGR->GetItemInfo(pItemInfo->GenGol);
	if (!pItemInfo || !pRItemInfo) return;

	float SuccessRate = pItemInfo->MinChub;

	if (VirtualItem[2].GetLinkItem())
	{
		WORD Dur = VirtualItem[2].GetLinkItem()->GetDurability();
		if (SuccessRate + Dur > 100)
		{
			SuccessRate = 100;
		}
		else
		{
			SuccessRate += Dur;
		}
	}
	((CObjectGuagen*)m_pOddsGuage)->SetValue(SuccessRate * 0.01, 0);
	DWORD Rate = SuccessRate * 100;
	if (Rate == 10000)
	{
		m_ImageDigit[0] = eMax_1;
		m_ImageDigit[1] = eMax_0;
		m_ImageDigit[2] = eMax_0;
		m_ImageDigit[3] = eMin_0;
		m_ImageDigit[4] = eMin_0;
	}
	else
	{
		BYTE Unit = Rate % 10;
		Rate /= 10;
		BYTE MaxTen = Rate % 10;
		Rate /= 10;
		BYTE Hundred = Rate % 10;
		Rate /= 10;
		BYTE Kilobit = Rate % 10;
		m_ImageDigit[0] = eImageDigit;
		m_ImageDigit[1] = SCRIPTMGR->SelecterImageDigit(Kilobit, TRUE);
		m_ImageDigit[2] = SCRIPTMGR->SelecterImageDigit(Hundred, TRUE);
		m_ImageDigit[3] = SCRIPTMGR->SelecterImageDigit(MaxTen, FALSE);
		m_ImageDigit[4] = SCRIPTMGR->SelecterImageDigit(Unit, FALSE);
	}
}



void PetMixDlg::MixSyn()
{
	if (!VirtualItem[0].GetLinkItem() || !VirtualItem[1].GetLinkItem())
		return;
	if (!m_ShopItemIdx || !m_ShopItemPos)
		return;
	MSG_DWORD8 msg;
	msg.Category = MP_PET;
	msg.Protocol = MP_PET_MixSyn;
	msg.dwObjectID = HERO->GetID();
	msg.dwData0 = m_ShopItemIdx;
	msg.dwData1 = m_ShopItemPos;
	msg.dwData2 = VirtualItem[0].GetLinkItem()->GetItemIdx();
	msg.dwData3 = VirtualItem[0].GetLinkItem()->GetPosition();
	msg.dwData4 = VirtualItem[1].GetLinkItem()->GetItemIdx();
	msg.dwData5 = VirtualItem[1].GetLinkItem()->GetPosition();
	if (VirtualItem[2].GetLinkItem())
	{
		msg.dwData6 = VirtualItem[2].GetLinkItem()->GetItemIdx();
		msg.dwData7 = VirtualItem[2].GetLinkItem()->GetPosition();
	}
	else
	{
		msg.dwData6 = 0;
		msg.dwData7 = 0;
	}
	NETWORK->Send(&msg, sizeof(msg));
}
void PetMixDlg::MixAck(MSG_DWORD3* pmsg)
{
	PET_TOTALINFO* pPetInfo = PETMGR->GetPetInfo(VirtualItem[0].GetLinkItem()->GetDBIdx());
	if (pmsg->dwData1 < 10 && pmsg->dwData1 > 0)
	{
		CHATMGR->AddMsg(CTC_SYSMSG, "合成失败%d。", pmsg->dwData1);
		return;
	}

	CItem* ItemOut;
	CItem* BasePet = (CItem*)VirtualItem[0].GetLinkItem();
	ITEMMGR->DeleteItem(VirtualItem[1].GetLinkItem()->GetPosition(), &ItemOut);
	RemovePetItemIcon(1);
	if (VirtualItem[2].GetLinkItem())
	{
		if (VirtualItem[2].GetLinkItem()->GetDurability() > pmsg->dwData2)
		{
			GAMEIN->GetInventoryDialog()->UpdateItemDurabilityAdd(
				VirtualItem[2].GetLinkItem()->GetPosition(), -pmsg->dwData2);
		}
		else
		{
			ITEMMGR->DeleteItem(VirtualItem[2].GetLinkItem()->GetPosition(), &ItemOut);
			RemovePetItemIcon(2);
		}
	}
	if (pmsg->dwData1 == 0)
	{
		ITEMMGR->DeleteItem(VirtualItem[0].GetLinkItem()->GetPosition(), &ItemOut);
		RemovePetItemIcon(0);
		//		CItem * shopitem = GAMEIN->GetInventoryDialog()->GetItemForPos(m_ShopItemPos);
		CHATMGR->AddMsg(CTC_SYSMSG, "合成成功。");
	}
	else
	{
		RemoveResultPetIcon();
		CHATMGR->AddMsg(CTC_SYSMSG, "合成失败%d。", pmsg->dwData1);
	}

	for (int i = 0; i < 5; i++)
	{
		m_ImageDigit[i] = eImageDigit;
	}
	((CObjectGuagen*)m_pOddsGuage)->SetValue(0, 0);
	QUICKMGR->RefreshQickItem();
}