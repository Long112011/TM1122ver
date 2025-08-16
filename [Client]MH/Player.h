
#pragma once

#include "Object.h"


class cStreetStallTitleTip; // LBS 03.09.23

class CPet;
//class CPetBase;
class CTitan;
class CMapObject;

class CPlayer : public CObject  
{
	void InitPlayer(CHARACTER_TOTALINFO* pTotalInfo);

	
	int m_StandardMotion;
	int m_Move_Start_Motion;
	int m_Move_Ing_Motion;
	int m_Move_End_Motion;

	ABILITY_STATS m_Ability;
	
	WANTEDTYPE			m_WantedIDX;

	//cTextStaticWid m_chatToolTip;										// Ã¤ÆÃÇ³¼±	
	
	int	m_ItemUseMotion;
	int m_ItemUseEffect;


	//SW070127 Å¸ÀÌÅº
	BOOL m_bInTitan;
	float m_TitanMoveSpeed;
	BOOL m_bTitanPreView;

	//2007. 8. 8. CBH - ¼¼Æ®¾ÆÀÌÅÆ ÀÌÆåÆ®
	DWORD m_dwSetItemEffectID;

protected:
	// CHERO¿¡¼­ Á¢±ÙÇÏ±â À§ÇØ 
	CHARACTER_TOTALINFO m_CharacterInfo;	
	SHOPITEMOPTION		m_ShopItemOption;	
	AVATARITEMOPTION	m_AvatarOption;

	//////////////////////////////////////////////////////////////////////////
	// 06. 06. 2Â÷ ÀüÁ÷ - ÀÌ¿µÁØ
	// ¹«°ø º¯È¯ Ãß°¡
	SKILLSTATSOPTION	m_SkillStatsOption;
	//////////////////////////////////////////////////////////////////////////
	//SW070127 Å¸ÀÌÅº
	TITAN_APPEARANCEINFO	m_TitanAppearanceInfo;
	TITAN_APPEARANCEINFO	m_TitanPreViewAppInfo;

	// ³ëÁ¡»ó Å¸ÀÌÆ² LBS 03.10.02
	cStreetStallTitleTip* m_pSSTitle;
	//SW051129 Pet
	CPet*				m_pPet;

	CTitan*				m_pTitan;

	// magi82(44) - ³ëÁ¡»ó ²Ù¹Ì±â ¾÷µ¥ÀÌÆ®
	CMapObject*			m_DecorationAddress;
	DWORD				m_dwDecorationStartTime;
	BOOL				m_bDecorationFlag;	// TRUE¸é »ý¼ºµÇ¹Ç·Î ¾ËÆÄ°ª Áõ±â, FALSE¸é ÇØÁ¦ µÇ¹Ç·Î ¾ËÆÄ°ª °¨¼Ò

	//2008. 1. 21. CBH - ½ºÅ² ·¹ÀÌ °ü·Ã º¯¼ö
	DWORD m_dwSkinDelayTime;	//·¹ÀÌ ÃÑ ½Ã°£ º¯¼ö
	BOOL m_bSkinDelayResult;	//·¹ÀÌÁßÀÎÁö Ã¼Å©ÇÏ´Â º¯¼ö
	///////////////////////////////////////////////////

	//weiye µ±Ç°³ÆºÅÍ¼Æ¬ItemIdx¶¨Òå 2018-04-21
	DWORD   m_NameImg;

public:
	CPlayer();
	virtual ~CPlayer();


	inline ABILITY_STATS* GetAbilityStats()		{	return &m_Ability;	}
	
	void GetCharacterTotalInfo(CHARACTER_TOTALINFO* pRtInfo);
	CHARACTER_TOTALINFO* GetCharacterTotalInfo()	{	return &m_CharacterInfo;	}

	void Process();

	friend class CObjectManager;
	friend class CAppearanceManager;
	friend void CS_BtnFuncEnter(LONG lId, void* p, DWORD we);

	void SetWearedItemIdx(DWORD WearedPosition,WORD ItemIdx);
	WORD GetWearedItemIdx(DWORD WearedPosition)					{	return m_CharacterInfo.WearedItemIdx[WearedPosition];	}
	WORD GetWeaponEquipType();
	WORD GetTitanWeaponEquipType();
	WORD GetTitanWearedItemIdx(DWORD WearedPosition);


