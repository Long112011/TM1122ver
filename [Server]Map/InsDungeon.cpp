#include "stdafx.h"
#include "InsDungeon.h"
#include "InsDungeonManager.h"
#include "PartyManager.h"
#include "Party.h"
#include "UserTable.h"
#include "RecallManager.h"
#include "ItemManager.h"
#include "MapDBMsgParser.h"
#include "Network.h"

CInsDungeon::CInsDungeon()
{
	m_pInsDungeonRegenManager = NULL;
	m_pInsDGMissionManager = NULL;
	m_dwChannel = 0;
	m_dwPartyIDX = 0;
	m_dwMunpaIDX = 0;
	m_dwInsDGStartTime = 0;
	m_dwUserCount = 0;	
	m_dwProgressTime = 0;
	m_dwReturnToMapWaitTime = 0;
	m_dwRequitalTime = 0;	
	m_bInsDGStartFlag = FALSE;
	m_bDestroyFlag = FALSE;
	m_bClearRequital = FALSE;
	m_dwTotalKillMonsterNum = 0;
	memset(&m_RankState, 0, sizeof(m_RankState));
	m_InsDungeonPlayerTable.Initialize(10);
	m_bPartyBreakup = FALSE;
}

CInsDungeon::~CInsDungeon()
{
	Destory();
}

void CInsDungeon::Destory()
{	
	RECALLMGR->DeleteInsDungeonRecallMonster(m_dwChannel);
	SAFE_DELETE(m_pInsDungeonRegenManager);
	SAFE_DELETE(m_pInsDGMissionManager);
	RemoveAllPlayerTable();
}

void CInsDungeon::Init()
{
	m_pInsDungeonRegenManager = new CInsDungeonRegenManager;
	m_pInsDungeonRegenManager->SetChannel(m_dwChannel);
	if( m_pInsDungeonRegenManager->LoadInsDungeonRegenData() == FALSE) //ä�� ���ú��� �ؾ��Ѵ�.
		ASSERT(0);

	//�̼� �Ŵ����� �ݵ�� ���� �Ŵ��� ���� �� ��������
	m_pInsDGMissionManager = new CInsDGMissionManager(this, m_pInsDungeonRegenManager);
	if( m_pInsDGMissionManager->LoadInsDGMissionInfoList() == FALSE)
		ASSERT(0);
}

void CInsDungeon::InitInsDGStartTime()
{
	m_dwInsDGStartTime = 0;
	m_dwProgressTime = 0;
	m_bInsDGStartFlag = FALSE;
}

void CInsDungeon::StartInsDGStartTime()
{
	m_dwInsDGStartTime = gCurTime;
	m_dwProgressTime = (m_dwInsDGStartTime + INSDUNGEONMGR->GetDungeonInfo()->dwRestrictiveTime) - gCurTime;
	m_bInsDGStartFlag = TRUE;
}

void CInsDungeon::Process()
{
	if(m_bInsDGStartFlag == FALSE)
		return;

	DWORD dwRestrictiveTime = INSDUNGEONMGR->GetDungeonInfo()->dwRestrictiveTime;

	//���� ���ѽð� ������..
	DWORD dwTime = m_dwInsDGStartTime + dwRestrictiveTime + m_dwRequitalTime;
	int nProcessTime = (int)((int)dwTime - (int)gCurTime);

	if(nProcessTime > (int)(m_dwInsDGStartTime + dwRestrictiveTime))
		nProcessTime = (int)(m_dwInsDGStartTime + dwRestrictiveTime);

	//�δ����� ��Ƽ�� ��ü �Ǹ� 5���Ŀ� ��������
	if(m_bPartyBreakup == TRUE)
	{
		PartyBreakupReturnToMap();		
		m_bDestroyFlag = TRUE;		
	}
	else
	{
		if( nProcessTime < 0)
		{
			ReturnToMap();
			//InitInsDGStartTime();
			m_bDestroyFlag = TRUE;
		}
		else
		{
			//�����ð�
			m_dwProgressTime = (m_dwInsDGStartTime + dwRestrictiveTime) - gCurTime;
		}

		m_pInsDungeonRegenManager->Process();
		m_pInsDGMissionManager->Process();

		//������ ������ �˻� �� �̼��� ��� �Ϸ��ϸ� Ŭ���� ����
		if(m_bClearRequital == FALSE)
		{
			if(INSDUNGEONMGR->IsFinalMap() == TRUE)
			{
				if(m_pInsDGMissionManager->IsSuccess() == TRUE)
				{
					this->InsDGClearRank();				
					m_bClearRequital = TRUE;
					m_dwReturnToMapWaitTime = gCurTime;
					SendInsDGClearNotifyMapServer();
				}
			}
		}
		else	//������ ������ 10���� �ʿ��� ������ �Ѵ�
		{
			if(m_dwReturnToMapWaitTime == 0)
				return;

			if(m_dwReturnToMapWaitTime + eRETURN_TO_MAP_WAIT_TIME < gCurTime)
			{
				ReturnToMap();
				//InitInsDGStartTime();
				m_bDestroyFlag = TRUE;
			}
		}
	}
}

