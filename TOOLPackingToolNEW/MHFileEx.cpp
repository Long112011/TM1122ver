#define __MHFILEEX_C__
#include "stdafx.h"
#include "MHFileEx.h"
DWORD	m_FileSize;
//BOOL	jTweak;
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

	m_fp = fopen("_","r");
	if(m_fp)
	{
		//jTweak=TRUE;
		fclose(m_fp);
	}
	sprintf(m_Encrypto,"%d",Encrypto);
	sprintf(m_Encrypts,"%d",Encrypts);
	

	memset( &m_DOFHeader, 0, sizeof(DOF_HEADER) );
	memset( &m_DOFTail, 0, sizeof(DOF_TAIL) );
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
		m_DOFHeader.dwDataSize = m_Header.dwDataSize;//sial
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
	
#ifdef _AES_FILE_
		if( m_fp )
		{
			Clear();
			fread( &m_Header, sizeof(m_Header), 1, m_fp );	// header

			if(!m_AESFile.SetBinVersion(m_Header.dwVersion - (m_Header.dwType+m_Header.dwDataSize)))
			{
				//ASSERT(!"Bin-File Version Error!");
				MessageBox( NULL, "Bin-File Version Error!", NULL, MB_OK );
				return FALSE;
			}

			if(m_AESFile.IsExistKey())
			{
				m_pData = new char[m_FileSize];
				if(m_pData == NULL)
				{
					//ASSERT(!"memory allocation failed - m_pData");
					return FALSE;
				}
				memset( m_pData, 0, m_FileSize );

				if(!m_AESFile.AESDecryptData(m_fp, m_pData, m_Header.dwDataSize, m_Header.dwType))
				{
					char str[512];
					sprintf(str, "File Open Error : %s",fullfilename);
					MessageBox( NULL, str, NULL, MB_OK );
					return FALSE;
				}
				fclose( m_fp );
				m_fp = NULL;

			}
			else
			{
				fread( &m_crc1, sizeof(char), 1, m_fp );			// crc1
				if( m_Header.dwDataSize )						// data
				{
					m_pData = new char[m_FileSize];
					memset( m_pData, 0, m_FileSize );
					fread( m_pData, sizeof(char), m_Header.dwDataSize, m_fp );
					m_pData[m_Header.dwDataSize] = 0;
				}
				fread( &m_crc2, sizeof(char), 1, m_fp );			// crc2
				
				fclose( m_fp );
				m_fp = NULL;

				if( !CheckHeader() )	return FALSE;
				if( !CheckCRC() )		return FALSE;

				m_DOFHeader.dwDataSize = m_Header.dwDataSize;
			}
		}
		else
		{
			return FALSE;
		}
