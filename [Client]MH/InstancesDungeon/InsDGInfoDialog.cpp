#include "StdAfx.h"
#include "WindowIDEnum.h"
#include "InsDGInfoDialog.h"
#include "./interface/cButton.h"
#include "InsDungeonManager.h"
#include "InsDGMission.h"
#include "InsDGMissionInfoDialog.h"
#include "GameIn.h"
#include "ChatManager.h"
#include "./interface/cResourceManager.h"
CInsDGInfoDialog::CInsDGInfoDialog()
{
}
CInsDGInfoDialog::~CInsDGInfoDialog()
{
}
void CInsDGInfoDialog::Linking()
{
	m_pProgressTimeStatic = (cStatic*)GetWindowForID(INSDGINFO_PROGRESS_TIME);
	m_pMissionCountStatic = (cStatic*)GetWindowForID(INSDGINFO_MISSION_COUNT);
	m_pMissionBtn = (cButton*)GetWindowForID(INSDGINFO_MISSION_BTN);
	m_dwEndTime = 0;
	m_dwProgressTime = 0;
	m_dwProcessStartTime = 0;
	SetActive(FALSE);
}
void CInsDGInfoDialog::SetActive( BOOL val )
{
	cDialog::SetActive( val );
}
void CInsDGInfoDialog::OnActionEvent( LONG lId, void * p, DWORD we )
{
	switch( lId )
	{		
	case INSDGINFO_MISSION_BTN:	
		{			
			CInsDGMissionInfoDialog* pMissionInfoDlg = GAMEIN->GetInsDGMissionInfoDlg();
			if(pMissionInfoDlg != NULL)
			{
				if(pMissionInfoDlg->IsActive() == TRUE)
					pMissionInfoDlg->SetActive(FALSE);
				else
					pMissionInfoDlg->SetActive(TRUE);
			}
		}
		break;
	}
}
void CInsDGInfoDialog::SetEndTime(DWORD dwTime)
{
	m_dwEndTime = dwTime;
}
void CInsDGInfoDialog::SetProgressTime(DWORD dwTime)
{
	m_dwProcessStartTime = gCurTime;	
	m_dwProgressTime = dwTime;			
}
void CInsDGInfoDialog::SetMissionCount(DWORD dwCount, DWORD dwMaxCount)
{
	m_pMissionCountStatic->SetStaticText("");
	CInsDGMission* pMission = INSDUNGEONMGR->GetCurMission();
	if(pMission == NULL)
		return;
	DWORD dwMissionCount = dwCount;
	if(dwMaxCount < dwCount)
		dwMissionCount = dwMaxCount;	
	if(pMission->GetMissionExecuteKind() == eMISSION_EXECUTE_KIND_MONSTER_KILLALL)	
	{
		m_pMissionCountStatic->SetStaticText(CHATMGR->GetChatMsg(2714));
	}
	else if(pMission->GetMissionExecuteKind() == eMISSION_EXECUTE_KIND_NPC_TALK)
	{
		m_pMissionCountStatic->SetStaticText(CHATMGR->GetChatMsg(2715));
	}
	else
	{
		char buf[128];
		sprintf(buf, "%s %d/%d", RESRCMGR->GetMsg(37), dwMissionCount, dwMaxCount);
		m_pMissionCountStatic->SetStaticText(buf);
	}	
}
void CInsDGInfoDialog::Render()
{
	if(IsActive() == FALSE)
		return;
	DWORD dwTime = m_dwProcessStartTime + m_dwProgressTime + INSDUNGEONMGR->GetRequitalTime();
	int nProcessTime = (int)( (int)dwTime - (int)gCurTime );
	if(nProcessTime > (int)(m_dwProcessStartTime + m_dwEndTime))
		nProcessTime = (int)(m_dwProcessStartTime + m_dwEndTime);
	nProcessTime /= 1000;
	if(nProcessTime < 0)
		nProcessTime = 0;
	char buf[128];
	sprintf(buf, "%02d:%02d", nProcessTime / 60, nProcessTime % 60);
	m_pProgressTimeStatic->SetStaticText(buf);	
	cDialog::Render();
}