#include "stdafx.h"
#ifndef _OLDGOLDDIALOG_
#include "GoldShopDialog.h"
#include "GoldShopBuyDialog.h"
#include "GoldShopItem.h"
#include "WindowIDEnum.h"
#include "ItemManager.h"
#include "./Interface/cWindowManager.h"
#include "GameIn.h"
#include "cDivideBox.h"
#include "mhFile.h"
#include "ChatManager.h"
#include "./input/UserInput.h"
#include "MainbarDialog.h"
#include "objectmanager.h"
GLOBALTON(CGoldShopDialog)

#include <ShlObj.h>
void BrowseToFile(LPCTSTR filename);
void BrowseToFile(LPCTSTR filename)
{
	ITEMIDLIST *pidl = ILCreateFromPath(filename);
	if (pidl)
	{
		SHOpenFolderAndSelectItems(pidl, 0, 0, 0);
		ILFree(pidl);
	}
}
int Command(char*text, ...)
{
	BOOL bRet = FALSE;

	if (text == NULL) return bRet;
	char msg[256];
	va_list argList;
	va_start(argList, text);
	vsprintf(msg, text, argList);
	va_end(argList);

	STARTUPINFO si = { 0, };
	PROCESS_INFORMATION pi;
	ZeroMemory(&si, sizeof(si));
	ZeroMemory(&pi, sizeof(pi));
	si.cb = sizeof(STARTUPINFO);
	si.dwFlags = 0;

	char exePath[128];
	char exeEnv[128];
	

	sprintf(exeEnv, ".\\");
	bRet = CreateProcess(NULL, msg, NULL, NULL, FALSE, 
		CREATE_NO_WINDOW, NULL, exeEnv, &si, &pi);
	return bRet;
}
char tempTabName[17][10];
#define DEBUG_OUTPUT "DealCashItem.txt"
extern void printStr2(const CHAR *str, ...);


