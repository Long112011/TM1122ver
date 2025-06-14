




#ifndef _PARTY_MATCHING_DLG_H
#define _PARTY_MATCHING_DLG_H

#include "interface/cDialog.h"
#include "interface/cEditBox.h"
#include "interface/cListDialog.h"
#include "GameResourceStruct.h"
#include "../input/Mouse.h"

class CPartyMatchingDlg : public cDialog
{

private:
	enum
	{
		eREFLESH_DELAYTIME = 5000,	//���� ����Ʈ ��� ���� ���� Ÿ�� 5��
	};

private:	
	cEditBox* m_pLimitLevelEdit;	//���� ����
	cListDialog* m_pPartyList;	//���� ��� ����Ʈ



	DWORD m_dwSelectIdx;
	DWORD m_dwReflashDelayTime;	//����Ʈ ��� ���� ���� �ð�

	CYHHashTable<PARTY_MATCHINGLIST_INFO> m_PartyInfoTable;
	
public:

	CPartyMatchingDlg();
	virtual ~CPartyMatchingDlg();


	void Linking();
	virtual void SetActive( BOOL val );
	virtual DWORD ActionEvent(CMouse* mouseInfo);	
	virtual BOOL OnActionEvent(LONG lId, void * p, DWORD we);
	
	void RemovePartyInfoTable();
	void SetPartyMatchingList(MSG_PARTYMATCHING_INFO* pPartyListInfo);	//2008. 5. 28. CBH - ��Ƽ���� ��û�� ����� ����
	void PartyListSort(PARTY_MATCHINGLIST_INFO* pPartyListInfo, WORD wMaxNum);	//��Ƽ ������ ����

	void PartyRandomSelect();		//��Ƽ �ڵ�����

	WORD GetPartyMemberNum(PARTY_MATCHINGLIST_INFO* pPartyInfo);		//��Ƽ �ο��� ��ȯ
	BOOL CheckPartyRequest(DWORD dwPartyIDX);		//��Ƽ��û üũ ����
	void MasterToPartyRequestSyn(DWORD dwPartyIDX, DWORD dwMasterID);	//��Ƽ��û	
	void SetPartyInfo(PARTY_MATCHINGLIST_INFO* pPartyInfo, PARTY_MATCHINGLIST_INFO Info);	//��Ƽ ������ ����
	void StartReflashDelayTime();		//��� ���� ���� �ð� �ʱ�ȭ	

	PARTY_MATCHINGLIST_INFO* GetPartyInfo(DWORD dwPartyIDX);
};

#endif _PARTY_MATCHING_DLG_H


