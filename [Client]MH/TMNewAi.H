#ifndef AISTRUCTFILE_H
#define AISTRUCTFILE_H
#pragma once

#include "./interface/cDialog.h"
#include "./Interface/cTabDialog.h"
#define AISETMGR USINGTON(CAiSetManager)
class CAiSetManager;
class cGuageBar;
struct sGAMEAI
{
	struct PAGE5
	{
		BOOL pbAutoLeveling;
		int pbSpawnHere;

		BOOL cbAreaXY;
		BOOL cbAutoRespawn;
		BOOL cbAutoRespawnTimeLimit;
		BOOL cbAutoSitHP;
		BOOL cbAutoSitMP;
		BOOL cbAutoSitSP;
		BOOL cbSitNoTarget;
		BOOL cbSitAttacked;
		BOOL cbNoMonsterWall;

		int spPosX;
		int spPosY;
		int spRange;
		int spSitHP;
		int spSitMP;
		int spSitSP;
		int spReviveTimeLimit;
		WORD spReviveTime;
		WORD spSitAttacked;

		BOOL cbIgnoreBoss;

	};
	struct PAGE6
	{
		BOOL bSkill1;
		BOOL bSkill2;
		BOOL bSkill3;
		BOOL bSkill4;
		BOOL bSkill5;
		BYTE bSkill1time;
		BYTE bSkill2time;
		BYTE bSkill3time;
		BYTE bSkill4time;
		BYTE bSkill5time;
		BYTE bSkill1page;
		BYTE bSkill2page;
		BYTE bSkill3page;
		BYTE bSkill4page;
		BYTE bSkill5page;
		BYTE bSkill1key;
		BYTE bSkill2key;
		BYTE bSkill3key;
		BYTE bSkill4key;
		BYTE bSkill5key;
		BOOL bHelpSkill1;
		BOOL bHelpSkill2;
		BOOL bHelpSkill3;
		BOOL bHelpSkill4;
		BOOL bHelpSkill5;
		BOOL bHelpSkill6;
		BOOL bHelpSkill7;
		BOOL bHelpSkill8;
		BOOL bHelpSkill9;
		BOOL bHelpSkill10;
		BYTE bTunaSpin;
	};
	struct PAGE7
	{
		BOOL ProtectLifeOpen1;
		BOOL ProtectLifeOpen2;
		BOOL ProtectLifeOpen3;
		int  ProtectLifePecent1;
		int  ProtectLifePecent2;
		int  ProtectLifePecent3;
		DWORD ProtectLifeTime1;
		DWORD ProtectLifeTime2;
		DWORD ProtectLifeTime3;
		WORD ProtectLifeKey1;
		WORD ProtectLifeKey2;
		WORD ProtectLifeKey3;
		WORD ProtectLifePage1;
		WORD ProtectLifePage2;
		WORD ProtectLifePage3;
		BOOL ProtectMagicOpen1;
		BOOL ProtectMagicOpen2;
		int  ProtectMagicPecent1;
		int  ProtectMagicPecent2;
		DWORD ProtectMagicTime1;
		DWORD ProtectMagicTime2;
		WORD ProtectMagicKey1;
		WORD ProtectMagicKey2;
		WORD ProtectMagicPage1;
		WORD ProtectMagicPage2;
		BOOL ProtectShieldOpen1;
		BOOL ProtectShieldOpen2;
		int  ProtectShieldPecent1;
		int  ProtectShieldPecent2;
		DWORD ProtectShieldTime1;
		DWORD ProtectShieldTime2;
		WORD ProtectShieldKey1;
		WORD ProtectShieldKey2;
		WORD ProtectShieldPage1;
		WORD ProtectShieldPage2;
	};
	struct PAGE8
	{
		BOOL m_AutoSkill1;
		BOOL m_AutoSkill2;
		BOOL m_AutoSkill3;
		BOOL m_AutoSkill4;
		BOOL m_AutoSkill5;
		BOOL m_AutoSkill6;
		BOOL h_AutoSkill1;
		BOOL h_AutoSkill2;
		BOOL h_AutoSkill3;
		BOOL h_AutoSkill4;
		BOOL h_AutoSkill5;
		BOOL h_AutoSkill6;

		BOOL isReConnServer;
		BOOL isAutoCallPet;
		BOOL isAutoUseBuffItem;
		BOOL isAutoCloseGetItemWin;
		BOOL isAutoJoinGroup;
		BOOL isAutoWant;
		int  WantMoney;

		BOOL isAutoAttrPlay;

		BOOL FuoMoo;

		BOOL cbReconnectMap;
		WORD spReconnectMap;

		WORD spPetEatPercent;
	};

	PAGE5	sPage5;
	PAGE6	sPage6;
	PAGE7	sPage7;
	PAGE8	sPage8;
};
class CAiSetManager  //: public cTabDialog
{
protected:
	sGAMEAI			m_GameAI;

public:
	CAiSetManager();
	virtual ~CAiSetManager();
	void InitForGameIn();

	void SetDefaultOption();

	BOOL LoadGameAI();
	void SaveGameOptionAI();
	sGAMEAI* GetGameAI() { return &m_GameAI; }
	void SetGameAI(sGAMEAI* pOptionAI);

	void SaveData();
	void ReadData();

	void TestQuickKey(DWORD AutoSkillBtnIdx);

	bool FindErrorPath();
	bool FindErrorPathFromAISet();
	void SetErrorPathInfo(BOOL bDisable);



	//friend class COptionDialog;
};
EXTERNGLOBALTON(CAiSetManager);
#endif