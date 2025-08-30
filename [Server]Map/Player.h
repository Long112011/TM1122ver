

// Player.h: interface for the CPlayer class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PLAYER_H__D36AF2E1_0FD9_4120_B257_64B0BE21D669__INCLUDED_)
#define AFX_PLAYER_H__D36AF2E1_0FD9_4120_B257_64B0BE21D669__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Object.h"

#include "ExchangeManager.h"

#include "OptionManager.h"
#include "ItemContainer.h"
#include "InventorySlot.h"
#include "PyogukSlot.h"
#include "WearSlot.h"
#include "ShopItemSlot.h"
#include "ShopInvenSlot.h"
#include "PetInvenSlot.h"
#include "PetWearSlot.h"
#include "TitanWearSlot.h"
#include "TitanShopItemSlot.h"
#include "Purse.h"
#include "AbilityGroup.h"
#include "../[CC]Skill/DelayGroup.h"
#include "ShopItemManager.h"

#include "PetManager.h"
#include "TitanManager.h"
#include "..\[CC]Header\GameResourceStruct.h"

// jsd quest
#include "..\[CC]Quest\QuestDefines.h"
#include "QuestGroup.h"


//
class cStreetStall;
class CMonster;
class CItemContainer;
class CItemSlot;
class CQuestBase;
class CQuestScript;
class CQuestGroup;
class CPet;
class CTitan;

enum
{
	PLAYERINITSTATE_ONLY_ADDED = 0,
	PLAYERINITSTATE_HERO_INFO = 1,
	PLAYERINITSTATE_MUGONG_INFO = 2,
	PLAYERINITSTATE_ITEM_INFO = 4,
	PLAYERINITSTATE_ITEM_OPTION_INFO = 8,
	PLAYERINITSTATE_ABILITY_INFO = 16,
	PLAYERINITSTATE_PYOGUK_INFO = 32,
	PLAYERINITSTATE_SHOPITEM_USEDINFO = 64,	
	PLAYERINITSTATE_ACKMSG_SEND = 128,
	PLAYERINITSTATE_INITED = 256,
	PLAYERINITSTATE_GMCONTROL_INFO = 512,
	PLAYERINITSTATE_ITEM_RARE_OPTION_INFO = 1024,
	PLAYERINITSTATE_PET_INFO = 2048,
	PLAYERINITSTATE_TITAN_INFO = 4096,
	PLAYERINITSTATE_ITEM_STONE_INFO =8192, // ÐÂÏµÍ³±êÖ¾ 2014-11-14
};

enum
{
	MONEY_ADDITION = 0,
	MONEY_SUBTRACTION,
};

// ¶Ä²©ÏµÍ³ÐÅÏ¢½á¹¹Ìå 2015-01-28

struct GAMBLE_USER_INFO
{
   DWORD CharacterIdx;
   int   Gamble_Year;
   int   Gamble_Month;
   int   Gamble_Day;
   int   Number1;
   int   Number2;
   int   Number3;
};

//SW051112 ¹«½Ö¸ðµå
#define MUSSANG_VALID_TIME		1000*60*10
enum eMussangStates{ eMSS_None=1, eMSS_Ready=2, eMSS_InMode=4, eMSS_LevelUp=8};

#define GETITEM_FLAG_INVENTORY	0x00000001
#define GETITEM_FLAG_WEAR		0x00000002
#define GETITEM_FLAG_PETWEAR	0x00000004
#define GETITEM_FLAG_TITANWEAR	0x00000008		// magi82 - Titan(070209)
#define GETITEM_FLAG_TITANSHOP	0x00000010		// magi82 - Titan(070227)
#define WANTED_DESTROYED_TIME 48*3600*1000
class CPlayer : public CObject  
{
private:
	//2007. 12. 10. CBH - ½ºÅ² µô·¹ÀÌ ½Ã°£ 10ÃÊ CPlayer¿¡¼­¸¸ ¾²±â?¹®¿¡ Å¬·¡½º ¾È¿¡ enumÀ¸·Î Àâ¾Ò´Ù.
    enum SKIN_DELAYTIME
	{
		eSkinDelayTime = 10000,
	};
/*
#ifdef _JAPAN_LOCAL_
	BOOL m_bPartyHelp;
#endif
*/	
	CDelayGroup m_DelayGroup;

	DWORD m_ChannelID;

	BOOL m_bPetInvenInit;

	// define last paodian time 2014-05-30
	int m_LastPaoDianTime;
	int m_LastMapTime;
	// ³É³¤ÏµÍ³¹ÖÎïÍ³¼Æ±êÖ¾ 2015-01-15

	int m_LastDungeonTime;

	WORD  m_EventIDFlag;
	int m_EventCountDownTimer;

	DWORD m_ItemGrowCount;

	// Í¶×¢ÐÅÏ¢  2015-01-28

	DWORD m_ItemFameCount;
	WORD  m_KillMonsterCountFame;
	BYTE  m_KillExpFlag;

	DWORD   m_KilledPlayerID;
	DWORD   m_LootingPos;


	GAMBLE_USER_INFO  m_GambleInfo;

	DWORD m_dwConnection;

	BOOL m_bGotWarehouseItems;
	BOOL m_bPyogukInit;
	BOOL m_bShopInit;
	DWORD m_UniqueIDinAgent;
	int m_initState;
	DWORD m_dwHpMpBitFlag;	// magi82 °æÇèÄ¡¸¶´Ù »óÅÂ È¸º¹ÇÏ´Â ÇÃ·¡±× 20% 40% 60% 80%
	CHARACTER_TOTALINFO m_HeroCharacterInfo;
	HERO_TOTALINFO		m_HeroInfo;
	MUGONG_TOTALINFO	m_HeroMugongInfo;
	
	CAbilityGroup		m_AbilityGroup;

	CItemContainer		m_ItemContainer;
	CInventorySlot		m_InventorySlot;
	CWearSlot			m_WearSlot;
	CPyogukSlot			m_PyogukSlot;
	CShopItemSlot		m_ShopItemSlot;			// ItemMall¿ë	
	CShopInvenSlot		m_ShopInvenSlot;
	CPetInvenSlot		m_PetInvenSlot;			// Pet Inventory
	CPetWearSlot		m_PetWearSlot;			// Pet Wear
	CTitanWearSlot		m_TitanWearSlot;		// magi82 - Titan(070207)
	CTitanShopItemSlot	m_TitanShopItemSlot;	// magi82 - Titan(070227)
	CInventoryPurse		m_InventoryPurse;
	CPyogukPurse		m_PyogukPurse;



	char m_StreetStallTitle[MAX_STREETSTALL_TITLELEN+1];

	player_calc_stats m_charStats;
	player_calc_stats m_itemStats;
	SET_ITEM_OPTION m_setItemStats;		// 2007. 6. 12. CBH - ¼¼Æ®¾ÆÀÌÅÆ ´É·ÂÄ¡ ±¸Á¶Ã¼ Ãß°¡
	UNIQUE_ITEM_OPTION_INFO m_UniqueItemStats;	// magi82 - UniqueItem(070626)
	ABILITY_STATS m_AbilityStats;
	SET_ITEMQUALITY_OPTION m_setItemQualityStats;
	// LIFE RECOVER´Â COBJECT·Î
	RECOVER_TIME m_NaeRyukRecoverTime;
	YYRECOVER_TIME m_YYNaeRyukRecoverTime;

	WORD m_ChangeBeforeMapNum;
	WORD m_ChangeWantMapNum;

	DWORD m_CurComboNum;

	WORD m_ChangeNpcIndex;

	BYTE m_curGravity;

// KES HACK CHECK
	int	m_nHackCount;			//skill + move
	DWORD m_dwHackStartTime;
	
	DWORD	m_dwSkillDelayStartTime;		// speedhack check;
	DWORD	m_dwLastSkillAnimationTime;

// KES 050221
	DWORD m_dwLastSocietyAction;

// KES EXCHNAGE 040112
//	CExchangeRoom*		m_pExchangeRoom;
	sEXCHANGECONTAINER	m_ExchangeContainer;
// KES VIMU 031101
	//º£Æ²ÁßÀÎ°¡?
	BOOL				m_bVimuing;
	DWORD				m_dwOpPlayerID;

// KES OPTION 031213
	sGAMEOPTION			m_GameOption;
// KES PK
	DWORD				m_dwPKModeStartTime;	//with gCurTime
	DWORD				m_dwPKContinueTime;
// KES BADFAME
	DWORD				m_dwLastReduceHour;		//with PlayTime
	void				AddBadFameReduceTime();
// KES EXIT
	BOOL				m_bExit;	//Á¾·áÇÃ·¡±×
	DWORD				m_dwExitStartTime;
	BOOL				m_bNormalExit;
	BOOL				m_bWaitExitPlayer;

	DWORD				m_bTidyTime;
	DWORD				m_bTidyShopTime;

	DWORD				m_bLuckerServerTimingLock;

//
	DWORD				m_dwUserLevel;
///////////	

// LBS ³ëÁ¡»ó °ü·Ã 03.09.29/////
	cStreetStall* m_pGuetStall;
////////////////////////////////

///////////////////////////////////////////
// LBS ½Ã°£º° Log °ü·Ã 04.02.06
//	DWORD				m_dwStartTime;
	DWORD				m_dwProgressTime;
//	MHTIMEMGR_OBJ->GetMHTime()
///////////////////////////////////////////
	
	WANTEDTYPE			m_WantedIDX;
	cPtrList			m_WantedList;

