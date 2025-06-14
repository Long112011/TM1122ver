#include "stdafx.h"
#include "DungeonMgr.h"
#include "..\ChannelSystem.h"
#include "..\[CC]BattleSystem\BattleSystem_Server.h"
#include "..\[CC]Header\GameResourceManager.h"
#include "..\ItemManager.h"
#include "..\PartyManager.h"
#include "..\Party.h"
#include "..\PackedData.h"
#include "..\UserTable.h"
#include "..\LootingManager.h"
#include "..\MapDBMsgParser.h"
#include "..\Monster.h"
#include "..\Npc.h"
#include "..\RegenManager.h"
#include "..\AISystem.h"
#include "MHFile.h"
//#include "..\Trigger\Manager.h"
//#include "..\Trigger\Timer.h"
#include "DungeonProcessorBase.h"
#include "CharMove.h"
//#include "..\[cc]Skill\Server\Manager\SkillManager.h"
//#include "..\..\[cc]skill\Server\Info\BuffSkillInfo.h"
#include "SkillManager_Server.h"
#include "..\Player.h"
#include "CheckRoutine.h"
#include "..\RecallManager.h"
#include "MapNetworkMsgParser.h"
#include "Network.h"
#include "RegenPrototype.h"
#include "ChannelSystem.h"
#include "RegenManager.h"
#include "RegenConditionInfo.h"
#include "InsDungeonManager.h"
#include "../[CC]Header/InsFUBENStruct.h"
//#include "Finite State Machine/Machine.h"

GLOBALTON(CDungeonMgr)

CDungeonMgr::CDungeonMgr(void)
{
	m_DungeonPool = NULL;
	m_ReservationPool = NULL;
	m_NoPartyPool = NULL;

	m_MonsterIDGenerator.Init(10000, NEWOBJECT_STARTINDEX);

	//memset( m_FBChannel, 0, sizeof(CChannel)*1000 );
	memset(&pDungeonMapInfo,0,sizeof(DUNGEON_MAPINFO)); 
	memset(&pInsDungeonInfo,0,sizeof(InsDungeonInfo));
	memset(&pInsMission,0,sizeof(DUNGEON_MISSION));
	memset(&pInsSubMissionTest,0,sizeof(DUNGEON_MISSION_SUBTEST));

	//memset(&wDGInfo,0,sizeof(sDUNGEONNEWINFO));
	memset(&wDGTest,0,sizeof(sDUNGEONNEWINIT));
	memset(&wDGTest2,0,sizeof(sDUNGEONNEWINITDesc));
	memset(&wDGAISystem,0,sizeof(sDUNGEONAISYS));
		

	m_DGInfo.Initialize(200);
	m_FBEntrance.Initialize(20);

	m_InsDGInfo.Initialize(20);

	

	pInsSubMissionCount=0;

//---------------------------------------
	//m_FbTimerInfo.Initialize(100);
	m_FBMapAreaInfo.Initialize(100);

	m_InsDGTimerInfo.Initialize(100);
//---------------------------------------


	pPlayerInfoTable.Initialize(1024);

	//for(int i=0;i<1000;i++)
	//{
	//	wDGInfo.killcount[i]=0;
	//}


	TempeDGRemain=-1;
	
}

CDungeonMgr::~CDungeonMgr(void)
{
	SAFE_DELETE(m_pDungeonProcessor);

	stDungeon* pDungeon = NULL;
	m_DungeonList.SetPositionHead();
	while((pDungeon = m_DungeonList.GetData()) != NULL)
	{
		m_DungeonPool->Free(pDungeon);
	}
	m_DungeonList.RemoveAll();

	m_ChannelList.RemoveAll();

	stDungeonReservation* pReservation = NULL;
	m_ReservationList.SetPositionHead();
	while((pReservation = m_ReservationList.GetData()) != NULL)
	{
		m_ReservationPool->Free(pReservation);
	}
	m_ReservationList.RemoveAll();

	stDungeonNoPartyList* pNoPartyList;
	m_NoPartyList.SetPositionHead();
	while((pNoPartyList = m_NoPartyList.GetData()) != NULL)
	{
		m_NoPartyPool->Free(pNoPartyList);
	}
	m_NoPartyList.RemoveAll();

	/*stMonsterList* pMonsterListEasy;
	m_MonsterList_Easy.SetPositionHead();
	while((pMonsterListEasy = m_MonsterList_Easy.GetData()) != NULL)
	{
		SAFE_DELETE(pMonsterListEasy);
	}

	stMonsterList* pMonsterListNormal;
	m_MonsterList_Normal.SetPositionHead();
	while((pMonsterListNormal = m_MonsterList_Normal.GetData()) != NULL)
	{
		SAFE_DELETE(pMonsterListNormal);
	}

	stMonsterList* pMonsterListHard;
	m_MonsterList_Hard.SetPositionHead();
	while((pMonsterListHard = m_MonsterList_Hard.GetData()) != NULL)
	{
		SAFE_DELETE(pMonsterListHard);
	}*/

	stWarpInfo* pWarp;
	m_WarpList.SetPositionHead();
	while((pWarp = m_WarpList.GetData()) != NULL)
	{
		SAFE_DELETE(pWarp);
	}

	stSwitchNpcInfo* pSwitchNpc;
	m_SwitchNpcList.SetPositionHead();
	while((pSwitchNpc = m_SwitchNpcList.GetData()) != NULL)
	{
		SAFE_DELETE(pSwitchNpc);
	}
	SAFE_DELETE(m_DungeonPool);
	SAFE_DELETE(m_ReservationPool);
	SAFE_DELETE(m_NoPartyPool);


	InsDungeonInfo *MapInfo = NULL;
	m_DGInfo.SetPositionHead();
	while(MapInfo=m_DGInfo.GetData())
	{
		delete MapInfo;
	}
	m_DGInfo.RemoveAll();

	FB_MAP_INFO *MapInfo2 = NULL;
	m_FBEntrance.SetPositionHead();
	while(MapInfo2=m_FBEntrance.GetData())
	{
		delete MapInfo2;
	}
	m_FBEntrance.RemoveAll();


//---------------------------------------
	/*FBTIMERINFO * m_FbTimer=NULL;
	m_FbTimerInfo.SetPositionHead();
	while(m_FbTimer=m_FbTimerInfo.GetData())
	{
		delete m_FbTimer;
		m_FbTimer=NULL;
	}
	m_FbTimerInfo.RemoveAll();*/


	FUBENMAPINFO * m_FBMapArea=NULL;
	m_FBMapAreaInfo.SetPositionHead();
	while(m_FBMapArea=m_FBMapAreaInfo.GetData())
	{
		delete m_FBMapArea;
		m_FBMapArea=NULL;
	}
	m_FBMapAreaInfo.RemoveAll();



	InsDungeonTimeStore * InsDGTimer=NULL;
	m_InsDGTimerInfo.SetPositionHead();
	while(InsDGTimer=m_InsDGTimerInfo.GetData())
	{
		delete InsDGTimer;
		InsDGTimer=NULL;
	}
	m_InsDGTimerInfo.RemoveAll();

	
//---------------------------------------


	GUILD_WAR_PLAYER_TEST* pPlayerInfo = NULL;
	pPlayerInfoTable.SetPositionHead();
	while(pPlayerInfo = pPlayerInfoTable.GetData())
		pPlayerInfo = NULL;
	pPlayerInfoTable.RemoveAll();












	InsDGInfo *DDGInfo = NULL;
	m_InsDGInfo.SetPositionHead();
	while(DDGInfo=m_InsDGInfo.GetData())
	{
		delete DDGInfo;
	}
	m_InsDGInfo.RemoveAll();


}

