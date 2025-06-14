#pragma once

#include "./Interface/cTabDialog.h"
//#include "WindowIDEnum.h"
//#include ".\interface\cWindowHeader.h"

#include "WearedExDialog.h"
#include "ItemShopInven.h"
#include "cShopItemInventPlusDlg.h"
//#define CELLNUMPERPAGE				20

class CWearedExDialog;
class CItem;
class CMugongBase;
class CDealItem;
class cStatic;
class cMsgBox;
class cPushupButton;

class CMoneyPacketDialog;

enum INVACTIONTYPE { IsLeftClicking = 0, IsRightClicking = 1, };
enum { INV=0, WEAR=1, SHOPIVN=2,SHOPIVNPLUS=3, };
enum eINVENTORY_STATE { eITEMDEFAULT_STATE, eITEMUPGRADE_STATE, eITEMDEAL_STATE, };
enum { ITEMINV=0, MALLINV=1, /*SHOPIVN=2,*/ };
class CInventoryExDialog : public cTabDialog  
{
	// 부활주문서 관련
	CItem*		m_pSelectedShopItem;
	REVIVEDATA	m_ReviveData;

	//weiye 憐잿관륵땍珂포宮밑변鑒땍屢     2018-04-05
	//DWORD       m_LastTidyTime;

	CMoneyPacketDialog * pMoneyPacketDlg;

	

public:
	CInventoryExDialog();
	virtual ~CInventoryExDialog();
	virtual void Render();
	virtual void Add(cWindow * window);
	virtual DWORD ActionEvent(CMouse * mouseInfo);

	BOOL	AddItem(ITEMBASE * itemBase);
	BOOL	AddItem(CItem* pItem);
	BOOL	DeleteItem(POSTYPE Pos,CItem** ppItem);
	void	MoveItem(POSTYPE FromPos, POSTYPE ToPos);
	virtual BOOL FakeMoveIcon(LONG x, LONG y, cIcon * icon);

	BOOL	GetBlankPositionRestrictRef(WORD & absPos);
	BOOL	CheckItemLockForItemIdx(WORD wItemIdx);

	void	Linking();


	BOOL	GetPositionForXYRef(BYTE param, LONG x, LONG y, WORD& pos);
	
	void	SetMoney(DWORD Money);
	BOOL	IsExist(POSTYPE abs_pos);
	BOOL	CanBeMoved(CItem* pItem,POSTYPE pos);
	

	void	UnselectAllItem(CItem * pItem3 = NULL);
	void	OnActionEvnet(LONG lId, void * p, DWORD we);
	

	void	UseItem(CItem* pItem, BYTE ActionType = IsLeftClicking);
	// data ope
	CItem * GetItemForPos(POSTYPE absPos);
	CItem * GetCurSelectedItem(BYTE type);								// inv(0), wear(1), SHOPIVN(2)
	BOOL	UpdateItemDurabilityAdd(POSTYPE absPos, int dur);				// 영약 아이템 중복처리를 위해(-)가 올수 도 있음: 아이템 사용했을 때
	void	SetState(eINVENTORY_STATE state){ m_iInventoryState = state; }
	WORD	GetTabIndex(POSTYPE absPos);
	

//KES EXCHNAGE 030929
	int		GetBlankNum();
//
//KES 040316 맵전환 로딩관련 수정	
	void	ReleaseInventory();

	virtual void SetActive(BOOL val);


	DURTYPE	GetTotalItemDurability(WORD wItemIdx);	
	CItem * GetItemLike(WORD wItemIdx);


	// RaMa - 04.12.09
	void OnShopItemBtn();
	void OffShopItemBtn();

	void OnShopItemBtnEx();
	void OffShopItemBtnEx();


	void OpenMall(BYTE bPage);


	void ItemUnSealingSync();
	void ItemUnSealing(DWORD absPos);
	void SetReviveData( DWORD ID, DWORD ItemIdx, DWORD ItemPos )
	{	m_ReviveData.TargetID = ID;		m_ReviveData.ItemIdx = (WORD)ItemIdx;		m_ReviveData.ItemPos = (POSTYPE)ItemPos;		}
	REVIVEDATA GetReviveData()			{	return m_ReviveData;	}
	void SetShopItemInvenBtn( BOOL val );
	CItem*	GetSelectedShopItem()		{	return m_pSelectedShopItem;	}
	void ResetSelectedShopItem()		{	m_pSelectedShopItem = NULL;	}
	CItemShopInven* GetShopInven()		{	return m_pItemShopInven;	}