void CInsDungeon::ReturnToMap()
{
	switch(INSDUNGEONMGR->GetDungeonInfo()->eType)
	{
	case eINSDG_TYPE_PARTY:	//��Ƽ
		PartyReturnToMap();
		break;
	case eINSDG_TYPE_MUNPA:	//����
		
		break;
	}	
}

void CInsDungeon::PartyReturnToMap()
{
	CParty* pParty = PARTYMGR->GetParty(m_dwPartyIDX);
	if(pParty == NULL)
		return;

	PARTYMEMBER* pPartyMember = pParty->GetPartyMember();
	if(pPartyMember == NULL)
		return;



	CPlayer* pPlayer = NULL;
	DWORD dwPlayerID = 0;

	MSG_DWORD msg;
	SetProtocol( &msg, MP_INSDUNGEON, MP_INSDUNGEON_RETURNTOMAP );
	msg.dwData = INSDUNGEONMGR->GetDungeonInfo()->EscapeMapNum;

	for(int i = 0 ; i < MAX_PARTY_LISTNUM ; i++)
	{
		dwPlayerID = pParty->GetMemberID(i);
		if(dwPlayerID == 0)
			continue;

		pPlayer = (CPlayer*)g_pUserTable->FindUser( dwPlayerID );
		if( pPlayer == NULL)
			continue;

		if( pPlayer->GetObjectKind() != eObjectKind_Player )
			continue;	
		
		pPlayer->SendMsg( &msg, sizeof(msg) );

		g_pServerSystem->RemovePlayer(pPlayer->GetID());
	}
}

void CInsDungeon::PartyBreakupReturnToMap()
{
	DWORD* pPlayerID = NULL;
	CPlayer* pPlayer = NULL;

	MSG_DWORD msg;
	SetProtocol( &msg, MP_INSDUNGEON, MP_INSDUNGEON_RETURNTOMAP );
	msg.dwData = INSDUNGEONMGR->GetDungeonInfo()->EscapeMapNum;

	m_InsDungeonPlayerTable.SetPositionHead();
	while (pPlayerID = m_InsDungeonPlayerTable.GetData())
	{
		DWORD dwPlayerID = (*pPlayerID);
		pPlayer = (CPlayer*)g_pUserTable->FindUser(dwPlayerID);
		if( pPlayer == NULL)
			continue;
		msg.dwObjectID = pPlayer->GetID();
		pPlayer->SendMsg( &msg, sizeof(msg) );		
		g_pServerSystem->RemovePlayer(dwPlayerID);
	}	
}

void CInsDungeon::RemoveObject(CObject* pObject)
{
	if( pObject->GetGridID() != m_dwChannel )
		return;
	
	m_pInsDungeonRegenManager->RemoveObject(pObject);	
}

void CInsDungeon::AddMissionEvent(MISSION_EVENT_INFO* pMissionEvent)
{
	m_pInsDGMissionManager->AddMissionEvent(pMissionEvent);
}


