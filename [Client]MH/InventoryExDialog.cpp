#include "stdafx.h"
#include "InventoryExDialog.h"
#include "WindowIDEnum.h"
#include "./Interface/cWindowManager.h"
#include "./Interface/cIconGridDialog.h"
#include "./Interface/cStatic.h"
#include "./Interface/cPushupButton.h"
#include "ObjectStateManager.h"
#include "GameIn.h"
#include "ItemManager.h"
#include "ObjectManager.h"
#include "KyungGongManager.h"
#include "MugongManager.h"
#include "SkillManager_Client.h"
#include "ChatManager.h"
#include "MussangManager.h"
#include "DealItem.h"
#include "cDivideBox.h"
#include "./Audio/MHAudioManager.h"
#include "MugongDialog.h"
#include "DealDialog.h"
#include "./Input/UserInput.h"
#include "PyogukDialog.h"
#include "GuildWarehouseDialog.h"
#include "MainBarDialog.h"
#include "PKManager.h"
#include "cMsgBox.h"
#include "QuestManager.h"
#include "MoveDialog.h"
#include "PointSaveDialog.h"
#include "SkillPointRedist.h"
#include "ChaseinputDialog.h"
#include "ReinforceDlg.h"
#include "MixDialog.h"
#include "CharChangeDlg.h"
#include "SealDialog.h"
#include "ChangeJobDialog.h"
#include "ReinforceResetDlg.h"
#include "NameChangeDialog.h"
#include "NameChangeNotifyDlg.h"
#include "RareCreateDialog.h"
#include "PetUpgradeDialog.h"
#include "PetRevivalDialog.h"
#include "PetWearedExDialog.h"
#include "TitanInventoryDlg.h"
#include "PetInventoryDlg.h"
#include "StreetStallManager.h"
#include "ShoutDialog.h"
#include "GoldShopDialog.h"  
#include "ItemStoneDkDialog.h" 
#include "ItemStoneXqDialog.h" 
#include "ItemStepReinforceDialog.h"
#include "ItemLockManager.h" 
#include "ItemUnLockManager.h" 
#include "SafeNumberIcon.h"   
#include "SendEquipDialog.h"      
#include "StreetStallManager.h"
#include "ShoutDialog.h"
#include "VimuManager.h"
#include "MoveManager.h"
#include "./Interface/cScriptManager.h"
#include "NpcScriptManager.h"
#include "MHMap.h"
#include "SiegeWarMgr.h"
#include "PetManager.h"
#include "TitanManager.h"
#include "GuildNoteDlg.h"
#include "UnionNoteDlg.h"
#include "SkillOptionCleardlg.h"
#include "SurvivalModeManager.h"
#include "SkinSelectDialog.h"
#include "CostumeSkinSelectDialog.h"
#include "UniqueItemCurseCancellationDlg.h"
#include "SkillPointResetDlg.h"
#include "MouseCursor.h"
#include "FortWarDialog.h"
#include "FortWarManager.h"
#include "QuickManager.h"
#include "AdPointDlg.h"
#include "ItemShopDialog.h"
#include "CharacterRBTN.h"
#include "MainGame.h"
#include "MoneyPacketDialog.h"
#include "ChangeLookDialog.h"
#include "ItemQualityDlg.h"
#include "ItemQualityChangeDlg.h"
#include "GradeChangeDlg.h"//武器升阶值转移卷
extern  int  m_PetPos;
extern BOOL IsMultiPet;
CInventoryExDialog::CInventoryExDialog()
{
	m_type = WT_INVENTORYDIALOG;
	m_pTidyMallBtn = NULL;
	m_pWearedDlg = NULL;
	m_pMoneyEdit = NULL;
	m_fSpread = FALSE;
	m_iInventoryState = eITEMDEFAULT_STATE;
	m_pItemShopInven = NULL;
	m_pItemShopInvenPlus = NULL;
	m_pMPageBtn[0] = NULL;
	m_pMPageBtn[1] = NULL;
	m_bShopMInven[0] = FALSE;
	m_bShopMInven[1] = FALSE;
	m_BtnPushstartTime = 0;
	m_BtnPushDelayTime = 500;
	m_bPushTabWithMouseOver = FALSE;
	m_pSelectedShopItem = NULL;
	m_dwItemTidyDelayTime = 0;
	m_dwItemTidyShopDelayTime = 0;
	m_pClearInvBtn[ITEMINV] = NULL;
	m_pClearInvBtn[MALLINV] = NULL;
	m_StoredLastGridPosition = 0;
	m_SelectedItemRbtn = 0;
	m_pChangeLookBtn = NULL;

	p_Money = NULL;	//返回金钱/泡点/元宝到背包
	p_PaoDian = NULL;
	p_Gold = NULL;
}
CInventoryExDialog::~CInventoryExDialog()
{
	m_pWearedDlg = NULL;
	m_pItemShopInven = NULL;
	m_pItemShopInvenPlus = NULL;
	m_pMPageBtn[0] = NULL;
	m_pMPageBtn[1] = NULL;
	m_pChangeLookBtn = NULL;

	p_Money = NULL;	//返回金钱/泡点/元宝到背包
	p_PaoDian = NULL;
	p_Gold = NULL;
}
void CInventoryExDialog::TestNewDialog(int i, CMoneyPacketDialog* pDlg)
{
	pMoneyPacketDlg = pDlg;
	pMoneyPacketDlg->Linking();
}
void CInventoryExDialog::ReleaseInventory()
{
}
void CInventoryExDialog::Render()
{
	cDialog::RenderWindow();
	cDialog::RenderComponent();
	cTabDialog::RenderTabComponent();
	static DWORD dwTime;
	if (GAMEIN->GetInventoryDialog()->IsActive())
	{
		if (gCurTime - dwTime>1500)
		{
			dwTime = gCurTime;
			CheckItemForTidyBtn();
		}
	}
}
void CInventoryExDialog::CheckItemForTidyBtn()
{
	if (this->IsActive())
	{
		if (GetItemMallCount() != 0 && (m_pMPageBtn[0]->IsPushed() == TRUE || m_pMPageBtn[1]->IsPushed() == TRUE))
			m_pClearInvBtn[MALLINV]->SetActive(true);
		else
			m_pClearInvBtn[MALLINV]->SetActive(false);
		if (GetItemCount() != 0)
			m_pClearInvBtn[ITEMINV]->SetActive(true);
		else
			m_pClearInvBtn[ITEMINV]->SetActive(false);
	}
}
void CInventoryExDialog::Add(cWindow * window)
{
	if (window->GetType() == WT_PUSHUPBUTTON
		&& window->GetID() != IN_SHOPITEMBTN1
		&& window->GetID() != IN_SHOPITEMBTN2
		&& window->GetID() != IN_CHANGELOOKBTN)
		AddTabBtn(curIdx1++, (cPushupButton *)window);
	else if (window->GetType() == WT_ICONGRIDDIALOG
		&& window->GetID() != IN_SHOPITEMGRID1
		&& window->GetID() != IN_SHOPITEMGRID2
		)
	{
		AddTabSheet(curIdx2++, window);
		((cIconGridDialog*)window)->SetDragOverIconType(WT_ITEM);
	}
	else
		cDialog::Add(window);
}
DWORD CInventoryExDialog::ActionEvent(CMouse * mouseInfo)
{
	if (WINDOWMGR->IsDragWindow() && WINDOWMGR->GetDragDlg())
	{
		if (WINDOWMGR->GetDragDlg()->GetType() == WT_ITEM)
		{
			DWORD we = WE_NULL;
			if (!m_bActive) return we;
			we = cDialog::ActionEvent(mouseInfo);
			BOOL bMouseOver = FALSE;
			for (int i = 0; i < 2; i++)
			{
				m_pMPageBtn[i]->ActionEvent(mouseInfo);
				if (we & WE_MOUSEOVER)
				if (m_pMPageBtn[i]->IsActive() && m_pMPageBtn[i]->PtInWindow(mouseInfo->GetMouseX(), mouseInfo->GetMouseY()))
				{
					bMouseOver = TRUE;
					if (m_bPushTabWithMouseOver)
					{
						if (i != 0)
						{
							m_pMPageBtn[0]->SetPush(false);
							m_bShopMInven[0] == false;
							m_pMPageBtn[1]->SetPush(true);
							m_bShopMInven[1] == true;
							OnShopItemBtnEx();
						}
						if (i != 1)
						{
							m_pMPageBtn[1]->SetPush(false);
							m_bShopMInven[1] == false;
							m_pMPageBtn[0]->SetPush(true);
							m_bShopMInven[0] == true;
							OnShopItemBtn();
						}
					}
					else
					{
						if (m_BtnPushstartTime == 0)
							m_BtnPushstartTime = gCurTime;
						else if (gCurTime - m_BtnPushstartTime > m_BtnPushDelayTime)
							m_bPushTabWithMouseOver = TRUE;
					}
				}
			}
			for (int i = 0; i < m_bTabNum; i++)
			{
				m_ppPushupTabBtn[i]->ActionEvent(mouseInfo);
				if (we & WE_MOUSEOVER)
				if (m_ppPushupTabBtn[i]->IsActive() && m_ppPushupTabBtn[i]->PtInWindow(mouseInfo->GetMouseX(), mouseInfo->GetMouseY()))
				{
					bMouseOver = TRUE;
					if (m_bPushTabWithMouseOver)
					{
						if (i != m_bSelTabNum)
						{
							SelectTab(i);
							m_bSelTabNum = i;
						}
					}
					else
					{
						if (m_BtnPushstartTime == 0)
							m_BtnPushstartTime = gCurTime;
						else if (gCurTime - m_BtnPushstartTime > m_BtnPushDelayTime)
							m_bPushTabWithMouseOver = TRUE;
					}
				}
			}
			if (!bMouseOver)
				m_BtnPushstartTime = 0;
			we |= m_ppWindowTabSheet[m_bSelTabNum]->ActionEvent(mouseInfo);
			return we;
		}
	}
	m_BtnPushstartTime = 0;
	m_bPushTabWithMouseOver = FALSE;
	return cTabDialog::ActionEvent(mouseInfo);
}
WORD CInventoryExDialog::GetTabIndex(POSTYPE absPos)
{
	return (absPos - TP_INVENTORY_START) / TABCELL_INVENTORY_NUM;
}
BOOL CInventoryExDialog::AddItem(ITEMBASE * itemBase)
{
	if (itemBase->dwDBIdx == 0)
	{
		return FALSE;
	}
	CItem * newItem = ITEMMGR->MakeNewItem(itemBase, "AddItem");
	if (newItem == NULL)
		return FALSE;
	return AddItem(newItem);
}
BOOL CInventoryExDialog::AddItem(CItem* pItem)
{
	if (pItem == NULL)
	{
		ASSERT(pItem);
		return FALSE;
	}
	ITEMMGR->RefreshItem(pItem);
	BOOL rt = FALSE;
	if (TP_INVENTORY_START <= pItem->GetPosition() && pItem->GetPosition() < TP_INVENTORY_END)
	{
		int tabIndex = GetTabIndex(pItem->GetPosition());
		cIconGridDialog * dlg = (cIconGridDialog *)GetTabSheet(tabIndex);
		WORD relPos = pItem->GetPosition() - TP_INVENTORY_START - TABCELL_INVENTORY_NUM*tabIndex;
		CItem * item = (CItem *)dlg->GetIconForIdx(relPos);
		if (item)
		{
			return FALSE;
		}
		else
		{
			rt = dlg->AddIcon(relPos, pItem);
			ITEMMGR->CheckInvenFullForAlert(CItemManager::eCBS_Inven);
		}
	}
	else if (TP_WEAR_START <= pItem->GetPosition() && pItem->GetPosition() < TP_WEAR_END)
	{
		return m_pWearedDlg->AddItem(pItem->GetPosition() - TP_WEAR_START, pItem);
	}
	else if (TP_WEAR_START <= pItem->GetPosition() && pItem->GetPosition() < TP_WEAR_END)
	{
		return m_pWearedDlg->AddItem(pItem->GetPosition() - TP_WEAR_START, pItem);
	}
	else if (m_pItemShopInven->InPt(pItem->GetPosition()))
	{
		rt = m_pItemShopInven->AddItem(pItem);
		ITEMMGR->CheckInvenFullForAlert(CItemManager::eCBS_SInven);
	}
	else if (m_pItemShopInvenPlus->InPt(pItem->GetPosition()))
	{
		rt = m_pItemShopInvenPlus->AddItem(pItem);
		ITEMMGR->CheckInvenFullForAlert(CItemManager::eCBS_SInvenPlus);
	}

	else if (GAMEIN->GetChangeLookDialog()->InPt(pItem->GetPosition()))//kiv
	{
		rt = GAMEIN->GetChangeLookDialog()->AddItem(pItem);
		//ITEMMGR->CheckInvenFullForAlert(CItemManager::eCBS_SInvenPlus);
	}

	else
		return FALSE;
	return rt;
}
BOOL CInventoryExDialog::DeleteItem(POSTYPE Pos, CItem** ppItem)
{
	if (TP_INVENTORY_START <= Pos && Pos < TP_INVENTORY_END)
	{
		int TabIndex = GetTabIndex(Pos);
		cIconGridDialog * Dlg = (cIconGridDialog *)GetTabSheet(TabIndex);
		if (!Dlg->IsAddable(Pos - TP_INVENTORY_START - TABCELL_INVENTORY_NUM*TabIndex))
			return Dlg->DeleteIcon(Pos - TP_INVENTORY_START - TABCELL_INVENTORY_NUM*TabIndex, (cIcon **)ppItem);
		else
			return FALSE;
	}
	else if (TP_WEAR_START <= Pos && Pos < TP_WEAR_END)
	{
		if (!m_pWearedDlg->IsAddable(Pos - TP_WEAR_START))
			return m_pWearedDlg->DeleteItem(Pos - TP_WEAR_START, (cIcon **)ppItem);
		else
		{
			return FALSE;
		}
	}
	else if (m_pItemShopInven->InPt(Pos))
	{
		if (!m_pItemShopInven->IsAddable(Pos - TP_SHOPINVEN_START))
			return m_pItemShopInven->DeleteItem(Pos, ppItem);
		else
		{
			return FALSE;
		}
	}
	else if (m_pItemShopInvenPlus->InPt(Pos))
	{
		if (!m_pItemShopInvenPlus->IsAddable(Pos - TP_SHOPINVEN_START - TABCELL_SHOPINVEN_NUM))
			return m_pItemShopInvenPlus->DeleteItem(Pos, ppItem);
		else
			return FALSE;
	}
	else
		return FALSE;
}
BOOL CInventoryExDialog::UpdateItemDurabilityAdd(POSTYPE absPos, int dur)
{
	if (TP_INVENTORY_START <= absPos && absPos < TP_INVENTORY_END)
	{
		int TabIndex = GetTabIndex(absPos);
		cIconGridDialog * Dlg = (cIconGridDialog *)GetTabSheet(TabIndex);
		WORD relPos = absPos - TP_INVENTORY_START - TABCELL_INVENTORY_NUM*TabIndex;
		CItem * item = (CItem *)Dlg->GetIconForIdx(relPos);
		item->SetDurability(item->GetDurability() + dur);
		return TRUE;
	}
	else if (m_pItemShopInven->InPt(absPos))
	{
		CItem* pItem = (CItem*)m_pItemShopInven->GetIconForIdx(absPos - TP_SHOPINVEN_START);
		if (pItem)
		{
			pItem->SetDurability(pItem->GetDurability() + dur);
			return TRUE;
		}
		else
			return FALSE;
	}
	else if (m_pItemShopInvenPlus->InPt(absPos))
	{
		CItem* pItem = (CItem*)m_pItemShopInvenPlus->GetIconForIdx(absPos - TP_SHOPINVEN_START - TABCELL_SHOPINVEN_NUM);
		if (pItem)
		{
			pItem->SetDurability(pItem->GetDurability() + dur);
			return TRUE;
		}
		else
			return FALSE;
	}
	else
		return FALSE;
}
CItem * CInventoryExDialog::GetItemForPos(POSTYPE absPos)
{
	if (TP_INVENTORY_START <= absPos && absPos < TP_INVENTORY_END)
	{
		int TabIndex = GetTabIndex(absPos);
		cIconGridDialog * Dlg = (cIconGridDialog *)GetTabSheet(TabIndex);
		return (CItem *)Dlg->GetIconForIdx(absPos - TP_INVENTORY_START - TABCELL_INVENTORY_NUM*TabIndex);
	}
	else if (TP_WEAR_START <= absPos && absPos < TP_WEAR_END)
	{
		return (CItem *)m_pWearedDlg->GetIconForIdx(absPos - TP_WEAR_START);
	}
	else if (m_pItemShopInven->InPt(absPos))
	{
		return (CItem*)m_pItemShopInven->GetIconForIdx(absPos - TP_SHOPINVEN_START);
	}
	else if (m_pItemShopInvenPlus->InPt(absPos))
	{
		return (CItem*)m_pItemShopInvenPlus->GetIconForIdx(absPos - TP_SHOPINVEN_START - TABCELL_SHOPINVEN_NUM);
	}

	else if (GAMEIN->GetChangeLookDialog()->InPt(absPos))//kiv
	{
		return (CItem*)GAMEIN->GetChangeLookDialog()->GetIconForIdx(absPos - TP_CHANGELOOKINVEN_START);
	}

	else
		return NULL;
}
void CInventoryExDialog::UseItem(CItem* pItem, BYTE ActionType)
{
	CObject* pObjectTemp = OBJECTMGR->GetObject(HERO->GetID());
	if (NULL == pObjectTemp)
	{
		return;
	}
	ASSERT(pItem);
	if (WINDOWMGR->IsDragWindow() && WINDOWMGR->GetDragDlg())
	if (WINDOWMGR->GetDragDlg()->GetType() == WT_ITEM)
		return;
	if (m_bDisable)	return;
	if (pItem->IsLocked()) return;
	if (HERO->GetState() == eObjectState_Die)
		return;
	if (HERO->IsHide())
	if (ITEMMGR->IsUnfitItemInfoListForHide(pItem->GetItemIdx()))
	{
		CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(1329));
		return;
	}
	eITEM_KINDBIT bits = pItem->GetItemKind();
	if (MAP->IsMapKind(eSurvivalMap))
	{
		if (!(bits == eYOUNGYAK_ITEM)
			&& !(bits & eEQUIP_ITEM)
			&& !(bits & eSHOP_ITEM_MAKEUP)
			&& !(bits & eSHOP_ITEM_DECORATION)
			&& !(bits & eSHOP_ITEM_EQUIP)
			)
		{
			CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(786));
			return;
		}
		if (8000 <= pItem->GetItemIdx() && pItem->GetItemIdx() <= 8013)
		{
			if (SVVMODEMGR->GetCurModeState() == eSVVMode_Fight)
			if (FALSE == SVVMODEMGR->AddItemUsingCount())
			{
				CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(932));
				return;
			}
		}
	}
	if (bits & eYOUNGYAK_ITEM)
	{
		YOUNGYAK_ITEM_DELAY_TYPE * Tmp = NULL;
		Tmp = GAMERESRCMNGR->GetYoungYakDelay()->GetData(pItem->GetItemIdx());
		if (Tmp)
		{
			DWORD dwRemainTime = 0;
			float delayRate = HERO->GetDelayGroup()->CheckYoungYakPotionDelay(Tmp->wtype, &dwRemainTime);
			if (delayRate > 0.0f)
			{
				CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(909));
				return;
			}
		}
		if (GAMEIN->GetGameInInitKind() == eGameInInitKind_EventMapEnter)
			return;
		if (bits == eYOUNGYAK_ITEM_PET)
		{
			if (PETMGR->GetCurSummonPet() == NULL)
			{
				CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(1263));
				return;
			}
		}
		else if (bits == eYOUNGYAK_ITEM_UPGRADE_PET)
		{
			if (HERO->GetState() != eObjectState_None && HERO->GetState() != eObjectState_Immortal)
				return;
			OBJECTSTATEMGR->StartObjectState(HERO, eObjectState_Deal);
			GAMEIN->GetPetUpgradeDialog()->SetUpgradeItemInfo(pItem->GetPosition(), pItem->GetItemIdx());
			BOOL Grade = FALSE;
			if (pItem->GetItemIdx() == 36001)	Grade = ePUDG_Grade2;
			else if (pItem->GetItemIdx() == 36002)	Grade = ePUDG_Grade3;
			GAMEIN->GetPetUpgradeDialog()->SetActive(Grade);
			return;
		}
		else if (bits == eYOUNGYAK_ITEM_REVIVAL_PET)
		{
			if (HERO->GetState() != eObjectState_None && HERO->GetState() != eObjectState_Immortal)
				return;
			OBJECTSTATEMGR->StartObjectState(HERO, eObjectState_Deal);
			GAMEIN->GetPetRevivalDialog()->SetShopItemInfo(pItem->GetItemIdx(), pItem->GetPosition());
			BOOL Grade = FALSE;
			if (pItem->GetItemIdx() == 36003)	Grade = ePRDG_Grade1;
			else if (pItem->GetItemIdx() == 36004)	Grade = ePRDG_Grade2;
			else if (pItem->GetItemIdx() == 36005)	Grade = ePRDG_Grade3;
			GAMEIN->GetPetRevivalDialog()->SetActiveWithGradeTxt(Grade);
			return;
		}
		else if (bits == eYOUNGYAK_ITEM_TITAN)
		{
			if (FALSE == HERO->InTitan())
			{
				CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(1655));
				return;
			}
			DWORD dwRemainTime = 0;
			float delayRate = HERO->GetDelayGroup()->CheckTitanPotionDelay(&dwRemainTime);
			if (delayRate > 0.0f)
			{
				CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(909));
				return;
			}
		}
		if (pItem->GetItemIdx() == 10700)
		{
			if (HERO->GetSingleSpecialState(eSingleSpecialState_Detect))
			{
				CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(1361));
				return;
			}
		}
		MSG_ITEM_USE_SYN msg;
		msg.Category = MP_ITEM;
		msg.Protocol = MP_ITEM_USE_SYN;
		msg.dwObjectID = HEROID;
		msg.TargetPos = pItem->GetPosition();
		msg.wItemIdx = pItem->GetItemIdx();
		msg.dwItemDBIdx = pItem->GetDBIdx();
		NETWORK->Send(&msg, sizeof(msg));
		if (ITEMMGR->m_nItemUseCount != 0)
		{
		}
		ITEMMGR->m_nItemUseCount++;
	}
	else if (bits & eCHANGE_ITEM)
	{
		if (bits == eCHANGE_ITEM_LOCK)
		{
			ITEM_INFO* pInfo = ITEMMGR->GetItemInfo(pItem->GetItemIdx());
			if (!pInfo)	return;
			if (HERO->GetLevel() < pInfo->ItemType)	return;
		}
		if (0 == this->GetBlankNum() && pItem->GetItemIdx() != 53125) return;
		if (!CheckItemLock() && pItem->GetItemIdx() != 53125)
		{
			return;
		}
		if (GetItemMallCount() == (40 + TABCELL_SHOPINVEN_NUM) || GetItemCount() == 80)
		{
			return;
		}
		MSG_ITEM_USE_SYN msg;
		msg.Category = MP_ITEM;
		msg.Protocol = MP_ITEM_USE_CHANGEITEM_SYN;
		msg.dwObjectID = HEROID;
		msg.TargetPos = pItem->GetPosition();
		msg.wItemIdx = pItem->GetItemIdx();
		NETWORK->Send(&msg, sizeof(msg));
	}
	else if (bits & eEQUIP_ITEM)
	{
		if (OBJECTSTATEMGR->CheckAvailableState(HERO, eCAS_EQUIPITEM) == FALSE)
		{
			CAction act;
			act.InitAction(eActionKind_EquipItem, (void*)pItem->GetDBIdx(), 0, NULL);
			HERO->SetNextAction(&act);
			return;
		}
		switch (GetTableIdxPosition(pItem->GetPosition()))
		{
		case eItemTable_Inventory:
		{
									 if (ITEMMGR->CanEquip(pItem->GetItemIdx()))
									 {
										 ITEM_INFO * pInfo = ITEMMGR->GetItemInfo(pItem->GetItemIdx());
										 if (!pInfo) return;
										 WORD EquipToPos = pInfo->EquipKind;
										 if (pInfo->EquipKind == eWearedItem_Ring1)
										 {
											 if (IsExist(EquipToPos + TP_WEAR_START))
											 {
												 if (!IsExist(EquipToPos + 1 + TP_WEAR_START))
													 EquipToPos++;
											 }
										 }
										 CItem * pToItem = GetItemForPos(EquipToPos + TP_WEAR_START);
										 FakeGeneralItemMove(EquipToPos + TP_WEAR_START, pItem, pToItem);
									 }
		}
			break;
		case eItemTable_Weared:
		{
								  POSTYPE emptyPos;
								  if (GetBlankPositionRestrictRef(emptyPos))
								  {
									  CItem * pToItem = GetItemForPos(emptyPos);
									  FakeGeneralItemMove(emptyPos, pItem, pToItem);
								  }
		}
			break;
		}
	}
	else if (bits & eMUGONG_ITEM)
	{
		if (bits == eMUGONG_ITEM_JOB)
		{
			if (!MUGONGMGR->SearchJobSkill())
			{
				CHATMGR->AddMsg(CTC_SYSMSG, "One only uses one life skill, please learn the life skill first");
				return;
			}
		}
		if (pItem->GetItemIdx() == 10750)
		{
			GAMEIN->GetSkillOptionClearDlg()->SetItem(pItem);
			GAMEIN->GetSkillOptionClearDlg()->SetActive(TRUE);
			return;
		}
		if (MUGONGMGR->GetMugongByMugongIdx(pItem->GetItemInfo()->MugongNum) != NULL)
			return;
		SKILL_CHANGE_INFO * pSkillChangeInfo = SKILLMGR->GetSkillChangeInfo(pItem->GetItemInfo()->MugongNum);
		if (pSkillChangeInfo)
		{
			if (MUGONGMGR->GetMugongByMugongIdx(pSkillChangeInfo->wTargetMugongIdx))
				return;
		}
		if (bits == eMUGONG_ITEM_TITAN)
		{
			if (HERO->InTitan() == FALSE)
			{
				CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(1653));
				return;
			}
		}
		if (bits == eMUGONG_ITEM_JINBUB)
		{
			if (!ITEMMGR->CanConvertToMugong(pItem->GetItemIdx(), MUGONGTYPE_JINBUB))
				return;
		}
		else if (bits == eMUGONG_ITEM_KYUNGGONG)
		{
			if (KYUNGGONGMGR->IsSetableKyungGong(HERO, pItem) == FALSE)
				return;
		}
		else if (bits == eMUGONG_ITEM_JOB)
		{
			if (!ITEMMGR->CanConvertToMugong(pItem->GetItemIdx(), MUGONGTYPE_JOB))
				return;
		}
		else
		{
			if (!ITEMMGR->CanConvertToMugong(pItem->GetItemIdx(), MUGONGTYPE_NORMAL))
				return;
		}
		MSG_ITEM_USE_SYN msg;
		msg.Category = MP_ITEM;
		msg.Protocol = MP_ITEM_USE_SYN;
		msg.dwObjectID = HEROID;
		msg.wItemIdx = pItem->GetItemIdx();
		msg.TargetPos = pItem->GetPosition();
		msg.dwItemDBIdx = pItem->GetDBIdx();
		NETWORK->Send(&msg, sizeof(msg));
		ITEMMGR->m_nItemUseCount++;
	}
	else if (bits & eEXTRA_ITEM)
	{
		if (pItem->GetItemIdx() == 50510)
		{
			GAMEIN->GetGuildNoteDlg()->Show(pItem);
		}
		if (pItem->GetItemIdx() == 50511)
		{
			GAMEIN->GetUnionNoteDlg()->Show(pItem);
		}
		if (bits & eEXTRA_ITEM_MATERIAL)
		{
			if (pItem->GetItemIdx() == eItemStone_Dk1 || pItem->GetItemIdx() == eItemStone_Dk2 || pItem->GetItemIdx() == eItemStone_Dk3)
			{
				CItemStoneDkDialog * pDlg = (CItemStoneDkDialog*)GAMEIN->GetItemStoneDkDlg();
				ITEM_INFO * pInfo = ITEMMGR->GetItemInfo(pItem->GetItemIdx());
				if (!pInfo)
				{
					return;
				}
				pItem->SetLock(TRUE);
				pDlg->SetItemStoneRate(pInfo->ItemGrade);
				pDlg->SetItemStone(pItem);
				if (pDlg->IsActive())
				{
					pDlg->SetActive(FALSE);
					pItem->SetLock(FALSE);
				}
				else
				{
					pDlg->SetActive(TRUE);
				}
			}
			if (pItem->GetItemIdx() == eItemStone_Xq)
			{
				CItemStoneXqDialog * pDlg = (CItemStoneXqDialog *)GAMEIN->GetItemStoneXqDlg();
				ITEM_INFO * pInfo = ITEMMGR->GetItemInfo(pItem->GetItemIdx());
				if (!pInfo)
				{
					return;
				}
				pItem->SetLock(TRUE);
				pDlg->SetXqItem(pItem);
				if (pDlg->IsActive())
				{
					pDlg->SetActive(FALSE);
					pItem->SetLock(FALSE);
				}
				else
				{
					pDlg->SetActive(TRUE);
				}
			}
			if (pItem->GetItemIdx() == eItemLockStone)
			{
				CItemLockDialog * pDlg = (CItemLockDialog*)GAMEIN->GetItemLockDialog();
				ITEM_INFO * pInfo = ITEMMGR->GetItemInfo(pItem->GetItemIdx());
				pDlg->SetLockItem(pItem);
				if (!pInfo)
				{
					return;
				}
				if (pDlg->IsActive())
				{
					pDlg->SetActive(FALSE);
					pItem->SetLock(FALSE);
				}
				else
				{
					pDlg->SetActive(TRUE);
				}
			}
			if (pItem->GetItemIdx() == eItemUnLockStone)
			{
				CItemUnLockDialog * pDlg = (CItemUnLockDialog*)GAMEIN->GetItemUnLockDialog();
				ITEM_INFO * pInfo = ITEMMGR->GetItemInfo(pItem->GetItemIdx());
				pDlg->SetLockItem(pItem);
				if (!pInfo)
				{
					return;
				}
				if (pDlg->IsActive())
				{
					pDlg->SetActive(FALSE);
					pItem->SetLock(FALSE);
				}
				else
				{
					pDlg->SetActive(TRUE);
				}
			}
		}
	}
	else if (bits & eEXTRA_ITEM_UPGRADE)
	{
	}
	else if (bits & eQUEST_ITEM)
	{
		if (bits == eQUEST_ITEM_START)
		{
			ITEM_INFO* pInfo = ITEMMGR->GetItemInfo(pItem->GetItemIdx());
			if (!pInfo)
				return;
			if (!QUESTMGR->IsQuestStarted(pInfo->ItemType))
				m_pQuestMsgBox = WINDOWMGR->MsgBox(MBI_QUEST_NONE, MBT_OK, CHATMGR->GetChatMsg(651));
			else
				m_pQuestMsgBox = WINDOWMGR->MsgBox(MBI_QUESTSTART, MBT_YESNO, CHATMGR->GetChatMsg(650));
			m_pTempQuestItem = pItem;
		}
		else if (bits == eQUEST_ITEM_PET)
		{
			if (2 == HERO->GetBattleTeam())
			{
				CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(1279));
				return;
			}
			if (HERO->GetSingleSpecialState(eSingleSpecialState_Hide))
			{
				CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(1329));
				return;
			}
			ITEM_INFO* pInfo = ITEMMGR->GetItemInfo(pItem->GetItemIdx());
			if (!pInfo)
				return;
			if (FALSE == PETMGR->CheckResummonAvailable())
			{
				CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(1254), DWORD(PETMGR->GetPetResummonRestTime() / 1000));
				return;
			}
			if (!IsMultiPet)
			{
				if (PETMGR->GetCurSummonPet() && FALSE == PETMGR->IsCurPetSummonItem(pItem->GetDBIdx()))
				{
					CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(1255));
					return;
				}
			}
			if (FALSE == PETMGR->CheckPetAlive(pItem->GetDBIdx()))
			{
				CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(1249));
				return;
			}
			if (pInfo->LimitLevel > HERO->GetMaxLevel())
			{
				CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(1242));
				return;
			}
			MOVEMGR->HeroMoveStop();
			MSG_ITEM_USE_SYN msg;
			msg.Category = MP_ITEM;
			msg.Protocol = MP_ITEM_USE_SYN;
			msg.dwObjectID = HEROID;
			msg.wItemIdx = pItem->GetItemIdx();
			msg.TargetPos = pItem->GetPosition();
			msg.dwItemDBIdx = pItem->GetDBIdx();
			NETWORK->Send(&msg, sizeof(msg));
			ITEMMGR->m_nItemUseCount++;
		}
		else if (bits == eQUEST_ITEM_EQUIP)
		{
			if (OBJECTSTATEMGR->CheckAvailableState(HERO, eCAS_EQUIPITEM) == FALSE)
			{
				CAction act;
				act.InitAction(eActionKind_EquipItem, (void*)pItem->GetDBIdx(), 0, NULL);
				HERO->SetNextAction(&act);
				return;
			}
			switch (GetTableIdxPosition(pItem->GetPosition()))
			{
			case eItemTable_Inventory:
			{
										 if (ITEMMGR->CanEquip(pItem->GetItemIdx()))
										 {
											 ITEM_INFO* pInfo = ITEMMGR->GetItemInfo(pItem->GetItemIdx());
											 if (!pInfo) return;
											 WORD EquipToPos = pInfo->EquipKind;
											 CItem* pToItem = GetItemForPos(EquipToPos + TP_WEAR_START);
											 FakeGeneralItemMove(EquipToPos + TP_WEAR_START, pItem, pToItem);
										 }
			}
				break;
			case eItemTable_Weared:
			{
									  POSTYPE emptyPos;
									  if (GetBlankPositionRestrictRef(emptyPos))
									  {
										  CItem * pToItem = GetItemForPos(emptyPos);
										  FakeGeneralItemMove(emptyPos, pItem, pToItem);
									  }
			}
				break;
			}
		}
		else if (bits == eQUEST_INSDUNGEON_ITEM_USE)
		{
			ITEM_INFO* pInfo = ITEMMGR->GetItemInfo(pItem->GetItemIdx());
			if (!pInfo)
				return;

			MSG_ITEM_USE_SYN msg;
			msg.Category = MP_ITEM;
			msg.Protocol = MP_ITEM_USE_SYN;
			msg.dwObjectID = HEROID;
			msg.wItemIdx = pItem->GetItemIdx();
			msg.TargetPos = pItem->GetPosition();

			NETWORK->Send(&msg, sizeof(msg));
		}
	}
	else if (bits & eSHOP_ITEM)
	{
		if (pItem->GetItemParam() & ITEM_PARAM_SEAL)
		{
			CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(751));
			return;
		}
		if (GAMEIN->GetGameInInitKind() == eGameInInitKind_EventMapEnter)
			return;
		ITEM_INFO * pInfo = ITEMMGR->GetItemInfo(pItem->GetItemIdx());
		if (!pInfo)	return;
		if (pInfo->ItemKind == eSHOP_ITEM_GOLDITEM)
		{
			MSGGOLDITEM msg;
			msg.Category = MP_ITEM;
			msg.Protocol = MP_ITEM_GOLDITEMUSE_SYN;
			msg.dwObjectID = HERO->GetID();
			msg.dwItemIdx = pInfo->ItemIdx;
			msg.dwItemDBidx = pItem->GetDBIdx();
			msg.dwGoldMoney = pInfo->SellPrice;
			NETWORK->Send(&msg, sizeof(msg));
			return;
		}
		if (pInfo->ItemKind == eSHOP_ITEM_FLASHITEM)
		{
			MSGFLASHNAME msg;
			msg.Category = MP_ITEMEXT;
			msg.Protocol = MP_ITEMEXT_FLASHNAME1_SYN;
			msg.dwObjectID = HERO->GetID();
			msg.dwItemIdx = pInfo->ItemIdx;
			msg.dwItemDBidx = pItem->GetDBIdx();
			msg.dwNameFlag = pInfo->SellPrice;
			NETWORK->Send(&msg, sizeof(msg));
			return;
		}
		if (pInfo->NaeRyukRecoverRate > HERO->GetLevel())
		{
			char temp[256] = { 0, };
			sprintf(temp, CHATMGR->GetChatMsg(238), (int)pInfo->NaeRyukRecoverRate);
			CHATMGR->AddMsg(CTC_SYSMSG, temp);
			return;
		}
		if (bits == eSHOP_ITEM_EQUIP)
		{
			switch (GetTableIdxPosition(pItem->GetPosition()))
			{
			case eItemTable_ShopInven:
			{
										 if (ITEMMGR->CanEquip(pItem->GetItemIdx()))
										 {
											 ITEM_INFO * pInfo = ITEMMGR->GetItemInfo(pItem->GetItemIdx());
											 if (!pInfo) return;
											 WORD EquipToPos = pInfo->EquipKind;
											 if (pInfo->EquipKind == eWearedItem_Ring1)
											 {
												 if (IsExist(EquipToPos + TP_WEAR_START))
												 {
													 if (!IsExist(EquipToPos + 1 + TP_WEAR_START))
														 EquipToPos++;
												 }
											 }
											 CItem * pToItem = GetItemForPos(EquipToPos + TP_WEAR_START);
											 FakeGeneralItemMove(EquipToPos + TP_WEAR_START, pItem, pToItem);
											 return;
										 }
			}
				break;
			case eItemTable_Weared:
			{
									  POSTYPE emptyPos;
									  if (m_pItemShopInven->GetBlankPositionRestrictRef(emptyPos))
									  {
										  CItem * pToItem = GetItemForPos(emptyPos);
										  FakeGeneralItemMove(emptyPos, pItem, pToItem);
										  return;
									  }
									  else if (m_pItemShopInvenPlus->GetBlankPositionRestrictRef(emptyPos))
									  {
										  CItem * pToItem = GetItemForPos(emptyPos);
										  FakeGeneralItemMove(emptyPos, pItem, pToItem);
										  return;
									  }
			}
				break;
			}
		}
		if (pInfo->ItemIdx == eIncantation_MemoryStone)
		{
			if (HERO->GetState() != eObjectState_None && HERO->GetState() != eObjectState_Immortal)
				return;
			DWORD Validsavenum = MAX_MOVEDATA_PERPAGE;
			if (ITEMMGR->GetUsedItemInfo(eIncantation_MemoryMoveExtend) ||
				ITEMMGR->GetUsedItemInfo(eIncantation_MemoryMove2) ||
				ITEMMGR->GetUsedItemInfo(eIncantation_MemoryMoveExtend7) ||
				ITEMMGR->GetUsedItemInfo(eIncantation_MemoryMoveExtend30))
				Validsavenum = MAX_MOVEDATA_PERPAGE*MAX_MOVEPOINT_PAGE;
			if (GAMEIN->GetMoveDialog()->GetSavedCount() >= Validsavenum)
			{
				CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(758));
				return;
			}
			DWORD dwCheckBit = eBossMap | eQuestRoom | eTournament | eInsDungeon;
			if ((MAP->IsMapKind(dwCheckBit) || MAP->IsMapSame(eRunningMap)) &&
				MAINGAME->GetUserLevel() == eUSERLEVEL_USER)
				return;
			ITEMMGR->SetDisableDialog(TRUE, eItemTable_Inventory);
			ITEMMGR->SetDisableDialog(TRUE, eItemTable_Pyoguk);
			ITEMMGR->SetDisableDialog(TRUE, eItemTable_GuildWarehouse);
			ITEMMGR->SetDisableDialog(TRUE, eItemTable_Shop);
			GAMEIN->GetPointSaveDialog()->SetItemToMapServer(pItem->GetItemIdx(), pItem->GetPosition());
			GAMEIN->GetPointSaveDialog()->SetDialogStatus(TRUE);
			GAMEIN->GetPointSaveDialog()->SetActive(TRUE);
			OBJECTSTATEMGR->StartObjectState(HERO, eObjectState_Deal);
		}
		else if (pInfo->ItemIdx == eIncantation_TownMove || pInfo->ItemIdx == eIncantation_MemoryMove)
		{
			if (HERO->GetState() != eObjectState_None && HERO->GetState() != eObjectState_Immortal)
			{
				return;
			}
			ITEMMGR->SetDisableDialog(TRUE, eItemTable_Inventory);
			ITEMMGR->SetDisableDialog(TRUE, eItemTable_Pyoguk);
			ITEMMGR->SetDisableDialog(TRUE, eItemTable_GuildWarehouse);
			ITEMMGR->SetDisableDialog(TRUE, eItemTable_Shop);
			GAMEIN->GetMoveDialog()->SetItemToMapServer(pItem->GetItemIdx(), pItem->GetPosition(), TRUE);
			if (pInfo->ItemIdx == eIncantation_TownMove)
			{
				GAMEIN->GetMoveDialog()->SetTownMoveView(TRUE);
				GAMEIN->GetMoveDialog()->SetActive(TRUE);
			}
			else
			{
				GAMEIN->GetMoveDialog()->SetTownMoveView(FALSE);
				MSGBASE msg;
				msg.Category = MP_ITEM;
				msg.Protocol = MP_ITEM_SHOPITEM_MPINFO;
				msg.dwObjectID = HERO->GetID();
				NETWORK->Send(&msg, sizeof(msg));
			}
			OBJECTSTATEMGR->StartObjectState(HERO, eObjectState_Deal);
		}
		else if (pInfo->ItemKind == eSHOP_ITEM_INCANTATION && pInfo->LimitLevel)
		{
			if (HERO->GetState() != eObjectState_None && HERO->GetState() != eObjectState_Immortal)
			{
				return;
			}
			CObject* pObject = OBJECTMGR->GetSelectedObject();
			if (pObject == NULL || pObject == HERO || pObject->GetObjectKind() != eObjectKind_Player)
			{
				CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(763));
				return;
			}
			ITEMMGR->SetDisableDialog(TRUE, eItemTable_Inventory);
			ITEMMGR->SetDisableDialog(TRUE, eItemTable_Pyoguk);
			ITEMMGR->SetDisableDialog(TRUE, eItemTable_GuildWarehouse);
			ITEMMGR->SetDisableDialog(TRUE, eItemTable_Shop);
			char buf[128] = { 0, };
			wsprintf(buf, CHATMGR->GetChatMsg(761), pObject->GetObjectName());
			m_pSelectedShopItem = pItem;
			WINDOWMGR->MsgBox(MBI_OTHERREVIVE, MBT_YESNO, buf);
		}
		else if (pInfo->ItemIdx == eIncantation_CharChange || pInfo->ItemIdx == eIncantation_ShapeChange)
		{
			if (MAP->IsTitanMap())
			{
				CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(1580));
				return;
			}
			if (HERO->GetState() != eObjectState_None && HERO->GetState() != eObjectState_Immortal)
			{
				return;
			}
			for (int i = 0; i<eAvatar_Effect; ++i)
			{
				if (HERO->GetShopItemStats()->Avatar[i])
				{
					CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(1184));
					return;
				}
			}
			this->SetActive(false);
			ITEMMGR->SetDisableDialog(TRUE, eItemTable_Inventory);
			ITEMMGR->SetDisableDialog(TRUE, eItemTable_Pyoguk);
			ITEMMGR->SetDisableDialog(TRUE, eItemTable_GuildWarehouse);
			ITEMMGR->SetDisableDialog(TRUE, eItemTable_Shop);
			GAMEIN->GetCharChangeDlg()->SetItemInfo(pItem->GetPosition());
			GAMEIN->GetCharChangeDlg()->SetCharacterInfo(HERO->GetCharacterTotalInfo());
			if (pInfo->ItemIdx == eIncantation_CharChange)
				GAMEIN->GetCharChangeDlg()->SetShapeChange(FALSE);
			else
				GAMEIN->GetCharChangeDlg()->SetShapeChange(TRUE);
			WINDOWMGR->ToggleShowInterface();
			GAMEIN->GetCharChangeDlg()->SetActive(TRUE);
			OBJECTSTATEMGR->StartObjectState(HERO, eObjectState_Deal);
		}
		else if (pInfo->ItemIdx == eIncantation_ItemSeal)
		{
			if (HERO->GetState() != eObjectState_None && HERO->GetState() != eObjectState_Immortal)
			{
				return;
			}
			OBJECTSTATEMGR->StartObjectState(HERO, eObjectState_Deal);
			if (GAMEIN->GetSealDlg())
			{
				GAMEIN->GetSealDlg()->SetItemInfo(pItem->GetItemIdx(), pItem->GetPosition());
				GAMEIN->GetSealDlg()->SetActive(TRUE);
			}
		}
		else if (pInfo->ItemIdx == eIncantation_ReinforceReset)
		{
			if (HERO->GetState() != eObjectState_None && HERO->GetState() != eObjectState_Immortal)
			{
				return;
			}
			OBJECTSTATEMGR->StartObjectState(HERO, eObjectState_Deal);
			if (GAMEIN->GetReinforceResetDlg())
			{
				GAMEIN->GetReinforceResetDlg()->SetItemInfo(pItem->GetItemIdx(), pItem->GetPosition());
				GAMEIN->GetReinforceResetDlg()->SetActive(TRUE);
			}
		}
		else if (pInfo->ItemKind == eSHOP_ITEM_MAKEUP || pInfo->ItemKind == eSHOP_ITEM_DECORATION ||
			pInfo->ItemKind == eSHOP_ITEM_IMAGENAME)
		{
			AVATARITEM* pAvatarItem = GAMERESRCMNGR->m_AvatarEquipTable.GetData(pInfo->ItemIdx);
			if (!pAvatarItem)
				return;
			if (pAvatarItem->Gender < 2 && pAvatarItem->Gender != HERO->GetGender())
			{
				CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(778));
				return;
			}
			else
			{
				if (HERO->GetState() == eObjectState_Move)
					MOVEMGR->HeroMoveStop();
				else if (HERO->GetState() != eObjectState_None && HERO->GetState() != eObjectState_Immortal)
				{
					CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(798));
					return;
				}
				MSG_WORD2 msg;
				msg.Category = MP_ITEM;
				if (pItem->GetUseParam())
					msg.Protocol = MP_ITEM_SHOPITEM_AVATAR_TAKEOFF;
				else
					msg.Protocol = MP_ITEM_SHOPITEM_AVATAR_USE_SYN;
				msg.dwObjectID = HEROID;
				msg.wData1 = pItem->GetItemIdx();
				msg.wData2 = pItem->GetPosition();
				NETWORK->Send(&msg, sizeof(msg));
			}
		}
		else if (pInfo->ItemKind == eSHOP_ITEM_INCANTATION && pInfo->Life)
		{
			if (HERO->GetState() != eObjectState_None && HERO->GetState() != eObjectState_Immortal)
			{
				GAMEIN->GetSkPointDlg()->SetCurItem(0, 0);
				return;
			}
			GAMEIN->GetSkPointDlg()->SetCurItem(pItem->GetItemIdx(), pItem->GetPosition());
			char buf[128];
			sprintf(buf, CHATMGR->GetChatMsg(737), pInfo->Life);
			WINDOWMGR->MsgBox(MBI_SKPOINTREDIST_USE, MBT_YESNO, buf);
		}
		else if (pInfo->ItemIdx == eIncantation_TownMove15 ||
			pInfo->ItemIdx == eIncantation_MemoryMove15 ||
			pInfo->ItemIdx == eIncantation_TownMove7 ||
			pInfo->ItemIdx == eIncantation_TownMove7_NoTrade ||
			pInfo->ItemIdx == eIncantation_MemoryMove7 ||
			pInfo->ItemIdx == eIncantation_MemoryMove7_NoTrade ||
			pInfo->ItemIdx == 55357 ||
			pInfo->ItemIdx == 55362 ||
			pInfo->ItemIdx == eIncantation_MemoryMoveExtend ||
			pInfo->ItemIdx == eIncantation_MemoryMove2 ||
			pInfo->ItemIdx == eIncantation_MemoryMoveExtend7 ||
			pInfo->ItemIdx == eIncantation_MemoryMoveExtend30)
		{
			if (HERO->GetState() != eObjectState_None && HERO->GetState() != eObjectState_Immortal)
				return;
			ITEMMGR->SetDisableDialog(TRUE, eItemTable_Inventory);
			ITEMMGR->SetDisableDialog(TRUE, eItemTable_Pyoguk);
			ITEMMGR->SetDisableDialog(TRUE, eItemTable_GuildWarehouse);
			ITEMMGR->SetDisableDialog(TRUE, eItemTable_Shop);
			GAMEIN->GetMoveDialog()->SetItemToMapServer(pItem->GetItemIdx(), pItem->GetPosition(), TRUE);
			if (pInfo->ItemIdx == eIncantation_TownMove15 ||
				pInfo->ItemIdx == eIncantation_TownMove7 ||
				pInfo->ItemIdx == eIncantation_TownMove7_NoTrade)
			{
				GAMEIN->GetMoveDialog()->SetTownMoveView(TRUE);
				GAMEIN->GetMoveDialog()->SetActive(TRUE);
			}
			else
			{
				GAMEIN->GetMoveDialog()->SetTownMoveView(FALSE);
				MSGBASE msg;
				msg.Category = MP_ITEM;
				msg.Protocol = MP_ITEM_SHOPITEM_MPINFO;
				msg.dwObjectID = HERO->GetID();
				NETWORK->Send(&msg, sizeof(msg));
			}
			OBJECTSTATEMGR->StartObjectState(HERO, eObjectState_Deal);
		}
		else if (pInfo->ItemIdx == eIncantation_ShowPyoguk
			|| pInfo->ItemIdx == eIncantation_ShowPyoguk7
			|| pInfo->ItemIdx == eIncantation_ShowPyoguk7_NoTrade)
		{
			if (HERO->GetState() != eObjectState_None && HERO->GetState() != eObjectState_Immortal)
				return;
			NPCSCRIPTMGR->StartNpcBusiness(0, eNpcParam_ShowpPyoguk);
		}
		else if (pInfo->ItemIdx == eSundries_RareItemCreate50 ||
			pInfo->ItemIdx == eSundries_RareItemCreate70 ||
			pInfo->ItemIdx == eSundries_RareItemCreate90 ||
			pInfo->ItemIdx == eSundries_RareItemCreate99
			//|| 
			//
			//pInfo->ItemIdx == eSpecialBlessingStoneSTR ||
			//pInfo->ItemIdx == eSpecialBlessingStoneAGI ||
			//pInfo->ItemIdx == eSpecialBlessingStoneCON ||
			//pInfo->ItemIdx == eSpecialBlessingStoneINT
			)




		{
		if (HERO->GetState() != eObjectState_None && HERO->GetState() != eObjectState_Immortal)
				return;
			OBJECTSTATEMGR->StartObjectState(HERO, eObjectState_Deal);
			GAMEIN->GetRareCreateDlg()->SetShopItemInfo(pItem->GetItemIdx(), pItem->GetPosition());
			GAMEIN->GetRareCreateDlg()->Clear();
			GAMEIN->GetRareCreateDlg()->SetActive(TRUE);
		}
