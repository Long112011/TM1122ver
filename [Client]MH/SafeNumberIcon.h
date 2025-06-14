#pragma once

#include "./interface/cDialog.h"
#include "./interface/cWindowHeader.h"
enum {eBtnLock=0, eBtnReset=1, eBtnChange=2,eBtnMax=3,};
class CSafeNumberIcon : public cDialog
{

	cDialog * m_Dlg;
	//cButton * m_Btn_Lock;
	//cButton * m_Btn_Reset;
	//cButton * m_Btn_Change;

	cButton * m_Btn_Icon[eBtnMax];

	cButton * m_Btn_Confirm;

	cEditBox * m_EditBox1;
	cEditBox * m_EditBox2;

	cStatic * m_StaticTitle;
	cStatic * m_Static;
	//cStatic * m_Static_on[3];
	//cStatic * m_Static_off[3];

	BYTE	bDialogState;

public :

	CSafeNumberIcon();

    virtual	~CSafeNumberIcon();

	void  SetActiveStatic(BOOL Val);

	void Linking();

	BOOL OnActionEvent(LONG lId, void * p, DWORD we);

	void SetSafeIconShow();

	void SetLock();

	void SetUnLock();

	void SetReset();

	BOOL  GetSafeStatic();

	void LockSafeCode();  //weiye 发送消息锁定安全锁  2018-04-08

	void ButtonLong(WORD bFlag);
	void Button(WORD bFlag);



	void ClearPwdStr();
	void ClearAll();


	//-------------for state 0
	void CheckSafeCode();

	//-------------for state 1
	void SetPwdMsg();

	//-------------for state 2
	void ChangePwdMsg();


	void Disable();

};