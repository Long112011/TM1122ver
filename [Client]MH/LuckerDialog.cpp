#include "Stdafx.h"
#include "LuckerDialog.h"
#include "INTERFACE\cButton.h"
#include "INTERFACE\cStatic.h"
#include "INTERFACE\cIconDialog.h"
#include "INTERFACE\cIconGridDialog.h"
#include "WindowIDEnum.h"
#include "ItemManager.h"
#include "cScriptManager.h"
#include "DealItem.h"
#include "cWindowManager.h"
#include "MHTimeManager.h"
#include "./Input/UserInput.h"
GLOBALTON(CLuckerDialog)
BOOL CLuckerDialog::IsStart;
extern char JACKJACK[64];
BOOL bFirstL;
BOOL bFirstLE;
#define ALPHA_PROCESS_TIME	500
CLuckerDialog::CLuckerDialog()
{
	IsStart = FALSE;
	bFirstL = FALSE;
	bFirstLE = FALSE;
	m_dwStartTime = 0;
	dwCurTime = 0;
	pDlg = NULL;
	m_pStart = NULL;
	lPos = -1;
	alphalPos = 0;
}
CLuckerDialog::~CLuckerDialog()
{
}
void CLuckerDialog::Render()
{
	if (!m_bActive) return;
	cDialog::RenderWindow();
	cDialog::RenderComponent();
	static VECTOR2 Pos;
	for (int i = 0; i < 84; ++i)
	{
		Pos.x = m_absPos.x + 19.f + (m_pLuckerGridDlg->GetCellBorderX() * (i % m_pLuckerGridDlg->GetColumn() + 1)) + (i % m_pLuckerGridDlg->GetColumn() * DEFAULT_ICONSIZE) - 2;
		Pos.y = m_absPos.y + 58.f + (m_pLuckerGridDlg->GetCellBorderY() * (i / m_pLuckerGridDlg->GetColumn() + 1)) + (i / m_pLuckerGridDlg->GetColumn() * DEFAULT_ICONSIZE) - 2;
		if (i != lPos)
		{
			cImage	m_JackIcon;
			SCRIPTMGR->GetImage(137, &m_JackIcon, PFT_HARDPATH);
			m_JackIcon.RenderSprite(NULL, NULL, 0, &Pos, RGBA_MERGE(m_dwImageRGB, alphalPos));
		}
	}
	if (bFirstL)
	{
		DWORD dwElapsed = MHTIMEMGR->GetNewCalcCurTime() - m_dwStartTime1;
		if (dwElapsed > (ALPHA_PROCESS_TIME))
		{
			alphalPos = 255;
			bFirstL = FALSE;
		}
		else
		{
			alphalPos = (BYTE)(0 + (dwElapsed * 255 / (ALPHA_PROCESS_TIME)));
		}
	}
	if (bFirstLE)
	{
		DWORD dwElapsed = MHTIMEMGR->GetNewCalcCurTime() - m_dwStartTime1;
		if (dwElapsed > (ALPHA_PROCESS_TIME * 6))
		{
			bFirstLE = FALSE;
			alphalPos = 0;
			m_pStart->SetActive(TRUE);
		}
		else
		{
			m_pStart->SetAlpha((BYTE)(0 + (dwElapsed * 255 / (ALPHA_PROCESS_TIME * 6))));
			alphalPos = (BYTE)(255 - (dwElapsed * 255 / (ALPHA_PROCESS_TIME * 6)));
		}
	}
	if (lPos != -1)
	{
		m_pLuckerGridDlg->SetCurSelCellPos(lPos);
	}
}
void CLuckerDialog::Linking()
{
	pDlg = WINDOWMGR->GetWindowForID(LUCKERDIALOG);
	m_pLuckerGridDlg = (cIconGridDialog *)GetWindowForID(LUCKER_GRID_ITEM);
	m_pStart = (cButton *)GetWindowForID(LUCKER_BTN_START);
	AddItemToGrid();
}
int TestCount = 0;
DWORD SelectCount[4] = { 700, 500, 50, 5 };
void CLuckerDialog::Process()
{
	if (IsStart)
	{
		static int Test;
		if (gCurTime - Test >= SelectCount[TestCount])
		{
			Test = gCurTime;
			int Pos = random(0, 84);
			SetGridSelect(Pos);
			lPos = Pos;
		}
		if (gCurTime - m_dwStartTime >= 1000)
		{
			m_dwStartTime = gCurTime;
			if (TestCount <= 0)
			{
				SendNetWorkMessage();
				EndProcess();
			}
			else
			{
			}
			--TestCount;
		}
	}
}
void CLuckerDialog::StartProcess()
{
	m_pStart->SetActive(FALSE);
	m_pStart->SetAlpha(0);
	m_dwStartTime = MHTIMEMGR->GetNewCalcCurTime() - 1000;
	IsStart = TRUE;
	bFirstLE = FALSE;
	TestCount = 4;
	lPos = -1;
	m_dwStartTime1 = MHTIMEMGR->GetNewCalcCurTime();
	bFirstL = TRUE;
}
void CLuckerDialog::EndProcess()
{
	IsStart = FALSE;
	m_dwStartTime = 0;
	m_dwStartTime1 = MHTIMEMGR->GetNewCalcCurTime();
	bFirstLE = TRUE;
}
void CLuckerDialog::SendNetWorkMessage()
{
	MSG_DWORD	msg;
	msg.Category = MP_ITEM;
	msg.Protocol = MP_ITEM_LUCKER_SYN;
	msg.dwObjectID = HEROID;
	msg.dwData = 1986;
	NETWORK->Send(&msg, sizeof(msg));
}
void CLuckerDialog::SetGridSelect(int Pos)
{
	cDialog * pDlg = WINDOWMGR->GetWindowForID(LUCKERDIALOG);
	if (pDlg)
	{
		cIconGridDialog * m_pGridDlg = (cIconGridDialog*)pDlg->GetWindowForID(LUCKER_GRID_ITEM);
		if (m_pGridDlg)
		{
			if (Pos >= 0 && Pos <= 83)
			{
				m_pGridDlg->SetCurSelCellPos(Pos);
				lPos = Pos;
			}
		}
	}
}
#include "chatmanager.h"
void CLuckerDialog::OnActionEvent(LONG lId, void * p, DWORD we)
{
	//if (this->IsActive())
	//	m_pLuckerGridDlg->SetCurSelCellPos(lPos);
	/*if (we & WE_RBTNCLICK)
	{
		CHATMGR->AddMsg(CTC_SYSMSG, "test");
	}
	if (we & WE_LBTNCLICK || we & WE_RBTNCLICK || we & WE_RBTNDBLCLICK || we & WE_LBTNDBLCLICK)
	//if (we == WE_LBTNCLICK || we == WE_RBTNCLICK || we == WE_RBTNDBLCLICK || we == WE_LBTNDBLCLICK)
	{
		m_pLuckerGridDlg->SetCurSelCellPos(lPos);
	}
	if (!WINDOWMGR->IsMouseDownUsed())
	if (PtInWindow(g_UserInput.GetMouse()->GetMouseEventX(), g_UserInput.GetMouse()->GetMouseEventY()))
	{
		m_pLuckerGridDlg->SetCurSelCellPos(lPos);
	}*/

	CLuckerDialog* pHandler = (CLuckerDialog*)this;
	cDialog * pDlg = WINDOWMGR->GetWindowForID(LUCKERDIALOG);
	cButton * m_pStart = (cButton*)pDlg->GetWindowForID(LUCKER_BTN_START);
	switch (lId)
	{
	case LUCKER_BTN_START:
		{
			if (pHandler)
			{
				pHandler->StartProcess();
			}
		}
	break;
	case LUCKER_BTN_EXIT:
		{
			pHandler->EndProcess();
			m_pStart->SetActive(TRUE);
			pHandler->SetActive(FALSE);
		}
	break;
	}
}
void CLuckerDialog::AddItemToGrid()
{
	cImage tmpImage;
	cImage imgToolTip;
	cImage imgHeader;
	SCRIPTMGR->GetImage(25, &imgHeader, PFT_JACKPATH);
	SCRIPTMGR->GetImage(26, &imgToolTip, PFT_JACKPATH);
	char buf[256] = { 0, };
	cIcon* pIcon = NULL;
	int LuckerItemIdx = 0;
	for (int i = 1; i <= ITEMMGR->GetItemLuckerNum(); i++)
	{
		pIcon = new cIcon;
		ITEM_LUCKER_INFO* pItem = ITEMMGR->GetItemLuckerInfo(i);
		ITEM_INFO* pInfo = ITEMMGR->GetItemInfo(pItem->ItemIdx);
		if (pInfo)
		{
			pIcon->Init(0, 0, DEFAULT_ICONSIZE, DEFAULT_ICONSIZE, ITEMMGR->GetIconImage(pInfo->ItemIdx, &tmpImage), IG_LUCKERITEM_STARTINDEX + LuckerItemIdx++);
			pIcon->SetData(pInfo->ItemIdx);
			pIcon->SetMovable(FALSE);
			pIcon->SetHeaderImage(&imgHeader);
			pIcon->SetToolTip("", RGBA_MAKE(255, 255, 255, 255), &imgToolTip, TTCLR_ITEM_CANEQUIP);
			pIcon->SetToolModel(1);
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
			m_pLuckerGridDlg->AddIcon(i - 1, pIcon);
		}
		pIcon = NULL;
		pItem = NULL;
		pInfo = NULL;
	}
}