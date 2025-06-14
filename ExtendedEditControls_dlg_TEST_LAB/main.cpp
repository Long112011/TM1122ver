// ExtendedEditControls.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "main.h"
//#include "DarkMode.h"
//#include "SettingDialog.h"
//#include "ScintillaWnd.h"



/*TCHAR szWindowClass[512];
ATOM				MyRegisterClass(HINSTANCE hInstance);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	strcpy(szWindowClass, "gg");
	WNDCLASSEX wcex;
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
	wcex.lpfnWndProc = (WNDPROC)WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDR_MAINFRAME));
	wcex.hCursor = NULL;
	wcex.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = NULL;
	return RegisterClassEx(&wcex);
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_CHAR:
	case WM_IME_CHAR:
	case WM_IME_STARTCOMPOSITION:
		return 0;


	case WM_CLOSE:
	{

	}
		break;
	case WM_SETFOCUS:
	{
						//CURSOR->Process();
	}
		return 0;
	case WM_SETCURSOR:
	{
						 //CURSOR->Process();
	}
		break;
	case WM_SYSKEYDOWN:
	{
						  if (wParam == VK_F4)
						  {
							  TT
						  }
	}
		return 0;
	case WM_SYSCOMMAND:
	{
						  if (wParam == SC_KEYMENU)
							  return 0;
	}
		break;
	case WM_SYSKEYUP:
	{
	}
		return 0;

	case WM_LBUTTONDOWN:
		TT
			break;


	}
	return DefWindowProc(hWnd, message, wParam, lParam);
}*/

BEGIN_MESSAGE_MAP(CMainApp, CWinApp)
END_MESSAGE_MAP()
CMainApp::CMainApp() : CWinApp(_T("bin Editor"))
, m_scintillaLoaded(FALSE)
{
	m_hSciDLL = NULL;

	//MyRegisterClass(AfxGetInstanceHandle());
	//m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_RESTART;
	//m_hLexDLL = NULL;
	//SetAppID(_T("RadNotepad.AppID.NoVersion"));
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}
CMainApp theApp;
//static HINSTANCE g_hRichEdDLL=NULL;
CString LastOutputPath, facename;


