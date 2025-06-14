#include "StdAfx.h"
#include "InsDGMissionExecute_Hunt.h"
#include "Player.h"
#include "PartyManager.h"
#include "Party.h"
#include "UserTable.h"

CInsDGMissionExecute_Hunt::CInsDGMissionExecute_Hunt(DWORD dwExecuteKind, DWORD dwMissionIDX, DWORD dwParam1, DWORD dwParam2, DWORD dwParam3)
	: CInsDGMissionExecute(dwExecuteKind, dwMissionIDX, dwParam1, dwParam2, dwParam3)
{	
	m_dwMonsterIndex = dwParam1;	
	m_dwParam2 = dwParam2;
	m_dwParam3 = dwParam3;
	m_dwCurCount = 0;	
}

CInsDGMissionExecute_Hunt::~CInsDGMissionExecute_Hunt()
{

}

void CInsDGMissionExecute_Hunt::Execute(MISSION_EVENT_INFO* pMissionEvent)
{	
	switch(m_dwMissionExecuteKind)
	{
	case eMISSION_EXECUTE_KIND_MONSTER_TARGETKILL:
		{
			if(m_dwMonsterIndex == pMissionEvent->dwExecuteData)
			{
				m_dwCurCount++;	
				UpdateMission(pMissionEvent);
			}

			//완료되었나 체크
			if(m_dwMaxCount <= m_dwCurCount)
				m_bSuccessFlag = TRUE;
		}		
		break;
	case eMISSION_EXECUTE_KIND_MONSTER_KILL:
		{
			m_dwCurCount++;	
			UpdateMission(pMissionEvent);
			
			//완료되었나 체크
			if(m_dwMaxCount <= m_dwCurCount)
				m_bSuccessFlag = TRUE;
		}		
		break;
	case eMISSION_EXECUTE_KIND_MONSTER_KILLALL:
		{
			m_dwCurCount++;	//여기선 안쓴다. 데이터 확인을 위해서 
			UpdateMission(pMissionEvent);			

			//완료되었나 체크
			if(pMissionEvent->dwData == 0)
				m_bSuccessFlag = TRUE;
		}
		break;	
	}
}

void CInsDGMissionExecute_Hunt::UpdateMission(MISSION_EVENT_INFO* pMissionEvent)
{
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