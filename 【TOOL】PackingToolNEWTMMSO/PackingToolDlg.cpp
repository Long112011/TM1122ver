#include "stdafx.h"
#include "PackingTool.h"
#include "PackingToolDlg.h"
#include "TList.h"
//#include "SkinScrollWnd.h"
#define FONTPATH1 ".\\Image\\InterfaceScript\\gamefont.ttc"
#define FONTPATH2 ".\\gamefont.ttc"
//#define FONTNAME "Arial"
int b = 0;
extern CString strUserName, strPassword, FontName;
extern UINT FontSize, FontColor, FontWeight, SizeX, SizeY;
extern CPackingToolApp theApp;
CPackingToolDlg theApp2;

//MessageBox("false", "false", MB_OK);
CPackingToolDlg::CPackingToolDlg(CWnd* pParent)
: CDialog(CPackingToolDlg::IDD, pParent),
m_pTList(NULL),
m_szMinimum(0, 0),
m_nlinePerScreen(0),
m_nScrollMaxPos(0),
m_nScrollPos(0)

{
	m_sFileNum = _T("");
	m_sCurFile = _T("");
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}
void CPackingToolDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BUTTON_SAVE_BIN, m_BtnSave);
	DDX_Control(pDX, IDC_BUTTON_SAVE_AS_BIN, m_BtnSaveAsBin);
	DDX_Control(pDX, IDC_EDIT, m_Edit);
	DDX_Control(pDX, IDC_BUTTON_SAVE_TO_TXT, m_BtnSaveAllToTxt);
	DDX_Control(pDX, IDC_BUTTON_SAVE_TO_BIN, m_BtnSaveAllToBin);
	DDX_Control(pDX, IDC_BUTTON_OPEN_BIN, m_BtnOpenBin);
	DDX_Control(pDX, IDC_BUTTON_OPEN, m_BtnOpen);
	DDX_Control(pDX, IDC_BUTTON_NEW, m_BtnNew);
	DDX_Control(pDX, IDC_BUTTON_DELALL, m_BtnDelAll);
	DDX_Control(pDX, IDC_LIST_FILE, m_FileList); 
	//DDX_Control(pDX, IDC_LINE_NUMBER, m_LineNumber);
	DDX_Text(pDX, IDC_STATIC_FILENUM, m_sFileNum);
	DDX_Text(pDX, IDC_STATIC_CURFILE, m_sCurFile);
	DDX_Control(pDX, IDC_EDIT_FINDSTR, m_FindStr);
	DDX_Control(pDX, IDC_STATIC_FONT, m_static1);

	//DDX_Control(pDX, IDC_SEPARATOR_COMBO, m_sepCombo);
	//DDX_Control(pDX, IDC_SEPARATOR_BUTTON, m_sepButton);
	//DDX_Control(pDX, IDC_COMBO_FONT_PICKER, m_cmbFontPicker);

	//DDX_Control(pDX, IDC_BUTTON_FONT_PICKER, m_btnFontPicker);//
	//DDX_FontPickerFont(pDX, IDC_BUTTON_FONT_PICKER, &m_font2);//

	//DDX_Control(pDX, IDC_SELECT_FONT, m_BtnSelFont);
	//DDX_Control(pDX, IDC_SCROLLBAR2, m_ctrlScroll);
}
BEGIN_MESSAGE_MAP(CPackingToolDlg, CDialog)
	ON_WM_CLOSE()
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_OPEN, OnButtonOpen)
	ON_BN_CLICKED(IDC_BUTTON_SAVE_TO_BIN, OnButtonSaveToBin)
	ON_BN_CLICKED(IDC_BUTTON_OPEN_BIN, OnButtonOpenBin)
	ON_BN_CLICKED(IDC_BUTTON_SAVE_TO_TXT, OnButtonSaveToTxt)
	ON_BN_CLICKED(IDC_BUTTON_DELALL, OnButtonDelall)
	ON_LBN_SELCHANGE(IDC_LIST_FILE, OnSelchangeListFile)
	ON_BN_CLICKED(IDC_BUTTON_NEW, OnButtonNew)
	ON_BN_CLICKED(IDC_BUTTON_SAVE_AS_BIN, OnButtonSaveAsBin)
	ON_WM_DROPFILES()
	ON_BN_CLICKED(IDC_BUTTON_SAVE_BIN, OnButtonSaveBin)
	ON_COMMAND(ID_SEARCH, OnSearch)
	ON_BN_CLICKED(IDC_BUTTON_SAVEDSOF, OnButtonSavedsof)
	ON_BN_CLICKED(IDC_BUTTON1, OnBnClickedButton1)
	ON_BN_CLICKED(IDC_SELECT_FONT, OnBnClickedSelectFont)
	ON_BN_CLICKED(IDC_BUTTON_FINDSTR, &CPackingToolDlg::OnBnClickedButtonFindstr)
	ON_EN_CHANGE(IDC_EDIT_FINDSTR, &CPackingToolDlg::OnEnChangeEditFindstr)
	ON_EN_CHANGE(IDC_EDIT, &CPackingToolDlg::OnEnChangeEdit)
	ON_WM_GETMINMAXINFO()
	ON_WM_SIZE()
	//ON_WM_HSCROLL()
	//ON_WM_VSCROLL()
	ON_WM_DESTROY()
	ON_WM_CTLCOLOR()
	ON_WM_DRAWITEM()	

	ON_WM_LBUTTONDOWN()
	//ON_BN_CLICKED(IDC_BUTTON_FONT_PICKER, OnButtonFontPicker)//
	//ON_BN_CLICKED(IDC_BUTTON_FONT_PICKER, &CPackingToolDlg::OnBnClickedSelectFont)
END_MESSAGE_MAP()

CTList * CPackingToolDlg::GetTList()
{
	return m_pTList;
}
BOOL CPackingToolDlg::IsStyle(DWORD dwStyle)
{
	return 0;// (dwStyle & m_DESC.dwListStyle);
}

