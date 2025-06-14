



//////////////////////////////////////////////////////////////////////
// Coolwater.Fire version 2001.1.15
// Ultimate Database Gasoline Engine
// If it does not work correctly, plz enter www.coolwater.fire.com 
// 
// programed by Park Sung Jun 
//

//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DB_H__1E9DC62E_BDF3_4845_B9CC_EE225C7914D5__INCLUDED_)
#define AFX_DB_H__1E9DC62E_BDF3_4845_B9CC_EE225C7914D5__INCLUDED_

#pragma once

#include "stdafx.h"
#include <sql.h>
#include <sqlext.h>
#include <initguid.h>

///////////////////////////////////////////////////////////////////////////////////////////////////////
/* ÁÖÀÇ!!!!!!!!!!!!!!!!!!!!!!
	¾Æ·¡ define µÈ °ªµéÀº ÀÌ °ªÀ» ¹Ù²Û´Ù°í ÇØ¼­ ½ÇÁ¦ ¹Ù²î´Â °ÍÀÌ ¾Æ´Õ´Ï´Ù.
	½ÇÁ¦ °ªÀº DLL ³»ºÎ¿¡ Á¤ÀûÀ¸·Î Á¤ÀÇµÇ¾î ÀÖ±â ‹š¹®¿�?º¯°æÀÌ ºÒ°¡´ÉÇÕ´Ï´Ù.
	ÀÌ´Â Äõ¸® °á°ú¿¡ µû¶ó ¸Þ¸ð¸® ÇÒ´çÇÏ´Â ºÎºÐÀÌ Á¤ÀûÀÎ °Í°ú µ¿ÀûÀÎ °Í µÎ°¡Áö µ¿½Ã¿¡ Æ÷ÇÔÇÏ±â ¶§¹®ÀÔ´Ï´Ù.
*/

#pragma pack(push, 1)



#define STR_LEN 256
#define MSG_BUF_SIZE 256
#define MAX_LENGTH_QUERY 3072
#define MAX_COL_LENGTH 17	//º¸ÅëÀÇ Ä®·³³»¿ëÀÇ ÃÖ´ë ±æÀÌ 
#define MAX_COL_NAME_LENGTH 24	//º¸ÅëÀÇ Ä®·³ÀÌ¸§ÀÇ ÃÖ´ë ±æÀÌ 
#define	MAX_NUM_SQL_CONNECTION			5 //weiye ���ݿ���������� 2015-01-26 Ĭ��Ϊ5
#define MAX_COL_NUM 80  //weiye ���ݿⷵ������ֶ�����չΪ70�� 2018-01-30
#define MAX_ROW_NUM 100 // ÇÑ¹ø Äõ¸®¿¡ ÃÖ´ë·Î ºÒ·¯¿À´Â ·¹ÄÚµå ¼ö 

#define MAX_LARGECOL_LENGTH 6144	//º¸ÅëÀÇ Ä®·³³»¿ëÀÇ ÃÖ´ë ±æÀÌ 
#define MAX_LARGECOL_NUM 20 // Äõ¸® °á°úÀÇ ÃÖ´ë Ä®·³¼ö 

#define MAX_MIDDLECOL_LENGTH 311	//º¸ÅëÀÇ Ä®·³³»¿ëÀÇ ÃÖ´ë ±æÀÌ 
#define MAX_MIDDLECOL_NUM 20 // Äõ¸® °á°úÀÇ ÃÖ´ë Ä®·³¼ö
#define MAX_MIDDLEROW_NUM 20 // ÇÑ¹ø Äõ¸®¿¡ ÃÖ´ë·Î ºÒ·¯¿À´Â ·¹ÄÚµå ¼ö 




///////////////////////////////////////////////////////////////////////////////////////////////////////

typedef void (*VOIDFUNC)(void);

