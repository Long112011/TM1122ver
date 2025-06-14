#include "StdAfx.h"
#include "InsDGMission.h"
#include "InsDGMissionExecute_Hunt.h"
#include "InsDGMissionExecute_Item.h"
#include "InsDGMissionExecute_NPC.h"
#include "Player.h"

CInsDGMission::CInsDGMission(DWORD dwMissionGroupIDX, DWORD dwMissionIDX)
{
	m_dwMissionGroupIDX = dwMissionGroupIDX;
	m_dwMissionIDX = dwMissionIDX;
	memset(&m_MissionRequital, 0, sizeof(m_MissionRequital));
	m_MissionRequitalItemTable.Initialize(20);

	m_pMissionEvent = NULL;
	m_pMissionExecute = NULL;
	m_dwMissionRegenGroupID = 0;
	m_dwNextMissionIDX = 0;	
	m_dwMissionRequitalItemMaxNum = 0;
	m_bSuccessFlag = FALSE;
}

CInsDGMission::~CInsDGMission()
{
	RemoveMissionRequitalItemList();
	SAFE_DELETE(m_pMissionEvent);
	SAFE_DELETE(m_pMissionExecute);	
}

CInsDGMissionEvent* CInsDGMission::CreateMissionEvent(DWORD dwMissionEventKind)
{	
	m_pMissionEvent = new CInsDGMissionEvent(dwMissionEventKind);

	return m_pMissionEvent;
}

CInsDGMissionExecute* CInsDGMission::CreateMissionExecute(DWORD dwMissionExecuteKind, DWORD dwMissionIDX, DWORD dwParam1, DWORD dwParam2, DWORD dwParam3)
{	
	switch(dwMissionExecuteKind)
	{
	case eMISSION_EXECUTE_KIND_MONSTER_TARGETKILL:
	case eMISSION_EXECUTE_KIND_MONSTER_KILL:
	case eMISSION_EXECUTE_KIND_MONSTER_KILLALL:		
		{
			m_pMissionExecute = new CInsDGMissionExecute_Hunt(dwMissionExecuteKind, dwMissionIDX, dwParam1, dwParam2, dwParam3);
		}
		break;
	case eMISSION_EXECUTE_KIND_ITEM_USE:
	case eMISSION_EXECUTE_KIND_ITEM_GET:
	case eMISSION_EXECUTE_KIND_ITEM_TARGET_GET:
		{
			m_pMissionExecute = new CInsDGMissionExecute_Item(dwMissionExecuteKind, dwMissionIDX, dwParam1, dwParam2, dwParam3);
		}
		break;
	case eMISSION_EXECUTE_KIND_NPC_TALK:
		{
			m_pMissionExecute = new CInsDGMissionExecute_NPC(dwMissionExecuteKind, dwMissionIDX, dwParam1, dwParam2, dwParam3);
		}
		break;
	}

	return m_pMissionExecute;
}

void CInsDGMission::OnMissionEvent(MISSION_EVENT_INFO* pMissionEvent)
{
	m_pMissionExecute->Execute(pMissionEvent);

	//미셔 완료 체크
	if(m_pMissionExecute->IsSuccess() == TRUE)
	{
		m_bSuccessFlag = TRUE;
	}
}

void CInsDGMission::SetMissionRequital(MISSION_REQUITAL* pMissionRequital)
{
	m_MissionRequital.dwExp = pMissionRequital->dwExp;
	m_MissionRequital.dwMoney = pMissionRequital->dwMoney;
	m_MissionRequital.dwSExp = pMissionRequital->dwSExp;
	m_MissionRequital.dwTime = pMissionRequital->dwTime;
}

void CInsDGMission::AddMissionRequitalItem(MISSION_REQUITAL_ITEM* pMissionRequitalItem)
{
	MISSION_REQUITAL_ITEM* pRequitalItem = new MISSION_REQUITAL_ITEM;	

	pRequitalItem->wItemIndex = pMissionRequitalItem->wItemIndex;
	pRequitalItem->dwNum = pMissionRequitalItem->dwNum;
	
	m_MissionRequitalItemTable.Add(pRequitalItem, m_dwMissionRequitalItemMaxNum);
	m_dwMissionRequitalItemMaxNum++;
}

void CInsDGMission::RemoveMissionRequitalItemList()
{
	MISSION_REQUITAL_ITEM* pMissionRequitalItem = NULL;

	m_MissionRequitalItemTable.SetPositionHead();
	while(pMissionRequitalItem = m_MissionRequitalItemTable.GetData())
	{
		SAFE_DELETE(pMissionRequitalItem);
	}
	m_MissionRequitalItemTable.RemoveAll();
}