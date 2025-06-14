#include "stdafx.h"
#include "main.h"
#include "dialog.h"
#include "MHFileMng.h"
//#include "DarkMode.h"
#include "cmdline.h"
#include "QuickFindInfo.h"
BOOL				editorRectSave;
BOOL				jackjack;
HWND				m_DialogHwnd;
HWND				m_hwndEditor;
HWND				hwndMain;
extern HWND			mCurrentStylerHwnd;
HWND g_hWndListView;
extern BOOL			bjFlag;
extern CString		toastMsg;

static int			nLine;
const UINT			ID_TIMER_MINUTE = 0x1001;
const UINT			ID_TIMER_SECONDS = 0x1000;
void theDlg::OnQuickReplace()
{
	CString selectedText(m_ScinCtrl.GetSelectedText());
	CWnd *pWnd = GetDlgItem(IDC_SCINCTRL);
	if (m_QuickFind)
	{
		m_QuickFind->SetNotifyOwner(pWnd);
	}
	else
	{
		m_QuickFind = new CQuickFindWnd(this);
		m_QuickFind->CreateNew(pWnd);
	}
	m_QuickFind->SendMessage(WM_COMMAND, ID_EDIT_REPLACE);
	if (!selectedText.IsEmpty())
	m_QuickFind->SetFindString(selectedText);
	m_QuickFind->SetActiveShowWindow();
}
void theDlg::OnQuickFind()
{
	CString selectedText(m_ScinCtrl.GetSelectedText());
	CWnd *pWnd = GetDlgItem(IDC_SCINCTRL);
	if (m_QuickFind)
	{
		m_QuickFind->SetNotifyOwner(pWnd);		
	}
	else
	{
		m_QuickFind = new CQuickFindWnd(this);
		m_QuickFind->CreateNew(pWnd);
	}
	m_QuickFind->SendMessage(WM_COMMAND, ID_EDIT_FIND);
	if (!selectedText.IsEmpty())
	m_QuickFind->SetFindString(selectedText);
	m_QuickFind->SetActiveShowWindow();
}
void theDlg::CheckCloseOwnedFindWindow()
{
	CWnd *pWnd = GetDlgItem(IDC_SCINCTRL);
	if (::IsWindow(m_QuickFind->GetSafeHwnd()) && m_QuickFind->GetOwner() == pWnd)
	{
		m_QuickFind->SendMessage(WM_CLOSE);
		m_QuickFind->SetOwner(nullptr);
		m_QuickFind = nullptr;
	}
}
void theDlg::GetFontHeight()
{
	/*if (m_QuickFind)
	{
		printInt(m_QuickFind->isFocused());
		m_QuickFind->Invalidate();
	}
	else
	{
		AfxMessageBox("dialog is not created!");
	}*/
	
	//CheckCloseOwnedFindWindow();
	//m_ScinCtrl.SearchTest();
	//printInt(m_ScinCtrl.getCurrentDocLen());
	//m_ScinCtrl.GotoPosition(m_ScinCtrl.getCurrentDocLen() + 1);
	//m_ScinCtrl.SetSel(10, m_ScinCtrl.getCurrentDocLen()+1);
	//m_ScinCtrl.SetSel(50,100);
	//printInt(m_ScinCtrl.SendMessage(SCI_GETZOOM));
	//printInt(m_ScinCtrl.GetFontheight());
}
BOOL theDlg::PreTranslateMessage(MSG* pMsg)
{
	HWND hWnd = (HWND)pMsg->hwnd;
	if (hWnd != m_hWnd)
	{
		switch (pMsg->message)
		{
		case WM_SYSKEYDOWN:
			{
				switch (pMsg->wParam)
				{
					//FAST_SWITCH_RETURN('Q', requestExitApp(m_hWnd, TRUE), 1);
					case 'Q':
					FAST_SWITCH_RETURN(VK_F4, requestExitApp(m_hWnd, TRUE), 1);
					FAST_SWITCH_RETURN(VK_F1, SERVERCMD->OnCommand("/kill THIS"), TRUE);
				}
			}			
		case WM_KEYDOWN:
			{
				switch (pMsg->wParam)
				{					
					FAST_SWITCH(VK_F2, GetFontHeight())
					//FAST_SWITCH(VK_F2, randomGUID())
					FAST_SWITCH_RETURN(VK_ESCAPE, requestExitApp(m_hWnd, TRUE); /*exit(0);*/, 1)//return 1;
					FAST_SWITCH(VK_F3, OnBnClickedButtonFindstr())
					FAST_SWITCH(VK_F6, m_ScinCtrl.commentBlockToolStripMenuItem_Click())
					FAST_SWITCH(VK_F7, m_ScinCtrl.SetSel(-1))
					FAST_SWITCH(VK_F4, OnQuickFind())
				}
			}
		}
	}
	return CDialog::PreTranslateMessage(pMsg);
}
//extern TCHAR *szEOL[];
theDlg::theDlg(CWnd* pParent)
:CDialog(ID_DESCRIPTION_FILE, pParent)
, m_QuickFind(NULL)
, m_StylerDlg(NULL)
, m_SaveAsDlg(NULL)
, m_szMinimum(0, 0)
, m_pmodeless(NULL)
, m_text(_T(""))
//, editorRectSave(FALSE)
{
	jackjack = false;
	m_pParent = pParent;
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_hwndEditor = NULL;
	m_hwndOutput = NULL;
	m_sFileNum = _T("");
	m_sFileErrorNum = _T("");
	m_nFileNum = 0;
	m_nFileErrorNum = 0;
	m_nOldIndex = 0;
	//m_szMinimum = (0, 0);
	editorRectSave = FALSE;
}
void theDlg::OnFeature_bak()
{
	nLine = 0;
	SetTimer(1, 10, 0);
}
void theDlg::StartTimer_bak()
{
	SetTimer(ID_TIMER_MINUTE, 60 * 1000, 0);
	SetTimer(ID_TIMER_SECONDS, 2500, 0);
}
void theDlg::StopTimer_bak()
{
	KillTimer(ID_TIMER_MINUTE);
	KillTimer(ID_TIMER_SECONDS);
}
void theDlg::OnTimer_Bak(UINT nTimer)
{
	switch (nTimer)
	{
	case ID_TIMER_MINUTE:break;
	case ID_TIMER_SECONDS:OndefaultButtonTesting(FALSE, "");break;
	}
}
void theDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//DDX_Text(pDX, IDC_EDIT15, m_text);
	DDX_Control(pDX, IDC_LIST_FILE, m_FileList);
	//DDX_Control(pDX, IDC_SCINCTRL, m_ScinCtrl);
	DDX_Text(pDX, IPAKMAN_STATIC_FILENUM, m_sFileNum);
	DDX_Text(pDX, IPAKMAN_STATIC_FILENUMERROR, m_sFileErrorNum);
	DDX_Text(pDX, IPAKMAN_STATIC_EOLCONVERSION, m_sCurrentEOL);

	DDX_Text(pDX, IDC_EDIT_FINDSTR, m_sExt);
	DDX_Control(pDX, IDC_EDIT_FINDSTR, m_FindStr);
	DDX_Control(pDX, IDC_TAB, m_TabControl);

	DDX_Control(pDX, IDC_EDIT1, m_Edit1);
}
void theDlg::OndefaultButtonTestingButton(CString gStr)
{
	LPCTSTR pszB = gStr;// theApp.GetGameAI()->sPage1.fontName;
	CString sC(pszB);
	const char* pszE = sC;
	char* pszF = const_cast<char*>(pszE);
	OndefaultButtonTesting(TRUE, pszF);
	//m_Edit1.SetForegroundWindow();
	GetDlgItem(IPAKMAN_BUTTON_FIND)->SetFocus();
}
void theDlg::OndefaultButtonTesting(BOOL bFlag, char*stR)
{
	if (bFlag)
	{
		StartTimer_bak();
		bjFlag = TRUE;
		toastMsg = stR;
		m_dayValidate = new CDayValidate;
		m_Edit1.SetValidationHandler(m_dayValidate);
		m_Edit1.SetText();
		m_Edit1.SetFocus();
		//pCombox2.SetFocus();
		return;
	}
	StopTimer_bak();
	bjFlag = FALSE;
	m_Edit1.Hide();
}
void theDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}
/*void theDlg::OnCancel()
{
	theApp.ExitInstance();
		EndDialog(0);
}*/
bool theDlg::doStreamComment()
{
	/*const TCHAR *commentStart;
	const TCHAR *commentEnd;

	generic_string symbolStart;
	generic_string symbolEnd;

	//--FLS: BlockToStreamComment:
	const TCHAR *commentLineSybol;
	generic_string symbol;

	Buffer * buf = m_ScinCtrl.getCurrentBuffer();
	//--FLS: Avoid side-effects (e.g. cursor moves number of comment-characters) when file is read-only.
	if (buf->isReadOnly())
		return false;

	if (buf->getLangType() == L_USER)
	{
		UserLangContainer * userLangContainer = NppParameters::getInstance()->getULCFromName(buf->getUserDefineLangName());

		if (!userLangContainer)
			return false;

		//--FLS: BlockToStreamComment: Next two lines needed to decide, if block-comment can be called below!
		symbol = extractSymbol('0', '0', userLangContainer->_keywordLists[SCE_USER_KWLIST_COMMENTS]);
		commentLineSybol = symbol.c_str();

		symbolStart = extractSymbol('0', '1', userLangContainer->_keywordLists[SCE_USER_KWLIST_COMMENTS]);
		commentStart = symbolStart.c_str();
		symbolEnd = extractSymbol('0', '2', userLangContainer->_keywordLists[SCE_USER_KWLIST_COMMENTS]);
		commentEnd = symbolEnd.c_str();
	}
	else
	{
		//--FLS: BlockToStreamComment: Next line needed to decide, if block-comment can be called below!
		commentLineSybol = buf->getCommentLineSymbol();
		commentStart = buf->getCommentStart();
		commentEnd = buf->getCommentEnd();
	}

	// if ((!commentStart) || (!commentStart[0]))
	// 		return false;
	// if ((!commentEnd) || (!commentEnd[0]))
	// 		return false;
	//--FLS: BlockToStreamComment: If there is no stream-comment symbol, try the block comment:
	if ((!commentStart) || (!commentStart[0]) || (commentStart == NULL) || (!commentEnd) || (!commentEnd[0]) || (commentEnd == NULL)) {
		if (!(!commentLineSybol || !commentLineSybol[0] || commentLineSybol == NULL))
			return doBlockComment(cm_comment);
		else
			return false;
	}

	generic_string start_comment(commentStart);
	generic_string end_comment(commentEnd);
	generic_string white_space(TEXT(" "));

	start_comment += white_space;
	white_space += end_comment;
	end_comment = white_space;
	size_t start_comment_length = start_comment.length();
	size_t selectionStart = m_ScinCtrl.SendMessage(SCI_GETSELECTIONSTART);
	size_t selectionEnd = m_ScinCtrl.SendMessage(SCI_GETSELECTIONEND);
	size_t caretPosition = m_ScinCtrl.SendMessage(SCI_GETCURRENTPOS);
	// checking if caret is located in _beginning_ of selected block
	bool move_caret = caretPosition < selectionEnd;

	// if there is no selection?
	if (selectionEnd - selectionStart <= 0)
	{
		int selLine = m_ScinCtrl.SendMessage(SCI_LINEFROMPOSITION, selectionStart);
		selectionStart = m_ScinCtrl.SendMessage(SCI_GETLINEINDENTPOSITION, selLine);
		selectionEnd = m_ScinCtrl.SendMessage(SCI_GETLINEENDPOSITION, selLine);
	}
	m_ScinCtrl.SendMessage(SCI_BEGINUNDOACTION);
	_pEditView->insertGenericTextFrom(selectionStart, start_comment.c_str());
	selectionEnd += start_comment_length;
	selectionStart += start_comment_length;
	_pEditView->insertGenericTextFrom(selectionEnd, end_comment.c_str());
	if (move_caret)
	{
		// moving caret to the beginning of selected block
		m_ScinCtrl.SendMessage(SCI_GOTOPOS, selectionEnd);
		m_ScinCtrl.SendMessage(SCI_SETCURRENTPOS, selectionStart);
	}
	else
	{
		m_ScinCtrl.SendMessage(SCI_SETSEL, selectionStart, selectionEnd);
	}
	m_ScinCtrl.SendMessage(SCI_ENDUNDOACTION);*/
	return true;
}
void theDlg::OnButtonExit()
{
	//

	//requestExitApp(m_DialogHwnd, false);
	//theApp.ExitInstance();
	//EndDialog(-1);
	//TT
	//PostQuitMessage(0);
	//SERVERCMD->OnCommand("/kill THIS");
	//OnCancel();
	//OnOK();
	OnCancel();
}
void theDlg::OnRClicked(NMHDR * pNotifyStruct, LRESULT * result)
{
	OnButtonOpenSettings();
}
void theDlg::FixCurrentFileEOL()
{
	m_ScinCtrl.SendMessage(SCI_SETEOLMODE, SC_EOL_CRLF, 0);
	m_ScinCtrl.SendMessage(SCI_CONVERTEOLS, SC_EOL_CRLF); //m_ScinCtrl.Paste();
}
void theDlg::undo()
{
	//SendMessage(SCI_CUT, 0, 0);
	m_ScinCtrl.Undo();
}
void theDlg::redo()
{
	//SendMessage(SCI_COPY, 0, 0);
	m_ScinCtrl.Redo();
}
void theDlg::Cut()
{
	//SendMessage(SCI_CUT, 0, 0);
	m_ScinCtrl.Cut();
}
void theDlg::Copy()
{
	//SendMessage(SCI_COPY, 0, 0);
	m_ScinCtrl.Copy();
}
void theDlg::Paste()
{
	//TT
	//SendMessage(SCI_PASTE, 0, 0);
	m_ScinCtrl.Paste();
}
void theDlg::SelectAll()
{
	//SendMessage(SCI_SELECTALL, 0, 0);
	m_ScinCtrl.SelectAll();
}

