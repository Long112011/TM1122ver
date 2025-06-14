#pragma once
#include "./Interface/cDialog.h"
#define ENTRANCETIPINFO_LEN		37

struct ENTRANCETIP_TEXT
{
	ENTRANCETIP_TEXT() { strToolTip = NULL; pNext = NULL; }
	char*			strToolTip;
	ENTRANCETIP_TEXT*	pNext;
};
enum EntranceStatic
{
	Level=0,
	Money,
	Quest,
	Item,
	MaxStatic,
};
class cListDialog;
class CInsDGEntranceInfoDlg : private cDialog
{
	cDialog*		pDlg;

	cButton*		bButton[3];

	cListDialog*	bEntranceInfo;
	cStatic*		bStaticEntrance[MaxStatic];

	CYHHashTable<ENTRANCETIP_TEXT>		m_EntranceTipTextList;

	MAPTYPE		wMapChangeNum;
	MAPTYPE		wMoveMapNum;
public:

	CInsDGEntranceInfoDlg();

	virtual ~CInsDGEntranceInfoDlg();
    void	Linking();
	void    OnActionEvent(LONG lId, void * p, DWORD we);

	MAPTYPE GetMapChange(){return wMapChangeNum;}
	MAPTYPE GetMoveMapChange(){return wMoveMapNum;}
	void NpcClick(WORD UniqueIdx,LEVELTYPE MinLevel,LEVELTYPE MaxLevel,MONEYTYPE FubenFee,BYTE Key,BYTE StrKey);

	void SetEntranceMsg(int line);

	void LoadEntranceTipList();
	ENTRANCETIP_TEXT* GetEntranceTipInfo( WORD wIdx );
};
