
#if !defined(AFX_FIREWALLMGR_H__4458B32A_934C_4817_A100_0F8D7286ACE7__INCLUDED_)
#define AFX_FIREWALLMGR_H__4458B32A_934C_4817_A100_0F8D7286ACE7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#define FIREWALLMGR	USINGTON(CFireWallManager)

struct FIREWALLINFOLIST
{
	DWORD Index;
	char IpAddr[MAX_IPADDRESS_SIZE];
	DWORD AttrTime;
};

class CFireWallManager
{

	CYHHashTable<FIREWALLINFOLIST> m_FireWallInfoList;

public:

	CFireWallManager();

	virtual ~CFireWallManager();

	void Release();

	void LoadFireWallInfo();

	void AddFireWallInfo(FIREWALLINFOLIST * pInfo);

	FIREWALLINFOLIST * GetFireWallInfo(char * IpAddr);

	void CheckIpAddr(DWORD dwConnectionIndex);

	void AddFireWallInfoToDB(DWORD dwConnectionIndex);

};

EXTERNGLOBALTON(CFireWallManager);

#endif