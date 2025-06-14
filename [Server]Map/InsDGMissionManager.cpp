#include "StdAfx.h"
#include "InsDGMissionManager.h"
#include "InsDungeonMissionDataManager.h"
#include "InsDungeon.h"
#include "Player.h"
#include "Party.h"
#include "UserTable.h"
#include "PartyManager.h"
#include "InsDGMissionRegenGroup.h"
#include "InsDGMissionRegenInfo.h"
#include "RecallManager.h"
#include "ItemManager.h"

CInsDGMissionManager::CInsDGMissionManager(CInsDungeon* pInsDungeon, CInsDungeonRegenManager* pInsDungeonRegenManager)
{
	m_pInsDungeon = pInsDungeon;
	m_pInsDungeonRegenManager = pInsDungeonRegenManager;

	m_InsDGMissionTable.Initialize(100);
	m_MissionEvent.Initialize(100);
	m_pCurrentMission = NULL;	
	m_dwMissionEventNum = 0;
	m_bSuccess = FALSE;
	m_dwMissionMonsterMaxRegenNum = 0;
}

CInsDGMissionManager::~CInsDGMissionManager()
{
	RemoveInsDGMissionInfoList();
	RemoveInsDGMissionEventList();
}

BOOL CInsDGMissionManager::LoadInsDGMissionInfoList()
{
	CYHHashTable<CInsDGMission>* pInsDGMissionTable = INSDUNGEONMISSIONDATAMGR->GetInsDGMissionTable();
	if(pInsDGMissionTable == NULL)
		return FALSE;	

	CInsDGMission* pMission = NULL;
	CInsDGMission* pInsDGMission = NULL;

	pInsDGMissionTable->SetPositionHead();
	while(pMission = pInsDGMissionTable->GetData())
	{
		pInsDGMission = INSDUNGEONMISSIONDATAMGR->LoadMissionInfoData(pMission);
		if(pInsDGMission == NULL)
			return FALSE;

		m_InsDGMissionTable.Add(pInsDGMission, pInsDGMission->GetMissionIDX());
		
		pMission = NULL;
		pInsDGMission = NULL;
	}

	//ù��° �̼����� ����
	m_pCurrentMission = m_InsDGMissionTable.GetData(1);
	if(m_pCurrentMission == NULL)
		return FALSE;

	return TRUE;
}

void CInsDGMissionManager::RemoveInsDGMissionInfoList()
{
	CInsDGMission* pMission = NULL;

	m_InsDGMissionTable.SetPositionHead();
	while(pMission = m_InsDGMissionTable.GetData())
	{
		SAFE_DELETE(pMission);
	}
	m_InsDGMissionTable.RemoveAll();
}

void CInsDGMissionManager::RemoveInsDGMissionEventList()
{
	MISSION_EVENT_INFO* pMissionEvent = NULL;

	m_MissionEvent.SetPositionHead();
	while(pMissionEvent = m_MissionEvent.GetData())
	{
		SAFE_DELETE(pMissionEvent);
	}
	m_MissionEvent.RemoveAll();
}

void CInsDGMissionManager::Process()
{
	if(m_pCurrentMission == NULL)
		return;

	if(m_dwMissionEventNum <= 0)
		return;

	MISSION_EVENT_INFO* pMissionEvent = NULL;
	m_MissionEvent.SetPositionHead();
	while(pMissionEvent = m_MissionEvent.GetData())
	{
		if(pMissionEvent->dwMissionEventKind != eMISSION_EVENT_KIND_NONE)
		{
			m_pCurrentMission->OnMissionEvent(pMissionEvent);
		}
	}

	RemoveInsDGMissionEventList();
	m_dwMissionEventNum = 0;

	//�̼� �Ϸ� üũ
	if(m_pCurrentMission->IsSuccess() == TRUE)
	{
		MissionRequital(m_pCurrentMission);
		MissionMonsterRegen(m_pCurrentMission);	//�̼��� �Ϸ�Ǹ� ���͸��� �׷��� ���� �Ǿ������� ������Ų��

		SendMissionResult(m_pCurrentMission);		
	}	
}

