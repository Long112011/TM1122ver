#include "Stdafx.h"
#include "ItemDropInfoDialog.h"
#include "INTERFACE\cStatic.h"
#include "INTERFACE\cIconDialog.h"
#include "INTERFACE\cIconGridDialog.h"
#include "WindowIDEnum.h"
#include "ItemManager.h"
#include "cScriptManager.h"
#include "DealItem.h"
#include "cWindowManager.h"
#include "ItemDropManager.h"
#include "gamein.h"
#include "chatmanager.h"
#include "cFont.h"
#include "ItemDropIcon.h"
#include "InventoryExDialog.h"
GLOBALTON(CItemDropInfoDialog)
extern char JACKJACK[64];
CItemDropInfoDialog::CItemDropInfoDialog()
{
	m_type = WT_ICONGRIDDIALOG;
	m_nIconType = WT_ITEM;
	ZeroMemory(m_pMonsterName, sizeof(m_pMonsterName));
}
CItemDropInfoDialog::~CItemDropInfoDialog()
{
	ClearIconGrid();
}
void CItemDropInfoDialog::SetActive(BOOL val)
{
	if (m_bDisable) return;

	if (!val)
		ClearIconGrid();
	cDialog::SetActiveRecursive(val);
}
void CItemDropInfoDialog::Linking()
{
	pDlg = WINDOWMGR->GetWindowForID(ITEMDROP_DLG);
	m_pTitle = (cStatic *)GetWindowForID(ITEMDROP_DLG_TITLE);
	m_pItemGridDlgEasy = (cIconGridDialog *)GetWindowForID(ITEMDROP_ICONGRID_EASY);
	m_pItemGridDlgMedium = (cIconGridDialog *)GetWindowForID(ITEMDROP_ICONGRID_MEDIUM);
	m_pItemGridDlgHard = (cIconGridDialog *)GetWindowForID(ITEMDROP_ICONGRID_HARD);

	for (int i = 0; i < 3; i++)
	{
		m_pStatic[i] = (cStatic *)GetWindowForID(ITEMDROP_STATIC_EASY + i);
	}
}

