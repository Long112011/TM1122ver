/* @doc
* @module ScintillaWnd.h | Definition of a Scintilla syntax coloring edit control for MFC<nl>
* This program is an example how to use the excellent scintilla edit control of Neil Hodgson.<nl>
* See www.scintilla.org for details<nl>
* Author: Horst Brückner, hb@ec-logic.com<nl>
* Environment: VisualC++ Version 6, static Build of Scintilla, SciLexer.dll as Lexer<nl>
*/

#ifndef __SCINTILLAWND_H__
#define __SCINTILLAWND_H__

#include "scintilla\include\scintilla.h"
#include "scintilla\include\scilexer.h"

//#include "QuickFindView.inl"
class CScintillaModule
{
public:
	CScintillaModule();
	~CScintillaModule();

	BOOL operator !() const
	{
		return m_hModule == nullptr;
	}
protected:
	HINSTANCE m_hModule;
};
class CScintillaWnd;
typedef void(*SCIWND_FN_CALLBACK)(CScintillaWnd*, int, CString);
////////////////////////////////////
// @class CScintillaWnd | Class of a GCL Scintilla syntax coloring edit control for MFC
// @base public | CWnd
//
class CScintillaWnd : public CWnd
{
	

	
public:
	// @access public constructor - destructor
	// @cmember empty Constructor
	CScintillaWnd();
	// @cmember destructor
	virtual ~CScintillaWnd();

public:

	int getCurrentDocLen() const {
		return int(SendMessage(SCI_GETLENGTH));
	};

	virtual void UpdateWindowPos();
	// @access public macro members
	// @cmember return linenumber display flag
	BOOL GetDisplayLinenumbers(){ return m_bLinenumbers; };
	// @cmember return selection display flag
	BOOL GetDisplaySelection(){ return m_bSelection; };
	// @cmember return folding margin display flag
	BOOL GetDisplayFolding(){ return m_bFolding; };
	// @cmember set search flags
	virtual void SetSearchflags(int nSearchflags){ m_nSearchflags = nSearchflags; };

	bool m_bDirty;

	bool m_bTemp;
public:

	CWnd* m_pParent;

	void insertGenericTextFrom(int position, const TCHAR *text2insert) const;
	// @access public function members
	// @cmember register a window class to use scintilla in a dialog
	static BOOL Register(CWinApp *app, UINT id);
	// @cmember try to load Lexer DLL
	static HMODULE LoadScintillaDll(LPCSTR szLexerDll = NULL);

 
	// @cmember create window NEW
	static HWND CreateEditor(LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, HWND pParentWnd, UINT nID);

	static BOOL CreateEditorNormal(LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, HWND pParentWnd, UINT nID);