void CInsDGMissionManager::SendMissionResult(CInsDGMission* pMission)
{
	if(pMission == NULL)
		return;

	DWORD dwNextMissionIDX = pMission->GetNextMissionIDX();

	MSG_DWORD4 msg;
	msg.Category = MP_INSDUNGEON;
	msg.Protocol = MP_INSDUNGEON_MISSION_RESULT;

	//��Ƽ���鿡�� �޼��� ���
	DWORD dwPartyIDX = m_pInsDungeon->GetPartyIDX();
	CParty* pParty = PARTYMGR->GetParty(dwPartyIDX);
	if(pParty == NULL)
		return;
	
	//�Ϸ�� �̼� ������ Ŭ���̾�Ʈ�� ������
	CPlayer* pPlayer = NULL;
	DWORD dwPlayerID;
	for(int n=0;n<MAX_PARTY_LISTNUM;++n)
	{
		dwPlayerID = pParty->GetMemberID(n);
		if(dwPlayerID == 0)
			continue;

		// network send
		pPlayer = (CPlayer*)g_pUserTable->FindUser( dwPlayerID );
		if( pPlayer == NULL )
			continue;

		msg.dwObjectID = dwPlayerID;
		msg.dwData1 = pMission->GetMissionGroupIDX();
		msg.dwData2 = pMission->GetMissionIDX();
		msg.dwData3 = dwNextMissionIDX;		
		msg.dwData4 = m_pInsDungeon->GetRequitalTime();
		pPlayer->SendMsg(&msg, sizeof(msg));
	}		

	//���� �̼��� ������ ���� �̼����� ����
	if(dwNextMissionIDX != 0)
	{
		m_pCurrentMission = m_InsDGMissionTable.GetData(dwNextMissionIDX);		
	}
	else	//���� �̼��� ������ ����
	{
		m_bSuccess = TRUE;
		m_pCurrentMission = NULL;
	}	
}

void CInsDGMissionManager::AddMissionEvent(MISSION_EVENT_INFO* pMissionEvent)
{
	if(m_pCurrentMission == NULL)
		return;

	MISSION_EVENT_INFO* pEvent = new MISSION_EVENT_INFO;
	pEvent->dwExecuteData = pMissionEvent->dwExecuteData;
	pEvent->dwMissionEventKind = pMissionEvent->dwMissionEventKind;	
	pEvent->pPlayer = pMissionEvent->pPlayer;

	//���� ��� �̼��� ���� ���ͼ��� �־��ش�
	if(pEvent->dwMissionEventKind == eMISSION_EVENT_KIND_HUNT)
	{
		m_pInsDungeon->MonsTerKillNumIncrease();	//���� ų �� ����
		pEvent->dwData = (m_pInsDungeonRegenManager->GetMonsterMaxRegenNum() + m_dwMissionMonsterMaxRegenNum) - m_pInsDungeon->GetTotalKillMonsterNum();
	}
	else	//�������� �����. ���߿� �ʿ��Ҷ� ����.
		pEvent->dwData = 0;	

	m_MissionEvent.Add(pEvent, m_dwMissionEventNum);
	m_dwMissionEventNum++;
}