	/*¹®ÆÄ*/
	void SetMunpaID(DWORD MunpaID)	{ m_CharacterInfo.MunpaID = MunpaID; 	}
	DWORD GetMunpaIdx()		{ return m_CharacterInfo.MunpaID; 	}
	
	void SetMunpaMemberRank(BYTE rank){ m_CharacterInfo.PositionInMunpa = rank; 	} //¹®ÆÄ¿¡¼­ÀÇ ÀÚ½ÅÀÇ ÁöÀ§ ¼³Á¤
	BYTE GetMunpaMemberRank()		{ return m_CharacterInfo.PositionInMunpa; 	} //¹®ÆÄ¿¡¼­ÀÇ ÀÚ½ÅÀÇ ÁöÀ§
	/*±æµå*/
	void SetGuildIdxRank(DWORD GuildIdx, BYTE Rank); 
	DWORD GetGuildIdx()		{ return m_CharacterInfo.MunpaID; 	}
	void SetGuildIdx(DWORD GuildIdx);
	void SetGuildMemberRank(BYTE rank){ m_CharacterInfo.PositionInMunpa = rank; 	} //¹®ÆÄ¿¡¼­ÀÇ ÀÚ½ÅÀÇ ÁöÀ§ ¼³Á¤
	BYTE GetGuildMemberRank()		{ return m_CharacterInfo.PositionInMunpa; 	} //¹®ÆÄ¿¡¼­ÀÇ ÀÚ½ÅÀÇ ÁöÀ§
	void SetGuildMarkName(MARKNAMETYPE MarkName);
	void SetNickName(char* pNickName);
	void SetGuildName(char* GuildName);

	char* GetGuildName();

	
	MARKNAMETYPE GetGuildMarkName();
	virtual void ClearGuildInfo();
	
	char* GetNickName();


	// magi82(47)
	void SetStreetStallBalloonImage( DWORD eKind, BOOL bFlag );	// ekind : ±¸¸Å³ëÁ¡»óÀÎÁö ÆÇ¸Å³ëÁ¡»óÀÎÁö  bFlag : TRUE¸é ¼¼ÆÃ FLASE¸é ÃÊ±âÈ­

	
	void SetPeaceMode(bool bPeace)		{	m_CharacterInfo.bPeace = bPeace;	}
	
	void SetBaseMotion();
	int GetStandardMotion()		{return m_StandardMotion;	}
	
	virtual void SetMotionInState(BYTE State);

//KES
	virtual BOOL StartSocietyAct( int nStartMotion, int nIngMotion = -1, int nEndMotion = -1, BOOL bHideWeapon = FALSE );

	//////////////////////////////////////////////////////////////////////////
	// ObjectStateManager¿¡¼­ StateÀÇ ½ÃÀÛ°ú ³¡¿¡¼­ È£ÃâÇØÁÖ´Â ÇÔ¼öµé
	virtual void OnStartObjectState(BYTE State);
	virtual void OnEndObjectState(BYTE State);
	//////////////////////////////////////////////////////////////////////////
	
	// LBS ³ëÁ¡»ó °ü·Ã 03.09.30//////////////////////
	void ShowStreetStallTitle( BOOL bShow, char* strTitle, DWORD dwColor = RGB_HALF( 0, 0, 200 ) );
	void SetStreetStallTitle( char* strTitle );
	void ShowStreetBuyStallTitle( BOOL bShow, char* strTitle, DWORD dwColor = RGB_HALF( 0, 0, 200 ) );
	void SetStreetBuyStallTitle( char* strTitle );
	
	/////////////////////////////////////////////////

	// Çàµ¿ ÇÔ¼öµé...
	virtual void Die(CObject* pAttacker,BOOL bFatalDamage,BOOL bCritical, BOOL bDecisive);
	virtual void Damage(CObject* pAttacker,BYTE DamageKind,DWORD Damage,DWORD ShieldDamage,BOOL bCritical, BOOL bDecisive, DWORD titanObserbDamage);
	virtual void Heal(CObject* pHealer,BYTE HealKind,DWORD HealVal);
	virtual void Recharge(CObject* pHealer,BYTE RechargeKind,DWORD RechargeVal);
	