void CItemDropInfoDialog::AddItemToGridEasy()
{	
	cImage tmpImage;
	cImage imgToolTip;
	cImage imgHeader;
	SCRIPTMGR->GetImage(25, &imgHeader, PFT_JACKPATH );
	SCRIPTMGR->GetImage(26, &imgToolTip, PFT_JACKPATH );
	CDropItem * pIcon = NULL;
	char buf[256] = { 0, };
	ClearIconGrid();

	//  放在循环外初始化
	int easyIdx = 0;
	int mediumIdx = 0;
	int hardIdx = 0;



	for (int i = 1; i <= ITEMDROPMGR->GetItemTempNum(); i++)
	{		
		MONSTER_DROP_TEMP* pItem = (MONSTER_DROP_TEMP*)ITEMDROPMGR->GetItemTempInfo(i);
		ITEM_INFO* pInfo = ITEMMGR->GetItemInfo(pItem->wItemIdx);
		if (pInfo)
		{
			pIcon = new CDropItem;

			cImageScale scale;
			scale.x = 0.5;
			scale.y = 0.5;
			pIcon->SetScale(&scale);

			pIcon->Init(0, 0, 220, 20, ITEMMGR->GetIconImage(pInfo->ItemIdx, &tmpImage), IG_ITEMDROPINFO_START + i);
			pIcon->SetData(pInfo->ItemIdx);
			pIcon->SetMovable(FALSE);
			pIcon->SetHeaderImage(&imgHeader);
			pIcon->SetToolTip("", RGBA_MAKE(255, 255, 255, 255), &imgToolTip, TTCLR_ITEM_CANEQUIP);
			pIcon->SetToolModel(1);

			pIcon->SetItemName(pInfo->ItemName);

			std::string s = std::to_string(pItem->wPercent);
			char* c = strcpy(new char[s.length() + 1], s.c_str());
			strcat(c,"%");

			pIcon->SetItemPercent(c);
			
			cImage * imgEquipInfo;
			imgEquipInfo = pIcon->GetBasicImage();
			if (imgEquipInfo)
			{
				pIcon->AddToolTipImageEquip(imgEquipInfo);
			}
			else
			{
				pIcon->AddToolTipLine("");
			}

			char line[128];
			wsprintf(line, "[%s]", pInfo->ItemName);
			pIcon->AddToolTipLine(line, TTTC_DEFAULT, NULL, 6);
			pIcon->AddToolTipLine(JACKJACK);
			ITEMMGR->SetExtraItemToolTip((cIcon*)pIcon, pInfo);
			pIcon->AddToolTipLine("");


		/*	int b;
			int a;
			if (pItem->wRate == 0)
			{
				m_pItemGridDlgEasy->AddIcon(i - 1, pIcon);
				b = i;
			}
			if (pItem->wRate == 1)
			{
				m_pItemGridDlgMedium->AddIcon(i - (1 + b), pIcon);
				a = i;
			}
			if (pItem->wRate == 2)
			{
				m_pItemGridDlgHard->AddIcon(i - (1 + a), pIcon);
			}*/
			// 用下面这段：
			if (pItem->wRate == 0)
			{
				m_pItemGridDlgEasy->AddIcon(easyIdx++, pIcon);
			}
			else if (pItem->wRate == 1)
			{
				m_pItemGridDlgMedium->AddIcon(mediumIdx++, pIcon);
			}
			else if (pItem->wRate == 2)
			{
				m_pItemGridDlgHard->AddIcon(hardIdx++, pIcon);
			}

		}
		pIcon = NULL;
		pItem = NULL;
		pInfo = NULL;
	}
	m_pTitle->SetStaticText(m_pMonsterName);
	for (int i = 0; i < 3; i++)
	{
		std::string b = std::to_string(ITEMDROPMGR->GetCalcRate(i));
		char* a = strcpy(new char[b.length() + 1], b.c_str());
		strcat(a, "%");

		m_pStatic[i]->SetStaticText(a);
	}
	SetActive(true);
}

void CItemDropInfoDialog::OnActionEvent(LONG lId, void * p, DWORD we)
{
	if (we & WE_CLOSEWINDOW)
	{
		SetActive(false);
	}
}
void CItemDropInfoDialog::ClearIconGrid()
{
	if (m_pItemGridDlgEasy)
	{
		for (int n = 0; n < 20; ++n)
		{
			cIcon* pIcon = m_pItemGridDlgEasy->GetIconForIdx(n);
			if (pIcon != NULL)
			{
				m_pItemGridDlgEasy->DeleteIcon(pIcon);
				pIcon->SetActive(FALSE);
				WINDOWMGR->AddListDestroyWindow(pIcon);
				pIcon = NULL;
			}
		}
	}
	if (m_pItemGridDlgMedium)
	{
		for (int n = 0; n < 20; ++n)
		{
			cIcon* pIcon = m_pItemGridDlgMedium->GetIconForIdx(n);
			if (pIcon != NULL)
			{
				m_pItemGridDlgMedium->DeleteIcon(pIcon);
				pIcon->SetActive(FALSE);
				WINDOWMGR->AddListDestroyWindow(pIcon);
				pIcon = NULL;
			}
		}
	}
	if (m_pItemGridDlgHard)
	{
		for (int n = 0; n < 20; ++n)
		{
			cIcon* pIcon = m_pItemGridDlgHard->GetIconForIdx(n);
			if (pIcon != NULL)
			{
				m_pItemGridDlgHard->DeleteIcon(pIcon);
				pIcon->SetActive(FALSE);
				WINDOWMGR->AddListDestroyWindow(pIcon);
				pIcon = NULL;
			}
		}
	}
}