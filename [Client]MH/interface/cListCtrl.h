#ifndef _cLISTCTRL_H_
#define _cLISTCTRL_H_
#if _MSC_VER > 1000
#pragma once
#endif 
#include "cWindow.h"
#include "cReportItem.h"
#include "cRitemEx.h"
#define   MAX_HEADER_IMG_ROWS  10
class cListCtrl : public cWindow, public cReportItem
{
public:
	cListCtrl();
	virtual ~cListCtrl();
	virtual void Init(LONG x, LONG y, WORD wid, WORD hei, cImage * basicImage, LONG ID);
	virtual void InitListCtrl(WORD wMaxColumns, WORD wLinePerPage);
	virtual void InitListCtrlImage(cImage * HeadImage, BYTE HHeight, cImage * BodyImage, BYTE BHeight, cImage * OverImage );
	virtual void Render();
	virtual DWORD ActionEvent(CMouse * mouseInfo);
	void SetMargin(const RECT * textRelRect);
	WORD PtIdxInRow(LONG x, LONG y);
	int GetSelectedRowIdx(){ return m_SelectedRowIdx; }
	void SetSelectedRowIdx( int Idx )		{	m_SelectedRowIdx = Idx;	}
	void SetOverImageScale( float x, float y ) { m_OverImageScale.x = x; m_OverImageScale.y = y; }
	void SetSelectOption( WORD wOption ) { m_wSelectOption = wOption; }
	void SetAlign( int nAlign )				{ m_nAlign = nAlign; }
	void SetRenderImage(BOOL Val){IsRenderImage=Val;}
	void SetRenderColumnHeader(BOOL Val){IsRenderColumnHeader=Val;}
	void SetRenderTooltip(BOOL Val){IsRenderTooltip=Val;}
protected:
	cImage m_HeadImage;
	cImage m_BodyImage;
	cImage m_OverImage;
	cImageScale		m_OverImageScale;
	BYTE m_bHeadLineHeight;
	BYTE m_bBodyLineHeight;
	WORD m_wLinePerPage;
	RECT m_textRelRect;				
	int m_SelectedRowIdx;					
	LONG m_lTopListItemIdx;
	int	m_nOverIdx;
	float m_fGap;
	WORD	m_wSelectOption;	
	int	    m_nAlign;
	BOOL  IsRenderImage;
	BOOL  IsRenderColumnHeader;
	BOOL  IsRenderTooltip;
};
#endif 