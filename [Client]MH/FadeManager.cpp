#include "StdAfx.h"
#include "FadeManager.h"
#include "mhFile.h"
#include "../[CC]JACK/JackJack.h"
GLOBALTON(FadeManager)
FadeManager::FadeManager()
{
	ZeroMemory(m_pChatMsg, sizeof(m_pChatMsg));
	m_NULL = 0;
	m_fademgrInitialize = false;
}
FadeManager::~FadeManager()
{
	for (int i = 0; i < MAX_CHATMSGNUM; ++i)
	{
		if (m_pChatMsg[i])
		{
			delete[] m_pChatMsg[i];
			m_pChatMsg[i] = NULL;
		}
	}
}
char* FadeManager::GetChatMsg(int nMsgNum)
{
	if (m_pChatMsg[nMsgNum])
		return m_pChatMsg[nMsgNum];
	else
		return &m_NULL;
}
void FadeManager::LoadMsgFile()
{
	char msg[512] = { 0, };
	CMHFile file;
	int nIndex;
	int nLen = 0;
	char line[256] = { 0, };
	if (!file.Init(( char*)JACKMGR->DecryptForClient("\xb8\xbe\xcf\xff\xf7\xf3\xd6\xac\xe8\xbe\xfb\xf5").c_str(), "rb"))
	{
		return;
	}
	/*while (file.IsEOF() == FALSE)
	{
		file.GetString(line);
		if (line[0] == '@')
		{
			file.GetLineX(line, 256);
			continue;
		}
		else//if (strcmp(line, "#SUBSTAGE") == 0)
		{
			//pInfo = new FAMEStruct;
			nIndex = file.GetInt();
			if (nIndex < 0 || nIndex >= MAX_CHATMSGNUM) break;
			//SafeStrCpy(msg, file.GetStringInQuotation(), MAX_NAME_LENGTH + 1);
			file.GetStringInQuotation(msg);			
		}
	}
	file.Release();*/

	while (TRUE)
	{
	//	file.GetString(line);
		
		nIndex = file.GetInt();
		if (file.IsEOF()) break;
		if (nIndex < 0 || nIndex >= MAX_CHATMSGNUM) break;
		file.GetStringInQuotation(msg);
		nLen = strlen(msg);
		if (msg[0] == '@')
		{
			file.GetLineX(line, 256);
			continue;
		}
		if (nLen > 0)
		{
			m_pChatMsg[nIndex] = new char[nLen + 1];
			memcpy(m_pChatMsg[nIndex], msg, nLen + 1);
			file.GetLine(msg, 256);
		}
	}
	file.Release();

	m_fademgrInitialize = true;
}
DWORD FadeManager::ConvertStringToDword(char* str)
{
	DWORD value = strtoul(str, NULL, 16);
	//DWORD value = atoi(str);
	sscanf(str, "%d", &value);
	char Temp[1024] = { 0, };
	sprintf(Temp, "%s", value);
	return value;// atoi(str);
}