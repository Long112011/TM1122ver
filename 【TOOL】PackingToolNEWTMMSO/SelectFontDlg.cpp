
#include "stdafx.h"
#include "PackingTool.h"
#include "SelectFontDlg.h"
#include "PackingToolDlg.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
//CPackingToolDlg theApp2;
//extern UINT FontSize, FontColor, FontWeight, SizeX, SizeY;
//extern CString strUserName, strPassword, FontName;
CSelectFontDlg::CSelectFontDlg(CWnd* pParent )
	: CDialog(CSelectFontDlg::IDD, pParent)
{
	
	m_sExt = _T("txt");
	
}
void CSelectFontDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	
	DDX_Text(pDX, IDC_EDIT_EXT, m_sExt);
	
}
BEGIN_MESSAGE_MAP(CSelectFontDlg, CDialog)
	//ON_WM_PAINT()
	
END_MESSAGE_MAP()
