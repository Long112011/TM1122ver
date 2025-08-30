// Hero.cpp: implementation of the CHero class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Hero.h"
#include "GameIn.h"
#include "MHCamera.h"
#include "ObjectManager.h"
#include "MoveManager.h"
#include "ObjectStateManager.h"
//#include "MinimapDialog.h"
#include "ItemManager.h"
#include "StatsCalcManager.h"
#include "SkillManager_Client.h"
#include "KyungGongManager.h"
#include "TacticManager.h"
#include "PartyManager.h"
#include "MugongManager.h"
#include "ObjectGuagen.h"
#include "./Audio/MHAudioManager.h"
#include "StreetStallManager.h"
#include "CheatMsgParser.h"
#include "interface/cWindowManager.h"
#include "OptionManager.h"
#include "WantedManager.h"
#include "ChatManager.h"
#include "TitanManager.h"

#include "MixDialog.h"
#include "ReinforceDlg.h"
#include "UpgradeDlg.h"
#include "DissolutionDialog.h"

#ifdef _DEBUG
#include "interface\cFont.h"
#include "ChatManager.h"
#endif


#include "WindowIdEnum.h"

#include "ReviveDialog.h"
#include "CharacterDialog.h"
#include "InventoryExDialog.h"
#include "DealDialog.h"
#include "CharStateDialog.h"

#include "mhMap.h"
#include "GameEventManager.h"

#include "BattleSystem_Client.h"
#include "NpcScriptDialog.h"
#include "CommonCalcFunc.h"
#include "AppearanceManager.h"

#include "AbilityInfo.h"

#include "SkillPointRedist.h"
#include "SkillPointNotify.h"
#include "SuryunDialog.h"
#include "AbilityManager.h"

#include "ChaseinputDialog.h"
#include "ShoutDialog.h"
#include "MoveDialog.h"

#include "QuickDialog.h"
#include "PeaceWarModeManager.h"
#include "GuageDialog.h"
#include "MussangManager.h"
#include "WeatherManager.h"
#include "PetManager.h"
#include "InventoryExDialog.h"
#include "PyoGukDialog.h"
#include "cMsgBox.h"

#include "EventMapInfo.h"
#include "SiegeWarMgr.h"
#include "PartyWar.h"
#include "GuildFieldWar.h"
#include "GuildManager.h"
#include "GoldShopDialog.h"   // 2014-08-16 GoldShop header file include
#include "LuckerDialog.h"     // the lucker file include 2014-06-05
#include "MoneyPacketDialog.h" // the MoneyPacketDialog file include 2014-11-09
#include "MiniMapPing.h"       // the ping map time include file 2014-11-13
//#include "GameAIManager.h"  //weiye ?€å¢ƒè??€?›æ???2017-12-05
#include "SafeArea.h"       //weiye ?‡ïŸµ?¥é??›æ???2018-03-07
//#include "AutoFindPathManager.h" //weiye èµ»é??†ç??›æ???2018-03-14


#include "CWayPointManager.h"
#include "TileManager.h"
#include "MovePoint.h"
#include "BigMapDlg.h"
#include "MiniMapDlg.h"
#include "GuildWarManager.h"
//#include "AiKyunGong.h"

#include "FortWarDialog.h"

//#include "cFindPatnIcon.h"
#include "VimuManager.h"
//#include "GuildWarDialog.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
#include "AiManager.h"
#include "autopath.h"

#include "GameCheckManager.h"
#include "./Input/UserInput.h"

#include "FameManager.h"
#include "FortWarManager.h"

#include "CXBOXController.h"
#include "ShoutchatDialog.h"
//extern char g_MoveSetbyAutoPath;
extern BOOL KeyboardMode;
extern volatile LONG g_PathThreadActivated;

extern BOOL jTweakToggle;
VECTOR3 gHeroPos;
extern BOOL jTweak;
int column/* = 0*/;
BOOL Gamepadmode;
BOOL isConnected;
//CXBOXController* Player[1];
CHero::CHero():
//Player[0](new CXBOXController),
m_WayPointManager(new CWayPointManager)
, m_KeyMove_DistOneStep(1000)
{
	isConnected = FALSE;
	m_NextAction.Clear();
	m_CurComboNum = 0;
	m_maxExpPoint = 0;
	m_PyogukNum = 0;	
	m_bUngiFlag = FALSE;
	m_AbilityGroup.SetOwenerObject(this);

	//memset(m_OtherPlayerEquip, 0, sizeof(CItem*)* eWearedItem_Max);

	for (int i=0;i<eWearedItem_Max;i++)
	{
		m_OtherPlayerEquip[i] = NULL;
	}

	m_bNeedMove = FALSE;



	bAllowedMoveToPos = false;

	m_SkipKyungong = false;

	vibLeft=0;
	vibright=0;

	vbAllowHaptic = FALSE;
	column = 0;

	Gamepadmode = FALSE;
	
	bDebugInfo = FALSE;

	//InitMainTitleMonsters();
}

CHero::~CHero()
{
	//Release();
	delete(Player[0]);
	SAFE_DELETE(m_WayPointManager);

	//RemoveMainTitleMonsters();
}

void CHero::InitHero(HERO_TOTALINFO * pHeroTotalInfo)
{
	Player[0] = new CXBOXController(1);

	//InitMainTitleMonsters();

	//m_KeyPress[XINPUT_GAMEPAD_A] = 1;
	//m_KeyPress[XINPUT_GAMEPAD_Y] = 1;
	//m_KeyPress[XINPUT_GAMEPAD_B] = 1;
	//m_KeyPress[XINPUT_GAMEPAD_X] = 1;

	VECTOR3 pos;
	pos.x = pos.y = pos.z = 0;
	HERO->m_WayPointManager->SetFlagDestination(pos);

	m_DelayGroup.Init();

	m_bSkillCooling = 0;
	m_MovingAction.Clear();
	m_NextAction.Clear();
	m_SkillStartAction.Clear();
	m_bIsAutoAttackMode = TRUE;
	m_bIsAutoAttacking = FALSE;
	m_bIsKyungGongMode = FALSE;
	m_WayPointManager->ClearAllWayPoint();
	memcpy(&m_HeroTotalInfo, pHeroTotalInfo, sizeof(HERO_TOTALINFO));
		
	m_maxExpPoint = GAMERESRCMNGR->GetMaxExpPoint(GetLevel());
	MUSSANGMGR->SetMussangReady(FALSE);
	MUSSANGMGR->SetMussangMaxPoint(pHeroTotalInfo->MaxMussangPoint);
	STATSMGR->Clear(&m_itemStats);
	HERO->ClearSetitemOption();
	HERO->ClearSetitemQualityOption();
	m_dwLastSocietyAction = gCurTime;
	m_bActionPenalty = TRUE;

	m_dwFollowPlayerIdx = 0;

	m_SkipKyungong = false;
}


#define SKILL_PERSUIT_TICK	1000
BOOL CHero::IsKyungGongModeProcess()
{	
	if (m_bIsKyungGongMode)
	{
		if (MOVEMGR->IsMoving(this) &&
			m_MoveInfo.KyungGongIdx == 0 &&
			GetKyungGongGrade() &&
			GetMoveSpeed() != 0 &&
			GetState() != eObjectState_Ungijosik)
		{
			if (m_MoveInfo.m_bEffectMoving == FALSE)
			{
				if (GetAvatarOption()->bKyungGong || GetShopItemStats()->bKyungGong)
					MOVEMGR->ToggleHeroKyungGongMode();

				else if ((WEATHERMGR->GetWeatherState() == eWS_Snow) &&
					(GetShopItemStats()->Avatar[eAvatar_Dress] == SHOPITEM_COS_CAT_DRESS ||
					GetShopItemStats()->Avatar[eAvatar_Dress] == SHOPITEM_COS_WEDDING_MAN ||
					GetShopItemStats()->Avatar[eAvatar_Dress] == SHOPITEM_COS_WEDDING_WOMAN)
					)
				{
					MOVEMGR->ToggleHeroKyungGongMode();
				}
				else
				{
					CKyungGongInfo* pKGInfo = KYUNGGONGMGR->GetKyungGongInfo(GetKyungGongGrade());
					DWORD ElapsedTime = gCurTime - m_ObjectState.State_Start_Time;
					DWORD dwSkip =0 ;
					
					switch (m_SkipKyungong)
					{
					case TRUE:
						if (jTweak)	dwSkip = 0;							
						else		dwSkip = pKGInfo->GetChangeTime();
					break;
					case FALSE:
						dwSkip = 0;
					break;
					}

					if (ElapsedTime > dwSkip)
					{
						MOVEMGR->ToggleHeroKyungGongMode();
					}
				}
			}
		}
	}
	//below if character get stuck in tile
	//-----------------------------------------------------	
	if (MAP->CollisionCheck_OneTile(&GetPosition()) == FALSE)
	{
		//static DWORD dwTempTiming;
		//if (gCurTime - dwTempTiming > 40/*2000*/)
		{
			//dwTempTiming = gCurTime;
			VECTOR3 vecDest = m_WayPointManager->GetFlagDestination();
			static int l_Count = 0;
			static int StuckCount = 0;
			l_Count++;
			StuckCount++;
			if (jTweakToggle)
			{
				char temptemp[1024];
				sprintf(temptemp, "Tile Hit! : %d - (%7.2f,%7.2f)", l_Count, GetPosition().x, GetPosition().z);
				CHATMGR->AddMsg(0, temptemp);

				sprintf(temptemp, "StuckCount:%d", StuckCount);
				CHATMGR->AddMsg(CTC_SYSMSG, temptemp);
				//LOGFILE("%u", temptemp);
			}
			if (StuckCount >= 8)
				StuckCount = 0;
		
			MoveStuck(StuckCount);
			
		}
	}
	//-----------------------------------------------------
	return TRUE;
}
BOOL CHero::WayPoint_IsWayPointEmpty()
{
	if (m_WayPointManager)
	{
		return m_WayPointManager->IsWayPointEmpty();
	}
	return FALSE;
}
void CHero::CancelWayPoint()
{
	if (m_WayPointManager)
	{
		return m_WayPointManager->ClearAllWayPoint();
	}
}
extern BOOL bAllowToShowNotify;

char IDNAMETEST[eMotion_Tactic_Excute+1][64] =
{
//#include "WindowIDs.h"

};
void CHero::Process()
{
	

	static DWORD	dwSkillPersuitTime	= 0;

	CPlayer::Process();

	

	LUCKERMGR->Process();

	MINIPINGMGR->Process();
	
	CAMERA->Process();	

	WEATHERMGR->Process();

	sCPlayerAI.Process();    
	
	AUTOPATH->Process();

	//SAFEAREAMGR->Process();

	Move_KeyboardInput();

	if (!bAllowToShowNotify)
		GamePadMoveProcess();


	//ProcessMainTitleMonsters();

	

	//CHATMGR->AddMsg(CTC_SYSMSG, "%d", IDSEARCH("OTI_SP_AUTOHUNT_TIMER_TIME_01"));
	
	
	if(m_MovingAction.HasAction())
	{
		if(m_MovingAction.CheckTargetDistance(&GetCurPosition()) == TRUE)
		{
			m_WayPointManager->ClearAllWayPoint();
			ClearWayPoint();
			MOVEMGR->HeroMoveStop();
			HERO->SetFollowPlayer( 0 );
			m_MovingAction.ExcuteAction(this);
			m_MovingAction.Clear();
		}
		else
		{
			if( m_MovingAction.GetActionKind() == eActionKind_Skill )
			{
				if( gCurTime - dwSkillPersuitTime > SKILL_PERSUIT_TICK )
				{
					HERO->SetFollowPlayer( 0 );
					m_MovingAction.ExcuteAction(this);
					dwSkillPersuitTime = gCurTime;
				}
			}
			else if( MOVEMGR->IsMoving(this) == FALSE &&
				WayPoint_IsWayPointEmpty() &&
				GetState() != eObjectState_Move &&
				GetState() != eObjectState_Rest &&
				m_WayPointManager->GetWMStatus() != eWMStatus_Active)
			{
				m_MovingAction.Clear();
			}			
		}
	}
	else
	{
		if(m_bIsAutoAttacking)
		{
			if(m_NextAction.HasAction() == FALSE)
			{
				if(GetCurComboNum() < 2 || GetCurComboNum() == SKILL_COMBO_NUM)
				{
					if(SKILLMGR->OnSkillCommand(this,&m_AutoAttackTarget,FALSE) == FALSE)
						DisableAutoAttack();
				}
			}
		}
	}

	SKILLMGR->UpdateSkillObjectTargetList(this);

	IsKyungGongModeProcess();

	if(m_bSkillCooling)
	{
		if(gCurTime - m_SkillCoolTimeStart > m_SkillCoolTimeDuration)
			m_bSkillCooling = FALSE;
	}

	VECTOR3 pos;
	float angle;
	GetPosition(&pos);
	angle = GetAngle();
	MAP->SetShadowPivotPos(&pos);
	AUDIOMGR->SetListenerPosition(&pos,angle);
	if (GetState() == eObjectState_SkillUsing)
	{
		if (gCurTime - GetStateStartTime() > 10000)
		{
			OBJECTSTATEMGR->EndObjectState(this, eObjectState_SkillUsing);
			CSkillObject* pSObj = SKILLMGR->GetSkillObject(TEMP_SKILLOBJECT_ID);
			if (pSObj)
				SKILLMGR->DeleteTempSkill();
		}
	}
	gHeroPos = pos;
	m_HeroTotalInfo.Playtime  += gTickTime/1000;
	PEACEWARMGR->DoCheckForPeaceMode(this);
	PETMGR->PetMgrProcess();

	MoveCheckingProcess();
	extern BOOL g_bDisplayFPS;
	if (g_bDisplayFPS)
		RenderMoveDebugInfo();

	
	
}
void CHero::MoveCheckingProcess()
{
	switch (m_WayPointManager->GetWMStatus())
	{
		case eWMStatus_Success:
		{
			VECTOR3 vecDest = m_WayPointManager->GetFlagDestination();
			m_WayPointManager->SetWMStatus(eWMStatus_Inactive);
			MOVEPOINT->SetPoint(&vecDest);
			m_bNeedMove = TRUE;
			NextMove();
			//m_WayPointManager->SetFlagDestination(vecDest);
			break;
		}
		case eWMStatus_Fail:
		{
			VECTOR3 vecDest = m_WayPointManager->GetFlagDestination();
			
			
			if (MAP->CollisionLine(&GetPosition(), &vecDest, &vecDest, MAP->GetMapNum(), this) == FALSE)
			{
				return;
			}
			if (MOVEMGR->IsMoving(this))
			{
				MOVEMGR->CalcPositionEx(this, gCurTime);
			}
			if (MAP->CollisionCheck_OneLine_New(&GetPosition(), &vecDest) == TRUE)
			{
				Move_Simple(&vecDest);
			//	MOVEPOINT->SetPoint(&vecDest);
			}
			m_WayPointManager->SetWMStatus(eWMStatus_Inactive);
			break;
		}
	}
	if (CheckMove() && m_bNeedMove)
	{
		NextMove();
		HERO->SetSkipKyungong(!true);
		//MAP->SetRenderTileData(1);
		SetMoveDebugInfo(TRUE);
	}
	else
	{
		if (GetState() != eObjectState_Move)//stopped moving
		{
			ClearWayPoint(1);
			HERO->SetSkipKyungong(!false);
			//MAP->SetRenderTileData(0);
			SetMoveDebugInfo(FALSE);
		}
	}
}
void CHero::TargetUpdate(CObject* pObject, MOVE_INFO * pMoveInfo, VECTOR3 * TargetPos)
{
	float dx = TargetPos->x - pMoveInfo->CurPosition.x;
	float dz = TargetPos->z - pMoveInfo->CurPosition.z;
	static DOUBLE total_marks, obtained_marks, percentage, oldpercentage;
	const DOUBLE db = 100;
	obtained_marks = percentage = oldpercentage = 0;
	float Distance = sqrtf(dx*dx + dz*dz);

	//if (obtained_marks==0)
		obtained_marks = (DOUBLE)sqrtf(dx*dx + dz*dz);

	if (total_marks == 0)
	total_marks = (DOUBLE)Distance;
	if (Distance <= 0.01)
	{
		//EndMove(pObject);
		return ;
	}
	float Speed = pObject->GetMoveSpeed();
	float ttt = Speed / Distance;
	percentage = db - ((db * obtained_marks) / total_marks);

	CHATMGR->AddMsg(CTC_SYSMSG, "DEV Speed:%1.0f Distance:%1.01f ttt:%7.4f steps( %2.0f / %2.0f ) %3.0f%%", Speed, Distance, ttt, obtained_marks,total_marks,  percentage);
}
void CHero::RenderMoveDebugInfo()
{
	//TargetUpdate(this, &HERO->m_MoveInfo, &AUTOPATH->GetFlagDestination());
	if (!bDebugInfo) return;

	extern VECTOR3 g_PathFindStart;
	extern VECTOR3 g_PathFindEnd;
	VECTOR3 vecDest = m_WayPointManager->GetFlagDestination();

	static DOUBLE total_marks, obtained_marks, percentage, oldpercentage;
	const DOUBLE db = 100;
	total_marks = obtained_marks = percentage = oldpercentage = 0;

	float distuncetmp = CalcDistanceXZ(&GetPosition(), &vecDest);
	float distuncetmp2 = CalcDistanceXZ(&g_PathFindStart, &g_PathFindEnd);

	obtained_marks = (DOUBLE)distuncetmp;
	total_marks = (DOUBLE)distuncetmp2;

	percentage = db - ((db * obtained_marks) / total_marks);

	static int MinFPS = INT_MAX;
	static int MaxFPS = 0;
	int data, num, count;
	data = percentage;
	if (data < MinFPS)
		MinFPS = data;
	if (data > INT_MIN)
		MaxFPS = data;

	static BOOL debugswitch;
	if (debugswitch)
	{
		debugswitch = FALSE;
		if (percentage > 0)
		{
			TCHAR text[MAX_PATH] = { 0 };
			_stprintf(text, 
				"%3.0f%%\t\
				%d\
				steps( %2.0f / %2.0f )",				
				(DOUBLE)MaxFPS, 
				MAP->GetPath_Optimize_Debug().size(),
				sqrt(obtained_marks), 
				sqrt(total_marks)				
				);
			GAMEIN->GetShoutchatDlg()->AddMsg(text, 1);

			//MOVE_INFO* pMoveInfo = &HERO->m_MoveInfo;
			
		}
	}

	if (!((int)obtained_marks == 0))
	{
		debugswitch = TRUE;
	}
	else
	{

		bDebugInfo = FALSE;
		percentage = 0;
	//	GAMEIN->GetShoutchatDlg()->AddMsg("done", 2);
	}
}
BOOL CHero::CheckMove()
{
	VECTOR3 t_Pos;
	GetPosition(&t_Pos);
	if(t_Pos.x == m_WayPoint.x&&t_Pos.z == m_WayPoint.z)
		return TRUE;
	else
		return FALSE;
}
void CHero::GetHeroTotalInfo(HERO_TOTALINFO * info)
{
	memcpy(info, &m_HeroTotalInfo, sizeof(HERO_TOTALINFO));
}


