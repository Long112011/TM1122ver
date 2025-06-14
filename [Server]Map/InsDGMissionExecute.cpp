#include "StdAfx.h"
#include "InsDGMissionExecute.h"

CInsDGMissionExecute::CInsDGMissionExecute(DWORD dwExecuteKind, DWORD dwMissionIDX, DWORD dwParam1, DWORD dwParam2, DWORD dwParam3)
{	
	m_dwMissionIDX = dwMissionIDX;
	m_dwMissionExecuteKind = dwExecuteKind;
	m_bSuccessFlag = FALSE;
}

CInsDGMissionExecute::~CInsDGMissionExecute()
{

}