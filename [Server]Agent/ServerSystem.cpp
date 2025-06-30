




// ServerSystem.cpp: implementation of the CServerSystem class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include <string>           // ÒýÓÃstring ·â°üÃÜÔ¿³×»áÓÃµ½ 2014-12-06
#include "ServerSystem.h"
#include "Console.h"
#include "Network.h"
#include "DataBase.h"
#include "CommonNetworkMsgParser.h"
#include "AgentNetworkMsgParser.h"

#include "Usertable.h"
#include "Servertable.h"

#include <MemoryPoolTempl.h>

#include "AgentDBMsgParser.h"
#include "Crypt.h"

#include "FilteringTable.h"
#include "MHTimeManager.h"
#include "BootManager.h"
#include "MHFile.h"
#include "AgentDBMsgParser.h"
#include "TrafficLog.h"
#include "GMPowerList.h"
#include "ShoutManager.h"
#include "PlustimeMgr.h"

#include "SkillDalayManager.h"
#include "JackpotManager_Agent.h"
#include "BobusangManager_Agent.h"
#include "PunishManager.h"
#include "TopManager.h"       // 2018-01-10 ÅÅÐÐÏµÍ³Í·ÎÄ¼þÒýÓÃ
#include "GambleManager.h"    // 2015-01-28 ¶Ä²©ÏµÍ³Í·ÎÄ¼þÒýÓÃ
#include "TimerManager.h"     // 2018-02-08 ¶¨Ê±Æ÷ÏûÏ¢Í·ÎÄ¼þÒýÓÃ
#include "AUTOBACKUP.h"

#include "..\[CC]ServerModule\MiniDumper.h"

#include "../[CC]JACK/JackJack.h"
#include "FireWallManager.h"

#ifdef _HACK_SHIELD_
#include "HackShieldManager.h"
#endif

#ifdef _NPROTECT_
#include "ggsrv25.h"
#include "NProtectManager.h"
#endif

#ifdef _NETWORKTEST
DWORD RecvServerMsgAmount[MP_MAX] = {0,};
DWORD RecvUserMsgAmount[MP_MAX] = {0,};
extern DWORD SendServerMsgAmount[MP_MAX];
extern DWORD SendUserMsgAmount[MP_MAX];
#endif

void _stdcall fConnectionCheck(DWORD dwEventIndex);

void ButtonReloadRanking();
void ButtonToggleLoadFBInfoTest();

//void ReloadSemuaBinMap();
void ReloadNULL();


DWORD g_dwAcceptUser = 0;
BOOL g_bCloseWindow = FALSE;
extern DWORD g_dwAcceptUser2;
extern int	g_nServerSetNum;

HWND g_hWnd;


// 

std::string PacketEnDesStr=NetServer;


// taiyo 
CServerSystem * g_pServerSystem = NULL;
// MAPSERVER¿¡¼­´Â USERINFO°¡ ¾Æ´Ï¶ó CPlayer°¡ µÈ´Ù!
CMemoryPoolTempl<USERINFO> g_UserInfoPool;
#ifdef _NPROTECT_
CMemoryPoolTempl<CCSAuth2> g_CCSAuth2Pool;
#endif
CServerSystem::CServerSystem()
{
	CoInitialize(NULL);


	/*g_UserInfoPool.Init(MAX_USER_IN_A_AGENT/2, 500,"UserInfoPool");
	g_pUserTable = new CUserTable;					// KEY : dwConnectionIndex
	g_pUserTable->Init(MAX_USER_IN_A_AGENT/2, 0);
	g_pUserTableForUserID = new CUserTable;			// KEY : dwUserIdx
	g_pUserTableForUserID->Init(MAX_USER_IN_A_AGENT/2, 0);
	g_pUserTableForObjectID = new CUserTable;		// KEY : dwObjectID
	g_pUserTableForObjectID->Init(MAX_USER_IN_A_AGENT/2, OnRemoveObjProc);
	g_pServerTable = new CServerTable;				// KEY : ServerPort
	g_pServerTable->Init(MAX_SERVER_CONNECTION);*/

#ifdef _CRYPTCHECK_ 
	m_bUseCrypt = TRUE;
#else
	m_bUseCrypt = FALSE;
#endif

	m_Nation = eNATION_KOREA;
	m_bUseEventNotify = FALSE;
	ZeroMemory( m_strEventNotifyTitle, sizeof( m_strEventNotifyTitle ) );
	ZeroMemory( m_strEventNotifyContext, sizeof( m_strEventNotifyContext ) );
	memset( m_bEvenList, 0, sizeof(BOOL)*eEvent_Max );

	m_bTestServer = FALSE;

	m_bUseEventNotifyGet = FALSE;
	ZeroMemory(m_strEventNotifyGetTitle, sizeof(m_strEventNotifyGetTitle));
	ZeroMemory(m_strEventNotifyGetContext, sizeof(m_strEventNotifyGetContext));

/*
#ifdef _HK_LOCAL_
	m_dwFPSTimeTest = 0;
	m_nDBProcessDelay = 0;
#endif
*/

}

CServerSystem::~CServerSystem()
{
	CoUninitialize();
}

