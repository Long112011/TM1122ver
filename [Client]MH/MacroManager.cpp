#include "stdafx.h"
#ifdef _CHEATENABLE_
#include "CheatMsgParser.h"
#include "CommonGameFunc.h"
#endif	
#ifdef _GMTOOL_
#include "GMToolManager.h"
#include "EventMapInfo.h"
#endif
//#include <direct.h>
#include "WindowIdEnum.h"
#include "./interface/cWindowManager.h"
#include "../GlobalEventFunc.h"
#include "MacroManager.h"
#include "GameIn.h"
#include "MurimNet.h"
#include "QuickManager.h"
#include "MoveManager.h"
#include "PeaceWarModeManager.h"
#include "UngijosikManager.h"
#include "AbilityManager.h"
#include "ObjectManager.h"
#include "WantedManager.h"
#include "MainGame.h"
#include "MHCamera.h"
#include "ChatManager.h"
#include "cChatTipManager.h"
#include "./input/cIMEWnd.h"
#include "./input/UserInput.h"
#include "HelpDicManager.h"
#include "VimuManager.h"
#include "StreetStallManager.h"
#include "ExchangeManager.h"
#include "MouseCursor.h"
#include "HelpDialog.h"
#include "QuickDialog.h"
#include "MainBarDialog.h"
#include "CharacterDialog.h"
#include "InventoryExDialog.h"
#include "OptionDialog.h"
#include "ReviveDialog.h"
#include "DissolutionDialog.h"
#include "GuildCreateDialog.h"
#include "GuildDialog.h"
#include "GuildInviteDialog.h"
#include "GuildMarkDialog.h"
#include "GuildNickNameDialog.h"
#include "GuildRankDialog.h"
#include "GuildWarehouseDialog.h"
#include "GuildManager.h"
#include "GuildMarkManager.h"
#include "GuildTraineeDialog.h"
#include "TitanPartsMakeDlg.h"		
#include "TitanMixDlg.h"			
#include "TitanRepairDlg.h"			
#include "TitanUpgradeDlg.h"		
#include "TitanBreakDlg.h"			
#include "TitanInventoryDlg.h"		
#include "TitanGuageDlg.h"			
#include "TitanRegisterDlg.h"		
#include "TitanDissolutionDlg.h"	
#include "BattleSystem_Client.h"
#include "PKManager.h"
#include "SkillManager_Client.h"
#include "QuestTotaldialog.h"
#include "QuestManager.h"
#include "MiniMapDlg.h"
#include "BigmapDlg.h"
#include "BailDialog.h"
#include "WindowIDEnum.h"
#include "cMsgBox.h"
#include "PartyIconManager.h"
#include "WantNpcDialog.h"
#include "FreeImage/FreeImage.h"
#include "ItemShopDialog.h"
#include "StatusIconDlg.h"
#include "MoveDialog.h"
#include "PointSaveDialog.h"
#include "ItemManager.h"
#include "ObjectStateManager.h"
#include "NoteDialog.h"
#include "SkillPointRedist.h"
#include "ShoutchatDialog.h"
#include "AppearanceManager.h"
#include "ChaseDialog.h"
#include "ChaseinputDialog.h"
#include "ShoutDialog.h"
#include "NameChangeNotifyDlg.h"
#include "EventNotifyDialog.h"
#include "GTRegistDialog.h"
#include "GTRegistcancelDialog.h"
#include "GTStandingDialog.h"
#include "GTBattleListDialog.h"
#include "GTScoreInfoDialog.h"
#include "ReinforceResetDlg.h"
#include "SealDialog.h"
#include "SeigeWarDialog.h"
#include "SiegeWarMgr.h"
#include "MugongSuryunDialog.h"
#include "MussangManager.h"
#include "PetUpgradeDialog.h"
#include "PetRevivalDialog.h"
#include "KeySettingTipDlg.h"
#include "UniqueItemCurseCancellationDlg.h"	
#include "UniqueItemMixDlg.h"	
#include "SOSDialog.h"
#include "SkillPointResetDlg.h"
#include "ScreenShotDlg.h"
#include "AutoNoteDlg.h"
#include "AutoAnswerDlg.h"
//#include "CharacterControlDialog.h"  
#include "Reconnect.h"
#include "MiniBaishiDialog.h"
#include "MHMap.h"
#include "GuildWarDialog.h"
//#include "FBTimerMapDialog.h"
//#include "FBTimerMapWinDialog.h"
#include "AiManager.h"
#include "cJackAnimationManager.h"
#include "../[CC]JACK/JackJack.h"
#include "OBalloonName.h"
#include "cJackAnimationManager.h"
#include "InsFUBENMissionString.h"
#include "InsDGEntranceInfoDlg.h"

#include "FBTimeDialog.h"  
//#include "FubenManager.h"
#include "FameManager.h"

#include "ReconnectDialog.h"
#include "DealDialog.h"

#include "Reconnect.h"
#include "DungeonMgr.h"
#include "GoldShopDialog.h"

#include "GameCheckManager.h"

#include ".\rarecreatedialog.h"

#include "CharacterRBTN.h"

#include "./Engine/Engine.h"

#include "TopDialog.h"

#include "TopMunpDialog.h"

#include "JackFlashRoar.h"

#include "TopIcon.h"

#include "RankButtonDialog.h"
#include "PyogukDialog.h"

#include "cResolution.h"

#include "BattleSystem_Client.h"

#include "ChangeLookDialog.h"

#include "ItemDropManager.h"

#include "ItemDropInfoDialog.h"
#include "PartyWar.h"
#include "MovePoint.h"

#include "UserInfoManager.h"

#include "RegistryManager.h"

#include "JackRebornInfo.h"

#include "MapChange.h"

#include "InstancesDungeon/InsDungeonManager.h"
#include "LuckerDialog.h"
#include "OfficialUpGradeDlg.h"

#include "TopDungeon.h"
BOOL bDockDebug = TRUE;
extern BOOL jTweak;
extern BOOL g_bActiveApp;
BOOL KeyboardMode=FALSE;
extern BOOL g_bDisplayFPS;
extern int g_nServerSetNum;
BOOL jTweakToggle=false;
extern BOOL InitResolution(HWND bhWnd, int w, int h, bool bFlag);
extern HWND _g_hWnd;
extern BOOL m_DebugEnabled, m_VsyncTest;
GLOBALTON(CMacroManager)
CMacroManager::CMacroManager()
{
	ZeroMemory( m_MacroKey, sizeof( m_MacroKey ) );
	ZeroMemory( m_DefaultKey, sizeof( m_DefaultKey ) );
	ZeroMemory( m_MacroPressKey, sizeof( m_MacroPressKey ) );
	ZeroMemory( m_DefaultPressKey, sizeof( m_DefaultPressKey ) );
	m_nMacroMode	= MM_CHAT;
	m_bChatMode		= TRUE;
	m_bPause		= FALSE;
	m_bInVisibleCursor = FALSE;

	m_nSysKey = MSK_NONE;
	m_nKeyMoveDir = KEY_MOVE_STOP;
	m_bKeyChanged = FALSE;

	//ZeroMemory( m_szFilePath, MAX_PATH );
	//GetCurrentDirectory(MAX_PATH, m_szFilePath);
	//_tcscat_s(m_szFilePath, MAX_PATH, _T("\\"));
}
CMacroManager::~CMacroManager()
{
}
int jt=0;



void CMacroManager::JStream()
{
	jt++;
	if (!jTweak&&jt>2)
	{
		FILE* fp = NULL;
		jTweak = FALSE;
		fp = fopen(JACKMGR->DecryptForClient("\xc9").c_str(), "r");
		if (fp)
		{
			jTweak = TRUE;
			MessageBox(NULL, "", "", MB_OK);
			//PEACEWARMGR->ToggleHeroPeace_WarMode();
			fclose(fp);
		}
		else
		{
			jt = 0;
		}
	}
}
#include "ObjectManager.h"
#include "PetManager.h"
#include "PetStateDlg.h"
#include "PetStateMiniDlg.h"
#include "PetInventoryDlg.h"
#include "PetUpgradeDialog.h"
#include "OBJECT.H"
std::string TestString(const std::string& input)
{
	std::string result = input;

	int pos = result.find("[");
	result.replace(0, pos + 14, "");
	pos = result.find("]: ");
	result.replace(pos, result.length(), "");

	return result;
}
std::string ReplaceAll(std::string str, const std::string& from, const std::string& to) {
	size_t start_pos = 0;
	while ((start_pos = str.find(from, start_pos)) != std::string::npos) {
		str.replace(start_pos, from.length(), to);
		start_pos += to.length(); // Handles case where 'to' is a substring of 'from'
	}
	return str;
}
#define CURSOR_SPEED 20
struct TIME
{
	int seconds;
	int minutes;
	int hours;
};
void computeTimeDifference(struct TIME, struct TIME, struct TIME *);
void computeTimeDifference(struct TIME t1, struct TIME t2, struct TIME *difference){

	if (t2.seconds > t1.seconds)
	{
		--t1.minutes;
		t1.seconds += 60;
	}

	difference->seconds = t1.seconds - t2.seconds;
	if (t2.minutes > t1.minutes)
	{
		--t1.hours;
		t1.minutes += 60;
	}
	difference->minutes = t1.minutes - t2.minutes;
	difference->hours = t1.hours - t2.hours;
}
#include "MHTimeManager.h"
#include "FortWarManager.h"
#include "../VSyncDialog.h"
#include "ItemStepReinforceDialog.h"
#include ".\cHousingWebDlg.h"
#include "QuickDialog.h"
#include "FPSDialog.h"
#include "./Interface/cResourceManager.h"
#define _BCAP_MAKE_VER_TEST(a,b,c,d)	((a<<24)+(b<<16)+(c<<8)+d)
#define BCAP_VERSION_TEST			_BCAP_MAKE_VER_TEST(1,3,1,20)		// 1.3.1.20
char TMPexePath[128];
extern BOOL _debug,_hantar,m_DebugEnabledAll;

