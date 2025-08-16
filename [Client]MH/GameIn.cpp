

// GameIn.cpp: implementation of the CGameIn class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "GameIn.h"
//#ifdef _CHEATENABLE_
#include "CheatMsgParser.h"
//#endif	//_CHEATENABLE_

#include "MainGame.h"
#include "WindowIdEnum.h"
#include "./Interface/cWindowManager.h"
#include "BattleSystem_client.h"

#include "SuryunDialog.h"
#include "MugongManager.h"
#include "ObjectManager.h"
#include "ItemManager.h"
#include "MoveManager.h"
#include "MHTimeManager.h"
#include "QuickManager.h"
#include "PeaceWarModeManager.h"
#include "PartyManager.h"
#include "UngijosikManager.h"
#include "ObjectActionManager.h"
#include "TacticManager.h"
#include "SkillManager_Client.h"
#include "GuildManager.h"
#include "KyungGongManager.h"
#include "StreetStallManager.h"
#include "PyogukManager.h"
#include "FriendManager.h"
#include "NoteManager.h"
#include "ChatManager.h"
#include "Hero.h"

//050630SW
#include "cChatTipManager.h"
//-
#include "ExchangeManager.h"
#include "NpcScriptManager.h"
//#include "HelpDicManager.h"
#include "HelperManager.h"
#include "./Audio/MHAudioManager.h"
#include "OptionManager.h"
#include "WantedManager.h"
#include "MHCamera.h"
#include "./Input/UserInput.h"
#include "MouseCursor.h"
#include "JournalManager.h"

#include "MainBarDialog.h"
#include "CharStateDialog.h"
#include "MiniMapDlg.h"
#include "PartyIconManager.h"
#include "ReviveDialog.h"
#include "MainBarDialog.h"
#include "WantNpcDialog.h"
#include "GuildTraineeDialog.h"

#include "mhMap.h"
#include "cMsgBox.h"

#include "MotionManager.h"

#include "MurimNet.h"
#include "CQuestManager.h"
#include "GameEventManager.h"
#include "objectstatemanager.h"
#include "PKManager.h"
#include "GMNotifyManager.h"
#include "UpgradeDlg.h"
#include "ReinforceDlg.h"
#include "DissloveDlg.h"
#include "NpcScriptDialog.h"
//#include "MixManager.h"
#include "StatusIconDlg.h"
#include "CharSelect.h"
#include "PetManager.h"
#include "PetStateDlg.h"
#include "PetStateMiniDlg.h"
#include "PetInventoryDlg.h"
#include "PetUpgradeDialog.h"
#include "PetRevivalDialog.h"

#include "Vimumanager.h"
#include "AbilityManager.h"
#include "AbilityUpdater_Interface.h"
#include "AbilityUpdater_ApplyData.h"

#include "UserInfoManager.h"
#include "ChatDialog.h"

//#include "cJackpotDialog.h"

#include "ExitManager.h"
#include "HackCheck.h"
#include "SocietyActManager.h"
#include "ShoutchatDialog.h"
#include "GTScoreInfoDialog.h"

#include "mmsystem.h"

#include "InstancesDungeon/InsDungeonManager.h"
#include "InstancesDungeon/InsDGEntranceInfoDialog.h"

#ifdef _GMTOOL_
#include "GMToolManager.h"
#endif

#include "Quest.h"
#include "QuestManager.h"

#include "../[CC]Suryun/SuryunManager_client.h"
#include "MapChange.h"
#include "EventMapInfo.h"
#include "GuildFieldWar.h"
#include "PartyWar.h"
#include "PartyWarDialog.h"

#include "ItemShopDialog.h"
#include "MoveDialog.h"
#include "EventNotifyDialog.h"

#include "../VSyncDialog.h"

#include "PyogukDialog.h"
#include "GuildTournamentMgr.h"
//SW050715
#include "cJackpotManager.h"

#include "SkillDelayManager.h"
#include "GuildUnion.h"
#include "SeigeWarDialog.h"
#include "SiegeWarMgr.h"

//
#include "CharChangeDlg.h"
#include "GuageDialog.h"

#include "MussangManager.h"
#include "WeatherManager.h"
#include "PetManager.h"
#include "TitanManager.h"
#include "TitanRecallDlg.h"
#include "BobusangManager.h"

#include "ProgressBarDlg.h"

//胶农赴鸡
#include "ScreenShotDlg.h"
#include "SurvivalModeManager.h"

#include "Reconnect.h"
#include "GuildWarDialog.h"
#include "GuildWarManager.h"

#include "DungeonMgr.h"
#include "ReconnectDialog.h"
#include "OptionDialog.h"
#ifdef TAIWAN_LOCAL
#include "BillingManager.h"
#endif

#ifdef _JAPAN_LOCAL_
#include "InventoryExDialog.h"
#include "MugongDialog.h"
#endif
#ifdef _HK_LOCAL_
#include "InventoryExDialog.h"
#include "MugongDialog.h"
#endif
#ifdef _TL_LOCAL_
#include "InventoryExDialog.h"
#include "MugongDialog.h"
#endif

#include "AutoNoteManager.h"
#include "FortWarManager.h"
#include "TopDialog.h"          //weiye 排行头文件 2018-01-10
#include "TopMunpDialog.h"      //weiye 帮会排行头文件 2018-01-10
#include "LuckerDialog.h"      // the LuckerDialog include 2014-06-02
#include "otherCharacterinfo.h" // the character info include 2014-08-11
#include "otherCharacterEquip.h" // the character equip include 2014-08-11
#include "ItemGambleManager.h"   // 赌博系统头引用 2015-01-26
//#include "GameAIManager.h"        //weiye 自动复活头文件 2017-12-06
//#include "ReDisConnManager.h"    //weiye 断线重连头文件 2017-12-07
//#include "SafeNumberDialog.h"    //weiye 安全锁头文件定义       2017-12-07
#include "SafeNumberIcon.h"      //weiye 安全锁图标接口　　　 2017-12-07
#include "FBTimeDialog.h"        //weiye 副本时间引用         2018-02-06
//#include "AutoFindPathManager.h" //weiye 自动寻路系统头文件引用 2018-03-13
#include "VideoCaptureIcon.h"        //weiye 屏幕录像图标头文件  2018-05-14
#include "BillBoardManager.h"

#include "AiManager.h"
#include "EventPlayTimeDialog.h"

#include "InsFUBENMissionString.h"
#include "InsDGEntranceInfoDlg.h"

#include "JackFlashRoar.h"
#include "HideBuff.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

#ifndef _RESOURCE_WORK_NOT_APPLY_
#include "AppearanceManager.h"
#endif
#include "RankButtonDialog.h"

#include "LuckyDrawIcon.h"

#include "BloodStormIcon.h"

#include "GameCheckManager.h"

#include "GoldShopDialog.h"

#include "QuickDialog.h"

#include "AdPointDlg.h"

#include "ChangeLookDialog.h"

#include "ItemDropInfoDialog.h"

#include "RegistryManager.h"

#include "FPSDialog.h"
#include "GradeChangeDlg.h"		//武器升阶值转移卷
#include "TopDungeon.h"
#include "CustomizingNameDlg.h"
#ifdef _TESTCLIENT_
#include "AppearanceManager.h"

#include "PathManager.h"

void InitTestCharacters(void);
void InitTestHero();
void InitTestMonster();
CMonster * g_pMonster = NULL;
CBossMonster * g_pBossMonster = NULL;


#endif

DWORD g_TempHeroID;	//荐访厘俊辑 靖
extern HWND _g_hWnd;
extern int bTempBoxT;

extern float ChannelExp[];
extern float ChannelAbility[];
extern float ChannelGoldRate[];

extern BOOL jTweakToggle;
extern bool g_ThisIsFirstLogin;
extern int m_CameraPos;
extern bool b_WaitFirstEvent;
GLOBALTON(CGameIn)


CGameIn::CGameIn()
{
	m_UpGoldMoneyTime = 0;
	m_bGameInAcked	= FALSE;
	m_bInitForGame	= FALSE;
	m_GameInInitKind = eGameInInitKind_Login;

	m_pMugongSuryunDlg	= NULL;
	m_pInventoryDlg		= NULL;
	m_pExchangeDlg		= NULL;
	m_pMixDlg			= NULL;
	m_pTitanPartsMakeDlg = NULL;	// magi82 - Titan(070112)
	// magi82 - Titan(070119) ////////////////////////////////////////////////
	m_pTitanMixDlg			= NULL;	
	m_pTitanRepairDlg		= NULL;	//2007. 9. 11. CBH - 鸥捞藕 荐府芒
	m_pTitanRecallDlg		= NULL;	//2007. 9. 13. CBH - 鸥捞藕 家券 霸捞瘤
	m_pTitanUpgradeDlg		= NULL;

	m_pTitanBreakDlg		= NULL;
	//////////////////////////////////////////////////////////////////////////
	m_pTitanChangePreViewDlg = NULL;	// magi82 - Titan(070212)
	m_pTitanInventoryDlg = NULL;		// magi82 - Titan(070222)
	m_pTitanGuageDlg	= NULL;			// magi82 - Titan(070305)
	//m_pTitanMugongMixDlg	= NULL;		// magi82 - Titan(070611) 鸥捞藕 公傍函券 林籍贸府
	m_pTitanRegisterDlg		= NULL;		// magi82 - Titan(070320)
	m_pTitanDissolutionDlg	= NULL;		// magi82 - Titan(070321)
	m_pDealDlg			= NULL;
	m_pCharDlg			= NULL;
	m_pChatDlg			= NULL;
//	m_pJackpotDlg		= NULL;
	m_pMainDlg			= NULL;
	m_pDebugDlg			= NULL;
// LYJ 051017 备涝畴痢惑 眠啊
	m_pStallKindSelectDlg	= NULL;
	m_pStreetStallDlg	= NULL;
	m_pNpcScriptDlg		= NULL;
	m_pHelpDlg			= NULL;
	m_pMacroDlg			= NULL;	
	m_pChatOptionDlg	= NULL;
	m_pOptionDlg		= NULL;
	m_pExitDlg			= NULL;
	m_pCharStateDlg		= NULL;

	m_pAuctionBoard		= NULL;

	m_MoneyDlg			= NULL;
	m_pQuickDlg			= NULL;
	m_pGuageDlg			= NULL;

	m_pMiniMapDlg		= NULL;
	m_pBigMapDlg		= NULL;

	m_pPyogukDialog		= NULL;
	m_pNoteDialog		= NULL;
	m_pFriendDialog		= NULL;
	m_pMiniNoteDialog	= NULL;
	m_pMiniFriendDialog	= NULL;
	m_pReviveDialog		= NULL;
	m_pWantNpcDialog	= NULL;

	m_pMonsterGuageDlg	= NULL;
	
	m_pBailDlg			= NULL;
	m_pDissolutionDlg	= NULL;
	m_pEventNotifyDlg	= NULL;

	m_pVsyncDlg = NULL;
	
	m_pMPRegistDialog	= NULL;
	m_pMPMissionDialog	= NULL;
	m_pMPNoticeDialog	= NULL;
	m_pMPGuageDialog	= NULL;
	
	m_pItemShopDialog	= NULL;
	m_pMoveDialog		= NULL;
	m_pPointSaveDialog	= NULL;	

	m_pSkPointDlg		= NULL;
	m_pSkPointNotifyDlg = NULL;

	m_pPetStateDlg		= NULL;
	m_pPetStateMiniDlg	= NULL;
	m_pPetInventoryDlg	= NULL;
	m_pPetUpgradeDlg	= NULL;
	m_pPetRevivalDlg	= NULL;

	m_pPetMultiDlg		= NULL;

	m_MovePoint = 0;
	m_MoveMap	= 0;
	m_GuildIdx	= 0;
	m_SiegeWarMapNum	= 0;

	m_pShoutDlg			= NULL;
	m_pShoutchatDlg		= NULL;
	m_pChaseDlg			= NULL;
	m_pChaseinputDlg	= NULL;
	m_pNameChangeDlg	= NULL;
	m_pNameChangeNotifyDlg	= NULL;

	m_pCharChangeDlg	= NULL;

	m_pSkillPointResetDlg	= NULL;	// magi82(42) - 讥酒捞袍 眠啊(荐访摹 檬扁拳)	

	m_pNumberPadDlg = NULL;

	// magi82(47)
	m_pStreetStallItemViewDlg = NULL;
	///////////////////////////////////
	m_QualityDlg = NULL;//觉醒
	m_QualityChangeDlg = NULL;
	m_GGDDlg = NULL;				//武器升阶值转移卷
	/////////////////////////////////////////////////////////////
	// 06. 06. 2瞒 傈流 - 捞康霖
	// 公傍 函券
	m_pSkillOptionChangeDlg = NULL;
	m_pSkillOptionClearDlg = NULL;
	/////////////////////////////////////////////////////////////

	m_bDieInBossMap = FALSE;
	m_nCountInBossMap = 0;
	m_dwTimeInBossMap = 0;
	
	//某腐磐 肺弊牢 矫埃函荐(辑滚,努扼捞攫飘) 檬扁拳
	memset(&GameinServerTime, 0, sizeof(GameinServerTime));
	m_Clientdate = 0;
	m_Clienttime = 0;

	m_pUniqueItemCurseCancellationDlg = NULL;	// magi82 - UniqueItem(070703)
	m_pUniqueItemMixDlg				  = NULL;	// magi82 - UniqueItem(070709)

	m_pSkinSelectDlg = NULL;
	m_pCostumeSkinSelectDlg = NULL;


	m_pStallFindDlg = NULL; // 畴痢惑 八祸 眠啊 by Stiner(8)

	//m_MallShopDialog=NULL;  // 2014-05-01 Create MallItem Window

	m_GoldShopDialog=NULL;
	m_GoldShopBuyDialog=NULL;

	m_pMiniMaryDialog=NULL; // 2014-05-08 Create marry window

	m_pMiniBaishiDialog=NULL; // 2014-05-10 Create Baishi window

	m_pControlDialog=NULL;    // 2014-05-23 Create Control window 

	m_pRankButtonDialog=NULL;

	m_pTopDialog = NULL;  // 2014-05-25 create billboard window
	m_pTopDungeon = NULL;
	m_pTopMunpDialog = NULL;  // 2018-01-12  帮会排行头文件定义 

	m_pLuckerDialog=NULL;     // 2014-06-02 Create LuckerDialog window

	m_pItemDropInfoDialog = NULL;

	m_pMoneyPacketDialog=NULL; // 2014-11-09 Create MoneyPacketDialog window 

	m_pOtherCharacterMoneyDlg=NULL; // 2014-11-10  Create other character money window 

	m_pOtherCharacterAmuletDlg=NULL; // 2014-11-10  Create other character Amulet window 

	m_pCharactorInfo=NULL;    // 2014-08-11 Create Character Other window

	m_pCharacterEquipInfo=NULL; // 2014-08-11 Create Character Equip Window!

	m_pItemStoneDkDlg =NULL;   // 2014-11-18 Create StoneDk Window!

	m_pItemStoneXqDlg=NULL ;   // 2014-11-18 Create StoneXq Window!

	m_pItemStepReinforceDlg = NULL;


	m_pItemLockDialog=NULL;     // 2014-12-07  ItemLock window clear;

	m_pItemUnLockDialog=NULL;     // 2014-12-07  UnItemLock window clear;

	//m_SafeNumberDialog=NULL;     //weiye 安全码窗口　      2017-12-08

   	m_SafeNumberIcon=NULL;        //weiye 安全码窗口图标　2017-12-08

	m_SafeNumberImg=NULL;         //weiye 安全码素材窗口  2018-04-08

	//m_SafeNumberChangeDialog=NULL;  //weiye  安全码修改窗口初始化 2018-04-08

	//m_SafeNumberLockDialog=NULL;     //weiye    安全码锁定窗口初始化 2018-04-08

	m_TopIcon=NULL;               //weiye 小地图下方窗口  2018-01-15

	m_FBTimeDialog=NULL;          //weiye 副本时间记时窗口定义 2018-02-06

	m_FameGoldDialog=NULL;        //weiye 元宝洗E窗口类初始化 2018-02-10

	//m_CharacterControlDialog=NULL;  //weiye 角色右键功能 2018-02-12

	m_SendEquipDialog=NULL;           //weiye 创建发送装备窗口 2018-03-12

	m_SendEquipSelectDialog=NULL;   //weiye 创建发送装备显示窗口 2018-03-14

	m_VideoCaptureDialog=NULL;       //weiye 屏幕录像窗口变量 2018-05-12

	m_VideoCaptureIcon= NULL;         //weiye 屏幕录像窗口图标 2018-05-14

	m_CodeDialog=NULL;

	m_bHideBuffDlg=NULL;

	m_ReconnectDlgDLG=NULL;

	m_InsDGEntranceInfoDlg=NULL;

	m_LuckyDrawIcon=NULL;

	m_BloodStormIcon=NULL;

	m_ChangeLookDialog = NULL;

	m_FPSDialog = NULL;

	m_JackRebornInfo = NULL;

	m_wInsDungeonMapNum = 0;

	m_pFadeDlg = NULL;
	m_VipDialog = NULL;               //创建VIP窗口
	m_pCustomizingDlg = NULL;
}

CGameIn::~CGameIn()
{

}

#include "AISetManager.h"
BOOL CGameIn::InitForGame()
{
	m_bInitForGame = TRUE;

	WINDOWMGR->CreateGameIn();

	//HELPERMGR->Init();//kiv by jack 03032022

	WINDOWMGR->AfterInit();
	
	if( FALSE == MAP->IsMapKind(eEventMap) )
		USERINFOMGR->LoadUserInfo(eUIK_INTERFACE /*| eUIK_RECONNECT*/);


	if( GetMainInterfaceDialog() )
		GetMainInterfaceDialog()->Refresh();
	if( GetCharStateDialog() )
		GetCharStateDialog()->Refresh();

	CHATMGR->SetChatDialog( (CChatDialog*)WINDOWMGR->GetWindowForID( CTI_DLG ) );
	CHATMGR->RestoreChatList();	

	QUESTMGR->InitializeQuest();
	PKMGR->Init();
	PETMGR->LoadPetImage();

	m_bDieInBossMap = FALSE;
	m_nCountInBossMap = 0;
	m_dwTimeInBossMap = 0;
	
	GUILDUNION->Clear();
	OPTIONMGR->InitForGameIn();

	AISETMGR->InitForGameIn();



	return TRUE;
}

