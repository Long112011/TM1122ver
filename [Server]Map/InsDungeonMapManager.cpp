#include "stdafx.h"
#include "InsDungeonMapManager.h"
#include "ChannelSystem.h"
#include "InsDungeonManager.h"

GLOBALTON(CInsDungeonMapManager);

CInsDungeonMapManager::CInsDungeonMapManager()
{
	m_bInsDungeonMap = FALSE;
}

CInsDungeonMapManager::~CInsDungeonMapManager()
{

}

BOOL CInsDungeonMapManager::Init(WORD wMapNum)
{
	if (g_pServerSystem->GetMap()->IsMapKind(eInsDungeon/*eInstanceDungeonMap*/, wMapNum))
	{
		m_bInsDungeonMap = TRUE;
		CHANNELSYSTEM->InitInsDungeonMapChannel();		
	}
	else
		m_bInsDungeonMap = FALSE;

	return m_bInsDungeonMap;
}

BOOL CInsDungeonMapManager::IsInsDungeonMap()
{
	return m_bInsDungeonMap;
}