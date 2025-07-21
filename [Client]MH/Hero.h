
#pragma once

#include "Player.h"
#include "CAction.h"
#include "AbilityGroup.h"
#include "../[CC]Skill/DelayGroup.h"
#include "..\[CC]Header\GameResourceStruct.h"
#include "Item.h"  // include the item.h for character equip ! 2014-08-12
#include "CWayPointManager.h"
#include "CXBOXController.h"

#include "monster.h"
#define _MAX_MAINTITLE_MONSTER_NUM 3

class CWayPointManager;
class CHero : public CPlayer  
{
	CDelayGroup		m_DelayGroup;

	//{
	HERO_TOTALINFO		m_HeroTotalInfo;

	EXPTYPE				m_maxExpPoint;

	//}

	player_calc_stats m_charStats;
	player_calc_stats m_itemStats;
	SET_ITEM_OPTION m_setItemStats;	// 2007. 6. 18. CBH - ¼¼Æ®¾ÆÀÌÅÆ ´É·ÂÄ¡ ±¸Á¶Ã¼ Ãß°¡
	UNIQUE_ITEM_OPTION_INFO m_UniqueItemStats;	// magi82 - UniqueItem(070626)

	CAction		m_MovingAction;
	CAction		m_NextAction;

	CAction		m_SkillStartAction;		// ¹«°ø¹ßµ¿ »óÅÂ°¡ Áö³ª¸é ½ÃÀÛÇÒ ¿¢¼Ç

	//////////////////////////////////////////////////////////////////////////
	DWORD		m_CurComboNum;

	BOOL		m_bIsAutoAttackMode;
	BOOL		m_bIsAutoAttacking;
	CActionTarget m_AutoAttackTarget;
	
	BOOL		m_bSkillCooling;
	DWORD		m_SkillCoolTimeDuration;
	DWORD		m_SkillCoolTimeStart;
	//////////////////////////////////////////////////////////////////////////

	BOOL		m_bIsKyungGongMode;

	BYTE		m_PyogukNum;

	CAbilityGroup m_AbilityGroup;

	BOOL		m_bUngiFlag;
	

	// Play Time
	stPlayTime	m_stPlayTime;

	DWORD	m_dwLastSocietyAction;

	BOOL		m_bActionPenalty;

	VECTOR3 m_WayPoint;
	BOOL    m_bNeedMove;

	bool	bAllowedMoveToPos;

	BOOL	bDebugInfo;
	SET_ITEMQUALITY_OPTION m_setItemQualityStats;//¾õÐÑ
	//
	
public:
	CHero();
	virtual ~CHero();
	void InitHero(HERO_TOTALINFO * pHeroTotalInfo);
	void GetHeroTotalInfo(HERO_TOTALINFO * info);
	HERO_TOTALINFO* GetHeroTotalInfo()		{	return &m_HeroTotalInfo;		}
	void Process();


	BOOL ActionProcess();

	CDelayGroup*	GetDelayGroup()		{	return &m_DelayGroup;	}
	CAbilityGroup*	GetAbilityGroup()	{	return &m_AbilityGroup;	}

	player_calc_stats* GetCharStats()			{	return &m_charStats;		}
	player_calc_stats* GetItemStats()			{	return &m_itemStats;		}
	// 2007. 6. 18. CBH - ¼¼Æ®¾ÆÀÌÅÆ ´É·ÂÄ¡ ±¸Á¶Ã¼ ¹ÝÈ¯ ÇÔ¼ö Ãß°¡
	SET_ITEM_OPTION* GetSetItemStats()		{	return &m_setItemStats;		}	
	SET_ITEMQUALITY_OPTION* GetSetItemQualityStats() { return &m_setItemQualityStats; }//¾õÐÑ
	UNIQUE_ITEM_OPTION_INFO* GetUniqueItemStats(){	return &m_UniqueItemStats;	}	// magi82 - UniqueItem(070626)
	
