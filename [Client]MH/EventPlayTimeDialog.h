#pragma once
#include "./Interface/cDialog.h"
#include "./ObjectGuagen.h"
//#include "./Interface/cTabDialog.h"
//#include "INTERFACE\cIcon.h"
//#include "DealItem.h"
class cStatic;
//class cButton;
class CObjectGuagen;
//class CItem;
//class cIconDialog;
//class cIconGridDialog;
//class CDealItem;
class CEventPlayTimeDlg : public cDialog
{
	//cIconGridDialog *	m_pLuckerGridDlg;

	//CDealItem*			pDitem;

	
private:
	//char	timeBuffer[128];
	char	ItemName[MAX_ITEMNAME_LENGTH+1];
	//char	IconItemName[MAX_ITEMNAME_LENGTH+1];
	//char	IconBuff[256];
protected:

	cDialog*		pDlg;
	CObjectGuagen*	m_pEngraveGuagen;
	cStatic*		m_pRemaintimeStatic;
	cStatic*		m_pPlayTimeStatic;
	cStatic*		TempImage;
	cStatic*		QuantityStatic;
	DWORD			m_dwProcessTime;
	//GUAGEVAL		m_fBasicTime;
	


	DWORD			m_dwItemIdx;
	DWORD			m_dwNFlag;

	BYTE			m_bCheckSum;

	DWORD			m_dwItemKey;

	int				m_dwItemQty;



	int				nLimitTime;
	int				m_StoredTime;

	//int				iTemp;


	double			value;

	int				m_dwTotalTime;

	int dwTiming;

	int dwTempTime;
	

public:
	CEventPlayTimeDlg();
	virtual ~CEventPlayTimeDlg();


	void Linking();
	//DWORD ActionEvent( CMouse * mouseInfo );

	void SetEventTime(DWORD m_time,BYTE CheckSum,DWORD ItemKey,char*Text,DWORD dwQty,DWORD dwTotalTime);


	//void SetActive();

	void Close();
	//void SetTimer2(char * pTimer);

	void ExRender();
};



