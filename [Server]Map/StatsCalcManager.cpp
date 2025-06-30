



// StatsCalcManager.cpp: implementation of the CStatsCalcManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "StatsCalcManager.h"
#include "..\[CC]Header\GameResourceManager.h"

#ifdef _MHCLIENT_
#include "Hero.h"
#include "GameIn.h"

#endif

#ifdef _MAPSERVER_
#include "Player.h"
#include "TitanManager.h"
#include "NewUpGrareAlexXMGR.h"
#endif

#include "CharacterCalcManager.h"
#include "ItemManager.h"

#include "FameManager.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CStatsCalcManager::CStatsCalcManager()
{
	// 2014-11-20 ÐÅÏ¢ÁÙÊ±
	m_StoneItemList.Initialize(5);
}

CStatsCalcManager::~CStatsCalcManager()
{
	// 2014-11-20 ÐÅÏ¢ÁÙÊ±
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
	
	ASSERT(pPlayer->GetMaxLevel() >= pPlayer->GetLevel());
	
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

			if(gap < 0)		gap = 0;
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
		
		if(ITEMMGR->IsOptionItem(pTargetItemBase->wIconIdx, pTargetItemBase->Durability))
		{
			DWORD chrid;
#ifdef _MHCLIENT_
			chrid = HEROID;
			ITEM_OPTION_INFO * pOptionInfo = ITEMMGR->GetItemOption(pTargetItemBase->Durability);
#else
			chrid = pPlayer->GetID();
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
				ASSERTMSG(pOptionInfo,"GetItemOption() is NULL");
				char temp[128];

				sprintf(temp,"%d  %d  %d",chrid,pTargetItemBase->dwDBIdx,pTargetItemBase->Durability);
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
//				item_stats->Critical += (DWORD)(pRareOptionInfo->CriticalPercent*ApplyRate);				
			}
			else
			{
				ASSERTMSG(pRareOptionInfo,"GetItemRareOption() is NULL");
			}
		}

        // 2014-11-20 Ð§¹ûÊµÏÖ ·þÎñÆ÷¶Ë

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
		//ÌìÄ« PVPÄÝ©Ê¼W¥[
		item_stats->PVPCri += pItemInfo->PVPCri;
		item_stats->PVPAttack += pItemInfo->PVPAttack;
		item_stats->PVPDef += pItemInfo->PVPDef;
		item_stats->PVPADef += pItemInfo->PVPADef;
		item_stats->PVPHit += pItemInfo->PVPHit;
		item_stats->PVPADodge += pItemInfo->PVPADodge;
		item_stats->PVPStunResist += pItemInfo->PVPStunResist;
		item_stats->PVPStunTimeReduce += pItemInfo->PVPStunTimeReduce;
		//weiye  ³É³¤¼¶±ðÊµÏÖ  Âú³É³¤250000, ÊµÏÖ·Ö¼¶  2018-01-24 

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
				increase_factor = NEWUPGRAREALEXX_MGR->AlexXUpGradeConfig.SetupGrade_AlexX[GENGOL_MINCHUB_CHERYUK_SIMMEK] * 1.00;
				item_stats->GenGol += (WORD)(pItemInfo->GenGol * pow(increase_factor, Grade));
			}
			if (pItemInfo->MinChub > 0)
			{
				increase_factor = NEWUPGRAREALEXX_MGR->AlexXUpGradeConfig.SetupGrade_AlexX[GENGOL_MINCHUB_CHERYUK_SIMMEK] * 1.00;
				item_stats->MinChub += (WORD)(pItemInfo->MinChub * pow(increase_factor, Grade));
			}
			if (pItemInfo->CheRyuk > 0)
			{
				increase_factor = NEWUPGRAREALEXX_MGR->AlexXUpGradeConfig.SetupGrade_AlexX[GENGOL_MINCHUB_CHERYUK_SIMMEK] * 1.00;
				item_stats->CheRyuk += (WORD)(pItemInfo->CheRyuk * pow(increase_factor, Grade));
			}
			if (pItemInfo->SimMek > 0)
			{
				increase_factor = NEWUPGRAREALEXX_MGR->AlexXUpGradeConfig.SetupGrade_AlexX[GENGOL_MINCHUB_CHERYUK_SIMMEK] * 1.00;
				item_stats->SimMaek += (WORD)(pItemInfo->SimMek * pow(increase_factor, Grade));
			}
			if (pItemInfo->Life > 0)
			{
				increase_factor = NEWUPGRAREALEXX_MGR->AlexXUpGradeConfig.SetupGrade_AlexX[LITE_SHIELD_NAERYUK_PHYDEF] * 1.00;
				item_stats->MaxLife += (DWORD)(pItemInfo->Life * pow(increase_factor, Grade));
			}
			if (pItemInfo->Shield > 0)
			{
				increase_factor = NEWUPGRAREALEXX_MGR->AlexXUpGradeConfig.SetupGrade_AlexX[LITE_SHIELD_NAERYUK_PHYDEF] * 1.00;
				item_stats->MaxShield += (DWORD)(pItemInfo->Shield * pow(increase_factor, Grade));
			}
			if (pItemInfo->NaeRyuk > 0)
			{
				increase_factor = NEWUPGRAREALEXX_MGR->AlexXUpGradeConfig.SetupGrade_AlexX[LITE_SHIELD_NAERYUK_PHYDEF] * 1.00;
				item_stats->MaxNaeRyuk += (DWORD)(pItemInfo->NaeRyuk * pow(increase_factor, Grade));
			}
			if (pItemInfo->PhyDef > 0)
			{
				increase_factor = NEWUPGRAREALEXX_MGR->AlexXUpGradeConfig.SetupGrade_AlexX[LITE_SHIELD_NAERYUK_PHYDEF] * 1.00;
				item_stats->PhysicalDefense += (WORD)(pItemInfo->PhyDef * pow(increase_factor, Grade));
			}
			if (pItemInfo->CriticalPercent > 0)
			{
				increase_factor = NEWUPGRAREALEXX_MGR->AlexXUpGradeConfig.SetupGrade_AlexX[CRITICAL] * 1.00;
				item_stats->Critical += (DWORD)((pItemInfo->CriticalPercent * pow(increase_factor, Grade) * ApplyRate));
			}
			/**/
			if (pItemInfo->MeleeAttackMin > 0)
			{
				increase_factor = NEWUPGRAREALEXX_MGR->AlexXUpGradeConfig.SetupGrade_AlexX[MELEE_RANGE_ATTACK] * 1.00;
				item_stats->MeleeAttackPowerMin += (WORD)((pItemInfo->MeleeAttackMin * pow(increase_factor, Grade) * ApplyRate));
			}
			if (pItemInfo->MeleeAttackMax > 0)
			{
				increase_factor = NEWUPGRAREALEXX_MGR->AlexXUpGradeConfig.SetupGrade_AlexX[MELEE_RANGE_ATTACK] * 1.00;
				item_stats->MeleeAttackPowerMax += (WORD)((pItemInfo->MeleeAttackMax * pow(increase_factor, Grade) * ApplyRate));
			}
			/**/
			if (pItemInfo->RangeAttackMin > 0)
			{
				increase_factor = NEWUPGRAREALEXX_MGR->AlexXUpGradeConfig.SetupGrade_AlexX[MELEE_RANGE_ATTACK] * 1.00;
				item_stats->RangeAttackPowerMin += (WORD)((pItemInfo->RangeAttackMin * pow(increase_factor, Grade) * ApplyRate));
			}
			if (pItemInfo->RangeAttackMax > 0)
			{
				increase_factor = NEWUPGRAREALEXX_MGR->AlexXUpGradeConfig.SetupGrade_AlexX[MELEE_RANGE_ATTACK] * 1.00;
				item_stats->RangeAttackPowerMax += (WORD)((pItemInfo->RangeAttackMax * pow(increase_factor, Grade) * ApplyRate));
			}
			/**/
			ATTRIBUTEREGIST AlexX_WhatTheFuckAttrAttack;
			ATTRIBUTEREGIST AlexX_WhatTheFuckAttrRegist;

			/*if (pItemInfo->AttrAttack.GetElement_Val(ATTR_FIRE) > 0)
			{
			increase_factor = NEWUPGRAREALEXX_MGR->AlexXUpGradeConfig.SetupGrade_AlexX[ELEMENT_ATTACK_REGIST] * 1.00;
			AlexX_WhatTheFuckAttrAttack.SetElement_Val(ATTR_FIRE, pItemInfo->AttrAttack.GetElement_Val(ATTR_FIRE) * pow(increase_factor, Grade));
			}
			if (pItemInfo->AttrAttack.GetElement_Val(ATTR_WATER) > 0)
			{
			increase_factor = NEWUPGRAREALEXX_MGR->AlexXUpGradeConfig.SetupGrade_AlexX[ELEMENT_ATTACK_REGIST] * 1.00;
			AlexX_WhatTheFuckAttrAttack.SetElement_Val(ATTR_WATER, pItemInfo->AttrAttack.GetElement_Val(ATTR_WATER) * pow(increase_factor, Grade));
			}
			if (pItemInfo->AttrAttack.GetElement_Val(ATTR_TREE) > 0)
			{
			increase_factor = NEWUPGRAREALEXX_MGR->AlexXUpGradeConfig.SetupGrade_AlexX[ELEMENT_ATTACK_REGIST] * 1.00;
			AlexX_WhatTheFuckAttrAttack.SetElement_Val(ATTR_TREE, pItemInfo->AttrAttack.GetElement_Val(ATTR_TREE) * pow(increase_factor, Grade));
			}
			if (pItemInfo->AttrAttack.GetElement_Val(ATTR_IRON) > 0)
			{
			increase_factor = NEWUPGRAREALEXX_MGR->AlexXUpGradeConfig.SetupGrade_AlexX[ELEMENT_ATTACK_REGIST] * 1.00;
			AlexX_WhatTheFuckAttrAttack.SetElement_Val(ATTR_IRON, pItemInfo->AttrAttack.GetElement_Val(ATTR_IRON) * pow(increase_factor, Grade));
			}
			if (pItemInfo->AttrAttack.GetElement_Val(ATTR_EARTH) > 0)
			{
			increase_factor = NEWUPGRAREALEXX_MGR->AlexXUpGradeConfig.SetupGrade_AlexX[ELEMENT_ATTACK_REGIST] * 1.00;
			AlexX_WhatTheFuckAttrAttack.SetElement_Val(ATTR_EARTH, pItemInfo->AttrAttack.GetElement_Val(ATTR_EARTH) * pow(increase_factor, Grade));
			}

			if (pItemInfo->AttrRegist.GetElement_Val(ATTR_FIRE) > 0)
			{
			increase_factor = NEWUPGRAREALEXX_MGR->AlexXUpGradeConfig.SetupGrade_AlexX[ELEMENT_ATTACK_REGIST] * 1.00;
			AlexX_WhatTheFuckAttrRegist.SetElement_Val(ATTR_FIRE, pItemInfo->AttrRegist.GetElement_Val(ATTR_FIRE) * pow(increase_factor, Grade));
			}
			if (pItemInfo->AttrRegist.GetElement_Val(ATTR_WATER) > 0)
			{
			increase_factor = NEWUPGRAREALEXX_MGR->AlexXUpGradeConfig.SetupGrade_AlexX[ELEMENT_ATTACK_REGIST] * 1.00;
			AlexX_WhatTheFuckAttrRegist.SetElement_Val(ATTR_WATER, pItemInfo->AttrRegist.GetElement_Val(ATTR_WATER) * pow(increase_factor, Grade));
			}
			if (pItemInfo->AttrRegist.GetElement_Val(ATTR_TREE) > 0)
			{
			increase_factor = NEWUPGRAREALEXX_MGR->AlexXUpGradeConfig.SetupGrade_AlexX[ELEMENT_ATTACK_REGIST] * 1.00;
			AlexX_WhatTheFuckAttrRegist.SetElement_Val(ATTR_TREE, pItemInfo->AttrRegist.GetElement_Val(ATTR_TREE) * pow(increase_factor, Grade));
			}
			if (pItemInfo->AttrRegist.GetElement_Val(ATTR_IRON) > 0)
			{
			increase_factor = NEWUPGRAREALEXX_MGR->AlexXUpGradeConfig.SetupGrade_AlexX[ELEMENT_ATTACK_REGIST] * 1.00;
			AlexX_WhatTheFuckAttrRegist.SetElement_Val(ATTR_IRON, pItemInfo->AttrRegist.GetElement_Val(ATTR_IRON) * pow(increase_factor, Grade));
			}
			if (pItemInfo->AttrRegist.GetElement_Val(ATTR_EARTH) > 0)
			{
			increase_factor = NEWUPGRAREALEXX_MGR->AlexXUpGradeConfig.SetupGrade_AlexX[ELEMENT_ATTACK_REGIST] * 1.00;
			AlexX_WhatTheFuckAttrRegist.SetElement_Val(ATTR_EARTH, pItemInfo->AttrRegist.GetElement_Val(ATTR_EARTH) * pow(increase_factor, Grade));
			}

			item_stats->AttributeAttack.AddATTRIBUTE_VAL(AlexX_WhatTheFuckAttrAttack, ApplyRate);
			item_stats->AttributeResist.AddATTRIBUTE_VAL(AlexX_WhatTheFuckAttrRegist, ApplyRate);*/

			bool kuy1 = false;
			bool kuy2 = false;
			for (int WhatTheFuck = ATTR_FIRE; WhatTheFuck <= ATTR_MAX; ++WhatTheFuck)
			{
				// T T AlexX So Sad
				increase_factor = NEWUPGRAREALEXX_MGR->AlexXUpGradeConfig.SetupGrade_AlexX[ELEMENT_ATTACK_REGIST] * 1.00;

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

	item_stats->AttributeResist.CheckLimit(10);//ÏÞÖÆ·¨¿¹ÉÏÏÞ

	if(pPlayer->IsMussangMode())
	{
		WORD stat = pPlayer->GetMussangStat();

		item_stats->GenGol += stat;
		item_stats->MinChub += stat;
		item_stats->CheRyuk += stat;
		item_stats->SimMaek += stat;
	}

	if(pPlayer->GetFame() > 0)
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

	pPlayer->GetPetManager()->GetPetBuffResultRt(ePB_MasterAllStatUp, item_stats);


	pPlayer->GetPetManager()->GetPetBuffResultRt(ePB_MasterAllStatRound, item_stats);



	/////////////////// 2007. 6. 11. CBH - ¼¼Æ®¾ÆÀÌÅÆ ´É·ÂÄ¡ Ãß°¡ ////////////////
	CalcSetItemStats(pPlayer);
	/////////////////////////////////////////////////////////////////////////////

	// magi82 - UniqueItem(070626)
	CalcUniqueItemStats(pPlayer);
	
	// ¾ÆÀÌÅÛ ¼öÄ¡°¡ º¯°æµÇ¸é Ä³¸¯ÅÍ ¼öÄ¡µµ º¯°æ
	CalcCharStats(pPlayer);
}

