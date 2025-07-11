



// DealItem.h: interface for the CDealItem class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DEALITEM_H__AAF8CAFA_E112_45C0_96E6_DD5FDBA39E1C__INCLUDED_)


#define AFX_DEALITEM_H__AAF8CAFA_E112_45C0_96E6_DD5FDBA39E1C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


//#include "./Interface/cIcon.h"
#include "./BaseItem.h"


class CDealItem : public CBaseItem  
{
public:
	WORD m_wIdx;
	CDealItem();

	virtual ~CDealItem();

	void SetItemIdx(WORD itemIdx)
	{
		m_wItemIdx = itemIdx;

	}
	WORD GetItemIdx()

	{
		return m_wItemIdx;
	}
	DWORD GetSellPrice();
	DWORD GetBuyPrice();

	char* GetItemName();


	void SetCount(int count)
	{
		m_nItemCount = count;
	}
	int GetCount()	{	return m_nItemCount;	}
	void Render();
	virtual inline void SetGrade30(DWORD Val) {}//ԭ��û�е�	
	virtual inline DWORD GetGrade30() { return 0; }//ԭ��û�е�
private:
	WORD m_wItemIdx;

	//SW070626 ���λ�NPC
	int m_nItemCount;

	cImage m_ScrollImg[22];
	int m_CurScrollImage;
};

#endif // !defined(AFX_DEALITEM_H__AAF8CAFA_E112_45C0_96E6_DD5FDBA39E1C__INCLUDED_)