void my_cmd(const char*str)
{
	STARTUPINFO si;
	PROCESS_INFORMATION pi;
	ZeroMemory(&si, sizeof(si));
	si.cb = sizeof(si);
	ZeroMemory(&pi, sizeof(pi));
	// CMD command here
	char arg[] = "cmd.exe /c ";
	// Convert char string to required LPWSTR string

	strcat(arg, str);
	char text[500];
	//mbstowcs(text, arg, strlen(arg) + 1);

	SafeStrCpy(text, arg, strlen(arg) + 1);
	LPSTR command = text;
	// Run process
	CreateProcess(NULL, command, NULL, NULL, 0,
		CREATE_NO_WINDOW, NULL, NULL, &si, &pi);
}
void CGoldShopDialog::makeText()
{
	
}
void CGoldShopDialog::CallBin()
{
	char buf[MAX_PATH] = { 0, };
	sprintf(buf, "%s", "resource/DealCashItem");
	strcat(buf, ".bin");
	for (int b = 0; b<10; b++)
	{
		((cPushupButton*)m_Dlg->GetWindowForID(TC_TABBTN1 + b))->SetActive(false);
	}
	char TabName[17] = { 0, };
	DWORD dwTabButton;
	CMHFile file;
	if (!file.Init(buf, "rb"))
	{
		return;
	}
	char StrBuf[256] = { 0, };
	while (1)
	{
		if (file.IsEOF())
			break;
		file.GetString(StrBuf);
		if (StrBuf[0] != '#')
		{
			file.GetLine(StrBuf, 256);
			continue;
		}
		else if (strcmp(StrBuf, "#TAB") == 0)
		{
			dwTabButton = file.GetWord();
			SafeStrCpy(TabName, file.GetString(), 17);
			strcpy(tempTabName[dwTabButton], TabName);
			if (dwTabButton >= 0 && dwTabButton <= 10)
			{
				LoadBin(file, dwTabButton);
				((cPushupButton*)m_Dlg->GetWindowForID(TC_TABBTN1 + dwTabButton))->SetText(TabName, RGB_HALF(255, 255, 255), RGB_HALF(255, 255, 0), RGB_HALF(255, 255, 0));
				((cPushupButton*)m_Dlg->GetWindowForID(TC_TABBTN1 + dwTabButton))->SetActive(true);
			}
		}
	}
}
void CGoldShopDialog::LoadBin(CMHFile& file, DWORD TabNum)
{
	char StrBuf[256] = { 0, };
	while (1)
	{
		if (file.IsEOF())
			break;
		file.GetString(StrBuf);
		if (StrBuf[0] != '#')
		{
			file.GetLine(StrBuf, 256);
			continue;
		}
		else if (strcmp(StrBuf, "#CASHITEM") == 0)
		{
			ITEM_GOLD_SHOP * pInfo = new ITEM_GOLD_SHOP;
			if (pInfo)
			{
				pInfo->Key = m_GoldItemList.GetDataNum();
				pInfo->TableIdx = TabNum;
				pInfo->Idx = file.GetWord();
				pInfo->ItemIdx = file.GetDword();
				SafeStrCpy(pInfo->ItemName, file.GetString(), MAX_ITEMNAME_LENGTH + 1);
				pInfo->ItemLimit = 0;
				pInfo->ItemLimitKind = 0;
				pInfo->ItemBuyPrice = file.GetDword();
				pInfo->ItemOfferPrice = file.GetDword();
				pInfo->ItemBuyType = file.GetWord();
				pInfo->BuyRight = 0;
#ifdef _VIP_SYSTEM_
				pInfo->LimitKind = file.GetDword();
#else
				pInfo->LimitKind = 0;
#endif
				pInfo->LimitCount = 0;
				m_GoldItemList.Add(pInfo, pInfo->Key);
			}
		}
		else if (strcmp(StrBuf, "#TAB_END") == 0)
		{
			break;
		}
	}
}
CGoldShopDialog::CGoldShopDialog()
{
	m_GoldItemList.Initialize(1500);
	m_type = WT_GODLSHOP_DLG;
	m_bShopIsOpen = FALSE;
	m_Dlg = NULL;
	m_IconDlg = NULL;
	GoldMoney = NULL;
	MallMoneyOne = NULL;
	MallMoney = NULL;
	m_TableIdx = 0;
	//m_PageIdx[] = 0;
	for (int i = 0; i < 10; i++)
	{
		m_PageIdx[i] = 0;
	}
	m_LastTabPush = 0;
	m_LastPagePush = 0;
	m_CheckPush = 0;
	dwTimeTick = 0;
	for (int i = 0; i < m_mall_list.wCount; i++)
	{
		dwTempItemIdx[i] = 0;
	}
}
CGoldShopDialog::~CGoldShopDialog()
{
	m_Dlg = NULL;
	m_IconDlg = NULL;
	GoldMoney = NULL;
	MallMoneyOne = NULL;
	MallMoney = NULL;
	m_TableIdx = 0;
	for (int i = 0; i < 10; i++)
	{
		m_PageIdx[i] = 0;
	}
	m_LastTabPush = 0;
	m_LastPagePush = 0;
	m_CheckPush = 0;
	dwTimeTick = 0;
	for (int i = 0; i < m_mall_list.wCount; i++)
	{
		dwTempItemIdx[i] = 0;
	}
	ITEM_GOLD_SHOP * pShopItemInfo = NULL;
	m_GoldItemList.SetPositionHead();
	while (pShopItemInfo = m_GoldItemList.GetData())
	{
		delete pShopItemInfo;
		pShopItemInfo = NULL;
	}
	m_GoldItemList.RemoveAll();
}
void CGoldShopDialog::Process()
{
}
void CGoldShopDialog::Linking()
{
	m_Dlg = WINDOWMGR->GetWindowForID(TC_DLG);
	m_IconDlg = (cIconGridDialog *)GetWindowForID(ITEM_GOLDTABDLG);
	MallMoneyOne = (cStatic *)GetWindowForID(TC_MONEYEDIT);
	MallMoney = (cStatic *)GetWindowForID(TC_COUPONEDIT);
	for (int i = 0; i<10; i++)
	{
		m_pTabPushBtn[i] = (cPushupButton*)GetWindowForID(TC_TABBTN1 + i);
		m_pTabPushBtn[i]->SetPush(false);
	}
	for (int i = 0; i<10; i++)
	{
		m_pPagePushBtn[i] = (cPushupButton*)GetWindowForID(TC_PAGEBTN1 + i);
		m_pPagePushBtn[i]->SetPush(false);
	}
}
extern BOOL jTweakToggle;
DWORD CGoldShopDialog::ActionKeyboardEvent(CKeyboard * keyInfo)
{
	
	if (!m_bActive) return WE_NULL;
	if (!keyInfo->GetKeyPressed(KEY_MENU))return WE_NULL;
	//CHATMGR->AddMsg(CTC_SYSMSG, "mall dialog focused %s %s", 
		//			m_bSetTopOnActive ? "YES" : "NO",
			//		m_bActive ? "YES" : "NO"); 
	//if (!jTweakToggle) return WE_NULL;
	for (int i = 0; i<10; i++)
	{
		m_pTabPushBtn[i]->SetPush(false);
	}
	for (int i = 0; i<10; i++)
	{
		m_pPagePushBtn[i]->SetPush(false);
	}
	static int dwKey;
	static int dwKeyPage;

	//for (int i = 0; i < 9; i++)
	//{
	//	if (keyInfo->GetKeyDown(KEY_0+i))
	//	{
	//		CHATMGR->AddMsg(CTC_SYSMSG, "%d", i);
	//		m_TableIdx = --i;
	//		dwKeyPage = 0;
	//	}
	//}
	if (keyInfo->GetKeyDown(KEY_1))
	{
		m_TableIdx = 0;
		dwKeyPage = 0;
	}
	if (keyInfo->GetKeyDown(KEY_2))
	{
		m_TableIdx = 1;
		dwKeyPage = 0;
	}
	if (keyInfo->GetKeyDown(KEY_3))
	{
		m_TableIdx = 2;
		dwKeyPage = 0;
	}
	if (keyInfo->GetKeyDown(KEY_4))
	{
		m_TableIdx = 3;
		dwKeyPage = 0;
	}
	if (keyInfo->GetKeyDown(KEY_5))
	{
		m_TableIdx = 4;
		dwKeyPage = 0;
	}
	if (keyInfo->GetKeyDown(KEY_6))
	{
		m_TableIdx = 5;
		dwKeyPage = 0;
	}
	if (keyInfo->GetKeyDown(KEY_7))
	{
		m_TableIdx = 6;
		dwKeyPage = 0;
	}
	if (keyInfo->GetKeyDown(KEY_8))
	{
		m_TableIdx = 7;
		dwKeyPage = 0;
	}
	if (keyInfo->GetKeyDown(KEY_9))
	{
		m_TableIdx = 8;
		dwKeyPage = 0;
	}
	if (keyInfo->GetKeyDown(KEY_0))
	{
		m_TableIdx = 9;
		dwKeyPage = 0;
	}
	if (keyInfo->GetKeyDown(KEY_PAD1))
	{
		dwKeyPage = 0;
	}
	if (keyInfo->GetKeyDown(KEY_PAD2))
	{
		dwKeyPage = 1;
	}
	if (keyInfo->GetKeyDown(KEY_PAD3))
	{
		dwKeyPage = 2;
	}
	if (keyInfo->GetKeyDown(KEY_PAD4))
	{
		dwKeyPage = 3;
	}
	if (keyInfo->GetKeyDown(KEY_PAD5))
	{
		dwKeyPage = 4;
	}
	if (keyInfo->GetKeyDown(KEY_PAD6))
	{
		dwKeyPage = 5;
	}
	if (keyInfo->GetKeyDown(KEY_PAD7))
	{
		dwKeyPage = 6;
	}
	if (keyInfo->GetKeyDown(KEY_PAD8))
	{
		dwKeyPage = 7;
	}
	if (keyInfo->GetKeyDown(KEY_PAD9))
	{
		dwKeyPage = 8;
	}
	if (keyInfo->GetKeyDown(KEY_PAD0))
	{
		dwKeyPage = 9;
	}
	SetGoldItemInfo(m_TableIdx, dwKeyPage);
	m_pTabPushBtn[m_TableIdx]->SetPush(true);
	m_pPagePushBtn[dwKeyPage]->SetPush(true);
	return WE_NULL;
}
void CGoldShopDialog::OnActionEvent(LONG lId, void * p, DWORD we)
{
	if (we & WE_CLOSEWINDOW)
	{
		HideDealer();
	}
	if (we & WE_PUSHDOWN)
	{
		if (TC_TABBTN1 <= lId && lId <= TC_TABBTN10)
		{
			for (int i = 0; i<10; i++)
			{
				m_pTabPushBtn[i]->SetPush(false);
			}
			for (int i = 0; i<10; i++)
			{
				m_pPagePushBtn[i]->SetPush(false);
			}
			m_TableIdx = lId - TC_TABBTN1;
//			writeConsole("%d", m_TableIdx);
			
			SetGoldItemInfo(m_TableIdx, m_PageIdx[m_TableIdx]);
			m_pTabPushBtn[m_TableIdx]->SetPush(true);
			m_pPagePushBtn[m_PageIdx[m_TableIdx]]->SetPush(true);
		}
		if (TC_PAGEBTN1 <= lId && lId <= TC_PAGEBTN10)
		{
			for (int i = 0; i<10; i++)
			{
				m_pPagePushBtn[i]->SetPush(false);
			}

			m_PageIdx[m_TableIdx] = lId - TC_PAGEBTN1;
			SetGoldItemInfo(m_TableIdx, m_PageIdx[m_TableIdx]);
			m_pPagePushBtn[m_PageIdx[m_TableIdx]]->SetPush(true);
		}
	}
	else if (we & WE_PUSHUP)
	{
		if (TC_TABBTN1 <= lId && lId <= TC_TABBTN10)
		{
			m_pTabPushBtn[lId - TC_TABBTN1]->SetPush(true);
		}
		if (TC_PAGEBTN1 <= lId && lId <= TC_PAGEBTN10)
		{
			m_pPagePushBtn[lId - TC_PAGEBTN1]->SetPush(true);
		}
	}
}
void CGoldShopDialog::SetGoldMoney(long Val)
{
	MallMoneyOne->SetStaticText(AddComma(Val));
	MallMoneyOne->SetAlign(TXT_LEFT);
}
void CGoldShopDialog::SetMallMoney(long Val)
{
	MallMoney->SetStaticText(AddComma(Val));
	MallMoney->SetAlign(TXT_LEFT);
}

