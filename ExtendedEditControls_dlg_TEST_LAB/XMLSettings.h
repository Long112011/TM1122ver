#if !defined(XMLSETTINGS_H)
#define XMLSETTINGS_H

#import "msxml4.dll" named_guids raw_interfaces_only

/////////////////////////////////////////////////////////////////////////////
// CXMLSettings window

#include <atlbase.h>

class CXMLSettings
{
// Construction
public:
	CXMLSettings()
	{
		XmlDocPtr = NULL;
		cstrConfigFile = "";
	}

// Implementation
public:
	void SetSettingsFile(CString cstrFile) { cstrConfigFile = cstrFile; };

	long GetSettingLong(CString cstrBaseKeyName, CString cstrValueName, long lDefaultValue);
	long SetSettingLong(CString cstrBaseKeyName, CString cstrValueName, long lValue);

	CString GetSettingString(CString cstrBaseKeyName, CString cstrValueName, CString cstrDefaultValue);
	long SetSettingString(CString cstrBaseKeyName, CString cstrValueName, CString cstrValue);

	BOOL DeleteSetting(CString cstrBaseKeyName, CString cstrValueName);

protected:
	MSXML2::IXMLDOMDocument2Ptr XmlDocPtr;
	CString cstrConfigFile;

	CString* ParseKeys(CString cstrFullKeyPath, int &iNumKeys);
	BOOL LoadXML();
	BOOL SaveXML();
	void UnloadXML() { XmlDocPtr = NULL; };
	MSXML2::IXMLDOMNodePtr FindNode(MSXML2::IXMLDOMNodePtr parentNode, CString* pCStrKeys, int iNumKeys, BOOL bAddNodes = FALSE);

};

#endif XMLSETTINGS_H

