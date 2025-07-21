#include "stdafx.h"
#include "MainTitle.h"
#include "MainGame.h"
#include "cWindowSystemFunc.h"
#include "WindowIDEnum.h"
#include "MHCamera.h"
#include "AppearanceManager.h"
#include "./Interface/cWindowManager.h"
#include "./Interface/cEditBox.h"
#include "./Interface/cResourceManager.h"
#include "../[CC]Header/GameResourceManager.h"
#include "ItemManager.h"
#include "KyungGongManager.h"
#include "ChatManager.h"
#include "MHTimeManager.h"
#include "FilteringTable.h"
#include "cMsgBox.h"
#include "SkillManager_client.h"
#include "mhMap.h"
#include "./Input/Mouse.h"
#include "./Input/Keyboard.h"
#include "MotionManager.h"
#include "AbilityManager.h"
#include "AbilityUpdater_Interface.h"
#include "AbilityUpdater_ApplyData.h"
#include "ServerListDialog.h"
#include "CharSelect.h"
#include "DissolutionManager.h"
#include "cMonsterSpeechManager.h"
#include "PetSpeechManager.h"
#include "TitanManager.h"	
#include "OptionManager.h"
#ifdef _GMTOOL_
#include "GMToolManager.h"
#endif
#ifdef TAIWAN_LOCAL
#include "BillingManager.h"
#endif
#include "GMNotifyManager.h"
#include "ObjectManager.h"
#include "MoveManager.h"
#include "UserInfoManager.h"
#include "cScriptManager.h"
#include "./Input/UserInput.h"
#include "Intro.h"
#include "IntroReplayDlg.h"
#include "NumberPadDialog.h"
#include "GameIn.h"
#include "ErrorMsg.h"
#include <time.h>
#include <shellapi.h>
#include "SafeArea.h"
#include "Reconnect.h"
#include "DungeonMgr.h"
#include "ReconnectDialog.h"
#include "FameManager.h"

#include "RegistryManager.h"
#define _LOGOWINDOW_
extern char g_DistributeAddr[16];
extern WORD g_DistributePort;
extern char g_AgentAddr[16];
extern WORD g_AgentPort;
extern char g_szHeroIDName[];
extern int	g_nServerSetNum;

extern BOOL _debug;

#define ALPHA_PROCESS_DEBUG_TIME	40

#define ALPHA_PROCESS_TIME	1500




#define WAIT_CONNECT_TIME	60000

float Rate = 0.f;
float AngleRate = 0.f;
float AngleSideRate = 0.f;
float UpDownRate = 0.f;
float YRate = 0.f;
VECTOR3 v3EyeT;
VECTOR3 v3RotT;

void SendOutMsg()
{
	MSGBASE msg;
	msg.Category = MP_USERCONN;
	msg.Protocol = MP_USERCONN_REQUEST_DISTOUT;
	NETWORK->Send(&msg, sizeof(msg));
}
BOOL bFirst;
BOOL ReadConfigReaded;
GLOBALTON(CMainTitle)
CMainTitle::CMainTitle()
{
	m_pLogoWindow = NULL;
	m_bInit = FALSE;
	m_pServerListDlg = NULL;
	memset(m_DistributeAddr, 0, 16);
	m_DistributePort = 0;
	m_bServerList = FALSE;
	m_bDisconntinToDist = FALSE;
	m_bDynamicDlg = FALSE;
	m_pIntroReplayDlg = NULL;
	ReadConfigReaded = FALSE;
	LoadInitInfo();
}
CMainTitle::~CMainTitle()
{
}
static BOOL g_bResourceLoaded = FALSE;

void CMainTitle::LoadInitInfo()
{
	char buff[64] = { 0, };
	CMHFile file;
	if (file.Init("./Resource/Client/MainTitle.bin", "rb") == FALSE)
		return;
	while (1)
	{
		if (file.IsEOF())
			break;
		file.GetString(buff);
		START_STRING_COMPARE(buff)
		COMPARE("#MAPNUM")		sInitConfig.wMapNum = file.GetWord();			
		COMPARE("#VPOINT")		sInitConfig.vPivot.x = file.GetFloat();	sInitConfig.vPivot.y = file.GetFloat();	sInitConfig.vPivot.z = file.GetFloat();		
		COMPARE("#CAMERANUM")	sInitConfig.CameraNum = file.GetWord();			
		COMPARE("#ANGLE")		sInitConfig.AngleX = file.GetFloat();	sInitConfig.AngleY = file.GetFloat();		
		COMPARE("#DISTANCE")	sInitConfig.fDistance = file.GetFloat();			
		END_COMPARE
	}
	file.Release();
}
BOOL CMainTitle::Init(void* pInitParam)
{
	CreateReconnectDlg();
	m_imageLoginBarTop.		LoadSprite("image/2D/login_bar00.TIF");
	m_imageLoginBarBottom.	LoadSprite("image/2D/login_bar01.TIF");

	char filename[256];
	sprintf(filename, "Image/LoadingMap/maploadingimage%02d.tga", 17);
	if (m_imageLoginBackGround.LoadSprite(filename) == FALSE)
	{
		sprintf(filename, "Image/LoadingMap/maploadingimage00.tga");
		m_imageLoginBackGround.LoadSprite(filename);
	}

	m_pLogoWindow		= NULL;
	m_bInit				= FALSE;
	m_pServerListDlg	= NULL;
	memset(m_DistributeAddr, 0, 16);
	m_DistributePort = 0;
	m_bDisconntinToDist = FALSE;
	m_bNoDiconMsg		= FALSE;
	m_bDynamicDlg		= FALSE;
	m_pIntroReplayDlg	= NULL;
	NETWORK->SetCurState(this);
	if (MAP->IsInited() == FALSE)
	{
		MAP->InitMap(sInitConfig.wMapNum);
	}
	else
	{
		MAP->Release();
		MAP->InitMap(sInitConfig.wMapNum);
	}
	if (g_bResourceLoaded == FALSE)
	{
		FILTERTABLE->Init();
		CHATMGR->Init();
		MON_SPEECHMGR->Init();
		PET_SPEECHMGR->Init();
		MOTIONMGR->LoadMotionList();
		SKILLMGR->Init();
		ITEMMGR->LoadItemList();
		ITEMMGR->LoadAutoUseBuffItem();

		ITEMMGR->LoadEventMap();
		ITEMMGR->LoadEventMapItemBlock();

		ITEMMGR->LoadCustomeHeadPair();
		ITEMMGR->LoadMixItemInfo();
		ITEMMGR->LoadItemLucker();
		ITEMMGR->LoadItemGrowSet();
		FAMEMGR->LoadStageLogo();
		ITEMMGR->LoadItemToolTipList();
		ITEMMGR->LoadSetItemOption();
		ITEMMGR->LoadRareItemInfo();
#ifdef _OLDGOLDDIALOG_
		ITEMMGR->LoadGoldShopItem();
#endif
		GAMERESRCMNGR->LoadMonsterList();
		GAMERESRCMNGR->LoadMonsterInfoInMapList();
		GAMERESRCMNGR->LoadMapChangeArea();
		GAMERESRCMNGR->LoadUserCommendList();
		GAMERESRCMNGR->LoadNpcChxList();
		GAMERESRCMNGR->LoadNpcList(GAMERESRCMNGR->m_NpcList);
		GAMERESRCMNGR->LoadItemChxList();
		GAMERESRCMNGR->LoadPetList();
		GAMERESRCMNGR->LoadPetRule();
		GAMERESRCMNGR->LoadPetBuffList();
		GAMERESRCMNGR->LoadTitanList();
		GAMERESRCMNGR->LoadExpPoint();
		GAMERESRCMNGR->LoadTacticStartInfoList();
		GAMERESRCMNGR->LoadPreDataTable();
		GAMERESRCMNGR->LoadServerList();
		GAMERESRCMNGR->LoadShopItemDupList();
		GAMERESRCMNGR->LoadFlagNpcInfo();
		KYUNGGONGMGR->Init();
		APPEARANCEMGR->Init();
		GAMERESRCMNGR->PreLoadData();
		ABILITYMGR->Init();
		ABILITYMGR->AddUpdater(CAbilityUpdater_Interface::GetInstance());
		ABILITYMGR->AddUpdater(CAbilityUpdater_ApplyData::GetInstance());
		DISSOLUTIONMGR->Init();
		TITANMGR->LoadTPMList();
		TITANMGR->LoadTitanPartsKindList();
		TITANMGR->LoadTitanUpgradeInfoList();
		TITANMGR->LoadTitanBreakInfoList();
		GAMERESRCMNGR->LoadTitanRule();
		GAMERESRCMNGR->LoadUniqueItemOptionList();
		GAMERESRCMNGR->LoadUniqueItemMixList();
		GAMERESRCMNGR->LoadNomalClothesSkinList();
		GAMERESRCMNGR->LoadCostumeSkinList();
		GAMERESRCMNGR->LoadMapKindInfo();
		GAMERESRCMNGR->LoadPlusItemEffect();
		GAMERESRCMNGR->LoadNewItemEffect();
		GAMERESRCMNGR->LoadNewItemPairEffect();
		GAMERESRCMNGR->LoadRebornSetting();
		DungeonMGR->LoadFBEntranceCondition();
		DungeonMGR->LoadToolTipList();
		GAMERESRCMNGR->LoadDataUpGrade();
		ITEMMGR->LoadSetItemQualityOption();
		g_bResourceLoaded = TRUE;
	}

	//-------------NEW CAMERA BY JACK
	/*VECTOR2 PosScale = { 1, 1 };
	float scaleRate = (float)GAMERESRCMNGR->m_GameDesc.dispInfo.dwWidth - MIDRSLTN_W;
	PosScale.x = scaleRate;

	VECTOR3 v3Eye;
	VECTOR3 v3Rot;
	v3Eye = { sInitConfig.vPivot.x, sInitConfig.vPivot.y, sInitConfig.vPivot.z };
	v3Rot = { sInitConfig.AngleX - (PosScale.x / 10000), sInitConfig.AngleY, 0.0f };

	g_pExecutive->GetGeometry()->SetCameraPos(&v3Eye, 0);
	g_pExecutive->GetGeometry()->SetCameraAngleRad(&v3Rot, 0);

	CAMERA->SetCameraMode(eCM_LOGIN);
	CAMERA->SetCurCamera(0);*/

	//-----------------OLD CAMERA JS
	CAMERA->InitCamera(sInitConfig.CameraNum, sInitConfig.AngleX, sInitConfig.AngleY, sInitConfig.fDistance, &sInitConfig.vPivot);


	WINDOWMGR->SetcbDropProcess(cbDragDropProcess);


	
	CreateMainTitle_m();
	m_pLogoWindow = WINDOWMGR->GetWindowForID(MT_LOGODLG);
	if (m_pLogoWindow)
	{
		m_pLogoWindow->SetActive(TRUE);
	}
		
	m_pServerListDlg = (CServerListDialog*)WINDOWMGR->GetWindowForID(SL_SERVERLISTDLG);
	if (m_pServerListDlg)
	{
		m_pServerListDlg->Linking();
		m_pServerListDlg->SetActive(FALSE);
	}
	if (REDISCONN->GetStart())
	{
		SKILLMGR->ReLoadSkillInfo();
		m_pReconnectDlg = (CReconnectDialog*)WINDOWMGR->GetWindowForID(RECONNECT_DLG);
		if (m_pReconnectDlg)
			m_pReconnectDlg->AddMessage(IMAGEMSG, 1348, (eReconnectDlg)BUTTON);
	}
	WINDOWMGR->AfterInit();
	m_bWaitConnectToAgent = FALSE;
	bFirst = TRUE;
	m_pIntroReplayDlg = (CIntroReplayDlg*)WINDOWMGR->GetWindowForID(INTRO_DLG);
	if (m_pIntroReplayDlg)
	{
		m_pIntroReplayDlg->SetAbsXY(GET_MAINWIN_W - 41, GET_MAINWIN_H - 120);
		m_pIntroReplayDlg->SetActive(FALSE);
	}

	return TRUE;
}
void CMainTitle::Release(CGameState* pNextGameState)
{
	WINDOWMGR->SetDragEnd();
	m_imageLoginBarTop.Release();
	m_imageLoginBarBottom.Release();
	m_imageLoginBackGround.Release();

	WINDOWMGR->DestroyWindowAll();
	RESRCMGR->ReleaseResource(24);
	if (pNextGameState == NULL)
		MAP->Release();
}

