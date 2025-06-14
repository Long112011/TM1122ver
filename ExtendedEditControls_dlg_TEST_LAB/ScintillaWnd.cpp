/* @doc
* @module ScintillaWnd.cpp | Implementation of a Scintilla syntax coloring edit control for MFC
* This program is an example how to use the excellent scintilla edit control of Neil Hodgson.<nl>
* See www.scintilla.org for details<nl>
* Author: Horst Br點kner, hb@ec-logic.com<nl>
* Environment: VisualC++ Version 6, static Build of Scintilla, SciLexer.dll as Lexer<nl>
*/
#include "stdafx.h"
#include "main.h"
#include "ScintillaWnd.h"
#include "Tokenizer.h"
//#include "scintilla\include\scintilla.h"
//#include "scintilla\include\SciLexer.h"
#include "scintilla\include\Accessor.h"
#include "scintilla\include\Propset.h"
#include "scintilla\include\keywords.h"
#include <fstream>
#include <io.h>
#include "dialog.h"
#include <shlwapi.h>
#include <winsock2.h>
//CScintillaWnd theScintillaEdit;
/////////////////////////////////////
// @mfunc init the view with reasonable defaults
// @rvalue void | not used
//
extern HWND			m_DialogHwnd;
extern HWND				m_hwndEditor;
void CScintillaWnd::UpdateWindowPos()
{
	//if (!m_info.IsFloating())
	//	return;
	// be aware that the owner and parent can be different
	auto pWndOwner = GetOwner();
	auto pWndParent = GetParent();
	ASSERT_VALID(pWndOwner);
	ASSERT_VALID(pWndParent);
	// owner window is the targeted restrain window
	CRect rectOwner;
	pWndOwner->GetClientRect(rectOwner);
	CRect rectDlg;
	GetWindowRect(rectDlg);
	pWndOwner->ScreenToClient(rectDlg);
	// Don't send WM_WINDOWPOSCHANGING to avoid auto snapping to parent during this procedure.
	
	{
		// in case the window was moved outside the client area, move it to proper place		
		CSize szDiff(rectDlg.right - rectOwner.right, rectDlg.bottom - rectOwner.bottom);
		if (szDiff.cx > 0)
			rectDlg.left = max(rectDlg.left - szDiff.cx, rectOwner.left);
		if (szDiff.cy > 0)
			rectDlg.top = max(rectDlg.top - szDiff.cy, rectOwner.top);
		if (szDiff.cx > 0 || szDiff.cy > 0)
		{
			if (pWndOwner->GetSafeHwnd() != pWndParent->GetSafeHwnd())
			{
				pWndOwner->MapWindowPoints(pWndParent, rectDlg);
			}
			UINT nFlags = SWP_NOSIZE | SWP_NOSENDCHANGING | SWP_NOZORDER | SWP_NOACTIVATE;
			SetWindowPos(nullptr, rectDlg.left, rectDlg.top, -1, -1, nFlags);
		}
	}
}
void CScintillaWnd::insertGenericTextFrom(int position, const TCHAR *text2insert) const
{
#ifdef UNICODE
	WcharMbcsConvertor *wmc = WcharMbcsConvertor::getInstance();
	unsigned int cp = execute(SCI_GETCODEPAGE);
	const char *text2insertA = wmc->wchar2char(text2insert, cp);
	execute(SCI_INSERTTEXT, position, (WPARAM)text2insertA);
#else
	SendMessage(SCI_INSERTTEXT, position, (WPARAM)text2insert);
#endif
}
void CScintillaWnd::Init()
{
	// clear all text styles
	SendMessage(SCI_CLEARDOCUMENTSTYLE, 0, 0);
	// set the number of styling bits to 7 - the asp/html views need a lot of styling - default is 5
	// If you leave the default you will see twiggle lines instead of ASP code
	SendMessage(SCI_SETSTYLEBITS, 7, 0);
	// set the display for indetation guides to on - this displays virtical dotted lines from the beginning of 
	// a code block to the end of the block
	SendMessage(SCI_SETINDENTATIONGUIDES, TRUE, 0);
	// set tabwidth to 3
	SendMessage(SCI_SETTABWIDTH, 3, 0);
	// set indention to 3
	SendMessage(SCI_SETINDENT, 3, 0);
	// set the caret blinking time to 400 milliseconds
	SendMessage(SCI_SETCARETPERIOD, 400, 0);
	// source folding section
	// tell the lexer that we want folding information - the lexer supplies "folding levels"
	SendMessage(SCI_SETPROPERTY, (WPARAM)_T("fold"), (LPARAM)_T("1"));
	SendMessage(SCI_SETPROPERTY, (WPARAM)_T("fold.html"), (LPARAM)_T("1"));
	SendMessage(SCI_SETPROPERTY, (WPARAM)_T("fold.html.preprocessor"), (LPARAM)_T("1"));
	SendMessage(SCI_SETPROPERTY, (WPARAM)_T("fold.comment"), (LPARAM)_T("1"));
	SendMessage(SCI_SETPROPERTY, (WPARAM)_T("fold.at.else"), (LPARAM)_T("1"));
	SendMessage(SCI_SETPROPERTY, (WPARAM)_T("fold.flags"), (LPARAM)_T("1"));
	SendMessage(SCI_SETPROPERTY, (WPARAM)_T("fold.preprocessor"), (LPARAM)_T("1"));
	SendMessage(SCI_SETPROPERTY, (WPARAM)_T("styling.within.preprocessor"), (LPARAM)_T("1"));
	SendMessage(SCI_SETPROPERTY, (WPARAM)_T("asp.default.language"), (LPARAM)_T("1"));
	// Tell scintilla to draw folding lines UNDER the folded line
	SendMessage(SCI_SETFOLDFLAGS, 16, 0);
	// Set margin 2 = folding margin to display folding symbols
	SendMessage(SCI_SETMARGINMASKN, 2, SC_MASK_FOLDERS);
	// allow notifications for folding actions
	SendMessage(SCI_SETMODEVENTMASK, SC_MOD_INSERTTEXT | SC_MOD_DELETETEXT, 0);
	//   SendMessage(SCI_SETMODEVENTMASK, SC_MOD_CHANGEFOLD|SC_MOD_INSERTTEXT|SC_MOD_DELETETEXT, 0);
	// make the folding margin sensitive to folding events = if you click into the margin you get a notification event
	SendMessage(SCI_SETMARGINSENSITIVEN, 2, TRUE);
	// define a set of markers to displa folding symbols
	SendMessage(SCI_MARKERDEFINE, SC_MARKNUM_FOLDEROPEN, SC_MARK_MINUS);
	SendMessage(SCI_MARKERDEFINE, SC_MARKNUM_FOLDER, SC_MARK_PLUS);
	SendMessage(SCI_MARKERDEFINE, SC_MARKNUM_FOLDERSUB, SC_MARK_EMPTY);
	SendMessage(SCI_MARKERDEFINE, SC_MARKNUM_FOLDERTAIL, SC_MARK_EMPTY);
	SendMessage(SCI_MARKERDEFINE, SC_MARKNUM_FOLDEREND, SC_MARK_EMPTY);
	SendMessage(SCI_MARKERDEFINE, SC_MARKNUM_FOLDEROPENMID, SC_MARK_EMPTY);
	SendMessage(SCI_MARKERDEFINE, SC_MARKNUM_FOLDERMIDTAIL, SC_MARK_EMPTY);
	// set the forground color for some styles
	SendMessage(SCI_STYLESETFORE, 0, RGB(0, 0, 0));
	SendMessage(SCI_STYLESETFORE, 2, RGB(0, 64, 0));
	SendMessage(SCI_STYLESETFORE, 5, RGB(0, 0, 255));
	SendMessage(SCI_STYLESETFORE, 6, RGB(200, 20, 0));
	SendMessage(SCI_STYLESETFORE, 9, RGB(0, 0, 255));
	SendMessage(SCI_STYLESETFORE, 10, RGB(255, 0, 64));
	SendMessage(SCI_STYLESETFORE, 11, RGB(0, 0, 0));
	// set the backgroundcolor of brace highlights
	SendMessage(SCI_STYLESETBACK, STYLE_BRACELIGHT, RGB(0, 255, 0));
	

	SetXmlLexer();

}
void CScintillaWnd::SetDefaultColorFont(int nSize, const TCHAR* face, int style, int bSelect)
{
	//SendMessage(SCI_SETSELFORE, TRUE, RGB(255, 255, 255));
	SendMessage(SCI_STYLESETFORE, style, GET_FORE_COLOR(bSelect));
	if (style == STYLE_BRACELIGHT)
	{
		SendMessage(SCI_STYLESETBACK, style, theApp.GetGameAI()->sPage1.GlobalBack ? GET_BACK_COLOR(0) : GET_BACK_COLOR(15));
	}
	else
	{
		SendMessage(SCI_STYLESETBACK, style, GET_BACK_COLOR(bSelect));
	}	
	
	SendMessage(SCI_STYLESETSIZE, style, nSize);
	SendMessage(SCI_STYLESETFONT, style, reinterpret_cast<LPARAM>(face));

	SendMessage(SCI_SETWHITESPACEFORE, style, GET_FORE_COLOR(1));
	SendMessage(SCI_SETWHITESPACEBACK, style, GET_BACK_COLOR(2));

	SetBold(style, theApp.GetGameAI()->sPage1.fontBold[bSelect]);
	SetItalic(style, theApp.GetGameAI()->sPage1.fontItalic[bSelect]);
	SetUnderline(style, theApp.GetGameAI()->sPage1.fontUnderline[bSelect]);

	
	
	//SendMessage(SCI_STYLESETCHARACTERSET, STYLE_DEFAULT, SC_CHARSET_OEM);
	//SetFontLocale("en-us");
	//setlocale(LC_ALL, "en-us");

	//use this two below
	//
	//SetCodePage(SC_CP_UTF8);
}
void CScintillaWnd::InitEdit()
{
	

	//SendMessage(SCI_STYLESETCHARACTERSET, STYLE_DEFAULT, SC_CHARSET_GB2312 | SC_CHARSET_ANSI | SC_CHARSET_CHINESEBIG5 | SC_CHARSET_DEFAULT | SC_CHARSET_HANGUL | SC_CHARSET_OEM | SC_CHARSET_SYMBOL | SC_CHARSET_THAI);
	SetCharacterSet(STYLE_DEFAULT, SC_CHARSET_ANSI);
	//SendMessage(SCI_STYLESETCHARACTERSET, STYLE_DEFAULT, SC_CHARSET_ANSI);
	//SendMessage(SCI_STYLESETCHARACTERSET, SCE_C_COMMENTLINE, SC_CHARSET_DEFAULT);
	SetCodePage(CP_OEMCP);

	SendMessage(SCI_INDICSETSTYLE,0, INDIC_SQUIGGLE);
	SendMessage(SCI_INDICSETSTYLE,1, INDIC_TT);
	SendMessage(SCI_INDICSETSTYLE,2, INDIC_PLAIN);

	//SendMessage(SCI_SETEOLMODE, SC_EOL_CRLF);
	//SendMessage( SCI_CLEARALLCMDKEYS, 0, 0);
	//SendMessage(SCI_ASSIGNCMDKEY, VK_CONTROL, reinterpret_cast<LPARAM>("G"));

	//SendMessage(SCI_SETZOOM, static_cast<WPARAM>(-10), 0);
	//SendMessage(SCI_SETVSCROLLBAR, FALSE, 0);
	//SendMessage(SCI_SETHSCROLLBAR, FALSE, 0);

	//::setlocale(LC_ALL, "en-us");
	//SendEditor(SCI_SETFONTLOCALE, 0, 0);
	//SendMessage(SCI_SETCODEPAGE, SC_CP_UTF8);
	//SetCodePage(SC_CP_UTF8);
	//SetCodePage(950);
	//SendMessage(static_cast<UINT>(SCI_SETCODEPAGE), static_cast<WPARAM>(0), 0);
	SendMessage(SCI_CLEARDOCUMENTSTYLE, 0, 0);
	SendMessage(SCI_SETSTYLEBITS, 7, 0);
	SendMessage(SCI_SETINDENTATIONGUIDES, TRUE, 0);//garisan indent
	//SendMessage(SCI_SETTABWIDTH, 3, 0);
	//SendMessage(SCI_SETINDENT, 3, 0);
	SendMessage(SCI_SETCARETPERIOD, 400, 0);

	SendMessage(SCI_SETFOLDFLAGS, 16, 0);

	SendMessage(SCI_SETMARGINMASKN, 2, SC_MASK_FOLDERS);//KALAU 1 DIA AKAN BOOKMARK LINE

	SendMessage(SCI_SETMODEVENTMASK, SC_MOD_INSERTTEXT | SC_MOD_DELETETEXT, 0);

	SendMessage(SCI_SETMARGINSENSITIVEN, 2, TRUE);//KALAU 1 DIA AKAN BOOKMARK LINE

	
	SetDefaultColorFont(theApp.GetGameAI()->sPage1.fontSize, theApp.GetGameAI()->sPage1.fontName,STYLE_DEFAULT,0);
	
	SendMessage(SCI_STYLECLEARALL); 



	SendMessage(SCI_ASSIGNCMDKEY, (WPARAM)('S' + (SCMOD_CTRL << 16)), (LPARAM)SCI_NULL);
	SendMessage(SCI_ASSIGNCMDKEY, (WPARAM)('B' + (SCMOD_CTRL << 16)), (LPARAM)SCI_NULL);

	SendMessage(SCI_ASSIGNCMDKEY, (WPARAM)('Q' + (SCMOD_ALT << 16)), (LPARAM)SCI_NULL);

	SendMessage(SCI_ASSIGNCMDKEY, (WPARAM)('Q' + (SCMOD_CTRL << 16)), (LPARAM)SCI_NULL);
	SendMessage(SCI_ASSIGNCMDKEY, (WPARAM)('W' + (SCMOD_CTRL << 16)), (LPARAM)SCI_NULL);
	SendMessage(SCI_ASSIGNCMDKEY, (WPARAM)('E' + (SCMOD_CTRL << 16)), (LPARAM)SCI_NULL);
	SendMessage(SCI_ASSIGNCMDKEY, (WPARAM)('R' + (SCMOD_CTRL << 16)), (LPARAM)SCI_NULL);
	SendMessage(SCI_ASSIGNCMDKEY, (WPARAM)('O' + (SCMOD_CTRL << 16)), (LPARAM)SCI_NULL);
	SendMessage(SCI_ASSIGNCMDKEY, (WPARAM)('P' + (SCMOD_CTRL << 16)), (LPARAM)SCI_NULL);

	SendMessage(SCI_ASSIGNCMDKEY, (WPARAM)('F' + (SCMOD_CTRL << 16)), (LPARAM)SCI_NULL);
	SendMessage(SCI_ASSIGNCMDKEY, (WPARAM)('G' + (SCMOD_CTRL << 16)), (LPARAM)SCI_NULL);
	SendMessage(SCI_ASSIGNCMDKEY, (WPARAM)('H' + (SCMOD_CTRL << 16)), (LPARAM)SCI_NULL);
	SendMessage(SCI_ASSIGNCMDKEY, (WPARAM)('J' + (SCMOD_CTRL << 16)), (LPARAM)SCI_NULL);
	SendMessage(SCI_ASSIGNCMDKEY, (WPARAM)('K' + (SCMOD_CTRL << 16)), (LPARAM)SCI_NULL);
	SendMessage(SCI_ASSIGNCMDKEY, (WPARAM)('L' + (SCMOD_CTRL << 16)), (LPARAM)SCI_NULL);

	SendMessage(SCI_ASSIGNCMDKEY, (WPARAM)('N' + (SCMOD_CTRL << 16)), (LPARAM)SCI_NULL);
	SendMessage(SCI_ASSIGNCMDKEY, (WPARAM)('M' + (SCMOD_CTRL << 16)), (LPARAM)SCI_NULL);


	//testing
	SendMessage(SCI_INDICSETSTYLE, 0, INDIC_ROUNDBOX);
	SendMessage(SCI_INDICSETFORE, 0, RGB(255,255,0));


	//automatic completion
	//SendMessage(SCI_AUTOCSETSEPARATOR, static_cast<WPARAM>(' '), 0);	//Set autocomplete list word separator
	//SendMessage(SCI_AUTOCSETMAXHEIGHT, static_cast<WPARAM>(15), 0);
	//SendMessage(SCI_AUTOCSETMAXWIDTH, static_cast<WPARAM>(0), 0);
	//SendMessage(SCI_AUTOCSETIGNORECASE, static_cast<WPARAM>(1), 0);


	//current line
	SendMessage(SCI_SETCARETLINEVISIBLE, TRUE);
	//SendEditor(SCI_SETCARETLINEVISIBLEALWAYS, TRUE);
	SendMessage(SCI_SETCARETFORE, GET_FORE_COLOR(2));
	//SendMessage(SCI_SETCARETLINEBACKALPHA, 50, RGB(255,0,0));
	SendMessage(SCI_SETCARETLINEBACK, GET_BACK_COLOR(2),10/*0xa0ffff*/);

	SendMessage(SCI_SETCARETLINEFRAME, 1);

	
	// SetBraceText();
	SetDefaultColorFont(theApp.GetGameAI()->sPage1.fontSize, theApp.GetGameAI()->sPage1.fontName, STYLE_BRACELIGHT, 15);

	//SetDefaultColorFont(theApp.GetGameAI()->sPage1.fontSize, theApp.GetGameAI()->sPage1.fontName, STYLE_BRACEBAD, 15);
	//SetDefaultColorFont(theApp.GetGameAI()->sPage1.fontSize, theApp.GetGameAI()->sPage1.fontName, STYLE_INDENTGUIDE, 15);
	//SendMessage(SCI_CONVERTEOLS, SC_EOL_CRLF, 0);//def 2
	SendMessage(SCI_SETVIEWEOL, theApp.GetGameAI()->sPage1.m_enableEOLflag, 0);
	
	SendMessage(SCI_SETPASTECONVERTENDINGS, TRUE, 0);

	SetMarker();

	// selected text
	SendMessage(SCI_SETSELBACK, TRUE, GET_BACK_COLOR(3));
	SendMessage(SCI_SETSELFORE, TRUE, GET_FORE_COLOR(3));

	// set hotspot
	SendMessage(SCI_STYLESETHOTSPOT, STYLE_DEFAULT);
	SendMessage(SCI_SETCARETFORE, GET_FORE_COLOR(2));
	
	
	//SetAStyle(STYLE_LINENUMBER, GET_FORE_COLOR(1), GET_BACK_COLOR(1), theApp.GetGameAI()->sPage1.fontSize, theApp.GetGameAI()->sPage1.fontName);
	SetDefaultColorFont(theApp.GetGameAI()->sPage1.fontSize, theApp.GetGameAI()->sPage1.fontName, STYLE_LINENUMBER, 1);
	SetDisplayLinenumbers(theApp.GetGameAI()->sPage1.m_nLineNumber);

	//SetDefaultColorFont(theApp.GetGameAI()->sPage1.fontSize, theApp.GetGameAI()->sPage1.fontName, STYLE_CONTROLCHAR, 0);

	SetDisplaySelection(TRUE);
	SetDisplayFolding(FALSE);

	//SendMessage(SCI_SETMARGINWIDTHN, 4, 16);
	//SendMessage(SCI_SETMARGINWIDTHN, 1, 6);
	SendMessage(SCI_SETSCROLLWIDTH, 5000);

	SetXmlLexer(theApp.GetGameAI()->sPage1.GlobalBack);

	setBorderEdge(m_hwndEditor, !theApp.GetGameAI()->sPage1.m_edgeNoBorder);

	SetTransparent(m_hwndEditor, theApp.GetGameAI()->sPage1.m_styleAlpha ? theApp.GetGameAI()->sPage1.m_styleAlphaPercent : 255);
}
void CScintillaWnd::SetBraceText()
{
	//brace text highlighting
	SendMessage(SCI_STYLESETFORE, STYLE_BRACELIGHT, GET_FORE_COLOR(15));
	SendMessage(SCI_STYLESETBACK, STYLE_BRACELIGHT, theApp.GetGameAI()->sPage1.GlobalBack ? GET_BACK_COLOR(0) : GET_BACK_COLOR(15));
	SendMessage(SCI_STYLESETBOLD, STYLE_BRACELIGHT, theApp.GetGameAI()->sPage1.fontBold[15]);
}
void CScintillaWnd::SetMarker()
{
	SendMessage(SCI_MARKERDEFINE, SC_MARKNUM_FOLDEROPEN, SC_MARK_MINUS);
	SendMessage(SCI_MARKERDEFINE, SC_MARKNUM_FOLDER, SC_MARK_PLUS);
	SendMessage(SCI_MARKERDEFINE, SC_MARKNUM_FOLDERSUB, SC_MARK_EMPTY);
	SendMessage(SCI_MARKERDEFINE, SC_MARKNUM_FOLDERTAIL, SC_MARK_EMPTY);
	SendMessage(SCI_MARKERDEFINE, SC_MARKNUM_FOLDEREND, SC_MARK_EMPTY);
	SendMessage(SCI_MARKERDEFINE, SC_MARKNUM_FOLDEROPENMID, SC_MARK_EMPTY);
	SendMessage(SCI_MARKERDEFINE, SC_MARKNUM_FOLDERMIDTAIL, SC_MARK_EMPTY);

	// set markersymbol for marker type 0 - bookmark
	SendMessage(SCI_MARKERSETFORE, 1, RGB(80, 0, 0));
	SendMessage(SCI_MARKERSETBACK, 1, RGB(255, 0, 0));
	SendMessage(SCI_MARKERDEFINE, 1, SC_MARK_LEFTRECT /*SC_MARK_CIRCLE*/);

	SendMessage(SCI_MARKERSETFORE, 0, RGB(80, 0, 0));
	SendMessage(SCI_MARKERSETBACK, 0, RGB(255, 123, 0));
	SendMessage(SCI_MARKERDEFINE, 0, SC_MARK_LEFTRECT /*SC_MARK_CIRCLE*/);
}
void CScintillaWnd::SetXmlLexer(BOOL useGlobal)
{
	/*if (FALSE == theApp.GetGameAI()->sPage1.m_colorSyntax)
	{
	return;
	}*/

	SendMessage(SCI_SETLEXER, SCLEX_CPP);

	//SendEditor(SCI_SETKEYWORDS, 0, (LPARAM)g_cppKeyWords);
	SendMessage(SCI_SETKEYWORDS, 0, reinterpret_cast<LPARAM>(g_cppKeyWords));
	//SendMessage(SCI_SETKEYWORDS, 1, reinterpret_cast<LPARAM>(g_cppKeyWordsTestType));



	SetProperty();

	//SendMessage(SCI_STYLESETEOLFILLED, SCE_C_COMMENT, true);
	SendMessage(SCI_STYLESETEOLFILLED, STYLE_DEFAULT, true);

	SendMessage(SCI_STYLESETBACK, SCE_C_WORD, useGlobal ? GET_BACK_COLOR(0) : GET_BACK_COLOR(5));
	SendMessage(SCI_STYLESETBACK, SCE_C_STRING, useGlobal ? GET_BACK_COLOR(0) : GET_BACK_COLOR(6));
	SendMessage(SCI_STYLESETBACK, SCE_C_CHARACTER, useGlobal ? GET_BACK_COLOR(0) : GET_BACK_COLOR(7));
	SendMessage(SCI_STYLESETBACK, SCE_C_PREPROCESSOR, useGlobal ? GET_BACK_COLOR(0) : GET_BACK_COLOR(8));
	SendMessage(SCI_STYLESETBACK, SCE_C_COMMENT, useGlobal ? GET_BACK_COLOR(0) : GET_BACK_COLOR(9));
	SendMessage(SCI_STYLESETBACK, SCE_C_COMMENTLINE, useGlobal ? GET_BACK_COLOR(0) : GET_BACK_COLOR(10));
	SendMessage(SCI_STYLESETBACK, SCE_C_COMMENTDOC, useGlobal ? GET_BACK_COLOR(0) : GET_BACK_COLOR(11));
	SendMessage(SCI_STYLESETBACK, SCE_C_NUMBER, useGlobal ? GET_BACK_COLOR(0) : GET_BACK_COLOR(12));
	SendMessage(SCI_STYLESETBACK, SCE_C_OPERATOR, useGlobal ? GET_BACK_COLOR(0) : GET_BACK_COLOR(13));
	SendMessage(SCI_STYLESETBACK, SCE_C_UUID, useGlobal ? GET_BACK_COLOR(0) : GET_BACK_COLOR(14));

	SendMessage(SCI_STYLESETFORE, SCE_C_WORD, theApp.GetGameAI()->sPage1.m_colorSyntax ? GET_FORE_COLOR(5) : GET_FORE_COLOR(0));
	SendMessage(SCI_STYLESETFORE, SCE_C_STRING, theApp.GetGameAI()->sPage1.m_colorSyntax ? GET_FORE_COLOR(6) : GET_FORE_COLOR(0));
	SendMessage(SCI_STYLESETFORE, SCE_C_CHARACTER, theApp.GetGameAI()->sPage1.m_colorSyntax ? GET_FORE_COLOR(7) : GET_FORE_COLOR(0));
	SendMessage(SCI_STYLESETFORE, SCE_C_PREPROCESSOR, theApp.GetGameAI()->sPage1.m_colorSyntax ? GET_FORE_COLOR(8) : GET_FORE_COLOR(0));
	SendMessage(SCI_STYLESETFORE, SCE_C_COMMENT, theApp.GetGameAI()->sPage1.m_colorSyntax ? GET_FORE_COLOR(9) : GET_FORE_COLOR(0));
	SendMessage(SCI_STYLESETFORE, SCE_C_COMMENTLINE, theApp.GetGameAI()->sPage1.m_colorSyntax ? GET_FORE_COLOR(10) : GET_FORE_COLOR(0));
	SendMessage(SCI_STYLESETFORE, SCE_C_COMMENTDOC, theApp.GetGameAI()->sPage1.m_colorSyntax ? GET_FORE_COLOR(11) : GET_FORE_COLOR(0));
	SendMessage(SCI_STYLESETFORE, SCE_C_NUMBER, theApp.GetGameAI()->sPage1.m_colorSyntax ? GET_FORE_COLOR(12) : GET_FORE_COLOR(0));
	SendMessage(SCI_STYLESETFORE, SCE_C_OPERATOR, theApp.GetGameAI()->sPage1.m_colorSyntax ? GET_FORE_COLOR(13) : GET_FORE_COLOR(0));
	SendMessage(SCI_STYLESETFORE, SCE_C_UUID, theApp.GetGameAI()->sPage1.m_colorSyntax ? GET_FORE_COLOR(14) : GET_FORE_COLOR(0));
}
void CScintillaWnd::SetProperty()
{
	SendMessage(SCI_SETPROPERTY, reinterpret_cast<WPARAM>("fold"), reinterpret_cast<LPARAM>("1"));
	SendMessage(SCI_SETPROPERTY, reinterpret_cast<WPARAM>("fold.compact"), reinterpret_cast<LPARAM>("0"));

	SendMessage(SCI_SETPROPERTY, reinterpret_cast<WPARAM>("fold.comment"), reinterpret_cast<LPARAM>("1"));
	SendMessage(SCI_SETPROPERTY, reinterpret_cast<WPARAM>("fold.cpp.comment.explicit"), reinterpret_cast<LPARAM>("0"));
	SendMessage(SCI_SETPROPERTY, reinterpret_cast<WPARAM>("fold.preprocessor"), reinterpret_cast<LPARAM>("1"));

	// Disable track preprocessor to avoid incorrect detection.
	// In the most of cases, the symbols are defined outside of file.
	SendMessage(SCI_SETPROPERTY, reinterpret_cast<WPARAM>("lexer.cpp.track.preprocessor"), reinterpret_cast<LPARAM>("0"));
}
void CScintillaWnd::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	//if (theApp.GetGameAI()->sPage1.m_nLineNumber)
		UpdateLineNumberWidth();
	__super::OnVScroll(nSBCode, nPos, pScrollBar);
	return;
	//TT
	if (pScrollBar->m_hWnd == m_hWnd)
	{
		SCROLLINFO si;
		si.fMask = SIF_ALL;
		GetScrollInfo(SIF_ALL, &si);
		switch (nSBCode)
		{
		case SB_LINEUP:
			si.nPos--;
			break;
		case SB_LINEDOWN:
			si.nPos++;
			break;
		case SB_PAGEUP:
			si.nPos -= si.nPage;
			break;
		case SB_PAGEDOWN:
			si.nPos += si.nPage;
			break;
		case SB_THUMBTRACK:
			si.nPos = nPos;
			break;
		}
		if (si.nPos>(int)(si.nMax - si.nMin - si.nPage + 1)) si.nPos = si.nMax - si.nMin - si.nPage + 1;
		if (si.nPos<si.nMin) si.nPos = si.nMin;
		si.fMask = SIF_POS;
		SetScrollInfo(SIF_ALL,&si);
		TRACE("\nPos=%d", si.nPos);
	}
}

