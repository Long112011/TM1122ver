
#pragma once
#include "OXDllExt.h"
#include "../LineNumberStatic.h"
//#include "../ScintillaWnd.h"
#ifndef __AFXTEMPL_H__
#include <afxtempl.h>
#define __AFXTEMPL_H__
#endif
#define ID_OXEDIT_TOOLTIP		0x00000001
const TCHAR OXEDITABLETEXT_SYMBOL = _T('#');
const TCHAR OXCURRENCY_SYMBOL = _T('$');
const CString OXNUMERICVALUE_PLACEHOLDER(_T("1.1"));
const int OXNUMERICVALUE_PLACEHOLDER_LENGTH = OXNUMERICVALUE_PLACEHOLDER.GetLength();
class OX_CLASS_DECL COXEdit : public CEdit
{
	//CScintillaWnd       m_ScinCtrl;
	DECLARE_DYNCREATE(COXEdit)
public:

	void SetContextMenuId(UINT nId) { m_nContextMenuId = nId; }

	inline COLORREF GetReadOnlyFontColor()
	{
		return m_clrReadOnlyFont;
	}
	inline void SetReadOnlyFontColor(COLORREF clrReadOnlyFont)
	{
		m_clrReadOnlyFont = clrReadOnlyFont;
	}
	inline COLORREF GetReadOnlyBkColor()
	{
		return m_clrReadOnlyBack;
	}
	inline void SetReadOnlyBkColor(COLORREF clrReadOnlyBk)
	{
		m_clrReadOnlyBack = clrReadOnlyBk;
	}
	CWnd* m_pParent;
	COXEdit(CWnd* pParent = NULL);
	BOOL Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID);
	void SetTextColor(COLORREF clrText, BOOL bRedraw = TRUE);
	inline COLORREF GetTextColor() const { return m_clrText; }
	void SetBkColor(COLORREF clrBack, BOOL bRedraw = TRUE);
	inline COLORREF GetBkColor() const { return m_clrBack; }
	BOOL SetToolTipText(LPCTSTR sText);
	inline CString GetToolTipText() const { return m_sToolTipText; }
	inline CToolTipCtrl* GetToolTip() { return &m_ttc; }
	BOOL SetMask(LPCTSTR lpszMask);
	inline CString GetMask() const { return m_sMask; }
	CString GetFormattedText() const;
	virtual CString GetInputData() const;
	virtual BOOL SetInputData(LPCTSTR pszInputData, int nBeginPos = 0);
	virtual void EmptyData(BOOL bOnlyInput = FALSE);
	void SetInsertMode(BOOL bInsertMode);
	BOOL GetInsertMode() const;
	int RPtoLP(int nRealPos) const;
	int LPtoRP(int nLogicalPos) const;
	virtual void ValidationError();
	inline void SetHighlightOnSetFocus(BOOL bHighlightOnSetFocus)
	{
		m_bHighlightOnSetFocus = bHighlightOnSetFocus;
	}
	inline BOOL GetHighlightOnSetFocus() const { return m_bHighlightOnSetFocus; }
	virtual BOOL PreTranslateMessage(MSG* pMsg);
//------------------line number started
	void SetMarginForegroundColor(COLORREF col, BOOL redraw = TRUE, BOOL bEnabled = TRUE);
	void SetMarginBackgroundColor(COLORREF col, BOOL redraw = TRUE, BOOL bEnabled = TRUE);
	void SetLineNumberFormat(CString format);
	void SetLineNumberRange(UINT nMin, UINT nMax = 0);
	void UseSystemColours(BOOL bUseEnabled = TRUE, BOOL bUseDisabled = TRUE);
	//void SetLineNumberEnable(BOOL bflag){ m_enablelinenum = bflag; UseSystemColours(); }
	virtual afx_msg void OnSysColorChange();
//------------------line number ended

	void Update(int nSelectionStart = 0);
