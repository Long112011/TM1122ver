#pragma once
#include "resource.h"
class CPreviewEditDlg :public CDialog
{
public:
	CPreviewEditDlg();
	CString	m_sExt, m_sInfo;
protected:		
	virtual BOOL OnInitDialog();
	virtual void DoDataExchange(CDataExchange* pDX);	
};
