// FubenString.cpp: implementation of the CFubenString class.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "FubenString.h"

#ifdef _MHCLIENT_
#include "./interface/cFont.h"
#include "./interface/cWindowManager.h"
#include "GameResourceManager.h"
#endif

#define TEXT_DELIMITER			'^'
#define TEXT_NEWLINECHAR		'n'
#define TEXT_FLAG				'$'
#define TEXT_TAB				'	'
#define MAX_STR_LENTH			30
#define FubenDIALOG_TEXTWIDTH			180

#include "MHFile.h"
#define LOADUNIT(ekind,strkind,classname,Fubenidx,subFubenidx)	\
		if( strcmp(strKind,strkind) == 0 )						\
			return new classname(ekind,pTokens,Fubenidx,subFubenidx);

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CFubenString::CFubenString()
{
}

CFubenString::~CFubenString()
{
	QString* pData = NULL;
	PTRLISTPOS pos = m_TitleList.GetHeadPosition();
	while( pos )
	{
		pData = (QString*)m_TitleList.GetNext( pos );
		if( pData )
			delete pData;
	}
	m_TitleList.RemoveAll();

	pos = m_DescList.GetHeadPosition();
	while( pos )
	{
		pData = (QString*)m_DescList.GetNext( pos );
		if( pData )
			delete pData;
	}
	m_DescList.RemoveAll();
}


void CFubenString::AddLine(char* pStr, int& line, BOOL bTitle)
{
	int len=0;
	int linelen = 0;
	char tb[128] = { 0, };

//	char linebuf[128] = { 0, };
	DWORD Color = 0;
	BOOL bHighlight = FALSE;

	while(*pStr)
	{
		tb[len]=*pStr;
		++linelen;
		if( pStr + 1 != CharNext( CharPrev( pStr, pStr + 1 ) ) )
		{
			tb[++len] = *(++pStr);
			++linelen;
		}

		// ¸ÇÃ³À½ÀÌ TabÀÏ °æ¿ì
		if(tb[0]==TEXT_TAB || tb[0] == ' ')		{	len = -1;	--linelen;	}		

		// ColorFlag°¡ ÀÖÀ» °æ¿ì
		if(tb[len] == TEXT_FLAG)
		{
			if(*(pStr+=2) == 's')
			{
				Color = Fuben_DESC_COLOR;
				bHighlight = TRUE;
			}
			else if(*(pStr) == 'e')
			{
				Color = Fuben_DESC_HIGHLIGHT;
				bHighlight = FALSE;
			}
			++pStr;
			

			// ÀÌÀüÀÇ ³»¿ë ±â·Ï
			if(len>1)
			{
				tb[len] = 0;
				
				QString* pStc = new QString;
				*pStc = tb;
				pStc->Line = line;	
				pStc->Color = Color;
				if(bTitle)		m_TitleList.AddTail(pStc);
				else			m_DescList.AddTail(pStc);

//				printf("%s\n", tb);
				
				memset(tb, 0, sizeof(tb));
				len=0;
			}
		}
		else
		{
			++len;
			++pStr;
		}

		// ÁöÁ¤µÈ ±æÀÌ¸¦ ³Ñ¾î°¬À» °æ¿ì ±â·Ï
		if(linelen > MAX_STR_LENTH)
		{
			QString* pStc = new QString;
			*pStc = tb;
			pStc->Line = line;
			if(bHighlight==FALSE)
				pStc->Color=Fuben_DESC_COLOR;
			else
				pStc->Color=Fuben_DESC_HIGHLIGHT;

			if(bTitle)		m_TitleList.AddTail(pStc);
			else			m_DescList.AddTail(pStc);
			++line;

			//			printf("%s\n", tb);

			memset(tb, 0, sizeof(tb));			
			len=0;
			linelen=0;
		}
	}

	// À§ÀÇ °æ¿ì¿¡ ÇØ´çµÇÁö ¾Ê´Â ÀÏ¹ÝÀûÀÎ ÅØ½ºÆ®
	if(len)
	{
		QString* pStc = new QString;
		*pStc = tb;
		pStc->Line = line;
		pStc->Color = Fuben_DESC_COLOR;
		if(bTitle)		m_TitleList.AddTail(pStc);
		else			m_DescList.AddTail(pStc);

//		printf("%s\n", tb);
	}	


}




CFubenString* CFubenString::LoadFubenString(CMHFile* pFile)
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