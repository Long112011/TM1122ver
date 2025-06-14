#include "stdafx.h"
#include "ChangeLookDialog.h"
#include "WindowIDEnum.h"
#include "cWindowManager.h"
#include "InventoryExDialog.h"
#include "gamein.h"
#include "./Interface/cIconGridDialog.h"
#include "./Interface/cIcon.h"
#include "ItemManager.h"
#include "./Interface/cScriptManager.h"
#include "ChatManager.h"
#include "AppearanceManager.h"
#include "OptionManager.h"
#include "ObjectManager.h"
#include "MoveManager.h"
#include "DealItem.h"
#define jj GetLineMsg(__LINE__,__FUNCTION__,__FILE__,"" ,"");//
cChangeLookDialog::cChangeLookDialog()
{
	//m_type = WT_CHANGELOOK_DLG;
	m_type = WT_ICONGRIDDIALOG;
	m_nIconType = WT_ITEM;
	p_Dlg=NULL;
	p_Grid = NULL;
	m_pWearedDlg = NULL;

	memset(m_CheckIcon, 0, sizeof(cCheckBox*)* 7);
	//for (int i=0; i < 7; i++)
	//{
	//	m_CheckIcon[i] = NULL;
	//} 

	XX = YY = 0;

	m_IsAutoOpenBox = FALSE;
}
cChangeLookDialog::~cChangeLookDialog()
{
	p_Dlg=NULL;
	p_Grid = NULL;
	m_pWearedDlg = NULL;
	for (int i=0; i < 7; i++)
	{
		m_CheckIcon[i] = NULL;
	}

	XX = YY = 0;
}
void cChangeLookDialog::ReleaseIconWear(cIcon * Icon)
{
	if (Icon == NULL)
	{
		ASSERT(0);
		return;
	}
	m_pWearedDlg->DeleteIcon(m_pWearedDlg->GetCurSelCellPos(),&Icon);
	WINDOWMGR->AddListDestroyWindow(Icon);

	HERO->GetShopItemStats()->wSkinItem[m_pWearedDlg->GetCurSelCellPos()] = 0;


	sendToServer();
	
}
void cChangeLookDialog::sendToServer()
{
	MSG_DWORD7 msg;
	SetProtocol(&msg, MP_ITEMEXT, MP_ITEMEXT_SKINITEM_SELECT_SYN);
	msg.dwObjectID = HEROID;
	msg.dwData0 = HERO->GetShopItemStats()->wSkinItem[eSkinItem_Mask];
	msg.dwData1 = HERO->GetShopItemStats()->wSkinItem[eSkinItem_Hat];
	msg.dwData2 = HERO->GetShopItemStats()->wSkinItem[eSkinItem_Sticker];
	msg.dwData3 = HERO->GetShopItemStats()->wSkinItem[eSkinItem_Weapon];
	msg.dwData4 = HERO->GetShopItemStats()->wSkinItem[eSkinItem_Dress];
	msg.dwData5 = HERO->GetShopItemStats()->wSkinItem[eSkinItem_Shoulder];
	msg.dwData6 = HERO->GetShopItemStats()->wSkinItem[eSkinItem_Shoes];
	//memcpy(&msg.dwSkinData, HERO->GetShopItemStats()->wSkinItem, sizeof(DWORD)*eSkinItem_Max);
	NETWORK->Send(&msg, sizeof(MSG_DWORD7));
}
void cChangeLookDialog::test()
{
	//CItem* pItem = ITEMMGR->GetItem(26);
	/*pItem = ITEMMGR->MakeNewItem((ITEMBASE*)ITEMMGR->GetItem(26), "MP_ITEM_DIVIDE_ACK");
	m_pWearedDlg->AddItem(0, pItem);*/


	CDealItem* pItemIcon = NULL;
	ITEM_INFO * pInfo = ITEMMGR->GetItemInfo(8000);
	if (pInfo)
	{
		pItemIcon = new CDealItem;
		cImage tmpimage;
		pItemIcon->Init(0, 0, DEFAULT_ICONSIZE, DEFAULT_ICONSIZE, ITEMMGR->GetIconImage(8000, &tmpimage), IG_DEALITEM_START + 3500 + 1);
		pItemIcon->SetData(pInfo->ItemIdx);
		pItemIcon->SetItemIdx(pInfo->ItemIdx);
		pItemIcon->SetType(WT_CHANGELOOK_ITEM);

		//ITEMMGR->SetGoldShopToolTipIcon(pItemIcon);
		WINDOWMGR->AddWindow(pItemIcon);
		m_pWearedDlg->AddIcon(1, pItemIcon);
	}

	//m_pWearedDlg->AddItem(0,(CItem*)pItem);
	
	//pItem = ITEMMGR->GetItem(8000);
	//m_pWearedDlg->AddItem(CHANGE_LOOK_WEARED_DLG_EQUIP+1, pItem);
}
//int cChangeLookDialog::getPositionByType(int itemType)
//{
//	if (itemType == eSkinItem_Mask)
//	{
//		return 0;
//	}
//	else if (itemType == eSkinItem_Hat)
//	{
//		return 1;
//	}
//	else if (itemType == eSkinItem_Sticker)
//	{
//		return 2;
//	}
//	else if (itemType == eSkinItem_Weapon)
//	{
//		return 3;
//	}
//	else if (itemType == eSkinItem_Dress)
//	{
//		return 4;
//	}
//	else if (itemType == eSkinItem_Shoulder)
//	{
//		return 5;
//	}
//	else if (itemType == eSkinItem_Shoes)
//	{
//		return 6;
//	}
//}
//int cChangeLookDialog::getTypeByPosition(int itemPos)
//{
//	if (itemPos == 0)
//		return eSkinItem_Mask;
//	else if (itemPos == 1)
//		return eSkinItem_Hat;
//	else if (itemPos == 2)
//		return eSkinItem_Sticker;
//	else if (itemPos == 3)
//		return eSkinItem_Weapon;
//	else if (itemPos == 4)
//		return eSkinItem_Dress;
//	else if (itemPos == 5)
//		return eSkinItem_Shoulder;
//	else if (itemPos == 6)
//		return eSkinItem_Shoes;
//	else	
//		return 0;
//}
void cChangeLookDialog::tambahIcon(int type, int itemidx)
{
	int finalType = -1;
	int idIdx = 0;
	if (type == eSkinItem_Hat)
	{
		finalType = eSkinItem_Hat;
		idIdx = 1;
	}
	if (type == eSkinItem_Dress)
	{
		finalType = eSkinItem_Dress;
		idIdx = 2;
	}
	if (type == eSkinItem_Shoes)
	{
		finalType = eSkinItem_Shoes;
		idIdx = 3;
	}
	if (type == eSkinItem_Mask)
	{
		finalType = eSkinItem_Mask;
		idIdx = 4;
	}
	if (type == eSkinItem_Shoulder)
	{
		finalType = eSkinItem_Shoulder;
		idIdx = 5;
	}
	if (type == eSkinItem_Weapon)
	{
		finalType = eSkinItem_Weapon;
		idIdx = 6;
	}
	if (type == eSkinItem_Sticker)
	{
		finalType = eSkinItem_Sticker;
		idIdx = 7;
	}


	if (idIdx == 0)
	{
		return; // return failed
	}
	if (finalType == -1)
	{
		return; // return failed
	}

	CDealItem* pItemIcon = NULL;
	ITEM_INFO * pInfo = ITEMMGR->GetItemInfo(itemidx);
	if (pInfo)
	{
		pItemIcon = new CDealItem;
		cImage tmpimage;
		pItemIcon->Init(0, 0, DEFAULT_ICONSIZE, DEFAULT_ICONSIZE, ITEMMGR->GetIconImage(itemidx, &tmpimage), IG_DEALITEM_START + 3500 + idIdx);
		pItemIcon->SetData(pInfo->ItemIdx);
		pItemIcon->SetItemIdx(pInfo->ItemIdx);
		pItemIcon->SetType(WT_CHANGELOOK_ITEM);

		//ITEMMGR->SetGoldShopToolTipIcon(pItemIcon);
		WINDOWMGR->AddWindow(pItemIcon);
		m_pWearedDlg->AddIcon(finalType, pItemIcon);
	}
}
void cChangeLookDialog::Linking()
{
	p_Dlg = WINDOWMGR->GetWindowForID( CHANGE_LOOK_DLG );
	p_Grid = (cIconGridDialog*)GetWindowForID(CHANGE_LOOK_ICON_GRID_DLG_INVEN);
	m_pWearedDlg = (CWearedExDialog*)GetWindowForID(CHANGE_LOOK_WEARED_DLG_EQUIP);

	for (int i=0; i < 7; i++)
	{
		m_CheckIcon[i] = (cCheckBox*)GetWindowForID(CHANGE_LOOK_CHECK_BOX_MASK+i);
		//Init();
		//m_CheckIcon[i]->SetChecked(FALSE);
	}
	XX = p_Dlg->GetAbsX();
	YY = p_Dlg->GetAbsY();

	DisableAlIcon();

	DisableAllCheckBox();

//	m_pWearedDlg->SetDisableIcon(4, 4);
	//setData();
	//test();
}
void cChangeLookDialog::clearIconForOneItem(int pos)
{
	cIcon* pIcon;
	pIcon = m_pWearedDlg->GetIconForIdx(pos);
	if (pIcon != NULL)
	{
		m_pWearedDlg->DeleteIcon(pos, &pIcon);
		pIcon->SetActive(FALSE);
		WINDOWMGR->AddListDestroyWindow(pIcon);
		pIcon = NULL;
	}
}
void cChangeLookDialog::setData()
{
	//if (!IsActive()) return;
	cIcon* pIcon;

	for (int n = 0; n < 7; ++n)
	{
		pIcon = m_pWearedDlg->GetIconForIdx(n);
		if (pIcon != NULL)
		{
			m_pWearedDlg->DeleteIcon(n,&pIcon);
			pIcon->SetActive(FALSE);
			WINDOWMGR->AddListDestroyWindow(pIcon);
			pIcon = NULL;
		}

	}
	

	tambahIcon(eSkinItem_Hat, HERO->GetShopItemStats()->wSkinItem[eSkinItem_Hat]);
	tambahIcon(eSkinItem_Dress, HERO->GetShopItemStats()->wSkinItem[eSkinItem_Dress]);
	tambahIcon(eSkinItem_Shoes, HERO->GetShopItemStats()->wSkinItem[eSkinItem_Shoes]);
	tambahIcon(eSkinItem_Mask, HERO->GetShopItemStats()->wSkinItem[eSkinItem_Mask]);

	tambahIcon(eSkinItem_Weapon, HERO->GetShopItemStats()->wSkinItem[eSkinItem_Weapon]);
	tambahIcon(eSkinItem_Shoulder, HERO->GetShopItemStats()->wSkinItem[eSkinItem_Shoulder]);
	tambahIcon(eSkinItem_Sticker, HERO->GetShopItemStats()->wSkinItem[eSkinItem_Sticker]);
}
void cChangeLookDialog::Render()
{


	this->SetAbsXY((LONG)GAMEIN->GetInventoryDialog()->GetAbsX() + XX, (LONG)GAMEIN->GetInventoryDialog()->GetAbsY() + YY);

	//cDialog::Render();
	cDialog::RenderWindow();
	cDialog::RenderComponent();
}
/*void cChangeLookDialog::Init()
{
	ReadConfig();
	((cCheckBox*)GetWindowForID(CHANGE_LOOK_CHECK_BOX_WING))->SetChecked(m_IsAutoOpenBox);
}*/

