#include "StdAfx.h"
#include "../main.h"
#include "OXEdit.h"
#include <math.h> 
#include <stdlib.h>
#pragma warning (push, 3)
#include <iomanip>
#include <sstream>
#pragma warning (pop)
using namespace std;
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
#if _MFC_VER<=0x0421
static void	RemoveCharFromString(CString& sText, TCHAR chToRemove)
{
	int nPos = sText.Find(chToRemove);
	while (nPos != -1)
	{
		sText = sText.Left(nPos) + sText.Mid(nPos + 1);
		nPos = sText.Find(chToRemove);
	}
}
static void	ReplaceCharInString(CString& sText, TCHAR chToBeReplaced, TCHAR chToReplaceTo)
{
	CString sCopy = sText;
	CString sToReplaceTo(chToReplaceTo);
	int nPos = sCopy.Find(chToBeReplaced);
	sText.Empty();
	while (nPos != -1)
	{
		sText = sText + sCopy.Left(nPos) + sToReplaceTo;
		sCopy = sCopy.Mid(nPos + 1);
		nPos = sCopy.Find(chToBeReplaced);
	}
	sText += sCopy;
}
#endif
extern UINT urm_SELECTLINE;
IMPLEMENT_DYNCREATE(COXEdit, CEdit)
BEGIN_MESSAGE_MAP(COXEdit, CEdit)
	ON_WM_CONTEXTMENU()
	ON_WM_CTLCOLOR_REFLECT()
	ON_WM_SIZE()
	ON_WM_DESTROY()
	ON_WM_KEYDOWN()
	ON_WM_CHAR()
	ON_WM_SETFOCUS()
	ON_WM_LBUTTONDOWN()
	ON_MESSAGE(WM_CUT, OnCut)
	ON_MESSAGE(WM_COPY, OnCopy)
	ON_MESSAGE(WM_PASTE, OnPaste)
	ON_MESSAGE(WM_CLEAR, OnClear)
	ON_MESSAGE(WM_SETTEXT, OnSetText)






	ON_CONTROL_REFLECT(EN_CHANGE, OnChange)
	ON_WM_VSCROLL()
	ON_CONTROL_REFLECT(EN_VSCROLL, OnVscroll)
	ON_MESSAGE(WM_SETFONT, OnSetFont)
	//ON_WM_SIZE()
	//ON_MESSAGE(WM_SETTEXT, OnSetText)
	ON_WM_SYSCOLORCHANGE()
	ON_WM_ENABLE()
	ON_MESSAGE(EM_LINESCROLL, OnLineScroll)
	ON_REGISTERED_MESSAGE(urm_SELECTLINE, OnSelectLine)