	virtual DWORD GetLife(){	return m_CharacterInfo.Life; }
	virtual void SetMaxLife(DWORD maxlife){ m_CharacterInfo.MaxLife = maxlife; }
	virtual void SetLife(DWORD life, BYTE type = 1);
	virtual DWORD DoGetMaxLife();

	virtual DWORD GetShield(){	return m_CharacterInfo.Shield; }
	virtual void SetMaxShield(DWORD maxShield){ m_CharacterInfo.MaxShield = maxShield; }
	virtual void SetShield(DWORD Shield, BYTE type = 1);
	virtual DWORD DoGetMaxShield();

	// 2014-08-12 »ñÈ¡Ä§·¨Öµ ÎªÊôÐÔ!
	virtual DWORD GetNaeRyuk(){return m_CharacterInfo.naeryuk;}
	virtual float DoGetMoveSpeed();
	WORD GetKyungGongLevel() const { return m_MoveInfo.AbilityKyungGongLevel; }
	BYTE GetLifePercent();
	BYTE GetShieldPercent();
	
	virtual void SetLevel(LEVELTYPE level) { m_CharacterInfo.Level = level; }
	LEVELTYPE GetLevel() { return m_CharacterInfo.Level; }
	
	BYTE GetGender() { return m_CharacterInfo.Gender; }
	//////////////////////////////////////////////////////////////////////////
	// EffectManager¿¡¼­ È£ÃâÇØÁÖ´Â ÇÔ¼öµé
	
	// ÀÌÆåÆ®°¡ ½Ã°£ÀÌ ´ÙµÇ¼­ ³¡³µÀ»¶§ È£Ãâ

	BOOL OnEndEffectProc(HEFFPROC hEff);

	//////////////////////////////////////////////////////////////////////////

	//PKMODE
	void SetPKMode( BOOL bPKMode );
	BOOL IsPKMode()		{ return m_CharacterInfo.bPKMode; }

	//¾Ç¸í
	virtual void SetBadFame(FAMETYPE val);
	FAMETYPE GetBadFame()			{ return m_CharacterInfo.BadFame;	}	
	
	//Çö»ó±Ý
	WANTEDTYPE GetWantedIdx()	{ return m_WantedIDX; }
	void SetWantedIdx(WANTEDTYPE val)	{ m_WantedIDX = val; }
	
	// quest

//	CQuestGroup*	m_pQuestGroup;
//	CQuestGroup*	GetQuestGroup()		{ return m_pQuestGroup; }

	// ShopItemOption
	void SetAvatarInfo(WORD* Avatar)			{	memcpy( m_ShopItemOption.Avatar, Avatar, sizeof(WORD)*eAvatar_Max);	}
	void SetShopItemOptionInfo(SHOPITEMOPTION* pInfo);
	SHOPITEMOPTION*		GetShopItemStats()		{	return &m_ShopItemOption;	}
	void SetItemUseMotionIdx( int Idx )			{	m_ItemUseMotion = Idx;	}
	void SetItemUseEffectIdx( int Idx )			{	m_ItemUseEffect = Idx;	}

	//
	AVATARITEMOPTION*	GetAvatarOption()		{	return &m_AvatarOption;		}

	//////////////////////////////////////////////////////////////////////////
	// 06. 06. 2Â÷ ÀüÁ÷ - ÀÌ¿µÁØ
	// ¹«°ø º¯È¯ Ãß°¡
	SKILLSTATSOPTION*	GetSkillStatsOption()		{	return &m_SkillStatsOption;	}
	//////////////////////////////////////////////////////////////////////////
	
	// Pet
	void SetPet(CPet* pet);
	CPet* GetPet()					{	return m_pPet;	}

	// stage
	virtual void	SetStage( BYTE Stage );
	BYTE			GetStage()		{ return m_CharacterInfo.Stage; }

	// guildunion
	void	SetGuildUnionIdx( DWORD dwUnionIdx )	{ m_CharacterInfo.dwGuildUnionIdx = dwUnionIdx; }
	void	SetGuildUnionName( char* pName )		{ strncpy( m_CharacterInfo.sGuildUnionName, pName, MAX_GUILD_NAME+1 ); }

