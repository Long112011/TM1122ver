// ListBoxTutorialDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ListBoxTutorial.h"
#include "ListBoxTutorialDlg.h"
#include <afxtempl.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

BOOL CALLBACK EnumWindowsProc( HWND hwnd, long lParam );

/////////////////////////////////////////////////////////////////////////////
// CListBoxTutorialDlg dialog

CListBoxTutorialDlg::CListBoxTutorialDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CListBoxTutorialDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CListBoxTutorialDlg)
	m_bLoadList = FALSE;
	m_strReport = _T("");
	m_InsertRemove = FALSE;
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_bTabbed = FALSE;
	for( int i=0; i<NumOfLists; i++ )
	{
		m_strSelection[i] = _T("");
	}
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_pFlyList = NULL;
}

CListBoxTutorialDlg::~CListBoxTutorialDlg()
{
	if( m_pFlyList )
		delete m_pFlyList;
}

void CListBoxTutorialDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CListBoxTutorialDlg)
//	DDX_Control(pDX, IDC_TOGGLEAREA, m_Template);
//	DDX_Check(pDX, IDC_CHECK1, m_bLoadList);
	DDX_Text(pDX, IDC_REPORT, m_strReport);
//	DDX_Check(pDX, IDC_CHECK2, m_InsertRemove);
	//}}AFX_DATA_MAP
	//for( int i=0; i<NumOfLists; i++ )
	{
		DDX_Control(pDX, IDC_LIST2/*+i*/, m_List[0]);
		DDX_LBString(pDX, IDC_LIST2/*+i*/, m_strSelection[0]);
	}
}

BEGIN_MESSAGE_MAP(CListBoxTutorialDlg, CDialog)
	//{{AFX_MSG_MAP(CListBoxTutorialDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
//	ON_BN_CLICKED(IDC_CHECK1, OnLoadList)
//	ON_BN_CLICKED(IDC_TEST, OnToggleListStyle)
//	ON_BN_CLICKED(IDC_CHECK2, OnInsertRemove)
//	ON_BN_CLICKED(IDC_SELECT, OnSelect)
	//}}AFX_MSG_MAP
	ON_CONTROL_RANGE(LBN_SELCHANGE,IDC_LIST1,IDC_LIST2/*+NumOfLists-1*/,OnSelchangeList)
	ON_CONTROL_RANGE(LBN_DBLCLK,IDC_LIST1,IDC_LIST2/*+NumOfLists-1*/,OnDblclkList)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CListBoxTutorialDlg message handlers

BOOL CListBoxTutorialDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here
	CRect rc;
	m_List[0].GetClientRect( &rc );
	//m_List[4].SetColumnWidth( rc.Width() );
	
	// set tabstop at half the listbox width 
	// the rest of the line is because the rectangle width in given in pixels and
	// whereas the SetTabStops uses dialog units.
	//m_List[5].SetTabStops(((rc.Width()/2)*4/3 * 4) / LOWORD(::GetDialogBaseUnits()));

	CString str;
	for( int i=0; i<NumOfLists; i++ )
		for( int j=0; j<10; j++ )
		{
			str.Format( "Item %d", j+1 );
			m_List[i].AddString( str );
		}

	// using static as a template so get its rect
	m_ListStyle = WS_CHILD|WS_VISIBLE|LBS_STANDARD|WS_HSCROLL;
	OnToggleListStyle();

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CListBoxTutorialDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CListBoxTutorialDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CListBoxTutorialDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CListBoxTutorialDlg::OnSelchangeList(UINT nID) 
{
	TRACE("CListBoxTutorialDlg::OnSelchangeList( %d )\n",nID);
	UpdateData();

	int list = nID - IDC_LIST2;
	if( list == 1 || list == 2 )
	{
		// Multi selections
		int count = m_List[list].GetSelCount();
		CArray<int,int> arrayListSel;
		arrayListSel.SetSize(count);								// make room in array
		m_List[list].GetSelItems(count, arrayListSel.GetData());	// copy data to array
		CString str = _T("");
		for( int i=0; i<count; i++ )
		{
			CString tmp = _T("");
			m_List[list].GetText( arrayListSel[i], tmp );
			str += (tmp + _T(", "));
		}
		m_strReport.Format( "The selection in List %d has changed to %s", list+1, str);
	}
	else if( list >= 0 )
		m_strReport.Format( "The selection in List %d has changed to %s", list+1, m_strSelection[list]);
	else
	{
		CString str;
		int idx = m_pFlyList->GetCurSel();
		m_pFlyList->GetText( idx, str );
		DWORD dw = m_pFlyList->GetItemData( idx );
		m_strReport.Format( "The selection in List 7 has changed to %s\n  Index = %d, and  ItemData = %d", str, idx, dw);
	}

	UpdateData(FALSE);
}