void CInsDungeon::ClearRequital(MSG_INSDG_RANK_INFO* pRankRequitalInfo)
{
	/*
	//��Ƽ���鿡�� �޼��� ���
	DWORD dwPartyIDX = m_dwPartyIDX;
	CParty* pParty = PARTYMGR->GetParty(dwPartyIDX);
	if(pParty == NULL)
		return;

	//�ο��� �Ǻ�
	CPlayer* pPlayer = NULL;
	DWORD dwPlayerID;
	WORD wPlayerCount = 0;
	for(int n=0;n<MAX_PARTY_LISTNUM;++n)
	{
		dwPlayerID = pParty->GetMemberID(n);
		if(dwPlayerID == 0)
			continue;

		// network send
		pPlayer = (CPlayer*)g_pUserTable->FindUser( dwPlayerID );
		if( pPlayer == NULL )
			continue;

		wPlayerCount++;
	}

	DWORD dwInsDGIndex = INSDUNGEONMGR->GetDungeonInfo()->dwInsDGIndex;
	INSDUNGEON_REQUITAL* pRequitalInfo = INSDUNGEONMGR->GetInsDGRequitalInfo(dwInsDGIndex, wPlayerCount);
	if(pRequitalInfo == NULL)
		return;

	float fRequitalRatio = INSDUNGEONMGR->RankRequitalRatio(dwRankValue);
	
	DWORD dwRequitalExp = (DWORD)( (float)pRequitalInfo->dwExp * fRequitalRatio );
	DWORD dwRequitalAbillity = (DWORD)( (float)pRequitalInfo->dwAbilityExp * fRequitalRatio );
	DWORD dwRequitalMoney = (DWORD)( (float)pRequitalInfo->dwMoney * fRequitalRatio );

	WORD wRequitalItemIndex = 0;
	if(fRequitalRatio != 0.0f)	//F��ũ�� ���� ����
		wRequitalItemIndex = pRequitalInfo->wItemIDX;	

	//���� �÷��̾�� ������ �ش�	
	for(int n=0;n<MAX_PARTY_LISTNUM;++n)
	{
		dwPlayerID = pParty->GetMemberID(n);
		if(dwPlayerID == 0)
			continue;
		
		pPlayer = (CPlayer*)g_pUserTable->FindUser( dwPlayerID );
		if( pPlayer == NULL )
			continue;

		if(pRequitalInfo->dwExp != 0)
			pPlayer->AddPlayerExpPoint(dwRequitalExp);
		if(pRequitalInfo->dwAbilityExp != 0)
			pPlayer->AddAbilityExp(dwRequitalAbillity);
		if(pRequitalInfo->dwMoney != 0)
			pPlayer->SetMoney( dwRequitalMoney, MONEY_ADDITION, MF_OBTAIN, eItemTable_Inventory, eMoneyLog_InstanceDungeonRequital, pPlayer->GetID() );
		if(wRequitalItemIndex != 0)
			this->ClearRequitalItem(pPlayer, wRequitalItemIndex);
	}
	*/

	DWORD dwPartyIDX = m_dwPartyIDX;
	CParty* pParty = PARTYMGR->GetParty(dwPartyIDX);
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

		pPlayer = (CPlayer*)g_pUserTable->FindUser( dwPlayerID );
		if( pPlayer == NULL )
			continue;

		WORD revisionLevel = INSDUNGEONMGR->GetDungeonInfo()->wRevisionLevel;
		DWORD resultExp = pRankRequitalInfo->dwExp;
		DWORD resultAbil = pRankRequitalInfo->dwAbillity;

		// ���� ���� ����ġ ���� 0�� �ƴϸ� ������ �Ѵ�.
		if( revisionLevel )
		{
			resultExp = resultExp - ( resultExp * ( revisionLevel - pPlayer->GetLevel() ) / 10 );
			resultAbil = resultAbil - ( resultAbil * ( revisionLevel - pPlayer->GetLevel() ) / 10 );
		}

		if(resultExp != 0)
			pPlayer->AddPlayerExpPoint(resultExp);
		if(resultAbil != 0)
			pPlayer->AddAbilityExp(resultAbil);
		if(pRankRequitalInfo->dwMoney != 0)
			pPlayer->SetMoney( pRankRequitalInfo->dwMoney, MONEY_ADDITION, MF_OBTAIN, eItemTable_Inventory, eMoneyLog_InstanceDungeonRequital, pPlayer->GetID() );
		if(pRankRequitalInfo->ResultalItemInfo.wItemIndex != 0)
			this->ClearRequitalItem(pPlayer, pRankRequitalInfo->ResultalItemInfo.wItemIndex);
	}
}

