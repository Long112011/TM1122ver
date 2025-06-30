







// StatsCalcManager.cpp: implementation of the CStatsCalcManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "StatsCalcManager.h"
#include "GameResourceManager.h"

#ifdef _MHCLIENT_
#include "Hero.h"
#include "GameIn.h"
#include "ObjectManager.h"
#endif

#ifdef _MAPSERVER_
#include "Player.h"
#endif

#include "CharacterCalcManager.h"
#include "ItemManager.h"
#include "MussangManager.h"
#include "PetManager.h"

#include "TitanManager.h"

#include "TitanInventoryDlg.h"

#include "FameManager.h"
#include "NewUpGrareAlexXC_MGR.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction

//////////////////////////////////////////////////////////////////////

CStatsCalcManager::CStatsCalcManager()
{
	// 2014-11-20 ÁÙÊ±
	m_StoneItemList.Initialize(5);
}

CStatsCalcManager::~CStatsCalcManager()
{
	// 2014-11-20 ÁÙÊ±
	ITEM_INFO * pStoneInfo=NULL;
	m_StoneItemList.SetPositionHead();
	while(pStoneInfo=m_StoneItemList.GetData())
	{
		delete pStoneInfo;
	}
	m_StoneItemList.RemoveAll();
}

void CStatsCalcManager::Clear(player_calc_stats * pStats)
{
	memset(pStats, 0, sizeof(player_calc_stats));
}

#ifdef _JAPAN_LOCAL_

float ItemApplyRate[] = 
{
	1,
		0.5f,
		0.4f,

		0.3f,
		0.2f,
		0.1f,
		0.0f,
};

#else

float ItemApplyRate[] = 
{
	1,
		0.6f,
		0.55f,
		0.5f,
		0.45f,
		0.4f,
		0.35f,
		0.30f,
		0.3f,

		0.25f,
		0.2f,
		0.15f,
		0.1f,
		0.05f,

		0.f,
};

#endif