void CameraRotate(CKeyboard& Keyboard, CMouse& Mouse);
void CameraWheelZoom(CKeyboard& Keyboard, CMouse& Mouse);
#include "ObjectGuagen.h"

BOOL NextFadeInit = false;
BOOL NextFade = false;

BOOL FinalFadeInit = false;
BOOL FinalFade = false;

BOOL FadeEnd = false;

BOOL FinalFinalFade = false;
BOOL redisStart = FALSE;
void CMainTitle::ProcessNormal()
{
	if (m_pLogoWindow && NextFade == false && FadeEnd == false)
	{
		DWORD dwElapsed = MHTIMEMGR->GetNewCalcCurTime() - m_dwStartTime;
		if (dwElapsed > (ALPHA_PROCESS_TIME-500))
		{
			NextFadeInit = TRUE;
			FadeEnd = TRUE;
			m_pLogoWindow->SetAlpha(0);
		}
		else
		{
			m_pLogoWindow->SetAlpha((BYTE)(255 - (dwElapsed * 255 / (ALPHA_PROCESS_TIME - 500))));
		}
	}
	if (NextFadeInit)
	{
		NextFadeInit = FALSE;

		cImage image;
		SCRIPTMGR->GetImage(142, &image, PFT_JACKPATH);
		m_pLogoWindow->SetBasicImage(&image);
		
		m_dwStartTime = MHTIMEMGR->GetNewCalcCurTime();
		NextFade = TRUE;
	}
	if (NextFade)
	{
		DWORD dwElapsed = MHTIMEMGR->GetNewCalcCurTime() - m_dwStartTime;
		if (dwElapsed > ALPHA_PROCESS_TIME)
		{
			m_pLogoWindow->SetAlpha(255);
			NextFade = FALSE;
			FinalFadeInit = TRUE;
		}
		else
		{
			m_pLogoWindow->SetAlpha((BYTE)(0 + (dwElapsed * 255 / ALPHA_PROCESS_TIME)));			
		}
	}
	if (FinalFadeInit)
	{
		FinalFadeInit = FALSE;

		m_dwStartTime = MHTIMEMGR->GetNewCalcCurTime();
		FinalFade = TRUE;
	}
	if (FinalFade)
	{
		DWORD dwElapsed = MHTIMEMGR->GetNewCalcCurTime() - m_dwStartTime;
		if (dwElapsed > ALPHA_PROCESS_TIME )
		{
			FinalFade = FALSE;
			m_pLogoWindow->SetAlpha(0);
			WINDOWMGR->AddListDestroyWindow(m_pLogoWindow);
			m_pLogoWindow = NULL;
			m_bServerList = TRUE;
		}
		else
		{
			m_pLogoWindow->SetAlpha((BYTE)(255 - (dwElapsed * 255 / ALPHA_PROCESS_TIME )));
		}
	}

	if (m_bServerList)
	{
		m_imageLoginBackGround.Release();
		m_pServerListDlg->SetActive(TRUE);
		
		m_bServerList = FALSE;
		m_pIntroReplayDlg->SetActive(FALSE);

		m_pServerListDlg->SetAlpha(0);
		FinalFinalFade = TRUE;
		m_dwStartTime = MHTIMEMGR->GetNewCalcCurTime();
	}
	if (FinalFinalFade)
	{
		DWORD dwElapsed = MHTIMEMGR->GetNewCalcCurTime() - m_dwStartTime;
		if (dwElapsed > 500)
		{
			FinalFinalFade = FALSE;
			m_pServerListDlg->SetAlpha(255);

			
			
		}
		else
		{
			m_pServerListDlg->SetAlpha((BYTE)(0 + (dwElapsed * 255 / 500)));
		}
	}
}
void CMainTitle::ProcessDebug()
{
	if (m_pLogoWindow)
	{
		DWORD dwElapsed = MHTIMEMGR->GetNewCalcCurTime() - m_dwStartTime;

		if (dwElapsed > ALPHA_PROCESS_DEBUG_TIME)
		{
			m_pLogoWindow->SetAlpha(0);
			WINDOWMGR->AddListDestroyWindow(m_pLogoWindow);
			m_pLogoWindow = NULL;


			m_bServerList = TRUE;

		}
		else
		{
			m_pLogoWindow->SetAlpha((BYTE)(255 - (dwElapsed * 255 / ALPHA_PROCESS_DEBUG_TIME)));
		}
	}

	if (m_bServerList)
	{
		m_imageLoginBackGround.Release();
		m_pIntroReplayDlg->SetActive(FALSE);
		m_pServerListDlg->SetActive(TRUE);
		m_bServerList = FALSE;

		if (!redisStart)
			{
				redisStart = TRUE;
			}
	}
}
void CMainTitle::KeyboardProcess()
{
	//keyboard function
	{
		if (KEYBOARD->GetKeyPressed(KEY_D))
		{
			Rate = Rate + 1.f;
		}
		if (KEYBOARD->GetKeyPressed(KEY_A))
		{
			Rate = Rate - 1.f;
		}
		if (KEYBOARD->GetKeyPressed(KEY_W))
		{
			UpDownRate = UpDownRate + 1.f;
		}
		if (KEYBOARD->GetKeyPressed(KEY_S))
		{
			UpDownRate = UpDownRate - 1.f;
		}
		/*if (KEYBOARD->GetKeyPressed(KEY_MENU))
		{
		//if (KEYBOARD->GetKeyPressed(KEY_UP))
		{
		AngleRate = AngleRate + CAMERA->GetCameraDesc()->m_AngleX.ToDeg();
		}

		//if (KEYBOARD->GetKeyPressed(KEY_RIGHT))
		{
		AngleSideRate = AngleSideRate + CAMERA->GetCameraDesc()->m_AngleY.ToDeg();
		}


		}
		else*/
		{
			if (KEYBOARD->GetKeyPressed(KEY_UP))
			{
				AngleRate = AngleRate + 0.001f;
			}
			if (KEYBOARD->GetKeyPressed(KEY_DOWN))
			{
				AngleRate = AngleRate - 0.001f;
			}
			if (KEYBOARD->GetKeyPressed(KEY_RIGHT))
			{
				AngleSideRate = AngleSideRate + 0.001f;
			}
			if (KEYBOARD->GetKeyPressed(KEY_LEFT))
			{
				AngleSideRate = AngleSideRate - 0.001f;
			}
		}



		if (KEYBOARD->GetKeyPressed(KEY_ADD))
		{
			YRate = YRate + 1.1f;
		}
		if (KEYBOARD->GetKeyPressed(KEY_SUBTRACT))
		{
			YRate = YRate - 1.1f;
		}
		if (KEYBOARD->GetKeyPressed(KEY_DELETE))
		{
			YRate = Rate = AngleRate = AngleSideRate = UpDownRate = 0.f;
		}


		VECTOR2 PosScale = { 1, 1 };
		float scaleRate = (float)GAMERESRCMNGR->m_GameDesc.dispInfo.dwWidth - MIDRSLTN_W;
		PosScale.x = scaleRate;

		/*v3EyeT = { sInitConfig.vPivot.x + Rate, sInitConfig.vPivot.y + YRate, sInitConfig.vPivot.z + UpDownRate };


		v3RotT = { sInitConfig.AngleX - (PosScale.x / 10000) + AngleRate, sInitConfig.AngleY + AngleSideRate, 0.0f };

		g_pExecutive->GetGeometry()->SetCameraPos(&v3EyeT, 0);
		g_pExecutive->GetGeometry()->SetCameraAngleRad(&v3RotT, 0);*/

		CAMERA->InitCamera(sInitConfig.CameraNum, sInitConfig.AngleX - (PosScale.x / 10000) + AngleRate, sInitConfig.AngleY + AngleSideRate, sInitConfig.fDistance, &sInitConfig.vPivot);

	}
}
void CMainTitle::Process()
{
	if (bFirst == TRUE)
	{
		bFirst = FALSE;
		CAMERA->MouseRotate(1, 0);
		m_dwStartTime = MHTIMEMGR->GetNewCalcCurTime();
	}

	if (_debug)
	{
		ProcessDebug();
	}
	else
	{
		ProcessNormal();
	}
	//KeyboardProcess();
	if (m_bInit)
	{
		InitConfig();
		if (m_bDynamicDlg == FALSE)
		{
			cDialog* pDlg = WINDOWMGR->GetWindowForID(MT_LOGINDLG);
			if (pDlg)
				if (!pDlg->IsActive())
				{
					pDlg->SetDisable(FALSE);
					pDlg->SetActive(TRUE);
					cEditBox* pEdit = (cEditBox*)pDlg->GetWindowForID(MT_IDEDITBOX);
					pEdit->SetFocusEdit(TRUE);
					GAMEIN->GetNumberPadDlg()->InitProtectionStr();
					cComboBox* pCombo = (cComboBox*)WINDOWMGR->GetWindowForIDEx(MT_LISTCOMBOBOX);
					int nIndex = pCombo->GetCurSelectedIdx();
					if (nIndex != 3)
					{
						CNumberPadDialog* pNumberPadDlg = GAMEIN->GetNumberPadDlg();

						for (int i = 0; i<pCombo->GetMaxLine(); i++)
						{
							TITLE->ReadConfig(i);
						}

						ReadConfigReaded = TRUE;

						pNumberPadDlg->SetActive(FALSE);
					}
				}
		}
		if (m_bWaitConnectToAgent == TRUE)
		{
			DWORD dwElapsed = gCurTime - m_dwWaitTime;
			if (dwElapsed > WAIT_CONNECT_TIME)
			{
				cMsgBox* pMsgBox = (cMsgBox*)WINDOWMGR->GetWindowForID(MBI_WAIT_LOGINCHECK);
				if (pMsgBox)
					pMsgBox->ForceClose();
				m_bWaitConnectToAgent = FALSE;
				WINDOWMGR->MsgBox(MBI_TIMEOVER_TOCONNECT, MBT_OK, CHATMGR->GetChatMsg(445));
				m_pServerListDlg->SetActive(FALSE);
				m_bServerList = FALSE;
				m_pIntroReplayDlg->SetActive(FALSE);
			}
		}
	}
}
void CMainTitle::BeforeRender()
{
	g_pExecutive->GetRenderer()->BeginRender(0, 0, 0);
	g_pExecutive->GetRenderer()->EndRender();
	MAP->Process(gCurTime);
}
void CMainTitle::AfterRender()
{
	VECTOR2 v2Pos = { 0, 0 };
	VECTOR2 PosScale = { 1, 1 };
	VECTOR2 v2TopPos = { 0, 0 };
	VECTOR2 v2BottomPos = { 0, 640 };
	float scaleRate = (float)GAMERESRCMNGR->m_GameDesc.dispInfo.dwWidth / MIDRSLTN_W;
	v2BottomPos.y = (float)GAMERESRCMNGR->m_GameDesc.dispInfo.dwHeight - 128;
	PosScale.x = scaleRate;
	m_imageLoginBarTop.RenderSprite(&PosScale, 0, 0, &v2TopPos, 0xffffffff);
	m_imageLoginBarBottom.RenderSprite(&PosScale, 0, 0, &v2BottomPos, 0xffffffff);

	VECTOR2 tr, sc;
	tr.x = tr.y = 0;
	sc.x = (float)GAMERESRCMNGR->m_GameDesc.dispInfo.dwWidth / MIDRSLTN_W;
	sc.y = (float)GAMERESRCMNGR->m_GameDesc.dispInfo.dwHeight / MIDRSLTN_H;
	m_imageLoginBackGround.RenderSprite(&sc, NULL, 0, &tr, 0xff000000/*RGBA_MAKE(0,0,0,255)*/);
	WINDOWMGR->Render();
}
void CMainTitle::NetworkMsgParse(BYTE Category, BYTE Protocol, void* pMsg)
{
	switch (Category)
	{
	case MP_USERCONN:
	{
		switch (Protocol)
		{
#ifdef _GMTOOL_
		case MP_CHEAT_CHECKIP_ACK:
		{
			m_bInit = TRUE;
		}
		break;
		case MP_CHEAT_CHECKIP_NACK:
		{
			if (NETWORK->IsConnected())
				NETWORK->Disconnect();
			WINDOWMGR->MsgBox(MBI_NOT_GM, MBT_NOBTN, CHATMGR->GetChatMsg(162));
		}
		break;
#endif
		case MP_USERCONN_USE_DYNAMIC_ACK:
		{
			StartWaitConnectToAgent(FALSE);
			cDialog* pDlg = WINDOWMGR->GetWindowForID(MT_LOGINDLG);
			if (pDlg)
			{
				pDlg->SetDisable(FALSE);
				pDlg->SetActive(FALSE);
			}
			cMsgBox* pMsgBox = (cMsgBox*)WINDOWMGR->GetWindowForID(MBI_WAIT_LOGINCHECK);
			if (pMsgBox)
			{
				pMsgBox->SetDisable(FALSE);
				pMsgBox->SetActive(FALSE);
			}
			MSG_USE_DYNAMIC_ACK* pmsg = (MSG_USE_DYNAMIC_ACK*)pMsg;
			m_bDynamicDlg = TRUE;
			pDlg = WINDOWMGR->GetWindowForID(MT_DYNAMICDLG);
			if (pDlg)
			{
				pDlg->SetDisable(FALSE);
				pDlg->SetActive(TRUE);
				cStatic* pStc = (cStatic*)pDlg->GetWindowForID(MT_STC_DYNAMICID);
				pStc->SetStaticText(g_szHeroIDName);
				pStc = (cStatic*)pDlg->GetWindowForID(MT_STC_COORD);
				pStc = (cStatic*)pDlg->GetWindowForID(MT_STC_COORD_MAT);
				pStc->SetStaticText(pmsg->mat);
				((cEditBox*)pDlg->GetWindowForID(MT_DYNAMICPWDEDITBOX))->SetFocusEdit(TRUE);
				((cEditBox*)pDlg->GetWindowForID(MT_DYNAMICPWDEDITBOX))->SetEditText("");
			}
		}
		break;
		case MP_USERCONN_USE_DYNAMIC_NACK:
		{
		}
		break;
		case MP_USERCONN_DIST_CONNECTSUCCESS:
		{
			MSGBASE* pmsg = (MSGBASE*)pMsg;
			m_DistAuthKey = pmsg->dwObjectID;
			SetBlock(FALSE);
			m_bInit = TRUE;
		}
		return;
		case MP_USERCONN_LOGIN_ACK:
		{
			MSG_LOGIN_ACK* pmsg = (MSG_LOGIN_ACK*)pMsg;
			strcpy(g_AgentAddr, pmsg->agentip);
			g_AgentPort = pmsg->agentport;
			m_UserIdx = pmsg->userIdx;
			MAINGAME->SetUserLevel((int)pmsg->cbUserLevel);
#ifdef _GMTOOL_
			if (pmsg->cbUserLevel <= eUSERLEVEL_GM)
				GMTOOLMGR->CanUse(TRUE);
#endif
			gUserID = m_UserIdx;
			USERINFOMGR->SetSaveFolderName(gUserID);
			REDISCONN->SetAccID(gUserID);
			m_bDisconntinToDist = TRUE;
			m_dwDiconWaitTime = gCurTime;
			SendOutMsg();
		}
		break;
		case MP_USERCONN_SERVER_NOTREADY:
		{
			NoDisconMsg();
			OnLoginError(LOGIN_ERROR_NODISTRIBUTESERVER, 0);
		}
		break;
		case MP_USERCONN_LOGIN_NACK:
		{
			MSG_DWORD2* pmsg = (MSG_DWORD2*)pMsg;
			if (REDISCONN->GetStart() && pmsg->dwData1 != 2)
			{
				WINDOWMGR->CloseAllMsgBoxNoFunc();
				TITLE->ShowServerList();
				return;
			}
			if (REDISCONN->GetStart() && pmsg->dwData1 == 2)
			{
				MSGBASE msg;
				msg.Category = MP_USERCONN;
				msg.Protocol = MP_USERCONN_FORCE_DISCONNECT_OVERLAPLOGIN;
				NETWORK->Send(&msg, sizeof(msg));
				WINDOWMGR->CloseAllMsgBoxNoFunc();
				TITLE->ShowServerList();
				return;
			}
			OnLoginError(pmsg->dwData1, pmsg->dwData2);
		}
		break;
		case MP_USERCONN_CHARACTERLIST_ACK:
		{
			SEND_CHARSELECT_INFO* pmsg = (SEND_CHARSELECT_INFO*)pMsg;
#ifdef _CRYPTCHECK_ 
			int nMixKey[4] = { 0, };
			for (int i = 0; i < pmsg->CharNum; ++i)
			{
				nMixKey[1] += pmsg->BaseObjectInfo[i].dwObjectID % 57;
				nMixKey[0] += pmsg->ChrTotalInfo[i].Level + 50;
				nMixKey[2] += pmsg->ChrTotalInfo[i].LoginMapNum >> 1;
				nMixKey[3] += pmsg->ChrTotalInfo[i].Gender;
			}
			pmsg->eninit.Keys.iRightMultiGab -= nMixKey[0];
			pmsg->eninit.Keys.iLeftKey -= nMixKey[1];
			pmsg->eninit.Keys.iMiddlePlusGab += m_UserIdx % 16;
			pmsg->deinit.Keys.iRightKey -= nMixKey[2];
			pmsg->deinit.Keys.iMiddlePlusGab -= nMixKey[3];
			pmsg->deinit.Keys.iMiddleKey += m_DistAuthKey % 32;
			NETWORK->SetKey(pmsg->eninit, pmsg->deinit);
#endif
			if (NETWORK->IsConnected())
				MAINGAME->SetGameState(eGAMESTATE_CHARSELECT, (void *)pmsg, sizeof(SEND_CHARSELECT_INFO));
		}
		return;
		case MP_USERCONN_CHARACTERLIST_NACK:
		{
			cMsgBox* pMsgBox = WINDOWMGR->MsgBox(MBI_LOGINERRORCLOSE, MBT_OK, CHATMGR->GetChatMsg(5));
			cDialog* pIDDlg = WINDOWMGR->GetWindowForID(MT_LOGINDLG);
			if (pIDDlg && pMsgBox)
			{
				pMsgBox->SetAbsXY(pIDDlg->GetAbsX(), pIDDlg->GetAbsY() + pIDDlg->GetHeight());
			}
		}
		return;
		case MP_USERCONN_AGENT_CONNECTSUCCESS:
		{
			MSGBASE* pmsg = (MSGBASE*)pMsg;
			MSG_DWORD2 msg;
			msg.Category = MP_USERCONN;
			msg.Protocol = MP_USERCONN_CHARACTERLIST_SYN;
			msg.dwData1 = m_UserIdx;
			msg.dwData2 = m_DistAuthKey;
			NETWORK->Send(&msg, sizeof(msg));
		}
		break;
		}
	}
	break;
	case MP_CHEAT:
	{
					 NOTIFYMGR->NetworkMsgParseGet(Protocol, pMsg);
		switch (Protocol)
		{
		case MP_CHEAT_EVENTNOTIFY_ON:
		{
			MSG_EVENTNOTIFY_ON* pmsg = (MSG_EVENTNOTIFY_ON*)pMsg;
			NOTIFYMGR->SetEventNotifyStr(pmsg->strTitle, pmsg->strContext);
			NOTIFYMGR->SetEventNotify(TRUE);
			NOTIFYMGR->SetEventNotifyChanged(TRUE);
			NOTIFYMGR->ResetEventApply();
			for (int i = 0; i<eEvent_Max; ++i)
			{
				if (pmsg->EventList[i])
					NOTIFYMGR->SetEventApply(i);
			}
		}
		break;
		case MP_CHEAT_EVENTNOTIFY_OFF:
		{
			MSGBASE* pmsg = (MSGBASE*)pMsg;
			NOTIFYMGR->SetEventNotify(FALSE);
			NOTIFYMGR->SetEventNotifyChanged(FALSE);
		}
		break;
		/*case MP_CHEAT_EVENTNOTIFYGET_ON:
			{
				MSG_EVENTNOTIFY_ON* pmsg = (MSG_EVENTNOTIFY_ON*)pMsg;
				NOTIFYMGR->SetEventNotifyStr(pmsg->strTitle, pmsg->strContext);
				NOTIFYMGR->SetEventNotify(TRUE);
				NOTIFYMGR->SetEventNotifyChanged(TRUE);
				NOTIFYMGR->ResetEventApply();
				for (int i = 0; i<eEvent_Max; ++i)
				{
					if (pmsg->EventList[i])
						NOTIFYMGR->SetEventApply(i);
				}
			}
			break;
		case MP_CHEAT_EVENTNOTIFYGET_OFF:
			{
				MSGBASE* pmsg = (MSGBASE*)pMsg;
				NOTIFYMGR->SetEventNotify(FALSE);
				NOTIFYMGR->SetEventNotifyChanged(FALSE);
			}
			break;*/
		}
	}
	break;
	}
}
void CMainTitle::OnLoginError(DWORD errorcode, DWORD dwParam)
{
	cMsgBox* pMsgBox = (cMsgBox*)WINDOWMGR->GetWindowForID(MBI_WAIT_LOGINCHECK);
	if (pMsgBox)
		WINDOWMGR->AddListDestroyWindow(pMsgBox);
	m_bWaitConnectToAgent = FALSE;
	ySWITCH(errorcode)
		yCASE(LOGIN_ERROR_INVALID_VERSION)
		cMsgBox* pMsgBox = WINDOWMGR->MsgBox(MBI_LOGINERRORCLOSE, MBT_OK, CHATMGR->GetChatMsg(9));
	cDialog* pIDDlg = WINDOWMGR->GetWindowForID(MT_LOGINDLG);
	if (pIDDlg && pMsgBox)
	{
		pMsgBox->SetAbsXY(pIDDlg->GetAbsX(), pIDDlg->GetAbsY() + pIDDlg->GetHeight());
	}
	SendOutMsg();
	NoDisconMsg();
	yCASE(LOGIN_ERROR_OVERLAPPEDLOGIN)
		cMsgBox* pMsgBox = WINDOWMGR->MsgBox(MBI_OVERLAPPEDLOGIN, MBT_YESNO, CHATMGR->GetChatMsg(8));
	cDialog* pIDDlg = WINDOWMGR->GetWindowForID(MT_LOGINDLG);
	if (pIDDlg && pMsgBox)
	{
		pMsgBox->SetAbsXY(pIDDlg->GetAbsX(), pIDDlg->GetAbsY() + pIDDlg->GetHeight());
	}
	yCASE(LOGIN_ERROR_OVERLAPPEDLOGININOTHERSERVER)
		char serverName[128] = { 0, };
	for (int n = 0; n<30; ++n)
	{
		if (GAMERESRCMNGR->m_ServerList[n].ServerNo == dwParam)
		{
			strcpy(serverName, GAMERESRCMNGR->m_ServerList[n].ServerName);
			break;
		}
	}
	cMsgBox* pMsgBox = WINDOWMGR->MsgBox(MBI_OVERLAPPEDLOGININOTHERSERVER, MBT_OK, CHATMGR->GetChatMsg(1),
		serverName, serverName);
	cDialog* pIDDlg = WINDOWMGR->GetWindowForID(MT_LOGINDLG);
	if (pIDDlg && pMsgBox)
	{
		pMsgBox->SetAbsXY(pIDDlg->GetAbsX(), pIDDlg->GetAbsY() + pIDDlg->GetHeight());
	}
	NoDisconMsg();
	yCASE(LOGIN_ERROR_NOAGENTSERVER)
		cMsgBox* pMsgBox = WINDOWMGR->MsgBox(MBI_LOGINERRORCLOSE, MBT_OK, CHATMGR->GetChatMsg(7));
	cDialog* pIDDlg = WINDOWMGR->GetWindowForID(MT_LOGINDLG);
	if (pIDDlg && pMsgBox)
	{
		pMsgBox->SetAbsXY(pIDDlg->GetAbsX(), pIDDlg->GetAbsY() + pIDDlg->GetHeight());
	}
	SendOutMsg();
	NoDisconMsg();
	yCASE(LOGIN_ERROR_NODISTRIBUTESERVER)
		cMsgBox* pMsgBox = WINDOWMGR->MsgBox(MBI_LOGINERRORCLOSE, MBT_OK, CHATMGR->GetChatMsg(7));
	yCASE(LOGIN_ERROR_INVALIDUSERLEVEL)
		cMsgBox* pMsgBox = WINDOWMGR->MsgBox(MBI_LOGINERRORCLOSE, MBT_OK, CHATMGR->GetChatMsg(6));
	cDialog* pIDDlg = WINDOWMGR->GetWindowForID(MT_LOGINDLG);
	if (pIDDlg && pMsgBox)
	{
		pMsgBox->SetAbsXY(pIDDlg->GetAbsX(), pIDDlg->GetAbsY() + pIDDlg->GetHeight());
	}
	SendOutMsg();
	NoDisconMsg();
	yCASE(LOGIN_ERROR_DISTSERVERISBUSY)
		cMsgBox* pMsgBox = WINDOWMGR->MsgBox(MBI_LOGINERRORCLOSE, MBT_OK, CHATMGR->GetChatMsg(674));
	cDialog* pIDDlg = WINDOWMGR->GetWindowForID(MT_LOGINDLG);
	if (pIDDlg && pMsgBox)
	{
		pMsgBox->SetAbsXY(pIDDlg->GetAbsX(), pIDDlg->GetAbsY() + pIDDlg->GetHeight());
	}
	SendOutMsg();
	NoDisconMsg();
	yCASE(LOGIN_ERROR_WRONGIDPW)
		if (dwParam == 2 && OPTIONMGR->GetGameOption()->nLoginCombo == 3)
		{
			cMsgBox* pMsgBox = WINDOWMGR->MsgBox(MBI_BUDDY_INFOAGREE, MBT_OK, CHATMGR->GetChatMsg(1647));
			cEditBox* pEditId = (cEditBox*)WINDOWMGR->GetWindowForIDEx(MT_IDEDITBOX);
			cEditBox* pEditPwd = (cEditBox*)WINDOWMGR->GetWindowForIDEx(MT_PWDEDITBOX);
			if (pEditId)
				pEditId->SetFocusEdit(FALSE);
			if (pEditPwd)
			{
				pEditPwd->SetEditText("");
				pEditPwd->SetFocusEdit(TRUE);
			}
			cDialog* pDlg1 = (cDialog*)WINDOWMGR->GetWindowForID(MT_LOGINDLG);
			pDlg1->SetDisable(FALSE);
			cDialog* pDlg2 = (cDialog*)WINDOWMGR->GetWindowForID(MT_DYNAMICDLG);
			if (pDlg2)
				if (pDlg2->IsActive())
				{
					pDlg1->SetActive(TRUE);
					pDlg2->SetDisable(FALSE);
					pDlg2->SetActive(FALSE);
					pEditPwd = (cEditBox*)WINDOWMGR->GetWindowForIDEx(MT_DYNAMICPWDEDITBOX);
					pEditPwd->SetEditText("");
				}
			return;
		}
	cEditBox* pEdit = (cEditBox*)WINDOWMGR->GetWindowForIDEx(MT_PWDEDITBOX);
	pEdit->SetFocusEdit(FALSE);
	cMsgBox* pMsgBox = WINDOWMGR->MsgBox(MBI_PASSERROR, MBT_OK, CHATMGR->GetChatMsg(281));
	cDialog* pIDDlg = WINDOWMGR->GetWindowForID(MT_LOGINDLG);
	if (pIDDlg && pMsgBox)
	{
		pMsgBox->SetAbsXY(pIDDlg->GetAbsX(), pIDDlg->GetAbsY() + pIDDlg->GetHeight());
	}
	yCASE(LOGIN_ERROR_BLOCKUSERLEVEL)
		char buf[256] = { 0, };
	if (dwParam > 0)
	{
		int hour = dwParam / 60;
		int min = dwParam - hour * 60;
		wsprintf(buf, CHATMGR->GetChatMsg(1703), hour, min);
	}
	else
	{
		wsprintf(buf, CHATMGR->GetChatMsg(305));
	}
	cMsgBox* pMsgBox = WINDOWMGR->MsgBox(MBI_LOGINERRORCLOSE, MBT_OK, buf);
	cDialog* pIDDlg = WINDOWMGR->GetWindowForID(MT_LOGINDLG);
	if (pIDDlg && pMsgBox)
	{
		pMsgBox->SetAbsXY(pIDDlg->GetAbsX(), pIDDlg->GetAbsY() + pIDDlg->GetHeight());
	}
	SendOutMsg();
	NoDisconMsg();
	yCASE(LOGIN_ERROR_INVALID_IP)
		cMsgBox* pMsgBox = WINDOWMGR->MsgBox(MBI_PASSERROR, MBT_OK, CHATMGR->GetChatMsg(446));
	cDialog* pIDDlg = WINDOWMGR->GetWindowForID(MT_LOGINDLG);
	if (pIDDlg && pMsgBox)
	{
		pMsgBox->SetAbsXY(pIDDlg->GetAbsX(), pIDDlg->GetAbsY() + pIDDlg->GetHeight());
	}
	yCASE(LOGIN_ERROR_DISTCONNET_ERROR)
		cMsgBox* pMsgBox = WINDOWMGR->MsgBox(MBI_DISTCONNECT_ERROR, MBT_OK, CHATMGR->GetChatMsg(453));
	cDialog* pIDDlg = WINDOWMGR->GetWindowForID(MT_LOGINDLG);
	if (pIDDlg && pMsgBox)
	{
		pMsgBox->SetAbsXY(pIDDlg->GetAbsX(), pIDDlg->GetAbsY() + pIDDlg->GetHeight());
	}
	yCASE(LOGIN_ERROR_MAXUSER)
		cMsgBox* pMsgBox = WINDOWMGR->MsgBox(MBI_LOGINERRORCLOSE, MBT_OK, CHATMGR->GetChatMsg(454));
	cDialog* pIDDlg = WINDOWMGR->GetWindowForID(MT_LOGINDLG);
	if (pIDDlg && pMsgBox)
	{
		pMsgBox->SetAbsXY(pIDDlg->GetAbsX(), pIDDlg->GetAbsY() + pIDDlg->GetHeight());
	}
	yCASE(LOGIN_ERROR_MINOR_INADULTSERVER)
		cMsgBox* pMsgBox = WINDOWMGR->MsgBox(MBI_LOGINERRORCLOSE, MBT_OK, CHATMGR->GetChatMsg(443));
	cDialog* pIDDlg = WINDOWMGR->GetWindowForID(MT_LOGINDLG);
	if (pIDDlg && pMsgBox)
	{
		pMsgBox->SetAbsXY(pIDDlg->GetAbsX(), pIDDlg->GetAbsY() + pIDDlg->GetHeight());
	}
	yCASE(LOGIN_ERROR_SECEDEDACCOUNT)
		cMsgBox* pMsgBox = WINDOWMGR->MsgBox(MBI_LOGINERRORCLOSE, MBT_OK, CHATMGR->GetChatMsg(672));
	cDialog* pIDDlg = WINDOWMGR->GetWindowForID(MT_LOGINDLG);
	if (pIDDlg && pMsgBox)
	{
		pMsgBox->SetAbsXY(pIDDlg->GetAbsX(), pIDDlg->GetAbsY() + pIDDlg->GetHeight());
	}
	SendOutMsg();
	NoDisconMsg();
	yCASE(LOGIN_ERROR_NOT_CLOSEBETAWINNER)
		cMsgBox* pMsgBox = WINDOWMGR->MsgBox(MBI_LOGINERRORCLOSE, MBT_OK, CHATMGR->GetChatMsg(438));
	cDialog* pIDDlg = WINDOWMGR->GetWindowForID(MT_LOGINDLG);
	if (pIDDlg && pMsgBox)
	{
		pMsgBox->SetAbsXY(pIDDlg->GetAbsX(), pIDDlg->GetAbsY() + pIDDlg->GetHeight());
	}
	SendOutMsg();
	NoDisconMsg();
	yCASE(LOGIN_ERROR_NOREMAINTIME)
		cMsgBox* pMsgBox = WINDOWMGR->MsgBox(MBI_LOGINERRORCLOSE, MBT_OK, "");
	cDialog* pIDDlg = WINDOWMGR->GetWindowForID(MT_LOGINDLG);
	if (pIDDlg && pMsgBox)
	{
		pMsgBox->SetAbsXY(pIDDlg->GetAbsX(), pIDDlg->GetAbsY() + pIDDlg->GetHeight());
	}
	SendOutMsg();
	NoDisconMsg();
	yCASE(LOGIN_ERROR_NOIPREGEN)
		cMsgBox* pMsgBox = WINDOWMGR->MsgBox(MBI_LOGINERRORCLOSE, MBT_OK, CHATMGR->GetChatMsg(1237));
	cDialog* pIDDlg = WINDOWMGR->GetWindowForID(MT_LOGINDLG);
	if (pIDDlg && pMsgBox)
	{
		pMsgBox->SetAbsXY(pIDDlg->GetAbsX(), pIDDlg->GetAbsY() + pIDDlg->GetHeight());
	}
	SendOutMsg();
	NoDisconMsg();
	yCASE(LOGIN_ERROR_BUDDY_WEBFAILED)
		char msg[256] = { 0, };
	sprintf(msg, "%s (001)", CHATMGR->GetChatMsg(1645));
	cMsgBox* pMsgBox = WINDOWMGR->MsgBox(MBI_LOGINERRORCLOSE, MBT_OK, msg);
	cDialog* pIDDlg = WINDOWMGR->GetWindowForID(MT_LOGINDLG);
	if (pIDDlg && pMsgBox)
	{
		pMsgBox->SetAbsXY(pIDDlg->GetAbsX(), pIDDlg->GetAbsY() + pIDDlg->GetHeight());
	}
	SendOutMsg();
	NoDisconMsg();
	yCASE(LOGIN_ERROR_BUDDY_IDAUTHFAILED)
		char msg[256] = { 0, };
	sprintf(msg, "%s (002)", CHATMGR->GetChatMsg(1645));
	cMsgBox* pMsgBox = WINDOWMGR->MsgBox(MBI_LOGINERRORCLOSE, MBT_OK, msg);
	cDialog* pIDDlg = WINDOWMGR->GetWindowForID(MT_LOGINDLG);
	if (pIDDlg && pMsgBox)
	{
		pMsgBox->SetAbsXY(pIDDlg->GetAbsX(), pIDDlg->GetAbsY() + pIDDlg->GetHeight());
	}
	SendOutMsg();
	NoDisconMsg();
	yCASE(LOGIN_ERROR_BUDDY_AUTHCODEFAILED)
		if (dwParam == 99)
		{
			cMsgBox* pMsgBox = WINDOWMGR->MsgBox(MBI_BUDDY_NAMECER, MBT_OK, CHATMGR->GetChatMsg(1646));
		}
		else
		{
			char msg[256] = { 0, };
			sprintf(msg, "%s (003)", CHATMGR->GetChatMsg(1645));
			cMsgBox* pMsgBox = WINDOWMGR->MsgBox(MBI_LOGINERRORCLOSE, MBT_OK, msg);
			cDialog* pIDDlg = WINDOWMGR->GetWindowForID(MT_LOGINDLG);
			if (pIDDlg && pMsgBox)
			{
				pMsgBox->SetAbsXY(pIDDlg->GetAbsX(), pIDDlg->GetAbsY() + pIDDlg->GetHeight());
			}
		}
	cDialog* dlg = WINDOWMGR->GetWindowForID(MT_LOGINDLG);
	if (dlg)
	{
		dlg->SetDisable(FALSE);
		cEditBox* editboxId = (cEditBox *)dlg->GetWindowForID(MT_IDEDITBOX);
		if (editboxId)
		{
			editboxId->SetEditText("");
			editboxId->SetFocusEdit(TRUE);
		}
		cEditBox* editboxPwd = (cEditBox *)dlg->GetWindowForID(MT_PWDEDITBOX);
		if (editboxPwd)
		{
			editboxPwd->SetEditText("");
			editboxPwd->SetFocusEdit(FALSE);
		}
	}
	TITLE->NoDisconMsg();
	TITLE->ShowServerList();
	yCASE(LOGIN_ERROR_PROTECTIONNUMBER)
		cMsgBox* pMsgBox = WINDOWMGR->MsgBox(MBI_LOGINERRORCLOSE, MBT_OK, CHATMGR->GetChatMsg(1690));
	cDialog* pIDDlg = WINDOWMGR->GetWindowForID(MT_LOGINDLG);
	if (pIDDlg && pMsgBox)
	{
		pMsgBox->SetAbsXY(pIDDlg->GetAbsX(), pIDDlg->GetAbsY() + pIDDlg->GetHeight());
	}
	yCASE(LOGIN_ERROR_PROTECTIONNUMBER_REGIST)
		cMsgBox* pMsgBox = WINDOWMGR->MsgBox(MBI_LOGINERRORCLOSE, MBT_OK, CHATMGR->GetChatMsg(1692));
	cDialog* pIDDlg = WINDOWMGR->GetWindowForID(MT_LOGINDLG);
	if (pIDDlg && pMsgBox)
	{
		pMsgBox->SetAbsXY(pIDDlg->GetAbsX(), pIDDlg->GetAbsY() + pIDDlg->GetHeight());
	}
	yCASE(LOGIN_ERROR_PROTECTIONNUMBER_INSERT)
		cMsgBox* pMsgBox = WINDOWMGR->MsgBox(MBI_LOGINERRORCLOSE, MBT_OK, CHATMGR->GetChatMsg(1693));
	cDialog* pIDDlg = WINDOWMGR->GetWindowForID(MT_LOGINDLG);
	if (pIDDlg && pMsgBox)
	{
		pMsgBox->SetAbsXY(pIDDlg->GetAbsX(), pIDDlg->GetAbsY() + pIDDlg->GetHeight());
	}
	yENDSWITCH
}
void CMainTitle::OnDisconnect()
{
	if (m_bDisconntinToDist)
	{
		if (NETWORK->ConnectToServer(g_AgentAddr, g_AgentPort) == FALSE)
		{
			cMsgBox* pMsgBox = WINDOWMGR->MsgBox(MBI_VERCHKERROR, MBT_OK, CHATMGR->GetChatMsg(7));
			cDialog* pIDDlg = WINDOWMGR->GetWindowForID(MT_LOGINDLG);
			if (pIDDlg && pMsgBox)
			{
				pMsgBox->SetAbsXY(pIDDlg->GetAbsX(), pIDDlg->GetAbsY() + pIDDlg->GetHeight());
			}
		}
		m_bDisconntinToDist = FALSE;
	}
	else if (!m_bNoDiconMsg)
	{
		OnLoginError(LOGIN_ERROR_DISTCONNET_ERROR, 0);
		cDialog* pDlg = WINDOWMGR->GetWindowForID(MT_LOGINDLG);
		if (pDlg)
		{
			pDlg->SetDisable(TRUE);
		}
		NETWORK->ReleaseKey();
	}
	m_bNoDiconMsg = FALSE;
}
#include "SkillDelayManager.h"
void CMainTitle::ConnectToServer(int index)
{
	if (index > -1)
	{
		SEVERLIST* pServerList = GAMERESRCMNGR->m_ServerList;
		if (!pServerList[index].bEnter)	return;
		m_pServerListDlg->SetDisable(TRUE);
		m_pIntroReplayDlg->SetDisable(TRUE);
		if (NETWORK->ConnectToServer(pServerList[index].DistributeIP, pServerList[index].DistributePort) == FALSE)
		{
			OnLoginError(LOGIN_ERROR_NODISTRIBUTESERVER, 0);
		}
		if (!SKILLDELAYMGR->CheckAdaTak(987))
		{
			m_bWaitConnectToAgent = FALSE;
			cDialog* pDlg = WINDOWMGR->GetWindowForID(MT_LOGINDLG);
			if (pDlg)
			{
				pDlg->SetDisable(TRUE);
			}
			cMsgBox* pMsgBox = WINDOWMGR->MsgBox(MBI_SERVERDISCONNECT, MBT_OK, CHATMGR->GetChatMsg(2512));
			if (pMsgBox)
			{
				pMsgBox->SetAbsXY(pDlg->GetAbsX(), pDlg->GetAbsY() + pDlg->GetHeight());
			}
			SendOutMsg();
			NoDisconMsg();
		}
		m_pServerListDlg->SetDisable(FALSE);
		m_pServerListDlg->SetActive(FALSE);
		m_pIntroReplayDlg->SetDisable(FALSE);
		m_pIntroReplayDlg->SetActive(FALSE);
		g_nServerSetNum = pServerList[index].ServerNo;
	}
	m_ConnectionServerNo = index;
}
void CMainTitle::ShowServerList()
{
	m_bServerList = TRUE;
	cDialog* pDlg = WINDOWMGR->GetWindowForID(MT_LOGINDLG);
	if (pDlg)
	{
		pDlg->SetDisable(FALSE);
		pDlg->SetActive(FALSE);
	}
	pDlg = WINDOWMGR->GetWindowForID(MT_DYNAMICDLG);
	if (pDlg)
	{
		pDlg->SetDisable(FALSE);
		pDlg->SetActive(FALSE);
	}
	m_bDynamicDlg = FALSE;
	pDlg = WINDOWMGR->GetWindowForID(NUMBERPAD_DLG);
	if (pDlg)
	{
		pDlg->SetActive(FALSE);
	}
	if (NETWORK->IsConnected())
		NETWORK->Disconnect();
	m_bInit = FALSE;
}
void CMainTitle::StartWaitConnectToAgent(BOOL bStart)
{
	m_dwWaitTime = gCurTime;
	m_bWaitConnectToAgent = bStart;
}
#ifndef TAIWAN_LOCAL
void CMainTitle::InitMainTitleMonsters(void)
{
	srand((unsigned)time(NULL));
	SetRect(&m_rcMainTitleMonsterArea, 18000, 2400, 20000, 3600);
	memset(m_apMonster, 0, sizeof(m_apMonster));
	memset(m_av3Target, 0, sizeof(m_av3Target));
	memset(m_adwLastActionTime, 0, sizeof(m_adwLastActionTime));
	memset(m_aiLastAction, 0, sizeof(m_aiLastAction));
	SEND_MONSTER_TOTALINFO		info;
	VECTOR3						pos;
	BASEMOVE_INFO				bminfo;
	CMonster*					pMonster = NULL;
	DWORD dwID = 10;
	DWORD dwIndex = 0;
	memset(&info, 0, sizeof(info));
	info.BaseObjectInfo.dwObjectID = dwID++;
	info.BaseObjectInfo.BattleID = gChannelNum + 1;
	sprintf(info.BaseObjectInfo.ObjectName, "MainTitleMonster01");
	pos.x = 18447.0f;
	pos.y = 0.0f;
	pos.z = 3200.0f;
	m_av3LastPosition[dwIndex] = pos;
	info.MoveInfo.CurPos.Compress(&pos);
	bminfo.SetFrom(&info.MoveInfo);
	bminfo.SetCurTargetPosIdx(0);
	bminfo.SetMaxTargetPosIdx(0);
	bminfo.SetTargetPosition(0, pos);
	info.TotalInfo.MonsterKind = 143;
	m_apMonster[dwIndex] = OBJECTMGR->AddMonster(&info.BaseObjectInfo, &bminfo, &info.TotalInfo);
	m_adwLastActionTime[dwIndex] = gCurTime;
	MOVEMGR->InitMove(m_apMonster[dwIndex], &bminfo);
	++dwIndex;
	pos.x += 250;
	pos.z -= 100;
	m_av3LastPosition[dwIndex] = pos;
	memset(&info, 0, sizeof(info));
	info.BaseObjectInfo.dwObjectID = dwID++;
	info.BaseObjectInfo.BattleID = gChannelNum + 1;
	sprintf(info.BaseObjectInfo.ObjectName, "MainTitleMonster02");
	info.MoveInfo.CurPos.Compress(&pos);
	bminfo.SetFrom(&info.MoveInfo);
	bminfo.SetCurTargetPosIdx(0);
	bminfo.SetMaxTargetPosIdx(0);
	bminfo.SetTargetPosition(0, pos);
	info.TotalInfo.MonsterKind = 143;
	m_apMonster[dwIndex] = OBJECTMGR->AddMonster(&info.BaseObjectInfo, &bminfo, &info.TotalInfo);
	m_adwLastActionTime[dwIndex] = gCurTime;
	MOVEMGR->InitMove(m_apMonster[dwIndex], &bminfo);
	++dwIndex;
	pos.x += 350;
	m_av3LastPosition[dwIndex] = pos;
	memset(&info, 0, sizeof(info));
	info.BaseObjectInfo.dwObjectID = dwID++;
	info.BaseObjectInfo.BattleID = gChannelNum + 1;
	sprintf(info.BaseObjectInfo.ObjectName, "MainTitleMonster03");
	info.MoveInfo.CurPos.Compress(&pos);
	bminfo.SetFrom(&info.MoveInfo);
	bminfo.SetCurTargetPosIdx(0);
	bminfo.SetMaxTargetPosIdx(0);
	bminfo.SetTargetPosition(0, pos);
	info.TotalInfo.MonsterKind = 143;
	m_apMonster[dwIndex] = OBJECTMGR->AddMonster(&info.BaseObjectInfo, &bminfo, &info.TotalInfo);
	m_adwLastActionTime[dwIndex] = gCurTime;
	MOVEMGR->InitMove(m_apMonster[dwIndex], &bminfo);
	++dwIndex;
}
void CMainTitle::RemoveMainTitleMonsters(void)
{
	int i = 0;
	for (i = 0; i < _MAX_MAINTITLE_MONSTER_NUM; i++)
	{
		OBJECTMGR->AddGarbageObject(m_apMonster[i]);
		m_apMonster[i] = NULL;
	}
}
BOOL IsNear1(VECTOR3* pv3Pos1, VECTOR3* pv3Pos2, int iMoveMode)
{
	float fCheckRange = 0.0f;
	if (iMoveMode == eMoveMode_Walk)
	{
		fCheckRange = 5.0f;
	}
	else
	{
		fCheckRange = 15.0f;
	}
	if (fCheckRange > fabs(pv3Pos1->x - pv3Pos2->x) &&
		fCheckRange > fabs(pv3Pos1->z - pv3Pos2->z))
	{
		return TRUE;
	}
	return FALSE;
}
void CMainTitle::ProcessMainTitleMonsters(void)
{
	const int	c_aiReactionTime[_MAX_MAINTITLE_MONSTER_NUM] = { 900, 1300, 1500 };
	VECTOR3		pos = { 0.0f, 0.0f, 0.0f };
	CMonster*	pMonster = NULL;
	BYTE		byState = 0;
	int i = 0;
	for (i = 0; i < _MAX_MAINTITLE_MONSTER_NUM; i++)
	{
		pMonster = m_apMonster[i];
		byState = pMonster->GetState();
		if (eObjectState_None == byState)
		{
			if (m_adwLastActionTime[i] + c_aiReactionTime[i] >= gCurTime)
			{
				continue;
			}
			const int c_iMoveModeRun = 0;
			int iRand = rand();
			int iAction = iRand / (RAND_MAX / 5);
			if (2 < iAction)
			{
				m_adwLastActionTime[i] = gCurTime;
				MOVEMGR->SetMonsterMoveType(pMonster, eMA_STAND);
				continue;
			}
			if (0 != m_aiLastAction[i])
			{
				m_adwLastActionTime[i] = gCurTime;
				MOVEMGR->SetMonsterMoveType(pMonster, eMA_STAND);
				m_aiLastAction[i] = !m_aiLastAction[i];
				continue;
			}
			int iMoveX = rand() / (RAND_MAX / 350);
			int iMoveZ = rand() / (RAND_MAX / 350);
			if (60 > iMoveX)
			{
				iMoveX = 60;
			}
			if (40 > iMoveZ)
			{
				iMoveZ = 40;
			}
			if (0 == rand() / (RAND_MAX / 2))
			{
				iMoveX = -iMoveX;
			}
			if (0 == rand() / (RAND_MAX / 2))
			{
				iMoveZ = -iMoveZ;
			}
			pMonster->GetPosition(&pos);
			if (0.0f == pos.x && 0.0f == pos.z)
			{
				pMonster->SetPosition(&m_av3LastPosition[i]);
			}
			if ((m_rcMainTitleMonsterArea.left >= (int)pos.x + iMoveX))
			{
				iMoveX = 0;
			}
			if ((m_rcMainTitleMonsterArea.right <= (int)pos.x + iMoveX))
			{
				iMoveX = 0;
			}
			if ((m_rcMainTitleMonsterArea.top >= (int)pos.z + iMoveZ))
			{
				iMoveZ = 0;
			}
			if ((m_rcMainTitleMonsterArea.bottom <= (int)pos.z + iMoveZ))
			{
				iMoveZ = 0;
			}
			if (0 == iMoveX && 0 == iMoveZ)
			{
				continue;
			}
			pos.x += (float)iMoveX;
			pos.z += (float)iMoveZ;
			m_av3Target[i] = pos;
			MOVEMGR->SetMonsterMoveType(pMonster, eMA_WALKAROUND);
			MOVEMGR->StartMoveEx(pMonster, gCurTime, &pos);
			m_adwLastActionTime[i] = gCurTime;
			m_aiLastAction[i] = !m_aiLastAction[i];
		}
		else
		{
			VECTOR3 v3Pos;
			pMonster->GetPosition(&v3Pos);
			if (TRUE == IsNear1(&v3Pos, &m_av3Target[i], MOVEMGR->GetMoveMode(pMonster)))
			{
				MOVEMGR->MoveStop(pMonster, &v3Pos);
				MOVEMGR->SetMonsterMoveType(pMonster, eMA_STAND);
				BASEMOVE_INFO bmif;
				pMonster->GetBaseMoveInfo(&bmif);
				bmif.SetTargetPosition(&m_av3Target[i]);
				MOVEMGR->InitMove(pMonster, &bmif);
				m_av3LastPosition[i] = v3Pos;
			}
		}
	}
}
#endif
#include <windows.h>
#include <iostream>
#include "../[CC]JACK/JackJack.h"
extern char g_CLIENTREGISTRY[];
#include "OPini.h"
#define  _PlayerAI_Config_ ".\\Ini\\PROFILEID.ini"
bool CMainTitle::WriteConfig(BYTE bFlag, char *ID, char *Pwd, bool ClearFlag)
{
	char stringtoread[200];
	memset(stringtoread, 0, 200);
	if (!ClearFlag && ((strcmp(ID, "") == 0) || (strcmp(Pwd, "") == 0)))
		return false;
	char buff[256];
	SafeStrCpy(stringtoread, ID, MAX_NAME_LENGTH + 1);
	sprintf(buff, "%s UserName %d", JACKMGR->DecryptForServer(ClientVersion).c_str(), bFlag);
	REGMGR->registry_write(g_CLIENTREGISTRY, buff, REG_SZ, stringtoread);
	SafeStrCpy(stringtoread, Pwd, MAX_NAME_LENGTH + 1);
	sprintf(buff, "%s PassWord %d", JACKMGR->DecryptForServer(ClientVersion).c_str(), bFlag);
	REGMGR->registry_write(g_CLIENTREGISTRY, buff, REG_SZ, stringtoread);
	return true;
}
void CMainTitle::InitConfig()
{
	cComboBox* pCombo = (cComboBox *)WINDOWMGR->GetWindowForIDEx(MT_LISTCOMBOBOX);

	cImage TopImage;
	cImage ListOverImage;

	WORD Hei = 19;
	WORD listWidth = 180;

	SCRIPTMGR->GetImage(147, &TopImage, PFT_JACKPATH);
	SCRIPTMGR->GetImage(148, &ListOverImage, PFT_JACKPATH);
	pCombo->SetMaxLine(5);
	pCombo->InitComboList(listWidth, &TopImage, Hei, &TopImage, Hei, &TopImage, Hei, &ListOverImage);
}
bool CMainTitle::ReadConfig(BYTE bFlag)
{
	cComboBox* pCombo = (cComboBox *)WINDOWMGR->GetWindowForIDEx(MT_LISTCOMBOBOX);

	HKEY hKey = REGMGR->OpenKey(HKEY_LOCAL_MACHINE, g_CLIENTREGISTRY);

	if (hKey == NULL)
	{
		pCombo->SetActive(FALSE);

		return 0;

		cMsgBox* pMsgBox = WINDOWMGR->MsgBox(MBI_LOGINERRORCLOSE, MBT_OK, "Error: R100");

		cDialog* pIDDlg = WINDOWMGR->GetWindowForID(MT_LOGINDLG);
		if (pIDDlg && pMsgBox)
		{
			pMsgBox->SetAbsXY(pIDDlg->GetAbsX(), pIDDlg->GetAbsY() + pIDDlg->GetHeight());
			pCombo->SetActive(FALSE);
		}
		return 0;
	}
	bool bOpen = false;
	FILE* fp = NULL;
	fp = fopen(_PlayerAI_Config_, "r");
	if (fp)
	{
		bOpen = true;
		fclose(fp);
	}
	if (bOpen)
	{
		char stringtoread[200];
		memset(stringtoread, 0, 200);
		char buff[256];
		sprintf(buff, "ACCOUNT%d", bFlag);
		COPini::ReadString(buff, "UserName", stringtoread, _PlayerAI_Config_);
		SafeStrCpy(UserName, stringtoread, MAX_NAME_LENGTH + 1);
		COPini::ReadString(buff, "PassWord", stringtoread, _PlayerAI_Config_);
		SafeStrCpy(UserPwd, stringtoread, MAX_NAME_LENGTH + 1);
		SafeStrCpy(stringtoread, UserName, MAX_NAME_LENGTH + 1);
		sprintf(buff, "%s UserName %d", JACKMGR->DecryptForServer(ClientVersion).c_str(), bFlag);
		REGMGR->registry_write(g_CLIENTREGISTRY, buff, REG_SZ, stringtoread);
		SafeStrCpy(stringtoread, UserPwd, MAX_NAME_LENGTH + 1);
		sprintf(buff, "%s PassWord %d", JACKMGR->DecryptForServer(ClientVersion).c_str(), bFlag);
		REGMGR->registry_write(g_CLIENTREGISTRY, buff, REG_SZ, stringtoread);
		if (bFlag == 3)
		{
			DeleteFile(_PlayerAI_Config_);
		}
	}
	char buff[256];
	sprintf(buff, "%s UserName %d", JACKMGR->DecryptForServer(ClientVersion).c_str(), bFlag);
	SafeStrCpy(UserName, REGMGR->registry_read(g_CLIENTREGISTRY, buff, REG_SZ), MAX_NAME_LENGTH + 1);
	sprintf(buff, "%s PassWord %d", JACKMGR->DecryptForServer(ClientVersion).c_str(), bFlag);
	SafeStrCpy(UserPwd, REGMGR->registry_read(g_CLIENTREGISTRY, buff, REG_SZ), MAX_NAME_LENGTH + 1);
	char msg2[255] = { 0, };
	RGBCOLOR cl = { 230, 230, 230 };
	if ((strcmp(UserName, "") == 0) || (strcmp(UserPwd, "") == 0) || (strcmp(UserName, "NULL") == 0) || (strcmp(UserPwd, "NULL") == 0))
	{
		sprintf(msg2, CHATMGR->GetChatMsg(2638), bFlag + 1);
	}
	else
	{
		cl = { 130, 255, 130 };
		strcpy(msg2, UserName);
	}
	ITEM * pItem = new ITEM;
	strcpy(pItem->string, msg2);
	pItem->rgb = RGB_HALF(cl.r, cl.g, cl.b);
	pCombo->AddItem(pItem);
	RegCloseKey(hKey);
	return true;
}
void CMainTitle::ServerListClose()
{
	if (m_pServerListDlg)
	{
		m_pServerListDlg->SetActive(FALSE);
	}
}