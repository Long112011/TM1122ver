#include "StdAfx.h"
#include "InsDGMissionExecute_Item.h"
#include "PartyManager.h"
#include "Party.h"
#include "ItemManager.h"
#include "UserTable.h"

CInsDGMissionExecute_Item::CInsDGMissionExecute_Item(DWORD dwExecuteKind, DWORD dwMissionIDX, DWORD dwParam1, DWORD dwParam2, DWORD dwParam3)
	: CInsDGMissionExecute(dwExecuteKind, dwMissionIDX, dwParam1, dwParam2, dwParam3)
{
	m_wItemIndex = dwParam1;
	m_dwParam2 = dwParam2;
	m_dwParam3 = dwParam3;
	m_dwCurCount = 0;
}

CInsDGMissionExecute_Item::~CInsDGMissionExecute_Item()
{

}

void CInsDGMissionExecute_Item::Execute(MISSION_EVENT_INFO* pMissionEvent)
{
	//아이탬 미션의 경우 아이탬은 몬스터 사냥시 드랍하기 때문에 여기서
	//몬스터 사망시 여기서 드랍
	if(pMissionEvent->dwMissionEventKind == eMISSION_EVENT_KIND_HUNT)
	{
		//아이탬 드랍
		switch(m_dwMissionExecuteKind)
		{
		case eMISSION_EXECUTE_KIND_ITEM_USE:
			{
				//아이탬 드랍
				if(RandomDropItem() == TRUE)
				{
					DropMissionItem(pMissionEvent->pPlayer);
				}
			}
			break;
		case eMISSION_EXECUTE_KIND_ITEM_GET:
			{
				//아이탬 드랍
				if(RandomDropItem() == TRUE)
				{
					DropMissionItem(pMissionEvent->pPlayer);
					m_dwCurCount++;
					UpdateMission(pMissionEvent);
				}

				//완료되었나 체크
				if(m_dwMaxCount <= m_dwCurCount)
					m_bSuccessFlag = TRUE;
			}		
			break;
		case eMISSION_EXECUTE_KIND_ITEM_TARGET_GET:
			{
				if(m_dwParam3 != pMissionEvent->dwExecuteData)
					return;
				
				//아이탬 드랍
				if(RandomDropItem() == TRUE)
				{
					DropMissionItem(pMissionEvent->pPlayer);
					m_dwCurCount++;
					UpdateMission(pMissionEvent);
				}

				//완료되었나 체크
				if(m_dwMaxCount <= m_dwCurCount)
					m_bSuccessFlag = TRUE;
			}
			break;
		}
	}
	else if(pMissionEvent->dwMissionEventKind == eMISSION_EVENT_KIND_ITEM)
	{
		switch(m_dwMissionExecuteKind)
		{
		case eMISSION_EXECUTE_KIND_ITEM_USE:
			{
				if(m_wItemIndex == pMissionEvent->dwExecuteData)
				{
					m_dwCurCount++;
					UpdateMission(pMissionEvent);
				}

				//완료되었나 체크
				if(m_dwMaxCount <= m_dwCurCount)
					m_bSuccessFlag = TRUE;				
			}
			break;
		}
	}
}

BOOL CInsDGMissionExecute_Item::RandomDropItem()
{
	DWORD dwRandRate = rand()%10001;

	//예외처리
	if(m_dwParam2 > 10000)
		m_dwParam2 = 10000;

	if(dwRandRate > 10000 - m_dwParam2)
	{
		return TRUE;
	}

	return FALSE;
}

void CInsDGMissionExecute_Item::UpdateMission(MISSION_EVENT_INFO* pMissionEvent)
{
	//미션 카운터도 갱신
	CPlayer* pEventPlayer = pMissionEvent->pPlayer;
	DWORD dwPartyIDX = pEventPlayer->GetPartyIdx();
	CParty* pParty = PARTYMGR->GetParty(dwPartyIDX);
	if(pParty == NULL)
		return;

	MSG_INSDG_MISSION_UPDATE msg;
	msg.Category = MP_INSDUNGEON;
	msg.Protocol = MP_INSDUNGEON_MISSION_UPDATE;
	msg.dwCount = m_dwCurCount;

	CPlayer* pPlayer = NULL;
	DWORD dwPlayerID = 0;
	for(int n=0;n<MAX_PARTY_LISTNUM;++n)
	{
		dwPlayerID = pParty->GetMemberID(n);
		if(dwPlayerID == 0)
			continue;

		// network send
		pPlayer = (CPlayer*)g_pUserTable->FindUser( dwPlayerID );
		if( pPlayer == NULL )
			continue;

		msg.dwObjectID = dwPlayerID;
		pPlayer->SendMsg(&msg, sizeof(msg));
	}
}

void CInsDGMissionExecute_Item::DropMissionItem(CPlayer* pPlayer)
{
	WORD EmptyCellPos[255];
	WORD EmptyCellNum;
	CItemSlot * pSlot = pPlayer->GetSlot(eItemTable_Inventory);
	WORD obtainItemNum = ITEMMGR->GetCanBuyNumInSpace( pPlayer, pSlot, m_wItemIndex, 1, EmptyCellPos, EmptyCellNum );
	if(obtainItemNum == 0)
		return;
	
	int rt = ITEMMGR->ObtainItemEx(pPlayer, ITEMMGR->Alloc(pPlayer, MP_ITEM, MP_ITEM_MONSTER_OBTAIN_NOTIFY, pPlayer->GetID(), 0, eLog_ItemObtainMonster, obtainItemNum, (DBResult)(ITEMMGR->ObtainItemDBResult), NULL), m_wItemIndex, obtainItemNum, EmptyCellPos, EmptyCellNum, EmptyCellNum, 0);

};
/*
void CInsDGMissionExecute_Item::DeleteMissionItem(CPlayer* pEventPlayer)
{
	DWORD dwPartyIDX = pEventPlayer->GetPartyIdx();
	CParty* pParty = PARTYMGR->GetParty(dwPartyIDX);
	if(pParty == NULL)
		return;

	CPlayer* pPlayer = NULL;
	DWORD dwPlayerID = 0;
	for(int n=0;n<MAX_PARTY_LISTNUM;++n)
	{
		dwPlayerID = pParty->GetMemberID(n);
		if(dwPlayerID == 0)
			continue;

		// network send
		pPlayer = (CPlayer*)g_pUserTable->FindUser( dwPlayerID );
		if( pPlayer == NULL )
			continue;

		CItemSlot * pSlot = pPlayer->GetSlot(eItemTable_Inventory);

		POSTYPE startPos = pSlot->GetStartPos();
		POSTYPE EndPos = startPos +  pSlot->GetSlotNum();	

		for(int i = startPos ; i < EndPos ; ++i )
		{
			const ITEMBASE* pItemBase = pSlot->GetItemInfoAbs(i);
			if(pItemBase == NULL)
				continue;

			ITEM_INFO* pItemInfo = ITEMMGR->GetItemInfo(pItemBase->wIconIdx);
			if(pItemInfo == NULL)
				continue;

			if( m_wItemIndex == pItemBase->wIconIdx )
			{
				if( EI_TRUE != ITEMMGR->DiscardItem(pPlayer, pItemBase->Position, pItemBase->wIconIdx, pItemBase->Durability) )
					ASSERT(0);
			}
		}
	}	
}
*/