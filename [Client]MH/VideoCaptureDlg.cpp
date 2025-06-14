#include "stdafx.h"
#include "VideoCaptureDlg.h"
#include "VideoCaptureIcon.h"
#include "GameIn.h"
#include "cComboBox.h"
#include "cCheckBox.h"
#include "./Interface/cResourceManager.h"
#include "WindowIDEnum.h"
#include "./Interface/cWindowManager.h"
#include "VideoCaptureManager.h"
CVideoCaptureDlg::CVideoCaptureDlg()
{
	m_Dlg=NULL;		
	m_pNoCursor=NULL;
	m_pComboBox[SIZECombo]=NULL;
	m_pComboBox[OPTTCombo]=NULL;
}
CVideoCaptureDlg::~CVideoCaptureDlg()
{
	m_Dlg=NULL;		
	m_pNoCursor=NULL;
	m_pComboBox[SIZECombo]=NULL;
	m_pComboBox[OPTTCombo]=NULL;
}
void CVideoCaptureDlg::Linking()
{
	m_Dlg= WINDOWMGR->GetWindowForID(VIDEOCAPTURE_DLG);

	m_pComboBox[SIZECombo]= (cComboBox*)m_Dlg->GetWindowForID( VIDEOCAPTURE_SIZE_COMBO );
	m_pComboBox[OPTTCombo]= (cComboBox*)m_Dlg->GetWindowForID( VIDEOCAPTURE_OPT_COMBO );
	m_pComboBox[SIZECombo]->SelectComboText( 0 );
	m_pComboBox[OPTTCombo]->SelectComboText( 0 );

	m_pNoCursor		= (cCheckBox*)m_Dlg->GetWindowForID( VIDEOCAPTURE_NO_CURSOR_CHECKBOX );
}
int CVideoCaptureDlg::GetSelectedSize()
{
	return m_pComboBox[SIZECombo]->GetCurSelectedIdx();
}
int CVideoCaptureDlg::GetSelectedOpt()
{
	return m_pComboBox[OPTTCombo]->GetCurSelectedIdx();
}
BOOL CVideoCaptureDlg::IsIncludeCursor()
{
	return m_pNoCursor->IsChecked();
}
void CVideoCaptureDlg::OnActionEvent(LONG lId, void * p, DWORD we)
{
	if(we & WE_BTNCLICK)
	{
		switch(lId)
		{
		case VIDEOCAPTURE_START_BTN:		
			
			
			this->SetActive(FALSE);
			GAMEIN->GetVideoCaptureIcon()->ShowStopBtn();
			VIDEOCAPTUREMGR->CaptureStart();
			break;
		case VIDEOCAPTURE_EXIT_BTN:
			this->SetActive(FALSE);
			break;
		default:
			ASSERT(0);
			break;
		}
	}
}