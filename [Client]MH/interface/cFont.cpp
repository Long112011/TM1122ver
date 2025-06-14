#include "stdafx.h"
#include "cFont.h"
#include "GameResourceManager.h"
extern HWND _g_hWnd;
GLOBALTON(cFont);
cFont::cFont() : m_FontMaxCount(0)
{
	for (int i = 0; i<FONTMAX; i++)
	{
		m_pFont[i] = NULL;
		m_hFont[i] = NULL;
	}
	ZeroMemory(m_LogFont, sizeof(m_LogFont));
	ZeroMemory(m_pFont, sizeof(m_pFont));
	ZeroMemory(m_hFont, sizeof(m_hFont));
}
cFont::~cFont()
{
	Release();
	ReleaseDC(_g_hWnd, m_hdc);
}
void cFont::Release()
{
	for (int i = 0; i<FONTMAX; i++)
	{
		if (m_pFont[i])
			SAFE_RELEASE(m_pFont[i]);
		if (m_hFont[i])
			DeleteObject(m_hFont[i]);
	}
}







BOOL cFont::CreateFontObject(LOGFONT * pFont, WORD idx)
{
	if (/*idx > FONTMAX || */idx < 0) return FALSE;
	m_pFont[idx] = g_pExecutive->GetRenderer()->CreateFontObject(pFont, SS3D_FONT);
	m_LogFont[idx] = *pFont;
	m_hFont[idx] = CreateFontIndirect(pFont);

	m_FontDesc[idx].wWidth = (unsigned short)pFont->lfWidth;
	m_FontDesc[idx].wHeight = (unsigned short)(-pFont->lfHeight);

	m_FontMaxCount++;
	return TRUE;
}
/*BOOL cFont::CreateFontObject(LOGFONT * pFont, WORD idx)
{
if(idx > FONTMAX || idx < 0) return FALSE;
m_pFont[idx] = g_pExecutive->GetRenderer()->CreateFontObject(pFont, SS3D_FONT);
m_LogFont[idx] = *pFont;
m_hFont[idx] = CreateFontIndirect( pFont );

m_FontDesc[idx].wWidth = (unsigned short)pFont->lfWidth;
m_FontDesc[idx].wHeight = (unsigned short)(-pFont->lfHeight);

m_FontMaxCount++;
return TRUE;
}*/






LONG cFont::GetTextExtent(WORD hFIdx, char * str, LONG strSize)
{
	if (hFIdx > m_FontMaxCount || hFIdx < 0)
		hFIdx = 0;

	WORD wid = m_FontDesc[hFIdx].wWidth;
	if (hFIdx /*== 6*/)
	{
		wid += 2;
	}
	WORD DBCNT = 0;
	WORD SBCNT = 0;
	for (int i = 0; i < strSize; i++)
	{
		if (IsDBCSLeadByte(str[i]))
		{
			DBCNT++;
			i++;
		}
		else
		{
			SBCNT++;
		}
	}
	LONG TextWidth = SBCNT*wid + DBCNT * 2 * wid;
	return TextWidth;
}
/*LONG cFont::GetTextExtent(WORD hFIdx, char * str, LONG strSize)
{
if(hFIdx > m_FontMaxCount || hFIdx < 0)
hFIdx = 0;

HDC hdc = GetDC( _g_hWnd );
HFONT OldFont = (HFONT)SelectObject( hdc, m_hFont[hFIdx] );
SIZE size;
GetTextExtentPoint32( hdc, str, strSize, &size );
SelectObject( hdc, OldFont );
ReleaseDC( _g_hWnd, hdc );
return size.cx;
}*/















LONG cFont::GetTextExtentEx(WORD hFIdx, char* str, int strLen)
{
	if (hFIdx > m_FontMaxCount || hFIdx < 0)
		hFIdx = 0;

	WORD wid = m_FontDesc[hFIdx].wWidth;
	WORD DBCNT = 0;
	WORD SBCNT = 0;
	for (int i = 0; i < strLen; i++)
	{
		if (IsDBCSLeadByte(str[i]))
		{
			DBCNT++;
			i++;
		}
		else
		{
			SBCNT++;
		}
	}
	LONG TextWidth = SBCNT*wid + DBCNT * 2 * wid;
	return TextWidth;
}
/*LONG cFont::GetTextExtentEx( WORD hFIdx, char* str, int strLen )
{
if(hFIdx > m_FontMaxCount || hFIdx < 0)
hFIdx = 0;

HDC hdc = GetDC( _g_hWnd );
HFONT OldFont = (HFONT)SelectObject( hdc, m_hFont[hFIdx] );
SIZE size;
GetTextExtentPoint32( hdc, str, strLen, &size );
SelectObject( hdc, OldFont );
ReleaseDC( _g_hWnd, hdc );
return size.cx;
}*/











