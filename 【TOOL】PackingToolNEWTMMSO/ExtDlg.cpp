
#include "stdafx.h"
#include "PackingTool.h"
#include "PackingToolDlg.h"
#include "ExtDlg.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
extern CPackingToolDlg theApp2;
CExtDlg::CExtDlg(CWnd* pParent )
	: CDialog(CExtDlg::IDD, pParent)
{
	
	m_sExt = _T("bin");
	//CWinApp* app = AfxGetApp();
	
}
void CExtDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	
	DDX_Text(pDX, IDC_EDIT_EXT, m_sExt);
	
}
BEGIN_MESSAGE_MAP(CExtDlg, CDialog)
	
	ON_BN_CLICKED(IDOK, TestClick)
END_MESSAGE_MAP()
void CExtDlg::TestClick()
{
	//theApp2.TestMsgFromAnotherFile();
	//CString  Big5Buffer, str;
	//m_Edit.GetWindowText(Big5Buffer);
	OnOK();
}