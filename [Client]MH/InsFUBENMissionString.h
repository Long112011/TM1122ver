#pragma once
#include "./Interface/cDialog.h"
class cListDialog;
class CInsFUBENMissionString : public cDialog
{
private:
	cListDialog*	pFbList;
	int				m_nCurSheetNum;
	int				m_GroupIdx;
public:
	CInsFUBENMissionString();
	virtual 		~CInsFUBENMissionString();
	virtual 		DWORD ActionEvent( CMouse * mouseInfo );
	void 			Linking();
    void			SetActive(BOOL bFlag);
	cListDialog* 	GetSheet( int nSheet ) { return pFbList; }
	int				GetCurSheetNum()				{ return m_nCurSheetNum; }
	BYTE 			GetLineNum();
	cListDialog * 	GetFBInfoListAdd();

	void 			SetInfoMsg(DWORD dwFlag,DWORD dwSub=0);
	void			SetGroupMsg(DWORD dwFlag){m_GroupIdx=1;m_GroupIdx=dwFlag;}
	int				GetGroupMsgIdx(){return m_GroupIdx;}

};