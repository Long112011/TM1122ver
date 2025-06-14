







// MHFile.cpp: implementation of the CMHFile class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MHFile.h"
//#include "./interface/Line_Node.h"
//#include "./interface/cWindowFunc.h"
#include "Engine/GraphicEngine.h"
//#include "../[CC]JACK/JackHeaderStruct.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
char         m_Encrypto[]= "E76B2413958B00E193A1";
CMHFile::CMHFile()
{
	fp = NULL;
	memset( &m_Header, 0, sizeof(m_Header) );
	m_crc1 = m_crc2 = 0;
	m_pData = NULL;
	m_Dfp = 0;
	m_bInit = FALSE;
	memset( m_sFileName, 0, MAX_PATH );

	m_pFileStorage = 0;
	m_pFilePointer = 0;
    sprintf(m_Encrypto,"%d",Encrypto);
	sprintf(m_Encrypts,"%d",Encrypts);
}

CMHFile::~CMHFile()
{
	Release();
}


BOOL CMHFile::OpenBin( char* filename )
{
	if( filename == NULL )
		return FALSE;

	fp = fopen( filename, "rb" );
	strcpy(m_sFileName,filename);
	if( fp )
	{
		Clear();
		fread( &m_Header, sizeof(m_Header), 1, fp );	// header

        for(int i=0;i<EncryptNumber;i++)                     //weiye ÐÞ¸Ä¼ÓÃÜµø´ø´ÎÊý 2018-01-12
		{
		  fread( &m_crc1, sizeof(char), 1, fp );		// crc1
		}

		if( m_Header.FileSize )							// data
		{
			m_pData = new char[m_Header.FileSize];
			fread( m_pData, sizeof(char), m_Header.FileSize, fp );
		}

		for(int i=0;i<EncryptNumber;i++)                    //weiye ÐÞ¸Ä¼ÓÃÜµø´ø´ÎÊý 2018-01-12
		{
		   fread( &m_crc2, sizeof(char), 1, fp );	   // crc2
		}
	}	
	else
	{
		return FALSE;
	}

	fclose( fp );

	fp = NULL;

	if( !CheckHeader() )	return FALSE;
	if( !CheckCRC() )		return FALSE;

	m_bInit = TRUE;

	return TRUE;
}

BOOL CMHFile::CheckHeader()
{
	if( m_Header.dwVersion != ( Encrypts  +m_Header.dwType+m_Header.FileSize) ) //weiye   20100415
	{
		Clear();
		MessageBox( NULL, "Header Version Error!!", "Error!!", MB_OK );
		return FALSE;
	}
	if( m_Header.dwType == 0 )
	{
		Clear();
		MessageBox( NULL, "Header Type Error!!", m_sFileName, MB_OK );
		return FALSE;
	}

	return TRUE;
}

BOOL CMHFile::CheckCRC()
{
	char crc = m_Header.dwType;

    int j=0;

	for( DWORD i = 0; i < m_Header.FileSize; ++i )
	{
		crc += m_pData[i];

		m_pData[i] -= ((char)i^m_Encrypto[j]);

		if(j>=strlen(m_Encrypto)-1)
		{
			j = 0;
		}
		if( i%m_Header.dwType == 0 )
		{
			m_pData[i] -= m_Header.dwType;
		}
	}
	return TRUE;
}


void CMHFile::Clear()
{
	if( m_pData )
	{
		delete [] m_pData;
		m_pData = NULL;
	}
	m_Dfp = 0;

	m_bInit = FALSE;
}

BOOL CMHFile::Init(char* filename,char* mode,DWORD dwFlag)
{
	if(!(dwFlag & MHFILE_FLAG_DONOTCHANGE_DIRECTORY_TOROOT))
		DIRECTORYMGR->SetLoadMode(eLM_Root);
	
	
	if(g_bUsingEnginePack)
	{
		if( strcmp( "rb", mode ) == 0 )		m_bReadMode = MHFILE_PACKEDFILE;
		else if( strcmp( "ET", mode ) == 0 )m_bReadMode = MHFILE_ENGINEPACKEDFILE;
		else if( strcmp( "EB", mode ) == 0 )m_bReadMode = MHFILE_ENGINEPACKEDFILE;
		else								m_bReadMode = MHFILE_NORMALMODE;
	}
	else
	{
		if( strcmp( "rb", mode ) == 0 )		m_bReadMode = MHFILE_PACKEDFILE;
		else								m_bReadMode = MHFILE_NORMALMODE;
	}

	
	ySWITCH(m_bReadMode)
		yCASE(MHFILE_PACKEDFILE)
			return OpenBin( filename );
		yCASE(MHFILE_NORMALMODE)
			fp = fopen(filename,"r");
			if(fp)
				return TRUE;
			else
			{
				if(!(dwFlag & MHFILE_FLAG_DONOTDISPLAY_NOTFOUNDERROR))
				{
					char buffer[255];
					sprintf(buffer, "[%s] file is not found",filename);
					LOGEX(buffer,PT_MESSAGEBOX);
				}
				
				return FALSE;
			}
		yCASE(MHFILE_ENGINEPACKEDFILE)
			g_pExecutive->GetFileStorage(&m_pFileStorage);
			m_pFilePointer = m_pFileStorage->FSOpenFile(filename,FSFILE_ACCESSMODE_TEXT);
			if (m_pFilePointer && m_pFileStorage)
			{
				return TRUE;
			}			
			else
			{
				char buffer[255];
				sprintf(buffer, "[%s] file is not found", filename);
				LOGEX(buffer, PT_MESSAGEBOX);
				return FALSE;
			}
				
	yENDSWITCH

	return FALSE;
}


