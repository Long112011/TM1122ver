////////////////////////////////////////////////////////////////////
//                          _ooOoo_                               //
//                         o8888888o                              //
//                         88" . "88                              //
//                         (| ^_^ |)                              //
//                         O\  =  /O                              //
//                      ____/`---'\____                           //
//                    .'  \\|     |//  `.                         //
//                   /  \\|||  :  |||//  \                        //
//                  /  _||||| -:- |||||-  \                       //
//                  |   | \\\  -  /// |   |                       //
//                  | \_|  ''\---/''  |   |                       //
//                  \  .-\__  `-`  ___/-. /                       //
//                ___`. .'  /--.--\  `. . ___                     //
//             ."" '<  `.___\_<|>_/___.'  >'"".                   //
//            | | :  `- \`.;`\ _ /`;.`/ - ` : | |                 //
//            \  \ `-.   \_ __\ /__ _/   .-` /  /                 //
//     ========`-.____`-.___\_____/___.-`____.-'========          //
//                          `=---='                               //
//     ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^         //
//         ���汣��       ����BUG     �����޸�				      //
////////////////////////////////////////////////////////////////////
#pragma once
class CPlayer;
class BillBoardManager
{
	/*TOP_LIST_INFO m_TopListInfo;
	WORD m_wTopListCount;
	WORD m_wTopListGuildCount;
	WORD m_wTopListKillerCount;
	SYSTEMTIME	UpdataTime;*/
	SEND_TOPLIST  pHeroData;
public:
	/*
	WORD GetTopListCount(){ return m_wTopListCount; }
	WORD GetTopListGuildCount(){ return m_wTopListGuildCount; }
	WORD GetTopListKillerCount(){ return m_wTopListKillerCount; }

	void SetTopList(const MSG_TOP_LIST_HERO* msg);

	void ResetTopList();

	TOPLISTINFOHERO * GetHeroList(){ return m_TopListInfo.HeroList; }
	TOPLISTINFOGUILD* GetGuildList(){ return m_TopListInfo.GuildList; }
	KILLPLAYERTOPLIST* GetKillList(){ return m_TopListInfo.KillerList; }
	TOPLISTINFOHERO*  GetHeroTop(){ return &m_TopListInfo.HeroList[0]; }
	TOPLISTINFOGUILD*  GetGuildTop(){ return &m_TopListInfo.GuildList[0]; }
	KILLPLAYERTOPLIST* GetKillTop(){ return &m_TopListInfo.KillerList[0]; }
	SYSTEMTIME GetUpdateTime(){ return UpdataTime; }
	//[���б�־����][By:ʮ���½�����][QQ:112582793][2018/2/25]
	
	bool IsCanUpdate();
	*/
	
	GETINSTANCE(BillBoardManager);
	BillBoardManager(void);
	~BillBoardManager(void);
	void SetHeroTopListFlg(CPlayer* pPlayer);
	void SetTopListInfo(SEND_TOPLIST* pListInfo);

	//void SetHeroTopListGameIn(DWORD Id);
};
#define sBOARDMGR BillBoardManager::GetInstance()