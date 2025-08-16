#include "stdafx.h"
#include "MHClient.h"
#include "reconnect.h"
#include "AIManager.h"
#include "InstancesDungeon/InsDungeonManager.h"
#define _LOGOWINDOW_
HANDLE g_hMap = NULL;
HINSTANCE g_hInst;								
HWND _g_hWnd;
TCHAR szTempTitle[MAX_LOADSTRING];
TCHAR szTitle[MAX_LOADSTRING];				
TCHAR szWindowClass[MAX_LOADSTRING];	
BOOL g_bActiveApp = FALSE;
char g_AgentAddr[16];
WORD g_AgentPort;
char g_szHeroIDName[17];
char g_szHeroKey[17];
char g_szHeroChar[4];
int	g_nServerSetNum;
bool g_ThisIsFirstLogin;
int m_CameraPos;
HIMC g_hIMC = NULL;
BOOL m_SafeIconShow=TRUE;
BOOL jTweak=FALSE;
BOOL _debug = FALSE;
BOOL _hantar = FALSE;
BOOL IsMultiPet = FALSE;
BOOL m_DebugEnabled, m_VsyncTest,m_DebugEnabledAll,m_DebugAutoReconnect;
int  m_PetIdx =-1; // 
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK	About(HWND, UINT, WPARAM, LPARAM);
void LeaveClientApplication();
BOOL EnterClientApplication();
#include "splash.h"
#include "DesktopNotify.h"
SPLASH mysplash;
CDesktopNotify myNotify;
BOOL g_ShowState = TRUE;
BOOL g_NoLogo;
void ResumeGameSpeed();
void GetDesktopResolution(int& horizontal, int& vertical)
{
	RECT desktop;
	const HWND hDesktop = GetDesktopWindow();
	GetWindowRect(hDesktop, &desktop);
	horizontal = desktop.right;
	vertical = desktop.bottom;
}
bool AnimateNextFrame(int desiredFrameRate)
{
	static float lastTime = GetTickCount() * 0.001f;
	static float elapsedTime = 0.0f;
	float currentTime = GetTickCount() * 0.001f;
	float deltaTime = currentTime - lastTime;
	float desiredFPS = 1.0f / desiredFrameRate;
	elapsedTime += deltaTime;
	lastTime = currentTime;
	if (elapsedTime > desiredFPS)
	{
		elapsedTime -= desiredFPS;
		return true;
	}
	return false;
}
#include "MHTimeManager.h"
double CurFPS;
class locale;
#include <windows.h>
#include <cstdio>
#include <ctime>
#include <cwchar>
#include <string>
#include <vector>
#include <locale>
#include <clocale>
#include <iostream>
#include <time.h>
#include <stdio.h>
#include <process.h>
#include <fstream>
#define BUFF_SIZE 100

