#include "stdafx.h"
#include "InsDungeonRegenGroup.h"
#include "RecallManager.h"
#include "Monster.h"

CInsDungeonRegenGroup::CInsDungeonRegenGroup()
{
	m_InsDGRegenObjectInfoTable.Initialize(100);
	m_pRegenConditionInfo = NULL;
	m_dwGroupID = 0;
	m_dwRegenObjectCount = 0;
	m_dwChannel = 0;
	m_dwCurRegenNum = 0;
	m_dwMaxRegenNum = 0;
	m_dwRegenType = eREGENTYPE_INFINITY;	
}

CInsDungeonRegenGroup::~CInsDungeonRegenGroup()
{
	RemoveInsDGRegenInfoList();
	SAFE_DELETE(m_pRegenConditionInfo);
}

void CInsDungeonRegenGroup::RemoveInsDGRegenInfoList()
{
	CInsDungeonRegenObjectInfo* pInfo = NULL;
	m_InsDGRegenObjectInfoTable.SetPositionHead();
	while (pInfo = m_InsDGRegenObjectInfoTable.GetData())
	{
		SAFE_DELETE(pInfo);
	}
	m_InsDGRegenObjectInfoTable.RemoveAll();
}
#include "AISystem.h"
BOOL CInsDungeonRegenGroup::LoadInsDGRegenInfoData(CInsDungeonRegenGroup* pRegenGroup, DWORD dwChannel)
{
	if(m_pRegenConditionInfo == NULL)
		return FALSE;	

	DWORD dwGroupID = 0;
	DWORD dwObjectID = 0;
	BYTE ObjectKind = 0;
	WORD wMonsterKind = 0;	
	BOOL bRegen = FALSE;
	DWORD dwKey = 0;
	VECTOR3* vPos = NULL;
	
	pRegenGroup->SetChannel(dwChannel);
	pRegenGroup->SetGroupID(m_dwGroupID);
	pRegenGroup->SetRegenType(m_dwRegenType);
	
	//RegenObjectInfo 정보 셋팅
	CInsDungeonRegenObjectInfo* pRegenObjectInfo = NULL;
	m_InsDGRegenObjectInfoTable.SetPositionHead();
	while (pRegenObjectInfo = m_InsDGRegenObjectInfoTable.GetData())
	{
		dwGroupID = pRegenObjectInfo->GetGroupID();
		dwObjectID = pRegenObjectInfo->GetObjectID() + g_pAISystem->GeneraterMonsterID();
		ObjectKind = pRegenObjectInfo->GetObjectKind();
		wMonsterKind = pRegenObjectInfo->GetMonsterKind();
		bRegen = pRegenObjectInfo->IsRegen();
		dwKey = pRegenObjectInfo->GetKey();
		vPos = pRegenObjectInfo->GetPosition();

		CInsDungeonRegenObjectInfo* pInfo = new CInsDungeonRegenObjectInfo;		
		pInfo->SetGroupID(dwGroupID);
		pInfo->SetObjectID(dwObjectID);
		pInfo->SetObjectKind(ObjectKind);
		pInfo->SetMonsterKind(wMonsterKind);
		pInfo->SetRegen(bRegen);
		pInfo->SetKey(dwKey);
		pInfo->SetPosition(vPos);
		pRegenGroup->AddInsDGRegenObjectInfo(pInfo);
	}

	//ConditionInfo 정보 셋팅
	DWORD dwConditionGroupID = m_pRegenConditionInfo->GetGroupID();
	float fRatio = m_pRegenConditionInfo->GetRatio();
	//DWORD dwRegenStartTime = m_pRegenConditionInfo->GetRegenStartTime();
	DWORD dwRegenStartTime = 0;
	DWORD dwDelayTime = m_pRegenConditionInfo->GetDelayTime();
	DWORD dwRegenType = m_dwRegenType;
	BOOL bConditionRegen = m_pRegenConditionInfo->IsRegen();

	CInsDungeonRegenConditionInfo* pRegenConditionInfo = new CInsDungeonRegenConditionInfo;
	pRegenConditionInfo->SetGroupID(dwConditionGroupID);
	pRegenConditionInfo->SetRatio(fRatio);
	pRegenConditionInfo->SetRegenStartTime(dwRegenStartTime);
	pRegenConditionInfo->SetDelayTime(dwDelayTime);	
	pRegenConditionInfo->SetRegen(bConditionRegen);
	pRegenGroup->SetRegenConditionInfo(pRegenConditionInfo);

	return TRUE;
}

