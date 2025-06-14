#pragma once

#define DungeonMGR USINGTON(CDungeonMgr)
#define DUNGEON_ASSERTMSG(a,b)			{ if(0 == (a)) { WriteAssertMsg(__FILE__,__LINE__,b); } }

#define COMBINEKEY(a, b, c)				\
	if(b<100)			c=a*100+b;		\
	else if(b<1000)		c=a*1000+b;		\
	else if(b<10000)	c=a*10000+b;	

#include "..\[CC]Fuben\FubenDefines.h"
//#include "Channel.h"

class CMHFile;
class CDungeonProcessorBase;

struct InsDungeonInfo
{
	WORD dgMapNum;
	BYTE dgParam1;
	BYTE dgParam2;
	BYTE dgBackMap;
	WORD dgMapChange;
	BYTE dgFinishMap;
	int  dgTime;
	BYTE dgStringIdx;

	void Clear()
	{
		memset(this, 0, sizeof(InsDungeonInfo));
	}

	InsDungeonInfo()
	{
		Clear();
	}
};
struct DUNGEON_MAPINFO
{
	WORD pMapNum;
	BYTE pChannnelNum;
	BYTE pLimitLevel;
	BYTE pClearMonster;
	BYTE pRangeLevel;

	void Clear()
	{
		memset(this, 0, sizeof(DUNGEON_MAPINFO));
	}

	DUNGEON_MAPINFO()
	{
		Clear();
	}
};
struct DUNGEON_MISSION
{
	BYTE mGroupNum;
};
struct DUNGEON_MISSION_SUB
{
	DWORD MissionID;
	DWORD TargetKill;
	DWORD MaxCount;
	DWORD NextSubMission;

	void Clear()
	{
		memset(this, 0, sizeof(DUNGEON_MISSION_SUB));
	}

	DUNGEON_MISSION_SUB()
	{
		Clear();
	}
};


struct DUNGEON_MISSION_SUBTEST
{
	DWORD MissionID;
	DWORD TargetKill;
	DWORD MaxCount;
	DWORD NextSubMission;


	void Clear()
	{
		memset(this, 0, sizeof(DUNGEON_MISSION_SUBTEST));
	}

	DUNGEON_MISSION_SUBTEST()
	{
		Clear();
	}
};







struct stDungeonReservation
{
	DWORD dwCharIndex;
	DWORD dwPartyIndex;
	DWORD dwChannelID;
	DWORD dwRegistTime;
	VECTOR3 vStartPos;

	//DWORD dwTest;

	void Clear()
	{
		memset(this, 0, sizeof(stDungeonReservation));
	}

	stDungeonReservation()
	{
		Clear();
	}
};

struct stMonsterList
{
	eDIFFICULTY difficulty;
	DWORD dwIndex;
	WORD dwPosX;
	WORD dwPosZ;
	char Alias[MAX_PATH];
	char GroupAlias[MAX_PATH];
	BOOL bActive;
	char FiniteStateMachine[MAX_PATH];
};

typedef CDungeonProcessorBase* (*DungeonProcessor)();
struct stDungeonProcessor 
{
	MAPTYPE				mapnum;
	DungeonProcessor	processor;
};

enum eAliasKind
{
	eAliasKind_None,
	eAliasKind_Alias,
	eAliasKind_GroupAlias
};

struct stDungeonNoPartyList
{
	DWORD	dwCharIndex;
	DWORD	dwOldPartyIndex;
	DWORD	dwOutTime;
};
class CParty;
struct GUILD_WAR_PLAYER_TEST
{
	//CParty* pParty;
	CPlayer* pPlayer;
	DWORD	pPartyIDX;
	DWORD	pCharacterIDX;
	DWORD	pUniconIDX;
	DWORD	pChannelIDX;
	//char	pCharacterName[16+1];

	DWORD	pPlayerKillCount;
	DWORD	pDieCount;

	BYTE	pFirstCreation;

	DWORD	pCurMapTime;
};


//-------------------------------------
// below is new and stable
//-------------------------------------
struct sDUNGEONNEWINFO
{
	int		killcount[1000];

