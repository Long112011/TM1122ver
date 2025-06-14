



// ServerSystem.cpp: implementation of the CServerSystem class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include <string>           // ç«˜è?string ?¾å©¦èº‡åŸ¥?»é??šå? 2014-12-06
#include "ServerSystem.h"
#include "Console.h"
#include "Network.h"
#include "DataBase.h"
#include "CommonNetworkMsgParser.h"
#include "MapNetworkMsgParser.h"

#include "Usertable.h"
#include "Servertable.h"

#include "ObjectFactory.h"
#include "UserTable.h"
#include "GridSystem.h"
#include "MapDBMsgParser.h"
#include "CharMove.h"
#include "Player.h"
#include "Pet.h"

#include "Monster.h"
#include "BossMonster.h"
#include "BossMonsterManager.h"
#include "BossRewardsManager.h"
#include "Npc.h"
#include "TacticObject.h"
#include "TacticStartInfo.h"
#include "Object.h"
#include "SkillObject_Server.h"
#include "..\[CC]Header\GameResourceManager.h"
#include "MHTimeManager.h"
#include "ItemManager.h"
#include "TileManager.h"
#include "RegenNPC.h"
#include "MHFile.h"
#include "CharacterCalcManager.h"
#include "PartyManager.h"
#include "ObjectStateManager.h"
#include "ItemDrop.h"
#include "MapDBMsgParser.h"
#include "TacticManager.h"
#include "AISystem.h"
#include "SkillManager_Server.h"
#include "BattleSystem_Server.h"
#include "ChannelSystem.h"
#include "SuryunRegenManager.h"
#include "../[CC]Suryun/SuryunManager_server.h"
#include "TitanItemManager.h"

//#include "Respwan.h"
#include "KyungGongManager.h"

//#include "ExchangeManager.h"
#include "StreetSTallManager.h "
#include "PyogukManager.h"
#include "FriendManager.h"
#include "./Condition/ConditionManager.h"
#include "FameManager.h"
#include "BootManager.h"
#include "PathManager.h"
#include "AbilityManager.h"
#include "AbilityUpdater_CheckLevelUp.h"
#include "AbilityUpdater_DataBase.h"
#include "AbilityUpdater_ApplyData.h"
#include "RegenPrototype.h"
#include "RegenManager.h"
#include "VimuManager.h"
#include "PKManager.h"
#include "LootingManager.h"

#include "ReinforceManager.h"
#include "DissolutionManager.h"
#include "ChangeItemMgr.h"
#include "EventMapMgr.h"
#include "GuildFieldWarMgr.h"
#include "PartyWarMgr.h"

#include "MemoryChecker.h"
#include "GuildTournamentMgr.h"
#include "SiegeWarMgr.h"
//SW_TEST
#include "cJackpotManager.h"
// jsd quest
#include "QuestManager.h"
#include "..\[CC]Quest\QuestEvent.h"
#include "QuestRegenMgr.h"
#include "QuestMapMgr.h"

#include "StatisticsManager.h"
#include "GuildManager.h"

#include "cMonsterSpeechManager.h"
#include "RarenessManager.h"
#include "SiegeWarProfitMgr.h"
#include "SiegeWarMgr.h"
#include "MapObject.h"
#include "..\[CC]ServerModule\MiniDumper.h"

// Weather System
#include "WeatherManager.h"

// PetManager
#include "PetSpeechManager.h"

// ?·è??¾è¶ - 05.12 æªœè???
#include "FieldBossMonsterManager.h"
#include "FieldBossMonster.h"
#include "FieldSubMonster.h"

//SW061019 ?®å¤¥æªœå?è³…è?
#include "SurvivalModeManager.h"

//SW070626 ?¾ç¡é¼»NPC
#include "BobusangManager.h"
#include "ItemLimitManager.h"
#include "GameEventManager.h"
#include "AutoNoteManager.h"
#include "FortWarManager.h"


// è£??„å¥§ å¬´æ???è¹ºé? by Stiner(2008/05/29)-MapDropItem
#include "MapItemDrop.h"
#include "SafeArea.h"            //weiye ?‡ïŸµ?¥é??›æ???2018-03-15
#include "GuildWarManager.h"

#include "Dungeon/DungeonMgr.h"
#include "../[CC]JACK/JackJack.h"

#include "LoginEventManager.h"

#include "InsDungeonManager.h"
#include "InsDungeonMapManager.h"
#include "InsDungeonRegenDataManager.h"
#include "InsDungeon.h"

#include "..\[CC]Header\Server_CMD.h"

void __stdcall ProcessDBMessage(DWORD dwEventIndex);
void __stdcall ReceivedMsgFromServer(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength);
void __stdcall ReceivedMsgFromUser(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength);
void __stdcall OnAcceptServer(DWORD dwConnectionIndex);
void __stdcall OnDisconnectServer(DWORD dwConnectionIndex);
void __stdcall OnAcceptUser(DWORD dwConnectionIndex);
void __stdcall OnDisconnectUser(DWORD dwConnectionIndex);

void GameProcess();

void ButtonToggleProcessTime();
void OnCommand(char* szCommand);

void ReLoadBinFile();
void ReloadConfig();
void ReloadTest();
void ReloadTurn();
void ReloadNULL();
void ReloadFortWar();
void DevTest();

BOOL g_bCloseWindow = FALSE;
HWND g_hWnd;

// 

//std::string PacketEnDesStr=NetServer;

int	g_nHackCheckNum = 15;

int	g_nHackCheckWriteNum = 15;

int g_nServerMapNum = 15;

struct ProcessTime
{
	DWORD	mCurrentTick;
	DWORD	mPreviousTick;
	DWORD	mMaxSpace;
	float	mAverageSpace;
	DWORD	mSpace;
	DWORD	mTotalSpace;
	DWORD	mCount;
}
processTime;

// GameConfig define 2014-05-05
bool CServerSystem::ReloadWantedEnable;

int CServerSystem::MaxReSetTime;

int CServerSystem::ReSetPower;

WORD  CServerSystem::ReLevelPoint;

int CServerSystem::MinReSetLevel;

DWORD CServerSystem::ReSetMoney;

int  CServerSystem::ReStagePower;   //

DWORD CServerSystem::FirstStageMoney;

DWORD CServerSystem::SecondStageMoney;

WORD  CServerSystem::MarryLvSet;

DWORD CServerSystem::MarryMoney;

DWORD CServerSystem::MarryGold;

WORD CServerSystem::ShiFuLvSet;

DWORD CServerSystem::ShiTuMoney;

DWORD CServerSystem::ShiTuGold;

WORD CServerSystem::PaoDianLevel;

WORD CServerSystem::PaoDianReSet;

WORD CServerSystem::PaoDianTime;

DWORD CServerSystem::PaoDianMap;

WORD CServerSystem::m_Grow;

BOOL CServerSystem::GrowSwitch;

WORD CServerSystem::m_MonsterCount;

WORD CServerSystem::CanBadFame;
WORD CServerSystem::LootItemNum;
DWORD CServerSystem::BolehRobKarma;

MONEYTYPE CServerSystem::LuckerIngot;
MONEYTYPE CServerSystem::LuckerBubble;
MONEYTYPE CServerSystem::LuckerGold;
WORD CServerSystem::LuckerMode;
BOOL CServerSystem::LuckerEnable;


WORD CServerSystem::GrowRate;
BOOL CServerSystem::gControlLucker;

BOOL CServerSystem::gBossShout;
BOOL CServerSystem::gBossSubShout;

WORD CServerSystem::TidyItemTime;
WORD CServerSystem::TidyMallTime;

//DEVELOPER TEST AREA
DWORD CServerSystem::DevMaxGradeUpPoint;
DWORD CServerSystem::DevMaxAbilityPoint;
//DWORD CServerSystem::DevGuildTournament;
//END DEVELOPER TEST AREA

BOOL CServerSystem::RobExp;
float CServerSystem::RobExpPecent;

BOOL CServerSystem::gBloodStorm;

BOOL CServerSystem::gIgnoreLatestUpdate;


/*
HHOOK hHook;

LRESULT WINAPI CallWndProc(int nCode, WPARAM wParam, LPARAM lParam) 
{ 
    CWPSTRUCT* lpWp = (CWPSTRUCT*)lParam;

	if(lpWp->message == WM_CLOSE)
	{
		DefWindowProc(g_hWnd, lpWp->message, lpWp->wParam, lpWp->lParam);
		PostQuitMessage(0);
	}
    
    return CallNextHookEx(hHook, nCode, wParam, lParam); 
}
*/




// taiyo 
CServerSystem * g_pServerSystem = NULL;

CServerSystem::CServerSystem()
{
	CoInitialize(NULL);
	m_start = FALSE;

	g_pUserTable = new CUserTable;
	g_pUserTable->Init(2000);
	g_pServerTable = new CServerTable;
	g_pServerTable->Init(50);
	g_pObjectFactory = new CObjectFactory;
	g_pObjectFactory->Init();
	m_pGridSystem = new CGridSystem;
	m_pGridSystem->Init();

	m_Nation = eNATION_KOREA;	

	m_mpWaitExitPlayer = new CMemoryPoolTempl<WAIT_EXIT_PLAYER_INFO>;
	m_mpWaitExitPlayer->Init( 100, 10, "WAIT_EXIT_PLAYER_INFO" );
	
#ifdef _CHINA_LOCAL_
//é¤Œå?
 	m_NoP[0] = 0;
 	m_NoPCode = 'n'+ 'o'+ 'p';
//
#endif
	
	m_dwQuestTime = 0;
	m_bQuestTime = FALSE;

	m_bCompletionChrUpdate = FALSE;

//-- for test
	m_dwMainProcessTime = 0;
	memset( &m_ProcessInfo, 0, sizeof(m_ProcessInfo) );
	m_bCheckInfo = FALSE;
}

CServerSystem::~CServerSystem()
{
	CoUninitialize();

	PTRLISTPOS pos = m_listWaitExitPlayer.GetHeadPosition();
	WAIT_EXIT_PLAYER_INFO* pInfo = NULL;
	while( pos )
	{
		WAIT_EXIT_PLAYER_INFO* pInfo = (WAIT_EXIT_PLAYER_INFO*)m_listWaitExitPlayer.GetNext(pos);
		m_mpWaitExitPlayer->Free( pInfo );
	}
	m_listWaitExitPlayer.RemoveAll();

	SAFE_DELETE( m_mpWaitExitPlayer );
}