void theDlg::OnBnClickedButton1()
{
	if (m_pmodeless)
		m_pmodeless->SetForegroundWindow();
	else
	{
		m_pmodeless = new CModeless(this);
		m_pmodeless->Create(CModeless::IDD, /*GetDesktopWindow()*/this);
		m_pmodeless->ShowWindow(SW_SHOW);
	}
}

BEGIN_MESSAGE_MAP(theDlg, CDialog)
	//ON_WM_CREATE()
	//ON_BN_CLICKED(IDC_BUTTON1, OnBnClickedButton1)
	ON_COMMAND(IDJ_MENU_TEST, OnBnClickedButton1)
	ON_COMMAND(IPAKMAN_STATIC_EOLCONVERSION, &theDlg::FixCurrentFileEOL)
	ON_COMMAND(ID_FILE_OPEN, &theDlg::OnLoadAllBinaryFile)//ON_COMMAND(IDC_MENU_FILE_OPENMULTIPLE, OnLoadAllBinaryFile)
	ON_COMMAND(ID_FILE_SAVEAS, &theDlg::OnButtonDecryptAS)//ON_COMMAND(IDC_MENU_FILE_SAVEACOPYAS, OnButtonDecryptAS)
	ON_COMMAND(IPAKMAN_BUTTON_ENCRYPTAS, &theDlg::OnButtonEncryptAS)
	ON_COMMAND(IPAKMAN_BUTTON_CLEAR, &theDlg::OnButtonDelall)
	ON_COMMAND(IPAKMAN_BUTTON_EXIT, &theDlg::OnButtonExit)
	ON_COMMAND(IDCANCEL, &theDlg::OnButtonExit)
	ON_BN_CLICKED(IDC_BUTTON_SETTING, &theDlg::OnButtonOpenSettings)
	ON_NOTIFY(NM_RCLICK, IDC_BUTTON_SETTING, &theDlg::OnRClicked)
	ON_COMMAND(IPAKMAN_BUTTON_ENCRYPT, &theDlg::OnButtonSaveBin)
	ON_COMMAND(IPAKMAN_STATIC_ALLEOLCONVERSION, &theDlg::FixLineEndings)
	ON_COMMAND(IDJ_MENU_PREVIEW, &theDlg::OnButtonOpenPreviewDlg)

	ON_COMMAND(ID_EDIT_UNDO,undo)
	ON_COMMAND(ID_EDIT_REDO, redo)
	ON_COMMAND(ID_EDIT_CUT, Cut)
	ON_COMMAND(ID_EDIT_COPY, Copy)
	ON_COMMAND(ID_EDIT_PASTE, Paste)
	ON_COMMAND(ID_EDIT_SELECT_ALL, SelectAll)
	/**********MENU ENDED*/	
	ON_WM_SYSCOMMAND()
	ON_WM_TIMER()
	ON_WM_PAINT()
	ON_WM_SIZE()
	//ON_WM_NCDESTROY()
	//ON_WM_DESTROY()
	ON_WM_CTLCOLOR()
	ON_WM_DROPFILES()
	ON_WM_GETMINMAXINFO()

	ON_COMMAND(IPAKMAN_BUTTON_REPLACE, &theDlg::OnQuickReplace)
	ON_COMMAND(IPAKMAN_BUTTON_FIND, &theDlg::OnBnClickedButtonFindstr)
	ON_LBN_SELCHANGE(IDC_LIST_FILE, &theDlg::OnSelchangeListFile)
	ON_EN_CHANGE(IDC_EDIT_FINDSTR, &theDlg::OnEnChangeEditFindstr)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB, &theDlg::OnClickTabCtrl)
	ON_NOTIFY(TCN_SELCHANGE, IDC_CONTEXT, &theDlg::OnClickTabCtrl)

	ON_MESSAGE(WM_CONTROLPLUSF, &theDlg::OnControlPlusF)
	ON_MESSAGE(WM_REFRESHBUTTONSTATES, &theDlg::OnRefreshButtonStates)
	ON_MESSAGE(WM_REFRESHBUTTONSTATES_EOL, &theDlg::OnRefreshButtonStatesEOL)
	ON_MESSAGE(WM_CONTROLCLEARLIST, &theDlg::OnClearButtonStates)


