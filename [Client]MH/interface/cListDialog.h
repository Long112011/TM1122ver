#ifndef _cLISTDIALOG_H_
#define _cLISTDIALOG_H_
#if _MSC_VER > 1000
#pragma once
#endif 
#include "cDialog.h"
#include "cListItem.h"
#include <list>
enum {LD_TOP, LD_MIDDLE, LD_DOWN};
class cListDialog : public cDialog, public cListItem
{
public:
	cListDialog();
	virtual ~cListDialog();
	virtual void Init(LONG x, LONG y, WORD wid, cImage * topImage, WORD topHei, cImage * middleImage, WORD middleHei, cImage * downImage, WORD downHei, cImage * overImage, BYTE num, LONG ID);
	virtual void InitList(WORD wMaxLine, RECT * textClippingRect);
	virtual DWORD ActionEvent(CMouse * mouseInfo);
	virtual void Add(cWindow * btn);
	virtual void Render();

	char     toolTipMsg[256];
	cMultiLineText m_toolTip;
	void AddToolTipLine3(char* msg, DWORD color = TTTC_DEFAULT, cImage * m_LineImage = NULL, WORD m_FontIdx = 0);

	//virtual void RemoveAll();
	//virtual void RemoveItem(const char*);
	//virtual void RemoveItem(DWORD index);
	//typedef std::list< std::string >	ToolTipTextList;
	//void AddItem(const char* text, DWORD color, const cMultiLineText&, int line = -1, DWORD dwData = 0, WORD wImageIdx = 0);


	BOOL IsMaxLineOver();
	BOOL Resize(BYTE middleNum);
	WORD GetHeight(BYTE i)
	{
		if(i == LD_TOP) return m_topHeight;
		if(i == LD_MIDDLE) return m_middleHeight;
		if(i == LD_DOWN) return m_downHeight;
		return 0;
	}
	BYTE GetMiddleImageNum(){ return m_middleNum; }
	void SetMinMaxMiddleNum(BYTE min, BYTE max) { m_minMiddleNum = min; m_maxMiddleNum = max; }
	void SetOverImageScale(float x, float y) { m_OverImageScale.x = x; m_OverImageScale.y = y; }
	void OnUpwardItem();
	void OnDownwardItem();
	int PtIdxInRow(LONG x, LONG y);

	virtual void AddItem(const char * str, DWORD color, int line = -1, BOOL Isglisten = FALSE, DWORD Value = 0,int GeneratedId=-1);
	virtual void AddItemRecall(const char * str, DWORD color, int line = -1, BOOL Recall = FALSE);

	void onSize(BOOL fSize);
	void ListMouseCheck( CMouse * mouseInfo );
	int GetCurSelectedRowIdx();
	void SetCurSelectedRowIdx(int Idx);
	void ResetGuageBarPos();
	void SetLineHeight( LONG height );
	void SetAutoScroll( BOOL bAuto ) { m_bAutoScroll = bAuto; }
	int GetSelectRowIdx()		{ return m_SelectedRowIdx;	}
	int GetTopListItemIdx()		{ return m_lTopListItemIdx;	}
	void SetTopListItemIdx(int Idx);	
	cPtrList* GetListItem()		{ return &m_ListItem; }
	BOOL PtInWindow(LONG x, LONG y);
	void SetShowSelect(BOOL val);
	ITEM*	GetSelectedITEM( CMouse * mouseInfo );
	void RenderFaceImage(LONG X,LONG Y,char * StaticText); 
	void SetRenderFaceImg(BOOL val);  
	void ClearFaceImg(char * StaticText,int StrLen);
	ITEM*	GetSelectedITEM( int IdxRow);   
	void SetFGColor(DWORD col)			{ m_fgColor = col;	}
	void SetFlickerRoar(BOOL bFlicker);
	void FlickerRoar();

	void CheckScrollbar();
protected:
	struct ToolTip
	{
		int nPos;
		cIcon*		mIcon;
	};
	typedef std::list< ToolTip >	ToolTipList;
	ToolTipList						mToolTipList;
	cIcon*	mToolTipIcon;


	LONG m_lLineHeight;
	LONG m_lLineNum;
	LONG m_lTopListItemIdx;
	cImage m_TopImage;		WORD m_topHeight;
	cImage m_MiddleImage;	WORD m_middleHeight;
	cImage m_DownImage;		WORD m_downHeight;
	BYTE m_middleNum;
	BYTE m_minMiddleNum;
	BYTE m_maxMiddleNum;
	RECT m_textRelRect;						
	cButton * m_pUpwardBtn;
	cButton * m_pDownwardBtn;
	cButton * m_pLengthLongBtn;
	cButton * m_pLengthShortBtn;
	cGuageBar * m_pGuageBar;
	int		m_SelectedRowIdx;
	int		m_ClickSelected;
	cImage			m_OverImage;
	cImageScale		m_OverImageScale;
	float			m_fGap;
	BOOL			m_bAutoScroll;
	BOOL			m_bShowSelect;
    BOOL            m_bFaceImage;         
	DWORD           m_glistenCount;
	DWORD           m_glisttenTime;
	cImage		    m_ItemLink;
	DWORD			m_fgColor;
	DWORD			m_fgColor2;
	cImage			pNameImage[20];
	DWORD           m_glistenCount2;
	DWORD           m_glisttenTime2;
	BOOL			m_bFlicker2;
	BOOL			m_bFlActive2;
	DWORD			m_dwFlickerSwapTime2;
	DWORD			count2;
};
#endif 