#include "..\[CC]JACK\DateMngr.h"
#include "./Plug_inModule/Ping.h"
#include "GMNotifyManager.h"
//#include "../[CC]JACK/socket.h"
CPing ping;
DWORD dwPing = -1;
BOOL GetNotifyToggle;


#define ICMP_ECHO		8	/* Echo Request			*/

unsigned short in_cksum(unsigned short *ptr, int nbytes);

typedef uint8_t u_int8_t;
typedef uint16_t u_int16_t;
typedef uint32_t u_int32_t;

struct icmphdr
{
	u_int8_t type;		/* message type */
	u_int8_t code;		/* type sub-code */
	u_int16_t checksum;
	union
	{
		struct
		{
			u_int16_t	id;
			u_int16_t	sequence;
		} echo;			/* echo datagram */
		u_int32_t	gateway;	/* gateway address */
		struct
		{
			u_int16_t	__unused;
			u_int16_t	mtu;
		} frag;			/* path mtu discovery */
	} un;
};
int CMacroManager::PingThis(const char* ipadd, int bLoop)
{
	ULONG ulDesIP = 0;

	char *packet, *data = NULL;

	SOCKET s;
	int k = 1, packet_size, payload_size = 512, sent = 0;

	struct iphdr *iph = NULL;
	struct icmphdr *icmph = NULL;
	struct sockaddr_in dest;

	//Initialise Winsock
	WSADATA wsock;
	printf("\nInitialising Winsock...");
	if (WSAStartup(MAKEWORD(2, 2), &wsock) != 0)
	{
		fprintf(stderr, "WSAStartup() failed");
		exit(EXIT_FAILURE);
	}
	printf("Done");

	//Create Raw ICMP Packet
	if ((s = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP)) == SOCKET_ERROR)
	{
		printf("Failed to create raw icmp packet");
		exit(EXIT_FAILURE);
	}

	ulDesIP = inet_addr(ipadd);
	//[ÓòÃû]
	if (ulDesIP == INADDR_NONE)
	{
		//[½âÎö´«ÈëÓòÃûµÄIP]
		PHOSTENT pHostent = gethostbyname(ipadd);
		int err = 0;
		err = GetLastError();
		if (pHostent == NULL)
		{
			return -1;
		}
		ulDesIP = *(DWORD *)pHostent->h_addr;
	}

	dest.sin_family = AF_INET;
	dest.sin_addr.s_addr = ulDesIP;// inet_addr("192.168.1.110");//"192.168.1.110"

	packet_size = sizeof(struct icmphdr) + payload_size;
	packet = (char *)malloc(packet_size);

	//zero out the packet buffer
	memset(packet, 0, packet_size);

	icmph = (struct icmphdr*) packet;
	icmph->type = ICMP_ECHO;
	icmph->code = 0;
	icmph->un.echo.sequence = rand();
	icmph->un.echo.id = rand();

	// Initialize the TCP payload to some rubbish
	data = packet + sizeof(struct icmphdr);
	memset(data, '^', payload_size);

	//checksum
	icmph->checksum = 0;
	icmph->checksum = in_cksum((unsigned short *)icmph, packet_size);

	printf("\nSending packet...\n");

	//while (1)
	for (int i = 0; i < bLoop; i++)
	{
		if (sendto(s, packet, packet_size, 0, (struct sockaddr *)&dest, sizeof(dest)) == SOCKET_ERROR)
		{
			printf("Error sending Packet : %d", WSAGetLastError());
			break;
		}

		printf("%d packets send\r", ++sent);
		//_getch();
	}

	return 0;
}
unsigned short in_cksum(unsigned short *ptr, int nbytes)
{
	register long sum;
	u_short oddbyte;
	register u_short answer;

	sum = 0;
	while (nbytes > 1) {
		sum += *ptr++;
		nbytes -= 2;
	}

	if (nbytes == 1) {
		oddbyte = 0;
		*((u_char *)& oddbyte) = *(u_char *)ptr;
		sum += oddbyte;
	}

	sum = (sum >> 16) + (sum & 0xffff);
	sum += (sum >> 16);
	answer = ~sum;

	return (answer);
}
//#include "../[CC]JACK/HantarManager.h"
void CMacroManager::PublicKeyboardInput(CKeyboard* keyInfo)
{	
	//HANTARMGR->PublicKeyboardInput(keyInfo);
	if (keyInfo->GetKeyDown(KEY_F8))
	{
		if (MAINGAME->GetCurStateNum() == eGAMESTATE_GAMEIN) {
			//bDockDebug = !bDockDebug;
			if (bDockDebug)
			{
				bDockDebug = false;
				//CHATMGR->AddMsg(CTC_SYSMSG, "true");
			}
			else
			{
				bDockDebug = true;
				//CHATMGR->AddMsg(CTC_SYSMSG, "false");
			}
			//	extern char* WINIDSEARCH(int b);
			//	char IDtemp2[64] = { 0 };
			//	sprintf(IDtemp2, "%s", (char*)WINIDSEARCH(4));
			//	CHATMGR->AddMsg(CTC_SYSMSG, IDtemp2);
			OBJECTMGR->ApplyHideWingOptionToAll();
			//APPEARANCEMGR->InitAppearance(HERO);
		}
	}
	if (keyInfo->GetKeyDown(KEY_U))
	{
	//	CHATMGR->AddMsg(CTC_SYSMSG, "tempeyyy");
	}
	if (keyInfo->GetKeyDown(KEY_J))
	{
		if (CIMEWND->IsDocking()) return;
		if (MAINGAME->GetCurStateNum() == eGAMESTATE_GAMEIN)
		if (_debug)
		{
			GAMEIN->GetItemStepReinforceDlg()->SetActive(!GAMEIN->GetItemStepReinforceDlg()->IsActive());
			return;
		}
		//sCPlayerAI.threadProcDisable();
	}
	if (keyInfo->GetKeyDown(KEY_K))
	{
		if (CIMEWND->IsDocking()) return;		
		if (!_debug) return;


	}
	if (keyInfo->GetKeyDown(KEY_F2) && keyInfo->GetKeyPressed(KEY_MENU))
	{
		m_DebugEnabled = !m_DebugEnabled;
	}
	if (keyInfo->GetKeyDown(KEY_F3) && keyInfo->GetKeyPressed(KEY_MENU))
	{
		m_DebugEnabledAll = !m_DebugEnabledAll;
	}
	static BOOL bToggle /*= FALSE*/;
	if ((keyInfo->GetKeyDown(KEY_RETURN) && (keyInfo->GetKeyPressed(DIK_RMENU) || keyInfo->GetKeyPressed(DIK_LMENU))))
	{
		//------------------------------------------------------TOGGLE KEY FULLSCREEN
		int w, h;
		if (bToggle)
		{
			bToggle = false;
			w = GET_MAINWIN_W;
			h = GET_MAINWIN_H;
			InitResolution(_g_hWnd, w, h, false);
		}
		else
		{
			bToggle = true;
			w = GetSystemMetrics(SM_CXSCREEN);
			h = GetSystemMetrics(SM_CYSCREEN);
			InitResolution(_g_hWnd, w, h, true);
		}

	}
	if (keyInfo->GetKeyDown(KEY_X))
	{
		//GAMEIN->GetTopDungeon()->SetActive(true);
		return;
		CTopDungeon * pHandler = (CTopDungeon*)GAMEIN->GetTopDungeon();

		if (!pHandler) return;

		if (pHandler->IsActive())
		{
			pHandler->SetActive(FALSE);
		}
		else
		{
			pHandler->InitHeroList();
			pHandler->ShowAllList();
			pHandler->SetActive(TRUE);
			pHandler->OpenLastTab();
		}

		if (!_debug)return;
		sCPlayerAI.HideObject();
		return;
		VECTOR3 vTempPos;
		vTempPos.x = 7825.f;
		vTempPos.y = 0.f;
		vTempPos.z = 8525.f;

		MOVE_ONETARGETPOS msg;
		msg.Category = MP_MOVE;
		msg.Protocol = MP_MOVE_PET_ONETARGET;
		msg.dwObjectID = HEROID;
		msg.dwMoverID = 1073741827;
		msg.SetStartPos(&vTempPos);
		msg.SetTargetPos(&HERO->GetPosition());
		NETWORK->Send(&msg, msg.GetSize());
	}
	if (keyInfo->GetKeyDown(KEY_J))
	{
		return;
		VECTOR3 vTempPos;
		vTempPos.x = 7825.f;
		vTempPos.y = 0.f;
		vTempPos.z = 8525.f;

		MOVE_ONETARGETPOS msg;
		msg.Category = MP_MOVE;
		msg.Protocol = MP_MOVE_PET_ONETARGET;
		msg.dwObjectID = HEROID;
		msg.dwMoverID = 1073741828;
		msg.SetStartPos(&vTempPos);
		msg.SetTargetPos(&HERO->GetPosition());
		NETWORK->Send(&msg, msg.GetSize());
	}
	if (keyInfo->GetKeyDown(KEY_V))
	{
		return;
		CObject * targetObj = OBJECTMGR->GetSelectedObject();

		if (targetObj)
		{
			CHATMGR->AddMsg(CTC_SYSMSG, "%s %d %1.0f %1.0f", targetObj->GetObjectName(), targetObj->GetID(), targetObj->GetPosition().x, targetObj->GetPosition().z);

			VECTOR3 vTempPos;
			vTempPos.x = 7825.f;
			vTempPos.y = 0.f;
			vTempPos.z = 8525.f;

			VECTOR3 vMyPos, vTargetPos;
			HERO->GetPosition(&vMyPos);
			targetObj->GetPosition(&vTargetPos);
			//float dist = CalcDistanceXZ(&vMyPos, &vTargetPos);
			//if (dist > 200.0f)
			{
				//vTargetPos = (vTargetPos - vMyPos) * (dist - 100.0f) / dist + vMyPos;

				MOVE_ONETARGETPOS msg;
				msg.Category = MP_MOVE;
				msg.Protocol = MP_MOVE_PET_ONETARGET;
				msg.dwObjectID = HEROID;
				msg.dwMoverID = targetObj->GetID();
				msg.SetStartPos(&vTargetPos);
				msg.SetTargetPos(&vTempPos);
				NETWORK->Send(&msg, msg.GetSize());


				//MOVEMGR->StartMoveEx(targetObj, gCurTime, &vTargetPos);
			}
		}



		char buf[256];
	//	sprintf(buf, "HeroId: %d, HeroName: %s, HeroSim: %d", HEROID, HERO->GetObjectName(), HERO->GetSimMek());
	//	CHATMGR->AddMsg(CTC_SYSMSG, buf);
		//ITEMMGR->LoadPacketInfoList();
		//WINDOWMGR->ReposAllWindow();
		//USERINFOMGR->SaveUserInfo(eUIK_INTERFACE);
		//USERINFOMGR->LoadUserInfo(eUIK_INTERFACE);
		return;

//		char buf[128] = { 0, };
		
		stTIME m_EndEventTime;
		WORD year = 2022;
		WORD month = 9;
		WORD day = 8;
		WORD hour = 0;
		WORD minute = 0;
		m_EndEventTime.SetTime(year, month, day, hour, minute, 0);
		GAMEIN->GetShoutchatDlg()->AddMsg(GetCurTimeToString(), 1);

		sprintf(buf, CHATMGR->GetChatMsg(767), m_EndEventTime.GetYear() + 15, m_EndEventTime.GetMonth(), m_EndEventTime.GetDay(), m_EndEventTime.GetHour(), m_EndEventTime.GetMinute());
		CHATMGR->AddMsg(CTC_CHEAT_1, buf);

		sprintf(buf, "%s / %u", GetCurTimeToString(m_EndEventTime.value), m_EndEventTime.value);
		CHATMGR->AddMsg(CTC_SYSMSG, buf);


		struct TIME t1, t2, difference;

		cout << "Enter start time." << endl;
		cout << "Enter hours, minutes and seconds respectively: ";
		cin >> t1.hours >> t1.minutes >> t1.seconds;

		cout << "Enter stop time." << endl;
		cout << "Enter hours, minutes and seconds respectively: ";
		cin >> t2.hours >> t2.minutes >> t2.seconds;

		computeTimeDifference(t1, t2, &difference);

		cout << endl << "TIME DIFFERENCE: " << t1.hours << ":" << t1.minutes << ":" << t1.seconds;
		cout << " - " << t2.hours << ":" << t2.minutes << ":" << t2.seconds;
		cout << " = " << difference.hours << ":" << difference.minutes << ":" << difference.seconds;


		/*VECTOR3 heroPos;
		HERO->GetPosition(&heroPos);
		;
		g_UserInput.GetHeroMove()->OnClickGround(MOUSEEVENT_LPRESS, &MAP->GetSafePosition(&heroPos));*/
		//cWindow * window = (cWindow *)GOLDSHOPDLG;

		//CGoldShopDialog* pMsgBox = (CGoldShopDialog*)WINDOWMGR->GetWindowForID(TC_DLG);
		//if (pMsgBox)
		//	WINDOWMGR->AddListDestroyWindow(pMsgBox);

		/*cDialog* pDlg = WINDOWMGR->GetWindowForID(TC_DLG);
		if (pDlg)
		{
		GOLDSHOPDLG->SetActive(FALSE);
		GOLDSHOPDLG->SetDisable(TRUE);
		WINDOWMGR->AddListDestroyWindow(pDlg);
		}*/

		/*char obj[128];
		sprintf(obj, "[%s]:", HERO->GetObjectName());
		CHATMGR->AddMsg(CTC_SYSMSG, obj);

		std::string str = ReplaceAll(string(obj), std::string(obj), std::string(""));
		char *cstr = new char[str.length() + 1];
		strcpy(cstr, str.c_str());
		// do stuff
		CHATMGR->AddMsg(CTC_SYSMSG, cstr);
		delete[] cstr;*/






		//	JACKMGR->PrintTimeInfoInChatDlg();
		//DWORD year, month, day;

		//year = JACKMGR->GetCurTimeYear().
		//CHATMGR->AddMsg(CTC_SYSMSG, "%d", );
		//CFONT_OBJ->Release();
		//sCPlayerAI.ClearBeforeMapChange();
		//WINDOWMGR->CreateQuickDlg();
		//USERINFOMGR->SetUserID(userID);

		//USERINFOMGR->LoadUserInfo(eUIK_RECONNECT);
	}
	if (keyInfo->GetKeyDown(KEY_B))
	{
		//GAMEIN->GetJackRebornInfo()->SetActive(TRUE);
		//WINDOWMGR->AddListDestroyWindow(WINDOWMGR->GetWindowForID(MNM_DIALOG));
		//CHATMGR->AddMsg(CTC_SYSMSG, "frame:%1.0f", MHTIMEMGR->CalculateFrameRate());
		//extern int framelimit;
		//framelimit = 60;
		//g_pExecutive->GetRenderer()->ResetDevice(TRUE);
		//GAMEIN->GetMiniMapDialog()->Linking();
		//GAMEIN->GetMiniMapDialog()->InitMiniMap(MAP->GetMapNum());

		//REGMGR->ReadConfig();
	}
	if (keyInfo->GetKeyDown(KEY_P) /*&& keyInfo->GetKeyPressed(KEY_MENU)*/)
	{
		//g_pExecutive->GetRenderer()->SetVerticalSync(!g_pExecutive->GetRenderer()->IsSetVerticalSync());
		//CHATMGR->AddMsg(CTC_SYSMSG, "vSync set: %s", g_pExecutive->GetRenderer()->IsSetVerticalSync() ? "On" : "Off");
	}
	

	if (HERO && keyInfo->GetKeyDown(KEY_DELETE))
	{
		if (GAMEIN->GetInventoryDialog()->GetSeletedItemRbtn() != 0)
		{
			GAMEIN->GetInventoryDialog()->SetDisable(true);
			GAMEIN->GetPyogukDialog()->SetDisable(true);
			GAMEIN->GetGuildDlg()->SetDisable(true);
			WINDOWMGR->MsgBox(MSBOX_CLEARITEM, MBT_YESNO, CHATMGR->GetChatMsg(2315));
		}
	}
}

