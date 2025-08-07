



// AttackManager.cpp: implementation of the CAttackManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "AttackManager.h"
#include "Network.h"
#include "PackedData.h"
#include "ServerSystem.h"
#include "CharMove.h"
#include "RegenNPC.h"
#include "UserTable.h"
#include "ObjectStateManager.h"

#include "PartyManager.h"
#include "Party.h"
#include "SiegeWarMgr.h"
#include "Player.h"
#include "PetManager.h"
#include "GuildManager.h"
#include "ItemManager.h"
#include "..\[CC]Header\CommonCalcFunc.h"

#include "..\[CC]Header\CommonCalcFunc.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

#define SHIELD_COMBO_DAMAGE			0.5f
#define SHIELD_OUT_MUGONG_DAMAGE	0.7f
#define SHIELD_IN_MUGONG_DAMAGE		0.7f

CAttackManager::CAttackManager()
{
	m_nDamageRate = 100.0f;	// magi82 cheat damage Ã³À½¿¡´Â 100%ÀÇ µ¥¹ÌÁö·Î Àû¿ë(Ä¡Æ®Å°¸¦ ¾²¸é º¯°æµÊ)
}

CAttackManager::~CAttackManager()
{

}

void CAttackManager::sendDieMsg(CObject * pAttacker,CObject* pTarget)
{
	MSG_DWORD2 m2c;
	m2c.Category = MP_USERCONN;


	if(pTarget->GetObjectKind() & eObjectKind_Monster)
		m2c.Protocol = MP_USERCONN_MONSTER_DIE;
	else if(pTarget->GetObjectKind() == eObjectKind_Player)
		m2c.Protocol = MP_USERCONN_CHARACTER_DIE;

	m2c.dwObjectID = pAttacker->GetID();
	m2c.dwData1 = pAttacker->GetID();
	m2c.dwData2 = pTarget->GetID();

	PACKEDDATA_OBJ->QuickSend(pTarget,&m2c,sizeof(m2c));
}
///ÐÂµÄpvp//ÎïÀíÉËº¦¼ÆËã
DWORD CAttackManager::GetComboPhyDamage(CObject* pAttacker, CObject* pTargetObject, float PhyAttackRate, float fCriticalRate,
	RESULTINFO* pDamageInfo, DWORD AmplifiedPower, float fDecreaseDamageRate)
{
	pDamageInfo->bCritical = m_ATTACKCALC.getCritical(pAttacker, pTargetObject, fCriticalRate);
	double attackPhyDamage = m_ATTACKCALC.getPhysicalAttackPower(pAttacker, PhyAttackRate, pDamageInfo->bCritical);

	// ±©»÷ÅÐ¶¨´¦Àí
// ±©»÷ÅÐ¶¨´¦Àí
	if (pDamageInfo->bCritical)
	{
		//attackPhyDamage *= 1.5f;
		attackPhyDamage *= gEventRate[eEvent_MugongPhyCritical];//±©»÷ÉËº¦±¶ÂÊÉèÖÃ 

		// Íæ¼Ò¹¥»÷Õß£º¸½¼ÓÆ·ÖÊ±©»÷ÉËº¦¡¢ÌØÐ§ÉËº¦
		if (pAttacker->GetObjectKind() == eObjectKind_Player)
		{
			// Æ·ÖÊÌ××°±©»÷¼Ó³É
			const ITEMBASE* pTargetItemBase = ITEMMGR->GetItemInfoAbsIn((CPlayer*)pAttacker, 81);
			if (pTargetItemBase)
			{//ÌØÊâÊôÐÔ
				ITEM_INFO* pItemInfo = ITEMMGR->GetItemInfo(pTargetItemBase->wIconIdx);
				if (pItemInfo && pItemInfo->WeaponType == 3)
				{
					attackPhyDamage += (attackPhyDamage * 0.15f); // ¹¥»÷ÕßÅå´÷µ¶Ê±£¬·ÜÁ¦Ò»»÷ÉËº¦ÌáÉý15%
				}
			}

			// Ì××°±©»÷ÉËº¦°Ù·Ö±È
			float fBonusCrit = ((CPlayer*)pAttacker)->GetSetItemQualityStats()->CriticalDamage * 0.01f;
			attackPhyDamage += (attackPhyDamage * fBonusCrit);

			// Ï¡ÓÐµÀ¾ß¹Ì¶¨±©»÷¼Ó³É
			attackPhyDamage += (double)(((CPlayer*)pAttacker)->GetUniqueItemStats()->nCriDamage);

			if (attackPhyDamage < 0.f)
				attackPhyDamage = 1.f;
		}

		// ±»¹¥»÷ÕßÎªÍæ¼ÒÊ±´¦Àí£º³èÎï¼õ±©»÷ÉËº¦£¨¹Ö´òÈË / ÈË´òÈË£©
		if (pTargetObject->GetObjectKind() == eObjectKind_Player)
		{
			((CPlayer*)pTargetObject)->GetPetManager()->GetPetBuffResultRt(ePB_ReduceCriticalDmg, &attackPhyDamage);
		}
	}


	attackPhyDamage += AmplifiedPower;
	attackPhyDamage *= fDecreaseDamageRate;

	if (pAttacker->GetObjectKind() == eObjectKind_Player && pTargetObject->GetObjectKind() == eObjectKind_Player)
		attackPhyDamage *= gEventRate[eEvent_MugongPhy];//ÔÚ´Ë´¦ÅäÖÃbinÅäÖÃÎïÀí ÉËº¦
	// ===== PvP ¼Ó³É£¨ÐÂÔö£©=====
	if (pAttacker->GetObjectKind() == eObjectKind_Player &&
		pTargetObject->GetObjectKind() == eObjectKind_Player)
	{
		attackPhyDamage *= (1.0f + ((CPlayer*)pAttacker)->GetItemStats()->PVPAttack);
		attackPhyDamage *= (1.0f + ((CPlayer*)pAttacker)->GetAvatarOption()->PVPAttack);
	}
	// ===========================
	// === ´Ì¿ÍÎäÆ÷¶îÍâÎïÀíÉËº¦±¶ÂÊ£¨´Ó bin ÅäÖÃ±í¶ÁÈ¡£©===
	if (pAttacker->GetObjectKind() == eObjectKind_Player)
	{
		const ITEMBASE* pItemBase = ITEMMGR->GetItemInfoAbsIn((CPlayer*)pAttacker, 81); // ÓÒÊÖÎäÆ÷
		if (pItemBase)
		{
			ITEM_INFO* pItemInfo = ITEMMGR->GetItemInfo(pItemBase->wIconIdx);
			if (pItemInfo && pItemInfo->WeaponType == 11) // ´Ì¿ÍÎäÆ÷ÀàÐÍ
			{
				// ¾ÙÀý£ºgEventRate[eEvent_AssassinPhyDmg] = 1.50f£¨±íÊ¾+50%ÉËº¦£©
				attackPhyDamage *= gEventRate[eEvent_AssassinPhyDmg];
			}
		}
	}

#ifdef _JAPAN_LOCAL_
	DWORD ShieldDamage = pTargetObject->CalcShieldDamage((DWORD)(attackPhyDamage * SHIELD_COMBO_DAMAGE));
	attackPhyDamage -= ShieldDamage;
#else
	DWORD ShieldDamage = 0;
	DWORD ReduceDamage = pTargetObject->CalcShieldDamage((DWORD)(attackPhyDamage * SHIELD_COMBO_DAMAGE), ShieldDamage);
	attackPhyDamage -= ReduceDamage;
#endif

	pDamageInfo->ShieldDamage += ShieldDamage;

	double defencePhyLevel = m_ATTACKCALC.getPhyDefenceLevel(pTargetObject, pAttacker);
	attackPhyDamage *= (1.0 - defencePhyLevel);

	if (pAttacker->GetObjectKind() == eObjectKind_Player && pTargetObject->GetObjectKind() & eObjectKind_Monster)
	{
		attackPhyDamage *= gEventRate[eEvent_DamageRate];
	}
	if (pAttacker->GetObjectKind() & eObjectKind_Monster && pTargetObject->GetObjectKind() == eObjectKind_Player)
	{
		attackPhyDamage *= gEventRate[eEvent_DamageReciveRate];
	}

	if (attackPhyDamage < 1)
		attackPhyDamage = 1;

	float fdam = 0.0f;
	if (pAttacker->GetObjectKind() == eObjectKind_Player)
	{
		attackPhyDamage *= (((CPlayer*)pAttacker)->GetShopItemStats()->ComboDamage * 0.01f + 1.0f);

		if (((CPlayer*)pAttacker)->GetGuildIdx())
			GUILDMGR->GetGuildPlustimeRt(((CPlayer*)pAttacker)->GetGuildIdx(), eGPT_DamageUp, &attackPhyDamage);

		((CPlayer*)pAttacker)->GetPetManager()->GetPetBuffResultRt(ePB_Demage_Percent, &attackPhyDamage);

		if (pTargetObject->GetObjectKind() & eObjectKind_Monster)
		{
			if (g_pServerSystem->GetMap()->IsMapKind(eBossMap))
				goto CalcEnd;

			fdam = (((CPlayer*)pAttacker)->GetAvatarOption()->TargetPhyDefDown * 0.01f) + (((CPlayer*)pAttacker)->GetSetItemQualityStats()->TargetPhyDefDown * 0.01f);
			//attackPhyDamage *= (fdam+1.0f);
			attackPhyDamage *= (fdam + 1.0f) * gEventRate[eEvent_MugongKillMonster];//PVEÉËº¦±¶ÂÊ

		}
		if (pTargetObject->GetObjectKind() == eObjectKind_Player)
		{//PVPÆÆ¼×
			fdam = (((CPlayer*)pAttacker)->GetSetItemQualityStats()->PlayerPhyDefDown * 0.01f);//PVPÆÆ¼×
			attackPhyDamage *= (fdam + 1.0f);

		}
		if (((CPlayer*)pAttacker)->IsMussangMode() && ((CPlayer*)pAttacker)->GetAvatarOption()->MussangDamage)
			attackPhyDamage *= (((CPlayer*)pAttacker)->GetAvatarOption()->MussangDamage * 0.01f + 1.0f);

		attackPhyDamage += (double)(((CPlayer*)pAttacker)->GetUniqueItemStats()->nPhyDamage);
		if (attackPhyDamage < 0.f)
			attackPhyDamage = 1.f;
	}
	else if (pAttacker->GetObjectKind() & eObjectKind_Monster)
	{
		if (pTargetObject->GetObjectKind() == eObjectKind_Player)
		{
			if (g_pServerSystem->GetMap()->IsMapKind(eBossMap))
				goto CalcEnd;

			fdam = ((CPlayer*)pTargetObject)->GetAvatarOption()->TargetAtkDown * 0.01f;
			attackPhyDamage *= (1.0f - fdam);
		}
	}

CalcEnd:
	return (DWORD)attackPhyDamage;
}

