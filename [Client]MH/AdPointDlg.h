#pragma once
#include "interface\cdialog.h"
#include "./Interface/cPushupButton.h"
#include "WindowIDEnum.h"
//#include "./Interface/cCheckBox.h"
#include "cEditBox.h"
//#include "string.h"

#define ADDPOINTDLG GAMEIN->GetAdPointDlg()

class cEditBox;
class cDialog;
class cCheckBox;

class AdPointDlg :public cDialog
{
	DWORD m_dwDBIdx;
	cButton* m_AddPointOk;
	cEditBox*  m_Residue;
	cEditBox*  m_Power;
	cEditBox*  m_Dex;
	cEditBox*  m_Con;
	cEditBox* m_Xm;
	
	int m_ResidueCount;
	int m_PowerCount;
	int m_DexCount;
	int m_ConCount;
	int m_XmCount;
	int m_count;
public:
	AdPointDlg(void);
	virtual ~AdPointDlg(void);

	void Show(int m_ResidueCount);
	void Close();
	void AddPoint();
	void CalcPoint();
	void Process();
	void Linking();
	void OnActionEvent(LONG lId, void * p, DWORD we);
	void SetItemDBIdx(DWORD dbIdx)		{	m_dwDBIdx = dbIdx;	}
	DWORD GetItemDBIdx()				{	return m_dwDBIdx;	}
	void UpData();
	void Zero();
};