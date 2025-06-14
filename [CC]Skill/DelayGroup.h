// DelayGroup.h: interface for the CDelayGroup class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DELAYGROUP_H__9E742F39_D6EB_4FA4_BB74_50B937A618A0__INCLUDED_)
#define AFX_DELAYGROUP_H__9E742F39_D6EB_4FA4_BB74_50B937A618A0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//#include "../[Lib]YHLibrary/YHHashTable.h"

class CDelayGroup  
{
	struct CDelay
	{
		WORD Kind;
		WORD Idx;
		DWORD StartTime;
		DWORD DelayTime;
	};
	static CMemoryPoolTempl<CDelay>* m_pDelayPool;
	static DWORD m_PoolRefCount;

	CYHHashTable<CDelay> m_DelayedTable;

	DWORD MakeKey(WORD Kind,WORD Idx);

	// Ÿ��ź ���� ���� ���� ���� - tamoo : 07.10.30
	CDelay* m_pTitanPotionDelay;
	CDelay* m_pYoungYakPotionDelay[8];
public:
	enum eDelayKind
	{
		eDK_Skill = 1,
		eDK_Item = 2,
	};

	CDelayGroup();
	virtual ~CDelayGroup();

	void Init();
	void Release();

	void AddDelay(WORD Kind,WORD Idx,DWORD DelayTime,DWORD ElapsedTime=0);

	// ���� ���� �ð��� 0~1�� �����Ѵ�. 0 �����ϸ� ���� ���ٴ� ��
	float CheckDelay(WORD Kind,WORD Idx,DWORD* pRemainTime = NULL );

	// Ÿ��ź ���� ���� ���� - - tamoo : 07.10.30
	void AddTitanPotionDelay( DWORD DelayTime,DWORD ElapsedTime=0 );
	float CheckTitanPotionDelay( DWORD* pRemainTime = NULL );

	void AddYoungYakPotionDelay(DWORD DelayTime, WORD Type, DWORD ElapsedTime = 0);
	float CheckYoungYakPotionDelay(WORD TYPE, DWORD* pRemainTime = NULL);
};

#endif // !defined(AFX_DELAYGROUP_H__9E742F39_D6EB_4FA4_BB74_50B937A618A0__INCLUDED_)
