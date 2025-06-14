class CColorComboBox : public CComboBoxEx
{
public:
    CColorComboBox();
    virtual ~CColorComboBox();

protected:

    CBrush m_BkBrush;

    DECLARE_MESSAGE_MAP()
public:
    afx_msg HBRUSH CtlColor(CDC* pDC, UINT nCtlColor);
    afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
};