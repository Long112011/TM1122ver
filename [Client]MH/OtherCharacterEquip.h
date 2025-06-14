
#pragma once
#include "CharacterDialog.h"
#include "WearedExDialog.h"
#include "Item.h"
class CPlayer;
class CQuipInfoDlg: public cIconDialog
{
public:
	CQuipInfoDlg(void);
	virtual ~CQuipInfoDlg(void);
	virtual void Init(LONG x, LONG y, WORD wid, WORD hei, cImage * basicImage, LONG ID=0);
	virtual void SetActive( BOOL val );
	void Linking();
	BOOL AddItem(CItem* pItem);
	BOOL DeleteItem(CItem** ppItem);
	void SetChr(char * CharName);
private:
	CWearedExDialog* m_pHeroWearDlg;
};