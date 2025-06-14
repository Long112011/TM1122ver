#ifndef _INSDUNGEON_H
#define _INSDUNGEON_H

#include "InsDungeonRegenManager.h"
#include "InsDGMissionManager.h"

class CPlayer;

class CInsDungeon
{
	enum
	{
		eRETURN_TO_MAP_WAIT_TIME = 10000,	//미션 클리어 후 보내내기 대기시간		
	};

public:
	CInsDungeon();
	~CInsDungeon();

	void Init();
	void Destory();

	void InitInsDGStartTime();	//인던 시간 초기화
	void StartInsDGStartTime();	//인던 시간 시작

	void Process();

	void ReturnToMap();
	void PartyReturnToMap();
	void PartyBreakupReturnToMap();

	void RemoveObject(CObject* pObject);		

	void AddMissionEvent(MISSION_EVENT_INFO* pMissionEvent);

	void ClearRequital(MSG_INSDG_RANK_INFO* pRankRequitalInfo);	//던전 클리어 보상
	void ClearRequitalItem(CPlayer* pPlayer, WORD wItemIndex);

	BOOL IsMissionSuccess();

	void InsDungeonDestoryForCheckPlayer(CPlayer* pPlayer);

	inline DWORD GetChannel()	{ return m_dwChannel; }
	inline void SetChannel(DWORD dwChannel)	{ m_dwChannel = dwChannel; }

	inline DWORD GetPartyIDX()	{ return m_dwPartyIDX; }
	inline void SetPartyIDX(DWORD dwPartyIDX)	{ m_dwPartyIDX = dwPartyIDX; }

	inline DWORD GetMunpaIDX()	{ return m_dwMunpaIDX; }
	inline void SetMunpaIDX(DWORD dwMunpaIDX)	{m_dwMunpaIDX = dwMunpaIDX; }

	inline DWORD GetStartTime()	{ return m_dwInsDGStartTime; }
	inline void SetStartTime(DWORD dwTime)	{m_dwInsDGStartTime = dwTime; }

	inline DWORD GetUserCount()	{ return m_dwUserCount; }
	inline void IncreaseUserCount() { m_dwUserCount++; }
	inline void DecreaseUserCount() { m_dwUserCount--; }

	inline BOOL IsInsDGStartFlag() { return m_bInsDGStartFlag; }
	
	inline BOOL IsDestroyFlag() { return m_bDestroyFlag; }

	inline DWORD GetProgressTime()	{ return m_dwProgressTime; }

	inline CInsDGMission* GetCurMission()	{ return m_pInsDGMissionManager->GetCurMission(); }

	inline void SetRequitalTime(DWORD dwRequitalTime)	{ m_dwRequitalTime = dwRequitalTime; }
	inline DWORD GetRequitalTime()	{ return m_dwRequitalTime; }	

	void SetRankState(INSDG_RANK_STATE* pRankState);
	INSDG_RANK_STATE* GetRankState();	

	void InsDGClearRank();

	void SendInsDGClearNotifyMapServer();

	void InsDGFinalClearNotice();	//같은 종류의 인던 맵 최종 클리어 통보

	void AddPlayer(CPlayer* pPlayer);	
	void RemovePlayer(CPlayer* pPlayer);
	void RemoveAllPlayerTable();

	inline void MonsTerKillNumIncrease()	{ m_dwTotalKillMonsterNum++; }
	inline DWORD GetTotalKillMonsterNum()	{ return m_dwTotalKillMonsterNum; }

	void PartyBreakup();	//인던안에서 파티가 해체 되었을때 처리
	void PartyBan(CPlayer* pPlayer);	//인던안에서 해당 캐릭터가 강퇴 당했을때 처리

	inline BOOL IsClear()	{ return m_bClearRequital; }

protected:
	CInsDungeonRegenManager* m_pInsDungeonRegenManager;
	CInsDGMissionManager* m_pInsDGMissionManager;

	CYHHashTable<DWORD>	m_InsDungeonPlayerTable;	//인던에 입장한 플레이어 테이블

	INSDG_RANK_STATE m_RankState;	//랭크 값
	DWORD m_dwChannel;				//채널ID
	DWORD m_dwPartyIDX;				//파티 인덱스
	DWORD m_dwMunpaIDX;				//문파 인덱스	
	DWORD m_dwInsDGStartTime;		//인던 시작시간
	DWORD m_dwUserCount;			//유저 카운트	
	DWORD m_dwProgressTime;			//진행시간
	DWORD m_dwReturnToMapWaitTime;	//미션 클리어 후 유저 내보내기 대기 시간
	DWORD m_dwRequitalTime;			//보상 미션 시간
	DWORD m_dwTotalKillMonsterNum; //인던에서 사냥한 몬스터 총 수
	BOOL m_bInsDGStartFlag;			//인던 시작 플래그
	BOOL m_bDestroyFlag;			//인던 삭제 플래그

	BOOL m_bClearRequital;		//보상 처리 한번만 하기 위해서 필요	
	BOOL m_bPartyBreakup;		//파티가 해체 되었는지 체크	
};

#endif //_INSDUNGEON_H
