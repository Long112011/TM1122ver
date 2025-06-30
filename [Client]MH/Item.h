



// Item.h: interface for the CItem class.aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ITEM_H__68113CBB_0A76_42D7_90B6_C64FBAF3E85C__INCLUDED_)
#define AFX_ITEM_H__68113CBB_0A76_42D7_90B6_C64FBAF3E85C__INCLUDED_


#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "./BaseItem.h"
#include "GameResourceManager.h"


class CExchangeItem;

class CItem : public CBaseItem
{
protected:

	ITEMBASE m_ItemBaseInfo;
	
	POSTYPE			m_LinkPos;	//KES 030919 鑳屽埜鑺?�?鐣寸棦鎯?妤锋惉鍥版懝

	CExchangeItem*	m_pLinkItem;

	DWORD			m_Param;	

	cImage			m_DelayImage;

    cImage          m_LockImageEx;
	cImage          m_LockHoldImageEx;

	cImage			m_JackIcon;

	DWORD			m_fgColor;

	DWORD			m_fgColor2;

	cImage m_ScrollImg[22];
	int m_CurScrollImage;

public:
	BOOL m_bAdded;

	CItem(ITEMBASE* pInfo);
	virtual ~CItem();

	virtual inline DWORD	GetDBIdx()					{	return m_ItemBaseInfo.dwDBIdx;	}
	virtual inline WORD		GetItemIdx()				{	return m_ItemBaseInfo.wIconIdx;	}
	virtual inline POSTYPE	GetPosition()				{	return m_ItemBaseInfo.Position;	}
	virtual inline void		SetPosition(POSTYPE pos)	{	m_ItemBaseInfo.Position = pos;	}


	virtual inline POSTYPE	GetQuickPosition()				{	return m_ItemBaseInfo.QuickPosition;	}
	virtual inline void		SetQuickPosition(POSTYPE pos)	{	m_ItemBaseInfo.QuickPosition = pos;		}

	virtual inline DWORD	GetRareness()					{   return m_ItemBaseInfo.RareIdx; }
	virtual inline DURTYPE	GetDurability()					{	return m_ItemBaseInfo.Durability;	}
	virtual inline void		SetDurability(DURTYPE dur)		{	m_ItemBaseInfo.Durability = dur;		}

	virtual inline DWORD    GetStoneIdx()                   {   return m_ItemBaseInfo.StoneIdx;}   // 鑾峰彇缂栧彿 2014-11-14
	virtual inline void     SetStoneIdx(DWORD dwStoneIdx)	{   m_ItemBaseInfo.StoneIdx=dwStoneIdx;} // 缂栧�?2014-11-15

//KES 030919 鑳屽埜鑺?�?鐣寸棦鎯?妤锋惉鍥版懝	
	inline POSTYPE GetLinkPosition() { return m_LinkPos; }
	inline void SetLinkPosition(POSTYPE pos)	{ m_LinkPos = pos; }
	void SetLinkItem( CExchangeItem* pItem )	{ m_pLinkItem = pItem; }
	CExchangeItem* GetLinkItem()				{ return m_pLinkItem; }

	const ITEMBASE*	GetItemBaseInfo() { return &m_ItemBaseInfo; }
	void SetItemBaseInfo( ITEMBASE * itemInfo );
	
	virtual void SetLock( BOOL bLock );
	virtual void SetLockHold(BOOL bLockHold);

	eITEM_KINDBIT GetItemKind();

	
	const ITEM_INFO* GetItemInfo();

	

	virtual void Render();
	
	virtual BOOL CanMoveToDialog( cDialog* ptargetdlg );
	virtual BOOL CanDelete();
	

	void SetItemParam(ITEMPARAM Param)		{ m_ItemBaseInfo.ItemParam = Param;	}

	DWORD GetItemParam()					{ return m_ItemBaseInfo.ItemParam;	}
	void SetUseParam( DWORD Param )			{ m_Param = Param;	}
	DWORD GetUseParam()						{ return m_Param;	}

	// 2014-12-04 鑾峰�?

	virtual inline	WORD	GetStatic()				{	return m_ItemBaseInfo.ItemStatic;	}
	virtual inline void		SetStatic(WORD Val)	{	m_ItemBaseInfo.ItemStatic =Val;	}

	// 2015-01-14 鑾峰�?
	virtual inline DWORD    GetGrow()  {return   m_ItemBaseInfo.ItemGrow;}
	virtual inline void     SetGrow(DWORD Val)  { m_ItemBaseInfo.ItemGrow=Val;}

	virtual inline char* GetPowerUp(  ) {return   m_ItemBaseInfo.PowerUp;}
	virtual inline char* GetGreen(  ) {return   m_ItemBaseInfo.Green;}

	virtual inline DWORD GetGradeAlexX() { return m_ItemBaseInfo.ItemGradeAlexX; }
	virtual inline void SetGradeAlexX(DWORD Val) { m_ItemBaseInfo.ItemGradeAlexX = Val; }
	virtual inline DWORD GetGradeAlexXWTF() { return m_ItemBaseInfo.ItemGradeAlexX + 1; }

};

#endif // !defined(AFX_ITEM_H__68113CBB_0A76_42D7_90B6_C64FBAF3E85C__INCLUDED_)


