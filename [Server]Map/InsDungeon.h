#ifndef _INSDUNGEON_H
#define _INSDUNGEON_H

#include "InsDungeonRegenManager.h"
#include "InsDGMissionManager.h"

class CPlayer;

class CInsDungeon
{
	enum
	{
		eRETURN_TO_MAP_WAIT_TIME = 10000,	//�̼� Ŭ���� �� �������� ���ð�		
	};

public:
	CInsDungeon();
	~CInsDungeon();

	void Init();
	void Destory();

	void InitInsDGStartTime();	//�δ� �ð� �ʱ�ȭ
	void StartInsDGStartTime();	//�δ� �ð� ����

	void Process();

	void ReturnToMap();
	void PartyReturnToMap();
	void PartyBreakupReturnToMap();

	void RemoveObject(CObject* pObject);		

	void AddMissionEvent(MISSION_EVENT_INFO* pMissionEvent);

	void ClearRequital(MSG_INSDG_RANK_INFO* pRankRequitalInfo);	//���� Ŭ���� ����
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

	void InsDGFinalClearNotice();	//���� ������ �δ� �� ���� Ŭ���� �뺸

	void AddPlayer(CPlayer* pPlayer);	
	void RemovePlayer(CPlayer* pPlayer);
	void RemoveAllPlayerTable();

	inline void MonsTerKillNumIncrease()	{ m_dwTotalKillMonsterNum++; }
	inline DWORD GetTotalKillMonsterNum()	{ return m_dwTotalKillMonsterNum; }

	void PartyBreakup();	//�δ��ȿ��� ��Ƽ�� ��ü �Ǿ����� ó��
	void PartyBan(CPlayer* pPlayer);	//�δ��ȿ��� �ش� ĳ���Ͱ� ���� �������� ó��

	inline BOOL IsClear()	{ return m_bClearRequital; }

protected:
	CInsDungeonRegenManager* m_pInsDungeonRegenManager;
	CInsDGMissionManager* m_pInsDGMissionManager;

	CYHHashTable<DWORD>	m_InsDungeonPlayerTable;	//�δ��� ������ �÷��̾� ���̺�

	INSDG_RANK_STATE m_RankState;	//��ũ ��
	DWORD m_dwChannel;				//ä��ID
	DWORD m_dwPartyIDX;				//��Ƽ �ε���
	DWORD m_dwMunpaIDX;				//���� �ε���	
	DWORD m_dwInsDGStartTime;		//�δ� ���۽ð�
	DWORD m_dwUserCount;			//���� ī��Ʈ	
	DWORD m_dwProgressTime;			//����ð�
	DWORD m_dwReturnToMapWaitTime;	//�̼� Ŭ���� �� ���� �������� ��� �ð�
	DWORD m_dwRequitalTime;			//���� �̼� �ð�
	DWORD m_dwTotalKillMonsterNum; //�δ����� ����� ���� �� ��
	BOOL m_bInsDGStartFlag;			//�δ� ���� �÷���
	BOOL m_bDestroyFlag;			//�δ� ���� �÷���

	BOOL m_bClearRequital;		//���� ó�� �ѹ��� �ϱ� ���ؼ� �ʿ�	
	BOOL m_bPartyBreakup;		//��Ƽ�� ��ü �Ǿ����� üũ	
};

#endif //_INSDUNGEON_H
