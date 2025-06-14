
#pragma once
#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif
#include "resource.h"	
#include "singleinstance.h"	
class CPackingToolApp : public CWinApp, public CSingleInstance
{
public:
	CPackingToolApp();	
public:
	virtual BOOL InitInstance();
	void LoadRegistry(BYTE flag=0);
	void CheckLoadedRegistry();
	virtual void WakeUp(LPCTSTR aCommandLine) const;
	DECLARE_MESSAGE_MAP()
};
extern CPackingToolApp App;