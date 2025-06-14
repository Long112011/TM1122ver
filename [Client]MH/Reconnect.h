#pragma once
//#include "./interface/cDialog.h"
#define REDISCONN  USINGTON(CReDisConnManager)
enum REDISCONNECTPROCESS
{
	eLastMapProcess,//true
	eLastPositionProcess,//true

	eMaxReconnection,
};
enum REDISCONNECTSTATE
{
	eFirstGameInDC=0,

	eLastMapOK,
	eLastPositionOK,

	eServerListStep,


	

	eMaxState,
};
class cDialog;
//class CChannelDialog;
//class CCharSelect;
class CReDisConnManager
{
	BOOL m_Master;
	BOOL m_ServerListStep;
	BOOL m_MovePlayAIMap;
	BOOL m_MovePlayAIMapStatic;
	BOOL m_MovePlayAIPos;
	BOOL m_MovePlayAIPosStatic;
	

	bool RECONNECT_PROCESS[eMaxReconnection];

	bool RECONNECT_STATE[eMaxState];


	DWORD m_dwStartTime;
	DWORD m_dwMoveTime;
	DWORD m_dwMovePosTime;

	WORD m_StuckedOnTileTime;





	char  UserName[MAX_NAME_LENGTH + 1];  
	char  UserPwd[MAX_NAME_LENGTH + 1];   
	char  UserPin[5];

	char m_strUserID[MAX_NAME_LENGTH+1];


	DWORD	dwHeroMapNum;
	int		m_HeroChannel;
	VECTOR3		m_Heropos;
	DWORD		dw_Heropos;
	char  SafePwd[MAX_NAME_LENGTH + 1];
	DWORD	dwTimeCount;
	DWORD   dwStepTime;
	BOOL	InGameDC;
	BOOL	m_OpenAutoPlay;
	WORD	WaitMapCount;

	BYTE	bCountDown;


	int m_ServerIndex;
	int m_ChannelIndex;
	int m_CharIndex;

	BOOL	m_bDisconnected;

	DWORD	dwMapLoadStuckCount;


	LONG	RecDlgX;
	LONG	RecDlgY;

	DWORD	dwAccID;
public:
	CReDisConnManager();
	~CReDisConnManager();

	void SetRecDlgAbsXY(LONG x,LONG y){RecDlgX=x;RecDlgY=y;}
	LONG GetRecDlgAbsX(){return RecDlgX;}
	LONG GetRecDlgAbsY(){return RecDlgY;}

	void NewLoopTime();//{m_dwMoveTime=gCurTime;}

	void FinishReconnect();

	//void InitOld();
	void Init();
	BOOL GetInGameDCStatus(){return InGameDC;}
	void StartProcess(DWORD dwTime=7);
	void Process();   
	void GoToGameLoginDialog();   
	void StopReconnect(WORD bFlag=0);
	bool FillLoginInfo(cDialog * p);
	BOOL GetStart(){return m_Master;}
	void SetStart(BOOL Val){m_Master=Val;}
	BOOL GetServerListStep(){return m_ServerListStep;}
	void SetServerListStep(BOOL Val){m_ServerListStep=Val;}
	char * GetUserName(){return UserName;}
	void SetUserName(char * Val){ SafeStrCpy(UserName,Val,MAX_NAME_LENGTH + 1);}
	char * GetUserPwd(){return UserPwd;}
	void SetUserPwd(char * Val){SafeStrCpy(UserPwd,Val,MAX_NAME_LENGTH + 1);}
	char * GetSafePwd(){return SafePwd;}
	void SetSafePwd(char * Val){SafeStrCpy(SafePwd,Val,MAX_NAME_LENGTH + 1);}
	char * GetUserPin(){return UserPin;}
	void SetUserPin(char * Val){SafeStrCpy(UserPin,Val,MAX_NAME_LENGTH + 1);}
	BOOL GetMovePlayAIMap(){return m_MovePlayAIMap;}
	void SetMovePlayAIMap(BOOL Val){m_MovePlayAIMap=Val;}
	BOOL GetMovePlayAIMapStatic(){return m_MovePlayAIMapStatic;}
	void SetMovePlayAIMapStatic(BOOL Val){m_MovePlayAIMapStatic=Val;}
	BOOL GetMovePlayAIPos(){return m_MovePlayAIPos;}
	void SetMovePlayAIPos(BOOL Val){m_MovePlayAIPos=Val;}
	BOOL GetMovePlayAIPosStatic(){return m_MovePlayAIPosStatic;}
	void SetMovePlayAIPosStatic(BOOL Val){m_MovePlayAIPosStatic=Val;}
	void SetMoveStartTime(DWORD m_MoveTime){m_dwMoveTime=m_MoveTime;}
	void SetMovePosStartTime(DWORD m_MovePosTime){m_dwMovePosTime=m_MovePosTime;}
	DWORD GetHeroMapNum(){return dwHeroMapNum;}
	void SetHeroMapNum(DWORD Val){dwHeroMapNum=Val;}
	void SetAutoPlay(BOOL val){m_OpenAutoPlay = val;}
	BOOL IsAutoPlay(){return m_OpenAutoPlay;}
	
	void DoneReconnecting();
	void MainTitleProcess();
	void CharSelectProcess();

	DWORD GetDWORDSavedHeroPos(){return dw_Heropos;}
	//void GameInMoveMap();
	//void GameInMovePosition();
	//BOOL IsStillMoving();

	//-----new
	//void DebugProcess();
	//void DebugCheck();




	void ShowMainTitle();

	bool CheckLastMap(){return RECONNECT_STATE[eLastMapOK];}
	void SetCheckLastMap(bool bFlag){RECONNECT_STATE[eLastMapOK]=bFlag;}
	void SetLastMapProcess(bool bFlag){RECONNECT_PROCESS[eLastMapProcess]=bFlag;
										m_dwMoveTime=gCurTime;}
	//void SetLastMapTime(){m_dwMoveTime=gCurTime;}
	bool LastMap();

	bool CheckLastPosition(){return RECONNECT_STATE[eLastPositionOK];}
	void SetLastPositionProcess(bool bFlag){RECONNECT_PROCESS[eLastPositionProcess]=bFlag;
											m_dwMovePosTime=gCurTime;}

	bool IsHeroIdle();
	void LastPosition();
	

	void HitWaitMapCount(){WaitMapCount=WaitMapCount+1;}

	int GetServerIdx(){return m_ServerIndex;}
	int GetChannelIdx(){return m_ChannelIndex;}
	int GetCharIdx(){return HEROID;}

	void SetServerIdx(int idx){m_ServerIndex=idx;}
	void SetChannelIdx(int idx){m_ChannelIndex=idx;}
	void SetCharIdx(int idx){m_CharIndex=idx;}



	void NewTime(){m_dwMoveTime=gCurTime;}


	BOOL RecMSG(int id);


	void SetUserID( char* strID );

	void SetAccID(DWORD dwID){ dwAccID = dwID; }
	DWORD GetAccID(){
		if (dwAccID != 0)
			return dwAccID;
	}

	virtual void OnDisconnect();
	virtual BOOL IsDisconnect();

	void SetDisConnected(BOOL bFlag=true){m_bDisconnected=bFlag;}
	BOOL IsDisConnected()	{	return m_bDisconnected ? TRUE : FALSE;	}


	//-----new end
};
EXTERNGLOBALTON(CReDisConnManager);