void CServerSystem::Start(WORD ServerNum)
{
	


//ÀÓ½Ã ·ÎÄÃ
	SetNation();

	m_wServerNum = ServerNum;
	srand(GetTickCount());

	//SW050729

	JACKPOTMGR->Start();

	BOBUSANGMGR->BobusangMgr_Start();

//	m_IdxCreater = ICCreate();
//	ICInitialize(m_IdxCreater,5000);
	char TitleText[255];
	sprintf(TitleText, "Agent Server: %d", ServerNum);

	//g_hWnd = GetActiveWindow();
	//SetWindowText(g_hWnd, TitleText);

	// 20
	MENU_CUSTOM_INFO menu[] = {
		{" ", ReloadNULL},
		{"Reload Ranking", ButtonReloadRanking},
		{"Reload Bin",ReloadAllBin},
		{"CLR_LoginEvent",ClearLoginEventDB},
		{ "BACKUP_DB", BACKUP_DB }
		//,{"LoadFBInfo",ButtonToggleLoadFBInfoTest}
	};
	if (FALSE == g_Console.Init(TitleText,sizeof(menu) / sizeof(*menu), menu, OnCommand))
	{
		MessageBox(
			0,
			"Console initializing Failed",
			0,
			0);
	}

	/*MENU_CUSTOM_INFO pMenu[4];
	strcpy(pMenu[0].szMenuName, "Connection Count");
	pMenu[0].cbMenuCommand = ButtonProc1;
	strcpy(pMenu[1].szMenuName, "Button2");
	pMenu[1].cbMenuCommand = ButtonProc2;
	strcpy(pMenu[2].szMenuName, "Button3");
	pMenu[2].cbMenuCommand = ButtonProc3;
	// 
	strcpy(pMenu[3].szMenuName, "GambleOnce");
	pMenu[3].cbMenuCommand = GambleOnceExec;

	if(!g_Console.Init(4,pMenu,OnCommand))
		MessageBox(NULL,"Console Initializing Failed",0,0);*/

	
	

//	DWORD    dwProcessID = NULL;
  //  DWORD    dwTreadID = ::GetWindowThreadProcessId(g_hWnd, &dwProcessID );

//	HOOKPROC hProc;
//    hProc = CallWndProc;
//    hHook = ::SetWindowsHookEx(WH_CALLWNDPROC, hProc, (HINSTANCE)NULL, dwTreadID);


	SetMapChangeInfo();
	SetRelogMapChangeInfo();
	//////////////////////////////////////////////////////////////////////////
	// ³×Æ®¿öÅ© ¼³Á¤

	/// ³×Æ®¿öÅ© ÆÄ¼­ ¼³Á¤-------------------------------
	g_pServerMsgParser[0] = NULL;	// YH 0¹øÀº ºñ?E? MP_SERVER == 1
	for(int i=1 ; i<MP_MAX ; ++i)
		g_pServerMsgParser[i] = TransToClientMsgParser;
	g_pServerMsgParser[MP_SERVER] = MP_AGENTSERVERMsgParser;
	g_pServerMsgParser[MP_POWERUP] = MP_POWERUPMsgParser;
	//g_pServerMsgParser[MP_CHAR] = MP_CHARMsgParser;
	g_pServerMsgParser[MP_MAP] = TransToClientMsgParser;
	g_pServerMsgParser[MP_ITEM] = MP_ITEMServerMsgParser;
	g_pServerMsgParser[MP_ITEMEXT] = MP_ITEMServerMsgParserExt;
	g_pServerMsgParser[MP_CHAT] = MP_CHATServerMsgParser;
	g_pServerMsgParser[MP_USERCONN] = MP_USERCONNMsgParser;
	g_pServerMsgParser[MP_MOVE] = TransToClientMsgParser;
	g_pServerMsgParser[MP_MUGONG] = TransToClientMsgParser;
	g_pServerMsgParser[MP_AUCTIONBOARD] = TransToClientMsgParser;
	g_pServerMsgParser[MP_CHEAT] = TransToClientMsgParser;
	g_pServerMsgParser[MP_QUICK] = TransToClientMsgParser;
	g_pServerMsgParser[MP_PACKEDDATA] = MP_PACKEDMsgParser;
	g_pServerMsgParser[MP_PARTY] = MP_PARTYServerMsgParser;	

	g_pServerMsgParser[MP_FRIEND] = MP_FRIENDMsgParser;
	g_pUserMsgParser[MP_NOTE] = MP_NOTEMsgParser;
	g_pServerMsgParser[MP_NOTE] = MP_NOTEServerMsgParser;
	g_pServerMsgParser[MP_WANTED] = MP_WANTEDServerMsgParser;
	
	g_pServerMsgParser[MP_AUCTION] = TransToClientMsgParser;
	g_pServerMsgParser[MP_MORNITORMAPSERVER] = MP_MonitorMsgParser;
	g_pServerMsgParser[MP_MURIMNET] = MP_MURIMNETServerMsgParser;
	g_pServerMsgParser[MP_CHEAT] = MP_CHEATServerMsgParser;
	g_pServerMsgParser[MP_HACKCHECK] = MP_HACKCHECKMsgParser;
	g_pServerMsgParser[MP_GUILD] = MP_GUILDServerMsgParser;
	g_pServerMsgParser[MP_GUILD_FIELDWAR] = MP_GUILD_FIELDWARServerMsgParser;	
	g_pServerMsgParser[MP_JACKPOT] = MP_JACKPOTServerMsgParser;
	g_pServerMsgParser[MP_SKILL] = MP_SkillServerMsgParser;
	g_pServerMsgParser[MP_GUILD_UNION] = MP_GUILD_UNIONServerMsgParser;
	g_pServerMsgParser[MP_GTOURNAMENT] = MP_GTOURNAMENTServerMsgParser;
	g_pServerMsgParser[MP_SIEGEWAR] = MP_SIEGEWARServerMsgParser;
	g_pServerMsgParser[MP_SIEGEWAR_PROFIT] = MP_SIEGEWARPROFITServerMsgParser;
	g_pServerMsgParser[MP_SURVIVAL] = MP_SURVIVALServerMsgParser;
	g_pServerMsgParser[MP_BOBUSANG] = MP_BOBUSANGServerMsgParser;
	g_pServerMsgParser[MP_OPTION] = TransToClientMsgParser;
	g_pServerMsgParser[MP_ITEMLIMIT] = MP_ITEMLIMITServerMsgParser;
	g_pServerMsgParser[MP_AUTONOTE] = MP_AUTONOTEServerMsgParser;
	g_pServerMsgParser[MP_FORTWAR] = MP_FORTWARServerMsgParser;
	g_pServerMsgParser[MP_MARRY] = MP_MARRYMsgParser;           // the marry agent msg send 2014-05-10 
	g_pServerMsgParser[MP_CLIENT] = MP_CLIENTMsgParser;         // the client agent msg send 2014-05-18
 
	g_pServerMsgParser[MP_GUILDWAR]	= MP_GUILDWARServerMsgParser;
	g_pServerMsgParser[MP_INSDUNGEON] = MP_INSDUNGEONServerMsgParser;
	for(int i=1 ; i<MP_MAX ; ++i)
		g_pUserMsgParser[i] = TransToMapServerMsgParser;
	g_pUserMsgParser[MP_USERCONN] = MP_USERCONNMsgParser;
	g_pUserMsgParser[MP_CHAT] = MP_CHATMsgParser;
	g_pUserMsgParser[MP_FRIEND] = MP_FRIENDMsgParser;
	g_pUserMsgParser[MP_NOTE] = MP_NOTEMsgParser;
	g_pUserMsgParser[MP_MURIMNET] = MP_MURIMNETUserMsgParser;
	g_pUserMsgParser[MP_OPTION] = MP_OPTIONUserMsgParser;
	g_pUserMsgParser[MP_DEBUG] = MP_DebugMsgParser;
	g_pUserMsgParser[MP_CHEAT] = MP_CHEATUserMsgParser;
	g_pUserMsgParser[MP_HACKCHECK] = MP_HACKCHECKMsgParser;
	g_pUserMsgParser[MP_GUILD] = MP_GUILDUserMsgParser;
	g_pUserMsgParser[MP_GUILD_FIELDWAR] = MP_GUILD_FIELDWARUserMsgParser;
	g_pUserMsgParser[MP_ITEM] = MP_ITEMUserMsgParser;
	g_pUserMsgParser[MP_ITEMEXT] = MP_ITEMUserMsgParserExt;
	g_pUserMsgParser[MP_PARTY] = MP_PARTYUserMsgParser;		//2008. 5. 26. CBH - ÆÄÆ¼ ¸ÅÄª ½Ã½ºÅÆ °ü·Ã Ãß°¡
	g_pUserMsgParser[MP_GTOURNAMENT] = MP_GTOURNAMENTUserMsgParser;
	g_pUserMsgParser[MP_JACKPOT] = MP_JACKPOTUserMsgParser;
	g_pUserMsgParser[MP_SKILL] = MP_SkillUserMsgParser;
	g_pUserMsgParser[MP_GUILD_UNION] = MP_GUILD_UNIONUserMsgParser;
	g_pUserMsgParser[MP_SIEGEWAR] = MP_SIEGEWARUserMsgParser;
	g_pUserMsgParser[MP_SIEGEWAR_PROFIT] = MP_SIEGEWARPROFITUserMsgParser;
	g_pUserMsgParser[MP_SURVIVAL] = MP_SURVIVALUserMsgParser;
	g_pUserMsgParser[MP_BOBUSANG] = MP_BOBUSANGUserMsgParser;

	g_pUserMsgParser[MP_EXCHANGE] = MP_EXCHANGEUserMsgParser;
	g_pUserMsgParser[MP_STREETSTALL] = MP_STREETSTALLUserMsgParser;

	// 2005 Å©¸®½º¸¶½º ÀÌº¥Æ®
	// GM¿¡ ÀÇÇÑ ³¯¾¾ ¼ÂÆÃÀ» ¸Êº°·Î ÇÏ?EÀ§ÇØ...
	g_pUserMsgParser[MP_WEATHER] = MP_WEATHERUserMsgParser;
	g_pUserMsgParser[MP_AUTONOTE] = MP_AUTONOTEUserMsgParser;

	//g_pUserMsgParser[MP_FBTIMER]= MP_FBTIMERUserMsgParser; 

	g_pUserMsgParser[MP_DUNGEON] = MP_DUNGEON_UserMsgParser;

	g_pUserMsgParser[MP_INSDUNGEON] = MP_INSDUNGEONUserMsgParser;
	g_pUserMsgParser[MP_NEWUPGRARE_ALEXX] = MP_NEWUPGRARE_ALEXXMsgParser;
	// servertable usertable ÃÊ±âÈ­ ---------------------

	
	g_UserInfoPool.Init(MAX_USER_IN_A_AGENT/2, 500,"UserInfoPool");

	/*g_pUserTable = new CUserTable;					// KEY : dwConnectionIndex
	g_pUserTable->Init(MAX_USER_IN_A_AGENT/2);
	g_pUserTableForUserID = new CUserTable;			// KEY : dwUserIdx
	g_pUserTableForUserID->Init(MAX_USER_IN_A_AGENT/2);
	g_pUserTableForObjectID = new CUserTable;		// KEY : dwObjectID
	g_pUserTableForObjectID->Init(MAX_USER_IN_A_AGENT/2);
	g_pServerTable = new CServerTable;				// KEY : ServerPort
	g_pServerTable->Init(MAX_SERVER_CONNECTION);*/

	g_pUserTable = new CUserTable;					
	g_pUserTable->Init(MAX_USER_IN_A_AGENT/2, 0);
	g_pUserTableForUserID = new CUserTable;			
	g_pUserTableForUserID->Init(MAX_USER_IN_A_AGENT/2, 0);
	g_pUserTableForObjectID = new CUserTable;		
	g_pUserTableForObjectID->Init(MAX_USER_IN_A_AGENT/2, OnRemoveObjProc);
	g_pServerTable = new CServerTable;				
	g_pServerTable->Init(MAX_SERVER_CONNECTION);

	/*g_UserInfoPool.Init(MAX_USER_IN_A_AGENT/2, 500,"UserInfoPool");
	g_pUserTable = new CUserTable;					
	g_pUserTable->Init(MAX_USER_IN_A_AGENT/2);
	g_pUserTableForUserID = new CUserTable;			
	g_pUserTableForUserID->Init(MAX_USER_IN_A_AGENT/2);
	g_pUserTableForObjectID = new CUserTable;		
	g_pUserTableForObjectID->Init(MAX_USER_IN_A_AGENT/2);
	g_pServerTable = new CServerTable;				
	g_pServerTable->Init(MAX_SERVER_CONNECTION);*/
	
	CUSTOM_EVENT ev[3];
	ev[0].dwPeriodicTime	= 500;
	ev[0].pEventFunc		= GameProcess;
//	ev[1].dwPeriodicTime	= 100;
	ev[1].dwPeriodicTime	= 0;
	ev[1].pEventFunc		= ProcessDBMessage;
	ev[2].dwPeriodicTime	= 1000*30;	// 30sec
	ev[2].pEventFunc		= fConnectionCheck;

	DESC_NETWORK desc;
	desc.OnAcceptServer = OnAcceptServer;
	desc.OnDisconnectServer = OnDisconnectServer;
	desc.OnAcceptUser = OnAcceptUser;
	desc.OnDisconnectUser = OnDisconnectUser;
	desc.OnRecvFromServerTCP = ReceivedMsgFromServer;
	desc.OnRecvFromUserTCP = ReceivedMsgFromUser;
	desc.dwCustomDefineEventNum = sizeof( ev ) / sizeof( *ev );//3;
	desc.pEvent = ev;

	desc.dwMainMsgQueMaxBufferSize = 20480000;			//max 20480000
	desc.dwMaxServerNum = MAX_SERVER_CONNECTION;		//max 3000
	desc.dwMaxUserNum = MAX_USER_IN_A_AGENT;			//max 3000
	desc.dwServerBufferSizePerConnection = 512000;		//max 512000
	desc.dwServerMaxTransferSize = 65535;				//max 65535

	desc.dwUserBufferSizePerConnection = 256000;		//max 512000
	desc.dwUserMaxTransferSize = 65535;					//max 65535
	desc.dwConnectNumAtSameTime = MAX_USER_IN_A_AGENT/4;	//about divide by 4
	desc.dwFlag = 0;

	g_Network.Init(&desc);

	BOOTMNGR->AddSelfBootList(AGENT_SERVER, ServerNum, g_pServerTable);
	if(!BOOTMNGR->StartServer(&g_Network, g_pServerTable->GetSelfServer()))
	{
		ASSERT(0);
	}
	if(!BOOTMNGR->ConnectToMS(&g_Network, g_pServerTable))
	{
		SERVERINFO info;
		info.wServerKind = MONITOR_SERVER;

		OnConnectServerFail(&info);		
	}


	DWORD maxthread = 2;
	DWORD maxqueryinsametime = 1024;
	FILE* fp = fopen("AgentDBInfo.txt","r");
	if(fp)
	{
		fscanf(fp,"%d %d",&maxthread,&maxqueryinsametime);
		fclose(fp);
	}
	if(g_DB.Init(maxthread,maxqueryinsametime,TRUE) == FALSE)
		MessageBox(NULL,"DataBase Initializing Failed",0,0);


//	g_Network.Start();

//KES
	//ÇÊÅÍ¸µ ÃÊ±âÈ­
	FILTERTABLE->Init();
	GMINFO->Init();

	PLUSTIMEMGR->Init();

	//TRAFFIC->StartRecord();
	TRAFFIC->Init();
	//TRAFFIC->WriteUserTrafficLogFile();
	
	SKILLDELAYMGR->LoadSkillUseInfo();

	//SW070626 º¸ºÎ»óNPC
	BOBUSANGMGR->BobusangMgr_Init();

	// punish
	PUNISHMGR->Init();

	// Dump
#ifdef _HK_LOCAL_
	MiniDumper md(MiniDumper::DUMP_LEVEL_0);
#elif defined _TL_LOCAL_
	MiniDumper md(MiniDumper::DUMP_LEVEL_0);
#elif defined _CHINA_LOCAL_
	MiniDumper md(MiniDumper::DUMP_LEVEL_0);
#elif defined _JAPAN_LOCAL_
	MiniDumper md(MiniDumper::DUMP_LEVEL_0);
#else
	MiniDumper md(MiniDumper::DUMP_LEVEL_2);
#endif

	
	
	g_Console.LOG(4, "-----------   AGENT SERVER START  -----------------");

	g_bReady = TRUE;

//	if(FALSE==JACKMGR->CheckSumInit(g_hWnd))
//		g_bReady = false;
//	else
//		g_bReady = true;

	

	TOPMGR->InitTopList();

	

	//TOPMGR->LoadFlushTimeConfig();
    
	char txt[128];
	sprintf(txt, "EXEC up_Agent_Down %d, %d", g_nServerSetNum , g_pServerTable->GetSelfServer()->wServerNum);
	g_DB.LoginQuery(eQueryType_FreeQuery, eAgentDown, 0, txt);
	LoadFireInfoFormDB();
	g_Console.WaitMessage();
}

