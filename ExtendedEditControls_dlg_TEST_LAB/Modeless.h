#pragma once


// CModeless dialog

class CModeless : public CDialog
{
	DECLARE_DYNAMIC(CModeless)

public:
	CModeless(CWnd* pParent = NULL);   // standard constructor
	virtual ~CModeless();

// Dialog Data
	enum { IDD = IDD_DIALOG1 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
protected:
	virtual void PostNcDestroy();
public:
	CString m_text;
	CWnd* m_pParent;
};
