




#pragma once

#include "./Interface/cImage.h"


#define PETMGR	USINGTON(CPetManager)

#define PET_DIST_CHECKTIME 500
#define PET_MAX_FRIENDSHIP		10000000
#define PET_SKILLCHARGE_CHECKTIME	1000
#define PET_MAX_SKILL_CHARGE	10000
#define PET_RESUMMON_VALID_TIME	30000

#define MAX_PET_BUFF_NUM	3

#define CRISTMAS_EVENTPET 8


enum ePetFeedResult{ePFR_Sucess=0, ePFR_Unsummoned, ePFR_StaminaFull};

enum ePetUpgradeDlgGrade{ePUDG_Default, ePUDG_Grade2=1, ePUDG_Grade3=2};

enum ePetRevivalDlgGrade{ePRDG_Default, ePRDG_Grade1, ePRDG_Grade2, ePRDG_Grade3, ePRDG_Grade_All};	//ePRDG_Grade_All => ShopItem ¿ë

//¹öÇÁ

enum ePetBuffKind
{ePB_None,
ePB_Demage_Percent = 1,		//Ôö¼Ó¹¥»÷Á¦
ePB_Dodge = 2,				//ÉÁ±Ü
ePB_MasterAllStatUp = 3,		//ËùÓĞÊôĞÔÔö¼Ó

ePB_Item_DoubleChance = 4,	//»ñµÃÎïÆ·»úÂÊÌáÉı
ePB_NoForeAtkMonster = 5,		//°²¸§Ö÷¶¯¹ÖÎï
ePB_ReduceCriticalDmg = 6,	//¼õÉÙÊÜµ½·ÜÁ¦Ò»»÷ÉËº¦
ePB_MasterAllStatRound = 7,	//ÊôĞÔµ÷Õû
ePB_Item_RareProbUp = 8,		//»ñµÃ×£¸£×°±¸»úÂÊÔö¼Ó
ePB_MussangTimeIncrease = 9,	//ÎŞË«Ä£Ê½Î¬³ÖÊ±¼äÔö¼Ó
ePB_Kind_Max};

enum {ePetRestBtn,ePetUseBtn};

//ÆêÁ¾·ù
enum ePetKind{ePK_None, ePK_CommonPet=1, ePK_ShopItemPet=2, ePK_EventPet=4,};

class CPet;

class cDialog;
class CPetStateDlg;
class CPetStateMiniDlg;
class CPetInventoryDlg;
class CPetMultiDlg;
class CPetManager
{
	CYHHashTable<PET_TOTALINFO>		m_PetInfoList;
	CYHHashTable<cImage>			m_PetImageList;

	DWORD				m_dwStateCheckTime;				//Æê ½ºÅÈ Ã¼Å© Å¸ÀÓ( 10ÃÊ )
	DWORD				m_dwStaminaDecrease;			//½ºÅ×¹Ì³ª °¨¼Ò·®
	DWORD				m_dwOldFriendShipForToolTipChange;	//ÅøÆÁ Ä£¹Ğµµ °»½ÅÀ» À§ÇÑ ÀúÀå

	DWORD				m_dwDistCheckTime;					//ÇÁ·Î¼¼½º Å¸ÀÓ(ÁÖÀÎ-Æê°Å¸®Ã¼Å©)
	BOOL				m_bValidDistance;				//¿òÁ÷¿©¾ß µÉ °Å¸®ÀÎ°¡
	BOOL				m_bReadytoMove;					//¿òÁ÷¿©µµ µÉ Á¶°ÇÀÎ°¡(movemgr¿¡¼­ ¼¼ÆÃ)

	DWORD				m_dwSkillRechargeCheckTime;		//½ºÅ³°ÔÀÌÁö Ã¼Å© Å¸ÀÓ(1ÃÊ)
	DWORD				m_dwSkillRechargeAmount;
	BOOL				m_bSkillGuageFull;
	BOOL				m_bReadyToSendSkillMsg;