// ÀåÂøÃ¢¿¡ ¾ÆÀÌÅÛ ÀåÂø,ÇØÁ¦½Ã
void CStatsCalcManager::CalcItemStats(PLAYERTYPE* pPlayer)
{
	player_calc_stats * item_stats = pPlayer->GetItemStats();
	Clear(item_stats);

//	WORD ItemIdx;
//		for(int part=0;part<eWearedItem_Max;++part)
//		{
//		ItemIdx = pPlayer->GetWearedItemIdx(part);
//		if(ItemIdx == 0)
//			continue;
	
	for(POSTYPE part = TP_WEAR_START ; part < TP_WEAR_END ; ++part )
	{
		const ITEMBASE * pTargetItemBase = ITEMMGR->GetItemInfoAbsIn(pPlayer, part);

		// yh Ã³À½ µé¾î°¥¶§ ¹®Á¦°¡... -_-a
		if(pTargetItemBase == NULL)
			continue;

		if(pTargetItemBase->dwDBIdx == 0)
			continue;

		ITEM_INFO * pItemInfo = ITEMMGR->GetItemInfo(pTargetItemBase->wIconIdx);
///		ASSERT(pItemInfo);
		if(pItemInfo == NULL)
			continue;

		float ApplyRate = 1.f;
		int gap = pItemInfo->LimitLevel - pPlayer->GetLevel();
		if(gap > 0)
		{
			if( pPlayer->GetShopItemStats()->EquipLevelFree )
				gap -= pPlayer->GetShopItemStats()->EquipLevelFree;
			

			if(gap < 0 )	gap = 0;
#ifdef _JAPAN_LOCAL_			
			if(gap > 6)		gap = 6;
#else
			if(gap > 13)	gap = 13;
#endif // _JAPAN_LOCAL_
			ApplyRate = ItemApplyRate[gap];
		}

		if(pItemInfo->ItemKind == eEQUIP_ITEM_ARMLET)
		{

			// È£È¯ÀÏ°æ¿ì ÃÖÁ¾µ¥¹ÌÁö¿¡ %·Î ´õÇØÁÜ
			item_stats->MeleeAttackPowerAddRateMin += pItemInfo->MeleeAttackMin*ApplyRate * 0.01f;
			item_stats->MeleeAttackPowerAddRateMax += pItemInfo->MeleeAttackMax*ApplyRate * 0.01f;
			item_stats->RangeAttackPowerAddRateMin += pItemInfo->RangeAttackMin*ApplyRate * 0.01f;
			item_stats->RangeAttackPowerAddRateMax += pItemInfo->RangeAttackMax*ApplyRate * 0.01f;
		}
		else
		{
			// ´Ù¸¥ °ÍµéÀº ±×³É °ø°Ý°è»ê½Ä¿¡ ´õÇØÁÜ
			item_stats->MeleeAttackPowerMin += (WORD)(pItemInfo->MeleeAttackMin*ApplyRate);		
			item_stats->MeleeAttackPowerMax += (WORD)(pItemInfo->MeleeAttackMax*ApplyRate);
			item_stats->RangeAttackPowerMin += (WORD)(pItemInfo->RangeAttackMin*ApplyRate);
			item_stats->RangeAttackPowerMax += (WORD)(pItemInfo->RangeAttackMax*ApplyRate);
		}

		item_stats->PhysicalDefense += (WORD)(pItemInfo->PhyDef*ApplyRate);
		item_stats->AttributeAttack.AddATTRIBUTE_VAL(pItemInfo->AttrAttack,ApplyRate);
		item_stats->AttributeResist.AddATTRIBUTE_VAL(pItemInfo->AttrRegist,ApplyRate);
		item_stats->GenGol += (WORD)(pItemInfo->GenGol*ApplyRate);
		item_stats->MinChub += (WORD)(pItemInfo->MinChub*ApplyRate);
		item_stats->CheRyuk += (WORD)(pItemInfo->CheRyuk*ApplyRate);
		item_stats->SimMaek += (WORD)(pItemInfo->SimMek*ApplyRate);
		item_stats->MaxLife += (DWORD)(pItemInfo->Life*ApplyRate);
		item_stats->MaxShield += (DWORD)(pItemInfo->Shield*ApplyRate);
		item_stats->MaxNaeRyuk += (DWORD)(pItemInfo->NaeRyuk*ApplyRate);
		item_stats->PVPDef += 100 * (pItemInfo->PVPDef * ApplyRate);
		item_stats->PVPADef += 100 * (pItemInfo->PVPADef * ApplyRate);
		item_stats->PVPCri += 100 * (pItemInfo->PVPCri * ApplyRate);
		item_stats->PVPADodge += 100 * (pItemInfo->PVPADodge * ApplyRate);
		item_stats->PVPHit += 100 * (pItemInfo->PVPHit * ApplyRate);
		item_stats->PVPAttack += 100 * (pItemInfo->PVPAttack * ApplyRate);
		item_stats->PVPStunResist += 100 * (pItemInfo->PVPStunResist * ApplyRate);
		item_stats->PVPStunTimeReduce += 100 * (pItemInfo->PVPStunTimeReduce * ApplyRate);
		if(ITEMMGR->IsOptionItem(pTargetItemBase->wIconIdx, pTargetItemBase->Durability))
		{
//			DWORD chrid;
#ifdef _MHCLIENT_
//			chrid = HEROID;
			ITEM_OPTION_INFO * pOptionInfo = ITEMMGR->GetItemOption(pTargetItemBase->Durability);
#else
//			chrid = pPlayer->GetID();
			ITEM_OPTION_INFO * pOptionInfo = pPlayer->GetItemOption(pTargetItemBase->Durability);
#endif
			if(pOptionInfo)
			{
				float rate = ApplyRate;
				if( pPlayer->GetShopItemStats()->ReinforceAmp )
					rate += (pPlayer->GetShopItemStats()->ReinforceAmp/100.f);

				item_stats->MeleeAttackPowerMin += (WORD)(pOptionInfo->PhyAttack*rate);		
				item_stats->MeleeAttackPowerMax += (WORD)(pOptionInfo->PhyAttack*rate);
				item_stats->RangeAttackPowerMin += (WORD)(pOptionInfo->PhyAttack*rate);
				item_stats->RangeAttackPowerMax += (WORD)(pOptionInfo->PhyAttack*rate);



				item_stats->PhysicalDefense += (WORD)(pOptionInfo->PhyDefense*rate);
				item_stats->AttributeAttack.AddATTRIBUTE_VAL(pOptionInfo->AttrAttack,rate);
				item_stats->AttributeResist.AddATTRIBUTE_VAL(pOptionInfo->AttrRegist,rate);
				item_stats->GenGol += (WORD)(pOptionInfo->GenGol*rate);
				item_stats->MinChub += (WORD)(pOptionInfo->MinChub*rate);
				item_stats->CheRyuk += (WORD)(pOptionInfo->CheRyuk*rate);
				item_stats->SimMaek += (WORD)(pOptionInfo->SimMek*rate);
				item_stats->MaxLife += (DWORD)(pOptionInfo->Life*rate);
				item_stats->MaxShield += (DWORD)(pOptionInfo->Shield*rate);
				item_stats->MaxNaeRyuk += (DWORD)(pOptionInfo->NaeRyuk*rate);
				item_stats->Critical += (DWORD)(pOptionInfo->CriticalPercent*rate);
			}
			else
			{
				ASSERTMSG(pOptionInfo,"pOptionInfo() is NULL");
			}
		}
		//SW050920 Rare
		if(ITEMMGR->IsRareOptionItem(pTargetItemBase->wIconIdx, pTargetItemBase->RareIdx))
		{
#ifdef _MHCLIENT_
			ITEM_RARE_OPTION_INFO* pRareOptionInfo = ITEMMGR->GetItemRareOption(pTargetItemBase->RareIdx);
#else
			ITEM_RARE_OPTION_INFO* pRareOptionInfo = pPlayer->GetItemRareOption(pTargetItemBase->RareIdx);
#endif
			if(pRareOptionInfo)
			{
				item_stats->MeleeAttackPowerMin += (WORD)(pRareOptionInfo->PhyAttack*ApplyRate);		
				item_stats->MeleeAttackPowerMax += (WORD)(pRareOptionInfo->PhyAttack*ApplyRate);
				item_stats->RangeAttackPowerMin += (WORD)(pRareOptionInfo->PhyAttack*ApplyRate);
				item_stats->RangeAttackPowerMax += (WORD)(pRareOptionInfo->PhyAttack*ApplyRate);

				item_stats->PhysicalDefense += (WORD)(pRareOptionInfo->PhyDefense*ApplyRate);
				item_stats->AttributeAttack.AddATTRIBUTE_VAL(pRareOptionInfo->AttrAttack,ApplyRate);
				item_stats->AttributeResist.AddATTRIBUTE_VAL(pRareOptionInfo->AttrRegist,ApplyRate);
				item_stats->GenGol += (WORD)(pRareOptionInfo->GenGol*ApplyRate);
				item_stats->MinChub += (WORD)(pRareOptionInfo->MinChub*ApplyRate);
				item_stats->CheRyuk += (WORD)(pRareOptionInfo->CheRyuk*ApplyRate);
				item_stats->SimMaek += (WORD)(pRareOptionInfo->SimMek*ApplyRate);
				item_stats->MaxLife += (DWORD)(pRareOptionInfo->Life*ApplyRate);
				item_stats->MaxShield += (DWORD)(pRareOptionInfo->Shield*ApplyRate);
				item_stats->MaxNaeRyuk += (DWORD)(pRareOptionInfo->NaeRyuk*ApplyRate);
				//Critical ¾øÀ½
			}
			else
			{
				ASSERTMSG(pRareOptionInfo,"pRareOptionInfo() is NULL");
			}
		}

		// 2014-11-20 Ð§¹û 

		eITEM_KINDBIT bits = (eITEM_KINDBIT)pItemInfo->ItemKind;

		if(ITEMMGR->IsStoneOptionItem(pTargetItemBase->wIconIdx, bits))
		{
#ifdef _MHCLIENT_
			ITEM_STONE_OPTION_INFO* pStoneOptionInfo = ITEMMGR->GetItemStoneOption(pTargetItemBase->StoneIdx);
#else
			ITEM_STONE_OPTION_INFO* pStoneOptionInfo = pPlayer->GetItemStoneOption(pTargetItemBase->StoneIdx);
#endif
			if(pStoneOptionInfo)
			{
				m_StoneItemList.RemoveAll();
	            
				ITEM_INFO * pStoneItem=NULL;

				if(pStoneOptionInfo->dwItemStone0>1)
				{
					pStoneItem=ITEMMGR->GetItemInfo(pStoneOptionInfo->dwItemStone0);

					if(pStoneItem)
					{
                       m_StoneItemList.Add(pStoneItem,pStoneItem->ItemIdx);
					}
                    pStoneItem=NULL;
				}

				if(pStoneOptionInfo->dwItemStone1>1)
				{
					pStoneItem=ITEMMGR->GetItemInfo(pStoneOptionInfo->dwItemStone1);

					if(pStoneItem)
					{
                       m_StoneItemList.Add(pStoneItem,pStoneItem->ItemIdx);
					}
                    pStoneItem=NULL;
				}
				if(pStoneOptionInfo->dwItemStone2>1)
				{
					pStoneItem=ITEMMGR->GetItemInfo(pStoneOptionInfo->dwItemStone2);

					if(pStoneItem)
					{
                       m_StoneItemList.Add(pStoneItem,pStoneItem->ItemIdx);
					}
                    pStoneItem=NULL;
				}
				if(pStoneOptionInfo->dwItemStone3>1)
				{
					pStoneItem=ITEMMGR->GetItemInfo(pStoneOptionInfo->dwItemStone3);

					if(pStoneItem)
					{
                       m_StoneItemList.Add(pStoneItem,pStoneItem->ItemIdx);
					}
                    pStoneItem=NULL;
				}
			    if(pStoneOptionInfo->dwItemStone4>1)
				{
					pStoneItem=ITEMMGR->GetItemInfo(pStoneOptionInfo->dwItemStone4);

					if(pStoneItem)
					{
                       m_StoneItemList.Add(pStoneItem,pStoneItem->ItemIdx);
					}
                    pStoneItem=NULL;
				}

				m_StoneItemList.SetPositionHead();

                ITEM_INFO * pTempInfo=NULL;

				while(pTempInfo=m_StoneItemList.GetData())
				{
					item_stats->MaxLife+=(DWORD)pTempInfo->Life;
					item_stats->MaxLife+=(DWORD)((item_stats->MaxLife)*(pTempInfo->LifeRecoverRate));
					item_stats->GenGol +=(int)pTempInfo->GenGol;
					item_stats->MinChub+=(int)pTempInfo->MinChub;
					item_stats->CheRyuk+=(int)pTempInfo->CheRyuk;
					item_stats->SimMaek+=(int)pTempInfo->SimMek;
					item_stats->PhysicalDefense+=(int)pTempInfo->PhyDef;
	                item_stats->MeleeAttackPowerMin += (WORD)(pTempInfo->MeleeAttackMin*ApplyRate);		
					item_stats->MeleeAttackPowerMax += (WORD)(pTempInfo->MeleeAttackMax*ApplyRate);
					item_stats->RangeAttackPowerMin+=(WORD)(pTempInfo->RangeAttackMin*ApplyRate);
					item_stats->RangeAttackPowerMax+=(WORD)(pTempInfo->RangeAttackMax*ApplyRate);
					item_stats->AttributeAttack.AddATTRIBUTE_VAL(pTempInfo->AttrAttack,ApplyRate);
				    item_stats->AttributeResist.AddATTRIBUTE_VAL(pTempInfo->AttrRegist,ApplyRate);

				}

				pTempInfo=NULL;

			}
			else
			{
				ASSERTMSG(pStoneOptionInfo,"GetItemStoneOption() is NULL");
			}

		}
		if (pTargetItemBase->ItemGradeAlexX > 0)
		{
			DWORD Grade = pTargetItemBase->ItemGradeAlexX;
			ITEM_INFO* pItemInfo = ITEMMGR->GetItemInfo(pTargetItemBase->wIconIdx);
			float increase_factor = 1.0; // ¡ÓË¹´¤èÒàÃÔèÁµé¹à»ç¹ 1.0 ËÃ×Í¤èÒÍ×è¹æ µÒÁ·Õè¤Ø³µéÍ§¡ÒÃ
			// ãªé§Ò¹ increase_factor ËÅÑ§¨Ò¡¡ÓË¹´¤èÒàÃÔèÁµé?
			//increase_factor = increase_factor * 1.03; // à¾ÔèÁ¤èÒà»ç¹ 3%
			//increase_factor = increase_factor * 1.11; //og

			if (pItemInfo->GenGol > 0)
			{
				increase_factor = NEWUPGRAREALEXX_CMGR->AlexXUpGradeConfig.SetupGrade_AlexX[GENGOL_MINCHUB_CHERYUK_SIMMEK] * 1.00;
				item_stats->GenGol += (WORD)(pItemInfo->GenGol * pow(increase_factor, Grade));
			}
			if (pItemInfo->MinChub > 0)
			{
				increase_factor = NEWUPGRAREALEXX_CMGR->AlexXUpGradeConfig.SetupGrade_AlexX[GENGOL_MINCHUB_CHERYUK_SIMMEK] * 1.00;
				item_stats->MinChub += (WORD)(pItemInfo->MinChub * pow(increase_factor, Grade));
			}
			if (pItemInfo->CheRyuk > 0)
			{
				increase_factor = NEWUPGRAREALEXX_CMGR->AlexXUpGradeConfig.SetupGrade_AlexX[GENGOL_MINCHUB_CHERYUK_SIMMEK] * 1.00;
				item_stats->CheRyuk += (WORD)(pItemInfo->CheRyuk * pow(increase_factor, Grade));
			}
			if (pItemInfo->SimMek > 0)
			{
				increase_factor = NEWUPGRAREALEXX_CMGR->AlexXUpGradeConfig.SetupGrade_AlexX[GENGOL_MINCHUB_CHERYUK_SIMMEK] * 1.00;
				item_stats->SimMaek += (WORD)(pItemInfo->SimMek * pow(increase_factor, Grade));
			}
			if (pItemInfo->Life > 0)
			{
				increase_factor = NEWUPGRAREALEXX_CMGR->AlexXUpGradeConfig.SetupGrade_AlexX[LITE_SHIELD_NAERYUK_PHYDEF] * 1.00;
				item_stats->MaxLife += (DWORD)(pItemInfo->Life * pow(increase_factor, Grade));
			}
			if (pItemInfo->Shield > 0)
			{
				increase_factor = NEWUPGRAREALEXX_CMGR->AlexXUpGradeConfig.SetupGrade_AlexX[LITE_SHIELD_NAERYUK_PHYDEF] * 1.00;
				item_stats->MaxShield += (DWORD)(pItemInfo->Shield * pow(increase_factor, Grade));
			}
			if (pItemInfo->NaeRyuk > 0)
			{
				increase_factor = NEWUPGRAREALEXX_CMGR->AlexXUpGradeConfig.SetupGrade_AlexX[LITE_SHIELD_NAERYUK_PHYDEF] * 1.00;
				item_stats->MaxNaeRyuk += (DWORD)(pItemInfo->NaeRyuk * pow(increase_factor, Grade));
			}
			if (pItemInfo->PhyDef > 0)
			{
				increase_factor = NEWUPGRAREALEXX_CMGR->AlexXUpGradeConfig.SetupGrade_AlexX[LITE_SHIELD_NAERYUK_PHYDEF] * 1.00;
				item_stats->PhysicalDefense += (WORD)(pItemInfo->PhyDef * pow(increase_factor, Grade));
			}
			if (pItemInfo->CriticalPercent > 0)
			{
				increase_factor = NEWUPGRAREALEXX_CMGR->AlexXUpGradeConfig.SetupGrade_AlexX[CRITICAL] * 1.00;
				item_stats->Critical += (DWORD)((pItemInfo->CriticalPercent * pow(increase_factor, Grade) * ApplyRate));
			}
			/**/
			if (pItemInfo->MeleeAttackMin > 0)
			{
				increase_factor = NEWUPGRAREALEXX_CMGR->AlexXUpGradeConfig.SetupGrade_AlexX[MELEE_RANGE_ATTACK] * 1.00;
				item_stats->MeleeAttackPowerMin += (WORD)((pItemInfo->MeleeAttackMin * pow(increase_factor, Grade) * ApplyRate));
			}
			if (pItemInfo->MeleeAttackMax > 0)
			{
				increase_factor = NEWUPGRAREALEXX_CMGR->AlexXUpGradeConfig.SetupGrade_AlexX[MELEE_RANGE_ATTACK] * 1.00;
				item_stats->MeleeAttackPowerMax += (WORD)((pItemInfo->MeleeAttackMax * pow(increase_factor, Grade) * ApplyRate));
			}
			/**/
			if (pItemInfo->RangeAttackMin > 0)
			{
				increase_factor = NEWUPGRAREALEXX_CMGR->AlexXUpGradeConfig.SetupGrade_AlexX[MELEE_RANGE_ATTACK] * 1.00;
				item_stats->RangeAttackPowerMin += (WORD)((pItemInfo->RangeAttackMin * pow(increase_factor, Grade) * ApplyRate));
			}
			if (pItemInfo->RangeAttackMax > 0)
			{
				increase_factor = NEWUPGRAREALEXX_CMGR->AlexXUpGradeConfig.SetupGrade_AlexX[MELEE_RANGE_ATTACK] * 1.00;
				item_stats->RangeAttackPowerMax += (WORD)((pItemInfo->RangeAttackMax * pow(increase_factor, Grade) * ApplyRate));
			}
			/**/
			ATTRIBUTEREGIST AlexX_WhatTheFuckAttrAttack;
			ATTRIBUTEREGIST AlexX_WhatTheFuckAttrRegist;

			/*if (pItemInfo->AttrAttack.GetElement_Val(ATTR_FIRE) > 0)
			{
			increase_factor = NEWUPGRAREALEXX_CMGR->AlexXUpGradeConfig.SetupGrade_AlexX[ELEMENT_ATTACK_REGIST] * 1.00;
			AlexX_WhatTheFuckAttrAttack.SetElement_Val(ATTR_FIRE, pItemInfo->AttrAttack.GetElement_Val(ATTR_FIRE) * pow(increase_factor, Grade));
			}
			if (pItemInfo->AttrAttack.GetElement_Val(ATTR_WATER) > 0)
			{
			increase_factor = NEWUPGRAREALEXX_CMGR->AlexXUpGradeConfig.SetupGrade_AlexX[ELEMENT_ATTACK_REGIST] * 1.00;
			AlexX_WhatTheFuckAttrAttack.SetElement_Val(ATTR_WATER, pItemInfo->AttrAttack.GetElement_Val(ATTR_WATER) * pow(increase_factor, Grade));
			}
			if (pItemInfo->AttrAttack.GetElement_Val(ATTR_TREE) > 0)
			{
			increase_factor = NEWUPGRAREALEXX_CMGR->AlexXUpGradeConfig.SetupGrade_AlexX[ELEMENT_ATTACK_REGIST] * 1.00;
			AlexX_WhatTheFuckAttrAttack.SetElement_Val(ATTR_TREE, pItemInfo->AttrAttack.GetElement_Val(ATTR_TREE) * pow(increase_factor, Grade));
			}
			if (pItemInfo->AttrAttack.GetElement_Val(ATTR_IRON) > 0)
			{
			increase_factor = NEWUPGRAREALEXX_CMGR->AlexXUpGradeConfig.SetupGrade_AlexX[ELEMENT_ATTACK_REGIST] * 1.00;
			AlexX_WhatTheFuckAttrAttack.SetElement_Val(ATTR_IRON, pItemInfo->AttrAttack.GetElement_Val(ATTR_IRON) * pow(increase_factor, Grade));
			}
			if (pItemInfo->AttrAttack.GetElement_Val(ATTR_EARTH) > 0)
			{
			increase_factor = NEWUPGRAREALEXX_CMGR->AlexXUpGradeConfig.SetupGrade_AlexX[ELEMENT_ATTACK_REGIST] * 1.00;
			AlexX_WhatTheFuckAttrAttack.SetElement_Val(ATTR_EARTH, pItemInfo->AttrAttack.GetElement_Val(ATTR_EARTH) * pow(increase_factor, Grade));
			}

			if (pItemInfo->AttrRegist.GetElement_Val(ATTR_FIRE) > 0)
			{
			increase_factor = NEWUPGRAREALEXX_CMGR->AlexXUpGradeConfig.SetupGrade_AlexX[ELEMENT_ATTACK_REGIST] * 1.00;
			AlexX_WhatTheFuckAttrRegist.SetElement_Val(ATTR_FIRE, pItemInfo->AttrRegist.GetElement_Val(ATTR_FIRE) * pow(increase_factor, Grade));
			}
			if (pItemInfo->AttrRegist.GetElement_Val(ATTR_WATER) > 0)
			{
			increase_factor = NEWUPGRAREALEXX_CMGR->AlexXUpGradeConfig.SetupGrade_AlexX[ELEMENT_ATTACK_REGIST] * 1.00;
			AlexX_WhatTheFuckAttrRegist.SetElement_Val(ATTR_WATER, pItemInfo->AttrRegist.GetElement_Val(ATTR_WATER) * pow(increase_factor, Grade));
			}
			if (pItemInfo->AttrRegist.GetElement_Val(ATTR_TREE) > 0)
			{
			increase_factor = NEWUPGRAREALEXX_CMGR->AlexXUpGradeConfig.SetupGrade_AlexX[ELEMENT_ATTACK_REGIST] * 1.00;
			AlexX_WhatTheFuckAttrRegist.SetElement_Val(ATTR_TREE, pItemInfo->AttrRegist.GetElement_Val(ATTR_TREE) * pow(increase_factor, Grade));
			}
			if (pItemInfo->AttrRegist.GetElement_Val(ATTR_IRON) > 0)
			{
			increase_factor = NEWUPGRAREALEXX_CMGR->AlexXUpGradeConfig.SetupGrade_AlexX[ELEMENT_ATTACK_REGIST] * 1.00;
			AlexX_WhatTheFuckAttrRegist.SetElement_Val(ATTR_IRON, pItemInfo->AttrRegist.GetElement_Val(ATTR_IRON) * pow(increase_factor, Grade));
			}
			if (pItemInfo->AttrRegist.GetElement_Val(ATTR_EARTH) > 0)
			{
			increase_factor = NEWUPGRAREALEXX_CMGR->AlexXUpGradeConfig.SetupGrade_AlexX[ELEMENT_ATTACK_REGIST] * 1.00;
			AlexX_WhatTheFuckAttrRegist.SetElement_Val(ATTR_EARTH, pItemInfo->AttrRegist.GetElement_Val(ATTR_EARTH) * pow(increase_factor, Grade));
			}

			item_stats->AttributeAttack.AddATTRIBUTE_VAL(AlexX_WhatTheFuckAttrAttack, ApplyRate);
			item_stats->AttributeResist.AddATTRIBUTE_VAL(AlexX_WhatTheFuckAttrRegist, ApplyRate);*/

			bool kuy1 = false;
			bool kuy2 = false;
			for (int WhatTheFuck = ATTR_FIRE; WhatTheFuck <= ATTR_MAX; ++WhatTheFuck)
			{
				// T T AlexX So Sad
				increase_factor = NEWUPGRAREALEXX_CMGR->AlexXUpGradeConfig.SetupGrade_AlexX[ELEMENT_ATTACK_REGIST] * 1.00;

				float val1 = (pItemInfo->AttrAttack.GetElement_Val(WhatTheFuck) * pow(increase_factor, Grade)) - pItemInfo->AttrAttack.GetElement_Val(WhatTheFuck);
				AlexX_WhatTheFuckAttrAttack.SetElement_Val(WhatTheFuck, val1);
				if (pItemInfo->AttrAttack.GetElement_Val(WhatTheFuck) > 0)
				{
					kuy1 = true;
				}

				float val2 = (pItemInfo->AttrRegist.GetElement_Val(WhatTheFuck) * pow(increase_factor, Grade)) - pItemInfo->AttrRegist.GetElement_Val(WhatTheFuck);
				AlexX_WhatTheFuckAttrRegist.SetElement_Val(WhatTheFuck, val2);
				if (pItemInfo->AttrRegist.GetElement_Val(WhatTheFuck) > 0)
				{
					kuy2 = true;
				}
			}
			if (kuy1)
				item_stats->AttributeAttack.AddATTRIBUTE_VAL(AlexX_WhatTheFuckAttrAttack, ApplyRate);
			if (kuy2)
				item_stats->AttributeResist.AddATTRIBUTE_VAL(AlexX_WhatTheFuckAttrRegist, ApplyRate);
		}
		// Ð§¹û¸½ 2015-01-15 

		/*if(pTargetItemBase->ItemGrow>0)
		{
            ITEM_GROW_SET * m_ItemGrowInfo = ITEMMGR->GetItemGrowSet();

			if(m_ItemGrowInfo &&  pTargetItemBase)
			{
				DWORD RateValue= (int)(pTargetItemBase->ItemGrow / m_ItemGrowInfo->GrowRate);

				item_stats->MaxLife+=(DWORD) (RateValue * m_ItemGrowInfo->Life);

				item_stats->MaxShield+=(DWORD)(RateValue * m_ItemGrowInfo->Shield);

				item_stats->MaxNaeRyuk+=(DWORD)(RateValue * m_ItemGrowInfo->NaeRyuk);

				item_stats->GenGol+=(DWORD)(RateValue * m_ItemGrowInfo->GenGol);

				item_stats->MinChub+=(DWORD)(RateValue * m_ItemGrowInfo ->MinChub);

				item_stats->CheRyuk+=(DWORD)(RateValue * m_ItemGrowInfo->CheRyuk);

				item_stats->SimMaek+=(DWORD)(RateValue * m_ItemGrowInfo->SimMek);

				item_stats->PhysicalDefense+=(DWORD)(RateValue * m_ItemGrowInfo->PhyDef);
			}

			m_ItemGrowInfo=NULL;
		}*/
	}


	item_stats->AttributeResist.CheckLimit(10);	///ÏÞÖÆ·¨¿¹ÉÏÏÞ

	// 06. 03. ±¹³»¹«½Ö - ÀÌ¿µÁØ
	if(MUSSANGMGR->IsMussangMode())
	{
		WORD stat = MUSSANGMGR->GetMussangStat();

		item_stats->GenGol += stat;
		item_stats->MinChub += stat;
		item_stats->CheRyuk += stat;
		item_stats->SimMaek += stat;
	}

	if( pPlayer->GetFame() > 0 )
	{
		//WORD famestat = pPlayer->GetFame();
		FAMETYPE val = pPlayer->GetFame();

		if (FAMEMGR->GetPowerSwitch(val) != 0)
		{
			item_stats->GenGol += FAMEMGR->GetSTR(val);
			item_stats->MinChub += FAMEMGR->GetAGI(val);
			item_stats->CheRyuk += FAMEMGR->GetCON(val);
			item_stats->SimMaek += FAMEMGR->GetINT(val);
		}

		
	}


	PETMGR->AddMasterStatFromPetBuff(item_stats);


	/////////////////// 2007. 6. 11. CBH - ¼¼Æ®¾ÆÀÌÅÆ ´É·ÂÄ¡ Ãß°¡ ////////////////
	CalcSetItemStats();
	/////////////////////////////////////////////////////////////////////////////

	// magi82 - UniqueItem(070626)
	CalcUniqueItemStats();

	// ¾ÆÀÌÅÛ ¼öÄ¡°¡ º¯°æµÇ¸é Ä³¸¯ÅÍ ¼öÄ¡µµ º¯°æ
	CalcCharStats(pPlayer);
}

