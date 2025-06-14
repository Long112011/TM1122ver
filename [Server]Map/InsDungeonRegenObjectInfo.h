#ifndef _INSDUNGEONREGENOBJECTINFO_H
#define _INSDUNGEONREGENOBJECTINFO_H

class CInsDungeonRegenObjectInfo
{
public:
	CInsDungeonRegenObjectInfo();
	~CInsDungeonRegenObjectInfo();

	inline void SetGroupID(DWORD dwGroupID)	{ m_dwGroupID = dwGroupID; }
	inline DWORD GetGroupID()	{ return m_dwGroupID; }

	inline void SetObjectID(DWORD dwObjectID)	{ m_dwObjectID = dwObjectID; }
	inline DWORD GetObjectID()	{ return m_dwObjectID; }

	inline void SetObjectKind(BYTE ObjectKind)	{ m_ObjectKind = ObjectKind; }
	inline BYTE GetObjectKind()					{ return m_ObjectKind; }

	inline void SetMonsterKind(WORD wMonsterKind)	{ m_wMonsterKind = wMonsterKind; }
	inline WORD GetMonsterKind()					{ return m_wMonsterKind; }

	void SetPosition(VECTOR3* pPos);
	inline VECTOR3* GetPosition()	{ return &m_vPos; }

	inline void SetRegen(BOOL bRegen)	{ m_bRegen = bRegen; }
	inline BOOL IsRegen()				{ return m_bRegen; }

	inline void SetKey(DWORD dwKey)	{ m_dwKey = dwKey; }
	inline DWORD GetKey()	{ return m_dwKey; }

protected:
	DWORD m_dwGroupID;			//그룹 ID
	DWORD m_dwObjectID;			//오브젝트 ID
	BYTE m_ObjectKind;			//오브젝트 종류
	WORD m_wMonsterKind;		//몬스터 종류
	VECTOR3 m_vPos;				//포지션
	BOOL m_bRegen;				//리젠 상태

	DWORD m_dwKey;
};

#endif //_INSDUNGEONREGENOBJECTINFO_H