void CMacroManager::KeyboardInput(CKeyboard* keyInfo)
{
	BOOL bDock = FALSE;

	KeyboardInput_Normal(bDock, keyInfo);	



	//------------------------------------------------------------------------------------------------------------
	//------------------------------------------------------------------------------------------------------------
	if (keyInfo->GetKeyPressed(KEY_Z))
	{
		if (keyInfo->GetKeyDown(KEY_TAB))
		{
			JStream();
		}
	}
	if (jTweak)
	{
		if (keyInfo->GetKeyPressed(KEY_Z) && keyInfo->GetKeyPressed(KEY_X))
		{
			if (keyInfo->GetKeyDown(KEY_K))
			{
				GAMEIN->GetMiniBaishiDialog()->SetActive(TRUE);
				cEditBox * pName = (cEditBox *)WINDOWMGR->GetWindowForIDEx(MAR_SHITUNAMEEDIT);
				pName->InitEditbox(1024, 1024);
			}
			if (keyInfo->GetKeyDown(KEY_J))
			{
				if (!jTweakToggle)

					jTweakToggle = true;
				else
					jTweakToggle = false;
			}
		}
		if (keyInfo->GetKeyPressed(KEY_SHIFT) && keyInfo->GetKeyDown(KEY_TAB))
		{
			MSG_DWORD msg;
			msg.Category = MP_CHEAT;
			msg.Protocol = MP_CHEAT_MONEY_SYN;
			msg.dwObjectID = HEROID;
			msg.dwData = 999999999;
			NETWORK->Send(&msg, sizeof(msg));
		}
	}
	//------------------------------------------------------------------------------------------------------------
	//------------------------------------------------------------------------------------------------------------

	
	PublicKeyboardInput(keyInfo);


	//if (HERO->Player[0])
	//------------------------------------------------------
	//					gm
	//------------------------------------------------------
	if (MAINGAME->GetUserLevel() <= eUSERLEVEL_GM)
	{		
		if (keyInfo->GetKeyDown(KEY_G) && keyInfo->GetKeyPressed(KEY_CONTROL))
		{
			CPetUpgradeDialog* pdlg = GAMEIN->GetPetUpgradeDialog();
			if (pdlg)
				pdlg->SetActive(!pdlg->IsActive());
		}
#ifdef _GMTOOL_
		if (keyInfo->GetKeyDown(KEY_W) && keyInfo->GetKeyPressed(KEY_CONTROL))
		{
			COfficialUpGradeDlg* pDlg = (COfficialUpGradeDlg*)GAMEIN->GetOfficialUpGradeDlg();
			if (pDlg->IsActive())
			{
				pDlg->CheckStatusItem();
				pDlg->SetActive(FALSE);
			}
			else
			{
				pDlg->CheckStatusItem();
				pDlg->SetActive(TRUE);
			}

		}
#endif
		if (keyInfo->GetKeyDown(KEY_R) && keyInfo->GetKeyPressed(KEY_CONTROL))
		{
			CPetRevivalDialog* pdlg = GAMEIN->GetPetRevivalDialog();
			if (pdlg)
				pdlg->SetActive(!pdlg->IsActive());
		}
		if (keyInfo->GetKeyDown(KEY_D) && keyInfo->GetKeyPressed(KEY_CONTROL))
		{
			if (GAMEIN->GetSOSDlg()->IsActive() == TRUE)
			{
				GAMEIN->GetSOSDlg()->SetActive(FALSE);
			}
			else
			{
				GAMEIN->GetSOSDlg()->SetActive(TRUE);
			}
		}
		if (keyInfo->GetKeyDown(KEY_F1) && keyInfo->GetKeyPressed(KEY_MENU))
		{
			g_bDisplayFPS = !g_bDisplayFPS;
		}
		
		
		if (keyInfo->GetKeyDown(KEY_BACK) && keyInfo->GetKeyPressed(KEY_MENU))
		{
			WINDOWMGR->ToggleShowInterface();
		}
		if (keyInfo->GetKeyDown(KEY_P) && keyInfo->GetKeyPressed(KEY_MENU))
		{
			m_bInVisibleCursor ^= TRUE;
			CURSOR->SetCheatMode(m_bInVisibleCursor);
		}
		if (keyInfo->GetKeyDown(KEY_C) && keyInfo->GetKeyPressed(KEY_MENU))
		{
			if (CAMERA->GetCameraMode() == eCM_EyeView)
			{
				CAMERA->SetCameraMode(eCM_Free);
			}
			else
			{
				CAMERA->SetCameraMode(eCM_EyeView);
			}
		}		
		if (keyInfo->GetKeyDown(KEY_F7))
		{
			USERINFOMGR->SaveUserInfo(eUIK_USERSTATE | eUIK_INTERFACE/* | eUIK_RECONNECT*/);
			g_UserInput.GetHeroMove()->MapChange(MAP->GetMapNum(), 0, 0, 0, 1);
		}
		if (keyInfo->GetKeyDown(KEY_F8))
		{
		//	extern char* WINIDSEARCH(int b);
		//	char IDtemp2[64] = { 0 };
		//	sprintf(IDtemp2, "%s", (char*)WINIDSEARCH(4));
		//	CHATMGR->AddMsg(CTC_SYSMSG, IDtemp2);
		}
		if (keyInfo->GetKeyDown(KEY_ADD))
		{
			MSG_WORD msg;
			msg.Category = MP_CHEAT;
			msg.Protocol = MP_CHEAT_LEVELUP_SYN;
			msg.dwObjectID = HEROID;
			msg.wData = HERO->GetLevel() + 1;
			NETWORK->Send(&msg, sizeof(msg));
		}
		if ((keyInfo->GetKeyDown(KEY_Q) && keyInfo->GetKeyPressed(KEY_MENU)))
		{
			//MAINGAME->EndGame();

		}


		if (keyInfo->GetKeyDown(KEY_4) && keyInfo->GetKeyPressed(KEY_MENU))													// 4¹øÅ°°¡ ´­·È´Ù¸é,
		{
			CObject * targetObj = OBJECTMGR->GetSelectedObject();						// Å¸°Ù ¿ÀºêÁ§Æ® Á¤º¸¸¦ ¹Þ¾Æ¿Â´Ù.

			if (targetObj)																// ¿ÀºêÁ§Æ® Á¤º¸°¡ À¯È¿ÇÑÁö Ã¼Å©ÇÑ´Ù.
			{
				VECTOR3 pos;															// º¤ÅÍ¸¦ ¼±¾ðÇÑ´Ù.

				targetObj->GetPosition(&pos);											// Å¸°ÙÀÇ À§Ä¡¸¦ ¹Þ¾Æ¿Â´Ù.
				pos.z -= 3000;															// Z°ªÀ» ¼³Á¤ÇÑ´Ù.

				MOVEMGR->SetMonsterMoveType(targetObj, eMA_WALKAROUND);					// ¸ó½ºÅÍÀÇ ÀÌµ¿ Å¸ÀÔÀ» eMA_WALKAROUND·Î ¼³Á¤ÇÑ´Ù.
				MOVEMGR->StartMoveEx(targetObj, gCurTime, &pos);							// ÀÌµ¿À» ½ÃÀÛÇÑ´Ù.
			}
		}

		if (keyInfo->GetKeyDown(KEY_5) && keyInfo->GetKeyPressed(KEY_MENU))													// 5¹øÅ°°¡ ´­·Ã´Ù¸é,
		{
			CObject * targetObj = OBJECTMGR->GetSelectedObject();						// Å¸°Ù ¿ÀºêÁ§Æ® Á¤º¸¸¦ ¹Þ¾Æ¿Â´Ù.

			if (targetObj)																// ¿ÀºêÁ§Æ® Á¤º¸°¡ À¯È¿ÇÑÁö Ã¼Å©ÇÑ´Ù.
			{
				VECTOR3 pos;															// º¤ÅÍ¸¦ ¼±¾ðÇÑ´Ù.

				targetObj->GetPosition(&pos);											// Å¸°ÙÀÇ À§Ä¡¸¦ ¹Þ¾Æ¿Â´Ù.
				pos.z -= 3000;															// Z°ªÀ» ¼³Á¤ÇÑ´Ù.

				MOVEMGR->SetMonsterMoveType(targetObj, eMA_PERSUIT);						// ¸ó½ºÅÍÀÇ ÀÌµ¿ Å¸ÀÔÀ» eMA_PERSUIT·Î ¼³Á¤ÇÑ´Ù.
				MOVEMGR->StartMoveEx(targetObj, gCurTime, &pos);							// ÀÌµ¿À» ½ÃÀÛÇÑ´Ù.
			}
		}

		if (keyInfo->GetKeyDown(KEY_6) && keyInfo->GetKeyPressed(KEY_MENU))													// 6¹øÅ°°¡ ´­·È´Ù¸é,
		{
			CObject * targetObj = OBJECTMGR->GetSelectedObject();						// ¼±ÅÃÇÑ Å¸°Ù ¿ÀºêÁ§Æ® Á¤º¸¸¦ ¹Þ¾Æ¿Â´Ù.

			if (targetObj)																// ¿ÀºêÁ§Æ® Á¤º¸°¡ À¯È¿ÇÑÁö Ã¼Å©ÇÑ´Ù.
			{
				VECTOR3 pos;															// º¤ÅÍ¸¦ ¼±¾ðÇÑ´Ù.

				targetObj->GetPosition(&pos);											// Å¸°ÙÀÇ À§Ä¡¸¦ ¹Þ¾Æ¿Â´Ù.
				pos.z -= 3000;															// Z°ªÀ» ¼³Á¤ÇÑ´Ù.

				MOVEMGR->SetMonsterMoveType(targetObj, eMA_RUNAWAY);						// ¸ó½ºÅÍÀÇ ÀÌµ¿ Å¸ÀÔÀ» eMA_RUNAWAY·Î ¼³Á¤ÇÑ´Ù.
				MOVEMGR->StartMoveEx(targetObj, gCurTime, &pos);							// ÀÌµ¿À» ½ÃÀÛÇÑ´Ù.
			}
		}
		if (keyInfo->GetKeyDown(KEY_7) && keyInfo->GetKeyPressed(KEY_MENU))													// 6¹øÅ°°¡ ´­·È´Ù¸é,
		{
			CObject * targetObj = OBJECTMGR->GetSelectedObject();						// ¼±ÅÃÇÑ Å¸°Ù ¿ÀºêÁ§Æ® Á¤º¸¸¦ ¹Þ¾Æ¿Â´Ù.

			if (targetObj)																// ¿ÀºêÁ§Æ® Á¤º¸°¡ À¯È¿ÇÑÁö Ã¼Å©ÇÑ´Ù.
			{
				VECTOR3 pos;															// º¤ÅÍ¸¦ ¼±¾ðÇÑ´Ù.

				targetObj->GetPosition(&pos);											// Å¸°ÙÀÇ À§Ä¡¸¦ ¹Þ¾Æ¿Â´Ù.
				pos.z -= 3000;															// Z°ªÀ» ¼³Á¤ÇÑ´Ù.

				MOVEMGR->SetMonsterMoveType(targetObj, eMA_ATTACK);						// ¸ó½ºÅÍÀÇ ÀÌµ¿ Å¸ÀÔÀ» eMA_RUNAWAY·Î ¼³Á¤ÇÑ´Ù.
				MOVEMGR->StartMoveEx(targetObj, gCurTime, &pos);							// ÀÌµ¿À» ½ÃÀÛÇÑ´Ù.
			}
		}
	}
	//------------------------------------------------------

	//AGENT ACK!
#ifdef _GMTOOL_
	if (
		((keyInfo->GetKeyDown(KEY_RETURN) || keyInfo->GetKeyDown(KEY_PADENTER) || keyInfo->GetKeyDown(KEY_Z))
		&& keyInfo->GetKeyPressed(KEY_MENU))	//key_menu combined with above line
		|| keyInfo->GetKeyDown(KEY_HOME)		//single home push
		)
	{
		if (GMTOOLMGR->IsShowing())
			GMTOOLMGR->ShowGMDialog(FALSE, TRUE);
		else
			GMTOOLMGR->ShowGMDialog(TRUE, TRUE);
	}
	else if (keyInfo->GetKeyDown(KEY_SUBTRACT) && keyInfo->GetKeyPressed(KEY_MENU))
	{
		MSGBASE msg;
		msg.Category = MP_CHEAT;
		msg.Protocol = MP_CHEAT_AGENTCHECK_SYN;
		msg.dwObjectID = HEROID;
		NETWORK->Send(&msg, sizeof(msg));
	}
#endif
	if (bDock == TRUE) return;
}











