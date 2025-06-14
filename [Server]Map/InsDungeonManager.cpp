#include "stdafx.h"
#include "UserTable.h"
#include "InsDungeonManager.h"
#include "MHFile.h"
#include "Player.h"
#include "QuestManager.h"
#include "..\[CC]Header\GameResourceManager.h"
#include "MapDBMsgParser.h"
#include "LootingManager.h"
#include "InsDungeon.h"
#include "ItemManager.h"
#include "Network.h"
#include "InsDungeonMapManager.h"
#include "InsDungeonRegenManager.h"
#include "InsDungeonMissionDataManager.h"
#include "Quest.h"
#include "ChannelSystem.h"

GLOBALTON(CInsDungeonManager);

CInsDungeonManager::CInsDungeonManager()
{
	m_EntranceConditionTable.Initialize(100);
	m_InsDungeonTable.Initialize(100);	
	m_InsDungeonRequitalTable.Initialize(100);
	m_InsDungeonRankStandardTable.Initialize(50);
	m_InsDungeonKindMapTable.Initialize(10);
}

CInsDungeonManager::~CInsDungeonManager()
{
	RemoveEntranceConditionInfo();
	RemoveAllInsDungeon();
	RemoveInsDGRequitalInfo();
	RemoveRankStandardInfo();
	RmoveInsDungeonKindMapTable();
}

void CInsDungeonManager::Init()
{
	LoadEntranceConditionInfo();
	LoadInsDungeonInfo();

	DWORD dwMissionGroupIDX = m_InsDungeonInfo.dwMissionGroupIDX;
	INSDUNGEONMISSIONDATAMGR->LoadInsDungeonMissionList(dwMissionGroupIDX); //LoadInsDungeonInfo 다음에 넣어라

	LoadInsDGRequitalInfo();
	INSDUNGEONMISSIONDATAMGR->LoadMissionRegen();
	LoadRankStandardInfo();
}

void CInsDungeonManager::NetworkMsgParse( DWORD dwConnectionIndex, BYTE Protocol, void* pMsg )
{
	switch(Protocol)
	{
	case MP_INSDUNGEON_ENTRANCE_SYN:
		{
			MSG_DWORD_WORD3* pmsg = (MSG_DWORD_WORD3*)pMsg;

			InsDGEntrance(pmsg);
		}
		break;
	case MP_INSDUNGEON_ENTER_SYN:
		{
			MSG_INSDG_ENTER* pmsg = (MSG_INSDG_ENTER*)pMsg;

			InsDGEnter(pmsg, dwConnectionIndex);
		}
		break;
	case MP_INSDUNGEON_LEAVE_SYN:
		{
			MSG_DWORD4* pmsg = (MSG_DWORD4*)pMsg;

			InsDGLeave(pmsg, dwConnectionIndex);
		}
		break;	
	case MP_INSDUNGEON_NPC_MISSIONEVENT_SYN:
		{
			MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;

			CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser( pmsg->dwObjectID );
			if( pPlayer == NULL )
				return;

			//2008. 8. 18. CBH - 인던일경우 미션 이벤트 발생
			if (g_pServerSystem->GetMap()->IsMapKind(eInsDungeon/*eInstanceDungeonMap*/))
			{
				MISSION_EVENT_INFO MissionEvent;
				memset(&MissionEvent, 0, sizeof(MissionEvent));

				MissionEvent.dwExecuteData = pmsg->dwData;
				MissionEvent.dwMissionEventKind = eMISSION_EVENT_KIND_NPC;
				MissionEvent.pPlayer = pPlayer;

				AddMissionEvent(&MissionEvent);
			}
		}
		break;
	case MP_INSDUNGEON_CLEAR_NOTIFY_TO_MAPSERVER:
		{
			MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;

			CInsDungeon* pTempInsDungeon = NULL;
			CInsDungeon* pInsDungeon = NULL;

			m_InsDungeonTable.SetPositionHead();
			while (pTempInsDungeon = m_InsDungeonTable.GetData())
			{
				if( pTempInsDungeon->GetPartyIDX() == pmsg->dwData )
				{
					pInsDungeon = pTempInsDungeon;
					break;
				}
			}
			
			if(pInsDungeon != NULL)
				pInsDungeon->InsDGFinalClearNotice();
		}
		break;
	}
}

BOOL CInsDungeonManager::LoadEntranceConditionInfo()
{
	CMHFile file;
	char filename[256];
#ifdef _FILE_BIN_
	sprintf(filename,"Resource/InsDungeonScript/InsDG_EntranceCondition.bin");
	if(!file.Init(filename,"rb"))
		return FALSE;
#else
	sprintf(filename,"Resource/InsDungeonScript/InsDG_EntranceCondition.txt");
	if(!file.Init(filename,"rt"))
		return FALSE;
#endif

	while(1)
	{
		if(file.IsEOF())
			break;

		WORD wMapNum = file.GetWord();
		DWORD dwNPCUniqueIDX = file.GetDword();
		WORD wMinLevel = file.GetWord();
		WORD wMaxLevel = file.GetWord();
		DWORD dwQuestIndex = file.GetDword();		
		WORD wItemIndex = file.GetWord();
		DWORD dwMoney = file.GetDword();
		DWORD dwMsgIndex = file.GetDword();

		INSDG_ENTRANCECONDITION_INFO* pEntranceInfo = new INSDG_ENTRANCECONDITION_INFO;
		pEntranceInfo->wMapNum = wMapNum;
		pEntranceInfo->dwNpcUniqueIDX = dwNPCUniqueIDX;
		pEntranceInfo->wMinLevel = wMinLevel;
		pEntranceInfo->wMaxLevel = wMaxLevel;
		pEntranceInfo->dwQuestIndex = dwQuestIndex;		
		pEntranceInfo->wItemIndex = wItemIndex;
		pEntranceInfo->dwMoney = dwMoney;
		pEntranceInfo->dwMsgIndex = dwMsgIndex;

		m_EntranceConditionTable.Add( pEntranceInfo, pEntranceInfo->dwNpcUniqueIDX );
	}
	file.Release();

	return TRUE;
}