void CServerSystem::Process()
{
	JACKMGR->Process();

	if(g_bCloseWindow)
	{
		SendMessage(g_hWnd, WM_CLOSE, 0,0);
		g_bCloseWindow = FALSE;
		g_bReady = FALSE;
	}

	MHTIMEMGR_OBJ->Process();

	//g_Console.LOG(4, "loop!");

	
	//JACKMGR->Process();

//db que³»¿ëÀ» Ç×»ó »ìÇÉ´Ù.
	g_DB.ProcessingQueue();
//	g_DB.ProcessingDBMessage();
//


	TRAFFIC->Process();

	SHOUTMGR->Process();
	
	//SW050729

	JACKPOTMGR->Process();

	//SW070626 º¸ºÎ»óNPC
	BOBUSANGMGR->BobusangMgr_Process();

	// punish
	PUNISHMGR->Process();

    //weiye 2016-03-20 ÅÅÐÐ×Ô¶¯Ë¢ÐÂ½ø³Ì 2016-03-20

	TOPMGR->Process();

	

	//GAMBLEMGR->Process();  // 2015-01-26 ¶Ä²©½ø³ÌÉèÖÃ!

	//TIMERMGR->Process();

	AUTOBACKUPMGR->Process();

#ifdef _NPROTECT_
	NPROTECTMGR->Update();
#endif

	
}