// Ä³¸¯°ÔÀÓIn
// ·¹º§¾÷½Ã ( ´É·ÂÄ¡ ¹Ù²ð¶§)

// ¹«°ø¹ßµ¿½Ã(+´É·ÂÄ¡) : ±¸Á¶Ã¼¸¦ ÇÏ³ª ´õ µÖ¾ß ÇÏ³ª?
void CStatsCalcManager::CalcCharStats(PLAYERTYPE* pPlayer)
{
	player_calc_stats * char_stats = pPlayer->GetCharStats();
	Clear(char_stats);

	HERO_TOTALINFO pHeroInfo;
	pPlayer->GetHeroTotalInfo(&pHeroInfo);
	// ¾ÆÀÌÅÛ ÀåÂøÇÑ°Å¿¡ µû¸¥ Ä³¸¯ÅÍ ¼öÄ¡ ¼ÂÆÃ
	char_stats->MinChub = pHeroInfo.wMinChub;
	char_stats->GenGol = pHeroInfo.wGenGol;


#ifdef _JAPAN_LOCAL_
	char_stats->PhysicalDefense = pPlayer->GetCheRyuk() * 10 / 15;

	char_stats->Critical = ( pPlayer->GetGenGol() / 4 ) + ( pPlayer->GetMinChub() / 6 );

#else
	char_stats->PhysicalDefense = (WORD)(((pPlayer->GetCheRyuk())/1.5)*0.6);//char_stats->PhysicalDefense = (WORD)(pPlayer->GetCheRyuk()/1.5);//char_stats->PhysicalDefense = (WORD)(((pPlayer->GetCheRyuk())/1.5)*0.6);//kiv
	// 10/27 ¼öÄ¡ º¯°æÀü	char_stats->PhysicalDefense = pHeroInfo.wCheRyuk/3;
	char_stats->Critical = (pPlayer->GetGenGol()) / 4;
	char_stats->Decisive = (pPlayer->GetSimMek()) / 4;
#endif

#ifdef _MAPSERVER_
	CCharacterCalcManager::CalcMaxLife(pPlayer);
	CCharacterCalcManager::CalcMaxShield(pPlayer);
	CCharacterCalcManager::CalcMaxNaeRyuk(pPlayer);
#endif
}

