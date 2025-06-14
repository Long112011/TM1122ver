#ifndef _INSDUNGEONMANAGER_H
#define _INSDUNGEONMANAGER_H
#define INSDUNGEONMGR	USINGTON(CInsDungeonManager)
#include "InsDGMissionManager.h"
class CMHFile;
class CInsDGMission;
class CInsDGMissionString;
class CImage;
class CNpc;
class CInsDungeonManager
{
public:
	CInsDungeonManager();
	~CInsDungeonManager();
	void Init();
	void Release();
	void InitData();
	void NetworkMsgParse( BYTE Protocol, void* pMsg );
	BOOL LoadEntranceConditionInfo();	
	void RemoveEntranceConditionInfo();	
	INSDG_ENTRANCECONDITION_INFO* GetEntranceConditionInfo(DWORD dwNpcUniqueIDX);
    BOOL LoadInsDungeonInfo();		
	void RemoveInsDungeonInfo();
	DWORD GetInsDungeonInfoCount();
    INSDUNGEON_INFO* GetInsDungeonInfo(MAPTYPE wMapNum);
	INSDUNGEON_INFO* GetInsDungeonInfo(DWORD dwInsDGIndex);
	CYHHashTable<INSDUNGEON_INFO>* GetInsDungeonInfoTable();
	BOOL LoadMissionString();
	CInsDGMissionString* LoadMissionStringInfo(CMHFile* pFile);
	void RemoveMissionString();
	CInsDGMissionString* GetMissionString(DWORD dwKey)	{ return m_InsDGMissionStringTable.GetData(dwKey); }
	BOOL LoadInsDungeonMissionInfo();	
	eINSDG_ENTRANCE_ERROR CheckEntranceCondition(DWORD dwNPCUniqueIDX);	
	void EntranceErrorMsg(eINSDG_ENTRANCE_ERROR eError);	
	CInsDGMissionManager* GetInsDGMissionManager()	{ return m_pInsDGMissionManager; }
	CInsDGMission* GetCurMission()	{ return m_pInsDGMissionManager->GetCurMission(); }
	void InitInsDGStartTime();	
	void StartInsDGStartTime();	
	void SetCurrentInsDungeonInfo(MAPTYPE wMapNum);
	void Process();
	void Render();
	void SendNPCMissionEvent(CNpc* pNpc);
	void InitRankState();
	void SetRankState(INSDG_RANK_STATE* pRankState);
	INSDG_RANK_STATE* GetRankState();
    void RemoveInsDGRankInfoTable();
	void SetInsDGRankTotalInfo(MSG_INSDG_RANK_TOTAL_INFO* pMsg);
	void AddInsDGRankInfo(INSDG_RANK_INFO* pRankInfo);
	INSDG_RANK_INFO* GetInsDGRankInfo(DWORD dwInsDGIndex);
	CYHHashTable<INSDG_RANK_INFO>* GetInsDGRankInfoTable();
	DWORD GetRank(DWORD dwRankValue);
	inline DWORD GetProgressTime()	{ return m_dwProgressTime; }
	inline DWORD GetMissionGroupIDX()	{ return m_pInsDGMissionManager->GetMissionGroupIDX(); }
	inline void SetRequitalTime(DWORD dwRequitalTime)	{ m_dwRequitalTime = dwRequitalTime; }
	inline DWORD GetRequitalTime()	{ return m_dwRequitalTime; }
private:
	CYHHashTable<INSDG_ENTRANCECONDITION_INFO>	m_EntranceConditionTable;	
	CYHHashTable<INSDUNGEON_INFO>	m_InsDungeonInfoTable;	
	CYHHashTable<CInsDGMissionString>	m_InsDGMissionStringTable;	
	CYHHashTable<INSDG_RANK_INFO> m_InsDGRankInfoTable;		
	INSDUNGEON_INFO* m_pCurInsDungeonInfo;		
	CInsDGMissionManager* m_pInsDGMissionManager;
protected:
	eINSDG_TYPE m_eInsDungeonType;		
	INSDG_RANK_STATE m_RankState;
	DWORD m_dwChannel;				
	DWORD m_dwPartyIDX;				
    DWORD m_dwMunpaIDX;				
	DWORD m_dwMonsterKillNum;		
	DWORD m_dwInsDGStartTime;		
	DWORD m_dwInsDGEndTime;			
	DWORD m_dwRequitalTime;			
	DWORD m_dwMaxMonsterCount;		
	DWORD m_dwProgressTime;			
	BOOL m_bInsDGStart;				
};
EXTERNGLOBALTON(CInsDungeonManager);
#endif 
