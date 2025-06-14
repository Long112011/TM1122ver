#ifndef _INSDUNGEON_MISSION_STRING_H
#define _INSDUNGEON_MISSION_STRING_H
#if _MSC_VER > 1000
#pragma once
#endif 
#include "ptrlist.h"
#include "..\[CC]Quest\QuestDefines.h"
class CInsDGMissionString
{
public:
	CInsDGMissionString();
	virtual ~CInsDGMissionString();
	void	StringLoader(char* str);
	void	SetIndex(DWORD dwMissionGroupIDX, DWORD dwMissionIDX);
	DWORD	GetMissionGroupIDX()	{	return m_dwMissionGroupIDX;	}
	DWORD	GetMissionIDX()		{	return m_dwMissionIDX;	}
	DWORD	GetKey()		{	return m_Key;		}
	void	AddLine(char* pStr, int& line, BOOL bTitle=FALSE);
	cPtrList*	GetTitle()	{	return &m_TitleList;	}
	cPtrList*	GetDesc()	{	return &m_DescList;	}
private:
	cPtrList		m_TitleList;
	cPtrList		m_DescList;
	DWORD			m_dwMissionGroupIDX;
	DWORD			m_dwMissionIDX;
	DWORD			m_Key;
};
#endif 
