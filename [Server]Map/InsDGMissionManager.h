#ifndef _INSDGMISSIONMANAGER_H
#define _INSDGMISSIONMANAGER_H

#include "InsDGMission.h"

class CInsDungeon;
class CInsDungeonRegenManager;

class CInsDGMissionManager
{
public:
	CInsDGMissionManager(CInsDungeon* pInsDungeon, CInsDungeonRegenManager* pInsDungeonRegenManager);
	~CInsDGMissionManager();

	BOOL LoadInsDGMissionInfoList();
	void RemoveInsDGMissionInfoList();
	void RemoveInsDGMissionEventList();

	void Process();

	void AddMissionEvent(MISSION_EVENT_INFO* pMissionEvent);
	BOOL IsSuccess()	{ return m_bSuccess; }

	CInsDGMission* GetCurMission()	{ return m_pCurrentMission; }

	void SendMissionResult(CInsDGMission* pMission);
	void MissionMonsterRegen(CInsDGMission* pMission);
	void MissionRequital(CInsDGMission* pMission);

	void MissionRequitalItem(CPlayer* pPlayer, MISSION_REQUITAL_ITEM* pMissionRequitalItem);

private:
	CYHHashTable<CInsDGMission>	m_InsDGMissionTable;

	CInsDGMission* m_pCurrentMission;
	CYHHashTable<MISSION_EVENT_INFO> m_MissionEvent;
	
	CInsDungeon* m_pInsDungeon;
	CInsDungeonRegenManager* m_pInsDungeonRegenManager;
	DWORD m_dwMissionEventNum;
	BOOL m_bSuccess;	//��� �̼��� �Ϸ�Ǿ����� �����ϴ� �÷���
	DWORD m_dwMissionMonsterMaxRegenNum;	//�̼����� ��ȯ�� ���� ���� ��
};

#endif //_INSDGMISSIONMANAGER_H