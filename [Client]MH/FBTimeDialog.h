#pragma once
#include "./Interface/cDialog.h"
enum FBStaticEnum
{
	FbTime=0,
	FbTargetInfo,
	FbStaticMax
};
class CFBTimeDialog : public cDialog
{
    cDialog * pDlg;

   //cStatic * pTime;

	cStatic * pFBStatic[FbStaticMax];

	cButton*	pFbMissionBtn;

	char      timeBuffer[128];
	char      infoBuffer[128];

	int	mCount;
	int mCountMsg;

	DWORD	m_dwStoredTime;
	DWORD	m_dwHitTime;

	BOOL CanStartTimer;
	BOOL bFlag;

public:


	CFBTimeDialog();

	virtual ~CFBTimeDialog();

	void Linking();

	void SetFBTime(DWORD m_time);

	void CreateFBTime();

    void SetActive();

	void OnActionEvent(LONG lId, void * p, DWORD we);

	void SetMonsterCount(int Count,BYTE bFlag);
	void AddMonsterCount(int Count);

	void AddNpcPortal( float x, float z,DWORD MapKind);
	void RemoveNpcPortal();

	void Process();

	void SendOut();
};