/*
void cFont::RenderFont(WORD wFontIdx, char * text, int size, RECT * pRect, DWORD color, BOOL bDetailedWidth)
{
if(wFontIdx > m_FontMaxCount || wFontIdx < 0)
wFontIdx = 0;
if( wFontIdx == 5 )
{
pRect->right = pRect->left + GetTextExtentWidth( wFontIdx, text, size ) + GetTextWidth( wFontIdx ) ;
}
else
{
if (bDetailedWidth)
{
pRect->right = pRect->left + GetTextExtentWidth( wFontIdx, text, size ) + GetTextWidth( wFontIdx );
}
else
{
pRect->right = pRect->left + GetTextExtent( wFontIdx, text, size ) + GetTextWidth( wFontIdx );
}
}
pRect->bottom = pRect->top + GetTextHeight( wFontIdx ) + 5;
DISPLAY_INFO	dispInfo ;
GAMERESRCMNGR->GetDispInfo( &dispInfo ) ;
if( pRect->left > (LONG)(dispInfo.dwWidth) || pRect->right < 0 || pRect->bottom < 0 || pRect->top > (LONG)(dispInfo.dwHeight) )
return;
if(*text)
{
DWORD dwColor = color;
g_pExecutive->GetGeometry()->RenderFont(m_pFont[wFontIdx],text,size,pRect,dwColor,CHAR_CODE_TYPE_ASCII,0,0);
}
}
*/

void cFont::RenderFontWithShadow(WORD wFontIdx, char * text, int size, RECT * pRect, DWORD color, DWORD shadowColor)
{
	RenderFont(wFontIdx, text, size, pRect, shadowColor/*RGBA_MERGE(RGB_HALF(70, 70, 70), 180)*/);
	pRect->left -= 1;
	pRect->top -= 1;
	CFONT_OBJ->RenderFont(wFontIdx, text, size, pRect, RGBA_MERGE(color, 255));
	
}
void cFont::RenderFont(WORD wFontIdx, char * text, int size, RECT * pRect, DWORD color)
{
	if (wFontIdx>m_FontMaxCount || wFontIdx<0)
		wFontIdx = 0;
	pRect->right = pRect->left + GetTextExtent(wFontIdx, text, size) + GetTextWidth(wFontIdx) / 4;
	pRect->bottom = pRect->top + GetTextHeight(wFontIdx) + 5;
	DISPLAY_INFO	dispInfo;
	GAMERESRCMNGR->GetDispInfo(&dispInfo);
	if (pRect->left >(LONG)(dispInfo.dwWidth) || pRect->right < 0 || pRect->bottom < 0 || pRect->top >(LONG)(dispInfo.dwHeight))
		return;
	if (*text)
	{
		DWORD dwColor = 0xff000000 | color;
		g_pExecutive->GetGeometry()->RenderFont(m_pFont[wFontIdx], text, size, pRect, dwColor, CHAR_CODE_TYPE_ASCII, 0, 0);
	}
}



void cFont::RenderFontRegen(WORD wFontIdx, LONG x, LONG y, DWORD dwColor, LPCTSTR str, ...)
{
	char txt[MAX_PATH] = { 0, };
	va_list vList;
	va_start(vList, str);
	vsprintf(txt, str, vList);
	va_end(vList);

	HDC hdc = GetDC(_g_hWnd);
	HFONT OldFont = (HFONT)SelectObject(hdc, m_hFont[wFontIdx]);
	SIZE size;
	GetTextExtentPoint32(hdc, str, _tcslen(str), &size);
	SelectObject(hdc, OldFont);
	//ReleaseDC(_g_hWnd, hdc);

	//CClientDC dc(this);
	//cFont* pOldFont = dc.SelectObject(&m_Font);
	//SIZE size;
	//GetTextExtentPoint32(dc.m_hDC, txt, strlen(txt), &size);
	//dc.SelectObject(pOldFont);

	for (int row = y - 1; row <= y + 1; ++row)
	{
		for (int column = x - 1; column <= x + 1; ++column)
		{
			/*RECT rect;
			rect.left += column;
			rect.top += row;
			rect.right += column + size.cx;
			rect.bottom += row + size.cy;*/

			RECT rect = { (long)(x), (long)(y), x + size.cx, y + size.cy };

			g_pExecutive->GetGeometry()->RenderFont(
				m_pFont[wFontIdx],
				LPTSTR(txt),
				_tcslen(txt),
				&rect,
				RGBA_MAKE(0, 0, 0, 255),
				CHAR_CODE_TYPE_ASCII,
				0,
				0);
		}
	}
	/*RECT rect = { (long)(x), (long)(y), x + size.cx, y + size.cy };
	g_pExecutive->GetGeometry()->RenderFont(
	m_pFont[wFontIdx],
	LPTSTR(txt),
	_tcslen(txt),
	&rect,
	dwColor,
	CHAR_CODE_TYPE_ASCII,
	0,
	0);*/
}









