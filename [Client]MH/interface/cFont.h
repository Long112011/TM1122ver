



// cFont.h: interface for the cFont class.
//
//////////////////////////////////////////////////////////////////////

#ifndef _cFONT_H_
#define _cFONT_H_


#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//#include "cWindowDef.h"
#include "../Engine/GraphicEngine.h"
#include "../4DyuchiGRX_common/IGeometry.h"



#define CFONT_OBJ	USINGTON(cFont)

typedef struct _FONT_DESC

{
	WORD wWidth;
	WORD wHeight;
	//WORD wHalfWidth;
	//WORD wHalfHeight;
}FONT_DESC;


class cFont
{
	HDC m_hdc;
public:
	//enum FONT_KIND{FONT0=0, FONT1, FONT2, FONT3, FONT4, FONT5, FONTMAX};
	//enum FONT_KIND{ FONT0 = 0, FONT1, FONT2, FONT3, FONT4, FONT5, FONT6, FONT7, FONT8, FONT9, FONT10, FONT11, FONT12, FONT13, FONT14, FONT15, FONT16, FONT17, FONT18, FONT19, FONTMAX = 20 };  //miningmso  ñòÊ¥ÞÌ?í®?ïÒ? 2018-05-09
	cFont();

	virtual ~cFont();

	void Init(int FontCount);
	void Release();
	// GetInstance()
	//	MAKESINGLETON(cFont);
	BOOL CreateFontObject(LOGFONT *pFont, WORD idx);
	void RenderFont(WORD wFontIdx, char * text, int size, RECT * pRect, DWORD color);
	void RenderFontRegen(WORD wFontIdx, LONG x, LONG y, DWORD dwColor, LPCTSTR str, ...);
	void RenderFontAlpha(WORD wFontIdx, char * text, int size, RECT * pRect, DWORD color);

	void RenderFontWithShadow(WORD wFontIdx, char * text, int size, RECT * pRect, DWORD color, DWORD shadowColor=RGBA_MERGE(RGB_HALF(70, 70, 70), 180));

	IDIFontObject * GetFont(WORD idx)
	{
#ifdef _DEBUG

		if (idx>FONTMAX || idx<0) return NULL;
#endif //_DEBUG
		return m_pFont[idx];

	}
	BOOL IsCreateFont(WORD idx)


	{
		if (idx>m_FontMaxCount || idx<0) return FALSE;
		if (m_pFont[idx]) return TRUE;
		else return FALSE;
	}
	LONG GetTextExtent(WORD hFIdx, char *str, LONG strSize);
	LONG GetTextHeight(WORD hFIdx) { return m_FontDesc[hFIdx].wHeight; }
	LONG GetTextWidth(WORD hFIdx) { return m_FontDesc[hFIdx].wWidth; }
	IDIFontObject * m_pFont[FONTMAX];
	FONT_DESC m_FontDesc[FONTMAX];

	//KES 030915
	HFONT m_hFont[FONTMAX];
	LOGFONT m_LogFont[FONTMAX];

	int	m_FontMaxCount;
	LONG GetTextExtentEx(WORD hFIdx, char* str, int strLen);

	LONG GetTextExtentWidth(WORD hFIdx, char* str, int strLen);
	void RenderNoticeMsg(WORD wFontIdx, char * text, int size, RECT * pRect, DWORD frontColor, DWORD backColor);

	void RenderText(WORD wFontIdx, char * text, int size, RECT * pRect, DWORD frontColor, DWORD backColor);
};

EXTERNGLOBALTON(cFont);
#endif // _cFONT_H_