void cChangeLookDialog::OnActionEvent(LONG lId, void * p, DWORD we)
{
	BOOL bAllow=false;
	if (we & WE_PUSHDOWN)
	{
	}
	else if (we & WE_PUSHUP)
	{
	}
	else if (we & WE_CHECKED)
	{
		if (lId == CHANGE_LOOK_CHECK_BOX_WING)
		{
			m_IsAutoOpenBox = ((cCheckBox*)GetWindowForID(CHANGE_LOOK_CHECK_BOX_WING))->IsChecked();
			bAllow = true;
		}
	}
	else if (we & WE_NOTCHECKED)
	{
		if (lId == CHANGE_LOOK_CHECK_BOX_WING)
		{
			m_IsAutoOpenBox = ((cCheckBox*)GetWindowForID(CHANGE_LOOK_CHECK_BOX_WING))->IsChecked();
			bAllow = true;
		}
	}
	if (bAllow)
	{
		if (HERO->GetState() == eObjectState_Move)
			MOVEMGR->HeroMoveStop();
		
//		WriteConfig();
		APPEARANCEMGR->InitAppearance(HERO);
	}
	
}
void cChangeLookDialog::ClearItemInfo()
{
	if (p_Dlg)
	{
		if (p_Grid)
		{
			cIcon* pIcon;

			for (int n = 0; n<p_Grid->m_nCol*p_Grid->m_nRow; ++n)
			{
				pIcon = p_Grid->GetIconForIdx(n);
				if (pIcon != NULL)
				{
					p_Grid->DeleteIcon(pIcon);
					pIcon->SetActive(FALSE);
					WINDOWMGR->AddListDestroyWindow(pIcon);
					pIcon = NULL;
				}

			}
		}
	}
}
void cChangeLookDialog::DisableAlIcon()
{
	if (p_Dlg)
	{
		if (p_Grid)
		{						
			p_Grid->SetDisableIcon(0,20);
			
		}
	}
}
void cChangeLookDialog::DisableAllCheckBox()
{
	if (p_Dlg)
	{
		for (int i = 0; i < 7; i++)
		{
			m_CheckIcon[i]->SetDisable(TRUE);
		}
	}
}

