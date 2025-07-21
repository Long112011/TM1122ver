#include "stdafx.h"

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
bool CLog::mIsEnable;

CLog::CLog()
{
	::AllocConsole();
}

CLog::~CLog()
{
	::FreeConsole();
}

void CLog::SetEnable(bool isEnable)
{
	mIsEnable = isEnable;
}

void CLog::Put(const TCHAR* text, ...)
{
//#ifndef _DEBUG
	if (!mIsEnable)
	{
		return;
	}
//#endif

	static CLog instance;

	const CTime time(CTime::GetCurrentTime());

	CString log;

	TCHAR buffer[MAX_PATH * 5] = { 0 };
	{
		va_list argument;
		va_start(argument, text);
		_vstprintf(buffer, text, argument);
		va_end(argument);
	}

	log.Format(
		_T("%04d.%02d.%02d %02d:%02d:%02d> %s\r\n"),
		time.GetYear(),
		time.GetMonth(),
		time.GetDay(),
		time.GetHour(),
		time.GetMinute(),
		time.GetSecond(),
		buffer);

	//fprintf(stderr,log);
	_tprintf(log);

	// ?? ??
	/*{
		CString name;
		{
			CString infix;
			infix.Format(
				_T("%04d%02d%02d"),
				time.GetYear(),
				time.GetMonth(),
				time.GetDay());

			name = CString(_T("Launcher")) + _T(".") + infix + _T(".") + _T("log");
		}

		CStdioFileEx file;

		// ?? ???? ??, ??? ????
		if (!file.Open(name, CFile::modeWrite) &&
			!file.Open(name, CFile::modeCreate | CFile::modeWrite))
		{
			return;
		}

		file.Seek(file.GetLength(), 0);
		file.WriteString(log);
	}*/
}