BOOL CGameIn::Init(void* pInitParam)
{
	int GameInInitKind = *(int*)pInitParam;
	m_GameInInitKind = GameInInitKind;
	m_bGameInAcked = FALSE;

	InitForGame();


#ifndef _RESOURCE_WORK_NOT_APPLY_
	APPEARANCEMGR->EnbaleReservation();
#endif


//	QUESTMGR->DeleteMapNpcList();
	if(MAP->GetMapNum() != 0)
		MAP->LoadStaticNpc(MAP->GetMapNum());


	VIMUMGR->Init();
	EXCHANGEMGR->Init();
	STREETSTALLMGR->Init();
	FRIENDMGR->Init();
	WANTEDMGR->Init();
	PARTYMGR->Init();
	JOURNALMGR->Init();
	GUILDMGR->InitGameIn();
	SURYUNMGR;
	EVENTMAP->Init();
	GUILDFIELDWAR->Init();
	PARTYWAR->Init();
	SIEGEMGR->Init();
	SWPROFIT->Init();
	MUSSANGMGR->Init();
	WEATHERMGR->Init();
	SVVMODEMGR->Init();
	FORTWARMGR->Init();
	
	INSDUNGEONMGR->LoadInsDungeonMissionInfo();
#ifdef _GMTOOL_
	if( GMTOOLMGR->IsShowing() )
		GMTOOLMGR->ShowGMDialog( TRUE );
#endif
	
	NETWORK->SetCurState(this);

	switch(GameInInitKind)
	{
	case eGameInInitKind_Login:
	case eGameInInitKind_MapChange:
		{
			/*MSG_DWORD2 msg;
			msg.Category = MP_USERCONN;
			msg.Protocol = MP_USERCONN_GAMEIN_SYN;
			if (!REDISCONN->GetStart())
			{
				msg.dwData1 = gChannelNum;
				REDISCONN->SetChannelIdx(gChannelNum);
			}
			else
			{
				msg.dwData1 = REDISCONN->GetChannelIdx();
			}

			msg.dwData2 = m_MoveMap;
			NETWORK->Send(&msg, sizeof(msg));*/
			MSG_DWORD3 msg;
			msg.Category = MP_USERCONN;
			msg.Protocol = MP_USERCONN_GAMEIN_SYN;
			msg.dwData1	 = gChannelNum;
			msg.dwData2  = m_MoveMap;
			if(REDISCONN->GetStart())
				msg.dwData3 = REDISCONN->GetDWORDSavedHeroPos();
			else
				msg.dwData3 = 0;
			NETWORK->Send( &msg, sizeof(msg) );
			
		}
		break;
	case eGameInInitKind_SuryunEnter:
		{
			MSGBASE msg;
			msg.Category = MP_SURYUN;
			msg.Protocol = MP_SURYUN_ENTER_SYN;
			msg.dwObjectID = g_TempHeroID;

			NETWORK->Send( &msg, sizeof(msg) );
		}
		break;
	case eGameInInitKind_SuryunLeave:
		{
			MSGBASE msg;
			msg.Category = MP_SURYUN;
			msg.Protocol = MP_SURYUN_LEAVE_SYN;
			msg.dwObjectID = g_TempHeroID;
			NETWORK->Send( &msg, sizeof(msg) );
		}
		break;
		
	case eGameInInitKind_MovePos:
		{
 			MSG_DWORD2 msg;
			msg.Category = MP_USERCONN;
			msg.Protocol = MP_USERCONN_GAMEINPOS_SYN;
			msg.dwObjectID = g_TempHeroID;
			msg.dwData1 = gChannelNum;
			if( GUILDMGR->GetSOSChannelIdx() >= 0 )
			{
				msg.dwData1 = GUILDMGR->GetSOSChannelIdx();
				gChannelNum = GUILDMGR->GetSOSChannelIdx();
				GUILDMGR->SetSOSChannelIdx(-1);

			}

			msg.dwData2 = m_MovePoint;

			NETWORK->Send( &msg, sizeof(msg) );			
		}
		break;

	case eGameInInitKind_EventMapEnter:
		{
			MSG_DWORD3 msg;
			msg.Category = MP_USERCONN;
			msg.Protocol = MP_USERCONN_ENTER_EVENTMAP_SYN;
			msg.dwObjectID = g_TempHeroID;
			msg.dwData1 = EVENTMAP->GetEventMapNum();
			msg.dwData2 = EVENTMAP->GetChannelNum();
			msg.dwData3 = EVENTMAP->GetTeamNum();

			NETWORK->Send( &msg, sizeof(msg) );
		}
		break;
	case eGameInInitKind_GTEnter:

		{
			MSG_DWORD3 msg;
			msg.Category = MP_GTOURNAMENT;
			msg.Protocol = MP_GTOURNAMENT_BATTLEJOIN_SYN;
			msg.dwObjectID = g_TempHeroID;
			msg.dwData1 = m_GuildIdx;
			msg.dwData2 = 0;
			msg.dwData3 = m_MoveMap;
			NETWORK->Send( &msg, sizeof(msg) );
			
			m_MoveMap = 0;
		}
		break;
	case eGameInInitKind_GTLeave:
		{
			MSGBASE msg;
			msg.Category = MP_GTOURNAMENT;
			msg.Protocol = MP_GTOURNAMENT_LEAVE_SYN;
			msg.dwObjectID = g_TempHeroID;
			NETWORK->Send( &msg, sizeof(msg) );
		}
		break;
	case eGameInInitKind_GTObserverEnter:
		{
			MSG_DWORD3 msg;
			msg.Category = MP_GTOURNAMENT;
			msg.Protocol = MP_GTOURNAMENT_OBSERVERJOIN_SYN;
			msg.dwObjectID = g_TempHeroID;
			msg.dwData1 = m_GuildIdx;
			msg.dwData2 = m_BattleIdx;
			msg.dwData3 = m_MoveMap;
			NETWORK->Send( &msg, sizeof(msg) );
		}
		break;
	case eGameInInitKind_SWEnter:
		{
			MSG_DWORD4 msg;
			msg.Category = MP_SIEGEWAR;
			msg.Protocol = MP_SIEGEWAR_BATTLEJOIN_SYN;
			msg.dwObjectID = g_TempHeroID;
			msg.dwData1 = m_GuildIdx;			// Guild Idx
			msg.dwData2 = m_MoveMap;			// ReturnMapNum
			msg.dwData3 = 0;					// bObserver
			msg.dwData4 = m_SiegeWarMapNum;		// SiegeWarMapNum
			NETWORK->Send( &msg, sizeof(msg) );
			
			m_MoveMap = 0;
			m_SiegeWarMapNum = 0;
		}
		break;
	case eGameInInitKind_SWLeave:
		{
			MSGBASE msg;
			msg.Category = MP_SIEGEWAR;
			msg.Protocol = MP_SIEGEWAR_LEAVE_SYN;
			msg.dwObjectID = g_TempHeroID;
			NETWORK->Send( &msg, sizeof(msg) );
		}
		break;
	case eGameInInitKind_SWObserverEnter:
		{
			MSG_DWORD4 msg;
			msg.Category = MP_SIEGEWAR;
			msg.Protocol = MP_SIEGEWAR_OBSERVERJOIN_SYN;
			msg.dwObjectID = g_TempHeroID;
			msg.dwData1 = m_GuildIdx;			// Guild Idx
			msg.dwData2 = m_MoveMap;			// ReturnMapNum
			msg.dwData3 = 1;					// bObserver
			msg.dwData4 = m_SiegeWarMapNum;		// SiegeWarMapNum
			NETWORK->Send( &msg, sizeof(msg) );
			
			m_MoveMap = 0;
			m_SiegeWarMapNum = 0;
		}
		break;
	case eGameInInitKind_SVMapLeave:
		{
			MSGBASE msg;
			msg.Category = MP_SURVIVAL;
			msg.Protocol = MP_SURVIVAL_LEAVE_SYN;
			msg.dwObjectID = g_TempHeroID;
			NETWORK->Send( &msg, sizeof(msg) );
		}
		break;
	case eGameInInitKind_InsDungeonMapEnter:	//2008. 7. 4. CBH - 牢带 呕免 包访 惑怕 眠啊
		{
													//CHATMGR->AddMsg(CTC_SYSMSG, "this dungeon");
			//OPTIONMGR->ApplySettings();
			//OPTIONMGR->SendOptionMsg();
			INSDG_RANK_STATE* pRankState = INSDUNGEONMGR->GetRankState();

			MAPCHANGE_INFO* pMapchangeInfo = GAMERESRCMNGR->GetMapChangeInfoFromToMapNum(m_MoveMap, m_wInsDungeonMapNum);
			if (pMapchangeInfo == NULL)
				return FALSE;

			stMOVEPOINT movePt;
			movePt.SetMovePoint(pMapchangeInfo->MovePoint.x, pMapchangeInfo->MovePoint.z);
			DWORD dwMovePoint = movePt.value;

			MSG_INSDG_ENTER msg;
			msg.Category = MP_INSDUNGEON;
			msg.Protocol = MP_INSDUNGEON_ENTER_SYN;
			msg.dwObjectID = g_TempHeroID;
			msg.RankState.dwRankTime = pRankState->dwRankTime;
			msg.RankState.dwMonsterKillNum = pRankState->dwMonsterKillNum;
			msg.dwData1 = m_MoveMap;
			msg.dwData2 = m_wInsDungeonMapNum;
			msg.dwData3 = 0;
			msg.dwData4 = 0;
			msg.dwMapMovePoint = dwMovePoint;
			NETWORK->Send(&msg, sizeof(msg));

			m_MoveMap = 0;
			m_wInsDungeonMapNum = 0;
			INSDUNGEONMGR->InitRankState();	//咯扁辑 静绊 檬扁拳茄促.
		}
	break;
	case eGameInInitKind_InsDungeonMapLeave:	//2008. 7. 4. CBH - 牢带 呕免 包访 惑怕 眠啊
		{
			MSG_DWORD msg;
			msg.Category = MP_INSDUNGEON;
			msg.Protocol = MP_INSDUNGEON_LEAVE_SYN;
			msg.dwObjectID = g_TempHeroID;
			msg.dwData = m_MovePoint;
			NETWORK->Send(&msg, sizeof(msg));
		}
	break;
	default:
		//ASSERT(0);
		break;
	}

	// debug侩
	ITEMMGR->m_nItemUseCount = 0;
//	SKILLMGR->m_nSkillUseCount = 0;//kiv

	return TRUE;
}

void CGameIn::ReleaseForGame()
{
	if( !m_bInitForGame ) return;
	WINDOWMGR->SetDragEnd();
	WINDOWMGR->DestroyWindowAll();

	//HELPERMGR->Release();


	//TESTQUESTMNG->Release();
	m_bInitForGame = FALSE;

	m_pMugongDlg			= NULL;
	m_pInventoryDlg			= NULL;
	m_pExchangeDlg			= NULL;
	m_pMixDlg				= NULL;
	m_pTitanPartsMakeDlg	= NULL;	// magi82 - Titan(070112)
	// magi82 - Titan(070119) ////////////////////////////////////////////////
	m_pTitanMixDlg			= NULL;	
	m_pTitanRepairDlg		= NULL;	//2007. 9. 11. CBH - 鸥捞藕 荐府芒
	m_pTitanRecallDlg		= NULL;	//2007. 9. 13. CBH - 鸥捞藕 家券 霸捞瘤
	m_pTitanUpgradeDlg		= NULL;
	m_pTitanBreakDlg		= NULL;
	//////////////////////////////////////////////////////////////////////////
	m_pTitanChangePreViewDlg = NULL;	// magi82 - Titan(070212)
	m_pTitanInventoryDlg	= NULL;		// magi82 - Titan(070222)
	m_pTitanGuageDlg		= NULL;		// magi82 - Titan(070305)
	//m_pTitanMugongMixDlg	= NULL;		// magi82 - Titan(070611) 鸥捞藕 公傍函券 林籍贸府
	m_pTitanRegisterDlg		= NULL;		// magi82 - Titan(070320)
	m_pTitanDissolutionDlg	= NULL;		// magi82 - Titan(070321)
	m_pDealDlg				= NULL;
	m_pCharDlg				= NULL;
	m_pChatDlg				= NULL;
//	m_pJackpotDlg			= NULL;
	m_pMainDlg				= NULL;
	m_pDebugDlg				= NULL;
// LYJ 051017 备涝畴痢惑 眠啊
	m_pStallKindSelectDlg	= NULL;
	m_pStreetStallDlg		= NULL;
	m_pNpcScriptDlg			= NULL;
	m_pHelpDlg				= NULL;
	m_pMacroDlg				= NULL;	
	m_pChatOptionDlg		= NULL;
	m_pOptionDlg			= NULL;
	m_pExitDlg				= NULL;
	m_pCharStateDlg			= NULL;

	m_pAuctionBoard			= NULL;

	m_MoneyDlg				= NULL;
	m_pQuickDlg				= NULL;
	m_pGuageDlg				= NULL;

	m_pMiniMapDlg			= NULL;
	m_pBigMapDlg			= NULL;

	m_pPyogukDialog			= NULL;
	m_pNoteDialog			= NULL;

	m_pFriendDialog			= NULL;
	m_pMiniNoteDialog		= NULL;
	m_pMiniFriendDialog		= NULL;
	m_pReviveDialog			= NULL;
	m_pWantNpcDialog		= NULL;

	m_pMonsterGuageDlg		= NULL;

	m_pUpgradeDlg			= NULL;
	m_pReinforceDlg			= NULL;

	m_pBailDlg				= NULL;
	m_pDissolutionDlg		= NULL;
	m_pEventNotifyDlg		= NULL;

	m_pVsyncDlg = NULL;
	
	m_pShoutDlg				= NULL;
	m_pShoutchatDlg			= NULL;

	m_pChaseDlg				= NULL;
	m_pChaseinputDlg		= NULL;
	
	m_pNameChangeDlg		= NULL;
	m_pNameChangeNotifyDlg	= NULL;	

	m_pCharChangeDlg		= NULL;

	m_pUniqueItemCurseCancellationDlg = NULL;	// magi82 - UniqueItem(070703)
	m_pUniqueItemMixDlg = NULL;	// magi82 - UniqueItem(070709)

	m_pUniqueItemMixProgressBarDlg = NULL;		//2007. 10. 22. CBH - 蜡聪农 酒捞牌 炼钦 贸府 官 霸捞瘤
	m_pTitanMixProgressBarDlg = NULL;		//2007. 10. 22. CBH - 鸥捞藕 炼钦 贸府 官 霸捞瘤
	m_pTitanPartsProgressBarDlg = NULL;		//2007. 10. 23. CBH - 鸥捞藕 颇明 炼钦 贸府 官 霸捞瘤

	m_pSkillPointResetDlg	= NULL;	// magi82(42) - 讥酒捞袍 眠啊(荐访摹 檬扁拳)

	// magi82(47)
	m_pStreetStallItemViewDlg = NULL;
	m_pStallFindDlg = NULL; // 畴痢惑 八祸 眠啊 by Stiner(8)

	//m_MallShopDialog=NULL;  // 2014-05-01 Create MallItem Window

	m_GoldShopDialog=NULL;
	m_GoldShopBuyDialog=NULL;

	m_pMiniMaryDialog=NULL; // 2014-05-08 Create marry window

	m_pMiniBaishiDialog=NULL; // 2014-05-10 Create Baishi window

	m_pControlDialog=NULL;    // 2014-05-23 Create Control window 

	m_pRankButtonDialog=NULL;

	m_pTopDialog=NULL;  // 2014-05-25 create billboard window
	m_pTopDungeon = NULL;
	m_pTopMunpDialog = NULL;  // 2018-01-12  帮会排行头文件定义 

	m_pLuckerDialog=NULL;     // 2014-06-02 Create LuckerDialog window

	m_pItemDropInfoDialog = NULL;

	m_pMoneyPacketDialog=NULL; // 2014-11-09 Create MoneyPacketDialog window 

	m_pOtherCharacterMoneyDlg=NULL; // 2014-11-10  Create other character money window 

	m_pOtherCharacterAmuletDlg=NULL; // 2014-11-10  Create other character Amulet window 

	m_pCharactorInfo=NULL;    // 2014-08-11 Create Character Other window

	m_pCharacterEquipInfo=NULL; // 2014-08-11 Create Character Equip Window!

	m_pItemStoneDkDlg =NULL;   // 2014-11-18 Create StoneDk Window!

	m_pItemStoneXqDlg=NULL ;   // 2014-11-18 Create StoneXq Window!

	m_pItemStepReinforceDlg = NULL;

	//m_SafeNumberDialog=NULL;     //weiye 安全码窗口　      2017-12-08

   	m_SafeNumberIcon=NULL;        //weiye 安全码窗口图标　2017-12-08

	m_SafeNumberImg=NULL;         //weiye 安全码素材窗口  2018-04-08

	//m_SafeNumberChangeDialog=NULL;  //weiye  安全码修改窗口初始化 2018-04-08

	//m_SafeNumberLockDialog=NULL;     //weiye    安全码锁定窗口初始化 2018-04-08

	m_TopIcon=NULL;              //weiye 小地图下方窗口  2018-01-15

	m_FBTimeDialog=NULL;          //weiye 副本时间记时窗口定义 2018-02-06

	m_FameGoldDialog=NULL;        //weiye 元宝洗E窗口类初始化 2018-02-10

	//m_CharacterControlDialog=NULL; //weiye 角色右键功能 2018-02-12

	m_SendEquipDialog=NULL;           //weiye 创建发送装备窗口 2018-03-12

	m_SendEquipSelectDialog=NULL;  //weiye 创建发送装备显示窗口 2018-03-14

	m_VideoCaptureDialog=NULL;       //weiye 屏幕录像窗口变量 2018-05-12

	m_VideoCaptureIcon= NULL;         //weiye 屏幕录像窗口图标 2018-05-14

	m_CodeDialog=NULL;

	m_bHideBuffDlg=NULL;

	m_ReconnectDlgDLG=NULL;

	m_InsDGEntranceInfoDlg=NULL;

	m_LuckyDrawIcon=NULL;

	m_BloodStormIcon=NULL;

	m_ChangeLookDialog = NULL;

	m_FPSDialog = NULL;

	m_JackRebornInfo = NULL;

	m_QualityDlg = NULL;
	m_QualityChangeDlg = NULL;//觉醒
	m_GGDDlg = NULL;				//武器升阶值转移卷

	m_VipDialog = NULL;               //创建VIP窗口
	m_pCustomizingDlg = NULL;
}