#ifdef _JUEXING_



		else if (pInfo->ItemIdx == eItemStone_Quality1_50 ||
			pInfo->ItemIdx == eItemStone_Quality51_70 ||
			pInfo->ItemIdx == eItemStone_Quality71_999)
			{
				if (HERO->GetState() != eObjectState_None && HERO->GetState() != eObjectState_Immortal)
					return;

				OBJECTSTATEMGR->StartObjectState(HERO, eObjectState_Deal);

				CItemQualityDlg* pDlg = (CItemQualityDlg*)GAMEIN->GetItemQualityDlg();

				ITEM_INFO* pInfo = ITEMMGR->GetItemInfo(pItem->GetItemIdx());

				if (!pInfo)
				{
					return;
				}
				pItem->SetLock(TRUE);
				pDlg->SetShopItemInfo(pItem->GetItemIdx(), pItem->GetPosition());
				if (pDlg->IsActive())
				{
					pDlg->SetActive(FALSE);
					pItem->SetLock(FALSE);
				}
				else
				{
					pDlg->SetActive(TRUE);
				}
		}


		else if (pInfo->ItemIdx >= 51513 && pInfo->ItemIdx <= 51539)
		{
			if (HERO->GetState() != eObjectState_None && HERO->GetState() != eObjectState_Immortal)
				return;

			OBJECTSTATEMGR->StartObjectState(HERO, eObjectState_Deal);

			CItemQualityChangeDlg* pDlg = (CItemQualityChangeDlg*)GAMEIN->GetItemQualityChangeDlg();

			ITEM_INFO* pInfo = ITEMMGR->GetItemInfo(pItem->GetItemIdx());

			if (!pInfo)
			{
				return;
			}
			pItem->SetLock(TRUE);
			pDlg->SetShopItemInfo(pItem->GetItemIdx(), pItem->GetPosition());
			if (pDlg->IsActive())
			{
				pDlg->SetActive(FALSE);
				pItem->SetLock(FALSE);
			}
			else
			{
				pDlg->SetActive(TRUE);
			}
			}