void CServerSystem::ConnectionCheck()
{	// YH Çö?E30ÃÊ¸¶´Ù ÇÑ¹ø¾¿ µé¾ûÛÈ
	DWORD _60sec = 60*1000;
	USERINFO* pInfo;
	DWORD elapsedtime;
	static int nForBill = 0;

	if(g_bReady == FALSE)
		return;



	

	
	cPtrList removelist;
	cPtrList disconnectlist;

	g_pUserTableForUserID->SetPositionHead();
	while(pInfo = g_pUserTableForUserID->GetData())
	{
		if(pInfo->dwConnectionIndex == 0)
		{
			// ¾ÆÁ÷ Á¢¼ÓÀÌ Á¦´ë·Î ÀÌ·ïÁöÁö ¾ÊÀº °æ¿ì
			elapsedtime = gCurTime - pInfo->dwLastConnectionCheckTime;
			if(elapsedtime > _60sec*2)
			{
				removelist.AddTail(pInfo);
			}
		}
		else
		{
			//1. ¸Ê ·Îµù(¸ÊÀÌµ¿)ÁßÀÇ ½Ã°£Àº ¾î¶»°Ô Ã³¸®ÇÏ³ª?
			//2. Ä³¸¯ÅÍ ¼±Ã¥Ã¢ÀÇ ½Ã°£Àº ¾î¶»°Ô Ã³¸®ÇÏ³ª?
#ifdef _CHINA_LOCAL_
			if( pInfo->bBillType == 8 || pInfo->bBillType == 7 )
			{
				DWORD dwElapsedRemainTime	= gCurTime - pInfo->dwLastCheckRemainTime;
				DWORD dwElapsedSec			= dwElapsedRemainTime/1000; //per sec
				pInfo->nRemainTime			-= dwElapsedSec;
				pInfo->dwLastCheckRemainTime += dwElapsedSec*1000;
		
				if( pInfo->nRemainTime <= 0 )
				{
					pInfo->nRemainTime = 0;
					disconnectlist.AddTail(pInfo);
					//DisconnectUser(pInfo->dwConnectionIndex, 8);
					continue;
				}
			}
#endif
			///////////---Billing

			if( nForBill == 1 )
				SendSpeedHackCheckMsg(pInfo);

			if(pInfo->UserLevel == eUSERLEVEL_PROGRAMMER)
				continue;

			elapsedtime = gCurTime - pInfo->dwLastConnectionCheckTime;
			
			if( elapsedtime > _60sec*10 )	//10min
			{
				// Á¢¼ÓÀÌ ÀÌ·ïÁø °æ¿ì
				if(pInfo->bConnectionCheckFailed)		// ¸¸¾à ÄÁ³Ø¼Ç Ã¼Å©¸¦ º¸³Â´Âµ¥ 1ºÐµ¿¾È ÀÀ´äÀÌ ¾ø¾ú´Ù¸é
				{
					disconnectlist.AddTail(pInfo);
					//DisconnectUser(pInfo->dwConnectionIndex, 1);
					continue;
				}
				else
				{
					pInfo->bConnectionCheckFailed = TRUE;
					SendConnectionCheckMsg(pInfo);
					pInfo->dwLastConnectionCheckTime = gCurTime;
				}
			}
		}
	}

	PTRLISTPOS pos = removelist.GetHeadPosition();
	while( pos )
	{
		USERINFO* p = (USERINFO*)removelist.GetNext(pos);

		LoginCheckDelete(p->dwUserID);		// ·Î±×ÀÎÃ¼Å© Å×ÀÌºí¿¡¼­ »èÁ¦
		g_pUserTableForUserID->RemoveUser(p->dwUserID);
		TRAFFIC->RemoveUser(p->dwUserID);
#ifdef _NPROTECT_
		if( p->m_pCSA )
		{
			p->m_pCSA->Close();
			g_CCSAuth2Pool.Free(p->m_pCSA);

		}
#endif
		memset(p,0,sizeof(USERINFO));
		g_UserInfoPool.Free(p);
	}
	removelist.RemoveAll();

	pos = disconnectlist.GetHeadPosition();
	while( pos )
	{
		USERINFO* p = (USERINFO*)disconnectlist.GetNext(pos);
		DisconnectUser(p->dwConnectionIndex);
	}
	disconnectlist.RemoveAll();


	if( nForBill == 1 )		//2¹ø¿¡ ÇÑ¹ø... Áï, 1ºÐ¸¶´Ù.
	{
#ifdef _CHINA_LOCAL_
		BillingUpdateAll( m_wServerNum );
#endif
		nForBill = 0;
	}
	else
	{
		++nForBill;
	}
	
	// Plustime
#ifndef _TL_LOCAL_
	PLUSTIMEMGR->Process();
#endif
}

void CServerSystem::SendConnectionCheckMsg(USERINFO* pInfo)
{
	MSGBASE msg;
	msg.Category = MP_USERCONN;
	msg.Protocol = MP_USERCONN_CONNECTION_CHECK;
	g_Network.Send2User(pInfo->dwConnectionIndex,(char*)&msg,sizeof(msg));

/*
	MSG_DWORD msg;
	msg.Category	= MP_USERCONN;
	msg.Protocol	= MP_USERCONN_CONNECTION_CHECK;
	msg.dwData		= gCurTime;
	g_Network.Send2User(pInfo->dwConnectionIndex,(char*)&msg,sizeof(msg));
*/
}

void CServerSystem::SendSpeedHackCheckMsg(USERINFO* pInfo)
{
	if( pInfo->wUserMapNum )
	{
		MSG_DWORD msg;
		msg.Category	= MP_HACKCHECK;
		msg.Protocol	= MP_HACKCHECK_SPEEDHACK;
		msg.dwData		= gCurTime;	//ÀÛÀº ½Ã°£À» ÁÖ´Â °ÍÀº ±¦Âú´Ù.
		g_Network.Send2User(pInfo->dwConnectionIndex,(char*)&msg,sizeof(msg));
	}

#ifdef _HACK_SHIELD_
	HACKSHIELDMGR->SendReq(pInfo);
#endif
#ifdef _NPROTECT_
	// 3ºÐ °£°ÝÀ¸·Î Á¡°Ë
//	static DWORD NProtectCheckTime = gCurTime;

	if( pInfo->m_nCSAInit == 1 || pInfo->m_nCSAInit == 2 ) //·Î±×ÀÎ½Ã Ã³À½ 2¹ø ÀÎÁõ Áß
	{
		if( gCurTime - pInfo->dwLastNProtectCheck >= 1000*60 ) //60ÃÊ³»·Î ÀÎÁõ °ªÀÌ ¾È¿À¸é
		{
			pInfo->m_nCSAInit = 4;	//Á¢¼Ó ²÷À½.

			MSGBASE msg;
			msg.Category = MP_NPROTECT;
			msg.Protocol = MP_NPROTECT_DISCONNECT;
			g_Network.Send2User(pInfo->dwConnectionIndex,(char*)&msg,sizeof(msg));
			if( pInfo->UserLevel >= eUSERLEVEL_GM )
				DisconnectUser(pInfo->dwConnectionIndex);
		}
	}
	else if( pInfo->m_nCSAInit == 3 )
	{
		if(gCurTime - pInfo->dwLastNProtectCheck >= 1000*60*3)
		{
			NPROTECTMGR->SendAuthQuery(pInfo);
		}
	}
#endif
}

