#pragma once
#include "resource.h"
#include "OXEdit.h"
#include "ScintillaWnd.h"
class CPreviewEditDlg :public CDialog
{
public:
	CPreviewEditDlg();
	CString	m_sExt;
	COXEdit	m_Edit;
protected:		
	//CScintillaWnd       m_ScinCtrl;
	virtual BOOL OnInitDialog();
	virtual void DoDataExchange(CDataExchange* pDX);	
	virtual void goDark();
};
