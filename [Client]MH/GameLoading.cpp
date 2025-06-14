// GameLoading.cpp: implementation of the CGameLoading class.

#include "stdafx.h"
#include "GameLoading.h"
#include "MainGame.h"
#include "GameIn.h"
#include "mhMap.h"
#include "cWindowSystemFunc.h"

#include "./WindowIDEnum.h"
#include "./Interface/cWindowManager.h"
#include "./Interface/cResourceManager.h"
#include "SkillManager_Client.h"
#include "./Input/Mouse.h"
#include "./Input/Keyboard.h"
#include "PathManager.h"
#include "LoadingDlg.h"
#include "./Interface/cScriptManager.h"
#include "cMsgBox.h"
#include "./Input/UserInput.h"
#include "UserInfoManager.h"
#include "ChatManager.h"

#include "GMNotifyManager.h"
#include "GameResourceManager.h"
#include "MHAudioManager.h"

#include "AiSetManager.h"
extern HWND _g_hWnd;
GLOBALTON(CGameLoading)
DWORD WINAPI LoadingThread(void *p);
CGameLoading::CGameLoading()
{
	m_bThreadTerminated=FALSE;
	m_bMapLoadComplete=FALSE;
	m_MapNum = -1;

	m_pDlg	= NULL;
}

CGameLoading::~CGameLoading()
{

}

BOOL CGameLoading::Init(void* pInitParam)
{

	AUDIOMGR->StopBGM();

	m_bThreadTerminated=FALSE;
	m_bMapLoadComplete=FALSE;
	NETWORK->SetCurState(this);

	GameLoadingParam * lParam = (GameLoadingParam *)pInitParam;

	MSG_WORD	msg;
	msg.Category	= MP_USERCONN;
	msg.Protocol	= MP_USERCONN_CHARACTERSELECT_SYN;
	msg.dwObjectID	= lParam->selectedCharNum;
	msg.wData		= gChannelNum;
	NETWORK->Send(&msg, sizeof(msg));

	char filename[256];
	sprintf(filename,"Image/LoadingMap/maploadingimage%02d.tga",lParam->m_MapNum);
	if(m_LoadingImage.LoadSprite(filename) == FALSE)
	{
		sprintf(filename,"Image/LoadingMap/maploadingimage00.tga");
		m_LoadingImage.LoadSprite(filename);
	}
	m_pDlg = (CLoadingDlg *)CreateGameLoading_m();
	

	WINDOWMGR->AfterInit();

	m_bFirst = TRUE;
	m_MapNum = -1;

	g_UserInput.SetInputFocus( FALSE );

	USERINFOMGR->SetMapChage( FALSE );//맵이동이 아닌경우에 처리할것이 있음

	return TRUE;
}
#include "AIManager.h"
void CGameLoading::Release(CGameState* pNextGameState)
{
	sCPlayerAI.ClearBeforeMapChange();

	m_LoadingImage.Release();
	m_pDlg = NULL;
}

void CGameLoading::Process()
{
	{
		EnterGame();
	}

	if(m_bFirst)
		m_bFirst = FALSE;
	else
	{
		if(m_bThreadTerminated == FALSE)
		{			
			EFFECTMGR->Init();
			
			m_bThreadTerminated = TRUE;
		}
		if(m_MapNum != (MAPTYPE)(-1) && m_bMapLoadComplete == FALSE && m_bThreadTerminated)
		{
			MAP->InitMap(m_MapNum);

			PATHMGR->SetMap(MAP, m_MapNum,MAP->GetTileWidth());			
			m_bMapLoadComplete = TRUE;
		}
	}

	return;
}
void CGameLoading::BeforeRender()
{
}
void CGameLoading::AfterRender()
{
	WINDOWMGR->Render();
	VECTOR2 tr,sc;
	tr.x = tr.y = 0;
	sc.x  = (float)GAMERESRCMNGR->m_GameDesc.dispInfo.dwWidth / MIDRSLTN_W;
	sc.y  = (float)GAMERESRCMNGR->m_GameDesc.dispInfo.dwHeight / MIDRSLTN_H;
	m_LoadingImage.RenderSprite(&sc,NULL,0,&tr,0xff000000/*RGBA_MAKE(0,0,0,255)*/);

	VECTOR2 tr1,sc1;
	sc1.x  = 1.0f;
	sc1.y  = 1.0f;
	tr1.x = tr1.y = 0;
	tr1.x = ((float)GAMERESRCMNGR->m_GameDesc.dispInfo.dwWidth - MIDRSLTN_W * sc1.x ) / 2;
	tr1.y = ((float)GAMERESRCMNGR->m_GameDesc.dispInfo.dwHeight-MIDRSLTN_H)/2;
	m_LoadingImage.RenderSprite(&sc1,NULL,0,&tr1,0xffffffff);
}

void CGameLoading::NetworkMsgParse(BYTE Category,BYTE Protocol,void* pMsg)
{
	switch(Category)
	{
	case MP_USERCONN:
		{
			switch(Protocol) 
			{
			case MP_USERCONN_CHARACTERSELECT_ACK:
				{
					MSG_BYTE* pmsg = (MSG_BYTE*)pMsg;
					m_MapNum = pmsg->bData;
				}
				return;

			case MP_USERCONN_CHARACTERSELECT_NACK:
				{
					PostMessage( _g_hWnd, WM_CLOSE, NULL, NULL );
				}
				return;
			}
		}
		break;
	case MP_CHAT:
		CHATMGR->NetworkMsgParse(Protocol, pMsg);
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
	case MP_JACKPOT:
		break;
	}
}

void CGameLoading::EnterGame()
{
	if(m_bMapLoadComplete && m_bThreadTerminated && !m_bDisconnected )
	{
		int initKind = eGameInInitKind_Login;
		MAINGAME->SetGameState(eGAMESTATE_GAMEIN,&initKind,sizeof(&initKind));
	}
}
DWORD WINAPI LoadingThread(void *p)
{

	return 0;
}
