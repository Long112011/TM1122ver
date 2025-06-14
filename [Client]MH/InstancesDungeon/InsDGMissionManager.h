#ifndef _INSDGMISSIONMANAGER_H
#define _INSDGMISSIONMANAGER_H
#include "InsDGMission.h"
#include "MHFile.h"
#define COMBINEKEY(a, b, c)				\
	if(b<100)			c=a*100+b;		\
	else if(b<1000)		c=a*1000+b;		\
	else if(b<10000)	c=a*10000+b;	
class CInsDGMissionManager
{
public:
	CInsDGMissionManager();
	~CInsDGMissionManager();	
	BOOL LoadInsDGMissionInfoList(DWORD dwMissionGroupIndex);
	void RemoveInsDGMissionInfoList();
	BOOL LoadMissionGroupData(CMHFile* pFile, DWORD dwMissionGroupIDX);
	CInsDGMission* LoadMissionInfoData(CMHFile* pFile, DWORD dwMissionGroupIDX, DWORD dwMissionIDX);
	CInsDGMission* CreateMission(DWORD dwMissionGroupIDX, DWORD dwMissionIDX, DWORD dwMissionEcevuteKind, DWORD dwMissionTargetIDX);
	void Process();
	void MissionUpdate(MSG_INSDG_MISSION_UPDATE* pMsg);
	inline CInsDGMission* GetCurMission()	{ return m_pCurrentMission; }
	void SetCurMission(DWORD dwMissionIDX);
	void MissionResult(MSG_DWORD4* pMsg);
	inline BOOL IsSuccess()	{ return m_bSuccess; }
	inline void SetSuccess(BOOL bSuccess)	{ m_bSuccess = bSuccess; }
	inline DWORD GetMissionGroupIDX()	{ return m_dwMissionGroupIDX; }
private:
	CYHHashTable<CInsDGMission>	m_InsDGMissionTable;
	CInsDGMission* m_pCurrentMission;	
	DWORD m_dwMissionGroupIDX;	
	BOOL m_bSuccess;	
};
#endif 
