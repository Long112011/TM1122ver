#ifndef _INSDGMISSIONEVENT_H
#define _INSDGMISSIONEVENT_H

enum eMISSION_EVENT_KIND
{
	eMISSION_EVENT_KIND_NONE = 0,
	eMISSION_EVENT_KIND_HUNT,
	eMISSION_EVENT_KIND_ITEM,
	eMISSION_EVENT_KIND_NPC,
};

struct MISSION_EVENT_INFO
{
	DWORD dwMissionEventKind;
	DWORD dwExecuteData;
	DWORD dwData;
	CPlayer* pPlayer;
};

class CInsDGMissionEvent
{
public:
	CInsDGMissionEvent(DWORD dwMissionEventKind);
	~CInsDGMissionEvent();

	inline void  SetMissionEventKind(DWORD dwMissionEventKind)	{ m_dwMissionEventKind = dwMissionEventKind; }
	inline DWORD GetMissionEventKind()	{ return m_dwMissionEventKind; }

protected:
	DWORD m_dwMissionEventKind;
};

#endif //_INSDGMISSIONEVENT_H