void CGameIn::Release(CGameState* pNextGameState)
{
	WINDOWMGR->SetDragEnd();
#ifndef _RESOURCE_WORK_NOT_APPLY_

	APPEARANCEMGR->DisableReservation();

#endif

	WEATHERMGR->Release();

	BATTLESYSTEM->ReleaseBattle();

	STATUSICONDLG->Release();
	
	CURSOR->SetActive( FALSE );
	CHATMGR->SetChatDialog( NULL );

	AUDIOMGR->StopAll();
	QUICKMGR->Release();
	ITEMMGR->Release();
	MUGONGMGR->Release();
	QUESTMGR1->Release();
	PKMGR->Release();
	INSDUNGEONMGR->Release();
	if(m_GameInInitKind != eGameInInitKind_SuryunEnter && m_GameInInitKind != eGameInInitKind_EventMapEnter)
		USERINFOMGR->SaveUserInfo(eUIK_USERSTATE | eUIK_INTERFACE/* | eUIK_RECONNECT*/);

	EFFECTMGR->RemoveAllEffect();
	OBJECTMGR->RemoveAllObject();
	
	MAP->Release();

	PICONMGR->DeleteAllPartyIcon();

	ReleaseForGame();
	
	PETMGR->PetMgrRelease();

	TITANMGR->TitanMgrRelease();
	
	EVENTMAP->Release();

	HACKCHECK->Release();



#ifdef _GMTOOL_
//set to close
	GMTOOLMGR->ShowGMDialog( FALSE );
#endif
	
	
	g_pExecutive->UnloadAllPreLoadedGXObject(0);

	g_TempHeroID = HEROID;
	HEROID = 0;
	gUserID = 0;
}
extern VECTOR3 gHeroPos;
extern double CurFPS;
#include "ItemStepReinforceDialog.h"
void CGameIn::Process()
{
///	MHTIMEMGR->Process();

	if(!m_bGameInAcked)
		return;
	GAMEEVENTMGR->Process();
	CHATTIPMGR->Process();
	GetMiniMapDialog()->Process();
	GetItemStepReinforceDlg()->Process();

	/*if(OBJECTMGR->GetHero()&&!REDISCONN->GetStart())
	{
		if(!HERO->GetKyungGongMode())
		{
			sCPlayerAI.DufualtUsekyungGong();
		}
		GetMiniMapDialog()->Process();
	}*/
	if(!CHANNEL->IsMoveMap())
		CHANNEL->InitDmove();
	if(CHANNEL->IsMoveMap())
		CHANNEL->DmoveMap();


	//if (MOVEMGR->IsMoving(HERO) && !HERO->IsPKMode() && !PKMGR->IsPKLooted() && !HERO->IsDied())
	//{		
	//	WORD curMapNum = HERO->GetCharacterTotalInfo()->CurMapNum;
	//	WORD mapNum = GAMERESRCMNGR->CheckMapChange(&gHeroPos, curMapNum);
	//
	//	if (mapNum != 65535)
	//		g_UserInput.GetHeroMove()->MapChange(mapNum);		
	//}

	char Temp[MAX_PATH];
	sprintf(Temp, "%1.0f",CurFPS);
	GAMEIN->GetFPSDialog()->SetFPS(Temp);

	//if (strcmp(code, "JPMONEY") == 0)
	{
		//DWORD money;
		//sscanf(cheat, "%s %d", code, &money);

		//if (!money || money > 999999999) return FALSE;

		//JACKPOTMGR->SetJPTotalMoneyTest(CurFPS);
	}


	EXITMGR->Process();
	HACKCHECK->Process();
	//DungeonMGR->Process();
	ITEMMGR->Process();
	GetShoutchatDlg()->Process();
	GetGTScoreInfoDlg()->Process();
	GetCharChangeDlg()->Process();
	GetSWEngraveDlg()->Process();
	GetSWTimeDlg()->Process();
	ProcessInBossMap();
	//OPTIONMGR->GraphicAutoCtrlProcess();

	sCPlayerAI.CheckKyunkong2();
	if(OBJECTMGR->GetHero()&&!REDISCONN->GetStart())
	{
		if(!HERO->GetKyungGongMode())
		{
			sCPlayerAI.DufualtUsekyungGong2();
		}		
	}

	GAMEIN->GetGuildWarDialog()->Process();

	GAMEIN->GetEventPlayTimeDlg()->ExRender();

	GAMEIN->GetQuickDialog()->MyView();

	//GAMEIN->GetGoldShopDialog()->Process();

	if(OPTIONDLG->IsActive())
	{
		AISETMGR->FindErrorPath();
		sCPlayerAI.CheckingSkill();
		//GAMEIN->GetOptionDialog()->GetPacketSelectNum();
	}
	

	if(jTweakToggle)
		GAMEIN->GetOptionDialog()->Test();

	INSDUNGEONMGR->Process();
	//ADDPOINTDLG->Process();
	//MAP->RenderTile2D();//2023 JACK
}

void CGameIn::BeforeRender()
{
	MAP->Process(gCurTime);
}

void CGameIn::AfterRender()
{
	if(m_bGameInAcked)
	{
		EFFECTMGR->Process(gCurTime);
		BATTLESYSTEM->Render();		
		WINDOWMGR->Render();
		NOTIFYMGR->Render();
		EVENTMAP->Render();
		PARTYWAR->Render();
		WEATHERMGR->Render();//

		INSDUNGEONMGR->Render();
		//MAP->RenderTileData_Debug();
	}	
}

void CGameIn::NetworkMsgParse(BYTE Category,BYTE Protocol,void* pMsg)
{
	/*if(NULL == HERO &&
		Category != MP_ITEM &&
//		Category != MP_TUTORIAL &&
		Protocol != MP_USERCONN_GAMEIN_ACK)
	{
	//	AddBackupMsg(pMsg, dwMsgSize);
		return;
	}*/
	switch(Category)
	{
	case MP_MOVE:				MOVEMGR->NetworkMsgParse(Protocol, pMsg);					break;
	case MP_SKILL:				SKILLMGR->NetworkMsgParse(Protocol, pMsg);					break;
	case MP_CHAR:				CHARCALCMGR->NetworkMsgParse(Protocol, pMsg);				break;
	case MP_SURYUN:				SURYUNMGR->NetworkMsgParse(Protocol,pMsg);					break;
	case MP_WEATHER:			WEATHERMGR->NetworkMsgParse(Protocol,pMsg);					break;
	case MP_GUILDWAR:			GUILDWARMGR->NetworkMsgParse(Protocol,pMsg);				break;
	case MP_USERCONN:			UserConn_NetworkMsgParse( Protocol, pMsg ) ;				break;		
    case MP_PARTY:				PARTYMGR->NetworkMsgParse(Protocol, pMsg);					break;
	case MP_QUICK:				QUICKMGR->NetworkMsgParse(Protocol, pMsg);					break;
	case MP_ITEM:				ITEMMGR->NetworkMsgParse(Protocol,pMsg);					break;
	case MP_ITEMEXT:			ITEMMGR->NetworkMsgParseExt(Protocol,pMsg);					break;
	case MP_MUGONG:				MUGONGMGR->NetworkMsgParse(Protocol, pMsg);					break;
	case MP_CHAT:				CHATMGR->NetworkMsgParse(Protocol, pMsg);					break;
	case MP_BATTLE:				BATTLESYSTEM->NetworkMsgParse(Protocol, pMsg);				break;
	case MP_PEACEWARMODE:		PEACEWARMGR->NetworkMsgParse(Protocol, pMsg);				break;
	case MP_UNGIJOSIK:			UNGIMGR->NetworkMsgParse(Protocol, pMsg);					break;
	case MP_TACTIC:				TACTICMGR->NetworkMsgParse(Protocol, pMsg);					break;
	case MP_KYUNGGONG:			KYUNGGONGMGR->NetworkMsgParse(Protocol, pMsg);				break;
	case MP_EXCHANGE:			EXCHANGEMGR->NetworkMsgParse(Protocol, pMsg);				break;
	case MP_STREETSTALL:		STREETSTALLMGR->NetworkMsgParse(Protocol, pMsg);			break;
	case MP_PYOGUK:				PYOGUKMGR->NetworkMsgParse(Protocol, pMsg);					break;
	case MP_FRIEND:				FRIENDMGR->NetworkMsgParse(Protocol, pMsg);					break;
	case MP_NOTE:				NOTEMGR->NetworkMsgParse(Protocol, pMsg);					break;
	case MP_BOSSMONSTER:		OBJECTMGR->NetworkMsgParse(Protocol, pMsg);					break;
	case MP_QUEST:				Quest_NetworkMsgParse(Protocol, pMsg);						break;
	case MP_PK:					PKMGR->NetworkMsgParse(Protocol, pMsg);						break;
	case MP_WANTED:				WANTEDMGR->NetworkMsgParse(Protocol, pMsg);					break;	
	case MP_CHEAT:				CHEATMGR->NetworkMsgParse(Category,Protocol,(MSGBASE*)pMsg);break;	
	case MP_JOURNAL:			JOURNALMGR->NetworkMsgParse(Protocol, pMsg);				break;	
	case MP_GUILD:				GUILDMGR->NetworkMsgParse(Protocol, pMsg);					break;
	case MP_GUILD_UNION:		GUILDUNION->NetworkMsgParse(Protocol, pMsg);				break;
	case MP_NPC:				NPC_NetworkMsgParse( Protocol, pMsg ) ;						break;		
	case MP_SIGNAL:				SIGNAL_NetworkMsgParse( Protocol, pMsg ) ;					break;		
	case MP_CLIENT:				CLIENT_NetworkMsgParse( Protocol, pMsg ) ;					break;		
	case MP_HACKCHECK:			HACKCHECK->NetworkMsgParse( Protocol, pMsg );				break;
	case MP_SOCIETYACT:			SOCIETYACTMGR->NetworkMsgParse( Protocol, pMsg );			break;
	case MP_MURIMNET:			MURIM_NetworkMsgParse( Protocol, pMsg ) ;					break;
	case MP_GUILD_FIELDWAR:		GUILDFIELDWAR->NetworkMsgParse( Protocol, pMsg );			break;
	case MP_PARTYWAR:			PARTYWAR->NetworkMsgParse( Protocol, pMsg );				break;
	case MP_GTOURNAMENT:		GTMGR->NetworkMsgParse( Protocol, pMsg);					break;
	case MP_JACKPOT:			
		{
			if(GAMEIN->GetMiniMapDialog())
				JACKPOTMGR->NetworkMsgParse( Protocol, pMsg );
		}
		break;
	case MP_SURVIVAL:
		{
			if(GAMEIN->GetSurvivalCountDlg())
				SVVMODEMGR->NetworkMsgParse( Protocol, pMsg );
		}
		break;
	case MP_BOBUSANG:			BOBUSANGMGR->NetworkMsgParse( Protocol, pMsg );
	case MP_OPTION:				OPTIONMGR->NetworkMsgParse( Protocol, pMsg );				break;		
	case MP_SIEGEWAR:			SIEGEMGR->NetworkMsgParse( Protocol, pMsg );				break;	
	case MP_SIEGEWAR_PROFIT:	SWPROFIT->NetworkMsgParse( Protocol, pMsg );				break;
	case MP_PET:				PETMGR->NetworkMsgParse( Protocol, pMsg );					break;
	case MP_TITAN:				TITANMGR->NetworkMsgParse( Protocol, pMsg );				break;
	case MP_ITEMLIMIT:			ITEMLIMIT_NetworkMsgParse( Protocol, pMsg );				break;
	case MP_AUTONOTE:			AUTONOTEMGR->NetworkMsgParse( Protocol, pMsg );				break;
	case MP_FORTWAR:			FORTWARMGR->NetworkMsgParse( Protocol, pMsg );				break;
	case MP_MARRY:				MARRY_NetworkMsgParse( Protocol, pMsg ) ;					break;
	case MP_SHITU:				SHITU_NetworkMsgParse( Protocol, pMsg ) ;					break;
	case MP_TOPLIST:			break;
	case MP_DUNGEON:			DungeonMGR->NetworkMsgParse(Protocol, pMsg);				break;
	case MP_INSDUNGEON:			INSDUNGEONMGR->NetworkMsgParse(Protocol, pMsg);		break;
	case MP_MALLLIST:			//DungeonMGR->NetworkMsgParse(Protocol, pMsg);				break;
	{
		/*CGoldShopDialog* pDlg =(CGoldShopDialog*) WINDOWMGR->GetWindowForID(TC_DLG);
		switch(Protocol)
		{
		case MP_MALLLIST_ACK:
			{
				SEND_MALLLIST* pmsg = (SEND_MALLLIST*)pMsg;
				if(pmsg->nCharIdx==HEROID)
				{
					memset(&pDlg->m_mall_list, 0, sizeof(SEND_MALLLIST));
					memcpy(&pDlg->m_mall_list,pmsg,pmsg->GetSize());
				}
			}
			break;
		}*/
	}
	break;
	}
}
#include "aimanager.h"
#include "AutoPath.h"
#include "MiniMapPing.h"
void CGameIn::OnDisconnect()
{
	MINIPINGMGR->SetPingShow(TRUE);
	m_bDisconnected = TRUE;
	sCPlayerAI.IsRun = FALSE;
	//AUTOPATH->SetFindPath(FALSE);//kiv
	AUTOPATH->SetTargetPos(HERO->GetCurPosition());
	GAMEIN->GetJackFlashDlg()->SetMapChange(true);
	WINDOWMGR->DisableAllWindow(true);
	if( cMsgBox::IsInit() )
	{
		if( HACKCHECK->IsHackUser() )
		{
			cMsgBox* pMsgBox = WINDOWMGR->MsgBox( MBI_SERVERDISCONNECT, MBT_OK, CHATMGR->GetChatMsg( 560 ) );
		}
		else
		{
			REDISCONN->Init();
			REDISCONN->StartProcess();
			WINDOWMGR->MsgBox(MBI_SERVERDISCONNECT, MBT_OK, CHATMGR->GetChatMsg(2300));
			g_UserInput.SetAllowMouseClick(false);
		}
	}
	else
	{
		MessageBox( NULL, "Disconnected To Server.\nThe Game is Closing.", 0, 0 );
		PostMessage( _g_hWnd, WM_CLOSE, NULL, NULL );
	}
}
BOOL CGameIn::IsDisconnect()
{
	return m_bDisconnected;
}
void CGameIn::ProcessInBossMap()
{
	if( FALSE == MAP->IsMapKind(eBossMap) )
		return;

	if( !m_bDieInBossMap )			return;
	
	if( gCurTime > m_dwTimeInBossMap )
	{
		m_dwTimeInBossMap = gCurTime + 1000;
		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(930), m_nCountInBossMap );
		--m_nCountInBossMap;
	}
	if( m_nCountInBossMap <= 0 )
		m_bDieInBossMap = FALSE;
}
void CGameIn::SetLoginTime(SYSTEMTIME Time)
{
	GameinServerTime = Time;
}

SYSTEMTIME CGameIn::GetLoginTime()
{
	return GameinServerTime;
}
void CGameIn::GetClientLoginTime(DWORD& date, DWORD& time)
{
	date = m_Clientdate;
	time = m_Clienttime;
}

void CGameIn::SetClientLoginTime(DWORD date, DWORD time)
{
	m_Clientdate = date;
	m_Clienttime = time;
}
void CGameIn::UpdataGoldMoney()
{
	if(gCurTime-m_UpGoldMoneyTime<10000) return;
	MSG_DWORD4 msg; 
	msg.Category = MP_ITEM;
	msg.Protocol = MP_ITEM_GOLD_MONEY_SYN;
	msg.dwObjectID = HERO->GetID();
	NETWORK->Send(&msg,sizeof(msg));
	m_UpGoldMoneyTime = gCurTime;
}
BOOL CGameIn::CopyToClipboard(const char* pszData, const int nDataLen)
{
    if(::OpenClipboard(NULL))
    {
        ::EmptyClipboard();
        HGLOBAL clipbuffer;
        char *buffer;
        clipbuffer = ::GlobalAlloc(GMEM_DDESHARE, nDataLen+1);
        buffer = (char *)::GlobalLock(clipbuffer);
        strcpy(buffer, pszData);
        ::GlobalUnlock(clipbuffer);
        ::SetClipboardData(CF_TEXT, clipbuffer);
        ::CloseClipboard();
        return TRUE;
    }
    return FALSE;
}
#include <iphlpapi.h>       // API GetAdaptersInfo 头文件  
#include <shlwapi.h>        // API StrCmpIA 头文件  
#include <Strsafe.h>        // API StringCbPrintfA 头文件  
#include <shellapi.h>       // API lstrcpyA 头文件
#pragma comment(lib, "iphlpapi.lib")  
#pragma comment(lib, "shlwapi.lib")
#include <fstream>
#include <WinInet.h>
void CGameIn::GetHWID(char *HWID)
{
	if (m_CurClientMac.length() > 0)
	{ 
		SafeStrCpy(HWID, m_CurClientMac.c_str(), 33);
		return;
	}
	else
	{
		//m_CurClientMac = HWINFO->GetHwID();
		SafeStrCpy(HWID, m_CurClientMac.c_str(), 33);
	}
}

std::string CGameIn::GetInternetIP()
{
	if(CheckIP(m_CurClientIP.c_str()))
		return m_CurClientIP;
	try
	{
		HINTERNET hINet,hHttpFile;
		char szSizeBuffer[32]={0,};
		DWORD dwLengthSizeBuffer = sizeof(szSizeBuffer);
		hINet = InternetOpen(NULL,INTERNET_OPEN_TYPE_PRECONFIG,NULL,NULL,NULL);
		char url[128]={0,};

		sprintf(url,"http://www.acemso.tk/index.html");
		if(!hINet)
			return m_CurClientIP;
		hHttpFile = InternetOpenUrl(hINet,url,NULL,0,INTERNET_FLAG_RELOAD,0);
		if(!hHttpFile)
			return m_CurClientIP;
		m_CurClientIP.resize(1024);

		DWORD dwByteRead;
		BOOL bRead = InternetReadFile(hHttpFile,&m_CurClientIP[0],1024,&dwByteRead);
		if(!bRead)
			return m_CurClientIP;
		InternetCloseHandle(hHttpFile);
		InternetCloseHandle(hINet);
		//int pos = m_CurClientIP.find("<title>");
		//m_CurClientIP.replace(0,pos+7,"");
		//pos = m_CurClientIP.find("</title>");
		int pos = m_CurClientIP.find("color=");
		m_CurClientIP.replace(0,pos+14,"");
		pos = m_CurClientIP.find("</font></h1>");
		m_CurClientIP.replace(pos,m_CurClientIP.length(),"");
	}
	catch (...)
	{
		
	}	
	return m_CurClientIP;
}

BOOL CGameIn::CheckIP(const char * szIP)
{
	int a,b,c,d;
	a=b=c=d=0;
	if(sscanf(szIP,"%d.%d.%d.%d",&a,&b,&c,&d)==4&&
		(a>0&&a<255)&&
		(b>=0&&b<=255)&&
		(c>=0&&c<=255)&&
		(d>0&&d<255))
		return TRUE;
	else
		return FALSE;
}



