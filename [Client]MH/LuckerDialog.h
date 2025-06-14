#ifndef _LUCKERDIALOG_
#define _LUCKERDIALOG_

#include "./interface/cDialog.h"

class cStatic;
class CItem;
class cIconDialog;
class cIconGridDialog;

#define LUCKERMGR USINGTON(CLuckerDialog)

class CLuckerDialog : public cDialog
{
	cIconGridDialog *	m_pLuckerGridDlg;

	DWORD	m_dwStartTime;
	DWORD	m_dwStartTime1;

	static BOOL    IsStart;
	cDialog * pDlg ;
	cButton * m_pStart;

	int		lPos;
	int		alphalPos;
public:


	DWORD dwCurTime;

	CLuckerDialog();

	virtual ~CLuckerDialog();

     void Linking();

	 void AddItemToGrid();

	 void Process();

	 void StartProcess();

	 void EndProcess();

	 void SendNetWorkMessage();

	 void SetGridSelect(int Pos);

	 void OnActionEvent(LONG lId, void * p, DWORD we);

	 virtual void Render();

};
EXTERNGLOBALTON(CLuckerDialog)
#endif //