void CDungeonMgr::Init()
{
	m_DungeonPool = new CMemoryPoolTempl<stDungeon>;
	m_DungeonPool->Init(50, 50, "m_DungeonPool");

	m_ReservationPool = new CMemoryPoolTempl<stDungeonReservation>;
	m_ReservationPool->Init(100, 100, "ReservationPool");

	m_NoPartyPool = new CMemoryPoolTempl<stDungeonNoPartyList>;
	m_NoPartyPool->Init(100, 100, "NoPartyList");

	m_DungeonList.Initialize(100);
	m_ChannelList.Initialize(100);
	m_ReservationList.Initialize(100);
	m_NoPartyList.Initialize(100);

	//m_MonsterList_Easy.Initialize(200);
	//m_MonsterList_Normal.Initialize(200);
	//m_MonsterList_Hard.Initialize(200);

	m_WarpList.Initialize(40);
	m_SwitchNpcList.Initialize(30);

	ClearDB(g_pServerSystem->GetMapNum());

	LoadFBEntranceCondition();
	LoadInsDungeonInfo();

}
void CDungeonMgr::Process()
{
	static DWORD dwLastCheckTick = gCurTime;
	static DWORD dwLastReserved ;
	if(gCurTime >= dwLastCheckTick+60000)
	{	
		stDungeonReservation* pReservation = NULL;
		m_ReservationList.SetPositionHead();
		while(/*pReservation && */(pReservation = m_ReservationList.GetData()) != NULL)
		{
			if( gCurTime - pReservation->dwRegistTime > 5000 )
			{
				//dwLastReserved=pReservation->dwCharIndex;
				//if(dwLastReserved==0) continue;
				//else
				//if(pReservation->dwChannelID && GetMonsterNum()==0 && GetPlayerNum()==0)
				//if(pReservation)
				{
					m_ReservationList.Remove(pReservation->dwCharIndex);
					m_ReservationPool->Free(pReservation);
				//	Keluar(pID->dwChannelID);
					//continue;
				}
				stDungeon* pID = m_DungeonList.GetData(pReservation->dwChannelID);
				if(pID && (0 == pID->dwJoinPlayerNum))
				{
					Keluar(pID->dwChannelID);
					continue;
				}
			}
		}
		CPlayer* pPlayer = NULL;
		stDungeonNoPartyList* pList = NULL;
		m_NoPartyList.SetPositionHead();
		while((pList = m_NoPartyList.GetData()) != NULL)
		{
			pPlayer = (CPlayer*)g_pUserTable->FindUser(pList->dwCharIndex);
			if(!pPlayer)
			{
				stDungeonNoPartyList* pNoPartyList = m_NoPartyList.GetData(pList->dwCharIndex);
				if(pNoPartyList)
				{
					m_NoPartyList.Remove(pList->dwCharIndex);
					m_NoPartyPool->Free(pNoPartyList);
				}
				continue;
			}
			if(pList->dwOutTime+30000<=gCurTime)
			{
				if(pList->dwOldPartyIndex != pPlayer->GetPartyIdx())
				{
					MoveToLoginMap(pList->dwCharIndex);
				}
				stDungeonNoPartyList* pNoPartyList = m_NoPartyList.GetData(pList->dwCharIndex);
				if(pNoPartyList)
				{
					m_NoPartyList.Remove(pList->dwCharIndex);
					m_NoPartyPool->Free(pNoPartyList);					
				}
			}
		}
		dwLastCheckTick = gCurTime;
	}
	if(m_pDungeonProcessor)
		m_pDungeonProcessor->Process();
}
void CDungeonMgr::NetworkMsgParser(DWORD dwConnectionIndex, BYTE Protocol, void* pMsg, DWORD dwLength)
{
	switch(Protocol)
	{
	case MP_DUNGEON_ENTRANCE_SYN:
		{
			MSG_DWORD4* pmsg = (MSG_DWORD4*)pMsg;
			//g_Console.LOG( 4, "MP_DUNGEON_ENTRANCE_SYN %d pmsg->dwObjectID:%d",DungeonMGR->GetChannelIDFromReservationList(pmsg->dwObjectID),pmsg->dwObjectID);
			KeluarTest(DungeonMGR->GetChannelIDFromReservationList(pmsg->dwObjectID));
		}
		break;

	case MP_DUNGEON_PORTAL_ENTRANCE_SYN:	// Request to enter the portal
		{
			MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;
			//g_Console.LOG(4, "MP_DUNGEON_PORTAL_ENTRANCE_SYN ");

			DungeonMGR->Entrance(pmsg->dwObjectID, pmsg->dwData);

			//MSG_DWORD_WORD3* pmsg = (MSG_DWORD_WORD3*)pMsg;
			//INSDUNGEONMGR->InsDGEntrance(pmsg);
			
		}
		break;
	case MP_DUNGEON_ENTRANCE_ACK:
		{
			MSG_DWORD5* pmsg = (MSG_DWORD5*)pMsg;
			CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser(pmsg->dwObjectID);
			if( pPlayer )
			{		
				if( pPlayer->IsPKMode() || LOOTINGMGR->IsLootedPlayer( pPlayer->GetID() ) )
				{
					MSG_DWORD msg;
					msg.Category	= MP_DUNGEON;
					msg.Protocol	= MP_DUNGEON_ENTRANCE_NACK;
					msg.dwObjectID  = pPlayer->GetID();
					msg.dwData		= 1;//eHOUSEERR_ERROR;
					pPlayer->SendMsg( &msg, sizeof( msg ) );
					return;
				}

				MSG_DWORD5 msg;
				ZeroMemory( &msg, sizeof(msg) );
				msg.Category = MP_DUNGEON;
				msg.Protocol = MP_DUNGEON_ENTRANCE_ACK;
				msg.dwObjectID = pmsg->dwObjectID;
				msg.dwData1 = pmsg->dwData1;
				msg.dwData2 = pmsg->dwData2;
				msg.dwData3 = pmsg->dwData3;
				msg.dwData4 = pmsg->dwData4;
				msg.dwData5 = pPlayer->GetChannelID();
				pPlayer->SendMsg( &msg, sizeof( msg ) );
			}
//------------------------------------------------------------------------------------------
			/*MAPCHANGE_INFO* ChangeInfo = GAMERESRCMNGR->GetMapChangeInfoForDestMapNum( pmsg->dwData1 );
			if( !ChangeInfo ) return;
			SaveMapChangePointUpdate( pPlayer->GetID(), ChangeInfo->Kind );

			pPlayer->UpdateLogoutToDB();

			VECTOR3 vStartPos;
			vStartPos.x = (float)pmsg->dwData2;
			vStartPos.y = 0.0f;
			vStartPos.z = (float)pmsg->dwData3;

			const MAPTYPE	mapType			= ChangeInfo->Kind;
			const WORD		channelIndex	= 0;
			const DWORD		playerIndex		= pPlayer->GetID();
			const DWORD		userIndex		= pPlayer->GetUserID();
			const MONEYTYPE	inventoryMoney	= pPlayer->GetMoney( eItemTable_Inventory );
			const MONEYTYPE	storageMoney	= pPlayer->GetMoney( eItemTable_Pyoguk );

			g_pServerSystem->RemovePlayer( pPlayer->GetID() );

			SaveMapChangePointReturn(	pmsg->dwObjectID,
										ChangeInfo->Kind,
										dwConnectionIndex,
										pPlayer->GetLevel(),
										pPlayer->GetPlayerExpPoint(),
										storageMoney
									);*/
//------------------------------------------------------------------------------------------			


			// Send map movement command
			/**/

		//	g_Console.LOG(4, "MP_DUNGEON_ENTRANCE_ACK");
		//	g_Console.LOG(4, "pmsg->dwData1 %d",pmsg->dwData1);//unique npc id
		//	g_Console.LOG(4, "pmsg->dwData2 %d",pmsg->dwData2);//current map
		//	g_Console.LOG(4, "pmsg->dwData3 %d",pmsg->dwData3);//party id
		//	g_Console.LOG(4, "pmsg->dwData4 %d",pmsg->dwData4);//move map number
		//	g_Console.LOG(4, "pPlayer->GetChannelID() %d",pPlayer->GetChannelID());
		//	g_Console.LOG(4, " ");
			//

			//PlayerEntered(pPlayer->GetID(), pPlayer->GetChannelID());
		}
		break;
	case MP_DUNGEON_ENTRANCE_FROM_SERVER_SYN:
		{
			MSG_DWORD5* pmsg = (MSG_DWORD5*)pMsg;
			DWORD dwError = eDungeonERR_None;
			DWORD dwResult = dwError;//Entrance(pmsg->dwObjectID, pmsg->dwData3);
			
			if( eDungeonERR_None == dwResult )
			{
				//break;
				MONEYTYPE	Fees	=EntraceConditionData(pmsg->dwData1)->FubenFee;
				LEVELTYPE	MinLevel=EntraceConditionData(pmsg->dwData1)->MinLevel;
				LEVELTYPE	MaxLevel=EntraceConditionData(pmsg->dwData1)->MaxLevel;
				BYTE		Key		=EntraceConditionData(pmsg->dwData1)->FubenKey;

				MSG_DWORD5 msg;
				ZeroMemory( &msg, sizeof(msg) );
				msg.Category = MP_DUNGEON;
				msg.Protocol = MP_DUNGEON_ENTRANCE_ACK;
				msg.dwObjectID = pmsg->dwObjectID;
				msg.dwData1 = pmsg->dwData1;
				msg.dwData2 = MinLevel;//pmsg->dwData2;
				msg.dwData3 = MaxLevel;//pmsg->dwData3;
				msg.dwData4 = Fees;//pmsg->dwData4;
				msg.dwData5 = Key;//pmsg->dwData5;
				msg.CyptKey = GetInsDungeonInfoData()->dgStringIdx;

			//	g_Console.LOG(4, "MP_DUNGEON_ENTRANCE_FROM_SERVER_SYN");
			//	g_Console.LOG(4, "pmsg->dwData1 %d",pmsg->dwData1);//unique npc id
			//	g_Console.LOG(4, "pmsg->dwData2 %d",pmsg->dwData2);//current map
			//	g_Console.LOG(4, "pmsg->dwData3 %d",pmsg->dwData3);//party id
			//	g_Console.LOG(4, "pmsg->dwData4 %d",pmsg->dwData4);//move map number
			//	g_Console.LOG(4, "pmsg->dwData5 %d",pmsg->dwData5);
			//	g_Console.LOG(4, " ");

				PACKEDDATA_OBJ->SendToMapServer(dwConnectionIndex,(MAPTYPE)pmsg->dwData2, &msg, sizeof(msg));
			}
			else
			{
				const BYTE DoRollback = 1;
				DungeonUpdate(pmsg->dwObjectID, (MAPTYPE)0, 0, 0, DoRollback);

				MSG_DWORD msg;
				ZeroMemory( &msg, sizeof(msg) );
				msg.Category	= MP_DUNGEON;
				msg.Protocol	= MP_DUNGEON_ENTRANCE_NACK;
				msg.dwObjectID	= pmsg->dwObjectID;
				msg.dwData		= dwResult;

				PACKEDDATA_OBJ->SendToMapServer(dwConnectionIndex,(MAPTYPE)pmsg->dwData2, &msg, sizeof(msg));
			}			
		}
		break;	
	case MP_DUNGEON_INFO_SYN:
		{
//-------------------------------------CLIENT
			MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;

			CObject* pObject = g_pUserTable->FindUser(pmsg->dwObjectID);
			if(! pObject || eObjectKind_Player != pObject->GetObjectKind())
				return;
//-------------------------------------
			CPlayer* const pPlayer = (CPlayer*)g_pUserTable->FindUser(pmsg->dwObjectID);
			if( !pPlayer )
				return;
//-------------------------------------
			
			DWORD dwNewTimer;
			stDungeon* pDungeon = m_DungeonList.GetData(pObject->GetGridID());
			if(pDungeon)
			{
				DGEntrance(dwConnectionIndex,g_pServerSystem->GetMapNum(),pmsg->dwObjectID,pDungeon->dwPartyIndex,g_pServerSystem->GetMapNum(),pDungeon->dwChannelID);

				if(false == GetTimerState(pDungeon->dwChannelID))
				{
					SetTimerStartBool(pDungeon->dwChannelID);
					Sleep(1000);
					LoadMonster(pDungeon->dwChannelID);			

					
				}

				
				
				/**/			
				dwNewTimer = DungeonMGR->CalculateTimer(pDungeon->dwChannelID);
				DGUpdate(pDungeon->dwPartyIndex, g_pServerSystem->GetMapNum(),pDungeon->dwChannelID,dwNewTimer,GetInsDungeonInfoData()->dgStringIdx);
					

				
			}
			if(pPlayer->GetFBMapArea()==0)
			{
				FUBENMAPINFO*pEventNew=GetFBMapAreaInfo(GetFBMapArea(pDungeon->dwChannelID));
				pPlayer->SetFBMapArea(GetFBMapArea(pDungeon->dwChannelID));
				DungeonMGR->SendNewMessage(GetInsDungeonInfoData()->dgStringIdx,0,0,pEventNew->FBMapKillCount,0,pPlayer);
			}
			else
			{

			}
			pPlayer->SetFBMapArea(GetFBMapArea(pDungeon->dwChannelID));
			pPlayer->SetFBTimeTo(dwNewTimer);
		}
		break;
	case MP_DUNGEON_ENTRANCE_NACK:
		{
			MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;
			CPlayer* const pPlayer = (CPlayer*)g_pUserTable->FindUser(pmsg->dwObjectID);
			if( !pPlayer )
			{
				return;
			}
			// Send message to the player
			MSG_DWORD msg;
			ZeroMemory( &msg, sizeof(msg) );
			msg.Category	= MP_DUNGEON;
			msg.Protocol	= MP_DUNGEON_ENTRANCE_NACK;
			msg.dwObjectID  = pPlayer->GetID();
			msg.dwData		= pmsg->dwData;
			pPlayer->SendMsg( &msg, sizeof( msg ) );
		}
		break;
	case MP_DUNGEON_FADEMOVE_SYN:
		{
			MSG_DUNGEON_FADE_MOVE* pmsg = (MSG_DUNGEON_FADE_MOVE*)pMsg;
			CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser(pmsg->dwMoverID) ;
			if( !pPlayer ) return;
			VECTOR3 pos = { 0 };
			pmsg->cpos.Decompress(&pos);
			if( !CheckValidPosition(pos) ) return ;
			CCharMove::SetPosition(pPlayer,&pos) ;			
			pmsg->Protocol = MP_DUNGEON_FADEMOVE_ACK;
			pPlayer->SendMsg(pmsg, sizeof(MSG_DUNGEON_FADE_MOVE));		
		}
		break;
	default:
		break;
	}
}
bool CDungeonMgr::SendErrorMsg(CPlayer* pPlayer,DWORD dwError)
{
	if(dwError != eDungeonERR_None)
	{
		MSG_DWORD	msg;
		msg.Category = MP_DUNGEON;
		msg.Protocol = MP_DUNGEON_ENTRANCE_NACK;
		msg.dwObjectID = pPlayer->GetID()/*pmsg->dwObjectID*/;
		msg.dwData = dwError;
		pPlayer->SendMsg(&msg, sizeof(msg));

		//g_Console.LOG(4, "DungeonMGR");
		return false;
	}

	return true;
}
bool CDungeonMgr::SendDialogMsg(CPlayer* pPlayer,MSG_DWORD5*pmsg,DWORD index)
{
	MONEYTYPE	Fees	=DungeonMGR->EntraceConditionData(pmsg->dwData2)->FubenFee;
	LEVELTYPE	MinLevel=DungeonMGR->EntraceConditionData(pmsg->dwData2)->MinLevel;
	LEVELTYPE	MaxLevel=DungeonMGR->EntraceConditionData(pmsg->dwData2)->MaxLevel;
	BYTE		Key		=DungeonMGR->EntraceConditionData(pmsg->dwData2)->FubenKey;

	MSG_DWORD5 msgACK;
	ZeroMemory( &msgACK, sizeof(msgACK) );

	msgACK.Category = MP_DUNGEON;
	msgACK.Protocol = MP_DUNGEON_ENTRANCE_ACK;
	msgACK.dwObjectID = pmsg->dwObjectID;
	msgACK.dwData1 = pmsg->dwData2;
	msgACK.dwData2 = MinLevel;
	msgACK.dwData3 = MaxLevel;
	msgACK.dwData4 = Fees;
	msgACK.dwData5 = Key;
	msgACK.CyptKey = 0;//DungeonMGR->GetInsDungeonInfoData()->dgStringIdx;
	PACKEDDATA_OBJ->SendToMapServer(index,(MAPTYPE)pmsg->dwData2, &msgACK, sizeof(msgACK));
	
	return true;
}
bool CDungeonMgr::MoveMapTest(CPlayer* pPlayer,WORD pMapUniqueNum,WORD pDestMapNum,DWORD dwConnectionIndex,BYTE pType)
{
//	Sleep(1500);
	if(pPlayer)
	{
		if((eMapChange_General || eMapChange_Item) == pType)
		{
			return SendErrorMsg(pPlayer,eDungeonERR_CreateFailed);
		}
		if(eMapChange_Dungeon != pType)
		{		
			return SendErrorMsg(pPlayer,eDungeonERR_NoParty);
		}
		CParty* pParty = PARTYMGR->GetParty(pPlayer->GetPartyIdx());
		if(!pParty)
		{
			return SendErrorMsg(pPlayer,eDungeonERR_NoParty);
		}
		if(!pPlayer->GetShopItemManager()->IsCanInPutFBMap() && DungeonMGR->GetFubenKey(pDestMapNum) == 1)
		{
			return SendErrorMsg(pPlayer,eDungeonERR_NoKeyInfo);
		}
		if(pPlayer->GetLevel() < EntraceConditionData(pDestMapNum)->MinLevel)
		{
			return SendErrorMsg(pPlayer,eDungeonERR_NotEnoughLevel);
		}
		if(EntraceConditionData(pDestMapNum)->MaxLevel < pPlayer->GetLevel())
		{
			return SendErrorMsg(pPlayer,eDungeonERR_OverLevel);
		}
		if(pPlayer->GetMoney() < EntraceConditionData(pDestMapNum)->FubenFee)
		{
			return SendErrorMsg(pPlayer,eDungeonERR_CreateFailed);
		}
		//if(!CHKRT->ItemOf(pPlayer, (POSTYPE)pmsg->dwData4, pmsg->dwData3, 0, 0, CB_EXIST|CB_ICONIDX))
		//{
		//	return SendErrorMsg(pPlayer,eDungeonERR_ERROR);
		//}
		//pPlayer->SetMoney((MONEYTYPE)EntraceConditionData(pDestMapNum)->FubenFee, MONEY_SUBTRACTION, MF_LOST, eItemTable_Inventory, eMoneyLog_GetCheatMoney, 0);

		

		pPlayer->SetMoney((MONEYTYPE)EntraceConditionData(pDestMapNum)->FubenFee,MONEY_SUBTRACTION);


		MAPCHANGE_INFO* ChangeInfo = GAMERESRCMNGR->GetMapChangeInfoForDestMapNum( pMapUniqueNum );
		if( !ChangeInfo ) return 1;
		SaveMapChangePointUpdate( pPlayer->GetID(), ChangeInfo->Kind );

		pPlayer->UpdateLogoutToDB();

		//VECTOR3 vStartPos;
		//vStartPos.x = (float)pmsg->dwData2;
		//vStartPos.y = 0.0f;
		//vStartPos.z = (float)pmsg->dwData3;

		const MAPTYPE	mapType			= ChangeInfo->Kind;
		const WORD		channelIndex	= 0;
		const DWORD		playerIndex		= pPlayer->GetID();
		const DWORD		userIndex		= pPlayer->GetUserID();
		const MONEYTYPE	inventoryMoney	= pPlayer->GetMoney( eItemTable_Inventory );
		const MONEYTYPE	storageMoney	= pPlayer->GetMoney( eItemTable_Pyoguk );

		pPlayer->GetPetManager()->SetPetSummonning(ePSS_SaveSummon);

		g_pServerSystem->RemovePlayer( pPlayer->GetID() );

		SaveMapChangePointReturn(	playerIndex,
									mapType,
									dwConnectionIndex,
									pPlayer->GetLevel(),
									pPlayer->GetPlayerExpPoint(),
									storageMoney
								);

		return SendErrorMsg(pPlayer,eDungeonERR_None);
	}
	return false;
}
stDungeon* CDungeonMgr::Create(DWORD dwPartyID, DWORD dwChannelID)
{
	stDungeon* pInst = m_DungeonPool->Alloc();

	if(pInst)
	{
		pInst->Clear();
		pInst->dwPartyIndex = dwPartyID;
		pInst->dwChannelID = dwChannelID;
		pInst->difficulty = eDIFFICULTY_NONE;//diffculty;
		
		m_ChannelList.Add(&pInst->dwChannelID, pInst->dwPartyIndex);
		m_DungeonList.Add(pInst, pInst->dwChannelID);

		//InsDGInfo*pNewEvent=GetInsDungeonInfoData();
		

		if(m_pDungeonProcessor)
			m_pDungeonProcessor->Create(pInst);


		LoadInsDGMission(GetInsDungeonInfoData()->dgStringIdx);
		//LoadMonster(dwChannelID);

		CreateNewTimer(dwChannelID,GetInsDungeonInfoData()->dgTime);

		//AddPoint(pInst->dwChannelID,1);

//		SetMonsterList(dwChannelID, diffculty);
//		InitWarpList(dwChannelID);
//		InitSwitchNpcList(dwChannelID);
	}
	return pInst;
}
void CDungeonMgr::Keluar(DWORD dwChannelID)
{//this loop is stable exit
	stDungeon* pInst = m_DungeonList.GetData(dwChannelID);
	if(pInst)
	{
		//CObject* pObject = NULL;
		//g_pUserTable->SetPositionUserHeadChannel(dwChannelID);
		//while((pObject = (CObject*)g_pUserTable->GetUserDataChannel(dwChannelID)) != NULL)
		//{
			/*switch(pObject->GetObjectKind())
			{
			case eObjectKind_Monster: g_pServerSystem->RemoveMonster(pObject->GetID());			break;
			case eObjectKind_Player: { 
										if(pInst->dwPartyIndex != ((CPlayer*)pObject)->GetPartyIdx())
										{
											MoveToLoginMap(((CPlayer*)pObject)->GetID());		
										}
																continue;//break;
									 }
			case eObjectKind_Npc: g_pServerSystem->RemoveNpc(pObject->GetID());					break;
			case eObjectKind_Pet:																continue;break;
			case eObjectKind_SkillObject: g_pServerSystem->RemoveSkillObject(pObject->GetID()); break;
			case eObjectKind_Tactic: g_pServerSystem->RemoveTacticObject(pObject->GetID());		break;
			case eObjectKind_MapObject: g_pServerSystem->RemoveMapObject(pObject->GetID());		break;
			default: {
						CSkillObject* pSkill = SKILLMGR->GetSkillObject(pObject->GetID());
						if(pSkill)	
						{
																continue;//break;
						}
					 }
			}*/


			/*if(pObject->GetObjectKind() & eObjectKind_Monster)
			{
				g_pServerSystem->RemoveMonster(pObject->GetID());
			}
			else if(pObject->GetObjectKind() == eObjectKind_Player)
			{
				if(pInst->dwPartyIndex != ((CPlayer*)pObject)->GetPartyIdx())
					MoveToLoginMap(((CPlayer*)pObject)->GetID());
				continue;
			}
			else if(pObject->GetObjectKind() == eObjectKind_Npc)
			{
				g_pServerSystem->RemoveNpc(pObject->GetID());
			}
			else if(pObject->GetObjectKind() == eObjectKind_Pet)
			{
				continue;
			}
			else if(pObject->GetObjectKind() == eObjectKind_SkillObject)
			{
				g_pServerSystem->RemoveSkillObject(pObject->GetID());
			}
			else if(pObject->GetObjectKind() == eObjectKind_MapObject)
			{
				g_pServerSystem->RemoveMapObject(pObject->GetID());
			}
			else
			{
				CSkillObject* pSkill = SKILLMGR->GetSkillObject(pObject->GetID());
				if(pSkill)	continue;				
			}*/
		//}

		CObject* pObject = NULL;
		g_pUserTable->SetPositionUserHead();
		while (pObject = g_pUserTable->GetData())
		{
			if (pObject->GetObjectKind() & eObjectKind_Monster)
			{
				if (pObject->GetGridID() != pInst->dwChannelID)
					continue;

				g_pServerSystem->RemoveMonster(pObject->GetID());
			}
		}
		ClearDBWithChannel(g_pServerSystem->GetMapNum(),pInst->dwPartyIndex);
		BATTLESYSTEM->DestroyChannel(pInst->dwChannelID);
		BATTLESYSTEM->ReleaseBattleID(pInst->dwChannelID);
		m_ChannelList.Remove(pInst->dwPartyIndex);
		RemoveTimer(pInst->dwChannelID);
		m_InsDGTimerInfo.Remove(pInst->dwChannelID);//kiv
		m_DungeonList.Remove(pInst->dwChannelID);
		m_DungeonPool->Free(pInst);		
	}
}
void CDungeonMgr::KeluarTest(DWORD dwChannelID)
{
	stDungeon* pID = m_DungeonList.GetData(dwChannelID);
	if(pID && (0 == pID->dwJoinPlayerNum))
	{
		m_DungeonList.Remove(pID->dwChannelID);
		m_DungeonPool->Free(pID);
		//Keluar(pID->dwChannelID);
		//continue;
	}
	/*stDungeon* pInst = m_DungeonList.GetData(dwChannelID);
	if(pInst)
	{
		m_DungeonList.Remove(pInst->dwChannelID);
		m_DungeonPool->Free(pInst);
	}*/
	/*stDungeonReservation* pReservation = NULL;
	m_ReservationList.SetPositionHead();
	while((pReservation = m_ReservationList.GetData()) != NULL)
	{
		//m_ReservationList.Remove(pReservation->dwCharIndex);
		//m_ReservationPool->Free(pReservation);			
		stDungeon* pID = m_DungeonList.GetData(dwChannelID);
		if(pID && (0 == pID->dwJoinPlayerNum))
		{
			Keluar(pID->dwChannelID);
			continue;
		}
	}*/
}
DWORD CDungeonMgr::Entrance(DWORD dwPlayerID, DWORD dwPartyID)
{
	DWORD* pChannelID = m_ChannelList.GetData(dwPartyID);
	if(! pChannelID)
	{
		DWORD dwNewChannel = BATTLESYSTEM->CreateChannel();
		stDungeon* pCreateDungeon = Create(dwPartyID, dwNewChannel);
		if( !pCreateDungeon )
		{
			return eDungeonERR_CreateFailed;
		}
		ReservationDungeon(dwPlayerID, dwPartyID, dwNewChannel);
		//DungeonMGR->PlayerEntered(dwPlayerID,dwNewChannel);
	}
	else
	{
		stDungeon* pDungeon = m_DungeonList.GetData(*pChannelID);
		if( !pDungeon )
		{
			return eDungeonERR_NoChannel;
		}
		ReservationDungeon(dwPlayerID, dwPartyID, *pChannelID);
		//DungeonMGR->PlayerEntered(dwPlayerID,*pChannelID);
	}
	return eDungeonERR_None;
}
void CDungeonMgr::Exit(CPlayer* pPlayer)
{
	//g_Console.LOG(4, "Exit");
	if(pPlayer && !pPlayer->IsDungeonObserver())
	{
		DWORD dwChannelID = pPlayer->GetChannelID();
		stDungeon* pDungeon = m_DungeonList.GetData(dwChannelID);
		if(pDungeon)
		{
			pDungeon->dwJoinPlayerNum--;
			//DGUpdate(pPlayer->GetPartyIdx(), pPlayer->GetCurMapNum(),pPlayer->GetChannelID(),0,GetInsDungeonInfoData()->dgStringIdx,5);
			if(0 == pDungeon->dwJoinPlayerNum)
			{
				//Keluar(dwChannelID);

				
				CObject* pObject = NULL;
				g_pUserTable->SetPositionUserHead();
				while (pObject = g_pUserTable->GetData())
				{
					if (pObject->GetObjectKind() & eObjectKind_Monster)
					{
						if (pObject->GetGridID() != pDungeon->dwChannelID)
							continue;

						g_pServerSystem->RemoveMonster(pObject->GetID());
					}
				}
				ClearDBWithChannel(g_pServerSystem->GetMapNum(), pDungeon->dwPartyIndex);
				BATTLESYSTEM->DestroyChannel(pDungeon->dwChannelID);
				BATTLESYSTEM->ReleaseBattleID(pDungeon->dwChannelID);
				RemoveTimer(pDungeon->dwChannelID);
				m_InsDGTimerInfo.Remove(pDungeon->dwChannelID);//kiv

				m_ChannelList.Remove(pDungeon->dwPartyIndex);
				
				m_DungeonList.Remove(pDungeon->dwChannelID);
				m_DungeonPool->Free(pDungeon);
				
				//SubtractPoint(dwChannelID,1);
			}
		}
		RemoveNoPartyList(pPlayer->GetID());
	}
}

