#include "stdafx.h"
#include "main.h"
#include "StylerDlg.h"
#include "afxdialogex.h"
//#include "DarkMode.h"
#include "dialog.h"
HWND				mCurrentStylerHwnd;
extern HWND			m_hwndEditor;
extern HWND			m_DialogHwnd;
/*********temporarily hold this value stored in memory*******/
//extern DWORD bSelFontSize[];
unsigned int bTempSelection[3];
COLORREF m_TempclrBacktemp[16];
COLORREF m_TempclrTexttemp[16];
#define GET_BACK_COLOR_TEMP(c) m_TempclrBacktemp[c]
#define GET_FORE_COLOR_TEMP(c) m_TempclrTexttemp[c]
/********************overide ShowWindow()*********************/
void CStylerDlg::ShowWindow(int nCmdShow)
{	
	CDialog::ShowWindow(nCmdShow);
	if (nCmdShow == SW_SHOW || nCmdShow == SW_SHOWNORMAL)
	{
		theApp.SetStylerEnable(TRUE);
	}
	else
	{
		theApp.SetStylerEnable(FALSE);
	}
}

/*************modeless dialog*****************/
void CStylerDlg::PostNcDestroy()
{
	CDialog::PostNcDestroy();
	if (m_pParent)
	{
		((theDlg*)m_pParent)->m_StylerDlg = NULL;
	}
	delete this;
}
void CStylerDlg::OnCancel()
{
	::SendMessage(m_DialogHwnd, WM_REFRESHBUTTONSTATES, FALSE, 0);
	DestroyWindow();
}
void CStylerDlg::OnOK()
{
	if (UpdateData(true))
	{
		((theDlg*)m_pParent)->UpdateData(false);
		DestroyWindow();
	}
}
void CStylerDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	OnCBTransparent();
	OnCBBorderWidth();
	CDialog::OnHScroll(nSBCode, nPos, pScrollBar);
}
HBRUSH CStylerDlg::OnCtlColor(CDC* pDC, CWnd *pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	//HBRUSH hbr = nullptr;
	if (theApp.GetGameAI()->sPage1.DarkMode)
	{
		hbr = darkHBR;
		DWORD CtrlID = pWnd->GetDlgCtrlID();
		if (CtrlID == IDJ_STYLEDESCRIPTION_STATIC)
		{
			pDC->SetTextColor(RGB(255, 255, 0));
		}
		else if (CtrlID == IDJ_STYLES_LIST)
		{
			pDC->SetBkColor(RGB(164, 164, 164));
			pDC->SetTextColor(RGB(255, 255, 255));
			hbr = (HBRUSH)GetStockObject(DKGRAY_BRUSH);
		}
		else
		{
			pDC->SetTextColor(RGB(241, 241, 241));
		}
		pDC->SetBkMode(TRANSPARENT);
	}
	return hbr;
}



IMPLEMENT_DYNAMIC(CStylerDlg, CDialog)
CStylerDlg::CStylerDlg(CWnd* pParent) : CDialog(CStylerDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_pParent = pParent;

	memset(m_TempclrBacktemp, 0, sizeof(COLORREF)*16);
}
void CStylerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	//DDX_Control(pDX, IDC_BUTTON_DEFAULT, m_TestDefaultBtn);

	DDX_Control(pDX, IDJ_STYLEDESCRIPTION_STATIC, m_styleDescStatic);
	DDX_FontPreviewCombo(pDX, IDJ_FONT_COMBO, m_faceName);

	DDX_Control(pDX, IDJ_SC_PERCENTAGE_SLIDER, m_slider);

	DDX_Control(pDX, IDC_BORDERWIDTH_SLIDER, m_sliderBorderWidth);

	DDX_Text(pDX, IDC_BORDERWIDTHVAL_STATIC, m_sliderBorderWidthValue);

	DDX_Control(pDX, IDC_FR_BACKGROUND1, m_staticBtnColor[BackColor]);
	DDX_Control(pDX, IDC_FR_TEXTGROUND1, m_staticBtnColor[TextColor]);
}
BEGIN_MESSAGE_MAP(CStylerDlg, CDialog)
	ON_WM_NCDESTROY()
	ON_WM_CTLCOLOR()
	ON_CBN_SELCHANGE(IDJ_FONT_COMBO, &CStylerDlg::OnDropDownFontSelchange)
	ON_CBN_SELCHANGE(IDJ_FONTSIZE_COMBO, &CStylerDlg::OnDropDownFontSizeSelchange)
	ON_BN_CLICKED(IDJ_SAVECLOSE_BUTTON, &CStylerDlg::OnBnClickedSavecloseButton)
	ON_LBN_SELCHANGE(IDJ_STYLES_LIST, &CStylerDlg::OnSelchangeStyleListBox)

	ON_BN_CLICKED(IDJ_SC_TRANSPARENT_CHECK, &CStylerDlg::OnCBTransparent)

	ON_BN_CLICKED(IDC_BUTTON_DEFAULT, &CStylerDlg::OnButtonDefault)
	ON_BN_CLICKED(IDC_CHECK_NOEDGE, &CStylerDlg::OnCBNoBorder)

	ON_WM_HSCROLL()

	ON_BN_CLICKED(IDJ_CB_ENABLE_EOL_FLAG, enableEOLflagCB)
	ON_BN_CLICKED(IDJ_LINE_NUMBER_CHECK, enableLineNumberCB)
	ON_BN_CLICKED(IDJ_CB_COLOR_SYNTAX, enableLexerForeStyleCB)
	ON_BN_CLICKED(IDJ_GLOBAL_BG_CHECK, enableLexerBackStyleCB)

	ON_BN_CLICKED(IDC_FR_BACKGROUND1, onSelectBackColor)
	ON_BN_CLICKED(IDC_FR_TEXTGROUND1, onSelectTextColor)

	ON_WM_SETTINGCHANGE()

