



// Player.cpp: implementation of the CPlayer class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Player.h"
#include "Monster.h"
#include "StatsCalcManager.h"
#include "PartyManager.h"
#include "Party.h"
#include "CharMove.h"
#include "PackedData.h"

#include "..\[CC]Header\GameResourceManager.h"
#include "MapDBMsgParser.h"
#include "ItemManager.h"
#include "SkillManager_Server.h"
#include "KyungGongManager.h"
#include "ObjectEvent.h"
#include "ObjectStateManager.h"
#include "streetstallmanager.h"
#include "PeaceWarModManager.h"
#include "AbilityManager.h"
#include "CharacterCalcManager.h"
#include "Wantedmanager.h"
#include "GuildManager.h"
#include "MugongManager.h"
#include "VimuStreet/Battle_Vimustreet.h"
#include "StateMachinen.h"
#include "MHError.h"

#include "CQuestBase.h"
#include "Quest.h"

#include "TileManager.h"
#include "..\[CC]Header\CommonCalcFunc.h"
#include "mhtimemanager.h"

#include "LootingManager.h"
#include "PKManager.h"
#include "QuestManager.h"
#include "PyogukManager.h"
#include "FameManager.h"
#include "EventMapMgr.h"
#include "UserTable.h"
#include "GuildFieldWarMgr.h"
#include "PartyWarMgr.h"
#include "QuestMapMgr.h"
#include "GuildTournamentMgr.h"
#include "SiegeWarMgr.h"
#include "SiegeWarProfitMgr.h"
#include "StreetStall.h"
#include "Pet.h"
#include "PetManager.h"
#include "ServerSystem.h"
#include "SurvivalModeManager.h"
#include "Titan.h"
#include "ChannelSystem.h"

// Weather System
#include "WeatherManager.h"
#include "GameEventManager.h"
#include "FortWarManager.h"

#include "MapNetworkMsgParser.h" // include for mallmoney process! 2014-05-30
#include <time.h>

#include "../[CC]BattleSystem/BattleSystem_Server.h"
#include "../[CC]Suryun/SuryunManager_Server.h"
#include "GuildWarManager.h"

#include "Dungeon/DungeonMgr.h"

#include "LoginEventManager.h"
//#include "FBTimerManager.h"
#define INSERTLOG_TIME			600000 // 10 min
#define INSERTLOG_TIME_CHINA	1800000 // 30 min
#define INSERTLOG_TIME_HK		1800000 // 30 min

extern int	g_nHackCheckNum;
extern int	g_nHackCheckWriteNum;
extern int	g_nServerSetNum;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
ITEM_RARE_OPTION_INFO CPlayer::m_NULLRAREINFO;
ITEM_OPTION_INFO CPlayer::NULLOPTIONINFO;
ITEM_STONE_OPTION_INFO CPlayer::NULLSTONEINFO;
CPlayer::CPlayer()
{
	m_AbilityGroup.SetOwenerObject(this);
	m_SkillList.Initialize(10);
	m_InsDGRankInfoTable.Initialize(50);
	m_SkillFailCount = 0;
	memset(m_szWantMarryName, 0, MAX_NAME_LENGTH + 1);

	m_KillExpFlag=0;

	//m_EventIDFlag=1;//kiv
	bFirstEvent=TRUE;

//--------------------------------------------
	m_FBTimerMonsterNum=0;
//--------------------------------------------
	m_FBMapArea=0;
	m_FBCreate=FALSE;
	m_FBTimerMapType=0;

	pDungeonFinishSwitch=false;
}

CPlayer::~CPlayer()
{
	m_SkillList.RemoveAll();
}

void CPlayer::InitClearData()
{
	m_ItemContainer.Init();
	m_ItemContainer.SetInit(eItemTable_Inventory,	TP_INVENTORY_START,		SLOT_INVENTORY_NUM,		&m_InventorySlot);
	m_ItemContainer.SetInit(eItemTable_Weared,		TP_WEAR_START,			SLOT_WEAR_NUM,			&m_WearSlot);
	m_ItemContainer.SetInit(eItemTable_Pyoguk,		TP_PYOGUK_START,		SLOT_PYOGUK_NUM,		&m_PyogukSlot);
	m_ItemContainer.SetInit(eItemTable_Shop,		TP_SHOPITEM_START,		SLOT_SHOPITEM_NUM,		&m_ShopItemSlot);	
	//m_ItemContainer.SetInit(eItemTable_ShopInven,	TP_SHOPINVEN_START,		SLOT_SHOPINVEN_NUM/*/2*/,	&m_ShopInvenSlot);
	m_ItemContainer.SetInit(eItemTable_ShopInven,	TP_SHOPINVEN_START,		SLOT_SHOPINVEN_NUM+TABCELL_SHOPINVEN_PLUS_NUM,	&m_ShopInvenSlot);
	//SW051129 Pet
	m_ItemContainer.SetInit(eItemTable_PetInven,	TP_PETINVEN_START,		SLOT_PETINVEN_NUM,		&m_PetInvenSlot);
	m_ItemContainer.SetInit(eItemTable_PetWeared,	TP_PETWEAR_START,		SLOT_PETWEAR_NUM,		&m_PetWearSlot);
	// magi82 - Titan(070207)
	m_ItemContainer.SetInit(eItemTable_Titan,		TP_TITANWEAR_START,		SLOT_TITANWEAR_NUM,		&m_TitanWearSlot);
	//////////////////////////////////////////////////////////////////////////
	// magi82 - Titan(070227)
	m_ItemContainer.SetInit(eItemTable_TitanShopItem,		TP_TITANSHOPITEM_START,		SLOT_TITANSHOPITEM_NUM,		&m_TitanShopItemSlot);
	//////////////////////////////////////////////////////////////////////////

	// member data init
	memset(&m_HeroCharacterInfo,0,sizeof(CHARACTER_TOTALINFO));
	memset(&m_HeroInfo,0,sizeof(HERO_TOTALINFO));
	memset(&m_HeroMugongInfo, 0, sizeof(MUGONG_TOTALINFO));
	memset(&m_itemStats,0,sizeof(m_itemStats));
	memset(&m_setItemStats,0,sizeof(m_setItemStats));		// 2007. 6. 12. CBH - ¼¼Æ®¾ÆÀÌÅÆ ´É·ÂÄ¡ ±¸Á¶Ã¼ ÃÊ±âÈ­ Ãß°¡.
	// RaMa - 04.11.08 ( ShopItemÀ¸·Î Ãß°¡µÇ´Â ¿É¼Ç )
	memset(&m_ShopItemOption,0,sizeof(SHOPITEMOPTION));
	for(int i=eAvatar_Weared_Hair; i<eAvatar_Max; i++)
		m_ShopItemOption.Avatar[i]=1;
	memset(&m_AvatarOption, 0 , sizeof(AVATARITEMOPTION));
	
	//////////////////////////////////////////////////////////////////////////
	// 06. 06. 2Â÷ ÀüÁ÷ - ÀÌ¿µÁØ
	// ¹«°ø º¯È¯ Ãß°¡
	memset(&m_SkillStatsOption, 0, sizeof(SKILLSTATSOPTION));
	memset(&(m_BaseObjectInfo.SingleSpecialState), 0, sizeof(bool) * eSingleSpecialState_Max);
	//////////////////////////////////////////////////////////////////////////

	m_ItemArrayList.Initialize(10);//10 //kiv
	m_FollowMonsterList.Initialize(5);
	m_ItemRareOptionList.Initialize(MAX_ITEM_OPTION_NUM);
	m_ItemOptionList.Initialize(MAX_ITEM_OPTION_NUM);
	m_ItemStoneOptionList.Initialize(MAX_ITEM_OPTION_NUM);
	m_QuestList.Initialize(30);
	m_QuestGroup.Initialize( this );
	m_ShopItemManager.Init( this );
	m_PetManager.Init( this );
	m_TitanManager.Init( this );
	m_InitPosition = 0;
	m_bSavePointInited = FALSE;
	m_bPetInvenInit = FALSE;

	//m_pCurPet = NULL;
	m_wKillMonsterLevel = 0;

	// 06. 03 ±¹³»¹«½Ö - ÀÌ¿µÁØ
#ifndef _JAPAN_LOCAL_
	m_dwMussangTime = 0;
	m_wMussangStat = 0;
#endif

#ifdef _JAPAN_LOCAL_
	m_dwCharacterSlotNum = 0;
#endif
#ifdef _HK_LOCAL_
	m_dwCharacterSlotNum = 0;
#endif
#ifdef _TL_LOCAL_
	m_dwCharacterSlotNum = 0;
#endif


	m_bGotWarehouseItems = FALSE;
	m_SkillFailCount = 0;
	m_FullMoonEventHCt = 0;
	m_fFullMoonEventTime = 0.f;
	m_LastActionTime = 0;
	m_bActionPanelty = TRUE;

	m_dwAutoNoteAskPlayerIdx = 0;
	m_dwLastAttackTimeForAutoNote = 0;
    // the last paodian time default set !2014-05-30
	m_LastPaoDianTime = time(NULL);
	m_EventCountDownTimer= time(NULL);
	m_LastDungeonTime = time(NULL);
	// Í¶×¢ÐÅÏ¢ 2015-01-28
    memset(&m_GambleInfo, 0, sizeof(GAMBLE_USER_INFO));

	this->m_LastRedEgeTime=time(NULL)-1;
	this->m_LastYellowEgeTime=time(NULL)-1;
	this->m_LastBlueEgeTime=time(NULL)-1;

	this->m_LastRecvPingSYNTime=0;
	this->m_PingSYNFastCount=0;

	m_KilledPlayerID = 0;
	m_LootingPos = 0;

	//m_EventCountDownTimer = time(NULL);
	pGuildWarDieTime = time(NULL);

	m_EventIDFlag=1;
	bFirstEvent=TRUE;

	pDungeonFinishSwitch=false;

	m_MovePackTime = 0;//kiv

	ForbidChatTime = 0;

	memset(&m_RankState, 0, sizeof(m_RankState));
}

BOOL CPlayer::Init(EObjectKind kind,DWORD AgentNum, BASEOBJECT_INFO* pBaseObjectInfo)
{
	m_ItemGrowCount=0;  // 2015-01-15  

	m_DelayGroup.Init();
	m_StreetStallTitle[0] = 0;
	m_bPetInvenInit = FALSE;
	m_bPyogukInit = 0;
	m_bShopInit = FALSE;
	m_WantedIDX = 0;
	m_MurdererIDX = 0;
	m_MurdererKind = 0;
	m_MurderObjKind = 0;
	m_bPenaltyByDie = FALSE;

	m_bReadyToRevive = FALSE;
	m_bSavePointInited = FALSE;
//KES VIMU 031101
	m_bVimuing		= FALSE;
//KES EXIT
	m_bExit			= FALSE;
	m_bNormalExit	= FALSE;
//
	CObject::Init(kind, AgentNum, pBaseObjectInfo); //¢¯¨Ï¡¾a¨ù¡©ùéA eObjectState_NoneA¢¬¡¤I ¢¬¢¬¥ìcùéU.

//KES 040827
	OBJECTSTATEMGR_OBJ->StartObjectState( this, eObjectState_Immortal, 0 );
	// 06.08.29. RaMa.
#ifdef _HK_LOCAL_
	OBJECTSTATEMGR_OBJ->EndObjectState( this, eObjectState_Immortal, 60000 );
#else
	OBJECTSTATEMGR_OBJ->EndObjectState( this, eObjectState_Immortal, 30000 );
#endif // _HK_LOCAL_
//

	m_CurComboNum = 0;
	m_ChangeNpcIndex = 0;
	SetGravity(0);
	m_pGuetStall = NULL;

//KES
	m_bNeedRevive = TRUE;
	ZeroMemory( &m_GameOption, sizeof( m_GameOption ) );
//
	//m_pItemContainer = new CItemContainer;


//	if( !m_pQuestGroup )
//		m_pQuestGroup = new CQuestGroup(this);

	m_LoginTime = 0;
	m_ContinuePlayTime = 0;

//HACK CHECK INIT
	m_nHackCount = 0;
	m_dwHackStartTime = gCurTime;
	m_dwSkillDelayStartTime = gCurTime;	
	m_dwLastSkillAnimationTime = 0;
	m_dwLastSocietyAction = 0;
//

	m_wKillMonsterLevel = 0;

	m_ReviveTime = 0;

	m_bDieForBattleChannel	= FALSE;
	m_bDieForGFW = FALSE;
	m_bDieInSpecialMap = FALSE;
	
	//SW051112 ¹«½Ö¸ðµå
	InitMussangPoint();
	m_dwMussangStartTime = 0;
	m_bIsMussangReady = FALSE;
	m_bMussangFlag = 0;

	//m_pCurPet	= NULL;
/*
#ifdef _JAPAN_LOCAL_
	m_bPartyHelp = FALSE;
#endif
*/
	m_bWaitExitPlayer = TRUE;

	// 06. 03 ±¹³»¹«½Ö - ÀÌ¿µÁØ
#ifndef _JAPAN_LOCAL_
	m_dwMussangTime = 0;
	m_wMussangStat = 0;
#endif

	m_bWaitExitPlayer = TRUE;

	//////////////////////////////////////////////////////////////////////////
	// 06. 06. 2Â÷ ÀüÁ÷ - ÀÌ¿µÁØ
	// ¹«°ø º¯È¯ Ãß°¡
	memset(&m_SkillStatsOption, 0, sizeof(SKILLSTATSOPTION));
	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	// 06. 06. 2Â÷ ÀüÁ÷ - ÀÌ¿µÁØ
	// Àº½Å/Çý¾È
	memset(&(m_BaseObjectInfo.SingleSpecialState), 0, sizeof(bool) * eSingleSpecialState_Max);
	//////////////////////////////////////////////////////////////////////////

#ifdef _JAPAN_LOCAL_
	m_dwCharacterSlotNum = 0;
#endif

#ifdef _HK_LOCAL_
	m_dwCharacterSlotNum = 0;
#endif

#ifdef _TL_LOCAL_
	m_dwCharacterSlotNum = 0;
#endif

	m_SkillList.RemoveAll();
	m_SkillFailCount = 0;
	m_FullMoonEventHCt = 0;
	m_fFullMoonEventTime = 0.f;
	m_dwHpMpBitFlag = 0x00;	// magi82	°æÇèÄ¡¸¶´Ù »óÅÂÈ¸º¹ °ü·Ã ÇÃ·¡±× ÃÊ±âÈ­

	m_CristmasEventSnowIdx = 0;

	//weiye 2017-12-07 Ä¬ÈÏ°²È«ËøÊÇËø¶¨×´Ì¬

	m_IsUnLock=FALSE;

	//weiye 2018-02-06 ¸±±¾Ê±¼ä³õÊ¼»¯

	InitSkinDelay();

	// the last paodian time default set !2014-05-30
	m_LastPaoDianTime = time(NULL);
	m_EventCountDownTimer= time(NULL);
	m_LastDungeonTime = time(NULL);

	m_LastMapTime = time(NULL);

	//SetLastMapTime(0);

	this->m_LastRedEgeTime=time(NULL)-1;
	this->m_LastYellowEgeTime=time(NULL)-1;
	this->m_LastBlueEgeTime=time(NULL)-1;

	this->m_LastRecvPingSYNTime=0;
	this->m_PingSYNFastCount=0;

	m_KilledPlayerID = 0;
	m_LootingPos = 0;

	//m_EventCountDownTimer = time(NULL);

	pGuildWarDieTime  = time(NULL);

	//SetLastMapTime(0);

//-----------------------------------------------------
	//m_FBTime=0;//kiv
	m_FBTimerMonsterNum=0;
//-----------------------------------------------------
	m_FBMapArea=0;
	m_FBCreate=FALSE;
	m_FBTimerMapType=0;


	m_MovePackTime = 0;//kiv

	return TRUE;
}
void CPlayer::Release()
{
	//SAFE_DELETE(m_pItemContainer);
	m_DelayGroup.Release();
	m_AbilityGroup.Release();
	m_ItemContainer.Release();


	ITEMOBTAINARRAYINFO* pInfo;
	m_ItemArrayList.SetPositionHead();
	while(pInfo = m_ItemArrayList.GetData())
	{
		ITEMMGR->Free(this, pInfo);
	}
	m_ItemArrayList.RemoveAll();

	m_FollowMonsterList.RemoveAll();

	ITEM_RARE_OPTION_INFO* pRInfo = NULL;
	m_ItemRareOptionList.SetPositionHead();
	while(pRInfo = m_ItemRareOptionList.GetData())
		delete pRInfo;
	m_ItemRareOptionList.RemoveAll();
	
	ITEM_OPTION_INFO * pOInfo = NULL;
	m_ItemOptionList.SetPositionHead();
	while(pOInfo = m_ItemOptionList.GetData())
		delete pOInfo;
	m_ItemOptionList.RemoveAll();


    // 2014-11-14 ÐÂ·þÎñÆ÷Á´±íÊÍ·Å!
	ITEM_STONE_OPTION_INFO * pStInfo = NULL;
	m_ItemStoneOptionList.SetPositionHead();
	while(pStInfo = m_ItemStoneOptionList.GetData())
		delete pStInfo;
	m_ItemStoneOptionList.RemoveAll();
	
//
	CQuestBase* pQuest;
	m_QuestList.SetPositionHead();
	while(pQuest = m_QuestList.GetData())
	{
		delete pQuest;
	}
	m_QuestList.RemoveAll();


	m_InventoryPurse.Release();
	m_PyogukPurse.Release();

	m_WantedList.RemoveAll();

	m_QuestGroup.Release();

	m_ShopItemManager.Release();

	m_PetManager.Release();

	m_TitanManager.Release();
	
	CObject::Release();


//	if( m_pQuestGroup )
//	{
//		delete m_pQuestGroup;
//		m_pQuestGroup = NULL;
//	}
	m_SkillList.RemoveAll();
	m_SkillFailCount = 0;
}

//SW050920 Rare
void CPlayer::AddItemRareOption(ITEM_RARE_OPTION_INFO* pInfo)
{
	if( pInfo == 0 ) return;

	if( m_ItemRareOptionList.GetData(pInfo->dwRareOptionIdx) ) return;
	
	ITEM_RARE_OPTION_INFO* pNewRare = new ITEM_RARE_OPTION_INFO;
	memcpy(pNewRare, pInfo, sizeof(ITEM_RARE_OPTION_INFO));
	m_ItemRareOptionList.Add(pNewRare, pNewRare->dwRareOptionIdx);
}