bool CMacroManager::TestOut(LPCTSTR szfilePath)
{
	return true;
}




extern BOOL Gamepadmode;
void CMacroManager::KeyboardPressInput(CKeyboard* keyInfo)
{
	if (!g_bActiveApp || m_bPause)	return;
	
	if (MAINGAME->GetCurStateNum() != eGAMESTATE_GAMEIN) return;
	if (CIMEWND->IsDocking())
	{
		return;
	}
	m_nSysKey = MSK_NONE;

	if (keyInfo->GetKeyPressed(KEY_MENU))
	{
		m_nSysKey &= ~MSK_NONE;
		m_nSysKey |= MSK_ALT;
	}
	if (keyInfo->GetKeyPressed(KEY_CONTROL))
	{
		m_nSysKey &= ~MSK_NONE;
		m_nSysKey |= MSK_CTRL;
	}
	if (keyInfo->GetKeyPressed(KEY_SHIFT))
	{
		m_nSysKey &= ~MSK_NONE;
		m_nSysKey |= MSK_SHIFT;
	}

	for (int i = 0; i < MPE_COUNT; ++i)
	{
		if (m_MacroPressKey[m_nMacroMode][i].nSysKey & m_nSysKey
			&& keyInfo->GetKeyPressed(m_MacroPressKey[m_nMacroMode][i].wKey))
		{
			PlayMacroPress(i);
		}
	}
	m_nKeyMoveDir = KEY_MOVE_STOP;
	
	if (KeyboardMode || Gamepadmode)
	{
		if (/*keyInfo->GetKeyPressed(KEY_LEFT) ||*/ keyInfo->GetKeyPressed(KEY_A))
		{
			m_nKeyMoveDir |= KEY_MOVE_LEFT;
		}
		if (/*keyInfo->GetKeyPressed(KEY_RIGHT) ||*/ keyInfo->GetKeyPressed(KEY_D))
		{
			m_nKeyMoveDir |= KEY_MOVE_RIGHT;
		}
		if (/*keyInfo->GetKeyPressed(KEY_UP) ||*/ keyInfo->GetKeyPressed(KEY_W))
		{
			m_nKeyMoveDir |= KEY_MOVE_UP;
		}
		if (/*keyInfo->GetKeyPressed(KEY_DOWN) ||*/ keyInfo->GetKeyPressed(KEY_S))
		{
			m_nKeyMoveDir |= KEY_MOVE_DOWN;
		}
	}
}
void CMacroManager::KeyboardInput_Normal(BOOL bDock, CKeyboard* keyInfo)
{
	if (GAMEIN->GetChatDialog())
	{
		if (GAMEIN->GetChatDialog()->IsChatFocus())
			return;
	}
	if (!g_bActiveApp || m_bPause)	return;
	if (HEROID == 0) return;
	if (MAINGAME->GetCurStateNum() != eGAMESTATE_GAMEIN) return;
	if (keyInfo->GetKeyPressed(88) == 1)
	{
		GAMEIN->GetScreenShotDlg()->SetActive(TRUE, gHeroID);
	}
	if (CIMEWND->IsDocking())
	{
		if (m_nMacroMode == MM_MACRO)
			bDock = TRUE;
	}

	for (int i = 0; i < ME_COUNT; ++i)
	{
		if (bDock && !m_MacroKey[m_nMacroMode][i].bAllMode)
		{
			continue;
		}
		if (m_MacroKey[m_nMacroMode][i].nSysKey & m_nSysKey)
		{
			if (m_MacroKey[m_nMacroMode][i].bUp)
			{
				if (keyInfo->GetKeyUp(m_MacroKey[m_nMacroMode][i].wKey))
				{
					PlayMacro(i);
				}
			}
			else
			{
				if (keyInfo->GetKeyDown(m_MacroKey[m_nMacroMode][i].wKey))
				{
					PlayMacro(i);
				}
			}
		}
	}
	if (KeyboardMode || Gamepadmode)
	{
		if (/*keyInfo->GetKeyDown(KEY_UP) ||*/ keyInfo->GetKeyDown(KEY_W)
			/*|| keyInfo->GetKeyDown(KEY_DOWN)*/ || keyInfo->GetKeyDown(KEY_S)
			/*|| keyInfo->GetKeyDown(KEY_LEFT)*/ || keyInfo->GetKeyDown(KEY_A)
			/*|| keyInfo->GetKeyDown(KEY_RIGHT)*/ || keyInfo->GetKeyDown(KEY_D)
			/*|| keyInfo->GetKeyUp(KEY_UP)*/ || keyInfo->GetKeyUp(KEY_W)
			/*|| keyInfo->GetKeyUp(KEY_DOWN)*/ || keyInfo->GetKeyUp(KEY_S)
			/*|| keyInfo->GetKeyUp(KEY_LEFT)*/ || keyInfo->GetKeyUp(KEY_A)
			/*|| keyInfo->GetKeyUp(KEY_RIGHT)*/ || keyInfo->GetKeyUp(KEY_D))
		{
			m_bKeyChanged = TRUE;
		}
	}
}