void CHero::ApplyInterface()
{
	SetMoney(m_HeroTotalInfo.Money);
	SetMallMoney(m_HeroTotalInfo.MallMoney);
	SetGoldMoney(m_HeroTotalInfo.GoldMoney);	
	SetMarryName(m_CharacterInfo.MarryName);
	//SetFlashName(m_CharacterInfo.FlashName);
	SetGoldMoney(m_HeroTotalInfo.GoldMoney);	
	SetKillCount(m_HeroTotalInfo.dwKillPlayerTimes);
	SetLevel(m_CharacterInfo.Level);
	SetMaxLife(m_CharacterInfo.MaxLife);
	SetLife(m_CharacterInfo.Life, 0);
	SetMaxNaeRyuk(m_HeroTotalInfo.wMaxNaeRyuk);
	SetNaeRyuk(m_HeroTotalInfo.wNaeRyuk, 0);
	SetMaxShield(m_CharacterInfo.MaxShield);
	SetShield(m_CharacterInfo.Shield, 0);
	SetMaxExpPoint(m_maxExpPoint);
	SetExpPoint(m_HeroTotalInfo.ExpPoint);	
	SetGenGol(m_HeroTotalInfo.wGenGol);
	SetMinChub(m_HeroTotalInfo.wMinChub);
	SetCheRyuk(m_HeroTotalInfo.wCheRyuk);
	SetSimMek(m_HeroTotalInfo.wSimMek);	
	SetPartyIdx(m_HeroTotalInfo.PartyID);
	SetMunpaMemberRank(m_CharacterInfo.PositionInMunpa);
	SetMunpaID(m_CharacterInfo.MunpaID);
	SetGuildName(m_CharacterInfo.GuildName);
	SetGuageName(m_BaseObjectInfo.ObjectName);
	SetMunpa();
	SetBadFame(m_CharacterInfo.BadFame);
	SetStage(m_CharacterInfo.Stage);
	GAMEIN->GetCharacterDialog()->RefreshPointInfo();//RefreshLevelupPoint();
	GAMEIN->GetSkPointDlg()->RefreshAbilityPoint();

	//SetTopListFlg(7);
}

void CHero::SetGuageName(char * szName)
{
	cStatic* ps = (cStatic *)GAMEIN->GetGuageDlg()->GetWindowForID(CG_GUAGENAME);
	if( ps )	ps->SetStaticText( szName );
}

void CHero::SetMaxLife(DWORD maxlife)
{
	CPlayer::SetMaxLife(maxlife);

	DWORD newMaxLife = GetMaxLife();
	if(newMaxLife == 0)
	{
		newMaxLife = 1;
	}
	((CObjectGuagen*)GAMEIN->GetGuageDlg()->GetWindowForID(CG_GUAGELIFE))->SetValue(
		(float)m_CharacterInfo.Life/(float)newMaxLife, 0 );

	// æª?”ª?´æ????ªæ???æ»²å”³
	GAMEIN->GetCharacterDialog()->SetLife(m_CharacterInfo.Life);

	// æª?”ª?´æ???: ?±ç? ?’è?
	char szValue[50];
	sprintf( szValue, "%d / %d", GetLife(), newMaxLife );

	cStatic* ps = (cStatic *)GAMEIN->GetGuageDlg()->GetWindowForID(CG_GUAGELIFETEXT);
	if( ps )	ps->SetStaticText( szValue );	
}


void CHero::SetMaxShield(DWORD maxShield)
{
	CPlayer::SetMaxShield(maxShield);

	DWORD newMaxShield = GetMaxShield();
	if(newMaxShield == 0)
	{
		newMaxShield = 1;
	}
	((CObjectGuagen*)GAMEIN->GetGuageDlg()->GetWindowForID(CG_GUAGESHIELD))->SetValue(
		(float)m_CharacterInfo.Shield/(float)newMaxShield, 0 );
	GAMEIN->GetCharacterDialog()->SetShield(m_CharacterInfo.Shield);
	char szValue[50];
	sprintf( szValue, "%d / %d", GetShield(), newMaxShield );
	cStatic* ps = (cStatic *)GAMEIN->GetGuageDlg()->GetWindowForID(CG_GUAGESHIELDTEXT);
	if( ps )	ps->SetStaticText( szValue );
}
void CHero::Heal(CObject* pHealer,BYTE HealKind,DWORD HealVal)
{
	ChangeLife(HealVal);
}
void CHero::ChangeLife(int changeval)
{
	DWORD newlife = GetLife() + changeval;
	SetLife(newlife);	
	if( m_bUngiFlag == FALSE )
	if( newlife )
	if( (GetMaxLife()/newlife) > 2 )
	{
		GAMEEVENTMGR->AddEvent(eGameEvent_Ability, 401);
		m_bUngiFlag = TRUE;
	}
}
void CHero::ChangeShield(int changeval)
{
	if(changeval < 0)
	{
		ASSERT((int)GetShield() >= (-changeval));
	}
	
	DWORD newShield = GetShield() + changeval;
	DWORD maxShield = GetMaxShield();

	SetShield(newShield);
}
void CHero::SetLife(DWORD life, BYTE type)
{
	CPlayer::SetLife(life);

	DWORD MaxLife = GetMaxLife();
	if(MaxLife == 0)
	{
		MaxLife = 1;
	}

	if (life > MaxLife)
		life = MaxLife;
	
	
	if(GetState() == eObjectState_Die)
		life = 0;

	//TITANMGR->CheckNotifyMsg(TNM_MASTER_LIFE);

	((CObjectGuagen*)GAMEIN->GetGuageDlg()->GetWindowForID(CG_GUAGELIFE))->SetValue(
		(float)life/(float)MaxLife, 
		(type == 0 ? 0 : (2000/MaxLife)*life) );
	GAMEIN->GetCharacterDialog()->SetLife(life);

	char szValue[50];
	sprintf( szValue, "%d / %d", life, GetMaxLife() );
	cStatic* ps = (cStatic *)GAMEIN->GetGuageDlg()->GetWindowForID(CG_GUAGELIFETEXT);
	if( ps )	ps->SetStaticText( szValue );
}

void CHero::SetShield(DWORD Shield, BYTE type)
{
	CPlayer::SetShield(Shield);

	DWORD MaxShield = GetMaxShield();
	if(MaxShield == 0)
	{
		MaxShield = 1;
	}
	if (Shield > MaxShield)
		Shield = MaxShield;
	
	if(GetState() == eObjectState_Die)
		Shield = 0;

	((CObjectGuagen*)GAMEIN->GetGuageDlg()->GetWindowForID(CG_GUAGESHIELD))->SetValue(
		(float)Shield/(float)MaxShield, 
		(type == 0 ? 0 : (2000/MaxShield)*Shield) );
	GAMEIN->GetCharacterDialog()->SetShield(Shield);

	char szValue[50];
	sprintf( szValue, "%d / %d", Shield, GetMaxShield() );
	cStatic* ps = (cStatic *)GAMEIN->GetGuageDlg()->GetWindowForID(CG_GUAGESHIELDTEXT);
	if( ps )	ps->SetStaticText( szValue );
}

void CHero::SetMoney(MONEYTYPE Money)
{
	m_HeroTotalInfo.Money = Money;
	if(GAMEIN->GetInventoryDialog())
	{
		GAMEIN->GetInventoryDialog()->SetMoney(Money);
	}
	if(GAMEIN->GetMoneyPacketDialog())
	{
       GAMEIN->GetMoneyPacketDialog()->SetMoney(Money);
	}
	if (GAMEIN->GetInventoryDialog())	//ÊµÊ±Ë¢ÐÂ±³°ü½ðÇ®
	{
		GAMEIN->GetInventoryDialog()->SetBackMoney(Money);
	}
}
void CHero::SetMallMoney(MONEYTYPE MallMoney)
{
	m_HeroTotalInfo.MallMoney=MallMoney;
	if(GAMEIN->GetGoldShopDialog())
	{
       GAMEIN->GetGoldShopDialog()->SetMallMoney(MallMoney);
	}
	if(GAMEIN->GetMoneyPacketDialog())
	{
       GAMEIN->GetMoneyPacketDialog()->SetMallMoney(MallMoney);
	}
	if (GAMEIN->GetInventoryDialog())	//ÊµÊ±Ë¢ÐÂ±³°üÅÝµã
	{
		GAMEIN->GetInventoryDialog()->SetBackMallMoney(MallMoney);
	}
}
void CHero::SetGoldMoney(MONEYTYPE GoldMoney)
{
	m_HeroTotalInfo.GoldMoney=GoldMoney;
	if(GAMEIN->GetGoldShopDialog())
	{
       GAMEIN->GetGoldShopDialog()->SetGoldMoney(GoldMoney);
	}
	if(GAMEIN->GetMoneyPacketDialog())
	{
       GAMEIN->GetMoneyPacketDialog()->SetGoldMoney(GoldMoney);
	}
	if (GAMEIN->GetInventoryDialog())	//ÊµÊ±Ë¢ÐÂ±³°üÔª±¦
	{
		GAMEIN->GetInventoryDialog()->SetBackGoldMoney(GoldMoney);
	}
}
void CHero::SetMarryName(char * pName)
{
	if (pName == NULL)
		return;
	SafeStrCpy( m_CharacterInfo.MarryName,pName, MAX_NAME_LENGTH + 1 );	
	if (IsMarry())
	{
		CHAR balloon[MAX_MASTERNAME_LENGTH + 1];
		if (GetGender() == 0)
		{
			sprintf(balloon, CHATMGR->GetChatMsg(2011), GetMarryName());
		}
		else
		{
			sprintf(balloon, CHATMGR->GetChatMsg(2012), GetMarryName());
		}
		SetMarryNameBalloon(balloon);
	}
	else
	{
		SetMarryNameBalloon(pName);
	}
}

char* CHero::GetMarryName()
{
	return m_CharacterInfo.MarryName;
}
void CHero::SetKyungGongGrade(WORD grade)
{	
	m_HeroTotalInfo.KyungGongGrade = grade;
	GAMEIN->GetCharacterDialog()->UpdateData();
}

void CHero::SetMaxExpPoint(EXPTYPE dwPoint)
{
	m_maxExpPoint = dwPoint;
	if (m_maxExpPoint == 0 || GetLevel()>= GAMERESRCMNGR->GetMaxLevelControl()/*100*/)
	{
		m_maxExpPoint = 4000000000;
	}

	double value = (double)GetExpPoint()/(double)m_maxExpPoint;

	((CObjectGuagen*)GAMEIN->GetGuageDlg()->GetWindowForID(CG_GUAGEEXPPOINT))->SetValue( (GUAGEVAL)value, 0 );
	GAMEIN->GetCharacterDialog()->SetExpPointPercent( (float)(value*100) );
	char szValue[50];
	sprintf( szValue, "%I64d / %I64d", GetExpPoint(), dwPoint );
	cStatic* ps = (cStatic *)GAMEIN->GetGuageDlg()->GetWindowForID(CG_GUAGEEXPPOINTTEXT);
	if( ps )	ps->SetStaticText( szValue );
}
void CHero::SetExpPoint(EXPTYPE dwPoint, BYTE type)
{
	m_HeroTotalInfo.ExpPoint = dwPoint;
	float value = (float)dwPoint/(float)m_maxExpPoint;
	double value2 = value / 0.2f;
	double value3 = value2 - (EXPTYPE)value2;
	static double value4 = 0.0f;

	DWORD valueTime = (type == 0)? 0 : (DWORD)((2000/m_maxExpPoint)*dwPoint);

	((CObjectGuagen*)GAMEIN->GetGuageDlg()->GetWindowForID(EXP_POINT1))->SetActive(FALSE);
	((CObjectGuagen*)GAMEIN->GetGuageDlg()->GetWindowForID(EXP_POINT2))->SetActive(FALSE);
	((CObjectGuagen*)GAMEIN->GetGuageDlg()->GetWindowForID(EXP_POINT3))->SetActive(FALSE);
	((CObjectGuagen*)GAMEIN->GetGuageDlg()->GetWindowForID(EXP_POINT4))->SetActive(FALSE);

	if(GetLevel() == GAMERESRCMNGR->GetMaxLevelControl() || type == 0 )
		((CObjectGuagen*)GAMEIN->GetGuageDlg()->GetWindowForID(CG_GUAGEEXPPOINT))->SetValue( 1, 0 );
	else
	{
		ySWITCH((int)value2)
		yCASE(1)
			((CObjectGuagen*)GAMEIN->GetGuageDlg()->GetWindowForID(EXP_POINT1))->SetActive(TRUE);
		yCASE(2)
			((CObjectGuagen*)GAMEIN->GetGuageDlg()->GetWindowForID(EXP_POINT1))->SetActive(TRUE);
			((CObjectGuagen*)GAMEIN->GetGuageDlg()->GetWindowForID(EXP_POINT2))->SetActive(TRUE);
		yCASE(3)
			((CObjectGuagen*)GAMEIN->GetGuageDlg()->GetWindowForID(EXP_POINT1))->SetActive(TRUE);
			((CObjectGuagen*)GAMEIN->GetGuageDlg()->GetWindowForID(EXP_POINT2))->SetActive(TRUE);
			((CObjectGuagen*)GAMEIN->GetGuageDlg()->GetWindowForID(EXP_POINT3))->SetActive(TRUE);
		yCASE(4)
			((CObjectGuagen*)GAMEIN->GetGuageDlg()->GetWindowForID(EXP_POINT1))->SetActive(TRUE);
			((CObjectGuagen*)GAMEIN->GetGuageDlg()->GetWindowForID(EXP_POINT2))->SetActive(TRUE);
			((CObjectGuagen*)GAMEIN->GetGuageDlg()->GetWindowForID(EXP_POINT3))->SetActive(TRUE);
			((CObjectGuagen*)GAMEIN->GetGuageDlg()->GetWindowForID(EXP_POINT4))->SetActive(TRUE);
		yENDSWITCH

		if((EXPTYPE)value4 == (EXPTYPE)value2)
			((CObjectGuagen*)GAMEIN->GetGuageDlg()->GetWindowForID(CG_GUAGEEXPPOINT))->SetValue( (GUAGEVAL)value3, valueTime );
		else
		{
			((CObjectGuagen*)GAMEIN->GetGuageDlg()->GetWindowForID(CG_GUAGEEXPPOINT))->SetValue( (GUAGEVAL)value3, 0 );
			value4 = value2;
		}
	}

	////////////////////////////////////////////////////////////////////////////////////


	// æª?”ª?´æ???: è­°è?????
	CObjectGuagen* ps = (CObjectGuagen *)GAMEIN->GetCharacterDialog()->GetWindowForID(CI_EXPPOINTGUAGE);
	if( ps )
	{
		if(type == 0)
			ps->SetValue( (GUAGEVAL)value, 0 );
		else
			ps->SetValue( (GUAGEVAL)value, valueTime );
	}	

	GAMEIN->GetCharacterDialog()->SetExpPointPercent((float)(value*100));
	// æª?”ª?´æ???: ?±ç? ?’è?

	char szValue[50];
	if(GetLevel() >= GAMERESRCMNGR->GetMaxLevelControl())//çµžè„¹??=99å¥€,è¿¡ä?è¶??‹å??Šå?121??
		sprintf( szValue, "[ %s ]", CHATMGR->GetChatMsg(179) );	//"[ é§?]"
	else
		sprintf( szValue, "%.2f%% / %.2f%%", value3*100, value*100 );	// magi82

#ifdef _huhu_	
	if(CHEATMGR->IsCheatEnable())
	{
		char buf1[66];
		wsprintf(buf1, " %I64d/%I64d", dwPoint, GetMaxExpPoint());
		strcat(szValue, buf1);
	}
#endif

	cStatic* ps2 = (cStatic *)GAMEIN->GetGuageDlg()->GetWindowForID(CG_GUAGEEXPPOINTTEXT);
	if( ps2 )	ps2->SetStaticText( szValue );
}
void CHero::SetLevel(LEVELTYPE level)
{
	GAMEIN->GetCharacterDialog()->SetLevel(level);
	cStatic* ps = (cStatic *)GAMEIN->GetGuageDlg()->GetWindowForID(CG_GUAGELEVEL);
	if( ps )	ps->SetStaticValue( level );

	if(level > CPlayer::GetLevel())
	{
		if(InTitan() == TRUE)
			EFFECTMGR->StartEffectProcess(eEffect_TitanLevelUpSentence,HERO,NULL,0,0);
		else
			EFFECTMGR->StartEffectProcess(eEffect_LevelUpSentence,HERO,NULL,0,0);
	}
	CPlayer::SetLevel(level);
	ITEMMGR->RefreshAllItem();
	MUGONGMGR->RefreshMugong();
	if( GAMEIN->GetQuickDialog() )
		GAMEIN->GetQuickDialog()->RefreshIcon();

	//sCPlayerAI.DufualtUsekyungGong();

}

