#include "stdafx.h"
#include "InsDungeonRegenObjectInfo.h"

CInsDungeonRegenObjectInfo::CInsDungeonRegenObjectInfo()
{
	m_dwGroupID = 0;
	m_dwObjectID = 0;
	m_ObjectKind = 0;
	m_wMonsterKind = 0;
	memset(&m_vPos, 0, sizeof(m_vPos));	
	m_bRegen = FALSE;

	m_dwKey = 0;
}

CInsDungeonRegenObjectInfo::~CInsDungeonRegenObjectInfo()
{

}

void CInsDungeonRegenObjectInfo::SetPosition(VECTOR3* pPos)
{
	m_vPos.x = pPos->x;
	m_vPos.y = pPos->y;
	m_vPos.z = pPos->z;
}