UINT CGameIn::GetAdapterCharacteristics(char* adapter_name)  
{  
    if(adapter_name == NULL || adapter_name[0] == 0)  
        return 0;  
  
    HKEY root = NULL;  
    // Open the registry root key of the storage adapter information  
    if(ERROR_SUCCESS != RegOpenKeyExA(HKEY_LOCAL_MACHINE, "SYSTEM\\CurrentControlSet\\Control\\Class\\{4D36E972-E325-11CE-BFC1-08002BE10318}", 0, KEY_READ, &root))  
        return 0;  
  
    DWORD subkeys = 0;  
    // Get the number of subkeys under the key  
    if(ERROR_SUCCESS != RegQueryInfoKeyA(root, NULL, NULL, NULL, &subkeys, NULL, NULL, NULL, NULL, NULL, NULL, NULL))  
        subkeys = 100;  
  
    DWORD ret_value = 0;  
    for(DWORD i = 0; i < subkeys; i++)  
    {  
        // Each adapter is stored with a subkey, and the subkey name is 4 digits starting from 0
        char subkey[5];  
        memset(subkey, 0, 5);  
        StringCbPrintfA(subkey, 5, "%04u", i);  
  
        // Open this subkey  
        HKEY hKey = NULL;  
        if(ERROR_SUCCESS != RegOpenKeyExA(root, subkey, 0, KEY_READ, &hKey))  
            continue;  
  
        // Get the adapter ID corresponding to this subkey and store it in name
        char name[MAX_PATH];  
        DWORD type = 0;  
        DWORD size = MAX_PATH;  
        if(ERROR_SUCCESS != RegQueryValueExA(hKey, "NetCfgInstanceId", NULL, &type, (LPBYTE)name, &size))  
        {  
            RegCloseKey(hKey);  
            continue;  
        }  
  
        // Compare the adapter ID with the adapter ID to get the characteristics
        if(StrCmpIA(name, adapter_name) != 0)  
        {  
            RegCloseKey(hKey);  
            continue;  
        }  
  
        // Read the characteristic flag of the adapter, which is stored in the value Characteristics
        DWORD val = 0;  
        size = 4;  
        LONG ls = RegQueryValueExA(hKey, "Characteristics", NULL, &type, (LPBYTE)&val, &size);  
        RegCloseKey(hKey);  
  
        if(ERROR_SUCCESS == ls)  
        {  
            ret_value = val;  
            break;  
        }  
    }  
  
    RegCloseKey(root);  
    return ret_value;  
}  
int CGameIn::GetMAC(BYTE mac[18])  
{  
#define NCF_PHYSICAL 0x4  
    DWORD AdapterInfoSize = 0;  
    if(ERROR_BUFFER_OVERFLOW != GetAdaptersInfo(NULL, &AdapterInfoSize))  
    {  
        StringCbPrintfA((LPSTR)mac, 18, "GetMAC Failed! ErrorCode: %d", GetLastError());  
        return 0;  
    }  
  
    void* buffer = malloc(AdapterInfoSize);  
    if(buffer == NULL)  
    {  
        lstrcpyA((LPSTR)mac, "GetMAC Failed! Because malloc failed!");  
        return 0;  
    }  
  
    PIP_ADAPTER_INFO pAdapt = (PIP_ADAPTER_INFO)buffer;  
    if(ERROR_SUCCESS != GetAdaptersInfo(pAdapt, &AdapterInfoSize))  
    {  
        StringCbPrintfA((LPSTR)mac, 18, "GetMAC Failed! ErrorCode: %d", GetLastError());  
        free(buffer);  
        return 0;  
    }  
  
    int mac_length = 0;  
    while(pAdapt)  
    {  
        if(pAdapt->AddressLength >= 6 && pAdapt->AddressLength <= 8)  
        {  
            memcpy(mac, pAdapt->Address, pAdapt->AddressLength);  
            mac_length = pAdapt->AddressLength;  
  
            UINT flag = GetAdapterCharacteristics(pAdapt->AdapterName);  
            bool is_physical = ((flag & NCF_PHYSICAL) == NCF_PHYSICAL);  
            if(is_physical)  
                break;  
        }  
        pAdapt = pAdapt->Next;  
    }  
    free(buffer);  
    return mac_length;  
}
void CGameIn::GetMacAddress( char* mac )  
{  
    BYTE buf[18];  
    memset(buf, 0, 18);  
	memset(mac,0,18);
    int len = GetMAC(buf);  
    if(len <= 0)  
    {  
        lstrcpyA(mac, (LPCSTR)buf);  
        return;  
    }  
  
    if(len == 6)  
        StringCbPrintfA(mac, 18, "%02X%02X%02X%02X%02X%02X", buf[0], buf[1], buf[2], buf[3], buf[4], buf[5]);  
    else  
        StringCbPrintfA(mac, 18, "%02X%02X%02X%02X%02X%02X%02X%02X", buf[0], buf[1], buf[2], buf[3], buf[4], buf[5], buf[6], buf[7]);  
}