void CHero::SetMaxNaeRyuk(DWORD val)
{
	m_HeroTotalInfo.wMaxNaeRyuk = val;
	DWORD newMaxNaeRyuk = GetMaxNaeRyuk();
	if(newMaxNaeRyuk == 0)
	{
		newMaxNaeRyuk = 1;
	}
	((CObjectGuagen*)GAMEIN->GetGuageDlg()->GetWindowForID(CG_GUAGENAERYUK))->SetValue(
		(float)m_HeroTotalInfo.wNaeRyuk/(float)newMaxNaeRyuk, 0 );

	GAMEIN->GetCharacterDialog()->SetNaeRyuk(m_HeroTotalInfo.wNaeRyuk);
	char szValue[50];
	sprintf( szValue, "%d / %d", GetNaeRyuk(), newMaxNaeRyuk );
	cStatic* ps = (cStatic *)GAMEIN->GetGuageDlg()->GetWindowForID(CG_GUAGENAERYUKTEXT);
	if( ps )	ps->SetStaticText( szValue );
}

void CHero::SetNaeRyuk(DWORD val, BYTE type)
{
	if(val > GetMaxNaeRyuk()) 
	{
		val = GetNaeRyuk();
	}
	DWORD dwMaxNaeRyuk = GetMaxNaeRyuk();

	if(m_HeroTotalInfo.wMaxNaeRyuk != 0)
		((CObjectGuagen*)GAMEIN->GetGuageDlg()->GetWindowForID(CG_GUAGENAERYUK))->SetValue(
		(float)val/(float)dwMaxNaeRyuk, 
		( type == 0 ? 0 : (1000/dwMaxNaeRyuk)*val) );

	GAMEIN->GetCharacterDialog()->SetNaeRyuk(val);

	char szValue[50];
	sprintf( szValue, "%d / %d", val, dwMaxNaeRyuk );
	cStatic* ps = (cStatic *)GAMEIN->GetGuageDlg()->GetWindowForID(CG_GUAGENAERYUKTEXT);
	if( ps )	ps->SetStaticText( szValue );
	m_HeroTotalInfo.wNaeRyuk = val;
}
void CHero::SetGenGol(WORD val)
{
	m_HeroTotalInfo.wGenGol = val;

	STATSMGR->CalcItemStats(this);
	
	GAMEIN->GetCharacterDialog()->SetGenGol();
	GAMEIN->GetCharacterDialog()->SetAttackRate();
	GAMEIN->GetCharacterDialog()->UpdateData();
}
void CHero::SetMinChub(WORD val)
{
	m_HeroTotalInfo.wMinChub = val;

	STATSMGR->CalcItemStats(this);
	//STATSMGR->CalcCharStats(this);
	

	// æª?”ª?´æ??????’è? ?±ç? æ»²å”³
	GAMEIN->GetCharacterDialog()->SetMinChub();
	GAMEIN->GetCharacterDialog()->SetAttackRate();
	GAMEIN->GetCharacterDialog()->UpdateData();
	GAMEIN->GetCharacterDialog()->UpdateMoveSpeedDisplay();
}
void CHero::SetCheRyuk(WORD val)
{
	m_HeroTotalInfo.wCheRyuk = val;

	STATSMGR->CalcItemStats(this);
	//STATSMGR->CalcCharStats(this);
	
	// æª?”ª?´æ??????’è? ?±ç? æ»²å”³
	GAMEIN->GetCharacterDialog()->SetCheRyuk();
	GAMEIN->GetCharacterDialog()->SetDefenseRate();
	GAMEIN->GetCharacterDialog()->UpdateData();
}
void CHero::SetSimMek(WORD val)
{
	m_HeroTotalInfo.wSimMek = val;

	STATSMGR->CalcItemStats(this);
	//STATSMGR->CalcCharStats(this);
	
	// æª?”ª?´æ??????’è? ?±ç? æ»²å”³
	GAMEIN->GetCharacterDialog()->SetSimMek();
	GAMEIN->GetCharacterDialog()->UpdateData();
	GAMEIN->GetCharacterDialog()->SetShield(GetShield());
	GAMEIN->GetCharacterDialog()->SetNaeRyuk(GetNaeRyuk());
}
void CHero::SetMunpa()
{
	GAMEIN->GetCharacterDialog()->RefreshGuildInfo();
}

void CHero::SetFame(FAMETYPE val)
{
	m_HeroTotalInfo.Fame = val;
	GAMEIN->GetCharacterDialog()->SetFame(GetFame());
	if(GetFame()!=-1)
	{
		CHAR balloon[MAX_MASTERNAME_LENGTH + 1];
		sprintf(balloon, "%d", GetFame());
		SetFameBalloon(balloon);

		CHAR famerank[MAX_MASTERNAME_LENGTH + 1];
		ZeroMemory(&famerank, sizeof(famerank));
		sprintf(famerank,"%s",FAMEMGR->GetFameNameString(GetFame()));
		SetFameRankBalloon(famerank);
	}
	else
	{
		SetFameBalloon("0");
	}
}

void CHero::SetBadFame( FAMETYPE val )
{
	m_CharacterInfo.BadFame = val;
	GAMEIN->GetCharacterDialog()->SetBadFame( val );	
}

BOOL CHero::StartSocietyAct( int nStartMotion, int nIngMotion, int nEndMotion, BOOL bHideWeapon )
{
	if( GetState() != eObjectState_None && GetState() != eObjectState_Immortal )
	{
		if( GetState() == eObjectState_Society )
		{
			if( m_ObjectState.State_End_Motion != -1 )
			{
				if( m_ObjectState.bEndState == TRUE )
					return FALSE;
				if( gCurTime - GetStateStartTime() < GetEngineObject()->GetAnimationTime(m_ObjectState.State_Start_Motion ) )
					return FALSE;

				OBJECTSTATEMGR->EndObjectState( this, GetState(), GetEngineObject()->GetAnimationTime( m_ObjectState.State_End_Motion ) );
				return TRUE;
			}
		}
		else
		{
			return FALSE;
		}
	}
	m_ObjectState.State_Start_Motion	= (short int)nStartMotion;
	m_ObjectState.State_Ing_Motion		= (short int)nIngMotion;
	m_ObjectState.State_End_Motion		= (short int)nEndMotion;

	if( nEndMotion != -1 )
		m_ObjectState.State_End_MotionTime = GetEngineObject()->GetAnimationTime(nEndMotion);
	else
		m_ObjectState.State_End_MotionTime = 0;
	if(m_ObjectState.State_Start_Motion != -1)
	{
		ChangeMotion( m_ObjectState.State_Start_Motion, FALSE );
		if( m_ObjectState.State_Ing_Motion != -1 )
			ChangeBaseMotion( m_ObjectState.State_Ing_Motion );
	}
	if( bHideWeapon && GetWeaponEquipType() != WP_GWUN)
		APPEARANCEMGR->HideWeapon( this );
	//m_ObjectState.BeforeState		= m_BaseObjectInfo.ObjectState;
	m_BaseObjectInfo.ObjectState	= eObjectState_Society;
	m_ObjectState.State_Start_Time	= gCurTime;
	m_ObjectState.bEndState			= FALSE;	
	if( m_ObjectState.State_Ing_Motion == -1 )
		OBJECTSTATEMGR->EndObjectState( this, eObjectState_Society, GetEngineObject()->GetAnimationTime( m_ObjectState.State_Start_Motion ) );

	return TRUE;
}

BOOL CHero::EndSocietyAct()
{
	if( m_ObjectState.State_Ing_Motion != -1 )
	{
		return FALSE;
	}
	return TRUE;
}

void CHero::StartSocietyActForQuest( int nStartMotion, int nIngMotion /* = -1 */, int nEndMotion /* = -1 */, BOOL bHideWeapon /* = FALSE  */ )
{
	m_ObjectState.State_Start_Motion = (short int)nStartMotion;
	m_ObjectState.State_Ing_Motion = (short int)nIngMotion;
	m_ObjectState.State_End_Motion = (short int)nEndMotion;

	if( nEndMotion != -1 )
		m_ObjectState.State_End_MotionTime = GetEngineObject()->GetAnimationTime(nEndMotion);
	else
		m_ObjectState.State_End_MotionTime = 0;

	if( m_ObjectState.State_Start_Motion != -1 )
	{
		ChangeMotion( m_ObjectState.State_Start_Motion, FALSE );
		if( m_ObjectState.State_Ing_Motion != -1 )
			ChangeBaseMotion( m_ObjectState.State_Ing_Motion );
	}
	if( bHideWeapon )
		APPEARANCEMGR->HideWeapon( this );
}

void CHero::OnStartObjectState(BYTE State)
{
	CPlayer::OnStartObjectState(State);
}
void CHero::OnEndObjectState(BYTE State)
{
	switch(State)
	{
	case eObjectState_Deal:
		{
			HERO->HeroStateNotify(eObjectState_Deal);
		}
		break;
		
	case eObjectState_SkillUsing:
	case eObjectState_SkillBinding:
		SKILLMGR->OnComboTurningPoint(this);
		break;
		
	case eObjectState_SkillSyn:
		{
#ifndef _TESTCLIENT_
			if(SKILLMGR->GetSkillObject(TEMP_SKILLOBJECT_ID) != NULL)
				OBJECTSTATEMGR->StartObjectState(this,eObjectState_SkillUsing);
			else				
				SKILLMGR->OnComboTurningPoint(this);
#else
			SKILLMGR->OnComboTurningPoint(this);
#endif
		}
		break;
		
	case eObjectState_Move:
		{
			if(PARTYMGR->GetTacticObjectID())
			{
				TACTICMGR->HeroTacticJoin();
			}
		}
		break;
	case eObjectState_SkillStart:
		{
			if(m_SkillStartAction.GetActionKind() != eActionKind_Skill_RealExecute)
				break;
			
			m_SkillStartAction.ExcuteAction(this);
			m_SkillStartAction.Clear();
		}
		break;
	}
	CPlayer::OnEndObjectState(State);
}
#include "MainBarDialog.h"
void CHero::Die(CObject* pAttacker,BOOL bFatalDamage,BOOL bCritical, BOOL bDecisive)
{
	if(AISETMGR->GetGameAI()->sPage5.cbAutoRespawnTimeLimit)
	{
		CHATMGR->AddMsg(CTC_SYSMSG,CHATMGR->GetChatMsg(2637),sCPlayerAI.DeathCounterUp(),AISETMGR->GetGameAI()->sPage5.spReviveTimeLimit);
		if(sCPlayerAI.GetDeathCount()==AISETMGR->GetGameAI()->sPage5.spReviveTimeLimit)
		{
			sCPlayerAI.SetAIDisable();
		}
	}
	if(sCPlayerAI.IsOpen() && !AISETMGR->GetGameAI()->sPage5.cbAutoRespawnTimeLimit)
	{
		GAMEIN->GetMainInterfaceDialog()->SetAlram( OPT_PRESETFORDEAD, TRUE );
		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg( 2495 ) );
	}

	if( GAMEIN->GetFWEngraveDlg()->IsActive() )
			KillEngravingDlg();

	if(HERO->GetState() == eObjectState_Engrave)
	{
		KillEngravingDlg();
	}
	if( CPlayer::GetID() == HEROID )
	{
		if( GAMEIN->GetFWEngraveDlg()->IsActive() )
			KillEngravingDlg();
	}
	
	WINDOWMGR->CloseAllMsgBox();

	GAMEIN->GetDealDialog()->SetDisable(FALSE);
	GAMEIN->GetDealDialog()->SetActive(FALSE);

	GAMEIN->GetNpcScriptDialog()->SetDisable(FALSE);
	GAMEIN->GetNpcScriptDialog()->SetActive(FALSE);

	if( GAMEIN->GetUpgradeDialog()->IsActive() &&
		!GAMEIN->GetUpgradeDialog()->IsDisable() )
	{
		GAMEIN->GetUpgradeDialog()->Release();
		GAMEIN->GetUpgradeDialog()->SetActiveRecursive( FALSE );
	}
	if( GAMEIN->GetMixDialog()->IsActive() &&
		!GAMEIN->GetMixDialog()->IsDisable() )
	{
		GAMEIN->GetMixDialog()->Release();
		GAMEIN->GetMixDialog()->SetActiveRecursive( FALSE );
	}
	if( GAMEIN->GetReinforceDialog()->IsActive() &&
		!GAMEIN->GetReinforceDialog()->IsDisable() )
	{
		GAMEIN->GetReinforceDialog()->Release();
		GAMEIN->GetReinforceDialog()->SetActiveRecursive( FALSE );
	}
	if( GAMEIN->GetDissolutionDialog()->IsActive() &&
		!GAMEIN->GetDissolutionDialog()->IsDisable() )
	{
		GAMEIN->GetDissolutionDialog()->SetActive( FALSE );
	}

	if( GAMEIN->GetSkPointDlg()->IsActive() )
	{
		GAMEIN->GetSkPointDlg()->SetActive( FALSE );
	}

	SetMovingAction(NULL);
	DisableAutoAttack();
	SetLife(0);
	
	STREETSTALLMGR->CloseStreetStall();
	
	if( pAttacker )
	if(pAttacker->GetObjectKind() & eObjectKind_Monster )
	{
		if( GetLevel() < 5 )
			GAMEEVENTMGR->AddEvent(eGameEvent_Die);
		else
			GAMEEVENTMGR->AddEvent(eGameEvent_DieAfter5);
	}

	if(GAMEIN->GetCharStateDialog())
		GAMEIN->GetCharStateDialog()->SetUngiMode( FALSE );

	DWORD time = 0;
	if( bFatalDamage )
		time = HERO->GetEngineObject()->GetAnimationTime(eMotion_Died_Fly);
	else
		time = HERO->GetEngineObject()->GetAnimationTime(eMotion_Died_Normal);
	CAMERA->SetCharState( eCS_Die, time );

	CPlayer::Die(pAttacker,bFatalDamage,bCritical,bDecisive);
	
	GAMEIN->GetCharacterDialog()->SetAttackRate();
	GAMEIN->GetCharacterDialog()->SetDefenseRate();
	GAMEIN->GetCharacterDialog()->UpdateData();
	GAMEIN->GetCharacterDialog()->UpdateMoveSpeedDisplay();
	if (pAttacker)
	{
		if( HEROID != pAttacker->GetID() )
		{
			if(pAttacker->GetObjectKind() == eObjectKind_Player)
			{
				CBattle* pTargetBattle = BATTLESYSTEM->GetBattle( HERO );
				CBattle* pAttackerBattle = BATTLESYSTEM->GetBattle( pAttacker->GetID() );
				WORD MapNum = MAP->GetMapNum();
				DWORD dwCheckBit = eBossMap | eEventMap | eSurvivalMap | eTournament;

				if(HERO->GetMunpaIdx())
				{
					if( HERO->IsPKMode() == TRUE
						|| (pTargetBattle->GetBattleKind() == eBATTLE_KIND_VIMUSTREET && pTargetBattle->GetBattleID() == pAttackerBattle->GetBattleID())
						|| (PARTYWAR->IsPartyWar() && PARTYWAR->IsEnemy((CPlayer*)pAttacker))
						|| GUILDFIELDWAR->IsEnemy((CPlayer*)pAttacker)
						|| MapNum == SIEGEMGR->GetSiegeWarMapNum()
						|| MAP->IsMapKind(dwCheckBit) 
						|| IsBattleChannel[gChannelNum] && FORTWARMGR->IsWarStart() )
					{
						//int a = 0;
						MSGBASE msg;
						SetProtocol(&msg, MP_GUILD, MP_GUILD_SOS_SEND_CANCEL);
						msg.dwObjectID = HEROID;
						NETWORK->Send(&msg, sizeof(MSGBASE));
					}
					else
					{
						WINDOWMGR->MsgBox( MBI_SOS, MBT_YESNO, CHATMGR->GetChatMsg(1633) );
					}
				}
				sCPlayerAI.StartAutoAttrPalyProcess(NULL);
			}
		}
	}
	SKILLMGR->RemoveTarget(this, SKILLRESULTKIND_POSITIVE|SKILLRESULTKIND_NEGATIVE);

	if( m_bActionPenalty == FALSE )
	{
		WINDOWMGR->MsgBox( MBI_ACTION_PENALTY, MBT_OK, CHATMGR->GetChatMsg(1606) );
		m_bActionPenalty = TRUE;
	}
	if (GUILDWARMGR->IsWarMap())
	{
		GUILDWARMGR->SetDieTime(gCurTime);
	}
	/*if(pAttacker->GetObjectKind()==eObjectKind_Player)
	{
		sCPlayerAI.SetAttrPlayerID(pAttacker->GetID());
	}*/
	ClearWayPoint();
}