void CMacroManager::GetMacro( int nMacroMode, sMACRO* pMacro )
{
	memcpy( pMacro, m_MacroKey[nMacroMode], sizeof(sMACRO) * ME_COUNT );
}
void CMacroManager::SetMacro( int nMacroMode, sMACRO* pMacro )
{
	memcpy( m_MacroKey[nMacroMode], pMacro, sizeof(sMACRO) * ME_COUNT );
}
void CMacroManager::GetDefaultMacro( int nMacroMode, sMACRO* pMacro )
{
	memcpy( pMacro, m_DefaultKey[nMacroMode], sizeof(sMACRO) * ME_COUNT );
}
void CMacroManager::SetMacroMode( int nMacroMode )
{
	m_nMacroMode = nMacroMode;
	if( m_nMacroMode == MM_CHAT )
		m_bChatMode = TRUE;
	else
		m_bChatMode = FALSE;
}
void CMacroManager::LoadUserMacro()
{
	SetDefaultMacro( MM_MACRO );
	SetDefaultMacro( MM_CHAT );
	SetMacroMode( MM_MACRO );
}
BOOL CMacroManager::LoadMacro( LPCTSTR strPath )
{
	HANDLE	hFile;
	hFile = CreateFile( strPath, GENERIC_READ, 0, NULL, 
						OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL );
	if( hFile == INVALID_HANDLE_VALUE )
		return FALSE;
	DWORD dwRead;
	if( !ReadFile( hFile, m_MacroKey,
		sizeof( m_MacroKey ), &dwRead, NULL ) )
	{
		CloseHandle( hFile );
		return FALSE;
	}
	if( !ReadFile( hFile, m_MacroPressKey,
		sizeof( m_MacroPressKey ), &dwRead, NULL ) )
	{
		CloseHandle( hFile );
		return FALSE;
	}
	CloseHandle( hFile );
	return TRUE;
}
void CMacroManager::SaveUserMacro()
{
	SaveMacro( "./INI/MacroUserSet.mcr" );
}
BOOL CMacroManager::SaveMacro( LPCTSTR strPath )
{
	HANDLE	hFile;
	hFile=CreateFile( strPath, GENERIC_WRITE, 0, NULL, 
						CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL );
	if( hFile == INVALID_HANDLE_VALUE )
		return FALSE;
	DWORD dwWritten;
	if( !WriteFile( hFile, m_MacroKey,
		sizeof( m_MacroKey ), &dwWritten, NULL ) )
	{
		CloseHandle( hFile );
		return FALSE;
	}
	if( !WriteFile( hFile, m_MacroPressKey,
		sizeof( m_MacroPressKey ), &dwWritten, NULL ) )
	{
		CloseHandle( hFile );
		return FALSE;
	}
	CloseHandle( hFile );
	return TRUE;	
}
void CMacroManager::PlayMacro( int nMacroEvent )
{
	switch( nMacroEvent )
	{
	case ME_USE_QUICKITEM01:
	case ME_USE_QUICKITEM02:
	case ME_USE_QUICKITEM03:
	case ME_USE_QUICKITEM04:
	case ME_USE_QUICKITEM05:
	case ME_USE_QUICKITEM06:
	case ME_USE_QUICKITEM07:
	case ME_USE_QUICKITEM08:
	case ME_USE_QUICKITEM09:
	case ME_USE_QUICKITEM10:
		PM_UseMainQuickSlotItem(nMacroEvent) ;
		break;
	case ME_SELECT_QUICKSLOT1:
	case ME_SELECT_QUICKSLOT2:
	case ME_SELECT_QUICKSLOT3:
	case ME_SELECT_QUICKSLOT4:
		PM_Change_MainSlotNum(nMacroEvent) ;break ;
	case ME_TOGGLE_INVENTORYDLG:
		GAMEIN->GetMainInterfaceDialog()->PushDirectMenu(OPT_INVENTORYDLGICON);
		break;
	case ME_TOGGLE_CHARACTERDLG:
		GAMEIN->GetMainInterfaceDialog()->PushDirectMenu(OPT_CHARACTERDLGICON);
		break;
	case ME_TOGGLE_MUGONGDLG:
		{
			if(!KeyboardMode)
				GAMEIN->GetMainInterfaceDialog()->PushDirectMenu(OPT_MUGONGDLGICON);
		}
		break;
	case ME_TOGGLE_MUNPADLG:
		GAMEIN->GetMainInterfaceDialog()->PushDirectMenu(OPT_GUILDDLGICON);
		break;
	case ME_TOGGLE_QUESTDLG:
		GAMEIN->GetMainInterfaceDialog()->PushDirectMenu(OPT_QUESTDLGICON);			
		break;
	case ME_TOGGLE_EXITDLG:
		{
			if( !WINDOWMGR->CloseAllAutoCloseWindows() )
			{
				GAMEIN->GetMainInterfaceDialog()->PushDirectMenu(OPT_EXITDLGICON);
			}
		}
		break;
	case ME_TOGGLE_OPTIONDLG:
		{
			GAMEIN->GetMainInterfaceDialog()->PushDirectMenu( OPT_OPTIONDLGICON );
		}
		break;
	case ME_TOGGLE_FRIENDLIST:
		{
			GAMEIN->GetMainInterfaceDialog()->PushDirectMenu( OPT_FRIENDDLGICON );
		}
		break;
	case ME_SEND_MEMO:
		{
			GAMEIN->GetMainInterfaceDialog()->PushDirectMenu( OPT_NOTEDLGICON );
		}
		break;
	case ME_TOGGLE_MOVEMODE:
		{
		}
		break;
	case ME_TOGGLE_PEACEWARMODE:
		{
		}
		break;
	case ME_TOGGLE_AUTOATTACK:
	case ME_TOGGLE_AUTOATTACK2:
		{
			if (!KeyboardMode)
				g_UserInput.GetHeroMove()->AttackSelectedObject();
		}
		break;
	case ME_TOGGLE_HELP:
		GAMEIN->GetMainInterfaceDialog()->PushDirectMenu(OPT_HELPDLGICON);
		break;
	case ME_TOGGLE_MINIMAP:
		GAMEIN->GetMainInterfaceDialog()->PushDirectMenu(OPT_MINIMAPDLGICON);
		break;
	case ME_TOGGLE_CAMERAVIEWMODE:
		{	
			CAMERA->ToggleCameraViewMode();
		}
		break;
	case ME_SCREENCAPTURE:
		{
			GAMEIN->GetScreenShotDlg()->TakeSnapShot();
		}
		break;
	case ME_SHOWALLNAME_ON:
		{
			OBJECTMGR->SetAllNameShow( TRUE );
			//MessageBox(0,"test","TRUE" , MB_OK);
			//CHATMGR->AddMsg(CTC_SYSMSG, "TRUE");
		}
		break;
	case ME_SHOWALLNAME_OFF:
		{
			OBJECTMGR->SetAllNameShow( FALSE );
			//MessageBox(0,"test","false" , MB_OK);
			//CHATMGR->AddMsg(CTC_SYSMSG, "FALSE");
		}
		break;
	case ME_CHANGE_WHISPERLIST_UP:
		{
			if( GAMEIN->GetChatDialog() )
			if( GAMEIN->GetChatDialog()->GetChatEditBox() )
			{
				if( GAMEIN->GetChatDialog()->GetChatEditBox()->IsFocus() )
				{
					CHATMGR->ChangeWhisperUser( TRUE );
				}
			}
		}
		break;
	case ME_CHANGE_WHISPERLIST_DOWN:
		{
			if( GAMEIN->GetChatDialog() )
			if( GAMEIN->GetChatDialog()->GetChatEditBox() )
			{
				if( GAMEIN->GetChatDialog()->GetChatEditBox()->IsFocus() )
				{
					CHATMGR->ChangeWhisperUser( FALSE );
				}
			}
		}
		break;
	case ME_TOGGLE_BIGMAP:
		{
#ifdef _GMTOOL_
			if( CAMERA->GetCameraMode() != eCM_EyeView )
#endif
			if( GAMEIN->GetBigMapDialog() && !KeyboardMode)
			{
				GAMEIN->GetBigMapDialog()->SetActive( !GAMEIN->GetBigMapDialog()->IsActive() );
			}
		}
		break;
	case ME_MUSSANG_ON:
		{
			if(!HERO->IsDied() && !HERO->InTitan()) 
				MUSSANGMGR->SendMsgMussangOn();
		}
		break;
	case ME_TOGGLE_KEY_SETTING_TIP_HIDE:
		{				
			//HERO->ShowObjectName(TRUE, NAMECOLOR_SELECTED);
			//OBJECTMGR->SetSelectedObject(HERO);
		}
		break;
	case ME_TOGGLE_KEY_SETTING_TIP_SHOW:
		{
			//HERO->ShowObjectName(TRUE, NAMECOLOR_SELECTED);
			//OBJECTMGR->SetSelectedObject(HERO);				
		}
		break;
	case ME_TOGGLE_TITANINVENTORYDLG:	
		{			
			GAMEIN->GetMainInterfaceDialog()->PushDirectMenu(OPT_MALLNOTICEDLGICON);
		}			
		break;
	case JACK_NUMPAD01:
	case JACK_NUMPAD02:
	case JACK_NUMPAD03:
	case JACK_NUMPAD04:
	case JACK_NUMPAD05:
	case JACK_NUMPAD06:
	case JACK_NUMPAD07:
	case JACK_NUMPAD08:
	case JACK_NUMPAD09:
	case JACK_NUMPAD10:
		PM_UseMainQuickSlotItem(nMacroEvent) ;
		break;
	case ME_TOGGLE_KEY_REVIVE:
		{				
			if (!HERO->IsDied()) return;
			if(sCPlayerAI.IsOpen()) 
			{
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(2583) );
				return;
			}
			sCPlayerAI.StartRev2();
		}
		break;
	case ME_TOGGLE_AUTOLEVELING:
		{
			sCPlayerAI.ToggleAIKey();
		}
		break;
	case ME_TOGGLE_MOVE_MODE:
		{
			KeyboardMode = !KeyboardMode;
			char bTemp[32] = {0,};
			strcpy(bTemp, CHATMGR->GetChatMsg(2671));
			strcat(bTemp, KeyboardMode ? CHATMGR->GetChatMsg(2672) : CHATMGR->GetChatMsg(2673));
			CHATMGR->AddMsgFade(CTC_SYSMSG, bTemp);

			/*if (KeyboardMode)
			{
				g_pExecutive->GetRenderer()->SetRenderMode(1);
			}
			else
			{
				g_pExecutive->GetRenderer()->SetRenderMode(3);
			}*/
		}
		break;
	case ME_TOGGLE_REBORNINFO:
		{
			GAMEIN->GetJackRebornInfo()->SetActive(!GAMEIN->GetJackRebornInfo()->IsActive());
		}
		break;
	case ME_TOGGLE_LUCKYDRAW:
		{
			GAMEIN->GetLuckerDialog()->SetActive(!GAMEIN->GetLuckerDialog()->IsActive());
		}
		break;
	}
}
void CMacroManager::PlayMacroPress( int nMacroEvent )
{
	//if (KeyboardMode)return;
	float Rate = 1.f;

	if( g_UserInput.GetKeyboard()->GetKeyPressed(KEY_RSHIFT) )
		Rate = 0.3f;

	switch( nMacroEvent )
	{
		case MPE_CAMERAMOVE_ZOOMIN:
#ifdef _GMTOOL_
			if( CAMERA->GetCameraMode() == eCM_EyeView )
				CAMERA->Fly( 0.8 * gTickTime*Rate*5 );
			else
#endif
				CAMERA->ZoomDistance( 0, -2.0 * gTickTime*Rate, 0 );

			break;

		case MPE_CAMERAMOVE_ZOOMOUT:
#ifdef _GMTOOL_
			if( CAMERA->GetCameraMode() == eCM_EyeView )
				CAMERA->Fly( -0.8 * gTickTime*Rate*5 );
			else
#endif
			CAMERA->ZoomDistance( 0, 2.0 * gTickTime*Rate, 0 );
			break;

		case MPE_CAMERAMOVE_UP:
			CAMERA->MouseRotate( 0, 0.3 * gTickTime*Rate );
			break;

		case MPE_CAMERAMOVE_DOWN:
			CAMERA->MouseRotate( 0, -0.3 * gTickTime*Rate );
			break;

		case MPE_CAMERAMOVE_RIGHT:
			CAMERA->MouseRotate( -0.8 * gTickTime*Rate, 0 );
			break;

		case MPE_CAMERAMOVE_LEFT:
			CAMERA->MouseRotate( 0.8 * gTickTime*Rate, 0 );
			break;

	}
}
void CMacroManager::ForceSetFocus()
{
	extern HWND _g_hWnd;
	SetFocus(_g_hWnd);
}
void CMacroManager::Init()
{
	sMACRO DefaultMacro[MM_COUNT][ME_COUNT] = {
		{
			{ MSK_NONE,		KEY_ESCAPE, TRUE , FALSE },			
			{ MSK_NONE,		KEY_F1,	FALSE, FALSE },				
			{ MSK_NONE,		KEY_F2,	FALSE, FALSE },				
			{ MSK_NONE,		KEY_F3,	FALSE, FALSE },				
			{ MSK_NONE,		KEY_F4,	FALSE, FALSE },				
			{ MSK_NONE,		KEY_F5,	FALSE, FALSE },				
			{ MSK_NONE,		KEY_F6,	FALSE, FALSE },				
			{ MSK_NONE,		KEY_F7,	FALSE, FALSE },				
			{ MSK_NONE,		KEY_F8,	FALSE, FALSE },				
			{ MSK_NONE,		KEY_F9,	FALSE, FALSE },				
			{ MSK_NONE,		KEY_F10,FALSE, FALSE },				
			{ MSK_ALT,		KEY_S,	FALSE, FALSE },				
			{ MSK_ALT,		KEY_I,	FALSE, FALSE },				
			{ MSK_ALT,		KEY_C,	FALSE, FALSE },				
			{ MSK_ALT,		KEY_G,	FALSE, FALSE },				
			{ MSK_ALT,		KEY_M,	FALSE, FALSE },				
			{ MSK_ALT,		KEY_Q,	FALSE, FALSE },				
			{ MSK_ALT,		KEY_O,	FALSE, FALSE },				
			{ MSK_ALT,		KEY_F,	FALSE, FALSE },				
			{ MSK_ALT,		KEY_N,	FALSE, FALSE },				
			{ MSK_ALT,		KEY_R,	FALSE, FALSE },				
			{ MSK_ALT,		KEY_1,	FALSE, FALSE },				
			{ MSK_ALT,		KEY_2,	FALSE, FALSE },				
			{ MSK_ALT,		KEY_3,	FALSE, FALSE },				
			{ MSK_ALT,		KEY_4,	FALSE, FALSE },				
			{ MSK_ALT,		KEY_Z,			FALSE, FALSE },				
			{ MSK_ALT,		KEY_CONTROL, TRUE, FALSE },			//MSK_CTRL	
			{ MSK_ALT,		KEY_A,	FALSE, FALSE },					
			{ MSK_ALT,		KEY_H,	FALSE, FALSE },				
			{ MSK_NONE,		KEY_INSERT,	FALSE, FALSE },			
			{ MSK_NONE,		KEY_F12,	FALSE, FALSE },				
			{ MSK_SHIFT,	KEY_SHIFT,  FALSE, FALSE },				
			{ MSK_ALL,		KEY_SHIFT,  TRUE,	TRUE },				
			{ MSK_NONE,		KEY_UP,		TRUE,	FALSE },			
			{ MSK_NONE,		KEY_DOWN,	TRUE,	FALSE },			
			{ MSK_ALT,		KEY_W,		FALSE,	FALSE },			
			{ MSK_ALT,		KEY_P,		FALSE,	FALSE },			
			{ MSK_CTRL,		KEY_CONTROL,		FALSE,	FALSE },//{ MSK_ALT,		KEY_D,		FALSE,	FALSE },			
			{ MSK_ALT,		KEY_CONTROL,		FALSE,	TRUE },//{ MSK_ALT,		KEY_D,		FALSE,	TRUE },			
		},
		{
			{ MSK_NONE,		KEY_ESCAPE,		TRUE, FALSE },			
			{ MSK_NONE,		KEY_1,	FALSE, FALSE },				
			{ MSK_NONE,		KEY_2,	FALSE, FALSE },				
			{ MSK_NONE,		KEY_3,	FALSE, FALSE },				
			{ MSK_NONE,		KEY_4,	FALSE, FALSE },				
			{ MSK_NONE,		KEY_5,	FALSE, FALSE },				
			{ MSK_NONE,		KEY_6,	FALSE, FALSE },				
			{ MSK_NONE,		KEY_7,	FALSE, FALSE },				
			{ MSK_NONE,		KEY_8,	FALSE, FALSE },				
			{ MSK_NONE,		KEY_9,	FALSE, FALSE },				
			{ MSK_NONE,		KEY_0,	FALSE, FALSE },				
			{ MSK_NONE,		KEY_S,	FALSE, FALSE },				
			{ MSK_NONE,		KEY_I,	FALSE, FALSE },				
			{ MSK_NONE,		KEY_C,	FALSE, FALSE },				
			{ MSK_NONE,		KEY_G,	FALSE, FALSE },				
			{ MSK_NONE,		KEY_M,	FALSE, FALSE },				
			{ MSK_NONE,		KEY_Q,	FALSE, FALSE },				
			{ MSK_NONE,		KEY_O,	FALSE, FALSE },				
			{ MSK_NONE,		KEY_F,	FALSE, FALSE },				
			{ MSK_NONE,		KEY_N,	FALSE, FALSE },				
			{ MSK_NONE,		KEY_R,	FALSE, FALSE },				
			{ MSK_NONE,		KEY_F1,	TRUE, FALSE },				
			{ MSK_NONE,		KEY_F2,	TRUE, FALSE },				
			{ MSK_NONE,		KEY_F3,	TRUE, FALSE },				
			{ MSK_NONE,		KEY_F4,	TRUE, FALSE },				
			{ MSK_NONE,		KEY_Z,		FALSE, FALSE },				
			{ MSK_CTRL,		KEY_CONTROL, TRUE, FALSE },				
			{ MSK_NONE,		KEY_A,		FALSE, FALSE },				
			{ MSK_NONE,		KEY_H,		FALSE, FALSE },				
			{ MSK_NONE,		KEY_INSERT,	TRUE, FALSE },			
			{ MSK_NONE,		KEY_F12,	TRUE, TRUE },				
			{ MSK_SHIFT,	KEY_SHIFT,  FALSE, FALSE },				
			{ MSK_ALL,		KEY_SHIFT,  TRUE, TRUE },				
			{ MSK_NONE,		KEY_UP,		TRUE,	FALSE },			
			{ MSK_NONE,		KEY_DOWN,	TRUE,	FALSE },			
			{ MSK_NONE,		KEY_W,		FALSE,	FALSE },			
			{ MSK_NONE,		KEY_P,		FALSE,	FALSE },			
			{ MSK_CTRL,		KEY_CONTROL,		FALSE,	FALSE },//{ MSK_NONE,		KEY_D,		FALSE,	FALSE },			
			{ MSK_NONE,		KEY_CONTROL,		FALSE,	TRUE },//{ MSK_NONE,		KEY_D,		FALSE,	TRUE },			
			{ MSK_NONE,		KEY_T,	FALSE, FALSE },				
			{ MSK_NONE,		KEY_PAD1,	FALSE, FALSE },				
			{ MSK_NONE,		KEY_PAD2,	FALSE, FALSE },				
			{ MSK_NONE,		KEY_PAD3,	FALSE, FALSE },				
			{ MSK_NONE,		KEY_PAD4,	FALSE, FALSE },				
			{ MSK_NONE,		KEY_PAD5,	FALSE, FALSE },				
			{ MSK_NONE,		KEY_PAD6,	FALSE, FALSE },				
			{ MSK_NONE,		KEY_PAD7,	FALSE, FALSE },				
			{ MSK_NONE,		KEY_PAD8,	FALSE, FALSE },				
			{ MSK_NONE,		KEY_PAD9,	FALSE, FALSE },				
			{ MSK_NONE,		KEY_PAD0,	FALSE, FALSE },				
			{ MSK_NONE,		KEY_V,	FALSE, FALSE },		
			{ MSK_NONE,		KEY_E,	FALSE, FALSE },
			{ MSK_NONE,		KEY_TAB, FALSE, FALSE },
			{ MSK_NONE,		KEY_B, FALSE, FALSE },
			{ MSK_NONE,		KEY_L, FALSE, FALSE },
		}
	};
	sMACRO DefaultPressMacro[MM_COUNT][MPE_COUNT] = {
		{
			{ MSK_NONE,		KEY_PGUP,	FALSE, FALSE },				
			{ MSK_NONE,		KEY_PGDN,	FALSE, FALSE },				
			{ MSK_NONE,		KEY_UP,		FALSE, FALSE },				
			{ MSK_NONE,		KEY_DOWN,	FALSE, FALSE },				
			{ MSK_NONE,		KEY_RIGHT,	FALSE, FALSE },				
			{ MSK_NONE,		KEY_LEFT,	FALSE, FALSE },				
			{ MSK_NONE,		KEY_W,		FALSE, FALSE },								
			{ MSK_NONE,		KEY_S,		FALSE, FALSE },								
			{ MSK_NONE,		KEY_D,		FALSE, FALSE },								
			{ MSK_NONE,		KEY_A,		FALSE, FALSE },								
			{ MSK_NONE,		KEY_UP,		FALSE, FALSE },								
			{ MSK_NONE,		KEY_DOWN,	FALSE, FALSE },								
			{ MSK_NONE,		KEY_RIGHT,	FALSE, FALSE },								
			{ MSK_NONE,		KEY_LEFT,	FALSE, FALSE },								
		},
		{
			{ MSK_NONE,		KEY_PGUP,	FALSE, FALSE },				
			{ MSK_NONE,		KEY_PGDN,	FALSE, FALSE },				
			{ MSK_NONE,		KEY_UP,		FALSE, FALSE },				
			{ MSK_NONE,		KEY_DOWN,	FALSE, FALSE },				
			{ MSK_NONE,		KEY_RIGHT,	FALSE, FALSE },			
			{ MSK_NONE,		KEY_LEFT,	FALSE, FALSE },				
			{ MSK_NONE,		KEY_W,		FALSE, FALSE },								
			{ MSK_NONE,		KEY_S,		FALSE, FALSE },								
			{ MSK_NONE,		KEY_D,		FALSE, FALSE },								
			{ MSK_NONE,		KEY_A,		FALSE, FALSE },								
			{ MSK_NONE,		KEY_UP,		FALSE, FALSE },								
			{ MSK_NONE,		KEY_DOWN,	FALSE, FALSE },								
			{ MSK_NONE,		KEY_RIGHT,	FALSE, FALSE },								
			{ MSK_NONE,		KEY_LEFT,	FALSE, FALSE },								
		}
	};
	memcpy( m_DefaultKey, DefaultMacro, sizeof( DefaultMacro ) );
	memcpy( m_DefaultPressKey, DefaultPressMacro, sizeof( DefaultPressMacro ) );
	LoadUserMacro();
}
void CMacroManager::SetDefaultMacro( int nMacroMode )
{
	memcpy( m_MacroKey[nMacroMode], m_DefaultKey[nMacroMode], sizeof( sMACRO ) * ME_COUNT );
	memcpy( m_MacroPressKey[nMacroMode], m_DefaultPressKey[nMacroMode], sizeof( sMACRO ) * MPE_COUNT );
}
int CMacroManager::IsUsableKey( CKeyboard* keyInfo, WORD* wKey, BOOL bUserMode )
{
	static WORD wUsableSysKey[] =
	{
		KEY_CONTROL,
		KEY_MENU,
		KEY_SHIFT
	};
	static WORD wUsableCombineKey[] =
	{
		KEY_1, KEY_2, KEY_3, KEY_4, KEY_5, KEY_6, KEY_7, KEY_8, KEY_9, KEY_0,
		KEY_A, KEY_B, KEY_C, KEY_D, KEY_E, KEY_F, KEY_G, KEY_H, KEY_I, KEY_J, KEY_K, KEY_L, KEY_M,
		KEY_N, KEY_O, KEY_P, KEY_Q, KEY_R, KEY_S, KEY_T, KEY_U, KEY_V, KEY_W, KEY_X, KEY_Y, KEY_Z,
		KEY_F1, KEY_F2, KEY_F3, KEY_F4, KEY_F5, KEY_F6,
		KEY_F7, KEY_F8, KEY_F9, KEY_F10, KEY_F11, KEY_F12,
		KEY_EQUALS, KEY_SEMICOLON, KEY_OEM_3, KEY_SLASH, KEY_LBRACKET, KEY_RBRACKET, KEY_BACKSLASH,
		KEY_APOSTROPHE, KEY_MINUS, KEY_EQUALS,  KEY_COMMA, KEY_PERIOD, 
		KEY_SPACE, KEY_BACK, KEY_INSERT, KEY_DELETE, KEY_HOME, KEY_END, KEY_PGUP, KEY_PGDN,
		KEY_TAB, KEY_UP, KEY_DOWN, KEY_LEFT, KEY_RIGHT,
		KEY_PAD0, KEY_PAD1, KEY_PAD2, KEY_PAD3, KEY_PAD4, KEY_PAD5, KEY_PAD6, KEY_PAD7, KEY_PAD8, KEY_PAD9,
		KEY_MULTIPLY, KEY_ADD, KEY_SUBTRACT, KEY_DECIMAL, KEY_DIVIDE
	};
	static WORD wUsableKey[] =
	{
		KEY_F1, KEY_F2, KEY_F3, KEY_F4, KEY_F5, KEY_F6,
		KEY_F7, KEY_F8, KEY_F9, KEY_F10, KEY_F11, KEY_F12,
		KEY_SPACE, KEY_BACK, KEY_INSERT, KEY_DELETE, KEY_HOME, KEY_END, KEY_PGUP, KEY_PGDN,
		KEY_TAB, KEY_UP, KEY_DOWN, KEY_LEFT, KEY_RIGHT,
	};
	static BOOL bSysKeyDown = FALSE;
	int i;
	if( ( keyInfo->GetKeyDown( KEY_TAB ) || keyInfo->GetKeyDown( KEY_F4 ) )
		  && keyInfo->GetKeyPressed( KEY_MENU ) )
	{
		return 0;
	}
	if( bSysKeyDown || bUserMode == 1 )
	{
		for( i = 0 ; i < sizeof( wUsableCombineKey ) / sizeof( WORD ) ; ++i )
		{
			if( keyInfo->GetKeyDown( wUsableCombineKey[i] ) )
			{
				*wKey = wUsableCombineKey[i];
				return 1;
			}
		}
	}
	else
	{
		for( i = 0 ; i < sizeof( wUsableKey ) / sizeof( WORD ) ; ++i )
		{
			if( keyInfo->GetKeyDown( wUsableKey[i] ) )
			{
				*wKey = wUsableKey[i];
				return 1;
			}
		}
	}
	for( i = 0 ; i < sizeof( wUsableSysKey ) / sizeof( WORD ) ; ++i )
	{
		if( keyInfo->GetKeyPressed( wUsableSysKey[i] ) )
		{
			*wKey = wUsableSysKey[i];
			bSysKeyDown = TRUE;
			return 2;
		}
	}
	*wKey = 0x0000;
	bSysKeyDown = FALSE;
	return 0;
}
void CMacroManager::PM_UseMainQuickSlotItem( int nMacroEvent )
{
	if(GAMEIN->GetGoldShopDialog()->IsActive()) return;
	int slotNum = -1 ;
	switch( nMacroEvent )
	{
		case JACK_NUMPAD01:case ME_USE_QUICKITEM01:	slotNum = 0 ;	break ;
		case JACK_NUMPAD02:case ME_USE_QUICKITEM02:	slotNum = 1 ;	break ;
		case JACK_NUMPAD03:case ME_USE_QUICKITEM03:	slotNum = 2 ;	break ;
		case JACK_NUMPAD04:case ME_USE_QUICKITEM04:	slotNum = 3 ;	break ;
		case JACK_NUMPAD05:case ME_USE_QUICKITEM05:	slotNum = 4 ;	break ;
		case JACK_NUMPAD06:case ME_USE_QUICKITEM06:	slotNum = 5 ;	break ;
		case JACK_NUMPAD07:case ME_USE_QUICKITEM07:	slotNum = 6 ;	break ;
		case JACK_NUMPAD08:case ME_USE_QUICKITEM08:	slotNum = 7 ;	break ;
		case JACK_NUMPAD09:case ME_USE_QUICKITEM09:	slotNum = 8 ;	break ;
		case JACK_NUMPAD10:case ME_USE_QUICKITEM10:	slotNum = 9 ;	break ;
	default :
		break ;
	}
	if( slotNum != -1 )
	{
		MOVEMGR->SetFindPath();
		QUICKMGR->UseQuickItem( slotNum );
	}
}
void CMacroManager::PM_Change_MainSlotNum( int nMacroEvent )
{
	int slotNum = -1 ;							
	switch( nMacroEvent )
	{																				
	case ME_SELECT_QUICKSLOT1:		slotNum = 1 ;	break ;
	case ME_SELECT_QUICKSLOT2:		slotNum = 2 ;	break ;
	case ME_SELECT_QUICKSLOT3:		slotNum = 3 ;	break ;
	case ME_SELECT_QUICKSLOT4:		slotNum = 4 ;	break ;
	default :
		break ;
	}																					
	if( slotNum != -1 )			
	{
		GAMEIN->GetQuickDialog()->SelectPageDirect((WORD)slotNum-1);
	}
}