BOOL CInsDungeonManager::LoadInsDGRequitalInfo()
{
	CMHFile file;
	char filename[256];
#ifdef _FILE_BIN_
	sprintf(filename,"Resource/InsDungeonScript/InsDGClearRequital.bin");
	if(!file.Init(filename,"rb"))
		return FALSE;
#else
	sprintf(filename,"Resource/InsDungeonScript/InsDGClearRequital.txt");
	if(!file.Init(filename,"rt"))
		return FALSE;
#endif

	DWORD dwKey = 0;
	DWORD dwKey_1 = 0;
	DWORD dwKey_2 = 0;
	
	while(1)
	{
		if(file.IsEOF())
			break;

		DWORD dwInsDGIndex = file.GetDword();	//인던 인덱스
		WORD wPlayerCount = file.GetWord();	//인원
		DWORD dwExp = file.GetDword();		//보상 경험치
		DWORD dwAbilityExp = file.GetDword();	//보상 수련치
		MONEYTYPE dwMoney = file.GetDword();	//보상 돈
		WORD wItemIDX = file.GetWord();		//보상 아이탬

		INSDUNGEON_REQUITAL* pInsDGRequital = new INSDUNGEON_REQUITAL;
		pInsDGRequital->dwInsDGIndex = dwInsDGIndex;
		pInsDGRequital->wPlayerCount = wPlayerCount;
		pInsDGRequital->dwExp = dwExp;
		pInsDGRequital->dwAbilityExp = dwAbilityExp;
		pInsDGRequital->dwMoney = dwMoney;
		pInsDGRequital->wItemIDX = wItemIDX;

		DWORD dwKey = 0;
		DWORD dwKey_1 = dwInsDGIndex;
		DWORD dwKey_2 = wPlayerCount;
		
		COMBINEKEY(dwKey_1, dwKey_2, dwKey);
		m_InsDungeonRequitalTable.Add(pInsDGRequital, dwKey);
	}
	file.Release();

	return TRUE;
}

void CInsDungeonManager::RemoveInsDGRequitalInfo()
{
	INSDUNGEON_REQUITAL* pInsDGRequital = NULL;

	m_InsDungeonRequitalTable.SetPositionHead();
	while (pInsDGRequital = m_InsDungeonRequitalTable.GetData())
	{
		SAFE_DELETE(pInsDGRequital);
	}
	m_InsDungeonRequitalTable.RemoveAll();
}

INSDUNGEON_REQUITAL* CInsDungeonManager::GetInsDGRequitalInfo(DWORD dwInsDGIndex, DWORD dwCount)
{
	DWORD dwKey = 0;
	DWORD dwKey_1 = dwInsDGIndex;
	DWORD dwKey_2 = dwCount;

	COMBINEKEY(dwKey_1, dwKey_2, dwKey);

	return m_InsDungeonRequitalTable.GetData(dwKey);
}

void CInsDungeonManager::RemoveEntranceConditionInfo()
{
	INSDG_ENTRANCECONDITION_INFO* pEntranceInfo = NULL;

	m_EntranceConditionTable.SetPositionHead();
	while(pEntranceInfo = m_EntranceConditionTable.GetData())
	{
		SAFE_DELETE(pEntranceInfo);
	}
	m_EntranceConditionTable.RemoveAll();
}

BOOL CInsDungeonManager::LoadInsDungeonInfo()
{
	CMHFile file;
	char filename[256];
#ifdef _FILE_BIN_
	sprintf(filename,"Resource/InsDungeonScript/InsDungeonInfo.bin");
	if(!file.Init(filename,"rb"))
		return FALSE;
#else
	sprintf(filename,"Resource/InsDungeonScript/InsDungeonInfo.txt");
	if(!file.Init(filename,"rt"))
		return FALSE;
#endif

	char szInsDGName[MAX_INSDUNGEONNAME_LENGTH+1];
	MAPTYPE CurMapNum = g_pServerSystem->GetMapNum();
	
	while(1)
	{
		if(file.IsEOF())
			break;

		MAPTYPE MapNum = file.GetWord();
		DWORD dwInsDGIndex = file.GetDword();
		memset(szInsDGName, 0, sizeof(*szInsDGName));
		SafeStrCpy( szInsDGName, file.GetString(), MAX_INSDUNGEONNAME_LENGTH+1 );
		DWORD dwType = file.GetDword();
		MAPTYPE wEscapeMapNum = file.GetWord();
		DWORD dwEscapeMapChangeKind = file.GetDword();
		MAPTYPE wNextMapNum = file.GetWord();
		DWORD dwRestrictiveTime = file.GetDword();
		DWORD dwMissionGroupIDX = file.GetDword();
		BOOL bFinalMap = file.GetBool();
		DWORD dwClearLimitLevel = file.GetDword();
		WORD wRevisionLevel = file.GetWord();
		
		//2008. 10. 9. CBH - 인던정보를 로딩할때 현재 맵과 같은 종류의 인던 맵번호 따로 저장
		if(CurMapNum != MapNum)
		{
			INSDG_KIND_INFO* pMapKindInfo = new INSDG_KIND_INFO;
			pMapKindInfo->wMapNum = MapNum;
			pMapKindInfo->dwMapKind = dwInsDGIndex;
			m_InsDungeonKindMapTable.Add(pMapKindInfo, MapNum);

			continue;
		}

		m_InsDungeonInfo.eType = (eINSDG_TYPE)dwType;
		m_InsDungeonInfo.dwInsDGIndex = dwInsDGIndex;
		SafeStrCpy( m_InsDungeonInfo.szInsDGName, szInsDGName, MAX_INSDUNGEONNAME_LENGTH+1 );		
		m_InsDungeonInfo.EscapeMapNum = wEscapeMapNum;
		m_InsDungeonInfo.dwEscapeMapChangeKind = dwEscapeMapChangeKind;
		m_InsDungeonInfo.NextMapNum = wNextMapNum;
		m_InsDungeonInfo.dwRestrictiveTime = dwRestrictiveTime;		
		m_InsDungeonInfo.dwMissionGroupIDX = dwMissionGroupIDX;
		m_InsDungeonInfo.bFinalMap = bFinalMap;
		m_InsDungeonInfo.dwClearLimitLevel = dwClearLimitLevel;
		m_InsDungeonInfo.wRevisionLevel = wRevisionLevel;
	}
	file.Release();

	return TRUE;
}

