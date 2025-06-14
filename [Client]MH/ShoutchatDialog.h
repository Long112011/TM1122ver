
#pragma once
#include "./Interface/cDialog.h"
class cListDialog;
class CShoutchatDialog : public cDialog
{
	cDialog	* pDlg;
	cListDialog*	m_pMsgListDlg;
	cListDialog*	m_pMsgShout;
	cStatic*		m_pMsgStatic;
	DWORD			m_LastMsgTime;
	BOOL			m_bFlicker;
	BOOL			m_bFlActive;
	DWORD			m_dwFlickerSwapTime;
	DWORD           m_glistenCount2;
	cImage			iConOne;
	cImage			iConTwo;
	BOOL			m_ChatActive;

	DWORD			m_SelectedItemIdx;
public:
	CShoutchatDialog();
	virtual ~CShoutchatDialog();
	virtual DWORD ActionEvent(CMouse * mouseInfo);
	void Linking();
	void Process();
	void SetActive(BOOL val);
	void SetActiveChat(BOOL val);
	void AddMsg(char* str, BOOL Isglisten = FALSE);
	cPtrList * GetShoutListChat();
	cListDialog * GetShoutListChatAdd();
	void RefreshPosition();
	void SetFlicker(BOOL bFlicker);
	void FlickerMussangGuage();
	virtual void Render();
	void AddMsgTest(char* str, BOOL Isglisten = FALSE);

	BOOL CanActive();
};