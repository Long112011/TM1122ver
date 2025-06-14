



// MHFile.h: interface for the CMHFile class.
//
//////////////////////////////////////////////////////////////////////


#if !defined(AFX_MHFILE_H__CF584ED8_B10C_4E1B_A1CF_4E098D34E802__INCLUDED_)
#define AFX_MHFILE_H__CF584ED8_B10C_4E1B_A1CF_4E098D34E802__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../4dyuchiGRX_common/IFileStorage.h"

#define MHFILE_FLAG_DONOTDISPLAY_NOTFOUNDERROR		0x00000001
#define MHFILE_FLAG_DONOTCHANGE_DIRECTORY_TOROOT	0x00000010
/*
#define    EncryptNumber	43                       //weiye 修改使用43跌代?��?!
#define    Encrypto    20180115
#define    Encrypts	   20180120
*/
/*#define    EncryptNumber	17                      //CoNaN 修改使用19跌代?��?!
#define    Encrypto    20180930
#define    Encrypts	   20181117*/
/*
struct MHFILE_HEADER
{
	DWORD	Version;	// version
	DWORD	Type;		// file ?¾·ù
	DWORD	FileSize;	// file size
	DWORD	Etc1;		// ¿¹ºñ data

	DWORD	Etc2;
};
*/
struct MHFILE_HEADER
{
	DWORD	dwVersion;	// version
	DWORD	dwType;		// file ?¾·ù
	DWORD	FileSize;	// data size
};

enum{
	MHFILE_NORMALMODE,
	MHFILE_PACKEDFILE,
	MHFILE_ENGINEPACKEDFILE,
};
class line_node;

class CMHFile  
{
	FILE* fp;
public:
	CMHFile();
	virtual ~CMHFile();

	BOOL Init(char* filename,char* mode,DWORD dwFlag = 0);
	void Release();

	char* GetStringInQuotation();
	void GetStringInQuotation(char* pBuf);
	char* GetString();
	int GetString(char* pBuf);
	void GetLine(char * pBuf, int len);
	void GetLineX(char * pBuf, int len);	// ??¼®?³¸® µ? °÷ ???»¶§ »ç¿ë
	int GetInt();
	LONG GetLong();
	float GetFloat();
	EXPTYPE GetDword();
	EXPTYPE GetWord();

	BYTE GetByte();
	BOOL GetBool();
	BOOL GetHex(DWORD* pOut);
	DWORD GetExpPoint();
	EXPTYPE GetExpPoint64();	// magi82 - Exp Variable Type Change(070523)
	LEVELTYPE GetLevel();
	

	void GetFloat(float* pArray,int count);
	void GetWord(WORD* pArray,int count);
	void GetDword(DWORD* pArray,int count);
		
	EXPTYPE GetWord(EXPTYPE* pOut);

	BOOL Seek(int n);
	

	BOOL IsEOF();


	BOOL IsInited();
	
	void SetString(char * str);

	// LBS 03.10.22 ??¹®??¾¿ °?»ç??±â ?§??¼­..
	int GetChar();

// jsd - binary file ??±â ?§??
protected:
	// yh
	DWORD			m_bReadMode;
	

	// ¿£?ø???·¿ë
	I4DyuchiFileStorage* m_pFileStorage;
	void* m_pFilePointer;

	int				m_Dfp;

	MHFILE_HEADER	m_Header;				// ???? ?ì´õ
	char			m_crc1;					// ¾??£?­ °?»ç µ¥????
	char			m_crc2;					// ¾??£?­ °?»ç µ¥????
	char*			m_pData;				// ½??¦ µ¥????
	BOOL			m_bInit;
	char			m_sFileName[MAX_PATH];

	char         m_Encrypto[9];     //weiye 定�??��??��?密钥  2018-01-13
	char         m_Encrypts[9];     //weiye 定�??��?保�?密钥  2018-01-13


public:
	BOOL	OpenBin( char* filename );
	void	Clear();
	BOOL	CheckHeader();
	BOOL	CheckCRC();
	char*	GetFileName()	{ return m_sFileName; }
	char*	GetData()		{ return m_pData;}
	DWORD	GetDataSize()	{ return m_Header.FileSize; }

	//BOOL	CheckLineEnd(int lineNum);	//SW071113

	char* BIG5ToGB2312(const char* szBIG5String);
	char* UnicodeToGB2312(const wchar_t* szUnicodeString);
	wchar_t* BIG5ToUnicode(const char* szBIG5String);
	char* GB2312ToBIG5(const char* szGBString);
	wchar_t* GB2312ToUnicode(const char* szGBString);
	char* UnicodeToBIG5(const wchar_t* szUnicodeString);

	WORD    GetOperationSystemType();
//

};


//void _parsingKeywordString(char * inString, char ** outString);
//void _parsingKeywordStringEx(char * inString, line_node ** Top);

#endif // !defined(AFX_MHFILE_H__CF584ED8_B10C_4E1B_A1CF_4E098D34E802__INCLUDED_)


