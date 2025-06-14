#pragma once
#include "./interface/cDialog.h"
class cPushupButton;
class CFPSDialog : public cDialog
{
	cDialog * p_Dlg;
	cStatic*p_Static;
	cPushupButton*p_PushUpBtn;

	DWORD p_Alpha;
	BOOL p_EnableAlpha;
public:

	CFPSDialog();
	virtual ~CFPSDialog();

	void Linking();
	void OnActionEvent(LONG lId, void * p, DWORD we);
	void SetPinned(BOOL val);
	virtual DWORD ActionEvent(CMouse * mouseInfo);
	virtual void	Render();
	void AlphaRender();
	void SetFPS(char * c);
};