//	ON_COMMAND(ID_EDIT_REPLACE, &theDlg::OnEditReplace)

	ON_WM_LBUTTONDOWN()
	ON_WM_RBUTTONDOWN()
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // 상태 줄 표시기
	ID_INDICATOR_INFO,
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};
int theDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	RECT rect;
	rect.top += 2;
	rect.bottom += 200;

	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators, sizeof(indicators) / sizeof(UINT)))
	{
		TRACE0("상태 표시줄을 만들지 못했습니다.\n");
		return -1;      // 만들지 못했습니다.
	}
	m_wndStatusBar.SetPaneInfo(1, ID_SEPARATOR, SBPS_NORMAL, 100);

	return 0;
}
//#include "ListViewUtil.h"
//HINSTANCE g_hInst;
BOOL theDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	::OleInitialize(nullptr);
	
	m_DialogHwnd = m_hWnd;
	//SetWindowTheme(m_DialogHwnd, L"DarkMode_Explorer", nullptr);
	//AllowDarkModeForWindow(m_DialogHwnd, true);
	//SetTitleToDark(m_DialogHwnd);
	SetTitleBarThemeColor(m_DialogHwnd, true);

	
	
	darkHBR = CreateSolidBrush(RGB(34, 34, 34));	

	drawUAHMenuNCBottomLine(m_DialogHwnd);
	CMenu m_ShortMenu;
	m_ShortMenu.CreateMenu();
	m_ShortMenu.LoadMenu(IDR_MENUBAR);
	SetMenu(&m_ShortMenu);
	

	
	SetIcon(m_hIcon, TRUE);
	SetIcon(m_hIcon, FALSE);
	m_pmodeless = NULL;
	// the editor	
	{
		LPCTSTR lClass;
		if (theApp.GetScintillaEnable())
		{
			lClass = STR_SCINTILLAWND;
		}
		else
		{
			lClass = _T("Edit");
		}
		//m_ScinCtrl.SubclassDlgItem(IDC_SCINCTRL, this);

		DWORD scStyle = WS_CHILD | WS_VISIBLE | WS_TABSTOP | WS_CLIPCHILDREN |
			ES_MULTILINE | ES_AUTOVSCROLL | ES_AUTOHSCROLL | ES_WANTRETURN | WS_VSCROLL | WS_HSCROLL;
		//CWnd *pWnd = this->GetDlgItem(IDC_SCINCTRL);
		CWnd *pWnd = this->GetParentOwner();
		RECT rc;
		if (m_ScinCtrl.Create(STR_SCINTILLAWND, "", scStyle, rc, pWnd, IDC_SCINCTRL));
		{
			//InitialiseEditor();
			SetEditorPositionDialog();
		}
	}

	if ((m_szMinimum.cx == 0) && (m_szMinimum.cy == 0))
	{
		CRect rcWindow;
		GetWindowRect(rcWindow);
		m_szMinimum = rcWindow.Size();
	}
	CRect rcClient;
	GetClientRect(rcClient);
	m_szInitial = rcClient.Size();

	SMovingChildTest s3;
	GetDlgItem(IPAKMAN_PAKMAN_GROUP1, &s3.m_hWnd);
	::GetWindowRect(s3.m_hWnd, &s3.m_rcInitial);

	stWidth = s3.m_rcInitial.Width();
	stHeight = s3.m_rcInitial.Height() + 30;

	m_MovingChildrenTest.push_back(s3);

	OnSizeJack(m_szInitial.cx, m_szInitial.cy);


	const int iFontSize = 18;
	const CString sFont = L"Consolas";

	hFont = CreateFont(iFontSize, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, sFont);
	::SendMessage(m_FindStr.m_hWnd, WM_SETFONT, reinterpret_cast<WPARAM>(hFont), TRUE);

	//mbstowcs

	CWinApp* app = AfxGetApp();
	int sh, t, b, r, l;

	// only restore if there is a previously saved position
	if (-1 != (sh = app->GetProfileInt("Frame", "Status", -1)) &&
		-1 != (t = app->GetProfileInt("Frame", "Top", -1)) &&
		-1 != (l = app->GetProfileInt("Frame", "Left", -1)) &&
		-1 != (b = app->GetProfileInt("Frame", "Bottom", -1)) &&
		-1 != (r = app->GetProfileInt("Frame", "Right", -1))
		) {
		// restore the window's status
		app->m_nCmdShow = sh;

		// restore the window's width and height
		m_szInitial.cx = r - l;
		m_szInitial.cy = b - t;

		// the following correction is needed when the taskbar is
		// at the left or top and it is not "auto-hidden"
		RECT workArea;
		SystemParametersInfo(SPI_GETWORKAREA, 0, &workArea, 0);
		l += workArea.left;
		t += workArea.top;

		// make sure the window is not completely out of sight
		int max_x = GetSystemMetrics(SM_CXSCREEN) -
			GetSystemMetrics(SM_CXICON);
		int max_y = GetSystemMetrics(SM_CYSCREEN) -
			GetSystemMetrics(SM_CYICON);

		MoveWindow(min(l, max_x), min(t, max_y), SizeX, SizeY);
	}

	//m_btnOpenSetting.SubclassDlgItem(IDC_BUTTON_SETTING, this);

	RefreshDarkButton();



	//m_ScinCtrl.InitEdit();
	
	m_ScinCtrl.SetContextMenuId(IDC_EDITORCONTEXT);
	m_FileList.SetContextMenuId(IDR_XLISTBOX);
	setBorderEdge(m_FileList.GetSafeHwnd(), TRUE);
	setBorderEdge(GetDlgItem(IPAKMAN_PAKMAN_GROUP1)->GetSafeHwnd(), FALSE);





	
	BigenChar = 0;

	
	InitialiseEditor();
		//InitSaveGroupBoxRect();//KIV
	
	//InitFindStringEditControl();

	

	OnCenterWindow();
	
	theApp.SplashQuit();

	UpdateData(FALSE);	
	this->DragAcceptFiles(TRUE);
	CommandLine_N();




	/*try
	{

	}
	catch (...)
	{
		::PostMessage(m_hWnd, WM_CLOSE, reinterpret_cast<WPARAM>(""), reinterpret_cast<LPARAM>(""));
		::PostMessage(m_hWnd, WM_QUIT, reinterpret_cast<WPARAM>(""), reinterpret_cast<LPARAM>(""));
		return TRUE;
	}*/
	return TRUE;
}


