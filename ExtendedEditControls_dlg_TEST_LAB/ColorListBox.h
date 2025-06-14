// This file was created on March 21st 2001. By Robert Brault
//
//
#if !defined(AFX_ColorEdit_H__E889B47D_AF6B_4066_B055_967508314A88__INCLUDED_)
#define AFX_ColorEdit_H__E889B47D_AF6B_4066_B055_967508314A88__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ColorEdit.h : header file
//
#include "Color.h" // File Holding (#define)'s for COLORREF Values

/////////////////////////////////////////////////////////////////////////////
// CColorListBox window
class CCheckListBox;
class CColorListBox : public CListBox
{
// Construction
public:
	CColorListBox();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CColorListBox)
	//}}AFX_VIRTUAL

	void SetBkColor(COLORREF crColor); // This Function is to set the BackGround Color for the Text and the Edit Box.
	void SetTextColor(COLORREF crColor); // This Function is to set the Color for the Text.
	BOOL SetReadOnly(BOOL flag = TRUE);
	virtual ~CColorListBox();
	virtual void OnOK();
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	virtual void MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct);
	HBRUSH CColorListBox::OnCtlColor(CDC* pDC, CWnd *pWnd, UINT nCtlColor);
	// Generated message map functions
public:
	int AddString(LPCTSTR lpszItem, COLORREF itemColor = RGB(255, 0, 0));
protected:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	CBrush m_brBkgnd; // Holds Brush Color for the Edit Box
	COLORREF m_crBkColor; // Holds the Background Color for the Text
	COLORREF m_crTextColor; // Holds the Color for the Text
	//{{AFX_MSG(CColorListBox)
	afx_msg HBRUSH CtlColor(CDC* pDC, UINT nCtlColor); // This Function Gets Called Every Time Your Window Gets Redrawn.
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ColorEdit_H__E889B47D_AF6B_4066_B055_967508314A88__INCLUDED_)
