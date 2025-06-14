#include "stdafx.h"
#include "./Interface/cWindowManager.h"
#include "WindowIdEnum.h"
#include "cMsgBox.h"
#include "DungeonMgr.h"
#include "InsFUBENMissionString.h"
#include "MHMap.h"
#include "MHFile.h"
#include "ObjectManager.h"
#include "GameResourceStruct.h"
#include "GameResourceManager.h"
//#include "./FadeDlg.h"
#include "./GameIn.h"
#include "ChatManager.h"
#include "TileManager.h"
#include "..\MHCamera.h"
#include "MoveManager.h"
#include "objectstatemanager.h"
#include "InsDGEntranceInfoDlg.h"
#include "FBTimeDialog.h"
#include "./Audio/MHAudioManager.h"
#include "aimanager.h"
#include "./input/UserInput.h"
#include "MHTimeManager.h"
#include "../[CC]Header/InsFUBENStruct.h"
//#include "FubenManager.h"

//-----------------------------------FROM FUBENMANAGER.CPP
//#include "..\[CC]Fuben\FubenScriptLoader.h"
#include "..\[CC]Fuben\FubenString.h"
//--------------------------------------------------------
GLOBALTON(CDungeonMgr)
CDungeonMgr::CDungeonMgr(void)
{
	m_CurDungeon.Clear();
	m_FBTipTextList.Initialize(200);
	m_FBEntrance.Initialize(20);
	IsLeaveStarted=false;
	dwCountDownOneSec=0;
	dwCountDownTick=15;
	memset(&wDGTest2,0,sizeof(sDUNGEONNEWINITDesc));

	//-----------------------------------FROM FUBENMANAGER.CPP
	m_FubenStringTable.Initialize(200);
	//--------------------------------------------------------

	StoredNpcUniqueIdx=0;
	Storeddestmapnumber=0;
	StoredNpcId=0;
	StoredNpcUniqueIdxT=0;

	m_InsDGInfo.Initialize(20);
}
CDungeonMgr::~CDungeonMgr(void)
{
	m_WarpList.SetPositionHead();
	for(stWarpInfo* pWarp = m_WarpList.GetData();
		0 < pWarp;
		pWarp = m_WarpList.GetData())
	{
		SAFE_DELETE(
			pWarp);
	}
	m_SwitchNpcList.SetPositionHead();
	for(stSwitchNpcInfo* pSwitchNpc = m_SwitchNpcList.GetData();
		0 < pSwitchNpc;
		pSwitchNpc = m_SwitchNpcList.GetData())
	{
		SAFE_DELETE(
			pSwitchNpc);
	}
	DUNGEON_TEXT* pToolTipText = NULL;
	DUNGEON_TEXT* pTemp = NULL;
	m_FBTipTextList.SetPositionHead();
	while( pToolTipText = m_FBTipTextList.GetData() )
	{
		do
		{
			pTemp = pToolTipText->pNext;
			if( pToolTipText->strToolTip )
				delete[] pToolTipText->strToolTip;
			delete pToolTipText;
			pToolTipText = pTemp;
		} while( pToolTipText );
	}
	m_FBTipTextList.RemoveAll();
	FB_MAP_INFO *MapInfo = NULL;
	m_FBEntrance.SetPositionHead();
	while(MapInfo=m_FBEntrance.GetData())
	{
		delete MapInfo;
	}
	m_FBEntrance.RemoveAll();


	//-----------------------------------FROM FUBENMANAGER.CPP
	CFubenString* pFubenString = NULL;
	m_FubenStringTable.SetPositionHead();
	while( pFubenString = m_FubenStringTable.GetData() )
	{
		delete pFubenString;
	}
	m_FubenStringTable.RemoveAll();
	//--------------------------------------------------------

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

	LoadFubenString(INFB_MISSION_STRING);
	GAMEIN->GetFBTimeDialog()->SetActive();
}
void CDungeonMgr::Process()
{
	if(! IsDungeonMap(MAP->GetMapNum())) return;
		
	GAMEIN->GetFBTimeDialog()->Process();
	LeaveProcess();

	if(HERO->GetBattleID()==1)
		WINDOWMGR->MsgBox( MBI_BUDDY_INFOAGREE, MBT_OK, CHATMGR->GetChatMsg(2658) );
}
void CDungeonMgr::StartLeave()
{
	IsLeaveStarted=true;
	dwCountDownOneSec=MHTIMEMGR->GetNewCalcCurTime();
	dwCountDownTick=15;
}
void CDungeonMgr::LeaveProcess()
{
	if(!IsLeaveStarted) return;
	if(gCurTime-dwCountDownOneSec>1000)
	{
		dwCountDownOneSec+=1000;
		if(dwCountDownTick<=0)
		{
			IsLeaveStarted=false;
			gChannelNum = 1;
			//g_UserInput.GetHeroMove()->MapChange( 17, 0, 0, eMapChange_GuildWar );
			g_UserInput.GetHeroMove()->MapChange(DungeonMGR->GetInsDungeonInfoData(MAP->GetMapNum())->dgMapChange);
		}
		--dwCountDownTick;
	}
}
void CDungeonMgr::CancelMoveMap()
{
	if(StoredNpcUniqueIdx==0||
	Storeddestmapnumber==0||
	StoredNpcId==0||
	StoredNpcUniqueIdxT==0)
		return;

	MSG_DWORD4 msg;
	ZeroMemory( &msg, sizeof(msg) );
	msg.Category = MP_DUNGEON;
	msg.Protocol = MP_DUNGEON_PORTAL_ENTRANCE_SYN;
	msg.dwData1 = StoredNpcUniqueIdx;//pNpc->GetNpcUniqueIdx();//MAP->GetMapNum();
	msg.dwData2 = Storeddestmapnumber;
	msg.dwData3 = StoredNpcId;
	msg.dwData4 = StoredNpcUniqueIdxT;
	msg.dwObjectID = HEROID;
	msg.CyptKey = 2;
	NETWORK->Send(&msg,sizeof(msg));
}
void CDungeonMgr::FubenMap(DWORD NpcUniqueIdx,DWORD destmapnumber,DWORD NpcId,DWORD NpcUniqueIdxT)
{
	StoredNpcUniqueIdx=NpcUniqueIdx;
	Storeddestmapnumber=destmapnumber;
	StoredNpcId=NpcId;
	StoredNpcUniqueIdxT=NpcUniqueIdxT;

	MSG_DWORD4 msg;
	ZeroMemory( &msg, sizeof(msg) );
	msg.Category = MP_DUNGEON;
	msg.Protocol = MP_DUNGEON_PORTAL_ENTRANCE_SYN;
	msg.dwData1 = NpcUniqueIdx;//pNpc->GetNpcUniqueIdx();//MAP->GetMapNum();
	msg.dwData2 = destmapnumber;
	msg.dwData3 = NpcId;
	msg.dwData4 = NpcUniqueIdxT;
	msg.dwObjectID = HEROID;
	msg.CyptKey = 0;
	NETWORK->Send(&msg,sizeof(msg));
}
void CDungeonMgr::NetworkMsgParse(BYTE Protocol,void* pMsg)
{
	switch(Protocol)
	{
	case MP_DUNGEON_ENTRANCE_ACK:
		{
			MSG_DWORD5* pmsg = (MSG_DWORD5*)pMsg;

			GAMEIN->GetInsFUBENEntranceInfoDlg()->NpcClick(pmsg->dwData1, pmsg->dwData2, pmsg->dwData3, pmsg->dwData4, pmsg->dwData5, pmsg->CyptKey);

			g_UserInput.SetInputFocus(true);
		}
		break;
	case MP_DUNGEON_ENTRANCE_NACK:
		{
			if( HERO->GetState() == eObjectState_Deal )
				OBJECTSTATEMGR->EndObjectState(HERO, eObjectState_Deal);
			g_UserInput.SetInputFocus(true);
			MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;
			switch(pmsg->dwData)
			{
			case eDungeonERR_NoKeyInfo:
				CHATMGR->AddMsg( CTC_SYSMSG,  CHATMGR->GetChatMsg(2519) );//Only party allow to entering
				break;
			case eDungeonERR_NoParty:
				CHATMGR->AddMsg( CTC_SYSMSG,  CHATMGR->GetChatMsg(2523) );//Only party allow to entering
				break;
			case eDungeonERR_NotEnoughLevel:
				CHATMGR->AddMsg( CTC_SYSMSG,  CHATMGR->GetChatMsg(2515) );//You're not meet the requirement
				break;
			case eDungeonERR_OverLevel:
				CHATMGR->AddMsg( CTC_SYSMSG,  CHATMGR->GetChatMsg(2517) );//Level are restricted
				break;
			case eDungeonERR_EntranceDelay:
				CHATMGR->AddMsg( CTC_SYSMSG,  CHATMGR->GetChatMsg(2516) );//Failed to enter
				break;
			case eDungeonERR_OverCount:
				CHATMGR->AddMsg( CTC_SYSMSG,  CHATMGR->GetChatMsg(1210) );
				break;
			case eDungeonERR_DungeonMapOff:
				CHATMGR->AddMsg( CTC_SYSMSG,  CHATMGR->GetChatMsg(2522) );//Server closed
				break;
			case eDungeonERR_WrongDiffculty:
				CHATMGR->AddMsg( CTC_SYSMSG,  CHATMGR->GetChatMsg(2263) );
				break;
			case eDungeonERR_CreateFailed://eDungeonERR_NotEnoughGold
				CHATMGR->AddMsg( CTC_SYSMSG,  CHATMGR->GetChatMsg(2520) );
				break;
			}
		}
		break;
	case MP_DUNGEON_ZONEBLOCK:
		{
			MSG_DWORD3* pmsg = (MSG_DWORD3*)pMsg;
			ZoneBlock((float)pmsg->dwData1, (float)pmsg->dwData2, (float)pmsg->dwData3);
		}
		break;
	case MP_DUNGEON_ZONEFREE:
		{
			MSG_DWORD3* pmsg = (MSG_DWORD3*)pMsg;
			ZoneFree((float)pmsg->dwData1, (float)pmsg->dwData2, (float)pmsg->dwData3);
		}
		break;
	case MP_DUNGEON_EDGEBLOCK:
		{
			MSG_DWORD3* pmsg = (MSG_DWORD3*)pMsg;
			EdgeBlock((float)pmsg->dwData1, (float)pmsg->dwData2, (float)pmsg->dwData3);
		}
		break;
	case MP_DUNGEON_EDGEFREE:
		{
			MSG_DWORD3* pmsg = (MSG_DWORD3*)pMsg;
			EdgeFree((float)pmsg->dwData1, (float)pmsg->dwData2, (float)pmsg->dwData3);
		}
		break;
	case MP_DUNGEON_FADEMOVE_ACK:
		{
			MSG_DUNGEON_FADE_MOVE* pmsg = (MSG_DUNGEON_FADE_MOVE*)pMsg;
		}
		break;
	case MP_DUNGEON_SETWARP_ACK:
		{
			MSG_DWORD3* pmsg = (MSG_DWORD3*)pMsg;
		}
		break;
	case MP_DUNGEON_SETSWITCH_ACK:
		{
			MSG_DWORD3  * pmsg =( MSG_DWORD3*)pMsg;
			//if(pmsg->dwObjectID==HEROID)
			{
				if(pmsg->CheckSum==0)
				{
//					GAMEIN->GetSignInTimeDialog()->SetInfoMsg(pmsg->CyptKey);
					GAMEIN->GetFBTimeDialog()->SetMonsterCount(0,pmsg->dwData1);
					AUDIOMGR->Play(1326, HERO);
					EFFECTMGR->StartHeroEffectProcess(eEffect_GetItem_Quest);
					CHATMGR->AddMsg(CTC_DUNGEON, CHATMGR->GetChatMsg(2525));
					CHATMGR->AddMsg(CTC_DUNGEON, CHATMGR->GetChatMsg(2526));
				}
				if(pmsg->CheckSum==1)
				{
					int nX=pmsg->dwData1 * 100;
					int nY=pmsg->CyptKey * 100;
					GAMEIN->GetFBTimeDialog()->SetMonsterCount(0,0);
					GAMEIN->GetFBTimeDialog()->AddNpcPortal((float)nX,(float)nY,pmsg->dwData2);
					CHATMGR->AddMsg(CTC_DUNGEON, CHATMGR->GetChatMsg(2525));
					CHATMGR->AddMsg(CTC_DUNGEON, CHATMGR->GetChatMsg(2529));
				}
				//if(pmsg->CheckSum==5)
				//{
				//	GAMEIN->GetInsDGMissionDialog()->SetActive(TRUE);
				//	GAMEIN->GetInsDGMissionDialog()->SetInfoMsg(pmsg->CyptKey-100+10);
				//}
			}
		}
		break;
	case MP_DUNGEON_INFO_ACK://BY JACK //2038 28022022
		{//kill counter
			MSG_DWORD3  * pmsg =( MSG_DWORD3*)pMsg;
			if(pmsg->dwData1==1)
			{
				GAMEIN->GetFBTimeDialog()->AddMonsterCount(pmsg->dwData2);
			}
			if(pmsg->dwData1==2)
			{
				AUDIOMGR->Play(1326, HERO);
				EFFECTMGR->StartHeroEffectProcess(eEffect_GetItem_Quest);
				//HERO->DisableAutoAttack();
				//HERO->SetNextAction(NULL);
				//MOVEMGR->HeroMoveStop();
				//OBJECTSTATEMGR->StartObjectState( HERO, eObjectState_Exit );
				//DWORD dwCheckBit = eInsDungeon;
				sCPlayerAI.SetOpen(false);
				sCPlayerAI.IsRun==false;
				GAMEIN->GetFBTimeDialog()->SetMonsterCount(0,0);
				CHATMGR->AddMsg(CTC_DUNGEON, CHATMGR->GetChatMsg(2527));
				CHATMGR->AddMsg(CTC_DUNGEON, CHATMGR->GetChatMsg(2528));
				StartLeave();
			}
			if(pmsg->dwData1==3)
			{
				if(pmsg->dwObjectID==HEROID)
					GAMEIN->GetFBTimeDialog()->SetFBTime(pmsg->dwData2);
			}
		}
		break;
	case MP_DUNGEON_SENDMSG_SYN:
		{
			MSG_DWORD4  * pmsg =( MSG_DWORD4*)pMsg;
			AUDIOMGR->Play(1326, HERO);
			EFFECTMGR->StartHeroEffectProcess(eEffect_GetItem_Quest);
			CHATMGR->AddMsg(CTC_DUNGEON, CHATMGR->GetChatMsg(2525));
			CHATMGR->AddMsg(CTC_DUNGEON, CHATMGR->GetChatMsg(2526));
			//GAMEIN->GetInsDGMissionDialog()->SetActive(TRUE);
			GAMEIN->GetInsDGMissionDialog()->SetInfoMsg(pmsg->dwData1,pmsg->dwData2);
			GAMEIN->GetFBTimeDialog()->SetMonsterCount(pmsg->dwData3,pmsg->dwData4);
			//GAMEIN->GetFBTimeDialog()->SetFBTime(pmsg->dwData2);
		}
		break;
	default:
		break;
	}
}

