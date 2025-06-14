#include "stdafx.h"
#include "OpenMultiBin.h"
#include "Tokenizer.h"
//#include "darkmode.h"
enum{
	FOUNDTHEM=1,
	FAILTOFIND=0
};
static TCHAR *szExtensionsNew[] =
{
	"bin,bsad,beff,befl,bmhm,map,md",										// GETFILEEXTENSION
	0,
};
static int nFormatsNew[] = {
	FOUNDTHEM,
	FAILTOFIND,
};


COpenMultiBin::COpenMultiBin()
: CDialog(COpenMultiBin::IDD)	
{
	m_sExt = szExtensionsNew[1 - 1];
	m_sInfo = "Supports multiple extension";
	m_nContextMenuId = (UINT)-1;
}
void COpenMultiBin::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);	
	DDX_Text(pDX, IDC_EDIT_EXT, m_sExt);
	DDX_Text(pDX, IDC_POPUP_CSTATIC, m_sInfo);
	DDX_Control(pDX, IDC_EDIT_EXT, m_sExtEdit);
}
BEGIN_MESSAGE_MAP(COpenMultiBin, CDialog)	
	ON_WM_CONTEXTMENU()
END_MESSAGE_MAP()
BOOL COpenMultiBin::OnInitDialog()
{
	SetTitleToDark(m_hWnd);
	CDialog::OnInitDialog();

	//m_sExtEdit.SetContextMenuId(IDR_XLISTBOX);
	return TRUE;
}
int COpenMultiBin::GetFormatFromExtension(LPCSTR szExtension) //@parm filename extension without dot e.g. "cpp"                                           
{


	//szExtensionsNew[1 - 1] = { m_sExt };
	//_tcscpy(szExtensionsNew[1 - 1], TEXT(m_sExt));
	//std::wstring string;

	//string = szExtensionsNew[0];
	//trim(szExtensionsNew[0]);
	szExtensionsNew[1 - 1] = _tcsdup(m_sExt);

	//AfxMessageBox(_T(szExtensionsNew[1-1]));


	int i = 0;
	TCHAR *pExtension = szExtensionsNew[i];
	while (pExtension != NULL)
	{
		CTokenizer st(szExtensionsNew[i], _T(","));
		while (st.HasMoreTokens())
		{
			if (st.Next().CompareNoCase(szExtension) == 0)
				return nFormatsNew[i];
		}
		i++;
		pExtension = szExtensionsNew[i];
	}
	free(szExtensionsNew[1 - 1]);
	return FAILTOFIND;
}
void COpenMultiBin::OnContextMenu(CWnd* /*pWnd*/, CPoint point)
{
	AFX_MANAGE_STATE(AFXMANAGESTATE())

	if (m_nContextMenuId == -1)
	{
		TRACE(_T("no context menu\n"));
		return;
	}

	CMenu menu;
	if (!menu.LoadMenu(m_nContextMenuId))
	{
		TRACE(_T("ERROR failed to load %d\n"), m_nContextMenuId);
		return;
	}

	menu.GetSubMenu(0)->TrackPopupMenu(0,
		point.x, point.y, this, NULL);
}