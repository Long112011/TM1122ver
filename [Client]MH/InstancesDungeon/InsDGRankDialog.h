#ifndef _INSDG_RANK_DLG_H
#define _INSDG_RANK_DLG_H
#pragma once
#include "./Interface/cDialog.h"
class cStatic;
class CInsDGRankDialog : public cDialog
{
public:
	CInsDGRankDialog();
	~CInsDGRankDialog();
	void Linking();	
	void OnActionEvent( LONG lId, void * p, DWORD we );	
	void SetRankInfo(MSG_INSDG_RANK_INFO* pRankInfo);
	void GetInsDGRankImage(DWORD dwRankValue, cImage* pImage);
protected:
	cStatic* m_pRankResultImage;
	cStatic* m_pParty;
	cStatic* m_pClearTime;
	cStatic* m_pHunt;
	cStatic* m_pExp;
	cStatic* m_pAbillity;
	cStatic* m_pMoney;
	cStatic* m_pItem;	
};
#endif 