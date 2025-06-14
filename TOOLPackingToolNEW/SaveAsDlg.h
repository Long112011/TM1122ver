
#pragma once
class CSaveAsDlg : public CDialog
{
public:
	CSaveAsDlg(CWnd* pParent = NULL);   
	
	enum { IDD = IDD_DIALOG_EXT };
	CString	m_sExt;
	CString	m_sExtT;
	
	
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    
	
protected:
	
	
	
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
};
