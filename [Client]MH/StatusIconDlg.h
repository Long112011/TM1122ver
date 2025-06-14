#pragma once
#include "./Interface/cImage.h"
#include "./Interface/cMultiLineText.h"
class CObject;
class CMouse;
#define STATUSICONDLG USINGTON(CStatusIconDlg)
struct ICONRENDERINFO
{
	DWORD	ItemIndex;

	BOOL	bPlus;

	BOOL	bAlpha;
	int		Alpha;

	
};



class CStatusIconDlg  
{
	int m_MaxDesc;

	DWORD DelItemIdx;
	BOOL  CanDelBuf;

	StaticString* m_pDescriptionArray;

	BOOL m_Check;


	CObject* m_pObject;
	WORD m_IconCount[eStatusIcon_Max];
	cImage m_StatusIcon[eStatusIcon_Max];
	ICONRENDERINFO m_IconInfo[eStatusIcon_Max];
	
	DWORD m_dwRemainTime[eStatusIcon_Max];

	DWORD m_dwStartTime[eStatusIcon_Max];

	VECTOR2 m_DrawPosition;		// �׸� ��ġ
	int m_MaxIconPerLine;		// ���ٿ� ���� �?

	cMultiLineText m_toolTip;
	//cMultiLineText m_toolTip2;
	int m_CurIconNum;
//	LONG m_ttX, m_ttY;

//	BOOL m_bInit;

	int	 m_nQuestIconCount;
	BOOL m_Show;	
	void LoadDescription();

	//BOOL bAppearBtn;
public:

	CStatusIconDlg();
	virtual ~CStatusIconDlg();

	void AddIcon(CObject* pObject,WORD StatusIconNum,WORD ItemIdx=0, DWORD dwRemainTime = 0);
	void AddQuestTimeIcon(CObject* pObject,WORD StatusIconNum);
	void RemoveIcon(CObject* pObject,WORD StatusIconNum, WORD ItemIdx=0);
	void RemoveQuestTimeIcon(CObject* pObject,WORD StatusIconNum);
	void RemoveAllQuestTimeIcon();

	void Render();
	void SetShow(BOOL val){m_Show=val;}
	BOOL IsShow(){return m_Show;}


	void Init(CObject* pObject,VECTOR2* pDrawPosition,int MaxIconPerLine);
	void Release();

	void SetOneMinuteToShopItem(DWORD ItemIdx);


	void AddQuestIconCount()	{ ++m_nQuestIconCount; }

	BOOL CheckStatusIcon(WORD StatusIconNum);     //weiye ���ͼ���Ƿ���ʾ 2017-12-07 ͨ���˽����ڹҼ�ʱ

	//BOOL CheckForButtonAppear() {return bAppearBtn;}

//	void ActionEvent(CMouse * mouseInfo);
//	BOOL IsInited() { return m_bInit; }
//	void SetToolTip( char* msg, DWORD color, cImage * image, DWORD imgColor );

	void SetDelItemIdx(DWORD idx){ DelItemIdx = idx; }
	DWORD GetDelItemIdx(){ return DelItemIdx; }
	void SetCanDelBuf(BOOL val){ CanDelBuf = val; }
};
EXTERNGLOBALTON(CStatusIconDlg)