void CInsDGMissionManager::MissionMonsterRegen(CInsDGMission* pMission)
{
	DWORD dwRegenGroupID = pMission->GetMissionRegenGroupID();
	if(dwRegenGroupID == 0)
		return;

	CInsDGMissionRegenGroup* pInsDGMissionRegenGroup = INSDUNGEONMISSIONDATAMGR->GetInsDGMissionRegenGroup(dwRegenGroupID);
	if(pInsDGMissionRegenGroup == NULL)
		return;

	CInsDGMissionRegenInfo* pInsDGMissionRegenInfo = NULL;
	WORD wMonsterIndex = 0;	
	VECTOR3* vPos = NULL;
	CMonster* pMonster = NULL;
	
	DWORD dwMaxNum = pInsDGMissionRegenGroup->GetMaxNum();
	for (int i = 0 ; i < dwMaxNum ; i++)
	{
		pInsDGMissionRegenInfo = pInsDGMissionRegenGroup->GetInsDGMissionRegenInfo(i);
		if(pInsDGMissionRegenInfo == NULL)
			continue;

		wMonsterIndex = pInsDGMissionRegenInfo->GetMonsterIndex();
		vPos = pInsDGMissionRegenInfo->GetPos();

		pMonster = RECALLMGR->InsDungeonMissionRecall( wMonsterIndex, 1, m_pInsDungeon->GetChannel(), vPos, 10 );		
		
		if(pMonster != NULL)
			m_dwMissionMonsterMaxRegenNum++;

		pInsDGMissionRegenInfo = NULL;
	}
}

void CInsDGMissionManager::MissionRequital(CInsDGMission* pMission)
{
	if(pMission == NULL)
		return;	

	MISSION_REQUITAL* pMissionRequital = pMission->GetMissionRequital();
	if(pMissionRequital == NULL)
		return;

	CParty* pParty = PARTYMGR->GetParty(m_pInsDungeon->GetPartyIDX());
	if(pParty == NULL)
		return;

	CPlayer* pPlayer = NULL;
	DWORD dwPlayerID = 0;

	//���� �÷��̾�� ������ �ش�	
	for(int n=0;n<MAX_PARTY_LISTNUM;++n)
	{
		dwPlayerID = pParty->GetMemberID(n);
		if(dwPlayerID == 0)
			continue;

		// network send
		pPlayer = (CPlayer*)g_pUserTable->FindUser( dwPlayerID );
		if( pPlayer == NULL )
			continue;

		if(pMissionRequital->dwExp != 0)
			pPlayer->AddPlayerExpPoint(pMissionRequital->dwExp);

		if(pMissionRequital->dwMoney != 0)
			pPlayer->SetMoney( pMissionRequital->dwMoney, MONEY_ADDITION, MF_OBTAIN, eItemTable_Inventory, eMoneyLog_InstanceDungeonMissionRequital, pPlayer->GetID() );		

		if(pMissionRequital->dwSExp != 0)
			pPlayer->AddAbilityExp(pMissionRequital->dwSExp);

		for(int i = 0 ; i < pMission->GetMissionRequitalItemMaxNum() ; i++)
		{
			MissionRequitalItem(pPlayer, pMission->GetMissionRequitalItem(i));
		}
	}
			
	if(pMissionRequital->dwTime != 0)
	{
		DWORD dwTime = m_pInsDungeon->GetRequitalTime();
		dwTime += pMissionRequital->dwTime;
		m_pInsDungeon->SetRequitalTime(dwTime);
	}
}

void CInsDGMissionManager::MissionRequitalItem(CPlayer* pPlayer, MISSION_REQUITAL_ITEM* pMissionRequitalItem)
{
	if(pMissionRequitalItem == 0)
		return;

	WORD wItemIndex = pMissionRequitalItem->wItemIndex;
	DWORD dwItemNum = pMissionRequitalItem->dwNum;

	WORD EmptyCellPos[255];
	WORD EmptyCellNum;
	CItemSlot * pSlot = pPlayer->GetSlot(eItemTable_Inventory);
	WORD obtainItemNum = ITEMMGR->GetCanBuyNumInSpace( pPlayer, pSlot, wItemIndex, dwItemNum, EmptyCellPos, EmptyCellNum );
	if(obtainItemNum == 0)
		return;

	int rt = ITEMMGR->ObtainItemEx(pPlayer, ITEMMGR->Alloc(pPlayer, MP_ITEM, MP_ITEM_MONSTER_OBTAIN_NOTIFY, pPlayer->GetID(), 0, eLog_ItemObtainFromInsDungeon, obtainItemNum, (DBResult)(ITEMMGR->ObtainItemDBResult), NULL), wItemIndex, obtainItemNum, EmptyCellPos, EmptyCellNum, EmptyCellNum, 0);

};