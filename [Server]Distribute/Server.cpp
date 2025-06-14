#include "stdafx.h"
#include "ServerSystem.h"
#include "MHFile.h"
#include "../[CC]JACK/JackJack.h"

HANDLE g_hMap = NULL;
char g_szMapName[64];
void LeaveServerApplication();
BOOL EnterServerApplication();
char * GetCommandLineArg(int idx, char * szCommandLine);
char g_szHeroIDName[17] = { 0, };

BOOL CheckUpdateFile();

int APIENTRY WinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR     lpCmdLine,
                     int       nCmdShow)
{
	HINSTANCE hUser32 = LoadLibrary("User32.dll");		//Daily 禁止以高DPI缩放运行
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
	JACKMGR->Init();
	if(JACKMGR->IsVirtualMachine())//Running outside ring-3 debugger
	{
		return 0;
	}
	static BOOL bCheckTime=JACKMGR->GetTrialWeek();
	if(!bCheckTime)
	{
		//MessageBox(_g_hWnd,JACKMGR->DecryptForServer("\xc9").c_str(),"huhu!",MB_OK);
		return 0;
	}

	CreateDirectory( "./Log", NULL );

	SEND_CHARACTER_TOTALINFO ss;
	SEND_MONSTER_TOTALINFO mm;

	WORD mapnum = atoi(lpCmdLine);

	wsprintf(g_szMapName, "DistributeServer_%d", mapnum); 
	if(!EnterServerApplication()) return 0;

	//if( CheckUpdateFile() == FALSE )
	//	return 0;

	if(JACKMGR->CheckFile()==false)
		return 0;

	g_pServerSystem = new CServerSystem;
	g_pServerSystem->Start(mapnum);
	g_pServerSystem->End();
	delete g_pServerSystem;
	g_pServerSystem = NULL;
	
	LeaveServerApplication();

	return 0;
}

char * GetCommandLineArg(int idx, char * szCommandLine)
{
	char sep[]   = " ";
	char line[255];
//	strcpy(line, szCommandLine);
	SafeStrCpy( line, szCommandLine, 255 );
	char * token = strtok( line, sep );
	while( token != NULL && idx != 0)
	{
		token = strtok( NULL, sep );
		--idx;
	}
	return token;
}

BOOL EnterServerApplication()
{//taiyo
    g_hMap = CreateFileMapping((HANDLE)0xFFFFFFFF, NULL, PAGE_READWRITE, 0, 1024, g_szMapName);
    if(g_hMap != NULL && GetLastError() == ERROR_ALREADY_EXISTS) 
    { 
		CloseHandle(g_hMap); 
		g_hMap = NULL;
		return FALSE;
    }
	
	return TRUE;
}

void LeaveServerApplication()
{//taiyo
	if(g_hMap)
	{
		CloseHandle(g_hMap);
		g_hMap = NULL;
	}	
}

BOOL CheckUpdateFile()
{
	CMHFile file;
	char temp[256] = {0,};

	if( !file.Init( "./Resource/Server/TitanServer.bin", "rb" ) )
		return FALSE;

	file.GetStringInQuotation( temp );

	if( strcmp( temp, "lol" ) != 0 )
		return FALSE;

	file.Release();

	return TRUE;
}
