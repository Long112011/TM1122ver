



// ServerSystem.cpp: implementation of the CServerSystem class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include <string>          
#include "ServerSystem.h"
#include "Console.h"
#include "Network.h"
#include "DataBase.h"
#include "CommonNetworkMsgParser.h"
#include "DistributeNetworkMsgParser.h"
#include "DistributeDBMsgParser.h"

#include "Usertable.h"
#include "Servertable.h"
#include <MemoryPoolTempl.h>
#include "BootManager.h"
#include "UserManager.h"
#include "MHFile.h"
#include "MHTimeManager.h"
#include "..\[CC]ServerModule\MiniDumper.h"
#include "BuddyAuth.h"
#include "../[CC]JACK/JackJack.h"

extern DWORD g_dwMaxUser;
extern DWORD g_dwMaxClientUser;
extern int g_nServerSetNum;
BOOL CServerSystem::IsCanReg; 
void RegOpenOrClose();
void ReadServerConfig();
void GetServerInfo();
void DevTest();
HWND g_hWnd;

std::string PacketEnDesStr = NetServer;





void _stdcall ProcessDBMessage(DWORD dwEventIndex)
{
	g_DB.ProcessingDBMessage();
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

// taiyo 
// 
CServerSystem * g_pServerSystem = NULL;
CMemoryPoolTempl<USERINFO> g_UserInfoPool;

CServerSystem::CServerSystem()
{
	// servertable usertable 초기화 ---------------------
	g_UserInfoPool.Init(3000, 500,"USERINFO");
	g_pUserTable = new CUserTable;
	g_pUserTable->Init(3000/3);
	g_pServerTable = new CServerTable;
	g_pServerTable->Init(50);

	CoInitialize(NULL);

	m_bUserFireWall = FALSE;
	ZeroMemory( m_strFireWallIP, sizeof(m_strFireWallIP) );
	
	m_Nation = eNATION_KOREA;

}

CServerSystem::~CServerSystem()
{
	CoUninitialize();
}

//extern DWORD g_dwMaxUser;


void CServerSystem::Start(WORD ServerNum)
{	

//AO½A ·IAA
	SetNation();

#ifdef _HK_LOCAL_
	g_Encrypt0 = g_Encrypt1 = GetTickCount() % 100;	//랜덤하게..
#endif

	FILE* fpMC = fopen( "MaxUserCount.txt", "r" );
	if(fpMC)
	{

		fscanf( fpMC, "%d", &g_dwMaxUser );
		fclose( fpMC );
	}

	BOOTMNGR->AddSelfBootList(DISTRIBUTE_SERVER, ServerNum, g_pServerTable);
	gUserMGR.LoadDistributeDesc();	//이걸 읽기위해서 위엣줄이 필요한거라구.

//	m_IdxCreater = ICCreate();
//	ICInitialize(m_IdxCreater,5000);
	
	//////////////////////////////////////////////////////////////////////////
	// 콘솔 초기화
	// 버튼 설정.외부입력용 버튼이 28개 준비되어있다.한개도 사용하지 않을 경우 pCustomButton 필드를 NULL로 채운다.
	
	MENU_CUSTOM_INFO menu[] =
	{
	//	{ " ", ReloadNULL },
		{ "RegOpenOrClose", RegOpenOrClose },
		{ "ReadConfig", ReadServerConfig },
		{ "Button3", ButtonProc3 },
		{ "DevTest", DevTest }


		//{"ReloadTest",ReloadTest}
	};


	/*MENU_CUSTOM_INFO pMenu[3];
	strcpy(pMenu[0].szMenuName, "RegOpenOrClose");
	pMenu[0].cbMenuCommand = RegOpenOrClose;
	strcpy(pMenu[1].szMenuName, "ReadServerConfig");
	pMenu[1].cbMenuCommand = ReadServerConfig;
	strcpy(pMenu[2].szMenuName, "Button3");
	pMenu[2].cbMenuCommand = ButtonProc3;*/

	char windowTitleMsg[64];
	sprintf(windowTitleMsg, "Distribute Server_%d", ServerNum);

	if (!g_Console.Init(windowTitleMsg, /*3,*/ sizeof(menu) / sizeof(*menu), menu, OnCommand))
		MessageBox(NULL,"Fail initialize Console",0,0);

	

	//g_hWnd = GetActiveWindow();
	//SetWindowText(g_hWnd, windowTitleMsg);


//	DWORD    dwProcessID = NULL;
//	DWORD    dwTreadID = ::GetWindowThreadProcessId(g_hWnd, &dwProcessID );

//	HOOKPROC hProc;
//    hProc = CallWndProc;
//    hHook = ::SetWindowsHookEx(WH_CALLWNDPROC, hProc, (HINSTANCE)NULL, dwTreadID);


	//////////////////////////////////////////////////////////////////////////
	// 네트워크 설정

	/// 네트워크 파서 설정-------------------------------
	g_pServerMsgParser[0] = NULL;
	for(int i=1 ; i<MP_MAX ; ++i)
		g_pServerMsgParser[i] = ErrorMsgParser;
	for(int i=1 ; i<MP_MAX ; ++i)
		g_pUserMsgParser[i] = ErrorMsgParser;
	g_pServerMsgParser[MP_SERVER] = MP_DISTRIBUTESERVERMsgParser;
	g_pServerMsgParser[MP_POWERUP] = MP_POWERUPMsgParser;
	g_pServerMsgParser[MP_USERCONN] = MP_USERCONNMsgParser;
	g_pUserMsgParser[MP_USERCONN] = MP_USERCONNMsgParser;
	g_pUserMsgParser[MP_AUTOPATCH] = MP_DISTRIBUTEAUTOPATCHMsgParser;
	
	g_pServerMsgParser[MP_MORNITORMAPSERVER] = MP_MonitorMsgParser;



	// ServerList 읽어들임 ------------------------

	//g_pListCollect = new CListCollect;
	//g_pListCollect->LoadServerInfo();
	//g_pListCollect->LoadDistributeNum();

	CUSTOM_EVENT ev[3];
	ev[0].dwPeriodicTime	= 500;
	ev[0].pEventFunc		= GameProcess;
//	ev[1].dwPeriodicTime	= 100;
	ev[1].dwPeriodicTime	= 0;
	ev[1].pEventFunc		= ProcessDBMessage;
	ev[2].dwPeriodicTime	= 1000 * 30;
	ev[2].pEventFunc		= fConnectionCheck;

		
	DESC_NETWORK desc;
	desc.OnAcceptServer = OnAcceptServer;
	desc.OnDisconnectServer = OnDisconnectServer;
	desc.OnAcceptUser = OnAcceptUser;
	desc.OnDisconnectUser = OnDisconnectUser;
	desc.OnRecvFromServerTCP = ReceivedMsgFromServer;
	desc.OnRecvFromUserTCP = ReceivedMsgFromUser;

#ifdef _DBTHREAD
	desc.dwCustomDefineEventNum = 1;
#else
	desc.dwCustomDefineEventNum = 3;
#endif

	desc.pEvent = ev;

	desc.dwMainMsgQueMaxBufferSize = 20480000;	//
	desc.dwMaxServerNum = 100;
	desc.dwMaxUserNum = 3000;
	desc.dwServerBufferSizePerConnection = 512000;
	desc.dwServerMaxTransferSize = 65535;
	desc.dwUserBufferSizePerConnection = 128000;
	desc.dwUserMaxTransferSize = 65535;
	desc.dwConnectNumAtSameTime = 3000;
	desc.dwFlag = 0;//NETDDSC_DEBUG_LOG;

	g_Network.Init(&desc);
	//g_Network.Start();
	
	// Booting
//	BOOTMNGR->AddSelfBootList(DISTRIBUTE_SERVER, ServerNum, g_pServerTable);//위로 옮겨놓구--;
	if(!BOOTMNGR->StartServer(&g_Network, g_pServerTable->GetSelfServer()))
	{
		MessageBox(NULL,"Server Initialize Failed (StartServer)",NULL,NULL);
		return;
	}
	if(!BOOTMNGR->ConnectToMS(&g_Network, g_pServerTable))
	{
		SERVERINFO info;
		info.wServerKind = MONITOR_SERVER;
		OnConnectServerFail(&info);		
	}

	//////////////////////////////////////////////////////////////////////////
	// DB 설정
	DWORD maxthread = 2;
	DWORD maxqueryinsametime = 1024;
	FILE* fp = fopen("DistributeDBInfo.txt","r");
	if(fp)
	{
		fscanf(fp,"%d %d",&maxthread,&maxqueryinsametime);
		fclose(fp);
	}
	if(g_DB.Init(maxthread,maxqueryinsametime,TRUE) == FALSE)
		MessageBox(NULL,"DataBase Initializing Failed",0,0);

	m_DBThreadTerminate = FALSE;
#ifdef _DBTHREAD
	CreateThread(0,0,ProcessDBMessageThread,0,0,&m_DBThreadID);
#endif
	
	
	
	LoginCheckInit();
#ifdef _HK_LOCAL_
	LoadSAT();
	LoadAC();
#endif
//	LoadFireWallIP();

	// Dump
#ifdef _JAPAN_LOCAL_
	MiniDumper md(MiniDumper::DUMP_LEVEL_0);
#elif defined _CHINA_LOCAL_
	MiniDumper md(MiniDumper::DUMP_LEVEL_0);
#elif defined _HK_LOCAL_
	MiniDumper md(MiniDumper::DUMP_LEVEL_0);
#elif defined _TL_LOCAL_
	MiniDumper md(MiniDumper::DUMP_LEVEL_0);
#else
	MiniDumper md(MiniDumper::DUMP_LEVEL_2);
#endif

	

	g_Console.LOG(4, "-----------   DISTRIBUTE SERVER START  -----------------");

	
	
	g_bReady = TRUE;

//	if(FALSE==JACKMGR->CheckSumInit(g_hWnd))
//		g_bReady = false;
//	else
//		g_bReady = true;

	ReadServerConfig();

	/*if(!CServerSystem::IsCanReg)
	{
		g_Console.LOC(0x0000ff,4,"REGISTRATION DISABLED");
	}
	else if(CServerSystem::IsCanReg)
	{
		g_Console.LOC(0xFFFF66,4,"REGISTRATION ENABLED");
	}
	char MaxUserLogin[16];
	sprintf(MaxUserLogin, "MaxUserLogin(global) : %d", g_dwMaxUser);
	g_Console.LOC(0xFFFF66, 4, MaxUserLogin);
	sprintf(MaxUserLogin, "MaxClientLogin(PC) : %d", g_dwMaxClientUser);
	g_Console.LOC(0xFFFF66, 4, MaxUserLogin);*/

	g_Console.WaitMessage();

	
}

#ifdef _HK_LOCAL_
void CServerSystem::LoadSAT()
{
	CMHFile file;
	file.Init("Resource/SAT.bin","rb");
	if(file.IsInited() == FALSE)
	{
		return;
	}

	file.GetDword();
	for(int n=0;n<30;++n)
	{
		file.GetWord();
		m_aniTimeMale[n] = file.GetDword();
		file.GetDword();
	}

	file.Release();
}

void CServerSystem::LoadAC()
{
	CMHFile file;
	file.Init("Resource/AbilityCalcInfo.bin","rb");
	if(file.IsInited() == FALSE)
	{
		return;
	}

	for(int n=0;n<30;++n)
	{
		file.GetDword();
		file.GetDword();
		file.GetFloat();
		file.GetFloat();
		file.GetDword();
		m_ac[n] = file.GetDword();
		file.GetDword();
		file.GetDword();
		file.GetDword();
		file.GetDword();
		file.GetFloat();
		file.GetFloat();
		file.GetDword();
		file.GetDword();
	}

	file.Release();
}
#endif

void CServerSystem::LoadFireWallIP()
{
	CMHFile file;
	if( !file.Init( "firewallip.txt", "rt", MHFILE_FLAG_DONOTDISPLAY_NOTFOUNDERROR ) )
		return;

	while(1)
	{
		if( file.IsEOF() )
			break;

		if( file.GetInt() == g_nServerSetNum )
		{
			SafeStrCpy( m_strFireWallIP, file.GetString(), 16 );
			if( *m_strFireWallIP != 0 )
				m_bUserFireWall = TRUE;

			break;
		}

	}

	file.Release();	
}

void CServerSystem::ConnectionCheck()
{	// YH 현재 30초마다 한번씩 들어옴
	DWORD _60sec = 60*1000;
	USERINFO* pInfo;
	DWORD elapsedtime;

	if(g_bReady == FALSE)
		return;








	static DWORD nLastEncryptCheck = gCurTime;
	if( gCurTime - nLastEncryptCheck >= 1000*60*60 )
	{
		//g_Console.LOC(0x0000ff,4,"EncryptCheck");
		nLastEncryptCheck = gCurTime;
	}
	BOOL bCheckTime=JACKMGR->GetTrialWeek();
	if(!bCheckTime)
	{
		//SendMessage(g_hWnd, WM_CLOSE, 0,0);
		//Start(FALSE);
		//g_Console.LOG(4, "loop!");
		exit(0);
		End();
		//return;
	}
	
#ifdef _HK_LOCAL_
	static DWORD nLastEncryptCheck = gCurTime;
	if( gCurTime - nLastEncryptCheck >= 1000*60*60 )	//1시간
	{
		g_Encrypt0 = g_Encrypt1;
		g_Encrypt1 = ( g_Encrypt1 + 1 ) % 100;
//		g_Console.LOG( 4, "Encrypt Changed : 0:%d, 1:%d", g_Encrypt0, g_Encrypt1 );
		nLastEncryptCheck = gCurTime;
	}
#endif

	cPtrList removelist;

	g_pUserTable->SetPositionHead();
	while(pInfo = g_pUserTable->GetData())
	{
		if(pInfo->dwConnectionIndex != 0)
		{
			// 아직 접속이 제대로 이뤄지지 않은 경우
			elapsedtime = gCurTime - pInfo->dwLastConnectionCheckTime;
			if( elapsedtime > _60sec * 2 )
			{
				if(pInfo->bConnectionCheckFailed)
				{
					//!!테스트 후 30초 마다 들어오게 바꾸고 지울것~!
					//pInfo->dwLastConnectionCheckTime += 1000 * 10;	//OnDisconnectUser( 리턴 받기 전에 반복 방지.
					removelist.AddTail(pInfo);
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
		ASSERT(p->dwConnectionIndex);
//		LoginCheckDelete(p->dwUserID);		// 로그인체크 테이블에서 삭제
//		g_pServerSystem->ReleaseAuthKey(p->dwUniqueConnectIdx);
//		g_pUserTable->RemoveUser(p->dwConnectionIndex);
//		memset(p,0,sizeof(USERINFO));
//		g_UserInfoPool.Free(p);
		DisconnectUser( p->dwConnectionIndex );
	}
	removelist.RemoveAll();
}

void CServerSystem::SendConnectionCheckMsg(USERINFO* pInfo)
{
	MSGBASE msg;
	msg.Category = MP_USERCONN;
	msg.Protocol = MP_USERCONN_CONNECTION_CHECK;
	g_Network.Send2User(pInfo->dwConnectionIndex,(char*)&msg,sizeof(msg));
}

void CServerSystem::Process()
{
	JACKMGR->Process();

	MHTIMEMGR_OBJ->Process();

	g_DB.ProcessingQueue();
//	g_DB.ProcessingDBMessage();

	g_BuddyAuth.RevMsgProcess();

	//g_Console.LOG(4, "loop!");

	
	//JACKMGR->Process();

	

}


void CServerSystem::End()
{
//	UnhookWindowsHookEx(hHook);
	m_DBThreadTerminate = TRUE;
//	ICRelease(m_IdxCreater);
	
	g_DB.Release();
	
	g_Network.Release();
	g_Console.Release();

//	if(g_pListCollect)
//	{
//		delete g_pListCollect;
//		g_pListCollect = NULL;
//	}
	if(g_pUserTable)
	{
		delete g_pUserTable;
		g_pUserTable = NULL;
	}
	if(g_pServerTable)
	{

		delete g_pServerTable;
		g_pServerTable = NULL;
	}
	CoFreeUnusedLibraries();
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


// global function

void _stdcall OnConnectServerSuccess(DWORD dwConnectionIndex, void* pVoid)
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

	//kiv
	if(!dwConnectionIndex) return;
	info->dwConnectionIndex = dwConnectionIndex;
	MSG_WORD msg;
	msg.Category = MP_SERVER;
	msg.Protocol = MP_SERVER_PWRUP;
	msg.wData = g_pServerTable->GetSelfServer()->wPortForServer;
	g_Network.Send2Server(dwConnectionIndex, (char*)&msg, sizeof(msg));
}

void _stdcall  OnDisconnectServer(DWORD dwConnectionIndex)
{
	g_Console.LOG(4, "Server Disconnected : ConnectionIndex %d", dwConnectionIndex);//pjslocal

	// 서버정보 지움
	SERVERINFO * delInfo = g_pServerTable->RemoveServer(dwConnectionIndex);
	if(!delInfo)
	{
		//BootManager작동이 완료하지 않았을 때 서버가 꺼질 경우 발생,(올바른 에러)
		ASSERT(0);
		return;
	}
	delete delInfo;
}

void _stdcall OnConnectServerFail(void* pVoid)
{	
	SERVERINFO* info = (SERVERINFO*)pVoid;
	if(info->wServerKind == MONITOR_SERVER)
	{
		BOOTMNGR->AddBootListINI(DISTRIBUTE_SERVER, 0, g_pServerTable);
		BOOTMNGR->BactchConnectToMap(&g_Network, g_pServerTable);
	//	g_Console.LOG(4, "Failed to Connect to the MS : %s, %d", info->szIPForServer, info->wPortForServer);
	}

	else
	{
		//not process
		g_Console.LOG(4, "Failed to Connect to the Server : %s, %d", info->szIPForServer, info->wPortForServer);
		BOOTMNGR->RemoveBootList(g_pServerTable, info->wPortForServer);
	}	
}
void _stdcall OnAcceptServer(DWORD dwConnectionIndex)
{// not used
	char strr[255];
	wsprintf(strr, "%d : connectionIndex Server connected", dwConnectionIndex);//pjslocal

	g_Console.LOG(4, strr);
}

void _stdcall  OnAcceptUser(DWORD dwConnectionIndex)
{	
	if(g_bReady == FALSE)
	{
		// 초기화가 완전히 안됐는데 들어온경우.
		MSGBASE send;
		send.Category = MP_USERCONN;
		send.Protocol = MP_USERCONN_SERVER_NOTREADY;

		send.dwObjectID = 0;
		g_Network.Send2User(dwConnectionIndex, (char *)&send, sizeof(send));

		DisconnectUser(dwConnectionIndex);
		return;
	}

	USERINFO* pPreInfo = g_pUserTable->FindUser( dwConnectionIndex );
	if( pPreInfo )
	{
		g_pUserTable->RemoveUser( dwConnectionIndex );
		memset(pPreInfo, 0, sizeof(USERINFO));
		g_UserInfoPool.Free( pPreInfo );
	}

	DWORD authkey = g_pServerSystem->MakeAuthKey();
	USERINFO * info = g_UserInfoPool.Alloc();
	memset(info, 0, sizeof(USERINFO));
	info->dwConnectionIndex = dwConnectionIndex;
	info->dwUniqueConnectIdx = authkey;
	info->dwLastConnectionCheckTime = gCurTime;	//SW051107 추가
	g_pUserTable->AddUser(info,dwConnectionIndex);

#ifndef _HK_LOCAL_
	MSGBASE send;
	send.Category = MP_USERCONN;
	send.Protocol = MP_USERCONN_DIST_CONNECTSUCCESS;
	send.dwObjectID = authkey;
	g_Network.Send2User(dwConnectionIndex, (char *)&send, sizeof(send));
#else
	MSG_DWORD2 send;
	send.Category = MP_USERCONN;
	send.Protocol = MP_USERCONN_DIST_CONNECTSUCCESS;

	send.dwObjectID = authkey+11;
	send.dwData1 = g_Encrypt1*7 + authkey + 23;
	send.dwData2 = authkey*3 + 47;

	g_Network.Send2User(dwConnectionIndex, (char *)&send, sizeof(send));
#endif

//	g_Console.Log(eLogDisplay,4, "OnAcceptUser : Client Connected - Idx:%d, AuthKey:%d, Total(%d)",dwConnectionIndex,authkey, g_pUserTable->GetUserCount());
//	g_Console.Log(eLogFile,4, "OnAcceptUser : Client Connected - Idx:%d, AuthKey:%d, Total(%d)",dwConnectionIndex,authkey, g_pUserTable->GetUserCount());
}

void  _stdcall  OnDisconnectUser(DWORD dwConnectionIndex)
{
	g_pUserTable->OnDisconnectUser(dwConnectionIndex);
//	g_Console.Log(eLogDisplay,4, "OnAcceptUser : Client Disconnected - Idx:%d, Total(%d)",dwConnectionIndex, g_pUserTable->GetUserCount());
//	g_Console.Log(eLogFile,4, "OnAcceptUser : Client Disconnected - Idx:%d, Total(%d)",dwConnectionIndex, g_pUserTable->GetUserCount());
}

void _stdcall ReceivedMsgFromServer(DWORD dwConnectionIndex,char* pMsg,DWORD dwLength)
{
	MSGROOT* pTempMsg = reinterpret_cast<MSGROOT*>(pMsg);
	
	if( g_pServerMsgParser[pTempMsg->Category] == NULL ||
		pTempMsg->Category >= MP_MAX ||
		pTempMsg->Category == 0)
		return;

	g_pServerMsgParser[pTempMsg->Category](dwConnectionIndex, pMsg, dwLength);
}

void _stdcall ReceivedMsgFromUser(DWORD dwConnectionIndex,char* pMsg,DWORD dwLength)
{

	MSGROOT* pTempMsg = reinterpret_cast<MSGROOT*>(pMsg);
  
	pTempMsg->Category = pTempMsg->Category ^ PacketEnDesStr[dwLength % (PacketEnDesStr.length() - 1)];
	pTempMsg->Protocol = pTempMsg->Protocol ^ PacketEnDesStr[dwLength % (PacketEnDesStr.length() - 1)];

	ASSERT(g_pUserMsgParser[pTempMsg->Category]);

//---KES Distribute Encrypt 071003
	USERINFO* pInfo = g_pUserTable->FindUser(dwConnectionIndex);
	if( pInfo )
	{
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
	}
//--------------------------------
	if( g_pUserMsgParser[pTempMsg->Category] == NULL ||
		pTempMsg->Category >= MP_MAX ||
		pTempMsg->Category == 0)
		return;

	g_pUserMsgParser[pTempMsg->Category](dwConnectionIndex, pMsg, dwLength);
}

//SW051107 추가
void  _stdcall  fConnectionCheck(DWORD dwEventIndex)
{
	g_pServerSystem->ConnectionCheck();
}

void  _stdcall  GameProcess(DWORD dwEventIndex)
{
	g_pServerSystem->Process();
}

// console interface function
void ButtonProc1()
{
	//SendMessage(g_hWnd, WM_CLOSE, 0, 0);
}

void ButtonProc2()
{
	ReadServerConfig();
}

void ButtonProc3()
{
	//GetServerInfo();
	g_Console.LOG(eLogDisplay, "UserCount : %d", g_pUserTable->GetUserCount());
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
	msg.CyptKey = 555;
	sprintf(msg.Msg,"[TO:ALL MAP]: %s",szCommand);
	g_Network.Broadcast2AgentServer((char*)&msg,sizeof(MSG_CHAT_WORD));*/


	
}

BOOL CServerSystem::IsInvalidCharInclude( char* pStr )
{
	while( *pStr )
	{
		BOOL bOk = FALSE;

		if( IsDBCSLeadByte( *pStr ) )
		{
			++pStr;
		}
		else
		{
			//영문
			if( ( *pStr >= 'A' && *pStr <= 'Z' ) || ( *pStr >= 'a' && *pStr <= 'z' ) )
				bOk = TRUE;
			//숫자
			else if( *pStr >= '0' && *pStr <= '9' )
				bOk = TRUE;
		}

		++pStr;

		if( bOk == FALSE )
		{
			return TRUE;
		}
	}

	return FALSE;
}



void RegOpenOrClose()
{
	if(CServerSystem::IsCanReg)
	{
		CServerSystem::IsCanReg = FALSE;
		g_Console.LOC(0x0000ff,4,"REGISTRATION DISABLED");
	}
	else if(!CServerSystem::IsCanReg)
	{
		CServerSystem::IsCanReg = TRUE;
		g_Console.LOC(0xFFFF66,4, "REGISTRATION ENABLED");
		//g_Console.LOG(4,"REGISTRATION ENABLED");
	}
}

void ReadServerConfig()
{
	CServerSystem::IsCanReg	=::GetPrivateProfileInt("Registration","Enable",0,".\\GameConfig.ini");

	g_dwMaxUser = ::GetPrivateProfileInt("MaxUserLogin", "value", 0, ".\\GameConfig.ini");

	g_dwMaxClientUser = ::GetPrivateProfileInt("MaxUserLogin", "client", 0, ".\\GameConfig.ini");

	GetServerInfo();
}
void GetServerInfo()
{
	if(!CServerSystem::IsCanReg)
	{
		g_Console.LOC(0x0000ff,4,"REGISTRATION DISABLED");
	}
	else if(CServerSystem::IsCanReg)
	{
		g_Console.LOC(0xFFFF66,4,"REGISTRATION ENABLED");
	}
	char MaxUserLogin[64];
	sprintf(MaxUserLogin, "MaxUserLogin(global) : %d", g_dwMaxUser);
	g_Console.LOC(0xFFFF66, 4, MaxUserLogin);
	sprintf(MaxUserLogin, "MaxClientLogin(PC)	: %d", g_dwMaxClientUser);
	g_Console.LOC(0xFFFF66, 4, MaxUserLogin);
}