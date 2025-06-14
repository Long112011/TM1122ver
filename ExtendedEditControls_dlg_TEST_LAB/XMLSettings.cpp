// XMLSettings.cpp : implementation file
//

#include "stdafx.h"
//#include "XmlSettingsTest.h"
#include "XMLSettings.h"

/////////////////////////////////////////////////////////////////////////////
// CXMLSettings

// get a long value
long CXMLSettings::GetSettingLong(CString cstrBaseKeyName, CString cstrValueName, long lDefaultValue)
{
	/* 
		Since XML is text based and we have no schema, just convert to a string and 
		call the GetSettingString method.
	*/
	long lRetVal = lDefaultValue;
	CString cstrDefaultValue;

	cstrDefaultValue.Format("%d", lRetVal);

	lRetVal = atol(GetSettingString(cstrBaseKeyName, cstrValueName, cstrDefaultValue));

	return lRetVal;
}

// get a string value
CString CXMLSettings::GetSettingString(CString cstrBaseKeyName, CString cstrValueName, CString cstrDefaultValue)
{
	int iNumKeys = 0;
	CString cstrValue = cstrDefaultValue;
	CString* pCStrKeys = NULL;

	// Add the value to the base key separated by a '\'
	cstrBaseKeyName += "\\";
	cstrBaseKeyName += cstrValueName;

	// Parse all keys from the base key name (keys separated by a '\')
	pCStrKeys = ParseKeys(cstrBaseKeyName, iNumKeys);

	// Traverse the xml using the keys parsed from the base key name to find the correct node
	if (pCStrKeys)
	{
		if (LoadXML())  // load the xml document
		{
			MSXML2::IXMLDOMElementPtr rootElem = NULL;
			MSXML2::IXMLDOMNodePtr foundNode = NULL;

			XmlDocPtr->get_documentElement(&rootElem);  // root node

			if (rootElem)
			{
				// returns the last node in the chain
				foundNode = FindNode(rootElem, pCStrKeys, iNumKeys); 
				
				if (foundNode)
				{
					// get the text of the node (will be the value we requested)
					BSTR bstr = NULL;
					foundNode->get_text(&bstr);

					cstrValue = (CString)bstr;

					if (bstr) { SysFreeString(bstr); bstr = NULL; }

					foundNode = NULL;
				}

				rootElem = NULL;
			}
		}

		UnloadXML();  // dump the XML document

		delete [] pCStrKeys;
		pCStrKeys = NULL;
	}

	return cstrValue;
}

// set a long value
long CXMLSettings::SetSettingLong(CString cstrBaseKeyName, CString cstrValueName, long lValue)
{
	/* 
		Since XML is text based and we have no schema, just convert to a string and 
		call the SetSettingString method.
	*/
	long lRetVal = 0;
	CString cstrValue = "";

	cstrValue.Format("%d", lValue);

	lRetVal = SetSettingString(cstrBaseKeyName, cstrValueName, cstrValue);

	return lRetVal;
}

// set a string value
long CXMLSettings::SetSettingString(CString cstrBaseKeyName, CString cstrValueName, CString cstrValue)
{
	/*
		RETURN VALUES:
		 0 = SUCCESS
		-1 = LOAD FAILED
		-2 = NODE NOT FOUND
		-3 = PUT TEXT FAILED
		-4 = SAVE FAILED
	*/

	long lRetVal = 0;
	int iNumKeys = 0;
	CString* pCStrKeys = NULL;

	// Add the value to the base key separated by a '\'
	cstrBaseKeyName += "\\";
	cstrBaseKeyName += cstrValueName;

	// Parse all keys from the base key name (keys separated by a '\')
	pCStrKeys = ParseKeys(cstrBaseKeyName, iNumKeys);

	// Traverse the xml using the keys parsed from the base key name to find the correct node
	if (pCStrKeys)
	{
		if (LoadXML())  // load the xml document
		{
			MSXML2::IXMLDOMElementPtr rootElem = NULL;
			MSXML2::IXMLDOMNodePtr foundNode = NULL;

			XmlDocPtr->get_documentElement(&rootElem);  // root node
			
			if (rootElem)
			{
				// returns the last node in the chain
				foundNode = FindNode(rootElem, pCStrKeys, iNumKeys, TRUE); 

				if (foundNode)
				{
					// set the text of the node (will be the value we sent)
					HRESULT hr = foundNode->put_text(_bstr_t(cstrValue));
					if (SUCCEEDED(hr))
					{
						if (!SaveXML()) { lRetVal = -4; }  // save the changed XML
					}
					else
						lRetVal = -3;

					foundNode = NULL;
				}
				else
					lRetVal = -2;

				rootElem = NULL;
			}
		}
		else
			lRetVal = -1;

		UnloadXML();  // dump the XML document

		delete [] pCStrKeys;
		pCStrKeys = NULL;
	}

	return lRetVal;
}

