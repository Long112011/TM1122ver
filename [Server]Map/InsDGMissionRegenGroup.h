#ifndef _INSDGMISSION_REGENGROUP_H
#define _INSDGMISSION_REGENGROUP_H

class CInsDGMissionRegenInfo;

class CInsDGMissionRegenGroup
{
public:
	CInsDGMissionRegenGroup(DWORD dwGroupIndex);
	virtual ~CInsDGMissionRegenGroup();	
	
	void AddMissionRegenInfo(CInsDGMissionRegenInfo* pInsDGMissionRegenInfo);
	void RemoveInsDGMissionRegenInfoList();
	CInsDGMissionRegenInfo* GetInsDGMissionRegenInfo(DWORD dwIndex);
	
	inline DWORD GetGroupID()	{ return m_dwGroupIndex; }
	inline DWORD GetMaxNum()	{return m_dwMaxNum; }

protected:
	CYHHashTable<CInsDGMissionRegenInfo> m_InsDGMissionRegenInfoList;

	DWORD m_dwGroupIndex;
	DWORD m_dwMaxNum;
};

#endif //_INSDGMISSION_REGENGROUP_H