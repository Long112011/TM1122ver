#pragma once
#ifdef _AES_FILE_
#define MAX_KEY_LENGTH		512
struct AESFILE_HEADER
{
	DWORD	dwVersion;	
	DWORD	dwType;		
	DWORD	dwDataSize;	
};
const DWORD dwVersion[] = 
{
	20040308,
	20090506,
	20101117,
	20150413,
	15042016,
	20230131,//baru
};
const unsigned char aKey[][MAX_KEY_LENGTH] = 
{
	"",
	"E76B2413958B00E193A1",
	"A15H3825287T19G082K8",
	"SI14J6I43KJL2VN1G69D",
	"590A34E294F093234CEA",
	"Z^KJ(11S82@4Q9^7*VF!",//baru
};
class cAESFile
{
	int	m_nVersionCount;
	DWORD m_dwCurVersion;
	DWORD m_dwLastVersion;
	unsigned char m_aCurKey[MAX_KEY_LENGTH];
	unsigned char m_aLastKey[MAX_KEY_LENGTH];
public:
	BOOL SetBinVersion( DWORD BinVersion );
	BOOL IsExistKey();
public:
	cAESFile();
	virtual ~cAESFile();
	void Init();
	BOOL AESEncryptData(FILE* fp, char* pFileName, char* pData, DWORD dwDataSize);
	BOOL AESDecryptData(FILE* fp, char* pData, DWORD dwDataSize, DWORD dwType);
};
#endif