// delete a key or chain of keys
BOOL CXMLSettings::DeleteSetting(CString cstrBaseKeyName, CString cstrValueName)
{
	BOOL bRetVal = FALSE;
	int iNumKeys = 0;
	CString* pCStrKeys = NULL;

	if (!cstrValueName.IsEmpty()) cstrBaseKeyName += "\\" + cstrValueName;

	// Parse all keys from the base key name (keys separated by a '\')
	pCStrKeys = ParseKeys(cstrBaseKeyName, iNumKeys);

	// Traverse the xml using the keys parsed from the base key name to find the correct node.
	if (pCStrKeys)
	{
		if (LoadXML())  // load the xml document
		{
			MSXML2::IXMLDOMElementPtr rootElem = NULL;
			MSXML2::IXMLDOMNodePtr foundNode = NULL;

			XmlDocPtr->get_documentElement(&rootElem);  // root node

			if (rootElem)
			{
				// returns the last node in the chain
				foundNode = FindNode(rootElem, pCStrKeys, iNumKeys); 

				if (foundNode)
				{
					// get the parent of the found node and use removeChild to delete the found node
					MSXML2::IXMLDOMNodePtr parentNode = NULL;
					
					foundNode->get_parentNode(&parentNode);

					if (parentNode)
					{
						HRESULT hr = parentNode->removeChild(foundNode, NULL);
						if (SUCCEEDED(hr))
						{
							if (SaveXML()) bRetVal = TRUE;  // save the changed XML
						}

						parentNode = NULL;
					}

					foundNode = NULL;
				}

				rootElem = NULL;
			}

			UnloadXML();  // dump the XML document
		}

		delete [] pCStrKeys;
		pCStrKeys = NULL;
	}

	return bRetVal;
}

// Parse all keys from the base key name.
CString* CXMLSettings::ParseKeys(CString cstrFullKeyPath, int &iNumKeys)
{
	CString cstrTemp;
	CString* pCStrKeys = NULL;
	
	// replace spaces with _ since xml doesn't like them
	cstrFullKeyPath.Replace(' ', '_');

	if (cstrFullKeyPath.GetAt(cstrFullKeyPath.GetLength() - 1) == '\\')
		cstrFullKeyPath.TrimRight('\\');  // remove slashes on the end

	cstrTemp = cstrFullKeyPath;

	iNumKeys = cstrTemp.Remove('\\') + 1;  // get a count of slashes

	pCStrKeys = new CString[iNumKeys];  // create storage for the keys

	if (pCStrKeys)
	{
		int iFind = 0, iLastFind = 0, iCount = -1;
		
		// get all of the keys in the chain
		while (iFind != -1)
		{
			iFind = cstrFullKeyPath.Find("\\", iLastFind);
			if (iFind > -1)
			{
				iCount++;
				pCStrKeys[iCount] = cstrFullKeyPath.Mid(iLastFind, iFind - iLastFind);
				iLastFind = iFind + 1;
			}
			else
			{
				// make sure we don't just discard the last key in the chain
				if (iLastFind < cstrFullKeyPath.GetLength()) 
				{
					iCount++;
					pCStrKeys[iCount] = cstrFullKeyPath.Right(cstrFullKeyPath.GetLength() - iLastFind);
				}
			}
		}
	}

	return pCStrKeys;
}

