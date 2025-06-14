




// MHFile.h: interface for the CMHFile class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MHFILE_H__CF584ED8_B10C_4E1B_A1CF_4E098D34E802__INCLUDED_)
#define AFX_MHFILE_H__CF584ED8_B10C_4E1B_A1CF_4E098D34E802__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define MHFILE_FLAG_DONOTDISPLAY_NOTFOUNDERROR 0x00000001

/*#define    EncryptNumber	17                       //weiye 修改使用43跌代加密!
#define    Encrypto    20180930
#define    Encrypts	   20181117*/

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

class CMHFile  
{
	FILE* fp;
public:
	CMHFile();
	virtual ~CMHFile();

	BOOL Init(char* filename,char* mode, DWORD dwFlag = 0);
	void Release();

	char* GetStringInQuotation();
	void GetStringInQuotation(char* pBuf);
	char* GetString();
	void GetString(char* pBuf);
	void GetLine(char * pBuf, int len);

	void GetLineX(char * pBuf, int len);	// ÁÖ¼®Ã³¸® µÈ °÷ ÀÐÀ»¶§ »ç¿ë
	int GetInt();
	LONG GetLong();
	float GetFloat();
	DWORD GetDword();
	WORD GetWord();

	BYTE GetByte();
	BOOL GetBool();
	BOOL GetExpPoint();
	BOOL GetLevel();
	BOOL Seek(int n);	
	
	void GetFloat(float* pArray,int count);
	void GetWord(WORD* pArray,int count);

	void GetDword(DWORD* pArray,int count);

	BOOL IsEOF();

	
	BOOL IsInited();

	void SetString(char * str);
	
protected:	// JSD - binary file
	DWORD			m_bReadMode;

	int				m_Dfp;
	MHFILE_HEADER	m_Header;				// ÆÄÀÏ Çì´õ
	char			m_crc1;					// ¾ÏÈ£È­ °Ë»ç µ¥ÀÌÅÍ
	char			m_crc2;					// ¾ÏÈ£È­ °Ë»ç µ¥ÀÌÅÍ
	char*			m_pData;				// ½ÇÁ¦ µ¥ÀÌÅÍ
	BOOL			m_bInit;

	char         m_Encrypto[9];     //weiye 定义加密打开密钥  2018-01-13
	char         m_Encrypts[9];     //weiye 定义加密保存密钥  2018-01-13

public:
	BOOL	OpenBin( char* filename );
	void	Clear();
	BOOL	CheckHeader();
	BOOL	CheckCRC();
	char*	GetData()		{ return m_pData; }
	DWORD	GetDataSize()	{ return m_Header.dwFileSize; }


};


#endif // !defined(AFX_MHFILE_H__CF584ED8_B10C_4E1B_A1CF_4E098D34E802__INCLUDED_)



