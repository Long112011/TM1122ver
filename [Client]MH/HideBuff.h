#pragma once
#include "./Interface/cDialog.h"
#define HIDEBUFFDLG USINGTON(CHideBuff)
class CHideBuff : public cDialog
{
	cDialog* pDlg;
	BOOL m_Show;
public:

	CHideBuff();

	virtual ~CHideBuff();
    void Linking();
	void OnActionEvent(LONG lId, void * p, DWORD we);

	//void HideButton(bool val);
	//void SetActive( BOOL val );
	//BOOL CanActive();

	void Render();
	void SetShow(BOOL val){ m_Show = val; }
	BOOL IsShow(){ return m_Show; }
};
EXTERNGLOBALTON(CHideBuff)