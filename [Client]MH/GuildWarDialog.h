
#if !defined(AFX_GUILDWARDLG_H__7D808836_FB10_4B68_8E74_0F797F496450__INCLUDED_)
#define AFX_GUILDWARDLG_H__7D808836_FB10_4B68_8E74_0F797F496450__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include "./Interface/cDialog.h"

#include "MinimapDlg.h"

class cStatic;
class cImage;
class cTextArea;



class CGuildWarDialog : public cDialog
{
	cStatic* pGuildListSta[3];
	cStatic* pCharacterKillSta;
	cStatic* pCharacterDieSta;

	DWORD    pLostTimeTick;
	cStatic* pLostTimeSta;

	BOOL	pIsClose;

	DWORD pPushTime;
	DWORD pLostTime;
	CYHHashTable<GUILD_WAR_GUILD_VALUE_CLIENT> pGuildInfoTable;

	char pTempJack[MAX_PATH];
public:
	CGuildWarDialog();

	virtual ~CGuildWarDialog();

	void Linking();
	void CGuildWarDialog::InitHeroInfo(DWORD pKillCount,DWORD pDieCount,WORD pState);
	void CGuildWarDialog::Process();
	void CGuildWarDialog::InitGuildInfo(MSG_GUILD_WAR_GUILD_INFO* pInfo);
	void IsClose(){pIsClose = TRUE;}
};

#endif // !defined(AFX_GUILDWARDLG_H__7D808836_FB10_4B68_8E74_0F797F496450__INCLUDED_)


