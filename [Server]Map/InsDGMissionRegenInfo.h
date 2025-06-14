#ifndef _INSDGMISSION_REGENINFO_H
#define _INSDGMISSION_REGENINFO_H

class CInsDGMissionRegenInfo
{
public:
	CInsDGMissionRegenInfo(DWORD dwGroupID, WORD wMonsterIndex, BYTE ObjectKind, VECTOR3* vPos);
	virtual ~CInsDGMissionRegenInfo();

	inline DWORD GetGroupID()	{ return m_dwGroupID; }
	inline WORD GetMonsterIndex()	{return m_wMonsterIndex; }
	inline BYTE GetObjectKind()	{ return m_ObjectKind; }
	inline VECTOR3* GetPos()	{ return &m_vPos; }

protected:
	DWORD m_dwGroupID;
	WORD m_wMonsterIndex;
	BYTE m_ObjectKind;
	VECTOR3 m_vPos;
};

#endif //_INSDGMISSION_REGENINFO_H