LRESULT theDlg::OnClearButtonStates(WPARAM, LPARAM)
{
	//&theDlg::OnButtonDelall;
	OnButtonDelall();
	return 0L;
}
LRESULT theDlg::OnRefreshButtonStatesEOL(WPARAM, LPARAM)
{	
	int eol=m_ScinCtrl.SendMessage(SCI_GETEOLMODE);
	m_sCurrentEOL.Format(szEOL[eol]);
	return 0L;
}
void theDlg::RefreshButtonStates(BOOL bImmediate)
{
}
LRESULT theDlg::OnRefreshButtonStates(WPARAM wparam, LPARAM)
{
	Invalidate();
	RedrawWindow();
	//InvalidateRect()
	RefreshTitleBarThemeColor(m_hWnd);
	SetTitleToDark(m_hWnd);
	//this->ShowWindow(SW_SHOWNORMAL);
	m_StylerDlg = NULL;
	m_ScinCtrl.ShowScrollBar(SB_BOTH, 0);
	
	
	int caretPos = m_ScinCtrl.GetCurrentPosition();

	CString str;

	/*m_FindStr.Invalidate();
	m_FindStr.RedrawWindow();
	m_FindStr.GetWindowText(str);
	m_FindStr.SetWindowText(str);
	str.Empty();*/

	if (wparam == TRUE)
	{
		OndefaultButtonTestingButton("Saved");
	}
	else
	{
		OndefaultButtonTestingButton("Closed");
	}
	
	m_ScinCtrl.InitEdit();
	m_ScinCtrl.Refresh();
	
	m_ScinCtrl.GetWindowText(str);
	m_ScinCtrl.SetText(str);
	str.Empty();
	m_ScinCtrl.SetFocus();
	m_ScinCtrl.GotoPosition(caretPos);

	
	InitDarkButton(GetDlgItem(IPAKMAN_BUTTON_FIND)->m_hWnd);
	
	//OnSizeJack(m_szInitial.cx, m_szInitial.cy);
	return 0L;
}
CButton*theDlg::GetCurButton(int ID)
{
	CButton* pBtn = (CButton*)GetDlgItem(ID);
	return pBtn;
}
void theDlg::SetCommandLine_TEST(char* argv[])
{
	AfxMessageBox(argv[1]);
}
void theDlg::SetCommandLine(LPCTSTR aCommand)
{
	//AfxMessageBox(aCommand);
	CString sFooter = aCommand;	

	char mypath[_MAX_PATH + 1];
	GetModuleFileName(NULL, mypath, _MAX_PATH);
	sFooter.Replace(_T(mypath), _T(""));// remove first parameters then become "" "file haha.bin"
	sFooter.Replace(_T("\"\" "), _T(""));// remove "" " , then become this "file haha.bin"
	sFooter.Replace(_T("\""), _T(""));	// remove " then become this file haha.bin
	//AfxMessageBox(sFooter);

	BukaFileStatic((char *)sFooter.GetString());
	UpdateCurrentSelection();
	m_ScinCtrl.DeleteAllBookmark();

	::ShowWindow(this->m_hWnd, SW_SHOWNORMAL);
	BringWindowToTop();
	UpdateData(FALSE);
	


}
void theDlg::CommandLine_N()
{
	char cmd[MAX_PATH] = { 0, };
	int nCmdLen = strlen(AfxGetApp()->m_lpCmdLine);
	if (nCmdLen)
	{
		strcpy(cmd, AfxGetApp()->m_lpCmdLine + 1);
		cmd[nCmdLen - 2] = 0;

		BukaFileStatic(cmd);
		UpdateCurrentSelection();
		m_ScinCtrl.DeleteAllBookmark();
		BringWindowToTop();
		UpdateData(FALSE);
	}
}
void theDlg::CommandLine()
{
	CCommandLine pCmd;
	CString strFlag = _T("");
	CString strParam = _T("");
	CString Output;
	BOOL bRet = pCmd.GetFirstParameter(strFlag, strParam);

	while (bRet)
	{
		CT2A ascii(strParam, CP_UTF8);
		CString sFooter = strParam;
		sFooter += strParam;
		sFooter.Replace(_T("\""), _T(""));
		AfxMessageBox(sFooter.GetString());
		BukaFileStatic((char *)sFooter.GetString());
		UpdateCurrentSelection();
		m_ScinCtrl.DeleteAllBookmark();
		bRet = pCmd.GetNextParameter(strFlag, strParam);

		//if (CSingleInstance::Create(_T("E435FC13-82C1-4f80-97C5-006FF4A4E302")) == FALSE)
		//	return ;

		UpdateData(FALSE);
	}
}
void theDlg::InitFindStringEditControl()
{
	if (theApp.GetGameAI()->sPage1.DarkMode)
	{
		m_FindStr.SetBkColor(RGB(34, 34, 34));
		m_FindStr.SetTextColor(RGB(255, 255, 255));
	}
	else
	{
	}
	setBorderEdge(m_FindStr.GetSafeHwnd(), TRUE);	
}
void theDlg::SetGroupBoxGangNewPos(int x, int y, int w, int h)
{
	m_TabControl.ShowWindow(SW_HIDE);
	GetDlgItem(IPAKMAN_PAKMAN_GROUP1)->MoveWindow((x + 0) * 2, (y + 0) * 2, (w + 104) * 2, (h + 90) * 2);
	GetDlgItem(IPAKMAN_BUTTON_ENCRYPT)->MoveWindow((x + 4) * 2, (y + 9) * 2, (w + 47) * 2, (h + 12) * 2);
	GetDlgItem(IPAKMAN_BUTTON_EXIT)->MoveWindow((x + 4) * 2, (y + 23) * 2, (w + 47) * 2, (h + 12) * 2);
	GetDlgItem(IDC_EDIT_FINDSTR)->MoveWindow((x + 2) * 2, (y + 38) * 2, (w + 82) * 2, (h + 14) * 2);
	GetDlgItem(ID_FILE_OPEN)->MoveWindow((x + 4) * 2, (y + 56) * 2, (w + 47) * 2, (h + 12) * 2);
	GetDlgItem(IPAKMAN_BUTTON_CLEAR)->MoveWindow((x + 4) * 2, (y + 74) * 2, (w + 44) * 2, (h + 12) * 2);
	GetDlgItem(IPAKMAN_BUTTON_ENCRYPTAS)->MoveWindow((x + 54) * 2, (y + 9) * 2, (w + 47) * 2, (h + 12) * 2);
	GetDlgItem(ID_FILE_SAVEAS)->MoveWindow((x + 54) * 2, (y + 23) * 2, (w + 47) * 2, (h + 12) * 2);
	GetDlgItem(IPAKMAN_BUTTON_FIND)->MoveWindow((x + 85) * 2, (y + 38) * 2, (w + 16) * 2, (h + 14) * 2);
	GetDlgItem(IDC_BUTTON_SETTING)->MoveWindow((x + 54) * 2, (y + 56) * 2, (w + 47) * 2, (h + 12) * 2);
	GetDlgItem(IPAKMAN_STATIC_FILENUMERROR)->MoveWindow((x + 50) * 2, (y + 72) * 2, (w + 44) * 2, (h + 8) * 2);
	GetDlgItem(IPAKMAN_STATIC_FILENUM)->MoveWindow((x + 50) * 2, (y + 80) * 2, (w + 44) * 2, (h + 8) * 2);
}
void theDlg::OnClickTabCtrl(NMHDR* pNotifyStruct, LRESULT* pResult)
{
	if (pNotifyStruct->idFrom == IDC_TAB)
	{
		OnSelchangeListFile();
		int nCurTab = m_TabControl.GetCurSel();
		CMHFileEx* pFile = MHFileManager.GetFile(nCurTab);
		m_FileList.SetCurSel(nCurTab);
		m_ScinCtrl.SetText(pFile->GetData());
	}
	if (pNotifyStruct->idFrom == IDC_CONTEXT)
	{
		m_TabControl.DeleteItem(m_TabControl.GetCurSel());
		m_FileList.DeleteString(m_FileList.GetCurSel());
		m_TabControl.SetCurSel(m_TabControl.GetCurSel() - 1);
		m_FileList.SetCurSel(m_FileList.GetCurSel() - 1);
	}
	*pResult = 0;
}
void theDlg::UpdateDialogTitle(CString Text)
{
	::SetWindowText(m_hWnd, Text);
}
void theDlg::OnEnChangeEditFindstr()
{
	
	BigenChar = 0;
	
}
void theDlg::randomGUID()
{
	AfxMessageBox(generateGUID_N().MakeUpper());
}
void theDlg::OnBnClickedButtonFindstr()
{
	UpdateData();

	if (m_QuickFind)
	{
		if (IsShiftPressed())
			m_QuickFind->SendMessage(WM_COMMAND, ID_QUICKFIND_PREVIOUS);
		else
			m_QuickFind->SendMessage(WM_COMMAND, ID_QUICKFIND_NEXT);
	}
	/*if (false == m_sExt.IsEmpty())
	{
		if (IsShiftPressed())
		{
			if (false == m_ScinCtrl.SearchBackward((LPSTR)(LPCTSTR)m_sExt))
			{
			}
		}
		else
		{
			if (false == m_ScinCtrl.SearchForward((LPSTR)(LPCTSTR)m_sExt))
			{
			}
		}
		
	}
	else
	{
		m_FindStr.SetBkColor(RGB(255, 162, 162));
	}*/
	
	UpdateData(FALSE);
}
LRESULT theDlg::OnControlPlusF(WPARAM, LPARAM)
{
	OnQuickFind();
	/*if (FALSE == m_ScinCtrl.GetSelectedText().IsEmpty())
	{
		m_FindStr.SetWindowText(m_ScinCtrl.GetSelectedText());
	}
	m_FindStr.SetFocus();
	m_FindStr.SetSel(m_FindStr.GetWindowTextLength(), 0);*/
	return 0L;
}
void theDlg::FixLineEndings()
{
	//m_FileList.SetCurSel(0);
	for (UINT ii = 0; ii<m_nFileNum; ++ii)
	{
		//Sleep(1);
		m_ScinCtrl.SendMessage(SCI_SETEOLMODE, SC_EOL_CRLF, 0);
		m_ScinCtrl.SendMessage(SCI_CONVERTEOLS, SC_EOL_CRLF);
		m_FileList.SetCurSel(ii);	OnSelchangeListFile();
		
	}
}
void theDlg::OnButtonOpenPreviewDlg()
{
	CPreviewEditDlg *dlg = new CPreviewEditDlg();
	dlg->m_sExt = m_ScinCtrl.GetText();
	dlg->DoModal();
	delete (dlg);
	//if (dlg->DoModal() == IDOK)
	//{
	//}
	
}
void theDlg::OnButtonEncryptAS()
{
	m_SaveAsDlg = new CSaveAsDlg();
	m_SaveAsDlg->m_sExt = "bin";
	if (m_SaveAsDlg->DoModal() == IDOK)
	{		
		MHFileManager.SetExt(m_SaveAsDlg->m_sExt);
		SaveData(m_FileList.GetCurSel());
		MHFileManager.SaveToBin();
	}
	delete (m_SaveAsDlg);
}
void theDlg::OnButtonDecryptAS()
{
	m_SaveAsDlg = new CSaveAsDlg();
	m_SaveAsDlg->m_sExt = "txt";
	if (m_SaveAsDlg->DoModal() == IDOK)
	{		
		MHFileManager.SetExt(m_SaveAsDlg->m_sExt);
		SaveData(m_FileList.GetCurSel());
		MHFileManager.SaveToTxt();
	}
	delete (m_SaveAsDlg);
}
void theDlg::OnLoadAllBinaryFile()
{
	if (m_MultiBinaryLoadDlg.DoModal() == IDOK)
	{
		CString    strFolderPath;
		BROWSEINFO broInfo = { 0 };
		TCHAR       szDisName[MAX_PATH] = { 0 };
		broInfo.hwndOwner = this->m_hWnd;
		broInfo.pidlRoot = NULL;
		broInfo.pszDisplayName = szDisName;
		broInfo.lpszTitle = _T("Choose a path");
		broInfo.ulFlags = BIF_NEWDIALOGSTYLE | BIF_DONTGOBELOWDOMAIN
			| BIF_BROWSEFORCOMPUTER | BIF_RETURNONLYFSDIRS | BIF_RETURNFSANCESTORS;
		broInfo.lpfn = NULL;
		broInfo.lParam = NULL;
		broInfo.iImage = IDR_MAINFRAME;
		LPITEMIDLIST pIDList = SHBrowseForFolder(&broInfo);
		if (pIDList != NULL)
		{
			memset(szDisName, 0, sizeof(szDisName));
			SHGetPathFromIDList(pIDList, szDisName);
			strFolderPath = szDisName;
			BrowseCurrentAllFile(strFolderPath);
		}
	}
}
void theDlg::BrowseCurrentAllFile(CString strDir)
{
	if (strDir == _T(""))
	{
		return;
	}
	else
	{
		if (strDir.Right(1) != FOLDER_PATH_T)
			strDir += FOLDER_PATH_L;
		strDir = strDir + _T("*.*");
	}
	CFileFind finder;
	CString strPath;
	BOOL bWorking = finder.FindFile(strDir);
	while (bWorking)
	{
		bWorking = finder.FindNextFile();
		strPath = finder.GetFilePath();
		if (finder.IsDirectory() && !finder.IsDots())
		{
			BrowseCurrentAllFile(strPath);
		}
		else if (!finder.IsDirectory() && !finder.IsDots())
		{
			BukaFileStatic((char*)strPath.GetString());
		}
	}
	UpdateCurrentSelection();
}
void theDlg::UpdateCurrentSelection()
{
	if (m_FileList.GetCount() == 0)
	{
		return;
	}
	CMHFileEx* pFile = MHFileManager.GetFile(m_nFileNum - 1);
	//TT
	m_FileList.SetCurSel(m_nFileNum - 1);
	m_TabControl.SetRedraw(false);
	m_TabControl.SetCurSel(m_nFileNum);
	m_TabControl.SetRedraw(true);
	m_TabControl.Invalidate();
	m_TabControl.UpdateWindow();
	m_ScinCtrl.SetText(pFile->GetData());
	SetWindowText(pFile->GetFullFileName());
}
void theDlg::OnDropFiles(HDROP hDropInfo)
{
	char filename[MAX_PATH] = { 0, };
	UINT count = ::DragQueryFile(hDropInfo, 0xFFFFFFFF, filename, MAX_PATH);
	if (count)
	{
		for (UINT ii = 0; ii < count; ++ii)
		{
			DragQueryFile(hDropInfo, ii, filename, MAX_PATH);
			BukaFileDynamic(filename);
			//AfxMessageBox(filename);
		}
	}
	UpdateCurrentSelection();
	m_ScinCtrl.DeleteAllBookmark();
	UpdateData(FALSE);
	CDialog::OnDropFiles(hDropInfo);
}
void theDlg::BukaFileStatic(char* pFileName)
{
	CString strFile(pFileName);
	int nIndex = strFile.ReverseFind('.');
	CString strExtension = strFile.Right(strFile.GetLength() - nIndex - 1);
	if (m_MultiBinaryLoadDlg.GetFormatFromExtension(strExtension) == 1)
	{
		if (MHFileManager.AddFile(pFileName, "rb"))
		{
			char * str = MHFileManager.GetFileName(m_nFileNum);
			m_FileList.AppendList(1,str);
			AddTab(m_nFileNum, str);
			++m_nFileNum;
		}
	}
	UpdateList();
}
void theDlg::BukaFileDynamic(char* pFileName)
{
	CString strFile(pFileName);
	int nIndex = strFile.ReverseFind('.');
	CString strExtension = strFile.Right(strFile.GetLength() - nIndex - 1);
	if (m_ScinCtrl.GetFormatFromExtension(strExtension) == GETFILEEXTENSION)
	{
		if (MHFileManager.AddFile(pFileName, "rb"))
		{
			char * str = MHFileManager.GetFileName(m_nFileNum);
			m_FileList.AppendList(1,str);
			AddTab(m_nFileNum, MHFileManager.GetFileName(m_nFileNum));
			++m_nFileNum;
		}
	}
	else
	{
		if (MHFileManager.AddFile(pFileName, "rt"))
		{
			char * str = MHFileManager.GetFileName(m_nFileNum);
			m_FileList.AppendList(FALSE,str);
			AddTab(m_nFileNum, MHFileManager.GetFileName(m_nFileNum));
			++m_nFileNum;
			++m_nFileErrorNum;
		}
	}
	UpdateList();
}