void CServerSystem::End()
{
	g_bReady = FALSE;

	//TRAFFIC->EndRecord();
	TRAFFIC->End();

//	UnhookWindowsHookEx(hHook);

	
	g_pUserTableForUserID->SetPositionHead();
	USERINFO* pInfo;
	while(pInfo = g_pUserTableForUserID->GetData())
	{
		LoginCheckDelete(pInfo->dwUserID);
		g_pServerSystem->ReleaseAuthKey(pInfo->dwUniqueConnectIdx);

#ifdef _NPROTECT_
		pInfo->m_pCSA->Close();
		g_CCSAuth2Pool.Free(pInfo->m_pCSA);
#endif
		g_UserInfoPool.Free(pInfo);
	}
	g_pUserTableForUserID->RemoveAll();

//	ICRelease(m_IdxCreater);

	// punish
	PUNISHMGR->Release();
	
	g_DB.Release();
	g_Network.Release();
	g_Console.Release();

	if(g_pUserTable)
	{
		g_pUserTable->RemoveAllUser();
		delete g_pUserTable;
		g_pUserTable = NULL;
	}
	if(g_pUserTableForObjectID)
	{
		g_pUserTableForObjectID->RemoveAllUser();
		delete g_pUserTableForObjectID;
		g_pUserTableForObjectID = NULL;
	}
	if(g_pServerTable)
	{
		g_pServerTable->RemoveAllServer();
		delete g_pServerTable;
		g_pServerTable = NULL;
	}
	CoFreeUnusedLibraries();
	FIREWALLMGR->Release();
}

void CServerSystem::SetMapChangeInfo()
{
	char filename[64];
	CMHFile file;
#ifdef _FILE_BIN_
	sprintf(filename,"./Resource/MapChange.bin");
	if(!file.Init(filename, "rb"))
		return;
#else
	sprintf(filename,"./Resource/MapChange.txt");
	if(!file.Init(filename, "rt"))
		return;
#endif

	int n =0;
	while(1)
	{
		if(file.IsEOF())
			break;
		
		m_MapChange[n].Kind = file.GetWord();
//		file.GetString(m_MapChange[n].CurMapName);
//		file.GetString(m_MapChange[n].ObjectName);
		SafeStrCpy( m_MapChange[n].CurMapName, file.GetString(), MAX_NAME_LENGTH+1 );
		SafeStrCpy( m_MapChange[n].ObjectName, file.GetString(), MAX_NAME_LENGTH+1 );
		m_MapChange[n].CurMapNum = file.GetWord();
		m_MapChange[n].MoveMapNum = file.GetWord();
		m_MapChange[n].CurPoint.x = file.GetFloat();
		m_MapChange[n].CurPoint.y = 0;

		m_MapChange[n].CurPoint.z = file.GetFloat();
		m_MapChange[n].MovePoint.x = file.GetFloat();
		m_MapChange[n].MovePoint.y = 0;
		m_MapChange[n].MovePoint.z = file.GetFloat();
		m_MapChange[n].chx_num = file.GetWord();
		
		++n;
	}
	
	file.Release();
}

MAPCHANGE_INFO* CServerSystem::GetMapChangeInfo(DWORD Index)
{
	for(int i=0; i<MAX_POINT_NUM; i++)
	{
		if(m_MapChange[i].Kind == Index)
			return &m_MapChange[i];
	}

	return NULL;
}




void CServerSystem::SetRelogMapChangeInfo()
{
	char filename[64];
	CMHFile file;
	sprintf(filename,"./Resource/Server/NoLastMap.bin");
	if(!file.Init(filename, "rb"))
		return;

	int n =0;
	while(1)
	{
		if(file.IsEOF())
			break;

		m_RelogMapChange[n].MoveMapNum = file.GetWord();
		SafeStrCpy( m_RelogMapChange[n].ObjectName, file.GetString(), MAX_NAME_LENGTH+1 );
		
		++n;
	}
	
	file.Release();
}
BOOL CServerSystem::GetRelogMapChangeInfo(DWORD Index)
{
	for(int i=0; i<MAX_POINT_NUM; i++)
	{
		if(m_RelogMapChange[i].MoveMapNum == Index)
			return FALSE;//return &m_RelogMapChange[i];
	}

	return TRUE;//NULL;
}

//¸Ê¼­¹ö ´Ù¿î½Ã ´Ù¿îµÈ ¸Ê¿¡ ÀÖ´Â À¯Àú¿¡°Ô ¸Ê´Ù¿îÀ» ¾Ë¸²
void CServerSystem::SendMsgMapServerDown( WORD wServerNum )
{
	USERINFO* pInfo;
	g_pUserTable->SetPositionUserHead();

	while( pInfo = g_pUserTable->GetUserData() )
	{
		if( pInfo->wUserMapNum == wServerNum )
		{
			//OnDisconnectUser( pInfo->dwConnectionIndex );
			DisconnectUser( pInfo->dwConnectionIndex );			
		}
	}
}



DWORD CServerSystem::MakeAuthKey()
{
	//return ICAllocIndex(m_IdxCreater) + 1;
	static DWORD ID = 1;//GetTickCount()*GetTickCount();
	if(ID == 0)
		++ID;
	return ID++;
}
void CServerSystem::ReleaseAuthKey(DWORD key)
{
	//ICFreeIndex(m_IdxCreater,key-1);
}

void CServerSystem::SetNation()
{
	CMHFile file;
	if( file.Init( "LocalizingInfo.txt", "rt", MHFILE_FLAG_DONOTDISPLAY_NOTFOUNDERROR ) )
	{

		if( strcmp( file.GetString(), "*NATION" ) == 0 )
		{
			if( strcmp( file.GetString(), "CHINA" ) == 0 )
			{
				m_Nation = eNATION_KOREA/* eNATION_CHINA*/;
			}
		}

		file.Release();
	}

	/*CMHFile file2;
	if( file2.Init( "_TESTSERVER", "rt", MHFILE_FLAG_DONOTDISPLAY_NOTFOUNDERROR ) )
	{
		m_bTestServer = TRUE;
		file2.Release();
	}*/
}


void CServerSystem::SetEventNotifyStr( char* pStrTitle, char* pStrContext )
{
	SafeStrCpy( m_strEventNotifyTitle, pStrTitle, 32 );
	SafeStrCpy( m_strEventNotifyContext, pStrContext, 128 );
}

void CServerSystem::SetUseEventNotify( BOOL bUse )
{
	m_bUseEventNotify = bUse;

	if( bUse )
	{
		USERINFO* pInfo;
		g_pUserTable->SetPositionUserHead();

		MSG_EVENTNOTIFY_ON msg;
		msg.Category	= MP_CHEAT;
		msg.Protocol	= MP_CHEAT_EVENTNOTIFY_ON;
		SafeStrCpy( msg.strTitle, m_strEventNotifyTitle, 32 );
		SafeStrCpy( msg.strContext, m_strEventNotifyContext, 128 );

		for( int i=0; i<eEvent_Max; ++i )
		{
			if( m_bEvenList[i] )				msg.EventList[i] = 1;
			else								msg.EventList[i] = 0;
		}

		while( pInfo = g_pUserTable->GetUserData() )
		{
			MSG_EVENTNOTIFY_ON msgTemp = msg;
			g_Network.Send2User( pInfo->dwConnectionIndex, (char*)&msgTemp, sizeof(msgTemp) );
		}
	}
	else
	{
		MSGBASE msg;
		msg.Category	= MP_CHEAT;
		msg.Protocol	= MP_CHEAT_EVENTNOTIFY_OFF;

		USERINFO* pInfo;
		g_pUserTable->SetPositionUserHead();

		while( pInfo = g_pUserTable->GetUserData() )
		{
			MSGBASE msgTemp = msg;
			g_Network.Send2User( pInfo->dwConnectionIndex, (char*)&msgTemp, sizeof(msgTemp) );
		}
	}
}
void CServerSystem::SetEventNotifyGetStr( char* pStrTitle, char* pStrContext )
{
	SafeStrCpy( m_strEventNotifyGetTitle, pStrTitle, 32 );
	SafeStrCpy( m_strEventNotifyGetContext, pStrContext, 128 );
}

