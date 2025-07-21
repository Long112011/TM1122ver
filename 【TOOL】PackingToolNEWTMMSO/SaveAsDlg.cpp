
#include "stdafx.h"
#include "PackingTool.h"
#include "SaveAsDlg.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
CSaveAsDlg::CSaveAsDlg(CWnd* pParent )
	: CDialog(CSaveAsDlg::IDD, pParent)
{
	
	m_sExt = _T("txt");
	
}
void CSaveAsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	
	DDX_Text(pDX, IDC_EDIT_EXT, m_sExt);
	
}
BEGIN_MESSAGE_MAP(CSaveAsDlg, CDialog)
	
	
END_MESSAGE_MAP()
