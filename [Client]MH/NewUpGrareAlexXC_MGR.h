#pragma once
#include <map> 
#include "..\[CC]Header\GameResourceStruct.h"
#include "MHFile.h"

#define NEWUPGRAREALEXX_CMGR NewUpGrareAlexXC_MGR::GetInstance()

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
class NewUpGrareAlexXC_MGR
{
public:
	NewUpGrareAlexXC_MGR(void);
	~NewUpGrareAlexXC_MGR(void);
	GETINSTANCE(NewUpGrareAlexXC_MGR);
	ALEXX_UPGRADESETUP AlexXUpGradeConfig;
	void LoadUpGradeItem_info();
};