	void	SetGuildUnionMarkIdx( DWORD dwMarkIdx )	{ m_CharacterInfo.dwGuildUnionMarkIdx = dwMarkIdx; }
	DWORD	GetGuildUnionIdx()		{ return m_CharacterInfo.dwGuildUnionIdx; }
	char*	GetGuildUnionName()		{ return m_CharacterInfo.sGuildUnionName; }
	DWORD	GetGuildUnionMarkIdx()	{ return m_CharacterInfo.dwGuildUnionMarkIdx; }
	
	//
	void SetCharChangeInfo( CHARACTERCHANGE_INFO* pInfo );

	//
	void SetSiegeName( DWORD NameType );
	void SetRestraintMode( bool val )			{	m_CharacterInfo.bRestraint = val;	}
	BOOL IsRestraintMode()						{	return m_CharacterInfo.bRestraint;	}

	//
	void CalcAvatarOption();

	// for attribute - japan
#ifdef _JAPAN_LOCAL_
	void	SetMainCharAttr( int nMainAttr )	{ m_CharacterInfo.nMainAttr = nMainAttr; }
	void	SetSubCharAttr( int nSubAttr )		{ m_CharacterInfo.nSubAttr = nSubAttr; }
	int 	GetMainCharAttr()					{ return m_CharacterInfo.nMainAttr; }		// ÁÖ¼º	
	int		GetSubCharAttr()					{ return m_CharacterInfo.nSubAttr; }		// ºÎ¼º
	int		WhatIsAttrRelation( int nOtherMainAttr );		// ¼º°ü°è´Â?
#endif

	//////////////////////////////////////////////////////////////////////////
	// 06. 06. 2Â÷ ÀüÁ÷ - ÀÌ¿µÁØ
	// ¹«°ø º¯È¯ Ãß°¡
	void AddSkillStatsOption(SKILLOPTION *pSkillOption);
	void RemoveSkillStatsOption(SKILLOPTION *pSkillOption);
	//////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////
	// 06. 06. 2Â÷ ÀüÁ÷ - ÀÌ¿µÁØ
	// Àº½Å/Çý¾È
	BOOL IsHide() { return m_BaseObjectInfo.SingleSpecialState[eSingleSpecialState_Hide]; } 

	BOOL IsDetect() 
	{ 
		return (m_BaseObjectInfo.SingleSpecialState[eSingleSpecialState_Detect] ||
			m_BaseObjectInfo.SingleSpecialState[eSingleSpecialState_DetectItem]); 
	} 
	//////////////////////////////////////////////////////////////////////////

	// 06.09.25 RaMa
	void SetFullMoonEventIndex( BYTE Index )		{	m_CharacterInfo.EventIndex = Index; }
	BYTE GetFullMoonEventIndex()					{	return m_CharacterInfo.EventIndex;	}


	//SW070127 Å¸ÀÌÅº
	void SetTitanAppearanceInfo(TITAN_APPEARANCEINFO* pInfo);
	void SetTitanMoveSpeed();
	void RidingTitan(BOOL bVal)		{	m_bInTitan = bVal;	}
	BOOL InTitan()					{	return m_bInTitan;	}
	
	void SetTitanPreView( TITAN_APPEARANCEINFO* pInfo );
	void ReleaseTitanPreView();
	BOOL IsTitanPreView()			{	return m_bTitanPreView;	}
	TITAN_APPEARANCEINFO*	GetTitanPreViewInfo()		{	return &m_TitanPreViewAppInfo;	}
	

	void	SetCurTitan(CTitan* pTitan);
	CTitan* GetCurTitan()			{	return m_pTitan;	}

	TITAN_APPEARANCEINFO* GetTitanAppearInfo()		{	return	&m_TitanAppearanceInfo;	}
	float GetTitanMoveSpeed()		{	return m_TitanMoveSpeed;	}

	bool GetAvatarView()							{	return m_CharacterInfo.bNoAvatarView;	}
	void SetAvatarView( bool bNoAvatarView )		{	m_CharacterInfo.bNoAvatarView = bNoAvatarView;	}

	//2007. 8. 8. CBH - ¼¼Æ®¾ÆÀÌÅÆ ÀÌÆåÆ® °ü·Ã Ãß°¡
	DWORD GetSetItemEffectID();
	void SetSetItemEffectID(DWORD dwEffectID);