BOOL CPlayer::RemoveItemRareOption(DWORD dwRareIdx)
{
	ASSERT(m_ItemRareOptionList.GetData(dwRareIdx));
// 	ITEM_RARE_OPTION_INFO* pRareInfo = m_ItemRareOptionList.GetData(dwRareIdx);
// 	SAFE_DELETE( pRareInfo );
// 	m_ItemRareOptionList.Remove(dwRareIdx);

	//SW060203 Assert ÅðÄ¡!
	ITEM_RARE_OPTION_INFO* pRareInfo = NULL;
	if( pRareInfo = m_ItemRareOptionList.GetData(dwRareIdx) )
	{
		SAFE_DELETE( pRareInfo );
		m_ItemRareOptionList.Remove(dwRareIdx);
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}
	
ITEM_RARE_OPTION_INFO* CPlayer::GetItemRareOption(DWORD dwRareIdx)
{
	ITEM_RARE_OPTION_INFO* pInfo = m_ItemRareOptionList.GetData(dwRareIdx);
	if( pInfo )
		return pInfo;

	static ITEM_RARE_OPTION_INFO NULLINFO;
	memset(&NULLINFO,0,sizeof(ITEM_RARE_OPTION_INFO));
	return &NULLINFO;
}

void CPlayer::AddItemOption(ITEM_OPTION_INFO * pInfo)
{
	if(pInfo == 0)
	{
		return;
	}
	if(m_ItemOptionList.GetData(pInfo->dwOptionIdx))
	{
		return;
	}
	ITEM_OPTION_INFO * pNewInfo = new ITEM_OPTION_INFO;
	memcpy(pNewInfo, pInfo, sizeof(ITEM_OPTION_INFO));
	m_ItemOptionList.Add(pNewInfo, pNewInfo->dwOptionIdx);
}

BOOL CPlayer::RemoveItemOption(DWORD dwOptionIdx)
{
	ASSERT(m_ItemOptionList.GetData(dwOptionIdx));
	ITEM_OPTION_INFO * pNewInfo = m_ItemOptionList.GetData(dwOptionIdx);

// 	if( pNewInfo )
// 	{
// 		delete pNewInfo;
// 		m_ItemOptionList.Remove(dwOptionIdx);
// 	}
// 	else
// 	{
// 		char buf[128];
// 		sprintf( buf, "RemoveError-OptionIdx: %d", dwOptionIdx );
// 		ASSERTMSG( 0, buf );
// 	}

	//SW060203 Assert ÅðÄ¡!
	if( pNewInfo )
	{
		SAFE_DELETE(pNewInfo);
		m_ItemOptionList.Remove(dwOptionIdx);
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

ITEM_OPTION_INFO * CPlayer::GetItemOption(DWORD dwOptionIdx)
{
	ITEM_OPTION_INFO* pInfo = m_ItemOptionList.GetData(dwOptionIdx);
	if(pInfo)
		return pInfo;

	static ITEM_OPTION_INFO NULLINFO;
	memset(&NULLINFO,0,sizeof(ITEM_OPTION_INFO));
	return &NULLINFO;
}


// 2014-11-14 ÐÂ£¬Ìí¼ÓÐÅÏ¢µ½½ÇÉ«ÐÅÏ¢

void CPlayer::AddItemStoneOption(ITEM_STONE_OPTION_INFO * pInfo)
{
	if(pInfo == 0)
	{
		return;
	}
	if(m_ItemStoneOptionList.GetData(pInfo->dwStoneOptionIdx))
	{
		m_ItemStoneOptionList.Remove(pInfo->dwStoneOptionIdx); // ¸üÐÂÐÅÏ¢ 2014-11-15
	}
	ITEM_STONE_OPTION_INFO * pNewInfo = new ITEM_STONE_OPTION_INFO;
	memcpy(pNewInfo, pInfo, sizeof(ITEM_STONE_OPTION_INFO));
	m_ItemStoneOptionList.Add(pNewInfo, pNewInfo->dwStoneOptionIdx);
}
// 2014-11-14 ÐÂ£¬É¾³ýÐÅÏ¢µ½½ÇÉ«
BOOL CPlayer::RemoveItemStoneOption(DWORD dwStoneIdx)
{
	ASSERT(m_ItemStoneOptionList.GetData(dwStoneIdx));

	ITEM_STONE_OPTION_INFO * pNewInfo = m_ItemStoneOptionList.GetData(dwStoneIdx);

	if( pNewInfo )
	{
		SAFE_DELETE(pNewInfo);
		m_ItemStoneOptionList.Remove(dwStoneIdx);
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}
// 2014-11-14 ÐÂ£¬»ñÈ¡ÐÅÏ¢µ½½ÇÉ«
ITEM_STONE_OPTION_INFO * CPlayer::GetItemStoneOption(DWORD dwStoneIdx)
{
	ITEM_STONE_OPTION_INFO* pInfo = m_ItemStoneOptionList.GetData(dwStoneIdx);

	if(pInfo)
	{
		return pInfo;
	}
	else
	{
		return NULL;
	}

}

DWORD CPlayer::GetGravity()
{
	DWORD totalG = 0;
	CMonster * pObject;
	m_FollowMonsterList.SetPositionHead();
	while(pObject = m_FollowMonsterList.GetData())
	{
//		if( pObject->GetLastAttackPlayer() != this )//³»°¡ °ø°ÝÇÑ ¸ó½ºÅÍ°¡ ¾Æ´Ñ°æ¿ì¸¸
			totalG += pObject->GetGravity();
	}
//	ASSERT(totalG <= 100);
	return totalG;
}

BOOL CPlayer::AddFollowList(CMonster * pMob)
{	
//	ASSERT(m_FollowMonsterList.GetData(pMob->GetID()) == NULL);
//	if(m_FollowMonsterList.GetData(pMob->GetID()) != NULL)
//	{
//		MHERROR->OutputFile("Debug.txt", MHERROR->GetStringArg("addfollow pID:%d,mID:%d,pstate:%d,mstate:%d,pG:%d,mobG:%d", 
//										GetID(),
//										pMob->GetID(), 
//										GetState(), 
//										pMob->GetState(),
//										GetGravity(), 
//										pMob->GetGravity()));
//	}
//KES ¸ÕÀú ³Ö°í! ³ªÁß¿¡ ºñÁßÀ» °è»êÇÏµµ·Ï º¯°æÇÏÀÚ.
//	if( GetGravity() + pMob->GetGravity() <= 100 )
	if( m_FollowMonsterList.GetDataNum() < 50 )		//max 50¸¶¸®
	{
		m_FollowMonsterList.Add(pMob, pMob->GetID());
		return TRUE;
	}
	else
	{
		return FALSE;
	}
//	else
//	{
//		MHERROR->OutputFile("Debug.txt", MHERROR->GetStringArg("Addfollow 100 over pID:%d,mID:%d,pstate:%d,mstate:%d,pG:%d,mobG:%d", 
//										GetID(),
//										pMob->GetID(), 
//										GetState(), 
//										pMob->GetState(),
//										GetGravity(), 
//										pMob->GetGravity()));
//	}
}
BOOL CPlayer::RemoveFollowAsFarAs(DWORD GAmount, CObject* pMe )
{	
	VECTOR3 * ObjectPos	= CCharMove::GetPosition(this);
	BOOL bMe = FALSE;

//	while(GAmount > 0)
	while(GAmount > 100)
	{	
		CMonster * pObject = NULL;
		CMonster * MaxObject = NULL;
		float	MaxDistance	= -1;
		float	Distance	= 0;

		m_FollowMonsterList.SetPositionHead();
		while(pObject = m_FollowMonsterList.GetData())
		{
//			if( pObject->GetLastAttackPlayer() == this )
//				continue;
			VECTOR3 * TObjectPos = CCharMove::GetPosition(pObject);
			if((Distance = CalcDistanceXZ( ObjectPos, TObjectPos )) > MaxDistance)
			{
				MaxDistance = Distance;
				MaxObject = pObject;
			}
		}
		if(MaxObject)
		{
			if(GAmount > MaxObject->GetGravity())
				GAmount -= MaxObject->GetGravity();
			else
				GAmount = 0;
			MaxObject->SetTObject(NULL);
//			RemoveFollowList(MaxObject->GetID());
			GSTATEMACHINE.SetState(MaxObject, eMA_WALKAROUND);
//			MaxObject->SetState(eMA_WALKAROUND)
			if( pMe == MaxObject )
				bMe = TRUE;
		}
		else
		{
			MHERROR->OutputFile("Debug.txt", MHERROR->GetStringArg("amount != 0"));
			GAmount = 0;
		}
	}

	return bMe;	
}

void CPlayer::RemoveFollowList(DWORD ID)
{
	m_FollowMonsterList.Remove(ID);
//	int a = m_FollowMonsterList.GetDataNum();
}

ITEMOBTAINARRAYINFO * CPlayer::GetArray(WORD id)
{
	return m_ItemArrayList.GetData(id);
}
void CPlayer::AddArray(ITEMOBTAINARRAYINFO * pInfo)
{
	m_ItemArrayList.Add(pInfo, pInfo->wObtainArrayID);
}
void CPlayer::RemoveArray(DWORD id)
{
	m_ItemArrayList.Remove(id);
}
void CPlayer::InitCharacterTotalInfo(CHARACTER_TOTALINFO* pTotalInfo)
{
	memcpy(&m_HeroCharacterInfo,pTotalInfo,sizeof(CHARACTER_TOTALINFO));
	if( GetUserLevel() == eUSERLEVEL_GM )
		m_HeroCharacterInfo.bVisible = FALSE;
	else
		m_HeroCharacterInfo.bVisible = TRUE;


}

void CPlayer::InitHeroTotalInfo(HERO_TOTALINFO* pHeroInfo)
{
	memcpy(&m_HeroInfo,pHeroInfo,sizeof(HERO_TOTALINFO));

	m_HeroInfo.MaxMussangPoint = GetMussangMaxPoint();
	/* Money Setting */
	m_ItemContainer.GetSlot(eItemTable_Inventory)->CreatePurse(&m_InventoryPurse, this, m_HeroInfo.Money, MAX_INVENTORY_MONEY);
	/* BadFame Reduce Time Init*/
	m_dwLastReduceHour = ( m_HeroInfo.Playtime - m_HeroInfo.LastPKModeEndTime ) / 3600; //¨öA¡Æ¡Ì
}

void CPlayer::InitMugongTotalInfo(MUGONG_TOTALINFO* pMugongInfo)
{
	memcpy(&m_HeroMugongInfo,pMugongInfo,sizeof(MUGONG_TOTALINFO));
}

void CPlayer::InitItemTotalInfo(ITEM_TOTALINFO* pItemInfo)
{
	//m_pIconTable[eItemTable_Inventory]->Init(MAX_INVENTORY_NUM, pItemInfo->Inventory, GetID(), INVENTORY_STARTPOSITION );
	//m_pIconTable[eItemTable_Weared]->Init(eWearedItem_Max, pItemInfo->WearedItem, GetID(), WEAR_STARTPOSITION);
	//m_pIconTable[eItemTable_StreetStall]->Init(MAX_STREETSTALL_ITEMNUM, pStallItem, GetID(), STALL_STARTPOSITION);

	m_ItemContainer.GetSlot(eItemTable_Inventory)->SetItemInfoAll(pItemInfo->Inventory);	
	m_ItemContainer.GetSlot(eItemTable_Weared)->SetItemInfoAll(pItemInfo->WearedItem);

	// µû·ÎÇØÁÖÀÚ
	//m_ItemContainer.GetSlot(eItemTable_ShopInven)->SetItemInfoAll(pItemInfo->ShopInventory);
}

void CPlayer::InitShopInvenInfo(ITEMBASE* pShopWareHouse)
{
	m_ItemContainer.GetSlot(eItemTable_ShopInven)->SetItemInfoAll(pShopWareHouse);	
}

void CPlayer::AddPyogukItem(ITEMBASE * pPyogukItem)
{
	CPyogukSlot * pSlot = (CPyogukSlot *)m_ItemContainer.GetSlot(eItemTable_Pyoguk);
	pSlot->InsertItemAbs(this, pPyogukItem->Position, pPyogukItem);
}

void CPlayer::InitPyogukInfo(BYTE Pyoguknum,MONEYTYPE money)
{
	CPyogukSlot * pSlot = (CPyogukSlot *)m_ItemContainer.GetSlot(eItemTable_Pyoguk);
	pSlot->SetPyogukNum(Pyoguknum);

	MONEYTYPE maxmoney = 0;
	if(Pyoguknum)
	{		
		PYOGUKLISTINFO* pInfo = PYOGUKMGR->GetPyogukInfo(Pyoguknum);
		ASSERT(pInfo);
		maxmoney = pInfo ? pInfo->MaxMoney : 10;
	}
	else
	{
		ASSERT(money == 0);
		maxmoney = 0;
	}
	pSlot->CreatePurse(&m_PyogukPurse, this, money, maxmoney);	
}

void CPlayer::InitPyogukItemInfo(PYOGUK_ITEM* pItemInfo)
{
	int rt;
//	char errbuf[128];
	CPyogukSlot * pSlot;
	pSlot = (CPyogukSlot *)m_ItemContainer.GetSlot(eItemTable_Pyoguk);
	if(m_bPyogukInit == FALSE)
	{
		pSlot->SetItemInfoAll(pItemInfo->Pyoguk);
		m_bPyogukInit = TRUE;
	}
	else
	{
		for(int i=0; i<SLOT_PYOGUK_NUM; ++i )
		{
			if(pItemInfo->Pyoguk[i].dwDBIdx != 0)
			{		
				rt = pSlot->InsertItemAbs(this, pItemInfo->Pyoguk[i].Position, &pItemInfo->Pyoguk[i]);
				if( rt != EI_TRUE)
				{
			//		sprintf(errbuf, "InitPyogukItemInfo Error Type: %d, itemDBIdx : %d, CharNO: %u", rt, pItemInfo->Pyoguk[i].dwDBIdx, GetID());
			//		ASSERTMSG(0, errbuf);
				}
			}
		}
	}
}

void CPlayer::InitPetInvenItemInfo(PETINVEN_ITEM* pItemInfo)
{
	int rt = 0;

	CItemSlot* pSlot = NULL;
	pSlot = m_ItemContainer.GetSlot(eItemTable_PetInven);
	
	pSlot->SetItemInfoAll(pItemInfo->PetInven);
	/*
	if(m_bPetInvenInit == FALSE)
	{
		pSlot->SetItemInfoAll(pItemInfo->PetInven);
		m_bPetInvenInit = TRUE;
	}
	else
	{
		for( int i = 0; i < SLOT_PETINVEN_NUM; ++i )
		{
			if(pItemInfo->PetInven[i].dwDBIdx != 0)
			{
				rt = pSlot->InsertItemAbs(this, pItemInfo->PetInven[i].Position, &pItemInfo->PetInven[i]);
				if( rt != EI_TRUE )
				{
					ASSERT(0);
				}
			}
		}
	}*/
}

void CPlayer::SendPetInvenItemInfo()
{
	SEND_PETINVEN_ITEM_INFO msg;
	memset(&msg, 0, sizeof(msg));
	msg.Category = MP_ITEM;
	msg.Protocol = MP_ITEM_PETINVEN_INFO_ACK;

	ITEM_OPTION_INFO OptionInfo[SLOT_PETINVEN_NUM];
	ITEM_RARE_OPTION_INFO RareOptionInfo[SLOT_PETINVEN_NUM];
	WORD OptionNum = 0;
	WORD RareOptionNum = 0;

	CItemSlot* pSlot = GetSlot(eItemTable_PetInven);
	POSTYPE StartPos = TP_PETINVEN_START;

	for( int i = 0; i < SLOT_PETINVEN_NUM; ++i )
	{
		if( pSlot->IsEmpty(StartPos+i) == FALSE )
		{
			msg.PetInvenItem[i] = *pSlot->GetItemInfoAbs(StartPos+i);

			if(ITEMMGR->IsOptionItem(msg.PetInvenItem[i].wIconIdx, msg.PetInvenItem[i].Durability))
			{
				memcpy(&OptionInfo[OptionNum], GetItemOption(msg.PetInvenItem[i].Durability), sizeof(ITEM_OPTION_INFO));
				++OptionNum;
			}
			if(ITEMMGR->IsRareOptionItem(msg.PetInvenItem[i].wIconIdx, msg.PetInvenItem[i].RareIdx))
			{
				memcpy(&RareOptionInfo[RareOptionNum], GetItemRareOption(msg.PetInvenItem[i].RareIdx), sizeof(ITEM_RARE_OPTION_INFO));
				++RareOptionNum;
			}
		}
	}

	if( OptionNum )
	{
		msg.AddableInfo.AddInfo( CAddableInfoList::ItemOption, sizeof(ITEM_OPTION_INFO)*OptionNum, OptionInfo );
		msg.wOptionCount = OptionNum;
	}
	if( RareOptionNum )
	{
		msg.AddableInfo.AddInfo( CAddableInfoList::ItemRareOption, sizeof(ITEM_RARE_OPTION_INFO)*RareOptionNum, RareOptionInfo );
		msg.wRareOptionCount = RareOptionNum;
	}

	SendMsg(&msg,msg.GetSize());
}

void CPlayer::InitPetWearItemInfo( ITEMBASE* pWearItemInfo )
{
	m_ItemContainer.GetSlot(eItemTable_PetWeared)->SetItemInfoAll(pWearItemInfo);	
}

void CPlayer::InitShopItemInfo(SHOP_ITEM* pItemInfo)
{
	CShopItemSlot* pSlot = (CShopItemSlot*)m_ItemContainer.GetSlot(eItemTable_Shop);
	if(m_bShopInit == FALSE)
	{
		pSlot->SetItemInfoAll(pItemInfo->Item);

		for(int i=0; i<SLOT_SHOPITEM_IMSI; i++)
		{
			if( pItemInfo->Item[i].dwDBIdx != 0 )			
				pSlot->SetLock(pItemInfo->Item[i].Position, TRUE);

			ITEM_INFO * pItemListInfo = ITEMMGR->GetItemInfo(pItemInfo->Item[i].wIconIdx);

			//SW060328 ¾ÆÀÌÅÛ¸ô Æê »ý¼º
			if(pItemListInfo)
			{
				if( pItemListInfo->ItemKind == eSHOP_ITEM_PET )
				{
					PET_TOTALINFO* pPetInfo = m_PetManager.GetPetTotalInfo(pItemInfo->Item[i].dwDBIdx, eWithNULL );
					if( NULL == pPetInfo )
					{
						g_pServerSystem->CreateNewPet(this, pItemListInfo->ItemIdx, pItemInfo->Item[i].dwDBIdx,  pItemListInfo->Plus_MugongIdx);
					}
				}
			}
		}
		
		m_bShopInit = TRUE;
	}
	else
	{
		for(int i=0; i<SLOT_SHOPITEM_IMSI; i++)
		{
			if( pItemInfo->Item[i].dwDBIdx != 0 )
			{				
				if( pSlot->InsertItemAbs( this, pItemInfo->Item[i].Position, &pItemInfo->Item[i] ) != EI_TRUE )
				{
					ASSERT(0);
				}
			}
		}
	}
}

void CPlayer::SetPyogukNum(BYTE n)
{
	CPyogukSlot * pSlot = (CPyogukSlot *)m_ItemContainer.GetSlot(eItemTable_Pyoguk);
	pSlot->SetPyogukNum(n);
}

BYTE CPlayer::GetPyogukNum()
{
	CPyogukSlot * pSlot = (CPyogukSlot *)m_ItemContainer.GetSlot(eItemTable_Pyoguk);
	return pSlot->GetPyogukNum();
}

MONEYTYPE CPlayer::GetMoney(eITEMTABLE tableIdx)
{
	CItemSlot* pSlot = m_ItemContainer.GetSlot(tableIdx);
	return pSlot->GetMoney();
}

void CPlayer::GetCharacterTotalInfo(CHARACTER_TOTALINFO* pRtInfo)
{
	memcpy(pRtInfo,&m_HeroCharacterInfo,sizeof(CHARACTER_TOTALINFO));

	// 2014-08-12 »ñÈ¡½ÇÉ«Á¦Á¿£¬ÐÄÂö£¬ÌåÖÊ£¬Ãô½Ý ÎïÀí·ÀÓù¼°¹¥»÷Á¦ÐÅÏ¢ ÎªÊôÐÔ!

	pRtInfo->genGoal = GetGenGol();
	pRtInfo->simmak = GetSimMek();
	pRtInfo->minchub = GetMinChub();
	pRtInfo->cheryuk = GetCheRyuk();
	pRtInfo->Fame = m_HeroInfo.Fame;
	pRtInfo->defenseRate = GetPhyDefense();
	pRtInfo->critical = GetCritical();
	pRtInfo->minAttack = DoGetPhyAttackPowerMin();
	pRtInfo->maxAttack = DoGetPhyAttackPowerMax();
	pRtInfo->naeryuk = GetNaeRyuk();
}
HERO_TOTALINFO* CPlayer::GetHeroTotalInfo()
{
	return &m_HeroInfo;
}

void CPlayer::GetHeroTotalInfo(HERO_TOTALINFO* pRtInfo)
{
	memcpy(pRtInfo,&m_HeroInfo,sizeof(HERO_TOTALINFO));
}

void CPlayer::GetMugongTotalInfo(MUGONG_TOTALINFO* pRtInfo)
{
	memcpy(pRtInfo,&m_HeroMugongInfo,sizeof(MUGONG_TOTALINFO));
}

void CPlayer::GetItemtotalInfo(ITEM_TOTALINFO* pRtInfo,DWORD dwFlag)
{
	if(dwFlag & GETITEM_FLAG_INVENTORY)
	{
		m_ItemContainer.GetSlot(eItemTable_Inventory)->GetItemInfoAll(pRtInfo->Inventory);
		m_ItemContainer.GetSlot(eItemTable_ShopInven)->GetItemInfoAll(pRtInfo->ShopInventory);
	}
	else
	{
		//memset(pRtInfo->Inventory,0,sizeof(ITEMBASE)*SLOT_INVENTORY_NUM);
		//memset(pRtInfo->ShopInventory,0,sizeof(ITEMBASE)*SLOT_SHOPINVEN_NUM);
		memset(pRtInfo->Inventory,0,sizeof(ITEMBASE)*(SLOT_INVENTORY_NUM/*+40*/));
		memset(pRtInfo->ShopInventory,0,sizeof(ITEMBASE)*(SLOT_SHOPINVEN_NUM+40));
	}
	if(dwFlag & GETITEM_FLAG_WEAR)
		m_ItemContainer.GetSlot(eItemTable_Weared)->GetItemInfoAll(pRtInfo->WearedItem);
	else
		memset(pRtInfo->WearedItem,0,sizeof(ITEMBASE)*eWearedItem_Max);

	if(dwFlag & GETITEM_FLAG_PETWEAR)
		m_ItemContainer.GetSlot(eItemTable_PetWeared)->GetItemInfoAll(pRtInfo->PetWearedItem);
	else
		memset(pRtInfo->WearedItem,0,sizeof(ITEMBASE)*SLOT_PETWEAR_NUM);
	///m_pIconTable[eItemTable_Inventory]->GetItemInfoAll(pRtInfo->Inventory);
	//m_pIconTable[eItemTable_Weared]->GetItemInfoAll(pRtInfo->WearedItem);

	// magi82 - Titan(070209)
	if(dwFlag & GETITEM_FLAG_TITANWEAR)
		m_ItemContainer.GetSlot(eItemTable_Titan)->GetItemInfoAll(pRtInfo->TitanWearedItem);
	else
		memset(pRtInfo->TitanWearedItem,0,sizeof(ITEMBASE)*SLOT_TITANWEAR_NUM);

	// magi82 - Titan(070227)
	if(dwFlag & GETITEM_FLAG_TITANSHOP)
		m_ItemContainer.GetSlot(eItemTable_TitanShopItem)->GetItemInfoAll(pRtInfo->TitanShopItem);
	else
		memset(pRtInfo->TitanShopItem,0,sizeof(ITEMBASE)*SLOT_TITANSHOPITEM_NUM);
}

void CPlayer::GetPyogukItemInfo(PYOGUK_ITEM * pRtInfo)
{
	m_ItemContainer.GetSlot(eItemTable_Pyoguk)->GetItemInfoAll(pRtInfo->Pyoguk);
}

void CPlayer::SetAddMsg(char* pAddMsg,WORD* pMsgLen,DWORD dwReceiverID,BOOL bLogin)
{
	if( m_dwUserLevel <= eUSERLEVEL_GM && m_HeroCharacterInfo.bVisible == FALSE )
	{
		*pMsgLen = 0;
		return;
	}

	SEND_CHARACTER_TOTALINFO* pMsg = (SEND_CHARACTER_TOTALINFO*)pAddMsg;
	pMsg->AddableInfo.Init();

	pMsg->Category = MP_USERCONN;
	pMsg->Protocol = MP_USERCONN_CHARACTER_ADD;
	pMsg->dwObjectID = dwReceiverID;
	GetSendMoveInfo(&pMsg->MoveInfo,&pMsg->AddableInfo,TRUE);	//SW071129 ¹æÇâ Ãß°¡
	pMsg->MoveInfo.AbilityKyungGongLevel = GetAbilityStats()->KyunggongLevel;
	GetBaseObjectInfo(&pMsg->BaseObjectInfo);
	GetCharacterTotalInfo(&pMsg->TotalInfo);	
	memcpy(&pMsg->ShopItemOption, GetShopItemStats(), sizeof(SHOPITEMOPTION));
	pMsg->bLogin = bLogin;

	cStreetStall* pStall = STREETSTALLMGR->FindStreetStall(this);
	if( pStall != NULL)
	{
		char StallTitle[MAX_STREETSTALL_TITLELEN+1];
		GetStreetStallTitle(StallTitle);
		StallTitle[MAX_STREETSTALL_TITLELEN] = 0;
		
		if( pStall->GetStallKind() == eSK_SELL)
			pMsg->AddableInfo.AddInfo(CAddableInfoList::StreetStall,strlen(StallTitle)+1,StallTitle);
		else if( pStall->GetStallKind() == eSK_BUY)
			pMsg->AddableInfo.AddInfo(CAddableInfoList::StreetBuyStall,strlen(StallTitle)+1,StallTitle);
	}

	if( GetBattle()->GetBattleKind() == eBATTLE_KIND_VIMUSTREET )	//¡§¡þn¨Ïo¢®iA¨Ï¡þAI ¡ËO¢®¡¿
	{
		COMPRESSEDPOS VimuPos;

		BATTLE_INFO_VIMUSTREET info;
		WORD wSize;
		GetBattle()->GetBattleInfo( (char*)&info, &wSize );

		VimuPos.Compress(&info.vStgPos);

		pMsg->AddableInfo.AddInfo(CAddableInfoList::VimuInfo,sizeof(COMPRESSEDPOS),&VimuPos);
	}
	
	int nTeam = PARTYWARMGR->IsMemberInPartyWar( this );
	if( nTeam != 0 )
	{
		pMsg->AddableInfo.AddInfo(CAddableInfoList::PartyWar, sizeof(nTeam), &nTeam);
	}

	// RaMa - 05.05.27
	if( GetBattle()->GetBattleKind() == eBATTLE_KIND_GTOURNAMENT )
	{
		nTeam = GetBattle()->GetBattleTeamID( this );		
		pMsg->AddableInfo.AddInfo(CAddableInfoList::GTournament, sizeof(nTeam), &nTeam);
	}
	else if( GetBattle()->GetBattleKind() == eBATTLE_KIND_SIEGEWAR )
	{
		SEIGEWAR_CHARADDINFO info;
		info.Team = (BYTE)GetBattle()->GetBattleTeamID( this );
		if( SIEGEWARMGR->GetEngraveIdx() == GetID() )
			info.bEngrave = 1;
		else
			info.bEngrave = 0;		

		pMsg->AddableInfo.AddInfo(CAddableInfoList::SiegeWar, sizeof(SEIGEWAR_CHARADDINFO), &info);
	}

	pMsg->bInTitan = InTitan();

	TITAN_APPEARANCEINFO TitanAppearanceInfo;
	// Å¸ÀÌÅº ¸Å´ÏÁ®¿¡ ¼¼ÆÃµÈ ÀåÂøÀÎµ¦½º Á¤º¸.
	m_TitanManager.GetAppearanceInfo(&TitanAppearanceInfo);
	pMsg->AddableInfo.AddInfo(CAddableInfoList::TitanAppearanceInfo, sizeof(TITAN_APPEARANCEINFO), &TitanAppearanceInfo);

	*pMsgLen = pMsg->GetMsgLength();
}

void CPlayer::SendMsg(MSGBASE* pMsg,int MsgLen)
{
	CObject::SendMsg(pMsg, MsgLen);
}

void CPlayer::SetStreetStallTitle(char* title)
{
	SafeStrCpy( m_StreetStallTitle, title, MAX_STREETSTALL_TITLELEN+1 );
}

void CPlayer::GetStreetStallTitle(char* title)
{
	SafeStrCpy( title, m_StreetStallTitle, MAX_STREETSTALL_TITLELEN+1);
}

void CPlayer::DoSetInited()
{
	STATSMGR->CalcItemStats(this);	
	
	// ¡Ë¡þEAI¡Íi¡Ë?CI¢®¨¡A¨Ï©ª¡§¡Ì C¡§¨£¡Íi¡Íi ¢®iy¡Ë¡þi¢®¢´A ¡§uEA¢®A¢®¨¡O
	SetLife(m_HeroCharacterInfo.Life);
	SetShield(m_HeroCharacterInfo.Shield);
	SetNaeRyuk(m_HeroInfo.wNaeRyuk);

	// ¢®¢´I¢®¨ú¢®¢¯AI C¨Ï¡þ; ¡ËO¢®¡¿ d¡§¡þ¡Ë¡þ¡Ë¡þ| A¢®©­¡ËOoAI¡§u¨Ï¡Ì¡§¢®¡Ëc¢®¢´I ¡§¡þ¡Ë¡þ¨Ï©ª¡Ë?
	SetPlayerLevelUpPoint(GetPlayerLevelUpPoint());

	// ¡§oA¢®¨¡¢®I¢®¨¡¡§¡Ì AE¢®¨úaE¢®¨Ï
//	m_dwStartTime = gCurTime;
	m_dwProgressTime = gCurTime;
}

void CPlayer::CalcState()
{
	STATSMGR->CalcItemStats(this);	

	// ¡Ë¡þEAI¡Íi¡Ë?CI¢®¨¡A¨Ï©ª¡§¡Ì C¡§¨£¡Íi¡Íi ¢®iy¡Ë¡þi¢®¢´A ¡§uEA¢®A¢®¨¡O
	SetLife(m_HeroCharacterInfo.Life);
	SetShield(m_HeroCharacterInfo.Shield);
	SetNaeRyuk(m_HeroInfo.wNaeRyuk);

//	// ¢®¢´I¢®¨ú¢®¢¯AI C¨Ï¡þ; ¡ËO¢®¡¿ d¡§¡þ¡Ë¡þ¡Ë¡þ| A¢®©­¡ËOoAI¡§u¨Ï¡Ì¡§¢®¡Ëc¢®¢´I ¡§¡þ¡Ë¡þ¨Ï©ª¡Ë?
//	SetPlayerLevelUpPoint(GetPlayerLevelUpPoint());
}


/*
void CPlayer::AddMoneyAndSendToPlayer(DWORD Money)
{
	DWORD CurM = GetMoney();
	SetMoney(Money + CurM);
	
	MSG_DWORD msg;
	msg.Category = MP_ITEM;
	msg.Protocol = MP_ITEM_MONEY;
	msg.dwObjectID = GetID();
	msg.dwData = GetMoney();
	SendMsg(&msg,sizeof(msg));
}
*/
/*
void CPlayer::AddHuntItemAndSendToPlayer(WORD DropItemNum)
{
	CIconTable * pInventory = GetIconTable(eItemTable_Inventory);
	POSTYPE absPosOut;
	MSG_WORD msg;
	WORD ItemIdx = DropItemNum;
	if(!pInventory->GetEmptyCellAbsPosRef(absPosOut)) goto HUNTITEM_FAILED;

	
	ITEMBASE item;
	item.dwDBIdx = 0;
	item.Durability = 0;
	item.Position = absPosOut;
	item.QuickPosition = 0;
	item.wIconIdx = DropItemNum;

	pInventory->InsertItemAbsDB(absPosOut, &item);
	
	//CharacterItemGetHuntIDX(GetID(), ItemIdx, 0, absPosOut);
	return;

HUNTITEM_FAILED:
	msg.Protocol = MP_ITEM_OBTAIN_FAILED_NOTIFY;
	SendMsg(&msg,sizeof(MSG_WORD));
	return;
}


BOOL CPlayer::HuntItemDBResult(DWORD dwPlayerID, ITEMBASE * itemInfo)
{
	MSG_ITEM msg;
	CIconTable * pInventory = GetIconTable(eItemTable_Inventory);
	if(this == NULL) goto HUNTITEM_FAILED;
	pInventory->InsertItemAbsDB(itemInfo->Position, itemInfo);


	msg.Category = MP_ITEM;
	msg.Protocol = MP_ITEM_OBTAIN_SUCCESS_NOTIFY;
	msg.dwObjectID = GetID();
	msg.ItemInfo = *itemInfo;
	SendMsg(&msg,sizeof(msg));

	return TRUE;

HUNTITEM_FAILED:
	MSG_WORD msg2;
	msg2.Category = MP_ITEM;
	msg2.Protocol = MP_ITEM_OBTAIN_FAILED_NOTIFY;
	SendMsg(&msg2,sizeof(MSG_WORD));
	return FALSE;
}
*/
// LBS 03.11.04 flagA¢®¢´¢®¨¡¡Ë¢ç ¨Ïo¢®¨¡¢®¨¡C¢®ic¢®¨úa¡Ë?I ¡§¢®E¢®¨úa¢®¨ú¡Ë¡þ¡§¡þ¡§¡Ë (CoAc) 0:¡§¢®E¢®¨úa, 1:¢®ic¢®¨úa, 
// 3:(DB¡§u¢®ACI¢®¨¡i, A¢®©­¡ËOoAI¡§u¨Ï¡Ì¡§¢®¡Ëc¡Ë?¡Ë¢ç ¡§uE¡Ë¡þ¡Ëc¡Ë¡þe¡§u¢®¨Ï ¡Ë¡þ¡§¡©¡§u¡§uAo¡Ë¡þ| ¡ËOc¡Ë?iAo ¡§uE¡Ë¡ÍA¡Ë¡ÍU.)

void CPlayer::MoneyUpdate( MONEYTYPE money )
{	
	m_HeroInfo.Money = money;

/*
	MSG_DWORD2 msg;
	msg.Category = MP_ITEM;
	msg.Protocol = MP_ITEM_MONEY;
	msg.dwData1 = 0;//GetMoney();
	msg.dwData2 = flag;
	SendMsg(&msg,sizeof(msg));
*/
}

/*
void CPlayer::SetMoneyNoMsg(DWORD money)
{	
	m_HeroInfo.Money = money;	

	// DB¡Ë?¡Ë¢ç ¡§u¢®A¡Íi¢®IAI¡§¢®¡Ëc
	CharacterHeroInfoUpdate(this);
}
*/

/*
void CPlayer::SetMoneyNoMsgNoUpdate(DWORD money)
{
	m_HeroInfo.Money = money;	
}
*/
/*
void CPlayer::ObtainMoney(DWORD addmoney)		//¡Ë¡þ¢®AA¢®¢¯¡Ë?¡§I¡§u¢®¨Ï ¡§uo¡§uu;¡ËO¢®¡¿¡Ë¡þ¡Ë¡þ E¢®IAa
{
	m_HeroInfo.Money += addmoney;	

	// DB¡Ë?¡Ë¢ç ¡§u¢®A¡Íi¢®IAI¡§¢®¡Ëc
	CharacterHeroInfoUpdate(this);

	MSG_DWORD msg;
	msg.Category = MP_ITEM;
	msg.Protocol = MP_ITEM_OBTAIN_MONEY;
	msg.dwObjectID = GetID();
	msg.dwData = m_HeroInfo.Money;
	SendMsg(&msg,sizeof(msg));		
}
*/
void CPlayer::SetGenGol(WORD val)
{
	m_HeroInfo.wGenGol = val;

	STATSMGR->CalcCharStats(this);

	// DB¡Ë?¡Ë¢ç ¡§u¢®A¡Íi¢®IAI¡§¢®¡Ëc
	CharacterHeroInfoUpdate(this);

	MSG_DWORD msg;
	msg.Category = MP_CHAR;
	msg.Protocol = MP_CHAR_GENGOL_NOTIFY;
	msg.dwObjectID = GetID();
	msg.dwData = val;
	SendMsg(&msg,sizeof(msg));
}

void CPlayer::SetMinChub(WORD val)
{
	m_HeroInfo.wMinChub = val;

	STATSMGR->CalcCharStats(this);

	// DB¡Ë?¡Ë¢ç ¡§u¢®A¡Íi¢®IAI¡§¢®¡Ëc
	CharacterHeroInfoUpdate(this);

	MSG_DWORD msg;
	msg.Category = MP_CHAR;
	msg.Protocol = MP_CHAR_MINCHUB_NOTIFY;
	msg.dwObjectID = GetID();
	msg.dwData = val;
	SendMsg(&msg,sizeof(msg));
}
void CPlayer::SetCheRyuk(WORD val)
{
	m_HeroInfo.wCheRyuk = val;

	// ¢®iy¡Ë¡þi¢®¢´A, ¨Ïo¨Ï¢®¡§ui¢®¢´A; ¡Ë¡ÍU¡§oA ¢®¨¡e¢®ie
	STATSMGR->CalcCharStats(this);

	// DB¡Ë?¡Ë¢ç ¡§u¢®A¡Íi¢®IAI¡§¢®¡Ëc
	CharacterHeroInfoUpdate(this);

	MSG_DWORD msg;
	msg.Category = MP_CHAR;
	msg.Protocol = MP_CHAR_CHERYUK_NOTIFY;
	msg.dwObjectID = GetID();
	msg.dwData = val;
	SendMsg(&msg,sizeof(msg));
}
void CPlayer::SetSimMek(WORD val)
{
	m_HeroInfo.wSimMek = val;

	// ¨Ï©ª¢®i¢®¢´A; ¡Ë¡ÍU¡§oA ¢®¨¡e¢®ie
	STATSMGR->CalcCharStats(this);

	// DB¡Ë?¡Ë¢ç ¡§u¢®A¡Íi¢®IAI¡§¢®¡Ëc
	CharacterHeroInfoUpdate(this);

	MSG_DWORD msg;
	msg.Category = MP_CHAR;
	msg.Protocol = MP_CHAR_SIMMEK_NOTIFY;
	msg.dwObjectID = GetID();
	msg.dwData = val;
	SendMsg(&msg,sizeof(msg));
}

//-------------- USE ITEM ------------------------------------------------------------
BOOL CPlayer::UseItem(WORD abs_pos)
{
	return TRUE;
}

// AI¡§¡þ¢®IAa¡Ë¡þ¡Ëc, AaA¨Ï¨£A¢®E, ¡Ë¡ÍUAaA¢®E
BOOL CPlayer::UseConsumptionItem(WORD abs_pos)
{
	// ¡§uO¡Ë¡þ¨Ï¡Ì¡§u¡§¡þ ¡§u¡§¢®AIAU

	return TRUE;
}

BOOL CPlayer::UseEquipItem(WORD abs_pos)
{
	// ¡§u¡§¢®AIAU

	return TRUE;
}

BOOL CPlayer::UseMugongItem(WORD abs_pos)
{
	return TRUE;
}


CItemSlot * CPlayer::GetSlot(POSTYPE absPos)
{
	// if( TP_INVENTORY_START <= absPos && absPos < TP_PYOGUK_END )
	if( TP_INVENTORY_START <= absPos && absPos < TP_SHOPITEM_END )
	{
		return m_ItemContainer.GetSlot(absPos);
	}
	else if( TP_GUILDWAREHOUSE_START <= absPos && absPos < TP_GUILDWAREHOUSE_END )
	{
		return (CItemSlot*)GUILDMGR->GetSlot(GetGuildIdx());
	}
	else if( TP_SHOPITEM_START <= absPos && absPos < TP_SHOPITEM_END )
	{
		return m_ItemContainer.GetSlot(absPos);
	}
	else if( TP_SHOPINVEN_START <= absPos && absPos < TP_SHOPINVEN_END/*-40*/ )//kiv
	{
		return m_ItemContainer.GetSlot(absPos);
	}
	else if( TP_PETINVEN_START <= absPos && absPos < TP_PETINVEN_END )
	{
		return m_ItemContainer.GetSlot(absPos);
	}
	else if( TP_PETWEAR_START <= absPos && absPos < TP_PETWEAR_END )
	{
		return m_ItemContainer.GetSlot(absPos);
	}
	else if( TP_TITANWEAR_START <= absPos && absPos < TP_TITANWEAR_END )
	{
		return m_ItemContainer.GetSlot(absPos);
	}
	else if( TP_TITANSHOPITEM_START <= absPos && absPos < TP_TITANSHOPITEM_END )
	{
		return m_ItemContainer.GetSlot(absPos);
	}
	return NULL;
}

CItemSlot * CPlayer::GetSlot(eITEMTABLE tableIdx)
{
	if(tableIdx < eItemTable_TableMax)
		return m_ItemContainer.GetSlot(tableIdx);
	else if( tableIdx == eItemTable_GuildWarehouse)
	{
		return (CItemSlot*)GUILDMGR->GetSlot(GetGuildIdx());
	}
	return NULL;
}

// ¨Ïo¢®i¢®¨¡¨Ï¨£¡§u¢®¨úAA¡Ë¡þ¡Ë¡þ ¡Ë?E
/*
BOOL CPlayer::UseQuickItem(WORD rel_pos)
{
	ASSERT(rel_pos < MAX_QUICKITEMPERSHEET_NUM*MAX_QUICKSHEET_NUM);
	SetQuickSelectIdx(rel_pos);
	QUICKITEM * pQuickItem = &m_QuickItem[m_quick_info.cur_selected_page][m_quick_info.cur_selected_rel_idx];
	ASSERT(pQuickItem != NULL);

	

	return TRUE;
}
void CPlayer::SetQuickSelectIdx(WORD idx) 
{ 
	if(idx >= MAX_QUICKITEMPERSHEET_NUM*MAX_QUICKSHEET_NUM) return;

	if(idx == 5)
	{
		//KEY_6 for test
	//	SetPlayerLevel(GetPlayerLevel()+1);
	}

	m_quick_info.cur_selected_page = idx/MAX_QUICKITEMPERSHEET_NUM;
	m_quick_info.cur_selected_rel_idx = idx%MAX_QUICKITEMPERSHEET_NUM;
	//m_quick_info.cur_selected_abs_idx = 
}
*/
//---------------------- ¡§uoA¡Ë¢ç  ---------------------------------------------------
void CPlayer::SetLife(DWORD val,BOOL bSendMsg) 
{
	if(GetState() == eObjectState_Die)
		return;

	DWORD maxlife = GetMaxLife();
	if(val > maxlife)
		val = maxlife;
	
	//SW070127 Å¸ÀÌÅº
	if( InTitan() && val )
	{
		DWORD lifeRate = val * 100 / maxlife;

		if( lifeRate < GAMERESRCMNGR->GetTitanRule()->MasterLifeRate_forTitanCall )
		{
			m_TitanManager.GetOffTitan(eMasterLifeRate);
		}
	}

	if(m_HeroCharacterInfo.Life != val)	// ¡Íi¢®I¨ÏoIAo AI¢®¨¡¨Ï¢®¡Ë?i¡Ë?¡Ë¢ç¡Ë¡ÍA ¡Ë?¢®©­Aa¡Ë?¡Ë¢ç¡§u¢®¨Ï ¢®i¡§IA¡§¨£¡Ë¡ÍU.
	{
		if(bSendMsg == TRUE)
		{
			// To A¢®©­¡ËOoAI¡§u¨Ï¡Ì¡§¢®¡Ëc -------------------------------
			MSG_INT msg;
			msg.Category = MP_CHAR;
			msg.Protocol = MP_CHAR_LIFE_ACK;
			msg.dwObjectID = GetID();
			msg.nData = val - GetLife();
			SendMsg(&msg,sizeof(msg));
		}
		
		if(GetPartyIdx())
			SendPlayerLifeToParty(val, GetPartyIdx());
	}
		
	m_HeroCharacterInfo.Life = val;
}

void CPlayer::SendPlayerLifeToParty(DWORD val, DWORD PartyID)
{	
	CParty* pParty = PARTYMGR->GetParty(PartyID);
	if(pParty == NULL)
		return;
	
	SEND_PARTY_GAUGE msg;
	msg.Category = MP_PARTY;
	msg.Protocol = MP_PARTY_MEMBERLIFE;
	msg.PlayerID = GetID();
	if( GetMaxLife() )
		msg.GaugePercent = (float)(val*100/GetMaxLife());
	else
	{
		ASSERT(0);
		msg.GaugePercent = 0;
	}
//	pParty->SendMsgExceptOne(&msg, sizeof(msg), msg.dwData1);
	pParty->SendMsgExceptOneinChannel(&msg, sizeof(msg), msg.PlayerID, GetGridID());
}

void CPlayer::SetShield(DWORD val,BOOL bSendMsg) 
{
	if(GetState() == eObjectState_Die)
		return;

	DWORD maxShield = GetMaxShield();
	if(val > maxShield)
		val = maxShield;
	
	if(bSendMsg == TRUE)	// ¡Íi¢®I¨ÏoIAo AI¢®¨¡¨Ï¢®¡Ë?i¡Ë?¡Ë¢ç¡Ë¡ÍA ¡Ë?¢®©­Aa¡Ë?¡Ë¢ç¡§u¢®¨Ï ¢®i¡§IA¡§¨£¡Ë¡ÍU.
	{
		// To A¢®©­¡ËOoAI¡§u¨Ï¡Ì¡§¢®¡Ëc -------------------------------
		MSG_INT msg;
		msg.Category = MP_CHAR;
		msg.Protocol = MP_CHAR_SHIELD_ACK;
		msg.dwObjectID = GetID();
		msg.nData = val - m_HeroCharacterInfo.Shield;
		SendMsg(&msg,sizeof(msg));
	}

	m_HeroCharacterInfo.Shield = val;

//	if(GetPartyIdx())
//		SendPlayerShieldToParty(val, GetPartyIdx());
}

void CPlayer::SendPlayerShieldToParty(DWORD val, DWORD PartyID)
{	
	// A¢®©­¡ËOoAI¡§u¨Ï¡Ì¡§¢®¡Ëc¡Ë?¡Ë¢ç¡§u¢®¨Ï ¡§¡þ¡Ë¡þ¡Ë?¡§IAO¡Ë¡ÍA¢®¨¡O ¡§u¨Ï¨£¡Ë¡ÍA¡Íi¢®I ¡Ë?O A¨Ï¡þ¢®¨¡¡Ë¢çC¨Ï¡þ;¢®¨úi¨Ï©ª¡§¡Ì... hs
/*
		CParty* pParty = PARTYMGR->GetParty(PartyID);
		if(pParty == NULL)
			return;
		
		SEND_PARTY_GAUGE msg;
		msg.Category = MP_PARTY;
		msg.Protocol = MP_PARTY_MEMBERSHIELD;
		msg.PlayerID = GetID();
		msg.GaugePercent = GetShieldPercent();
		pParty->SendMsgExceptOne(&msg, sizeof(msg), msg.dwData1);
*/
}

void CPlayer::SetNaeRyuk(DWORD val,BOOL bSendMsg)
{ 
	if(GetState() == eObjectState_Die)
		return;

	DWORD MaxNaeryuk = GetMaxNaeRyuk();
	if(val > MaxNaeryuk)
		val = MaxNaeryuk;

	// To A¢®©­¡ËOoAI¡§u¨Ï¡Ì¡§¢®¡Ëc -------------------------------
	if( m_HeroInfo.wNaeRyuk != val)
	{
		if(bSendMsg)
		{
			MSG_DWORD msg;
			msg.Category = MP_CHAR;
			msg.Protocol = MP_CHAR_NAERYUK_ACK;
			msg.dwObjectID = GetID();
			msg.dwData = val;
			SendMsg(&msg,sizeof(msg));
		}
		
		if(m_HeroInfo.PartyID)
			SendPlayerNaeRyukToParty(val, m_HeroInfo.PartyID);
	}
	
	m_HeroInfo.wNaeRyuk = val; 
}

void CPlayer::SendPlayerNaeRyukToParty(DWORD val, DWORD PartyID)
{
	CParty* pParty = PARTYMGR->GetParty(PartyID);
	if(pParty == NULL)
		return;
	
	SEND_PARTY_GAUGE msg;
	msg.Category = MP_PARTY;
	msg.Protocol = MP_PARTY_MEMBERNAERYUK;
	msg.PlayerID = GetID();
	if( GetMaxNaeRyuk() )
		msg.GaugePercent = (float)((val*100)/GetMaxNaeRyuk());
	else
	{
		ASSERT(0);
		msg.GaugePercent = 0;
	}

//	pParty->SendMsgExceptOne(&msg, sizeof(msg), msg.dwData1);
	pParty->SendMsgExceptOneinChannel(&msg, sizeof(msg), msg.PlayerID, GetGridID());
}

void CPlayer::SendMoveInfotoParty()
{
/*	CParty* pParty = PARTYMGR->GetParty(PartyID);
	if(pParty == NULL)
		return;

	SEND_PARTYICON_MOVEINFO msg;
	msg.Category = MP_PARTY;
	msg.Protocol = MP_PARTY_SENDPOS;
*/
}

void CPlayer::SetMaxLife(DWORD val)
{
	m_HeroCharacterInfo.MaxLife = val;

	// To A¢®©­¡ËOoAI¡§u¨Ï¡Ì¡§¢®¡Ëc -------------------------------
	MSG_DWORD msg;
	msg.Category = MP_CHAR;
	msg.Protocol = MP_CHAR_MAXLIFE_NOTIFY;
	msg.dwObjectID = GetID();
	msg.dwData = val;
	SendMsg(&msg,sizeof(msg));
}
void CPlayer::SetMaxShield(DWORD val)
{
	m_HeroCharacterInfo.MaxShield = val;

	// To A¢®©­¡ËOoAI¡§u¨Ï¡Ì¡§¢®¡Ëc -------------------------------
	MSG_DWORD msg;
	msg.Category = MP_CHAR;
	msg.Protocol = MP_CHAR_MAXSHIELD_NOTIFY;
	msg.dwObjectID = GetID();
	msg.dwData = val;
	SendMsg(&msg,sizeof(msg));
}
void CPlayer::SetMaxNaeRyuk(DWORD val)
{
	m_HeroInfo.wMaxNaeRyuk= val;

	// To A¢®©­¡ËOoAI¡§u¨Ï¡Ì¡§¢®¡Ëc -------------------------------
	MSG_DWORD msg;
	msg.Category = MP_CHAR;
	msg.Protocol = MP_CHAR_MAXNAERYUK_NOTIFY;
	msg.dwObjectID = GetID();
	msg.dwData = val;
	SendMsg(&msg,sizeof(msg));
}
void CPlayer::AddGenGol(int val)
{
	SetGenGol(m_HeroInfo.wGenGol+val);
}
void CPlayer::AddSimMek(int val)
{
	SetSimMek(m_HeroInfo.wSimMek+val);
}
void CPlayer::AddMinChub(int val)
{
	SetMinChub(m_HeroInfo.wMinChub+val);
}
void CPlayer::AddCheRyuk(int val)
{
	SetCheRyuk(m_HeroInfo.wCheRyuk+val);
}
void CPlayer::SetPlayerLevelUpPoint(LEVELTYPE val) 
{ 
	m_HeroInfo.LevelUpPoint=val;
	
	MSG_DWORD msg;
	msg.Category = MP_CHAR;
	msg.Protocol = MP_CHAR_LEVELUPPOINT_NOTIFY;
	msg.dwObjectID = GetID();
	msg.dwData = val;
	SendMsg(&msg,sizeof(msg));
}


void CPlayer::SetLevel(LEVELTYPE level)
{
	if(level >= GAMERESRCMNGR->GetMaxLevelControl()+1 ) //MAX_PLAYERLEVEL_NUM ÕâÀï½«µÈ¼¶ÏÞÖÆÈ¥µô
	{
		ASSERT(0);
		return;
	}

	if(m_HeroCharacterInfo.Level == level) return;

	if(level > m_HeroCharacterInfo.Level)
	{
		if(m_HeroInfo.MaxLevel < level)
		{
			m_HeroInfo.MaxLevel = level;
			/////////////////////////////////////////////////////////////
			// 06. 06. 2Â÷ ÀüÁ÷ - ÀÌ¿µÁØ
			// 70ÀÌ»ó ·¹º§¾÷½Ã È¹µæ ½ºÅÈ Áõ°¡
			if( level > 70 )
				SetPlayerLevelUpPoint(GetPlayerLevelUpPoint()+4);	// magi82
			else
				SetPlayerLevelUpPoint(GetPlayerLevelUpPoint()+2);	// magi82
			/////////////////////////////////////////////////////////////

#ifdef _KOR_LOCAL_
			// Event¿ë , ´õºí¾÷ÀÌº¥Æ®
			static LEVELTYPE lvtbl[8] = { 10, 15, 20, 25, 30, 35, 40, 45 };
			for( int i=0; i<8; ++i )
			{
				if( level == lvtbl[i] )
				{
					GMEvent01( GetUserID(), GetID(), m_HeroInfo.MaxLevel, g_nServerSetNum );
					break;
				}
			}
#endif
#ifdef _JAPAN_LOCAL_
			if( m_HeroInfo.MaxLevel == 10 )
                GMEvent01( GetUserID(), GetID(), m_HeroInfo.MaxLevel, g_nServerSetNum );
#endif

			// °ÔÀÓÀÌº¥Æ®·Î ¾ÆÀÌÅÛ ¾ò´Â´Ù...
			GAMEEVENTMGR->ProcessLevelUpEvent( this, m_HeroInfo.MaxLevel );

			//SW060719 ¹®ÆÄÆ÷ÀÎÆ®	//¹®ÆÄ¿ø ·¾¾÷¿¡ µû¸¥ °¡»ê.
			GUILDMGR->GuildMemberLevelUpToGuildPoint(this);

			//SW070103 ¹®ÆÄÆ÷ÀÎÆ®°³¼±	//¹®ÇÏ»ý ·¾¾÷¿¡ µû¸¥ Æ÷ÀÎÆ® ´©Àû. ÃàÇÏ±Ý
			GUILDMGR->GuildStudentLvUpPointDBUpdate(this);
		}
		
		m_HeroCharacterInfo.Level = level;
		
		STATSMGR->CalcItemStats(this);
		

		SetLife(GetMaxLife());
		SetShield(GetMaxShield());
		SetNaeRyuk(GetMaxNaeRyuk());
		SetMussangMaxPoint();
		
		// Journal DB¿¡ Ãß°¡
		JournalInsertLevel(GetID(), eJournal_Levelup, level);

		// Client¿¡ ¾Ë¸²
		SEND_JOURNALINFO msg;
		msg.Category = MP_JOURNAL;
		msg.Protocol = MP_JOURNAL_ADD;
		msg.dwObjectID = GetID();
		msg.m_Info.Param = GetLevel();
		msg.m_Info.Kind = eJournal_Levelup;
		SendMsg(&msg, sizeof(msg));


		MSG_LEVEL msg1;
		msg1.Category = MP_CHAR;
		msg1.Protocol = MP_CHAR_LEVEL_NOTIFY;
		msg1.dwObjectID = GetID();
		msg1.Level = GetLevel();
		msg1.MaxExpPoint = GAMERESRCMNGR->GetMaxExpPoint(level);
		msg1.CurExpPoint = GetPlayerExpPoint();
		SendMsg(&msg1,sizeof(msg1));

		MSG_DWORD msg2;
		msg2.Category = MP_CHAR;
		msg2.Protocol = MP_CHAR_PLAYERLEVELUP_NOTIFY;
		msg2.dwObjectID = GetID();
		msg2.dwData = GetID();
		PACKEDDATA_OBJ->QuickSendExceptObjectSelf(this,&msg2,sizeof(msg2));	


		QUESTMGR->AddQuestEvent( this, &CQuestEvent( eQuestEvent_Level, level, 1 ) );
// jsd quest
// ¡Ë?¡§I¢®¨úa¡§u¢®¨Ï levelup event¨Ïo¨Ï¡þ¢®iy
//	QUESTEVENT qe;
//	qe.Init( (void*)this, eQuestEvent_LevelUp, m_HeroCharacterInfo.Level, 0 );
//	QUESTEVENTMGR->AddQuestEvent( qe );
//
	}
	else
	{
		m_HeroCharacterInfo.Level = level;

		STATSMGR->CalcItemStats(this);

		MSG_LEVEL msg1;
		msg1.Category = MP_CHAR;
		msg1.Protocol = MP_CHAR_LEVEL_NOTIFY;
		msg1.dwObjectID = GetID();
		msg1.Level = GetLevel();
		msg1.MaxExpPoint = GAMERESRCMNGR->GetMaxExpPoint(level);
		msg1.CurExpPoint = GetPlayerExpPoint();
		SendMsg(&msg1,sizeof(msg1));
	}
	
	ABILITYMGR->UpdateAbilityState(ABILITYUPDATE_CHARACTERLEVEL_CHANGED,GetLevel(),GetAbilityGroup());

	if(GetPartyIdx())
		PARTYMGR->MemberLevelUp(GetPartyIdx(), GetID(), GetLevel());
	if(GetGuildIdx())
		GUILDMGR->MemberLevelUp(GetGuildIdx(), GetID(), GetLevel());
}

void CPlayer::SetPlayerExpPoint(EXPTYPE point,BYTE bDie) 
{
	//ASSERT(GetLevel() <= MAX_CHARACTER_LEVEL_NUM);
	//if(GetLevel() == MAX_CHARACTER_LEVEL_NUM)
	ASSERT(GetLevel() <= GAMERESRCMNGR->GetMaxLevelControl()/*MAX_CHARACTER_LEVEL_NUM*/);
	if(GetLevel() == GAMERESRCMNGR->GetMaxLevelControl()+1/*MAX_CHARACTER_LEVEL_NUM*/)
		return;
	// ¢®¢´¨Ïo¡§¡þ¢®¡¿¡§u¢®A ¡§¢®¢®AAI¡§¢®¡Ëc AIAo A¡§uA¡§I!!!
	EXPTYPE pointForLevel = GAMERESRCMNGR->GetMaxExpPoint(GetLevel());

	if(point >= pointForLevel)
	{
		m_HeroInfo.ExpPoint = point - pointForLevel;

//		LEVELTYPE beforemaxLevel = GetMaxLevel();

		
//		LEVELTYPE aftermaxLevel = GetMaxLevel();

		// magi82 LevelUp ±¸°£È­ ¼öÁ¤ -> ·¹º§¾÷À» ÇßÀ»¶§ ÃÖ°í·¹º§°ú µ¿ÀÏÇØ¾ß¸¸ ½ºÅÝ ÇÃ·¡±×¸¦ ÃÊ±âÈ­(2·¹º§ ´Ù¿îÈÄ 1·¹º§¾÷ ÇßÀ»¶§ ÃÊ±âÈ­µÇ¸é ¾ÈµÇ¹Ç·Î ÀÛ¾÷ÇÔ)
		LEVELTYPE maxLevel = GetMaxLevel();
		//LEVELTYPE curLevel = GetLevel();
		SetLevel(m_HeroCharacterInfo.Level+1);
		LEVELTYPE NEWmaxLevel = GetMaxLevel();
		if( NEWmaxLevel > maxLevel)
		//if( maxLevel < GetLevel() && maxLevel == curLevel )
		{
//		if( maxLevel == curLevel )
//		{
			// ·¹º§¾÷ÇßÀ»¶§ÀÇ ½ºÅÝ ÁÖ´Â ÇÔ¼ö
			SetLevelUpStatPoint(); // magi82

			// ·¾¾÷À» ÇßÀ¸¹Ç·Î flag¸¦ ¸ðµÎ ÃÊ±âÈ­(´Ù½Ã 20% 40% 60% 80% ÀÏ¶§ ½ºÅÝ°ú »óÅÂÈ¸º¹À» Áà¾ßÇÏ´Ï±î..)
//			m_HeroInfo.ExpFlag = 0x00;
//			SetOnBit(&m_HeroInfo.ExpFlag, 4);	// magi82 LevelUp ±¸°£È­ ¼öÁ¤
//			UCharacterExpFlag(GetID(), m_HeroInfo.ExpFlag);
//			m_dwHpMpBitFlag = 0x00;
		}

		// YH
		CharacterHeroInfoUpdate(this);
		CharacterTotalInfoUpdate(this);
		
		// character info log
		InsertLogCharacter( GetID(), m_HeroCharacterInfo.Level, &m_HeroInfo );
		//LEVEL downA¨¬ reduceexppoint¢¯¢®¨ù¡© ©ø©÷¡¾e
		InsertLogExp( eExpLog_LevelUp, GetID(), GetLevel(), 0, GetPlayerExpPoint(), 0, 0, GetPlayerAbilityExpPoint());

		// Object AI¡§¡þ¢®I¡§¢®¡Ëc¡Ë?¡Ë¢ç ¡Ë¡ÍeCN A¡§uA¡§I¡Ë¡þ| CN¡Ë¡ÍU.
		CObjectEvent::ObjectEvent(this, OE_LEVELUP, &m_HeroCharacterInfo.Level);
	}
	else
	{
		m_HeroInfo.ExpPoint = point;

		// °æÇèÄ¡°¡ 20% 40% 60% 80% µÇ¾úÀ»¶§ ½ºÅÝÁÖ´Â ºÎºÐ
		LEVELTYPE maxLevel = GetMaxLevel();
		LEVELTYPE curLevel = GetLevel();
		if( maxLevel == curLevel )
		{
			SetNormalStatPoint();	// magi82
		}

		// to client -------------------------------------
		MSG_EXPPOINT msg;
		msg.Category = MP_CHAR;
		msg.Protocol = MP_CHAR_EXPPOINT_ACK;
		msg.dwObjectID = GetID();
		msg.ExpPoint = m_HeroInfo.ExpPoint;
		if(bDie == 0)
			msg.ExpKind = MSG_EXPPOINT::EXPPOINTKIND_ACQUIRE;
		else if(bDie == 1)
			msg.ExpKind = MSG_EXPPOINT::EXPPOINTKIND_DIE;

		SendMsg(&msg,sizeof(msg));
	}
}

// magi82 //////////////////////////////////////////////////////////////////////////

void CPlayer::SetLevelUpStatPoint()
{
	// ·¾¾÷ÇßÀ»°æ¿ì ·¾¾÷ÇÏ±âÀü ½ºÅÝÀ» ¸ø¹ÞÀº¸¸Å­ ¹ÞÀ½
	for( int i = 0; i < 4; i++ )
	{
		if(!CheckBit(m_HeroInfo.ExpFlag, i))
			SetPlayerLevelUpPoint(GetPlayerLevelUpPoint()+1);
	}

	m_HeroInfo.ExpFlag = 0x00;
	SetOnBit(&m_HeroInfo.ExpFlag, 4);	// magi82 LevelUp ±¸°£È­ ¼öÁ¤
	UCharacterExpFlag(GetID(), m_HeroInfo.ExpFlag);
	m_dwHpMpBitFlag = 0x00;	// magi82

	EXPTYPE maxexp = GAMERESRCMNGR->GetMaxExpPoint(GetLevel()+1);

	// ·¾¾÷ÈÄ¿¡ ³ª¸ÓÁö °æÇèÄ¡µéÀÌ ¿Ã¶úÀ»¶§ ½ºÅÝÆ÷ÀÎÆ® Á¦°øÇØÁÖ´Â ºÎºÐ(20% 40% 60% 80%)
	int count = int(( ( (float)m_HeroInfo.ExpPoint / (float)maxexp ) * 100 ) / 20);

	for( int i = 0; i < count; i++ )
	{
		if( !CheckBit(m_HeroInfo.ExpFlag, i) && i < 4 )
		{
			SetPlayerLevelUpPoint(GetPlayerLevelUpPoint()+1);
			SetOnBit(&m_HeroInfo.ExpFlag, i);
			UCharacterExpFlag(GetID(), m_HeroInfo.ExpFlag);

			SetLife(GetMaxLife());
			SetShield(GetMaxShield());
			SetNaeRyuk(GetMaxNaeRyuk());
		}
	}
}

void CPlayer::SetNormalStatPoint()
{
	EXPTYPE maxexp = GAMERESRCMNGR->GetMaxExpPoint( GetLevel() );

	// °æÇèÄ¡ ¿Ã¶úÀ»¶§ ½ºÅÝÆ÷ÀÎÆ® Á¦°øÇØÁÖ´Â ºÎºÐ(20% 40% 60% 80%)
	int count = int((((float)m_HeroInfo.ExpPoint / (float)maxexp)*100) / 20);

	for( int i = 0; i < count; i++ )
	{
        if( !CheckBit(m_HeroInfo.ExpFlag, i) && i < 4)
		{
			SetPlayerLevelUpPoint(GetPlayerLevelUpPoint()+1);
			SetOnBit(&m_HeroInfo.ExpFlag, i);
			UCharacterExpFlag(GetID(), m_HeroInfo.ExpFlag);

			CharacterHeroInfoUpdate(this);	// magi82(31)
		}
	}

	for( int i = 0; i < 4; i++ )
	{
		// 20 40 60 80 ÀÌ ³Ñ¾úÀ»¶§ Àû¿ëÇØÁÖ¸é¼­ TRUE
		if(count == i+1 && !CheckBit(m_dwHpMpBitFlag, i))
		{
			SetLife(GetMaxLife());
			SetShield(GetMaxShield());
			SetNaeRyuk(GetMaxNaeRyuk());
			m_dwHpMpBitFlag = 0x00;
			for( int j = 0; j < count; j++ )
                SetOnBit(&m_dwHpMpBitFlag, i);
		}
		// Á×¾î¼­ °æÇèÄ¡°¡ ³»·Á°¬À»¶§ FALSE
		if(count == i && CheckBit(m_dwHpMpBitFlag, i))
		{
			m_dwHpMpBitFlag = 0x00;
			for( int j = 0; j < count; j++ )
				SetOnBit(&m_dwHpMpBitFlag, i);
		}
	}
}

////////////////////////////////////////////////////////////////////////////////////

void CPlayer::AddPlayerExpPoint(EXPTYPE Exp)
{
	if( Exp == 0 ) return;
	if(GetLevel()==GAMERESRCMNGR->GetMaxLevelControl()/*99*/)        //99¼¶²»ÔÙÔö¼Ó¾­Ñé    by:ºúººÈý	QQ:112582793
	{
		Exp = 0;
	}
	EXPTYPE NewExp = GetPlayerExpPoint() + Exp;
	if(NewExp>GAMERESRCMNGR->GetMaxExpPoint(GetLevel()))
	{
		if((NewExp-GAMERESRCMNGR->GetMaxExpPoint(GetLevel()))>GAMERESRCMNGR->GetMaxExpPoint(GetLevel()+1))
			NewExp = GAMERESRCMNGR->GetMaxExpPoint(GetLevel());
		SetPlayerExpPoint(NewExp,TRUE);
		CharacterLvlExpUpdate(GetID(), GetLevel(), GetPlayerExpPoint());
		++m_KillExpFlag;
		return;
	}
	SetPlayerExpPoint(NewExp);
	SetPlayerExpPoint(NewExp,TRUE);
	CharacterLvlExpUpdate(GetID(), GetLevel(), GetPlayerExpPoint());
	++m_KillExpFlag;
	//SetPlayerExpPoint(NewExp);
}

void CPlayer::ReduceExpPoint(EXPTYPE minusExp, BYTE bLogType)
{
	LEVELTYPE minuslevel = 0;
	EXPTYPE CurExp = GetPlayerExpPoint();

	if(GetLevel() <= 1 && CurExp < minusExp)	//¡¤©ö¨¬¡×1A¨¬ 0¡¾iAo¢¬¢¬ ¡¾iAIùéU.
		minusExp = CurExp;

	InsertLogExp( bLogType, GetID(), GetLevel(), (DWORD)minusExp, GetPlayerExpPoint(), m_MurdererKind, m_MurdererIDX, GetPlayerAbilityExpPoint());
	
	while(1)
	{
		if(CurExp < minusExp)
		{
			minusExp -= CurExp;

			++minuslevel;
			CurExp = GAMERESRCMNGR->GetMaxExpPoint(GetLevel()-minuslevel) - 1;
			ASSERT(minuslevel<2);	//E¢´¨öA©ø¨£ C¨ª¨ù¡©
			if(minuslevel > 3)		//E¢´¨öA©ø¨£ CIùéA... ©ö¡ìCN¡¤cCA ©ö©¡Ao¢¯e
				break;
		}
		else
		{
			CurExp -= minusExp;
			break;
		}
	}

	if(minuslevel > 0)
	{
		SetLevel(m_HeroCharacterInfo.Level-minuslevel);
		
		// ·¹º§´Ù¿î ´çÇßÀ»¶§ÀÇ Ã³¸®
		// (´Ù¿î´çÇÑ ·¹º§¶§ÀÇ ½ºÅÝÀº ¹Ì¸® ´Ù ¹Þ¾ÒÀ¸¹Ç·Î
		// Flag¸¦ True·Î ÇÑ´Ù.)

		//m_HeroInfo.ExpFlag = 0x0F; // magi82
		//SetOnBit(&m_HeroInfo.ExpFlag, 4);	// magi82 LevelUp ±¸°£È­ ¼öÁ¤
		//UCharacterExpFlag(GetID(), m_HeroInfo.ExpFlag);	// magi82

		// »óÅÂÈ¸º¹ ¿ª½Ã ¸¶Âù°¡Áö·Î ¹Ì¸® ¹Þ¾ÒÀ¸¹Ç·Î Flag¸¦ True·Î ÇÑ´Ù.
		m_dwHpMpBitFlag = 0x0F;	// magi82

		// character info log
		InsertLogCharacter( GetID(), m_HeroCharacterInfo.Level, &m_HeroInfo );
	}
	SetPlayerExpPoint(CurExp,TRUE);	

	CharacterLvlExpUpdate(GetID(), GetLevel(), GetPlayerExpPoint());
}

void CPlayer::SetPlayerAbilityExpPoint(DWORD point,BOOL bUse) 
{
	// 2005 Å©¸®½º¸¶½º ÀÌº¥Å© ÄÚµå
	// ´« ³»¸±¶§´Â ¼ö·ÃÄ¡ 10 Áõ°¡
	if( WEATHERMGR->GetWeatherState() == eWS_Snow )
		point += 10;

	DWORD FromAbilityExp = m_AbilityGroup.GetAbilityExp();
	DWORD ChangeAbilityExp = point - FromAbilityExp;
	if((point > 1000000000 || ChangeAbilityExp > 1000000000) && GetUserLevel() > eUSERLEVEL_GM )
	{
		AbilityLevelupLog(this,
			0,99,
			FromAbilityExp,point);
		
		return;
	}
	
	m_AbilityGroup.SetAbilityExp(point);

	// to client -------------------------------------
	MSG_ABILLITY_EXPPOINT msg;
	msg.Category = MP_CHAR;
	msg.Protocol = MP_CHAR_ABILITYEXPPOINT_ACK;
	msg.dwObjectID = GetID();
	msg.ExpPoint = GetPlayerAbilityExpPoint();
	if(bUse == 0)
		msg.ExpKind = MSG_ABILLITY_EXPPOINT::EXPPOINTKIND_ACQUIRE;
	else
		msg.ExpKind = MSG_ABILLITY_EXPPOINT::EXPPOINTKIND_ABILITYUSE;

	SendMsg(&msg,sizeof(msg));
}
void CPlayer::AddAbilityExp(DWORD AbilityExp)
{
	DWORD NewExp = GetPlayerAbilityExpPoint() + AbilityExp;

	SetPlayerAbilityExpPoint(NewExp);
}

DWORD CPlayer::AddExpPointToMugong(WORD MugongIdx,DWORD exp,BOOL bSuryunComplete)
{
	// 10000À» ³ÑÀ¸¸é Å¸ÀÌÅº ½ºÅ³ Á¤º¸ÀÌ¹Ç·Î »©Áà¾ßÇÔ
	//if(MugongIdx >= SKILLNUM_TO_TITAN)
	//{
	//	MugongIdx = MugongIdx - SKILLNUM_TO_TITAN;
	//}

	MUGONGBASE* pMugong = GetMugongBaseByMugongIdx(MugongIdx);
	if(pMugong == NULL)
		return 0;

	if(pMugong->dwDBIdx == 0)
		return 0;
	DWORD point =0;
	pMugong->ExpPoint += exp;

	//¨ú¡À¥ì¡ÍAI¨¡¢ç CE¢¯aCIùéU¡Æi C¡Í¨öA
	pMugong->bWear = TRUE;
	
	CSkillInfo* pSkillInfo = SKILLMGR->GetSkillInfo(pMugong->wIconIdx);
	// YH2DO
	if(pSkillInfo == NULL)
		return 0;
	
	point = pSkillInfo->GetNeedExp(pMugong->Sung);
	if(point == DWORD(-1))
		return 0;

	if(SURYUNMGR->NeedSuryun(pMugong->wIconIdx,pMugong->Sung,pMugong->ExpPoint))
		pMugong->ExpPoint = point - 1;

	if(bSuryunComplete)
	{
		pMugong->ExpPoint = 0;
		BYTE upSung = pMugong->Sung+1;
		if( pSkillInfo->GetNeedExp(pMugong->Sung+1) == DWORD(-1) )
		{
			upSung = 12;
		}
		SetPlayerMugongSung(pMugong->Position, upSung ,eLog_MugongLevelup);
	}
	else if(pMugong->ExpPoint >= point)
	{
		pMugong->ExpPoint = pMugong->ExpPoint - point;
		SetPlayerMugongSung(pMugong->Position, pMugong->Sung+1,eLog_MugongLevelup);
	}
	
	MSG_MUGONGEXP msg;
	msg.Category = MP_MUGONG;
	msg.Protocol = MP_MUGONG_EXPPOINT_NOTIFY;
	msg.absPos = pMugong->Position;
	msg.ExpPoint = pMugong->ExpPoint;
	SendMsg(&msg,sizeof(msg));
	
	return pMugong->ExpPoint;
}

void CPlayer::SetPlayerMugongSung(POSTYPE abspos, BYTE sung, WORD Reason)
{
	MUGONGBASE* pMugong = GetMugongBase(abspos);
	ASSERT(pMugong);
	if(pMugong == NULL)
		return;
	pMugong->Sung = sung;

	MSG_WORD3 msg;
	msg.Category = MP_MUGONG;
	msg.Protocol = MP_MUGONG_SUNG_NOTIFY;
	msg.dwObjectID = GetID();
	msg.wData1 = abspos;
	msg.wData2 = sung;
	msg.wData3 = Reason;
	SendMsg(&msg,sizeof(msg));

	MSGBASE msg1;
	SetProtocol( &msg1, MP_MUGONG, MP_MUGONG_SUNG_LEVELUP );
	msg1.dwObjectID = GetID();
	PACKEDDATA_OBJ->QuickSendExceptObjectSelf( this, &msg1, sizeof(msg1) );

	//db ¡§uod¡Ë?a
	MugongUpdateToDB(pMugong, "MUGONGSUNG");

	// Log ¨Ïo¢®i¢®¨¡¨Ï¨£¡§u¡§¡þAC ¡§¡þ?E¢®¨Ï
	InsertLogMugong( Reason, GetID(), pMugong->wIconIdx, pMugong->dwDBIdx, sung, pMugong->ExpPoint );

//	CharacterMugongUpdate(pMugong);
}

void CPlayer::MugongLevelDown(WORD MugongIdx, WORD Reason)
{
	MUGONGBASE* pMugong = GetMugongBaseByMugongIdx(MugongIdx);
	if(pMugong == NULL)
		return;

	if(pMugong->dwDBIdx == 0 || pMugong->Sung == 0)
		return;
	
	CSkillInfo* pSkillInfo = SKILLMGR->GetSkillInfo(pMugong->wIconIdx);
	if(pSkillInfo == NULL)
		return;
	
	SKILL_CHANGE_INFO* pSkillChangeInfo = SKILLMGR->GetSkillChangeInfobyDeadlyMugongIdx(pMugong->wIconIdx);
	if(pSkillChangeInfo) // back to before mugong
	{
		MUGONGMNGR->ChangeMugong(this, MugongIdx, pMugong->Position, pSkillChangeInfo->wMugongIdx, eLog_MugongDestroyByBadFameJulChoDown);
	}
	else
	{
		float fRate = pMugong->ExpPoint / (float)pSkillInfo->GetNeedExp(pMugong->Sung);
		
		BYTE newSung = pMugong->Sung - 1;
		DWORD newExp = newSung ? (DWORD)(pSkillInfo->GetNeedExp(newSung)*fRate) : 0;
		pMugong->ExpPoint = newExp;
		SetPlayerMugongSung(pMugong->Position,newSung,Reason);
	}
	//¨ú¡À¥ì¡ÍAI¨¡¢ç CE¢¯aCIùéU¡Æi C¡Í¨öA
	pMugong->bWear = TRUE;
}

BYTE CPlayer::GetLifePercent()
{
	BYTE lifepercent = (BYTE)((GetLife() / (float)GetMaxLife()) * 100);
	return lifepercent;
}
BYTE CPlayer::GetShieldPercent()
{
	BYTE Shieldpercent = (BYTE)((GetShield() / (float)GetMaxShield()) * 100);
	return Shieldpercent;
}

BYTE CPlayer::GetNaeRyukPercent()
{
	BYTE naeryukpercent = (BYTE)((m_HeroInfo.wNaeRyuk / (float)GetMaxNaeRyuk()) * 100);
	return naeryukpercent;
}
void CPlayer::OnStartObjectState(BYTE State,DWORD dwParam)
{
	switch(State)
	{
	case eObjectState_Ungijosik:
		m_LifeRecoverTime.lastCheckTime = gCurTime;
		m_NaeRyukRecoverTime.lastCheckTime = gCurTime;
		m_ShieldRecoverTime.lastCheckTime = gCurTime;
		break;
	}
}

void CPlayer::OnEndObjectState(BYTE State)
{
	switch(State)
	{

	case eObjectState_Die:
		GetBattle()->OnTeamMemberRevive(GetBattleTeam(),this);
		m_bNeedRevive = TRUE;
		break;
	}

}

WORD CPlayer::GetWeaponEquipType()
{
	WORD WeaponItemIdx = GetWearedWeapon();
	if(WeaponItemIdx == 0)
		return WP_GWUN;
	ITEM_INFO * pItemInfo = ITEMMGR->GetItemInfo(WeaponItemIdx);

	// ÀÏ´Ü ¼­¹ö Á×´Â°Å ¶«½Ã ¸·À½... tamoo
	if( pItemInfo )
		return pItemInfo->WeaponType;
	return 0;
//	ASSERT(pItemInfo);
//	return pItemInfo->WeaponType;
}

WORD CPlayer::GetTitanWeaponEquipType()
{
	if( !InTitan() )
		return 0;

	CItemSlot * pSlot =GetSlot(eItemTable_Titan);
	const ITEMBASE* pItemBase = pSlot->GetItemInfoAbs(TP_TITANWEAR_START + eTitanWearedItem_Weapon);
	ITEM_INFO* pInfo = ITEMMGR->GetItemInfo(pItemBase->wIconIdx);
	if(pInfo && pInfo->WeaponType != 0)
		return pInfo->WeaponType;
	else if( pItemBase->wIconIdx == 0 )
		return WP_GWUN;
	else
		return 0;
}
/*
void CPlayer::Revive()
{	
	MOVE_POS msg;
	msg.Category = MP_USERCONN;
	msg.Protocol = MP_USERCONN_CHARACTER_REVIVE;
	msg.dwObjectID = GetID();
	msg.dwMoverID = GetID();
	
	WORD ServerMapNum = (WORD)GAMERESRCMNGR->GetLoadMapNum();
	
	VECTOR3 pos,RandPos;
	GAMERESRCMNGR->GetRegenPoint(pos,RandPos,m_HeroCharacterInfo.LoginPoint_Index,m_HeroCharacterInfo.MapChangePoint_Index);
	
	pos = g_pServerSystem->GetMap()->GetTileManager()->GetNextCloserPos(this,&RandPos,&pos,70,170);
	
	msg.cpos.Compress(&pos);
	
	//A¨Ï©ª¡Ë¡þ?AI ¢®iy¡Ë¡þi¢®¢´A Full¢®¢´I A¡Ëùé¡Ë?o
	SetLife(m_HeroCharacterInfo.MaxLife);
	
	CCharMove::SetPosition(this,&pos);
	
	PACKEDDATA_OBJ->QuickSend(this,&msg,sizeof(msg));
}
*/

void CPlayer::MoveToLogInPosition()
{	
	//g_Console.LOC(0xFFFF66, 4, "MoveToLogInPosition()");
	MOVE_POS msg;
	msg.Category = MP_USERCONN;
	msg.Protocol = MP_USERCONN_CHARACTER_REVIVE;
	msg.dwObjectID = GetID();
	msg.dwMoverID = GetID();
	
	/*MOVE_POS posMsg;
	posMsg.Category = MP_MOVE;
	posMsg.Protocol = MP_MOVE_RECONN_POS_ACK;
	posMsg.dwObjectID = GetID();
	posMsg.dwMoverID = GetID();*/
	
	WORD ServerMapNum = (WORD)GAMERESRCMNGR->GetLoadMapNum();
	
	VECTOR3 pos,RandPos;
	if( BATTLESYSTEM->GetBattle( this )->GetBattleKind() == eBATTLE_KIND_SIEGEWAR )
	{
		DWORD LoginIndex = 0;
		if( GetBattleTeam() == eBattleTeam1 )			LoginIndex = 1103;
		else											LoginIndex = 1102;
		GAMERESRCMNGR->GetRegenPoint(pos,RandPos,m_HeroCharacterInfo.LoginPoint_Index,(WORD)LoginIndex);
	}
	else
		GAMERESRCMNGR->GetRegenPoint(pos,RandPos,m_HeroCharacterInfo.LoginPoint_Index,m_HeroCharacterInfo.MapChangePoint_Index);
	
	pos = RandPos;//g_pServerSystem->GetMap()->GetTileManager()->GetNextCloserPos(this,&RandPos,&pos,70,170);

	
	//posMsg.cpos.Compress(&pos);
	
	msg.cpos.Compress(&pos);
	
	CCharMove::SetPosition(this,&pos);
	CPet* pPet = m_PetManager.GetCurSummonPet();
	if(pPet)
	{
		CCharMove::SetPosition(pPet,&pos);
		MOVE_POS	Msg;
		Msg.Category = MP_MOVE;
		Msg.Protocol = MP_MOVE_PET_WARP_ACK;
		Msg.dwObjectID = GetID();
		Msg.dwMoverID = pPet->GetID();
		Msg.cpos.Compress(&pos);
		PACKEDDATA_OBJ->QuickSend(this,&Msg,sizeof(Msg));
	}

	PACKEDDATA_OBJ->QuickSend(this,&msg,sizeof(msg));
		
	//OBJECTSTATEMGR_OBJ->EndObjectState(this,eObjectState_Die);

	//if( IsActionPanelty() == FALSE )
	{
	//	SetActionPanelty();
	//	InsetNoActionPaneltyLog( eExpLog_NoActionPanelty_Login );
	}
	//else
	//	ReviveLogInPenelty();

	//OBJECTSTATEMGR_OBJ->StartObjectState(this,eObjectState_Immortal,0);

	//OBJECTSTATEMGR_OBJ->EndObjectState( this, eObjectState_Immortal, 30000 );

	
	//m_YYLifeRecoverTime.bStart = FALSE;
	//m_YYNaeRyukRecoverTime.bStart = FALSE;


	if(m_HeroInfo.PartyID)
	{	
		CParty* pParty = PARTYMGR->GetParty(m_HeroInfo.PartyID);
		if(!pParty)		return;

		SEND_PARTYICON_REVIVE msg;		
		msg.Category = MP_PARTY;
		msg.Protocol = MP_PARTY_REVIVEPOS;
		msg.dwMoverID = m_BaseObjectInfo.dwObjectID;
		msg.Pos.posX = (WORD)pos.x;
		msg.Pos.posZ = (WORD)pos.z;
		pParty->SendMsgToAll(&msg, sizeof(msg));
	}
	//ClearMurderIdx();

	//m_bDieInSpecialMap = FALSE;
}

void CPlayer::ReviveAfterVimu( BOOL bSendMsg )
{
	ClearMurderIdx();
	m_bNeedRevive = TRUE;

	if( bSendMsg )
	{
		MOVE_POS msg;
		msg.Category = MP_USERCONN;
		msg.Protocol = MP_USERCONN_CHARACTER_REVIVE;
		msg.dwObjectID = GetID();
		msg.dwMoverID = GetID();
	
		msg.cpos.Compress(CCharMove::GetPosition(this));
		
		PACKEDDATA_OBJ->QuickSend(this,&msg,sizeof(msg));
	}

	OBJECTSTATEMGR_OBJ->EndObjectState(this,eObjectState_Die);

	m_YYLifeRecoverTime.bStart = FALSE;
	m_YYNaeRyukRecoverTime.bStart = FALSE;
	SetLife( GetMaxLife() * 30 / 100 );	//¡§uoA¡Ë¢ç ¡§ui¡ËO¢®i¢®¨¡O CO¢®¨úi.
}


void CPlayer::RevivePresentSpot()
{	
	//g_Console.LOC(0xFFFF66, 4, "RevivePresentSpot()");
	if(GetState() != eObjectState_Die)
	{
		ASSERT(0);
		MSG_BYTE msg;
		msg.Category	= MP_USERCONN;
		msg.Protocol	= MP_USERCONN_CHARACTER_REVIVE_NACK;
		msg.bData		= 1; //errorcode
		SendMsg( &msg, sizeof(msg) );

		return;
	}

	if( LOOTINGMGR->IsLootedPlayer( GetID() ) )	//¢®¢´c¡§¢®AA¨Ï¡þ¡Ë?¢®I
	{
		MSG_BYTE msg;
		msg.Category	= MP_USERCONN;
		msg.Protocol	= MP_USERCONN_CHARACTER_REVIVE_NACK;
		msg.bData		= 2; //errorcode
		SendMsg( &msg, sizeof(msg) );

		return;
	}

	if( IsExitStart() )
	{
		MSG_BYTE msg;
		msg.Category	= MP_USERCONN;
		msg.Protocol	= MP_USERCONN_CHARACTER_REVIVE_NACK;
		msg.bData		= 4; //errorcode
		SendMsg( &msg, sizeof(msg) );

		return;
	}

	m_bNeedRevive = TRUE;
	MOVE_POS msg;
	msg.Category = MP_USERCONN;
	msg.Protocol = MP_USERCONN_CHARACTER_REVIVE;
	msg.dwObjectID = GetID();
	msg.dwMoverID = GetID();
	
	WORD ServerMapNum = (WORD)GAMERESRCMNGR->GetLoadMapNum();
	/*if (FORTWARMGR->IsFortWaring(this))
	{
		VECTOR3 FortPos;
		srand(time(NULL));
		int randNum = (rand() % 3);

		FortPos.x = FORTWARMGR->GetSpawnVector(ServerMapNum, randNum).x;
		FortPos.y = FORTWARMGR->GetSpawnVector(ServerMapNum, randNum).y;
		FortPos.z = FORTWARMGR->GetSpawnVector(ServerMapNum, randNum).y;
		msg.cpos.Compress(&FortPos);
		CCharMove::SetPosition(this, &FortPos);
		CPet* pPet = m_PetManager.GetCurSummonPet();
		if (pPet)
		{
			CCharMove::SetPosition(pPet, &FortPos);
			MOVE_POS	Msg;
			Msg.Category = MP_MOVE;
			Msg.Protocol = MP_MOVE_PET_WARP_ACK;
			Msg.dwObjectID = GetID();
			Msg.dwMoverID = pPet->GetID();
			Msg.cpos.Compress(&FortPos);
			PACKEDDATA_OBJ->QuickSend(this, &Msg, sizeof(Msg));
		}

		//PACKEDDATA_OBJ->QuickSend(this, &msg, sizeof(msg));

		//return;
	}
	else*/
	{
		msg.cpos.Compress(CCharMove::GetPosition(this));

		//PACKEDDATA_OBJ->QuickSend(this, &msg, sizeof(msg));
	}
	
		
	PACKEDDATA_OBJ->QuickSend(this,&msg,sizeof(msg));

#ifdef _KOR_LOCAL_
	if( IsActionPanelty() == FALSE )
	{
		SetActionPanelty();
		InsetNoActionPaneltyLog( eExpLog_NoActionPanelty_Present );
	}
	else
#endif
	{

	//if( g_pServerSystem->GetMapNum() != nakyang_siege &&
	//	g_pServerSystem->GetMapNum() != Tournament )
	DWORD dwCheckBit = eSiegeWarMap | eTournament | eEventMap | eQuestRoom | eSurvivalMap;
	if( !(CHANNELSYSTEM->IsBattleChannel(GetChannelID()) && IsDieForBattleChannel()) )	//SW080515 Ã¤³Îº° ¼¼ÆÃ
	if( FALSE == g_pServerSystem->GetMap()->IsMapKind(dwCheckBit) )
	//if( !EVENTMAPMGR->IsEventMap() &&
	//	!QUESTMAPMGR->IsQuestMap() &&
	//	g_pServerSystem->GetMapNum() != RUNNINGMAP &&
	//	g_pServerSystem->GetMapNum() != PKEVENTMAP &&
	//	!g_pServerSystem->CheckMapKindIs(eSurvival) )

//	if( !EVENTMAPMGR->IsEventMap()  && g_pServerSystem->GetMapNum() != RUNNINGMAP 
//		&& g_pServerSystem->GetMapNum() != PKEVENTMAP
//		&& !g_pServerSystem->CheckMapKindIs(eSurvival) )
	if(GetLevel() >= 5 && m_bDieForGFW == FALSE )
	{
		// RaMa -> °æÇèÄ¡+µ·º¸È£
		DWORD ItemIdx = 0;
		SHOPITEMWITHTIME* pShopItem = NULL;
		if( GetShopItemStats()->ProtectCount )		
		{
			DWORD ItemIdx = m_ShopItemManager.GetProtectItemIdx();
			if( ItemIdx == 0 )
				goto ProtectAll_UseFail;
			pShopItem = m_ShopItemManager.GetUsingItemInfo( ItemIdx );
			if( !pShopItem )
				goto ProtectAll_UseFail;

			// LogÃß°¡
			LogItemMoney( GetID(), GetObjectName(), 0, "",
				eLog_ShopItemProtectAll, GetMoney(eItemTable_Inventory), 0, 0,
				ItemIdx, pShopItem->ShopItem.ItemBase.dwDBIdx, pShopItem->ShopItem.ItemBase.Position, 0, pShopItem->ShopItem.ItemBase.Durability, GetPlayerExpPoint());
			InsertLogExp( eExpLog_ProtectExp, GetID(), GetLevel(), GetShopItemStats()->ProtectCount, GetPlayerExpPoint(), m_MurdererKind, m_MurdererIDX, GetPlayerAbilityExpPoint());

			--GetShopItemStats()->ProtectCount;
			if( GetShopItemStats()->ProtectCount <= 0 )
			{
				m_ShopItemManager.SendMsgDwordToPlayer(MP_ITEM_SHOPITEM_USEEND, ItemIdx);
				m_ShopItemManager.DeleteUsingShopItem( ItemIdx );
				m_ShopItemManager.SetProtectItemIdx( 0 );
			}
			else
			{
				// DB¿¡ ¾÷µ¥ÀÌÆ®
				ShopItemUseParamUpdateToDB( 0,  GetID(), ItemIdx, GetShopItemStats()->ProtectCount );
			}
			m_ShopItemManager.SendMsgDwordToPlayer(MP_ITEM_SHOPITEM_PROTECTALL, GetShopItemStats()->ProtectCount);
		}
		else
		{
ProtectAll_UseFail:
			// RaMa -> µ·º¸È£ ÁÖ¹®¼­
			if( m_ShopItemManager.GetUsingItemInfo( eIncantation_MoneyProtect ) )
			{
				m_ShopItemManager.SendMsgDwordToPlayer(MP_ITEM_SHOPITEM_MONEYPROTECT, eIncantation_MoneyProtect);
				m_ShopItemManager.DeleteUsingShopItem( eIncantation_MoneyProtect );
			}
			else
			{
				CItemSlot* pSlot = m_ItemContainer.GetSlot(eItemTable_Inventory);
				DWORD CurMoney = pSlot->GetMoney();
				if( !m_bDieForGFW )
					SetMoney((MONEYTYPE)(CurMoney*0.06), MONEY_SUBTRACTION, MF_LOST, eItemTable_Inventory, eMoneyLog_LoseDie, 0);

//				if( m_bDieForGFW )
//					SetMoney((MONEYTYPE)(CurMoney*0.03), MONEY_SUBTRACTION, MF_LOST, eItemTable_Inventory, eMoneyLog_LoseDie, 0);
//				else
//					SetMoney((MONEYTYPE)(CurMoney*0.06), MONEY_SUBTRACTION, MF_LOST, eItemTable_Inventory, eMoneyLog_LoseDie, 0);
			}
			
			// RaMa -> °æÇèÄ¡º¸È£ ÁÖ¹®¼­
			if( m_ShopItemManager.GetUsingItemInfo( eIncantation_ExpProtect ) )
			{
				m_ShopItemManager.SendMsgDwordToPlayer(MP_ITEM_SHOPITEM_EXPPROTECT, eIncantation_ExpProtect);
				m_ShopItemManager.DeleteUsingShopItem( eIncantation_ExpProtect );
			}
			else
			{
				EXPTYPE pointForLevel = GAMERESRCMNGR->GetMaxExpPoint(GetLevel());
				EXPTYPE minusExp = 0;

				// magi82 //////////////////////////////////////////////////////////////////////////

				float fpa = 3.0f;
				LEV_PENALTY* pTemp = GAMERESRCMNGR->GetLevPenalty(GetLevel());
				if( pTemp )
					fpa = pTemp->fNow;

				float fRate = fpa * 0.01f;

				if( !m_bDieForGFW )
					minusExp = (EXPTYPE)(pointForLevel * fRate);


//				if( !m_bDieForGFW )	minusExp = (EXPTYPE)(pointForLevel * 0.03);
//				if( m_bDieForGFW )	minusExp = (EXPTYPE)(pointForLevel * 0.015);
//				else				minusExp = (EXPTYPE)(pointForLevel * 0.03);


				////////////////////////////////////////////////////////////////////////////////////

				ReduceExpPoint(minusExp, eExpLog_LosebyRevivePresent);

				//SW070531 Æê Ä£¹Ðµµ º¸È£±â´É
				m_PetManager.ReducePetFriendshipWithMastersRevive();
			}
		}
	}

	}	// _KOR_LOCAL_

	OBJECTSTATEMGR_OBJ->EndObjectState(this,eObjectState_Die);
	
	DWORD MaxLife = GetMaxLife();
	DWORD MaxNaeryuk = GetMaxNaeRyuk();
	DWORD MaxShield = GetMaxShield();
	SetLife((DWORD)(MaxLife*0.3));
	SetNaeRyuk(0);
	SetShield((DWORD)(MaxShield*0.3));
	
	m_YYLifeRecoverTime.bStart = FALSE;
	m_YYNaeRyukRecoverTime.bStart = FALSE;
	ClearMurderIdx();

	m_bDieForBattleChannel = FALSE;	//SW080515 Ã¤³Îº° ¼¼ÆÃ
	m_bDieForGFW = FALSE;
	m_bDieInSpecialMap = FALSE;
}

void CPlayer::ReviveLogIn()
{	
	
	
	if(GetState() != eObjectState_Die)
	{
		ASSERT(0);
		MSG_BYTE msg;
		msg.Category	= MP_USERCONN;
		msg.Protocol	= MP_USERCONN_CHARACTER_REVIVE_NACK;
		msg.bData		= 1; //errorcode
		SendMsg( &msg, sizeof(msg) );

		return;
	}
	
	if( LOOTINGMGR->IsLootedPlayer( GetID() ) )	//¢®¢´c¡§¢®AA¨Ï¡þ¡Ë?¢®I
	{
		MSG_BYTE msg;
		msg.Category	= MP_USERCONN;
		msg.Protocol	= MP_USERCONN_CHARACTER_REVIVE_NACK;
		msg.bData		= 2; //errorcode
		SendMsg( &msg, sizeof(msg) );

		return;
	}

	if( IsExitStart() )
	{
		MSG_BYTE msg;
		msg.Category	= MP_USERCONN;
		msg.Protocol	= MP_USERCONN_CHARACTER_REVIVE_NACK;
		msg.bData		= 4; //errorcode
		SendMsg( &msg, sizeof(msg) );

		return;
	}

	m_bNeedRevive = TRUE;
	MOVE_POS msg;
	msg.Category = MP_USERCONN;
	msg.Protocol = MP_USERCONN_CHARACTER_REVIVE;
	msg.dwObjectID = GetID();
	msg.dwMoverID = GetID();
	
	WORD ServerMapNum = (WORD)GAMERESRCMNGR->GetLoadMapNum();
	
	VECTOR3 pos,RandPos,FortPos;
	if( BATTLESYSTEM->GetBattle( this )->GetBattleKind() == eBATTLE_KIND_SIEGEWAR )
	{
		DWORD LoginIndex = 0;
		if( GetBattleTeam() == eBattleTeam1 )			LoginIndex = 1103;
		else											LoginIndex = 1102;
		GAMERESRCMNGR->GetRegenPoint(pos,RandPos,m_HeroCharacterInfo.LoginPoint_Index,(WORD)LoginIndex);
	}
	else
		GAMERESRCMNGR->GetRegenPoint(pos,RandPos,m_HeroCharacterInfo.LoginPoint_Index,m_HeroCharacterInfo.MapChangePoint_Index);
	


	{
		pos = RandPos;
	}
	
	msg.cpos.Compress(&pos);
	
	CCharMove::SetPosition(this,&pos);
	CPet* pPet = m_PetManager.GetCurSummonPet();
	if(pPet)
	{
		CCharMove::SetPosition(pPet,&pos);
		MOVE_POS	Msg;
		Msg.Category = MP_MOVE;
		Msg.Protocol = MP_MOVE_PET_WARP_ACK;
		Msg.dwObjectID = GetID();
		Msg.dwMoverID = pPet->GetID();
		Msg.cpos.Compress(&pos);
		PACKEDDATA_OBJ->QuickSend(this,&Msg,sizeof(Msg));
	}

	PACKEDDATA_OBJ->QuickSend(this,&msg,sizeof(msg));
		
	OBJECTSTATEMGR_OBJ->EndObjectState(this,eObjectState_Die);

	if( IsActionPanelty() == FALSE )
	{
		SetActionPanelty();
		InsetNoActionPaneltyLog( eExpLog_NoActionPanelty_Login );
	}
	else
		ReviveLogInPenelty();

	OBJECTSTATEMGR_OBJ->StartObjectState(this,eObjectState_Immortal,0);

	OBJECTSTATEMGR_OBJ->EndObjectState( this, eObjectState_Immortal, 30000 );

	
	m_YYLifeRecoverTime.bStart = FALSE;
	m_YYNaeRyukRecoverTime.bStart = FALSE;


	if(m_HeroInfo.PartyID)
	{	
		CParty* pParty = PARTYMGR->GetParty(m_HeroInfo.PartyID);
		if(!pParty)		return;

		SEND_PARTYICON_REVIVE msg;		
		msg.Category = MP_PARTY;
		msg.Protocol = MP_PARTY_REVIVEPOS;
		msg.dwMoverID = m_BaseObjectInfo.dwObjectID;
		msg.Pos.posX = (WORD)pos.x;
		msg.Pos.posZ = (WORD)pos.z;
		pParty->SendMsgToAll(&msg, sizeof(msg));
	}
	ClearMurderIdx();

	m_bDieInSpecialMap = FALSE;
}

void CPlayer::ReviveLogInPenelty()
{
	// RaMa -04.11.24   ->ÇÁ¸®¹Ì¾ö ¾ÆÀÌÅÛÀº ÆÐ³ÎÆ¼ ¾øÀ½.
	// RaMa -04.12.14¼öÁ¤   ->°æÇèÄ¡¿Í µ·À¸·Î ³ª´©°í, °á°ú¸¦ Client·Î º¸³¿

	//if( g_pServerSystem->GetMapNum() != nakyang_siege &&
	//	g_pServerSystem->GetMapNum() != Tournament )
	//if( !EVENTMAPMGR->IsEventMap() &&
	//	!QUESTMAPMGR->IsQuestMap() &&
	//	g_pServerSystem->GetMapNum() != RUNNINGMAP &&
	//	g_pServerSystem->GetMapNum() != PKEVENTMAP &&
	//	!g_pServerSystem->CheckMapKindIs(eSurvival) )

	DWORD dwCheckBit = eSiegeWarMap | eTournament | eEventMap | eQuestRoom | eSurvivalMap;
	if( !(CHANNELSYSTEM->IsBattleChannel(GetChannelID()) && IsDieForBattleChannel()) )	//SW080515 Ã¤³Îº° ¼¼ÆÃ
	if( FALSE == g_pServerSystem->GetMap()->IsMapKind(dwCheckBit) )
	if(GetLevel() >= 5 && m_bDieForGFW == FALSE )
	{		
		BOOL bExchanged = FALSE;
		// RaMa -> °æÇèÄ¡+µ·º¸È£
		DWORD ItemIdx = 0;
		SHOPITEMWITHTIME* pShopItem = NULL;
		if( GetShopItemStats()->ProtectCount )		
		{
			DWORD ItemIdx = m_ShopItemManager.GetProtectItemIdx();
			if( ItemIdx == 0 )
				goto ProtectAll_UseFail;
			pShopItem = m_ShopItemManager.GetUsingItemInfo( ItemIdx );
			if( !pShopItem )
				goto ProtectAll_UseFail;

			// LogÃß°¡
			LogItemMoney( GetID(), GetObjectName(), 0, "",
				eLog_ShopItemProtectAll, GetMoney(eItemTable_Inventory), 0, 0,
				ItemIdx, pShopItem->ShopItem.ItemBase.dwDBIdx, pShopItem->ShopItem.ItemBase.Position, 0, pShopItem->ShopItem.ItemBase.Durability, GetPlayerExpPoint());
			InsertLogExp( eExpLog_ProtectExp, GetID(), GetLevel(), GetShopItemStats()->ProtectCount, GetPlayerExpPoint(), m_MurdererKind, m_MurdererIDX, GetPlayerAbilityExpPoint());

			--GetShopItemStats()->ProtectCount;
			if( GetShopItemStats()->ProtectCount <= 0 )
			{
				m_ShopItemManager.SendMsgDwordToPlayer(MP_ITEM_SHOPITEM_USEEND, ItemIdx);
				m_ShopItemManager.DeleteUsingShopItem( ItemIdx );
				m_ShopItemManager.SetProtectItemIdx( 0 );
			}
			else
			{
				// DB¿¡ ¾÷µ¥ÀÌÆ®
				ShopItemUseParamUpdateToDB( 0, GetID(), ItemIdx, GetShopItemStats()->ProtectCount );
			}
			m_ShopItemManager.SendMsgDwordToPlayer(MP_ITEM_SHOPITEM_PROTECTALL, GetShopItemStats()->ProtectCount);
		}
		else
		{
ProtectAll_UseFail:
			if( m_ShopItemManager.GetUsingItemInfo( eIncantation_MoneyProtect ) )
			{
				m_ShopItemManager.SendMsgDwordToPlayer(MP_ITEM_SHOPITEM_MONEYPROTECT, eIncantation_MoneyProtect);
				m_ShopItemManager.DeleteUsingShopItem( eIncantation_MoneyProtect );
			}
			else
			{
				CItemSlot* pSlot = m_ItemContainer.GetSlot(eItemTable_Inventory);
				DWORD CurMoney = pSlot->GetMoney();
				if( !m_bDieForGFW )
					SetMoney( (MONEYTYPE)(CurMoney*0.04), MONEY_SUBTRACTION, MF_LOST , eItemTable_Inventory, eMoneyLog_LoseDie, 0 );
//				if( m_bDieForGFW )
//					SetMoney( (MONEYTYPE)(CurMoney*0.02), MONEY_SUBTRACTION, MF_LOST , eItemTable_Inventory, eMoneyLog_LoseDie, 0 );
//				else
//					SetMoney( (MONEYTYPE)(CurMoney*0.04), MONEY_SUBTRACTION, MF_LOST , eItemTable_Inventory, eMoneyLog_LoseDie, 0 );
			}

			// RaMa - 04.12.28     ->ÁÖ¹®¼­Ãß°¡
			if( m_ShopItemManager.GetUsingItemInfo( eIncantation_ExpProtect ) )
			{
				m_ShopItemManager.SendMsgDwordToPlayer(MP_ITEM_SHOPITEM_EXPPROTECT, eIncantation_ExpProtect);
				m_ShopItemManager.DeleteUsingShopItem( eIncantation_ExpProtect );
			}
			else
			{
				EXPTYPE pointForLevel = GAMERESRCMNGR->GetMaxExpPoint(GetLevel());
				EXPTYPE minusExp = 0;				

				// magi82 //////////////////////////////////////////////////////////////////////////

				float fpa = 2.0f;
				LEV_PENALTY* pTemp = GAMERESRCMNGR->GetLevPenalty(GetLevel());
				if( pTemp )
					fpa = pTemp->fSave;

				float fRate = fpa * 0.01f;

				////////////////////////////////////////////////////////////////////////////////////

				if( GetBattle()->GetBattleKind() == eBATTLE_KIND_SIEGEWAR )
					fRate = 0.01f;

				if( !m_bDieForGFW )	minusExp = (EXPTYPE)(pointForLevel * fRate);				
//				if( m_bDieForGFW )	minusExp = (EXPTYPE)(pointForLevel * 0.01);
//				else				minusExp = (EXPTYPE)(pointForLevel * 0.02);				

				ReduceExpPoint(minusExp, eExpLog_LosebyReviveLogIn);

				//SW070531 Æê Ä£¹Ðµµ º¸È£±â´É
				m_PetManager.ReducePetFriendshipWithMastersRevive();
			}
		}

		// RaMa - ÃßÈÄÃß°¡
/*		if( bExchanged && m_ShopItemOption.ExpPeneltyPoint==0 && m_ShopItemOption.MoneyPeneltyPoint==0 )
		{
			MSG_DWORD msg;
			msg.Category = MP_ITEM;
			msg.Protocol = MP_ITEM_SHOPITEM_MSG;
			msg.dwData = eShopItem_PeneltyCountZero;
			SendMsg(&msg, sizeof(msg));
		}*/
	}
	
	DWORD CurLife = GetMaxLife();
	DWORD CurNaeryuk = GetMaxNaeRyuk();
	DWORD CurShield = GetMaxShield();
	SetLife((DWORD)(CurLife*0.3));
	SetNaeRyuk(0);
	SetShield((DWORD)(CurShield*0.3));
	ClearMurderIdx();

	m_bDieForBattleChannel = FALSE;	//SW080515 Ã¤³Îº° ¼¼ÆÃ
	m_bDieForGFW = FALSE;
}

void CPlayer::DoDie(CObject* pAttacker)
{
	OBJECTSTATEMGR_OBJ->StartObjectState(this,eObjectState_Die,pAttacker->GetID());

	////////////////////////////////////////////////////////////////////////////////
	// 06. 06. 2Â÷ ÀüÁ÷ - ÀÌ¿µÁØ
	// Àº½Å / Çý¾È
	// »ç¸Á½Ã ÇØÁ¦

	for(WORD State = eSingleSpecialState_Hide; State < eSingleSpecialState_Max; State++)
	{
		if(!GetSingleSpecialState(State))
			continue;
	
		SetSingleSpecialStateUsedTime(State, gCurTime);
		SetSingleSpecialState(State, false);
		SendSingleSpeicalStateAck(State, false);
	}
	////////////////////////////////////////////////////////////////////////////////
	
	CPet* pPet = GetCurPet();
	if(pPet)
	{
		pPet->GetRandMotionNSpeech(ePM_MASTER_DIE, ePMF_ALWAYS);
	}
	
	//SW070531 Æê Ä£¹Ðµµ º¸È£±â´É
	m_PetManager.SetReduceAmountPetFriendship(pAttacker);

	if(pAttacker->GetObjectKind() == eObjectKind_Player)
	{
		CPlayer* pAttackPlayer = (CPlayer*)pAttacker;
		CBattle* pBattle = pAttacker->GetBattle();

		// magi82 - SOS(070726)	pk ´çÇÑ À¯Àú°¡ ¹®ÆÄ°¡ ÀÖÀ»¶§¸¸ Àû¿ë
		WORD MapNum = g_pServerSystem->GetMapNum();
		DWORD dwCheckBit = eBossMap | eEventMap | eTournament | eSurvivalMap;
		if( GetGuildIdx() > 0 )
		{
			//DWORD dwCheckBit = eBossMap | eEventMap | eTournament | eSurvivalMap;
			// magi82 - SOS(070720) pk ´çÇßÀ»¶§ SOS È£Ãâ
			if( IsPKMode() == TRUE
				|| (GetBattle()->GetBattleKind() == eBATTLE_KIND_VIMUSTREET && pAttacker->GetBattleID() == GetBattleID())
				|| MapNum == SIEGEWARMGR->GetSiegeMapNum()
				|| g_pServerSystem->GetMap()->IsMapKind(dwCheckBit)
				//|| MapNum == BOSSMONSTER_MAP
				//|| MapNum == BOSSMONSTER_2NDMAP || MapNum == RUNNINGMAP
				//|| MapNum == PKEVENTMAP || g_pServerSystem->CheckMapKindIs(eSurvival)
				//|| EVENTMAPMGR->IsEventMap() || MapNum == Tournament
				|| PARTYWARMGR->IsEnemy(this, pAttackPlayer) || GUILDFIELDWARMGR->IsEnemy(this, pAttackPlayer) )
			{
				SetSOSState(0);
			}
			else
				SetSOSState(1);
			//////////////////////////////////////////////////////////////////////////
		}

		//SW080515 Ã¤³Îº° ¼¼ÆÃ
		if( CHANNELSYSTEM->IsBattleChannel(GetChannelID()) )
		{
			if( IsPKMode() == TRUE
				|| (GetBattle()->GetBattleKind() == eBATTLE_KIND_VIMUSTREET && pAttacker->GetBattleID() == GetBattleID())
				|| MapNum == SIEGEWARMGR->GetSiegeMapNum()
				|| g_pServerSystem->GetMap()->IsMapKind(dwCheckBit)
				|| PARTYWARMGR->IsEnemy(this, pAttackPlayer) || GUILDFIELDWARMGR->IsEnemy(this, pAttackPlayer) )
			{
				SetDieForBattleChannel(FALSE);
			}
			else
				SetDieForBattleChannel(TRUE);

			pAttackPlayer->AddKillTimes();

			if (pAttackPlayer->GetFame() <= gFameSet[0] - 1)
			{
				if (GetFame() >= gFameSet[eFamePlayerDied])
				{
					pAttackPlayer->SetHeroFame(gFameSet[eFamePlayerKill], pAttackPlayer->GetObjectName(), GetObjectName(), eYouKillOther);
					SetHeroFame(gFameSet[eFamePlayerDied], pAttackPlayer->GetObjectName(), GetObjectName(), eYouKilledBy);
				}
				else
				{
					pAttackPlayer->SetHeroFame(0, pAttackPlayer->GetObjectName(), GetObjectName(), eOpponentHasNoFame);
				}
			}
		}

		if(pBattle->GetBattleKind() == eBATTLE_KIND_NONE || pAttacker->GetBattleID() != GetBattleID() )
		{
			/*//SW070531 Æê Ä£¹Ðµµ º¸È£±â´É
			//SW070109 È«Äá/´ë¸¸ Æê Æä³ÎÆ¼
#ifdef _HK_LOCAL_
			if(pPet)
			{
				WORD wMapNum = g_pServerSystem->GetMapNum();
				if(wMapNum != Tournament && wMapNum != QUESTMAPNUM1 && wMapNum != QUESTMAPNUM2)		//È­°æ/±Ø¸¶ ÀüÁ÷ ¸Ê 73
				{
					int Petfriendly = GAMERESRCMNGR->GetPetRule()->FriendShipPerMasterDie;

					if( pAttackPlayer->IsPKMode() == TRUE )
					{
						if( FALSE == IsPKMode())
						{
							Petfriendly /= 2;
						}
					}
#ifdef _HK_LOCAL_
					else if( wMapNum == nakyang_siege )
#else
					else if( g_pServerSystem->CheckMapKindIs(eSGMap) )
#endif
					{
						Petfriendly /= 2;
					}

					if( (TRUE == PARTYWARMGR->IsEnemy(this, pAttackPlayer) && FALSE == pAttackPlayer->IsPKMode())		//¹æÆÄÀü »ó´ëÀÌ¸é¼­ »ì±â »óÅÂ°¡ ¾Æ´Ï¸é
						|| TRUE == GUILDFIELDWARMGR->IsEnemy(this, pAttackPlayer) && FALSE == pAttackPlayer->IsPKMode() )	//¹®ÆÄÀü »ó´ëÀÌ¸é¼­ »ì±â »óÅÂ°¡ ¾Æ´Ï¸é
					{
						//Do nothing
					}
					else
					{
						pPet->AddFriendship(Petfriendly, TRUE);

						const PET_TOTALINFO* pInfo = pPet->GetPetTotalInfo();
						LogPet(GetID(),GetUserID(),ePetLog_MasterDie, pInfo->PetDBIdx, pInfo->PetSummonItemDBIdx,

							pInfo->PetGrade,pInfo->PetStamina,pInfo->PetFriendly,pInfo->bAlive);
					}
				}
			}
#endif*/
			
			// for pk
			if( LOOTINGMGR->IsLootingSituation( this, pAttacker ) )
			{
				if( PARTYWARMGR->IsinSamePartyWar( this, (CPlayer*)pAttacker ) == FALSE )
				{
					//SW060831 ¹æÆÄ ºñ¹« ½Ã //°­Á¦ Á¾·á½Ã Ã¼Å© º¯¼ö
					if(PARTYWARMGR->IsMemberInPartyWar( this ))
					{
						SetPenaltyByDie(TRUE);
					}

					LOOTINGMGR->CreateLootingRoom( this, (CPlayer*)pAttacker );
				}
			}
			if( IsPKMode() == FALSE && pAttackPlayer->IsPKMode() == TRUE && 
				WANTEDMGR->IsOwner((CPlayer*)pAttacker, GetWantedIdx()) == FALSE &&
				PARTYWARMGR->IsinSamePartyWar( this, (CPlayer*)pAttacker ) == FALSE )	// attacker is pk
			{
				SetMurderIDX(pAttacker->GetID());
				SetMurderKind(pAttacker->GetObjectKind());
				WANTEDMGR->SendRegistWanted(this, (CPlayer*)pAttacker);
				//journal insert
				JouranlInsertWanted(GetID(), eJournal_Wanted, eJournal_MurderedbyChr, ((CPlayer*)pAttacker)->GetObjectName());

				SetPenaltyByDie( TRUE );
				// partywar
				if( PARTYWARMGR->PlayerDie( this, (CPlayer*)pAttacker ) )
				{
					SetReadyToRevive( FALSE );
					SetPenaltyByDie( FALSE );
				}

				WANTEDMGR->Complete((CPlayer*)pAttacker, this);
			}
			else
			{				
				SetReadyToRevive(TRUE);
				SetPenaltyByDie( TRUE );

				// partywar
				if( PARTYWARMGR->PlayerDie( this, (CPlayer*)pAttacker ) )
				{
					SetReadyToRevive( FALSE );
					SetPenaltyByDie( FALSE );
				}
				//SW061019 ¼­¹ÙÀÌ¹ú¸ðµå 
				//else if( g_pServerSystem->CheckMapKindIs(eSurvival) )
				else if( g_pServerSystem->GetMap()->IsMapKind(eSurvivalMap) )
				{
					SVVMODEMGR->RemoveAliveUser(this);
					SetReadyToRevive( FALSE );
					SetPenaltyByDie( FALSE );
					m_bNeedRevive = FALSE;
				}
				else
				{
					// guildfieldwar
					GUILDFIELDWARMGR->JudgeGuildFieldWar( this, (CPlayer*)pAttacker );

					WANTEDMGR->Complete((CPlayer*)pAttacker, this);
				}
			}

			    // the kill player send msg to client 2014-05-18

				SYSTEMTIME now;
				GetSystemTime(&now);
				DWORD nowSecond = now.wHour*3600 + now.wMinute*60 + now.wSecond;

				//weiye Íæ¼ÒÔÚ10·ÖÖÓÄÚÉ±º¦ÈËÊý´ïµ½20±¶ÊýÊ±¿ªÊ¼È«ÆÁµçÊÓ   2017-12-21
				//pAttacker->m_KillTime=pAttacker->m_KillTime+1;    //weiye Ã¿É±Ò»ÈË£¬¼ÆÊýÆ÷+1

				if((nowSecond - pAttacker->m_KillTimeout) > 600)
				{
					pAttacker->m_KillTime = 0;
					pAttacker->m_KillTimeout = nowSecond;
				}
				else
					pAttacker->m_KillTime+=1;

				if(pAttacker->m_KillTime>=0)
				{
					if(pAttacker->m_KillTime%1==0)
					{

						//farid requested
						if (gFameSet[eFameOveridedByPVPSystem]==TRUE)
						if (pAttackPlayer->GetFame() <= gFameSet[0] - 1)
						{
							if (GetFame() >= gFameSet[4])
							{
								pAttackPlayer->SetHeroFame(gFameSet[3], pAttackPlayer->GetObjectName(), GetObjectName(), eYouKillOther);
								SetHeroFame(gFameSet[4], pAttackPlayer->GetObjectName(), GetObjectName(), eYouKilledBy);
							}
							else
							{
								pAttackPlayer->SetHeroFame(0, pAttackPlayer->GetObjectName(), GetObjectName(), eOpponentHasNoFame);
							}
						}


						MSG_CHAT_WORD2 msg;
						msg.Category = MP_CLIENT;
						msg.Protocol = MP_CLIENT_SECOND_AGENT;
						if((IsPKMode() == FALSE && pAttackPlayer->IsPKMode() == TRUE) || (IsPKMode() == TRUE && pAttackPlayer->IsPKMode() == FALSE) )
						{
							msg.Flag = eKillByWar;
						}
						else
						{
							msg.Flag = eKillByRed;				
						}
						
						SafeStrCpy(msg.Name1,pAttacker->GetObjectName(),MAX_NAME_LENGTH+1);
						SafeStrCpy(msg.Name2,GetObjectName(),MAX_NAME_LENGTH+1);
						msg.MapNum = g_pServerSystem->GetMapNum();
						msg.ChannelNum = pAttacker->GetBattleID();
						msg.Stage=GetStage();
						msg.AttackerStage=pAttacker->GetStage();
						msg.GuildIdx1 = GetGuildMarkName();
						msg.GuildIdx2 = pAttacker->GetGuildMarkName();
						msg.KillTime = pAttacker->m_KillTime+1;
						pAttackPlayer->SendMsg(&msg, sizeof(msg));

						//g_Console.LOG(4,"DGInfo: [%d:GetGuildMarkName()], [%d:pAttacker->GetGuildMarkName()]",GetGuildMarkName(),pAttacker->GetGuildMarkName());
					}
				}
				/*if((nowSecond - pAttacker->m_KillTimeout) > 600)
				{
					pAttacker->m_KillTime =0; //weiye Ã¿10·ÖÖÓÖØÖÃ¼ÆÊ±Æ÷
					pAttacker->m_KillTimeout = nowSecond;
				}
				else
				{
					    if( pAttacker->m_KillTime >=20 && pAttacker->m_KillTime%20==0)
						{
							MSG_CHAT_WORD2 msg;
							msg.Category = MP_CLIENT;
							msg.Protocol = MP_CLIENT_MSG_AGENT;
							msg.Flag = 1;
							msg.KillTime = pAttacker->m_KillTime;
							SafeStrCpy(msg.Name1,pAttacker->GetObjectName(),MAX_NAME_LENGTH+1);
							SafeStrCpy(msg.Name2,GetObjectName(),MAX_NAME_LENGTH+1);
							SafeStrCpy(msg.MapName,g_pServerSystem->GetMap()->GetMapName(),128);
							msg.ChannelNum = pAttacker->GetBattleID();
							pAttackPlayer->SendMsg(&msg, sizeof(msg));
					}
				}*/
		}
	}
	else if(pAttacker->GetObjectKind() & eObjectKind_Monster )
	{	
		/*
		if( GetBattle()->GetBattleKind() != eBATTLE_KIND_SURYUN )
		{
			//CPet* pPet = GetCurPet();
			if(pPet)
			{
				WORD wMapNum = g_pServerSystem->GetMapNum();
				if(wMapNum != Tournament && wMapNum != QUESTMAPNUM1 && wMapNum != QUESTMAPNUM2)		//È­°æ/±Ø¸¶ ÀüÁ÷ ¸Ê 73
				{
					int Petfriendly = GAMERESRCMNGR->GetPetRule()->FriendShipPerMasterDie;
					pPet->AddFriendship(Petfriendly, TRUE);

					const PET_TOTALINFO* pInfo = pPet->GetPetTotalInfo();
					LogPet(GetID(),GetUserID(),ePetLog_MasterDie, pInfo->PetDBIdx, pInfo->PetSummonItemDBIdx,
						pInfo->PetGrade,pInfo->PetStamina,pInfo->PetFriendly,pInfo->bAlive);
				}
			}
		}*/

		SetMurderKind(((CMonster*)pAttacker)->GetMonsterKind());
			
		//SW060831 ¹æÆÄ ºñ¹« ½Ã //°­Á¦ Á¾·á½Ã Ã¼Å© º¯¼ö
#ifdef _KOR_LOCAL_

		// RaMa 5ÃÊµ¿¾È ·ºÀÌ¶ó°í »ý°¢µÉ °æ¿ì
		if( (gCurTime - m_LastActionTime) > GAMERESRCMNGR->GetPaneltyTime() )
		{
			m_bActionPanelty = FALSE;
			m_LastActionTime = gCurTime;
			SendNoActionPaneltyMsg();
		}
		else
#endif
			SetPenaltyByDie(TRUE);

		SetReadyToRevive(TRUE);

		// add quest event
		QUESTMGR->AddQuestEvent( this, &CQuestEvent( eQuestEvent_Die, g_pServerSystem->GetMapNum(), 0 ) );
	}

	//KES EXIT
	ExitCancel();
	//KES EXCHANGE 031002
	EXCHANGEMGR->CancelExchange( this );
	STREETSTALLMGR->UserLogOut( this );
	//KES PKLOOTING
	LOOTINGMGR->LootingCancel( this );
	//KES PKPlayerPanelty
	PKMGR->DiePanelty( this, pAttacker );

	CMonster * pObject = NULL;
	m_FollowMonsterList.SetPositionHead();
	while(pObject = (CMonster *)m_FollowMonsterList.GetData())
	{
		pObject->SetTObject(NULL);
	}
	m_FollowMonsterList.RemoveAll();

	//SW051112 ¹«½Ö¸ðµå
	InitMussangPoint();
	SendMussangInfo();

	//¹«½Ö¸ðµå »óÅÂÀÏ¶§¸¸ Á¾·á½ÃÄÑÁØ´Ù
	if(IsMussangMode())
	{
		SetMussangMode(FALSE);
		STATSMGR->CalcItemStats(this);	//¹«½Ö ½ºÅÈ ÃÊ±âÈ­.
		SendMussangEnd();
	}
	
	if( CCharMove::IsMoving(this) )
	{
		VECTOR3 pos;
		GetPosition( &pos );
		CCharMove::EndMove( this, gCurTime, &pos );
	}

	QUESTMAPMGR->DiePlayer( this );

	SKILLOPTION* pSkillOption = NULL;
	CSkillObject* pSkill = NULL;
	m_SkillList.SetPositionHead();
	while( pSkill = m_SkillList.GetData() )	
	{
		SKILLOBJECT_INFO* pSkillObjInfo = pSkill->GetSkillObjectInfo();
		if(pSkillObjInfo != NULL)
		{
			pSkillOption = SKILLMGR->GetSkillOption(pSkillObjInfo->Option);
			if(pSkillOption != NULL)
			{
				RemoveSkillStatsOption(pSkillOption);

				CCharacterCalcManager::CalcMaxLife(this);
				CCharacterCalcManager::CalcMaxShield(this);
				CCharacterCalcManager::CalcMaxNaeRyuk(this);
			}
		}
		pSkillObjInfo = NULL;
		pSkillOption = NULL;

		pSkill->DoDie(NULL);
	}
	m_SkillList.RemoveAll();

	if(this->GetTitanManager()->IsTitanRecall())
	{
		this->GetTitanManager()->InitTitanRecall();

		MSGBASE msg;
		msg.Category = MP_TITAN;
		msg.Protocol = MP_TITAN_RECALL_CANCEL_ACK;
		msg.dwObjectID = this->GetID();

		this->SendMsg(&msg, sizeof(msg));
	}	

	FORTWARMGR->DiePlayer( this );
	if(pAttacker->GetObjectKind() == eObjectKind_Player)
		GUILDWARMGR->KillPlayer((CPlayer*)pAttacker,this);
}

float CPlayer::DoGetMoveSpeed()
{
	/*
	if(m_MoveInfo.KyungGongIdx)
	{
		CKyungGongInfo* pKGInfo = KYUNGGONGMGR->GetKyungGongInfo(m_MoveInfo.KyungGongIdx);
		ASSERT(pKGInfo);
		if(pKGInfo == NULL)
			return 0;
		float speed = pKGInfo->GetSpeed()+GetAbilityStats()->Kyunggong;

		// ¾Æ¹ÙÅ¸¾ÆÀÌÅÛ °æ°ø¼Óµµ »ó½Â
        speed += m_AvatarOption.KyunggongSpeed;

		// ºÎÀû»ç¿ë °æ°ø¼Óµµ »ó½Â
		speed += m_ShopItemOption.KyungGongSpeed;			
		return speed;
	}
	else
	{
		if(m_MoveInfo.MoveMode == eMoveMode_Run)
			return RUNSPEED;
		else
			return WALKSPEED;
	}*/
	//SW070127 Å¸ÀÌÅº
	// Å¸ÀÌÅº Å¾½Â ÁßÀÌ¸é
	float speed = 0;
	if( InTitan() )
	{	
		if(m_MoveInfo.KyungGongIdx)
		{	
			TITANINFO_GRADE* pGradeInfo = GAMERESRCMNGR->GetTitanGradeInfo(m_TitanManager.GetCurRidingTitan()->GetTitanKind(), 
				m_TitanManager.GetCurRidingTitan()->GetTitanGrade());
			if( !pGradeInfo )
				return TITAN_WALKSPEED;

			WORD klev = 0;		// ½ÅÇà¹éº¯
			if( m_MoveInfo.KyungGongIdx == 2602 )
				klev = 1;		// ÃÊ»óºñ
			else if( m_MoveInfo.KyungGongIdx == 2604 )
				klev = 2;		// 
			speed = pGradeInfo->KyungGongSpeed[klev];

			// ¾Æ¹ÙÅ¸ ¾ÆÀÌÅÛ °æ°ø ½ºÇÇµå 
			speed += GetAvatarOption()->KyunggongSpeed;
			// ºÎÀû °æ°ø ¼Óµµ »ó½Â
			speed += GetShopItemStats()->KyungGongSpeed;

			return speed;
		}
		else
		{
			if(m_MoveInfo.MoveMode == eMoveMode_Run)
				return m_TitanManager.GetTitanStats()->MoveSpeed;
			else
				return TITAN_WALKSPEED;
		}
	}
	else
	{

		if(m_MoveInfo.KyungGongIdx)
		{
			CKyungGongInfo* pKGInfo = KYUNGGONGMGR->GetKyungGongInfo(m_MoveInfo.KyungGongIdx);
			ASSERT(pKGInfo);
			if(pKGInfo == NULL)
				return 0;

			speed = pKGInfo->GetSpeed()+GetAbilityStats()->Kyunggong;

			// ¾Æ¹ÙÅ¸¾ÆÀÌÅÛ °æ°ø¼Óµµ »ó½Â
			speed += m_AvatarOption.KyunggongSpeed;
			// ºÎÀû»ç¿ë °æ°ø¼Óµµ »ó½Â
			speed += m_ShopItemOption.KyungGongSpeed;			
			return speed;
		}
		else
		{
			if(m_MoveInfo.MoveMode == eMoveMode_Run)
			{
				return RUNSPEED;
			}				
			else
			{
				return WALKSPEED;
			}
		}
	}

	return speed;
}

MUGONGBASE * CPlayer::GetMugongBaseByMugongIdx(WORD MugongIdx)
{
	MUGONGBASE* pMugong;
	for(int n=0;n<SLOT_MUGONGTOTAL_NUM;++n)
	{
		// magi82 - Titan(070611) Å¸ÀÌÅº ¹«°øº¯È¯ ÁÖ¼®Ã³¸®
		// ½ºÅ³ÀÎµ¦½º°¡ 10000ÀÌ ³ÑÀ¸¸é Å¸ÀÌÅº ½ºÅ³ÀÌ´Ù.
		//if(MugongIdx < SKILLNUM_TO_TITAN)
            pMugong = &m_HeroMugongInfo.mugong[n];
		//else
		//	pMugong = &m_HeroMugongInfo.Titanmugong[n];

		if(pMugong->dwDBIdx == 0)
			continue;
		if(pMugong->wIconIdx == MugongIdx)
			return pMugong;
	}
	return NULL;
}

int CPlayer::GetMugongLevel(WORD MugongIdx)
{
	//SW070127 Å¸ÀÌÅº
		/*if(MugongIdx < 100)	// A¡§¡©¡§¡þ¡Ë¡þAI¡Ë¡þe
		return 1;*/

	// !!!!!!!!!!! magi82 - ¿ø·¡´Â Å¸ÀÌÅº ÄÞº¸¸¦ µû·Î ¸¸µé¾î¾ßÇÏÁö¸¸ Áö±ÝÀº ±ÞÇØ¼­ ÀÏ´Ü ÀÌ·¸°Ô ÀÓ½Ã·Î ¾¸ 
	if(MugongIdx < 100 || ( 10000 <= MugongIdx && MugongIdx < 10000 + 100 ))
		return 1;
	
	MUGONGBASE * pMugong = GetMugongBaseByMugongIdx(MugongIdx);
	if(pMugong == NULL)
	{
		return -1;
	}

	return pMugong->Sung;
}

int CPlayer::GetMugongNum()
{
	int count = 0;
	MUGONGBASE* pMugong;
	for(int n=0;n<SLOT_MUGONGTOTAL_NUM;++n)
	{
		pMugong = &m_HeroMugongInfo.mugong[n];
		if(pMugong->dwDBIdx && pMugong->Sung > 0)
			++count;		
	}
	return count;
}

//////////////////////////////////////////////////////////////////////////
// 06. 06. 2Â÷ ÀüÁ÷ - ÀÌ¿µÁØ
// ¹«°ø º¯È¯ Ãß°¡
WORD CPlayer::GetSkillOptionIndex(WORD MugongIdx)
{
	//SW070127 Å¸ÀÌÅº
	//if( MugongIdx > SKILLNUM_TO_TITAN && InTitan() )
	//	MugongIdx -= SKILLNUM_TO_TITAN;

	MUGONGBASE * pMugong = GetMugongBaseByMugongIdx(MugongIdx);
	if(pMugong == NULL)
	{
		return 0;
	}

	return pMugong->OptionIndex;
}
//////////////////////////////////////////////////////////////////////////

void CPlayer::SetInitedGrid()
{
	MSGBASE msg;
	msg.Category = MP_USERCONN;
	msg.Protocol = MP_USERCONN_GRIDINIT;
	SendMsg(&msg,sizeof(msg));

	CGridUnit::SetInitedGrid();
	
	CBattle* pBattle = BATTLESYSTEM->GetBattle(this->GetBattleID());
	if(pBattle && pBattle->GetBattleKind() != eBATTLE_KIND_NONE)
		BATTLESYSTEM->AddObjectToBattle(pBattle, this);

	QUESTMGR->AddQuestEvent( this, &CQuestEvent( eQuestEvent_GameEnter, 0, 1 ) );
	QUESTMGR->AddQuestEvent( this, &CQuestEvent( eQuestEvent_MapChange, 0, g_pServerSystem->GetMapNum() ) );

	//Pet SummonningPet
	//m_PetManager.CheckSummonningPet();
}
// RaMa - 04.11.10    -> ShopItemOption Ãß°¡   AvatarOptionÃß°¡(05.02.16)
DWORD CPlayer::DoGetCritical()
{
	//DWORD cri = GetCharStats()->Critical + GetItemStats()->Critical + GetShopItemStats()->Critical + GetAvatarOption()->Critical;

	//SW070127 Å¸ÀÌÅº
	DWORD cri = 0;
	if( InTitan() )
	{
		titan_calc_stats* pTitanStat = m_TitanManager.GetTitanStats();
		cri = pTitanStat->Critical;
	}
	else
	{
		cri = GetCharStats()->Critical + GetItemStats()->Critical + GetShopItemStats()->Critical + GetAvatarOption()->Critical;
	}

	if( GetPartyIdx() )
	{
		CParty* pParty = PARTYMGR->GetParty( GetPartyIdx() );
		if( pParty )
		{
			int count = pParty->GetMemberCountofMap( GetID() );
			if( count && gPartyEvent[ePartyEvent_CriticalRate].Rate[count-1] )
				cri = (DWORD)(cri*gPartyEvent[ePartyEvent_CriticalRate].Rate[count-1]);
		}
	}

	return cri;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
// 06. 07 ³»°ø ÀûÁß(ÀÏ°Ý) - ÀÌ¿µÁØ
DWORD CPlayer::DoGetDecisive()
{	
	DWORD cri = GetCharStats()->Decisive + GetItemStats()->Critical + GetShopItemStats()->Decisive + GetAvatarOption()->Decisive;

	if( GetPartyIdx() )
	{
		CParty* pParty = PARTYMGR->GetParty( GetPartyIdx() );
		if( pParty )
		{
			int count = pParty->GetMemberCountofMap( GetID() );
			if( count && gPartyEvent[ePartyEvent_CriticalRate].Rate[count-1] )
				cri = (DWORD)(cri*gPartyEvent[ePartyEvent_CriticalRate].Rate[count-1]);
		}
	}

	return cri;
}
///////////////////////////////////////////////////////////////////////////////////////////////////
DWORD CPlayer::DoGetPhyAttackPowerMin()
{
	const player_calc_stats * pChar_stats = GetCharStats();
	const player_calc_stats * pItem_stats = GetItemStats();	
	const SET_ITEM_OPTION* pSetItem_Stats = GetSetItemStats();
	AVATARITEMOPTION* pAvatarOption = GetAvatarOption();

	WORD WeaponKind = GetWeaponEquipType();
	
	////	2005 Å©¸®½º¸¶½º ÀÌº¥Æ® ÄÚµå
	//if( WeaponKind == WP_EVENT || WeaponKind == WP_EVENT_HAMMER )
	//	return 0;
	//SW061211 Å©¸®½º¸¶½ºÀÌº¥Æ®
	if( WeaponKind == WP_EVENT_HAMMER )
		return 0;

	// RaMa - 04.11.10    -> ShopItemOption Ãß°¡		AvatarOptionÃß°¡(05.02.16)
	if(WeaponKind == WP_GUNG || WeaponKind == WP_AMGI)
	{
		WORD MinChub = (WORD)(pChar_stats->MinChub + GetAbilityStats()->StatMin + pItem_stats->MinChub + GetShopItemStats()->Minchub + pAvatarOption->Minchub + pSetItem_Stats->wMinChub);

#ifdef _JAPAN_LOCAL_

		WORD RareWeapon = pItem_stats->RangeAttackPowerMin;
/*
		if( IsPartyHelp() )
		{
			RareWeapon = RareWeapon + RareWeapon * 20 / 100;
		}
*/
		WORD WeaponAttack = (WORD)(RareWeapon
						  * ( 1.f + pItem_stats->RangeAttackPowerAddRateMin )
						  + pAvatarOption->Attack);

		return CalcRangeAttackPower( MinChub, WeaponAttack )
			 + GetLevel() * 4
			 + GetAbilityStats()->GetPhyAttackUp( WeaponKind );
#else
		return (DWORD)((CalcRangeAttackPower(MinChub,pItem_stats->RangeAttackPowerMin+pAvatarOption->Attack+pSetItem_Stats->wRangeAttackMin)
			+ GetAbilityStats()->GetPhyAttackUp(WeaponKind))
			* (1+GetItemStats()->RangeAttackPowerAddRateMin));
#endif
	}
	else
	{
		WORD GenGol = (WORD)(pChar_stats->GenGol + GetAbilityStats()->StatGen + pItem_stats->GenGol + GetShopItemStats()->Gengol + pAvatarOption->Gengol + pSetItem_Stats->wGenGol);

		WORD WeaponItemIdx = GetWearedItemIdx(eWearedItem_Weapon);
		WORD DefaultPower = 0;
		if( WeaponItemIdx == 0 )
			DefaultPower = 5;	

#ifdef _JAPAN_LOCAL_

		WORD RareWeapon = pItem_stats->MeleeAttackPowerMin + DefaultPower;
/*
		if( IsPartyHelp() )
		{
			RareWeapon = RareWeapon + RareWeapon * 20 / 100;
		}
*/
		WORD WeaponAttack = (WORD)(RareWeapon
						  * ( 1.f + pItem_stats->MeleeAttackPowerAddRateMin )
						  + pAvatarOption->Attack);

		return CalcMeleeAttackPower( GenGol, WeaponAttack )
			 + GetLevel() * 4
			 + GetAbilityStats()->GetPhyAttackUp( WeaponKind );
#else
		return (DWORD)((CalcMeleeAttackPower(GenGol,pItem_stats->MeleeAttackPowerMin+DefaultPower+pAvatarOption->Attack+pSetItem_Stats->wMeleeAttackMin)
			+ GetAbilityStats()->GetPhyAttackUp(WeaponKind))
			* (1+GetItemStats()->MeleeAttackPowerAddRateMin));
#endif
	}
}

DWORD CPlayer::DoGetPhyAttackPowerMax()
{
	const player_calc_stats * pChar_stats = GetCharStats();
	const player_calc_stats * pItem_stats = GetItemStats();
	const SET_ITEM_OPTION* pSetItem_Stats = GetSetItemStats();
	AVATARITEMOPTION* pAvatarOption = GetAvatarOption();

	// RaMa - 04.11.10    -> ShopItemOption Ãß°¡		AvatarOptionÃß°¡(05.02.16)
	WORD WeaponKind = GetWeaponEquipType();

	////	2005 Å©¸®½º¸¶½º ÀÌº¥Æ® ÄÚµå
	//if( WeaponKind == WP_EVENT || WeaponKind == WP_EVENT_HAMMER )
	//	return 0;
	//SW061211 Å©¸®½º¸¶½ºÀÌº¥Æ®
	if( WeaponKind == WP_EVENT_HAMMER )
		return 0;

	if(WeaponKind == WP_GUNG || WeaponKind == WP_AMGI)
	{
		WORD MinChub = (WORD)(pChar_stats->MinChub + GetAbilityStats()->StatMin + pItem_stats->MinChub + GetShopItemStats()->Minchub + GetAvatarOption()->Minchub + pSetItem_Stats->wMinChub);

#ifdef _JAPAN_LOCAL_
		
//--- ÀÏº» ¹æÆÄ½Ã »ó»ý°ü°è¿¡ µû¸¥ °ø°Ý·Â Áõ°¡

		WORD RareWeapon = pItem_stats->RangeAttackPowerMax;
/*
		if( IsPartyHelp() && GetBattle()->GetBattleKind() != eBATTLE_KIND_VIMUSTREET )
		{
			RareWeapon = RareWeapon + RareWeapon * 20 / 100;
		}
*/
		WORD WeaponAttack = (WORD)(RareWeapon
						  * ( 1.f + pItem_stats->RangeAttackPowerAddRateMax )
						  + pAvatarOption->Attack);

		return CalcRangeAttackPower( MinChub, WeaponAttack )
			 + GetLevel() * 4
			 + GetAbilityStats()->GetPhyAttackUp( WeaponKind );
#else
		return (DWORD)((CalcRangeAttackPower(MinChub,pItem_stats->RangeAttackPowerMax+pAvatarOption->Attack+pSetItem_Stats->wRangeAttackMax)
			+ GetAbilityStats()->GetPhyAttackUp(WeaponKind))
			* (1+GetItemStats()->RangeAttackPowerAddRateMax));
#endif
	}
	else
	{
		WORD GenGol = (WORD)(pChar_stats->GenGol + GetAbilityStats()->StatGen + pItem_stats->GenGol + GetShopItemStats()->Gengol + GetAvatarOption()->Gengol + pSetItem_Stats->wGenGol);

		WORD WeaponItemIdx = GetWearedItemIdx(eWearedItem_Weapon);
		WORD DefaultPower = 0;
		if( WeaponItemIdx == 0 )
			DefaultPower = 5;		
		
#ifdef _JAPAN_LOCAL_

		WORD RareWeapon = pItem_stats->MeleeAttackPowerMax + DefaultPower;
/*
		if( IsPartyHelp() )
		{
			RareWeapon = RareWeapon + RareWeapon * 20 / 100;
		}
*/
		WORD WeaponAttack = (WORD)(RareWeapon
						  * ( 1.f + pItem_stats->MeleeAttackPowerAddRateMax )
						  + pAvatarOption->Attack);

		return CalcMeleeAttackPower( GenGol, WeaponAttack )

			 + GetLevel() * 4
			 + GetAbilityStats()->GetPhyAttackUp( WeaponKind );
#else
		return (DWORD)((CalcMeleeAttackPower(GenGol,pItem_stats->MeleeAttackPowerMax+DefaultPower+pAvatarOption->Attack+pSetItem_Stats->wMeleeAttackMax)
			+ GetAbilityStats()->GetPhyAttackUp(WeaponKind))
			* (1+GetItemStats()->MeleeAttackPowerAddRateMax));
#endif
	}
}

void CPlayer::DoDamage(CObject* pAttacter,RESULTINFO* pDamageInfo,DWORD beforeLife)
{
	////////////////////////////////////////////////////////////////////////////////
	// 06. 06. 2Â÷ ÀüÁ÷ - ÀÌ¿µÁØ
	// Àº½Å / Çý¾È
	// Àº½Å ÆÐ³ÎÆ¼
	// °ø°Ý´çÇÒ½Ã Àº½ÅÇØÁ¦
	if(IsHide())
	{
		SetSingleSpecialStateUsedTime(eSingleSpecialState_Hide, gCurTime);
		SetSingleSpecialState(eSingleSpecialState_Hide, false);
		SendSingleSpeicalStateAck(eSingleSpecialState_Hide, false, true);
	}
	////////////////////////////////////////////////////////////////////////////////
}

void CPlayer::InitBaseObjectInfo(BASEOBJECT_INFO* pBaseInfo)
{
	ASSERT(GetInitState() == PLAYERINITSTATE_ONLY_ADDED);
	memcpy(&m_BaseObjectInfo,pBaseInfo,sizeof(BASEOBJECT_INFO));
}

/* ¡§oC¡§¡þ?E¢®¨Ï¢®¢´¡Ëc Return */
MONEYTYPE CPlayer::SetMoney( MONEYTYPE ChangeValue, BYTE bOper, BYTE MsgFlag, eITEMTABLE tableIdx, BYTE LogType, DWORD TargetIdx )
{

	CPurse* pPurse = m_ItemContainer.GetPurse(tableIdx);
	if( !pPurse) return 0;

	MONEYTYPE Real = 0;
	if( bOper == MONEY_SUBTRACTION )
	{
		Real = pPurse->Subtraction( ChangeValue, MsgFlag );
	}
	else
	{
		Real = pPurse->Addition( ChangeValue, MsgFlag );
	}

	// Log Money AId¢®¢´¡Ëc AI¢®io ¡§¡þ?CO¡ËO¢®¡¿ 
	if( tableIdx == eItemTable_Inventory || tableIdx == eItemTable_ShopInven )
	{
		if( Real >= 10000 )
			InsertLogWorngMoney( LogType, GetID(), Real, GetMoney(), GetMoney(eItemTable_Pyoguk), TargetIdx );
	}

	return Real;
}

/* ¡Íi¢®¢´; ¨Ï©ªO; A¨Ï¢®¡§¡þ¡§¡ËCN ¢®¨¡¨Ï¨£¢®¨¡¢®IAI AO¡Ë¡ÍAAo¡Ë¡þ| ¨Ïo?¡Ë¡ÍA¡Ë¡ÍU.*/
BOOL CPlayer::IsEnoughAdditionMoney(MONEYTYPE money, eITEMTABLE tableIdx )
{
	CPurse* pPurse = m_ItemContainer.GetPurse(tableIdx);
	if(!pPurse) return FALSE;

	return pPurse->IsAdditionEnough( money );
}


MONEYTYPE CPlayer::GetMaxPurseMoney(eITEMTABLE TableIdx)
{
	CPurse* pPurse = m_ItemContainer.GetPurse(TableIdx);
	if( !pPurse) return FALSE;
	return pPurse->GetMaxMoney();
}

void CPlayer::SetMaxPurseMoney(eITEMTABLE TableIdx, MONEYTYPE MaxMoney)
{
	//C¢®I¢®¨ú¨Ïo; ¢®ic¡Ë¡þe ¡Íi¢®¢´ ¡§¡þ¡Ë¡þ¢®¨¡u ¡Ë¡þ¡§¢®¡§o¡§¡þA¡Ë¢ç¢®¨¡¡Ë¢ç ¡Ë¡ÍA¡§ui¨Ï©ª¢®¨Ï¡Ë¡ÍU.
	if(TableIdx != eItemTable_Pyoguk)
	{
		ASSERT(0);
		return;
	}
	CPurse* pPurse = m_ItemContainer.GetPurse(TableIdx);
	if( !pPurse) return;
	pPurse->SetMaxMoney(MaxMoney);
}

void CPlayer::AddQuest(CQuestBase* pQuest)
{
	if(m_QuestList.GetData(pQuest->GetQuestIdx()))
	{
		ASSERT(0);
		return;
	}
	m_QuestList.Add( pQuest, pQuest->GetQuestIdx() );
}

void CPlayer::RemoveQuest(DWORD QuestIdx)
{
	m_QuestList.Remove(QuestIdx);
}

BOOL CPlayer::SetQuestState(DWORD QuestIdx, QSTATETYPE value)
{
	CQuestBase* pQuest;
	pQuest = m_QuestList.GetData(QuestIdx);

	if( !pQuest ) 
	{
//		char buff[256] = {0,};
//		sprintf(buff, "¡Ë?a¢®¨úa¡Ë¡ÍA Au¡§o¡§¡þ¡§¢®¡Ëc¢®¨¡¡Ë¢ç xAc ¡§uECN¡Ë¡ÍU¨Ï©ª¨Ï¡þ CI¡Ë?¡§I¢®¨¡¢®I ¨Ïoy¡§u¡Ëc¡Ë?¡Ë¢ç¢®¨¡O ¡§uE¢®¢´AAa¡Ë?¢®¨¡ [QUEST ID : %d]", QuestIdx);
//		ASSERTMSG(0, buff);
		return FALSE;
	}

	pQuest->SetValue(value);
	BOOL bEnd = pQuest->IsComplete();

	// DB¡Ë?¡Ë¢ç ¡§uA¡§¢®ACN¡Ë¡ÍU.
	QuestUpdateToDB( GetID(), QuestIdx, value, bEnd );

	if( bEnd )
	{
		m_QuestList.Remove(QuestIdx);

		MSG_DWORD msg;
		msg.Category = MP_QUEST;
		msg.Protocol = MP_QUEST_REMOVE_NOTIFY;
		msg.dwObjectID = GetID();
		msg.dwData = QuestIdx;
		SendMsg(&msg, sizeof(msg));
	}

	return TRUE;
}

void CPlayer::SetInitState(int initstate,DWORD protocol)
{
	
	if(initstate == PLAYERINITSTATE_ONLY_ADDED)
	{
		m_initState = PLAYERINITSTATE_ONLY_ADDED;
		InitClearData();
	}
	else
		m_initState |= initstate;

	
	if ((m_initState & PLAYERINITSTATE_MUGONG_INFO) &&
		(m_initState & PLAYERINITSTATE_ITEM_INFO) &&
		(m_initState & PLAYERINITSTATE_ITEM_RARE_OPTION_INFO) &&
		(m_initState & PLAYERINITSTATE_ITEM_OPTION_INFO) &&
		(m_initState & PLAYERINITSTATE_ITEM_STONE_INFO)  &&           // ÐÂ´ò¿ØÏµÍ³±êÖ¾ 2014-11-14
		(m_initState & PLAYERINITSTATE_ABILITY_INFO) &&
//		(m_initState & PLAYERINITSTATE_GMCONTROL_INFO) &&
		(m_initState & PLAYERINITSTATE_HERO_INFO) &&
		(m_initState & PLAYERINITSTATE_PET_INFO) &&
		(m_initState & PLAYERINITSTATE_TITAN_INFO) &&
		!(m_initState & PLAYERINITSTATE_PYOGUK_INFO) )
	{
		CharacterPyogukInfo(GetUserID(), GetID());
		return;
	}

	if ((m_initState & PLAYERINITSTATE_MUGONG_INFO) &&
		(m_initState & PLAYERINITSTATE_ITEM_INFO) &&
		(m_initState & PLAYERINITSTATE_ITEM_RARE_OPTION_INFO) &&
		(m_initState & PLAYERINITSTATE_ITEM_OPTION_INFO) &&
		(m_initState & PLAYERINITSTATE_ITEM_STONE_INFO)  &&           // ÐÂ´ò¿ØÏµÍ³±êÖ¾ 2014-11-14
		(m_initState & PLAYERINITSTATE_ABILITY_INFO) &&
//		(m_initState & PLAYERINITSTATE_GMCONTROL_INFO) &&
		(m_initState & PLAYERINITSTATE_HERO_INFO) && 
		(m_initState & PLAYERINITSTATE_PET_INFO) &&
		(m_initState & PLAYERINITSTATE_TITAN_INFO) &&
		(m_initState & PLAYERINITSTATE_PYOGUK_INFO) &&
		!(m_initState & PLAYERINITSTATE_SHOPITEM_USEDINFO) )
	{
		// Mall ItemÀ» ¹Þ¾Æ¿Àµµ·Ï ÇÑ´Ù.
		UsingShopItemInfo( GetID() );
		SavedMovePointInfo( GetID() );
		return;
	}

	if ((m_initState & PLAYERINITSTATE_MUGONG_INFO) &&
		(m_initState & PLAYERINITSTATE_ITEM_INFO) &&
		(m_initState & PLAYERINITSTATE_ITEM_RARE_OPTION_INFO) &&
		(m_initState & PLAYERINITSTATE_ITEM_OPTION_INFO) &&
		(m_initState & PLAYERINITSTATE_ITEM_STONE_INFO)  &&           // ÐÂ´ò¿ØÏµÍ³±êÖ¾ 2014-11-14
		(m_initState & PLAYERINITSTATE_ABILITY_INFO) &&
		(m_initState & PLAYERINITSTATE_HERO_INFO) &&
		(m_initState & PLAYERINITSTATE_PET_INFO) &&
		(m_initState & PLAYERINITSTATE_TITAN_INFO) &&
		(m_initState & PLAYERINITSTATE_PYOGUK_INFO) &&
		(m_initState & PLAYERINITSTATE_SHOPITEM_USEDINFO) )
	{
		//AuAo¡§¢®oE¢®¨Ï¡Ë¡þ¨Ï¡Ì¡Íia

		//SW050907 ÀÌ»óÇÑ ÄÚµå ¸·´Â´Ù.
		//PWMODMGR_OBJ->Init_PeaceWarMode(this);
		SetPeaceMode( g_pServerSystem->GetMap()->IsVillage() );

		//SW060718 ¸ÊÀÌµ¿ Æê¼ÒÈ¯ ÁßÀÏ ¶§ À§Ä¡ÀÌµ¿. SetInitedGrid() ¿¡¼­..
		GetPetManager()->CheckSummonningPet();

		//GetTitanManager()->CheckRidingTitan();	//Inited ÀÌÈÄ·Î..

		// A¨Ï©ª¡Ë¡þ?d¡§¡þ¡Ë¡þ + ¨Ïo¢®i¢®¨¡¨Ï¨£d¡§¡þ¡Ë¡þ + ¡§u¡§¢®AIAUd¡§¡þ¡Ë¡þ¢®¨¡¡Ë¢ç ¡Ë¡þ¨Ï¡Ì¡ÍiI ¡§uA¡§¢®A¡Íi¡§¢®8¨ÏoC¢®¢´I CA¢®¢´¨ÏoA¡§I¡§uA¡§¢®AEA Send CN¡Ë¡ÍU
		CalcState(); //¡Ë¡þOAu ¢®¨¡e¢®ie¡§uoA¡Ë¢ç¡Íie; ¢®¨¡e¢®ieCN¡Ë¡ÍU. LBS 03.11.20

		//////////////////////////////////////////////////////////////////////////
		// WearedItemInfo Check
		for(POSTYPE part = TP_WEAR_START ; part < TP_WEAR_END ; ++part )
		{
			const ITEMBASE * pTargetItemBase = ITEMMGR->GetItemInfoAbsIn(this, part);
			
			m_HeroCharacterInfo.WearedItemIdx[part-TP_WEAR_START] = 0;
			
			if(pTargetItemBase == NULL)		continue;			
			if(pTargetItemBase->dwDBIdx == 0)	continue;

			m_HeroCharacterInfo.WearedItemIdx[part-TP_WEAR_START] = pTargetItemBase->wIconIdx;
		}
		//////////////////////////////////////////////////////////////////////////
		
		// GuildTournament Observer
		//if( g_pServerSystem->GetMapNum() == GTMAPNUM && m_ObserverBattleIdx )
		if( g_pServerSystem->GetMap()->IsMapKind(eTournament) && m_ObserverBattleIdx )
			m_HeroCharacterInfo.bVisible = FALSE;
		if( (g_pServerSystem->GetMapNum() == SIEGEWARMGR->GetSiegeMapNum()) && 
			SIEGEWARMGR->IsProcessSiegewar() && m_ObserverBattleIdx )
			m_HeroCharacterInfo.bVisible = FALSE;

		SEND_HERO_TOTALINFO msg;
		memset( &msg, 0, sizeof(msg) );
//		msg.InitOptionInfo();
		GetBaseObjectInfo(&msg.BaseObjectInfo);
		GetCharacterTotalInfo(&msg.ChrTotalInfo);
		GetHeroTotalInfo(&msg.HeroTotalInfo);
		GetMugongTotalInfo(&msg.MugongTotalInfo);
		// MallItemInfo ¾ò¾î´Ù°¡ º¸³»ÁØ´Ù. Ãß°¡                                                                                                                                                                                                                                         
		DWORD getItem_Flag;
		if(protocol == MP_SURYUN_ENTER_SYN)
			getItem_Flag = GETITEM_FLAG_WEAR;
		else
			getItem_Flag = GETITEM_FLAG_INVENTORY | GETITEM_FLAG_WEAR | GETITEM_FLAG_PETWEAR | GETITEM_FLAG_TITANWEAR | GETITEM_FLAG_TITANSHOP;	// magi82 - Titan(070227)                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                        
		GetItemtotalInfo(&msg.ItemTotalInfo,getItem_Flag);
		GetSendMoveInfo(&msg.SendMoveInfo,NULL);                                                                                                                                                                                                                                                                                                                                                           
		// ShopItemOption Ãß°¡
		memcpy(&msg.ShopItemOption, GetShopItemStats(), sizeof(SHOPITEMOPTION));
		

		ASSERT(!(msg.SendMoveInfo.CurPos.wx == 0 && msg.SendMoveInfo.CurPos.wz == 0));

		ABILITYMGR->InitAbilityExp(GetAbilityGroup(),m_HeroInfo.AbilityExp);
		ABILITYMGR->UpdateAbilityState(ABILITYUPDATE_CHARACTERLEVEL_CHANGED,msg.ChrTotalInfo.Level,GetAbilityGroup());

		ABILITYMGR->GetAbilityTotalInfo(GetAbilityGroup(),&msg.AbilityInfo);		
		
		msg.ChrTotalInfo.CurMapNum = GAMERESRCMNGR->GetLoadMapNum();
		msg.UniqueIDinAgent = GetUniqueIDinAgent();

		//SW070127 Å¸ÀÌÅº
		msg.TitanEndrncNum = 0;
		WORD TitanEndrncNum = 0;
		TITAN_ENDURANCE_ITEMINFO EndrncInfo[SLOT_INVENTORY_NUM + SLOT_SHOPINVEN_NUM];

		TitanEndrncNum = m_TitanManager.GetTitanEquipItemInfoList(EndrncInfo);

		if(TitanEndrncNum)	//Å¸ÀÌÅº ÀåÂøÅÛ ³»±¸µµ°ü·Ã Á¤º¸
		{
			msg.AddableInfo.AddInfo( CAddableInfoList::TitanEndrncInfo, sizeof(TITAN_ENDURANCE_ITEMINFO)*TitanEndrncNum, EndrncInfo);
			msg.TitanEndrncNum = TitanEndrncNum;
		}

		msg.TitanNum = 0;
		WORD TitanNum = 0;
		TITAN_TOTALINFO TitanInfo[SLOT_INVENTORY_NUM];

		TitanNum = m_TitanManager.GetTitanInfoList(TitanInfo);

		if(TitanNum)
		{
			msg.AddableInfo.AddInfo( CAddableInfoList::TitanTotalInfo, sizeof(TITAN_TOTALINFO)*TitanNum, TitanInfo );
			msg.TitanNum = TitanNum;
		}

		// Pet Info //
		msg.PetNum = 0;
		WORD PetNum = 0;
		PET_TOTALINFO PetInfo[SLOT_INVENTORY_NUM + SLOT_SHOPINVEN_NUM+40];
		
		PetNum = m_PetManager.GetPetInfoList(PetInfo);

		if(PetNum)
		{
			msg.AddableInfo.AddInfo( CAddableInfoList::PetTotalInfo, sizeof(PET_TOTALINFO)*PetNum, PetInfo );
			msg.PetNum = PetNum;
		}
/*
		// ITEM Option Info /////////////////////////////////////////////////////
		m_ItemOptionList.SetPositionHead();
		
		ITEM_OPTION_INFO * pOInfo = NULL;
		while(pOInfo = m_ItemOptionList.GetData())
		{
			msg.AddOptionInfo(pOInfo);
		}

		msg.InitRareOptionInfo();
		m_ItemRareOptionList.SetPositionHead();
		ITEM_RARE_OPTION_INFO* pRInfo = NULL;
		while(pRInfo = m_ItemRareOptionList.GetData())
		{
			msg.AddRareOptionInfo(pRInfo);
		}*/
		//SW051004 ¼öÁ¤	//¸Þ½ÃÁö Å©±â¸¦ ÁÙÀÌ±â À§ÇÑ ¼öÁ¤ÀÌ´Ù.
		//°­È­¿É¼Ç
		msg.OptionNum = 0;	//ÃÊ±âÈ­ ÇÊ
		WORD OptionNum = 0;
		ITEM_OPTION_INFO OptionInfo[MAX_ITEM_OPTION_NUM+SLOT_PYOGUK_NUM]; //Ã¢°íÀÇ °³¼ö¸¦ ´õÇÏÀÚ2006.4.8
		m_ItemOptionList.SetPositionHead();
		ITEM_OPTION_INFO * pOInfo = NULL;
		while(pOInfo = m_ItemOptionList.GetData())
		{
			memcpy(&OptionInfo[OptionNum], pOInfo, sizeof(ITEM_OPTION_INFO));
			++OptionNum;		
			if( OptionNum >= MAX_ITEM_OPTION_NUM+SLOT_PYOGUK_NUM ) //2006.4.8 stack oveflow?
				break;
		}
		if(OptionNum)
		{
			msg.AddableInfo.AddInfo( CAddableInfoList::ItemOption, sizeof(ITEM_OPTION_INFO)*OptionNum, OptionInfo );
			msg.OptionNum = OptionNum;
		}
		//·¹¾î¿É¼Ç
		msg.RareOptionNum = 0;	//ÃÊ±âÈ­ ÇÊ
		OptionNum = 0;
		ITEM_RARE_OPTION_INFO RareOptionInfo[MAX_ITEM_OPTION_NUM+SLOT_PYOGUK_NUM]; //Ã¢°íÀÇ °³¼ö¸¦ ´õÇÏÀÚ2006.4.8
		m_ItemRareOptionList.SetPositionHead();
		ITEM_RARE_OPTION_INFO* pRInfo = NULL;
		while(pRInfo = m_ItemRareOptionList.GetData())
		{
			memcpy(&RareOptionInfo[OptionNum], pRInfo, sizeof(ITEM_RARE_OPTION_INFO));
			++OptionNum;
			if( OptionNum >= MAX_ITEM_OPTION_NUM+SLOT_PYOGUK_NUM ) //2006.4.8 stack oveflow?
				break;
		}
		if(OptionNum)
		{
			msg.AddableInfo.AddInfo( CAddableInfoList::ItemRareOption, sizeof(ITEM_RARE_OPTION_INFO)*OptionNum, RareOptionInfo);
			msg.RareOptionNum = OptionNum;
		}


		// 2014-11-15 ·¢ËÍÐÅÏ¢ 

		msg.StoneOptionNum = 0;	//ÃÊ±âÈ­ ÇÊ
		WORD StoneNum = 0;
		ITEM_STONE_OPTION_INFO StoneOptionInfo[MAX_ITEM_OPTION_NUM+SLOT_PYOGUK_NUM]; 
		m_ItemStoneOptionList.SetPositionHead();
		ITEM_STONE_OPTION_INFO* pSInfo = NULL;
		while(pSInfo = m_ItemStoneOptionList.GetData())
		{
			memcpy(&StoneOptionInfo[StoneNum], pSInfo, sizeof(ITEM_STONE_OPTION_INFO));
			++StoneNum;
			if( StoneNum >= MAX_ITEM_OPTION_NUM+SLOT_PYOGUK_NUM ) 
				break;
		}
		if(StoneNum)
		{
			msg.AddableInfo.AddInfo( CAddableInfoList::ItemStoneOption, sizeof(ITEM_STONE_OPTION_INFO)*StoneNum, StoneOptionInfo);
			msg.StoneOptionNum = StoneNum;
		}

		// magi82 - °³ÀÎÃ¢°í¸¦ ¿­°í ¿¬¹«ÀåÈÄ ´Ù½Ã °³ÀÎÃ¢°í¸¦ ¿­¸é ÃÊ±âÈ­°¡ ¾ÈµÇ¾î ÀÖ¾î¼­ Ä³¸¯ÅÍ°¡ µô»óÅÂ¿¡ °É¸°´Ù..
		SetGotWarehouseItems( FALSE );

		//Ä³¸¯ÅÍ ¼­¹ö ·Î±×ÀÎ ½Ã°£
		GetLocalTime(&msg.ServerTime);
		
		msg.Category = MP_USERCONN;
		msg.Protocol = MP_USERCONN_GAMEIN_ACK;
		SendMsg((MSGBASE*)&msg, msg.GetMsgLength());

		// Weather System
		WEATHERMGR->SendWeather( this );

		VECTOR3 pos;
		msg.SendMoveInfo.CurPos.Decompress(&pos);
//		g_Console.Log(eLogDisplay,4 ,"ConnectUser : %d %s, (x,z):(%f,%f)", 
//										GetID(), GetObjectName(), pos.x, pos.z);
		
		if(protocol != MP_SURYUN_ENTER_SYN)
		{
			BOOL bNotifyUserLogin = TRUE;
			if(protocol != MP_SURYUN_LEAVE_SYN && 
				protocol != MP_USERCONN_GAMEIN_SYN)
				bNotifyUserLogin = FALSE;
			PARTYMGR->UserLogIn(this,bNotifyUserLogin);
			
			GUILDMGR->UserLogIn(this);
		}

		WantedInfoByUserLogIn(GetID());
		//KESA¨Ï¡þ¢®¨¡¡Ë¢ç
		MSG_WORD2 msgMapDesc;
		msgMapDesc.Category		= MP_USERCONN;
		msgMapDesc.Protocol		= MP_USERCONN_MAPDESC;
		msgMapDesc.wData1		= g_pServerSystem->GetMap()->IsVillage();
		msgMapDesc.wData2		= CHANNELSYSTEM->IsPKAllowChannel(GetChannelID());//g_pServerSystem->GetMap()->IsPKAllow();
		SendMsg( &msgMapDesc, sizeof(msgMapDesc) );

		m_initState = PLAYERINITSTATE_ACKMSG_SEND;
	
		SERVERINFO* pserver = g_pServerTable->GetSelfServer();
		RegistLoginMapInfo(GetID(), GetObjectName(), (BYTE)GAMERESRCMNGR->GetLoadMapNum(), (WORD)pserver->wPortForServer);

		m_initState = PLAYERINITSTATE_INITED;
		SetInited();

		//yh 
		MSG_BYTE pymsg;
		pymsg.Category = MP_PYOGUK;
		pymsg.Protocol = MP_PYOGUK_INFO;
		pymsg.bData = GetPyogukNum();
		SendMsg(&pymsg,sizeof(pymsg));
		
		//yh debug code
/*
		if(protocol != MP_SURYUN_ENTER_SYN && protocol != MP_SURYUN_LEAVE_SYN)
		{
			if(GAMERESRCMNGR->CheckCorrectLoginPoint(&pos) == FALSE)
			{
				char temp[128];
				sprintf(temp,"wrong login position [%s], lp:%d, mp:%d",
					m_BaseObjectInfo.ObjectName,
					m_HeroCharacterInfo.LoginPoint_Index,
					m_HeroCharacterInfo.MapChangePoint_Index);
				ASSERTMSG(0,temp);
			}
		}
*/
		
		//
		if(protocol == MP_SURYUN_LEAVE_SYN )
			m_ShopItemManager.SendShopItemUseInfo();

		// guildfieldwar
		GUILDFIELDWARMGR->AddPlayer( this );		
		// GuildTournament
		GTMGR->AddPlayer( this );
		// SiegeWar
		SIEGEWARMGR->AddPlayer( this );
		SWPROFITMGR->AddPlayer( this );

		// fortwar
		FORTWARMGR->AddPlayer( this );

		GUILDWARMGR->AddPlayer(this);

		//SW061019 ¼­¹ÙÀÌ¹ú¸ðµå 
		//if( g_pServerSystem->CheckMapKindIs(eSurvival) )
		if( g_pServerSystem->GetMap()->IsMapKind(eSurvivalMap) )
			SVVMODEMGR->AddSVModeUser(this);

		//SW061220 »óÅÂ±ê¹ßNPC°ü·Ã Ãß°¡. //±ê¹ß Ç¥½Ã ¾ÈÇÏ±â À§ÇÑ »óÅÂ.
		SendFlagNPCState();

		//SW070127 Å¸ÀÌÅº
		ApplyTitanWearItem();	// 1/2
		GetTitanManager()->CheckRidingTitan();	// 2/2


		ForbidChatLoad(GetID());
	}

	
}

int CPlayer::CanExitStart()	//~¢®¢´av¢®¨¡C
{
//	if( GetState() != eObjectState_None && GetState() != eObjectState_Move )
//		return FALSE;
	if( IsPKMode() )
		return eEXITCODE_PKMODE;
	if( LOOTINGMGR->IsLootedPlayer(GetID()) )	//PK¢®¢´c¡§¢®A; ¡Ë¡ÍcCI¡Ë¡ÍA A¨Ï¡þAI¢®¨¡¡Ë¢ç?
		return eEXITCODE_LOOTING;

	if( GetState() == eObjectState_Exchange )	//¡¾©øE?A©¬¢¯¡Ì A¨ú¡¤aCO ¨ùo ¨ú©ªùéU.
		return eEXITCODE_NOT_ALLOW_STATE;

	if( GetState() == eObjectState_StreetStall_Owner ||
		GetState() == eObjectState_StreetStall_Guest )	//©øeA¢®A©¬¢¯¡Ì A¨ú¡¤aCO ¨ùo ¨ú©ªùéU.
		return eEXITCODE_NOT_ALLOW_STATE;

	if( GetState() == eObjectState_Deal )	//¡íoA¢® AI¢¯eA©¬¢¯¡Ì A¨ú¡¤aCO ¨ùo ¨ú©ªùéU.
		return eEXITCODE_NOT_ALLOW_STATE;
	
	return eEXITCODE_OK;
}

void CPlayer::SetExitStart( BOOL bExit )
{
	m_bExit				= bExit;
	m_dwExitStartTime	= gCurTime;
}

int CPlayer::CanExit()
{
	DWORD lCurTime = MHTIMEMGR_OBJ->GetNewCalcCurTime();
	if( lCurTime - m_dwExitStartTime < EXIT_COUNT*1000 - 2000 )	//8.0	//¹öÆÛ¸¦ ÃæºÐÈ÷ ÀâÀÚ
		return eEXITCODE_SPEEDHACK;

	if( IsPKMode() )							//PK¡Ë¡þ¨Ï¡Ì¡ÍiaAI¢®¨¡¡Ë¢ç?
		return eEXITCODE_PKMODE;
	if( LOOTINGMGR->IsLootedPlayer(GetID()) )	//PK¢®¢´c¡§¢®A; ¡Ë¡ÍcCI¡Ë¡ÍA A¨Ï¡þAI¢®¨¡¡Ë¢ç?
		return eEXITCODE_LOOTING;
	
	//KES AUTONOTE
	if( GetAutoAskPlayerIdx() )
		return eEXITCODE_NOT_ALLOW_STATE;
	//---------------

	return eEXITCODE_OK;
}

void CPlayer::ExitCancel()
{
	if( IsExitStart() )
	{
		SetExitStart( FALSE );
		MSG_BYTE msgNack;
		msgNack.Category	= MP_CHAR;
		msgNack.Protocol	= MP_CHAR_EXIT_NACK;
		msgNack.bData		= eEXITCODE_DIEONEXIT;
		SendMsg(&msgNack, sizeof(msgNack));		
	}
}

int CPlayer::PKModeOn()
{
	if( IsPKMode() ) return ePKCODE_ALREADYPKMODEON;		//AI¨ÏoI PK¡Ë¡þ¨Ï¡Ì¡Íia
	if( IsVimuing() ) return ePKCODE_VIMUING;		//¡§¡þn¨Ïo¢®iA¨Ï¡þAI¡Ë¡þe ¡§uE¡ÍiE¡Ë¡ÍU

	//pk¢¬©£¥ìa A¡Æ¢¬e ©ö¡ìAu¡íoAA C¨ªA|
	if( GetState() == eObjectState_Immortal )
		OBJECTSTATEMGR_OBJ->EndObjectState( this, eObjectState_Immortal );

	if( GetState() == eObjectState_Die )
		return ePKCODE_STATECONFLICT;	//¡Ë¡ÍU¡Ë¡þ¢®I¢®ioAAAI¡ËO¢®¡¿¡Ë¡ÍA ¡§uE¡ÍiE¡Ë¡ÍU.
	
	m_HeroCharacterInfo.bPKMode = TRUE;
	m_dwPKModeStartTime			= gCurTime;
	m_dwPKContinueTime			= GetBadFame()/200 + PKMODETIME;
	return ePKCODE_OK;
}
DWORD CPlayer::GetPKOffTime()
{
	return (m_dwPKContinueTime-(gCurTime - m_dwPKModeStartTime))/1000;
}
BOOL CPlayer::PKModeOff()
{
	if( !IsPKMode() ) return FALSE;

	if( gCurTime - m_dwPKModeStartTime >= m_dwPKContinueTime )
	{
		m_HeroCharacterInfo.bPKMode = FALSE;
		SetPKModeEndtime();
		PKCharacterUpdate( GetID(), m_HeroInfo.LastPKModeEndTime );
		return TRUE;
	}

	return FALSE;
}

void CPlayer::PKModeOffForce()
{
	m_HeroCharacterInfo.bPKMode = FALSE;
}

void CPlayer::StateProcess()
{
	switch( GetState() )
	{
	case eObjectState_Die:
		{
			DWORD dwElapsed = gCurTime - m_ObjectState.State_Start_Time;
			if( m_bNeedRevive )
			{
				//SW080515 Ã¤³Îº° ¼¼ÆÃ
				DWORD reviveDelayTime = PLAYERREVIVE_TIME;
				if( CHANNELSYSTEM->IsBattleChannel(GetChannelID()) && IsDieForBattleChannel() )
					reviveDelayTime = PLAYERREVIVE_TIME * 2;

				if( dwElapsed > PLAYERREVIVE_TIME )
				{
					if( m_bDieInSpecialMap )	// quest map
					{
						SendPlayerToMap();
						break;
					}
					if( LOOTINGMGR->IsLootedPlayer( GetID() ) )
						break;
					if( IsReadyToRevive() != TRUE )
						break;
					
					// magi82 - SOS(070724)
					if( GetSOSState() > 0 )
						break;

					MSGBASE msg;
					msg.Category = MP_USERCONN;
					msg.Protocol = MP_USERCONN_READY_TO_REVIVE;
					SendMsg( &msg, sizeof(msg) );

					m_bNeedRevive = FALSE;

					//if( g_pServerSystem->GetMapNum() == BOSSMONSTER_MAP ||
					//	g_pServerSystem->GetMapNum() == BOSSMONSTER_2NDMAP )	// BossMap
					if( g_pServerSystem->GetMap()->IsMapKind(eBossMap) )
					{
						m_bDieInSpecialMap = TRUE;
						m_ObjectState.State_Start_Time = gCurTime;
						break;
					}
				}
			}

			if( m_bDieInSpecialMap )	// BossMap
			{
				//if( g_pServerSystem->GetMapNum() == BOSSMONSTER_MAP ||
				//	g_pServerSystem->GetMapNum() == BOSSMONSTER_2NDMAP )	// BossMap
				if( g_pServerSystem->GetMap()->IsMapKind(eBossMap) )
				if( dwElapsed > 60000 )
				{
					m_bDieInSpecialMap = TRUE;
					m_wReturnMapNum = 12;
					m_bNeedRevive = TRUE;
					break;
				}
			}
		}
		break;
	}

	//in Korea : per 10min
	//in China : per 30min
	DWORD dwInsertLogTime = INSERTLOG_TIME;
#ifdef _CHINA_LOCAL_
	dwInsertLogTime = INSERTLOG_TIME_CHINA;
#endif
#ifdef _HK_LOCAL_
	dwInsertLogTime = INSERTLOG_TIME_HK;
#endif

	if( GetInited() )
	if( gCurTime - m_dwProgressTime >= dwInsertLogTime )
	{
		// ¡Íi¢®¢´, ¢®¨¡¨Ï¢®CeA¡Ë¢ç, ¨Ïo¢®i¢®¨¡¨Ï¨£¢®¨¡¨Ï¢®CeA¡Ë¢ç
		InsertLogExp( eExpLog_Time, GetID(), GetLevel(), 0, GetPlayerExpPoint(), 0, 0, GetPlayerAbilityExpPoint());
		InsertLogMoney(0, GetID(), GetMoney(), 0, GetUserID(), GetMoney(eItemTable_Pyoguk));
		
		if( IsPKMode() ) m_HeroInfo.LastPKModeEndTime = 0;
		// ¡§oA¢®¨¡¢®I¡§¡þ¢®¨¡ ¡Íi¢®¢´ ¨Ïo¢®¢¯ ¢®¨¡¨Ï¢®CeA¡Ë¢ç AuAa		
		UpdateCharacterInfoByTime(GetID(), GetPlayerExpPoint(), GetPlayerAbilityExpPoint(), GetMoney(), m_HeroInfo.Playtime, m_HeroInfo.LastPKModeEndTime );
		
		//pet
		m_PetManager.UpdateCurPetInfo();

		//SW070127 Å¸ÀÌÅº
		m_TitanManager.UpDateCurTitanAndEquipItemInfo();

		for(int i=0; i<SLOT_MUGONGTOTAL_NUM;++i)
		{
			if( m_HeroMugongInfo.mugong[i].dwDBIdx != 0 )
			{
				// ¨Ïo¢®i¢®¨¡¨Ï¨£ d¡§¡þ¡Ë¡þ ¢®¢´I¢®¨ú¢®¢¯
				InsertLogMugongExp(GetID(), 0, m_HeroMugongInfo.mugong[i].wIconIdx, m_HeroMugongInfo.mugong[i].dwDBIdx, 
									m_HeroMugongInfo.mugong[i].Sung, m_HeroMugongInfo.mugong[i].ExpPoint, m_HeroMugongInfo.mugong[i].Position );

				// ¡§oA¢®¨¡¢®I¡§¡þ¢®¨¡ ¨Ïo¢®i¢®¨¡¨Ï¨£ d¡§¡þ¡Ë¡þ AuAa
				UpdateMugongExpByTime(GetID(), m_HeroMugongInfo.mugong[i].dwDBIdx, m_HeroMugongInfo.mugong[i].ExpPoint);				
			}
		}

		m_dwProgressTime = gCurTime;
	}

	// Play Time
	// ¨úiA¡ÀCC ¡¤cCA¥ì¥ìùéA¡ÆA ¥ìu¡¤I¢¬¢¬¥ìaùéAùéI.. ¡ÆA AOA¢¬ùéI¡¾i ¢¯¨Ï¡¾a¨ù¡© CIAU.. ¢´¡í¢´¡í¢´¡í RaMa
	m_ContinuePlayTime += gTickTime;
	DWORD dwPT = m_ContinuePlayTime/1000;
	if(dwPT)
	{
		m_HeroInfo.Playtime += dwPT;
		m_ContinuePlayTime -= dwPT*1000;
	}

	AddBadFameReduceTime();	
	m_ShopItemManager.CheckEndTime();

	//SW051129 Pet
	m_PetManager.PetProcess();

	//SW070127 Å¸ÀÌÅº
	//SW070912 ±âÈ¹º¯°æ
	m_TitanManager.TitanProcess();

	// ºÎÈ°ÁÖ¹®¼­ »ç¿ë½Ã ½Ã°£Ã¼Å©
	if( m_ReviveTime )
	{
		m_ReviveTime -= gTickTime;

		MSG_WORD msg;
		msg.Category = MP_ITEM;
		msg.Protocol = MP_ITEM_SHOPITEM_REVIVEOTHER_NACK;

		CPlayer* pTarget  = (CPlayer*)g_pUserTable->FindUser(m_ReviveData.TargetID);
		if(!pTarget)
		{
			msg.wData = eShopItemErr_Revive_Fail;
			SendMsg( &msg, sizeof(msg) );

			m_ReviveTime = 0;
			memset( &m_ReviveData, 0, sizeof(m_ReviveData) );
		}				
		else if( m_ReviveTime <= 0 )
		{
			msg.wData = eShopItemErr_Revive_TimeOver;
			SendMsg( &msg, sizeof(msg) );
			pTarget->SendMsg( &msg, sizeof(msg) );
			
			m_ReviveTime = 0;
			memset( &m_ReviveData, 0, sizeof(m_ReviveData) );
		}
		else if( GetChannelID() != pTarget->GetChannelID() )
		{
			msg.wData = eShopItemErr_Revive_Fail;
			SendMsg( &msg, sizeof(msg) );
			pTarget->SendMsg( &msg, sizeof(msg) );

			m_ReviveTime = 0;
			memset( &m_ReviveData, 0, sizeof(m_ReviveData) );

			/*
			VECTOR3 mypos, targetpos;
			GetPosition( &mypos );
			pTarget->GetPosition( &targetpos );
			if( CalcDistanceXZ( &mypos, &targetpos ) > (float)(REVIVE_OTHER_DIST + 4000) )	//50m±îÁö
			{
				msg.wData = eShopItemErr_Revive_TooFar;
				SendMsg( &msg, sizeof(msg) );	
				pTarget->SendMsg( &msg, sizeof(msg) );

				m_ReviveTime = 0;
				memset( &m_ReviveData, 0, sizeof(m_ReviveData) );
			}*/
		}
	}

	// 06.09.25 RaMa - Ãß¼®ÀÌº¥Æ®
	if( m_fFullMoonEventTime > 0 )
	{
		m_fFullMoonEventTime -= gTickTime;

		if( m_fFullMoonEventTime <= 0.f )
		{
			m_fFullMoonEventTime = 0.f;
			m_HeroCharacterInfo.EventIndex = eFULLMOONEVENT_NONE;

			MSG_BYTE msg;
			SetProtocol( &msg, MP_CHAR, MP_CHAR_FULLMOONEVENT_CHANGE );
			msg.bData = eFULLMOONEVENT_NONE;
			msg.dwObjectID = GetID();
			PACKEDDATA_OBJ->QuickSend( this, &msg, sizeof(msg) );
		}
	}

	//2007. 12. 10. CBH - ½ºÅ² µô·¹ÀÌ ½Ã°£ Ã³¸®
	SkinDelayTimeProcess();
}

void CPlayer::SetWearedItemIdx(DWORD WearedPosition,WORD ItemIdx)
{	
	if(WearedPosition == eWearedItem_Weapon)
	{
		// ¨Ïo¢®i¢®¨úa¡Ë¡þ| ¨ÏoU¨Ï¡ÀU¡Ë¡þe Ao¡§¢®¨Ï¨£: ¢®ic¡ËOoA¨Ï¨£¡Ë¡ÍU.
		GetSpecialStateParam1(eSpecialState_AmplifiedPowerPhy);
	}

	m_HeroCharacterInfo.WearedItemIdx[WearedPosition] = ItemIdx;
}

void CPlayer::AddWanted(WANTEDTYPE WantedIDX)
{
	m_WantedList.AddTail((void*)WantedIDX);	
}

void CPlayer::DeleteWanted(WANTEDTYPE WantedIDX)
{
	m_WantedList.Remove((void*)WantedIDX);
}

void CPlayer::ClearWantedList()
{
	m_WantedList.RemoveAll();
}

BOOL CPlayer::IsWantedOwner(WANTEDTYPE WantedIDX)
{
	PTRLISTSEARCHSTART(m_WantedList, WANTEDTYPE, id)
		if( id == WantedIDX )
		{
			return TRUE;
		}
	PTRLISTSEARCHEND
		return FALSE;
}

// jsd quest
/*void CPlayer::AddQuestScript( DWORD dwQuestIdx )
{
	m_pQuest = new CQuestScript;
//	*m_pQuest = *TESTQUESTMNG->GetQuest( dwQuestIdx );	
}

BOOL CPlayer::CheckQuestEnd( DWORD dwMonsterIdx )
{
	if( m_pQuest )
	if( m_pQuest->m_QuestCondition.dwMonsterIdx == dwMonsterIdx )
	if( m_pQuest->m_QuestCondition.dwMonsterNum )
	{
		--m_pQuest->m_QuestCondition.dwMonsterNum;
		MSG_DWORD2 msg;
		msg.Category = MP_QUEST;
		msg.Protocol = MP_QUEST_TEST_RUN_ACK;
		msg.dwObjectID = GetID();
		msg.dwData1 = m_pQuest->m_QuestInfo.dwQuestIdx;
		msg.dwData2 = m_pQuest->m_QuestCondition.dwMonsterNum;
		SendMsg( &msg, sizeof(msg) );
		return TRUE;
	}
	return FALSE;
}
*/

void CPlayer::QuestProcess()
{
	m_QuestGroup.Process();
//	if( m_pQuestGroup )
//		m_pQuestGroup->Process();
}


void CPlayer::SetPKModeEndtime()
{
	//CA¡¤©öAI¨öA¡Æ¡ÌA¢¬¡¤I ¨ù¨ù¨¡ACNùéU.
/*	
	m_HeroInfo.LastPKModeEndTime = GetPlayTime();
	m_dwLastReduceHour = 0;

	//DB Update
	PKCharacterUpdate( GetID(), m_HeroInfo.LastPKModeEndTime );
*/
	
	m_HeroInfo.LastPKModeEndTime = 0;
}


void CPlayer::AddBadFameReduceTime()
{
	if( g_pServerSystem->GetMap()->IsVillage() )	return;
	if( GetState() == eObjectState_Immortal )		return;
	if( IsPKMode() )								return;
	if( GetBadFame() == 0 && GetWantedIdx() == 0 )	return;

	m_HeroInfo.LastPKModeEndTime += gTickTime;


	if( GetBadFame() == 0 ) //Çö»ó¼ö¹è  
	{
		if(m_HeroInfo.LastPKModeEndTime > WANTED_DESTROYED_TIME)
		{
			if(GetWantedIdx() != 0 )
			{	
				WANTEDMGR->DestroyWanted(this);
			}
		}     
	}
//	else if( m_HeroInfo.LastPKModeEndTime > 3*3600*1000 )
	// ¾Ç¸íÄ¡ °ü·Ã Á¶Á¤ 10ºÐ¿¡ 1¾¿ ±ïÀÌ°Ô...
	else if( m_HeroInfo.LastPKModeEndTime > 600*1000 )
	{
//		m_HeroInfo.LastPKModeEndTime -= 3600*1000;
		m_HeroInfo.LastPKModeEndTime -= 600*1000;
		PKCharacterUpdate( GetID(), m_HeroInfo.LastPKModeEndTime );

		SetBadFame( GetBadFame()-1 );
		if(GetBadFame() == 0)
			m_HeroInfo.LastPKModeEndTime = 0;
		BadFameCharacterUpdate( GetID(), GetBadFame() );
		FAMEMGR->SendBadFameMsg( this, GetBadFame() );
	}
}




BOOL CPlayer::SpeedHackCheck(WORD wTime)
{
//	if( m_dwHackStartTime == 0 )
//		m_dwHackStartTime = gCurTime;

	BOOL bIsHack = FALSE;

	++m_nHackCount;

	if( gCurTime - m_dwHackStartTime >= 60000 )	//1ºÐ
	{
		if( m_nHackCount >= g_nHackCheckWriteNum )
		{
			InsertSpeedHackCheck( GetID(), GetObjectName(), m_nHackCount, wTime );
			bIsHack = TRUE;
		}
		if( m_nHackCount >= g_nHackCheckNum )
		{
			MSGBASE Msg;
			Msg.Category = MP_HACKCHECK;
			Msg.Protocol = MP_HACKCHECK_BAN_USER_TOAGENT;
			SendMsg( &Msg, sizeof(Msg) );
			bIsHack = TRUE;
		}
		m_nHackCount		= 1;
		m_dwHackStartTime	= gCurTime;
	}
	return bIsHack;
}

void CPlayer::ClearMurderIdx()
{
	SetReadyToRevive(FALSE);
	m_MurdererKind = 0;
	m_MurdererIDX = 0;
	m_MurderObjKind = 0;
	m_bPenaltyByDie = FALSE;
}


DWORD CPlayer::Damage(CObject* pAttacker,RESULTINFO* pDamageInfo)
{
	//SW070127 Å¸ÀÌÅº
	DWORD realDamage = pDamageInfo->RealDamage;

	if( InTitan() )
	{
		DWORD titanObsorb = DWORD((pDamageInfo->RealDamage / 100.f)*90.f + 0.5f);

		if(titanObsorb)
		{
			pDamageInfo->TitanObsorbDamage = titanObsorb;
		}

		// Ä³¸¯ÅÍ
		pDamageInfo->RealDamage =  realDamage - titanObsorb;

		GetCurTitan()->Damage(pAttacker, pDamageInfo);
	}

	DWORD life = GetLife();
	DWORD beforelife = life;

	if( life > pDamageInfo->RealDamage )
	{
		life -= pDamageInfo->RealDamage;
	}
	else
	{
		if( GetUserLevel() == eUSERLEVEL_GM ) // && g_pServerSystem->GetNation() == eNATION_KOREA )
		{
			life = 1;		//gmÀº µ¥¹ÌÁö´Â ¹Þ¾Æµµ Á×Áö ¾Êµµ·Ï
		}
		else
		{
			life = 0;
		}
	}
	
	SetLife(life,FALSE);
	DoDamage(pAttacker,pDamageInfo,beforelife);

	return life;
}

void CPlayer::SetGuildInfo(DWORD GuildIdx, BYTE Grade, char* GuildName, MARKNAMETYPE MarkName)
{
	m_HeroCharacterInfo.MunpaID = GuildIdx;
	m_HeroCharacterInfo.PositionInMunpa = Grade;
	SafeStrCpy(m_HeroCharacterInfo.GuildName, GuildName, MAX_GUILD_NAME+1);
	m_HeroCharacterInfo.MarkName = MarkName;
}

void CPlayer::RSetMoney( MONEYTYPE money, BYTE flag )
{
	CPurse* pPurse = m_ItemContainer.GetPurse(eItemTable_Inventory);
	if( !pPurse ) return;

	pPurse->RSetMoney( money, flag );
	m_HeroInfo.Money = money;
}

void CPlayer::SetGuildMarkName(MARKNAMETYPE MarkName)
{
	m_HeroCharacterInfo.MarkName = MarkName;
}

char* CPlayer::GetGuildCanEntryDate()
{ 
	return m_HeroInfo.MunpaCanEntryDate;
}

void CPlayer::SetGuildCanEntryDate(char* date)
{
	SafeStrCpy(m_HeroInfo.MunpaCanEntryDate, date, 11);
}

void CPlayer::ClrGuildCanEntryDate()
{
	SafeStrCpy(m_HeroInfo.MunpaCanEntryDate, "2004.01.01", 11);
}


void CPlayer::SendShopItemInfo()
{
	SEND_SHOPITEM_INFO ShopItem;
	ShopItem.Category = MP_ITEM;
	ShopItem.Protocol = MP_ITEM_SHOPITEM_INFO_ACK;	
	ShopItem.ItemCount = 0;


	POSTYPE startpos = TP_SHOPITEM_START;

	for(int i=0; i<SLOT_SHOPITEM_IMSI; i++)
	{
		if( m_ShopItemSlot.IsEmpty(startpos+i) == FALSE )
		{
			ShopItem.Item[ShopItem.ItemCount] = *m_ShopItemSlot.GetItemInfoAbs(startpos+i);
			if( ShopItem.Item[ShopItem.ItemCount].ItemParam & ITEM_PARAM_PRESENT )
			{
				((ITEMBASE*)m_ShopItemSlot.GetItemInfoAbs(startpos+i))->ItemParam ^= ITEM_PARAM_PRESENT;				
				ShopItemParamUpdateToDB( GetID(), ShopItem.Item[ShopItem.ItemCount].dwDBIdx, m_ShopItemSlot.GetItemInfoAbs(startpos+i)->ItemParam );
			}
			++ShopItem.ItemCount;
		}
	}

	SendMsg(&ShopItem, ShopItem.GetSize());
}


void CPlayer::UpdateLogoutToDB()
{
	m_ShopItemManager.UpdateLogoutToDB();
	m_PetManager.UpdateLogoutToDB();
}


BOOL CPlayer::IsAbleReviveOther()
{
	if(GetState() != eObjectState_Die)
	{
		ASSERT(0);
		MSG_BYTE msg;
		msg.Category	= MP_USERCONN;
		msg.Protocol	= MP_USERCONN_CHARACTER_REVIVE_NACK;
		msg.bData		= 1; //errorcode
		SendMsg( &msg, sizeof(msg) );

		return FALSE;
	}

	if( LOOTINGMGR->IsLootedPlayer( GetID() ) )	//¡¤c¨¡AA©¬¢¯¡Ì
	{
		MSG_BYTE msg;
		msg.Category	= MP_USERCONN;
		msg.Protocol	= MP_USERCONN_CHARACTER_REVIVE_NACK;
		msg.bData		= 2; //errorcode
		SendMsg( &msg, sizeof(msg) );

		return FALSE;
	}
	
	if( IsExitStart() )
	{
		MSG_BYTE msg;
		msg.Category	= MP_USERCONN;
		msg.Protocol	= MP_USERCONN_CHARACTER_REVIVE_NACK;
		msg.bData		= 4; //errorcode
		SendMsg( &msg, sizeof(msg) );

		return FALSE;
	}

	return TRUE;
}


BOOL CPlayer::ReviveShopItem( WORD ItemIdx )
{
	ITEM_INFO* pItem = ITEMMGR->GetItemInfo( ItemIdx );
	if( !pItem )		return FALSE;

	MOVE_POS msg;
	msg.Category = MP_USERCONN;
	msg.Protocol = MP_USERCONN_CHARACTER_REVIVE;
	msg.dwObjectID = GetID();
	msg.dwMoverID = GetID();
	msg.cpos.Compress(CCharMove::GetPosition(this));
	PACKEDDATA_OBJ->QuickSend(this,&msg,sizeof(msg));
	

/*	if(GetLevel() >= 5)
	{
		// µ· ¹ÝÀ¸·Î  0.04->0.02
		CItemSlot* pSlot = m_ItemContainer.GetSlot(eItemTable_Inventory);
		DWORD CurMoney = pSlot->GetMoney();
		SetMoney( (MONEYTYPE)(CurMoney*0.02), MONEY_SUBTRACTION, MF_LOST , eItemTable_Inventory, eMoneyLog_LoseDie, 0 );

		// °æÇèÄ¡ ¹ÝÀ¸·ç..  0.02->0.01
		EXPTYPE pointForLevel = GAMERESRCMNGR->GetMaxExpPoint(GetLevel());
		EXPTYPE minusExp = (EXPTYPE)(pointForLevel * 0.01);
		ReduceExpPoint(minusExp, eExpLog_LosebyReviveReviveOther);
	}*/

	if( pItem->SellPrice )	
		InsertLogExp( eExpLog_ReviveExpPeriod, GetID(), GetLevel(), GetShopItemStats()->ProtectCount, GetPlayerExpPoint(), m_MurdererKind, m_MurdererIDX, GetPlayerAbilityExpPoint());
	else
		InsertLogExp( eExpLog_ReviveExp, GetID(), GetLevel(), GetShopItemStats()->ProtectCount, GetPlayerExpPoint(), m_MurdererKind, m_MurdererIDX, GetPlayerAbilityExpPoint());

	OBJECTSTATEMGR_OBJ->EndObjectState(this,eObjectState_Die);
	
	DWORD CurLife = GetMaxLife();
	DWORD CurNaeryuk = GetMaxNaeRyuk();
	DWORD CurShield = GetMaxShield();
	SetLife((DWORD)(CurLife));
	SetNaeRyuk((DWORD)(CurNaeryuk));
	SetShield((DWORD)(CurShield));

	m_YYLifeRecoverTime.bStart = FALSE;
	m_YYNaeRyukRecoverTime.bStart = FALSE;
	ClearMurderIdx();


	MSG_DWORD2	msg1;
	msg1.Category = MP_ITEM;
	msg1.Protocol = MP_ITEM_SHOPITEM_EFFECT;				
	msg1.dwData1 = GetID();
	msg1.dwData2 = eEffect_ShopItem_Revive;
	PACKEDDATA_OBJ->QuickSend( this, &msg1, sizeof(msg1) );

	m_bDieForGFW = FALSE;
	m_bDieInSpecialMap = FALSE;

	return TRUE;
}

void CPlayer::ReviveVillage()
{
	if(GetState() != eObjectState_Die)
	{
		ASSERT(0);
		MSG_BYTE msg;
		msg.Category	= MP_USERCONN;
		msg.Protocol	= MP_USERCONN_CHARACTER_REVIVE_NACK;
		msg.bData		= 1; //errorcode
		SendMsg( &msg, sizeof(msg) );

		return;
	}
	
	if( LOOTINGMGR->IsLootedPlayer( GetID() ) )	//¢®¢´c¡§¢®AA¨Ï¡þ¡Ë?¢®I
	{
		MSG_BYTE msg;
		msg.Category	= MP_USERCONN;
		msg.Protocol	= MP_USERCONN_CHARACTER_REVIVE_NACK;
		msg.bData		= 2; //errorcode
		SendMsg( &msg, sizeof(msg) );

		return;
	}

	if( IsExitStart() )
	{
		MSG_BYTE msg;
		msg.Category	= MP_USERCONN;
		msg.Protocol	= MP_USERCONN_CHARACTER_REVIVE_NACK;
		msg.bData		= 4; //errorcode
		SendMsg( &msg, sizeof(msg) );

		return;
	}


	if( IsActionPanelty() == FALSE )
	{
		SetActionPanelty();
		InsetNoActionPaneltyLog( eExpLog_NoActionPanelty_Village );
	}
	else
	{

		if( !EVENTMAPMGR->IsEventMap() )
		if(GetLevel() >= 5 && m_bDieForGFW == FALSE )
		{
			// RaMa -> °æÇèÄ¡+µ·º¸È£
			DWORD ItemIdx = 0;
			SHOPITEMWITHTIME* pShopItem = NULL;
			if( GetShopItemStats()->ProtectCount )		
			{
				DWORD ItemIdx = m_ShopItemManager.GetProtectItemIdx();
				if( ItemIdx == 0 )
					goto ProtectAll_UseFail;
				pShopItem = m_ShopItemManager.GetUsingItemInfo( ItemIdx );
				if( !pShopItem )
					goto ProtectAll_UseFail;

				// LogÃß°¡
				LogItemMoney( GetID(), GetObjectName(), 0, "",
					eLog_ShopItemProtectAll, GetMoney(eItemTable_Inventory), 0, 0,
					ItemIdx, pShopItem->ShopItem.ItemBase.dwDBIdx, pShopItem->ShopItem.ItemBase.Position, 0, pShopItem->ShopItem.ItemBase.Durability, GetPlayerExpPoint());
				InsertLogExp( eExpLog_ProtectExp, GetID(), GetLevel(), GetShopItemStats()->ProtectCount, GetPlayerExpPoint(), m_MurdererKind, m_MurdererIDX, GetPlayerAbilityExpPoint());

				--GetShopItemStats()->ProtectCount;
				if( GetShopItemStats()->ProtectCount <= 0 )
				{
					m_ShopItemManager.SendMsgDwordToPlayer(MP_ITEM_SHOPITEM_USEEND, ItemIdx);
					m_ShopItemManager.DeleteUsingShopItem( ItemIdx );
					m_ShopItemManager.SetProtectItemIdx( 0 );
				}
				else
				{
					// DB¿¡ ¾÷µ¥ÀÌÆ®
					ShopItemUseParamUpdateToDB( 0, GetID(), ItemIdx, GetShopItemStats()->ProtectCount );
				}
				m_ShopItemManager.SendMsgDwordToPlayer(MP_ITEM_SHOPITEM_PROTECTALL, GetShopItemStats()->ProtectCount);
			}
			else
			{
	ProtectAll_UseFail:
				// RaMa -> µ·º¸È£ ÁÖ¹®¼­
				if( m_ShopItemManager.GetUsingItemInfo( eIncantation_MoneyProtect ) )
				{
					m_ShopItemManager.SendMsgDwordToPlayer(MP_ITEM_SHOPITEM_MONEYPROTECT, eIncantation_MoneyProtect);
					m_ShopItemManager.DeleteUsingShopItem( eIncantation_MoneyProtect );
				}
				else
				{
					CItemSlot* pSlot = m_ItemContainer.GetSlot(eItemTable_Inventory);
					DWORD CurMoney = pSlot->GetMoney();
					if( !m_bDieForGFW )
						SetMoney((MONEYTYPE)(CurMoney*0.04), MONEY_SUBTRACTION, MF_LOST, eItemTable_Inventory, eMoneyLog_LoseDie, 0);
				}
				
				// RaMa -> °æÇèÄ¡º¸È£ ÁÖ¹®¼­
				if( m_ShopItemManager.GetUsingItemInfo( eIncantation_ExpProtect ) )
				{
					m_ShopItemManager.SendMsgDwordToPlayer(MP_ITEM_SHOPITEM_EXPPROTECT, eIncantation_ExpProtect);
					m_ShopItemManager.DeleteUsingShopItem( eIncantation_ExpProtect );

				}
				else
				{
					EXPTYPE pointForLevel = GAMERESRCMNGR->GetMaxExpPoint(GetLevel());
					EXPTYPE minusExp = 0;

					if( !m_bDieForGFW )	minusExp = (EXPTYPE)(pointForLevel * 0.01);

					ReduceExpPoint(minusExp, eExpLog_LosebyRevivePresent);

					//SW070531 Æê Ä£¹Ðµµ º¸È£±â´É
					m_PetManager.ReducePetFriendshipWithMastersRevive();
				}
			}
		}

	}		// _KOR_LOCAL_


	if(GetState() == eObjectState_Die)
		OBJECTSTATEMGR_OBJ->EndObjectState( this ,eObjectState_Die );

	MSG_DWORD msg;
	msg.Category = MP_SIEGEWAR;
	msg.Protocol = MP_SIEGEWAR_RETURNTOMAP;
	msg.dwData = GetReturnMapNum();
	SendMsg( &msg, sizeof(msg) );

	g_pServerSystem->RemovePlayer( GetID() );
}

void CPlayer::SetNickName(char* NickName)
{
	SafeStrCpy(m_HeroCharacterInfo.NickName,NickName, MAX_GUILD_NICKNAME+1);
}

BOOL CPlayer::CanSocietyAction( DWORD curTime )
{
	DWORD dwElapsedTime = curTime - m_dwLastSocietyAction;

	if( dwElapsedTime > 2000 )	//2ÃÊ
	{
		m_dwLastSocietyAction = curTime;
		return TRUE;
	}

	return FALSE;
}

LEVELTYPE CPlayer::GetLevel()
{ 
	return m_HeroCharacterInfo.Level; 
}

DWORD CPlayer::GetLife() 
{ 
	return m_HeroCharacterInfo.Life; 
}

DWORD CPlayer::GetShield() 
{ 
	return m_HeroCharacterInfo.Shield; 
}

DWORD CPlayer::GetNaeRyuk()
{ 
	return m_HeroInfo.wNaeRyuk; 
}

DWORD CPlayer::DoGetMaxLife()
{ 
	return m_HeroCharacterInfo.MaxLife; 
}

DWORD CPlayer::DoGetMaxShield()
{ 
	return m_HeroCharacterInfo.MaxShield; 
}

DWORD CPlayer::DoGetMaxNaeRyuk()
{ 
	return m_HeroInfo.wMaxNaeRyuk; 
}

DWORD CPlayer::DoGetPhyDefense()
{ 
	/*// ¹æ¾î °ø½Ä µû·Î Ã³¸®.
	//SW070127 Å¸ÀÌÅº
	if(InTitan())
		return GetCurTitan()->DoGetPhyDefense();
	else*/
	int nPhyDefen = (int)(GetCharStats()->PhysicalDefense+GetItemStats()->PhysicalDefense+GetAbilityStats()->DefenceUp+GetSetItemStats()->wPhyDef+GetUniqueItemStats()->nDefen);
	if(nPhyDefen >= 0)
	{
		return (DWORD)nPhyDefen;
	}
	else
	{
		return 0;
	}
}

float CPlayer::DoGetAttDefense(WORD Attrib)
{
	//SW070127 Å¸ÀÌÅº
	if(InTitan())
		return GetCurTitan()->DoGetAttDefense(Attrib);
	else
		return GetItemStats()->AttributeResist.GetElement_Val(Attrib)+GetAbilityStats()->AttRegistUp.GetElement_Val(Attrib)+GetSetItemStats()->AttrRegist.GetElement_Val(Attrib); 
}	

float CPlayer::GetAttribPlusPercent(WORD Attrib)
{ 
	return GetItemStats()->AttributeAttack.GetElement_Val(Attrib)+GetAbilityStats()->AttAttackUp.GetElement_Val(Attrib)+GetSetItemStats()->AttrAttack.GetElement_Val(Attrib);	
}

void CPlayer::SetStage( BYTE stage )
{
	if( m_HeroCharacterInfo.Stage == stage )	return;
	if( stage == eStage_Normal )
	{
		m_HeroCharacterInfo.Stage = stage;

		MSG_BYTE Msg;
		Msg.Category = MP_CHAR;
		Msg.Protocol = MP_CHAR_STAGE_NOTIFY;
		Msg.dwObjectID = GetID();
		Msg.bData = stage;
		PACKEDDATA_OBJ->QuickSend( this, &Msg, sizeof(Msg) );

		CharacterTotalInfoUpdate( this );	
	}
	else
	{
//		if( m_HeroCharacterInfo.Stage == eStage_Normal )
		{
			m_HeroCharacterInfo.Stage = stage;
			
			MSG_BYTE Msg;
			Msg.Category = MP_CHAR;
			Msg.Protocol = MP_CHAR_STAGE_NOTIFY;
			Msg.dwObjectID = GetID();
			Msg.bData = stage;
			PACKEDDATA_OBJ->QuickSend( this, &Msg, sizeof(Msg) );

			CharacterTotalInfoUpdate( this );	
		}
//		else
//		{
//			if( !(m_HeroCharacterInfo.Stage & stage) )	return;

//			m_HeroCharacterInfo.Stage = stage;
			
//			MSG_BYTE Msg;
//			Msg.Category = MP_CHAR;
//			Msg.Protocol = MP_CHAR_STAGE_NOTIFY;
//			Msg.dwObjectID = GetID();
//			Msg.bData = stage;
//			PACKEDDATA_OBJ->QuickSend( this, &Msg, sizeof(Msg) );

//			CharacterTotalInfoUpdate( this );
//		}
	}

	// 06. 03 ±¹³»¹«½Ö - ÀÌ¿µÁØ
#ifndef _JAPAN_LOCAL_
	//ÀüÁ÷½Ã ¹«½ÖÃÖ´ëÆ÷ÀÎÆ® ¼öÁ¤
	SetMussangMaxPoint();
#endif
}

/*
void CPlayer::DoDie(CObject* pAttacker)
{
//KES EXIT
	ExitCancel();
//KES EXCHANGE 031002
	EXCHANGEMGR->CancelExchange( this );
	STREETSTALLMGR->UserLogOut( this );
//KES PKLOOTING
	LOOTINGMGR->LootingCancel( this );

	OBJECTSTATEMGR_OBJ->StartObjectState(this,eObjectState_Die,pAttacker->GetID());
	
	if(pAttacker->GetObjectKind() == eObjectKind_Player)
	{
		CPlayer* pAttackPlayer = (CPlayer*)pAttacker;
		CBattle* pBattle = pAttacker->GetBattle();
		if(pBattle->GetBattleKind() == eBATTLE_KIND_NONE || pAttacker->GetBattleID() != GetBattleID() )
		{
			if( GUILDFIELDWARMGR->JudgeGuildFieldWar( this, (CPlayer*)pAttacker ) == FALSE )
			{
				//PKLooting!
				//IsLootingSituation¢¯¢®¨ù¡©ùéA ¨¬n©ö¡ìA©¬AIAo ¡ÆE¡íc ¨úECNùéU. ¡¾¡¿¡¤¢®¨ù¡© ¢¯¨Ï¡¾aùéU¡Æ¢® ©øO¨úuùéU.
				if( LOOTINGMGR->IsLootingSituation( this, pAttacker ) )
				{
					LOOTINGMGR->CreateLootingRoom( this, (CPlayer*)pAttacker );
				}
				
				if( IsPKMode() == FALSE &&
					pAttackPlayer->IsPKMode() == TRUE && 
					WANTEDMGR->IsOwner((CPlayer*)pAttacker, GetWantedIdx()) == FALSE )
				{
					SetMurderIDX(pAttacker->GetID());
					SetMurderKind(pAttacker->GetObjectKind());
					WANTEDMGR->SendRegistWanted(this, (CPlayer*)pAttacker);
					//journal insert
					JouranlInsertWanted(GetID(), eJournal_Wanted, eJournal_MurderedbyChr, ((CPlayer*)pAttacker)->GetObjectName());
				}
				else
					SetReadyToRevive(TRUE);
				WANTEDMGR->Complete((CPlayer*)pAttacker, this);
			}
			else
			{
				SetReadyToRevive(TRUE);
				WANTEDMGR->Complete((CPlayer*)pAttacker, this);
			}

			// partywar
			if( PARTYWARMGR->PlayerDie( this, (CPlayer*)pAttacker ) )
			{
				SetReadyToRevive( FALSE );
			}
		}
	}
	else if(pAttacker->GetObjectKind() & eObjectKind_Monster )
	{		
		SetMurderKind(((CMonster*)pAttacker)->GetMonsterKind());
		SetReadyToRevive(TRUE);
	}

	//KES PKPlayerPanelty
	PKMGR->DiePanelty( this, pAttacker );

	// ¨ÏoI¡Ë?i
	//SetGravity(0);
	CMonster * pObject = NULL;
	m_FollowMonsterList.SetPositionHead();
	while(pObject = (CMonster *)m_FollowMonsterList.GetData())
	{
		pObject->SetTObject(NULL);
		//totalG += pObject->GetGravity();
	}
	m_FollowMonsterList.RemoveAll();
}
*/

void CPlayer::SendPlayerToMap()
{
	if( !m_wReturnMapNum )	return;

	MSG_WORD2 Msg;
	Msg.Category = MP_USERCONN;
	Msg.Protocol = MP_USERCONN_BACKTOBEFOREMAP_SYN;
	Msg.wData1 = g_pServerSystem->GetMapNum();
	Msg.wData2 = m_wReturnMapNum;

	SendMsg( &Msg, sizeof(Msg) );

	m_bDieInSpecialMap = FALSE;
	m_wReturnMapNum = 0;
	m_bNeedRevive = FALSE;
}

DOUBLE CPlayer::GetPlayerTotalExpPoint()
{
	DOUBLE exp = 0;

	for(int i=1; i<GetLevel(); ++i)
	{
		exp += GAMERESRCMNGR->GetMaxExpPoint( i );
	}

	exp += GetPlayerExpPoint();

	return exp;
}

void CPlayer::SetChangeAbilityKyungGongLevel( WORD wLevel )
{
	MSG_DWORD2 msg;
	msg.Category = MP_KYUNGGONG;
	msg.Protocol = MP_KYUNGGONG_ABILITY_CHANGE_NOTIFY;
	msg.dwData1 = this->GetID();
	msg.dwData2 = wLevel;
	PACKEDDATA_OBJ->QuickSendExceptObjectSelf( this, &msg, sizeof(msg) );	
}

void CPlayer::InitGuildUnionInfo( DWORD dwGuildUnionIdx, char* pGuildUnionName, DWORD dwMarkIdx )
{
	m_HeroCharacterInfo.dwGuildUnionIdx = dwGuildUnionIdx;
	strncpy( m_HeroCharacterInfo.sGuildUnionName, pGuildUnionName, MAX_GUILD_NAME+1 );
	m_HeroCharacterInfo.dwGuildUnionMarkIdx = dwMarkIdx;
}

void CPlayer::SetGuildUnionInfo( DWORD dwGuildUnionIdx, char* pGuildUnionName, DWORD dwMarkIdx )
{
	m_HeroCharacterInfo.dwGuildUnionIdx = dwGuildUnionIdx;
	strncpy( m_HeroCharacterInfo.sGuildUnionName, pGuildUnionName, MAX_GUILD_NAME+1 );
	m_HeroCharacterInfo.dwGuildUnionMarkIdx = dwMarkIdx;

	MSG_NAME_DWORD3 Msg;
	Msg.Category = MP_GUILD_UNION;
	Msg.Protocol = MP_GUILD_UNION_PLAYER_INFO;
	Msg.dwData1 = GetID();
	Msg.dwData2 = dwGuildUnionIdx;
	Msg.dwData3 = dwMarkIdx;
	strncpy( Msg.Name, m_HeroCharacterInfo.sGuildUnionName, MAX_GUILD_NAME+1 );

	PACKEDDATA_OBJ->QuickSendExceptObjectSelf( this, &Msg, sizeof(Msg) );
}

void CPlayer::SetCharChangeInfo( CHARACTERCHANGE_INFO* pInfo )
{
	m_HeroCharacterInfo.Gender = pInfo->Gender;
	m_HeroCharacterInfo.HairType = pInfo->HairType;
	m_HeroCharacterInfo.FaceType = pInfo->FaceType;
	m_HeroCharacterInfo.Height = pInfo->Height;
	m_HeroCharacterInfo.Width = pInfo->Width;
}

#ifdef _JAPAN_LOCAL_
BOOL CPlayer::ChangeMainCharAttr( int nMainAttr )
{
	m_HeroCharacterInfo.nMainAttr = nMainAttr;
	m_HeroCharacterInfo.nSubAttr = 0;

	// dbupdate
	ChangeCharacterAttr( GetID(), MAKEDWORD(nMainAttr, 0) );
	
	return TRUE;
}

BOOL CPlayer::ChangeSubCharAttr( int nSubAttr )
{
	int nMainAttr = GetMainCharAttr();

	if( nMainAttr == eCA_None )		return FALSE;
	if( nMainAttr == nSubAttr )		return FALSE;

	switch( nMainAttr )
	{
	case eCA_Fire:
		{
			if( nSubAttr != eCA_Earth && nSubAttr != eCA_Tree )		return FALSE;
		}
		break;
	case eCA_Water:
		{
			if( nSubAttr != eCA_Tree && nSubAttr != eCA_Iron )		return FALSE;
		}
		break;
	case eCA_Tree:
		{
			if( nSubAttr != eCA_Fire && nSubAttr != eCA_Water )		return FALSE;
		}
		break;
	case eCA_Iron:
		{
			if( nSubAttr != eCA_Water && nSubAttr != eCA_Earth )	return FALSE;
		}
		break;
	case eCA_Earth:
		{
			if( nSubAttr != eCA_Iron && nSubAttr != eCA_Fire )		return FALSE;
		}
		break;
	}
	
	m_HeroCharacterInfo.nSubAttr = nSubAttr;

	// dbupdate
	ChangeCharacterAttr( GetID(), MAKEDWORD(nMainAttr, nSubAttr) );

	return TRUE;
}

int CPlayer::WhatIsAttrRelation( int nOtherMainAttr )
{
	if( m_HeroCharacterInfo.nMainAttr == eCA_None || nOtherMainAttr == eCA_None )	return eCAR_None;

	int nRelation = nOtherMainAttr - GetMainCharAttr();
	if( nRelation < 0 )	nRelation += 5;

	return nRelation;
}

void CPlayer::SetExtraSlotCount( DWORD PyogukSlot, DWORD InvenSlot, DWORD MugongSlot )
{
	m_HeroCharacterInfo.ExtraInvenSlot = (char)InvenSlot;
	m_HeroCharacterInfo.ExtraPyogukSlot = (char)PyogukSlot;
	m_HeroCharacterInfo.ExtraMugongSlot = (char)MugongSlot;

	m_PyogukSlot.SetExtraSlotCount( PyogukSlot );
	m_InventorySlot.SetExtraSlotCount( InvenSlot );
}

#endif


#ifdef _HK_LOCAL_
void CPlayer::SetExtraSlotCount( DWORD PyogukSlot, DWORD InvenSlot, DWORD MugongSlot )
{
	m_HeroCharacterInfo.ExtraInvenSlot = (char)InvenSlot;
	m_HeroCharacterInfo.ExtraPyogukSlot = (char)PyogukSlot;
	m_HeroCharacterInfo.ExtraMugongSlot = (char)MugongSlot;

	m_PyogukSlot.SetExtraSlotCount( PyogukSlot );
	m_InventorySlot.SetExtraSlotCount( InvenSlot );
}
#endif

#ifdef _TL_LOCAL_
void CPlayer::SetExtraSlotCount( DWORD PyogukSlot, DWORD InvenSlot, DWORD MugongSlot )
{
	m_HeroCharacterInfo.ExtraInvenSlot = (char)InvenSlot;
	m_HeroCharacterInfo.ExtraPyogukSlot = (char)PyogukSlot;
	m_HeroCharacterInfo.ExtraMugongSlot = (char)MugongSlot;

	m_PyogukSlot.SetExtraSlotCount( PyogukSlot );
	m_InventorySlot.SetExtraSlotCount( InvenSlot );
}
#endif

//SW051112 ¹«½Ö¸ðµå
void CPlayer::SetMussangMaxPoint()
{//·¹º§ º¯µ¿½Ã¿¡ ¾´´Ù.
	int iMaxMussangPoint = GetMussangMaxPoint();

	if(iMaxMussangPoint < 0)
	{
		iMaxMussangPoint = 0;
		ASSERT(0);
	}

	if( m_HeroInfo.MaxMussangPoint != (DWORD)iMaxMussangPoint )
	{
		m_HeroInfo.MaxMussangPoint = iMaxMussangPoint;

		MSG_DWORDBYTE msg;	//SendMussangInfo ¿Í °°ÀÌ ¾´´Ù. ·¾¾÷°ú ¹ßµ¿ÁØºñ°¡ °°ÀÌ ÀÌ·ç¾î Áú¼ö ÀÖ±â¶§¹®¿¡ µû·Î ÇÑ´Ù.
		msg.Category = MP_CHAR;
		msg.Protocol = MP_CHAR_MUSSANG_INFO;
		msg.dwObjectID = GetID();
		msg.dwData = m_HeroInfo.MaxMussangPoint;
		msg.bData = eMSS_LevelUp;
		
		SendMsg(&msg,sizeof(msg));
	}
}

void CPlayer::AddMussangPointFromLevel(DWORD dwMonsterLevel)
{
	if( m_bIsMussangReady ) return;
	//°ø½Ä °è»ê
	int dwAddPoint = dwMonsterLevel + dwMonsterLevel - GetLevel();
	//int dwAddPoint = 100;	//!!!test

	//ÃÖ¼Ò 1
	if( dwAddPoint < 1 )
		dwAddPoint = 1;
	// Rama - 06.06.09
	if( m_AvatarOption.MussangCharge )
		dwAddPoint = int(dwAddPoint*(1+m_AvatarOption.MussangCharge*0.01f));

	if( GetPartyIdx() )
	{
		CParty* pParty = PARTYMGR->GetParty( GetPartyIdx() );
		if( pParty )
		{
			int count = pParty->GetMemberCountofMap( GetID() );
			if( count && gPartyEvent[ePartyEvent_MussangCharge].Rate[count-1] )
				dwAddPoint = (DWORD)(dwAddPoint*gPartyEvent[ePartyEvent_MussangCharge].Rate[count-1]);
		}
	}

	//´õÇØÁÖ°í
	m_dwMussangPoint += dwAddPoint;	
	//ÃÖ´ë°ªº¸Á¤
	if( m_dwMussangPoint >= m_HeroInfo.MaxMussangPoint )
	{
		m_dwMussangPoint = m_HeroInfo.MaxMussangPoint;
	}
	//°ªº¸³»±â (ÃÖ´ë°ªÀÌÇÏÀÏ¶§¸¸)
	if( !m_bIsMussangReady )
	{
		//ÃÖ´ë°ªÀÌ¸é ÁØºñ»óÅÂ
		if( m_dwMussangPoint == m_HeroInfo.MaxMussangPoint )
			m_bIsMussangReady = TRUE;
		SendMussangInfo();
	}
}

DWORD CPlayer::GetMussangMaxPoint()	//Ä³¸¯ÅÍ DBÁ¤º¸ ¸®ÅÏÈÄ »ç¿ë.
{
//	return ( ( GetLevel() + 3 ) * 120 ) - ( ( 30 - GetLevel() ) * 10 );
#ifdef _JAPAN_LOCAL_
	return GetLevel()*130 + 60;
#else
	// 06. 03 ±¹³»¹«½Ö - ÀÌ¿µÁØ
	switch(m_HeroCharacterInfo.Stage)
	{
	case eStage_Normal:	
		return GetLevel()*130 + 780;	//( GetLevel() + 9 ) * 120 ) - ( ( 30 - GetLevel() ) * 10 )
	case eStage_Hwa:		
	case eStage_Geuk:
		return GetLevel()*250 + 1620;	//( GetLevel() + 8 ) * 240 ) - ( ( 30 - GetLevel() ) * 10 )
	case eStage_Hyun:	  		
	case eStage_Tal:	
		return GetLevel()*370 + 2220;	//( GetLevel() + 7 ) * 360 ) - ( ( 30 - GetLevel() ) * 10 )
	}

	return 0;
#endif
}

void CPlayer::SendMussangInfo()
{
	//ÇöÀç ¹«½Ö Æ÷ÀÎÆ®¸¦ º¸³½´Ù.
	MSG_DWORDBYTE msg;
	msg.Category = MP_CHAR;
	msg.Protocol = MP_CHAR_MUSSANG_INFO;
	msg.dwObjectID = GetID();
	msg.dwData	= m_dwMussangPoint;

	if( m_bIsMussangReady )	m_bMussangFlag |= eMSS_Ready;	//¹«½ÖÀÌ¸é¼­ °ÔÀÌÁö Ç®(´ë±â)»óÅÂÀÏ ¼ö µµÀÖ´Ù. ¹«½Ö »óÅÂ¿¡¼­ °ÔÀÌÁö ´©Àû.
//	if( m_HeroCharacterInfo.bMussangMode )	m_bMussangFlag |= eMSS_InMode;
//	else m_bMussangFlag |= eMSS_None;

	msg.bData	= m_bMussangFlag;

	SendMsg(&msg,sizeof(msg));

	m_bMussangFlag = 0;
}

DWORD	CPlayer::StartMussangMode()
{
	if( !m_bIsMussangReady )
	{
		ASSERTMSG(0, "MussangPoint:Sync Err");
		return -99;
	}

	m_HeroCharacterInfo.bMussangMode = TRUE;

	m_bIsMussangReady = FALSE;	// ´ë±â »óÅÂ ÃÊ±â

	InitMussangPoint();	// ¹«½Ö Æ÷ÀÎÆ® ÃÊ±â

	SetMussangMode(TRUE);	// ¹«½Ö ¸ðµå

	SendMussangInfo();
	
	m_dwMussangStartTime = gCurTime;

	m_dwMussangTime = 0;
#ifndef _JAPAN_LOCAL_
	// 06. 03 ±¹³»¹«½Ö - ÀÌ¿µÁØ
	switch(m_HeroCharacterInfo.Stage)
	{
	case eStage_Normal:	
		{
			m_wMussangStat = 0;
			m_dwMussangTime = 60000;
		}
		break;
	case eStage_Hwa:		
	case eStage_Geuk:
		{
			m_wMussangStat = 10;
			m_dwMussangTime = 90000;
		}
		break;
	case eStage_Hyun:
	case eStage_Tal:
		{
			m_wMussangStat = 15;
			m_dwMussangTime = 120000;
		}
		break;
	}

	m_dwMussangTime += (m_HeroCharacterInfo.Level / 10) * 10000;

	//SW061120 ½Å±ÔÆê
	DWORD addTime = 0;

	GetPetManager()->GetPetBuffResultRt(ePB_MussangTimeIncrease, &addTime);

	if(addTime)
	{
		m_dwMussangTime += addTime;
	}

	CalcState();
#endif

	return m_dwMussangTime;
}

void CPlayer::CheckMussangTime()
{
	if( !m_HeroCharacterInfo.bMussangMode ) return;	//¹«½Ö »óÅÂ°¡ ¾Æ´Ï¸é ¸®ÅÏ

#ifdef _JAPAN_LOCAL_
	if( gCurTime - m_dwMussangStartTime > MUSSANG_VALID_TIME )
	{
		SetMussangMode(FALSE);
		SendMussangEnd();
	}
#else
	// 06. 03 ±¹³»¹«½Ö - ÀÌ¿µÁØ
	if( gCurTime - m_dwMussangStartTime > m_dwMussangTime )
	{
		SetMussangMode(FALSE);
		SendMussangEnd();

		m_wMussangStat = 0;

	
		CalcState();
	}
#endif
}

void CPlayer::SendMussangEnd()
{
	MSGBASE msg;
	msg.Category = MP_CHAR;
	msg.Protocol = MP_CHAR_MUSSANG_END;
	msg.dwObjectID = GetID();
	
	PACKEDDATA_OBJ->QuickSend(this, &msg, sizeof(MSGBASE));
}

void CPlayer::CheatMussangReady()
{
	m_dwMussangPoint = m_HeroInfo.MaxMussangPoint;
	m_bIsMussangReady = TRUE;

	SendMussangInfo();
}

#ifdef _JAPAN_LOCAL_
DWORD CPlayer::CalcShieldDamage(DWORD ShieldDamage)
{
	DWORD Shield = GetShield();

	//SW070127 Å¸ÀÌÅº
	if(InTitan())	return 0;

	if(Shield < ShieldDamage)
	{
		SetShield(0,FALSE);
		return Shield;
	}
	else
	{
		SetShield(Shield-ShieldDamage,FALSE);
		return ShieldDamage;
	}
}
#else
// 06. 03 ±¹³»¹«½Ö - ÀÌ¿µÁØ
DWORD CPlayer::CalcShieldDamage(DWORD ShieldDamage, DWORD &RealShieldDamage)
{
	DWORD Shield = GetShield();
	RealShieldDamage = ShieldDamage;

	if(IsMussangMode())
		RealShieldDamage = (DWORD)(RealShieldDamage * 0.7);

	if(Shield < RealShieldDamage)
	{
		SetShield(0,FALSE);
		RealShieldDamage = Shield;
		return (WORD)Shield;
	}
	else
	{
		SetShield(Shield-RealShieldDamage,FALSE);
		return ShieldDamage;
	}
}
#endif

////////////////////////////////////////////////////////
//06. 06 2Â÷ ÀüÁ÷ - ÀÌ¿µÁØ
//ÀÌÆåÆ® »ý·«(¹«ÃÊ)
BOOL CPlayer::IsSkipSkill()
{
	//Çö°æ Å»¸¶°¡ ¾Æ´Ï¸é »ç¿ë¸øÇÔ
	if(	m_HeroCharacterInfo.Stage != eStage_Hyun &&
		m_HeroCharacterInfo.Stage != eStage_Tal )
		return FALSE;

	//¹«ÃÊ»óÅÂ°¡ °É·ÁÀÖ´ÂÁö °Ë»ç
	WORD SkipKind = 0;

	PTRLISTSEARCHSTART(m_StatusList,CStatus*,pSL)
		pSL->GetSkipEffectKind(SkipKind);
	PTRLISTSEARCHEND;

	if(SkipKind == 0)
		return FALSE;
	else
		return (BOOL)SkipKind;
}
////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
// 06. 06. 2Â÷ ÀüÁ÷ - ÀÌ¿µÁØ
// ¹«°ø º¯È¯ Ãß°¡
void CPlayer::AddSkillStatsOption(SKILLOPTION *pSkillOption)
{
	m_SkillStatsOption.Life += pSkillOption->Life;
	m_SkillStatsOption.NaeRyuk += pSkillOption->NaeRyuk;
	m_SkillStatsOption.Shield += pSkillOption->Shield;
	m_SkillStatsOption.PhyDef += pSkillOption->PhyDef;
	m_SkillStatsOption.AttDef += pSkillOption->AttDef;

	m_SkillStatsOption.Range += pSkillOption->Range;
	m_SkillStatsOption.ReduceNaeRyuk += pSkillOption->ReduceNaeRyuk;
	m_SkillStatsOption.PhyAtk += pSkillOption->PhyAtk;
	m_SkillStatsOption.BaseAtk += pSkillOption->BaseAtk;
	m_SkillStatsOption.AttAtk += pSkillOption->AttAtk;
}

void CPlayer::RemoveSkillStatsOption(SKILLOPTION *pSkillOption)
{
	m_SkillStatsOption.Life -= pSkillOption->Life;
	m_SkillStatsOption.NaeRyuk -= pSkillOption->NaeRyuk;
	m_SkillStatsOption.Shield -= pSkillOption->Shield;
	m_SkillStatsOption.PhyDef -= pSkillOption->PhyDef;
	m_SkillStatsOption.AttDef -= pSkillOption->AttDef;

	m_SkillStatsOption.Range -= pSkillOption->Range;
	m_SkillStatsOption.ReduceNaeRyuk -= pSkillOption->ReduceNaeRyuk;
	m_SkillStatsOption.PhyAtk -= pSkillOption->PhyAtk;
	m_SkillStatsOption.BaseAtk -= pSkillOption->BaseAtk;
	m_SkillStatsOption.AttAtk -= pSkillOption->AttAtk;
}
//////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////
// 06. 07. »óÅÂ°­Á¦º¯°æ - ÀÌ¿µÁØ
// ÀÏÁ¤ÀÌ»ó ½ºÅ³ »ç¿ë½ÇÆÐ½Ã Æ¯º°ÇÑ »óÅÂ¸¦ Á¦¿ÜÇÑ ³ª¸ÓÁö »óÅÂ´Â ¸ðµÎ ÃÊ±âÈ­
// Á¤»óÀûÀÎ »óÅÂ¿¡¼­´Â °ªÀ» ÃÊ±âÈ­
// ½ÇÆÐ°ªÀ» ´©ÀûÇÏÁö ¾Ê´Â´Ù.
// ¿¬¼Ó ½ÇÆÐ½Ã¿¡¸¸ »óÅÂÇØÁ¦
BOOL CPlayer::CanSkillState()
{
	//¿¬¼Ó 5È¸ ÀÌ»ó ½ÇÆÐ½Ã
	if(m_SkillFailCount >= 5)
	{
		switch(m_BaseObjectInfo.ObjectState)
		{
		// ½ºÅ³À» »ç¿ëÇÒ¼ö ÀÖ´Â »óÅÂ¸é °ªÀ» ÃÊ±âÈ­ÇÏ°í TRUE ¸®ÅÏ
		case eObjectState_None:
		case eObjectState_Move:
		case eObjectState_TiedUp_CanSkill:
			{
				m_SkillFailCount = 0;
				return TRUE;
			}
			break;
		// ½ºÅ³À» »ç¿ëÇÒ¼ö ¾ø´Â »óÅÂÁß Ç®¾îÁàµµ ¹«¹æÇÑ »óÅÂ¸é °ªÀ» ÃÊ±âÈ­ ÇÏ°í
		// »óÅÂ¸¦ ÃÊ±âÈ­ ÇÑµÚ TRUE ¸®ÅÏ
		case eObjectState_SkillStart:
		case eObjectState_SkillSyn:	
		case eObjectState_SkillBinding:
		case eObjectState_SkillUsing:
		case eObjectState_SkillDelay:
		case eObjectState_Society:
		case eObjectState_Enter:
		case eObjectState_Ungijosik:
		case eObjectState_Tactic:
		case eObjectState_Rest:
			{
				m_SkillFailCount = 0;
				m_BaseObjectInfo.ObjectState = eObjectState_None;
				return TRUE;
			}
			break;
		// ±× ¿ÜÀÇ °æ¿ì¿£ °ªÀ» ÃÊ±âÈ­ ÇÏ°í FALSE ¸®ÅÏ
		default:
			{
				m_SkillFailCount = 0;
				return FALSE;
			}
			break;
		}
	}

	// 5È¸ ÀÌÇÏ ÀÏ¶§ ½ºÅ³À» »ç¿ëÇÒ¼ö ¾ø´Â »óÅÂ¸é
	// °ªÀ» Áõ°¡ÇÏ°í FALSE ¸®ÅÏ
	if(m_BaseObjectInfo.ObjectState != eObjectState_None &&
	   m_BaseObjectInfo.ObjectState != eObjectState_Move &&
	   m_BaseObjectInfo.ObjectState != eObjectState_TiedUp_CanSkill )
	{
		m_SkillFailCount++;
		return FALSE;
	}

	// Á¤»ó »óÅÂÀÏ¶§ °ªÀ» ÃÊ±âÈ­ÇÏ°í TRUE ¸®ÅÏ
	m_SkillFailCount = 0;
	return TRUE;
}
////////////////////////////////////////////////////////////////////////////

void CPlayer::IncreaseEventHammerCount()
{
	if( m_HeroCharacterInfo.EventIndex != eFULLMOONEVENT_NONE )
		return;

	++m_FullMoonEventHCt;

	if( m_FullMoonEventHCt >= 4 )
	{
		int rate = rand()%100;
		m_HeroCharacterInfo.EventIndex = GAMERESRCMNGR->GetFullMoonEventIdx( rate );

		if( m_HeroCharacterInfo.EventIndex != eFULLMOONEVENT_NONE )
		{
			m_fFullMoonEventTime = 60000.f;

			// Client·Î º¸³½´Ù.
			MSG_BYTE	msg;
			SetProtocol( &msg, MP_CHAR, MP_CHAR_FULLMOONEVENT_CHANGE );
			msg.bData = m_HeroCharacterInfo.EventIndex;
			msg.dwObjectID = GetID();
			PACKEDDATA_OBJ->QuickSend( this, &msg, sizeof(msg) );		
		}
		m_FullMoonEventHCt = 0;
	}
}

void CPlayer::SummonEventPetRndm()
{
	//ÇöÀç ¼ÒÈ¯ÁßÀÎ ÆêÀÌ ÀÖÀ¸¸é
	if( m_PetManager.GetCurSummonPet() != NULL )
		return;

	//30% È®·ü·Î Æê ¼ÒÈ¯.
#define SUMMONRATE_EVENTPET 10
	int rnd = rand()%100;

	if( SUMMONRATE_EVENTPET < rnd )
		return;
	else
	{
		m_PetManager.SummonEventPet();
	}


}

void CPlayer::SendFlagNPCState()
{
	MSG_DWORD2 msg;
	msg.Category = MP_USERCONN;
	msg.Protocol = MP_USERCONN_FLAGNPC_ONOFF;

	msg.dwData1 = GTMGR->GetGTState();
	msg.dwData2 = SIEGEWARMGR->GetSWState();

	SendMsg(&msg, sizeof(msg));
}

// magi82 - Titan(070209)
void CPlayer::InitTitanWearItemInfo( ITEMBASE* pTitanItemInfo )
{
	m_ItemContainer.GetSlot(eItemTable_Titan)->SetItemInfoAll(pTitanItemInfo);	
}

// magi82 - Titan(070228)
void CPlayer::InitTitanShopItemInfo( ITEMBASE* pTitanItemInfo )
{
	m_ItemContainer.GetSlot(eItemTable_TitanShopItem)->SetItemInfoAll(pTitanItemInfo);	
}

BOOL CPlayer::InTitan()
{
	if( NULL != m_TitanManager.GetCurRidingTitan() )
		return TRUE;
	else
		return FALSE;
}

BOOL CPlayer::CanUseTitanSkill()
{
	//Å¸ÀÌÅº Å¾½Â ÁßÀÎ°¡
	if( FALSE == InTitan() )
		return FALSE;

	//Å¸ÀÌÅº ³»·ÂÀÌ ÃæºÐÇÑ°¡(½ºÅ³¿¡¼­ °Ë»ç)

	//Å¸ÀÌÅº ºÎÇ°Áß ³»±¸µµ 0 À¸·Î ÆÄ±«µÈ °ÍÀÌ ÇÏ³ª¶óµµ ÀÖ´Â°¡.. °¹¼ö È®ÀÎ
	//if( FALSE == m_TitanManager.CheckEquipState() )
	/*±âÈ¹ º¯°æÀ¸·Î Á¦°Å. ÀåÂøÅÛÀº ¼ÒÈ¯/½ºÅ³»ç¿ë°ú ¹«°ü.
	if( FALSE == m_TitanManager.CheckUsingEquipItemNum() )
		return FALSE;

	if( FALSE == m_TitanManager.IsAvaliableEndurance() )
		return FALSE;*/

	return TRUE;
}

void CPlayer::SetTitanWearInfo( POSTYPE slotPos, ITEMBASE* pItemBase, BOOL bIn ) 
{
	m_TitanManager.SetWearedInfo(slotPos, pItemBase, bIn);
}

void CPlayer::ApplyTitanWearItem()
{
	STATSMGR->CalcTitanItemStats(this);

	m_TitanManager.SetTitanStats();

	if(GetInited())
	{
		m_TitanManager.SendTitanStats();
		m_TitanManager.SendTitanPartsChange();	//Å¸ÀÌÅº ºñ¼ÒÈ¯ÀÏ ¶§¸¸ ÆÄÃ÷ º¯°æ°¡´É.
	}

	//2007. 10. 5. CBH - Å¸ÀÌÅº Àåºñ Âø¿ë½Ã Å¸ÀÌÅº EP¼öÄ¡ °»½Å
	if(InTitan() == TRUE)
	{
		CTitan* pRidingTitan = m_TitanManager.GetCurRidingTitan();
		titan_calc_stats* pStats = pRidingTitan->GetTitanStats();
		if(pStats->MaxFuel < pRidingTitan->GetTotalInfo()->Fuel)
		{
			pRidingTitan->SetLife(pStats->MaxFuel);
		}	
	}	
}

///// 2007. 6. 14. CBH - ¼¼Æ®¾ÆÀÌÅÆ ´É·ÂÄ¡ Ã³¸® ÇÔ¼ö
void CPlayer::AddSetitemOption(SET_ITEM_OPTION* pSetItemOption)
{
	float attrvalue = 0;

	/// ±Ù°ñ
	m_setItemStats.wGenGol += pSetItemOption->wGenGol;	
	//¹ÎÃ¸
	m_setItemStats.wMinChub += pSetItemOption->wMinChub;
	//Ã¼·Â
	m_setItemStats.wCheRyuk += pSetItemOption->wCheRyuk;
	//½É¸Æ
	m_setItemStats.wSimMek += pSetItemOption->wSimMek;
	//ÃÖ´ë»ý¸í·Â
	m_setItemStats.dwLife += pSetItemOption->dwLife;
	//È£½Å°­±â
	m_setItemStats.dwShield += pSetItemOption->dwShield;
	//ÃÖ´ë³»·Â
	m_setItemStats.dwNaeRyuk += pSetItemOption->dwNaeRyuk;
	//¼Ó¼º ÀúÇ×·Â	
	m_setItemStats.AttrRegist.AddATTRIBUTE_VAL(pSetItemOption->AttrRegist,1);	
	//±Ù°Å¸® ÃÖ¼Ò °ø°Ý·Â
	m_setItemStats.wMeleeAttackMin += pSetItemOption->wMeleeAttackMin;
	//±Ù°Å¸® ÃÖ´ë °ø°Ý·Â
	m_setItemStats.wMeleeAttackMax += pSetItemOption->wMeleeAttackMax;
	//¿ø°Å¸® ÃÖ¼Ò °ø°Ý·Â
	m_setItemStats.wRangeAttackMin += pSetItemOption->wRangeAttackMin;
	//¿ø°Å¸® ÃÖ´ë °ø°Ý·Â	
	m_setItemStats.wRangeAttackMax += pSetItemOption->wRangeAttackMax;
	//Å©¸®Æ¼ÄÃ
	m_setItemStats.wCriticalPercent += pSetItemOption->wCriticalPercent;	
	//¼Ó¼º °ø°Ý·Â	
	m_setItemStats.AttrAttack.AddATTRIBUTE_VAL(pSetItemOption->AttrAttack,1);		
	//¹°¸®¹æ¾î·Â
	m_setItemStats.wPhyDef += pSetItemOption->wPhyDef;
	//»ý¸í·Â È¸º¹ (°íÁ¤)	
	m_setItemStats.wLifeRecover += pSetItemOption->wLifeRecover;
	//»ý¸í·Â È¸º¹
	m_setItemStats.fLifeRecoverRate += pSetItemOption->fLifeRecoverRate;
	//³»·Â È¸º¹ (°íÁ¤)
	m_setItemStats.wNaeRyukRecover += pSetItemOption->wNaeRyukRecover;
	//³»·Â È¸º¹
	m_setItemStats.fNaeRyukRecoverRate += pSetItemOption->fNaeRyukRecoverRate;
}

void CPlayer::ClearSetitemOption()
{
	memset(&m_setItemStats, 0, sizeof(SET_ITEM_OPTION));
}

void CPlayer::SendNoActionPaneltyMsg()
{
	MSGBASE msg;
	SetProtocol( &msg, MP_CHAR, MP_CHAR_NOACTIONPANELTY_NOTIFY );
	SendMsg( &msg, sizeof(msg) );
}

void CPlayer::InsetNoActionPaneltyLog( BYTE eLogKind )
{
	InsertLogExp( eLogKind, GetID(), GetLevel(), 
		GetMoney(), GetPlayerExpPoint(), m_MurdererKind, m_MurdererIDX, GetPlayerAbilityExpPoint());
}


/////////////////////////////////////////////////////////////////////////////////////

// magi82 - UniqueItem(070626)
void CPlayer::AddUniqueItemOption(UNIQUE_ITEM_OPTION_INFO* pUniqueOption)
{
	m_UniqueItemStats.nHp += pUniqueOption->nHp;
	m_UniqueItemStats.nMp += pUniqueOption->nMp;
	m_UniqueItemStats.nShield += pUniqueOption->nShield;
	m_UniqueItemStats.nPhyDamage += pUniqueOption->nPhyDamage;
	m_UniqueItemStats.nCriDamage += pUniqueOption->nCriDamage;
	m_UniqueItemStats.nCriRate += pUniqueOption->nCriRate;
	m_UniqueItemStats.nGengol += pUniqueOption->nGengol;
	m_UniqueItemStats.nMinChub += pUniqueOption->nMinChub;
	m_UniqueItemStats.nCheRyuk += pUniqueOption->nCheRyuk;
	m_UniqueItemStats.nSimMek += pUniqueOption->nSimMek;
	m_UniqueItemStats.nDefen += pUniqueOption->nDefen;
	m_UniqueItemStats.nRange += pUniqueOption->nRange;
	m_UniqueItemStats.nAttR += pUniqueOption->nAttR;
	m_UniqueItemStats.nEnemyDefen += pUniqueOption->nEnemyDefen;
	m_UniqueItemStats.nShield += pUniqueOption->nShield;
}
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
//2007. 12. 10. CBH - ½ºÅ²±³Ã¼ µô·¹ÀÌ ½Ã°£ °ü·Ã ÃÊ±âÈ­
void CPlayer::InitSkinDelay()	
{
	m_dwSkinDelayTime = 0;
	m_bSkinDelayResult = FALSE;
}

//2007. 12. 10. CBH - ½ºÅ²±³Ã¼ µô·¹ÀÌ ½ÃÀÛ
void CPlayer::StartSkinDelay()
{
	m_dwSkinDelayTime = gCurTime + eSkinDelayTime;
	m_bSkinDelayResult = TRUE;
}

//2007. 12. 10. CBH - ½ºÅ² µô·¹ÀÌ ½Ã°£ Ã³¸®
void CPlayer::SkinDelayTimeProcess()
{
	if(m_bSkinDelayResult == TRUE)
	{
		if(gCurTime > m_dwSkinDelayTime)
		{
			InitSkinDelay();
		}
	}
}

//2007. 12. 10. CBH - ½ºÅ² µô·¹ÀÌÁßÀÎÁö ¹ÝÈ¯
BOOL CPlayer::GetSkinDelayResult()
{
	return m_bSkinDelayResult;
}

//2007. 12. 10. CBH - ½ºÅ² µô·¹ÀÌ ³²Àº½Ã°£ ¹ÝÈ¯
DWORD CPlayer::GetSkinDelayTime()
{
	DWORD dwTime = m_dwSkinDelayTime - gCurTime;
	if(m_bSkinDelayResult == FALSE)
	{
		dwTime = 0;
	}

	return dwTime;
}

// 2014-05-04 MallMonery  GoldMoney Fame Vistor!

void CPlayer::SetMallMoney(MONEYTYPE ChangeValue,BYTE nFlag,DWORD ItemIdx)
{

	long  HeroTotalMallMoney= m_HeroInfo.MallMoney;

	long  ChangeMallMoney=ChangeValue;

	long  LastMallMoney= m_HeroInfo.MallMoney;

	if(nFlag==1)
	{
     	LastMallMoney = HeroTotalMallMoney-ChangeValue;
	}
	if(nFlag==2)
	{
        LastMallMoney = HeroTotalMallMoney+ChangeValue;
	}

	if( LastMallMoney<0)
	{
		LastMallMoney=0;
	}

	m_HeroInfo.MallMoney=LastMallMoney;

	MSG_MONEY msgMoney;
	msgMoney.Category = MP_ITEM;
	msgMoney.Protocol = MP_ITEM_MALL_MONEY;
	msgMoney.dwObjectID = GetID();
	msgMoney.dwTotalMoney = ChangeValue;
	msgMoney.ItemIdx=ItemIdx;
	msgMoney.bFlag = nFlag;
	SendMsg(&msgMoney, sizeof(msgMoney));

	CharacterHeroInfoUpdate(this);
}

void CPlayer::SetGoldMoney(MONEYTYPE ChangeValue,BYTE nFlag,DWORD ItemIdx,DWORD ItemDurability)
{
	long  HeroTotalGoldMoney= m_HeroInfo.GoldMoney;

	long  ChangeGoldMoney=ChangeValue;

	long  LastGoldMoney=m_HeroInfo.GoldMoney;
		
	if(nFlag==1 || nFlag==2 || nFlag==3||nFlag==6)
	{
		LastGoldMoney  = HeroTotalGoldMoney-ChangeGoldMoney;
		if(ItemIdx!=0)
		{	//Ôª±¦½»Ò×ÈÕÖ¾	by:ºúººÈý	QQ:112582793
			LogGoldMoney(eLog_GoldMoneyBuyItem,GetID(),m_HeroInfo.GoldMoney,65533,m_HeroInfo.GoldMoney-ChangeValue,ChangeValue,ItemIdx,ItemDurability);
		}



		/*if(ItemIdx!=0)
		{	//Ôª±¦½»Ò×ÈÕÖ¾	by:ºúººÈý	QQ:112582793
			LogGoldMoney(eLog_GoldMoneyBuyItem,GetID(),m_HeroInfo.GoldMoney,65533,m_HeroInfo.GoldMoney-ChangeValue,ChangeValue,ItemIdx,ItemDurability);
		}*/
	}
    if(nFlag==4)
	{       
        LastGoldMoney  = HeroTotalGoldMoney+ChangeGoldMoney;
	}
	if(LastGoldMoney<0)
	{
		LastGoldMoney  =0;
	}

	m_HeroInfo.GoldMoney= LastGoldMoney;

	MSG_MONEY msgMoney;
	msgMoney.Category = MP_ITEM;
	msgMoney.Protocol = MP_ITEM_GOLD_MONEY;
	msgMoney.dwObjectID = GetID();
	msgMoney.dwTotalMoney= ChangeValue;
	msgMoney.dwRealMoney = m_HeroInfo.GoldMoney;
	msgMoney.ItemIdx=ItemIdx;
	msgMoney.bFlag = nFlag;
	SendMsg(&msgMoney, sizeof(msgMoney));
	CharacterHeroGoldInfoUpdate(GetID(),LastGoldMoney); //ÔÚÏß³äÖµÔª±¦Ë¢ÐÂ by:ºúººÈý	QQ:112582793
}


void CPlayer::SetSwMoney(MONEYTYPE ChangeValue,BYTE nFlag,DWORD ItemIdx)
{
	m_HeroInfo.Fame = m_HeroInfo.Fame -ChangeValue;

	if(m_HeroInfo.Fame<0)
	{
		m_HeroInfo.Fame=0;
	}

	MSG_MONEY msgMoney;
	msgMoney.Category = MP_ITEM;
	msgMoney.Protocol = MP_ITEM_SW_MONEY;
	msgMoney.dwObjectID = GetID();
	msgMoney.dwTotalMoney = ChangeValue;
	msgMoney.ItemIdx=ItemIdx;
	msgMoney.bFlag = nFlag;
	SendMsg(&msgMoney, sizeof(msgMoney));

	CharacterHeroInfoUpdate(this);
}

void CPlayer::SetReSet(DWORD Val,WORD Type)
{
	m_HeroInfo.ReSet = Val;

	if(m_HeroInfo.ReSet<0)
	{
		m_HeroInfo.ReSet=0;
	}

	MSG_DWORD2 msg;
	msg.Category = MP_CHAR;
	msg.Protocol = MP_CHAR_CHANGELIFE_ACK;
	msg.dwObjectID=GetID();
	msg.dwData1=Val;
	msg.dwData2=Type;
	SendMsg(&msg, sizeof(msg));

	CharacterHeroInfoUpdate(this);
}




void CPlayer::SetReSetN(DWORD Val,WORD Type)
{

	MSG_DWORD2 msg;
	msg.Category = MP_CHAR;
	msg.Protocol = MP_CHAR_CHANGELIFE_NACK;
	msg.dwObjectID=GetID();
	msg.dwData1=Val;
	msg.dwData2=Type;
	SendMsg(&msg, sizeof(msg));

}

void CPlayer::SetMarryName(char* pName)
{
	if (pName == NULL)
		return;

	SafeStrCpy(m_HeroCharacterInfo.MarryName,pName,MAX_NAME_LENGTH + 1);

	//weiye Êý¾Ý¿â¸üÐÂ¹ý³Ì·ÅÔÚ»Øµ÷º¯ÊýÖÐ´¦Àí 2018-05-01
}
char* CPlayer::GetMarryName()
{
	return m_HeroCharacterInfo.MarryName;
}
bool CPlayer::IsMarry()
{
	return strcmp("0", GetMarryName()) != 0;
}

void CPlayer::SetShiFu(CPlayer* pShiFu)
{
	m_HeroCharacterInfo.ShiTuId=pShiFu->GetID();

	SafeStrCpy( m_HeroCharacterInfo.ShiTuName, pShiFu->GetObjectName(), MAX_NAME_LENGTH + 1 );

	CharacterHeroInfoUpdate(this);
}
void CPlayer::ClearShiFu(DWORD ShiFuId)
{
	m_HeroCharacterInfo.ShiTuId=ShiFuId;

	ZeroMemory(m_HeroCharacterInfo.ShiTuName,sizeof(m_HeroCharacterInfo.ShiTuName));

	CharacterHeroInfoUpdate(this);

}

// 2014-05-30 the mallmoney process 

void CPlayer::MallMoneyProcess()
{
	if (GetLevel() >= CServerSystem::PaoDianLevel && GetReSet() >=CServerSystem::PaoDianReSet)
	{
		if( GetState() != eObjectState_Die )
		{
			m_LastPaoDianTime -= g_pServerSystem->GetMainProcessTime();

			if (m_LastPaoDianTime <= 0)
			{
				SetMallMoney(CServerSystem::PaoDianMap,2, 0);

				m_LastPaoDianTime = 60 * CServerSystem::PaoDianTime * 1000;
			}
		}
	}

	

	//if( DungeonMGR->IsDungeon(GetCurMapNum()) && 
	//	!GetShopItemManager()->IsCanInPutFBMap() && 
	//	DungeonMGR->GetFubenKey(m_HeroCharacterInfo.MapChangePoint_Index)==1)
	{//¸±±¾µØÍ¼´Ë´¦¼ì²éÊÇ·ñ·ûºÏ½øÈëÌõ¼þ£¬·ÀÖ¹¶Ý¼×·É½øÈ¥	by:ºúººÈý	QQ:112582793

	//	DungeonMGR->ReturnToMap(this);
		//DungeonMGR->MoveToLoginMap(GetID());
		/*MSG_DWORD3 msg;
		msg.Category = MP_DUNGEON;
		msg.Protocol = MP_DUNGEON_INFO_ACK;
		msg.dwObjectID = GetID();
		msg.dwData1 = 2;
		SendMsg(&msg,sizeof(msg));*/
	}

	/*if(g_pServerSystem->GetMap()->IsMapKind(eInsDungeon,GetCurMapNum()))
	{
		m_LastMapTime -= g_pServerSystem->GetMainProcessTime();
		if (m_LastMapTime <= 0)
		{
			SetFBTime(GetFBTime()-60000);
			m_LastMapTime = 60000;
		}
	}*/

	//weiye ¸±±¾Í¨¹ýµ¥¶ÀµÄ½ø³Ì´¦Àí 2018-02-06
}

void CPlayer::SetPlayerLucker()
{
#ifdef _CONQMSO_
	DWORD lCurTime = MHTIMEMGR_OBJ->GetNewCalcCurTime();
	if (lCurTime - m_bLuckerServerTimingLock< (6 * 1000)/*30000*/)
	{
		MSG_DWORD msg;
		msg.Category = MP_ITEM;
		msg.Protocol = MP_ITEM_LUCKER_NACK;
		msg.dwData = 4;
		SendMsg(&msg, sizeof(msg));
		//TidyFailed(CServerSystem::TidyMallTime - (lCurTime - m_bLuckerServerTimingLock) / 1000);
		return;
	}
#endif

	


	ITEM_LUCKER_INFO * pLuckInfo=ITEMMGR->GetRandLuckerItem();

	DWORD pLuckerIdx;

	pLuckerIdx=0;

	if(pLuckInfo)
	{
		ITEM_INFO* pItemInfo = ITEMMGR->GetItemInfo( pLuckInfo->ItemIdx );

		if(!pItemInfo)		return;

		if( pItemInfo->ItemKind & eSHOP_ITEM )
		{
			ITEMMGR->CheatObtainShopItem(this, pLuckInfo->ItemIdx,1 );
		}
		else
		{
			ITEMMGR->CheatObtainItem(this, pLuckInfo->ItemIdx,1);
		}

		pLuckerIdx=pLuckInfo->Idx;
		
		if(pLuckInfo->ItemMsg==1)
		{

			char ItemName[MAX_ITEMNAME_LENGTH+1];

			sprintf(ItemName,pItemInfo->ItemName);

			MSG_CHAT_WORD2 msg;
			SetProtocol(&msg,MP_CLIENT,MP_CLIENT_SECOND_AGENT);
			msg.Flag = eLuckyDraw;
			SafeStrCpy(msg.Name1,GetObjectName(),MAX_NAME_LENGTH+1);
			msg.dwData=pItemInfo->ItemIdx;
			SendMsg(&msg,sizeof(msg));
		}
	}

	pLuckInfo=NULL;	
	MSG_DWORD msg;
	SetProtocol(&msg,MP_ITEM,MP_ITEM_LUCKER_ACK);
	msg.dwData=pLuckerIdx;
	SendMsg(&msg,sizeof(msg));

	if(CServerSystem::LuckerMode==1)
		SetGoldMoney(CServerSystem::LuckerIngot,2,0);
	else if(CServerSystem::LuckerMode==2)
		SetMallMoney(CServerSystem::LuckerBubble, 1, 0);
	else if(CServerSystem::LuckerMode==3)
		SetMoney(CServerSystem::LuckerGold,1,0);

	m_bLuckerServerTimingLock = MHTIMEMGR_OBJ->GetNewCalcCurTime();
}

// 2015-01-15  

void CPlayer::AddItemGrowKillMonster(CMonster * Monster)
{
	if(Monster)
	{
		const ITEMBASE * pTargetItem = ITEMMGR->GetItemInfoAbsIn(this, 81);  // 

		if(pTargetItem)
		{
			m_ItemGrowCount=m_ItemGrowCount+1;

			if(m_ItemGrowCount>50)  // 
			{
				ItemGrowUpdate(this->GetID(),pTargetItem->dwDBIdx,100);  //

				m_ItemGrowCount=0;  // 
				
			}
		}
	}
}

// 2015-01-28 

void CPlayer::SetGambleInfo(GAMBLE_USER_INFO * pGambleInfo)
{
	memcpy(&m_GambleInfo,pGambleInfo,sizeof(GAMBLE_USER_INFO));
}
// 2015-01-30 

void CPlayer::ClearGambInfo()
{
	ZeroMemory( &m_GambleInfo, sizeof(GAMBLE_USER_INFO) );
}

//weiye °²È«ËøÏà¹Øº¯Êý 2017-12-04

void CPlayer::SetSafeStatic(BOOL Val)
{
	m_IsUnLock=Val;

	if(m_IsUnLock) //weiye ½âËø³É¹¦·¢ËÍ
	{
		MSG_WORD msg;
		SetProtocol(&msg, MP_ITEM,MP_ITEM_SAFE_CHECK_ACK);
		msg.dwObjectID=GetID();
		msg.wData=1;
		SendMsg(&msg, sizeof(msg));
	}
	else          //weiye Ëø¶¨³É¹¦·¢ËÍ
	{
   		MSG_WORD msg;
		SetProtocol(&msg, MP_ITEM,MP_ITEM_SAFE_LOCK_ACK);
		msg.dwObjectID=GetID();
		msg.wData=1;
		SendMsg(&msg, sizeof(msg));
	}
}
void CPlayer::SetSafeStaticValue(BOOL Val)
{
    m_IsUnLock=Val;
}


void CPlayer::SetFBTime(DWORD Val)
{
    /*if(Val<0)
	{
		Val=0;
	}*/

	FBTimeUpdate(GetID(),Val);
}


void CPlayer::SetFBTimeTo(DWORD Val)
{
	m_FBTime=Val;
	MSG_DWORD msg;
	msg.Category=MP_CHAR;
	msg.Protocol=MP_CHAR_FB_TIME_ACK;
	msg.dwObjectID=GetID();
	msg.dwData=Val;
	SendMsg( &msg, sizeof(msg) );
}



BOOL CPlayer::CheckFBTime(DWORD MapIndex)
{
   return TRUE;
}
void CPlayer::SetItemTidy()
{
	DWORD lCurTime = MHTIMEMGR_OBJ->GetNewCalcCurTime();
	if(lCurTime-m_bTidyTime< (CServerSystem::TidyItemTime*1000)/*30000*/)
	{
		TidyFailed(CServerSystem::TidyItemTime-(lCurTime-m_bTidyTime)/1000);
		return;
	}

	if (0 == IsCanMovePack())
	{
		bool bRet = false;
		MoveInventPack(bRet);
		if (bRet)
		{
			//LoadCharacterItemInfoForMovePak(pPlayer->GetID());
			LoadTidyItemInfo(GetID(), MP_USERCONN_GAMEIN_SYN);
		}
		else
		{
	//		MSGBASE msg;
	//		msg.Category = MP_ITEMEXT;
	//		msg.Protocol = MP_ITEMEX_PAKMOVED_ACK;
	//		pPlayer->SendMsg(&msg, sizeof(msg));
		}
	}
	//TidyItem(GetID(),MAX_YOUNGYAKITEM_DUPNUM);

	m_bTidyTime= MHTIMEMGR_OBJ->GetNewCalcCurTime();
}
void CPlayer::SetItemShopTidy()
{	
	DWORD lCurTime = MHTIMEMGR_OBJ->GetNewCalcCurTime();
	if(lCurTime-m_bTidyShopTime< (CServerSystem::TidyMallTime*1000)/*30000*/)
	{
		TidyFailed(CServerSystem::TidyMallTime-(lCurTime-m_bTidyShopTime)/1000);
		return;
	}
	TidyItemShop(GetID(),MAX_YOUNGYAKITEM_DUPNUM);

	m_bTidyShopTime= MHTIMEMGR_OBJ->GetNewCalcCurTime();
}
void CPlayer::SendItemTidyInfo()
{	
	ITEMBASE pInventItemInfo[SLOT_INVENTORY_NUM];
	m_ItemContainer.GetSlot(eItemTable_Inventory)->GetItemInfoAll(pInventItemInfo);
	MSG_TIDY_ITEM msg;
	msg.dwObjectID=GetID();
	msg.Category=MP_ITEMEXT;
	msg.Protocol=MP_ITEMEXT_TIDY_ACK;

	//SetProtocol(&msg,MP_CLIENT,MP_CLIENT_SECOND_AGENT);

	msg.CyptKey=CServerSystem::TidyItemTime;
	memcpy(&msg.Inventory,pInventItemInfo,SLOT_INVENTORY_NUM * sizeof(ITEMBASE));
	SendMsg(&msg,sizeof(MSG_TIDY_ITEM));
		
}
void CPlayer::SendItemTidyShopInfo()
{	
	ITEMBASE pInventItemShopInfo[TABCELL_SHOPINVEN_NUM+40];
	m_ItemContainer.GetSlot(eItemTable_ShopInven)->GetItemInfoAll(pInventItemShopInfo);
	MSG_TIDY_SHOPITEM msg;
	msg.dwObjectID=GetID();
	msg.Category=MP_ITEMEXT;
	msg.Protocol=MP_ITEMEXT_TIDY_SHOP_ACK;
	msg.CyptKey=CServerSystem::TidyMallTime;
	memcpy(&msg.ShopInventory,pInventItemShopInfo,(TABCELL_SHOPINVEN_NUM+40) * sizeof(ITEMBASE));
	SendMsg(&msg,sizeof(MSG_TIDY_SHOPITEM));
		
}
void CPlayer::TidyFailed(DWORD dwTime)
{
	
	MSG_DWORD msg;
	msg.dwObjectID=GetID();
	msg.Category=MP_ITEMEXT;
	msg.Protocol=MP_ITEMEXT_TIDY_NACK;
	msg.dwData=dwTime;
	SendMsg(&msg,sizeof(MSG_DWORD));
		
}

void CPlayer::CountBeforeFame(CMonster * Monster)
{
	if((this->GetLevel())-(Monster->GetLevel())<=10)
	if(m_KillExpFlag!=0)
	{
		if(Monster)
		{
			m_KillMonsterCountFame +=1;			
			if(m_KillMonsterCountFame>=gFameSet[1])
			{
				m_KillExpFlag=0;
				m_ItemFameCount += gFameSet[2];
				m_KillMonsterCountFame=0;
				if(m_ItemFameCount>=1)
				{
					m_KillExpFlag=0;
					if (this->GetFame() < gFameSet[0])//kiv
					{
						SetHeroFame(gFameSet[2],"","",eFameIncreased);
						m_ItemFameCount = 0;
					}
				}
			}
		}
	}
}
void CPlayer::SetHeroFame(int val,char * aName,char * dName,BYTE Type )
{
	//TestPet();
	
	//if(gFameSet[3]==0&&gFameSet[4]==0)return;


	if(val==0)return;

	if (gFameSet[eFameNoIncreaseIfOpponentVal] != 0)		// else than zero defined as enabled
	if (Type == eYouKillOther || Type == eYouKilledBy)
	{
		if (GetFame() <= gFameSet[eFameNoIncreaseIfOpponentVal])
		{
			Type = eFameOpponentBelowLimit;
		}
	}



	switch(Type)
	{
	case eFameIncreased:
	case eYouKillOther:
		{
			m_HeroInfo.Fame += val;			
		}
		break;
	case eFameOpponentBelowLimit:
	case eOpponentHasNoFame:
		{
			break;
		}
	case eYouKilledBy:
		{
			if(m_HeroInfo.Fame>0)
				m_HeroInfo.Fame -= val;
		}
		break;		
	case eFameDecreased:
		{
			m_HeroInfo.Fame -= val;
		}
		break;
	case eFameCleared:
		{
			if(m_HeroInfo.Fame>0)
				m_HeroInfo.Fame *= 0;
		}
		break;
	case eFameDataMax:
		{
			if(val>gFameSet[0])
				m_HeroInfo.Fame = gFameSet[0];
			else
				m_HeroInfo.Fame = val;
		}
		break;
	}

	if (m_HeroInfo.Fame >= gFameSet[0])
		m_HeroInfo.Fame = gFameSet[0];


	MSG_FAME_ADD  msg;
	msg.Category = MP_ITEM;
	msg.Protocol = MP_ITEM_FAME_ADD;
	msg.bFlag = Type;
	msg.wFame = val;
	SafeStrCpy(msg.aName,aName,MAX_NAME_LENGTH + 1);
	SafeStrCpy(msg.dName,dName,MAX_NAME_LENGTH + 1);
	SendMsg(&msg,sizeof(msg));

	FAMEMGR->SendFame(this,m_HeroInfo.Fame);
}
void CPlayer::GetDBGoldMoney()  //ÔÚÏß³äÖµÔª±¦Ë¢ÐÂ	by:ºúººÈý	QQ:112582793
{
	char txt[64];
	sprintf(txt,"EXEC %s %d", "dbo.NEW_SELECT_USER_CREDIT", GetID());
	g_DB.Query(eQueryType_FreeQuery,eGetDBGoldMoney,GetID(),txt );
}

void CPlayer::UpdateGoldMoney(DWORD DBGoldMoney,DWORD ChangeMoney)//ÔÚÏß³äÖµÔª±¦Ë¢ÐÂ	by:ºúººÈý	QQ:112582793
{
	m_HeroInfo.GoldMoney = DBGoldMoney;
	MSG_MONEY msgMoney;
	msgMoney.Category = MP_ITEM;
	msgMoney.Protocol = MP_ITEM_GOLD_MONEY;
	msgMoney.dwObjectID = GetID();
	msgMoney.dwTotalMoney= ChangeMoney;
	msgMoney.dwRealMoney = m_HeroInfo.GoldMoney;
	msgMoney.ItemIdx=0;
	msgMoney.bFlag = 5;
	SendMsg(&msgMoney, sizeof(msgMoney));
	CharacterHeroInfoUpdate(this);
}
void CPlayer::StartLoginEvent()
{
	/*bFirstEvent=true;
	LOGINEVENTINFO * pEvent=LOGINEVENTMGR->GetLoginEventItemInfo(GetEventIDFlag());
	if(pEvent)
	{
		if(bFirstEvent)
		{
			bFirstEvent=FALSE;
			//m_EventCountDownTimer=60*pEvent->Count*1000;
			LOGINEVENTMGR->LoginEventSend(this,0,m_EventCountDownTimer,pEvent->dwItemIdx,pEvent->cTextSend,pEvent->dwQuantity,60*pEvent->dwTiming*1000);
			
		}	
	}*/
}
void CPlayer::LoginEventProcess()
{
	if( GetState() != eObjectState_Die )
	{
		LOGINEVENTINFO * pEvent=LOGINEVENTMGR->GetLoginEventItemInfo(GetEventIDFlag());
		if(pEvent)
		{
			if(pEvent->Key==0&&pEvent->dwNextLine==0)
			{
				memset( &m_EventCountDownTimer, 0, sizeof(LOGINEVENTINFO) );
				bFirstEvent=FALSE;
				m_EventIDFlag=0;
			}

		//	if( m_EventIDFlag == 1 && m_EventCountDownTimer ==0 && bFirstEvent )
		//	{
		//		m_EventCountDownTimer=/*60*pEvent->Count*1000*/pEvent->dwTiming;
		//		LOGINEVENTMGR->LoginEventSend(this,0,m_EventCountDownTimer,pEvent->dwItemIdx,pEvent->cTextSend,pEvent->dwQuantity,pEvent->dwTiming);
		//		bFirstEvent=FALSE;
		//		return;
		//	}
			if(bFirstEvent)
			{
				
				//m_EventCountDownTimer=/*60*pEvent->Count*1000*/pEvent->dwTiming;
				LOGINEVENTMGR->LoginEventSend(this,0,m_EventCountDownTimer,pEvent->dwItemIdx,pEvent->cTextSend,pEvent->dwQuantity,pEvent->dwTiming);
				bFirstEvent=FALSE;
			}
						

			if( GetEventIDFlag()!=0 )
			{
				m_EventCountDownTimer -= g_pServerSystem->GetMainProcessTime();				
			}
			
			if (m_EventCountDownTimer <= 0)
			{
				if(bFirstEvent)
					return;

				ITEMMGR->ObtainItemFromGameEvent( this, pEvent->dwItemIdx, pEvent->dwQuantity );

				

				

				/*ITEM_INFO* pItemInfo = ITEMMGR->GetItemInfo(pEvent->dwItemIdx);
				BYTE rt = FALSE;


				if( pItemInfo )
				{
					if( pItemInfo->ItemKind & eSHOP_ITEM )
					{
						rt = ITEMMGR->CheatObtainShopItem(this,pItemInfo->ItemIdx,pEvent->dwQuantity);
						if(rt)//equals to 1 / true
							rt=rt+1;//sum is 2
					}
					else
					{
						rt = ITEMMGR->CheatObtainItem(this,pItemInfo->ItemIdx,pEvent->dwQuantity);
						if(rt)//equals to 1 / true
							rt=rt+2;//sum is 3
					}
					if( rt != 0 )//inventory slot is full
					{
						MSGBASE msg;
						msg.Category = MP_ITEM;
						msg.Protocol = MP_ITEM_FULLINVEN_ALERT;
						this->SendMsg( &msg, sizeof(msg) );

						//m_EventCountDownTimer=pEvent->dwTiming;//60*pEvent->dwTiming*1000;
						m_EventCountDownTimer=m_EventCountDownTimer;

						LOGINEVENTMGR->LoginEventSend(this,2,m_EventCountDownTimer,pEvent->dwItemIdx,pEvent->cTextSend,pEvent->dwQuantity,pEvent->dwTiming);

						//return;
					}
				}
				else
				{
					g_Console.LOG(4,"gg");
					m_EventIDFlag=pEvent->dwNextLine;
					m_EventCountDownTimer=pEvent->dwTiming;

					bFirstEvent=TRUE;
				}*/

				

				if( pEvent->dwNextLine!=0 )
				{
					m_EventIDFlag=pEvent->dwNextLine;
					bFirstEvent=TRUE;
				}
				else
				{
					m_EventIDFlag=0;
					LOGINEVENTMGR->LoginEventSend(this,1,m_EventCountDownTimer,pEvent->dwItemIdx,pEvent->cTextSend,pEvent->dwQuantity,pEvent->dwTiming);
					m_EventCountDownTimer=0;//m_EventCountDownTimer;
				}

				m_EventCountDownTimer=pEvent->dwTiming;//60*pEvent->dwTiming*1000;
			}
		}
	}
}



void CPlayer::GetItemtotalInfo(ITEMBASE Inventory[])
{
	m_ItemContainer.GetSlot(eItemTable_Inventory)->GetItemInfoAll(Inventory);
}
void CPlayer::MoveInventPack(bool &bRet)
{
	m_ItemContainer.GetSlot(eItemTable_Inventory)->MovePack(GetID(), bRet);
}
void CPlayer::SetItemTotalInfo(ITEMBASE * pItemBaseAll)
{
	m_ItemContainer.GetSlot(eItemTable_Inventory)->SetItemInfoAll(pItemBaseAll);
}
DWORD CPlayer::IsCanMovePack()
{
	if (LOOTINGMGR->IsLootedPlayer(GetID()))
		return eEXITCODE_LOOTING;

	if (GetState() == eObjectState_Exchange)
		return eEXITCODE_NOT_ALLOW_STATE;

	if (GetState() == eObjectState_StreetStall_Owner ||
		GetState() == eObjectState_StreetStall_Guest)
		return eEXITCODE_NOT_ALLOW_STATE;
	if (gCurTime - m_MovePackTime < 5000)
		return 1;

	return 0;
}


BOOL CPlayer::IsForbidChat() const
{
	__time64_t time = 0;
	_time64( &time );

	if( time > ForbidChatTime || 0 == ForbidChatTime )
		return FALSE;

	return TRUE;
}








void CPlayer::AddKillTimes()
{
	m_HeroInfo.dwKillPlayerTimes += 1;
	m_HeroCharacterInfo.dwKillPlayerTimes += 1;

	MSG_KILLCOUNT msg;
	msg.Category = MP_CHAR;
	msg.Protocol = MP_CHAR_KILLCOUNT_NOTIFY;
	msg.KillCount = m_HeroInfo.dwKillPlayerTimes;
	msg.dwObjectID = this->GetID();
	PACKEDDATA_OBJ->QuickSend(this, &msg, sizeof(msg));

	//CharacterHeroInfoUpdate(this);
}
void CPlayer::ClearKillTimes()
{
	m_HeroInfo.dwKillPlayerTimes = 0;
	CharacterHeroInfoUpdate(this);
}


void CPlayer::SetRankState(INSDG_RANK_STATE* pRankState)
{
	m_RankState.dwRankTime = pRankState->dwRankTime;
	m_RankState.dwMonsterKillNum = pRankState->dwMonsterKillNum;
}

INSDG_RANK_STATE* CPlayer::GetRankState()
{
	return &m_RankState;
}

void CPlayer::RemoveInsDGRankInfoTable()
{
	INSDG_RANK_INFO* pRankInfo = NULL;

	m_InsDGRankInfoTable.SetPositionHead();
	while (pRankInfo = m_InsDGRankInfoTable.GetData())
	{
		SAFE_DELETE(pRankInfo);
	}
	m_InsDGRankInfoTable.RemoveAll();
}

void CPlayer::AddInsDGRankInfo(INSDG_RANK_INFO* pRankInfo)
{
	if (pRankInfo == NULL)
		return;

	INSDG_RANK_INFO* pInsDGRankInfo = new INSDG_RANK_INFO;
	memcpy(pInsDGRankInfo, pRankInfo, sizeof(*pInsDGRankInfo));
	m_InsDGRankInfoTable.Add(pInsDGRankInfo, pInsDGRankInfo->dwInsDGIndex);
}

INSDG_RANK_INFO* CPlayer::GetInsDGRankInfo(DWORD dwInsDGIndex)
{
	return m_InsDGRankInfoTable.GetData(dwInsDGIndex);
}