#include "stdafx.h"
#include "RecallManager.h"
#include "..\[CC]Header\GameResourceStruct.h"
#include "Monster.h"
#include "RegenManager.h"
#include "AISystem.h"
#include "RegenPrototype.h"
#include "CharMove.h"
#include "ChannelSystem.h"
#include "UserTable.h"
#include "..\[CC]Header\GameResourceManager.h"
CRecallManager::CRecallManager()
{
}
CRecallManager::~CRecallManager()
{
}
void CRecallManager::DeleteEventRecallMonster()
{
	g_pUserTable->SetPositionUserHead();
	CObject* pObject;
	while( pObject = g_pUserTable->GetUserData() )
	{
		if( pObject->GetObjectKind() & eObjectKind_Monster )
		{
			{
				if( ((CMonster*)pObject)->GetMonsterGroupNum() == (WORD)EVENTRECALL_GROUP_ID )
				{
					g_pServerSystem->RemoveMonster( pObject->GetID() );
				}
			}
		}
	}
}
void CRecallManager::EventRecall(CObject * pRecaller, WORD MonsterKind, BYTE cbMonsterCount, BYTE cbChannel, VECTOR3* pPos, WORD wRadius, WORD DropItemID, DWORD dwDropRatio )
{
	DWORD dwID, dwSubID;
	DWORD dwChannelID;
	VECTOR3 vRecallPos;
	for( int i = 0 ; i < cbMonsterCount ; ++i )
	{
		if( cbChannel == 0 )
		{
			for( DWORD j = 0 ; j < CHANNELSYSTEM->GetChannelCount() ; ++j )
			{
				dwChannelID = CHANNELSYSTEM->GetChannelID( j );
				if( dwChannelID )
				{
					dwID = g_pAISystem->GeneraterMonsterID();
					dwSubID = g_pAISystem->GeneraterMonsterID();
					if( wRadius != 0 )
					{
						vRecallPos.x = pPos->x + (rand()%(wRadius*2)) - wRadius;
						vRecallPos.z = pPos->z + (rand()%(wRadius*2)) - wRadius;
					}
					else
					{
						vRecallPos.x = pPos->x;
						vRecallPos.z = pPos->z;
					}
					if( vRecallPos.x < 0 )				vRecallPos.x = 0;
					else if( vRecallPos.x > 51199 )		vRecallPos.x = 51199;
					if( vRecallPos.z < 0 )				vRecallPos.z = 0;
					else if( vRecallPos.z > 51199 )		vRecallPos.z = 51199;
					BASE_MONSTER_LIST* pList = GAMERESRCMNGR->GetMonsterListInfo(MonsterKind);
					if(pList)
					{
						//RegenObjectForGMTool
						//RegenObject
						//daily
						REGENMGR->RegenObjectForGMTool( dwID, dwSubID, dwChannelID, pList->ObjectKind, pList->MonsterKind, &vRecallPos, EVENTRECALL_GROUP_ID, DropItemID, dwDropRatio, FALSE, TRUE );					
					}
				}
			}
		}
		else if( cbChannel > 0 )
		{
			dwChannelID = CHANNELSYSTEM->GetChannelID( cbChannel - 1 );
			if( dwChannelID )
			{
				dwID = g_pAISystem->GeneraterMonsterID();
				dwSubID = g_pAISystem->GeneraterMonsterID();
				if( wRadius != 0 )
				{
					vRecallPos.x = pPos->x + (rand()%(wRadius*2)) - wRadius;
					vRecallPos.z = pPos->z + (rand()%(wRadius*2)) - wRadius;
				}
				else
				{
					vRecallPos.x = pPos->x;
					vRecallPos.z = pPos->z;
				}
				if( vRecallPos.x < 0 )				vRecallPos.x = 0;
				else if( vRecallPos.x > 51199 )		vRecallPos.x = 51199;
				if( vRecallPos.z < 0 )				vRecallPos.z = 0;
				else if( vRecallPos.z > 51199 )		vRecallPos.z = 51199;
				BASE_MONSTER_LIST* pList = GAMERESRCMNGR->GetMonsterListInfo(MonsterKind);
				if(pList)
				{
					//RegenObjectForGMTool
					//RegenObject
					//daily
					REGENMGR->RegenObjectForGMTool( dwID, dwSubID, dwChannelID, pList->ObjectKind, pList->MonsterKind, &vRecallPos, EVENTRECALL_GROUP_ID, DropItemID, dwDropRatio, FALSE, TRUE );					
				}
			}
		}
	}
}
int CRecallManager::RecallRange( CObject* pRecaller, DWORD dwGridID, VECTOR3 * vPos, WORD wRecallMonsterKind, DWORD wRecallNum, DWORD dwRange, int GroupID,BOOL bRandomPos , BOOL bEventRegen , WORD ObjectKind  )
{
	VECTOR3 vRecallPos;
	float dx = 0, dz = 0;
	int ran = 0;
	int regencount = 0;
	WORD wObjectKind;
	for( WORD i = 0 ; i < wRecallNum ; ++i)
	{
		if(dwRange)
		{		
			ran = rand();
			dx = float(ran%dwRange) * (ran%2?1:-1);
			ran = rand();
			dz = float(ran%dwRange) * (ran%2?1:-1);
			vRecallPos.x = vPos->x + dx;
			vRecallPos.z = vPos->z + dz;
		}
		else
		{
			vRecallPos.x = vPos->x;
			vRecallPos.z = vPos->z;
		}
		DWORD dwID = g_pAISystem->GeneraterMonsterID();
		DWORD dwSubID;
		if(ObjectKind == eObjectKind_Npc)
			dwSubID = 0;
		else 
			dwSubID = g_pAISystem->GeneraterMonsterID();
		wObjectKind = ObjectKind;
		CMonster* pMonster = REGENMGR->RegenObject(dwID, dwSubID, dwGridID, wObjectKind, 
						wRecallMonsterKind, &vRecallPos, RECALL_GROUP_ID,
						0,100,bRandomPos,bEventRegen);
		if(pMonster)
		{
			++regencount;
			pMonster->SetSuryunGroup(GroupID);
		}
	}
	return regencount;
}
void CRecallManager::RecallObject(CMonster * pRecaller, RECALLINFO * pRecallInfo)
{
	switch(pRecallInfo->RecallType)
	{
	case RECALL_GROUP:
		{
			REGENMGR->RegenGroup(pRecallInfo->RecallID, pRecaller->GetGridID());
		}
		break;
	case RECALL_RECALLERRANGE:
		{
			VECTOR3 vRecallPos;
			pRecaller->GetPosition(&vRecallPos);
			RecallRange(pRecaller, pRecaller->GetGridID(), &vRecallPos, (WORD)pRecallInfo->RecallID, 
				pRecallInfo->RecallMonsterNum, pRecallInfo->RecallRange, 0 );
		}
		break;
	case RECALL_RECALLERPOSITION:
		{
			VECTOR3 vRecallPos;
			pRecaller->GetPosition(&vRecallPos);
			RecallRange(pRecaller, pRecaller->GetGridID(), &vRecallPos, (WORD)pRecallInfo->RecallID, 1, 0, 0 );
		}
		break;
	case RECALL_TARGETRANGE:
		{
			VECTOR3 vRecallPos;
			if(!pRecaller->GetTObject()) return;
			pRecaller->GetTObject()->GetPosition(&vRecallPos);
			RecallRange(pRecaller, pRecaller->GetGridID(), &vRecallPos, (WORD)pRecallInfo->RecallID, 
				pRecallInfo->RecallMonsterNum, pRecallInfo->RecallRange, 0 );
		}
		break;
	}
}
void CRecallManager::QuestRecall( WORD wMonsterKind, BYTE bMonsterCount, DWORD dwChannel, VECTOR3* pMonsterPos, WORD wRadius )
{
	DWORD dwID, dwSubID;	
	VECTOR3 vRecallPos;
	for( BYTE i = 0; i < bMonsterCount; ++i )
	{
		if( dwChannel )
		{
			dwID = g_pAISystem->GeneraterMonsterID();
			dwSubID = g_pAISystem->GeneraterMonsterID();
			if( wRadius )
			{
				vRecallPos.x = pMonsterPos->x + (rand()%(wRadius*2)) - wRadius;
				vRecallPos.z = pMonsterPos->z + (rand()%(wRadius*2)) - wRadius;
			}
			else
			{
				vRecallPos.x = pMonsterPos->x;
				vRecallPos.z = pMonsterPos->z;
			}
			if( vRecallPos.x < 0 )				vRecallPos.x = 0;
			else if( vRecallPos.x > 51199 )		vRecallPos.x = 51199;
			if( vRecallPos.z < 0 )				vRecallPos.z = 0;
			else if( vRecallPos.z > 51199 )		vRecallPos.z = 51199;
			REGENMGR->RegenObject( dwID, dwSubID, dwChannel, eObjectKind_Monster, wMonsterKind, &vRecallPos, QUESTRECALL_GROUP_ID, 0, 100, FALSE, TRUE );
		}
	}
}
void CRecallManager::DeleteQuestRecallMonster( DWORD dwChannel )
{
	CObject* pObject = NULL;
	CMonster* pMonster = NULL;
	g_pUserTable->SetPositionUserHead();	
	while( pObject = g_pUserTable->GetUserData() )
	{
		if( pObject->GetObjectKind() & eObjectKind_Monster )
		{
			pMonster = (CMonster*)pObject;
			if( pMonster->GetMonsterGroupNum() == (WORD)QUESTRECALL_GROUP_ID &&
				pMonster->GetBattleID() == dwChannel )
				g_pServerSystem->RemoveMonster( pObject->GetID() );
		}
	}
}
CMonster* CRecallManager::InsDungeonRecall(WORD dwObjectKind, WORD wMonsterKind, DWORD dwGroupID, DWORD dwChannel, VECTOR3* pMonsterPos, WORD wRadius)
{
	CMonster* pMonster = NULL;
	DWORD dwID, dwSubID;
	VECTOR3 vRecallPos;
	if (dwChannel)
	{
		dwID = g_pAISystem->GeneraterMonsterID();
		dwSubID = g_pAISystem->GeneraterMonsterID();
		if (wRadius)
		{
			vRecallPos.x = pMonsterPos->x + (rand() % (wRadius * 2)) - wRadius;
			vRecallPos.z = pMonsterPos->z + (rand() % (wRadius * 2)) - wRadius;
		}
		else
		{
			vRecallPos.x = pMonsterPos->x;
			vRecallPos.z = pMonsterPos->z;
		}
		if (vRecallPos.x < 0)				vRecallPos.x = 0;
		else if (vRecallPos.x > 51199)		vRecallPos.x = 51199;
		if (vRecallPos.z < 0)				vRecallPos.z = 0;
		else if (vRecallPos.z > 51199)		vRecallPos.z = 51199;
		pMonster = REGENMGR->RegenObject(dwID, dwSubID, dwChannel, dwObjectKind, wMonsterKind, &vRecallPos, dwGroupID, 0, 100, FALSE, TRUE);
		if (pMonster == NULL)
			ASSERT(0);
	}
	else
		ASSERT(0);
	return pMonster;
}
void CRecallManager::DeleteInsDungeonRecallMonster(DWORD dwChannel)
{
	CObject* pObject = NULL;
	CMonster* pMonster = NULL;
	g_pUserTable->SetPositionUserHead();
	while (pObject = g_pUserTable->GetUserData())
	{
		if (pObject->GetObjectKind() & eObjectKind_Monster)
		{
			pMonster = (CMonster*)pObject;
			if (pMonster->GetBattleID() == dwChannel)
				g_pServerSystem->RemoveMonster(pObject->GetID());
		}
	}
}
CMonster* CRecallManager::InsDungeonMissionRecall(WORD wMonsterKind, BYTE bMonsterCount, DWORD dwChannel, VECTOR3* pMonsterPos, WORD wRadius)
{
	CMonster* pMonster = NULL;
	DWORD dwID, dwSubID;
	VECTOR3 vRecallPos;
	for (BYTE i = 0; i < bMonsterCount; ++i)
	{
		if (dwChannel)
		{
			dwID = g_pAISystem->GeneraterMonsterID();
			dwSubID = g_pAISystem->GeneraterMonsterID();
			if (wRadius)
			{
				vRecallPos.x = pMonsterPos->x + (rand() % (wRadius * 2)) - wRadius;
				vRecallPos.z = pMonsterPos->z + (rand() % (wRadius * 2)) - wRadius;
			}
			else
			{
				vRecallPos.x = pMonsterPos->x;
				vRecallPos.z = pMonsterPos->z;
			}
			if (vRecallPos.x < 0)				vRecallPos.x = 0;
			else if (vRecallPos.x > 51199)		vRecallPos.x = 51199;
			if (vRecallPos.z < 0)				vRecallPos.z = 0;
			else if (vRecallPos.z > 51199)		vRecallPos.z = 51199;
			BASE_MONSTER_LIST* pList = GAMERESRCMNGR->GetMonsterListInfo(wMonsterKind);
			if (pList != NULL)
				pMonster = REGENMGR->RegenObject(dwID, dwSubID, dwChannel, pList->ObjectKind, pList->MonsterKind, &vRecallPos, MISSIONRECALL_GROUP_ID, 0, 100, FALSE, TRUE);
		}
	}
	return pMonster;
}