// ¾²·¹µù È°¿ëÀ§ÇÑ ±¸Á¶Ã¼
typedef struct {
	DWORD				dwMessageID;		// ¸Þ¼¼Áö °íÀ¯¾ÆÀÌµð, ¸»ÇÏÀÚ¸é Ã³¸®¹øÈ£.

	BOOL				bNotifyOnlyError;	// ÀÌ °ªÀÌ ÂüÀÌ¸é ÇØ´çÇÔ¼ö ½ÇÇàÈÄ¿¡ ¿¡·¯°¡ ¹ß»ýÇÒ °æ¿ì¿¡¸¸ °íÁö
	LPVOID				pData;				// Ã³¸®¸¦ ¿øÇÏ´Â ÀÔ·Âµ¥ÀÌÅÍÀÇ Æ÷ÀÎÅÍ
	BYTE				bFNNo;				// Æã¼Ç¹øÈ£..°á±¹ ÀÌ·± Ä¡Á¹ÇÑ º¡¹ýÀ» ¾²°Ô µÇ´Â±º..-_-;;
	DWORD				dwResult;			// ÇÊ¿ä¿¡ µû¶ó ÀÌ °ªÀ» ¾´´Ù.

	DWORD				dwID;
} DBMESSAGE, *LPDBMESSAGE;

struct DESC_DBTHREAD
{
	BYTE		bUsingThread;
	BYTE		bUsingDLL;
	BYTE		bUsingEventObject;
	HWND		hWndToPostMessage;
    void		(*ReadDBResult)(DWORD ret,  LPDBMESSAGE pMessage);
	void		(*WriteMessage)(char* pmsg);
	
	DWORD		dwMessageMaxSize;		// DBMESSAGE ±¸Á¶Ã¼ÀÇ ÃÖ´ë Å©±â
	DWORD		dwNumofMessage;			// ÃÖ´ë µ¿½Ã Äõ¸® °¡´É °¹¼ö 
	DWORD		dwResultMaxSize;		// Äõ¸® °á°úÀÇ ÃÖ´ë »çÀÌÁî(select¹®)
	DWORD		dwNumofResult;			// Äõ¸® °á°úÀÇ ÃÖ´ë °¹¼ö  (select¹®)
	DWORD		dwSizeofStack;			// Äõ¸®½Ã ÀÔ·ÂÇÑ ÀÎÀÚ¸¦ À§ÇÑ ¸Þ¸ð¸® Å©±â 
	DWORD		dwDBMessageID;

	char		Default_User_table[128];
	char		Default_User_account[128];
	char		Default_User_pw[128];
	char		Default_User_guid[128];
	char		AdminID[128];

	char		ClientID[128];
	char		AdminPW[128];
	char		ClientPW[128];
	char		DBName[128];
	char		AdminCursor[128];
	char		ClientCursor[128];

};



typedef struct {
	LPDBMESSAGE	lpDBMessage;
	DWORD		Ret;
} DBRESULTMESSAGE, *LPDBRESULTMESSAGE;



typedef struct
{
	char Data1[MAX_COL_LENGTH];
} DBDATA1, *LPDATA1;
typedef struct
{
	char Data1[MAX_COL_LENGTH];
	char Data2[MAX_COL_LENGTH];
} DBDATA2, *LPDATA2;
typedef struct
{
	char Data1[MAX_COL_LENGTH];
	char Data2[MAX_COL_LENGTH];
	char Data3[MAX_COL_LENGTH];
} DBDATA3, *LPDATA3;
typedef struct
{
	char Data1[MAX_COL_LENGTH];
	char Data2[MAX_COL_LENGTH];
	char Data3[MAX_COL_LENGTH];
	char Data4[MAX_COL_LENGTH];
} DBDATA4, *LPDATA4;
typedef struct
{
	char Data1[MAX_COL_LENGTH];
	char Data2[MAX_COL_LENGTH];
	char Data3[MAX_COL_LENGTH];
	char Data4[MAX_COL_LENGTH];
	char Data5[MAX_COL_LENGTH];
} DBDATA5, *LPDATA5;
typedef struct
{
	char Data1[MAX_COL_LENGTH];
	char Data2[MAX_COL_LENGTH];
	char Data3[MAX_COL_LENGTH];
	char Data4[MAX_COL_LENGTH];
	char Data5[MAX_COL_LENGTH];
	char Data6[MAX_COL_LENGTH];
} DBDATA6, *LPDATA6;
typedef struct
{
	char Data1[MAX_COL_LENGTH];
	char Data2[MAX_COL_LENGTH];
	char Data3[MAX_COL_LENGTH];
	char Data4[MAX_COL_LENGTH];
	char Data5[MAX_COL_LENGTH];
	char Data6[MAX_COL_LENGTH];
	char Data7[MAX_COL_LENGTH];
} DBDATA7, *LPDATA7;
typedef struct
{
	char Data1[MAX_COL_LENGTH];
	char Data2[MAX_COL_LENGTH];
	char Data3[MAX_COL_LENGTH];
	char Data4[MAX_COL_LENGTH];
	char Data5[MAX_COL_LENGTH];
	char Data6[MAX_COL_LENGTH];
	char Data7[MAX_COL_LENGTH];
	char Data8[MAX_COL_LENGTH];
} DBDATA8, *LPDATA8;
typedef struct
{

	char Data1[MAX_COL_LENGTH];
	char Data2[MAX_COL_LENGTH];
	char Data3[MAX_COL_LENGTH];
	char Data4[MAX_COL_LENGTH];
	char Data5[MAX_COL_LENGTH];
	char Data6[MAX_COL_LENGTH];
	char Data7[MAX_COL_LENGTH];
	char Data8[MAX_COL_LENGTH];
	char Data9[MAX_COL_LENGTH];
} DBDATA9, *LPDATA9;
typedef struct
{
	char Data1[MAX_COL_LENGTH];
	char Data2[MAX_COL_LENGTH];
	char Data3[MAX_COL_LENGTH];

	char Data4[MAX_COL_LENGTH];
	char Data5[MAX_COL_LENGTH];
	char Data6[MAX_COL_LENGTH];
	char Data7[MAX_COL_LENGTH];
	char Data8[MAX_COL_LENGTH];
	char Data9[MAX_COL_LENGTH];
	char Data10[MAX_COL_LENGTH];
} DBDATA10, *LPDATA10;

