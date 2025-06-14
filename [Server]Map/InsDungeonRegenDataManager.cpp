#include "stdafx.h"
#include "InsDungeonRegenDataManager.h"
#include "InsDungeonRegenGroup.h"
#include "..\[CC]Header\GameResourceManager.h"
#include "MHFile.h"
#include "InsDungeonRegenConditionInfo.h"
#include "InsDungeonRegenObjectInfo.h"

GLOBALTON(CInsDungeonRegenDataManager);

CInsDungeonRegenDataManager::CInsDungeonRegenDataManager()
{
	m_InsDGRegenGroupTable.Initialize(100);
}

CInsDungeonRegenDataManager::~CInsDungeonRegenDataManager()
{
	RemoveAllInsDGRegenGroup();
}

BOOL CInsDungeonRegenDataManager::LoadInsDungeonRegenList()
{
	CMHFile file;
	char filename[256];
#ifdef _FILE_BIN_	
	sprintf(filename,"Resource/Server/InsDGMonster_%02d.bin",GAMERESRCMNGR->GetLoadMapNum());
	if(!file.Init(filename,"rb"))
		return FALSE;
#else
	sprintf(filename,"Resource/Server/InsDGMonster_%02d.txt",GAMERESRCMNGR->GetLoadMapNum());
	if(!file.Init(filename,"rt"))
		return FALSE;
#endif

	CInsDungeonRegenGroup* pRegenGroup = NULL;

	char buff[256] = {0,};
	while(1)
	{
		if(file.IsEOF())		
			break;
		
		file.GetString(buff);
		if(buff[0] == '}')
			continue;
		
		if(buff[0] == '{')
			continue;
		
		if(buff[0] == '@')
		{
			file.GetLineX(buff, 256);
			continue;
		}
		strcpy(buff, _strupr(buff));

		if(strcmp(buff, "$GROUP") == 0)
		{
			DWORD m_dwGroupID = file.GetDword();
			pRegenGroup = new CInsDungeonRegenGroup;
			pRegenGroup->SetGroupID(m_dwGroupID);
			m_InsDGRegenGroupTable.Add(pRegenGroup, m_dwGroupID);
		}
		else if(strcmp(buff, "#MAXOBJECT") == 0)
			file.GetDword();
		else if(strcmp(buff, "#PROPERTY") == 0)		//리젠 타입 기존의 안쓰는 데이터 활용
		{
			DWORD dwRegenType = file.GetDword();
			pRegenGroup->SetRegenType(dwRegenType);
		}
		else if(strcmp(buff, "#GROUPNAME") == 0)
			file.GetString();
		else if(strcmp(buff, "#ADDCONDITION") == 0)
		{
			DWORD dwGroupID				= file.GetDword();
			float fRatio				= file.GetFloat();
			DWORD dwRegenDelayTime		= file.GetDword();
			BOOL bRegen					= file.GetBool();			

			CInsDungeonRegenConditionInfo* pConditionInfo = new CInsDungeonRegenConditionInfo;
			pConditionInfo->SetGroupID(dwGroupID);
			pConditionInfo->SetRatio(fRatio);
			pConditionInfo->SetDelayTime(dwRegenDelayTime);
			pConditionInfo->SetRegen(bRegen);	
			
			pRegenGroup->SetRegenConditionInfo(pConditionInfo);
		}
		else if(strcmp(buff, "#ADD") == 0)
		{
			DWORD dwGroupID = pRegenGroup->GetGroupID();
			BYTE ObjectKind = file.GetByte();
			file.GetDword();
			WORD wMonsterKind = file.GetWord();

			VECTOR3 vPos;
			memset(&vPos, 0, sizeof(vPos));
			vPos.x = file.GetFloat();
			vPos.y = 0;
			vPos.z = file.GetFloat();

			BOOL bRegen = file.GetBool();

			CInsDungeonRegenObjectInfo* pRegenObjectInfo = new CInsDungeonRegenObjectInfo;
			pRegenObjectInfo->SetGroupID(dwGroupID);
			pRegenObjectInfo->SetObjectKind(ObjectKind);
			pRegenObjectInfo->SetMonsterKind(wMonsterKind);
			pRegenObjectInfo->SetPosition(&vPos);
			pRegenObjectInfo->SetRegen(bRegen);
			//pRegenObjectInfo->SetKey(dwKey);
			pRegenGroup->AddInsDGRegenObjectInfo(pRegenObjectInfo);
		}
	}
	file.Release();

	return TRUE;
}

void CInsDungeonRegenDataManager::RemoveAllInsDGRegenGroup()
{
	CInsDungeonRegenGroup* pRegenGroup = NULL;

	m_InsDGRegenGroupTable.SetPositionHead();
	while (pRegenGroup = m_InsDGRegenGroupTable.GetData())
	{
		SAFE_DELETE(pRegenGroup);
	}
	m_InsDGRegenGroupTable.RemoveAll();
}