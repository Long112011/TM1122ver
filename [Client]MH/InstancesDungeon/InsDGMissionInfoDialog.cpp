#include "StdAfx.h"
#include "InsDGMissionInfoDialog.h"
#include "WindowIDEnum.h"
#include "./interface/cListDialog.h"
#include "InsDungeonManager.h"
#include "InsDGMissionString.h"
#include "input/Mouse.h"
CInsDGMissionInfoDialog::CInsDGMissionInfoDialog()
{
}
CInsDGMissionInfoDialog::~CInsDGMissionInfoDialog()
{
}
void CInsDGMissionInfoDialog::Linking()
{
	m_pMissionInfoList =  (cListDialog*)GetWindowForID(INSDG_MISSIONINFO_LIST);	
}
void CInsDGMissionInfoDialog::SetActive( BOOL val )
{
	cDialog::SetActive( val );
	if(val == TRUE)
	{
		SetMissionString();
	}
}
void CInsDGMissionInfoDialog::SetMissionString()
{
	m_pMissionInfoList->RemoveAll();	
	DWORD dwKey = 0;
	DWORD dwKey1 = 0;
	DWORD dwKey2 = 0;	
	if(INSDUNGEONMGR->GetInsDGMissionManager()->IsSuccess() == FALSE)
	{
		CInsDGMission* pMission = INSDUNGEONMGR->GetCurMission();
		if(pMission == NULL)
			return;
		dwKey1 = pMission->GetMissionGroupIDX();
		dwKey2 = pMission->GetMissionIDX();
	}
	else	
	{
		dwKey1 = INSDUNGEONMGR->GetMissionGroupIDX();
		dwKey2 = 0;
	}
	COMBINEKEY(dwKey1, dwKey2, dwKey);
	CInsDGMissionString* pMissionString = INSDUNGEONMGR->GetMissionString(dwKey);
	if(pMissionString == NULL)
		return;
	QString* pTitleStr = NULL;
	PTRLISTPOS sTitlepos = pMissionString->GetTitle()->GetHeadPosition();
	while (sTitlepos)
	{
		pTitleStr = (QString*)pMissionString->GetTitle()->GetNext(sTitlepos);
		if(pTitleStr != NULL)
		{
			m_pMissionInfoList->AddItem(pTitleStr->Str, SUBQUEST_TITLE_COLOR_SEL);			
		}		
	}
	m_pMissionInfoList->AddItem(" ", 0);
	QString* pDescStr = NULL;
	PTRLISTPOS sDescpos = pMissionString->GetDesc()->GetHeadPosition();
	while (sDescpos)
	{
		pDescStr = (QString*)pMissionString->GetDesc()->GetNext(sDescpos);
		if(pDescStr != NULL)
		{
			m_pMissionInfoList->AddItem(pDescStr->Str, QUEST_DESC_COLOR);
		}		
	}
}
DWORD CInsDGMissionInfoDialog::ActionEvent(CMouse * mouseInfo)
{
	if (!IsActive()) return WE_NULL;
	if (!mouseInfo) return WE_NULL;
	DWORD we = cDialog::ActionEvent(mouseInfo);
	if (PtInWindow((LONG)mouseInfo->GetMouseX(), (LONG)mouseInfo->GetMouseY()))
	{
		int Wheel = mouseInfo->GetWheel();
		if (Wheel)
		{
			if (Wheel > 0)
			{
				m_pMissionInfoList->OnUpwardItem();
			}
			else
			{
				m_pMissionInfoList->OnDownwardItem();
			}
		}
	}
	return we;
}