typedef struct
{
	TCHAR	szDBName[STR_LEN];			// data source name, configured through control panel
	TCHAR	szID[STR_LEN];				// Login user name
	TCHAR	szPW[STR_LEN];				// Login user password
	SQLHDBC	hDBC;						// handle to an ODBC database connection
	HSTMT	hStmt;						// an ODBC statement handle, for use with SQLExecDirect
	TCHAR	szCursorName[STR_LEN];		// Ä¿¼­ ÀÌ¸§ 
	int		nCursorNum;					// ÇöÀç Ä¿¼­ÀÇ À§Ä¡
	HSTMT	hScrollStmt;
} DBCON, *LPDBCON;

// ÀÚÀ¯ Äõ¸®¹® ½ÇÇà½Ã¿¡ ¸®ÅÏµÇ´Â Æ÷ÀÎÅÍ ±¸Á¶Ã¼ 
typedef struct
{
	UCHAR   Data[MAX_COL_NUM][MAX_COL_LENGTH];  // Results Data Array
	SDWORD  Length[MAX_COL_NUM];			// Results Data Length Array
	int		nNum;							// Ä®·³ÀÇ °¹¼ö 
} QUERYST, *LPQUERY;
// ÀÚÀ¯ Äõ¸®¹® ½ÇÇà½Ã¿¡ ¸®ÅÏµÇ´Â Æ÷ÀÎÅÍ ±¸Á¶Ã¼ 
typedef struct
{
	UCHAR   Data[MAX_LARGECOL_NUM][MAX_LARGECOL_LENGTH];  // Results Data Array
	SDWORD  Length[MAX_LARGECOL_NUM];			// Results Data Length Array
	int		nNum;							// Ä®·³ÀÇ °¹¼ö 
} LARGEQUERYST, *LPLARGEQUERY;
// ÀÚÀ¯ Äõ¸®¹® ½ÇÇà½Ã¿¡ ¸®ÅÏµÇ´Â Æ÷ÀÎÅÍ ±¸Á¶Ã¼ 
typedef struct
{
	UCHAR   Data[MAX_MIDDLECOL_NUM][MAX_MIDDLECOL_LENGTH];  // Results Data Array
	SDWORD  Length[MAX_MIDDLECOL_NUM];			// Results Data Length Array
	int		nNum;							// Ä®·³ÀÇ °¹¼ö 
} MIDDLEQUERYST, *LPMIDDLEQUERY;
// ¸Þ¸ð¸® µ¿ÀûÇÒ´çÀ» À§ÇÑ Äõ¸® ±¸Á¶Ã¼
typedef struct
{
	int		nNum;							// Ä®·³ÀÇ °¹¼ö 
	LPVOID	pData;
	LPVOID	pLength;
} QUERYDA, *LPQUERYDA;

typedef struct
{
	UCHAR	 szData[MAX_COL_LENGTH];
} DBSTRING, *LPDBSTRING;

