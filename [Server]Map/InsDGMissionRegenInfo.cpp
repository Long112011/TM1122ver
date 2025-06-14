#include "StdAfx.h"
#include "InsDGMissionRegenInfo.h"

CInsDGMissionRegenInfo::CInsDGMissionRegenInfo(DWORD dwGroupID, WORD wMonsterIndex, BYTE ObjectKind, VECTOR3* vPos)
{	
	m_dwGroupID = dwGroupID;
	m_wMonsterIndex = wMonsterIndex;
	m_ObjectKind = ObjectKind;
	m_vPos.x = vPos->x;
	m_vPos.y = vPos->y;
	m_vPos.z = vPos->z;
}

CInsDGMissionRegenInfo::~CInsDGMissionRegenInfo()
{

}