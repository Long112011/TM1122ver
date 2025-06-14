#include "Stdafx.h"
#include "GoldShopBuyDialog.h"
#include "GoldShopDialog.h"
#include "WindowIDEnum.h"
#include "./Interface/cWindowManager.h" 
#include "ItemManager.h"
#include "GameIn.h"
#include "./input/UserInput.h"
extern int bTempBoxT;
CGoldShopBuyDialog::CGoldShopBuyDialog()
{
	pDlg = NULL;
	m_pItemName = NULL;
	m_pItemName2 = NULL;
	m_pItemPrice = NULL;
	m_pItemPriceOffer = NULL;
	m_BaseImage = NULL;
	m_GoldImage = NULL;
	m_MallImage = NULL;
	m_ItemIdx = 0;
	m_Key = 0;
	m_Count = 1;
	memset(m_pButton, 0, sizeof(cButton*)* 2);
}
CGoldShopBuyDialog::~CGoldShopBuyDialog()
{
	pDlg = NULL;
	m_pItemName = NULL;
	m_pItemName2 = NULL;
	m_pItemPrice = NULL;
	m_pItemPriceOffer = NULL;
	m_BaseImage = NULL;
	m_GoldImage = NULL;
	m_MallImage = NULL;
	m_ItemIdx = 0;
	m_Key = 0;
	m_Count = 1;
	memset(m_pButton, 0, sizeof(cButton*)* 2);
}
void CGoldShopBuyDialog::Linking()
{
	pDlg = WINDOWMGR->GetWindowForID(SHOP_BUY_DLG);
	m_pItemName = (cStatic*)GetWindowForID(SHOP_BUY_NAME);
	m_pItemName2 = (cStatic*)GetWindowForID(SHOP_BUY_NAME2);
	m_pItemPrice = (cStatic*)GetWindowForID(SHOP_BUY_PRICE);
	m_pItemPriceOffer = (cStatic*)GetWindowForID(SHOP_BUY_OFFER);
	m_BaseImage = (cStatic*)GetWindowForID(SHOP_BUY_ITEM);
	m_GoldImage = (cStatic*)GetWindowForID(SHOP_BUY_GOLD);
	m_MallImage = (cStatic*)GetWindowForID(SHOP_BUY_MALL);
	m_pButton[0] = (cButton*)GetWindowForID(SHOP_BUY_OK);
	m_pButton[1] = (cButton*)GetWindowForID(SHOP_BUY_EXIT);
	pDlg->SetTopOnActive(TRUE);
	pDlg->SetAlwaysTop(TRUE);
	m_pItemPrice->SetFGColor(RGBA_MERGE(0xFFFFFF, m_alpha * m_dwOptionAlpha / 100));
}
void CGoldShopBuyDialog::Init(ITEM_GOLD_SHOP * pShopItem)
{
#ifndef _OLDGOLDDIALOG_
	if (pShopItem)
	{
		ITEM_INFO * pItem = ITEMMGR->GetItemInfo(pShopItem->ItemIdx);
		if (pItem)
		{
			char Pricing[16] = { 0, };
			m_ItemIdx = pShopItem->ItemIdx;
			m_Key = pShopItem->Key;
			cImage itemImage;
			ITEMMGR->GetIconImage(pItem->ItemIdx, &itemImage);
			m_BaseImage->SetBasicImage(&itemImage);
			m_pItemName->SetStaticText(pItem->ItemName);
			m_pItemName2->SetStaticText(pShopItem->ItemName);
			if (pShopItem->ItemBuyType == 1)
			{
				m_GoldImage->SetActive(FALSE);
				m_MallImage->SetActive(TRUE);
				if (pShopItem->ItemOfferPrice != 0)
				{
					m_pItemPriceOffer->SetActive(TRUE);
					strcpy(Pricing, AddComma(pShopItem->ItemOfferPrice*GetCount()));
					m_pItemPriceOffer->SetStaticText(Pricing);
					strcpy(Pricing, AddComma(pShopItem->ItemBuyPrice*GetCount()));
					m_pItemPrice->SetStaticText(Pricing);
					m_pItemPrice->SetFGColor(RGBA_MERGE(0xFF0000, m_alpha * m_dwOptionAlpha / 100));
					m_pItemPrice->SetShadowColor(RGBA_MERGE(0x000000, m_alpha * m_dwOptionAlpha / 100));
				}
				else
				{
					m_pItemPriceOffer->SetActive(FALSE);
					strcpy(Pricing, AddComma(pShopItem->ItemBuyPrice*GetCount()));
					m_pItemPrice->SetStaticText(Pricing);
					m_pItemPrice->SetFGColor(RGBA_MERGE(0xFFFFFF, m_alpha * m_dwOptionAlpha / 100));
					m_pItemPrice->SetShadowColor(RGBA_MERGE(0x000000, m_alpha * m_dwOptionAlpha / 100));
				}
			}
			else if (pShopItem->ItemBuyType == 2)
			{
				m_GoldImage->SetActive(TRUE);
				m_MallImage->SetActive(FALSE);
				if (pShopItem->ItemOfferPrice != 0)
				{
					m_pItemPriceOffer->SetActive(TRUE);
					strcpy(Pricing, AddComma(pShopItem->ItemOfferPrice*GetCount()));
					m_pItemPriceOffer->SetStaticText(Pricing);
					strcpy(Pricing, AddComma(pShopItem->ItemBuyPrice*GetCount()));
					m_pItemPrice->SetStaticText(Pricing);
					m_pItemPrice->SetFGColor(RGBA_MERGE(0xFF0000, m_alpha * m_dwOptionAlpha / 100));
					m_pItemPrice->SetShadowColor(RGBA_MERGE(0x000000, m_alpha * m_dwOptionAlpha / 100));
				}
				else
				{
					m_pItemPriceOffer->SetActive(FALSE);
					strcpy(Pricing, AddComma(pShopItem->ItemBuyPrice*GetCount()));
					m_pItemPrice->SetStaticText(Pricing);
					m_pItemPrice->SetFGColor(RGBA_MERGE(0xFFFFFF, m_alpha * m_dwOptionAlpha / 100));
					m_pItemPrice->SetShadowColor(RGBA_MERGE(0x000000, m_alpha * m_dwOptionAlpha / 100));
				}
			}
			else if (pShopItem->ItemBuyType == 3)
			{
				m_GoldImage->SetActive(FALSE);
				m_MallImage->SetActive(FALSE);
				if (pShopItem->ItemOfferPrice != 0)
				{
					m_pItemPriceOffer->SetActive(TRUE);
					strcpy(Pricing, AddComma(pShopItem->ItemOfferPrice*GetCount()));
					m_pItemPriceOffer->SetStaticText(Pricing);
					strcpy(Pricing, AddComma(pShopItem->ItemBuyPrice*GetCount()));
					m_pItemPrice->SetStaticText(Pricing);
					m_pItemPrice->SetFGColor(RGBA_MERGE(0xFF0000, m_alpha * m_dwOptionAlpha / 100));
					m_pItemPrice->SetShadowColor(RGBA_MERGE(0x000000, m_alpha * m_dwOptionAlpha / 100));
				}
				else
				{
					m_pItemPriceOffer->SetActive(FALSE);
					strcpy(Pricing, AddComma(pShopItem->ItemBuyPrice*GetCount()));
					m_pItemPrice->SetStaticText(Pricing);
					m_pItemPrice->SetFGColor(RGBA_MERGE(0xFFFFFF, m_alpha * m_dwOptionAlpha / 100));
					m_pItemPrice->SetShadowColor(RGBA_MERGE(0x000000, m_alpha * m_dwOptionAlpha / 100));
				}
			}
		}
	}
#endif
}
extern HWND _g_hWnd;
void CGoldShopBuyDialog::SetActive(BOOL val)
{
	if (val)
	{
		bTempBoxT = bTempBoxT + 1;
		POINT point = { LONG(m_pButton[0]->GetAbsX()) + 20, LONG(m_pButton[0]->GetAbsY()) + 10 };
		ClientToScreen(
			_g_hWnd,
			&point);
		SetCursorPos(point.x, point.y);
	}
	cDialog::SetActive(val);
	GAMEIN->GetGoldShopDialog()->SetDisable(val);
	ITEMMGR->SetDisableDialog(val, eItemTable_Inventory);
	ITEMMGR->SetDisableDialog(val, eItemTable_Pyoguk);
	ITEMMGR->SetDisableDialog(val, eItemTable_GuildWarehouse);
	ITEMMGR->SetDisableDialog(val, eItemTable_PetInven);
}
DWORD CGoldShopBuyDialog::ActionKeyboardEvent(CKeyboard * keyInfo)
{
	if (!m_bActive) return WE_NULL;
	if (keyInfo->GetKeyDown(KEY_RETURN) || keyInfo->GetKeyDown(KEY_PADENTER))
	{
		BuyThis();
	}
	else if (keyInfo->GetKeyDown(KEY_ESCAPE))
	{
		BuyThis(false);
	}
}
void CGoldShopBuyDialog::OnActionEvent(LONG lId, void * p, DWORD we)
{
	if (we & WE_BTNCLICK)
	{
		if (lId == SHOP_BUY_OK)
		{
			BuyThis();
		}
		else if (lId == SHOP_BUY_EXIT)
		{
			BuyThis(false);
		}
	}
}
void CGoldShopBuyDialog::BuyThis(BOOL bFlag)
{
#ifndef _OLDGOLDDIALOG_
	if (bFlag)
	{
		ITEM_GOLD_SHOP * pShopItem = GAMEIN->GetGoldShopDialog()->GetGoldShopItemByKey(m_Key);
		if (pShopItem)
		{
			ITEM_INFO* pItemInfo = ITEMMGR->GetItemInfo(pShopItem->ItemIdx);
			if (pItemInfo)
			{
				MSG_ITEM_BUY_SYN sMsg;
				sMsg.wDealerIdx = MALLSHOPIDX;
				sMsg.BuyItemNum = GetCount();
				sMsg.wBuyItemIdx = pShopItem->ItemIdx;
				sMsg.wIdxPos = pShopItem->Key;
				sMsg.wBuyType = pShopItem->ItemBuyType;
				sMsg.wLimit = pShopItem->ItemLimit;
				sMsg.BuyRight = pShopItem->BuyRight;
				sMsg.LimitKind = pShopItem->LimitKind;
				sMsg.LimitCount = pShopItem->LimitCount;
				sMsg.Category = MP_ITEM;
				sMsg.Protocol = MP_ITEM_BUY_SYN;
				sMsg.dwObjectID = HEROID;
				NETWORK->Send(&sMsg, sMsg.GetSize());
			}
		}
		SetActive(FALSE);
	}
	else
	{
		SetActive(FALSE);
		ITEMMGR->SetDisableDialog(FALSE, eItemTable_Inventory);
		ITEMMGR->SetDisableDialog(FALSE, eItemTable_Pyoguk);
		ITEMMGR->SetDisableDialog(FALSE, eItemTable_GuildWarehouse);
		ITEMMGR->SetDisableDialog(FALSE, eItemTable_PetInven);
	}
#endif
}