//¾ÉµÄ
//DWORD CAttackManager::GetComboPhyDamage(CObject* pAttacker,CObject* pTargetObject,float PhyAttackRate,float fCriticalRate,
//										RESULTINFO* pDamageInfo,DWORD AmplifiedPower,float fDecreaseDamageRate )
//{
//	pDamageInfo->bCritical = m_ATTACKCALC.getCritical(pAttacker,pTargetObject,fCriticalRate);
//	double attackPhyDamage = m_ATTACKCALC.getPhysicalAttackPower(pAttacker,PhyAttackRate,pDamageInfo->bCritical);
//
//	// RaMa - Å©¸®Æ¼ÄÃÀÏ¶§ ½ºÅÏÈ®·ü Ãß°¡ÇØ¾ßÇÔ
//	//#ifndef _JAPAN_LOCAL_	//´Ù¸¥°÷(getPlayerPhysicalAttackPower) ¸·¾Ò´Ù.
//	if(pDamageInfo->bCritical)
//	{
//		attackPhyDamage *= 1.5f;
//		/*
//		// ½ºÅÏ
//		if( pAttacker->GetObjectKind() == eObjectKind_Player )
//		if((rand()%100) < ((CPlayer*)pAttacker)->GetShopItemStats()->StunByCri)
//		{
//		// RaMa - 04.11.18
//		// ¾ÆÀÌÅÛ¸ô¿¡°üÇÑ ¼¼ºÎ°èÈ¹¼­´Â ´ÙÀ½ÁÖÁß( ~11.27)¿¡ ¹Þ±â·Î ÇÔ.
//		// pTargetObject->StartSpecialState( eSpecialState_Stun, 10, 0, 0, 0, FindEffectNum("monster_stun_s.beff"), 0 );
//		}
//		}*/
//		//SW060906 ½Å±ÔÆê
//		if( (pAttacker->GetObjectKind() & eObjectKind_Monster) && pTargetObject->GetObjectKind() == eObjectKind_Player )
//		{
//			((CPlayer*)pTargetObject)->GetPetManager()->GetPetBuffResultRt(ePB_ReduceCriticalDmg, &attackPhyDamage);
//		}
//
//		// magi82 - UniqueItem(070627)
//		if( pAttacker->GetObjectKind() == eObjectKind_Player )
//		{
//			attackPhyDamage += (double)(((CPlayer*)pAttacker)->GetUniqueItemStats()->nCriDamage);
//			if(attackPhyDamage < 0.f)
//				attackPhyDamage = 1.f;
//		}
//	}
//	//#endif
//
//	attackPhyDamage += AmplifiedPower;
//
//	//041213 KES
//	attackPhyDamage *= fDecreaseDamageRate;
//	//
//
//	//PvP ÀÏ¶§´Â °ø°Ý·ÂÀÇ 50%¸¸
//	if( pAttacker->GetObjectKind() == eObjectKind_Player &&
//
//		pTargetObject->GetObjectKind() == eObjectKind_Player )
//#ifdef _JAPAN_LOCAL_
//		attackPhyDamage *= 0.25f;
//#else
//		attackPhyDamage *= 0.5f;
//#endif
//
//#ifdef _JAPAN_LOCAL_
//
//	DWORD ShieldDamage = pTargetObject->CalcShieldDamage((DWORD)(attackPhyDamage * SHIELD_COMBO_DAMAGE));
//	attackPhyDamage -= ShieldDamage;
//#else
//	// 06. 03 ±¹³»¹«½Ö - ÀÌ¿µÁØ
//	DWORD ShieldDamage = 0;
//	DWORD ReduceDamage = pTargetObject->CalcShieldDamage((DWORD)(attackPhyDamage * SHIELD_COMBO_DAMAGE), ShieldDamage);
//	attackPhyDamage -= ReduceDamage;
//#endif
//
//	pDamageInfo->ShieldDamage += ShieldDamage;
//
//	double defencePhyLevel = m_ATTACKCALC.getPhyDefenceLevel(pTargetObject,pAttacker);
//	attackPhyDamage *= (1-defencePhyLevel);
//
//	// RaMa - 04.11.24   ->µ¥¹ÌÁöºñÀ²
//	if(pAttacker->GetObjectKind() == eObjectKind_Player && pTargetObject->GetObjectKind() & eObjectKind_Monster)	
//	{
//		attackPhyDamage *= gEventRate[eEvent_DamageRate];
//	}
//	if(pAttacker->GetObjectKind() & eObjectKind_Monster && pTargetObject->GetObjectKind() == eObjectKind_Player)
//	{
//		attackPhyDamage *= gEventRate[eEvent_DamageReciveRate];
//	}
//
//	if(attackPhyDamage < 1)
//		attackPhyDamage = 1;
//
//	float fdam = 0.0f;
//	// RaMa - 04.11.10    -> ShopItemOptionÃß°¡
//	if(pAttacker->GetObjectKind() == eObjectKind_Player)
//	{
//		attackPhyDamage *= (((CPlayer*)pAttacker)->GetShopItemStats()->ComboDamage*0.01f+1.0f);
//
//		//SW060719 ¹®ÆÄÆ÷ÀÎÆ®
//		if( ((CPlayer*)pAttacker)->GetGuildIdx() )
//			GUILDMGR->GetGuildPlustimeRt( ((CPlayer*)pAttacker)->GetGuildIdx(), eGPT_DamageUp, &attackPhyDamage );
//
//		//PET %°ø°Ý·Â
//		((CPlayer*)pAttacker)->GetPetManager()->GetPetBuffResultRt(ePB_Demage_Percent, &attackPhyDamage);
//
//		// RaMa - 05.10.10 -> ÇØ°ñ°¡¸é ¿É¼ÇÃß°¡
//		if( pTargetObject->GetObjectKind() & eObjectKind_Monster )
//		{
//			//if( g_pServerSystem->GetMapNum() == BOSSMONSTER_MAP ||
//			//	g_pServerSystem->GetMapNum() == BOSSMONSTER_2NDMAP )	goto CalcEnd;
//			if( g_pServerSystem->GetMap()->IsMapKind(eBossMap) )
//				goto CalcEnd;
//
//			fdam = ((CPlayer*)pAttacker)->GetAvatarOption()->TargetPhyDefDown*0.01f;
//			attackPhyDamage *= (fdam+1.0f);
//		}
//		// RaMa - 06.11.13 -> ¹«½Ö¸ðµåÀÏ¶§ °ø°Ý·ÂÁõ°¡ ¾Æ¹ÙÅ¸
//		if( ((CPlayer*)pAttacker)->IsMussangMode() && ((CPlayer*)pAttacker)->GetAvatarOption()->MussangDamage )
//			attackPhyDamage *= (((CPlayer*)pAttacker)->GetAvatarOption()->MussangDamage*0.01f+1.0f);
//
//		// magi82 - UniqueItem(070627)
//		attackPhyDamage += (double)(((CPlayer*)pAttacker)->GetUniqueItemStats()->nPhyDamage);
//		if(attackPhyDamage < 0.f)
//			attackPhyDamage = 1.f;
//	}	
//	else if( pAttacker->GetObjectKind() & eObjectKind_Monster )
//	{
//		// RaMa - 05.10.10 -> ÇØ°ñ°¡¸é ¿É¼ÇÃß°¡
//		if( pTargetObject->GetObjectKind() == eObjectKind_Player )
//		{
//			//if( g_pServerSystem->GetMapNum() == BOSSMONSTER_MAP ||
//			//	g_pServerSystem->GetMapNum() == BOSSMONSTER_2NDMAP )	goto CalcEnd;
//			if( g_pServerSystem->GetMap()->IsMapKind(eBossMap) )
//				goto CalcEnd;
//
//			fdam = ((CPlayer*)pTargetObject)->GetAvatarOption()->TargetAtkDown*0.01f;
//			attackPhyDamage *= (1.0f-fdam);
//		}
//	}
//
//CalcEnd:
//	return (DWORD)attackPhyDamage;
//}


