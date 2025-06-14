#include "StdAfx.h"
#include "InsDungeonMissionDataManager.h"
#include "MHFile.h"
#include "InsDGMission.h"
#include "InsDGMissionExecute.h"
#include "InsDGMissionExecute_Item.h"
#include "ItemManager.h"
#include "InsDGMissionRegenGroup.h"
#include "InsDGMissionRegenInfo.h"

GLOBALTON(CInsDungeonMissionDataManager);

CInsDungeonMissionDataManager::CInsDungeonMissionDataManager()
{
	m_InsDGMissionTable.Initialize(100);
	m_InsDGMissionItemInfoTable.Initialize(100);
	m_InsDGMissionRegenGroupTable.Initialize(50);
}

CInsDungeonMissionDataManager::~CInsDungeonMissionDataManager()
{
	RemoveAllInsDGMissionList();
	RemoveAllInsDGMissionItemInfo();
	RemoveAllInsDGMissionRegenList();
}

void CInsDungeonMissionDataManager::RemoveAllInsDGMissionList()
{
	CInsDGMission* pMission = NULL;

	m_InsDGMissionTable.SetPositionHead();
	while(pMission = m_InsDGMissionTable.GetData())
	{
		SAFE_DELETE(pMission);
	}
	m_InsDGMissionTable.RemoveAll();
}

BOOL CInsDungeonMissionDataManager::LoadInsDungeonMissionList(DWORD dwMissionGroupIndex)
{
	//미션이 없으면 FALSE
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

			//해당 던전의 미션데이터만 로드
			if(dwMissionGroupIDX != dwMissionGroupIndex)
				continue;

			bSuccess = LoadMissionGroupData(&file, dwMissionGroupIDX);
			if(bSuccess == FALSE)
				return FALSE;
		}
	}
	file.Release();	

	return TRUE;
}

BOOL CInsDungeonMissionDataManager::LoadMissionGroupData(CMHFile* pFile, DWORD dwMissionGroupIDX)
{
	char buff[256] = {0,};
	memset(buff, 0, sizeof(buff));
	DWORD dwMissionIDX = 0;
	CInsDGMission* pMission = NULL;

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

			m_InsDGMissionTable.Add(pMission, pMission->GetMissionIDX());
			pMission = NULL;
		}
	}

	return TRUE;
}

