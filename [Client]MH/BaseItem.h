






// BaseItem.h: interface for the CBaseItem class.
//
//////////////////////////////////////////////////////////////////////


#if !defined(AFX_BASEITEM_H__DBA8F3D4_D724_47CC_AB85_FB6382D443FD__INCLUDED_)
#define AFX_BASEITEM_H__DBA8F3D4_D724_47CC_AB85_FB6382D443FD__INCLUDED_


#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "INTERFACE\cIcon.h"

class CBaseItem : public cIcon  
{
public:
	CBaseItem();

	virtual ~CBaseItem();

	virtual inline DWORD	GetDBIdx(){ return 0; }
	virtual inline WORD		GetItemIdx(){ return 0; }
	virtual inline POSTYPE	GetPosition(){ return 0; }
	virtual inline void		SetPosition(POSTYPE pos){}



	virtual inline POSTYPE	GetQuickPosition()				{	return 0;	}
	virtual inline void		SetQuickPosition(POSTYPE pos)	{}
	
	virtual inline DWORD	GetRareness()					{   return 0;	}
	virtual inline DURTYPE	GetDurability()					{	return 0;	}
	virtual inline void		SetDurability(DURTYPE dur)		{}

	virtual inline DWORD	GetStoneIdx()					{   return 0;	}  // 2014-11-15 ∏Ω 



	
	virtual inline void		SetExpPoint(DWORD point){}
	virtual inline DWORD	GetExpPoint()			{	return 0;	}
	virtual inline void		SetSung(BYTE sung)		{}
	virtual inline BYTE		GetSung()				{	return 0;	}

	// 2015-01-14 ªÒ»°
	virtual inline DWORD    GetGrow()  {return  0;}
	virtual inline void     SetGrow(DWORD Val)  {}

	virtual inline void	SetGrade(DWORD Val) {}
	virtual inline DWORD	GetGrade() { return 0; }

	virtual inline WORD     GetQuality() { return 0; }
	virtual inline WORD     GetEntry1() { return 0; }
	virtual inline WORD     GetEntry2() { return 0; }
	virtual inline WORD     GetEntry3() { return 0; }

};

#endif // !defined(AFX_BASEITEM_H__DBA8F3D4_D724_47CC_AB85_FB6382D443FD__INCLUDED_)


