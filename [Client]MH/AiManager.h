#ifndef AIMANAGERFILE_H
#define AIMANAGERFILE_H
#pragma once
#include "skillinfo.h"
#include "Monster.h"
#include <set>
#include "InventoryExDialog.h"
#include "OptionManager.h"
#include "AiSetManager.h"
#include "../ImageNumber.h"
enum prtected_enum_config
{
	protected_life_first,   
	protected_neo_first,  
	protected_aco_first,    
	protected_life_second,  
	protected_neo_second,   
	protected_aco_second,   
	protected_life_third,
	Protected_Num,			
};
enum player_state   
{
	player_isin_idel,
	player_isin_dazuo,
	player_isin_acttact_monster,
};
struct SkillSet
{
	struct HelpSkillSet   
	{
		DWORD SkillZXTime;
		DWORD SkillHBTime;
		DWORD SkillYHTime;
		DWORD SkillPLTime;
		DWORD SkillCYTime;
		DWORD SkillJZTime;
		DWORD SkillTLTime;
		BOOL  SkillTLStatic;
		int   SkillTLPecent;
		DWORD m_SkillWzTime;

		DWORD m_AutoSkill1;
		DWORD m_AutoSkill2;
		DWORD m_AutoSkill3;
		DWORD m_AutoSkill4;
		DWORD m_AutoSkill5;
		DWORD m_AutoSkill6;

		DWORD h_AutoSkill1;
		DWORD h_AutoSkill2;
		DWORD h_AutoSkill3;
		DWORD h_AutoSkill4;
		DWORD h_AutoSkill5;
		DWORD h_AutoSkill6;

		DWORD SkillFUMOTime;
	};
	HelpSkillSet m_HelpSkillSet;
	SkillSet()
	{
		memset(&m_HelpSkillSet,0,sizeof(HelpSkillSet));
	}
};
struct sGameActtactMonsterSet   
{
	struct AUTOACCTMONSTER		
	{
		BOOL Auto;
		int x;
		int y;
		int r;
		WORD LastMap;
	} ;
	struct AUTOPET   
	{
		BOOL AutoPet;
		int	HungerPecent;
		int	PressKey0;
	};
	struct AUTOSKILL	
	{
		BOOL Auto1;		
		int PressKey1;	
		BOOL Auto2;		
		int PressKey2;	
	};
	sGameActtactMonsterSet()
	{
		memset(&AutoActtactMonsterSet,0,sizeof(AUTOACCTMONSTER));
		memset(&AutoPet,0,sizeof(AUTOPET));
		memset(&AutoSkill,0,sizeof(AUTOSKILL));
	}
	AUTOACCTMONSTER AutoActtactMonsterSet;
	AUTOPET  AutoPet;
	AUTOSKILL AutoSkill;
};

class CObject;
class CPlayerAI       
{
	friend class CAiSetManager;
public:
	CPlayerAI(void);
	virtual ~CPlayerAI(void);

	static CPlayerAI& getinstance();  
	//HANDLE m_threadHandle;   	
	VECTOR3 Heropos;
	SkillSet   m_SkillSet;	
	sGameActtactMonsterSet m_AttactedSet;
	
	bool TestPeace;
	void ProcessTestPeace();
	void ToggleTestPeace(){ TestPeace = !TestPeace; }

