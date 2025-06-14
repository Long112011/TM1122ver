#include "stdafx.h"
#include "UserTable.h"
#include "Network.h"
#include "AgentDBMsgParser.h"

#ifdef _AGENTSERVER
#include "TrafficLog.h"
#include "GMPowerList.h"
#endif

#ifdef _NPROTECT_
#include "ggsrv25.h"
#endif

#include "PunishManager.h"

CUserTable * g_pUserTable = NULL;
CUserTable * g_pUserTableForUserID = NULL;
CUserTable * g_pUserTableForObjectID = NULL;

extern int g_nServerSetNum;

CUserTable::CUserTable()
{
}

CUserTable::~CUserTable()
{
}

void CUserTable::Init(DWORD dwBucket,USERDisconnectProc pProc)
{
	CYHHashTable<USERINFO>::Initialize(dwBucket);
	m_MaxUserCount = 0;
	m_dwUserCount = 0;
	
	m_addCount = 0;
	m_removeCount = 0;

	m_pOnDisconnectProc	=	pProc;
}
USERINFO* CUserTable::FindUser(DWORD dwKey)
{
	return CYHHashTable<USERINFO>::GetData(dwKey);
}

BOOL CUserTable::AddUser(USERINFO* pObject,DWORD dwKey)
{
	//ASSERT(!FindUser(dwKey));
	++m_dwUserCount;

	++ m_addCount;

	return CYHHashTable<USERINFO>::Add(pObject,dwKey);	
}

USERINFO * CUserTable::RemoveUser(DWORD dwKey)
{
	USERINFO * info = NULL;
	info = (USERINFO *)GetData(dwKey);

	if( !info )
		return NULL;

	if( m_pOnDisconnectProc)
			m_pOnDisconnectProc( info);

	Remove(dwKey);
	--m_dwUserCount;

	++m_removeCount;

	return info;
}

void CUserTable::RemoveAllUser()
{
//	SetPositionHead();
//	USERINFO * info = NULL;
//	while(info = (USERINFO *)GetData())
//	{
//		Remove(info->dwConnectionIndex);
//		info = NULL;
//	}
	m_dwUserCount = 0;
	CYHHashTable<USERINFO>::RemoveAll();
}

void CUserTable::SetCalcMaxCount(DWORD CurCount)
{
	if(m_MaxUserCount < CurCount)
	{
		m_MaxUserCount = CurCount;
	}
}

BOOL CUserTable::SendToUser(DWORD dwKey,DWORD dwUniqueConnectIdx,MSGBASE* pmsg,DWORD size)
{
	USERINFO* pUserInfo = FindUser(dwKey);
	if(pUserInfo == NULL)
		return FALSE;
	if(pUserInfo->dwUniqueConnectIdx != dwUniqueConnectIdx)
		return FALSE;

	g_Network.Send2User(pUserInfo->dwConnectionIndex,(char*)pmsg,size);
	return TRUE;
}


extern CMemoryPoolTempl<USERINFO> g_UserInfoPool;
#ifdef _NPROTECT_

extern CMemoryPoolTempl<CCSAuth2> g_CCSAuth2Pool;
#endif

BOOL CUserTable::OnDisconnectUser(DWORD dwKey)
{
	USERINFO* pInfo = g_pUserTable->FindUser( dwKey );
	if(pInfo == NULL) return FALSE;

	//---KES Network Process Fix 071114
	g_pUserTable->RemoveUser( dwKey );
	//----------------------------

	if( pInfo->UserLevel == eUSERLEVEL_GM )
	{
		 GMINFO->RemoveGMList( dwKey );
	}


//update Billing
	if(pInfo->dwCharacterID)
	{
		g_pUserTableForObjectID->RemoveUser(pInfo->dwCharacterID);

		SERVERINFO* pServerInfo = g_pServerTable->FindServerForConnectionIndex(pInfo->dwMapServerConnectionIndex);
		if( pServerInfo )
		{
			// �α��� Character_ID���� �ʱ�ȭ
			MSG_DWORD msg;
			msg.Category	= MP_USERCONN;
			msg.Protocol	= MP_USERCONN_DISCONNECTED;
			msg.dwObjectID	= pInfo->dwCharacterID;
			msg.dwData		= pInfo->dwUserID;
			g_Network.Send2Server(pInfo->dwMapServerConnectionIndex, (char*)&msg, sizeof(msg));
		}
		else
		{
			if(pInfo->dwUserID)
				LoginCheckDelete(pInfo->dwUserID);		// LOGOUT�ð��� ������Ʈ �մϴ�.
		}

		// ChangeMapPoint �ʱ�ȭ
		//SaveMapChangePointUpdate(pInfo->dwCharacterID, 0);
	}

	else

	{
		if(pInfo->dwUserID)
			LoginCheckDelete(pInfo->dwUserID);		// LOGOUT�ð��� ������Ʈ �մϴ�.
	}

	if(pInfo->dwUserID)
	{
		g_pUserTableForUserID->RemoveUser(pInfo->dwUserID);
#ifdef _NPROTECT_

		if( pInfo->m_pCSA )
		{
			pInfo->m_pCSA->Close();
			g_CCSAuth2Pool.Free(pInfo->m_pCSA);

		}
#endif
		// punish
		PUNISHMGR->RemovePunishUnitAll( pInfo->dwUserID );
		//

		//---KES Network Process Fix 071114
		memset( pInfo, 0, sizeof(USERINFO) );
		g_UserInfoPool.Free(pInfo);	
		//----------------------------
	}

	g_pUserTable->RemoveUser( dwKey );


	//user count ���==============================================================================
	SERVERINFO * myInfo = g_pServerTable->GetSelfServer();
	myInfo->wAgentUserCnt = (WORD)GetUserCount();

	MSG_WORD2 msg2;
	msg2.Category = MP_SERVER;
	msg2.Protocol = MP_SERVER_USERCNT;
	msg2.wData1 = myInfo->wPortForServer;
	msg2.wData2 = myInfo->wAgentUserCnt;

	
	g_pServerTable->SetPositionHead();
	SERVERINFO* pOtherAgentInfo = NULL;
	while(pOtherAgentInfo = g_pServerTable->GetNextDistServer())
	{
		if(myInfo == pOtherAgentInfo)
			continue;

		g_Network.Send2Server(pOtherAgentInfo->dwConnectionIndex, (char *)&msg2, sizeof(msg2));
	}
	//user count ���==============================================================================

	return TRUE;
}

BOOL DisconnectUser(DWORD dwConnectionIndex)
{
	g_Network.DisconnectUser(dwConnectionIndex);

	return TRUE;
}
void	__stdcall OnRemoveObjProc( USERINFO* pInfo)
{
}