/////////////////////////////////////////////////////////////////////////////

void CScintillaWnd::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	UpdateLineNumberWidth();
	__super::OnHScroll(nSBCode, nPos, pScrollBar);
	return;
	if (pScrollBar->m_hWnd == m_hWnd)
	{
		SCROLLINFO si;
		si.fMask = SIF_ALL;
		GetScrollInfo(SIF_ALL, &si);
		switch (nSBCode)
		{
		case SB_LINEUP:
			si.nPos--;
			break;
		case SB_LINEDOWN:
			si.nPos++;
			break;
		case SB_PAGEUP:
			si.nPos -= si.nPage;
			break;
		case SB_PAGEDOWN:
			si.nPos += si.nPage;
			break;
		}
		if (si.nPos>(int)(si.nMax - si.nMin - si.nPage + 1)) si.nPos = si.nMax - si.nMin - si.nPage + 1;
		if (si.nPos<si.nMin) si.nPos = si.nMin;
		SetScrollInfo(SIF_ALL,&si);
	}
}

// the next 2 arrays are used to determine lexer format from file extensions
static TCHAR *szExtensions[] =
{
	_T("py"),																	// SCLEX_PYTHON
	_T("c|cc|cpp|cxx|cs|h|hh|hpp|hxx|sma|dlg|rc|rc2|idl|odl|mak|js|java|txt"),   // SCLEX_CPP
	_T("htm|html|shtml|htt|cfm|tpl|hta"),										// SCLEX_HTML
	_T("xml|gcl|xsl|svg|xul|xsd|dtd|xslt|axl"),									// SCLEX_XML
	_T("pl|pm|cgi|pod"),															// SCLEX_PERL
	_T("sql|spec|body|sps|spb|sf|sp"),											// SCLEX_SQL
	_T("vb|bas|frm|cls|ctl|pag|dsr|dob"),										// SCLEX_VB
	_T("properties"),															// SCLEX_PROPERTIES
	_T("err"),																	// SCLEX_ERRORLIST
	_T("iface|mak"),																// SCLEX_MAKEFILE
	_T("bat|cmd|nt"),															// SCLEX_BATCH
	_T("xcode"),																	// SCLEX_XCODE
	_T("tex|sty"),																// SCLEX_LATEX
	_T("lua"),																	// SCLEX_LUA
	_T("diff"),																	// SCLEX_DIFF
	_T("conf"),																	// SCLEX_CONF
	_T("pas|inc|pp"),															// SCLEX_PASCAL
	_T("ave"),																	// SCLEX_AVE
	_T("ada|ads|adb"),															// SCLEX_ADA
	_T("lsp|lisp|scm|smd|ss"),													// SCLEX_LISP
	_T("rb"),																	// SCLEX_RUBY
	_T("e"),																		// SCLEX_EIFFEL
	_T("e"),																		// SCLEX_EIFFELKW
	_T("tcl"),																	// SCLEX_TCL
	_T("tab|spf"),																// SCLEX_NNCRONTAB
	_T("ant"),																	// SCLEX_BULLANT
	_T("vbs|dsm"),																// SCLEX_VBSCRIPT
	_T("asp|aspx"),																// SCLEX_ASP
	_T("php|php3|php4"),															// SCLEX_PHP
	_T("bc|cln"),																// SCLEX_BAAN
	_T("m"),																		// SCLEX_MATLAB
	_T("sol"),																	// SCLEX_SCRIPTOL
	_T("asm"),																	// SCLEX_ASM
	_T("cpp"),																	// SCLEX_CPPNOCASE
	_T("f|for|f90|f95"),															// SCLEX_FORTRAN
	_T("f77"),																	// SCLEX_F77
	_T("css"),																	// SCLEX_CSS
	_T("pov"),																	// SCLEX_POV
	_T("lt|lot"),																// SCLEX_LOUT
	_T("src|em"),																// SCLEX_ESCRIPT
	_T("bin|bsad|beff|befl|bmhm|map|md"),										// GETFILEEXTENSION
	0,
};
static int nFormats[] = {
	SCLEX_PYTHON,
	SCLEX_CPP,
	SCLEX_HTML,
	SCLEX_XML,
	SCLEX_PERL,
	SCLEX_SQL,
	SCLEX_VB,
	SCLEX_PROPERTIES,
	SCLEX_ERRORLIST,
	SCLEX_MAKEFILE,
	SCLEX_BATCH,
	SCLEX_XCODE,
	SCLEX_LATEX,
	SCLEX_LUA,
	SCLEX_DIFF,
	SCLEX_CONF,
	SCLEX_PASCAL,
	SCLEX_AVE,
	SCLEX_ADA,
	SCLEX_LISP,
	SCLEX_RUBY,
	SCLEX_EIFFEL,
	SCLEX_EIFFELKW,
	SCLEX_TCL,
	SCLEX_NNCRONTAB,
	SCLEX_BULLANT,
	SCLEX_VBSCRIPT,
	29,
	30,
	//   SCLEX_ASP,
	//   SCLEX_PHP,
	SCLEX_BAAN,
	SCLEX_MATLAB,
	SCLEX_SCRIPTOL,
	SCLEX_ASM,
	SCLEX_CPPNOCASE,
	SCLEX_FORTRAN,
	SCLEX_F77,
	SCLEX_CSS,
	SCLEX_POV,
	SCLEX_LOUT,
	SCLEX_ESCRIPT,
	GETFILEEXTENSION,
	0,
};
extern const char g_cppKeyWords[];
//extern const char g_cppKeyWordsTestType[];

