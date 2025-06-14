
#pragma once
// ColorStatic.h : header file
//
#include "color.h"
/////////////////////////////////////////////////////////////////////////////
// CColorButton window

class CColorButton : public CButton
{
// Construction
public:
	void SetTextColor(COLORREF crColor); // This Function is to set the Color for the Text.
	void SetBkColor(COLORREF crColor); // This Function is to set the BackGround Color for the Text.
	CColorButton();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CColorButton)
	//}}AFX_VIRTUAL

	virtual ~CColorButton();

	// Generated message map functions
protected:
	//{{AFX_MSG(CColorButton)

	CBrush m_brBkgnd; // Holds Brush Color for the Static Text
	COLORREF m_crBkColor; // Holds the Background Color for the Text
	COLORREF m_crTextColor; // Holds the Color for the Text

	afx_msg HBRUSH CtlColor(CDC* pDC, UINT nCtlColor);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};