	/*void Clear()
	{
		memset(this, 0, sizeof(sDUNGEONNEWINFO));
	}

	sDUNGEONNEWINFO()
	{
		Clear();
	}*/
};

struct sDUNGEONAISYS
{
	WORD	GroupNum;
	WORD	MaxObject;

	WORD	AddKind;
	WORD	AddGenerateId;
	WORD	AddMonsterId;
	VECTOR3	AddPosXYZ;

	int		create[1000];

	void Clear()
	{
		memset(this, 0, sizeof(sDUNGEONAISYS));
	}

	sDUNGEONAISYS()
	{
		Clear();
	}
};

struct sDUNGEONNEWINITDesc
{
	WORD	GroupNum;
	WORD	SubMission;
	char	FiniteStateMachine[1024];
};
enum eDGRemain
{
	eDGStart= -1,
	eDGNone = 0,
	eDGMonsterKill = 1,
	eDGMonsterKillReset = 2,
	eDGCurMapDone = 3,
	eWaitingToStart=4,
};


//-------------------------------------
// end
//-------------------------------------

//-------------------------------------
// enum for InsDungeonTimeStore
//-------------------------------------
/*enum EDGTimeStoreEnum
{
	eWaitingToStart=0,
	eIsNowStarted=1,
	eAllMonsterKilled=2,
	eSubMissionDone=3,
	eAllSubMissionDone=4,
	eTimerWasStopped=5
};*/
struct InsDungeonTimeStore
{
	DWORD InsDGChannelIdx;
	DWORD InsDGTimer;
	DWORD InsDGStartTime;
	bool  IsStart;
	DWORD IsInsDGState;
	int	InsDGMonsterKillCount;
	WORD InsDGFBMapArea;
};
class CDungeonMgr
{
private:
	CDungeonProcessorBase*						m_pDungeonProcessor;

	CYHHashTable<GUILD_WAR_PLAYER_TEST> pPlayerInfoTable;

	CMemoryPoolTempl<stDungeon>*				m_DungeonPool;
	CMemoryPoolTempl<stDungeonReservation>*		m_ReservationPool;
	CMemoryPoolTempl<stDungeonNoPartyList>*		m_NoPartyPool;
	
	CYHHashTable<stDungeon>						m_DungeonList;
	CYHHashTable<DWORD>							m_ChannelList;
	CYHHashTable<stDungeonReservation>			m_ReservationList;
	CYHHashTable<stDungeonNoPartyList>			m_NoPartyList;

	//CYHHashTable<stMonsterList>					m_MonsterList_Easy;
	//CYHHashTable<stMonsterList>					m_MonsterList_Normal;
	//CYHHashTable<stMonsterList>					m_MonsterList_Hard;

	CYHHashTable<stWarpInfo>					m_WarpList;
	CYHHashTable<stSwitchNpcInfo>				m_SwitchNpcList;

	//typedef std::vector< DWORD >				BuffVector;
	//typedef std::map< DWORD, BuffVector >		DungeonBuffMap;
	//DungeonBuffMap								m_DungeonBuffMap;
	
	// 인던 생성 & 해제

	
	stDungeon* Create(DWORD dwPartyID, DWORD dwChannelID);
	void Keluar(DWORD dwChannelID);
	void KeluarTest(DWORD dwChannelID);
	
	//BOOL	m_bFubenMap;// 인던 해제

	CIndexGenerator m_MonsterIDGenerator;


	CYHHashTable<InsDungeonInfo> m_DGInfo;

	CYHHashTable<FB_MAP_INFO> m_FBEntrance;

	CYHHashTable<InsDGInfo> m_InsDGInfo;//

//---------------------------------------
	//CYHHashTable<FBTIMERINFO>  m_FbTimerInfo;
	CYHHashTable<FUBENMAPINFO>  m_FBMapAreaInfo;

	CYHHashTable<InsDungeonTimeStore>  m_InsDGTimerInfo;
	BOOL mCreate;
//---------------------------------------

	BYTE TEMPEDGTimeStoreEnum;
	BYTE TempeDGRemain;

public:
	CDungeonMgr(void);
	virtual ~CDungeonMgr(void);