	DWORD				m_dwResummonDelayTime;			//Æê Àç¼ÒÈ¯ ·¹ÀÌ//ºÀÀÎÈÄ 30ÃÊ Ä«¿îÆ®

	CPet*				m_pCurSummonPet;
	BOOL				m_bIsPetStateDlgToggle;			//Åä±Û »óÅÂ ÀúÀå
	cDialog*			m_pCurPetStateDlg;				//ÇöÀç Æê »óÅÂÃ¢
	CPetStateDlg*		m_pStateDlg;					//Æê »óÅÂÅ«Ã¢

	CPetStateMiniDlg*	m_pStateMiniDlg;				//Æê »óÅÂÀÛÀºÃ¢
	CPetInventoryDlg*	m_pInvenDlg;

	CPetMultiDlg*		m_pMultiDlg;
	BOOL				m_bReadyToSendRestMsg;			//Æê ÈŞ½Ä »óÅÂ ¼³Á¤Á¤º¸ ¼­¹ö ¸®ÅÏ µÚ TRUE

	WORD				m_wPetKind;						//ÆêÁ¾·ù. ÀÏ¹İ/¾ÆÀÌÅÛ¸ô/ÀÌº¥Æ® Æê

public:
	CPetManager(void);
	~CPetManager(void);

	void	PetMgrInit();
	void	PetMgrRelease();
	void	PetMgrProcess();
	void	AddHeroPet(CPet* pPet);
	void	InitPetInfo(PET_TOTALINFO* pPetInfo, int num);
	void	AddPetInfo(PET_TOTALINFO* pPetInfo);
	void	RemovePetInfo(DWORD dwSummonItemDBIdx);
	void	ReleasePetInfoList();
	PET_TOTALINFO*	GetPetInfo(DWORD dwItemDBIdx);
	void	UpdateCurPetInfo(CPet* pPet);
	void	OnPetRemove(CPet* pPet);

	void	SetResummonCheckTime();
	BOOL	CheckResummonAvailable();
	DWORD	GetPetResummonRestTime();

	BOOL	CheckDefaultFriendship(DWORD dwItemDBIdx);
	DWORD	GetPetFriendship(DWORD dwItemDBIdx);
	BOOL	IsCurPetStaminaFull();
	BOOL	IsCurPetSummonItem(DWORD dwItemDBIdx);
	void	RemovePetFromTable(DWORD dwPetID);
	void	SetCurSummonPet(CPet* pPet)	{	m_pCurSummonPet=pPet;	}
	CPet*	GetCurSummonPet() {	return m_pCurSummonPet;	}
	CPet*	GetHeroPet(DWORD dwPetID);
	DWORD	GetHeroPetID();
	BOOL	CheckPetSummoned();
	BOOL	CheckPetAlive(DWORD dwSummonItemDBIdx);
	void	ReleasePetTable();

	void	NetworkMsgParse(BYTE Protocol, void* pMsg);

//Æê ½ºÅÈ
	//½ºÅ³ °ÔÀÌÁö ÃæÀü
	void	CalcSkillRecharge();
	void	SetSkillRechargeAmount(DWORD amount);
	BOOL	IsSkillGuageFull()	{	return m_bSkillGuageFull;	}
	void	SetSkillGuageFull(BOOL bVal)	{m_bSkillGuageFull = bVal;}

	//µû¶ó°¡±â
	void	CheckDistWithMaster();
	BOOL	IsValidDist() {	return m_bValidDistance;	}
	void	SetMoveReady(BOOL bVal)	{	m_bReadytoMove = bVal;	}

	//UI
	void	OpenPetStateDlg();		//ÇöÀç Æê »óÅÂÃ¢ ¿­±â
	void	OpenPetInvenDlg();
	void	SetCurPetStateDlg(cDialog* pDlg) {m_pCurPetStateDlg=pDlg;}	//ÇöÀç Æê »óÅÂÃ¢ ¼¼ÆÃ

	void	SetPetStateDlg(CPetStateDlg* pDlg) {m_pStateDlg=pDlg;}
	void	SetPetStateMiniDlg(CPetStateMiniDlg* pDlg) {m_pStateMiniDlg=pDlg;}
	void	SetPetInventoryDlg(CPetInventoryDlg* pDlg) {m_pInvenDlg=pDlg;}