void cFont::RenderFontAlpha(WORD wFontIdx, char * text, int size, RECT * pRect, DWORD color)
{
	if (wFontIdx > m_FontMaxCount || wFontIdx < 0)
		wFontIdx = 0;
	pRect->right = pRect->left + GetTextExtent(wFontIdx, text, size) + GetTextWidth(wFontIdx) / 4;
	pRect->bottom = pRect->top + GetTextHeight(wFontIdx) + 1;
	DISPLAY_INFO	dispInfo;
	GAMERESRCMNGR->GetDispInfo(&dispInfo);
	if (pRect->left >(LONG)(dispInfo.dwWidth) || pRect->right < 0 || pRect->bottom < 0 || pRect->top >(LONG)(dispInfo.dwHeight))
		return;
	if (*text)
	{
		DWORD dwColor = color;
		g_pExecutive->GetGeometry()->RenderFont(m_pFont[wFontIdx], text, size, pRect, dwColor, CHAR_CODE_TYPE_ASCII, 0, 0);
	}
}
/*void cFont::RenderFontAlpha(WORD wFontIdx, char * text, int size, RECT * pRect, DWORD color)
{
if(wFontIdx>m_FontMaxCount||wFontIdx<0)
wFontIdx = 0;
pRect->right = pRect->left + GetTextExtent( wFontIdx, text, size ) + GetTextWidth( wFontIdx ) / 4;
pRect->bottom = pRect->top + GetTextHeight( wFontIdx ) + 5;
if( pRect->left > GAMERESRCMNGR->m_GameDesc.dispInfo.dwWidth || pRect->right < 0 || pRect->bottom < 0 || pRect->top > GAMERESRCMNGR->m_GameDesc.dispInfo.dwHeight )
return;
if(*text)
{
DWORD dwColor = color;
g_pExecutive->GetGeometry()->RenderFont(m_pFont[wFontIdx],text,size,pRect,dwColor,CHAR_CODE_TYPE_ASCII,0,0);
}
}*/









LONG cFont::GetTextExtentWidth(WORD hFIdx, char* str, int strLen)
{
	if (hFIdx > m_FontMaxCount || hFIdx < 0) return 0;
	HDC hdc = GetDC(_g_hWnd);
	HFONT OldFont = (HFONT)SelectObject(hdc, m_hFont[hFIdx]);
	SIZE size;
	GetTextExtentPoint32(hdc, str, strLen, &size);
	SelectObject(hdc, OldFont);
	ReleaseDC(_g_hWnd, hdc);
	return size.cx;
}
/*LONG cFont::GetTextExtentWidth( WORD hFIdx, char* str, int strLen )
{
if(hFIdx > m_FontMaxCount || hFIdx < 0) return 0;
HDC hdc = GetDC(_g_hWnd) ;
HFONT OldFont = (HFONT)SelectObject( hdc, m_hFont[hFIdx] ) ;
SIZE size ;
GetTextExtentPoint32( hdc, str, strLen, &size ) ;
SelectObject( hdc, OldFont ) ;
ReleaseDC( _g_hWnd, hdc ) ;
return size.cx ;
}*/





