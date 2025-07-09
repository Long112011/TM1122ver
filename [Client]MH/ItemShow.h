







// ItemShow.h: interface for the CItemShow class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ITEMSHOW_H__9E79636A_8195_4C5B_9F7E_4E8F2A1A1DA0__INCLUDED_)
#define AFX_ITEMSHOW_H__9E79636A_8195_4C5B_9F7E_4E8F2A1A1DA0__INCLUDED_

#if _MSC_VER > 1000

#pragma once
#endif // _MSC_VER > 1000

#include "BaseItem.h"



class CItemShow  : public CBaseItem

{
protected:

	ITEMBASE m_ItemBaseInfo;

	WORD m_wItemIdx;

	DURTYPE m_Dur;

	cImage m_ScrollImg[22];
	int m_CurScrollImage;
public:

	CItemShow();
	virtual ~CItemShow();
	void Init(WORD wItemIdx, DURTYPE dur, WORD Grade30 = 0, WORD Option = 0, WORD Rare = 0, WORD Grow = 0);
	virtual inline WORD		GetItemIdx(){ return m_wItemIdx; }
	virtual void Render();

	const ITEMBASE*	GetItemBaseInfo() { return &m_ItemBaseInfo; }
	void SetItemBaseInfo( ITEMBASE * itemInfo );

	eITEM_KINDBIT GetItemKind();
};

#endif // !defined(AFX_ITEMSHOW_H__9E79636A_8195_4C5B_9F7E_4E8F2A1A1DA0__INCLUDED_)