////¾ÉµÄ//////////////////////////////////////////
//DWORD CAttackManager::GetMugongPhyDamage(CObject* pAttacker,CObject* pTargetObject,float PhyAttackRate,float fCriticalRate,
//										 RESULTINFO* pDamageInfo,DWORD AmplifiedPower,float fDecreaseDamageRate )
//{
//	pDamageInfo->bCritical = m_ATTACKCALC.getCritical(pAttacker,pTargetObject,fCriticalRate);
//	double attackPhyDamage = m_ATTACKCALC.getPhysicalAttackPower(pAttacker,PhyAttackRate,pDamageInfo->bCritical );
//
//	// RaMa - Å©¸®Æ¼ÄÃÀÏ¶§ ½ºÅÏÈ®·ü Ãß°¡ÇØ¾ßÇÔ
//	//#ifndef _JAPAN_LOCAL_ //´Ù¸¥°÷(getPlayerPhysicalAttackPower) ¸·¾Ò´Ù.
//	if(pDamageInfo->bCritical)
//	{
//		attackPhyDamage *= 1.5f;
//		/*
//		// ½ºÅÏ
//		if( pAttacker->GetObjectKind() == eObjectKind_Player )
//		if((rand()%100) < ((CPlayer*)pAttacker)->GetShopItemStats()->StunByCri)
//		{
//		// RaMa - 04.11.18
//		// ¾ÆÀÌÅÛ¸ô¿¡°üÇÑ ¼¼ºÎ°èÈ¹¼­´Â ´ÙÀ½ÁÖÁß( ~11.27)¿¡ ¹Þ±â·Î ÇÔ.
//		// pTargetObject->StartSpecialState( eSpecialState_Stun, 10, 0, 0, 0, FindEffectNum("monster_stun_s.beff"), 0 );
//		}
//		*/
//		//SW060906 ½Å±ÔÆê
//		if( (pAttacker->GetObjectKind() & eObjectKind_Monster) && pTargetObject->GetObjectKind() == eObjectKind_Player )
//		{
//			((CPlayer*)pTargetObject)->GetPetManager()->GetPetBuffResultRt(ePB_ReduceCriticalDmg, &attackPhyDamage);
//		}
//
//		if ((pAttacker->GetObjectKind() & eObjectKind_Player) && pTargetObject->GetObjectKind() == eObjectKind_Player)
//		{
//			((CPlayer*)pTargetObject)->GetPetManager()->GetPetBuffResultRt(ePB_ReduceCriticalDmg, &attackPhyDamage);
//		}
//
//		// magi82 - UniqueItem(070627)
//		if( pAttacker->GetObjectKind() == eObjectKind_Player )
//		{
//			attackPhyDamage += (double)(((CPlayer*)pAttacker)->GetUniqueItemStats()->nCriDamage);
//			if(attackPhyDamage < 0.f)
//				attackPhyDamage = 1.f;
//		}
//	}
//	//#endif
//
//	attackPhyDamage += AmplifiedPower;
//
//	//041213 KES
//	attackPhyDamage *= fDecreaseDamageRate;
//	//
//
//	//PvP ÀÏ¶§´Â °ø°Ý·ÂÀÇ 50%¸¸
//	if( pAttacker->GetObjectKind() == eObjectKind_Player &&
//		pTargetObject->GetObjectKind() == eObjectKind_Player )
//#ifdef _JAPAN_LOCAL_
//		attackPhyDamage *= 0.25f;
//#else
//		attackPhyDamage *= 0.5f;
//#endif
//
//#ifdef _JAPAN_LOCAL_
//	DWORD ShieldDamage = pTargetObject->CalcShieldDamage((DWORD)(attackPhyDamage * SHIELD_OUT_MUGONG_DAMAGE));
//	attackPhyDamage -= ShieldDamage;
//#else
//	// 06. 03 ±¹³»¹«½Ö - ÀÌ¿µÁØ
//	DWORD ShieldDamage = 0;
//	DWORD ReduceDamage = pTargetObject->CalcShieldDamage((DWORD)(attackPhyDamage * SHIELD_OUT_MUGONG_DAMAGE), ShieldDamage);
//	attackPhyDamage -= ReduceDamage;
//#endif
//
//	pDamageInfo->ShieldDamage += ShieldDamage;
//
//	double defencePhyLevel = m_ATTACKCALC.getPhyDefenceLevel(pTargetObject,pAttacker);
//	attackPhyDamage *= (1-defencePhyLevel);
//
//	// RaMa - 04.11.24   ->µ¥¹ÌÁöºñÀ²
//	if(pAttacker->GetObjectKind() == eObjectKind_Player && pTargetObject->GetObjectKind() & eObjectKind_Monster)
//	{
//		attackPhyDamage *= gEventRate[eEvent_DamageRate];
//	}
//	if(pAttacker->GetObjectKind() & eObjectKind_Monster && pTargetObject->GetObjectKind() == eObjectKind_Player)
//	{
//		attackPhyDamage *= gEventRate[eEvent_DamageReciveRate];
//	}
//
//	if(attackPhyDamage < 1)
//		attackPhyDamage = 1;
//
//
//	float fdam = 0.0f;
//	// RaMa - 04.11.10    -> ShopItemOptionÃß°¡, AvatarItemOptionÃß°¡(05.08.16)
//	if(pAttacker->GetObjectKind() == eObjectKind_Player)
//	{
//		fdam = (((CPlayer*)pAttacker)->GetShopItemStats()->WoigongDamage*0.01f) + 
//			(((CPlayer*)pAttacker)->GetAvatarOption()->WoigongDamage*0.01f);
//		attackPhyDamage *= (fdam+1.0f);
//
//		//SW060719 ¹®ÆÄÆ÷ÀÎÆ®
//		if( ((CPlayer*)pAttacker)->GetGuildIdx() )
//			GUILDMGR->GetGuildPlustimeRt( ((CPlayer*)pAttacker)->GetGuildIdx(), eGPT_DamageUp, &attackPhyDamage );
//
//		////PET %°ø°Ý·Â========================================================
//		((CPlayer*)pAttacker)->GetPetManager()->GetPetBuffResultRt(ePB_Demage_Percent, &attackPhyDamage);
//
//		// RaMa - 05.10.10 -> ÇØ°ñ°¡¸é ¿É¼ÇÃß°¡
//		if( pTargetObject->GetObjectKind() & eObjectKind_Monster )
//		{
//			//if( g_pServerSystem->GetMapNum() != BOSSMONSTER_MAP &&
//			//	g_pServerSystem->GetMapNum() != BOSSMONSTER_2NDMAP )
//			if( FALSE == g_pServerSystem->GetMap()->IsMapKind(eBossMap) )
//			{
//				fdam = ((CPlayer*)pAttacker)->GetAvatarOption()->TargetPhyDefDown*0.01f;
//				attackPhyDamage *= (fdam+1.0f);
//			}
//		}
//		// RaMa - 06.11.13 -> ¹«½Ö¸ðµåÀÏ¶§ °ø°Ý·ÂÁõ°¡ ¾Æ¹ÙÅ¸
//		if( ((CPlayer*)pAttacker)->IsMussangMode() && ((CPlayer*)pAttacker)->GetAvatarOption()->MussangDamage )
//			attackPhyDamage *= (((CPlayer*)pAttacker)->GetAvatarOption()->MussangDamage*0.01f+1.0f);
//
//		// magi82 - UniqueItem(070627)
//		attackPhyDamage += (double)(((CPlayer*)pAttacker)->GetUniqueItemStats()->nPhyDamage);
//		if(attackPhyDamage < 0.f)
//			attackPhyDamage = 1.f;
//	}	
//	else if( pAttacker->GetObjectKind() & eObjectKind_Monster )
//	{
//		// RaMa - 05.10.10 -> ÇØ°ñ°¡¸é ¿É¼ÇÃß°¡
//		if( pTargetObject->GetObjectKind() == eObjectKind_Player )
//
//		{
//			//if( g_pServerSystem->GetMapNum() != BOSSMONSTER_MAP &&
//			//	g_pServerSystem->GetMapNum() != BOSSMONSTER_2NDMAP )
//			if( FALSE == g_pServerSystem->GetMap()->IsMapKind(eBossMap) )
//			{
//				fdam = ((CPlayer*)pTargetObject)->GetAvatarOption()->TargetAtkDown*0.01f;
//				attackPhyDamage *= (1.0f-fdam);
//			}
//		}
//	}
//
//	//CalcEnd:
//
//	return (DWORD)attackPhyDamage;
//}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Îä¹¦ÎïÀíÉËº¦¼ÆËã
DWORD CAttackManager::GetMugongPhyDamage(CObject* pAttacker, CObject* pTargetObject, float PhyAttackRate, float fCriticalRate,
	RESULTINFO* pDamageInfo, DWORD AmplifiedPower, float fDecreaseDamageRate)
{
	pDamageInfo->bCritical = m_ATTACKCALC.getCritical(pAttacker, pTargetObject, fCriticalRate);
	double attackPhyDamage = m_ATTACKCALC.getPhysicalAttackPower(pAttacker, PhyAttackRate, pDamageInfo->bCritical);

	if (pDamageInfo->bCritical)
	{
		//attackPhyDamage *= 1.5f;
		attackPhyDamage *= gEventRate[eEvent_MugongPhyCritical];//Õ½Ê¿±©»÷ÉËº¦±¶ÂÊÉèÖÃ 

		// ¢Ù Íæ¼ÒÅå´÷ÎäÆ÷ + ×°±¸±©»÷ÉËº¦¼Ó³É
		if (pAttacker->GetObjectKind() == eObjectKind_Player)
		{
			CPlayer* pPlayer = (CPlayer*)pAttacker;

			const ITEMBASE* pTargetItemBase = ITEMMGR->GetItemInfoAbsIn(pPlayer, 81); // 81 = ÎäÆ÷²Û
			if (pTargetItemBase)
			{//ÌØÊâÊôÐÔ
				ITEM_INFO* pItemInfo = ITEMMGR->GetItemInfo(pTargetItemBase->wIconIdx);
				if (pItemInfo && pItemInfo->WeaponType == 3) // µ¶ÀàÎäÆ÷
				{
					attackPhyDamage += (attackPhyDamage * 0.15f); // +15%·ÜÁ¦Ò»»÷ÉËº¦
				}
			}

			// Æ·ÖÊÏµÍ³±©»÷ÉËº¦±¶ÂÊ
			attackPhyDamage += (attackPhyDamage * (pPlayer->GetSetItemQualityStats()->CriticalDamage * 0.01f));

			// Ï¡ÓÐ/ÌØÓÐ×°±¸¸½¼Ó±©»÷ÉËº¦
			attackPhyDamage += (float)pPlayer->GetUniqueItemStats()->nCriDamage;
		}

		// ¢Ú ±»¹¥»÷ÕßÊÇÍæ¼Ò ¡ú ³èÎï±©»÷¼õÉË
		if (pTargetObject->GetObjectKind() == eObjectKind_Player)
		{
			CPlayer* pTargetPlayer = (CPlayer*)pTargetObject;
			float fReduceCritDmg = 0.0f;
			pTargetPlayer->GetPetManager()->GetPetBuffResultRt(ePB_ReduceCriticalDmg, &fReduceCritDmg);

			// °²È«ÅÐ¶¨Óë¼õÉËÓ¦ÓÃ
			if (fReduceCritDmg > 0.0f && fReduceCritDmg <= 1.0f)
				attackPhyDamage *= (1.0f - fReduceCritDmg);
		}
	}


	attackPhyDamage += AmplifiedPower;
	attackPhyDamage *= fDecreaseDamageRate;
	if (pAttacker->GetObjectKind() == eObjectKind_Player && pTargetObject->GetObjectKind() == eObjectKind_Player)
		attackPhyDamage *= gEventRate[eEvent_MugongPhy];//ÔÚ´Ë´¦ÅäÖÃbinÅäÖÃÎä¹¦ÎïÀíÉËº¦

	//  ÐÂÔö PvP ¹¥»÷¼Ó³É£¨ÎÞ gEventRateFile£©
	if (pAttacker->GetObjectKind() == eObjectKind_Player &&
		pTargetObject->GetObjectKind() == eObjectKind_Player)
	{
		attackPhyDamage *= (1.0f + ((CPlayer*)pAttacker)->GetItemStats()->PVPAttack);
		attackPhyDamage *= (1.0f + ((CPlayer*)pAttacker)->GetAvatarOption()->PVPAttack);
	}

#ifdef _JAPAN_LOCAL_
	DWORD ShieldDamage = pTargetObject->CalcShieldDamage((DWORD)(attackPhyDamage * SHIELD_OUT_MUGONG_DAMAGE));
	attackPhyDamage -= ShieldDamage;
#else
	DWORD ShieldDamage = 0;
	DWORD ReduceDamage = pTargetObject->CalcShieldDamage((DWORD)(attackPhyDamage * SHIELD_OUT_MUGONG_DAMAGE), ShieldDamage);
	attackPhyDamage -= ReduceDamage;
#endif

	pDamageInfo->ShieldDamage += ShieldDamage;

	double defencePhyLevel = m_ATTACKCALC.getPhyDefenceLevel(pTargetObject, pAttacker);
	attackPhyDamage *= (1.0 - defencePhyLevel);

	if (pAttacker->GetObjectKind() == eObjectKind_Player && pTargetObject->GetObjectKind() & eObjectKind_Monster)
	{
		attackPhyDamage *= gEventRate[eEvent_DamageRate];
	}
	if (pAttacker->GetObjectKind() & eObjectKind_Monster && pTargetObject->GetObjectKind() == eObjectKind_Player)
	{
		attackPhyDamage *= gEventRate[eEvent_DamageReciveRate];
	}

	if (attackPhyDamage < 1)
		attackPhyDamage = 1;

	float fdam = 0.0f;
	if (pAttacker->GetObjectKind() == eObjectKind_Player)
	{//Íâ¹¦ÉËº¦¼ÆËã
		fdam = (((CPlayer*)pAttacker)->GetShopItemStats()->WoigongDamage * 0.01f) +
			(((CPlayer*)pAttacker)->GetAvatarOption()->WoigongDamage * 0.01f) + (((CPlayer*)pAttacker)->GetSetItemQualityStats()->WoigongDamage * 0.01f);

		attackPhyDamage *= (fdam + 1.0f);//PVP  ±¶ÂÊÔÚÇ°ÃæÈ«¾ÖÒÑÉèÖÃ

		//SW060719 ¹®ÆÄÆ÷ÀÎÆ®
		if (((CPlayer*)pAttacker)->GetGuildIdx())
			GUILDMGR->GetGuildPlustimeRt(((CPlayer*)pAttacker)->GetGuildIdx(), eGPT_DamageUp, &attackPhyDamage);

		////PET %°ø°Ý·Â========================================================
		((CPlayer*)pAttacker)->GetPetManager()->GetPetBuffResultRt(ePB_Demage_Percent, &attackPhyDamage);
		//»ñÈ¡Åå´÷ÎäÆ÷ÐÅÏ¢
		const ITEMBASE* pTargetItemBase = ITEMMGR->GetItemInfoAbsIn((CPlayer*)pAttacker, 81);
		ITEM_INFO* pItemInfo = ITEMMGR->GetItemInfo(pTargetItemBase->wIconIdx);
		// RaMa - 05.10.10 -> ÇØ		°ñ°¡¸é ¿É¼ÇÃß°¡
		if (pTargetObject->GetObjectKind() == eObjectKind_Monster)
		{//PVEÆÆ¼×
			//if( g_pServerSystem->GetMapNum() != BOSSMONSTER_MAP &&
			//	g_pServerSystem->GetMapNum() != BOSSMONSTER_2NDMAP )
			if (FALSE == g_pServerSystem->GetMap()->IsMapKind(eBossMap))
			{
				fdam = (((CPlayer*)pAttacker)->GetAvatarOption()->TargetPhyDefDown * 0.01f);
				//attackPhyDamage *= (fdam+1.0f);
			}//ÌØÊâÊôÐÔ
			if (pItemInfo->WeaponType == 5)//Èç¹û´©´÷µÄÊÇ¹­£¬Ôö¼Ó15%ÆÆ¼×
				fdam += 0.15f;
			//ÎÞÊÓ¶Ô·½15%·ÀÓù
			fdam += fdam * (((CPlayer*)pAttacker)->GetSetItemQualityStats()->TargetPhyDefDown * 0.01f);
			attackPhyDamage *= (fdam + 1.0f) * gEventRate[eEvent_MugongKillMonster];//PVEÉËº¦±¶ÂÊ

		}
		if (pTargetObject->GetObjectKind() == eObjectKind_Player)
		{//PVPÆÆ¼×//ÌØÊâÊôÐÔ
			if (pItemInfo->WeaponType == 5)//Èç¹û´©´÷µÄÊÇ¹­£¬Ôö¼Ó15%ÆÆ¼×£¨ÎÞÊÓ¶Ô·½15%·ÀÓù£©
				fdam = (((CPlayer*)pAttacker)->GetSetItemQualityStats()->PlayerPhyDefDown * 0.01f) + 0.15f;//PVPÆÆ¼×
			else
				fdam = (((CPlayer*)pAttacker)->GetSetItemQualityStats()->PlayerPhyDefDown * 0.01f);//PVPÆÆ¼×

			//attackPhyDamage *= (fdam+1.0f);
			attackPhyDamage *= (fdam + 1.0f);
		}
		// RaMa - 06.11.13 -> ¹«½Ö¸ðµåÀÏ¶§ °ø°Ý·ÂÁõ°¡ ¾Æ¹ÙÅ¸
		if (((CPlayer*)pAttacker)->IsMussangMode() && ((CPlayer*)pAttacker)->GetAvatarOption()->MussangDamage)
			attackPhyDamage *= (((CPlayer*)pAttacker)->GetAvatarOption()->MussangDamage * 0.01f + 1.0f);

		// magi82 - UniqueItem(070627)
		attackPhyDamage += (double)(((CPlayer*)pAttacker)->GetUniqueItemStats()->nPhyDamage);
		//ÌØÊâÊôÐÔ
		if (pItemInfo->WeaponType == 6)//¹¥»÷ÕßÅå´÷°µÆ÷Ê±£¬Ôö¼Ó5%¼¼ÄÜÉËº¦
		{
			if (pAttacker->GetLife() < (pAttacker->DoGetMaxLife()) * 0.5)
				attackPhyDamage = attackPhyDamage * 1.20;//ÑªÁ¿µÍÓÚ50%Ê±£¬15%¼¼ÄÜÉËº¦
			else
				attackPhyDamage = attackPhyDamage * 1.05;
		}
		if (attackPhyDamage < 0.f)
			attackPhyDamage = 1.f;
	}
	else if (pAttacker->GetObjectKind() & eObjectKind_Monster)//¹ÖÎï´òÈËµÄÉËº¦¼ÆËã
	{
		if (pTargetObject->GetObjectKind() == eObjectKind_Player)
		{
			if (FALSE == g_pServerSystem->GetMap()->IsMapKind(eBossMap))
			{
				fdam = ((CPlayer*)pTargetObject)->GetAvatarOption()->TargetAtkDown * 0.01f;
				attackPhyDamage *= (1.0f - fdam);
			}
		}
	}

	return (DWORD)attackPhyDamage;
}

