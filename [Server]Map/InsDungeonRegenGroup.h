#ifndef _INSDUNGEONREGENGROUP_H
#define _INSDUNGEONREGENGROUP_H

#include "InsDungeonRegenConditionInfo.h"
#include "InsDungeonRegenObjectInfo.h"

class CInsDungeonRegenGroup
{
public:
	enum eREGEN_TYPE		//���� Ÿ�� ���
	{
		eREGENTYPE_INFINITY = 0,		//���Ѹ���
		eREGENTYPE_ONE,					//�ѹ�����		
	};

public:
	CInsDungeonRegenGroup();
	~CInsDungeonRegenGroup();

	BOOL LoadInsDGRegenInfoData(CInsDungeonRegenGroup* pRegenGroup, DWORD dwChannel);
	void RemoveInsDGRegenInfoList();

	inline void SetChannel(DWORD dwChannel)	{ m_dwChannel = dwChannel; }
	inline DWORD GetChannel()				{ return m_dwChannel; }

	inline void SetGroupID(DWORD dwGroupID)	{ m_dwGroupID = dwGroupID; }
	inline DWORD GetGroupID()				{ return m_dwGroupID; }

	inline DWORD GetRegenObjectCount()	{ return m_dwRegenObjectCount; }

	inline DWORD GetCurRegenNum()	{ return m_dwCurRegenNum; }
	inline DWORD GetMaxRegenNum()	{ return m_dwMaxRegenNum; }

	inline void SetRegenType(DWORD dwRegenType)	{ m_dwRegenType = dwRegenType; }
	inline DWORD GetRegenType()	{ return m_dwRegenType; }

	inline void SetRegenConditionInfo(CInsDungeonRegenConditionInfo* pRegenConditionInfo)
	{
		m_pRegenConditionInfo = pRegenConditionInfo;
	}
	inline CInsDungeonRegenConditionInfo* GetRegenConditionInfo()	{ return m_pRegenConditionInfo; }

	void AddInsDGRegenObjectInfo(CInsDungeonRegenObjectInfo* pInsDGRegenObjectInfo);

	void MonsterRegen(DWORD dwChannel);

	void Process();

	void RemoveObject(CObject* pObject);

	void RegenCheck(DWORD CurObjectNum, DWORD MaxObjectNum);

protected:
	CYHHashTable<CInsDungeonRegenObjectInfo>	m_InsDGRegenObjectInfoTable;
	CInsDungeonRegenConditionInfo* m_pRegenConditionInfo;

	DWORD m_dwRegenType;
	DWORD m_dwChannel;
	DWORD m_dwRegenObjectCount;
	DWORD m_dwGroupID;
	DWORD m_dwCurRegenNum;	//�� ���� ��ġ�� ���ŵȴ� ���� �����Ǿ��ִ� ���� ���̴�
	DWORD m_dwMaxRegenNum;	//�� �����Ǿ� ������ ���� ��ġ�̴�
};

#endif //_INSDUNGEONREGENGROUP_H