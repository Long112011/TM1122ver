#include "stdafx.h"
#include "PreviewEditDlg.h"
//#include "darkmode.h"
CPreviewEditDlg::CPreviewEditDlg() : CDialog(IDD_DIALOG_PREVIEW_EXT)
{
}
void CPreviewEditDlg::DoDataExchange(CDataExchange* pDX)
{
	__super::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_EXT, m_sExt);
	DDX_Control(pDX, IDC_EDIT_EXT, m_Edit);
}
#include "ScintillaWnd.h"
BOOL CPreviewEditDlg::OnInitDialog()
{
	__super::OnInitDialog();

	m_Edit.SetTextColor(RGB(200, 200, 200));
	m_Edit.SetBkColor(RGB(60, 60, 60));
	m_Edit.SetLimitText(1024 * 1024 * 100);
	
	/*DWORD scStyle = WS_CHILD | WS_VISIBLE | WS_TABSTOP | WS_CLIPCHILDREN;
	// ES_MULTILINE | ES_AUTOVSCROLL | ES_AUTOHSCROLL | ES_WANTRETURN | WS_VSCROLL | WS_HSCROLL | WS_TABSTOP;	
	RECT rc = { 10, 10, 500, 400 };
	m_ScinCtrl.Create(STR_SCINTILLAWND,"", scStyle, rc, this->GetParentOwner(), IDC_EDIT_EXT);	
	m_ScinCtrl.InitEdit();
	m_ScinCtrl.SetBraceText();*/

	//m_Edit.SetReadOnly(TRUE);
	//hEdit = CScintillaWnd::CreateEditor("", scStyle, rc, GetSafeHwnd(), IDC_EDIT_EXT);
	//m_ScinCtrl.Create("Edit", "", scStyle, rc, this->GetParentOwner(), IDC_EDIT_EXT);
	//m_ScinCtrl.CreateEditorNormal("", scStyle, rc, GetSafeHwnd(), IDC_EDIT_EXT);
	//SetWindowSubclass(m_ScinCtrl.m_hWnd, NULL, 0, 0);
	//m_ScinCtrl.SetFocus();
	//m_ScinCtrl.InitEdit();
	UpdateData(FALSE);
	return TRUE;
}
void CPreviewEditDlg::goDark()
{
	SetTitleToDark(m_hWnd);

	HWND hButton = GetDlgItem(IDOK)->m_hWnd;
	SetWindowTheme(hButton, L"Explorer", nullptr);
	AllowDarkModeForWindow(hButton, TRUE);
	UpdateWindow();
}