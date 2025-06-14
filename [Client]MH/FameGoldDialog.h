#if !defined(AFX_FAMEGOLDDLG_H__8C774E0A_9D8C_4535_A3B2_F847517FA424__INCLUDED_)
#define AFX_FAMEGOLDDLG_H__8C774E0A_9D8C_4535_A3B2_F847517FA424__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "./Interface/cDialog.h"
#include "cSpin.h"

class CFameGoldDialog :  public cDialog
{

	  cDialog *    m_Dlg;
	  cStatic  *    m_FameValue;
	  cSpin    *    m_FrameSet;

public:

      CFameGoldDialog();

	  virtual  ~CFameGoldDialog();

	  void Linking();

	  void  SetFame();

	  BOOL OnActionEvent(LONG lId, void * p, DWORD we);

	  void JustForKen();

};

#endif