// load the XML file into the parser
BOOL CXMLSettings::LoadXML()
{
	VARIANT_BOOL vbSuccessful;

	// initialize the Xml parser
	HRESULT hr = XmlDocPtr.CreateInstance(MSXML2::CLSID_DOMDocument);
	
	if (XmlDocPtr == NULL) return FALSE;

	// see if the file exists
	CFile file;
	if (!file.Open(cstrConfigFile, CFile::modeReadWrite))  // if not
	{
		// create it
		XmlDocPtr->loadXML(_bstr_t("<?xml version=\"1.0\" ?><XMLConfigSettings></XMLConfigSettings>"), &vbSuccessful);
	}
	else  // if so
	{
		file.Close();

		// load it
		XmlDocPtr->load(CComVariant::CComVariant((LPCSTR)cstrConfigFile), &vbSuccessful);
	}

	if (vbSuccessful == VARIANT_TRUE)
		return TRUE;  // loaded
	else
	{
		// an XML load error occurred so display the reason
		MSXML2::IXMLDOMParseErrorPtr pIParseError = NULL;
		XmlDocPtr->get_parseError(&pIParseError);

		if (pIParseError)
		{
			long value;
			BSTR bstr = NULL;

			HRESULT hr = pIParseError->get_errorCode(&value);
			pIParseError->get_reason(&bstr);

			CString cstrMessage;
			cstrMessage.Format("%s", (char *)_bstr_t(bstr, TRUE));

			if (!cstrMessage.IsEmpty()) 
			{
				MessageBox(NULL, cstrMessage, "LoadXML Malformed XML Error", MB_OK);
			}

			if (bstr) { SysFreeString(bstr); bstr = NULL; }

			pIParseError = NULL;
		}

		return FALSE;
	}	
}

// save the XML file
BOOL CXMLSettings::SaveXML()
{
	HRESULT hr = XmlDocPtr->save(CComVariant::CComVariant(cstrConfigFile));

	if (SUCCEEDED(hr))
		return TRUE;
	else
		return FALSE;
}

// find a node given a chain of key names
MSXML2::IXMLDOMNodePtr CXMLSettings::FindNode(MSXML2::IXMLDOMNodePtr parentNode, 
												  CString* pCStrKeys, int iNumKeys, 
												  BOOL bAddNodes /*= FALSE*/)
{
	MSXML2::IXMLDOMNodePtr foundNode = NULL;
	MSXML2::IXMLDOMElementPtr rootElem = NULL, tempElem = NULL;

	XmlDocPtr->get_documentElement(&rootElem);  // root element

	for (int i=0; i<iNumKeys; i++)
	{
		// find the node named X directly under the parent
		HRESULT hr = parentNode->selectSingleNode(_bstr_t(pCStrKeys[i]), &foundNode);

		if (FAILED(hr) || foundNode == NULL) 
		{
			// if its not found...
			if (bAddNodes)  // create the node and append to parent (Set only)
			{
				XmlDocPtr->createElement(_bstr_t(pCStrKeys[i]), &tempElem);
				if (tempElem) 
				{
					parentNode->appendChild(tempElem, &foundNode);

					// since we are traversing the nodes, we need to set the parentNode to our foundNode
					parentNode = NULL;
					parentNode = foundNode;
					foundNode = NULL;
				}
			}
			else
			{
				foundNode = NULL;
				parentNode = NULL;
				break;
			}
		}
		else
		{
			// since we are traversing the nodes, we need to set the parentNode to our foundNode
			parentNode = NULL;
			parentNode = foundNode;
			foundNode = NULL;
		}
	}

	rootElem = NULL;

	return parentNode;
}
