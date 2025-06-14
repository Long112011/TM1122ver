#include "StdAfx.h"
#include "SafeArea.h"
#include "MHFile.h"
#include <MMSystem.h>
#include "ObjectManager.h"
#include "./Interface/cWindowManager.h" 
#include "./interface/cStatic.h"
#include "WindowIDEnum.h"
#include "MHMap.h"
#include "ChatManager.h"
#include "MoveManager.h"

int bYes;
GLOBALTON(SafeAreaManager)

SafeAreaManager::SafeAreaManager()
{
	m_SafeAreaTime=timeGetTime();

	m_SafeAreaList.Initialize(100);

	m_IsMsg=FALSE;  //weiye 默认不发送区域消息 2018-03-12

	bYes = 0;
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

	if (!file.Init("Resource/Safe_Area.bin", "rb"))
	{
		bYes = 0;
		return;
	}
	

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
	bYes = -1;
	file.Release();
}

SAFEAREAINFO * SafeAreaManager::GetSafeAreaInfo(int MapNum)
{
	return m_SafeAreaList.GetData(MapNum);
}

void SafeAreaManager::Process()
{
	DWORD CurTime= timeGetTime();

	if(CurTime-m_SafeAreaTime>1000)  //weiye 默认一秒检测一次
	{
	    cDialog * pDlg = WINDOWMGR->GetWindowForID( TOP_NUMBER_ICON );

		if(pDlg)
		{
			cStatic * pSafeAreaLabOn =(cStatic*)pDlg->GetWindowForID(TOP_SAFE_ON); 

			cStatic * pSafeAreaLabOff=(cStatic*)pDlg->GetWindowForID(TOP_SAFE_OFF);

			int ret = CheckSafeArea();

			if(ret==0)
			{
				pSafeAreaLabOn->SetActive(FALSE);
				pSafeAreaLabOff->SetActive(TRUE);
				if(!m_IsMsg)
				{
				   //CHATMGR->AddMsg( CTC_OPERATOR2, CHATMGR->GetChatMsg(2194));
				   m_IsMsg=TRUE;
				}
			}	
			if(ret==1)
			{
				pSafeAreaLabOn->SetActive(TRUE);

				pSafeAreaLabOff->SetActive(FALSE);

				if(m_IsMsg)
				{
				  // CHATMGR->AddMsg( CTC_OPERATOR3, CHATMGR->GetChatMsg(2193));
				   m_IsMsg=FALSE;
				}
			}
			if(ret==-1)
			{
				pSafeAreaLabOn->SetActive(FALSE);
				pSafeAreaLabOff->SetActive(FALSE);
			}
		}

		m_SafeAreaTime=timeGetTime();
	}
}
int  SafeAreaManager::CheckSafeArea() //weiye 2018-03-05 根据角色位置判断是否在安全区域 
{
	HERO->GetPosition(&Pos);

	SAFEAREAINFO * pSafeInfo = GetSafeAreaInfo(MAP->GetMapNum());

	if(!pSafeInfo)
	{
		return bYes;
	}

	int A = (pSafeInfo->P2.x - pSafeInfo->P1.x)*(Pos.z - pSafeInfo->P1.y) - (pSafeInfo->P2.y - pSafeInfo->P1.y)*(Pos.x - pSafeInfo->P1.x ); 

	int B = (pSafeInfo->P3.x - pSafeInfo->P2.x)*(Pos.z - pSafeInfo->P2.y) - (pSafeInfo->P3.y - pSafeInfo->P2.y)*(Pos.x - pSafeInfo->P2.x );  

	int C = (pSafeInfo->P4.x - pSafeInfo->P3.x)*(Pos.z - pSafeInfo->P3.y ) -(pSafeInfo->P4.y - pSafeInfo->P3.y)*(Pos.x - pSafeInfo->P3.x );  

	int D = (pSafeInfo->P1.x - pSafeInfo->P4.x)*(Pos.z - pSafeInfo->P4.y ) -(pSafeInfo->P1.y - pSafeInfo->P4.y)*(Pos.x - pSafeInfo->P4.x );  
  

    if((A > 0 && B > 0 && C > 0 && D > 0) || (A < 0 && B < 0 && C < 0 && D < 0)) 
	{  
        return 1;
	}
	else
	{
		return 0;
	}
}

int  SafeAreaManager::CheckSafeArea(VECTOR3 * pMovePos) //weiye 2018-03-05 根据角色位置判断是否在安全区域 
{

	SAFEAREAINFO * pSafeInfo = GetSafeAreaInfo(MAP->GetMapNum());

	if(!pSafeInfo)
	{
		return bYes;
	}

	int A = (pSafeInfo->P2.x - pSafeInfo->P1.x)*(pMovePos->z - pSafeInfo->P1.y) - (pSafeInfo->P2.y - pSafeInfo->P1.y)*(pMovePos->x - pSafeInfo->P1.x ); 

	int B = (pSafeInfo->P3.x - pSafeInfo->P2.x)*(pMovePos->z - pSafeInfo->P2.y) - (pSafeInfo->P3.y - pSafeInfo->P2.y)*(pMovePos->x- pSafeInfo->P2.x );  

	int C = (pSafeInfo->P4.x - pSafeInfo->P3.x)*(pMovePos->z - pSafeInfo->P3.y ) -(pSafeInfo->P4.y - pSafeInfo->P3.y)*(pMovePos->x - pSafeInfo->P3.x );  

	int D = (pSafeInfo->P1.x - pSafeInfo->P4.x)*(pMovePos->z - pSafeInfo->P4.y ) -(pSafeInfo->P1.y - pSafeInfo->P4.y)*(pMovePos->x - pSafeInfo->P4.x );  
  

    if((A > 0 && B > 0 && C > 0 && D > 0) || (A < 0 && B < 0 && C < 0 && D < 0)) 
	{  
        return 1;
	}
	else
	{
		return 0;
	}
}