void CGameIn::UserConn_NetworkMsgParse(BYTE Protocol,void* pMsg)
{
	switch(Protocol)
	{
	case MP_USERCONN_OBJECT_REMOVE:
		{
			MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;
			CObject* pObject = OBJECTMGR->GetObject(pmsg->dwData);
			if(pObject == NULL)
			{
				return;
			}
			if(pObject->GetObjectKind() == eObjectKind_Pet)
			{				
				PETMGR->OnPetRemove((CPet*)pObject);
			}
			if(pObject->GetObjectKind() == eObjectKind_Player)
			{				
				((CPlayer*)pObject)->SetPet(NULL);
				OBJECTMGR->RemoveDecoration((CPlayer*)pObject);
			}

			APPEARANCEMGR->CancelAlphaProcess(pObject);
			APPEARANCEMGR->CancelReservation(pObject);
			OBJECTMGR->AddGarbageObject(pObject);
		}

		break;
	case MP_USERCONN_PET_DIE:
		{
			MSG_DWORD2* pmsg = (MSG_DWORD2*)pMsg;
			CPet* pPet = (CPet*)OBJECTMGR->GetObject(pmsg->dwData2);
			if(!pPet)	return;

			if(pPet == PETMGR->GetCurSummonPet())
			{
				DWORD SummonItemDBIdx = pPet->GetPetSommonItemDBIdx();
				PET_TOTALINFO* pInfo = PETMGR->GetPetInfo(SummonItemDBIdx);
				if(!pInfo)
				{
					ASSERT(0);
					break;
				}
				pInfo->bAlive = FALSE;
				pInfo->PetFriendly = 0;

				PETMGR->SetPetGuageText( pInfo->PetStamina, pInfo->PetFriendly );
				ITEMMGR->RefreshItemToolTip(SummonItemDBIdx);
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1261) );

				DWORD RedFilter = RGBA_MAKE(255,10,10,255);
				ITEMMGR->SetIconColorFilter(pInfo->PetSummonItemDBIdx,RedFilter);

				PETMGR->ClosePetAllDlg();
			}
			pPet->Die();
		}
		break;
	case MP_USERCONN_MONSTER_DIE:
		{
			MSG_DWORD2* pmsg = (MSG_DWORD2*)pMsg;
			CObject* pObject = OBJECTMGR->GetObject(pmsg->dwData2);
			if(!pObject)
				return;
			pObject->SetDieFlag();

			if( pmsg->dwData1 == HERO->GetID() )
			{
				GAMEEVENTMGR->AddEvent(eGameEvent_Hunt);
			}
		}
		break;
	case MP_USERCONN_CHARACTER_DIE:
		{
			MSG_DWORD2* pmsg = (MSG_DWORD2*)pMsg;
			CObject* pObject = OBJECTMGR->GetObject(pmsg->dwData2);
			if(!pObject)
				return;
			pObject->SetDieFlag();
		}
		break;		
	case MP_USERCONN_READY_TO_REVIVE:
		{
			if( MAP->IsMapKind(eEventMap) )
				return;
			GAMEIN->GetReviveDialog()->SetActive(TRUE);				

			if( GAMEIN->GetPyogukDialog()->IsActive() )
				GAMEIN->GetPyogukDialog()->SetActive( FALSE );
			
			if( MAP->IsMapKind(eBossMap) )
			{
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(929) );

				m_bDieInBossMap = TRUE;
				m_dwTimeInBossMap = gCurTime + 50000;
				m_nCountInBossMap = 10;
			}
		}
		break;

	case MP_USERCONN_CHARACTER_REVIVE:
		{

			MOVE_POS* pmsg = (MOVE_POS*)pMsg;
			VECTOR3 pos;
			CObject* pObject = OBJECTMGR->GetObject(pmsg->dwMoverID);
			if( pObject )
			{
				pmsg->cpos.Decompress(&pos);
				OBJECTACTIONMGR->Revive(pObject,&pos);
				if( pObject == HERO )
				{
					GAMEIN->GetReviveDialog()->SetDisable(FALSE);
					GAMEIN->GetReviveDialog()->SetActive(FALSE);
					
					cDialog* pDlg = WINDOWMGR->GetWindowForID( MBI_REVIVECONFIRM );
					if( pDlg )
						WINDOWMGR->AddListDestroyWindow( pDlg );

					m_bDieInBossMap = FALSE;
				}
			}
		}
		break;
		
	case MP_USERCONN_CHARACTER_REVIVE_NACK:
		{
			MSG_BYTE* pmsg = (MSG_BYTE*)pMsg;
			GAMEIN->GetReviveDialog()->SetDisable(FALSE);
		}
		break;

	case MP_USERCONN_NPC_ADD:
		{
			SEND_NPC_TOTALINFO* pmsg = (SEND_NPC_TOTALINFO*)pMsg;
			BASEMOVE_INFO moveInfo;
			moveInfo.SetFrom(&pmsg->MoveInfo);
			CAddableInfoIterator iter(&pmsg->AddableInfo);
			BYTE AddInfoKind;
			while((AddInfoKind = iter.GetInfoKind()) != CAddableInfoList::None)
			{
				ySWITCH(AddInfoKind)
					yCASE(CAddableInfoList::MoveInfo)
						COMPRESSEDTARGETPOS tpos;
						iter.GetInfoData(&tpos);
						moveInfo.bMoving = TRUE;
						moveInfo.SetMaxTargetPosIdx(tpos.PosNum);
						ASSERT( tpos.PosNum <= MAX_CHARTARGETPOSBUF_SIZE );
						moveInfo.SetCurTargetPosIdx(0);
						for(int n=0;n<tpos.PosNum;++n)
							tpos.pos[n].Decompress(moveInfo.GetTargetPosition(n));
					yCASE(CAddableInfoList::VimuInfo)
					yCASE(CAddableInfoList::StreetStall)
				yENDSWITCH
				
				iter.ShiftToNextData();
			}			
			CNpc* pNpc = OBJECTMGR->AddNpc(&pmsg->BaseObjectInfo,&moveInfo,&pmsg->TotalInfo);
			MOVEMGR->SetAngle(pNpc,pmsg->Angle,0);
			QUESTMGR->SetNpcData( pNpc );
		}
		break;
	case MP_USERCONN_GRIDINIT:
		{
			//DeleteFile("./Log/DEBUG.txt");
			m_bGameInAcked = TRUE;

			g_UserInput.GetKeyboard()->ClearKeyState();
			g_UserInput.GetKeyboard()->UpdateKeyboardState();
			g_UserInput.SetInputFocus( TRUE );
			USERINFOMGR->LoadUserInfo( eUIK_USERSTATE );
			
		}
		break;
	case MP_USERCONN_GETSERVERTIME_ACK:
		{
			stTime64t* pmsg = (stTime64t*)pMsg;
			MHTIMEMGR->SetServerTime( pmsg->time64t );
			/*
#ifdef _GMTOOL_
			struct tm curTimeWhen = *_localtime64( &pmsg->time64t );
			CHATMGR->AddMsg( CTC_SYSMSG, "server time: %d-%d-%d %02d:%02d:%02d", 
				curTimeWhen.tm_year + 1900,
				curTimeWhen.tm_mon + 1,
				curTimeWhen.tm_mday,
				curTimeWhen.tm_hour,
				curTimeWhen.tm_min,
				curTimeWhen.tm_sec );
#endif*/
		}
		break;

	case MP_USERCONN_GAMEIN_ACK:
		{//first login
			
			SEND_HERO_TOTALINFO * pmsg = (SEND_HERO_TOTALINFO *)pMsg;
			ASSERT(!(pmsg->SendMoveInfo.CurPos.wx == 0 && pmsg->SendMoveInfo.CurPos.wz == 0));

			//HERO->m_WayPointManager->ClearAllWayPoint();
			g_UserInput.SetAllowMouseClick(TRUE);
			g_UserInput.SetInputFocus(TRUE);

			if(REDISCONN->GetStart())
			{
			//	sCPlayerAI.DufualtUsekyungGong2();
				ASSERT(!m_bGameInAcked);
			}
			if (m_bDisconnected)
				m_bDisconnected = FALSE;


			GAMEIN->GetTopDialog()->GetHeroTopListInfo();
			GAMEIN->GetTopMunpDialog()->GetMunpTopListInfo();
			GAMEIN->GetTopDungeon()->GetHeroTopListInfo();
			bTempBoxT=0;

			//GAMEIN->GetGoldShopDialog()->SendSelectRequest(HEROID);

			//CHATMGR->AddMsg(CTC_SYSMSG,"%d",pmsg->dwObjectID);
			//-------------------------------------------------new
			
			
	
			

			OBJECTMGR->RegisterHero(pmsg);

			//CHATMGR->AddMsg(CTC_SYSMSG,"%d",pmsg->HeroTotalInfo.Las);
			
			GAMEIN->GetMiniMapDialog()->AddHeroIcon(HERO);
								
			EFFECTMGR->StartHeroEffectProcess(eEffect_NewCharacter);

			OPTIONMGR->ApplySettings();
			OPTIONMGR->SendOptionMsg();

			FRIENDMGR->LogInNotify();
			
			VECTOR2 sidpos;
			sidpos.x = 140;
			sidpos.y = 0;
			STATUSICONDLG->Init(HERO,&sidpos,8);

			GAMEEVENTMGR->AddEvent(eGameEvent_MapChange, MAP->GetMapNum());

			if( HERO->GetLevel() > 4 && !MUGONGMGR->GetMugongNum())
			{
				GAMEEVENTMGR->AddEvent(eGameEvent_LevelUp,6);
			}
			VECTOR3 scale;
			SetVector3(&scale,
				HERO->GetCharacterTotalInfo()->Width,
				HERO->GetCharacterTotalInfo()->Height,
				HERO->GetCharacterTotalInfo()->Width);
				HERO->GetEngineObject()->SetScale(&scale);

		//	CHATMGR->AddMsg(CTC_SYSMSG,"%1.0f,%1.0f",HERO->GetCharacterTotalInfo()->Width,
		//	HERO->GetCharacterTotalInfo()->Height);

			/*if(17 == MAP->GetMapNum())
			{
				HERO->GetCharacterTotalInfo()->Width = 9.3f;
				HERO->GetCharacterTotalInfo()->Height = 9.3f;

				VECTOR3 scale;
				SetVector3(&scale,
					HERO->GetCharacterTotalInfo()->Width,
					HERO->GetCharacterTotalInfo()->Height,
					HERO->GetCharacterTotalInfo()->Width);
				HERO->GetEngineObject()->SetScale(&scale);			
			}*/
			
			QUESTMGR->ProcessNpcMark();
			EVENTMAP->AddHero();

			if( GAMEIN->GetEventNotifyDialog() )
			{
				if( NOTIFYMGR->IsEventNotifyUse() && ( NOTIFYMGR->IsEventNotifyChanged() || GAMEIN->GetEventNotifyDialog()->IsActive() ) )
				{
					GAMEIN->GetEventNotifyDialog()->SetTitle( NOTIFYMGR->GetEventNotifyTitle() );
					GAMEIN->GetEventNotifyDialog()->SetContext( NOTIFYMGR->GetEventNotifyContext() );
					GAMEIN->GetEventNotifyDialog()->Open(); //GAMEIN->GetEventNotifyDialog()->SetActive(TRUE);
					NOTIFYMGR->SetEventNotifyChanged( FALSE );
				}
				else
				{
					GAMEIN->GetEventNotifyDialog()->Close(); //GAMEIN->GetEventNotifyDialog()->SetActive(FALSE);
				}
			}

			ITEMMGR->RefreshStatsBuffIcon();

			/*if( HERO->InTitan() )
			{
				CAMERA->InitCamera( 0, CAMERA->GetCameraDesc()->m_AngleX.ToDeg(), CAMERA->GetCameraDesc()->m_AngleY.ToDeg(),
					1500, HERO, TRUE );
				MAP->ToggleSunPosForShadow( TRUE );
			}
			else
			{
				CAMERA->Init(HERO,CAMERA->GetCameraDesc()->m_AngleX.ToDeg(),CAMERA->GetCameraDesc()->m_AngleY.ToDeg(),1000);
				MAP->ToggleSunPosForShadow( FALSE );
			}*/

			//CAMERA->SetTargetAngleY(0,0.f,0);
			//CAMERA->SetTargetAngleX(0,29.9f,0);
			
			//MAP->ToggleSunPosForShadow( FALSE );

			NPCSCRIPTMGR->SetSelectedNpc(NULL);

			bool bTemp;
			bool bShow;
			
			if(GTMAPNUM == MAP->GetMapNum())
			{
				
				m_CameraPos=1000;
				MAP->ToggleSunPosForShadow(FALSE);

				CAMERA->SetTargetAngleY(0, 0.f, 0);
				CAMERA->SetTargetAngleX(0, 29.9f, 0);

				OBJECTMGR->SetAllNameShow( TRUE );
				HERO->GetEngineObject()->SetAlpha( 1.0f );
				bTemp=false;
				bShow=true;
			}
			/*else if( DungeonMGR->IsDungeonMap(MAP->GetMapNum()))
			{
				DungeonMGR->Init();


				GAMEIN->SetMoveMap( MAP->GetMapNum() );

				m_CameraPos=GAMERESRCMNGR->m_GameDesc.CameraMaxDistance;

				//CAMERA->Init(HERO,30,0,1000);
				
				//CAMERA->Init(HERO,CAMERA->GetCameraDesc()->m_AngleX.ToDeg(),CAMERA->GetCameraDesc()->m_AngleY.ToDeg(),GAMERESRCMNGR->m_GameDesc.CameraMaxDistance);

				//GAMEIN->GetFBTimeDialog()->SetActive();

				MSGBASE msg;
				SetProtocol(&msg, MP_DUNGEON, MP_DUNGEON_INFO_SYN);
				msg.dwObjectID = gHeroID;
				NETWORK->Send( &msg, sizeof( msg ) );				

				bTemp=true;
				bShow=true;
			}*/
			else
			{
				GAMEIN->SetMoveMap( MAP->GetMapNum() );

				m_CameraPos=GAMERESRCMNGR->m_GameDesc.CameraMaxDistance;

				//CAMERA->Init(HERO,30,0,1000);
				
				//CAMERA->Init(HERO,CAMERA->GetCameraDesc()->m_AngleX.ToDeg(),CAMERA->GetCameraDesc()->m_AngleY.ToDeg(),GAMERESRCMNGR->m_GameDesc.CameraMaxDistance);

				bTemp=true;
				bShow=true;
			}

			if(g_ThisIsFirstLogin)
			{
				b_WaitFirstEvent=true;
				m_CameraPos=1000;
				g_ThisIsFirstLogin=false;
				GAMEIN->SetLoginTime(pmsg->ServerTime);
				GAMEIN->SetClientLoginTime(MHTIMEMGR->GetMHDate(), MHTIMEMGR->GetMHTime());
				TITANMGR->SetServerLogInTime();

				//REGMGR->ReadConfig();//disabled 
				//g_pExecutive->GetRenderer()->SetVerticalSync(OPTIONMGR->GetVSyncStatus());

				
			}

			CAMERA->Init(HERO, CAMERA->GetCameraDesc()->m_AngleX.ToDeg(), CAMERA->GetCameraDesc()->m_AngleY.ToDeg(), m_CameraPos);


			GAMEIN->GetQuickDialog()->MyView();


			CObject* pObj = OBJECTMGR->GetObject(pmsg->dwObjectID);
			CPlayer* ppla = (CPlayer*)pObj;
			if(bShow)
			{
				ppla->GetEngineObject()->SetAlpha( 1.0f );
				ppla->GetEngineObject()->Show();
			}
			else
			{
				ppla->GetEngineObject()->SetAlpha( 0.3f );
				ppla->GetEngineObject()->HideWithScheduling();
			}
			ppla->GetCharacterTotalInfo()->bVisible = bShow;


			sBOARDMGR->SetHeroTopListFlg(/*(CPlayer*)HERO*/ppla);
			sBOARDMGR->SetHeroTopListFlg((CPlayer*)HERO);
//			sBOARDMGR->SetHeroTopListGameIn(pmsg->dwObjectID);


			GAMEIN->GetSafeNumberIcon()->SetSafeIconShow();
			GAMEIN->GetVideoCaptureIcon()->ShowVideoIconStatic();
			

			sCPlayerAI.IsRun=bTemp;

			if(sCPlayerAI.IsRun)
			{
				//sCPlayerAI.HideObject();
				sCPlayerAI.HideObjectGamein();
				sCPlayerAI.SaveTemp();
			}
			

			if(!REDISCONN->GetStart()&&sCPlayerAI.IsOpen()&&!REDISCONN->IsAutoPlay())
				sCPlayerAI.SetOpen(false);

			sGAMECHECK->InitGameCheck();

			if(REDISCONN->GetStart())
			{
				//GAMEIN->GetReconnectDlg()->ReloadDlgPosition();
				//USERINFOMGR->LoadUserInfo(eUIK_RECONNECT);
				GAMEIN->GetReconnectDlg()->AddMessage(IMAGEMSG, 1348, (eReconnectDlg)BUTTON);
				if(REDISCONN->GetHeroMapNum()==0)
				{
					REDISCONN->StopReconnect();
				}
				if(MAP->GetMapNum()==REDISCONN->GetHeroMapNum())
				{//bukan last map, start process lastmap kat sini //"this is my map"
					sCPlayerAI.DufualtUsekyungGong2();
					REDISCONN->SetLastMapProcess(false);
					REDISCONN->SetCheckLastMap(true);
					REDISCONN->SetLastPositionProcess(true);
				}
				else//if(MAP->GetMapNum()!=REDISCONN->GetHeroMapNum())
				{//lastmap success, tutup process lastmap buka process position //"this not last map"
					REDISCONN->SetCheckLastMap(false);
					REDISCONN->SetLastMapProcess(true);
				}
				
				WINDOWMGR->DisableAllWindow(false);
			}

			

		}
		return;
	case MP_USERCONN_GAMEIN_NACK:
		{
			__asm int 3;
		}
		return;

	case MP_USERCONN_CHARACTER_ADD:
		{
			SEND_CHARACTER_TOTALINFO* pmsg = (SEND_CHARACTER_TOTALINFO*)pMsg;
			/*BASEMOVE_INFO moveInfo;
			ZeroMemory(&moveInfo, sizeof(moveInfo));
			moveInfo.SetFrom(&pmsg->MoveInfo);

			for(CAddableInfoIterator iter(&pmsg->AddableInfo);
				CAddableInfoList::MoveInfo == iter.GetInfoKind();
				iter.ShiftToNextData())
			{
				COMPRESSEDTARGETPOS compressedPosition = {0};
				iter.GetInfoData(&compressedPosition);
				moveInfo.SetMaxTargetPosIdx(compressedPosition.PosNum);

				for(BYTE i = 0;i < compressedPosition.PosNum; ++i)
				{
					VECTOR3* const targetPosition = moveInfo.GetTargetPosition(i);
					compressedPosition.pos[i].Decompress(targetPosition);
				}
			}

			moveInfo.bMoving = (0 < moveInfo.GetMaxTargetPosIdx());

			CAddableInfoIterator iter(&pmsg->AddableInfo);
			BYTE AddInfoKind;
			while((AddInfoKind = iter.GetInfoKind()) != CAddableInfoList::None)
			{
				ySWITCH(AddInfoKind)
					yCASE(CAddableInfoList::MoveInfo)
						COMPRESSEDTARGETPOS tpos;
						iter.GetInfoData(&tpos);
						moveInfo.bMoving = TRUE;
						moveInfo.SetMaxTargetPosIdx(tpos.PosNum);
						ASSERT( tpos.PosNum <= MAX_CHARTARGETPOSBUF_SIZE );
						moveInfo.SetCurTargetPosIdx(0);
						for(int n=0;n<tpos.PosNum;++n)
							tpos.pos[n].Decompress(moveInfo.GetTargetPosition(n));
				yENDSWITCH

				iter.ShiftToNextData();
			}

			CPlayer* pPlayer = OBJECTMGR->AddPlayerThroughCache(&pmsg->BaseObjectInfo,&moveInfo,&pmsg->TotalInfo,&pmsg->ShopItemOption, pmsg);


			sBOARDMGR->SetHeroTopListFlg(pPlayer);
//			sBOARDMGR->SetHeroTopListGameIn(pPlayer->GetID());

			//VECTOR3 temp;
			//temp.x = pmsg->MoveInfo.Move_Direction.x;
			//temp.z = pmsg->MoveInfo.Move_Direction.y;
			//float fDirDeg = RADTODEG(VECTORTORAD(temp));
			//MOVEMGR->SetAngle(pPlayer, fDirDeg, 0);
			//EVENTMAP->AddPlayer( pPlayer );

			if(!pmsg->bLogin)
			{
				TARGETSET set;
				set.pTarget = pPlayer;
				EFFECTMGR->StartEffectProcess(eEffect_MonRegen,pPlayer,&set,0,pPlayer->GetID());
			}


			CPet* pPet = OBJECTMGR->FindPlayerOwnPet( pPlayer->GetID() );
			if(pPet)
			{
				pPlayer->SetPet(pPet);
				pPet->SetMaster(pPlayer);
			}*/


			BASEMOVE_INFO moveInfo;
			ZeroMemory(&moveInfo, sizeof(moveInfo));
			moveInfo.SetFrom(&pmsg->MoveInfo);

			CAddableInfoIterator iter(&pmsg->AddableInfo);
			BYTE AddInfoKind;
			while ((AddInfoKind = iter.GetInfoKind()) != CAddableInfoList::None)
			{
				ySWITCH(AddInfoKind)
					yCASE(CAddableInfoList::MoveInfo)
					COMPRESSEDTARGETPOS tpos;
				iter.GetInfoData(&tpos);
				moveInfo.bMoving = TRUE;
				moveInfo.SetMaxTargetPosIdx(tpos.PosNum);
				ASSERT(tpos.PosNum <= MAX_CHARTARGETPOSBUF_SIZE);
				moveInfo.SetCurTargetPosIdx(0);
				for (int n = 0; n < tpos.PosNum; ++n)
					tpos.pos[n].Decompress(moveInfo.GetTargetPosition(n));
				yENDSWITCH

					iter.ShiftToNextData();
			}

			CPlayer* pPlayer = OBJECTMGR->AddPlayerThroughCache(&pmsg->BaseObjectInfo, &moveInfo, &pmsg->TotalInfo, &pmsg->ShopItemOption, pmsg);

			VECTOR3 temp;
			temp.x = pmsg->MoveInfo.Move_Direction.x;
			temp.z = pmsg->MoveInfo.Move_Direction.y;
			float fDirDeg = RADTODEG(VECTORTORAD(temp));
			MOVEMGR->SetAngle(pPlayer, fDirDeg, 0);

			EVENTMAP->AddPlayer(pPlayer);

			CPet* pPet = OBJECTMGR->FindPlayerOwnPet(pPlayer->GetID());
			if (pPet)
			{
				pPlayer->SetPet(pPet);
				pPet->SetMaster(pPlayer);
			}
		}
		break;
	case MP_USERCONN_PET_ADD:
		{
			SEND_PET_TOTALINFO* pmsg = (SEND_PET_TOTALINFO*)pMsg;
			BASEMOVE_INFO moveInfo;
			moveInfo.SetFrom(&pmsg->MoveInfo);
			CAddableInfoIterator iter(&pmsg->AddableInfo);
			BYTE AddInfoKind;
			while((AddInfoKind = iter.GetInfoKind()) != CAddableInfoList::None)
			{
				ySWITCH(AddInfoKind)
					yCASE(CAddableInfoList::MoveInfo)
						COMPRESSEDTARGETPOS tpos;
						iter.GetInfoData(&tpos);
						moveInfo.bMoving = TRUE;
						moveInfo.SetMaxTargetPosIdx(tpos.PosNum);
						ASSERT( tpos.PosNum <= MAX_CHARTARGETPOSBUF_SIZE );
						moveInfo.SetCurTargetPosIdx(0);
						for(int n=0;n<tpos.PosNum;++n)
							tpos.pos[n].Decompress(moveInfo.GetTargetPosition(n));
					yCASE(CAddableInfoList::VimuInfo)
					yCASE(CAddableInfoList::StreetStall)
				yENDSWITCH

					iter.ShiftToNextData();
			}

			CPet* pPet = OBJECTMGR->AddPet(&pmsg->BaseObjectInfo, &moveInfo, &pmsg->TotalInfo, pmsg->MasterName);

			pPet->SetMaster(NULL);
			CPlayer* pPlayer = (CPlayer*)OBJECTMGR->GetObject(pmsg->MasterID);
			if(pPlayer)
			{
				pPlayer->SetPet(pPet);
				pPet->SetMaster(pPlayer);

				if(HEROID == pmsg->MasterID)
				{
					PETMGR->SetCurSummonPet(pPet);
					PETMGR->SetCurSummonPetKind(pPet);
					if( PETMGR->CheckCurSummonPetKindIs(ePK_EventPet) )
					{
						CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(1482) );
					}
					else
					{
						PETMGR->OpenPetStateDlg();
					}
					PETMGR->SetPetStateDlgInfo(pPet);
					PETMGR->SetPetStateMiniDlgInfo(pPet);
					PETMGR->SetPetStateDlgUseRestInfo(pPet);
					PETMGR->SetCurPetRest(pmsg->TotalInfo.bRest);
					PETMGR->SetPetStateDlgUseRestInfo(pPet);

					PETMGR->RefleshPetMaintainBuff();
				}
			}
			else
			{
				//pPet->SetMaster(NULL);
			}
			pPet->SetMasterID(pmsg->MasterID);
		}
		break;

	case MP_USERCONN_MONSTER_ADD:
		{
			SEND_MONSTER_TOTALINFO* pmsg = (SEND_MONSTER_TOTALINFO*)pMsg;

			if(pmsg->TotalInfo.MonsterKind > 10000)
			{
				ASSERT(0);
			}
			BASEMOVE_INFO moveInfo;
			moveInfo.SetFrom(&pmsg->MoveInfo);
			CAddableInfoIterator iter(&pmsg->AddableInfo);
			BYTE AddInfoKind;
			while((AddInfoKind = iter.GetInfoKind()) != CAddableInfoList::None)
			{
				ySWITCH(AddInfoKind)
					yCASE(CAddableInfoList::MoveInfo)
						COMPRESSEDTARGETPOS tpos;
						iter.GetInfoData(&tpos);
						moveInfo.bMoving = TRUE;
						moveInfo.SetMaxTargetPosIdx(tpos.PosNum);
						ASSERT( tpos.PosNum <= MAX_CHARTARGETPOSBUF_SIZE );
						moveInfo.SetCurTargetPosIdx(0);
						for(int n=0;n<tpos.PosNum;++n)
							tpos.pos[n].Decompress(moveInfo.GetTargetPosition(n));
					yCASE(CAddableInfoList::VimuInfo)
					yCASE(CAddableInfoList::StreetStall)
				yENDSWITCH
				
				iter.ShiftToNextData();

			}
			
			
			CMonster* pMon = OBJECTMGR->AddMonster(&pmsg->BaseObjectInfo,&moveInfo,&pmsg->TotalInfo);
								
			//////////////////////////////////////////////////////////////////////////

			if(pmsg->bLogin)
			{
				TARGETSET set;
				set.pTarget = pMon;
				EFFECTMGR->StartEffectProcess(eEffect_MonRegen,pMon,&set,0,pMon->GetID());
			}
		}
		break;	
	case MP_USERCONN_BOSSMONSTER_ADD:
		{
			SEND_MONSTER_TOTALINFO* pmsg = (SEND_MONSTER_TOTALINFO*)pMsg;
			if(pmsg->TotalInfo.MonsterKind > 10000)
			{
				ASSERT(0);
			}
			
			BASEMOVE_INFO moveInfo;
			moveInfo.SetFrom(&pmsg->MoveInfo);
			CAddableInfoIterator iter(&pmsg->AddableInfo);
			BYTE AddInfoKind;
			while((AddInfoKind = iter.GetInfoKind()) != CAddableInfoList::None)
			{
				ySWITCH(AddInfoKind)
					yCASE(CAddableInfoList::MoveInfo)
						COMPRESSEDTARGETPOS tpos;
						iter.GetInfoData(&tpos);
						moveInfo.bMoving = TRUE;
						moveInfo.SetMaxTargetPosIdx(tpos.PosNum);
						ASSERT( tpos.PosNum <= MAX_CHARTARGETPOSBUF_SIZE );
						moveInfo.SetCurTargetPosIdx(0);
						for(int n=0;n<tpos.PosNum;++n)
							tpos.pos[n].Decompress(moveInfo.GetTargetPosition(n));
					yCASE(CAddableInfoList::VimuInfo)
					yCASE(CAddableInfoList::StreetStall)
				yENDSWITCH
				
				iter.ShiftToNextData();
			}
			CBossMonster* pMon = OBJECTMGR->AddBossMonster(&pmsg->BaseObjectInfo,&moveInfo,&pmsg->TotalInfo);
								
			//////////////////////////////////////////////////////////////////////////

			if(pmsg->bLogin)
			{
				TARGETSET set;
				set.pTarget = pMon;
				EFFECTMGR->StartEffectProcess(eEffect_MonRegen,pMon,&set,0,pMon->GetID());
			}
//					GetMiniMapDialog()->AddMonsterIcon(pMon);

			//GetDebugDlg()->DebugMsgParser(DBG_CHAT, "MONSTER_ADD %d", pmsg->BaseObjectInfo.dwObjectID);
		}
		break;	

	case MP_USERCONN_CHANGEMAP_ACK:
		{
			AISETMGR->GetGameAI()->sPage5.pbAutoLeveling==false;
			sCPlayerAI.IsRun==false;

			MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;
			DWORD map = pmsg->dwData;

			BATTLESYSTEM->ReleaseBattle();	
			CURSOR->SetActive( FALSE );

	
			if( GAMEIN->GetMoveDialog()->IsMovePos() )
			{
				int MotionIdx = -1;
				int EffectIdx = -1;
				WORD Weapontype = HERO->GetWeaponEquipType();
				if( Weapontype==WP_GUM || Weapontype==WP_GWUN || Weapontype==WP_GUNG || Weapontype==WP_AMGI ||
					Weapontype==WP_EVENT || Weapontype==WP_EVENT_HAMMER )
				{
					MotionIdx = eMotion_Item_Teleport_1;
					EffectIdx = eEffect_ShopItem_Teleport1;
				}
				else if( Weapontype==WP_DO )
				{
					MotionIdx = eMotion_Item_Teleport_2;
					EffectIdx = eEffect_ShopItem_Teleport2;
				}
				else if( Weapontype==WP_CHANG )

				{
					MotionIdx = eMotion_Item_Teleport_3;
					EffectIdx = eEffect_ShopItem_Teleport3;
				}

				MOVEMGR->HeroMoveStop();
				HERO->SetItemUseMotionIdx( MotionIdx );
				HERO->SetItemUseEffectIdx( EffectIdx );
				HERO->GetEngineObject()->ChangeMotion(MotionIdx, FALSE);
				m_MoveMap = map;
				OBJECTSTATEMGR->StartObjectState(HERO, eObjectState_ItemUse);
				OBJECTSTATEMGR->EndObjectState( HERO, eObjectState_ItemUse, HERO->GetEngineObject()->GetAnimationTime(MotionIdx)) ;
				// m_MoveMap = map;
				m_MovePoint = GetMoveDialog()->GetSavedPoint();
			}
			else if( GUILDMGR->GetSOSTargetPos() != 0 )	// magi82 - SOS(070724)
			{
				//Release(MAINGAME->GetCurGameState());

				m_MoveMap = map;
				m_MovePoint = GUILDMGR->GetSOSTargetPos();
				GUILDMGR->SetSOSTargetPos(0);
				WINDOWMGR->MsgBox( MBI_NOBTNMSGBOX, MBT_NOBTN, CHATMGR->GetChatMsg( 160 ) );
				MAPCHANGE->SetGameInInitKind(eGameInInitKind_MovePos);
				MAINGAME->SetGameState(eGAMESTATE_MAPCHANGE,&map,4);

				
			}
			else
			{
				//Release(MAINGAME->GetCurGameState());
				WINDOWMGR->MsgBox( MBI_NOBTNMSGBOX, MBT_NOBTN, CHATMGR->GetChatMsg( 160 ) );
				MAPCHANGE->SetGameInInitKind(eGameInInitKind_MapChange);
				MAINGAME->SetGameState(eGAMESTATE_MAPCHANGE,&map,4);

				
			}

			CHATMGR->SaveChatList();
			ITEMMGR->DeleteShopItemInfo();

			/*MAPTYPE mapNum = MAP->GetMapNum();
			if(DungeonMGR->IsDungeonMap(mapNum))
			{
				DungeonMGR->ClearCurDungeon();
			}*/
		}
		break;

	case MP_USERCONN_CHEAT_CHANGEMAP_ACK:
		{
			//AISETMGR->GetGameAI()->sPage5.pbAutoLeveling==false;
			//sCPlayerAI.IsRun==false;

			//Sleep(15);
		//	AUTOPATH->SetFindPath( false );			
		//	sCPlayerAI.SetOpen(FALSE);
		//	AISETMGR->GetGameAI()->sPage5.pbAutoLeveling==false;
		//	GAMEIN->GetJackFlashDlg()->SetMapChange(true);
		//	sCPlayerAI.IsRun=FALSE;
			//Release(MAINGAME->GetCurGameState());

			MOVEMGR->HeroMoveStop();//kiv


			BATTLESYSTEM->ReleaseBattle();	
			CURSOR->SetActive( FALSE );

			sCPlayerAI.BeforeMapChange();

			MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;
			DWORD map = pmsg->dwData;


			WINDOWMGR->MsgBox( MBI_NOBTNMSGBOX, MBT_NOBTN, CHATMGR->GetChatMsg( 700 ) );
			
			MAPCHANGE->SetGameInInitKind(eGameInInitKind_MapChange);
			MAINGAME->SetGameState(eGAMESTATE_MAPCHANGE,&map,4);

			
			ITEMMGR->DeleteShopItemInfo();//kiv
			CHATMGR->SaveChatList();
		}
		break;			
		
	case MP_USERCONN_CHANGEMAP_NACK:
		{

			if( HERO->GetState() == eObjectState_Deal )
				OBJECTSTATEMGR->EndObjectState(HERO, eObjectState_Deal);

			if(REDISCONN->GetStart())//auto reconnect cuba nak masuk map ni
			{						 //tapi dah return nack sebab offline
									 //jika REDISCONN still running,
									 //loop code kat bawah ni
				
				//REDISCONN->SetMovePlayAIMap(TRUE);
				REDISCONN->HitWaitMapCount();
			}
			else
			{
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(292) );
				g_UserInput.SetInputFocus(TRUE);
				if(GUILDMGR->GetSOSTargetPos())GUILDMGR->SetSOSTargetPos(0);
				if(GUILDMGR->GetSOSChannelIdx())GUILDMGR->SetSOSChannelIdx(-1);
				if( GetMoveDialog()->GetSavedPoint() )
				{
					ITEMMGR->SetDisableDialog(FALSE, eItemTable_Inventory);
					ITEMMGR->SetDisableDialog(FALSE, eItemTable_Pyoguk);
					ITEMMGR->SetDisableDialog(FALSE, eItemTable_GuildWarehouse);
					ITEMMGR->SetDisableDialog(FALSE, eItemTable_Shop);						
				}			
				GAMEIN->GetMoveDialog()->SetItemToMapServer( 0, 0, FALSE );
			}

			g_UserInput.SetInputFocus(TRUE);
		}
		break;

	case MP_USERCONN_CHARACTERLIST_ACK:
		{		
			SEND_CHARSELECT_INFO* pmsg = (SEND_CHARSELECT_INFO*)pMsg;
			MAINGAME->SetGameState(eGAMESTATE_CHARSELECT, (void *)pmsg, sizeof(SEND_CHARSELECT_INFO));
			ITEMMGR->DeleteShopItemInfo();
		}
		return;

	case MP_USERCONN_BACKTOCHARSEL_ACK:
		{
			AISETMGR->GetGameAI()->sPage5.pbAutoLeveling==false;
			sCPlayerAI.IsRun==false;
			MAINGAME->SetGameState( eGAMESTATE_CHARSELECT );
		}
		break;

	case MP_USERCONN_BACKTOCHARSEL_NACK:
		{
			sCPlayerAI.IsRun==true;
			WINDOWMGR->AddListDestroyWindow( WINDOWMGR->GetWindowForID( MBI_NOBTNMSGBOX ) );
			OBJECTSTATEMGR->EndObjectState( HERO, eObjectState_Enter );
		}
		break;
	case MP_USERCONN_MAPDESC:
		{
			MSG_WORD2* pmsg = (MSG_WORD2*)pMsg;
			MAP->SetVillage( pmsg->wData1 );
			PKMGR->SetPKAllow( pmsg->wData2 );
			DWORD dwCheckBit = eEventMap | eTournament | eQuestRoom | eSurvivalMap;
			if( MAP->IsMapKind(dwCheckBit) )
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg( 998 ), MAP->GetMapName() );
			else
			{
				if( MAP->IsVillage() )
					CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg( 440 ), MAP->GetMapName(), gChannelNum+1 );
				else
					CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg( 441 ), MAP->GetMapName(), gChannelNum+1 );
			}

			if( PKMGR->IsPKAllow() )
			{
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg( 558 ) );
			}

			if(HERO->GetGuildIdx() && GUILDMGR->GetGuildNotice())
				CHATMGR->AddMsg(CTC_GUILD_NOTICE, CHATMGR->GetChatMsg(1280), GUILDMGR->GetGuildNotice());
		}
		break;
	case MP_USERCONN_SETVISIBLE:
		{
			MSG_DWORD2* pmsg = (MSG_DWORD2*)pMsg;
			CObject* pObj = OBJECTMGR->GetObject(pmsg->dwData1);
			if( !pObj )
			{
				ASSERT(0);
				return;
			}
			
			if(pObj->GetObjectKind() != eObjectKind_Player)
			{
				ASSERT(0);
				return;
			}

			CPlayer* ppla = (CPlayer*)pObj;
			if(pmsg->dwData2)
			{
#ifdef _GMTOOL_
				if( MAINGAME->GetUserLevel() <= eUSERLEVEL_GM )
					ppla->GetEngineObject()->SetAlpha( 1.0f );
				else
#endif						
					ppla->GetEngineObject()->Show();
				
				ppla->GetCharacterTotalInfo()->bVisible = true;
			}
			else
			{
#ifdef _GMTOOL_
				if( MAINGAME->GetUserLevel() <= eUSERLEVEL_GM )
					ppla->GetEngineObject()->SetAlpha( 0.3f );
				else
#endif						
					ppla->GetEngineObject()->HideWithScheduling();

				ppla->GetCharacterTotalInfo()->bVisible = false;
			}
		}
		break;
		