/////////////////// 2007. 6. 11. CBH - ¼¼Æ®¾ÆÀÌÅÆ ´É·ÂÄ¡ Ãß°¡ ////////////////
void CStatsCalcManager::CalcSetItemStats()
{
	HERO->ClearSetitemOption();
	
	SET_ITEM_INFO setItemInfo[MAX_SETITEM_KIND_NUM];

	memset(setItemInfo, 0, sizeof(SET_ITEM_INFO) * MAX_SETITEM_KIND_NUM);

	//ÀåÂøµÈ ¼¼Æ®¾ÆÀÌÅÆÀÇ Á¾·ù¿Í °¹¼ö¸¦ °Ë»öÇÑ´Ù.
	for(POSTYPE part = TP_WEAR_START ; part < TP_WEAR_END ; ++part )
	{
		const ITEMBASE * pTargetItemBase = ITEMMGR->GetItemInfoAbsIn(HERO, part);

		if(pTargetItemBase == NULL)
		{
			continue;
		}

		if(pTargetItemBase->dwDBIdx == 0)
		{
			continue;
		}

		ITEM_INFO * pItemInfo = ITEMMGR->GetItemInfo(pTargetItemBase->wIconIdx);

		if(pItemInfo == NULL)
			continue;

		//ÀåÂøµÈ ¼¼Æ®¾ÆÀÌÅÆÀÇ Á¾·ù¿Í °¹¼öÁ¤º¸¸¦ ¹è¿­¿¡ ÀúÀå
		if(pItemInfo->wSetItemKind != 0)
		{
			for(unsigned int ui = 0 ; ui < MAX_SETITEM_KIND_NUM ; ui++)
			{
				if(setItemInfo[ui].wKind == pItemInfo->wSetItemKind)

				{
					setItemInfo[ui].wCount++;
					break;
				}
				else
				{
					if(setItemInfo[ui].wKind == 0)
					{
						setItemInfo[ui].wKind = pItemInfo->wSetItemKind;
						setItemInfo[ui].wCount++;
						break;
					}					
				}				
			}
		}		
	}

	//¹è¿­¿¡ ÀúÀåµÈ ¼¼Æ®¾ÆÀÌÅÆ Á¤º¸¸¦ °¡Áö°í ¿É¼Ç Àû¿ë
	CYHHashTable<SET_ITEM_OPTION>* setItemOptionList = ITEMMGR->GetSetItemOptionList();

	for(unsigned int ui = 0 ; ui < MAX_SETITEM_KIND_NUM ; ui++)
	{
		if(setItemInfo[ui].wKind != 0)
		{			
			setItemOptionList->SetPositionHead();

			SET_ITEM_OPTION* pSetItemOption = NULL;
			while(pSetItemOption = setItemOptionList->GetData())
			{			
				if(pSetItemOption->wSetItemKind == setItemInfo[ui].wKind)
				{
					WORD wSetValue = pSetItemOption->wSetValue;					

					if(setItemInfo[ui].wCount >= wSetValue)
					{
						HERO->AddSetitemOption( pSetItemOption );
					}					
				}				
			}			
		}

	}
}
/////////////////////////////////////////////////////////////////////////////

