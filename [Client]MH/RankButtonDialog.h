#pragma once
#include "./interface/cDialog.h"
#define RANKBTNDLG GAMEIN->GetRankButtonDialog()
enum {HRANK=0,GRANK=1,};
class CRankButtonDialog:public cDialog
{
	cDialog * pDlg;
	cButton * pHeroBtn;
	cButton * pMunpBtn;
	cStatic * pMapInfo;
	cStatic * NotifyIcon[2];

	cImage m_TooltipImage;
	cImage m_ScrollImg[3];


	//preserved for map info

	float MapExp;
	float MapSkillPoint;
	float MapGoldRate;
	float MapRegenRate;
	float MapDropRate;

	int m_CurScrollImage[2];

	BOOL  m_bFlicker[2];
	BOOL  m_bFlActive[2];
	DWORD m_bCounter[2];
	DWORD m_bSwapTime[2];

	VECTOR2 mScale ;
	DWORD dwColor ;

	//end
public:

	CRankButtonDialog();    // the bar control class 2014-05-23

    virtual  ~CRankButtonDialog();

	void Linking();

	void CurrentChannelRateMsg(MSG_CHANNEL_MSGJACK* pInfo);
	void OnActionEvent(LONG lId, void * p, DWORD we);
	virtual DWORD ActionEvent( CMouse* mouseInfo );
	virtual void Render();

	void SetFlicker(BOOL bFlicker , BYTE bFlag);
	void HRankRendering();
	void GRankRendering();



	void SetShow(bool p);
};