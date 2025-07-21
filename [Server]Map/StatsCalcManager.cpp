



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
#endif

#include "CharacterCalcManager.h"
#include "ItemManager.h"

#include "FameManager.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CStatsCalcManager::CStatsCalcManager()
{
	// 2014-11-20 斤口줄珂
	m_StoneItemList.Initialize(5);
}

CStatsCalcManager::~CStatsCalcManager()
{
	// 2014-11-20 斤口줄珂
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
DWORD CalStatusGradeInt(DWORD Status, WORD Grade)
{
	float getdmgmin = Status;
	for (int i = 0; i < Grade; i++)
	{
		getdmgmin = getdmgmin + (getdmgmin * 2.80 / 100);//욺훙2.5 //꼿눈펜3.5
	}
	int CalGrade = static_cast<int>(getdmgmin);
	return CalGrade;
}

float CalStatusGradefloat(float Status, WORD Grade)
{
	float getdmgmin = Status;
	for (int i = 0; i < Grade; i++)
	{
		getdmgmin = getdmgmin + (getdmgmin * 2.80 / 100);//욺훙2.5 //꼿눈펜3.5
	}
	int CalGrade = static_cast<int>(getdmgmin);
	return (float)getdmgmin;
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

// 장착창에 아이템 장착,해제시
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

		// yh 처음 들어갈때 문제가... -_-a
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
			item_stats->MeleeAttackPowerAddRateMin += CalStatusGradeInt(pItemInfo->MeleeAttackMin, pTargetItemBase->Grade30) * ApplyRate * 0.01f;
			item_stats->MeleeAttackPowerAddRateMax += CalStatusGradeInt(pItemInfo->MeleeAttackMax, pTargetItemBase->Grade30) * ApplyRate * 0.01f;
			item_stats->RangeAttackPowerAddRateMin += CalStatusGradeInt(pItemInfo->RangeAttackMin, pTargetItemBase->Grade30) * ApplyRate * 0.01f;
			item_stats->RangeAttackPowerAddRateMax += CalStatusGradeInt(pItemInfo->RangeAttackMax, pTargetItemBase->Grade30) * ApplyRate * 0.01f;
		}
		else
		{
			// 다른 것들은 그냥 공격계산식에 더해줌
			item_stats->MeleeAttackPowerMin += (WORD)(CalStatusGradeInt(pItemInfo->MeleeAttackMin, pTargetItemBase->Grade30) * ApplyRate);
			item_stats->MeleeAttackPowerMax += (WORD)(CalStatusGradeInt(pItemInfo->MeleeAttackMax, pTargetItemBase->Grade30) * ApplyRate);
			item_stats->RangeAttackPowerMin += (WORD)(CalStatusGradeInt(pItemInfo->RangeAttackMin, pTargetItemBase->Grade30) * ApplyRate);
			item_stats->RangeAttackPowerMax += (WORD)(CalStatusGradeInt(pItemInfo->RangeAttackMax, pTargetItemBase->Grade30) * ApplyRate);
		}
		//景喝橄昑
		if (pItemInfo->WeaponType == 2)
		{//톡덖嶠포槨화珂/첼5듐竟醴속寧듐묑샌
			item_stats->MeleeAttackPowerMin += pPlayer->GetCheRyuk() / 10;
			item_stats->MeleeAttackPowerMax += pPlayer->GetCheRyuk() / 10;
			item_stats->RangeAttackPowerMin += pPlayer->GetCheRyuk() / 10;
			item_stats->RangeAttackPowerMax += pPlayer->GetCheRyuk() / 10;
		}
		item_stats->PhysicalDefense += (WORD)(CalStatusGradeInt(pItemInfo->PhyDef, pTargetItemBase->Grade30) * ApplyRate);//莖카세減錦맣 2023 - 12 - 29 錦릿65535 섐掘
		item_stats->GenGol += (WORD)(CalStatusGradeInt(pItemInfo->GenGol, pTargetItemBase->Grade30) * ApplyRate);
		item_stats->MinChub += (WORD)(CalStatusGradeInt(pItemInfo->MinChub, pTargetItemBase->Grade30) * ApplyRate);
		item_stats->CheRyuk += (WORD)(CalStatusGradeInt(pItemInfo->CheRyuk, pTargetItemBase->Grade30) * ApplyRate);
		item_stats->SimMaek += (WORD)(CalStatusGradeInt(pItemInfo->SimMek, pTargetItemBase->Grade30) * ApplyRate);
		item_stats->MaxLife += (DWORD)(CalStatusGradeInt(pItemInfo->Life, pTargetItemBase->Grade30) * ApplyRate);
		item_stats->MaxShield += (DWORD)(CalStatusGradeInt(pItemInfo->Shield, pTargetItemBase->Grade30) * ApplyRate);
		item_stats->MaxNaeRyuk += (DWORD)(CalStatusGradeInt(pItemInfo->NaeRyuk, pTargetItemBase->Grade30) * ApplyRate);
		
		ATTRIBUTEATTACK AttrAtkPercent_Cal;
		AttrAtkPercent_Cal.SetElement_Val(ATTR_FIRE, CalStatusGradefloat(pItemInfo->AttrAttack.GetElement_Val(ATTR_FIRE), pTargetItemBase->Grade30));
		AttrAtkPercent_Cal.SetElement_Val(ATTR_WATER, CalStatusGradefloat(pItemInfo->AttrAttack.GetElement_Val(ATTR_WATER), pTargetItemBase->Grade30));
		AttrAtkPercent_Cal.SetElement_Val(ATTR_TREE, CalStatusGradefloat(pItemInfo->AttrAttack.GetElement_Val(ATTR_TREE), pTargetItemBase->Grade30));
		AttrAtkPercent_Cal.SetElement_Val(ATTR_EARTH, CalStatusGradefloat(pItemInfo->AttrAttack.GetElement_Val(ATTR_EARTH), pTargetItemBase->Grade30));
		AttrAtkPercent_Cal.SetElement_Val(ATTR_IRON, CalStatusGradefloat(pItemInfo->AttrAttack.GetElement_Val(ATTR_IRON), pTargetItemBase->Grade30));
		item_stats->AttributeAttack.AddATTRIBUTE_VAL(AttrAtkPercent_Cal, ApplyRate);


		ATTRIBUTEREGIST AttrResPercent_Cal;
		AttrResPercent_Cal.SetElement_Val(ATTR_FIRE, CalStatusGradefloat(pItemInfo->AttrRegist.GetElement_Val(ATTR_FIRE), pTargetItemBase->Grade30));
		AttrResPercent_Cal.SetElement_Val(ATTR_WATER, CalStatusGradefloat(pItemInfo->AttrRegist.GetElement_Val(ATTR_WATER), pTargetItemBase->Grade30));
		AttrResPercent_Cal.SetElement_Val(ATTR_TREE, CalStatusGradefloat(pItemInfo->AttrRegist.GetElement_Val(ATTR_TREE), pTargetItemBase->Grade30));
		AttrResPercent_Cal.SetElement_Val(ATTR_EARTH, CalStatusGradefloat(pItemInfo->AttrRegist.GetElement_Val(ATTR_EARTH), pTargetItemBase->Grade30));
		AttrResPercent_Cal.SetElement_Val(ATTR_IRON, CalStatusGradefloat(pItemInfo->AttrRegist.GetElement_Val(ATTR_IRON), pTargetItemBase->Grade30));
		item_stats->AttributeResist.AddATTRIBUTE_VAL(AttrResPercent_Cal, ApplyRate);

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

        // 2014-11-20 槻벎茄君 륩蛟포똥

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
		//莖카 PVP콜㈙펃�[
		item_stats->PVPCri += pItemInfo->PVPCri;
		item_stats->PVPAttack += pItemInfo->PVPAttack;
		item_stats->PVPDef += pItemInfo->PVPDef;
		item_stats->PVPADef += pItemInfo->PVPADef;
		item_stats->PVPHit += pItemInfo->PVPHit;
		item_stats->PVPADodge += pItemInfo->PVPADodge;
		item_stats->PVPStunResist += pItemInfo->PVPStunResist;
		item_stats->PVPStunTimeReduce += pItemInfo->PVPStunTimeReduce;
		//weiye  냥낀섬깎茄君  찮냥낀250000, 茄君롸섬  2018-01-24 

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

	item_stats->AttributeResist.CheckLimit(10);

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



	/////////////////// 2007. 6. 11. CBH - 세트아이탬 능력치 추가 ////////////////
	CalcSetItemStats(pPlayer);
	/////////////////////////////////////////////////////////////////////////////

	// magi82 - UniqueItem(070626)
	CalcUniqueItemStats(pPlayer);
	CalcSetItemQualityStats(pPlayer); //틔醴
	// 아이템 수치가 변경되면 캐릭터 수치도 변경
	CalcCharStats(pPlayer);
}

