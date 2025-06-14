
#pragma once

#include "ptrlist.h"
#include "..\[CC]Fuben\FubenDefines.h"

#include "StrTokenizer.h"
class CMHFile;

class CFubenString
{	
	cPtrList		m_TitleList;
	cPtrList		m_DescList;

	DWORD			m_mIndex;
	DWORD			m_sIndex;
	DWORD			m_Key;
	
public:
	
	CFubenString();
	virtual ~CFubenString();

	void	StringLoader(char* str);
	void	SetIndex(DWORD mIdx, DWORD sIdx)		{	m_mIndex = mIdx;	m_sIndex=sIdx;	COMBINEKEY(m_mIndex, m_sIndex, m_Key);	}
	DWORD	GetFubenID()	{	return m_mIndex;	}
	DWORD	GetSubID()		{	return m_sIndex;	}
	DWORD	GetKey()		{	return m_Key;		}

	void	AddLine(char* pStr, int& line, BOOL bTitle=FALSE);

	cPtrList*	GetTitle()	{	return &m_TitleList;	}
	cPtrList*	GetDesc()	{	return &m_DescList;	}


	static CFubenString*	LoadFubenString(CMHFile* pFile);
};

template<class T> void GetScriptParam( T& dataOut, CStrTokenizer* pTokens )
{
	char* pOneToken = pTokens->GetNextTokenUpper();
	dataOut = (T)atol(pOneToken);
}