void CInsDungeonRegenGroup::AddInsDGRegenObjectInfo(CInsDungeonRegenObjectInfo* pInsDGRegenObjectInfo)
{	
	m_dwRegenObjectCount++;
	pInsDGRegenObjectInfo->SetKey(m_dwRegenObjectCount);
	m_InsDGRegenObjectInfoTable.Add(pInsDGRegenObjectInfo, m_dwRegenObjectCount);	
}

void CInsDungeonRegenGroup::MonsterRegen(DWORD dwChannel)
{	
	BYTE ObjectKind = 0;
	WORD wMonsterKind = 0;
	DWORD dwGroupID = 0;
	VECTOR3* vPos = NULL;
	WORD wRadius = 0;
	CMonster* pMonster = NULL;
	CInsDungeonRegenObjectInfo* pInfo = NULL;

	m_InsDGRegenObjectInfoTable.SetPositionHead();
	while (pInfo = m_InsDGRegenObjectInfoTable.GetData())
	{
		if( pInfo->IsRegen() == FALSE )
			continue;

		ObjectKind = pInfo->GetObjectKind();
		wMonsterKind = pInfo->GetMonsterKind();
		dwGroupID = pInfo->GetGroupID();
		vPos = pInfo->GetPosition();

		pMonster = RECALLMGR->InsDungeonRecall(ObjectKind, wMonsterKind, dwGroupID, dwChannel, vPos, wRadius);
		if(pMonster != NULL)
		{
			pInfo->SetObjectID(pMonster->GetID());
			m_dwCurRegenNum++;	//몬스터 리젠 수
			m_dwMaxRegenNum++;  //총 몬스터 리젠 수 
			pInfo->SetRegen(FALSE);			
		}
	}
}

void CInsDungeonRegenGroup::RegenCheck(DWORD CurObjectNum, DWORD MaxObjectNum)
{
	if(m_pRegenConditionInfo == NULL)	//예외처리
		return;

	float ratio = (float)CurObjectNum/(float)MaxObjectNum;
	float fConditionRatio = m_pRegenConditionInfo->GetRatio();	

	if(fConditionRatio != 0)
	{	
		if( ratio <= ( 1 - fConditionRatio ) )
		{
			if(m_pRegenConditionInfo->IsRegen() == FALSE)
			{
				m_pRegenConditionInfo->SetRegenStartTime(gCurTime);
				m_pRegenConditionInfo->SetRegen(TRUE);
			}			
		}
	}	
}

void CInsDungeonRegenGroup::Process()
{
	if(m_pRegenConditionInfo == NULL)
		return;

	//리젠 타입에 따라 처리한다.
	switch(m_dwRegenType)
	{
	case eREGEN_TYPE::eREGENTYPE_ONE:		//한번 리젠
		{			
			if(m_pRegenConditionInfo->IsRegen() == TRUE)	//리젠 안되어있다
			{
				this->MonsterRegen(m_dwChannel);
				m_pRegenConditionInfo->SetRegen(FALSE);
			}
		}
		break;
	case eREGEN_TYPE::eREGENTYPE_INFINITY:		//무한 리젠
		{
			this->RegenCheck(m_dwCurRegenNum, m_dwRegenObjectCount);

			if(m_pRegenConditionInfo->IsRegen() == TRUE)
			{
				DWORD dwRegenStartTime = m_pRegenConditionInfo->GetRegenStartTime();

				if(dwRegenStartTime + m_pRegenConditionInfo->GetDelayTime() < gCurTime)
				{
					m_pRegenConditionInfo->SetRegen(FALSE);
					this->MonsterRegen(m_dwChannel);
				}			
			}		
		}
		break;
	}
}

void CInsDungeonRegenGroup::RemoveObject(CObject* pObject)
{
	if(m_dwCurRegenNum > 0)
		m_dwCurRegenNum--;
	else
		m_dwCurRegenNum = 0;

	CInsDungeonRegenObjectInfo* pInfo = NULL;

	m_InsDGRegenObjectInfoTable.SetPositionHead();
	while (pInfo = m_InsDGRegenObjectInfoTable.GetData())
	{
		//몬스터 리젠 상태 갱신
		if( pInfo->GetObjectID() == pObject->GetID() )
			pInfo->SetRegen(TRUE);
	}
}