// 캐릭게임In
// 레벨업시 ( 능력치 바뀔때)
// 무공발동시(+능력치) : 구조체를 하나 더 둬야 하나?
// ShopItem사용시의 옵션추가
void CStatsCalcManager::CalcCharStats(PLAYERTYPE* pPlayer)
{
	player_calc_stats * char_stats = pPlayer->GetCharStats();
	Clear(char_stats);

	HERO_TOTALINFO pHeroInfo;
	pPlayer->GetHeroTotalInfo(&pHeroInfo);
	// 아이템 장착한거에 따른 캐릭터 수치 셋팅
	char_stats->MinChub = pHeroInfo.wMinChub;
	char_stats->GenGol = pHeroInfo.wGenGol;

#ifdef _JAPAN_LOCAL_
	char_stats->PhysicalDefense = pPlayer->GetCheRyuk() * 10 / 15;
	char_stats->Critical = ( pPlayer->GetGenGol() / 4 ) + ( pPlayer->GetMinChub() / 6 );
#else
	char_stats->PhysicalDefense = (WORD)(pPlayer->GetCheRyuk()/1.5);
	// 10/27 수치 변경전	char_stats->PhysicalDefense = pHeroInfo.wCheRyuk/3;
	char_stats->Critical = ( pPlayer->GetGenGol()) / 4;
	//////////////////////////////////////////////////////
	// 06. 07 내공 적중(일격) - 이영준
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
	//타이탄 아이템 속성구조체를 가져와
	//장착창 슬롯의 아이템 정보로 갱신한다.

	titan_calc_stats* pItemStat = pPlayer->GetTitanManager()->GetTitanItemStats();

	memset(pItemStat, 0, sizeof(titan_calc_stats));

	for( POSTYPE part = TP_TITANWEAR_START; part < TP_TITANWEAR_END; ++part )
	{
		const ITEMBASE* pTargetItemBase = ITEMMGR->GetItemInfoAbsIn(pPlayer, part);

		if(NULL == pTargetItemBase)
			continue;

		if(0 == pTargetItemBase->dwDBIdx)
			continue;

		// 070621 기획변경
		TITAN_ENDURANCE_ITEMINFO* pInfo = NULL;
		pInfo = (pPlayer->GetTitanManager())->GetTitanItemEnduranceInfo(pTargetItemBase->dwDBIdx);
		if(NULL == pInfo)
			continue;

		if( 0 == pInfo->Endurance )	//내구도 없을시 옵션 비적용.
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
		pItemStat->AttackRange			+= pItemInfo->MugongNum;	//	무공서	연결 무공  번호
		pItemStat->MoveSpeed			+= pItemInfo->AllPlus_Value;//	무공 유형별 무공+ 정도
		pItemStat->AttributeAttack.AddATTRIBUTE_VAL(pItemInfo->AttrAttack,1);
		pItemStat->AttributeResist.AddATTRIBUTE_VAL(pItemInfo->AttrRegist,1);

	}

}