void theDlg::UpdateList()
{
	if (m_nFileNum)
	{
		m_sFileNum.Format("%d", m_nFileNum);
		m_sFileErrorNum.Format("%d", m_nFileErrorNum);
		m_nOldIndex = m_nFileNum - 1;
		UpdateData(FALSE);
	}
}
void theDlg::OnButtonDelall()
{
	m_ScinCtrl.SendMessage(SCI_EMPTYUNDOBUFFER);
	MHFileManager.Clear();
	m_nFileNum = 0;
	m_nFileErrorNum = 0;
	m_sFileNum.Format("%d", m_nFileNum);
	m_sFileErrorNum.Format("%d", m_nFileErrorNum);
	m_FileList.ResetContent();
	m_ScinCtrl.ShowScrollBar(SB_BOTH, 0);
	m_ScinCtrl.OnButtonDelall();
	UpdateData(FALSE);
}

void theDlg::SaveData(int index)
{
	//TT
	CString str;
	m_ScinCtrl.GetWindowText(str);
	MHFileManager.SetFileData(index, str);
	str.Empty();
}
void theDlg::AddTab(int index, char*str)
{
	return;
	TC_ITEM item;
	item.mask = TCIF_TEXT;
	{
		m_TabControl.InsertItem(index, str);
		m_TabControl.SetCurSel(index);
	}
}
void theDlg::OnSelchangeListFile()
{
	SaveData(m_nOldIndex);
	int index = m_FileList.GetCurSel();
	if (m_nOldIndex == index)
	{
		m_FileList.SetCurSel(m_nOldIndex);
		return;
	}
	m_nOldIndex = index;
	CMHFileEx* pFile = MHFileManager.GetFile(index);
	CString str;
	str = MHFileManager.GetFullFileName(index);
	SetWindowText(str);
	m_ScinCtrl.SetText(pFile->GetData());
	UpdateData(FALSE);
}
void theDlg::InitialiseEditor()
{
	m_hwndEditor = m_ScinCtrl.m_hWnd;
	if (!::IsWindow(m_hwndEditor))
	{
		TRACE("Unable to create editor window\n");
		return;
	}
	m_ScinCtrl.InitEdit();
}
void theDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this);
		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}