#include "afxshellmanager.h"
#include "cmdline.h"
BOOL CMainApp::InitInstance()
{
	AfxOleInit();
	ReadConfig();
	m_SplashInit = SplashInit(0);

	// We use a GUID here, so that we are sure the string is 100% unique
	if (false==theApp.GetGameAI()->sPage1.m_nOpenFileNewWindow)
	if (CSingleInstance::Create(_T(/*"E435FC13-82C1-4f80-97C5-006FF4A4E302"*/m_GameAI.sPage1.m_storedGUID)) == FALSE)
		return FALSE;

	HINSTANCE hUser32 = LoadLibrary("User32.dll");
	if (hUser32)
	{
		typedef BOOL(WINAPI* LPSetProcessDPIAware)(void);
		LPSetProcessDPIAware pSetProcessDPIAware = (LPSetProcessDPIAware)GetProcAddress(hUser32, "SetProcessDPIAware");
		if (pSetProcessDPIAware)
		{
			pSetProcessDPIAware();
		}
		FreeLibrary(hUser32);
	}
	INITCOMMONCONTROLSEX controlFlags;
	controlFlags.dwSize = sizeof(INITCOMMONCONTROLSEX);
	controlFlags.dwICC = ICC_STANDARD_CLASSES | ICC_DATE_CLASSES;
	InitCommonControlsEx(&controlFlags);
	SetRegistryKey(_T("JACKJACKJACK"));

	CWinApp::InitInstance();

	if ((BOOL)theApp.GetGameAI()->sPage1.DarkMode)
	{
		InitDarkMode();
	}
	if (!Scintilla_RegisterClasses(AfxGetInstanceHandle()))
	{
		m_scintillaLoaded = FALSE;
		AfxMessageBox("failed to initiailze");
	}
	else
	{
		m_scintillaLoaded = TRUE;
	}
	

	theDlg dlg2;// = new theDlg;
	m_pMainWnd = &dlg2;
	

	//TT
	/*SERVERCMD->OnCommand("/kill THIS");
	SplashQuit();
	ExitInstance();
	m_pMainWnd = NULL;*/

	INT_PTR nResponse = dlg2.DoModal();
	if (nResponse == IDOK)
	{
	}
	else if (nResponse == IDCANCEL)
	{
		TT
		/*SERVERCMD->OnCommand("/kill WER");
		SplashQuit();
		ExitInstance();
		m_pMainWnd = NULL;*/
		//m_pMainWnd->DestroyWindow();
	}
	
	return FALSE;
}
int CMainApp::ExitInstance()
{

	// TODO: Add your specialized code here and/or call the base class

	// unload the library
	//if(g_hRichEdDLL!=NULL)
	//	::FreeLibrary(g_hRichEdDLL);

	//FreeLibrary(m_hSciDLL);
	//m_hSciDLL = NULL;

	//FreeLibrary(m_hLexDLL);
	//m_hLexDLL = NULL;

	// Release Scintilla
	Scintilla_ReleaseResources();

	SplashQuit();
	ExitInstance();
	m_pMainWnd = NULL;

	/*if (m_hDll != NULL)
	{
	FreeLibrary(m_hDll);
	m_hDll = NULL;
	}*/
	AfxOleTerm(FALSE);
	return CWinApp::ExitInstance();
}
int CMainApp::SplashInit(int timer)
{
	if (timer == 0) return 0;
	//ADDITION OF SPLASH SCREEN COMPONENT
	//Bring up the splash screen in a secondary UI thread
	pSplashThread = (CSplashThread*)AfxBeginThread(RUNTIME_CLASS(CSplashThread), THREAD_PRIORITY_NORMAL, 0, CREATE_SUSPENDED);
	if (pSplashThread == NULL)
	{
		AfxMessageBox(_T("Failed to create splash screen"), MB_OK | MB_ICONSTOP);
		return FALSE;
	}
	ASSERT(pSplashThread->IsKindOf(RUNTIME_CLASS(CSplashThread)));

	//Tell the splash screen to load the bitmap from a resource called IDB_SPLASH.
	//Alternatively, you could use the line:
	//  pSplashThread->SetBitmapToUse(GetHomeDirectory() + _T("TEST.BMP")); 
	//if you wanted the splasher code to load a bitmap from file at runtime.
	pSplashThread->SetBitmapToUse(IDB_SPLASH);

	pSplashThread->ResumeThread();  //Resume the thread now that we have set it up 

	Sleep(timer);                    //Simulate some slow loading process. Your code might

	return 1;
}

