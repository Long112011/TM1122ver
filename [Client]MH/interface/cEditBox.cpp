#include "stdafx.h"
#include "cEditBox.h"
#include <string>   
#include "ctype.h"  
#include "cScriptManager.h"     
#include "cWindowManager.h"
#include "cFont.h"
#include "../input/cIMEWnd.h"
#include "../cIMEex.h"
#include "../Input/Mouse.h"
#include "./Input/Keyboard.h"
#include "./Input/UserInput.h"
extern BOOL m_DebugEnabled;
extern char* WINIDSEARCH(int b);
cEditBox::cEditBox()
{
	m_nonactiveTextColor = 0xffffff;
	m_activeTextColor = 0xffffff;
	m_bInitEdit = FALSE;
	m_bSecret = FALSE;
	m_bTextChanged = 0;
	m_dwCaretCurTick = m_dwCaretLastTick = 0;
	m_type = WT_EDITBOX;
	m_bReadOnly = FALSE;
	m_bFaceImage = FALSE;
	m_pIMEex = new cIMEex;
	m_pIMEex->SetParentEdit(this);
	m_bCaret = FALSE;
	m_pEditText = NULL;
	m_bShowCaretInReadOnly = FALSE;
	m_nAlign = TXT_LEFT;
	m_lTextLeftOffset = 5;
	m_lTextRightOffset = 5;
	m_lTextTopOffset = 6;
	m_cbEditFunc = NULL;

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
cEditBox::~cEditBox()
{
	CIMEWND->SetDockingEx(m_pIMEex, FALSE);
	WINDOWMGR->SetFocusedEdit(this, FALSE);
	if (m_pIMEex)
	{
		m_pIMEex->Release();
		delete m_pIMEex;
		m_pIMEex = NULL;
	}
	if (m_pEditText)
	{
		delete[] m_pEditText;
		m_pEditText = NULL;
	}
}
void cEditBox::DebugPressed()
{
	m_DebugBoolean[DebugKDBActivated] = TRUE; m_DebugTimerKDBActivated = gCurTime;
}
void cEditBox::SetTextOffset(LONG Left, LONG Right, LONG Top)
{
	m_lTextLeftOffset = Left;
	m_lTextRightOffset = Right;
	m_lTextTopOffset = Top;
}
void cEditBox::Init(LONG x, LONG y, WORD wid, WORD hei, cImage* basicImage, cImage* focusImage, LONG ID)
{
	cWindow::Init(x, y, wid, hei, basicImage, ID);
	if (focusImage)
	{
		if (focusImage->IsNull())
		{
			if (basicImage)
				m_FocusImage = *basicImage;
		}
		else
		{
			m_FocusImage = *focusImage;
		}
	}
	else
	{
		if (basicImage)
			m_FocusImage = *basicImage;
	}

	//if (m_DebugEnabled)
	{
		m_SavedXY.x = x;
		m_SavedXY.y = y;
		m_SavedID = ID;
		m_SavedWidthHeight[0] = wid;
		m_SavedWidthHeight[1] = hei;
		strncpy(m_szTextDebug, Vector2Text(m_SavedXY.x, m_SavedXY.y,  m_SavedWidthHeight[0], m_SavedWidthHeight[1]), strlen(Vector2Text(m_SavedXY.x, m_SavedXY.y,  m_SavedWidthHeight[0], m_SavedWidthHeight[1])) + 1);
	}
	SetFocusEdit(FALSE);
}
void cEditBox::InitEditbox(WORD editBoxPixel, WORD BufBytes)
{
	m_bInitEdit = TRUE;
	m_pIMEex->Init(BufBytes, editBoxPixel, TRUE, 1);
	m_pEditText = new char[BufBytes + 1];
	ZeroMemory(m_pEditText, BufBytes + 1);
}
DWORD cEditBox::ActionEvent(CMouse * mouseInfo)
{
	DWORD we = WE_NULL;
	if (!m_bActive) return we;
	we = cWindow::ActionEvent(mouseInfo);
	if (m_bDisable) return we;
	if (mouseInfo->LButtonDown())
	{
		if (PtInWindow(mouseInfo->GetMouseEventX(), mouseInfo->GetMouseEventY()) && (we & WE_LBTNCLICK))
		{
			SetFocusEdit(TRUE);
			if (cbWindowFunc)
				(*cbWindowFunc)(m_ID, m_pParent, WE_SETFOCUSON);
			we |= WE_SETFOCUSON;
		}
		else
		{
			SetFocusEdit(FALSE);
		}
	}
	if (m_bTextChanged)
	{
		if (cbWindowFunc)
			(*cbWindowFunc)(m_ID, m_pParent, WE_CHANGETEXT);
		we |= WE_CHANGETEXT;
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
	}
	return we;
}
DWORD cEditBox::ActionKeyboardEvent(CKeyboard * keyInfo)
{
	//if (!m_bActive) return WE_NULL;
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
void cEditBox::SetActive(BOOL val)
{
	if (m_bDisable) return;
	cWindow::SetActive(val);
	if (!val)
		SetFocusEdit(FALSE);
}
void cEditBox::SetFocus(BOOL val)
{
	cWindow::SetFocus(val);
}
void cEditBox::SetFocusEdit(BOOL val)
{
	if (m_bFocus == val) return;
	if (val && !m_bActive) return;
	SetFocus(val);
	if (val)
	{
		if (!m_bReadOnly)
			CIMEWND->SetDockingEx(m_pIMEex, TRUE);
		else if (!m_bShowCaretInReadOnly)
			return;
		m_bCaret = TRUE;
		m_dwCaretLastTick = m_dwCaretCurTick = GetTickCount();
		WINDOWMGR->SetFocusedEdit(this, TRUE);
		m_pCurImage = &m_FocusImage;
		IMEEvent(eIMEE_SETFOCUS);
	}
	else
	{
		m_bCaret = FALSE;
		CIMEWND->SetDockingEx(m_pIMEex, FALSE);
		WINDOWMGR->SetFocusedEdit(this, FALSE);
		m_pCurImage = &m_BasicImage;
		IMEEvent(eIMEE_KILLFOCUS);
	}
}
void cEditBox::Render()
{
	cWindow::Render();
	char strBuf[256];
	int nStrLen;
	RECT rcText;
	LONG TxtPosX, TxtPosY;
	TxtPosY = (LONG)m_absPos.y + m_lTextTopOffset;
	if (m_bFocus && (!m_bReadOnly || m_bShowCaretInReadOnly))
	{
		for (int i = 0; i < m_pIMEex->GetTotalLine(); ++i)
		{
			m_pIMEex->GetLineText(i, strBuf, &nStrLen);
			if (m_bSecret)
			{
				memset(strBuf, '*', nStrLen);
			}
			if (m_nAlign & TXT_LEFT)
			{
				TxtPosX = (LONG)m_absPos.x + m_lTextLeftOffset;
			}
			else if (m_nAlign & TXT_RIGHT)
			{
				TxtPosX = (LONG)m_absPos.x + m_width
					- CFONT_OBJ->GetTextExtentEx(m_wFontIdx, strBuf, nStrLen) - m_lTextRightOffset;
			}
			else
			{
				TxtPosX = (LONG)m_absPos.x + (m_width
					- CFONT_OBJ->GetTextExtentEx(m_wFontIdx, strBuf, nStrLen)) / 2;
			}
			SetRect(&rcText, TxtPosX,
				TxtPosY + LINE_HEIGHT * i, 1, 1);
			if (m_bFaceImage)
			{
				RenderFaceImage(TxtPosX, TxtPosY + LINE_HEIGHT * i, strBuf);
				ClearFaceImg(strBuf, nStrLen);
			}
			CFONT_OBJ->RenderFont(m_wFontIdx, strBuf, nStrLen, &rcText, RGBA_MERGE(m_activeTextColor, m_alpha * m_dwOptionAlpha / 100));
			if (m_bCaret)
			{
				if (i == m_pIMEex->GetInsertPos()->nLine)
				{
					LONG lExtent = CFONT_OBJ->GetTextExtentEx(m_wFontIdx, strBuf, m_pIMEex->GetInsertPos()->nIndex);
					SetRect(&rcText, TxtPosX + lExtent - 2,
						TxtPosY + m_pIMEex->GetInsertPos()->nLine * LINE_HEIGHT, 1, 1);
					if (m_bFaceImage)
					{
						RenderFaceImage(TxtPosX, TxtPosY + m_pIMEex->GetInsertPos()->nLine * LINE_HEIGHT, strBuf);
						ClearFaceImg(strBuf, nStrLen);
					}
					CFONT_OBJ->RenderFont(m_wFontIdx, "|", 1, &rcText, RGBA_MERGE(0x00ffffff, m_alpha * m_dwOptionAlpha / 100));
				}
			}
		}
		m_dwCaretCurTick = GetTickCount();
		if (m_dwCaretCurTick - m_dwCaretLastTick >= 500)
		{
			m_bCaret = !m_bCaret;
			while (m_dwCaretCurTick - m_dwCaretLastTick >= 500)
				m_dwCaretLastTick += 500;
		}
	}
	else
	{
		for (int i = 0; i < m_pIMEex->GetTotalLine(); ++i)
		{
			m_pIMEex->GetLineText(i, strBuf, &nStrLen);
			if (m_bSecret)
			{
				memset(strBuf, '*', nStrLen);
			}
			if (m_nAlign & TXT_LEFT)
			{
				TxtPosX = (LONG)m_absPos.x + m_lTextLeftOffset;
			}
			else if (m_nAlign & TXT_RIGHT)
			{
				TxtPosX = (LONG)m_absPos.x + m_width
					- CFONT_OBJ->GetTextExtentEx(m_wFontIdx, strBuf, nStrLen) - m_lTextLeftOffset;
			}
			else
			{
				TxtPosX = (LONG)m_absPos.x + (m_width
					- CFONT_OBJ->GetTextExtentEx(m_wFontIdx, strBuf, nStrLen)) / 2;
			}
			SetRect(&rcText, TxtPosX, TxtPosY + LINE_HEIGHT * i, 1, 1);
			if (m_bFaceImage)
			{
				RenderFaceImage(TxtPosX, TxtPosY + LINE_HEIGHT * i, strBuf);
				ClearFaceImg(strBuf, nStrLen);
			}
			CFONT_OBJ->RenderFont(m_wFontIdx, strBuf, nStrLen, &rcText, RGBA_MERGE(m_nonactiveTextColor, m_alpha * m_dwOptionAlpha / 100));
		}
	}
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
void cEditBox::SetAbsXY(LONG x, LONG y)
{
	cWindow::SetAbsXY(x, y);
}
char* cEditBox::GetEditText()
{
	if (m_pIMEex)
	{
		if (m_pIMEex->GetTotalLine() > 1)
			m_pIMEex->GetScriptText(m_pEditText);
		else
			m_pIMEex->GetLineText(0, m_pEditText);
	}
	else
		m_pEditText[0] = 0;
	return m_pEditText;
}
void cEditBox::SetEditText(char * text)
{
	if (m_bInitEdit)
	{
		m_pIMEex->SetScriptText(text);
	}
}
void cEditBox::SetValidCheck(int nSetNum)
{
	if (m_pIMEex)
		m_pIMEex->SetValidCheckMethod(nSetNum);
}
int cEditBox::GetValidCheckMethod()
{
	if (m_pIMEex)
		return m_pIMEex->GetValidCheckMethod();
	return 0;
}
void cEditBox::SetAlign(int nAlign)
{
	m_nAlign = nAlign;
}
void cEditBox::SetDisable(BOOL val)
{
	m_bDisable = val;
	if (val)
		SetFocusEdit(FALSE);
}
void cEditBox::IMEEvent(DWORD dwEvent)
{
	if (m_cbEditFunc)
		m_cbEditFunc(m_ID, this, dwEvent);
}
void cEditBox::GetCaretPos(LONG* X, LONG* Y)
{
	LONG TxtPosY = (LONG)m_absPos.y + m_lTextTopOffset;
	LONG TxtPosX = 0;
	char strBuf[256] = { 0, };
	int nStrLen = 0;
	if (m_pIMEex->GetLineText(m_pIMEex->GetInsertPos()->nLine, strBuf, &nStrLen) == FALSE)
	{
		*X = 0;
		*Y = 0;
		return;
	}
	if (m_nAlign & TXT_LEFT)
	{
		TxtPosX = (LONG)m_absPos.x + m_lTextLeftOffset;
	}
	else if (m_nAlign & TXT_RIGHT)
	{
		TxtPosX = (LONG)m_absPos.x + m_width
			- CFONT_OBJ->GetTextExtentEx(m_wFontIdx, strBuf, nStrLen) - m_lTextRightOffset;
	}
	else
	{
		TxtPosX = (LONG)m_absPos.x + (m_width
			- CFONT_OBJ->GetTextExtentEx(m_wFontIdx, strBuf, nStrLen)) / 2;
	}
	LONG lExtent = CFONT_OBJ->GetTextExtentEx(m_wFontIdx, strBuf, m_pIMEex->GetInsertPos()->nIndex);
	*X = TxtPosX + lExtent - 2;
	*Y = TxtPosY + m_pIMEex->GetInsertPos()->nLine * LINE_HEIGHT;
}
void cEditBox::RenderFaceImage(LONG X, LONG Y, char * StaticText)
{
	std::string Buffer = StaticText;
	if (Buffer.length()>0)
	{
		int FindStartPos = 0;
		while (FindStartPos >= 0 && FindStartPos< Buffer.length())
		{
			int FindPos = Buffer.find('&', FindStartPos);
			if (FindPos >= 0)
			{
				std::basic_string <char> ImgStr = Buffer.substr(FindPos + 1, 2);
				std::basic_string <char> FlagStr = Buffer.substr(0, FindPos);
				int ImageIdx = atoi(ImgStr.c_str());
				if (ImageIdx >= 0 && ImageIdx < 70)
				{
					if (SCRIPTMGR->GetEmoji(ImageIdx))
					{
						int Rate = 5;
						int Index = 0;
						SCRIPTMGR->GetImage(SCRIPTMGR->GetEmoji(ImageIdx)->ImageIdx, &pNameImage[Index], &SCRIPTMGR->GetEmoji(ImageIdx)->rect[Index]);
						if (!pNameImage[Index].IsNull())
						{
							VECTOR2 p_Scal, p_Pos;
							p_Scal.x = p_Scal.y = 0.9f;
							p_Pos.x = X + 1.0f + (FlagStr.length()) * 6;
							p_Pos.y = Y - 2.6f;
							pNameImage[Index].RenderSprite(0, NULL, 0, &p_Pos, 0xffffffff);
						}
					}
				}
				FindStartPos = FindPos + 2;
			}
			else
			{
				break;
			}
		}
	}
}
void cEditBox::SetRenderFaceImg(BOOL val)
{
	m_bFaceImage = val;
}
void cEditBox::ClearFaceImg(char * StaticText, int StrLen)
{
	std::string Buffer = StaticText;
	if (Buffer.length()>0)
	{
		int FindStartPos = 0;
		while (FindStartPos >= 0 && FindStartPos< Buffer.length())
		{
			int FindPos = Buffer.find('&', FindStartPos);
			if (FindPos >= 0)
			{
				Buffer.replace(FindPos, 3, "   ");
				FindStartPos = FindPos + 2;
			}
			else
			{
				break;
			}
		}
	}
	memset(StaticText, 0, StrLen);
	memcpy(StaticText, Buffer.c_str(), StrLen);
}