void theDlg::DoDefaultFolding(int nMargin,long lPosition)	
{
	if (nMargin == 2)
	{
		long lLine = SendMessage(SCI_LINEFROMPOSITION, lPosition, 0);
		SendMessage(SCI_TOGGLEFOLD, lLine, 0);
	}
}
void theDlg::Refresh()
{
	SendMessage(SCI_COLOURISE, 0, -1);
}

BOOL theDlg::OnNotify(WPARAM wParam,LPARAM lParam,LRESULT* pResult)
{
	///////////////////////////////////////
	/*UAHMENU* pUDM = (UAHMENU*)lParam;
	RECT rc = {};

	// get the menubar rect
	{
	MENUBARINFO mbi = { sizeof(mbi) };
	::GetMenuBarInfo(m_DialogHwnd, OBJID_MENU, 0, &mbi);

	RECT rcWindow;
	::GetWindowRect(m_DialogHwnd, &rcWindow);

	// the rcBar is offset by the window rect
	rc = mbi.rcBar;
	OffsetRect(&rc, -rcWindow.left, -rcWindow.top);

	rc.top -= 1;
	}

	::FillRect(pUDM->hdc, &rc, CreateSolidBrush(RGB(34, 34, 34)));

	//////////////////////////////*/
	NMHDR *phDR;
	phDR = (NMHDR*)lParam;
	if (phDR != NULL && phDR->hwndFrom == m_hwndEditor)
	{
		SCNotification *pMsg = (SCNotification*)lParam;
		switch (phDR->code)
		{
		case SCI_NULL:TT
			break;
		case SCEN_CHANGE:
			//TT
			break;
		case SCN_STYLENEEDED:
			break;
		case SCN_CHARADDED:
			break;
		case SCN_SAVEPOINTREACHED:
			break;
		case SCN_SAVEPOINTLEFT:
			break;
		case SCN_MODIFYATTEMPTRO:
			break;
		case SCN_KEY:
			break;
		case SCN_DOUBLECLICK:
			break;
		case SCN_ZOOM:
		{
						 if (theApp.GetGameAI()->sPage1.m_nLineNumber)
							 m_ScinCtrl.UpdateLineNumberWidth();
		}
			break;
		case SCN_UPDATEUI:
		{
							 if (theApp.GetGameAI()->sPage1.m_nLineNumber)
								 m_ScinCtrl.UpdateLineNumberWidth();
							 m_ScinCtrl.doMatch();
		}
			break;
		case SCN_MODIFIED:
		{
							 m_ScinCtrl.OnChange();
							 //TT
							 //TT
		}
			break;
		case SCN_MACRORECORD:
			break;
		case SCN_MARGINCLICK:
		{
								//TT
								m_ScinCtrl.DoDefaultFolding(pMsg->margin, pMsg->position);
		}
			break;
		case SCN_NEEDSHOWN:
			break;
		case SCN_PAINTED:
			break;
		case SCN_USERLISTSELECTION:
			break;
		case SCN_URIDROPPED:
			break;
		case SCN_DWELLSTART:
			break;
		case SCN_DWELLEND:
			break;
		}
		return TRUE;
	}
	return CDialog::OnNotify(wParam, lParam, pResult);
}
void theDlg::ShowError(char * stRing)
{
}
void theDlg::OnMultiFileOpen()
{
	CFileDialog dlg(TRUE, NULL, "*.*", OFN_ALLOWMULTISELECT | OFN_HIDEREADONLY,
		_T(""));
	dlg.m_ofn.lpstrFilter = "Binary text\0*.bin; *.beff; *.befl; *.bmhm; *.bsad; *.md;\0"
		"Plain text\0*.txt\0";
	char buff[65535] = "";
	dlg.m_ofn.lpstrFile = buff;
	dlg.m_ofn.nMaxFile = 2048;
	CString fullfilename, filename;
	if (dlg.DoModal() == IDOK)
	{
		for (POSITION ps = dlg.GetStartPosition(); ps != NULL;)
		{
			fullfilename = dlg.GetNextPathName(ps);
			BukaFileDynamic(buff);
		}
		UpdateCurrentSelection();
	}
	UpdateData(FALSE);
}
void theDlg::OnSingleFileOpen()
{
	char fname[MAX_PATH + 1024] = "";
	OPENFILENAME	ofn;
	ZeroMemory((LPVOID)&ofn, sizeof(OPENFILENAME));
	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.lpstrFilter = "Binary (*.bin; *.beff; *.befl; *.bmhm; *.bsad)\x0*.bin; *.beff; *.befl; *.bmhm; *.bsad\x0"
		"Text (*.txt;)\x0*.txt;\x0";
	ofn.lpstrTitle = "Open Source File";
	ofn.Flags = OFN_HIDEREADONLY | OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
	ofn.lpstrFile = fname;
	ofn.nMaxFile = sizeof(fname);
	ofn.lpstrDefExt = "";
	ofn.hwndOwner = GetSafeHwnd();
	if (!::GetOpenFileName(&ofn))
		return;
	m_ScinCtrl.SendMessage(SCI_SETTEXT, 0, (WPARAM)"");
	if (0xffffffff == ::GetFileAttributes(fname))
	{
		ShowError("File does not exist"); return;
	}
	try
	{
		CFile f;
		if (!f.Open(fname, CFile::modeRead | CFile::shareDenyNone))
		{
			ShowError("Error opening file"); return;
		}
		UINT uSize = f.GetLength();
		if (!uSize)
		{
			ShowError("File of zero length"); return;
		}
		char *pBuf = new char[uSize + 1];
		if (!pBuf)
		{
			ShowError("Memory allocation error"); return;
		}
		if (f.Read(pBuf, uSize))
		{
			pBuf[uSize] = 0;
			m_ScinCtrl.SetText(pBuf);
		}
		std::string str = fname;
		MessageBox(getFileExt(str).c_str(), _T(""), MB_OK);
		f.Close();
		delete[] pBuf;
	}
	catch (...)
	{
		ShowError("Assertion while reading file");
	}
}
void theDlg::OnButtonSaveBin()
{
	//FixLineEndings();
	if (m_nFileNum == NULL)
	{
		return;
	}
	try
	{
		CString str;
		str.Format("There is %d Unencrypt files detected from the list,\
			\nby continuing this, those file will be encrypted without change its original extensions.", m_nFileErrorNum);
		if (m_nFileErrorNum!=0)
		if (MessageBox(str, "info", MB_YESNO | MB_ICONWARNING) == IDNO)
		{			
			return;
		}
		else
		{
			
		}
		
		//m_ScinCtrl.SendMessage(SCI_CONVERTEOLS, SC_EOL_CRLF);
		SaveData(m_FileList.GetCurSel());
		MHFileManager.SaveToBin();
		
	}
	catch (...)
	{
		ShowError("Assertion while reading file");
	}
	MessageBox("Saved!", "info", MB_OK);
}
void theDlg::OnFileSaveas()
{
	char fname[MAX_PATH + 1024] = "";
	OPENFILENAME	ofn;
	ZeroMemory((LPVOID)&ofn, sizeof(OPENFILENAME));
	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.lpstrFilter = "Binary (*.bin; *.beff; *.befl; *.bmhm; *.bsad)\x0*.bin; *.beff; *.befl; *.bmhm; *.bsad\x0"
		"Text (*.txt;)\x0*.txt;\x0";
	ofn.lpstrTitle = "Open Source File";
	ofn.Flags = OFN_HIDEREADONLY | OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
	ofn.lpstrFile = fname;
	ofn.nMaxFile = sizeof(fname);
	ofn.lpstrDefExt = "";
	ofn.hwndOwner = GetSafeHwnd();
	if (!::GetSaveFileName(&ofn))
		return;
	try
	{
		CFile f;
		if (!f.Open(fname, CFile::modeCreate | CFile::shareExclusive | CFile::modeWrite))
		{
			ShowError("Error creating file"); return;
		}
		UINT uSize = m_ScinCtrl.SendMessage(SCI_GETLENGTH, 0, 0L);
		if (!uSize) return;
		char *pBuf = new char[uSize + 1 + 8];
		if (!pBuf)
		{
			ShowError("Memory allocation error"); return;
		}
		m_ScinCtrl.SendMessage(SCI_GETTEXT, uSize + 1, (LPARAM)pBuf);
		pBuf[uSize] = 0;
		f.Write(pBuf, uSize);
		f.Close();
	}
	catch (...)
	{
		ShowError("Assertion while reading file");
	}
}
void theDlg::PostNcDestroy()
{
	TT
	CDialog::PostNcDestroy();
	if (m_pParent)
	{
		((theDlg*)m_pParent)->m_StylerDlg = NULL;
	}
	delete this;
}
void theDlg::OnDestroy()
{
	if (::IsWindow(m_StylerDlg->m_hWnd))
	{
		::DestroyWindow(m_StylerDlg->m_hWnd);
		m_StylerDlg->m_hWnd = NULL;
	}
	/*if (::IsWindow(mCurrentStylerHwnd))
	{
		::DestroyWindow(mCurrentStylerHwnd);
		mCurrentStylerHwnd = NULL;
	}*/
	if (::IsWindow(m_hwndEditor))
	{
		::DestroyWindow(m_hwndEditor);
		m_hwndEditor = NULL;
	}
	if (::IsWindow(m_DialogHwnd))
	{
		::DestroyWindow(m_DialogHwnd);
		m_DialogHwnd = NULL;
	}
	if (hFont != NULL)
	{
		delete hFont;
		delete []hFont;
		hFont = NULL;
	}
	CDialog::OnDestroy();
}
void theDlg::OnCenterWindow()
{
	int horizontal = 0;
	int vertical = 0;
	GetDesktopResolution(horizontal, vertical);
	float xx, yy;
	xx = ((float)horizontal / 2 - MIN_WINDOW_SIZE_X / 2);
	yy = ((float)vertical / 2 - MIN_WINDOW_SIZE_Y / 2);
	int max_x = GetSystemMetrics(SM_CXSCREEN) - GetSystemMetrics(SM_CXICON);
	int max_y = GetSystemMetrics(SM_CYSCREEN) - GetSystemMetrics(SM_CYICON);
	MoveWindow(xx, yy, MIN_WINDOW_SIZE_X, MIN_WINDOW_SIZE_Y);
}
void theDlg::InitSaveGroupBoxRect()
{
	SMovingChildTest s;
	GetDlgItem(IPAKMAN_PAKMAN_GROUP1, &s.m_hWnd);
	::GetWindowRect(s.m_hWnd, &s.m_rcInitial);
	stWidth = s.m_rcInitial.Width() + 8;
	stHeight = s.m_rcInitial.Height() + 8;
	m_MovingChildrenTest.push_back(s);
	CWinApp* app = AfxGetApp();
	int sh, t, b, r, l;
	if (-1 != (sh = app->GetProfileInt("Frame", "Status", -1)) &&
		-1 != (t = app->GetProfileInt("Frame", "Top", -1)) &&
		-1 != (l = app->GetProfileInt("Frame", "Left", -1)) &&
		-1 != (b = app->GetProfileInt("Frame", "Bottom", -1)) &&
		-1 != (r = app->GetProfileInt("Frame", "Right", -1))
		) {
		app->m_nCmdShow = sh;
		m_szInitial.cx = r - l;
		m_szInitial.cy = b - t;
		RECT workArea;
		SystemParametersInfo(SPI_GETWORKAREA, 0, &workArea, 0);
		l += workArea.left;
		t += workArea.top;
		int max_x = GetSystemMetrics(SM_CXSCREEN) - GetSystemMetrics(SM_CXICON);
		int max_y = GetSystemMetrics(SM_CYSCREEN) - GetSystemMetrics(SM_CYICON);
		MoveWindow(min(l, max_x), min(t, max_y), SizeX, SizeY);
	}
}
void theDlg::OnGetMinMaxInfo(MINMAXINFO* lpMMI)
{
	//lpMMI->ptMinTrackSize.x = MIN_WINDOW_SIZE_X;
	//lpMMI->ptMinTrackSize.y = MIN_WINDOW_SIZE_Y;
	//CDialog::OnGetMinMaxInfo(lpMMI);
	CDialog::OnGetMinMaxInfo(lpMMI);
	//if (lpMMI->ptMinTrackSize.x < m_szMinimum.cx)
	//	lpMMI->ptMinTrackSize.x = m_szMinimum.cx;
	//if (lpMMI->ptMinTrackSize.y < m_szMinimum.cy)
	//	lpMMI->ptMinTrackSize.y = m_szMinimum.cy;


	lpMMI->ptMinTrackSize.x = MIN_WINDOW_SIZE_X-100;
	//lpMMI->ptMinTrackSize.x = m_szMinimum.cx;
	lpMMI->ptMinTrackSize.y = m_szMinimum.cy;
}
BOOL theDlg::Size()
{
	if (!::IsWindow(GetSafeHwnd()))	return FALSE;
	RECT rect, ctrl, dlgRect, dlgOwnWH;
	GetClientRect(&rect);
	CopyRect(&ctrl, &rect);
	ctrl.bottom -= (6 * 24);
	ctrl.top = ctrl.bottom;
	ctrl.bottom = rect.bottom;
	m_ScinCtrl.MoveWindow(&ctrl);
	return TRUE;
}
void theDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);
	Invalidate();
	
	//m_FileList.MoveWindow(2, stHeight - 3, stWidth + 1, cy - stHeight);
	
	/*RECT crt;
	::GetClientRect(m_hWnd, &crt);
	int Width = crt.right - crt.left;*/

	

	/*CRect rect;
	GetWindowRect(rect);
	ScreenToClient(rect);
	//rect.top = IsShowReplaceUI() ? m_arrUIRows[2].top : m_arrUIRows[1].top;
	m_ScinCtrl.SetWindowPos(nullptr, -1, -1, rect.Width(), rect.Height(), SWP_NOSIZE | SWP_NOZORDER);*/


	OnSizeJack(cx, cy);
	
	/*if (m_ScinCtrl)
	{
		m_ScinCtrl.MoveWindow(stWidth + 4, 0, cx - stWidth - 4, cy);
		//m_ScinCtrl.UpdateWindowPos();
	}*/
	if (m_QuickFind)
	{
		m_QuickFind->UpdateWindowPos();
	}
		

	//m_TabControl.MoveWindow(0, 0, cx, 26);
}
void theDlg::OnSizeJack(int cx, int cy,int percentage)
{
	m_FileList.MoveWindow(2, stHeight-3, stWidth+1 , cy - stHeight);
	m_ScinCtrl.MoveWindow(stWidth+4, 0, cx - stWidth-4, cy);
	


	/*int b = 0 * 2;
	int bDef = 0;
	m_FileList.MoveWindow(2, (stHeight)+b, stWidth - 10, (cy - stHeight) - b);
	int percent;		
	if (editorRectSave)
		percent = theApp.GetGameAI()->sPage1.borderWidth;
	else
		percent = 0;
	m_ScinCtrl.MoveWindow(
		(stWidth - 4) + percent,
		(bDef + b) + percent,
		(cx - stWidth + 4) - percent * 2,
		((cy - bDef) - b) - percent * 2
		);*/
}
void theDlg::OnCBBorderWidth()
{
	
}
void theDlg::OnButtonTestBackcolor(COLORREF clr)
{
	UpdateData();
}