void CServerSystem::ReadServerConfig(WORD MapNum)
{
	/*MaxReSetTime=::GetPrivateProfileInt("ReSetConfig","MaxReSetTime",0,".\\GameConfig.ini");

	MinReSetLevel=::GetPrivateProfileInt("ReSetConfig","MinReSetLevel",0,".\\GameConfig.ini");

	ReSetMoney=::GetPrivateProfileInt("ReSetConfig","ReSetMoney",0,".\\GameConfig.ini");

	ReSetPower=::GetPrivateProfileInt("ReSetConfig","ReSetPower",0,".\\GameConfig.ini");

	FirstStageMoney=::GetPrivateProfileInt("ReStageConfig","FirstStageMoney",0,".\\GameConfig.ini");

	SecondStageMoney=::GetPrivateProfileInt("ReStageConfig","SecondStageMoney",0,".\\GameConfig.ini");
	
    ReStagePower =::GetPrivateProfileInt("ReStageConfig","ReStagePower",0,".\\GameConfig.ini");   // ç¾²å£½ 2015-02-15

	MarryLvSet=::GetPrivateProfileInt("MarryConfig","MarryLvSet",0,".\\GameConfig.ini");

	MarryMoney=::GetPrivateProfileInt("MarryConfig","MarryMoney",0,".\\GameConfig.ini");

	MarryGold=::GetPrivateProfileInt("MarryConfig","MarryGold",0,".\\GameConfig.ini");

	ShiFuLvSet=::GetPrivateProfileInt("ShiTuConfig","ShiFuLvSet",0,".\\GameConfig.ini");

	ShiTuMoney=::GetPrivateProfileInt("ShiTuConfig","ShiTuMoney",0,".\\GameConfig.ini");

	ShiTuGold=::GetPrivateProfileInt("ShiTuConfig","ShiTuGold",0,".\\GameConfig.ini");

	PaoDianLevel=::GetPrivateProfileInt("BubblePointConfig","MinimumLevel",0,".\\GameConfig.ini");

	PaoDianReSet=::GetPrivateProfileInt("BubblePointConfig","MinimumTurn",0,".\\GameConfig.ini");

	PaoDianTime=::GetPrivateProfileInt("BubblePointConfig","Timing",0,".\\GameConfig.ini");

    PaoDianMap=::GetPrivateProfileInt("BubblePointConfig","Map",0,".\\GameConfig.ini");

	char PaoDianMapFlag[20];

	// by the map number read the paodian value! 2014-05-30

	DWORD PaoDianTemp;

	sprintf(PaoDianMapFlag,"Map_%d",MapNum);

	PaoDianTemp=::GetPrivateProfileInt("BubblePointConfig",PaoDianMapFlag,0,".\\GameConfig.ini");

	if(PaoDianTemp>0)
	{
       PaoDianMap=PaoDianTemp;
	}*/

	MaxReSetTime	=::GetPrivateProfileInt("ReSetConfig","MaxReSetTime",0,".\\GameConfig.ini");
	MinReSetLevel	=::GetPrivateProfileInt("ReSetConfig","MinReSetLevel",0,".\\GameConfig.ini");
	ReSetMoney		=::GetPrivateProfileInt("ReSetConfig","ReSetMoney",0,".\\GameConfig.ini");
	ReSetPower		=::GetPrivateProfileInt("ReSetConfig","ReSetPower",0,".\\GameConfig.ini");	
	ReLevelPoint	=::GetPrivateProfileInt("ReSetConfig","ReLevelPoint",0,".\\GameConfig.ini");
	//ReCheRyukPoint	=::GetPrivateProfileInt("ReSetConfig","ReCheRyukPoint",0,".\\GameConfig.ini");

	FirstStageMoney	=::GetPrivateProfileInt("ReStageConfig","FirstStageMoney",0,".\\GameConfig.ini");
	SecondStageMoney=::GetPrivateProfileInt("ReStageConfig","SecondStageMoney",0,".\\GameConfig.ini");	
    ReStagePower	=::GetPrivateProfileInt("ReStageConfig","ReStagePower",0,".\\GameConfig.ini");

	MarryLvSet		=::GetPrivateProfileInt("MarryConfig","MarryLvSet",0,".\\GameConfig.ini");
	MarryMoney		=::GetPrivateProfileInt("MarryConfig","MarryMoney",0,".\\GameConfig.ini");
	MarryGold		=::GetPrivateProfileInt("MarryConfig","MarryGold",0,".\\GameConfig.ini");

	ShiFuLvSet		=::GetPrivateProfileInt("ShiTuConfig","ShiFuLvSet",0,".\\GameConfig.ini");
	ShiTuMoney		=::GetPrivateProfileInt("ShiTuConfig","ShiTuMoney",0,".\\GameConfig.ini");
	ShiTuGold		=::GetPrivateProfileInt("ShiTuConfig","ShiTuGold",0,".\\GameConfig.ini");

	PaoDianLevel	=::GetPrivateProfileInt("BubblePointConfig","MinimumLevel",0,".\\GameConfig.ini");
	PaoDianReSet	=::GetPrivateProfileInt("BubblePointConfig","MinimumTurn",0,".\\GameConfig.ini");
	PaoDianTime		=::GetPrivateProfileInt("BubblePointConfig","Timing",0,".\\GameConfig.ini");
    PaoDianMap		=::GetPrivateProfileInt("BubblePointConfig","Map",0,".\\GameConfig.ini");

	GrowSwitch		=::GetPrivateProfileInt("GrowConfig","Enable",0,".\\GameConfig.ini");
	m_Grow			=::GetPrivateProfileInt("GrowConfig","Grow",0,".\\GameConfig.ini");
	m_MonsterCount	=::GetPrivateProfileInt("GrowConfig","MonsterCount",0,".\\GameConfig.ini");
	GrowRate		=::GetPrivateProfileInt("GrowConfig","GrowRate",0,".\\GameConfig.ini");

	CanBadFame		=::GetPrivateProfileInt("RobSystemConfig","CanBadFame",0,".\\GameConfig.ini");
	LootItemNum		=::GetPrivateProfileInt("RobSystemConfig","LootItemNum",0,".\\GameConfig.ini");
	BolehRobKarma = ::GetPrivateProfileInt("RobSystemConfig", "BolehRobKarma", 0, ".\\GameConfig.ini");
	
	LuckerIngot		=::GetPrivateProfileInt("LuckyDrawConfig","LuckerIngot",0,".\\GameConfig.ini");
	LuckerBubble	=::GetPrivateProfileInt("LuckyDrawConfig","LuckerBubble",0,".\\GameConfig.ini");
	LuckerGold		=::GetPrivateProfileInt("LuckyDrawConfig","LuckerGold",0,".\\GameConfig.ini");

	LuckerMode		=::GetPrivateProfileInt("LuckyDrawConfig","LuckerMode",0,".\\GameConfig.ini");
	LuckerEnable	=::GetPrivateProfileInt("LuckyDrawConfig","LuckerEnable",0,".\\GameConfig.ini");

	gBossShout		=::GetPrivateProfileInt("ShoutSystem","BossShout",0,".\\GameConfig.ini");
	gBossSubShout	=::GetPrivateProfileInt("ShoutSystem","SubBossShout",0,".\\GameConfig.ini");

	TidyItemTime	=::GetPrivateProfileInt("TidyTimer","Inventory",0,".\\GameConfig.ini");
	TidyMallTime	=::GetPrivateProfileInt("TidyTimer","MallInventory",0,".\\GameConfig.ini");


//DEVELOPER TEST AREA
	DevMaxGradeUpPoint	=::GetPrivateProfileInt("DEVELOPER_TEMPORARY","DevMaxGradeUpPoint",0,".\\DEVELOPER_TEMPORARY.ini");
	DevMaxAbilityPoint	=::GetPrivateProfileInt("DEVELOPER_TEMPORARY","DevMaxAbilityPoint",0,".\\DEVELOPER_TEMPORARY.ini");

	//DevGuildTournament	=::GetPrivateProfileInt("DEVELOPER_TEMPORARY","DevGuildTournament",0,".\\DEVELOPER_TEMPORARY.ini");
//END DEVELOPER TEST AREA

	RobExp			=::GetPrivateProfileInt("RobExpSystem","Enable",0,".\\GameConfig.ini");
	RobExpPecent	=::GetPrivateProfileInt("RobExpSystem","ExpPercent",0,".\\GameConfig.ini");

	gBloodStorm = ::GetPrivateProfileInt("BloodStormInfo","Enable",0,".\\BloodStorm.ini");


	gIgnoreLatestUpdate = ::GetPrivateProfileInt("Set_1_To_Disable", "PowerUp", 0, ".\\LatestUpdate.ini");
	

	CServerSystem::gControlLucker = LuckerEnable;

	char PaoDianMapFlag[20];
	DWORD PaoDianTemp;
	sprintf(PaoDianMapFlag,"Map_%d",MapNum);

	PaoDianTemp		=::GetPrivateProfileInt("BubblePointConfig",PaoDianMapFlag,0,".\\GameConfig.ini");

	if(PaoDianTemp>0)
	{
       PaoDianMap=PaoDianTemp;
	}



	ReloadWantedEnable =::GetPrivateProfileInt("Experimental","EnableWantedLoop5Min",0,".\\GameConfig.ini");


	//g_Console.LOC(0xFFFF66,4, "GAMECONFIG.INI INIT OK!");

	FORTWARMGR->ClearDB(eAllStateAbove);



}

void CServerSystem::Start(WORD ServerNum)
{	
	m_bCheckProcessTime = FALSE;
	m_dwProcessCount = 0;

	m_wMapNum = ServerNum;

	g_nServerMapNum = ServerNum;

	SetMapKind(ServerNum);

	SetNation();


	SetReloadWantedMapInfo();

	BOOTMNGR->AddSelfBootList(MAP_SERVER, ServerNum, g_pServerTable);

	
	//const int temp = 2;
	//BOOTMNGR->AddSelfBootList(MAP_SERVER, ServerNum, g_pServerTable);

	//if(DungeonMGR->IsDungeon(ServerNum))
	{
		MENU_CUSTOM_INFO menu[] = 
		{			
			{ " ", ReloadNULL },
			{ "Fuben Time", ButtonToggleProcessTime }, 
			{"Reload Bin", ReLoadBinFile}, 
			{"Reload Config", ReloadConfig}, 
			{ "Reload Turn", ReloadTurn }, 
			{ "Reload FortWar", ReloadFortWar },
			{ "DevTest", DevTest }
				
			
			//{"ReloadTest",ReloadTest}
		};
		GAMERESRCMNGR->LoadMapKindInfo();
		char TitleText[128];
		DWORD Pid = GetCurrentProcessId();
		sprintf(TitleText, "MAP%d(%s) [PID_%d]", ServerNum, GetMap()->GetMapName(), Pid);

		if (FALSE == g_Console.Init(TitleText,sizeof(menu) / sizeof(*menu), menu, OnCommand))	MessageBox(0, "Console initializing Failed", 0, 0);
	}
	g_hWnd = GetActiveWindow();
	// load hackcheck
//	LoadHackCheck();

	//FILE* fpstart = fopen("Log/ServerStart.txt","w");
//#define STARTLOG(a)	fprintf(fpstart,#a);	a;
//	STARTLOG(CONDITIONMGR->Initial());
//	STARTLOG(CONDITIONMGR->LoadConditionList());
	SKILLMGR->Init();

	GAMERESRCMNGR->LoadMapChangeArea(GAMERESRCMNGR->m_MapChangeArea);
	GAMERESRCMNGR->LoadLoginPoint(GAMERESRCMNGR->m_LoginPoint);
	GAMERESRCMNGR->LoadMonsterList();
	GAMERESRCMNGR->LoadPetList();
	GAMERESRCMNGR->LoadPetRule();
	GAMERESRCMNGR->LoadPetBuffList();

	//SW070127 é¡«æ?é©?
//	STARTLOG(GAMERESRCMNGR->LoadTitanList());
//	STARTLOG(GAMERESRCMNGR->LoadTitanRule());
//	STARTLOG(GAMERESRCMNGR->LoadTitanSpellCostInfo());
//	STARTLOG(GAMERESRCMNGR->LoadTitanSkillLink());
//	STARTLOG(GAMERESRCMNGR->LoadTitanEquipEnduranceInfo());

	BOSSMONMGR->LoadSummonInfoList();
	BOSSMONMGR->LoadBossMonsterInfoList();

	//DUNGEONMGR->LoadSummonInfoList();
	//DUNGEONMGR->LoadBossMonsterInfoList();
	//DUNGEONMGR->LoadBossMonster("resource/InsDungeonScript/InsDGMissionTest");

	BOSSREWARDSMGR->LoadBossRewardsInfo();
	
	FIELDBOSSMONMGR->Init();

	//DISSOLUTIONMGR->Init();


	GAMERESRCMNGR->LoadNpcList(GAMERESRCMNGR->m_NpcList);
	GAMERESRCMNGR->LoadNpcRegen(REGENNPC_OBJ->m_NpcRegen);
	GAMERESRCMNGR->LoadStaticNpc();

	GAMERESRCMNGR->LoadHideNpcList();	
	GAMERESRCMNGR->LoadExpPoint();
	GAMERESRCMNGR->LoadPlayerxMonsterPoint();
	GAMERESRCMNGR->LoadFullMoonEventRate();
	GAMERESRCMNGR->LoadPaneltyTime();

	KYUNGGONGMGR->Init();

	GAMERESRCMNGR->LoadLevPenalty();
	PYOGUKMGR->LoadPyogukList();

	ITEMMGR->LoadItemList();
	ITEMMGR->LoadGoldShopItem();
	ITEMMGR->LoadItemLucker();
	ITEMMGR->LoadItemGrowSet();
	ITEMMGR->LoadGameCheckList();//kiv
	//ITEMMGR->LoadSignItemList();
	//ITEMMGR->LoadMixItemInfo();
	MIXMGR->LoadMixList();
	ITEMMGR->LoadSetItemOption();
	ITEMMGR->LoadReinforceItemInfo();
	ITEMMGR->LoadDealerItem();

	LOGINEVENTMGR->LoadLoginEventItemList();

	//STARTLOG(SAFEAREAMGR->LoadSafeAreaInfo());
	//STARTLOG(FBTIMERMGR->LoadFBTimerInfo());

	

//	STARTLOG(TITANITEMMGR->LoadTitanMixList());
//	STARTLOG(TITANITEMMGR->LoadTitanPartsKindList());
//	STARTLOG(TITANITEMMGR->LoadTitanUpgradeInfoList());
//	STARTLOG(TITANITEMMGR->LoadTitanBreakInfoList());

	
	GAMERESRCMNGR->LoadTacticStartInfoList();
	GAMERESRCMNGR->LoadTaticAbilityInfo();
	GAMERESRCMNGR->LoadAvatarEquipList();	
	GAMERESRCMNGR->LoadShopItemDupList();
	GAMERESRCMNGR->LoadUniqueItemOptionList();	
	GAMERESRCMNGR->LoadUniqueItemMixList();		
	GAMERESRCMNGR->LoadItemChangeRateofLv(); 
	GAMERESRCMNGR->LoadItemChangeRateofAttrib(); 
	GAMERESRCMNGR->LoadFlagNpcInfo();
	GAMERESRCMNGR->LoadTitanMapMonsterDropInfo();
	MON_SPEECHMGR->LoadMonSpeechInfoList();
	RAREITEMMGR->LoadRareItemOptionInfo();
	RAREITEMMGR->LoadRareItemInfo();
	RAREITEMMGR->LoadRareValueProbList();
	PET_SPEECHMGR->LoadPetSpeechInfoList();
	GAMERESRCMNGR->LoadBobusangNpcInfo();
	ABILITYMGR->Init();
	ABILITYMGR->AddUpdater(CAbilityUpdater_CheckLevelUp::GetInstance());
	ABILITYMGR->AddUpdater(CAbilityUpdater_Database::GetInstance());
	ABILITYMGR->AddUpdater(CAbilityUpdater_ApplyData::GetInstance());
	ITEMDROP_OBJ->Init();
	SURYUNMGR;
	m_Map.InitMap(ServerNum);	
	PATHMGR->SetMap(&m_Map, ServerNum, m_Map.GetTileWidth(ServerNum));
	PKMGR->Init( m_Map.IsPKAllow() );	
	GUILDWARMGR->Init();
	LoadEventRate("./Resource/Server/droprate.bin");
	LoadPartyPlustime("./Resource/PartyPlustimeInfo.bin");
	LoadFameConfig("./Resource/FameList.bin");
	QUESTMGR->LoadQuestScript();
	QUESTREGENMGR->LoadData();	
	CHANGEITEMMGR->Init();
	SWPROFITMGR->LoadMapInfo( ServerNum );
	GAMERESRCMNGR->LoadNomalClothesSkinList();
	GAMERESRCMNGR->LoadCostumeSkinList();
	
	ITEMLIMITMGR->LoadItemLimitInfo();
	GAMEEVENTMGR->LoadGameEventInfo();
	AUTONOTEMGR->Init();
	FORTWARMGR->Init();
	//ClearFortWarData();
	GAMERESRCMNGR->LoadRebornSetting();

	
	INSDUNGEONREGENDATAMGR->LoadInsDungeonRegenList();
	INSDUNGEONMGR->Init();

	//SetWindowText(g_hWnd, TitleText);

	int i=0 ;
	for(i=0 ; i<MP_MAX ; ++i)
		g_pServerMsgParser[i] = NULL;
	for(i=0 ; i<MP_MAX ; ++i)
		g_pUserMsgParser[i] = NULL;
	g_pServerMsgParser[MP_SERVER] = MP_MAPSERVERMsgParser;
	g_pServerMsgParser[MP_POWERUP] = MP_POWERUPMsgParser;
	g_pServerMsgParser[MP_CHAR] = MP_CHARMsgParser;
	g_pServerMsgParser[MP_MAP] = NULL;
	g_pServerMsgParser[MP_ITEM] = MP_ITEMMsgParser;
	g_pServerMsgParser[MP_ITEMEXT] = MP_ITEMMsgParserExt;
	g_pServerMsgParser[MP_CHAT] = MP_CHATMsgParser;
	g_pServerMsgParser[MP_USERCONN] = MP_USERCONNMsgParser;
	g_pServerMsgParser[MP_MOVE] = MP_MOVEMsgParser;
	g_pServerMsgParser[MP_MUGONG] = MP_MUGONGMsgParser;
	g_pServerMsgParser[MP_AUCTIONBOARD] = NULL;
	g_pServerMsgParser[MP_CHEAT] = MP_CHEATMsgParser;
	g_pServerMsgParser[MP_QUICK] = MP_QUICKMsgParser;
	g_pServerMsgParser[MP_PACKEDDATA] = NULL;
	g_pServerMsgParser[MP_PARTY] = MP_PARTYMsgParser;
	g_pServerMsgParser[MP_PEACEWARMODE] = MP_PEACEWARMODEMsgParser;
	g_pServerMsgParser[MP_UNGIJOSIK] = MP_UNGIJOSIKMsgParser;
	g_pServerMsgParser[MP_AUCTION] = NULL;
	g_pServerMsgParser[MP_TACTIC] = MP_TACTICMsgParser;
	g_pServerMsgParser[MP_SKILL] = MP_SkillMsgParser;
	g_pServerMsgParser[MP_PYOGUK] = MP_PYOGUKMsgParser;
	g_pServerMsgParser[MP_BATTLE] = MP_BattleMsgParser;
	g_pServerMsgParser[MP_CHAR_REVIVE] = MP_REVIVEMsgParser;
	g_pServerMsgParser[MP_WANTED] = MP_WANTEDMsgParser;
	g_pServerMsgParser[MP_JOURNAL] = MP_JOURNALMsgParser;
	g_pServerMsgParser[MP_SURYUN] = MP_SURYUNMsgParser;

//KES EXCHANGE 030922
	g_pServerMsgParser[MP_EXCHANGE] = MP_EXCHANGEMsgParser;
	
// LBS 03.09.25 ä½¿å?ea?³io
	g_pServerMsgParser[MP_STREETSTALL] = MP_STREETSTALLMsgParser;
	g_pServerMsgParser[MP_OPTION] = MP_OPTIONMsgParser;

// LBS 03.12.24 NPCï¹?IAC ?³ioE?³IAUï¹?e
	g_pServerMsgParser[MP_NPC] = MP_NPCMsgParser;
	
	g_pServerMsgParser[MP_MURIMNET] = MP_MurimnetMsgParser;

	g_pServerMsgParser[MP_QUEST] = MP_QUESTMsgParser;
	g_pServerMsgParser[MP_MORNITORMAPSERVER] = MP_MonitorMsgParser;

	g_pServerMsgParser[MP_PK] = MP_PKMsgParser;
	g_pServerMsgParser[MP_HACKCHECK] = MP_HACKCHECKMsgParser;
	g_pServerMsgParser[MP_SOCIETYACT] = MP_SOCIETYACTMsgParser;
	g_pServerMsgParser[MP_GUILD] = MP_GUILDMsgParser;
	g_pServerMsgParser[MP_GUILD_FIELDWAR] = MP_GUILDFIELDWARMsgParser;
	g_pServerMsgParser[MP_PARTYWAR] = MP_PARTYWARMsgParser;
	g_pServerMsgParser[MP_GTOURNAMENT] = MP_GTOURNAMENTMsgParser;
	g_pServerMsgParser[MP_GUILD_UNION] = MP_GUILUNIONMsgParser;
	g_pServerMsgParser[MP_SIEGEWAR] = MP_SIEGEWARMsgParser;
	g_pServerMsgParser[MP_SIEGEWAR_PROFIT] = MP_SIEGEWARPROFITMsgParser;

	// Weather System
	g_pServerMsgParser[MP_WEATHER] = MP_WEATHERMsgParser;

	// Pet
	g_pServerMsgParser[MP_PET] = MP_PETMsgParser;

	// SurvivialMode
	g_pServerMsgParser[MP_SURVIVAL] = MP_SURVIVALMsgParser;

	// Bobusang
	g_pServerMsgParser[MP_BOBUSANG] = MP_BOBUSANGMsgParser;

	//Titan
	g_pServerMsgParser[MP_TITAN] = MP_TITANMsgParser;

	// ItemLimit
	g_pServerMsgParser[MP_ITEMLIMIT] = MP_ITEMLIMITMsgParser;

	// Autonote
	g_pServerMsgParser[MP_AUTONOTE] = MP_AUTONOTEMsgParser;

	// fortwar
	g_pServerMsgParser[MP_FORTWAR] = MP_FORTWARMsgParser;

	// 2014-05-09 Marry Procotol define 

	g_pServerMsgParser[MP_MARRY] = MP_MARRYMsgParser;

	// 2014-05-25 shitu  Procotol define

    g_pServerMsgParser[MP_SHITU] = MP_SHITUMsgParser;

	g_pServerMsgParser[MP_GUILDWAR]	   = MP_GuildWarMPMsgParser;

	g_pServerMsgParser[MP_DUNGEON] = MP_DUNGEON_MsgParser;

	//g_pServerMsgParser[MP_FBTIMER]=MP_FBTIMERMsgParser;
	g_pServerMsgParser[MP_INSDUNGEON] = MP_INSDUNGEONMsgParser;

	g_pServerMsgParser[MP_MALLLIST] = MP_MALLLISTMsgParser;

	CHANNELSYSTEM->Init( ServerNum );

	g_pAISystem = new CAISystem;
	//if(false == g_pServerSystem->GetMap()->IsMapKind(eInsDungeon) )
	{		
		g_pAISystem->LoadAIGroupList();
	}

	SURYUNREGENMGR->LoadSuryunRegenList();
	
	CUSTOM_EVENT ev[3];

	ev[0].dwPeriodicTime	= 100;
	ev[0].pEventFunc		= GameProcess;
	ev[1].dwPeriodicTime	= 0;
	ev[1].pEventFunc		= ProcessDBMessage;
	ev[2].dwPeriodicTime	= 1000*60*5;
	ev[2].pEventFunc		= _5minGameProcess;
	
	DESC_NETWORK desc;
	desc.OnAcceptServer = OnAcceptServer;
	desc.OnDisconnectServer = OnDisconnectServer;
	desc.OnAcceptUser = OnAcceptUser;
	desc.OnDisconnectUser = OnDisconnectUser;
	desc.OnRecvFromServerTCP = ReceivedMsgFromServer;
	desc.OnRecvFromUserTCP = ReceivedMsgFromUser;
	desc.dwCustomDefineEventNum = (sizeof(ev) / sizeof(*ev)); //3;
	desc.pEvent = ev;

	
	desc.dwMainMsgQueMaxBufferSize = 20480000;
	desc.dwMaxServerNum = 50;
	desc.dwMaxUserNum = 10;
	desc.dwServerBufferSizePerConnection = 512000;
	desc.dwServerMaxTransferSize = 65535;
	desc.dwUserBufferSizePerConnection = 65535;
	desc.dwUserMaxTransferSize = 65535;
	desc.dwConnectNumAtSameTime = 100;
	desc.dwFlag = 0;

	g_Network.Init(&desc);
	
	if(!BOOTMNGR->StartServer(&g_Network, g_pServerTable->GetSelfServer()))
	{
		//ASSERT(0);
	}
	if(!BOOTMNGR->ConnectToMS(&g_Network, g_pServerTable))
	{
		SERVERINFO info;
		info.wServerKind = MONITOR_SERVER;
		OnConnectServerFail(&info);		
	}

	//////////////////////////////////////////////////////////////////////////
	// DB ?œuä½¿å?d
	DWORD maxthread = 4;
	DWORD maxqueryinsametime = 1024;
	/*FILE* fp = fopen("MapDBInfo.txt","r");
	if(fp)
	{
		fscanf(fp,"%d %d",&maxthread,&maxqueryinsametime);
		fclose(fp);
	}*/
	if(g_DB.Init(maxthread,maxqueryinsametime,FALSE) == FALSE)
		MessageBox(NULL,"DataBase Initializing Failed",0,0);


	GUILDFIELDWARMGR->Init();
	PARTYWARMGR->Init();
	JACKPOTMGR->Init(ServerNum);
	JACKPOTMGR->LoadJackpotInfo();
	SIEGEWARMGR->Init();
	GTMGR->Init();
	WEATHERMGR->Init();
	SVVMODEMGR->Init();	
	//DungeonMGR->Init();
	WantedLoad(0);
	REINFORCEMGR->Init();
	DISSOLUTIONMGR->Init();
	BOBUSANGMGR->BobusangMgr_Init();
	MAPITEMDROP_OBJ->Init();


	m_dwQuestTime = gCurTime;
	m_bQuestTime = FALSE;

	MiniDumper md(MiniDumper::DUMP_LEVEL_0);

	g_Console.LOG(4, "-----------   Map Number %d     -----------------", GAMERESRCMNGR->GetLoadMapNum());
    ReadServerConfig(ServerNum);

	//DungeonMGR->LoadTestNewBin(ServerNum);

	g_bReady = TRUE;

//	if(FALSE==JACKMGR->CheckSumInit(g_hWnd))
//	{
//		g_bReady = false;
//		SetStart(FALSE);
//	}
//	else
//		g_bReady = true;

//	fclose(fpstart);
	g_Console.WaitMessage();
}