BOOL cChangeLookDialog::InPt(POSTYPE absPos)
{
	//jj
	return (TP_CHANGELOOKINVEN_START <= absPos&&absPos<TP_CHANGELOOKINVEN_END);
}
BOOL cChangeLookDialog::DeleteItem(POSTYPE Pos, CItem** ppItem)
{
	
	return DeleteIcon(Pos - (/*TP_SHOPINVEN_START + TABCELL_SHOPINVEN_NUM*/TP_CHANGELOOKINVEN_START), (cIcon **)ppItem);
}
BOOL cChangeLookDialog::FakeMoveIcon(LONG x, LONG y, cIcon * icon)
{
	//p_Grid->AddIcon(1,icon);
	//CHATMGR->AddMsg(CTC_SYSMSG,"Can't move");
	//return false;
	BOOL bSend = false;
	CItem* pFromItem = (CItem*)icon;
	if (pFromItem->GetItemKind() == eEQUIP_ITEM_DRESS)
	{
		clearIconForOneItem(eSkinItem_Dress);
		tambahIcon(eSkinItem_Dress, pFromItem->GetItemIdx());

		HERO->GetShopItemStats()->wSkinItem[eSkinItem_Dress] = pFromItem->GetItemIdx();

		bSend = true;
	}
	else if (pFromItem->GetItemKind() == eEQUIP_ITEM_WEAPON)
	{
		clearIconForOneItem(eSkinItem_Weapon);
		tambahIcon(eSkinItem_Weapon, pFromItem->GetItemIdx());

		HERO->GetShopItemStats()->wSkinItem[eSkinItem_Weapon] = pFromItem->GetItemIdx();

		bSend = true;
	}
	else if (pFromItem->GetItemKind() == eEQUIP_ITEM_SHOES)
	{
		clearIconForOneItem(eSkinItem_Shoes);
		tambahIcon(eSkinItem_Shoes, pFromItem->GetItemIdx());

		HERO->GetShopItemStats()->wSkinItem[eSkinItem_Shoes] = pFromItem->GetItemIdx();

		bSend = true;
	}
	else if (pFromItem->GetItemKind() == eEQUIP_ITEM_HAT)
	{
		clearIconForOneItem(eSkinItem_Hat);
		tambahIcon(eSkinItem_Hat, pFromItem->GetItemIdx());

		HERO->GetShopItemStats()->wSkinItem[eSkinItem_Hat] = pFromItem->GetItemIdx();

		bSend = true;
	}
	else if (pFromItem->GetItemKind() == eSHOP_ITEM_IMAGENAME)
	{
		clearIconForOneItem(eSkinItem_Sticker);
		tambahIcon(eSkinItem_Sticker, pFromItem->GetItemIdx());
		HERO->GetShopItemStats()->wSkinItem[eSkinItem_Sticker] = pFromItem->GetItemIdx();

		bSend = true;
	}
	else if (pFromItem->GetItemKind() == eSHOP_ITEM_DECORATION)
	{
		AVATARITEM* pAvatarUseInfo = GAMERESRCMNGR->m_AvatarEquipTable.GetData(pFromItem->GetItemIdx());
		if ((pAvatarUseInfo != NULL))
		{
			if (pAvatarUseInfo->Position == 3)
			{
				clearIconForOneItem(eSkinItem_Mask);
				tambahIcon(eSkinItem_Mask, pFromItem->GetItemIdx());
				HERO->GetShopItemStats()->wSkinItem[eSkinItem_Mask] = pFromItem->GetItemIdx();

				bSend = true;
			}
			else if (pAvatarUseInfo->Position == 7)
			{
				clearIconForOneItem(eSkinItem_Shoulder);
				tambahIcon(eSkinItem_Shoulder, pFromItem->GetItemIdx());
				HERO->GetShopItemStats()->wSkinItem[eSkinItem_Shoulder] = pFromItem->GetItemIdx();

				bSend = true;
			}
			else if (pAvatarUseInfo->Position == 6)
			{
				clearIconForOneItem(eSkinItem_Dress);
				tambahIcon(eSkinItem_Dress, pFromItem->GetItemIdx());
				HERO->GetShopItemStats()->wSkinItem[eSkinItem_Dress] = pFromItem->GetItemIdx();

				bSend = true;
			}
			else if (pAvatarUseInfo->Position == 0)
			{
				clearIconForOneItem(eSkinItem_Hat);
				tambahIcon(eSkinItem_Hat, pFromItem->GetItemIdx());

				HERO->GetShopItemStats()->wSkinItem[eSkinItem_Hat] = pFromItem->GetItemIdx();

				bSend = true;
			}
			else if (pAvatarUseInfo->Position == 17|| pAvatarUseInfo->Position == 18 || pAvatarUseInfo->Position == 19 || 
				pAvatarUseInfo->Position == 20 || pAvatarUseInfo->Position == 21 || pAvatarUseInfo->Position == 22 )
			{
				clearIconForOneItem(eSkinItem_Weapon);
				tambahIcon(eSkinItem_Weapon, pFromItem->GetItemIdx());

				HERO->GetShopItemStats()->wSkinItem[eSkinItem_Weapon] = pFromItem->GetItemIdx();

				bSend = true;
			}
		}
	}
	if(bSend == true)
	sendToServer();

	return GAMEIN->GetInventoryDialog()->FakeMoveIcon(x, y, icon);
}
BOOL cChangeLookDialog::AddItem(CItem* pItem)
{
	if (!pItem)		return FALSE;
	if (pItem->GetItemBaseInfo()->ItemParam & ITEM_PARAM_SEAL)
	{
		pItem->SetMovable(TRUE);
	}
	ITEMMGR->RefreshItem(pItem);
	ITEM_INFO* pItemInfo = ITEMMGR->GetItemInfo(pItem->GetItemBaseInfo()->wIconIdx);
	if (!pItemInfo)		return FALSE;
	if (pItemInfo->ItemKind == eSHOP_ITEM_MAKEUP || pItemInfo->ItemKind == eSHOP_ITEM_DECORATION)
	{
		SHOPITEMBASE* pUsedInfo = ITEMMGR->GetUsedItemInfo(pItem->GetItemBaseInfo()->wIconIdx);
		if (pUsedInfo)
			ITEMMGR->AddUsedAvatarItemToolTip(pUsedInfo);
	}
	else if (pItemInfo->ItemKind == eSHOP_ITEM_EQUIP)
	{
		SHOPITEMBASE* pUsedInfo = ITEMMGR->GetUsedItemInfo(pItem->GetItemBaseInfo()->wIconIdx);
		if (pUsedInfo)
			ITEMMGR->AddUsedShopEquipItemToolTip(pUsedInfo);
	}
	POSTYPE relPos = GetRelativePosition(pItem->GetPosition());
	return AddIcon(relPos, pItem);
}
POSTYPE cChangeLookDialog::GetRelativePosition(POSTYPE absPos)
{
	return (absPos - (TP_CHANGELOOKINVEN_START)) % (20);
}
BOOL cChangeLookDialog::FakeMoveItem(LONG MouseX, LONG MouseY, CItem* pSrcItem)
{
	
	WORD ToPos = 0;
	if (!GetPositionForXYRef(MouseX, MouseY, ToPos))
		return FALSE;
	ToPos = ToPos + TP_CHANGELOOKINVEN_START;// TP_SHOPINVEN_START + TABCELL_SHOPINVEN_NUM;
	CItem * pToItem = GetItemForPos(ToPos);
	if (pToItem && pToItem->IsLocked() == TRUE)
		return FALSE;
	return FakeGeneralItemMove(ToPos, pSrcItem, pToItem);
}
CItem* cChangeLookDialog::GetItemForPos(POSTYPE absPos)
{
	if (InPt(absPos))
	{
		return (CItem*)GetIconForIdx(absPos - (TP_CHANGELOOKINVEN_START/*TP_SHOPINVEN_START + TABCELL_SHOPINVEN_NUM*/));
	}
	return NULL;
}
BOOL cChangeLookDialog::FakeGeneralItemMove(POSTYPE ToPos, CItem * pFromItem, CItem * pToItem)
{
	
	if (CanBeMoved(pFromItem, ToPos) == FALSE)
		return FALSE;
	BYTE FromIdx = ITEMMGR->GetTableIdxForAbsPos(pFromItem->GetPosition());
	if (FromIdx >= eItemTable_Max) return FALSE;
	BYTE ToIdx = ITEMMGR->GetTableIdxForAbsPos(ToPos);
	if (ToIdx >= eItemTable_Max) return FALSE;
	if (pFromItem->GetPosition() == ToPos)
		return FALSE;
	if (pToItem && pToItem->GetUseParam() == 1)
	{
		if (FromIdx != ToIdx)
		{
			CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(748));
			return FALSE;
		}
	}
	if (pToItem && CanBeMoved(pToItem, pFromItem->GetPosition()) == FALSE)
	{
		return FALSE;
	}
	MSG_ITEM_MOVE_SYN msg;
	msg.Category = MP_ITEM;
	msg.Protocol = MP_ITEM_MOVE_SYN;
	msg.dwObjectID = HEROID;
	msg.FromPos = pFromItem->GetPosition();
	msg.wFromItemIdx = pFromItem->GetItemIdx();
	msg.ToPos = ToPos;
	msg.wToItemIdx = (pToItem ? pToItem->GetItemIdx() : 0);
	NETWORK->Send(&msg, sizeof(msg));
	return TRUE;
}
BOOL cChangeLookDialog::CanBeMoved(CItem* pItem, POSTYPE pos)
{
	ASSERT(pItem);
	if (TP_CHANGELOOKINVEN_START <= pos && pos < TP_CHANGELOOKINVEN_END)
	{
		POSTYPE fromPos = pItem->GetPosition();
		if (TP_CHANGELOOKINVEN_START <= fromPos && fromPos < TP_CHANGELOOKINVEN_END)
			return TRUE;
		else
			return FALSE;
	}
	else
		return FALSE;
}
BOOL cChangeLookDialog::GetBlankPositionRestrictRef(WORD& absPos)
{
	for (int j = 0; j < GetCellNum(); ++j)
	{
		if (IsAddable(j))
		{
			absPos = TP_CHANGELOOKINVEN_START + j;// TP_SHOPINVEN_START + j + TABCELL_SHOPINVEN_NUM;
			return TRUE;
		}
	}
	return FALSE;
}
BOOL cChangeLookDialog::GetBlankNum()
{
	int nBlankNum = 0;
	for (int i = 0; i < GetCellNum(); ++i)
	{
		if (IsAddable(i))
		{
			++nBlankNum;
		}
	}
	return nBlankNum;
}