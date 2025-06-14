#ifndef _INSDGMISSION_HUNT_H
#define _INSDGMISSION_HUNT_H

#include "InsDGMissionExecute.h"

class CInsDGMissionExecute_Hunt : public CInsDGMissionExecute
{
public:
	CInsDGMissionExecute_Hunt(DWORD dwExecuteKind, DWORD dwMissionIDX, DWORD dwParam1, DWORD dwParam2, DWORD dwParam3);
	virtual ~CInsDGMissionExecute_Hunt();

	virtual void Execute(MISSION_EVENT_INFO* pMissionEvent);

	virtual void SetExecuteData(DWORD dwExecuteData)	{ m_dwMaxCount = dwExecuteData; }
	virtual DWORD GetExecuteData()	{ return m_dwMaxCount; }

	virtual DWORD GetParam1()	{ return m_dwMonsterIndex; }
	virtual DWORD GetParam2()	{ return m_dwParam2; }
	virtual DWORD GetParam3()	{ return m_dwParam3; }

	virtual DWORD GetCount()	{ return m_dwCurCount; }

	void UpdateMission(MISSION_EVENT_INFO* pMissionEvent);

protected:	
	DWORD m_dwMonsterIndex;
	DWORD m_dwMaxCount;
	DWORD m_dwCurCount;
	DWORD m_dwParam2;
	DWORD m_dwParam3;
};

#endif //_INSDGMISSION_HUNT_H