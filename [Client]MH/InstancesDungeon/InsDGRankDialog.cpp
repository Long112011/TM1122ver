#include "StdAfx.h"
#include "InsDGRankDialog.h"
#include "WindowIDEnum.h"
#include "./interface/cStatic.h"
#include "./interface/cImage.h"
#include "ItemManager.h"
#include "cScriptManager.h"
#include "InsDungeonManager.h"
CInsDGRankDialog::CInsDGRankDialog()
{
	m_pRankResultImage = NULL;
	m_pParty = NULL;
	m_pClearTime = NULL;
	m_pHunt = NULL;
	m_pExp = NULL;
	m_pAbillity = NULL;
	m_pMoney = NULL;
	m_pItem = NULL;	
}
CInsDGRankDialog::~CInsDGRankDialog()
{
}
void CInsDGRankDialog::Linking()
{
	m_pRankResultImage =  (cStatic*)GetWindowForID(INSDG_RANK_RESULT_IMAGE);	
	m_pParty =  (cStatic*)GetWindowForID(INSDG_RANK_PARTY);	
	m_pClearTime =  (cStatic*)GetWindowForID(INSDG_RANK_CLEARTIME);	
	m_pHunt =  (cStatic*)GetWindowForID(INSDG_RANK_HUNT);	
	m_pExp =  (cStatic*)GetWindowForID(INSDG_RANK_EXP);	
	m_pAbillity =  (cStatic*)GetWindowForID(INSDG_RANK_ABILLITY);	
	m_pMoney =  (cStatic*)GetWindowForID(INSDG_RANK_MONEY);	
	m_pItem =  (cStatic*)GetWindowForID(INSDG_RANK_ITEM);
}
void CInsDGRankDialog::OnActionEvent( LONG lId, void * p, DWORD we )
{	
	switch( lId )
	{		
	case INSDG_RANK_OK:
		{			
			this->SetActive(FALSE);
		}
		break;
	}
}
void CInsDGRankDialog::SetRankInfo(MSG_INSDG_RANK_INFO* pRankInfo)
{
	m_pParty->SetStaticText("");
	m_pClearTime->SetStaticText("");
	m_pHunt->SetStaticText("");
	m_pExp->SetStaticText("");
	m_pAbillity->SetStaticText("");
	m_pMoney->SetStaticText("");
	m_pItem->SetStaticText("");	
	cImage RankImage;
	GetInsDGRankImage(pRankInfo->dwRankValue, &RankImage);
	m_pRankResultImage->SetBasicImage(&RankImage);
	char szPartyStr[64];
	memset(szPartyStr, 0, sizeof(&szPartyStr));
	itoa(pRankInfo->dwPartyNum, szPartyStr, 10);
	m_pParty->SetStaticText(szPartyStr);
	char szClearTimeStr[64];
	memset(szClearTimeStr, 0, sizeof(&szClearTimeStr));
	itoa(pRankInfo->dwClearTime, szClearTimeStr, 10);
	DWORD dwTime = pRankInfo->dwClearTime / 1000;
	sprintf(szClearTimeStr, "%02d minutes %02d seconds", dwTime / 60, dwTime % 60);
	m_pClearTime->SetStaticText(szClearTimeStr);
	char szHuntStr[64];
	memset(szHuntStr, 0, sizeof(&szHuntStr));
	itoa(pRankInfo->dwMonsterKillNum, szHuntStr, 10);
	m_pHunt->SetStaticText(szHuntStr);
	char szExpStr[64];
	memset(szExpStr, 0, sizeof(&szExpStr));
	itoa(pRankInfo->dwExp, szExpStr, 10);
	m_pExp->SetStaticText(szExpStr);		
	char szAbillityStr[64];
	memset(szAbillityStr, 0, sizeof(&szAbillityStr));
	itoa(pRankInfo->dwAbillity, szAbillityStr, 10);
	m_pAbillity->SetStaticText(szAbillityStr);		
	char szMoneyStr[64];
	memset(szMoneyStr, 0, sizeof(&szMoneyStr));
	itoa(pRankInfo->dwMoney, szMoneyStr, 10);
	m_pMoney->SetStaticText(szMoneyStr);		
	char szItemStr[64];
	memset(szItemStr, 0, sizeof(&szItemStr));
	if(pRankInfo->ResultalItemInfo.wItemIndex != 0)
	{
		ITEM_INFO* pItemInfo = ITEMMGR->GetItemInfo(pRankInfo->ResultalItemInfo.wItemIndex);
		if(pItemInfo != NULL)
		{			
			sprintf(szItemStr, "%s   %d", pItemInfo->ItemName, pRankInfo->ResultalItemInfo.dwItemNum);
			m_pItem->SetStaticText(szItemStr);
		}		
	}	
}
void CInsDGRankDialog::GetInsDGRankImage(DWORD dwRankValue, cImage* pImage)
{
	DWORD dwRank = INSDUNGEONMGR->GetRank(dwRankValue);	
	switch(dwRank)
	{
	case eINSDG_RANK_S:
	SCRIPTMGR->GetImage(166, pImage, PFT_JACKPATH);
		break;
	case eINSDG_RANK_A:
	SCRIPTMGR->GetImage(165, pImage, PFT_JACKPATH);
		break;
	case eINSDG_RANK_B:
	SCRIPTMGR->GetImage(164, pImage, PFT_JACKPATH);
		break;
	case eINSDG_RANK_C:
	SCRIPTMGR->GetImage(163, pImage, PFT_JACKPATH);
		break;
	case eINSDG_RANK_F:
	SCRIPTMGR->GetImage(162, pImage, PFT_JACKPATH);
		break;
	}	
}