	WORD				m_MurdererKind;
	DWORD				m_MurdererIDX;
	BOOL				m_bReadyToRevive; //Á×Àº »óÅÂ¿¡¼­¸¸ ¾¸

	//SW060831 Ãß°¡. Ä³¸¯ÅÍ¸¦ Á×ÀÎ ´ë»óÀÇ ObjectKind
	WORD				m_MurderObjKind;
	BOOL				m_bPenaltyByDie;
	//
	DWORD				m_LoginTime;
	DWORD				m_ContinuePlayTime;	
	
	// RaMa - 04.11.08 ( ShopItemÀ¸·Î Ãß°¡µÇ´Â ¿É¼Ç )
	SHOPITEMOPTION		m_ShopItemOption;
	AVATARITEMOPTION	m_AvatarOption;
	CShopItemManager	m_ShopItemManager;

	//////////////////////////////////////////////////////////////////////////
	// 06. 06. 2Â÷ ÀüÁ÷ - ÀÌ¿µÁØ
	// ¹«°ø º¯È¯ Ãß°¡
	SKILLSTATSOPTION	m_SkillStatsOption;
	//////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////
	// 06. 07. ½ºÅ³°³¼± - ÀÌ¿µÁØ
	// ÇÃ·¹ÀÌ¾î°¡ ÀÚ±â¿¡°Ô ¿µÇâÀ» ¹ÌÄ¡´Â ½ºÅ³ÀÇ ¸®½ºÆ®¸¦ °®°ÔÇÑ´Ù.
	CYHHashTable<CSkillObject> m_SkillList;
	//////////////////////////////////////////////////////////////////////////

	//SW051129 Pet
	CPetManager			m_PetManager;
//	CPet*				m_pCurPet;		//JACK

	//SW070127 Å¸ÀÌÅº
	CTitanManager		m_TitanManager;

	DWORD				m_InitPosition;
	REVIVEDATA			m_ReviveData;
	int					m_ReviveTime;
	BOOL				m_bSavePointInited;

	static ITEM_RARE_OPTION_INFO m_NULLRAREINFO;
	static ITEM_OPTION_INFO NULLOPTIONINFO;
	static ITEM_STONE_OPTION_INFO NULLSTONEINFO;

	//SW051112 ¹«½Ö¸ðµå
	DWORD				m_dwMussangPoint;
	DWORD				m_dwMussangStartTime;
	BOOL				m_bIsMussangReady;
	BYTE				m_bMussangFlag;

	DWORD	m_dwWantMarryID;
	CHAR	m_szWantMarryName[MAX_NAME_LENGTH + 1];

	////////////////////////////////////////////////////////////////////////////
	// 06. 07. »óÅÂ°­Á¦º¯°æ - ÀÌ¿µÁØ
	// ÀÏÁ¤ÀÌ»ó ½ºÅ³ »ç¿ë½ÇÆÐ½Ã Æ¯º°ÇÑ »óÅÂ¸¦ Á¦¿ÜÇÑ ³ª¸ÓÁö »óÅÂ´Â ¸ðµÎ ÃÊ±âÈ­
	WORD	m_SkillFailCount;
	////////////////////////////////////////////////////////////////////////////


	// 06.09.25 RaMa Ãß¼®ÀÌº¥Æ®
	int					m_FullMoonEventHCt;
	float				m_fFullMoonEventTime;

	DWORD				m_LastActionTime;
	BOOL				m_bActionPanelty;


	//SW061211 Å©¸®½º¸¶½ºÀÌº¥Æ®
	DWORD				m_CristmasEventSnowIdx;

	///////////////////////////////////////////////////////////////////////////////////
	DWORD m_dwSkinDelayTime;	//2007. 12. 10. CBH - ½ºÅ²±³Ã¼ µô·¹ÀÌ ½Ã°£
	BOOL m_bSkinDelayResult;	//2007. 12. 10. CBH - ½ºÅ²±³Ã¼ µô·¹ÀÌ ±¸ºÐ	

	DWORD				pGuildWarDieTime;

	DWORD				pDungeonFinishTime;

	BOOL			pDungeonFinishSwitch;

	//weiye °²È«Ëø×´Ì¬±äÁ¿ 2017-10-23

	BOOL    m_IsUnLock;

	//weiye FB¸±±¾Ïà¹Ø±äÁ¿¶¨Òå 2016-10-07

	DWORD m_FBTime;

	void SkinDelayTimeProcess();	//2007. 12. 10. CBH - ½ºÅ² µô·¹ÀÌ ½Ã°£ Ã³¸®
	///////////////////////////////////////////////////////////////////////////////////
    
	void DoSetInited();
	virtual void SetInitedGrid();

	friend class CCharacterCalcManager;
	friend class CInventoryPurse;
	friend class CAbilityGroup;
	
	void SetMurderIDX(DWORD CharacterIDX)			{ m_MurdererIDX = CharacterIDX; }
	void SetMurderKind(WORD Kind)					{ m_MurdererKind = Kind; }

	// magi82 - SOS(070720)
	int					m_SOSState;		// 0ÀÌ¸é SOS »óÅÂ ¾Æ´Ô, 0ÀÌ ¾Æ´Ï¸é SOS »óÅÂ


	// 06. 03 ±¹³»¹«½Ö - ÀÌ¿µÁØ
#ifndef _JAPAN_LOCAL_
	DWORD	m_dwMussangTime;
	WORD	m_wMussangStat;
#endif

#ifdef _JAPAN_LOCAL_
	DWORD	m_dwCharacterSlotNum;
#endif
#ifdef _HK_LOCAL_
	DWORD	m_dwCharacterSlotNum;
#endif
#ifdef _TL_LOCAL_
	DWORD	m_dwCharacterSlotNum;
#endif

public:
/*
#ifdef _JAPAN_LOCAL_
	void SetPartyHelp( BOOL bPartyHelp ) { m_bPartyHelp = bPartyHelp; }
	BOOL IsPartyHelp() { return m_bPartyHelp; }
#endif
*/
	DWORD GetGuildWarDieTime(){return pGuildWarDieTime;}
	void SetGuildWarDieTime(DWORD pTime){pGuildWarDieTime = pTime; }

	void	SetMurderObjKind(WORD ObjKind)		{	m_MurderObjKind = ObjKind;	}
	WORD	GetMurderObjKind()					{	return m_MurderObjKind;	}

	void	SetPenaltyByDie(BOOL bVal)		{	m_bPenaltyByDie = bVal;	}	//¹æÆÄ ºñ¹« ½Ã »ó´ë¿¡ ÀÇÇØ Á×¾úÀ» ½Ã TRUE //°­Á¦Á¾·á ½Ã ÆÐ³ÎÆ¼ Ã¼Å©¸¦ À§ÇÑ º¯¼ö
	BOOL	IsPenaltyByDie()				{	return m_bPenaltyByDie;	}

	BOOL IsGotWarehouseItems() { return m_bGotWarehouseItems; }
	void SetGotWarehouseItems( BOOL bGot ) { m_bGotWarehouseItems = bGot; }

	void SetWaitExitPlayer( BOOL bWait ) { m_bWaitExitPlayer = bWait; }
	BOOL IsWaitExitPlayer() { return m_bWaitExitPlayer; }

	void SetChannelID(DWORD id)		{	m_ChannelID = id;	}
	DWORD GetChannelID()			{	return m_ChannelID;	}
// KES
	BOOL				m_bNeedRevive;
//
	DWORD GetSkillDelayStartTime()				{ return m_dwSkillDelayStartTime; }
	void SetSkillDelayStartTime( DWORD dwTime )	{m_dwSkillDelayStartTime = dwTime; }

	DWORD GetLastSkillAnimationTime()			{ return m_dwLastSkillAnimationTime; }
	void SetLastSkillAnimationTime( DWORD dwTime ) { m_dwLastSkillAnimationTime = dwTime; }
	
	BOOL SpeedHackCheck(WORD wTime= 0);//void SpeedHackCheck();
	BOOL CanSocietyAction( DWORD curTime );
	
	int GetGender() { return m_HeroCharacterInfo.Gender; }
//
	HERO_TOTALINFO* GetHeroTotalInfo();

	ABILITY_STATS*	GetAbilityStats()	{	return &m_AbilityStats;	}
	CAbilityGroup*	GetAbilityGroup()	{	return &m_AbilityGroup;	}
	CDelayGroup*	GetDelayGroup()		{	return &m_DelayGroup;	}

	CItemSlot * GetSlot(POSTYPE absPos);
	CItemSlot * GetSlot(eITEMTABLE tableIdx);

	//CItemContainer * GetContainer() { return m_pItemContainer; } 
	void SetUniqueIDinAgent(DWORD uniqueid)	{	m_UniqueIDinAgent = uniqueid;	}
	DWORD GetUniqueIDinAgent()	{	return m_UniqueIDinAgent;	}
	
	void InitBaseObjectInfo(BASEOBJECT_INFO* pBaseInfo);

	BOOL IsVisible()
	{
		return (m_HeroCharacterInfo.bVisible ? TRUE : FALSE);
	}
	void SetVisible(BOOL val)
	{
		m_HeroCharacterInfo.bVisible = val ? true : false;
	}
	
	// Gravity --------------------------------------------------------------
	virtual DWORD GetGravity();
	virtual void SetGravity(DWORD gravity)
	{
	}
	// Gravity --------------------------------------------------------------

	CPlayer();
	virtual ~CPlayer();
	void InitClearData();

	virtual BOOL Init(EObjectKind kind,DWORD AgentNum, BASEOBJECT_INFO* pBaseObjectInfo);
	virtual void Release();