void CHero::Damage(CObject* pAttacker,BYTE DamageKind,DWORD Damage,DWORD ShieldDamage,BOOL bCritical, BOOL bDecisive, DWORD titanObserbDamage)
{
	VECTOR3 push;
	MATRIX4 my;
	SetVector3(&push,0,0,30);
	SetRotationYMatrix(&my,-m_RotateInfo.Angle.ToRad());
	TransformVector3_VPTR2(&push,&push,&my,1);
	CAMERA->SetPushedVibration(&push,200);
	//SetXboxVibration(65535,65535);
	if (pAttacker)
	{
		// Do not look in the direction of the attack while moving. otherwise a moonwalker will occur
		if (0 == OBJECTMGR->GetSelectedObject() &&
			FALSE == GetBaseMoveInfo()->bMoving)
		{				
			if (KeyboardMode)
			{
				VECTOR3 position = pAttacker->GetCurPosition() - GetCurPosition();
				GetEngineObject()->SetDirection(&position);
				OBJECTMGR->SetSelectedObject(pAttacker);
			}			
		}
		
		if (pAttacker->GetObjectKind() == eObjectKind_Player && !IsPKMode())
		{
			sCPlayerAI.SetRunWay(FALSE);
			sCPlayerAI.StartAutoAttrPalyProcess(pAttacker);
		}
	}
	CPlayer::Damage(pAttacker,DamageKind,Damage,ShieldDamage,bCritical,bDecisive,titanObserbDamage);	
}

void CHero::OnHitTarget(CObject* pMainTarget)
{
	VECTOR3 targetpos = pMainTarget->GetCurPosition();
	VECTOR3 Curpos = HERO->GetCurPosition();

	float WeaponMinDistance = 0.f;
	switch(GetWeaponEquipType())
	{//kiv
	case WP_GUM:	WeaponMinDistance = 150;	break;
	case WP_GWUN:	WeaponMinDistance = 100;	break;
	case WP_DO:		WeaponMinDistance = 150;	break;
	case WP_CHANG:	WeaponMinDistance = 150;	break;
	case WP_GUNG:	WeaponMinDistance = 100;	break;
	case WP_AMGI:	WeaponMinDistance = 100;	break;	
	case WP_EVENT_HAMMER:WeaponMinDistance = 0.f;break;
	case WP_AXE:	WeaponMinDistance = 100;	break;
	case WP_DAGGER:	WeaponMinDistance =  10;	break;
	//case WP_GUM:	WeaponMinDistance = 120;	break;//150
	//case WP_GWUN:	WeaponMinDistance = 150;	break;//100
	//case WP_DO:		WeaponMinDistance = 150;	break;
	//case WP_CHANG:	WeaponMinDistance = 150;	break;
	//case WP_GUNG:	WeaponMinDistance = 50;	break;//100
	//case WP_AMGI:	WeaponMinDistance = 80;	break;//100	
	//case WP_EVENT_HAMMER:WeaponMinDistance = 0.f;break;
	default: ASSERT(0);
	}
	if(CalcDistanceXZ(&targetpos,&Curpos) < WeaponMinDistance)
	{
		VECTOR3 pos;	pos.x = 0;	pos.y = 0;	pos.z = 10.f;
		float angle = HERO->GetAngle();
		TransToRelatedCoordinate(&pos,&pos,angle);
		pos = pos + Curpos;
		MOVEMGR->HeroEffectMove(HERO,&pos,0,0);
		//CHATMGR->AddMsg(CTC_SYSMSG, "555555555");
	}
}
void CHero::ClearTarget(CObject* pObject)
{
	if(m_AutoAttackTarget.GetTargetID() == pObject->GetID())
		DisableAutoAttack();
	if(m_NextAction.GetTargetID() == pObject->GetID())
	{
		m_NextAction.Clear();
	}
}
void CHero::Revive(VECTOR3* pRevivePos)
{
	DisableAutoAttack();

	CAMERA->SetCharState( eCS_Normal, 0 );
	
	CPlayer::Revive(pRevivePos);
}

BOOL CHero::GetKyungGongMode()
{
	return m_bIsKyungGongMode;
}

void CHero::SetKyungGongMode(BOOL bMode)
{
	m_bIsKyungGongMode = bMode;

	if( m_bIsKyungGongMode == FALSE )
	{
		if(MOVEMGR->IsMoving(this) && m_MoveInfo.KyungGongIdx != 0)
			MOVEMGR->ToggleHeroKyungGongMode();
	}
	else
	{
		if(MOVEMGR->IsMoving(this) && m_MoveInfo.KyungGongIdx != 0)
			MOVEMGR->ToggleHeroKyungGongMode();
	}

//KES CHARSTATEDLG 031016
	if(GAMEIN->GetCharStateDialog())
		GAMEIN->GetCharStateDialog()->SetKyungGongMode( m_bIsKyungGongMode );
//
	
//	OPTIONMGR->SetKyungGongMode(m_bIsKyungGongMode);//***
}


int CHero::GetMugongLevel(WORD MugongIdx)
{
	//if(MugongIdx < 100)		// å·çˆ¾
	//	return 1;

	// !!!!!!!!!!! magi82 - ?³æ???é¡«æ?é©?å·çˆ¾??è©•ç? ?œè?æ©«æ’¿?¼é????–æ????ï???æ©¾æ¬½ æªœæº¢??æ­œè?????!!!!!!!!!!!!!!!!!!1
	if(MugongIdx < 100 || ( 10000 <= MugongIdx && MugongIdx < 10000 + 100 ))		// å·çˆ¾
		return 1;

	CMugongBase* pMugong = MUGONGMGR->GetMugongByMugongIdx(MugongIdx);
	if(pMugong == NULL)
		return 0;

	return pMugong->GetSung();
}

//////////////////////////////////////////////////////////////////////////
// 06. 06. 2???ªé? - æªœè???
// é¼ å¥¢ æ»²ïŸ¹ è¹ºé?
WORD CHero::GetSkillOptionIndex(WORD MugongIdx)
{
	if(MugongIdx < 100)		// å·çˆ¾
		return 0;

	//if( MugongIdx > SKILLNUM_TO_TITAN && InTitan() )
	//	MugongIdx -= SKILLNUM_TO_TITAN;

	CMugongBase* pMugong = MUGONGMGR->GetMugongByMugongIdx(MugongIdx);
	if(pMugong == NULL)
		return 0;

	return pMugong->GetOption();
}
//////////////////////////////////////////////////////////////////////////
void CHero::HeroStatusReload()
{
	//CObject::AddStatus(pStatus);
	
	MOVEMGR->RefreshHeroMove();

	SetMaxLife(m_CharacterInfo.MaxLife);
	SetMaxShield(m_CharacterInfo.MaxShield);
	SetMaxNaeRyuk(m_HeroTotalInfo.wMaxNaeRyuk);
	
	GAMEIN->GetCharacterDialog()->SetAttackRate();
	GAMEIN->GetCharacterDialog()->SetDefenseRate();
	GAMEIN->GetCharacterDialog()->UpdateData();
	GAMEIN->GetCharacterDialog()->UpdateMoveSpeedDisplay();
}
void CHero::AddStatus(CStatus* pStatus)
{
	CObject::AddStatus(pStatus);
	
	MOVEMGR->RefreshHeroMove();

	SetMaxLife(m_CharacterInfo.MaxLife);
	SetMaxShield(m_CharacterInfo.MaxShield);
	SetMaxNaeRyuk(m_HeroTotalInfo.wMaxNaeRyuk);
	
	GAMEIN->GetCharacterDialog()->SetAttackRate();
	GAMEIN->GetCharacterDialog()->SetDefenseRate();
	GAMEIN->GetCharacterDialog()->UpdateData();
	GAMEIN->GetCharacterDialog()->UpdateMoveSpeedDisplay();
}
void CHero::RemoveStatus(CStatus* pStatus)
{
	CObject::RemoveStatus(pStatus);

	MOVEMGR->RefreshHeroMove();
/*

	((cGuage*)GAMEIN->GetGuageDlg()->GetWindowForID(CG_GUAGENAERYUK))->SetMaxValue(GetMaxNaeRyuk());
	GAMEIN->GetCharacterDialog()->SetNaeRyuk(m_HeroTotalInfo.wNaeRyuk);
	((cGuage*)GAMEIN->GetGuageDlg()->GetWindowForID(CG_GUAGELIFE))->SetMaxValue(GetMaxLife());
	GAMEIN->GetCharacterDialog()->SetLife(m_CharacterInfo.Life);
*/

	GAMEIN->GetCharacterDialog()->SetAttackRate();
	GAMEIN->GetCharacterDialog()->SetDefenseRate();
	GAMEIN->GetCharacterDialog()->UpdateData();
	GAMEIN->GetCharacterDialog()->UpdateMoveSpeedDisplay();
}

DWORD CHero::DoGetPhyAttackPowerMin()//¼ÆËãÎäÆ÷¹¥»÷½ü¾àÀë»òÔ¶¾àÀë
{
	const player_calc_stats * pChar_stats = GetCharStats();
	const player_calc_stats * pItem_stats = GetItemStats();
	const SET_ITEM_OPTION* pSetItem_Stats = GetSetItemStats(); //2007. 6. 13. CBH - ?®ïš¾å¬´æ?é½?æ£Ÿæ?çº?è¹ºé?

	WORD WeaponKind = GetWeaponEquipType();
	if(WeaponKind == WP_GUNG || WeaponKind == WP_AMGI)//Èç¹ûÎäÆ÷¹­¼ýºÍ°µÆ÷
	{
		WORD MinChub = (WORD)(pChar_stats->MinChub + GetAbilityStats()->StatMin + pItem_stats->MinChub + GetShopItemStats()->Minchub + GetSetItemQualityStats()->wMinChub + GetAvatarOption()->Minchub + pSetItem_Stats->wMinChub);


		return (DWORD)((CalcRangeAttackPower(MinChub,pItem_stats->RangeAttackPowerMin+GetAvatarOption()->Attack+pSetItem_Stats->wRangeAttackMin)
			+ GetAbilityStats()->GetPhyAttackUp(WeaponKind))
			* (1+GetItemStats()->RangeAttackPowerAddRateMin));
	}
	else
	{
		WORD GenGol = (WORD)(pChar_stats->GenGol + GetAbilityStats()->StatGen + pItem_stats->GenGol + GetShopItemStats()->Gengol + GetSetItemQualityStats()->wGenGol + GetAvatarOption()->Gengol + pSetItem_Stats->wGenGol);
		WORD WeaponItemIdx = GetWearedItemIdx(eWearedItem_Weapon);
		WORD DefaultPower = 0;
		if( WeaponItemIdx == 0 )
			DefaultPower = 5;
		

		return (DWORD)((CalcMeleeAttackPower(GenGol,pItem_stats->MeleeAttackPowerMin+DefaultPower+GetAvatarOption()->Attack+pSetItem_Stats->wMeleeAttackMin)
			+ GetAbilityStats()->GetPhyAttackUp(WeaponKind))
			* (1+GetItemStats()->MeleeAttackPowerAddRateMin));
	}
}
DWORD CHero::DoGetPhyAttackPowerMax()//¼ÆËãÎäÆ÷¹¥»÷½ü¾àÀë»òÔ¶¾àÀë
{
	const player_calc_stats * pChar_stats = GetCharStats();
	const player_calc_stats * pItem_stats = GetItemStats();
	const SET_ITEM_OPTION* pSetItem_Stats = GetSetItemStats(); //2007. 6. 13. CBH - ?®ïš¾å¬´æ?é½?æ£Ÿæ?çº?è¹ºé?

	WORD WeaponKind = GetWeaponEquipType();
	if(WeaponKind == WP_GUNG || WeaponKind == WP_AMGI)//Èç¹ûÎäÆ÷¹­¼ýºÍ°µÆ÷
	{
		WORD MinChub = (WORD)(pChar_stats->MinChub + GetAbilityStats()->StatMin + pItem_stats->MinChub + GetShopItemStats()->Minchub + GetSetItemQualityStats()->wMinChub + GetAvatarOption()->Minchub + pSetItem_Stats->wMinChub);

		return (DWORD)((CalcRangeAttackPower(MinChub,pItem_stats->RangeAttackPowerMax+GetAvatarOption()->Attack+pSetItem_Stats->wRangeAttackMax)
			+ GetAbilityStats()->GetPhyAttackUp(WeaponKind))
			* (1+GetItemStats()->RangeAttackPowerAddRateMax));
	}
	else
	{
		WORD GenGol = (WORD)(pChar_stats->GenGol + GetAbilityStats()->StatGen + pItem_stats->GenGol + GetShopItemStats()->Gengol + GetSetItemQualityStats()->wGenGol + GetAvatarOption()->Gengol + pSetItem_Stats->wGenGol);

		WORD WeaponItemIdx = GetWearedItemIdx(eWearedItem_Weapon);
		WORD DefaultPower = 0;
		if( WeaponItemIdx == 0 )
			DefaultPower = 5;
		
		return (DWORD)((CalcMeleeAttackPower(GenGol,pItem_stats->MeleeAttackPowerMax+DefaultPower+GetAvatarOption()->Attack+pSetItem_Stats->wMeleeAttackMax)
			+ GetAbilityStats()->GetPhyAttackUp(WeaponKind))
			* (1+GetItemStats()->MeleeAttackPowerAddRateMax));
	}
}

void CHero::HeroStateNotify(BYTE EndState)
{
	MSG_BYTE msg;
	msg.Category = MP_CHAR;
	msg.Protocol = MP_CHAR_STATE_NOTIFY;
	msg.dwObjectID = GetID();
	msg.bData = EndState;

	NETWORK->Send(&msg, sizeof(msg));
}

