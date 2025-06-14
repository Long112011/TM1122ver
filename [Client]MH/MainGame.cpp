#include "stdafx.h"
#include "MainGame.h"
#include "MainTitle.h"
#include "Intro.h"	
#include "ServerConnect.h"
#include "CharSelect.h"
#include "CharMake.h"
#include "GameLoading.h"
#include "GameIn.h"
#include "MapChange.h"
#include "MurimNet.h"
#include "MHTimeManager.h"
#include "mhnetwork.h"
#include "./Engine/Engine.h"
#include "./Interface/cResourceManager.h"
#include "./Interface/cWindowManager.h"
#include "../[CC]Header/GameResourceManager.h"
#include "AppearanceManager.h"
#include "ObjectManager.h"
#include "OptionManager.h"
#include "StringLoader.h"
#include "MovePoint.h"
#include "./Input/cIMEWnd.h"
#include "./Audio/MHAudioManager.h"
#include "mhMap.h"
#include "./Input/UserInput.h"
#include "CQuestManager.h"
#include "MacroManager.h"
#include "..\[CC]MixManager\MixManager.h"
#include "NpcScriptManager.h"
#include "HelpDicManager.h"
#include "AbilityManager.h"
#include "CheatMsgparser.h"
#include "MouseCursor.h"
#include "ChatManager.h"
#include "cChatTipManager.h"
#include "ReinforceManager.h"
#ifdef _GMTOOL_
#include "GMToolManager.h"
#endif
#include "GuildManager.h"
#include "SkillDelayManager.h"
#ifdef _DEBUG
#include "MiniDumper.h"
#endif 
#ifdef TAIWAN_LOCAL
#include "BillingManager.h"
#endif
#ifndef _RESOURCE_WORK_NOT_APPLY_
#include "./Engine/EngineObject.h"
#include "./Engine/EngineObjectCache.h"
#endif
#include "cScriptManager.h"
#include "VideoCaptureManager.h"
#include "Reconnect.h"
#include "../[CC]JACK/JackJack.h"
#include "GameCheckManager.h"
#include "ItemDropManager.h"
#include "RegistryManager.h"
#define MAX_PROFILE_NUM 6
GLOBALTON(CMainGame);
CMainGame* pTempMainGame = NULL;
HWND gHwnd = NULL;
CMainGame::CMainGame()
{
	ASSERT(!pTempMainGame);
	m_bChangeState = FALSE;
	m_ChangeState = -1;
	m_ppGameState = NULL;
	m_pCurrentGameState = NULL;
	m_pEngine = NULL;
	m_bEndGame = FALSE;
	m_pStateInitParam = NULL;
	pTempMainGame = this;
	m_nUserLevel = eUSERLEVEL_USER;

	srand(GetTickCount());
}
CMainGame::~CMainGame()
{
}
void CMainGame::Init(HWND hMainWnd)
{
	//srand(GetTickCount());
	

	m_pEngine = new CEngine;
	m_pEngine->Init(hMainWnd, &GAMERESRCMNGR->m_GameDesc.dispInfo,
		GAMERESRCMNGR->m_GameDesc.MaxShadowNum, GAMERESRCMNGR->m_GameDesc.MaxShadowTexDetail, 0,
		GAMERESRCMNGR->m_GameDesc.FramePerSec);

	/*MAINGAME->LoadPack(
		"character.pak");
	MAINGAME->LoadPack(
		"effect.pak");
	MAINGAME->LoadPack(
		"monster.pak");
	MAINGAME->LoadPack(
		"npc.pak");
	MAINGAME->LoadPack(
		"PET.pak");
	MAINGAME->LoadPack(
		"jack.pak");
	MAINGAME->LoadPack(
		"titan.pak");*/

	DIRECTORYMGR->Init();
	DIRECTORYMGR->SetLoadMode(eLM_EffectScript);
	LoadEffectFileTable("list_m.befl");
	LoadEffectFileTable("list_w.befl");
	DIRECTORYMGR->SetLoadMode(eLM_Root);
	
	RESRCMGR->Init(FILE_IMAGE_PATH, FILE_IMAGE_MSG);
	SCRIPTMGR->InitParseTypeData();
	NETWORK->Init(hMainWnd);
	WINDOWMGR->Init();
	gHwnd = hMainWnd;
	OPTIONMGR->Init();
	OPTIONMGR->ApplySettings();
	MACROMGR->Init();
	MIXMGR->LoadMixList();
	REINFORCEMGR->LoadReinforceItemInfo();
	REINFORCEMGR->LoadRareReinnforceItemInfo();
	NPCSCRIPTMGR->Init();
	HELPDICMGR->Init();
	AUDIOMGR->Initialize(hMainWnd);
	MOVEPOINT->Init();
	g_UserInput.Init();
	GUILDMGR->Init();
	CHATTIPMGR->Init();
	SKILLDELAYMGR->LoadSkillUseInfo();
	ITEMDROPMGR->Init();
#ifndef _RESOURCE_WORK_NOT_APPLY_
	g_pEngineObjectCache = new CEngineObjectCache;
	g_pEngineObjectCache->Init(g_pExecutive, CEngineObjectCache::MAX_ENGINE_OBJECT_CACHE_SIZE);
	{
		VECTOR3 v3Eye = { 1000, 0, 0 };
		VECTOR3 v3Rot = { 0, 0, 0 };
		g_pExecutive->GetGeometry()->SetCameraPos(&v3Eye, 0);
		g_pExecutive->GetGeometry()->SetCameraAngleRad(&v3Rot, 0);
	}
#endif 
}
void CMainGame::Release()
{
	WINDOWMGR->SetDragEnd();
	MOVEPOINT->Release();
	if (m_pCurrentGameState)
	{
		m_pCurrentGameState->Release(NULL);
		m_pCurrentGameState = NULL;
	}
	CHATTIPMGR->Release();
	EFFECTMGR->Release();
	WINDOWMGR->Release();
	RESRCMGR->Release();
	NETWORK->Release();
	SAFE_DELETE_ARRAY(m_pStateInitParam);
	AUDIOMGR->Terminate();
	g_UserInput.Release();
	CURSOR->Release();
	GUILDMGR->Release();

	GAMEIN->Release(NULL);

#ifndef _RESOURCE_WORK_NOT_APPLY_
	g_pEngineObjectCache->Release();
	delete g_pEngineObjectCache;
	g_pEngineObjectCache = NULL;
#endif
	if (m_pEngine)
	{
		m_pEngine->Release();
		delete m_pEngine;
		m_pEngine = NULL;
	}
}
void CMainGame::SetGameState(int StateNum, void* pStateInitParam, int ParamLen)
{
	m_bChangeState = TRUE;
	m_ChangeState = StateNum;
	if (m_ChangeState == eGAMESTATE_END)
	{
		m_bEndGame = TRUE;
#ifdef _GMTOOL_
		GMTOOLMGR->DestroyGMDialog();
#endif
	}
	if (pStateInitParam && ParamLen)
	{
		m_pStateInitParam = new char[ParamLen];
		memcpy(m_pStateInitParam, pStateInitParam, ParamLen);
	}
}
extern BOOL g_bActiveApp;
extern BOOL bAllowToShowNotify; 
extern BOOL g_bDisplayFPS;
extern double CurFPS;
void CMainGame::FPSRenderProcess()
{
	if (m_pCurrentGameState)
	{
		
		if (m_pCurrentGameState->IsBlock() == FALSE)
		{
			CurFPS = CMHTimeManager::Instance().CalculateFrameRate() /3-1;

			g_UserInput.Process();
			MHTIMEMGR->Process();
			
			WINDOWMGR->Process();
			m_pCurrentGameState->Process();
			CurFPS = CMHTimeManager::Instance().CalculateFrameRate()/2- 1;
			AUDIOMGR->Process();
			REDISCONN->Process();
			JACKMGR->Process();
			sGAMECHECK->Process();
			if (!bAllowToShowNotify)
				HERO->GamePad();

			CurFPS = CMHTimeManager::Instance().CalculateFrameRate()-1;
		}			
	}
}

