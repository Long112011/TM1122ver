#include "stdafx.h"
#include "EventNotifyDialog.h"
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
#define PosCount 75
#define TickCount 25
#define DownTick 35
CEventNotifyDialog::CEventNotifyDialog()
{
	m_pStcTitle = NULL;
	m_pTAContext = NULL;
	m_glistenCount2 = 0;
	m_bFlicker = FALSE;
	m_bFlActive = FALSE;
	m_dwFlickerSwapTime = 0;
	m_bFlicker2 = FALSE;
	m_bFlActive2 = FALSE;
	m_dwFlickerSwapTime2 = 0;

	absX=0.f;
	absY=0.f;
}
CEventNotifyDialog::~CEventNotifyDialog()
{
}
void CEventNotifyDialog::Linking()
{
	m_pStcTitle = (cStatic*)GetWindowForID(GMOT_STC_TITLE);
	m_pTAContext = (cTextArea*)GetWindowForID(GMOT_TA_CONTEXT);
	cImage ToolTipImg;
	SCRIPTMGR->GetImage(63, &ToolTipImg, PFT_HARDPATH);

	absX = GAMEIN->GetQuickDialog()->GetAbsX();
	absY = GAMEIN->GetQuickDialog()->GetAbsY() + 35;
}
void CEventNotifyDialog::SetActive(BOOL val)
{
	cDialog::SetActive(val);
	if (m_pTAContext && !val)
		m_pTAContext->SetScriptText("");
}
DWORD CEventNotifyDialog::ActionEvent(CMouse* mouseInfo)
{
	DWORD we = cDialog::ActionEvent(mouseInfo); return we;
}
void CEventNotifyDialog::SetTitle(char* pStrTitle)
{
	if (m_pStcTitle)
		m_pStcTitle->SetStaticText(pStrTitle);
}
void CEventNotifyDialog::SetContext(char* pStrContext)
{
	if (m_pTAContext)
		m_pTAContext->SetScriptText(pStrContext);
}
void CEventNotifyDialog::Open()
{
	if (this->IsActive())
		return;
	SetAlpha(0);
	SetActive(TRUE);
	SetFlicker(TRUE);
}
void CEventNotifyDialog::Close()
{
	if (!this->IsActive())
		return;
	SetAlpha(220);
	SetTitle("");
	SetContext("");
	SetFlicker2(TRUE);
}
void CEventNotifyDialog::SetFlicker(BOOL bFlicker)
{
	m_bFlicker = bFlicker;
	m_dwFlickerSwapTime = MHTIMEMGR->GetNewCalcCurTime();//gCurTime;
}
void CEventNotifyDialog::SetFlicker2(BOOL bFlicker)
{
	m_bFlicker2 = bFlicker;
	m_dwFlickerSwapTime2 = MHTIMEMGR->GetNewCalcCurTime();//gCurTime;
}
void CEventNotifyDialog::FlickerMussangGuage()
{
	if (!m_bFlicker)	return;
	int TopIconTick = 0;
	if (gCurTime - m_dwFlickerSwapTime > 50)
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
			this->SetAbsXY(((LONG)absX - PosCount) - m_glistenCount2, absY);
			this->SetAlpha(this->m_alpha + m_glistenCount2);
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
void CEventNotifyDialog::FlickerMussangGuage2()
{
	if (!m_bFlicker2)	return;
	int TopIconTick = 0;
	if (gCurTime - m_dwFlickerSwapTime2 > 50)
	{
		if (m_glistenCount2 >= PosCount)
		{
			this->SetAlpha(0);
			SetActive(FALSE);
			SetFlicker2(FALSE);
			m_glistenCount2 = 0;
		}
		else
		{
			m_glistenCount2 = m_glistenCount2 + TickCount;
			this->SetAbsXY(((LONG)absX - PosCount) + m_glistenCount2, absY);
			this->SetAlpha(this->m_alpha - (m_glistenCount2 * 2));
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
void CEventNotifyDialog::Render()
{
	cDialog::Render();
	FlickerMussangGuage();
	FlickerMussangGuage2();
}