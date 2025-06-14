#define __MHFILEEX_C__

#include "stdafx.h"
#include "MHFileEx.h"

DWORD		m_FileSize;
CMHFileEx::CMHFileEx()
{
	m_fp = NULL;
	memset( m_szFullFileName, 0, 256 );
	memset( m_szFileName, 0, 256 );
	memset( m_szExt, 0, 256 );
	memset( &m_Header, 0, sizeof(MHFILE_HEADER) );
	m_crc1 = m_crc2 = 0;
	m_pData = NULL;	
	m_pBinData = NULL;
	sprintf(m_Encrypto,"%d",Encrypto);
	sprintf(m_Encrypts,"%d",Encrypts);
}

CMHFileEx::~CMHFileEx()
{
	Clear();
}

void CMHFileEx::Clear()
{
	if( m_pData )
	{
		delete [] m_pData;
		m_pData = NULL;
	}
	if( m_pBinData )
	{
		delete [] m_pBinData;
		m_pBinData = NULL;
	}
}

BOOL CMHFileEx::New( const char* fullfilename )
{
	m_FileSize = 4096;
	Clear();
	m_pData = new char[m_FileSize];
	memset( m_pData, 0, m_FileSize );

	InitFileName( fullfilename );

	return TRUE;
}

BOOL CMHFileEx::OpenTxt( const char* fullfilename )
{
	InitFileName( fullfilename );
	m_FileSize = 0;
	//__asm int 3;
	m_fp = fopen( fullfilename, "rb" );
	fseek(m_fp,0,SEEK_END);
	m_FileSize = ftell(m_fp);
	fseek(m_fp,0,SEEK_SET);
	m_FileSize +=1;
	if( m_fp )
	{
		Clear();
		m_pData = new char[m_FileSize];
		memset( m_pData, 0, m_FileSize );

		m_Header.dwDataSize = 0;
		while( !feof(m_fp) )
		{
			m_pData[m_Header.dwDataSize] = fgetc( m_fp );
			m_Header.dwDataSize++;
			if( m_Header.dwDataSize >= m_FileSize )
				break;
		}
		m_Header.dwDataSize--;
		m_pData[m_Header.dwDataSize] = 0;		
		fclose( m_fp );
	}
	return TRUE;
}

BOOL CMHFileEx::OpenBin( const char* fullfilename )
{
	InitFileName( fullfilename );
	m_FileSize=0;
	m_fp = fopen( fullfilename, "rb" );
	fseek(m_fp,0,SEEK_END);
	m_FileSize = ftell(m_fp);
	fseek(m_fp,0,SEEK_SET);
	m_FileSize +=1;
	if( m_fp )
	{
		Clear();
		fread( &m_Header, sizeof(m_Header), 1, m_fp );	// header

		for(int i=0;i<EncryptNumber;i++)
		{
	     	fread( &m_crc1, sizeof(char), 1, m_fp );	
		}

		if( m_Header.dwDataSize )						// data
		{
			m_pData = new char[MEGA];
			memset( m_pData, 0, MEGA );
			fread( m_pData, sizeof(char), m_Header.dwDataSize, m_fp );
			m_pData[m_Header.dwDataSize] = 0;
		}

		for(int i=0;i<EncryptNumber;i++)
		{

		    fread( &m_crc2, sizeof(char), 1, m_fp );

		}
	}	
	fclose( m_fp );
	if( !CheckHeader() )	return FALSE;
	if( !CheckCRC() )		return FALSE;
	return TRUE;
}

BOOL CMHFileEx::SaveToTxt( const char* filename )
{
	m_fp = fopen( filename, "wb" );
	if( m_fp )
	if( m_pData )
		fwrite( m_pData, sizeof(char), m_Header.dwDataSize, m_fp );
	fclose( m_fp );

	return TRUE;
}

BOOL CMHFileEx::SaveToTxt()
{
	MakeFullFileName();
	m_fp = fopen( m_szFullFileName, "wb" );
	if( m_fp )
	if( m_pData )
		fwrite( m_pData, sizeof(char), m_Header.dwDataSize, m_fp );
	fclose( m_fp );

	return TRUE;
}