	void	SetPetMultiDlg(CPetMultiDlg* pDlg) { m_pMultiDlg = pDlg; }
	//060310 UI ÀúÀå
	void	SetPetDlgToggle(BOOL bVal)	{	m_bIsPetStateDlgToggle = bVal;	}	//FALSE °¡ Å«Ã¢(±âº»)
	BOOL	GetPetDlgToggle()	{	return m_bIsPetStateDlgToggle;	}
	cDialog* GetCurPetStateDlg()	{	return m_pCurPetStateDlg;	}

	void	TogglePetStateDlg();
	void	SetPetStateDlgInfo(CPet* pPet);
	void	SetPetStateMiniDlgInfo(CPet* pPet);
	void	SetPetStateDlgUseRestInfo(CPet* pPet);
	void	ClosePetAllDlg();

	void	SetPetGuageText(DWORD dwStamina, DWORD dwFriendShip);

	DWORD	GetPetValidInvenMaxTabNum();
	void	SetPetValidInvenTab();

//	BOOL	GetPetDlgToggle() {return m_bIsPetStateDlgToggle;}			//Åä±Û»óÅÂ°¡Á®¿À±â. Å«Ã¢/ÀÛÀºÃ¢
	
	//rest
	void	SetCurPetRest(BOOL bRest);
	void	SendPetRestMsg(BOOL bRest);//Æê »ç¿ë/ÈŞ½Ä Á¤º¸ º¸³»±â
	void	SetReadyToSendRestMsg(BOOL bReady) {	m_bReadyToSendRestMsg = bReady;	}
	BOOL	IsReadyToSendRestMsg()	{	return m_bReadyToSendRestMsg;	}

	//seal

	void	SendPetSealMsg();

	//ability icon. pet skill
	void	CheckRestNSkillUse();	//¹«°øÃ¢ ½ºÅ³¹öÆ° ±â´É. ÈŞ½Ä»óÅÂ¸é ÇØÁ¦½ÃÅ°°í °ÔÀÌÁöÇ®ÀÌ¸é ½ºÅ³ »ç¿ë. ±âÈ¹ÀÚ¿¡°Ô ¹®ÀÇÈÄ ¾Ë°ÔµÊ.

	//skill
	void	InitPetSkillGuage();
	void	UseCurPetSkill();
	void	SetReadyToSendSkillMsg(BOOL bReady) {	m_bReadyToSendSkillMsg = bReady;	}
	BOOL	IsReadyToSendSkillMsg()	{	return m_bReadyToSendSkillMsg;	}
	//SW060324 Æê ¹öÇÁ Ãß°¡
	void	AddMasterStatFromPetBuff(player_calc_stats* pMasterAdditionalStat);
	void	RefleshPetMaintainBuff();

	//pet equip
	WORD	GetPetEquipItemMax(DWORD dwItemDBIdx);

	void	LoadPetImage();
	cImage* GetPetImage(DWORD dwPetIdx);
	void	RemovePetImage();

	void	Release();

	void	SetCurSummonPetKind(CPet* pPet);
	BOOL	CheckCurSummonPetKindIs(int kind);
};

EXTERNGLOBALTON(CPetManager)

//UI Á¤º¸°ü¸®
//HERO's Pet °ü¸®, PetObject´Â ÇÏ³ª¸¸ ÀÖ´Ù. ¸ğµç ¼ÒÀ¯ ÆêµéÀÇ ±âº»Á¤º¸¸¸ °ü¸®.
//HEROÀÇ Æê ¼ÒÈ¯ ¾ÆÀÌÅÛ¿¡ °¡Áö°í ÀÖ´Â Æê ¾ÆÀÌÅÛÀÇ Æê Á¤º¸¸¦ °¡Áö°í ÀÖ´Ù. ¾ÆÀÌÅÛ ÅøÆÁ µî¿¡ º¸¿©ÁØ´Ù.


