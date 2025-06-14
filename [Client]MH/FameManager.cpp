#include "StdAfx.h"
#include "FameManager.h"
#include "mhFile.h"
//#include "ObjectManager.h"
GLOBALTON(FameManager)
FameManager::FameManager()
{
	m_FameLogoList.Initialize(100);
	m_StageLogoList.Initialize(100);
}
FameManager::~FameManager()
{
	FAMEStruct * pStageLogo=NULL;
	m_FameLogoList.SetPositionHead();
	while(pStageLogo=m_FameLogoList.GetData())
	{
		delete pStageLogo;
	}
	m_FameLogoList.RemoveAll();

	STAGELOGO * pStageIcon=NULL;
	m_StageLogoList.SetPositionHead();
	while(pStageIcon=m_StageLogoList.GetData())
	{
		delete pStageIcon;
	}
	m_StageLogoList.RemoveAll();
}
void FameManager::LoadFameStage()
{
	CMHFile file;
	if(!file.Init("Resource/FameList.bin", "rb"))
		return;

	char line[256] = { 0, };
	FAMEStruct * pInfo = NULL;

	while (file.IsEOF() == FALSE)
	{
		file.GetString(line);
		if (line[0] == '@')
		{
			file.GetLineX(line, 256);
			continue;
		}
		if (strcmp(line, "#SUBSTAGE") == 0)
		{
			pInfo = new FAMEStruct;
			pInfo->Idx			= file.GetInt();
			pInfo->FameVal		= file.GetInt();
			SafeStrCpy(pInfo->FameName, file.GetStringInQuotation(), MAX_NAME_LENGTH + 1);
			pInfo->PowerSwitch	= file.GetLong();

			if (pInfo->PowerSwitch)
			{
				for (int i = 0; i<4; i++)
				{
					pInfo->Power[i] = file.GetLong();
				}
			}
			else
			{
				for (int i = 0; i<4; i++)
				{
					pInfo->Power[i] = 0;
				}
			}
			m_FameLogoList.Add(pInfo, pInfo->Idx);
		}		
	}
	file.Release();
}
FAMEStruct * FameManager::GetFameStage(int idx)
{
	return m_FameLogoList.GetData(idx);
}
char * FameManager::GetFameNameString(FAMETYPE pFame)
{
	int fMax=GetFameStage(0)->FameVal;
	for( WORD i = 0; i < fMax+1; ++i )
	{
		if( pFame < GetFameStage(i)->FameVal+1 )
			return GetFameStage(i)->FameName;

		else if(pFame >= GetFameStage(fMax)->FameVal)
			return GetFameStage(fMax)->FameName;
	}
}
std::string FameManager::GetFameNameStrTrainee()
{
	return GetFameStage(1)->FameName;
}
bool FameManager::LoadStageLogo()
{
	CMHFile file;
	if (!file.Init("Resource/FameList.bin", "rb"))
	{
		return FALSE;
	}
	STAGELOGO * pInfo;
	while( file.IsEOF() == FALSE )
	{
		pInfo = new STAGELOGO;
		pInfo->LogoIdx = file.GetInt();
		pInfo->FameVal = file.GetInt();
		m_StageLogoList.Add(pInfo, pInfo->LogoIdx);
		pInfo = NULL;
	}
	file.Release();

	FAMEMGR->LoadFameStage();

	return TRUE;
}
STAGELOGO * FameManager::GetStageLogoVal(int idx)
{
	return m_StageLogoList.GetData(idx);
}
DWORD FameManager::GetFameLogoChange(FAMETYPE pFame)
{
	for( WORD i = 0; i < 8; ++i )
	{
		if( pFame <= GetStageLogoVal(i)->FameVal )
		{
			if((GetStageLogoVal(i)->LogoIdx-1)<0)
				return 0;
			else
				return GetStageLogoVal(i)->LogoIdx-1;
		}
		else if(pFame > GetStageLogoVal(7)->FameVal)
			return 7;
	}	
}