BYTE CHero::GetNaeRyukPercent()
{
	return (BYTE)((GetNaeRyuk()*100) / GetMaxNaeRyuk());
}

DWORD CHero::DoGetMaxNaeRyuk()
{
	return m_HeroTotalInfo.wMaxNaeRyuk;
}
DWORD CHero::DoGetPhyDefense()
{
	float baseDef =
		GetCharStats()->PhysicalDefense +
		GetItemStats()->PhysicalDefense +
		GetAbilityStats()->DefenceUp +
		GetSetItemStats()->wPhyDef +
		GetUniqueItemStats()->nDefen;

	// Æ·ÖÊÌ××°¸½¼Ó·ÀÓù = µÀ¾ß·ÀÓù * Æ·ÖÊ¼Ó³É°Ù·Ö±È
	float qualityDef =
		(GetItemStats()->PhysicalDefense) * (GetSetItemQualityStats()->wPhyDef * 0.01f);

	int totalDef = (int)(baseDef + qualityDef);

	return totalDef >= 0 ? (DWORD)totalDef : 0;
}
float CHero::DoGetAttDefense(WORD Attrib)
{
	float base =
		GetItemStats()->AttributeResist.GetElement_Val(Attrib) +
		GetAbilityStats()->AttRegistUp.GetElement_Val(Attrib) +
		GetSetItemStats()->AttrRegist.GetElement_Val(Attrib);

	float qualityBonus =
		GetItemStats()->AttributeResist.GetElement_Val(Attrib) *
		(GetSetItemQualityStats()->AttrRegistDef * 0.01f);

	return base + qualityBonus;
}

void CHero::ClearGuildInfo()
{
	CPlayer::ClearGuildInfo();
}

char* CHero::GetGuildEntryDate()
{
	return m_HeroTotalInfo.MunpaCanEntryDate;
}

void CHero::SetGuildEntryDate(char* day)
{
	SafeStrCpy(m_HeroTotalInfo.MunpaCanEntryDate, day, 11);
}

void CHero::CalcShopItemOption( WORD wIdx, BOOL bAdd, DWORD Param )
{
	if( wIdx == 0 )
		return;

	ITEM_INFO* pItemInfo = ITEMMGR->GetItemInfo( wIdx );
	if(!pItemInfo)
	{
	//	char buf[256]= { 0, };
	//	sprintf(buf, "S ItemInfo??æ©ˆæ£». PlayerIdx[%d] Idx[%d]", GetID(), wIdx);
	//	ASSERTMSG(0, buf);
		return;
	}

	int calc = -1;
	if(bAdd)	calc=1;

	//////////////////////////////////////////////////////
	// 06. 07 ?‚å¥¢ ?³é?(æ©¾å?) - æªœè???
	if( pItemInfo->NaeRyukRecover )
	{
		m_ShopItemOption.Decisive += (pItemInfo->NaeRyukRecover*calc);
		if(m_ShopItemOption.Decisive <= 0)
			m_ShopItemOption.Decisive=0;
	}
	//////////////////////////////////////////////////////

	if( pItemInfo->ItemKind == eSHOP_ITEM_PREMIUM )
	{
		// ?³ï?çº?  GenGol
		if( pItemInfo->GenGol>0 )
		{
			m_ShopItemOption.AddExp += (pItemInfo->GenGol*calc);
			if(m_ShopItemOption.AddExp <= 0)
				m_ShopItemOption.AddExp=0;
			
		}
		// å¬´æ????„å¥§?¸ç¦
		if( pItemInfo->MinChub>0 )
		{
			m_ShopItemOption.AddItemDrop += (pItemInfo->MinChub*calc);
			if(m_ShopItemOption.AddItemDrop <= 0)
				m_ShopItemOption.AddItemDrop=0;
		}
		// ?¿æ­·?Šé™½ ?³ï?çº‚è«¦ çµ?? å‰²??æ©ˆæ???	
	}
	else if( pItemInfo->ItemKind == eSHOP_ITEM_INCANTATION )
	{
		// è¤»ï??¸ç¦
		/*if( pItemInfo->LimitJob )
		{
			m_ShopItemOption.ItemMixSuccess = (char)pItemInfo->LimitJob;
			if(m_ShopItemOption.ItemMixSuccess < 0)
				m_ShopItemOption.ItemMixSuccess=0;
		}		
		else */
		if( pItemInfo->GenGol )
		{
			if( bAdd )
			{
				m_ShopItemOption.StatePoint += pItemInfo->GenGol;
				GAMEIN->GetCharacterDialog()->RefreshPointInfo();
			}
		}
		else if( pItemInfo->Life )
		{
			m_ShopItemOption.SkillPoint += pItemInfo->Life;
			GAMEIN->GetSkPointDlg()->RefreshAbilityPoint();
			GAMEIN->GetSuryunDialog()->RedistBtnActive();			
			GAMEIN->GetSkPointNotifyDlg()->SetActive( TRUE );
		}
		else if( pItemInfo->CheRyuk )
		{
			if( bAdd )
				m_ShopItemOption.ProtectCount = (char)pItemInfo->CheRyuk;
			else
				m_ShopItemOption.ProtectCount = (char)Param;
		}
		else if( ( pItemInfo->ItemIdx == eIncantation_Tracking ||
				   pItemInfo->ItemIdx == eIncantation_Tracking7 ||
				   pItemInfo->ItemIdx == eIncantation_Tracking7_NoTrade ) && bAdd )
		{
			//if( MAP->GetMapNum() == RUNNINGMAP || MAP->GetMapNum() == PKEVENTMAP )
			if( MAP->IsMapSame(eRunningMap) || MAP->GetMapNum() == PKEVENTMAP )
			{
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1457) );
				return;
			}

			GAMEIN->GetChaseinputDlg()->SetActive( TRUE );
			GAMEIN->GetChaseinputDlg()->SetItemIdx( pItemInfo->ItemIdx );
		}
		else if( pItemInfo->ItemIdx == eIncantation_ChangeName && bAdd )
		{
/*			GAMEIN->GetNameChangeNotifyDlg()->SetActive( TRUE );
			GAMEIN->GetInventoryDialog()->SetDisable( TRUE );*/
		}
		else if( pItemInfo->LimitJob )			// æº¯æ¼£?¯ï¾?…è–¯ è¼¿åƒ¥??
		{
			if( bAdd )
				m_ShopItemOption.EquipLevelFree += (BYTE)pItemInfo->LimitJob;
			else
			{
				m_ShopItemOption.EquipLevelFree -= (BYTE)pItemInfo->LimitJob;
			}
			STATSMGR->CalcItemStats( HERO );
			GAMEIN->GetCharacterDialog()->RefreshInfo();
			ITEMMGR->RefreshAllItem();
		}
		else if (pItemInfo->ItemIdx == eIncantation_MugongExtend && bAdd)
		{
#ifdef _SKILLSLOT_
			++m_CharacterInfo.ExtraMugongSlot;
#else
			GAMEIN->GetMugongDialog()->SetMugongExtend(TRUE);
#endif
			if (GAMEIN->GetMugongDialog()->IsActive())
				GAMEIN->GetMugongDialog()->SetActive(TRUE);
		}
		else if( pItemInfo->ItemIdx == eIncantation_MemoryMoveExtend || pItemInfo->ItemIdx == eIncantation_MemoryMoveExtend7 || pItemInfo->ItemIdx == eIncantation_MemoryMove2 ||
			     pItemInfo->ItemIdx == eIncantation_MemoryMoveExtend30 )
		{
			GAMEIN->GetMoveDialog()->SetExtend( bAdd );
		}
	}
	else if( pItemInfo->ItemKind == eSHOP_ITEM_CHARM ||
		pItemInfo->ItemKind == eSHOP_ITEM_FLGNAME )//ÉÁÃû25
	{
		if( pItemInfo->GenGol>0 )
		{
			m_ShopItemOption.Gengol += (pItemInfo->GenGol*calc);
			if(m_ShopItemOption.Gengol <= 0)
				m_ShopItemOption.Gengol=0;

			STATSMGR->CalcItemStats(HERO);
			GAMEIN->GetCharacterDialog()->SetGenGol();
			GAMEIN->GetCharacterDialog()->SetAttackRate();
		}
		if( pItemInfo->MinChub>0 )
		{
			m_ShopItemOption.Minchub += (pItemInfo->MinChub*calc);
			if(m_ShopItemOption.Minchub <= 0)
				m_ShopItemOption.Minchub=0;

			STATSMGR->CalcItemStats(HERO);
			GAMEIN->GetCharacterDialog()->SetMinChub();
			GAMEIN->GetCharacterDialog()->SetAttackRate();
			GAMEIN->GetCharacterDialog()->UpdateMoveSpeedDisplay();
		}
		if( pItemInfo->CheRyuk>0 )
		{		
			m_ShopItemOption.Cheryuk += (pItemInfo->CheRyuk*calc);
			if(m_ShopItemOption.Cheryuk <= 0)
				m_ShopItemOption.Cheryuk=0;

			STATSMGR->CalcItemStats(this);

			GAMEIN->GetCharacterDialog()->SetCheRyuk();
			GAMEIN->GetCharacterDialog()->SetDefenseRate();
		}
		if( pItemInfo->SimMek>0 )
		{
			m_ShopItemOption.Simmek += (pItemInfo->SimMek*calc);
			if(m_ShopItemOption.Simmek <= 0)
				m_ShopItemOption.Simmek=0;

			STATSMGR->CalcItemStats(HERO);
			GAMEIN->GetCharacterDialog()->SetSimMek();
		}
		// ?‚å¥¢ç­‰å???  Life
		if( pItemInfo->Life>0 )
		{
			m_ShopItemOption.NeagongDamage += (char)(pItemInfo->Life*calc);
			if(m_ShopItemOption.NeagongDamage <= 0)
				m_ShopItemOption.NeagongDamage=0;
		}
		// è«¼å¥¢æ¸ å???  Shield
		if( pItemInfo->Shield>0 )
		{
			m_ShopItemOption.WoigongDamage += (char)(pItemInfo->Shield*calc);
			if(m_ShopItemOption.WoigongDamage <= 0)
				m_ShopItemOption.WoigongDamage=0;
		}
		// é¼ å¥¢?¿æ™¦çº?1?? Plus_MugongIdx
		if( pItemInfo->NaeRyuk>0 )
		{
			m_ShopItemOption.AddSung += (pItemInfo->NaeRyuk*calc);
			if( m_ShopItemOption.AddSung < 0 )
				m_ShopItemOption.AddSung = 0;
		}
		// ?¦ç?ç­‰å??? Plus_Value
		if( pItemInfo->LimitJob>0 )
		{
			m_ShopItemOption.ComboDamage += (pItemInfo->LimitJob*calc);
			if(m_ShopItemOption.ComboDamage <= 0)
				m_ShopItemOption.ComboDamage=0;
		}
		// è§¼è‘¬?Œé½ ?¸é?æ¦?  CriticalPercent
		if( pItemInfo->LimitGender>0 )
		{
			m_ShopItemOption.Critical += (pItemInfo->LimitGender*calc);
			if(m_ShopItemOption.Critical <= 0)
				m_ShopItemOption.Critical=0;

			GAMEIN->GetCharacterDialog()->SetCritical();
		}
		// è§¼è‘¬?Œé½è¡??¶æ??¸ç¦    RangeAttackMin
		if( pItemInfo->LimitLevel>0 )
		{
			m_ShopItemOption.StunByCri += (pItemInfo->LimitLevel*calc);
			if(m_ShopItemOption.StunByCri <= 0)
				m_ShopItemOption.StunByCri=0;
		}
		// ?­è‘¬å¯žæ©«æº?
		if( pItemInfo->LimitGenGol>0 )
		{
			m_ShopItemOption.RegistPhys += (pItemInfo->LimitGenGol*calc);
			if(m_ShopItemOption.RegistPhys < 0)
				m_ShopItemOption.RegistPhys = 0;
			
			GAMEIN->GetCharacterDialog()->SetDefenseRate();
		}
		// ?©å?æ©«æ?
		if( pItemInfo->LimitMinChub>0 )
		{
			m_ShopItemOption.RegistAttr += (pItemInfo->LimitMinChub*calc);
			if(m_ShopItemOption.RegistAttr < 0)
				m_ShopItemOption.RegistAttr = 0;
			GAMEIN->GetCharacterDialog()->UpdateMoveSpeedDisplay();
			GAMEIN->GetCharacterDialog()->UpdateData();
		}
		// ?‚æ?æ¨¡è?é¦¬æ¨¡
		if( pItemInfo->LimitCheRyuk>0 )
		{
			m_ShopItemOption.NeaRyukSpend += (pItemInfo->LimitCheRyuk*calc);
			if(m_ShopItemOption.NeaRyukSpend < 0 )
				m_ShopItemOption.NeaRyukSpend = 0;
		}
		// è­†æ??…è²²æº??¸é?çº?
		if( pItemInfo->Plus_MugongIdx )
		{
			m_ShopItemOption.Life += (pItemInfo->Plus_MugongIdx*calc);
			if( m_ShopItemOption.Life <= 0 )
				m_ShopItemOption.Life = 0;	
		}
		// è­†æ??­è?é¬¼æ™¦ ?¸é?çº?
		if( pItemInfo->Plus_Value )
		{
			m_ShopItemOption.Shield += (pItemInfo->Plus_Value*calc);
			if( m_ShopItemOption.Shield <= 0 )
				m_ShopItemOption.Shield = 0;
		}
		// è­†æ??‚æ? ?¸é?çº?
		if( pItemInfo->AllPlus_Kind )
		{
			m_ShopItemOption.Naeryuk += (pItemInfo->AllPlus_Kind*calc);
			if( m_ShopItemOption.Naeryuk <= 0 )
				m_ShopItemOption.Naeryuk = 0;
		}
		// ?³å¥¢æº¯æ?
		if( pItemInfo->RangeAttackMin )
		{
			m_ShopItemOption.bKyungGong += (pItemInfo->RangeAttackMin*calc);
		}
		// ?³å¥¢ç´?é¼»è?
		if( pItemInfo->RangeAttackMax )
		{
			m_ShopItemOption.KyungGongSpeed += (pItemInfo->RangeAttackMax*calc);
		}
		// é¬¼ïŸ· ?¸ï? ?±ç?
		if( pItemInfo->CriticalPercent )
		{
			m_ShopItemOption.ReinforceAmp += (pItemInfo->CriticalPercent*calc);

			STATSMGR->CalcItemStats(this);
			GAMEIN->GetCharacterDialog()->RefreshInfo();
		}
		// å¬´æ??±è?å¥§ïŸ¸??
		if( pItemInfo->PhyDef )
		{
			m_ShopItemOption.AddItemDrop += (pItemInfo->PhyDef*calc);
		}
		// è§¼è‘¬?Œé½ ?¸é?æ¦?  CriticalPercent
		if( pItemInfo->NaeRyukRecover>0 )
		{
			m_ShopItemOption.Decisive += (pItemInfo->NaeRyukRecover*calc);
			if(m_ShopItemOption.Decisive <= 0)
				m_ShopItemOption.Decisive=0;

			GAMEIN->GetCharacterDialog()->SetCritical();
		}
		// ?»è?é¼?ç´±å????»è?é¼»ç´±?æ™¦æªœè? ?§å¬´æªœè ± è¤«æš®ç¸??…æ¸¡ å¬´æ???æª???¶è? ?¥æ?æ£?
		if( (DWORD)(pItemInfo->AttrRegist.GetElement_Val(ATTR_FIRE)) > 0 )
		{
			MSG_DWORD2 msg;
			msg.Category = MP_ITEMEXT;
			msg.Protocol = MP_ITEMEXT_SHOPITEM_DECORATION_ON;
			msg.dwObjectID = HEROID;
			msg.dwData2 = STREETSTALLMGR->GetStallKind();
			
			if(bAdd == FALSE)
			{
				msg.dwData1 = 0;
				m_ShopItemOption.dwStreetStallDecoration = 0;
				OBJECTMGR->SetRemoveDecorationInfo(HERO);
			}
			else
			{
				msg.dwData1 = pItemInfo->ItemIdx;
				m_ShopItemOption.dwStreetStallDecoration = pItemInfo->ItemIdx;


				if( GetState() == eObjectState_StreetStall_Owner)
                    STREETSTALLMGR->AddStreetStallDecoration(HERO, STREETSTALLMGR->GetStallKind());
			}
			
			NETWORK->Send(&msg,sizeof(msg));
		}
		
/*		// ?‚æ??‚ç???1.5å¯?
		if( pItemInfo->NaeRyukRecoverRate>0 )
		{
			m_ShopItemOption.RecoverRate += (pItemInfo->NaeRyukRecoverRate*calc);
			if( m_ShopItemOption.RecoverRate<=0 )
				m_ShopItemOption.RecoverRate = 0;

		}*/
	}
	else if( pItemInfo->ItemKind == eSHOP_ITEM_HERB )
	{
		// è­†æ??…è²²æº??¸é?çº?
		if( pItemInfo->Life > 0 )
		{
			m_ShopItemOption.Life += (WORD)(pItemInfo->Life*calc);
			if( m_ShopItemOption.Life <= 0 )
				m_ShopItemOption.Life = 0;		
		}
		// è­†æ??­è?é¬¼æ™¦ ?¸é?çº?
		if( pItemInfo->Shield > 0 )
		{
			m_ShopItemOption.Shield += (WORD)(pItemInfo->Shield*calc);
			if( m_ShopItemOption.Shield <= 0 )
				m_ShopItemOption.Shield = 0;
		}
		// è­†æ??‚æ? ?¸é?çº?
		if( pItemInfo->NaeRyuk > 0 )
		{
			m_ShopItemOption.Naeryuk += (pItemInfo->NaeRyuk*calc);
			if( m_ShopItemOption.Naeryuk <= 0 )
				m_ShopItemOption.Naeryuk = 0;
		}

		// ?¶å? ?³è¾¨
		// STATSMGR->CalcCharStats( pPlayer );
	}
	else if( pItemInfo->ItemKind == eSHOP_ITEM_MAKEUP || pItemInfo->ItemKind == eSHOP_ITEM_DECORATION )
	{
//		APPEARANCEMGR->SetAvatarItem( HERO, pItemInfo->ItemIdx, bAdd );
	}
	else if( pItemInfo->ItemKind == eSHOP_ITEM_SUNDRIES )
	{
		/*
		if( pItemInfo->ItemIdx == eSundries_Shout && bAdd )
			GAMEIN->GetShoutDlg()->SetActive( TRUE );
		*/


		// ?»è??ƒæ’²
		if( pItemInfo->CheRyuk )
			m_ShopItemOption.bStreetStall += (pItemInfo->CheRyuk*calc);
	}	
}


