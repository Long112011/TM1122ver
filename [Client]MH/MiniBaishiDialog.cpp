#include "stdafx.h"
#include "MiniBaishiDialog.h"
#include "WindowIDEnum.h"
#include "cImeEx.h"

CBaiShiDialog::CBaiShiDialog()
{
	
}

CBaiShiDialog::~CBaiShiDialog()
{

}
void CBaiShiDialog::Linking()
{
	m_pName = (cStatic*)GetWindowForID(MAR_SHITUNAME);
	m_pNameEdit = (cEditBox*)GetWindowForID(MAR_SHITUNAMEEDIT);
	m_pNameEdit->SetValidCheck(VCM_CHARNAME);
	m_pNameEdit->SetEditText("");
	m_pAddOkBtn = (cButton*)GetWindowForID(MAR_SHITUADDOKBTN);
	m_pAddCancelBtn = (cButton*)GetWindowForID(MAR_SHITUADDCANCELBTN);

}

void CBaiShiDialog::SetActive( BOOL val )
{
	if( m_bDisable ) return;

	if(val)
		m_pNameEdit->SetEditText("");
	cDialog::SetActiveRecursive(val);
}

void CBaiShiDialog::SetName( char* Name )
{
	m_pNameEdit->SetEditText(Name);
}