#else
	
		if( m_fp )
		{
			Clear();
			fread( &m_Header, sizeof(m_Header), 1, m_fp );	
			for(int i=0;i<EncryptNumber;i++)
			{
	     		fread( &m_crc1, sizeof(char), 1, m_fp );	
			}
			if( m_Header.dwDataSize )						
			{
				m_pData = new char[m_FileSize];
				memset( m_pData, 0, m_FileSize );
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
		m_DOFHeader.dwDataSize = m_Header.dwDataSize;//sial
#endif
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
#ifdef _AES_FILE_
			char fname[512];
			m_pBinData = new char[m_Header.dwDataSize+1];
			memcpy( m_pBinData, m_pData, m_Header.dwDataSize+1 );
			m_pBinData[m_Header.dwDataSize] = 0;
			
			strcpy(fname, filename);
			if(!m_AESFile.AESEncryptData(m_fp, fname, m_pBinData, m_Header.dwDataSize))
			{
				MessageBox( NULL, "Failed Save To Bin", NULL, MB_OK );
				return FALSE;
			}
#else
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
#endif
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
#ifdef _AES_FILE_
	if( m_Header.dwVersion != ( 20040308  +m_Header.dwType+m_Header.dwDataSize) )
#else
	if( m_Header.dwVersion != ( Encrypts  +m_Header.dwType+m_Header.dwDataSize) )
#endif
	{
		Clear();
		MessageBox(NULL, _T("Header Version Error!!"), _T("Error!!"), MB_OK);
		return FALSE;
	}
	if( m_Header.dwType == 0 )
	{
		Clear();
		MessageBox(NULL, _T("Header Type Error!!"), _T("Error!!"), MB_OK);
		return FALSE;
	}
	if( m_Header.dwDataSize > m_FileSize )
	{
		Clear();
		MessageBox(NULL, _T("Header DataSize Error!!"), _T("Error!!"), MB_OK);
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
#ifdef _AES_FILE_
		m_pData[i] -= (char)i;
		if( i%m_Header.dwType == 0 )
			m_pData[i] -= m_Header.dwType;
#else
		m_pData[i] -= ((char)i^m_Encrypto[j]);
		if(j>=strlen(m_Encrypto)-1)
		{
			j = 0;
		}
		if( i%m_Header.dwType == 0 )
		{
			m_pData[i] -= m_Header.dwType;
		}
#endif
	}
	return TRUE;
}
BOOL CMHFileEx::ConvertBin()
{
#ifdef _AES_FILE_
	m_Header.dwVersion = 20040308;
#else
	m_Header.dwVersion = Encrypts;
#endif
	if( m_Header.dwDataSize == 0 )
		m_Header.dwType = 1;
	else
		m_Header.dwType = rand()%m_Header.dwDataSize + 1;
	m_Header.dwVersion += (m_Header.dwType+m_Header.dwDataSize);
	m_pBinData = new char[m_Header.dwDataSize+1];
	memcpy( m_pBinData, m_pData, m_Header.dwDataSize+1 );
	m_pBinData[m_Header.dwDataSize] = 0;
	char crc = m_Header.dwType;
	int j = 0;
	for( DWORD i = 0; i < m_Header.dwDataSize; ++i )
	{
		if(m_Encrypts==0)
		{
			m_pBinData[i] += ((char)i^m_Encrypts[j]);
			if(j>=strlen(m_Encrypts)-1)
				j = 0;
			if( i%m_Header.dwType == 0 )
				m_pBinData[i] += m_Header.dwType;
		}
		else
		{
			m_pBinData[i] += ((char)i^m_Encrypts[j]);
			if(j>=strlen(m_Encrypts)-1)
				j = 0;
			if( i%m_Header.dwType == 0 )
				m_pBinData[i] += m_Header.dwType;
		}
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
void CMHFileEx::SetData(const char * str)
{
	m_FileSize += (strlen( str )-m_Header.dwDataSize)-(m_FileSize-m_Header.dwDataSize);
	if( m_pData )
	{
		delete [] m_pData;
		m_pData = new char[m_FileSize+1];
		memcpy( m_pData, str, m_FileSize );
		m_pData[m_FileSize]='\0';
		m_Header.dwDataSize = strlen(str);// str.GetLength();
	}
	else
	{
		m_pData = new char[m_FileSize+1];
		memcpy( m_pData, str, m_FileSize );
		m_pData[m_FileSize]='\0';
		m_Header.dwDataSize = strlen(str);//str.GetLength();
	}
}
BOOL CMHFileEx::MakeDOFName()
{
	int len = strlen(m_szFullFileName);
	int lenext = strlen(m_szDOF);
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
		m_szFullFileName[j+1+i] = m_szDOF[i];
	m_szFullFileName[j+1+i] = 0;
	return TRUE;
}
BOOL CMHFileEx::SaveToDOF()
{
	MakeDOFName();
	return SaveToDOF( m_szFullFileName );
}
BOOL CMHFileEx::SaveToDOF( const char* filename )
{
	if( m_pData )
	{
		m_fp = fopen( filename, "wb" );
		if( m_fp )
		{
			ConvertDOF();
			fwrite( &m_DOFHeader, sizeof(m_DOFHeader), 1, m_fp );
			fwrite( &m_crc1, sizeof( char ), 1, m_fp );
			fwrite( m_pBinData, sizeof( char ), m_DOFHeader.dwDataSize, m_fp );
			fwrite( &m_DOFTail, sizeof( m_DOFTail ), 1, m_fp );
		}
		fclose( m_fp );
	}
	return TRUE;
}
BOOL CMHFileEx::ConvertDOF()
{
	m_DOFHeader.dwSeed = rand()%m_DOFHeader.dwDataSize;
	m_DOFTail.dwSeed = rand()%m_DOFHeader.dwDataSize;
	m_DOFHeader.dwVersion = (DWORD)DOFHEADER + m_DOFHeader.dwSeed + m_DOFHeader.dwDataSize;
	m_DOFTail.dwVersion = (DWORD)DOFTAIL + m_DOFTail.dwSeed;
	m_pBinData = new char[m_DOFHeader.dwDataSize];
	memcpy( m_pBinData, m_pData, m_DOFHeader.dwDataSize );
	m_crc1 = 0;
	for( DWORD i = 0; i < m_DOFHeader.dwDataSize; ++i )
	{
		m_pBinData[i] += ((char)i + m_DOFHeader.dwSeed + m_DOFTail.dwSeed);
		m_crc1 += m_pBinData[i];
	}
	return TRUE;
}
BOOL CMHFileEx::OpenDOF( const char* fullfilename )
{
	InitFileName( fullfilename );
	m_fp = fopen( fullfilename, "rb" );
	if( m_fp )
	{
		Clear();
		fread( &m_DOFHeader, sizeof(m_DOFHeader), 1, m_fp );	
		fread( &m_crc1, sizeof(char), 1, m_fp );				
		if( m_DOFHeader.dwDataSize )							
		{
			m_pData = new char[m_FileSize];
			memset( m_pData, 0, m_FileSize);
			fread( m_pData, sizeof(char), m_DOFHeader.dwDataSize, m_fp );
			m_pData[m_DOFHeader.dwDataSize] = 0;
		}
		fread( &m_DOFTail, sizeof(m_DOFTail), 1, m_fp );		
	}	
	fclose( m_fp );
	if( !CheckDOF() )	return FALSE;
	return TRUE;
}
BOOL CMHFileEx::CheckDOF()
{
	if( m_DOFHeader.dwVersion != ((DWORD)DOFHEADER + m_DOFHeader.dwSeed + m_DOFHeader.dwDataSize) )
	{
		Clear();
		MessageBox(NULL, _T("Header Version Error!!"), _T("Error!!"), MB_OK);
		return FALSE;
	}
	if( m_DOFHeader.dwSeed == 0 )
	{
		Clear();
		MessageBox(NULL, _T("Header Seed Error!!"), _T("Error!!"), MB_OK);
		return FALSE;
	}
	if( m_DOFHeader.dwDataSize > m_FileSize )
	{
		Clear();
		MessageBox(NULL, _T("Header Datasize Error!!"), _T("Error!!"), MB_OK);
		return FALSE;
	}
	if( m_DOFTail.dwVersion != ((DWORD)DOFTAIL + m_DOFTail.dwSeed) )
	{
		Clear();
		MessageBox(NULL, _T("Header Version Error!!"), _T("Error!!"), MB_OK);
		return FALSE;
	}
	char crc = 0;
	for( DWORD i = 0; i < m_DOFHeader.dwDataSize; ++i )
	{
		crc += m_pData[i];
		m_pData[i] -= ((char)i + m_DOFHeader.dwSeed + m_DOFTail.dwSeed);
	}
	if( m_crc1 != crc )
	{
		Clear();
		MessageBox(NULL, _T("CheckCrc Error!!"), _T("Error!!"), MB_OK);
		return FALSE;
	}
	return TRUE;
}