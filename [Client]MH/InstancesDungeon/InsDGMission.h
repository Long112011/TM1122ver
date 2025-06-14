#ifndef _INSDGMISSION_H
#define _INSDGMISSION_H
enum eMISSION_EXECUTE_KIND
{
	eMISSION_EXECUTE_KIND_MONSTER_TARGETKILL = 0,	
	eMISSION_EXECUTE_KIND_MONSTER_KILL,				
	eMISSION_EXECUTE_KIND_MONSTER_KILLALL,			
	eMISSION_EXECUTE_KIND_ITEM_USE,					
	eMISSION_EXECUTE_KIND_ITEM_GET,					
	eMISSION_EXECUTE_KIND_ITEM_TARGET_GET,			
	eMISSION_EXECUTE_KIND_NPC_TALK,					
};
class CInsDGMission
{
public:
	CInsDGMission(DWORD dwMissionGroupIDX, DWORD dwMissionIDX);
	virtual ~CInsDGMission();
	inline DWORD GetMissionGroupIDX()	{ return m_dwMissionGroupIDX; }
	inline DWORD GetMissionIDX()	{ return m_dwMissionIDX; }
	inline DWORD GetNextMissionIDX()	{ return m_dwNextMissionIDX; }
	inline void SetNextMissionIDX(DWORD dwNextMissionIDX)	{ m_dwNextMissionIDX = dwNextMissionIDX; }
	inline DWORD GetMaxCount()	{ return m_dwMaxCount; }
	inline void SetMaxCount(DWORD dwMaxCount)	{ m_dwMaxCount = dwMaxCount; }
	inline DWORD GetCurrentCount()	{ return m_dwCurrentCount; }
	inline void SetCurrentCount(DWORD dwCurrentCount)	{ m_dwCurrentCount = dwCurrentCount; }
	inline DWORD GetMissionExecuteKind()	{ return m_dwMissionExecuteKind; }
	inline void SetMissionExecuteKind(DWORD dwMissionExecuteKind)	{ m_dwMissionExecuteKind = dwMissionExecuteKind; }
	inline DWORD GetMissionTargetIDX()	{ return m_dwMissionTargetIDX; }
	inline void SetMissionTargetIDX(DWORD dwMissionTargetIDX)	{ m_dwMissionTargetIDX = dwMissionTargetIDX; }
	inline void SetSuccess(BOOL bSuccessFlag)	{ m_bSuccessFlag = bSuccessFlag; }
	inline BOOL IsSuccess()	{ return m_bSuccessFlag; }
protected:	
	DWORD m_dwMissionGroupIDX;		
	DWORD m_dwMissionIDX;			
	DWORD m_dwNextMissionIDX;		
	DWORD m_dwMaxCount;				
	DWORD m_dwCurrentCount;			
	DWORD m_dwMissionExecuteKind;	
	DWORD m_dwMissionTargetIDX;		
	BOOL m_bSuccessFlag;			
};
#endif 