#pragma once
#include "./interface/cDialog.h"
//#include "./interface/cStatic.h"
#include "./interface/cButton.h"
#define RECONNDLG CReconnectDialog::GetInstance()

class CReconnectDialog : public cDialog
{
	cButton* m_RegNo;
	cStatic* m_StaticText[2];
public:
	CReconnectDialog();
	~CReconnectDialog();
	static CReconnectDialog* GetInstance()	{static CReconnectDialog gInstance;return &gInstance;}
	void Clear();
	void Linking();
	void OnActionEvent(LONG lId, void * p, DWORD we);

	void AddMessage(int nChatFlag, int nResChat, eReconnectDlg = NOBUTTON);
	void AddMessage(int nButton, char* str, ...);

	void DisableSet();

	void ReloadDlgPosition();

	//virtual void SetActive(BOOL bFlag);
};



