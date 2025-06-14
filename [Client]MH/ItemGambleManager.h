
#include "./interface/cDialog.h"
#include "./Interface/cIconDialog.h"
#include "./Interface/cWindowManager.h"
#include "./Interface/cStatic.h"
#include "./Interface/cIcon.h"

#define NUMBERLISTIMGMAX	10  // 用到的数字为10个

class cStatic;
class CItem;
class cIconDialog;
class cIconGridDialog;
class cIcon;

class CItemGambleDialog : public cDialog
{
	cDialog     *    pDlg;

	cIconDialog *	m_pIconDlg[6];

	cIcon       *   m_icon[6];

	cImage          m_NumberList[NUMBERLISTIMGMAX];

public :

	CItemGambleDialog();

	virtual ~CItemGambleDialog();

	void Linking();

	void LoadNumberListImage();

	void SetGamblePublic(int Number[3]);

	void SetGamblePrivate(int Number[3]);

	void SendGambleInfo();

	BOOL CheckGambleNumber(int Number);


};