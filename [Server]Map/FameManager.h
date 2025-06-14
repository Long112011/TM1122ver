#pragma once

#define FAMEMGR CFameManager::GetInstance()

struct FAMEStruct
{
	int		Idx;
	int		FameVal;
	char	FameName[16];

	LONG	PowerSwitch;
	LONG	Power[4];
};

enum Fame_Case 
{
	eFame_beMaster = 0,
	eFame_beMember,
	eFame_beMembertoSenior = 2,
	eFame_beMembertoViceMaster,
	eFame_beSeniortoViceMaster,
	eFame_beSeniortoMember = 5,
	eFame_beViceMastertoSenior,
	eFame_beViceMastertoMember,
	eFame_beMembertoNotmember = 8, 
	eFame_beSeniortoNotmember,
	eFame_beVicemastertoNotmember,
	eFame_Breakup = 50, 
	eFame_BreakupMaster = eFame_Breakup + MUNPA_MASTER,
	eFame_BreakupViceMaster = eFame_Breakup + MUNPA_VICE_MASTER,
	eFame_BreakupSenior = eFame_Breakup + MUNPA_SENIOR,
	eFame_BreakupMember = eFame_Breakup + MUNPA_MEMBER,
};
enum eBADFAME_KIND
{
	eBADFAME_PKMODEON	= 1,
	eBADFAME_ATTACK		= 5,
	eBADFAME_KILL		= 5,
	eBADFAME_BAIL		= -500,
};
class CFameManager  
{
	BOOL m_bIsUpdated;
	WORD m_StartUpdateTime;
	BYTE m_UpdatedDate;
	
	CYHHashTable<FAMEStruct> m_FameLogoList;
public:
	MAKESINGLETON(CFameManager);
	CFameManager();
	virtual ~CFameManager();
	void ChangePlayerFame(CPlayer* pPlayer, BYTE FameCase);
	void ChangePlayersVSPlayersFame(CPlayer** ppWinPlayers, int WinerNum, CPlayer** ppLosePlayer, int LoserNum, BYTE FameCase);
	void Process();
	BOOL IsTimetoFameUpdate();
	void MunpaFameUpdate(WORD MapNum);
	void SetMunpaFame(DWORD MunpaIDX, FAMETYPE Val);
	void SendFame(CPlayer* pPlayer, FAMETYPE FameVal);
	void SetPlayerFame(CPlayer* pPlayer, FAMETYPE FameVal);
	void ChangePlayerBadFame(CPlayer* pPlayer, int eBadFameKind);
	void SendBadFameMsg(CPlayer* pPlayer, FAMETYPE BadFameVal);
	void SetPlayerBadFame(CPlayer* pPlayer, FAMETYPE BadFameVal);
	BOOL BailBadFame(CPlayer* pPlayer, FAMETYPE BailBadFame);
	BOOL  SetBadFameByGold(CPlayer* pPlayer, FAMETYPE BailBadFame);
	
	void LoadFameStage();
	FAMEStruct * GetFameStage(int ItemIdx);
	LONG GetSTR(FAMETYPE pFame);
	LONG GetAGI(FAMETYPE pFame);

	LONG GetCON(FAMETYPE pFame);
	LONG GetINT(FAMETYPE pFame);

	LONG GetPowerSwitch(FAMETYPE pFame)
	{
		int fMax = GetFameStage(0)->FameVal;
		for (WORD i = 0; i < fMax + 1; ++i)
		{
			if (pFame < GetFameStage(i)->FameVal + 1) return GetFameStage(i)->PowerSwitch;
			else if (pFame >= GetFameStage(fMax)->FameVal) return GetFameStage(fMax)->PowerSwitch;
		}
	}
};