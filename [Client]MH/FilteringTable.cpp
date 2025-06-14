#include "stdafx.h"
#include "FilteringTable.h"
#include "MHFile.h"
#ifdef _MHCLIENT_
#include "MainGame.h"
extern HWND _g_hWnd;
GLOBALTON(CFilteringTable)
#endif
extern int g_nServerSetNum;
CFilteringTable::CFilteringTable()
{
}
CFilteringTable::~CFilteringTable()
{
	Release();
}
void CFilteringTable::Init()
{
	LoadFilterWordBinary(FILEPATH_FILTERWORD);
	m_pCurNode = NULL;
}
void CFilteringTable::Release()
{
	for (int i = 0; i < eFilter_Count; ++i)
	{
		if (m_RootNode[i].pChild)
			DeleteNode(m_RootNode[i].pChild);
	}
}
void CFilteringTable::DeleteNode(FILTER_NODE* pNode)
{
	if (pNode->pChild)
	{
		DeleteNode(pNode->pChild);
		pNode->pChild = NULL;
	}
	if (pNode->pSibling)
	{
		DeleteNode(pNode->pSibling);
		pNode->pSibling = NULL;
	}
	delete pNode;
}
void CFilteringTable::CleanNode(FILTER_NODE* pNode)
{
	if (pNode->pChild)
	{
		CleanNode(pNode->pChild);
	}
	if (pNode->pSibling)
	{
		CleanNode(pNode->pSibling);
	}
}
void CFilteringTable::AddWord(char *pWord, int nKind)
{
	AddNode(&m_RootNode[nKind], pWord);
}
void CFilteringTable::AddNode(FILTER_NODE* pNode, char* pWord)
{
	if (*pWord == NULL)
	{
		pNode->bEndFlag = TRUE;
		return;
	}
	if (pNode->pChild)
	{
		pNode = pNode->pChild;
		while (pNode)
		{
			if (pNode->cChar == *pWord)
			{
				if (IsDBCSLeadByte(pNode->cChar))
				{
					if (pNode->cExChar == *(pWord + 1))
					{
						pWord += 2;
						AddNode(pNode, pWord);
						return;
					}
				}
				else
				{
					++pWord;
					AddNode(pNode, pWord);
					return;
				}
			}
			if (pNode->pSibling == NULL)
			{
				FILTER_NODE* pSibling = new FILTER_NODE;
				pSibling->cChar = *pWord++;
				if (IsDBCSLeadByte(pSibling->cChar))
				{
					pSibling->cExChar = *pWord++;
				}
				pNode->pSibling = pSibling;
				AddNode(pSibling, pWord);
				return;
			}
			pNode = pNode->pSibling;
		}
	}
	else
	{
		FILTER_NODE* pChild = new FILTER_NODE;
		pChild->cChar = *pWord++;
		if (IsDBCSLeadByte(pChild->cChar))
		{
			pChild->cExChar = *pWord++;
		}
		pNode->pChild = pChild;
		AddNode(pChild, pWord);
	}
}
BOOL CFilteringTable::FilterWordInString(char* pStr, int nKind, int nMethod)
{
	for (int i = 0; i < eFilter_Count; ++i)
	{
		if (m_RootNode[i].pChild)
			CleanNode(m_RootNode[i].pChild);
	}
	m_bSearched = FALSE;
	switch (nMethod)
	{
	case eFM_WHOLE_MATCH:
		return FM_WholeMatch(pStr, nKind);
	case eFM_INCLUDE:
		return FM_Include(pStr, nKind);
	case eFM_ALLOWSPACE:
		return FM_Include(pStr, nKind);
	}
	return FALSE;
}
BOOL CFilteringTable::FM_WholeMatch(char* pStr, int nKind)
{
	while (*pStr)
	{
		m_bSpread = FALSE;
		if (IsDBCSLeadByte(*pStr))
		{
			SearchNode(m_RootNode[nKind].pChild, pStr, 0, TRUE);
			pStr += 2;
		}
		else
		{
			SearchNode(m_RootNode[nKind].pChild, pStr, 0, FALSE);
			++pStr;
		}
		if (m_bSearched)
		{
			if (*pStr) m_bSearched = FALSE;
			else		return TRUE;
		}
		if (m_bSpread == FALSE) return FALSE;
	}
	return FALSE;
}
BOOL CFilteringTable::FM_Include(char* pStr, int nKind)
{
	m_pCurNode = m_RootNode[nKind].pChild;
	while (*pStr)
	{
		m_bSpread = FALSE;
		if (IsDBCSLeadByte(*pStr))
		{
			SearchNode(m_pCurNode, pStr, 0, TRUE);
			pStr += 2;
		}
		else
		{
			SearchNode(m_pCurNode, pStr, 0, FALSE);
			++pStr;
		}
		if (m_bSpread == TRUE)
		{
			if (m_pCurStrPos == NULL)
				m_pCurStrPos = pStr;
		}
		else
		{
			m_pCurNode = m_RootNode[nKind].pChild;
			if (m_pCurStrPos)
			{
				pStr = m_pCurStrPos;
				m_pCurStrPos = NULL;
			}
		}
		if (m_bSearched) return TRUE;
	}
	return FALSE;
}
void CFilteringTable::SearchNode(FILTER_NODE* pNode, char* pStr, int nDepth, BOOL bDBC)
{
	if (pNode == NULL)	return;
	{
		if (bDBC)
		{
			if (pNode->cChar == *pStr && pNode->cExChar == *(pStr + 1))
			{
				m_bSpread = TRUE;
				m_pCurNode = pNode->pChild;
				if (pNode->bEndFlag == TRUE)
					m_bSearched = TRUE;
				return;
			}
		}
		else
		{
			if (pNode->cChar == *pStr)
			{
				m_bSpread = TRUE;
				m_pCurNode = pNode->pChild;
				if (pNode->bEndFlag == TRUE)
					m_bSearched = TRUE;
				return;
			}
		}
	}
	SearchNode(pNode->pSibling, pStr, nDepth, bDBC);
}
BOOL CFilteringTable::FilterChat(char* pChat)
{
	char tempChar;
	strncpy(&tempChar, pChat, 1);

	char checkChar = '/';
	char checkCommand = tempChar;

	if (checkChar == checkCommand)
	{
		char desc[128] = { 0, };
		char* tempStr1;

		int length1 = 0;
		int length2 = 0;
		int result = 0;

		tempStr1 = strchr(pChat, ' ');

		if (tempStr1)
		{
			++tempStr1;
			strcpy(m_strBuf, tempStr1);
			strupr(m_strBuf);
		}
		else
		{
			strcpy(m_strBuf, pChat);
			strupr(m_strBuf);
		}
	}
	else
	{
		strcpy(m_strBuf, pChat);
		strupr(m_strBuf);
	}

	if (FilterWordInString(m_strBuf, eFilter_Slang, eFM_INCLUDE)) //eFM_ALLOWSPACE
	{
		return TRUE;
	}

	return FALSE;
	/*strcpy(m_strBuf, pChat);
	strupr(m_strBuf);
	if (FilterWordInString(m_strBuf, eFilter_Slang, eFM_INCLUDE))
	{
		return TRUE;
	}
	return FALSE;*/
}
BOOL CFilteringTable::IsUsableName(char* pName, BOOL bUseByte)
{
	strcpy(m_strBuf, pName);
	strupr(m_strBuf);
	if (FilterWordInString(m_strBuf, eFilter_System, eFM_WHOLE_MATCH))
		return FALSE;
	if (FilterWordInString(m_strBuf, eFilter_GM, eFM_INCLUDE))
		return FALSE;
	if (FilterWordInString(m_strBuf, eFilter_Slang, eFM_INCLUDE))
		return FALSE;
	if (bUseByte)
	if (FilterWordInString(m_strBuf, eFilter_Byte, eFM_INCLUDE))
		return FALSE;
	return TRUE;
}
BOOL CFilteringTable::IsInvalidCharInclude(unsigned char* pStr, BOOL AllowAllDBC)
{
	/*BOOL bOk;
	while (*pStr)
	{
		bOk = FALSE;
		if (IsDBCSLeadByte(*pStr))
		{
			if (AllowAllDBC)
			{
				bOk = TRUE;
			}
			else
			{
				bOk = TRUE;
			}
			++pStr;
		}
		else
		{
			if ((*pStr >= 32 && *pStr < 127) || (*pStr >= 164 && *pStr < 251) || IsDBCSLeadByte(*pStr))
				bOk = TRUE;

			if (*pStr == 0x20)
			{
				bOk = FALSE;
			}
		}
		++pStr;
		if (bOk == FALSE)
			return TRUE;
	}
	return FALSE;*/
	BOOL bOk;
	while (*pStr)
	{
		bOk = FALSE;
		if (IsDBCSLeadByte(*pStr))
		{
			if (AllowAllDBC)
			{
				bOk = TRUE;
			}
			else
			{
#ifdef _JAPAN_LOCAL_
				if (IsInDBCRange(MAKEWORD(pStr[1], pStr[0])))
					bOk = TRUE;
#elif defined TAIWAN_LOCAL
				bOk = TRUE;
#elif defined _ML_LOCAL_
#ifdef _MHCLIENT_
				if( g_nServerSetNum != 1 || MAINGAME->GetUserLevel() <= 4 )	
#else
				if( g_nServerSetNum != 1 || g_pServerSystem->IsTestServer() )
#endif
				{
					if( ( pStr[0] >= 0xa4 && pStr[0] <= 0xc5 ) || ( pStr[0] >= 0xc9 && pStr[0] <= 0xf8 ) )
					{
						if( ( pStr[1] >= 0x40 && pStr[1] < 0x7f ) || ( pStr[1] > 0xa0 && pStr[1] < 0xff ) )
							bOk = TRUE;
					}
					else if( pStr[0] == 0xc6 )
					{
						if( pStr[1] >= 0x40 && pStr[1] < 0x7f )
							bOk = TRUE;
					}
					else if( pStr[0] == 0xf9 )
					{
						if( ( pStr[1] >= 0x40 && pStr[1] < 0x7f ) || ( pStr[1] > 0xa0 && pStr[1] <= 0xdc ) )
							bOk = TRUE;
					}
				}
#elif defined _HK_LOCAL_
				bOk = TRUE;
#elif defined _TL_LOCAL_
				bOk = FALSE;
#else
				if( pStr[0] >= 0xb0 && pStr[0] <=0xc8 && pStr[1] >= 0xa1 && pStr[1] <= 0xfe )
				{
					bOk = TRUE;
				}
				else if( pStr[0] >= 0x81 && pStr[0] <= 0xc6 && pStr[1] >= 0x41 && pStr[1] <= 0xfe )
				{
					bOk = TRUE;
				}
				if( pStr[0] >= 0xa1 && pStr[0] <= 0xac && pStr[1] >= 0x80 && pStr[1] <= 0xfe )
				{
					bOk = FALSE;
				}
#endif
			}
			++pStr;
		}
		else
		{
			if( ( *pStr >= 'A' && *pStr <= 'Z' ) || ( *pStr >= 'a' && *pStr <= 'z' ) )
				bOk = TRUE;
			else if( *pStr >= '0' && *pStr <= '9' )
				bOk = TRUE;
#ifdef _JAPAN_LOCAL_
			else if( *pStr >= 0xa6 && *pStr <= 0xdf )	
				bOk = TRUE;
#endif
#ifdef _TL_LOCAL_
			else if( *pStr >= 0xa1 && *pStr <= 0xce )
				bOk = TRUE;
			else if (*pStr >= 0xcf && *pStr <= 0xd9)
				bOk = TRUE;
			else if (*pStr >= 0xe0 && *pStr <= 0xec)
				bOk = TRUE;
#endif
			if (IsExceptionInvalidCharInclude(pStr))
			{
				bOk = TRUE;
			}
		}
		++pStr;
		if (bOk == FALSE)
		{
			SetExceptionInvalidCharIncludeType(FET_NONE);
			return TRUE;
		}
	}
	SetExceptionInvalidCharIncludeType(FET_NONE);
	return FALSE;
}
BOOL CFilteringTable::IsExceptionInvalidCharInclude(unsigned char* pszText)
{
	switch (m_eFilterExceptionType)
	{
	case FET_WHISPER_CHARNAME:
		if (*pszText == '[' || *pszText == ']')
		{
			return TRUE;
		}
		break;
	}

	return FALSE;
}
BOOL CFilteringTable::IsCharInString(char* pStr, char* pNoChar)
{
	while (*pStr)
	{
		if (IsDBCSLeadByte(*pStr))
		{
			++pStr;
		}
		else
		{
			char* p = pNoChar;
			while (*p)
			{
				if (*pStr == *p) return TRUE;
				++p;
			}
		}
		++pStr;
	}
	return FALSE;
}
BOOL CFilteringTable::LoadFilterWord(char* strFile)
{
	HANDLE	hFile;
	hFile = CreateFile(strFile, GENERIC_READ, 0, NULL,
		OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile == INVALID_HANDLE_VALUE)
	{
#ifdef _MHCLIENT_
		MessageBox(_g_hWnd, "File Not Found!", "Error", MB_OK);
		PostMessage(_g_hWnd, WM_CLOSE, NULL, NULL);
#endif
		return FALSE;
	}
	DWORD dwFileSize = GetFileSize(hFile, NULL);
	if (dwFileSize == -1) return FALSE;
	char* buf = new char[dwFileSize + 1];
	DWORD dwRead;
	if (!ReadFile(hFile, buf, dwFileSize, &dwRead, NULL))
	{
		CloseHandle(hFile);
		delete[] buf;
		return FALSE;
	}
	buf[dwFileSize] = NULL;
	SettingFilterWord(buf, dwFileSize);
	CloseHandle(hFile);
	delete[] buf;
	return TRUE;
}
BOOL CFilteringTable::LoadFilterWordBinary(char* strFile)
{
	CMHFile file;
	if (file.OpenBin(strFile))
	{
		DWORD dwFileSize = file.GetDataSize();
		char* buf = new char[dwFileSize + 1];
		memcpy(buf, file.GetData(), dwFileSize + 1);
		buf[dwFileSize] = NULL;
		SettingFilterWord(buf, dwFileSize);
		delete[] buf;
		file.Release();
		return TRUE;
	}
#ifdef _MHCLIENT_
	MessageBox(_g_hWnd, "File Not Found!", "Error", MB_OK);
	PostMessage(_g_hWnd, WM_CLOSE, NULL, NULL);
#endif
	return FALSE;
}
void CFilteringTable::SettingFilterWord(char* pStr, DWORD bufLen)
{
	int nKind = 0;
	char seps[] = ",\t\n\r";
	char* token;
	char OutBuf[256];
	token = strtok(pStr, seps);
	RemoveSpace(token, OutBuf);
	strupr(OutBuf);
	while (token != NULL)
	{
		if (token[0] != '@')
		{
			if (token[0] == '#')
			{
				if (strcmp(token, "#SLANG") == 0)
				{
					nKind = eFilter_Slang;
				}
				else if (strcmp(token, "#GM") == 0)
				{
					nKind = eFilter_GM;
				}
				else if (strcmp(token, "#SYSTEM") == 0)
				{
					nKind = eFilter_System;
				}
				else if (strcmp(token, "#BYTE") == 0)
				{
					nKind = eFilter_Byte;
				}
			}
			else
			{
				AddWord(OutBuf, nKind);
			}
		}
		token = strtok(NULL, seps);
		RemoveSpace(token, OutBuf);
		strupr(OutBuf);
	}
}
int CFilteringTable::RemoveSpace(char* pStrIn, char* pStrOut)
{
	if (pStrIn == NULL) return 0;
	int nLen = 0;
	while (*pStrIn)
	{
		if (*pStrIn != ' ')
		{
			*pStrOut = *pStrIn;
			++pStrOut;
			++nLen;
		}
		++pStrIn;
	}
	*pStrOut = 0;
	return nLen;
}