CInsDGMission* CInsDungeonMissionDataManager::LoadMissionInfoData(CMHFile* pFile, DWORD dwMissionGroupIDX, DWORD dwMissionIDX)
{
	char buff[256] = {0,};	
	CInsDGMission* pMission = NULL;
	MISSION_REQUITAL MissionRequital;
	memset(&MissionRequital, 0, sizeof(MissionRequital));

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

			pMission = new CInsDGMission(dwMissionGroupIDX, dwMissionIDX);
			pMission->CreateMissionEvent(eMISSION_EVENT_KIND_HUNT);
			pMission->CreateMissionExecute(eMISSION_EXECUTE_KIND_MONSTER_TARGETKILL, dwMissionIDX, dwMonsterIndex, 0, 0);
		}
		else if( strcmp(buff,"#MONSTER_KILL") == 0 )
		{
			pFile->GetDword();	//이값은 안쓴다.
			
			pMission = new CInsDGMission(dwMissionGroupIDX, dwMissionIDX);
			pMission->CreateMissionEvent(eMISSION_EVENT_KIND_HUNT);
			pMission->CreateMissionExecute(eMISSION_EXECUTE_KIND_MONSTER_KILL, dwMissionIDX, 0, 0, 0);
		}
		else if( strcmp(buff,"#MONSTER_ALLKILL") == 0 )
		{
			pFile->GetDword();	//몬스터 전멸은 인덱스가 필요없다

			pMission = new CInsDGMission(dwMissionGroupIDX, dwMissionIDX);
			pMission->CreateMissionEvent(eMISSION_EVENT_KIND_HUNT);
			pMission->CreateMissionExecute(eMISSION_EXECUTE_KIND_MONSTER_KILLALL, dwMissionIDX, 0, 0, 0);
		}
		else if( strcmp(buff,"#ITEM_USE") == 0 )
		{
			WORD wItemIndex = pFile->GetWord();			
			DWORD dwRate = pFile->GetDword();
			
			pMission = new CInsDGMission(dwMissionGroupIDX, dwMissionIDX);
			pMission->CreateMissionEvent(eMISSION_EVENT_KIND_ITEM);			
			pMission->CreateMissionExecute(eMISSION_EXECUTE_KIND_ITEM_USE, dwMissionIDX, wItemIndex, dwRate, 0);

			AddMissionItemInfo(wItemIndex, eMISSION_EXECUTE_KIND_ITEM_USE);
		}
		else if( strcmp(buff,"#ITEM_GET") == 0 )
		{
			WORD wItemIndex = pFile->GetWord();
			DWORD dwRate = pFile->GetDword();

			pMission = new CInsDGMission(dwMissionGroupIDX, dwMissionIDX);
			pMission->CreateMissionEvent(eMISSION_EVENT_KIND_HUNT);			
			pMission->CreateMissionExecute(eMISSION_EXECUTE_KIND_ITEM_GET, dwMissionIDX, wItemIndex, dwRate, 0);

            AddMissionItemInfo(wItemIndex, eMISSION_EXECUTE_KIND_ITEM_GET);			
		}
		else if( strcmp(buff,"#ITEM_TARGET_GET") == 0 )
		{
			WORD wItemIndex = pFile->GetWord();
			DWORD dwRate = pFile->GetDword();
			DWORD dwMonsterIDX = pFile->GetDword();

			pMission = new CInsDGMission(dwMissionGroupIDX, dwMissionIDX);
			pMission->CreateMissionEvent(eMISSION_EVENT_KIND_HUNT);			
			pMission->CreateMissionExecute(eMISSION_EXECUTE_KIND_ITEM_TARGET_GET, dwMissionIDX, wItemIndex, dwRate, dwMonsterIDX);

			AddMissionItemInfo(wItemIndex, eMISSION_EXECUTE_KIND_ITEM_TARGET_GET);
		}
		else if( strcmp(buff,"#NPC_TALK") == 0 )
		{
			DWORD dwNPCIndex = pFile->GetDword();
			
			pMission = new CInsDGMission(dwMissionGroupIDX, dwMissionIDX);
			pMission->CreateMissionEvent(eMISSION_EVENT_KIND_NPC);			
			pMission->CreateMissionExecute(eMISSION_EXECUTE_KIND_NPC_TALK, dwMissionIDX, dwNPCIndex, 0, 0);			
		}		
		else if( strcmp(buff,"#MAXCOUNT") == 0 )
		{
			DWORD dwMaxCount = pFile->GetDword();
			pMission->SetExecuteData(dwMaxCount);
		}
		else if( strcmp(buff,"#REQUITAL_ITEM") == 0 )
		{
			DWORD dwMaxNum = pFile->GetDword();			

			MISSION_REQUITAL_ITEM MissionRequitalItem;
			memset(&MissionRequitalItem, 0, sizeof(MissionRequitalItem));			
			
			for(int i = 0 ; i < dwMaxNum ; i++)
			{
				MissionRequitalItem.wItemIndex = pFile->GetWord();
				MissionRequitalItem.dwNum = pFile->GetDword();
				pMission->AddMissionRequitalItem(&MissionRequitalItem);
			}
		}
		else if( strcmp(buff,"#REQUITAL_MONEY") == 0 )
		{
			DWORD dwMoney = pFile->GetDword();
			MissionRequital.dwMoney = dwMoney;
		}
		else if( strcmp(buff,"#REQUITAL_EXP") == 0 )
		{
			DWORD dwExp = pFile->GetDword();			
			MissionRequital.dwExp = dwExp;
		}
		else if( strcmp(buff,"#REQUITAL_SEXP") == 0 )
		{
			DWORD dwSExp = pFile->GetDword();
			MissionRequital.dwSExp = dwSExp;
		}
		else if( strcmp(buff,"#REQUITAL_TIME") == 0 )
		{
			DWORD dwTime = pFile->GetDword();			
			MissionRequital.dwTime = dwTime;
		}
		else if( strcmp(buff,"#MONSTER_REGEN") == 0 )
		{
			DWORD dwGroupIndex = pFile->GetDword();
			pMission->SetMissionRegenGroupID(dwGroupIndex);
		}
		else if( strcmp(buff,"#NEXT_MISSION") == 0 )
		{
			DWORD dwNextMissionIndex = pFile->GetDword();
			pMission->SetNextMissionIDX(dwNextMissionIndex);
		}		
	}

	pMission->SetMissionRequital(&MissionRequital);

	return pMission;
}

CInsDGMission* CInsDungeonMissionDataManager::LoadMissionInfoData(CInsDGMission* pMission)
{
	DWORD dwMissionGroupIDX = pMission->GetMissionGroupIDX();
	DWORD dwMissionIDX = pMission->GetMissionIDX();
	DWORD dwNextMissionIDX = pMission->GetNextMissionIDX();
	DWORD dwMissionEventKind = pMission->GetMissionEventKind();
	DWORD dwMissionExecuteKind = pMission->GetMissionExecuteKind();
	DWORD dwMissionExecuteParam1 = pMission->GetExecuteParam1();
	DWORD dwMissionExecuteParam2 = pMission->GetExecuteParam2();
	DWORD dwMissionExecuteParam3 = pMission->GetExecuteParam3();
	DWORD dwMissionExecuteData = pMission->GetExecuteData();
	DWORD dwMissionRegenGroupID = pMission->GetMissionRegenGroupID();
	DWORD dwMissionRequitalItemMaxNum = pMission->GetMissionRequitalItemMaxNum();

	CInsDGMission* pInsDGMission = new CInsDGMission(dwMissionGroupIDX, dwMissionIDX);

	pInsDGMission->SetNextMissionIDX(dwNextMissionIDX);
	pInsDGMission->CreateMissionEvent(dwMissionEventKind);	
	pInsDGMission->CreateMissionExecute(dwMissionExecuteKind, dwMissionIDX, dwMissionExecuteParam1, dwMissionExecuteParam2, dwMissionExecuteParam3);	
	
	pInsDGMission->SetExecuteData(dwMissionExecuteData);
	pInsDGMission->SetMissionRegenGroupID(dwMissionRegenGroupID);
	pInsDGMission->SetMissionRequital(pMission->GetMissionRequital());
	for(int i = 0 ; i < dwMissionRequitalItemMaxNum ; i++)
	{
		MISSION_REQUITAL_ITEM* pMissionRequitalItem = pMission->GetMissionRequitalItem(i);
		if(pMissionRequitalItem == NULL)
			continue;

		pInsDGMission->AddMissionRequitalItem(pMissionRequitalItem);
	}

	return pInsDGMission;
}

