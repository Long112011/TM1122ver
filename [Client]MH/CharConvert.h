#pragma once
char* BIG5ToGB2312(const char* szBIG5String);
char* UnicodeToGB2312(const wchar_t* szUnicodeString);
wchar_t* BIG5ToUnicode(const char* szBIG5String);
char* GB2312ToBIG5(const char* szGBString);
wchar_t* GB2312ToUnicode(const char* szGBString);
char* UnicodeToBIG5(const wchar_t* szUnicodeString);