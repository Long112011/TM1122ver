#if !defined(AFX_SAFENUMBERIMG_H__3565EDD2_97D4_4E4F_8535_23A911D95094__INCLUDED_)
#define AFX_SAFENUMBERIMG_H__3565EDD2_97D4_4E4F_8535_23A911D95094__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "./interface/cDialog.h"

class CSafeNumberImg : public cDialog
{
private:

	cDialog * m_Dlg;
	cButton * m_Static_on[3];
	cButton * m_Static_off[3];

	BOOL Drag;
public:

	CSafeNumberImg();

	~CSafeNumberImg();

    void Linking();

	BOOL OnActionEvent(LONG lId, void * p, DWORD we);

	void SetLock();
	
	void SetUnLock();

	virtual DWORD ActionEvent(CMouse * mouseInfo);
	
};

#endif