/////////////////////////////////////
// @mfunc This is an empty constructor
// @rvalue void | not used
//
CScintillaWnd::CScintillaWnd()
{
	//m_bLinenumbers = FALSE;// theApp.GetGameAI()->sPage1.m_nLineNumber;
	//m_bSelection = FALSE;
	//m_bFolding = FALSE;// theApp.GetGameAI()->sPage1.m_nLineNumber;
	m_nSearchflags = 0;
}

/////////////////////////////////////
// @mfunc This is a destructor
// @rvalue void | not used
//
CScintillaWnd::~CScintillaWnd()
{

}
/////////////////////////////////////
// @mfunc Create the window NEW
// @rvalue BOOL | TRUE on success else FALSE on error
//
HWND CScintillaWnd::CreateEditor(LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, HWND pParentWnd, UINT nID)
{
	HWND hEdit = CreateWindowEx(0, "Scintilla", ""
		, dwStyle
		, 10, 10, 500, 400
		, pParentWnd, (HMENU)nID
		, AfxGetApp()->m_hInstance, NULL);


	/*HWND hEdit = CreateWindowEx(WS_EX_CLIENTEDGE, STR_SCINTILLAWND, TEXT(""),
		WS_CHILD | WS_VISIBLE | WS_VSCROLL | ES_MULTILINE | ES_AUTOVSCROLL | ES_NOHIDESEL ,
		CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
		pParentWnd, (HMENU)IDC_EDIT_EXT, AfxGetApp()->m_hInstance, NULL);*/

	return hEdit;
}
BOOL CScintillaWnd::CreateEditorNormal(LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, HWND pParentWnd, UINT nID)
{
	HWND hEdit = CreateWindowEx(WS_EX_CLIENTEDGE, "Edit", lpszWindowName
		, dwStyle
		, rect.left, rect.top, rect.right, rect.bottom
		, pParentWnd, (HMENU)nID
		, AfxGetApp()->m_hInstance, NULL);


	/*HWND hEdit = CreateWindowEx(WS_EX_CLIENTEDGE, STR_SCINTILLAWND, TEXT(""),
	WS_CHILD | WS_VISIBLE | WS_VSCROLL | ES_MULTILINE | ES_AUTOVSCROLL | ES_NOHIDESEL ,
	CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
	pParentWnd, (HMENU)IDC_EDIT_EXT, AfxGetApp()->m_hInstance, NULL);*/

	return TRUE;
}
BOOL CScintillaWnd::Create(LPCTSTR wclass,LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID)
{	
	if (!CWnd::CreateEx(WS_EX_CLIENTEDGE, wclass, lpszWindowName, dwStyle, rect, pParentWnd, (UINT)nID))
	{
		/*LPVOID lpMsgBuf;
		::FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
			NULL, GetLastError(), MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPTSTR)&lpMsgBuf, 0, NULL);
		// Write to stderr
		TRACE(_T("%s\n"), (LPCTSTR)lpMsgBuf);
		// Free the buffer.
		LocalFree(lpMsgBuf);*/
		return FALSE;
	}
	m_pParent = pParentWnd;
	return TRUE;
}
/////////////////////////////////////
// @mfunc Try to load the Scintilla dll - usually named "SciLexer.dll" or "Scintilla.dll".  We try to locate the dll in 
// the current dirtectory and along the path environment.
// Call this function in your CWinApp derived application in the InitInstance function by calling:<nl>
// CScintillaWnd::LoadScintillaDll()<nl>
// @rvalue BOOL | FALSE on error - TRUE on success
//
HMODULE CScintillaWnd::LoadScintillaDll(
	LPCSTR szDllFile) //@parm filename of the lexer dll - default "SciLexer.dll"
{
	CString strLexer = STR_LEXERDLL;
	if (szDllFile != NULL)
		strLexer = szDllFile;
	// this call to LoadLibrary searches in:
	// 1.) current directory
	// 2.) wint
	// 3.) winnt/system32
	// 4.) path
	HMODULE hModule = ::LoadLibrary(strLexer);
	// if load fails get an extended error message 
	if (hModule == NULL)
	{
		LPVOID lpMsgBuf;
		::FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
			NULL, GetLastError(), MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPTSTR)&lpMsgBuf, 0, NULL);
		// Write to stderr
		TRACE(_T("%s:%s\n"), (LPCSTR)strLexer, (LPCTSTR)lpMsgBuf);
		// Free the buffer.
		LocalFree(lpMsgBuf);
		return NULL;
	}
	return hModule;
}
/////////////////////////////////////
// @mfunc Reset the Scintiall control and add new Text
// @rvalue void | not used
//
void CScintillaWnd::SetText(LPCSTR szText) //@parm pointer to new text	
{
	//SendMessage(SCI_SETTEXT, 0, (LPARAM)szText);
	LRESULT lResult = 0;
	if (szText != NULL)
	{
		lResult = SendMessage(SCI_SETTEXT, 0, (LPARAM)szText);
		DWORD len = strlen(szText);
		CString str = szText;
		DWORD End = 0;
		for (size_t i = 0; i < len; i++)
		{
			if (str.Mid(i, 2) == "\r\n")
			{//CR LF 
				//TT
				//End = i + 2;
				SendMessage(SCI_SETEOLMODE, SC_EOL_CRLF, 0);
			}
			else if (str.Mid(i, 2) == "\n")
			{//LF 
				SendMessage(SCI_SETEOLMODE, SC_EOL_LF, 0);
			}
			else if (str.Mid(i, 2) == "\r")
			{//CR 
				SendMessage(SCI_SETEOLMODE, SC_EOL_CR, 0);
			}
			End = i + 2;
		}
		GotoPosition(0);

		SendMessage(SCI_EMPTYUNDOBUFFER);
		//SendMessage(SCI_CONVERTEOLS, SC_EOL_CRLF);
		DeleteAllBookmark();

		::SendMessage(m_DialogHwnd, WM_REFRESHBUTTONSTATES_EOL, 0, 0);
	}

	//SendMessage(SCI_EMPTYUNDOBUFFER);
}
/////////////////////////////////////
// @mfunc Get the text from the control
// @rvalue void | not used
//
void CScintillaWnd::GetText(CString &strText) //@parm handle to receive text	
{
	LPSTR szText = GetText();
	if (szText != NULL)
	{
		strText = szText;
		delete[] szText;
	}
}
/////////////////////////////////////
// @mfunc Get the text from the control
// @rvalue LPSTR | a character string with text from the control - NULL on error - the caller has to free pointer
//
LPSTR CScintillaWnd::GetText()
{
	long lLen = SendMessage(SCI_GETLENGTH, 0, 0) + 1;
	if (lLen > 0)
	{
		TCHAR *pReturn = new TCHAR[lLen];
		if (pReturn != NULL)
		{
			*pReturn = '\0';
			SendMessage(SCI_GETTEXT, lLen, (long)pReturn);
			return pReturn;
		}
	}
	return NULL;
}
/////////////////////////////////////
// @mfunc Try to load a new file
// @rvalue BOOL | FALSE on error - TRUE on success
//
BOOL CScintillaWnd::LoadFile(LPCSTR szPath) //@parm filename of to load
{																							//ROSAK
	// if pathname is empty do nothing
	if (szPath == NULL || *szPath == '\0')
		return TRUE;
	// try to get extension and figure out what lexer to use
	CString strFile(szPath);
	int nIndex = strFile.ReverseFind('.');
	CString strExtension = strFile.Right(strFile.GetLength() - nIndex - 1);
	SendMessage(SCI_SETLEXER, GetFormatFromExtension(strExtension), 0);

	BOOL bReturn = TRUE;
	// ty to open file in sharing mode
	/* ::ifstream file;
	long len = 0L;
	long nTotal;
	TCHAR *szBuffer = NULL;
	file.open(szPath, ios::in, filebuf::sh_read);
	// ok success - try to get length of file
	if (file.is_open())
	{
	len = _filelength(file.fd());
	if (len > 0)
	{
	//       alloc new buffer of sie = filesize+1 for termination NULL
	szBuffer = new TCHAR[len+1];
	if (szBuffer != NULL)
	{
	file.read(szBuffer, len);
	nTotal = file.gcount();
	if (nTotal > 0 && nTotal <= len)
	szBuffer[nTotal] = '\0';
	//          read error
	if (file.bad())
	{
	file.close();
	bReturn = FALSE;
	}
	}
	else
	{
	file.close();
	bReturn = FALSE;
	}
	}
	file.close();
	//    set text to control
	SetText (szBuffer);
	//    tell scintilla that we have an unmodified document
	SendMessage(SCI_SETSAVEPOINT,0 , 0);
	GotoPosition(0);

	}
	// file open error - return
	else
	bReturn = FALSE;
	// clean up
	if (szBuffer != NULL)
	delete [] szBuffer;*/
	return bReturn;
}
/////////////////////////////////////
// @mfunc Try to save the file
// @rvalue BOOL | FALSE on error - TRUE on success
//
BOOL CScintillaWnd::SaveFile(LPCSTR szPath) //@parm filename to save to
{
	::ofstream file;
	file.open(szPath);
	if (file.fail())
	{
		return FALSE;
	}
	int buflen = SendMessage(SCI_GETLENGTH) + 1; //last NULL
	TCHAR *pBuffer = new TCHAR[buflen];
	if (pBuffer != NULL)
	{
		SendMessage(SCI_GETTEXT, buflen, (long)pBuffer);
		AfxMessageBox(pBuffer);
		file.write(pBuffer, buflen - 1);

		delete[] pBuffer;
	}
	file << ends;
	file.close();
	return TRUE;
}
BOOL CScintillaWnd::SendSaveFile(LPCSTR szPath) //@parm filename to save to