void CMHFile::Release()
{
	if(fp)
	{
		fclose(fp);
		fp = NULL;
	}
	Clear();

	if(m_pFilePointer)
	{
		m_pFileStorage->FSCloseFile(m_pFilePointer);
		m_pFileStorage->Release();
		m_pFilePointer = NULL;
		m_pFileStorage = NULL;
	}
}

char* CMHFile::GetString()
{
	static char buf[512];
	
	GetString(buf);

	return buf;
}

int CMHFile::GetString( char* pBuf )
{
	char buf[512] = {0,};
	DWORD j = 0;
	BOOL bStart, bEnd;
	bStart = bEnd = FALSE;
	// yh;
	int rt = 0;

	ySWITCH(m_bReadMode)
		yCASE(MHFILE_PACKEDFILE)
			if(m_Dfp == (int)m_Header.FileSize) return (rt=EOF);

			for( DWORD i = m_Dfp; i < m_Header.FileSize; ++i )
			{
				if( m_pData[i] == 0x0d && m_pData[i+1] == 0x0a )	// return
				{
					if( bStart )
						break;
					else
					{
						m_Dfp += 2;
						++i;
					}
				}
				else if( m_pData[i] == 0x20 || m_pData[i] == 0x09 )	// spacebar, tab
				{

					++m_Dfp;
					if( bStart )
						bEnd = TRUE;
				}
				else
				{
					if( bEnd ) break;

					buf[j] = m_pData[i];
					++j;
					++m_Dfp;
					bStart = TRUE;
				}
			}
			buf[j] = 0;
		yCASE(MHFILE_NORMALMODE)
			rt = fscanf( fp, "%s", buf );
		yCASE(MHFILE_ENGINEPACKEDFILE)

			rt = m_pFileStorage->FSScanf(m_pFilePointer,"%s",buf);

	yENDSWITCH

	_parsingKeywordString( buf, (char**)(&pBuf) );


	return rt;
}
void CMHFile::SetString(char * str)
{
	fprintf(fp, "%s", str);
}
char* CMHFile::GetStringInQuotation()
{
	char buf[512]={0,};
	static char buf2[512];
	int c;

	ySWITCH(m_bReadMode)
		yCASE(MHFILE_PACKEDFILE)
//			m_Dfp--;
			while( (c=m_pData[m_Dfp]) != '"' )	// Ã¹µû¿ÈÇ¥ ³ª¿Ã¶§±îÁö ½ºÅµ
			{
				++m_Dfp;
				if( c == 10 )					//Ã¹µû¿ÈÇ¥ ³ª¿À±âÀü¿¡ ¿£ÅÍ°¡ ÀÖÀ¸¸é ÁßÁö
				{
					buf2[0] = 0;
					return buf2;
				}
			}
			int n = 0;
			while( 1 )
			{
				++m_Dfp;
				buf[n] = m_pData[m_Dfp];
				if( buf[n] == '"' )
				{
					buf[n] = 0;
					++m_Dfp;
					break;
				}
				if( buf[n] == 10 )				// ¿£ÅÍ
				{
					__asm int 3;
					buf[n] = 0;
					break;
				}
				++n;
			}
			char* aa = &buf2[0];
			memcpy( buf2, buf, n+1 );	
		yCASE(MHFILE_NORMALMODE)
			while( (c=fgetc(fp)) != '"')	// Ã¹µû¿ÈÇ¥ ³ª¿Ã¶§±îÁö ½ºÅµ
			{
				if( c == 10 )//Ã¹µû¿ÈÇ¥ ³ª¿À±âÀü¿¡ ¿£ÅÍ°¡ ÀÖÀ¸¸é ÁßÁö
				{
					Seek(-1);
					buf2[0] = 0;
					return buf2;
				}
			}
			int n=0;
			while(1)
			{
				ASSERT(!feof(fp));
				buf[n] = fgetc(fp);
				if(buf[n] == '"' )
				{
					buf[n] = 0;
					break;
				}
				if(buf[n] == 10) // ¿£ÅÍ
				{
					Seek(-1);

					buf[n] = 0;
					break;
				}
				++n;
			}
			char * aa = &buf2[0];
			//	_parsingKeywordString(buf, &aa);	//KES 030828 ¸·¾ÒÀ½. Æ¯¼ö±â´É Å°°¡ Â©¸²?
			memcpy( buf2, buf, n+1 );
		yCASE(MHFILE_ENGINEPACKEDFILE)
			ASSERT(0);
			// »à!~ Áö¿ø¾ÈÇÔ

	yENDSWITCH

	return buf2;
}