void CServerSystem::SetUseEventNotifyGet( BOOL bUse )
{
	m_bUseEventNotifyGet = bUse;

	if( bUse )
	{
		USERINFO* pInfo;
		g_pUserTable->SetPositionUserHead();

		MSG_EVENTNOTIFY_ON msg;
		msg.Category = MP_CHEAT;
		msg.Protocol = MP_CHEAT_EVENTNOTIFYGET_ON;
		SafeStrCpy(msg.strTitle, m_strEventNotifyGetTitle, 32);
		SafeStrCpy(msg.strContext, m_strEventNotifyGetContext, 128);

		for (int i = 0; i<eEvent_Max; ++i)
		{
			if (m_bEvenList[i])				msg.EventList[i] = 1;
			else								msg.EventList[i] = 0;
		}

		while (pInfo = g_pUserTable->GetUserData())
		{
			MSG_EVENTNOTIFY_ON msgTemp = msg;
			g_Network.Send2User(pInfo->dwConnectionIndex, (char*)&msgTemp, sizeof(msgTemp));
		}
	}
	else
	{
		MSGBASE msg;
		msg.Category = MP_CHEAT;
		msg.Protocol = MP_CHEAT_EVENTNOTIFYGET_OFF;

		USERINFO* pInfo;
		g_pUserTable->SetPositionUserHead();

		while (pInfo = g_pUserTable->GetUserData())
		{
			MSGBASE msgTemp = msg;
			g_Network.Send2User(pInfo->dwConnectionIndex, (char*)&msgTemp, sizeof(msgTemp));
		}
	}
}
#ifdef _CHINA_LOCAL_
void CServerSystem::SendBillingRemainTimeMsg( USERINFO* pInfo )
{
	if( pInfo->nRemainTime < 0 ) pInfo->nRemainTime = 0;

	//counting start;
	MSG_INT msgTime;
	msgTime.Category	= MP_USERCONN;
	msgTime.Protocol	= MP_USERCONN_REMAINTIME_NOTIFY;
	msgTime.nData		= pInfo->nRemainTime;
		
	g_Network.Send2User( pInfo->dwConnectionIndex, (char*)&msgTime, sizeof(msgTime) );	

}
#endif


// global function

void  _stdcall  OnConnectServerSuccess(DWORD dwConnectionIndex, void* pVoid)
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


	//SERVERINFO* info = (SERVERINFO*)pVoid;
	if(!dwConnectionIndex) return;

	//char strr[255];
	//wsprintf(strr, ""%d´Î·þÎñÆ÷Á¬½Ó³É¹¦¡£", dwConnectionIndex);//pjslocal
	//g_Console.LOG(4, strr);
	
	//kiv
	info->dwConnectionIndex = dwConnectionIndex;					// ÀÌ°Ô Á¦´ë·Î µ¿ÀÛÇÏ³ª? Ã¼Å©
	MSG_WORD msg;
	msg.Category = MP_SERVER;
	msg.Protocol = MP_SERVER_PWRUP;
	msg.wData = g_pServerTable->GetSelfServer()->wPortForServer;
	g_Network.Send2Server(dwConnectionIndex, (char*)&msg, sizeof(msg));


	
	//YH2DO ÁÖ¼®Ã³¸® µÇ¾îÀÖ´ø °ÍÀ» Áö¿ò
	if(g_pServerTable->GetMaxServerConnectionIndex() < dwConnectionIndex)			//??????
		g_pServerTable->SetMaxServerConnectionIndex(dwConnectionIndex);	
}

void _stdcall OnDisconnectServer(DWORD dwConnectionIndex)
{
	char strr[255];
	
	SERVERINFO* serverinfo = g_pServerTable->FindServerForConnectionIndex(dwConnectionIndex);

	wsprintf(strr, "%d : connectionIndex Server disconnected", dwConnectionIndex);//pjslocal
	g_Console.LOG(4, strr);

	// ¿©±â´Ù ³ÊµÎ µÅ³ª??------------------------------------------------------------------------
//	if( serverinfo )
//	if( serverinfo->wServerKind == AGENT_SERVER )	//Agent¼­¹öÀÎ°æ¿ì
//	{
		//´Ù½ÃÁ¢¼Ó...
//		SERVERINFO Info;
//		g_Network->ConnectToServer( serverinfo->szIPForServer, serverinfo->wPortForServer, (void*)&Info );
//		return;
//	}
	//-------------------------------------------------------------------------------------------

	if(serverinfo)
	if( serverinfo->wServerKind == 3 )	//map¼­¹öÀÎ°æ¿ì
	{
		//¸Ê¼­¹ö¿¡ ¿¬°áµÈ À¯Àú¿¡°Ô ¸Ê¼­¹ö ´Ù¿îÀ» ¾Ë¸²
		g_pServerSystem->SendMsgMapServerDown(serverinfo->wServerNum);
	}

	SERVERINFO * delInfo = g_pServerTable->RemoveServer(dwConnectionIndex);
	if(!delInfo)
	{
		ASSERT(0);
	}
	delete delInfo;
}

void _stdcall OnConnectMASSuccess(DWORD dwConnectionIndex, void* pVoid)
{
	MessageBox(0, "OnConnectMASSuccess", 0, 0);
}

void _stdcall OnConnectMASFail(void* pVoid)
{
	MessageBox(0, "OnConnectMASFail", 0, 0);
}


void _stdcall OnConnectServerFail(void* pVoid)
{
	SERVERINFO* info = (SERVERINFO*)pVoid;
	if(info->wServerKind == MONITOR_SERVER)
	{
		BOOTMNGR->AddBootListINI(AGENT_SERVER, g_pServerSystem->GetServerNum(), g_pServerTable);
		BOOTMNGR->BactchConnectToMap(&g_Network, g_pServerTable);
		//g_Console.LOG(4, "Failed to Connect to the MS : %s, %d", info->szIPForServer, info->wPortForServer);
	}
	else
	{
		//g_Console.LOG(4, "Failed to Connect to the Server : %s, %d", info->szIPForServer, info->wPortForServer);
		BOOTMNGR->RemoveBootList(g_pServerTable, info->wPortForServer);
	}	
		
}
void _stdcall  OnAcceptServer(DWORD dwConnectionIndex)
{
	g_Console.LOG(4, "Server Connected : ConnectionIndex %d", dwConnectionIndex );
}

