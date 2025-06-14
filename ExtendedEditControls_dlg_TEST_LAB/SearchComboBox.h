#pragma once
#include "ComboBoxBrowseCtrl.h"

typedef CComboBoxBrowseCtrl	CSearchComboBoxBase;

class QUICKFIND_EXT_CLASS CSearchComboBox : public CSearchComboBoxBase
{
	//DECLARE_DYNAMIC(CSearchComboBox)
public:
	CSearchComboBox(CWnd* pParent = NULL);
	~CSearchComboBox();
public:
	CWnd* m_pParent;
	BOOL	m_bSearchOK;
	void SetNewOwner(CWnd* pOwner){ m_pParent = pOwner; }
	void OnAfterClearText();
// Overrides
protected:
	void Init() override;

	std::unique_ptr<CComboBoxBrowseCtrlEdit> CreateEditControl() override;
protected:
	CBrush	m_brErr;
protected:
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);

	DECLARE_MESSAGE_MAP()
};

