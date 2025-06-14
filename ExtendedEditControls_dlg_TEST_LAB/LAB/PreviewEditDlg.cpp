#include "stdafx.h"
#include "PreviewEditDlg.h"
#include "darkmode.h"
CPreviewEditDlg::CPreviewEditDlg(): CDialog(IDD_DIALOG_POPUP_EXT)
{
	m_sInfo = "Semua file dalam list";
}
void CPreviewEditDlg::DoDataExchange(CDataExchange* pDX)
{
	__super::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_EXT, m_sExt);
	DDX_Text(pDX, IDC_POPUP_CSTATIC, m_sInfo);
}
BOOL CPreviewEditDlg::OnInitDialog()
{
	SetTitleToDark(m_hWnd);	
	return __super::OnInitDialog();
}
