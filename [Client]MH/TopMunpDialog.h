#include "./Interface/cDialog.h"
class cLCColumn;
class cListCtrl;
class cPushupButton;
class CTopMunpDialog : public cDialog
{
  
	cDialog  * pDlg;
	cListCtrl * pMunpList;
	cListCtrl   * pMunpPos;
	cStatic   * pUpdateTime;
	cStatic	*	pColumnStatic[8];	
	cPushupButton * pPageList[5];
	SEND_TOPLIST  pMunpData;
	int       m_StartPos;
	int       m_EndPos;

	int         m_TableIdx;

public :

	CTopMunpDialog();
	virtual ~CTopMunpDialog();
	virtual DWORD ActionEvent(CMouse* mouseInfo);
	void OnActionEvent(LONG lId, void * p, DWORD we);
	void Linking();
	void SetMunpTopListInfo(SEND_TOPLIST* pListInfo);
	void GetMunpTopListInfo();
	void ShowMunpList();
	void InitMunpList();
	void SetMunpPage(int PageIndex);
	void SetPage(int PageIndex);

	void LoadList();
	void LoadHeroList();
	void OpenLastTab();

	bool checkName(char * c);
};