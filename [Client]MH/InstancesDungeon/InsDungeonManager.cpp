#include "stdafx.h"
#include "InsDungeonManager.h"
#include "MHFile.h"
#include "Player.h"
#include "ChatManager.h"
#include "ObjectManager.h"
#include "QuestManager.h"
#include "GameIn.h"
#include "InventoryExDialog.h"
#include "MHMap.h"
#include "MainGame.h"
#include "MapChange.h"
#include "./interface/cWindowManager.h"
#include "WindowIDEnum.h"
#include "cMsgBox.h"
#include "ObjectStateManager.h"
#include "InsDGMissionManager.h"
#include "InsDGInfoDialog.h"
#include "InsDGMissionString.h"
#include "InsDGMissionInfoDialog.h"
#include "InsDGRankDialog.h"
#include "Quest.h"
#include "cImage.h"
#include "cScriptManager.h"
#include "GameResourceManager.h"
#include "InsDGRankInfoDialog.h"
GLOBALTON(CInsDungeonManager);
CInsDungeonManager::CInsDungeonManager()
{
	m_EntranceConditionTable.Initialize(100);
	m_InsDungeonInfoTable.Initialize(100);
	m_InsDGMissionStringTable.Initialize(100);
	m_InsDGRankInfoTable.Initialize(50);
	InitData();
}
CInsDungeonManager::~CInsDungeonManager()
{
	RemoveEntranceConditionInfo();
	RemoveInsDungeonInfo();	
	SAFE_DELETE(m_pInsDGMissionManager);
	RemoveMissionString();
	RemoveInsDGRankInfoTable();
}
void CInsDungeonManager::Init()
{
	LoadEntranceConditionInfo();
	LoadInsDungeonInfo();    
	LoadMissionString();
}
void CInsDungeonManager::Release()
{
	if( m_pInsDGMissionManager != NULL )
		m_pInsDGMissionManager->RemoveInsDGMissionInfoList();
	RemoveInsDGRankInfoTable();
}
void CInsDungeonManager::InitData()
{
	m_pCurInsDungeonInfo = NULL;	
	m_eInsDungeonType = eINSDG_TYPE_PARTY;
	m_dwChannel = 0;
	m_dwPartyIDX = 0;
	m_dwMunpaIDX = 0;
	m_dwMonsterKillNum = 0;
	m_dwInsDGStartTime = 0;	
	m_dwInsDGEndTime = 0;
	m_dwMaxMonsterCount = 0;
	m_dwProgressTime = 0;
	m_dwRequitalTime = 0;
	m_bInsDGStart = FALSE;	
	InitRankState();
	InitInsDGStartTime();
}
void CInsDungeonManager::NetworkMsgParse( BYTE Protocol, void* pMsg )
{
	switch(Protocol)
	{
	case MP_INSDUNGEON_ENTRANCE_ACK:
		{
			MSG_INSDG_ENTRANCE* pmsg = (MSG_INSDG_ENTRANCE*)pMsg;
			INSDUNGEONMGR->SetRankState(&pmsg->RankState);
			WINDOWMGR->MsgBox( MBI_NOBTNMSGBOX, MBT_NOBTN, CHATMGR->GetChatMsg( 160 ) );
			WORD wMapnum = pmsg->dwData;
			GAMEIN->SetMoveMap(MAP->GetMapNum());
			GAMEIN->SetInsDungeonMapNum(wMapnum);
			MAPCHANGE->SetGameInInitKind( eGameInInitKind_InsDungeonMapEnter );
			MAINGAME->SetGameState( eGAMESTATE_MAPCHANGE, &wMapnum, 4 );
			OBJECTSTATEMGR->EndObjectState( HERO, eObjectState_Deal );			
		}
		break;
	case MP_INSDUNGEON_ENTRANCE_NACK:
		{
			MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;
			EntranceErrorMsg((eINSDG_ENTRANCE_ERROR)pmsg->dwData);
		}
		break;
	case MP_INSDUNGEON_ENTER_NACK:
		{
			MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;
			EntranceErrorMsg((eINSDG_ENTRANCE_ERROR)pmsg->dwData);
		}
		break;
	case MP_INSDUNGEON_CREATE_ACK:
		{
			MSG_DWORD3* pmsg = (MSG_DWORD3*)pMsg;
			m_dwChannel = pmsg->dwData1;
			switch(m_eInsDungeonType)
			{
			case eINSDG_TYPE_PARTY:	
				m_dwPartyIDX = pmsg->dwData2;
				break;
			case eINSDG_TYPE_MUNPA:	
				m_dwMunpaIDX = pmsg->dwData2;
				break;
			}
			SetCurrentInsDungeonInfo(MAP->GetMapNum());			
		}
		break;	
	case MP_INSDUNGEON_RETURNTOMAP:
		{
			MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;
			WINDOWMGR->MsgBox( MBI_NOBTNMSGBOX, MBT_NOBTN, CHATMGR->GetChatMsg( 160 ) );
			MAPCHANGE->SetGameInInitKind( eGameInInitKind_InsDungeonMapLeave );
			DWORD dwMoveMapNum = pmsg->dwData;
			MAPTYPE wCurMapNum = MAP->GetMapNum();
			INSDUNGEON_INFO* pInsDungeonInfo = INSDUNGEONMGR->GetInsDungeonInfo(wCurMapNum);
			if(pInsDungeonInfo == NULL)
				return;
			MAPCHANGE_INFO* pMapchangeInfo = GAMERESRCMNGR->GetMapChangeInfo(pInsDungeonInfo->dwEscapeMapChangeKind);
			if(pMapchangeInfo == NULL)
				return;
			stMOVEPOINT movePt;
			movePt.SetMovePoint( pMapchangeInfo->MovePoint.x, pMapchangeInfo->MovePoint.z );
			DWORD dwMovePoint = movePt.value;			
			MAINGAME->SetGameState( eGAMESTATE_MAPCHANGE, &dwMoveMapNum, 4 );
			CHATMGR->SaveChatList();
			InitData();
		}
		break;
	case MP_INSDUNGEON_INFO_ACK:
		{
			MSG_INSDUNGEON_INFO* pmsg = (MSG_INSDUNGEON_INFO*)pMsg;
			m_dwChannel = pmsg->dwChannel;
			m_dwPartyIDX = pmsg->dwPartyIDX;
			m_dwMunpaIDX = pmsg->dwMunpaIDX;
			m_dwRequitalTime = pmsg->dwRequitalTime;
			StartInsDGStartTime();
			m_dwInsDGEndTime = m_dwInsDGStartTime + pmsg->dwProgressTime;
			SetCurrentInsDungeonInfo(MAP->GetMapNum());
			m_pInsDGMissionManager->SetCurMission(pmsg->dwCurMissionIDX);
			CInsDGMission* pCurMission = m_pInsDGMissionManager->GetCurMission();
			if(pCurMission != NULL)
				pCurMission->SetCurrentCount(pmsg->dwMissionCount);
			m_pInsDGMissionManager->SetSuccess(pmsg->bMissionSuccess);
			CInsDGInfoDialog* pInsDGInfoDlg = GAMEIN->GetInsDGInfoDlg();
			if(pInsDGInfoDlg != NULL && m_pCurInsDungeonInfo)
			{
				pInsDGInfoDlg->SetEndTime(m_pCurInsDungeonInfo->dwRestrictiveTime);
				pInsDGInfoDlg->SetProgressTime(pmsg->dwProgressTime);
				pInsDGInfoDlg->SetActive(TRUE);
				DWORD dwMaxCount = 0;
				if(pCurMission != NULL)
					 dwMaxCount = pCurMission->GetMaxCount();
				pInsDGInfoDlg->SetMissionCount(pmsg->dwMissionCount, dwMaxCount);
			}
			CInsDGMissionInfoDialog* pMissionInfoDlg = GAMEIN->GetInsDGMissionInfoDlg();
			if(pMissionInfoDlg != NULL)
				pMissionInfoDlg->SetActive(TRUE);
		}
		break;
	case MP_INSDUNGEON_MISSION_UPDATE:
		{
			MSG_INSDG_MISSION_UPDATE* pmsg = (MSG_INSDG_MISSION_UPDATE*)pMsg;
			m_pInsDGMissionManager->MissionUpdate(pmsg);			
		}
		break;
	case MP_INSDUNGEON_MISSION_RESULT:
		{
			MSG_DWORD4* pmsg = (MSG_DWORD4*)pMsg;			
			m_pInsDGMissionManager->MissionResult(pmsg);
		}
		break;	
	case MP_INSDUNGEON_CLEAR_RANK_INFO:
		{
			MSG_INSDG_RANK_INFO* pmsg = (MSG_INSDG_RANK_INFO*)pMsg;			
			CInsDGRankDialog* pInsDGRankDlg = GAMEIN->GetInsDGRankDialog();
			if(pInsDGRankDlg == NULL)
				return;
			pInsDGRankDlg->SetRankInfo(pmsg);
			pInsDGRankDlg->SetActive(TRUE);
		}
		break;	
	case MP_INSDUNGEON_RANK_TOTAL_INFO:
		{
			MSG_INSDG_RANK_TOTAL_INFO* pmsg = (MSG_INSDG_RANK_TOTAL_INFO*)pMsg;
            SetInsDGRankTotalInfo(pmsg);
		}
		break;
	case MP_INSDUNGEON_CLEAR_MESSAGE:
		{
			MSGBASE* pmsg = (MSGBASE*)pMsg;
			CHATMGR->AddMsg( CTC_QUEST, CHATMGR->GetChatMsg(1854) );
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
INSDG_ENTRANCECONDITION_INFO* CInsDungeonManager::GetEntranceConditionInfo(DWORD dwNpcUniqueIDX)
{
	return m_EntranceConditionTable.GetData(dwNpcUniqueIDX);
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
	while(1)
	{
		if(file.IsEOF())
			break;
		INSDUNGEON_INFO* pInsDungeonInfo = new INSDUNGEON_INFO;
		pInsDungeonInfo->MapNum = file.GetWord();		
		pInsDungeonInfo->dwInsDGIndex = file.GetDword();
		memset(pInsDungeonInfo->szInsDGName, 0, sizeof(*pInsDungeonInfo->szInsDGName));
		SafeStrCpy( pInsDungeonInfo->szInsDGName, file.GetString(), MAX_INSDUNGEONNAME_LENGTH+1 );
		pInsDungeonInfo->eType = (eINSDG_TYPE)file.GetDword();
		pInsDungeonInfo->EscapeMapNum = file.GetWord();
		pInsDungeonInfo->dwEscapeMapChangeKind = file.GetDword();
		pInsDungeonInfo->NextMapNum = file.GetWord();
		pInsDungeonInfo->dwRestrictiveTime = file.GetDword();		
		pInsDungeonInfo->dwMissionGroupIDX = file.GetDword();
		pInsDungeonInfo->bFinalMap = file.GetBool();
		pInsDungeonInfo->dwClearLimitLevel = file.GetDword();
		pInsDungeonInfo->wRevisionLevel = file.GetWord();
		m_InsDungeonInfoTable.Add(pInsDungeonInfo, pInsDungeonInfo->MapNum);
	}
	file.Release();
	return TRUE;
}
void CInsDungeonManager::RemoveInsDungeonInfo()
{
	INSDUNGEON_INFO* pInsDungeonInfo = NULL;
	m_InsDungeonInfoTable.SetPositionHead();
	while(pInsDungeonInfo = m_InsDungeonInfoTable.GetData())
	{
		SAFE_DELETE(pInsDungeonInfo);
	}
	m_InsDungeonInfoTable.RemoveAll();
}
INSDUNGEON_INFO* CInsDungeonManager::GetInsDungeonInfo(MAPTYPE wMapNum)
{
	INSDUNGEON_INFO* pInsDungeonInfo = NULL;
	m_InsDungeonInfoTable.SetPositionHead();
	while(pInsDungeonInfo = m_InsDungeonInfoTable.GetData())
	{
		if(pInsDungeonInfo->MapNum == wMapNum)
			return pInsDungeonInfo;
	}
	return NULL;
}
CYHHashTable<INSDUNGEON_INFO>* CInsDungeonManager::GetInsDungeonInfoTable()
{
	return &m_InsDungeonInfoTable;
}
INSDUNGEON_INFO* CInsDungeonManager::GetInsDungeonInfo(DWORD dwInsDGIndex)
{
	INSDUNGEON_INFO* pInsDungeonInfo = NULL;
	m_InsDungeonInfoTable.SetPositionHead();
	while(pInsDungeonInfo = m_InsDungeonInfoTable.GetData())
	{
		if(pInsDungeonInfo->dwInsDGIndex == dwInsDGIndex)
			return pInsDungeonInfo;
	}
	return NULL;	
}

DWORD CInsDungeonManager::GetInsDungeonInfoCount()
{
	return m_InsDungeonInfoTable.GetDataNum();
}


BOOL CInsDungeonManager::LoadMissionString()
{
	CMHFile file;
#ifdef _FILE_BIN_
	if( !file.Init( "Resource/InsDungeonScript/InsDGMissionString.bin", "rb") )
		return FALSE;
#else
	if( !file.Init( "Resource/InsDungeonScript/InsDGMissionString.txt", "rt") )
		return FALSE;
#endif
	char buf[256];
	DWORD Key=0;
	DWORD Key_1=0;
	DWORD Key_2=0;
	CInsDGMissionString* pMissionString = NULL;
	while( !file.IsEOF() )
	{
		file.GetString(buf);
		if(strcmp(buf,"$MISSIONSTR") == 0)	
		{
			Key_1 = file.GetDword();
			Key_2 = file.GetDword();
			COMBINEKEY(Key_1, Key_2, Key);
			pMissionString = LoadMissionStringInfo(&file);
			if(pMissionString)
			{
				m_InsDGMissionStringTable.Add(pMissionString, Key);
				pMissionString->SetIndex(Key_1, Key_2);
			}
		}
	}
	return TRUE;
}
CInsDGMissionString* CInsDungeonManager::LoadMissionStringInfo(CMHFile* pFile)
{
	CInsDGMissionString* pMissionString = new CInsDGMissionString;
	char* pChekString = NULL;
	char buf[1024];
	char Token[1024];
	int	tline=0;
	int dline=0;
	while( !pFile->IsEOF() )
	{
		pFile->GetString(Token);
		if( pChekString = strstr( Token, "}" ) )
			break;
		if(strcmp(Token,"#TITLE") == 0)
		{
			pFile->GetLine(buf, 1024);
			pMissionString->AddLine(buf, tline, TRUE);
		}
		if(strcmp(Token,"#DESC") == 0)
		{
			while( !pFile->IsEOF() ) 
			{
				pFile->GetLine(buf, 1024);
				char * p = buf;
				int nRt = 0;
				while( *p )
				{
					if( IsDBCSLeadByte( *p ) )
					{
						++p;
					}
					else
					{
						if( *p == '{' )
						{
							nRt = 1;
							break;
						}
						else if(  *p == '}' )
						{
							nRt = 2;
							break;
						}
					}
					++p;
				}
				if( nRt == 1 ) continue;
				else if( nRt == 2 ) break;
				pMissionString->AddLine(buf, dline);
				++dline;
			}
		}
	}
	return pMissionString;
}
void CInsDungeonManager::RemoveMissionString()
{
	CInsDGMissionString* pMissionString = NULL;
	m_InsDGMissionStringTable.SetPositionHead();
	while (pMissionString = m_InsDGMissionStringTable.GetData())
	{
		SAFE_DELETE(pMissionString);
	}
	m_InsDGMissionStringTable.RemoveAll();
}
BOOL CInsDungeonManager::LoadInsDungeonMissionInfo()
{
	INSDUNGEON_INFO* pInsDungeonInfo =  GetInsDungeonInfo(MAP->GetMapNum());
	if(pInsDungeonInfo == NULL)
		return FALSE;
	DWORD dwMissionGroupIndex = pInsDungeonInfo->dwMissionGroupIDX;	
	m_pInsDGMissionManager = new CInsDGMissionManager;
	BOOL bRt = m_pInsDGMissionManager->LoadInsDGMissionInfoList(dwMissionGroupIndex);
	if(bRt == FALSE)
		return FALSE;
	return TRUE;
}
eINSDG_ENTRANCE_ERROR CInsDungeonManager::CheckEntranceCondition(DWORD dwNPCUniqueIDX)
{
	LEVELTYPE wPlayerLevel = HERO->GetLevel();
	INSDG_ENTRANCECONDITION_INFO* pEntranceInfo = m_EntranceConditionTable.GetData(dwNPCUniqueIDX);
	if(pEntranceInfo == NULL)
		return eINSDG_ENTRANCE_ERROR_NULL;
	if(wPlayerLevel < pEntranceInfo->wMinLevel)
		return eINSDG_ENTRANCE_ERROR_LEVEL;
	if(wPlayerLevel > pEntranceInfo->wMaxLevel)
		return eINSDG_ENTRANCE_ERROR_LEVEL;
	if(pEntranceInfo->dwQuestIndex != 0)
	{
		CQuest* pQuest = QUESTMGR->GetQuest(pEntranceInfo->dwQuestIndex);
		if(pQuest == NULL)		
			return eINSDG_ENTRANCE_ERROR_QUEST;
	}
	CInventoryExDialog* pInvenDlg = GAMEIN->GetInventoryDialog();
	if(pInvenDlg == NULL)
		return eINSDG_ENTRANCE_ERROR_NULL;	
	if(HERO->GetMoney() < pEntranceInfo->dwMoney)
		return eINSDG_ENTRANCE_ERROR_MONEY;	
	return eINSDG_ENTRANCE_ERROR_NONE;
}
void CInsDungeonManager::EntranceErrorMsg(eINSDG_ENTRANCE_ERROR eError)
{
	switch(eError)
	{
	case eINSDG_ENTRANCE_ERROR_NULL:
		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(2704) );
		break;
	case eINSDG_ENTRANCE_ERROR_FAIL:
		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(2704) );
		break;
	case eINSDG_ENTRANCE_ERROR_LEVEL:
		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(2705) );
		break;
	case eINSDG_ENTRANCE_ERROR_QUEST:
		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(2706) );
		break;	
	case eINSDG_ENTRANCE_ERROR_ITEM:
		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(2707) );
		break;	
	case eINSDG_ENTRANCE_ERROR_MONEY:
		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(2708) );
		break;	
	case eINSDG_ENTRANCE_ERROR_MISSION:
		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(2709) );
		break;
	case eINSDG_ENTRANCE_ERROR_MOVEMAP:
		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(2710) );
		break;
	case eINSDG_ENTRANCE_ERROR_PARTY:
		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(2711) );
		break;
	case eINSDG_ENTRANCE_ERROR_MUNPA:
		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(2712) );
		break;		
	case eINSDG_ENTRANCE_ERROR_CLEAR:
		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(2713) );
		break;		
	}	
}
void CInsDungeonManager::InitInsDGStartTime()
{
	m_dwInsDGStartTime = 0;
	m_dwProgressTime = 0;
	m_bInsDGStart = FALSE;
}
void CInsDungeonManager::StartInsDGStartTime()
{
	m_dwInsDGStartTime = gCurTime;
	m_bInsDGStart = TRUE;
}
void CInsDungeonManager::SetCurrentInsDungeonInfo(MAPTYPE wMapNum)
{	
	m_pCurInsDungeonInfo = GetInsDungeonInfo(wMapNum);
}
void CInsDungeonManager::Process()
{
	if(m_bInsDGStart == FALSE)
		return;
	if(m_pCurInsDungeonInfo == NULL)
		return;
	if(m_dwInsDGEndTime > gCurTime)
	{
		DWORD dwTime = m_dwInsDGStartTime + m_dwInsDGEndTime + m_dwRequitalTime;
		m_dwProgressTime = dwTime - gCurTime;
		if(m_dwProgressTime > m_dwInsDGStartTime + m_dwInsDGEndTime)
			m_dwProgressTime = m_dwInsDGStartTime + m_dwInsDGEndTime;
	}		
}
void CInsDungeonManager::Render()
{
}
void CInsDungeonManager::SendNPCMissionEvent(CNpc* pNpc)
{
	MSG_DWORD msg;
	msg.Category = MP_INSDUNGEON;
	msg.Protocol = MP_INSDUNGEON_NPC_MISSIONEVENT_SYN;
	msg.dwObjectID = HEROID;
	msg.dwData = pNpc->GetNpcUniqueIdx();
	NETWORK->Send( &msg, sizeof(msg) );
}
void CInsDungeonManager::InitRankState()
{
	memset(&m_RankState, 0, sizeof(m_RankState));
}
void CInsDungeonManager::SetRankState(INSDG_RANK_STATE* pRankState)
{
	m_RankState.dwRankTime = pRankState->dwRankTime;
	m_RankState.dwMonsterKillNum = pRankState->dwMonsterKillNum;
}
INSDG_RANK_STATE* CInsDungeonManager::GetRankState()
{
	return &m_RankState;
}
void CInsDungeonManager::RemoveInsDGRankInfoTable()
{
	INSDG_RANK_INFO* pRankInfo = NULL;
	m_InsDGRankInfoTable.SetPositionHead();
	while (pRankInfo = m_InsDGRankInfoTable.GetData())
	{
		SAFE_DELETE(pRankInfo);
	}
	m_InsDGRankInfoTable.RemoveAll();
}
void CInsDungeonManager::AddInsDGRankInfo(INSDG_RANK_INFO* pRankInfo)
{
	if(pRankInfo == NULL)
		return;
	INSDG_RANK_INFO* pInsDGRankInfo = new INSDG_RANK_INFO;
	memcpy(pInsDGRankInfo, pRankInfo, sizeof(*pInsDGRankInfo));
	m_InsDGRankInfoTable.Add(pInsDGRankInfo, pInsDGRankInfo->dwInsDGIndex);
}
INSDG_RANK_INFO* CInsDungeonManager::GetInsDGRankInfo(DWORD dwInsDGIndex)
{
	return m_InsDGRankInfoTable.GetData(dwInsDGIndex);
}
void CInsDungeonManager::SetInsDGRankTotalInfo(MSG_INSDG_RANK_TOTAL_INFO* pMsg)
{
	int nCount = pMsg->nCount;
	for(int i = 0 ; i < nCount ; i++)
	{
		AddInsDGRankInfo(&pMsg->RankInfo[i]);
		CInsDGRankInfoDialog* pInsDGRankInfoDlg = GAMEIN->GetInsDGRankInfoDialog();
		if(pInsDGRankInfoDlg != NULL)
			pInsDGRankInfoDlg->AddRankInfoState(&pMsg->RankInfo[i]);
	}
}
CYHHashTable<INSDG_RANK_INFO>* CInsDungeonManager::GetInsDGRankInfoTable()
{
	return &m_InsDGRankInfoTable;
}
DWORD CInsDungeonManager::GetRank(DWORD dwRankValue)
{
	DWORD dwRank = eINSDG_RANK_F;
	if(dwRankValue >= 10)		
		dwRank = eINSDG_RANK_S;
	else if( (dwRankValue >= 8) && (dwRankValue <= 9) )	
		dwRank = eINSDG_RANK_A;
	else if( (dwRankValue >= 5) && (dwRankValue <= 7) )	
		dwRank = eINSDG_RANK_B;
	else if( (dwRankValue >= 2) && (dwRankValue <= 4) )	
		dwRank = eINSDG_RANK_C;
	else if(dwRankValue < 2)	
		dwRank = eINSDG_RANK_F;
	return dwRank;
}