// eventmap ---------------------------------------------------------------------------------------
	case MP_USERCONN_MAP_OUT_TO_EVENTMAP:
		{
			//AISETMGR->GetGameAI()->sPage5.pbAutoLeveling==false;
			//sCPlayerAI.IsRun==false;

			//Sleep(15);
		//	AUTOPATH->SetFindPath( false );			
		//	sCPlayerAI.SetOpen(FALSE);
		//	AISETMGR->GetGameAI()->sPage5.pbAutoLeveling==false;
		//	GAMEIN->GetJackFlashDlg()->SetMapChange(true);
		//	sCPlayerAI.IsRun=FALSE;

			sCPlayerAI.BeforeMapChange();

			MSG_DWORD3* pmsg = (MSG_DWORD3*)pMsg;
			DWORD map = pmsg->dwData1;

			EVENTMAP->SetData( pmsg->dwData1, pmsg->dwData2, pmsg->dwData3 );
			//if( MAP->GetMapNum() != 58 )
			if( FALSE == MAP->IsMapKind(eEventMap) )
				EVENTMAP->m_dwBeforeMapNum = MAP->GetMapNum();

			WINDOWMGR->MsgBox( MBI_NOBTNMSGBOX, MBT_NOBTN, CHATMGR->GetChatMsg( 700 ) );
			
			MAPCHANGE->SetGameInInitKind(eGameInInitKind_EventMapEnter);
			MAINGAME->SetGameState(eGAMESTATE_MAPCHANGE,&map,4);
			CHATMGR->SaveChatList();

			//Sleep(15);
		//	AUTOPATH->SetFindPath( false );			
		//	sCPlayerAI.SetOpen(FALSE);
		//	AISETMGR->GetGameAI()->sPage5.pbAutoLeveling==false;
		//	GAMEIN->GetJackFlashDlg()->SetMapChange(true);
		//	sCPlayerAI.IsRun=FALSE;

			//sCPlayerAI.BeforeMapChange();
		}
		break;
	case MP_USERCONN_MAP_OUT_TO_EVENTBEFOREMAP:
		{
			//AISETMGR->GetGameAI()->sPage5.pbAutoLeveling==false;
			//sCPlayerAI.IsRun==false;

			//Sleep(15);
		//	AUTOPATH->SetFindPath( false );			
		//	sCPlayerAI.SetOpen(FALSE);
		//	AISETMGR->GetGameAI()->sPage5.pbAutoLeveling==false;
		//	GAMEIN->GetJackFlashDlg()->SetMapChange(true);
		//	sCPlayerAI.IsRun=FALSE;

			sCPlayerAI.BeforeMapChange();

			WINDOWMGR->MsgBox( MBI_NOBTNMSGBOX, MBT_NOBTN, CHATMGR->GetChatMsg( 700 ) );

			MSG_WORD2 msg;
			msg.Category = MP_USERCONN;
			msg.Protocol = MP_USERCONN_MAP_OUT_WITHMAPNUM;
			msg.dwObjectID = HEROID;
			msg.wData1 = (WORD)EVENTMAP->m_dwBeforeMapNum;
			msg.wData2 = -1;

			NETWORK->Send(&msg, sizeof(msg));	
		}
		break;			
	case MP_USERCONN_EVENT_READY:
		{
			EVENTMAP->Ready();
		}
		break;
	case MP_USERCONN_EVENT_START:
		{
			EVENTMAP->Start();
		}
		break;
	case MP_USERCONN_EVENT_END:
		{
			EVENTMAP->End( (MSG_DWORD3*)pMsg );
		}
		break;					
//----------------------------------------------------------------------------------------------
	case MP_USERCONN_EVENTITEM_USE:
		{
			MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;
			if( pmsg->dwData != 0 )
                CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1178) );
			else
				CHATMGR->AddMsg( CTC_SYSMSG, "Qing Dynasty, Huolei, twisted to 50 centimeters." );	// 捞亥飘 包访 烙矫肺 持澜...
		}
		break;
	case MP_USERCONN_EVENTITEM_USE2:
		{
			CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1307) );
		}
		break;
	case MP_USERCONN_CASTLEGATE_ADD:
		{
			//SEND_GATE_TOTALINFO* pmsg = (SEND_GATE_TOTALINFO*)pMsg;

			//SIEGEMGR->CreateCastleGate( pmsg );
		}
		break;
	case MP_USERCONN_FLAGNPC_ONOFF:

		{//SW061220 惑怕标惯NPC包访 眠啊. //标惯 钎矫 救窍扁 困茄 惑怕.
			MSG_DWORD2* pmsg = (MSG_DWORD2*)pMsg;

			int flg = 0;
			if( pmsg->dwData1 )
			{
				flg |= eWeekforGT;
			}
			if( pmsg->dwData2 && pmsg->dwData2 != eSWState_EndSiegeWar )
			{
				flg |= eWeekforSG;
			}

			OBJECTMGR->SetWeekData(flg);

			//SW061205 标惯 NPC
			SYSTEMTIME st;
			GetLocalTime(&st);
			WORD day = st.wDayOfWeek;
			DWORD GTFlg, SGFlg;

			GTFlg = GAMERESRCMNGR->GetFlagFromDate(eGTFlg, day);
			OBJECTMGR->ChangeFlagNPC(eGTFlg, GTFlg);

			SGFlg = GAMERESRCMNGR->GetFlagFromDate(eSGFlg, day);
			OBJECTMGR->ChangeFlagNPC(eSGFlg, SGFlg);
		}
		break;
	case MP_USERCONN_CHANGEMAP_CHANNELINFO_ACK:
		{
			MSG_CHANNEL_INFO * pmsg = (MSG_CHANNEL_INFO*)pMsg;

			CChannelDialog* pChannelDlg = (CChannelDialog*)WINDOWMGR->GetWindowForID( CHA_CHANNELDLG );
			if(pChannelDlg != NULL)
			{
				pChannelDlg->SetChannelList(pmsg);
				
				if(pmsg->Count == 1)
				{							
				//	pChannelDlg->MapChange();
					pChannelDlg->SetActive(TRUE);
				}
			}					
		}
		break;

	case MP_USERCONN_CHANGEMAP_CHANNELINFO_NACK:
		{
			MSG_BYTE* msg = (MSG_BYTE*)pMsg;
			if( msg->bData == 0 )
			{
				CChannelDialog* pChannelDlg = (CChannelDialog*)WINDOWMGR->GetWindowForID( CHA_CHANNELDLG );
				if(pChannelDlg != NULL)
				{
					CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1700) );
					pChannelDlg->SetActive(FALSE);
				}

				if( HERO->GetState() == eObjectState_Deal )
					OBJECTSTATEMGR->EndObjectState(HERO, eObjectState_Deal);
				if(GAMEIN->GetMoveDialog()->IsDisable())
					GAMEIN->GetMoveDialog()->SetDisable(FALSE);
				if(GAMEIN->GetMoveDialog()->IsActive())
					GAMEIN->GetMoveDialog()->SetActive(FALSE);
			}
		}
		break;	
	case MP_USERCONN_CURRENTMAP_CHANNELINFO:
		{
			MSG_CHANNEL_INFO* pmsg = (MSG_CHANNEL_INFO*)pMsg;
			memset(IsBattleChannel, 0, sizeof(bool)*MAX_CHANNEL_NUM);
			memset(IsPKAllowChannel, 0, sizeof(bool)*MAX_CHANNEL_NUM);

			
            for( int i = 0; i < pmsg->Count; ++i )
			{
				IsBattleChannel[i]= pmsg->bBattleChannel[i];
				IsPKAllowChannel[i]= pmsg->bPKAllowChannel[i];

				//GAMEIN->GetRankButtonDialog()->CurrentChannelRate(pmsg->ExpRate[i],pmsg->AbilityExpRate[i],pmsg->MoneyRate[i]);

				
			}
		}
		break;
	case MP_USERCONN_CURRENT_CHANNEL_RATE:
		{
			MSG_CHANNEL_MSGJACK* pmsg = (MSG_CHANNEL_MSGJACK*)pMsg;
			GAMEIN->GetRankButtonDialog()->CurrentChannelRateMsg(pmsg);


			GAMEIN->GetBloodStormIcon()->SetActive(pmsg->IsWarNow);

			//GAMEIN->GetLuckyDrawIcon()->SetDisable(!pmsg->IsLuckyDraw);
			//GAMEIN->GetLuckyDrawIcon()->SetActive(pmsg->IsLuckyDraw); 

			GAMEIN->GetLuckyDrawIcon()->SetOpen(pmsg->IsLuckyDraw);
			

			//if(pmsg->IsWarNow)
			//	CHATMGR->AddMsg(CTC_SYSMSG,"Iswarnow");
			//else
			//	CHATMGR->AddMsg(CTC_SYSMSG,"IsNOTwarnow");

			//CHATMGR->AddMsg(0,"%1.0f,%1.0f,%1.0f",pmsg->ExpRate,pmsg->AbilityExpRate,pmsg->MoneyRate);
		}
		break;
	}
}
void CGameIn::Quest_NetworkMsgParse(BYTE Protocol,void* pMsg)
{
	QUESTMGR1->NetworkMsgParse(Protocol, pMsg);				
	QUESTMGR->NetworkMsgParse(Protocol, pMsg);
}
void CGameIn::NPC_NetworkMsgParse(BYTE Protocol,void* pMsg)
{
	switch(Protocol)
	{
	case MP_NPC_SPEECH_ACK:
		{
			MSG_DWORD2* pmsg = (MSG_DWORD2*)pMsg;

			CNpc* pNpc = (CNpc*)OBJECTMGR->GetObject(pmsg->dwData1);

			if( !pNpc )
			{
				ASSERT(0);
				if( HERO->GetState() == eObjectState_Deal )
					OBJECTSTATEMGR->EndObjectState(HERO, eObjectState_Deal);
				return;
			}

			switch( pNpc->GetNpcKind() )
			{
			case 31:
			case 81:
			case 82:
				{
					if( HERO->IsPKMode() )
					{
						CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(144) );

						if( HERO->GetState() == eObjectState_Deal )
							OBJECTSTATEMGR->EndObjectState(HERO, eObjectState_Deal);
						break;
					}

					char buf[64] = {0,};

					int nLen = strlen( pNpc->GetObjectName() ) - 6;
					if( nLen > 0 && strcmp( pNpc->GetObjectName() + nLen, "OBJ" ) == 0 )
					{
						strncpy( buf, pNpc->GetObjectName(), nLen );
						buf[nLen] = 0;
					}
					else
					{
						strcpy( buf, pNpc->GetObjectName() );
					}

					unsigned int CharLevel = HERO->GetLevel();

					WORD uniquenum = pNpc->GetNpcUniqueIdx();
					MAPCHANGE_INFO* pMCInfo = NULL;
					
					pMCInfo = GAMERESRCMNGR->GetMapChangeInfo(uniquenum);

					int destmapnumber = pMCInfo->MoveMapNum;

					cMsgBox* pBox = NULL;
					CHARSELECT->GetChannelDialog()->SendMapChannelInfoSYN(uniquenum);
					
					if( pBox )
						pBox->SetParam( pNpc->GetID() );
				}
				break;

			case 30:
			case 83:
				{
					cMsgBox* pBox = WINDOWMGR->MsgBox( MBI_SELECTLOGINPOINT, MBT_YESNO, CHATMGR->GetChatMsg(361), pNpc->GetObjectName() );
					if( pBox )
						pBox->SetParam( pNpc->GetID() );

					GAMEEVENTMGR->AddEvent(eGameEvent_LoginPoint);
				}
				break;
			case 23:
				{
					VECTOR3 pos;
					pNpc->GetPosition( &pos );
					if( pos.x == 0 && pos.z == 0 )
					{
						OBJECTSTATEMGR->EndObjectState( HERO, eObjectState_Deal );
						return;
					}

					NPCSCRIPTMGR->StartNpcScript( pNpc );
					GAMEEVENTMGR->AddEvent(eGameEvent_NpcClick);	
				}
				break; 
			case 16:
				{
					CGuild* pGuild = NULL;

					if( HERO->GetGuildMemberRank() != GUILD_MASTER )
					{
						CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1010) );								
						goto EngraveFailed;
					}
					pGuild = GUILDMGR->GetGuild();
					if( pGuild && pGuild->GetLevel() != 5 )
					{
						CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1011) );
						goto EngraveFailed;
					}
					if( HERO->GetBattleTeam() != eBattleTeam2 )
					{
						CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1057) );
						goto EngraveFailed;
					}
					
					WINDOWMGR->MsgBox( MBI_SW_ENGRAVE_SYN, MBT_YESNO, CHATMGR->GetChatMsg(1046) );
					break;
					
