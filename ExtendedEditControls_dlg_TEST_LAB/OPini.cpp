#include "stdafx.h"
#include ".\opini.h"
COPini::COPini()
{
} 

COPini::~COPini()
{

}

BOOL COPini::WriteString(LPCTSTR section, LPCTSTR key, char *stringtoadd, char *filename)
{
    CHAR FilePath[255]; 
    GetModuleFileName(NULL,FilePath,255); 
    //Scan a string for the last occurrence of a character.
    (strrchr(FilePath,'\\'))[1] = 0; 
    strcat(FilePath,filename);
    return ::WritePrivateProfileString(section,key,stringtoadd,FilePath);
} 


DWORD COPini::ReadString(char *section, char * key,  char stringtoread[],  char * filename)
{
    CHAR FilePath[255]; 
    GetModuleFileName(NULL,FilePath,255); 
    (strrchr(FilePath,'\\'))[1] = 0; 
    strcat(FilePath,filename);
    return ::GetPrivateProfileString(section, key,NULL,stringtoread,255,FilePath);
} 

CString COPini::Load(char *section, char * key, char stringtoread[], char * defaultStr)
{
	int rt = 999;
	rt = COPini::ReadString(section, key, stringtoread, _PlayerAI_Config_);
	if (0 == rt)
	{
		//memset(stringtoread, 0, strlen(stringtoread));
		COPini::WriteString(section, key, defaultStr, _PlayerAI_Config_);
		SafeStrCpy(stringtoread, defaultStr, strlen(defaultStr) + 1);
	}
	return stringtoread;
}
BOOL COPini::Set(LPCTSTR section, LPCTSTR key, char *stringtoadd/*, char *filename*/)
{
	LPCTSTR pszA = section;
	CString sB(pszA);
	const char* pszC = sB;
	char* pszD = const_cast<char*>(pszC);

	LPCTSTR pszB = key;
	CString sC(pszB);
	const char* pszE = sC;
	char* pszF = const_cast<char*>(pszE);

	char stringtoread[200];
	memset(stringtoread, 0, 200);
	char buff[256];
	COPini::ReadString(pszD, pszF, stringtoread, _PlayerAI_Config_);
	if (strcmp(stringtoread, stringtoadd) != 0)
	{
		//AfxMessageBox("!=0");

		COPini::WriteString(section, key, stringtoadd, _PlayerAI_Config_);
	}
	else
	{
		//sama

		//AfxMessageBox("else !=0");
	}
	return 0;
}














/*COLORREF COPini::LoadXmlandDefaultLong(CString cstrKey, CString cstrValueName, long	cstrValue)
{
	xmlSettings.SetSettingsFile(SAVED_XML_FILE);
	return xmlSettings.GetSettingLong(cstrKey, cstrValueName, cstrValue);
}
CString COPini::LoadXmlandDefaultString(CString cstrKey, CString cstrValueName, char*  cstrRetVal)
{
	xmlSettings.SetSettingsFile(SAVED_XML_FILE);
	return xmlSettings.GetSettingString(cstrKey, cstrValueName, cstrRetVal);
}
void COPini::SetXmlNewValue(CString cstrKey, CString cstrValueName, char*  cstrRetVal)
{
	long lRetVal = 0;
	xmlSettings.SetSettingsFile(SAVED_XML_FILE);
	lRetVal = xmlSettings.SetSettingString(cstrKey, cstrValueName, cstrRetVal);
}
void COPini::SetXmlNewValueInt(CString cstrKey, CString cstrValueName, int cstrRetVal)
{
	long lRetVal = 0;
	xmlSettings.SetSettingsFile(SAVED_XML_FILE);
	lRetVal = xmlSettings.SetSettingLong(cstrKey, cstrValueName, cstrRetVal);
}*/