#define dwWidth  (800  - 10)
#define dwHeight (400 - 58)
#define dwExHeight -0.5f
void CPackingToolDlg::CreateList(LOGFONT * pFont)
{
	DestroyList();
	//CPackingToolDlg dlg;
	m_pTList = new CTList;
	m_pTList->CreateList(TLO_NOTMESSAGECLOSE|TLO_LINENUMBER|TLO_SCROLLTUMBTRACKUPDATE, 
					0, 0, 800, 400+dwExHeight,//0, 0, (m_DESC.Width - 0), (m_DESC.Height - FIXED_HEIGHT),
					65535, 100, 
					m_hWnd, 0);



	//if(!(TLO_NOTMESSAGECLOSE|TLO_LINENUMBER|TLO_SCROLLTUMBTRACKUPDATE & TL_NAIVEFONT))
		m_pTList->CreateFont(pFont);
	m_pTList->SetLineHeightMargin(1);//2
}
void CPackingToolDlg::DestroyList()
{
	if(m_pTList)
	{
		delete m_pTList;
		m_pTList = NULL;
	}
}
void CPackingToolDlg::OnBnClickedSelectFont()
{
	//if (m_SelectFontDlg.DoModal() == IDOK)
	/*{
		//m_FileMng.SetExt(m_SaveAsDlg.m_sExt);
		//SaveData(m_FileList.GetCurSel());
		//m_FileMng.SaveToTxt();
		BOOL bRet;
		CHOOSEFONT cf;
		static LOGFONT lf;        // logical font structure
		static COLORREF rgbCurrent;  // current text color
		rgbCurrent = RGB(0, 0, 0);
		rgbColor = rgbCurrent;
		int nPointSize;
		// Initialize CHOOSEFONT
		ZeroMemory(&cf, sizeof(cf));
		cf.lStructSize = sizeof (cf);
		cf.hwndOwner = NULL;

		lf.lfHeight = -MulDiv(cf.iPointSize * 10, 96, 72);
		lf.lfWidth = -lf.lfHeight / 2;
		lf.lfCharSet = 134;
		lf.lfQuality = PROOF_QUALITY;

		cf.lpLogFont = &lf;
		cf.rgbColors = rgbCurrent;

		cf.Flags = CF_SCREENFONTS | CF_EFFECTS;
		bRet = ChooseFont(&cf);

		if (bRet)
		{
			rgbColor = cf.rgbColors;
			m_font.CreateFontIndirect(&lf);
			nPointSize = cf.iPointSize;
		
			m_Edit.SetFont(&m_font, TRUE); 

			//m_LineNumber.SetFont(&m_font, TRUE);

			theApp.WriteProfileString("Init", "FontName", lf.lfFaceName);
			theApp.WriteProfileInt("Init", "FontSize", nPointSize); 
			theApp.WriteProfileInt("Init", "FontColor", (DWORD)rgbColor);
			theApp.WriteProfileInt("Init", "FontWeight", lf.lfWeight);

			FontName = theApp.GetProfileString("Init", "FontName");
			FontSize = theApp.GetProfileInt("Init", "FontSize", 0);
			FontColor = theApp.GetProfileInt("Init", "FontColor", 0);
			FontWeight = theApp.GetProfileInt("Init", "FontWeight", 0);
		}
	}*/
	
	BOOL bRet;
	CHOOSEFONT cf;
	static LOGFONT lf;        // logical font structure
	static COLORREF rgbCurrent;  // current text color
	rgbCurrent = RGB(0, 0, 0);
	rgbColor = rgbCurrent;
	int nPointSize;
	// Initialize CHOOSEFONT
	ZeroMemory(&cf, sizeof(cf));
	cf.lStructSize = sizeof (cf);
	cf.hwndOwner = NULL;

	lf.lfHeight = -MulDiv(cf.iPointSize * 10, 96, 72);
	lf.lfWidth = -lf.lfHeight / 2;
	lf.lfCharSet = 134;
	lf.lfQuality = PROOF_QUALITY;

	cf.lpLogFont = &lf;
	cf.rgbColors = rgbCurrent;

	cf.Flags = CF_SCREENFONTS | CF_EFFECTS;
	bRet = ChooseFont(&cf);

	if (bRet)
	{
		rgbColor = cf.rgbColors;
		m_font.DeleteObject();//KIV
		m_font.CreateFontIndirect(&lf);
		nPointSize = cf.iPointSize;
		
		m_Edit.SetFont(&m_font, TRUE); 

		//m_LineNumber.SetFont(&m_font, TRUE);

		theApp.WriteProfileString("Init", "FontName", lf.lfFaceName);
		theApp.WriteProfileInt("Init", "FontSize", nPointSize); 
		theApp.WriteProfileInt("Init", "FontColor", (DWORD)rgbColor);
		theApp.WriteProfileInt("Init", "FontWeight", lf.lfWeight);

		FontName = theApp.GetProfileString("Init", "FontName");
		FontSize = theApp.GetProfileInt("Init", "FontSize", 0);
		FontColor = theApp.GetProfileInt("Init", "FontColor", 0);
		FontWeight = theApp.GetProfileInt("Init", "FontWeight", 0);
	}
}
void CPackingToolDlg::LoadFont(BYTE bFlag )
{
	if (bFlag == 1)//inc
	{
		b = b + 1;
		if (b > 24)
		{
			b = 24;
			return;
		}
	}


	if (bFlag == 2)//dec
	{
		b = b - 1;

		if (b < -10)
		{
			b = -10;
			return;
		}
	}
	
	if (bFlag == 0)//load
	{
		b = 0;
		m_fontLoaded = FALSE;
		m_fontLoadedKind = 0;
		if (AddFontResourceEx(FONTPATH1, FR_PRIVATE, 0) == false)
		{
			if (AddFontResourceEx(FONTPATH2, FR_PRIVATE, 0))
			{
				m_fontLoadedKind = 2;
				m_fontLoaded = TRUE;
			}
		}
		else
		{
			m_fontLoadedKind = 1;
			m_fontLoaded = TRUE;
		}		
	}

	if (m_fontLoaded || bFlag == 2 || bFlag == 1)
	{
		m_pFont = new CFont;
		int lfHeight, lfWidth;
		lfHeight = -MulDiv(FontSize / 10+b, 96, 72);
		lfWidth = -lfHeight / 2;
		m_pFont->DeleteObject();//KIV
		m_pFont->CreateFont(lfHeight, 
							lfWidth,
							0,
							0, 
							FontWeight,
							(BYTE)0,
							(BYTE)0,
							0, 
							(BYTE)134,
							OUT_DEFAULT_PRECIS, 
							CLIP_DEFAULT_PRECIS,
							PROOF_QUALITY, 
							DEFAULT_PITCH, 
							_T(FontName));
		m_Edit.SetFont(m_pFont);


		/*LOGFONT logFont;
		logFont.lfHeight = 10;

		logFont.lfWidth			= lfWidth; 
		logFont.lfEscapement	= 0; 
		logFont.lfOrientation	= 0; 
		logFont.lfWeight = FontWeight;
		logFont.lfItalic		= 0; 
		logFont.lfUnderline		= 0; 

		logFont.lfStrikeOut		= 0; 
		logFont.lfCharSet = (BYTE)134;
		logFont.lfOutPrecision	= OUT_DEFAULT_PRECIS; 
		logFont.lfClipPrecision	= CLIP_DEFAULT_PRECIS; 
		logFont.lfQuality = PROOF_QUALITY;
		logFont.lfPitchAndFamily	= DEFAULT_PITCH; 
		strcpy(logFont.lfFaceName, _T(FontName));

		CreateList(&logFont);*/
		//m_LineNumber.SetFont(m_pFont);
	}
}
char* L2C(LPCTSTR lpc)
{
	LPCTSTR pszA = lpc;
		CStringA sB(pszA);
	const char* pszC = sB;
	char* pszD = const_cast<char*>(pszC);
	return pszD;
}
std::vector<std::string> splitpath(
	const std::string& str
	, const std::set<char> delimiters)
{
	std::vector<std::string> result;

	char const* pch = str.c_str();
	char const* start = pch;
	for (; *pch; ++pch)
	{
		if (delimiters.find(*pch) != delimiters.end())
		{
			if (start != pch)
			{
				std::string str(start, pch);
				result.push_back(str);
			}
			else
			{
				result.push_back("");
			}
			start = pch + 1;
		}
	}
	result.push_back(start);

	return result;
}


