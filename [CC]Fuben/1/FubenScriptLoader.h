
#pragma once

#include "StrTokenizer.h"

class CMHFile;
class CFubenString;


class CFubenScriptLoader  
{
public:
	CFubenScriptLoader();
	virtual ~CFubenScriptLoader();

	static CFubenString*	LoadFubenString(CMHFile* pFile);

};


template<class T> void GetScriptParam( T& dataOut, CStrTokenizer* pTokens )
{
	char* pOneToken = pTokens->GetNextTokenUpper();
	dataOut = (T)atol(pOneToken);
}