BOOL CInsDungeonMissionDataManager::AddMissionItemInfo(WORD wItemIndex, DWORD dwMissionKind)
{
	ITEM_INFO* pItemInfo = ITEMMGR->GetItemInfo(wItemIndex);
	if(pItemInfo == NULL)
	{
		ASSERT(0);
		return FALSE;
	}

	INSDG_MISSION_ITEM_INFO* pMissionItemInfo = new INSDG_MISSION_ITEM_INFO;
	pMissionItemInfo->dwItemIndex = wItemIndex;
	pMissionItemInfo->dwItemMissionKind = dwMissionKind;
	m_InsDGMissionItemInfoTable.Add(pMissionItemInfo, wItemIndex);

	return TRUE;
}

INSDG_MISSION_ITEM_INFO* CInsDungeonMissionDataManager::GetMissionItemInfo(WORD wItemIndex)
{
	return m_InsDGMissionItemInfoTable.GetData(wItemIndex);
}

void CInsDungeonMissionDataManager::RemoveAllInsDGMissionItemInfo()
{
	INSDG_MISSION_ITEM_INFO* pInfo = NULL;
	
	m_InsDGMissionItemInfoTable.SetPositionHead();
	while (pInfo = m_InsDGMissionItemInfoTable.GetData())
	{
		SAFE_DELETE(pInfo);
	}
	m_InsDGMissionItemInfoTable.RemoveAll();
}

BOOL CInsDungeonMissionDataManager::LoadMissionRegen()
{
	CMHFile file;
	char filename[256];
#ifdef _FILE_BIN_	
	sprintf(filename,"Resource/Server/InsDGMissionRegen.bin");
	if(!file.Init(filename,"rb"))
		return FALSE;
#else
	sprintf(filename,"Resource/Server/InsDGMissionRegen.bin");
	if(!file.Init(filename,"rt"))
		return FALSE;
#endif

	char buff[256] = {0,};
	CInsDGMissionRegenGroup* pRegenGroup = NULL;
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

		if(strcmp(buff, "$GROUP") == 0)
		{
			DWORD dwGroupID = file.GetDword();
			pRegenGroup = new CInsDGMissionRegenGroup(dwGroupID);
			m_InsDGMissionRegenGroupTable.Add(pRegenGroup, dwGroupID);
		}
		else if(strcmp(buff, "#MAXOBJECT") == 0)
			file.GetDword();
		else if(strcmp(buff, "#ADD") == 0)
		{
			DWORD dwGroupID = pRegenGroup->GetGroupID();
			BYTE ObjectKind = file.GetByte();
			file.GetDword();
			WORD wMonsterIndex = file.GetWord();

			VECTOR3 vPos;
			memset(&vPos, 0, sizeof(vPos));
			vPos.x = file.GetFloat();
			vPos.y = 0;
			vPos.z = file.GetFloat();			

			CInsDGMissionRegenInfo* pRegenObjectInfo = new CInsDGMissionRegenInfo(dwGroupID, wMonsterIndex, ObjectKind, &vPos);
			pRegenGroup->AddMissionRegenInfo(pRegenObjectInfo);
		}
	}
	file.Release();

	return TRUE;
}

void CInsDungeonMissionDataManager::RemoveAllInsDGMissionRegenList()
{
	CInsDGMissionRegenGroup* pInsDGMissionRegenGroup = NULL;

	m_InsDGMissionRegenGroupTable.SetPositionHead();
	while(pInsDGMissionRegenGroup = m_InsDGMissionRegenGroupTable.GetData())
	{
		SAFE_DELETE(pInsDGMissionRegenGroup);
	}
	m_InsDGMissionRegenGroupTable.RemoveAll();
}

CInsDGMissionRegenGroup* CInsDungeonMissionDataManager::GetInsDGMissionRegenGroup(DWORD dwGroupID)
{
	return m_InsDGMissionRegenGroupTable.GetData(dwGroupID);
}