void CPackingToolDlg::SetCommandLine(LPCTSTR aCommand)
{

	
	CCommandLine pCmd;
	CString strFlag = _T("");
	CString strParam = _T("");

	BOOL bRet = pCmd.GetFirstParameter(strFlag, strParam);
	while (bRet)
	{
		//DoSomethingWithTheParameter(strFlag, strParam);
		//CString str(strParam);
		CT2A ascii(strParam, CP_UTF8);
		//AfxMessageBox(RemoveBracket((char *)str.GetString()));
		OpenFile(RemoveBracket((char *)strParam.GetString()));
		bRet = pCmd.GetNextParameter(strFlag, strParam);
		UpdateData(FALSE);
	}
	//::ShowWindow(this->GetSafeHwnd(), SW_SHOWNORMAL);
	
	//T2A(aCommand);

	//std::set<char> delims{ '\\' };
	//std::vector<std::string> path = splitpath("C:\\MyDirectory\\MyFile.bat", delims);
	//path.back();


	//string filename = RemoveBracket(L2C(aCommand));		//this
	//char* filepart = PathFindFileName(filename.c_str());	//this
	//PathRemoveExtension(filepart);
	//AfxMessageBox(L2C(filepart));

	//OpenFileTwo(L2C(filepart));							//this

	//SetDlgItemText(IDC_EDIT_FINDSTR, aCommand);

	
	
}

BOOL CPackingToolDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	LoadFont();

	if ((m_szMinimum.cx == 0) && (m_szMinimum.cy == 0))
	{
		CRect rcWindow;
		GetWindowRect(rcWindow);
		m_szMinimum = rcWindow.Size();
	}

	CRect rcClient;
	GetClientRect(rcClient);
	m_szInitial = rcClient.Size();
	
	SetIcon(m_hIcon, TRUE);
	SetIcon(m_hIcon, FALSE);
	m_nFileNum = 0;
	m_nOldIndex = 0;
	m_sFileNum.Format("%d", m_nFileNum);
	m_BtnDelAll.EnableWindow(FALSE);
	m_BtnSaveAllToBin.EnableWindow(FALSE);
	m_BtnSaveAllToTxt.EnableWindow(FALSE);
	m_BtnSaveAsBin.EnableWindow(FALSE);
	m_BtnSave.EnableWindow(FALSE);
	m_Edit.EnableWindow(FALSE);
	m_Edit.SetWindowText(NULL);
	m_Edit.SetLimitText(1024 * 1024 * 100);		
	//m_Edit.SetTabStops(7);

	SMovingChildTest s;
	GetDlgItem(IDC_GROUP1, &s.m_hWnd);
	::GetWindowRect(s.m_hWnd, &s.m_rcInitial);

	stWidth = s.m_rcInitial.Width() + 8;
	stHeight = s.m_rcInitial.Height() + 8;	

	m_MovingChildrenTest.push_back(s);

	OnSizeJack(m_szInitial.cx, m_szInitial.cy);

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

	m_Edit.ShowScrollBar(SB_BOTH, 0);
	//m_LineNumber.ShowScrollBar(SB_BOTH, 0);

	GetButton(IDC_BUTTON_FINDSTR)->EnableWindow(FALSE);
	GetButton(IDC_EDIT_FINDSTR)->EnableWindow(FALSE);

	UpdateData(FALSE);
	this->DragAcceptFiles(TRUE);

	
	

	/*char cmd[MAX_PATH] = { 0, };
	int nCmdLen = strlen(AfxGetApp()->m_lpCmdLine);
	if (nCmdLen)
	{
		strcpy(cmd, AfxGetApp()->m_lpCmdLine + 1);
		cmd[nCmdLen - 2] = 0;
		OpenFile(cmd);
		UpdateData(FALSE);
	}*/
	BigenChar = 0;
	m_nWheelMsg = 0;
	m_ctrlFocused = FALSE;

	CCommandLine pCmd;
	CString strFlag = _T("");
	CString strParam = _T("");

	BOOL bRet = pCmd.GetFirstParameter(strFlag, strParam);
	while (bRet)
	{
		//DoSomethingWithTheParameter(strFlag, strParam);
		//CString str(strParam);
		CT2A ascii(strParam, CP_UTF8);
		//AfxMessageBox(RemoveBracket((char *)str.GetString()));
		OpenFile(RemoveBracket((char *)strParam.GetString()));
		bRet = pCmd.GetNextParameter(strFlag, strParam);
		UpdateData(FALSE);
	}
	return TRUE;
}
CButton*CPackingToolDlg::GetButton(int ID)
{
	CButton* pBtn = (CButton*)GetDlgItem(ID);
	return pBtn;
}
void CPackingToolDlg::OnDestroy()
{
	CDialog::OnDestroy();

	if (m_fontLoaded)
	{
		if (m_fontLoadedKind==1)
			RemoveFontResourceEx(FONTPATH1, FR_PRIVATE, 0);
		if (m_fontLoadedKind==2)
			RemoveFontResourceEx(FONTPATH2, FR_PRIVATE, 0);
	}
	
	DeleteObject(m_hBmpScrollBar);
}