	// InitXXX°è¿­ÀÇ ÇÔ¼ö È£Ãâ ÈÄ  ÀÎÅÍÆäÀÌ½º¿¡ Ç¥½ÃÇÒ ÇÊ¿ä°¡ÀÖ´Â °Íµé À§ÇØ
	void ChangeLife(int changeval);
	void ChangeShield(int changeval);
	void ApplyInterface();
	virtual void SetMaxLife(DWORD maxlife);
	virtual void SetMaxShield(DWORD maxlife);
	virtual void SetLife(DWORD life, BYTE type = 1);
	virtual void SetShield(DWORD life, BYTE type = 1);
	virtual DWORD GetNaeRyuk(){ return m_HeroTotalInfo.wNaeRyuk; }
	virtual DWORD DoGetMaxNaeRyuk();
	virtual void SetMaxNaeRyuk(DWORD val);
	virtual void SetNaeRyuk(DWORD val, BYTE type = 1);
	virtual DWORD DoGetPhyDefense();

	virtual float DoGetAttDefense(WORD Attrib);
	virtual DWORD DoGetPhyAttackPowerMin();//¼ÆËãÎäÆ÷¹¥»÷½ü¾àÀë»òÔ¶¾àÀë
	virtual DWORD DoGetPhyAttackPowerMax();//¼ÆËãÎäÆ÷¹¥»÷½ü¾àÀë»òÔ¶¾àÀë
#ifdef _JAPAN_LOCAL_
	virtual float DoGetAddAttackRange(){	return (float)GetMinChub() / 2.f;	}
#else
	virtual float DoGetAddAttackRange(){	return GetMinChub() / 3.f;	}
#endif


	virtual DWORD DoGetCritical() { return GetCharStats()->Critical + GetItemStats()->Critical + GetSetItemStats()->wCriticalPercent + GetSetItemQualityStats()->Critical; }
	virtual DWORD DoGetDecisive() { return GetCharStats()->Decisive + GetItemStats()->Critical + GetSetItemStats()->wCriticalPercent + GetSetItemQualityStats()->Decisive; }

	LEVELTYPE GetMaxLevel()		{	return m_HeroTotalInfo.MaxLevel;	}
	void SetMaxLevel( LEVELTYPE Level )			{	m_HeroTotalInfo.MaxLevel = Level;	}

	void SetGuageName(char * szName);
	// 2014-05-04 The Money Hero Interface!
	
	MONEYTYPE GetMoney() { return m_HeroTotalInfo.Money; }
	void SetMoney(MONEYTYPE Money);

	MONEYTYPE GetMallMoney(){return m_HeroTotalInfo.MallMoney;}
	void SetMallMoney(MONEYTYPE MallMoney);

	MONEYTYPE GetGoldMoney(){return m_HeroTotalInfo.GoldMoney;}
	void SetGoldMoney(MONEYTYPE GoldMoney);

	void SetReset(WORD Val){m_HeroTotalInfo.ReSet=Val;}

	WORD GetReset(){return m_HeroTotalInfo.ReSet;}

	EXPTYPE GetExpPoint() { return m_HeroTotalInfo.ExpPoint; }
	EXPTYPE GetMaxExpPoint() { return m_maxExpPoint; }
	void SetExpPoint(EXPTYPE dwPoint, BYTE type=1);
	void SetMaxExpPoint(EXPTYPE dwPoint);
	virtual void SetLevel(LEVELTYPE level);
	
	void SetMunpa();

	void SetGenGol(WORD val);
	void SetMinChub(WORD val);

	void SetCheRyuk(WORD val);
	void SetSimMek(WORD val);

	// magi82 - UniqueItem(070629) - ½ºÅÝÀÌ ¸¶ÀÌ³Ê½º°¡ µÉ °æ¿ì ¿¹¿ÜÃ³¸® Ãß°¡
	WORD GetGenGol(){ return (int)(m_HeroTotalInfo.wGenGol + GetAbilityStats()->StatGen + GetItemStats()->GenGol + GetShopItemStats()->Gengol
		+ GetAvatarOption()->Gengol + GetSetItemStats()->wGenGol + GetUniqueItemStats()->nGengol + GetSetItemQualityStats()->wGenGol) >= 0 ? (WORD)(m_HeroTotalInfo.wGenGol
		+ GetAbilityStats()->StatGen + GetItemStats()->GenGol + GetShopItemStats()->Gengol + GetAvatarOption()->Gengol + GetSetItemStats()->wGenGol
		+ GetUniqueItemStats()->nGengol + GetSetItemQualityStats()->wGenGol) : 0; }