//[ÊôÐÔ¹¥»÷ÉËº¦¼ÆËã][2017/12/7]
DWORD CAttackManager::GetMugongAttrDamage(CObject* pAttacker, CObject* pTargetObject,
	WORD Attrib, DWORD AttAttackMin, DWORD AttAttackMax, float AttAttackRate,
	float fCriticalRate, RESULTINFO* pDamageInfo, float fDecreaseDamageRate)
{
#ifdef _HK_LOCAL_
	pDamageInfo->bDecisive = FALSE;
#else
	pDamageInfo->bDecisive = m_ATTACKCALC.getDecisive(pAttacker, pTargetObject, fCriticalRate);
#endif

	double attackAttrDamage = m_ATTACKCALC.getAttributeAttackPower(pAttacker, Attrib, AttAttackMin, AttAttackMax, AttAttackRate);
	attackAttrDamage *= fDecreaseDamageRate;

	//   ÐÂÔö£ºPvP ÊôÐÔ¹¥»÷¼Ó³É£¨¹¥»÷Õß£©
	if (pAttacker->GetObjectKind() == eObjectKind_Player && pTargetObject->GetObjectKind() == eObjectKind_Player)
	{
		attackAttrDamage *= (1.0f + ((CPlayer*)pAttacker)->GetItemStats()->PVPAttack);
		attackAttrDamage *= (1.0f + ((CPlayer*)pAttacker)->GetAvatarOption()->PVPAttack);
	}

	if (pDamageInfo->bDecisive)//Îä¹¦ÊôÐÔ¹¥»÷±©»÷ÉËº¦
	{
		//attackAttrDamage *= 2.25f;
		attackAttrDamage *= gEventRate[eEvent_MugongAttrCritical];//·¨Ê¦±©»÷ÉËº¦±¶ÂÊÉèÖÃ 

		if (pAttacker->GetObjectKind() == eObjectKind_Player)
		{//ÌØÊâÊôÐÔ
			const ITEMBASE* pTargetItemBase = ITEMMGR->GetItemInfoAbsIn((CPlayer*)pAttacker, 81);
			ITEM_INFO* pItemInfo = ITEMMGR->GetItemInfo(pTargetItemBase->wIconIdx);
			if (pItemInfo->WeaponType == 3)//¹¥»÷Õß´©´÷µ¶Ê±£¬ÌáÉý15%·ÜÁ¦Ò»»÷ÉËº¦
				attackAttrDamage += (attackAttrDamage * 0.15f);//·¨Ê¦±©»÷ÉËº¦±¶ÂÊÉèÖÃ 

			attackAttrDamage += (attackAttrDamage * ((((CPlayer*)pAttacker)->GetSetItemQualityStats()->DecisiveDamage) * 0.01f));
		}
		if ((pAttacker->GetObjectKind() & eObjectKind_Monster) && pTargetObject->GetObjectKind() == eObjectKind_Player)
		{
			((CPlayer*)pTargetObject)->GetPetManager()->GetPetBuffResultRt(ePB_ReduceCriticalDmg, &attackAttrDamage);
		}

		if (pAttacker->GetObjectKind() == eObjectKind_Player)
		{
			attackAttrDamage += (double)(((CPlayer*)pAttacker)->GetUniqueItemStats()->nCriDamage);
			if (attackAttrDamage < 0.f)
				attackAttrDamage = 1.f;
		}
	}

#ifdef _JAPAN_LOCAL_
	DWORD ShieldDamage = pTargetObject->CalcShieldDamage((DWORD)(attackAttrDamage * SHIELD_IN_MUGONG_DAMAGE));
	attackAttrDamage -= ShieldDamage;
#else
	if (pAttacker->GetObjectKind() == eObjectKind_Player && pTargetObject->GetObjectKind() == eObjectKind_Player)
	{
		//attackAttrDamage *= 0.5f;
		attackAttrDamage *= gEventRate[eEvent_MugongAttr];//ÔÚ´Ë´¦ÅäÖÃbinÅäÖÃÊôÐÔÉËº¦//·¨Ê¦´òÈËÉËº¦¼ÆËã		
	}
	DWORD ShieldDamage = 0;
	DWORD ReduceDamage = pTargetObject->CalcShieldDamage((DWORD)(attackAttrDamage * SHIELD_IN_MUGONG_DAMAGE), ShieldDamage);
	attackAttrDamage -= ReduceDamage;
#endif

	pDamageInfo->ShieldDamage += ShieldDamage;

	float RegVal = pTargetObject->GetAttDefense(Attrib);

	//   ÐÂÔö£ºPvP ÊôÐÔ·ÀÓù¼Ó³É£¨·ÀÊØÕß£©
	if (pAttacker->GetObjectKind() == eObjectKind_Player &&
		pTargetObject->GetObjectKind() == eObjectKind_Player)
	{
		RegVal *= (1.0f + ((CPlayer*)pTargetObject)->GetItemStats()->PVPADef);
		RegVal *= (1.0f + ((CPlayer*)pTargetObject)->GetAvatarOption()->PVPADef);
	}

	if (pTargetObject->GetObjectKind() == eObjectKind_Player)
	{
		float val = 1 + ((CPlayer*)pTargetObject)->GetSkillStatsOption()->AttDef;
		if (val < 0.0f)
			val = 0.0f;
		RegVal = RegVal * val;
	}

	if (RegVal > 1) RegVal = 1;
	if (RegVal < 0) RegVal = 0;

	RegVal *= 0.7f;

	double resAttrDamage = (attackAttrDamage * (1 - RegVal));

	if (pAttacker->GetObjectKind() == eObjectKind_Player && pTargetObject->GetObjectKind() & eObjectKind_Monster)
	{
		resAttrDamage *= gEventRate[eEvent_DamageRate];
	}
	if (pAttacker->GetObjectKind() & eObjectKind_Monster && pTargetObject->GetObjectKind() == eObjectKind_Player)
	{
		resAttrDamage *= gEventRate[eEvent_DamageReciveRate];
	}

	float fdam = 0.0f;
	if (pAttacker->GetObjectKind() == eObjectKind_Player)
	{//ÄÚ¹¦ÉËº¦¼ÆËã
		fdam = (((CPlayer*)pAttacker)->GetShopItemStats()->NeagongDamage * 0.01f) +
			(((CPlayer*)pAttacker)->GetAvatarOption()->NeagongDamage * 0.01f) + (((CPlayer*)pAttacker)->GetSetItemQualityStats()->NaegongDamage * 0.01f);

		//resAttrDamage = (resAttrDamage*(fdam+1.0f));
		resAttrDamage *= (fdam + 1.0f);//PVP  ±¶ÂÊÔÚÇ°ÃæÈ«¾ÖÒÑÉèÖÃ

		//SW060719 ¹®ÆÄÆ÷ÀÎÆ®
		if (((CPlayer*)pAttacker)->GetGuildIdx())
			GUILDMGR->GetGuildPlustimeRt(((CPlayer*)pAttacker)->GetGuildIdx(), eGPT_DamageUp, &resAttrDamage);

		//PET %°ø°Ý·Â
		((CPlayer*)pAttacker)->GetPetManager()->GetPetBuffResultRt(ePB_Demage_Percent, &resAttrDamage);
		//»ñÈ¡Åå´÷ÎäÆ÷ÐÅÏ¢
		const ITEMBASE* pTargetItemBase = ITEMMGR->GetItemInfoAbsIn((CPlayer*)pAttacker, 81);
		ITEM_INFO* pItemInfo = ITEMMGR->GetItemInfo(pTargetItemBase->wIconIdx);

		// RaMa - 05.10.10 -> ÇØ°ñ°¡¸é ¿É¼ÇÃß°¡
		if (pTargetObject->GetObjectKind() == eObjectKind_Monster)
		{//PVEÆÆÄ§
			//if( g_pServerSystem->GetMapNum() == BOSSMONSTER_MAP ||
			//	g_pServerSystem->GetMapNum() == BOSSMONSTER_2NDMAP )	goto CalcEnd;
			//if( g_pServerSystem->GetMap()->IsMapKind(eBossMap) )
			//	goto CalcEnd;
			if (FALSE == g_pServerSystem->GetMap()->IsMapKind(eBossMap))
			{
				fdam = (((CPlayer*)pAttacker)->GetAvatarOption()->TargetAttrDefDown * 0.01f);
			}//ÌØÊâÊôÐÔ
			if (pItemInfo->WeaponType == 6)//¹¥»÷ÕßÅå´÷°µÆ÷Ê±£¬pveÆÆÄ§15%
				fdam += 0.15f;
			//ÎÞÊÓ¶Ô·½15%·ÀÓù
			fdam += fdam * (((CPlayer*)pAttacker)->GetSetItemQualityStats()->TargetPhyDefDown * 0.01f);
			//resAttrDamage = (resAttrDamage*(fdam+1.0f));
			resAttrDamage *= (fdam + 1.0f) * gEventRate[eEvent_MugongKillMonster];//PVEÉËº¦±¶ÂÊ

		}
		if (pTargetObject->GetObjectKind() == eObjectKind_Player)
		{//PVPÆÆÄ§//ÌØÊâÊôÐÔ
			if (pItemInfo->WeaponType == 6)//¹¥»÷ÕßÅå´÷°µÆ÷Ê±£¬pvpÆÆÄ§8%
				fdam = ((CPlayer*)pAttacker)->GetSetItemQualityStats()->PlayerAttrDefDown * 0.01f + 0.08f;
			else
				fdam = ((CPlayer*)pAttacker)->GetSetItemQualityStats()->PlayerAttrDefDown * 0.01f;

			resAttrDamage *= (fdam + 1.0f);
		}
		// RaMa - 06.11.13 -> ¹«½Ö¸ðµåÀÏ¶§ °ø°Ý·ÂÁõ°¡ ¾Æ¹ÙÅ¸
		if (((CPlayer*)pAttacker)->IsMussangMode() && ((CPlayer*)pAttacker)->GetAvatarOption()->MussangDamage)
			resAttrDamage *= (((CPlayer*)pAttacker)->GetAvatarOption()->MussangDamage * 0.01f + 1.0f);

		// magi82 - UniqueItem(070627)
		resAttrDamage *= (((CPlayer*)pAttacker)->GetUniqueItemStats()->nAttR * 0.01f + 1.0f);
		//ÌØÊâÊôÐÔ
		if (pItemInfo->WeaponType == 6)//¹¥»÷ÕßÅå´÷°µÆ÷Ê±£¬Ôö¼Ó5%¼¼ÄÜÉËº¦
		{
			if (pAttacker->GetLife() < (pAttacker->DoGetMaxLife()) * 0.5)
				resAttrDamage = resAttrDamage * 1.15f;//ÑªÁ¿µÍÓÚ50%Ê±£¬15%¼¼ÄÜÉËº¦
			else
				resAttrDamage = resAttrDamage * 1.05f;
		}
	}
	else if (pAttacker->GetObjectKind() & eObjectKind_Monster)
	{
		if (pTargetObject->GetObjectKind() == eObjectKind_Player)
		{
			if (g_pServerSystem->GetMap()->IsMapKind(eBossMap))
				goto CalcEnd;

			fdam = ((CPlayer*)pTargetObject)->GetAvatarOption()->TargetAtkDown * 0.01f;
			resAttrDamage *= (1.0f - fdam);
		}
	}

CalcEnd:
	return (DWORD)resAttrDamage;
}

