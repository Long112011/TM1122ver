#if !defined(AFX_MINIBAISHIDIALOG_H__3565EDD2_97D4_4E4F_8535_23A911D95094__INCLUDED_)
#define AFX_MINIBAISHIDIALOG_H__3565EDD2_97D4_4E4F_8535_23A911D95094__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "INTERFACE\cDialog.h"
#include ".\interface\cWindowHeader.h"

class CBaiShiDialog :public cDialog
{
private:
	cStatic * m_pName;
	cEditBox * m_pNameEdit;
	cButton * m_pAddOkBtn;
	cButton * m_pAddCancelBtn;
public:
	CBaiShiDialog();
	virtual ~CBaiShiDialog();
	void Linking();
	virtual void SetActive(BOOL val);
	void SetName(char* Name);
};
#endif 