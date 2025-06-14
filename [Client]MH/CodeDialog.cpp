#include "stdafx.h"
#include "CodeDialog.h"
#include "WindowIDEnum.h"
#include "./interface/cWindowManager.h"
#include "ObjectStateManager.h"
#include "ObjectManager.h"
#include "GameIn.h"
#include "ChatManager.h"
#include "cImeEx.h"
CCodeDialog::CCodeDialog()
{
}
CCodeDialog::~CCodeDialog()
{
}
void CCodeDialog::Linking()
{
	m_pName = (cStatic*)GetWindowForID(CD_STC_DESC);
	m_pNameEdit = (cEditBox*)GetWindowForID(CD_EDT_CODE);
	m_pNameEdit->SetValidCheck(VCM_CHARNAME);
	m_pNameEdit->SetEditText("");
	m_pAddOkBtn = (cButton*)GetWindowForID(CD_BTN_OK);
	m_pAddCancelBtn = (cButton*)GetWindowForID(CD_BTN_CANCEL);
}
void CCodeDialog::SetActive(BOOL val)
{
	if( m_bDisable ) return;
	if(val)
		m_pNameEdit->SetEditText("");
	cDialog::SetActiveRecursive(val);
}
void CCodeDialog::SetName(char* Name)
{
	m_pNameEdit->SetEditText(Name);
}