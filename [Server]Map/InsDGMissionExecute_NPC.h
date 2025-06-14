#ifndef _INSDGMISSION_NPC_H
#define _INSDGMISSION_NPC_H

#include "InsDGMissionExecute.h"

class CInsDGMissionExecute_NPC : public CInsDGMissionExecute
{
public:
	CInsDGMissionExecute_NPC(DWORD dwExecuteKind, DWORD dwMissionIDX, DWORD dwParam1, DWORD dwParam2, DWORD dwParam3);
	virtual ~CInsDGMissionExecute_NPC();

	virtual void Execute(MISSION_EVENT_INFO* pMissionEvent);

	virtual void SetExecuteData(DWORD dwExecuteData)	{ m_dwMaxCount = dwExecuteData; }
	virtual DWORD GetExecuteData()	{ return m_dwMaxCount; }

	virtual DWORD GetParam1()	{ return m_dwNPCIndex; }
	virtual DWORD GetParam2()	{ return m_dwParam2; }
	virtual DWORD GetParam3()	{ return m_dwParam3; }

	virtual DWORD GetCount()	{ return m_dwCurCount; }

	void UpdateMission(MISSION_EVENT_INFO* pMissionEvent);

protected:	
	DWORD m_dwNPCIndex;
	DWORD m_dwMaxCount;
	DWORD m_dwCurCount;
	DWORD m_dwParam2;
	DWORD m_dwParam3;
};

#endif //_INSDGMISSION_NPC_H