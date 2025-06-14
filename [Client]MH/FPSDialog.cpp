#include "stdafx.h"
#include "FPSDialog.h"
#include "WindowIDEnum.h"
#include "./Interface/cWindowManager.h"
#include "../Input/Mouse.h"
#include "ChatManager.h"
#include "./Interface/cPushupButton.h"
extern BOOL DragIcon;
CFPSDialog::CFPSDialog()
{
	p_Dlg=NULL;
	p_Static = NULL;
	p_PushUpBtn = NULL;
	p_Alpha = 0;
	p_EnableAlpha = FALSE;  
}
CFPSDialog::~CFPSDialog()
{
	p_Dlg=NULL;
	p_Static = NULL;
	p_PushUpBtn = NULL;
	p_Alpha = 0;
	p_EnableAlpha = FALSE;
}
void CFPSDialog::Linking()
{
	p_Dlg = WINDOWMGR->GetWindowForID(FPS_DLG);
	p_Static = (cStatic*)GetWindowForID(FPS_STATIC);
	p_PushUpBtn = (cPushupButton*)GetWindowForID(FPS_PIN_BTN);
	p_PushUpBtn->SetAlpha(p_Alpha);

	RECT captionRect = { 0, 0, 0, 0 };
	captionRect.left = 0;
	captionRect.top = 0;
	captionRect.right = p_Static->GetWidth();
	captionRect.bottom = p_Static->GetHeight();
	SetCaptionRect(&captionRect);
	//SetAlwaysTop( TRUE );
	SetWH(p_Static->GetWidth(), p_Static->GetHeight());

	//p_PushUpBtn->SetPush(this->IsMovable());
}
void CFPSDialog::OnActionEvent(LONG lId, void * p, DWORD we)
{
	if (we & WE_PUSHDOWN)
	{
		if (lId == FPS_PIN_BTN)
		{
			p_Dlg->SetMovable(0);			
		}
	}
	if (we & WE_PUSHUP)
	{
		if (lId == FPS_PIN_BTN)
		{
			p_Dlg->SetMovable(1);			
		}
	}	
}
void CFPSDialog::SetPinned(BOOL val)
{
	p_PushUpBtn->SetPush(!val);
}
DWORD CFPSDialog::ActionEvent(CMouse *mouseInfo)
{
	DWORD rt = cDialog::ActionEvent(mouseInfo);
	if (rt&&PtInWindow((LONG)mouseInfo->GetMouseX(), (LONG)mouseInfo->GetMouseY()))
	{
		if (rt & WE_MOUSEOVER)
		{
			p_EnableAlpha = TRUE;
		}		
		
		BOOL ButtonDown = mouseInfo->LButtonDown();
		if (ButtonDown)
		{
			DragIcon = FALSE;
		}
		BOOL ButtonUp = mouseInfo->LButtonUp();
		if (ButtonUp)
		{
			DragIcon = FALSE;
		}
		BOOL ButtonDrag = mouseInfo->LButtonDrag();
		if (ButtonDrag)
		{
			DragIcon = TRUE;
		}
	}

	else
	{
		p_EnableAlpha = FALSE;
	}
	return rt;
}
void CFPSDialog::Render()
{
	AlphaRender();
	cDialog::Render();
}
void CFPSDialog::AlphaRender()
{		
	if (p_EnableAlpha&&p_Alpha<255)
		p_PushUpBtn->SetAlpha(p_Alpha = p_Alpha+5);
	if (!p_EnableAlpha&&p_Alpha>0)
		p_PushUpBtn->SetAlpha(p_Alpha = p_Alpha-5);
}
void CFPSDialog::SetFPS(char * c)
{	
	p_Static->SetStaticText(c);	
}