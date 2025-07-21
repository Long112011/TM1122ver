// MHFile.h: interface for the CMHFile class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MHFILE_H__CF584ED8_B10C_4E1B_A1CF_4E098D34E802__INCLUDED_)
#define AFX_MHFILE_H__CF584ED8_B10C_4E1B_A1CF_4E098D34E802__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//090811 - PANG -
#ifdef _AES_FILE_
#include "AESFile.h"
#endif
//////////////////
#define MHFILE_FLAG_DONOTDISPLAY_NOTFOUNDERROR 0x00000001

struct MHFILE_HEADER
{
	DWORD	dwVersion;	// version
	DWORD	dwType;		// file Á¾·ù
	DWORD	dwFileSize;	// data size
};

enum
{
	MHFILE_NORMALMODE,
	MHFILE_PACKEDFILE,
	MHFILE_ENGINEPACKEDFILE,
};

//2008. 10. 17. CBH
enum HACKCHECK_CALCULATION_FLAG
{
	eHACKCHECK_CALCULATION_NONE = 0,
	eHACKCHECK_CALCULATION_ADD,
	eHACKCHECK_CALCULATION_SUB,
	eHACKCHECK_CALCULATION_MULTI,
	eHACKCHECK_CALCULATION_DIVISION,
};

//2008. 10. 20. CBH 
enum HACKCHECK_CALCULATION_VALUE
{
	HACKCHECK_CALCULATION_VALUE_ADD = 1000,
	HACKCHECK_CALCULATION_VALUE_SUB = 100,
	HACKCHECK_CALCULATION_VALUE_MULTI = 3,
	HACKCHECK_CALCULATION_VALUE_DIVISION = 2,
};

//extern int g_DataHackCheckValue;
class CMHFile  
{
	FILE* fp;
public:
	CMHFile();
	virtual ~CMHFile();

	BOOL Init(char* filename,char* mode, DWORD dwFlag = 0,DWORD dwCulculationFlag = eHACKCHECK_CALCULATION_NONE);
	void Release();

	char* GetStringInQuotation();
	void GetStringInQuotation(char* pBuf);
	char* GetString();
	void GetString(char* pBuf);
	void GetLine(char * pBuf, int len);
	void GetLineX(char * pBuf, int len);
	int GetInt();
	LONG GetLong();
	float GetFloat();
	DWORD GetDword();
	WORD GetWord();
	BYTE GetByte();
	BOOL GetBool();
	BOOL Seek(int n);	
	void GetFloat(float* pArray,int count);
	void GetWord(WORD* pArray,int count);
	void GetDword(DWORD* pArray,int count);

	BOOL IsEOF();
	BOOL IsInited();

	void SetString(char * str);
protected:
	DWORD			m_bReadMode;
	int				m_Dfp;
	MHFILE_HEADER	m_Header;				// ÆÄÀÏ Çì´õ
	char			m_crc1;					// ¾ÏÈ£È­ °Ë»ç µ¥ÀÌÅÍ
	char			m_crc2;					// ¾ÏÈ£È­ °Ë»ç µ¥ÀÌÅÍ
	char*			m_pData;				// ½ÇÁ¦ µ¥ÀÌÅÍ
	BOOL			m_bInit;

private:
	//2008. 10. 17. CBH -
	void	CulculationCRC(int nCRCValue, DWORD dwCulculationFlag);

public:
	BOOL	OpenBin( char* filename, DWORD dwCulculationFlag = eHACKCHECK_CALCULATION_NONE );
	void	Clear();
	BOOL	CheckHeader();
	BOOL	CheckCRC();
	char*	GetData()		{ return m_pData; }
	DWORD	GetDataSize()	{ return m_Header.dwFileSize; }
	//090811 - PANG - 
	char	GetCrc1() const	{ return m_crc1; }
	char	GetCrc2() const	{ return m_crc2; }

#ifdef _AES_FILE_
private:
	cAESFile m_AESFile;
#endif
	//////////////////
};


#endif // !defined(AFX_MHFILE_H__CF584ED8_B10C_4E1B_A1CF_4E098D34E802__INCLUDED_)