	cShopItemInventPlusDlg* GetShopInvenPlus(){return m_pItemShopInvenPlus;}
	void SetShopInvenPlus(cShopItemInventPlusDlg* pDlg){m_pItemShopInvenPlus = pDlg;}
	
	BOOL	FakeMoveItem(CItem * pFromItem,WORD ToPos);
	void GetItemFromPyoguk(CItem* pItem);
	void GetItemFromItemShop(CItem* pItem);

	void CheckItemForTidyBtn();

	void TestNewDialog(int i, CMoneyPacketDialog* pDlg);
private:

	void	FakeBuyItem(LONG mouseX, LONG mouseY, CDealItem * icon);
	BOOL	IsDivideFunctionKey();
	BOOL	IsSelectFunctionKey();
	BOOL	FakeMoveItem(LONG mouseX, LONG mouseY, CItem * icon);				// network syn msg send
	BOOL	FakeGeneralItemMove( POSTYPE ToPos, CItem * pFromItem, CItem * pToItem );
	BOOL	FakeItemCombine( POSTYPE ToPos, CItem * pFromItem, CItem * pToItem );
	BOOL	FakeItemDivide( POSTYPE ToPos, CItem * pFromItem, CItem * pToItem );
	static void OnFakeItemDivideOk( LONG iId, void* p, DWORD param1, void * vData1, void * vData2 );
	static void OnFakeItemDivideCancel( LONG iId, void* p, DWORD param1, void * vData1, void * vData2 );

	CWearedExDialog		* m_pWearedDlg;
	cStatic				* m_pMoneyEdit;
	// RaMa -04.12.09

	CItemShopInven*		m_pItemShopInven;
	cShopItemInventPlusDlg*		m_pItemShopInvenPlus;

	//cPushupButton*		m_pItemShopBtn;
	//cPushupButton*		m_pTestBtn;

	cPushupButton*		m_pMPageBtn[2];

	cPushupButton*		m_pChangeLookBtn;

	BOOL				m_bShopMInven[2];

	//BOOL				m_bShopMInven[0];
	//BOOL				m_bShopMInven[1];

	BOOL				m_fSpread;
	eINVENTORY_STATE	m_iInventoryState;


	MSG_ITEM_DIVIDE_SYN	m_divideMsg;



//버튼푸쉬 처리
	DWORD	m_BtnPushstartTime;	
	DWORD	m_BtnPushDelayTime;
	BOOL	m_bPushTabWithMouseOver;

	cButton	* m_pTidyMallBtn;

	

	cButton	* m_pClearInvBtn[2];

	DWORD m_dwItemTidyDelayTime;
	DWORD m_dwItemTidyShopDelayTime;

	DWORD	m_StoredLastGridPosition;

	DWORD	m_SelectedItemRbtn;

// quest
public:
	cMsgBox*			m_pQuestMsgBox;
	CItem*				m_pTempQuestItem;
	void	ItemUseForQuestStart();

	CItem*	GetPriorityItemForCristmasEvent();
	DWORD	GetPriorityItemIdx();

	void   DeleteAllItem();

	BOOL   CheckItemLock();

	BOOL   CheckItemQuick();

	DWORD	GetItemCount();
	DWORD	GetItemMallCount();

	void	DeleteAllItemShop();

	void    SetInventoryItemTidy();
	void    SetInventoryItemTidyShop();

	void	StartItemTidyDelayTime(DWORD wTime);
	void	StartItemTidyShopDelayTime(DWORD wTime);

	bool	IsMallPageOpen(){return m_bShopMInven[0];}
	bool	IsMallExPageOpen(){return m_bShopMInven[1];}

	DWORD	GetSeletedItemRbtn(){return m_SelectedItemRbtn;}

	void	SetSeletedItemRbtn(DWORD idx){m_SelectedItemRbtn=idx;}

	void SetUnlockAll();

	bool	IsChangeLookButtonPushed();
};