void CHero::ActiveOptionInfoToInterface()
{
	if( m_ShopItemOption.Gengol )
		GAMEIN->GetCharacterDialog()->SetGenGol();
	if( m_ShopItemOption.Minchub )
		GAMEIN->GetCharacterDialog()->SetMinChub();
	if( m_ShopItemOption.Cheryuk )
		GAMEIN->GetCharacterDialog()->SetCheRyuk();
	if( m_ShopItemOption.Simmek )
		GAMEIN->GetCharacterDialog()->SetSimMek();
}


void CHero::CheckShopItemUseInfoToInventory( WORD ItemIdx, WORD ItemPos )
{
	CItem*	pItem = NULL;
	
	AVATARITEM* pAvatarItem = GAMERESRCMNGR->m_AvatarEquipTable.GetData( ItemIdx );
	if(!pAvatarItem)	return;

	for(int i=0; i<eAvatar_Max; i++)
	{
		if( (!pAvatarItem->Item[i] && m_ShopItemOption.Avatar[i]) ||
			(m_ShopItemOption.Avatar[i] && i==pAvatarItem->Position) )
		{
			for(int n=0; n<(SLOT_SHOPINVEN_NUM+TABCELL_SHOPINVEN_PLUS_NUM)/*/2*/; n++)
			{
				pItem = GAMEIN->GetInventoryDialog()->GetItemForPos( n+TP_SHOPINVEN_START );
				if(!pItem)		continue;

				if( pItem->GetItemIdx()==m_ShopItemOption.Avatar[i] && pItem->GetUseParam() )
				{
					pItem->SetUseParam( 0 );
					break;
				}				
			}
		}
	}
}
void CHero::UseShopItem( WORD ItemIdx, WORD ItemPos )
{
	CItem*	pItem = NULL;
	
	AVATARITEM* pAvatarItem = GAMERESRCMNGR->m_AvatarEquipTable.GetData( ItemIdx );
	if(!pAvatarItem)	return;

	for(int i=0; i<eAvatar_Max; i++)
	{
		if( (!pAvatarItem->Item[i] && m_ShopItemOption.Avatar[i]) ||
			(m_ShopItemOption.Avatar[i] && i==pAvatarItem->Position) )
		{		
			pItem = GAMEIN->GetInventoryDialog()->GetItemForPos( ItemPos );
			if(!pItem)		continue;

			if( pItem->GetItemIdx()==m_ShopItemOption.Avatar[i] && !pItem->GetUseParam() )
			{
				pItem->SetUseParam( 1 );
				break;
			}
		}
	}
}

void CHero::RefreshLevelupPoint()
{
	if( m_ShopItemOption.StatePoint==0 )		return;

	// Hard Coding
	WORD point = m_HeroTotalInfo.LevelUpPoint + (30 - m_ShopItemOption.StatePoint);	
	GAMEIN->GetCharacterDialog()->RefreshPointInfo();//( TRUE, point );
}

void CHero::SetMunpaName(char* Name)
{
	strcpy(m_CharacterInfo.GuildName, Name);
}

void CHero::SetGuildName(char* Name)	
{ 
	SafeStrCpy(m_CharacterInfo.GuildName, Name, MAX_GUILD_NAME+1);	
}

char* CHero::GetGuildName()			
{ 
	return m_CharacterInfo.GuildName;
}

BOOL CHero::CanSocietyAction( DWORD curTime )
{
	if( GetState() != eObjectState_None && GetState() != eObjectState_Society &&
		GetState() != eObjectState_Immortal )
		return FALSE;

	DWORD dwElapsedTime = gCurTime - m_dwLastSocietyAction;
	if( dwElapsedTime > 2000 ) //2??
	{
		m_dwLastSocietyAction = gCurTime;
		return TRUE;
	}

	return FALSE;
}

void CHero::SetStage( BYTE Stage )
{
	CPlayer::SetStage( Stage );

	GAMEIN->GetCharacterDialog()->SetStage( Stage );

	ABILITYMGR->UpdateAbilityState( ABILITYUPDATE_CHARACTERSTAGE_CHANGED, 0, GetAbilityGroup() );
}
DWORD CHero::DoGetAttAttackPowerMax(WORD Attrib)// ÊôÐÔ¹¥»÷Ìí¼ÓÁ¦×î´óÖµ 
{
	DWORD MaxV = 0;
	WORD SimMek = GetSimMek();
	DWORD MaxLVV = (GetLevel() + 5) + 5;
	DWORD AttribPlus = GetAttribPlusPercent(Attrib);

	MaxV = MaxLVV + AttribPlus * 100 + SimMek / 2;
	return MaxV;
}


DWORD CHero::DoGetAttAttackPowerMin(WORD Attrib)// ÊôÐÔ¹¥»÷Ìí¼ÓÁ¦×îÐ¡Öµ
{
	DWORD MinV = 0;
	WORD SimMek = GetSimMek();
	DWORD MinLVV = (GetLevel() + 5) - 5;
	DWORD AttribPlus = GetAttribPlusPercent(Attrib);

	MinV = MinLVV + AttribPlus * 100 + SimMek / 2;
	return MinV;
}

DWORD CHero::GetAttribPlusPercent(WORD Attrib)
{
	return GetItemStats()->AttributeAttack.GetElement_Val(Attrib) +//ÊôÐÔ¹¥»÷
		GetAbilityStats()->AttAttackUp.GetElement_Val(Attrib) + (GetSimMek() * 0.001);
}

BOOL CHero::IsSkipSkill()
{
	if(	m_CharacterInfo.Stage != eStage_Hyun &&
		m_CharacterInfo.Stage != eStage_Tal )
		return FALSE;

	WORD SkipKind = 0;

	PTRLISTSEARCHSTART(m_StatusList,CStatus*,pSL)
		pSL->GetSkipEffectKind(SkipKind);
	PTRLISTSEARCHEND;

	if(SkipKind == 0)
		return FALSE;
	else
		return (BOOL)SkipKind;
}
////////////////////////////////////////////////////////

//SW070127 é¡«æ?é©?
BOOL CHero::CanUseTitanSkill()
{
	BOOL canUse = FALSE;
	//?¦ï?æ»²å”³ //æ¿°ç?è«?é¼ å©¦
	//canUse = ( HERO->InTitan() && TITANMGR->CheckUsingEquipItemNum() );
	canUse = HERO->InTitan();

	return canUse;
}

////// 2007. 6. 14. CBH - ?®ïš¾å¬´æ?é½?æ£Ÿæ?çº??—éª¯ ?ç†± è¹ºé?
void CHero::AddSetitemOption(SET_ITEM_OPTION* pSetItemOption)
{
	float attrvalue = 0;

	/// ?¬åŸ¤
	m_setItemStats.wGenGol += pSetItemOption->wGenGol;	
	//?˜ç¹ª
	m_setItemStats.wMinChub += pSetItemOption->wMinChub;
	//ç¾¹æ?
	m_setItemStats.wCheRyuk += pSetItemOption->wCheRyuk;
	//è¤•è?
	m_setItemStats.wSimMek += pSetItemOption->wSimMek;
	//è­†æ??…è²²æº?
	m_setItemStats.dwLife += pSetItemOption->dwLife;
	//?­è?é¬¼æ™¦
	m_setItemStats.dwShield += pSetItemOption->dwShield;
	//è­†æ??‚æ?
	m_setItemStats.dwNaeRyuk += pSetItemOption->dwNaeRyuk;
	//???ªï?æº?
	m_setItemStats.AttrRegist.AddATTRIBUTE_VAL(pSetItemOption->AttrRegist,1);	
	//?¬å‰ª??è­†æ¨¡ å¥¢å?æº?
	m_setItemStats.wMeleeAttackMin += pSetItemOption->wMeleeAttackMin;
	//?¬å‰ª??è­†æ? å¥¢å?æº?
	m_setItemStats.wMeleeAttackMax += pSetItemOption->wMeleeAttackMax;
	//?³å‰ª??è­†æ¨¡ å¥¢å?æº?
	m_setItemStats.wRangeAttackMin += pSetItemOption->wRangeAttackMin;
	//?³å‰ª??è­†æ? å¥¢å?æº?
	m_setItemStats.wRangeAttackMax += pSetItemOption->wRangeAttackMax;
	//è§¼è‘¬?Œé½
	m_setItemStats.wCriticalPercent += pSetItemOption->wCriticalPercent;	
	//??å¥¢å?æº?
	m_setItemStats.AttrAttack.AddATTRIBUTE_VAL(pSetItemOption->AttrAttack,1);		
	//?­è‘¬å¯žæ©«æº?
	m_setItemStats.wPhyDef += pSetItemOption->wPhyDef;
	//?…è²²æº??‚ç? (?…è?)	
	m_setItemStats.wLifeRecover += pSetItemOption->wLifeRecover;
	//?…è²²æº??‚ç?
	m_setItemStats.fLifeRecoverRate += pSetItemOption->fLifeRecoverRate;
	//?‚æ? ?‚ç? (?…è?)
	m_setItemStats.wNaeRyukRecover += pSetItemOption->wNaeRyukRecover;
	//?‚æ? ?‚ç?
	m_setItemStats.fNaeRyukRecoverRate += pSetItemOption->fNaeRyukRecoverRate;
}

void CHero::ClearSetitemOption()
{
	memset(&m_setItemStats, 0, sizeof(SET_ITEM_OPTION));
}
///////////////////////////////////////////////////////////////////////////////////
void CHero::AddSetitemQualityOption(SET_ITEMQUALITY_OPTION* pSetItemQualityOption)
{
	//×î´ó×£¸£±¶ÂÊ
	m_setItemQualityStats.RareVal += pSetItemQualityOption->RareVal;
	//Á¦Á¿
	m_setItemQualityStats.wGenGol += pSetItemQualityOption->wGenGol;
	if (m_setItemQualityStats.wGenGol > 40)
	{
		m_setItemQualityStats.wGenGol = 40;
	}
	//Ãô½Ý
	m_setItemQualityStats.wMinChub += pSetItemQualityOption->wMinChub;
	if (m_setItemQualityStats.wMinChub > 40)
	{
		m_setItemQualityStats.wMinChub = 40;
	}
	//ÌåÖÊ
	m_setItemQualityStats.wCheRyuk += pSetItemQualityOption->wCheRyuk;
	if (m_setItemQualityStats.wCheRyuk > 60)
	{
		m_setItemQualityStats.wCheRyuk = 60;
	}
	//ÐÄÂö
	m_setItemQualityStats.wSimMek += pSetItemQualityOption->wSimMek;
	if (m_setItemQualityStats.wSimMek > 40)
	{
		m_setItemQualityStats.wSimMek = 40;
	}
	//ÉúÃü
	m_setItemQualityStats.dwLife += pSetItemQualityOption->dwLife;
	if (m_setItemQualityStats.dwLife > 60)
	{
		m_setItemQualityStats.dwLife = 60;
	}
	//»¤Ìå
	m_setItemQualityStats.dwShield += pSetItemQualityOption->dwShield;
	if (m_setItemQualityStats.dwShield > 60)
	{
		m_setItemQualityStats.dwShield = 60;
	}
	//ÄÚÁ¦
	m_setItemQualityStats.dwNaeRyuk += pSetItemQualityOption->dwNaeRyuk;
	if (m_setItemQualityStats.dwNaeRyuk > 60)
	{
		m_setItemQualityStats.dwNaeRyuk = 60;
	}
	//ÊôÐÔ·ÀÓù
	m_setItemQualityStats.AttrRegistDef += pSetItemQualityOption->AttrRegistDef;
	if (m_setItemQualityStats.AttrRegistDef > 30)
	{
		m_setItemQualityStats.AttrRegistDef = 30;
	}
	//ÎïÀí·ÀÓù
	m_setItemQualityStats.wPhyDef += pSetItemQualityOption->wPhyDef;
	if (m_setItemQualityStats.wPhyDef > 30)
	{
		m_setItemQualityStats.wPhyDef = 30;
	}
	//ÄÚ¹¦ÉËº¦
	m_setItemQualityStats.NaegongDamage += pSetItemQualityOption->NaegongDamage;
	if (m_setItemQualityStats.NaegongDamage > 20)
	{
		m_setItemQualityStats.NaegongDamage = 20;
	}
	//Íâ¹¦ÉËº¦
	m_setItemQualityStats.WoigongDamage += pSetItemQualityOption->WoigongDamage;
	if (m_setItemQualityStats.WoigongDamage > 20)
	{
		m_setItemQualityStats.WoigongDamage = 20;
	}
	//ÉÁ±Ü
	m_setItemQualityStats.wDodgeRate += pSetItemQualityOption->wDodgeRate;
	if (m_setItemQualityStats.wDodgeRate > 20)
	{
		m_setItemQualityStats.wDodgeRate = 20;
	}
	//PVPÆÆ¼×%
	m_setItemQualityStats.PlayerPhyDefDown += pSetItemQualityOption->PlayerPhyDefDown;
	if (m_setItemQualityStats.PlayerPhyDefDown > 20)
	{
		m_setItemQualityStats.PlayerPhyDefDown = 20;
	}
	// PVPÆÆÄ§%
	m_setItemQualityStats.PlayerAttrDefDown += pSetItemQualityOption->PlayerAttrDefDown;
	if (m_setItemQualityStats.PlayerAttrDefDown > 20)
	{
		m_setItemQualityStats.PlayerAttrDefDown = 20;
	}
	// PVEÆÆ¼×%
	m_setItemQualityStats.TargetPhyDefDown += pSetItemQualityOption->TargetPhyDefDown;
	if (m_setItemQualityStats.TargetPhyDefDown > 30)
	{
		m_setItemQualityStats.TargetPhyDefDown = 30;
	}
	// PVEÆÆÄ§%
	m_setItemQualityStats.TargetAttrDefDown += pSetItemQualityOption->TargetAttrDefDown;
	if (m_setItemQualityStats.TargetAttrDefDown > 30)
	{
		m_setItemQualityStats.TargetAttrDefDown = 30;
	}
	// ÃüÖÐ
	m_setItemQualityStats.fDodgeRate += pSetItemQualityOption->fDodgeRate;
	if (m_setItemQualityStats.fDodgeRate > 20)
	{
		m_setItemQualityStats.fDodgeRate = 20;
	}
	// ÅÝµã¼Ó³É
	m_setItemQualityStats.MallMoneyPuls += pSetItemQualityOption->MallMoneyPuls;
	if (m_setItemQualityStats.MallMoneyPuls > 20)
	{
		m_setItemQualityStats.MallMoneyPuls = 20;
	}
	// Çá¹¦ËÙ¶È
	m_setItemQualityStats.KyunggongSpeed += pSetItemQualityOption->KyunggongSpeed;
	if (m_setItemQualityStats.KyunggongSpeed > 200)
	{
		m_setItemQualityStats.KyunggongSpeed = 200;
	}
	// PVEµÄÉËº¦Ôö¼Ó
	m_setItemQualityStats.AttMonsterDamage += pSetItemQualityOption->AttMonsterDamage;
	if (m_setItemQualityStats.AttMonsterDamage > 20)
	{
		m_setItemQualityStats.AttMonsterDamage = 20;
	}
	// PVPµÄÉËº¦Ôö¼Ó
	m_setItemQualityStats.AttPlayerDamage += pSetItemQualityOption->AttPlayerDamage;
	if (m_setItemQualityStats.AttPlayerDamage > 10)
	{
		m_setItemQualityStats.AttPlayerDamage = 10;
	}
	// ÊÜµ½ÉËº¦¼õÉÙ
	m_setItemQualityStats.RealDamageDown += pSetItemQualityOption->RealDamageDown;
	if (m_setItemQualityStats.RealDamageDown > 20)
	{
		m_setItemQualityStats.RealDamageDown = 20;
	}
	// PVP¼¸ÂÊÎüÑª
	m_setItemQualityStats.PVPLifePlus += pSetItemQualityOption->PVPLifePlus;
	if (m_setItemQualityStats.PVPLifePlus > 20)
	{
		m_setItemQualityStats.PVPLifePlus = 20;
	}
	//¼¸ÂÊÂúÑª¸´»î
	m_setItemQualityStats.Resurrected += pSetItemQualityOption->Resurrected;
	if (m_setItemQualityStats.Resurrected > 30)
	{
		m_setItemQualityStats.Resurrected = 30;
	}
	//Íâ¹¦±©»÷Ôö¼Ó
	m_setItemQualityStats.Critical += pSetItemQualityOption->Critical;
	if (m_setItemQualityStats.Critical > 30)
	{
		m_setItemQualityStats.Critical = 30;
	}
	//ÄÚ¹¦±©»÷Ôö¼Ó
	m_setItemQualityStats.Decisive += pSetItemQualityOption->Decisive;
	if (m_setItemQualityStats.Decisive > 30)
	{
		m_setItemQualityStats.Decisive = 30;
	}
	//Íâ¹¦¼¼ÄÜ±©»÷ÉËº¦Ôö¼Ó
	m_setItemQualityStats.CriticalDamage += pSetItemQualityOption->CriticalDamage;
	if (m_setItemQualityStats.CriticalDamage > 20)
	{
		m_setItemQualityStats.CriticalDamage = 20;
	}
	//ÄÚ¹¦¼¼ÄÜ±©»÷ÉËº¦Ôö¼Ó
	m_setItemQualityStats.DecisiveDamage += pSetItemQualityOption->DecisiveDamage;
	if (m_setItemQualityStats.DecisiveDamage > 20)
	{
		m_setItemQualityStats.DecisiveDamage = 20;
	}
	//³ÖÐøÉËº¦Ôö¼Ó
	m_setItemQualityStats.ContinueAttAttack += pSetItemQualityOption->ContinueAttAttack;
	if (m_setItemQualityStats.ContinueAttAttack > 20)
	{
		m_setItemQualityStats.ContinueAttAttack = 20;
	}
}

