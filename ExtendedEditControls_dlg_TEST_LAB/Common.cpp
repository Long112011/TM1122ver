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
#include "stdafx.h"
#include <algorithm>
#include <stdexcept>
#include <shlwapi.h>
//#include <uxtheme.h>
#include <cassert>
#include <codecvt>
//#include <locale>
//#include "StaticDialog.h"
//#include "CustomFileDialog.h"

//#include "FileInterface.h"
#include "Common.h"
#include "Utf8.h"
#include "main.h"
//#include <Parameters.h>
//#include "Buffer.h"
DWORD bSelFontSize[15] = { 5, 6, 7, 8, 9, 10, 11, 12, 14, 16, 18, 20, 22, 24, 26 };

TCHAR *szEOL[] =
{
	_T("Windows (CR LF)"), _T("Macintosh (CR)"), _T("Unix (LF)")
};

int(*fn)(void*, int, int, int);
void * ptr;
int canundo;






void editorRegister(HWND hWnd)
{
	fn = (int(__cdecl *)(void *, int, int, int))::SendMessage(hWnd, SCI_GETDIRECTFUNCTION, 0, 0);
	ptr = (void *)::SendMessage(hWnd, SCI_GETDIRECTPOINTER, 0, 0);
	canundo = fn(ptr, SCI_CANUNDO, 0, 0);
}

void drawUAHMenuNCBottomLine(HWND hWnd)
{
	MENUBARINFO mbi = { sizeof(mbi) };
	if (!GetMenuBarInfo(hWnd, OBJID_MENU, 0, &mbi))
	{
		return;
	}

	RECT rcClient = {};
	GetClientRect(hWnd, &rcClient);
	MapWindowPoints(hWnd, nullptr, (POINT*)&rcClient, 2);

	RECT rcWindow = {};
	GetWindowRect(hWnd, &rcWindow);

	OffsetRect(&rcClient, -rcWindow.left, -rcWindow.top);

	// the rcBar is offset by the window rect
	RECT rcAnnoyingLine = rcClient;
	rcAnnoyingLine.bottom = rcAnnoyingLine.top;
	rcAnnoyingLine.top--;


	HDC hdc = GetWindowDC(hWnd);
	FillRect(hdc, &rcAnnoyingLine, CreateSolidBrush(RGB(34, 34, 34)));
	ReleaseDC(hWnd, hdc);
}
void disableVisualStyle(HWND hwnd, bool doDisable)
{
	if (doDisable)
	{
		SetWindowTheme(hwnd, L"", L"");
	}
	else
	{
		SetWindowTheme(hwnd, nullptr, nullptr);
	}
}
void subclassAndThemeButton(HWND hwnd)
{
	auto nButtonStyle = ::GetWindowLongPtr(hwnd, GWL_STYLE);
	switch (nButtonStyle & BS_TYPEMASK)
	{
		// Plugin might use BS_3STATE and BS_AUTO3STATE button style
	case BS_CHECKBOX:
	case BS_AUTOCHECKBOX:
	case BS_3STATE:
	case BS_AUTO3STATE:
	case BS_RADIOBUTTON:
	case BS_AUTORADIOBUTTON:
	{
							   if ((nButtonStyle & BS_PUSHLIKE) == BS_PUSHLIKE)
							   {
								   //if (p._theme)
								   {
									   SetWindowTheme(hwnd, L"DarkMode_Explorer", nullptr);
								   }
								   break;
							   }
							   /*if (p._subclass)
							   {
							   NppDarkMode::subclassButtonControl(hwnd);
							   }*/
							   break;
	}

	case BS_GROUPBOX:
	{
						/*if (p._subclass)
						{
						NppDarkMode::subclassGroupboxControl(hwnd);
						}*/
						break;
	}

	case BS_DEFPUSHBUTTON:
	case BS_PUSHBUTTON:
	{
						  /*if (p._theme)
						  {
						  ::SetWindowTheme(hwnd, p._themeClassName, nullptr);
						  }*/
						  break;
	}

	default:
	{
			   break;
	}
	}
}
CString generateGUID_N()
{
	CString m_ListID(L"error");
	GUID guid;
	HRESULT hr = CoCreateGuid(&guid);

	// Convert the GUID to a string
	_TUCHAR * guidStr;
	UuidToString(&guid, &guidStr);
	m_ListID = (LPTSTR)guidStr;
	RpcStringFree(&guidStr);

	return m_ListID;
}
CString generateGUID()
{
	//don't forget to add Rpcrt4.lib to your project
	CString m_ListID(L"error");
	RPC_CSTR guidStr;
	GUID guid;
	HRESULT hr = CoCreateGuid(&guid);
	if (hr == S_OK)
	{
		if (UuidToString(&guid, &guidStr) == RPC_S_OK)
		{
			m_ListID = (LPTSTR)guidStr;
			RpcStringFree(&guidStr);
		}
	}
	return m_ListID;
}
/***********************************************************************
*             map_fileW
*
* Helper function to map a file to memory:
*  name			-	file name
*  [RETURN] ptr		-	pointer to mapped file
*  [RETURN] filesize           -       pointer size of file to be stored if not NULL
*/
const LPBYTE map_file(LPCTSTR name, LPDWORD filesize)
{
	HANDLE hFile, hMapping;
	LPBYTE ptr = NULL;

	hFile = CreateFile(name, GENERIC_READ, FILE_SHARE_READ, NULL,
		OPEN_EXISTING, FILE_FLAG_RANDOM_ACCESS, 0);
	if (hFile != INVALID_HANDLE_VALUE)
	{
		hMapping = CreateFileMapping(hFile, NULL, PAGE_READONLY, 0, 0, NULL);
		if (hMapping)
		{
			ptr = (LPBYTE)MapViewOfFile(hMapping, FILE_MAP_READ, 0, 0, 0);
			CloseHandle(hMapping);
			if (filesize)
				*filesize = GetFileSize(hFile, NULL);
		}
		CloseHandle(hFile);
	}
	return ptr;
}
void LoadMultiFile(CWnd*parent)
{
	const int MaxFileCount = 50;
	const int MaxFilePath = MAX_PATH;
	const int MaxFileBuffer = MaxFileCount * MaxFilePath + 1;
	std::vector < TCHAR > fileNamesBuffer(MaxFileBuffer);
	CFileDialog * fileDlg = NULL;
	while (true) {
		ASSERT(fileDlg == NULL);
		fileDlg = new CFileDialog(
			TRUE,
			NULL,
			NULL,
			OFN_HIDEREADONLY | OFN_ALLOWMULTISELECT,
			_T("All files (*.*)|*.*||"),
			parent
			);
		fileDlg->GetOFN().lpstrFile = &(fileNamesBuffer[0]);
		fileDlg->GetOFN().nMaxFile = (DWORD)fileNamesBuffer.size();
		::ZeroMemory(&(fileNamesBuffer[0]),
			fileNamesBuffer.size() * sizeof(TCHAR));
		INT_PTR result = fileDlg->DoModal();
		delete fileDlg;
		fileDlg = NULL;
		if (result == IDCANCEL)
		{
			DWORD dw = ::CommDlgExtendedError();
			if (dw & 0x3003)
			{
				AfxMessageBox(_T("Too many files selected."));
				continue;
			}
			break;
		}
		ProcessFileList(&(fileNamesBuffer[0]));
		break;
	}
}
void ProcessFileList(LPCTSTR fileList)
{
	ASSERT(fileList != NULL);
	LPCTSTR pstr = fileList;
	std::vector < CString > strings;
	while (true) {
		if (*pstr == _T('\0') && * (pstr + 1) == _T('\0')) {
			break;
		}
		strings.push_back(CString(pstr));
		pstr += (_tcslen(pstr) + 1);
	}
	CString msg;
	msg = _T("*** Selected files ***\n\n");
	msg += _T("Path : ") + strings.at(0);
	msg += _T("\n\n");
	for (size_t i = 1; i < strings.size(); i++) {
		msg += strings.at(i);
		msg += _T("\n");
	}
	AfxMessageBox(msg);
}
void ShortMessage(PCWSTR stR, ...)
{
	TaskDialog(nullptr, AfxGetApp()->m_hInstance, L"ShortMessage", nullptr, stR, TDCBF_OK_BUTTON, TD_ERROR_ICON, nullptr);
}
void requestExitApp(HWND hwnd,BOOL bMsgBox)
{
	if (MessageBox(hwnd, "Are you sure to quit ?", "info", MB_YESNO) == IDNO)
	{
		return;
	}
	else
	{
		SERVERCMD->OnCommand("/kill THIS");
		//PostQuitMessage(0);
		//EndDialog(hwnd, IDCANCEL);
		/*//SERVERCMD->OnCommand("/kill THIS");
		//SERVERCMD->OnCommand("/kill WER");
		//exit(0);
		
		EndDialog(hwnd, -1);*/
	}
	
	//::PostMessage(hwnd, WM_CLOSE, reinterpret_cast<WPARAM>(""), reinterpret_cast<LPARAM>(""));
	//::PostMessage(hwnd, WM_QUIT, reinterpret_cast<WPARAM>(""), reinterpret_cast<LPARAM>(""));
	
}
char* RemoveBracket(char* str)
{
	static char buf[32];
	char* p = buf;
	while (*str)
	{
		if (*str != '"')
		{
			*p = *str;
			++p;
		}
		++str;
	}
	*p = 0;
	return buf;
}
LPSTR ConvertUsingLocalePage(CString str)
{
	// Convert using the local code page
	//CString str(_T("Hello, world!"));
	CT2A ascii(str);
	TRACE(_T("ASCII: %S\n"), ascii.m_psz);

	return ascii.m_psz;
}
LPSTR Convert2UTF8(CString str)
{
	// Convert to UTF8
	//CString str(_T("Some Unicode goodness"));
	CT2A ascii(str, CP_UTF8);
	TRACE(_T("UTF8: %S\n"), ascii.m_psz);

	return ascii.m_psz;
}
LPSTR Convert2Thai(CString str)
{
	// Convert to Thai code page
	//CString str(_T("Some Thai text"));
	CT2A ascii(str, 874);
	TRACE(_T("Thai: %S\n"), ascii.m_psz);

	return ascii.m_psz;
}