void CInsDungeon::ClearRequitalItem(CPlayer* pPlayer, WORD wItemIndex)
{
	WORD EmptyCellPos[255];
	WORD EmptyCellNum;
	CItemSlot * pSlot = pPlayer->GetSlot(eItemTable_Inventory);
	WORD obtainItemNum = ITEMMGR->GetCanBuyNumInSpace( pPlayer, pSlot, wItemIndex, 1, EmptyCellPos, EmptyCellNum );
	if(obtainItemNum == 0)
		return;

	int rt = ITEMMGR->ObtainItemEx(pPlayer, ITEMMGR->Alloc(pPlayer, MP_ITEM, MP_ITEM_MONSTER_OBTAIN_NOTIFY, pPlayer->GetID(), 0, eLog_ItemObtainFromInsDungeon, obtainItemNum, (DBResult)(ITEMMGR->ObtainItemDBResult), NULL), wItemIndex, obtainItemNum, EmptyCellPos, EmptyCellNum, EmptyCellNum, 0);

};

BOOL CInsDungeon::IsMissionSuccess()
{
	return m_pInsDGMissionManager->IsSuccess();
}

void CInsDungeon::InsDungeonDestoryForCheckPlayer(CPlayer* pPlayer)
{
	CParty* pParty = PARTYMGR->GetParty(m_dwPartyIDX);
	if(pParty == NULL)
		return;

	DWORD dwPlayerID = 0;
	DWORD dwCount = 0;

	//��Ƽ �ο��� ������ �ִ��� üũ
	for(int i = 0 ; i < MAX_PARTY_LISTNUM ; i++)
	{
		dwPlayerID = pParty->GetMemberID(i);
		if(dwPlayerID == 0)
			continue;

		pPlayer = (CPlayer*)g_pUserTable->FindUser( dwPlayerID );
		if( pPlayer == NULL )
			continue;

		dwCount++;
	}

	//�ο��� ������ �δ� ����
	if(dwCount == 0)
		m_bDestroyFlag = TRUE;
}

void CInsDungeon::SetRankState(INSDG_RANK_STATE* pRankState)
{
	m_RankState.dwRankTime = pRankState->dwRankTime;
	m_RankState.dwMonsterKillNum = pRankState->dwMonsterKillNum;
}

INSDG_RANK_STATE* CInsDungeon::GetRankState()
{
	return &m_RankState;
}