	// @cmember create window
	virtual BOOL Create(LPCTSTR wclass, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID);
	// @cmember Set Text to the Scintilla control
	virtual void SetText(LPCSTR szText);
	// @cmember Get Text from the Scintilla control
	virtual void GetText(CString &strText);
	// @cmember Get Text from the Scintilla control
	virtual LPSTR GetText();
	void getText(char *dest, int start, int end) const;
	// @cmember Load a file
	virtual BOOL LoadFile(LPCSTR szFile);
	// @cmember Save a file
	virtual BOOL SaveFile(LPCSTR szFile);
	// @cmember Save a file
	virtual BOOL SendSaveFile(LPCSTR szFile);
	// @cmember try to find lexer format from extension
	virtual int GetFormatFromExtension(LPCSTR szExtension);
	// @cmember calcluate number of chars needed for linenumberdisplay
	//virtual int GetLinenumberChars();
	// @cmember calcluate number of pixels for linenumber display
	//virtual int GetLinenumberWidth();
	// @cmember set display of linenumbers on/off
	virtual void SetDisplayLinenumbers(BOOL bFlag = TRUE);
	// @cmember set display of selection/bookmark margin on/off
	virtual void SetDisplaySelection(BOOL bFlag = TRUE);
	// @cmember set display of source folding margin on/off
	virtual void SetDisplayFolding(BOOL bFlag = TRUE);
	// @cmember cut selection to clipboard
	virtual void Cut();
	// @cmember copy selection to clipboard
	virtual void Copy();
	// @cmember paste from clipboard
	virtual void Paste();
	// @cmember clear selection
	virtual void Clear();
	// @cmember undo last change
	virtual void Undo();
	// @cmember redo last change
	virtual void Redo();
	// @cmember check if we can undo
	virtual BOOL CanUndo();
	// @cmember check if we can redo
	virtual BOOL CanRedo();
	// @cmember check if we have something to paste from clipboard
	virtual BOOL CanPaste();
	// @cmember select complete text
	virtual void SelectAll();
	// @cmember return the current line number
	virtual long GetCurrentLine();
	// @cmember return the current column number
	virtual long GetCurrentColumn();
	// @cmember return the current character position within the file
	virtual long GetCurrentPosition();
	// @cmember return the current style number at the current character position
	virtual int GetCurrentStyle();
	// @cmember return the current folding level at the current character position
	virtual int GetCurrentFoldinglevel();
	// @cmember set the fontname for a style number
	virtual void SetFontname(int nStyle, LPCSTR szFontname);
	// @cmember set the fontname height in points for a style number
	virtual void SetFontheight(int nStyle, int nHeight);
	virtual long GetFontheight();
	// @cmember set the foregroundcolor for a style number
	virtual void SetForeground(int nStyle, COLORREF crForeground);
	// @cmember set the backgroundcolor for a style number
	virtual void SetBackground(int nStyle, COLORREF crBackground);
	// @cmember set given style to bold
	virtual void SetBold(int nStyle, BOOL bFlag);
	// @cmember set given style to bold
	virtual void SetItalic(int nStyle, BOOL bFlag);
	// @cmember set given style to bold
	virtual void SetUnderline(int nStyle, BOOL bFlag);
	// @cmember get flag if we are in overstrike mode
	virtual BOOL GetOverstrike();
	// @cmember set flag for overstrike mode
	virtual void SetOverstrike(BOOL bOverstrike);
	// @cmember init the view with reasonable defaults
	virtual void Init();
	// @cmember called whenever the text is changed - we can update any indicators and do brace matching
	virtual void UpdateUI();
	virtual void findMatchingBracePos(int & braceAtCaret, int & braceOpposite);
	virtual bool doMatch();
	virtual void UpdateLineNumberWidth(void);
	//virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);
	// @cmember do default folding 
	virtual void DoDefaultFolding(int nMargin, long lPosition);
	// @cmember refresh lexer and display after new settings
	virtual void Refresh();
	// @cmember add a bookmark = marker type 0
	virtual void AddBookmark(long lLine);
	// @cmember delete a bookmark = marker type 0
	virtual void DeleteBookmark(long lLine);
	virtual void DeleteAllBookmark();
	// @mCustom modified document marker
	virtual void MarkLine(long lLine, long pLine=0);
	// @cmember check for bookmark = marker type 0
	virtual BOOL HasBookmark(long lLine, long pLine=0);
	// @cmember Find next bookmark
	virtual void FindNextBookmark();
	// @cmember Find previous bookmark
	virtual void FindPreviousBookmark();
	// @cmember goto line
	virtual void GotoLine(long lLine);
	// @cmember goto position
	virtual void GotoPosition(long lPos);
	// @cset code page
	virtual void SetCodePage(long lPos);
	virtual void SetCharacterSet(long style,long charset);
	virtual void SetFontLocale(LPSTR lpFileName);