void CMHFile::GetStringInQuotation(char* pBuf)
{
	strcpy(pBuf,GetStringInQuotation());
}

void CMHFile::GetLine(char * pBuf, int len)
{
	char buf[2048] = {0,};
	DWORD j = 0;

	ySWITCH(m_bReadMode)
		yCASE(MHFILE_PACKEDFILE)
			for( DWORD i = m_Dfp; i < m_Header.FileSize; ++i )
			{
				if( m_pData[i] == 0x0d && m_pData[i+1] == 0x0a )	// return
				{
					m_Dfp += 2;
					break;
				}
				else
				{
					buf[j] = m_pData[i];
					j++;
					m_Dfp++;
				}
			}
			buf[j] = 0;
		yCASE(MHFILE_NORMALMODE)
			fgets(buf, 512, fp);
			if( buf[strlen(buf)-1] == 10 )
				buf[strlen(buf)-1] = 0;
		yCASE(MHFILE_ENGINEPACKEDFILE)

			ASSERT(0);
			// »à!~ Áö¿ø¾ÈÇÔ

	yENDSWITCH

	_parsingKeywordString(buf, &pBuf);

}

void CMHFile::GetLineX(char * pBuf, int len)
{
	char buf[2048] = {0,};
	DWORD j = 0;

	ySWITCH(m_bReadMode)
		yCASE(MHFILE_PACKEDFILE)
			if( m_Dfp > 2 ) m_Dfp -= 2;
			for( DWORD i = m_Dfp; i < m_Header.FileSize; ++i )
			{
				if( m_pData[i] == 0x0d && m_pData[i+1] == 0x0a )	// return
				{
					m_Dfp += 2;
					break;
				}

				else
				{
					buf[j] = m_pData[i];
					j++;
					m_Dfp++;
				}
			}
			buf[j] = 0;
		yCASE(MHFILE_NORMALMODE)
			fgets(buf, 512, fp);
			if( buf[strlen(buf)-1] == 10 )
				buf[strlen(buf)-1] = 0;
		yCASE(MHFILE_ENGINEPACKEDFILE)
			ASSERT(0);
			// »à!~ Áö¿ø¾ÈÇÔ

	yENDSWITCH

	_parsingKeywordString(buf, &pBuf);


}

int CMHFile::GetInt()
{
	return atoi(GetString());
}

LONG CMHFile::GetLong()
{
	return (LONG)atoi(GetString());
}

float CMHFile::GetFloat()
{
	return (float)atof(GetString());
}

EXPTYPE CMHFile::GetDword()
{
	return (EXPTYPE)_atoi64(GetString());//return (DWORD)atoi(GetString());
}

EXPTYPE CMHFile::GetWord()
{
	return (WORD)_atoi64(GetString());
}

BYTE CMHFile::GetByte()
{
	return (BYTE)atoi(GetString());
}

BOOL CMHFile::GetBool()
{
	return (BOOL)atoi(GetString());
}

void CMHFile::GetFloat(float* pArray,int count)
{
	for(int n=0;n<count;++n)
	{
		pArray[n] = GetFloat();
	}
}

void CMHFile::GetWord(WORD* pArray,int count)
{
	for(int n=0;n<count;++n)
	{
		pArray[n] = GetWord();
	}
}

void CMHFile::GetDword(DWORD* pArray,int count)
{
	for(int n=0;n<count;++n)
	{
		pArray[n] = GetDword();
	}
}

EXPTYPE CMHFile::GetWord(EXPTYPE* pOut)
{
	ASSERT(m_bReadMode == MHFILE_ENGINEPACKEDFILE);
	
	return m_pFileStorage->FSScanf(m_pFilePointer,"%d",pOut);
}