int get_date(unsigned char* str)
{
	__time64_t ltime;
	struct tm  thetime;
	_time64(&ltime);
	_gmtime64_s(&thetime, &ltime);
	if (!strftime((char *)str, BUFF_SIZE, "%#x",
		(const struct tm *)&thetime))
	{
		printf("strftime failed!\n");
		return -1;
	}
	return 0;
}
uintptr_t __stdcall SecondThreadFunc(void* pArguments)
{
	unsigned char str[BUFF_SIZE];
	char * locale = (char *)pArguments;
	printf("The thread locale is now set to %s.\n",
		setlocale(LC_ALL, locale));
	if (get_date(str) == 0)
	{
		printf("The date in %s locale is: '%s'\n", locale, str);
	}
	_endthreadex(0);
	return 0;
}
std::string ConvertWideToANSI(const std::wstring& wstr)
{
	int count = WideCharToMultiByte(CP_ACP, 0, wstr.c_str(), wstr.length(), NULL, 0, NULL, NULL);
	std::string str(count, 0);
	WideCharToMultiByte(CP_ACP, 0, wstr.c_str(), -1, &str[0], count, NULL, NULL);
	return str;
}
std::wstring ConvertAnsiToWide(const std::string& str)
{
	int count = MultiByteToWideChar(CP_ACP, 0, str.c_str(), str.length(), NULL, 0);
	std::wstring wstr(count, 0);
	MultiByteToWideChar(CP_ACP, 0, str.c_str(), str.length(), &wstr[0], count);
	return wstr;
}
std::string ConvertWideToUtf8(const std::wstring& wstr)
{
	int count = WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), wstr.length(), NULL, 0, NULL, NULL);
	std::string str(count, 0);
	WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), -1, &str[0], count, NULL, NULL);
	return str;
}
std::wstring ConvertUtf8ToWide(const std::string& str)
{
	int count = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), str.length(), NULL, 0);
	std::wstring wstr(count, 0);
	MultiByteToWideChar(CP_UTF8, 0, str.c_str(), str.length(), &wstr[0], count);
	return wstr;
}
int setenv(const char *name, const char *value, int overwrite)
{
	int errcode = 0;
	if (!overwrite) {
		size_t envsize = 0;
		errcode = getenv_s(&envsize, NULL, 0, name);
		if (errcode || envsize) return errcode;
	}
	return _putenv_s(name, value);
}
int framelimit;
int APIENTRY WinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR     lpCmdLine,
                     int       nCmdShow)
{
	g_NoLogo = FALSE;
	g_ShowState = TRUE;

	

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
	framelimit = 920;
	m_DebugEnabled= m_VsyncTest=m_DebugEnabledAll = m_DebugAutoReconnect=FALSE;
	g_ThisIsFirstLogin=true;
	BOOL mapHandle = FALSE;
	if (!EnterClientApplication())
		mapHandle = TRUE;
	JACKMGR->Init();
	if(JACKMGR->IsVirtualMachine())
	{		
		exit(0);
		return 0;
	}
	if(!JACKMGR->GetTrialWeek())
	{
		exit(0);
		return 0;
	}
	jTweak=FALSE;
	_debug = FALSE;
	_hantar = FALSE;
	FILE* fp = NULL;
	std::string str;
	
	fp = fopen("_NOLOGO_", "r");
	if (fp)
	{
		g_NoLogo = TRUE;
		fclose(fp);
	}
	fp = fopen(JACKMGR->DecryptForClient("\xc9").c_str(),"r");
	if(fp)
	{
		jTweak=TRUE;
		fclose(fp);
	}
	fp = fopen(JACKMGR->DecryptForClient("\xd5\xab\xd5\xc1").c_str(), "r");
	if (fp)
	{
		jTweak = TRUE;
		fclose(fp);
	}
	str = JACKMGR->DecryptForClient("\xb8\xcd\xd5\xc1\xc3\xdf\xd5\x98").c_str();
	const char * a = str.c_str();
	/*fp = fopen(a, "r");//	.\\_PING
	if (fp)
	{
		_hantar = TRUE;

		char stringtoread[255];

		::GetPrivateProfileString("0", "1", NULL, stringtoread, 255, a);
		HANTARMGR->SetSenderHost(stringtoread);

		::GetPrivateProfileString("0", "2", NULL, stringtoread, 255, a);
		HANTARMGR->SetSenderPort(stringtoread);

		fclose(fp);
		str = '\0';
	}*/
	fp = fopen(JACKMGR->DecryptForClient("\xc9\xf5\xec\xfc\xe6\xf1").c_str(), "r");
	if (fp)
	{
		_debug = TRUE;
		fclose(fp);
	}
	str = JACKMGR->DecryptForClient("\xb8\xcd\xd5\xc1\xf2\xe3\xef\xb0\xfd\xf5\xf5\xfe\xe5\xf4\xe8\x9c\xe2").c_str();	
	const char * c = str.c_str();
	fp = fopen(c, "r");
	if (fp)
	{
		m_DebugAutoReconnect = TRUE;
		if (m_DebugAutoReconnect)
		{
			char stringtoread[255];

			REDISCONN->SetServerIdx(::GetPrivateProfileInt("REDISCONN", "ServerIdx", 0, c));

			::GetPrivateProfileString("REDISCONN", "UserName", NULL, stringtoread, 255, c);
			REDISCONN->SetUserName(stringtoread);

			::GetPrivateProfileString("REDISCONN", "UserPassword", NULL, stringtoread, 255, c);
			REDISCONN->SetUserPwd(stringtoread);

			::GetPrivateProfileString("REDISCONN", "UserPin", NULL, stringtoread, 255, c);
			REDISCONN->SetUserPin(stringtoread);

			REDISCONN->SetCharIdx(::GetPrivateProfileInt("REDISCONN", "CharIdx", 0, c));

			REDISCONN->StartProcess(1);
		}
		fclose(fp);
	}

	FADEMGR->LoadMsgFile();















	DeleteFile("./Log/Debug.txt");
	CreateDirectory("Log",NULL);
	GAMERESRCMNGR->LoadGameDesc();
	strcpy(g_CLIENTVERSION,JACKMGR->GetFullVersion());
	strcpy(g_CLIENTREGISTRY, JACKMGR->GetFullRegistry());
	sprintf(szTitle,"%s %s [PID_%d]", GAMERESRCMNGR->m_GameDesc.strWindowTitle,g_CLIENTVERSION,GetCurrentProcessId());
#ifdef _GMTOOL_
	CHEATMGR->SetCheatEnable( TRUE );
	strcat(szTitle," [GM_TOOL]");
#endif
	if (jTweak)
	{
		strcat(szTitle, "**");
	}
	strcpy(szWindowClass, szTitle);
	MyRegisterClass(hInstance);
	QUESTMGR->LoadQuestString( "./Resource/QuestScript/QuestString.bin" );
	QUESTMGR->LoadQuestScript( "./Resource/QuestScript/QuestScript.bin" );
	QUESTMGR->LoadQuestNpc( "./Resource/QuestScript/QuestNpcList.bin" );
	SCRIPTMGR->InitScriptManager();
	INSDUNGEONMGR->Init();
	if (!InitInstance (hInstance, nCmdShow)) 
	{
		LeaveClientApplication();
		return FALSE;
	}
	MSG msg;
	while (1) 
    {
		if(PeekMessage(&msg,NULL,0,0,PM_REMOVE))
		{
#ifdef _GMTOOL_
			if( !GMTOOLMGR->IsGMDialogMessage(&msg) )
#endif
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
		else
		{
				if (MAINGAME->Process() == -1)
					break;
		}		
	}
	LeaveClientApplication();
	return msg.wParam;
}
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;
	wcex.cbSize = sizeof(WNDCLASSEX); 
	wcex.style			= CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;  
	wcex.lpfnWndProc	= (WNDPROC)WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon( hInstance, MAKEINTRESOURCE(IDI_GAMEICON) );
	wcex.hCursor		= NULL;
	wcex.hbrBackground	= (HBRUSH)GetStockObject(BLACK_BRUSH);
	wcex.lpszMenuName	= NULL;
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= NULL;
	return RegisterClassEx(&wcex);
}
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	CURSOR->Init( hInstance );
	HWND hWnd;
	g_hInst = hInstance;
	int horizontal = 0;
	int vertical = 0;
	GetDesktopResolution(horizontal, vertical);
	float xx, yy;
	xx = ((float)horizontal / 2 - GET_MAINWIN_W / 2);
	yy = ((float)vertical / 2 - GET_MAINWIN_H / 2);
	int x = GetSystemMetrics(SM_CXSIZEFRAME) * 4.5;
	int y = GetSystemMetrics(SM_CYSIZEFRAME) * 4.5 + GetSystemMetrics(SM_CYCAPTION);//
	DWORD dwStyle = WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX;
	DWORD width		= 0 ;
	DWORD height	= 0 ;
	DISPLAY_INFO	dispInfo ;
	GAMERESRCMNGR->GetDispInfo( &dispInfo ) ;
	width	= dispInfo.dwWidth ;
	height	= dispInfo.dwHeight ;
	int nUserScreenWidth = GetSystemMetrics(SM_CXSCREEN);
	_g_hWnd = hWnd = CreateWindow(szWindowClass, szTitle, dwStyle, 
		xx, yy,
		width+x, height+y, 
		NULL, NULL, hInstance, NULL);
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);
	ShowCursor(TRUE);
	CoInitialize(NULL);
	MAINGAME->Init(hWnd);
	MAINGAME->SetGameState(eGAMESTATE_CONNECT);
	g_pExecutive->GetRenderer()->SetVerticalSync(OPTIONMGR->GetGameOption()->bVerticalSync);
