#include "stdafx.h"
#include "cWindow.h"
#include "cFont.h"
#include "cWindowManager.h"
#include "../Input/Mouse.h"
#include "MouseCursor.h"
#include "./Input/UserInput.h"
#include "GameIn.h"
#include "SendEquipSelectDialog.h"
#include "chatManager.h"
#include "shoutchatdialog.h"
#include "maingame.h"
#include "cScriptManager.h"
char	m_szTextDebugMouse[MAX_TEXT_SIZE];
extern BOOL jTweakToggle;
extern BOOL m_DebugEnabledAll;
extern char* WINIDSEARCH(int b);
void defaultWindowFunc(LONG lId, void * p, DWORD we){}
int i = 0;
BOOL gorender;
BOOL tiprender;
bool bAllowTooltipRender = false;
extern double CurFPS;
#define WINDOWTIMER 200
cWindow::cWindow()
: m_WindowEvent(0)
{
	m_ID = 0;
	m_wFontIdx = 0;
	m_type = WT_WINDOW;
	m_bActive = TRUE;
	m_bDisable = FALSE;
	m_bFocus = FALSE;
	m_bMovable = FALSE;
	m_width = 0;
	m_height = 0;
	m_pCurImage = NULL;
	m_pToolTipImage = NULL;
	m_bDepend = FALSE;
	m_pParent = NULL;
	m_dwViewCurTime = m_dwViewLastTime = 0;
	m_dwViewHoldCurTime = m_dwViewHoldLastTime = 0;
	m_dwImageRGB = 0xffffff;
	m_alpha = 255;
	m_dwOptionAlpha = 100;
	m_validPos.x = 0;
	m_validPos.y = 0;
	cbWindowFunc = defaultWindowFunc;
	m_scale.x = 1.f;
	m_scale.y = 1.f;
	m_bAlwaysTop = FALSE;
	m_bDestroy = FALSE;
	m_bSetTopOnActive = TRUE;
	m_pRelationTip = NULL;
	m_pRelationTip1 = NULL;
	m_pToolTipLink = NULL;
	gorender = FALSE;
	tiprender = FALSE;
	m_bCheckDispPoint = FALSE;
	m_renderRect.left = 0;
	m_renderRect.top = 0;
	m_renderRect.right = 0;
	m_renderRect.bottom = 0;
	m_renderShadowRect = m_renderRect;
	ZeroMemory(m_vDeltaPos, sizeof(m_vDeltaPos));
	ZeroMemory(
		&m_vDispPoint,
		sizeof(m_vDispPoint));
	nWindowBase = 1;
	m_OffSetX = 0;
	m_OffSetY = 0;
	*m_szTextDebugMouse = 0;
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
cWindow::~cWindow()
{
	toolTip.Release();
	StaticToolTip.Release();
	m_pRelationTip = NULL;
	m_pRelationTip1 = NULL;
	m_pToolTipLink = NULL;
}
void cWindow::SaveDispPoint(VECTOR2 point1, VECTOR2 point2, VECTOR2 point3)
{
	m_bCheckDispPoint = TRUE;
	memcpy(&m_vDispPoint[0], &point1, sizeof(VECTOR2));
	memcpy(&m_vDispPoint[1], &point2, sizeof(VECTOR2));
	memcpy(&m_vDispPoint[2], &point3, sizeof(VECTOR2));
}
void cWindow::Init(LONG x, LONG y, WORD wid, WORD hei, cImage * pBasicImage, LONG ID)
{
	if (m_bCheckDispPoint)
	{
		DISPLAY_INFO	dispInfo;
		GAMERESRCMNGR->GetDispInfo(&dispInfo);
		switch (nWindowBase)
		{
		case 1:
		{
				  x = 0;
				  y = 0;
		}
			break;
		case 2:
		{
				  x = ((float)dispInfo.dwWidth / 2 - wid / 2);
				  y = 0;
		}
			break;
		case 3:
		{
				  x = ((float)dispInfo.dwWidth - wid);
				  y = m_absPos.y;
		}
			break;
		case 4:
		{
				  x = 0;
				  y = ((float)dispInfo.dwHeight / 2 - hei / 2);
		}
			break;
		case 5:
		{
				  x = ((float)dispInfo.dwWidth / 2 - wid / 2);
				  y = ((float)dispInfo.dwHeight / 2 - hei / 2);
		}
			break;
		case 6:
		{
				  x = ((float)dispInfo.dwWidth - wid);
				  y = ((float)dispInfo.dwHeight / 2 - hei / 2);
		}
			break;
		case 7:
		{
				  x = m_absPos.x;
				  y = ((float)dispInfo.dwHeight - hei);
		}
			break;
		case 8:
		{
				  x = ((float)dispInfo.dwWidth / 2 - wid / 2);
				  y = (float)dispInfo.dwHeight - hei;
		}
			break;
		case 9:
		{
				  x = (float)dispInfo.dwWidth - wid;
				  y = (float)dispInfo.dwHeight - hei;
		}
			break;
		}
		SetAbsXY(x + m_OffSetX, y + m_OffSetY);
		SetRelXY(x + m_OffSetX, y + m_OffSetY);
		SetWH(wid, hei);
		m_bCheckDispPoint = FALSE;
	}
	else
	{
		SHORT_RECT rect = { 0 };
		WORD value = 0;
		g_pExecutive->GetRenderer()->GetClientRect(&rect, &value, &value);
		x += (LONG)m_vDeltaPos[0].x;
		y += (LONG)m_vDeltaPos[0].y;
		SetAbsXY(x, y);
		SetRelXY(x, y);
		SetWH(wid, hei);
	}
	m_ID = ID;
	if (pBasicImage)
	{
		m_BasicImage = *pBasicImage;
		m_pCurImage = &m_BasicImage;
	}
	m_renderRect.left = x;
	m_renderRect.top = y;
	m_renderRect.right = x + wid;
	m_renderRect.bottom = y + hei;
	m_renderShadowRect = m_renderRect;
	{
		m_SavedXY.x = x;
		m_SavedXY.y = y;
		m_SavedID = ID;
		m_SavedWidthHeight[0] = wid;
		m_SavedWidthHeight[1] = hei;
		strncpy(m_szTextDebug, Vector2Text(m_SavedXY.x, m_SavedXY.y, m_SavedWidthHeight[0], m_SavedWidthHeight[1]), strlen(Vector2Text(m_SavedXY.x, m_SavedXY.y, m_SavedWidthHeight[0], m_SavedWidthHeight[1])) + 1);
	}
}
void cWindow::SetDeltaPos(int nIndex, VECTOR2 point)
{
	m_vDeltaPos[0] = point;
}
BOOL cWindow::PtInWindow(LONG x, LONG y)
{
	if (m_absPos.x + m_validPos.x > x || m_absPos.y + m_validPos.y > y || m_absPos.x + m_validPos.x + m_width < x || m_absPos.y + m_validPos.y + m_height < y)
		return FALSE;
	else
		return TRUE;
}
DWORD cWindow::ActionEvent(CMouse * mouseInfo)
{
	m_WindowEvent = WE_NULL;
	if (!m_bActive || m_bDestroy) return m_WindowEvent;
	if (m_DebugEnabledAll)
	{
		if (m_WindowEvent & WE_MOUSEOVER)
		{
			sprintf(m_szTextDebugMouse, "%d %d", mouseInfo->GetMouseX(), mouseInfo->GetMouseY());
			SetAbsXY(m_absPos.x - MOUSE->GetMouseAxisX(), m_absPos.y - MOUSE->GetMouseAxisY());
			m_SavedXY.x = m_SavedXY.x - MOUSE->GetMouseAxisX();
			m_SavedXY.y = m_SavedXY.y - MOUSE->GetMouseAxisY();
			SetRelXY(m_SavedXY.x, m_SavedXY.y);
			strncpy(m_szTextDebug, Vector2Text(m_SavedXY.x, m_SavedXY.y, m_SavedWidthHeight[0], m_SavedWidthHeight[1]), strlen(Vector2Text(m_SavedXY.x, m_SavedXY.y, m_SavedWidthHeight[0], m_SavedWidthHeight[1])) + 1);
		}
	}
	if (!WINDOWMGR->IsMouseOverUsed() && PtInWindow(mouseInfo->GetMouseX(), mouseInfo->GetMouseY()))
	{
		if (!(WINDOWMGR->IsDragWindow() && (cWindow*)WINDOWMGR->GetDragDlg() == this))
		{
			m_WindowEvent |= WE_MOUSEOVER;
			m_dwViewCurTime = GetTickCount();
			if (m_dwViewCurTime - m_dwViewLastTime > WINDOWTIMER)
			{
				tiprender = TRUE;
				m_ttX = mouseInfo->GetMouseX();
				m_ttY = mouseInfo->GetMouseY();
				//m_ttX = m_absPos.x;
				//m_ttY = m_absPos.y;
			}
		}
		if (!WINDOWMGR->IsDragWindow())
		{
			if (m_type & WT_ICON)
				CURSOR->SetCursor(eCURSOR_DEFAULT);
		}
	}
	else
	{
		tiprender = FALSE;
		if (gorender)
			m_dwViewLastTime = m_dwViewCurTime = 0;
		else
			m_dwViewLastTime = m_dwViewCurTime = GetTickCount();
	}
	if (!WINDOWMGR->IsMouseDownUsed())
	if (PtInWindow(mouseInfo->GetMouseEventX(), mouseInfo->GetMouseEventY()))
	{
		if (mouseInfo->LButtonDown())
			m_WindowEvent |= WE_LBTNCLICK;
		if (mouseInfo->RButtonDown())
			m_WindowEvent |= WE_RBTNCLICK;
		if (mouseInfo->LButtonDoubleClick())
			m_WindowEvent |= WE_LBTNDBLCLICK;
		if (mouseInfo->RButtonDoubleClick())
			m_WindowEvent |= WE_RBTNDBLCLICK;
	}
	return m_WindowEvent;
}
#define COMPAREEQSIZE 258
void cWindow::ToolTipRender()
{
	if (!toolTip.IsValid() /*|| !StaticToolTip.IsValid()*/)
	{
		static DWORD timetick;
		if (GetTickCount() - timetick > WINDOWTIMER)
		{
			timetick = GetTickCount();
			gorender = FALSE;
		}
		return;
	}
	if ((toolTip.IsValid() /*|| StaticToolTip.IsValid()*/) && m_dwViewCurTime - m_dwViewLastTime > WINDOWTIMER)
	{
		gorender = TRUE;
		WINDOWMGR->SetToolTipWindow(&toolTip);
		toolTip.SetXY(m_ttX + TOOLTIP_MOUSESIZE_X, m_ttY + TOOLTIP_MOUSESIZE_Y);
		//WINDOWMGR->SetStaticToolTip(&toolTip);
		//StaticToolTip.SetXY(m_absPos.x, m_absPos.y);
		if (m_pRelationTip)
		{
			if (g_UserInput.GetKeyboard()->GetKeyPressed(KEY_CONTROL))
			if (m_pRelationTip->IsValid())
			{
				WINDOWMGR->SetRelationTipWindow(m_pRelationTip);
				m_pRelationTip->SetXY(m_ttX - COMPAREEQSIZE, m_ttY + TOOLTIP_MOUSESIZE_Y);
				m_pRelationTip->SetShowCurItem(TRUE);
			}
		}
		if (m_pRelationTip1)
		{
			if (g_UserInput.GetKeyboard()->GetKeyPressed(KEY_CONTROL))
			if (m_pRelationTip1->IsValid())
			{
				WINDOWMGR->SetRelationTipWindow1(m_pRelationTip1);
				m_pRelationTip1->SetXY(m_ttX - COMPAREEQSIZE, m_ttY - m_pRelationTip1->GetHight());
				m_pRelationTip1->SetShowCurItem(TRUE);
			}
		}
	}
}
void cWindow::ExcuteCBFunc(DWORD we)
{
	if (cbWindowFunc)
		(*cbWindowFunc)(m_ID, m_pParent, we);
}
extern BOOL g_bActiveApp;
void cWindow::TestRender()
{
}
void cWindow::Render()
{
	if (m_bActive)
	if (m_pCurImage)
	{
		DWORD alpha = m_alpha * m_dwOptionAlpha / 100;
		DWORD dwRGBA = RGBA_MERGE(m_dwImageRGB, alpha);
		m_pCurImage->RenderSprite(&m_scale, NULL, 0, &m_absPos, dwRGBA);
		ToolTipRender();
	}
	else
	{
		if (jTweakToggle)
			i = 0;
	}
	if (m_DebugEnabledAll)
	{
		DWORD inLine = RGBA_MERGE(RGB_HALF(255, 0, 0), 255);
		{
			inLine = RGBA_MERGE(RGB_HALF(0, 0, 255), 255);
			if (*m_szTextDebugMouse != 0)
			{
				LONG tempX = m_absPos.x - 11;
				LONG tempY = m_absPos.y - 41;
				WORD nameFontPOS = 0;
				WORD nameFontID = 1;
				RECT rectDebug = { (LONG)tempX, (LONG)tempY + CFONT_OBJ->GetTextHeight(nameFontID), 1, 1 };
				inLine = RGBA_MERGE(RGB_HALF(255, 0, 0), 255);
				RECT rectDebugID = { (LONG)tempX, (LONG)tempY, 1, 1 };
				CFONT_OBJ->RenderFontWithShadow(nameFontPOS, m_szTextDebugMouse, strlen(m_szTextDebug), &rectDebug, inLine);
			}
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
		}
	}
}
void cWindow::SetToolTip(char* msg, DWORD color, cImage * image, DWORD imgColor)
{
	if (msg)
	{
		toolTip.Init(0, color, image, imgColor);
		toolTip.SetImageAlpha(TOOLTIP_ALPHA);
		toolTip = msg;
		//StaticToolTip.Init(0, color, image, imgColor);
		//StaticToolTip.SetImageAlpha(TOOLTIP_ALPHA);
		//StaticToolTip = msg;
	}
}
void cWindow::AddToolTipLine(char* msg, DWORD color, cImage * m_LineImage, WORD m_FontIdx, BYTE bWhichLine)
{
	if (jTweakToggle)
	{
		char bTemp[512] = { 0, };
		sprintf(bTemp, "%s	[%d]", msg, i++);
		toolTip.AddLine(bTemp, color, m_LineImage, m_FontIdx, bWhichLine);
	}
	else
		toolTip.AddLine(msg, color, m_LineImage, m_FontIdx, bWhichLine);
}
void cWindow::AddToolTipLine2(char* msg, char* msg2, DWORD color, DWORD color2, cImage * m_LineImage, WORD m_FontIdx, BYTE bWhichLine)
{
	toolTip.AddLine2(msg, msg2, color, color2, m_LineImage, m_FontIdx, bWhichLine);
}
void cWindow::AddToolTipImageStone(cImage * image, int Index)
{
	toolTip.AddImageStone(image, Index);
}
void cWindow::AddToolTipImageEquip(cImage * image, cImage * image2)
{
	i = 0;
	toolTip.AddImageEquip(image, image2);
}
void cWindow::SetEquipItem()
{
	toolTip.SetEquipItem(TRUE);
}
void cWindow::SetAutoLineToolTip(const char* text, DWORD color, cImage* image, DWORD width, cImage * imageOne, cImage * imageTwo)
{
	i = 0;
	SetToolTip("", color, image, color);
	toolTip.AddImageEquip(imageOne, imageTwo);
	const DWORD fontSize = 6;
	BYTE line[MAX_TEXT_SIZE];
	int length = _mbslen((BYTE*)text);
	for (const BYTE* ch = (BYTE*)text;
		0 < length;
		)
	{
		_mbsnbcpy(line, ch, width);
		line[width] = 0;
		AddToolTipLine((char*)line, color);
		const DWORD lineLength = _mbslen(line);
		for (DWORD i = 0; i < lineLength; ++i)
		{
			ch = _mbsinc(ch);
		}
		length -= lineLength;
	}
}
WORD cWindow::GetTipLineCount()
{
	return toolTip.GetLineCount();
}
void cWindow::IniTipLineCount()
{
}
void cWindow::GetVariablePostion(SIZE& standardResolution, POINT& standardPosition, SIZE& currentResolution)
{
	const SIZE standardResolutionArray[] = {
		{ 800, 600 },
		{ 1024, 768 },
		{ 1280, 1024 },
	};
	LONG minimumSpace = LONG_MAX;
	SHORT_RECT rect = { 0 };
	WORD value = 0;
	g_pExecutive->GetRenderer()->GetClientRect(
		&rect,
		&value,
		&value);
	currentResolution.cx = rect.right;
	currentResolution.cy = rect.bottom;
	for (int i = sizeof(standardResolutionArray) / sizeof(*standardResolutionArray);
		0 < i--;)
	{
		const SIZE& resolution = standardResolutionArray[i];
		const VECTOR2& position = m_vDispPoint[i];
		const VECTOR2 emptyPosition = { -1, -1 };
		if (0 == memcmp(
			&position,
			&emptyPosition,
			sizeof(position)))
		{
			continue;
		}
		const LONG space = abs(resolution.cy - LONG(rect.bottom));
		if (minimumSpace < space)
		{
			continue;
		}
		minimumSpace = space;
		standardPosition.x = LONG(position.x);
		standardPosition.y = LONG(position.y);
		standardResolution = resolution;
	}
	if (standardResolution.cy > rect.right)
	{
		const float widthRatio = float(rect.bottom) / standardResolution.cx;
		const float heightRatio = float(rect.right) / standardResolution.cy;
		standardPosition.x = LONG(float(standardPosition.x) * widthRatio);
		standardPosition.y = LONG(float(standardPosition.y) * heightRatio);
		standardResolution.cx = rect.bottom;
		standardResolution.cy = rect.right;
	}
}