char *strpbrkEx(const char *str, const char **strs)
{
	char *minp = (char*)-1, *p;
	int len, lenmin;
	if (NULL == str || NULL == strs)return NULL;
	while (*strs){
		p = (char *)strstr(str, *strs++);
		if (p && minp > p)
			minp = p;
	}
	if (minp == (char*)-1) return NULL;
	return minp;
}
BOOL GetFileExtensionCheck(const char* file_name)
{

	int ext = '.';
	const char* extension = NULL;

	extension = strrchr(file_name, ext);

	if (extension == NULL)
	{
		
		return FALSE;
	}
	//else
	
	//AfxMessageBox(output);

	return TRUE;
}
const char* GetFileExtension(const char* file_name)
{

	int ext = '.';
	const char* extension = NULL;
	char output [1024]/* = { 0 }*/;
	CString outputS;
	extension = strrchr(file_name, ext);

	if (extension == NULL)
	{
		sprintf(output, "Invalid extension encountered\n");
		outputS.Format("%s", output);
		return outputS;
	}
	//else
	{
		//SafeStrCpy(output,)
		sprintf(output, "File extension is %s\n", extension);
	}
	//AfxMessageBox(output);
	
	return extension;
}
DWORD changeOpacity(DWORD color, float opacity) {
	int alpha = (color >> 24) & 0xff;
	int r = (color >> 16) & 0xff;
	int g = (color >> 8) & 0xff;
	int b = color & 0xff;

	int newAlpha = ceil(alpha * opacity);

	UINT newColor = r << 16;
	newColor += g << 8;
	newColor += b;
	newColor += (newAlpha << 24);


	return (DWORD)newColor;
}

void GetDesktopResolution(int& horizontal, int& vertical)
{
	RECT desktop;
	const HWND hDesktop = GetDesktopWindow();
	GetWindowRect(hDesktop, &desktop);
	horizontal = desktop.right;
	vertical = desktop.bottom;
}
static string utf16ToUTF8(const wstring &s)
{
	const int size = ::WideCharToMultiByte(CP_UTF8, 0, s.c_str(), -1, NULL, 0, 0, NULL);

	vector<char> buf(size);
	::WideCharToMultiByte(CP_UTF8, 0, s.c_str(), -1, &buf[0], size, 0, NULL);

	return string(&buf[0]);
}
void hbrushTOrgbquad(HBRUSH br, RGBQUAD&rgbq)
{
	static COLORREF lbColor;
	LOGBRUSH logbr = { 0 };
	//RGBQUAD rgbq = { 0 };
	if (GetObject(br, sizeof(logbr), &logbr))
	{
		lbColor = logbr.lbColor;
		rgbq.rgbBlue = GetBValue(lbColor);
		rgbq.rgbGreen = GetGValue(lbColor);
		rgbq.rgbRed = GetRValue(lbColor);
		rgbq.rgbReserved = 0;
	}
	//return rgbq;
}
void SafeStrCpy(char* pDest, const char* pSrc, int nDestBufSize)
{
	strncpy(pDest, pSrc, nDestBufSize - 1);
	pDest[nDestBufSize - 1] = 0;
}
void DrawOrangeRectangle(/*HWND hWnd*/)
{
	// Draw orange rectangle
	CDC *pDC = CDC::FromHandle(GetDC(GetActiveWindow()));
	//CDC* pDC = ;
	CBrush NewBrush(RGB(255, 123, 0));
	CBrush *pBrush = pDC->SelectObject(&NewBrush);
	//pDC->MoveTo(500, 500);
	pDC->Rectangle(0, 0, 300, 150);
	pDC->SelectObject(pBrush);	
}
void DrawRectangle()
{
	CDC *pDC = CDC::FromHandle(GetDC(GetActiveWindow()));
	CBrush NewBrush(RGB(255, 123, 0));
	CBrush *pBrush = pDC->SelectObject(&NewBrush);
	//pDC->MoveTo(500, 500);
	pDC->Rectangle(0, 0, 300, 150);
	pDC->SelectObject(pBrush);
}
void GetLineMsg(int line, char*function, char*file)
{
	CString jack;
	jack.Format("%d\n%s\n%s", line, function, file);
	AfxMessageBox(jack);
}
char* IntergerToString(int i)
{
	char str[256];
	memset(str, 0, 256);
	sprintf(str, "%d", i);
	return str;
}
std::string getFileExt(const std::string& s)
{
	size_t i = s.rfind('.', s.length());
	if (i != string::npos) {
		return(s.substr(i + 1, s.length() - i));
	}

	return("");
}
void printInt(int int2print)
{
	TCHAR str[32];
	wsprintf(str, TEXT("%d"), int2print);
	::MessageBox(NULL, str, TEXT(""), MB_OK);
}
void removeTransparent(HWND hwnd)
{
	if (hwnd != nullptr)
		::SetWindowLongPtr(hwnd, GWL_EXSTYLE, ::GetWindowLongPtr(hwnd, GWL_EXSTYLE) & ~WS_EX_LAYERED);
}