// Ä³¸¯°ÔÀÓIn
// ·¹º§¾÷½Ã ( ´É·ÂÄ¡ ¹Ù²ð¶§)
// ¹«°ø¹ßµ¿½Ã(+´É·ÂÄ¡) : ±¸Á¶Ã¼¸¦ ÇÏ³ª ´õ µÖ¾ß ÇÏ³ª?
// ShopItem»ç¿ë½ÃÀÇ ¿É¼ÇÃß°¡
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
	char_stats->PhysicalDefense = (WORD)(pPlayer->GetCheRyuk()/1.5);
	// 10/27 ¼öÄ¡ º¯°æÀü	char_stats->PhysicalDefense = pHeroInfo.wCheRyuk/3;
	char_stats->Critical = ( pPlayer->GetGenGol()) / 4;
	//////////////////////////////////////////////////////
	// 06. 07 ³»°ø ÀûÁß(ÀÏ°Ý) - ÀÌ¿µÁØ
	char_stats->Decisive = ( pPlayer->GetSimMek()) / 4;
	//////////////////////////////////////////////////////
#endif

#ifdef _MAPSERVER_
	CCharacterCalcManager::CalcMaxLife(pPlayer);
	CCharacterCalcManager::CalcMaxShield(pPlayer);
	CCharacterCalcManager::CalcMaxNaeRyuk(pPlayer);
