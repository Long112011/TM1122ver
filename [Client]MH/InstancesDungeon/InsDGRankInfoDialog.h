#ifndef _INSDG_RANK_INFO_DLG_H
#define _INSDG_RANK_INFO_DLG_H
#pragma once
#include "./Interface/cDialog.h"
#include "../input/Mouse.h"
class cListDialog;
enum
{	
	eINSDG_RANKINFOSTATE_CLOSE,
	eINSDG_RANKINFOSTATE_OPEN,
};
struct INSDG_RANKINFO_STATE
{
	DWORD dwInsDGIndex;
	DWORD dwState;
	DWORD dwLineCount;
};
class CInsDGRankInfoDialog : public cDialog
{
public:
	CInsDGRankInfoDialog();
	~CInsDGRankInfoDialog();
	virtual void SetActive( BOOL val );
	void Linking();	
	virtual DWORD ActionEvent(CMouse* mouseInfo);
	void ResetRankInfo();
	void AddRankInfoState(INSDG_RANK_INFO* pRankInfo);
	INSDG_RANKINFO_STATE* GetRankInfoState(DWORD dwInsDGIndex);
	void RemoveRankInfoStateTable();
	char* GetInsDGRank(DWORD dwRankValue);
protected:	
	cListDialog* m_pRankInfoListDlg;
	CYHHashTable<INSDG_RANKINFO_STATE> m_RankInfoStateTable;
	DWORD m_dwRankInfoCount;
	DWORD m_dwSelectListIndex;
};
#endif 