void CDungeonMgr::ReservationDungeon(DWORD dwPlayerID, DWORD dwPartyID, DWORD dwChannelID)
{
	stDungeonReservation* pReservation = NULL;
	pReservation = m_ReservationList.GetData(dwPlayerID);
	
	if(pReservation)
	{
		pReservation->dwCharIndex	= dwPlayerID;
		pReservation->dwPartyIndex	= dwPartyID;
		pReservation->dwChannelID	= dwChannelID;
		
	//	pReservation->vStartPos.x	= 0;
	//	pReservation->vStartPos.y	= 0;
	//	pReservation->vStartPos.z	= 0;
		pReservation->dwRegistTime	= gCurTime;
	}
	else
	{
		pReservation = m_ReservationPool->Alloc();
		if(pReservation)
		{
			pReservation->Clear();
			pReservation->dwCharIndex	= dwPlayerID;
			pReservation->dwPartyIndex	= dwPartyID;
			pReservation->dwChannelID	= dwChannelID;
			
		//	pReservation->vStartPos.x	= 0;
		//	pReservation->vStartPos.y	= 0;
		//	pReservation->vStartPos.z	= 0;
			pReservation->dwRegistTime	= gCurTime;
			m_ReservationList.Add(pReservation, dwPlayerID);
		}
	}
}


