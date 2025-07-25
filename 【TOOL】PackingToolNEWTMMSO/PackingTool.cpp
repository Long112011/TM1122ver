
#include "stdafx.h"
#include "PackingTool.h"
#include "PackingToolDlg.h"
//#include "sinstance.h"
BEGIN_MESSAGE_MAP(CPackingToolApp, CWinApp)	
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()
void CPackingToolApp::WakeUp(LPCTSTR aCommandLine) const
// Name:        WakeUp 
// Type:        Function
// Description: virtual override for wake up, so we can process the command line
{
	// Call parent class to handle the basic functionality
	CSingleInstance::WakeUp(aCommandLine);

	// Send new command line to the dialog
	CPackingToolDlg* lSingletonDlg = (CPackingToolDlg*)m_pMainWnd;
	if (lSingletonDlg)
	{
		lSingletonDlg->SetCommandLine(aCommandLine);
	}
}

CPackingToolApp::CPackingToolApp()
{
	//CLog::SetEnable(true);
}
CPackingToolApp theApp;
CPackingToolApp App;
CString strUserName, strPassword, FontName;
UINT FontSize, FontColor, FontWeight, SizeX, SizeY;

void CPackingToolApp::LoadRegistry(BYTE flag)
{
	FontWeight = GetProfileInt("Init", "FontWeight", 0);
	FontColor = RGBA_MAKE(0, 255, 0, 0);  // 强制绿色
	FontSize = GetProfileInt("Init", "FontSize", 0);

	FontName = GetProfileString("Init", "FontName");

	SizeX = GetProfileInt("Init", "SizeX", 0);
	SizeY = GetProfileInt("Init", "SizeY", 0);

	if (flag == 1)
	{
		CheckLoadedRegistry();
	}	
}
void CPackingToolApp::CheckLoadedRegistry()
{
	if (FontWeight == NULL)
	{
		WriteProfileInt("Init", "FontWeight", 400);
	}
	if (FontColor == NULL)
	{
		DWORD temp = RGBA_MAKE(0, 255, 0, 0);
		WriteProfileInt("Init", "FontColor", temp);
		FontColor = GetProfileInt("Init", "FontColor", 0);
	}
	if (FontSize == NULL)
	{
		WriteProfileInt("Init", "FontSize", 10);
	}
	if (strcmp(FontName, "") == 0)
	{
		WriteProfileString("Init", "FontName", "MingLiU");
	}
	if (SizeX == NULL)
	{
		WriteProfileInt("Init", "SizeX", 400);
	}
	if (SizeY == NULL)
	{
		WriteProfileInt("Init", "SizeY", 250);
	}

	LoadRegistry(1);
}

BOOL CPackingToolApp::InitInstance()
{
	
	// We use a GUID here, so that we are sure the string is 100% unique
	if (CSingleInstance::Create(_T("E435FC13-82C1-4f80-97C5-006FF4A4E302")) == FALSE)
		return FALSE;

	SetRegistryKey(_T("TrinityMs By 狂人技术"));
	LoadRegistry();

	//Check for the previous instance as soon as possible
	/*CInstanceChecker instanceChecker;
	if (instanceChecker.PreviousInstanceRunning())
	{
		AfxMessageBox(_T("Previous version detected, will now restore it"), MB_OK);
		instanceChecker.ActivatePreviousInstance();
		return FALSE;
	}*/

	/*CPackingToolDlg dlg2;
	m_pMainWnd = &dlg2;
	dlg2.DoModal();*/

	//Standard MFC stuff, create the mainframe
	/*CPackingToolDlg* pMainFrm = new CPackingToolDlg();
	if (!pMainFrm->DoModal())
		return FALSE;
	pMainFrm->ShowWindow(SW_SHOW);
	m_pMainWnd = pMainFrm;*/

	CPackingToolDlg lSingletonDlg;
	m_pMainWnd = &lSingletonDlg;
	lSingletonDlg.DoModal();

	// If this is the first instance of our App then track it so any other instances can find it.
	//if (!instanceChecker.PreviousInstanceRunning())
	//	instanceChecker.TrackFirstInstanceRunning();
	
	/*HINSTANCE hUser32 = LoadLibrary("User32.dll");		//Daily 禁止以高DPI缩放运行
	if (hUser32)
	{
		typedef BOOL(WINAPI* LPSetProcessDPIAware)(void);
		LPSetProcessDPIAware pSetProcessDPIAware = (LPSetProcessDPIAware)GetProcAddress(hUser32, "SetProcessDPIAware");
		if (pSetProcessDPIAware)
		{
			pSetProcessDPIAware();
		}
		FreeLibrary(hUser32);
	}*/
	
	//CWinApp::InitInstance();
	//AfxEnableControlContainer();
	
	//HWND hWnd;
	//HWND hedit = CreateWindow(TEXT("EDIT"), TEXT("TEST APP"), WS_VISIBLE | WS_CHILD | WS_BORDER | ES_LEFT, 0, 0, 400, 300, hWnd, 0, 0, 0);
	

	
	
	return FALSE;
}