void CDungeonMgr::SetSwitchNpc(WORD wIndex, BOOL bActive)
{

}
BOOL CDungeonMgr::IsDungeonMap(MAPTYPE map)
{
	//DWORD dwCheckBit = ;
		return MAP->IsMapKind(eInsDungeon,map);
}
void CDungeonMgr::ZoneBlock(float posx, float posz, float range)
{
	
}
void CDungeonMgr::ZoneFree(float posx, float posz, float range)
{
	
}
void CDungeonMgr::EdgeBlock(float posx, float posz, float range)
{
	
}
void CDungeonMgr::EdgeFree(float posx, float posz, float range)
{
	
}
void CDungeonMgr::ClearCurDungeon()
{
	m_CurDungeon.Clear();
}
void CDungeonMgr::AddEntranceNpc( DWORD dwDungeonKey, DWORD dwNpcJob )
{
	m_mapEntranceNpcJob.insert( std::make_pair( dwNpcJob, dwDungeonKey ) );
}
BOOL CDungeonMgr::IsDungeonEntranceNpc( const DWORD dwNpcJob ) const
{
	return ( m_mapEntranceNpcJob.find( dwNpcJob ) != m_mapEntranceNpcJob.end() );
}
#define FBCHATINFO_TEXTLEN 33
#include "InsFUBENMissionString.h"
#include "..\[CC]Quest\QuestDefines.h"

