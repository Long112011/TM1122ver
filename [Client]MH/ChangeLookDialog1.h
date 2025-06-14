#ifndef CHANGEFILE_H
#define CHANGEFILE_H
#pragma once
#include "./interface/cDialog.h"
#include "./interface/cCheckBox.h"
#include "WearedExDialog.h"
#include "./Interface/cIconGridDialog.h"
class CWearedExDialog;
class cCheckBox;
class CItem;
class cChangeLookDialog : public cIconGridDialog//cDialog
{
	cDialog * p_Dlg;

	cCheckBox*	m_CheckIcon[7];

	CWearedExDialog * m_pWearedDlg;

	cIconGridDialog * p_Grid;

	float XX;
	float YY;

	BOOL m_IsAutoOpenBox;
public:

	cChangeLookDialog();
	virtual ~cChangeLookDialog();

	void Linking();

	virtual void Render();

	void OnActionEvent(LONG lId, void * p, DWORD we);

	void ClearItemInfo();

	BOOL AddItem(CItem* pItem);
	virtual BOOL FakeMoveIcon(LONG x, LONG y, cIcon * icon);
	POSTYPE GetRelativePosition(POSTYPE absPos);
	BOOL InPt(POSTYPE absPos);
	BOOL DeleteItem(POSTYPE Pos, CItem** ppItem);
	BOOL FakeMoveItem(LONG MouseX, LONG MouseY, CItem* pSrcItem);
	CItem* GetItemForPos(POSTYPE absPos);
	BOOL FakeGeneralItemMove(POSTYPE ToPos, CItem * pFromItem, CItem * pToItem);
	BOOL CanBeMoved(CItem* pItem, POSTYPE pos);
	BOOL GetBlankPositionRestrictRef(WORD& absPos);
	BOOL GetBlankNum();

	void DisableIcon();

	//void Init();

	void WriteConfig();
	void ReadConfig();

	BOOL GetTest(){ return m_IsAutoOpenBox; }
};
#endif