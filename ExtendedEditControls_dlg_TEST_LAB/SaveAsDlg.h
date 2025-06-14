#pragma once
#include "resource.h"
class CSaveAsDlg :public CDialog
{
public:
	CSaveAsDlg();
	CString	m_sExt, m_sInfo;
protected:		
	virtual BOOL OnInitDialog();
	virtual void DoDataExchange(CDataExchange* pDX);	
};