void CServerSystem::End()
{
	//g_bReady = FALSE;
	SetStart(FALSE);

	
	if( TRUE != GetCharUpdateCompletion() )
	{
		HandlingBeforeServerEND();
	}

	SKILLMGR->Release();
	BOBUSANGMGR->BobusangMgr_Release();

	g_Network.Release();
	g_Console.Release();

	BATTLESYSTEM->Release();

	SAFE_DELETE( g_pUserTable );
	SAFE_DELETE( g_pServerTable );
	SAFE_DELETE( g_pAISystem );
	SAFE_DELETE( g_pObjectFactory );
	SAFE_DELETE( m_pGridSystem );

	m_Map.Release();
	CoFreeUnusedLibraries();
}

void CServerSystem::Process()
{
	
	QueryPerformanceFrequency(&m_freq);
	processTime.mPreviousTick	= GetTickCount();

	if(g_bCloseWindow)
	{
		SendMessage(g_hWnd, WM_CLOSE, 0,0);
		SetStart(FALSE);
	}

	if(GetStart() == FALSE)
		return;

	
	
	MHTIMEMGR_OBJ->Process();

	
	
	//JACKMGR->Process();
//	CONDITIONMGR->Process();


#ifdef _CHINA_LOCAL_
	if( m_NoPCode != 'n'+ 'o'+ 'p' )
	{
		strcat( m_NoP, "00010011101010101110101010001" );
	}
#endif

	if(!g_pUserTable)
		return;

//--for test
//	Sleep(2000);
//---test
	static DWORD dwLastProcessTime = 0;

//	if( m_dwMainProcessTime > 250 )
//	{
//		m_dwMainProcessTime = 0;
//		dwLastProcessTime = gCurTime;
//		return;
//	}
	
	m_pGridSystem->GridProcess();

	// quest time
	if( gCurTime > m_dwQuestTime+60000 )
	{
		m_dwQuestTime = gCurTime;
		m_bQuestTime = TRUE;
	}

	//JACKMGR->Process();

	g_pUserTable->SetPositionUserHead();
	CObject* pObject;
	while(pObject = (CObject*)g_pUserTable->GetUserData())
	{
		if(pObject->GetInited() == FALSE)
			continue;
		if(pObject->GetObjectKind() == eObjectKind_Pet )
			continue;	//è¼¿æª£???®ç??®è¶ç¸‘æ†® ç±€?? è¬™æ???

		OBJECTSTATEMGR_OBJ->StateProcess(pObject);
		CCharMove::MoveProcess(pObject);

		pObject->SpecialStateProcess(gTickTime);
		CCharacterCalcManager::UpdateLife(pObject);

		if(pObject->GetObjectKind() == eObjectKind_Player)
		{
			CPlayer* pPlayer = (CPlayer*)pObject;
			CCharacterCalcManager::UpdateShield(pPlayer);
			CCharacterCalcManager::ProcessShield(pPlayer);
			CCharacterCalcManager::ProcessLife(pPlayer);
			CCharacterCalcManager::UpdateNaeRyuk(pPlayer);	
			CCharacterCalcManager::ProcessNaeRyuk(pPlayer);
 
			pPlayer->QuestProcess();
			pPlayer->CheckMussangTime();
			pPlayer->MallMoneyProcess();   // MallMoney Process is Set 2014-05-30 !
			//pPlayer->FBProcess();          //weiye FB?µæ?è¼›æ?ç¡’ä¿´      2018-02-06
			//pPlayer->LoginEventProcess();
			LOGINEVENTMGR->Process(pPlayer);


			//QueryPerformanceCounter(&m_DungeonProc[0]);
			//DungeonMGR->Process(pPlayer);
			//{
			//	processTime.mCurrentTick	=	GetTickCount();
			//	processTime.mSpace			=	processTime.mCurrentTick - processTime.mPreviousTick;
			//	processTime.mMaxSpace		=	max( processTime.mSpace, processTime.mMaxSpace );
			//	processTime.mTotalSpace		+=	processTime.mSpace;
			//	processTime.mAverageSpace	=	float( processTime.mTotalSpace ) / ++processTime.mCount;
			//}
			

			if( m_bQuestTime )
			{
				// add quest event
				QUESTMGR->AddQuestEvent( pPlayer, &CQuestEvent( eQuestEvent_Time, 0, 0 ) );
			}
		}
		else if(pObject->GetObjectKind() == eObjectKind_Tactic)
		{
			TACTICMGR->TacticProcess((CTacticObject*)pObject);
		}
		else if(pObject->GetObjectKind() == eObjectKind_BossMonster)
		{
			((CBossMonster*)pObject)->Process();
			CCharacterCalcManager::UpdateShield(pObject);
			CCharacterCalcManager::ProcessShieldBoss((CBossMonster *)pObject);
			CCharacterCalcManager::ProcessLifeBoss((CBossMonster *)pObject);
		}
		else if(pObject->GetObjectKind() == eObjectKind_Npc)
		{
			CNpc* pNpc = (CNpc*)pObject;
			pNpc->Process();
		}
		
	}

	// ?·è??¾è¶ - 05.12 æªœè???
	FIELDBOSSMONMGR->Process();

	//db?½ç? ?³èµ ?­è‘¬??æ°ˆæ£»è³?..
	g_DB.ProcessingQueue();	//ProcessingDBMessage()???¾å????…è¼¿è³?æ©«é›²æ¢?
//	g_DB.ProcessingDBMessage();
	
	SKILLMGR->Process();
	TACTICMGR->ProcessGarbageTactic();
	BATTLESYSTEM->Process();


	if(g_pAISystem)
		g_pAISystem->Process();
	//MonsterStateProc();		// ï¹Œï?o?œo?œï?AI ?³ioAA ï¹ iä½¿oï¹Œï?ï¹Œcï¹Œï?A CO?œuo
	
	LOOTINGMGR->ProcessTimeOutCheck();	//?œoA?³è??³IAI ï¹Œï?Uï¹ iE PK?³ï?c?œâ•³A ~?³ï?aCI?³å?a
	QUESTMGR->Process();

	EVENTMAPMGR->Process();

	PARTYWARMGR->Process();

//	MEMORYCHK->Process();

	//SW050719
//COMMENT:JACKPOT
	JACKPOTMGR->Process();

	//SW060719 ?¥ï??‡æª£??
	GUILDMGR->GUILDMGR_Process();

	// Guild Tournament
	GTMGR->Process();


	// SiegeWar
	SIEGEWARMGR->Process();
	SWPROFITMGR->ProcessDBUpdateProfitMoney();

	// Weather System

	WEATHERMGR->Process();


	//SW061019 ?®å¤¥æªœå?è³…è?
	//if( CheckMapKindIs(eSurvival) )
	if( g_pServerSystem->GetMap()->IsMapKind(eSurvivalMap) )
		SVVMODEMGR->Process();

	AUTONOTEMGR->Process();

	FORTWARMGR->ProcessFortWar();
	FORTWARMGR->ProcessDBUpdateProfitMoney();

	ProcessWaitExitPlayer();

	PARTYMGR->Process();

	m_bQuestTime = FALSE;

//--- Test
	m_dwMainProcessTime = gCurTime - dwLastProcessTime;
	dwLastProcessTime = gCurTime;

	if( m_bCheckInfo && m_ProcessInfo.Count < 5 )
	{
		m_ProcessInfo.ProcessInfo.ServerProcesstime[m_ProcessInfo.Count] = m_dwMainProcessTime;
		++m_ProcessInfo.Count;

		if( m_ProcessInfo.Count > 4 )
		{
            if( m_ProcessInfo.ProcessInfo.GameQueryReturntime && m_ProcessInfo.ProcessInfo.LogQueryReturntime )
			{
				CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser( m_ProcessInfo.dwObjectID );
				if( pPlayer )
					pPlayer->SendMsg( &m_ProcessInfo, sizeof(m_ProcessInfo) );

				m_bCheckInfo = FALSE;
			}
		}
	}
	QueryPerformanceCounter(&m_DungeonProc[0]);
	if (m_bCheckProcessTime)
	{
		static DWORD dwLastPutLogTime = gCurTime;
		double fTimeDungeonProc = (double)(m_DungeonProc[1].QuadPart - m_DungeonProc[0].QuadPart) / m_freq.QuadPart;
		m_fAvrDungeonProc += fTimeDungeonProc;
		m_dwProcessCount++;
		if (m_fTimeDungeonProc < fTimeDungeonProc)
			m_fTimeDungeonProc = fTimeDungeonProc;
		if (gCurTime > dwLastPutLogTime + 2000)
		{
			//if (DungeonMGR->IsDungeon(g_pServerSystem->GetMapNum()))
			if (GetMap()->IsMapKind(eInsDungeon))
			{
				g_Console.LOG(4, "DGInfo: [%d:Players], [%d:Mobs], [%d:Npcs], [%d:Obj]",
							  g_pUserTable->GetUserCount(),
							  g_pUserTable->GetMonsterCount(),
							  g_pUserTable->GetNpcCount(),
							  g_pUserTable->GetExtraCount());
			}
			dwLastPutLogTime = gCurTime;
		}
	}
	GUILDWARMGR->Process();

	//if (g_pServerSystem->GetMap()->IsMapKind(eInsDungeon))
		//DungeonMGR->Process();

	QueryPerformanceCounter(&m_DungeonProc[0]);

	/*if( g_pServerSystem->GetMap()->IsMapKind(eInsDungeon) )//kiv	
	{
		processTime.mCurrentTick	=	GetTickCount();
		processTime.mSpace			=	processTime.mCurrentTick - processTime.mPreviousTick;
		processTime.mMaxSpace		=	max( processTime.mSpace, processTime.mMaxSpace );
		processTime.mTotalSpace		+=	processTime.mSpace;
		processTime.mAverageSpace	=	float( processTime.mTotalSpace ) / ++processTime.mCount;
	}*/

	if(m_bCheckProcessTime)
	{
		static DWORD dwLastPutLogTime = gCurTime;
		double fTimeDungeonProc = (double)(m_DungeonProc[1].QuadPart - m_DungeonProc[0].QuadPart) / m_freq.QuadPart;

		m_fAvrDungeonProc += fTimeDungeonProc;
		m_dwProcessCount++;

		if(m_fTimeDungeonProc < fTimeDungeonProc)
			m_fTimeDungeonProc = fTimeDungeonProc;

		if(gCurTime > dwLastPutLogTime + 2000)
		{
			/*if(DungeonMGR->IsDungeon(g_pServerSystem->GetMapNum()))
			{
				g_Console.LOG(4,"DGInfo: [%d:Rooms], [%d:Players], [%d:Mobs], [%d:Npcs], [%d:Obj]",
					DungeonMGR->GetDungeonNum(),
					DungeonMGR->GetPlayerNum(),
					DungeonMGR->GetMonsterNum(),
					DungeonMGR->GetNpcNum(),
					DungeonMGR->GetExtraNum());
			}*/
			dwLastPutLogTime = gCurTime;
		}
	}
	JACKMGR->Process();

	INSDUNGEONMGR->Process();
}

