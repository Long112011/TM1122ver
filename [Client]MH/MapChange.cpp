

#include "stdafx.h"
#include "MapChange.h"

#include "MainGame.h"
#include "./Interface/cWindowManager.h"
#include "./Interface/cResourceManager.h"
#include "cWindowSystemFunc.h"

#include "mhMap.h"
#include "./Input/Mouse.h"
#include "./Input/Keyboard.h"
#include "PathManager.h"
#include "LoadingDlg.h"
#include "./Interface/cScriptManager.h"

#include "./Input/UserInput.h"
#include "UserInfoManager.h"
#include "ChatManager.h"

#include "GMNotifyManager.h"
#include "GuildTournamentMgr.h"
#include "SiegeWarMgr.h"
#include "GameResourceManager.h"
#include "MHAudioManager.h"
#include "SurvivalModeManager.h"

#include "aimanager.h"
#include "AiSetManager.h"

GLOBALTON(CMapChange)
CMapChange::CMapChange()
{
	m_pDlg = NULL;
	m_pReDlg = NULL;
	m_GameInInitKind = eGameInInitKind_Login;
	m_bThreadTerminated = FALSE;
	m_bMapLoadComplete = FALSE;
	m_bFirst = FALSE;

	hThread = NULL;

	m_MapNum = 0;

	const VECTOR2 emptyVector = {0};
	mImageBlackScreenPosition = emptyVector;
}

CMapChange::~CMapChange()
{


}
#include "ReconnectDialog.h"
BOOL CMapChange::Init(void* pInitParam)
{
	AUDIOMGR->StopBGM();

	m_MapNum = *((MAPTYPE*)pInitParam);
	
	char filename[256];
	sprintf(filename,"Image/LoadingMap/maploadingimage%02d.tga",m_MapNum);
	if(m_LoadingImage.LoadSprite(filename) == FALSE)
	{
		sprintf(filename,"Image/LoadingMap/maploadingimage00.tga");
		m_LoadingImage.LoadSprite(filename);
	}
	m_pDlg = (CLoadingDlg *)CreateGameLoading_m();
	
	WINDOWMGR->AfterInit();

	m_bThreadTerminated=FALSE;
	m_bMapLoadComplete=FALSE;
	NETWORK->SetCurState(this);
	
	m_bFirst = TRUE;

	g_UserInput.SetInputFocus( FALSE );
	if(m_MapNum == 44 || m_MapNum == 28)
		USERINFOMGR->SetMapChage( FALSE );
	else
		USERINFOMGR->SetMapChage( TRUE );

	return TRUE;
}
void CMapChange::Release(CGameState* pNextGameState)
{
	WINDOWMGR->SetDragEnd();	
	WINDOWMGR->DestroyWindowAll();
	RESRCMGR->ReleaseResource(28);

	sCPlayerAI.ClearBeforeMapChange();

	m_LoadingImage.Release();
	m_pDlg = NULL;
	m_pReDlg = NULL;
}

void CMapChange::Process()
{
	EnterGame();
	if(m_bFirst)

		m_bFirst = FALSE;
	else
	{
		if(m_bThreadTerminated == FALSE)
		{			
			m_bThreadTerminated = TRUE;
		}
		if(m_bMapLoadComplete == FALSE)
		{
			MAP->InitMap(m_MapNum);
			PATHMGR->SetMap(MAP, m_MapNum, MAP->GetTileWidth());
			m_bMapLoadComplete = TRUE;


		}
	}
	
	return;
}
void CMapChange::BeforeRender()
{
}
void CMapChange::AfterRender()
{
	WINDOWMGR->Render();
	VECTOR2 sc,tr1,sc1;

	sc.x  = (float)GAMERESRCMNGR->m_GameDesc.dispInfo.dwWidth / MIDRSLTN_W;
	sc.y  = (float)GAMERESRCMNGR->m_GameDesc.dispInfo.dwHeight / MIDRSLTN_H;
	m_LoadingImage.RenderSprite(&sc,0,0,&mImageBlackScreenPosition,RGBA_MAKE(0,0,0,255));

	sc1.x  = sc1.y = 1.0f;
	tr1.x = ((float)GAMERESRCMNGR->m_GameDesc.dispInfo.dwWidth - MIDRSLTN_W * sc1.x ) / 2;
	tr1.y = ((float)GAMERESRCMNGR->m_GameDesc.dispInfo.dwHeight-MIDRSLTN_H)/2;
	m_LoadingImage.RenderSprite(&sc1,0,0,&tr1,RGBA_MAKE(255,255,255,255));
}

void CMapChange::NetworkMsgParse(BYTE Category,BYTE Protocol,void* pMsg)
{
	switch(Category)
	{
	case MP_FRIEND:
		break;
		
	case MP_PARTY:

		break;

	case MP_CHAT:
		CHATMGR->NetworkMsgParse(Protocol, pMsg);
		break;


	case MP_MUNPA:
		break;

	case MP_HACKCHECK:
		break;
	case MP_SIGNAL:
		break;
	case MP_GTOURNAMENT:
		GTMGR->NetworkMsgParse(Protocol, pMsg);
		break;
	case MP_JACKPOT:
		break;
	case MP_PET:
		break;
	case MP_SIEGEWAR:
		SIEGEMGR->NetworkMsgParse(Protocol, pMsg);
		break;
	case MP_SURVIVAL:
		break;
	case MP_CHEAT:
		{
					 NOTIFYMGR->NetworkMsgParseGet(Protocol, pMsg);
			switch(Protocol) 
			{
				case MP_CHEAT_EVENTNOTIFY_ON:
				{
					MSG_EVENTNOTIFY_ON* pmsg = (MSG_EVENTNOTIFY_ON*)pMsg;

					NOTIFYMGR->SetEventNotifyStr( pmsg->strTitle, pmsg->strContext );
					NOTIFYMGR->SetEventNotify( TRUE );
						NOTIFYMGR->SetEventNotifyChanged( TRUE );

					NOTIFYMGR->ResetEventApply();
					for(int i=0; i<eEvent_Max; ++i)
					{
						if( pmsg->EventList[i] )
							NOTIFYMGR->SetEventApply( i );
					}
				}
				break;
		
				case MP_CHEAT_EVENTNOTIFY_OFF:
				{
					MSGBASE* pmsg = (MSGBASE*)pMsg;
					NOTIFYMGR->SetEventNotify( FALSE );
					NOTIFYMGR->SetEventNotifyChanged( FALSE );
				}	
				break;
				//case MP_CHEAT_EVENTNOTIFYGET_ON:
					
				//case MP_CHEAT_EVENTNOTIFYGET_OFF:
					
				//	break;
			}
		}
		break;

	default:
		{
		}
	}
}

void CMapChange::EnterGame()
{
	if(m_bMapLoadComplete && m_bThreadTerminated && !m_bDisconnected )
	{
		MAINGAME->SetGameState(eGAMESTATE_GAMEIN,&m_GameInInitKind,sizeof(&m_GameInInitKind));
	}
}