DWORD CDungeonMgr::GetChannelIDFromReservationList(DWORD dwPlayerID)
{
	stDungeonReservation* pReservation = m_ReservationList.GetData(dwPlayerID);
	if(pReservation)
	{
		return pReservation->dwChannelID;
	}

	//JackLogOutWithColorConsole("GetChannelIDFromReservationList return 0");
	/*FILE* fpLog = NULL;
	fpLog = fopen( "./Log/DungeonEnterLog.txt", "a+" );
	if( fpLog )
	{
		SYSTEMTIME sysTime;
		GetLocalTime( &sysTime );

		fprintf( fpLog, "[%04d-%02d-%02d %02d:%02d:%02d] %s - not exist reservation data [PlayerID : %d] \n", 
			sysTime.wYear, sysTime.wMonth, sysTime.wDay, sysTime.wHour, sysTime.wMinute, sysTime.wSecond,
			__FUNCTION__,
			dwPlayerID );

		fclose( fpLog );
	}*/

	return 0;
}
DWORD CDungeonMgr::GetChannelIDFromPartyIdx(CPlayer* pPlayer)
{	
	return pPlayer->GetPartyIdx();
}
VECTOR3* CDungeonMgr::GetStartPosFromeservationList(DWORD dwPlayerID)
{
	stDungeonReservation* pReservation = m_ReservationList.GetData(dwPlayerID);
	if(pReservation)
	{
		PlayerEntered(dwPlayerID, pReservation->dwChannelID);		
		return NULL;//&pReservation->vStartPos;
	}

	return NULL;
}
void CDungeonMgr::PlayerEntered(DWORD dwPlayerID, DWORD dwChannelID)
{
	stDungeon* pDungeon = m_DungeonList.GetData(dwChannelID);
	if(pDungeon)
	{
		pDungeon->dwJoinPlayerNum++;
		//DGUpdate(pDungeon->dwPartyIndex, g_pServerSystem->GetMapNum(),dwChannelID,0,0);
	//	g_Console.LOG(4,"pDungeon->dwPartyIndex:%d",pDungeon->dwPartyIndex);
		//Sleep(1000);
		stDungeonReservation* pReservation = m_ReservationList.GetData(dwPlayerID);
		if(pReservation)
		{
			m_ReservationList.Remove(dwPlayerID);
			m_ReservationPool->Free(pReservation);
		}
	}
}
DWORD CDungeonMgr::GetPlayerNumInDungeon(DWORD dwChannelID)
{
	stDungeon* pDungeon = m_DungeonList.GetData( dwChannelID );
	if( pDungeon != NULL )
		return pDungeon->dwJoinPlayerNum;
	//else
	return 0;
}