/////////////////// 2007. 6. 11. CBH - 세트아이탬 능력치 추가 ////////////////
void CStatsCalcManager::CalcSetItemStats(CPlayer* pPlayer)
{
	pPlayer->ClearSetitemOption();

	SET_ITEM_INFO setItemInfo[MAX_SETITEM_KIND_NUM];

	memset(setItemInfo, 0, sizeof(SET_ITEM_INFO) * MAX_SETITEM_KIND_NUM);

	//장착된 세트아이탬의 종류와 갯수를 검색한다.
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

		//장착된 세트아이탬의 종류와 갯수정보를 배열에 저장
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

	//배열에 저장된 세트아이탬 정보를 가지고 스탯 적용
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
	// 유니크 관련 스텟을 초기화후 다시 세팅
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

		// 유니크 아이템이라면..
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
void CStatsCalcManager::CalcSetItemQualityStats(CPlayer* pPlayer)
{
	pPlayer->ClearSetitemQualityOption();

	//장착된 세트아이탬의 종류와 갯수를 검색한다.
	for (POSTYPE pos = TP_WEAR_START; pos < TP_WEAR_END; ++pos)
	{
		const ITEMBASE* pTargetItemBase = ITEMMGR->GetItemInfoAbsIn(pPlayer, pos);

		if (pTargetItemBase == NULL)
		{
			continue;
		}

		if (pTargetItemBase->dwDBIdx == 0)
		{
			continue;
		}

		ITEM_INFO* pItemInfo = ITEMMGR->GetItemInfo(pTargetItemBase->wIconIdx);

		if (pItemInfo == NULL)
			continue;
		SET_ITEMQUALITY_OPTION* pSetItemOption = NULL;
		pSetItemOption = ITEMMGR->GetSetItemQualityOption(pTargetItemBase->ItemQuality, pTargetItemBase->ItemEntry1, pTargetItemBase->ItemEntry2);

		pPlayer->AddSetitemQualityOption(pSetItemOption);
	}
}