{
	::ofstream file;
	file.open(szPath);
	if (file.fail())
	{
		return FALSE;
	}
	int buflen = SendMessage(SCI_GETLENGTH) + 1; //last NULL
	TCHAR *pBuffer = new TCHAR[buflen];
	if (pBuffer != NULL)
	{
		SendMessage(SCI_GETTEXT, buflen, (long)pBuffer);
		//AfxMessageBox(pBuffer);
		file.write(pBuffer, buflen - 1);

		delete[] pBuffer;
	}
	file << ends;
	file.close();
	return TRUE;
}
/////////////////////////////////////
// @mfunc Try to find format for lexer by looking at the file extension.<nl>
// See global arrays at top of file.
// @rvalue int | Scintilla integer format for lexer
//
int CScintillaWnd::GetFormatFromExtension(LPCSTR szExtension) //@parm filename extension without dot e.g. "cpp"                                           
{
	int i = 0;
	TCHAR *pExtension = szExtensions[i];
	while (pExtension != NULL)
	{
		CTokenizer st(szExtensions[i], _T("|"));
		while (st.HasMoreTokens())
		{
			if (st.Next().CompareNoCase(szExtension) == 0)
				return nFormats[i];
		}
		i++;
		pExtension = szExtensions[i];
	}
	return SCLEX_NULL;
}
/////////////////////////////////////
// @mfunc Try to calculate the number of characters needed for the display of the linenumbers.
// This function returns 3 for 10-99 lines in file, 4 for 100-999 lines and so on.
// @rvalue int | number of characters needed to display linenumbers
//

/////////////////////////////////////
// @mfunc Set the display of line numbers on or off.
// Scintilla uses 3 "Margin" at the left of the edit window. The Margin (0)
// is used to display the linenumbers. If set to 0 no numbers are displayed. If
// set to a width > 0 - linenumbers are displayed. We use a helper function to 
// calculate the size of this margin.
// @rvalue int | number of pixels for the margin width of margin (0) 
//
void CScintillaWnd::SetDisplayLinenumbers(
	BOOL bFlag) //@parm flag if we shuld display line numbers
{
	// if nothing changes just return
	if (GetDisplayLinenumbers() == bFlag)
		return;
	// if display is turned off we set margin 0 to 0
	if (false==bFlag)
	{
		SendMessage(SCI_SETMARGINWIDTHN, 0, 0);
	}
	// if display is turned o we set margin 0 to the calculated width
	else
	{
		//int nWidth = GetLinenumberWidth() + 4;
		auto linesVisible = SendMessage(SCI_LINESONSCREEN);
		if (linesVisible)
		{
			int nbDigits = 0;
			auto firstVisibleLineVis = SendMessage(SCI_GETFIRSTVISIBLELINE);
			auto lastVisibleLineVis = linesVisible + firstVisibleLineVis + 1;
			auto lastVisibleLineDoc = SendMessage(SCI_DOCLINEFROMVISIBLE, lastVisibleLineVis);

			nbDigits = nbDigitsFromNbLines(lastVisibleLineDoc);
			nbDigits = nbDigits < 2 ? 2 : nbDigits;
			auto pixelWidth = 8 + nbDigits * SendMessage(SCI_TEXTWIDTH, STYLE_LINENUMBER, reinterpret_cast<LPARAM>("8"));
			SendMessage(SCI_SETMARGINWIDTHN, 0, pixelWidth);
		}
	}
	m_bLinenumbers = bFlag;
}
/////////////////////////////////////
// @mfunc Cut the selected text to the clipboard
// @rvalue void | not used
//
void CScintillaWnd::Cut()
{
	SendMessage(SCI_CUT, 0, 0);
}
/////////////////////////////////////
// @mfunc Copy the selected text to the clipboard
// @rvalue void | not used
//
void CScintillaWnd::Copy()
{
	SendMessage(SCI_COPY, 0, 0);
}
/////////////////////////////////////
// @mfunc Paste the text from the clipboard to the control
// @rvalue void | not used
//
void CScintillaWnd::Paste()
{
	//TT
	SendMessage(SCI_PASTE, 0, 0);
}
/////////////////////////////////////
// @mfunc Delete the selected text
// @rvalue void | not used
//
void CScintillaWnd::Clear()
{
	SendMessage(SCI_CLEAR, 0, 0);
}
/////////////////////////////////////
// @mfunc Select the complete text
// @rvalue void | not used
//
void CScintillaWnd::SelectAll()
{
	SendMessage(SCI_SELECTALL, 0, 0);
}
/////////////////////////////////////
// @mfunc Undo the last action
// @rvalue void | not used
//
void CScintillaWnd::Undo()
{
	//TT
	SendMessage(SCI_UNDO, 0, 0);
}
/////////////////////////////////////
// @mfunc Redo the last undone action
// @rvalue void | not used
//
void CScintillaWnd::Redo()
{
	SendMessage(SCI_REDO, 0, 0);
}
/////////////////////////////////////
// @mfunc Returns a flag if we can undo the last action
// @rvalue BOOL | TRUE if we can undo - else FALSE
//
BOOL CScintillaWnd::CanUndo()
{
	return SendMessage(SCI_CANUNDO, 0, 0) != 0;
}
/////////////////////////////////////
// @mfunc Returns a flag if we can redo the last undone action
// @rvalue BOOL | TRUE if we can redo - else FALSE
//
BOOL CScintillaWnd::CanRedo()
{
	return SendMessage(SCI_CANREDO, 0, 0) != 0;
}
/////////////////////////////////////
// @mfunc Returns a flag if there is text in the clipboard which we can paste
// @rvalue BOOL | TRUE if the clipboard contains text to paste - else FALSE
//