void SetTransparent(HWND hwnd, int percent)
{
	::SetWindowLongPtr(hwnd, GWL_EXSTYLE, ::GetWindowLongPtr(hwnd, GWL_EXSTYLE) | WS_EX_LAYERED);
	if (percent > 255)
		percent = 255;
	else if (percent < 0)
		percent = 0;
	::SetLayeredWindowAttributes(hwnd, 0, static_cast<BYTE>(percent), LWA_ALPHA);
}

void printStr(const TCHAR *str2print)
{
	::MessageBox(NULL, str2print, TEXT(""), MB_OK);
}

generic_string commafyInt(size_t n)
{
	generic_stringstream ss;
	ss.imbue(std::locale(""));
	ss << n;
	return ss.str();
}

std::string getFileContent(const TCHAR *file2read)
{
	if (!::PathFileExists(file2read))
		return "";

	const size_t blockSize = 1024;
	char data[blockSize];
	std::string wholeFileContent = "";
	FILE *fp = fopen(file2read, TEXT("rb"));

	size_t lenFile = 0;
	do
	{
		lenFile = fread(data, 1, blockSize, fp);
		if (lenFile <= 0) break;
		wholeFileContent.append(data, lenFile);
	}
	while (lenFile > 0);

	fclose(fp);
	return wholeFileContent;
}

char getDriveLetter()
{
	char drive = '\0';
	TCHAR current[MAX_PATH];

	::GetCurrentDirectory(MAX_PATH, current);
	int driveNbr = ::PathGetDriveNumber(current);
	if (driveNbr != -1)
		drive = 'A' + char(driveNbr);

	return drive;
}


generic_string relativeFilePathToFullFilePath(const TCHAR *relativeFilePath)
{
	generic_string fullFilePathName;
	TCHAR fullFileName[MAX_PATH];
	BOOL isRelative = ::PathIsRelative(relativeFilePath);

	if (isRelative)
	{
		::GetFullPathName(relativeFilePath, MAX_PATH, fullFileName, NULL);
		fullFilePathName += fullFileName;
	}
	else
	{
		if ((relativeFilePath[0] == '\\' && relativeFilePath[1] != '\\') || relativeFilePath[0] == '/')
		{
			fullFilePathName += getDriveLetter();
			fullFilePathName += ':';
		}

		fullFilePathName += relativeFilePath;
	}

	return fullFilePathName;
}





void writeLog(const TCHAR *logFileName, const char *log2write)
{
	const DWORD accessParam{ GENERIC_READ | GENERIC_WRITE };
	const DWORD shareParam{ FILE_SHARE_READ | FILE_SHARE_WRITE };
	const DWORD dispParam{ OPEN_ALWAYS }; // Open existing file for writing without destroying it or create new
	const DWORD attribParam{ FILE_ATTRIBUTE_NORMAL };
	HANDLE hFile = ::CreateFile(logFileName, accessParam, shareParam, NULL, dispParam, attribParam, NULL);

	if (hFile != INVALID_HANDLE_VALUE)
	{
		LARGE_INTEGER offset{};
		offset.QuadPart = 0;
		::SetFilePointerEx(hFile, offset, NULL, FILE_END);

		SYSTEMTIME currentTime = {};
		::GetLocalTime(&currentTime);
		generic_string dateTimeStrW = getDateTimeStrFrom(TEXT("yyyy-MM-dd HH:mm:ss"), currentTime);
		std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
		std::string log2writeStr = "writelog";// converter.to_bytes(dateTimeStrW);
		log2writeStr += "  ";
		log2writeStr += log2write;
		log2writeStr += "\n";

		DWORD bytes_written = 0;
		::WriteFile(hFile, log2writeStr.c_str(), static_cast<DWORD>(log2writeStr.length()), &bytes_written, NULL);

		::FlushFileBuffers(hFile);
		::CloseHandle(hFile);
	}
}





void ClientRectToScreenRect(HWND hWnd, RECT* rect)
{
	POINT		pt{};

	pt.x		 = rect->left;
	pt.y		 = rect->top;
	::ClientToScreen( hWnd, &pt );
	rect->left   = pt.x;
	rect->top    = pt.y;

	pt.x		 = rect->right;
	pt.y		 = rect->bottom;
	::ClientToScreen( hWnd, &pt );
	rect->right  = pt.x;
	rect->bottom = pt.y;
}


std::vector<generic_string> tokenizeString(const generic_string & tokenString, const char delim)
{
	//Vector is created on stack and copied on return
	std::vector<generic_string> tokens;

    // Skip delimiters at beginning.
	generic_string::size_type lastPos = tokenString.find_first_not_of(delim, 0);
    // Find first "non-delimiter".
    generic_string::size_type pos     = tokenString.find_first_of(delim, lastPos);

    while (pos != std::string::npos || lastPos != std::string::npos)
    {
        // Found a token, add it to the vector.
        tokens.push_back(tokenString.substr(lastPos, pos - lastPos));
        // Skip delimiters.  Note the "not_of"
        lastPos = tokenString.find_first_not_of(delim, pos);
        // Find next "non-delimiter"
        pos = tokenString.find_first_of(delim, lastPos);
    }
	return tokens;
}


void ScreenRectToClientRect(HWND hWnd, RECT* rect)
{
	POINT		pt{};

	pt.x		 = rect->left;
	pt.y		 = rect->top;
	::ScreenToClient( hWnd, &pt );
	rect->left   = pt.x;
	rect->top    = pt.y;

	pt.x		 = rect->right;
	pt.y		 = rect->bottom;
	::ScreenToClient( hWnd, &pt );
	rect->right  = pt.x;
	rect->bottom = pt.y;
}

