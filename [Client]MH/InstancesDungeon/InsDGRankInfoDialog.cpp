#include "StdAfx.h"
#include "InsDGRankInfoDialog.h"
#include "WindowIDEnum.h"
#include "./interface/cListDialog.h"
#include "InsDungeonManager.h"
CInsDGRankInfoDialog::CInsDGRankInfoDialog()
{
	m_type = WT_INSDGRANKINFODIALOG;
	m_pRankInfoListDlg = NULL;
	m_RankInfoStateTable.Initialize(20);		
	m_dwSelectListIndex = 0;
	m_dwRankInfoCount = 0;
}
CInsDGRankInfoDialog::~CInsDGRankInfoDialog()
{
	RemoveRankInfoStateTable();
}
void CInsDGRankInfoDialog::Linking()
{
	m_pRankInfoListDlg = (cListDialog*)GetWindowForID(INSDG_RANK_INFO_LIST);
}
void CInsDGRankInfoDialog::SetActive( BOOL val )
{	
	if(val == TRUE)
		ResetRankInfo();
	cDialog::SetActive(val);
}
DWORD CInsDGRankInfoDialog::ActionEvent(CMouse* mouseInfo)
{
	DWORD we = WE_NULL;
	if( !m_bActive )		return we;
	we = cDialog::ActionEvent(mouseInfo);	
	if(m_pRankInfoListDlg->PtIdxInRow(mouseInfo->GetMouseX(),mouseInfo->GetMouseY()) != -1)
	{
		if(we & WE_LBTNCLICK)
		{
			m_dwSelectListIndex = m_pRankInfoListDlg->GetCurSelectedRowIdx();		
			ResetRankInfo();
		}	
	}	
	return we;
}
void CInsDGRankInfoDialog::ResetRankInfo()
{
	char buf[128] = { 0, };
	m_pRankInfoListDlg->RemoveAll();
	CYHHashTable<INSDG_RANK_INFO>*  pRankInfoTable = INSDUNGEONMGR->GetInsDGRankInfoTable();
	if(pRankInfoTable == NULL)
		return;
	INSDUNGEON_INFO* pInsDGInfo = NULL;
	INSDG_RANK_INFO* pRankInfo = NULL;
	INSDG_RANKINFO_STATE* pRankInfoState = NULL;	
	pRankInfoTable->SetPositionHead();
	while (pRankInfo = pRankInfoTable->GetData())
	{
		pRankInfoState = GetRankInfoState(pRankInfo->dwInsDGIndex);
		if(pRankInfoState == NULL)
			continue;
		pInsDGInfo = INSDUNGEONMGR->GetInsDungeonInfo(pRankInfo->dwInsDGIndex);
		if(pInsDGInfo == NULL)
			continue;		
		sprintf(buf, "%s", pInsDGInfo->szInsDGName);
		m_pRankInfoListDlg->AddItem(buf, RGB(255, 200, 0));
		pRankInfoState->dwLineCount++;
		memset(buf, 0, sizeof(*buf));
		{
			memset(buf, 0, sizeof(*buf));			
			DWORD dwTime = pRankInfo->dwClearTime / 1000;			
			sprintf(buf, "Play time: %dmin %dsec", dwTime / 60, dwTime % 60);			
			m_pRankInfoListDlg->AddItem(buf, RGB(0, 180, 210));
			pRankInfoState->dwLineCount++;
			memset(buf, 0, sizeof(*buf));
			sprintf(buf, "Total kill: %d", pRankInfo->dwMonsterKillNum);			
			m_pRankInfoListDlg->AddItem(buf, RGB(0, 180, 210));
			pRankInfoState->dwLineCount++;
			memset(buf, 0, sizeof(*buf));
			char buff[64];
			SafeStrCpy(buff, GetInsDGRank(pRankInfo->dwRankValue), MAX_NAME_LENGTH + 1);
			sprintf(buf, "Rank: %s",buff);
			m_pRankInfoListDlg->AddItem(buf, RGB(0, 180, 210));
			pRankInfoState->dwLineCount++;
			m_pRankInfoListDlg->AddItem(" ", 0);
			pRankInfoState->dwLineCount++;
		}		
	}
}
void CInsDGRankInfoDialog::AddRankInfoState(INSDG_RANK_INFO* pRankInfo)
{
	INSDG_RANKINFO_STATE* pInfo = m_RankInfoStateTable.GetData(pRankInfo->dwInsDGIndex);
	if(pInfo != NULL)
		return;
	DWORD dwInsDGIndex = pRankInfo->dwInsDGIndex;
	INSDG_RANKINFO_STATE* pRankState = new INSDG_RANKINFO_STATE;
	pRankState->dwInsDGIndex = dwInsDGIndex;
	pRankState->dwState = eINSDG_RANKINFOSTATE_CLOSE;
	pRankState->dwLineCount = 0;
	m_RankInfoStateTable.Add(pRankState, pRankState->dwInsDGIndex);
	m_dwRankInfoCount++;
}
INSDG_RANKINFO_STATE* CInsDGRankInfoDialog::GetRankInfoState(DWORD dwInsDGIndex)
{
	return m_RankInfoStateTable.GetData(dwInsDGIndex);
}
void CInsDGRankInfoDialog::RemoveRankInfoStateTable()
{
	INSDG_RANKINFO_STATE* pRankInfoState = NULL;
	m_RankInfoStateTable.SetPositionHead();
	while (pRankInfoState = m_RankInfoStateTable.GetData())
	{
		SAFE_DELETE(pRankInfoState);
	}
	m_RankInfoStateTable.RemoveAll();
}
char* CInsDGRankInfoDialog::GetInsDGRank(DWORD dwRankValue)
{
	DWORD dwRank = INSDUNGEONMGR->GetRank(dwRankValue);	
	char buf[64] = { 0, };	
	switch(dwRank)
	{
	case eINSDG_RANK_S:
		sprintf(buf, "S");
		break;
	case eINSDG_RANK_A:
		sprintf(buf, "A");
		break;
	case eINSDG_RANK_B:
		sprintf(buf, "B");
		break;
	case eINSDG_RANK_C:
		sprintf(buf, "C");
		break;
	case eINSDG_RANK_F:
		sprintf(buf, "F");
		break;
	}	
	return buf;
}