BOOL CScintillaWnd::CanPaste()
{
	return SendMessage(SCI_CANPASTE, 0, 0) != 0;
}
/////////////////////////////////////
// @mfunc Get the current line number - this the with the caret in it
// @rvalue long | line number with the caret in it - starts with 1
//
long CScintillaWnd::GetCurrentLine()
{
	long lPos = SendMessage(SCI_GETCURRENTPOS, 0, 0);
	return SendMessage(SCI_LINEFROMPOSITION, lPos, 0) + 1;
}
/////////////////////////////////////
// @mfunc Get the current column number = position of the caret within the line.
// This return value my be affected by the TAB setting! Starts with 1
// @rvalue long | current column number
//
long CScintillaWnd::GetCurrentColumn()
{
	long lPos = SendMessage(SCI_GETCURRENTPOS, 0, 0);
	return SendMessage(SCI_GETCOLUMN, lPos, 0) + 1;
}
/////////////////////////////////////
// @mfunc Return the current character position within the file.
// @rvalue long | current character position
//
long CScintillaWnd::GetCurrentPosition()
{
	return SendMessage(SCI_GETCURRENTPOS, 0, 0);
}
/////////////////////////////////////
// @mfunc Return the current style at the caret
// @rvalue int | the current style index (0...127)
//
int CScintillaWnd::GetCurrentStyle()
{
	long lPos = SendMessage(SCI_GETCURRENTPOS, 0, 0);
	return SendMessage(SCI_GETSTYLEAT, lPos, 0);
}
/////////////////////////////////////
// @mfunc Return the current folding level at the caret line
// @rvalue int | the current folding level (0...n)
//
int CScintillaWnd::GetCurrentFoldinglevel()
{
	long lLine = GetCurrentLine();
	int level = (SendMessage(SCI_GETFOLDLEVEL, lLine, 0)) & SC_FOLDLEVELNUMBERMASK;
	return level - 1024;
}
/////////////////////////////////////
// @mfunc set the fontname e.g. "Arial"
// @rvalue void | not used
//
void CScintillaWnd::SetFontname(int nStyle, LPCSTR szFontname)
{
	SendMessage(SCI_STYLESETFONT, nStyle, (long)szFontname);
}
/////////////////////////////////////
// @mfunc Set the font height in points
// @rvalue void | not used
//
void CScintillaWnd::SetFontheight(int nStyle, int nHeight)
{
	SendMessage(SCI_STYLESETSIZE, nStyle, (long)nHeight);
}
long CScintillaWnd::GetFontheight()
{
	return SendMessage(SCI_STYLEGETSIZE, 0, 0);
}
/////////////////////////////////////
// @mfunc Set the foreground color
// @rvalue void | not used
//
void CScintillaWnd::SetForeground(int nStyle, COLORREF crForeground)
{
	SendMessage(SCI_STYLESETFORE, nStyle, (long)crForeground);
}
/////////////////////////////////////
// @mfunc set the backgroundcolor
// @rvalue void | not used
//
void CScintillaWnd::SetBackground(int nStyle, COLORREF crBackground)
{
	SendMessage(SCI_STYLESETBACK, nStyle, (long)crBackground);
}
/////////////////////////////////////
// @mfunc set given style to bold
// @rvalue void | not used
//
void CScintillaWnd::SetBold(int nStyle, BOOL bBold)
{
	SendMessage(SCI_STYLESETBOLD, nStyle, (long)bBold);
}
/////////////////////////////////////
// @mfunc set given style to bold
// @rvalue void | not used
//
void CScintillaWnd::SetItalic(int nStyle, BOOL bItalic)
{
	SendMessage(SCI_STYLESETITALIC, nStyle, (long)bItalic);
}
/////////////////////////////////////
// @mfunc set given style to bold
// @rvalue void | not used
//
void CScintillaWnd::SetUnderline(int nStyle, BOOL bUnderline)
{
	SendMessage(SCI_STYLESETUNDERLINE, nStyle, (long)bUnderline);
}
/////////////////////////////////////
// @mfunc Return flag if we are in overstrike mode
// @rvalue BOOL | TRUE if we are in overstrike mode - else FALSE
//
BOOL CScintillaWnd::GetOverstrike()
{
	return SendMessage(SCI_GETOVERTYPE, 0, 0);
}
/////////////////////////////////////
// @mfunc set set overstrike mode (TRUE) or insert mode (FALSE)
// @rvalue void | not used
//
void CScintillaWnd::SetOverstrike(BOOL bOverstrike)
{
	SendMessage(SCI_SETOVERTYPE, bOverstrike, 0);
}
/////////////////////////////////////
// @mfunc Toggle the display of the selection bookmark margin
// @rvalue void | not used
//
void CScintillaWnd::SetDisplaySelection(BOOL bFlag)
{
	m_bSelection = bFlag;
	if (bFlag)
		SendMessage(SCI_SETMARGINWIDTHN, 1, 3);
	else
		SendMessage(SCI_SETMARGINWIDTHN, 1, 1);
}
/////////////////////////////////////
// @mfunc Toggle the display of the folding margin
// @rvalue void | not used
//
void CScintillaWnd::SetDisplayFolding(BOOL bFlag)
{
	m_bFolding = bFlag;
	if (bFlag)
		SendMessage(SCI_SETMARGINWIDTHN, 2, 16);
	else
		SendMessage(SCI_SETMARGINWIDTHN, 2, 0);
}

LRESULT CScintillaWnd::OnNcHitTest(CPoint point)
{
	CPoint ptHitClient = point;
	ScreenToClient(&ptHitClient);
	CRect rect;
	if (/*GetSizeGripperRect(rect) &&*/ rect.PtInRect(ptHitClient))
	{
		int nGripCY = GetSystemMetrics(SM_CYVSCROLL);
		if (ptHitClient.y >= rect.bottom - nGripCY)
			//	return HTBOTTOMLEFT;
			return HTLEFT;
	}
	//else if ( GetMoveGripperRect(rect) && rect.PtInRect(ptHitClient) )
	//{
	//	return HTCAPTION;
	//}
	//else
	//{
	//	CRect rcClient;
	//	GetClientRect(rcClient);
	//	if (ptHitClient.y >= rcClient.bottom - 5)
	//		return HTBOTTOM;
	//}
	return CWnd::OnNcHitTest(point);
}

//=============================================================================	
BEGIN_MESSAGE_MAP(CScintillaWnd, CWnd)
	//=============================================================================	
	//{{AFX_MSG_MAP(CXListBox)
	//ON_WM_LBUTTONDBLCLK()
	ON_WM_NCHITTEST()
	ON_WM_KEYDOWN()
	ON_WM_CONTEXTMENU()
	ON_COMMAND(ID_EDIT_CUT,Cut)
	ON_COMMAND(ID_EDIT_COPY, Copy)
	ON_COMMAND(ID_EDIT_CLEAR, Clear)
	ON_COMMAND(ID_EDIT_SELECT_ALL, SelectAll)
	ON_COMMAND(ID_EDIT_PASTE, Paste)
	ON_CONTROL_REFLECT(SCEN_CHANGE, OnChange)
	ON_WM_HSCROLL()
	ON_WM_VSCROLL()

	ON_MESSAGE(WM_PASTE, OnPaste)

	ON_WM_LBUTTONDOWN()
	ON_WM_RBUTTONDOWN()
	//ON_WM_ERASEBKGND()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

void CScintillaWnd::ShowAutoComplete(const char ch)
{
	if (SendMessage(SCI_AUTOCACTIVE, 0, 0) != 0)
		return;

	long lStart = SendMessage(SCI_GETCURRENTPOS, 0, 0);
	int startPos = SendMessage(SCI_WORDSTARTPOSITION, lStart, true);

	if (ch == '.')
	{
		/*SStringA str = g_pMainDlg->m_UIResFileMgr.GetSkinAutos(_T(""));
		if (!str.IsEmpty())
		{
			SendMessage(SCI_AUTOCSHOW, lStart - startPos, (LPARAM)(LPCSTR)str);
		}*/
	}
	else if (ch == '/')
	{
		//int startPos = SendEditor(SCI_WORDSTARTPOSITION, lStart-1, true);
		/*SStringA clsName = GetNotePart(lStart - 1);
		SStringA str;
		if (clsName.IsEmpty())
			str = g_pMainDlg->m_UIResFileMgr.GetStyleAutos(_T(""));
		else if (clsName.CompareNoCase("color") == 0)
			str = g_pMainDlg->m_UIResFileMgr.GetColorAutos(_T(""));
		else if (clsName.CompareNoCase("string") == 0)
			str = g_pMainDlg->m_UIResFileMgr.GetStringAutos(_T(""));

		if (!str.IsEmpty())
		{
			SendEditor(SCI_AUTOCSHOW, lStart - startPos, (LPARAM)(LPCSTR)str);
		}*/
	}
	else if (ch == '<')
	{
		/*SStringA str = g_SysDataMgr.GetCtrlAutos();
		if (!str.IsEmpty())
		{
			SendEditor(SCI_AUTOCSHOW, lStart - startPos, (LPARAM)(LPCSTR)str);
		}*/
	}
	else if (ch >= 'a' && ch <= 'z')
	{
		/*SStringT tagname = GetHtmlTagname();
		if (!tagname.IsEmpty())
		{
			SStringA str = g_SysDataMgr.GetCtrlAttrAutos(tagname);
			if (!str.IsEmpty())
			{	// 自动完成字串要进行升充排列, 否则功能不正常
				SendEditor(SCI_AUTOCSHOW, lStart - startPos, (LPARAM)(LPCSTR)str);
			}
		}*/
	}
}