void _stdcall  OnAcceptUser(DWORD dwConnectionIndex)
{
	if(g_bReady == FALSE)
	{
		g_Network.DisconnectUser( dwConnectionIndex );//kiv
		return;
	}
	
	++g_dwAcceptUser;
//---------------------------------kiv
	/*USERINFO* pInfo = g_UserInfoPool.Alloc();
	memset( pInfo, 0, sizeof(USERINFO) );
	pInfo->dwConnectionIndex = dwConnectionIndex;
	pInfo->dwLastConnectionCheckTime = gCurTime;
	g_pUserTable->AddUser( pInfo, dwConnectionIndex );*/
//---------------------------------
	MSGBASE send;
	send.Category = MP_USERCONN;
	send.Protocol = MP_USERCONN_AGENT_CONNECTSUCCESS;
	g_Network.Send2User(dwConnectionIndex, (char*)&send, sizeof(send));

	//event°øÁö
	if( g_pServerSystem->IsUseEventNotify() )
	{
		MSG_EVENTNOTIFY_ON msg;
		msg.Category	= MP_CHEAT;
		msg.Protocol	= MP_CHEAT_EVENTNOTIFY_ON;
		SafeStrCpy( msg.strTitle, g_pServerSystem->GetEventNotifyTitle(), 32 );
		SafeStrCpy( msg.strContext, g_pServerSystem->GetEventNotifyContext(), 128 );


		// magi82(43)
		BOOL* pEventlist = g_pServerSystem->GetApplyEvent();

		for( int i=0; i<eEvent_Max; ++i )
		{
			if( pEventlist[i] )				msg.EventList[i] = 1;
			else							msg.EventList[i] = 0;
		}
				
		g_Network.Send2User(dwConnectionIndex, (char*)&msg, sizeof(msg));
	}	
	if (g_pServerSystem->IsUseEventNotifyGet())
	{
		MSG_EVENTNOTIFY_ON msg;
		msg.Category = MP_CHEAT;
		msg.Protocol = MP_CHEAT_EVENTNOTIFYGET_ON;
		SafeStrCpy(msg.strTitle, g_pServerSystem->GetEventNotifyGetTitle(), 32);
		SafeStrCpy(msg.strContext, g_pServerSystem->GetEventNotifyGetContext(), 128);


		// magi82(43)
		BOOL* pEventlist = g_pServerSystem->GetApplyEvent();

		for (int i = 0; i<eEvent_Max; ++i)
		{
			if (pEventlist[i])				msg.EventList[i] = 1;
			else							msg.EventList[i] = 0;
		}

		g_Network.Send2User(dwConnectionIndex, (char*)&msg, sizeof(msg));
	}
	//user count °è»ê=============================================================================
	SERVERINFO * myInfo = g_pServerTable->GetSelfServer();
	++myInfo->wAgentUserCnt;

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
	//user count °è»ê=============================================================================
	
	

	//¿¡ÀÌÁ¯Æ® ¼­¹öÀÌ¸é À¯Àú Ä«¿îÅÍ¸¦ Áõ°¡½ÃÅ²´Ù.
	DWORD CurUserCount = g_pUserTable->GetUserCount();
	g_pUserTable->SetCalcMaxCount(CurUserCount);
	DWORD MaxUserCount = g_pUserTable->GetUserMaxCount();
	SetUserCurAndMaxCount(CurUserCount, MaxUserCount);
	
	

}

void _stdcall  OnDisconnectUser(DWORD dwConnectionIndex)
{
	if(g_bReady == FALSE)
		return;

//	g_Console.LOG(4, "  User Disconnected : ConnectionIndex %d", dwConnectionIndex);

	g_pUserTable->OnDisconnectUser(dwConnectionIndex);
//	DisconnectUser( dwConnectionIndex );

//	g_Console.LOG(4, "UserCount %d   %d   %d",
//		g_pUserTableForUserID->GetUserCount(),
//		g_pUserTable->GetUserCount(),
//		g_pUserTableForObjectID->GetUserCount());

}

void _stdcall  ProcessDBMessage(DWORD dwEventIndex)
{
	printf("Agent : ProcessDBMessage\n");
	g_DB.ProcessingDBMessage();
}

void _stdcall ReceivedMsgFromServer(DWORD dwConnectionIndex,char* pMsg,DWORD dwLength)
{
	MSGROOT* pTempMsg = reinterpret_cast<MSGROOT*>(pMsg);
	ASSERT(g_pServerMsgParser[pTempMsg->Category] != NULL);
	
//KES
	if( pTempMsg->Category <= 0 || pTempMsg->Category >= MP_MAX ||
		g_pServerMsgParser[pTempMsg->Category] == NULL )
	{
		//char buf[128];
		//sprintf( buf, "Category error : %d", pTempMsg->Category );
		//ASSERTMSG( 0, buf );
		return;
	}
	try
	{
		g_pServerMsgParser[pTempMsg->Category](dwConnectionIndex, pMsg, dwLength);//kiv
	}
	catch(...)
	{
		char szTxt[256];
		sprintf(szTxt, "A  ReceivedMsgFromUser, %u, %u, %u, %u", dwConnectionIndex, pTempMsg->Category, pTempMsg->Protocol, dwLength);
	}
	//g_pServerMsgParser[pTempMsg->Category](dwConnectionIndex, pMsg, dwLength);//kiv
}

void  _stdcall  ReceivedMsgFromUser(DWORD dwConnectionIndex,char* pMsg,DWORD dwLength)
{
	MSGROOT* pTempMsg = reinterpret_cast<MSGROOT*>(pMsg);

	pTempMsg->Category = pTempMsg->Category ^ PacketEnDesStr[dwLength % (PacketEnDesStr.length() - 1)];
	pTempMsg->Protocol = pTempMsg->Protocol ^ PacketEnDesStr[dwLength % (PacketEnDesStr.length() - 1)];

	USERINFO* pInfo = g_pUserTable->FindUser(dwConnectionIndex);

	if( pInfo )
	if( g_pServerSystem->IsEnableCrypt() )
	{
//---KES CheckSum 071003
		if( pInfo->cbCheckSum == 0 )
		{
			pInfo->cbCheckSum = pTempMsg->CheckSum;
		}
		else if( pInfo->cbCheckSum != pTempMsg->CheckSum )
		{
			OnDisconnectUser( dwConnectionIndex );
			DisconnectUser( dwConnectionIndex );
			return;
		}

		++pInfo->cbCheckSum;
//----------------------

		int headerSize = sizeof( MSGROOT );

		if( !pInfo->crypto.Decrypt( ( char * )pTempMsg +  headerSize, dwLength - headerSize ) )
		{
			ASSERTMSG(0,"Decrypt Error");
			OnDisconnectUser( dwConnectionIndex );
			DisconnectUser( dwConnectionIndex );
			return;
		}
		char aaa = pInfo->crypto.GetDeCRCConvertChar();
		if( pTempMsg->Code !=  aaa )
		{
			ASSERTMSG(0,"Decrypt CRC Error");
			OnDisconnectUser( dwConnectionIndex );
			DisconnectUser( dwConnectionIndex );
			return;
		}
		TRAFFIC->AddUserPacket( pInfo->dwUserID, pTempMsg->Category );
	}

	ASSERT(g_pUserMsgParser[pTempMsg->Category] != NULL);
	ASSERT(pTempMsg->Category < MP_MAX);
	ASSERT(pTempMsg->Category != 0);
	if( g_pUserMsgParser[pTempMsg->Category] == NULL ||
		pTempMsg->Category >= MP_MAX ||
		pTempMsg->Category == 0)
		return;
	FIREWALLMGR->CheckIpAddr(dwConnectionIndex);
	g_pUserMsgParser[pTempMsg->Category](dwConnectionIndex, pMsg, dwLength);

	/*if( pInfo )
	{
		USERINFO* pcInfo = g_pUserTableForUserID->FindUser( pInfo->dwUserID );
		if( pcInfo )
		{
			if( pInfo->DistAuthKey != pcInfo->DistAuthKey ) // 
				return;
		}

#ifdef _CRYPTCHECK_

		int headerSize = sizeof( MSGROOT );

		if( !pInfo->crypto.Decrypt( ( char * )pTempMsg +  headerSize, dwLength - headerSize ) )
		{
			ASSERTMSG(0,"Decrypt Error");
			OnDisconnectUser( dwConnectionIndex );
			DisconnectUser( dwConnectionIndex );
			return;
		}
		char aaa = pInfo->crypto.GetDeCRCConvertChar();
		if( pTempMsg->Code !=  aaa )
		{
			ASSERTMSG(0,"Decrypt CRC Error");
			OnDisconnectUser( dwConnectionIndex );
			DisconnectUser( dwConnectionIndex );
			return;
		}

#endif
		
		TRAFFIC->AddUserPacket( pInfo->dwUserID, pTempMsg->Category );
	    
	 }

	//ASSERT(g_pUserMsgParser[pTempMsg->Category] != NULL);
	//ASSERT(pTempMsg->Category < MP_MAX);
	//ASSERT(pTempMsg->Category != 0);
	if( pTempMsg->Category >= MP_MAX ||
		g_pUserMsgParser[pTempMsg->Category] == NULL ||
		pTempMsg->Category == 0)
	{
		return;
	}
	try
	{
		g_pUserMsgParser[pTempMsg->Category](dwConnectionIndex, pMsg, dwLength);
	}
	catch(...)
	{
		char szTxt[256];
		sprintf(szTxt, "A  ReceivedMsgFromUser, %u, %u, %u, %u", dwConnectionIndex, pTempMsg->Category, pTempMsg->Protocol, dwLength);
	}*/
}

