#ifndef _INSDUNGEONMAPMANAGER_H
#define _INSDUNGEONMAPMANAGER_H

#define INSDUNGEONMAPMGR	USINGTON(CInsDungeonMapManager)

class CPlayer;

class CInsDungeonMapManager
{
public:
	CInsDungeonMapManager();
	~CInsDungeonMapManager();

	BOOL Init(WORD wMapNum);

	BOOL IsInsDungeonMap();
	void ReleaseIsInsDungeonMap();

private:
	BOOL m_bInsDungeonMap;
};

EXTERNGLOBALTON(CInsDungeonMapManager);

#endif //_INSDUNGEONMAPMANAGER_H