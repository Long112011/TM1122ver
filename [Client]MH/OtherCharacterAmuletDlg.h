#include "./interface/cDialog.h"
#include "./Interface/cIconGridDialog.h"

// 2014-11-10 增益功开发！

class COtherCharacterAmuletDlg : public cDialog
{

	DWORD m_LastTime;

	cDialog * p_Dlg;

    cIconGridDialog * p_Grid;

	cStatic * p_Name;

	cImage tmpImage;

public:

    COtherCharacterAmuletDlg();

	virtual ~ COtherCharacterAmuletDlg();

	DWORD GetLastTime();

	void  SetLastTime(DWORD Val);

	void  SetName(char * Name);

	void  SetAmuletInfo(int Pos,SHOPITEMBASE * pItem);

	void  ClearAmuletInfo();

	void Linking();

};