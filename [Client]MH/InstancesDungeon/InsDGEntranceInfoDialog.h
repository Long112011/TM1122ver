#ifndef _INSDG_ENTRANCEINFO_DLG_H
#define _INSDG_ENTRANCEINFO_DLG_H
#pragma once
#include "./Interface/cDialog.h"
class cStatic;
class cListDialog;
class CInsDGEntranceInfoDialog : public cDialog
{
public:
	CInsDGEntranceInfoDialog();
	~CInsDGEntranceInfoDialog();
	void Linking();
	virtual void SetActive( BOOL val );
	void OnActionEvent( LONG lId, void * p, DWORD we );
	void SetNpcUniqueIndex(WORD wNpcUniqueIDX);
	void InsDGEntranceInfo();
	void InsDGEntranceSyn();
protected:	
	cListDialog* m_pInsDGMsgList;	
	cStatic* m_pLevelStatic;		
	cStatic* m_pQuestlStatic;		
	cStatic* m_pItemStatic;			
	cStatic* m_pMoneyStatic;		
	WORD m_wNpcUniqueIDX;
};
#endif 
