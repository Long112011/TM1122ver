#if !defined(AFX_MYBUTTON_H__46A1ECCC_0FAD_485A_B6B8_C21B6538148E__INCLUDED_)
#define AFX_MYBUTTON_H__46A1ECCC_0FAD_485A_B6B8_C21B6538148E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MyButton.h : header file
// CMyButton window

class CMyButton : public CButton  //CMyButton  =>derive from the CButton.
{
// Construction
public:
    CMyButton();

// Attributes
public:

// Operations
public:

// Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CMyButton)
    //}}AFX_VIRTUAL

// Implementation
public:
    virtual ~CMyButton();

    // Generated message map functions
protected:
    //{{AFX_MSG(CMyButton)
    afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
    //}}AFX_MSG

    DECLARE_MESSAGE_MAP()
};

#endif // !defined(AFX_MYBUTTON_H__46A1ECCC_0FAD_485A_B6B8_C21B6538148E__INCLUDED_)