END_MESSAGE_MAP()
/**************************Dialog initializer**********************************/
BOOL CStylerDlg::OnInitDialog()
{
	//if (m_ScinCtrlStyler.m_hWnd == NULL)
	//	m_ScinCtrlStyler.m_hWnd = m_hwndEditor;
	if (mCurrentStylerHwnd==NULL)
		mCurrentStylerHwnd = m_hWnd;
	//SetWindowTheme(m_hWnd, L"Explorer", nullptr);
	//AllowDarkModeForWindow(m_hWnd, true);
	//SetTitleToDark(m_hWnd);
	SetTitleBarThemeColor(m_hWnd, theApp.GetGameAI()->sPage1.DarkMode);
	
	darkHBR = CreateSolidBrush(RGB(34, 34, 34));



	//auto resAccel = MAKEINTRESOURCE(IDR_QUICKFIND_ACCEL);
	//auto hInstance = AfxFindResourceHandle(resAccel, RT_ACCELERATOR);
	/*m_hAccel = LoadAccelerators(hInstance, resAccel);
	ASSERT(m_hAccel != NULL);
	if (m_hAccel)
	{
		m_nAccelSize = ::CopyAcceleratorTable(m_hAccel, NULL, 0);

		m_pAccelTable = new ACCEL[m_nAccelSize];
		ENSURE(m_pAccelTable != NULL);

		::CopyAcceleratorTable(m_hAccel, m_pAccelTable, m_nAccelSize);
	}*/
	//VERIFY(InitButton(m_TestDefaultBtn, IDC_BUTTON_DEFAULT, hInstance));

	InitButton();
	InitCheckBox();
	InitAllDarkButton();
	InitFontComboBox();
	InitFontSizeComboBox();
	InitStyleListBox();
	InitColorButton();
	InitGroupBox();
	InitStaticControl();
	InitSliderControl();

	SetSelectThemeEnable(FALSE);

	return TRUE;
}
BOOL CStylerDlg::InitButton(CMFCButton& btn, UINT nID, HINSTANCE hResInst, int nImgSize) const
{
	auto resName = MAKEINTRESOURCE(nID);
	if (!hResInst)
	{
		hResInst = AfxFindResourceHandle(resName, RT_GROUP_ICON);
		if (!hResInst)
		{
			return FALSE;
		}
	}
	btn.m_nFlatStyle = CMFCButton::BUTTONSTYLE_FLAT;
	//btn.m_bTransparent = TRUE;
	btn.SetWindowText(_T(""));

	auto hIcon = (HICON)LoadImage(hResInst, resName, IMAGE_ICON, nImgSize, nImgSize, LR_SHARED);
	btn.SetImage(hIcon);

	CString strTooltip;
	strTooltip.LoadString(nID);

	CString strAccel;

	BOOL bHasAccel = FindAccelerator(nID, strAccel);
	if (bHasAccel)
	{
		strTooltip += _T(" (");
		strTooltip += strAccel;
		strTooltip += _T(')');
	}

	btn.SetTooltip(strTooltip);
	return TRUE;
}
BOOL CStylerDlg::FindAccelerator(UINT uiCmd, CString& str) const
{
	if (!m_pAccelTable)
		return FALSE;
	BOOL bFound = FALSE;
	for (int i = 0; i < m_nAccelSize; i++)
	{
		if (m_pAccelTable[i].cmd == uiCmd)
		{
			bFound = TRUE;
			 
			CMFCAcceleratorKey helper(&m_pAccelTable[i]);

			CString strKey;
			helper.Format(strKey);

			if (!str.IsEmpty())
			{
				str += _T("; ");
			}

			str += strKey;

			break;
		}
	}
	return bFound;
}
void CStylerDlg::InitSliderControl()
{
	m_slider.SetRangeMin(50, false);
	m_slider.SetRangeMax(200, false);
	
	CString str;
	str.Format("%d",  m_slider.GetRangeMin());
	m_sliderValue = str;
	m_slider.ShowWindow(SW_SHOW);

	GetDlgItem(IDJ_SC_TRANSPARENT_CHECK)->ShowWindow(SW_SHOW);

	m_sliderBorderWidth.SetRangeMin(0, false);
	m_sliderBorderWidth.SetRangeMax(30, false);
	m_sliderBorderWidth.ShowWindow(SW_SHOW);
	GetDlgItem(IDC_BORDERWIDTH_SLIDER)->ShowWindow(SW_SHOW);

	UpdateData();
	int value = static_cast<int32_t>(::SendDlgItemMessage(GetSafeHwnd(), IDC_BORDERWIDTH_SLIDER, TBM_SETPOS,-1, theApp.GetGameAI()->sPage1.borderWidth));
	int percent = static_cast<int32_t>(::SendDlgItemMessage(GetSafeHwnd(), IDC_BORDERWIDTH_SLIDER, TBM_GETPOS, 0, 0));
	m_sliderBorderWidthValue.Format("%d", percent);

	value = percent = 0;

	value = static_cast<int32_t>(::SendDlgItemMessage(GetSafeHwnd(), IDJ_SC_PERCENTAGE_SLIDER, TBM_SETPOS, -1, theApp.GetGameAI()->sPage1.m_styleAlphaPercent));
	percent = static_cast<int32_t>(::SendDlgItemMessage(GetSafeHwnd(), IDJ_SC_PERCENTAGE_SLIDER, TBM_GETPOS, 0, 0));
	m_sliderAlphaPercent.Format("%d", percent);
	UpdateData(FALSE);
}
void CStylerDlg::InitStaticControl()
{
	m_styleDescStatic.SetTextColor(RGB(10, 10, 10));
}
void CStylerDlg::InitGroupBox()
{
	SetWindowTheme(GetDlgItem(IDJ_FONTGROUP_STATIC)->m_hWnd, L"wstr", L"wstr");
	SetWindowTheme(GetDlgItem(IDJ_COLOURGROUP_STATIC)->m_hWnd, L"wstr", L"wstr");
	SetWindowTheme(GetDlgItem(IDC_BORDERWIDTH_STATIC)->m_hWnd, L"wstr", L"wstr");

	SetWindowTheme(::GetDlgItem(m_DialogHwnd, IPAKMAN_PAKMAN_GROUP1), L"wstr", L"wstr");
}
void CStylerDlg::InitColorButton()
{
	for (int i = 0; i < 16; i++)
	{
		m_TempclrBacktemp[i] = theApp.GetGameAI()->sPage1.fontBackColor[i];
		m_TempclrTexttemp[i] = theApp.GetGameAI()->sPage1.fontTextColor[i];
	}
	UpdateColorPickerButton();
}