END_MESSAGE_MAP()
void COXEdit::OnContextMenu(CWnd* /*pWnd*/, CPoint point)
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
COXEdit::COXEdit(CWnd* pParent) //: CEdit(IDD_QUICK_FIND_REPLACE, pParent)
{
	m_pParent = pParent;
	m_clrText = ::GetSysColor(COLOR_WINDOWTEXT);
	m_clrBack = ::GetSysColor(COLOR_WINDOW);
	m_clrReadOnlyBack = ::GetSysColor(COLOR_BTNFACE);
	m_clrReadOnlyFont = ::GetSysColor(COLOR_GRAYTEXT);
	m_bInsertMode = TRUE;
	TCHAR chMask[2];
	chMask[0] = OXEDITABLETEXT_SYMBOL;
	chMask[1] = _T('\0');
	VERIFY(SetMask(chMask));
	m_bInitialized = FALSE;
	m_nSetTextSemaphor = 0;
	m_bNotifyParent = TRUE;
	m_bHighlightOnSetFocus = FALSE;
	m_bNoSaveFont = FALSE;


	m_enablelinenum = FALSE;

	m_nContextMenuId = (UINT)-1;

	m_hWnd = NULL;
	m_line.m_hWnd = NULL;
	m_zero.cx = 0;
	m_zero.cy = 0;
	m_format = _T("%3i");
	m_LineDelta = 1;
	m_maxval = 0;
	m_bUseEnabledSystemColours = FALSE;
	m_bUseDisabledSystemColours = TRUE;
	m_EnabledFgCol = RGB(255, 45, 48);
	m_EnabledBgCol = RGB(30, 30, 30);
	m_DisabledFgCol = GetSysColor(COLOR_GRAYTEXT);
	m_DisabledBgCol = GetSysColor(COLOR_3DFACE);
	SetWindowColour();
}
void COXEdit::SetTextColor(COLORREF clrText, BOOL bRedraw/*=TRUE*/)
{
	if (m_clrText != clrText)
	{
		m_clrText = clrText;
		//m_EnabledFgCol = clrText;
		if (bRedraw)
			RedrawWindow();
	}
}
void COXEdit::SetBkColor(COLORREF clrBack, BOOL bRedraw/*=TRUE*/)
{
	if (m_clrBack != clrBack)
	{
		m_clrBack = clrBack;
		//m_EnabledBgCol = clrBack;
		if (bRedraw)
			RedrawWindow();
	}
}
BOOL COXEdit::SetToolTipText(LPCTSTR sText)
{
	if (::IsWindow(m_ttc.GetSafeHwnd()))
	{
		m_sToolTipText = sText;
		m_ttc.UpdateTipText(m_sToolTipText, this, ID_OXEDIT_TOOLTIP);
		return TRUE;
	}
	return FALSE;
}
HBRUSH COXEdit::CtlColor(CDC* pDC, UINT nCtlColor)
{
	UNREFERENCED_PARAMETER(nCtlColor);
	COLORREF clrText = GetTextColor();
	COLORREF clrBk = GetBkColor();
	if (!IsWindowEnabled())
	{
		clrText = ::GetSysColor(COLOR_GRAYTEXT);
		clrBk = ::GetSysColor(COLOR_BTNFACE);
	}
	else if (GetStyle()&ES_READONLY)
	{
		clrText = m_clrReadOnlyFont;
		clrBk = m_clrReadOnlyBack;
	}
	pDC->SetTextColor(clrText);
	pDC->SetBkColor(clrBk);
	if ((HFONT)m_font != NULL)
	{
		pDC->SelectObject(&m_font);
	}
	static CBrush brush;
	if ((HBRUSH)brush != NULL)
		brush.DeleteObject();
	brush.CreateSolidBrush(clrBk);
	return (HBRUSH)brush;
}
BOOL COXEdit::PreTranslateMessage(MSG* pMsg)
{
	if (::IsWindow(m_ttc.GetSafeHwnd()))
	{
		m_ttc.Activate(TRUE);
		m_ttc.RelayEvent(pMsg);
	}
	return CEdit::PreTranslateMessage(pMsg);
}
void COXEdit::PreSubclassWindow()
{
	_AFX_THREAD_STATE* pThreadState = AfxGetThreadState();
	if (pThreadState->m_pWndInit == NULL && !m_bInitialized)
	{
		if (!InitControl())
			TRACE(_T("COXEdit::PreSubclassWindow: failed to initialize edit control\n"));
	}
	SetLineNumberFormat(m_format);
	CEdit::PreSubclassWindow();
}
void COXEdit::OnSize(UINT nType, int cx, int cy)
{
	CEdit::OnSize(nType, cx, cy);
	if (m_line.m_hWnd)
		Prepare(m_enablelinenum);

	CRect rect;
	GetWindowRect(rect);
	CWnd* pParent = GetParent();
	if (pParent != NULL)
	{
		pParent->ScreenToClient(rect);
	}
	/*if(::IsWindow(m_ttc.GetSafeHwnd()))
	{
	CToolInfo toolInfo;
	if(m_ttc.GetToolInfo(toolInfo,this,ID_OXEDIT_TOOLTIP))
	{
	toolInfo.rect.left=0;
	toolInfo.rect.top=0;
	toolInfo.rect.right=rect.Width();
	toolInfo.rect.bottom=rect.Height();
	UTBStr::tcscpy(toolInfo.szText, _tcsclen(m_sToolTipText) + 1, m_sToolTipText);
	m_ttc.SetToolInfo(&toolInfo);
	}
	}*/
}
BOOL COXEdit::InitControl()
{
	ASSERT(::IsWindow(GetSafeHwnd()));
	GetLocaleSettings();
	CRect rect;
	GetClientRect(rect);
	m_ttc.Create(this);
	m_ttc.AddTool(this, m_sToolTipText, rect, ID_OXEDIT_TOOLTIP);
	VERIFY(SaveFont());
	CString sText;
	GetWindowText(sText);
	EmptyData();
	SetInputData(sText);
	m_bInitialized = TRUE;
	return TRUE;
}
BOOL COXEdit::Create(DWORD dwStyle, const RECT& rect,
	CWnd* pParentWnd, UINT nID)
{
	BOOL bResult = CEdit::Create(dwStyle, rect, pParentWnd, nID);
	if (bResult)
	{
		if (!InitControl())
		{
			TRACE(_T("COXEdit::Create: failed to initialize edit control"));
			return FALSE;
		}
	}
	return bResult;
}
void COXEdit::OnDestroy()
{
	if (::IsWindow(m_ttc.GetSafeHwnd()))
	{
		m_ttc.DelTool(this, ID_OXEDIT_TOOLTIP);
		m_ttc.DestroyWindow();
	}
	CEdit::OnDestroy();
}
BOOL COXEdit::SaveFont()
{
	LOGFONT lf;
	CFont* pFont = GetFont();
	if (pFont != NULL)
	{
		if (!pFont->GetLogFont(&lf))
			return FALSE;
	}
	if ((HFONT)m_font != NULL)
		m_font.DeleteObject();
	if (pFont != NULL)
	{
		if (!m_font.CreateFontIndirect(&lf))
			return FALSE;
	}
	return TRUE;
}
LRESULT COXEdit::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
#if defined (_WINDLL)
#if defined (_AFXDLL)
	AFX_MANAGE_STATE(AfxGetAppModuleState());
