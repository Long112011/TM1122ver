#include "StdAfx.h"
#include "InsDGPartyMakeDialog.h"
#include "WindowIDEnum.h"
#include "GameIn.h"
#include "PartyCreateDlg.h"
#include "PartyMatchingDlg.h"
#include "ObjectManager.h"
#include "ObjectStateManager.h"
CInsDGPartyMakeDialog::CInsDGPartyMakeDialog()
{
}
CInsDGPartyMakeDialog::~CInsDGPartyMakeDialog()
{
}
void CInsDGPartyMakeDialog::Linking()
{	
}
void CInsDGPartyMakeDialog::OnActionEvent( LONG lId, void * p, DWORD we )
{
	switch( lId )
	{		
	case INSDGPARTYMAKE_CREATE:	
		{
			CPartyCreateDlg* pPartyCreateDlg = GAMEIN->GetPartyCreateDialog();
			if(pPartyCreateDlg == NULL)
				return;
			pPartyCreateDlg->SetActive(TRUE);
			SetActive(FALSE);
			if( HERO->GetState() == eObjectState_Deal )
				OBJECTSTATEMGR->EndObjectState( HERO, eObjectState_Deal );
		}
		break;
	case INSDGPARTYMAKE_MATCHING:	
		{			
			CPartyMatchingDlg* pPartyMatchingDlg = GAMEIN->GetPartyMatchingDlg();
			if(pPartyMatchingDlg == NULL)
				return;
			pPartyMatchingDlg->SetActive(TRUE);
			SetActive(FALSE);
			if( HERO->GetState() == eObjectState_Deal )
				OBJECTSTATEMGR->EndObjectState( HERO, eObjectState_Deal );
		}
		break;
	case INSDGPARTYMAKE_CANCEL:	
		{			
			SetActive(FALSE);
			if( HERO->GetState() == eObjectState_Deal )
				OBJECTSTATEMGR->EndObjectState( HERO, eObjectState_Deal );
		}
		break;
	case INSDGPARTYMAKE_CLOSE:
		{
			SetActive(FALSE);
			if( HERO->GetState() == eObjectState_Deal )
				OBJECTSTATEMGR->EndObjectState( HERO, eObjectState_Deal );
		}
		break;
	}
}