// magi82 - UniqueItem(070626)
void CStatsCalcManager::CalcUniqueItemStats()
{
	// À¯´ÏÅ© °ü·Ã ½ºÅÝÀ» ÃÊ±âÈ­ÈÄ ´Ù½Ã ¼¼ÆÃ
	HERO->ClearUniqueItemOption();

	for( POSTYPE pos = TP_WEAR_START; pos < TP_WEAR_END; pos++ )
	{
		const ITEMBASE* pItemBase = ITEMMGR->GetItemInfoAbsIn(HERO, pos);
		if( !pItemBase )
		{
			continue;
		}

		ITEM_INFO* pInfo = ITEMMGR->GetItemInfo(pItemBase->wIconIdx);
		if(!pInfo)
		{
			continue;
		}

		// À¯´ÏÅ© ¾ÆÀÌÅÛÀÌ¶ó¸é..

		if( pInfo->ItemKind == eEQUIP_ITEM_UNIQUE )
		{
			UNIQUE_ITEM_OPTION_INFO* pUniqueItemInfo = GAMERESRCMNGR->GetUniqueItemOptionList(pInfo->ItemIdx);
			if( !pUniqueItemInfo )
			{
				continue;
			}

			HERO->AddUniqueItemOption(pUniqueItemInfo);
		}
	}
}
//////////////////////////////////////////////////////////////////////////