void CPackingToolDlg::OnGetMinMaxInfo(MINMAXINFO FAR* lpMMI)
{
	CDialog::OnGetMinMaxInfo(lpMMI);
	if (lpMMI->ptMinTrackSize.x < m_szMinimum.cx)
		lpMMI->ptMinTrackSize.x = m_szMinimum.cx;
	if (lpMMI->ptMinTrackSize.y < m_szMinimum.cy)
		lpMMI->ptMinTrackSize.y = m_szMinimum.cy;
}
int countT = 0;
void CPackingToolDlg::OnSize(UINT nType, int cx, int cy)
{
	//
	CDialog::OnSize(nType, cx, cy);
	OnSizeJack(cx, cy);	

	theApp.WriteProfileInt("Init", "SizeX", cx);
	theApp.WriteProfileInt("Init", "SizeY", cy);


	/*CTList * pTList = NULL;

	CRect rect;
	GetClientRect(&rect);
	int Width = rect.right - rect.left;
	int Height = rect.bottom - rect.top;
	//if(pTConsole)
	{
		if(m_pTList)
		{
			
			m_pTList->SetSizetest(true);
			m_pTList->SetSize(cx, cy);
			m_pTList->Draw();
		}
				
	}*/

	//DlgMsgBox(m_Edit.GetLineCount());
}
void CPackingToolDlg::OnLButtonDown(UINT nFlags, CPoint point)

