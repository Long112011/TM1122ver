#include "Interface\cDialog.h"
#include "itemManager.h"
#include "VirtualItem.h"
#include "Item.h"

class cIconDialog;
class CItem;
class cStatic;

// 2014-11-13 系统开发，!

class CItemStoneDkDialog : public cDialog
{

	cDialog * pDlg; 

	cStatic * pRate;

	DWORD    pRateValue;

	cIconDialog*	m_pIconDlg;

	CVirtualItem	m_VirtualItem;

	CItem *         m_StoneItem;


public:

    CItemStoneDkDialog();

	virtual ~CItemStoneDkDialog();

	void Linking();

	void SetItemStoneRate(DWORD Val);

	void SetItemStone(CItem * pItem);

	CItem * GetItemStone();

	virtual BOOL FakeMoveIcon(LONG x, LONG y, cIcon * icon);

	void AddVirtualItem( CItem* pItem );

	int  GetStoneDkCount(ITEM_STONE_OPTION_INFO * pStoneInfo);

	void SendItemStoneDkMsg();

	void Release();
 
};