BOOL CDungeonMgr::LoadFBEntranceCondition()
{
	CMHFile file;
	if (!file.Init(INFB_ENTRANCE_CONDITION, "rb"))
		return FALSE;
	FB_MAP_INFO * MapInfo = NULL;
	while(1)
	{
		if(file.IsEOF())
			break;
		MapInfo = new FB_MAP_INFO;
		MapInfo->MapCurMap = file.GetByte();
		MapInfo->MapIDX = file.GetWord();		
		MapInfo->MinLevel = file.GetWord();
		MapInfo->MaxLevel = file.GetWord();
		MapInfo->FubenKey = file.GetByte();
		MapInfo->FubenFee = file.GetInt();
//		MapInfo->SoloMap = file.GetByte();
		m_FBEntrance.Add(MapInfo,MapInfo->MapIDX);
		MapInfo = NULL;
	}
	file.Release();
	return TRUE;
}
FB_MAP_INFO * CDungeonMgr::GetFBEntranceData(DWORD Flag)
{
	return m_FBEntrance.GetData(Flag);
}
BYTE CDungeonMgr::GetFBEntranceMap(DWORD MapNum)
{
	return GetFBEntranceData(MapNum)->MapCurMap;
}
WORD CDungeonMgr::GetFBMapChangeIdx(DWORD MapNum)
{
	return GetFBEntranceData(MapNum)->MapIDX;
}
void CDungeonMgr::LoadToolTipList()
{
	CMHFile file;
	if (!file.Init(INFB_MISSION_STRING, "rb")) return;
		
	char buf[512];
	char buf2[512]={0,};
	DUNGEON_TEXT* pToolTipText;
	DUNGEON_TEXT* pTemp;

	int nCpyNum = 0;
	int nLen;
	char* str;

	DWORD Key=0;
	DWORD Key_1=0;
	DWORD Key_2=0;

	while( 1 )
	{
		if( file.IsEOF() ) break;
			pToolTipText = NULL;
		//file.GetString(buf);
		//START_STRING_COMPARE(buf)

		//COMPARE("$MISSIONSTR")
		SafeStrCpy( buf, file.GetString(), 512 );
		if( strcmp( buf, "$MISSIONSTR" ) == 0 )
		{
			Key_1 = file.GetDword();
			Key_2 = file.GetDword();
			COMBINEKEY(Key_1, Key_2, Key);
			SafeStrCpy( buf, file.GetString(), 512 );

			if( strcmp( buf, "{" ) == 0 )
			{			
				file.GetLine( buf, 512 );
				str = buf;
				nLen = strlen( buf );
				while( *buf != '}' )
				{

					//COMPARE("#DESC")
					SafeStrCpy( buf, file.GetString(), 512 );
					if( strcmp( buf, "#DESC" ) == 0 )
					{
						SafeStrCpy( buf, file.GetString(), 512 );		
							
						if( strcmp( buf, "{" ) == 0 )
						{			
							file.GetLine( buf, 512 );
							str = buf;
							nLen = strlen( buf );
							while( *buf != '}' )
							{						
								while( nLen > FBCHATINFO_TEXTLEN )
								{
									if( ( str + FBCHATINFO_TEXTLEN ) != CharNext( CharPrev( str, str + FBCHATINFO_TEXTLEN ) ) )
									{
										nCpyNum = FBCHATINFO_TEXTLEN - 1;
									}
									else
									{
										nCpyNum = FBCHATINFO_TEXTLEN;
									}
									pTemp = new DUNGEON_TEXT;
									pTemp->strToolTip = new char[nCpyNum+1];
									strncpy( pTemp->strToolTip, str, nCpyNum );
									pTemp->strToolTip[nCpyNum] = 0;
									nLen -= nCpyNum;
									str += nCpyNum;
									if( *str == '  ' ) ++str;
									if( pToolTipText )
									{
										pToolTipText->pNext = pTemp;
									}
									else
									{
										m_FBTipTextList.Add( pTemp, Key );
									}
									pToolTipText = pTemp;
								}
								if( nLen > 0 )
								{
									pTemp = new DUNGEON_TEXT;
									pTemp->strToolTip = new char[nLen+1];
									strncpy( pTemp->strToolTip, str, nLen );
									pTemp->strToolTip[nLen] = 0;
									if( pToolTipText )
									{
										pToolTipText->pNext = pTemp;
									}
									else
									{
										m_FBTipTextList.Add( pTemp, Key );
									}
									pToolTipText = pTemp;
								}
								file.GetLine( buf, 512 );
								str = buf;
								nLen = strlen( buf );

								
								//break;
							}
							if( strcmp( buf, "}" ) == 0 )
							{
								//file.GetLine( buf, 512 );
								str = buf;
								nLen = strlen( buf )-1;
							}
							
							//END_COMPARE
						}
					}
				//	file.GetLine( buf, 512 );
				//	str = buf;
				//	nLen = strlen( buf );
				}
				
			}
		}
	}
	file.Release();
}
DUNGEON_TEXT* CDungeonMgr::GetToolTipInfo( WORD wIdx )
{
	return m_FBTipTextList.GetData( wIdx );
}