{
	// TODO: Add your message handler code here and/or call default
	CDialog::OnLButtonDown(nFlags, point);
	//m_Edit.SetSel(0, -1, FALSE);
}
void CPackingToolDlg::OnSizeJack(int cx, int cy)
{
	m_FileList.MoveWindow(3, stHeight, stWidth - 10, cy - stHeight);

	//m_LineNumber.MoveWindow(stWidth, 6, 100, cy - 6);

	m_Edit.MoveWindow(stWidth, 6, cx - stWidth, cy - 6);

	//m_LineNumber.ShowScrollBar(SB_BOTH, 0);

	


	

	
}
void CPackingToolDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	CDialog::OnSysCommand(nID, lParam);
}
void CPackingToolDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM)dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);

		char c[MAX_PATH] = { 0, };

		sprintf(c, "%d", m_Edit.GetLineCount());
		TextOut(dc, x, y, c, strlen(c));
	}
	else
	{
		CDialog::OnPaint();
	}
}
HCURSOR CPackingToolDlg::OnQueryDragIcon()
{
	return (HCURSOR)m_hIcon;
}
void CPackingToolDlg::OnButtonOpen()
{
}
void CPackingToolDlg::OnButtonSaveToBin()
{ 
	if (m_ExtDlg.DoModal() == IDOK)
	{
		m_FileMng.SetExt(m_ExtDlg.m_sExt);
		SaveData(m_FileList.GetCurSel());
		m_FileMng.SaveToBin();
	}
}
void CPackingToolDlg::TestMsgFromAnotherFile()
{
	MessageBox("TestMsgFromAnotherFile!", "info", MB_OK);
}
void CPackingToolDlg::OnButtonOpenBin()
{
	CFileDialog dlg(TRUE, NULL, "*.*", OFN_ALLOWMULTISELECT | OFN_HIDEREADONLY,
		"Binary|*.bin; *.beff; *.befl; *.bmhm; *.bsad; *.md||", NULL);
	char buff[65535] = "";
	dlg.m_ofn.lpstrFile = buff;
	dlg.m_ofn.nMaxFile = 65535;
	CString fullfilename, filename;

	if (dlg.DoModal() == IDOK)
	{
		for (POSITION ps = dlg.GetStartPosition(); ps != NULL;)
		{
			fullfilename = dlg.GetNextPathName(ps);

			char ext[MAX_PATH] = { 0, };
			int len = strlen(fullfilename);
			int i, j;
			for (i = len - 1; i >= 0; --i)
			{
				if (fullfilename[i] == '.')
				{
					j = i;
					break;
				}
			}
			for (i = 0; i < len - j; ++i)
				ext[i] = fullfilename[j + 1 + i];
			strupr(ext);
			if (strcmp(ext, "BIN") == 0 ||
				strcmp(ext, "BEFF") == 0 ||
				strcmp(ext, "BEFL") == 0 ||
				strcmp(ext, "BMHM") == 0 ||
				strcmp(ext, "BSAD") == 0 ||
				strcmp(ext, "MAP") == 0 ||
				strcmp(ext, "TEMPBIN") == 0 ||
				strcmp(ext, "MD") == 0)
			{
				if (m_FileMng.AddFile(fullfilename, "rb"))
				{
					m_FileList.AddString(m_FileMng.GetFileName(m_nFileNum));

					//m_LineNumber.AddString("1");
					++m_nFileNum;
				}
			}
			
		}
	}

	if (m_nFileNum)
	{
		m_sFileNum.Format("%d", m_nFileNum);
		m_BtnDelAll.EnableWindow(TRUE);
		m_BtnSaveAllToBin.EnableWindow(TRUE);
		m_BtnSaveAllToTxt.EnableWindow(TRUE);
		m_BtnSaveAsBin.EnableWindow(TRUE);
		m_BtnSave.EnableWindow(TRUE);
		//		m_BtnOpen.EnableWindow( FALSE );
		// list
		m_FileList.SetCurSel(m_nFileNum - 1);
		// edit
		m_Edit.EnableWindow(TRUE);
		CMHFileEx* pFile = m_FileMng.GetFile(m_nFileNum - 1);
		m_Edit.SetWindowText(pFile->GetData());
		//AfxMessageBox(pFile->GetData());
		m_sCurFile = pFile->GetFullFileName();

		m_nOldIndex = m_nFileNum - 1;
	}
	UpdateList();
	UpdateData(FALSE);
}
void CPackingToolDlg::OnButtonSaveToTxt()
{
	if (m_SaveAsDlg.DoModal() == IDOK)
	{
		m_FileMng.SetExt(m_SaveAsDlg.m_sExt);
		SaveData(m_FileList.GetCurSel());
		m_FileMng.SaveToTxt();
	}
}
void CPackingToolDlg::OnButtonDelall()
{
	m_FileMng.Clear();
	m_nFileNum = 0;
	m_sFileNum.Format("%d", m_nFileNum);
	m_FileList.ResetContent();

	//m_LineNumber.ResetContent();

	m_BtnDelAll.EnableWindow(FALSE);
	m_BtnSaveAllToBin.EnableWindow(FALSE);
	m_BtnSaveAllToTxt.EnableWindow(FALSE);
	m_BtnSaveAsBin.EnableWindow(FALSE);
	m_BtnSave.EnableWindow(FALSE);
	m_Edit.SetWindowText(NULL);
	m_Edit.EnableWindow(FALSE);
	m_Edit.ShowScrollBar(SB_BOTH, 0);
	m_sCurFile = "";
	UpdateData(FALSE);
	UpdateDialogTitle("new 1");

	GetButton(IDC_BUTTON_FINDSTR)->EnableWindow(FALSE);
	GetButton(IDC_EDIT_FINDSTR)->EnableWindow(FALSE);
}
void CPackingToolDlg::OnSelchangeListFile()
{
	SaveData(m_nOldIndex);
	int index = m_FileList.GetCurSel();
	if (m_nOldIndex == index) return;

	m_sCurFile = m_FileMng.GetFullFileName(index);
	CMHFileEx* pFile = m_FileMng.GetFile(index);
	m_Edit.SetWindowText(pFile->GetData());
	m_Edit.ShowScrollBar(SB_BOTH, 1);
	m_nOldIndex = index;
	UpdateDialogTitle(m_sCurFile);
	UpdateData(FALSE);

	GetButton(IDC_BUTTON_FINDSTR)->EnableWindow(TRUE);
	GetButton(IDC_EDIT_FINDSTR)->EnableWindow(TRUE);

	//AddLineNumber();
}
void CPackingToolDlg::AddLineNumber()
{
	//m_LineNumber.ResetContent();

	for (int i = 1; i < m_Edit.GetLineCount()+1; i++)
	{
		/*char btemp[64];
			sprintf(btemp, "%d test", i);
			m_pTList->AddString(btemp);*/
	//	m_LineNumber.AddString(IntToStr(i));
		
	}
	//m_LineNumber.SetWindowText(IntToStr(m_Edit.GetLineCount()));
	
}
void CPackingToolDlg::OnButtonNew()
{
	CFileDialog dlg(FALSE, NULL, "*.bin", OFN_FILEMUSTEXIST | OFN_HIDEREADONLY, "", NULL);
	if (dlg.DoModal() == IDOK)
	{
		CString lpszPathName = dlg.GetPathName();
		m_FileMng.New(lpszPathName);
		m_FileList.AddString(m_FileMng.GetFileName(m_nFileNum));
		m_sCurFile = lpszPathName;
		SaveData(m_nOldIndex);
		++m_nFileNum;
	}
	if (m_nFileNum)
	{
		m_sFileNum.Format("%d", m_nFileNum);
		m_BtnDelAll.EnableWindow(TRUE);
		m_BtnSaveAllToBin.EnableWindow(TRUE);
		m_BtnSaveAllToTxt.EnableWindow(TRUE);
		m_BtnSaveAsBin.EnableWindow(TRUE);
		m_BtnSave.EnableWindow(TRUE);
		m_FileList.SetCurSel(m_nFileNum - 1);
		m_Edit.EnableWindow(TRUE);
		m_Edit.SetWindowText(NULL);
		m_Edit.ShowScrollBar(SB_BOTH, 1);
		m_nOldIndex = m_nFileNum - 1;

		GetButton(IDC_BUTTON_FINDSTR)->EnableWindow(TRUE);
		GetButton(IDC_EDIT_FINDSTR)->EnableWindow(TRUE);
	}
	UpdateData(FALSE);
}
void CPackingToolDlg::SaveData(int index)
{
	char* str = new char[MEGA];
	memset(str, 0, MEGA);
	m_Edit.GetWindowText(str, MEGA);
	int nLen = lstrlen(str);
	for (int i = nLen - 1; i >= 0; i--)
	{
		if (str[i] == 0x20 ||
			str[i] == 0x09)
		{
			str[i] = NULL;
		}
		else if (str[i - 1] == 0x0d && str[i] == 0x0a)
		{
			str[i - 1] = NULL;
			str[i] = NULL;
			i--;
		}
		else
		{
			break;
		}
	}
	m_FileMng.SetFileData(index, str);
	delete[] str;
}
void CPackingToolDlg::OnButtonSaveAsBin()
{
	CFileDialog dlg(FALSE, NULL, "*.*", OFN_FILEMUSTEXIST | OFN_HIDEREADONLY, "", NULL);
	if (dlg.DoModal() == IDOK)
	{
		CString lpszPathName = dlg.GetPathName();
		int index = m_FileList.GetCurSel();
		m_FileMng.SaveAsBin(index, lpszPathName);
	}
}
void CPackingToolDlg::OnButtonSaveBin()
{
	if (m_nFileNum == NULL)
	{
		return;
	}
	SaveData(m_FileList.GetCurSel());
	m_FileMng.SaveToBin();
	UpdateDialogTitle(m_sCurFile);

	MessageBox("Saved!", "info", MB_OK);
}

