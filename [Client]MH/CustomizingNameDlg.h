



// NameChangeDialog.h: interface for the CNameChangeDialog class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CUSTOMIZINGNAMEDIALOG_H__73CE85B3_4B5D_4484_A158_A3067F4458BF__INCLUDED_)

#define AFX_CUSTOMIZINGNAMEDIALOG_H__73CE85B3_4B5D_4484_A158_A3067F4458BF__INCLUDED_

#if _MSC_VER > 1000

#pragma once
#endif // _MSC_VER > 1000



#include "./Interface/cDialog.h"

class cEditBox;



class CCustomizingNameDialog : public cDialog

{

	cEditBox* m_pNameBox;
	DWORD       m_DBIdx;
public:
	CCustomizingNameDialog();
	virtual ~CCustomizingNameDialog();

	void Linking();
	void SetActive(BOOL val);
	void CreatCustomizingNameSyn();
	DWORD GetItemDBIdx() { return m_DBIdx; }
};

#endif // !defined(AFX_NAMECHANGEDIALOG_H__73CE85B3_4B5D_4484_A158_A3067F4458BF__INCLUDED_)