#else
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
#endif
#endif
	LRESULT lResult = CEdit::WindowProc(message, wParam, lParam);
	switch (message)
	{
	case WM_SETFONT:
	{
					   if (!m_bNoSaveFont)
					   {
						   VERIFY(SaveFont());
					   }
					   break;
	}
	case WM_SETTINGCHANGE:
	{
							 CString sInputData = GetInputData();
							 GetLocaleSettings();
							 EmptyData(TRUE);
							 VERIFY(SetInputData(sInputData));
							 break;
	}
	}
	return lResult;
}
void COXEdit::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if ((GetStyle()&ES_READONLY) == ES_READONLY)
	{
		CEdit::OnKeyDown(nChar, nRepCnt, nFlags);
		return;
	}
	BOOL bIsShiftKeyDown = ::GetAsyncKeyState(VK_SHIFT)<0;
	BOOL bIsCtrlKeyDown = ::GetAsyncKeyState(VK_CONTROL)<0;
	if (nChar == VK_DELETE)
	{
		if (bIsShiftKeyDown)
		{
			CEdit::OnKeyDown(nChar, nRepCnt, nFlags);
			return;
		}
		int nSelectionStart = 0;
		int nSelectionEnd = 0;
		GetSel(nSelectionStart, nSelectionEnd);
		if (nSelectionStart == nSelectionEnd)
		{
			nSelectionEnd++;
			if (DeleteRange(nSelectionStart, nSelectionEnd))
			{
				Update(nSelectionStart);
			}
			else
			{
				while (nSelectionEnd <=
					m_arrLeftLiterals.GetSize() + m_arrInputData.GetSize())
				{
					nSelectionStart++;
					nSelectionEnd++;
					if (DeleteRange(nSelectionStart, nSelectionEnd))
					{
						Update(nSelectionStart);
						break;
					}
				}
			}
		}
		else if (DeleteRange(nSelectionStart, nSelectionEnd))
		{
			Update(nSelectionStart);
		}
		else
		{
			while (nSelectionEnd <=
				m_arrLeftLiterals.GetSize() + m_arrInputData.GetSize())
			{
				nSelectionStart++;
				nSelectionEnd++;
				if (DeleteRange(nSelectionStart, nSelectionEnd))
				{
					Update(nSelectionStart);
					break;
				}
			}
		}
	}
	else if (nChar == VK_HOME)
	{
		CEdit::OnKeyDown(nChar, nRepCnt, nFlags);
		if (!bIsShiftKeyDown)
		{
			int nSelectionStart = 0;
			int nSelectionEnd = 0;
			GetSel(nSelectionStart, nSelectionEnd);
			if (!IsInputPosition(nSelectionStart))
				UpdateInsertionPointForward(nSelectionStart);
		}
	}
	else if (nChar == VK_END)
	{
		CEdit::OnKeyDown(nChar, nRepCnt, nFlags);
		if (!bIsShiftKeyDown)
		{
			int nSelectionStart = 0;
			int nSelectionEnd = 0;
			GetSel(nSelectionStart, nSelectionEnd);
			if (!IsInputPosition(nSelectionStart))
				UpdateInsertionPointBackward(nSelectionStart);
		}
	}
	else if (nChar == VK_LEFT)
	{
		CEdit::OnKeyDown(nChar, nRepCnt, nFlags);
		if (!bIsShiftKeyDown)
		{
			int nSelectionStart = 0;
			int nSelectionEnd = 0;
			GetSel(nSelectionStart, nSelectionEnd);
			UpdateInsertionPointBackward(nSelectionStart);
		}
	}
	else if (nChar == VK_UP)
	{
		CEdit::OnKeyDown(nChar, nRepCnt, nFlags);
		if (!bIsShiftKeyDown)
		{
			int nSelectionStart = 0;
			int nSelectionEnd = 0;
			GetSel(nSelectionStart, nSelectionEnd);
			UpdateInsertionPointBackward(nSelectionStart);
		}
	}
	else if (nChar == VK_RIGHT)
	{
		CEdit::OnKeyDown(nChar, nRepCnt, nFlags);
		if (!bIsShiftKeyDown)
		{
			int nSelectionStart = 0;
			int nSelectionEnd = 0;
			GetSel(nSelectionStart, nSelectionEnd);
			UpdateInsertionPointForward(nSelectionStart);
		}
	}
	else if (nChar == VK_DOWN)
	{
		CEdit::OnKeyDown(nChar, nRepCnt, nFlags);
		if (!bIsShiftKeyDown)
		{
			int nSelectionStart = 0;
			int nSelectionEnd = 0;
			GetSel(nSelectionStart, nSelectionEnd);
			UpdateInsertionPointForward(nSelectionStart);
		}
	}
	else if (nChar == VK_INSERT)
	{
		if (!bIsShiftKeyDown && !bIsCtrlKeyDown)
		{
			SetInsertMode(!GetInsertMode());
		}
		else
			CEdit::OnKeyDown(nChar, nRepCnt, nFlags);
	}
	else
	{
		CEdit::OnKeyDown(nChar, nRepCnt, nFlags);
	}
}
void COXEdit::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if ((GetStyle()&ES_READONLY) == ES_READONLY)
	{
		CEdit::OnChar(nChar, nRepCnt, nFlags);
		return;
	}
	int nSelectionStart = 0;
	int nSelectionEnd = 0;
	GetSel(nSelectionStart, nSelectionEnd);
	BOOL bAcceptReturnKey = (GetStyle()&ES_MULTILINE) && (GetStyle()&ES_WANTRETURN);
	if (nChar == 10)
	{
		nChar = VK_RETURN;
		bAcceptReturnKey = (GetStyle()&ES_MULTILINE);
	}
	if (nChar >= 32 || (nChar == VK_RETURN && bAcceptReturnKey))
	{
		if (nSelectionStart == nSelectionEnd)
		{
			if (GetStyle()&ES_NUMBER && nChar != VK_RETURN &&
				(nChar<_T('0') || nChar>_T('9')))
			{
				CEdit::OnChar(nChar, nRepCnt, nFlags);
				return;
			}
			if (IsInputPosition(nSelectionStart))
			{
				int nActualInsertionPoint = nSelectionStart;
				if (m_bInsertMode)
				{
					if (nChar == VK_RETURN)
					{
						nActualInsertionPoint = InsertAt(nSelectionStart, _T('\r'));
						if (nActualInsertionPoint >= 0)
						{
							nSelectionStart = nActualInsertionPoint + 1;
							nActualInsertionPoint = InsertAt(nSelectionStart, _T('\n'));
						}
					}
					else
					{
						nActualInsertionPoint = InsertAt(nSelectionStart, (TCHAR)nChar);
					}
				}
				else
				{
					if (nChar == VK_RETURN)
					{
						nActualInsertionPoint = SetAt(nSelectionStart, _T('\r'));
						if (nActualInsertionPoint >= 0)
						{
							nSelectionStart = nActualInsertionPoint + 1;
							nActualInsertionPoint = SetAt(nSelectionStart, _T('\n'));
						}
					}
					else
					{
						nActualInsertionPoint = SetAt(nSelectionStart, (TCHAR)nChar);
					}
				}
				if (nActualInsertionPoint >= 0)
				{
					nSelectionStart = nActualInsertionPoint + 1;
				}
				else
				{
					ValidationError();
				}
				Update(nSelectionStart);
			}
			else
			{
				ValidationError();
				UpdateInsertionPointForward(nSelectionStart);
			}
		}
		else
		{
			if (DeleteRange(nSelectionStart, nSelectionEnd))
			{
				int nActualInsertionPoint = nSelectionStart;
				if (nChar == VK_RETURN)
				{
					nActualInsertionPoint = InsertAt(nSelectionStart, _T('\r'));
					if (nActualInsertionPoint >= 0)
					{
						nSelectionStart = nActualInsertionPoint + 1;
						nActualInsertionPoint = InsertAt(nSelectionStart, _T('\n'));
					}
				}
				else
				{
					nActualInsertionPoint = InsertAt(nSelectionStart, (TCHAR)nChar);
				}
				if (nActualInsertionPoint >= 0)
				{
					nSelectionStart = nActualInsertionPoint + 1;
				}
				else
				{
					ValidationError();
				}
				Update(nSelectionStart);
			}
			else
			{
				ValidationError();
				UpdateInsertionPointForward(nSelectionStart);
			}
		}
	}
	else
	{
		if (nChar == VK_BACK)
		{
			if (nSelectionStart == nSelectionEnd)
			{
				while (nSelectionStart > 0 && nSelectionStart == nSelectionEnd)
				{
					nSelectionStart--;
					if (DeleteRange(nSelectionStart, nSelectionEnd))
					{
						Update(nSelectionStart);
						break;
					}
					nSelectionEnd--;
				}
			}
			else if (DeleteRange(nSelectionStart, nSelectionEnd))
			{
				Update(nSelectionStart);
			}
			else
			{
				if (nSelectionStart >= 1)
				{
					while (nSelectionStart>0)
					{
						nSelectionStart--;
						if (DeleteRange(nSelectionStart, nSelectionEnd))
						{
							Update(nSelectionStart);
							break;
						}
						nSelectionEnd--;
					}
				}
			}
		}
		else
		{
			CEdit::OnChar(nChar, nRepCnt, nFlags);
		}
	}
}
void COXEdit::OnSetFocus(CWnd* pOldWnd)
{
	CEdit::OnSetFocus(pOldWnd);
	if (!GetHighlightOnSetFocus())
	{
		int nSelectionStart = 0;
		int nSelectionEnd = 0;
		GetSel(nSelectionStart, nSelectionEnd);
		if ((nSelectionStart == 0) && (nSelectionEnd == GetWindowTextLength()))
		{
			UpdateInsertionPointForward(0);
		}
	}
	m_bNoSaveFont = TRUE;
	SetFont(&m_font);
	m_bNoSaveFont = FALSE;
}
LONG COXEdit::OnCut(UINT wParam, LONG lParam)
{
	UNREFERENCED_PARAMETER(wParam);
	UNREFERENCED_PARAMETER(lParam);
	int nSelectionStart = 0;
	int nSelectionEnd = 0;
	GetSel(nSelectionStart, nSelectionEnd);
	CEdit::Default();
	int nDeleteCount = DeleteRange(nSelectionStart, nSelectionEnd);
	Update(nSelectionStart);
	if (nDeleteCount == 0)
	{
	}
	return 0;
}
LONG COXEdit::OnCopy(UINT wParam, LONG lParam)
{
	UNREFERENCED_PARAMETER(wParam);
	UNREFERENCED_PARAMETER(lParam);
	CEdit::Default();
	return 0;
}
LONG COXEdit::OnPaste(UINT wParam, LONG lParam)
{
	UNREFERENCED_PARAMETER(wParam);
	UNREFERENCED_PARAMETER(lParam);
	int nSelectionStart = 0;
	int nSelectionEnd = 0;
	GetSel(nSelectionStart, nSelectionEnd);
	CEdit::Default();
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
	//SetSel(nNewInsertionPoint, nNewInsertionPoint);
	SetSel(-1);
	//SetSel(0, -1);
	//PostMessage(0,-1);
	return 0;
}
LONG COXEdit::OnClear(UINT wParam, LONG lParam)
{
	UNREFERENCED_PARAMETER(wParam);
	UNREFERENCED_PARAMETER(lParam);
	TRACE(_T("COXEdit::OnClear\n"));
	int nSelectionStart = 0;
	int nSelectionEnd = 0;
	GetSel(nSelectionStart, nSelectionEnd);
	CEdit::Default();
	int nDeleteCount = DeleteRange(nSelectionStart, nSelectionEnd);
	UNREFERENCED_PARAMETER(nDeleteCount);
	Update(nSelectionStart);
	return 0;
}
void COXEdit::OnLButtonDown(UINT nFlags, CPoint point)
{
	CEdit::OnLButtonDown(nFlags, point);
	int nSelectionStart = 0;
	int nSelectionEnd = 0;
	GetSel(nSelectionStart, nSelectionEnd);
	if (nSelectionStart == nSelectionEnd && !IsInputPosition(nSelectionStart))
		UpdateInsertionPointForward(0);
}
LRESULT COXEdit::OnSetText(UINT wParam, LONG lParam)
{
	UNREFERENCED_PARAMETER(wParam);
	UNREFERENCED_PARAMETER(lParam);
	
	if (m_nSetTextSemaphor>0)
	{
		LRESULT result = DefWindowProc(WM_SETTEXT, wParam, lParam); //CEdit::Default();
		UpdateTopAndBottom();
		NotifyParent(EN_UPDATE);
		if (m_bNotifyParent)
			NotifyParent(EN_CHANGE);
		return result;
	}
	else
	{
		ASSERT(m_nSetTextSemaphor == 0);
		CString sText = (LPCTSTR)(INT_PTR)lParam;
		m_arrInputData.RemoveAll();
		m_bNotifyParent = FALSE;
		SetInputData(sText, 0);
		m_bNotifyParent = TRUE;
		return TRUE;
	}
}
BOOL COXEdit::SetMask(LPCTSTR lpszMask)
{
	CString sMask = lpszMask;
	if (sMask.IsEmpty() || sMask.Find(OXEDITABLETEXT_SYMBOL) == -1)
		return FALSE;
	DeleteContents();
	BOOL bLeftLiteral = TRUE;
	for (int nIndex = 0; nIndex<sMask.GetLength(); nIndex++)
	{
		BOOL bSaveLiteral = TRUE;
		TCHAR chSymbol = sMask[nIndex];
		TCHAR chSpecialSymbol = OXEDITABLETEXT_SYMBOL;
		if (chSymbol == chSpecialSymbol && (nIndex == sMask.GetLength() - 1 ||
			sMask[nIndex + 1] != chSpecialSymbol))
		{
			bSaveLiteral = FALSE;
			bLeftLiteral = FALSE;
		}
		else if (chSymbol == chSpecialSymbol && nIndex<sMask.GetLength() - 1 &&
			sMask[nIndex + 1] == chSpecialSymbol)
		{
			nIndex++;
		}
		if (bSaveLiteral)
		{
			if (bLeftLiteral)
				m_arrLeftLiterals.Add(chSymbol);
			else
				m_arrRightLiterals.Add(chSymbol);
		}
	}
	if (bLeftLiteral)
	{
		VERIFY(SetMask(GetMask()));
		return FALSE;
	}
	m_sMask = sMask;
	Validate();
	Update();
	return TRUE;
}
void COXEdit::DeleteContents()
{
	m_arrLeftLiterals.RemoveAll();
	m_arrRightLiterals.RemoveAll();
	m_arrInputData.RemoveAll();
}
CString COXEdit::GetInputData()const
{
	CString sInputData;
	for (int nIndex = 0; nIndex<m_arrInputData.GetSize(); nIndex++)
	{
		sInputData += m_arrInputData[nIndex];
	}
	return sInputData;
}
BOOL COXEdit::SetInputData(LPCTSTR pszInputData, int nBeginPos/*=0*/)
{
	if (nBeginPos<0 || nBeginPos>m_arrInputData.GetSize())
		return FALSE;
	CString sInputData = pszInputData;
	for (int nIndex = 0; nIndex<sInputData.GetLength(); nIndex++)
	{
		if (GetInsertMode())
			m_arrInputData.InsertAt(nBeginPos + nIndex, sInputData[nIndex]);
		else
			m_arrInputData.SetAtGrow(nBeginPos + nIndex, sInputData[nIndex]);
	}
	Validate();
	Update(-1);
	return TRUE;
}
void COXEdit::EmptyData(BOOL bOnlyInput/*=FALSE*/)
{
	if (bOnlyInput)
		m_arrInputData.RemoveAll();
	else
		DeleteContents();
	Update();
}
BOOL COXEdit::GetInsertMode() const
{
	return m_bInsertMode;
}
void COXEdit::SetInsertMode(BOOL bInsertMode)
{
	m_bInsertMode = bInsertMode;
}
CString COXEdit::GetFormattedText() const
{
	CString sFormattedText;
	int nIndex = 0;
	for (nIndex = 0; nIndex<m_arrLeftLiterals.GetSize(); nIndex++)
		sFormattedText += m_arrLeftLiterals[nIndex];
	for (nIndex = 0; nIndex<m_arrInputData.GetSize(); nIndex++)
		sFormattedText += m_arrInputData[nIndex];
	for (nIndex = 0; nIndex<m_arrRightLiterals.GetSize(); nIndex++)
		sFormattedText += m_arrRightLiterals[nIndex];
	return sFormattedText;
}
BOOL COXEdit::IsInputData(int nPosition) const
{
	return((nPosition >= m_arrLeftLiterals.GetSize() &&
		nPosition<m_arrLeftLiterals.GetSize() +
		m_arrInputData.GetSize()) ? TRUE : FALSE);
}
BOOL COXEdit::IsInputPosition(int nPosition) const
{
	return((nPosition >= m_arrLeftLiterals.GetSize() &&
		nPosition <= m_arrLeftLiterals.GetSize() +
		m_arrInputData.GetSize()) ? TRUE : FALSE);
}
int COXEdit::DeleteRange(int& nSelectionStart, int& nSelectionEnd)
{
	int nDeleteCount = 0;
	TCHAR chLastDeleted = _T('\0');
	TCHAR chFirstDeleted = _T('\0');
	int nIndex = 0;
	for (nIndex = nSelectionEnd - 1; nIndex >= nSelectionStart; nIndex--)
	{
		if (IsInputData(nIndex))
		{
			if (chFirstDeleted == _T('\0'))
			{
				chFirstDeleted = m_arrInputData[RPtoLP(nIndex)];
			}
			chLastDeleted = m_arrInputData[RPtoLP(nIndex)];
			m_arrInputData.RemoveAt(RPtoLP(nIndex));
			nDeleteCount++;
		}
	}
	if (chLastDeleted == _T('\r') && RPtoLP(nIndex + 1) != -1 &&
		m_arrInputData[RPtoLP(nIndex + 1)] == _T('\n'))
	{
		m_arrInputData.RemoveAt(RPtoLP(nIndex + 1));
		nDeleteCount++;
	}
	if (chFirstDeleted == _T('\n') && RPtoLP(nSelectionStart - 1) != -1 &&
		m_arrInputData[RPtoLP(nSelectionStart - 1)] == _T('\r'))
	{
		m_arrInputData.RemoveAt(RPtoLP(nSelectionStart - 1));
		nDeleteCount++;
		nSelectionStart--;
	}
	if (nDeleteCount)
	{
		Update(-1);
	}
	return nDeleteCount;
}
int COXEdit::InsertAt(int nSelectionStart, TCHAR chNewChar)
{
	int nInsertionPoint = nSelectionStart;
	if (!IsInputPosition(nInsertionPoint))
	{
		nInsertionPoint = GetNextInputLocation(nInsertionPoint + 1);
		ASSERT(IsInputPosition(nInsertionPoint));
	}
	m_arrInputData.InsertAt(RPtoLP(nInsertionPoint), chNewChar);
	nInsertionPoint += Validate();
	Update(-1);
	return nInsertionPoint;
}
int COXEdit::SetAt(int nSelectionStart, TCHAR chNewChar)
{
	int nInsertionPoint = nSelectionStart;
	if (!IsInputData(nInsertionPoint))
	{
		nInsertionPoint = GetNextInputLocation(nInsertionPoint + 1);
		if (!IsInputData(nInsertionPoint))
			return -1;
	}
	m_arrInputData.SetAt(RPtoLP(nInsertionPoint), chNewChar);
	nInsertionPoint += Validate();
	Update(-1);
	return nInsertionPoint;
}
int COXEdit::GetNextInputLocation(int nSelectionStart)
{
	int nNextInputLocation = nSelectionStart;
	if (!IsInputPosition(nNextInputLocation))
	{
		nNextInputLocation = PtrToInt(m_arrLeftLiterals.GetSize() +
			m_arrInputData.GetSize());
		for (int nIndex = nSelectionStart;
			nIndex<m_arrInputData.GetSize() + m_arrLeftLiterals.GetSize();
			nIndex++)
		{
			if (IsInputPosition(nIndex))
			{
				nNextInputLocation = nIndex;
				break;
			}
		}
	}
	ASSERT(IsInputPosition(nNextInputLocation));
	return nNextInputLocation;
}
int COXEdit::GetPreviousInputLocation(int nSelectionStart)
{
	int nPreviousInputLocation = nSelectionStart;
	if (!IsInputPosition(nPreviousInputLocation))
	{
		nPreviousInputLocation = PtrToInt(m_arrLeftLiterals.GetSize());
		for (int nIndex = nSelectionStart;
			nIndex >= m_arrLeftLiterals.GetSize();
			nIndex--)
		{
			if (IsInputPosition(nIndex))
			{
				nPreviousInputLocation = nIndex;
				break;
			}
		}
	}
	ASSERT(IsInputPosition(nPreviousInputLocation));
	return nPreviousInputLocation;
}
void COXEdit::Update(int nSelectionStart/*=0*/)
{
	if (::IsWindow(GetSafeHwnd()))
	{
		m_nSetTextSemaphor++;
		SetWindowText(GetFormattedText());
		m_nSetTextSemaphor--;
		SetModify(TRUE);
		if (nSelectionStart >= 0)
			UpdateInsertionPointForward(nSelectionStart);
	}
}
void COXEdit::UpdateInsertionPointForward(int nSelectionStart)
{
	int nNewInsertionPoint = GetNextInputLocation(nSelectionStart);
	SetSel(nNewInsertionPoint, nNewInsertionPoint);
}
void COXEdit::UpdateInsertionPointBackward(int nSelectionStart)
{
	int nNewInsertionPoint = GetPreviousInputLocation(nSelectionStart);
	SetSel(nNewInsertionPoint, nNewInsertionPoint);
}
int COXEdit::RPtoLP(int nRealPos)const
{
	if (nRealPos<m_arrLeftLiterals.GetSize() ||
		nRealPos>m_arrLeftLiterals.GetSize() + m_arrInputData.GetSize())
	{
		return -1;
	}
	else
	{
		return PtrToInt(nRealPos - m_arrLeftLiterals.GetSize());
	}
}
int COXEdit::LPtoRP(int nLogicalPos)const
{
	if (nLogicalPos<0 || nLogicalPos>m_arrInputData.GetSize())
	{
		return -1;
	}
	else
	{
		return PtrToInt(nLogicalPos + m_arrLeftLiterals.GetSize());
	}
}
void COXEdit::ValidationError()
{
	::MessageBeep(MB_ICONEXCLAMATION);
}
CString COXEdit::GetLeftLiterals() const
{
	CString sLeftLiterals;
	for (int nIndex = 0; nIndex<m_arrLeftLiterals.GetSize(); nIndex++)
	{
		sLeftLiterals += m_arrLeftLiterals[nIndex];
	}
	return sLeftLiterals;
}
CString COXEdit::GetRightLiterals() const
{
	CString sRightLiterals;
	for (int nIndex = 0; nIndex<m_arrRightLiterals.GetSize(); nIndex++)
	{
		sRightLiterals += m_arrRightLiterals[nIndex];
	}
	return sRightLiterals;
}
BOOL COXEdit::NotifyParent(UINT nNotificationID)
{
	CWnd* pParentWnd = GetParent();
	if (pParentWnd == NULL)
		return FALSE;
	pParentWnd->SendMessage(WM_COMMAND, MAKEWPARAM(GetDlgCtrlID(), nNotificationID),
		(LPARAM)GetSafeHwnd());
	return TRUE;
}

