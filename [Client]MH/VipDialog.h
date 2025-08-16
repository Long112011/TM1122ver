#if !defined(AFX_VIPDLG_H__8C774E0A_9D8C_4535_A3B2_F847517FA424__INCLUDED_)
#define AFX_VIPDLG_H__8C774E0A_9D8C_4535_A3B2_F847517FA424__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "./Interface/cTabDialog.h"
#include "./interface/cIconGridDialog.h"
#include "ObjectGuagen.h"

// VIP系统窗口实现  2018-12-25

class cVipDealItem;
class VipDialog : public cTabDialog
{
	CYHHashTable<VIP_INFO> m_VipInfoList;
	cDialog* pDlg;
	cStatic* pVipImage;
	cStatic* pVipMsg;
	CObjectGuagen* pVipProgress;
	cStatic* pVipUpRate;
	cStatic* pVipMsg2;
	cDialog* pTableDlg[MAX_VIP_LEVEL];
	cIconGridDialog* pVipGrid[MAX_VIP_LEVEL];
	cStatic* pVipMall[MAX_VIP_LEVEL];
	cStatic* pVipGold[MAX_VIP_LEVEL];
	cButton* pVipGetButton[MAX_VIP_LEVEL];

	DWORD               m_TotalGold;
	WORD                 m_VipLevel;
	int                        m_VipValue[MAX_VIP_LEVEL];
public:

	VipDialog();
	virtual ~VipDialog();
	void Linking();
	virtual void Add(cWindow* window);
	BOOL  LoadVipInfo();
	void    SetVipItem();
	void    SetVipInfo(DWORD TotalGold, WORD VipLevel, int* VipValue);
	DWORD GetPreviousMaxGold(WORD VipLevel);
	void   SetButtonStatic(int* VipValue);
	BOOL  OnActionEvent(LONG lId, void* p, DWORD we);
	WORD GetVipLevel() { return m_VipLevel; }
	void SetVipInfoByLevel(WORD VipLevel, int Flag);
};
#endif