	WORD GetMinChub(){ return 
		(int)		
		(
		m_HeroTotalInfo.wMinChub + 
		GetAbilityStats()->StatMin + 
		GetItemStats()->MinChub + 
		GetShopItemStats()->Minchub + 
		GetAvatarOption()->Minchub + 
		GetSetItemStats()->wMinChub + 
		GetUniqueItemStats()->nMinChub
			+ GetSetItemQualityStats()->wMinChub
		)
		
		>= 0 ? 

		(WORD)
		(
		m_HeroTotalInfo.wMinChub + 
		GetAbilityStats()->StatMin + 
		GetItemStats()->MinChub +
		GetShopItemStats()->Minchub + 
		GetAvatarOption()->Minchub + 
		GetSetItemStats()->wMinChub +
		GetUniqueItemStats()->nMinChub
			+ GetSetItemQualityStats()->wMinChub
		) 
		
		: 0
		
		;
	
	}

	WORD GetCheRyuk(){ return (int)(m_HeroTotalInfo.wCheRyuk + GetAbilityStats()->StatChe + GetItemStats()->CheRyuk + GetShopItemStats()->Cheryuk
		+ GetAvatarOption()->Cheryuk + GetSetItemStats()->wCheRyuk + GetUniqueItemStats()->nCheRyuk + GetSetItemQualityStats()->wCheRyuk) >= 0 ? (WORD)(m_HeroTotalInfo.wCheRyuk
		+ GetAbilityStats()->StatChe + GetItemStats()->CheRyuk + GetShopItemStats()->Cheryuk + GetAvatarOption()->Cheryuk + GetSetItemStats()->wCheRyuk
		+ GetUniqueItemStats()->nCheRyuk + GetSetItemQualityStats()->wCheRyuk) : 0; }

	WORD GetSimMek(){ return (int)(m_HeroTotalInfo.wSimMek + GetAbilityStats()->StatSim + GetItemStats()->SimMaek + GetShopItemStats()->Simmek
		+ GetAvatarOption()->Simmek + GetSetItemStats()->wSimMek + GetUniqueItemStats()->nSimMek + GetSetItemQualityStats()->wSimMek) >= 0 ? (WORD)(m_HeroTotalInfo.wSimMek
		+ GetAbilityStats()->StatSim + GetItemStats()->SimMaek + GetShopItemStats()->Simmek + GetAvatarOption()->Simmek + GetSetItemStats()->wSimMek
		+ GetUniqueItemStats()->nSimMek + GetSetItemQualityStats()->wSimMek) : 0; }
	
	void SetAbilityExp(DWORD val)	{	m_AbilityGroup.SetAbilityExp(val);	}
	DWORD GetAbilityExp()			{ return m_AbilityGroup.GetAbilityExp(); 	}

	int GetMugongLevel(WORD MugongIdx);
	//////////////////////////////////////////////////////////////////////////
	// 06. 06. 2Â÷ ÀüÁ÷ - ÀÌ¿µÁØ
	// ¹«°ø º¯È¯ Ãß°¡
	WORD GetSkillOptionIndex(WORD MugongIdx);
	//////////////////////////////////////////////////////////////////////////
	
	void SetFame(FAMETYPE val);
	FAMETYPE GetFame()			{ return m_HeroTotalInfo.Fame;	}

	virtual void SetBadFame( FAMETYPE val );

	DWORD GetPartyIdx() { return m_HeroTotalInfo.PartyID; }
	void SetPartyIdx(DWORD PartyId) { m_HeroTotalInfo.PartyID = PartyId;	}
/*¹®ÆÄ*/		
	void SetMunpaName(char* Name);
	
	void SetGuildName(char* Name);
	char* GetGuildName();
	virtual void ClearGuildInfo();
	char* GetGuildEntryDate();
	void SetGuildEntryDate(char* day);
	
	BYTE GetPyogukNum()				{ return m_PyogukNum;	}
	void SetPyogukNum(BYTE num)		{ m_PyogukNum = num;	}
	
	// stage
	virtual void	SetStage( BYTE Stage );

	//weiye ½á»éÏà¹Øº¯Êý¶¨Òå 2018-04-29

	void SetMarryName(char * pName);
	char*	GetMarryName();
	
public:

	// marry msg define 2014-05-09

	MSG_MARRY_ADDMSG m_mary_msg;