void setBorderEdge(HWND hWnd, bool doWithBorderEdge)
{
	long style = static_cast<long>(::GetWindowLongPtr(hWnd, GWL_STYLE));
	long exStyle = static_cast<long>(::GetWindowLongPtr(hWnd, GWL_EXSTYLE));

	if (theApp.GetGameAI()->sPage1.DarkMode)
	{
		exStyle &= ~WS_EX_CLIENTEDGE;

		if (doWithBorderEdge)
			style |= WS_BORDER;
		else
			style &= ~WS_BORDER;
	}
	else
	{
		style &= ~WS_BORDER;

		if (doWithBorderEdge)
			exStyle |= WS_EX_CLIENTEDGE;
		else
			exStyle &= ~WS_EX_CLIENTEDGE;
	}

	::SetWindowLongPtr(hWnd, GWL_STYLE, style);
	::SetWindowLongPtr(hWnd, GWL_EXSTYLE, exStyle);
	::SetWindowPos(hWnd, NULL, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_FRAMECHANGED);
}
CRect GetEditorPosition(HWND hDlg, int id)
{
	CRect rect;
	HWND hwndTool = GetDlgItem(hDlg, id);
	//CWnd *pWnd = //hDlg.GetDlgItem( id);///*hDlg->*/GetDlgItem(id);
		/*pWnd->*/::GetWindowRect(hwndTool,&rect);
		POINT		pt{};

		pt.x = rect.left;
		pt.y = rect.top;
		::ScreenToClient(hwndTool, &pt); //optional step - see below
		
		pt.x = rect.right;
		pt.y = rect.bottom;
		::ScreenToClient(hwndTool, &pt);
		rect.right = pt.x;
		rect.bottom = pt.y;

	return rect;
	//position:  rect.left, rect.top
	//size: rect.Width(), rect.Height()
}
int filter(unsigned int code, struct _EXCEPTION_POINTERS *)
{
    if (code == EXCEPTION_ACCESS_VIOLATION)
        return EXCEPTION_EXECUTE_HANDLER;
    return EXCEPTION_CONTINUE_SEARCH;
}


bool isInList(const TCHAR *token, const TCHAR *list)
{
	if ((!token) || (!list))
		return false;

	const size_t wordLen = 64;
	size_t listLen = lstrlen(list);

	TCHAR word[wordLen] = { '\0' };
	size_t i = 0;
	size_t j = 0;

	for (; i <= listLen; ++i)
	{
		if ((list[i] == ' ')||(list[i] == '\0'))
		{
			if (j != 0)
			{
				word[j] = '\0';
				j = 0;

				if (!wcsicmp((const wchar_t *)token, (const wchar_t *) word))
					return true;
			}
		}
		else
		{
			word[j] = list[i];
			++j;

			if (j >= wordLen)
				return false;
		}
	}
	return false;
}


generic_string purgeMenuItemString(const TCHAR * menuItemStr, bool keepAmpersand)
{
	const size_t cleanedNameLen = 64;
	TCHAR cleanedName[cleanedNameLen] = TEXT("");
	size_t j = 0;
	size_t menuNameLen = lstrlen(menuItemStr);
	if (menuNameLen >= cleanedNameLen)
		menuNameLen = cleanedNameLen - 1;

	for (size_t k = 0 ; k < menuNameLen ; ++k)
	{
		if (menuItemStr[k] == '\t')
		{
			cleanedName[k] = 0;
			break;
		}
		else
		{
			if (menuItemStr[k] == '&')
			{
				if (keepAmpersand)
					cleanedName[j++] = menuItemStr[k];
				//else skip
			}
			else
				cleanedName[j++] = menuItemStr[k];
		}
	}

	cleanedName[j] = 0;
	return cleanedName;
}




std::wstring string2wstring(const std::string & rString, UINT codepage)
{
	int len = MultiByteToWideChar(codepage, 0, rString.c_str(), -1, NULL, 0);
	if (len > 0)
	{
		std::vector<wchar_t> vw(len);
		MultiByteToWideChar(codepage, 0, rString.c_str(), -1, &vw[0], len);
		return &vw[0];
	}
	return std::wstring();
}


std::string wstring2string(const std::wstring & rwString, UINT codepage)
{
	int len = WideCharToMultiByte(codepage, 0, rwString.c_str(), -1, NULL, 0, NULL, NULL);
	if (len > 0)
	{
		std::vector<char> vw(len);
		WideCharToMultiByte(codepage, 0, rwString.c_str(), -1, &vw[0], len, NULL, NULL);
		return &vw[0];
	}
	return std::string();
}


// Escapes ampersands in file name to use it in menu
template <typename T>
generic_string convertFileName(T beg, T end)
{
	generic_string strTmp;

	for (T it = beg; it != end; ++it)
	{
		if (*it == '&') strTmp.push_back('&');
		strTmp.push_back(*it);
	}

	return strTmp;
}


generic_string intToString(int val)
{
	std::vector<TCHAR> vt;
	bool isNegative = val < 0;
	// can't use abs here because std::numeric_limits<int>::min() has no positive representation
	//val = std::abs(val);

	vt.push_back('0' + static_cast<TCHAR>(std::abs(val % 10)));
	val /= 10;
	while (val != 0)
	{
		vt.push_back('0' + static_cast<TCHAR>(std::abs(val % 10)));
		val /= 10;
	}

	if (isNegative)
		vt.push_back('-');

	return generic_string(vt.rbegin(), vt.rend());
}

generic_string uintToString(unsigned int val)
{
	std::vector<TCHAR> vt;

	vt.push_back('0' + static_cast<TCHAR>(val % 10));
	val /= 10;
	while (val != 0)
	{
		vt.push_back('0' + static_cast<TCHAR>(val % 10));
		val /= 10;
	}

	return generic_string(vt.rbegin(), vt.rend());
}

// Build Recent File menu entries from given
generic_string BuildMenuFileName(int filenameLen, unsigned int pos, const generic_string &filename, bool ordinalNumber)
{
	generic_string strTemp;

	if (ordinalNumber)
	{
		if (pos < 9)
		{
			strTemp.push_back('&');
			strTemp.push_back('1' + static_cast<TCHAR>(pos));
		}
		else if (pos == 9)
		{
			strTemp.append(TEXT("1&0"));
		}
		else
		{
			div_t splitDigits = div(pos + 1, 10);
			strTemp.append(uintToString(splitDigits.quot));
			strTemp.push_back('&');
			strTemp.append(uintToString(splitDigits.rem));
		}
		strTemp.append(TEXT(": "));
	}
	else
	{
		strTemp.push_back('&');
	}

	if (filenameLen > 0)
	{
		std::vector<TCHAR> vt(filenameLen + 1);
		// W removed from PathCompactPathExW due to compiler errors for ANSI version.
		PathCompactPathEx(&vt[0], filename.c_str(), filenameLen + 1, 0);
		strTemp.append(convertFileName(vt.begin(), vt.begin() + lstrlen(&vt[0])));
	}
	else
	{
		// (filenameLen < 0)
		generic_string::const_iterator it = filename.begin();

		if (filenameLen == 0)
			it += PathFindFileName(filename.c_str()) - filename.c_str();

		// MAX_PATH is still here to keep old trimming behaviour.
		if (filename.end() - it < MAX_PATH)
		{
			strTemp.append(convertFileName(it, filename.end()));
		}
		else
		{
			strTemp.append(convertFileName(it, it + MAX_PATH / 2 - 3));
			strTemp.append(TEXT("..."));
			strTemp.append(convertFileName(filename.end() - MAX_PATH / 2, filename.end()));
		}
	}

	return strTemp;
}


