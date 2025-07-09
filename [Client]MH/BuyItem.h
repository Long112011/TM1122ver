// BuyItem.h: interface for the CBuyItem class.
// 구매용 가상 아이템
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BUYITEM_H__3002E03D_BE93_4267_B015_37BB522D150A__INCLUDED_)
#define AFX_BUYITEM_H__3002E03D_BE93_4267_B015_37BB522D150A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "./Interface/cIcon.h"

class CItem;

class CBuyItem : public cIcon  
{
	WORD		m_ItemIdx;
	WORD		m_nVolume;
	DWORD		m_nMoney;
	ITEMPARAM	m_dwParam;
	BOOL		m_bVolume;
	DWORD       m_bGrow;
	DWORD       m_nGold;

	cImage m_ScrollImg[22];
	int m_CurScrollImage;

	DWORD m_Grade30;
		
public:
	CBuyItem();
	virtual ~CBuyItem();

	void InitItem(ITEMBASE ItemInfo, WORD Volume, DWORD Money,DWORD Gold);
	
	WORD GetVolume() { return m_nVolume; }
	void SetVolume(WORD Volume) { m_nVolume = Volume; }
	
	DWORD GetMoney() { return m_nMoney; }
	void SetMoney(DWORD Money) { m_nMoney = Money; }

	DWORD GetGold() { return m_nGold; }
	void  SetGold(DWORD Gold) { m_nGold = Gold; }

	WORD GetItemIdx() { return m_ItemIdx; }

	virtual DWORD   GetGrow()  {return  m_bGrow;}
    virtual void    SetGrow(DWORD Val)  {m_bGrow=Val;}

	virtual void Render();

	virtual DWORD   GetGrade30() { return  m_Grade30; }
	virtual void    SetGrade30(DWORD Val) { m_Grade30 = Val; }
};

#endif // !defined(AFX_BUYITEM_H__3002E03D_BE93_4267_B015_37BB522D150A__INCLUDED_)
