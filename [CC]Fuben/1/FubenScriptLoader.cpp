#include "stdafx.h"
#include "FubenScriptLoader.h"

#include "MHFile.h"
#include "FubenString.h"

#define LOADUNIT(ekind,strkind,classname,Fubenidx,subFubenidx)	\
		if( strcmp(strKind,strkind) == 0 )						\
			return new classname(ekind,pTokens,Fubenidx,subFubenidx);

CFubenScriptLoader::CFubenScriptLoader()
{
}

CFubenScriptLoader::~CFubenScriptLoader()
{
}

CFubenString* CFubenScriptLoader::LoadFubenString(CMHFile* pFile)
{
	CFubenString* pQStr = new CFubenString;


	char* pChekString = NULL;
	char buf[1024];
	char Token[1024];
	int	tline=0;
	int dline=0;

	while( !pFile->IsEOF() )
	{
		pFile->GetString(Token);

		if( pChekString = strstr( Token, "}" ) )
			break;

		
		if(strcmp(Token,"#TITLE") == 0)
		{
			pFile->GetLine(buf, 1024);
			pQStr->AddLine(buf, tline, TRUE);
		}
		if(strcmp(Token,"#DESC") == 0)
		{
			while( !pFile->IsEOF() ) 
			{
				pFile->GetLine(buf, 1024);

				char * p = buf;
				int nRt = 0;
				while( *p )
				{
					if( IsDBCSLeadByte( *p ) )
					{
						++p;
					}

					else
					{
						if( *p == '{' )
						{
							nRt = 1;
							break;
						}
						else if(  *p == '}' )
						{

							nRt = 2;
							break;
						}
					}

					++p;
				}

				if( nRt == 1 ) continue;
				else if( nRt == 2 ) break;

//				if( pChekString = strstr( buf, "{" ) )			continue;
//				else if( pChekString = strstr( buf, "}" ) )		break;

				pQStr->AddLine(buf, dline);
				++dline;
			}
		}
	}

	return pQStr;
}




