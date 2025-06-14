// Modeless.cpp : implementation file
//

#include "stdafx.h"
#include "main.h"
#include "Modeless.h"
#include "dialog.h"


// CModeless dialog

IMPLEMENT_DYNAMIC(CModeless, CDialog)
CModeless::CModeless(CWnd* pParent /*=NULL*/)
	: CDialog(CModeless::IDD, pParent)
	, m_text(_T(""))
{
	m_pParent = pParent;
}

CModeless::~CModeless()
{
}

void CModeless::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT15, m_text);
}


BEGIN_MESSAGE_MAP(CModeless, CDialog)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, OnBnClickedCancel)
END_MESSAGE_MAP()


// CModeless message handlers

void CModeless::OnBnClickedOk()
{
	if(UpdateData(true))
    {
		((theDlg*)m_pParent)->m_text = m_text;
		((theDlg*)m_pParent)->UpdateData(false);
        DestroyWindow();
	}
}

void CModeless::OnBnClickedCancel()
{
	DestroyWindow();	
}

void CModeless::PostNcDestroy()
{
	CDialog::PostNcDestroy();
	if(m_pParent)
	{		
		((theDlg*)m_pParent)->m_pmodeless = NULL;
	}
	delete this;
}
