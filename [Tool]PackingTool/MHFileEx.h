#ifndef __MHFILEEX_H__
#define __MHFILEEX_H__

#include <stdio.h>

#define		MEGA		1024*1024*10

/*#define    EncryptNumber	17                       //weiye Ñ«??èËÎ¥?43???äÈ??!
#define    Encrypto    20180930
#define    Encrypts	   20181117*/

struct MHFILE_HEADER
{
	DWORD	dwVersion;	// version
	DWORD	dwType;		// file ?×ÀÖØ?
	DWORD	dwDataSize;	// data size
};

struct DOF_HEADER
{
	DWORD	dwVersion;
	DWORD	dwDataSize;
	DWORD	dwSeed;
};

struct DOF_TAIL
{
	DWORD	dwVersion;
	DWORD	dwSeed;
};

class CMHFileEx
{
protected:
	FILE*			m_fp;
	char			m_szFullFileName[256];
	char			m_szFileName[256];
	char			m_szExt[256];

	MHFILE_HEADER	m_Header;
	char			m_crc1;
	char			m_crc2;
	char*			m_pData;
	char*			m_pBinData;

	char         m_Encrypto[9];
	char         m_Encrypts[9];
	
public:
	CMHFileEx();
	virtual ~CMHFileEx();

	void	Clear();
	BOOL	New( const char* fullfilename );
	BOOL	OpenTxt( const char* fullfilename );
	BOOL	OpenBin( const char* fullfilename );
	BOOL	SaveToTxt( const char* filename );
	BOOL	SaveToTxt();
	BOOL	SaveToBin( const char* filename );
	BOOL	SaveToBin();
	BOOL	CheckCRC();
	BOOL	ConvertBin();
	BOOL	CheckHeader();

	void	InitFileName( const char* fullfilename );				
	char*	GetFullFileName()								{ return m_szFullFileName; }
	char*	GetFileName()									{ return m_szFileName; }
	char*	GetExt()										{ return m_szExt; }
	void	SetFullFileName( const char* fullfilename )		{ strcpy( m_szFullFileName, fullfilename ); }
	void	SetFileName( const char* filename )				{ strcpy( m_szFileName, filename ); }
	void	SetExt( const char* ext )						{ strcpy( m_szExt, ext ); }
	void	MakeFullFileName();
	char*	GetData()										{ return m_pData; }
	void	SetData( CString str );
	MHFILE_HEADER	GetHeader()								{ return m_Header; }

};

#endif