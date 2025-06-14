#include "stdafx.h"
#include "ColorComboBox.h"
#include "Color.h"
CColorComboBox::CColorComboBox()
{
    m_BkBrush.CreateSolidBrush(RGB(0, 255, 0)); 
}

CColorComboBox::~CColorComboBox()
{
}

BEGIN_MESSAGE_MAP(CColorComboBox, CComboBoxEx)
    ON_WM_CTLCOLOR_REFLECT()
    ON_WM_CTLCOLOR()
END_MESSAGE_MAP()

HBRUSH CColorComboBox::CtlColor(CDC* pDC, UINT nCtlColor)
{
    pDC->SetTextColor(RGB(255, 0, 0));
    pDC->SetBkColor(RGB(0, 255, 0));
    return m_BkBrush;
}

HBRUSH CColorComboBox::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
    return CtlColor(pDC, nCtlColor);
}