/////¾ÉµÄ//////
//DWORD CAttackManager::GetMugongAttrDamage(CObject* pAttacker,CObject* pTargetObject,
//										  WORD Attrib,DWORD AttAttackMin,DWORD AttAttackMax,float AttAttackRate,
//										  float fCriticalRate,RESULTINFO* pDamageInfo,float fDecreaseDamageRate )
//{
//#ifdef _HK_LOCAL_	//hk block
//	pDamageInfo->bDecisive = FALSE;
//#else
//	pDamageInfo->bDecisive = m_ATTACKCALC.getDecisive(pAttacker,pTargetObject,fCriticalRate);
//#endif
//	double attackAttrDamage = m_ATTACKCALC.getAttributeAttackPower(pAttacker,Attrib,AttAttackMin,AttAttackMax,AttAttackRate);
//	//041213 KES
//	attackAttrDamage *= fDecreaseDamageRate;
//
//	if(pDamageInfo->bDecisive)
//	{
//		attackAttrDamage += attackAttrDamage*0.6f;//attackAttrDamage *= 2.25f;//KIV
//
//		//SW060906 ½Å±ÔÆê
//		if( (pAttacker->GetObjectKind() & eObjectKind_Monster) && pTargetObject->GetObjectKind() == eObjectKind_Player )
//		{
//			((CPlayer*)pTargetObject)->GetPetManager()->GetPetBuffResultRt(ePB_ReduceCriticalDmg, &attackAttrDamage);
//		}
//
//		// magi82 - UniqueItem(070627)
//		if( pAttacker->GetObjectKind() == eObjectKind_Player )
//		{
//			attackAttrDamage += (double)(((CPlayer*)pAttacker)->GetUniqueItemStats()->nCriDamage);
//			if(attackAttrDamage < 0.f)
//				attackAttrDamage = 1.f;
//		}
//	}
//	//
//	//PvP ÀÏ¶§´Â °ø°Ý·ÂÀÇ 50%¸¸
//#ifdef _JAPAN_LOCAL_	
//	//	int nRel = eCAR_None;
//	if( pAttacker->GetObjectKind() == eObjectKind_Player &&
//		pTargetObject->GetObjectKind() == eObjectKind_Player )
//	{
//		attackAttrDamage *= 0.25f;
//
//		//		if( Attrib >= 1 && Attrib <= 7 )
//		//			nRel = ((CPlayer*)pTargetObject)->WhatIsAttrRelation(Attrib);
//	}
//#else
//	if( pAttacker->GetObjectKind() == eObjectKind_Player &&
//		pTargetObject->GetObjectKind() == eObjectKind_Player )
//	{
//		attackAttrDamage *= 0.15f;//attackAttrDamage *= 0.5f;//KIV
//	}
//#endif
//
//#ifdef _JAPAN_LOCAL_
//	DWORD ShieldDamage = pTargetObject->CalcShieldDamage((DWORD)(attackAttrDamage * SHIELD_IN_MUGONG_DAMAGE));
//	attackAttrDamage -= ShieldDamage;
//#else
//	// 06. 03 ±¹³»¹«½Ö - ÀÌ¿µÁØ
//	DWORD ShieldDamage = 0;
//	DWORD ReduceDamage = pTargetObject->CalcShieldDamage((DWORD)(attackAttrDamage * SHIELD_IN_MUGONG_DAMAGE), ShieldDamage);
//	attackAttrDamage -= ReduceDamage;
//#endif
//
//	pDamageInfo->ShieldDamage += ShieldDamage;	
//
//
//	float RegVal = pTargetObject->GetAttDefense(Attrib);
//
//	//////////////////////////////////////////////////////////////////////////
//	// 06. 06. 2Â÷ ÀüÁ÷ - ÀÌ¿µÁØ
//	// ¹«°ø º¯È¯ Ãß°¡
//	// ¼Ó¼º¹æ¾î·Â
//	if(pTargetObject->GetObjectKind() == eObjectKind_Player)
//	{
//		float val = 1 + ((CPlayer*)pTargetObject)->GetSkillStatsOption()->AttDef;
//
//		if( val < 0 )
//			val = 0.0f;
//
//		RegVal = RegVal * val;
//	}
//	//////////////////////////////////////////////////////////////////////////
//
//	if( RegVal > 1 )		RegVal = 1;
//	if( RegVal < 0 )		RegVal = 0;
//
//	RegVal *= 0.7f;
//	
//	double resAttrDamage = (attackAttrDamage * (1 - RegVal) );
//
//	// RaMa - 04.11.24   ->µ¥¹ÌÁöºñÀ²
//	if(pAttacker->GetObjectKind() == eObjectKind_Player && pTargetObject->GetObjectKind() & eObjectKind_Monster)
//	{
//		resAttrDamage = (resAttrDamage*gEventRate[eEvent_DamageRate]);
//	}
//	if(pAttacker->GetObjectKind() & eObjectKind_Monster && pTargetObject->GetObjectKind() == eObjectKind_Player)
//	{
//		resAttrDamage = (resAttrDamage*gEventRate[eEvent_DamageReciveRate]);
//	}
//
//	float fdam = 0.0f;
//	// RaMa - 04.11.10    -> ShopItemOptionÃß°¡, AvatarItemOptionÃß°¡(05.08.16)
//	if(pAttacker->GetObjectKind() == eObjectKind_Player)
//	{
//		fdam = (((CPlayer*)pAttacker)->GetShopItemStats()->NeagongDamage*0.01f)+
//			(((CPlayer*)pAttacker)->GetAvatarOption()->NeagongDamage*0.01f);
//		resAttrDamage = (resAttrDamage*(fdam+1.0f));
//
//		//SW060719 ¹®ÆÄÆ÷ÀÎÆ®
//		if( ((CPlayer*)pAttacker)->GetGuildIdx() )
//			GUILDMGR->GetGuildPlustimeRt( ((CPlayer*)pAttacker)->GetGuildIdx(), eGPT_DamageUp, &resAttrDamage );
//
//		//PET %°ø°Ý·Â
//		((CPlayer*)pAttacker)->GetPetManager()->GetPetBuffResultRt(ePB_Demage_Percent, &resAttrDamage);
//
//		// RaMa - 05.10.10 -> ÇØ°ñ°¡¸é ¿É¼ÇÃß°¡
//		if( pTargetObject->GetObjectKind() & eObjectKind_Monster )
//		{
//			//if( g_pServerSystem->GetMapNum() == BOSSMONSTER_MAP ||
//			//	g_pServerSystem->GetMapNum() == BOSSMONSTER_2NDMAP )	goto CalcEnd;
//			if( g_pServerSystem->GetMap()->IsMapKind(eBossMap) )
//				goto CalcEnd;
//
//			fdam = ((CPlayer*)pAttacker)->GetAvatarOption()->TargetAttrDefDown*0.01f;
//			resAttrDamage = (resAttrDamage*(fdam+1.0f));
//		}
//
//		// RaMa - 06.11.13 -> ¹«½Ö¸ðµåÀÏ¶§ °ø°Ý·ÂÁõ°¡ ¾Æ¹ÙÅ¸
//		if( ((CPlayer*)pAttacker)->IsMussangMode() && ((CPlayer*)pAttacker)->GetAvatarOption()->MussangDamage )
//			resAttrDamage *= (((CPlayer*)pAttacker)->GetAvatarOption()->MussangDamage*0.01f+1.0f);
//
//		// magi82 - UniqueItem(070627)
//		resAttrDamage *= (((CPlayer*)pAttacker)->GetUniqueItemStats()->nAttR * 0.01f + 1.0f);
//	}	
//	else if( pAttacker->GetObjectKind() & eObjectKind_Monster )
//	{
//		// RaMa - 05.10.10 -> ÇØ°ñ°¡¸é ¿É¼ÇÃß°¡
//		if( pTargetObject->GetObjectKind() == eObjectKind_Player )
//		{
//			//if( g_pServerSystem->GetMapNum() == BOSSMONSTER_MAP ||
//			//	g_pServerSystem->GetMapNum() == BOSSMONSTER_2NDMAP )	goto CalcEnd;
//			if( g_pServerSystem->GetMap()->IsMapKind(eBossMap) )
//				goto CalcEnd;
//
//			fdam = ((CPlayer*)pTargetObject)->GetAvatarOption()->TargetAtkDown*0.01f;
//			resAttrDamage = (resAttrDamage*(1.0f-fdam));
//		}
//	}
//
//CalcEnd:	
//
//	return (DWORD)resAttrDamage;
//}
// 





