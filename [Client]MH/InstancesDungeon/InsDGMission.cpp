#include "StdAfx.h"
#include "InsDGMission.h"
#include "Player.h"
CInsDGMission::CInsDGMission(DWORD dwMissionGroupIDX, DWORD dwMissionIDX)
{
	m_dwMissionGroupIDX = dwMissionGroupIDX;
	m_dwMissionIDX = dwMissionIDX;
	m_dwNextMissionIDX = 0;	
	m_dwMaxCount = 0;
	m_dwCurrentCount = 0;
	m_dwMissionExecuteKind = 0;
	m_dwMissionTargetIDX = 0;
	m_bSuccessFlag = FALSE;
}
CInsDGMission::~CInsDGMission()
{	
}