	sDUNGEONNEWINFO wDGInfo;
	sDUNGEONNEWINIT wDGTest;
	sDUNGEONNEWINITDesc wDGTest2;
	sDUNGEONAISYS wDGAISystem;

	void	SetMapState(BYTE bState){TempeDGRemain=bState;}
	BYTE	GetMapState(){return TempeDGRemain;}

	void	Init();
	
	//void	GetMissionGroup();
	//void	Process(CPlayer* pPlayer);
	void	Process();
	void	NetworkMsgParser(DWORD dwConnectionIndex, BYTE Protocol, void* pMsg, DWORD dwLength);


	// 인던 입장 & 퇴장
	DWORD Entrance(DWORD dwPlayerID, DWORD dwPartyID);
	void Exit(CPlayer* pPlayer);
	void ReservationDungeon(DWORD dwPlayerID, DWORD dwPartyID, DWORD dwChannelID);
	void PlayerEntered(DWORD dwPlayerID, DWORD dwChannelID);
	DWORD GetdwTest(DWORD dwPlayerID);
	DWORD GetChannelIDFromReservationList(DWORD dwPlayerID);
	DWORD GetChannelIDFromPartyIdx(CPlayer* pPlayer);
	VECTOR3* GetStartPosFromeservationList(DWORD dwPlayerID);

	DWORD GetPlayerNumInDungeon(DWORD dwChannelID);
	DWORD GetPartyIndexFromDungeon(DWORD dwChannelID);
	
	DWORD GetDungeonNum();
	DWORD GetPlayerNum();
	DWORD GetMonsterNum();
	DWORD GetNpcNum();
	DWORD GetExtraNum();

	void AddPoint(DWORD dwChannelID, DWORD value);
	void SubtractPoint(DWORD dwChannelID, DWORD value);
	DWORD GetPoint(DWORD dwChannelID);

	void SendMsgToChannel(MSGBASE* pMsg, DWORD dwLength, DWORD dwChannelID);

	void AddNoPartyList(DWORD dwPlayerID, DWORD dwPartyID);
	void RemoveNoPartyList(DWORD dwPlayerID);
	void MoveToLoginMap(DWORD dwCharIndex);


	BOOL CheckHackNpc( CPlayer* pPlayer, WORD wNpcIdx );
	//DWORD GetNpcObjectIndex(DWORD dwChannelID, WORD dwNpcIndex);
	DWORD AddNpc( stSwitchNpcInfo* pSwitchNpc, DWORD dwChannelID );
	


	void CreateNewTimer(DWORD dwChannelID, DWORD timerAlias);
	void RemoveTimer(DWORD dwChannelID);

	DWORD GetTimer(DWORD dwChannelID);

	DWORD CalculateTimer(DWORD dwChannelID);

	bool GetTimerState(DWORD dwChannelID);
	void SetTimerStartBool(DWORD dwChannelID);

	int GetMonsterKillCount(DWORD dwChannelID);
	WORD GetFBMapArea(DWORD dwChannelID);
	void SetMonsterKillCount(DWORD dwChannelID,int Count);
	void SetFBMapArea(DWORD dwChannelID,int Count);

	// Uitility Functions
	//void		LoadDungeonInfo();
	DWORD		GetElapsedSecondFromStartTime(DWORD dwChannelID);
	//eDIFFICULTY GetDifficulty(DWORD dwChannelID);
	//eDIFFICULTY GetDifficultyFromKey(DWORD keyItemIndex);
	BOOL		IsDungeon(MAPTYPE) const;
	//const stDungeon& GetDungeon(DWORD channelIndex);
	//DWORD GetAliasHashCode( stMonsterList* pMonster, DWORD dwChannelID, eAliasKind eAlias );

	// 던전 모니터링 관련
	//void	SendInfoSummary(DWORD dwConnectionIndex, char* pMsg);
	//void	SendInfoDetail(DWORD dwConnectionIndex, char* pMsg);

	// 100419 ONS 현재 인던내에 부여된 버프 저장/삭제
	//void	StoreSwitchBuff( DWORD dwChannelID, DWORD dwBuffIdx );
	//void	DelSwitchBuff( DWORD dwChannelID, DWORD dwBuffIdx );

