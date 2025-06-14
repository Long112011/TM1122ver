#ifndef _INSDGMISSIONEXECUTE_H
#define _INSDGMISSIONEXECUTE_H

#include "InsDGMissionEvent.h"

enum eMISSION_EXECUTE_KIND
{
	eMISSION_EXECUTE_KIND_MONSTER_TARGETKILL = 0,	//Ư�� ���� ���
	eMISSION_EXECUTE_KIND_MONSTER_KILL,				//�ƹ� ���� ���
	eMISSION_EXECUTE_KIND_MONSTER_KILLALL,			//���� ����
	eMISSION_EXECUTE_KIND_ITEM_USE,					//������ ���
	eMISSION_EXECUTE_KIND_ITEM_GET,					//������ ���
	eMISSION_EXECUTE_KIND_ITEM_TARGET_GET,			//�ش� ������ ������ ���	
	eMISSION_EXECUTE_KIND_NPC_TALK,					//NPC�� ��ȭ	
};

class CInsDGMissionExecute
{
public:
	CInsDGMissionExecute(DWORD dwExecuteKind, DWORD dwMissionIDX, DWORD dwParam1, DWORD dwParam2, DWORD dwParam3);
	virtual ~CInsDGMissionExecute();
	
	virtual void Execute(MISSION_EVENT_INFO* pMissionEvent) = 0;
	virtual void SetExecuteData(DWORD dwExecuteData) = 0;
	virtual DWORD GetExecuteData() = 0;	
	virtual DWORD GetParam1() = 0;
	virtual DWORD GetParam2() = 0;
	virtual DWORD GetParam3() = 0;
	virtual DWORD GetCount() = 0;

	inline DWORD GetMissionIDX()	{ return m_dwMissionIDX; }
	inline DWORD GetMissionExecuteKind()	{return m_dwMissionExecuteKind; }

	inline BOOL IsSuccess()	{ return m_bSuccessFlag; }

protected:
	DWORD m_dwMissionIDX;
	DWORD m_dwMissionExecuteKind;
	BOOL m_bSuccessFlag;
};

#endif //_INSDGMISSIONEXECUTE_H