generic_string PathRemoveFileSpec(generic_string& path)
{
    generic_string::size_type lastBackslash = path.find_last_of(TEXT('\\'));
    if (lastBackslash == generic_string::npos)
    {
        if (path.size() >= 2 && path[1] == TEXT(':'))  // "C:foo.bar" becomes "C:"
            path.erase(2);
        else
            path.erase();
    }
    else
    {
        if (lastBackslash == 2 && path[1] == TEXT(':') && path.size() >= 3)  // "C:\foo.exe" becomes "C:\"
            path.erase(3);
        else if (lastBackslash == 0 && path.size() > 1) // "\foo.exe" becomes "\"
            path.erase(1);
        else
            path.erase(lastBackslash);
    }
	return path;
}


generic_string pathAppend(generic_string& strDest, const generic_string& str2append)
{
	if (strDest.empty() && str2append.empty()) // "" + ""
	{
		strDest = TEXT("\\");
		return strDest;
	}

	if (strDest.empty() && !str2append.empty()) // "" + titi
	{
		strDest = str2append;
		return strDest;
	}

	if (strDest[strDest.length() - 1] == '\\' && (!str2append.empty() && str2append[0] == '\\')) // toto\ + \titi
	{
		strDest.erase(strDest.length() - 1, 1);
		strDest += str2append;
		return strDest;
	}

	if ((strDest[strDest.length() - 1] == '\\' && (!str2append.empty() && str2append[0] != '\\')) // toto\ + titi
		|| (strDest[strDest.length() - 1] != '\\' && (!str2append.empty() && str2append[0] == '\\'))) // toto + \titi
	{
		strDest += str2append;
		return strDest;
	}

	// toto + titi
	strDest += TEXT("\\");
	strDest += str2append;

	return strDest;
}


COLORREF getCtrlBgColor(HWND hWnd)
{
	COLORREF crRet = CLR_INVALID;
	if (hWnd && IsWindow(hWnd))
	{
		RECT rc;
		if (GetClientRect(hWnd, &rc))
		{
			HDC hDC = GetDC(hWnd);
			if (hDC)
			{
				HDC hdcMem = CreateCompatibleDC(hDC);
				if (hdcMem)
				{
					HBITMAP hBmp = CreateCompatibleBitmap(hDC,
					rc.right, rc.bottom);
					if (hBmp)
					{
						HGDIOBJ hOld = SelectObject(hdcMem, hBmp);
						if (hOld)
						{
							if (SendMessage(hWnd, WM_ERASEBKGND, reinterpret_cast<WPARAM>(hdcMem), 0))
							{
								crRet = GetPixel(hdcMem, 2, 2); // 0, 0 is usually on the border
							}
							SelectObject(hdcMem, hOld);
						}
						DeleteObject(hBmp);
					}
					DeleteDC(hdcMem);
				}
				ReleaseDC(hWnd, hDC);
			}
		}
	}
	return crRet;
}


generic_string stringToUpper(generic_string strToConvert)
{
    std::transform(strToConvert.begin(), strToConvert.end(), strToConvert.begin(), 
        [](wchar_t ch){ return static_cast<wchar_t>(towupper(ch)); }
    );
    return strToConvert;
}

generic_string stringToLower(generic_string strToConvert)
{
    std::transform(strToConvert.begin(), strToConvert.end(), strToConvert.begin(), ::towlower);
    return strToConvert;
}


generic_string stringReplace(generic_string subject, const generic_string& search, const generic_string& replace)
{
	size_t pos = 0;
	while ((pos = subject.find(search, pos)) != std::string::npos)
	{
		subject.replace(pos, search.length(), replace);
		pos += replace.length();
	}
	return subject;
}


std::vector<generic_string> stringSplit(const generic_string& input, const generic_string& delimiter)
{
	size_t start = 0U;
	size_t end = input.find(delimiter);
	std::vector<generic_string> output;
	const size_t delimiterLength = delimiter.length();
	while (end != std::string::npos)
	{
		output.push_back(input.substr(start, end - start));
		start = end + delimiterLength;
		end = input.find(delimiter, start);
	}
	output.push_back(input.substr(start, end));
	return output;
}


bool str2numberVector(generic_string str2convert, std::vector<size_t>& numVect)
{
	numVect.clear();

	for (auto i : str2convert)
	{
		switch (i)
		{
		case ' ':
		case '0': case '1':	case '2': case '3':	case '4':
		case '5': case '6':	case '7': case '8':	case '9':
		{
			// correct. do nothing
		}
		break;

		default:
			return false;
		}
	}

	std::vector<generic_string> v = stringSplit(str2convert, TEXT(" "));
	for (const auto& i : v)
	{
		// Don't treat empty string and the number greater than 9999
		if (!i.empty() && i.length() < 5)
		{
			numVect.push_back(std::stoi(i));
		}
	}
	return true;
}

generic_string stringJoin(const std::vector<generic_string>& strings, const generic_string& separator)
{
	generic_string joined;
	size_t length = strings.size();
	for (size_t i = 0; i < length; ++i)
	{
		joined += strings.at(i);
		if (i != length - 1)
		{
			joined += separator;
		}
	}
	return joined;
}


generic_string stringTakeWhileAdmissable(const generic_string& input, const generic_string& admissable)
{
	// Find first non-admissable character in "input", and remove everything after it.
	size_t idx = input.find_first_not_of(admissable);
	if (idx == std::string::npos)
	{
		return input;
	}
	else
	{
		return input.substr(0, idx);
	}
}


double stodLocale(const generic_string& str, _locale_t loc, size_t* idx)
{
	// Copied from the std::stod implementation but uses _wcstod_l instead of wcstod.
	/*const char* ptr = str.c_str();
	errno = 0;
	wchar_t* eptr;
#ifdef __MINGW32__
	double ans = ::wcstod(ptr, &eptr);
#else
	double ans = ::_wcstod_l(ptr, &eptr, loc);
#endif
	if (ptr == eptr)
		throw std::invalid_argument("invalid stod argument");
	if (errno == ERANGE)
		throw std::out_of_range("stod argument out of range");
	if (idx != NULL)
		*idx = (size_t)(eptr - ptr);
	return ans;*/
	return 0;
}

// Source: https://blogs.msdn.microsoft.com/greggm/2005/09/21/comparing-file-names-in-native-code/
// Modified to use TCHAR's instead of assuming Unicode and reformatted to conform with Notepad++ code style
static TCHAR ToUpperInvariant(TCHAR input)
{
	TCHAR result;
	LONG lres = LCMapString(LOCALE_INVARIANT, LCMAP_UPPERCASE, &input, 1, &result, 1);
	if (lres == 0)
	{
		assert(false and "LCMapString failed to convert a character to upper case");
		result = input;
	}
	return result;
}

