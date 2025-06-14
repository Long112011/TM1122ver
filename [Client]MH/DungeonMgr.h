#pragma once
#include "stdafx.h"
#include "CommonStruct.h"
//-----------------------------------FROM FUBENMANAGER.CPP
#include "PtrList.h"
#include "..\[CC]Fuben\FubenDefines.h"
//#include "NewbieGuide.h"
class CFubenString;
//--------------------------------------------------------

#define DungeonMGR USINGTON(CDungeonMgr)
struct DUNGEON_TEXT
{
	DUNGEON_TEXT() { strToolTip = NULL; pNext = NULL; }
	char*			strToolTip;
	DUNGEON_TEXT*	pNext;
};
struct sDUNGEONNEWINITDesc
{
	WORD	GroupNum;
	WORD	SubMission;
	char FiniteStateMachine[1024];
};
class CDungeonMgr
{
	//-----------------------------------FROM FUBENMANAGER.CPP
protected:
	CYHHashTable<CFubenString>	m_FubenStringTable;
	//DWORD						m_dwMaxNpc;
	//CNewbieGuide				m_NewbieGuide;		// newbieguide
	//DWORD						m_dwLastMsg;

public:
	CFubenString*	GetFubenString( DWORD dwFubenIdx )	{ return m_FubenStringTable.GetData( dwFubenIdx );	}
	char*			GetFubenTitle( DWORD dwFubenIdx , DWORD dwSub=0);
	char*			GetFubenDesc( DWORD dwFubenIdx , DWORD dwSub=0);
	char*			GetFubenDesc2( DWORD dwFubenIdx , DWORD dwSub=0);
	BOOL	LoadFubenString( char* strFileName );
	void	AddStringTest();
	//--------------------------------------------------------
private:
	CYHHashTable<stWarpInfo>		m_WarpList;
	CYHHashTable<stSwitchNpcInfo>	m_SwitchNpcList;
	stDungeon						m_CurDungeon;
	std::multimap< DWORD, DWORD >	m_mapEntranceNpcJob;

	CYHHashTable<DUNGEON_TEXT>		m_FBTipTextList;

	CYHHashTable<FB_MAP_INFO> m_FBEntrance;

	CYHHashTable<InsDGInfo> m_InsDGInfo;//

	BOOL	IsLeaveStarted;
	DWORD	dwCountDownTick;
	DWORD	dwCountDownOneSec;



	DWORD StoredNpcUniqueIdx;
	DWORD Storeddestmapnumber;
	DWORD StoredNpcId;
	DWORD StoredNpcUniqueIdxT;

public:
	CDungeonMgr(void);
	virtual ~CDungeonMgr(void);

	void	Init();
	void	Process();
	void	NetworkMsgParse(BYTE Protocol,void* pMsg);

	//void	LoadDungeonInfo();
	//void	SetWarp(DWORD dwIndex, BOOL bActive);
	void	SetSwitchNpc(WORD dwIndex, BOOL bActive);

	BOOL	IsDungeonMap(MAPTYPE map);

	void	ZoneBlock(float posx, float posz, float range);
	void	ZoneFree(float posx, float posz, float range);
	void	EdgeBlock(float posx, float posz, float range);
	void	EdgeFree(float posx, float posz, float range);
	//void	AfterWarpAction( DWORD dwWarpIndex );
	void	ClearCurDungeon();
	void	AddEntranceNpc( DWORD dwDungeonKey, DWORD dwNpcJob );
	BOOL	IsDungeonEntranceNpc( const DWORD dwNpcJob ) const;

	void CancelMoveMap();
	void	FubenMap(DWORD NpcUniqueIdx,DWORD destmapnumber,DWORD NpcId,DWORD NpcUniqueIdxT);
	std::multimap< DWORD, DWORD >& GetEntranceNpcJobMap() { return m_mapEntranceNpcJob; }

	DUNGEON_TEXT* GetFBTipInfo( WORD wIdx );
	char*	CheckFBTipInfo(DWORD MapNum);
	void TestLoadInfo();

	BOOL LoadFBEntranceCondition();
	FB_MAP_INFO * GetFBEntranceData(DWORD Flag);
	BYTE GetFBEntranceMap(DWORD MapNum);
	WORD GetFBMapChangeIdx(DWORD MapNum);

	void StartLeave();
	void LeaveProcess();

	void LoadToolTipList();
	DUNGEON_TEXT* GetToolTipInfo( WORD wIdx );

	sDUNGEONNEWINITDesc wDGTest2;
	void LoadTestNewBinTest();


	BOOL LoadInsDungeonInfo();
	InsDGInfo * GetInsDungeonInfoData(int dwMapNum);
};

EXTERNGLOBALTON(CDungeonMgr)