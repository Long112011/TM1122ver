#ifndef _INSDG_INFO_DLG_H
#define _INSDG_INFO_DLG_H
#pragma once
#include "./Interface/cDialog.h"
class cStatic;
class cButton;
class CInsDGInfoDialog : public cDialog
{
public:
	CInsDGInfoDialog();
	~CInsDGInfoDialog();
	void Linking();
	virtual void SetActive( BOOL val );		
	virtual void Render();
	void OnActionEvent( LONG lId, void * p, DWORD we );
	void SetEndTime(DWORD dwTime);
	void SetProgressTime(DWORD dwTime);
	void SetMissionCount(DWORD dwCount, DWORD dwMaxCount);
protected:
	cStatic*	m_pProgressTimeStatic;
	cStatic*	m_pMissionCountStatic;
	cButton*	m_pMissionBtn;
	DWORD		m_dwEndTime;	
	DWORD		m_dwProgressTime;	
	DWORD		m_dwProcessStartTime;	
};
#endif 