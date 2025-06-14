
#if !defined(AFX_GUILDWARMGR_H__4F0BBCDB_B978_4804_A05F_F72B21DF285F__INCLUDED_)

#define AFX_GUILDWARMGR_H__4F0BBCDB_B978_4804_A05F_F72B21DF285F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // 小山竹 盟战系统
enum{
	eGuildState_Wait,
	eGuildState_WarIng,
	eGuildState_End,
};
#define GUILDWARMGR	USINGTON(CGuildWarManager)
class CGuildWarManager  
{
	DWORD pLoginHeroID;
	BOOL  pIsWarMap;
	DWORD pDieTime;
	WORD pWarMapNum;
	WORD pChannelNum;
public:
	CGuildWarManager();
	virtual ~CGuildWarManager();

	void CGuildWarManager::NetworkMsgParse(WORD Protocol,void* pMsg);

	WORD GetWarMapNum(){return pWarMapNum;}
	WORD GetWarChannelNum(){return pChannelNum;}
	void SetLoginHeroID(DWORD pID){pLoginHeroID = pID;}
	void SetWarMap(BOOL BL){pIsWarMap = BL;}
	BOOL IsWarMap(){return pIsWarMap;}
	void SetDieTime(DWORD pTime){pDieTime = pTime;}
	DWORD GetDieTime(){return pDieTime;}
};
EXTERNGLOBALTON(CGuildWarManager)

#endif // !defined(AFX_GUILDWARMGR_H__4F0BBCDB_B978_4804_A05F_F72B21DF285F__INCLUDED_)