#endif // _JUEXING_
		else if (pInfo->ItemIdx == eSundries_GradeChange)	//武器升阶值转移卷
		{
			if (HERO->GetState() != eObjectState_None && HERO->GetState() != eObjectState_Immortal)
				return;

			OBJECTSTATEMGR->StartObjectState(HERO, eObjectState_Deal);
			GAMEIN->GetGradeChangeDlg()->SetShopItemInfo(pItem->GetItemIdx(), pItem->GetPosition());
			GAMEIN->GetGradeChangeDlg()->SetActive(TRUE);
			}

		else if (pInfo->ItemIdx == eIncantation_PetRevival)
		{
			if (HERO->GetState() != eObjectState_None && HERO->GetState() != eObjectState_Immortal)
				return;
			OBJECTSTATEMGR->StartObjectState(HERO, eObjectState_Deal);
			GAMEIN->GetPetRevivalDialog()->SetShopItemInfo(pItem->GetItemIdx(), pItem->GetPosition());
			GAMEIN->GetPetRevivalDialog()->SetActive(TRUE);
		}
		else if (pInfo->ItemIdx == eIncantation_PetGrade2
			|| pInfo->ItemIdx == eIncantation_PetGrade3)
		{
			if (HERO->GetState() != eObjectState_None && HERO->GetState() != eObjectState_Immortal)
				return;
			OBJECTSTATEMGR->StartObjectState(HERO, eObjectState_Deal);
			GAMEIN->GetPetUpgradeDialog()->SetUpgradeItemInfo(pItem->GetPosition(), pItem->GetItemIdx());
			BOOL Grade = FALSE;
			if (pItem->GetItemIdx() == eIncantation_PetGrade2)	Grade = ePUDG_Grade2;
			else if (pItem->GetItemIdx() == eIncantation_PetGrade3)	Grade = ePUDG_Grade3;
			GAMEIN->GetPetUpgradeDialog()->SetActive(Grade);
			return;
		}
		else if (pInfo->ItemKind == eSHOP_ITEM_SUNDRIES && pInfo->GenGol)
		{
			if (pItem->GetItemIdx() >= 55633 && pItem->GetItemIdx() <= 55635)
			{
				GAMEIN->GetReinforceDialog()->Release();
				GAMEIN->GetReinforceDialog()->SetActiveRecursive(FALSE);
				GAMEIN->GetReinforceDialog()->SetShopItemInfo(pItem->GetItemIdx(), pItem->GetPosition());
				pItem->SetLock(TRUE);
				GAMEIN->GetReinforceDialog()->SetActiveRecursive(TRUE);
			}
			if (pItem->GetItemIdx() >= 55636 && pItem->GetItemIdx() <= 55638)
			{
				GAMEIN->GetMixDialog()->Release();
				GAMEIN->GetMixDialog()->SetActiveRecursive(FALSE);
				GAMEIN->GetMixDialog()->SetShopItemInfo(pItem->GetItemIdx(), pItem->GetPosition());
				pItem->SetLock(TRUE);
				GAMEIN->GetMixDialog()->SetActiveRecursive(TRUE);
			}
			return;
		}
		else if (pInfo->ItemKind == eSHOP_ITEM_SUNDRIES && pInfo->CheRyuk)
		{
			if (GAMEIN->GetGameInInitKind() == eGameInInitKind_SuryunEnter ||
				GAMEIN->GetGameInInitKind() == eGameInInitKind_EventMapEnter)
				return;
			if ((HERO->GetState() == eObjectState_None || HERO->GetState() == eObjectState_Immortal)
				&& !VIMUMGR->IsVimuing())
			{
				STREETSTALLMGR->ChangeDialogState();
			}
		}
		else if (pInfo->ItemIdx == eIncantation_ChangeJob)
		{
			if (HERO->GetState() != eObjectState_None && HERO->GetState() != eObjectState_Immortal)
				return;
			if (HERO->GetStage() != eStage_Hwa && HERO->GetStage() != eStage_Geuk)
				return;
			ITEMMGR->SetDisableDialog(TRUE, eItemTable_Inventory);
			ITEMMGR->SetDisableDialog(TRUE, eItemTable_Pyoguk);
			ITEMMGR->SetDisableDialog(TRUE, eItemTable_GuildWarehouse);
			ITEMMGR->SetDisableDialog(TRUE, eItemTable_Shop);
			OBJECTSTATEMGR->StartObjectState(HERO, eObjectState_Deal);
			GAMEIN->GetChangeJobDlg()->SetActive(TRUE);
			GAMEIN->GetChangeJobDlg()->SetItemInfo(pItem->GetPosition(), pItem->GetDBIdx());
		}
		else if (pInfo->ItemIdx == eIncantation_ChangeName || pInfo->ItemIdx == eIncantation_ChangeName_Dntrade)
		{
			if (HERO->IsPKMode() || VIMUMGR->IsVimuing() || PKMGR->IsPKLooted() || (HERO->GetState() != eObjectState_None && HERO->GetState() != eObjectState_Immortal))
				return;
			GAMEIN->GetNameChangeNotifyDlg()->SetActive(TRUE);
			GAMEIN->GetInventoryDialog()->SetDisable(TRUE);
		}
		else if (pInfo->ItemIdx == 59707)
		{
			if (HERO->IsPKMode() || VIMUMGR->IsVimuing() || PKMGR->IsPKLooted() || (HERO->GetState() != eObjectState_None && HERO->GetState() != eObjectState_Immortal))
				return;
			GAMEIN->GetNameChangeNotifyDlg()->SetActive(TRUE);
			GAMEIN->GetInventoryDialog()->SetDisable(TRUE);
		}
		else if (pInfo->ItemIdx == eSundries_Shout || pInfo->ItemIdx == eSundries_Shout_Once || pInfo->ItemIdx == eSundries_Shout_Once_NoTrade)
		{
			GAMEIN->GetShoutDlg()->SetActive(TRUE);
			GAMEIN->GetShoutDlg()->SetItemInfo(pInfo->ItemIdx, pItem->GetPosition());
			return;
		}
		else if (pInfo->ItemIdx == eSundries_ScreenShout || pInfo->ItemIdx == eSundries_ScreenShout_Once)
		{
			GAMEIN->GetShoutDlg()->SetActive(TRUE);
			GAMEIN->GetShoutDlg()->SetItemInfo(pInfo->ItemIdx, pItem->GetPosition(), 1);
			return;
		}
		else if (pInfo->ItemIdx == eSundries_SuperShout || pInfo->ItemIdx == eSundries_SuperShout_Once)
		{
			GAMEIN->GetShoutDlg()->SetActive(TRUE);
			GAMEIN->GetShoutDlg()->SetItemInfo(pInfo->ItemIdx, pItem->GetPosition(), 2);
			return;
		}
		else if (pInfo->ItemIdx == eIncantation_MugongOptionReset)
		{
			if (GAMEIN->GetSkillOptionClearDlg()->IsActive())
				return;
			pItem->SetLock(TRUE);
			GAMEIN->GetSkillOptionClearDlg()->SetItem(pItem);
			GAMEIN->GetSkillOptionClearDlg()->SetActive(TRUE);
			return;
		}
		else if (pInfo->ItemKind == eSHOP_ITEM_PET_EQUIP)
		{
			if (PETMGR->GetCurSummonPet() == NULL)
			{
				CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(1263));
				return;
			}
			CPetWearedExDialog* pPetEquipDlg = ((CPetInventoryDlg*)GAMEIN->GetPetInventoryDialog())->GetPetWearedDlg();
			POSTYPE	EmptyPos = 0;
			if (pPetEquipDlg->GetBlankPositionRestrictRef(EmptyPos))
			{
				CItem* pToItem = GAMEIN->GetPetInventoryDialog()->GetItemForPos(EmptyPos);
				FakeGeneralItemMove(EmptyPos, pItem, pToItem);
				return;
			}
			else
			{
				CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(1287));
				return;
			}
		}
		else if (pInfo->ItemIdx == eIncantation_Curse_Cancellation)
		{
			GAMEIN->GetUniqueItemCurseCancellationDlg()->SetShopItemPosion(pItem->GetPosition());
			GAMEIN->GetUniqueItemCurseCancellationDlg()->SetActive(TRUE);
			OBJECTSTATEMGR->StartObjectState(HERO, eObjectState_Deal);
		}
		else if (pInfo->ItemKind == eSHOP_ITEM_TITAN_EQUIP)
		{
			switch (GetTableIdxPosition(pItem->GetPosition()))
			{
			case eItemTable_ShopInven:
			{
										 POSTYPE	EmptyPos = 0;
										 if (GAMEIN->GetTitanInventoryDlg()->GetBlankSlotPosition(EmptyPos))
										 {
											 CItem* pToItem = GAMEIN->GetTitanInventoryDlg()->GetItemForPos(EmptyPos);
											 FakeGeneralItemMove(EmptyPos, pItem, pToItem);
											 return;
										 }
										 else
										 {
											 CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(1680));
											 return;
										 }
			}
				break;
			case eItemTable_TitanShopItem:
			{
											 POSTYPE EmptyPos = 0;
											 if (m_pItemShopInven->GetBlankPositionRestrictRef(EmptyPos) == TRUE)
											 {
												 CItem* pToItem = GetItemForPos(EmptyPos);
												 FakeGeneralItemMove(EmptyPos, pItem, pToItem);
											 }
											 else if (m_pItemShopInvenPlus->GetBlankPositionRestrictRef(EmptyPos) == TRUE)
											 {
												 CItem* pToItem = GetItemForPos(EmptyPos);
												 FakeGeneralItemMove(EmptyPos, pItem, pToItem);
											 }
			}
				break;
			}
		}
		else if (pInfo->ItemKind == eSHOP_ITEM_NOMALCLOTHES_SKIN)
		{
			if (GAMEIN->GetSkinSelectDlg()->IsActive() == FALSE)
				GAMEIN->GetSkinSelectDlg()->SetActive(TRUE);
			return;
		}
		else if (pInfo->ItemKind == eSHOP_ITEM_COSTUME_SKIN)
		{
			if (GAMEIN->GetCostumeSkinSelectDlg()->IsActive() == FALSE)
				GAMEIN->GetCostumeSkinSelectDlg()->SetActive(TRUE);
			return;
		}
		else if (pInfo->ItemIdx == eIncantation_StatusFastAdd)
		{
			GAMEIN->GetAdPointDlg()->Show(HERO->GetHeroLevelUpPoint());
			return;
		}
		else if (pInfo->ItemIdx == eIncantation_StatusPointReset_71 ||
			pInfo->ItemIdx == eIncantation_StatusPointReset_1_50 ||
			pInfo->ItemIdx == eIncantation_StatusPointReset_1_99)
		{
			if (HERO->GetShopItemStats()->UseStatePoint > 0)
			{
				CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(1688));
				return;
			}
			int nMaxLevel = HERO->GetMaxLevel();
			if (nMaxLevel < pInfo->RangeType || nMaxLevel > pInfo->EquipKind)
			{
				CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(1082));
				return;
			}
			ITEMMGR->SetDisableDialog(TRUE, eItemTable_Inventory);
			ITEMMGR->SetDisableDialog(TRUE, eItemTable_Pyoguk);
			ITEMMGR->SetDisableDialog(TRUE, eItemTable_GuildWarehouse);
			ITEMMGR->SetDisableDialog(TRUE, eItemTable_Shop);
			m_pSelectedShopItem = pItem;
			WINDOWMGR->MsgBox(MBI_STATUS_POINT_RESET, MBT_YESNO, CHATMGR->GetChatMsg(1683));
			return;
		}
		else if (pInfo->ItemIdx == eIncantation_SkillPoint_Reset)
		{
			if (HERO->GetState() != eObjectState_None && HERO->GetState() != eObjectState_Immortal)
			{
				CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(734));
				return;
			}
			GAMEIN->GetSkillPointResetDlg()->SetShopItemPos(pItem->GetPosition());
			GAMEIN->GetSkillPointResetDlg()->SetActive(TRUE);
			OBJECTSTATEMGR->StartObjectState(HERO, eObjectState_Deal);
		}
		else
		{
			if (pInfo->ItemKind == eSHOP_ITEM_CHARM && pInfo->EquipKind)
			{
				if (HERO->GetMaxLevel() < pInfo->EquipKind)
				{
					CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(931));
					return;
				}
			}
			else if (pInfo->ItemKind == eSHOP_ITEM_INCANTATION && pInfo->LimitJob)
			{
				if (pInfo->LimitGender == 0 && HERO->GetLevel() > 50)
				{
					CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(1082));
					return;
				}
				else if (pInfo->LimitGender == 1 && (HERO->GetLevel() <51 || HERO->GetLevel() > 70))
				{
					CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(1082));
					return;
				}
				else if (pInfo->LimitGender == 2 && (HERO->GetLevel() <71 || HERO->GetLevel() > 90))
				{
					CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(1082));
					return;
				}
			}
			if (pInfo->ItemKind == eSHOP_ITEM_PET)
			{
				if (2 == HERO->GetBattleTeam())
				{
					CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(1279));
					return;
				}
				if (HERO->GetSingleSpecialState(eSingleSpecialState_Hide))
				{
					CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(1329));
					return;
				}
				if (FALSE == PETMGR->CheckResummonAvailable())
				{
					CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(1254), DWORD(PETMGR->GetPetResummonRestTime() / 1000));
					return;
				}
				if (!IsMultiPet)
				{
					if (PETMGR->GetCurSummonPet() && FALSE == PETMGR->IsCurPetSummonItem(pItem->GetDBIdx())
						&& !PETMGR->CheckCurSummonPetKindIs(ePK_EventPet))
					{
						CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(1255));
						return;
					}
				}
				if (FALSE == PETMGR->CheckPetAlive(pItem->GetDBIdx()))
				{
					CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(1249));
					return;
				}
				if (pInfo->EquipKind > HERO->GetMaxLevel())
				{
					return;
				}
				MOVEMGR->HeroMoveStop();
				m_PetPos = pItem->GetPosition();
			}
			MSG_SHOPITEM_USE_SYN msg;
			msg.Category = MP_ITEM;
			msg.Protocol = MP_ITEM_SHOPITEM_USE_SYN;
			msg.dwObjectID = HEROID;
			msg.UseBaseInfo.TargetItemPos = pItem->GetPosition();
			msg.UseBaseInfo.TargetItemIdx = pItem->GetItemIdx();
			msg.UseBaseInfo.ShopItemPos = pItem->GetPosition();
			msg.UseBaseInfo.ShopItemIdx = pItem->GetItemIdx();
			NETWORK->Send(&msg, sizeof(msg));
		}
	}
	else if (bits & eTITAN_EQUIPITEM)
	{
		{
			switch (GetTableIdxPosition(pItem->GetPosition()))
			{
			case eItemTable_Inventory:
			{
										 ITEM_INFO * pInfo = ITEMMGR->GetItemInfo(pItem->GetItemIdx());
										 if (!pInfo) return;
										 DWORD EquipToPos = pInfo->ItemKind - eTITAN_EQUIPITEM_HELMET;
										 CItem * pToItem = GAMEIN->GetTitanInventoryDlg()->GetItemForPos((POSTYPE)(EquipToPos + TP_TITANWEAR_START));
										 FakeGeneralItemMove((POSTYPE)(EquipToPos + TP_TITANWEAR_START), pItem, pToItem);
			}
				break;
			case eItemTable_Titan:
			{
									 POSTYPE emptyPos;
									 if (GetBlankPositionRestrictRef(emptyPos))
									 {
										 CItem * pToItem = GetItemForPos(emptyPos);
										 FakeGeneralItemMove(emptyPos, pItem, pToItem);
									 }
			}
				break;
			}
		}
	}
	else if (bits & eTITAN_ITEM)
	{
		if (eTITAN_ITEM_PAPER == bits)
		{
			if (HERO->GetState() != eObjectState_None)
			{
				CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(1548));
				return;
			}
			if (FALSE == TITANMGR->IsRegistedTitan(pItem->GetDBIdx()))
			{
				CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(1510));
				return;
			}
			if (TITANMGR->GetCurRidingTitan() && TITANMGR->GetCallItemDBIdx() != pItem->GetDBIdx())
			{
				CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(1549));
				return;
			}
			if (NULL == TITANMGR->GetCurRidingTitan())
			{
				if (FALSE == TITANMGR->CheckRecallAvailable())
				{
					stTIME stCurTime = TITANMGR->GetTitanResummonTime();
					CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(1545), GetCurTimeToString(&stCurTime));
					return;
				}
				if (FALSE == TITANMGR->CheckMasterLifeAmountForRiding())
				{
					CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(403));
					return;
				}
			}
			if (MUSSANGMGR->IsMussangMode() == TRUE)
			{
				CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(1668));
				return;
			}
			MOVEMGR->HeroMoveStop();
			MSG_ITEM_USE_SYN msg;
			msg.Category = MP_ITEM;
			msg.Protocol = MP_ITEM_USE_SYN;
			msg.dwObjectID = HEROID;
			msg.wItemIdx = pItem->GetItemIdx();
			msg.TargetPos = pItem->GetPosition();
			msg.dwItemDBIdx = pItem->GetDBIdx();
			NETWORK->Send(&msg, sizeof(msg));
			ITEMMGR->m_nItemUseCount++;
		}
	}
	else
	{
		ASSERT(0);
	}
}
void CInventoryExDialog::SetActive(BOOL val)
{
#ifndef _MPAGES_
	m_pItemShopInvenPlus->SetDisableIcon(0, 40);
#endif
	if (!m_bDisable)
	{
		if (val == FALSE)
		{
			CDealDialog * dDlg = GAMEIN->GetDealDialog();
			if (dDlg)
			if (dDlg->IsShow())
				dDlg->SetActive(FALSE);
			CPyogukDialog* pPyogukDlg = GAMEIN->GetPyogukDialog();
			if (pPyogukDlg->IsActive())
				pPyogukDlg->SetActive(FALSE);
			if (GAMEIN->GetGuildWarehouseDlg()->IsActive())
				GAMEIN->GetGuildWarehouseDlg()->SetActive(FALSE);
			if (GAMEIN->GetCharacterRBTNDlg()->IsActive())
				GAMEIN->GetCharacterRBTNDlg()->SetActive(FALSE);
			if (GAMEIN->GetMoneyPacketDialog()->IsActive())
				GAMEIN->GetMoneyPacketDialog()->SetActive(FALSE);
			if (GAMEIN->GetChangeLookDialog()->IsActive())
			{
				GAMEIN->GetChangeLookDialog()->SetActive(FALSE);
			}
		}
		else
		{
		//	GAMEIN->GetChangeLookDialog()->setData();
		}
		cTabDialog::SetActive(val);
		//
		if (GAMEIN->GetInventoryDialog()->GetItemCount() == 0)
		{
			m_pClearInvBtn[ITEMINV]->SetActive(false);
		}
		else
		{
			m_pClearInvBtn[ITEMINV]->SetActive(true);
		}
		if (m_pChangeLookBtn)
		{
			GAMEIN->GetChangeLookDialog()->SetActive(m_pChangeLookBtn->IsPushed() && IsActive());
			/*if (m_pChangeLookBtn->IsPushed() && IsActive())
			{
				GAMEIN->GetChangeLookDialog() ->setData();
			}*/
		}
		if (m_bShopMInven[0])
		{
			OnShopItemBtn();
			m_bShopMInven[1] = false;
		}
		else if (m_bShopMInven[1])
		{
			OnShopItemBtnEx();
			m_bShopMInven[0] = false;
		}
		else
		{
			m_pMPageBtn[0]->SetPush(FALSE);
			m_pItemShopInven->SetActive(FALSE);
			m_pItemShopInvenPlus->SetActive(FALSE);
			m_pTidyMallBtn->SetActive(FALSE);
			m_pClearInvBtn[MALLINV]->SetActive(false);
			m_bShopMInven[0] = FALSE;
		}
	}
}
void CInventoryExDialog::OnShopItemBtn()
{
	VECTOR3 WH;
	WH.x = 251.f;
	WH.y = 380.f;
	for (int i = 0; i<m_bTabNum; i++)
	{
		SetWH(WH.x, WH.y);
		m_ppWindowTabSheet[i]->SetActive(TRUE);
	}
	if (m_pMPageBtn[0]->IsPushed() == TRUE)
	{
		m_pMPageBtn[0]->SetPush(true);
		m_pItemShopInven->SetActive(TRUE);
		m_pItemShopInvenPlus->SetActive(false);
		m_pMPageBtn[1]->SetPush(false);
		m_pTidyMallBtn->SetActive(TRUE);
		CheckItemForTidyBtn();
		m_bShopMInven[0] = TRUE;
		m_bShopMInven[1] = false;
		SetWH(WH.x + 230.5f, WH.y + 9.5f);
	}
	else
	{
		m_pItemShopInvenPlus->SetActive(FALSE);
		m_pItemShopInven->SetActive(FALSE);
		m_pTidyMallBtn->SetActive(FALSE);
		m_pClearInvBtn[MALLINV]->SetActive(false);
		m_bShopMInven[0] = FALSE;
		m_bShopMInven[1] = false;
		SetWH(WH.x + .5f, WH.y + 9.5f);
	}
}
void CInventoryExDialog::OffShopItemBtn()
{
	m_bShopMInven[0] = FALSE;
	m_pMPageBtn[0]->SetPush(FALSE);
	m_pMPageBtn[1]->SetPush(false);
	m_pItemShopInven->SetActive(FALSE);
	m_pItemShopInvenPlus->SetActive(FALSE);
	m_pTidyMallBtn->SetActive(FALSE);
	m_pClearInvBtn[MALLINV]->SetActive(false);
}
void CInventoryExDialog::OnShopItemBtnEx()
{
	VECTOR3 WH;
	WH.x = 251.f;
	WH.y = 380.f;
	for (int i = 0; i<m_bTabNum; i++)
	{
		SetWH(WH.x, WH.y);
		m_ppWindowTabSheet[i]->SetActive(TRUE);
	}
	if (m_pMPageBtn[1]->IsPushed() == TRUE)
	{
		m_pMPageBtn[1]->SetPush(true);
		m_pMPageBtn[0]->SetPush(FALSE);
		m_pItemShopInven->SetActive(false);
		m_pItemShopInvenPlus->SetActive(TRUE);
		m_pTidyMallBtn->SetActive(TRUE);
		CheckItemForTidyBtn();
		m_bShopMInven[1] = TRUE;
		m_bShopMInven[0] = false;
		SetWH(WH.x + 230.5f, WH.y + 9.5f);
	}
	else
	{
		m_pItemShopInvenPlus->SetActive(FALSE);
		m_pItemShopInven->SetActive(FALSE);
		m_pTidyMallBtn->SetActive(FALSE);
		m_pClearInvBtn[MALLINV]->SetActive(false);
		m_bShopMInven[1] = FALSE;
		m_bShopMInven[0] = false;
		SetWH(WH.x + .5f, WH.y + 9.5f);
	}
}
void CInventoryExDialog::OffShopItemBtnEx()
{
	m_bShopMInven[1] = FALSE;
	m_bShopMInven[0] = FALSE;
	m_pMPageBtn[0]->SetPush(FALSE);
	m_pMPageBtn[1]->SetPush(false);
	m_pItemShopInven->SetActive(FALSE);
	m_pItemShopInvenPlus->SetActive(FALSE);
	m_pTidyMallBtn->SetActive(FALSE);
	m_pClearInvBtn[MALLINV]->SetActive(false);
}
void CInventoryExDialog::OpenMall(BYTE bPage)
{
	VECTOR3 WH;
	WH.x = 251.f;
	WH.y = 380.f;
	for (int i = 0; i<m_bTabNum; i++)
	{
		SetWH(WH.x, WH.y);
		m_ppWindowTabSheet[i]->SetActive(TRUE);
	}
	switch (bPage)
	{
	case 1:
	{
			  m_pMPageBtn[1]->SetPush(false);
			  m_pMPageBtn[0]->SetPush(true);
			  OnShopItemBtn();
	}
		break;
	case 2:
	{
			  m_pMPageBtn[1]->SetPush(true);
			  m_pMPageBtn[0]->SetPush(false);
			  OnShopItemBtn();
	}
		break;
	}
}
void CInventoryExDialog::UnselectAllItem(CItem * pItem3)
{
	m_StoredLastGridPosition = pItem3->GetPosition();
	for (int i = 0; i <= 79; i++)
	{
		pItem3 = GAMEIN->GetInventoryDialog()->GetItemForPos(i);
		if (pItem3)
		{
			pItem3->SetLock(false);
			m_SelectedItemRbtn = 0;
		}
	}
}
void CInventoryExDialog::SetUnlockAll()
{
	for (int i = 0; i <= 79; i++)
	{
		CItem * pItem = GAMEIN->GetInventoryDialog()->GetItemForPos(i);
		if (pItem)
		{
			pItem->SetLock(false);
			m_SelectedItemRbtn = 0;
		}
	}
}
#include "StreetStall.h"
#include "StreetBuyStall.h"
void CInventoryExDialog::OnActionEvnet(LONG lId, void * p, DWORD we)
{
	if (we == WE_LBTNDBLCLICK)
	{
		CItem * pItem = NULL;
		switch (lId)
		{
		case IN_WEAREDDLG:		pItem = (CItem*)m_pWearedDlg->GetIconForIdx((WORD)m_pWearedDlg->GetCurSelCellPos()); break;
		case IN_SHOPITEMGRID1:	pItem = (CItem*)m_pItemShopInven->GetIconForIdx((WORD)m_pItemShopInven->GetCurSelCellPos()); break;
		case IN_SHOPITEMGRID2:	pItem = (CItem*)m_pItemShopInvenPlus->GetIconForIdx((WORD)m_pItemShopInvenPlus->GetCurSelCellPos()); break;
		default:				cIconGridDialog * gridDlg = (cIconGridDialog *)GetTabSheet(GetCurTabNum()); if (!gridDlg) return;
			pItem = (CItem*)gridDlg->GetIconForIdx((WORD)gridDlg->GetCurSelCellPos()); break;
		}
		if (!pItem) return;
		if (pItem->GetItemParam() & ITEM_PARAM_SEAL)
		{
			if (pItem->IsLocked()) return;
			WINDOWMGR->MsgBox(MBI_SHOPITEM_LOCK, MBT_YESNO, CHATMGR->GetChatMsg(774));
			return;
		}
		UseItem(pItem);
	}
	else if (we == WE_LBTNCLICK)
	{
		
		if (lId == IN_SHOPITEMGRID1)
		{
			m_pItemShopInvenPlus->SetCurSelCellPos(-1);
		}
		else if (lId == IN_SHOPITEMGRID2)
		{
			m_pItemShopInven->SetCurSelCellPos(-1);
		}
		if (lId != IN_WEAREDDLG && lId != IN_SHOPITEMGRID1 && lId != IN_SHOPITEMGRID2)
		{
			if (GAMEIN->GetReinforceDialog()->IsActive() || GAMEIN->GetMixDialog()->IsActive() || GAMEIN->GetItemStoneXqDlg()->IsActive())
				return;
		
			CItem * pItem3 = NULL;
			cIconGridDialog * gridDlg = (cIconGridDialog *)GetTabSheet(GetCurTabNum());
			if (!gridDlg) return;
			pItem3 = (CItem*)gridDlg->GetIconForIdx((WORD)gridDlg->GetCurSelCellPos());
			if (!pItem3) return;


			//KEEP IN VIEW BELOW!!!!!! 1415 07062023 BY JACK
			if (GAMEIN->GetStreetStallDialog()->IsActive() == TRUE
				|| GAMEIN->GetStreetBuyStallDialog()->IsActive() == TRUE
				|| GAMEIN->GetStallKindSelectDialog()->IsActive() == TRUE)
			{
				if (pItem3->IsLocked())
				{
					//WINDOWMGR->MsgBox(MBI_SHOPITEM_LOCK, MBT_YESNO, "test");
					CHATMGR->AddMsg(CTC_SYSMSG, "cannot move item when locked");
					return;
				}
			}
			else
			{
				if (MOUSE->ControlPressed())
				{
					pItem3->SetLock(!pItem3->IsLocked());
					m_SelectedItemRbtn = 1;
					return;
				}
				if (MOUSE->ShiftPressed())
				{
					int m_StoredNewGridPosition = pItem3->GetPosition();
					if (m_StoredLastGridPosition>m_StoredNewGridPosition)
					{
						for (int i = m_StoredLastGridPosition; i >= m_StoredNewGridPosition; --i)
						{
							pItem3 = GAMEIN->GetInventoryDialog()->GetItemForPos(i);
							if (pItem3)
							{
								pItem3->SetLock(true);
							}
						}
					}
					if (m_StoredLastGridPosition<m_StoredNewGridPosition)
					{
						for (int i = m_StoredLastGridPosition; i <= m_StoredNewGridPosition; i++)
						{
							pItem3 = GAMEIN->GetInventoryDialog()->GetItemForPos(i);
							if (pItem3)
							{
								pItem3->SetLock(true);
							}
						}
					}
					m_SelectedItemRbtn = 2;
					return;
				}
				else
				{

					UnselectAllItem(pItem3);
				}
			}
			//////////////////////////////////////////////////////////////
			
			
		}
		if (CURSOR->GetCursor() == eCURSOR_TITANREPAIR && lId != IN_WEAREDDLG && lId != IN_SHOPITEMGRID1 && lId != IN_SHOPITEMGRID2)
		{
			CHATMGR->AddMsg(CTC_SYSMSG, "this file version doesnt support titan.");
		}
		switch (m_iInventoryState)
		{
		case eITEMUPGRADE_STATE: break;
		case eITEMDEAL_STATE:
		{
								if (!GAMEIN->GetDealDialog()->IsShow()) return;
								CItem * pItemTT = GetCurSelectedItem(INV);
								if (!pItemTT)
								{
									GAMEIN->GetDealDialog()->SetMoney(0, 2);
									return;
								}
								int itemIDX = pItemTT->GetItemIdx();
								ITEM_INFO * pItem = ITEMMGR->GetItemInfo(itemIDX);
								ASSERT(pItem);
								DWORD SellPrice = SWPROFIT->CalTexRateForSell(pItem->SellPrice);
								SellPrice = FORTWARMGR->CalTexRateForSell(pItem->SellPrice);
								GAMEIN->GetDealDialog()->SetMoney(SellPrice, 0);
								break;
		}
		}
	}




	else if (we == WE_RBTNCLICK)
	{
		CItem * pItem = NULL;
		BYTE dwSHOP;
		switch (lId)
		{
			case IN_WEAREDDLG:
			{
				dwSHOP = WEAR;
				if (MOUSE->RButtonUp())
					pItem = (CItem*)m_pWearedDlg->GetIconForIdx((WORD)m_pWearedDlg->GetCurSelCellPos());
				break;
			}
			case IN_SHOPITEMGRID1:
			{
				dwSHOP = SHOPIVN;
				pItem = (CItem*)m_pItemShopInven->GetIconForIdx((WORD)m_pItemShopInven->GetCurSelCellPos());
				break;
			}
			case IN_SHOPITEMGRID2:
			{
				dwSHOP = SHOPIVNPLUS;
				pItem = (CItem*)m_pItemShopInvenPlus->GetIconForIdx((WORD)m_pItemShopInvenPlus->GetCurSelCellPos());
				break;
			}
			default:
			{
				dwSHOP = INV;
				cIconGridDialog * gridDlg = (cIconGridDialog *)GetTabSheet(GetCurTabNum());
				if (!gridDlg) return;
				pItem = (CItem*)gridDlg->GetIconForIdx((WORD)gridDlg->GetCurSelCellPos());
				break;
			}
		}
		if (!pItem) return;

		//if (pItem->IsLocked() == TRUE)
		//{
		//	//CHATMGR->AddMsg(CTC_SYSMSG, "this function has been blocked due to item locking!");
		//	//return;
		//}
		/*if (GAMEIN->GetStreetStallDialog()->IsActive() == TRUE
			|| GAMEIN->GetStreetBuyStallDialog()->IsActive() == TRUE
			|| GAMEIN->GetStallKindSelectDialog()->IsActive() == TRUE)
		{
			if (pItem->IsLocked())
			{
				//WINDOWMGR->MsgBox(MBI_SHOPITEM_LOCK, MBT_YESNO, "test");
				CHATMGR->AddMsg(CTC_SYSMSG, "cannot move item when locked");
				return;
			}
		}*/


		CDealDialog * pDealDlg = GAMEIN->GetDealDialog();
		if (pDealDlg)
		{
			if (pDealDlg->IsActive() && !pItem->IsLocked())
			{
				pDealDlg->FakeMoveIcon(0, 0, pItem);
				return;
			}
		}

		



		switch (dwSHOP)
		{
			case WEAR: break;
			case SHOPIVN:
			case SHOPIVNPLUS:
			{
				CItemShopDialog* pItemShopDlg = GAMEIN->GetItemShopDialog();
				if (pItemShopDlg)
				{
					if (pItemShopDlg->IsActive())
					{
						pItemShopDlg->MoveItemToItemShop(pItem);
						return;
					}
				}
				if (pItem->GetItemParam() & ITEM_PARAM_SEAL)
				{
					if (pItem->IsLocked()) return;
					WINDOWMGR->MsgBox(MBI_SHOPITEM_LOCK, MBT_YESNO, CHATMGR->GetChatMsg(774));
					return;
				}
				break;
			}
			case INV:
			{
				CRareCreateDialog* pRareCreate = GAMEIN->GetRareCreateDlg();
				if (pRareCreate&&pRareCreate->IsActive())
				{
					if (!pItem->IsLocked())
					{
						pRareCreate->FakeMoveIcon(0, 0, pItem);
						OBJECTSTATEMGR->StartObjectState(HERO, eObjectState_Deal);
						return;
					}
					return;
				}
				CPetInventoryDlg* pPetInven = GAMEIN->GetPetInventoryDialog();
				if (pPetInven&&pPetInven->IsActive())
				{
					if (pItem->IsLocked()) return;
					pPetInven->MoveItemToPetInventory(pItem);
					return;
				}
				CGuildWarehouseDialog* pGuildPyogukDlg = GAMEIN->GetGuildWarehouseDlg();
				if (pGuildPyogukDlg&&pGuildPyogukDlg->IsActive())
				{
					if (pItem->IsLocked()) return;
					pGuildPyogukDlg->MoveItemToGuildPyoguk(pItem);
					return;
				}
				CPyogukDialog* pPyogukDlg = GAMEIN->GetPyogukDialog();
				if (pPyogukDlg&&pPyogukDlg->IsActive())
				{
					if (pItem->IsLocked()) return;
					pPyogukDlg->MoveItemToPyoguk(pItem);
					return;
				}
				break;
			}
		}
		switch (pItem->IsLocked())
		{
			case TRUE : CHARRBTN->SetItemNameExt(dwSHOP); break;
			case FALSE: CHARRBTN->SetItemName(pItem->GetItemIdx(), dwSHOP); break;
		}
	}
}
CItem * CInventoryExDialog::GetCurSelectedItem(BYTE type)
{
	switch (type)
	{
	case INV:
		{
			cIconGridDialog * gridDlg = (cIconGridDialog *)GetTabSheet(GetCurTabNum());
			if (gridDlg->GetCurSelCellPos() == -1)
				return NULL;
			return (CItem *)gridDlg->GetIconForIdx((WORD)gridDlg->GetCurSelCellPos());
		}
		break;
	case WEAR:
	{
				 if (m_pWearedDlg->GetCurSelCellPos() == -1)
					 return NULL;
				 return (CItem *)m_pWearedDlg->GetIconForIdx((WORD)m_pWearedDlg->GetCurSelCellPos());
	}
		break;
	case SHOPIVN:
	{
					return m_pItemShopInven->GetItemForPos((POSTYPE)(m_pItemShopInven->GetCurSelCellPos() + TP_SHOPINVEN_START));
	}
		break;
	case SHOPIVNPLUS:
	{
						return m_pItemShopInvenPlus->GetItemForPos((POSTYPE)(m_pItemShopInvenPlus->GetCurSelCellPos() + TP_SHOPINVEN_START + TABCELL_SHOPINVEN_NUM));
	}
		break;
	default:
	{
			   ASSERT(0);
	}
	}
	return NULL;
}
BOOL CInventoryExDialog::GetBlankPositionRestrictRef(WORD & absPos)
{
	for (int i = 0; i < GetTabNum(); ++i)
	{
		cIconGridDialog * gridDlg = (cIconGridDialog *)GetTabSheet(i);
		for (int j = 0; j < gridDlg->GetCellNum(); ++j)
		{
			if (gridDlg->IsAddable(j))
			{
				absPos = TP_INVENTORY_START + i*TABCELL_INVENTORY_NUM + j;
				return TRUE;
			}
		}
	}
	return FALSE;
}
int CInventoryExDialog::GetBlankNum()
{
	int nBlankNum = 0;
	BYTE TabNum = GetTabNum();
	for (BYTE i = 0; i < TabNum; ++i)
	{
		cIconGridDialog* gridDlg = (cIconGridDialog*)GetTabSheet(i);
		for (WORD j = 0; j < gridDlg->GetCellNum(); ++j)
		{
			if (gridDlg->IsAddable(j))
				++nBlankNum;
		}
	}
	return nBlankNum;
}
BOOL CInventoryExDialog::FakeMoveIcon(LONG x, LONG y, cIcon * icon)
{
	ASSERT(icon);
	if (m_bDisable)	return FALSE;
	if (PKMGR->IsPKLooted())	return FALSE;
	if (icon->GetType() == WT_STALLITEM || icon->GetType() == WT_EXCHANGEITEM) return FALSE;
	if (icon->GetType() == WT_ITEM)
	{
		return FakeMoveItem(x, y, (CItem *)icon);
	}
	else if (icon->GetType() == WT_DEALITEM || icon->GetType() == WT_SHOPITEM || icon->GetType() == WT_SHOPGOLDITEM)
	{
		if (!GAMEIN->GetSafeNumberIcon()->GetSafeStatic())
		{
			CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(2599));
			return FALSE;
		}
		FakeBuyItem(x, y, (CDealItem *)icon);
		AUDIOMGR->Play(58, HERO);
	}
	return FALSE;
}
void CInventoryExDialog::FakeBuyItem(LONG mouseX, LONG mouseY, CDealItem * pFromItem)
{
	if (pFromItem)
	{
		GAMEIN->GetDealDialog()->FakeBuyItem((LONG)pFromItem->GetAbsX(), (LONG)pFromItem->GetAbsY(), pFromItem);
	}
}
void CInventoryExDialog::MoveItem(POSTYPE FromPos, POSTYPE ToPos)
{
	CItem * FromItem = NULL;
	CItem * ToItem = NULL;
	DeleteItem(FromPos, &FromItem);
	DeleteItem(ToPos, &ToItem);
	if (FromItem)
	{
		FromItem->SetPosition(ToPos);
		if (!AddItem(FromItem))
			assert(0);
	}
	if (ToItem)
	{
		ToItem->SetPosition(FromPos);
		if (!AddItem(ToItem))
			assert(0);
	}
}
BOOL CInventoryExDialog::FakeGeneralItemMove(POSTYPE ToPos, CItem * pFromItem, CItem * pToItem)
{
	if (CanBeMoved(pFromItem, ToPos) == FALSE)
		return FALSE;
	BYTE FromIdx = ITEMMGR->GetTableIdxForAbsPos(pFromItem->GetPosition());
	if (FromIdx >= eItemTable_Max) return FALSE;
	if (pToItem && CanBeMoved(pToItem, pFromItem->GetPosition()) == FALSE)
	{
		return FALSE;
	}
	if (pToItem && (FromIdx == eItemTable_MunpaWarehouse))
	{
		if (FromIdx == eItemTable_GuildWarehouse)
			CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(53));
		return FALSE;
	}
	if (pToItem)
	{
		if (FromIdx == eItemTable_Pyoguk && pToItem->GetDBIdx() == TITANMGR->GetRegistedTitanItemDBIdx())
		{
			CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(1578));
			return FALSE;
		}
	}
	BYTE ToIdx = ITEMMGR->GetTableIdxForAbsPos(ToPos);
	if (ToIdx >= eItemTable_Max) return FALSE;
	MSG_ITEM_MOVE_SYN msg;
	msg.Category = MP_ITEM;
	msg.Protocol = MP_ITEM_MOVE_SYN;
	msg.dwObjectID = HEROID;
	msg.FromPos = pFromItem->GetPosition();
	msg.wFromItemIdx = pFromItem->GetItemIdx();
	msg.ToPos = ToPos;
	msg.wToItemIdx = (pToItem ? pToItem->GetItemIdx() : 0);
	if (msg.FromPos == msg.ToPos)
		return FALSE;
	ITEMMGR->SetDisableDialog(TRUE, eItemTable_Inventory);
	ITEMMGR->SetDisableDialog(TRUE, eItemTable_Pyoguk);
	ITEMMGR->SetDisableDialog(TRUE, eItemTable_GuildWarehouse);
	ITEMMGR->SetDisableDialog(TRUE, eItemTable_PetInven);
	NETWORK->Send(&msg, sizeof(msg));
	return TRUE;
}
BOOL CInventoryExDialog::FakeItemDivide(POSTYPE ToPos, CItem * pFromItem, CItem * pToItem)
{
	if (!CanBeMoved(pFromItem, ToPos))
		return FALSE;
	if (pFromItem->GetPosition() == ToPos)
		return FALSE;
	if (pFromItem->IsLocked())
		return FALSE;
	if (pFromItem->GetItemBaseInfo()->ItemStatic == ITEM_STATIC_PD || pFromItem->GetItemBaseInfo()->ItemStatic == ITEM_STATIC_NEWBIE)
	{
		CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(2266));
		return FALSE;
	}
	BYTE FromIdx = ITEMMGR->GetTableIdxForAbsPos(pFromItem->GetPosition());
	if (FromIdx >= eItemTable_Max) return FALSE;
	BYTE ToIdx = ITEMMGR->GetTableIdxForAbsPos(ToPos);
	if (ToIdx >= eItemTable_Max) return FALSE;
	if (pFromItem->GetDurability() == 1)
	{
		return FakeGeneralItemMove(ToPos, pFromItem, pToItem);
	}
	m_divideMsg.Category = MP_ITEM;
	m_divideMsg.Protocol = MP_ITEM_DIVIDE_SYN;
	m_divideMsg.dwObjectID = HEROID;
	m_divideMsg.wItemIdx = pFromItem->GetItemIdx();
	m_divideMsg.FromPos = pFromItem->GetPosition();
	m_divideMsg.FromDur = pFromItem->GetDurability();
	m_divideMsg.ToPos = ToPos;
	m_divideMsg.ToDur = 0;
	m_divideMsg.dwItemDBIdx = pFromItem->GetDBIdx();
	ITEMMGR->SetDisableDialog(TRUE, eItemTable_Inventory);
	ITEMMGR->SetDisableDialog(TRUE, eItemTable_Pyoguk);
	ITEMMGR->SetDisableDialog(TRUE, eItemTable_GuildWarehouse);
	ITEMMGR->SetDisableDialog(TRUE, eItemTable_ShopInven);
	ITEMMGR->SetDisableDialog(TRUE, eItemTable_PetInven);
	cDivideBox * pDivideBox = WINDOWMGR->DivideBox(DBOX_DIVIDE_INV, (LONG)pFromItem->GetAbsX(), (LONG)pFromItem->GetAbsY(), OnFakeItemDivideOk, OnFakeItemDivideCancel, this, pFromItem, CHATMGR->GetChatMsg(185));
	pDivideBox->SetMaxValue(MAX_YOUNGYAKITEM_DUPNUM);
	pDivideBox->SetValue(1);
	return FALSE;
}
void CInventoryExDialog::OnFakeItemDivideCancel(LONG iId, void* p, DWORD param1, void * vData1, void * vData2)
{
	ITEMMGR->SetDisableDialog(FALSE, eItemTable_Inventory);
	ITEMMGR->SetDisableDialog(FALSE, eItemTable_Pyoguk);
	ITEMMGR->SetDisableDialog(FALSE, eItemTable_GuildWarehouse);
	ITEMMGR->SetDisableDialog(FALSE, eItemTable_PetInven);
}
void CInventoryExDialog::OnFakeItemDivideOk(LONG iId, void* p, DWORD param1, void * vData1, void * vData2)
{
	CInventoryExDialog * tDlg = (CInventoryExDialog *)vData1;
	if (param1 == 0)
	{
		OnFakeItemDivideCancel(0, NULL, 0, NULL, NULL);
		return;
	}
	WORD TargetTableIdx = ITEMMGR->GetTableIdxForAbsPos(tDlg->m_divideMsg.FromPos);
	CItem * pTargetItem = ITEMMGR->GetItemofTable(TargetTableIdx, tDlg->m_divideMsg.FromPos);
	if (!pTargetItem)
	{
		OnFakeItemDivideCancel(0, NULL, 0, NULL, NULL);
		return;
	}
	if (pTargetItem->IsLocked())
	{
		OnFakeItemDivideCancel(0, NULL, 0, NULL, NULL);
		return;
	}
	if (tDlg->m_divideMsg.wItemIdx != pTargetItem->GetItemIdx())
	{
		OnFakeItemDivideCancel(0, NULL, 0, NULL, NULL);
		return;
	}
	if (tDlg->m_divideMsg.FromDur != pTargetItem->GetDurability())
	{
		OnFakeItemDivideCancel(0, NULL, 0, NULL, NULL);
		return;
	}
	CItem * pToItem = ITEMMGR->GetItemofTable(ITEMMGR->GetTableIdxForAbsPos(tDlg->m_divideMsg.ToPos), tDlg->m_divideMsg.ToPos);
	if (pToItem)
	{
		OnFakeItemDivideCancel(0, NULL, 0, NULL, NULL);
		return;
	}
	if (tDlg->m_divideMsg.FromDur > param1)
	{
		tDlg->m_divideMsg.ToDur = param1;
		tDlg->m_divideMsg.FromDur = tDlg->m_divideMsg.FromDur - param1;
		NETWORK->Send(&tDlg->m_divideMsg, sizeof(tDlg->m_divideMsg));
	}
	else
	{
		tDlg->FakeGeneralItemMove(tDlg->m_divideMsg.ToPos, (CItem *)vData2, NULL);
		ITEMMGR->SetDisableDialog(FALSE, eItemTable_Inventory);
		ITEMMGR->SetDisableDialog(FALSE, eItemTable_Pyoguk);
		ITEMMGR->SetDisableDialog(FALSE, eItemTable_GuildWarehouse);
		ITEMMGR->SetDisableDialog(FALSE, eItemTable_PetInven);
	}
}
BOOL CInventoryExDialog::FakeItemCombine(POSTYPE ToPos, CItem * pFromItem, CItem * pToItem)
{
	if (pFromItem->GetPosition() == pToItem->GetPosition())
		return FALSE;
	if (pFromItem->GetItemIdx() != pToItem->GetItemIdx())
		return FALSE;
	if (CanBeMoved(pFromItem, ToPos) == FALSE)
		return FALSE;
	if (pToItem && CanBeMoved(pToItem, pFromItem->GetPosition()) == FALSE)
		return FALSE;
	if (pFromItem->IsLocked() || pToItem->IsLocked())
		return FALSE;
	if (pFromItem->GetStatic() != pToItem->GetStatic())
	{
		CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(2266));
		return FALSE;
	}
	BYTE FromIdx = ITEMMGR->GetTableIdxForAbsPos(pFromItem->GetPosition());
	if (FromIdx >= eItemTable_Max) return FALSE;
	BYTE ToIdx = ITEMMGR->GetTableIdxForAbsPos(ToPos);
	if (ToIdx >= eItemTable_Max) return FALSE;
	MSG_ITEM_COMBINE_SYN msg;
	msg.Category = MP_ITEM;
	msg.Protocol = MP_ITEM_COMBINE_SYN;
	msg.dwObjectID = HEROID;
	msg.FromPos = pFromItem->GetPosition();
	msg.ToPos = pToItem->GetPosition();
	msg.wItemIdx = pFromItem->GetItemIdx();
	msg.FromDur = pFromItem->GetDurability();
	msg.ToDur = pToItem->GetDurability();
	ITEMMGR->SetDisableDialog(TRUE, eItemTable_Inventory);
	ITEMMGR->SetDisableDialog(TRUE, eItemTable_Pyoguk);
	ITEMMGR->SetDisableDialog(TRUE, eItemTable_GuildWarehouse);
	NETWORK->Send(&msg, sizeof(msg));
	return TRUE;
}
void CInventoryExDialog::Linking()
{
	if (!m_fSpread)
	{
		cImage Image;
		SCRIPTMGR->GetImage(63, &Image, PFT_HARDPATH);
		m_pMPageBtn[0] = (cPushupButton*)GetWindowForID(IN_SHOPITEMBTN1);
		m_pMPageBtn[0]->SetActive(false);
		m_pMPageBtn[0]->SetToolTip(CHATMGR->GetChatMsg(731), TTTC_DEFAULT, &Image, TTCLR_DEFAULT);
		m_pItemShopInven = (CItemShopInven*)GetWindowForID(IN_SHOPITEMGRID1);
		m_pItemShopInven->SetActive(false);
		m_pMPageBtn[1] = (cPushupButton*)GetWindowForID(IN_SHOPITEMBTN2);
		m_pMPageBtn[1]->SetActive(false);
		m_pMPageBtn[1]->SetToolTip(CHATMGR->GetChatMsg(731), TTTC_DEFAULT, &Image, TTCLR_DEFAULT);
		m_pChangeLookBtn = (cPushupButton*)GetWindowForID(IN_CHANGELOOKBTN);
		m_pChangeLookBtn->SetActive(false);
		m_pItemShopInvenPlus = (cShopItemInventPlusDlg*)GetWindowForID(IN_SHOPITEMGRID2);
		m_pItemShopInvenPlus->SetActive(false);
		m_pWearedDlg = (CWearedExDialog*)GetWindowForID(IN_WEAREDDLG);
		m_pMoneyEdit = (cStatic*)GetWindowForID(IN_MONEYEDIT);
		m_pTidyMallBtn = (cButton*)GetWindowForID(IN_BTN_M_REARRANGE);
		m_pClearInvBtn[MALLINV] = (cButton*)GetWindowForID(IN_CLEARBAIBAO);
		m_pClearInvBtn[ITEMINV] = (cButton*)GetWindowForID(IN_CLEARITEM);
		m_fSpread = TRUE;
		p_Money = (cStatic*)GetWindowForID(IN_BACKMONEY);	//返回金钱/泡点/元宝
		p_PaoDian = (cStatic*)GetWindowForID(IN_BACKPODIAN);
		p_Gold = (cStatic*)GetWindowForID(IN_BACKGOLD);

		pBuffBtn = (cButton*)GetWindowForID(IN_GETBUFFLIST);
	}
}
void CInventoryExDialog::GetBuff()
{
	char line[512];

	sprintf(line,  CHATMGR->GetChatMsg(2763));
	pBuffBtn->AddToolTipLine(line, TTTC_ITEMGROW5);
	pBuffBtn->AddToolTipLine("");

	DWORD color = TTTC_ITEMGROW;
	if (HERO->GetSetItemQualityStats()->wGenGol != 0)
	{
		sprintf(line, "%s +%d  Max:%d", CHATMGR->GetChatMsg(382), HERO->GetSetItemQualityStats()->wGenGol, 40);
		if (HERO->GetSetItemQualityStats()->wGenGol == 40)
		{
			color = TTTC_ITEMGROW5;
		}
		else
		{
			color = TTTC_ITEMGROW0;
		}

		pBuffBtn->AddToolTipLine(line, color);
	}
	if (HERO->GetSetItemQualityStats()->wMinChub != 0)
	{
		sprintf(line, "%s +%d  Max:%d", CHATMGR->GetChatMsg(383), HERO->GetSetItemQualityStats()->wMinChub, 40);
		if (HERO->GetSetItemQualityStats()->wMinChub == 40)
		{
			color = TTTC_ITEMGROW5;
		}
		else
		{
			color = TTTC_ITEMGROW0;
		}

		pBuffBtn->AddToolTipLine(line, color);
	}
	if (HERO->GetSetItemQualityStats()->wCheRyuk != 0)
	{
		sprintf(line, "%s +%d  Max:%d", CHATMGR->GetChatMsg(384), HERO->GetSetItemQualityStats()->wCheRyuk, 60);
		if (HERO->GetSetItemQualityStats()->wCheRyuk == 60)
		{
			color = TTTC_ITEMGROW5;
		}
		else
		{
			color = TTTC_ITEMGROW0;
		}

		pBuffBtn->AddToolTipLine(line, color);
	}
	if (HERO->GetSetItemQualityStats()->wSimMek != 0)
	{
		sprintf(line, "%s +%d  Max:%d", CHATMGR->GetChatMsg(385), HERO->GetSetItemQualityStats()->wSimMek, 40);
		if (HERO->GetSetItemQualityStats()->wSimMek == 40)
		{
			color = TTTC_ITEMGROW5;
		}
		else
		{
			color = TTTC_ITEMGROW0;
		}

		pBuffBtn->AddToolTipLine(line, color);
	}
	if (HERO->GetSetItemQualityStats()->dwLife != 0)
	{
		sprintf(line, "%s  +%d%%  Max:%d%%", CHATMGR->GetChatMsg(2764), HERO->GetSetItemQualityStats()->dwLife, 60);
		if (HERO->GetSetItemQualityStats()->dwLife == 60)
		{
			color = TTTC_ITEMGROW5;
		}
		else
		{
			color = TTTC_ITEMGROW0;
		}

		pBuffBtn->AddToolTipLine(line, color);
	}
	if (HERO->GetSetItemQualityStats()->dwShield != 0)
	{
		sprintf(line, "%s  +%d%%  Max:%d%%", CHATMGR->GetChatMsg(2765),HERO->GetSetItemQualityStats()->dwShield, 60);
		if (HERO->GetSetItemQualityStats()->dwShield == 60)
		{
			color = TTTC_ITEMGROW5;
		}
		else
		{
			color = TTTC_ITEMGROW0;
		}

		pBuffBtn->AddToolTipLine(line, color);
	}
	if (HERO->GetSetItemQualityStats()->dwNaeRyuk != 0)
	{
		sprintf(line, "内力+%d%%  Max:%d%%", HERO->GetSetItemQualityStats()->dwNaeRyuk, 60);
		if (HERO->GetSetItemQualityStats()->dwNaeRyuk == 60)
		{
			color = TTTC_ITEMGROW5;
		}
		else
		{
			color = TTTC_ITEMGROW0;
		}

		pBuffBtn->AddToolTipLine(line, color);
	}
	if (HERO->GetSetItemQualityStats()->AttrRegistDef != 0)
	{
		sprintf(line, "属性防御+%d%%  Max:%d%%", HERO->GetSetItemQualityStats()->AttrRegistDef, 30);
		if (HERO->GetSetItemQualityStats()->AttrRegistDef == 30)
		{
			color = TTTC_ITEMGROW5;
		}
		else
		{
			color = TTTC_ITEMGROW0;
		}

		pBuffBtn->AddToolTipLine(line, color);
	}
	if (HERO->GetSetItemQualityStats()->wPhyDef != 0)
	{
		sprintf(line, "物理防御+%d%%  Max:%d%%", HERO->GetSetItemQualityStats()->wPhyDef, 30);
		if (HERO->GetSetItemQualityStats()->wPhyDef == 30)
		{
			color = TTTC_ITEMGROW5;
		}
		else
		{
			color = TTTC_ITEMGROW0;
		}

		pBuffBtn->AddToolTipLine(line, color);
	}
	if (HERO->GetSetItemQualityStats()->NaegongDamage != 0)
	{
		sprintf(line, "内功伤害+%d%%  Max:%d%%", HERO->GetSetItemQualityStats()->NaegongDamage, 20);
		if (HERO->GetSetItemQualityStats()->NaegongDamage == 20)
		{
			color = TTTC_ITEMGROW5;
		}
		else
		{
			color = TTTC_ITEMGROW0;
		}

		pBuffBtn->AddToolTipLine(line, color);
	}
	if (HERO->GetSetItemQualityStats()->WoigongDamage != 0)
	{
		sprintf(line, "外功伤害+%d%%  Max:%d%%", HERO->GetSetItemQualityStats()->WoigongDamage, 20);
		if (HERO->GetSetItemQualityStats()->WoigongDamage == 20)
		{
			color = TTTC_ITEMGROW5;
		}
		else
		{
			color = TTTC_ITEMGROW0;
		}

		pBuffBtn->AddToolTipLine(line, color);
	}
	if (HERO->GetSetItemQualityStats()->wDodgeRate != 0)
	{
		sprintf(line, "闪避+%d%%  Max:%d%%", HERO->GetSetItemQualityStats()->wDodgeRate, 20);
		if (HERO->GetSetItemQualityStats()->wDodgeRate == 20)
		{
			color = TTTC_ITEMGROW5;
		}
		else
		{
			color = TTTC_ITEMGROW0;
		}

		pBuffBtn->AddToolTipLine(line, color);
	}
	if (HERO->GetSetItemQualityStats()->PlayerPhyDefDown != 0)
	{
		sprintf(line, "PVP破甲+%d%%  Max:%d%%", HERO->GetSetItemQualityStats()->PlayerPhyDefDown, 20);
		if (HERO->GetSetItemQualityStats()->PlayerPhyDefDown == 20)
		{
			color = TTTC_ITEMGROW5;
		}
		else
		{
			color = TTTC_ITEMGROW0;
		}

		pBuffBtn->AddToolTipLine(line, color);
	}
	if (HERO->GetSetItemQualityStats()->PlayerAttrDefDown != 0)
	{
		sprintf(line, "PVP破魔+%d%%  Max:%d%%", HERO->GetSetItemQualityStats()->PlayerAttrDefDown, 20);
		if (HERO->GetSetItemQualityStats()->PlayerAttrDefDown == 20)
		{
			color = TTTC_ITEMGROW5;
		}
		else
		{
			color = TTTC_ITEMGROW0;
		}

		pBuffBtn->AddToolTipLine(line, color);
	}
	if (HERO->GetSetItemQualityStats()->TargetPhyDefDown != 0)
	{
		sprintf(line, "PVE破甲+%d%%  Max:%d%%", HERO->GetSetItemQualityStats()->TargetPhyDefDown, 30);
		if (HERO->GetSetItemQualityStats()->TargetPhyDefDown == 30)
		{
			color = TTTC_ITEMGROW5;
		}
		else
		{
			color = TTTC_ITEMGROW0;
		}

		pBuffBtn->AddToolTipLine(line, color);
	}
	if (HERO->GetSetItemQualityStats()->TargetAttrDefDown != 0)
	{
		sprintf(line, "PVE破魔+%d%%  Max:%d%%", HERO->GetSetItemQualityStats()->TargetAttrDefDown, 30);
		if (HERO->GetSetItemQualityStats()->TargetAttrDefDown == 30)
		{
			color = TTTC_ITEMGROW5;
		}
		else
		{
			color = TTTC_ITEMGROW0;
		}

		pBuffBtn->AddToolTipLine(line, color);
	}
	if (HERO->GetSetItemQualityStats()->fDodgeRate != 0)
	{
		sprintf(line, "命中+%d%%  Max:%d%%", HERO->GetSetItemQualityStats()->fDodgeRate, 20);
		if (HERO->GetSetItemQualityStats()->fDodgeRate == 20)
		{
			color = TTTC_ITEMGROW5;
		}
		else
		{
			color = TTTC_ITEMGROW0;
		}

		pBuffBtn->AddToolTipLine(line, color);
	}
	if (HERO->GetSetItemQualityStats()->MallMoneyPuls != 0)
	{
		sprintf(line, "泡点加成+%d  Max:%d", HERO->GetSetItemQualityStats()->MallMoneyPuls, 20);
		if (HERO->GetSetItemQualityStats()->MallMoneyPuls == 20)
		{
			color = TTTC_ITEMGROW5;
		}
		else
		{
			color = TTTC_ITEMGROW0;
		}

		pBuffBtn->AddToolTipLine(line, color);
	}
	if (HERO->GetSetItemQualityStats()->KyunggongSpeed != 0)
	{
		sprintf(line, "轻功速度+%d  Max:%d", HERO->GetSetItemQualityStats()->KyunggongSpeed, 200);
		if (HERO->GetSetItemQualityStats()->KyunggongSpeed == 200)
		{
			color = TTTC_ITEMGROW5;
		}
		else
		{
			color = TTTC_ITEMGROW0;
		}

		pBuffBtn->AddToolTipLine(line, color);
	}
	if (HERO->GetSetItemQualityStats()->AttMonsterDamage != 0)
	{
		sprintf(line, "PVE最终伤害+%d%%  Max:%d%%", HERO->GetSetItemQualityStats()->AttMonsterDamage, 20);
		if (HERO->GetSetItemQualityStats()->AttMonsterDamage == 20)
		{
			color = TTTC_ITEMGROW5;
		}
		else
		{
			color = TTTC_ITEMGROW0;
		}

		pBuffBtn->AddToolTipLine(line, color);
	}
	if (HERO->GetSetItemQualityStats()->AttPlayerDamage != 0)
	{
		sprintf(line, "PVP最终伤害+%d%%  Max:%d%%", HERO->GetSetItemQualityStats()->AttPlayerDamage, 10);
		if (HERO->GetSetItemQualityStats()->AttPlayerDamage == 10)
		{
			color = TTTC_ITEMGROW5;
		}
		else
		{
			color = TTTC_ITEMGROW0;
		}

		pBuffBtn->AddToolTipLine(line, color);
	}
	if (HERO->GetSetItemQualityStats()->RealDamageDown != 0)
	{
		sprintf(line, "受到伤害减少%d%%  Max:%d%%", HERO->GetSetItemQualityStats()->RealDamageDown, 20);
		if (HERO->GetSetItemQualityStats()->RealDamageDown == 20)
		{
			color = TTTC_ITEMGROW5;
		}
		else
		{
			color = TTTC_ITEMGROW0;
		}

		pBuffBtn->AddToolTipLine(line, color);
	}
	if (HERO->GetSetItemQualityStats()->PVPLifePlus != 0)
	{
		sprintf(line, "PVP吸血几率+%d%%  Max:%d%%", HERO->GetSetItemQualityStats()->PVPLifePlus, 20);
		if (HERO->GetSetItemQualityStats()->PVPLifePlus == 20)
		{
			color = TTTC_ITEMGROW5;
		}
		else
		{
			color = TTTC_ITEMGROW0;
		}

		pBuffBtn->AddToolTipLine(line, color);
	}
	if (HERO->GetSetItemQualityStats()->Resurrected != 0)
	{
		sprintf(line, "%d%%几率满血复活  Max:%d%%", HERO->GetSetItemQualityStats()->Resurrected, 20);
		if (HERO->GetSetItemQualityStats()->Resurrected == 20)
		{
			color = TTTC_ITEMGROW5;
		}
		pBuffBtn->AddToolTipLine(line, color);
	}
	if (HERO->GetSetItemQualityStats()->Critical != 0)
	{
		sprintf(line, "外功暴击率+%d%%  Max:%d%%", HERO->GetSetItemQualityStats()->Critical, 30);
		if (HERO->GetSetItemQualityStats()->Critical == 30)
		{
			color = TTTC_ITEMGROW5;
		}
		else
		{
			color = TTTC_ITEMGROW0;
		}

		pBuffBtn->AddToolTipLine(line, color);
	}
	if (HERO->GetSetItemQualityStats()->Decisive != 0)
	{
		sprintf(line, "内功暴击率+%d%%  Max:%d%%", HERO->GetSetItemQualityStats()->Decisive, 30);
		if (HERO->GetSetItemQualityStats()->Decisive == 30)
		{
			color = TTTC_ITEMGROW5;
		}
		else
		{
			color = TTTC_ITEMGROW0;
		}

		pBuffBtn->AddToolTipLine(line, color);
	}
	if (HERO->GetSetItemQualityStats()->CriticalDamage != 0)
	{
		sprintf(line, "外功暴击伤害+%d%%  Max:%d%%", HERO->GetSetItemQualityStats()->CriticalDamage, 20);
		if (HERO->GetSetItemQualityStats()->CriticalDamage == 20)
		{
			color = TTTC_ITEMGROW5;
		}
		else
		{
			color = TTTC_ITEMGROW0;
		}

		pBuffBtn->AddToolTipLine(line, color);
	}
	if (HERO->GetSetItemQualityStats()->DecisiveDamage != 0)
	{
		sprintf(line, "内功暴击伤害+%d%%  Max:%d%%", HERO->GetSetItemQualityStats()->DecisiveDamage, 20);
		if (HERO->GetSetItemQualityStats()->DecisiveDamage == 20)
		{
			color = TTTC_ITEMGROW5;
		}
		else
		{
			color = TTTC_ITEMGROW0;
		}

		pBuffBtn->AddToolTipLine(line, color);
	}
	if (HERO->GetSetItemQualityStats()->ContinueAttAttack != 0)
	{
		sprintf(line, "持续伤害+%d%%  Max:%d%%", HERO->GetSetItemQualityStats()->ContinueAttAttack, 20);
		if (HERO->GetSetItemQualityStats()->ContinueAttAttack == 20)
		{
			color = TTTC_ITEMGROW5;
		}
		else
		{
			color = TTTC_ITEMGROW0;
		}

		pBuffBtn->AddToolTipLine(line, color);
	}
}
CItem * CInventoryExDialog::GetItemLike(WORD wItemIdx)
{
	ITEM_INFO* pItemInfo = ITEMMGR->GetItemInfo(wItemIdx);
	if (pItemInfo->ItemKind & eSHOP_ITEM)
	{
		for (int i = 0; i<(SLOT_SHOPINVEN_NUM + TABCELL_SHOPINVEN_PLUS_NUM); i++)
		{
			if (m_pItemShopInven->InPt(i + TP_SHOPINVEN_START))
			{
				if (!m_pItemShopInven->IsAddable(i))
				{
					CItem* pItem = (CItem*)m_pItemShopInven->GetIconForIdx(i);
					if (pItem->GetItemIdx() == wItemIdx && !pItem->IsLocked())
					{
						return pItem;
					}
				}
			}
			else if (m_pItemShopInvenPlus->InPt(i + TP_SHOPINVEN_START))
			{
				if (!m_pItemShopInvenPlus->IsAddable(i - TABCELL_SHOPINVEN_NUM))
				{
					CItem* pItem = (CItem*)m_pItemShopInvenPlus->GetIconForIdx(i - TABCELL_SHOPINVEN_NUM);
					if (pItem->GetItemIdx() == wItemIdx && !pItem->IsLocked())
					{
						return pItem;
					}
				}
			}
		}
	}
	BYTE TabNum = GetTabNum();
	for (BYTE i = 0; i < TabNum; ++i)
	{
		cIconGridDialog* gridDlg = (cIconGridDialog*)GetTabSheet(i);
		for (WORD j = 0; j < gridDlg->GetCellNum(); ++j)
		{
			if (!gridDlg->IsAddable(j))
			{
				CItem * pItem = (CItem *)gridDlg->GetIconForIdx(TP_INVENTORY_START + j);
				if (pItem->GetItemIdx() == wItemIdx)
					return pItem;
			}
		}
	}
	return NULL;
}
DURTYPE	CInventoryExDialog::GetTotalItemDurability(WORD wItemIdx)
{
	DURTYPE DurSum = 0;
	ITEM_INFO* pItemInfo = ITEMMGR->GetItemInfo(wItemIdx);
	if (pItemInfo)
	if (pItemInfo->ItemKind & eSHOP_ITEM)
	{
		for (int i = 0; i<m_pItemShopInven->GetCellNum(); i++)
		{
			if (!m_pItemShopInven->IsAddable(i))
			{
				CItem* pItem = (CItem*)m_pItemShopInven->GetIconForIdx(i);
				if (pItem->GetItemIdx() == wItemIdx && !(pItem->GetItemParam()&ITEM_PARAM_SEAL))
					DurSum += pItem->GetDurability();
			}
		}
		for (int i = 0; i<m_pItemShopInvenPlus->GetCellNum(); i++)
		{
			if (!m_pItemShopInvenPlus->IsAddable(i))
			{
				CItem* pItem = (CItem*)m_pItemShopInvenPlus->GetIconForIdx(i);
				if (pItem->GetItemIdx() == wItemIdx && !(pItem->GetItemParam()&ITEM_PARAM_SEAL))
					DurSum += pItem->GetDurability();
			}
		}
		return DurSum;
	}
	BYTE TabNum = GetTabNum();
	for (BYTE i = 0; i < TabNum; ++i)
	{
		cIconGridDialog* gridDlg = (cIconGridDialog*)GetTabSheet(i);
		for (WORD j = 0; j < gridDlg->GetCellNum(); ++j)
		{
			if (!gridDlg->IsAddable(j))
			{
				CItem * pItem = (CItem *)gridDlg->GetIconForIdx(TP_INVENTORY_START + j);
				if (pItem->GetItemIdx() == wItemIdx)
					DurSum += pItem->GetDurability();
			}
		}
	}
	return DurSum;
}
BOOL CInventoryExDialog::GetPositionForXYRef(BYTE param, LONG x, LONG y, WORD& pos)
{
	WORD position = 0;
	if (param != 2)
	{
		if (m_bShopMInven[0])
		{
			if (m_pItemShopInven->GetPositionForXYRef(x, y, position))
			{
				pos = position + TP_SHOPINVEN_START;
				return TRUE;
			}
		}
		if (m_bShopMInven[1])
		{
			if (m_pItemShopInvenPlus->GetPositionForXYRef(x, y, position))
			{
				pos = position + TP_SHOPINVEN_START + TABCELL_SHOPINVEN_NUM;
				return TRUE;
			}
		}
		if (m_pItemShopInven)
		{
			cIconGridDialog * gridDlg = (cIconGridDialog *)GetTabSheet(GetCurTabNum());
			if (gridDlg->GetPositionForXYRef(x, y, position))
			{
				pos = position + TP_INVENTORY_START + TABCELL_INVENTORY_NUM*GetCurTabNum();
				return TRUE;
			}
		}
	}
	if (param != 1)
	if (m_pWearedDlg->GetPositionForXYRef(x, y, position))
	{
		pos = position + TP_WEAR_START;
		return TRUE;
	}
	return FALSE;
}
BOOL CInventoryExDialog::CheckItemLockForItemIdx(WORD wItemIdx)
{
	BYTE TabNum = GetTabNum();
	for (BYTE i = 0; i < TabNum; ++i)
	{
		cIconGridDialog* gridDlg = (cIconGridDialog*)GetTabSheet(i);
		for (WORD j = 0; j < gridDlg->GetCellNum(); ++j)
		{
			if (!gridDlg->IsAddable(j))
			{
				CItem * pItem = (CItem *)gridDlg->GetIconForIdx(TP_INVENTORY_START + j);
				if (pItem->GetItemIdx() == wItemIdx && pItem->IsLocked())
					return FALSE;
			}
		}
	}
	return TRUE;
}
void CInventoryExDialog::SetMoney(DWORD Money)
{
	m_pMoneyEdit->SetStaticText(AddComma(Money));
#ifdef _OLDGOLDDIALOG_
	GAMEIN->GetGoldShopDialog()->SetMoney(Money);
#endif
}
BOOL CInventoryExDialog::IsExist(POSTYPE abs_pos)
{
	BOOL bExist = FALSE;
	if (TP_INVENTORY_START <= abs_pos && abs_pos < TP_INVENTORY_END)
	{
		int tabIndex = GetTabIndex(abs_pos);
		cIconGridDialog * dlg = (cIconGridDialog *)GetTabSheet(tabIndex);
		if (dlg->GetIconForIdx(abs_pos - TABCELL_INVENTORY_NUM*tabIndex - TP_INVENTORY_START))
			bExist = TRUE;
	}
	else if (TP_WEAR_START <= abs_pos && abs_pos < TP_WEAR_END)
	{
		if (m_pWearedDlg->GetIconForIdx(abs_pos - TP_WEAR_START))
			bExist = TRUE;
	}
	return bExist;
}
BOOL CInventoryExDialog::CanBeMoved(CItem* pItem, POSTYPE pos)
{
	ASSERT(pItem);
	if (pItem)
	{
		POSTYPE fromPos = pItem->GetPosition();
		POSTYPE toPos = pos;
		if ((TP_WEAR_START <= fromPos && fromPos < TP_WEAR_END) ||
			(TP_WEAR_START <= toPos && toPos < TP_WEAR_END))
		{
			if (OBJECTSTATEMGR->CheckAvailableState(HERO, eCAS_EQUIPITEM) == FALSE)
			{
				CAction act;
				act.InitAction(eActionKind_EquipItem, (void*)pItem->GetDBIdx(), 0, NULL);
				HERO->SetNextAction(&act);
				return FALSE;
			}
		}
	}
	if (TP_INVENTORY_START <= pos && pos < TP_INVENTORY_END)
	{
		POSTYPE frompos = pItem->GetPosition();
		if (TP_SHOPITEM_START <= frompos && frompos < TP_SHOPITEM_END)
			return FALSE;
		else if (TP_SHOPINVEN_START <= frompos && frompos < TP_SHOPINVEN_END)
			return FALSE;
		else if (TP_PETWEAR_START <= frompos && frompos < TP_PETWEAR_END)
			return FALSE;
		else if (TP_TITANWEAR_START <= frompos && frompos < TP_TITANWEAR_END)
			return TRUE;
		else
			return TRUE;
	}
	else if (TP_WEAR_START <= pos && pos < TP_WEAR_END)
	{
		if (!(pItem->GetItemKind() & eEQUIP_ITEM))
		{
			if (pItem->GetItemKind() == eQUEST_ITEM_EQUIP)
			{
			}
			else if (pItem->GetItemKind() == eSHOP_ITEM_EQUIP)
			{
				if (pItem->GetItemParam() & ITEM_PARAM_SEAL)
					return FALSE;
			}
			else
				return FALSE;
		}
		else
		{
			CItem* ptarget = (CItem*)m_pWearedDlg->GetIconForIdx(pos - TP_WEAR_START);
			if (ptarget)
			{
				POSTYPE frompos = pItem->GetPosition();
				if ((TP_PYOGUK_START <= frompos && frompos < TP_PYOGUK_END) &&
					ptarget->GetItemKind() & eSHOP_ITEM_EQUIP)
					return FALSE;
			}
		}
		ITEM_INFO * pInfo = ITEMMGR->GetItemInfo(pItem->GetItemIdx());
		if (!pInfo) return FALSE;
		if (pInfo->EquipKind == eWearedItem_Ring1)
		{
			if (pos - TP_WEAR_START != eWearedItem_Ring1 && pos - TP_WEAR_START != eWearedItem_Ring2)
				return FALSE;
		}
		else
		{
			if (pos - TP_WEAR_START != pInfo->EquipKind)
				return FALSE;
		}
		return ITEMMGR->CanEquip(pItem->GetItemIdx());
	}
	else if (TP_PYOGUK_START <= pos && pos < TP_PYOGUK_END)
	{
		return TRUE;
	}
	else if (TP_SHOPINVEN_START <= pos && pos <TP_SHOPINVEN_END)
	{
		POSTYPE frompos = pItem->GetPosition();
		if (TP_SHOPITEM_START <= frompos && frompos < TP_SHOPITEM_END)
			return TRUE;
		else if (TP_SHOPINVEN_START <= frompos && frompos < TP_SHOPINVEN_END)
			return TRUE;
		else if (TP_WEAR_START <= frompos && frompos < TP_WEAR_END)
		{
			return TRUE;
		}
		else if (TP_PETWEAR_START <= frompos && frompos < TP_PETWEAR_END)
			return FALSE;
		else if (TP_TITANSHOPITEM_START <= frompos && frompos < TP_TITANSHOPITEM_END)
			return TRUE;
		else
			return FALSE;
	}
	else if (TP_PETWEAR_START <= pos && pos < TP_PETWEAR_END)
	{
		POSTYPE fromPos = pItem->GetPosition();
		if (TP_SHOPINVEN_START <= fromPos && fromPos < TP_SHOPINVEN_END)
		{
			CPetWearedExDialog* pPetEquipDlg = ((CPetInventoryDlg*)GAMEIN->GetPetInventoryDialog())->GetPetWearedDlg();
			if (FALSE == pPetEquipDlg->CheckDuplication(pItem->GetItemIdx()))
			{
				return TRUE;
			}
			return FALSE;
		}
		else
			return FALSE;
	}
	else if (TP_TITANWEAR_START <= pos && pos < TP_TITANWEAR_END)
	{
		if ((pItem->GetItemKind() & eTITAN_EQUIPITEM))
		{
			ITEM_INFO * pInfo = ITEMMGR->GetItemInfo(pItem->GetItemIdx());
			if (!pInfo) return FALSE;
			if (pos - TP_TITANWEAR_START != pInfo->ItemKind - eTITAN_EQUIPITEM_HELMET)
			{
				CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(1505));
				return FALSE;
			}
			if (pInfo->LimitLevel > HERO->GetLevel())
			{
				char temp[256] = { 0, };
				sprintf(temp, CHATMGR->GetChatMsg(238), (int)pInfo->LimitLevel);
				CHATMGR->AddMsg(CTC_SYSMSG, temp);
				return FALSE;
			}
			return TRUE;
		}
	}
	else if (TP_TITANSHOPITEM_START <= pos && pos < TP_TITANSHOPITEM_END)
	{
		if (pItem->GetItemKind() == eSHOP_ITEM_TITAN_EQUIP)
		{
			ITEM_INFO * pInfo = ITEMMGR->GetItemInfo(pItem->GetItemIdx());
			if (!pInfo) return FALSE;
			return TRUE;
		}
	}
	return FALSE;
}
void CInventoryExDialog::ItemUseForQuestStart()
{
	if (m_pTempQuestItem)
	{
		MSG_ITEM_USE_SYN msg;
		msg.Category = MP_ITEM;
		msg.Protocol = MP_ITEM_USE_FOR_QUESTSTART_SYN;
		msg.dwObjectID = HEROID;
		msg.TargetPos = m_pTempQuestItem->GetPosition();
		msg.wItemIdx = m_pTempQuestItem->GetItemIdx();
		NETWORK->Send(&msg, sizeof(msg));
	}
}
void CInventoryExDialog::ItemUnSealingSync()
{
	BOOL bIsPlus = FALSE;
	CItem* pItem = ITEMMGR->GetItemofTable(eItemTable_ShopInven, (POSTYPE)(m_pItemShopInven->GetCurSelCellPos() + TP_SHOPINVEN_START));
	if (!pItem)
	{
		bIsPlus = TRUE;
		pItem = ITEMMGR->GetItemofTable(eItemTable_ShopInven, (POSTYPE)(m_pItemShopInvenPlus->GetCurSelCellPos() + TP_SHOPINVEN_START + TABCELL_SHOPINVEN_NUM));
	}
	if (!pItem || !(pItem->GetItemParam()&ITEM_PARAM_SEAL))				return;
	if (pItem->IsLocked()) return;
	ITEM_INFO* pItemInfo = ITEMMGR->GetItemInfo(pItem->GetItemIdx());
	if (!pItemInfo)	return;
	if (pItemInfo->ItemKind == eSHOP_ITEM_MAKEUP ||
		pItemInfo->ItemKind == eSHOP_ITEM_DECORATION ||
		pItemInfo->ItemKind == eSHOP_ITEM_IMAGENAME ||
		pItemInfo->ItemKind == eSHOP_ITEM_PET)
	{
		if (ITEMMGR->GetUsedItemInfo(pItemInfo->ItemIdx))
		{
			CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(752));
			return;
		}
	}
	MSG_DWORD msg;
	msg.Category = MP_ITEM;
	msg.Protocol = MP_ITEM_SHOPITEM_UNSEAL_SYN;
	msg.dwObjectID = HERO->GetID();
	if (bIsPlus)
		msg.dwData = m_pItemShopInvenPlus->GetCurSelCellPos() + TP_SHOPINVEN_START + TABCELL_SHOPINVEN_NUM;
	else
		msg.dwData = m_pItemShopInven->GetCurSelCellPos() + TP_SHOPINVEN_START;
	NETWORK->Send(&msg, sizeof(msg));
}
void CInventoryExDialog::ItemUnSealing(DWORD absPos)
{
	CItem* pItem = ITEMMGR->GetItemofTable((WORD)eItemTable_ShopInven, (POSTYPE)absPos);
	if (!pItem)		return;
	ITEMPARAM Param = pItem->GetItemBaseInfo()->ItemParam;
	Param ^= ITEM_PARAM_SEAL;
	pItem->SetItemParam(Param);
	ITEM_INFO* pItemInfo = ITEMMGR->GetItemInfo(pItem->GetItemIdx());
	if (pItemInfo &&
		pItemInfo->ItemKind != eSHOP_ITEM_MAKEUP &&
		pItemInfo->ItemKind != eSHOP_ITEM_DECORATION &&
		pItemInfo->ItemKind != eSHOP_ITEM_EQUIP)
		ITEMMGR->SetToolTipIcon(pItem);
	CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(776));
}
void CInventoryExDialog::SetShopItemInvenBtn(BOOL val)
{
	m_pMPageBtn[0]->SetActive(val);
}
CItem* CInventoryExDialog::GetPriorityItemForCristmasEvent()
{
	CItem* pItem = NULL;
	pItem = GetItemLike(EVENT_ITEM_HARDSNOW);
	if (pItem)
		return pItem;
	else
	{
		pItem = GetItemLike(EVENT_ITEM_SNOW);
	}
	return pItem;
}
DWORD CInventoryExDialog::GetPriorityItemIdx()
{
	CItem* pItem = NULL;
	DWORD Idx = 0;
	pItem = GetPriorityItemForCristmasEvent();
	if (pItem)
	{
		Idx = pItem->GetItemIdx();
	}
	return Idx;
}
BOOL CInventoryExDialog::CheckItemLock()
{
	for (int i = TP_INVENTORY_START; i<TP_INVENTORY_END; i++)
	{
		CItem * pItem = GetItemForPos(i);
		if (pItem)
		{
			if (pItem->IsLocked())
			{
				return FALSE;
			}
		}
	}
	return TRUE;
}
BOOL CInventoryExDialog::CheckItemQuick()
{
	for (int i = TP_INVENTORY_START; i<TP_INVENTORY_END; i++)
	{
		CItem * pItem = GetItemForPos(i);
		if (pItem)
		{
			if (!QUICKMGR->CheckQPosForItemIdx(pItem->GetItemIdx()))
			{
				return FALSE;
			}
		}
	}
	return TRUE;
}
DWORD CInventoryExDialog::GetItemCount()
{
	int Num = 0;
	for (int i = TP_INVENTORY_START; i<TP_INVENTORY_END; i++)
	{
		CItem * pItem = GetItemForPos(i);
		if (pItem)
		{
			Num = Num + 1;
		}
	}
	return Num;
}
DWORD CInventoryExDialog::GetItemMallCount()
{
	int Num = 0;
	int b = 430 + TABCELL_SHOPINVEN_NUM;
	for (int i = 390; i <= b; i++)
	{
		CItem* pItem = GetItemForPos(i);
		if (pItem)
		{
			Num = Num + 1;
		}
	}
	return Num;
}
void CInventoryExDialog::DeleteAllItem()
{
	for (int i = 0; i < 80; i++)
	{
		CItem * FromItem = GAMEIN->GetInventoryDialog()->GetItemForPos(i);
		if (FromItem)
		{
			if (FromItem->GetQuickPosition() != 0)
			{
				QUICKMGR->RemQuickItem(FromItem->GetQuickPosition());
			}
			GAMEIN->GetInventoryDialog()->DeleteItem(i, &FromItem);
			if (FromItem)
			{
				ITEMMGR->ItemDelete(FromItem);
				FromItem = NULL;
			}
		}
	}
	WINDOWMGR->DestroyWindowProcess();
}
void CInventoryExDialog::DeleteAllItemShop()
{
	for (int i = TP_SHOPINVEN_START; i<TP_SHOPINVEN_END; i++)
	{
		CItem * FromItem = GAMEIN->GetInventoryDialog()->GetItemForPos(i);
		if (FromItem)
		{
			if (FromItem->GetQuickPosition() != 0)
			{
				QUICKMGR->RemQuickItem(FromItem->GetQuickPosition());
			}
			GAMEIN->GetInventoryDialog()->DeleteItem(i, &FromItem);
			if (FromItem)
			{
				ITEMMGR->ItemDelete(FromItem);
				FromItem = NULL;
			}
		}
	}
	WINDOWMGR->DestroyWindowProcess();
}
void CInventoryExDialog::SetInventoryItemTidy()
{
	if ((m_dwItemTidyDelayTime != 0) && (m_dwItemTidyDelayTime > gCurTime))
	{
		DWORD dwTime = ((m_dwItemTidyDelayTime - gCurTime) / 1000) + 1;
		CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(2331), dwTime);
		return;
	}
	if (HERO->GetState() == eObjectState_Die ||
		HERO->GetState() == eObjectState_Exchange ||
		HERO->GetState() == eObjectState_StreetStall_Owner ||
		HERO->GetState() == eObjectState_StreetStall_Guest)
	{
		CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(2198));
		return;
	}
	if (GetItemCount() == 0)
	{
		CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(2274));
		return;
	}
	if (!CheckItemLock())
	{
		CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(2199));
		return;
	}
	if (HERO->IsPKMode() || PKMGR->IsPKLooted())
	{
		CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(2369));
		return;
	}
	if (GAMEIN->GetRareCreateDlg()->IsActive())
	{
		CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(2273));
		return;
	}
	MSG_DWORD msg;
	msg.Category = MP_ITEMEXT;
	msg.Protocol = MP_ITEMEXT_TIDY_SYN;
	msg.dwObjectID = HERO->GetID();
	msg.dwData = 1986;
	NETWORK->Send(&msg, sizeof(msg));
}
void CInventoryExDialog::SetInventoryItemTidyShop()
{
	if ((m_dwItemTidyShopDelayTime != 0) && (m_dwItemTidyShopDelayTime > gCurTime))
	{
		DWORD dwTime = ((m_dwItemTidyShopDelayTime - gCurTime) / 1000) + 1;
		CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(2331), dwTime);
		return;
	}
	if (HERO->GetState() == eObjectState_Die ||
		HERO->GetState() == eObjectState_Exchange ||
		HERO->GetState() == eObjectState_StreetStall_Owner ||
		HERO->GetState() == eObjectState_StreetStall_Guest)
	{
		CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(2198));
		return;
	}
	if (GetItemMallCount() == 0)
	{
		CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(2274));
		return;
	}
	if (HERO->IsPKMode() || PKMGR->IsPKLooted())
	{
		CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(2369));
		return;
	}
	if (GAMEIN->GetRareCreateDlg()->IsActive())
	{
		CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(2273));
		return;
	}
	MSG_DWORD msg;
	msg.Category = MP_ITEMEXT;
	msg.Protocol = MP_ITEMEXT_TIDY_SHOP_SYN;
	msg.dwObjectID = HERO->GetID();
	msg.dwData = 1986;
	NETWORK->Send(&msg, sizeof(msg));
}
void CInventoryExDialog::StartItemTidyDelayTime(DWORD wTime)
{
	m_dwItemTidyDelayTime = wTime + gCurTime;
}
void CInventoryExDialog::StartItemTidyShopDelayTime(DWORD wTime)
{
	m_dwItemTidyShopDelayTime = wTime + gCurTime;
}
void CInventoryExDialog::GetItemFromPyoguk(CItem* pItem)
{
	CItem * pPosItem = NULL;
	for (int i = 0; i<80; i++)
	{
		if (!GetItemForPos(i))
		{
			FakeGeneralItemMove(i, pItem, NULL);
			return;
		}
	}
	CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(2632));
}
void CInventoryExDialog::GetItemFromItemShop(CItem* pItem)
{
	CItem * pPosItem = NULL;
	if (m_bShopMInven[0])
	{
		for (int i = TP_SHOPINVEN_START; i<(TP_SHOPINVEN_END - TABCELL_SHOPINVEN_NUM); i++)
		{
			if (!m_pItemShopInven->GetItemForPos(i))
			{
				m_pItemShopInven->FakeGeneralItemMove(i, pItem, NULL);
				return;
			}
		}
	}
	else if (m_bShopMInven[1])
	{
		for (int i = (TP_SHOPINVEN_START + TABCELL_SHOPINVEN_NUM); i<TP_SHOPINVEN_END; i++)
		{
			if (!m_pItemShopInvenPlus->GetItemForPos(i))
			{
				m_pItemShopInvenPlus->FakeGeneralItemMove(i, pItem, NULL);
				return;
			}
		}
	}
	CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(2632));
}
BOOL CInventoryExDialog::IsSelectFunctionKey()
{
	return KEYBOARD->GetKeyPressed(KEY_CONTROL);
}
BOOL CInventoryExDialog::IsDivideFunctionKey()
{
	return KEYBOARD->GetKeyPressed(KEY_MENU);
}
bool CInventoryExDialog::IsChangeLookButtonPushed()
{
	return m_pChangeLookBtn->IsPushed();
}
BOOL CInventoryExDialog::FakeMoveItem(CItem * pFromItem, WORD ToPos)
{
	CItem * pToItem = GetItemForPos(ToPos);
	if (ITEMMGR->GetTableIdxForAbsPos(ToPos) == eItemTable_Weared)
		AUDIOMGR->Play(59, HERO);
	else
		AUDIOMGR->Play(58, HERO);
	if (pToItem)
	if (pToItem->IsLocked()) return FALSE;
	if (pFromItem->IsLocked()) return FALSE;
	if (TP_PYOGUK_START <= pFromItem->GetPosition() && pFromItem->GetPosition() < TP_PYOGUK_END)
	{
		return FakeGeneralItemMove(ToPos, pFromItem, pToItem);
	}
	else if (TP_GUILDWAREHOUSE_START <= pFromItem->GetPosition() && pFromItem->GetPosition() < TP_GUILDWAREHOUSE_END)
	{
		return FakeGeneralItemMove(ToPos, pFromItem, pToItem);
	}

	else if (TP_SHOPITEM_START <= pFromItem->GetPosition() && pFromItem->GetPosition() < TP_SHOPITEM_END)
	{
		if (m_pItemShopInven->InPt(pFromItem->GetPosition()))
		{
			return m_pItemShopInven->FakeGeneralItemMove(ToPos, pFromItem, pToItem);
		}
		else if (m_pItemShopInvenPlus->InPt(pFromItem->GetPosition()))
		{
			return m_pItemShopInvenPlus->FakeGeneralItemMove(ToPos, pFromItem, pToItem);
		}

		else if (GAMEIN->GetChangeLookDialog()->InPt(pFromItem->GetPosition()))//kiv
		{
			return GAMEIN->GetChangeLookDialog()->FakeGeneralItemMove(ToPos, pFromItem, pToItem);
		}
	}
	else if (GAMEIN->GetChangeLookDialog()->InPt(pFromItem->GetPosition()))
	{
		return GAMEIN->GetChangeLookDialog()->FakeGeneralItemMove(ToPos, pFromItem, pToItem);
	}

	else if (m_pItemShopInven->InPt(pFromItem->GetPosition()))
	{
		if (ITEMMGR->IsDupItem(pFromItem->GetItemIdx()))
		{
			if (IsDivideFunctionKey() && !(pToItem))
			{
				return FakeItemDivide(ToPos, pFromItem, pToItem);
			}
			else if ((pToItem && pFromItem->GetItemIdx() == pToItem->GetItemIdx()))
			{
				return FakeItemCombine(ToPos, pFromItem, pToItem);
			}
			else
			{
				return m_pItemShopInven->FakeGeneralItemMove(ToPos, pFromItem, pToItem);
			}
		}
	}
	else if (m_pItemShopInvenPlus->InPt(pFromItem->GetPosition()))
	{
		if (ITEMMGR->IsDupItem(pFromItem->GetItemIdx()))
		{
			if (IsDivideFunctionKey() && !(pToItem))
			{
				return FakeItemDivide(ToPos, pFromItem, pToItem);
			}
			else if ((pToItem && pFromItem->GetItemIdx() == pToItem->GetItemIdx()))
			{
				return FakeItemCombine(ToPos, pFromItem, pToItem);
			}
			else
			{
				return m_pItemShopInvenPlus->FakeGeneralItemMove(ToPos, pFromItem, pToItem);
			}
		}
	}
	else if ((TP_SIEGEFORTWARE_START <= pFromItem->GetPosition() && pFromItem->GetPosition() < TP_SIEGEFORTWARE_END))
	{
		return GAMEIN->GetFWWareHouseDlg()->FakeGeneralItemMove(ToPos, pFromItem, pToItem);
	}
	if (ITEMMGR->IsDupItem(pFromItem->GetItemIdx()))
	{
		if (IsDivideFunctionKey() && !(pToItem))
		{
			return FakeItemDivide(ToPos, pFromItem, pToItem);
		}
		else if ((pToItem && pFromItem->GetItemIdx() == pToItem->GetItemIdx()))
		{
			return FakeItemCombine(ToPos, pFromItem, pToItem);
		}
	}
	return FakeGeneralItemMove(ToPos, pFromItem, pToItem);
}
void CInventoryExDialog::SetBackMoney(long Val)
{
	p_Money->SetStaticText(AddComma(Val));
	//	p_Money->SetAlign(TXT_CENTER);
	p_Money->SetAlign(TXT_RIGHT);
}