typedef struct
{
	SDWORD	 pLeng;
} DBSDWORD, *LPDBSDWORD;


typedef struct
{
	int			nNumData;
	int			nColNum;
	LPVOID		pData[MAX_ROW_NUM];
	SQLSMALLINT QueryType[MAX_COL_NUM];
} BINDST, *LPBIND;

typedef struct
{
	UCHAR		Data[MAX_COL_NUM][MAX_COL_LENGTH];  // Results Data Array

	SDWORD		Length[MAX_COL_NUM];				// Results Data Length Array
	SDWORD		LengthRead[MAX_COL_NUM];			// Results Data Length Array
} ROWWISEBIND, *LPROWWISEBIND;


// ¾²·¹µù¿ë ÀÎÀÚ ±¸Á¶Ã¼ 
typedef struct
{
	LPDBCON pCon;
	char	szBuffer[MAX_LENGTH_QUERY];
	DWORD	dwNumRow;
} DBFreeQuery, *LPDBFreeQuery;
typedef struct
{
	LPDBCON pCon;
	char	szTable[MAX_COL_NAME_LENGTH];
	DWORD	dwValue;
	char	lpCol[MAX_COL_NAME_LENGTH];
	SQLSMALLINT	sqltype;
} DBSingleQuery, *LPDBSingleQuery;
typedef struct
{
	LPDBCON pCon;
	char	szBuffer[MAX_LENGTH_QUERY];
	LPBIND	lpBind;
	int		StSize;
	SQLSMALLINT	sqlType;
	int		rowset;
} DBGenerateResultSet, *LPDBGenerateResultSet;
typedef struct
{
	LPDBCON pCon;
	char	szQuery[MAX_LENGTH_QUERY];

} DBExcuteIUD, *LPDBExcuteIUD;
typedef struct
{
	LPDBCON pCon;
	char	szTableName[MAX_COL_NAME_LENGTH];
	char	lpCol[MAX_COL_NAME_LENGTH];
	char	lpValue[MAX_COL_LENGTH];
	char	DestCol[MAX_COL_NAME_LENGTH];
	char	DestValue[MAX_COL_LENGTH];
} DBUpdateRecord, *LPDBUpdateRecord;
typedef struct
{
	LPDBCON pCon;
	char	szTableName[MAX_COL_NAME_LENGTH];
	char	lpCol[MAX_COL_NAME_LENGTH];
	char	lpValue[MAX_COL_LENGTH];
} DBDeleteRecord, *LPDBDeleteRecord;
typedef struct

{
	LPDBCON pCon;
	char	szTableName[MAX_COL_NAME_LENGTH];
	LPVOID	lpCol;
	LPVOID	lpValue;
	BYTE	bNumData;
} DBInsertRecord, *LPDBInsertRecord;
typedef struct
{
	LPDBCON pCon;
	char	szTableName[MAX_COL_NAME_LENGTH];
	char	pColID[MAX_COL_NAME_LENGTH];
	char	pColPW[MAX_COL_NAME_LENGTH];
	char	pID[MAX_COL_NAME_LENGTH];
	char	pPW[MAX_COL_NAME_LENGTH];
	char	pRetCol[MAX_COL_NAME_LENGTH];
	LPVOID	pRetValue;
	int		iLength;
	SQLSMALLINT sqlType;
} DBLogin, *LPDBLogin;
typedef struct
{
	LPDBCON pCon;
	char	pID[MAX_COL_NAME_LENGTH];
	char	pPW[MAX_COL_NAME_LENGTH];
	LPVOID	pRetValue;
} DBSimpleLogin, *LPDBSimpleLogin;

typedef struct
{
	LPDBCON pCon;
	char	szQuery[MAX_LENGTH_QUERY];
	int		nSize;
	LPVOID	pPara;
	long	nBuffersize;
	LPVOID	pSettingData;
} DBSaveDataByPtr, *LPDBSaveDataByPtr;

typedef struct
{
	LPDBCON pCon;
	char	szQuery[MAX_LENGTH_QUERY];
	int		nSize;
	LPVOID	pData;
} DBLoadDataByPtr, *LPDBLoadDataByPtr;

#pragma pack(pop)


#endif // !defined(AFX_DB_H__1E9DC62E_BDF3_4845_B9CC_EE225C7914D5__INCLUDED_)