void CServerSystem::_5minProcess()
{
	GUILDMGR->GUILDMGR_5MinProcess();

	if(CServerSystem::ReloadWantedEnable && GetReloadWantedMapInfo(g_pServerSystem->GetMapNum()))
	{
		WantedLoad(0);
		g_Console.LOG( 4, "[5min loop] Wanted reloaded %s",GetCurTimeToString());
	}

	if( g_pServerSystem->GetMap()->IsMapKind(eTournament) )
	{
		GTMGR->CheckDateforFlagNPC();
	}

	if( g_pServerSystem->GetMap()->IsMapKind(eSiegeWarMap) )
	{
		SIEGEWARMGR->CheckDateforFlagNPC();
	}
	//	FAMEMGR->Process();
//	STATISTICSMGR->Process();
}

void CServerSystem::SetStart( BOOL state )
{
	m_start = state;
	if( m_start )
	{
		g_Console.LOG( 4, "-----------   MAP SERVER LOAD DATA FROM DB  -----------------");
	}
}

void CServerSystem::SetMapKind(WORD wMapNum)
{
	if(wMapNum == SURVIVALMAP)
		m_iMapKind = eSurvival;
	else if(wMapNum == Tournament)
		m_iMapKind = eGTMap;
	else if(wMapNum == nakyang_siege)
		m_iMapKind = eSGMap;
	else
		m_iMapKind = eNormalMap;
}

BOOL CServerSystem::CheckMapKindIs(int eMapkind)
{
	return (m_iMapKind == eMapkind);
}

CPlayer* CServerSystem::AddPlayer(DWORD dwPlayerID,DWORD dwAgentNum,DWORD UniqueIDinAgent,int ChannelNum, eUSERLEVEL userLevel )
{
	CObject* pPreObj = g_pUserTable->FindUser(dwPlayerID);
	if(pPreObj)
	{
		pPreObj->SetNotInited();
		RemovePlayer(dwPlayerID);
	}

	/*DWORD num = CHANNELSYSTEM->GetChannelID(ChannelNum);
	if( QUESTMAPMGR->IsQuestMap() )
		num = CHANNELSYSTEM->CreateQuestMapChannel();
	else if(DungeonMGR->IsDungeon(g_pServerSystem->GetMapNum()))
	{
		num = DungeonMGR->GetChannelIDFromReservationList(dwPlayerID);
	}
	else
		num = CHANNELSYSTEM->GetChannelID(ChannelNum);

	if(0 == num)
	{
		const int temporaryChannel = 1;
		num = temporaryChannel;
	}*/

	BASEOBJECT_INFO binfo;
	memset(&binfo, 0, sizeof(binfo));
	binfo.dwObjectID = dwPlayerID;
	CPlayer* pPlayer = (CPlayer*)g_pObjectFactory->MakeNewObject(eObjectKind_Player,dwAgentNum, &binfo);

	if (!CHANNELSYSTEM->IsInsDungeonMap())
	{
		DWORD num = 0;
		if (QUESTMAPMGR->IsQuestMap())
			num = CHANNELSYSTEM->CreateQuestMapChannel();
		else
			num = CHANNELSYSTEM->GetChannelID(ChannelNum);
		if (!num)	return NULL;
		pPlayer->SetBattleID(num);
		pPlayer->SetGridID(num);
		pPlayer->SetChannelID(num);
	}

	pPlayer->SetUniqueIDinAgent(UniqueIDinAgent);	
	/*pPlayer->SetBattleID(num);
	pPlayer->SetGridID(num);
	pPlayer->SetChannelID(num);*/

	pPlayer->SetInitState(PLAYERINITSTATE_ONLY_ADDED,0);

	CHANNELSYSTEM->IncreasePlayerNum(pPlayer->GetChannelID());

	g_pUserTable->AddUser(pPlayer,pPlayer->GetID());

	return pPlayer;
}

/*
CPlayer* CServerSystem::AddPlayer(DWORD dwPlayerID,DWORD dwAgentNum,DWORD UniqueIDinAgent,int ChannelNum)
{
	CObject* pPreObj = g_pUserTable->FindUser(dwPlayerID);
	ASSERT(pPreObj == NULL);
	if(pPreObj)
	{
		pPreObj->SetNotInited();
		RemovePlayer(dwPlayerID);
	}

	DWORD num = CHANNELSYSTEM->GetChannelID(ChannelNum);
	if(!num) 
		return NULL;

	BASEOBJECT_INFO binfo;
	memset(&binfo,0,sizeof(binfo));
	binfo.dwObjectID = dwPlayerID;
	CPlayer* pPlayer = (CPlayer*)g_pObjectFactory->MakeNewObject(eObjectKind_Player,dwAgentNum, &binfo);
	pPlayer->SetUniqueIDinAgent(UniqueIDinAgent);
	pPlayer->SetBattleID(num);
	pPlayer->SetGridID(num);
	pPlayer->SetChannelID(num);
	pPlayer->SetInitState(PLAYERINITSTATE_ONLY_ADDED,0);

	CHANNELSYSTEM->IncreasePlayerNum(pPlayer->GetChannelID());

	g_pUserTable->AddUser(pPlayer,pPlayer->GetID());
//	g_Console.LOG(4, "User Connected : ID %d, Total(%d)", pPlayer->GetID(), g_pUserTable->GetUserCount());
	return pPlayer;
}
*/

CPlayer* CServerSystem::InitPlayerInfo(BASEOBJECT_INFO* pBaseObjectInfo,CHARACTER_TOTALINFO* pTotalInfo,HERO_TOTALINFO* pHeroInfo)
{
	CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser(pBaseObjectInfo->dwObjectID);
	if(pPlayer == NULL)
		return NULL;
	pBaseObjectInfo->BattleID = pPlayer->GetBattleID();
	ASSERT(pPlayer->GetID() == pBaseObjectInfo->dwObjectID);
	pPlayer->Init(eObjectKind_Player,pPlayer->GetAgentNum(),pBaseObjectInfo);
	pPlayer->InitCharacterTotalInfo(pTotalInfo);
	pPlayer->InitHeroTotalInfo(pHeroInfo);

	return pPlayer;
}

CTitan*	CServerSystem::MakeTitan( CPlayer* pOwner )
{
	CTitan* pTitan = NULL;
	BASEOBJECT_INFO BaseInfo;
	pTitan = (CTitan*)g_pObjectFactory->MakeNewObject(eObjectKind_Titan, 0, &BaseInfo);
	
	return pTitan;
}

TITAN_TOTALINFO* CServerSystem::MakeTitanInfo( TITAN_TOTALINFO* pInfo )
{
	TITAN_TOTALINFO* pNewInfo = NULL;
	pNewInfo = g_pObjectFactory->MakeTitanInfo(pInfo);

	return pNewInfo;
}

void CServerSystem::RemoveTitanInfo( TITAN_TOTALINFO* pInfo )
{
	g_pObjectFactory->ReleaseTitanInfo(pInfo);	
}

void CServerSystem::CreateNewPet( CPlayer* pMaster, DWORD dwItemIdx, DWORD SummonItemDBIdx, WORD wPetKind, WORD PetGrade /*= 1*/ )
{
	BASE_PET_LIST*	pPetListInfo;
	pPetListInfo = GAMERESRCMNGR->GetPetListInfo(wPetKind);
	if( NULL == pPetListInfo )
		return;

	PET_TOTALINFO TotalInfo;
	TotalInfo.PetSummonItemDBIdx = SummonItemDBIdx;			//å¬´æ??±æ? DBID	//ç¾…å???æ©¾é™½ ?®ï?.
	TotalInfo.PetKind		= wPetKind;	//è¬™ç?
	TotalInfo.PetStamina	= pPetListInfo->StaminaMax[PetGrade-1];	//?¶ç??é‡­ è­†æ?çº?
	TotalInfo.PetFriendly = GAMERESRCMNGR->GetPetRule()->DefaultFriendship;		//?¦æ™¦?„è€€å¡µç´«
	TotalInfo.PetGrade	= PetGrade;	//æ¬½å?
	TotalInfo.PetMasterUserID	= pMaster->GetUserID();	//?·è?? ?·è? DBå©¦æº¼
	TotalInfo.bAlive		= TRUE;
	TotalInfo.bSummonning	= FALSE;
	TotalInfo.bRest			= TRUE;

	PetInsertToDB(pMaster->GetID(), pMaster->GetUserID(), dwItemIdx, &TotalInfo);
}

CPet* CServerSystem::AddPet( CPlayer* pMaster, DWORD dwPetObjID, PET_TOTALINFO* pTotalInfo, VECTOR3* pPos )
{
	CPet* pPet = NULL;

	if(NULL == pMaster)
		return NULL;
	if(NULL == pTotalInfo)
		return NULL;

	if(pMaster->GetUserID() != pTotalInfo->PetMasterUserID)
	{
		ASSERT(0);
		return NULL;
	}

	BASEOBJECT_INFO BaseInfo;
	BASE_PET_LIST*	pPetListInfo;

	pPetListInfo = GAMERESRCMNGR->GetPetListInfo(pTotalInfo->PetKind);
	if(NULL == pPetListInfo)
		return NULL;

	BaseInfo.dwObjectID = dwPetObjID;
	SafeStrCpy(BaseInfo.ObjectName, pPetListInfo->Name, MAX_NAME_LENGTH+1);

	pPet = (CPet*)g_pObjectFactory->MakeNewObject(eObjectKind_Pet, 0, &BaseInfo);
	pPet->InitPet(pTotalInfo);

	pPet->SetGridID(pMaster->GetGridID());
	
	pPet->SetMaster(pMaster);
	//pMaster->SetCurPet(pPet);

	pPet->SetInited();
	CCharMove::SetPosition(pPet, pPos);
	CCharMove::InitMove(pPet, pPos);

	g_pUserTable->AddUser(pPet, pPet->GetID());

	return pPet;
}