DWORD CDungeonMgr::GetPartyIndexFromDungeon(DWORD dwChannelID)
{
	stDungeon* pID = m_DungeonList.GetData(dwChannelID);
	if(pID)
		return pID->dwPartyIndex;
	//else
	return 0;
}

DWORD CDungeonMgr::GetDungeonNum()
{
	return m_DungeonList.GetDataNum();
}

DWORD CDungeonMgr::GetPlayerNum()
{
	return g_pUserTable->GetUserCount();
}

DWORD CDungeonMgr::GetMonsterNum()
{
	return g_pUserTable->GetMonsterCount();
}

DWORD CDungeonMgr::GetNpcNum()
{
	return g_pUserTable->GetNpcCount();
}

DWORD CDungeonMgr::GetExtraNum()
{
	return g_pUserTable->GetExtraCount();
}

void CDungeonMgr::AddPoint(DWORD dwChannelID, DWORD value)
{
	stDungeon* pID = m_DungeonList.GetData(dwChannelID);
	if(pID)
		pID->dwPoint += value;
}

void CDungeonMgr::SubtractPoint(DWORD dwChannelID, DWORD value)
{
	stDungeon* pID = m_DungeonList.GetData(dwChannelID);
	if(pID)
	{
		if(pID->dwPoint > value)
			pID->dwPoint -= value;
		else
			pID->dwPoint = 0;
	}
}

DWORD CDungeonMgr::GetPoint(DWORD dwChannelID)
{
	stDungeon* pID = m_DungeonList.GetData(dwChannelID);
	if(pID)
		return pID->dwPoint;

	return 0;
}

void CDungeonMgr::SendMsgToChannel(MSGBASE* pMsg, DWORD dwLength, DWORD dwChannelID)
{
	if(! pMsg)
		return;

	CObject* pObject;
	g_pUserTable->SetPositionHead();
	while((pObject = g_pUserTable->GetData()) != NULL)
	{
		if(pObject &&
			eObjectKind_Player == pObject->GetObjectKind() &&
			pObject->GetGridID() == dwChannelID)
		{
			PACKEDDATA_OBJ->QuickSend( pObject, pMsg, dwLength );
		}
	}
}


void CDungeonMgr::AddNoPartyList(DWORD dwPlayerID, DWORD dwPartyID)
{
	if(FALSE == DungeonMGR->IsDungeon(g_pServerSystem->GetMapNum()))
		return;

	if(0 == g_pUserTable->FindUser(dwPlayerID))
		return;

	stDungeonNoPartyList* pList = m_NoPartyPool->Alloc();
	if(pList)
	{
		pList->dwCharIndex = dwPlayerID;
		pList->dwOldPartyIndex = dwPartyID;
		pList->dwOutTime = gCurTime;

		m_NoPartyList.Add(pList, pList->dwCharIndex);
	}
}

void CDungeonMgr::RemoveNoPartyList(DWORD dwPlayerID)
{
	m_NoPartyList.Remove(dwPlayerID);
}




void CDungeonMgr::CreateNewTimer(DWORD dwChannelID, DWORD timerAlias)
{
	InsDungeonTimeStore * pInfo = new InsDungeonTimeStore;
	pInfo->InsDGChannelIdx		= dwChannelID;
	pInfo->InsDGTimer			= timerAlias;
	pInfo->InsDGStartTime		= gCurTime;
	pInfo->IsStart				= false;
	pInfo->IsInsDGState			= eWaitingToStart;
	pInfo->InsDGMonsterKillCount= 0;
	m_InsDGTimerInfo.Add(pInfo, pInfo->InsDGChannelIdx);
}
InsDungeonTimeStore * CDungeonMgr::GetTimeStoreInfo(DWORD dwChannelID)
{
	return m_InsDGTimerInfo.GetData(dwChannelID);
}
void CDungeonMgr::SetCurrentMapState(DWORD dwChannelID,DWORD dwState)
{
	InsDungeonTimeStore* pDungeon2 = m_InsDGTimerInfo.GetData(dwChannelID);
	if(pDungeon2)
	{
		pDungeon2->IsInsDGState = dwState;
	}
}
void CDungeonMgr::RemoveTimer(DWORD dwChannelID)
{
	stDungeon* pDungeon = m_DungeonList.GetData(dwChannelID);
	if(pDungeon)
	{
		pDungeon->m_TimerAlias = 0;
	}
	//m_InsDGTimerInfo.RemoveAll();
	//m_DungeonList.RemoveAll();
	/*InsDungeonTimeStore* pDungeon2 = m_InsDGTimerInfo.GetData(dwChannelID);
	if(pDungeon2)
	{
		pDungeon2->InsDGTimer = 0;
		pDungeon2->InsDGStartTime = 0;
		pDungeon2->IsStart = false;
		pDungeon2->IsInsDGState = eAllSubMissionDone;
	}*/
}
DWORD CDungeonMgr::GetTimer(DWORD dwChannelID)
{
	if(GetTimeStoreInfo(dwChannelID))
		return GetTimeStoreInfo(dwChannelID)->InsDGTimer;
}
bool CDungeonMgr::GetTimerState(DWORD dwChannelID)
{
	if(GetTimeStoreInfo(dwChannelID))
		return GetTimeStoreInfo(dwChannelID)->IsStart;
}
int CDungeonMgr::GetMonsterKillCount(DWORD dwChannelID)
{
	if(GetTimeStoreInfo(dwChannelID))
		return GetTimeStoreInfo(dwChannelID)->InsDGMonsterKillCount;
}
WORD CDungeonMgr::GetFBMapArea(DWORD dwChannelID)
{
	if(GetTimeStoreInfo(dwChannelID))
		return GetTimeStoreInfo(dwChannelID)->InsDGFBMapArea;
}
void CDungeonMgr::SetTimerStartBool(DWORD dwChannelID)
{
	InsDungeonTimeStore* pDungeon = m_InsDGTimerInfo.GetData(dwChannelID);
	if(pDungeon)
	{
		pDungeon->InsDGStartTime = gCurTime;
		pDungeon->IsStart = true;
		pDungeon->InsDGMonsterKillCount = 0;
		pDungeon->InsDGFBMapArea = 1;
	}
}
void CDungeonMgr::SetMonsterKillCount(DWORD dwChannelID,int Count)
{
	InsDungeonTimeStore* pDungeon = m_InsDGTimerInfo.GetData(dwChannelID);
	if(pDungeon)
	{
		pDungeon->InsDGMonsterKillCount = Count;
	}
}
void CDungeonMgr::SetFBMapArea(DWORD dwChannelID,int Count)
{
	InsDungeonTimeStore* pDungeon = m_InsDGTimerInfo.GetData(dwChannelID);
	if(pDungeon)
	{
		pDungeon->InsDGFBMapArea = Count;
	}
}
DWORD CDungeonMgr::CalculateTimer(DWORD dwChannelID)
{
	//if(!GetTimerState(dwChannelID)) return 0;
	DWORD dwNewTimer =GetTimer(dwChannelID)-(GetElapsedSecondFromStartTime(dwChannelID)*1000);
	return dwNewTimer;
}


