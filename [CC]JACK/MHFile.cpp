



// MHFile.cpp: implementation of the CMHFile class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MHFile.h"
#include "../[CC]JACK/JackHeaderStruct.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMHFile::CMHFile()

{
	fp = NULL;
	m_bReadMode = MHFILE_NORMALMODE;
	memset( &m_Header, 0, sizeof(m_Header) );
	m_crc1 = m_crc2 = 0;
	m_pData = NULL;
	m_Dfp = 0;
	m_bInit = FALSE;

	sprintf(m_Encrypto,"%d",Encrypto);   //weiye º”√‹√‹‘ø≥ı ºªØ 2018-01-14
	sprintf(m_Encrypts,"%d",Encrypts);
}

CMHFile::~CMHFile()
{
	Release();
}


BOOL CMHFile::Init(char* filename,char* mode, DWORD dwFlag)
{
	if( strcmp( "rb", mode ) == 0 )
		m_bReadMode = MHFILE_PACKEDFILE;
	else
		m_bReadMode = MHFILE_NORMALMODE;

	if( m_bReadMode == MHFILE_PACKEDFILE )
		return OpenBin( filename );
	else if( m_bReadMode == MHFILE_NORMALMODE )
	{		
		fp = fopen(filename,mode);
		if(fp)
			return TRUE;
		else
		{
			if(!(dwFlag & MHFILE_FLAG_DONOTDISPLAY_NOTFOUNDERROR))
			{			
				char buffer[255];
				sprintf(buffer, "[%s] file is not found",filename);
				MessageBox(NULL, buffer, 0,0);
			}		
			return FALSE;
		}
	}


	return TRUE;
}

void CMHFile::Release()
{
	if(fp)
	{
		fclose(fp);
		fp = NULL;
	}
	Clear();
}

char* CMHFile::GetString()
{
	static char buf[256];
	GetString( buf );
	return buf;
}

void CMHFile::GetString(char* pBuf)
{
	if( m_bReadMode == MHFILE_PACKEDFILE )
	{
		DWORD j = 0;
		BOOL bStart, bEnd;
		bStart = bEnd = FALSE;
		
		if( m_Dfp == (int)m_Header.dwFileSize ) return;
		
		for( DWORD i = m_Dfp; i < m_Header.dwFileSize; ++i )
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
				m_Dfp++;
				if( bStart )
					bEnd = TRUE;
			}
			else
			{
				if( bEnd ) break;
				pBuf[j] = m_pData[i];

				j++;
				m_Dfp++;
				bStart = TRUE;
			}
		}
		pBuf[j] = 0;	
	}
	else if( m_bReadMode == MHFILE_NORMALMODE )
		fscanf(fp,"%s",pBuf);
}

void CMHFile::GetLine(char * pBuf, int len)
{
	if( m_bReadMode == MHFILE_PACKEDFILE )
	{		
		DWORD j = 0;
		
		for( DWORD i = m_Dfp; i < m_Header.dwFileSize; ++i )
		{
			if( m_pData[i] == 0x0d && m_pData[i+1] == 0x0a )	// return
			{
				m_Dfp += 2;
				break;
			}
			else
			{
				pBuf[j] = m_pData[i];
				j++;
				m_Dfp++;
			}
		}
		pBuf[j] = 0;
	}
	else if( m_bReadMode == MHFILE_NORMALMODE )
		fgets(pBuf, len, fp);
}