void CPackingToolDlg::DlgMsgBox(CString c)
{
	MessageBox("string", c, MB_OK);
}
void CPackingToolDlg::DlgMsgBox(BYTE b)
{
	char c[MAX_PATH] = { 0, };
	sprintf(c, "%d", b);
	MessageBox("byte", c, MB_OK);
}

void CPackingToolDlg::OnDropFiles(HDROP hDropInfo)
{
	char filename[MAX_PATH] = { 0, };
	UINT count = ::DragQueryFile(hDropInfo, 0xFFFFFFFF, filename, MAX_PATH);
	if (count)
	{
		for (UINT ii = 0; ii < count; ++ii)
		{
			DragQueryFile(hDropInfo, ii, filename, MAX_PATH);
			OpenFile(filename);
		}
	}
	AddLineNumber();
	UpdateData(FALSE);
	UpdateDialogTitle(filename);
	CDialog::OnDropFiles(hDropInfo);
}
void CPackingToolDlg::UpdateDialogTitle(CString Text)
{
	SetWindowText(Text);
}
void CPackingToolDlg::OpenFile(char* pFileName)
{
	char ext[MAX_PATH] = { 0, };
	int len = strlen(pFileName);
	int i, j;
	for (i = len - 1; i >= 0; --i)
	{
		if (pFileName[i] == '.')
		{
			j = i;
			break;
		}
	}
	for (i = 0; i < len - j; ++i)
		ext[i] = pFileName[j + 1 + i];
	strupr(ext);
	if (strcmp(ext, "BIN") == 0 ||
		strcmp(ext, "BEFF") == 0 ||
		strcmp(ext, "BEFL") == 0 ||
		strcmp(ext, "BMHM") == 0 ||
		strcmp(ext, "BSAD") == 0 ||
		strcmp(ext, "MAP") == 0 ||
		strcmp(ext, "TEMPBIN") == 0 ||
		strcmp(ext, "MD") == 0)
	{
		
		if (m_FileMng.AddFile(pFileName, "rb"))
		{
			/*if (*nextInstacesStr)
			{
				AfxMessageBox("test");
			}*/
			/*if (strcmp(nextInstacesStr, "") != 0)
			{
				MessageBox("", "", MB_OK);
				//TT
			}*/
			m_FileList.AddString(m_FileMng.GetFileName(m_nFileNum));

			

			++m_nFileNum;

			//m_LineNumber.AddString(IntToStr(m_nFileNum));
		}
	}
	else if (strcmp(ext, "MAP") == 0 ||strcmp(ext, "TXT") == 0 || strcmp(ext, "TEMPBIN1") == 0)
	{
		if (m_FileMng.AddFile(pFileName, "rt"))
		{
			m_FileList.AddString(m_FileMng.GetFileName(m_nFileNum));

			

			++m_nFileNum;

			//m_LineNumber.AddString(IntToStr(m_nFileNum));
		}
	}
	else
	{
		MessageBox("Cannot open this file", pFileName, MB_OK);
		//--m_nFileNum;
		return;
		//DlgMsgBox("failed");
	}
	UpdateList();
}
void CPackingToolDlg::OpenFileTwo(char* pFileName)
{
	char ext[MAX_PATH] = { 0, };
	int len = strlen(pFileName);
	int i, j;
	for (i = len - 1; i >= 0; --i)
	{
		if (pFileName[i] == '.')
		{
			j = i;
			break;
		}
	}
	for (i = 0; i < len - j; ++i)
		ext[i] = pFileName[j + 1 + i];
	strupr(ext);
	if (strcmp(ext, "BIN") == 0 ||
		strcmp(ext, "BEFF") == 0 ||
		strcmp(ext, "BEFL") == 0 ||
		strcmp(ext, "BMHM") == 0 ||
		strcmp(ext, "BSAD") == 0 ||
		strcmp(ext, "MAP") == 0 ||
		strcmp(ext, "TEMPBIN") == 0 ||
		strcmp(ext, "MD") == 0)
	{

		if (m_FileMng.AddFile(pFileName, "rb"))
		{
			/*if (*nextInstacesStr)
			{
			AfxMessageBox("test");
			}*/
			/*if (strcmp(nextInstacesStr, "") != 0)
			{
			MessageBox("", "", MB_OK);
			//TT
			}*/
			m_FileList.AddString(pFileName);



			++m_nFileNum;

			//m_LineNumber.AddString(IntToStr(m_nFileNum));
		}
	}
	else if (strcmp(ext, "MAP") == 0 || strcmp(ext, "TXT") == 0 || strcmp(ext, "TEMPBIN1") == 0)
	{
		if (m_FileMng.AddFile(pFileName, "rt"))
		{
			m_FileList.AddString(pFileName);



			++m_nFileNum;

			//m_LineNumber.AddString(IntToStr(m_nFileNum));
		}
	}
	else
	{
		MessageBox("Cannot open this file", pFileName, MB_OK);
		//--m_nFileNum;
		return;
		//DlgMsgBox("failed");
	}
	UpdateList();
}
CString CPackingToolDlg::IntToStr(int i)
{
	char c[MAX_PATH] = { 0, };
	sprintf(c, "%d", i);
	return c;
}
void CPackingToolDlg::UpdateList()
{
	if (m_nFileNum)
	{
		m_sFileNum.Format("%d", m_nFileNum);

		m_BtnDelAll.EnableWindow(TRUE);
		m_BtnSaveAllToBin.EnableWindow(TRUE);
		m_BtnSaveAllToTxt.EnableWindow(TRUE);
		m_BtnSaveAsBin.EnableWindow(TRUE);
		m_BtnSave.EnableWindow(TRUE);

		m_FileList.SetCurSel(m_nFileNum - 1);

		

		CMHFileEx* pFile = m_FileMng.GetFile(m_nFileNum - 1);

		m_Edit.EnableWindow(TRUE);
		m_Edit.ShowScrollBar(SB_BOTH, 1);
		m_Edit.SetWindowText(pFile->GetData());

		m_sCurFile = pFile->GetFullFileName();
		m_nOldIndex = m_nFileNum - 1;

		GetButton(IDC_BUTTON_FINDSTR)->EnableWindow(TRUE);
		GetButton(IDC_EDIT_FINDSTR)->EnableWindow(TRUE);
	}
}
WORD CPackingToolDlg::GetOperationSystemType()
{
	return 0;
}
void CPackingToolDlg::OnSearch()
{
	int a = 0;
}
void CPackingToolDlg::OnButtonSavedsof()
{
}
void CPackingToolDlg::OnBnClickedButton1()
{
	CString  Big5Buffer, str;
	m_Edit.GetWindowText(Big5Buffer);
	OnOK();
}
string CPackingToolDlg::GetCurrentWindowTitle()
{
	char wnd_Title[256];
	GetWindowText(wnd_Title, 256);
	return wnd_Title;
}
void CPackingToolDlg::OnBnClickedButtonFindstr()
{
	CString strFind, str;
	DWORD end;
	m_Edit.GetWindowText(strFind);
	m_FindStr.GetWindowText(str);
	BigenChar = strFind.Find(str, BigenChar);
	end = str.GetLength();
	m_Edit.SetSel(BigenChar, BigenChar + end, 0);
	m_Edit.SetFocus();
	BigenChar += end;
}
void CPackingToolDlg::OnEnChangeEditFindstr()
{
	BigenChar = 0;
}
void CPackingToolDlg::OnEnChangeEdit()
{
	char Temp[256] = { 0, };
	int index = m_FileList.GetCurSel();
	wsprintf(Temp, "*%s", m_FileMng.GetFullFileName(index));
	UpdateDialogTitle(Temp);
}
BOOL CPackingToolDlg::PreTranslateMessage(MSG* pMsg)
{
	switch (pMsg->message)
	{
	case WM_KEYDOWN:
		{
			if (pMsg->wParam == VK_CONTROL)
			{
				HWND hWnd = (HWND)pMsg->hwnd;
				while (hWnd)
				{
					m_ctrlFocused = TRUE;
				/*if (m_mapHookedWindows.Lookup(hWnd, pHook))
				{
				SendMessage(pHook->m_hNotify ? pHook->m_hNotify : pHook->m_hParent, UM_HIDEEDITOR, pMsg->wParam == VK_RETURN, 0);
				break;
				}*/
					hWnd = ::GetParent(hWnd);
				}
			}
			if (pMsg->wParam == VK_ESCAPE)
			{
				HWND hWnd = (HWND)pMsg->hwnd;
				if (MessageBox( "Are you sure to quit ?", "info", MB_YESNO) == IDNO)
				{//no					
					break;
				}
				else
				{//yes
					//MessageBox("2 ", "2", MB_OK);
					exit(0);
					break;
				}
			}	
			if (pMsg->wParam == VK_F5)
			{
				OnButtonSaveBin();
			}
		}
		break;
	case WM_KILLFOCUS:		
	case WM_KEYUP:
		if (pMsg->wParam == VK_CONTROL)
		{
		//	MessageBox("r ", "r", MB_OK);
			m_ctrlFocused = FALSE;
		}
		
		break;
	case WM_MOUSEWHEEL:
		{
			m_nWheelMsg = int(pMsg->wParam) >> 16;

			float Wheel = static_cast<float>(m_nWheelMsg);
			if (m_ctrlFocused)
			if (Wheel)
			if (Wheel > 0)
			{
				//MessageBox("1 ", "1", MB_OK);
				//GG();
				LoadFont(1);
				return 1;
			}
			else
			{
				//BB();
				LoadFont(2);
				return 1;
			}
			//MessageBox("2 ", "2", MB_OK); 
			m_ctrlFocused = FALSE;
		}
		break;
	case WM_DESTROY:
		{
			// delete m_pFont;
			m_ctrlFocused = FALSE;
		}
		break;
	/*case WM_HSCROLL:
		{
					   m_Edit.GetScrollInfo();
		break;
		}*/
	}
	
	/*if (pMsg->message == WM_KEYDOWN&&pMsg->wParam == VK_ESCAPE)
	{
		return CDialog::PreTranslateMessage(pMsg);
	}
	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN)
	{
		{
		}
		return CDialog::PreTranslateMessage(pMsg);
	}*/
	//return CDialog::PreTranslateMessage(pMsg);
	return 0;
}
void CPackingToolDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
/*	if (pScrollBar->m_hWnd == m_ctrlScroll.m_hWnd)
	{
		SCROLLINFO si;
		si.fMask = SIF_ALL;
		m_ctrlScroll.GetScrollInfo(&si, SIF_ALL);
		switch (nSBCode)
		{
		case SB_LINEUP:
			si.nPos--;
			break;
		case SB_LINEDOWN:
			si.nPos++;
			break;
		case SB_PAGEUP:
			si.nPos -= si.nPage;
			break;
		case SB_PAGEDOWN:
			si.nPos += si.nPage;
			break;
		}
		if (si.nPos>(int)(si.nMax - si.nMin - si.nPage + 1)) si.nPos = si.nMax - si.nMin - si.nPage + 1;
		if (si.nPos<si.nMin) si.nPos = si.nMin;
		m_ctrlScroll.SetScrollInfo(&si);
	}*/
}

