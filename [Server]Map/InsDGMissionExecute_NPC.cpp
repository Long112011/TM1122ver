#include "StdAfx.h"
#include "InsDGMissionExecute_NPC.h"
#include "Player.h"
#include "PartyManager.h"
#include "Party.h"
#include "UserTable.h"
#include "RecallManager.h"

CInsDGMissionExecute_NPC::CInsDGMissionExecute_NPC(DWORD dwExecuteKind, DWORD dwMissionIDX, DWORD dwParam1, DWORD dwParam2, DWORD dwParam3)
: CInsDGMissionExecute(dwExecuteKind, dwMissionIDX, dwParam1, dwParam2, dwParam3)
{	
	m_dwNPCIndex = dwParam1;	
	m_dwParam2 = dwParam2;
	m_dwParam3 = dwParam3;
	m_dwCurCount = 0;	
}

CInsDGMissionExecute_NPC::~CInsDGMissionExecute_NPC()
{

}

void CInsDGMissionExecute_NPC::Execute(MISSION_EVENT_INFO* pMissionEvent)
{	
	if(pMissionEvent->dwExecuteData != m_dwNPCIndex)
		return;

	switch(m_dwMissionExecuteKind)
	{
	case eMISSION_EXECUTE_KIND_NPC_TALK:
		{
			UpdateMission(pMissionEvent);
			m_bSuccessFlag = TRUE;
		}		
		break;		
	}
}

void CInsDGMissionExecute_NPC::UpdateMission(MISSION_EVENT_INFO* pMissionEvent)
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