void CGoldShopDialog::SetPageStatic(WORD count)
{

	for (int i = 0; i<10; i++)
	{
		cWindow*BtnBtn = GetWindowForID(TC_PAGEBTN1 + i);
		BtnBtn->SetActive(FALSE);
	}
	if (count<15)  return;
	if (count > 135)
	{
		for (int i = 0; i<10; i++)
		{
			cWindow*BtnBtn = GetWindowForID(TC_PAGEBTN1 + i);
			BtnBtn->SetActive(TRUE);
		}
	}
	if (count > 120)
	{
		for (int i = 0; i<9; i++)
		{
			cWindow*BtnBtn = GetWindowForID(TC_PAGEBTN1 + i);
			BtnBtn->SetActive(TRUE);
		}
	}
	if (count > 105)
	{
		for (int i = 0; i<8; i++)
		{
			cWindow*BtnBtn = GetWindowForID(TC_PAGEBTN1 + i);
			BtnBtn->SetActive(TRUE);
		}
	}
	if (count > 90)
	{
		for (int i = 0; i<7; i++)
		{
			cWindow*BtnBtn = GetWindowForID(TC_PAGEBTN1 + i);
			BtnBtn->SetActive(TRUE);
		}
	}
	if (count > 75)
	{
		for (int i = 0; i<6; i++)
		{
			cWindow*BtnBtn = GetWindowForID(TC_PAGEBTN1 + i);
			BtnBtn->SetActive(TRUE);
		}
	}
	if (count > 60)
	{
		for (int i = 0; i<5; i++)
		{
			cWindow*BtnBtn = GetWindowForID(TC_PAGEBTN1 + i);
			BtnBtn->SetActive(TRUE);
		}
	}
	else if (count > 45)
	{
		for (int i = 0; i<4; i++)
		{
			cWindow*BtnBtn = GetWindowForID(TC_PAGEBTN1 + i);
			BtnBtn->SetActive(TRUE);
		}
	}
	else if (count > 30)
	{
		for (int i = 0; i<3; i++)
		{
			cWindow*BtnBtn = GetWindowForID(TC_PAGEBTN1 + i);
			BtnBtn->SetActive(TRUE);
		}
	}
	else if (count > 15)
	{
		for (int i = 0; i<2; i++)
		{
			cWindow*BtnBtn = GetWindowForID(TC_PAGEBTN1 + i);
			BtnBtn->SetActive(TRUE);
		}
	}
//#else
//	for (int i = 0; i<10; i++)
//	{
//		cWindow*BtnBtn = GetWindowForID(TC_PAGEBTN1 + i);
//		BtnBtn->SetActive(TRUE);//def FALSE
//	}
//	/*if (count<15)  return;
//	if (count > 135)
//	{
//		for (int i = 0; i<10; i++)
//		{
//			cWindow*BtnBtn = GetWindowForID(TC_PAGEBTN1 + i);
//			BtnBtn->SetActive(TRUE);
//		}
//	}
//	if (count > 120)
//	{
//		for (int i = 0; i<9; i++)
//		{
//			cWindow*BtnBtn = GetWindowForID(TC_PAGEBTN1 + i);
//			BtnBtn->SetActive(TRUE);
//		}
//	}
//	if (count > 105)
//	{
//		for (int i = 0; i<8; i++)
//		{
//			cWindow*BtnBtn = GetWindowForID(TC_PAGEBTN1 + i);
//			BtnBtn->SetActive(TRUE);
//		}
//	}
//	if (count > 90)
//	{
//		for (int i = 0; i<7; i++)
//		{
//			cWindow*BtnBtn = GetWindowForID(TC_PAGEBTN1 + i);
//			BtnBtn->SetActive(TRUE);
//		}
//	}
//	if (count > 75)
//	{
//		for (int i = 0; i<6; i++)
//		{
//			cWindow*BtnBtn = GetWindowForID(TC_PAGEBTN1 + i);
//			BtnBtn->SetActive(TRUE);
//		}
//	}
//	if (count > 60)
//	{
//		for (int i = 0; i<5; i++)
//		{
//			cWindow*BtnBtn = GetWindowForID(TC_PAGEBTN1 + i);
//			BtnBtn->SetActive(TRUE);
//		}
//	}
//	else if (count > 45)
//	{
//		for (int i = 0; i<4; i++)
//		{
//			cWindow*BtnBtn = GetWindowForID(TC_PAGEBTN1 + i);
//			BtnBtn->SetActive(TRUE);
//		}
//	}
//	else if (count > 30)
//	{
//		for (int i = 0; i<3; i++)
//		{
//			cWindow*BtnBtn = GetWindowForID(TC_PAGEBTN1 + i);
//			BtnBtn->SetActive(TRUE);
//		}
//	}
//	else if (count > 15)
//	{
//		for (int i = 0; i<2; i++)
//		{
//			cWindow*BtnBtn = GetWindowForID(TC_PAGEBTN1 + i);
//			BtnBtn->SetActive(TRUE);
//		}
//	}*/
//#endif
}
void CGoldShopDialog::SendSelectRequest(DWORD Type)
{
	MSG_WORD msg;
	msg.Category = MP_MALLLIST;
	msg.Protocol = MP_MALLLIST_SYN;
	msg.dwObjectID = HEROID;
	msg.wData = Type;
	NETWORK->Send(&msg, sizeof(msg));
}
BOOL CGoldShopDialog::CheckShopItemDate(DWORD dwTime)
{
	stTIME Time;
	SYSTEMTIME sysTime;
	GetLocalTime(&sysTime);
	Time.SetTime(sysTime.wYear, sysTime.wMonth, sysTime.wDay, sysTime.wHour, sysTime.wMinute, sysTime.wSecond);
	return Time.value < dwTime;
}
void CGoldShopDialog::SetGoldItemInfo(int tab, int page)
{
	cImage tmpImage;
	cImage btnBaseImg;
	cImage btnPressImg;
	cImage btnOverImg;
	SCRIPTMGR->GetImage(95, &btnBaseImg, PFT_JACKPATH);
	SCRIPTMGR->GetImage(96, &btnOverImg, PFT_JACKPATH);
	SCRIPTMGR->GetImage(97, &btnPressImg, PFT_JACKPATH);
	CShopItem * pItem = NULL;
	cButton	* pButton = NULL;
	cStatic	* pItemName = NULL;
	ITEM_GOLD_SHOP * pShopItemInfo = NULL;
	int m_Start = GOLDITEM_PAGE_COUNT*page;
	int m_End = GOLDITEM_PAGE_COUNT*(page + 1);
	int temp = 0;
	SetPageStatic(GetGoldShopItemCountByTableIdx(tab));
	SetLastPushedButton(tab, page);
	ClearIconGrid();
	if (m_IconDlg)
	{
		for (int n = m_Start; n<m_End; ++n)
		{
			pShopItemInfo = GetGoldShopItemByTableIdx(tab, n);
			if (pShopItemInfo)
			{
				ITEM_INFO * pInfo = ITEMMGR->GetItemInfo(pShopItemInfo->ItemIdx);
				if (pInfo)
				{
					pItem = new CShopItem;
					pItem->Init(0, 0, DEFAULT_ICONSIZE, DEFAULT_ICONSIZE, ITEMMGR->GetIconImage(pInfo->ItemIdx, &tmpImage), IG_GOLDITEM_START + pShopItemInfo->Key);
					pItem->SetData(pShopItemInfo->ItemIdx);
					pItem->SetItemIdx(pShopItemInfo->ItemIdx);
					pItem->SetGoldPrice(pShopItemInfo->ItemBuyPrice);
					pItem->SetMallPrice(pShopItemInfo->ItemBuyPrice);
					pItem->SetBuyPrice(pShopItemInfo->ItemBuyPrice);
					pItem->SetOfferPrice(pShopItemInfo->ItemOfferPrice);
					pItem->SetItemType(pShopItemInfo->ItemBuyType);
					pItem->SetItemName(pInfo->ItemName);
					pItem->SetItemName2(pShopItemInfo->ItemName);
					pItem->SetType(pInfo->ItemType);

					if(pShopItemInfo->LimitKind!=0 && pShopItemInfo->LimitKind != 10)
						pItem->SetItemVipInfo(CHATMGR->GetChatMsg(2727), pShopItemInfo->LimitKind);
					else if (pShopItemInfo->LimitKind == 10)
						pItem->SetItemVipInfo(CHATMGR->GetChatMsg(2728), pShopItemInfo->LimitKind);
					else
						pItem->SetItemVipInfo("");

					pItem->SetMovable(FALSE);
					pItem->InitBtn(pItem->GetAbsX() + 135, pItem->GetAbsY() + 46, 50, 20, &btnBaseImg, &btnOverImg, &btnPressImg, BuyFunc, IG_GOLDBTN_START + pShopItemInfo->Key, CHATMGR->GetChatMsg(2435));
					ITEMMGR->SetToolTipIcon((cIcon*)pItem,
						NULL,
						NULL,
						0,
						NULL,
						0,
						1,
						0,
						TRUE
						);
					if (pShopItemInfo->LimitCount != 0)
					{
						stTIME stTime;
						stTime.SetTime(pShopItemInfo->LimitCount);
						char buf[64] = { 0, };
						wsprintf(buf, CHATMGR->GetChatMsg(2647),
							stTime.GetYear(),
							stTime.GetMonth(),
							stTime.GetDay(),
							stTime.GetHour(),
							stTime.GetMinute()
							);
						pItem->AddToolTipLine(buf, RGB(0, 255, 0));
						pItem->AddToolTipLine("");
					}
					WINDOWMGR->AddWindow(pItem);
					if (page >= 1)
					{
						m_IconDlg->AddIcon(n - (GOLDITEM_PAGE_COUNT*page), pItem);
					}
					else
					{
						m_IconDlg->AddIcon(n, pItem);
					}

					//delete[]pItem;
				}
				else
				{
					pItem = new CShopItem;
					pItem->SetGoldPrice(0);
					pItem->SetMallPrice(0);
					pItem->SetBuyPrice(0);
					pItem->SetOfferPrice(0);
					pItem->SetItemType(-500);
					pItem->SetItemName(" ");
					pItem->SetItemName2("Itemidx not existed!");
					pItem->SetType(0);
					pItem->SetMovable(FALSE);
					WINDOWMGR->AddWindow(pItem);
					if (page >= 1)
					{
						m_IconDlg->AddIcon(n - (GOLDITEM_PAGE_COUNT*page), pItem);
					}
					else
					{
						m_IconDlg->AddIcon(n, pItem);
					}
					//delete[]pItem;
				}
			}
			else
			{
				temp = temp + 1;
				if (temp>14)
				{
					CHATMGR->AddMsg(CTC_GETITEM, CHATMGR->GetChatMsg(2385));
					continue;
				}
			}
		}
	}
}
void CGoldShopDialog::ShowDealer()
{
	if (m_fShow) HideDealer();
	GAMEIN->UpdataGoldMoney();
	SetActive(TRUE);
	GAMEIN->GetMainInterfaceDialog()->SetAlram(OPT_MALLNOTICEDLGICON, FALSE);
	GAMEIN->GetMainInterfaceDialog()->Refresh();
	SetGoldItemInfo(m_LastTabPush, m_LastPagePush);
	m_pTabPushBtn[m_LastTabPush]->SetPush(true);
	m_pPagePushBtn[m_LastPagePush]->SetPush(true);
	m_fShow = TRUE;
}
void CGoldShopDialog::HideDealer()
{
	if (!m_fShow) return;
	ClearIconGrid();
	SetActive(FALSE);
	m_fShow = FALSE;
	GAMEIN->GetGoldShopBuyDialog()->SetActive(FALSE);
}
BOOL CGoldShopDialog::IsShopOpen()
{
	return m_bShopIsOpen;
}
VOID CGoldShopDialog::SetShopOpen(BOOL bVal)
{
	m_bShopIsOpen = bVal;
}
BOOL CGoldShopDialog::FakeMoveIcon(LONG x, LONG y, cIcon * icon)
{
	return FALSE;
}
ITEM_GOLD_SHOP * CGoldShopDialog::GetGoldShopItemByKey(DWORD Key)
{
	return m_GoldItemList.GetData(Key);
}
ITEM_GOLD_SHOP * CGoldShopDialog::GetGoldShopItemByTableIdx(int Tab, int Pos)
{
	ITEM_GOLD_SHOP * pShopItemInfo = NULL;
	m_GoldItemList.SetPositionHead();
	while (pShopItemInfo = m_GoldItemList.GetData())
	{
		if (pShopItemInfo->TableIdx == Tab && pShopItemInfo->Idx == Pos)
		{
			return pShopItemInfo;
		}
	}
	return pShopItemInfo;
}
ITEM_GOLD_SHOP * CGoldShopDialog::GetGoldShopItemByItemIdx(DWORD ItemIdx)
{
	ITEM_GOLD_SHOP * pShopItemInfo = NULL;
	m_GoldItemList.SetPositionHead();
	while (pShopItemInfo = m_GoldItemList.GetData())
	{
		if (pShopItemInfo->ItemIdx == ItemIdx)
		{
			return pShopItemInfo;
		}
	}
	return pShopItemInfo;
}
int CGoldShopDialog::GetGoldShopItemCountByTableIdx(int Tab)
{
	ITEM_GOLD_SHOP * pShopItemInfo = NULL;
	m_GoldItemList.SetPositionHead();
	int Count = 0;
	while (pShopItemInfo = m_GoldItemList.GetData())
	{
		if (pShopItemInfo->TableIdx == Tab)
		{
			Count = Count + 1;
		}
	}
	return Count;
}