eINSDG_ENTRANCE_ERROR CInsDungeonManager::CheckEntranceCondition(CPlayer* pPlayer, DWORD dwNPCUniqueIDX)
{
	//파티 및 문파 체크 체크
	DWORD dwIndex = 0;
	switch(m_InsDungeonInfo.eType)
	{
	case eINSDG_TYPE_PARTY:	//파티
		{
			dwIndex = pPlayer->GetPartyIdx();
			if(dwIndex == 0)
				return eINSDG_ENTRANCE_ERROR_PARTY;
		}		
		break;
	case eINSDG_TYPE_MUNPA:	//문파
		{
			dwIndex = pPlayer->GetMunpaIdx();
			if(dwIndex == 0)
				return eINSDG_ENTRANCE_ERROR_MUNPA;
		}		
		break;
	}	

	LEVELTYPE wPlayerLevel = pPlayer->GetLevel();

	INSDG_ENTRANCECONDITION_INFO* pEntranceInfo = m_EntranceConditionTable.GetData(dwNPCUniqueIDX);
	if(pEntranceInfo == NULL)
		return eINSDG_ENTRANCE_ERROR_NULL;

	if( pPlayer->IsPKMode() || LOOTINGMGR->IsLootedPlayer( pPlayer->GetID() ) ||
		eObjectState_Die == pPlayer->GetState() || pPlayer->GetAutoAskPlayerIdx() )	
		return eINSDG_ENTRANCE_ERROR_FAIL;

	//최소 레벨 체크
	if(wPlayerLevel < pEntranceInfo->wMinLevel)
		return eINSDG_ENTRANCE_ERROR_LEVEL;

	//최대 레벨 체크
	if(wPlayerLevel > pEntranceInfo->wMaxLevel)
		return eINSDG_ENTRANCE_ERROR_LEVEL;

	//퀘스트 체크
	if(pEntranceInfo->dwQuestIndex != 0)
	{
		CQuestGroup* pQuestGroup = pPlayer->GetQuestGroup();
		if(pQuestGroup != NULL)
		{
			CQuest* pQuest = pQuestGroup->GetQuest(pEntranceInfo->dwQuestIndex);
			/*
			if(pQuest != NULL)
			{
				BOOL bSuccess = pQuest->IsQuestComplete();
				if(bSuccess == FALSE)
					return eINSDG_ENTRANCE_ERROR_QUEST;
			}
			else
				return eINSDG_ENTRANCE_ERROR_QUEST;
				*/
			if(pQuest == NULL)			
				return eINSDG_ENTRANCE_ERROR_QUEST;
		}
		
	}	

	//필요한 아이탬 체크
	if(pEntranceInfo->wItemIndex != 0)
	{
		ITEM_TOTALINFO ItemTotalInfo;	
		WORD wCheckItemIndex = 0;
		pPlayer->GetItemtotalInfo( &ItemTotalInfo, GETITEM_FLAG_INVENTORY );
		for(int i = 0; i < SLOT_INVENTORY_NUM;	++i )
		{
			if( (ItemTotalInfo.Inventory[i].wIconIdx == pEntranceInfo->wItemIndex) && (pEntranceInfo->wItemIndex != 0) )
			{
				wCheckItemIndex = ItemTotalInfo.Inventory[i].wIconIdx;
				break;
			}
		}
		if( (wCheckItemIndex == 0) && (pEntranceInfo->wItemIndex != 0) )
			return eINSDG_ENTRANCE_ERROR_ITEM;
	}	

	//요금 체크
	if(pPlayer->GetMoney() < pEntranceInfo->dwMoney)
		return eINSDG_ENTRANCE_ERROR_MONEY;	

	//현재 맵이 인던일때는 미션이 완료 되었나 체크한다.
	if (g_pServerSystem->GetMap()->IsMapKind(eInsDungeon/*eInstanceDungeonMap*/))
	{
		CInsDungeon* pInsDungeon = m_InsDungeonTable.GetData(pPlayer->GetChannelID());
		if(pInsDungeon == NULL)
			return eINSDG_ENTRANCE_ERROR_NULL;
		else
		{
			if(pInsDungeon->IsMissionSuccess() == FALSE)
				return eINSDG_ENTRANCE_ERROR_MISSION;

			//2008. 10. 22. CBH - 마지막 보스방이 클리어 되면 입장 못하게 한다
			if(pInsDungeon->IsClear() == TRUE)
				return eINSDG_ENTRANCE_ERROR_CLEAR;
		}		
	}	

	return eINSDG_ENTRANCE_ERROR_NONE;
}