	// ÃÊ±âÈ­ 
	void InitCharacterTotalInfo(CHARACTER_TOTALINFO* pTotalInfo);
	void InitHeroTotalInfo(HERO_TOTALINFO* pHeroInfo);
	void InitMugongTotalInfo(MUGONG_TOTALINFO* pMugongInfo);
	void InitItemTotalInfo(ITEM_TOTALINFO* pItemInfo);
	void InitPyogukItemInfo(PYOGUK_ITEM* pItemInfo);

	// Æê¾ÆÀÌÅÛ °ü·Ã
	void InitPetInvenItemInfo(PETINVEN_ITEM* pItemInfo);
	void SendPetInvenItemInfo();
	void InitPetWearItemInfo(ITEMBASE* pWearItemInfo);

	int GetInitState(){ return m_initState; }
	void SetInitState(int initstate,DWORD protocol);

	// ÆÄÆ¼ °ü·Ã ÇÔ¼ö	
	void SendPlayerLifeToParty(DWORD life, DWORD PartyIdx);
	void SendPlayerShieldToParty(DWORD Shield, DWORD PartyIdx);
	void SendPlayerNaeRyukToParty(DWORD naeryuk, DWORD PartyIdx);
	void SendMoveInfotoParty();

	// Ä³¸¯ÅÍ ¼öÄ¡ ¼³Á¤ //////////////////////////////////////////////////////////
	LEVELTYPE GetPlayerLevelUpPoint() { return m_HeroInfo.LevelUpPoint;	}
	void SetPlayerLevelUpPoint(LEVELTYPE val);
	EXPTYPE GetPlayerExpPoint() { return m_HeroInfo.ExpPoint; }
	DOUBLE GetPlayerTotalExpPoint();
	void SetPlayerExpPoint(EXPTYPE point,BYTE bDie=0);
	void SetLevelUpStatPoint();			// magi82
	void SetNormalStatPoint();			// magi82
	BYTE GetExpFlag()	{	return m_HeroInfo.ExpFlag;	}	// magi82(41)
	void AddPlayerExpPoint(EXPTYPE Exp);
	void ReduceExpPoint(EXPTYPE minusExp, BYTE bLogType);
	DWORD GetPlayerAbilityExpPoint() { return m_AbilityGroup.GetAbilityExp(); }
	void SetPlayerAbilityExpPoint(DWORD point,BOOL bUse=0);
	void AddAbilityExp(DWORD AbilityExp);

	DWORD AddExpPointToMugong(WORD MugongIdx,DWORD exp,BOOL bSuryunComplete = FALSE);
	void SetPlayerMugongSung(POSTYPE abspos, BYTE sung, WORD Reason);
	void MugongLevelDown(WORD MugongIdx, WORD Reason);
	
	FAMETYPE GetFame()			{ return m_HeroInfo.Fame;	}
	void SetFame(FAMETYPE val)	{ m_HeroInfo.Fame = val; 	}

	FAMETYPE GetBadFame()			{ return m_HeroCharacterInfo.BadFame;	}
	void SetBadFame(FAMETYPE val)	{ m_HeroCharacterInfo.BadFame = val; 	}

	// RaMa - 04.11.10    -> ShopItemOption Ãß°¡  AvatarOptionÃß°¡(05.02.16)
	// magi82 - UniqueItem(070629) - ½ºÅÝÀÌ ¸¶ÀÌ³Ê½º°¡ µÉ °æ¿ì ¿¹¿ÜÃ³¸® Ãß°¡
	WORD GetCheRyuk(){ return (int)(m_HeroInfo.wCheRyuk + GetAbilityStats()->StatChe + GetItemStats()->CheRyuk + GetShopItemStats()->Cheryuk
		+ GetAvatarOption()->Cheryuk + GetSetItemStats()->wCheRyuk + GetUniqueItemStats()->nCheRyuk) + GetSetItemQualityStats()->wCheRyuk >= 0 ? (WORD)(m_HeroInfo.wCheRyuk
		+ GetAbilityStats()->StatChe + GetItemStats()->CheRyuk + GetShopItemStats()->Cheryuk + GetAvatarOption()->Cheryuk + GetSetItemStats()->wCheRyuk
		+ GetUniqueItemStats()->nCheRyuk + GetSetItemQualityStats()->wCheRyuk) : 0; }

	WORD GetSimMek(){ return (int)(m_HeroInfo.wSimMek + GetAbilityStats()->StatSim + GetItemStats()->SimMaek + GetShopItemStats()->Simmek
		+ GetAvatarOption()->Simmek + GetSetItemStats()->wSimMek + GetUniqueItemStats()->nSimMek + GetSetItemQualityStats()->wSimMek) >= 0 ? (WORD)(m_HeroInfo.wSimMek
		+ GetAbilityStats()->StatSim + GetItemStats()->SimMaek + GetShopItemStats()->Simmek + GetAvatarOption()->Simmek + GetSetItemStats()->wSimMek
		+ GetUniqueItemStats()->nSimMek + GetSetItemQualityStats()->wSimMek) : 0; }

	WORD GetGenGol(){ return (int)(m_HeroInfo.wGenGol + GetAbilityStats()->StatGen + GetItemStats()->GenGol + GetShopItemStats()->Gengol
		+ GetAvatarOption()->Gengol + GetSetItemStats()->wGenGol + GetUniqueItemStats()->nGengol + GetSetItemQualityStats()->wGenGol) >= 0 ? (WORD)(m_HeroInfo.wGenGol
		+ GetAbilityStats()->StatGen + GetItemStats()->GenGol + GetShopItemStats()->Gengol + GetAvatarOption()->Gengol + GetSetItemStats()->wGenGol
		+ GetUniqueItemStats()->nGengol + GetSetItemQualityStats()->wGenGol) : 0; }

	WORD GetMinChub(){ return (int)(m_HeroInfo.wMinChub + GetAbilityStats()->StatMin + GetItemStats()->MinChub + GetShopItemStats()->Minchub
		+ GetAvatarOption()->Minchub + GetSetItemStats()->wMinChub + GetUniqueItemStats()->nMinChub + GetSetItemQualityStats()->wMinChub) >= 0 ? (WORD)(m_HeroInfo.wMinChub
		+ GetAbilityStats()->StatMin + GetItemStats()->MinChub + GetShopItemStats()->Minchub + GetAvatarOption()->Minchub + GetSetItemStats()->wMinChub
		+ GetUniqueItemStats()->nMinChub + GetSetItemQualityStats()->wMinChub) : 0; }

	void SetCheRyuk(WORD val);
	void SetSimMek(WORD val);
	void SetGenGol(WORD val);
	void SetMinChub(WORD val);

	void AddGenGol(int val);
	void AddSimMek(int val);
	void AddMinChub(int val);
	void AddCheRyuk(int val);
	
	void SetStage( BYTE stage );
	BYTE GetStage()		{ return m_HeroCharacterInfo.Stage; }

	MARKNAMETYPE GetGuildMarkName() {return m_HeroCharacterInfo.MarkName;}
	
	WORD GetPlayerLoginPoint(){return m_HeroCharacterInfo.LoginPoint_Index;}
	void SetLoginPoint(WORD LoginPoint){m_HeroCharacterInfo.LoginPoint_Index = LoginPoint;}
	void InitMapChangePoint(){m_HeroCharacterInfo.MapChangePoint_Index = 0;}
	void SetMapChangePoint(WORD point){m_HeroCharacterInfo.MapChangePoint_Index = point;}	// YH
	WORD GetMapChangePoint() { return m_HeroCharacterInfo.MapChangePoint_Index;	} //hs for party
	
	void CalcState();
	