	// ×°±¸ 2014-08-12

	CItem *	m_OtherPlayerEquip[eWearedItem_Max];

	//////////////////////////////////////////////////////////////////////////
	// SkillManager¿¡¼­ ¾²´Â ÇÔ¼öµé
	BOOL IsAutoAttacking()	{	return m_bIsAutoAttacking;	}
	CActionTarget* GetAutoAttackTarget()	{	return &m_AutoAttackTarget;	}

	void SetMovingAction(CAction* pAction)	{	m_MovingAction.CopyFrom(pAction);	}
	void SetNextAction(CAction* pAction)	{	m_NextAction.CopyFrom(pAction);	}
	void SetSkillStartAction(CAction* pAction)	{	m_SkillStartAction.CopyFrom(pAction);	}

	CAction* GetNextAction()	{	return &m_NextAction;	}

	DWORD GetCurComboNum()			{ return m_CurComboNum < MAX_COMBO_NUM ? m_CurComboNum : 0;	}
	void SetCurComboNum(DWORD num)	{ m_CurComboNum = num;	}

	void EnableAutoAttack(CActionTarget* pTarget)
	{	if(m_bIsAutoAttackMode && pTarget->GetTargetKind() == eActionTargetKind_Object){
		m_bIsAutoAttacking = TRUE;	m_AutoAttackTarget.CopyFrom(pTarget); }
	}
	void DisableAutoAttack()
	{	m_bIsAutoAttacking = FALSE; m_AutoAttackTarget.CopyFrom(NULL); }
	void ToggleAutoAttackMode()
	{	m_bIsAutoAttackMode = !m_bIsAutoAttackMode;	}

	void ClearTarget(CObject* pObject);

	BOOL IsSkillCooling()	{	return m_bSkillCooling;	}
	void SetSkillCoolTime(DWORD CoolTime)	{
		m_SkillCoolTimeDuration = CoolTime;
		m_SkillCoolTimeStart = gCurTime;
		m_bSkillCooling = TRUE;
	}
	//////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////
	// °æ°ø
	BOOL GetKyungGongMode();
	void SetKyungGongMode(BOOL bMode);
	//////////////////////////////////////////////////////////////////////////	
	
	//////////////////////////////////////////////////////////////////////////
	// ObjectStateManager¿¡¼­ StateÀÇ ½ÃÀÛ°ú ³¡¿¡¼­ È£ÃâÇØÁÖ´Â ÇÔ¼öµé
	void OnStartObjectState(BYTE State);

	void OnEndObjectState(BYTE State);
	
//KES
	virtual BOOL StartSocietyAct( int nStartMotion, int nIngMotion = -1, int nEndMotion = -1, BOOL bHideWeapon = FALSE );
	virtual BOOL EndSocietyAct();
	void	StartSocietyActForQuest( int nStartMotion, int nIngMotion = -1, int nEndMotion = -1, BOOL bHideWeapon = FALSE );
	BOOL CanSocietyAction( DWORD curTime );
	//////////////////////////////////////////////////////////////////////////

	void OnHitTarget(CObject* pMainTarget);
	
	// Çàµ¿ ÇÔ¼öµé...
	void Die(CObject* pAttacker,BOOL bFatalDamage,BOOL bCritical, BOOL bDecisive);
	void Damage(CObject* pAttacker,BYTE DamageKind,DWORD Damage,DWORD ShieldDamage,BOOL bCritical, BOOL bDecisive, DWORD titanObserbDamage);
	void Revive(VECTOR3* pRevivePos);
	void Heal(CObject* pHealer,BYTE HealKind,DWORD HealVal);

	friend class CAttackManager;

	WORD GetKyungGongGrade()	{	return m_HeroTotalInfo.KyungGongGrade;	}
	void SetKyungGongGrade(WORD grade);
	
	void HeroStatusReload();
	virtual void AddStatus(CStatus* pStatus);
	virtual void RemoveStatus(CStatus* pStatus);
	

	// ¼­¹ö¸¦ º¯°æÇÏ°í ¾Æ¹«·± Ã³¸®°¡ ¾øÀ»°Ü¿ì »ç¿ëÇÒ°Í..
	void HeroStateNotify(BYTE State);
	
	BYTE GetNaeRyukPercent();