#endif
}


void CStatsCalcManager::CalcCharLife(PLAYERTYPE* pPlayer)
{
#ifdef _MAPSERVER_
	CCharacterCalcManager::CalcMaxLife(pPlayer);
#endif
}
void CStatsCalcManager::CalcCharShield(PLAYERTYPE* pPlayer)
{
#ifdef _MAPSERVER_
	CCharacterCalcManager::CalcMaxShield(pPlayer);
#endif
}
void CStatsCalcManager::CalcCharNaeruyk(PLAYERTYPE* pPlayer)
{
#ifdef _MAPSERVER_
	CCharacterCalcManager::CalcMaxNaeRyuk(pPlayer);
#endif
}

void CStatsCalcManager::CalcTitanItemStats( PLAYERTYPE* pPlayer )
{
	//Å¸ÀÌÅº ¾ÆÀÌÅÛ ¼Ó¼º±¸Á¶Ã¼¸¦ °¡Á®¿Í
	//ÀåÂøÃ¢ ½½·ÔÀÇ ¾ÆÀÌÅÛ Á¤º¸·Î °»½ÅÇÑ´Ù.

	titan_calc_stats* pItemStat = pPlayer->GetTitanManager()->GetTitanItemStats();

	memset(pItemStat, 0, sizeof(titan_calc_stats));

	for( POSTYPE part = TP_TITANWEAR_START; part < TP_TITANWEAR_END; ++part )
	{
		const ITEMBASE* pTargetItemBase = ITEMMGR->GetItemInfoAbsIn(pPlayer, part);

		if(NULL == pTargetItemBase)
			continue;

		if(0 == pTargetItemBase->dwDBIdx)
			continue;

		// 070621 ±âÈ¹º¯°æ
		TITAN_ENDURANCE_ITEMINFO* pInfo = NULL;
		pInfo = (pPlayer->GetTitanManager())->GetTitanItemEnduranceInfo(pTargetItemBase->dwDBIdx);
		if(NULL == pInfo)
			continue;

		if( 0 == pInfo->Endurance )	//³»±¸µµ ¾øÀ»½Ã ¿É¼Ç ºñÀû¿ë.
			continue;

		ITEM_INFO* pItemInfo = ITEMMGR->GetItemInfo(pTargetItemBase->wIconIdx);

		if(NULL == pItemInfo)
			continue;

		pItemStat->MaxFuel				+= pItemInfo->Life;
		pItemStat->MaxSpell				+= pItemInfo->NaeRyuk;
		pItemStat->PhysicalDefense		+= pItemInfo->PhyDef;
		pItemStat->Critical				+= pItemInfo->CriticalPercent;
		pItemStat->MeleeAttackPowerMin	+= pItemInfo->MeleeAttackMin;
		pItemStat->MeleeAttackPowerMax	+= pItemInfo->MeleeAttackMax;
		pItemStat->RangeAttackPowerMin	+= pItemInfo->RangeAttackMin;
		pItemStat->RangeAttackPowerMax	+= pItemInfo->RangeAttackMax;
		pItemStat->AttackRange			+= pItemInfo->MugongNum;	//	¹«°ø¼­	¿¬°á ¹«°ø  ¹øÈ£
		pItemStat->MoveSpeed			+= pItemInfo->AllPlus_Value;//	¹«°ø À¯Çüº° ¹«°ø+ Á¤µµ
		pItemStat->AttributeAttack.AddATTRIBUTE_VAL(pItemInfo->AttrAttack,1);
		pItemStat->AttributeResist.AddATTRIBUTE_VAL(pItemInfo->AttrRegist,1);

	}

}

