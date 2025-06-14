#ifndef _ItemDropInfoDialog_
#define _ItemDropInfoDialog_
#include "./interface/cDialog.h"
#include "./interface/cIconGridDialog.h"
class cStatic;
class CItem;
class cIconDialog;
class cIconGridDialog;
#define ITEMDROPINFODLG GAMEIN->GetItemDropInfoDialog()//USINGTON(CItemDropInfoDialog)
class CItemDropInfoDialog : public cIconGridDialog
{
	cDialog *			pDlg;
	cIconGridDialog *	m_pItemGridDlgEasy;
	cIconGridDialog *	m_pItemGridDlgMedium;
	cIconGridDialog *	m_pItemGridDlgHard;

	cStatic *			m_pTitle;
	cStatic *			m_pStatic[3];

	char	m_pMonsterName[17];
public:
	CItemDropInfoDialog();
	virtual ~CItemDropInfoDialog();

	virtual void SetActive(BOOL val);
	void Linking();
	void AddItemToGridEasy();
	
	void OnActionEvent(LONG lId, void * p, DWORD we);

	void ClearIconGrid();

	void SetMonsterName(char * name){ strcpy(m_pMonsterName, name); }

};
EXTERNGLOBALTON(CItemDropInfoDialog)
#endif //