void CPackingToolDlg::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
/*	if (pScrollBar->m_hWnd == m_ctrlScroll.m_hWnd)
	{
		SCROLLINFO si;
		si.fMask = SIF_ALL;
		m_ctrlScroll.GetScrollInfo(&si, SIF_ALL);
		switch (nSBCode)
		{
		case SB_LINEUP:
			si.nPos--;
			break;
		case SB_LINEDOWN:
			si.nPos++;
			break;
		case SB_PAGEUP:
			si.nPos -= si.nPage;
			break;
		case SB_PAGEDOWN:
			si.nPos += si.nPage;
			break;
		case SB_THUMBTRACK:
			si.nPos = nPos;
			break;
		}
		if (si.nPos>(int)(si.nMax - si.nMin - si.nPage + 1)) si.nPos = si.nMax - si.nMin - si.nPage + 1;
		if (si.nPos<si.nMin) si.nPos = si.nMin;
		si.fMask = SIF_POS;
		m_ctrlScroll.SetScrollInfo(&si);
		TRACE("\nPos=%d", si.nPos);
	}*/

}
void CPackingToolDlg::OnClose() // message handler for WM_CLOSE
{
	// Save main window position
	CWinApp* app = AfxGetApp();
	WINDOWPLACEMENT wp;
	GetWindowPlacement(&wp);
	app->WriteProfileInt("Frame", "Status", wp.showCmd);
	app->WriteProfileInt("Frame", "Top", wp.rcNormalPosition.top);
	app->WriteProfileInt("Frame", "Left", wp.rcNormalPosition.left);
	app->WriteProfileInt("Frame", "Bottom", wp.rcNormalPosition.bottom);
	app->WriteProfileInt("Frame", "Right", wp.rcNormalPosition.right);
	//MessageBox("2 ", "2", MB_OK);
	//theApp.WriteProfileInt("Init", "FontSize", FontSize*10 +b);
	CDialog::OnClose();
}

