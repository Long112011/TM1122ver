
#if !defined(AFX_SHOPBUYDLG_H__3565EDD2_97D4_4E4F_8535_23A911D95094__INCLUDED_)
#define AFX_SHOPBUYDLG_H__3565EDD2_97D4_4E4F_8535_23A911D95094__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "INTERFACE\cDialog.h"
#include ".\interface\cWindowHeader.h"
#include "GameResourceStruct.h"

class CGoldShopBuyDialog : public cDialog    //CoNaN 2017-08-25  结婚系统添加对象窗口类 
{
protected:
	cDialog * pDlg;
	cStatic * m_pItemName;
	cStatic * m_pItemName2;
	cStatic * m_pItemPrice;
	cStatic * m_pItemPriceOffer;
	cStatic * m_BaseImage;
	cStatic * m_GoldImage;
	cStatic * m_MallImage;
	DWORD     m_ItemIdx;
	DWORD     m_Key;
	DWORD     m_Count;
	cButton	* m_pButton[2];
public:
	CGoldShopBuyDialog();
	virtual ~CGoldShopBuyDialog();
	virtual void SetActive(BOOL val);
	virtual DWORD ActionKeyboardEvent( CKeyboard * keyInfo );
	void Init(ITEM_GOLD_SHOP * pShopItem);
	void Linking();
	
	void OnActionEvent(LONG lId, void * p, DWORD we);
	void SetCount(DWORD Val){m_Count=Val;}
	DWORD GetCount(){return m_Count;}

	void BuyThis(BOOL bFlag=true);
};
#endif