protected:
//------------------line number started
	UINT m_nContextMenuId;
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	//virtual void PreSubclassWindow();
	//virtual afx_msg void OnSize(UINT nType, int cx, int cy);
	//virtual afx_msg LRESULT OnSetText(WPARAM wParam, LPARAM lParam);
	virtual afx_msg void OnEnable(BOOL bEnable);
	virtual afx_msg void OnChange();
	virtual afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	virtual afx_msg void OnVscroll();
	
	virtual afx_msg LRESULT OnSetFont(WPARAM wParam, LPARAM lParam);
	
	virtual afx_msg LRESULT OnLineScroll(WPARAM wParam, LPARAM lParam);
	virtual afx_msg LRESULT OnSelectLine(WPARAM wParam, LPARAM lParam);
//------------------line number ended
	virtual BOOL GetLocaleSettings() { return TRUE; }
	virtual BOOL InitControl();
	virtual int ValidateInput() { return 0; }
	virtual void DeleteContents();
	virtual BOOL IsInputData(int nPosition) const;
	virtual BOOL IsInputPosition(int nPosition) const;
	CString GetLeftLiterals() const;
	CString GetRightLiterals() const;
	virtual int DeleteRange(int& nSelectionStart, int& nSelectionEnd);
	virtual int InsertAt(int nSelectionStart, TCHAR chNewChar);
	virtual int SetAt(int nSelectionStart, TCHAR chNewChar);
	virtual int GetNextInputLocation(int nSelectionStart);
	virtual int GetPreviousInputLocation(int nSelectionStart);
	virtual void UpdateMask() {};
	
	void UpdateInsertionPointForward(int nSelectionStart);
	void UpdateInsertionPointBackward(int nSelectionStart);
	BOOL NotifyParent(UINT nNotificationID);


	virtual void PreSubclassWindow();
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	COLORREF m_clrText;
	COLORREF m_clrBack;
	CFont m_font;
	CString m_sMask;
	CArray<TCHAR, TCHAR> m_arrLeftLiterals;
	CArray<TCHAR, TCHAR> m_arrRightLiterals;
	CArray<TCHAR, TCHAR> m_arrInputData;
	BOOL m_bInsertMode;
	CString m_sToolTipText;
	CToolTipCtrl m_ttc;
	int m_nSetTextSemaphor;
	int m_bNotifyParent;
	BOOL m_bHighlightOnSetFocus;
	BOOL m_bNoSaveFont;

protected:
	int Validate();
	COLORREF m_clrReadOnlyBack;
	COLORREF m_clrReadOnlyFont;
	BOOL SaveFont();
	inline BOOL IsInitialized() const { return m_bInitialized; }
	afx_msg HBRUSH CtlColor(CDC* pDC, UINT nCtlColor);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnDestroy();
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg LONG OnCut(UINT wParam, LONG lParam);
	afx_msg LONG OnCopy(UINT wParam, LONG lParam);
	afx_msg LONG OnPaste(UINT wParam, LONG lParam);
	afx_msg LONG OnClear(UINT wParam, LONG lParam);
	afx_msg LRESULT OnSetText(UINT wParam, LONG lParam);
	DECLARE_MESSAGE_MAP()
private:
	void Prepare(BOOL bEnable);
	int CalcLineNumberWidth();
	void UpdateTopAndBottom();
	void SetWindowColour(BOOL bEnable = TRUE);
	BOOL				m_enablelinenum;
	BOOL				m_bUseEnabledSystemColours;
	COLORREF			m_EnabledFgCol;
	COLORREF			m_EnabledBgCol;
	BOOL				m_bUseDisabledSystemColours;
	COLORREF			m_DisabledFgCol;
	COLORREF			m_DisabledBgCol;
	CLineNumberStatic	m_line;
	CSize				m_zero;
	int					m_maxval;
	CString				m_format;
	int                 m_LineDelta;
