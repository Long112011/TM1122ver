#include "stdafx.h"
#include "SaveAsDlg.h"
//#include "darkmode.h"
CSaveAsDlg::CSaveAsDlg(): CDialog(IDD_DIALOG_POPUP_EXT)
{
	m_sInfo = "Semua file dalam list";
}
void CSaveAsDlg::DoDataExchange(CDataExchange* pDX)
{
	__super::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_EXT, m_sExt);
	DDX_Text(pDX, IDC_POPUP_CSTATIC, m_sInfo);
}
BOOL CSaveAsDlg::OnInitDialog()
{
	SetTitleToDark(m_hWnd);	
	return __super::OnInitDialog();
}