void CMHFile::GetLineX(char * pBuf, int len)
{
	if( m_bReadMode == MHFILE_PACKEDFILE )
	{
		DWORD j = 0;
		
		if( m_Dfp > 2 ) m_Dfp -= 2;
		for( DWORD i = m_Dfp; i < m_Header.dwFileSize; ++i )
		{
			if( m_pData[i] == 0x0d && m_pData[i+1] == 0x0a )	// return
			{
				m_Dfp += 2;
				break;
			}
			else
			{
				pBuf[j] = m_pData[i];
				j++;
				m_Dfp++;
			}
		}
		pBuf[j] = 0;
	}
	else if( m_bReadMode == MHFILE_NORMALMODE )
		fgets(pBuf, len, fp);
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
DWORD CMHFile::GetDword()
{
	return (DWORD)_atoi64(GetString());//return (DWORD)atoi(GetString());
}
WORD CMHFile::GetWord()
{
	return (WORD)atoi(GetString());
}
BYTE CMHFile::GetByte()
{
	return (BYTE)atoi(GetString());
}
BOOL CMHFile::GetBool()
{
	return (BOOL)atoi(GetString());
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
BOOL CMHFile::GetLevel()
{
	return (LEVELTYPE)atoi(GetString());
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

BOOL CMHFile::IsEOF()
{
	if( m_bReadMode == MHFILE_PACKEDFILE )
		return (m_Header.dwFileSize == (DWORD)m_Dfp) ? TRUE : FALSE;
	else if( m_bReadMode == MHFILE_NORMALMODE )
		return feof(fp) ? TRUE : FALSE;

	return FALSE;
}

BOOL CMHFile::Seek(int n)
{
	if( m_bReadMode == MHFILE_PACKEDFILE )

	{
		m_Dfp += n;
		return TRUE;
	}
	else if( m_bReadMode == MHFILE_NORMALMODE )
		return fseek(fp,n,SEEK_CUR);

	return TRUE;
}

char* CMHFile::GetStringInQuotation()
{
	char buf[256]={0,};
	static char buf2[256];
	int c;
	
	if( m_bReadMode == MHFILE_PACKEDFILE )
	{
		while( (c=m_pData[m_Dfp]) != '"' )	// √πµ˚ø»«• ≥™ø√∂ß±Ó¡ˆ Ω∫≈µ
		{
			++m_Dfp;
			if( c == 10 )					//√πµ˚ø»«• ≥™ø¿±‚¿¸ø° ø£≈Õ∞° ¿÷¿∏∏È ¡ﬂ¡ˆ
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
			if( buf[n] == 10 )				// ø£≈Õ
			{
				buf[n] = 0;
				break;
			}
			++n;
		}
		char* aa = &buf2[0];
		memcpy( buf2, buf, n+1 );
	}

	else if( m_bReadMode == MHFILE_NORMALMODE )
	{
		while( (c=fgetc(fp)) != '"')	// √πµ˚ø»«• ≥™ø√∂ß±Ó¡ˆ Ω∫≈µ
		{
			if( c == 10 )//√πµ˚ø»«• ≥™ø¿±‚¿¸ø° ø£≈Õ∞° ¿÷¿∏∏È ¡ﬂ¡ˆ
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
			if(buf[n] == 10) // ø£≈Õ
			{
				Seek(-1);
				buf[n] = 0;
				break;
			}
			++n;
		}
		char * aa = &buf2[0];
		memcpy( buf2, buf, n+1 );	
	}

	return buf2;
}

void CMHFile::GetStringInQuotation(char* pBuf)
{
	strcpy(pBuf,GetStringInQuotation());
}

void CMHFile::SetString(char * str)
{
	fprintf(fp, "%s", str);
}

BOOL CMHFile::IsLineEnd()
{
	BOOL bEnd = FALSE;
	if( m_bReadMode == MHFILE_PACKEDFILE )
	{
		DWORD i = m_Dfp;


		if( m_pData[i] == 0x0d && m_pData[i+1] == 0x0a )	// return
		{
			bEnd = TRUE;
		}
		else
		{
			bEnd = FALSE;
		}
	}
	else if( m_bReadMode == MHFILE_NORMALMODE )
	{
		if( fgetc(fp) == 10 )
			bEnd = TRUE;
		else
			bEnd = FALSE;
	}

	return bEnd;
}



// Binary File
BOOL CMHFile::OpenBin( char* filename )
{
	if( filename == NULL )
		return FALSE;

	fp = fopen( filename, "rb" );
	if( fp )
	{
		Clear();

		fread( &m_Header, sizeof(m_Header), 1, fp );	   // header

        for(int i=0;i<EncryptNumber;i++)                     //weiye –ﬁ∏ƒº”√‹µ¯¥¯¥Œ ˝ 2018-01-12
		{
		  fread( &m_crc1, sizeof(char), 1, fp );		// crc1
		}

		if( m_Header.dwFileSize )							// data
		{
			m_pData = new char[m_Header.dwFileSize];
			fread( m_pData, sizeof(char), m_Header.dwFileSize, fp );
		}

		for(int i=0;i<EncryptNumber;i++)                    //weiye –ﬁ∏ƒº”√‹µ¯¥¯¥Œ ˝ 2018-01-12
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

BOOL CMHFile::CheckHeader()
{
	if( m_Header.dwVersion != ( Encrypts  +m_Header.dwType+m_Header.dwFileSize) ) //weiye   ºÏ≤ÈbinŒƒº˛∞Ê±æ 2018-01-14
	{
		Clear();
		MessageBox( NULL, "Header Version Error!!", "Error!!", MB_OK );
		return FALSE;
	}
	if( m_Header.dwType == 0 )
	{
		Clear();
		MessageBox( NULL, "Header Type Error!!", "Error!!", MB_OK );
		return FALSE;
	}	

	return TRUE;
}

BOOL CMHFile::CheckCRC()
{
	char crc = m_Header.dwType;

    int j=0;

	for( DWORD i = 0; i < m_Header.dwFileSize; ++i )
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

BOOL CMHFile::IsInited()
{
	if( m_bReadMode == MHFILE_PACKEDFILE )
		return m_bInit ? TRUE : FALSE;
	else if( m_bReadMode == MHFILE_NORMALMODE )
		return fp ? TRUE : FALSE;

	return FALSE;
}
/*
char* CMHFile::BIG5ToGB2312(const char* szBIG5String)
{

LCID lcid = MAKELCID(MAKELANGID(LANG_CHINESE,SUBLANG_CHINESE_SIMPLIFIED),SORT_CHINESE_PRC);

wchar_t* szUnicodeBuff = BIG5ToUnicode(szBIG5String);

char* szGB2312Buff = UnicodeToGB2312(szUnicodeBuff);

int nLength = LCMapString(lcid,LCMAP_SIMPLIFIED_CHINESE, szGB2312Buff,-1,NULL,0);

char* pBuffer = new char[nLength + 1];

LCMapString(0x0804,LCMAP_SIMPLIFIED_CHINESE,szGB2312Buff,-1,pBuffer,nLength);

pBuffer[nLength] = 0;


delete[] szUnicodeBuff;

delete[] szGB2312Buff;

return pBuffer;

}
char* CMHFile::UnicodeToGB2312(const wchar_t* szUnicodeString)
{

UINT nCodePage = 936; //GB2312

int nLength=WideCharToMultiByte(nCodePage,0,szUnicodeString,-1,NULL,0,NULL,NULL);

char* pBuffer=new char[nLength+1];

WideCharToMultiByte(nCodePage,0,szUnicodeString,-1,pBuffer,nLength,NULL,NULL);

pBuffer[nLength]=0;

return pBuffer;

}

wchar_t* CMHFile::BIG5ToUnicode(const char* szBIG5String)
{

UINT nCodePage = 950; //BIG5

int nLength=MultiByteToWideChar(nCodePage,0,szBIG5String,-1,NULL,0);

wchar_t* pBuffer = new wchar_t[nLength+1];

MultiByteToWideChar(nCodePage,0,szBIG5String,-1,pBuffer,nLength);

pBuffer[nLength]=0;

return pBuffer;

}
char* CMHFile::GB2312ToBIG5(const char* szGBString)

{

LCID lcid = MAKELCID(MAKELANGID(LANG_CHINESE,SUBLANG_CHINESE_SIMPLIFIED),SORT_CHINESE_PRC);

int nLength = LCMapString(lcid,LCMAP_TRADITIONAL_CHINESE,szGBString,-1,NULL,0);

char* pBuffer=new char[nLength+1];

LCMapString(lcid,LCMAP_TRADITIONAL_CHINESE,szGBString,-1,pBuffer,nLength);

pBuffer[nLength]=0;

wchar_t* pUnicodeBuff = GB2312ToUnicode(pBuffer);

char* pBIG5Buff = UnicodeToBIG5(pUnicodeBuff);

delete[] pBuffer;

delete[] pUnicodeBuff;

return pBIG5Buff;

} 

wchar_t* CMHFile::GB2312ToUnicode(const char* szGBString)

{

UINT nCodePage = 936; //GB2312

int nLength=MultiByteToWideChar(nCodePage,0,szGBString,-1,NULL,0);

wchar_t* pBuffer = new wchar_t[nLength+1];

MultiByteToWideChar(nCodePage,0,szGBString,-1,pBuffer,nLength);

pBuffer[nLength]=0;

return pBuffer;

}

char* CMHFile::UnicodeToBIG5(const wchar_t* szUnicodeString)
{

UINT nCodePage = 950; //BIG5

int nLength=WideCharToMultiByte(nCodePage,0,szUnicodeString,-1,NULL,0,NULL,NULL);

char* pBuffer=new char[nLength+1];

WideCharToMultiByte(nCodePage,0,szUnicodeString,-1,pBuffer,nLength,NULL,NULL);

pBuffer[nLength]=0;

return pBuffer;

}


*/