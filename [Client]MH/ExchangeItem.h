




#ifndef _EXCHANGEITEM_H
#define _EXCHANGEITEM_H



#include "./interface/cIcon.h"


class CItem;

class CExchangeItem : public cIcon
{


protected:

	POSTYPE			m_Position;
	POSTYPE			m_LinkPos;
	CItem*			m_pLinkItem;

	cImage*			m_pLinkImage;
	cImage          m_LockImageEx;
	cImage          m_LockHoldImageEx;
	

	DURTYPE			m_Durability; // LBS 03.09.22 °¹¼ö°¡ ÀÖ´Â ¾ÆÀÌÅÛ¶§¹®¿¡...
	WORD			m_ItemIdx;
	DWORD			m_dwRareIdx;
	DWORD			m_dwStoneIdx;  // ¸½ 2014-11-15
	DWORD			m_dwDBId;
	DWORD			m_dwParam;
    DWORD           m_Grow;        // 2015-01-17  
	WORD			m_Static;
	eITEM_KINDBIT	m_Kind;

	cImage m_ScrollImg[22];
	int m_CurScrollImage;
	DWORD           m_GradeAlexX;
public:

	CExchangeItem();
	~CExchangeItem();

	void	InitItem( CItem* pItem, POSTYPE Pos, LONG id );
	 
	void	SetPosition( POSTYPE Pos )				{ m_Position = Pos; }
	POSTYPE GetPosition()							{ return m_Position; }

	void	SetLinkPos( POSTYPE LinkPos )			{ m_LinkPos = LinkPos; }
	POSTYPE GetLinkPos()							{ return m_LinkPos; }


	void	SetLinkItem( CItem* pItem )				{ m_pLinkItem = pItem; }

	CItem*	GetLinkItem()							{ return m_pLinkItem; }
	virtual void SetLock( BOOL bLock );

	virtual void SetLockHold(BOOL bLockHold);

	//SW050920 Rare
	inline DWORD	GetRareness()					{ return m_dwRareIdx; }
	inline void		SetRareness( DWORD RareIdx)		{ m_dwRareIdx = RareIdx; }

	inline DURTYPE	GetDurability()					{ return m_Durability; }
	inline void		SetDurability( DURTYPE dura )	{ m_Durability = dura; }

	inline DWORD   GetStoneIdx()                    {return m_dwStoneIdx;}   // 2014-11-15 ¸½
	inline void    SetStoneIdx(DWORD StoneIdx)		{m_dwStoneIdx=StoneIdx;} // 2014-11-15 ¸½

    inline DWORD    GetGrow()  {return  m_Grow;}           	// 2015-01-14 »ñÈ¡
    inline void     SetGrow(DWORD Val)  {m_Grow=Val;}
	
	void SetDBId( DWORD id )						{ m_dwDBId = id; }
	DWORD GetDBId()									{ return m_dwDBId; }

	char* GetItemName();

	virtual void Render();

	void SetItemParam(ITEMPARAM Param) { m_dwParam = Param; }
	ITEMPARAM GetItemParam()			{ return m_dwParam; }


	//
	inline DWORD    GetGradeAlexX() { return  m_GradeAlexX; }
	inline void     SetGradeAlexX(DWORD Val) { m_GradeAlexX = Val; }
};



#endif


