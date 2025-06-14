#pragma once
#include "./Interface/cDialog.h"
class cStatic;
class cTextArea;
class cVSyncDialog : public cDialog
{
protected:
	cStatic*	m_pStcTitle;
	cStatic*	m_pTAContext;
	cDialog*	m_pDialog;
	BOOL			m_bFlicker;
	BOOL			m_bFlActive;
	DWORD			m_dwFlickerSwapTime;

	BOOL			m_bFlicker2;
	BOOL			m_bFlActive2;
	DWORD			m_dwFlickerSwapTime2;

	DWORD           m_glistenCount2;

	float absX;
	float absY;

	DWORD		m_CloseCountdown;
	DWORD		m_CloseCountdownTick;
	BOOL		m_CloseCountdownStart;
public:
	cVSyncDialog();
	virtual ~cVSyncDialog();
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
	void AddMsg(char* title, char* context);
	void CloseCountdown(int count/*BOOL start*/);
	void CloseCountdownProcess();
};