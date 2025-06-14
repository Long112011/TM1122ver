#ifndef _INSDUNGEONMANAGER_H
#define _INSDUNGEONMANAGER_H

#define INSDUNGEONMGR	USINGTON(CInsDungeonManager)

#include "InsDGMissionEvent.h"

class CPlayer;
class CInsDungeon;

struct INSDUNGEON_REQUITAL
{
	DWORD dwInsDGIndex;	//인던 인덱스
	WORD wPlayerCount;	//인원
	DWORD dwExp;		//보상 경험치
	DWORD dwAbilityExp;	//보상 수련치
	MONEYTYPE dwMoney;	//보상 돈
	WORD wItemIDX;		//보상 아이탬
};

class CInsDungeonManager
{
public:
	CInsDungeonManager();
	~CInsDungeonManager();

	void Init();

	void NetworkMsgParse( DWORD dwConnectionIndex, BYTE Protocol, void* pMsg );
	
	BOOL LoadEntranceConditionInfo();	//입장 조건 데이터 로드
	void RemoveEntranceConditionInfo();	//입장 조건 데이터 삭제
	eINSDG_ENTRANCE_ERROR CheckEntranceCondition(CPlayer* pPlayer,DWORD dwNPCUniqueIDX);	//입장 조건 체크

	BOOL LoadInsDungeonInfo();		//던전 정보 로드
	
	BOOL LoadInsDGRequitalInfo();	//던전 보상 정보 로드
	void RemoveInsDGRequitalInfo();	//던전 보상 정보 삭제
	INSDUNGEON_REQUITAL* GetInsDGRequitalInfo(DWORD dwInsDGIndex, DWORD dwCount);

	BOOL CreateInsDungeon(DWORD dwChannel, CPlayer* pPlayer);		//인던 생성
	void RemoveInsDungeon(DWORD dwChennel);		//인던 삭제
	void RemoveAllInsDungeon();		//인던 전부 삭제
	DWORD GetInsDungeonChennel(DWORD dwIndex);	//해당 인덱스의 인던 채널 정보 리턴 (인덱스)
	DWORD GetInsDungeonChennel(CPlayer* pPlayer);	//해당 인덱스의 인던 채널 정보 리턴 (플레이어)

	void DeleteMissionItem(CPlayer* pPlayer);	//미션 아이탬을 모두 지운다
	
	void Process();

	inline INSDUNGEON_INFO* GetDungeonInfo()	{ return &m_InsDungeonInfo; }

	CInsDungeon* GetInsDungeon(DWORD dwChannel);
	
	void RemoveObject(CObject* pObject);

	void InsDGEntrance(MSG_DWORD_WORD3* pMsg);
	void InsDGEnter(MSG_INSDG_ENTER* pMsg, DWORD dwConnectionIndex);
	void InsDGLeave(MSG_DWORD4* pMsg, DWORD dwConnectionIndex);

	void SendInsDungeonInfo(CPlayer* pPlayer);

	void AddMissionEvent(MISSION_EVENT_INFO* pMissionEvent);

	void InsDungeonDestoryForCheckPlayer(CPlayer* pPlayer);

	BOOL LoadRankStandardInfo();	//인던 랭크 계산 기준 데이터 로드
	void RemoveRankStandardInfo();	//인던 랭크 계산 기준 데이터 삭제
	INSDG_RANK_STANDARD* GetRankStandardInfo(DWORD dwInsDGIndex);

	DWORD ResultRank(DWORD dwInsDGIndex, DWORD dwClearTime, DWORD dwMonsterKillNum, DWORD dwPlayerCount);
	float RankRequitalRatio(DWORD dwRankValue);
	DWORD GetRank(DWORD dwRankValue);

	CYHHashTable<INSDG_KIND_INFO>* GetInsDungeonKindMapTable();
	void RmoveInsDungeonKindMapTable();
	
	inline BOOL IsFinalMap()	{ return m_InsDungeonInfo.bFinalMap; }	//마지막 던전인지 구분

	void PartyBreakup(DWORD dwPartyID); //인던안에서 파티가 해체 되었을때 처리
	void PartyBan(CPlayer* pPlayer);	//인던안에서 해당 캐릭터가 강퇴 당했을때 처리

private:
	CYHHashTable<CInsDungeon>	m_InsDungeonTable;	//각 채널 인던 테이블
	CYHHashTable<INSDG_ENTRANCECONDITION_INFO>	m_EntranceConditionTable;	//입장조건 데이터 테이블
	CYHHashTable<INSDUNGEON_REQUITAL>	m_InsDungeonRequitalTable;	//맵별 던전 보상 정보 테이블	
	CYHHashTable<INSDG_RANK_STANDARD>	m_InsDungeonRankStandardTable;	//인던 랭크 계산 기준 데이터
	CYHHashTable<INSDG_KIND_INFO>	m_InsDungeonKindMapTable;	//같은 인던의 맵번호들을 저장

	INSDUNGEON_INFO m_InsDungeonInfo;	//던전 정보	
};

EXTERNGLOBALTON(CInsDungeonManager);

#endif //_INSDUNGEONMANAGER_H