void CListBoxTutorialDlg::OnDblclkList(UINT nID) 
{
	TRACE("CListBoxTutorialDlg::OnDblclkList( %d )\n",nID);
	UpdateData();

	int list = nID - IDC_LIST2;
	if( list >= 0 )
		m_strReport.Format( "List %d has been Double clicked", list+1 );
	else
		m_strReport.Format( "List 7 has been Double clicked");
	
	UpdateData(FALSE);
}

void CListBoxTutorialDlg::OnLoadList() 
{
	TRACE("CListBoxTutorialDlg::OnLoadList( )\n");
	UpdateData();

	m_List[5].ResetContent();		// empty the list
	CString str = _T("");

	if( m_bLoadList )
		for( int j=0; j<10; j++ )
		{
			str.Format( "Item\t%d", j+1 );
			m_List[5].AddString( str );
		}
	else
		for( int j=0; j<10; j++ )
		{
			str.Format( "Item %d", j+1 );
			m_List[5].AddString( str );
		}
}

void CListBoxTutorialDlg::OnToggleListStyle() 
{
	TRACE("CListBoxTutorialDlg::OnToggleListStyle( )\n");

	if( m_pFlyList )
		delete m_pFlyList;

	CRect rc;
	CString str = _T("");
	m_Template.GetWindowRect( &rc );
	ScreenToClient( &rc );
	m_pFlyList = new CListBox;
	m_ListStyle ^= LBS_SORT;
	m_pFlyList->Create( m_ListStyle, rc, this, IDC_LIST1 );
	m_pFlyList->ModifyStyleEx( 0, WS_EX_CLIENTEDGE );
	// set font same as dialog to be sure they are the same
	m_pFlyList->SetFont( GetFont() );
	for( int j=0; j<10; j++ )
	{
		str.Format( "Item %d", j+1 );
		int idx = m_pFlyList->AddString( str );
		m_pFlyList->SetItemData( idx, j+1 );
	}

	// reset the checkbox
	m_InsertRemove = 0;
	UpdateData(FALSE);
}

void CListBoxTutorialDlg::OnInsertRemove() 
{
	TRACE("CListBoxTutorialDlg::OnInsertRemove( )\n");
	UpdateData();
	
	if( m_InsertRemove )
	{
		int idx = m_pFlyList->InsertString( 4, "Item 11" );
		m_pFlyList->SetItemData( idx, 11 );
	}
	else
	{
		int idx = m_pFlyList->FindString( -1, "Item 11" );
		m_pFlyList->DeleteString( idx );
	}
}

void CListBoxTutorialDlg::OnSelect() 
{
	TRACE("CListBoxTutorialDlg::OnSelect( )\n");

	m_List[0].SetCurSel( 2 );
	m_List[1].SelItemRange( TRUE, 3, 8 );
	m_List[1].SelItemRange( FALSE, 5, 5 );
	m_List[2].SelItemRange( TRUE, 3, 6 );
	m_pFlyList->SelectString( -1, "Item 4" );
}