void CHero::ClearSetitemQualityOption()
{
	memset(&m_setItemQualityStats, 0, sizeof(SET_ITEMQUALITY_OPTION));
}
///////////////////////////////////////////////////////////////////////////////////
// magi82 - UniqueItem(070626)
void CHero::AddUniqueItemOption(UNIQUE_ITEM_OPTION_INFO* pUniqueOption)
{
	m_UniqueItemStats.nHp += pUniqueOption->nHp;
	m_UniqueItemStats.nMp += pUniqueOption->nMp;
	m_UniqueItemStats.nShield += pUniqueOption->nShield;
	m_UniqueItemStats.nPhyDamage += pUniqueOption->nPhyDamage;
	m_UniqueItemStats.nCriDamage += pUniqueOption->nCriDamage;
	m_UniqueItemStats.nCriRate += pUniqueOption->nCriRate;
	m_UniqueItemStats.nGengol += pUniqueOption->nGengol;
	m_UniqueItemStats.nMinChub += pUniqueOption->nMinChub;
	m_UniqueItemStats.nCheRyuk += pUniqueOption->nCheRyuk;
	m_UniqueItemStats.nSimMek += pUniqueOption->nSimMek;
	m_UniqueItemStats.nDefen += pUniqueOption->nDefen;
	m_UniqueItemStats.nRange += pUniqueOption->nRange;
	m_UniqueItemStats.nAttR += pUniqueOption->nAttR;
	m_UniqueItemStats.nEnemyDefen += pUniqueOption->nEnemyDefen;
}
//////////////////////////////////////////////////////////////////////////


void CHero::ClearWayPoint(BYTE bState)
{
	if(bState==0)
	{
		m_bNeedMove = FALSE;
		m_WayPointManager->ClearAllWayPoint();
		//CBigMapDlg * pBigMapDlg = GAMEIN->GetBigMapDialog();
		//CMiniMapDlg* pMiniMapDlg = GAMEIN->GetMiniMapDialog();
		//if(pBigMapDlg) pBigMapDlg->ClearMovePoint();
		//if(pMiniMapDlg) pMiniMapDlg->ClearMovePoint();
	}

	GAMEIN->GetMiniMapDialog()->ClearMovePoint();
	GAMEIN->GetBigMapDialog()->ClearMovePoint();
}
BOOL CHero::Move_UsePath(VECTOR3* pDestPos,BOOL bSimpleMode,BOOL bMousePointDisplay,BOOL bIsCollisionPosMove)
{
	//if( InterlockedCompareExchange(&g_PathThreadActivated, 0, 0) == 1 )
	//{
	//	return FALSE;
	//}
	/*if (MAP->CollisionCheck_OneTile(pDestPos) == FALSE)
	{
		return FALSE;
	}*/

	ClearWayPoint();
	/*VECTOR3 t_StartPos;
	GetPosition( &t_StartPos );

	BOOL t_CollisionCheck_Oneline = MAP->CollisionCheck_OneLine_New(&GetPosition(), pDestPos);
	if (t_CollisionCheck_Oneline == TRUE)
	{
		Move_Simple(pDestPos);
	}
	else
	{
		//BOOL t_PathFindResult = MAP->PathFind(&t_StartPos, pDestPos, m_WayPointManager,TRUE);
		//if (t_PathFindResult = FALSE)
		//{
		//	return FALSE;
		//}
	}*/

	//if(FALSE == MAP->PathFind(&t_StartPos,pDestPos,m_WayPointManager,TRUE))
	//{
	//	return FALSE;
	//}

	m_WayPointManager->SetFlagDestination( *pDestPos );

	return TRUE;
}
BOOL CHero::Move_Simple(VECTOR3* pTargetPos,BOOL bRenderTileDebug)
{
	if (this->GetState() == eObjectState_Engrave)
		return FALSE;
	if (this->GetState() == eObjectState_Deal)
		return FALSE;
	if (this->IsInSpecialState(eSpecialState_Stun))
		return FALSE;
	if (this->GetState() != eObjectState_None &&
		this->GetState() != eObjectState_Move &&
		this->GetState() != eObjectState_TiedUp_CanMove &&
		this->GetState() != eObjectState_Immortal)
	{
		if (this->GetState() == eObjectState_Society)
		{			
			return FALSE;			
		}
		else
		{
			return FALSE;
		}
		return FALSE;
	}
	//if (bRenderTileDebug)
	//	MAP->SetRenderTileData(MAP->CollisionCheck_OneTile(pTargetPos));

	m_MoveInfo.InitTargetPosition();
	m_MoveInfo.SetTargetPosition( 0, *pTargetPos );
	m_MoveInfo.SetMaxTargetPosIdx( 1 );

	MOVE_ONETARGETPOS msg;
	msg.Category = MP_MOVE;
	msg.Protocol = MP_MOVE_ONETARGET;
	msg.dwObjectID = HEROID;
	msg.dwMoverID = HEROID;
	msg.SetStartPos( &m_MoveInfo.CurPosition );
	msg.SetTargetPos( m_MoveInfo.GetTargetPosition(0) );

	NETWORK->Send(&msg,msg.GetSize());

	MOVEMGR->StartMoveEx(this, pTargetPos, gCurTime);
#ifdef  _MUTIPET_
	for (int i = 0; i < 3; ++i)//µ¶¸ç  3³èÎï
	{
		if (PETMGR->GetCurSummonPet(i))
		{
			MOVEMGR->SetPetTarget(PETMGR->GetCurSummonPet(i), &HERO->m_MoveInfo, 1);
		}
	}
#else
	MOVEMGR->SetPetTarget(&m_MoveInfo,1);
#endif //  _MUTIPET_
	return TRUE;
}
BOOL CHero::NextMove()
{
	if ( m_WayPointManager->PopWayPoint(&m_WayPoint) == TRUE )
	{
		if (m_WayPoint.x == 0 || m_WayPoint.z == 0)
			return TRUE;
		if( !Move_Simple( &m_WayPoint ) )
		{
			ClearWayPoint();
			m_bNeedMove = FALSE;
			MOVEMGR->SetFindPath();
			return m_bNeedMove;
		}
		m_bNeedMove = TRUE;
		return m_bNeedMove;
	}
	ClearWayPoint();
	m_bNeedMove = FALSE;
	return m_bNeedMove;
}

void CHero::SetImageName(WORD ImageName)
{
	m_HeroTotalInfo.ImageName = ImageName;
	m_CharacterInfo.ImageName = ImageName;
	SetImageNameBalloon(ImageName);
}
#include "MHTimeManager.h"
void CHero::Move_KeyboardInput()
{
	if(KeyboardMode)
	{
		int nKeyMoveDir = MACROMGR->GetKeyMoveDir();
		if( MACROMGR->IsKeyChanged() )
		{
			MACROMGR->ClearKeyChanged();

			if( nKeyMoveDir != KEY_MOVE_STOP )
			{
				Move(nKeyMoveDir);
			}
			else
			{
				MOVEMGR->HeroMoveStop();
				Stop_SendtoServer();
				AUTOPATH->SetTargetPos(GetCurPosition(),FALSE);
			}
		}
		else if( MHTIMEMGR->GetInstance()->CheckTimer100() )
		{
			if( nKeyMoveDir != KEY_MOVE_STOP )
			{
				Move(nKeyMoveDir);
			}
		}
	}	
}
void CHero::Stop_SendtoServer()
{
	if (GetState() == eObjectState_Die) return;

	MOVE_POS msg;
	msg.Category = MP_MOVE;
	msg.Protocol = MP_MOVE_STOP;
	msg.dwObjectID = HEROID;
	msg.dwMoverID = HEROID;
	msg.cpos.Compress(&GetPosition());

	NETWORK->Send(&msg, sizeof(MOVE_POS));
}
#include "GlobalEventFunc.h"
#include "PKManager.h"
#include "CharSelect.h"
#include "MainGame.h"
#define AxisXSense 12//21
#define CURSOR_SPEED 20
extern HWND _g_hWnd;


#define SPEEDCAMERA 0.02f
extern BOOL g_bActiveApp;
void CHero::ResetCursorPosition()
{
	POINT point = { LONG(WINDOWMGR->GetScreenX() / 2), LONG(WINDOWMGR->GetScreenY() / 2) };
	ClientToScreen(_g_hWnd, &point);
	SetCursorPos(point.x, point.y);
}
void CHero::KeyboardModeCameraProcess()
{
	if (!g_bActiveApp)return;
	if (!KeyboardMode)return;
	

	//CAMERA->MouseRotate(0, 0);
	//return;

	POINT MouseAxis = { LONG(MOUSE->GetMouseAxisX()), LONG(MOUSE->GetMouseAxisY()) };


	static short SmoothY = MouseAxis.y;
	static short SmoothX = MouseAxis.x;

	if (SmoothX != 0 || SmoothY != 0)
	{
		CAMERA->MouseRotate(SmoothX, SmoothY);
		SmoothX = 0;
		SmoothY = 0;
		ResetCursorPosition();
	}
	


	
	return;

	

	char str[64] ={0,};
	sprintf(str, "%d %d", MouseAxis.x, MouseAxis.y);
	SetWindowText(_g_hWnd, str);

	short xx = MOUSE->GetMouseAxisX();
	short yy = MOUSE->GetMouseAxisY();
	if (xx != 0 || yy != 0)
	{
		CAMERA->MouseRotate(xx, yy);
	}

	//return;

	ResetCursorPosition();
	
}

static BOOL bswitch = FALSE;
static BOOL bswitchpad = FALSE;
void CHero::GamePadCameraProcess()
{	
	//for camera 
	//right thumb
	SHORT thumbX = Player[0]->GetState().Gamepad.sThumbRX;
	SHORT thumbY = Player[0]->GetState().Gamepad.sThumbRY;
	short y = thumbY / 32767;
	short SmoothX = -thumbX / 800;

	//if (SmoothX > AxisXSense)
	//	SmoothX = AxisXSense;
	//if (SmoothX < -AxisXSense)
	//	SmoothX = -AxisXSense;	

	static BOOL camswitch=FALSE;
	if (SmoothX > AxisXSense)
	{
		camswitch = TRUE;
	}		
	if (SmoothX < -AxisXSense)
	{
		camswitch = TRUE;
	}

	//if (camswitch)
		CAMERA->MouseRotate(0.02 * gTickTime*(float)SmoothX, 0);	

	if (y <= 0)
		MACROMGR->PlayMacroPress(MPE_CAMERAMOVE_DOWN);
	if (y >= 0)
		MACROMGR->PlayMacroPress(MPE_CAMERAMOVE_UP);	
}
void CHero::GamePadMoveProcess()
{	
	//for character movements
	//left thumb
	static BYTE bOnce;

	SHORT thumbXX = Player[0]->GetState().Gamepad.sThumbLX;
	SHORT thumbYY = Player[0]->GetState().Gamepad.sThumbLY;
	short xx = Player[0]->GetState().Gamepad.sThumbLX / 32767;
	short yy = Player[0]->GetState().Gamepad.sThumbLY / 32767;

	short SmoothXX = -thumbXX / 800;
	short SmoothYY = -thumbYY / 800;

	if (SmoothXX > AxisXSense)
		SmoothXX = AxisXSense;
	if (SmoothXX < -AxisXSense)
		SmoothXX = -AxisXSense;

	if (SmoothYY > AxisXSense)
		SmoothYY = AxisXSense;
	if (SmoothYY < -AxisXSense)
		SmoothYY = -AxisXSense;

	//CHATMGR->AddMsg(CTC_SYSMSG, "x[%d] y[%d]     X:%d Y:%d    X:%d Y:%d", xx, yy, thumbXX, thumbYY, SmoothXX, SmoothYY);

	int i_nKeyMoveDir = KEY_MOVE_STOP;
	if (xx || yy)
	{
		MACROMGR->OpenKeyChanged();

		HERO->DisableAutoAttack();
		HERO->SetCurComboNum(0);
		HERO->SetMovingAction(NULL);

		bOnce = 1;
	}
	else
	{
		if (bOnce == 1)
		{
			bOnce = 2;
			MACROMGR->OpenKeyChanged();
		}
	}
	if (xx == -1)
	{
		i_nKeyMoveDir |= KEY_MOVE_LEFT;
	}
	if (xx == 1)
	{
		i_nKeyMoveDir |= KEY_MOVE_RIGHT;
	}
	if (yy == 1)
	{
		i_nKeyMoveDir |= KEY_MOVE_UP;
	}
	if (yy == -1)
	{
		i_nKeyMoveDir |= KEY_MOVE_DOWN;
	}
	MACROMGR->SetKeyMoveDir(i_nKeyMoveDir);

	//------------------------------------------------------------------------------------
	int nKeyMoveDir2 = i_nKeyMoveDir;
	if (MACROMGR->IsKeyChanged())
	{
		MACROMGR->ClearKeyChanged();

		if (nKeyMoveDir2 != KEY_MOVE_STOP)
		{
			Move(nKeyMoveDir2);
		}
		else
		{
			MOVEMGR->HeroMoveStop();
			//Stop_SendtoServer();
		}
	}
	else if (MHTIMEMGR->GetInstance()->CheckTimer100())
	{
		if (nKeyMoveDir2 != KEY_MOVE_STOP)
		{
			Move(nKeyMoveDir2);
		}
	}
}
void CHero::GamePadHapticProcess()
{	
	//return;
	if (vibLeft != 0 || vibright != 0)
	{
		if (vbAllowHaptic)
		{
			vbAllowHaptic = false;

			Player[0]->Vibrate(vibLeft, vibright);
		}
		else
		{
			vibLeft = vibright = 0;
			//Player[0]->Vibrate();
			Player[0]->Vibrate(vibLeft, vibright);
		}
		
	}
	//
}