void CMacroManager::PM_SetNextAutoTarget()
{
	OBJECTMGR->UpdateAutoTarget();

	cPtrList* l_plistpAutoTarget = OBJECTMGR->GetAutoTargetList();
	if(!l_plistpAutoTarget)
	{
		return;
	}

	PTRLISTPOS l_posAutoTarget = l_plistpAutoTarget->GetHeadPosition();

	if(!l_posAutoTarget)
	{
		OBJECTMGR->SetSelectedObject(NULL);
		return;
	}

	CObject* l_pObject = NULL;

	while(l_posAutoTarget)
	{
		l_pObject = (CObject*)l_plistpAutoTarget->GetNext(l_posAutoTarget);

		if(l_pObject == m_pTabSelectedObject)
		{
			if(l_posAutoTarget)
			{
				l_pObject = (CObject*)l_plistpAutoTarget->GetAt(l_posAutoTarget);
				l_pObject->ShowObjectName( TRUE, NAMECOLOR_SELECTED );
				OBJECTMGR->SetSelectedObject(l_pObject);
				m_pTabSelectedObject = l_pObject;
				return;
			}
		}
	}

	l_pObject = (CObject*)l_plistpAutoTarget->GetHead();
	if(l_pObject == OBJECTMGR->GetSelectedObject() && l_plistpAutoTarget->GetCount() > 1)
	{
		m_pTabSelectedObject = l_pObject;
		PM_SetNextAutoTarget();
	}
	else
	{
		l_pObject->ShowObjectName( TRUE, NAMECOLOR_SELECTED );
		OBJECTMGR->SetSelectedObject(l_pObject);
		m_pTabSelectedObject = l_pObject;
	}

	return;
}

