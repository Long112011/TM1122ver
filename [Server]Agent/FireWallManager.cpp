
#include "Stdafx.h"
#include "Network.h"
#include "FireWallManager.h"
#include "AgentDBMsgParser.h"

#define MAX_ATTR_TIME 20

GLOBALTON(CFireWallManager);

CFireWallManager::CFireWallManager()
{
	m_FireWallInfoList.Initialize(100); //100个不同的攻击者
}

CFireWallManager::~CFireWallManager()
{
    FIREWALLINFOLIST * pInfo=NULL;
	m_FireWallInfoList.SetPositionHead();
	while(pInfo=m_FireWallInfoList.GetData())
	{
		delete pInfo;
		pInfo=NULL;
	}
	m_FireWallInfoList.RemoveAll();
}

void CFireWallManager::Release()
{
    FIREWALLINFOLIST * pInfo=NULL;
	m_FireWallInfoList.SetPositionHead();
	while(pInfo=m_FireWallInfoList.GetData())
	{
		delete pInfo;
		pInfo=NULL;
	}
	m_FireWallInfoList.RemoveAll();
}

void CFireWallManager::AddFireWallInfo(FIREWALLINFOLIST * pInfo)
{
	FIREWALLINFOLIST * p_Info = GetFireWallInfo(pInfo->IpAddr);

	if(!p_Info)
	{
		m_FireWallInfoList.Add(pInfo,pInfo->Index);
	}
	else
	{
		m_FireWallInfoList.Remove(p_Info->Index);
		m_FireWallInfoList.Add(pInfo,pInfo->Index);
	}
}
FIREWALLINFOLIST * CFireWallManager::GetFireWallInfo(char * IpAddr)
{
	FIREWALLINFOLIST * pInfo=NULL;
	m_FireWallInfoList.SetPositionHead();
	while(pInfo=m_FireWallInfoList.GetData())
	{
		if(strcmp(pInfo->IpAddr,IpAddr)==0)
		{
			return pInfo;
		}
	}
}

void CFireWallManager::CheckIpAddr(DWORD dwConnectionIndex)
{
	char ip[16];
	WORD port;
	g_Network.GetUserAddress( dwConnectionIndex, ip, &port );
	//g_Console.LOG(4, "ip : %s", ip);
    FIREWALLINFOLIST * pInfo=GetFireWallInfo(ip);
    if(pInfo!=NULL)
	{
		if(pInfo->AttrTime>MAX_ATTR_TIME)
		{
			DisconnectUser( dwConnectionIndex );
			return;
		}
	}
}

void CFireWallManager::AddFireWallInfoToDB(DWORD dwConnectionIndex)
{
	char ip[16];
	WORD port;
	g_Network.GetUserAddress( dwConnectionIndex, ip, &port );
	FIREWALLINFOLIST * pInfo=GetFireWallInfo(ip);
	if(pInfo!=NULL)
	{
		if(pInfo->AttrTime>10)
		{
			DisconnectUser( dwConnectionIndex );
			return;
		}
		else
		{
			AddFireInfoToDB(pInfo->IpAddr,pInfo->AttrTime+1);
		}
	}
	else
	{
         AddFireInfoToDB(ip,1);
	}
    
}

