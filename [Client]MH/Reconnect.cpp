#include "stdafx.h"
#include "Reconnect.h"
#include "ReconnectDialog.h"
#include "CharSelect.h"
#include "MainTitle.h"
#include "MainGame.h"
#include "Gamein.h"
#include "GameState.h"
#include "MHCamera.h"
#include "ObjectManager.h"
#include "MoveManager.h"
#include "ObjectStateManager.h"
#include "GlobalEventFunc.h"
#include "ServerListDialog.h"
#include "NumberPadDialog.h"
#include "WindowIDEnum.h"
#include "cResourceManager.h"
#include "cDialog.h"
#include "./Interface/CButton.h"
#include "cMsgBox.h"
#include "cWindowManager.h"
#include "ChannelDialog.h"
#include "MHTimeManager.h"
#include "OptionManager.h"
#include "ChatManager.h"
#include "UserInfoManager.h"
#include "petmanager.h"
#include "SkillManager_client.h"
#include "MHMap.h"
#include "AiManager.h"
#include "Itemmanager.h"
#include "./input/UserInput.h"
#include "mugongdialog.h"
#include "DungeonMgr.h"
extern char g_CLIENTVERSION[];
extern HWND _g_hWnd;
DWORD dwCheckBit2 = eEventMap | eTournament | eQuestRoom | eSurvivalMap | eInsDungeon;
GLOBALTON(CReDisConnManager);
CReDisConnManager::CReDisConnManager()
{
	//ZeroMemory(m_strUserID, sizeof(m_strUserID));
	m_bDisconnected = FALSE;
	//dwAccID = 0;
}
CReDisConnManager::~CReDisConnManager()
{
}
void CReDisConnManager::Init()
{
	dwMapLoadStuckCount = 0;
	m_bDisconnected = TRUE;
	//SetUserID(USERINFOMGR->GetUserID());
	for (int i = 0; i<eMaxState; i++)
	{
		RECONNECT_STATE[i] = false;
	}
	for (int i = 0; i<eMaxReconnection; i++)
	{
		RECONNECT_PROCESS[i] = false;
	}
	WaitMapCount = 0;
	CHATMGR->SaveChatList();
	m_OpenAutoPlay = AISETMGR->GetGameAI()->sPage5.pbAutoLeveling;
	if (MAP->IsMapKind(dwCheckBit2, MAP->GetMapNum()))
		dwHeroMapNum = 0;
	RECONNECT_STATE[eServerListStep] = true;
	RECONNECT_STATE[eFirstGameInDC] = true;
	m_dwStartTime = 0;
	m_StuckedOnTileTime = 0;
	OTI_DlgFunc(OTI_BTN_OK, NULL, 0);
	if (HERO)
	{
		HERO->DisableAutoAttack();
		HERO->RemoveAllObjectEffect();
		EFFECTMGR->RemoveAllEffect();
		HERO->SetCurComboNum(0);
		HERO->SetMovingAction(NULL);
		HERO->SetNextAction(NULL);
#ifdef _MUTIPET_
		for (int i = 0; i < 3; ++i)
			if (auto pPet = PETMGR->GetCurSummonPet(i))
				MOVEMGR->PetMoveStop(pPet);
#else
		if (PETMGR->GetCurSummonPet())
			MOVEMGR->PetMoveStop();
#endif
		OBJECTSTATEMGR->InitObjectState(HERO);
	}
	if (dwHeroMapNum == 0 && !DungeonMGR->IsDungeonMap((WORD)MAP->GetMapNum()))
	{
		dwHeroMapNum = MAP->GetMapNum();
		m_HeroChannel = gChannelNum;
	}
	else
	{
		dwHeroMapNum = 0;
	}
}
void CReDisConnManager::StartProcess(DWORD dwTime)
{
	m_Master = TRUE;
	m_dwStartTime = MHTIMEMGR->GetNewCalcCurTime(); //gCurTime;
	bCountDown = dwTime;
}
BOOL CReDisConnManager::IsDisconnect()
{
	return m_bDisconnected;
}
void CReDisConnManager::OnDisconnect()
{
	m_bDisconnected = TRUE;
}
void CReDisConnManager::NewLoopTime()
{
	m_dwMoveTime = MHTIMEMGR->GetNewCalcCurTime();//gCurTime;
	m_dwStartTime = MHTIMEMGR->GetNewCalcCurTime();//gCurTime;
	m_dwMovePosTime = MHTIMEMGR->GetNewCalcCurTime();//gCurTime;
	GAMEIN->GetReconnectDlg()->ReloadDlgPosition();
}
void CReDisConnManager::Process()
{
	if (m_Master)
	{
		if (RECONNECT_PROCESS[eLastMapProcess])
		{
			if (MHTIMEMGR->GetNewCalcCurTime() - m_dwMoveTime >= (bCountDown * 1000))
			{
				NewLoopTime();
				LastMap();
				return;
			}
		}
		if (RECONNECT_PROCESS[eLastPositionProcess])
		{
			if (MHTIMEMGR->GetNewCalcCurTime() - m_dwMovePosTime >= (bCountDown * 1000))
			{
				NewLoopTime();
				LastPosition();
				return;
			}
		}
		if (!RECONNECT_PROCESS[eLastMapProcess] && !RECONNECT_PROCESS[eLastPositionProcess])
		{
			if (MHTIMEMGR->GetNewCalcCurTime() - m_dwStartTime >= (bCountDown * 1000))
			{
				NewLoopTime();
				switch (MAINGAME->GetCurStateNum())
				{
				case eGAMESTATE_GAMEIN:
				{
					if (RECONNECT_STATE[eServerListStep] && RECONNECT_STATE[eFirstGameInDC])
					{
						if (HERO->GetState() == eObjectState_Move)
						{
							break;
						}
						if (MAP->CollisionCheck_OneTile(&HERO->GetCurPosition()) == FALSE)
						{
							VECTOR3 vecDest = HERO->m_WayPointManager->GetFlagDestination();
							MOVEMGR->SetHeroTarget(&vecDest, true);
							if (m_StuckedOnTileTime<4)
							{
								m_StuckedOnTileTime = m_StuckedOnTileTime + 1;
								return;
							}
						}
						HERO->GetPosition(&m_Heropos);

						//GAMEIN->Release(MAINGAME->GetCurGameState());
						ShowMainTitle();
						break;
					}
				}
				case eGAMESTATE_TITLE:		MainTitleProcess();	 break;
				case eGAMESTATE_CHARSELECT:	CharSelectProcess(); break;
				case eGAMESTATE_GAMELOADING:
					if (m_bDisconnected)
					{
						RECONNECT_STATE[eServerListStep] = true;
						RECONNECT_STATE[eFirstGameInDC] = true;
						//NewLoopTime();
						g_UserInput.SetAllowMouseClick(true);
						g_UserInput.SetInputFocus(TRUE);
						ShowMainTitle();
						//MessageBox(NULL, "Error", "Could not get registry value", MB_OK);
					}
					
					break;
				//default: ShowMainTitle(); break;					
				//case eGAMESTATE_MAPCHANGE:
				
				}
			}
		}
	}
}
bool CReDisConnManager::LastMap()
{
	if (MAP->GetMapNum() == REDISCONN->GetHeroMapNum())
	{
		RECONNECT_PROCESS[eLastMapProcess] = false;
		return true;
	}
	gChannelNum = m_HeroChannel;
	g_UserInput.GetHeroMove()->MapChange(dwHeroMapNum, 0, 0, 0, 1);
	return false;
}
bool CReDisConnManager::IsHeroIdle()
{
	if (HERO->GetState() == eObjectState_Immortal ||
		HERO->GetState() == eObjectState_None)
		return true;
	return false;
}
void CReDisConnManager::LastPosition()
{
	if (!IsHeroIdle())
		return;
	VECTOR3 vHeroPos;
	HERO->GetPosition(&vHeroPos);
	double distuncetmp = ((vHeroPos.x - m_Heropos.x)*(vHeroPos.x - m_Heropos.x) + (vHeroPos.z - m_Heropos.z)*(vHeroPos.z - m_Heropos.z));
	m_Master = MAP->CollisionCheck_OneTile(&m_Heropos);
	if (m_Master)
	{
		bCountDown = 2;
		if (sqrt(distuncetmp)> (1 * 100))
		{
			g_UserInput.GetHeroMove()->MapPosition(HERO, &m_Heropos);
			MOVEMGR->SetHeroTarget(&m_Heropos, 1.f);
			return;
		}
	}
	DoneReconnecting();
}
void CReDisConnManager::ShowMainTitle()
{
	dwMapLoadStuckCount = 0;
	m_bDisconnected = FALSE;
	RECONNECT_STATE[eFirstGameInDC] = false;
	RECONNECT_STATE[eServerListStep] = false;
	MAINGAME->SetGameState(eGAMESTATE_TITLE);
	NETWORK->ReleaseKey();
	TITLE->NoDisconMsg();
	TITLE->SetServerList();
}
void CReDisConnManager::StopReconnect(WORD bFlag)
{
	if (GAMEIN->GetReconnectDlg()->IsActive())
		GAMEIN->GetReconnectDlg()->SetActive(false);
	g_UserInput.SetAllowMouseClick(true);
	switch (bFlag)
	{
	case 2:
		MAINGAME->SetGameState(eGAMESTATE_TITLE);
		NETWORK->ReleaseKey();
		TITLE->NoDisconMsg();
		TITLE->SetServerList();
		break;
	}
	if (MAINGAME->GetCurStateNum() == eGAMESTATE_GAMEIN)
	{
		sCPlayerAI.IsRun = true;
		FinishReconnect();
	}
	dwHeroMapNum = 0;
	m_OpenAutoPlay = false;
	m_Master = FALSE;
}
bool serverlisttemp = false;
void CReDisConnManager::MainTitleProcess()
{
	RECONNECT_STATE[eFirstGameInDC] = false;
	GAMEIN->GetReconnectDlg()->DisableSet();
	cDialog* m_LoginIDDlg = WINDOWMGR->GetWindowForID(MT_LOGINDLG);
	cDialog* m_ServerListDlg = WINDOWMGR->GetWindowForID(SL_SERVERLISTDLG);

	RecMSG(MBI_PASSERROR);	

	if (m_ServerListDlg)
	{
		if (m_ServerListDlg->IsActive())
		{
			m_ServerListDlg->SetDisable(TRUE);
			TITLE->ConnectToServer(m_ServerIndex);
			GAMEIN->GetReconnectDlg()->AddMessage(IMAGEMSG, 1348, BUTTON);
		}
		else
		{
			if (m_LoginIDDlg&&!m_LoginIDDlg->IsActive())
			if (!RECONNECT_STATE[eServerListStep] && !serverlisttemp)
			{
				serverlisttemp = true;
				TITLE->NoDisconMsg();
				TITLE->SetServerList();
			}
		}
	}
	RecMSG(MBI_LOGINERRORCLOSE);
	cDialog* m_LoginDlg = WINDOWMGR->GetWindowForID(MT_LOGINDLG);
	if (m_LoginDlg)
	{
		if (m_LoginDlg->IsActive())
		{			
			if (FillLoginInfo(m_LoginDlg))
			{
				MSG_LOGIN_SYN msg;
				msg.Category = MP_USERCONN;
				msg.Protocol = MP_USERCONN_LOGIN_SYN;
				SafeStrCpy(msg.id, UserName, MAX_NAME_LENGTH + 1);
				SafeStrCpy(msg.pw, UserPwd, MAX_NAME_LENGTH + 1);
				SafeStrCpy(msg.pn, UserPin, MAX_NAME_LENGTH + 1);
				SafeStrCpy(msg.Version, g_CLIENTVERSION, 256);
				GAMEIN->GetMacAddress(msg.Mac);
				msg.AuthKey = TITLE->GetDistAuthKey();
				NETWORK->Send(&msg, sizeof(msg));
				TITLE->StartWaitConnectToAgent(TRUE);
				//USERINFOMGR->SetUserID(UserName);
				MT_EditReturnFunc(MT_OKBTN, NULL, WE_BTNCLICK);
			}
			else
			{
				m_dwStartTime = MHTIMEMGR->GetNewCalcCurTime(); //gCurTime;
				return;
			}
			RecMSG(MBI_LOGINERRORCLOSE);
			RecMSG(MBI_WAIT_LOGINCHECK);
			RecMSG(MBI_OVERLAPPEDLOGIN);
			
			RecMSG(MBI_LOGINEDITCHECK);
			RecMSG(MBI_DISTCONNECT_ERROR);

			
		}
	}
}
void CReDisConnManager::CharSelectProcess()
{
	RecMSG(MBI_SERVERDISCONNECT);
	CChannelDialog* m_ChannelDlg = (CChannelDialog*)WINDOWMGR->GetWindowForID(CHA_CHANNELDLG);
	if (m_ChannelDlg)
	{
		if (m_ChannelDlg->IsActive())
		{
			m_ChannelDlg->SetMovable(false);
			m_ChannelDlg->SelectChannel(m_ChannelIndex);
			m_ChannelDlg->OnConnect();
		}
	}
	if (CHARSELECT->GetCurSelectedPlayer() != NULL)
	{
		CHARSELECT->SendMsgGetChannelInfo();
	}
	cDialog* m_CharSelectDlg = WINDOWMGR->GetWindowForID(CS_CHARSELECTDLG);
	if (m_CharSelectDlg && m_CharSelectDlg->IsActive())
	{
		CHARSELECT->SelectPlayer(m_CharIndex);
		CHARSELECT->SetDisablePick(TRUE);
	}
	RecMSG(MBI_MAPSERVER_CLOSE);
	RecMSG(MBI_SERVERDISCONNECT);
}
void CReDisConnManager::DoneReconnecting()
{
	WINDOWMGR->DisableAllWindow(false);
	HERO->GetPosition(&m_Heropos);
	MOVEMGR->SetHeroTarget(&m_Heropos, FALSE);
	StopReconnect();
}
void CReDisConnManager::FinishReconnect()
{
	if (ITEMMGR->GetUsedItemInfo(eIncantation_MugongExtend))
	{
		GAMEIN->GetMugongDialog()->SetMugongExtend(TRUE);
		if (GAMEIN->GetMugongDialog()->IsActive())
			GAMEIN->GetMugongDialog()->SetActive(TRUE);
	}
	sCPlayerAI.SetOpen(m_OpenAutoPlay);
	sCPlayerAI.SetNeedSitNeutral();
	sCPlayerAI.AutoCallPet();
}
bool CReDisConnManager::FillLoginInfo(cDialog * p)
{
	cDialog  * dlg = (cDialog *)p;
	cEditBox * editboxId = (cEditBox *)dlg->GetWindowForID(MT_IDEDITBOX);
	cEditBox * editboxPwd = (cEditBox *)dlg->GetWindowForID(MT_PWDEDITBOX);
	cEditBox * editboxPin = (cEditBox *)dlg->GetWindowForID(MT_PNSTATIC);
	editboxId->SetEditText(UserName);
	editboxPwd->SetEditText(UserPwd);
	editboxPin->SetEditText(UserPin);
	char * userID = editboxId->GetEditText();
	char * userPWD = editboxPwd->GetEditText();
	if ((strcmp(userID, "") == 0) && (strcmp(userPWD, "") == 0))
	{
		GAMEIN->GetReconnectDlg()->AddMessage(IMAGEMSG, 1348, BUTTON);
		return false;
	}
	GAMEIN->GetReconnectDlg()->AddMessage(IMAGEMSG, 1348);
	return true;
}
void CReDisConnManager::GoToGameLoginDialog()
{
	InGameDC = FALSE;
	if (MAINGAME->GetCurStateNum() == eGAMESTATE_TITLE)
	{
		TITLE->ShowServerList();
	}
	else
	{
		RECONNECT_STATE[eServerListStep] = false;
		if (OBJECTMGR->GetHero())
		{
			if (dwHeroMapNum == 0)
			{
				HERO->GetPosition(&m_Heropos);
				dwHeroMapNum = MAP->GetMapNum();
				m_HeroChannel = gChannelNum;
			}
			HERO->DisableAutoAttack();
			HERO->RemoveAllObjectEffect();
			HERO->InitHero(HERO->GetHeroTotalInfo());
			HERO->InitSkinDelayTime();
		}
		MAINGAME->SetGameState(eGAMESTATE_TITLE);
	}
	NETWORK->ReleaseKey();
	TITLE->NoDisconMsg();
	TITLE->SetServerList();
}
BOOL CReDisConnManager::RecMSG(int id)
{
	/*if (id==MBI_OVERLAPPEDLOGIN)
	{
		MSGBASE msg;
		msg.Category = MP_USERCONN;
		msg.Protocol = MP_USERCONN_FORCE_DISCONNECT_OVERLAPLOGIN;
		NETWORK->Send(&msg, sizeof(msg));

		return false;
	}*/
	/**/


	cDialog* m_DialogToKill = WINDOWMGR->GetWindowForID(id);
	if (m_DialogToKill && m_DialogToKill->IsActive())
	{
		if (m_DialogToKill == WINDOWMGR->GetWindowForID(MBI_PASSERROR))
		{
			m_Master = FALSE;

			if (FALSE == m_Master)
			{
				GAMEIN->GetReconnectDlg()->AddMessage(BUTTON, CHATMGR->GetChatMsg(281));
			}
		}

		if (m_DialogToKill == WINDOWMGR->GetWindowForID(MBI_OVERLAPPEDLOGIN))
		{
			MSGBASE msg;
			msg.Category = MP_USERCONN;
			msg.Protocol = MP_USERCONN_FORCE_DISCONNECT_OVERLAPLOGIN;
			NETWORK->Send(&msg, sizeof(msg));
		}

		WINDOWMGR->CloseAllMsgBoxNoFunc();
		WINDOWMGR->CloseAllMsgBox();		
	}
	return TRUE;
}
void CReDisConnManager::SetUserID(char* strID)
{
	SafeStrCpy(m_strUserID, strID,strlen(strID));
}