	BOOL MoveMap(CPlayer* pPlayer,WORD pMapNum,WORD pChannelIDX,BYTE pType);

	bool SendErrorMsg(CPlayer* pPlayer,DWORD dwError);
	bool SendDialogMsg(CPlayer* pPlayer,MSG_DWORD5*pmsg,DWORD index);
	bool MoveMapTest(CPlayer* pPlayer,WORD pMapUniqueNum,WORD pDestMapNum,DWORD dwConnectionIndex,BYTE pType);

	DWORD GeneraterMonsterID();

	void TestTest();

	void ReturnToMap(CPlayer* pPlayer);
	void AutoReturnToMap();

	void GoToMap(CPlayer* pPlayer, DWORD MapNum);

	bool CheckMap(MSG_DWORD4* pmsg);

	void RemoveMonster(WORD pChannel);
	BOOL SuspendMonster(WORD pChannel);




	






	void SetCurrentMapState(DWORD dwChannelID,DWORD dwState=0);

	


	BYTE GetFubenKey(WORD MapNum);




	InsDungeonTimeStore *GetTimeStoreInfo(DWORD dwChannelID);


//----------------------------------------------------------
	//void LoadInsDGMission_v13();
	void CheckKillCount(CPlayer* pPlayer,DWORD Num, DWORD MonsterId);
	//void CreateMonster(CPlayer* pPlayer,BOOL Flag);
	void SendKillCount(DWORD Val,DWORD dwChannelID);
	void SendMessage(DWORD Val,DWORD dwChannelID);
	void SendNewMessage(DWORD dwLoadSubMsg,DWORD dwSubMsg,DWORD dwCurKillCount,DWORD dwMaxKillCount,DWORD dwChannelID=0,CPlayer* pPlayer=NULL);
	void FubenInfoKind(DWORD CharacterIdx,DWORD dwChannelID,DWORD Val,DWORD dwFlag,BYTE Type,DWORD Val2 = 0);

	FUBENMAPINFO*GetFBMapAreaInfo(DWORD Flag);
	DWORD GetFBMapAreaFlag(DWORD dwFlag);
	//char* CheckKindNextFlag(DWORD MapNum);

	/*FBTIMERINFO * GetFBTimerAreaInfo(DWORD Flag);
	BYTE CheckIfMapNeedPass(DWORD MapNum);
	char* CheckFBMapKind(DWORD MapNum);
	DWORD MapEnterTimer(DWORD MapNum);*/
//----------------------------------------------------------




	//void TambahPlayer(CPlayer* pPlayer,DWORD dwMapNumber);
	
	void RemoveAllPlayer();

	DWORD pSendAllInfoTime;


	//void LoadInsDGMission_NEW(DWORD MapIdx);

	//void GetMissionGroup(CMHFile* file,DWORD TabNum=0);
	//void GetMission(CMHFile* file,DWORD TabNum=0);

	
	//-------------below is stable
	void LoadInsDGMission(DWORD dwGroupNum);
	int LoadMonster(DWORD dwChannel);
	//----------------------------

	BOOL LoadInsDungeonInfo();
	InsDGInfo* GetInsDungeonInfoData();
	void BackToTown(CPlayer* pPlayer);

	void LoadFBEntranceCondition();
	FB_MAP_INFO * EntraceConditionData(DWORD UniqueIdx);
	BYTE GetEntraceCurMapNum(DWORD UniqueIdx);
	WORD GetEntraceUniqueNpcIdx(DWORD UniqueIdx);

	void ClearDB(DWORD dwMapIndex);
	void ClearDBWithChannel(DWORD dwMapIndex,DWORD dwChannelIdx);

	void Routine60Sec();


protected:
		//CChannel	m_FBChannel[1000];

private:
		DUNGEON_MAPINFO pDungeonMapInfo;

		InsDungeonInfo pInsDungeonInfo;

		DUNGEON_MISSION pInsMission;

		WORD pInsSubMissionCount;
		DUNGEON_MISSION_SUB* pInsSubMission;

		DUNGEON_MISSION_SUBTEST pInsSubMissionTest;
};

EXTERNGLOBALTON(CDungeonMgr)
