// This file is part of Notepad++ project
// Copyright (C)2021 Don HO <don.h@free.fr>

// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// at your option any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <https://www.gnu.org/licenses/>.
#pragma once
#include <vector>
#include <string>
#include <sstream>
#include <windows.h>
#include <iso646.h>
#include <cstdint>
#include <unordered_set>
#include <algorithm>
#include <tchar.h>


const bool dirUp = true;
const bool dirDown = false;

#define NPP_CP_WIN_1252           1252
#define NPP_CP_DOS_437            437
#define NPP_CP_BIG5               950

#define LINKTRIGGERED WM_USER+555

#ifdef UNICODE
	#define NppMainEntry wWinMain
	#define generic_strtol wcstol
	#define generic_strncpy wcsncpy
	#define generic_stricmp wcsicmp
	#define generic_strncmp wcsncmp
	#define generic_strnicmp wcsnicmp
	#define generic_strncat wcsncat
	#define generic_strchr wcschr
	#define generic_atoi _wtoi
	#define generic_itoa _itow
	#define generic_atof _wtof
	#define generic_strtok wcstok
	#define generic_strftime wcsftime
	#define generic_fprintf fwprintf
	#define generic_sprintf swprintf
	#define generic_sscanf swscanf
	#define generic_fopen _wfopen
	#define generic_fgets fgetws
	#define generic_stat _wstat
	#define generic_sprintf swprintf
	#define COPYDATA_FILENAMES COPYDATA_FILENAMESW
#else
	#define NppMainEntry WinMain
	#define generic_strtol strtol
	#define generic_strncpy strncpy
	#define generic_stricmp stricmp
	#define generic_strncmp strncmp
	#define generic_strnicmp strnicmp
	#define generic_strncat strncat
	#define generic_strchr strchr
	#define generic_atoi atoi
	#define generic_itoa itoa
	#define generic_atof atof
	#define generic_strtok strtok
	#define generic_strftime strftime
	#define generic_fprintf fprintf
	#define generic_sprintf sprintf
	#define generic_sscanf sscanf
	#define generic_fopen fopen
	#define generic_fgets fgets
	#define generic_stat _stat
	#define generic_sprintf sprintf
	#define COPYDATA_FILENAMES COPYDATA_FILENAMESA
#endif

#define assert	ASSERT	//(_Expression) ((void)0)

#define BCKGRD_COLOR (RGB(255,102,102))
#define TXT_COLOR    (RGB(255,255,255))

#ifndef __MINGW32__
#define WCSTOK wcstok
#else
#define WCSTOK wcstok_s
#endif




//#define NPP_INTERNAL_FUCTION_STR L"Notepad++::InternalFunction"

LPSTR ConvertUsingLocalePage(CString str);
LPSTR Convert2UTF8(CString str);
LPSTR Convert2Thai(CString str);

DWORD bSelFontSize[];

TCHAR *szEOL[];


void DrawRectangle();
void editorRegister(HWND hWnd);
void drawUAHMenuNCBottomLine(HWND hWnd);
void disableVisualStyle(HWND hwnd, bool doDisable);
void subclassAndThemeButton(HWND hwnd);
CString generateGUID_N();
CString generateGUID();
const LPBYTE map_file(LPCTSTR name, LPDWORD filesize);
void LoadMultiFile(CWnd*parent);
void ProcessFileList(LPCTSTR fileList);
void ShortMessage(PCWSTR stR, ...);
void requestExitApp(HWND hwnd , BOOL bMsgBox);
char* RemoveBracket(char* str);
typedef std::basic_string<TCHAR> generic_string;
typedef std::basic_stringstream<TCHAR> generic_stringstream;
char *strpbrkEx(const char *str, const char **strs);
BOOL GetFileExtensionCheck(const char* file_name);
const char* GetFileExtension(const char* file_name);
void hbrushTOrgbquad(HBRUSH br, RGBQUAD&rgbq);
void SafeStrCpy(char* pDest, const char* pSrc, int nDestBufSize);
void GetLineMsg(int line, char*function, char*file);
void DrawOrangeRectangle(/*HWND hWnd*/);
char* IntergerToString(int i);
void GetDesktopResolution(int& horizontal, int& vertical);
//int nbDigitsFromNbLines(size_t nbLines);
DWORD changeOpacity(DWORD color, float opacity);
std::string getFileExt(const std::string& s);
void printInt(int int2print);
void printStr(const TCHAR *str2print);
void removeTransparent(HWND hwnd);
void SetTransparent(HWND hwnd, int percent);
generic_string commafyInt(size_t n);

