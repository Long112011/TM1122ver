#ifndef _INSDGMISSION_H
#define _INSDGMISSION_H

#include "InsDGMissionEvent.h"
#include "InsDGMissionExecute.h"

//미션 보상
struct MISSION_REQUITAL
{
	DWORD dwMoney;	//돈
	DWORD dwExp;	//경험치
	DWORD dwSExp;	//수련치
	DWORD dwTime;	//제한시간
};

//미션 아이탬 보상
struct MISSION_REQUITAL_ITEM 
{
	WORD wItemIndex;		//아아템 인덱스
	DWORD dwNum;			//갯수
};


class CInsDGMission
{
public:
	CInsDGMission(DWORD dwMissionGroupIDX, DWORD dwMissionIDX);
	virtual ~CInsDGMission();	

	CInsDGMissionEvent* CreateMissionEvent(DWORD dwMissionEventKind);
	CInsDGMissionExecute* CreateMissionExecute(DWORD dwMissionExecuteKind, DWORD dwMissionIDX, DWORD dwParam1, DWORD dwParam2, DWORD dwParam3);

	void OnMissionEvent(MISSION_EVENT_INFO* pMissionEvent);	

	inline DWORD GetMissionGroupIDX()	{ return m_dwMissionGroupIDX; }
	inline void SetMiddionGroupIDX(DWORD dwMissionGroupIDX)	{ m_dwMissionGroupIDX = dwMissionGroupIDX; }
	
	inline DWORD GetMissionIDX()	{ return m_dwMissionIDX; }
	inline void SetMissionIDX(DWORD dwMissionIDX)	{ m_dwMissionIDX = dwMissionIDX; }

	inline void SetMissionRegenGroupID(DWORD dwMissionRegenGroupID)	{ m_dwMissionRegenGroupID = dwMissionRegenGroupID; }
	inline DWORD GetMissionRegenGroupID()	{ return m_dwMissionRegenGroupID; }

	inline DWORD GetNextMissionIDX()	{ return m_dwNextMissionIDX; }
	inline void SetNextMissionIDX(DWORD dwNextMissionIDX)	{ m_dwNextMissionIDX = dwNextMissionIDX; }
	
	inline void SetSuccess(BOOL bSuccessFlag)	{ m_bSuccessFlag = bSuccessFlag; }
	inline BOOL IsSuccess()	{ return m_bSuccessFlag; }

	inline DWORD GetMissionEventKind()	{ return m_pMissionEvent->GetMissionEventKind(); }
	inline DWORD GetMissionExecuteKind()	{ return m_pMissionExecute->GetMissionExecuteKind(); }
	inline void SetExecuteData(DWORD dwExecutedata)	{ m_pMissionExecute->SetExecuteData(dwExecutedata); }
	inline DWORD GetExecuteData()	{ return m_pMissionExecute->GetExecuteData(); }
	inline DWORD GetExecuteParam1()	{ return m_pMissionExecute->GetParam1(); }
	inline DWORD GetExecuteParam2()	{ return m_pMissionExecute->GetParam2(); }
	inline DWORD GetExecuteParam3()	{ return m_pMissionExecute->GetParam3(); }
	inline DWORD GetCount()	{ return m_pMissionExecute->GetCount(); }

	void SetMissionRequital(MISSION_REQUITAL* pMissionRequital);
	inline MISSION_REQUITAL* GetMissionRequital()	{ return &m_MissionRequital; }

	void AddMissionRequitalItem(MISSION_REQUITAL_ITEM* pMissionRequitalItem);
	void RemoveMissionRequitalItemList();
	inline MISSION_REQUITAL_ITEM* GetMissionRequitalItem(DWORD dwKey)	{ return m_MissionRequitalItemTable.GetData(dwKey); }
	inline DWORD GetMissionRequitalItemMaxNum()	{ return m_dwMissionRequitalItemMaxNum; }

protected:
	CInsDGMissionEvent* m_pMissionEvent;
	CInsDGMissionExecute* m_pMissionExecute;
	MISSION_REQUITAL m_MissionRequital;
	CYHHashTable<MISSION_REQUITAL_ITEM>	m_MissionRequitalItemTable;	//미션 보상 관련 아이탬

	DWORD m_dwMissionGroupIDX;
	DWORD m_dwMissionIDX;
	DWORD m_dwMissionRegenGroupID;
	DWORD m_dwNextMissionIDX;
	DWORD m_dwMissionRequitalItemMaxNum;
	BOOL m_bSuccessFlag;	
};

#endif //_INSDGMISSION_H