void CHero::GamePadDInputProcess(BOOL bswitch)
{
	WORD isButtonPressed = Player[0]->GetState().Gamepad.wButtons;
	CChannelDialog* m_ChannelDlg = (CChannelDialog*)WINDOWMGR->GetWindowForID(CHA_CHANNELDLG);
	//D-input
	//column = 0;
	if(isButtonPressed & XINPUT_GAMEPAD_DPAD_UP)
	{
		if (bswitch == 1)
		{
			MACROMGR->PlayMacroPress(MPE_CAMERAMOVE_ZOOMIN);
			//return;
		}
		else
		{
			if (m_ChannelDlg->IsActive())
			{
				m_ChannelDlg->SelectChannelGamePadDec();
			}
			else
			{
				if (column>0)
				column = column - 1;
			}
		}
		//return;
	}
	if (isButtonPressed & XINPUT_GAMEPAD_DPAD_DOWN)
	{
		if (bswitch == 1)
		{
			MACROMGR->PlayMacroPress(MPE_CAMERAMOVE_ZOOMOUT);
			//return;
		}
		else
		{
			if (m_ChannelDlg->IsActive())
			{
				m_ChannelDlg->SelectChannelGamePadInc();
			}
			else
			{
				if (column<7)
					column = column + 1;
			}
		}
		//return;
	}
	if (bswitch)return; 

	if (isButtonPressed & XINPUT_GAMEPAD_DPAD_LEFT)
	{
		if (m_ChannelDlg->IsActive())
		{
			CHA_DlgBtnFunc(CHA_CHANNELPAGE01, NULL, WE_BTNCLICK);
		}
		else
		{
			QI_QuickDlgFunc(QI_PREVPAGEBTN, NULL, WE_BTNCLICK);
		}
		return;
	}
	if (isButtonPressed & XINPUT_GAMEPAD_DPAD_RIGHT)
	{
		if (m_ChannelDlg->IsActive())
		{
			CHA_DlgBtnFunc(CHA_CHANNELPAGE02, NULL, WE_BTNCLICK);
		}

		else
		{
			QI_QuickDlgFunc(QI_NEXTPAGEBTN, NULL, WE_BTNCLICK);
		}
		return;
	}
}
BOOL bSelect;
WORD mapNum;
void CHero::GamePadActionButtonProcess(BOOL bswitch)
{
	WORD isButtonPressed = Player[0]->GetState().Gamepad.wButtons;
	CChannelDialog* m_ChannelDlg = (CChannelDialog*)WINDOWMGR->GetWindowForID(CHA_CHANNELDLG);

	
	mapNum = 65535;
	if (/*!MOVEMGR->IsMoving(HERO) &&*/ !HERO->IsPKMode() && !PKMGR->IsPKLooted() && !HERO->IsDied())
	{
		WORD curMapNum = HERO->GetCharacterTotalInfo()->CurMapNum;
		mapNum = GAMERESRCMNGR->CheckMapChange(&gHeroPos, curMapNum);
		//CHATMGR->AddMsg(CTC_SYSMSG, "mapNum %d", mapNum);
		if (mapNum != 65535)
		{
			OBJECTMGR->SetAllNameShow(TRUE);
			bSelect = TRUE;
		}
		else
		{
			OBJECTMGR->SetAllNameShow(FALSE);
			bSelect = FALSE;
		}
	}


	if (isButtonPressed & XINPUT_GAMEPAD_A)
	//if (!GamePadTestButton(XINPUT_GAMEPAD_A) && isButtonPressed == 0)
	{
		//CHATMGR->AddMsg(CTC_SYSMSG, "A");
		if (m_ChannelDlg)
		{
			if (m_ChannelDlg->IsActive())
			{
				MOVEMGR->HeroMoveStop();

				if (MAINGAME->GetCurStateNum() == eGAMESTATE_GAMEIN)
					m_ChannelDlg->MapChange();
			}
		}

		if (bSelect)
		{			
			if (/*!MOVEMGR->IsMoving(HERO) && */!HERO->IsPKMode() && !PKMGR->IsPKLooted() && !HERO->IsDied())
			{
				if (mapNum != 65535)
				{
					OBJECTSTATEMGR->StartObjectState(HERO, eObjectState_Deal);
					CHARSELECT->GetChannelDialog()->SendMapChannelInfoSYN(mapNum);
				}
			}
			
		}
		else
		{			
			sCPlayerAI.ProcessLookMonsterGamepad();
		}
		
		return;
	}
	//BOOL autosearch = FALSE;
	if (isButtonPressed & XINPUT_GAMEPAD_X)
	//else if (!GamePadTestButton(XINPUT_GAMEPAD_X) && isButtonPressed == 0)
	{
		sCPlayerAI.ProcessLookMonsterGamepad();
		MACROMGR->PlayMacro(ME_USE_QUICKITEM01 + column);
		return;
	}
	if (isButtonPressed & XINPUT_GAMEPAD_Y)
	//else if (!GamePadTestButton(XINPUT_GAMEPAD_Y) && isButtonPressed == 0)
	{
		sCPlayerAI.ProcessLookMonsterGamepad();
		MACROMGR->PlayMacro(ME_USE_QUICKITEM02 + column);
		return;
	}
	if (isButtonPressed & XINPUT_GAMEPAD_B)
	//else if (!GamePadTestButton(XINPUT_GAMEPAD_B) && isButtonPressed == 0)
	{
		if (m_ChannelDlg->IsActive())
		{
			CHA_DlgBtnFunc(CHA_CONNNECTCANCEL, NULL, WE_BTNCLICK);
		}
		else
		{
			sCPlayerAI.ProcessLookMonsterGamepad();
			MACROMGR->PlayMacro(ME_USE_QUICKITEM03 + column);
		}
		return;
	}

	if (isButtonPressed & XINPUT_GAMEPAD_BACK)
	//else if (!GamePadTestButton(XINPUT_GAMEPAD_BACK) && isButtonPressed == 0)
	{
		if (m_ChannelDlg)
		{
			if (m_ChannelDlg->IsActive())
			{
				CHA_DlgBtnFunc(CHA_CONNNECTCANCEL, NULL, WE_BTNCLICK);
			}
		}
		return;
	}
}
static BOOL mswitch;
void CHero::GamePadButtonProcess()
{
	BYTE L1 = Player[0]->GetState().Gamepad.bLeftTrigger;
	BYTE R1 = Player[0]->GetState().Gamepad.bRightTrigger;
	static BYTE bSwitch;

	bSwitch = (L1 && R1);

	static int timeTemp;
	WORD isButtonPressed = Player[0]->GetState().Gamepad.wButtons;
	
	if (gCurTime - timeTemp >= 400)
	{
		GamePadActionButtonProcess(bSwitch);
		GamePadDInputProcess(bSwitch);

		if (bSwitch)
			timeTemp = 0;
		else
			timeTemp = gCurTime;		
	}
}
WORD CHero::GamePadTestButton(WORD key)
{
	//if (key == 0) goto Reload_AllBin;
	static WORD isButtonPressedT;
	static bool isswitch;
	if(key != 0)
		isButtonPressedT = key;
	else
		isButtonPressedT = 0;

	const WORD isButtonPressed = Player[0]->GetState().Gamepad.wButtons;
	
	if (key & isButtonPressed)
	{
		if (!bswitchpad)
		{
			bswitchpad = TRUE;
			isswitch = true;
			return 1;
		}		
	}
	else
	{
		if (bswitchpad)
		{			
			bswitchpad = FALSE;
			return 2;
		}		
	}
}

#include "MouseCursor.h"

void CHero::GamePad()
{
	if (HERO)
	{
		if (Player[0]->IsConnected())
		{
			if (!bswitch)
			{
				vibLeft = vibright = 0;
				vbAllowHaptic = FALSE;
				bswitch = TRUE;
				isConnected = TRUE;
				GAMEIN->GetShoutchatDlg()->AddMsg("Connected", 2);
				Gamepadmode = TRUE;
			}
			
			GamePadCameraProcess();
			//GamePadMoveProcess();
			GamePadHapticProcess();
			GamePadButtonProcess();
		}
		else
		{
			if (bswitch)
			{
				Gamepadmode = FALSE;
				//delete(Player[0]);
				bswitch = FALSE;
				isConnected = FALSE;
				GAMEIN->GetShoutchatDlg()->AddMsg("Disonnected", 2);
			}
		}
	}
}
#define HapticFeedbackMax 20767.5 
#define HapticFeedbackMaxF 22767.5 //65535
void CHero::SetXboxVibration(int left, int right)
{
	vbAllowHaptic = TRUE;
	if (left > HapticFeedbackMax)
		vibLeft = HapticFeedbackMax;
	else
		vibLeft = left;

	if (right > HapticFeedbackMax)
		vibright = HapticFeedbackMax;
	else
		vibright = right;
	
}
void CHero::Jump()
{
	//if (GetState() == eObjectState_Rest)
	//	CONDUCTMGR->LifeSkillCommand(1);

	if ((GetState() != eObjectState_Move) && (GetState() != eObjectState_None))
	{
		return;
	}

	/*if (m_JumpMode != eJumpMode_None)
	{
		return;
	}*/

	/*if (mAbnormalStatus.IsStun ||
		mAbnormalStatus.IsFreezing ||
		mAbnormalStatus.IsStone ||
		mAbnormalStatus.IsSleep ||
		mAbnormalStatus.IsParalysis)
	{
		return;
	}*/

	//m_JumpMode = eJumpMode_Start;

	if (GetState() == eObjectState_Move) SetState(eObjectState_Move);
	if (GetState() == eObjectState_None) SetState(eObjectState_None);

	/*MSG_MOVE_JUMPMODE msg;
	msg.Category = MP_MOVE;
	msg.dwObjectID = HEROID;
	msg.Protocol = MP_MOVE_JUMPMODE;
	msg.eJumpMode = eJumpMode_Start;*/


	//NETWORK->Send(&msg, sizeof(msg));
}
void CHero::Move(int nKeyMoveDir)
{
	if (!KeyboardMode&&!Gamepadmode)return;

	if (this->GetState() == eObjectState_Engrave)
		return ;
	if (this->GetState() == eObjectState_Deal)
		return ;
	if (this->IsInSpecialState(eSpecialState_Stun))
		return ;
	if (this->GetState() != eObjectState_None &&
		this->GetState() != eObjectState_Move &&
		this->GetState() != eObjectState_TiedUp_CanMove &&
		this->GetState() != eObjectState_Immortal)
	{
		if (this->GetState() == eObjectState_Society)
		{
			return ;
		}
		else
		{
			return ;
		}
		return ;
	}
	if (HERO->GetState() == eObjectState_Deal)
		return;
	if (HERO->GetState() != eObjectState_None &&
		HERO->GetState() != eObjectState_Move &&
		HERO->GetState() != eObjectState_TiedUp_CanMove &&
		HERO->GetState() != eObjectState_Immortal)
	{
		return;
	}
	
	VECTOR3 Depan = CAMERA->GetForwardVector();
	VECTOR3 Belakang = Depan * -1;
	VECTOR3 Kiri, Kanan;
	Kiri.x = Depan.z * -1;
	Kiri.y = 0;
	Kiri.z = Depan.x;
	Kanan.x = Depan.z;
	Kanan.y = 0;
	Kanan.z = Depan.x * -1;

	VECTOR3 stPos;

	if (nKeyMoveDir)
	{
		HERO->GetPosition(&stPos);

		if (nKeyMoveDir & KEY_MOVE_LEFT)	stPos = stPos + Kiri		* m_KeyMove_DistOneStep;
		if (nKeyMoveDir & KEY_MOVE_RIGHT)	stPos = stPos + Kanan		* m_KeyMove_DistOneStep;
		if (nKeyMoveDir & KEY_MOVE_UP)		stPos = stPos + Depan		* m_KeyMove_DistOneStep;
		if (nKeyMoveDir & KEY_MOVE_DOWN)	stPos = stPos + Belakang	* m_KeyMove_DistOneStep;

		//if (MAP->CollisionCheck_OneLine_New(&HERO->m_MoveInfo.CurPosition, &stPos) == TRUE)
		{
		//	Move_Simple(&stPos);
			MOVEMGR->SetHeroTarget(&stPos, true);//·ÀÖ¹Ó¢ÐÛÅ×³öµØÍ¼£¬awsd
		//	return;
		}
		/*else
		{
			if (nKeyMoveDir & KEY_MOVE_LEFT)	stPos = stPos + Kiri		* 500;
			if (nKeyMoveDir & KEY_MOVE_RIGHT)	stPos = stPos + Kanan		* 500;
			if (nKeyMoveDir & KEY_MOVE_UP)		stPos = stPos + Depan		* 500;
			if (nKeyMoveDir & KEY_MOVE_DOWN)	stPos = stPos + Belakang	* 500;
			Move_Simple(&stPos);
		}*/
	}	
}
void CHero::MoveStuck(int nKeyMoveDir)
{
	if (MHTIMEMGR->GetInstance()->CheckTimer400())
	{
		VECTOR3 Depan = CAMERA->GetForwardVector();
		VECTOR3 Belakang = Depan * -1;
		VECTOR3 Kiri, Kanan, stPos;
		Kiri.x = Depan.z * -1;
		Kiri.y = 0;
		Kiri.z = Depan.x;
		Kanan.x = Depan.z;
		Kanan.y = 0;
		Kanan.z = Depan.x * -1;

		HERO->GetPosition(&stPos);

		if (nKeyMoveDir == 1)	stPos = stPos + Belakang				* m_KeyMove_DistOneStep;
		if (nKeyMoveDir == 2)	stPos = stPos + Belakang + Kiri			* m_KeyMove_DistOneStep;
		if (nKeyMoveDir == 3)	stPos = stPos + Belakang + Depan		* m_KeyMove_DistOneStep;
		if (nKeyMoveDir == 4)	stPos = stPos + Belakang + Kanan		* m_KeyMove_DistOneStep;

		if (nKeyMoveDir == 5)	stPos = stPos + Depan					* m_KeyMove_DistOneStep;
		if (nKeyMoveDir == 6)	stPos = stPos + Kiri					* m_KeyMove_DistOneStep;
		if (nKeyMoveDir == 7)	stPos = stPos + Belakang				* m_KeyMove_DistOneStep;
		if (nKeyMoveDir == 8)	stPos = stPos + Kanan					* m_KeyMove_DistOneStep;

		//MOVEMGR->SetHeroTarget(&stPos, 1.f);
		MOVEMGR->SetHeroTarget(&stPos, FALSE);
		//g_UserInput.GetHeroMove()->OnClickGround(MOUSEEVENT_LPRESS, &stPos);		
	}
}
void CHero::KillEngravingDlg()
{
	MSGBASE msg;
	msg.Category = MP_FORTWAR;
	msg.Protocol = MP_FORTWAR_ENGRAVE_CANCEL_SYN;
	msg.dwObjectID = HEROID;
	NETWORK->Send( &msg, sizeof(msg) );
}



//void CHero::SetFlashNameFlag(int Flag)
//{
//	m_CharacterInfo.FlashNameFlag=Flag;
//
//	SetFlashNameBalloonFlag(Flag);
//}
//void CHero::SetFlashName(char * pName)
//{
//   if(!pName)
//   {
//	   return;
//   }
//
//   SafeStrCpy(m_CharacterInfo.FlashName,pName,MAX_NAME_LENGTH+1);
//
//   SetFlashNameBalloon(pName);
//}

void CHero::SetKillCount(DWORD val)
{
	m_HeroTotalInfo.dwKillPlayerTimes = val;

	std::string s = std::to_string(val);
	char* c = strcpy(new char[s.length() + 1], s.c_str());

	SetKillCountBalloon(c);	
}

void CHero::SetVipLevel(WORD level)
{
	m_HeroTotalInfo.VipLevel = level;

	CPlayer::SetVipLevel(level);

}

