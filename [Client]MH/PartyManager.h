



// PartyManager.h: interface for the CPartyManager class.
//
//////////////////////////////////////////////////////////////////////

#ifndef _CPARTYMANAGER_H_
#define _CPARTYMANAGER_H_
#pragma once

#include "Party.h"


#define PARTYMGR USINGTON(CPartyManager)

class CPartyManager 
{
	CParty m_Party;
	void SetPartyInfo(PARTY_INFO* pmsg);
	DWORD m_RequestPartyID;

	BOOL m_bIsProcessing;

	PARTYEVENT m_PartyEventRate[ePartyEvent_Max];

	DWORD m_dwRequestPlayerID;	//2008. 5. 23. CBH - ��Ƽ ��û�� �÷��̾� ID ����	

public:
	CPartyManager();
	virtual ~CPartyManager();

	void Init();
	void NetworkMsgParse(BYTE Protocol,void* pMsg);	
	void AddPartyMemberSyn(DWORD AddPartymemeberID); 
	void DelPartyMemberSyn();
	void BanPartyMemberSyn(DWORD DelPartymemberID);
	void ChangeMasterPartySyn(DWORD FromMemberID, DWORD ToMemberID);
	void BreakUpSyn();
	void PartyInviteAccept();
	void PartyInviteDeny();

	void SetRequestPartyID(DWORD id)		{ m_RequestPartyID = id;	}
	DWORD GetRequestPartyID()				{ return m_RequestPartyID;	}


	
	//void CreatePartySyn(int Option);
	//void CreatePartyResult(MSG_DWORDBYTE* pMsg);
	// 2008. 5. 21. CBH - ��Ƽ ���� �߰��� �ɼ� ���� ���� ////
	void CreatePartySyn(PARTY_ADDOPTION* pAddOption);
	void CreatePartyResult(PARTY_ADDOPTION* pMsg);	

	/////////////////////////////////////////////////////////
	void AddPartyMemberResult(SEND_PARTY_MEMBER_INFO* pMsg); 
	void DelPartyMemberResult(MSG_DWORD* pMsg);
	void BanPartyMemberResult(MSG_DWORD* pMsg);
	void ChangeMasterPartyResult(MSG_DWORD* pMsg);
	void BreakUpPartyResult();


	void UserLogIn(SEND_PARTY_MEMBER_INFO* pMsg);
	void UserLogOut(MSG_DWORD* pMsg);
	
	void SetPartyMemberLifePercent(DWORD PlayerID, DWORD LifePercent);
	BYTE GetPartyMemberLifePercent(DWORD PlayerID);


	void SetPartyMemberShieldPercent(DWORD PlayerID, DWORD ShieldPercent);
	BYTE GetPartyMemberShieldPercent(DWORD PlayerID);
	
	void SetPartyMemberNaeRyukPercent(DWORD PlayerID, DWORD NaeryukPercent);
	BYTE GetPartyMemberNaeRyukPercent(DWORD PlayerID);
	
	void SetPartyMemberLevel(DWORD PlayerID, LEVELTYPE lvl);
	LEVELTYPE GetPartyMemberLevel(DWORD PlayerID);


	PARTY_MEMBER* GetPartyMemberInfo(int i);

//	DWORD GetPartyIdx()		{	return m_Party.GetPartyIdx();	}
	DWORD GetMasterID()		{	return m_Party.GetMasterIdx();	}
	
	char* GetPartyMemberName(DWORD PlayerID) { return m_Party.GetMemberName(PlayerID); }
	DWORD GetPartyMemberID(int n) { return m_Party.GetMemberID(n); }


	BOOL IsMemberLogIn(int n)	{	return m_Party.IsLogIn(n);	}
		
	DWORD GetTacticObjectID()	{	return m_Party.GetTacticObjectID();	}
	void SetTacticObjectID(DWORD TObjID)	{	m_Party.SetTacticObjectID(TObjID);	}
	
	BOOL IsPartyMember(DWORD PlayerID);
	BOOL PartyChat(char* ChatMsg, char* SenderName);
	BOOL CanActivate();
	
	void SetIsProcessing(BOOL val) { m_bIsProcessing = val;	}
	BOOL IsProcessing() { return m_bIsProcessing;	}

	void LoadPartyEventInfo();
	void RefreshPartyEventBuffDesc();
    DWORD GetPartyEventRate( DWORD Index );

	void SetRequestPlayerID(DWORD dwPlayerID) { m_dwRequestPlayerID = dwPlayerID; }	//���� ��û�� �÷��̾�ID ���̵� ����
	void PartyRequestResult(BOOL bConsent);		//���� ��û ��� ó��
	void PartyError(DWORD dwkind);		//2008. 5. 29. CBH - ��Ƽ ���� ���� �޼��� ��� �Լ�
	void PartyDecition(MSG_NAME_DWORD* pMsg);	//2008. 5. 29. CBH - ��Ƽ��û ������ �޾� ���� ���� ó���Ѵ�.

	void RefleshPartyMatchingList(LEVELTYPE wLevel);			//2008. 5. 27. CBH - ���� ��Ī ���̾�α� ����Ʈ ����	
		
#ifdef _JAPAN_LOCAL_

	BOOL m_bLastPartyHelp;
	BOOL IsHelpPartyMember();
	void Process();
#endif

private:
	BOOL m_MasterChanging;

private:
	void SetMasterChanging(BOOL val) { m_MasterChanging = val;	}
	BOOL IsMasterChanging();
};
EXTERNGLOBALTON(CPartyManager);

#endif


