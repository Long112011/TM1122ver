#include "stdafx.h"
#include "Button_Input.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMyButton

CMyButton::CMyButton()
{
}

CMyButton::~CMyButton()
{
}

BEGIN_MESSAGE_MAP(CMyButton, CButton)
    //{{AFX_MSG_MAP(CMyButton)
    ON_WM_RBUTTONUP()
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMyButton message handlers

void CMyButton::OnRButtonUp(UINT nFlags, CPoint point) 
{
	//TT
    // TODO: Add your message handler code here and/or call default
    NMHDR hdr;
    hdr.code = NM_RCLICK;
    hdr.hwndFrom = this->GetSafeHwnd();
    hdr.idFrom = GetDlgCtrlID();
    TRACE("OnRButtonUp");
    this->GetParent()->SendMessage(WM_NOTIFY, (WPARAM)hdr.idFrom, (LPARAM)&hdr);
}