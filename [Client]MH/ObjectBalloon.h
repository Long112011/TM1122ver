#pragma once
#include "INTERFACE\cWindow.h"
enum
{
	ObjectBalloon_Name = 1,
	ObjectBalloon_Chat = 2,
	ObjectBalloon_Title = 4,
	ObjectBalloon_MunpaMark = 8,
};
class COBalloonName;
class COBalloonChat;
class CStreetStallTitleTip;
class CGuildMark;
class CObjectBalloon : public cWindow
{
public:
	CObjectBalloon();
	virtual ~CObjectBalloon();
	void InitBalloon(CObject * pParent, LONG tall, BYTE flag);
	void Release();
	void Render();
	COBalloonName * GetOBalloonName() { return m_pName; }
	COBalloonChat * GetOBalloonChat() { return m_pChat; }
	CStreetStallTitleTip* GetSSTitleTip() { return m_pSSTitle; }
	CStreetStallTitleTip* GetSBSTitleTip() { return m_pSBSTitle; }
	CGuildMark* GetGuildMark() { return m_pGuildMark; }
	void SetNameColor(DWORD color) { m_fgColor = color; }
	BOOL SetGuildMark(DWORD GuildIdx);
	void SetNickName(char* NickName);
	void SetName(char* Name);
	void SetFortName(char* Name);
	void SetTall(LONG	Tall) { m_lTall = Tall; }
	void SetOverInfoOption(DWORD dwOption);
	void ShowObjectName(BOOL bShow, DWORD dwColor);
	void ShowChatBalloon(BOOL bShow, char* chatMsg, DWORD dwColor, DWORD dwAliveTime);
	void ShowStreeStallTitle(BOOL bShow, char* strTitle, DWORD dwColor);
	void ShowStreeBuyStallTitle(BOOL bShow, char* strTitle, DWORD dwColor);
	void SetSiegeName(DWORD NameType, DWORD dwLength);
	void SetMarryName(char* MarryName);
	void SetShiTuName(char* name);
	void SetMasterName(char* MasterName);
	void SetFlashNameFlag(WORD Flag);
	void SetImageName(WORD ImageNum);
	void SetFame(char* Fame);
	void SetStageLogo(char* Fame);
	void SetTopListFlg(WORD val);
	void InitTopListFlg();
	void SetFameRank(char* fame);
	void SetFlashName(char * FlashName);
	void SetKillCount(char * kill);
protected:
	CObject *				m_pObject;
	COBalloonName *			m_pName;
	COBalloonChat *			m_pChat;
	CStreetStallTitleTip *	m_pSSTitle;
	CStreetStallTitleTip *	m_pSBSTitle;
	CGuildMark*				m_pGuildMark;
	cImage*					m_pNpcMark;
	LONG					m_lTall;
	DWORD					m_fgColor;
	DWORD					m_glistenCount;
	VECTOR2					m_OldAbsPos;
};