//////ÒÑ¾­¼ÓÁËÉÁ±ÜpvpÃ»ÓÐÐÂ½¨
void CAttackManager::Attack(BOOL bMugong, CObject* pAttacker,CObject* pTarget,DWORD AmplifiedPower,
							float PhyAttackRate,
							WORD Attrib,DWORD AttAttackMin,DWORD AttAttackMax,float AttAttackRate,
							float fCriticalRate,
							RESULTINFO* pDamageInfo,BOOL bCounter, float fDecreaseDamageRate,
							WORD AmplifiedPowerAttrib, BOOL bContinueAttack )
{	
	pDamageInfo->Clear();

	// »ñÈ¡Ä¿±êÉÁ±ÜÂÊ
	float fDodgeRate = pTarget->GetDodgeRate();
	// ÈôÄ¿±êÎªÍæ¼Ò£¬´¦Àí×°±¸Ó°Ïì
	if (pTarget->GetObjectKind() == eObjectKind_Player)
	{//ÌØÊâÊôÐÔ
		// ÅÐ¶ÏÊÇ·ñÅå´÷¡°Ç¹¡±£¨WeaponType == 4£©£¬¼Ó³É +10%
		const ITEMBASE* pTargetItemBase = ITEMMGR->GetItemInfoAbsIn((CPlayer*)pTarget, 81);
		ITEM_INFO* pItemInfo = ITEMMGR->GetItemInfo(pTargetItemBase->wIconIdx);
		if (pItemInfo && pItemInfo->WeaponType == 4)
			fDodgeRate += fDodgeRate * 0.1f;

		// Ì××°Æ·ÖÊÉÁ±Ü¼Ó³É
		fDodgeRate += fDodgeRate * (((CPlayer*)pTarget)->GetSetItemQualityStats()->wDodgeRate * 0.01f);
	}
	//  PvP ÉÁ±ÜÐÞÕýÂß¼­£¨¹¥»÷ÕßÃüÖÐ & Ä¿±êÉÁ±Ü£©
	if (pAttacker->GetObjectKind() == eObjectKind_Player && pTarget->GetObjectKind() == eObjectKind_Player)
	{
		// ¹¥»÷ÕßÃüÖÐ¼Ó³É£¨¼õÉÙÄ¿±êÉÁ±Ü£©
		fDodgeRate -= ((CPlayer*)pAttacker)->GetItemStats()->PVPHit;
		fDodgeRate -= ((CPlayer*)pAttacker)->GetAvatarOption()->PVPHit;

		// Ä¿±êÉÁ±Ü¼Ó³É£¨Ôö¼ÓÉÁ±Ü£©
		fDodgeRate += ((CPlayer*)pTarget)->GetItemStats()->PVPADodge;
		fDodgeRate += ((CPlayer*)pTarget)->GetAvatarOption()->PVPADodge;
	}
	if (pAttacker->GetObjectKind() == eObjectKind_Player)
	{//ÌØÊâÊôÐÔ
		const ITEMBASE* pTargetItemBase = ITEMMGR->GetItemInfoAbsIn((CPlayer*)pAttacker, 81);
		ITEM_INFO* pItemInfo = ITEMMGR->GetItemInfo(pTargetItemBase->wIconIdx);
		if (pItemInfo->WeaponType == 1)//¹¥»÷ÕßÅå´÷½£Ê±£¬ÃüÖÐÔö¼Ó10
			fDodgeRate = fDodgeRate - 10;

		fDodgeRate -= (fDodgeRate * (((CPlayer*)pAttacker)->GetSetItemQualityStats()->fDodgeRate) * 0.01f);//×°±¸ÃüÖÐ¼Ó³É
	}
	if(fDodgeRate != 0)
	{
		if(CheckRandom(fDodgeRate,pTarget->GetLevel(),pAttacker->GetLevel()) == TRUE)
		{
			pDamageInfo->bDodge = TRUE;
			pDamageInfo->RealDamage = 0;
			pDamageInfo->ShieldDamage = 0;
			return;
		}
	}

	if(pTarget->GetObjectKind() == eObjectKind_Player)
	{
		//Æê ¹öÇÁ È¸ÇÇ
		BOOL rtDodge = FALSE;
		((CPlayer*)pTarget)->GetPetManager()->GetPetBuffResultRt(ePB_Dodge, &rtDodge);
		if( TRUE == rtDodge )
		{
			pDamageInfo->bDodge = TRUE;
			pDamageInfo->RealDamage = 0;
			pDamageInfo->ShieldDamage = 0;
			return;
		}

		//SW060911 È«Äá¿äÃ» ¼öÁ¤.
		if( ((CPlayer*)pTarget)->GetUserLevel() <= eUSERLEVEL_GM && ((CPlayer*)pTarget)->IsVisible() == FALSE )
		{
			return;
		}
	}

	// 2005 Å©¸®½º¸¶½º ÀÌº¥Æ®
	if(( pAttacker->GetObjectKind() == eObjectKind_Player ) && ( pTarget->GetObjectKind() & eObjectKind_Monster ))
	{	// ÇÃ·¹ÀÌ¾î°¡ ¸ó½ºÅÍ °ø°Ý½Ã
		CPlayer* pPlayer = (CPlayer*)pAttacker;
		CMonster* pMonster = (CMonster*)pTarget;

		// ¸¸¾à ÀÌº¥Æ® ¹«±â ÀåÂøÁßÀÌ¶ó¸é
		if( pPlayer->GetWeaponEquipType() == WP_EVENT || pPlayer->GetWeaponEquipType() == WP_EVENT_HAMMER )
		{
			// ¹«°øÀ» »ç¿ëÇÑ°ÍÀÌ¸é ¸®ÅÏÇÑ´Ù.
			if(bMugong)
				return;

			// Áö¼Ó °ø°Ýµµ ¸®ÅÏ
			if(bContinueAttack)
				return;
		}
		else if( pMonster->GetMonsterKind() == EVENT_MONSTER_SANTA1 || pMonster->GetMonsterKind() == EVENT_MONSTER_SANTA2
			|| pMonster->GetMonsterKind() == EVENT_FIELDBOSS_SNOWMAN_SM || pMonster->GetMonsterKind() == EVENT_FIELDSUB_SNOWMAN_SM 
			|| pMonster->GetMonsterKind() == EVENT_FIELDBOSS_SNOWMAN_MD || pMonster->GetMonsterKind() == EVENT_FIELDSUB_SNOWMAN_MD 
			|| pMonster->GetMonsterKind() == EVENT_FIELDBOSS_SNOWMAN_LG || pMonster->GetMonsterKind() == EVENT_FIELDSUB_SNOWMAN_LG 
			/*|| pMonster->GetMonsterKind() == EVENT_SNOWMAN_SM
			|| pMonster->GetMonsterKind() == EVENT_SNOWMAN_MD
			|| pMonster->GetMonsterKind() == EVENT_SNOWMAN_LG*/
			)
			return;
	}

	if(pTarget->GetInited() == FALSE)
		return;

	DWORD AttrDamage =0,PhyDamage =0;

	if(pTarget->GetState() == eObjectState_Die || pTarget->GetState() == eObjectState_Immortal)
	{
		return;
	}

	if( pAttacker->GetGridID() != pTarget->GetGridID() )
	{
		//		ASSERT(0);
		return;
	}
	/*
	#ifdef _JAPAN_LOCAL_

	if( pAttacker->GetObjectKind() == eObjectKind_Player )
	{
	CPlayer* pAttackPlayer = (CPlayer*)pAttacker;

	if( pAttackPlayer->GetPartyIdx() )
	{
	CParty* pParty = PARTYMGR->GetParty( pAttackPlayer->GetPartyIdx() );

	if( pParty )
	if( pParty->IsHelpPartyMember( pAttackPlayer->GetID() ) )
	{
	pAttackPlayer->SetPartyHelp( TRUE );	//ÆÄÆ¼ »ó»ý È¿°ú 
	}
	}
	}
	#endif
	*/
	//³ÖÐøÉËº¦¼Ó³É¼ÆËã
	if (pAttacker->GetObjectKind() == eObjectKind_Player)
	{
		AttAttackMin += AttAttackMin * (((CPlayer*)pAttacker)->GetSetItemQualityStats()->ContinueAttAttack) * 0.01f ;
		AttAttackMax += AttAttackMax * (((CPlayer*)pAttacker)->GetSetItemQualityStats()->ContinueAttAttack) * 0.01f ;
	}
	if(PhyAttackRate > 0.000001f)
	{//Íâ¹¦ÉËº¦
		if(bMugong)
		{
			PhyDamage = GetMugongPhyDamage(pAttacker,pTarget,PhyAttackRate,fCriticalRate,pDamageInfo,AmplifiedPower,fDecreaseDamageRate );
		}
		else
		{
			PhyDamage = GetComboPhyDamage(pAttacker,pTarget,PhyAttackRate,fCriticalRate,pDamageInfo,AmplifiedPower,fDecreaseDamageRate );
		}
	}

	if(AttAttackMax != 0)
	{
		// ÄÚ¹¦ÉËº¦
		AttrDamage = GetMugongAttrDamage(pAttacker,pTarget,Attrib,AttAttackMin,AttAttackMax,AttAttackRate,fCriticalRate,pDamageInfo,fDecreaseDamageRate );
	}

	if (pTarget->GetObjectKind() == eObjectKind_Player)
	{
		((CPlayer*)pTarget)->GetPetManager()->GetPetBuffResultRt(ePB_ReduceDemageRate, &PhyDamage);
		((CPlayer*)pTarget)->GetPetManager()->GetPetBuffResultRt(ePB_ReduceDemageRate, &AttrDamage);
	}

	//×°±¸´ÊÌõ×îÖÕÉËº¦¼Ó³É
	if (pAttacker->GetObjectKind() == eObjectKind_Player)
	{
		if (PhyAttackRate > 0.000001f)
		{
			if (pTarget->GetObjectKind() == eObjectKind_Player)
			{//PVP
				PhyDamage += (PhyDamage * (((CPlayer*)pAttacker)->GetSetItemQualityStats()->AttPlayerDamage) * 0.01f);
			}
			if (pTarget->GetObjectKind() == eObjectKind_Monster)
			{//PVE
				PhyDamage += (PhyDamage * (((CPlayer*)pAttacker)->GetSetItemQualityStats()->AttMonsterDamage) * 0.01f);
			}
		}
		if (AttAttackMax != 0)
		{
			if (pTarget->GetObjectKind() == eObjectKind_Player)
			{//PVP
				AttrDamage += (AttrDamage * (((CPlayer*)pAttacker)->GetSetItemQualityStats()->AttPlayerDamage) * 0.01f);
			}
			if (pTarget->GetObjectKind() == eObjectKind_Monster)
			{//PVE
				AttrDamage += (AttrDamage * (((CPlayer*)pAttacker)->GetSetItemQualityStats()->AttMonsterDamage) * 0.01f);
			}
		}
	}
	float fdam = 0.0f;
	//×°±¸´ÊÌõ×îÖÕÉËº¦¼õÉÙ
	if (pTarget->GetObjectKind() == eObjectKind_Player)
	{
		fdam = ((CPlayer*)pTarget)->GetSetItemQualityStats()->RealDamageDown * 0.01f;
		if (PhyAttackRate > 0.000001f)
			PhyDamage *= (1.0f - fdam);
		if (AttAttackMax != 0)
			AttrDamage *= (1.0f - fdam);
	}
	//¼¸ÂÊÎüÑª
	if (pAttacker->GetObjectKind() == eObjectKind_Player)
	{
		WORD value = rand() % 100 + 1;
		if (value < (((CPlayer*)pAttacker)->GetSetItemQualityStats()->PVPLifePlus))
		{
			DWORD NowLife = ((CPlayer*)pAttacker)->GetLife();
			if (PhyAttackRate > 0.000001f)
				((CPlayer*)pAttacker)->SetLife(NowLife + (PhyDamage * 0.5f));
			if (AttAttackMax != 0)
				((CPlayer*)pAttacker)->SetLife(NowLife + (AttrDamage * 0.5f));
		}
	}
	pAttacker->CalcRealAttack(pTarget, PhyDamage, AttrDamage, pDamageInfo, bContinueAttack );

	//== ÀÌ»óÇÏ°Ô µÇ¾úµû T_T ÀÏ´Ü ÀÓ½Ã·Î ±×³É °©½Ã´Ù!
	DWORD UpAttackDamage = 0;
	if( PhyDamage + AttrDamage < pDamageInfo->RealDamage )
		UpAttackDamage = ( pDamageInfo->RealDamage - ( PhyDamage + AttrDamage ) ) / 2;
	//===

	//SW070811 ±âÈ¹º¯°æ pvp ÀÏ ¶§ 10% µ¥¹ÌÁö.	//attackPhyDamage *= 0.1f;	//attackAttrDamage *= 0.1f;
	// °ø¼ºÀü½Ã¿¡´Â µ¥¹ÌÁö Àû¿ë
	/*float fsiegedamage = 0.2f;
	#ifdef _HK_LOCAL_
	fsiegedamage = 0.5f;
	#endif*/
	float fsiegedamage = 1.f;
//#ifdef _HK_LOCAL_
#ifdef _KOR_LOCAL_
	fsiegedamage = 0.5f;
#endif

	if( g_pServerSystem->GetMapNum() == SIEGEWARMGR->GetSiegeMapNum() )
	{
		if( pAttacker->GetObjectKind() == eObjectKind_Player && pTarget->GetObjectKind() == eObjectKind_Player )
		{
			// °è»êÇØ¼­ 0ÀÌ ³ª¿Ã¶§¸¦ À§ÇØ¼­ +1
			if( PhyDamage )
				PhyDamage = (DWORD)(PhyDamage*fsiegedamage + 1);
			if( AttrDamage )
				AttrDamage = (DWORD)(AttrDamage*fsiegedamage + 1);
			if( UpAttackDamage )

				UpAttackDamage = (DWORD)(UpAttackDamage*fsiegedamage + 1);
			if( pDamageInfo->RealDamage )
				pDamageInfo->RealDamage = (DWORD)(pDamageInfo->RealDamage*fsiegedamage + 1);
			if( pDamageInfo->ShieldDamage )
				pDamageInfo->ShieldDamage = (DWORD)(pDamageInfo->ShieldDamage*fsiegedamage + 1);
		}
	}

	pTarget->CalcRealDamage(pAttacker,PhyDamage+UpAttackDamage,AttrDamage+UpAttackDamage,pDamageInfo);

	// RaMa - µ¥¹ÌÁö ÀÌ»ó¼öÄ¡ È®ÀÎ¿ë ÄÚµå
	if( (pAttacker->GetObjectKind() & eObjectKind_Monster) && pDamageInfo->RealDamage > 1000000 )
	{
		char buf[256] = { 0, };
		sprintf( buf, "[DAMAGE] Name: %s, bCritical: %d, Damage: %d, ShieldDamage : %d, PhyDamage: %d, AttrDamage : %d, UpAttackDamage : %d ", 
			pAttacker->GetObjectName(), pDamageInfo->bCritical, pDamageInfo->RealDamage, pDamageInfo->ShieldDamage, 
			PhyDamage, AttrDamage, UpAttackDamage );
		ASSERTMSG( 0, buf );
		return;
	}

	if( pAttacker->GetObjectKind() == eObjectKind_Player )
	{
		CPlayer* pAttackPlayer = (CPlayer*)pAttacker;
		/*
		#ifdef _JAPAN_LOCAL_
		pAttackPlayer->SetPartyHelp( FALSE );	//°è»ê ³¡³µ´Ù. ÃÊ±âÈ­ÇÏÀÚ.
		#endif
		*/
#ifdef _JAPAN_LOCAL_

		if( pAttackPlayer->GetPartyIdx() )
		{
			CParty* pParty = PARTYMGR->GetParty( pAttackPlayer->GetPartyIdx() );

			if( pParty )
				if( pParty->IsHelpPartyMember( pAttackPlayer->GetID() ) )
				{
					pDamageInfo->RealDamage = pDamageInfo->RealDamage + pDamageInfo->RealDamage / 2;
				}
		}

#endif

		if( pTarget->GetObjectKind() == eObjectKind_Player )
		{
			CPlayer* pTargetPlayer = (CPlayer*)pTarget;

			//PK¸ðµå½Ã¿¡ PK½Ã°£ °»½Å
			if( pAttackPlayer->IsPKMode() /*&& pAttackPlayer->IsVimuing() == FALSE*/ ) //ºñ¹«ÀÏ¸®°¡ ¾ø´Ù.
			{
				if( AttrDamage || PhyDamage )
					if( pTargetPlayer->IsPKMode() == FALSE )
						pAttackPlayer->SetPKStartTimeReset();
			}

			// 06.09.25 RaMa Ãß¼®ÀÌº¥Æ®
			if( pAttackPlayer->GetWeaponEquipType() == WP_EVENT_HAMMER )
				pTargetPlayer->IncreaseEventHammerCount();
			//#ifndef _HK_LOCAL_	//hk block christmas
			if( pAttackPlayer->GetWeaponEquipType() == WP_EVENT )
			{
				pTargetPlayer->SummonEventPetRndm();
			}
			//#endif
		}
	}

	if(bCounter)
		pTarget->CalcCounterDamage(Attrib,AttrDamage,PhyDamage,pDamageInfo);
	else
		pDamageInfo->CounterDamage = 0;

	//-----¹Þ´Â µ¥¹ÌÁö¿¡ µû¸¥ Ã¼·Â ³»·Â Èí¼ö


	pTarget->CalcReverseVampiric(pDamageInfo);	//ÀÌÆåÆ® Ã³¸®´Â ¾ÈÇÑ´Ù --;
	//-----


	//	BOOL bVampiric = TRUE;
	//	if(bVampiric)
	//	{
	pAttacker->CalcVampiric(pDamageInfo);
	//	}

	// 06. 03 ±¹³»¹«½Ö - ÀÌ¿µÁØ
#ifndef _JAPAN_LOCAL_
	if( pAttacker->GetObjectKind() == eObjectKind_Player )
	{
		if(((CPlayer*)pAttacker)->IsMussangMode())
			pDamageInfo->RealDamage = (DWORD)(pDamageInfo->RealDamage * 1.05);
	}
#endif
	if (pTarget->GetObjectKind() == eObjectKind_Player)
	{//ÌØÊâÊôÐÔ
		const ITEMBASE* pTargetItemBase = ITEMMGR->GetItemInfoAbsIn((CPlayer*)pTarget, 81);
		ITEM_INFO* pItemInfo = ITEMMGR->GetItemInfo(pTargetItemBase->wIconIdx);
		if (pItemInfo->WeaponType == 4)//±»¹¥»÷ÕßÅå´÷Ç¹Ê±£¬×îÖÕÉËº¦¼õÉÙ5%
			pDamageInfo->RealDamage = (DWORD)(pDamageInfo->RealDamage * 0.95);
	}
	// magi82 cheat damage //////////////////////////////////////////////////////////////////////////
	float fRate = m_nDamageRate * 0.01f;

	if(pDamageInfo->RealDamage > 0 && ((pDamageInfo->RealDamage * fRate)*10) <= 10)
		pDamageInfo->RealDamage = 1;
	else
		pDamageInfo->RealDamage *= (DWORD)fRate;
	//////////////////////////////////////////////////////////////////////////

	DWORD dwRealDemage = pDamageInfo->RealDamage;

	pDamageInfo->RealDamage = GetPenaltyDemege(pAttacker, pTarget, dwRealDemage);

	if (pAttacker->GetObjectKind() == eObjectKind_Player && pTarget->GetObjectKind() == eObjectKind_Player)
		pDamageInfo->RealDamage *= gEventRate[eEvent_DamagePlayerToPlayerRate];

	if( pAttacker->GetObjectKind() == eObjectKind_Player )
	{
		CPlayer* pPlayer = (CPlayer*)pAttacker;
		if( pPlayer->InTitan() )
		{
			pPlayer->GetTitanManager()->DoRandomDecrease(AtAtk);
		}
	}

	DWORD newLife = pTarget->Damage(pAttacker,pDamageInfo);

	if(pDamageInfo->CounterDamage != 0)
	{
		RESULTINFO counterdmginfo;
		PhyDamage = 0;
		AttrDamage = pDamageInfo->CounterDamage;
		counterdmginfo.CounterDamage = 0;
		pAttacker->CalcRealDamage(pAttacker,PhyDamage,AttrDamage,&counterdmginfo);
		DWORD attackerlife = pAttacker->Damage(pTarget,&counterdmginfo);
		if(attackerlife == 0)
		{
			ATTACKMGR->sendDieMsg(pTarget,pAttacker);
			pAttacker->Die(pTarget);
		}
	}

	if(newLife == 0)
	{
		ATTACKMGR->sendDieMsg(pAttacker,pTarget);
		pTarget->Die(pAttacker);
	}
}

