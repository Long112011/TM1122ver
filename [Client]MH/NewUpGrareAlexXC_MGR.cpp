#include "stdafx.h"
#include "NewUpGrareAlexXC_MGR.h"


NewUpGrareAlexXC_MGR::NewUpGrareAlexXC_MGR(void)
{
	
}

NewUpGrareAlexXC_MGR::~NewUpGrareAlexXC_MGR(void)
{

}
void NewUpGrareAlexXC_MGR::LoadUpGradeItem_info()
{
	CMHFile file;
	char szBuf[256] = { 0, };

	if (!file.Init("./Resource/AlexX_UpGradeItem_info.bin", "rb"))
		return;

	while (!file.IsEOF())
	{
		strcpy(szBuf, strupr(file.GetString()));

		//if (szBuf[0] == ';') continue;

		if (strcmp(szBuf, "#ALEXX_UPGRADE_RATE") == 0)
		{
			AlexXUpGradeConfig.SetupGrade_AlexX[UpGradeRate] = file.GetFloat();
		}
		if (strcmp(szBuf, "#ALEXX_MONEY_RATE") == 0)
		{
			AlexXUpGradeConfig.SetupGrade_AlexX[MoneyRate] = file.GetFloat();
		}
		if (strcmp(szBuf, "#ALEXX_ITEM_GENGOL_MINCHUB_CHERYUK_SIMMEK") == 0)
		{
			AlexXUpGradeConfig.SetupGrade_AlexX[GENGOL_MINCHUB_CHERYUK_SIMMEK] = file.GetFloat();
		}
		if (strcmp(szBuf, "#ALEXX_ITEM_LITE_SHIELD_NAERYUK_PHYDEF") == 0)
		{
			AlexXUpGradeConfig.SetupGrade_AlexX[LITE_SHIELD_NAERYUK_PHYDEF] = file.GetFloat();
		}
		if (strcmp(szBuf, "#ALEXX_ITEM_CRITICAL") == 0)
		{
			AlexXUpGradeConfig.SetupGrade_AlexX[CRITICAL] = file.GetFloat();
		}
		if (strcmp(szBuf, "#ALEXX_ITEM_MELEE_RANGE_ATTACK") == 0)
		{
			AlexXUpGradeConfig.SetupGrade_AlexX[MELEE_RANGE_ATTACK] = file.GetFloat();
		}
		if (strcmp(szBuf, "#ALEXX_ITEM_ELEMENT_ATTACK_REGIST") == 0)
		{
			AlexXUpGradeConfig.SetupGrade_AlexX[ELEMENT_ATTACK_REGIST] = file.GetFloat();
		}
		if (strcmp(szBuf, "#ALEXX_ITEM_MATERIAL_LV_01_PERCENT") == 0)
		{
			AlexXUpGradeConfig.SetupGrade_AlexX[ITEM_MATERIAL_LV_01_PERCENT] = file.GetFloat();
		}
		if (strcmp(szBuf, "#ALEXX_ITEM_MATERIAL_LV_02_PERCENT") == 0)
		{
			AlexXUpGradeConfig.SetupGrade_AlexX[ITEM_MATERIAL_LV_02_PERCENT] = file.GetFloat();
		}
		if (strcmp(szBuf, "#ALEXX_ITEM_MATERIAL_LV_03_PERCENT") == 0)
		{
			AlexXUpGradeConfig.SetupGrade_AlexX[ITEM_MATERIAL_LV_03_PERCENT] = file.GetFloat();
		}
		if (strcmp(szBuf, "#ALEXX_ITEM_MATERIAL_LV_04_PERCENT") == 0)
		{
			AlexXUpGradeConfig.SetupGrade_AlexX[ITEM_MATERIAL_LV_04_PERCENT] = file.GetFloat();
		}
	}
	file.Release();
}
