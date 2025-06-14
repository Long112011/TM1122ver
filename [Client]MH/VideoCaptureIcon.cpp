#include "StdAfx.h"
#include "VideoCaptureIcon.h"
#include "VideoCaptureManager.h"
#include "VideoCaptureDlg.h"
#include "GameIn.h"
#include "cComboBox.h"
#include "cCheckBox.h"
#include "cButton.h"
#include "./Interface/cResourceManager.h"
#include "WindowIDEnum.h"
#include "./Interface/cWindowManager.h"
#include "../Input/Mouse.h"
#include "MHTimeManager.h"
//#define PRESS TRUE
//#define RELEASE FALSE
//BOOL button = RELEASE;
//BOOL Drag = FALSE;
extern BOOL DragIcon;
extern double CurFPS;
CVideoCaptureIcon::CVideoCaptureIcon()
{
	m_type = WT_VIDICON_DLG;

	//m_IconBtn[NormalBtn]=NULL;
	//m_IconBtn[StopBtn]=NULL;

	memset(m_IconBtn, 0, sizeof(cButton*)* 2);
}

CVideoCaptureIcon::~CVideoCaptureIcon()
{
	//m_IconBtn[NormalBtn]=NULL;
	//m_IconBtn[StopBtn]=NULL;
	memset(m_IconBtn, 0, sizeof(cButton*)* 2);
}

void CVideoCaptureIcon::Linking()
{
	m_IconBtn[NormalBtn]	= (cButton*)GetWindowForID( VIDEO_CAPTURE_ICON_NORMAL );
	m_IconBtn[StopBtn]		= (cButton*)GetWindowForID( VIDEO_CAPTURE_ICON_STOP );
}

void CVideoCaptureIcon::OnActionEvent(LONG lId, void * p, DWORD we)
{
	
	if( we & WE_BTNCLICK )
	{
		switch( lId )
		{
		case VIDEO_CAPTURE_ICON_NORMAL:	
			{
				 if (DragIcon)
					break;
				GAMEIN->GetVideoCaptureDlg()->SetActive(TRUE);	
			}
			break;

		case VIDEO_CAPTURE_ICON_STOP:
			{
				if (DragIcon)
					break;
				ShowNormalBtn();	break;
			}
		}
	}
}

void CVideoCaptureIcon::ShowNormalBtn()
{
	VIDEOCAPTUREMGR->CaptureStop();

	m_IconBtn[NormalBtn]->SetActive(true);
	m_IconBtn[StopBtn]->SetActive(false);

	//char Temp[MAX_PATH];
	//sprintf(Temp, "%d", (int)CurFPS);
	//m_IconBtn[NormalBtn]->SetText(Temp, 0xFF0000, 0xFF0000, 0xFF0000);
}


void CVideoCaptureIcon::ShowStopBtn()
{
	m_IconBtn[StopBtn]->SetActive(true);
	m_IconBtn[NormalBtn]->SetActive(false);
}

void CVideoCaptureIcon::ShowVideoIconStatic()
{
	m_IconBtn[StopBtn]->SetActive(VIDEOCAPTUREMGR->GetVideoWorkStatic());
	m_IconBtn[NormalBtn]->SetActive(!VIDEOCAPTUREMGR->GetVideoWorkStatic());
}
DWORD CVideoCaptureIcon::ActionEvent(CMouse *mouseInfo)
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
void CVideoCaptureIcon::SetFPS(char * c)
{
	if (VIDEOCAPTUREMGR->GetVideoWorkStatic())
	{
		m_IconBtn[StopBtn]->SetText(c, 0x0000FF, 0x0000FF, 0x0000FF);
		m_IconBtn[StopBtn]->SetFontIdx(14);
	}
	else
	{
		m_IconBtn[NormalBtn]->SetText(c, 0xFF0000, 0xFF0000, 0xFF0000);
		m_IconBtn[NormalBtn]->SetFontIdx(14);
	}
}