	///////////////////////////////////////////////////////////////////////
	// ¹«°ø	
	void SetMugongBase(POSTYPE abspos, MUGONGBASE * mugong)
	{
#ifdef _JAPAN_LOCAL_
		if( abspos < TP_MUGONG_START && abspos >= TP_MUGONG_END )
 			ASSERT(0);
		POSTYPE pos = abspos - TP_MUGONG_START;
#elif defined _HK_LOCAL_
		if( abspos < TP_MUGONG_START && abspos >= TP_MUGONG_END )
 			ASSERT(0);

		POSTYPE pos = abspos - TP_MUGONG_START;
#elif defined _TL_LOCAL_
		if( abspos < TP_MUGONG_START && abspos >= TP_MUGONG_END )
 			ASSERT(0);


		POSTYPE pos = abspos - TP_MUGONG_START;
#else
		if( abspos < TP_MUGONG1_START && abspos >= TP_MUGONG2_END )
		{
			if( abspos < TP_JINBUB_START && abspos >= TP_JINBUB_END )
				ASSERT(0);
		}

		DWORD EndPos = TP_MUGONG1_END;
		if( m_ShopItemManager.GetUsingItemInfo( eIncantation_MugongExtend ) )
			EndPos = TP_MUGONG2_END;

		POSTYPE pos;
		if( abspos >= TP_MUGONG1_START && abspos < TP_TITANMUGONG_END )
			pos = abspos - TP_MUGONG1_START;
		/*
		else if( abspos >= TP_JINBUB_START && abspos < TP_JINBUB_END )
			pos = abspos - TP_JINBUB_START + (SLOT_MUGONG_NUM*2);
			*/
		else
		{
			ASSERT(0);
			return;
		}
#endif
		if( mugong->dwDBIdx != 0 && mugong->Position == 0)
			CRITICALASSERT(0);

		// magi82 - Titan(070611) Å¸ÀÌÅº ¹«°øº¯È¯ ÁÖ¼®Ã³¸®
		// ½ºÅ³ÀÎµ¦½º°¡ 10000º¸´Ù Å©¸é Å¸ÀÌÅº ½ºÅ³
		//if(mugong->wIconIdx < 10000)
            memcpy(&m_HeroMugongInfo.mugong[pos], mugong, sizeof(MUGONGBASE));
		//else
		//	memcpy(&m_HeroMugongInfo.Titanmugong[pos], mugong, sizeof(MUGONGBASE));
	}
	MUGONGBASE * GetMugongBase(POSTYPE abspos)
	{
#ifdef _JAPAN_LOCAL_
		if( abspos < TP_MUGONG_START && abspos >= TP_MUGONG_END )
   		{	
			ASSERT(0);
 			return NULL;
   		}

		POSTYPE pos = abspos - TP_MUGONG_START;

#elif defined _HK_LOCAL_
		if( abspos < TP_MUGONG_START && abspos >= TP_MUGONG_END )
   		{	
			ASSERT(0);
 			return NULL;
   		}
		POSTYPE pos = abspos - TP_MUGONG_START;
#elif defined _TL_LOCAL_
		if( abspos < TP_MUGONG_START && abspos >= TP_MUGONG_END )
   		{	
			ASSERT(0);
 			return NULL;
   		}
		POSTYPE pos = abspos - TP_MUGONG_START;
#else
		if( abspos < TP_MUGONG1_START && abspos >= TP_MUGONG2_END )
		{	
			if( abspos < TP_JINBUB_START && abspos >= TP_JINBUB_END )
			{
				ASSERT(0);
				return NULL;
			}
		}

		DWORD EndPos = TP_MUGONG1_END;
		if( m_ShopItemManager.GetUsingItemInfo( eIncantation_MugongExtend ) )
			EndPos = TP_MUGONG2_END;


		POSTYPE pos;
		if( abspos >= TP_MUGONG1_START && abspos < TP_TITANMUGONG_END )
			pos = abspos - TP_MUGONG1_START;
		/*
		else if( abspos >= TP_JINBUB_START && abspos < TP_JINBUB_END )
			pos = abspos - TP_MUGONG1_START;	// TP_JINBUB_START + (SLOT_MUGONG_NUM*2);
		else if( abspos >= TP_TITANMUGONG_START && abspos < TP_TITANMUGONG_END )	
			pos = abspos - TP_TITANMUGONG_START;
		*/
		else
			return NULL;
#endif
		return &m_HeroMugongInfo.mugong[pos];
	}	
	MUGONGBASE * GetMugongBaseByMugongIdx(WORD MugongIdx);
	int GetMugongLevel(WORD MugongIdx);
	int GetMugongNum();
	//////////////////////////////////////////////////////////////////////////
	// 06. 06. 2Â÷ ÀüÁ÷ - ÀÌ¿µÁØ
	// ¹«°ø º¯È¯ Ãß°¡
	WORD GetSkillOptionIndex(WORD MugongIdx);
	//////////////////////////////////////////////////////////////////////////
	
	///////////////////////////////////////////////////////////////////////

	
	void GetCharacterTotalInfo(CHARACTER_TOTALINFO* pRtInfo);
	void GetHeroTotalInfo(HERO_TOTALINFO* pRtInfo);
	void GetMugongTotalInfo(MUGONG_TOTALINFO* pRtInfo);

	// ¾ÆÀÌÅÛ //////////////////////////////////////////////////////////////////////
	void GetItemtotalInfo(ITEM_TOTALINFO* pRtInfo,DWORD dwFlag);
	void GetPyogukItemInfo(PYOGUK_ITEM * pRtInfo);

	WORD GetWearedItemIdx(DWORD WearedPosition)	{	return m_HeroCharacterInfo.WearedItemIdx[WearedPosition];	}
	void SetWearedItemIdx(DWORD WearedPosition,WORD ItemIdx);
	BOOL UseItem(WORD abs_pos);
	BOOL UseConsumptionItem(WORD abs_pos);
	BOOL UseEquipItem(WORD abs_pos);
	BOOL UseMugongItem(WORD abs_pos);

	void SetAddMsg(char* pAddMsg,WORD* pMsgLen,DWORD dwReceiverID,BOOL bLogin);
	
	virtual void SendMsg(MSGBASE* pMsg,int MsgLen);
	
private:
	void MoneyUpdate( MONEYTYPE money ); //(Money Class¿¡¼­ È£Ãâ ), DB¿¡ Set
	CYHHashTable<ITEM_RARE_OPTION_INFO> m_ItemRareOptionList;
	CYHHashTable<ITEM_OPTION_INFO> m_ItemOptionList;
	CYHHashTable<ITEM_STONE_OPTION_INFO> m_ItemStoneOptionList;   // ÐÂ ÄÚ´æ·þÎñ¶ËÁ´±í 2014-11-13
public:
	//SW050920 Rare
	void AddItemRareOption(ITEM_RARE_OPTION_INFO* pInfo);
	BOOL RemoveItemRareOption(DWORD dwRareIdx);
	ITEM_RARE_OPTION_INFO* GetItemRareOption(DWORD dwRareIdx);

	void AddItemOption(ITEM_OPTION_INFO * pInfo);
	BOOL RemoveItemOption(DWORD dwOptionIdx);
	ITEM_OPTION_INFO * GetItemOption(DWORD dwOptionIdx);

	// ÐÂ ÐÅÏ¢µ½½ÇÉ«·ÃÎÊ! 2014-11-14

	void AddItemStoneOption(ITEM_STONE_OPTION_INFO * pInfo);
    BOOL RemoveItemStoneOption(DWORD dwStoneIdx);
	ITEM_STONE_OPTION_INFO * GetItemStoneOption(DWORD dwStoneIdx);

	MONEYTYPE SetMoney( MONEYTYPE ChangeValue, BYTE bOper, BYTE MsgFlag = 0, eITEMTABLE tableIdx = eItemTable_Inventory, BYTE LogType = 0, DWORD TargetIdx = 0 );
	MONEYTYPE GetMoney(eITEMTABLE tableIdx = eItemTable_Inventory);

	// 2014-05-04 MallMonery  GoldMoney Fame Vistor!

	MONEYTYPE GetMallMoney(){return  m_HeroInfo.MallMoney;}
	void SetMallMoney(MONEYTYPE ChangeValue,BYTE nFlag,DWORD ItemIdx=0);
	void    UpdateMallMoney(DWORD DBMallMoney, DWORD ChangeMoney, BOOL bIsSend = FALSE);

	MONEYTYPE GetGoldMoney(){return m_HeroInfo.GoldMoney;}
	void SetGoldMoney(MONEYTYPE ChangeValue, BYTE nFlag, WORD ItemIdx = 0, WORD ItemDurability = 0);
	void	GetDBGoldMoney();//ÔÚÏß³äÖµÔª±¦Ë¢ÐÂ	by:ºúººÈý	QQ:112582793
	//void	UpdateGoldMoney(DWORD DBGoldMoney,DWORD ChangeMoney);
	void	UpdateGoldMoney(DWORD DBGoldMoney, DWORD ChangeMoney, BOOL bIsSend = FALSE);//ÔÚÏß³äÖµÔª±¦Ë¢ÐÂ



	MONEYTYPE GetSwMoney(){return m_HeroInfo.Fame;}

	void SetSwMoney(MONEYTYPE ChangeValue,BYTE nFlag,DWORD ItemIdx=0);

	// 2014-05-05 ChangeLife Marry Baishi Vistor !

	WORD GetReSet(){return m_HeroInfo.ReSet;}

	void SetReSet(DWORD Val,WORD Type);

	void SetReSetN(DWORD Val,WORD Type);

	// 2014-05-09 Marry set Name msg;

	void SetMarryName(char* pName);
	char* GetMarryName();
	bool  IsMarry();
	void SetWantMarryID(DWORD id){ m_dwWantMarryID = id; }
	DWORD GetWantMarryID(){ return m_dwWantMarryID; }
	void SetWantMarryName(char* name){ SafeStrCpy(m_szWantMarryName, name, MAX_NAME_LENGTH + 1); }
	char* GetWantMarryName(){ return m_szWantMarryName; }

	// 2014-05-12 ShiTu Name msg Set;
	bool	HasShiFu(){ return m_HeroCharacterInfo.ShiTuId != 0;}
	void    SetShiFu(CPlayer* pShiFu);
	void    ClearShiFu(DWORD ShiFuId);
	void	SetShiFuID(DWORD id){ m_HeroCharacterInfo.ShiTuId = id;}
	DWORD	GetShiFuID()	{return m_HeroCharacterInfo.ShiTuId; }

	// 2015-01-15 ³É³¤ÏµÍ³º¯Êý

	void    AddItemGrowKillMonster(CMonster * Monster);

	void    CountBeforeFame(CMonster * Monster);

	MONEYTYPE GetMaxPurseMoney(eITEMTABLE TableIdx);
	void SetMaxPurseMoney(eITEMTABLE TableIdx, MONEYTYPE MaxMoney);
	BOOL IsEnoughAdditionMoney(MONEYTYPE money, eITEMTABLE tableIdx = eItemTable_Inventory );

//	void ObtainMoney(DWORD addmoney);		//¸÷Á×¿©¼­ ¾ò¾úÀ»¶§¸¸ È£Ãâ
//	void SetMoneyNoMsgNoUpdate(DWORD money);
	