LRESULT CScintillaWnd::OnNotify(int idCtrl, LPNMHDR pnmh)
{
	if (pnmh->hwndFrom != m_hWnd) return 0;
	LRESULT lRet = 0;
	SCNotification *pSCNotification = (SCNotification*)pnmh;
	switch (pnmh->code)
	{
	case SCI_NULL:TT
		break;
	case SCN_MARGINCLICK:
	{
							int nLine = SendMessage(SCI_LINEFROMPOSITION, (WPARAM)pSCNotification->position);
							SendMessage(SCI_TOGGLEFOLD, (WPARAM)nLine);
	}
		break;

	case SCN_MODIFIED:
	{
						 const char *pp = pSCNotification->text;
						 if (pp)	//判断是否是文字改变
						 {
							 UpdateLineNumberWidth();
							 SetDirty(true);
						 }
	}
		break;

#ifdef SCN_AUTOCCOMPLETED
		// SOUI内核带Scintilla没实现这个, 需要 Scintilla 3.6.0 以上版本
	case SCN_AUTOCCOMPLETED:
	{
							   const char *pp = pSCNotification->text;
							   if (pp)
							   {
								   char name[8] = { 0 };
								   Sci_TextRange sci_tr;
								   sci_tr.chrg.cpMin = pSCNotification->position - 2;
								   sci_tr.chrg.cpMax = pSCNotification->position;
								   sci_tr.lpstrText = name;
								   SendEditor(SCI_GETTEXTRANGE, 0, reinterpret_cast<LPARAM>(&sci_tr));
								   if ((strcmp(name, "\".") == 0) || (strcmp(name, "\"/") == 0))
								   {
									   SendEditor(SCI_DELETERANGE, pSCNotification->position - 1, 1);
								   }
							   }
	}
		break;
#endif // SCN_AUTOCCOMPLETED


	case SCN_CHARADDED:
	{
						  BOOL bReadonly = (BOOL)SendMessage(SCI_GETREADONLY);
						  if (!bReadonly)
							  SetDirty(true);

						  char pp = tolower(pSCNotification->ch);
						  ShowAutoComplete(pp);
	}
		break;

	case SCN_UPDATEUI:
	{
						 doMatch();
	}
		break;

	default:
		break;
	}
	return lRet;
}


void CScintillaWnd::commentBlockToolStripMenuItem_Click()
{

	int textLength = strlen(GetSelectedText());
	if (textLength > 0)
	{
		long lLen = 0;
		long f = GetSelectionStart();
		long t = GetSelectionEnd();
		for (int i = f; i <= t; i++)
		{
			SendMessage(SCI_INSERTTEXT, -1, reinterpret_cast<LPARAM>("// "));
			GotoLine(GetCurrentLine() + 1);
		}
		SendMessage(SCI_SETTARGETSTART, GetSelectionStart());
		SendMessage(SCI_SETTARGETEND, GetSelectionEnd());
	}

	/*long lPos = GetCurrentPosition();
	long startLine = GetSelectionStart();
	if (lPos >= 0)
	{
		SendMessage(SCI_VCHOME);
		SendMessage(SCI_INSERTTEXT, 0, reinterpret_cast<LPARAM>("// "));
		GotoLine(GetCurrentLine());
	}*/

	/*long lLen = (GetSelectionEnd() - GetSelectionStart());
	if (lLen> 0)
	{
		int f = GetSelectionStart();
		int t = GetSelectionEnd();
		for (int i = f; i <= t; i++)
		{
			SendMessage(SCI_INSERTTEXT, f, reinterpret_cast<LPARAM>("// "));			
			SendMessage(SCI_VCHOME);
		}
		SendMessage(SCI_SETTARGETSTART, GetSelectionStart());
		SendMessage(SCI_SETTARGETEND, GetSelectionEnd());
	}*/
}
int CScintillaWnd::TestComment()
{
	BOOL bUseSelection = TRUE;
	LPCSTR szFind("// ");
	LPCSTR szReplace("");
	int nCount = 0;
	if (bUseSelection)
	{
		//    get starting selection range
		long lLen = 0;
		long lStart = GetSelectionStart();
		long lEnd = GetSelectionEnd();
		//    set target to selection
		SendMessage(SCI_SETTARGETSTART, lStart);
		SendMessage(SCI_SETTARGETEND, lEnd);
		//    try to find text in target for the first time
		long lPos = SendMessage(SCI_SEARCHINTARGET, strlen(szFind), (long)szFind);
		//    loop over selection until end of selection reached - moving the target start each time
		while (lPos < lEnd && lPos >= 0)
		{
			//SendMessage(SCI_VCHOME);
			if (m_nSearchflags & SCFIND_REGEXP) // check for regular expression flag
				lLen = SendMessage(SCI_REPLACETARGETRE, strlen(szReplace), (long)szReplace);
			else
				lLen = SendMessage(SCI_REPLACETARGET, strlen(szReplace), (long)szReplace);
			//       the end of the selection was changed - recalc the end
			lEnd = GetSelectionEnd();
			//       move start of target behind last change and end of target to new end of selection
			SendMessage(SCI_SETTARGETSTART, SCI_VCHOME /*lPos + lLen*/);
			SendMessage(SCI_SETTARGETEND, lEnd);
			//       find again - if nothing found loop exits
			lPos = SendMessage(SCI_SEARCHINTARGET, strlen(szFind), (long)szFind);
			nCount++;
		}
	}
	return nCount;
}
void CScintillaWnd::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	__super::OnKeyDown(nChar, nRepCnt, nFlags);

	if (IsAltPressed())
	{
		switch (nChar)
		{
		case 'Q':
			{
				if (MessageBox("Are you sure to quit ?", "info", MB_YESNO) == IDNO)
				{
					return;
				}
				else
				{
					exit(0);
				}
			}
			break;
		}
	}
	if (IsCtrlPressed())
	{
		switch (nChar)
			{
		case 'S':	break;
		case 'H':	::SendMessage(m_DialogHwnd, WM_COMMAND, IPAKMAN_BUTTON_REPLACE, 0); break;
		case 'F':	::SendMessage(m_DialogHwnd, WM_CONTROLPLUSF, 0, 0); break;
		case 'Q':
		{
					TestComment();
					//SendMessage(SCI_VCHOME);
					//CString sFooter;
					//sFooter.Replace(_T("// "), _T(""));
					//SendMessage(SCI_INSERTTEXT, -1, reinterpret_cast<LPARAM>("// "));
		}	 
			break;
			}
	}
	


	/*if (nChar == 'S' && IsCtrlPressed())
	{		
		
	}
	else if (nChar == 'F' && IsCtrlPressed())
	{
		::SendMessage(m_DialogHwnd, WM_CONTROLPLUSF, 0, 0);
	}
	else if (nChar == 'G' && IsCtrlPressed())
	{
		SendMessage(SCI_VCHOME);
	}*/
	// 不加这个, 默认是Handled消息 Scintilla功能会不正常
	//::SetMsgHandled(FALSE);
	/*if ((GetStyle()&ES_READONLY) == ES_READONLY)
	{
		//CWnd::OnKeyDown(nChar, nRepCnt, nFlags);
		return;
	}
	BOOL bIsShiftKeyDown = ::GetAsyncKeyState(VK_SHIFT)<0;
	BOOL bIsCtrlKeyDown = ::GetAsyncKeyState(VK_CONTROL)<0;

	if (nChar == 'G')
	{
		//TT
		__super::OnKeyDown(nChar, nRepCnt, nFlags);
		if (bIsCtrlKeyDown)
		{
			SendMessage(WM_PASTE);
		}
	}
	else if (nChar == 'Z')
	{
		//TT
		__super::OnKeyDown(nChar, nRepCnt, nFlags);
		if (bIsCtrlKeyDown)
		{
			Undo();
			//SendMessage(WM_PASTE);
		}
	}
	else
	{
		CWnd::OnKeyDown(nChar, nRepCnt, nFlags);
	}*/
}
LONG CScintillaWnd::OnPaste(UINT wParam, LONG lParam)
{
	//TT
	//UNREFERENCED_PARAMETER(wParam);
	//UNREFERENCED_PARAMETER(lParam);

	Paste();
	/*int nSelectionStart = 0;
	int nSelectionEnd = 0;
	GetSel(nSelectionStart, nSelectionEnd);
	CWnd::Default();
	CString sNewString;
	GetWindowText(sNewString);
	if (nSelectionStart != nSelectionEnd)
	{
		CString sLeftLiterals = GetLeftLiterals();
		CString sRightLiterals = GetRightLiterals();
		if (sNewString.Find(sLeftLiterals) == 0)
			sNewString = sNewString.Mid(sLeftLiterals.GetLength());
		int nPosition = sNewString.Find(sRightLiterals);
		if (nPosition != -1 &&
			nPosition + sRightLiterals.GetLength() == sNewString.GetLength())
			sNewString = sNewString.Left(nPosition);
	}
	EmptyData(TRUE);
	SetInputData(sNewString, 0);
	int nNewInsertionPoint = GetNextInputLocation(nSelectionStart);
	SetSel(nNewInsertionPoint, nNewInsertionPoint);*/



	//SetSel(-1);
	//SetSel(0, -1);
	//PostMessage(0,-1);
	return 0;
}
void CScintillaWnd::OnChange()
{
	//NotifyParent(EN_CHANGE);
	MarkLine(GetCurrentLine());
}
/////////////////////////////////////
// @mfunc Update UI and do brace matching
// @rvalue void | not used
//
void CScintillaWnd::UpdateUI()
{
	// do brace matching
	/*long lStart = SendMessage(SCI_GETCURRENTPOS, 0, 0);
	long lEnd = SendMessage(SCI_BRACEMATCH, lStart - 1, 0);
	// if there is a matching brace highlight it
	if (lEnd >= 0)
		SendMessage(SCI_BRACEHIGHLIGHT, lStart - 1, lEnd);
	// if there is NO matching brace erase old highlight
	else
		SendMessage(SCI_BRACEHIGHLIGHT, -1, -1);*/
}
void CScintillaWnd::findMatchingBracePos(int & braceAtCaret, int & braceOpposite)
{
	int caretPos = int(SendMessage(SCI_GETCURRENTPOS));
	braceAtCaret = -1;
	braceOpposite = -1;
	TCHAR charBefore = '\0';

	int lengthDoc = int(SendMessage(SCI_GETLENGTH));

	if ((lengthDoc > 0) && (caretPos > 0))
	{
		charBefore = TCHAR(SendMessage(SCI_GETCHARAT, caretPos - 1, 0));
	}
	// Priority goes to character before caret
	if (charBefore && _tcschr(TEXT("<>[](){}"), charBefore))
	{
		braceAtCaret = caretPos - 1;
	}

	if (lengthDoc > 0 && (braceAtCaret < 0))
	{
		// No brace found so check other side
		TCHAR charAfter = TCHAR(SendMessage(SCI_GETCHARAT, caretPos, 0));
		if (charAfter && _tcschr(TEXT("<>[](){}"), charAfter))
		{
			braceAtCaret = caretPos;
		}
	}
	if (braceAtCaret >= 0)
		braceOpposite = int(SendMessage(SCI_BRACEMATCH, braceAtCaret, 0));
}

