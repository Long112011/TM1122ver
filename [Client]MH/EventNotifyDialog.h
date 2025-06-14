#pragma once
#include "./Interface/cDialog.h"
class cStatic;
class cTextArea;
class CEventNotifyDialog : public cDialog
{
protected:
	cStatic*	m_pStcTitle;
	cTextArea*	m_pTAContext;
	BOOL			m_bFlicker;
	BOOL			m_bFlActive;
	DWORD			m_dwFlickerSwapTime;

	BOOL			m_bFlicker2;
	BOOL			m_bFlActive2;
	DWORD			m_dwFlickerSwapTime2;

	DWORD           m_glistenCount2;

	float absX;
	float absY;
public:
	CEventNotifyDialog();
	virtual ~CEventNotifyDialog();
	virtual void SetActive(BOOL val);
	virtual DWORD ActionEvent(CMouse* mouseInfo);
	virtual void Render();
	void Linking();
	void SetTitle(char* pStrTitle);
	void SetContext(char* pStrContext);
	void SetEventCount(BOOL bAdd);
	void Open();
	void Close();
	void SetFlicker(BOOL bFlicker);
	void FlickerMussangGuage();

	void SetFlicker2(BOOL bFlicker);
	void FlickerMussangGuage2();
};