#pragma once


#include "cConstLinkedList.h"
#include "StateMachinen.h"
#include "AIGroupManager.h"


class CObject;
class CMsgRouter;

class CAISystem  
{
	CYHHashTable<CObject> m_AISubordinatedObject;
	CMsgRouter * m_pROUTER;
	CIndexGenerator m_MonsterIDGenerator;
public:
	CAISystem();
	virtual ~CAISystem();
	
	
	void Process();


	void ConstantProcess(CObject * obj);

	void PeriodicProcess(CObject * obj);

	void AddObject(CObject * obj);

	CObject * RemoveObject(DWORD dwID);
	
/*	void RecallObject(DWORD dwGroupID, WORD kind, VECTOR3 * pPos);*/


//	void SetState(CObject * obj, DWORD inputEvent);
	void SetTransition(CObject * obj, DWORD inputEvent);

	void SendMsg(WORD msgID, DWORD src, DWORD dest, DWORD delay, DWORD flag);
//	CIndexGenerator * GetIndexGen() { return &m_objectIdxGen; }
	DWORD GeneraterMonsterID();
	void ReleaseMonsterID(DWORD id);

	void LoadAIGroupList();
	void RemoveAllList();
};
extern CAISystem * g_pAISystem;