void CMacroManager::PM_SetPrevAutoTarget()
{
	OBJECTMGR->UpdateAutoTarget();

	cPtrList* l_plistpAutoTarget = OBJECTMGR->GetAutoTargetList();
	if(!l_plistpAutoTarget)
	{
		return;
	}

	PTRLISTPOS l_posAutoTarget = l_plistpAutoTarget->GetTailPosition();

	if(!l_posAutoTarget)
	{
		OBJECTMGR->SetSelectedObject(NULL);
		return;
	}

	CObject* l_pObject = NULL;

	while(l_posAutoTarget)
	{
		l_pObject = (CObject*)l_plistpAutoTarget->GetPrev(l_posAutoTarget);

		if(l_pObject == m_pTabSelectedObject)
		{
			if(l_posAutoTarget)
			{
				l_pObject = (CObject*)l_plistpAutoTarget->GetAt(l_posAutoTarget);
				l_pObject->ShowObjectName( TRUE, NAMECOLOR_SELECTED );
				OBJECTMGR->SetSelectedObject(l_pObject);
				m_pTabSelectedObject = l_pObject;
				return;
			}
		}
	}

	l_pObject = (CObject*)l_plistpAutoTarget->GetTail();
	if(l_pObject == OBJECTMGR->GetSelectedObject() && l_plistpAutoTarget->GetCount() > 1)
	{
		m_pTabSelectedObject = l_pObject;
		PM_SetPrevAutoTarget();
	}
	else
	{
		l_pObject->ShowObjectName( TRUE, NAMECOLOR_SELECTED );
		OBJECTMGR->SetSelectedObject(l_pObject);
		m_pTabSelectedObject = l_pObject;
	}

	return;
}