	// magi82(44) - ³ëÁ¡»ó ²Ù¹Ì±â ¾÷µ¥ÀÌÆ®
	CMapObject* GetDecorationAddress()	{	return m_DecorationAddress;	}
	void SetDecorationAddress(CMapObject* pMapObject)	{	m_DecorationAddress = pMapObject;	}
	DWORD GetDecorationStartTime()	{	return m_dwDecorationStartTime;	}
	BOOL GetDecorationFlag()	{	return m_bDecorationFlag;	}
	void SetDecorationInfo(DWORD dwTime, BOOL bFlag)	{	m_dwDecorationStartTime = dwTime;	m_bDecorationFlag = bFlag;	}

	//2008. 1. 21. CBH - ½ºÅ² ·¹ÀÌ °ü·Ã ÇÔ¼ö
	void InitSkinDelayTime();
	void StartSkinDelayTime();
	BOOL CheckSkinDelay();
	/////////////////////////////////////////////
	void	SetMarryName(char* pName);
	char*	GetMarryName();
	bool	IsMarry(){ return strcmp(GetMarryName(), "0") != 0; }


	bool	HasShiFu(){ return m_CharacterInfo.ShiTuId!= 0;}
	void	SetShiFuID(DWORD id){ m_CharacterInfo.ShiTuId = id;}
	DWORD	GetShiFuID()	{return m_CharacterInfo.ShiTuId; }
	void    SetShiFuName(char* pName);
	void    ClearShiFu(DWORD ShiFuId);
	char    * GetShiFuName();

	bool	IsImageName(){return m_CharacterInfo.ImageName!=0;}
	void	SetImageName(WORD val){m_CharacterInfo.ImageName = val;}
	WORD	GetImageName(){return m_CharacterInfo.ImageName;}

	//VIPÍ¼±ê

	void	SetVipLevel(WORD VipLevel);
	BOOL	IsVip() { return m_CharacterInfo.VipLevel != 0; }
	WORD	GetVipLevel() { return m_CharacterInfo.VipLevel; }

	virtual void SetFame(FAMETYPE val);
	FAMETYPE GetFame()			{ return m_CharacterInfo.Fame;	}

	virtual char*	TestFameLogoChangeNo(FAMETYPE pFame);

	//virtual void SetFameRank(char* fame);

	//weiye ³ÆºÅÍ¼Æ¬ÉèÖÃ Êý¾ÝÀ´×ÔÓÚAppearanceManager.cpp  2018-04-19

	//void    SetNameItem(DWORD ItemIdx){	m_NameImg=ItemIdx;}
	//DWORD   GetNameItem(){return m_NameImg;}


	//ÉÁÃû³ÆºÅº¯Êý¶¨Òå
	bool	IsFlgName() { return m_CharacterInfo.FlgName != 0; }
	void	SetFlgName(WORD val) { m_CharacterInfo.FlgName = val; }
	WORD	GetFlgName() { return m_CharacterInfo.FlgName; }
	//×Ô¶¨Òå³ÆºÅ
	void SetCustomizingName(char* pName);
	char* GetCustomizingName() { return m_CharacterInfo.CustomizingName; }
	bool   IsCustomizing() { return strcmp(GetCustomizingName(), "0") != 0; }

	BOOL    IsTurnOne(){return  m_CharacterInfo.TurnStageOne!=0;}
	void    SetTurnOne(int Flag){m_CharacterInfo.TurnStageOne=Flag;}
	WORD    GetTurnOne(){return m_CharacterInfo.TurnStageOne;}

	BOOL    IsTurnTwo(){return  m_CharacterInfo.TurnStageTwo!=0;}
	void    SetTurnTwo(int Flag){m_CharacterInfo.TurnStageTwo=Flag;}
	WORD    GetTurnTwo(){return m_CharacterInfo.TurnStageTwo;}

	BOOL    IsTurnThree(){return  m_CharacterInfo.TurnStageThree!=0;}
	void    SetTurnThree(int Flag){m_CharacterInfo.TurnStageThree=Flag;}
	WORD    GetTurnThree(){return m_CharacterInfo.TurnStageThree;}



	virtual void SetKillCount(DWORD val);
	DWORD GetKillCount()			{ return m_CharacterInfo.dwKillPlayerTimes; }
};