void CInsDungeon::InsDGClearRank()
{
	//��Ƽ���鿡�� �޼��� ���
	DWORD dwPartyIDX = m_dwPartyIDX;
	CParty* pParty = PARTYMGR->GetParty(dwPartyIDX);
	if(pParty == NULL)
		return;

	//���� ��� �� ����
	CPlayer* pPlayer = NULL;
	DWORD dwPlayerID;
	DWORD dwPlayerCount = 0;
	DWORD dwTotalMonsterKillNum = 0;
	BOOL bClear = TRUE;	//�δ� ��ũ üũ (�� �� ������)
	DWORD dwClearLimitLevel = 0;	//�δ� Ŭ���� �ִ� ���� üũ
	INSDUNGEON_INFO* pDungeonInfo = INSDUNGEONMGR->GetDungeonInfo();
	if(pDungeonInfo != NULL)
		dwClearLimitLevel = pDungeonInfo->dwClearLimitLevel;
	
	for(int n=0;n<MAX_PARTY_LISTNUM;++n)
	{
		dwPlayerID = pParty->GetMemberID(n);
		if(dwPlayerID == 0)
			continue;

		// network send
		pPlayer = (CPlayer*)g_pUserTable->FindUser( dwPlayerID );
		if( pPlayer == NULL )
			continue;

		dwPlayerCount++;
		dwTotalMonsterKillNum += pPlayer->GetRankState()->dwMonsterKillNum;

		// �δ� Ŭ���� ��ũ���� ���� ���� üũ
		if(dwClearLimitLevel < pPlayer->GetLevel())
			bClear = FALSE;
	}
	
	m_RankState.dwMonsterKillNum = dwTotalMonsterKillNum;

	DWORD dwInsDGIndex = INSDUNGEONMGR->GetDungeonInfo()->dwInsDGIndex;

	DWORD dwClearTime = gCurTime - this->GetStartTime();

	//Ŭ����� ��ũ ���� ����
	INSDG_RANK_INFO RankInfo;
	memset(&RankInfo, 0, sizeof(RankInfo));	
	RankInfo.dwInsDGIndex = dwInsDGIndex;
	RankInfo.dwClearTime = m_RankState.dwRankTime + dwClearTime;
	RankInfo.dwMonsterKillNum = m_RankState.dwMonsterKillNum;
	if(bClear == TRUE)
		RankInfo.dwRankValue = INSDUNGEONMGR->ResultRank(dwInsDGIndex, m_RankState.dwRankTime, m_RankState.dwMonsterKillNum, dwPlayerCount);
	else
		RankInfo.dwRankValue = 0;

	//�� �÷��̾�� ��ũ ����� ������.	
	//�÷��̾�� ���� ��ũ ���� ����
	MSG_INSDG_RANK_INFO msg;
	memset(&msg, 0, sizeof(msg));
	SetProtocol( &msg, MP_INSDUNGEON, MP_INSDUNGEON_CLEAR_RANK_INFO );
	msg.dwPartyNum = dwPlayerCount;
	msg.dwClearTime = RankInfo.dwClearTime;
	msg.dwMonsterKillNum = RankInfo.dwMonsterKillNum;
	msg.dwRankValue = RankInfo.dwRankValue;
	
	INSDUNGEON_REQUITAL* pRequitalInfo = INSDUNGEONMGR->GetInsDGRequitalInfo(dwInsDGIndex, dwPlayerCount);
	if( (pRequitalInfo != NULL) && (RankInfo.dwRankValue != 0) )
	{
		float fRequitalRatio = INSDUNGEONMGR->RankRequitalRatio(RankInfo.dwRankValue);

		msg.dwExp = (DWORD)( (float)pRequitalInfo->dwExp * fRequitalRatio );
		msg.dwAbillity = (DWORD)( (float)pRequitalInfo->dwAbilityExp * fRequitalRatio );
		msg.dwMoney = (DWORD)( (float)pRequitalInfo->dwMoney * fRequitalRatio );

		if(fRequitalRatio != 0)
		{
			msg.ResultalItemInfo.wItemIndex = pRequitalInfo->wItemIDX;
			msg.ResultalItemInfo.dwItemNum = 1;		//������ ������ 1���� �ش�. ���� ������Ʈ
		}
	}

	//��ũ ������ �� ��Ƽ������ �ش�
	this->ClearRequital(&msg);

	for(int n=0;n<MAX_PARTY_LISTNUM;++n)
	{
		dwPlayerID = pParty->GetMemberID(n);
		if(dwPlayerID == 0)
			continue;

		pPlayer = (CPlayer*)g_pUserTable->FindUser( dwPlayerID );
		if( pPlayer == NULL )
			continue;

		pPlayer->SendMsg( &msg, sizeof(msg) );

		//�ֱ� ��ϵ� ��ũ��Ϻ��� ������ ����
        INSDG_RANK_INFO* pPlayerRankInfo = pPlayer->GetInsDGRankInfo(RankInfo.dwInsDGIndex);
		if(pPlayerRankInfo == NULL)	//��ũ����� ������ �־��ش�
		{
			pPlayer->AddInsDGRankInfo(&RankInfo);
			UpdateInstanceDungeonRank(dwPlayerID, &RankInfo);	//DB
		}
		else	//��ũ����� �ְ� ���� ��Ϻ��� ������ ����
		{
			if(RankInfo.dwRankValue > pPlayerRankInfo->dwRankValue)
			{
				pPlayer->AddInsDGRankInfo(&RankInfo);
				UpdateInstanceDungeonRank(dwPlayerID, &RankInfo);	//DB
			}			
		}
	}	
}

