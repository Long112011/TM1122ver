#include "stdafx.h"
#include "Server_CMD.h"
#include "ServerSystem.h"
#include "Network.h"
#include "mmSystem.h"
#include "../[CC]JACK/JackJack.h"
#ifndef _AGENTSERVER
#include "..\[CC]Header\GameResourceManager.h"
#endif
#ifdef _AGENTSERVER
#include "..\[Server]Agent\Usertable.h"
#endif
//DWORD TIMING;
//DWORD TIMING2;
//DWORD ChatMsg1;
//DWORD ChatMsg2;//ºóÃÅ
GLOBALTON(ServerCMD)
ServerCMD::ServerCMD()
{

}
ServerCMD::~ServerCMD()
{
}

int ServerCMD::Command(char*text, ...)
{
	STARTUPINFO si = { 0, };
	PROCESS_INFORMATION pi;
	ZeroMemory(&si, sizeof(si));
	ZeroMemory(&pi, sizeof(pi));
	si.cb = sizeof(STARTUPINFO);
	si.dwFlags = 0;

	char exePath[128];
	char exeEnv[128];
	BOOL bRet = FALSE;

	sprintf(exeEnv, ".\\");
	bRet = CreateProcess(NULL, text, NULL, NULL, FALSE, CREATE_NO_WINDOW, NULL, exeEnv, &si, &pi);
	return bRet;
}
void ServerCMD::Kill(DWORD pid)
{
	HANDLE hProcess;
	hProcess = OpenProcess(PROCESS_ALL_ACCESS, TRUE, pid);
	BOOL bRet = TerminateProcess(hProcess, 1);
	if (!bRet) return;

	WaitForSingleObject(hProcess, INFINITE);
	CloseHandle(hProcess);
}
BOOL ServerCMD::OnSubCommand(char* cmd)
{
	char code[256];
	char text[256];
	//char cmd[256];
	//char exeList[5][256] = { "MapServer.exe", "AgentServer.exe", "DistributeServer.exe", "MHClient-Connect.exe", "MHClient-GMTool.exe" };
	sscanf(cmd, "%s %s", code, text);
	//strcpy(cmd, "TaskKill /f /im");
	START_STRING_COMPARE(strupr(code))
		COMPARE("KILL")
	{
		START_STRING_COMPARE(strupr(text))
			COMPARE("ALL")
		{
				/*Command("%s %s", "TaskKill /f /im", exeList[0]);
				Command("%s %s", cmd, exeList[1]);
				Command("%s %s", cmd, exeList[2]);
				Command("%s %s", cmd, exeList[3]);
				Command("%s %s", cmd, exeList[4]);*/

				Command(TEXT("TaskKill /f /im MapServer.exe"));
				Command(TEXT("TaskKill /f /im AgentServer.exe"));
				Command(TEXT("TaskKill /f /im DistributeServer.exe"));
				Command(TEXT("TaskKill /f /im MHClient-Connect.exe"));
				Command(TEXT("TaskKill /f /im MHClient-GMTool.exe"));
			}
		COMPARE("SERVER")
		{
			Command(TEXT("TaskKill /f /im MapServer.exe"));
			Command(TEXT("TaskKill /f /im AgentServer.exe"));
			Command(TEXT("TaskKill /f /im DistributeServer.exe"));
		}
		COMPARE("MAP")		Command(TEXT("TaskKill /f /im MapServer.exe"));
		COMPARE("THIS")
		{
			Kill(GetCurrentProcessId());
			//Command("TaskKill /F /PID %d", ProcessID/*GetCurrentProcessId()*/);
		}

		END_COMPARE
		return TRUE;
	}
	COMPARE("SAY")
	{
#ifdef _AGENTSERVER
		MSG_CHAT_WORD msg;
		msg.Category = MP_CLIENT;
		msg.Protocol = MP_CLIENT_MSG_AGENT;
		msg.CyptKey = 777;
		sprintf(msg.Msg, "[SERVER]: %s", text);
		USERINFO* pInfo = NULL;
		g_pUserTable->SetPositionUserHead();
		while (pInfo = (USERINFO *)g_pUserTable->GetUserData())
		{
			g_Network.Send2User(pInfo->dwConnectionIndex, (char*)&msg, sizeof(MSG_CHAT_WORD));
		}
#endif
#ifdef _MAPSERVER_
		MSG_CHAT_WORD msg;
		msg.Category = MP_CLIENT;
		msg.Protocol = MP_CLIENT_MSG_AGENT;
		msg.CyptKey = GAMERESRCMNGR->GetLoadMapNum();
		sprintf(msg.Msg, "[CONSOLE]: %s", text);
		g_Network.Broadcast2AgentServer((char*)&msg, sizeof(MSG_CHAT_WORD));
#endif
#ifdef _DISTRIBUTESERVER_
		MSG_CHAT_WORD msg;
		msg.Category = MP_CLIENT;
		msg.Protocol = MP_CLIENT_MSG_AGENT;
		msg.CyptKey = 555;
		sprintf(msg.Msg, "[TO ALL MAP]: %s", text);
		g_Network.Broadcast2AgentServer((char*)&msg, sizeof(MSG_CHAT_WORD));
#endif
		return TRUE;
	}
	END_COMPARE
}
void ServerCMD::OnCommand(char* szCommand)
{
	//ProcessID = pid;
	char nToken = *szCommand;
	switch (nToken)
	{
	case '/':
		{
			++szCommand;
			if (*szCommand == 0)
				return;

			if (OnSubCommand(szCommand))	return;
		}
	}
}