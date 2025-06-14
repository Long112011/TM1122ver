

#if !defined(AFX_CSHOPITEM_H__AAF8CAFA_E112_45C0_96E6_DD5FDBA39E1C__INCLUDED_)
#define AFX_CSHOPITEM_H__AAF8CAFA_E112_45C0_96E6_DD5FDBA39E1C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "INTERFACE\cIcon.h"
#include "cImage.h"

struct sCASHSHOPICONINIT
{
	VECTOR3	 vIconPos[3];
};


void BtnFunc(LONG lId, void * p, DWORD we);

class CShopItem : public cIcon
{
	sCASHSHOPICONINIT sCashShopIcon;
private:

	WORD m_wItemIdx;

    DWORD m_GoldPrice;

    DWORD m_MallPrice;

	DWORD m_BuyPrice;

	DWORD m_OfferPrice;

    WORD  m_wItemType;  //元宝还是泡点物品

	char  m_ItemName[MAX_NAME_LENGTH+5];

	char  m_ItemName2[MAX_NAME_LENGTH+5];

	cImage   m_GoldOrMallImg[3];

	cButton *  m_Button;

	char  m_ItemVipOnly[MAX_NAME_LENGTH + 5];

public:

	CShopItem();   

	virtual ~CShopItem();

	void Render();

	void SetItemIdx(WORD itemIdx)
	{
		m_wItemIdx = itemIdx;
	}
	WORD GetItemIdx()
	{
		return m_wItemIdx;
	}
	void SetGoldPrice(DWORD Val){m_GoldPrice=Val;}
	
	DWORD  GetGoldPrice(){return m_GoldPrice;}

	void SetMallPrice(DWORD Val){m_MallPrice=Val;}
	
	DWORD  GetMallPrice(){return m_MallPrice;}

	void SetBuyPrice(DWORD Val){m_BuyPrice=Val;}
	
	DWORD  GetBuyPrice(){return m_BuyPrice;}

	void SetOfferPrice(DWORD Val){m_OfferPrice=Val;}
	
	DWORD  GetOfferPrice(){return m_OfferPrice;}

	char * GetItemName(){return m_ItemName;}

	char * GetItemName2(){return m_ItemName2;}

	void  SetItemName(char * pName){SafeStrCpy(m_ItemName,pName,MAX_NAME_LENGTH+5);}

	void  SetItemName2(char * pName){SafeStrCpy(m_ItemName2,pName,MAX_NAME_LENGTH+5);}

	char * GetItemVipInfo() { return m_ItemVipOnly; }
	void  SetItemVipInfo(char * pName,...) { 
		if (pName == NULL) return;
		char msg[256];
		va_list argList;
		va_start(argList, pName);
		vsprintf(msg, pName, argList);
		va_end(argList); 
		SafeStrCpy(m_ItemVipOnly, msg, MAX_NAME_LENGTH + 5); }

	WORD GetItemType(){return m_wItemType;}

	void SetItemType(WORD Val){m_wItemType=Val;}

	//void  OnActionEvnet(LONG lId, void * p, DWORD we);

	void  InitBtn(LONG x,LONG y, WORD w,WORD h,cImage * BaseImg,cImage * OverImg,cImage * PressImg,cbFUNC Func,LONG lId,char * BtnText);

	void LoadCashShopIcon();

	//void ClearFaceImg(char * StaticText,int StrLen);
};

#endif // !defined(AFX_CSHOPITEM_H__AAF8CAFA_E112_45C0_96E6_DD5FDBA39E1C__INCLUDED_)
