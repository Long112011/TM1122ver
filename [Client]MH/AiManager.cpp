#include "stdafx.h"
#include "AIManager.h"
#include "cWindowManager.h"
#include "WindowIDEnum.h"
#include <MMSystem.h>
#include "ObjectManager.h"
#include "QuickItem.h"
#include "QuickDialog.h"
#include "QuickManager.h"
#include "OptionDialog.h"
#include "./input/UserInput.h"
#include "SkillManager_client.h"
#include "ChatManager.h"
#include "MoveManager.h"
#include "ExitManager.h"
#include "GameIn.h"
#include "ItemManager.h"
#include "ReviveDialog.h"
#include "StatusIconDlg.h"
#include "MHMap.h"
#include "MHTimeManager.h"
#include "interface/cEditBox.h"
#include "./interface/cEditBox.h"
#include "./Interface/cWindowManager.h" 
#include "./Interface/cPushupButton.h"
#include "./Interface/cGuageBar.h"
#include "./Interface/cCheckBox.h"
#include "./interface/cStatic.h"
#include "./interface/cCheckbox.h"      
#include "./Interface/cListDialog.h"    
#include "cSpin.h"
#include "TileManager.h"
#include "ObjectStateManager.h"
#include "FileEncrypt.h"
#include "MugongBase.h"
#include "MugongDialog.h"
#include "GameIn.h"
#include "MussangManager.h"
#include "MHNetwork.h"
#include "PetManager.h"
#include "ChatManager.h"
#include "MainGame.h"
#include <time.h>
#include "cMsgBox.h"
#include "Reconnect.h"
#include "SkillDelayManager.h"
#include "MiniMapDlg.h"
#include "GlobalEventFunc.h"
#include "PartyInviteDlg.h"
#include "PartyManager.h"
#include "WantedManager.h"
#include "BattleSystem_Client.h"
#include "VimuManager.h"
#include "JackFlashRoar.h"
#include "MugongManager.h"
#include "PeaceWarModeManager.h"
#include "AutoPath.h"
#include "PathManager.h"
#define MaxMugongSlot	680
//DWORD WINAPI threadProc2(LPVOID p_Param);
#define ADDMSG(a) CHATMGR->AddMsg(CTC_SYSMSG, a);
extern HWND _g_hWnd;
extern int  m_PetIdx;
DWORD dwCheckBitNoAuto = eNoAuto;
DWORD dwCheckBit = eEventMap | eTournament | eQuestRoom | eSurvivalMap | eNoAuto;
BOOL bagimakan;
BOOL debugBoolStart;
DWORD	debugStartTick;
DWORD	dwEndTick;
#define DEBUG_START_TEST debugStartTick = GetTickCount();
#define DEBUG_END_TEST dwEndTick = GetTickCount(); DWORD	dwFuncTick = dwEndTick - debugStartTick;	\
	char	debugBUFF[1024];	char	debugBUFF2[1024];	sprintf(debugBUFF, "Tick:%d\n", dwFuncTick);	\
	DWORD	miliseconds = dwFuncTick % 1000;	dwFuncTick /= 1000;	DWORD	seconds = dwFuncTick % 60;	\
	dwFuncTick /= 60;DWORD	minutes = dwFuncTick % 60;dwFuncTick /= 60;  \
	DWORD	hours = dwFuncTick;	sprintf(debugBUFF2, "[MAP LOAD SEC]\n\n\n%d:%02d:%02d.%03d\n%s\n", hours, minutes, seconds, miliseconds, debugBUFF); \ { CHATMGR->AddMsg(CTC_SYSMSG, debugBUFF2); }
GLOBALTON(CPlayerAI);


/*DWORD WINAPI threadProc2(LPVOID p_Param)
{
	if (true == GAMEIN->GetReviveDialog()->IsActive() && !HERO->IsDied()) GAMEIN->GetReviveDialog()->SetActive(false);
	if (OBJECTMGR->GetSelectedObjectID() == HEROID) OBJECTMGR->SetSelectedObject(NULL);
	sCPlayerAI.ProcessReviving2();
	sCPlayerAI.ProcessTambahan();
	if (AISETMGR->GetGameAI()->sPage5.pbAutoLeveling)
	{
		if (MAP->GetMapNum() != sCPlayerAI.m_AttactedSet.AutoActtactMonsterSet.LastMap) return TRUE;
		if (sCPlayerAI.dwHeroID != HEROID) { sCPlayerAI.SetAIDisable(); return TRUE; }
		sCPlayerAI.ProcessLepasMatiBaru();
		if (HERO->IsDied())		return TRUE;
		if (sCPlayerAI.CheckHeroIdle() > 2) { HERO->m_WayPointManager->SetFlagDestination(sCPlayerAI.vPos); MOVEMGR->SetHeroTarget(&sCPlayerAI.vPos, 1.f); }
		sCPlayerAI.ProcessSit();
		if (sCPlayerAI.m_bSitProcessing && HERO->GetState() == eObjectState_Ungijosik) sCPlayerAI.ProcessSitFailed();
		sCPlayerAI.ProcessSkillGelung();
		sCPlayerAI.ProcessAutoPotion();
		sCPlayerAI.ProcessMussang();
		if (!sCPlayerAI.m_StopPlease) { sCPlayerAI.ProcessBelasahPVP(); sCPlayerAI.CheckAreaProcess(); }
		sCPlayerAI.ProcessLookMonsterRunupMxo();
		sCPlayerAI.ProcessAttackMacro();
	}
	return FALSE;
}*/


