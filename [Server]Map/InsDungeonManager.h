#ifndef _INSDUNGEONMANAGER_H
#define _INSDUNGEONMANAGER_H

#define INSDUNGEONMGR	USINGTON(CInsDungeonManager)

#include "InsDGMissionEvent.h"

class CPlayer;
class CInsDungeon;

struct INSDUNGEON_REQUITAL
{
	DWORD dwInsDGIndex;	//�δ� �ε���
	WORD wPlayerCount;	//�ο�
	DWORD dwExp;		//���� ����ġ
	DWORD dwAbilityExp;	//���� ����ġ
	MONEYTYPE dwMoney;	//���� ��
	WORD wItemIDX;		//���� ������
};

class CInsDungeonManager
{
public:
	CInsDungeonManager();
	~CInsDungeonManager();

	void Init();

	void NetworkMsgParse( DWORD dwConnectionIndex, BYTE Protocol, void* pMsg );
	
	BOOL LoadEntranceConditionInfo();	//���� ���� ������ �ε�
	void RemoveEntranceConditionInfo();	//���� ���� ������ ����
	eINSDG_ENTRANCE_ERROR CheckEntranceCondition(CPlayer* pPlayer,DWORD dwNPCUniqueIDX);	//���� ���� üũ

	BOOL LoadInsDungeonInfo();		//���� ���� �ε�
	
	BOOL LoadInsDGRequitalInfo();	//���� ���� ���� �ε�
	void RemoveInsDGRequitalInfo();	//���� ���� ���� ����
	INSDUNGEON_REQUITAL* GetInsDGRequitalInfo(DWORD dwInsDGIndex, DWORD dwCount);

	BOOL CreateInsDungeon(DWORD dwChannel, CPlayer* pPlayer);		//�δ� ����
	void RemoveInsDungeon(DWORD dwChennel);		//�δ� ����
	void RemoveAllInsDungeon();		//�δ� ���� ����
	DWORD GetInsDungeonChennel(DWORD dwIndex);	//�ش� �ε����� �δ� ä�� ���� ���� (�ε���)
	DWORD GetInsDungeonChennel(CPlayer* pPlayer);	//�ش� �ε����� �δ� ä�� ���� ���� (�÷��̾�)

	void DeleteMissionItem(CPlayer* pPlayer);	//�̼� �������� ��� �����
	
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

	BOOL LoadRankStandardInfo();	//�δ� ��ũ ��� ���� ������ �ε�
	void RemoveRankStandardInfo();	//�δ� ��ũ ��� ���� ������ ����
	INSDG_RANK_STANDARD* GetRankStandardInfo(DWORD dwInsDGIndex);

	DWORD ResultRank(DWORD dwInsDGIndex, DWORD dwClearTime, DWORD dwMonsterKillNum, DWORD dwPlayerCount);
	float RankRequitalRatio(DWORD dwRankValue);
	DWORD GetRank(DWORD dwRankValue);

	CYHHashTable<INSDG_KIND_INFO>* GetInsDungeonKindMapTable();
	void RmoveInsDungeonKindMapTable();
	
	inline BOOL IsFinalMap()	{ return m_InsDungeonInfo.bFinalMap; }	//������ �������� ����

	void PartyBreakup(DWORD dwPartyID); //�δ��ȿ��� ��Ƽ�� ��ü �Ǿ����� ó��
	void PartyBan(CPlayer* pPlayer);	//�δ��ȿ��� �ش� ĳ���Ͱ� ���� �������� ó��

private:
	CYHHashTable<CInsDungeon>	m_InsDungeonTable;	//�� ä�� �δ� ���̺�
	CYHHashTable<INSDG_ENTRANCECONDITION_INFO>	m_EntranceConditionTable;	//�������� ������ ���̺�
	CYHHashTable<INSDUNGEON_REQUITAL>	m_InsDungeonRequitalTable;	//�ʺ� ���� ���� ���� ���̺�	
	CYHHashTable<INSDG_RANK_STANDARD>	m_InsDungeonRankStandardTable;	//�δ� ��ũ ��� ���� ������
	CYHHashTable<INSDG_KIND_INFO>	m_InsDungeonKindMapTable;	//���� �δ��� �ʹ�ȣ���� ����

	INSDUNGEON_INFO m_InsDungeonInfo;	//���� ����	
};

EXTERNGLOBALTON(CInsDungeonManager);

#endif //_INSDUNGEONMANAGER_H