void CAttackManager::RecoverLife(CObject* pOperator,CObject* pObject,WORD m_Attrib,DWORD RecoverLifeVal,RESULTINFO* pResultInfo)
{
	DWORD realAddVal;
#ifdef _JAPAN_LOCAL_
	pObject->AddLife(RecoverLifeVal,&realAddVal,FALSE);
#else
	float fPlus = pOperator->GetAttribPlusPercent(m_Attrib);
	DWORD val = (DWORD)(RecoverLifeVal * (1 + fPlus));
	pObject->AddLife(val,&realAddVal,FALSE);
#endif
	pResultInfo->HealLife = (WORD)realAddVal;
}
void CAttackManager::RecoverNaeRyuk(CObject* pOperator,CObject* pObject,WORD m_Attrib,DWORD RecoverNaeRyukVal,RESULTINFO* pResultInfo)
{
	DWORD realAddVal;
#ifdef _JAPAN_LOCAL_
	pObject->AddNaeRyuk(RecoverNaeRyukVal,&realAddVal);
#else
	float fPlus = pOperator->GetAttribPlusPercent(m_Attrib);
	DWORD val = (DWORD)(RecoverNaeRyukVal * (1 + fPlus));
	pObject->AddNaeRyuk(val,&realAddVal);
#endif
	pResultInfo->RechargeNaeryuk = (WORD)realAddVal;
}


