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

	/////// 스크립트 데이터 로딩 관련 함수 //////////////////////////////////
	BOOL LoadInsDungeonMissionList(DWORD dwMissionGroupIndex);
	void RemoveAllInsDGMissionList();

	BOOL LoadMissionGroupData(CMHFile* pFile, DWORD dwMissionGroupIDX);
	CInsDGMission* LoadMissionInfoData(CMHFile* pFile, DWORD dwMissionGroupIDX, DWORD dwMissionIDX);

	BOOL AddMissionItemInfo(WORD wItemIndex, DWORD dwMissionKind);
	INSDG_MISSION_ITEM_INFO* GetMissionItemInfo(WORD wItemIndex);
	void RemoveAllInsDGMissionItemInfo();
	/////////////////////////////////////////////////////////////////////////

	/////// 던전 미션 데이터 할당 함수 ///////////////////////////////////////////
	CInsDGMission* LoadMissionInfoData(CInsDGMission* pMission);
	/////////////////////////////////////////////////////////////////////////

	/////// 미션 리젠 관련 함수 //////////////////////////////////
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