DWORD CDungeonMgr::GetElapsedSecondFromStartTime(DWORD dwChannelID)
{
	//if(!GetTimerState(dwChannelID)) return 0;
	return (gCurTime - GetTimeStoreInfo(dwChannelID)->InsDGStartTime) / 1000;
}

BOOL CDungeonMgr::IsDungeon(MAPTYPE mapType) const
{
	return g_pServerSystem->GetMap()->IsMapKind(eInsDungeon, mapType);
}


BOOL CDungeonMgr::CheckHackNpc( CPlayer* pPlayer, WORD wNpcIdx )
{
	stSwitchNpcInfo* pInfo = m_SwitchNpcList.GetData(wNpcIdx);
    if( pInfo == NULL )
	{
		return FALSE;
	}
	
	VECTOR3 vPos;
	VECTOR3 vNpcPos = {0,};
	vNpcPos.x = pInfo->wPosX;
	vNpcPos.z = pInfo->wPosZ;

	pPlayer->GetPosition( &vPos );
	float dist = CalcDistanceXZ( &vPos, &vNpcPos );
	if( dist > 3000.0f )
	{
		return FALSE;
	}

	return TRUE;
}


DWORD CDungeonMgr::AddNpc( stSwitchNpcInfo* pSwitchNpc, DWORD dwChannelID )
{
	MAPTYPE MapNum = pSwitchNpc->wMapNum;

	VECTOR3 vOutPos = {0};
	vOutPos.x = pSwitchNpc->wPosX;
	vOutPos.z = pSwitchNpc->wPosZ;


	BASEOBJECT_INFO Baseinfo;
	NPC_TOTALINFO NpcTotalInfo;

	ZeroMemory(&Baseinfo, sizeof(BASEOBJECT_INFO));
	ZeroMemory(&NpcTotalInfo, sizeof(NPC_TOTALINFO));
	
	NPC_LIST* pNpcList = GAMERESRCMNGR->GetNpcInfo(pSwitchNpc->wNpcJob);
	if(pNpcList == 0)
		return 0;

	//Baseinfo.dwObjectID =  g_pAISystem->GeneraterMonsterID();
	Baseinfo.BattleID = dwChannelID;
	
	SafeStrCpy(Baseinfo.ObjectName, pSwitchNpc->name, MAX_NAME_LENGTH+1);
	NpcTotalInfo.Group = 0;
	NpcTotalInfo.MapNum = MapNum;
	NpcTotalInfo.NpcJob = pNpcList->JobKind;
	NpcTotalInfo.NpcKind = pNpcList->NpcKind;
	NpcTotalInfo.NpcUniqueIdx = pSwitchNpc->wIndex;

	CNpc* pNpc = g_pServerSystem->AddNpc(&Baseinfo, &NpcTotalInfo, &vOutPos);
	if(pNpc)
	{
		pNpc->SetDieTime(0);
	}
	else
		return 0;

	return pNpc->GetID();
}

/*const stDungeon& CDungeonMgr::GetDungeon(DWORD channelIndex)
{
	const stDungeon* const dungeon = m_DungeonList.GetData(channelIndex);

	if(0 == dungeon)
	{
		static const stDungeon emptyDungeon;
		return emptyDungeon;
	}

	return *dungeon;
}
*/

/*
void CDungeonMgr::StoreSwitchBuff( DWORD dwChannelID, DWORD dwBuffIdx )
{
	if( IsDungeon( g_pServerSystem->GetMapNum() ) == FALSE )
		return;

	stDungeon* pDungeon = m_DungeonList.GetData( dwChannelID );
	if( !pDungeon )
		return;
	
	BuffVector& buff = m_DungeonBuffMap[dwChannelID];

	BuffVector::iterator iter = find(buff.begin(), buff.end(), dwBuffIdx );
	if( iter == buff.end() )
		buff.push_back(dwBuffIdx);
}

void CDungeonMgr::DelSwitchBuff( DWORD dwChannelID, DWORD dwBuffIdx )
{
	if( IsDungeon( g_pServerSystem->GetMapNum() ) == FALSE )
		return;

	stDungeon* pDungeon = m_DungeonList.GetData( dwChannelID );
	if( !pDungeon )
		return;
	
	BuffVector& buff = m_DungeonBuffMap[dwChannelID];

	BuffVector::iterator iter = find(buff.begin(), buff.end(), dwBuffIdx );
	if( iter != buff.end() )
		buff.erase( iter );
}*/


DWORD CDungeonMgr::GeneraterMonsterID()
{
	DWORD id = m_MonsterIDGenerator.GenerateIndex();
	//ASSERT(id != 0);
	return id;
}
//------------------------------------------kiv
void CDungeonMgr::MoveToLoginMap(DWORD dwCharIndex)
{
	CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser(dwCharIndex);
	if(pPlayer)
	{
		MAPTYPE mapNum = pPlayer->GetCurMapNum();
		MAPCHANGE_INFO* ChangeInfo = GAMERESRCMNGR->GetMapChangeInfoForDestMapNum( mapNum );
		if( !ChangeInfo ) return;
		SaveMapChangePointUpdate( pPlayer->GetID(), ChangeInfo->Kind );

		pPlayer->UpdateLogoutToDB();

		g_pServerSystem->RemovePlayer( pPlayer->GetID() );

//		Exit(pPlayer->GetID());//kuv
	}
}
//------------------------------------------
void CDungeonMgr::ReturnToMap(CPlayer* pPlayer)
{
	MAPCHANGE_INFO* ChangeInfo = GAMERESRCMNGR->GetMapChangeInfoForDestMapNum( nanju );
	if( !ChangeInfo ) return;
	SaveMapChangePointUpdate( pPlayer->GetID(), ChangeInfo->Kind );

	pPlayer->UpdateLogoutToDB();
			
	g_pServerSystem->RemovePlayer( pPlayer->GetID() );

	MSG_WORD2 msg;
	msg.Category	= MP_USERCONN;
	msg.Protocol	= MP_USERCONN_MAP_OUT_WITHMAPNUM;
	msg.dwObjectID	= pPlayer->GetID();
	msg.wData1		= nanju;
	msg.wData2		= 1;

	g_Network.Broadcast2AgentServer( (char*)&msg, sizeof(msg));
}
void CDungeonMgr::GoToMap(CPlayer* pPlayer, DWORD MapNum)
{
	MAPCHANGE_INFO* ChangeInfo = GAMERESRCMNGR->GetMapChangeInfoForDestMapNum( MapNum );
	if( !ChangeInfo ) return;
	SaveMapChangePointUpdate( pPlayer->GetID(), ChangeInfo->Kind );

	pPlayer->UpdateLogoutToDB();
			
	g_pServerSystem->RemovePlayer( pPlayer->GetID() );

	MSG_WORD2 msg;
	msg.Category	= MP_USERCONN;
	msg.Protocol	= MP_USERCONN_MAP_OUT_WITHMAPNUM;
	msg.dwObjectID	= pPlayer->GetID();
	msg.wData1		= MapNum;
	msg.wData2		= 1;

	g_Network.Broadcast2AgentServer( (char*)&msg, sizeof(msg));

}
bool CDungeonMgr::CheckMap(MSG_DWORD4* pmsg)
{
	if(g_pServerSystem->GetStart() == FALSE )
	{
		MSG_BYTE nmsg;
		nmsg.Category = MP_USERCONN;
		nmsg.Protocol = MP_USERCONN_CHANGEMAP_CHANNELINFO_NACK;
		nmsg.bData = 0;
		nmsg.dwObjectID = pmsg->dwObjectID;
		g_Network.Send2Server(1,(char*)&nmsg,sizeof(nmsg));

		g_Console.LOG(4,"CheckMap false");
		return false;
	}
	g_Console.LOG(4,"CheckMap true");
	return true;
}

void CDungeonMgr::RemoveMonster(WORD pChannel)
{
	if(FALSE == IsDungeon(g_pServerSystem->GetMapNum()))
		return;

	CObject* pObject = NULL;
	g_pUserTable->SetPositionUserHead();
	while(pObject = g_pUserTable->GetData())
	{
		if(pObject->GetObjectKind() & eObjectKind_Monster)
		{
			//if(pObject->GetGridID() != pDungeonMapInfo.pChannnelNum)
			//	continue;

			if(pObject->GetGridID() == pChannel)
				g_pServerSystem->RemoveMonster(pObject->GetID());
		}
	}
}
BOOL CDungeonMgr::SuspendMonster(WORD pChannel)
{
	//if(!pDungeonMapInfo.pClearMonster)
	//	return FALSE;

	//if(IsDungeon(g_pServerSystem->GetMapNum()))
	//	return TRUE;
	//return FALSE;

	if(FALSE == IsDungeon(g_pServerSystem->GetMapNum()))
		return FALSE;

	CObject* pObject = NULL;
	g_pUserTable->SetPositionUserHead();
	while(pObject = g_pUserTable->GetData())
	{
		if(pObject->GetObjectKind() & eObjectKind_Monster)
		{
			if(pObject->GetGridID() == pChannel)
			{
				g_pServerSystem->RemoveMonster(pObject->GetID());
				return TRUE;//g_pServerSystem->RemoveMonster(pObject->GetID());
			}
		}
	}
	
}