void CStylerDlg::InitCheckBox()
{
	int isChecked = (theApp.GetGameAI()->sPage1.DarkMode) ? BST_CHECKED : BST_UNCHECKED;
	::SendDlgItemMessage(GetSafeHwnd(), IPAKMAN_DARKMODE_CHECKBOX, BM_SETCHECK, isChecked, 0);

	isChecked = (theApp.GetGameAI()->sPage1.GlobalBack) ? BST_CHECKED : BST_UNCHECKED;
	::SendDlgItemMessage(GetSafeHwnd(), IDJ_GLOBAL_BG_CHECK, BM_SETCHECK, isChecked, 0);

	isChecked = (theApp.GetGameAI()->sPage1.m_edgeNoBorder) ? BST_CHECKED : BST_UNCHECKED;
	::SendDlgItemMessage(GetSafeHwnd(), IDC_CHECK_NOEDGE, BM_SETCHECK, isChecked, 0);

	isChecked = (theApp.GetGameAI()->sPage1.m_styleAlpha) ? BST_CHECKED : BST_UNCHECKED;
	::SendDlgItemMessage(GetSafeHwnd(), IDJ_SC_TRANSPARENT_CHECK, BM_SETCHECK, isChecked, 0);


	isChecked = (theApp.GetGameAI()->sPage1.m_nOpenFileNewWindow) ? BST_CHECKED : BST_UNCHECKED;
	::SendDlgItemMessage(GetSafeHwnd(), IDJ_CB_OPEN_FILE_NEW_WINDOW, BM_SETCHECK, isChecked, 0);

	isChecked = (theApp.GetGameAI()->sPage1.m_nLineNumber) ? BST_CHECKED : BST_UNCHECKED;
	::SendDlgItemMessage(GetSafeHwnd(), IDJ_LINE_NUMBER_CHECK, BM_SETCHECK, isChecked, 0);

	isChecked = (theApp.GetGameAI()->sPage1.m_colorSyntax) ? BST_CHECKED : BST_UNCHECKED;
	::SendDlgItemMessage(GetSafeHwnd(), IDJ_CB_COLOR_SYNTAX, BM_SETCHECK, isChecked, 0);

	isChecked = (theApp.GetGameAI()->sPage1.m_enableEOLflag) ? BST_CHECKED : BST_UNCHECKED;
	::SendDlgItemMessage(GetSafeHwnd(), IDJ_CB_ENABLE_EOL_FLAG, BM_SETCHECK, isChecked, 0);//

	isChecked = (theApp.GetGameAI()->sPage1.fontItalic[bTempSelection[STYLELIST]]) ? BST_CHECKED : BST_UNCHECKED;
	::SendDlgItemMessage(GetSafeHwnd(), IDJ_ITALIC_CHECK, BM_SETCHECK, isChecked, 0);
	isChecked = (theApp.GetGameAI()->sPage1.fontBold[bTempSelection[STYLELIST]]) ? BST_CHECKED : BST_UNCHECKED;
	::SendDlgItemMessage(GetSafeHwnd(), IDJ_BOLD_CHECK, BM_SETCHECK, isChecked, 0);
	isChecked = (theApp.GetGameAI()->sPage1.fontUnderline[bTempSelection[STYLELIST]]) ? BST_CHECKED : BST_UNCHECKED;
	::SendDlgItemMessage(GetSafeHwnd(), IDJ_UNDERLINE_CHECK, BM_SETCHECK, isChecked, 0);

	
}
void CStylerDlg::InitAllDarkButton()
{
	UpdateWindow();
	
	//AllowDarkModeForWindow(GetDlgItem(IPAKMAN_DARKMODE_CHECKBOX)->m_hWnd, theApp.GetGameAI()->sPage1.DarkMode);
	AllowDarkModeForWindow(GetDlgItem(IDJ_SWITCH2THEME_COMBO)->m_hWnd, theApp.GetGameAI()->sPage1.DarkMode);
	AllowDarkModeForWindow(GetDlgItem(IDJ_FONTSIZE_COMBO)->m_hWnd, theApp.GetGameAI()->sPage1.DarkMode);
	AllowDarkModeForWindow(GetDlgItem(IDJ_FONT_COMBO)->m_hWnd, theApp.GetGameAI()->sPage1.DarkMode);
	
	LPCWSTR classList;
	if (theApp.GetGameAI()->sPage1.DarkMode)
	{
		classList = L"DarkMode_CFD";
	}
	else
	{
		classList = L"wstr";
	}


	// checkbox
	//SetWindowTheme(GetDlgItem(IPAKMAN_DARKMODE_CHECKBOX)->m_hWnd, L"DarkMode_Explorer", nullptr);
	//subclassAndThemeButton(GetDlgItem(IPAKMAN_DARKMODE_CHECKBOX)->m_hWnd);
	//SetWindowTheme(GetDlgItem(IPAKMAN_DARKMODE_CHECKBOX)->m_hWnd, classList, L"wstr");
	disableVisualStyle(GetDlgItem(IPAKMAN_DARKMODE_CHECKBOX)->m_hWnd, TRUE);
	SetWindowTheme(GetDlgItem(IDJ_LINE_NUMBER_CHECK)->m_hWnd, classList, L"wstr");
	SetWindowTheme(GetDlgItem(IDJ_CB_COLOR_SYNTAX)->m_hWnd, classList, L"wstr");
	SetWindowTheme(GetDlgItem(IDJ_CB_ENABLE_EOL_FLAG)->m_hWnd, classList, L"wstr");
	SetWindowTheme(GetDlgItem(IDJ_CB_OPEN_FILE_NEW_WINDOW)->m_hWnd, classList, L"wstr");

	SetWindowTheme(GetDlgItem(IDC_CHECK_NOEDGE)->m_hWnd, classList, L"wstr");
	SetWindowTheme(GetDlgItem(IDJ_BOLD_CHECK)->m_hWnd, classList, L"wstr");
	SetWindowTheme(GetDlgItem(IDJ_ITALIC_CHECK)->m_hWnd, classList, L"wstr");
	SetWindowTheme(GetDlgItem(IDJ_UNDERLINE_CHECK)->m_hWnd, classList, L"wstr");
	SetWindowTheme(GetDlgItem(IDJ_SC_TRANSPARENT_CHECK)->m_hWnd, classList, L"wstr");

	SetWindowTheme(GetDlgItem(IDJ_GLOBAL_BG_CHECK)->m_hWnd, classList, L"wstr");
	SetWindowTheme(GetDlgItem(IDJ_GLOBAL_FG_CHECK)->m_hWnd, classList, L"wstr");
	SetWindowTheme(GetDlgItem(IDJ_GLOBAL_FONTSIZE_CHECK)->m_hWnd, classList, L"wstr");
	SetWindowTheme(GetDlgItem(IDJ_GLOBAL_BOLD_CHECK)->m_hWnd, classList, L"wstr");
	SetWindowTheme(GetDlgItem(IDJ_GLOBAL_ITALIC_CHECK)->m_hWnd, classList, L"wstr");
	SetWindowTheme(GetDlgItem(IDJ_GLOBAL_UNDERLINE_CHECK)->m_hWnd, classList, L"wstr");


	// combolist
	SetWindowTheme(GetDlgItem(IDJ_FONT_COMBO)->m_hWnd,classList, nullptr);
	SetWindowTheme(GetDlgItem(IDJ_FONTSIZE_COMBO)->m_hWnd,classList, nullptr);
	SetWindowTheme(GetDlgItem(IDJ_SWITCH2THEME_COMBO)->m_hWnd,classList, nullptr);
		
	// button
	InitDarkButton(GetDlgItem(IDC_BUTTON_DEFAULT)->m_hWnd);
	InitDarkButton(GetDlgItem(IDJ_SAVECLOSE_BUTTON)->m_hWnd);
	InitDarkButton(GetDlgItem(IDCANCEL)->m_hWnd);

	InitDarkButton(::GetDlgItem(m_DialogHwnd, IDC_BUTTON_SETTING));
	InitDarkButton(::GetDlgItem(m_DialogHwnd, IPAKMAN_BUTTON_CLEAR));
	InitDarkButton(::GetDlgItem(m_DialogHwnd, ID_FILE_OPEN));
	InitDarkButton(::GetDlgItem(m_DialogHwnd, ID_FILE_SAVEAS));
	InitDarkButton(::GetDlgItem(m_DialogHwnd, IPAKMAN_BUTTON_ENCRYPT));

	InitDarkButton(::GetDlgItem(m_DialogHwnd, IPAKMAN_BUTTON_ENCRYPTAS));
	InitDarkButton(::GetDlgItem(m_DialogHwnd, IPAKMAN_BUTTON_EXIT));
	InitDarkButton(::GetDlgItem(m_DialogHwnd, IPAKMAN_BUTTON_FIND));

	OnCBTransparent();// tumpang - nanti tukarkan daripada On kepada Init
}
/*void CStylerDlg::InitDarkButton1(HWND hButton)
{
	int isChecked = (theApp.GetGameAI()->sPage1.DarkMode) ? BST_CHECKED : BST_UNCHECKED;

	LPCWSTR classList;
	if (theApp.GetGameAI()->sPage1.DarkMode)
	{
		classList = L"Explorer";
	}
	else
	{
		classList = L"wstr";
	}
	SetWindowTheme(hButton, L"Explorer", nullptr);
	AllowDarkModeForWindow(hButton, isChecked);
}*/
void CStylerDlg::InitButton()
{
	GetDlgItem(IDJ_USER_KEYWORDS_STATIC)->ShowWindow(SW_HIDE);
	GetDlgItem(IDJ_USER_KEYWORDS_EDIT)->ShowWindow(SW_HIDE);

	GetDlgItem(IDJ_GLOBAL_GOTOSETTINGS_LINK)->ShowWindow(SW_HIDE);
	GetDlgItem(IDJ_DEF_KEYWORDS_EDIT)->ShowWindow(SW_HIDE);

	GetDlgItem(IDJ_DEF_EXT_STATIC)->ShowWindow(SW_HIDE);
	GetDlgItem(IDJ_DEF_EXT_EDIT)->ShowWindow(SW_HIDE);

	GetDlgItem(IDJ_USER_EXT_STATIC)->ShowWindow(SW_HIDE);
	GetDlgItem(IDJ_USER_EXT_EDIT)->ShowWindow(SW_HIDE);

	GetDlgItem(IDJ_PLUSSYMBOL2_STATIC)->ShowWindow(SW_HIDE);

	GetDlgItem(IDJ_LANGDESC_STATIC)->ShowWindow(SW_HIDE);
	GetDlgItem(IDJ_LANGUAGES_LIST)->ShowWindow(SW_HIDE);
}
void CStylerDlg::InitFontComboBox()
{
	m_fontCombo.SubclassDlgItem(IDJ_FONT_COMBO, this);
	m_fontCombo.m_csSample = "No Fate But What We Make";
	m_fontCombo.SetFontHeight(10);
	m_fontCombo.SetPreviewStyle(CFontPreviewCombo::NAME_GUI_FONT, !false);
	m_fontCombo.Init();
	SelectFont(theApp.GetGameAI()->sPage1.fontName);
}
void CStylerDlg::InitFontSizeComboBox()
{
	m_fontSizeCombo.SubclassDlgItem(IDJ_FONTSIZE_COMBO, this);
	m_fontSizeCombo.ResetContent();
	CString tmp;
	for (int i = 0; i<15; ++i)
	{
		tmp.Format("%d", static_cast<DWORD>(bSelFontSize[i]));
		m_fontSizeCombo.AddString(tmp);
	}
	tmp.Format("%d", theApp.GetGameAI()->sPage1.fontSize);
	SelectFontSize(tmp);
}
void CStylerDlg::InitStyleListBox()
{
	m_styleListBox.SubclassDlgItem(IDJ_STYLES_LIST, this);
	m_styleListBox.ResetContent();
	CString tmp;
	for (int i = 0; i<TOTAL_STYLE; ++i)
	{
		tmp.LoadString(IDS_STRING102 + i);
		m_styleListBox.AddString(tmp);
	}

	m_styleListBox.SetCurSel(bTempSelection[STYLELIST]);
}
/**************************Dialog initializer end******************************/









