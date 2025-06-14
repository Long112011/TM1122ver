#include "StdAfx.h"
#include "LuckyDrawIcon.h"
#include "WindowIDEnum.h"
#include "GameIn.h"
#include "cButton.h"
#include "./Interface/cResourceManager.h"
#include "./Interface/cWindowManager.h"
#include "LuckerDialog.h"

#include "SafeNumberIcon.h"
#include "chatmanager.h"

#include "../Input/Mouse.h"
BOOL DragIcon = FALSE;
CLuckyDrawIcon::CLuckyDrawIcon()
{
	m_type=WT_LUCKYICON_DLG;
	m_IconBtn=NULL;
	IsLuckyDraw = FALSE;
}

CLuckyDrawIcon::~CLuckyDrawIcon()
{
	m_IconBtn=NULL;
	IsLuckyDraw = FALSE;
}
void CLuckyDrawIcon::SetOpen(bool b)
{ 
	IsLuckyDraw = b; 

	SetActive(b);
	m_IconBtn->SetActive(b);
}
void CLuckyDrawIcon::Linking()
{
	m_IconBtn	= (cButton*)GetWindowForID( LUCKY_ICON_BTN );
}

void CLuckyDrawIcon::OnActionEvent(LONG lId, void * p, DWORD we)
{
	if(!GAMEIN->GetSafeNumberIcon()->GetSafeStatic())
	{
		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(2599) );
		return;
	}
	if (!IsLuckyDraw)
	{
		CHATMGR->AddMsg(CTC_SYSMSG, "disabled!");
		return;
	}
	if( we & WE_BTNCLICK )
	{
		switch( lId )
		{
		case LUCKY_ICON_BTN:	
			{
				if(DragIcon)
					break;

				GAMEIN->GetLuckerDialog()->SetActive(!GAMEIN->GetLuckerDialog()->IsActive());	
			}
			break;
		}
	}
}
DWORD CLuckyDrawIcon::ActionEvent(CMouse *mouseInfo)
{
	DWORD rt = cDialog::ActionEvent(mouseInfo) ;
	if(rt&&PtInWindow((LONG)mouseInfo->GetMouseX(), (LONG)mouseInfo->GetMouseY()) )
	{
		BOOL ButtonDown = mouseInfo->LButtonDown();
		if(ButtonDown)
		{
			DragIcon = FALSE;
		}

		BOOL ButtonUp = mouseInfo->LButtonUp();
		if(ButtonUp)
		{
			DragIcon = FALSE;
		}
		
		BOOL ButtonDrag = mouseInfo->LButtonDrag();
		if(ButtonDrag)
		{
			DragIcon = TRUE;
		}
	}
	return rt;
}