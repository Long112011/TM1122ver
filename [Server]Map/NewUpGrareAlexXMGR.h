#pragma once
#include <map> 
#include "..\[CC]Header\GameResourceStruct.h"
#include "MHFile.h"

#define NEWUPGRAREALEXX_MGR NewUpGrareAlexXMGR::GetInstance()
class CPlayer;

enum RateFull
{
	UpGradeRate,
	MoneyRate,
	GENGOL_MINCHUB_CHERYUK_SIMMEK,
	LITE_SHIELD_NAERYUK_PHYDEF,
	CRITICAL,
	MELEE_RANGE_ATTACK,
	ELEMENT_ATTACK_REGIST,
	ITEM_MATERIAL_LV_01_PERCENT,
	ITEM_MATERIAL_LV_02_PERCENT,
	ITEM_MATERIAL_LV_03_PERCENT,
	ITEM_MATERIAL_LV_04_PERCENT,
	eMaxSetUp,
};

struct ALEXX_UPGRADESETUP
{
	float SetupGrade_AlexX[eMaxSetUp];
	ALEXX_UPGRADESETUP()
	{
		memset(&SetupGrade_AlexX, 0, sizeof(eMaxSetUp));
	}
};
class NewUpGrareAlexXMGR
{
public:
	NewUpGrareAlexXMGR(void);
	~NewUpGrareAlexXMGR(void);
	GETINSTANCE(NewUpGrareAlexXMGR);
	void NetworkMsgParse(DWORD dwConnectionIndex, BYTE Protocol, void* pMsg);
	void Release();
	BOOL IsCanUpGrareItem(DWORD wItemIdx, eITEM_KINDBIT bits);
	bool Random_Succeeds(float targetValue, float chanceOutOf);
	void WhatError_num(DWORD error, DWORD dwObjectID);
	DWORD WhatLvStone(DWORD lv);
	ALEXX_UPGRADESETUP AlexXUpGradeConfig;
	BOOL LoadUpGradeItem_info();
};
