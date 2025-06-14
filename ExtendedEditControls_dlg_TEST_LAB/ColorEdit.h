
#pragma once
#include "Color.h" // File Holding (#define)'s for COLORREF Values

/////////////////////////////////////////////////////////////////////////////
// CColorEdit window

class CColorEdit : public CEdit
{
// Construction
public:
	CColorEdit();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CColorEdit)
	//}}AFX_VIRTUAL

	void SetBkColor(COLORREF crColor); // This Function is to set the BackGround Color for the Text and the Edit Box.
	void SetTextColor(COLORREF crColor); // This Function is to set the Color for the Text.
	BOOL SetReadOnly(BOOL flag = TRUE);
	virtual ~CColorEdit();

	// Generated message map functions
protected:
	CBrush m_brBkgnd; // Holds Brush Color for the Edit Box
	COLORREF m_crBkColor; // Holds the Background Color for the Text
	COLORREF m_crTextColor; // Holds the Color for the Text
	//{{AFX_MSG(CColorEdit)
	afx_msg HBRUSH CtlColor(CDC* pDC, UINT nCtlColor); // This Function Gets Called Every Time Your Window Gets Redrawn.
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};
