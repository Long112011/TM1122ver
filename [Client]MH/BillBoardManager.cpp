#include "stdafx.h"
#include "BillBoardManager.h"
#include "CommonStruct.h"
#include "ObjectManager.h"
#include "GameIn.h"
#include "TopDialog.h"
#include "ChatManager.h"
BillBoardManager::BillBoardManager(void)
{
	memset(&pHeroData,0,sizeof(SEND_TOPLIST));
}
BillBoardManager::~BillBoardManager(void)
{
	memset(&pHeroData,0,sizeof(SEND_TOPLIST));
}
void BillBoardManager::SetTopListInfo(SEND_TOPLIST * pListInfo)
{
    memset(&pHeroData,0,sizeof(SEND_TOPLIST));
	memcpy(&pHeroData,pListInfo,sizeof(SEND_TOPLIST)); 
}
void BillBoardManager::SetHeroTopListFlg(CPlayer* pPlayer)
{
	if(!pPlayer) return;
	pPlayer->InitTopListFlg();
	if (pHeroData.TopInfo[0].ObjectId == pPlayer->GetID())
	{
		pPlayer->SetTopListFlg(0);
		//return;
	}
	else if (pHeroData.TopInfo[1].ObjectId == pPlayer->GetID())
	{
		pPlayer->SetTopListFlg(1);
		//return;
	}
	else if (pHeroData.TopInfo[2].ObjectId == pPlayer->GetID())
	{
		pPlayer->SetTopListFlg(2);
		//return;
	}
	else if (pHeroData.TopInfo[3].ObjectId == pPlayer->GetID())
	{
		pPlayer->SetTopListFlg(3);
		//return;
	}
	else if (pHeroData.TopInfo[4].ObjectId == pPlayer->GetID())
	{
		pPlayer->SetTopListFlg(4);
		//return;
	}
	else if (pHeroData.TopInfo[5].ObjectId == pPlayer->GetID())
	{
		pPlayer->SetTopListFlg(5);
		//return;
	}
	else if (pHeroData.TopInfo[6].ObjectId == pPlayer->GetID())
	{
		pPlayer->SetTopListFlg(6);
		//return;
	}
	else if (pHeroData.TopInfo[7].ObjectId == pPlayer->GetID())
	{
		pPlayer->SetTopListFlg(7);
		//return;
	}
}
/*void BillBoardManager::SetHeroTopListGameIn(DWORD Id)
{
	//if(!pPlayer) return;
	//pPlayer->InitTopListFlg();
	CObject* pObj = OBJECTMGR->GetObject(Id);
		CPlayer* ppla = (CPlayer*)pObj;

	if (pHeroData.TopInfo[0].ObjectId == Id)
	{
		pObj->SetTopListFlg(0);
	}
	else if (pHeroData.TopInfo[1].ObjectId == Id)
	{
		pObj->SetTopListFlg(1);
	}
	else if (pHeroData.TopInfo[2].ObjectId == Id)
	{
		pObj->SetTopListFlg(2);
	}
	else if (pHeroData.TopInfo[3].ObjectId == Id)
	{
		pObj->SetTopListFlg(3);
	}
	else if (pHeroData.TopInfo[4].ObjectId == Id)
	{
		pObj->SetTopListFlg(4);
	}
	else if (pHeroData.TopInfo[5].ObjectId == Id)
	{
		pObj->SetTopListFlg(5);
	}
	else if (pHeroData.TopInfo[6].ObjectId == Id)
	{
		pObj->SetTopListFlg(6);
	}
	else if (pHeroData.TopInfo[7].ObjectId == Id)
	{
		pObj->SetTopListFlg(7);
	}
}*/