//CPet* CServerSystem::AddPet(DWORD dwPetObjID, CPlayer* pMaster, BASEOBJECT_INFO* pBaseObjectInfo, PET_TOTALINFO* pTotalInfo, WORD wPetKind, int flag /*= ePET_FROM_DB*/)
//{
////	ASSERT(g_pUserTable->FindUser(pBaseObjectInfo->dwObjectID) == NULL);
//
//	CPet* pPet = NULL;
//
//	if(flag == ePET_FROM_ITEM)	//ç¾??…æ’©.
//	{
////		PET_TOTALINFO	PetInfo;
//		BASEOBJECT_INFO BaseInfo;
//		BASE_PET_LIST*	pPetListInfo;
//
//		pPetListInfo = GAMERESRCMNGR->GetPetListInfo(wPetKind);
//		if(pPetListInfo == NULL)
//			return NULL;
//
//		BaseInfo.dwObjectID = dwPetObjID;
//		SafeStrCpy(BaseInfo.ObjectName, pPetListInfo->Name, MAX_NAME_LENGTH+1);
//
//		PetInfo.PetSummonItemID = SummonItemID;			//å¬´æ??±æ? DBID	//ç¾…å???æ©¾é™½ ?®ï?.
//		PetInfo.PetKind		= wPetKind;	//è¬™ç?
//		PetInfo.PetStamina	= pPetListInfo->StaminaMax[0];	//?¶ç??é‡­ è­†æ?çº?
//		PetInfo.PetFriendly = PET_DEFAULT_FRIENDLY;		//?¦æ™¦?„è€€å¡µç´«
//		PetInfo.PetGrade	= 1;	//1æ¬½å?
//		PetInfo.PetMasterUserID	= pMaster->GetID();	//?·è??
//		PetInfo.bAlive		= TRUE;
//		//PetInfo.MapNum		?·è??
//
//		PetInsertToDB(pMaster->GetID(), &PetInfo);
//
//		//PetInsertToDB ?¬æ? ??ç±€??
//		pPet = (CPet*)g_pObjectFactory->MakeNewObject(eObjectKind_Pet, 0, &BaseInfo);
//		pPet->InitPet(pTotalInfo);
//	}
//	else
//	{
//		pPet = (CPet*)g_pObjectFactory->MakeNewObject(eObjectKind_Pet, 0, pBaseObjectInfo);
//		pPet->InitPet(pTotalInfo);
//	}
//
//	pPet->SetGridID(pMaster->GetGridID());
//	//pPet->SetInited();
//	pPet->SetMaster(pMaster);
//
//	return pPet;
//}

CMonster* CServerSystem::AddMonster(DWORD dwSubID, BASEOBJECT_INFO* pBaseObjectInfo,MONSTER_TOTALINFO* pTotalInfo,VECTOR3* pPos,WORD wObjectKind)
{
	ASSERT(g_pUserTable->FindUser(pBaseObjectInfo->dwObjectID) == NULL);

	EObjectKind objectkind = eObjectKind_Monster;
	
	if(wObjectKind == eObjectKind_SpecialMonster)objectkind = eObjectKind_SpecialMonster;
	else if(wObjectKind == eObjectKind_ToghterPlayMonster)objectkind = eObjectKind_ToghterPlayMonster;
	//2007. 7. 5. CBH - ?ªåƒ¥?¦ç? ?ªè¾¨ è·¨è¶??è¬™ç? ?²è? è¹ºé?
	else if(wObjectKind == eObjectKind_Mining)	objectkind = eObjectKind_Mining;
	else if(wObjectKind == eObjectKind_Collection)	objectkind = eObjectKind_Collection;
	else if(wObjectKind == eObjectKind_Hunt)	objectkind = eObjectKind_Hunt;
	//2007. 10. 30. CBH - é¡«æ?é©??ªè¾¨ è·¨è¶??è¬™ç? ?²è? è¹ºé?
	else if(wObjectKind == eObjectKind_TitanMonster)	objectkind = eObjectKind_TitanMonster;

	CMonster* pMonster = (CMonster*)g_pObjectFactory->MakeNewObject(objectkind,0, pBaseObjectInfo);
	pMonster->SetSubID(dwSubID);
	pMonster->InitMonster(pTotalInfo);

	pMonster->SetGridID(pBaseObjectInfo->BattleID);

	pMonster->SetInited();
	CCharMove::InitMove(pMonster,pPos);

	g_pUserTable->AddUser(pMonster,pMonster->GetID());

	//AISystem
	g_pAISystem->AddObject(pMonster);
	
	CBattle* pBattle = BATTLESYSTEM->GetBattle(pMonster->GetBattleID());
	if(pBattle->GetBattleKind() != eBATTLE_KIND_NONE)
		BATTLESYSTEM->AddObjectToBattle(pBattle, pMonster);

	return pMonster;
}

CBossMonster* CServerSystem::AddBossMonster(DWORD dwSubID, BASEOBJECT_INFO* pBaseObjectInfo,MONSTER_TOTALINFO* pTotalInfo,VECTOR3* pPos)
{
	//if(DungeonMGR->IsDungeon(g_pServerSystem->GetMapNum()))
	//	return NULL;
	ASSERT(g_pUserTable->FindUser(pBaseObjectInfo->dwObjectID) == NULL);

	CBossMonster* pBossMonster = (CBossMonster*)g_pObjectFactory->MakeNewObject(eObjectKind_BossMonster,0, pBaseObjectInfo);
	pBossMonster->SetSubID(dwSubID);
	pBossMonster->InitMonster(pTotalInfo);
	//pMonster->SetSMonsterList();
	pBossMonster->SetGridID(pBaseObjectInfo->BattleID);
	pBossMonster->SetInited();
	
	// The Boss is Out In Msg to Client ! 2014-05-18

	if(CServerSystem::gBossShout && !DungeonMGR->IsDungeon(g_pServerSystem->GetMapNum()))
	{
		MSG_CHAT_WORD2 msg;
		msg.Category = MP_CLIENT;
		msg.Protocol = MP_CLIENT_SECOND_AGENT;
		msg.Flag = eAddBossMonster;
		SafeStrCpy(msg.MonsterName,pBaseObjectInfo->ObjectName,MAX_MONSTER_NAME_LENGTH+1);//////BY JACK
		msg.MapNum = g_pServerSystem->GetMapNum();
		msg.ChannelNum = pBaseObjectInfo->BattleID;
		msg.x = pPos->x / 100;
		msg.y = pPos->z / 100;
		g_Network.Broadcast2AgentServer((char*)&msg,sizeof(msg));
		
	}

	g_Console.LOG(4,"[ch %d] Boss Spawned Pos:%1.0f,%1.0f",pBaseObjectInfo->BattleID,pPos->x / 100,pPos->z / 100);

	

	CCharMove::InitMove(pBossMonster,pPos);
	g_pUserTable->AddUser(pBossMonster,pBossMonster->GetID());
	g_pAISystem->AddObject(pBossMonster); 
	BOSSMONMGR->SetBossInfo(pBossMonster);
	
	CBattle* pBattle = BATTLESYSTEM->GetBattle(pBossMonster->GetBattleID());
	if(pBattle->GetBattleKind() != eBATTLE_KIND_NONE)
		BATTLESYSTEM->AddObjectToBattle(pBattle, pBossMonster);

	return pBossMonster;
}


CFieldBossMonster* CServerSystem::AddFieldBossMonster(DWORD dwSubID, BASEOBJECT_INFO* pBaseObjectInfo,MONSTER_TOTALINFO* pTotalInfo,VECTOR3* pPos)
{
	//if()
	//	return NULL;

	ASSERT(g_pUserTable->FindUser(pBaseObjectInfo->dwObjectID) == NULL);

	CFieldBossMonster* pFieldBossMonster = (CFieldBossMonster*)g_pObjectFactory->MakeNewObject(eObjectKind_FieldBossMonster,0, pBaseObjectInfo);
	pFieldBossMonster->SetSubID(dwSubID);
	pFieldBossMonster->InitMonster(pTotalInfo);
	pFieldBossMonster->SetGridID(pBaseObjectInfo->BattleID);
	pFieldBossMonster->SetInited();
	
	if(CServerSystem::gBossSubShout && !DungeonMGR->IsDungeon(g_pServerSystem->GetMapNum()))
	{
		MSG_CHAT_WORD2 msg;
		msg.Category = MP_CLIENT;
		msg.Protocol = MP_CLIENT_SECOND_AGENT;
		msg.Flag = eAddFieldBossMonster;
		SafeStrCpy(msg.MonsterName,pBaseObjectInfo->ObjectName,MAX_MONSTER_NAME_LENGTH+1);//////BY JACK
		msg.MapNum = g_pServerSystem->GetMapNum();
		msg.ChannelNum = pBaseObjectInfo->BattleID;
		msg.x = pPos->x / 100;
		msg.y = pPos->z / 100;
		g_Network.Broadcast2AgentServer((char*)&msg,sizeof(msg));
		
		
	}

	g_Console.LOG(4,"[ch %d] Boss Spawned Pos:%1.0f,%1.0f",pBaseObjectInfo->BattleID,pPos->x / 100,pPos->z / 100);

	
	
	//sprintf(msg.Msg,"[%s]?¤è¯?žBOSS?¤ç?å©“[%s] [%d]???´æ? %1.0f,%1.0f",pBaseObjectInfo->ObjectName,GetMap()->GetMapName(),pBaseObjectInfo->BattleID,pPos->x / 100,pPos->z / 100);
	//g_Network.Broadcast2AgentServer((char*)&msg,sizeof(msg));
	CCharMove::InitMove(pFieldBossMonster,pPos);

	g_pUserTable->AddUser(pFieldBossMonster,pFieldBossMonster->GetID());

	g_pAISystem->AddObject(pFieldBossMonster); 
		
	CBattle* pBattle = BATTLESYSTEM->GetBattle(pFieldBossMonster->GetBattleID());
	if(pBattle->GetBattleKind() != eBATTLE_KIND_NONE)
		BATTLESYSTEM->AddObjectToBattle(pBattle, pFieldBossMonster);

	return pFieldBossMonster;
}
// ?·è??¾è¶ - 05.12 æªœè???
// ?·è??¾è¶ ?¡ï¼ è¹ºé? ?ç†±
CFieldSubMonster* CServerSystem::AddFieldSubMonster(DWORD dwSubID, BASEOBJECT_INFO* pBaseObjectInfo,MONSTER_TOTALINFO* pTotalInfo,VECTOR3* pPos)
{
	//if(DungeonMGR->IsDungeon(g_pServerSystem->GetMapNum()))
	//	return NULL;

	ASSERT(g_pUserTable->FindUser(pBaseObjectInfo->dwObjectID) == NULL);

	CFieldSubMonster* pFieldSubMonster = (CFieldSubMonster*)g_pObjectFactory->MakeNewObject(eObjectKind_FieldSubMonster,0, pBaseObjectInfo);
	pFieldSubMonster->SetSubID(dwSubID);
	pFieldSubMonster->InitMonster(pTotalInfo);
	pFieldSubMonster->SetGridID(pBaseObjectInfo->BattleID);
	pFieldSubMonster->SetInited();
	
	CCharMove::InitMove(pFieldSubMonster,pPos);

	g_pUserTable->AddUser(pFieldSubMonster,pFieldSubMonster->GetID());

	g_pAISystem->AddObject(pFieldSubMonster); 
		
	CBattle* pBattle = BATTLESYSTEM->GetBattle(pFieldSubMonster->GetBattleID());
	if(pBattle->GetBattleKind() != eBATTLE_KIND_NONE)
		BATTLESYSTEM->AddObjectToBattle(pBattle, pFieldSubMonster);

	return pFieldSubMonster;
}

CNpc* CServerSystem::AddNpc(BASEOBJECT_INFO* pBaseObjectInfo,NPC_TOTALINFO* pTotalInfo,VECTOR3* pPos,float angle/* = 0*/)
{
	ASSERT(g_pUserTable->FindUser(pBaseObjectInfo->dwObjectID) == NULL);

	CNpc* pNpc = (CNpc*)g_pObjectFactory->MakeNewObject(eObjectKind_Npc,0, pBaseObjectInfo);
	pNpc->InitNpc(pTotalInfo);
	pNpc->SetGridID(pBaseObjectInfo->BattleID);

	pNpc->SetNpcAngle(angle);	//SW070704 è¹ºé?

	pNpc->SetInited();
	CCharMove::InitMove(pNpc,pPos);

	g_pUserTable->AddUser(pNpc,pNpc->GetID());
	
	CBattle* pBattle = BATTLESYSTEM->GetBattle(pNpc->GetBattleID());
	if(pBattle->GetBattleKind() != eBATTLE_KIND_NONE)
		BATTLESYSTEM->AddObjectToBattle(pBattle, pNpc);

	return pNpc;
}


CTacticObject* CServerSystem::AddTacticObject(BASEOBJECT_INFO* pBaseObjectInfo,CTacticStartInfo* pInfo,CPlayer* pOperator,DIRINDEX Direction)
{
	ASSERT(g_pUserTable->FindUser(pBaseObjectInfo->dwObjectID) == NULL);

	CTacticObject* pTactic = (CTacticObject*)g_pObjectFactory->MakeNewObject(eObjectKind_Tactic,0, pBaseObjectInfo);
	BYTE AbilLevel = 0;
	BYTE stage = pOperator->GetStage();
	if( stage == eStage_Hwa || stage == eStage_Hyun )
		AbilLevel = ABILITYMGR->GetAbilityLevel(ABILITYINDEX_TACTIC_HWA, pOperator->GetAbilityGroup());
	else if( stage == eStage_Geuk || stage == eStage_Tal )
		AbilLevel = ABILITYMGR->GetAbilityLevel(ABILITYINDEX_TACTIC_GEUK, pOperator->GetAbilityGroup());
		
	pTactic->InitTacticObject(pInfo,pOperator,Direction,AbilLevel);

	pTactic->SetInited();
	VECTOR3* pPos = CCharMove::GetPosition(pOperator);
	CCharMove::InitMove(pTactic,pPos);

	g_pUserTable->AddUser(pTactic,pTactic->GetID());
	
	return pTactic;
}

