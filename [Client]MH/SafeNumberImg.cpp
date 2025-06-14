#include "StdAfx.h"
#include "SafeNumberImg.h"
//#include "SafeNumberDialog.h"
#include "SafeNumberIcon.h"
#include "WindowIDEnum.h"
#include "./Interface/cWindowManager.h" 
#include "GameIn.h"
#include "../Input/Mouse.h"
CSafeNumberImg::CSafeNumberImg()
{
	m_type=WT_SAFENUM_IMG;

	for(int i=0;i<3;i++)
	{
        m_Static_on[i]=NULL;
		m_Static_off[i]=NULL;
	}

	Drag=false;
}

CSafeNumberImg::~CSafeNumberImg()
{
	m_Dlg=NULL;

	for(int i=0;i<3;i++)
	{
        m_Static_on[i]=NULL;
		m_Static_off[i]=NULL;
	}

	Drag=false;
}

void CSafeNumberImg::Linking()
{
	m_Dlg= WINDOWMGR->GetWindowForID(SAFE_NUMBER_IMG);

	for(int i=0;i<3;i++)
	{
        m_Static_on[i]=(cButton*)GetWindowForID(SAFE_IMG_PACKET_ON1+i);
		m_Static_off[i]=(cButton*)GetWindowForID(SAFE_IMG_PACKET_OFF1+i);
	}
}

void CSafeNumberImg::SetLock()
{
	for(int i=0;i<3;i++)
	{
		m_Static_on[i]->SetActive(FALSE);
		m_Static_off[i]->SetActive(TRUE);
	}
}

void CSafeNumberImg::SetUnLock()
{
	for(int i=0;i<3;i++)
	{
		m_Static_on[i]->SetActive(TRUE);
		m_Static_off[i]->SetActive(FALSE);
	}
}

BOOL CSafeNumberImg::OnActionEvent(LONG lId, void * p, DWORD we)
{
	if(Drag) return false;

	if(we & WE_BTNCLICK)
	{
		if(GAMEIN->GetSafeNumberIcon()->IsActive())
		{
			GAMEIN->GetSafeNumberIcon()->SetActive(FALSE);
			GAMEIN->GetSafeNumberIcon()->SetActiveStatic(FALSE);  //weiye 安全锁窗口状态 2018-04-18
		}
		else
		{
			GAMEIN->GetSafeNumberIcon()->SetActive(TRUE);
			GAMEIN->GetSafeNumberIcon()->SetActiveStatic(TRUE);  //weiye 安全锁窗口状态 2018-04-18
		}
	}

	return TRUE;
}
DWORD CSafeNumberImg::ActionEvent(CMouse *mouseInfo)
{
	DWORD rt = cDialog::ActionEvent(mouseInfo) ;
	if(rt&&PtInWindow((LONG)mouseInfo->GetMouseX(), (LONG)mouseInfo->GetMouseY()) )
	{
		BOOL ButtonDown = mouseInfo->LButtonDown();
		if(ButtonDown)
		{
			Drag = FALSE;
		}

		BOOL ButtonUp = mouseInfo->LButtonUp();
		if(ButtonUp)
		{
			Drag = FALSE;
		}
		
		BOOL ButtonDrag = mouseInfo->LButtonDrag();
		if(ButtonDrag)
		{
			Drag = TRUE;
		}
	}
	return rt;
}