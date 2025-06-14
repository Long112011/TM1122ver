
#include "stdafx.h"
#include "PackingTool.h"
#include "MyEdit.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
CMyEdit::CMyEdit()
{
	m_bFlag = FALSE;
	//this->SendMessage(EM_SETBKGNDCOLOR, 0, RGB( 123,43,143) );
	
}
CMyEdit::~CMyEdit()
{
}
BEGIN_MESSAGE_MAP(CMyEdit, CEdit)
	
	ON_WM_CREATE()
	ON_WM_DROPFILES()

	ON_WM_GETMINMAXINFO()
    ON_WM_SIZE()
	
END_MESSAGE_MAP()
BOOL CMyEdit::PreTranslateMessage(MSG* pMsg) 
{	
	/*if (pMsg->message == WM_KEYDOWN&&pMsg->wParam == VK_ESCAPE)
	{
		MessageBoxA("TESTING FILE 3 ", "TESTING FILE", MB_OK);
		return TRUE;
	}
	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN)
	{
		MessageBoxA("TESTING FILE 4", "TESTING FILE", MB_OK);
		return TRUE;
	}
	return CEdit::PreTranslateMessage(pMsg);*/
	switch (pMsg->message) 
    {
    case WM_KEYDOWN:
      
		switch(pMsg->wParam)
		{
		case VK_TAB:
			{
				if( m_bFlag == FALSE )
				{
					keybd_event( VK_CONTROL, MapVirtualKey(VK_CONTROL,0), 0, 0 );  
					keybd_event( VK_TAB, MapVirtualKey(VK_TAB,0), 0, 0 );
					keybd_event( VK_TAB, MapVirtualKey(VK_TAB,0), KEYEVENTF_KEYUP, 0 ); 
					keybd_event( VK_CONTROL, MapVirtualKey(VK_CONTROL,0), KEYEVENTF_KEYUP, 0 );
					m_bFlag = TRUE;
					return TRUE;
				}
				else
				{
					m_bFlag = FALSE;
				}
			}
			break;
		case VK_RETURN:
			{
				if( m_bFlag == FALSE )
				{
					keybd_event( VK_CONTROL, MapVirtualKey(VK_CONTROL,0), 0, 0 );  
					keybd_event( VK_RETURN, MapVirtualKey(VK_RETURN,0), 0, 0 );
					keybd_event( VK_RETURN, MapVirtualKey(VK_RETURN,0), KEYEVENTF_KEYUP, 0 ); 
					keybd_event( VK_CONTROL, MapVirtualKey(VK_CONTROL,0), KEYEVENTF_KEYUP, 0 );
					m_bFlag = TRUE;
					return TRUE;
				}
				else
				{
					m_bFlag = FALSE;
				}
			}
			break;
		}            
        break;
    default:
        return CEdit::PreTranslateMessage(pMsg);
    }
    return 0;
}
BOOL CMyEdit::PreCreateWindow(CREATESTRUCT& cs) 
{
	//this->SendMessage(EM_SETBKGNDCOLOR, 0, RGB( 123,43,143) );
	this->DragAcceptFiles();
	return CEdit::PreCreateWindow(cs);
}
int CMyEdit::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CEdit::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	
	return 0;
}
void CMyEdit::OnDropFiles(HDROP hDropInfo) 
{
	int a = 0;
	
	CEdit::OnDropFiles(hDropInfo);
}
void CMyEdit::OnSize(UINT nType, int cx, int cy)
{
	CEdit::OnSize(nType, cx, cy);

	//CRect rc;
	//GetClientRect(rc);
//	CDialog::OnSize(nType, cx, cy);
}