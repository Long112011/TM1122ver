// SkillObjectAttachUnit_CounterAttack.cpp: implementation of the CSkillObjectAttachUnit_CounterAttack class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SkillObjectAttachUnit_CounterAttack.h"

#ifdef _MHCLIENT_
#include "StatusIconDlg.h"
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSkillObjectAttachUnit_CounterAttack::CSkillObjectAttachUnit_CounterAttack()
{

}

CSkillObjectAttachUnit_CounterAttack::~CSkillObjectAttachUnit_CounterAttack()
{

}

void CSkillObjectAttachUnit_CounterAttack::Init( CSkillInfo* pSkillInfo,WORD SkillLevel,CObject* pOper/* =NULL */)
{
	m_Attrib = pSkillInfo->GetAttrib();
	m_AttrCounterRate = pSkillInfo->GetSkillInfo()->CounterAttAttack[SkillLevel-1];
	m_PhyCounterRate = pSkillInfo->GetSkillInfo()->CounterPhyAttack[SkillLevel-1];

#ifdef _MHCLIENT_
	
/*
maintain_counter_S.beff	물리 일부 반격
maintain_counter07_S.beff	성 일부 반격
*/
	//지효과표시
	if(m_AttrCounterRate > 0)
	{
		ySWITCH(m_Attrib)
			yCASE(ATTR_FIRE)
				
			yCASE(ATTR_WATER)
				
			yCASE(ATTR_TREE)
				m_StateIconNum[0] = eStatusIcon_AttrCounterTree;
			yCASE(ATTR_IRON)
				m_StateIconNum[0] = eStatusIcon_AttrCounterIron;
			yCASE(ATTR_EARTH)
				m_StateIconNum[0] = eStatusIcon_AttrCounterEarth;
			yCASE(ATTR_ALLATTR)
				m_StateIconNum[0] = eStatusIcon_AttrCounterAll;
		yENDSWITCH

		m_AttachEffectNum[0] = pSkillInfo->GetSkillInfo()->EffectMineOperate;
	}
	if(m_PhyCounterRate > 0)
	{
		m_AttachEffectNum[1] = pSkillInfo->GetSkillInfo()->EffectMineOperate;
		m_StateIconNum[1] = eStatusIcon_PhyCounter;
	}
/*
683	물리 반격 상태		물리 반격>0					
704	성 반격 증가 (화)		성 방어력 증가>0	성값 1				
707	성 반격 증가 (수)		성 방어력 증가>0	성값 2				
710	성 반격 증가 (목)		성 방어력 증가>0	성값 3				
713	성 반격 증가 (금)		성 방어력 증가>0	성값 4				
716	성 반격 증가 (토)		성 방어력 증가>0	성값 5				
719	성 반격 증가 (전성)		성 방어력 증가>0	성값 7				
*/
#endif
}

void CSkillObjectAttachUnit_CounterAttack::GetCounterAttackDamage(WORD Attrib,DWORD AttrDamage,DWORD PhyDamage,RESULTINFO* pDamageInfo,DWORD& CounterDamage)
{
	DWORD cdmg = 0;
	if(m_Attrib == Attrib || m_Attrib == ATTR_ALLATTR)
		cdmg += (DWORD)(AttrDamage * m_AttrCounterRate);

	cdmg += (DWORD)(PhyDamage * m_PhyCounterRate);

	if(cdmg > CounterDamage)
		CounterDamage = cdmg;
}