	player_calc_stats*	GetCharStats()			{	return &m_charStats;		}
	player_calc_stats*	GetItemStats()			{	return &m_itemStats;		}
	SET_ITEM_OPTION* GetSetItemStats()		{	return &m_setItemStats;		}	// 2007. 6. 12. CBH - ¼¼Æ®¾ÆÀÌÅÆ ´É·ÂÄ¡ ¹ÝÈ¯ ÇÔ¼ö Ãß°¡
	UNIQUE_ITEM_OPTION_INFO* GetUniqueItemStats(){	return &m_UniqueItemStats;	}	// magi82 - UniqueItem(070626)
	SHOPITEMOPTION*		GetShopItemStats()		{	return &m_ShopItemOption;	}
	AVATARITEMOPTION*	GetAvatarOption()		{	return &m_AvatarOption;		}
	SET_ITEMQUALITY_OPTION* GetSetItemQualityStats() { return &m_setItemQualityStats; }

	//////////////////////////////////////////////////////////////////////////
	// 06. 06. 2Â÷ ÀüÁ÷ - ÀÌ¿µÁØ
	// ¹«°ø º¯È¯ Ãß°¡
	SKILLSTATSOPTION*	GetSkillStatsOption()		{	return &m_SkillStatsOption;	}
	//////////////////////////////////////////////////////////////////////////
	
//	void AddHuntItemAndSendToPlayer(WORD DropItemNum);
//	BOOL HuntItemDBResult(DWORD dwPlayerID, ITEMBASE * itemInfo);
	DWORD GetPartyIdx()	{	return m_HeroInfo.PartyID;	}
	void SetPartyIdx(DWORD PartyIDx) { m_HeroInfo.PartyID = PartyIDx; }

	/*¹®ÆÄ°¡ ¹Ù²î³×...*/
	DWORD GetMunpaIdx()	{	return m_HeroCharacterInfo.MunpaID;	}
	void SetMunpaIdx(DWORD MunpaIDx) { m_HeroCharacterInfo.MunpaID = MunpaIDx; }
	
	BYTE GetMunpaMemberRank() { return m_HeroCharacterInfo.PositionInMunpa; 	}
	void SetMunpaMemberRank(BYTE rank){ m_HeroCharacterInfo.PositionInMunpa = rank; 	} //¹®ÆÄ¿¡¼­ÀÇ ÀÚ½ÅÀÇ ÁöÀ§ ¼³Á¤
	void SetMunpaName(char* MunpaName){ SafeStrCpy(m_HeroCharacterInfo.GuildName, MunpaName, MAX_MUNPA_NAME+1); }
	/**/

	DWORD GetGuildIdx()	{	return m_HeroCharacterInfo.MunpaID;	}
	void SetGuildIdx(DWORD GuildIdx) { m_HeroCharacterInfo.MunpaID = GuildIdx; }

	void SetGuildInfo(DWORD GuildIdx, BYTE Grade, char* GuildName, MARKNAMETYPE MarkName);
	BYTE GetGuildMemberRank() { return m_HeroCharacterInfo.PositionInMunpa; 	}
	void SetGuildMemberRank(BYTE rank){ m_HeroCharacterInfo.PositionInMunpa = rank; 	} //¹®ÆÄ¿¡¼­ÀÇ ÀÚ½ÅÀÇ ÁöÀ§ ¼³Á¤
	void SetGuildName(char* GuildName){ SafeStrCpy(m_HeroCharacterInfo.GuildName, GuildName, MAX_MUNPA_NAME+1); }
	void SetGuildMarkName(MARKNAMETYPE MarkName);
	char* GetGuildCanEntryDate();
	void SetGuildCanEntryDate(char* date);
	void ClrGuildCanEntryDate();
	void SetNickName(char* NickName);

	BYTE GetLifePercent();
	BYTE GetShieldPercent();
	BYTE GetNaeRyukPercent();
	
	// »óÅÂ º¯È¯
	void OnStartObjectState(BYTE State,DWORD dwParam);
	void OnEndObjectState(BYTE State);

	WORD GetWearedWeapon() { return m_HeroCharacterInfo.WearedItemIdx[eWearedItem_Weapon]; }
	WORD GetWeaponEquipType();
	WORD GetTitanWeaponEquipType();

	void SetCurComboNum(DWORD combonum)	{	m_CurComboNum = combonum;	}
	DWORD GetCurComboNum()				{	return m_CurComboNum;	}
//	DWORD GetCurMugongNum()				{ return GetCurQuickItem()->dwIconIdx; }

	void SetPeaceMode(BOOL bPeace)	{	m_HeroCharacterInfo.bPeace = bPeace?true:false;	}
	BOOL GetPeaceMode()				{	return m_HeroCharacterInfo.bPeace;		}
	
	//Ç¥±¹
	void AddPyogukItem(ITEMBASE * pPyogukItem);
	void InitPyogukInfo(BYTE pyoguknum,MONEYTYPE money);
	BOOL IsPyogukInit() {return m_bPyogukInit; }
	void SetPyogukNum(BYTE n);
	BYTE GetPyogukNum();

	void MoveToLogInPosition();
	void RevivePresentSpot();
	void ReviveLogIn();
	void ReviveAfterVimu( BOOL bSendMsg = TRUE );

	//Çö»ó±Ý
	void ClearWantedList();
	WANTEDTYPE GetWantedIdx()	{ return m_WantedIDX; }
	void SetWantedIdx(WANTEDTYPE val)	{ m_WantedIDX = val; }
		
	DWORD GetWantedNum()			{ return m_WantedList.GetCount(); }
	
	void ClearMurderIdx();
	DWORD GetMurderIDX()						{ return m_MurdererIDX;	}
	WORD GetMurderKind()						{ return m_MurdererKind;}
	BOOL IsReadyToRevive()						{ return m_bReadyToRevive;	}
	void SetReadyToRevive(BOOL val)				{ m_bReadyToRevive = val;	}
	void AddWanted(WANTEDTYPE WantedIDX);
	void DeleteWanted(WANTEDTYPE WantedIDX);
	BOOL IsWantedOwner(WANTEDTYPE WantedIDX);
	

// KES EXCHNAGE 030922 
	void SetExchangeContainer( sEXCHANGECONTAINER* ExchangeContainer )

											{ m_ExchangeContainer = *ExchangeContainer; }
	sEXCHANGECONTAINER* GetExchangeContainer() { return &m_ExchangeContainer; }
// KES VIMU 031101
	void SetVimuing( BOOL bVimuing ) { m_bVimuing = bVimuing; }
	BOOL IsVimuing() { return m_bVimuing; }
	void SetVimuOpPlayerID( DWORD dwOpPlayerID ) { m_dwOpPlayerID = dwOpPlayerID; }
	DWORD GetVimuOpPlayerID()	{ return m_dwOpPlayerID; }
//	BOOL IsKilledWithPanelty()			{ return m_bNeedRevive; }
// KES OOPTION 031213
	sGAMEOPTION* GetGameOption() { return &m_GameOption; }
	void SetGameOption( sGAMEOPTION* pGameOption )	{ m_GameOption = *pGameOption; }
	void SetAvatarView( bool bNoAvatarView )		{ m_HeroCharacterInfo.bNoAvatarView = bNoAvatarView;	}
	bool GetAvatarView()		{ return m_HeroCharacterInfo.bNoAvatarView;	}
// KES PK 040205
	int PKModeOn();
	BOOL PKModeOff();
	DWORD GetPKOffTime();
	void PKModeOffForce();
	BOOL IsPKMode()		{ return m_HeroCharacterInfo.bPKMode; }
	void SetPKStartTimeReset() { m_dwPKModeStartTime = gCurTime; }
	void AddPKContinueTime( DWORD dwTime ) { m_dwPKContinueTime += dwTime; }

	void SetPKModeEndtime();
// KES EXIT
	BOOL IsExitStart() { return m_bExit; }
	int CanExitStart();
	void SetExitStart( BOOL bExit );
	int CanExit();
	void ExitCancel();
	void SetNormalExit() { m_bNormalExit = TRUE; }
	BOOL IsNormalExit() { return m_bNormalExit; }
//
	void SetUserLevel( DWORD dwUserLevel ) { m_dwUserLevel = dwUserLevel; }
	DWORD GetUserLevel() { return m_dwUserLevel; }
////////


// LBS ³ëÁ¡»ó °ü·Ã 03.09.29 /////////////////////////////////////////////
	void SetGuestStall( cStreetStall* pStall ) { m_pGuetStall = pStall; }
	cStreetStall* GetGuestStall() { return m_pGuetStall; }
	void SetStreetStallTitle(char* title);
	void GetStreetStallTitle(char* title);
/////////////////////////////////////////////////////////////////////////


	//////////////////////////////////////////////////////////////////////////
	// ¿À¹ö¶óÀÌµù ÇÔ¼öµé
	virtual void DoDie(CObject* pAttacker);
	virtual void DoDamage(CObject* pAttacter,RESULTINFO* pDamageInfo,DWORD beforeLife);
	virtual DWORD Damage(CObject* pAttacker,RESULTINFO* pDamageInfo);
	
	void SetMaxLife(DWORD val);
	void SetMaxShield(DWORD val);
	void SetMaxNaeRyuk(DWORD val);
	
	LEVELTYPE GetMaxLevel()		{	return m_HeroInfo.MaxLevel;	}

	void SetMaxLevel(LEVELTYPE val){ m_HeroInfo.MaxLevel = val; }

	//////////////////////////////////////////////////////////////////////////
	// Á¤º¸ ¾ò¾î¿À´Â ÇÔ¼öµé
	virtual LEVELTYPE GetLevel();
	virtual void SetLevel(LEVELTYPE level);