EngraveFailed:
					if( HERO->GetState() == eObjectState_Deal )
							OBJECTSTATEMGR->EndObjectState(HERO, eObjectState_Deal);
				}
				break;
			case 17:
			//case 18:
				{
					WORD uniquenum = pNpc->GetNpcUniqueIdx();
					FORTWARMGR->SetUniqueNpc(uniquenum);
					FORTWARMGR->SetNpcID(pNpc->GetID());
					
					WINDOWMGR->MsgBox( MBI_FT_ENGRAVE_START_SYN, MBT_YESNO, CHATMGR->GetChatMsg(1046) );
				}
				break;
			case 88:
				{
					//MAPCHANGE_INFO* pMCInfo = NULL;
					//pMCInfo = GAMERESRCMNGR->GetMapChangeInfo(pNpc->GetNpcUniqueIdx());
					//int destmapnumber = pMCInfo->MoveMapNum;

					//WORD uniquenum = pNpc->GetNpcUniqueIdx();
					//g_UserInput.GetHeroMove()->MapChange( destmapnumber, uniquenum, pNpc->GetID(), eMapChange_Dungeon );

					CInsDGEntranceInfoDialog* pInsDGEntraInfoDialog = GAMEIN->GetInsDGEntranceInfoDlg();
					if (pInsDGEntraInfoDialog != NULL)
					{
						pInsDGEntraInfoDialog->SetNpcUniqueIndex(pNpc->GetNpcUniqueIdx());
						pInsDGEntraInfoDialog->SetActive(TRUE);
					}
				}
				break;
			case 108:
				{
					VECTOR3 pos;
					pNpc->GetPosition( &pos );
					if( pos.x == 0 && pos.z == 0 )
					{
						OBJECTSTATEMGR->EndObjectState( HERO, eObjectState_Deal );
						return;
					}
					//CHATMGR->AddMsg( CTC_SYSMSG, "test 108" );
					NPCSCRIPTMGR->StartNpcScript( pNpc );
					GAMEEVENTMGR->AddEvent(eGameEvent_NpcClick);	
				}
				break;
			default:
				{
					WORD wNpcJobKind = pNpc->GetNpcJob();
					if (wNpcJobKind == INSDG_MISSION_ROLE)	//牢带 固记 浚乔矫 贸府 眠啊
					{
						NPCSCRIPTMGR->StartNpcScript(pNpc);
						INSDUNGEONMGR->SendNPCMissionEvent(pNpc);
					}
					else
					{
						NPCSCRIPTMGR->StartNpcScript(pNpc);
						GAMEEVENTMGR->AddEvent(eGameEvent_NpcClick);
					}
				}
				break;
			}
		}
		break;
	case MP_NPC_SPEECH_NACK:
		{
			if( HERO->GetState() == eObjectState_Deal )
				OBJECTSTATEMGR->EndObjectState(HERO, eObjectState_Deal);

			MSG_DWORD *pmsg = (MSG_DWORD*)pMsg;
			if(pmsg->dwData == 271)
			{
				NPCSCRIPTMGR->SetSelectedNpc(NULL);
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(883) );
			}
		}
		break;
	case MP_NPC_CLOSEBOMUL_ACK:
		{
			if(GetNpcScriptDialog()->IsActive())
			{
				NPCSCRIPTMGR->SetSelectedNpc(NULL);
				GetNpcScriptDialog()->SetActive(FALSE);						
			}
		}
		break;
	case MP_NPC_DIE_ACK:
		{
			MSG_DWORD * pmsg = (MSG_DWORD*)pMsg;
			CNpc* pNpc = NPCSCRIPTMGR->GetSelectedNpc();
			if(pNpc)
			{
				if(pNpc->GetID() == pmsg->dwData)
				{
					if(GetNpcScriptDialog()->IsActive())
					{
						NPCSCRIPTMGR->SetSelectedNpc(NULL);
						GetNpcScriptDialog()->SetActive(FALSE);						
					}
				}
			}
		}
		break;
		
	case MP_NPC_DOJOB_NACK:
		{
			NPCSCRIPTMGR->SetSelectedNpc(NULL);
			GetNpcScriptDialog()->SetActive(FALSE);						
			if( HERO->GetState() == eObjectState_Deal )
				OBJECTSTATEMGR->EndObjectState(HERO, eObjectState_Deal);
			
		}
		break;
	}
}
void CGameIn::SIGNAL_NetworkMsgParse(BYTE Protocol,void* pMsg)
{
	switch(Protocol)
	{
	case MP_SIGNAL_COMMONUSER:
		{
			TESTMSG *data = (TESTMSG *)pMsg;			
			CHATMGR->AddMsg( CTC_OPERATOR, data->Msg );
		}
		break;
	case MP_SIGNAL_ONEUSER: break;
		
	case MP_SIGNAL_SYSTEM: break;
		
	case MP_SIGNAL_BATTLE: break;
		
	case MP_SIGNAL_VIMU_RESULT:
		{
			MSG_DWORD2* pmsg = (MSG_DWORD2*)pMsg;
			CObject* pWinner = OBJECTMGR->GetObject(pmsg->dwData1);
			CObject* pLoser = OBJECTMGR->GetObject(pmsg->dwData2);
			if( pWinner == NULL || pLoser == NULL ) return;
			
			CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(655),
			pWinner->GetObjectName(), pLoser->GetObjectName(), pWinner->GetObjectName() );

		}
		break;				
	}
}
#include "TopIcon.h"
void CGameIn::CLIENT_NetworkMsgParse(BYTE Protocol,void* pMsg)
{
	switch(Protocol)
	{
	case MP_CLIENT_MSG_SYN:
		{
			MSG_CLIENT_MSG *data = (MSG_CLIENT_MSG *)pMsg;
			/*switch(data->CyptKey)
			{
			case -555:
			case MAP->GetMapNum():
				{
					CHATMGR->AddMsg( CTC_OPERATOR, data->Msg );
				}
				break;
			}
			break;*/
			if(data->CyptKey==555)
			{
				CHATMGR->AddMsg( CTC_OPERATOR, data->Msg );
			}
			if(data->CyptKey==777)
			{
				CHATMGR->AddMsg( CTC_OPERATOR, data->Msg );
			}
			else if(data->CyptKey==MAP->GetMapNum())
			{
				CHATMGR->AddMsg( CTC_OPERATOR, data->Msg );
			}
		}
		break;
	case MP_CLIENT_GAMBLE_ACK:
		{
			/*CItemGambleDialog * Dlg =GAMEIN->GetItemGambleDialog();
			if(Dlg)
			{
				MSG_DWORD3 * pmsg =(MSG_DWORD3 *)pMsg;
				int * Number =new int[3];
				Number[0]=pmsg->dwData1;
				Number[1]=pmsg->dwData2;
				Number[2]=pmsg->dwData3;
				Dlg->SetGamblePublic(Number);
				CHATMGR->AddMsg( CTC_OPERATOR, CHATMGR->GetChatMsg(2136),pmsg->dwData1,pmsg->dwData2,pmsg->dwData3);
			}*/
		}
		break;
	case MP_CLIENT_SECOND_SYN:
		{
			MSG_CHAT_WORD2 *data = (MSG_CHAT_WORD2 *)pMsg;
			if(data->Flag==eKillByWar)
			{
				char buff[16];char buff2[16];char buff3[16];char buff4[16];char buff5[256];

				if( data->Stage == 0 ) strcpy( buff,"[");					

				if (data->Stage == 64 || data->Stage == 65) strcpy( buff, "[+");					

				if (data->Stage == 128 || data->Stage == 129) strcpy( buff, "[^");
					

				if (data->AttackerStage == 0) strcpy( buff2, "[");

				if (data->AttackerStage == 64 || data->AttackerStage == 65) strcpy( buff2, "[+");

				if (data->AttackerStage == 128 || data->AttackerStage == 129) strcpy( buff2, "[^");
					

				if( data->GuildIdx1 != 0 ) wsprintf(buff3,"*%d]",data->GuildIdx1);
					
				else strcpy( buff3, "]");
					

				if( data->GuildIdx2 != 0 ) wsprintf(buff4,"*%d]",data->GuildIdx2);
					
				else strcpy( buff4, "]");
					

				wsprintf( buff5 , CHATMGR->GetChatMsg(2347) , buff2 , data->Name1 , buff4 , buff , data->Name2 , buff3 , data->KillTime );

				NOTIFYMGR->AddMsg( buff5, eNTC_ALERT );

				wsprintf( buff5, CHATMGR->GetChatMsg(2348), MAP->GetMapName(data->MapNum), data->ChannelNum );

				NOTIFYMGR->AddMsg( buff5, eNTC_ALERT );
			}
			if(data->Flag==eKillByRed)
			{
				char buff[16];char buff2[16];char buff3[16];char buff4[16];char buff5[256];

				if (data->Stage == 0) strcpy(buff, "[");

				if (data->Stage == 64 || data->Stage == 65) strcpy(buff, "[+");

				if (data->Stage == 128 || data->Stage == 129) strcpy(buff, "[^");


				if (data->AttackerStage == 0) strcpy(buff2, "[");

				if (data->AttackerStage == 64 || data->AttackerStage == 65) strcpy(buff2, "[+");

				if (data->AttackerStage == 128 || data->AttackerStage == 129) strcpy(buff2, "[^");


				if (data->GuildIdx1 != 0) wsprintf(buff3, "*%d]", data->GuildIdx1);

				else strcpy(buff3, "]");


				if (data->GuildIdx2 != 0) wsprintf(buff4, "*%d]", data->GuildIdx2);

				else strcpy(buff4, "]");


				wsprintf(buff5, CHATMGR->GetChatMsg(2347), buff2, data->Name1, buff4, buff, data->Name2, buff3, data->KillTime);

				NOTIFYMGR->AddMsg(buff5, eNTC_PRINTKILL);

				wsprintf(buff5, CHATMGR->GetChatMsg(2348), MAP->GetMapName(data->MapNum), data->ChannelNum);

				NOTIFYMGR->AddMsg(buff5, eNTC_PRINTKILL);

			}
			if(data->Flag==eLuckyDraw)
			{
				char ItemName[MAX_ITEMNAME_LENGTH+1];
				ITEM_INFO* pItemInfo = ITEMMGR->GetItemInfo( (WORD)data->dwData );
				//wsprintf(ItemName,"%s",pItemInfo->ItemName);
				SafeStrCpy(ItemName, pItemInfo->ItemName, MAX_ITEMNAME_LENGTH + 1);
				CHATMGR->AddMsg( CTC_OPERATOR,CHATMGR->GetChatMsg(2322),data->Name1,ItemName);
			}
			if(data->Flag==eAddBossMonster)
			{
				char buff5[256];
				//GAMEIN->GetTopIcon()->SetBossNameTemp(data->MonsterName);

				char buf[256] = { 0, };
				char * aa = buf;
				_parsingKeywordString(data->MonsterName, &aa);
				//sprintf(TitleText, "MAP%d(%s) [PID_%d]", ServerNum, buf, Pid);

				
				sprintf(buff5, CHATMGR->GetChatMsg(2277), buf, MAP->GetMapName(data->MapNum), data->ChannelNum, data->x, data->y);
				
				//sprintf(buff5,CHATMGR->GetChatMsg(2277),/*data->MonsterName*/GAMEIN->GetTopIcon()->GetBossNameTemp(),MAP->GetMapName(data->MapNum),data->ChannelNum,data->x,data->y);
				NOTIFYMGR->AddMsg( buff5, eNTC_BOSSYELLOW );
				CHATMGR->AddMsg( CTC_PRINT_BOSS,buff5);
			}
			if(data->Flag==eAddFieldBossMonster)
			{
				char buff5[256];
				//GAMEIN->GetTopIcon()->SetBossNameTemp(data->MonsterName);

				char buf[256] = { 0, };
				char * aa = buf;
				_parsingKeywordString(data->MonsterName, &aa);
				//sprintf(TitleText, "MAP%d(%s) [PID_%d]", ServerNum, buf, Pid);

				sprintf(buff5, CHATMGR->GetChatMsg(2277), buf, MAP->GetMapName(data->MapNum), data->ChannelNum, data->x, data->y);
				NOTIFYMGR->AddMsg( buff5, eNTC_BOSSYELLOW );
				CHATMGR->AddMsg( CTC_PRINT_BOSS,buff5);
			}
			if(data->Flag==eShoutBoxItem)
			{
				char ItemName[MAX_ITEMNAME_LENGTH + 1];
				ITEM_INFO* pItemInfo = ITEMMGR->GetItemInfo((WORD)data->dwData);
				//sprintf(ItemName, "%s", pItemInfo->ItemName);
				SafeStrCpy(ItemName, pItemInfo->ItemName, MAX_ITEMNAME_LENGTH + 1);
				CHATMGR->AddMsg(CTC_OPERATOR, CHATMGR->GetChatMsg(2455), data->Name1, ItemName);
			}
			if (data->Flag == eRecallChat)//recall chat
			{
				char TempMsg[MAX_CHAT_LENGTH + 1];
				char OwnerMsg[MAX_CHAT_LENGTH + 1];
				sprintf(TempMsg, "[%s]: %s", data->Name1, data->Msg);
				CHATMGR->RemoveMsg(TempMsg);
				sprintf(OwnerMsg, data->Name1);
				if (strcmp(data->Name1, HERO->GetObjectName()) == 0)
				{
					sprintf(OwnerMsg, "You");
				}
				CHATMGR->AddMsg(CTC_RECALL, "-\t%s %s\t-", OwnerMsg, CHATMGR->GetChatMsg(2687));
			}
			if (data->Flag == eLuckerShout)//lucker
			{
				GAMEIN->GetLuckyDrawIcon()->SetOpen((BOOL)data->dwData);
				//CHATMGR->AddMsg(CTC_OPERATOR, "LuckyDraw Enabled");
			}
			if (data->Flag == eBossDieShout)
			{
				char buf[256] = { 0, };
				char * aa = buf;
				_parsingKeywordString(data->Name1, &aa);
				CHATMGR->AddMsg(CTC_JACKPOT, CHATMGR->GetChatMsg(2723), buf, MAP->GetMapName(data->MapNum), data->ChannelNum);
			}
			if (data->Flag == eMonsterDropItemTip)
			{
				char buf[256] = { 0, };
				char * aa = buf;
				_parsingKeywordString(data->ItemName, &aa);
				char buf2[256] = { 0, };
				char * aa2 = buf2;
				_parsingKeywordString(data->ItemName1, &aa2);
				CHATMGR->AddMsg(CTC_OPERATOR, CHATMGR->GetChatMsg(2721), data->Name1, buf, buf2);
			}
			if (data->Flag == eItemTip)
			{
				char buf[256] = { 0, };
				char * aa = buf;
				_parsingKeywordString(data->ItemName, &aa);
				char buf2[256] = { 0, };
				char * aa2 = buf2;
				_parsingKeywordString(data->ItemName1, &aa2);
				CHATMGR->AddMsg(CTC_JACKPOT, CHATMGR->GetChatMsg(2724), data->Name1, buf2, buf);
			}

			///switch c++ code version //BY JACK

			/*switch(data->Flag)
			{
			case eKillByWar:
				{
					char buff[16];char buff2[16];char buff3[16];char buff4[16];char buff5[256];

					if((WORD)data->Stage==0)wsprintf(buff,"$%d",0);
					if((WORD)data->Stage==64||(WORD)data->Stage==65)wsprintf(buff,"$%d",1);
					if((WORD)data->Stage==128||(WORD)data->Stage==129)wsprintf(buff,"$%d",2);
					if(data->AttackerStage==0)sprintf(buff2,"$%d",0);
					if(data->AttackerStage==64||data->AttackerStage==65)sprintf(buff2,"$%d",1);
					if(data->AttackerStage==128||data->AttackerStage==129)sprintf(buff2,"$%d",2);

					if((DWORD)data->GuildIdx1!=0)sprintf(buff3,"*%d",(DWORD)data->GuildIdx1);
					else sprintf(buff3,"*%d",0);

					if((DWORD)data->GuildIdx2!=0)sprintf(buff4,"*%d",(DWORD)data->GuildIdx2);
					else sprintf(buff4,"*%d",0);
						
					sprintf(buff5,CHATMGR->GetChatMsg(2347),buff2,data->Name1,buff4,buff,data->Name2,buff3,data->KillTime);
					NOTIFYMGR->AddMsg( buff5, eNTC_ALERT );

					sprintf(buff5,CHATMGR->GetChatMsg(2348),MAP->GetMapName(data->MapNum),data->ChannelNum);
					NOTIFYMGR->AddMsg( buff5, eNTC_ALERT );
				}
				break;
			case eKillByRed:
				{
					char buff[16];char buff2[16];char buff3[16];char buff4[16];char buff5[256];

					if((WORD)data->Stage==0)wsprintf(buff,"$%d",0);
					if((WORD)data->Stage==64||(WORD)data->Stage==65)wsprintf(buff,"$%d",1);
					if((WORD)data->Stage==128||(WORD)data->Stage==129)wsprintf(buff,"$%d",2);
					if(data->AttackerStage==0)sprintf(buff2,"$%d",0);
					if(data->AttackerStage==64||data->AttackerStage==65)sprintf(buff2,"$%d",1);
					if(data->AttackerStage==128||data->AttackerStage==129)sprintf(buff2,"$%d",2);

					if((DWORD)data->GuildIdx1!=0)sprintf(buff3,"*%d",(DWORD)data->GuildIdx1);
					else sprintf(buff3,"*%d",0);

					if((DWORD)data->GuildIdx2!=0)sprintf(buff4,"*%d",(DWORD)data->GuildIdx2);
					else sprintf(buff4,"*%d",0);

					sprintf(buff5,CHATMGR->GetChatMsg(2347),buff2,data->Name1,buff4,buff,data->Name2,buff3,data->KillTime);
					NOTIFYMGR->AddMsg( buff5, eNTC_PRINTKILL );

					sprintf(buff5,CHATMGR->GetChatMsg(2348),MAP->GetMapName(data->MapNum),data->ChannelNum);
					NOTIFYMGR->AddMsg( buff5, eNTC_PRINTKILL );
				}
				break;
			case eLuckyDraw:
				{
					char ItemName[MAX_ITEMNAME_LENGTH+1];
					ITEM_INFO* pItemInfo = ITEMMGR->GetItemInfo( (WORD)data->dwData );
					wsprintf(ItemName,"%s",pItemInfo->ItemName);
					CHATMGR->AddMsg( CTC_OPERATOR,CHATMGR->GetChatMsg(2322),data->Name1,ItemName);
				}
				break;
			case eAddBossMonster:
				{
					CHATMGR->AddMsg( CTC_PRINT_BOSS,CHATMGR->GetChatMsg(2277),data->MonsterName,MAP->GetMapName(data->MapNum),data->ChannelNum,data->x,data->y);
					CHATMGR->AddMsg( CTC_PRINT_BOSS,CHATMGR->GetChatMsg(2346),data->x,data->y);
					
					char buff5[256];
					sprintf(buff5,CHATMGR->GetChatMsg(2277),data->MonsterName,MAP->GetMapName(data->MapNum),data->ChannelNum,data->x,data->y);
					NOTIFYMGR->AddMsg( buff5, eNTC_BOSSYELLOW );

					sprintf(buff5,CHATMGR->GetChatMsg(2346),data->x,data->y);
					NOTIFYMGR->AddMsg( buff5, eNTC_BOSSYELLOW );
				}
				break;
			case eAddFieldBossMonster:
				{
					CHATMGR->AddMsg( CTC_PRINT_BOSS,CHATMGR->GetChatMsg(2277),data->MonsterName,MAP->GetMapName(data->MapNum),data->ChannelNum,data->x,data->y);
					CHATMGR->AddMsg( CTC_PRINT_BOSS,CHATMGR->GetChatMsg(2346),data->x,data->y);

					char buff5[256];
					sprintf(buff5,CHATMGR->GetChatMsg(2277),data->MonsterName,MAP->GetMapName(data->MapNum),data->ChannelNum,data->x,data->y);
					NOTIFYMGR->AddMsg( buff5, eNTC_BOSSYELLOW );

					sprintf(buff5,CHATMGR->GetChatMsg(2346),data->x,data->y);
					NOTIFYMGR->AddMsg( buff5, eNTC_BOSSYELLOW );
				}
				break;
			case eShoutBoxItem:
				{
					char ItemName[MAX_ITEMNAME_LENGTH+1];
					ITEM_INFO* pItemInfo = ITEMMGR->GetItemInfo( (WORD)data->dwData );
					wsprintf(ItemName,"%s",pItemInfo->ItemName);
				}
			break;
			}*/
		}
		break;

	}
	
}

