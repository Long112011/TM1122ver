#include "stdafx.h"
#include "InsDungeonRegenConditionInfo.h"

CInsDungeonRegenConditionInfo::CInsDungeonRegenConditionInfo()
{
	m_dwGroupID = 0;
	m_fRatio = 0.0f;
	m_dwRegenStartTime = 0;
	m_dwDelayTime = 0;	
	m_bRegen = FALSE;
}

CInsDungeonRegenConditionInfo::~CInsDungeonRegenConditionInfo()
{

}