	virtual DWORD GetLife();
	virtual void SetLife(DWORD Life,BOOL bSendMsg = TRUE);
	virtual DWORD GetShield();
	virtual void SetShield(DWORD Shield,BOOL bSendMsg = TRUE);
	virtual DWORD GetNaeRyuk();
	virtual void SetNaeRyuk(DWORD valb,BOOL SendMsg = TRUE);	
	virtual DWORD DoGetMaxLife();
	virtual DWORD DoGetMaxShield();
	virtual DWORD DoGetMaxNaeRyuk();
	virtual DWORD DoGetPhyDefense();
	virtual float DoGetAttDefense(WORD Attrib);
	virtual float GetAttribPlusPercent(WORD Attrib);

	virtual DWORD DoGetPhyAttackPowerMin();
	virtual DWORD DoGetPhyAttackPowerMax();

	virtual float DoGetMoveSpeed();

#ifdef _JAPAN_LOCAL_
	virtual float DoGetAddAttackRange(){	return (float)GetMinChub() / 2.f;	}
#else
	virtual float DoGetAddAttackRange(){	return GetMinChub() / 3.f;	}
#endif

	virtual DWORD DoGetCritical();
	virtual DWORD DoGetDecisive();



	WORD GetKyungGongGrade()	{	return m_HeroInfo.KyungGongGrade;	}
	void SetKyungGongGrade(WORD grade)	{	m_HeroInfo.KyungGongGrade = grade;	}

	ITEMOBTAINARRAYINFO * GetArray(WORD id);
	void AddArray(ITEMOBTAINARRAYINFO * pInfo);
	void RemoveArray(DWORD id);
	CYHHashTable<ITEMOBTAINARRAYINFO> m_ItemArrayList;

	BOOL AddFollowList(CMonster * pMob);
	void RemoveFollowList(DWORD ID);
//	void RemoveFollowAsFarAs(DWORD GAmount);
	BOOL RemoveFollowAsFarAs(DWORD GAmount, CObject* pObject );

	CYHHashTable<CMonster> m_FollowMonsterList;

// ÃÊº¸ÀÚ °¡ÀÌµå
	void AddQuest(CQuestBase* pQuest);
	void RemoveQuest(DWORD QuestIdx);
	BOOL SetQuestState(DWORD QuestIdx, QSTATETYPE value);
	CYHHashTable<CQuestBase>	m_QuestList;

	virtual void StateProcess();

	// PlayTime
	DWORD GetPlayTime()				{	return m_HeroInfo.Playtime;	}
	void SetLoginTime(int time)		{	m_LoginTime = time;	}

	void ReviveLogInPenelty();
// jsd quest
protected:
	CQuestGroup		m_QuestGroup;
public:
	CQuestGroup*	GetQuestGroup()		{ return &m_QuestGroup; }
	void			QuestProcess();

	void			RSetMoney( MONEYTYPE money, BYTE flag );	

	DWORD			m_dwOldMapNum;
	WORD			m_wKillMonsterLevel;

	//  
	int m_LastRedEgeTime;
	int m_LastYellowEgeTime;
	int m_LastBlueEgeTime;
	// 2
	int m_LastRecvPingSYNTime;
	int m_PingSYNFastCount; // 2016
	
	void	SetKillMonsterLevel( WORD wMonLevel )	{ m_wKillMonsterLevel = wMonLevel; }
	WORD	GetKillMonsterLevel()					{ return m_wKillMonsterLevel; }

	// ShopItem
	CShopItemManager* GetShopItemManager()			{	return &m_ShopItemManager;	}
	void SetShopItemInit( BOOL bInit )				{	m_bShopInit = bInit;	}
	void InitShopItemInfo(SHOP_ITEM* pItemInfo);
	void SendShopItemInfo();

	void UpdateLogoutToDB();
	
	void SetInitPoint( DWORD Pos )		{	m_InitPosition = Pos;	}
	DWORD GetInitPoint()				{	return m_InitPosition;	}
	
	BOOL IsAbleReviveOther();
	BOOL ReviveShopItem( WORD ItemIdx );
	void SetReviveData( DWORD Id, WORD ItemIdx, POSTYPE ItemPos )
	{	m_ReviveData.TargetID=Id;	m_ReviveData.ItemIdx=ItemIdx;	m_ReviveData.ItemPos=ItemPos;	}
	REVIVEDATA* GetReviveData()			{	return &m_ReviveData;	}
	void SetReviveTime( DWORD time )	{	 m_ReviveTime = time;	}

	BOOL GetSavePointInit()					{	return m_bSavePointInited;	}
	void SetSavePointInit( BOOL val )		{	m_bSavePointInited = val;	}
	
	// ItemLoad ¼öÁ¤
	void InitShopInvenInfo(ITEMBASE* pShopWareHouse);

	//SW051129 Pet
	CPetManager* GetPetManager()			{	return &m_PetManager;	}
#ifdef  _MUTIPET_
	CPet* GetCurPet(BYTE i) { return m_PetManager.GetCurSummonPet(i); }//µ¶¸ç  3pet
#endif //  _MUTIPET_
//	void SetCurPet(CPet* pPet)				{	m_pCurPet = pPet;	}  // JACK
//	CPet*	GetCurPet()						{	return m_pCurPet;	}  // JACK


	
	BOOL GetKyungGongIdx()				{	return m_MoveInfo.KyungGongIdx;	}

protected:
	//SW080515 Ã¤³Îº° ¼¼ÆÃ
	BOOL		m_bDieForBattleChannel;

	BOOL		m_bDieForGFW;		// ¹®ÆÄÀü»ó´ëÇÑÅ× Á×¾úÀ»¶§...
	BOOL		m_bDieInSpecialMap;
	WORD		m_wReturnMapNum;
	WORD		m_wGTReturnMapNum;
	int			m_ObserverBattleIdx;

public:
	void		SetDieForBattleChannel( BOOL bVal )	{	m_bDieForBattleChannel = bVal;	}	//¹èÆ²Ã¤³Î¿¡¼­ Æò»ó°ø°ÝÀ¸·Î Á×¾úÀ» ¶§ ÇØ´ç. »ì±â³ª ±âÅ¸ battle»óÅÂ Á¦¿Ü.
	BOOL		IsDieForBattleChannel()		{	return m_bDieForBattleChannel;	}
	void		SetDieForGFW( BOOL bDie )	{ m_bDieForGFW = bDie; }
	BOOL		IsDieFromGFW()				{ return m_bDieForGFW; }
	void		SetDieInSpecialMap( BOOL bDie, WORD wReturnMap )	{ m_bDieInSpecialMap = bDie; m_wReturnMapNum = wReturnMap; }	
	
	void		SendPlayerToMap();	
	
	// Guild Tournament
	void SetReturnMapNum( WORD num )	{	m_wGTReturnMapNum = num;	}
	WORD GetReturnMapNum()			{	return m_wGTReturnMapNum;	}
	void SetObserverBattleIdx( int Idx )		{	m_ObserverBattleIdx = Idx;	}
	int	GetObserverBattleIdx()				{	return m_ObserverBattleIdx;	}

	void		SetChangeAbilityKyungGongLevel( WORD wLevel );

	// guildunion
	DWORD GetGuildUnionIdx()		{ return m_HeroCharacterInfo.dwGuildUnionIdx; }
	void InitGuildUnionInfo( DWORD dwGuildUnionIdx, char* pGuildUnionName, DWORD dwMarkIdx );
	void SetGuildUnionInfo( DWORD dwGuildUnionIdx, char* pGuildUnionName, DWORD dwMarkIdx );
	void SetGuildUnionMarkIdx( DWORD dwMarkIdx )	{ m_HeroCharacterInfo.dwGuildUnionMarkIdx = dwMarkIdx; }

	// SiegeWar
	void SetRestraintMode( bool val )			{	m_HeroCharacterInfo.bRestraint = val;	}
	BOOL IsRestraintMode()						{	return m_HeroCharacterInfo.bRestraint;	}
	void ReviveVillage();

	//
	void SetCharChangeInfo( CHARACTERCHANGE_INFO* pInfo );	

	// character attribute (¼Ó¼º)
#ifdef _JAPAN_LOCAL_
//	DWORD	GetCharAttr()			{ return m_HeroCharacterInfo.dwCharacterAttr; }
	int		GetMainCharAttr()		{ return m_HeroCharacterInfo.nMainAttr; }		// ÁÖ¼Ó¼º	
	int		GetSubCharAttr()		{ return m_HeroCharacterInfo.nSubAttr; }		// ºÎ¼Ó¼º
	BOOL	ChangeMainCharAttr( int nMainAttr );
	BOOL	ChangeSubCharAttr( int nSubAttr );	
	int		WhatIsAttrRelation( int nOtherMainAttr );		// ¼Ó¼º°ü°è´Â?			