//------------------line number started
void COXEdit::OnEnable(BOOL bEnable)
{
	CEdit::OnEnable(bEnable);
	SetWindowColour(bEnable);
}
//extern HWND			mCurrentHwnd;
//extern HWND				m_hwndEditor;
void COXEdit::OnChange()
{
	//if (m_ScinCtrl.m_hWnd==NULL)
	//	m_ScinCtrl.m_hWnd = m_hwndEditor;
	CString  str;
	GetWindowText(str);
	if (str.IsEmpty())
	{
		//m_FindStr.SetTextColor(RGB(10, 0, 0));
		//SetBkColor(RGB(255, 162, 162));
		//	return;
	}
	else
	{
		if (theApp.GetGameAI()->sPage1.DarkMode)
		{
			SetBkColor(RGB(34, 34, 34));
			SetTextColor(RGB(255, 255, 255));
		}
		else
		{
			SetBkColor(::GetSysColor(COLOR_WINDOW));
			SetTextColor(::GetSysColor(COLOR_WINDOWTEXT));
		}
	}
	const char *ch = str.GetString();
	LPSTR lpstr = const_cast<LPSTR>(ch);
	if (false == ((theDlg*)m_pParent)->m_ScinCtrl.SearchForward(lpstr))
	{
		//TT
		//::SendMessage(m_hwndEditor, SCI_GOTOPOS, 0, 0);
		//m_ScinCtrl.GotoPosition(0);
		//m_ScinCtrl.SearchForward(lpstr);
	}

	//::SendMessage(mCurrentHwnd, WM_REFRESHBUTTONSTATES_TEST, 0, 0);
	//GetParent()->PostMessage(WM_REFRESHBUTTONSTATES_TEST, 0, 0);
	UpdateTopAndBottom();
}
void COXEdit::OnVscroll()
{
	UpdateTopAndBottom();
}
void COXEdit::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	CEdit::OnVScroll(nSBCode, nPos, pScrollBar);
	UpdateTopAndBottom();
}
LRESULT COXEdit::OnSetFont(WPARAM wParam, LPARAM lParam)
{
	DefWindowProc(WM_SETFONT, wParam, lParam);
	Prepare(m_enablelinenum);
	return 0;
}
LRESULT COXEdit::OnLineScroll(WPARAM wParam, LPARAM lParam)
{
	LRESULT retval = DefWindowProc(EM_LINESCROLL, wParam, lParam);
	UpdateTopAndBottom();
	return retval;
}
LRESULT COXEdit::OnSelectLine(WPARAM wParam, LPARAM)
{
	int lineno = wParam + GetScrollPos(SB_VERT);
	int start = LineIndex(lineno);
	int end = LineIndex(lineno + 1);
	SetSel(end - 1, start);
	//SetSel(start, end - 1);
	return 0;
}
int COXEdit::CalcLineNumberWidth()
{
	CClientDC dc(this);
	CFont* font = GetFont();
	CFont* oldFont = dc.SelectObject(font);
	m_zero = dc.GetTextExtent(_T("0"));
	CString format;
	int maxval = GetLimitText();
	if (m_maxval > 0)
		maxval = m_maxval + m_LineDelta;
	format.Format(m_format, maxval);
	CSize fmt = dc.GetTextExtent(format);
	dc.SelectObject(oldFont);
	return fmt.cx /*- 15*/;
}
void COXEdit::UpdateTopAndBottom()
{
	CRect rect;
	GetClientRect(&rect);
	int maxline = GetLineCount() + m_LineDelta;
	int lineheight = m_zero.cy;
	int topline = GetFirstVisibleLine() + m_LineDelta;
	if ((topline + (rect.Height() / lineheight)) < maxline)
		maxline = topline + (rect.Height() / lineheight);
	if (m_maxval > 0 && maxline > m_maxval + m_LineDelta)
		maxline = m_maxval + m_LineDelta;
	m_line.SetTopAndBottom(topline, maxline);
}
void COXEdit::SetWindowColour(BOOL bEnable)
{
	if (m_bUseEnabledSystemColours)
	{
		m_EnabledFgCol = GetSysColor(COLOR_WINDOWTEXT);
		m_EnabledBgCol = GetSysColor(COLOR_WINDOW);
	}
	if (m_bUseDisabledSystemColours)
	{
		m_DisabledFgCol = GetSysColor(COLOR_GRAYTEXT);
		m_DisabledBgCol = GetSysColor(COLOR_3DFACE);
	}
	if (bEnable)
	{
		m_line.SetFgColor(m_EnabledFgCol, TRUE);
		m_line.SetBgColor(m_EnabledBgCol, TRUE);
	}
	else {
		m_line.SetFgColor(m_DisabledFgCol, TRUE);
		m_line.SetBgColor(m_DisabledBgCol, TRUE);
	}
}
void COXEdit::Prepare(BOOL bEnable)
{
	//return;
	int width = CalcLineNumberWidth();
	CRect rect;
	GetClientRect(&rect);
	CRect rectEdit(rect);
	rect.right = width;
	rectEdit.left = rect.right + 1;
	SetRect(&rectEdit);
	{
		if (m_line.m_hWnd)
		{
			m_line.MoveWindow(1.5f, 0, width, rect.Height());
		}
		else
		{
			if (bEnable)
			{
				m_line.Create(NULL, WS_CHILD | WS_VISIBLE | SS_NOTIFY, rect, this, 1);
			}
		}
	}
	GetRect(&rectEdit);
	m_line.SetTopMargin(rectEdit.top);
	UpdateTopAndBottom();
}
void COXEdit::OnSysColorChange()
{
	CEdit::OnSysColorChange();
	Invalidate();
	SetWindowColour(IsWindowEnabled());
}
void COXEdit::UseSystemColours(BOOL bUseEnabled, BOOL bUseDisabled)
{
	m_bUseEnabledSystemColours = bUseEnabled;
	m_bUseDisabledSystemColours = bUseDisabled;
	BOOL bEnable = TRUE;
	if (::IsWindow(m_hWnd))
		bEnable = IsWindowEnabled();
	SetWindowColour(bEnable);
}
void COXEdit::SetLineNumberFormat(CString format)
{
	m_format = format;
	m_line.SetLineNumberFormat(format);
	if (m_hWnd)
		Prepare(m_enablelinenum);
}
void COXEdit::SetLineNumberRange(UINT nMin, UINT nMax)
{
	m_LineDelta = (int)nMin;
	m_maxval = (int)nMax;
}
void COXEdit::SetMarginForegroundColor(COLORREF col, BOOL redraw, BOOL bEnabled)
{
	m_line.SetFgColor(col, redraw);
	if (bEnabled)
	{
		m_bUseEnabledSystemColours = FALSE;
		m_EnabledFgCol = col;
	}
	else {
		m_bUseDisabledSystemColours = FALSE;
		m_DisabledFgCol = col;
	}
}
void COXEdit::SetMarginBackgroundColor(COLORREF col, BOOL redraw, BOOL bEnabled)
{
	m_line.SetBgColor(col, redraw);
	if (bEnabled)
	{
		m_bUseEnabledSystemColours = FALSE;
		m_EnabledBgCol = col;
	}
	else {
		m_bUseDisabledSystemColours = FALSE;
		m_DisabledBgCol = col;
	}
}



int COXEdit::Validate()
{
	static CString sFullText;
	int nRet = ValidateInput();
	CString sNewText = GetInputData();
	if ((::IsWindow(m_hWnd)) && (UINT)sNewText.GetLength()>GetLimitText())
	{
		int n = 0;
		for (n = 0; n<sFullText.GetLength(); n++)
			m_arrInputData.SetAtGrow(n, sFullText.GetAt(n));
		m_arrInputData.SetAtGrow(n, NULL);
	}
	sFullText = COXEdit::GetInputData();
	return nRet;
}