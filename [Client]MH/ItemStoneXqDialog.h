#include "Interface\cDialog.h"
#include "itemManager.h"
#include "VirtualItem.h"
#include "Item.h"

// 2014-11-18 系统开发

class cIconDialog;
class CItem;
class cStatic;
class cIcon;

class CItemStoneXqDialog : public cDialog
{


	cDialog * pDlg; 

	cStatic * pRate;

	CItem   * pXqItem;

	cIconDialog*	m_pIconDlgEquip;

	cIconDialog*	m_pIconDlgStone0;
	cIconDialog*	m_pIconDlgStone1;
	cIconDialog*	m_pIconDlgStone2;
	cIconDialog*	m_pIconDlgStone3;
	cIconDialog*	m_pIconDlgStone4;

	cIcon  *        m_IconStone0;
	cIcon  *        m_IconStone1;
	cIcon  *        m_IconStone2;
	cIcon  *        m_IconStone3;
	cIcon  *        m_IconStone4;

	CVirtualItem	m_VirtualItemEquip;
	CVirtualItem    m_VirtualItemStone0;
	CVirtualItem    m_VirtualItemStone1;
	CVirtualItem    m_VirtualItemStone2;
	CVirtualItem    m_VirtualItemStone3;
	CVirtualItem    m_VirtualItemStone4;

public:

       CItemStoneXqDialog();

	   virtual ~CItemStoneXqDialog();

	   void Linking();

	   void SetXqItem(CItem * pItem);

	   CItem * GetXqItem();

	   void  DeleteXqStoneAll();

	   virtual BOOL FakeMoveIcon(LONG x, LONG y, cIcon * icon);

	   void AddVirtualItem( CItem* pItem );

	   void AddVirtualIcon( CItem* pItem );

	   int  GetStoneXqCount(ITEM_STONE_OPTION_INFO * pStoneInfo);

	   int  GetStoneVirtualCount();

	   void SendItemStoneXqMsg();

	   void ResetItemInfo();

	   void Release();

};


