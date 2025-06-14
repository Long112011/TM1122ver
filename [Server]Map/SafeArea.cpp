#include "StdAfx.h"
#include "SafeArea.h"
#include "MHFile.h"

GLOBALTON(SafeAreaManager)

SafeAreaManager::SafeAreaManager()
{
	m_SafeAreaList.Initialize(100);
}

SafeAreaManager::~SafeAreaManager()
{
	SAFEAREAINFO * pInfo=NULL;

	m_SafeAreaList.SetPositionHead();

	while(pInfo = m_SafeAreaList.GetData())
	{
		delete pInfo;
	}
	m_SafeAreaList.RemoveAll();
}

void  SafeAreaManager::LoadSafeAreaInfo()
{
	CMHFile file;

	#ifdef _FILE_BIN_
		if(!file.Init("Resource/Safe_Area.bin", "rb"))
			return;
	#else
		if(!file.Init("Resource/Safe_Area.txt", "rt"))
			return;
	#endif	// _FILE_BIN_

		SAFEAREAINFO * pInfo = NULL;

		while(1)
		{
			if(file.IsEOF())
				break;

			ASSERT(!pInfo);

			pInfo = new SAFEAREAINFO;
            
			pInfo->MapNum=file.GetInt();

			pInfo->P1.x=file.GetDword();

			pInfo->P1.y=file.GetDword();

			pInfo->P2.x=file.GetDword();

			pInfo->P2.y=file.GetDword();

			pInfo->P3.x=file.GetDword();

			pInfo->P3.y=file.GetDword();

			pInfo->P4.x=file.GetDword();

			pInfo->P4.y=file.GetDword();

			m_SafeAreaList.Add(pInfo,pInfo->MapNum);

			pInfo=NULL;
		}

		file.Release();
}

SAFEAREAINFO * SafeAreaManager::GetSafeAreaInfo(int MapNum)
{
	return m_SafeAreaList.GetData(MapNum);
}