	virtual void SetSel(int nStartChar, int nEndChar, BOOL bNoScroll=0);
	virtual void SetSel(DWORD dwSelection, BOOL bNoScroll = 0);
	// @cmember search forward for a given text
	virtual BOOL SearchForward(LPSTR szText);
	virtual BOOL SearchForwardReplace(LPSTR szText);
	virtual void SearchTest();
	// @cmember search backward for a given text
	virtual BOOL SearchBackward(LPSTR szText);
	// @cmember replace a text found by SearchForward or SearchBackward
	virtual void commentBlockToolStripMenuItem_Click();
	virtual int TestComment();
	virtual void ReplaceSearchedText(LPCSTR szText);
	// @cmember Set your own lexer
	virtual void SetLexer(int nLexer);
	// @cmember return start position of selection
	virtual long GetSelectionStart();
	// @cmember return end position of selection
	virtual long GetSelectionEnd();
	// @cmember get selected text
	virtual void clearOccurrences();
	virtual CString GetSelectedText();

	// @cmember replace all in buffer or selection
	virtual int ReplaceAll(LPCSTR szFind, LPCSTR szReplace, BOOL bSelection = TRUE);

	virtual void SetDefaultColorFont(int nSize, const TCHAR* face, int style = STYLE_DEFAULT,int bSelect=0);
	virtual void SetBraceText();
	virtual void SetMarker();
	virtual void SetProperty();
	virtual void SetXmlLexer(BOOL useGlobal = false);
	virtual void InitEdit();
	virtual void OnButtonDelall();

	virtual  BOOL OpenFile(LPCTSTR lpFileName);
	virtual void DoSave();
	virtual void SetDirty(bool bDirty);
	virtual intptr_t const getCurrentColumnNumber();
	virtual void ResetRedo();

	virtual void SetTemp(BOOL bFlag){ m_bTemp = bFlag; }
	virtual BOOL GetTemp(){ return m_bTemp; }

	void SetContextMenuId(UINT nId) { m_nContextMenuId = nId; }

	virtual afx_msg void OnChange();

	void SetSaveCallback(SCIWND_FN_CALLBACK fun)
	{
		m_fnCallback = fun;
	}
protected:
	SCIWND_FN_CALLBACK m_fnCallback;
	LRESULT OnNotify(int idCtrl, LPNMHDR pnmh);

	/*BEGIN_MSG_MAP(CScintillaWnd)
		MSG_OCM_NOTIFY(OnNotify)
		MSG_WM_KEYDOWN(OnKeyDown)
		END_MSG_MAP()*/
	void ShowAutoComplete(const char ch);
	afx_msg LRESULT OnNcHitTest(CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	UINT m_nContextMenuId;
	
	afx_msg LONG OnPaste(UINT wParam, LONG lParam);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	CString m_strFileName;
	//SCIWND_FN_CALLBACK m_fnCallback;
	// @access protected data members
	// @cmember flag if we should display line numbers
	BOOL m_bLinenumbers;
	// @cmember flag if we should display selection and bookmark margin
	BOOL m_bSelection;
	// @cmember flag if we should display folding margin
	BOOL m_bFolding;
	// @cmember search flags
	int  m_nSearchflags;
	DECLARE_MESSAGE_MAP()
private:
	bool findCharInRange(unsigned char beginRange, unsigned char endRange, int startPos, bool direction, bool wrap);
	bool getRangeFromUI(unsigned char & startRange, unsigned char & endRange);
	void getDirectionFromUI(bool & whichDirection, bool & isWrap);

	void SetAStyle(int style, COLORREF fore, COLORREF back = RGB(0, 0, 0), int size = -1, const char *face = 0)
	{
		SendMessage(SCI_STYLESETFORE, style, fore);
		SendMessage(SCI_STYLESETBACK, style, back);
		if (size >= 1)
			SendMessage(SCI_STYLESETSIZE, style, size);
		if (face)
			SendMessage(SCI_STYLESETFONT, style, reinterpret_cast<LPARAM>(face));
	}

};
#define STR_SCINTILLAWND _T("Scintilla")
#define STR_LEXERDLL     _T("scilexer.dll")

#endif // !__SCINTILLAWND_H__
//extern CScintillaWnd theScintillaEdit;