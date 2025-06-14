#ifndef _INSDUNGEONMISSIONDATAMANAGER_H
#define _INSDUNGEONMISSIONDATAMANAGER_H

#define INSDUNGEONMISSIONDATAMGR	USINGTON(CInsDungeonMissionDataManager)

#include "MHFile.h"

class CInsDGMission;
class CInsDGMissionRegenGroup;

class CInsDungeonMissionDataManager
{
public:
	CInsDungeonMissionDataManager();
	~CInsDungeonMissionDataManager();

	/////// ��ũ��Ʈ ������ �ε� ���� �Լ� //////////////////////////////////
	BOOL LoadInsDungeonMissionList(DWORD dwMissionGroupIndex);
	void RemoveAllInsDGMissionList();

	BOOL LoadMissionGroupData(CMHFile* pFile, DWORD dwMissionGroupIDX);
	CInsDGMission* LoadMissionInfoData(CMHFile* pFile, DWORD dwMissionGroupIDX, DWORD dwMissionIDX);

	BOOL AddMissionItemInfo(WORD wItemIndex, DWORD dwMissionKind);
	INSDG_MISSION_ITEM_INFO* GetMissionItemInfo(WORD wItemIndex);
	void RemoveAllInsDGMissionItemInfo();
	/////////////////////////////////////////////////////////////////////////

	/////// ���� �̼� ������ �Ҵ� �Լ� ///////////////////////////////////////////
	CInsDGMission* LoadMissionInfoData(CInsDGMission* pMission);
	/////////////////////////////////////////////////////////////////////////

	/////// �̼� ���� ���� �Լ� //////////////////////////////////
	BOOL LoadMissionRegen();
	void RemoveAllInsDGMissionRegenList();
	CInsDGMissionRegenGroup* GetInsDGMissionRegenGroup(DWORD dwGroupID);
	/////////////////////////////////////////////////////////////////////////

	inline CYHHashTable<CInsDGMission>* GetInsDGMissionTable()	{ return &m_InsDGMissionTable; }

private:
	CYHHashTable<CInsDGMission>	m_InsDGMissionTable;
	CYHHashTable<INSDG_MISSION_ITEM_INFO>	m_InsDGMissionItemInfoTable;
	CYHHashTable<CInsDGMissionRegenGroup>	m_InsDGMissionRegenGroupTable;
};

EXTERNGLOBALTON(CInsDungeonMissionDataManager);

#endif //_INSDUNGEONMISSIONDATAMANAGER_H