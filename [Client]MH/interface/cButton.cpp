#include "stdafx.h"
#include "cButton.h"
#include "..\Audio\MHAudioManager.h"
#include "../Input/Mouse.h"
#include "cFont.h"
#include "cWindowManager.h"
#include "cScriptManager.h"
#include "./Input/UserInput.h"
#include "ChatManager.h"
#include "MHTimeManager.h"
#include "cPushupButton.h"
extern BOOL m_DebugEnabled;
extern char* WINIDSEARCH(int b);
cButton::cButton()
{
	*m_szText = 0;
	
	m_fgBasicColor = RGB_HALF(255, 255, 255);
	m_fgOverColor = RGB_HALF(255, 255, 255);
	m_fgPressColor = RGB_HALF(255, 255, 255);
	m_fgCurColor = RGB_HALF(255, 255, 255);
	m_nCurTextLen = 0;
	m_type = WT_BUTTON;
	m_textXY.x = 5;
	m_textXY.y = 4;
	m_bClickInside = FALSE;
	m_BtnTextAniX = 1;
	m_BtnTextAniY = 1;
	m_bSound = FALSE;
	m_shadowTextXY.x = 1;
	m_shadowTextXY.y = 0;
	m_shadowColor = RGB_HALF(10, 10, 10);
	m_bShadow = FALSE;

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

cButton::~cButton()
{
}

void cButton::Init(LONG x, LONG y, WORD wid, WORD hei, cImage * basicImage, cImage * overImage, cImage * pressImage, cbFUNC Func, LONG ID)
{
	cWindow::Init(x, y, wid, hei, basicImage, ID);
	m_type = WT_BUTTON;
	if (overImage)		m_OverImage = *overImage;
	if (pressImage)	m_PressImage = *pressImage;
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
void cButton::DebugPressed()
{
	m_DebugBoolean[DebugKDBActivated] = TRUE; m_DebugTimerKDBActivated = gCurTime;
}
DWORD cButton::ActionKeyboardEvent(CKeyboard * keyInfo)
{
	if (!m_bActive) return WE_NULL;

	if (m_DebugEnabled)
	if (m_DebugBoolean[DebugClicked])
	{
		if (keyInfo->GetKeyDown(KEY_LEFT))
		{
			SetAbsXY(m_absPos.x - 1, m_absPos.y);
			DebugPressed();
			
			m_SavedXY.x = m_SavedXY.x - 1;
			strncpy(m_szTextDebug, Vector2Text(m_SavedXY.x, m_SavedXY.y,  m_SavedWidthHeight[0], m_SavedWidthHeight[1]), strlen(Vector2Text(m_SavedXY.x, m_SavedXY.y,  m_SavedWidthHeight[0], m_SavedWidthHeight[1])) + 1);
		}
		if (keyInfo->GetKeyDown(KEY_RIGHT))
		{
			SetAbsXY(m_absPos.x + 1, m_absPos.y);
			DebugPressed();
			m_SavedXY.x = m_SavedXY.x + 1;
			strncpy(m_szTextDebug, Vector2Text(m_SavedXY.x, m_SavedXY.y,  m_SavedWidthHeight[0], m_SavedWidthHeight[1]), strlen(Vector2Text(m_SavedXY.x, m_SavedXY.y,  m_SavedWidthHeight[0], m_SavedWidthHeight[1])) + 1);
		}
		if (keyInfo->GetKeyDown(KEY_UP))
		{
			SetAbsXY(m_absPos.x, m_absPos.y - 1);
			DebugPressed();
			m_SavedXY.y = m_SavedXY.y - 1;
			strncpy(m_szTextDebug, Vector2Text(m_SavedXY.x, m_SavedXY.y,  m_SavedWidthHeight[0], m_SavedWidthHeight[1]), strlen(Vector2Text(m_SavedXY.x, m_SavedXY.y,  m_SavedWidthHeight[0], m_SavedWidthHeight[1])) + 1);
		}
		if (keyInfo->GetKeyDown(KEY_DOWN))
		{
			SetAbsXY(m_absPos.x, m_absPos.y + 1);
			DebugPressed();
			m_SavedXY.y = m_SavedXY.y + 1;
			strncpy(m_szTextDebug, Vector2Text(m_SavedXY.x, m_SavedXY.y,  m_SavedWidthHeight[0], m_SavedWidthHeight[1]), strlen(Vector2Text(m_SavedXY.x, m_SavedXY.y,  m_SavedWidthHeight[0], m_SavedWidthHeight[1])) + 1);
		}
		SetRelXY(m_SavedXY.x, m_SavedXY.y);
		m_DebugBoolean[DebugKDBActivated] = !keyInfo->GetKeyUp(KEY_LEFT) | !keyInfo->GetKeyUp(KEY_RIGHT) | !keyInfo->GetKeyUp(KEY_UP) | !keyInfo->GetKeyUp(KEY_UP);
	}
	return WE_NULL;
}
DWORD cButton::ActionEvent(CMouse * mouseInfo)
{
	DWORD we = WE_NULL;
	if (!m_bActive) return we;
	we |= cWindow::ActionEvent(mouseInfo);
	if (m_bDisable)
	{
		m_bClickInside = FALSE;
		m_pCurImage = &m_BasicImage;
		m_fgCurColor = m_fgBasicColor;
		return we;
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

	LONG x = mouseInfo->GetMouseX();
	LONG y = mouseInfo->GetMouseY();
	BOOL LeftUp = mouseInfo->LButtonUp();
	BOOL LeftPress = mouseInfo->LButtonPressed();
	if (we & (WE_LBTNCLICK | WE_LBTNDBLCLICK))
	{
		m_bClickInside = TRUE;
	}
	if (LeftUp)
	{
		if (m_bClickInside && PtInWindow(mouseInfo->GetMouseEventX(), mouseInfo->GetMouseEventY()))
		{
			(*cbWindowFunc)(m_ID, m_pParent, WE_BTNCLICK);
			we |= WE_BTNCLICK;
		}
		m_bClickInside = FALSE;
	}
	if (we & WE_MOUSEOVER)
	{
		if (LeftPress && m_bClickInside)
		{
			m_pCurImage = &m_PressImage;
			m_fgCurColor = m_fgPressColor;
		}
		else
		{
			m_pCurImage = &m_OverImage;
			m_fgCurColor = m_fgOverColor;
		}
	}
	else
	{
		if (LeftPress && m_bClickInside)
		{
			m_pCurImage = &m_OverImage;
			m_fgCurColor = m_fgOverColor;
		}
		else
		{
			m_pCurImage = &m_BasicImage;
			m_fgCurColor = m_fgBasicColor;
		}
	}
	return we;
}

extern HWND _g_hWnd;
#include "GameResourceManager.h"

void cButton::Render()
{
	if (m_bActive == FALSE) return;

	if (m_DebugEnabled)
	if (m_DebugBoolean[DebugHover] || m_DebugBoolean[DebugClicked])
	{
		DWORD inLine = RGBA_MERGE(RGB_HALF(255, 0, 0), 255);
		VECTOR2	TopLeft, TopRight, BottomLeft, BottomRight;
		GetVec2(m_absPos, m_width, m_height, TopLeft, TopRight, BottomLeft, BottomRight);
		if (m_DebugBoolean[DebugHover])
		{
			g_pExecutive->GetRenderer()->RenderLine(&TopLeft, &TopRight, inLine);
			g_pExecutive->GetRenderer()->RenderLine(&TopRight, &BottomRight, inLine);
			g_pExecutive->GetRenderer()->RenderLine(&BottomRight, &BottomLeft, inLine);
			g_pExecutive->GetRenderer()->RenderLine(&BottomLeft, &TopLeft, inLine);
		}
		if (m_DebugBoolean[DebugClicked])
		{
			inLine = RGBA_MERGE(RGB_HALF(0, 0, 255), 255);
			
			g_pExecutive->GetRenderer()->RenderLine(&TopLeft, &TopRight, inLine);
			g_pExecutive->GetRenderer()->RenderLine(&TopRight, &BottomRight, inLine);
			g_pExecutive->GetRenderer()->RenderLine(&BottomRight, &BottomLeft, inLine);
			g_pExecutive->GetRenderer()->RenderLine(&BottomLeft, &TopLeft, inLine);
			if (*m_szTextDebug != 0)
			{
				LONG tempX = m_absPos.x - 11;
				LONG tempY = m_absPos.y - 41;
				WORD nameFontID = 1;
				WORD nameFontPOS = 0;
			
				char IDtemp2[64] = { 0 };

				RECT rectDebugID = { (LONG)tempX, (LONG)tempY, 1, 1 };

				RECT rectDebug = { (LONG)tempX, (LONG)tempY + CFONT_OBJ->GetTextHeight(nameFontID), 1, 1 };

				sprintf(IDtemp2, "%s", (char*)WINIDSEARCH(m_SavedID));
				
				CFONT_OBJ->RenderFontWithShadow(nameFontID, IDtemp2, strlen(IDtemp2), &rectDebugID, inLine);

				inLine = RGBA_MERGE(RGB_HALF(255, 0, 0), 255);
				
				CFONT_OBJ->RenderFontWithShadow(nameFontPOS, m_szTextDebug, strlen(m_szTextDebug), &rectDebug, inLine);
			}
			if (m_DebugBoolean[DebugKDBActivated])
			{
				if (gCurTime - m_DebugTimerKDBActivated > 500)
				{
					if (g_UserInput.GetKeyboard()->GetKeyPressed(KEY_LEFT))
					{
						SetAbsXY(m_absPos.x - 1, m_absPos.y);
						m_SavedXY.x = m_SavedXY.x - 1;
						strncpy(m_szTextDebug, Vector2Text(m_SavedXY.x, m_SavedXY.y,  m_SavedWidthHeight[0], m_SavedWidthHeight[1]), strlen(Vector2Text(m_SavedXY.x, m_SavedXY.y,  m_SavedWidthHeight[0], m_SavedWidthHeight[1])) + 1);
					}
					if (g_UserInput.GetKeyboard()->GetKeyPressed(KEY_RIGHT))
					{
						SetAbsXY(m_absPos.x + 1, m_absPos.y);
						m_SavedXY.x = m_SavedXY.x + 1;
						strncpy(m_szTextDebug, Vector2Text(m_SavedXY.x, m_SavedXY.y,  m_SavedWidthHeight[0], m_SavedWidthHeight[1]), strlen(Vector2Text(m_SavedXY.x, m_SavedXY.y,  m_SavedWidthHeight[0], m_SavedWidthHeight[1])) + 1);
					}
					if (g_UserInput.GetKeyboard()->GetKeyPressed(KEY_UP))
					{
						SetAbsXY(m_absPos.x, m_absPos.y - 1);
						m_SavedXY.y = m_SavedXY.y - 1;
						strncpy(m_szTextDebug, Vector2Text(m_SavedXY.x, m_SavedXY.y,  m_SavedWidthHeight[0], m_SavedWidthHeight[1]), strlen(Vector2Text(m_SavedXY.x, m_SavedXY.y,  m_SavedWidthHeight[0], m_SavedWidthHeight[1])) + 1);
					}
					if (g_UserInput.GetKeyboard()->GetKeyPressed(KEY_DOWN))
					{
						SetAbsXY(m_absPos.x, m_absPos.y + 1);
						m_SavedXY.y = m_SavedXY.y + 1;
						strncpy(m_szTextDebug, Vector2Text(m_SavedXY.x, m_SavedXY.y,  m_SavedWidthHeight[0], m_SavedWidthHeight[1]), strlen(Vector2Text(m_SavedXY.x, m_SavedXY.y,  m_SavedWidthHeight[0], m_SavedWidthHeight[1])) + 1);
					}
					SetRelXY(m_SavedXY.x, m_SavedXY.y);
				}
			}
		}
	}
	cWindow::Render();

	
	if (m_nCurTextLen != 0)
	{
		LONG TxtPosX;
		if (m_nAlign & TXT_LEFT)
		{
			TxtPosX = (LONG)m_absPos.x + m_textXY.x;
		}
		else if (m_nAlign & TXT_RIGHT)
		{
			TxtPosX = (LONG)m_absPos.x + m_width - m_textXY.x
				- CFONT_OBJ->GetTextExtentEx(m_wFontIdx, m_szText, strlen(m_szText));
		}
		else
		{
			TxtPosX = (LONG)m_absPos.x + (m_width
				- CFONT_OBJ->GetTextExtentEx(m_wFontIdx, m_szText, strlen(m_szText))) / 2;
		}
		RECT rect = { TxtPosX, (LONG)m_absPos.y + m_textXY.y, 1, 1 };
		if (m_pCurImage == (&m_PressImage))
		{
			OffsetRect(&rect, m_BtnTextAniX, m_BtnTextAniY);
		}
		if (m_bShadow)
		{
			RECT rcShadow = rect;
			OffsetRect(&rcShadow, m_shadowTextXY.x, m_shadowTextXY.y);
			CFONT_OBJ->RenderFont(m_wFontIdx, m_szText, m_nCurTextLen, &rcShadow, RGBA_MERGE(m_shadowColor, m_alpha * m_dwOptionAlpha / 100));
		}
		CFONT_OBJ->RenderFont(m_wFontIdx, m_szText, m_nCurTextLen, &rect, RGBA_MERGE(m_fgCurColor, m_alpha * m_dwOptionAlpha / 100));			
	}

	
}
void cButton::SetText(char * text, DWORD basicColor, DWORD overColor, DWORD pressColor)
{
	ASSERT(MAX_TEXT_SIZE>strlen(text));
	m_nCurTextLen = strlen(text);
	strncpy(m_szText, text, m_nCurTextLen + 1);
	m_fgCurColor = m_fgBasicColor = m_fgOverColor = m_fgPressColor = basicColor;
	if (overColor)
		m_fgOverColor = overColor;
	if (pressColor)
		m_fgPressColor = pressColor;
}
void cButton::SetTextValue(DWORD value)
{
	wsprintf(m_szText, "%d", value);
	m_nCurTextLen = strlen(m_szText);
}
