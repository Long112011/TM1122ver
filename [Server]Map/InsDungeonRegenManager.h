#ifndef _INSDUNGEONREGENMANAGER_H
#define _INSDUNGEONREGENMANAGER_H

class CInsDungeonRegenGroup;

class CInsDungeonRegenManager
{
public:
	CInsDungeonRegenManager();
	~CInsDungeonRegenManager();

	BOOL LoadInsDungeonRegenData();

	void RemoveAllInsDGRegenGroup();
	
	void Process();

	void RemoveObject(CObject* pObject);

	inline void SetChannel(DWORD dwChannel)	{ m_dwChannel = dwChannel; }
	
	DWORD GetMonsterMaxRegenNum();
	DWORD GetMonsterCurRegenNum();

private:
	CYHHashTable<CInsDungeonRegenGroup>	m_InsDGRegenGroupTable;

	DWORD m_dwChannel;	
};

#endif //_INSDUNGEONREGENMANAGER_H