void CDungeonMgr::LoadFBEntranceCondition()
{
	CMHFile file;
	if (!file.Init(INFB_ENTRANCE_CONDITION, "rb"))
		return;
	FB_MAP_INFO * MapInfo = NULL;
	while(1)
	{
		if(file.IsEOF())
			break;

		MapInfo = new FB_MAP_INFO;
		MapInfo->MapCurMap	= file.GetByte();
		MapInfo->MapIDX		= file.GetWord();		
		MapInfo->MinLevel	= file.GetWord();
		MapInfo->MaxLevel	= file.GetWord();
		MapInfo->FubenKey	= file.GetByte();
		MapInfo->FubenFee	= file.GetInt();
		m_FBEntrance.Add(MapInfo,MapInfo->MapIDX);
		MapInfo = NULL;
	}
	file.Release();
	return;
}
FB_MAP_INFO * CDungeonMgr::EntraceConditionData(DWORD UniqueIdx)
{
	return m_FBEntrance.GetData(UniqueIdx);
}
BYTE CDungeonMgr::GetEntraceCurMapNum(DWORD UniqueIdx)
{
	return EntraceConditionData(UniqueIdx)->MapCurMap;
}
WORD CDungeonMgr::GetEntraceUniqueNpcIdx(DWORD UniqueIdx)
{
	return EntraceConditionData(UniqueIdx)->MapIDX;
}
BYTE CDungeonMgr::GetFubenKey(WORD MapNum)
{
	FB_MAP_INFO * MapInfo = NULL;
	MapInfo = m_FBEntrance.GetData(MapNum);
	if(MapInfo!=NULL)
		return MapInfo->FubenKey;
	else
		return 0;
}










//----------------------------------------------------------------------------------------------



void CDungeonMgr::CheckKillCount(CPlayer* pPlayer,DWORD Num,DWORD MonsterId)
{
	if(FALSE == DungeonMGR->IsDungeon(g_pServerSystem->GetMapNum()))return;
		
	if(NULL == pPlayer)return;

	FUBENMAPINFO*pEvent=GetFBMapAreaInfo(/*pPlayer->GetFBMapArea()*/GetFBMapArea(pPlayer->GetChannelID()));


	if(pEvent->FBMapMonster==MonsterId)
	{
		if(/*pPlayer->GetFBTimerMonsterNum()*/GetMonsterKillCount(pPlayer->GetChannelID())<=pEvent->FBMapKillCount)
		{//if current kill number less than you set

			if(/*pPlayer->GetFBTimerMonsterNum()*/GetMonsterKillCount(pPlayer->GetChannelID())==pEvent->FBMapKillCount-1)
			{
				if(pEvent->FBMapNextFlag!=0)
				{
					//pPlayer->InitFBTimerMonsterNum();
					//pPlayer->SetFBMapArea(pEvent->FBMapNextFlag);
					SetFBMapArea(pPlayer->GetChannelID(),pEvent->FBMapNextFlag);
					DGUpdate(pPlayer->GetPartyIdx(), pPlayer->GetCurMapNum(),pPlayer->GetChannelID(),0,0,eDGMonsterKillReset);
					SendKillCount(0,pPlayer->GetChannelID());

					FUBENMAPINFO*pEventNew=GetFBMapAreaInfo(/*pPlayer->GetFBMapArea()*/GetFBMapArea(pPlayer->GetChannelID()));

					DungeonMGR->SetFBMapArea(pPlayer->GetChannelID(),/*pEvent->FBMapNextFlag*/pEvent->FBMapNextFlag);
					DungeonMGR->SendNewMessage(GetInsDungeonInfoData()->dgStringIdx,pEvent->FBMapNextFlag,0,pEventNew->FBMapKillCount,pPlayer->GetChannelID());
					DungeonMGR->SetMonsterKillCount(pPlayer->GetChannelID(),0);
					return;
				}
				else
				{
					//pPlayer->InitFBTimerMonsterNum();
					DGUpdate(pPlayer->GetPartyIdx(), pPlayer->GetCurMapNum(),pPlayer->GetChannelID(),0,0,eDGCurMapDone);
					SendKillCount(0,pPlayer->GetChannelID());
					DungeonMGR->SetMonsterKillCount(pPlayer->GetChannelID(),0);

					if(GetInsDungeonInfoData()->dgFinishMap != GetInsDungeonInfoData()->dgBackMap)
					{
						//g_Console.LOG( 4, "pNewEvent->dgMapChange:%d",GetInsDungeonInfoData()->dgMapChange);

						FubenInfoKind(pPlayer->GetID(),pPlayer->GetChannelID(),450,157,1,1171);

						//ClearDBWithChannel(g_pServerSystem->GetMapNum(),pPlayer->GetChannelID());

						//Keluar(pPlayer->GetChannelID());
					}
					else
					{
						SendMessage(2,pPlayer->GetChannelID());
					}
					return;
				}
			}
			

			DGUpdate(pPlayer->GetPartyIdx(), pPlayer->GetCurMapNum(),pPlayer->GetChannelID(),1,0,eDGMonsterKill);
		//	pPlayer->AddFBTimerMonsterNum(1);
		//	SendKillCount(pPlayer->GetFBTimerMonsterNum(),pPlayer->GetChannelID());
		}
	}
}
void CDungeonMgr::AutoReturnToMap()
{
	if(FALSE ==g_pServerSystem->GetMap()->IsMapKind(eInsDungeon))
		return;

	CPlayer* pPlayer = NULL;
	g_pUserTable->SetPositionUserHead();
	while(pPlayer = (CPlayer*)g_pUserTable->GetData())
	{
		if(pPlayer->GetObjectKind() == eObjectKind_Player)
		{
			if(pPlayer->GetPartyIdx()==0)
			{
				RemoveAllPlayer();
			}
			if(pPlayer->GetDungeonFinishSwitch())

				//return;
			//if(pPlayer->GetChannelID() != pWarMapInfo.pChannnelNum)
				//continue;
			//if (pPlayer->GetState() != eObjectState_Die)
			//	continue;

			if(gCurTime - pPlayer->GetDungeonFinishTime() >= 15000)
			{
				//ReturnToMap(pPlayer);

				RemoveAllPlayer();
				pPlayer->SetDungeonFinishTime(gCurTime);

				//pPlayer->SetDungeonFinishSwitch(false);
			}
		}
	}
}
void CDungeonMgr::RemoveAllPlayer()
{
	CPlayer* pPlayer = NULL;
	g_pUserTable->SetPositionUserHead();
	while(pPlayer = (CPlayer*)g_pUserTable->GetData())
	{
		if(pPlayer->GetObjectKind() != eObjectKind_Player)
			continue;

		//if(pPlayer->GetChannelID() != /*pWarMapInfo.pChannnelNum*/pPlayer->GetGridID())
		//	continue;

		if(pPlayer->GetPartyIdx()==0)
			ReturnToMap(pPlayer);

		if(pPlayer->GetDungeonFinishSwitch())
			ReturnToMap(pPlayer);
	}
}


FUBENMAPINFO * CDungeonMgr::GetFBMapAreaInfo(DWORD Flag)
{
	return m_FBMapAreaInfo.GetData(Flag);
}
DWORD CDungeonMgr::GetFBMapAreaFlag(DWORD dwFlag)
{
	FUBENMAPINFO* fbMapInfo = m_FBMapAreaInfo.GetData( dwFlag );
	if( fbMapInfo != NULL )
		return fbMapInfo->FBMapMonsterY;

	return 0;
}


/*
FBTIMERINFO * CDungeonMgr::GetFBTimerAreaInfo(DWORD Flag)
{
	return m_FbTimerInfo.GetData(Flag);	
}
BYTE CDungeonMgr::CheckIfMapNeedPass(DWORD MapNum)
{
	FBTIMERINFO* fbTimer = m_FbTimerInfo.GetData( MapNum );
	if( fbTimer != NULL )
		return fbTimer->MapPass;

	return 0;
}
char* CDungeonMgr::CheckFBMapKind(DWORD MapNum)
{
	FBTIMERINFO* fbTimer = m_FbTimerInfo.GetData( MapNum );
	if( fbTimer != NULL )
		return fbTimer->MapKindSet;

	return 0;
}
DWORD CDungeonMgr::MapEnterTimer(DWORD MapNum)
{
	FBTIMERINFO* fbTimer = m_FbTimerInfo.GetData( MapNum );
	if( fbTimer != NULL )
		return fbTimer->MapTimerMin;

	return 0;
}*/



void CDungeonMgr::SendKillCount(DWORD Val,DWORD dwChannelID)
{
	MSG_DWORD3 msg;
	msg.Category = MP_DUNGEON;
	msg.Protocol = MP_DUNGEON_INFO_ACK;
	msg.dwData1 = 1;	//1 AddMonsterCount	//2 SetMonsterCount/leaving dungeon
	msg.dwData2 = Val;
	SendMsgToChannel(&msg, sizeof(msg), dwChannelID);
}
void CDungeonMgr::SendMessage(DWORD Val,DWORD dwChannelID)
{
	MSG_DWORD3 msg;
	msg.Category = MP_DUNGEON;
	msg.Protocol = MP_DUNGEON_INFO_ACK;
	msg.dwData1 = Val;
	SendMsgToChannel(&msg, sizeof(msg), dwChannelID);
}