void CMainApp::SplashQuit()
{
	/*try
	{
		try
		{
			// code here
			AfxMessageBox(_T("try"), MB_OK | MB_ICONSTOP);
			pSplashThread->HideSplash();
		}
		catch (int n)
		{
			throw;
		}
	}
	catch (...)
	{
		cout << "Exception occurred";
	}*/


	if (m_SplashInit == FALSE)
	{
		//AfxMessageBox(_T("if"), MB_OK | MB_ICONSTOP);
		return;
	}

	pSplashThread->HideSplash();
	/*try
	{
		
	}
	catch (...)
	{
		AfxMessageBox(_T("catch"), MB_OK | MB_ICONSTOP);
		//AfxMessageBox(_T("Failed to create splash screen"), MB_OK | MB_ICONSTOP);
	}*/
}
void CMainApp::defaultConfig()
{
	DeleteFile(_PlayerAI_Config_);
	ReadConfig();
}
void CMainApp::WriteConfig()
{
	char stringtoread[MAX_NAME_LENGTH+1];
	memset(stringtoread, 0, MAX_NAME_LENGTH+1);
	char section[MAX_NAME_LENGTH+1];
	sprintf(section, "Configuration");

	COPini::Set(section, "GUID", m_GameAI.sPage1.m_storedGUID);

	COPini::Set(section, "FontName", m_GameAI.sPage1.fontName);

	char str[MAX_NAME_LENGTH+1];
	sprintf(str, "%d", m_GameAI.sPage1.DarkMode);
	COPini::Set(section, "DarkMode", str);

	sprintf(str, "%d", m_GameAI.sPage1.GlobalBack);
	COPini::Set(section, "GlobalBack", str);

	sprintf(str, "%d", m_GameAI.sPage1.m_nOpenFileNewWindow);
	COPini::Set(section, "OpenFileNewWindow", str);

	sprintf(str, "%d", m_GameAI.sPage1.m_nLineNumber);
	COPini::Set(section, "LineNumber", str);

	sprintf(str, "%d", m_GameAI.sPage1.m_colorSyntax);
	COPini::Set(section, "ColorSyntax", str);

	sprintf(str, "%d", m_GameAI.sPage1.m_edgeNoBorder);
	COPini::Set(section, "BorderEdge", str);

	sprintf(str, "%d", m_GameAI.sPage1.m_styleAlpha);
	COPini::Set(section, "StylerAlpha", str);

	sprintf(str, "%d", m_GameAI.sPage1.m_enableEOLflag);
	COPini::Set(section, "EnableEOLflag", str);




	sprintf(str, "%d", m_GameAI.sPage1.fontSize);
	COPini::Set(section, "FontSize", str);

	sprintf(str, "%d", m_GameAI.sPage1.borderWidth);
	COPini::Set(section, "borderWidth", str);

	sprintf(str, "%d", m_GameAI.sPage1.m_styleAlphaPercent);
	COPini::Set(section, "stylerAlphaPercent", str);


	for (int b = 0; b < TOTAL_STYLE; ++b)
	{
		sprintf(section, "Style_%d", b);

		sprintf(str, "%d", m_GameAI.sPage1.fontTextColor[b]);
		COPini::Set(section, "FontTextColor", str);

		sprintf(str, "%d", m_GameAI.sPage1.fontBackColor[b]);
		COPini::Set(section, "FontBackColor", str);


		sprintf(str, "%d", m_GameAI.sPage1.fontUnderline[b]);
		COPini::Set(section, "fontUnderline", str);
		sprintf(str, "%d", m_GameAI.sPage1.fontBold[b]);
		COPini::Set(section, "fontBold", str);
		sprintf(str, "%d", m_GameAI.sPage1.fontItalic[b]);
		COPini::Set(section, "fontItalic", str);
	}
	
}
void CMainApp::ReadConfig()
{	
	char stringtoread[MAX_NAME_LENGTH+1];
	memset(stringtoread, 0, MAX_NAME_LENGTH+1);
	char section[MAX_NAME_LENGTH+1];

	sprintf(section, "Configuration");

	COPini::Load(section, "GUID", stringtoread, (LPSTR)generateGUID_N().MakeUpper().GetString());
	SafeStrCpy(m_GameAI.sPage1.m_storedGUID, stringtoread, MAX_NAME_LENGTH + 1);

	COPini::Load(section, "FontName", stringtoread, "Consolas");
	SafeStrCpy(m_GameAI.sPage1.fontName, stringtoread, MAX_NAME_LENGTH + 1);

	COPini::Load(section, "DarkMode", stringtoread, "0");
	m_GameAI.sPage1.DarkMode = atoi(stringtoread);

	COPini::Load(section, "GlobalBack", stringtoread, "1");
	m_GameAI.sPage1.GlobalBack = atoi(stringtoread);

	COPini::Load(section, "OpenFileNewWindow", stringtoread, "0");
	m_GameAI.sPage1.m_nOpenFileNewWindow = atoi(stringtoread);

	COPini::Load(section, "LineNumber", stringtoread, "1");
	m_GameAI.sPage1.m_nLineNumber = atoi(stringtoread);

	COPini::Load(section, "ColorSyntax", stringtoread, "1");
	m_GameAI.sPage1.m_colorSyntax = atoi(stringtoread);

	COPini::Load(section, "BorderEdge", stringtoread, "1");
	m_GameAI.sPage1.m_edgeNoBorder = atoi(stringtoread);

	COPini::Load(section, "StylerAlpha", stringtoread, "0");
	m_GameAI.sPage1.m_styleAlpha = atoi(stringtoread);

	COPini::Load(section, "EnableEOLflag", stringtoread, "0");
	m_GameAI.sPage1.m_enableEOLflag = atoi(stringtoread);

	COPini::Load(section, "FontSize", stringtoread, "10");
	m_GameAI.sPage1.fontSize = atoi(stringtoread);

	COPini::Load(section, "borderWidth", stringtoread, "0");
	m_GameAI.sPage1.borderWidth = atoi(stringtoread); 

	COPini::Load(section, "stylerAlphaPercent", stringtoread, "255");
	m_GameAI.sPage1.m_styleAlphaPercent = atoi(stringtoread);


	char str[256];
	//for (int b = 0; b < TOTAL_STYLE; ++b)
	for (long b = 0; g_rgbSyntaxCpp[b].iItem != -1; b++)
	{
		sprintf(section, "Style_%d", b);
		
		//sprintf(str, "%d", RGB(100 * b, 10, 100 * b));
		sprintf(str, "%d", g_rgbSyntaxCpp[b].rgbFore);
		COPini::Load(section, "FontTextColor", stringtoread, str);
		m_GameAI.sPage1.fontTextColor[b] = atoi(stringtoread);

		sprintf(str, "%d", g_rgbSyntaxCpp[b].rgbBack);
		COPini::Load(section, "FontBackColor", stringtoread, str);
		m_GameAI.sPage1.fontBackColor[b] = atoi(stringtoread);

		COPini::Load(section, "fontUnderline", stringtoread, "0");
		m_GameAI.sPage1.fontUnderline[b] = atoi(stringtoread);
		COPini::Load(section, "fontBold", stringtoread, "0");
		m_GameAI.sPage1.fontBold[b] = atoi(stringtoread);
		COPini::Load(section, "fontItalic", stringtoread, "0");
		m_GameAI.sPage1.fontItalic[b] = atoi(stringtoread);
	}	
}
unsigned int fontstyle,
fontsize,
fontstrike,
fontunderline,
fontcolor;
void CMainApp::LoadConfig_OLD()
{
	SetRegistryKey(REGNAMED);

	LastOutputPath = GetProfileString(REGFOLDER, "LastOutputPath", "headergenerator");
	WriteProfileString(REGFOLDER,"LastOutputPath","headergenerator");

	facename = GetProfileString(REGFOLDER, "facename", "Consolas");
	WriteProfileString(REGFOLDER, "facename", facename);

	fontstyle = GetProfileInt(REGFOLDER,"fontstyle", 0);
	WriteProfileInt(REGFOLDER,"fontstyle", fontstyle);

	fontsize = GetProfileInt(REGFOLDER,"fontsize", 10);
	WriteProfileInt(REGFOLDER,"fontsize", fontsize);

	fontstrike = GetProfileInt(REGFOLDER,"fontstrike", 0);
	WriteProfileInt(REGFOLDER,"fontstrike", fontstrike);

	fontunderline = GetProfileInt(REGFOLDER,"fontunderline", 0);
	WriteProfileInt(REGFOLDER,"fontunderline", fontunderline);

	fontcolor = GetProfileInt(REGFOLDER,"fontcolor", 0);
	WriteProfileInt(REGFOLDER,"fontcolor", fontcolor);
}
HMODULE CMainApp::LoadLibraryFromApplicationDirectory(LPCTSTR lpFileName)
{
	//Get the Application directory
	CString sFullPath;
	const DWORD dwGMFN{ GetModuleFileName(nullptr, sFullPath.GetBuffer(_MAX_PATH), _MAX_PATH) };
	sFullPath.ReleaseBuffer();
	if (dwGMFN == 0)
#pragma warning(suppress: 26487)
		return nullptr;

	//Form the new path
	CString sDrive;
	CString sDir;
	_tsplitpath_s(sFullPath, sDrive.GetBuffer(_MAX_DRIVE), _MAX_DRIVE, sDir.GetBuffer(_MAX_DIR), _MAX_DIR, nullptr, 0, nullptr, 0);
	sDir.ReleaseBuffer();
	sDrive.ReleaseBuffer();
	CString sFname;
	CString sExt;
	_tsplitpath_s(lpFileName, nullptr, 0, nullptr, 0, sFname.GetBuffer(_MAX_FNAME), _MAX_FNAME, sExt.GetBuffer(_MAX_EXT), _MAX_EXT);
	sExt.ReleaseBuffer();
	sFname.ReleaseBuffer();
	_tmakepath_s(sFullPath.GetBuffer(_MAX_PATH), _MAX_PATH, sDrive, sDir, sFname, sExt);
	sFullPath.ReleaseBuffer();

	//Delegate to LoadLibrary
#pragma warning(suppress: 26487)
	return LoadLibrary(sFullPath);
}
void CMainApp::WakeUp(LPCTSTR aCommandLine) const
// Name:        WakeUp 
// Type:        Function
// Description: virtual override for wake up, so we can process the command line
{
	// Call parent class to handle the basic functionality
	CSingleInstance::WakeUp(aCommandLine);

	// Send new command line to the dialog
	theDlg* ltheDlg = (theDlg*)m_pMainWnd;
	if (ltheDlg)
	{
		ltheDlg->SetCommandLine(aCommandLine);
	}
}