BOOL CInsDungeonManager::CreateInsDungeon(DWORD dwChannel, CPlayer* pPlayer)
{
	DWORD dwIndex = 0;

	switch(m_InsDungeonInfo.eType)
	{
	case eINSDG_TYPE_PARTY:	//파티
		dwIndex = pPlayer->GetPartyIdx();
		break;
	case eINSDG_TYPE_MUNPA:	//문파		
		dwIndex = pPlayer->GetMunpaIdx();
		break;
	}

	if( (dwIndex == 0) || (dwChannel == 0) )
	{
		ASSERT(dwIndex != 0);
		ASSERT(dwChannel != 0);
		return FALSE;
	}

	CInsDungeon* pInsDungeon = new CInsDungeon;

	//인던의 채널 정보 셋팅
	pInsDungeon->SetChannel(dwChannel);

	pInsDungeon->Init();	//초기화, *중요 : 인던의 채널먼저 셋팅해야한다. 
	
	switch(m_InsDungeonInfo.eType)
	{
	case eINSDG_TYPE_PARTY:	//파티
		pInsDungeon->SetPartyIDX(dwIndex);
		break;
	case eINSDG_TYPE_MUNPA:	//문파		
		pInsDungeon->SetMunpaIDX(dwIndex);
		break;
	}	

    m_InsDungeonTable.Add(pInsDungeon, dwChannel);		

	pInsDungeon->StartInsDGStartTime();	

	INSDG_RANK_STATE* pRankState = pPlayer->GetRankState();
	if(pRankState == NULL)
		return FALSE;

	INSDG_RANK_STATE RankState;
	RankState.dwRankTime = pRankState->dwRankTime;
	RankState.dwMonsterKillNum = 0;
	pInsDungeon->SetRankState(&RankState);

	return TRUE;
}

void CInsDungeonManager::RemoveInsDungeon(DWORD dwChennel)
{
	CInsDungeon* pInsDungeon = NULL;

	m_InsDungeonTable.SetPositionHead();
	while(pInsDungeon = m_InsDungeonTable.GetData())
	{
		if( pInsDungeon->GetChannel() == dwChennel )
		{
			SAFE_DELETE(pInsDungeon);
			m_InsDungeonTable.Remove(dwChennel);
			return;
		}
	}
}

void CInsDungeonManager::RemoveAllInsDungeon()
{
	CInsDungeon* pInsDungeon = NULL;

	m_InsDungeonTable.SetPositionHead();
	while(pInsDungeon = m_InsDungeonTable.GetData())
	{
		SAFE_DELETE(pInsDungeon);		
	}
	m_InsDungeonTable.RemoveAll();
}

DWORD CInsDungeonManager::GetInsDungeonChennel(DWORD dwIndex)
{
	DWORD dwChannel = 0;
	CInsDungeon* pInsDungeon = NULL;

	m_InsDungeonTable.SetPositionHead();
	while(pInsDungeon = m_InsDungeonTable.GetData())
	{
		//InsDungeonType에 따라 검색할 인덱스가 틀리다
		//해당 파티나 문파의 인덱스로 생성된 인던 정보가있으면
		//채널을 반환한다.
		switch(m_InsDungeonInfo.eType)
		{
		case eINSDG_TYPE_PARTY:	//파티
			{
				if( dwIndex == pInsDungeon->GetPartyIDX() )
					return pInsDungeon->GetChannel();
			}			
			break;
		case eINSDG_TYPE_MUNPA:	//문파
			{
				if( dwIndex == pInsDungeon->GetMunpaIDX() )
					return pInsDungeon->GetChannel();
			}
			break;
		}
	}

	return dwChannel;
}

DWORD CInsDungeonManager::GetInsDungeonChennel(CPlayer* pPlayer)
{
	DWORD dwChannel = 0;
	CInsDungeon* pInsDungeon = NULL;

	m_InsDungeonTable.SetPositionHead();
	while(pInsDungeon = m_InsDungeonTable.GetData())
	{
		//InsDungeonType에 따라 검색할 인덱스가 틀리다
		//해당 파티나 문파의 인덱스로 생성된 인던 정보가있으면
		//채널을 반환한다.
		switch(m_InsDungeonInfo.eType)
		{
		case eINSDG_TYPE_PARTY:	//파티
			{
				if( pPlayer->GetPartyIdx() == pInsDungeon->GetPartyIDX() )
					return pInsDungeon->GetChannel();
			}			
			break;
		case eINSDG_TYPE_MUNPA:	//문파
			{
				if( pPlayer->GetMunpaIdx() == pInsDungeon->GetMunpaIDX() )
					return pInsDungeon->GetChannel();
			}
			break;
		}
	}

	return dwChannel;
}

void CInsDungeonManager::Process()
{
	m_InsDungeonTable.SetPositionHead();
	while(CInsDungeon* pInsDungeon = m_InsDungeonTable.GetData())
	{
		//등록된 인던들의 Process처리
		if(pInsDungeon->IsInsDGStartFlag() == TRUE)
		{
			pInsDungeon->Process();
			// 인던이 끝나면 삭제한다
			if(pInsDungeon->IsDestroyFlag() == TRUE)
			{				
				pInsDungeon->Destory();				
				CHANNELSYSTEM->DestroyInsDungeonMapChannel(pInsDungeon->GetChannel());
				m_InsDungeonTable.Remove(pInsDungeon->GetChannel());
				SAFE_DELETE(pInsDungeon);
			}
		}
	}
}

CInsDungeon* CInsDungeonManager::GetInsDungeon(DWORD dwChannel)
{
	CInsDungeon* pInsDungeon = NULL;

	m_InsDungeonTable.SetPositionHead();
	while (pInsDungeon = m_InsDungeonTable.GetData())
	{
		if( pInsDungeon->GetChannel() == dwChannel )
			return pInsDungeon;
	}

	return NULL;
}

