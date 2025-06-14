#ifndef _INSDUNGEONREGENDATAMANAGER_H
#define _INSDUNGEONREGENDATAMANAGER_H

#define INSDUNGEONREGENDATAMGR	USINGTON(CInsDungeonRegenDataManager)

class CInsDungeonRegenGroup;

class CInsDungeonRegenDataManager
{
public:
	CInsDungeonRegenDataManager();
	~CInsDungeonRegenDataManager();

	BOOL LoadInsDungeonRegenList();
	void RemoveAllInsDGRegenGroup();

	inline CYHHashTable<CInsDungeonRegenGroup>* GetInsDGRegenGroupTable()	{ return &m_InsDGRegenGroupTable; }

private:
	CYHHashTable<CInsDungeonRegenGroup>	m_InsDGRegenGroupTable;
};

EXTERNGLOBALTON(CInsDungeonRegenDataManager);

#endif //_INSDUNGEONREGENDATAMANAGER_H