void CInsDungeon::SendInsDGClearNotifyMapServer()
{
	INSDG_KIND_INFO* pInsDGKindInfo = NULL;
	INSDUNGEON_INFO* pInsDGInfo = NULL;
	DWORD dwIndex = 0;

	CYHHashTable<INSDG_KIND_INFO>* pInsDungeonKindMapTable = INSDUNGEONMGR->GetInsDungeonKindMapTable();
	if(pInsDungeonKindMapTable == NULL)
		return;

	pInsDGInfo = INSDUNGEONMGR->GetDungeonInfo();
	if(pInsDGInfo == NULL)
		return;

	MSG_INSDG_KIND_INFO msg;
	msg.Category = MP_INSDUNGEON;
	msg.Protocol = MP_INSDUNGEON_CLEAR_NOTIFY_TO_MAPSERVER;	
	msg.dwPartyIndex = m_dwPartyIDX;

	pInsDungeonKindMapTable->SetPositionHead();
	while(pInsDGKindInfo = pInsDungeonKindMapTable->GetData())
	{
		if(pInsDGKindInfo->dwMapKind == pInsDGInfo->dwInsDGIndex)
		{
			msg.dwMapNum[dwIndex] = pInsDGKindInfo->wMapNum;
			dwIndex++;
		}
	}
	msg.nCount = dwIndex;

	g_Network.Send2AgentServer((char*)&msg, msg.GetMsgLength());
}

void CInsDungeon::InsDGFinalClearNotice()
{
	m_bClearRequital = TRUE;
	m_dwReturnToMapWaitTime = gCurTime;	

	CPlayer* pPlayer = NULL;
	DWORD dwPlayerID;
	
	DWORD dwPartyIDX = m_dwPartyIDX;
	CParty* pParty = PARTYMGR->GetParty(dwPartyIDX);
	if(pParty == NULL)
		return;

	MSGBASE msg;
	memset(&msg, 0, sizeof(msg));
	SetProtocol( &msg, MP_INSDUNGEON, MP_INSDUNGEON_CLEAR_MESSAGE );

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
		pPlayer->SendMsg( &msg, sizeof(msg) );
	}
}

void CInsDungeon::AddPlayer(CPlayer* pPlayer)
{
	DWORD dwPlayerID = pPlayer->GetID();

	DWORD* pPlayerID = new DWORD;
	*pPlayerID = dwPlayerID;
	m_InsDungeonPlayerTable.Add(pPlayerID, dwPlayerID);
}

void CInsDungeon::RemovePlayer(CPlayer* pPlayer)
{
	DWORD* pPlayerID = m_InsDungeonPlayerTable.GetData(pPlayer->GetID());
	if(pPlayerID != NULL)
	{
		m_InsDungeonPlayerTable.Remove(*pPlayerID);
		SAFE_DELETE(pPlayerID);
	}
}

void CInsDungeon::RemoveAllPlayerTable()
{
	DWORD* pPlayerID = NULL;

	m_InsDungeonPlayerTable.SetPositionHead();
	while (pPlayerID = m_InsDungeonPlayerTable.GetData())
	{
		SAFE_DELETE(pPlayerID);
	}
	m_InsDungeonPlayerTable.RemoveAll();
}

void CInsDungeon::PartyBreakup()
{	
	m_bPartyBreakup = TRUE;
}

void CInsDungeon::PartyBan(CPlayer* pPlayer)
{
	DWORD dwPlayerID = pPlayer->GetID();

	pPlayer = (CPlayer*)g_pUserTable->FindUser(dwPlayerID);
	if( pPlayer == NULL)
	{
		//ASSERT(0);
		return;
	}	
	MSG_DWORD msg;
	SetProtocol( &msg, MP_INSDUNGEON, MP_INSDUNGEON_RETURNTOMAP );
	msg.dwData = INSDUNGEONMGR->GetDungeonInfo()->EscapeMapNum;
	msg.dwObjectID = dwPlayerID;
	pPlayer->SendMsg( &msg, sizeof(msg) );
	g_pServerSystem->RemovePlayer(dwPlayerID);
	this->RemovePlayer(pPlayer);
}