/*******************************On selection control**************************************/

void CStylerDlg::OnBnClickedSavecloseButton()
{
	UpdateData();
	Invalidate();

	OnButtonBackcolor();
	OnButtonTextcolor();

	SafeStrCpy(theApp.GetGameAI()->sPage1.fontName, m_faceName, strlen(m_faceName)+1);
	theApp.GetGameAI()->sPage1.fontSize = atoi(m_fontSize);

	bool isChecked = (BST_CHECKED == ::SendDlgItemMessage(GetSafeHwnd(), IDJ_GLOBAL_BG_CHECK, BM_GETCHECK, 0, 0));
	theApp.GetGameAI()->sPage1.GlobalBack = isChecked;

	isChecked = (BST_CHECKED == ::SendDlgItemMessage(GetSafeHwnd(), IDC_CHECK_NOEDGE, BM_GETCHECK, 0, 0));
	theApp.GetGameAI()->sPage1.m_edgeNoBorder = isChecked;

	isChecked = (BST_CHECKED == ::SendDlgItemMessage(GetSafeHwnd(), IDJ_SC_TRANSPARENT_CHECK, BM_GETCHECK, 0, 0));
	theApp.GetGameAI()->sPage1.m_styleAlpha = isChecked;

	isChecked = (BST_CHECKED == ::SendDlgItemMessage(GetSafeHwnd(), IPAKMAN_DARKMODE_CHECKBOX, BM_GETCHECK, 0, 0));
	theApp.GetGameAI()->sPage1.DarkMode = isChecked;
	SetDarkMode((BOOL)isChecked, (BOOL)isChecked);
	if (isChecked)
		InitDarkMode();
	InitAllDarkButton();

	isChecked = (BST_CHECKED == ::SendDlgItemMessage(GetSafeHwnd(), IDJ_LINE_NUMBER_CHECK, BM_GETCHECK, 0, 0));
	theApp.GetGameAI()->sPage1.m_nLineNumber = isChecked;

	isChecked = (BST_CHECKED == ::SendDlgItemMessage(GetSafeHwnd(), IDJ_CB_OPEN_FILE_NEW_WINDOW, BM_GETCHECK, 0, 0));
	theApp.GetGameAI()->sPage1.m_nOpenFileNewWindow = isChecked;

	isChecked = (BST_CHECKED == ::SendDlgItemMessage(GetSafeHwnd(), IDJ_CB_COLOR_SYNTAX, BM_GETCHECK, 0, 0));
	theApp.GetGameAI()->sPage1.m_colorSyntax = isChecked;

	isChecked = (BST_CHECKED == ::SendDlgItemMessage(GetSafeHwnd(), IDJ_CB_ENABLE_EOL_FLAG, BM_GETCHECK, 0, 0));
	theApp.GetGameAI()->sPage1.m_enableEOLflag = isChecked;

	isChecked = (BST_CHECKED == ::SendDlgItemMessage(GetSafeHwnd(), IDJ_UNDERLINE_CHECK, BM_GETCHECK, 0, 0));
	theApp.GetGameAI()->sPage1.fontUnderline[m_styleListBox.GetCurSel()] = isChecked;
	isChecked = (BST_CHECKED == ::SendDlgItemMessage(GetSafeHwnd(), IDJ_BOLD_CHECK, BM_GETCHECK, 0, 0));
	theApp.GetGameAI()->sPage1.fontBold[m_styleListBox.GetCurSel()] = isChecked;
	isChecked = (BST_CHECKED == ::SendDlgItemMessage(GetSafeHwnd(), IDJ_ITALIC_CHECK, BM_GETCHECK, 0, 0));
	theApp.GetGameAI()->sPage1.fontItalic[m_styleListBox.GetCurSel()] = isChecked;

	theApp.GetGameAI()->sPage1.m_styleAlphaPercent = GetStylerAlphaPercent();
	theApp.GetGameAI()->sPage1.borderWidth = GetBorderWidth();

	theApp.WriteConfig();
	::RefreshTitleBarThemeColor(m_DialogHwnd);
	::SendMessage(m_DialogHwnd, WM_REFRESHBUTTONSTATES, TRUE, 0);

	OnOK();
}
void CStylerDlg::OnDropDownFontSelchange()
{
	UpdateData();
	//LPCTSTR pszB = m_faceName;
	//CString sC(pszB);
	//const char* pszE = sC;
	//char* pszF = const_cast<char*>(pszE);
	//AfxMessageBox(pszF);
}
void CStylerDlg::OnDropDownFontSizeSelchange()
{
	UpdateData();
	bTempSelection[FONTSIZECOMBO] = m_fontSizeCombo.GetCurSel();
	m_fontSizeCombo.GetLBText(bTempSelection[FONTSIZECOMBO], m_fontSize);
	//AfxMessageBox(m_fontSize);
}
void CStylerDlg::OnSelchangeStyleListBox()
{
	UpdateData();
	CString str;
	bTempSelection[STYLELIST] = m_styleListBox.GetCurSel();
	m_styleListBox.GetText(bTempSelection[STYLELIST], str);

	UpdateColorPickerButton();

	BOOL isChecked = (theApp.GetGameAI()->sPage1.fontItalic[bTempSelection[STYLELIST]]) ? BST_CHECKED : BST_UNCHECKED;
	::SendDlgItemMessage(GetSafeHwnd(), IDJ_ITALIC_CHECK, BM_SETCHECK, isChecked, 0);
	isChecked = (theApp.GetGameAI()->sPage1.fontBold[bTempSelection[STYLELIST]]) ? BST_CHECKED : BST_UNCHECKED;
	::SendDlgItemMessage(GetSafeHwnd(), IDJ_BOLD_CHECK, BM_SETCHECK, isChecked, 0);
	isChecked = (theApp.GetGameAI()->sPage1.fontUnderline[bTempSelection[STYLELIST]]) ? BST_CHECKED : BST_UNCHECKED;
	::SendDlgItemMessage(GetSafeHwnd(), IDJ_UNDERLINE_CHECK, BM_SETCHECK, isChecked, 0);
	//AfxMessageBox(str);
}
void CStylerDlg::OnButtonBackcolor()
{
	UpdateData();
	for (int i = 0; i < 16; i++)
	{
		theApp.GetGameAI()->sPage1.fontBackColor[i] = m_TempclrBacktemp[i];
	}
	//theApp.GetGameAI()->sPage1.fontBackColor[m_styleListBox.GetCurSel()] = m_staticBtnColor[BackColor].GetColor();
}
void CStylerDlg::OnButtonTextcolor()
{
	UpdateData();
	for (int i = 0; i < 16; i++)
	{
		theApp.GetGameAI()->sPage1.fontTextColor[i] = m_TempclrTexttemp[i];
	}
	//theApp.GetGameAI()->sPage1.fontTextColor[m_styleListBox.GetCurSel()] = m_staticBtnColor[TextColor].GetColor();
}
void CStylerDlg::OnCBBorderWidth()
{
	UpdateData();

	//int percent = static_cast<int32_t>(::SendDlgItemMessage(GetSafeHwnd(), IDC_BORDERWIDTH_SLIDER, TBM_GETPOS, 0, 0));
	m_sliderBorderWidthValue.Format("%d", GetBorderWidth());
	theApp.GetGameAI()->sPage1.borderWidth = GetBorderWidth();

	CRect rc = m_editorRect;
	::MoveWindow(m_hwndEditor,
		rc.left + GetBorderWidth(),
		rc.top + GetBorderWidth(),
		rc.Width() - GetBorderWidth() * 2,
		rc.Height() - GetBorderWidth() * 2, TRUE);

	UpdateData(FALSE);
}
int CStylerDlg::GetBorderWidth()
{
	return static_cast<int32_t>(::SendDlgItemMessage(GetSafeHwnd(), IDC_BORDERWIDTH_SLIDER, TBM_GETPOS, 0, 0));
}
void CStylerDlg::OnCBNoBorder()
{
	CButton*btn = (CButton*)GetDlgItem(IDC_CHECK_NOEDGE);
	if (btn->GetCheck())
		setBorderEdge(m_hwndEditor, FALSE);
	else
		setBorderEdge(m_hwndEditor, TRUE);
}
void CStylerDlg::OnCBTransparent()
{
	UpdateData();
	CButton*btn = (CButton*)GetDlgItem(IDJ_SC_TRANSPARENT_CHECK);
	SetTransparent(GetSafeHwnd(), btn->GetCheck() ? GetStylerAlphaPercent() : 255);
	UpdateData(FALSE);
}
int CStylerDlg::GetStylerAlphaPercent()
{
	return static_cast<int32_t>(::SendDlgItemMessage(GetSafeHwnd(), IDJ_SC_PERCENTAGE_SLIDER, TBM_GETPOS, 0, 0));	
}
void CStylerDlg::OnButtonDefault()
{
	if (MessageBox("Set everything to default?", "info", MB_YESNO) == IDNO)
	{
		return;
	}
	else
	{
		theApp.defaultConfig();
		::SendMessage(m_DialogHwnd, WM_REFRESHBUTTONSTATES, TRUE, 0);
		OnOK();
	}
}
void CStylerDlg::OnCBDarkMode()
{
	UpdateData();
	bool isDarkModeChecked = (BST_CHECKED == ::SendDlgItemMessage(GetSafeHwnd(), IPAKMAN_DARKMODE_CHECKBOX, BM_GETCHECK, 0, 0));
	theApp.GetGameAI()->sPage1.DarkMode = isDarkModeChecked;
	Invalidate();
}
void CStylerDlg::UpdateColorPickerButton_temp()
{
	UpdateData();

	for (int i = 0; i < 16; i++)
	{
		m_TempclrBacktemp[i] = theApp.GetGameAI()->sPage1.fontBackColor[i];
		m_TempclrTexttemp[i] = theApp.GetGameAI()->sPage1.fontTextColor[i];
	}
}
void CStylerDlg::UpdateColorPickerButton()
{
	UpdateData();

	COLORREF m_TempclrBack;
	COLORREF m_TempclrText;

	m_TempclrText = m_TempclrTexttemp[m_styleListBox.GetCurSel()];// theApp.GetGameAI()->sPage1.fontTextColor[m_styleListBox.GetCurSel()];
	m_TempclrBack = m_TempclrBacktemp[m_styleListBox.GetCurSel()];// theApp.GetGameAI()->sPage1.fontBackColor[m_styleListBox.GetCurSel()];

	//m_TempclrText = theApp.GetGameAI()->sPage1.fontTextColor[m_styleListBox.GetCurSel()];
	//m_TempclrBack = theApp.GetGameAI()->sPage1.fontBackColor[m_styleListBox.GetCurSel()];

	m_staticBtnColor[BackColor].SetColor(m_TempclrBack);
	m_staticBtnColor[TextColor].SetColor(m_TempclrText);

	CString str;
	m_styleListBox.GetText(m_styleListBox.GetCurSel(), str);

	CString strColor = str, strText;

	strText.Format("Global Styles : %s", strColor);
	m_styleDescStatic.SetWindowText(strText);
}
int CStylerDlg::SelectFont(LPCTSTR lpszFontName)
{
	ASSERT(lpszFontName != NULL);
	CString sFontName = lpszFontName;
	if (sFontName.IsEmpty())
	{
		m_faceName = _T("");
		return m_fontCombo.SetCurSel(-1);
	}
	else
	{
		m_faceName = _T(lpszFontName);
		return m_fontCombo.SelectString(-1, sFontName);
	}
}
int CStylerDlg::SelectFontSize(LPCTSTR lpszFontSize)
{
	ASSERT(lpszFontSize != NULL);
	CString sFontName = lpszFontSize;
	if (sFontName.IsEmpty())
	{
		m_fontSize = _T("");
		return m_fontSizeCombo.SetCurSel(-1);
	}
	else
	{
		m_fontSize = _T(lpszFontSize);
		return m_fontSizeCombo.SelectString(-1, m_fontSize);
	}
}
void CStylerDlg::SelectDefaultFocus(int id)
{
	::SetFocus(::GetDlgItem(m_hWnd, id));
}
void CStylerDlg::enableEOLflagCB()
{
	bool isChecked = (BST_CHECKED == ::SendDlgItemMessage(GetSafeHwnd(), IDJ_CB_ENABLE_EOL_FLAG, BM_GETCHECK, 0, 0));

	::SendMessage(m_hwndEditor, SCI_SETVIEWEOL, isChecked, 0);
}
void CStylerDlg::enableLineNumberCB()
{
	bool isChecked = (BST_CHECKED == ::SendDlgItemMessage(GetSafeHwnd(), IDJ_LINE_NUMBER_CHECK, BM_GETCHECK, 0, 0));

	//((theDlg*)m_pParent)->m_ScinCtrl.SendMessage(SCI_SETMARGINWIDTHN, 0, 0);
	((theDlg*)m_pParent)->m_ScinCtrl.SetDisplayLinenumbers(isChecked);
	//::SendMessage(m_hwndEditor, SCI_SETVIEWEOL, isChecked, 0);
}
void CStylerDlg::onSelectBackColor()
{
	m_TempclrBacktemp[m_styleListBox.GetCurSel()] = m_staticBtnColor[BackColor].GetColor();
	enableLexerBackStyleCB();
}
void CStylerDlg::onSelectTextColor()
{
	m_TempclrTexttemp[m_styleListBox.GetCurSel()] = m_staticBtnColor[TextColor].GetColor();
	enableLexerForeStyleCB();
}
void CStylerDlg::enableLexerBackStyleCB()
{
	bool isChecked = (BST_CHECKED == ::SendDlgItemMessage(GetSafeHwnd(), IDJ_GLOBAL_BG_CHECK, BM_GETCHECK, 0, 0));

	//::SendMessage(m_hwndEditor, SCI_SETVIEWEOL, isChecked, 0);

	::SendMessage(m_hwndEditor, SCI_STYLESETBACK, STYLE_DEFAULT, GET_BACK_COLOR_TEMP(0));
	::SendMessage(m_hwndEditor, SCI_COLOURISE, 0, -1);

	::SendMessage(m_hwndEditor,SCI_STYLESETBACK, SCE_C_WORD, isChecked ? GET_BACK_COLOR_TEMP(0) : GET_BACK_COLOR_TEMP(5));
	::SendMessage(m_hwndEditor,SCI_STYLESETBACK, SCE_C_STRING, isChecked ? GET_BACK_COLOR_TEMP(0) : GET_BACK_COLOR_TEMP(6));
	::SendMessage(m_hwndEditor,SCI_STYLESETBACK, SCE_C_CHARACTER, isChecked ? GET_BACK_COLOR_TEMP(0) : GET_BACK_COLOR_TEMP(7));
	::SendMessage(m_hwndEditor,SCI_STYLESETBACK, SCE_C_PREPROCESSOR, isChecked ? GET_BACK_COLOR_TEMP(0) : GET_BACK_COLOR_TEMP(8));
	::SendMessage(m_hwndEditor,SCI_STYLESETBACK, SCE_C_COMMENT, isChecked ? GET_BACK_COLOR_TEMP(0) : GET_BACK_COLOR_TEMP(9));
	::SendMessage(m_hwndEditor,SCI_STYLESETBACK, SCE_C_COMMENTLINE, isChecked ? GET_BACK_COLOR_TEMP(0) : GET_BACK_COLOR_TEMP(10));
	::SendMessage(m_hwndEditor,SCI_STYLESETBACK, SCE_C_COMMENTDOC, isChecked ? GET_BACK_COLOR_TEMP(0) : GET_BACK_COLOR_TEMP(11));
	::SendMessage(m_hwndEditor,SCI_STYLESETBACK, SCE_C_NUMBER, isChecked ? GET_BACK_COLOR_TEMP(0) : GET_BACK_COLOR_TEMP(12));
	::SendMessage(m_hwndEditor,SCI_STYLESETBACK, SCE_C_OPERATOR, isChecked ? GET_BACK_COLOR_TEMP(0) : GET_BACK_COLOR_TEMP(13));
	::SendMessage(m_hwndEditor,SCI_STYLESETBACK, SCE_C_UUID, isChecked ? GET_BACK_COLOR_TEMP(0) : GET_BACK_COLOR_TEMP(14));
}
void CStylerDlg::enableLexerForeStyleCB()
{
	bool isChecked = (BST_CHECKED == ::SendDlgItemMessage(GetSafeHwnd(), IDJ_CB_COLOR_SYNTAX, BM_GETCHECK, 0, 0));

	//::SendMessage(m_hwndEditor, SCI_SETVIEWEOL, isChecked, 0);
	::SendMessage(m_hwndEditor, SCI_STYLESETFORE, STYLE_DEFAULT, GET_FORE_COLOR_TEMP(0));
	::SendMessage(m_hwndEditor, SCI_COLOURISE, 0, -1);


	::SendMessage(m_hwndEditor, SCI_STYLESETFORE, SCE_C_WORD, isChecked ? GET_FORE_COLOR_TEMP(5) : GET_FORE_COLOR_TEMP(0));
	::SendMessage(m_hwndEditor, SCI_STYLESETFORE, SCE_C_STRING, isChecked ? GET_FORE_COLOR_TEMP(6) : GET_FORE_COLOR_TEMP(0));
	::SendMessage(m_hwndEditor, SCI_STYLESETFORE, SCE_C_CHARACTER, isChecked ? GET_FORE_COLOR_TEMP(7) : GET_FORE_COLOR_TEMP(0));
	::SendMessage(m_hwndEditor, SCI_STYLESETFORE, SCE_C_PREPROCESSOR, isChecked ? GET_FORE_COLOR_TEMP(8) : GET_FORE_COLOR_TEMP(0));
	::SendMessage(m_hwndEditor, SCI_STYLESETFORE, SCE_C_COMMENT, isChecked ? GET_FORE_COLOR_TEMP(9) : GET_FORE_COLOR_TEMP(0));
	::SendMessage(m_hwndEditor, SCI_STYLESETFORE, SCE_C_COMMENTLINE, isChecked ? GET_FORE_COLOR_TEMP(10) : GET_FORE_COLOR_TEMP(0));
	::SendMessage(m_hwndEditor, SCI_STYLESETFORE, SCE_C_COMMENTDOC, isChecked ? GET_FORE_COLOR_TEMP(11) : GET_FORE_COLOR_TEMP(0));
	::SendMessage(m_hwndEditor, SCI_STYLESETFORE, SCE_C_NUMBER, isChecked ? GET_FORE_COLOR_TEMP(12) : GET_FORE_COLOR_TEMP(0));
	::SendMessage(m_hwndEditor, SCI_STYLESETFORE, SCE_C_OPERATOR, isChecked ? GET_FORE_COLOR_TEMP(13) : GET_FORE_COLOR_TEMP(0));
	::SendMessage(m_hwndEditor, SCI_STYLESETFORE, SCE_C_UUID, isChecked ? GET_FORE_COLOR_TEMP(14) : GET_FORE_COLOR_TEMP(0));
}


/*************************************************/
void CStylerDlg::SetSelectThemeEnable(BOOL bFlag)
{
	if (bFlag == FALSE)
	{
		GetDlgItem(IDJ_SWITCH2THEME_STATIC)->ShowWindow(SW_HIDE);
		GetDlgItem(IDJ_SWITCH2THEME_COMBO)->ShowWindow(SW_HIDE);
	}	
}