BOOL CMHFileEx::SaveToBin( const char* filename )
{
	if( m_pData )
	{
		m_fp = fopen( filename, "wb" );
		if( m_fp )
		{
			ConvertBin();
			fwrite( &m_Header, sizeof(m_Header), 1, m_fp );

			for(int i=0;i<EncryptNumber;i++)
			{

	     		fread( &m_crc1, sizeof(char), 1, m_fp );	

			}
			fwrite( m_pBinData, sizeof( char ), m_Header.dwDataSize, m_fp );

			for(int i=0;i<EncryptNumber;i++)
			{

	     		fread( &m_crc2, sizeof(char), 1, m_fp );	
			}

		}
		fclose( m_fp );
	}

	return TRUE;
}

BOOL CMHFileEx::SaveToBin()
{
	MakeFullFileName();
	return SaveToBin( m_szFullFileName );
}


BOOL CMHFileEx::CheckHeader()
{
	if( m_Header.dwVersion != ( Encrypts  +m_Header.dwType+m_Header.dwDataSize) ) //weiye   20100415
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
	if( m_Header.dwDataSize > MEGA )
	{
		Clear();
		MessageBox( NULL, "Header DataSize Error!!", "Error!!", MB_OK );
		return FALSE;
	}

	return TRUE;
}

BOOL CMHFileEx::CheckCRC()
{
	char crc = m_Header.dwType;

    int j=0;

	for( DWORD i = 0; i < m_Header.dwDataSize; ++i )
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

BOOL CMHFileEx::ConvertBin()
{
	m_Header.dwVersion = Encrypts ;

	m_Header.dwType = rand()%m_Header.dwDataSize + 1;

	m_Header.dwVersion += (m_Header.dwType+m_Header.dwDataSize);

	m_pBinData = new char[m_Header.dwDataSize+1];

	memcpy( m_pBinData, m_pData, m_Header.dwDataSize+1 );

	m_pBinData[m_Header.dwDataSize] = 0;

	char crc = m_Header.dwType;

	int j=0;

	for( DWORD i = 0; i < m_Header.dwDataSize; ++i )
	{
		m_pBinData[i] += (char)i^m_Encrypts[j];

		if(j>strlen(m_Encrypts)-1)
		{
               j=0;
		}
		if( i%m_Header.dwType == 0 )
		{
			m_pBinData[i] += m_Header.dwType;
		}

		crc += m_pBinData[i];
	}

	m_crc1 = m_crc2 = crc;

	return TRUE;
}

void CMHFileEx::InitFileName( const char* fullfilename )
{
	strcpy( m_szFullFileName, fullfilename );

	int len = strlen(m_szFullFileName);
	int i, j, k;

	for( i = len-1; i >= 0; --i )
	{
		if( m_szFullFileName[i] == '.' )
			j = i;
		if( m_szFullFileName[i] == '\\' )
		{
			k = i;
			break;
		}
	}

	for( i = 0; i < len-j; ++i )
		m_szExt[i] = m_szFullFileName[j+1+i];
	for( i = 0; i < len-k; ++i )
		m_szFileName[i] = m_szFullFileName[k+1+i];	
}

void CMHFileEx::MakeFullFileName()
{
	int len = strlen(m_szFullFileName);
	int lenext = strlen(m_szExt);
	int i, j;

	for( i = len-1; i >= 0; --i )
	{
		if( m_szFullFileName[i] == '.' )
		{
			j = i;
			break;
		}
	}
	for( i = 0; i < lenext; ++i )
		m_szFullFileName[j+1+i] = m_szExt[i];

	m_szFullFileName[j+1+i] = 0;
}

void CMHFileEx::SetData( CString str )
{
	m_FileSize += (strlen( str )-m_Header.dwDataSize)-(m_FileSize-m_Header.dwDataSize);
	if( m_pData )
	{
		delete [] m_pData;
		m_pData = new char[m_FileSize+1];
		memcpy( m_pData, str, m_FileSize );
		m_pData[m_FileSize]='\0';
		m_Header.dwDataSize = str.GetLength();
	}
	else
	{
		m_pData = new char[m_FileSize+1];
		memcpy( m_pData, str, m_FileSize );
		m_pData[m_FileSize]='\0';
		m_Header.dwDataSize = str.GetLength();
	}
}

