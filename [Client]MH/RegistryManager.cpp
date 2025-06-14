#include "StdAfx.h"
#include "RegistryManager.h"
#include "mhFile.h"
#include "OptionManager.h"
//#define REGISTRY_PATH "Software\\TrinityMs By JACK"
#include "../[CC]JACK/JackJack.h"
#include "ChatManager.h"
#include "WindowIDEnum.h"
#include "cWindowManager.h"
#include "OptionDialog.h"
extern HWND _g_hWnd;
extern char g_CLIENTVERSION[];
extern char g_CLIENTREGISTRY[];
GLOBALTON(RegManager)
RegManager::RegManager()
{
}
RegManager::~RegManager()
{
}
void RegManager::Init()
{
//	srand((unsigned)time(NULL));
}
HKEY RegManager::OpenKey(HKEY hRootKey, char* strKey)
{
	HKEY hKey;
	LONG nError = RegOpenKeyEx(hRootKey, strKey, NULL, KEY_ALL_ACCESS | KEY_WOW64_32KEY, &hKey);

	if (nError == ERROR_FILE_NOT_FOUND)
	{
		nError = RegCreateKeyEx(hRootKey, strKey, NULL, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hKey, NULL);
	}

	if (nError)
	{
		return 0;
	}
	return hKey;
}
char * RegManager::registry_read(LPCTSTR subkey, LPCTSTR name, DWORD type)
{
	HKEY key;
	TCHAR value[255];
	DWORD value_length = 255;
	RegOpenKeyEx(HKEY_LOCAL_MACHINE, subkey, NULL, KEY_ALL_ACCESS | KEY_WOW64_32KEY, &key);
	LONG openRes = RegQueryValueEx(key, name, NULL, &type, (LPBYTE)&value, &value_length);

	if (openRes == ERROR_SUCCESS)
	{
		return value;
	}
	else
	{
		return "";
	}

	RegCloseKey(key);
}
void RegManager::registry_write(LPCTSTR subkey, LPCTSTR name, DWORD type, const char* value)
{
	HKEY key;
	RegOpenKeyEx(HKEY_LOCAL_MACHINE, subkey, NULL, KEY_ALL_ACCESS | KEY_WOW64_32KEY, &key);
	RegSetValueEx(key, name, 0, type, (LPBYTE)value, strlen(value)*sizeof(char));
	RegCloseKey(key);
}


//---------------------------------------------------------------------------

//g_CLIENTVERSION
void RegManager::ReadConfig()
{
	HKEY hKey = REGMGR->OpenKey(HKEY_LOCAL_MACHINE, g_CLIENTREGISTRY);

	if (hKey == NULL)
	{		
		return ;
	}
	bool bOpen = false;	

	char buff[256];
	sprintf(buff, "%s CurrentVersion", JACKMGR->DecryptForServer(ClientVersion).c_str());
	SafeStrCpy(CurrentVersion, REGMGR->registry_read(g_CLIENTREGISTRY, buff, REG_SZ), MAX_NAME_LENGTH + 1);
	
	if (strcmp(CurrentVersion, g_CLIENTVERSION /*JACKMGR->GetFullVersion()*/) == 0)
	{
		//same version
		//MessageBox(_g_hWnd, "same version", "1", MB_OK);		
	}
	else
	{		
		//wrong version
		//MessageBox(_g_hWnd, "wrong version", "1", MB_OK);
		WriteConfig(g_CLIENTVERSION/*JACKMGR->GetFullVersion()*/);

		CHATMGR->AddMsg(CTC_KILLED, "System detect client version has changed since last time you run this client");
		
		m_pOptionDlg = (COptionDialog*)WINDOWMGR->GetWindowForID(OTI_TABDLG);
		m_pOptionDlg->SetActive(TRUE);
		
		OPTIONMGR->SetDefaultOption();
		
	}
	RegCloseKey(hKey);
	return ;
}
void RegManager::WriteConfig(char * version)
{
	char stringtoread[200];
	memset(stringtoread, 0, 200);

	char buff[256];
	SafeStrCpy(stringtoread, version, MAX_NAME_LENGTH + 1);
	sprintf(buff, "%s CurrentVersion", JACKMGR->DecryptForServer(ClientVersion).c_str());
	REGMGR->registry_write(g_CLIENTREGISTRY, buff, REG_SZ, stringtoread);
	return ;
}
//---------------------------------------------------------------------------