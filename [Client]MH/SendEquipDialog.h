#if !defined(AFX_SENDEQUIPDLG_H__3565EDD2_97D4_4E4F_8535_23A911D95094__INCLUDED_)
#define AFX_SENDEQUIPDLG_H__3565EDD2_97D4_4E4F_8535_23A911D95094__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "./interface/cDialog.h"

class  CSendEquipDialog : public cDialog
{
	cDialog * pDlg;

	cButton * pItemName;

public:

	CSendEquipDialog();

	virtual ~CSendEquipDialog();

	void Linking();

	void SetItemName(DWORD ItemIdx);

	void  OnActionEvent(LONG lId, void * p, DWORD we);

	void SendEquipMsg(DWORD ItemDBIdx);
};

#endif