bool CScintillaWnd::doMatch()
{
	int braceAtCaret = -1;
	int braceOpposite = -1;
	findMatchingBracePos(braceAtCaret, braceOpposite);

	if ((braceAtCaret != -1) && (braceOpposite == -1))
	{
		SendMessage(SCI_BRACEBADLIGHT, braceAtCaret);
		SendMessage(SCI_SETHIGHLIGHTGUIDE, 0);
	}
	else
	{
		SendMessage(SCI_BRACEHIGHLIGHT, braceAtCaret, braceOpposite);
		//SendMessage(SCI_SETSEL, braceAtCaret, braceOpposite+1);
		//if (_pEditView->isShownIndentGuide())
		{
			int columnAtCaret = int(SendMessage(SCI_GETCOLUMN, braceAtCaret));
			int columnOpposite = int(SendMessage(SCI_GETCOLUMN, braceOpposite));
			SendMessage(SCI_SETHIGHLIGHTGUIDE, (columnAtCaret < columnOpposite) ? columnAtCaret : columnOpposite);
		}
	}

	return (braceAtCaret != -1);
}
void CScintillaWnd::UpdateLineNumberWidth(void)
{
	if (FALSE == theApp.GetGameAI()->sPage1.m_nLineNumber)
		return;
	auto linesVisible = SendMessage(SCI_LINESONSCREEN);
	if (linesVisible)
	{
		int nbDigits = 0;

		//if (1)
		if (theApp.GetGameAI()->sPage1.m_nLineNumber)
		{
			auto firstVisibleLineVis = SendMessage(SCI_GETFIRSTVISIBLELINE);
			auto lastVisibleLineVis = linesVisible + firstVisibleLineVis + 1;
			auto lastVisibleLineDoc = SendMessage(SCI_DOCLINEFROMVISIBLE, lastVisibleLineVis);

			nbDigits = nbDigitsFromNbLines(lastVisibleLineDoc);
			nbDigits = nbDigits < 2 ? 2 : nbDigits;
		}
		else
		{
			auto nbLines = SendMessage(SCI_GETLINECOUNT);
			nbDigits = nbDigitsFromNbLines(nbLines);
			nbDigits = nbDigits < 3 ? 3 : nbDigits;
		}

		auto pixelWidth = 8 + nbDigits * SendMessage(SCI_TEXTWIDTH, STYLE_LINENUMBER, reinterpret_cast<LPARAM>("8"));
		SendMessage(SCI_SETMARGINWIDTHN, 0, pixelWidth);
	}
}
/*BOOL CScintillaWnd::OnNotify
(
WPARAM wParam,
LPARAM lParam,
LRESULT* pResult
)
{
	NMHDR *phDR;
	phDR = (NMHDR*)lParam;
	if (phDR != NULL && phDR->hwndFrom == m_hWnd)
	{
		SCNotification *pMsg = (SCNotification*)lParam;
		switch (phDR->code)
		{
		case SCN_STYLENEEDED:
			break;
		case SCN_CHARADDED:
			break;
		case SCN_SAVEPOINTREACHED:
			break;
		case SCN_SAVEPOINTLEFT:
			break;
		case SCN_MODIFYATTEMPTRO:
			break;
		case SCN_KEY:
			break;
		case SCN_DOUBLECLICK:
			break;
		case SCN_ZOOM:
		{
						 if (theApp.GetGameAI()->sPage1.m_nLineNumber)
							 UpdateLineNumberWidth();
		}
			break;
		case SCN_UPDATEUI:
		{
							 if (theApp.GetGameAI()->sPage1.m_nLineNumber)
								 UpdateLineNumberWidth();
							 doMatch();
		}
			break;
		case SCN_MODIFIED:
		{
							 TT
		}
			break;
		case SCN_MACRORECORD:
			break;
		case SCN_MARGINCLICK:
		{
								DoDefaultFolding(pMsg->margin, pMsg->position);
		}
			break;
		case SCN_NEEDSHOWN:
			break;
		case SCN_PAINTED:
			break;
		case SCN_USERLISTSELECTION:
			break;
		case SCN_URIDROPPED:
			break;
		case SCN_DWELLSTART:
			break;
		case SCN_DWELLEND:
			break;
		case SCI_PASTE:
			TT
				break;
		}
		return TRUE;
	}
	return CWnd::OnNotify(wParam, lParam, pResult);
}*/
/////////////////////////////////////
// @mfunc Do default folding for given margin and position - usually called from notification handler
// @rvalue void | not used
//
void CScintillaWnd::DoDefaultFolding(
	int nMargin,      //@parm maring we want to handle - normally (2)
	long lPosition)   //@parm character position where user clicked margin
{
	// simply toggle fold
	if (nMargin == 2)
	{
		long lLine = SendMessage(SCI_LINEFROMPOSITION, lPosition, 0);
		SendMessage(SCI_TOGGLEFOLD, lLine, 0);
	}
}
/////////////////////////////////////
// @mfunc Do default folding for given margin and position - usually called from notification handler
// @rvalue void | not used
//
void CScintillaWnd::Refresh()
{
	SendMessage(SCI_COLOURISE, 0, -1);
}
/////////////////////////////////////
// @mfunc Add a bookmark at given line
// @rvalue void | not used
//
void CScintillaWnd::AddBookmark(
	long lLine) //@parm line where to add bookmark - lines start at 1
{
	SendMessage(SCI_MARKERADD, lLine - 1, 0);
}
/////////////////////////////////////
// @mfunc Delete a bookmark at given line
// @rvalue void | not used
//
void CScintillaWnd::DeleteBookmark(
	long lLine) //@parm line where to delete bookmark - lines start at 1
{
	if (HasBookmark(lLine))
	{
		SendMessage(SCI_MARKERDELETE, lLine - 1, 0);
	}
}
void CScintillaWnd::DeleteAllBookmark() //@parm line where to delete bookmark - lines start at 1
{
	//if (HasBookmark(lLine))
	{
		SendMessage(SCI_MARKERDELETEALL, 0, 0);
	}
}
/////////////////////////////////////
// @mCustom modified document marker
// @ BY JACK
//
void CScintillaWnd::MarkLine(long lLine,long pLine) //@parm line where to delete bookmark - lines start at 1
{

	if (!HasBookmark(lLine))
	{
		SendMessage(SCI_MARKERADD, lLine - 1, pLine);
	}
}
/////////////////////////////////////
// @mfunc Check if given line has a bookmark
// @rvalue BOOL | TRUE if given line has a bookmark - else FALSE
//
BOOL CScintillaWnd::HasBookmark(long lLine, long pLine) //@parm line where to add bookmark - lines start at 1

{
	int n = SendMessage(SCI_MARKERGET, lLine - 1, pLine);
	// check mask for markerbit 0
	if (n & 0x1)
		return TRUE;
	return FALSE;
}
/////////////////////////////////////
// @mfunc Find next bookmark from current line
// @rvalue void | not used
//
void CScintillaWnd::FindNextBookmark()
{
	long lLine = SendMessage(SCI_MARKERNEXT, GetCurrentLine(), 1);
	if (lLine >= 0)
		SendMessage(SCI_GOTOLINE, lLine, 0);
}
/////////////////////////////////////
// @mfunc Find previous bookmark from current line
// @rvalue void | not used
//
void CScintillaWnd::FindPreviousBookmark()
{
	long lLine = SendMessage(SCI_MARKERPREVIOUS, GetCurrentLine() - 2, 1);
	if (lLine >= 0)
		SendMessage(SCI_GOTOLINE, lLine, 0);
}
/////////////////////////////////////
// @mfunc Goto givven character position
// @rvalue void | not used
//
void CScintillaWnd::GotoPosition(
	long lPos) //@parm new character position
{
	SendMessage(SCI_GOTOPOS, lPos, 0);
}
/////////////////////////////////////
// @mfunc set codepage
// @ BY JACK
//
void CScintillaWnd::SetCodePage(
	long lPos) //@parm new character position
{
	//SendMessage(SCI_SETCODEPAGE, lPos);
	SendMessage(static_cast<UINT>(SCI_SETCODEPAGE), static_cast<WPARAM>(lPos), 0);
}
void CScintillaWnd::SetCharacterSet(long style, long charset)
{
	SendMessage(SCI_STYLESETCHARACTERSET, static_cast<WPARAM>(style), static_cast<LPARAM>(charset));
}
void CScintillaWnd::SetFontLocale(
	LPSTR lpFileName) //@parm new character position
{
	//SendMessage(SCI_SETCODEPAGE, lPos);
	//SendMessage(static_cast<UINT>(SCI_SETFONTLOCALE), static_cast<WPARAM>(lpFileName), 0);
}
/////////////////////////////////////
// @mfunc Goto givven line position
// @rvalue void | not used
//
void CScintillaWnd::GotoLine(
	long lLine) //@parm new line - lines start at 1
{
	SendMessage(SCI_GOTOLINE, lLine - 1, 0);
}
/////////////////////////////////////
// @mfunc Search forward for a given string and select it if found. You may use regular expressions on the text.
// @rvalue BOOL | TRUE if text is ffound else FALSE
//
BOOL CScintillaWnd::SearchForward(LPSTR szText) //@parm text to search	
{
	if (szText == NULL)
		return FALSE;
	long lPos = GetCurrentPosition();
	TextToFind tf;
	tf.lpstrText = szText;
	tf.chrg.cpMin = lPos /*+ 1*/;
	tf.chrg.cpMax = SendMessage(SCI_GETLENGTH, 0, 0)+1;
	lPos = SendMessage(SCI_FINDTEXT, m_nSearchflags, (long)&tf);
	if (lPos >= 0)
	{
		//SetFocus();
		GotoPosition(lPos);
		SendMessage(SCI_SETSEL, tf.chrgText.cpMin, tf.chrgText.cpMax);
		SendMessage(SCI_FINDTEXT, m_nSearchflags, (long)&tf);
		return TRUE;
	}
	//SendMessage(SCI_SETCURRENTPOS, 0, 0);
	//GotoPosition(0);
	//SetSel(0);
	//return SearchForward(szText);
	return FALSE;
}
void CScintillaWnd::getText(char *dest, int start, int end) const
{
	TextRange tr;
	tr.chrg.cpMin = start;
	tr.chrg.cpMax = end;
	tr.lpstrText = dest;
	SendMessage(SCI_GETTEXTRANGE, 0, reinterpret_cast<LPARAM>(&tr));
}
bool CScintillaWnd::findCharInRange(unsigned char beginRange, unsigned char endRange, int startPos, bool direction, bool wrap)
{
	int totalSize = getCurrentDocLen();
	if (startPos == -1)
		startPos = direction == dirDown ? 0 : totalSize - 1;
	if (startPos > totalSize)
		return false;

	char *content = new char[totalSize + 1];
	getText(content, 0, totalSize);
	int found = -1;

	for (int i = startPos - (direction == dirUp ? 1 : 0);
		(direction == dirDown) ? i < totalSize : i >= 0;
		(direction == dirDown) ? (i++) : (i--))
	{
		if ((unsigned char)content[i] >= beginRange && (unsigned char)content[i] <= endRange)
		{
			found = i;
			break;
		}
	}

	if (found == -1)
	{
		if (wrap)
		{
			for (int i = (direction == dirUp ? totalSize - 1 : 0);
				(direction == dirDown) ? i < totalSize : i >= 0;
				(direction == dirDown) ? (i++) : (i--))
			{
				if ((unsigned char)content[i] >= beginRange && (unsigned char)content[i] <= endRange)
				{
					found = i;
					break;
				}
			}
		}
	}

	if (found != -1)
	{
		//printInt(found);
		int sci_line = SendMessage(SCI_LINEFROMPOSITION, found);
		SendMessage(SCI_ENSUREVISIBLE, sci_line);
		SendMessage(SCI_GOTOPOS, found);
		SendMessage(SCI_SETSEL, (direction == dirDown) ? found : found + 1, (direction == dirDown) ? found + 1 : found);
	}
	delete[] content;
	return (found != -1);
}
void CScintillaWnd::SearchTest()
{
	int currentPos = SendMessage(SCI_GETCURRENTPOS);
	unsigned char startRange = 0;
	unsigned char endRange = 255;
	bool direction = dirDown;
	bool isWrap = true;
	if (!getRangeFromUI(startRange, endRange))
	{
		//STOP!
		::MessageBox(this->GetSafeHwnd(), TEXT("You should type between from 0 to 255."), TEXT("Range Value problem"), MB_OK);
		return ;
	}
	getDirectionFromUI(direction, isWrap);
	findCharInRange(startRange, endRange, currentPos, direction, isWrap);
}
void CScintillaWnd::getDirectionFromUI(bool & whichDirection, bool & isWrap)
{
	whichDirection = FALSE;// isCheckedOrNot(ID_FINDCHAR_DIRUP);
	isWrap = TRUE;// isCheckedOrNot(ID_FINDCHAR_WRAP);
}
bool CScintillaWnd::getRangeFromUI(unsigned char & startRange, unsigned char & endRange)
{
	BOOL bFlag=TRUE;
	if (!bFlag/*isCheckedOrNot(IDC_NONASCCI_RADIO)*/)
	{
		startRange = 128;
		endRange = 255;
		return true;
	}
	if (bFlag/*isCheckedOrNot(IDC_ASCCI_RADIO)*/)
	{
		startRange = 0;
		endRange = 127;
		return true;
	}

	if (!bFlag/*isCheckedOrNot(IDC_MYRANGE_RADIO)*/)
	{
		BOOL startBool, endBool;
		int start = 0;// ::GetDlgItemInt(_hSelf, IDC_RANGESTART_EDIT, &startBool, FALSE);
		int end = 0;// ::GetDlgItemInt(_hSelf, IDC_RANGEEND_EDIT, &endBool, FALSE);

		if (!startBool || !endBool)
			return false;
		if (start > 255 || end > 255)
			return false;
		if (start > end)
			return false;
		startRange = (unsigned char)start;
		endRange = (unsigned char)end;
		return true;
	}

	return false;
}
BOOL CScintillaWnd::SearchForwardReplace(LPSTR szText) //@parm text to search	
{
	if (szText == NULL)
		return FALSE;
	long lPos = GetCurrentPosition();
	TextToFind tf;
	tf.lpstrText = szText;
	tf.chrg.cpMin = lPos + 1;
	tf.chrg.cpMax = SendMessage(SCI_GETLENGTH, 0, 0);
	lPos = SendMessage(SCI_FINDTEXT, m_nSearchflags, (long)&tf);
	if (lPos >= 0)
	{
		//SetFocus();
		GotoPosition(lPos);
		SendMessage(SCI_SETSEL, tf.chrgText.cpMin, tf.chrgText.cpMax);
		SendMessage(SCI_FINDTEXT, m_nSearchflags, (long)&tf);
		return TRUE;
	}
	//SendMessage(SCI_SETCURRENTPOS, 0, 0);
	//GotoPosition(0);
	//SetSel(0);
	return FALSE;
}
void CScintillaWnd::SetSel(DWORD dwSelection, BOOL bNoScroll )
{
	SendMessage(SCI_SETSEL, dwSelection, 0);
}
void CScintillaWnd::SetSel(int nStartChar, int nEndChar,BOOL bNoScroll)
{
	SendMessage(SCI_SETSEL, nStartChar, nEndChar);
}
/////////////////////////////////////
// @mfunc Search backward for a given string and select it if found. You may use regular expressions on the text.
// @rvalue BOOL | TRUE if text is ffound else FALSE
//
BOOL CScintillaWnd::SearchBackward(LPSTR szText) //@parm text to search	
{
	if (szText == NULL)
		return FALSE;
	long lPos = GetCurrentPosition();
	long lMinSel = GetSelectionStart();
	TextToFind tf;
	tf.lpstrText = szText;
	if (lMinSel >= 0)
		tf.chrg.cpMin = lMinSel /*- 1*/;
	else
		tf.chrg.cpMin = lPos /*- 1*/;
	tf.chrg.cpMax = 0;
	lPos = SendMessage(SCI_FINDTEXT, m_nSearchflags, (long)&tf);
	if (lPos >= 0)
	{
		//SetFocus();
		GotoPosition(lPos);
		SendMessage(SCI_SETSEL, tf.chrgText.cpMin, tf.chrgText.cpMax);
		SendMessage(SCI_FINDTEXT, m_nSearchflags, (long)&tf);
		return TRUE;
	}
	return FALSE;
}