// Source: https://blogs.msdn.microsoft.com/greggm/2005/09/21/comparing-file-names-in-native-code/
// Modified to use TCHAR's instead of assuming Unicode and reformatted to conform with Notepad++ code style
int OrdinalIgnoreCaseCompareStrings(LPCTSTR sz1, LPCTSTR sz2)
{
	if (sz1 == sz2)
	{
		return 0;
	}

	if (sz1 == nullptr) sz1 = _T("");
	if (sz2 == nullptr) sz2 = _T("");

	for (;; sz1++, sz2++)
	{
		const TCHAR c1 = *sz1;
		const TCHAR c2 = *sz2;

		// check for binary equality first
		if (c1 == c2)
		{
			if (c1 == 0)
			{
				return 0; // We have reached the end of both strings. No difference found.
			}
		}
		else
		{
			if (c1 == 0 || c2 == 0)
			{
				return (c1-c2); // We have reached the end of one string
			}

			// IMPORTANT: this needs to be upper case to match the behavior of the operating system.
			// See http://msdn.microsoft.com/library/default.asp?url=/library/en-us/dndotnet/html/StringsinNET20.asp
			const TCHAR u1 = ToUpperInvariant(c1);
			const TCHAR u2 = ToUpperInvariant(c2);
			if (u1 != u2)
			{
				return (u1-u2); // strings are different
			}
		}
	}
}

bool str2Clipboard(const generic_string &str2cpy, HWND hwnd)
{
	size_t len2Allocate = (str2cpy.size() + 1) * sizeof(TCHAR);
	HGLOBAL hglbCopy = ::GlobalAlloc(GMEM_MOVEABLE, len2Allocate);
	if (hglbCopy == NULL)
	{
		return false;
	}
	if (!::OpenClipboard(hwnd))
	{
		::GlobalFree(hglbCopy);
		::CloseClipboard();
		return false;
	}
	if (!::EmptyClipboard())
	{
		::GlobalFree(hglbCopy);
		::CloseClipboard();
		return false;
	}
	// Lock the handle and copy the text to the buffer.
	TCHAR *pStr = (TCHAR *)::GlobalLock(hglbCopy);
	if (pStr == NULL)
	{
		::GlobalUnlock(hglbCopy);
		::GlobalFree(hglbCopy);
		::CloseClipboard();
		return false;
	}
	//wcscpy(pStr, len2Allocate / sizeof(TCHAR), str2cpy.c_str());
	strcpy(pStr, /*len2Allocate / sizeof(TCHAR),*/ str2cpy.c_str());
	::GlobalUnlock(hglbCopy);
	// Place the handle on the clipboard.
	unsigned int clipBoardFormat = CF_UNICODETEXT;
	if (::SetClipboardData(clipBoardFormat, hglbCopy) == NULL)
	{
		::GlobalFree(hglbCopy);
		::CloseClipboard();
		return false;
	}
	if (!::CloseClipboard())
	{
		return false;
	}
	return true;
}

bool buf2Clipborad(const std::vector<Buffer*>& buffers, bool isFullPath, HWND hwnd)
{
/*	const generic_string crlf = _T("\r\n");
	generic_string selection;
	for (auto&& buf : buffers)
	{
		if (buf)
		{
			const TCHAR* fileName = isFullPath ? buf->getFullPathName() : buf->getFileName();
			if (fileName)
				selection += fileName;
		}
		if (!selection.empty() && !selection.ends_with(crlf))
			selection += crlf;
	}
	if (!selection.empty())
		return str2Clipboard(selection, hwnd);*/
	return false;
}

bool matchInList(const TCHAR *fileName, const std::vector<generic_string> & patterns)
{
	bool is_matched = false;
	for (size_t i = 0, len = patterns.size(); i < len; ++i)
	{
		if (patterns[i].length() > 1 && patterns[i][0] == '!')
		{
			if (PathMatchSpec(fileName, patterns[i].c_str() + 1))
				return false;

			continue;
		} 

		if (PathMatchSpec(fileName, patterns[i].c_str()))
			is_matched = true;
	}
	return is_matched;
}

bool matchInExcludeDirList(const TCHAR* dirName, const std::vector<generic_string>& patterns, size_t level)
{
	for (size_t i = 0, len = patterns.size(); i < len; ++i)
	{
		size_t patterLen = patterns[i].length();

		if (patterLen > 3 && patterns[i][0] == '!' && patterns[i][1] == '+' && patterns[i][2] == '\\') // check for !+\folderPattern: for all levels - search this pattern recursively
		{
			if (PathMatchSpec(dirName, patterns[i].c_str() + 3))
				return true;
		}
		else if (patterLen > 2 && patterns[i][0] == '!' && patterns[i][1] == '\\') // check for !\folderPattern: exclusive pattern for only the 1st level
		{
			if (level == 1)
				if (PathMatchSpec(dirName, patterns[i].c_str() + 2))
					return true;
		}
	}
	return false;
}

bool allPatternsAreExclusion(const std::vector<generic_string> patterns)
{
	bool oneInclusionPatternFound = false;
	for (size_t i = 0, len = patterns.size(); i < len; ++i)
	{
		if (patterns[i][0] != '!')
		{
			oneInclusionPatternFound = true;
			break;
		}
	}
	return !oneInclusionPatternFound;
}

generic_string GetLastErrorAsString(DWORD errorCode)
{
	generic_string errorMsg(_T(""));
	// Get the error message, if any.
	// If both error codes (passed error n GetLastError) are 0, then return empty
	if (errorCode == 0)
		errorCode = GetLastError();
	if (errorCode == 0)
		return errorMsg; //No error message has been recorded

	LPWSTR messageBuffer = nullptr;
	FormatMessageW(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
		nullptr, errorCode, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPWSTR)&messageBuffer, 0, nullptr);

	//errorMsg += messageBuffer;

	//Free the buffer.
	LocalFree(messageBuffer);

	return errorMsg;
}

HWND CreateToolTip(int toolID, HWND hDlg, HINSTANCE hInst, const PTSTR pszText, bool isRTL)
{
	if (!toolID || !hDlg || !pszText)
	{
		return NULL;
	}

	// Get the window of the tool.
	HWND hwndTool = GetDlgItem(hDlg, toolID);
	if (!hwndTool)
	{
		return NULL;
	}

	// Create the tooltip. g_hInst is the global instance handle.
	HWND hwndTip = CreateWindowEx(isRTL ? WS_EX_LAYOUTRTL : 0, TOOLTIPS_CLASS, NULL,
		WS_POPUP | TTS_ALWAYSTIP | TTS_BALLOON,
		CW_USEDEFAULT, CW_USEDEFAULT,
		CW_USEDEFAULT, CW_USEDEFAULT,
		hDlg, NULL,
		hInst, NULL);

	if (!hwndTip)
	{
		return NULL;
	}

//	NppDarkMode::setDarkTooltips(hwndTip, NppDarkMode::ToolTipsType::tooltip);

	// Associate the tooltip with the tool.
	TOOLINFO toolInfo = {};
	toolInfo.cbSize = sizeof(toolInfo);
	toolInfo.hwnd = hDlg;
	toolInfo.uFlags = TTF_IDISHWND | TTF_SUBCLASS;
	toolInfo.uId = (UINT_PTR)hwndTool;
	toolInfo.lpszText = pszText;
	if (!SendMessage(hwndTip, TTM_ADDTOOL, 0, (LPARAM)&toolInfo))
	{
		DestroyWindow(hwndTip);
		return NULL;
	}

	SendMessage(hwndTip, TTM_ACTIVATE, TRUE, 0);
	SendMessage(hwndTip, TTM_SETMAXTIPWIDTH, 0, 200);
	// Make tip stay 15 seconds
	SendMessage(hwndTip, TTM_SETDELAYTIME, TTDT_AUTOPOP, MAKELPARAM((15000), (0)));

	return hwndTip;
}

