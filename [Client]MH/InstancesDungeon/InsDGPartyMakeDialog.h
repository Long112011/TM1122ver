#ifndef _INSDG_PARTYMAKE_DLG_H
#define _INSDG_PARTYMAKE_DLG_H
#pragma once
#include "./Interface/cDialog.h"
class cStatic;
class CInsDGPartyMakeDialog : public cDialog
{
public:
	CInsDGPartyMakeDialog();
	~CInsDGPartyMakeDialog();
	void Linking();
	void OnActionEvent( LONG lId, void * p, DWORD we );		
};
#endif 