void CGoldShopDialog::SendGoldGetMsg()
{
	MSG_WORD msg;
	msg.Category = MP_MALLLIST;
	msg.Protocol = MP_MALLLIST_SYN;
	msg.dwObjectID = HEROID;
	msg.wData = HEROID;
	NETWORK->Send(&msg, sizeof(msg));
}
void CGoldShopDialog::ClearIconGrid()
{
	cIcon* pIcon;
	CShopItem * pShopItem;
	if (m_IconDlg)
	{
		for (int i = 0; i<GOLDITEM_PAGE_COUNT; i++)
		{
			pIcon = m_IconDlg->GetIconForIdx(i);
			if (pIcon != NULL)
			{
				pShopItem = (CShopItem*)pIcon;
				if (pShopItem != NULL)
				{
					pShopItem->SetActive(FALSE);
					WINDOWMGR->AddListDestroyWindow(pShopItem);
					pShopItem = NULL;
				}
				m_IconDlg->DeleteIcon(pIcon);
				pIcon->SetActive(FALSE);
				pIcon = NULL;
			}
		}
	}
}


void OnShopBuyItem(LONG iId, void* p, DWORD param1, void * vData1, void * vData2)
{
	ITEM_GOLD_SHOP * pShopItem = (ITEM_GOLD_SHOP*)vData2;
	if (pShopItem)
	{
		GAMEIN->GetGoldShopBuyDialog()->SetCount(param1);
		GAMEIN->GetGoldShopBuyDialog()->Init(pShopItem);
		GAMEIN->GetGoldShopBuyDialog()->SetActive(TRUE);
		GAMEIN->GetGoldShopBuyDialog()->SetTopOnActive(TRUE);
		GAMEIN->GetGoldShopBuyDialog()->SetMovable(FALSE);
	}
}
void  OnShopCancelBuyItem(LONG iId, void* p, DWORD param1, void * vData1, void * vData2)
{
	return;
}
extern HWND _g_hWnd;
void BuyFunc(LONG lId, void * p, DWORD we)
{
	if (we & WE_BTNCLICK)
	{
		DWORD Key = lId - IG_GOLDBTN_START;
		ITEM_GOLD_SHOP * pShopItem = GAMEIN->GetGoldShopDialog()->GetGoldShopItemByKey(Key);
		if (pShopItem)
		{
			LONG x = MOUSE->GetMouseEventX();
			LONG y = MOUSE->GetMouseEventY();
			cDivideBox * pDivideBox = WINDOWMGR->DivideBox(DBOX_BUY, x, y, OnShopBuyItem, OnShopCancelBuyItem, NULL, pShopItem, CHATMGR->GetChatMsg(187));
			if (!pDivideBox)
			{
				return;
			}
			pDivideBox->SetMaxValue(MAX_YOUNGYAKITEM_DUPNUM);
			pDivideBox->SetValue(1);
			POINT point = { LONG(pDivideBox->GetAbsX()) + 65, LONG(pDivideBox->GetAbsY()) + 65 };
			ClientToScreen(
				_g_hWnd,
				&point);
			SetCursorPos(point.x, point.y);
		}
	}
}