	void ProcessCheckOutOfBound();
	void BeforeMapChange();
	void ClearBeforeMapChange();
	void threadProcEnable();
	void threadProcDisable();
	void Process();
	void ProcessLookMonster();
	bool StartLookMonster;
	void StartLookMonsterNew(bool bFlag){StartLookMonster=bFlag;}
	void ProcessLookMonsterNew(LPOBJECT currentMonster = NULL,double currentMonsterDistance = 0);
	void ProcessLookMonsterGamepad();
	void ProcessLookMonsterRunupMxo();
	void ProcessCariMonster();
	void ProcessQuickDialog();
	void ProcessAttackMacro(BOOL bFlag=FALSE);
	void ProcessSkillGelung();
	void ProcessSkillTeran();
	void ProcessAutoPotion();
	void ProcessLepasMatiBaru();
	//void ProcessLepasMati();	
	void ProcessTambahan();
	void ProcessBelasahPVP();
	void ProcessDaZuo();
	void ProcessSit();
	void ProcessMussang();
	void ProcessSitFailed();
	//void InitUtkProcessLepasMati();
	void CheckingSkill();
	void CheckAreaProcess();
	void CheckKyunkong2();
	void AutoCallPet();
	void AutoEatPet();
	void AutoUseBuff();
	void AutoUseBuff2();
	void HideObjectGamein();
	void HideObject();	
	void EndingDaZuo();
	void ExecSit();
	//void MovePoint();
	void UseTuNa();
	//void UseBaHuan();
	//void UseLongXiang();
	//void UseJiuYing();
	//void UseHaMa();
	//void UseTianMo();
	//void UseXiXing();
	//void UseJinGang();
	//void UseShenZhao();
	//void UseJiuYang();
	//void UseTaiXuan();
	//void UseBeiMing();
	//void UseFuMo();
	void InitRangeArea();
	BOOL IsEventAttackable(CObject* pTargetObj);
	void OnAttack(CObject* pTargetObj,bool bMugong=false);
	DWORD dwRangeArea;
	void SetAIPosition();
	void ToggleAIKey();
	void SaveTemp();
	void SetAIEnable();
	void SetAIDisable();
	void SetPoint();
	void UsePhAndSx(CMonster* pMonster,double distance1=0);
	BOOL CheckPohunOnObject(CMonster* pMonster);
	void UseMuGongSkill(DWORD MugongIdx);
	//void InitNotMainSkill();
	void DufualtUsekyungGong2();
//	void GoToArea(){bReturnToArea=TRUE;}	
	void StartAutoAttrPalyProcess(CObject *AttrPlayer);
	void StartSitFailedCheck();
	void UseQuickItem( DOUBLE minValue,DOUBLE maxValue ,DWORD curtime,int type);
	void SetOpen(BOOL val){AISETMGR->GetGameAI()->sPage5.pbAutoLeveling = val;}
	//void SetNotMainSkillOpen(DWORD StatusIconNum,BOOL check);     
	//void SetNotMainSkillClose(DWORD StatusIconNum);  
	void SetRunWay(BOOL val){m_IsRunWay = val;}
	void SetNeedSitNeutral(){m_IsNeedSit=FALSE;}
	void SetAttrPlayerID(DWORD val){AttackerID=val;}
	void SetFistRun(BOOL val){IsFirstRun = val;}
	void SetCanUse(BOOL val){m_CanUse = val;}
	void SetSkillNextFlag(DWORD Flag);
	void AttactMonster(LPOBJECT lpObj,double distance1);
	void ProcessReviving2();
	void StartRev2();
	void StopPlease(bool bFlag){m_StopPlease=bFlag;}
	void HitByMonsterCount(){m_MonsterAttrTime=m_MonsterAttrTime+1;}
	DWORD DeathCounterUp(){return m_DeathCount=m_DeathCount+1;}
	DWORD GetDeathCount(){return m_DeathCount;}

	void PM_SetNextAutoTarget();

	//void ProcessCheckReviveDialog();

	CObject *  pAttacker;
	CMugongBase * CheckHeightOrLow(int SkillIdx1,int SkillIdx2);
	CMugongBase * CheckHeightOrLowForOptionDlg(int SkillIdx1,int SkillIdx2);

	CMugongBase * CheckHeightOrLowTeran(int SkillIdx1,int SkillIdx2,int SkillIdx3,int SkillIdx4,int SkillIdx5,int SkillIdx6,int SkillIdx7);

	//DWORD m_threadId;
	DWORD m_TimeForProtect[Protected_Num];
	DWORD m_SkillTimeLast[5];
	DWORD m_SkillNextFlag[5];
	DWORD m_TimeForActtact;			
	DWORD m_TimeForTuna;			
	DWORD m_TimeForDaZuo;
	DWORD dzCurTime;
	DWORD m_ReMoveTime;
	DWORD m_MovePoint;
	DWORD m_ReSelectTime;
	DWORD m_TimeForRevive;
	DWORD m_TimeForNotMainSkillUse;
	DWORD m_TimeForSkillUse;
	DWORD m_TimeHeroDead;
	DWORD m_ReBuildTime;
	DWORD TimeNow;

	DWORD m_DeathCount;

