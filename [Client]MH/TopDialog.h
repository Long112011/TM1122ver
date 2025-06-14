#include "./Interface/cDialog.h"
class cLCColumn;
class cListCtrl;
class cPushupButton;
class CTopDialog : public cDialog
{  
	cDialog			* pDlg;
	cListCtrl		* pHeroList;
	cListCtrl		* pHeroPos;
	cStatic			* pUpdateTime;
	cStatic			* pColumnStatic[7];	
	cPushupButton	* pPageList[5];
	SEND_TOPLIST	  pHeroData;
	int				  m_StartPos;
	int				  m_EndPos;
	int				  m_TableIdx;
public :
	CTopDialog();
	virtual ~CTopDialog();
	virtual DWORD ActionEvent(CMouse* mouseInfo);
	void OnActionEvent(LONG lId, void * p, DWORD we);
	void Linking();
	void SetHeroTopListInfo(SEND_TOPLIST* pListInfo);
	void GetHeroTopListInfo();
	void ShowAllList();
	void InitHeroList();
	void SetHeroPage(int PageIndex);
	void SetPage(int PageIndex);
	void LoadList();
	void LoadHeroList();
	void OpenLastTab();
	bool checkName(char * c);
};