HBRUSH CPackingToolDlg::OnCtlColor(CDC* pDC, CWnd *pWnd, UINT nCtlColor)
{
	HBRUSH hbr = (HBRUSH)GetStockObject(DKGRAY_BRUSH);
	if (pWnd->GetDlgCtrlID() == IDC_EDIT )
	{
		pDC->SetBkColor(RGB(30, 30, 30));
		pDC->SetTextColor(FontColor);
	}
	if (pWnd->GetDlgCtrlID() == IDC_EDIT_FINDSTR || nCtlColor == CTLCOLOR_STATIC)
	{
		pDC->SetBkColor(RGB(30, 30, 30));
		pDC->SetTextColor(RGB(241, 241, 241));		
	}
	if (pWnd->GetDlgCtrlID() == IDC_LIST_FILE || pWnd->GetDlgCtrlID() == IDC_STATIC)
	{
		pDC->SetBkColor(RGB(64, 64, 64));
		pDC->SetTextColor(RGB(241, 241, 241));
	}
	return hbr;
}
void CPackingToolDlg::OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	CDialog::OnDrawItem(nIDCtl, lpDrawItemStruct);
	if (nIDCtl == IDC_BUTTON_SAVE_BIN 
		|| nIDCtl == IDC_BUTTON_SAVE_TO_TXT 
		|| nIDCtl == IDC_BUTTON_SAVE_TO_BIN 
		|| nIDCtl == IDC_BUTTON1 
		|| nIDCtl == IDC_BUTTON_FINDSTR 
		|| nIDCtl == IDC_BUTTON_DELALL
		|| nIDCtl == IDC_SELECT_FONT 
		|| nIDCtl == IDC_BUTTON_OPEN_BIN
		/*|| nIDCtl == IDC_BUTTON_FONT_PICKER*/)
	{
		DWORD bkColor = RGB(45, 45, 48);
		DWORD txtColor = RGB(241, 241, 241);
		CDC dc;
		dc.Attach(lpDrawItemStruct->hDC);
		RECT rect;
		rect = lpDrawItemStruct->rcItem;
		dc.Draw3dRect(&rect, RGB(30, 30, 30), RGB(0, 0, 0));
		dc.FillSolidRect(&rect, bkColor);
		UINT state = lpDrawItemStruct->itemState;
		if ((state & ODS_SELECTED))
		{
			dc.DrawEdge(&rect, EDGE_SUNKEN, BF_RECT);
		}
		else
		{
			dc.DrawEdge(&rect, EDGE_RAISED, BF_RECT);
		}
		dc.SetBkColor(bkColor);
		dc.SetTextColor(txtColor);
		TCHAR buffer[MAX_PATH];
		ZeroMemory(buffer, MAX_PATH);
		::GetWindowText(lpDrawItemStruct->hwndItem, buffer, MAX_PATH);
		dc.DrawText(buffer, &rect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
		dc.Detach();
	}
}
void CPackingToolDlg::AutoMove(int iID, double dXMovePct, double dYMovePct, double dXSizePct, double dYSizePct)
{
	SMovingChild s;
	GetDlgItem(iID, &s.m_hWnd);
	s.m_dXMoveFrac = dXMovePct / 100.0;
	s.m_dYMoveFrac = dYMovePct / 100.0;
	s.m_dXSizeFrac = dXSizePct / 100.0;
	s.m_dYSizeFrac = dYSizePct / 100.0;
	::GetWindowRect(s.m_hWnd, &s.m_rcInitial);
	ScreenToClient(s.m_rcInitial);
	m_MovingChildren.push_back(s);
}