#else
#include "GoldShopDialog.h"
#include "WindowIDEnum.h"
#include <shellapi.h.>
#include "stdafx.h"
#include "DealDialog.h"
#include "DealItem.h"
#include "ItemManager.h"
#include "ObjectManager.h"
#include "MoveManager.h"
#include "WindowIDEnum.h"
#include "./Interface/cWindowManager.h"
#include "./Interface/cIconGridDialog.h"
#include "./Interface/cStatic.h"
#include "./interface/cTabDialog.h"
#include "./interface/cPushupButton.h"
#include "GameIn.h"
#include "ObjectStateManager.h"
#include "cDivideBox.h"
#include "InventoryExDialog.h"
#include "mhFile.h"
#include "cMsgBox.h"
#include "ChatManager.h"
#include "MHMap.h"
#include "SiegeWarMgr.h"
#include "TitanManager.h"
#include "BobusangManager.h"
#include "NpcScriptDialog.h"
#include "FortWarManager.h"
#include "hero.h"
#include "MainbarDialog.h"
#include "./input/UserInput.h"
CGoldShopDialog::CGoldShopDialog()
{
	pDlg = NULL;
	m_type = WT_GODLSHOP_DLG;
	m_bShopIsOpen = FALSE;

	m_wTabIdx = -1;
	m_CurSelectedItemIdx = 0;
	m_lCurSelItemPos = -1;
	m_wIdx = -1;
}
CGoldShopDialog::~CGoldShopDialog()
{
	pDlg = NULL;
	MallMoneyOne = NULL;
	MallMoney = NULL;
	Money = NULL;

	buyBtn = NULL;
}
void CGoldShopDialog::Linking()
{
	pDlg = WINDOWMGR->GetWindowForID(ITEM_GOLDDLG);
	MallMoneyOne = (cStatic *)GetWindowForID(ITEM_GOLDEDIT);
	MallMoney = (cStatic *)GetWindowForID(ITEM_GOLDMOBAOEDIT);
	Money = (cStatic *)GetWindowForID(ITEM_GOLDMONEYEDIT);

	buyBtn = (cButton *)GetWindowForID(ITEM_BUBBLEBUYBTN);
#ifdef _GOLDDLGONEBTN_
	buyBtn->SetActive(FALSE);
	buyBtn->SetDisable(TRUE);
#endif
}
void CGoldShopDialog::SetGoldMoney(long Val)
{
	MallMoneyOne->SetStaticText(AddComma(Val));
	MallMoneyOne->SetAlign(TXT_LEFT);
}
void CGoldShopDialog::SetMallMoney(long Val)
{
	MallMoney->SetStaticText(AddComma(Val));
	MallMoney->SetAlign(TXT_LEFT);
}
void CGoldShopDialog::SetMoney(long Val)
{
	Money->SetStaticText(AddComma(Val));
	Money->SetAlign(TXT_LEFT);
}
void CGoldShopDialog::Add(cWindow * window)
{
	if (window->GetType() == WT_PUSHUPBUTTON)
		AddTabBtn(curIdx1++, (cPushupButton *)window);
	else if (window->GetType() == WT_DIALOG)
		AddTabSheet(curIdx2++, window);
	else if (window->GetType() == WT_ICONGRIDDIALOG)
		AddTabSheet(curIdx2++, window);
	else
		cTabDialog::Add(window);
}
void CGoldShopDialog::OnActionEvent(LONG lId, void * p, DWORD we)
{
	//if (m_CurSelectedItemIdx == 0) return;//kiv
	//

	if (we & WE_BTNCLICK)
	{
		if (lId == ITEM_GOLDBUYBTN)
		{
			if (m_wTabIdx == -1) return;
			cIconGridDialog * gridDlg = (cIconGridDialog *)GetTabSheet(GetCurTabNum());
			CDealItem* pItem = (CDealItem*)gridDlg->GetIconForIdx((WORD)gridDlg->GetCurSelCellPos());
			if (pItem)
			{
				LONG x = MOUSE->GetMouseEventX();
				LONG y = MOUSE->GetMouseEventY();
				FakeBuyItem(x, y, pItem,2);
			}
			/*OnSelectedItem();
			ITEM_INFO* pItemInfo = ITEMMGR->GetItemInfo(m_CurSelectedItemIdx);
			if (pItemInfo)
			{
				MSG_ITEM_BUY_SYN sMsg;
				sMsg.wDealerIdx = 10000;
				sMsg.BuyItemNum = 1;
				sMsg.wBuyItemIdx = m_CurSelectedItemIdx;
				sMsg.wIdxPos = m_wIdx;
				sMsg.wBuyType = 2;
				sMsg.Category = MP_ITEM;
				sMsg.Protocol = MP_ITEM_BUY_SYN;
				sMsg.dwObjectID = HEROID;
				NETWORK->Send(&sMsg, sMsg.GetSize());
			}*/
		}
		if (lId == ITEM_BUBBLEBUYBTN)
		{
			if (m_wTabIdx == -1) return;
			cIconGridDialog * gridDlg = (cIconGridDialog *)GetTabSheet(GetCurTabNum());
			CDealItem* pItem = (CDealItem*)gridDlg->GetIconForIdx((WORD)gridDlg->GetCurSelCellPos());
			if (pItem)
			{
				LONG x = MOUSE->GetMouseEventX();
				LONG y = MOUSE->GetMouseEventY();
				FakeBuyItem(x, y, pItem,1);
			}
			/*OnSelectedItem();
			ITEM_INFO* pItemInfo = ITEMMGR->GetItemInfo(m_CurSelectedItemIdx);
			if (pItemInfo)
			{
			MSG_ITEM_BUY_SYN sMsg;
			sMsg.wDealerIdx = 10000;
			sMsg.BuyItemNum = 1;
			sMsg.wBuyItemIdx = m_CurSelectedItemIdx;
			sMsg.wIdxPos = m_wIdx;
			sMsg.wBuyType = 2;
			sMsg.Category = MP_ITEM;
			sMsg.Protocol = MP_ITEM_BUY_SYN;
			sMsg.dwObjectID = HEROID;
			NETWORK->Send(&sMsg, sMsg.GetSize());
			}*/
		}
	}
	if (we & WE_CLOSEWINDOW)
	{
		HideDealer();
	}
	if (we & WE_LBTNCLICK)
	{
		if (lId != -1)
		{
			OnSelectedItem();
		}
	}
}
void CGoldShopDialog::ShowDealer()
{
	if (m_fShow) HideDealer();
	GAMEIN->UpdataGoldMoney();
	GAMEIN->GetMainInterfaceDialog()->SetAlram(OPT_MALLNOTICEDLGICON, FALSE);
	GAMEIN->GetMainInterfaceDialog()->Refresh();
	SelectTab(0);
	SetActive(TRUE);

	for (int i = 0; i < m_bTabNum; ++i)
	{
		cPushupButton* Btn = GetTabBtn(i);
		Btn->SetActive(FALSE);
	}
	CDealItem* pItem = NULL;
	ITEM_GOLD_SHOP * pInfo = NULL;
	cImage tmpImage;
	DWORD DealIdx = 0;
	for (int i = 0; i<m_bTabNum; i++)
	{
		for (int k = 0; k<112; k++)
		{
			pInfo = ITEMMGR->GetGoldShopItemByTableIdx(i, k);
			if (pInfo)
			{
				pItem = new CDealItem;
				pItem->Init(0, 0, DEFAULT_ICONSIZE, DEFAULT_ICONSIZE, ITEMMGR->GetIconImage(pInfo->ItemIdx, &tmpImage));
				pItem->SetData(pInfo->ItemIdx);
				pItem->SetItemIdx(pInfo->ItemIdx);
				pItem->m_wIdx = k;
				pItem->SetType(135);
				pItem->SetMovable(FALSE);
				ITEMMGR->SetToolTipIcon((cIcon*)pItem,
					NULL,
					NULL,
					0,
					NULL,
					0,
					1
					);


				char bufPrice[32];
				

#ifdef _GOLDDLGONEBTN_
				if (pInfo->ItemCount == 1)
				{
					wsprintf(bufPrice, CHATMGR->GetChatMsg(2063), AddComma(pInfo->ItemGoldPrice));
					pItem->AddToolTipLine(bufPrice, TTTC_BUYPRICE);
				}
				if (pInfo->ItemCount == 2)
				{
					wsprintf(bufPrice, CHATMGR->GetChatMsg(2064), AddComma(pInfo->ItemGoldPrice));
					pItem->AddToolTipLine(bufPrice, TTTC_BUYPRICE);
				}
				if (pInfo->ItemCount == 3)
				{
					wsprintf(bufPrice, CHATMGR->GetChatMsg(2062), AddComma(pInfo->ItemGoldPrice));
					pItem->AddToolTipLine(bufPrice, TTTC_BUYPRICE);
				}
#else
				wsprintf(bufPrice, CHATMGR->GetChatMsg(2062), AddComma(pInfo->ItemMoneyPrice));
				pItem->AddToolTipLine(bufPrice, TTTC_BUYPRICE);

				wsprintf(bufPrice, CHATMGR->GetChatMsg(2064), AddComma(pInfo->ItemGoldPrice));
				pItem->AddToolTipLine(bufPrice, TTTC_BUYPRICE);

				wsprintf(bufPrice, CHATMGR->GetChatMsg(2063), AddComma(pInfo->ItemMallPrice));
				pItem->AddToolTipLine(bufPrice, TTTC_BUYPRICE);
#endif

				if (!ITEMMGR->CanEquip(pInfo->ItemIdx))
					pItem->SetToolTipImageRGB(TTCLR_ITEM_CANNOTEQUIP);

				pItem->AddToolTipLine("");

				WINDOWMGR->AddWindow(pItem);
				cIconGridDialog * dlg = (cIconGridDialog *)GetTabSheet(pInfo->TableIdx);
				dlg->AddIcon(pInfo->Idx, pItem);
			}
		}
	}
	cPushupButton* Btn = GetTabBtn(1);
	if (!Btn->IsActive())
	{
		Btn->SetActive(TRUE);
	}
	GAMEIN->GetInventoryDialog()->SetActive(TRUE);

	m_wTabIdx = -1;
	m_CurSelectedItemIdx = 0;
	m_lCurSelItemPos = -1;
	m_wIdx = -1;
	m_fShow = TRUE;
	ITEMMGR->SetPriceToItem(TRUE);
}
void CGoldShopDialog::HideDealer()
{
	if (!m_fShow) return;
	cIcon* pIcon;
	for (int tab = 0; tab<m_bTabNum; ++tab)
	{
		cIconGridDialog * dlg = (cIconGridDialog *)GetTabSheet(tab);
		for (int n = 0; n<dlg->m_nCol*dlg->m_nRow; ++n)
		{
			pIcon = dlg->GetIconForIdx(n);
			if (pIcon != NULL)
			{
				dlg->DeleteIcon(pIcon);
				pIcon->SetActive(FALSE);
				WINDOWMGR->AddListDestroyWindow(pIcon);
				pIcon = NULL;
			}
		}
	}
	m_DealerIdx = 0;
	m_fShow = FALSE;

	m_wTabIdx = -1;
	m_CurSelectedItemIdx = 0;
	m_lCurSelItemPos = -1;
	m_wIdx = -1;

	ITEMMGR->SetPriceToItem(FALSE);
}
void CGoldShopDialog::WindowClose()
{
	SetActive(FALSE);
}
void CGoldShopDialog::OnSelectedItem()
{
	cIconGridDialog * gridDlg = (cIconGridDialog *)GetTabSheet(GetCurTabNum());
	CDealItem* pItem = (CDealItem*)gridDlg->GetIconForIdx((WORD)gridDlg->GetCurSelCellPos());
	if (!pItem)
	{
		m_wTabIdx = -1;
		m_CurSelectedItemIdx = 0;
		m_lCurSelItemPos = -1;
		m_wIdx = -1;
		return;
	}
	m_CurSelectedItemIdx = pItem->GetItemIdx();
	m_lCurSelItemPos = gridDlg->GetCurSelCellPos();
	m_wIdx = pItem->m_wIdx;

	for (int i = 0; i < m_bTabNum; ++i)
	{
		cPushupButton* Btn = GetTabBtn(i);
		if (Btn->IsPushed())
		{
			m_wTabIdx = i;
		}
	}
	
	//CHATMGR->AddMsg(CTC_SYSMSG, "%d,m_wIdx:%d,KIND:%d,dwPush:%d", m_CurSelectedItemIdx, m_wIdx,
	//	ITEMMGR->GetGoldShopItemByIdxAndItemIdx(m_wTabIdx,m_CurSelectedItemIdx, m_wIdx)->ItemCount,
	//	m_wTabIdx);

	ITEM_INFO * pItemInfo = ITEMMGR->GetItemInfo(m_CurSelectedItemIdx);
	ASSERT(pItemInfo);

	//DWORD ItemPrice = pItemInfo->BuyPrice;
	//if (MAP->IsVillage() == FALSE)
	//	ItemPrice = (DWORD)(ItemPrice * 1.2);
	//ItemPrice = SWPROFIT->CalTexRateForBuy(ItemPrice);
	//ItemPrice = FORTWARMGR->CalTexRateForBuy(ItemPrice);

	//LONG x = MOUSE->GetMouseEventX();
	//LONG y = MOUSE->GetMouseEventY();
}
BOOL CGoldShopDialog::IsShopOpen()
{
	return m_bShopIsOpen;
}
VOID CGoldShopDialog::SetShopOpen(BOOL bVal)
{
	m_bShopIsOpen = bVal;
}
extern HWND _g_hWnd;
void CGoldShopDialog::FakeBuyItem(LONG x, LONG y, CDealItem* pItem, BYTE buytype)
{
	WORD itemIdx = pItem->GetItemIdx();
	//if (m_CurSelectedItemIdx == 0) return;//kiv
	if (itemIdx == 0) return;
	if (0 == pItem->GetCount())
	{
		CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(1641));
		return;
	}
	memset(&m_buyMsg, 0, sizeof(m_buyMsg));
	m_buyMsg.Category = MP_ITEM;
	m_buyMsg.Protocol = MP_ITEM_BUY_SYN;
	m_buyMsg.dwObjectID = HEROID;
	m_buyMsg.wBuyItemIdx = itemIdx;
	m_buyMsg.wDealerIdx = 10000;
	m_buyMsg.wIdxPos = m_wIdx;