CSkillObject* CServerSystem::AddSkillObject(CSkillObject* pSkillObj,VECTOR3* pPos)
{
	ASSERT(g_pUserTable->FindUser(pSkillObj->GetID()) == NULL);

	pSkillObj->SetInited();
	CCharMove::InitMove(pSkillObj,pPos);//kiv

	g_pUserTable->AddUser(pSkillObj,pSkillObj->GetID());
	
	return pSkillObj;
}

CMapObject* CServerSystem::AddMapObject(DWORD Kind, BASEOBJECT_INFO* pBaseObjectInfo, MAPOBJECT_INFO* pMOInfo, VECTOR3* pPos)
{
	ASSERT(g_pUserTable->FindUser(pBaseObjectInfo->dwObjectID) == NULL);

	CMapObject* pMapObject = (CMapObject*)g_pObjectFactory->MakeNewObject((EObjectKind)Kind,0, pBaseObjectInfo);
	
	pMapObject->InitMapObject( pMOInfo );
	pMapObject->SetGridID( pBaseObjectInfo->BattleID );
	pMapObject->SetInited();
	CCharMove::InitMove( pMapObject, pPos );

	g_pUserTable->AddUser( pMapObject, pMapObject->GetID() );
	
	CBattle* pBattle = BATTLESYSTEM->GetBattle(pMapObject->GetBattleID());
	if(pBattle->GetBattleKind() != eBATTLE_KIND_NONE)
		BATTLESYSTEM->AddObjectToBattle( pBattle, pMapObject );

	return pMapObject;
}

/*
CItemObject* CServerSystem::AddItemObject(ITEMBASE* pItemInfo,VECTOR3* pPos)
{
	//ASSERT(GetNpc(pBaseObjectInfo->dwObjectID) == NULL);

	CItemObject* pItemObject = (CItemObject*)g_pObjectFactory->MakeNewObject(eObjectKind_Item,0,pItemInfo->MapNum,NULL);
	pItemObject->InitItem(pItemInfo);

	CCharMove::InitMove(pItemObject,pPos);

	m_ItemObjectTable.Add(pItemObject,pItemObject->GetID());
	pItemObject->SetInited();
	return pItemObject;
}
*/	

//SW060608 DBUpdate Safely
void CServerSystem::HandlingBeforeServerEND()
{
	//SetUnableProcess(TRUE);				//1. Process ?¬æ? & DBReturnMsg ç±€??å¯°ï?.

	//g_Console.LOG(4, "[0/4]Start Working For Map Server Shutdown" );

	RemoveServerForKind(AGENT_SERVER);	//2. AgentSrv ?’æ™¦.
	//g_Console.LOG(4, "[1/4]Agent Disconnecting...Done" );

	//3. =======Object ç±€??
	g_pUserTable->SetPositionHead();
	CObject* pObject;
	while(pObject = g_pUserTable->GetData())
	{	
		if(pObject->GetObjectKind() == eObjectKind_Player)
			g_pServerSystem->RemovePlayer(pObject->GetID(), FALSE);
			//FALSE: è¤’è–¯??UserTableç¸‘æ†®???–è¾¦??å½Šæ?æ£?
		else
		{
			CCharMove::ReleaseMove(pObject);
			g_pObjectFactory->ReleaseObject(pObject);
			BYTE objectKind = pObject->GetObjectKind();
			/*
			if( objectKind == eObjectKind_Monster ||
				objectKind == eObjectKind_BossMonster ||
				objectKind == eObjectKind_TitanMonster )
				*/
			if( objectKind & eObjectKind_Monster )
			{
				if(g_pAISystem)
					g_pAISystem->RemoveObject(pObject->GetID());
			}
		}
	}
	//g_Console.LOG(4, "[2/4]Start Object Handling (Update/Delete)...Done" );

	g_pUserTable->RemoveAll();
	//g_Console.LOG(4, "[3/4]Removing All Object on UserTable...Done" );

	//g_Console.LOG(4, "[4/4]Start DB Release..." );
	g_DB.Release();
	//g_Console.LOG(4, "[4/4]Release...Done" );

	SetCharUpdateCompletion(TRUE);
}


void CServerSystem::RemovePlayer(DWORD dwPlayerID, BOOL bRemoveFromUserTable )
{
	CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser(dwPlayerID);
	if(!pPlayer)
		return;
	ASSERT(pPlayer->GetObjectKind() == eObjectKind_Player);

//	FRIENDMGR->UserLogOut(pPlayer->GetID());
	EXCHANGEMGR->UserLogOut(pPlayer);
	STREETSTALLMGR->UserLogOut(pPlayer);
	VIMUMGR->UserLogOut(pPlayer);
	LOOTINGMGR->LootingCancel(pPlayer);	//PKLOOT
	BOSSMONMGR->UserLogOut(pPlayer->GetID(), pPlayer->GetGridID());
	SIEGEWARMGR->UserLogOut(pPlayer);
	
	// YH d?œï?ï¹Œï? ?œu?³Aï¹ i?³IAI?œâ•³ï¹Œcï¹Œï?A AE?³å?aE?³ä½¿?³è?ï¹Œï¼¹ fï¹Œï?e?³ï?I ï¹ iE /Au?³è?ï¹Œï¼¹ ä½¿å??œï??³è??³Iï¹ŒO?³â–¼ï¹Œï?ï¹Œï? C?œè?A?œè?ï¹Œï?U.
	if(pPlayer->GetInited() == TRUE)
	{
		if(pPlayer->GetBattle()->GetBattleKind() == eBATTLE_KIND_SURYUN)
		{
			pPlayer->SetLife(pPlayer->GetMaxLife(),FALSE);
			pPlayer->SetShield(pPlayer->GetMaxShield(),FALSE);
			pPlayer->SetNaeRyuk(pPlayer->GetMaxNaeRyuk(),FALSE);
		}		

		//Ac?³iy ?³è?u?³ï?A ?³è?ä½¿â•³CeAï¹Œï¼¹, ?³iyï¹Œï?i?³ï?A, ä½¿å??³i?³ï?A ?³å?iAO. (Dataï¹Œï?| AuAaCI?³å?a Auï¹?ï¹Œï¼¹ ?³è?ä½¿â•³CeAï¹Œï¼¹ï¹?ï¹Œï¼¹ ï¹Œï?eCN ?œï?I?œï??œï?; Aä½¿å?ï¹Œï?ï¹ŒcC?œè??œuä½¿ï? COï¹ ii.)
		//Resultï¹ iC?œui?œu?³ä½¿ KINE_NONEAI ï¹ iC?œuuï¹Œï?Uï¹Œï?e?
		if(pPlayer->GetState() == eObjectState_Die)
		{
#ifdef _KOR_LOCAL_
			if( pPlayer->IsActionPanelty() == FALSE )
			{
				pPlayer->InsetNoActionPaneltyLog( eExpLog_NoActionPanelty_Exit );
				pPlayer->SetActionPanelty();

			}
			else
#endif
			{
				if( ( pPlayer->GetBattle()->GetBattleKind() == eBATTLE_KIND_NONE && pPlayer->IsPenaltyByDie() ) ||
					pPlayer->GetBattle()->GetBattleKind() == eBATTLE_KIND_SIEGEWAR )
				{
					pPlayer->ReviveLogInPenelty();
				}
				else	//?œï?nä½¿o?³iAä½¿ï? A?³ï??œuuï¹Œï?U.	//?œï?nä½¿o?³iAä½¿ï? ï¹Œï?o?œo?œï?AIï¹?ï¹Œï¼¹?³è?O A?³ï?; ?œuoï¹ iï¹ i AOï¹Œï?U. confirm
				{
					pPlayer->ReviveAfterVimu( FALSE );	//ï¹‰aï¹‰uA?†â•±ï¼¹â•±??. msgè¶³â•±?†âˆ®Ao ?–E?žA?žU.6
				}
			}
		}

		// Aä½¿å?ï¹Œï??d?œï?ï¹Œï? ?œu?³Aï¹ i?³IAI?œâ•³ï¹Œc///////////////////////
		if( pPlayer->IsPKMode() )
			pPlayer->SetPKModeEndtime();

		CharacterHeroInfoUpdate(pPlayer);
		CharacterTotalInfoUpdate(pPlayer);
		CharacterMugongInfoUpdate(pPlayer);
		CharacterSkinInfoUpdate(pPlayer); 
		///////////////////////////////////////////

		CCharMove::ReleaseMove(pPlayer);
		pPlayer->GetPetManager()->RemovePet();

//		g_Console.Log(eLogDisplay, 4, "DisconnectUser : %d %s", dwPlayerID, pPlayer->GetObjectName());
		
		BOOL rt = BATTLESYSTEM->DeleteObjectFromBattle(pPlayer);
	}
	else		
	{
		CCharMove::ReleaseMove(pPlayer);
		pPlayer->GetPetManager()->RemovePet();
	}

	/*if(FBTIMERMGR->IsFBTimerMap()&&FBTIMERMGR->IsFBTimerChannel(pPlayer->GetChannelID()))  //CoNaN ?„å£ºè¤’ä?å¥€ç·Šç??¿å??å?ç­‰ï??µæ??„ç?,?®å?åº‹æº«è¨§åŸ­ 2018-12-10
	{
		pPlayer->SetFBTimerMonsterNum(0);  //CoNaN ç¾²å??µæ?è¼›æ??›è‚®å¥€ç¬­ç¦»ä¼€å¢…æ?è¬? 2018-12-15

		FBTIMERMGR->RemoveFBTimerChannelInfo(pPlayer->GetID());
	}*/

	CHANNELSYSTEM->DecreasePlayerNum(pPlayer->GetChannelID());


	/*DWORD playerSizeInChannel = CHANNELSYSTEM->GetPlayerNumInChannel(
		pPlayer->GetChannelID());
	if(DungeonMGR->IsDungeon(GetMapNum())&&
		! pPlayer->IsDungeonObserver())
	{
		--playerSizeInChannel;
	}*/


	if( g_pServerSystem->GetMap()->IsMapKind(eSurvivalMap) )
		SVVMODEMGR->RemoveSVModeUser(pPlayer);


	if(bRemoveFromUserTable)
		g_pUserTable->RemoveUser(dwPlayerID);

	g_pObjectFactory->ReleaseObject(pPlayer);
	UnRegistLoginMapInfo(dwPlayerID); //dbï¹?ï¹Œï¼¹ CoAc ï¹Œï?Eä½¿oä½¿è?E?³I 08?³ï?I ?œuA?œâ•³A
	
	EVENTMAPMGR->RemoveMember( pPlayer, 0, pPlayer->GetChannelID() );
	PARTYWARMGR->RemovePlayer( pPlayer );
	QUESTMAPMGR->RemovePlayer( pPlayer );

	BOBUSANGMGR->LeaveGuest( pPlayer );

	FORTWARMGR->RemovePlayer( pPlayer );

	GUILDWARMGR->RemovePlayer(pPlayer);

	if (GetMap()->IsMapKind(eInsDungeon))
	{
		INSDUNGEONMGR->DeleteMissionItem(pPlayer);
		DWORD dwChannelID = pPlayer->GetChannelID();
		CInsDungeon* pInsDungeon = INSDUNGEONMGR->GetInsDungeon(dwChannelID);
		if (pInsDungeon != NULL)
			pInsDungeon->RemovePlayer(pPlayer);
		INSDUNGEONMGR->InsDungeonDestoryForCheckPlayer(pPlayer);
	}

	/*if(DungeonMGR->IsDungeon(g_pServerSystem->GetMapNum()))
	{
		DungeonMGR->Exit(pPlayer);
		DGUpdate(pPlayer->GetPartyIdx(),GetMapNum(),DungeonMGR->GetPlayerNumInDungeon(pPlayer->GetChannelID()),0,0,5);
	}*/
}

void CServerSystem::RemoveTitan( CTitan* pTitan )
{
	g_pObjectFactory->ReleaseObject((CObject*)pTitan);
}

void CServerSystem::RemovePet( DWORD dwPetObjectID )
{
	CObject* pObject = g_pUserTable->FindUser(dwPetObjectID);
	ASSERT(pObject);
	if(!pObject)	return;

	CCharMove::ReleaseMove(pObject);

	g_pUserTable->RemoveUser(dwPetObjectID);

	g_pObjectFactory->ReleaseObject(pObject);
}

void CServerSystem::RemoveMonster(DWORD dwMonster)
{
	CObject* pObject = g_pUserTable->FindUser(dwMonster);
	ASSERT(pObject);
	if(!pObject)	return;

	CCharMove::ReleaseMove(pObject);
	
	BOOL rt = BATTLESYSTEM->DeleteObjectFromBattle(pObject);
	ASSERT(rt);

	if(g_pAISystem)
	g_pAISystem->RemoveObject(dwMonster);
	
	g_pUserTable->RemoveUser(dwMonster);

	g_pObjectFactory->ReleaseObject(pObject);
}


void CServerSystem::RemoveBossMonster( DWORD dwMonster, BOOL bDisappear )
{
	CObject* pObject = g_pUserTable->FindUser(dwMonster);
	ASSERT(pObject);
	if(!pObject)	return;

	CCharMove::ReleaseMove(pObject);
	
	BOOL rt = BATTLESYSTEM->DeleteObjectFromBattle(pObject);
	ASSERT(rt);

	if(g_pAISystem && !bDisappear)
		g_pAISystem->RemoveObject(dwMonster);

	g_pUserTable->RemoveUser(dwMonster);

	g_pObjectFactory->ReleaseObject(pObject);

//CONDITIONMGR->RemoveListToPool(dwMonster);

}

void CServerSystem::RemoveNpc(DWORD dwNpcID)
{
	CObject* pObject = g_pUserTable->FindUser(dwNpcID);
	ASSERT(pObject);
	if(!pObject)	return;
	
	CCharMove::ReleaseMove(pObject);
	
	BOOL rt = BATTLESYSTEM->DeleteObjectFromBattle(pObject);
	ASSERT(rt);
	if(!pObject)	return;

	g_pUserTable->RemoveUser(dwNpcID);
	g_pObjectFactory->ReleaseObject(pObject);
}

void CServerSystem::RemoveTacticObject(DWORD dwTacticObjID)
{
	CObject* pObject = g_pUserTable->FindUser(dwTacticObjID);
	ASSERT(pObject);
	if(!pObject)	return;
	
	CCharMove::ReleaseMove(pObject);
	
	g_pUserTable->RemoveUser(dwTacticObjID);
	g_pObjectFactory->ReleaseObject(pObject);
}
void CServerSystem::RemoveSkillObject(DWORD SkillObjectID)
{
	CObject* pObject = g_pUserTable->FindUser(SkillObjectID);
	//ASSERT(pObject);
	if(!pObject)	return;	

	CCharMove::ReleaseMove(pObject);
	g_pUserTable->RemoveUser(SkillObjectID);
	g_pObjectFactory->ReleaseObject(pObject);
}