	// ItemSlot
	void	SetExtraSlotCount( DWORD PyogukSlot, DWORD InvenSlot, DWORD MugongSlot );
	DWORD	GetExtraInvenSlot()			{	return m_HeroCharacterInfo.ExtraInvenSlot;		}
	DWORD	GetExtraPyogukSlot()		{	return m_HeroCharacterInfo.ExtraPyogukSlot;		}
	DWORD	GetExtraMugongSlot()		{	return m_HeroCharacterInfo.ExtraMugongSlot;		}
	void	SetExtraCharacterSlot( DWORD SlotNum )		{	m_dwCharacterSlotNum = SlotNum;	}
	DWORD	GetExtraCharacterSlot()		{	return m_dwCharacterSlotNum;	}
#endif

#ifdef _HK_LOCAL_
	// ItemSlot
	void	SetExtraSlotCount( DWORD PyogukSlot, DWORD InvenSlot, DWORD MugongSlot );
	DWORD	GetExtraInvenSlot()			{	return m_HeroCharacterInfo.ExtraInvenSlot;		}
	DWORD	GetExtraPyogukSlot()		{	return m_HeroCharacterInfo.ExtraPyogukSlot;		}
	DWORD	GetExtraMugongSlot()		{	return m_HeroCharacterInfo.ExtraMugongSlot;		}
	void	SetExtraCharacterSlot( DWORD SlotNum )		{	m_dwCharacterSlotNum = SlotNum;	}
	DWORD	GetExtraCharacterSlot()		{	return m_dwCharacterSlotNum;	}
#endif

#ifdef _TL_LOCAL_
	// ItemSlot
	void	SetExtraSlotCount( DWORD PyogukSlot, DWORD InvenSlot, DWORD MugongSlot );
	DWORD	GetExtraInvenSlot()			{	return m_HeroCharacterInfo.ExtraInvenSlot;		}
	DWORD	GetExtraPyogukSlot()		{	return m_HeroCharacterInfo.ExtraPyogukSlot;		}
	DWORD	GetExtraMugongSlot()		{	return m_HeroCharacterInfo.ExtraMugongSlot;		}
	void	SetExtraCharacterSlot( DWORD SlotNum )		{	m_dwCharacterSlotNum = SlotNum;	}
	DWORD	GetExtraCharacterSlot()		{	return m_dwCharacterSlotNum;	}
#endif


	void	SetExtraMugongSlot(DWORD MugongSlot)
	{
		if (MugongSlot>8)
			m_HeroCharacterInfo.ExtraMugongSlot = 8;
		else
			m_HeroCharacterInfo.ExtraMugongSlot = MugongSlot;
	}
	DWORD	GetExtraMugongSlot()		{ return m_HeroCharacterInfo.ExtraMugongSlot; }
	//SW051112 ¹«½Ö¸ðµå
	void	InitMussangPoint()			{	m_dwMussangPoint = 0;		}
	void	SetMussangMode(BOOL bMode)	{	m_HeroCharacterInfo.bMussangMode = bMode;	}
	BOOL	IsMussangMode()				{	return m_HeroCharacterInfo.bMussangMode;	}
	BOOL	IsMussangReady()			{	return m_bIsMussangReady;	}
	DWORD	StartMussangMode();
	void	CheckMussangTime();
	void	SetMussangMaxPoint();
//	DWORD	GetMussangMaxPoint()		{	return m_HeroInfo.MaxMussangPoint;	}
	DWORD	GetMussangMaxPoint();
	void	AddMussangPointFromLevel(DWORD dwMonsterLevel);
	void	SendMussangInfo();
	void	SendMussangEnd();	//Ä³¸¯ Á×¾úÀ» ½Ã ¶Ç´Â Áö¼Ó ½Ã°£ Á¾·á½Ã
	void	CheatMussangReady();

#ifdef _JAPAN_LOCAL_
	virtual DWORD CalcShieldDamage(DWORD ShieldDamage);
#else
	// 06. 03 ±¹³»¹«½Ö - ÀÌ¿µÁØ
	virtual DWORD CalcShieldDamage(DWORD ShieldDamage, DWORD &RealShieldDamage);
	WORD	GetMussangStat() { return m_wMussangStat; }
#endif

	////////////////////////////////////////////////////////
	//06. 06 2Â÷ ÀüÁ÷ - ÀÌ¿µÁØ
	//ÀÌÆåÆ® »ý·«(¹«ÃÊ)
	BOOL IsSkipSkill();
	////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////
	// 06. 06. 2Â÷ ÀüÁ÷ - ÀÌ¿µÁØ
	// ¹«°ø º¯È¯ Ãß°¡
	void AddSkillStatsOption(SKILLOPTION *pSkillOption);
	void RemoveSkillStatsOption(SKILLOPTION *pSkillOption);

	//////////////////////////////////////////////////////////////////////////
	
	//////////////////////////////////////////////////////////////////////////
	// 06. 06. 2Â÷ ÀüÁ÷ - ÀÌ¿µÁØ
	// Àº½Å
	BOOL IsHide() { return m_BaseObjectInfo.SingleSpecialState[eSingleSpecialState_Hide]; } 
	BOOL IsDetect()
	{ 

		return (m_BaseObjectInfo.SingleSpecialState[eSingleSpecialState_Detect] ||
			m_BaseObjectInfo.SingleSpecialState[eSingleSpecialState_DetectItem]); 
	} 
	//////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////
	// 06. 07. ½ºÅ³°³¼± - ÀÌ¿µÁØ
	CYHHashTable<CSkillObject>* GetSkillList() { return &m_SkillList; }
	//////////////////////////////////////////////////////////////////////////

	////////////////////////////////////////////////////////////////////////////
	// 06. 07. »óÅÂ°­Á¦º¯°æ - ÀÌ¿µÁØ
	// ÀÏÁ¤ÀÌ»ó ½ºÅ³ »ç¿ë½ÇÆÐ½Ã Æ¯º°ÇÑ »óÅÂ¸¦ Á¦¿ÜÇÑ ³ª¸ÓÁö »óÅÂ´Â ¸ðµÎ ÃÊ±âÈ­
	BOOL CanSkillState();
	////////////////////////////////////////////////////////////////////////////

	// 06.09.25 RaMa Ãß¼®ÀÌº¥Æ®
	void IncreaseEventHammerCount();

	//SW061211 Å©¸®½º¸¶½ºÀÌº¥Æ®
	void SummonEventPetRndm();
	void SetEventSnowIdx(int snowIdx)	{ m_CristmasEventSnowIdx = snowIdx;	}
	DWORD GetEventSnowIdx()	{	return m_CristmasEventSnowIdx;	}

	//SW061205 ±ê¹ß NPC
	void SendFlagNPCState();

	void InitTitanWearItemInfo( ITEMBASE* pTitanItemInfo );	// magi82 - Titan(070209)
	void InitTitanShopItemInfo( ITEMBASE* pTitanItemInfo );	// magi82 - Titan(070228)

	//SW070127 Å¸ÀÌÅº
	BOOL InTitan();
	BOOL CanUseTitanSkill();
	CTitanManager*	GetTitanManager()	{	return &m_TitanManager;	}
	CTitan* GetCurTitan()	{	return m_TitanManager.GetCurRidingTitan();	}
	void SetTitanWearInfo(POSTYPE slotPos, ITEMBASE* pItemBase, BOOL bIn);
	void ApplyTitanWearItem();

	///// 2007. 6. 14. CBH - ¼¼Æ®¾ÆÀÌÅÆ ´É·ÂÄ¡ Ã³¸® ÇÔ¼ö
	void AddSetitemOption(SET_ITEM_OPTION* pSetItemOption);
	void ClearSetitemOption();
	///////////////////////////////////////////////////////
	void AddSetitemQualityOption(SET_ITEMQUALITY_OPTION* pSetItemQualityOption);
	void ClearSetitemQualityOption();
	// 
	void SetActionTime()				{	m_LastActionTime = gCurTime;	}
	BOOL IsActionPanelty()				{	return m_bActionPanelty;		}
	void SetActionPanelty()				{	m_bActionPanelty = TRUE;		}
	void SendNoActionPaneltyMsg();
	void InsetNoActionPaneltyLog( BYTE eLogKind );

	// magi82 - UniqueItem(070626)
	void AddUniqueItemOption(UNIQUE_ITEM_OPTION_INFO* pUniqueOption);
	void ClearUniqueItemOption()	{	ZeroMemory(&m_UniqueItemStats, sizeof(m_UniqueItemStats));	}
	//////////////////////////////////////////////////////////////////////////

	// magi82 - SOS(070720)
	int GetSOSState()	{	return m_SOSState;	}
	void SetSOSState(int num)	{	m_SOSState = num;	}

	/////////////////////////////////////////////////////////////////////////////////////////////////
	void InitSkinDelay();			//2007. 12. 10. CBH - ½ºÅ²±³Ã¼ µô·¹ÀÌ ½Ã°£ °ü·Ã ÃÊ±âÈ­
	void StartSkinDelay();			//2007. 12. 10. CBH - ½ºÅ²±³Ã¼ µô·¹ÀÌ ½ÃÀÛ	
	BOOL GetSkinDelayResult();		//2007. 12. 10. CBH - ½ºÅ² µô·¹ÀÌÁßÀÎÁö ¹ÝÈ¯
	DWORD GetSkinDelayTime();		//2007. 12. 10. CBH - ½ºÅ² µô·¹ÀÌ ³²Àº½Ã°£ ¹ÝÈ¯
	/////////////////////////////////////////////////////////////////////////////////////////////////
	INSDG_RANK_STATE m_RankState;		
	CYHHashTable<INSDG_RANK_INFO> m_InsDGRankInfoTable;
	// AutoNote
protected:
	DWORD	m_dwAutoNoteAskPlayerIdx;		// ¿ÀÅäÁú¹®»ó´ëÀÎµ¦½º
	DWORD	m_dwLastAttackTimeForAutoNote;

public:
	void SetAutoAskPlayerIdx( DWORD dwPlayerIdx )	{ m_dwAutoNoteAskPlayerIdx = dwPlayerIdx; }
	DWORD GetAutoAskPlayerIdx()						{ return m_dwAutoNoteAskPlayerIdx; }