void CPlayerAI::threadProcEnable()
{
	//----------------------------------------------------------------------------------
	// below is thread function for testing purposed
	
	//threadHandle = CreateThread(NULL, 0, threadProc2, NULL, 0, NULL);
	//if (threadHandle == NULL)
	//{
	//	ADDMSG( "threadHandle is null");
	//}

	//WaitForSingleObject(threadHandle, INFINITE);
	
	// thread function for testing purposed ended
	//----------------------------------------------------------------------------------
}
void CPlayerAI::threadProcDisable()
{
	//----------------------------------------------------------------------------------
	// below is thread function for testing purposed
	//CloseHandle(threadHandle);
	// thread function for testing purposed ended
	//----------------------------------------------------------------------------------
}
void CPlayerAI::Process()
{
	/*if (false == sCPlayerAI.IsRun) return;
	if (MAINGAME->GetCurStateNum() != eGAMESTATE_GAMEIN) return;	
	if (true == REDISCONN->GetStart()) return;	
	//----------------------------------------------------------------------------------
	// below is thread function for testing purposed
	HANDLE threadHandle;
	DWORD        threadID;
	threadHandle = CreateThread(NULL, 0, threadProc2, NULL, 0, &threadID);
	if (threadHandle == NULL)
	{
		ADDMSG("threadHandle is null");
	}
	WaitForSingleObject(threadHandle, INFINITE);
	CloseHandle(threadHandle);*/
	// thread function for testing purposed ended
	//----------------------------------------------------------------------------------
	
	if (false == sCPlayerAI.IsRun) return;
	if (MAINGAME->GetCurStateNum() != eGAMESTATE_GAMEIN) return;	
	if (true == REDISCONN->GetStart()) return;	
	if (MAP->IsMapKind(dwCheckBitNoAuto) && AISETMGR->GetGameAI()->sPage5.pbAutoLeveling)
	{
		CHATMGR->AddMsg(CTC_SYSMSG, "Auto leveling is disabled for this map.");
		return;
	}
	if (true == GAMEIN->GetReviveDialog()->IsActive() && !HERO->IsDied()) GAMEIN->GetReviveDialog()->SetActive(false);
	if (OBJECTMGR->GetSelectedObjectID() == HEROID) OBJECTMGR->SetSelectedObject(NULL);
	ProcessReviving2();
	ProcessTambahan();
	if (AISETMGR->GetGameAI()->sPage5.pbAutoLeveling)
	{
		if (MAP->GetMapNum() != m_AttactedSet.AutoActtactMonsterSet.LastMap) return;
		if (dwHeroID != HEROID) { SetAIDisable(); return; }
		ProcessLepasMatiBaru();
		if (HERO->IsDied())		return;
		if (CheckHeroIdle() > 2) { HERO->m_WayPointManager->SetFlagDestination(vPos); MOVEMGR->SetHeroTarget(&vPos, TRUE); }
		ProcessSit();
		if (m_bSitProcessing && HERO->GetState() == eObjectState_Ungijosik) ProcessSitFailed();
		ProcessSkillGelung();
		ProcessAutoPotion();
		ProcessMussang();
		if (!m_StopPlease) { ProcessBelasahPVP(); CheckAreaProcess(); }
		ProcessLookMonsterRunupMxo();
		ProcessAttackMacro();
	}
}
void CPlayerAI::SetAIEnable()
{
	if (!sCPlayerAI.IsRun) return; 
	if (MAP->IsMapKind(dwCheckBitNoAuto))
	{
		CHATMGR->AddMsg(CTC_SYSMSG, "Auto leveling is disabled for this map.");
		return;
	}
	if (HERO != NULL && MAP->IsMapKind(dwCheckBit)) return;
	if (HERO->IsDied()) { CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(2583)); return; }
	if (GAMEIN->GetGameInInitKind() == eGameInInitKind_EventMapEnter) return;
	COptionDialog * pDlg = GAMEIN->GetOptionDialog();
	cDialog* m_Dlg = (cDialog*)pDlg->GetTabSheet(4);
	if (m_Dlg)
	{
		SaveTemp();
		AISETMGR->GetGameAI()->sPage5.pbAutoLeveling = true;
		((cPushupButton*)m_Dlg->GetWindowForID(OTI_PB_AUTOHUNT_ON))->SetPush(AISETMGR->GetGameAI()->sPage5.pbAutoLeveling);
	}
	m_AttactedSet.AutoActtactMonsterSet.LastMap = MAP->GetMapNum();
	CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(2267));
	CHATMGR->AddMsgFade(CTC_SYSMSG, CHATMGR->GetChatMsg(2267));
	DufualtUsekyungGong2();
	AUTOPATH->SetTargetPos(HERO->GetCurPosition());
	if (MOVEMGR->IsMoving(HERO)) MOVEMGR->HeroMoveStop();
	HERO->SetFollowPlayer(0);
	InitRangeArea();
	if (HERO->GetState() == eObjectState_Immortal || HERO->GetState() == eObjectState_None) OBJECTSTATEMGR->EndObjectState(HERO, eObjectState_Move);
	bImmortalTimeCount = 0;
	dwHeroID = HEROID;
}
#include "./Interface/cResourceManager.h"
void CPlayerAI::ClearBeforeMapChange()
{
	WINDOWMGR->SetDragEnd();
	WINDOWMGR->DestroyWindowAll();
	RESRCMGR->ReleaseResource(28);
}
void CPlayerAI::BeforeMapChange()
{
	AUTOPATH->SetTargetPos(HERO->GetCurPosition());
	sCPlayerAI.SetOpen(FALSE);
	AISETMGR->GetGameAI()->sPage5.pbAutoLeveling == false;
	GAMEIN->GetJackFlashDlg()->SetMapChange(true);
	sCPlayerAI.IsRun = FALSE;
}
void CPlayerAI::SetAIDisable()
{
	OBJECTMGR->SetSelectedObject(NULL);
	AISETMGR->GetGameAI()->sPage5.pbAutoLeveling = FALSE;
	CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(2268));
	CHATMGR->AddMsgFade(CTC_SYSMSG, CHATMGR->GetChatMsg(2268));
	COptionDialog * pDlg = GAMEIN->GetOptionDialog();
	cDialog* m_Dlg = (cDialog*)pDlg->GetTabSheet(4);
	if (m_Dlg)((cPushupButton*)m_Dlg->GetWindowForID(OTI_PB_AUTOHUNT_ON))->SetPush(AISETMGR->GetGameAI()->sPage5.pbAutoLeveling);
	AUTOPATH->SetTargetPos(HERO->GetCurPosition());
	if (MOVEMGR->IsMoving(HERO))
	{
		MOVEMGR->HeroMoveStop();
	}
	SaveTemp();
	HERO->DisableAutoAttack();
	HERO->SetCurComboNum(0);
	HERO->SetMovingAction(NULL);
	HERO->SetNextAction(NULL);
	bImmortalTimeCount = 0;
}
CPlayerAI::CPlayerAI(void)
{
	AISETMGR->GetGameAI()->sPage5.pbAutoLeveling = false;
	memset(m_TimeForProtect, 0, sizeof(DWORD*)* Protected_Num);
	memset(m_SkillTimeLast, 0, sizeof(DWORD*)* 5);
	IsRun = false;
	m_CanUse = TRUE;
	AttackerID = 0;
	m_IsRunWay = true;
	m_MonsterAttrTime = 0;
	m_ReMoveTime = time(NULL);
	m_MovePoint = time(NULL);
	IsFirstRun = TRUE;
	IsAutoAttr = FALSE;
	CheckKyunKong = TRUE;
	pAttacker = NULL;
	ReBuildIsMovePoint = FALSE;
	m_TimeForActtact = 0;
	m_TimeForTuna = 0;
	m_TimeHeroDead = 0;
	ReadyForRelive = FALSE;
	m_TimeForDaZuo = 0;
	dzCurTime = gCurTime;
	m_ReSelectTime = gCurTime;
	m_playerstate = player_isin_idel;
	SetSkillNextFlag(0);
	vPos.x = AISETMGR->GetGameAI()->sPage5.spPosX * 100;
	vPos.z = AISETMGR->GetGameAI()->sPage5.spPosY * 100;
	m_GradeTime = 0;
	m_IsNeedSit = FALSE;
	m_nCount = 0;
	m_nSitCount = 60;
	m_dwSitStartTime = 0;
	m_bSitProcessing = FALSE;
	phFlag = FALSE;
	m_dwRevStartTime2 = 0;
	m_nRevCount2 = EXIT_COUNT;
	m_nCount2 = 0;
	m_bRevProcessing2 = FALSE;
	m_TimeForHideObject = 0;
	m_DeathCount = 0;
	m_StopPlease = false;
	dwCheckAreaTimeTick = 0;
	dwRangeArea = 0;
	bImmortal = false;
	bImmortalTimeCount = 0;
	bReviveDialogIsShowingSwitch = true;
	bReviveDialogIsShowing = false;
	dwReviveDialogTimeTick = 0;
	LockIntoPvp = false;
	pAttackerObject = NULL;
	OutOfRangePvp = false;
	TestPeace = false;
	bagimakan = FALSE;
}
void CPlayerAI::ProcessTestPeace()
{
	if (!TestPeace)return;
	static int TempTime;
	if (gCurTime - TempTime >= 1)
	{
		TempTime = gCurTime;
		PEACEWARMGR->ToggleHeroPeace_WarMode();
		CHATMGR->AddMsg(CTC_SYSMSG, "is running");
	}
}
DWORD CPlayerAI::CheckHeroIdle()
{
	static DWORD timeTemp;
	if (HERO->GetState() == eObjectState_Immortal || HERO->GetState() == eObjectState_None)		
	{
		if (gCurTime - timeTemp > 1000)
		{
			timeTemp = gCurTime;
			bImmortalTimeCount = bImmortalTimeCount + 1;
		}
	}
	else
	{
		timeTemp = 0;
		bImmortalTimeCount = 0;
	}
	return bImmortalTimeCount;
}
CPlayerAI::~CPlayerAI(void)
{
	AISETMGR->GetGameAI()->sPage5.pbAutoLeveling = false;
}
CPlayerAI& CPlayerAI::getinstance()
{
	static CPlayerAI playerai;
	return playerai;
}
bool CPlayerAI::IfMonster()
{
	CObject*sObj = OBJECTMGR->GetSelectedObject();
	if (sObj &&
		sObj->GetObjectKind() & eObjectKind_Monster &&
		!sObj->IsDied() &&
		sObj->GetObjectKind() != eObjectKind_Player
		)
		return true;
	return false;
}
int b = 1;
void CPlayerAI::ProcessCheckOutOfBound()
{
	if (gCurTime - dwCheckAreaTimeTick>500)
	{
		dwCheckAreaTimeTick = gCurTime;
		VECTOR3 vHeroPos;
		HERO->GetPosition(&vHeroPos);
		double distuncetmp = ((vHeroPos.x - vPos.x)*(vHeroPos.x - vPos.x) + (vHeroPos.z - vPos.z)*(vHeroPos.z - vPos.z));
		if (sqrt(distuncetmp) > (vPos, dwRangeArea * 100))
		{
			return;
		}
		else
		{
			if ((HERO->GetState() == eObjectState_Immortal ||
				HERO->GetState() == eObjectState_None) && OBJECTMGR->GetSelectedObject() == NULL)
			{
				if (b > 5)
				{
					CheckAreaProcessSmallPoint();
					b = 1;
				}
			}
		}
	}
}
void CPlayerAI::ProcessLookMonsterGamepad()
{
	if (HERO->GetState() == eObjectState_SkillUsing)
		return;
	CObject*sObj = OBJECTMGR->GetSelectedObject();
	if (sObj )
	{
		if (sObj->GetObjectKind() == eObjectKind_Player)
		{
			if (AttackerID == 0)
			{
				OBJECTMGR->SetSelectedObject(NULL);
				return;
			}
			CPlayer* pPlayer = (CPlayer*)OBJECTMGR->GetObject(AttackerID);
			if (!pPlayer) return;
			if (!pPlayer->IsDied() || ((CHero*)pPlayer)->IsPKMode())
			{
				return;
			}
		}
		else
		{
			OBJECTMGR->SetSelectedObject(NULL);
			{
			}
		}
	}
	if (HERO->GetState() == eObjectState_Ungijosik)
	{
		return;
	}
	if (HERO->GetState() == eObjectState_SkillUsing)
	{
		return;
	}
	if (
		OBJECTMGR->GetSelectedObject() &&
		(OBJECTMGR->GetSelectedObject()->GetObjectKind() >= eObjectKind_Monster && OBJECTMGR->GetSelectedObject()->GetObjectKind() <= eObjectKind_ToghterPlayMonster) &&
		!OBJECTMGR->GetSelectedObject()->IsDied() &&
		OBJECTMGR->GetSelectedObject()->GetObjectKind() != eObjectKind_Player)
	{
		return;
	}
	TimeNow = gCurTime;
	VECTOR3 herotmppos, monstertmppos,newVPos;
	double distunce = 0;
	BOOL IgnoreBoss = AISETMGR->GetGameAI()->sPage5.cbIgnoreBoss;
	ObjectArray<LPOBJECT> oArray;
	HERO->GetPosition(&newVPos);
	OBJECTMGR->GetTargetInRange(&newVPos, dwRangeArea * 100, &oArray, FALSE, TRUE, IgnoreBoss);
	oArray.SetPositionHead();
	LPOBJECT lptmpMons = NULL;
	bool bInit = false;
	for (int i = 0; i<oArray.nObjectNum; i++)
	{
		LPOBJECT lpObj = oArray.GetNextObject();
		BYTE wJobMonsterKind = lpObj->GetObjectKind();
		if (wJobMonsterKind & eObjectKind_Monster)
		{
			HERO->GetPosition(&herotmppos);
			lpObj->GetPosition(&monstertmppos);
			double distuncetmp = ((monstertmppos.x - herotmppos.x)*(monstertmppos.x - herotmppos.x) + (monstertmppos.z - herotmppos.z)*(monstertmppos.z - herotmppos.z));
			if (!bInit)
			{
				lptmpMons = lpObj;
				distunce = distuncetmp;
				bInit = true;
				continue;
			}
			if (distuncetmp < distunce)
			{
				distunce = distuncetmp;
				lptmpMons = lpObj;
			}
		}
	}
	CMonster* pMonster = (CMonster*)lptmpMons;
	OBJECTMGR->SetSelectedObject(pMonster);
}
void CPlayerAI::ProcessLookMonsterRunupMxo()
{
	if (HERO->GetState() == eObjectState_SkillUsing)
		return;
	CObject*sObj = OBJECTMGR->GetSelectedObject();
	if (sObj && sObj->GetObjectKind() == eObjectKind_Player)
	{
		if (AttackerID == 0)
		{
			OBJECTMGR->SetSelectedObject(NULL);
			return;
		}
		CPlayer* pPlayer = (CPlayer*)OBJECTMGR->GetObject(AttackerID);
		if (!pPlayer) return;
		if (!pPlayer->IsDied() || ((CHero*)pPlayer)->IsPKMode())
		{
			return;
		}
	}
	if (HERO->GetState() == eObjectState_Ungijosik)
	{
		return;
	}
	if (HERO->GetState() == eObjectState_SkillUsing)
	{
		return;
	}
	if (
		OBJECTMGR->GetSelectedObject() &&
		(OBJECTMGR->GetSelectedObject()->GetObjectKind() >= eObjectKind_Monster && OBJECTMGR->GetSelectedObject()->GetObjectKind() <= eObjectKind_ToghterPlayMonster) &&
		!OBJECTMGR->GetSelectedObject()->IsDied() &&
		OBJECTMGR->GetSelectedObject()->GetObjectKind() != eObjectKind_Player &&
		(gCurTime - TimeNow<8000))
	{
		return;
	}
	TimeNow = gCurTime;
	VECTOR3 herotmppos, monstertmppos;
	double distunce = 0;
	BOOL MonsterBehindWall = AISETMGR->GetGameAI()->sPage5.cbNoMonsterWall;
	BOOL IgnoreBoss = AISETMGR->GetGameAI()->sPage5.cbIgnoreBoss;
	ObjectArray<LPOBJECT> oArray;
	OBJECTMGR->GetTargetInRange(&vPos, dwRangeArea * 100, &oArray, FALSE, MonsterBehindWall, IgnoreBoss);
	oArray.SetPositionHead();
	LPOBJECT lptmpMons = NULL;
	bool bInit = false;
	for (int i = 0; i<oArray.nObjectNum; i++)
	{
		LPOBJECT lpObj = oArray.GetNextObject();
		BYTE wJobMonsterKind = lpObj->GetObjectKind();
		if (wJobMonsterKind & eObjectKind_Monster)
		{
			HERO->GetPosition(&herotmppos);
			lpObj->GetPosition(&monstertmppos);
			double distuncetmp = ((monstertmppos.x - herotmppos.x)*(monstertmppos.x - herotmppos.x) + (monstertmppos.z - herotmppos.z)*(monstertmppos.z - herotmppos.z));
			if (!bInit)
			{
				lptmpMons = lpObj;
				distunce = distuncetmp;
				bInit = true;
				continue;
			}
			if (distuncetmp < distunce)
			{
				distunce = distuncetmp;
				lptmpMons = lpObj;
			}
		}
	}
	AttactMonster(lptmpMons, distunce);
}
void CPlayerAI::HideObjectGamein()
{
	OBJECTMGR->HideAllNpc(OPTIONMGR->GetGameOption()->bShowNpc);
	OBJECTMGR->HideAllPet(!OPTIONMGR->GetGameOption()->bShowPet);
	OBJECTMGR->HideAllMonster(OPTIONMGR->GetGameOption()->bShowMonster);

	if (OBJECTMGR->IsNpcHiding() != OPTIONMGR->GetGameOption()->bShowNpc)
		OBJECTMGR->HideAllNpc(OPTIONMGR->GetGameOption()->bShowNpc);
	if (OBJECTMGR->IsPetHiding() != OPTIONMGR->GetGameOption()->bShowPet)
		OBJECTMGR->HideAllPet(!OPTIONMGR->GetGameOption()->bShowPet);
	if (OBJECTMGR->IsMonsterHiding() != OPTIONMGR->GetGameOption()->bShowMonster)
		OBJECTMGR->HideAllMonster(OPTIONMGR->GetGameOption()->bShowMonster);
}
void CPlayerAI::HideObject()
{
	//OBJECTMGR->HideAllNpc(OPTIONMGR->GetGameOption()->bShowNpc);
	//OBJECTMGR->HideAllPet(!OPTIONMGR->GetGameOption()->bShowPet);
	//OBJECTMGR->HideAllMonster(OPTIONMGR->GetGameOption()->bShowMonster);

	if (OBJECTMGR->IsNpcHiding() != OPTIONMGR->GetGameOption()->bShowNpc)
		OBJECTMGR->HideAllNpc(OPTIONMGR->GetGameOption()->bShowNpc);
	if (OBJECTMGR->IsPetHiding() != OPTIONMGR->GetGameOption()->bShowPet)
		OBJECTMGR->HideAllPet(!OPTIONMGR->GetGameOption()->bShowPet);
	if (OBJECTMGR->IsMonsterHiding() != OPTIONMGR->GetGameOption()->bShowMonster)
		OBJECTMGR->HideAllMonster(OPTIONMGR->GetGameOption()->bShowMonster);
}
void CPlayerAI::DufualtUsekyungGong2()
{
	if (HERO->IsDied())	 return;
	if (GetNaeRyuk() <= 15) return;
	if (!AISETMGR->GetGameAI()->sPage5.pbAutoLeveling && !REDISCONN->GetStart())return;
	LEVELTYPE kyungGong = HERO->GetLevel();
	CAbility* pAblilty1 = HERO->GetAbilityGroup()->GetAbility(201);
	CAbility* pAblilty2 = HERO->GetAbilityGroup()->GetAbility(204);
	CAbility* pAblilty3 = HERO->GetAbilityGroup()->GetAbility(207);
	if (kyungGong >= 40 && pAblilty3)
	{
		pAblilty3->Use();
		return;
	}
	if (kyungGong >= 25 && kyungGong <= 39 && pAblilty2)
	{
		pAblilty2->Use();
		return;
	}
	if (kyungGong >= 0 && kyungGong <= 24)
	{
		pAblilty1->Use();
		return;
	}
}
void CPlayerAI::CheckKyunkong2()
{
	if (!CheckKyunKong) return;
	if (HERO->GetLevel() == 1)
	{
		HERO->SetKyungGongMode(FALSE);
		DufualtUsekyungGong2();
		CheckKyunKong = FALSE;
		return;
	}
	if (HERO->GetLevel() == 25)
	{
		HERO->SetKyungGongMode(FALSE);
		DufualtUsekyungGong2();
		CheckKyunKong = FALSE;
		return;
	}
	if (HERO->GetLevel() == 40)
	{
		HERO->SetKyungGongMode(FALSE);
		DufualtUsekyungGong2();
		CheckKyunKong = FALSE;
		return;
	}
}
void CPlayerAI::ProcessMussang()
{
	BOOL IsMussang = MUSSANGMGR->IsMussangMode();
	if (IsMussang
		|| HERO == NULL
		|| m_playerstate == player_isin_dazuo
		|| HERO->IsDied()
		)
		return;
	if (!IsMussang
		&&	MUSSANGMGR->IsMussangReady()
		)
		MUSSANGMGR->SendMsgMussangOn();
}
BOOL CPlayerAI::CanEndingDaZuo()
{
	if ((int)HERO->GetState() == eObjectState_Ungijosik)
	{
		if (GetLife() >= AISETMGR->GetGameAI()->sPage5.spSitHP &&
			GetNaeRyuk() == 100 &&
			OBJECTMGR->GetSelectedObject() != NULL
			)
		{
			return TRUE;
		}
	}
	if ((int)HERO->GetState() == eObjectState_Ungijosik)
	{
		if (GetLife() == 100 && GetShield() == 100 && GetNaeRyuk() == 100) return TRUE;
	}
	if (m_MonsterAttrTime>5 && (int)HERO->GetState() == eObjectState_Ungijosik)
	{
		if (GetLife() >= 30 && GetNaeRyuk() == 100)
		{
			m_MonsterAttrTime = 0; return TRUE;
		}
	}
	return FALSE;
}
void CPlayerAI::EndingDaZuo()
{
	dzCurTime = gCurTime;
	if (HERO->IsDied()) return;
	if (dzCurTime - m_TimeForDaZuo<2150) return;
	if ((int)HERO->GetState() != eObjectState_Ungijosik && (m_playerstate == player_isin_dazuo))
	{
		m_playerstate = player_isin_acttact_monster; return;
	}
	if (!CanEndingDaZuo()) return;
	CAbility* pAbility = HERO->GetAbilityGroup()->GetAbility(401);
	if (pAbility == NULL) return;
	if (m_playerstate == player_isin_dazuo)
	{
		pAbility->Use();
		m_playerstate = player_isin_acttact_monster;
		dzCurTime = gCurTime;
		m_ReSelectTime = dzCurTime;
		SkillTime = gCurTime;
		return;
	}
	if ((dzCurTime - m_TimeForDaZuo>2250))
	{
		pAbility->Use();
		m_playerstate = player_isin_acttact_monster;
		dzCurTime = gCurTime;
		m_ReSelectTime = dzCurTime;
		SkillTime = gCurTime;
		return;
	}
	if (pAttacker)
	{
		if ((pAttacker->GetObjectKind() == eObjectKind_Player) && (m_playerstate == player_isin_dazuo))
		{
			pAbility->Use();
			m_playerstate = player_isin_acttact_monster;
			dzCurTime = gCurTime;
			m_ReSelectTime = dzCurTime;
			SkillTime = gCurTime;
			return;
		}
	}
}
void CPlayerAI::ExecSit()
{
	YouStop(HERO);
	if (OBJECTMGR->GetSelectedObject() != NULL &&
		AISETMGR->GetGameAI()->sPage5.cbSitNoTarget)
	{
		g_UserInput.GetHeroMove()->AttackSelectedObject();
		return;
	}
	COptionDialog* pOptionDlg = (COptionDialog*)WINDOWMGR->GetWindowForID(OTI_TABDLG);
	if (!pOptionDlg) return;
	CAbility* pAbility = HERO->GetAbilityGroup()->GetAbility(401);
	if (pAbility == NULL) return;
	OBJECTMGR->SetSelectedObject(NULL);
	if (MOVEMGR->IsMoving(HERO))
	{
		MOVEMGR->HeroMoveStop();
		OBJECTSTATEMGR->EndObjectState(HERO, eObjectState_Move, 0);
	}
	pAbility->Use();
	StartSitFailedCheck();
	m_MonsterAttrTime = 0;
	m_IsNeedSit = FALSE;
}
void CPlayerAI::ProcessAutoPotion()
{
	DWORD curtime = timeGetTime();
	COptionDialog* pOptionDlg = (COptionDialog*)WINDOWMGR->GetWindowForID(OTI_TABDLG);
	if (!pOptionDlg) return;
	if (HERO->IsDied()) return;
	for (int i = 0; i<Protected_Num; i++)
	{
		switch (i)
		{
		case protected_life_first:
		{
			if (AISETMGR->GetGameAI()->sPage7.ProtectLifeOpen1 && (curtime - m_TimeForProtect[protected_life_first]) >= AISETMGR->GetGameAI()->sPage7.ProtectLifeTime1 * 1000)
			{
				DOUBLE d1 = (DOUBLE)HERO->GetLife();
				DOUBLE d2 = (DOUBLE)HERO->DoGetMaxLife();
				UseQuickItem(d1, d2, curtime, i);
			}
		}
		break;
		case protected_neo_first:
		{
			if (AISETMGR->GetGameAI()->sPage7.ProtectMagicOpen1 && (curtime - m_TimeForProtect[protected_neo_first]) >= AISETMGR->GetGameAI()->sPage7.ProtectMagicTime1 * 1000)
			{
				DOUBLE d1 = (DOUBLE)HERO->GetNaeRyuk();
				DOUBLE d2 = (DOUBLE)HERO->DoGetMaxNaeRyuk();
				UseQuickItem(d1, d2, curtime, i);
			}
		}
		break;
		case protected_aco_first:
		{
			if (AISETMGR->GetGameAI()->sPage7.ProtectShieldOpen1 && (curtime - m_TimeForProtect[protected_aco_first]) >= AISETMGR->GetGameAI()->sPage7.ProtectShieldTime1 * 1000)
			{
				DOUBLE d1 = (DOUBLE)HERO->GetShield();
				DOUBLE d2 = (DOUBLE)HERO->DoGetMaxShield();
				UseQuickItem(d1, d2, curtime, i);
			}
		}
		break;
		case protected_life_second:
		{
			if (AISETMGR->GetGameAI()->sPage7.ProtectLifeOpen2 && (curtime - m_TimeForProtect[protected_life_second]) >= AISETMGR->GetGameAI()->sPage7.ProtectLifeTime2 * 1000)
			{
				DOUBLE d1 = (DOUBLE)HERO->GetLife();
				DOUBLE d2 = (DOUBLE)HERO->DoGetMaxLife();
				UseQuickItem(d1, d2, curtime, i);
			}
		}
		break;
		case protected_neo_second:
		{
			if (AISETMGR->GetGameAI()->sPage7.ProtectMagicOpen2 && (curtime - m_TimeForProtect[protected_neo_second]) >= AISETMGR->GetGameAI()->sPage7.ProtectMagicTime2 * 1000)
			{
				DOUBLE d1 = (DOUBLE)HERO->GetNaeRyuk();
				DOUBLE d2 = (DOUBLE)HERO->DoGetMaxNaeRyuk();
				UseQuickItem(d1, d2, curtime, i);
			}
		}
		break;
		case protected_aco_second:
		{
			if (AISETMGR->GetGameAI()->sPage7.ProtectShieldOpen2 && (curtime - m_TimeForProtect[protected_aco_second]) >= AISETMGR->GetGameAI()->sPage7.ProtectShieldTime2 * 1000)
			{
				DOUBLE d1 = (DOUBLE)HERO->GetShield();
				DOUBLE d2 = (DOUBLE)HERO->DoGetMaxShield();
				UseQuickItem(d1, d2, curtime, i);
			}
		}
		break;
		case protected_life_third:
		{
			if (AISETMGR->GetGameAI()->sPage7.ProtectLifeOpen3 && (curtime - m_TimeForProtect[protected_life_third]) >= AISETMGR->GetGameAI()->sPage7.ProtectLifeTime3 * 1000)
			{
				DOUBLE d1 = (DOUBLE)HERO->GetLife();
				DOUBLE d2 = (DOUBLE)HERO->DoGetMaxLife();
				UseQuickItem(d1, d2, curtime, i);
			}
		}
		break;
		}
	}
}
void CPlayerAI::UseQuickItem(DOUBLE minValue, DOUBLE maxValue, DWORD curtime, int type)
{
	COptionDialog* pOptionDlg = (COptionDialog*)WINDOWMGR->GetWindowForID(OTI_TABDLG);
	if (!pOptionDlg) return;
	int j = (minValue / maxValue) * 100;
	if (type == protected_life_first)
	{
		if (j <AISETMGR->GetGameAI()->sPage7.ProtectLifePecent1)
		{
			QUICKMGR->UseQuickItem(AISETMGR->GetGameAI()->sPage7.ProtectLifePage1 - 1, AISETMGR->GetGameAI()->sPage7.ProtectLifeKey1 - 1);
			m_TimeForProtect[protected_life_first] = curtime;
		}
	}
	if (type == protected_neo_first)
	{
		if (j <AISETMGR->GetGameAI()->sPage7.ProtectMagicPecent1)
		{
			QUICKMGR->UseQuickItem(AISETMGR->GetGameAI()->sPage7.ProtectMagicPage1 - 1, AISETMGR->GetGameAI()->sPage7.ProtectMagicKey1 - 1);
			m_TimeForProtect[protected_neo_first] = curtime;
		}
	}
	if (type == protected_aco_first)
	{
		if (j <AISETMGR->GetGameAI()->sPage7.ProtectShieldPecent1)
		{
			QUICKMGR->UseQuickItem(AISETMGR->GetGameAI()->sPage7.ProtectShieldPage1 - 1, AISETMGR->GetGameAI()->sPage7.ProtectShieldKey1 - 1);
			m_TimeForProtect[protected_aco_first] = curtime;
		}
	}
	if (type == protected_life_second)
	{
		if (j <AISETMGR->GetGameAI()->sPage7.ProtectLifePecent2)
		{
			QUICKMGR->UseQuickItem(AISETMGR->GetGameAI()->sPage7.ProtectLifePage2 - 1, AISETMGR->GetGameAI()->sPage7.ProtectLifeKey2 - 1);
			m_TimeForProtect[protected_life_second] = curtime;
		}
	}
	if (type == protected_neo_second)
	{
		if (j <AISETMGR->GetGameAI()->sPage7.ProtectMagicPecent2)
		{
			QUICKMGR->UseQuickItem(AISETMGR->GetGameAI()->sPage7.ProtectMagicPage2 - 1, AISETMGR->GetGameAI()->sPage7.ProtectMagicKey2 - 1);
			m_TimeForProtect[protected_neo_second] = curtime;
		}
	}
	if (type == protected_aco_second)
	{
		if (j <AISETMGR->GetGameAI()->sPage7.ProtectShieldPecent2)
		{
			QUICKMGR->UseQuickItem(AISETMGR->GetGameAI()->sPage7.ProtectShieldPage2 - 1, AISETMGR->GetGameAI()->sPage7.ProtectShieldKey2 - 1);
			m_TimeForProtect[protected_aco_second] = curtime;
		}
	}
	if (type == protected_life_third)
	{
		if (j <AISETMGR->GetGameAI()->sPage7.ProtectLifePecent3)
		{
			QUICKMGR->UseQuickItem(AISETMGR->GetGameAI()->sPage7.ProtectLifePage3 - 1, AISETMGR->GetGameAI()->sPage7.ProtectLifeKey3 - 1);
			m_TimeForProtect[protected_life_third] = curtime;
		}
	}
}
void CPlayerAI::ForceShowReviveDialog()
{
	if (!bReviveDialogIsShowingSwitch) return;
	if (false == GAMEIN->GetReviveDialog()->IsActive() && HERO->IsDied())
	{
		m_bRevProcessing2 = TRUE;
		DWORD dwCurTime = MHTIMEMGR->GetNewCalcCurTime();
		m_dwRevStartTime2 = dwCurTime - 1000;
		m_nCount2 = m_nRevCount2 * 2;
	}
	bReviveDialogIsShowingSwitch = false;
}
#include "MainBarDialog.h"
void CPlayerAI::ProcessLepasMatiBaru()
{
	if (HERO->GetState() == eObjectState_Die)
	{
		if (AISETMGR->GetGameAI()->sPage5.cbAutoRespawn)
		{
			static DWORD dwStaticDieTime;
			if (gCurTime - dwStaticDieTime >= 1000)
			{
				dwStaticDieTime = gCurTime;
				if (m_ReviveCount<40)
				{
					if (!GAMEIN->GetReviveDialog()->IsActive())
						if (bReviveDialogIsShowingSwitch)
							ForceShowReviveDialog();
				}
				if (m_ReviveCount<0)
				{
					WINDOWMGR->CloseAllMsgBoxNoFunc();
					if (GAMEIN->GetReviveDialog()->IsActive())
					{
						if (AISETMGR->GetGameAI()->sPage5.pbSpawnHere)
							CR_DlgBtnFunc(CR_PRESENTSPOT, NULL, 0);
						else
							CR_DlgBtnFunc(CR_LOGINSPOT, NULL, 0);
						GAMEIN->GetMainInterfaceDialog()->SetAlram(OPT_PRESETFORDEAD, false);
					}
					else
					{
						if (bReviveDialogIsShowingSwitch)
							ForceShowReviveDialog();
					}
					m_ReviveCount = 60 * (AISETMGR->GetGameAI()->sPage5.spReviveTime);
				}
				else
				{
					CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(2173), m_ReviveCount);
				}
				--m_ReviveCount;
			}
		}
	}
}
void CPlayerAI::StartAutoAttrPalyProcess(CObject *AttrPlayer)
{
	if (AttrPlayer == NULL) return;
	if (AttrPlayer->GetID() == HEROID) return;
	if (AttrPlayer)
		AttackerID = AttrPlayer->GetID();
	else
		AttackerID = 0;
}
void CPlayerAI::ProcessBelasahPVP()
{
	if (AttackerID == 0) return;
	CPlayer* pPlayer = (CPlayer*)OBJECTMGR->GetObject(AttackerID);
	if (!pPlayer) return;
	if (AISETMGR->GetGameAI()->sPage8.isAutoAttrPlay == FALSE ||
		HERO->GetState() == eObjectState_Deal)
		return;
	if (pPlayer->IsDied() || !((CHero*)pPlayer)->IsPKMode())
	{
		AttackerID = 0;
		m_IsRunWay = true;
		return;
	}
	VECTOR3 herotmppos, AttrPlayerpos;
	HERO->GetPosition(&herotmppos);
	pPlayer->GetPosition(&AttrPlayerpos);
	double distuncetmp = ((AttrPlayerpos.x - vPos.x)*(AttrPlayerpos.x - vPos.x) + (AttrPlayerpos.z - vPos.z)*(AttrPlayerpos.z - vPos.z));
	if (sqrt(distuncetmp) > (vPos, dwRangeArea * 100))
	{
		m_IsRunWay = true;
		OBJECTMGR->SetSelectedObject(NULL);
	}
	else
	{
		m_IsRunWay = false;
		OBJECTMGR->SetSelectedObject(pPlayer);
	}
}
BOOL CPlayerAI::NeedDaZuo()
{
	return FALSE;
}
void CPlayerAI::SetSkillNextFlag(DWORD Flag)
{
	for (int i = 0; i<5; i++)
	{
		if (i == Flag)
		{
			m_SkillNextFlag[i] = 1;
		}
		else
		{
			m_SkillNextFlag[i] = 0;
		}
	}
}
DOUBLE CPlayerAI::GetLife()
{
	return ((DOUBLE)HERO->GetLife() / (DOUBLE)HERO->DoGetMaxLife()) * 100;
}
DOUBLE	CPlayerAI::GetShield()
{
	return ((DOUBLE)HERO->GetShield() / (DOUBLE)HERO->DoGetMaxShield()) * 100;
}
DOUBLE CPlayerAI::GetNaeRyuk()
{
	return ((DOUBLE)HERO->GetNaeRyuk() / (DOUBLE)HERO->DoGetMaxNaeRyuk()) * 100;
}
void CPlayerAI::ProcessDaZuo()
{
	COptionDialog* pOptionDlg = (COptionDialog*)WINDOWMGR->GetWindowForID(OTI_TABDLG);
	if (!pOptionDlg) return;
	CAbility* pAbility = HERO->GetAbilityGroup()->GetAbility(401);
	if (pAbility == NULL) return;
	if (HERO->GetState() != eObjectState_None && HERO->GetState() != eObjectState_Immortal) return;
	if (gCurTime - m_TimeForActtact + 500 < QUICKMGR->GetUserSkillTime()) return;
	OBJECTMGR->SetSelectedObject(NULL);
	if (MOVEMGR->IsMoving(HERO))
	{
		MOVEMGR->HeroMoveStop();
	}
	if (HERO->GetState() == eObjectState_Immortal ||
		HERO->GetState() == eObjectState_None)
	{
		OBJECTSTATEMGR->EndObjectState(HERO, eObjectState_Move, 0);
	}
	pAbility->Use();
	m_MonsterAttrTime = 0;
	m_playerstate = player_isin_dazuo;
	m_TimeForDaZuo = dzCurTime;
	m_ReSelectTime = gCurTime;
}
void CPlayerAI::StartSitFailedCheck()
{
	m_bSitProcessing = TRUE;
	DWORD dwCurTime = MHTIMEMGR->GetNewCalcCurTime();
	m_dwSitStartTime = dwCurTime - 1000;
	m_nCount = m_nSitCount;
}
void CPlayerAI::ProcessSitFailed()
{
	if (!m_bSitProcessing) return;
	if (gCurTime - m_dwSitStartTime >= 1000)
	{
		m_dwSitStartTime += 1000;
		if (m_nCount <= 0)
		{
			m_bSitProcessing = FALSE;
			CAbility* pAbility = HERO->GetAbilityGroup()->GetAbility(401);
			if (pAbility == NULL) return;
			if (sCPlayerAI.IsOpen())
			{
				m_IsNeedSit = FALSE;
				pAbility->Use();
			}
		}
		--m_nCount;
	}
}
void CPlayerAI::YouStop(CObject* pObject)
{
	if (MHTIMEMGR->GetInstance()->CheckTimer2000())
	{
		if (MOVEMGR->IsMoving(pObject) == TRUE)
		{
			MOVEMGR->MoveStop(pObject);
			if (pObject->GetState() != eObjectState_None)
				OBJECTSTATEMGR->EndObjectState(pObject, pObject->GetState());
		}
		CObject * targetObj = OBJECTMGR->GetSelectedObject();
		if (OBJECTMGR->GetSelectedObject() != NULL)
		{
			HERO->ClearTarget(targetObj);
			OBJECTMGR->SetSelectedObject(NULL);
		}
	}
}
void CPlayerAI::ProcessSit()
{
	if (HERO->GetState() == eObjectState_Ungijosik)
	{
		if (!AISETMGR->GetGameAI()->sPage5.cbAutoSitHP &
			!AISETMGR->GetGameAI()->sPage5.cbAutoSitSP &
			!AISETMGR->GetGameAI()->sPage5.cbAutoSitMP
			)
		{
			m_IsNeedSit = TRUE;
			m_StopPlease = true;
			ExecSit();
			return;
		}
		if (AISETMGR->GetGameAI()->sPage5.cbSitAttacked)
		{
			if (m_MonsterAttrTime >= AISETMGR->GetGameAI()->sPage5.spSitAttacked)
			{
				CAbility* pAbility = HERO->GetAbilityGroup()->GetAbility(401);
				if (pAbility == NULL)
				{
					return;
				}
				while (TRUE)
				{
					m_IsNeedSit = FALSE;
					pAbility->Use();
					break;
				}
				m_MonsterAttrTime = 0;
				return;
			}
		}
		DOUBLE L1, L2, M1, M2, S1, S2;
		L1 = (DOUBLE)HERO->GetLife();
		L2 = (DOUBLE)HERO->DoGetMaxLife();
		M1 = (DOUBLE)HERO->GetNaeRyuk();
		M2 = (DOUBLE)HERO->DoGetMaxNaeRyuk();
		S1 = (DOUBLE)HERO->GetShield();
		S2 = (DOUBLE)HERO->DoGetMaxShield();
		if (L1 >= L2 && M1 >= M2 && S1 >= S2)
		{
			CAbility* pAbility = HERO->GetAbilityGroup()->GetAbility(401);
			if (pAbility == NULL) return;
			while (TRUE)
			{
				m_IsNeedSit = FALSE;
				m_StopPlease = false;
				pAbility->Use();
				break;
			}
		}
	}
	else
	{
		DOUBLE d1, d2;
		DOUBLE tmtpmt;
		if (AISETMGR->GetGameAI()->sPage5.cbAutoSitHP)
		{
			d1 = (DOUBLE)HERO->GetLife();
			d2 = (DOUBLE)HERO->DoGetMaxLife();
			tmtpmt = (d1 / d2) * 100;
			if (tmtpmt <AISETMGR->GetGameAI()->sPage5.spSitHP)
			{
				m_IsNeedSit = TRUE;
				m_StopPlease = true;
				ExecSit();
			}
		}
		if (AISETMGR->GetGameAI()->sPage5.cbAutoSitMP)
		{
			d1 = (DOUBLE)HERO->GetNaeRyuk();
			d2 = (DOUBLE)HERO->DoGetMaxNaeRyuk();
			tmtpmt = (d1 / d2) * 100;
			if (tmtpmt <AISETMGR->GetGameAI()->sPage5.spSitMP)
			{
				m_IsNeedSit = TRUE;
				m_StopPlease = true;
				ExecSit();
			}
		}
		if (AISETMGR->GetGameAI()->sPage5.cbAutoSitSP)
		{
			d1 = (DOUBLE)HERO->GetShield();
			d2 = (DOUBLE)HERO->DoGetMaxShield();
			tmtpmt = (d1 / d2) * 100;
			if (tmtpmt <AISETMGR->GetGameAI()->sPage5.spSitSP)
			{
				m_IsNeedSit = TRUE;
				m_StopPlease = true;
				ExecSit();
			}
		}
	}
}
BOOL CPlayerAI::PlayerUseSkill(CSkillInfo* pInfo)
{
	if (HERO->GetCharacterTotalInfo()->bVisible == FALSE) return FALSE;
	CActionTarget Target;
	if (!pInfo)				return FALSE;
	WORD wSkillKind = pInfo->GetSkillKind();
	if ((HERO->InTitan()) && (SKILLMGR->CheckSkillKind(wSkillKind) == FALSE))
	{
		pInfo = SKILLMGR->GetSkillInfo(pInfo->GetSkillIndex());
	}
	if (pInfo->GetSkillInfo()->TargetKind == 0)
	{
		CObject * targetObj = OBJECTMGR->GetSelectedObject();
		if (targetObj)
		{
			if (targetObj->IsDied() || targetObj->GetState() == eObjectState_Die) return FALSE;
			Target.InitActionTarget(targetObj, NULL);
			if (SKILLMGR->CheckSkillKind(wSkillKind) == TRUE)
			{
				HERO->DisableAutoAttack();
			}
		}
		else
		{
			CHATMGR->AddMsgFade(CTC_SYSMSG, CHATMGR->GetChatMsg(400));
			return FALSE;
		}
	}
	else if (pInfo && pInfo->GetSkillInfo()->TargetKind == 1)
	{
		Target.InitActionTarget(HERO, NULL);
	}
	if (pInfo && !pInfo->GetSpecialState())
	{
		if (HERO->GetSingleSpecialState(eSingleSpecialState_Hide))
		{
			CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(1329)); return FALSE;
		}
	}
	SKILLMGR->SetSkill(pInfo);
	return SKILLMGR->OnSkillCommand(HERO, &Target, TRUE);
}
BOOL CPlayerAI::FindSkillStatusFLg(DWORD dwFlg, CMonster* pMonster)
{
	if (!pMonster) return FALSE;
	cPtrList& cPtrL = pMonster->GetSkillStatusList();
	PTRLISTPOS pos = cPtrL.GetHeadPosition();
	while (pos)
	{
		CStatus* pData = (CStatus*)cPtrL.GetNext(pos);
		if (pData->GetFlag() == dwFlg) return TRUE;
	}
	return FALSE;
}
#include "MovePoint.h"
void CPlayerAI::CheckAreaProcess()
{
	if (vPos.x == 0 || vPos.z == 0) return;
	if (m_StopPlease) return;
	if (HERO->IsAutoAttacking()) return;
	if (m_IsNeedSit)
	{
		ExecSit();
		return;
	}
	if (OBJECTMGR->GetSelectedObject() == NULL)
		if (MHTIMEMGR->CheckTimer1000())
		{
			float dist = CalcDistanceXZ(&vPos, &HERO->GetCurPosition());
			if (dist > (vPos, dwRangeArea * 100))
			{
				if (FALSE == HERO->GetBaseMoveInfo()->bMoving)
				{
					HERO->m_WayPointManager->SetFlagDestination(vPos);
					if (MOVEMGR->SetHeroTarget(&vPos, 1.f))
					{
						MAP->CollisionCheck_OneTile(&vPos);
					}
				}
				ReBuildIsMovePoint = FALSE;
				m_NoAction = FALSE;
			}
			else
			{
				if (OBJECTMGR->GetSelectedObject() == NULL)
				{
					if (b > 5)
					{
						CheckAreaProcessSmallPoint();
						b = 1;
					}
				}
			}
		}
}
void CPlayerAI::CheckAreaProcessSmallPoint()
{
	float dist = CalcDistanceXZ(&vPos, &HERO->GetCurPosition());
	if (dist > (vPos, 5 * 100))
	{
		if (FALSE == HERO->GetBaseMoveInfo()->bMoving)
		{
			HERO->m_WayPointManager->SetFlagDestination(vPos);
			if (MOVEMGR->SetHeroTarget(&vPos, 1.f))
			{
				MAP->CollisionCheck_OneTile(&vPos);
			}
		}
		ReBuildIsMovePoint = FALSE;
		m_NoAction = FALSE;
	}
}
void CPlayerAI::SetPoint()
{
	COptionDialog * pDlg = GAMEIN->GetOptionDialog();
	cDialog* m_Dlg = (cDialog*)pDlg->GetTabSheet(4);
	if (m_Dlg)
	{
		AISETMGR->GetGameAI()->sPage5.spPosX = (int)(HERO->GetCurPosition().x / 100);
		AISETMGR->GetGameAI()->sPage5.spPosY = (int)(HERO->GetCurPosition().z / 100);
		char TempPosX[10];
		char TempPosZ[10];
		itoa(AISETMGR->GetGameAI()->sPage5.spPosX, TempPosX, 10);
		itoa(AISETMGR->GetGameAI()->sPage5.spPosY, TempPosZ, 10);
		((cSpin*)m_Dlg->GetWindowForID(OTI_SP_AUTOHUNT_LOCA_X))->SetEditText(TempPosX);
		((cSpin*)m_Dlg->GetWindowForID(OTI_SP_AUTOHUNT_LOCA_Y))->SetEditText(TempPosZ);
	}
}
void CPlayerAI::SetAIPosition()
{
	OBJECTMGR->SetSelectedObject(NULL);
	VECTOR3	TempPos;
	HERO->GetPosition(&TempPos + 10);
	MOVEMGR->SetHeroTarget(&TempPos, FALSE);
	m_IsNeedSit = FALSE;
	MOVEMGR->HeroMoveStop();
	m_StopPlease = false;
}
void CPlayerAI::ToggleAIKey()
{
	if (sCPlayerAI.IsOpen())
	{
		sCPlayerAI.SetAIDisable();
	}
	else
	{
		sCPlayerAI.SetAIEnable();
	}
}
void CPlayerAI::SaveTemp()
{
	bReviveDialogIsShowingSwitch = true;
	bReviveDialogIsShowing = false;
	dwReviveDialogTimeTick = 0;
	m_MonsterAttrTime = 0;
	m_DeathCount = 0;
	m_ReviveCount = 60 * (AISETMGR->GetGameAI()->sPage5.spReviveTime);
	m_StopPlease = false;
	m_IsNeedSit = false;
	m_IsNeedSit = FALSE;
	if (AISETMGR->GetGameAI()->sPage5.cbAreaXY && AISETMGR->FindErrorPathFromAISet())
		CHATMGR->AddMsg(CTC_CHEAT_1, CHATMGR->GetChatMsg(2639));
	if (AISETMGR->GetGameAI()->sPage5.cbAreaXY)
	{
		vPos.x = (float)AISETMGR->GetGameAI()->sPage5.spPosX * 100;
		vPos.z = (float)AISETMGR->GetGameAI()->sPage5.spPosY * 100;
	}
	else
	{
		HERO->GetPosition(&vPos);
	}
	if (MAP->CollisionCheck_OneTile(&vPos) == FALSE && AISETMGR->GetGameAI()->sPage5.cbAreaXY)
	{
		HERO->GetPosition(&vPos);
	}
	if (GAMEIN->GetGameInInitKind() == eGameInInitKind_SuryunEnter)
	{
		if (AISETMGR->GetGameAI()->sPage5.cbAreaXY)
		{
			AISETMGR->GetGameAI()->sPage5.cbAreaXY = false;
			HERO->GetPosition(&vPos);
		}
	}
	InitRangeArea();
}
void CPlayerAI::UsePhAndSx(CMonster* pMonster, double distance1)
{
	if (HERO->GetBaseMoveInfo()->bMoving)
		return;
	if (AISETMGR->GetGameAI()->sPage6.bHelpSkill1)
	{
		if (HERO->GetLevel() < 5) return;
		BOOL Phflg = FindSkillStatusFLg(256, pMonster);
		CMugongBase* pMugongBasePh = MUGONGMGR->GetMugongByMugongIdx(1202);
		if (!Phflg && pMugongBasePh)
		{
			m_StopPlease = !NewTeran(1202);
		}
	}
	if (AISETMGR->GetGameAI()->sPage6.bHelpSkill2)
	{
		BOOL Sxflg = FindSkillStatusFLg(4096, pMonster);
		CMugongBase* pMugongBaseSx = MUGONGMGR->GetMugongByMugongIdx(1203);
		if (!Sxflg && pMugongBaseSx)
		{
			m_StopPlease = !NewTeran(1203);
		}
	}
	m_StopPlease = false;
}
void CPlayerAI::UseTuNa()
{
	if (!AISETMGR->GetGameAI()->sPage6.bHelpSkill10) return;
	if (gCurTime - m_TimeForTuna < 3000) return;
	if (HERO->GetLevel()<5)
	{
		AISETMGR->GetGameAI()->sPage6.bHelpSkill10 = FALSE;
		return;
	}
	CMugongBase*pMugongBaseTL = sCPlayerAI.CheckHeightOrLow(900, 910);
	if (pMugongBaseTL)
	{
		sCPlayerAI.NewTeran(pMugongBaseTL->GetItemIdx());
	}
}
void CPlayerAI::ProcessSkillGelung()
{
	if (HERO->GetState() == eObjectState_Ungijosik) return;
	if (HERO->IsDied()) return;
	if (HERO->GetLevel()<5) return;
	DWORD cCurTime = time(NULL);
	static DWORD dwTempTime;
	if (gCurTime - dwTempTime>400)
	{
		dwTempTime = gCurTime;
		if (AISETMGR->GetGameAI()->sPage6.bHelpSkill9)
		{
			CMugongBase*pMugongBaseWZ = CheckHeightOrLow(60, 61);
			if (pMugongBaseWZ)
			{
				if ((cCurTime - sCPlayerAI.m_SkillSet.m_HelpSkillSet.m_SkillWzTime)>1)
				{
					if (!STATUSICONDLG->CheckStatusIcon(eStatusIcon_SkipEffect))
					{
						m_StopPlease = !NewTeran(pMugongBaseWZ->GetItemIdx());
					}
					else m_StopPlease = false;
					sCPlayerAI.m_SkillSet.m_HelpSkillSet.m_SkillWzTime = time(NULL);
				}
			}
		}
		if (HERO->GetStage() == 128 || HERO->GetStage() == 129)
		{
			if (AISETMGR->GetGameAI()->sPage8.m_AutoSkill1)
			{
				CMugongBase* pUseBaHuan = CheckHeightOrLowTeran(5055, 5035, 5015, 1279, 1259, 1239, 1219);
				if (pUseBaHuan)
				{
					if ((cCurTime - sCPlayerAI.m_SkillSet.m_HelpSkillSet.m_AutoSkill1)>1)
					{
						if (!STATUSICONDLG->CheckStatusIcon(34))
						{
							m_StopPlease = !NewTeran(pUseBaHuan->GetItemIdx());
						}
						else m_StopPlease = false;
						sCPlayerAI.m_SkillSet.m_HelpSkillSet.m_AutoSkill1 = time(NULL);
					}
				}
			}
			if (AISETMGR->GetGameAI()->sPage8.m_AutoSkill2)
			{
				CMugongBase* pUseLongXiang = CheckHeightOrLowTeran(5043, 5023, 5003, 1267, 1247, 1227, 1207);
				if (pUseLongXiang)
				{
					if ((cCurTime - sCPlayerAI.m_SkillSet.m_HelpSkillSet.m_AutoSkill2)>1)
					{
						if (!STATUSICONDLG->CheckStatusIcon(40))
						{
							m_StopPlease = !NewTeran(pUseLongXiang->GetItemIdx());
						}
						else m_StopPlease = false;
						sCPlayerAI.m_SkillSet.m_HelpSkillSet.m_AutoSkill2 = time(NULL);
					}
				}
			}
			if (AISETMGR->GetGameAI()->sPage8.m_AutoSkill3)
			{
				CMugongBase* pUseJiuYing = CheckHeightOrLowTeran(5059, 5039, 5019, 1283, 1263, 1243, 1223);
				if (pUseJiuYing)
				{
					if ((cCurTime - sCPlayerAI.m_SkillSet.m_HelpSkillSet.m_AutoSkill3)>1)
					{
						if (!STATUSICONDLG->CheckStatusIcon(36))
						{
							m_StopPlease = !NewTeran(pUseJiuYing->GetItemIdx());
						}
						else m_StopPlease = false;
						sCPlayerAI.m_SkillSet.m_HelpSkillSet.m_AutoSkill3 = time(NULL);
					}
				}
			}
			if (AISETMGR->GetGameAI()->sPage8.m_AutoSkill4)
			{
				CMugongBase*pUseHama = CheckHeightOrLowTeran(5041, 5021, 5001, 1265, 1245, 1225, 1205);
				if (pUseHama)
				{
					if ((cCurTime - sCPlayerAI.m_SkillSet.m_HelpSkillSet.m_AutoSkill4)>1)
					{
						if (!STATUSICONDLG->CheckStatusIcon(39))
						{
							m_StopPlease = !NewTeran(pUseHama->GetItemIdx());
						}
						else m_StopPlease = false;
						sCPlayerAI.m_SkillSet.m_HelpSkillSet.m_AutoSkill4 = time(NULL);
					}
				}
			}
			if (AISETMGR->GetGameAI()->sPage8.m_AutoSkill5)
			{
				CMugongBase* pUseTianMo = CheckHeightOrLowTeran(5057, 5037, 5017, 1281, 1261, 1241, 1221);
				if (pUseTianMo)
				{
					if ((cCurTime - sCPlayerAI.m_SkillSet.m_HelpSkillSet.m_AutoSkill5)>1)
					{
						if (!STATUSICONDLG->CheckStatusIcon(35))
						{
							m_StopPlease = !NewTeran(pUseTianMo->GetItemIdx());
						}
						else m_StopPlease = false;
						sCPlayerAI.m_SkillSet.m_HelpSkillSet.m_AutoSkill5 = time(NULL);
					}
				}
			}
			if (AISETMGR->GetGameAI()->sPage8.m_AutoSkill6)
			{
				CMugongBase* pUseXiXing = CheckHeightOrLowTeran(5047, 5027, 5007, 1271, 1251, 1231, 1211);
				if (pUseXiXing)
				{
					if ((cCurTime - sCPlayerAI.m_SkillSet.m_HelpSkillSet.m_AutoSkill6)>1)
					{
						if (!STATUSICONDLG->CheckStatusIcon(45))
						{
							m_StopPlease = !NewTeran(pUseXiXing->GetItemIdx());
						}
						else m_StopPlease = false;
						sCPlayerAI.m_SkillSet.m_HelpSkillSet.m_AutoSkill6 = time(NULL);
					}
				}
			}
		}
		if (HERO->GetStage() == 64 || HERO->GetStage() == 65)
		{
			if (AISETMGR->GetGameAI()->sPage8.h_AutoSkill1)
			{
				CMugongBase* pUseBaHuan = CheckHeightOrLowTeran(5054, 5034, 5014, 1278, 1258, 1238, 1218);
				if (pUseBaHuan)
				{
					if ((cCurTime - sCPlayerAI.m_SkillSet.m_HelpSkillSet.h_AutoSkill1)>1)
					{
						if (!STATUSICONDLG->CheckStatusIcon(34))
						{
							m_StopPlease = !NewTeran(pUseBaHuan->GetItemIdx());
						}
						else m_StopPlease = false;
						sCPlayerAI.m_SkillSet.m_HelpSkillSet.h_AutoSkill1 = time(NULL);
					}
				}
			}
			if (AISETMGR->GetGameAI()->sPage8.h_AutoSkill2)
			{
				CMugongBase* pUseLongXiang = CheckHeightOrLowTeran(5042, 5022, 5002, 1266, 1246, 1226, 1206);
				if (pUseLongXiang)
				{
					if ((cCurTime - sCPlayerAI.m_SkillSet.m_HelpSkillSet.h_AutoSkill2)>1)
					{
						if (!STATUSICONDLG->CheckStatusIcon(40))
						{
							m_StopPlease = !NewTeran(pUseLongXiang->GetItemIdx());
						}
						else m_StopPlease = false;
						sCPlayerAI.m_SkillSet.m_HelpSkillSet.h_AutoSkill2 = time(NULL);
					}
				}
			}
			if (AISETMGR->GetGameAI()->sPage8.h_AutoSkill3)
			{
				CMugongBase* pUseJiuYing = CheckHeightOrLowTeran(5058, 5038, 5018, 1282, 1262, 1242, 1222);
				if (pUseJiuYing)
				{
					if ((cCurTime - sCPlayerAI.m_SkillSet.m_HelpSkillSet.h_AutoSkill3)>1)
					{
						if (!STATUSICONDLG->CheckStatusIcon(36))
						{
							m_StopPlease = !NewTeran(pUseJiuYing->GetItemIdx());
						}
						else m_StopPlease = false;
						sCPlayerAI.m_SkillSet.m_HelpSkillSet.h_AutoSkill3 = time(NULL);
					}
				}
			}
			if (AISETMGR->GetGameAI()->sPage8.h_AutoSkill4)
			{
				CMugongBase*pUseHama = CheckHeightOrLowTeran(5056, 5036, 5016, 1280, 1260, 1240, 1220);
				if (pUseHama)
				{
					if ((cCurTime - sCPlayerAI.m_SkillSet.m_HelpSkillSet.h_AutoSkill4)>1)
					{
						if (!STATUSICONDLG->CheckStatusIcon(35))
						{
							m_StopPlease = !NewTeran(pUseHama->GetItemIdx());
						}
						else m_StopPlease = false;
						sCPlayerAI.m_SkillSet.m_HelpSkillSet.h_AutoSkill4 = time(NULL);
					}
				}
			}
			if (AISETMGR->GetGameAI()->sPage8.h_AutoSkill5)
			{
				CMugongBase* pUseTianMo = CheckHeightOrLowTeran(5046, 5026, 5006, 1270, 1250, 1230, 1210);
				if (pUseTianMo)
				{
					if ((cCurTime - sCPlayerAI.m_SkillSet.m_HelpSkillSet.h_AutoSkill5)>1)
					{
						if (!STATUSICONDLG->CheckStatusIcon(37))
						{
							m_StopPlease = !NewTeran(pUseTianMo->GetItemIdx());
						}
						else m_StopPlease = false;
						sCPlayerAI.m_SkillSet.m_HelpSkillSet.h_AutoSkill5 = time(NULL);
					}
				}
			}
			if (AISETMGR->GetGameAI()->sPage8.h_AutoSkill6)
			{
				CMugongBase* pUseXiXing = CheckHeightOrLowTeran(5040, 5020, 5000, 1264, 1244, 1224, 1204);
				if (pUseXiXing)
				{
					if ((cCurTime - sCPlayerAI.m_SkillSet.m_HelpSkillSet.h_AutoSkill6)>1)
					{
						if (!STATUSICONDLG->CheckStatusIcon(39))
						{
							m_StopPlease = !NewTeran(pUseXiXing->GetItemIdx());
						}
						else m_StopPlease = false;
						sCPlayerAI.m_SkillSet.m_HelpSkillSet.h_AutoSkill6 = time(NULL);
					}
				}
			}
		}
		if (AISETMGR->GetGameAI()->sPage6.bHelpSkill3)
		{
			CMugongBase*pMugongBaseZX = CheckHeightOrLow(702, 712);
			if (pMugongBaseZX)
			{
				if ((cCurTime - sCPlayerAI.m_SkillSet.m_HelpSkillSet.SkillZXTime)>1)
				{
					if (!STATUSICONDLG->CheckStatusIcon(eStatusIcon_AttrCounterFire))
					{
						m_StopPlease = !NewTeran(pMugongBaseZX->GetItemIdx());
					}
					else m_StopPlease = false;
					sCPlayerAI.m_SkillSet.m_HelpSkillSet.SkillZXTime = time(NULL);
				}
			}
		}
		if (AISETMGR->GetGameAI()->sPage6.bHelpSkill4)
		{
			CMugongBase*pMugongBaseHB = CheckHeightOrLow(802, 812);
			if (pMugongBaseHB)
			{
				if ((cCurTime - sCPlayerAI.m_SkillSet.m_HelpSkillSet.SkillHBTime)>1)
				{
					if (!STATUSICONDLG->CheckStatusIcon(eStatusIcon_AttrCounterWater))
					{
						m_StopPlease = !NewTeran(pMugongBaseHB->GetItemIdx());
					}
					else m_StopPlease = false;
					sCPlayerAI.m_SkillSet.m_HelpSkillSet.SkillHBTime = time(NULL);
				}
			}
		}
		if (AISETMGR->GetGameAI()->sPage6.bHelpSkill5)
		{
			CMugongBase*pMugongBaseYH = CheckHeightOrLow(902, 911);
			if (pMugongBaseYH)
			{
				if ((cCurTime - sCPlayerAI.m_SkillSet.m_HelpSkillSet.SkillYHTime)>1)
				{
					if (!STATUSICONDLG->CheckStatusIcon(eStatusIcon_AttrCounterTree))
					{
						m_StopPlease = !NewTeran(pMugongBaseYH->GetItemIdx());
					}
					else m_StopPlease = false;
					sCPlayerAI.m_SkillSet.m_HelpSkillSet.SkillYHTime = time(NULL);
				}
			}
		}
		if (AISETMGR->GetGameAI()->sPage6.bHelpSkill6)
		{
			CMugongBase*pMugongBasePL = CheckHeightOrLow(1002, 1012);
			if (pMugongBasePL)
			{
				if ((cCurTime - sCPlayerAI.m_SkillSet.m_HelpSkillSet.SkillPLTime)>1)
				{
					if (!STATUSICONDLG->CheckStatusIcon(eStatusIcon_AttrCounterIron))
					{
						m_StopPlease = !NewTeran(pMugongBasePL->GetItemIdx());
					}
					else m_StopPlease = false;
					sCPlayerAI.m_SkillSet.m_HelpSkillSet.SkillPLTime = time(NULL);
				}
			}
		}
		if (AISETMGR->GetGameAI()->sPage6.bHelpSkill7)
		{
			CMugongBase*pMugongBaseCY = CheckHeightOrLow(1102, 1112);
			if (pMugongBaseCY)
			{
				if ((cCurTime - sCPlayerAI.m_SkillSet.m_HelpSkillSet.SkillCYTime)>1)
				{
					if (!STATUSICONDLG->CheckStatusIcon(eStatusIcon_AttrCounterEarth))
					{
						m_StopPlease = !NewTeran(pMugongBaseCY->GetItemIdx());
					}
					else m_StopPlease = false;
					sCPlayerAI.m_SkillSet.m_HelpSkillSet.SkillCYTime = time(NULL);
				}
			}
		}
		if (AISETMGR->GetGameAI()->sPage6.bHelpSkill8)
		{
			CMugongBase*pMugongBaseJZ = CheckHeightOrLow(1100, 1110);
			if (pMugongBaseJZ)
			{
				if ((cCurTime - sCPlayerAI.m_SkillSet.m_HelpSkillSet.SkillJZTime)>1)
				{
					if (!STATUSICONDLG->CheckStatusIcon(eStatusIcon_PhyDefenceUp))
					{
						m_StopPlease = !NewTeran(pMugongBaseJZ->GetItemIdx());
					}
					else m_StopPlease = false;
					sCPlayerAI.m_SkillSet.m_HelpSkillSet.SkillJZTime = time(NULL);
				}
			}
		}
		if (AISETMGR->GetGameAI()->sPage8.FuoMoo && HERO->GetPartyIdx() != 0)
		{
			CMugongBase*pMugongBaseFUMO = CheckHeightOrLow(2013, 2013);
			if (pMugongBaseFUMO)
			{
				if ((cCurTime - sCPlayerAI.m_SkillSet.m_HelpSkillSet.SkillFUMOTime)>1)
				{
					if (!STATUSICONDLG->CheckStatusIcon(eStatusIcon_PhyAttackUp))
					{
						m_StopPlease = NewTeran(pMugongBaseFUMO->GetItemIdx());
					}
					else m_StopPlease = false;
					sCPlayerAI.m_SkillSet.m_HelpSkillSet.SkillFUMOTime = time(NULL);
				}
			}
		}
		if (AISETMGR->GetGameAI()->sPage6.bHelpSkill10)
		{
			CMugongBase*pMugongBaseTL = CheckHeightOrLow(900, 910);
			if (pMugongBaseTL)
			{
				DOUBLE d1 = (DOUBLE)HERO->GetLife();
				DOUBLE d2 = (DOUBLE)HERO->DoGetMaxLife();
				DOUBLE tmpdouble = (d1 / d2) * 100;
				if (tmpdouble<AISETMGR->GetGameAI()->sPage6.bTunaSpin)
				{
					m_StopPlease = !NewTeran(pMugongBaseTL->GetItemIdx());
				}
				else m_StopPlease = false;
				sCPlayerAI.m_SkillSet.m_HelpSkillSet.SkillTLTime = time(NULL);
			}
		}
	}
}
CMugongBase * CPlayerAI::CheckHeightOrLow(int SkillIdx1, int SkillIdx2)
{
	CMugongBase* pMugongBase1 = MUGONGMGR->GetMugongByMugongIdx(SkillIdx1);
	CMugongBase* pMugongBase2 = MUGONGMGR->GetMugongByMugongIdx(SkillIdx2);
	if (pMugongBase1&&pMugongBase2 || !pMugongBase1 && pMugongBase2)
	{
		return pMugongBase2;
	}
	else if (pMugongBase1)
	{
		return pMugongBase1;
	}
	else
	{
		return NULL;
	}
}
CMugongBase * CPlayerAI::CheckHeightOrLowTeran(int SkillIdx1, int SkillIdx2, int SkillIdx3, int SkillIdx4, int SkillIdx5, int SkillIdx6, int SkillIdx7)
{
	CMugongBase* pMugongBase1 = MUGONGMGR->GetMugongByMugongIdx(SkillIdx1);
	CMugongBase* pMugongBase2 = MUGONGMGR->GetMugongByMugongIdx(SkillIdx2);
	CMugongBase* pMugongBase3 = MUGONGMGR->GetMugongByMugongIdx(SkillIdx3);
	CMugongBase* pMugongBase4 = MUGONGMGR->GetMugongByMugongIdx(SkillIdx4);
	CMugongBase* pMugongBase5 = MUGONGMGR->GetMugongByMugongIdx(SkillIdx5);
	CMugongBase* pMugongBase6 = MUGONGMGR->GetMugongByMugongIdx(SkillIdx6);
	CMugongBase* pMugongBase7 = MUGONGMGR->GetMugongByMugongIdx(SkillIdx7);
	if (pMugongBase1)
	{
		CSkillInfo* SkillInfo = NULL;
		SkillInfo = SKILLMGR->GetSkillInfo(pMugongBase1->GetSkillInfo()->GetSkillIndex());
		if (SkillInfo&&HERO->GetLevel() >= SkillInfo->GetSkillInfo()->RestrictLevel)
		{
			return pMugongBase1;
		}
		return NULL;
	}
	if (pMugongBase2)
	{
		CSkillInfo* SkillInfo = NULL;
		SkillInfo = SKILLMGR->GetSkillInfo(pMugongBase2->GetSkillInfo()->GetSkillIndex());
		if (SkillInfo&&HERO->GetLevel() >= SkillInfo->GetSkillInfo()->RestrictLevel)
		{
			return pMugongBase2;
		}
		return NULL;
	}
	if (pMugongBase3)
	{
		CSkillInfo* SkillInfo = NULL;
		SkillInfo = SKILLMGR->GetSkillInfo(pMugongBase3->GetSkillInfo()->GetSkillIndex());
		if (SkillInfo&&HERO->GetLevel() >= SkillInfo->GetSkillInfo()->RestrictLevel)
		{
			return pMugongBase3;
		}
		return NULL;
	}
	if (pMugongBase4)
	{
		CSkillInfo* SkillInfo = NULL;
		SkillInfo = SKILLMGR->GetSkillInfo(pMugongBase4->GetSkillInfo()->GetSkillIndex());
		if (SkillInfo&&HERO->GetLevel() >= SkillInfo->GetSkillInfo()->RestrictLevel)
		{
			return pMugongBase4;
		}
		return NULL;
	}
	if (pMugongBase5)
	{
		CSkillInfo* SkillInfo = NULL;
		SkillInfo = SKILLMGR->GetSkillInfo(pMugongBase5->GetSkillInfo()->GetSkillIndex());
		if (SkillInfo&&HERO->GetLevel() >= SkillInfo->GetSkillInfo()->RestrictLevel)
		{
			return pMugongBase5;
		}
		return NULL;
	}
	if (pMugongBase6)
	{
		CSkillInfo* SkillInfo = NULL;
		SkillInfo = SKILLMGR->GetSkillInfo(pMugongBase6->GetSkillInfo()->GetSkillIndex());
		if (SkillInfo&&HERO->GetLevel() >= SkillInfo->GetSkillInfo()->RestrictLevel)
		{
			return pMugongBase6;
		}
		return NULL;
	}
	if (pMugongBase7)
	{
		CSkillInfo* SkillInfo = NULL;
		SkillInfo = SKILLMGR->GetSkillInfo(pMugongBase7->GetSkillInfo()->GetSkillIndex());
		if (SkillInfo&&HERO->GetLevel() >= SkillInfo->GetSkillInfo()->RestrictLevel)
		{
			return pMugongBase7;
		}
		return NULL;
	}
	else
	{
		return NULL;
	}
	return NULL;
}
CMugongBase * CPlayerAI::CheckHeightOrLowForOptionDlg(int SkillIdx1, int SkillIdx2)
{
	CMugongBase* pMugongBase1 = MUGONGMGR->GetMugongByMugongIdx(SkillIdx1);
	CMugongBase* pMugongBase2 = MUGONGMGR->GetMugongByMugongIdx(SkillIdx2);
	CMugongManager::eMUGONG_CANNOT_REASON re;
	re = MUGONGMGR->CanUse(pMugongBase1);
	if (re == CMugongManager::eMCUR_SungZero)
	{
		return NULL;
	}
	re = MUGONGMGR->CanUse(pMugongBase2);
	if (re == CMugongManager::eMCUR_SungZero)
	{
		return NULL;
	}
	if (pMugongBase1&&pMugongBase2 || !pMugongBase1 && pMugongBase2)
	{
		return pMugongBase2;
	}
	else if (pMugongBase1)
	{
		return pMugongBase1;
	}
	else
	{
		return NULL;
	}
}
void CPlayerAI::UseMuGongSkill(DWORD MugongIdx)
{
	CMugongDialog * p_MuGongDialog = GAMEIN->GetMugongDialog();
	if (p_MuGongDialog)
	{
		CMugongBase * p_Base = NULL;
		for (int i = 600; i<MaxMugongSlot; i++)
		{
			p_Base = p_MuGongDialog->GetMugongAbs(i);
			if (p_Base)
			{
				if (p_Base->GetItemIdx() == MugongIdx)
				{
					p_MuGongDialog->ExcuteMugong(i);
					MOVEMGR->SetFindPath();
					break;
				}
			}
		}
	}
}
void CPlayerAI::InitRangeArea()
{
	if (GAMEIN->GetGameInInitKind() == eGameInInitKind_SuryunEnter)
	{
		dwRangeArea = 25;
	}
	else
	{
		dwRangeArea = AISETMGR->GetGameAI()->sPage5.spRange;
	}
}
#include "GuildManager.h"  
#include "GuildUnion.h"  
BOOL CPlayerAI::IsEventAttackable(CObject* pTargetObj)
{
	if (HERO->GetWeaponEquipType() == WP_EVENT)
	{
		CItem* pItem = GAMEIN->GetInventoryDialog()->GetPriorityItemForCristmasEvent();
		if (!pItem)
		{
			CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(583));
			return	FALSE;
		}
	}
	else
	{
		if (pTargetObj->GetObjectKind() & eObjectKind_Monster)
		{
			CMonster* pMonster = (CMonster*)pTargetObj;
			if (pMonster->GetMonsterKind() == EVENT_MONSTER_SANTA1 || pMonster->GetMonsterKind() == EVENT_MONSTER_SANTA2
				|| pMonster->GetMonsterKind() == EVENT_FIELDBOSS_SNOWMAN_SM || pMonster->GetMonsterKind() == EVENT_FIELDSUB_SNOWMAN_SM
				|| pMonster->GetMonsterKind() == EVENT_FIELDBOSS_SNOWMAN_MD || pMonster->GetMonsterKind() == EVENT_FIELDSUB_SNOWMAN_MD
				|| pMonster->GetMonsterKind() == EVENT_FIELDBOSS_SNOWMAN_LG || pMonster->GetMonsterKind() == EVENT_FIELDSUB_SNOWMAN_LG
				)
			{
				CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(584));
				return FALSE;
			}
		}
	}
	return TRUE;
}
void CPlayerAI::OnAttack(CObject* pTargetObj, bool bMugong)
{
	static CObject* pLastObj = NULL;
	static DWORD	dwAttackTime = 0;
	if (HERO && pTargetObj)
	{
		if (HERO->GetCharacterTotalInfo()->bVisible)
		{
			if (pLastObj != pTargetObj || gCurTime - dwAttackTime > 500)
			{
				if (pTargetObj->GetState() == eObjectState_Die)
					return;
				if (HERO->GetSingleSpecialState(eSingleSpecialState_Hide))
				{
					CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(1329));
					return;
				}
				if (!IsEventAttackable(pTargetObj))
					return;
				if (HERO->IsPKMode())
				{
					if (PARTYMGR->IsPartyMember(pTargetObj->GetID()))
					{
						CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(630));
						return;
					}
					if (GUILDMGR->IsSameGuild((CPlayer*)HERO, (CPlayer*)pTargetObj))
					{
						CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(932));
						return;
					}
					if (GUILDUNION->IsSameUnion((CPlayer*)HERO, (CPlayer*)pTargetObj))
					{
						CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(932));
						return;
					}
				}
				pTargetObj->ShowObjectName(TRUE, RGB_HALF(255, 141, 39));
				CActionTarget Target;
				Target.InitActionTarget(pTargetObj, NULL);
				HERO->EnableAutoAttack(&Target);
				SKILLMGR->OnSkillCommand(HERO, &Target, bMugong);
				pLastObj = pTargetObj;
				dwAttackTime = gCurTime;
			}
		}
	}
}
void CPlayerAI::ProcessCariMonster()
{
	CObject*sObj = OBJECTMGR->GetSelectedObject();
	if (sObj && sObj->GetObjectKind() == eObjectKind_Player)
		return;
	static int gTemp;
	if (IfMonster() &&
		(gCurTime - gTemp >= 8000))
	{
		gTemp = gCurTime;
		{
			PM_SetNextAutoTarget();
		}
		return;
	}
	MACROMGR->PM_SetPrevAutoTarget();
}
void CPlayerAI::PM_SetNextAutoTarget()
{
	OBJECTMGR->UpdateAutoTarget();
	cPtrList* l_plistpAutoTarget = OBJECTMGR->GetAutoTargetList();
	if (!l_plistpAutoTarget)
	{
		return;
	}
	PTRLISTPOS l_posAutoTarget = l_plistpAutoTarget->GetHeadPosition();
	if (!l_posAutoTarget)
	{
		OBJECTMGR->SetSelectedObject(NULL);
		return;
	}
	CObject* l_pObject = NULL;
	while (l_posAutoTarget)
	{
		l_pObject = (CObject*)l_plistpAutoTarget->GetNext(l_posAutoTarget);
		if (l_pObject == m_pTabSelectedObject)
		{
			if (l_posAutoTarget)
			{
				l_pObject = (CObject*)l_plistpAutoTarget->GetAt(l_posAutoTarget);
				l_pObject->ShowObjectName(TRUE, NAMECOLOR_SELECTED);
				OBJECTMGR->SetSelectedObject(l_pObject);
				m_pTabSelectedObject = l_pObject;
				return;
			}
		}
	}
	l_pObject = (CObject*)l_plistpAutoTarget->GetHead();
	if (l_pObject == OBJECTMGR->GetSelectedObject() && l_plistpAutoTarget->GetCount() > 1)
	{
		m_pTabSelectedObject = l_pObject;
		PM_SetNextAutoTarget();
	}
	else
	{
		l_pObject->ShowObjectName(TRUE, NAMECOLOR_SELECTED);
		OBJECTMGR->SetSelectedObject(l_pObject);
		m_pTabSelectedObject = l_pObject;
	}
	return;
}
void CPlayerAI::ProcessLookMonsterNew(LPOBJECT currentMonster, double currentMonsterDistance)
{
	VECTOR3 mobPos[50], heroCurPos;
	ZeroMemory(mobPos, sizeof(mobPos));
	static DWORD dwMouseMoveTick;
	CObject*sObj = OBJECTMGR->GetSelectedObject();
	if (sObj && sObj->GetObjectKind() == eObjectKind_Player)
	{
		if (AttackerID == 0) return;
		CPlayer* pPlayer = (CPlayer*)OBJECTMGR->GetObject(AttackerID);
		if (!pPlayer) return;
		if (!pPlayer->IsDied() || ((CHero*)pPlayer)->IsPKMode())
		{
			return;
		}
	}
	if (IfMonster() &&
		(gCurTime - dwMouseMoveTick >= 8000))
	{
		dwMouseMoveTick = gCurTime;
		HERO->GetPosition(&heroCurPos);
		OBJECTMGR->GetSelectedObject()->GetPosition(&mobPos[0]);
		AttactMonster(OBJECTMGR->GetSelectedObject(), currentMonsterDistance);
		return;
	}
	if (!IfMonster())
		PM_SetNextAutoTarget();
}
void CPlayerAI::ProcessLookMonster()
{
	if (HERO->GetState() == eObjectState_Ungijosik) return;
	double distunce = 0;
	VECTOR3 herotmppos, monstertmppos;
	if (
		OBJECTMGR->GetSelectedObject() &&
		!(OBJECTMGR->GetSelectedObject()->GetObjectKind() >= eObjectKind_Monster && OBJECTMGR->GetSelectedObject()->GetObjectKind() <= eObjectKind_ToghterPlayMonster)
		)
	{
		OBJECTMGR->SetSelectedObject(NULL);
	}
	static DWORD dwMouseMoveTick;
	if (
		OBJECTMGR->GetSelectedObject() &&
		(OBJECTMGR->GetSelectedObject()->GetObjectKind() >= eObjectKind_Monster && OBJECTMGR->GetSelectedObject()->GetObjectKind() <= eObjectKind_ToghterPlayMonster) &&
		!OBJECTMGR->GetSelectedObject()->IsDied() &&
		OBJECTMGR->GetSelectedObject()->GetObjectKind() != eObjectKind_Player &&
		(gCurTime - dwMouseMoveTick>5000)
		)
	{
		HERO->GetPosition(&herotmppos);
		OBJECTMGR->GetSelectedObject()->GetPosition(&monstertmppos);
		double distuncetmp = ((monstertmppos.x - herotmppos.x)*(monstertmppos.x - herotmppos.x) + (monstertmppos.z - herotmppos.z)*(monstertmppos.z - herotmppos.z));
		if (sqrt(distuncetmp)> (5 * 100))
			AttactMonster(OBJECTMGR->GetSelectedObject(), distunce);
		else
			MOVEMGR->SetFindPath();
		return;
	}
	dwMouseMoveTick = gCurTime;
	ObjectArray<LPOBJECT> oArray;
	OBJECTMGR->GetTargetInRange(&vPos, dwRangeArea * 100, &oArray, FALSE, AISETMGR->GetGameAI()->sPage5.cbNoMonsterWall,0);
	oArray.SetPositionHead();
	LPOBJECT lptmpMons = NULL;
	bool bInit = false;
	for (int i = 0; i<oArray.nObjectNum; i++)
	{
		LPOBJECT lpObj = oArray.GetNextObject();
		BYTE wJobMonsterKind = lpObj->GetObjectKind();
		if (wJobMonsterKind >= eObjectKind_Monster && wJobMonsterKind <= eObjectKind_ToghterPlayMonster)
		{
			HERO->GetPosition(&herotmppos);
			lpObj->GetPosition(&monstertmppos);
			double distuncetmp = ((monstertmppos.x - herotmppos.x)*(monstertmppos.x - herotmppos.x) + (monstertmppos.z - herotmppos.z)*(monstertmppos.z - herotmppos.z));
			if (!bInit)
			{
				lptmpMons = lpObj;
				distunce = distuncetmp;
				bInit = true;
				continue;
			}
			if (distuncetmp < distunce)
			{
				distunce = distuncetmp;
				lptmpMons = lpObj;
			}
		}
		else
		{
			continue;
		}
	}
	AttactMonster(lptmpMons, distunce);
}
bool CPlayerAI::CheckQuickBar(DWORD dwQuickSlot, DWORD dwNextFlag)
{
	CQuickDialog * pQuickDlg = GAMEIN->GetQuickDialog();
	if (!pQuickDlg) return false;
	CQuickItem * pQuickItem = pQuickDlg->GetQuickItem(dwQuickSlot);
	if (!pQuickItem)
	{
		SetSkillNextFlag(dwNextFlag);
		UntickAfterCheckingQuickBar(dwNextFlag);
		return false;
	}
	CSkillInfo* pInfo = SKILLMGR->GetSkillInfo(pQuickItem->GetSrcIdx());
	if (!pInfo)
	{
		SetSkillNextFlag(dwNextFlag);
		UntickAfterCheckingQuickBar(dwNextFlag);
		return false;
	}
	return true;
}
void CPlayerAI::UntickAfterCheckingQuickBar(DWORD dwSlot)
{
	switch (dwSlot)
	{
	case 1:AISETMGR->GetGameAI()->sPage6.bSkill1 = FALSE; break;
	case 2:AISETMGR->GetGameAI()->sPage6.bSkill2 = FALSE; break;
	case 3:AISETMGR->GetGameAI()->sPage6.bSkill3 = FALSE; break;
	case 4:AISETMGR->GetGameAI()->sPage6.bSkill4 = FALSE; break;
	case 5:AISETMGR->GetGameAI()->sPage6.bSkill5 = FALSE; break;
	}
}
bool IfTargetBetweenMonster()
{
	CObject* pTargetObj = OBJECTMGR->GetSelectedObject();
	if (pTargetObj)
	{
		BYTE wJobMonsterKind = pTargetObj->GetObjectKind();
		if (wJobMonsterKind >= eObjectKind_Monster && wJobMonsterKind <= eObjectKind_ToghterPlayMonster)
		{
			return true;
		}
	}
	else
		return false;
}
void CPlayerAI::ProcessAttackMacro(BOOL bFlag)
{
	if (HERO->GetState() == eObjectState_SkillUsing) return;
	if (HERO->GetState() == eObjectState_Ungijosik) return;
	if (AISETMGR->GetGameAI()->sPage6.bHelpSkill10)
	{
		if (GetLife() < AISETMGR->GetGameAI()->sPage6.bTunaSpin&&gCurTime - m_TimeForTuna > 3000)
		{
			UseTuNa();
			return;
		}
	}
	if (IfTargetBetweenMonster())
	{
		CMonster* pMonster = (CMonster*)OBJECTMGR->GetSelectedObject();
		if (false == sCPlayerAI.CheckPohunOnObject(pMonster))
		{
			UsePhAndSx(pMonster);
			return;
		}
	}
	bool bLoop;
	CObject* pTargetObj = OBJECTMGR->GetSelectedObject();
	if (pTargetObj)
	{
		VECTOR3 TempPos;
		VECTOR3 SelPos;
		pTargetObj->GetPosition(&SelPos);
		HERO->GetPosition(&TempPos);
		if (MOVEMGR->IsMoving(HERO) & PATHMGR->CheckCollisionLine(&TempPos, &SelPos, HERO)
			)
		{
			bLoop = false;
		}
		else
		{
			bLoop = true;
		}			
	}
	{
		if ((!AISETMGR->GetGameAI()->sPage6.bSkill1
			&& !AISETMGR->GetGameAI()->sPage6.bSkill2
			&& !AISETMGR->GetGameAI()->sPage6.bSkill3
			&& !AISETMGR->GetGameAI()->sPage6.bSkill4
			&& !AISETMGR->GetGameAI()->sPage6.bSkill5)
			|| GetNaeRyuk() < 2
			|| HERO->GetLevel() < 5
			)
		{
			g_UserInput.GetHeroMove()->AttackSelectedObject();
			m_MovePoint = time(NULL);
			m_TimeForActtact = gCurTime;
		}
		else if (gCurTime - m_TimeForActtact >= AISETMGR->GetGameAI()->sPage6.bSkill1time + QUICKMGR->GetUserSkillTime() && m_SkillNextFlag[0] == 1)
		{
			if (AISETMGR->GetGameAI()->sPage6.bSkill1)
			{
				if (time(NULL) - m_SkillTimeLast[0] >= AISETMGR->GetGameAI()->sPage6.bSkill1time)
				{
					int selectedQuickAbsPos = (AISETMGR->GetGameAI()->sPage6.bSkill1page - 1) * TABCELL_QUICK_NUM + (AISETMGR->GetGameAI()->sPage6.bSkill1key - 1);
					if (CheckQuickBar(selectedQuickAbsPos, 1))
					{
						QUICKMGR->UseQuickItem(AISETMGR->GetGameAI()->sPage6.bSkill1page - 1, AISETMGR->GetGameAI()->sPage6.bSkill1key - 1);
						m_SkillTimeLast[0] = time(NULL);
						m_MovePoint = time(NULL);
						m_TimeForActtact = gCurTime;
					}
				}
			}
			SetSkillNextFlag(1);
		}
		else if (gCurTime - m_TimeForActtact >= AISETMGR->GetGameAI()->sPage6.bSkill2time + QUICKMGR->GetUserSkillTime() && m_SkillNextFlag[1] == 1)
		{
			if (AISETMGR->GetGameAI()->sPage6.bSkill2)
			{
				if (time(NULL) - m_SkillTimeLast[1] >= AISETMGR->GetGameAI()->sPage6.bSkill2time)
				{
					int selectedQuickAbsPos = (AISETMGR->GetGameAI()->sPage6.bSkill2page - 1) * TABCELL_QUICK_NUM + (AISETMGR->GetGameAI()->sPage6.bSkill2key - 1);
					if (CheckQuickBar(selectedQuickAbsPos, 2))
					{
						QUICKMGR->UseQuickItem(AISETMGR->GetGameAI()->sPage6.bSkill2page - 1, AISETMGR->GetGameAI()->sPage6.bSkill2key - 1);
						m_SkillTimeLast[1] = time(NULL);
						m_MovePoint = time(NULL);
						m_TimeForActtact = gCurTime;
					}
				}
			}
			SetSkillNextFlag(2);
		}
		else if (gCurTime - m_TimeForActtact >= AISETMGR->GetGameAI()->sPage6.bSkill3time + QUICKMGR->GetUserSkillTime() && m_SkillNextFlag[2] == 1)
		{
			if (AISETMGR->GetGameAI()->sPage6.bSkill3)
			{
				if (time(NULL) - m_SkillTimeLast[2] >= AISETMGR->GetGameAI()->sPage6.bSkill3time)
				{
					int selectedQuickAbsPos = (AISETMGR->GetGameAI()->sPage6.bSkill3page - 1) * TABCELL_QUICK_NUM + (AISETMGR->GetGameAI()->sPage6.bSkill3key - 1);
					if (CheckQuickBar(selectedQuickAbsPos, 3))
					{
						QUICKMGR->UseQuickItem(AISETMGR->GetGameAI()->sPage6.bSkill3page - 1, AISETMGR->GetGameAI()->sPage6.bSkill3key - 1);
						m_SkillTimeLast[2] = time(NULL);
						m_MovePoint = time(NULL);
						m_TimeForActtact = gCurTime;
					}
				}
			}
			SetSkillNextFlag(3);
		}
		else if (gCurTime - m_TimeForActtact >= AISETMGR->GetGameAI()->sPage6.bSkill4time + QUICKMGR->GetUserSkillTime() && m_SkillNextFlag[3] == 1)
		{
			if (AISETMGR->GetGameAI()->sPage6.bSkill4)
			{
				if (time(NULL) - m_SkillTimeLast[3] >= AISETMGR->GetGameAI()->sPage6.bSkill4time)
				{
					int selectedQuickAbsPos = (AISETMGR->GetGameAI()->sPage6.bSkill4page - 1) * TABCELL_QUICK_NUM + (AISETMGR->GetGameAI()->sPage6.bSkill4key - 1);
					if (CheckQuickBar(selectedQuickAbsPos, 4))
					{
						QUICKMGR->UseQuickItem(AISETMGR->GetGameAI()->sPage6.bSkill4page - 1, AISETMGR->GetGameAI()->sPage6.bSkill4key - 1);
						m_SkillTimeLast[3] = time(NULL);
						m_MovePoint = time(NULL);
						m_TimeForActtact = gCurTime;
					}
				}
			}
			SetSkillNextFlag(4);
		}
		else if (gCurTime - m_TimeForActtact >= AISETMGR->GetGameAI()->sPage6.bSkill5time + QUICKMGR->GetUserSkillTime() && m_SkillNextFlag[4] == 1)
		{
			if (AISETMGR->GetGameAI()->sPage6.bSkill5)
			{
				if (time(NULL) - m_SkillTimeLast[4] >= AISETMGR->GetGameAI()->sPage6.bSkill5time)
				{
					int selectedQuickAbsPos = (AISETMGR->GetGameAI()->sPage6.bSkill5page - 1) * TABCELL_QUICK_NUM + (AISETMGR->GetGameAI()->sPage6.bSkill5key - 1);
					if (CheckQuickBar(selectedQuickAbsPos, 0))
					{
						QUICKMGR->UseQuickItem(AISETMGR->GetGameAI()->sPage6.bSkill5page - 1, AISETMGR->GetGameAI()->sPage6.bSkill5key - 1);
						m_SkillTimeLast[4] = time(NULL);
						m_MovePoint = time(NULL);
						m_TimeForActtact = gCurTime;
					}
				}
			}
			SetSkillNextFlag(0);
		}
	}
}
void CPlayerAI::CheckingSkill()
{
	COptionDialog * pDlg = GAMEIN->GetOptionDialog();
	cDialog* m_Dlg;
	m_Dlg = (cDialog*)pDlg->GetTabSheet(4);
	if (!((cCheckBox*)m_Dlg->GetWindowForID(OTI_CB_AUTOUNGI))->IsChecked() &&
		!((cCheckBox*)m_Dlg->GetWindowForID(OTI_CB_AUTOHUNT_UNGI_MP))->IsChecked() &&
		!((cCheckBox*)m_Dlg->GetWindowForID(OTI_CB_AUTOHUNT_UNGI_SP))->IsChecked())
	{
		((cCheckBox*)m_Dlg->GetWindowForID(OTI_CB_AUTOHUNT_UNGI_AT_NOTARGET))->SetChecked(FALSE);
		((cCheckBox*)m_Dlg->GetWindowForID(OTI_CB_AUTOHUNT_UNGI_AT_NOTARGET))->SetDisable(TRUE);
		((cCheckBox*)m_Dlg->GetWindowForID(OTI_CB_AUTOHUNT_UNGI_ATTACKED))->SetChecked(FALSE);
		((cCheckBox*)m_Dlg->GetWindowForID(OTI_CB_AUTOHUNT_UNGI_ATTACKED))->SetDisable(TRUE);
	}
	m_Dlg = (cDialog*)pDlg->GetTabSheet(5);
	CMugongBase * pMugong1 = CheckHeightOrLowForOptionDlg(60, 61);
	CMugongBase * pMugong2 = CheckHeightOrLowForOptionDlg(1100, 1110);
	CMugongBase * pMugong3 = CheckHeightOrLowForOptionDlg(900, 910);
	CMugongBase * pMugong4 = CheckHeightOrLowForOptionDlg(702, 712);
	CMugongBase * pMugong5 = CheckHeightOrLowForOptionDlg(802, 812);
	CMugongBase * pMugong6 = CheckHeightOrLowForOptionDlg(902, 911);
	CMugongBase * pMugong7 = CheckHeightOrLowForOptionDlg(1002, 1012);
	CMugongBase * pMugong8 = CheckHeightOrLowForOptionDlg(1102, 1112);
	CMugongBase * pMugong9 = CheckHeightOrLowForOptionDlg(1202, 1202);
	CMugongBase * pMugong10 = CheckHeightOrLowForOptionDlg(1203, 1203);
	DWORD Temp = 61 | 60;
	if (!pMugong1)
	{
		AISETMGR->GetGameAI()->sPage6.bHelpSkill9 = FALSE;
		((cCheckBox*)m_Dlg->GetWindowForID(OTI_CB_AUTOSKIPEFFECT))->SetChecked(FALSE);
		((cCheckBox*)m_Dlg->GetWindowForID(OTI_CB_AUTOSKIPEFFECT))->SetDisable(TRUE);
	}
	else((cCheckBox*)m_Dlg->GetWindowForID(OTI_CB_AUTOSKIPEFFECT))->SetDisable(FALSE);
	if (!pMugong2)
	{
		AISETMGR->GetGameAI()->sPage6.bHelpSkill8 = FALSE;
		((cCheckBox*)m_Dlg->GetWindowForID(OTI_CB_AUTOEARTHGUARD))->SetChecked(FALSE);
		((cCheckBox*)m_Dlg->GetWindowForID(OTI_CB_AUTOEARTHGUARD))->SetDisable(TRUE);
	}
	else((cCheckBox*)m_Dlg->GetWindowForID(OTI_CB_AUTOEARTHGUARD))->SetDisable(FALSE);
	if (!pMugong3)
	{
		AISETMGR->GetGameAI()->sPage6.bHelpSkill10 = FALSE;
		((cCheckBox*)m_Dlg->GetWindowForID(OTI_CB_AUTOADDLIFESKILL))->SetChecked(FALSE);
		((cCheckBox*)m_Dlg->GetWindowForID(OTI_CB_AUTOADDLIFESKILL))->SetDisable(TRUE);
		((cSpin*)m_Dlg->GetWindowForID(OTI_CB_TNLIFE))->SetDisable(TRUE);
		((cSpin*)m_Dlg->GetWindowForID(OTI_CB_TNLIFE))->SetDisable(TRUE);
		((cSpin*)m_Dlg->GetWindowForID(OTI_CB_TNLIFE))->SetMinMax(0, 0);
	}
	else
	{
		((cCheckBox*)m_Dlg->GetWindowForID(OTI_CB_AUTOADDLIFESKILL))->SetDisable(FALSE);
		((cSpin*)m_Dlg->GetWindowForID(OTI_CB_TNLIFE))->SetDisable(FALSE);
		((cSpin*)m_Dlg->GetWindowForID(OTI_CB_TNLIFE))->SetMinMax(5, 90);
	}
	if (!pMugong4)
	{
		AISETMGR->GetGameAI()->sPage6.bHelpSkill3 = FALSE;
		((cCheckBox*)m_Dlg->GetWindowForID(OTI_CB_AUTOFIRE))->SetChecked(FALSE);
		((cCheckBox*)m_Dlg->GetWindowForID(OTI_CB_AUTOFIRE))->SetDisable(TRUE);
	}
	else((cCheckBox*)m_Dlg->GetWindowForID(OTI_CB_AUTOFIRE))->SetDisable(FALSE);
	if (!pMugong5)
	{
		AISETMGR->GetGameAI()->sPage6.bHelpSkill4 = FALSE;
		((cCheckBox*)m_Dlg->GetWindowForID(OTI_CB_AUTOWATER))->SetChecked(FALSE);
		((cCheckBox*)m_Dlg->GetWindowForID(OTI_CB_AUTOWATER))->SetDisable(TRUE);
	}
	else((cCheckBox*)m_Dlg->GetWindowForID(OTI_CB_AUTOWATER))->SetDisable(FALSE);
	if (!pMugong6)
	{
		AISETMGR->GetGameAI()->sPage6.bHelpSkill5 = FALSE;
		((cCheckBox*)m_Dlg->GetWindowForID(OTI_CB_AUTOTREE))->SetChecked(FALSE);
		((cCheckBox*)m_Dlg->GetWindowForID(OTI_CB_AUTOTREE))->SetDisable(TRUE);
	}
	else((cCheckBox*)m_Dlg->GetWindowForID(OTI_CB_AUTOTREE))->SetDisable(FALSE);
	if (!pMugong7)
	{
		AISETMGR->GetGameAI()->sPage6.bHelpSkill6 = FALSE;
		((cCheckBox*)m_Dlg->GetWindowForID(OTI_CB_AUTOIRON))->SetChecked(FALSE);
		((cCheckBox*)m_Dlg->GetWindowForID(OTI_CB_AUTOIRON))->SetDisable(TRUE);
	}
	else((cCheckBox*)m_Dlg->GetWindowForID(OTI_CB_AUTOIRON))->SetDisable(FALSE);
	if (!pMugong8)
	{
		AISETMGR->GetGameAI()->sPage6.bHelpSkill7 = FALSE;
		((cCheckBox*)m_Dlg->GetWindowForID(OTI_CB_AUTOEARTH))->SetChecked(FALSE);
		((cCheckBox*)m_Dlg->GetWindowForID(OTI_CB_AUTOEARTH))->SetDisable(TRUE);
	}
	else((cCheckBox*)m_Dlg->GetWindowForID(OTI_CB_AUTOEARTH))->SetDisable(FALSE);
	if (!pMugong9)
	{
		AISETMGR->GetGameAI()->sPage6.bHelpSkill1 = FALSE;
		((cCheckBox*)m_Dlg->GetWindowForID(OTI_CB_AUTODOWNDEFENCE))->SetChecked(FALSE);
		((cCheckBox*)m_Dlg->GetWindowForID(OTI_CB_AUTODOWNDEFENCE))->SetDisable(TRUE);
	}
	else((cCheckBox*)m_Dlg->GetWindowForID(OTI_CB_AUTODOWNDEFENCE))->SetDisable(FALSE);
	if (!pMugong10)
	{
		AISETMGR->GetGameAI()->sPage6.bHelpSkill2 = FALSE;
		((cCheckBox*)m_Dlg->GetWindowForID(OTI_CB_AUTODOWNATTACK))->SetChecked(FALSE);
		((cCheckBox*)m_Dlg->GetWindowForID(OTI_CB_AUTODOWNATTACK))->SetDisable(TRUE);
	}
	else((cCheckBox*)m_Dlg->GetWindowForID(OTI_CB_AUTODOWNATTACK))->SetDisable(FALSE);
	m_Dlg = (cDialog*)pDlg->GetTabSheet(9);
	if (HERO->GetStage() == 128 || HERO->GetStage() == 129)
	{
		((cCheckBox*)m_Dlg->GetWindowForID(OTI_CB_M_AUTOSKILL_BH))->SetDisable(false);
		((cCheckBox*)m_Dlg->GetWindowForID(OTI_CB_M_AUTOSKILL_LX))->SetDisable(false);
		((cCheckBox*)m_Dlg->GetWindowForID(OTI_CB_M_AUTOSKILL_JYING))->SetDisable(false);
		((cCheckBox*)m_Dlg->GetWindowForID(OTI_CB_M_AUTOSKILL_HM))->SetDisable(false);
		((cCheckBox*)m_Dlg->GetWindowForID(OTI_CB_M_AUTOSKILL_TM))->SetDisable(false);
		((cCheckBox*)m_Dlg->GetWindowForID(OTI_CB_M_AUTOSKILL_XX))->SetDisable(false);
	}
	else
	{
		AISETMGR->GetGameAI()->sPage8.m_AutoSkill1 = false;
		AISETMGR->GetGameAI()->sPage8.m_AutoSkill2 = false;
		AISETMGR->GetGameAI()->sPage8.m_AutoSkill3 = false;
		AISETMGR->GetGameAI()->sPage8.m_AutoSkill4 = false;
		AISETMGR->GetGameAI()->sPage8.m_AutoSkill5 = false;
		AISETMGR->GetGameAI()->sPage8.m_AutoSkill6 = false;
		((cCheckBox*)m_Dlg->GetWindowForID(OTI_CB_M_AUTOSKILL_BH))->SetChecked(false);
		((cCheckBox*)m_Dlg->GetWindowForID(OTI_CB_M_AUTOSKILL_LX))->SetChecked(false);
		((cCheckBox*)m_Dlg->GetWindowForID(OTI_CB_M_AUTOSKILL_JYING))->SetChecked(false);
		((cCheckBox*)m_Dlg->GetWindowForID(OTI_CB_M_AUTOSKILL_HM))->SetChecked(false);
		((cCheckBox*)m_Dlg->GetWindowForID(OTI_CB_M_AUTOSKILL_TM))->SetChecked(false);
		((cCheckBox*)m_Dlg->GetWindowForID(OTI_CB_M_AUTOSKILL_XX))->SetChecked(false);
		((cCheckBox*)m_Dlg->GetWindowForID(OTI_CB_M_AUTOSKILL_BH))->SetDisable(true);
		((cCheckBox*)m_Dlg->GetWindowForID(OTI_CB_M_AUTOSKILL_LX))->SetDisable(true);
		((cCheckBox*)m_Dlg->GetWindowForID(OTI_CB_M_AUTOSKILL_JYING))->SetDisable(true);
		((cCheckBox*)m_Dlg->GetWindowForID(OTI_CB_M_AUTOSKILL_HM))->SetDisable(true);
		((cCheckBox*)m_Dlg->GetWindowForID(OTI_CB_M_AUTOSKILL_TM))->SetDisable(true);
		((cCheckBox*)m_Dlg->GetWindowForID(OTI_CB_M_AUTOSKILL_XX))->SetDisable(true);
	}
	if (HERO->GetStage() == 64 || HERO->GetStage() == 65)
	{
		((cCheckBox*)m_Dlg->GetWindowForID(OTI_CB_H_AUTOSKILL_JG))->SetDisable(false);
		((cCheckBox*)m_Dlg->GetWindowForID(OTI_CB_H_AUTOSKILL_SZ))->SetDisable(false);
		((cCheckBox*)m_Dlg->GetWindowForID(OTI_CB_H_AUTOSKILL_JYANG))->SetDisable(false);
		((cCheckBox*)m_Dlg->GetWindowForID(OTI_CB_H_AUTOSKILL_TX))->SetDisable(false);
		((cCheckBox*)m_Dlg->GetWindowForID(OTI_CB_H_AUTOSKILL_BM))->SetDisable(false);
		((cCheckBox*)m_Dlg->GetWindowForID(OTI_CB_H_AUTOSKILL_FM))->SetDisable(false);
	}
	else
	{
		AISETMGR->GetGameAI()->sPage8.h_AutoSkill1 = false;
		AISETMGR->GetGameAI()->sPage8.h_AutoSkill2 = false;
		AISETMGR->GetGameAI()->sPage8.h_AutoSkill3 = false;
		AISETMGR->GetGameAI()->sPage8.h_AutoSkill4 = false;
		AISETMGR->GetGameAI()->sPage8.h_AutoSkill5 = false;
		AISETMGR->GetGameAI()->sPage8.h_AutoSkill6 = false;
		((cCheckBox*)m_Dlg->GetWindowForID(OTI_CB_H_AUTOSKILL_JG))->SetChecked(false);
		((cCheckBox*)m_Dlg->GetWindowForID(OTI_CB_H_AUTOSKILL_SZ))->SetChecked(false);
		((cCheckBox*)m_Dlg->GetWindowForID(OTI_CB_H_AUTOSKILL_JYANG))->SetChecked(false);
		((cCheckBox*)m_Dlg->GetWindowForID(OTI_CB_H_AUTOSKILL_TX))->SetChecked(false);
		((cCheckBox*)m_Dlg->GetWindowForID(OTI_CB_H_AUTOSKILL_BM))->SetChecked(false);
		((cCheckBox*)m_Dlg->GetWindowForID(OTI_CB_H_AUTOSKILL_FM))->SetChecked(false);
		((cCheckBox*)m_Dlg->GetWindowForID(OTI_CB_H_AUTOSKILL_JG))->SetDisable(true);
		((cCheckBox*)m_Dlg->GetWindowForID(OTI_CB_H_AUTOSKILL_SZ))->SetDisable(true);
		((cCheckBox*)m_Dlg->GetWindowForID(OTI_CB_H_AUTOSKILL_JYANG))->SetDisable(true);
		((cCheckBox*)m_Dlg->GetWindowForID(OTI_CB_H_AUTOSKILL_TX))->SetDisable(true);
		((cCheckBox*)m_Dlg->GetWindowForID(OTI_CB_H_AUTOSKILL_BM))->SetDisable(true);
		((cCheckBox*)m_Dlg->GetWindowForID(OTI_CB_H_AUTOSKILL_FM))->SetDisable(true);
	}
}
void CPlayerAI::ProcessTambahan()
{
	DWORD dwCurTime = MHTIMEMGR->GetNewCalcCurTime();
	if (dwCurTime - m_TimeForHideObject >= 1000/*> 0.1*/)
	{
		m_TimeForHideObject = MHTIMEMGR->GetNewCalcCurTime();
		if (OPTIONMGR->GetGameOption()->bShowPet)
			OBJECTMGR->HideAllPet(false);
		if (OPTIONMGR->GetGameOption()->bShowMonster)
			OBJECTMGR->HideAllMonster(TRUE);
		OBJECTMGR->ApplyOverInfoOptionToAll();
		m_TimeForHideObject = time(NULL);
	}
	static DWORD dwNewTime;
	if (gCurTime - dwNewTime >= 100)
	{
		dwNewTime = gCurTime;
		if (AISETMGR->GetGameAI()->sPage8.isAutoJoinGroup)
		{
			CPartyInviteDlg * pDlg = (CPartyInviteDlg*)WINDOWMGR->GetWindowForID(PA_INVITEDLG);
			if (pDlg->IsActive())
			{
				PARTYMGR->PartyInviteAccept();
				pDlg->SetActive(FALSE);				
			}
		}
		if (AISETMGR->GetGameAI()->sPage8.isReConnServer)
		{
			AutoEatPet();
		}
		if (AISETMGR->GetGameAI()->sPage8.isAutoUseBuffItem)
		{
			AutoUseBuff();
		}
		if (AISETMGR->GetGameAI()->sPage8.isAutoWant)
		{
			cDialog* WantReg_Dialog = WINDOWMGR->GetWindowForID(WANTREG_DLG);
			if (WantReg_Dialog->IsActive())
			{
				char  WantMoneyStr[10];
				cEditBox* WantReg_Money = (cEditBox*)WantReg_Dialog->GetWindowForID(WANTREG_PRIZEEDIT);
				WantReg_Money->SetEditText(itoa(AISETMGR->GetGameAI()->sPage8.WantMoney, WantMoneyStr, 10));
				char* temp = RemoveComma(WantReg_Money->GetEditText());
				DWORD prize = atoi(temp);
				WANTEDMGR->RegistKillerSyn(prize);
			}
			cMsgBox* pMsgBox = (cMsgBox*)WINDOWMGR->GetWindowForID(MBI_SOS);
			if (pMsgBox)
			{
				MSGBASE msg;
				SetProtocol(&msg, MP_GUILD, MP_GUILD_SOS_SEND_CANCEL);
				msg.dwObjectID = HEROID;
				NETWORK->Send(&msg, sizeof(MSGBASE));
				pMsgBox->SetActive(FALSE);
			}
		}
	}
}
DWORD bLaparError = 0;
void CPlayerAI::AutoEatPet()
{
#ifndef  _MUTIPET_
	CPet * m_HeroPet = HERO->GetPet();
	if (!m_HeroPet) return;
	if (PETMGR->IsCurPetStaminaFull())
	{
		bagimakan = FALSE;
	}
	if (!m_HeroPet->IsPetStaminaFull())
	{
		DOUBLE d1, d2;
		DOUBLE tmtpmt;
		d1 = (DOUBLE)m_HeroPet->GetPetStamina();
		d2 = (DOUBLE)m_HeroPet->GetMaxStamina();
		tmtpmt = (d1 / d2) * 100;		
		if (tmtpmt < AISETMGR->GetGameAI()->sPage8.spPetEatPercent)
		{
			bagimakan = TRUE;
		}
	}
	else
	{
		DOUBLE L1, L2;
		L1 = (DOUBLE)m_HeroPet->GetPetStamina();
		L2 = (DOUBLE)m_HeroPet->GetMaxStamina();
		if (L1 >= L2)
		{
			bagimakan = FALSE;
		}
	}
	if (!bagimakan) return;
	for (int i = TP_INVENTORY_START; i < TP_INVENTORY_END; i++)
	{
		CItem * PetFood = GAMEIN->GetInventoryDialog()->GetItemForPos(i);
		if (PetFood)
		{
			if (PetFood->GetItemIdx() == 8502)
			{
				GAMEIN->GetInventoryDialog()->UseItem(PetFood);
				break;
			}
			else if (PetFood->GetItemIdx() == 8501 )
			{
				GAMEIN->GetInventoryDialog()->UseItem(PetFood);
				break;
			}
			else if (PetFood->GetItemIdx() == 8500)
			{
				GAMEIN->GetInventoryDialog()->UseItem(PetFood);
				break;
			}
		}
	}
#endif //  _MUTIPET_
}
void CPlayerAI::AutoUseBuff()
{
	if (HERO)
	{
		if (HERO->IsDied())
		{
			return;
		}
		for (int i = TP_SHOPINVEN_START; i<TP_SHOPINVEN_END; i++)
		{
			CItem * ItemBuffer = (CItem*)GAMEIN->GetInventoryDialog()->GetItemForPos(i);
			if (ItemBuffer)
			{
				if (ItemBuffer->GetItemKind() == eSHOP_ITEM_CHARM || ItemBuffer->GetItemKind() == eSHOP_ITEM_HERB)
				{
					if (ItemBuffer->GetItemParam() & ITEM_PARAM_SEAL)
					{
						continue;
					}
					if (ITEMMGR->GetUsedItemInfo(ItemBuffer->GetItemIdx()))
					{
						continue;
					}
					if (ItemBuffer->GetItemIdx() >= 55101 && ItemBuffer->GetItemIdx() <= 55212)
					{
						GAMEIN->GetInventoryDialog()->UseItem(ItemBuffer);
						return;
					}
					if (ITEMMGR->IsAutoUseBuffItem(ItemBuffer->GetItemIdx()))
					{
						GAMEIN->GetInventoryDialog()->UseItem(ItemBuffer);
						return;
					}
				}
			}
		}
	}
}
void CPlayerAI::AutoUseBuff2()
{
	if (HERO)
	{
		if (HERO->IsDied())
		{
			return;
		}
		for (int i = TP_SHOPINVEN_START; i<TP_SHOPINVEN_END; i++)
		{
			CItem * ItemBuffer = (CItem*)GAMEIN->GetInventoryDialog()->GetItemForPos(i);
			if (ItemBuffer)
			{
				if (ItemBuffer->GetItemKind() == eSHOP_ITEM_CHARM || ItemBuffer->GetItemKind() == eSHOP_ITEM_HERB)
				{
					if (ItemBuffer->GetItemParam() & ITEM_PARAM_SEAL)
					{
						GAMEIN->GetInventoryDialog()->GetShopInven()->SetCurSelCellPos(i);
						MSG_DWORD msg;
						msg.Category = MP_ITEM;
						msg.Protocol = MP_ITEM_SHOPITEM_UNSEAL_SYN;
						msg.dwObjectID = HERO->GetID();
						msg.dwData = i;
						NETWORK->Send(&msg, sizeof(msg));
						GAMEIN->GetInventoryDialog()->UseItem(ItemBuffer);
					}
					if (ITEMMGR->GetUsedItemInfo(ItemBuffer->GetItemIdx()))
					{
						continue;
					}
					GAMEIN->GetInventoryDialog()->UseItem(ItemBuffer);
				}
			}
		}
	}
}
void CPlayerAI::AutoCallPet()
{
	if (AISETMGR->GetGameAI()->sPage8.isAutoCallPet)
	{
		if (m_PetIdx == -1)
		{
			return;
		}
		//CItem * ItemPet = GAMEIN->GetInventoryDialog()->GetItemForPos(m_PetPos); // OLD CODE
		CItem* ItemPet = GAMEIN->GetInventoryDialog()->GetItemLike(m_PetIdx); // JACK
		if (ItemPet)
		{
			CHATMGR->AddMsg(CTC_SYSMSG, "%d", m_PetIdx);
			if (ItemPet->GetItemKind() == eSHOP_ITEM_PET || ItemPet->GetItemKind() == eQUEST_ITEM_PET)
			{
#ifndef  _MUTIPET_
				if (HERO)
				{
					if (HERO->GetPet() == NULL)
					{
						GAMEIN->GetInventoryDialog()->UseItem(ItemPet);
					}
				}
#endif //  _MUTIPET_	
			}
		}
	}
}
void CPlayerAI::AttactMonster(LPOBJECT lpObj, double distance1)
{
	{
		{
			{
				CMonster* pMonster = (CMonster*)lpObj;
				OBJECTMGR->SetSelectedObject(pMonster);
				UsePhAndSx(pMonster, distance1);
			}
		}
	}
}
void CPlayerAI::StartRev2()
{
	if (false == GAMEIN->GetReviveDialog()->IsActive() && HERO->IsDied())
	{
		m_bRevProcessing2 = TRUE;
		DWORD dwCurTime = MHTIMEMGR->GetNewCalcCurTime();
		m_dwRevStartTime2 = dwCurTime - 1000;
		m_nCount2 = m_nRevCount2;
	}
}
void CPlayerAI::ProcessReviving2()
{
	DWORD dwCheckBit = eEventMap | eTournament | eQuestRoom | eSurvivalMap;
	if (HERO != NULL && !MAP->IsMapKind(dwCheckBit))
	{
		if (BATTLESYSTEM->GetBattle(HERO)->GetBattleKind() != eBATTLE_KIND_NONE || VIMUMGR->IsVimuing()) return;
	}
	if (!m_bRevProcessing2) return;
	if (GAMEIN->GetReviveDialog()->IsActive() ||
		!HERO->IsDied())
	{
		m_bRevProcessing2 = FALSE;
		bReviveDialogIsShowingSwitch = true;
		return;
	}
	if (gCurTime - m_dwRevStartTime2 >= 1000)
	{
		m_dwRevStartTime2 += 1000;
		if (m_nCount2 <= 0)
		{
			m_bRevProcessing2 = FALSE;
			bReviveDialogIsShowingSwitch = true;
			if (false == GAMEIN->GetReviveDialog()->IsActive() && HERO->IsDied())
				GAMEIN->GetReviveDialog()->SetActive(TRUE);
		}
		else
		{
			CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(2416), m_nCount2);
		}
		--m_nCount2;
	}
}
BOOL CPlayerAI::CheckPohunOnObject(CMonster* pMonster)
{
	BOOL Phflg = FindSkillStatusFLg(256, pMonster);
	CMugongBase* pMugongBasePh = MUGONGMGR->GetMugongByMugongIdx(1202);
	BOOL Sxflg = FindSkillStatusFLg(4096, pMonster);
	CMugongBase* pMugongBaseSx = MUGONGMGR->GetMugongByMugongIdx(1203);
	if (pMugongBasePh && !Phflg && AISETMGR->GetGameAI()->sPage6.bHelpSkill1)
		return FALSE;
	else if (pMugongBaseSx && !Sxflg && AISETMGR->GetGameAI()->sPage6.bHelpSkill2)
		return FALSE;
	return TRUE;
}
BOOL CPlayerAI::NewTeran(DWORD dwSkillIdx)
{
	CMugongBase* pMugongBase = MUGONGMGR->GetMugongByMugongIdx(dwSkillIdx);
	if (pMugongBase)
	{
		CSkillInfo* SkillInfo = NULL;
		SkillInfo = SKILLMGR->GetSkillInfo(pMugongBase->GetSkillInfo()->GetSkillIndex());
		if (SkillInfo&&HERO->GetLevel() >= SkillInfo->GetSkillInfo()->RestrictLevel)
		{
			if (HERO->GetDelayGroup()->CheckDelay(CDelayGroup::eDK_Skill, SkillInfo->GetSkillIndex()) == 0)
			{
				return PlayerUseSkill(SkillInfo);
			}
		}
	}
	return false;
}
CMonster * g_pMonster = NULL;
void CPlayerAI::InitTestMonster()
{
	DWORD id = 10;
	float x, z;
	x = 25200;
	z = 23400;
	DWORD temp = 0;
	for (int n = 0; n<4; ++n)
	{
		SEND_MONSTER_TOTALINFO info;
		memset(&info, 0, sizeof(info));
		info.BaseObjectInfo.dwObjectID = id++;
		info.BaseObjectInfo.BattleID = gChannelNum + 1;
		sprintf(info.BaseObjectInfo.ObjectName, "", temp++);
		VECTOR3 pos;
		pos.x = x;
		pos.z = z;
		info.MoveInfo.CurPos.Compress(&pos);
		BASEMOVE_INFO bminfo;
		bminfo.SetFrom(&info.MoveInfo);
		info.TotalInfo.MonsterKind = 426 + n;
		g_pMonster = OBJECTMGR->AddMonster(&info.BaseObjectInfo, &bminfo, &info.TotalInfo);
		x += 200;
		z += 5 * rand() % 30;
	}
	temp = 0;
}