void CServerSystem::RemoveMapObject( DWORD MapObjID )
{
	CObject* pObject = g_pUserTable->FindUser(MapObjID);
	ASSERT(pObject);
	if(!pObject)	return;
	
	CCharMove::ReleaseMove(pObject);
	
	BOOL rt = BATTLESYSTEM->DeleteObjectFromBattle(pObject);
	ASSERT(rt);
	if(!pObject)	return;

	g_pUserTable->RemoveUser(MapObjID);
	g_pObjectFactory->ReleaseObject(pObject);
}


void CServerSystem::SendToOne(CObject* pObject,void* pMsg,int MsgLen)
{
	g_Network.Send2Server(pObject->GetAgentNum(),(char*)pMsg,MsgLen);
}

void CServerSystem::ReloadResourceData()
{
	//CHANNELSYSTEM->Init( m_wMapNum );

	LoadEventRate("./Resource/Server/droprate.bin");
	LoadPartyPlustime("./Resource/PartyPlustimeInfo.bin");
	LoadFameConfig("./Resource/FameList.bin");

	SKILLMGR->Release();
	SKILLMGR->Init();
	GAMERESRCMNGR->LoadPlayerxMonsterPoint();
	ITEMMGR->ReloadItemList();
	ITEMMGR->LoadReinforceItemInfo();
	ITEMMGR->LoadGoldShopItem();
	ITEMMGR->LoadItemLucker();
	ITEMMGR->LoadItemGrowSet();

	LOGINEVENTMGR->LoadLoginEventItemList();
	ITEMMGR->LoadGameCheckList();//kiv

	//SAFEAREAMGR->LoadSafeAreaInfo();
	//FBTIMERMGR->LoadFBTimerInfo(); 

	//Regen
	//AI Remove
	g_pUserTable->SetPositionUserHead();
	CObject* pObject;
	while(pObject = (CObject*)g_pUserTable->GetUserData())
	{
		BYTE objectKind = pObject->GetObjectKind();

		switch(objectKind)
		{
		case eObjectKind_Monster:
		case eObjectKind_Mining:
		case eObjectKind_Collection:
		case eObjectKind_Hunt:
		case eObjectKind_TitanMonster:
			{
				RemoveMonster(pObject->GetID());
				break;
			}
		case eObjectKind_BossMonster:
			{
				RemoveBossMonster(pObject->GetID());
				break;
			}
		case eObjectKind_Npc:
			{
				RemoveNpc(pObject->GetID());
				break;
			}		
		}
	}
	g_pAISystem->RemoveAllList();

	REGENMGR->Release();
	GROUPMGR->Release();
	
	GAMERESRCMNGR->ResetPetList();
	GAMERESRCMNGR->LoadPetList();
	GAMERESRCMNGR->LoadPetRule();
	GAMERESRCMNGR->ResetPetBuffList();
	GAMERESRCMNGR->LoadPetBuffList();

	

	//SW070127 é¡«æ?é©?
//	GAMERESRCMNGR->RemoveTitanList();
//	GAMERESRCMNGR->LoadTitanList();
//	GAMERESRCMNGR->LoadTitanRule();
//	GAMERESRCMNGR->LoadTitanSpellCostInfo();

//	GAMERESRCMNGR->RemoveTitanSkillLink();
//	GAMERESRCMNGR->LoadTitanSkillLink();
//	GAMERESRCMNGR->RemoveTitanEquipEnduranceInfo();
//	GAMERESRCMNGR->LoadTitanEquipEnduranceInfo();

	//////////////////////////////////////////////////////////////////////////
	//MonsterList
	GAMERESRCMNGR->ResetMonsterList();
	GAMERESRCMNGR->LoadMonsterList();
	//GAMERESRCMNGR->LoadSMonsterList();
	GAMERESRCMNGR->LoadPaneltyTime();

	

	MON_SPEECHMGR->LoadMonSpeechInfoList();

	RAREITEMMGR->LoadRareItemOptionInfo();
	RAREITEMMGR->LoadRareItemInfo();
	RAREITEMMGR->LoadRareValueProbList();

	GAMERESRCMNGR->LoadNomalClothesSkinList();
	GAMERESRCMNGR->LoadCostumeSkinList();
	
	GAMERESRCMNGR->LoadRebornSetting();

	ITEMDROP_OBJ->Init();

	MAPITEMDROP_OBJ->Init();

	g_pAISystem->LoadAIGroupList();
	g_Console.LOG(4, "Bin reloaded!");


//---------------------------below is for dev test
	/*g_pUserTable->SetPositionUserHead();
	CObject* pObject;
	while(pObject = (CObject*)g_pUserTable->GetUserData())
	{
		BYTE objectKind = pObject->GetObjectKind();

		switch(objectKind)
		{
		case eObjectKind_Monster:
		case eObjectKind_Mining:
		case eObjectKind_Collection:
		case eObjectKind_Hunt:
		case eObjectKind_TitanMonster:
			{
				RemoveMonster(pObject->GetID());
				break;
			}
		case eObjectKind_BossMonster:
			{
				RemoveBossMonster(pObject->GetID());
				break;
			}
		case eObjectKind_Npc:
			{
				RemoveNpc(pObject->GetID());
				break;
			}		
		}
	}
	g_pAISystem->RemoveAllList();
	REGENMGR->Release();
	GROUPMGR->Release();
	g_pAISystem->LoadAIGroupList();*/
}


void CServerSystem::AddWaitExitPlayer( DWORD dwAgentConnetionIndex, DWORD dwPlayerID )
{
	WAIT_EXIT_PLAYER_INFO* pInfo = m_mpWaitExitPlayer->Alloc();
	pInfo->dwAgentConnectionIndex = dwAgentConnetionIndex;
	pInfo->dwPlayerID = dwPlayerID;
	pInfo->dwStartTime = gCurTime;
	m_listWaitExitPlayer.AddTail( pInfo );
}

void CServerSystem::ProcessWaitExitPlayer()
{
	if( m_listWaitExitPlayer.GetCount() == 0 ) return;

	WAIT_EXIT_PLAYER_INFO* pInfo = (WAIT_EXIT_PLAYER_INFO*)m_listWaitExitPlayer.GetHead();
	if( pInfo )
	{
		if( gCurTime - pInfo->dwStartTime >= 5*1000 )	//10??-->5?¾ç? æ»²å”³(2005.11.1)
		{
			CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser( pInfo->dwPlayerID );
			if( pPlayer )
			{
				DWORD dwObjectID = pPlayer->GetID();
				DWORD dwUserID = pPlayer->GetUserID();

				// RaMa - 04.12.14
				pPlayer->UpdateLogoutToDB();

				PARTYMGR->UserLogOut( pPlayer );
				FRIENDMGR->UserLogOut( dwObjectID );
				GUILDMGR->UserLogOut( pPlayer );


				g_pServerSystem->RemovePlayer( dwObjectID );
				//
				ConfirmUserOut( pInfo->dwAgentConnectionIndex, dwUserID, dwObjectID, 2 );
			}

			m_mpWaitExitPlayer->Free( pInfo );
			m_listWaitExitPlayer.RemoveHead();
		}
	}
}

void CServerSystem::SetNation()
{
	CMHFile file;
	if( !file.Init( "LocalizingInfo.txt", "rt", MHFILE_FLAG_DONOTDISPLAY_NOTFOUNDERROR ) )
		return;

	if( strcmp( file.GetString(), "*NATION" ) == 0 )
	{
		if( strcmp( file.GetString(), "CHINA" ) == 0 )
		{
			m_Nation = eNATION_CHINA;
		}
	}

	file.Release();
}

void CServerSystem::RemoveServerForKind( WORD wSrvKind )
{
	g_pServerTable->RemoveServerForKind(wSrvKind);
}


void CServerSystem::LoadHackCheck()
{
	CMHFile file;
	if( !file.Init( "ServerSet/HackCheck.txt", "rt", MHFILE_FLAG_DONOTDISPLAY_NOTFOUNDERROR ) )
	{
		//g_Console.LOG(4, "HackCheckNum : %d, HackCheckWriteNum : %d", g_nHackCheckNum, g_nHackCheckWriteNum );
		return;
	}

	char temp[256] = {0, };
	while( 1 )
	{
		if( file.IsEOF() )	break;

		file.GetString( temp );
		if( strcmp( temp, "*HACKCHECKNUM" ) == 0 )
		{
			g_nHackCheckNum = file.GetInt();
		}
		else if( strcmp( temp, "*HACKCHECKWRITENUM" ) == 0 )
		{
			g_nHackCheckWriteNum = file.GetInt();
		}
	}

	file.Release();

	//g_Console.LOG(4, "HackCheckNum : %d, HackCheckWriteNum : %d", g_nHackCheckNum, g_nHackCheckWriteNum );
}


void CServerSystem::CheckServerInfo( DWORD ObjectID )
{
	memset( &m_ProcessInfo, 0, sizeof(m_ProcessInfo) );
	SetProtocol( &m_ProcessInfo,MP_CHEAT, MP_CHEAT_MAP_CONDITION );
	m_ProcessInfo.dwObjectID = ObjectID;
	m_bCheckInfo = TRUE;

	TestGameQuery( ObjectID );
	TestLogQuery( ObjectID );

	m_ProcessInfo.ProcessInfo.DBQueryCount = g_DB.GetQueryQueueCount();
	m_ProcessInfo.StarTime = gCurTime;
}


// global function
void __stdcall OnConnectServerSuccess(DWORD dwConnectionIndex, void* pVoid)
{
	SERVERINFO* info = (SERVERINFO*)pVoid;
	info->dwConnectionIndex = dwConnectionIndex;
	if(info->wServerKind == MONITOR_SERVER)
	{
		BOOTMNGR->NotifyBootUpToMS(&g_Network);
		g_Console.LOG(4, "Connected to the MS : %s, %d, (%d)", info->szIPForServer, info->wPortForServer, dwConnectionIndex);
	}
	else
	{
		BOOTMNGR->SendConnectSynMsg(&g_Network, dwConnectionIndex, g_pServerTable);
		g_Console.LOG(4, "Connected to the Server : %s, %d, (%d)", info->szIPForServer, info->wPortForServer, dwConnectionIndex);
	}

	if (!dwConnectionIndex) return;
	info->dwConnectionIndex = dwConnectionIndex;
	MSG_WORD msg;
	msg.Category = MP_SERVER;
	msg.Protocol = MP_SERVER_PWRUP;
	msg.wData = g_pServerTable->GetSelfServer()->wPortForServer;
	g_Network.Send2Server(dwConnectionIndex, (char*)&msg, sizeof(msg));

	if(g_pServerTable->GetMaxServerConnectionIndex() < dwConnectionIndex)
			g_pServerTable->SetMaxServerConnectionIndex(dwConnectionIndex);
}

void __stdcall OnConnectServerFail(void* pVoid)
{
	SERVERINFO* info = (SERVERINFO*)pVoid;
	if(info->wServerKind == MONITOR_SERVER)
	{
		BOOTMNGR->AddBootListINI(MAP_SERVER, GAMERESRCMNGR->GetLoadMapNum(), g_pServerTable);
		BOOTMNGR->BactchConnectToMap(&g_Network, g_pServerTable);
		//g_Console.LOG(4, "Failed to Connect to the MS : %s, %d", info->szIPForServer, info->wPortForServer);
	}
	else
	{
		g_Console.LOG(4, "Failed to Connect to the Server : %s, %d", info->szIPForServer, info->wPortForServer);
		BOOTMNGR->RemoveBootList(g_pServerTable, info->wPortForServer);
	}	
}

void __stdcall OnAcceptServer(DWORD dwConnectionIndex)
{
	g_Console.LOG(4, "Server Connected : ConnectionIndex %d", dwConnectionIndex);
}

void __stdcall OnDisconnectServer(DWORD dwConnectionIndex)
{
	if( g_pServerSystem->GetStart() == FALSE ) return;

	g_Console.LOG(4, "Server Disconnected : ConnectionIndex %d", dwConnectionIndex);

	SERVERINFO * delInfo = g_pServerTable->RemoveServer(dwConnectionIndex);
	if(!delInfo)
	{
		ASSERT(0);
	}
	delete delInfo;


	// taiyo
	g_pUserTable->SetPositionUserHead();
	CObject * info = NULL;
	cPtrList list;

	while(info = g_pUserTable->GetUserData())
	{
		if(info->GetObjectKind() == eObjectKind_Player)
		{
			CPlayer * ply = (CPlayer *)info;
			if(ply->GetAgentNum() == dwConnectionIndex)
			{
				SaveMapChangePointUpdate(ply->GetID(), 0);
				//g_pServerSystem->RemovePlayer(ply->GetID());
				list.AddTail( ply );
			}
		}
	}
	PTRLISTPOS pos = list.GetHeadPosition();
	while( pos )
	{
		CPlayer* p = (CPlayer*)list.GetNext(pos);
		g_pServerSystem->RemovePlayer(p->GetID());
	}
	list.RemoveAll();
}

void __stdcall OnAcceptUser(DWORD dwConnectionIndex)
{
}

void __stdcall OnDisconnectUser(DWORD dwConnectionIndex)
{

}

void __stdcall ProcessDBMessage(DWORD dwEventIndex)
{
	if( g_bCloseWindow == TRUE )
		return;
	g_DB.ProcessingDBMessage();
}

void __stdcall ReceivedMsgFromServer(DWORD dwConnectionIndex,char* pMsg,DWORD dwLength)
{
	MSGROOT* pTempMsg = reinterpret_cast<MSGROOT*>(pMsg);
	ASSERT(pTempMsg->Category < MP_MAX);
	
	if( g_pServerMsgParser[pTempMsg->Category] == NULL ||
		pTempMsg->Category >= MP_MAX ||
		pTempMsg->Category == 0)
		return;

	g_pServerMsgParser[pTempMsg->Category](dwConnectionIndex, pMsg, dwLength);
}
std::string PacketEnDesStr=NetServer;
void __stdcall ReceivedMsgFromUser(DWORD dwConnectionIndex,char* pMsg,DWORD dwLength)
{
	MSGROOT* pTempMsg = reinterpret_cast<MSGROOT*>(pMsg);
	pTempMsg->Category = pTempMsg->Category ^ PacketEnDesStr[dwLength % (PacketEnDesStr.length() - 1)];
	pTempMsg->Protocol = pTempMsg->Protocol ^ PacketEnDesStr[dwLength % (PacketEnDesStr.length() - 1)];

	if( g_pUserMsgParser[pTempMsg->Category] == NULL ||
		pTempMsg->Category >= MP_MAX ||
		pTempMsg->Category == 0)
	{
		return;
	}

	g_pUserMsgParser[pTempMsg->Category](dwConnectionIndex, pMsg, dwLength);
}