HWND CreateToolTipRect(int toolID, HWND hWnd, HINSTANCE hInst, const PTSTR pszText, const RECT rc)
{
	if (!toolID || !hWnd || !pszText)
	{
		return NULL;
	}

	// Create the tooltip. g_hInst is the global instance handle.
	HWND hwndTip = CreateWindowEx(0, TOOLTIPS_CLASS, NULL,
		WS_POPUP | TTS_ALWAYSTIP | TTS_BALLOON,
		CW_USEDEFAULT, CW_USEDEFAULT,
		CW_USEDEFAULT, CW_USEDEFAULT,
		hWnd, NULL,
		hInst, NULL);

	if (!hwndTip)
	{
		return NULL;
	}

	// Associate the tooltip with the tool.
	TOOLINFO toolInfo = {};
	toolInfo.cbSize = sizeof(toolInfo);
	toolInfo.hwnd = hWnd;
	toolInfo.uFlags = TTF_SUBCLASS;
	toolInfo.uId = toolID;
	toolInfo.lpszText = pszText;
	toolInfo.rect = rc;
	if (!SendMessage(hwndTip, TTM_ADDTOOL, 0, (LPARAM)&toolInfo))
	{
		DestroyWindow(hwndTip);
		return NULL;
	}

	SendMessage(hwndTip, TTM_ACTIVATE, TRUE, 0);
	SendMessage(hwndTip, TTM_SETMAXTIPWIDTH, 0, 200);
	// Make tip stay 15 seconds
	SendMessage(hwndTip, TTM_SETDELAYTIME, TTDT_AUTOPOP, MAKELPARAM((15000), (0)));

	return hwndTip;
}

std::wstring s2ws(const std::string& str)
{
	using convert_typeX = std::codecvt_utf8<wchar_t>;
	std::wstring_convert<convert_typeX, wchar_t> converterX("Error in Notepad++ string conversion s2ws!", L"Error in Notepad++ string conversion s2ws!");

	return converterX.from_bytes(str);
}

std::string ws2s(const std::wstring& wstr)
{
	using convert_typeX = std::codecvt_utf8<wchar_t>;
	std::wstring_convert<convert_typeX, wchar_t> converterX("Error in Notepad++ string conversion ws2s!", L"Error in Notepad++ string conversion ws2s!");

	return converterX.to_bytes(wstr);
}

bool deleteFileOrFolder(const generic_string& f2delete)
{
	auto len = f2delete.length();
	TCHAR* actionFolder = new TCHAR[len + 2];
	//wcscpy_s(actionFolder, len + 2, f2delete.c_str());
	strcpy(actionFolder,/*, len + 2,*/ f2delete.c_str());
	actionFolder[len] = 0;
	actionFolder[len + 1] = 0;

	SHFILEOPSTRUCT fileOpStruct = {};
	fileOpStruct.hwnd = NULL;
	fileOpStruct.pFrom = actionFolder;
	fileOpStruct.pTo = NULL;
	fileOpStruct.wFunc = FO_DELETE;
	fileOpStruct.fFlags = FOF_NOCONFIRMATION | FOF_SILENT | FOF_ALLOWUNDO;
	fileOpStruct.fAnyOperationsAborted = false;
	fileOpStruct.hNameMappings = NULL;
	fileOpStruct.lpszProgressTitle = NULL;

	int res = SHFileOperation(&fileOpStruct);

	delete[] actionFolder;
	return (res == 0);
}