//-----------------------------------FROM FUBENMANAGER.CPP
BOOL CDungeonMgr::LoadFubenString(char* strFileName)
{
	CMHFile file;
	if( !file.Init( strFileName, "rb") )
		return FALSE;
	char Token[256];
	DWORD Key=0;
	DWORD Key_1=0;
	DWORD Key_2=0;
	CFubenString* pFubenString = NULL;
	while( !file.IsEOF() )
	{
		file.GetString(Token);
		if(strcmp(Token,"$MISSIONSTR") == 0)	
		{
			Key_1 = file.GetDword();
			Key_2 = file.GetDword();
			COMBINEKEY(Key_1, Key_2, Key);
			pFubenString = CFubenString::LoadFubenString(&file);
			if(pFubenString)
			{
				m_FubenStringTable.Add(pFubenString, Key);
				pFubenString->SetIndex(Key_1, Key_2);				
			}
		}
	}
	return TRUE;
}
void CDungeonMgr::AddStringTest()
{
	CFubenString* pFubenString = NULL;
}
char* CDungeonMgr::GetFubenTitle( DWORD dwFubenIdx , DWORD dwSub)
{
	int Key = 0;
	COMBINEKEY( dwFubenIdx, dwSub, Key );
	CFubenString* pQString = m_FubenStringTable.GetData(Key);
	if( pQString == NULL )	return NULL;
	cPtrList* p = pQString->GetTitle();
	QString* string = (QString*)p->GetHead();
	//cPtrList* pDesc = pQString->GetDesc();
	//QString* stringDesc = (QString*)pDesc->AddTail(&p);
	return string->Str;
}
char* CDungeonMgr::GetFubenDesc( DWORD dwFubenIdx , DWORD dwSub)
{
	int Key = 0;
	COMBINEKEY( dwFubenIdx, dwSub, Key );
	CFubenString* pQString = m_FubenStringTable.GetData(Key);
	if( pQString == NULL )	return NULL;
	cPtrList* p = pQString->GetDesc();
	QString* string = (QString*)p->GetHeadPosition();
	PTRLISTPOS pos = NULL;
	pos=(PTRLISTPOS)string;
	QString* stringDesc = (QString*)p->GetNext(pos);
	return stringDesc->Str;
}
char* CDungeonMgr::GetFubenDesc2( DWORD dwFubenIdx , DWORD dwSub)
{
	int Key = 0;
	COMBINEKEY( dwFubenIdx, dwSub, Key );
	CFubenString* pQString = m_FubenStringTable.GetData(Key);
	if( pQString == NULL )	return NULL;
	cPtrList* p = pQString->GetDesc();
	QString* string = (QString*)p->GetHead();
	return string->Str;
}
//--------------------------------------------------------








BOOL CDungeonMgr::LoadInsDungeonInfo()
{
	if( IsDungeonMap( MAP->GetMapNum() ) == FALSE ) return FALSE;		

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
InsDGInfo * CDungeonMgr::GetInsDungeonInfoData(int dwMapNum)
{
	return m_InsDGInfo.GetData(dwMapNum);
}














void CDungeonMgr::LoadTestNewBinTest()
{
	
}




void CDungeonMgr::TestLoadInfo()
{
	
}
DUNGEON_TEXT* CDungeonMgr::GetFBTipInfo( WORD wIdx )
{
	return m_FBTipTextList.GetData( wIdx );
}
char* CDungeonMgr::CheckFBTipInfo(DWORD MapNum)
{
	return GetFBTipInfo(MapNum)->strToolTip;
}