private:
	BOOL m_bInitialized;
};
class OX_CLASS_DECL COXNumericEdit : public COXEdit
{
public:
	COXNumericEdit();
	virtual CString GetInputData() const;
	BOOL SetGroupSeparator(TCHAR chGroupSeparator);
	inline TCHAR GetGroupSeparator() const { return m_chGroupSeparator; }
	BOOL SetGroupLength(int nGroupLength);
	inline int GetGroupLength() const { return m_nGroupLength; }
	BOOL SetDecimalSeparator(TCHAR chDecimalSeparator);
	inline TCHAR GetDecimalSeparator() const { return m_chDecimalSeparator; }
	void SetShowLeadingZero(BOOL bShowLeadingZero);
	inline BOOL GetShowLeadingZero() const { return m_bShowLeadingZero; }
	BOOL SetFractionalDigitCount(int nFractionalDigitCount);
	inline int GetFractionalDigitCount() const { return m_nFractionalDigitCount; }
	BOOL SetDecimalDigitCount(int nDecimalDigitCount);
	inline int GetDecimalDigitCount() const { return m_nDecimalDigitCount; }
	virtual BOOL SetPositiveFormat(LPCTSTR lpszFormatPositive);
	inline CString GetPositiveFormat() const { return m_sFormatPositive; }
	virtual BOOL SetNegativeFormat(LPCTSTR lpszFormatNegative);
	inline CString GetNegativeFormat() const { return m_sFormatNegative; }
	void SetNegativeTextColor(COLORREF clrTextNegative, BOOL bRedraw = TRUE);
	inline COLORREF GetNegativeTextColor() const { return m_clrTextNegative; }
	double GetValue() const { return m_dValue; }
	BOOL SetValue(double dValue);
	inline BOOL IsNegativeValue() const { return m_bNegativeValue; }
	inline void SetForceToUsePeriod(BOOL bForceToUsePeriod) { m_bForceToUsePeriod = bForceToUsePeriod; };
	inline BOOL GetForceToUsePeriod() const { return m_bForceToUsePeriod; }
protected:
	virtual BOOL GetLocaleSettings();
	virtual BOOL InitControl();
	virtual int ValidateInput();
	virtual void DeleteContents();
	virtual BOOL IsInputData(int nPosition) const;
	virtual int ChangeSign(BOOL bNegative);
	virtual int DeleteRange(int& nSelectionStart, int& nSelectionEnd);
	virtual int InsertAt(int nSelectionStart, TCHAR chNewChar);
	virtual int SetAt(int nSelectionStart, TCHAR chNewChar);
	DECLARE_DYNCREATE(COXNumericEdit)
	TCHAR m_chGroupSeparator;
	int m_nGroupLength;
	TCHAR m_chDecimalSeparator;
	BOOL m_bShowLeadingZero;
	int m_nFractionalDigitCount;
	int m_nDecimalDigitCount;
	CString m_sFormatNegative;
	CString m_sFormatPositive;
	COLORREF m_clrTextNegative;
	double m_dValue;
	BOOL m_bNegativeValue;
	BOOL m_bForceToUsePeriod;
	TCHAR m_chGroupSeparatorDefault;
	int m_nGroupLengthDefault;
	TCHAR m_chDecimalSeparatorDefault;
	BOOL m_bShowLeadingZeroDefault;
	int m_nFractionalDigitCountDefault;
	int m_nDecimalDigitCountDefault;
	CString m_sFormatNegativeDefault;
	CString m_sFormatPositiveDefault;
protected:
	int GetDecimalDigitsInserted() const;
	int GetFractionalDigitsInserted() const;
	int GetGroupSeparatorSymbolsInserted(
		int nStartPos = 0,
		int nEndPos = -1) const;
	int FindSymbol(TCHAR chSymbol) const;
	virtual int TranslateSymbol(int nSelectionStart, TCHAR chNewChar,
		BOOL bReplaceExisting);
	afx_msg HBRUSH CtlColor(CDC* pDC, UINT nCtlColor);
	DECLARE_MESSAGE_MAP()
};