// Get a vector of full file paths in a given folder. File extension type filter should be *.*, *.xml, *.dll... according the type of file you want to get.  
void getFilesInFolder(std::vector<generic_string>& files, const generic_string& extTypeFilter, const generic_string& inFolder)
{
	generic_string filter = inFolder;
	pathAppend(filter, extTypeFilter);

	WIN32_FIND_DATA foundData;
	HANDLE hFindFile = ::FindFirstFile(filter.c_str(), &foundData);

	if (hFindFile != INVALID_HANDLE_VALUE && !(foundData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
	{
		generic_string foundFullPath = inFolder;
		pathAppend(foundFullPath, foundData.cFileName);
		files.push_back(foundFullPath);

		while (::FindNextFile(hFindFile, &foundData))
		{
			generic_string foundFullPath2 = inFolder;
			pathAppend(foundFullPath2, foundData.cFileName);
			files.push_back(foundFullPath2);
		}
	}
	::FindClose(hFindFile);
}

void trim(generic_string& str)
{
	// remove any leading or trailing spaces from str

	generic_string::size_type pos = str.find_last_not_of(' ');

	if (pos != generic_string::npos)
	{
		str.erase(pos + 1);
		pos = str.find_first_not_of(' ');
		if (pos != generic_string::npos) str.erase(0, pos);
	}
	else str.erase(str.begin(), str.end());
}

int nbDigitsFromNbLines(size_t nbLines)
{
	int nbDigits = 0; // minimum number of digit should be 4
	if (nbLines < 10) nbDigits = 1;
	else if (nbLines < 100) nbDigits = 2;
	else if (nbLines < 1000) nbDigits = 3;
	else if (nbLines < 10000) nbDigits = 4;
	else if (nbLines < 100000) nbDigits = 5;
	else if (nbLines < 1000000) nbDigits = 6;
	else // rare case
	{
		nbDigits = 7;
		nbLines /= 10000000;

		while (nbLines)
		{
			nbLines /= 10;
			++nbDigits;
		}
	}
	return nbDigits;
}

namespace
{
	/*constexpr*/ TCHAR timeFmtEscapeChar = 0x1;
	/*constexpr*/ TCHAR middayFormat[] = _T("tt");

	// Returns AM/PM string defined by the system locale for the specified time.
	// This string may be empty or customized.
	generic_string getMiddayString(const TCHAR* localeName, const SYSTEMTIME& st)
	{
		generic_string midday;
		midday.resize(MAX_PATH);
		int ret = GetTimeFormatEx((LPCWSTR)localeName, 0, &st, (LPCWSTR)middayFormat, (LPWSTR)&midday[0], static_cast<int>(midday.size()));
		if (ret > 0)
			midday.resize(ret - 1); // Remove the null-terminator.
		else
			midday.clear();
		return midday;
	}

	// Replaces conflicting time format specifiers by a special character.
	bool escapeTimeFormat(generic_string& format)
	{
		bool modified = false;
		for (auto& ch : format)
		{
			if (ch == middayFormat[0])
			{
				ch = timeFmtEscapeChar;
				modified = true;
			}
		}
		return modified;
	}

	// Replaces special time format characters by actual AM/PM string.
	/*void unescapeTimeFormat(generic_string& format, const generic_string& midday)
	{
		if (midday.empty())
		{
			auto it = std::remove(format.begin(), format.end(), timeFmtEscapeChar);
			if (it != format.end())
				format.erase(it, format.end());
		}
		else
		{
			size_t i = 0;
			while ((i = format.find(timeFmtEscapeChar, i)) != generic_string::npos)
			{
				if (i + 1 < format.size() && format[i + 1] == timeFmtEscapeChar)
				{
					// 'tt' => AM/PM
					format.erase(i, std::size(middayFormat) - 1);
					format.insert(i, midday);
				}
				else
				{
					// 't' => A/P
					format[i] = midday[0];
				}
			}
		}
	}*/
}

generic_string getDateTimeStrFrom(const generic_string& dateTimeFormat, const SYSTEMTIME& st)
{
	const TCHAR* localeName = LOCALE_NAME_USER_DEFAULT;
	const DWORD flags = 0;

	///*constexpr*/ int bufferSize = MAX_PATH;
	TCHAR buffer[MAX_PATH] = {};
	int ret = 0;


	// 1. Escape 'tt' that means AM/PM or 't' that means A/P.
	// This is needed to avoid conflict with 'M' date format that stands for month.
	generic_string newFormat = dateTimeFormat;
	const bool hasMiddayFormat = escapeTimeFormat(newFormat);

	// 2. Format the time (h/m/s/t/H).
	ret = GetTimeFormatEx((LPCWSTR)localeName, flags, &st, (LPCWSTR)newFormat.c_str(), (LPWSTR)buffer, MAX_PATH);
	if (ret != 0)
	{
		// 3. Format the date (d/y/g/M). 
		// Now use the buffer as a format string to process the format specifiers not recognized by GetTimeFormatEx().
		ret = GetDateFormatEx((LPCWSTR)localeName, flags, &st, (LPCWSTR)buffer, (LPWSTR)buffer, MAX_PATH, nullptr);
	}

	if (ret != 0)
	{
		if (hasMiddayFormat)
		{
			// 4. Now format only the AM/PM string.
			const generic_string midday = getMiddayString(localeName, st);
			generic_string result = buffer;
			//unescapeTimeFormat(result, midday);
			return result;
		}
		return buffer;
	}

	return {};
}

// Don't forget to use DeleteObject(createdFont) before leaving the program
HFONT createFont(const TCHAR* fontName, int fontSize, bool isBold, HWND hDestParent)
{
	HDC hdc = GetDC(hDestParent);

	LOGFONT logFont = {};
	logFont.lfHeight = -MulDiv(fontSize, GetDeviceCaps(hdc, LOGPIXELSY), 72);
	if (isBold)
		logFont.lfWeight = FW_BOLD;

	//wcscpy_s(logFont.lfFaceName, fontName);
	StrCpy(logFont.lfFaceName, fontName);

	HFONT newFont = CreateFontIndirect(&logFont);

	ReleaseDC(hDestParent, hdc);

	return newFont;
}

// "For file I/O, the "\\?\" prefix to a path string tells the Windows APIs to disable all string parsing
// and to send the string that follows it straight to the file system..."
// Ref: https://learn.microsoft.com/en-us/windows/win32/fileio/naming-a-file#win32-file-namespaces
bool isWin32NamespacePrefixedFileName(const generic_string& fileName)
{
	// TODO:
	// ?! how to handle similar NT Object Manager path style prefix case \??\...
	// (the \??\ prefix instructs the NT Object Manager to search in the caller's local device directory for an alias...)

	// the following covers the \\?\... raw Win32-filenames or the \\?\UNC\... UNC equivalents
	// and also its *nix like forward slash equivalents
	return 0;// (fileName.starts_with(TEXT("\\\\?\\")) || fileName.starts_with(TEXT("//?/")));
}

bool isWin32NamespacePrefixedFileName(const TCHAR* szFileName)
{
	const generic_string fileName = szFileName;
	return isWin32NamespacePrefixedFileName(fileName);
}

bool isUnsupportedFileName(const generic_string& fileName)
{
	bool isUnsupported = true;

	// until the Notepad++ (and its plugins) will not be prepared for filenames longer than the MAX_PATH,
	// we have to limit also the maximum supported length below
	/*if ((fileName.size() > 0) && (fileName.size() < MAX_PATH))
	{
		// possible raw filenames can contain space(s) or dot(s) at its end (e.g. "\\?\C:\file."), but the Notepad++ advanced
		// Open/SaveAs IFileOpenDialog/IFileSaveDialog COM-interface based dialogs currently do not handle this well
		// (but e.g. direct Notepad++ Ctrl+S works ok even with these filenames)
		if (!fileName.ends_with(_T('.')) && !fileName.ends_with(_T(' ')))
		{
			bool invalidASCIIChar = false;

			for (size_t pos = 0; pos < fileName.size(); ++pos)
			{
				TCHAR c = fileName.at(pos);
				if (c <= 31)
				{
					invalidASCIIChar = true;
				}
				else
				{
					// as this could be also a complete filename with path and there could be also a globbing used,
					// we tolerate here some other reserved Win32-filename chars: /, \, :, ?, *
					switch (c)
					{
						case '<':
						case '>':
						case '"':
						case '|':
							invalidASCIIChar = true;
							break;
					}
				}

				if (invalidASCIIChar)
					break;
			}

			if (!invalidASCIIChar)
			{
				// strip input string to a filename without a possible path and extension(s)
				generic_string fileNameOnly;
				size_t pos = fileName.find_first_of(TEXT("."));
				if (pos != std::string::npos)
					fileNameOnly = fileName.substr(0, pos);
				else
					fileNameOnly = fileName;

				pos = fileNameOnly.find_last_of(TEXT("\\"));
				if (pos == std::string::npos)
					pos = fileNameOnly.find_last_of(TEXT("/"));
				if (pos != std::string::npos)
					fileNameOnly = fileNameOnly.substr(pos + 1);

				const std::vector<generic_string>  reservedWin32NamespaceDeviceList{
				TEXT("CON"), TEXT("PRN"), TEXT("AUX"), TEXT("NUL"),
				TEXT("COM1"), TEXT("COM2"), TEXT("COM3"), TEXT("COM4"), TEXT("COM5"), TEXT("COM6"), TEXT("COM7"), TEXT("COM8"), TEXT("COM9"),
				TEXT("LPT1"), TEXT("LPT2"), TEXT("LPT3"), TEXT("LPT4"), TEXT("LPT5"), TEXT("LPT6"), TEXT("LPT7"), TEXT("LPT8"), TEXT("LPT9")
				};

				// last check is for all the old reserved Windows OS filenames
				if (std::find(reservedWin32NamespaceDeviceList.begin(), reservedWin32NamespaceDeviceList.end(), fileNameOnly) == reservedWin32NamespaceDeviceList.end())
				{
					// ok, the current filename tested is not even on the blacklist
					isUnsupported = false;
				}
			}
		}
	}*/

	return isUnsupported;
}

bool isUnsupportedFileName(const TCHAR* szFileName)
{
	const generic_string fileName = szFileName;
	return isUnsupportedFileName(fileName);
}


BOOL IsCtrlPressed()
{
	return ((GetKeyState(VK_CONTROL) & (1 << (sizeof(SHORT)* 8 - 1))) != 0);
}

BOOL IsAltPressed()
{
	return ((GetKeyState(VK_MENU) & (1 << (sizeof(SHORT)* 8 - 1))) != 0);
}

BOOL IsShiftPressed()
{
	return ((GetKeyState(VK_SHIFT) & (1 << (sizeof(SHORT)* 8 - 1))) != 0);
}