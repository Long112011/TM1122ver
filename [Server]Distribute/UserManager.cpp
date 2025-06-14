// UserManager.cpp: implementation of the CUserManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "UserManager.h"
#include "Network.h"
#include "MHFile.h"
#include "../[CC]JACK/JackJack.h"
//#include "../[CC]JACK/JackHeaderStruct.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CUserManager gUserMGR;
extern int g_nServerSetNum;
/*std::string damn2(const std::string& input)
{
  
  const size_t passwordLength = 16;
  
  static const char password[passwordLength] = "invalid pointer";

  
  std::string result = input;
  for (size_t i = 0; i < input.length(); i++)
    result[i] ^= ~password[i % passwordLength];
  return result;
}*/
CUserManager::CUserManager()
{
	m_LimitUserLevel = eUSERLEVEL_GM;

	LoadVersion();
	m_LimitAge = 0;

	//JACKMGR->Init();
}

CUserManager::~CUserManager()
{

}
void CUserManager::SetVersion(char *szV)
{
	if(szV[0] != 0)
		strcpy(CLIENT_VERSION, szV);
}
BOOL CUserManager::IsValidVersion(char * szVersion)
{
	if(strcmp(CLIENT_VERSION, szVersion) == 0)
		return TRUE;
	return FALSE;
}
void CUserManager::LoadVersion()
{
	CMHFile file;
	DeleteFile("./ClientVerInfo.ver");
	if( !file.Init("./ClientVerInfo.ver", "r", MHFILE_FLAG_DONOTDISPLAY_NOTFOUNDERROR ) )
	{
		strcpy(CLIENT_VERSION, JACKMGR->GetFullVersion());
		file.Release();

		SaveVersion();
		return;
	}
	strcpy(CLIENT_VERSION, file.GetString());
	file.Release();


	//strcpy(CLIENT_VERSION, JACKMGR->GetFullVersion());
}
void CUserManager::SaveVersion()
{
	CMHFile file;
	file.Init("./ClientVerInfo.ver", "w");
	file.SetString(CLIENT_VERSION);
	file.Release();
}
char * CUserManager::GetVersion()
{
	return CLIENT_VERSION;
}

BOOL CUserManager::IsValidUserLevel(BYTE UserLevel)
{
	if(m_LimitUserLevel >= UserLevel)
		return TRUE;
	else
		return FALSE;
}

void CUserManager::LoadDistributeDesc()
{
	char filename[256];
	sprintf(filename,"serverset/%d/DistributeDesc.txt",g_nServerSetNum);
	char buf[256];
	CMHFile file;
	
	if(file.Init(filename, "rt", MHFILE_FLAG_DONOTDISPLAY_NOTFOUNDERROR ))
	{
		while(1)
		{
			if( file.IsEOF() )
				break;
			file.GetString(buf);
			if(strcmp(buf, "*LIMITAGE") == 0)
			{
				m_LimitAge = file.GetByte();
			}
			else if(strcmp(buf, "*DEFAULTUSERLEVEL") == 0 )
			{
				m_LimitUserLevel = file.GetByte();
			}
		}
	}
}
