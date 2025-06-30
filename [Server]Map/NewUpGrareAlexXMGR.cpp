#include "stdafx.h"
#include "NewUpGrareAlexXMGR.h"
#include "MapDBMsgParser.h"
#include "Player.h"
#include "ItemManager.h"
#include "Network.h"
#include "UserTable.h"
#include "Console.h"
#include <random>
#include <ctime>
#include "ItemDrop.h"

NewUpGrareAlexXMGR::NewUpGrareAlexXMGR(void)
{
	SYSTEMTIME time;
	GetLocalTime(&time);
}

NewUpGrareAlexXMGR::~NewUpGrareAlexXMGR(void)
{

}
void NewUpGrareAlexXMGR::Release()
{

}
void NewUpGrareAlexXMGR::NetworkMsgParse(DWORD dwConnectionIndex, BYTE Protocol, void* pMsg)
{
	MSGBASE* pTempMsg = (MSGBASE*)pMsg;
	switch (pTempMsg->Protocol)
	{
	case MP_NEWUPGRARE_ALEXX_NEWUPGRARE_SYN:
	{
		MSG_NEWYPGRARE_ALEXX* pmsg = (MSG_NEWYPGRARE_ALEXX*)pMsg;
		g_Console.LOG(4, "MSG_NEWYPGRARE_ALEXX");
		CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser(pmsg->dwObjectID);
		if (NULL == pPlayer) return;

		DWORD WhatError = 0;
		DWORD result = 0;

		if (TP_WEAR_START <= pmsg->ItemPosition && pmsg->ItemPosition < TP_WEAR_END)
		{
			WhatError = 1;
			WhatError_num(WhatError, pmsg->dwObjectID);
			return;
		}
		if (TP_SHOPINVEN_START <= pmsg->ItemPosition && pmsg->ItemPosition < TP_SHOPINVEN_END)
		{
			WhatError = 2;
			WhatError_num(WhatError, pmsg->dwObjectID);
			return;
		}

		const ITEMBASE* pTargetBase = ITEMMGR->GetItemInfoAbsIn(pPlayer, pmsg->ItemPosition);
		if (!pTargetBase)
		{
			WhatError = 3;
			g_Console.LOG(4, "WhatError 1");
			WhatError_num(WhatError, pmsg->dwObjectID);
			return;
		}

		ITEM_INFO* pTargetItem = ITEMMGR->GetItemInfo(pmsg->ItemDBIdx);
		float increase_factor = 1.20;
		float full_naterial_X = 0.0;
		float full_naterial_1 = 0.0;
		float full_naterial_2 = 0.0;
		float full_naterial_3 = 0.0;
		float full_naterial_4 = 0.0;
		float full_stone = 0.0;
		bool x_stone_go = false;

		if (pPlayer->GetMoney() < pmsg->money)
		{
			WhatError = 9;
			WhatError_num(WhatError, pmsg->dwObjectID);
			return;
		}

		DWORD real_lv_item = pTargetItem->LimitLevel;
		DWORD real_fullMoney = ITEMMGR->GetUpGradeItemPercentList(real_lv_item)->Money;
		DWORD Xreal_fullMoney = real_fullMoney * pow(AlexXUpGradeConfig.SetupGrade_AlexX[MoneyRate], pTargetBase->ItemGradeAlexX);

		if (Xreal_fullMoney == pmsg->money)
		{
			WhatError = 10;
			WhatError_num(WhatError, pmsg->dwObjectID);
			return;
		}

		eITEM_KINDBIT bits = (eITEM_KINDBIT)pTargetItem->ItemKind;
		if (!IsCanUpGrareItem(pmsg->ItemDBIdx, bits))
		{
			WhatError = 4;
			g_Console.LOG(4, "WhatError 2");
			WhatError_num(WhatError, pmsg->dwObjectID);
			return;
		}

		if (pmsg->Material_count_1 == 0 && pmsg->Material_count_2 == 0 && pmsg->Material_count_3 == 0 && pmsg->Material_count_4 == 0)
		{
			WhatError = 6;
			WhatError_num(WhatError, pmsg->dwObjectID);
			return;
		}

		const ITEMBASE* pTargetBase_M_1 = ITEMMGR->GetItemInfoAbsIn(pPlayer, pmsg->Material_count_1_POS);
		const ITEMBASE* pTargetBase_M_2 = ITEMMGR->GetItemInfoAbsIn(pPlayer, pmsg->Material_count_2_POS);
		const ITEMBASE* pTargetBase_M_3 = ITEMMGR->GetItemInfoAbsIn(pPlayer, pmsg->Material_count_3_POS);
		const ITEMBASE* pTargetBase_M_4 = ITEMMGR->GetItemInfoAbsIn(pPlayer, pmsg->Material_count_4_POS);
		if (!pTargetBase_M_1 && !pTargetBase_M_2 && !pTargetBase_M_3 && !pTargetBase_M_4)
		{
			WhatError = 7;
			WhatError_num(WhatError, pmsg->dwObjectID);
			return;
		}

		if (pmsg->Material_count_1 == 0)
		{
			full_naterial_1 = 0.0;
		}
		else
		{
			ITEM_INFO* pTargetItem_M_1 = ITEMMGR->GetItemInfo(pmsg->Material_count_1_DB);
			if (!pTargetItem_M_1->ItemIdx == eAlexXItem_ItemUpGradeMaterial_lv1)
			{
				WhatError = 5;
				WhatError_num(WhatError, pmsg->dwObjectID);
				return;
			}
			//full_naterial_1 = 1.5 * pmsg->Material_count_1;
			full_naterial_1 = AlexXUpGradeConfig.SetupGrade_AlexX[ITEM_MATERIAL_LV_01_PERCENT] * pmsg->Material_count_1;
			full_naterial_X += full_naterial_1;

			if (EI_TRUE != ITEMMGR->DiscardItem(pPlayer, pmsg->Material_count_1_POS, pmsg->Material_count_1_DB, pmsg->Material_count_1))
			{
				WhatError = 8;
				WhatError_num(WhatError, pmsg->dwObjectID);
				return;
			}
			else
			{
				MSG_NEWYPGRARE_ALEXX_DELITEM msg;
				msg.Category = MP_NEWUPGRARE_ALEXX;
				msg.Protocol = MP_NEWUPGRARE_ALEXX_NEWUPGRARE_DELITEM;
				msg.dwObjectID = pPlayer->GetID();
				msg.ItemIdx = pmsg->Material_count_1_DB;
				msg.Pos = pmsg->Material_count_1_POS;
				msg.count = pmsg->Material_count_1;
				pPlayer->SendMsg(&msg, sizeof(msg));
			}
		}

		if (pmsg->Material_count_2 == 0)
		{
			full_naterial_2 = 0.0;
		}
		else
		{
			ITEM_INFO* pTargetItem_M_2 = ITEMMGR->GetItemInfo(pmsg->Material_count_2_DB);
			if (!pTargetItem_M_2->ItemIdx == eAlexXItem_ItemUpGradeMaterial_lv2)
			{
				WhatError = 5;
				WhatError_num(WhatError, pmsg->dwObjectID);
				return;
			}
			full_naterial_2 = AlexXUpGradeConfig.SetupGrade_AlexX[ITEM_MATERIAL_LV_02_PERCENT] * pmsg->Material_count_2;
			full_naterial_X += full_naterial_2;

			if (EI_TRUE != ITEMMGR->DiscardItem(pPlayer, pmsg->Material_count_2_POS, pmsg->Material_count_2_DB, pmsg->Material_count_2))
			{
				WhatError = 8;
				WhatError_num(WhatError, pmsg->dwObjectID);
				return;
			}
			else
			{
				MSG_NEWYPGRARE_ALEXX_DELITEM msg;
				msg.Category = MP_NEWUPGRARE_ALEXX;
				msg.Protocol = MP_NEWUPGRARE_ALEXX_NEWUPGRARE_DELITEM;
				msg.dwObjectID = pPlayer->GetID();
				msg.ItemIdx = pmsg->Material_count_2_DB;
				msg.Pos = pmsg->Material_count_2_POS;
				msg.count = pmsg->Material_count_2;
				pPlayer->SendMsg(&msg, sizeof(msg));
			}
		}

		if (pmsg->Material_count_3 == 0)
		{
			full_naterial_3 = 0.0;
		}
		else
		{
			ITEM_INFO* pTargetItem_M_3 = ITEMMGR->GetItemInfo(pmsg->Material_count_3_DB);
			if (!pTargetItem_M_3->ItemIdx == eAlexXItem_ItemUpGradeMaterial_lv3)
			{
				WhatError = 5;
				WhatError_num(WhatError, pmsg->dwObjectID);
				return;
			}
			full_naterial_3 = AlexXUpGradeConfig.SetupGrade_AlexX[ITEM_MATERIAL_LV_03_PERCENT] * pmsg->Material_count_3;
			full_naterial_X += full_naterial_3;

			if (EI_TRUE != ITEMMGR->DiscardItem(pPlayer, pmsg->Material_count_3_POS, pmsg->Material_count_3_DB, pmsg->Material_count_3))
			{
				WhatError = 8;
				WhatError_num(WhatError, pmsg->dwObjectID);
				return;
			}
			else
			{
				MSG_NEWYPGRARE_ALEXX_DELITEM msg;
				msg.Category = MP_NEWUPGRARE_ALEXX;
				msg.Protocol = MP_NEWUPGRARE_ALEXX_NEWUPGRARE_DELITEM;
				msg.dwObjectID = pPlayer->GetID();
				msg.ItemIdx = pmsg->Material_count_3_DB;
				msg.Pos = pmsg->Material_count_3_POS;
				msg.count = pmsg->Material_count_3;
				pPlayer->SendMsg(&msg, sizeof(msg));
			}
		}

		if (pmsg->Material_count_4 == 0)
		{
			full_naterial_4 = 0.0;
		}
		else
		{
			ITEM_INFO* pTargetItem_M_4 = ITEMMGR->GetItemInfo(pmsg->Material_count_4_DB);
			if (!pTargetItem_M_4->ItemIdx == eAlexXItem_ItemUpGradeMaterial_lv4)
			{
				WhatError = 5;
				WhatError_num(WhatError, pmsg->dwObjectID);
				return;
			}
			full_naterial_4 = AlexXUpGradeConfig.SetupGrade_AlexX[ITEM_MATERIAL_LV_04_PERCENT] * pmsg->Material_count_4;
			full_naterial_X += full_naterial_4;

			if (EI_TRUE != ITEMMGR->DiscardItem(pPlayer, pmsg->Material_count_4_POS, pmsg->Material_count_4_DB, pmsg->Material_count_4))
			{
				WhatError = 8;
				WhatError_num(WhatError, pmsg->dwObjectID);
				return;
			}
			else
			{
				MSG_NEWYPGRARE_ALEXX_DELITEM msg;
				msg.Category = MP_NEWUPGRARE_ALEXX;
				msg.Protocol = MP_NEWUPGRARE_ALEXX_NEWUPGRARE_DELITEM;
				msg.dwObjectID = pPlayer->GetID();
				msg.ItemIdx = pmsg->Material_count_4_DB;
				msg.Pos = pmsg->Material_count_4_POS;
				msg.count = pmsg->Material_count_4;
				pPlayer->SendMsg(&msg, sizeof(msg));
			}
		}

		if (pmsg->Stone_count == 0)
		{
			full_stone = 0.0;
		}
		else
		{
			ITEM_INFO* pTargetItem_stone = ITEMMGR->GetItemInfo(pmsg->Stone_DB);
			if (!pTargetItem_stone->ItemIdx == eAlexXItem_ItemProtectUpGrade30
				|| !pTargetItem_stone->ItemIdx == eAlexXItem_ItemProtectUpGrade50
				|| !pTargetItem_stone->ItemIdx == eAlexXItem_ItemProtectUpGrade100)
			{
				WhatError = 10;
				WhatError_num(WhatError, pmsg->dwObjectID);
				return;
			}
			else
			{
				if (pTargetItem_stone->ItemIdx == eAlexXItem_ItemProtectUpGrade100)
				{
					x_stone_go = true;
				}
				full_stone = WhatLvStone(pmsg->Stone_DB);
			}

			if (EI_TRUE != ITEMMGR->DiscardItem(pPlayer, pmsg->Stone_POS, pmsg->Stone_DB, pmsg->Stone_count))
			{
				WhatError = 8;
				WhatError_num(WhatError, pmsg->dwObjectID);
				return;
			}
			else
			{
				MSG_NEWYPGRARE_ALEXX_DELITEM msg;
				msg.Category = MP_NEWUPGRARE_ALEXX;
				msg.Protocol = MP_NEWUPGRARE_ALEXX_NEWUPGRARE_DELITEM;
				msg.dwObjectID = pPlayer->GetID();
				msg.ItemIdx = pmsg->Stone_DB;
				msg.Pos = pmsg->Stone_POS;
				msg.count = pmsg->Stone_count;
				pPlayer->SendMsg(&msg, sizeof(msg));
			}
		}

		pPlayer->SetMoney(pmsg->money, MONEY_SUBTRACTION);

		char line[128];
		sprintf_s(line, sizeof(line), "Material_count_1 : %d", pmsg->Material_count_1);
		g_Console.LOG(4, line);
		sprintf_s(line, sizeof(line), "Material_count_2 : %d", pmsg->Material_count_2);
		g_Console.LOG(4, line);
		sprintf_s(line, sizeof(line), "Material_count_3 : %d", pmsg->Material_count_3);
		g_Console.LOG(4, line);
		sprintf_s(line, sizeof(line), "Material_count_4 : %d", pmsg->Material_count_4);
		g_Console.LOG(4, line);
		sprintf_s(line, sizeof(line), "Stone_count : %d %d", pmsg->Stone_count, pmsg->Stone_DB);
		g_Console.LOG(4, line);

		//DWORD real_lv_item = pTargetItem->LimitLevel;
		float real_fullPercen = ITEMMGR->GetUpGradeItemPercentList(real_lv_item)->MaxPercent;
		float Xreal_fullPercen = real_fullPercen * pow(AlexXUpGradeConfig.SetupGrade_AlexX[UpGradeRate], pTargetBase->ItemGradeAlexX);

		float targetValue = full_naterial_X;
		float chanceOutOf = Xreal_fullPercen;

		if (Random_Succeeds(targetValue, chanceOutOf))/*     */
		{
			g_Console.LOG(4, "Succeeds");
			DWORD ItemGradeP = pTargetBase->ItemGradeAlexX + 1;
			ItemGradeAlexXUpdate(pPlayer->GetID(), pTargetBase->dwDBIdx, ItemGradeP);
			result = 1;
		}
		else/*        */
		{
			if (pTargetBase->ItemGradeAlexX > 10)
			{
				if (!x_stone_go)
				{
					std::random_device rd;
					std::mt19937 gen(rd());
					std::uniform_int_distribution<int> dist(1, 100 + full_stone);
					int randomNumber = dist(gen);
					int max;

					sprintf_s(line, sizeof(line), "randomNumber : %d", randomNumber);
					g_Console.LOG(4, line);

					if (pTargetBase->ItemGradeAlexX >= 10 && pTargetBase->ItemGradeAlexX <= 15)
					{
						max = 30;
					}
					else if (pTargetBase->ItemGradeAlexX >= 16 && pTargetBase->ItemGradeAlexX <= 25)
					{
						max = 50;
					}
					else if (pTargetBase->ItemGradeAlexX >= 26 && pTargetBase->ItemGradeAlexX <= 30)
					{
						max = 70;
					}

					if (randomNumber < max)
					{
						g_Console.LOG(4, "No Succeeds T T sad");
						result = 3;
						DWORD ItemGradeP = pTargetBase->ItemGradeAlexX - 1;
						ItemGradeAlexXUpdate(pPlayer->GetID(), pTargetBase->dwDBIdx, ItemGradeP);
						sprintf_s(line, sizeof(line), "ItemGradeP : %d", ItemGradeP);
						g_Console.LOG(4, line);
					}
					else
					{
						g_Console.LOG(4, "No Succeeds T T");
						result = 2;
					}
				}
				else
				{
					g_Console.LOG(4, "No Succeeds T T");
					result = 2;
				}
			}
			else
			{
				g_Console.LOG(4, "No Succeeds T T");
				result = 2;
			}
		}

		MSG_NEWYPGRARE_ALEXX_BACKGAME msg;
		msg.Category = MP_NEWUPGRARE_ALEXX;
		msg.Protocol = MP_NEWUPGRARE_ALEXX_NEWUPGRARE_ACK;
		msg.dwObjectID = pPlayer->GetID();
		msg.wTargetItemIdx = pTargetItem->ItemIdx;
		msg.WhatError = 0;
		msg.result = result;
		pPlayer->SendMsg(&msg, sizeof(msg));

		/*CItem* pOutItem = NULL;
		DeleteItem(pmsg->TargetPos, &pOutItem);
		if (pOutItem)
		ReLinkQuickPosition(pOutItem);*/

		/*if (pmsg->wData1 == 1)
		{
		ItemGradeAlexXUpdate(1, 5, 15);
		}*/

		/*MSG_SINGEDINFO msg;
		msg.Category = MP_NEWUPGRARE_ALEXX;
		msg.Protocol = MP_NEWUPGRARE_ALEXX_NEWUPGRARE_ACK;
		/*eSINGED rt;
		if (pmsg->eType<m_day)
		{//[    С ڵ ǰ  ǩ][2018/1/4]
		if (pPlayer->GetGoldMoney()<NeedGoldMoney)
		msg.eType = eNotMoney;
		else
		{
		rt = GiftGiving(pPlayer, pmsg->eType);
		if (rt == eOK)
		{
		SetSingedMsgInfo(pPlayer, msg);
		pPlayer->SetGoldMoney(NeedGoldMoney, eSinged);//[  ǩ ɹ  ۳ Ԫ  ][2018/1/4]
		LogGoldMoney(eLog_Singed, pPlayer->GetID(), pPlayer->GetGoldMoney() + NeedGoldMoney, 65533, pPlayer->GetGoldMoney(), NeedGoldMoney, 0, 0);
		}
		}
		}
		else if (pmsg->eType == m_day)
		rt = GiftGiving(pPlayer);//[    ǩ  ][2018/1/4]
		else if (pmsg->eType == eCompelete)
		rt = GiftGivingCompelet(pPlayer);//[  ǩ  ȡ][2018/1/8]
		else
		rt = eCanGetDay; //[ǩ     ڴ  ڵ ǰ      ǩ  ][2018/1/4]
		if (rt = eOK)
		SetSingedMsgInfo(pPlayer, msg);
		msg.eType = rt;
		pPlayer->SendMsg(&msg, sizeof(msg));*/
	}
	break;
	}
}
BOOL NewUpGrareAlexXMGR::IsCanUpGrareItem(DWORD wItemIdx, eITEM_KINDBIT bits)
{
	if (ITEMMGR->IsDupItem(wItemIdx))
	{
		return FALSE;
	}
	switch (bits)
	{
	case eEQUIP_ITEM_DRESS:
	{
		return TRUE;
	}
	break;
	case eEQUIP_ITEM_HAT:
	{
		return TRUE;
	}
	break;
	case eEQUIP_ITEM_SHOES:
	{
		return TRUE;
	}
	break;
	case eEQUIP_ITEM_WEAPON:
	{
		return TRUE;
	}
	break;
	case eEQUIP_ITEM_RING:
	{
		return TRUE;
	}
	break;
	case eEQUIP_ITEM_CAPE:
	{
		return TRUE;
	}
	break;
	case eEQUIP_ITEM_NECKLACE:
	{
		return TRUE;
	}
	break;
	case eEQUIP_ITEM_ARMLET:
	{
		return TRUE;
	}
	break;
	case eEQUIP_ITEM_BELT:
	{
		return TRUE;
	}
	//case eEQUIP_ITEM_EAR:
	//{
	//	return TRUE;
	//}
	//case eEQUIP_ITEM_BAR:
	//{
	//	return TRUE;
	//}
	}

	return FALSE;
}
bool NewUpGrareAlexXMGR::Random_Succeeds(float targetValue, float chanceOutOf)
{
	/*std::random_device rd;  //    ҧ random_device              ù
	std::mt19937 gen(rd()); //    ҧ Mersenne Twister engine    ¤ Ҩҡ random_device
	std::uniform_int_distribution<> dis(1, chanceOutOf); //    ҧ  á Ш  Ẻ   㹪 ǧ 1  ֧ chanceOutOf

	float randomValue = dis(gen); //    ҧ Ţ     ҡ  á Ш

	return randomValue <= targetValue; //    º  º   ͹ */

	float result = static_cast<float>(rand()) / RAND_MAX * chanceOutOf; //         ͡       㹪 ǧ 0-chanceOutOf

	char line[64];
	sprintf_s(line, sizeof(line), "result : %.4f", result);
	g_Console.LOG(4, line);
	sprintf_s(line, sizeof(line), "targetValue : %.4f", targetValue);
	g_Console.LOG(4, line);

	if (result <= targetValue)
	{
		return true; //      
	}
	else
	{
		return false; //         
	}
}
void NewUpGrareAlexXMGR::WhatError_num(DWORD error, DWORD dwObjectID)
{
	CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser(dwObjectID);
	if (NULL == pPlayer) return;

	MSG_NEWYPGRARE_ALEXX_BACKGAME msg;
	msg.Category = MP_NEWUPGRARE_ALEXX;
	msg.Protocol = MP_NEWUPGRARE_ALEXX_NEWUPGRARE_ACK;
	msg.dwObjectID = pPlayer->GetID();
	msg.wTargetItemIdx = 0;
	msg.WhatError = error;
	msg.result = 0;
	pPlayer->SendMsg(&msg, sizeof(msg));
}
DWORD NewUpGrareAlexXMGR::WhatLvStone(DWORD lv)
{
	DWORD ItemIdx;
	switch (lv)
	{
	case eAlexXItem_ItemProtectUpGrade30:
	{
		ItemIdx = 30;
	}
	break;
	case eAlexXItem_ItemProtectUpGrade50:
	{
		ItemIdx = 50;
	}
	break;
	case eAlexXItem_ItemProtectUpGrade100:
	{
		ItemIdx = 100;
	}
	break;
	}
	return ItemIdx;
}
BOOL NewUpGrareAlexXMGR::LoadUpGradeItem_info()
{
	CMHFile file;
	char szBuf[256] = { 0, };

	if (!file.Init("./Resource/AlexX_UpGradeItem_info.bin", "rb"))
		return FALSE;

	while (!file.IsEOF())
	{
		strcpy(szBuf, strupr(file.GetString()));

		if (szBuf[0] == ';') continue;

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
	return TRUE;
}