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
	DWORD m_dwGroupID;			//�׷� ID
	DWORD m_dwObjectID;			//������Ʈ ID
	BYTE m_ObjectKind;			//������Ʈ ����
	WORD m_wMonsterKind;		//���� ����
	VECTOR3 m_vPos;				//������
	BOOL m_bRegen;				//���� ����

	DWORD m_dwKey;
};

#endif //_INSDUNGEONREGENOBJECTINFO_H