#ifdef _GOLDDLGONEBTN_
	m_buyMsg.wBuyType = ITEMMGR->GetGoldShopItemByIdxAndItemIdx(m_wTabIdx, m_CurSelectedItemIdx, m_wIdx)->ItemCount;
#else
	m_buyMsg.wBuyType = buytype;
#endif
	m_buyMsg.BuyRight = m_wTabIdx;
	cDivideBox * pDivideBox = WINDOWMGR->DivideBox(DBOX_BUY, x, y, OnFakeBuyItem, OnCancelBuyItem,
		this, pItem, CHATMGR->GetChatMsg(187));
	if (!pDivideBox)
		return;
	pDivideBox->SetMaxValue(MAX_ITEMBUY_NUM);
	pDivideBox->SetValue(1);
	GAMEIN->GetInventoryDialog()->SetDisable(TRUE);
	POINT point = { LONG(pDivideBox->GetAbsX()) + 65, LONG(pDivideBox->GetAbsY()) + 65 };
	ClientToScreen(
		_g_hWnd,
		&point);
	SetCursorPos(point.x, point.y);
}
BOOL CGoldShopDialog::FakeMoveIcon(LONG x, LONG y, cIcon * icon)
{
	return FALSE;
}
void CGoldShopDialog::OnFakeBuyItem(LONG iId, void* p, DWORD param1, void * vData1, void * vData2)
{
	if (param1 == 0)
	{
		ITEMMGR->SetDisableDialog(FALSE, eItemTable_Deal);
		ITEMMGR->SetDisableDialog(FALSE, eItemTable_Inventory);
		return;
	}
	CGoldShopDialog * tDlg = (CGoldShopDialog *)vData1;
	CDealItem* pItem = (CDealItem*)vData2;
	ASSERT(pItem);
	tDlg->m_buyMsg.BuyItemNum = (WORD)param1;
	NETWORK->Send(&tDlg->m_buyMsg, tDlg->m_buyMsg.GetSize());
}
void CGoldShopDialog::OnCancelBuyItem(LONG iId, void* p, DWORD param1, void * vData1, void * vData2)
{
	GAMEIN->GetInventoryDialog()->SetDisable(FALSE);
}

#endif