#include "StdAfx.h"
#include "InsDGEntranceInfoDialog.h"
#include "WindowIDEnum.h"
#include "InsDungeonManager.h"
#include "InventoryExDialog.h"
#include "Item.h"
#include "GameIn.h"
#include "ObjectManager.h"
#include "MHMap.h"
#include "ObjectStateManager.h"
#include "ItemManager.h"
#include "QuestManager.h"
#include "../[CC]Quest/QuestString.h"
#include "ChatManager.h"
#include "cDialogueList.h"
#include "./interface/cListDialog.h"
#include "NpcScriptManager.h"
#include "InsDGPartyMakeDialog.h"
CInsDGEntranceInfoDialog::CInsDGEntranceInfoDialog()
{
}
CInsDGEntranceInfoDialog::~CInsDGEntranceInfoDialog()
{
}
void CInsDGEntranceInfoDialog::Linking()
{	
	m_pInsDGMsgList = (cListDialog*)GetWindowForID(INSDG_ENTRANCEINFO_LIST);
	m_pLevelStatic = (cStatic*)GetWindowForID(INSDG_ENTRANCEINFO_LEVEL);
	m_pQuestlStatic = (cStatic*)GetWindowForID(INSDG_ENTRANCEINFO_QUEST);
	m_pItemStatic = (cStatic*)GetWindowForID(INSDG_ENTRANCEINFO_ITEM);
	m_pMoneyStatic = (cStatic*)GetWindowForID(INSDG_ENTRANCEINFO_MONEY);		
	m_wNpcUniqueIDX = 0;
}
void CInsDGEntranceInfoDialog::SetActive( BOOL val )
{
	if(val == TRUE)
		InsDGEntranceInfo();
	else
		m_wNpcUniqueIDX = 0;
	cDialog::SetActive( val );	
}
void CInsDGEntranceInfoDialog::OnActionEvent( LONG lId, void * p, DWORD we )
{
	switch( lId )
	{		
	case INSDG_ENTRANCEINFO_CONNECT:	
		{			
			if(HERO->GetPartyIdx() != 0)
			{
				InsDGEntranceSyn();
				SetActive(FALSE);
			}
			else
			{
				CInsDGPartyMakeDialog* pInsDGPartyMakeDlg = GAMEIN->GetInsDGPartyMakeDlg();
				if(pInsDGPartyMakeDlg == NULL)
					return;
				pInsDGPartyMakeDlg->SetActive(TRUE);
				SetActive(FALSE);
			}
		}
		break;
	case INSDG_ENTRANCEINFO_CANCEL:	
		{			
			SetActive(FALSE);
			if( HERO->GetState() == eObjectState_Deal )
				OBJECTSTATEMGR->EndObjectState( HERO, eObjectState_Deal );			
		}
		break;
	case INSDG_ENTRANCEINFO_CLOSE:	
		{			
			SetActive(FALSE);
			if( HERO->GetState() == eObjectState_Deal )
				OBJECTSTATEMGR->EndObjectState( HERO, eObjectState_Deal );			
		}
		break;		
	}
}
void CInsDGEntranceInfoDialog::SetNpcUniqueIndex(WORD wNpcUniqueIDX)
{
	m_wNpcUniqueIDX = wNpcUniqueIDX;
}
void CInsDGEntranceInfoDialog::InsDGEntranceInfo()
{
	INSDG_ENTRANCECONDITION_INFO* pEntranceInfo = INSDUNGEONMGR->GetEntranceConditionInfo(m_wNpcUniqueIDX);
	if(pEntranceInfo == NULL)
		return;
	LEVELTYPE wMinLevel = pEntranceInfo->wMinLevel;
	LEVELTYPE wMaxLevel = pEntranceInfo->wMaxLevel;
	DWORD dwQuestIndex = pEntranceInfo->dwQuestIndex;	
	WORD wItemIndex = pEntranceInfo->wItemIndex;
	DWORD dwMoney = pEntranceInfo->dwMoney;
	DWORD dwMsgIndex = pEntranceInfo->dwMsgIndex;
	DWORD dwIndex = 0;
	DIALOGUE* temp = NULL;
	cDialogueList* pList = NPCSCRIPTMGR->GetDialogueList();
	if( pList == NULL ) return;
	m_pInsDGMsgList->RemoveAll();
	while(1)
	{
		temp = pList->GetDialogue( dwMsgIndex, dwIndex );
		if( temp == NULL ) 
			break;
		m_pInsDGMsgList->AddItem(temp->str, RGB(255, 255, 255));
		dwIndex++;
	}
	char szLevelStr[64];
	memset(szLevelStr, 0, sizeof(&szLevelStr));
	sprintf(szLevelStr, CHATMGR->GetChatMsg(2694), wMinLevel, wMaxLevel);
	m_pLevelStatic->SetStaticText(szLevelStr);
	char szQuestStr[128];
	memset(szQuestStr, 0, sizeof(&szQuestStr));
	DWORD dwKey = 0;
	COMBINEKEY(dwQuestIndex, 0, dwKey);
	CQuestString* pQuestString = QUESTMGR->GetQuestString(dwKey);
	if(pQuestString == NULL)
	{
		sprintf(szQuestStr, CHATMGR->GetChatMsg(2695), CHATMGR->GetChatMsg(249));		
	}
	else
	{
		cPtrList* pQuestTitle = pQuestString->GetTitle();
		QString* pQuestString = (QString*)pQuestTitle->GetHead();
		sprintf(szQuestStr, CHATMGR->GetChatMsg(2695), pQuestString->Str);		
	}
	m_pQuestlStatic->SetStaticText(szQuestStr);
	char szItemStr[64];
	memset(szItemStr, 0, sizeof(&szItemStr));
	if(wItemIndex != 0)
	{
		ITEM_INFO* pItemInfo = ITEMMGR->GetItemInfo(wItemIndex);
		if(pItemInfo != NULL)
		{			
			sprintf(szItemStr, CHATMGR->GetChatMsg(2696), pItemInfo->ItemName);
		}
	}
	else
	{
		sprintf(szItemStr, CHATMGR->GetChatMsg(2696), CHATMGR->GetChatMsg(249));
	}
	m_pItemStatic->SetStaticText(szItemStr);
	char szMoneyStr[64];
	memset(szMoneyStr, 0, sizeof(&szMoneyStr));
	if(dwMoney != 0)
		sprintf(szMoneyStr, CHATMGR->GetChatMsg(2697), dwMoney);
	else
		sprintf(szMoneyStr, CHATMGR->GetChatMsg(2697), 0);	
	m_pMoneyStatic->SetStaticText(szMoneyStr);
}
void CInsDGEntranceInfoDialog::InsDGEntranceSyn()
{
	eINSDG_ENTRANCE_ERROR eInsDG_Error = INSDUNGEONMGR->CheckEntranceCondition(m_wNpcUniqueIDX);
	if( eInsDG_Error != eINSDG_ENTRANCE_ERROR_NONE )	
	{
		INSDUNGEONMGR->EntranceErrorMsg(eInsDG_Error);								
	}
	else	
	{
		INSDG_ENTRANCECONDITION_INFO* pEntranceInfo = INSDUNGEONMGR->GetEntranceConditionInfo(m_wNpcUniqueIDX);
		if(pEntranceInfo == NULL)
		{
			INSDUNGEONMGR->EntranceErrorMsg(eINSDG_ENTRANCE_ERROR_NULL);
			if( HERO->GetState() == eObjectState_Deal )
				OBJECTSTATEMGR->EndObjectState( HERO, eObjectState_Deal );
			return;
		}
		MSG_DWORD_WORD3 msg;
		msg.Category = MP_INSDUNGEON;
		msg.Protocol = MP_INSDUNGEON_ENTRANCE_SYN;
		msg.dwObjectID = HEROID;
		msg.dwData = m_wNpcUniqueIDX;
		msg.wData1 = pEntranceInfo->wItemIndex;
		if(pEntranceInfo->wItemIndex != 0)
		{
			CInventoryExDialog* pInvenDlg = GAMEIN->GetInventoryDialog();
			if(pInvenDlg == NULL)
			{
				INSDUNGEONMGR->EntranceErrorMsg(eINSDG_ENTRANCE_ERROR_NULL);
				if( HERO->GetState() == eObjectState_Deal )
					OBJECTSTATEMGR->EndObjectState( HERO, eObjectState_Deal );
				return;
			}
			CItem* pItem = pInvenDlg->GetItemLike(pEntranceInfo->wItemIndex);
			if(pItem == NULL)
			{
				INSDUNGEONMGR->EntranceErrorMsg(eINSDG_ENTRANCE_ERROR_ITEM);
				if( HERO->GetState() == eObjectState_Deal )
					OBJECTSTATEMGR->EndObjectState( HERO, eObjectState_Deal );
				return;									
			}
			msg.wData2 = pItem->GetPosition();
		}
		else
			msg.wData2 = 0;
		MAPCHANGE_INFO* pMCInfo = GAMERESRCMNGR->GetMapChangeInfo(m_wNpcUniqueIDX);;
		if(pMCInfo == NULL)
			return;		
		msg.wData3 = pMCInfo->MoveMapNum;
		NETWORK->Send( &msg, sizeof(msg) );
	}
	if( HERO->GetState() == eObjectState_Deal )
		OBJECTSTATEMGR->EndObjectState( HERO, eObjectState_Deal );	
}