int CMainGame::Process()
{
	if (g_bActiveApp == FALSE)
	{
		//CMHTimeManager::Instance().Sleep(10);
		Sleep(10);
	}
	//Sleep(1);
	//CurFPS = CMHTimeManager::Instance().CalculateFrameRate2();
	if (m_bEndGame)
	{
		if (m_pCurrentGameState)
		{
			m_pCurrentGameState->Release(GetGameState(m_ChangeState));
			m_pCurrentGameState->SetBlock(FALSE);
			m_pCurrentGameState = NULL;
		}
		Release();
		return -1;
	}
	BeforeRenderFunc();
	BOOL rt = m_pEngine->BeginProcess(NULL, AfterRenderFunc);
	//if (rt)
	//if (!g_bDisplayFPS)
	if (OPTIONMGR->GetGameOption()->bFPSChange)
	{
		FPSRenderProcess();
	}
	else
	{ 
		if (rt)	
		{
			FPSRenderProcess();
		}
	}
	if (m_pCurrentGameState == GAMEIN && GAMEIN->IsGameInAcked() == FALSE)
	{
		g_pExecutive->GetRenderer()->BeginRender(0, 0, 0);
		g_pExecutive->GetRenderer()->EndRender();
	}
	VIDEOCAPTUREMGR->Process();

	m_pEngine->EndProcess();

	if (m_bChangeState)
	{
		if (m_pCurrentGameState)
		{
			m_pCurrentGameState->Release(GetGameState(m_ChangeState));
			m_pCurrentGameState->SetBlock(FALSE);
			m_pCurrentGameState = NULL;//kiv
		}
		m_pCurrentGameState = GetGameState(m_ChangeState);
		if (!m_pCurrentGameState)
		{
			Release();
			return -1;
		}
		m_nCurStateNum = m_ChangeState;
		CIMEWND->SetDockingEx(NULL, TRUE);
	}

	APPEARANCEMGR->ProcessReservation();
	APPEARANCEMGR->ProcessAlphaAppearance();	
	APPEARANCEMGR->ProcessAppearance();	
	g_pEngineObjectCache->ProcessCacheDelete();

	OBJECTMGR->ProcessGarbage();

	

	if (m_bChangeState)
	{
		m_pCurrentGameState->SetBlock(FALSE);
		if (m_pCurrentGameState->Init(m_pStateInitParam) == FALSE)
		{
			if (m_nCurStateNum == eGAMESTATE_TITLE)
			{
				m_pCurrentGameState->SetBlock(FALSE);
				TITLE->OnLoginError(LOGIN_ERROR_NODISTRIBUTESERVER, 0);
			}
			/*else if (m_nCurStateNum == eGAMESTATE_INTRO)
			{
				MAINGAME->SetGameState(eGAMESTATE_CONNECT);
				return 0;
			}*/
			else
			{
				MessageBox(0, CHATMGR->GetChatMsg(444), 0, 0);
				SAFE_DELETE_ARRAY(m_pStateInitParam);
				Release();
				return -1;
			}
		}
		SAFE_DELETE_ARRAY(m_pStateInitParam);
		m_bChangeState = FALSE;
	}

	
	return 0;
}
void CMainGame::BeforeRender()
{
	if (m_pCurrentGameState)
		m_pCurrentGameState->BeforeRender();
}
void CMainGame::AfterRender()
{
	if (m_pCurrentGameState)
		m_pCurrentGameState->AfterRender();
}
void CMainGame::PauseRender(BOOL bPause)
{
	m_pEngine->PauseRender(bPause);
}
CGameState* CMainGame::GetGameState(int StateNum)
{
	CGameState* pState = NULL;
	switch (StateNum)
	{
	case eGAMESTATE_INTRO:
		pState = INTRO;
		break;
	case eGAMESTATE_CONNECT:
		pState = SERVERCONNECT;
		break;
	case eGAMESTATE_TITLE:
		pState = TITLE;
		break;
	case eGAMESTATE_CHARSELECT:
		pState = CHARSELECT;
		break;
	case eGAMESTATE_CHARMAKE:
		pState = CHARMAKE;
		break;
	case eGAMESTATE_GAMELOADING:
		pState = GAMELOADING;
		break;
	case eGAMESTATE_GAMEIN:
		pState = GAMEIN;
		break;
	case eGAMESTATE_MAPCHANGE:
		pState = MAPCHANGE;
		break;
	case eGAMESTATE_MURIMNET:
		pState = MURIMNET;
		break;
	case eGAMESTATE_END:
		pState = NULL;
		break;
	default:
		break;
	}
	return pState;
}
DWORD _stdcall BeforeRenderFunc()
{
	pTempMainGame->BeforeRender();
	return 0;
}
DWORD _stdcall AfterRenderFunc()
{
	pTempMainGame->AfterRender();
	return 0;
}
DWORD _stdcall AfterRenderFunc__()
{
	WINDOWMGR->Render();
	return 0;
}
/*DWORD __stdcall MapLoadCallback(DWORD dwCurCount, DWORD dwTotalCount, void* pArg)
{
	g_pExecutive->Run(0xff000000, NULL, AfterRenderFunc__, 0);
	g_pExecutive->GetGeometry()->Present(NULL);
	return 0;
}*/
DWORD __stdcall MapLoadCallback(DWORD dwCurCount, DWORD dwTotalCount, void* pArg)
{
	g_pExecutive->Run(0xff000000, NULL, AfterRenderFunc__, 0);
	g_pExecutive->GetGeometry()->Present(NULL);
	return 0;
}