/*void theDlg::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if ((GetStyle()&ES_READONLY) == ES_READONLY)
	{
		CDialog::OnKeyDown(nChar, nRepCnt, nFlags);
		return;
	}
	BOOL bIsShiftKeyDown = ::GetAsyncKeyState(VK_SHIFT)<0;
	BOOL bIsCtrlKeyDown = ::GetAsyncKeyState(VK_CONTROL)<0;
	
	if (nChar == VK_HOME)
	{
		TT
		CDialog::OnKeyDown(nChar, nRepCnt, nFlags);
		if (!bIsShiftKeyDown)
		{
			int nSelectionStart = 0;
			int nSelectionEnd = 0;
		//	GetSel(nSelectionStart, nSelectionEnd);
		//	if (!IsInputPosition(nSelectionStart))
		//		UpdateInsertionPointForward(nSelectionStart);
		}
	}
	else if (nChar == VK_END)
	{
		CDialog::OnKeyDown(nChar, nRepCnt, nFlags);
		if (!bIsShiftKeyDown)
		{
			int nSelectionStart = 0;
			int nSelectionEnd = 0;
		//	GetSel(nSelectionStart, nSelectionEnd);
		//	if (!IsInputPosition(nSelectionStart))
		//		UpdateInsertionPointBackward(nSelectionStart);
		}
	}
	else if (nChar == VK_LEFT)
	{
		CDialog::OnKeyDown(nChar, nRepCnt, nFlags);
		if (!bIsShiftKeyDown)
		{
			int nSelectionStart = 0;
			int nSelectionEnd = 0;
		//	GetSel(nSelectionStart, nSelectionEnd);
		//	UpdateInsertionPointBackward(nSelectionStart);
		}
	}
	else if (nChar == VK_UP)
	{
		CDialog::OnKeyDown(nChar, nRepCnt, nFlags);
		if (!bIsShiftKeyDown)
		{
			int nSelectionStart = 0;
			int nSelectionEnd = 0;
		//	GetSel(nSelectionStart, nSelectionEnd);
		//	UpdateInsertionPointBackward(nSelectionStart);
		}
	}
	else if (nChar == VK_RIGHT)
	{
		CDialog::OnKeyDown(nChar, nRepCnt, nFlags);
		if (!bIsShiftKeyDown)
		{
			int nSelectionStart = 0;
			int nSelectionEnd = 0;
		//	GetSel(nSelectionStart, nSelectionEnd);
		//	UpdateInsertionPointForward(nSelectionStart);
		}
	}
	else if (nChar == VK_DOWN)
	{
		CDialog::OnKeyDown(nChar, nRepCnt, nFlags);
		if (!bIsShiftKeyDown)
		{
			int nSelectionStart = 0;
			int nSelectionEnd = 0;
		//	GetSel(nSelectionStart, nSelectionEnd);
		//	UpdateInsertionPointForward(nSelectionStart);
		}
	}
	else if (nChar == VK_INSERT)
	{
		if (!bIsShiftKeyDown && !bIsCtrlKeyDown)
		{
		//	SetInsertMode(!GetInsertMode());
		}
		else
			CDialog::OnKeyDown(nChar, nRepCnt, nFlags);
	}
	else
	{
		CDialog::OnKeyDown(nChar, nRepCnt, nFlags);
	}
}*/
void theDlg::OnButtonCloseSettings()
{
	Invalidate();
	m_ScinCtrl.InitEdit();
}
void theDlg::InitStyler()
{
	m_StylerDlg = new CStylerDlg(this);
	m_StylerDlg->Create(CStylerDlg::IDD, this);
	m_StylerDlg->InitAllDarkButton();
}
void theDlg::OnButtonOpenSettings()
{
	if (m_StylerDlg)
	{
	}
	else
	{
		InitStyler();
		m_StylerDlg->m_editorRect = m_editorRect;// GetEditorPositionDialog();
		m_StylerDlg->ShowWindow(SW_SHOW);
		::SetFocus(m_StylerDlg->m_hWnd);
		//OnKillFocus(this);
	}
}
void theDlg::RefreshDarkButton()
{
	InitStyler();
	
	m_StylerDlg = NULL;
}
void theDlg::SetEditorPositionDialog()
{
	editorRectSave = FALSE;
	m_editorRect = GetEditorPositionDialog();
	editorRectSave = TRUE;
}
CRect theDlg::GetEditorPositionDialog()
{
	CRect rect;
	CWnd *pWnd = this->GetDlgItem(IDC_SCINCTRL);
	pWnd->GetWindowRect(&rect);
	this->ScreenToClient(&rect);
	return rect;
}
HBRUSH theDlg::OnCtlColor(CDC* pDC, CWnd *pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	DWORD CtrlID = pWnd->GetDlgCtrlID();
	if (CtrlID == IDC_EDIT_FINDSTR)
	{
		//pDC->SetBkColor(theApp.GetGameAI()->sPage1.fontBackColor[0]);
		//pDC->SetTextColor(theApp.GetGameAI()->sPage1.fontTextColor[0]);
		return hbr;
	}

	if (theApp.GetGameAI()->sPage1.DarkMode)
	{
		hbr = darkHBR;
		pDC->SetTextColor(RGB(241, 241, 241));
		pDC->SetBkMode(TRANSPARENT);
	}
	if (CtrlID == IPAKMAN_STATIC_FILENUMERROR)
	{
		pDC->SetTextColor(RGB(255, 0, 0));
		pDC->SetBkMode(TRANSPARENT);
	}
	return hbr;
}
/*void theDlg::OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	if (nIDCtl == IPAKMAN_BUTTON_EXIT){
		CDC dc;
		RECT rect;
		dc.Attach(lpDrawItemStruct->hDC);
		rect = lpDrawItemStruct->rcItem;
		dc.Draw3dRect(&rect, RGB(255, 255, 255), RGB(0, 0, 0));
		dc.FillSolidRect(&rect, RGB(0, 121, 217));
		UINT state = lpDrawItemStruct->itemState;
		if ((state & ODS_SELECTED))
		{
			dc.DrawEdge(&rect, EDGE_SUNKEN, BF_RECT);
		}
		dc.SetBkColor(RGB(0, 121, 217));
		dc.SetTextColor(RGB(255, 255, 255));
		TCHAR buffer[MAX_PATH];
		ZeroMemory(buffer, MAX_PATH);
		::GetWindowText(lpDrawItemStruct->hwndItem, buffer, MAX_PATH);
		dc.DrawText(buffer, &rect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
		dc.Detach();
	}
	CDialog::OnDrawItem(nIDCtl, lpDrawItemStruct);
}*/
void theDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	//SetFocus();
	if (m_QuickFind)
		m_QuickFind->Invalidate();
	//TT
	CDialog::OnLButtonDown(nFlags, point);
}

void theDlg::OnRButtonDown(UINT nFlags, CPoint point)
{
	//SetFocus();
	if (m_QuickFind)
	m_QuickFind->Invalidate();
	//TT
	CDialog::OnRButtonDown(nFlags, point);
}
