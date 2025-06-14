// SkillObjectAttachUnit_MoveSpeed.cpp: implementation of the CSkillObjectAttachUnit_MoveSpeed class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SkillObjectAttachUnit_MoveSpeed.h"

#ifdef _MHCLIENT_
#include "StatusIconDlg.h"
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSkillObjectAttachUnit_MoveSpeed::CSkillObjectAttachUnit_MoveSpeed()
{
	m_Flag = STATUS_FLAG_MOVE;
}

CSkillObjectAttachUnit_MoveSpeed::~CSkillObjectAttachUnit_MoveSpeed()
{

}

void CSkillObjectAttachUnit_MoveSpeed::Init(CSkillInfo* pSkillInfo,WORD SkillLevel,CObject* pOper/* =NULL */)
{
	ASSERT(pSkillInfo->GetSkillInfo()->ChangeSpeedType == 1);
	m_MoveSpeedUp = 0;		// �� ������ ���ִ°� ���� ����
	m_MoveSpeedDown = pSkillInfo->GetSkillInfo()->ChangeSpeedRate;
	
//	m_Probability = pSkillInfo->GetSkillInfo()->ChangeSpeedProbability;
	
#ifdef _MHCLIENT_	
	//��ȿ��ǥ��
	m_AttachEffectNum[0] = pSkillInfo->GetSkillInfo()->EffectMineOperate;
	m_StateIconNum[0] = eStatusIcon_SpeedDown;
//	656	������ (���)		�� ��ȭ (1:�̵�)
#endif
}
	
void CSkillObjectAttachUnit_MoveSpeed::GetMoveSpeed(float Original,float& CalcedUp,float& CalcedDown)
{
	if(m_MoveSpeedUp)
		CalcedUp = Original * (m_MoveSpeedUp*0.01f);
	if(m_MoveSpeedDown)
		CalcedDown = Original * (m_MoveSpeedDown*0.01f);
}