// magi82 - Titan(070809)
void CStatsCalcManager::CalcTitanStats(DWORD dwDBIdx)
{
	// Å¸ÀÌÅº Áõ¼­ ¾ÆÀÌÅÛ
	CItem* pItem = ITEMMGR->GetItem(dwDBIdx);

	if(!pItem)
	{
		return;
	}

	titan_calc_stats* pTitanStats = TITANMGR->GetTitanStats();
	ZeroMemory(pTitanStats, sizeof(titan_calc_stats));

	ITEM_INFO* pInfo = ITEMMGR->GetItemInfo(pItem->GetItemIdx());
	BASE_TITAN_LIST* pTitanBaseInfo = GAMERESRCMNGR->GetTitanListInfo(pInfo->Plus_MugongIdx);	// Å¸ÀÌÅº ¸®½ºÆ®

	// Å¸ÀÌÅº ½ºÅÝ¿¡ ¸ÕÀú Å¸ÀÌÅº º»Ã¼ÀÇ ½ºÅÝÀ» ÀúÀåÇÏ°í³ª¼­ ¾ÆÀÌÅÛ¿¡ ÀÖ´Â ¼º°ªÀ» ÀúÀå
	pTitanStats->MaxFuel				= pTitanBaseInfo->GradeInfo[pInfo->Plus_Value - 1].MaxFuel;
	pTitanStats->MaxSpell				= pTitanBaseInfo->GradeInfo[pInfo->Plus_Value - 1].MaxSpell;
	pTitanStats->PhysicalDefense		= (WORD)pTitanBaseInfo->GradeInfo[pInfo->Plus_Value - 1].PhyDef;
	pTitanStats->Critical				= pTitanBaseInfo->GradeInfo[pInfo->Plus_Value - 1].Critical;
	pTitanStats->MeleeAttackPowerMin	= pTitanBaseInfo->GradeInfo[pInfo->Plus_Value - 1].MeleeAttackPowerMin;
	pTitanStats->MeleeAttackPowerMax	= pTitanBaseInfo->GradeInfo[pInfo->Plus_Value - 1].MeleeAttackPowerMax;
	pTitanStats->RangeAttackPowerMin	= pTitanBaseInfo->GradeInfo[pInfo->Plus_Value - 1].RangeAttackPowerMin;
	pTitanStats->RangeAttackPowerMax	= pTitanBaseInfo->GradeInfo[pInfo->Plus_Value - 1].RangeAttackPowerMax;
	pTitanStats->AttackRange			= pTitanBaseInfo->GradeInfo[pInfo->Plus_Value - 1].AttackRange;
	pTitanStats->MoveSpeed				= (float)pTitanBaseInfo->GradeInfo[pInfo->Plus_Value - 1].Speed;
	pTitanStats->AttributeAttack.AddATTRIBUTE_VAL(pTitanBaseInfo->GradeInfo[pInfo->Plus_Value - 1].AttAtk, ATTR_FIRE);
	pTitanStats->AttributeResist.AddATTRIBUTE_VAL(pTitanBaseInfo->GradeInfo[pInfo->Plus_Value - 1].AttReg, ATTR_FIRE);

	for( POSTYPE pos = TP_TITANWEAR_START; pos < TP_TITANWEAR_END; pos++ )
	{
		CItem* pItem = ITEMMGR->GetItemofTable( eItemTable_Titan, pos );
		if(!pItem)
		{
			continue;
		}

		TITAN_ENDURANCE_ITEMINFO* pEnduranceInfo = NULL;
		pEnduranceInfo = TITANMGR->GetTitanEnduranceInfo(pItem->GetDBIdx());
		if(NULL == pEnduranceInfo || pEnduranceInfo->Endurance == 0)	// ³»±¸ Á¤º¸°¡ ¾ø°Å³ª ³»±¸µµ°¡ 0ÀÌ¸é ÆÐ½º
			continue;


		ITEM_INFO* pItemInfo = ITEMMGR->GetItemInfo(pItem->GetItemIdx());
		if(NULL == pItemInfo)
			continue;

		pTitanStats->MaxFuel				+= pItemInfo->Life;
		pTitanStats->MaxSpell				+= pItemInfo->NaeRyuk;
		pTitanStats->PhysicalDefense		+= pItemInfo->PhyDef;
		pTitanStats->Critical				+= pItemInfo->CriticalPercent;
		pTitanStats->MeleeAttackPowerMin	+= pItemInfo->MeleeAttackMin;
		pTitanStats->MeleeAttackPowerMax	+= pItemInfo->MeleeAttackMax;
		pTitanStats->RangeAttackPowerMin	+= pItemInfo->RangeAttackMin;
		pTitanStats->RangeAttackPowerMax	+= pItemInfo->RangeAttackMax;
		pTitanStats->AttackRange			+= pItemInfo->MugongNum;
		pTitanStats->MoveSpeed				+= pItemInfo->AllPlus_Value;

		pTitanStats->AttributeAttack.AddATTRIBUTE_VAL(pItemInfo->AttrAttack, ATTR_FIRE);
		pTitanStats->AttributeResist.AddATTRIBUTE_VAL(pItemInfo->AttrRegist, ATTR_FIRE);


		/*
		pTitanStats->AttributeAttack.SetElement_Val(ATTR_FIRE, pTitanBaseInfo->GradeInfo[pInfo->Plus_Value - 1].AttAtk.GetElement_Val(ATTR_FIRE)
			+ pItemInfo->AttrAttack.GetElement_Val(ATTR_FIRE));

		pTitanStats->AttributeResist.SetElement_Val(ATTR_FIRE, pTitanBaseInfo->GradeInfo[pInfo->Plus_Value - 1].AttReg.GetElement_Val(ATTR_FIRE)
			+ pItemInfo->AttrRegist.GetElement_Val(ATTR_FIRE));
		pTitanStats->AttributeResist.SetElement_Val(ATTR_WATER, pTitanBaseInfo->GradeInfo[pInfo->Plus_Value - 1].AttReg.GetElement_Val(ATTR_FIRE)

			+ pItemInfo->AttrRegist.GetElement_Val(ATTR_FIRE));
		pTitanStats->AttributeResist.SetElement_Val(ATTR_TREE, pTitanBaseInfo->GradeInfo[pInfo->Plus_Value - 1].AttReg.GetElement_Val(ATTR_FIRE)
			+ pItemInfo->AttrRegist.GetElement_Val(ATTR_FIRE));
		pTitanStats->AttributeResist.SetElement_Val(ATTR_IRON, pTitanBaseInfo->GradeInfo[pInfo->Plus_Value - 1].AttReg.GetElement_Val(ATTR_FIRE)
			+ pItemInfo->AttrRegist.GetElement_Val(ATTR_FIRE));
		pTitanStats->AttributeResist.SetElement_Val(ATTR_EARTH, pTitanBaseInfo->GradeInfo[pInfo->Plus_Value - 1].AttReg.GetElement_Val(ATTR_FIRE)
			+ pItemInfo->AttrRegist.GetElement_Val(ATTR_FIRE));
		*/
	}

	GAMEIN->GetTitanInventoryDlg()->SetTitanInvenInfo();
}
//////////////////////////////////////////////////////////////////////////




