#include "stdafx.h"
#include "cCheckBox.h"
#include "cFont.h"
#include "./Input/Mouse.h"

#include "cWindowManager.h"
#include "./Input/Keyboard.h"
#include "./Input/UserInput.h"
extern BOOL m_DebugEnabled;
extern char* WINIDSEARCH(int b);
cCheckBox::cCheckBox()
{
	memset(m_szCheckBoxText, 0, MAX_CHECKBOXTEXT_SIZE);
	m_dwCheckBoxTextColor = RGB_HALF(255, 255, 255);
	m_fChecked = FALSE;
	m_type = WT_CHECKBOX;

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
cCheckBox::~cCheckBox()
{
}
void cCheckBox::DebugPressed()
{
	m_DebugBoolean[DebugKDBActivated] = TRUE; m_DebugTimerKDBActivated = gCurTime;
}
void cCheckBox::Init(LONG x, LONG y, WORD wid, WORD hei, cImage * basicImage, cImage * checkBoxImage, cImage * checkImage, cbFUNC Func, LONG ID)
{
	cWindow::Init(x, y, wid, hei, basicImage, ID);
	m_type = WT_CHECKBOX;
	m_CheckBoxImage = *checkBoxImage;
	m_CheckImage = *checkImage;
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
DWORD cCheckBox::ActionEvent(CMouse * mouseInfo)
{
	DWORD we = WE_NULL;
	if (!m_bActive) return we;
	we |= cWindow::ActionEvent(mouseInfo);
	if (m_bDisable)
	{
		return we;
	}
	if (m_DebugEnabled)
	{
		if (we & (WE_LBTNCLICK | WE_LBTNDBLCLICK))
		{
			m_DebugBoolean[DebugIsStillDown] = TRUE;			
		}		
		if (mouseInfo->LButtonUp())
		{			
			m_DebugBoolean[DebugIsStillDown] = FALSE;
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
		//return we;
	}



	if (we & WE_LBTNCLICK)
	{
		if (PtInWindow(mouseInfo->GetMouseEventX(), mouseInfo->GetMouseEventY()))
		{
			m_fChecked ^= TRUE;
			(*cbWindowFunc)(m_ID, m_pParent, (m_fChecked ? WE_CHECKED : WE_NOTCHECKED));
		}
	}

	
	return we;
}
DWORD cCheckBox::ActionKeyboardEvent(CKeyboard * keyInfo)
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
void cCheckBox::Render()
{
	VECTOR2 start_pos;
	start_pos.x = m_absPos.x;
	start_pos.y = m_absPos.y;
	m_CheckBoxImage.RenderSprite(NULL, NULL, 0, &start_pos, RGBA_MERGE(m_dwImageRGB, m_alpha * m_dwOptionAlpha / 100));
	if (m_fChecked)
		m_CheckImage.RenderSprite(NULL, NULL, 0, &start_pos, RGBA_MERGE(m_dwImageRGB, m_alpha * m_dwOptionAlpha / 100));
	RECT rect = { (LONG)m_absPos.x + 15, (LONG)m_absPos.y + 6, 1, 1 };
	CFONT_OBJ->RenderFont(m_wFontIdx, m_szCheckBoxText, lstrlen(m_szCheckBoxText), &rect, RGBA_MERGE(m_dwCheckBoxTextColor, m_alpha * m_dwOptionAlpha / 100));
	cWindow::Render();

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
}
