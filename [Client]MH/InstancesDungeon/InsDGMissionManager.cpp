#include "StdAfx.h"
#include "InsDGMissionManager.h"
#include "GameIn.h"
#include "InsDGInfoDialog.h"
#include "ChatManager.h"
#include "InsDGMissionInfoDialog.h"
#include "InsDungeonManager.h"
#include "MHMap.h"
#include "..\Effect\EffectManager.h"
#include "ObjectManager.h"
#include "aimanager.h"
CInsDGMissionManager::CInsDGMissionManager()
{
	m_InsDGMissionTable.Initialize(100);	
	m_pCurrentMission = NULL;
	m_dwMissionGroupIDX = 0;
	m_bSuccess = FALSE;
}
CInsDGMissionManager::~CInsDGMissionManager()
{
	RemoveInsDGMissionInfoList();
}
BOOL CInsDGMissionManager::LoadInsDGMissionInfoList(DWORD dwMissionGroupIndex)
{
	if(dwMissionGroupIndex == 0)
		return FALSE;
	CMHFile file;
	char filename[256];
#ifdef _FILE_BIN_	
	sprintf(filename,"Resource/InsDungeonScript/InsDGMission.bin");
	if(!file.Init(filename,"rb"))
		return FALSE;
#else
	sprintf(filename,"Resource/InsDungeonScript/InsDGMission.bin");
	if(!file.Init(filename,"rt"))
		return FALSE;
#endif
	char buff[256] = {0,};
	CInsDGMission* pMission = NULL;
	BOOL bSuccess = TRUE;
	while(1)
	{
		if(file.IsEOF() == TRUE)
			break;
		file.GetString(buff);
		if(buff[0] == '{')
			continue;
		if(buff[0] == '}')
			continue;		
		if( strcmp(buff,"$MISSIONGROUP") == 0 )
		{
			DWORD dwMissionGroupIDX = file.GetDword();			
			if(dwMissionGroupIDX != dwMissionGroupIndex)
				continue;
			m_dwMissionGroupIDX = dwMissionGroupIDX;
			bSuccess = LoadMissionGroupData(&file, dwMissionGroupIDX);
			if(bSuccess == FALSE)
				return FALSE;
		}
	}
	file.Release();	
	return TRUE;
}
BOOL CInsDGMissionManager::LoadMissionGroupData(CMHFile* pFile, DWORD dwMissionGroupIDX)
{
	char buff[256] = {0,};
	memset(buff, 0, sizeof(buff));
	DWORD dwMissionIDX = 0;
	CInsDGMission* pMission = NULL;
	DWORD dwKey = 0;
	DWORD dwKey1 = 0;
	DWORD dwKey2 = 0;
	while(1)
	{		
		pFile->GetString(buff);
		if(buff[0] == '{')
			continue;
		if(buff[0] == '}')
			break;
		if( strcmp(buff,"$MISSION") == 0 )
		{
			dwMissionIDX = pFile->GetDword();
			pMission = LoadMissionInfoData(pFile, dwMissionGroupIDX, dwMissionIDX);
			if(pMission == NULL)
				return FALSE;			
			m_InsDGMissionTable.Add(pMission, dwMissionIDX);
			pMission = NULL;
		}
	}
	return TRUE;
}
CInsDGMission* CInsDGMissionManager::LoadMissionInfoData(CMHFile* pFile, DWORD dwMissionGroupIDX, DWORD dwMissionIDX)
{
	char buff[256] = {0,};	
	CInsDGMission* pMission = NULL;
	while(1)
	{
		pFile->GetString(buff);
		if(buff[0] == '{')
			continue;
		if(buff[0] == '}')
			break;
		if( strcmp(buff,"#MONSTER_TARGETKILL") == 0 )
		{
			DWORD dwMonsterIndex = pFile->GetDword();
			pMission = CreateMission(dwMissionGroupIDX, dwMissionIDX, eMISSION_EXECUTE_KIND_MONSTER_TARGETKILL, dwMonsterIndex);
		}
		else if( strcmp(buff,"#MONSTER_KILL") == 0 )
		{
			pFile->GetDword();
			pMission = CreateMission(dwMissionGroupIDX, dwMissionIDX, eMISSION_EXECUTE_KIND_MONSTER_KILL, 0);
		}
		else if( strcmp(buff,"#MONSTER_ALLKILL") == 0 )
		{
			pFile->GetDword();	
			pMission = CreateMission(dwMissionGroupIDX, dwMissionIDX, eMISSION_EXECUTE_KIND_MONSTER_KILLALL, 0);
		}
		else if( strcmp(buff,"#ITEM_USE") == 0 )
		{
			WORD wItemIndex = pFile->GetWord();			
			pFile->GetDword();
			pMission = CreateMission(dwMissionGroupIDX, dwMissionIDX, eMISSION_EXECUTE_KIND_ITEM_USE, 0);
		}
		else if( strcmp(buff,"#ITEM_GET") == 0 )
		{
			WORD wItemIndex = pFile->GetWord();
			pFile->GetDword();
			pMission = CreateMission(dwMissionGroupIDX, dwMissionIDX, eMISSION_EXECUTE_KIND_ITEM_GET, 0);
		}
		else if( strcmp(buff,"#ITEM_TARGET_GET") == 0 )
		{
			WORD wItemIndex = pFile->GetWord();
			pFile->GetDword();
			pFile->GetDword();
			pMission = CreateMission(dwMissionGroupIDX, dwMissionIDX, eMISSION_EXECUTE_KIND_ITEM_GET, 0);
		}
		else if( strcmp(buff,"#NPC_TALK") == 0 )
		{
			DWORD dwNPCIndex = pFile->GetDword();
			pMission = CreateMission(dwMissionGroupIDX, dwMissionIDX, eMISSION_EXECUTE_KIND_NPC_TALK, dwNPCIndex);
		}		
		else if( strcmp(buff,"#MAXCOUNT") == 0 )
		{
			DWORD dwMaxCount = pFile->GetDword();
			pMission->SetMaxCount(dwMaxCount);
		}
		else if( strcmp(buff,"#NEXT_MISSION") == 0 )
		{
			DWORD dwNextMissionIndex = pFile->GetDword();
			pMission->SetNextMissionIDX(dwNextMissionIndex);
		}		
	}
	return pMission;
}
CInsDGMission* CInsDGMissionManager::CreateMission(DWORD dwMissionGroupIDX, DWORD dwMissionIDX, DWORD dwMissionEcevuteKind, DWORD dwMissionTargetIDX)
{
	CInsDGMission* pMission = NULL;
	pMission = new CInsDGMission(dwMissionGroupIDX, dwMissionIDX);
	pMission->SetMissionExecuteKind(dwMissionEcevuteKind);
	pMission->SetMissionTargetIDX(dwMissionTargetIDX);
	return pMission;
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
void CInsDGMissionManager::Process()
{
}
void CInsDGMissionManager::MissionUpdate(MSG_INSDG_MISSION_UPDATE* pMsg)
{
	if(m_pCurrentMission == NULL)
		return;
	m_pCurrentMission->SetCurrentCount(pMsg->dwCount);
	CInsDGInfoDialog* pInsDGInfoDlg = GAMEIN->GetInsDGInfoDlg();
	if(pInsDGInfoDlg != NULL)
	{
		pInsDGInfoDlg->SetMissionCount(pMsg->dwCount, m_pCurrentMission->GetMaxCount());
	}
}
void CInsDGMissionManager::SetCurMission(DWORD dwMissionIDX)
{
	m_pCurrentMission = m_InsDGMissionTable.GetData(dwMissionIDX);	
}
void CInsDGMissionManager::MissionResult(MSG_DWORD4* pMsg)
{
	DWORD dwMissionGroupIDX = pMsg->dwData1;
	DWORD dwMissionIDX = pMsg->dwData2;
	DWORD dwNextMissionIDX = pMsg->dwData3;
	if(m_pCurrentMission->GetMissionGroupIDX() != dwMissionGroupIDX)
	{
		ASSERT(0);
		return;
	}
	if(m_pCurrentMission->GetMissionIDX() != dwMissionIDX)
	{
		ASSERT(0);
		return;
	}
	m_pCurrentMission->SetSuccess(TRUE);	
	CHATMGR->AddMsg( CTC_QUEST, CHATMGR->GetChatMsg(2698) );
	DWORD dwRequitalTime = INSDUNGEONMGR->GetRequitalTime();
	if(dwRequitalTime != pMsg->dwData4)
	{
		DWORD dwTime = (pMsg->dwData4 - dwRequitalTime) / 1000;
		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(2699), dwTime);
	}
	INSDUNGEONMGR->SetRequitalTime(pMsg->dwData4);
	if(HERO->InTitan() == TRUE)
		EFFECTMGR->StartEffectProcess(eEffect_InctanceDungeon_MissionSuccess_Titan,HERO,NULL,0,0);
	else
		EFFECTMGR->StartEffectProcess(eEffect_InctanceDungeon_MissionSuccess,HERO,NULL,0,0);
	if(dwNextMissionIDX != 0)
	{
		CInsDGMission* pMission = m_InsDGMissionTable.GetData(dwNextMissionIDX);
		if(pMission == NULL)
		{
			ASSERT(0);
			return;
		}
		m_pCurrentMission = pMission;
        CHATMGR->AddMsg( CTC_QUEST, CHATMGR->GetChatMsg(2700) );
	}
	else
	{
		m_pCurrentMission = NULL;
		this->SetSuccess(TRUE);	
		INSDUNGEON_INFO* pDungeonInfo = INSDUNGEONMGR->GetInsDungeonInfo(MAP->GetMapNum());
		if(pDungeonInfo == NULL)		
			return;
		if(pDungeonInfo->bFinalMap == TRUE)
		{
			CHATMGR->AddMsg( CTC_QUEST, CHATMGR->GetChatMsg(2701) );
			CHATMGR->AddMsg( CTC_QUEST, CHATMGR->GetChatMsg(2702) );
			//AISETMGR->GetGameAI()->sPage5.pbAutoLeveling == false;
			sCPlayerAI.SetAIDisable();
		}
		else
			CHATMGR->AddMsg( CTC_QUEST, CHATMGR->GetChatMsg(2703) );
	}
	CInsDGInfoDialog* pInsDGInfoDlg = GAMEIN->GetInsDGInfoDlg();
	if(pInsDGInfoDlg != NULL)
	{
		if(m_pCurrentMission != NULL)
			pInsDGInfoDlg->SetMissionCount(0, m_pCurrentMission->GetMaxCount());
		else
			pInsDGInfoDlg->SetMissionCount(0, 0);
	}
	CInsDGMissionInfoDialog* pMissionInfoDlg = GAMEIN->GetInsDGMissionInfoDlg();
	if(pMissionInfoDlg != NULL)
	{
		pMissionInfoDlg->SetMissionString();
	}	
}