	// PlayTime
	void SetPlayTime(int time)	{	m_stPlayTime.value = time;	}
	int GetPlayTime()			{	m_stPlayTime.value;	}
/*	void GetPlayTime(int& Year, int& Day, int& Hour, int& Minute, int& Second)
	{
		m_stPlayTime.GetTime(Year, Day, Hour, Minute, Second);
	}*/

	//
	void CalcShopItemOption( WORD wIdx, BOOL bAdd, DWORD Param=0 );
	void ActiveOptionInfoToInterface();
	void CheckShopItemUseInfoToInventory( WORD ItemIdx, WORD ItemPos );

	void UseShopItem( WORD ItemIdx, WORD ItemPos );

	void RefreshLevelupPoint();
	void SetHeroLevelupPoint( LEVELTYPE dwPoint )	{	m_HeroTotalInfo.LevelUpPoint = dwPoint;	}
	DWORD GetHeroLevelUpPoint()					{	return m_HeroTotalInfo.LevelUpPoint;	}
	
	//ÊôÐÔ¹¥»÷Á¦
	DWORD DoGetAttAttackPowerMax(WORD Attrib);
	DWORD DoGetAttAttackPowerMin(WORD Attrib);
	DWORD GetAttribPlusPercent(WORD Attrib);
#ifdef _JAPAN_LOCAL_
	DWORD GetExtraInvenSlot()					{	return m_CharacterInfo.ExtraInvenSlot;		}
	DWORD GetExtraPyogukSlot()					{	return m_CharacterInfo.ExtraPyogukSlot;		}
	DWORD GetExtraMugongSlot()					{	return m_CharacterInfo.ExtraMugongSlot;		}

//KES: Show AttrAttack Damage
	DWORD DoGetAttAttackPowerMax( WORD Attrib );
	DWORD DoGetAttAttackPowerMin( WORD Attrib );
	DWORD GetAttribPlusPercent( WORD Attrib );

#endif
#ifdef _HK_LOCAL_
	DWORD GetExtraInvenSlot()					{	return m_CharacterInfo.ExtraInvenSlot;		}
	DWORD GetExtraPyogukSlot()					{	return m_CharacterInfo.ExtraPyogukSlot;		}
	DWORD GetExtraMugongSlot()					{	return m_CharacterInfo.ExtraMugongSlot;		}
#endif
#ifdef _TL_LOCAL_
	DWORD GetExtraInvenSlot()					{	return m_CharacterInfo.ExtraInvenSlot;		}
	DWORD GetExtraPyogukSlot()					{	return m_CharacterInfo.ExtraPyogukSlot;		}
	DWORD GetExtraMugongSlot()					{	return m_CharacterInfo.ExtraMugongSlot;		}

#endif

	DWORD GetExtraMugongSlot()					{ return m_CharacterInfo.ExtraMugongSlot; }

	////////////////////////////////////////////////////////
	//06. 06 2Â÷ ÀüÁ÷ - ÀÌ¿µÁØ
	//ÀÌÆåÆ® »ý·«(¹«ÃÊ)
	BOOL IsSkipSkill();
	////////////////////////////////////////////////////////

	//SW070127 Å¸ÀÌÅº
	BOOL CanUseTitanSkill();

	//////2007. 6. 14. CBH - ¼¼Æ®¾ÆÀÌÅÆ ´É·ÂÄ¡ °è»ê ÇÔ¼ö Ãß°¡
	void AddSetitemOption(SET_ITEM_OPTION* pSetItemOption);
	void ClearSetitemOption();
	//////////////////////////////////////////////////////////

	void SetNoActionPenalty()					{	m_bActionPenalty = FALSE;		}

	void AddUniqueItemOption(UNIQUE_ITEM_OPTION_INFO* pUniqueOption);
	void ClearUniqueItemOption()	{	ZeroMemory(&m_UniqueItemStats, sizeof(m_UniqueItemStats));	}


	int   GetFlashNameFlag(){return m_CharacterInfo.FlashNameFlag;}
	void  SetFlashNameFlag(int Flag);

	BOOL IsImageName(){return m_HeroTotalInfo.ImageName>0;}
	void SetImageName(WORD ImageName);

