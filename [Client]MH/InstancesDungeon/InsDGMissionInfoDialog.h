#ifndef _INSDG_MISSIONINFO_DLG_H
#define _INSDG_MISSIONINFO_DLG_H
#pragma once
#include "./Interface/cDialog.h"
class cListDialog;
class CInsDGMissionString;
class CInsDGMissionInfoDialog : public cDialog
{
public:
	CInsDGMissionInfoDialog();
	~CInsDGMissionInfoDialog();
	void Linking();
	virtual void SetActive( BOOL val );
	virtual DWORD ActionEvent(CMouse * mouseInfo);
	void SetMissionString();
protected:	
	cListDialog* m_pMissionInfoList;	
};
#endif 