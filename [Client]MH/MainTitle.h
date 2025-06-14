#pragma once

#include "GameState.h"
#include "cImageSelf.h"
#include "Monster.h"

#define TITLE USINGTON(CMainTitle)

class CEngineMap;
class CEngineCamera;
class cDialog;
class CServerListDialog;
class CIntroReplayDlg;
class CReconnectDialog;

#define _MAX_MAINTITLE_MONSTER_NUM 3

struct sINIT
{
	WORD	wMapNum;
	VECTOR3 vPivot;
	int CameraNum;
	float AngleX;
	float AngleY;
	float fDistance;
};
class CMainTitle : public CGameState
{
	sINIT sInitConfig;
	CEngineCamera*	m_pCamera;

	BOOL	m_bDisconntinToDist;
	DWORD	m_DistAuthKey;
	DWORD	m_UserIdx;
		
//KES 031107
	cDialog*	m_pLogoWindow;
	DWORD		m_dwStartTime;
	BOOL		m_bInit;

#ifdef TAIWAN_LOCAL
	cDialog*	m_pAdvice;
#endif
// JSD
	CServerListDialog*	m_pServerListDlg;
	char				m_DistributeAddr[16];
	WORD				m_DistributePort;
	BOOL				m_bServerList;

//KES
	DWORD		m_dwDiconWaitTime; // for disconnect from Distribute;
	DWORD		m_dwWaitTime;	//for agent connection
	BOOL		m_bWaitConnectToAgent;
	BOOL		m_bNoDiconMsg;


	DWORD		m_ConnectionServerNo;

	// magi82 - Intro(070802)

	CIntroReplayDlg* m_pIntroReplayDlg;

	char  UserName[MAX_NAME_LENGTH + 1];
	char  UserPwd[MAX_NAME_LENGTH + 1];

	CReconnectDialog* m_pReconnectDlg;

public:
	CMainTitle();
	virtual ~CMainTitle();
	BOOL	Init(void* pInitParam);
	void	Release(CGameState* pNextGameState);
	void	LoadInitInfo();


	void KeyboardProcess();
	void Process();
	void ProcessNormal();
	void ProcessDebug();
	void BeforeRender();

	void AfterRender();	
	void NetworkMsgParse(BYTE Category,BYTE Protocol,void* pMsg);

	DWORD GetDistAuthKey()		{ return m_DistAuthKey;	}
	DWORD GetUserIdx()			{ return m_UserIdx; }
	void OnLoginError(DWORD errorcode,DWORD dwParam);
	void OnDisconnect();
	
	// JSD
	void	ConnectToServer( int index );

	void	ShowServerList();
	void	SetServerList()		{ m_bServerList = TRUE; }

	CServerListDialog*	GetServerListDialog()	{ return m_pServerListDlg; }

//KES
	void	StartWaitConnectToAgent( BOOL bStart );
	void	NoDisconMsg()			{ m_bNoDiconMsg = TRUE; }
	
	BOOL	m_bDynamicDlg;


	cImageSelf m_imageLoginBarTop;
	cImageSelf m_imageLoginBarBottom;

	// magi82 - ServerListBackImage(070802)
	cImageSelf m_imageLoginBackGround;

	bool	WriteConfig(BYTE bFlag,char *ID,char *Pwd,bool ClearFlag=false);
	bool	ReadConfig(BYTE bFlag);

	void InitConfig();

	char * GetUserName(){return UserName;}
	char * GetUserPwd(){return UserPwd;}

	void ServerListClose();
#ifndef TAIWAN_LOCAL
	void	InitMainTitleMonsters(void);	
	void	RemoveMainTitleMonsters(void);
	void	ProcessMainTitleMonsters(void);

	CMonster*	m_apMonster[_MAX_MAINTITLE_MONSTER_NUM];
	VECTOR3		m_av3Target[_MAX_MAINTITLE_MONSTER_NUM];	
	VECTOR3		m_av3LastPosition[_MAX_MAINTITLE_MONSTER_NUM];
	DWORD		m_adwLastActionTime[_MAX_MAINTITLE_MONSTER_NUM];
	INT			m_aiLastAction[_MAX_MAINTITLE_MONSTER_NUM];

	RECT	m_rcMainTitleMonsterArea;

#endif
};

EXTERNGLOBALTON(CMainTitle)