#include "stdafx.h"
#include "InsDungeonRegenManager.h"
#include "InsDungeonRegenGroup.h"
#include "..\[CC]Header\GameResourceManager.h"
#include "MHFile.h"
#include "InsDungeonRegenConditionInfo.h"
#include "InsDungeonRegenObjectInfo.h"
#include "InsDungeonRegenDataManager.h"
#include "Monster.h"

CInsDungeonRegenManager::CInsDungeonRegenManager()
{
	m_InsDGRegenGroupTable.Initialize(100);
	m_dwChannel = 0;
}

CInsDungeonRegenManager::~CInsDungeonRegenManager()
{
	RemoveAllInsDGRegenGroup();
}

BOOL CInsDungeonRegenManager::LoadInsDungeonRegenData()
{	
	//기본 인던 리젠 데이터의 값을 셋팅한다.
	CYHHashTable<CInsDungeonRegenGroup>* pInsDungeonRegenGroupTable = INSDUNGEONREGENDATAMGR->GetInsDGRegenGroupTable();

	CInsDungeonRegenGroup* pInsDungeonRegenGroup = NULL;
	pInsDungeonRegenGroupTable->SetPositionHead();	
	while(pInsDungeonRegenGroup = pInsDungeonRegenGroupTable->GetData())
	{
		CInsDungeonRegenGroup* pRegenGroup = new CInsDungeonRegenGroup;
		pInsDungeonRegenGroup->LoadInsDGRegenInfoData(pRegenGroup, m_dwChannel);
		m_InsDGRegenGroupTable.Add(pRegenGroup, pRegenGroup->GetGroupID());
	}

	return TRUE;
}

void CInsDungeonRegenManager::RemoveAllInsDGRegenGroup()
{
	CInsDungeonRegenGroup* pRegenGroup = NULL;

	m_InsDGRegenGroupTable.SetPositionHead();
	while (pRegenGroup = m_InsDGRegenGroupTable.GetData())
	{
		SAFE_DELETE(pRegenGroup);
	}
	m_InsDGRegenGroupTable.RemoveAll();
}

void CInsDungeonRegenManager::Process()
{
	CInsDungeonRegenGroup* pRegenGroup = NULL;

	m_InsDGRegenGroupTable.SetPositionHead();
	while (pRegenGroup = m_InsDGRegenGroupTable.GetData())
	{
		pRegenGroup->Process();
	}	
}

DWORD CInsDungeonRegenManager::GetMonsterMaxRegenNum()
{
	DWORD dwNum = 0;

	CInsDungeonRegenGroup* pRegenGroup = NULL;

	m_InsDGRegenGroupTable.SetPositionHead();
	while (pRegenGroup = m_InsDGRegenGroupTable.GetData())
	{
		dwNum += pRegenGroup->GetMaxRegenNum();
	}	

	return dwNum;
}

DWORD CInsDungeonRegenManager::GetMonsterCurRegenNum()
{
	DWORD dwNum = 0;

	CInsDungeonRegenGroup* pRegenGroup = NULL;

	m_InsDGRegenGroupTable.SetPositionHead();
	while (pRegenGroup = m_InsDGRegenGroupTable.GetData())
	{
		dwNum += pRegenGroup->GetCurRegenNum();
	}	

	return dwNum;
}

void CInsDungeonRegenManager::RemoveObject(CObject* pObject)
{
	WORD wGroupID = ((CMonster*)pObject)->GetMonsterGroupNum();
	CInsDungeonRegenGroup* pRegenGroup = m_InsDGRegenGroupTable.GetData(wGroupID);
	if(pRegenGroup == NULL)
		return;

	pRegenGroup->RemoveObject(pObject);
}