void CInsDungeonManager::RemoveObject(CObject* pObject)
{
	DWORD dwGridID =  pObject->GetGridID();

	CInsDungeon* pInsDungeon = GetInsDungeon(dwGridID);
	if(pInsDungeon == NULL)
		return;

	pInsDungeon->RemoveObject(pObject);
}

void CInsDungeonManager::InsDGEntrance(MSG_DWORD_WORD3* pMsg)
{
	CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser( pMsg->dwObjectID );
	if( pPlayer == NULL )
		return;

	MSG_DWORD msg;
	msg.Category = MP_INSDUNGEON;
	msg.Protocol = MP_INSDUNGEON_ENTRANCE_NACK;
	msg.dwObjectID = pPlayer->GetID();

	//입장 조건 체크
	eINSDG_ENTRANCE_ERROR eEntraceError = CheckEntranceCondition(pPlayer, pMsg->dwData);
	if( eEntraceError != eINSDG_ENTRANCE_ERROR_NONE )	//입장 실패
	{				
		msg.dwData = eEntraceError;
		pPlayer->SendMsg( &msg, sizeof(msg) );
	}
	else	//입장 성공
	{
		MAPCHANGE_INFO* pChangeInfo = GAMERESRCMNGR->GetMapChangeInfo(pMsg->dwData);
		if( !pChangeInfo )
			eEntraceError = eINSDG_ENTRANCE_ERROR_NULL;

		if( eEntraceError != eINSDG_ENTRANCE_ERROR_NONE ) //맵이동 실패
		{					
			msg.dwData = eEntraceError;
			pPlayer->SendMsg( &msg, sizeof(msg) );
		}
		else	//맵이동 성공
		{
			//일반맵에서 인던으로 들어갈때 아이탬을 없앤다
			//인던끼리 옮길때는 없애면 안된다.
			if(INSDUNGEONMAPMGR->IsInsDungeonMap() == FALSE)
			{
				CMHMap* pMap = g_pServerSystem->GetMap();
				ASSERT(pMap != NULL)
					//현재 맵이 인던이 아니면 아이탬을 지운다. (인던 처음 입장시)
				if (pMap->IsMapKind(eInsDungeon/*eInstanceDungeonMap*/) == FALSE)
					{
						//입장 체크 성공하면 입장아이탬이 있을 경우 삭제
						if(pMsg->wData1 != 0)
						{
							if( EI_TRUE != ITEMMGR->DiscardItem( pPlayer, (POSTYPE)pMsg->wData2, (WORD)pMsg->wData1, 1 ) )
							{								
								msg.dwData = eINSDG_ENTRANCE_ERROR_ITEM;
								pPlayer->SendMsg( &msg, sizeof(msg) );
								return;
							}
						}							

						//입장요금이 있으면 요금 차감
						MAPTYPE wMapNum = g_pServerSystem->GetMapNum();
						INSDG_ENTRANCECONDITION_INFO* pEntranceInfo = m_EntranceConditionTable.GetData(pMsg->dwData);
						if(pEntranceInfo == NULL)
						{
							msg.dwData = eINSDG_ENTRANCE_ERROR_NULL;
							pPlayer->SendMsg( &msg, sizeof(msg) );
						}

						if(pEntranceInfo->dwMoney != 0)
							pPlayer->SetMoney( pEntranceInfo->dwMoney, MONEY_SUBTRACTION, 0, eItemTable_Inventory, eMoneyLog_InstanceDungeonEnter, wMapNum );							
					}
			}					

			MSG_INSDG_ENTRANCE Msg;
			Msg.Category = MP_INSDUNGEON;
			Msg.Protocol = MP_INSDUNGEON_ENTRANCE_ACK;
			Msg.dwObjectID = pPlayer->GetID();
			Msg.dwData = pChangeInfo->MoveMapNum;
			
			//인던의 다음맵에 랭크 기록 정보를 누적하여줘야 하기때문에 셋팅하여 보낸다.
			CInsDungeon* pCurInsDungeon = m_InsDungeonTable.GetData(pPlayer->GetChannelID());
			if(pCurInsDungeon == NULL)
			{
				Msg.RankState.dwRankTime = 0;
				Msg.RankState.dwMonsterKillNum = 0;
			}
			else
			{
				DWORD dwClearTime = gCurTime - pCurInsDungeon->GetStartTime();
				Msg.RankState.dwRankTime = dwClearTime + pCurInsDungeon->GetRankState()->dwRankTime ;
				Msg.RankState.dwMonsterKillNum = pPlayer->GetRankState()->dwMonsterKillNum;
			}

			pPlayer->SendMsg( &Msg, sizeof(Msg) );

			pPlayer->GetPetManager()->SetPetSummonning(ePSS_SaveSummon);
			//pPlayer->GetTitanManager()->SetTitanRiding(TRUE);			
			pPlayer->UpdateLogoutToDB();			
			g_pServerSystem->RemovePlayer(pPlayer->GetID());
			SaveMapChangePointUpdate( pPlayer->GetID(), pChangeInfo->Kind );
		}			
	}
}

