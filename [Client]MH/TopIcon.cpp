#include "Stdafx.h"
#include "TopIcon.h"
#include "TopDialog.h"
#include "TopMunpDialog.h"
#include "WindowIDEnum.h"
#include "GameIn.h"
#include "LuckerDialog.h"
//#include "SafeNumberDialog.h"
#include "cWindowManager.h"
#include "FameGoldDialog.h"
#include "./Interface/cWindowManager.h" 

CTopIcon::CTopIcon()
{
	pDlg=NULL;
	m_ShowBtn=NULL;
	m_HideBtn=NULL;
	m_IsHide=FALSE;
	m_Herolb=NULL;
	m_MunpLb=NULL;

	m_BossNameTemp=NULL;
}

CTopIcon::~CTopIcon()
{
	pDlg=NULL;
	
	m_ShowBtn=NULL;
	m_HideBtn=NULL;
	m_IsHide=FALSE;
	m_Herolb=NULL;
	m_MunpLb=NULL;

	m_BossNameTemp=NULL;
}

void CTopIcon::Linking()
{
    pDlg = WINDOWMGR->GetWindowForID( TOP_NUMBER_ICON );
   
    m_ShowBtn=(cButton*)GetWindowForID(TOP_SHOW_BTN);
	m_HideBtn=(cButton*)GetWindowForID(TOP_HIDE_BTN);
	m_Herolb =(cStatic *)GetWindowForID(TOP_HERO_LB);
	m_MunpLb =(cStatic *)GetWindowForID(TOP_MUNP_LB);


	m_BossNameTemp =(cStatic *)GetWindowForID(TOP_SAFE_ON);

}
void CTopIcon::SetShow(bool p)
{
	cDialog::SetActive(p);
}
/*void CTopIcon::SetShow()
{
	m_ShowBtn->SetActive(TRUE);
	m_MunpLb->SetActive(TRUE);
	m_HideBtn->SetActive(FALSE);
	m_IsHide=FALSE;
	m_Herolb->SetActive(TRUE);
}
void CTopIcon::SetHide()
{
	m_MunpLb->SetActive(FALSE);
	m_ShowBtn->SetActive(FALSE);
	m_HideBtn->SetActive(TRUE);
	m_IsHide=TRUE;
	m_Herolb->SetActive(FALSE);
}*/
/*void CTopIcon::CheckShowOrHide()
{
	if(m_IsHide)
	{
		SetHide();
	}
	else
	{
		SetShow();
	}
}*/

/*BOOL CTopIcon::IsShow()
{
	return m_IsHide;
}*/

void CTopIcon::OnActionEvent(LONG lId, void * p, DWORD we)
{
	if(we & WE_BTNCLICK)
	{
		/*if(lId==TOP_SHOW_BTN)
		{
			SetHide();
		}
		if(lId==TOP_HIDE_BTN)
		{
			SetShow();
		}*/
	}
}
void CTopIcon::SetHeroFirst(char * pName)
{
	m_Herolb->SetStaticText(pName);
}
void CTopIcon::SetMunpFirst(char * pName)
{
	m_MunpLb->SetStaticText(pName);
}
void CTopIcon::SetBossNameTemp(char * pName)
{
	m_BossNameTemp->SetStaticText(pName);
}