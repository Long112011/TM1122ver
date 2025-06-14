#include "stdafx.h"
#include "cPushupButton.h"
#include "../Input/Mouse.h"
#include "./Input/UserInput.h"
#include "cFont.h"
#include "cWindowManager.h"
extern BOOL m_DebugEnabled;
extern char* WINIDSEARCH(int b);
cPushupButton::cPushupButton()
{
	m_fPushed = FALSE;
	m_fPassive = FALSE;
	m_fDoubleClick = FALSE;
	m_type = WT_PUSHUPBUTTON;

	*m_szTextDebug = 0;
	m_SavedXY.x = 0;
	m_SavedXY.y = 0;
	m_SavedID = 0;
	m_DebugBoolean[0] = FALSE;
	m_DebugBoolean[1] = FALSE;
	m_DebugBoolean[2] = FALSE;
	m_DebugBoolean[3] = FALSE;
	m_SavedWidthHeight[0] = 0;
	m_SavedWidthHeight[1] = 0;
}
cPushupButton::~cPushupButton()
{
}
void cPushupButton::Init(LONG x, LONG y, WORD wid, WORD hei, cImage * basicImage, cImage * overImage, cImage * pressImage, void(*Func)(LONG lId, void * p, DWORD we), LONG ID)
{
	cButton::Init(x, y, wid, hei, basicImage, overImage, pressImage, NULL, ID);
	m_type = WT_PUSHUPBUTTON;
	if (Func != NULL)
		cbWindowFunc = Func;

	//if (m_DebugEnabled)
	{
		m_SavedXY.x = x;
		m_SavedXY.y = y;
		m_SavedID = ID;
		m_SavedWidthHeight[0] = wid;
		m_SavedWidthHeight[1] = hei;
		strncpy(m_szTextDebug, Vector2Text(m_SavedXY.x, m_SavedXY.y,  m_SavedWidthHeight[0], m_SavedWidthHeight[1]), strlen(Vector2Text(m_SavedXY.x, m_SavedXY.y,  m_SavedWidthHeight[0], m_SavedWidthHeight[1])) + 1);
	}
}
DWORD cPushupButton::ActionEvent(CMouse * mouseInfo)
{
	DWORD we = WE_NULL;
	if (!m_bActive) return we;
	we = cWindow::ActionEvent(mouseInfo);
	if (m_bDisable) return we;
	long x = mouseInfo->GetMouseX();
	long y = mouseInfo->GetMouseY();
	long x2 = mouseInfo->GetMouseEventX();
	long y2 = mouseInfo->GetMouseEventY();
	if (m_fPassive)
	{
		if (m_fDoubleClick)
		{
			if (we & (WE_LBTNDBLCLICK))
			{
				if (PtInWindow(x2, y2))
				{
					(m_fPushed ? we |= WE_PUSHUP : we |= WE_PUSHDOWN);
					cbWindowFunc(m_ID, m_pParent, (m_fPushed ? WE_PUSHUP : WE_PUSHDOWN));
				}
			}
		}
		else
		{
			if (we & (WE_LBTNCLICK))
			{
				if (PtInWindow(x2, y2))
				{
					(m_fPushed ? we |= WE_PUSHUP : we |= WE_PUSHDOWN);
					cbWindowFunc(m_ID, m_pParent, (m_fPushed ? WE_PUSHUP : WE_PUSHDOWN));
				}
			}
		}
	}
	else
	{
		if (m_fDoubleClick)
		{
			if (we & (WE_LBTNDBLCLICK))
			{
				if (PtInWindow(x2, y2))
				{
					m_fPushed ^= TRUE;
					SetPush(m_fPushed);
					if (cbWindowFunc)
					{
						(m_fPushed ? we |= WE_PUSHDOWN : we |= WE_PUSHUP);
						(*cbWindowFunc)(m_ID, m_pParent, (m_fPushed ? WE_PUSHDOWN : WE_PUSHUP));
					}
				}
			}
		}
		else
		{
			if (we & (WE_LBTNCLICK))
			{
				if (PtInWindow(x2, y2))
				{
					m_fPushed ^= TRUE;
					SetPush(m_fPushed);
					if (cbWindowFunc)
					{
						(m_fPushed ? we |= WE_PUSHDOWN : we |= WE_PUSHUP);
						(*cbWindowFunc)(m_ID, m_pParent, (m_fPushed ? WE_PUSHDOWN : WE_PUSHUP));
					}
				}
			}
		}
	}

	if (m_DebugEnabled)
	{
		if (we & (WE_LBTNCLICK | WE_LBTNDBLCLICK))
		{
			m_DebugBoolean[DebugIsStillDown] = TRUE;
			//return we;
		}		
		if (mouseInfo->LButtonUp())
		{			
			m_DebugBoolean[DebugIsStillDown] = FALSE;
			//return we;
		}
		if (mouseInfo->LButtonPressed())
		{			
			if (m_DebugBoolean[DebugClicked])
			if (m_DebugBoolean[DebugIsStillDown])
			if (!WINDOWMGR->IsDragWindow())
			{			
				SetAbsXY(m_absPos.x - MOUSE->GetMouseAxisX(), m_absPos.y - MOUSE->GetMouseAxisY());
				m_SavedXY.x = m_SavedXY.x - MOUSE->GetMouseAxisX();
				m_SavedXY.y = m_SavedXY.y - MOUSE->GetMouseAxisY();
				SetRelXY(m_SavedXY.x, m_SavedXY.y);
				strncpy(m_szTextDebug, Vector2Text(m_SavedXY.x, m_SavedXY.y,  m_SavedWidthHeight[0], m_SavedWidthHeight[1]), strlen(Vector2Text(m_SavedXY.x, m_SavedXY.y,  m_SavedWidthHeight[0], m_SavedWidthHeight[1])) + 1);				
			}
			//return we;
		}
		if (we & WE_MOUSEOVER)
		{
			m_DebugBoolean[DebugHover] = TRUE;
			if (g_UserInput.GetKeyboard()->GetKeyPressed(KEY_CONTROL) && mouseInfo->LButtonDown())
			{
				if (m_DebugBoolean[DebugClicked])
				{
					m_DebugBoolean[DebugClicked] = FALSE;
					return we;
				}
				else
				{
					m_DebugBoolean[DebugClicked] = TRUE;
					return we;
				}
			}
		}
		else
		{
			m_DebugBoolean[DebugHover] = FALSE;
		}
		if (m_DebugBoolean[DebugClicked])
		{
			return we;
		}
	}
	return we;
}
void cPushupButton::SetPush(BOOL val)
{
	m_fPushed = val;
	if (m_fPushed)
	{
		m_pCurImage = &m_PressImage;
		m_fgCurColor = m_fgPressColor;
	}
	else
	{
		m_pCurImage = &m_BasicImage;
		m_fgCurColor = m_fgBasicColor;
	}
}
void cPushupButton::SetPushEx(BOOL val)
{
	SetPush(val);
	if (cbWindowFunc)
		(*cbWindowFunc)(m_ID, m_pParent, (m_fPushed ? WE_PUSHDOWN : WE_PUSHUP));
}