void CInsDungeonManager::InsDGEnter(MSG_INSDG_ENTER* pMsg, DWORD dwConnectionIndex)
{
	CObject* pObject = g_pUserTable->FindUser(pMsg->dwObjectID);				
	if( pObject )
	{
		if( pObject->GetObjectKind() == eObjectKind_Player )
		{
			MSG_DWORD msg;
			SetProtocol( &msg, MP_USERCONN, MP_USERCONN_GAMEIN_NACK );
			msg.dwData = pMsg->dwObjectID;					
			g_Network.Send2Server( dwConnectionIndex, (char*)&msg, sizeof(msg) );
			return;
		}
	}

	CPlayer* pPlayer = g_pServerSystem->AddPlayer( pMsg->dwObjectID, dwConnectionIndex, pMsg->dwData1, 0 );
	if(!pPlayer) return;
	
    pPlayer->SetRankState(&pMsg->RankState);	
	pPlayer->SetInitPoint( pMsg->dwMapMovePoint );

	pPlayer->SetUserLevel( pMsg->dwData2 );
	pPlayer->SetReturnMapNum( (WORD)pMsg->dwData4 );			
	//pPlayer->SetSafeLock(FALSE);	
	//UpdateSafeStatic(pPlayer->GetID(), 0);
	CharacterNumSendAndCharacterInfo(pMsg->dwObjectID, MP_USERCONN_GAMEIN_SYN);
	CharacterMugongInfo(pMsg->dwObjectID, MP_USERCONN_GAMEIN_SYN);
	CharacterItemOptionInfo(pMsg->dwObjectID, MP_USERCONN_GAMEIN_SYN);
	CharacterItemStoneOptionInfo(pMsg->dwObjectID, MP_USERCONN_GAMEIN_SYN);
	CharacterItemRareOptionInfo(pMsg->dwObjectID, MP_USERCONN_GAMEIN_SYN);
	CharacterAbilityInfo(pMsg->dwObjectID, MP_USERCONN_GAMEIN_SYN);
	CharacterSkinInfo(pPlayer->GetID());

	QuestTotalInfo(pMsg->dwObjectID);		// 초보자 퀘스트

	// quest 정보 qurey 추가
	QUESTMGR->CreateQuestForPlayer( pPlayer );
	// 항상 서브퀘스트 정보부터 읽어야 함...			
	QuestSubQuestLoad(pMsg->dwObjectID);	
	
	//CharacterTitanInfo(pMsg->dwObjectID, pPlayer->GetUserID());

	//랭크정보 요청
	LoadInstanceDungeonRank(pMsg->dwObjectID);

	/*MSG_CHANNEL_INFO msg1;
	memset(&msg1, 0, sizeof(msg1));
	msg1.Category = MP_USERCONN;
	msg1.Protocol = MP_USERCONN_CURRENTMAP_CHANNELINFO;
	CHANNELSYSTEM->GetChannelInfo(&msg1);

	pPlayer->SendMsg(&msg1, sizeof(msg1));*/
}

void CInsDungeonManager::InsDGLeave(MSG_DWORD4* pMsg, DWORD dwConnectionIndex)
{
	CObject* pObject = g_pUserTable->FindUser(pMsg->dwObjectID);
	if( pObject != NULL )
	{
		if( pObject->GetObjectKind() == eObjectKind_Player )
		{
			MSG_DWORD msg;
			SetProtocol(&msg, MP_USERCONN, MP_USERCONN_GAMEIN_NACK);
			msg.dwData	= pMsg->dwObjectID;					
			g_Network.Send2Server( dwConnectionIndex, (char*)&msg, sizeof(msg) );
			return;
		}
	}

	CPlayer* pPlayer = g_pServerSystem->AddPlayer( pMsg->dwObjectID, dwConnectionIndex, pMsg->dwData1, pMsg->dwData3 );
	if(!pPlayer) return;


	pPlayer->SetUserLevel(pMsg->dwData2);
	pPlayer->SetReturnMapNum((WORD)pMsg->dwData4);
	//pPlayer->SetSafeLock(FALSE);
	//UpdateSafeStatic(pPlayer->GetID(), 0);
	CharacterNumSendAndCharacterInfo(pMsg->dwObjectID, MP_USERCONN_GAMEIN_SYN);
	CharacterMugongInfo(pMsg->dwObjectID, MP_USERCONN_GAMEIN_SYN);
	CharacterItemOptionInfo(pMsg->dwObjectID, MP_USERCONN_GAMEIN_SYN);
	CharacterItemStoneOptionInfo(pMsg->dwObjectID, MP_USERCONN_GAMEIN_SYN);
	CharacterItemRareOptionInfo(pMsg->dwObjectID, MP_USERCONN_GAMEIN_SYN);
	CharacterAbilityInfo(pMsg->dwObjectID, MP_USERCONN_GAMEIN_SYN);
	CharacterSkinInfo(pPlayer->GetID());

	QuestTotalInfo(pMsg->dwObjectID);		// 초보자 퀘스트

	// quest 정보 qurey 추가
	QUESTMGR->CreateQuestForPlayer( pPlayer );
	// 항상 서브퀘스트 정보부터 읽어야 함...			
	QuestSubQuestLoad(pMsg->dwObjectID);

	//랭크정보 요청
	LoadInstanceDungeonRank(pMsg->dwObjectID);

	/*MSG_CHANNEL_INFO msg1;
	memset(&msg1, 0, sizeof(msg1));
	msg1.Category = MP_USERCONN;
	msg1.Protocol = MP_USERCONN_CURRENTMAP_CHANNELINFO;
	CHANNELSYSTEM->GetChannelInfo(&msg1);

	pPlayer->SendMsg(&msg1, sizeof(msg1));*/
}

