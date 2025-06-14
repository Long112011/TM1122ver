




// cListCtrlEx.h: interface for the cListCtrlEx class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CLISTCTRLEX_H__ECF00283_366A_4CA9_90BF_70D8A776A187__INCLUDED_)
#define AFX_CLISTCTRLEX_H__ECF00283_366A_4CA9_90BF_70D8A776A187__INCLUDED_


#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//#include "cListCtrl.h"
#include "./Interface/cDialog.h"
#include "cLinkedList.h"

////////////////////////////////////////////
//cListCtrl¿¡ ½ºÅ©·Ñ°ú ´õºíÅ¬¸¯±â´ÉÀ» Ã·°¡ÇÔ

class cButton;
class cGuageBar;


//ListControlItem;
class cLCItemBase
{
protected:

	char*	m_strText;
	WORD	m_wFontIdx;
	DWORD	m_dwTextColor;

	cImage	m_Image;
	DWORD	m_dwImageColor;

public:


	void SetFont( WORD wFontIdx ) { m_wFontIdx = wFontIdx; }
	void SetTextColor( DWORD dwColor );
	void SetItemText( char* strText );
	void SetImageColor( DWORD dwColor );
	void SetItemImage( cImage* pImage );	
	virtual void SetItemContent( void* pContent ) {}
	virtual void Render( LONG x, LONG y ) {}		//text¸¦ Âï°Å³ª image¸¦ ±×¸°´Ù. ¹ÙÅÁÀº ¾ÈÂï´Â´Ù.
};



//-----------
//
class cLCColumn
{
protected:


	int			m_nLeftMargin;
	int			m_nTopMargin;
	int			m_nAlign;
	int			m_nColumnWidth;

	cLinkedList<cLCItemBase>	m_ListItem;

public:

	cLCColumn();
	virtual ~cLCColumn();

	void AddTail( cLCItemBase* pItem )		{ m_ListItem.AddTail( pItem ); }
	void AddHead( cLCItemBase* pItem )		{ m_ListItem.AddHead( pItem ); }
	cLCItemBase* Remove( int nIndex )		{ return m_ListItem.DeleteAt( nIndex ); }
	void RemoveAll();

	int GetColumnWidth()					{ return m_nColumnWidth; }
	void SetColumnWidth( int nWidth )		{ m_nColumnWidth = nWidth; }

	void Render( LONG x, LONG y, int nStartIndex, int nEndIndex, int nRowHeight );
};



class cListCtrlEx : public cDialog
{


protected:

	cLCColumn*		m_pColumn;
	int				m_nColumnNum;
	int				m_nRowNum;

	int				m_nFirstViewRowIndex;	//Ã³À½ ·»´õ¸µµÇ´Â ¶óÀÎ ---> ½ºÅ©·Ñ¶§¹®¿¡ 
	int				m_nViewRowCount;		//È­¸é¿¡ ÇÑ¹ø¿¡ º¸ÀÌ´Â ¶óÀÎ ¼ö
	int				m_nRowHeight;


	int				m_nLeftMargin;
	int				m_nTopMargin;
	
	cImage			m_ImgTop;
	cImage			m_ImgMiddle;
	cImage			m_ImgBottom;
	cImage			m_ImgMouseOn;
	cImage			m_ImgSelected;

public:

	cListCtrlEx();
	virtual ~cListCtrlEx();

	void InitListCtrlEx( int nColumnNum, int nViewRowCount, int nRowHeight, int nLeftMargin, int nTopMargin );
	void InitListCtrlExImage( cImage* pTop, cImage* pMiddle, cImage* pBottom, cImage* pSelected, cImage* pMouseOn );

	void Release();

	void CreateNewItem( cLCItemBase** ppItem );
	void AddTail( cLCItemBase** ppItem );
	void AddHead( cLCItemBase** ppItem );
	void Remove( int nIndex );

	int	GetCount()	{ return m_nRowNum; }
	
	virtual void Render();
};


#endif // !defined(AFX_CLISTCTRLEX_H__ECF00283_366A_4CA9_90BF_70D8A776A187__INCLUDED_)