/////////////////////////////////////
// @mfunc Set lexer format - see scintilla.h
// @rvalue void | not used
//
void CScintillaWnd::SetLexer(
	int nLexer) //@parm lexer code e.g. 
{
	SendMessage(SCI_SETLEXER, nLexer, 0);
}
/////////////////////////////////////
// @mfunc Get start of selection (anchor) as character position
// @rvalue long | character position of selection start (anchor) - -1 on error
//
long CScintillaWnd::GetSelectionStart()
{
	return SendMessage(SCI_GETSELECTIONSTART, 0, 0);
}
/////////////////////////////////////
// @mfunc Get end of selection as character position
// @rvalue long | character position of selection end - -1 on error
//
long CScintillaWnd::GetSelectionEnd()
{
	return SendMessage(SCI_GETSELECTIONEND, 0, 0);
}
void CScintillaWnd::clearOccurrences()
{
	
	//SendMessage(SCI_SETINDICATORCURRENT, 0);
	//	SendMessage(SCI_INDICATORCLEARRANGE, 0, editor.Length)
}
/////////////////////////////////////
// @mfunc Get selected string
// @rvalue CString | string with currentliy selected text
//
CString CScintillaWnd::GetSelectedText()
{
	long lLen = (GetSelectionEnd() - GetSelectionStart()) + 1;
	if (lLen > 0)
	{
		TCHAR *p = new TCHAR[lLen + 1];
		if (p != NULL)
		{
			*p = '\0';
			SendMessage(SCI_GETSELTEXT, 0, (long)p);
			CString strReturn = p;
			delete[] p;
			return strReturn;
		}
	}
	return _T("");
}
/////////////////////////////////////
// @mfunc Replace a text with a new text. You can use regular expression with tagging on the replacement text.
// @rvalue void | not used
//

void CScintillaWnd::ReplaceSearchedText(
	LPCSTR szText) //@parm new text
{
	if (szText == NULL)
		return;
	SendMessage(SCI_TARGETFROMSELECTION, 0, 0);
	if (m_nSearchflags & SCFIND_REGEXP)
		SendMessage(SCI_REPLACETARGETRE, strlen(szText), (long)szText);
	else
		SendMessage(SCI_REPLACETARGET, strlen(szText), (long)szText);
}
/////////////////////////////////////
// @mfunc Replace a text in a selection or in the complete file multiple times
// @rvalue int | number of replacements
//
int CScintillaWnd::ReplaceAll(
	LPCSTR szFind,
	LPCSTR szReplace,
	BOOL bUseSelection)
{
	int nCount = 0;
	// different branches for replace in selection or total file
	if (bUseSelection)
	{
		//    get starting selection range
		long lLen = 0;
		long lStart = GetSelectionStart();
		long lEnd = GetSelectionEnd();
		//    set target to selection
		SendMessage(SCI_SETTARGETSTART, lStart);
		SendMessage(SCI_SETTARGETEND, lEnd);
		//    try to find text in target for the first time
		long lPos = SendMessage(SCI_SEARCHINTARGET, strlen(szFind), (long)szFind);
		//    loop over selection until end of selection reached - moving the target start each time
		while (lPos < lEnd && lPos >= 0)
		{
			if (m_nSearchflags & SCFIND_REGEXP) // check for regular expression flag
				lLen = SendMessage(SCI_REPLACETARGETRE, strlen(szReplace), (long)szReplace);
			else
				lLen = SendMessage(SCI_REPLACETARGET, strlen(szReplace), (long)szReplace);
			//       the end of the selection was changed - recalc the end
			lEnd = GetSelectionEnd();
			//       move start of target behind last change and end of target to new end of selection
			SendMessage(SCI_SETTARGETSTART, lPos + lLen);
			SendMessage(SCI_SETTARGETEND, lEnd);
			//       find again - if nothing found loop exits
			lPos = SendMessage(SCI_SEARCHINTARGET, strlen(szFind), (long)szFind);
			nCount++;
		}
	}
	else
	{
		//    start with first and last char in buffer
		long lLen = 0;
		long lStart = 0;
		long lEnd = SendMessage(SCI_GETTEXTLENGTH, 0, 0);
		//    set target to selection
		SendMessage(SCI_SETTARGETSTART, lStart, 0);
		SendMessage(SCI_SETTARGETEND, lEnd, 0);
		//    try to find text in target for the first time
		long lPos = SendMessage(SCI_SEARCHINTARGET, strlen(szFind), (long)szFind);
		//    loop over selection until end of selection reached - moving the target start each time
		while (lPos < lEnd && lPos >= 0)
		{
			if (m_nSearchflags & SCFIND_REGEXP) // check for regular expression flag
				lLen = SendMessage(SCI_REPLACETARGETRE, strlen(szReplace), (long)szReplace);
			else
				lLen = SendMessage(SCI_REPLACETARGET, strlen(szReplace), (long)szReplace);
			//       the end of the selection was changed - recalc the end
			lEnd = SendMessage(SCI_GETTEXTLENGTH, 0, 0);;
			//       move start of target behind last change and end of target to new end of buffer
			SendMessage(SCI_SETTARGETSTART, lPos + lLen);
			SendMessage(SCI_SETTARGETEND, lEnd);
			//       find again - if nothing found loop exits
			lPos = SendMessage(SCI_SEARCHINTARGET, strlen(szFind), (long)szFind);
			nCount++;
		}
	}
	return nCount;
}


void CScintillaWnd::OnButtonDelall()
{
	SetText("");
	SendMessage(SCI_EMPTYUNDOBUFFER);
	ShowScrollBar(SB_BOTH, 0);
	//MHFileManager.Clear();
	//m_nFileNum = 0;
	//m_sFileNum.Format("%d", m_nFileNum);
	//m_FileList.ResetContent();

	//m_editNew.SetWindowText("");
	//m_editNew.ShowScrollBar(SB_BOTH, 0);

	UpdateData(FALSE);
}

//=============================================================================	
void CScintillaWnd::OnContextMenu(CWnd* /*pWnd*/, CPoint point)
//=============================================================================	
{
	AFX_MANAGE_STATE(AFXMANAGESTATE())

	if (m_nContextMenuId == -1)
	{
		TRACE(_T("no context menu\n"));
		return;
	}

	CMenu menu;
	if (!menu.LoadMenu(m_nContextMenuId))
	{
		TRACE(_T("ERROR failed to load %d\n"), m_nContextMenuId);
		return;
	}

	menu.GetSubMenu(0)->TrackPopupMenu(0,
		point.x, point.y, this, NULL);
}



void CScintillaWnd::ResetRedo()
{
	SendMessage(SCI_EMPTYUNDOBUFFER, 0, 0);
	SendMessage(SCI_SETUNDOCOLLECTION, 1, 0);
}
BOOL CScintillaWnd::OpenFile(LPCTSTR lpFileName)
{
	DWORD dwSize = 0;
	const LPBYTE pbuf = static_cast<LPBYTE>(map_file(lpFileName, &dwSize));
	if (!pbuf) return FALSE;
	CString str;
	if (dwSize > 2 && pbuf[0] == 0xFF && pbuf[1] == 0xFE)
	{//utf16
		CString strBuf((LPCWSTR)(pbuf + 2), dwSize / 2 - 1);
		str = CA2W(strBuf, CP_UTF8);//CW2A
	}
	else if (dwSize > 3 && pbuf[0] == 0xEF && pbuf[1] == 0xBB && pbuf[2] == 0xBF)
	{//utf8有签名
		str = CString((LPCSTR)(pbuf + 3), dwSize - 3);
	}
	else
	{//utf8无签名
		str = CString((LPCSTR)pbuf, dwSize);
	}
	UnmapViewOfFile(pbuf);
	SendMessage(SCI_CLEARALL);
	SendMessage(SCI_MARKERDELETEALL, (WPARAM)-1, 0);

	SendMessage(SCI_ADDTEXT, str.GetLength(),
		reinterpret_cast<LPARAM>((LPCSTR)str));

	SendMessage(SCI_SETSAVEPOINT, 0, 0);
	SendMessage(SCI_GOTOPOS, 0, 0);
	SendMessage(SCI_CHOOSECARETX, 0, 0);
	ResetRedo();
	::SetFocus(m_hWnd);
	UpdateLineNumberWidth();

	m_strFileName = lpFileName;
	SetDirty(false);

	return TRUE;
}
void CScintillaWnd::DoSave()
{
	if (m_strFileName.IsEmpty())
	{
		return;
	}

	if (SaveFile(m_strFileName))
		SetDirty(false);
}
void CScintillaWnd::SetDirty(bool bDirty)
{
	m_bDirty = bDirty;
	if (!::IsWindow(m_hWnd))
		return;

	if (bDirty)
	{
		SetXmlLexer(RGB(255, 235, 240));
	}
	else
	{
		SetXmlLexer(RGB(255, 255, 255));
	}

	//if (m_fnCallback && !m_strFileName.IsEmpty())
	//{
	//	(this->m_fnCallback)(this, 1, bDirty ? _T("dirty") : _T(""));
	//}
}
intptr_t const CScintillaWnd::getCurrentColumnNumber()
{
	return SendMessage(SCI_GETCOLUMN, SendMessage(SCI_GETCURRENTPOS));
}
void CScintillaWnd::OnLButtonDown(UINT nFlags, CPoint point)
{
	//SetFocus();
	if (((theDlg*)m_pParent) -> m_QuickFind)
		((theDlg*)m_pParent)->m_QuickFind->Invalidate();
	//TT
	CWnd::OnLButtonDown(nFlags, point);
}

void CScintillaWnd::OnRButtonDown(UINT nFlags, CPoint point)
{
	//SetFocus();
	if (((theDlg*)m_pParent)->m_QuickFind)
		((theDlg*)m_pParent)->m_QuickFind->Invalidate();
	//TT
	CWnd::OnRButtonDown(nFlags, point);
}