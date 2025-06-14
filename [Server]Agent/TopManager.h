#pragma once
#define TOPMGR	USINGTON(TopManager)
class TopManager
{
	SEND_TOPLIST m_HeroTopList;
	SEND_TOPLIST m_MunpTopList;
	SEND_TOPLIST m_DungeonTopList;
	DWORD  m_LastTime;
	DWORD  m_FlushTime;
	DWORD  m_FlushTime2;
	DWORD  m_ChatMsg1;
	DWORD  m_ChatMsg2;
	WORD wTodayDate;
public:
	TopManager();
	virtual ~TopManager();
	void InitTopList();
	void SetHeroTopList(SEND_TOPLIST * m_HeroList);
	void SetMunpTopList(SEND_TOPLIST * m_HeroList);
	void SetDungeonTopList(SEND_TOPLIST * m_HeroList);
	void Process();
	void SendAllHeroTopListInfo();
	void SendAllMunpTopListInfo();
	void SendAllDungeonTopListInfo();
	void SendHeroTopListInfo(DWORD dwConnectionIdx);
	void SendMunpTopListInfo(DWORD dwConnectionIdx);
	void SendDungeonTopListInfo(DWORD dwConnectionIdx);
	void LoadFlushTimeConfig();
	WORD TodayDate();
	SYSTEMTIME TodayFullDate();
};
EXTERNGLOBALTON(TopManager)