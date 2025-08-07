



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
	m_nDamageRate = 100.0f;	// magi82 cheat damage ó������ 100%�� �������� ����(ġƮŰ�� ���� �����)
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
///�µ�pvp//�����˺�����
DWORD CAttackManager::GetComboPhyDamage(CObject* pAttacker, CObject* pTargetObject, float PhyAttackRate, float fCriticalRate,
	RESULTINFO* pDamageInfo, DWORD AmplifiedPower, float fDecreaseDamageRate)
{
	pDamageInfo->bCritical = m_ATTACKCALC.getCritical(pAttacker, pTargetObject, fCriticalRate);
	double attackPhyDamage = m_ATTACKCALC.getPhysicalAttackPower(pAttacker, PhyAttackRate, pDamageInfo->bCritical);

	// �����ж�����
// �����ж�����
	if (pDamageInfo->bCritical)
	{
		//attackPhyDamage *= 1.5f;
		attackPhyDamage *= gEventRate[eEvent_MugongPhyCritical];//�����˺��������� 

		// ��ҹ����ߣ�����Ʒ�ʱ����˺�����Ч�˺�
		if (pAttacker->GetObjectKind() == eObjectKind_Player)
		{
			// Ʒ����װ�����ӳ�
			const ITEMBASE* pTargetItemBase = ITEMMGR->GetItemInfoAbsIn((CPlayer*)pAttacker, 81);
			if (pTargetItemBase)
			{//��������
				ITEM_INFO* pItemInfo = ITEMMGR->GetItemInfo(pTargetItemBase->wIconIdx);
				if (pItemInfo && pItemInfo->WeaponType == 3)
				{
					attackPhyDamage += (attackPhyDamage * 0.15f); // �����������ʱ������һ���˺�����15%
				}
			}

			// ��װ�����˺��ٷֱ�
			float fBonusCrit = ((CPlayer*)pAttacker)->GetSetItemQualityStats()->CriticalDamage * 0.01f;
			attackPhyDamage += (attackPhyDamage * fBonusCrit);

			// ϡ�е��߹̶������ӳ�
			attackPhyDamage += (double)(((CPlayer*)pAttacker)->GetUniqueItemStats()->nCriDamage);

			if (attackPhyDamage < 0.f)
				attackPhyDamage = 1.f;
		}

		// ��������Ϊ���ʱ��������������˺����ִ��� / �˴��ˣ�
		if (pTargetObject->GetObjectKind() == eObjectKind_Player)
		{
			((CPlayer*)pTargetObject)->GetPetManager()->GetPetBuffResultRt(ePB_ReduceCriticalDmg, &attackPhyDamage);
		}
	}


	attackPhyDamage += AmplifiedPower;
	attackPhyDamage *= fDecreaseDamageRate;

	if (pAttacker->GetObjectKind() == eObjectKind_Player && pTargetObject->GetObjectKind() == eObjectKind_Player)
		attackPhyDamage *= gEventRate[eEvent_MugongPhy];//�ڴ˴�����bin�������� �˺�
	// ===== PvP �ӳɣ�������=====
	if (pAttacker->GetObjectKind() == eObjectKind_Player &&
		pTargetObject->GetObjectKind() == eObjectKind_Player)
	{
		attackPhyDamage *= (1.0f + ((CPlayer*)pAttacker)->GetItemStats()->PVPAttack);
		attackPhyDamage *= (1.0f + ((CPlayer*)pAttacker)->GetAvatarOption()->PVPAttack);
	}
	// ===========================
	// === �̿��������������˺����ʣ��� bin ���ñ��ȡ��===
	if (pAttacker->GetObjectKind() == eObjectKind_Player)
	{
		const ITEMBASE* pItemBase = ITEMMGR->GetItemInfoAbsIn((CPlayer*)pAttacker, 81); // ��������
		if (pItemBase)
		{
			ITEM_INFO* pItemInfo = ITEMMGR->GetItemInfo(pItemBase->wIconIdx);
			if (pItemInfo && pItemInfo->WeaponType == 11) // �̿���������
			{
				// ������gEventRate[eEvent_AssassinPhyDmg] = 1.50f����ʾ+50%�˺���
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
			attackPhyDamage *= (fdam + 1.0f) * gEventRate[eEvent_MugongKillMonster];//PVE�˺�����

		}
		if (pTargetObject->GetObjectKind() == eObjectKind_Player)
		{//PVP�Ƽ�
			fdam = (((CPlayer*)pAttacker)->GetSetItemQualityStats()->PlayerPhyDefDown * 0.01f);//PVP�Ƽ�
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

//�ɵ�
//DWORD CAttackManager::GetComboPhyDamage(CObject* pAttacker,CObject* pTargetObject,float PhyAttackRate,float fCriticalRate,
//										RESULTINFO* pDamageInfo,DWORD AmplifiedPower,float fDecreaseDamageRate )
//{
//	pDamageInfo->bCritical = m_ATTACKCALC.getCritical(pAttacker,pTargetObject,fCriticalRate);
//	double attackPhyDamage = m_ATTACKCALC.getPhysicalAttackPower(pAttacker,PhyAttackRate,pDamageInfo->bCritical);
//
//	// RaMa - ũ��Ƽ���϶� ����Ȯ�� �߰��ؾ���
//	//#ifndef _JAPAN_LOCAL_	//�ٸ���(getPlayerPhysicalAttackPower) ���Ҵ�.
//	if(pDamageInfo->bCritical)
//	{
//		attackPhyDamage *= 1.5f;
//		/*
//		// ����
//		if( pAttacker->GetObjectKind() == eObjectKind_Player )
//		if((rand()%100) < ((CPlayer*)pAttacker)->GetShopItemStats()->StunByCri)
//		{
//		// RaMa - 04.11.18
//		// �����۸������� ���ΰ�ȹ���� ��������( ~11.27)�� �ޱ�� ��.
//		// pTargetObject->StartSpecialState( eSpecialState_Stun, 10, 0, 0, 0, FindEffectNum("monster_stun_s.beff"), 0 );
//		}
//		}*/
//		//SW060906 �ű���
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
//	//PvP �϶��� ���ݷ��� 50%��
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
//	// 06. 03 �������� - �̿���
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
//	// RaMa - 04.11.24   ->����������
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
//	// RaMa - 04.11.10    -> ShopItemOption�߰�
//	if(pAttacker->GetObjectKind() == eObjectKind_Player)
//	{
//		attackPhyDamage *= (((CPlayer*)pAttacker)->GetShopItemStats()->ComboDamage*0.01f+1.0f);
//
//		//SW060719 ��������Ʈ
//		if( ((CPlayer*)pAttacker)->GetGuildIdx() )
//			GUILDMGR->GetGuildPlustimeRt( ((CPlayer*)pAttacker)->GetGuildIdx(), eGPT_DamageUp, &attackPhyDamage );
//
//		//PET %���ݷ�
//		((CPlayer*)pAttacker)->GetPetManager()->GetPetBuffResultRt(ePB_Demage_Percent, &attackPhyDamage);
//
//		// RaMa - 05.10.10 -> �ذ񰡸� �ɼ��߰�
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
//		// RaMa - 06.11.13 -> ���ָ���϶� ���ݷ����� �ƹ�Ÿ
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
//		// RaMa - 05.10.10 -> �ذ񰡸� �ɼ��߰�
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


////�ɵ�//////////////////////////////////////////
//DWORD CAttackManager::GetMugongPhyDamage(CObject* pAttacker,CObject* pTargetObject,float PhyAttackRate,float fCriticalRate,
//										 RESULTINFO* pDamageInfo,DWORD AmplifiedPower,float fDecreaseDamageRate )
//{
//	pDamageInfo->bCritical = m_ATTACKCALC.getCritical(pAttacker,pTargetObject,fCriticalRate);
//	double attackPhyDamage = m_ATTACKCALC.getPhysicalAttackPower(pAttacker,PhyAttackRate,pDamageInfo->bCritical );
//
//	// RaMa - ũ��Ƽ���϶� ����Ȯ�� �߰��ؾ���
//	//#ifndef _JAPAN_LOCAL_ //�ٸ���(getPlayerPhysicalAttackPower) ���Ҵ�.
//	if(pDamageInfo->bCritical)
//	{
//		attackPhyDamage *= 1.5f;
//		/*
//		// ����
//		if( pAttacker->GetObjectKind() == eObjectKind_Player )
//		if((rand()%100) < ((CPlayer*)pAttacker)->GetShopItemStats()->StunByCri)
//		{
//		// RaMa - 04.11.18
//		// �����۸������� ���ΰ�ȹ���� ��������( ~11.27)�� �ޱ�� ��.
//		// pTargetObject->StartSpecialState( eSpecialState_Stun, 10, 0, 0, 0, FindEffectNum("monster_stun_s.beff"), 0 );
//		}
//		*/
//		//SW060906 �ű���
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
//	//PvP �϶��� ���ݷ��� 50%��
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
//	// 06. 03 �������� - �̿���
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
//	// RaMa - 04.11.24   ->����������
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
//	// RaMa - 04.11.10    -> ShopItemOption�߰�, AvatarItemOption�߰�(05.08.16)
//	if(pAttacker->GetObjectKind() == eObjectKind_Player)
//	{
//		fdam = (((CPlayer*)pAttacker)->GetShopItemStats()->WoigongDamage*0.01f) + 
//			(((CPlayer*)pAttacker)->GetAvatarOption()->WoigongDamage*0.01f);
//		attackPhyDamage *= (fdam+1.0f);
//
//		//SW060719 ��������Ʈ
//		if( ((CPlayer*)pAttacker)->GetGuildIdx() )
//			GUILDMGR->GetGuildPlustimeRt( ((CPlayer*)pAttacker)->GetGuildIdx(), eGPT_DamageUp, &attackPhyDamage );
//
//		////PET %���ݷ�========================================================
//		((CPlayer*)pAttacker)->GetPetManager()->GetPetBuffResultRt(ePB_Demage_Percent, &attackPhyDamage);
//
//		// RaMa - 05.10.10 -> �ذ񰡸� �ɼ��߰�
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
//		// RaMa - 06.11.13 -> ���ָ���϶� ���ݷ����� �ƹ�Ÿ
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
//		// RaMa - 05.10.10 -> �ذ񰡸� �ɼ��߰�
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
//�书�����˺�����
DWORD CAttackManager::GetMugongPhyDamage(CObject* pAttacker, CObject* pTargetObject, float PhyAttackRate, float fCriticalRate,
	RESULTINFO* pDamageInfo, DWORD AmplifiedPower, float fDecreaseDamageRate)
{
	pDamageInfo->bCritical = m_ATTACKCALC.getCritical(pAttacker, pTargetObject, fCriticalRate);
	double attackPhyDamage = m_ATTACKCALC.getPhysicalAttackPower(pAttacker, PhyAttackRate, pDamageInfo->bCritical);

	if (pDamageInfo->bCritical)
	{
		//attackPhyDamage *= 1.5f;
		attackPhyDamage *= gEventRate[eEvent_MugongPhyCritical];//սʿ�����˺��������� 

		// �� ���������� + װ�������˺��ӳ�
		if (pAttacker->GetObjectKind() == eObjectKind_Player)
		{
			CPlayer* pPlayer = (CPlayer*)pAttacker;

			const ITEMBASE* pTargetItemBase = ITEMMGR->GetItemInfoAbsIn(pPlayer, 81); // 81 = ������
			if (pTargetItemBase)
			{//��������
				ITEM_INFO* pItemInfo = ITEMMGR->GetItemInfo(pTargetItemBase->wIconIdx);
				if (pItemInfo && pItemInfo->WeaponType == 3) // ��������
				{
					attackPhyDamage += (attackPhyDamage * 0.15f); // +15%����һ���˺�
				}
			}

			// Ʒ��ϵͳ�����˺�����
			attackPhyDamage += (attackPhyDamage * (pPlayer->GetSetItemQualityStats()->CriticalDamage * 0.01f));

			// ϡ��/����װ�����ӱ����˺�
			attackPhyDamage += (float)pPlayer->GetUniqueItemStats()->nCriDamage;
		}

		// �� ������������� �� ���ﱩ������
		if (pTargetObject->GetObjectKind() == eObjectKind_Player)
		{
			CPlayer* pTargetPlayer = (CPlayer*)pTargetObject;
			float fReduceCritDmg = 0.0f;
			pTargetPlayer->GetPetManager()->GetPetBuffResultRt(ePB_ReduceCriticalDmg, &fReduceCritDmg);

			// ��ȫ�ж������Ӧ��
			if (fReduceCritDmg > 0.0f && fReduceCritDmg <= 1.0f)
				attackPhyDamage *= (1.0f - fReduceCritDmg);
		}
	}


	attackPhyDamage += AmplifiedPower;
	attackPhyDamage *= fDecreaseDamageRate;
	if (pAttacker->GetObjectKind() == eObjectKind_Player && pTargetObject->GetObjectKind() == eObjectKind_Player)
		attackPhyDamage *= gEventRate[eEvent_MugongPhy];//�ڴ˴�����bin�����书�����˺�

	//  ���� PvP �����ӳɣ��� gEventRateFile��
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
	{//�⹦�˺�����
		fdam = (((CPlayer*)pAttacker)->GetShopItemStats()->WoigongDamage * 0.01f) +
			(((CPlayer*)pAttacker)->GetAvatarOption()->WoigongDamage * 0.01f) + (((CPlayer*)pAttacker)->GetSetItemQualityStats()->WoigongDamage * 0.01f);

		attackPhyDamage *= (fdam + 1.0f);//PVP  ������ǰ��ȫ��������

		//SW060719 ��������Ʈ
		if (((CPlayer*)pAttacker)->GetGuildIdx())
			GUILDMGR->GetGuildPlustimeRt(((CPlayer*)pAttacker)->GetGuildIdx(), eGPT_DamageUp, &attackPhyDamage);

		////PET %���ݷ�========================================================
		((CPlayer*)pAttacker)->GetPetManager()->GetPetBuffResultRt(ePB_Demage_Percent, &attackPhyDamage);
		//��ȡ���������Ϣ
		const ITEMBASE* pTargetItemBase = ITEMMGR->GetItemInfoAbsIn((CPlayer*)pAttacker, 81);
		ITEM_INFO* pItemInfo = ITEMMGR->GetItemInfo(pTargetItemBase->wIconIdx);
		// RaMa - 05.10.10 -> ��		�񰡸� �ɼ��߰�
		if (pTargetObject->GetObjectKind() == eObjectKind_Monster)
		{//PVE�Ƽ�
			//if( g_pServerSystem->GetMapNum() != BOSSMONSTER_MAP &&
			//	g_pServerSystem->GetMapNum() != BOSSMONSTER_2NDMAP )
			if (FALSE == g_pServerSystem->GetMap()->IsMapKind(eBossMap))
			{
				fdam = (((CPlayer*)pAttacker)->GetAvatarOption()->TargetPhyDefDown * 0.01f);
				//attackPhyDamage *= (fdam+1.0f);
			}//��������
			if (pItemInfo->WeaponType == 5)//����������ǹ�������15%�Ƽ�
				fdam += 0.15f;
			//���ӶԷ�15%����
			fdam += fdam * (((CPlayer*)pAttacker)->GetSetItemQualityStats()->TargetPhyDefDown * 0.01f);
			attackPhyDamage *= (fdam + 1.0f) * gEventRate[eEvent_MugongKillMonster];//PVE�˺�����

		}
		if (pTargetObject->GetObjectKind() == eObjectKind_Player)
		{//PVP�Ƽ�//��������
			if (pItemInfo->WeaponType == 5)//����������ǹ�������15%�Ƽף����ӶԷ�15%������
				fdam = (((CPlayer*)pAttacker)->GetSetItemQualityStats()->PlayerPhyDefDown * 0.01f) + 0.15f;//PVP�Ƽ�
			else
				fdam = (((CPlayer*)pAttacker)->GetSetItemQualityStats()->PlayerPhyDefDown * 0.01f);//PVP�Ƽ�

			//attackPhyDamage *= (fdam+1.0f);
			attackPhyDamage *= (fdam + 1.0f);
		}
		// RaMa - 06.11.13 -> ���ָ���϶� ���ݷ����� �ƹ�Ÿ
		if (((CPlayer*)pAttacker)->IsMussangMode() && ((CPlayer*)pAttacker)->GetAvatarOption()->MussangDamage)
			attackPhyDamage *= (((CPlayer*)pAttacker)->GetAvatarOption()->MussangDamage * 0.01f + 1.0f);

		// magi82 - UniqueItem(070627)
		attackPhyDamage += (double)(((CPlayer*)pAttacker)->GetUniqueItemStats()->nPhyDamage);
		//��������
		if (pItemInfo->WeaponType == 6)//�������������ʱ������5%�����˺�
		{
			if (pAttacker->GetLife() < (pAttacker->DoGetMaxLife()) * 0.5)
				attackPhyDamage = attackPhyDamage * 1.20;//Ѫ������50%ʱ��15%�����˺�
			else
				attackPhyDamage = attackPhyDamage * 1.05;
		}
		if (attackPhyDamage < 0.f)
			attackPhyDamage = 1.f;
	}
	else if (pAttacker->GetObjectKind() & eObjectKind_Monster)//������˵��˺�����
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

//[���Թ����˺�����][2017/12/7]
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

	//   ������PvP ���Թ����ӳɣ������ߣ�
	if (pAttacker->GetObjectKind() == eObjectKind_Player && pTargetObject->GetObjectKind() == eObjectKind_Player)
	{
		attackAttrDamage *= (1.0f + ((CPlayer*)pAttacker)->GetItemStats()->PVPAttack);
		attackAttrDamage *= (1.0f + ((CPlayer*)pAttacker)->GetAvatarOption()->PVPAttack);
	}

	if (pDamageInfo->bDecisive)//�书���Թ��������˺�
	{
		//attackAttrDamage *= 2.25f;
		attackAttrDamage *= gEventRate[eEvent_MugongAttrCritical];//��ʦ�����˺��������� 

		if (pAttacker->GetObjectKind() == eObjectKind_Player)
		{//��������
			const ITEMBASE* pTargetItemBase = ITEMMGR->GetItemInfoAbsIn((CPlayer*)pAttacker, 81);
			ITEM_INFO* pItemInfo = ITEMMGR->GetItemInfo(pTargetItemBase->wIconIdx);
			if (pItemInfo->WeaponType == 3)//�����ߴ�����ʱ������15%����һ���˺�
				attackAttrDamage += (attackAttrDamage * 0.15f);//��ʦ�����˺��������� 

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
		attackAttrDamage *= gEventRate[eEvent_MugongAttr];//�ڴ˴�����bin���������˺�//��ʦ�����˺�����		
	}
	DWORD ShieldDamage = 0;
	DWORD ReduceDamage = pTargetObject->CalcShieldDamage((DWORD)(attackAttrDamage * SHIELD_IN_MUGONG_DAMAGE), ShieldDamage);
	attackAttrDamage -= ReduceDamage;
#endif

	pDamageInfo->ShieldDamage += ShieldDamage;

	float RegVal = pTargetObject->GetAttDefense(Attrib);

	//   ������PvP ���Է����ӳɣ������ߣ�
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
	{//�ڹ��˺�����
		fdam = (((CPlayer*)pAttacker)->GetShopItemStats()->NeagongDamage * 0.01f) +
			(((CPlayer*)pAttacker)->GetAvatarOption()->NeagongDamage * 0.01f) + (((CPlayer*)pAttacker)->GetSetItemQualityStats()->NaegongDamage * 0.01f);

		//resAttrDamage = (resAttrDamage*(fdam+1.0f));
		resAttrDamage *= (fdam + 1.0f);//PVP  ������ǰ��ȫ��������

		//SW060719 ��������Ʈ
		if (((CPlayer*)pAttacker)->GetGuildIdx())
			GUILDMGR->GetGuildPlustimeRt(((CPlayer*)pAttacker)->GetGuildIdx(), eGPT_DamageUp, &resAttrDamage);

		//PET %���ݷ�
		((CPlayer*)pAttacker)->GetPetManager()->GetPetBuffResultRt(ePB_Demage_Percent, &resAttrDamage);
		//��ȡ���������Ϣ
		const ITEMBASE* pTargetItemBase = ITEMMGR->GetItemInfoAbsIn((CPlayer*)pAttacker, 81);
		ITEM_INFO* pItemInfo = ITEMMGR->GetItemInfo(pTargetItemBase->wIconIdx);

		// RaMa - 05.10.10 -> �ذ񰡸� �ɼ��߰�
		if (pTargetObject->GetObjectKind() == eObjectKind_Monster)
		{//PVE��ħ
			//if( g_pServerSystem->GetMapNum() == BOSSMONSTER_MAP ||
			//	g_pServerSystem->GetMapNum() == BOSSMONSTER_2NDMAP )	goto CalcEnd;
			//if( g_pServerSystem->GetMap()->IsMapKind(eBossMap) )
			//	goto CalcEnd;
			if (FALSE == g_pServerSystem->GetMap()->IsMapKind(eBossMap))
			{
				fdam = (((CPlayer*)pAttacker)->GetAvatarOption()->TargetAttrDefDown * 0.01f);
			}//��������
			if (pItemInfo->WeaponType == 6)//�������������ʱ��pve��ħ15%
				fdam += 0.15f;
			//���ӶԷ�15%����
			fdam += fdam * (((CPlayer*)pAttacker)->GetSetItemQualityStats()->TargetPhyDefDown * 0.01f);
			//resAttrDamage = (resAttrDamage*(fdam+1.0f));
			resAttrDamage *= (fdam + 1.0f) * gEventRate[eEvent_MugongKillMonster];//PVE�˺�����

		}
		if (pTargetObject->GetObjectKind() == eObjectKind_Player)
		{//PVP��ħ//��������
			if (pItemInfo->WeaponType == 6)//�������������ʱ��pvp��ħ8%
				fdam = ((CPlayer*)pAttacker)->GetSetItemQualityStats()->PlayerAttrDefDown * 0.01f + 0.08f;
			else
				fdam = ((CPlayer*)pAttacker)->GetSetItemQualityStats()->PlayerAttrDefDown * 0.01f;

			resAttrDamage *= (fdam + 1.0f);
		}
		// RaMa - 06.11.13 -> ���ָ���϶� ���ݷ����� �ƹ�Ÿ
		if (((CPlayer*)pAttacker)->IsMussangMode() && ((CPlayer*)pAttacker)->GetAvatarOption()->MussangDamage)
			resAttrDamage *= (((CPlayer*)pAttacker)->GetAvatarOption()->MussangDamage * 0.01f + 1.0f);

		// magi82 - UniqueItem(070627)
		resAttrDamage *= (((CPlayer*)pAttacker)->GetUniqueItemStats()->nAttR * 0.01f + 1.0f);
		//��������
		if (pItemInfo->WeaponType == 6)//�������������ʱ������5%�����˺�
		{
			if (pAttacker->GetLife() < (pAttacker->DoGetMaxLife()) * 0.5)
				resAttrDamage = resAttrDamage * 1.15f;//Ѫ������50%ʱ��15%�����˺�
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

/////�ɵ�//////
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
//		//SW060906 �ű���
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
//	//PvP �϶��� ���ݷ��� 50%��
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
//	// 06. 03 �������� - �̿���
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
//	// 06. 06. 2�� ���� - �̿���
//	// ���� ��ȯ �߰�
//	// �Ӽ�����
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
//	// RaMa - 04.11.24   ->����������
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
//	// RaMa - 04.11.10    -> ShopItemOption�߰�, AvatarItemOption�߰�(05.08.16)
//	if(pAttacker->GetObjectKind() == eObjectKind_Player)
//	{
//		fdam = (((CPlayer*)pAttacker)->GetShopItemStats()->NeagongDamage*0.01f)+
//			(((CPlayer*)pAttacker)->GetAvatarOption()->NeagongDamage*0.01f);
//		resAttrDamage = (resAttrDamage*(fdam+1.0f));
//
//		//SW060719 ��������Ʈ
//		if( ((CPlayer*)pAttacker)->GetGuildIdx() )
//			GUILDMGR->GetGuildPlustimeRt( ((CPlayer*)pAttacker)->GetGuildIdx(), eGPT_DamageUp, &resAttrDamage );
//
//		//PET %���ݷ�
//		((CPlayer*)pAttacker)->GetPetManager()->GetPetBuffResultRt(ePB_Demage_Percent, &resAttrDamage);
//
//		// RaMa - 05.10.10 -> �ذ񰡸� �ɼ��߰�
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
//		// RaMa - 06.11.13 -> ���ָ���϶� ���ݷ����� �ƹ�Ÿ
//		if( ((CPlayer*)pAttacker)->IsMussangMode() && ((CPlayer*)pAttacker)->GetAvatarOption()->MussangDamage )
//			resAttrDamage *= (((CPlayer*)pAttacker)->GetAvatarOption()->MussangDamage*0.01f+1.0f);
//
//		// magi82 - UniqueItem(070627)
//		resAttrDamage *= (((CPlayer*)pAttacker)->GetUniqueItemStats()->nAttR * 0.01f + 1.0f);
//	}	
//	else if( pAttacker->GetObjectKind() & eObjectKind_Monster )
//	{
//		// RaMa - 05.10.10 -> �ذ񰡸� �ɼ��߰�
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





//////�Ѿ���������pvpû���½�
void CAttackManager::Attack(BOOL bMugong, CObject* pAttacker,CObject* pTarget,DWORD AmplifiedPower,
							float PhyAttackRate,
							WORD Attrib,DWORD AttAttackMin,DWORD AttAttackMax,float AttAttackRate,
							float fCriticalRate,
							RESULTINFO* pDamageInfo,BOOL bCounter, float fDecreaseDamageRate,
							WORD AmplifiedPowerAttrib, BOOL bContinueAttack )
{	
	pDamageInfo->Clear();

	// ��ȡĿ��������
	float fDodgeRate = pTarget->GetDodgeRate();
	// ��Ŀ��Ϊ��ң�����װ��Ӱ��
	if (pTarget->GetObjectKind() == eObjectKind_Player)
	{//��������
		// �ж��Ƿ������ǹ����WeaponType == 4�����ӳ� +10%
		const ITEMBASE* pTargetItemBase = ITEMMGR->GetItemInfoAbsIn((CPlayer*)pTarget, 81);
		ITEM_INFO* pItemInfo = ITEMMGR->GetItemInfo(pTargetItemBase->wIconIdx);
		if (pItemInfo && pItemInfo->WeaponType == 4)
			fDodgeRate += fDodgeRate * 0.1f;

		// ��װƷ�����ܼӳ�
		fDodgeRate += fDodgeRate * (((CPlayer*)pTarget)->GetSetItemQualityStats()->wDodgeRate * 0.01f);
	}
	//  PvP ���������߼������������� & Ŀ�����ܣ�
	if (pAttacker->GetObjectKind() == eObjectKind_Player && pTarget->GetObjectKind() == eObjectKind_Player)
	{
		// ���������мӳɣ�����Ŀ�����ܣ�
		fDodgeRate -= ((CPlayer*)pAttacker)->GetItemStats()->PVPHit;
		fDodgeRate -= ((CPlayer*)pAttacker)->GetAvatarOption()->PVPHit;

		// Ŀ�����ܼӳɣ��������ܣ�
		fDodgeRate += ((CPlayer*)pTarget)->GetItemStats()->PVPADodge;
		fDodgeRate += ((CPlayer*)pTarget)->GetAvatarOption()->PVPADodge;
	}
	if (pAttacker->GetObjectKind() == eObjectKind_Player)
	{//��������
		const ITEMBASE* pTargetItemBase = ITEMMGR->GetItemInfoAbsIn((CPlayer*)pAttacker, 81);
		ITEM_INFO* pItemInfo = ITEMMGR->GetItemInfo(pTargetItemBase->wIconIdx);
		if (pItemInfo->WeaponType == 1)//�����������ʱ����������10
			fDodgeRate = fDodgeRate - 10;

		fDodgeRate -= (fDodgeRate * (((CPlayer*)pAttacker)->GetSetItemQualityStats()->fDodgeRate) * 0.01f);//װ�����мӳ�
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
		//�� ���� ȸ��
		BOOL rtDodge = FALSE;
		((CPlayer*)pTarget)->GetPetManager()->GetPetBuffResultRt(ePB_Dodge, &rtDodge);
		if( TRUE == rtDodge )
		{
			pDamageInfo->bDodge = TRUE;
			pDamageInfo->RealDamage = 0;
			pDamageInfo->ShieldDamage = 0;
			return;
		}

		//SW060911 ȫ���û ����.
		if( ((CPlayer*)pTarget)->GetUserLevel() <= eUSERLEVEL_GM && ((CPlayer*)pTarget)->IsVisible() == FALSE )
		{
			return;
		}
	}

	// 2005 ũ�������� �̺�Ʈ
	if(( pAttacker->GetObjectKind() == eObjectKind_Player ) && ( pTarget->GetObjectKind() & eObjectKind_Monster ))
	{	// �÷��̾ ���� ���ݽ�
		CPlayer* pPlayer = (CPlayer*)pAttacker;
		CMonster* pMonster = (CMonster*)pTarget;

		// ���� �̺�Ʈ ���� �������̶��
		if( pPlayer->GetWeaponEquipType() == WP_EVENT || pPlayer->GetWeaponEquipType() == WP_EVENT_HAMMER )
		{
			// ������ ����Ѱ��̸� �����Ѵ�.
			if(bMugong)
				return;

			// ���� ���ݵ� ����
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
	pAttackPlayer->SetPartyHelp( TRUE );	//��Ƽ ��� ȿ�� 
	}
	}
	}
	#endif
	*/
	//�����˺��ӳɼ���
	if (pAttacker->GetObjectKind() == eObjectKind_Player)
	{
		AttAttackMin += AttAttackMin * (((CPlayer*)pAttacker)->GetSetItemQualityStats()->ContinueAttAttack) * 0.01f ;
		AttAttackMax += AttAttackMax * (((CPlayer*)pAttacker)->GetSetItemQualityStats()->ContinueAttAttack) * 0.01f ;
	}
	if(PhyAttackRate > 0.000001f)
	{//�⹦�˺�
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
		// �ڹ��˺�
		AttrDamage = GetMugongAttrDamage(pAttacker,pTarget,Attrib,AttAttackMin,AttAttackMax,AttAttackRate,fCriticalRate,pDamageInfo,fDecreaseDamageRate );
	}

	if (pTarget->GetObjectKind() == eObjectKind_Player)
	{
		((CPlayer*)pTarget)->GetPetManager()->GetPetBuffResultRt(ePB_ReduceDemageRate, &PhyDamage);
		((CPlayer*)pTarget)->GetPetManager()->GetPetBuffResultRt(ePB_ReduceDemageRate, &AttrDamage);
	}

	//װ�����������˺��ӳ�
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
	//װ�����������˺�����
	if (pTarget->GetObjectKind() == eObjectKind_Player)
	{
		fdam = ((CPlayer*)pTarget)->GetSetItemQualityStats()->RealDamageDown * 0.01f;
		if (PhyAttackRate > 0.000001f)
			PhyDamage *= (1.0f - fdam);
		if (AttAttackMax != 0)
			AttrDamage *= (1.0f - fdam);
	}
	//������Ѫ
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

	//== �̻��ϰ� �Ǿ��� T_T �ϴ� �ӽ÷� �׳� ���ô�!
	DWORD UpAttackDamage = 0;
	if( PhyDamage + AttrDamage < pDamageInfo->RealDamage )
		UpAttackDamage = ( pDamageInfo->RealDamage - ( PhyDamage + AttrDamage ) ) / 2;
	//===

	//SW070811 ��ȹ���� pvp �� �� 10% ������.	//attackPhyDamage *= 0.1f;	//attackAttrDamage *= 0.1f;
	// �������ÿ��� ������ ����
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
			// ����ؼ� 0�� ���ö��� ���ؼ� +1
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

	// RaMa - ������ �̻��ġ Ȯ�ο� �ڵ�
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
		pAttackPlayer->SetPartyHelp( FALSE );	//��� ������. �ʱ�ȭ����.
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

			//PK���ÿ� PK�ð� ����
			if( pAttackPlayer->IsPKMode() /*&& pAttackPlayer->IsVimuing() == FALSE*/ ) //���ϸ��� ����.
			{
				if( AttrDamage || PhyDamage )
					if( pTargetPlayer->IsPKMode() == FALSE )
						pAttackPlayer->SetPKStartTimeReset();
			}

			// 06.09.25 RaMa �߼��̺�Ʈ
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

	//-----�޴� �������� ���� ü�� ���� ���


	pTarget->CalcReverseVampiric(pDamageInfo);	//����Ʈ ó���� ���Ѵ� --;
	//-----


	//	BOOL bVampiric = TRUE;
	//	if(bVampiric)
	//	{
	pAttacker->CalcVampiric(pDamageInfo);
	//	}

	// 06. 03 �������� - �̿���
#ifndef _JAPAN_LOCAL_
	if( pAttacker->GetObjectKind() == eObjectKind_Player )
	{
		if(((CPlayer*)pAttacker)->IsMussangMode())
			pDamageInfo->RealDamage = (DWORD)(pDamageInfo->RealDamage * 1.05);
	}
#endif
	if (pTarget->GetObjectKind() == eObjectKind_Player)
	{//��������
		const ITEMBASE* pTargetItemBase = ITEMMGR->GetItemInfoAbsIn((CPlayer*)pTarget, 81);
		ITEM_INFO* pItemInfo = ITEMMGR->GetItemInfo(pTargetItemBase->wIconIdx);
		if (pItemInfo->WeaponType == 4)//�����������ǹʱ�������˺�����5%
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

	// ��ȡĿ��������
	float fDodgeRate = pTarget->GetDodgeRate();

	//  PvP �������������������� & Ŀ�����ܣ�
	if (pAttacker->GetObjectKind() == eObjectKind_Player &&
		pTarget->GetObjectKind() == eObjectKind_Player)
	{
		// ���з�������
		fDodgeRate -= ((CPlayer*)pAttacker)->GetItemStats()->PVPHit;
		fDodgeRate -= ((CPlayer*)pAttacker)->GetAvatarOption()->PVPHit;

		// ������������
		fDodgeRate += ((CPlayer*)pTarget)->GetItemStats()->PVPADodge;
		fDodgeRate += ((CPlayer*)pTarget)->GetAvatarOption()->PVPADodge;

		//  ��ѡ�����������ʲ�������Χ�����ⳬ�����ʣ�//���� 100%
		if (fDodgeRate < 0.0f) fDodgeRate = 0.0f;
		if (fDodgeRate > 1.0f) fDodgeRate = 1.0f;
	}

	// �ж�����
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

	// ��������
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

	// ����״̬��֤
	if (!pTarget->GetInited() ||
		pTarget->GetState() == eObjectState_Die ||
		pTarget->GetState() == eObjectState_Immortal ||
		pAttacker->GetGridID() != pTarget->GetGridID())
	{
		return;
	}

	// PK ģʽ�ж�
	if (pAttacker->GetObjectKind() == eObjectKind_Player &&
		((CPlayer*)pAttacker)->IsPKMode() &&
		pTarget->GetObjectKind() == eObjectKind_Player &&
		AttackRate > 0.0f &&
		!((CPlayer*)pTarget)->IsPKMode())
	{
		((CPlayer*)pAttacker)->SetPKStartTimeReset();
	}

	// �����˺�
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

	//  PvP ���� / ���ܼӳɵ���
	if (pAttacker->GetObjectKind() == eObjectKind_Player &&
		pTarget->GetObjectKind() == eObjectKind_Player)
	{
		fDodgeRate -= ((CPlayer*)pAttacker)->GetItemStats()->PVPHit;
		fDodgeRate -= ((CPlayer*)pAttacker)->GetAvatarOption()->PVPHit;

		fDodgeRate += ((CPlayer*)pTarget)->GetItemStats()->PVPADodge;
		fDodgeRate += ((CPlayer*)pTarget)->GetAvatarOption()->PVPADodge;

		// ���������ʷ�Χ�� 0.0f ~ 1.0f
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

	//PvP �϶��� ���ݷ��� 50%��
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
	// 06. 03 �������� - �̿���
	DWORD ShieldDamage = 0;
	DWORD ReduceDamage = pTargetObject->CalcShieldDamage((DWORD)(attackPhyDamage * SHIELD_OUT_MUGONG_DAMAGE), ShieldDamage);
	attackPhyDamage -= ReduceDamage;
#endif

	pDamageInfo->ShieldDamage += (DWORD)ShieldDamage;

	double defencePhyLevel = m_ATTACKCALC.getPhyDefenceLevel(pTargetObject,pAttacker);
	attackPhyDamage = (DWORD)(attackPhyDamage*(1-defencePhyLevel));

	// RaMa - 04.11.24   ->����������
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

//2007. 10. 30. CBH - ������ �г�Ƽ ��� �Լ�
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
			//Ÿ��ź ž���������� ��ž�� ������ ���� ������ �г�Ƽ
			if(targetKind == eObjectKind_TitanMonster)
			{
				//Ÿ���� Ÿ��ź �����̰� �÷��̾ Ÿ��ź �� ž�½� �г�Ƽ ������ (���� �������� 5%�� ��)
				if(pPlayer->InTitan() == FALSE)
				{
					dwResultDemege = (DWORD)( (float)dwDemage * 0.05f );
				}				
			}
			else
			{
				//Ÿ���� �Ϲ� �����̰� �÷��̾ Ÿ��ź ž�½� �г�Ƽ ������ (���� �������� 50%�� ��)
				if(pPlayer->InTitan() == TRUE)
				{
					dwResultDemege = (DWORD)( (float)dwDemage * 0.5f );
				}				
			}
		}
	}	

	return dwResultDemege;
}


