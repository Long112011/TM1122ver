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
	// ��Ȱ�ֹ��� ����
	CItem*		m_pSelectedShopItem;
	REVIVEDATA	m_ReviveData;

	//weiye ���������ʱ����غ�������     2018-04-05
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
	BOOL	UpdateItemDurabilityAdd(POSTYPE absPos, int dur);				// ���� ������ �ߺ�ó���� ����(-)�� �ü� �� ����: ������ ������� ��
	void	SetState(eINVENTORY_STATE state){ m_iInventoryState = state; }
	WORD	GetTabIndex(POSTYPE absPos);
	

//KES EXCHNAGE 030929
	int		GetBlankNum();
//
//KES 040316 ����ȯ �ε����� ����	
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

	void SetBackMoney(long Val);	//���ؽ�Ǯ

	void SetBackGoldMoney(long Val);	//����Ԫ��

	void SetBackMallMoney(long Val);	//�����ݵ�
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



//��ưǪ�� ó��
	DWORD	m_BtnPushstartTime;	
	DWORD	m_BtnPushDelayTime;
	BOOL	m_bPushTabWithMouseOver;

	cButton	* m_pTidyMallBtn;

	

	cButton	* m_pClearInvBtn[2];

	DWORD m_dwItemTidyDelayTime;
	DWORD m_dwItemTidyShopDelayTime;

	DWORD	m_StoredLastGridPosition;

	DWORD	m_SelectedItemRbtn;

	cStatic* p_Money;	//���ؽ�Ǯ��
	cStatic* p_PaoDian;	//�����ݵ㵽Ǯ��
	cStatic* p_Gold;	//����Ԫ����Ǯ��
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

	void SetInventoryExDialogWH(BOOL IsShop);	//�ٱ�������ȵ���
};