void CInsDungeonManager::SendInsDungeonInfo(CPlayer* pPlayer)
{
	//DWORD dwIndex = 0;
	CInsDungeon* pInsDungeon = NULL;
	CInsDungeon* pCurInsDungeon = NULL;
	
	/*
	m_InsDungeonTable.SetPositionHead();
	while(pInsDungeon = m_InsDungeonTable.GetData())
	{
		//InsDungeonType에 따라 검색할 인덱스가 틀리다
		//해당 파티나 문파의 인덱스로 생성된 인던 정보가있으면
		//채널을 반환한다.
		switch(m_InsDungeonInfo.eType)
		{
		case eINSDG_TYPE_PARTY:	//파티
			{
				DWORD dwPartyIndex = pPlayer->GetPartyIdx();
				if( dwPartyIndex == pInsDungeon->GetPartyIDX() )
				{
					dwIndex = dwPartyIndex;
					pCurInsDungeon = pInsDungeon;
					break;
				}
			}			
			break;
		case eINSDG_TYPE_MUNPA:	//문파
			{
				DWORD dwMunpaIndex = pPlayer->GetMunpaIdx();
				if( dwMunpaIndex == pInsDungeon->GetMunpaIDX() )
				{
					dwIndex = dwMunpaIndex;
					pCurInsDungeon = pInsDungeon;
					break;
				}
			}
			break;
		}
	}*/

	pCurInsDungeon = m_InsDungeonTable.GetData(pPlayer->GetChannelID());
	ASSERT(pCurInsDungeon != NULL);
	
	//인던 정보에 플레이어 ID 셋팅
	pCurInsDungeon->AddPlayer(pPlayer);

	//DWORD dwEndTime = pCurInsDungeon->GetStartTime() + m_InsDungeonInfo.dwRestrictiveTime;	//종료시간
	//DWORD dwRestTime = dwEndTime - gCurTime; //남은 시간 정보
	//DWORD dwProgressTime = m_InsDungeonInfo.dwRestrictiveTime - dwRestTime;	//진행된 시간


	//셋팅된 던전 기본 정보를 클라이언트로 보낸다
	MSG_INSDUNGEON_INFO msg;
	memset(&msg, 0, sizeof(msg));
	msg.Category = MP_INSDUNGEON;
	msg.Protocol = MP_INSDUNGEON_INFO_ACK;
	msg.dwObjectID = pPlayer->GetID();
	msg.dwChannel = pCurInsDungeon->GetChannel();
	msg.dwPartyIDX = pCurInsDungeon->GetPartyIDX();
	msg.dwMunpaIDX = pCurInsDungeon->GetMunpaIDX();	
	//msg.dwProgressTime = dwProgressTime;
	msg.dwProgressTime = pCurInsDungeon->GetProgressTime();
	msg.dwRequitalTime = pCurInsDungeon->GetRequitalTime();
	msg.bMissionSuccess = pCurInsDungeon->IsMissionSuccess();

	CInsDGMission* pCurMission = pCurInsDungeon->GetCurMission();
	if(pCurMission != NULL)
	{
		msg.dwMissionCount = pCurMission->GetCount();
		msg.dwCurMissionIDX = pCurMission->GetMissionIDX();
	}
	
	pPlayer->SendMsg(&msg, sizeof(msg));
}

void CInsDungeonManager::AddMissionEvent(MISSION_EVENT_INFO* pMissionEvent)
{
	CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser( pMissionEvent->pPlayer->GetID() );
	if( pPlayer == NULL )
		return;

	DWORD dwKey = pPlayer->GetChannelID();
	CInsDungeon* pInsDungeon = m_InsDungeonTable.GetData(dwKey);
	if(pInsDungeon == NULL)
		return;

	pInsDungeon->AddMissionEvent(pMissionEvent);

}

void CInsDungeonManager::DeleteMissionItem(CPlayer* pPlayer)
{
	CItemSlot * pSlot = pPlayer->GetSlot(eItemTable_Inventory);

	POSTYPE startPos = pSlot->GetStartPos();
	POSTYPE EndPos = startPos +  pSlot->GetSlotNum();	

	for(int i = startPos ; i < EndPos ; ++i )
	{
		const ITEMBASE* pItemBase = pSlot->GetItemInfoAbs(i);
		if(pItemBase == NULL)
			continue;

		ITEM_INFO* pItemInfo = ITEMMGR->GetItemInfo(pItemBase->wIconIdx);
		if(pItemInfo == NULL)
			continue;		

		//미션 아이탬 검사
		INSDG_MISSION_ITEM_INFO* pMissionItemInfo = INSDUNGEONMISSIONDATAMGR->GetMissionItemInfo(pItemBase->wIconIdx);
		//미션 아이탬 정보가 있으면 미션 아이탬이다~!
		if(pMissionItemInfo != NULL)
		{
			if( EI_TRUE != ITEMMGR->DiscardItem(pPlayer, pItemBase->Position, pItemBase->wIconIdx, pItemBase->Durability) )
				ASSERT(0);
		}		
	}
}

void CInsDungeonManager::InsDungeonDestoryForCheckPlayer(CPlayer* pPlayer)
{
	if(pPlayer == NULL)
		return;

	CInsDungeon* pInsDungeon = m_InsDungeonTable.GetData(pPlayer->GetChannelID());
	if(pInsDungeon == NULL)
		return;

	pInsDungeon->InsDungeonDestoryForCheckPlayer(pPlayer);
}

BOOL CInsDungeonManager::LoadRankStandardInfo()
{
	CMHFile file;
	char filename[256];
#ifdef _FILE_BIN_
	sprintf(filename,"Resource/Server/InsDGRankStandard.bin");
	if(!file.Init(filename,"rb"))
		return FALSE;
#else
	sprintf(filename,"Resource/Server/InsDGRankStandard.txt");
	if(!file.Init(filename,"rt"))
		return FALSE;
#endif	

	while(1)
	{
		if(file.IsEOF())
			break;

		INSDG_RANK_STANDARD* pRankStandard = new INSDG_RANK_STANDARD;

		pRankStandard->dwInsDGIndex = file.GetDword();

		for (int i = 0 ; i < INSDG_RANK_STANDARD_MAX ; i++)
		{
			pRankStandard->dwClearTime[i] = file.GetDword();
		}
		
		for (int i = 0 ; i < INSDG_RANK_STANDARD_MAX ; i++)
		{
			pRankStandard->dwMonsterKill[i] = file.GetDword();
		}

		m_InsDungeonRankStandardTable.Add(pRankStandard, pRankStandard->dwInsDGIndex);
	}
	file.Release();

	return TRUE;
}