void CAttackManager::AttackAbs(CObject* pAttacker, CObject* pTarget, int nAbsKind, float AttackRate, RESULTINFO* pDamageInfo)
{
	if (nAbsKind == 0)
		return;

	pDamageInfo->Clear();

	// »ñÈ¡Ä¿±êÉÁ±ÜÂÊ
	float fDodgeRate = pTarget->GetDodgeRate();

	//  PvP ÉÁ±ÜÐÞÕý£¨¹¥»÷ÕßÃüÖÐ & Ä¿±êÉÁ±Ü£©
	if (pAttacker->GetObjectKind() == eObjectKind_Player &&
		pTarget->GetObjectKind() == eObjectKind_Player)
	{
		// ÃüÖÐ·½¼õÉÁ±Ü
		fDodgeRate -= ((CPlayer*)pAttacker)->GetItemStats()->PVPHit;
		fDodgeRate -= ((CPlayer*)pAttacker)->GetAvatarOption()->PVPHit;

		// ±»»÷·½¼ÓÉÁ±Ü
		fDodgeRate += ((CPlayer*)pTarget)->GetItemStats()->PVPADodge;
		fDodgeRate += ((CPlayer*)pTarget)->GetAvatarOption()->PVPADodge;

		//  ¿ÉÑ¡£ºÏÞÖÆÉÁ±ÜÂÊ²»³¬¹ý·¶Î§£¨±ÜÃâ³¬³ö¸ÅÂÊ£©//ÏÞÖÆ 100%
		if (fDodgeRate < 0.0f) fDodgeRate = 0.0f;
		if (fDodgeRate > 1.0f) fDodgeRate = 1.0f;
	}

	// ÅÐ¶¨ÉÁ±Ü
	if (fDodgeRate != 0)
	{
		if (CheckRandom(fDodgeRate, pTarget->GetLevel(), pAttacker->GetLevel()) == TRUE)
		{
			pDamageInfo->bDodge = TRUE;
			pDamageInfo->RealDamage = 0;
			pDamageInfo->ShieldDamage = 0;
			return;
		}
	}

	// ³èÎïÉÁ±Ü
	if (pTarget->GetObjectKind() == eObjectKind_Player)
	{
		BOOL rtDodge = FALSE;
		((CPlayer*)pTarget)->GetPetManager()->GetPetBuffResultRt(ePB_Dodge, &rtDodge);
		if (TRUE == rtDodge)
		{
			pDamageInfo->bDodge = TRUE;
			pDamageInfo->RealDamage = 0;
			pDamageInfo->ShieldDamage = 0;
			return;
		}
	}

	// »ù´¡×´Ì¬ÑéÖ¤
	if (!pTarget->GetInited() ||
		pTarget->GetState() == eObjectState_Die ||
		pTarget->GetState() == eObjectState_Immortal ||
		pAttacker->GetGridID() != pTarget->GetGridID())
	{
		return;
	}

	// PK Ä£Ê½ÅÐ¶Ï
	if (pAttacker->GetObjectKind() == eObjectKind_Player &&
		((CPlayer*)pAttacker)->IsPKMode() &&
		pTarget->GetObjectKind() == eObjectKind_Player &&
		AttackRate > 0.0f &&
		!((CPlayer*)pTarget)->IsPKMode())
	{
		((CPlayer*)pAttacker)->SetPKStartTimeReset();
	}

	// ¼ÆËãÉËº¦
	DWORD TargetLife = pTarget->GetLife();
	DWORD TargetShield = pTarget->GetShield();
	DWORD MinusLife = 0;
	DWORD MinusShield = 0;

	if (nAbsKind & eAAK_LIFE)
	{
		MinusLife = (DWORD)(TargetLife * AttackRate);
	}
	else if (nAbsKind & eAAK_SHIELD)
	{
		MinusShield = (DWORD)(TargetShield * AttackRate);
	}

	pDamageInfo->RealDamage = MinusLife;
	pDamageInfo->ShieldDamage = MinusShield;

	DWORD newLife = pTarget->Damage(pAttacker, pDamageInfo);

	if (newLife == 0)
	{
		ATTACKMGR->sendDieMsg(pAttacker, pTarget);
		pTarget->Die(pAttacker);
	}
}




void CAttackManager::AttackJinbub(CObject* pAttacker, CObject* pTarget, DWORD AttackPower,
	DWORD AttackMin, DWORD AttackMax, RESULTINFO* pDamageInfo, float fDecreaseDamageRate)
{
	pDamageInfo->Clear();

	float fDodgeRate = pTarget->GetDodgeRate();

	//  PvP ÃüÖÐ / ÉÁ±Ü¼Ó³Éµ÷Õû
	if (pAttacker->GetObjectKind() == eObjectKind_Player &&
		pTarget->GetObjectKind() == eObjectKind_Player)
	{
		fDodgeRate -= ((CPlayer*)pAttacker)->GetItemStats()->PVPHit;
		fDodgeRate -= ((CPlayer*)pAttacker)->GetAvatarOption()->PVPHit;

		fDodgeRate += ((CPlayer*)pTarget)->GetItemStats()->PVPADodge;
		fDodgeRate += ((CPlayer*)pTarget)->GetAvatarOption()->PVPADodge;

		// ÏÞÖÆÉÁ±ÜÂÊ·¶Î§ÔÚ 0.0f ~ 1.0f
		if (fDodgeRate < 0.0f) fDodgeRate = 0.0f;
		if (fDodgeRate > 1.0f) fDodgeRate = 1.0f;
	}

	if (fDodgeRate != 0)
	{
		if (CheckRandom(fDodgeRate, pTarget->GetLevel(), pAttacker->GetLevel()) == TRUE)
		{
			pDamageInfo->bDodge = TRUE;
			pDamageInfo->RealDamage = 0;
			pDamageInfo->ShieldDamage = 0;
			return;
		}
	}

	if (pTarget->GetObjectKind() == eObjectKind_Player)
	{
		BOOL rtDodge = FALSE;
		((CPlayer*)pTarget)->GetPetManager()->GetPetBuffResultRt(ePB_Dodge, &rtDodge);
		if (TRUE == rtDodge)
		{
			pDamageInfo->bDodge = TRUE;
			pDamageInfo->RealDamage = 0;
			pDamageInfo->ShieldDamage = 0;
			return;
		}
	}

	if (!pTarget->GetInited()) return;
	if (pTarget->GetState() == eObjectState_Die || pTarget->GetState() == eObjectState_Immortal) return;
	if (pAttacker->GetGridID() != pTarget->GetGridID()) return;
	if (AttackPower == 0) return;

	if (pAttacker->GetObjectKind() == eObjectKind_Player)
	{
		if (((CPlayer*)pAttacker)->IsPKMode())
		{
			if (pTarget && pTarget->GetObjectKind() == eObjectKind_Player &&
				!((CPlayer*)pTarget)->IsPKMode())
			{
				((CPlayer*)pAttacker)->SetPKStartTimeReset();
			}
		}
	}

	if (AttackMax < AttackMin)
		AttackMin = AttackMax;

	DWORD PlusAttack = random(AttackMin, AttackMax);
	DWORD RealAttack = GetJinbubDamage(pAttacker, pTarget, AttackPower + PlusAttack, pDamageInfo, fDecreaseDamageRate);

	float fsiegedamage = 0.5f;
	if (g_pServerSystem->GetMapNum() == SIEGEWARMGR->GetSiegeMapNum())
	{
		if (pAttacker->GetObjectKind() == eObjectKind_Player &&
			pTarget->GetObjectKind() == eObjectKind_Player)
		{
			if (RealAttack)
				RealAttack = (DWORD)(RealAttack * fsiegedamage + 1);
			if (pDamageInfo->RealDamage)
				pDamageInfo->RealDamage = (DWORD)(pDamageInfo->RealDamage * fsiegedamage + 1);
			if (pDamageInfo->ShieldDamage)
				pDamageInfo->ShieldDamage = (DWORD)(pDamageInfo->ShieldDamage * fsiegedamage + 1);
		}
	}

	pTarget->CalcRealDamage(pAttacker, (WORD)RealAttack, 0, pDamageInfo);
	pTarget->CalcReverseVampiric(pDamageInfo);

	DWORD newLife = pTarget->Damage(pAttacker, pDamageInfo);

	if (newLife == 0)
	{
		ATTACKMGR->sendDieMsg(pAttacker, pTarget);
		pTarget->Die(pAttacker);
	}
}


DWORD CAttackManager::GetJinbubDamage(CObject* pAttacker,CObject* pTargetObject,DWORD AttackPower,
									  RESULTINFO* pDamageInfo,float fDecreaseDamageRate)
{
	//041213 KES
	DWORD attackPhyDamage = (DWORD)(AttackPower * fDecreaseDamageRate);
	//

	//PvP ÀÏ¶§´Â °ø°Ý·ÂÀÇ 50%¸¸
	if( pAttacker->GetObjectKind() == eObjectKind_Player &&
		pTargetObject->GetObjectKind() == eObjectKind_Player )
#ifdef _JAPAN_LOCAL_
		attackPhyDamage = (DWORD)(attackPhyDamage*0.25f);
#else
		attackPhyDamage = (DWORD)(attackPhyDamage*0.5f);
#endif

#ifdef _JAPAN_LOCAL_
	DWORD ShieldDamage = pTargetObject->CalcShieldDamage((DWORD)(attackPhyDamage * SHIELD_OUT_MUGONG_DAMAGE));
	attackPhyDamage -= ShieldDamage;
#else
	// 06. 03 ±¹³»¹«½Ö - ÀÌ¿µÁØ
	DWORD ShieldDamage = 0;
	DWORD ReduceDamage = pTargetObject->CalcShieldDamage((DWORD)(attackPhyDamage * SHIELD_OUT_MUGONG_DAMAGE), ShieldDamage);
	attackPhyDamage -= ReduceDamage;
#endif

	pDamageInfo->ShieldDamage += (DWORD)ShieldDamage;

	double defencePhyLevel = m_ATTACKCALC.getPhyDefenceLevel(pTargetObject,pAttacker);
	attackPhyDamage = (DWORD)(attackPhyDamage*(1-defencePhyLevel));

	// RaMa - 04.11.24   ->µ¥¹ÌÁöºñÀ²
	if(pAttacker->GetObjectKind() == eObjectKind_Player && pTargetObject->GetObjectKind() & eObjectKind_Monster)
	{
		// attackPhyDamage *= gDamageRate;
		attackPhyDamage = (DWORD)( attackPhyDamage*gEventRate[eEvent_DamageRate]);
	}
	if(pAttacker->GetObjectKind() & eObjectKind_Monster && pTargetObject->GetObjectKind() == eObjectKind_Player)
	{
		// attackPhyDamage *= gDamageReciveRate;
		attackPhyDamage = (DWORD)(attackPhyDamage*gEventRate[eEvent_DamageReciveRate]);
	}

	if(attackPhyDamage < 1)
		attackPhyDamage = 1;

	return attackPhyDamage;
}

//2007. 10. 30. CBH - µ¥¹ÌÁö ÆÐ³ÎÆ¼ °è»ê ÇÔ¼ö
DWORD CAttackManager::GetPenaltyDemege(CObject* pAttacker,CObject* pTargetObject, DWORD dwDemage)
{
	DWORD dwResultDemege = dwDemage;

	BYTE attackerKind = pAttacker->GetObjectKind();
	BYTE targetKind = pTargetObject->GetObjectKind();

	if(targetKind & eObjectKind_Monster)
	{
		if(attackerKind == eObjectKind_Player)
		{
			CPlayer* pPlayer = (CPlayer*)pAttacker;
			//Å¸ÀÌÅº Å¾½ÂÇßÀ»¶§¿Í ¹ÌÅ¾½Â ÇßÀ»¶§ ¸ó½ºÅÍ µ¥¹ÌÁö ÆÐ³ÎÆ¼
			if(targetKind == eObjectKind_TitanMonster)
			{
				//Å¸°ÙÀÌ Å¸ÀÌÅº ¸ó½ºÅÍÀÌ°í ÇÃ·¹ÀÌ¾î°¡ Å¸ÀÌÅº ¹Ì Å¾½Â½Ã ÆÐ³ÎÆ¼ µ¥¹ÌÁö (¿ø·¡ µ¥¹ÌÁöÀÇ 5%¸¸ µé¾î°¨)
				if(pPlayer->InTitan() == FALSE)
				{
					dwResultDemege = (DWORD)( (float)dwDemage * 0.05f );
				}				
			}
			else
			{
				//Å¸°ÙÀÌ ÀÏ¹Ý ¸ó½ºÅÍÀÌ°í ÇÃ·¹ÀÌ¾î°¡ Å¸ÀÌÅº Å¾½Â½Ã ÆÐ³ÎÆ¼ µ¥¹ÌÁö (¿ø·¡ µ¥¹ÌÁöÀÇ 50%¸¸ µé¾î°¨)
				if(pPlayer->InTitan() == TRUE)
				{
					dwResultDemege = (DWORD)( (float)dwDemage * 0.5f );
				}				
			}
		}
	}	

	return dwResultDemege;
}