void _stdcall GameProcess(DWORD dwEventIndex)
{
	g_pServerSystem->Process();
}

void  _stdcall fConnectionCheck(DWORD dwEventIndex)
{
	g_pServerSystem->ConnectionCheck();
}



// console interface function
void ButtonProc1()
{

	int servercount = g_pServerTable->GetDataNum();
	g_Console.LOG(4, "Connected Server Count : %d", servercount);

	char temp[128] = {0,};
	SERVERINFO* pInfo = NULL;
	g_pServerTable->SetPositionHead();
	while( pInfo = g_pServerTable->GetNextServer() )
	{
		switch( pInfo->wServerKind )
		{
		case DISTRIBUTE_SERVER:	sprintf( temp, "DistributeServer" ); break;
		case AGENT_SERVER:		sprintf( temp, "AgentServer" );	break;
		case MAP_SERVER:		sprintf( temp, "MapServer" );	break;		
		}
		g_Console.LOG(4, "%s : IP = %s, Port = %d", temp, pInfo->szIPForServer, pInfo->wPortForServer );
	}
}

void ButtonProc2()
{
	g_Console.LOG(4, "UserCount %d %d %d, AcceptUser %d %d [ USERIDX[%d, %d] UT[%d, %d] OBJECTIDX[%d, %d]]",
		g_pUserTableForUserID->GetUserCount(),
		g_pUserTable->GetUserCount(),
		g_pUserTableForObjectID->GetUserCount(),
		g_dwAcceptUser, g_dwAcceptUser2, 
		g_pUserTableForUserID->GetAddCount(), 
		g_pUserTableForUserID->GetRemoveCount(), 
		g_pUserTable->GetAddCount(), 
		g_pUserTable->GetRemoveCount(),
		g_pUserTableForObjectID->GetAddCount(), 
		g_pUserTableForObjectID->GetRemoveCount() );
}

void ButtonProc3()
{
/*
#ifdef _HK_LOCAL_
	if( g_pServerSystem )
	{
		g_Console.LOG(4, "Agent FPS : %d, Dealy: %d",
			g_pServerSystem->GetFPSTimeTest(), g_pServerSystem->GetDBProcessDelay() );
	}
#endif
*/
	g_Console.LOG(4, "INetwork GetConnectedUserNum() : %d", g_Network.GetINet()->GetConnectedUserNum());
}
char *strLower( char *str ) 
{
	char *temp;
	for ( temp = str; *temp; temp++ ) 
	{
		*temp = tolower( *temp );
	}
	return str;
}
#include "..\[CC]Header\Server_CMD.h"
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
	}

	MSG_CHAT_WORD msg;
	msg.Category = MP_CLIENT;
	msg.Protocol = MP_CLIENT_MSG_AGENT;
	msg.CyptKey = 777;
	sprintf(msg.Msg,"[AGENT CONSOLE]: %s",szCommand);

	USERINFO* pInfo = NULL;
	g_pUserTable->SetPositionUserHead();
	while( pInfo = (USERINFO *)g_pUserTable->GetUserData() )
	{

		g_Network.Send2User( pInfo->dwConnectionIndex, (char*)&msg,sizeof(MSG_CHAT_WORD));
	}*/
	//g_Network.Broadcast2AgentServer((char*)&msg,sizeof(MSG_CHAT_WORD));


	/*MSG_CHAT_WORD msg;
	msg.Category = MP_CLIENT;
	msg.Protocol = MP_CLIENT_MSG_AGENT;
	sprintf(msg.Msg,"[CONSOLE]: %s",szCommand);
	USERINFO* pInfo = NULL;
	g_pUserTable->SetPositionUserHead();
	while( pInfo = (USERINFO *)g_pUserTable->GetUserData() )
	{	
		MSG_CHAT_WORD msgTemp = msg;
		g_Network.Send2User( pInfo->dwConnectionIndex, (char*)&msgTemp,sizeof(msgTemp));
		
	}*/
	/*
	MSG_CHAT_WORD msg;
	msg.Category = MP_CLIENT;
	msg.Protocol = MP_CLIENT_MSG_AGENT;
	sprintf(msg.Msg,"[CONSOLE]: %s",szCommand);
	//g_Network.Broadcast2AgentServer((char*)&msg,sizeof(MSG_CHAT_WORD));

	USERINFO* pInfo = NULL;
	g_pUserTable->SetPositionUserHead();
	while( pInfo = (USERINFO *)g_pUserTable->GetUserData() )
	{

		g_Network.Send2User( pInfo->dwConnectionIndex, (char*)&msg,sizeof(MSG_CHAT_WORD));
	}*/
	/*strupr(szCommand);
	char Header[256] = {0,};	
	sscanf(szCommand,"%s",Header);
	START_STRING_COMPARE(Header)
		COMPARE("FINDUSER")
			DWORD UserID;
			sscanf(szCommand,"%s %d",Header,&UserID);
			USERINFO* pInfo = g_pUserTableForUserID->FindUser(UserID);
			if(pInfo == NULL)
				g_Console.Log(eLogDisplay,4,"No User (%d)",UserID);
			else
			{
				g_Console.Log(eLogDisplay,4,"UserID:%d  ConIdx:%d  DistAuthKey:%d  AgentAuthKey:%d",
					UserID,pInfo->dwConnectionIndex,pInfo->DistAuthKey,pInfo->dwUniqueConnectIdx);
			}
	END_COMPARE*/

}

// ¶Ä²©ÏµÍ³Á¢¼´½øÐÐ¿ª½± 2015-01-28

void GambleOnceExec()
{
   GAMBLEMGR->GambleOnceExec(); // ¶Ä²©ÏµÍ³Á¢¼´½øÐÐ¿ª½± 
}



void ButtonReloadRanking()
{
	GetHeroTopList(MAX_TOP_NUM);
	GetMunpTopList(MAX_TOP_NUM);
	//GetHeroTopFameList(MAX_TOP_NUM);
}
/*void ReloadSemuaBinMap()
{
	ReloadAllBin();
}*/
void ReloadNULL()
{
	//SQLHSTMT    hStmt;
	//SQLExecDirect(hStmt, (SQLCHAR*)"BACKUP DATABASE MHCMEMBER TO DISK = 'C:\AUTOBACKUP\MHCMEMBER2.bak'", 1024);


	//char txt[128];
	//sprintf(txt, "BACKUP DATABASE MHCMEMBER TO DISK = 'C:\\AUTOBACKUP\\MHCMEMBER2.bak'", g_nServerSetNum, g_pServerTable->GetSelfServer()->wServerNum);
	//g_DB.LoginQuery(eQueryType_FreeQuery, eAgentDown, 0, txt);
}