void CMainGame::LoadPack(LPCTSTR fileName)
{
	GetEngine()->GetGraphicsEngine()->LoadPack(
		fileName);
}






#include "UserInfoManager.h"
void CMainGame::InitChangeResolution(HWND hMainWnd)
{
	//m_pEngine = new CEngine;
	//m_pEngine->Init(hMainWnd,&GAMERESRCMNGR->m_GameDesc.dispInfo,
	//	GAMERESRCMNGR->m_GameDesc.MaxShadowNum,GAMERESRCMNGR->m_GameDesc.MaxShadowTexDetail,0,
	//	GAMERESRCMNGR->m_GameDesc.FramePerSec);

	//WINDOWMGR->Release();

	//WINDOWMGR->SetDragEnd();
	//WINDOWMGR->DestroyWindowAll();
	//RESRCMGR->ReleaseResource(28);

	//WINDOWMGR->Release2();
	//WINDOWMGR->AfterInit();
	

	gHwnd = hMainWnd;

	g_pExecutive->GetGeometry()->UpdateWindowSize();


	//WINDOWMGR->CreateGameIn();
	//WINDOWMGR->AfterInit();

	//

	//WINDOWMGR->AfterInit();
	//

	//USERINFOMGR->LoadUserInfo(eUIK_INTERFACE | eUIK_RECONNECT);
}