#pragma once

class CPartyMemberDlg;
class CPartyBtnDlg;



class CPartyDialog 
{
	CPartyMemberDlg * m_MemberDlg[MAX_PARTY_LISTNUM-1];

	CPartyBtnDlg * m_PartyBtnDlg;
	BOOL m_bActive;
	
	DWORD m_ClickedMemberID;
	
	BOOL	m_bShowMember;
	BOOL	m_bOption;


public:
	CPartyDialog();
	virtual ~CPartyDialog();

	void RegistMemberDlg(int i, CPartyMemberDlg* pDlg);
	void RegistBtnDlg(CPartyBtnDlg* pDlg);

	BOOL IsActive();
	void SetActive(BOOL bState);
	void RefreshDlg();
		
	void MemberBtnPushUp(DWORD id);

	void SetClickedMemberID(DWORD id);
	DWORD GetClickedMemberID();
	
	void Init();
	
	void	ShowMember();

	void	ShowOption();

	void	SetOptionActive( BOOL bFlag );

	void	SetMemberActive( BOOL bFlag );
	BOOL	IsOption()	{ return m_bOption; }
	BOOL	IsMember()	{ return m_bShowMember; }
};