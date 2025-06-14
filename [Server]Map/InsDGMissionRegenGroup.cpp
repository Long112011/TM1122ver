#include "StdAfx.h"
#include "InsDGMissionRegenGroup.h"
#include "InsDGMissionRegenInfo.h"

CInsDGMissionRegenGroup::CInsDGMissionRegenGroup(DWORD dwGroupIndex)
{	
	m_InsDGMissionRegenInfoList.Initialize(20);
	m_dwGroupIndex = dwGroupIndex;
	m_dwMaxNum = 0;
}

CInsDGMissionRegenGroup::~CInsDGMissionRegenGroup()
{
	RemoveInsDGMissionRegenInfoList();
}

void CInsDGMissionRegenGroup::AddMissionRegenInfo(CInsDGMissionRegenInfo* pInsDGMissionRegenInfo)
{	
	m_InsDGMissionRegenInfoList.Add(pInsDGMissionRegenInfo, m_dwMaxNum);
	m_dwMaxNum++;
}

void CInsDGMissionRegenGroup::RemoveInsDGMissionRegenInfoList()
{
	CInsDGMissionRegenInfo* pInsDGMissionRegenInfo = NULL;

	m_InsDGMissionRegenInfoList.SetPositionHead();
	while(pInsDGMissionRegenInfo = m_InsDGMissionRegenInfoList.GetData())
	{
		SAFE_DELETE(pInsDGMissionRegenInfo);
	}
	m_InsDGMissionRegenInfoList.RemoveAll();
}

CInsDGMissionRegenInfo* CInsDGMissionRegenGroup::GetInsDGMissionRegenInfo(DWORD dwIndex)
{
	return m_InsDGMissionRegenInfoList.GetData(dwIndex);
}