void writeLog(const TCHAR *logFileName, const char *log2write);
int filter(unsigned int code, struct _EXCEPTION_POINTERS *ep);
generic_string purgeMenuItemString(const TCHAR * menuItemStr, bool keepAmpersand = false);
std::vector<generic_string> tokenizeString(const generic_string & tokenString, const char delim);

void ClientRectToScreenRect(HWND hWnd, RECT* rect);
void ScreenRectToClientRect(HWND hWnd, RECT* rect);
void setBorderEdge(HWND hWnd, bool doWithBorderEdge);
CRect GetEditorPosition(HWND hDlg, int id);
std::wstring string2wstring(const std::string & rString, UINT codepage);
std::string wstring2string(const std::wstring & rwString, UINT codepage);
bool isInList(const TCHAR *token, const TCHAR *list);
generic_string BuildMenuFileName(int filenameLen, unsigned int pos, const generic_string &filename, bool ordinalNumber = true);

std::string getFileContent(const TCHAR *file2read);
generic_string relativeFilePathToFullFilePath(const TCHAR *relativeFilePath);

bool matchInList(const TCHAR *fileName, const std::vector<generic_string> & patterns);
bool matchInExcludeDirList(const TCHAR* dirName, const std::vector<generic_string>& patterns, size_t level);
bool allPatternsAreExclusion(const std::vector<generic_string> patterns);



#define REBARBAND_SIZE sizeof(REBARBANDINFO)

generic_string PathRemoveFileSpec(generic_string & path);
generic_string pathAppend(generic_string &strDest, const generic_string & str2append);
COLORREF getCtrlBgColor(HWND hWnd);
generic_string stringToUpper(generic_string strToConvert);
generic_string stringToLower(generic_string strToConvert);
generic_string stringReplace(generic_string subject, const generic_string& search, const generic_string& replace);
std::vector<generic_string> stringSplit(const generic_string& input, const generic_string& delimiter);
bool str2numberVector(generic_string str2convert, std::vector<size_t>& numVect);
generic_string stringJoin(const std::vector<generic_string>& strings, const generic_string& separator);
generic_string stringTakeWhileAdmissable(const generic_string& input, const generic_string& admissable);
double stodLocale(const generic_string& str, _locale_t loc, size_t* idx = NULL);

int OrdinalIgnoreCaseCompareStrings(LPCTSTR sz1, LPCTSTR sz2);

bool str2Clipboard(const generic_string &str2cpy, HWND hwnd);
class Buffer;
bool buf2Clipborad(const std::vector<Buffer*>& buffers, bool isFullPath, HWND hwnd);

generic_string GetLastErrorAsString(DWORD errorCode = 0);

generic_string intToString(int val);
generic_string uintToString(unsigned int val);

HWND CreateToolTip(int toolID, HWND hDlg, HINSTANCE hInst, const PTSTR pszText, bool isRTL);
HWND CreateToolTipRect(int toolID, HWND hWnd, HINSTANCE hInst, const PTSTR pszText, const RECT rc);


std::wstring s2ws(const std::string& str);
std::string ws2s(const std::wstring& wstr);

bool deleteFileOrFolder(const generic_string& f2delete);

void getFilesInFolder(std::vector<generic_string>& files, const generic_string& extTypeFilter, const generic_string& inFolder);

template<typename T> size_t vecRemoveDuplicates(std::vector<T>& vec, bool isSorted = false, bool canSort = false)
{
	if (!isSorted && canSort)
	{
		std::sort(vec.begin(), vec.end());
		isSorted = true;
	}

	if (isSorted)
	{
		typename std::vector<T>::iterator it;
		it = std::unique(vec.begin(), vec.end());
		vec.resize(distance(vec.begin(), it));  // unique() does not shrink the vector
	}
	else
	{
		std::unordered_set<T> seen;
		auto newEnd = std::remove_if(vec.begin(), vec.end(), [&seen](const T& value)
			{
				return !seen.insert(value).second;
			});
		vec.erase(newEnd, vec.end());
	}
	return vec.size();
}

void trim(generic_string& str);

int nbDigitsFromNbLines(size_t nbLines);

generic_string getDateTimeStrFrom(const generic_string& dateTimeFormat, const SYSTEMTIME& st);

HFONT createFont(const TCHAR* fontName, int fontSize, bool isBold, HWND hDestParent);

bool isWin32NamespacePrefixedFileName(const generic_string& fileName);
bool isWin32NamespacePrefixedFileName(const TCHAR* szFileName);
bool isUnsupportedFileName(const generic_string& fileName);
bool isUnsupportedFileName(const TCHAR* szFileName);


BOOL IsCtrlPressed();

BOOL IsAltPressed();

BOOL IsShiftPressed();