void CDungeonMgr::SendNewMessage(DWORD dwLoadSubMsg,DWORD dwSubMsg,DWORD dwCurKillCount,DWORD dwMaxKillCount,DWORD dwChannelID,CPlayer* pPlayer)
{
	MSG_DWORD4 msg;
	msg.Category = MP_DUNGEON;
	msg.Protocol = MP_DUNGEON_SENDMSG_SYN;
	msg.dwData1 = dwLoadSubMsg;
	msg.dwData2 = dwSubMsg;
	msg.dwData3 = dwCurKillCount;
	msg.dwData4 = dwMaxKillCount;

	if(dwChannelID!=0)
		SendMsgToChannel(&msg, sizeof(msg), dwChannelID);
	else
	{
		msg.dwObjectID = pPlayer->GetID();
		pPlayer->SendMsg( &msg, sizeof(msg) );
	}
}
void CDungeonMgr::FubenInfoKind(DWORD CharacterIdx,DWORD dwChannelID,DWORD Val,DWORD dwFlag,BYTE Type,DWORD Val2)
{
	MSG_DWORD3 msg;
	msg.Category = MP_DUNGEON;
	msg.Protocol = MP_DUNGEON_SETSWITCH_ACK;
	msg.dwData1 = Val;
	msg.dwData2=Val2;
	msg.CheckSum=Type;
	msg.CyptKey=dwFlag;
	SendMsgToChannel(&msg, sizeof(msg), dwChannelID);
}
//----------------------------------------------------------------------------------------------
void CDungeonMgr::LoadInsDGMission(DWORD dwGroupNum)
{
	char buff[128]={0,};
	static DWORD Group;
	CMHFile file;
	if (file.Init(INFB_FUBEN_MISSION, "rb") == FALSE) return;
		
	while(1)
	{
		if(file.IsEOF()) break;

		file.GetString(buff);

		START_STRING_COMPARE(buff)

			COMPARE("$MISSIONGROUP")	
				wDGTest.GroupNum			= file.GetWord();
			COMPARE("$MISSION")				
				wDGTest.SubMission			= file.GetInt();
			COMPARE("#MONSTER_TARGETKILL")
				wDGTest.MonsterTarget		= file.GetWord();
			COMPARE("#MAXCOUNT")
				wDGTest.MonsterKillCount	= file.GetWord();
			COMPARE("#NEXT_MISSION")
				wDGTest.NextSubMission		= file.GetWord();
			//COMPARE("#MONSTER_ALLKILL")
			//	wDGTest.EnableKillAll		= file.GetBool();
				//wDGTest.KillAllZero			= file.GetWord();

			if(wDGTest.GroupNum!=dwGroupNum)
			{				
				continue;
			}	

			FUBENMAPINFO * pInfo = new FUBENMAPINFO;
			pInfo->FBMapID			= wDGTest.SubMission;
			pInfo->FBMapMonster		= wDGTest.MonsterTarget;
			pInfo->FBMapNextFlag	= wDGTest.NextSubMission;

			//if(!wDGTest.EnableKillAll)
			pInfo->FBMapKillCount	= wDGTest.MonsterKillCount;
			
			//pInfo->FBMapKillAllZero	= wDGTest.KillAllZero;
			m_FBMapAreaInfo.Add(pInfo, pInfo->FBMapID);

		END_COMPARE
	}
	file.Release();	
}
int CDungeonMgr::LoadMonster(DWORD dwChannel)
{
	char buff[128]={0,};
	static int CountObj=0;
	CMHFile file;
	char filename[256];
	sprintf(filename,"Resource/Server/InsFUBENMonster_%02d.bin",GAMERESRCMNGR->GetLoadMapNum());
	if(!file.Init(filename,"rb"))
		return 0;
	while(1)
	{
		if(file.IsEOF())
			break;
		file.GetString(buff);
		START_STRING_COMPARE(buff)
			COMPARE("$Group")
				wDGAISystem.GroupNum		= file.GetWord();
			COMPARE("#MAXOBJECT")
				wDGAISystem.MaxObject		= file.GetWord();
			COMPARE("#ADD")
				wDGAISystem.AddKind			= file.GetWord();
				wDGAISystem.AddGenerateId	= file.GetWord();
				wDGAISystem.AddMonsterId	= file.GetWord();

				wDGAISystem.AddPosXYZ.x		= file.GetFloat();
				wDGAISystem.AddPosXYZ.y		= file.GetFloat();
				wDGAISystem.AddPosXYZ.z		= file.GetFloat();
//----------------------------------------
			
			DWORD dwID, dwSubID;
			VECTOR3 vPos = { (float)wDGAISystem.AddPosXYZ.x, 0.0f, (float)wDGAISystem.AddPosXYZ.y };

			BASE_MONSTER_LIST* pList = GAMERESRCMNGR->GetMonsterListInfo(wDGAISystem.AddMonsterId);
			if(pList)
			{
				for(int i=0;i<1/*wDGAISystem.MaxObject*/;++i)
				{					
					dwID = g_pAISystem->GeneraterMonsterID();
					dwSubID = g_pAISystem->GeneraterMonsterID();
					REGENMGR->RegenObject(	dwID,
											0,						/*dwSubID*///
											dwChannel,				//player channel number
											wDGAISystem.AddKind,	//monster kind 
											pList->MonsterKind,		//monster list kind 
											&vPos,					//spawn position
											EVENTRECALL_GROUP_ID,	//group idx
											0,						/*8000*///drop item idx
											0,						/*100*///drop ratio
											FALSE					//,//random spawn position
																	//TRUE//regen for event
											);
					CountObj=CountObj+1;
					
				}
			}
//----------------------------------------
		END_COMPARE
			
	}
	file.Release();
	return CountObj;
}

BOOL CDungeonMgr::LoadInsDungeonInfo()
{
	if( IsDungeon( g_pServerSystem->GetMapNum() ) == FALSE ) return FALSE;		

	CMHFile file;
	if (!file.Init(INFB_FUBEN_INFO, "rb")) return FALSE;
		
	InsDGInfo * MapInfo = NULL;
	while(1)
	{
		if(file.IsEOF()) break;
//200	1	Charstr		0	17	1170	203		60000	1			
		MapInfo = new InsDGInfo;
		MapInfo->dgMapNum = file.GetWord();
		MapInfo->dgParam1 = file.GetByte();
		file.GetString();
		MapInfo->dgParam2 = file.GetByte();
		MapInfo->dgBackMap = file.GetByte();
		
		MapInfo->dgMapChange = file.GetWord();
		MapInfo->dgFinishMap = file.GetByte();
		MapInfo->dgTime = file.GetInt();
		MapInfo->dgStringIdx = file.GetByte();
		
		m_InsDGInfo.Add(MapInfo,MapInfo->dgMapNum);
		
		MapInfo = NULL;
	}
	file.Release();
	return TRUE;
}
InsDGInfo * CDungeonMgr::GetInsDungeonInfoData()
{
	return m_InsDGInfo.GetData(g_pServerSystem->GetMapNum());
}
void CDungeonMgr::BackToTown(CPlayer* pPlayer)
{
	MAPCHANGE_INFO* ChangeInfo = GAMERESRCMNGR->GetMapChangeInfoForDestMapNum( GetInsDungeonInfoData()->dgBackMap );
	if( !ChangeInfo ) return;
	SaveMapChangePointUpdate( pPlayer->GetID(), ChangeInfo->Kind );

	pPlayer->UpdateLogoutToDB();
			
	g_pServerSystem->RemovePlayer( pPlayer->GetID() );

	MSG_WORD2 msg;
	msg.Category	= MP_USERCONN;
	msg.Protocol	= MP_USERCONN_MAP_OUT_WITHMAPNUM;
	msg.dwObjectID	= pPlayer->GetID();
	msg.wData1		= GetInsDungeonInfoData()->dgBackMap;
	msg.wData2		= 1;

	g_Network.Broadcast2AgentServer( (char*)&msg, sizeof(msg));
}




#define STORED_CLEAR_DUNGEON_INFO	"dbo.JACK_DUNGEON"
void CDungeonMgr::ClearDB(DWORD dwMapIndex)
{
	char txt[128];	
	sprintf(txt, "EXEC dbo.JACK_DUNGEONRECORD_Delete %d", dwMapIndex );
	g_DB.Query(eQueryType_FreeQuery, eDeleteGuildWarRecord, 0, txt);
}
void CDungeonMgr::ClearDBWithChannel(DWORD dwMapIndex,DWORD dwChannelIdx)
{
	char txt[128];
	sprintf(txt, "EXEC dbo.JACK_DUNGEONRECORD_ChannelRemove %d, %d", dwMapIndex ,dwChannelIdx);
	g_DB.Query(eQueryType_FreeQuery, eDeleteGuildWarRecord, 0, txt);
}
void CDungeonMgr::Routine60Sec()
{
	char txt[128];
	sprintf(txt, "EXEC dbo.JACK_DUNGEONRECORD_Routine %d", g_pServerSystem->GetMapNum());
	g_DB.Query(eQueryType_FreeQuery, eDeleteGuildWarRecord, 0, txt);
}
