#pragma once
#include "./interface/cDialog.h"
#include "./interface/cStatic.h"
#include "cEditBox.h"

class RegID : public cDialog
{
	cDialog * p_Dlg;
	cButton* m_RegBtn;
	cButton* m_RegNo;
	char m_Pwd1[20];

	//float XX;
	//float YY;

	float SavedXX;
	float SavedYY;
public:
	RegID();
	~RegID();
	void Linking();
	void Show();
	void Close();
	BOOL IsInvalidCharInclude( char* pStr );
	BOOL TelIsInvalidCharInclude( char* pStr ); 
	BOOL OnActionEvent(LONG lId, void * p, DWORD we);
	//virtual void Render();
	//BOOL RegGetMac(char* mac);
};



