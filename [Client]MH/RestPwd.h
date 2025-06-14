#pragma once
#include "./interface/cDialog.h"
#include "./interface/cStatic.h"
#include "cEditBox.h"
enum {FORREGISTER1=0,FORREGISTER2=1,FORCHANGEPASS1=2,FORCHANGEPASS2=3,};
class RestPwd : public cDialog
{
	//m_ResButton[FORREGISTER]
	//m_ResButton[FORCHANGEPASS]
	//cButton * m_ResButton[4];

	cButton* m_RegBtn;
	cButton* m_RegNo;

	char m_Pwd1[20];
	char m_SaveID[20];
	char m_SavePASS[20];
	//float XX;
	//float YY;

	float SavedXX;
	float SavedYY;

	cEditBox*editboxId;
	cEditBox*editboxPwd;

	cEditBox*editboxNewPwd;
	cEditBox*editboxNewPwd1;
	cEditBox*editboxTel;
	cEditBox*editboxEmail;
public:
	RestPwd();
	~RestPwd();
	void Linking();
	void Show();
	void Close();
	BOOL IsInvalidCharInclude( char* pStr );
	BOOL TelIsInvalidCharInclude( char* pStr );
	BOOL OnActionEvent(LONG lId, void * p, DWORD we);
	void SetIDPASS(char*id, char*pass);

};



