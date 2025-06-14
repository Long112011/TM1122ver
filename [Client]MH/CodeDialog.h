#pragma once
#include "INTERFACE\cDialog.h"
#include ".\interface\cWindowHeader.h"
class CCodeDialog : public cDialog  
{
protected:
	cStatic * m_pName;
	cEditBox * m_pNameEdit;
	cButton * m_pAddOkBtn;
	cButton * m_pAddCancelBtn;
public:
	CCodeDialog();
	virtual ~CCodeDialog();
	void Linking();
	virtual void SetActive(BOOL val);
	void SetName(char* Name);
};