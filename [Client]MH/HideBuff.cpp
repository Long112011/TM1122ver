#include "Stdafx.h"
#include "HideBuff.h"
#include "WindowIDEnum.h"
#include "cWindowManager.h"
#include "StatusIconDlg.h"
#include "GameIn.h"
#include "./Interface/cAnimationManager.h"
#include "RankButtonDialog.h"
#include "TopIcon.h"
GLOBALTON(CHideBuff)
CHideBuff::CHideBuff()
{
	m_type=WT_HIDEBUFF_DLG;
	pDlg=NULL;

	m_Show = TRUE; 
}

CHideBuff::~CHideBuff()
{
	pDlg=NULL;
}
void CHideBuff::Linking()
{
    pDlg = WINDOWMGR->GetWindowForID( HIDE_BUFF_DLG );
}
void CHideBuff::Render()
{
	if (!m_Show) return;
	cDialog::Render();
}
void CHideBuff::OnActionEvent(LONG lId, void * p, DWORD we)
{
	if (!m_Show) return;
	switch(lId)
	{
		case HIDE_BUFF_BTN:
		{
			STATUSICONDLG->SetShow(!STATUSICONDLG->IsShow());
			RANKBTNDLG->SetShow(STATUSICONDLG->IsShow());
			TOPICONDLG->SetShow(STATUSICONDLG->IsShow());		
		}
		break;
	}
}