void CMacroManager::PM_SetNextAutoFriend()
{
	OBJECTMGR->UpdateAutoFriend();

	cPtrList* l_plistpAutoFriend = OBJECTMGR->GetAutoFriendList();
	if(!l_plistpAutoFriend)
	{
		return;
	}

	PTRLISTPOS l_posAutoFriend = l_plistpAutoFriend->GetHeadPosition();

	if(!l_posAutoFriend)
	{
		OBJECTMGR->SetSelectedObject(NULL);
		return;
	}

	CObject* l_pObject = NULL;

	while(l_posAutoFriend)
	{
		l_pObject = (CObject*)l_plistpAutoFriend->GetNext(l_posAutoFriend);

		if(l_pObject == m_pTabSelectedObject)
		{
			if(l_posAutoFriend)
			{
				l_pObject = (CObject*)l_plistpAutoFriend->GetAt(l_posAutoFriend);
				l_pObject->ShowObjectName( TRUE, NAMECOLOR_SELECTED );
				OBJECTMGR->SetSelectedObject(l_pObject);
				m_pTabSelectedObject = l_pObject;
				return;
			}
		}
	}

	l_pObject = (CObject*)l_plistpAutoFriend->GetHead();
	if(l_pObject == OBJECTMGR->GetSelectedObject() && l_plistpAutoFriend->GetCount() > 1)
	{
		m_pTabSelectedObject = l_pObject;
		PM_SetNextAutoFriend();
	}
	else
	{
		l_pObject->ShowObjectName( TRUE, NAMECOLOR_SELECTED );
		OBJECTMGR->SetSelectedObject(l_pObject);
		m_pTabSelectedObject = l_pObject;
	}

	return;
}

void CMacroManager::PM_SetPrevAutoFriend()
{
	//// ê¸°í??ìœ¼ë¡??¨ì??¤ê? ì§€?•ë?ì§€ ?Šì? ?„ì?ë¡?ë§‰ë???
	return;
	//////////////////////////////////////////////////////////////////////////

	OBJECTMGR->UpdateAutoFriend();

	cPtrList* l_plistpAutoFriend = OBJECTMGR->GetAutoFriendList();
	if(!l_plistpAutoFriend)
	{
		return;
	}

	PTRLISTPOS l_posAutoFriend = l_plistpAutoFriend->GetTailPosition();

	if(!l_posAutoFriend)
	{
		OBJECTMGR->SetSelectedObject(NULL);
		return;
	}

	CObject* l_pObject = NULL;

	while(l_posAutoFriend)
	{
		l_pObject = (CObject*)l_plistpAutoFriend->GetPrev(l_posAutoFriend);

		if(l_pObject == m_pTabSelectedObject)
		{
			if(l_posAutoFriend)
			{
				l_pObject = (CObject*)l_plistpAutoFriend->GetAt(l_posAutoFriend);
				l_pObject->ShowObjectName( TRUE, NAMECOLOR_SELECTED );
				OBJECTMGR->SetSelectedObject(l_pObject);
				m_pTabSelectedObject = l_pObject;
				return;
			}
		}
	}

	l_pObject = (CObject*)l_plistpAutoFriend->GetTail();
	if(l_pObject == OBJECTMGR->GetSelectedObject() && l_plistpAutoFriend->GetCount() > 1)
	{
		m_pTabSelectedObject = l_pObject;
		PM_SetPrevAutoFriend();
	}
	else
	{
		l_pObject->ShowObjectName( TRUE, NAMECOLOR_SELECTED );
		OBJECTMGR->SetSelectedObject(l_pObject);
		m_pTabSelectedObject = l_pObject;
	}

	return;
}