#ifdef _GMTOOL_
		GMTOOLMGR->CreateGMDialog();
		GMTOOLMGR->ShowGMDialog(TRUE, TRUE);
#endif
	SetFocus(hWnd);
	return TRUE;
}
#define CURSOR_SPEED 20
void MoveCursorWithJoystick(LPARAM lParam)
{
	POINT cursorPos;
	int x = 0, y = 0;
	GetCursorPos(&cursorPos);
	x = LOWORD(lParam) >> 12;
	y = HIWORD(lParam) >> 12;
	if (x == 0)
		cursorPos.x -= CURSOR_SPEED;
	else if (x == 15)
		cursorPos.x += CURSOR_SPEED;
	if (y == 0)
		cursorPos.y -= CURSOR_SPEED;
	else if (y == 15)
		cursorPos.y += CURSOR_SPEED;
	SetCursorPos(cursorPos.x, cursorPos.y);
}
BOOL InitJoystick(HWND hWnd)
{
	DWORD dwResult = 0;
	if (!joyGetNumDevs())
	{
		MessageBox(NULL, "There are no joystick devices installed.", "Error", MB_OK | MB_ICONEXCLAMATION);
		return FALSE;
	}
	dwResult = joySetCapture(hWnd, JOYSTICKID1, 0, FALSE);
	switch (dwResult)
	{
	case JOYERR_UNPLUGGED:
		MessageBox(hWnd, "Please plug in the joystick first.", NULL, MB_OK | MB_ICONEXCLAMATION);
		return FALSE;
	case MMSYSERR_NODRIVER:
		MessageBox(hWnd, "No valid joystick driver.", NULL, MB_OK | MB_ICONEXCLAMATION);
		return FALSE;
	case JOYERR_NOCANDO:
		MessageBox(hWnd, "Couldn't capture joystick input, try restarting.", NULL, MB_OK | MB_ICONEXCLAMATION);
		return FALSE;
	}
	return TRUE;
}
BOOL bAllowToShowNotify;
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	MOUSE->GetWindowMessage(hWnd, message, wParam, lParam);
	CHATMGR->MsgProc( message, wParam );
	CIMEWND->OnMsg(hWnd, message, wParam, lParam);
	switch (message) 
	{
	case WM_CHAR:
	case WM_IME_CHAR:
	case WM_IME_STARTCOMPOSITION:		
		return 0;		
	case WM_ACTIVATEAPP:
		{
			g_bActiveApp = (BOOL)wParam;
			if( !g_bActiveApp )
			{
				AUDIOMGR->SetBGMVolume(0);
				AUDIOMGR->SetSoundVolume(0);
				bAllowToShowNotify=true;
				CIMEWND->SetCtrlPushed( FALSE );
			}
			else
			{
				if (OPTIONMGR->GetGameOption()->bSoundBGM)
				{
					AUDIOMGR->SetBGMVolume((float)OPTIONMGR->GetGameOption()->nVolumnBGM / 100.0f);
				}
				if (OPTIONMGR->GetGameOption()->bSoundEnvironment)
				{
					AUDIOMGR->SetSoundVolume((float)OPTIONMGR->GetGameOption()->nVolumnEnvironment / 100.0f);
				}
				bAllowToShowNotify=false;
			}
		}
		break;
	case WM_ACTIVATE:
		if( g_UserInput.GetKeyboard()->GetDirectInput() )
		{
			if( WA_INACTIVE != wParam )
			{
				g_UserInput.GetKeyboard()->ClearKeyState();
				g_UserInput.GetKeyboard()->UpdateKeyboardState();
				OBJECTMGR->SetAllNameShow( FALSE );
			}
		}
		break;
	case WM_USER+155:
		{
			ISC_BaseNetwork* lpNet = NETWORK->GetBaseNetwork();
			if(lpNet)
				lpNet->SocketEventProc();
		}
		return TRUE;		
	case WM_CLOSE:
		{
			if (MAINGAME->GetUserLevel() <= eUSERLEVEL_GM)
			{
				MAINGAME->EndGame();
				break;
			}
			if( EXITMGR->IsExitWaiting() )
				return 0;
			if( MAINGAME->GetCurStateNum() != eGAMESTATE_GAMEIN  )
			{
				MAINGAME->EndGame();
				break;
			}
			else
			{
				WINDOWMGR->MsgBox( MBI_EXIT, MBT_YESNO, CHATMGR->GetChatMsg( 280 ) );
				return 0;
			}
		}
		break;
	case WM_SETFOCUS:
		{ 
			CURSOR->Process();
		}
		return 0;
	case WM_SETCURSOR:
		{ 
			CURSOR->Process();			
		}
		break;	
	case WM_SYSKEYDOWN:	
		{
			if( wParam == VK_F4 )
			{
				if (MAINGAME->GetCurStateNum() == eGAMESTATE_GAMEIN &&MAINGAME->GetUserLevel() <= eUSERLEVEL_GM)
				{
					exit(0);
					break;
				}
				if( MAINGAME->GetCurStateNum() == eGAMESTATE_TITLE )
				{
					break;	
				}
				else if( MAINGAME->GetCurStateNum() == eGAMESTATE_CHARSELECT ||
					MAINGAME->GetCurStateNum() == eGAMESTATE_CHARMAKE ||
					MAINGAME->GetCurStateNum() == eGAMESTATE_GAMEIN )
				{
					WINDOWMGR->MsgBox( MBI_EXIT, MBT_YESNO, CHATMGR->GetChatMsg( 280 ) );
					return 0;
				}
			}
		}
		return 0;
	case WM_SYSCOMMAND:
		{
			if( wParam == SC_KEYMENU )
				return 0;
		}
		break;
	case WM_SYSKEYUP:	
		{
		}
		return 0;
#ifdef _GMTOOL_
	case WM_MOVE:
		GMTOOLMGR->SetPositionByMainWindow();
		break;
#endif
	case WM_LBUTTONDOWN:
		if (mysplash.SHOWING)
		{
			mysplash.Hide();
		}
		break;
	case WM_CREATE:
		{
			if (FALSE==g_NoLogo)
			{
				mysplash.Init(hWnd, g_hInst/*, IDB_BITMAP1*/);
				mysplash.Show();
				::Sleep(5000);
				mysplash.Hide();

				//myNotify.Init(hWnd, g_hInst, IDB_BITMAP1);
				//myNotify.Show();
			}
			
#ifdef _GMTOOL_
			INITCOMMONCONTROLSEX icex;
			icex.dwSize = sizeof( icex );
			icex.dwICC	= ICC_DATE_CLASSES;
			InitCommonControlsEx( &icex );
#endif
		}
		break;
	case WM_HOTKEY:
		if (wParam == 0x7777)
		{
			g_ShowState = !g_ShowState;
			if (g_ShowState)ShowWindow(_g_hWnd, SW_SHOWNORMAL);
			else ShowWindow(_g_hWnd, SW_HIDE);
			DWORD err = 0;
			err = GetLastError();
			err = 0;
		}
		break;
	case MM_JOY1BUTTONDOWN:
		g_bActiveApp = (BOOL)wParam;
		if (g_bActiveApp)
		{
			break;
	case MM_JOY1MOVE:
		MoveCursorWithJoystick(lParam);
		break;
		}
	}
	return DefWindowProc(hWnd, message, wParam, lParam);
}
extern void ResumeGameSpeed()
{
	WORD* byte_pos = (WORD*)::timeGetTime;
	if (*byte_pos == 0x3D83) // cmp [83,3D] ЦёБо
		return;
	HANDLE hMyHandle = ::OpenProcess(PROCESS_VM_OPERATION | PROCESS_VM_WRITE, FALSE, ::GetCurrentProcessId());
	if (hMyHandle)
	{
		BYTE cmp_[7] = { 0x83, 0x3D, 0x14, 0x00, 0xB3, 0x76, 0x00 };
		::WriteProcessMemory(hMyHandle, (BYTE*)byte_pos, cmp_, 7, 0);
		::CloseHandle(hMyHandle);
	}
}
LRESULT CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
		case WM_INITDIALOG:
				return TRUE;
		case WM_COMMAND:
			if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL) 
			{
				EndDialog(hDlg, LOWORD(wParam));
				return TRUE;
			}
			break;
	}
    return FALSE;
}
BOOL EnterClientApplication()
{
    g_hMap = CreateFileMapping((HANDLE)0xFFFFFFFF, NULL, PAGE_READONLY, 0, 16, "MHOnline Client Application");
#ifdef _GMTOOL_
	return TRUE;
#endif
	if(g_hMap != NULL && GetLastError() == ERROR_ALREADY_EXISTS) 
    { 
		CloseHandle(g_hMap); 
		g_hMap = NULL; 
		return FALSE;
    }	
	if( GetLastError() == ERROR_ALREADY_EXISTS )
	{	
		return FALSE;
	}
	return TRUE;
}
void LeaveClientApplication()
{
	if(g_hMap)
	{
		CloseHandle(g_hMap);
		g_hMap = NULL;
	}	
}
#ifdef _NPROTECT_
BOOL CALLBACK NPGameMonCallback(DWORD dwMsg, DWORD dwArg)
{
	return NPROTECTMGR->MsgProc(dwMsg, dwArg);
}
#endif
BOOL InitResolution(HWND bhWnd, int w, int h, bool bFlag);
BOOL InitResolution(HWND bhWnd, int w, int h, bool bFlag)
{
	DWORD dwStyle = WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX;
	DWORD dwStyle2 = WS_OVERLAPPED |   WS_MINIMIZEBOX;
	RECT rect;
	rect.left = 0;
	rect.top = 0;
	rect.right = WINDOWMGR->GetScreenX();// m_DpiSet->x;
	rect.bottom = WINDOWMGR->GetScreenY();// m_DpiSet->y;
	if (bFlag == false)
	{
		SetWindowLongPtr(bhWnd, GWL_STYLE,dwStyle2);
		MoveWindow(bhWnd, 0, 0, w - rect.left, h - rect.top, TRUE);
		GAMERESRCMNGR->SetNewResolution(w, h, false);
	}
	else
	{
		SetWindowLongPtr(bhWnd, GWL_STYLE, dwStyle);
		AdjustWindowRect(&rect, dwStyle/*WS_CAPTION | WS_POPUPWINDOW*/, FALSE);
		MoveWindow(bhWnd, 0, 0, rect.right - rect.left, rect.bottom - rect.top, TRUE);
		GAMERESRCMNGR->SetNewResolution(w, h, true);
	}
	ShowWindow(bhWnd, 1);
	UpdateWindow(bhWnd);
	ShowCursor(TRUE);
	CoInitialize(NULL);
	g_pExecutive->GetGeometry()->UpdateWindowSize();
	return true;
}