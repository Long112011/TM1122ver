#ifndef _cBUTTON_H_
#define _cBUTTON_H_
#endif
#pragma once
#include "cWindow.h"
#include "cWindowDef.h"

class cButton : public cWindow
{
public:
	cButton();
	virtual ~cButton();
	virtual void Init(LONG x, LONG y, WORD wid, WORD hei, cImage * basicImage, cImage * overImage, cImage * pressImage, cbFUNC Func, LONG ID = 0);
	virtual DWORD ActionEvent(CMouse * mouseInfo);
	virtual void Render();
	void SetText(char * text, DWORD basicColor, DWORD overColor = NULL, DWORD PressColor = NULL);
	void SetTextValue(DWORD value);
	void SetTextXY(LONG x, LONG y)			{ m_textXY.x = x; m_textXY.y = y; }
	BOOL IsClickInside()					{ return m_bClickInside; }
	void SetTextAni(BYTE x, BYTE y)			{ m_BtnTextAniX = x; m_BtnTextAniY = y; }
	void SetClickSound(SNDIDX idx)			{ m_sndIdx = idx; m_bSound = TRUE; }
	void SetShadowTextXY(LONG x, LONG y)	{ m_shadowTextXY.x = x; m_shadowTextXY.y = y; }
	void SetShadowColor(DWORD color)		{ m_shadowColor = color; }
	void SetShadow(BOOL val)				{ m_bShadow = val; }
	void SetAlign(int nAlign)				{ m_nAlign = nAlign; }

	virtual DWORD ActionKeyboardEvent(CKeyboard * keyInfo);

protected:
	cImage	m_OverImage;
	cImage	m_PressImage;
	cCoord	m_textXY;
	int		m_nCurTextLen;
	char	m_szText[MAX_TEXT_SIZE];
	DWORD	m_fgBasicColor;
	DWORD	m_fgOverColor;
	DWORD	m_fgPressColor;
	DWORD	m_fgCurColor;
	BOOL	m_bClickInside;
	BYTE	m_BtnTextAniX;
	BYTE	m_BtnTextAniY;
	int		m_nAlign;
	SNDIDX	m_sndIdx;
	BOOL	m_bSound;
	BOOL	m_bShadow;
	cCoord	m_shadowTextXY;
	DWORD	m_shadowColor;
	
	

	char	m_szTextDebug[MAX_TEXT_SIZE];
	VECTOR2	m_SavedXY;
	WORD	m_SavedWidthHeight[2];
	int		m_SavedID;
	DWORD	m_DebugTimerKDBActivated;
	BOOL	m_DebugBoolean[4];
	void	DebugPressed();
	
};