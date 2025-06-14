#include "StdAfx.h"
#include "InsDGMissionEvent.h"

CInsDGMissionEvent::CInsDGMissionEvent(DWORD dwMissionEventKind)
{
	m_dwMissionEventKind = eMISSION_EVENT_KIND_NONE;
	m_dwMissionEventKind = dwMissionEventKind;	
}

CInsDGMissionEvent::~CInsDGMissionEvent()
{

}
