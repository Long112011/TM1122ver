#include "stdafx.h"
#include "VSyncDialog.h"
#include "WindowIDEnum.h"
#include "./Interface/cStatic.h"
#include "./Interface/cTextArea.h"
#include "./Input/Mouse.h"
#include "./Interface/cScriptManager.h"
#include "ChatManager.h"
#include "GMNotifyManager.h"
#include "QuickDialog.h"
#include "GameIn.h"
#include "MHTimeManager.h"
#include "TopIcon.h"
#include "cWindowManager.h"
#define PosCount 75
#define TickCount 25
#define DownTick 35
#define SPEED_START_ACCELERATE 10
extern BOOL m_VsyncTest;
cVSyncDialog::cVSyncDialog()
{
	m_pStcTitle = NULL;
	m_pTAContext = NULL;
	m_pDialog = NULL;
	m_glistenCount2 = 0;
	m_bFlicker = FALSE;
	m_bFlActive = FALSE;
	m_dwFlickerSwapTime = 0;
	m_bFlicker2 = FALSE;
	m_bFlActive2 = FALSE;
	m_dwFlickerSwapTime2 = 0;

	absX=0.f;
	absY=0.f;

	m_CloseCountdown = 0;
	m_CloseCountdownTick = 0;
	m_CloseCountdownStart = FALSE;
	//this->m_bMovable = FALSE;
}
void cVSyncDialog::CloseCountdown(int count/*BOOL start*/)
{
	m_CloseCountdown = MHTIMEMGR->GetNewCalcCurTime();
	m_CloseCountdownTick = count;
	m_CloseCountdownStart = TRUE;
}
void cVSyncDialog::CloseCountdownProcess()
{
	if (!m_CloseCountdownStart) return;
	if (MHTIMEMGR->GetNewCalcCurTime() - m_CloseCountdown >= (m_CloseCountdownTick * 1000))
	{
		m_CloseCountdownStart = FALSE;
		Close();
	}
}
cVSyncDialog::~cVSyncDialog()
{
}
void cVSyncDialog::Linking()
{
	m_pDialog = WINDOWMGR->GetWindowForID(VSYNC_DLG);
	m_pStcTitle = (cStatic*)GetWindowForID(VSYNC_TITLE);
	m_pTAContext = (cStatic*)GetWindowForID(VSYNC_CONTEXT);
	cImage ToolTipImg;
	SCRIPTMGR->GetImage(63, &ToolTipImg, PFT_HARDPATH);

	absX = GAMEIN->GetQuickDialog()->GetAbsX();
	absY = GAMEIN->GetQuickDialog()->GetAbsY() + 35 + m_pDialog->GetAbsY();
}
void cVSyncDialog::AddMsg(char* title, char* context)
{
	SetActive(FALSE);
	if (m_pStcTitle)
		m_pStcTitle->SetStaticText(title);

	if (m_pTAContext)
		m_pTAContext->SetStaticText(context);

	Open();
}
void cVSyncDialog::SetActive(BOOL val)
{
	cDialog::SetActive(val);
	if (m_pTAContext && !val)
		m_pTAContext->SetStaticText("");
}
DWORD cVSyncDialog::ActionEvent(CMouse* mouseInfo)
{
	/*DWORD we = cDialog::ActionEvent(mouseInfo); return we;
	if (mouseInfo->LButtonDown())
	{
		SetActive(FALSE);
	}*/

	DWORD we = cDialog::ActionEvent(mouseInfo);

	if (m_pDialog->PtInWindow(mouseInfo->GetMouseEventX(), mouseInfo->GetMouseEventY()))
	if (mouseInfo->LButtonDown())
	{
		Close();
	}

	return we;
}
void cVSyncDialog::SetTitle(char* pStrTitle)
{
	if (m_pStcTitle)
		m_pStcTitle->SetStaticText(pStrTitle);
}
void cVSyncDialog::SetContext(char* pStrContext)
{
	if (m_pTAContext)
		m_pTAContext->SetStaticText(pStrContext);
}
void cVSyncDialog::Open()
{
	if (m_pDialog->IsActive())
		return;
	SetAlpha(0);
	SetActive(TRUE);
	SetFlicker(TRUE);
	CloseCountdown(5);
}
void cVSyncDialog::Close()
{
	if (m_VsyncTest)
		m_VsyncTest = FALSE;
	//if (!m_pDialog->IsActive())
	//	return;
	SetAlpha(220);
	SetTitle("");
	SetContext("");
	SetFlicker2(TRUE);
}
void cVSyncDialog::SetFlicker(BOOL bFlicker)
{
	m_bFlicker = bFlicker;
	m_dwFlickerSwapTime = MHTIMEMGR->GetNewCalcCurTime();//gCurTime;
}
void cVSyncDialog::SetFlicker2(BOOL bFlicker)
{
	m_bFlicker2 = bFlicker;
	m_dwFlickerSwapTime2 = MHTIMEMGR->GetNewCalcCurTime();//gCurTime;
}
void cVSyncDialog::FlickerMussangGuage()
{
	if (!m_bFlicker)	return;
	int TopIconTick = 0;
	if (gCurTime - m_dwFlickerSwapTime > SPEED_START_ACCELERATE)
	{
		if (m_glistenCount2 >= PosCount)
		{
			this->SetAlpha(255);
			SetFlicker(FALSE);
			m_glistenCount2 = 0;
		}
		else
		{
			m_glistenCount2 = m_glistenCount2 + TickCount;
			m_pDialog->SetAbsXY(((LONG)absX - PosCount) - m_glistenCount2, absY);
			m_pDialog->SetAlpha(m_alpha + m_glistenCount2);
		}
		if (TopIconTick >= DownTick)
		{
			TopIconTick = DownTick;
		}
		else
		{
			TopIconTick = TopIconTick + 5;
		}
		m_bFlActive = !m_bFlActive;
		m_dwFlickerSwapTime = MHTIMEMGR->GetCountOneSec();
	}
}
void cVSyncDialog::FlickerMussangGuage2()
{
	if (!m_bFlicker2)	return;
	int TopIconTick = 0;
	if (gCurTime - m_dwFlickerSwapTime2 > SPEED_START_ACCELERATE)
	{
		if (m_glistenCount2 >= PosCount)
		{
			m_pDialog->SetAlpha(0);
			SetActive(FALSE);
			SetFlicker2(FALSE);
			m_glistenCount2 = 0;
		}
		else
		{
			m_glistenCount2 = m_glistenCount2 + TickCount;
			m_pDialog->SetAbsXY(((LONG)absX - PosCount) + m_glistenCount2, absY);
			m_pDialog->SetAlpha(m_alpha - (m_glistenCount2 * 2));
		}
		if (TopIconTick >= DownTick)
		{
			TopIconTick = DownTick;
		}
		else
		{
			TopIconTick = TopIconTick + 5;
		}
		m_bFlActive2 = !m_bFlActive2;
		m_dwFlickerSwapTime2 = MHTIMEMGR->GetCountOneSec();
	}
}
void cVSyncDialog::Render()
{
	cDialog::Render();
	FlickerMussangGuage();
	FlickerMussangGuage2();
	CloseCountdownProcess();
}