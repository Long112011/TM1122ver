



// ItemSlot.h: interface for the CItemSlot class.

// created by taiyo
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ITEMSLOT_H__8056E436_4AA0_474D_AFFB_BA0ED84B6697__INCLUDED_)
#define AFX_ITEMSLOT_H__8056E436_4AA0_474D_AFFB_BA0ED84B6697__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


//enum UPDATE_BIT{ UB_DBIDX = 1, UB_ICONIDX=2, UB_ABSPOS=4, UB_QABSPOS=8, UB_DURA=16, UB_ALL=31, };

//SW050920 Rare

enum UPDATE_BIT {
	UB_DBIDX = 1,
	UB_ICONIDX = 2,
	UB_ABSPOS = 4,
	UB_QABSPOS = 8,
	UB_DURA = 16,
	UB_RARE = 32,
	UB_STONE = 64,
	UB_LOCK = 128,
	UB_UNLOCK = 256,
	UB_GROW = 512,
	UB_POWERUP = 1024,
	UB_GREEN = 2048,
	UB_GRADE30 = 4096,

	// ÐÂÔö£º
	UB_QUALITY = 8192,
	UB_ENTRY1 = 16384,
	UB_ENTRY2 = 32768,
	UB_ENTRY3 = 65536,

	UB_ALL = 65537  // ÔÊÐíÎ»ÑÚÂëÈ«¿ª
}; enum ERROR_ITEM { EI_TRUE = 0, EI_OUTOFPOS = 1, EI_NOTEQUALDATA, EI_EXISTED, EI_NOTEXIST, EI_LOCKED, EI_PASSWD, EI_NOTENOUGHMONEY, EI_NOSPACE, EI_MAXMONEY };
// SS_LOCKOMIT À» ¼öÇàÇÑ ÇÔ¼ö ÈÄ¿¡ ¶ôÀÌ Ç®¸°´Ù. 
// ÁÖÀÇ : Updateitemabsµµ ¶ôÀÌ Ç®¸°´Ù!
enum SLOT_STATE { SS_NONE = 0, SS_PREINSERT=1, SS_LOCKOMIT = 2, SS_CHKDBIDX = 4, };

class CPlayer;
class CPurse;

class CItemSlot  
{
	bool bIsMovePack;//kiv
public:
	CItemSlot();
	virtual ~CItemSlot();

	virtual void Init(POSTYPE startAbsPos, POSTYPE slotNum, ITEMBASE * pInfo, SLOTINFO * pSlotInfo);
	void Release();
	
	virtual ITEMBASE /*const*/ * GetItemInfoAbs(POSTYPE absPos);//kiv const
#ifdef  _MUTIPET_
	virtual ITEMBASE /*const*/* GetItemInfoDbid(DWORD DBidx);//µ¶¸ç 3pet
#endif //  _MUTIPET_
	virtual void SetItemInfoAbs(ITEMBASE* pItemBase, POSTYPE absPos);
	void GetItemInfoAll(ITEMBASE * pItemBaseAll);
	void SetItemInfoAll(ITEMBASE * pItemBaseAll);
	char * _ToStringSlot();


    // 2014-11-14 UpdateItemAbs  ³É³¤Êý¾Ý
	virtual ERROR_ITEM UpdateItemAbs(CPlayer * pPlayer, POSTYPE whatAbsPos, DWORD dwDBIdx, WORD wItemIdx, POSTYPE position, POSTYPE quickPosition, DURTYPE Dur, WORD itemstatic,  WORD ItemQuality, WORD ItemEntry1, WORD ItemEntry2, WORD ItemEntry3, WORD flag=UB_ALL, WORD state=SS_NONE, DWORD RareDBIdx = 0, DWORD StoneIdx=0,DWORD Grow=0,char * PowerUp="NULL",char * Green = "NULL", DWORD Grade30 = 0);
	virtual ERROR_ITEM InsertItemAbs(CPlayer * pPlayer, POSTYPE absPos, ITEMBASE * pItem, WORD state=SS_NONE);
	virtual ERROR_ITEM DeleteItemAbs(CPlayer * pPlayer, POSTYPE absPos, ITEMBASE * pItemOut, WORD state=SS_NONE);
		

	POSTYPE GetStartPos() { return m_StartAbsPos; }
	POSTYPE GetSlotNum() { return m_SlotNum; }

	inline virtual BOOL IsEmpty(POSTYPE absPos);

	CPurse*	GetPurse() { return m_pPurse; }
	MONEYTYPE GetMoney();

//	virtual MONEYTYPE Addition(MONEYTYPE money);
//	virtual MONEYTYPE Subtraction(MONEYTYPE money);


	/* purse func */
	BOOL CreatePurse( CPurse* pNewPurse, void* pOwner, MONEYTYPE money, MONEYTYPE max = MAXMONEY );

	virtual BOOL IsAddable(POSTYPE absPos, ITEMBASE * pItemBase){	return TRUE;	}
	virtual WORD GetEmptyCell(WORD * EmptyCellPos, WORD NeedCount = 1) { return 0; }
	virtual void SetLock(POSTYPE absPos, BOOL val);
	virtual BOOL IsLock(POSTYPE absPos);

	virtual WORD GetItemCount();

	// RaMa	
	virtual BOOL CheckQPosForItemIdx( WORD ItemIdx )		{	return TRUE;	}
	virtual BOOL CheckItemLockForItemIdx( WORD ItemIdx )	{	return TRUE;	}

	void MovePack(DWORD dwID,bool& bRet);//kiv
	
protected:
	virtual inline BOOL IsEmptyInner(POSTYPE absPos);
	virtual inline BOOL IsPosIn(POSTYPE absPos);
	virtual inline BOOL IsPassword(POSTYPE absPos);

	ITEMBASE *		m_ItemBaseArray;

	SLOTINFO *		m_SlotInfoArray;
	POSTYPE			m_StartAbsPos;
	POSTYPE			m_SlotNum;						// SlotÀÇ °¹¼ö


	/* purse value */
	CPurse*			m_pPurse;
};

#endif // !defined(AFX_ITEMSLOT_H__8056E436_4AA0_474D_AFFB_BA0ED84B6697__INCLUDED_)