	int GetSafeStatic(){return m_CharacterInfo.SafeStatic;}
	void SetSafeStatic(int Flag){m_CharacterInfo.SafeStatic=Flag;}
	BOOL IsCanUseMarryChar();    //Í¬ÐÄ·û
	//////////////////////////////////////////////////////////////////////////


	char* GetFlashName(){return m_CharacterInfo.FlashName;}
	void  SetFlashName(char * pName);


	BOOL    IsTurnOne(){return  m_CharacterInfo.TurnStageOne!=0;}
	void    SetTurnOne(int Flag){m_CharacterInfo.TurnStageOne=Flag;}
	WORD    GetTurnOne(){return m_CharacterInfo.TurnStageOne;}

	BOOL    IsTurnTwo(){return  m_CharacterInfo.TurnStageTwo!=0;}
	void    SetTurnTwo(int Flag){m_CharacterInfo.TurnStageTwo=Flag;}
	WORD    GetTurnTwo(){return m_CharacterInfo.TurnStageTwo;}

	BOOL    IsTurnThree(){return  m_CharacterInfo.TurnStageThree!=0;}
	void    SetTurnThree(int Flag){m_CharacterInfo.TurnStageThree=Flag;}
	WORD    GetTurnThree(){return m_CharacterInfo.TurnStageThree;}

	void SetKillCount(DWORD val);
	DWORD GetKillCount()			{ return m_HeroTotalInfo.dwKillPlayerTimes; }

public:
	CWayPointManager* m_WayPointManager;
	CXBOXController* Player[1];
	void CancelWayPoint();
	BOOL WayPoint_IsWayPointEmpty();
	void Stop_SendtoServer();
	void Jump();
	void ClearWayPoint(BYTE bState=0);
	BOOL Move_UsePath(VECTOR3* pDestPos,BOOL bSimpleMode,BOOL bMousePointDisplay,BOOL bIsCollisionPosMove=FALSE);
	BOOL Move_Simple(VECTOR3* pTargetPos, BOOL bRenderTileDebug=TRUE);
	BOOL NextMove();
	BOOL CheckMove();
	/*BOOL CanSendBobuSangInfoMSG(DWORD& value)
	{ 
		if (m_dwBobusangInfo == 0) return TRUE;
		value = gCurTime - m_dwBobusangInfo;
		return value >= 60000;
	}
	void SetBobuSangInfoMsgTime(){ m_dwBobusangInfo = gCurTime;}*/
private:
	float m_KeyMove_DistOneStep;
public:
	void Move_KeyboardInput();

	void GamePad();
	void Move(int nKeyMoveDir);

	void KillEngravingDlg();

protected:
	DWORD	m_dwFollowPlayerIdx;

	BOOL	m_SkipKyungong;

	int	vibLeft;
	int	vibright;

	BOOL vbAllowHaptic;
	BYTE m_KeyPress[65535];
public:
	void SetFollowPlayer( DWORD PlayerIdx )		{ m_dwFollowPlayerIdx = PlayerIdx; }
	DWORD GetFollowPlayer()						{ return m_dwFollowPlayerIdx; }

	VECTOR3* GetNextPos(){return &m_WayPoint;};


	//void MoveOne(VECTOR3* vPos);

	void MoveStuck(int nKeyMoveDir);

	BOOL IsKyungGongModeProcess();

	void SetSkipKyungong(BOOL b)		{ m_SkipKyungong = b; }

	void SetXboxVibration(int left=0, int right=0);
	void KeyboardModeCameraProcess();
	void ResetCursorPosition();
	void GamePadCameraProcess();
	void GamePadMoveProcess();
	void GamePadHapticProcess();
	void GamePadButtonProcess();
	void GamePadDInputProcess(BOOL bswitch=FALSE);
	void GamePadActionButtonProcess(BOOL bswitch = FALSE);
	WORD GamePadTestButton(WORD key);
	void RenderMoveDebugInfo();
	void SetMoveDebugInfo(BOOL bStart){ bDebugInfo = bStart; }
	BOOL GetMoveDebugInfo(){ return bDebugInfo ; }
	void MoveCheckingProcess();
	void TargetUpdate(CObject* pObject, MOVE_INFO * pMoveInfo, VECTOR3 * TargetPos);
	void AddSetitemQualityOption(SET_ITEMQUALITY_OPTION* pSetItemQualityOption);
	void ClearSetitemQualityOption();


};