void __stdcall GameProcess(DWORD dwEventIndex)
{
	g_pServerSystem->Process();
}
void __stdcall _5minGameProcess(DWORD dwEventIndex)
{
	g_pServerSystem->_5minProcess();
	MAPITEMDROP_OBJ->Process();
}



extern BOOL g_bWriteQuery;
char *strLower( char *str ) 
{
	char *temp;
	for ( temp = str; *temp; temp++ ) 
	{
		*temp = tolower( *temp );
	}
	return str;
}
void OnCommand(char* szCommand)
{
	SERVERCMD->OnCommand(szCommand);
	/*if(strcmp(strLower(szCommand),"killserver")==0||
		strcmp(strLower(szCommand),"killmap")==0||
		strcmp(strLower(szCommand),"kill")==0)
	{
		char cmd[256];
		if(strcmp(strLower(szCommand),"killserver")==0) wsprintfA(cmd,_T("TaskKill /f /im MapServer.exe & TaskKill /f /im AgentServer.exe & TaskKill /f /im DistributeServer.exe"));
		if(strcmp(strLower(szCommand),"killmap")==0) wsprintfA(cmd,_T("TaskKill /f /im MapServer.exe"));
		if(strcmp(strLower(szCommand),"kill")==0) wsprintfA(cmd,_T("TaskKill /F /PID %d"),GetCurrentProcessId());

		system(cmd);
		return;
	}*/
	/*ITEMDROP_OBJ->DevMonsterDrop(atoi(strLower(szCommand)));

	char buff[128] = { 0, };
	char cmd[256];
	DWORD idx;
	START_STRING_COMPARE(strLower(szCommand))

	COMPARE("killserver")
		strcpy(cmd, _T("TaskKill /f /im MapServer.exe & TaskKill /f /im AgentServer.exe & TaskKill /f /im DistributeServer.exe"));
	return;// goto go_ORDER;
	COMPARE("killmap")
		strcpy(cmd, _T("TaskKill /f /im MapServer.exe"));
	return;//	goto go_ORDER;
	COMPARE("kill")
		wsprintfA(cmd, _T("TaskKill /F /PID %d"), GetCurrentProcessId());
	return;//	goto go_ORDER;
	END_COMPARE*/


	/*MSG_CHAT_WORD msg;
	msg.Category = MP_CLIENT;
	msg.Protocol = MP_CLIENT_MSG_AGENT;
	msg.CyptKey = GAMERESRCMNGR->GetLoadMapNum();
	sprintf(msg.Msg,"[CONSOLE]: %s",szCommand);
	g_Network.Broadcast2AgentServer((char*)&msg,sizeof(MSG_CHAT_WORD));*/


//go_ORDER:
	//system(cmd);
	//return;
}

BOOL LoadEventRate(char* strFileName)
{
	int i=0;
	for(i=0; i<eEvent_Max; ++i)	
	{
		gEventRate[i] = 1.f;
		gEventRateFile[i] = 1.f;
	}

	CMHFile file;
#ifdef _FILE_BIN_
	if( !file.Init( strFileName, "rb") )
		return FALSE;
#else
	if( !file.Init( strFileName, "rt") )
		return FALSE;
#endif

	char Token[256];
	
// RaMa -04.11.24
	while( !file.IsEOF() )
	{
		file.GetString(Token);

		if(strcmp(Token,"#EXP") == 0)	
		{			
			//gExpRate = file.GetFloat();
			gEventRateFile[eEvent_ExpRate] = file.GetFloat();
		}
		else if(strcmp(Token, "#ABIL") == 0)
		{
			//gAbilRate = file.GetFloat();
			gEventRateFile[eEvent_AbilRate] = file.GetFloat();
		}
		else if(strcmp(Token,"#ITEM") == 0)
		{
			//gItemRate = file.GetFloat();
			gEventRateFile[eEvent_ItemRate] = file.GetFloat();
		}
		else if(strcmp(Token,"#MONEY") == 0)
		{
			//gMoneyRate = file.GetFloat();
			gEventRateFile[eEvent_MoneyRate] = file.GetFloat();
		}
		else if(strcmp(Token,"#GETMONEY") == 0)
		{
			//gGetMoney = file.GetFloat();
			gEventRateFile[eEvent_GetMoney] = file.GetFloat();
		}
		else if(strcmp(Token,"#DAMAGERECIVE") == 0)
		{
			//gDamageReciveRate = file.GetFloat();
			gEventRateFile[eEvent_DamageReciveRate] = file.GetFloat();
		}
		else if(strcmp(Token,"#DAMAGERATE") == 0)
		{
			//gDamageRate = file.GetFloat();
			gEventRateFile[eEvent_DamageRate] = file.GetFloat();
		}
		else if(strcmp(Token,"#NAERYUKSPEND") == 0)
		{
			//gNaeRuykRate = file.GetFloat();
			gEventRateFile[eEvent_NaeRuykRate] = file.GetFloat();
		}
		else if(strcmp(Token,"#UNGISPEED") == 0)
		{
			//gUngiSpeed = file.GetFloat();
			gEventRateFile[eEvent_UngiSpeed] = file.GetFloat();
		}
		else if(strcmp(Token,"#PARTYEXP") == 0)
		{
			//gPartyExpRate = file.GetFloat();
			gEventRateFile[eEvent_PartyExpRate] = file.GetFloat();
		}
		else if(strcmp(Token,"#MUGONGEXPRATE") == 0)
		{
			gEventRateFile[eEvent_MugongExp] = file.GetFloat();
		}
		else if (strcmp(Token, "#DAMAGEPLAYERRATE") == 0)
		{
			gEventRateFile[eEvent_DamagePlayerToPlayerRate] = file.GetFloat();
		}
	}


	file.Release();

	for(i=0; i<eEvent_Max; ++i)	
		gEventRate[i] = gEventRateFile[i];

	return TRUE;
}



BOOL LoadPartyPlustime(char* strFileName)
{
	memset( &gPartyEvent, 0, sizeof(PARTYEVENT)*ePartyEvent_Max );

	CMHFile file;
	if( !file.Init( strFileName, "rb") )
		return FALSE;
	int count = file.GetInt();	

	for( int i=0; i<count; ++i )
	{
		int idx = file.GetInt();
		gPartyEvent[idx].Rate[0] = file.GetFloat();
		gPartyEvent[idx].Rate[1] = file.GetFloat();
		gPartyEvent[idx].Rate[2] = file.GetFloat();
		gPartyEvent[idx].Rate[3] = file.GetFloat();
		gPartyEvent[idx].Rate[4] = file.GetFloat();
		gPartyEvent[idx].Rate[5] = file.GetFloat();
		gPartyEvent[idx].Rate[6] = file.GetFloat();
	}
	file.Release();
	return TRUE;
}



BOOL LoadFameConfig(char* strFileName)
{
	FAMEMGR->LoadFameStage();

	memset(&gFameSet, 0, sizeof(DWORD)*eFameSetMax);//kiv

	int i=0;
	for(i=0; i<eFameSetMax; ++i)	
	{
		gFameSet[i] = 1;
	}

	CMHFile file;
	if( !file.Init( strFileName, "rb") )
		return FALSE;

	char Token[256];
	
	while( !file.IsEOF() )
	{
		file.GetString(Token);

		if(Token[0] == '@')
		{
			file.GetLineX(Token, 256);
			continue;
		}
		if (Token[0] == '#')
		{
			file.GetLineX(Token, 256);
			continue;
		}

		if(strcmp(Token,"7") == 0)	
		{
			gFameSet[0] = file.GetDword();
		}
		else if(strcmp(Token,"*MONSTER_KILL_COUNT") == 0)	
		{
			gFameSet[1] = file.GetDword();
		}
		else if(strcmp(Token,"*MONSTER_KILL_FAME") == 0)	
		{
			gFameSet[2] = file.GetDword();
		}
		else if(strcmp(Token,"*PLAYER_KILL") == 0)	
		{
			gFameSet[eFamePlayerKill] = file.GetDword();
		}
		else if(strcmp(Token,"*PLAYER_DIED") == 0)	
		{
			gFameSet[eFamePlayerDied] = file.GetDword();
		}
		else if(strcmp(Token,"*ONLY_IN_WAR_CH") == 0)	
		{
			gFameSet[5] = file.GetBool();
		}
		else if(strcmp(Token,"*ONLY_WHEN_WAR_RUNNING") == 0)	
		{
			gFameSet[6] = file.GetBool();
		}
		else if (strcmp(Token, "*OVERIDE_PVP_SYSTEM") == 0)
		{
			gFameSet[eFameOveridedByPVPSystem] = file.GetBool();
		}
		else if (strcmp(Token, "*OPPONENT_FAME_LIMITER") == 0)
		{
			gFameSet[eFameNoIncreaseIfOpponentVal] = file.GetDword();
		}
		

		char FameMapFlag[256];
		FAMETYPE FameTemp;
		sprintf(FameMapFlag,"*MONSTER_KILL_COUNT_%d",g_pServerSystem->GetMapNum());

		if(strcmp(Token,FameMapFlag) == 0)
		{

			FameTemp		=file.GetDword();//::GetPrivateProfileInt("TEST",FameMapFlag,0,strFileName);
			if(FameTemp>0)
			{
				//g_Console.LOG(4, "triggered");
				gFameSet[1]=FameTemp;
			}
		}

		

		/*if(strcmp(Token,"*ONLY_WHEN_WAR_RUNNING") == 0)	
		{
			gFameSet[6] = file.GetDword();
		}*/
	}
	file.Release();

	return TRUE;
}
void CServerSystem::ToggleCheckProcessTime()
{
	m_bCheckProcessTime = ! m_bCheckProcessTime;

	if(m_bCheckProcessTime)
	{
		//m_fTimeObjLoopProc = 0.0f;
		//m_fTimeTriggerProc = 0.0f;
		m_fTimeDungeonProc = 0.0f;

		//m_fAvrObjLoopProc = 0.0f;
		//m_fAvrTriggerProc = 0.0f;
		m_fAvrDungeonProc = 0.0f;
		m_dwProcessCount = 0;
	}
}
void ButtonToggleProcessTime()
{
	g_pServerSystem->ToggleCheckProcessTime();
}
void ReLoadBinFile()
{
	g_pServerSystem->ReloadResourceData();
	//g_Console.LOG(4, "Bin reloaded!");
}
unsigned long  CheckOCTREE()
{
	unsigned long dwCrc = 0, dwTemp = 0;
	HANDLE              hSrch;
	WIN32_FIND_DATA     wfd;
	BOOL                bResult = TRUE;

	char strScriptPath[MAX_PATH];
	sprintf(strScriptPath, "%s\\OCTREE\\*.bin", ".");

	hSrch = FindFirstFile(strScriptPath, &wfd);

	int iCount = 0;

	while (bResult)
	{
		if (wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			;
		}
		else
		{
			sprintf(strScriptPath, "%s\\OCTREE\\%s", ".", wfd.cFileName);

			dwTemp = 0;
			if (CCrc32Static::FileCrc32Assembly(strScriptPath, dwTemp) != NO_ERROR)
			{
				;
			}
			dwCrc = dwCrc ^ dwTemp;
			iCount++;
		}

		bResult = FindNextFile(hSrch, &wfd);
	}
	FindClose(hSrch);

	return dwCrc;
}
LONG GetCRC1(char * str)
{
	char strScriptPath[MAX_PATH];
	unsigned long dwCrc[1], dwTemp;
	unsigned long m_dwVersion;

	char strCheckSum[MAX_PATH];

	ZeroMemory(dwCrc, sizeof(unsigned long)* 1);

//	strcpy(m_strClientPath, ".");
	sprintf(strScriptPath, "%s\\%s", ".", str);
	if (CCrc32Static::FileCrc32Assembly(strScriptPath, dwCrc[0]) != NO_ERROR)
	{
		sprintf(strCheckSum, "error1");
	}

	dwTemp = CheckOCTREE();
	dwCrc[0] = dwCrc[0] ^ dwTemp;

	m_dwVersion = dwCrc[0];

	//m_dwCrc1 = m_dwVersion;

	return m_dwVersion;
}
void printLong(LONG long2print)
{
	TCHAR str[1024];
	wsprintf(str, TEXT("0x%08x"), long2print);
	::MessageBox(NULL, str, TEXT(""), MB_OK);
}
void DevTest()
{
	printLong(GetCRC1("\\resource\\itemlist.bin"));
}
void ReloadFortWar()
{
	FORTWARMGR->Init();
}

void ReloadConfig()
{
	g_pServerSystem->ReadServerConfig(g_pServerSystem->GetMapNum());  // Reload The Game Config 2014-05-06
}
void ReloadTest()
{
	//g_Console.LOG(4, "DungeonMGR->GetPlayerNumInDungeon(2):%d",DungeonMGR->GetPlayerNumInDungeon(2));
}

void CServerSystem::ReloadBloodStorm()
{
	gBloodStorm=::GetPrivateProfileInt("BloodStormInfo","Enable",0,".\\BloodStorm.ini");
}
void ReloadTurn()
{
	GAMERESRCMNGR->LoadRebornSetting();
}
void ReloadNULL()
{
	//ITEMDROP_OBJ->DevMonsterDrop(1);
}



void CServerSystem::SetReloadWantedMapInfo()
{
	char filename[64];
	CMHFile file;
	sprintf(filename,"./Resource/Server/WantedMapReload.bin");
	if(!file.Init(filename, "rb"))
		return;

	int n =0;
	while(1)
	{
		if(file.IsEOF())
			break;

		m_ReloadWantedMap[n].MoveMapNum = file.GetWord();
		SafeStrCpy( m_ReloadWantedMap[n].ObjectName, file.GetString(), MAX_NAME_LENGTH+1 );
		
		++n;
	}
	
	file.Release();
}
BOOL CServerSystem::GetReloadWantedMapInfo(DWORD Index)
{
	for(int i=0; i<MAX_POINT_NUM; i++)
	{
		if(m_ReloadWantedMap[i].MoveMapNum == Index)
			return TRUE;
	}
	return FALSE;
}