/////////////////// 2007. 6. 11. CBH - ¼¼Æ®¾ÆÀÌÅÆ ´É·ÂÄ¡ Ãß°¡ ////////////////
void CStatsCalcManager::CalcSetItemStats(CPlayer* pPlayer)
{
	pPlayer->ClearSetitemOption();

	SET_ITEM_INFO setItemInfo[MAX_SETITEM_KIND_NUM];

	memset(setItemInfo, 0, sizeof(SET_ITEM_INFO) * MAX_SETITEM_KIND_NUM);

	//ÀåÂøµÈ ¼¼Æ®¾ÆÀÌÅÆÀÇ Á¾·ù¿Í °¹¼ö¸¦ °Ë»öÇÑ´Ù.
	for(POSTYPE part = TP_WEAR_START ; part < TP_WEAR_END ; ++part )
	{
		const ITEMBASE * pTargetItemBase = ITEMMGR->GetItemInfoAbsIn(pPlayer, part);

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

	//¹è¿­¿¡ ÀúÀåµÈ ¼¼Æ®¾ÆÀÌÅÆ Á¤º¸¸¦ °¡Áö°í ½ºÅÈ Àû¿ë
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
						pPlayer->AddSetitemOption(pSetItemOption);
					}					
				}				
			}			
		}
	}
}


/////////////////////////////////////////////////////////////////////////////

// magi82 - UniqueItem(070626)
void CStatsCalcManager::CalcUniqueItemStats(PLAYERTYPE* pPlayer)
{
	// À¯´ÏÅ© °ü·Ã ½ºÅÝÀ» ÃÊ±âÈ­ÈÄ ´Ù½Ã ¼¼ÆÃ
	pPlayer->ClearUniqueItemOption();

	for( POSTYPE pos = TP_WEAR_START; pos < TP_WEAR_END; pos++ )
	{
		const ITEMBASE* pItemBase = ITEMMGR->GetItemInfoAbsIn(pPlayer, pos);
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

            pPlayer->AddUniqueItemOption(pUniqueItemInfo);
		}
	}
}
//////////////////////////////////////////////////////////////////////////