void cFont::RenderNoticeMsg(WORD wFontIdx, char * text, int size, RECT * pRect, DWORD frontColor, DWORD backColor)
{
	//wFontIdx = 6 ;
	if (wFontIdx > m_FontMaxCount || wFontIdx < 0)
		wFontIdx = 0;
	if (wFontIdx)
	{
		pRect->right = pRect->left + GetTextExtentWidth(wFontIdx, text, size) + GetTextWidth(wFontIdx);
	}
	else
	{
		pRect->right = pRect->left + GetTextExtent(wFontIdx, text, size) + GetTextWidth(wFontIdx);
	}
	pRect->bottom = pRect->top + GetTextHeight(wFontIdx) + 5;
	DISPLAY_INFO	dispInfo;
	GAMERESRCMNGR->GetDispInfo(&dispInfo);
	if (pRect->left > (LONG)(dispInfo.dwWidth) || pRect->right < 0 || pRect->bottom < 0 || pRect->top >(LONG)(dispInfo.dwHeight))
		return;
	if (*text)
	{
		pRect->left -= 1;
		g_pExecutive->GetGeometry()->RenderFont(m_pFont[wFontIdx], text, size, pRect, backColor, CHAR_CODE_TYPE_ASCII, 0, 0);
		pRect->left += 2;
		g_pExecutive->GetGeometry()->RenderFont(m_pFont[wFontIdx], text, size, pRect, backColor, CHAR_CODE_TYPE_ASCII, 0, 0);
		pRect->left -= 1;
		pRect->top -= 1;
		g_pExecutive->GetGeometry()->RenderFont(m_pFont[wFontIdx], text, size, pRect, backColor, CHAR_CODE_TYPE_ASCII, 0, 0);
		pRect->top += 2;
		g_pExecutive->GetGeometry()->RenderFont(m_pFont[wFontIdx], text, size, pRect, backColor, CHAR_CODE_TYPE_ASCII, 0, 0);
		pRect->top -= 1;
		g_pExecutive->GetGeometry()->RenderFont(m_pFont[wFontIdx], text, size, pRect, frontColor, CHAR_CODE_TYPE_ASCII, 0, 0);
	}
}
/*void cFont::RenderNoticeMsg(WORD wFontIdx, char * text, int size, RECT * pRect, DWORD frontColor, DWORD backColor)
{
wFontIdx = wFontIdx ;
if(wFontIdx > m_FontMaxCount || wFontIdx < 0)
wFontIdx = 0;
if( wFontIdx )
{
pRect->right = pRect->left + GetTextExtentWidth( wFontIdx, text, size ) + GetTextWidth( wFontIdx ) / 4;
}
else
{
pRect->right = pRect->left + GetTextExtent( wFontIdx, text, size ) + GetTextWidth( wFontIdx ) / 4;
}
pRect->bottom = pRect->top + GetTextHeight( wFontIdx ) + 5;
if( pRect->left > GAMERESRCMNGR->m_GameDesc.dispInfo.dwWidth || pRect->right < 0 || pRect->bottom < 0 || pRect->top > GAMERESRCMNGR->m_GameDesc.dispInfo.dwHeight )
return;
if(*text)
{
pRect->left -= 1 ;
g_pExecutive->GetGeometry()->RenderFont(m_pFont[wFontIdx],text,size,pRect,backColor,CHAR_CODE_TYPE_ASCII,0,0);
pRect->left += 2 ;
g_pExecutive->GetGeometry()->RenderFont(m_pFont[wFontIdx],text,size,pRect,backColor,CHAR_CODE_TYPE_ASCII,0,0);
pRect->left -= 1 ;
g_pExecutive->GetGeometry()->RenderFont(m_pFont[wFontIdx],text,size,pRect,backColor,CHAR_CODE_TYPE_ASCII,0,0);
pRect->top -= 1 ;
g_pExecutive->GetGeometry()->RenderFont(m_pFont[wFontIdx],text,size,pRect,backColor,CHAR_CODE_TYPE_ASCII,0,0);
pRect->top += 2 ;
g_pExecutive->GetGeometry()->RenderFont(m_pFont[wFontIdx],text,size,pRect,backColor,CHAR_CODE_TYPE_ASCII,0,0);
pRect->top -= 1 ;
g_pExecutive->GetGeometry()->RenderFont(m_pFont[wFontIdx],text,size,pRect,frontColor,CHAR_CODE_TYPE_ASCII,0,0);
}
}*/











void cFont::RenderText(WORD wFontIdx, char * text, int size, RECT * pRect, DWORD frontColor, DWORD backColor)
{
	if (wFontIdx>m_FontMaxCount || wFontIdx<0)
		wFontIdx = 0;
	pRect->right = pRect->left + GetTextExtent(wFontIdx, text, size) + GetTextWidth(wFontIdx) / 4;
	pRect->bottom = pRect->top + GetTextHeight(wFontIdx) + 5;
	if (pRect->left > GAMERESRCMNGR->m_GameDesc.dispInfo.dwWidth || pRect->right < 0 || pRect->bottom < 0 || pRect->top > GAMERESRCMNGR->m_GameDesc.dispInfo.dwHeight)
		return;
	if (*text)
	{
		g_pExecutive->GetGeometry()->RenderFont(m_pFont[wFontIdx], text, size, pRect, backColor, CHAR_CODE_TYPE_ASCII, 0, 0);
		pRect->left -= 1;
		pRect->top -= 1;
		g_pExecutive->GetGeometry()->RenderFont(m_pFont[wFontIdx], text, size, pRect, frontColor, CHAR_CODE_TYPE_ASCII, 0, 0);
	}
}