void CGameIn::MURIM_NetworkMsgParse(BYTE Protocol,void* pMsg)
{
	switch( Protocol )
	{
	case MP_MURIMNET_CHANGETOMURIMNET_ACK:
		{
			MSGBASE* pmsg = (MSGBASE*)pMsg;
			
			MAPTYPE dwMapNum = MAP->GetMapNum();
			MAINGAME->SetGameState( eGAMESTATE_MURIMNET, &dwMapNum, 4 );
		}
		break;

	case MP_MURIMNET_CHANGETOMURIMNET_NACK:
		{
			MessageBox(NULL, "Fail To Connect To MurimServer.",0,0);
		}
		break;

	case MP_MURIMNET_RETURNTOMURIMNET_ACK:
		{
			MSGBASE* pmsg = (MSGBASE*)pMsg;
			
			MAINGAME->SetGameState( eGAMESTATE_MURIMNET );
		}
		break;

	case MP_MURIMNET_RETURNTOMURIMNET_NACK:
		{
			MessageBox(NULL, "Fail To Come Back To MurimServer. Connect Again Plz",0,0);
			PostMessage( _g_hWnd, WM_CLOSE, NULL, NULL );
		}
		break;

	case MP_MURIMNET_MURIMSERVERIN_ACK:
		{
			SEND_HERO_TOTALINFO * pmsg = (SEND_HERO_TOTALINFO *)pMsg;
			OBJECTMGR->RegisterHero(pmsg);
			
			GAMEIN->GetMiniMapDialog()->AddHeroIcon(HERO);
								
			CAMERA->Init((CObject*)OBJECTMGR->GetHero(),30,0,1000);

			CAMERA->SetTargetAngleX(0,29.9f,0);					
		}
		break;

	case MP_MURIMNET_MURIMSERVERIN_NACK: break;		
	}
}
void CGameIn::ITEMLIMIT_NetworkMsgParse(BYTE Protocol,void* pMsg)
{
	switch( Protocol )
	{
	case MP_ITEMLIMIT_FULL_TO_CLIENT:
		{
			MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;
			ITEM_INFO* pInfo = ITEMMGR->GetItemInfo( pmsg->dwData );
			if( pInfo )
                CHATMGR->AddMsg( CTC_ALERT_YELLOW, "%s", pInfo->ItemName );
			else
				CHATMGR->AddMsg( CTC_ALERT_YELLOW, "[%d]", pmsg->dwData );
		}
		break;
	}
}
void CGameIn::MARRY_NetworkMsgParse(BYTE Protocol,void* pMsg)
{
	switch(Protocol)
	{
	case MP_MARRY_ADD_INVITE:
		{
			MSG_MARRY_ADDMSG* pmsg = (MSG_MARRY_ADDMSG*)pMsg;
			HERO->m_mary_msg = *pmsg;
			cMsgBox* pMsgBox = WINDOWMGR->MsgBox( MARY_FT_ACCEPT_BT, MBT_YESNO, CHATMGR->GetChatMsg(1817),pmsg->Name );
			
		}
		break;
	case MP_MARRY_ADD_ACK:
		{
			MSG_MARRY_ADDMSG* pmsg = (MSG_MARRY_ADDMSG*)pMsg;

			HERO->SetMarryName(pmsg->Name);

			OBJECTMGR->ApplyOverInfoOptionToAll();
										
			CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg( 1806 ) );
		}
		break;
	case MP_MARRY_ADD_DENY_ACK:
		{
			CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg( 1852 ) );
		}
		break;
	case MP_MARRY_ADD_NACK:
		{
            MSG_DWORD2* pmsg = (MSG_DWORD2*)pMsg;

			if(pmsg->dwData2==1)
			{
               CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1804));

			   return ;
			}
			if(pmsg->dwData2==2)
			{
               CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1849));

			   return ;
			}
			if(pmsg->dwData2==3)
			{
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1827),pmsg->dwData1);

			   return ;
			}
			if(pmsg->dwData2==4)
			{
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1828),pmsg->dwData1);

			   return ;
			}
			if(pmsg->dwData2==5)
			{
			   CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1829),pmsg->dwData1);

			   return ;
			}
			if(pmsg->dwData2==6)
			{
			   CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1801));

			   return ;
			}
			if(pmsg->dwData2==7)
			{
			   CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1802));

			   return ;
			}
			if(pmsg->dwData2==8)
			{
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1803),pmsg->dwData1);

			   return ;
			}
			if(pmsg->dwData2==9)
			{
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1804));

			   return ;
			}			
		}
		break;
	case MP_MARRY_DEL_NACK:
		{
            MSG_DWORD2* pmsg = (MSG_DWORD2*)pMsg;

			if(pmsg->dwData2==0)
			{
               CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1807));

			   return ;
			}

			if(pmsg->dwData2==1)
			{
               CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(2083));

			   return ;
			}
			if(pmsg->dwData2==2)
			{
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(2085),pmsg->dwData1);

			   return ;
			}
			if(pmsg->dwData2==3)
			{
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(2084),pmsg->dwData1);

			   return ;
			}
		}
		break;
	case MP_MARRY_DEL_ACK:  //weiye 离婚客户端处理 2018-05-01 
		{
			MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;
			CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg( 1808 ) );
			HERO->SetMarryName("0");
			CPlayer* pPlayer = (CPlayer*)OBJECTMGR->GetObject(pmsg->dwData);
			if(pPlayer)
			{
				pPlayer->SetMarryName("0");
			} 
			OBJECTMGR->ApplyOverInfoOptionToAll();
		}
		break;
	}
}
void CGameIn::SHITU_NetworkMsgParse(BYTE Protocol,void* pMsg)
{
	switch(Protocol)
	{
		case MP_SHITU_ADD_NACK:
		{
            MSG_DWORD2* pmsg = (MSG_DWORD2*)pMsg;

			if(pmsg->dwData2==1)
			{
				 CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(2086));
		         return;
			}
			if(pmsg->dwData2==2)
			{
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(2087),pmsg->dwData1);
		         return;
			}
			if(pmsg->dwData2==3)
			{	
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(2009),pmsg->dwData1);
		         return;
			}
		}
	    break;
	    case MP_SHITU_ADD_INVITE:
		{
			MSG_MARRY_ADDMSG* pmsg = (MSG_MARRY_ADDMSG*)pMsg;
			HERO->m_mary_msg = *pmsg;
			cMsgBox* pMsgBox = WINDOWMGR->MsgBox( BAISHI_FT_ACCEPT_BT, MBT_YESNO, CHATMGR->GetChatMsg(1838),pmsg->Name );
		}
		break;
		case MP_SHITU_ADD_TUDI_ACK:
		{
			MSG_MARRY_ADDMSG* pmsg = (MSG_MARRY_ADDMSG*)pMsg;

			CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(2088),pmsg->Name);
		}
		break;
		case MP_SHITU_ADD_SHIFU_ACK:
		{
			MSG_MARRY_ADDMSG* pmsg = (MSG_MARRY_ADDMSG*)pMsg;
			HERO->SetShiFuID(pmsg->PlayerID);
			HERO->SetShiFuName(pmsg->Name);
		}
		break;
		case MP_SHITU_DEL_NACK:
		{

			MSG_DWORD2 *pmsg=(MSG_DWORD2*) pMsg;

			if(pmsg->dwData2==1)
			{
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(2090),pmsg->dwData1);
		         return;
			}
			if(pmsg->dwData2==2)
			{
                CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(2091));
		         return;
			}
		}
		break;
        case MP_SHITU_DEL_ACK:
		{
             MSG_DWORD_CHAR * pmsg=(MSG_DWORD_CHAR*)pMsg;

			 if(pmsg->dwData==1)
			 {
                 CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(2092));
                 HERO->ClearShiFu(0);
		         return;
			 }
			 if(pmsg->dwData==2)
			 {
				 CHATMGR->AddMsg(CTC_SYSMSG,CHATMGR->GetChatMsg(2093),pmsg->CharMsg);
                 return;
			 }

		}
		break;
	}
}























#ifdef _TESTCLIENT_

void InitTestHero()
{
	DWORD id = 1;
	{	// AOAI∑┆
		SEND_HERO_TOTALINFO info;
		memset(&info,0,sizeof(info));
		info.BaseObjectInfo.dwObjectID = id++;
		strcpy( info.BaseObjectInfo.ObjectName, "DARK");
		VECTOR3 pos;
		pos.x = GAMERESRCMNGR->m_TestClientInfo.x;
		pos.z = GAMERESRCMNGR->m_TestClientInfo.z;
		info.SendMoveInfo.CurPos.Compress(&pos);
		info.ChrTotalInfo.Gender = GAMERESRCMNGR->m_TestClientInfo.Gender;
		info.ChrTotalInfo.Life = 1000;
		info.ChrTotalInfo.HairType = GAMERESRCMNGR->m_TestClientInfo.HairType;
		info.ChrTotalInfo.FaceType = GAMERESRCMNGR->m_TestClientInfo.FaceType;
		info.ChrTotalInfo.MaxLife = 1000;
		info.HeroTotalInfo.wNaeRyuk = 1000;
		info.HeroTotalInfo.wMaxNaeRyuk = 1000;
		info.ChrTotalInfo.Level = 99;
		info.ChrTotalInfo.Height = 1;
		info.ChrTotalInfo.Width = 1;
		info.ChrTotalInfo.WearedItemIdx[eWearedItem_Weapon] = GAMERESRCMNGR->m_TestClientInfo.WeaponIdx;
		info.ChrTotalInfo.WearedItemIdx[eWearedItem_Dress] = GAMERESRCMNGR->m_TestClientInfo.DressIdx;
		info.ChrTotalInfo.WearedItemIdx[eWearedItem_Hat] = GAMERESRCMNGR->m_TestClientInfo.HatIdx;
		info.ChrTotalInfo.WearedItemIdx[eWearedItem_Shoes] = GAMERESRCMNGR->m_TestClientInfo.ShoesIdx;
		info.HeroTotalInfo.KyungGongGrade = 2604;
		info.ChrTotalInfo.bVisible = true;
		
		info.MugongTotalInfo.mugong[0].dwDBIdx = 1;
		info.MugongTotalInfo.mugong[0].bWear = FALSE;
		info.MugongTotalInfo.mugong[0].ExpPoint = 500;
		info.MugongTotalInfo.mugong[0].Position = TP_MUGONG1_START;
		info.MugongTotalInfo.mugong[0].QuickPosition = 0;
		info.MugongTotalInfo.mugong[0].Sung = 1;
		info.MugongTotalInfo.mugong[0].wIconIdx = GAMERESRCMNGR->m_TestClientInfo.MugongIdx;
		
		info.MugongTotalInfo.mugong[1].dwDBIdx = 2;
		info.MugongTotalInfo.mugong[1].bWear = FALSE;
		info.MugongTotalInfo.mugong[1].ExpPoint = 200;
		info.MugongTotalInfo.mugong[1].Position = TP_MUGONG1_START+1;
		info.MugongTotalInfo.mugong[1].QuickPosition = 0;
		info.MugongTotalInfo.mugong[1].Sung = 0;
		info.MugongTotalInfo.mugong[1].wIconIdx = 200;
		
		info.MugongTotalInfo.mugong[2].dwDBIdx = 3;
		info.MugongTotalInfo.mugong[2].bWear = FALSE;
		info.MugongTotalInfo.mugong[2].ExpPoint = 1000;
		info.MugongTotalInfo.mugong[2].Position = TP_MUGONG1_START+2;
		info.MugongTotalInfo.mugong[2].QuickPosition = 0;
		info.MugongTotalInfo.mugong[2].Sung = 2;
		info.MugongTotalInfo.mugong[2].wIconIdx = 302;
		for(int n=0;n<eAGK_Max;++n)
		{
			memset(info.AbilityInfo.AbilityDataArray[n],'B',MAX_ABILITY_NUM_PER_GROUP);
			memset(info.AbilityInfo.AbilityQuickPositionArray[n],' ',MAX_ABILITY_NUM_PER_GROUP);
		}
		OBJECTMGR->RegisterHero(&info);
		GAMEIN->GetMiniMapDialog()->AddHeroIcon(HERO);
		
		CAMERA->Init((CObject*)OBJECTMGR->GetHero(),30,0,1000);
		
		VECTOR2 sidpos;
		sidpos.x = 140;
		sidpos.y = 0;
		STATUSICONDLG->Init(HERO,&sidpos,5);


		BASEOBJECT_INFO obinfo;
		BASEMOVE_INFO moinfo;
		memset( &obinfo, 0, sizeof(obinfo) );		
		memset( &moinfo, 0, sizeof(moinfo) );

		obinfo.dwObjectID = 100;
		obinfo.dwUserID = 5572;
		sprintf( obinfo.ObjectName, "pet-01" );
		obinfo.BattleID = gChannelNum+2;

		moinfo.CurPosition = HERO->GetCurPosition();
		moinfo.CurPosition.x -= 150;

		if(GAMERESRCMNGR->m_TestClientInfo.bInTitan == TRUE)
		{
			HERO->RidingTitan( TRUE );
			HERO->SetTitanAppearanceInfo( &GAMERESRCMNGR->m_TestClientInfo.titaninfo );
			HERO->SetTitanPreView( &GAMERESRCMNGR->m_TestClientInfo.titaninfo );
		}		

		CAMERA->InitCamera( 0, CAMERA->GetCameraDesc()->m_AngleX.ToDeg(), CAMERA->GetCameraDesc()->m_AngleY.ToDeg(),
			1500, HERO, TRUE );
		MAP->ToggleSunPosForShadow( TRUE );
	
	//	CPetBase* pPet = OBJECTMGR->AddPet( &obinfo, &moinfo );
	//	HERO->SetPet( pPet );
	}
}


void InitTestMonster()
{
	DWORD id = 10;
	float x,z;
	x = GAMERESRCMNGR->m_TestClientInfo.x + 25;
	z = GAMERESRCMNGR->m_TestClientInfo.z + 500 + 25;


	DWORD temp =0;
	for(int n=0;n<GAMERESRCMNGR->m_TestClientInfo.MonsterNum;++n)
	{
		SEND_MONSTER_TOTALINFO info;
		memset(&info,0,sizeof(info));
		info.BaseObjectInfo.dwObjectID = id++;
		info.BaseObjectInfo.BattleID = gChannelNum+1;
		sprintf(info.BaseObjectInfo.ObjectName,"Monster%02d",temp++);
		VECTOR3 pos;
		pos.x = x;

		pos.z = z;
		info.MoveInfo.CurPos.Compress(&pos);
		BASEMOVE_INFO bminfo;
		bminfo.SetFrom(&info.MoveInfo);
		info.TotalInfo.MonsterKind = GAMERESRCMNGR->m_TestClientInfo.MonsterKind;
		g_pMonster = OBJECTMGR->AddMonster(&info.BaseObjectInfo,&bminfo,&info.TotalInfo);
		
		x+= 100;
	}

	temp =0;
	for(n=0;n<GAMERESRCMNGR->m_TestClientInfo.BossMonsterNum;++n)
	{
		SEND_MONSTER_TOTALINFO info;
		memset(&info,0,sizeof(info));
		info.BaseObjectInfo.dwObjectID = id++;
		sprintf(info.BaseObjectInfo.ObjectName,"BossMonster%02d",temp++);
		VECTOR3 pos;
		pos.x = x;
		pos.z = z;
		info.MoveInfo.CurPos.Compress(&pos);
		BASEMOVE_INFO bminfo;
		bminfo.SetFrom(&info.MoveInfo);
		info.TotalInfo.MonsterKind = GAMERESRCMNGR->m_TestClientInfo.BossMonsterKind;
		g_pBossMonster = OBJECTMGR->AddBossMonster(&info.BaseObjectInfo,&bminfo,&info.TotalInfo);
	//	g_pBossMonster->LoadBossMonster(GAMERESRCMNGR->m_TestClientInfo.BossInfofile);
		
		x+= 100;
	}

	x = GAMERESRCMNGR->m_TestClientInfo.x + 25;
	z -= 200;
	int lastnp = 0;
	for(n=0;n<GAMERESRCMNGR->m_TestClientInfo.CharacterNum;++n)
	{
		SEND_CHARACTER_TOTALINFO cinfo;
		memset(&cinfo,0,sizeof(cinfo));
		cinfo.BaseObjectInfo.dwObjectID = id++;
		sprintf(cinfo.BaseObjectInfo.ObjectName,"Character%02d",temp++);
		VECTOR3 pos;
		pos.x = x;
		pos.z = z;
		cinfo.MoveInfo.CurPos.Compress(&pos);
		BASEMOVE_INFO bminfo;
		bminfo.SetFrom(&cinfo.MoveInfo);
		cinfo.TotalInfo.bVisible = 1;
		cinfo.TotalInfo.Height = 1;
		cinfo.TotalInfo.Width = 1;
		cinfo.TotalInfo.bVisible = 1;
		cinfo.TotalInfo.WearedItemIdx[eWearedItem_Dress] = 23000 + rand()%150;
		cinfo.TotalInfo.WearedItemIdx[eWearedItem_Weapon] = 11000 + rand()%150;
		OBJECTMGR->AddPlayer(&cinfo.BaseObjectInfo,&bminfo,&cinfo.TotalInfo,NULL);
		
		x+= 100;
		if(n/50 != lastnp)
		{
			lastnp = n/50;
			x = GAMERESRCMNGR->m_TestClientInfo.x + 25;
			z -= 200;
		}
	}
	
/*
	SEND_MONSTER_TOTALINFO info;
	memset(&info,0,sizeof(info));
	info.BaseObjectInfo.dwObjectID = id++;
	sprintf(info.BaseObjectInfo.ObjectName,"BossMonster%02d",temp++);
	info.BaseMoveInfo.CurPosition.x = x;
	info.BaseMoveInfo.CurPosition.z = z;
	info.TotalInfo.MonsterKind = 65;
	g_pBossMonster = OBJECTMGR->AddBossMonster(&info.BaseObjectInfo,&info.BaseMoveInfo,&info.TotalInfo);
*/
}

//trustpak
void InitTestCharacters(void)
{	
	DWORD id = 1000;
	DWORD temp =0;

	//for(n=0;n<GAMERESRCMNGR->m_TestClientInfo.CharacterNum;++n)
	DWORD n = 0;
	for (n = 0; n < GAMERESRCMNGR->m_TestClientInfo.dwTestCharacterNo;n++)
	{
		SEND_CHARACTER_TOTALINFO cinfo;
		memset(&cinfo,0,sizeof(cinfo));
		cinfo.BaseObjectInfo.dwObjectID = id++;
		sprintf(cinfo.BaseObjectInfo.ObjectName,"Test Character%02d",temp++);
		
		VECTOR3 pos;
		pos.x = GAMERESRCMNGR->m_TestClientInfo.aTestCharacterInfo[n].fX;
		pos.y = GAMERESRCMNGR->m_TestClientInfo.aTestCharacterInfo[n].fY;
		pos.z = GAMERESRCMNGR->m_TestClientInfo.aTestCharacterInfo[n].fZ;
		cinfo.MoveInfo.CurPos.Compress(&pos);

		BASEMOVE_INFO bminfo;
		bminfo.SetFrom(&cinfo.MoveInfo);
		cinfo.TotalInfo.bVisible = 1;
		cinfo.TotalInfo.Height = 1;
		cinfo.TotalInfo.Width = 1;
		cinfo.TotalInfo.bVisible = 1;
		cinfo.TotalInfo.WearedItemIdx[eWearedItem_Dress] = 23000 + rand()%150;
		cinfo.TotalInfo.WearedItemIdx[eWearedItem_Weapon] = 11000 + rand()%150;		
		cinfo.TotalInfo.Gender = GAMERESRCMNGR->m_TestClientInfo.aTestCharacterInfo[n].byGender;

		OBJECTMGR->AddPlayer(&cinfo.BaseObjectInfo,&bminfo,&cinfo.TotalInfo,NULL);		
		CObject * pObj = OBJECTMGR->GetObject(cinfo.BaseObjectInfo.dwObjectID);
		pObj->SetAngle(GAMERESRCMNGR->m_TestClientInfo.aTestCharacterInfo[n].fAngle);		
	}	
}

#endif
