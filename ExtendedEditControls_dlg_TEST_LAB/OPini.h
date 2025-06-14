#pragma once
#include "XMLSettings.h"
class COPini  
{
protected:
//	CXMLSettings xmlSettings;
public:
	static DWORD ReadString (char *section, char * key,  char stringtoread[],  char * filename);
	static BOOL WriteString(LPCTSTR section, LPCTSTR key,char* stringtoadd, char *filename);
	static CString Load(char *section, char * key, char stringtoread[], char * defaultStr);
	static BOOL Set(LPCTSTR section, LPCTSTR key, char* stringtoadd/*, char *filename*/);

/*	afx_msg virtual COLORREF LoadXmlandDefaultLong(CString cstrKey, CString cstrValueName, long	cstrValue = 0);
	afx_msg virtual CString LoadXmlandDefaultString(CString cstrKey, CString cstrValueName, char*  cstrRetVal = "");
	afx_msg virtual void SetXmlNewValue(CString cstrKey, CString cstrValueName, char*  cstrRetVal = "");
	afx_msg virtual void SetXmlNewValueInt(CString cstrKey, CString cstrValueName, int cstrRetVal = 0);*/

	COPini();
	virtual ~COPini(); 

}; 