	DWORD SkillTimeBHJG;
	DWORD SkillTimeBHJGUse;

	DWORD SkillTimeLXSZ;
	DWORD SkillTimeLXSZUse;

	DWORD SkillTimeJY;
	DWORD SkillTimeJYUse;

	DWORD SkillTimeHMFM;
	DWORD SkillTimeHMFMUse;

	DWORD SkillTimeTMTX;
	DWORD SkillTimeTMTXUse;

	DWORD SkillTimeXX;
	DWORD SkillTimeXXUse;

	DWORD SkillTimeBM;
	DWORD SkillTimeBMUse;

	DWORD SkillTime;

	DWORD m_GradeTime;
	DWORD m_nCount;
	DWORD m_nSitCount;
	DWORD m_dwSitStartTime;
	DWORD AttackerID;
	DWORD GetAttrPlayerID(){return AttackerID;};
	DWORD m_dwRevStartTime2;
	DWORD m_TimeForHideObject;

	BOOL ReBuildIsMovePoint;
	BOOL m_NoAction;
	BOOL m_CheckA;
	BOOL m_CheckB;
	BOOL m_CheckBHOrJG;	
	BOOL m_CheckLXOrSZ;	
	BOOL m_CheckJY;	
	BOOL m_CheckHMOrFM;	
	BOOL m_CheckTMOrTX;	
	BOOL m_CheckXX;	
	BOOL h_CheckBM;	
	BOOL m_CanUse;	
	BOOL DobuleSkillFlg;
	BOOL NeedDaZuo();
	BOOL CanEndingDaZuo();
	BOOL m_IsNeedSit;
	//BOOL bReturnToArea;
	BOOL IsRun;
	BOOL m_IsNeedHelpSkill;	
	BOOL m_bSitProcessing;
	BOOL ReadyForRelive;	
	BOOL m_IsRunWay;
	//BOOL ChecSkill();
	BOOL CheckKyunKong;
	BOOL IsOpen(){return AISETMGR->GetGameAI()->sPage5.pbAutoLeveling;}
	BOOL IsCanUse(){return m_CanUse;}
	BOOL PlayerUseSkill(CSkillInfo* pInfo);
	BOOL FindSkillStatusFLg(DWORD dwFlg, CMonster* pMonster);
	BOOL phFlag;

	BOOL m_bRevProcessing2;

	

	BOOL m_StopPlease;

	//BOOL UseHunXue();
	BOOL NewTeran(DWORD dwSkillIdx);

	bool CheckQuickBar(DWORD dwQuickSlot,DWORD dwNextFlag);
	void UntickAfterCheckingQuickBar(DWORD dwSlot);
	
	int	m_nRevCount2;
	int	m_nCount2;

	int m_ReviveCount;
	int m_playerstate;

	int m_MonsterAttrTime;
	int m_PlayerDealTime;  
	BOOL  IsFirstRun; 
	BOOL  IsAutoAttr; 
	MSG_SHOPITEM_USE_SYN PetUseMsg;
	
	DOUBLE GetLife();
	DOUBLE GetShield();
	DOUBLE GetNaeRyuk();

	DWORD dwCheckAreaTimeTick;

	bool HoldGelung;
	//DOUBLE GetObjLife(LPOBJECT lpObj);

	void InitTestMonster();
	void ProcessLookMonsterDEV(LPOBJECT currentMonster = NULL,double currentMonsterDistance = 0);


	DWORD CheckHeroIdle();
	bool bImmortal;
	DWORD bImmortalTimeCount;

	bool bReviveDialogIsShowingSwitch;
	bool bReviveDialogIsShowing;
	DWORD dwReviveDialogTimeTick;
	void ForceShowReviveDialog();

	DWORD	dwHeroID;

	void YouStop(CObject* pObject);

	VECTOR3 vPos;

	VECTOR3 GetvPos(){return vPos;}

	bool LockIntoPvp;
	bool OutOfRangePvp;

	CObject* pAttackerObject;

	bool IfMonster();
	void CheckAreaProcessSmallPoint();
	CImageNumber	m_ImageNumber;
private:
	CObject* m_pTabSelectedObject;

	

};
EXTERNGLOBALTON(CPlayerAI);
#define sCPlayerAI CPlayerAI::getinstance()
#endif