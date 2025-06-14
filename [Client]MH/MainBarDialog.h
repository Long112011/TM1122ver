



// MainBarDialog.h: interface for the CMainBarDialog class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAINBARDIALOG_H__3BE004A2_0D2F_41D5_8985_EE769F6200EA__INCLUDED_)
#define AFX_MAINBARDIALOG_H__3BE004A2_0D2F_41D5_8985_EE769F6200EA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include "./Interface/cDialog.h"
#include "PtrList.h"
#include "IndexGenerator.h"

enum
{
	OPT_MALLNOTICEDLGICON,
	OPT_HELPDLGICON,
	OPT_CHARACTERDLGICON,
	OPT_INVENTORYDLGICON,
	OPT_MUGONGDLGICON,
	OPT_QUESTDLGICON,
	OPT_MINIMAPDLGICON,
	OPT_GUILDDLGICON,
	OPT_FRIENDDLGICON,
	OPT_NOTEDLGICON,
	OPT_OPTIONDLGICON,	
	OPT_EXITDLGICON,
	OPT_PKMODEICON,

	OPT_PRESETFORDEAD,
	
	OPT_MAXDLGICON,
};

class CBarIcon;
class cStatic;
class cButton;



enum eSLIDING
{
	eSLIDING_NONE,
	eSLIDING_RIGHT,
	eSLIDING_LEFT,
};


class CMainBarDialog : public cDialog  
{
public:
	CMainBarDialog();
	virtual ~CMainBarDialog();


	void InitBar();	
	void ReleaseBar();

	// ���콺�� �̿��� ������ �̵�, �и�, ��ŷ
	// ������ ������ ��
	virtual DWORD ActionEvent(CMouse * mouseInfo);
	virtual void SetAbsXY(LONG x, LONG y);
	
	void OnActionEvent(LONG lId, void * p, DWORD we);

	// �⺻ ����� �ִٸ� ������ ������ �°� ���
	// Split Bar��� 
	// 2���� List�� �ִ� ���� ������� ���
	virtual void Render();

	BOOL AddIcon(POSTYPE AtPos, CBarIcon * pIcon);
	BOOL RemoveIcon(POSTYPE AtPos, CBarIcon ** ppIcon);
	BOOL MoveIcon(POSTYPE FromPos, POSTYPE ToPos);
	

	void LoadInfo();
	void SaveInfo();
	CBarIcon * GetBarIcon(WORD kind);
	void SetPushBarIcon( WORD idx, BOOL bPushed );//�ܺο��� ���� ���̻��¸� �ٲܶ�

	void PushMenuID( DWORD windowID, BOOL bPushed );
	void PushMenu( WORD idx );
	void PushDirectMenu(WORD idx);
	// �ɼ��� �����ϰų� �ٽ� ǥ���� �ʿ䰡 ���� ��
	// OptionManager�� ���� �о �ٽ� ǥ��
	void Refresh();

	void RefreshBBPoint();

	void SetDragStart(LONG sX, LONG sY, LONG x, LONG y, CBarIcon * icon)


	{
//		m_OldX=(float)x;m_OldY=(float)y;
//		m_OldDragFlag=TRUE;

//		m_posBeforeDrag.x=(float)sX;
//		m_posBeforeDrag.y=(float)sY;
//		m_pDragIcon = icon;
	}
	BOOL m_OldDragFlag;

	void Fold( BOOL bFold );
	BOOL IsFolded() { return m_bFold; }

	void SetAlram( int idx, BOOL bAlram );

	void RenderNotifyTitle();
	void SetNotifyTitle(char * msg);
	void SetDefaultTitle();
	
protected:

	CBarIcon * GetIcon(WORD idx);
	void RepositionIconAbs(POSTYPE AtPos, CBarIcon * pIcon);
	void RepositionIcon(int offset, CBarIcon * pIcon);

	cPtrList m_MainList;

	int m_nIconNum;
	WORD m_IconBarWidth;				// 15
	WORD m_IconBarHeight;				// 15
	WORD m_IconBarSpace;				// 2

	cImage m_SplitBarImage;
	cImage m_MoveCrossImage;
	cImage m_ImgNgAlram[8];
	CIndexGenerator m_IconIndexCreator;
	

	float m_OldX, m_OldY;
	VECTOR2 m_posBeforeDrag;
	CBarIcon * m_pDragIcon;

	int m_crossStep;
	int m_crossBarHalfX;
	int m_crossBarHalfY;

//	cImage m_IconToolTipImg;

//	cStatic* m_pStcTail;
	cButton* m_pBtnSize;

	BOOL	m_bFold;
	int		m_nSlidingDir;


	LONG	m_lOriX;
	LONG	m_lOriY;
	LONG	m_lSlidingX;


	LONG	m_lOffSetX;


	BOOL	m_bFlicker;
	BOOL	m_bStaticFlicker;
	int		NotifyTitleMsgLoop;



};

#endif // !defined(AFX_MAINBARDIALOG_H__3BE004A2_0D2F_41D5_8985_EE769F6200EA__INCLUDED_)