DWORD CMHFile::GetExpPoint()
{
	return (DWORD)atoi(GetString());
}

// magi82 - Exp Variable Type Change(070523)
EXPTYPE CMHFile::GetExpPoint64()
{
	return (EXPTYPE)_atoi64(GetString());
}

LEVELTYPE CMHFile::GetLevel()
{
	return (LEVELTYPE)atoi(GetString());
}


BOOL CMHFile::IsEOF()

{
	ySWITCH(m_bReadMode)
		yCASE(MHFILE_PACKEDFILE)
			if( m_Header.FileSize == (DWORD)m_Dfp )
				return TRUE;
			else
				return FALSE;
		yCASE(MHFILE_NORMALMODE)
			return feof(fp) ? TRUE : FALSE;
		yCASE(MHFILE_ENGINEPACKEDFILE)
			ASSERT(0);
			// »à!~ Áö¿ø¾ÈÇÔ

	yENDSWITCH
		
	return TRUE;
}

BOOL CMHFile::IsInited()
{
	ySWITCH(m_bReadMode)
		yCASE(MHFILE_PACKEDFILE)
			return m_bInit ? TRUE : FALSE;
		yCASE(MHFILE_NORMALMODE)
			return fp ? TRUE : FALSE;
		yCASE(MHFILE_ENGINEPACKEDFILE)
			ASSERT(0);
			// »à!~ Áö¿ø¾ÈÇÔ

	yENDSWITCH
		

	return TRUE;
}
BOOL CMHFile::Seek(int n)
{
	ySWITCH(m_bReadMode)
		yCASE(MHFILE_PACKEDFILE)
			m_Dfp += n;
			return TRUE;
		yCASE(MHFILE_NORMALMODE)
			return fseek( fp, n, SEEK_CUR );
		yCASE(MHFILE_ENGINEPACKEDFILE)
			ASSERT(0);
			// »à!~ Áö¿ø¾ÈÇÔ

	yENDSWITCH
		
	return TRUE;
}
BOOL CMHFile::GetHex(DWORD* pOut)

{
	ySWITCH(m_bReadMode)
		yCASE(MHFILE_PACKEDFILE)
			ASSERT(0);
			// »à!~ Áö¿ø¾ÈÇÔ
		yCASE(MHFILE_NORMALMODE)
			return fscanf( fp, "%x", pOut );
		yCASE(MHFILE_ENGINEPACKEDFILE)
			return m_pFileStorage->FSScanf(m_pFilePointer,"%x",pOut);

	yENDSWITCH

	return TRUE;
}
/*
BOOL CMHFile::CheckLineEnd( int lineNum )
{
	char buf[512] = {0,};
	int rt = TRUE;
	if( m_Dfp == (int)m_Header.FileSize ) 
		return rt;

	char* pData = m_pData;
	if( m_bReadMode == MHFILE_PACKEDFILE )
	{
		for( DWORD i = m_Dfp; i < m_Header.FileSize; ++i )
		{
			if( 0x0d == pData[i] && 0x0a == pData[i+1] )	//CR
			{
				rt = TRUE;
				break;
			}
			else if( 0x09 == pData[i] || 0x20 == pData[i] )
			{
				continue;
			}
			else
			{
				sprintf( buf, "Check! LastValidNum [ %d ]  Gets End '%s'\nWanna Stop?", lineNum, GetString() );
				if( MessageBox( NULL, buf, m_sFileName, MB_YESNO) == IDYES )
				{
					HWND pHwnd = GetActiveWindow();
					PostMessage( pHwnd, WM_CLOSE, NULL, NULL );
				}
				rt = FALSE;
				
				break;
			}
		}
	}
	else
	{
		MessageBox( NULL, "ReadMode is not PACKEDFILE", m_sFileName, MB_OK );
		rt = FALSE;
	}

	return rt;
}*/
/*
void _parsingKeywordString(char * inString, char ** outString)
{   
	char * sp = inString;
	char * sp2 = *outString;
	while(*sp != 0)
	{

		if( IsDBCSLeadByte(*sp) )
		{
			*sp2++ = *sp++;
			*sp2++ = *sp++;
		}
		else
		{
			switch(*sp)
			{
			case TEXT_DELIMITER:
				{
					++sp;
					char tmp = *sp;
					switch(tmp)
					{
					case TEXT_SPACECHAR:
						{	
							*sp2 = ' ';
							++sp2;
						}
						break;
					case TEXT_EMPTYCHAR:
						{
							*outString[0] = 0;
						}
						break;
					case TEXT_DELIMITER:	//KES Ãß°¡
						{
							*sp2 = '^';
							++sp2;
						}
						break;
					}

				}
				break;
			default:
				{
					*sp2 = *sp;
					++sp2;
				}
			}
			++sp;
		}
	}
	*sp2 = 0;
}*/

