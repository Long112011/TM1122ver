#include "stdafx.h"
#include "MHMap.h"
#include "MHFile.h"
#include "WindowIDEnum.h"
#include "DungeonMgr.h"
#include "./Interface/cListDialog.h"
#include "ChatManager.h"
#include "InsFUBENMissionString.h"
#include "GameIn.h"
#include "ItemManager.h"
#include "input/Mouse.h"
CInsFUBENMissionString::CInsFUBENMissionString()
{
}
CInsFUBENMissionString::~CInsFUBENMissionString()
{
}
void CInsFUBENMissionString::Linking()
{
	pFbList=(cListDialog*)GetWindowForID( INSFUBEN_MISSIONINFO_LIST );
	char buf[61] = { 0, };
}
void CInsFUBENMissionString::SetActive(BOOL bFlag)
{
	cDialog::SetActive(bFlag);
}
DWORD CInsFUBENMissionString::ActionEvent( CMouse * mouseInfo )
{
	if( !IsActive() ) return WE_NULL;
	if( !mouseInfo ) return WE_NULL;
    DWORD we = cDialog::ActionEvent( mouseInfo );
	if( PtInWindow((LONG)mouseInfo->GetMouseX(), (LONG)mouseInfo->GetMouseY()) )
	{		
		int Wheel = mouseInfo->GetWheel();
		if(Wheel)
		{
			if( Wheel > 0 )
			{
				pFbList->OnUpwardItem();
			}
			else
			{
				pFbList->OnDownwardItem();
			}
		}
	}
	return we;
}
cListDialog * CInsFUBENMissionString::GetFBInfoListAdd()
{
	return pFbList;
}
void CInsFUBENMissionString::SetInfoMsg(DWORD dwFlag,DWORD dwSub)
{
	pFbList->RemoveAll();
	if(DungeonMGR->GetFubenTitle(dwFlag,dwSub)==NULL)
	{
		pFbList->AddItem( "null", RGB_HALF(250, 200, 50));
	}
	else
	{
		pFbList->AddItem( DungeonMGR->GetFubenTitle(dwFlag,dwSub), RGB_HALF(250, 200, 50));
	}

	pFbList->AddItem( "",0);

	DUNGEON_TEXT* pTooltip = DungeonMGR->GetToolTipInfo( (dwFlag*100)+dwSub );
	if(pTooltip==NULL)
	{
		//pFbList->AddItem( pTooltip->strToolTip, TTTC_DESCRIPTION );
		pFbList->AddItem( "null", TTTC_DESCRIPTION );
	}
	while( pTooltip )
	{
		pFbList->AddItem( pTooltip->strToolTip, TTTC_DESCRIPTION );
		pTooltip = pTooltip->pNext;
	}
}