
#pragma once
//---KES Distribute Encrypt 071003
#include "Crypt.h"
//--------------------------------
enum eDISCONNECT_OPTION
{
	eDISCONOPT_NONE				= 0,
	eDISCONOPT_DELETELOGINCHECK	= 1,
};

BOOL DisconnectUser( DWORD dwConnectionIndex );
//------

typedef struct tagUSERINFO
{
	DWORD	dwConnectionIndex;
	DWORD	dwUserID;
	BYTE	UserLevel;


	WORD	wUserMapNum;

	DWORD	State;
	BOOL	bRecvDistOut;

	DWORD	dwUniqueConnectIdx;
	DWORD	dwLastConnectionCheckTime;
	BOOL	bConnectionCheckFailed;

	DWORD	dwDisconOption;	
	WORD    wServerNum;
//---KES Distribute Encrypt 071003
	BYTE	cbCheckSum;
	CCrypt	crypto;
//--------------------------------
	char   reUserID[20];
	char   rePwd[20];
	char   reTel[20];
	char   reEmail[50];
	char   NewPwd[20];
	char   Mac[18];
	char   IP[18];


} USERINFO;

class CUserTable : public CYHHashTable<USERINFO>
{
	DWORD m_MaxUserCount;
	DWORD m_dwUserCount;
	DWORD m_addCount;

public:

	CUserTable();
	virtual ~CUserTable();

	void SetPositionUserHead() {  CYHHashTable<USERINFO>::SetPositionHead(); }
	USERINFO * GetUserData() { return CYHHashTable<USERINFO>::GetData(); }

	void Init(DWORD dwBucket);
	USERINFO * FindUser(DWORD dwKey);

	BOOL AddUser(USERINFO* pObject,DWORD dwKey);
	USERINFO * RemoveUser(DWORD dwKey);
	void RemoveAllUser();

	DWORD GetUserCount() { return m_dwUserCount; }

	DWORD GetUserMaxCount() {return m_MaxUserCount;}

	BOOL SendToUser(DWORD dwKey,DWORD dwUniqueConnectIdx,MSGBASE* pmsg,DWORD size);
	BOOL OnDisconnectUser(DWORD dwKey);

	DWORD GetAddCount() { return m_addCount; }
};  


extern CUserTable * g_pUserTable;							// key : connectionidx

struct sLOGINUSER
{
	DWORD dwTime;
	DWORD dwCount;
};

class CLoginUser
{
	CYHHashTable<sLOGINUSER>	m_User;


public:
	CLoginUser();

	virtual ~CLoginUser();
/*
	void	SetCount( DWORD dwUserIdx, WORD wNum );
	void	Receive( DWORD dwUserIdx );
	int		Check( DWORD dwUserIdx );
*/
	void	Remove( DWORD dwUserIdx );
	void	AddWaitForLoginCheckDelete( DWORD dwUserIdx );

};

extern CLoginUser g_LoginUser;