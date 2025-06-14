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

	DisableIcon();
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
#include "OPini.h"
#define  _Test_Config_ "./Ini/OpenBox_Config.ini"
void cChangeLookDialog::WriteConfig()
{
	/*char stringtoread[2];
	memset(stringtoread, 0, 2);
	sprintf(stringtoread, "%d", m_IsAutoOpenBox);
	COPini::WriteString("OpenBox", "IsAutoOpenBox", stringtoread, _Test_Config_);*/
}
void cChangeLookDialog::ReadConfig()
{
	/*char stringtoread[2];
	memset(stringtoread, 0, 2);
	COPini::ReadString("OpenBox", "IsAutoOpenBox", stringtoread, _Test_Config_);
	m_IsAutoOpenBox = atoi(stringtoread);*/
}
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
		
		WriteConfig();
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
void cChangeLookDialog::DisableIcon()
{
	if (p_Dlg)
	{
		if (p_Grid)
		{						
			p_Grid->SetDisableIcon(0,20);
			
		}
	}
}


BOOL cChangeLookDialog::InPt(POSTYPE absPos)
{
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