void CInsDungeonManager::RemoveRankStandardInfo()
{
	INSDG_RANK_STANDARD* pRankStandard = NULL;

	m_InsDungeonRankStandardTable.SetPositionHead();
	while(pRankStandard = m_InsDungeonRankStandardTable.GetData())
	{
		SAFE_DELETE(pRankStandard);		
	}
	m_InsDungeonRankStandardTable.RemoveAll();
}

INSDG_RANK_STANDARD* CInsDungeonManager::GetRankStandardInfo(DWORD dwInsDGIndex)
{
	return m_InsDungeonRankStandardTable.GetData(dwInsDGIndex);
}

DWORD CInsDungeonManager::ResultRank(DWORD dwInsDGIndex, DWORD dwClearTime, DWORD dwMonsterKillNum, DWORD dwPlayerCount)
{
	DWORD dwResultRankValue = 0;
	DWORD dwClearTimeGrade = 0;
	DWORD dwHuntGrade = 0;
	DWORD dwPlayerCountGrade = 0;

	INSDG_RANK_STANDARD* pRankStandard = GetRankStandardInfo(dwInsDGIndex);
	if(pRankStandard == NULL)
		return 0;	
	
	//점수는 각 기준 데이터 값을 비교하여 0, 1, 2, 3, 4 점으로 준다
	for (int i = INSDG_RANK_STANDARD_MAX-1 ; i >= 0 ; i--)
	{
		
		//클리어타임 점수
		if(dwClearTime < pRankStandard->dwClearTime[i])
            dwClearTimeGrade++;

		//사냥 점수
		if(dwMonsterKillNum > pRankStandard->dwMonsterKill[i])			
			dwHuntGrade++;
	}

	//파티원 점수
	//1~5인 기준에 따라 4, 3, 2, 1, 0의 점수를 준다
	for (int i = 1 ; i <= INSDG_RANK_STANDARD_MAX+1 ; i++)
	{
		if(dwPlayerCount >= i)
			dwPlayerCountGrade = (INSDG_RANK_STANDARD_MAX+1) - i;
	}

	dwResultRankValue = dwClearTimeGrade + dwHuntGrade + dwPlayerCountGrade;


	return dwResultRankValue;
}

//랭크에 다라 보상 배율을 정한다.
float CInsDungeonManager::RankRequitalRatio(DWORD dwRankValue)
{
	DWORD dwRank = this->GetRank(dwRankValue);
	float fRatio = 0.0f;

	switch(dwRank)
	{
	case eINSDG_RANK_S:
		fRatio = 1.5f;
		break;
	case eINSDG_RANK_A:
		fRatio = 1.2f;
		break;
	case eINSDG_RANK_B:
		fRatio = 1.0f;
		break;
	case eINSDG_RANK_C:
		fRatio = 0.5f;
		break;
	case eINSDG_RANK_F:
		fRatio = 0.0f;
		break;
	}	

	return fRatio;
}

DWORD CInsDungeonManager::GetRank(DWORD dwRankValue)
{
	DWORD dwRank = eINSDG_RANK_F;

	if(dwRankValue >= 10)		// 10점 이상이면 S
		dwRank = eINSDG_RANK_S;
	else if( (dwRankValue >= 8) && (dwRankValue <= 9) )	// 8 ~ 9 점이면 A
		dwRank = eINSDG_RANK_A;
	else if( (dwRankValue >= 5) && (dwRankValue <= 7) )	// 5 ~ 7 점이면 B
		dwRank = eINSDG_RANK_B;
	else if( (dwRankValue >= 2) && (dwRankValue <= 4) )	// 2 ~ 4 점이면 C
		dwRank = eINSDG_RANK_C;
	else if(dwRankValue < 2)	// 2 ~ 4 점이면 F	
		dwRank = eINSDG_RANK_F;

	return dwRank;
}

CYHHashTable<INSDG_KIND_INFO>* CInsDungeonManager::GetInsDungeonKindMapTable()
{
	return &m_InsDungeonKindMapTable;
}

void CInsDungeonManager::RmoveInsDungeonKindMapTable()
{
	INSDG_KIND_INFO* pMapKindInfo = NULL;

	m_InsDungeonKindMapTable.SetPositionHead();
	while(pMapKindInfo = m_InsDungeonKindMapTable.GetData())
	{
		SAFE_DELETE(pMapKindInfo);		
	}
	m_InsDungeonKindMapTable.RemoveAll();
}

void CInsDungeonManager::PartyBreakup(DWORD dwPartyID)
{
	CInsDungeon* pTempInsDungeon = NULL;
	CInsDungeon* pInsDungeon = NULL;

	m_InsDungeonTable.SetPositionHead();
	while (pTempInsDungeon = m_InsDungeonTable.GetData())
	{
		if( pTempInsDungeon->GetPartyIDX() == dwPartyID )
		{
			pInsDungeon = pTempInsDungeon;
			break;
		}
	}

	if(pInsDungeon != NULL)
		pInsDungeon->PartyBreakup();
}

void CInsDungeonManager::PartyBan(CPlayer* pPlayer)
{
	if(pPlayer == NULL)
		return;

	CInsDungeon* pInsDungeon = this->GetInsDungeon(pPlayer->GetChannelID());
	if(pInsDungeon == NULL)
	{
		ASSERT(0);
		return;
	}

	pInsDungeon->PartyBan(pPlayer);
}