	void SetLastAttackTimeForAutoNote( DWORD dwTime )	{ m_dwLastAttackTimeForAutoNote = dwTime; }
	DWORD GetLastAttackTimeForAutoNote()				{ return m_dwLastAttackTimeForAutoNote; }

	// 2014-05-30 the mallmoney process !

	void MallMoneyProcess();

	void	SetLastDungeonTime(UINT val){ m_LastDungeonTime = val; }
	UINT	GetLastDungeonTime(){ return m_LastDungeonTime; }


	int     GetCurMapNum(){ return m_HeroCharacterInfo.CurMapNum; }
    void	SetPaoDianTime(UINT val){ m_LastPaoDianTime = val; }
	UINT	GetPaoDianTime(){ return m_LastPaoDianTime; }
	void	SetLastMapTime(UINT val){ m_LastMapTime = val; }
	UINT	GetLastMapTime(){ return m_LastMapTime; }
    void	SetConnection(DWORD nIndex){ m_dwConnection = nIndex; }
	DWORD	GetConnection(){ return m_dwConnection; }

	// 2014-06-18 the lucker func!

	void    SetPlayerLucker();

	//ÉÁÃû³ÆºÅº¯Êý¶¨Òå
	bool	IsFlgName() { return m_HeroCharacterInfo.FlgName != 0; }
	void	SetFlgName(WORD val) { m_HeroCharacterInfo.FlgName = val; }
	WORD	GetFlgName() { return m_HeroCharacterInfo.FlgName; }	


	//IMAGE NAME BY JACK
	bool IsImageName(){return m_HeroCharacterInfo.ImageName != 0;}
	void SetImageName(WORD val){m_HeroCharacterInfo.ImageName = val;}
	WORD GetImageName(){return m_HeroCharacterInfo.ImageName;}

	// 2015-01-28 

	void    SetGambleInfo(GAMBLE_USER_INFO * pGambleInfo);

	GAMBLE_USER_INFO * GetGambleInfo(){return &m_GambleInfo;}

	void    ClearGambInfo();

	void SyncVipLevelFromDb();

	//weiye 2017-10-23 °²È«ËøÏà¹Øº¯Êý¶¨Òå 2017-10-24

	void SetSafeStatic(BOOL Val);

	void SetSafeStaticValue(BOOL Val);

	BOOL GetSafeStatic(){return m_IsUnLock;}

	//weiye ¸±±¾Ê±¼ä´¦Àíº¯Êý 2018-02-06

	//void  FBProcess();

/*	int  GetCurMapNum(){return m_HeroCharacterInfo.CurMapNum;}
	void SetLastMapTime(UINT val){ m_LastMapTime = val; }
	UINT GetLastMapTime(){ return m_LastMapTime; }*/

	DWORD GetFBTime(){return m_FBTime;}
	void  SetFBTime(DWORD Val);
	void  SetFBTimeTo(DWORD Val);  //weiye FB¸±±¾Ê±¼ä²»Ð´Êý¾Ý¿â,Ë¢ÐÂÓÃ»§ÄÚ´æÐÅÏ¢
	//void  CreateNewFBTime(); 
	BOOL  CheckFBTime(DWORD MapIndex);

	//weiye 2018-03-22     ÕûÀí±³°üÏà¹Øº¯Êý¶¨Òå
	void SetItemTidy();
	void SetItemShopTidy();
	void SendItemTidyInfo();

	void SendItemTidyShopInfo();

	void TidyFailed(DWORD dwTime);


	void	SetHeroFame(int val,char * aName,char * dName,BYTE Type = 0);


	bool    IsTurnOne(){return  m_HeroCharacterInfo.TurnStageOne!=0;}
	void    SetTurnOne(WORD Flag){ m_HeroCharacterInfo.TurnStageOne=Flag;}
	WORD    GetTurnOne(){return m_HeroCharacterInfo.TurnStageOne;}

	bool    IsTurnTwo(){return  m_HeroCharacterInfo.TurnStageTwo!=0;}
	void    SetTurnTwo(WORD Flag){ m_HeroCharacterInfo.TurnStageTwo=Flag;}
	WORD    GetTurnTwo(){return m_HeroCharacterInfo.TurnStageTwo;}

	bool    IsTurnThree(){return  m_HeroCharacterInfo.TurnStageThree!=0;}
	void    SetTurnThree(WORD Flag){ m_HeroCharacterInfo.TurnStageThree=Flag;}
	WORD    GetTurnThree(){return m_HeroCharacterInfo.TurnStageThree;}


	


	void SetKilledPlayerID(DWORD val){m_KilledPlayerID = val;}
	DWORD GetKilledPlayerID(){return m_KilledPlayerID;}
	void SetLootPos(DWORD val){m_LootingPos = val;}
	DWORD GetLootPos(){return m_LootingPos;}

	//void    SetFlashName(char * FlashName){SafeStrCpy(m_HeroCharacterInfo.FlashName,FlashName,MAX_NAME_LENGTH+1);}
	//char *  GetFlashName(){return m_HeroCharacterInfo.FlashName;}

	BOOL bFirstEvent;
	void LoginEventProcess();
	

	void	SetEventLastMapTime(UINT val){ m_EventCountDownTimer = val; }
	UINT	GetEventLastMapTime(){ return m_EventCountDownTimer; }
	WORD GetEventIDFlag(){return m_EventIDFlag;}
	void SetEventIDFlag(int val){ m_EventIDFlag = val; }





	BOOL   m_FBCreate;
	BOOL   GetFBCreate(){return m_FBCreate;}
	void   SetFBCreate(BOOL Flag){m_FBCreate=Flag;}

	BOOL   m_FBCreateFirstTime;
	BOOL   GetFBCreateFirstTime(){return m_FBCreateFirstTime;}
	void   SetFBCreateFirstTime(BOOL Flag){m_FBCreateFirstTime=Flag;}

//-------------------------------------------------------------
	DWORD  m_FBTimerMonsterNum;
	void   InitFBTimerMonsterNum(){m_FBTimerMonsterNum=0;}
	DWORD  GetFBTimerMonsterNum(){return m_FBTimerMonsterNum;}
	void   SetFBTimerMonsterNum(DWORD Val){m_FBTimerMonsterNum=Val;}
	void   AddFBTimerMonsterNum(DWORD Num){m_FBTimerMonsterNum=m_FBTimerMonsterNum+Num;}
//-------------------------------------------------------------

	DWORD  m_FBTimerMapType;
	void   InitFBTimerMapType(){m_FBTimerMapType=0;}
	DWORD  GetFBTimerMapType(){return m_FBTimerMapType;}
	void   SetFBTimerMapType(DWORD Val){m_FBTimerMapType=Val;}
	void   AddFBTimerMapType(DWORD Num){m_FBTimerMapType=m_FBTimerMapType+Num;}

	DWORD  m_FBMapArea;
	void   InitFBMapArea(){m_FBMapArea=0;}
	DWORD  GetFBMapArea(){return m_FBMapArea;}
	void   SetFBMapArea(DWORD Val){m_FBMapArea=Val;}
	void   AddFBMapArea(DWORD Num){m_FBMapArea=m_FBMapArea+Num;}
	//void FubenInfoKind(DWORD Val,DWORD dwFlag,BYTE Type,DWORD Val2=0);




	void TestPet();

	void StartLoginEvent();





	void SetRankState(INSDG_RANK_STATE* pRankState);
	INSDG_RANK_STATE* GetRankState();
	void RemoveInsDGRankInfoTable();
	void AddInsDGRankInfo(INSDG_RANK_INFO* pRankInfo);
	INSDG_RANK_INFO* GetInsDGRankInfo(DWORD dwInsDGIndex);







private:
	BOOL		m_bDungeonObserver;
	VECTOR3		m_DungeonObserverPos;
public:
	void		SetDungeonObserver(BOOL bVal)		{m_bDungeonObserver = bVal;}
	BOOL		IsDungeonObserver()					{return m_bDungeonObserver;}
	void		SetDungeonObserverPos(VECTOR3* pPos){memcpy(&m_DungeonObserverPos, pPos, sizeof(VECTOR3));}
	VECTOR3*	GetDungeonObserverPos()				{return &m_DungeonObserverPos;}

	DWORD GetDungeonFinishTime(){return pDungeonFinishTime;}
	void SetDungeonFinishTime(DWORD pTime){pDungeonFinishTime = pTime;}
	void SetDungeonFinishSwitch(BOOL bFlag){pDungeonFinishSwitch=bFlag;}
	BOOL GetDungeonFinishSwitch(){return pDungeonFinishSwitch;}


	void GetItemtotalInfo(ITEMBASE Inventory[]);
	void MoveInventPack(bool &bRet);
	void SetItemTotalInfo(ITEMBASE * pItemBaseAll);
	DWORD m_MovePackTime;
	DWORD IsCanMovePack();
	void SetMovePackTime(){m_MovePackTime = gCurTime;}


private:
	__time64_t	ForbidChatTime;
public:
	void		SetForbidChatTime( __time64_t time ) { ForbidChatTime = time; }
	__time64_t	GetForbidChatTime() const { return ForbidChatTime; }
	BOOL		IsForbidChat() const;


	void AddKillTimes();
	void ClearKillTimes();
	//ÉèÖÃVipµÈ¼¶
	void SetVipLevel(DWORD Level);
	WORD GetVipLevel() { return m_HeroInfo.VipLevel; }//»ñÈ¡VipµÈ¼¶
};

#endif // !defined(AFX_PLAYER_H__D36AF2E1_0FD9_4120_B257_64B0BE21D669__INCLUDED_)


