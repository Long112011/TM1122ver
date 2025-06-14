#pragma once
#ifndef _OLDGOLDDIALOG_
#include "./Interface/cDialog.h"
#include "DealItem.h"
#include "./Interface/cIconGridDialog.h"
#include "..\[CC]Header\GameResourceStruct.h"
#define GOLDITEM_PAGE_COUNT	15
#define GOLDSHOPDLG GAMEIN->GetGoldShopDialog()
void BuyFunc(LONG lId, void * p, DWORD we);
class CMHFile;
class cPushupButton;
class CGoldShopDialog : public cDialog
{
public:
	void makeText();
	cDialog * m_Dlg;
	cIconGridDialog * m_IconDlg;
	cStatic * GoldMoney;
	cStatic *MallMoneyOne;
	cStatic *MallMoney;
	MSG_ITEM_BUY_SYN			m_buyMsg;
	CYHHashTable<ITEM_GOLD_SHOP>    m_GoldItemList;
	CGoldShopDialog();
	virtual ~CGoldShopDialog();
	virtual DWORD ActionKeyboardEvent(CKeyboard * keyInfo);

	void SetGoldItemInfo(int tab, int page);
	void OnActionEvent(LONG lId, void * p, DWORD we);
	void Linking();
	void SetGoldMoney(long Val);
	void SetMallMoney(long Val);
	void ShowDealer();
	void HideDealer();
	BOOL IsShopOpen();
	VOID SetShopOpen(BOOL bVal);
	ITEM_GOLD_SHOP * GetGoldShopItemByKey(DWORD Key);
	ITEM_GOLD_SHOP * GetGoldShopItemByItemIdx(DWORD ItemIdx);
	ITEM_GOLD_SHOP * GetGoldShopItemByTableIdx(int Tab, int Pos);
	ITEM_GOLD_SHOP * GetGoldShopItemByIdx(DWORD ItemIdx);
	int GetGoldShopItemCountByTableIdx(int Tab);

	virtual BOOL FakeMoveIcon(LONG x, LONG y, cIcon * icon);
	void SendGoldGetMsg();
	void ClearIconGrid();
	void SetPageStatic(WORD count);
	void SetLastPushedButton(BYTE Tab, BYTE Page){ m_LastTabPush = Tab; m_LastPagePush = Page; }
	
	

	void CallBin();
	void LoadBin(CMHFile& file, DWORD TabNum = 0);
	SEND_MALLLIST m_mall_list;
	void SendSelectRequest(DWORD Type);
	BOOL CheckShopItemDate(DWORD dwTime);
	void Process();
protected:
	BOOL		m_fShow;
	DWORD		m_CurSelectedItemIdx;
	LONG		m_lCurSelItemPos;
	WORD		m_DealerIdx;
	DWORD		m_wIdx;
	BOOL		m_bShopIsOpen;
	int         m_TableIdx;
	int			m_PageIdx[10];
	BYTE		m_LastTabPush;
	BYTE		m_LastPagePush;
	WORD		m_CheckPush;
	cPushupButton*	m_pTabPushBtn[10];
	cPushupButton*	m_pPagePushBtn[10];
	DWORD		dwTimeTick;
	DWORD		dwTempItemIdx[100];
};
EXTERNGLOBALTON(CGoldShopDialog)
#else
#pragma once
#pragma once
#include "./Interface/cTabDialog.h"
#include "DealItem.h"
#include <vector>
using std::vector;
class CGoldShopDialog : public cTabDialog
{
public:
	cDialog * pDlg;
	cStatic *MallMoneyOne;
	cStatic *MallMoney;
	cStatic *Money;

	cButton * buyBtn;

	MSG_ITEM_BUY_SYN			m_buyMsg;
	CGoldShopDialog();
	virtual ~CGoldShopDialog();
	virtual void Add(cWindow * window);
	void OnActionEvent(LONG lId, void * p, DWORD we);
	void Linking();
	void FakeBuyItem(LONG x, LONG y, CDealItem* pItem, BYTE buytype);
	virtual BOOL FakeMoveIcon(LONG x, LONG y, cIcon * icon);
	static void OnFakeBuyItem(LONG iId, void* p, DWORD param1, void * vData1, void* vData2);
	static void OnCancelBuyItem(LONG iId, void* p, DWORD param1, void * vData1, void* vData2);
	void SetGoldMoney(long Val);
	void SetMallMoney(long Val);
	void SetMoney(long Val);
	void ShowDealer();
	void HideDealer();
	void WindowClose();
	void OnSelectedItem();
	BOOL IsShopOpen();
	VOID SetShopOpen(BOOL bVal);
protected:
	BOOL						m_fShow;
	DWORD						m_wTabIdx;
	DWORD						m_CurSelectedItemIdx;
	LONG						m_lCurSelItemPos;
	DWORD						m_DealerIdx;
	DWORD						m_wIdx;
	BOOL						m_bShopIsOpen;
};

#endif