void CInventoryExDialog::SetBackGoldMoney(long Val)
{
	p_Gold->SetStaticText(AddComma(Val));
	//	p_Gold->SetAlign(TXT_CENTER);
	p_Gold->SetAlign(TXT_RIGHT);
}

void CInventoryExDialog::SetBackMallMoney(long Val)
{
	p_PaoDian->SetStaticText(AddComma(Val));
	//	p_PaoDian->SetAlign(TXT_CENTER);
	p_PaoDian->SetAlign(TXT_RIGHT);
}
BOOL CInventoryExDialog::FakeMoveItem(LONG mouseX, LONG mouseY, CItem * pFromItem)
{
	WORD ToPos = 0;
	if (!GetPositionForXYRef(0, mouseX, mouseY, ToPos))
	{
		return FALSE;
	}
	CItem * pToItem = GetItemForPos(ToPos);
	if (ITEMMGR->GetTableIdxForAbsPos(ToPos) == eItemTable_Weared)
	{
		AUDIOMGR->Play(59, HERO);
	}
	else
	{
		AUDIOMGR->Play(58, HERO);
	}
	if (pToItem)
	if (pToItem->IsLocked()) return FALSE;
	if (pFromItem->IsLocked()) return FALSE;

	if (TP_PYOGUK_START <= pFromItem->GetPosition() && pFromItem->GetPosition() < TP_PYOGUK_END)
	{
		return FakeGeneralItemMove(ToPos, pFromItem, pToItem);
	}
	else if (TP_GUILDWAREHOUSE_START <= pFromItem->GetPosition() && pFromItem->GetPosition() < TP_GUILDWAREHOUSE_END)
	{
		return FakeGeneralItemMove(ToPos, pFromItem, pToItem);
	}
	else if (TP_SHOPITEM_START <= pFromItem->GetPosition() && pFromItem->GetPosition() < TP_SHOPITEM_END)
	{
		if (m_pItemShopInven->InPt(pFromItem->GetPosition()))
		{
			return m_pItemShopInven->FakeGeneralItemMove(ToPos, pFromItem, pToItem);
		}
		else if (m_pItemShopInvenPlus->InPt(pFromItem->GetPosition()))
		{
			return m_pItemShopInvenPlus->FakeGeneralItemMove(ToPos, pFromItem, pToItem);
		}

		else if (GAMEIN->GetChangeLookDialog()->InPt(pFromItem->GetPosition()))//kiv
		{
			return GAMEIN->GetChangeLookDialog()->FakeGeneralItemMove(ToPos, pFromItem, pToItem);
		}
	}

	else if (GAMEIN->GetChangeLookDialog()->InPt(pFromItem->GetPosition()))//kiv
	{		
		GAMEIN->GetChangeLookDialog()->SetCurSelCellPos(-1);
		return GAMEIN->GetChangeLookDialog()->FakeGeneralItemMove(ToPos, pFromItem, pToItem);			
	}

	else if (m_pItemShopInven->InPt(pFromItem->GetPosition()))
	{
		if (ITEMMGR->IsDupItem(pFromItem->GetItemIdx()))
		{
			if (IsDivideFunctionKey() && !(pToItem))
			{
				return FakeItemDivide(ToPos, pFromItem, pToItem);
			}
			else if ((pToItem && pFromItem->GetItemIdx() == pToItem->GetItemIdx()))
			{
				return FakeItemCombine(ToPos, pFromItem, pToItem);
			}
			else
			{
				m_pItemShopInven->SetCurSelCellPos(-1);
				return m_pItemShopInven->FakeGeneralItemMove(ToPos, pFromItem, pToItem);
			}
		}
	}
	else if (m_pItemShopInvenPlus->InPt(pFromItem->GetPosition()))
	{
		if (ITEMMGR->IsDupItem(pFromItem->GetItemIdx()))
		{
			if (IsDivideFunctionKey() && !(pToItem))
			{
				return FakeItemDivide(ToPos, pFromItem, pToItem);
			}
			else if ((pToItem && pFromItem->GetItemIdx() == pToItem->GetItemIdx()))
			{
				return FakeItemCombine(ToPos, pFromItem, pToItem);
			}
			else
			{
				m_pItemShopInvenPlus->SetCurSelCellPos(-1);
				return m_pItemShopInvenPlus->FakeGeneralItemMove(ToPos, pFromItem, pToItem);
			}
		}
	}
	else if ((TP_SIEGEFORTWARE_START <= pFromItem->GetPosition() && pFromItem->GetPosition() < TP_SIEGEFORTWARE_END))
	{
		return GAMEIN->GetFWWareHouseDlg()->FakeGeneralItemMove(ToPos, pFromItem, pToItem);
	}
	if (ITEMMGR->IsDupItem(pFromItem->GetItemIdx()))
	{
		if (IsDivideFunctionKey() && !(pToItem))
		{
			return FakeItemDivide(ToPos, pFromItem, pToItem);
		}
		else if ((pToItem && pFromItem->GetItemIdx() == pToItem->GetItemIdx()))
		{
			return FakeItemCombine(ToPos, pFromItem, pToItem);
		}
	}
	return FakeGeneralItemMove(ToPos, pFromItem, pToItem);
}