// c
/*
WORD CMHFile::GetOperationSystemType()
{
   UINT  nLanID   =   GetSystemDefaultLangID();
   WORD  PriLan   =   PRIMARYLANGID(nLanID);
   WORD  SubLan   =   SUBLANGID(nLanID); 


   if(PriLan == LANG_CHINESE)
   {
	  if( SubLan == SUBLANG_CHINESE_SIMPLIFIED )
      {
          return 1;
      }
      else if( SubLan == SUBLANG_CHINESE_TRADITIONAL ) 
      {
          return 2;
      }
	  else
	  {
          return 3;
	  }

   }
   else
   {
          return 4;
   }
}

char* CMHFile::BIG5ToGB2312(const char* szBIG5String)
{
	LCID lcid = MAKELCID(MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_SIMPLIFIED), SORT_CHINESE_PRC);
	wchar_t* szUnicodeBuff = BIG5ToUnicode(szBIG5String);
	char* szGB2312Buff = UnicodeToGB2312(szUnicodeBuff);
	int nLength = LCMapString(lcid, LCMAP_SIMPLIFIED_CHINESE, szGB2312Buff, -1, NULL, 0);
	char* pBuffer = new char[nLength + 1];
	LCMapString(0x0804, LCMAP_SIMPLIFIED_CHINESE, szGB2312Buff, -1, pBuffer, nLength);
	pBuffer[nLength] = 0;
	delete[] szUnicodeBuff;
	delete[] szGB2312Buff;
	return pBuffer;
}
char* CMHFile::UnicodeToGB2312(const wchar_t* szUnicodeString)
{
	UINT nCodePage = 936; //GB2312
	int nLength = WideCharToMultiByte(nCodePage, 0, szUnicodeString, -1, NULL, 0, NULL, NULL);
	char* pBuffer = new char[nLength + 1];
	WideCharToMultiByte(nCodePage, 0, szUnicodeString, -1, pBuffer, nLength, NULL, NULL);
	pBuffer[nLength] = 0;
	return pBuffer;
}
wchar_t* CMHFile::BIG5ToUnicode(const char* szBIG5String)
{
	UINT nCodePage = 950; //BIG5
	int nLength = MultiByteToWideChar(nCodePage, 0, szBIG5String, -1, NULL, 0);
	wchar_t* pBuffer = new wchar_t[nLength + 1];
	MultiByteToWideChar(nCodePage, 0, szBIG5String, -1, pBuffer, nLength);
	pBuffer[nLength] = 0;
	return pBuffer;
}
char* CMHFile::GB2312ToBIG5(const char* szGBString)
{
	LCID lcid = MAKELCID(MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_SIMPLIFIED), SORT_CHINESE_PRC);
	int nLength = LCMapString(lcid, LCMAP_TRADITIONAL_CHINESE, szGBString, -1, NULL, 0);
	char* pBuffer = new char[nLength + 1];
	LCMapString(lcid, LCMAP_TRADITIONAL_CHINESE, szGBString, -1, pBuffer, nLength);
	pBuffer[nLength] = 0;
	wchar_t* pUnicodeBuff = GB2312ToUnicode(pBuffer);
	char* pBIG5Buff = UnicodeToBIG5(pUnicodeBuff);
	delete[] pBuffer;
	delete[] pUnicodeBuff;
	return pBIG5Buff;
}
wchar_t* CMHFile::GB2312ToUnicode(const char* szGBString)
{
	UINT nCodePage = 936; //GB2312
	int nLength = MultiByteToWideChar(nCodePage, 0, szGBString, -1, NULL, 0);
	wchar_t* pBuffer = new wchar_t[nLength + 1];
	MultiByteToWideChar(nCodePage, 0, szGBString, -1, pBuffer, nLength);
	pBuffer[nLength] = 0;
	return pBuffer;
}
char* CMHFile::UnicodeToBIG5(const wchar_t* szUnicodeString)
{
	UINT nCodePage = 950; //BIG5
	int nLength = WideCharToMultiByte(nCodePage, 0, szUnicodeString, -1, NULL, 0, NULL, NULL);
	char* pBuffer = new char[nLength + 1];
	WideCharToMultiByte(nCodePage, 0, szUnicodeString, -1, pBuffer, nLength, NULL, NULL);
	pBuffer[nLength] = 0;
	return pBuffer;
}*/