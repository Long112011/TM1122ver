




// VirtualItem.h: interface for the CVirtualItem class.

//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_VIRTUALITEM_H__B62DCBD9_833C_4B73_9B2D_6B4B29C1318F__INCLUDED_)
#define AFX_VIRTUALITEM_H__B62DCBD9_833C_4B73_9B2D_6B4B29C1318F__INCLUDED_


#if _MSC_VER > 1000
#pragma once

#endif // _MSC_VER > 1000

#include "BaseItem.h"

class CVirtualItem : public CBaseItem  
{
	bool IsWTF;
public:
	CVirtualItem();
	virtual ~CVirtualItem();
	void Init();
	WORD GetSrcItemIdx()			{ return m_pItemLink->GetItemIdx();		}
	POSTYPE GetSrcPosition()		{ return m_pItemLink->GetPosition();	}

	eITEM_KINDBIT GetSrcItemKind();

	
	DWORD	GetRareness()			{   return m_pItemLink->GetRareness();	}

	DURTYPE	GetSrcDurability()		{	return m_pItemLink->GetDurability();	}

	DWORD   GetStoneIdx()			{	return m_pItemLink->GetStoneIdx();	}
	// 2015-01-14 »ñÈ¡
	DWORD    GetGrow()				{   return  m_pItemLink->GetGrow();}
	DWORD    GetGrade() { return  m_pItemLink->GetGrade(); }
	void SetIsWTF(bool Val) { IsWTF = Val; }
	//void     SetGrade(DWORD Val) {}
	void SetLinkItem(CBaseItem * pItem);
	CBaseItem * GetLinkItem()
	{
		return m_pItemLink;
	}
	virtual void Render();
protected:
	CBaseItem * m_pItemLink;
	
	cImage m_ScrollImg[22];
	int m_CurScrollImage;


};


#endif // !defined(AFX_VIRTUALITEM_H__B62DCBD9_833C_4B73_9B2D_6B4B29C1318F__INCLUDED_)


