#ifndef _INSDGMISSION_ITEM_H
#define _INSDGMISSION_ITEM_H

#include "InsDGMissionExecute.h"

class CPlayer;

class CInsDGMissionExecute_Item : public CInsDGMissionExecute
{
public:
	CInsDGMissionExecute_Item(DWORD dwExecuteKind, DWORD dwMissionIDX, DWORD dwParam1, DWORD dwParam2, DWORD dwParam3);
	virtual ~CInsDGMissionExecute_Item();	

	virtual void Execute(MISSION_EVENT_INFO* pMissionEvent);

	virtual void SetExecuteData(DWORD dwExecuteData)	{ m_dwMaxCount = dwExecuteData; }
	virtual DWORD GetExecuteData()	{ return m_dwMaxCount; }

	virtual DWORD GetParam1()	{ return m_wItemIndex; }	
	virtual DWORD GetParam2()	{ return m_dwParam2; }
	virtual DWORD GetParam3()	{ return m_dwParam3; }

	virtual DWORD GetCount()	{ return m_dwCurCount; }

	BOOL RandomDropItem();

	void UpdateMission(MISSION_EVENT_INFO* pMissionEvent);
	void DropMissionItem(CPlayer* pPlayer);
	//void DeleteMissionItem(CPlayer* pEventPlayer);

protected:
	WORD m_wItemIndex;
	DWORD m_dwMaxCount;
	DWORD m_dwCurCount;
	DWORD m_dwParam2;		//µå¶øÈ®·ü
	DWORD m_dwParam3;
};

#endif //_INSDGMISSION_ITEM_H