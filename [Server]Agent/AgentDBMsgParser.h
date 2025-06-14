




#ifndef __AGENTDBMSGPARSER_H__
#define __AGENTDBMSGPARSER_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include "db.h"
#include "CommonDBMsgParser.h"


//////////////////////////////////////////////////////////////////
// DB驴隆录颅 戮虏脌脤麓脗 录枚脛隆脛隆脠炉.
#ifdef _JAPAN_LOCAL_
#define dMAXCHARACTER 5
#elif defined _HK_LOCAL_
#define dMAXCHARACTER 5
#elif defined _TL_LOCAL_
#define dMAXCHARACTER 5
#else
#define dMAXCHARACTER 4
#endif

//extern MSG_CHAT g_WisperTemp;
//extern MSG_CHAT g_MunpaTemp;
//extern MSG_CHAT g_PartyTemp;

enum BasicCharacterInfo
{
	eCL_ObjectID = 0, eCL_StandIndex, eCL_ObjectName, eCL_BodyType, eCL_HeadType, 
	eCL_Hat, eCL_Dress, eCL_shoes, eCL_Weapon, 
	eCL_Grade, eCL_Map, eCL_Gender, eCL_Height, eCL_Width, eCL_Stage, eCL_AuthKey,
	eCL_LastMap,
};

enum MakeCharacterInfo
{
	eMC_UserID = 0, eMC_Gengoal, eMC_DEX, eMC_STA, eMC_Simmak, eMC_Name, eMC_Job, eMC_BodyType, eMC_HeadType
};

enum Friendinfo
{
	eFr_TargetID=0, eFr_FromID, eFr_ToPlayerName, eFr_FromPlayerName, eFr_Err, 
};
enum Friendisvalidtarget
{
	eFr_vtFromid , eFr_vtTargetid , eFr_vtFromname , eFr_vtToname, eFr_vtvailderr, 
};
enum Frienddel
{
	eFr_targetname, eFr_IsSuccess, 
};
enum Friendadd 
{
	eFr_addToErr, eFr_addToIDX, eFr_addToName, eFr_addFromErr, eFr_addFromIDX, eFr_addFromName, 
};
enum Friendlist
{
	eFr_FriendName, eFr_IsLoggin,
};
enum Friendlogin
{

	eFr_LTargetID, eFr_LLoggedname,
};
enum Friendloginedfriend
{
	eFr_LFFriendID, eFr_LFFriendName,
};
enum 
{
	eFr_FLFriendid, eFr_FLFriendname, eFr_FLIsLogin, eFr_FLTotalPage,
};
enum Friendsendnote
{
	eFr_NErr, eFr_NToName, eFr_NToId,
};
enum FriendNotelist
{
	eFr_NNoteID, eFr_NSender, eFr_NSentDate, eFr_NSentTime, eFr_NTotalpage, eFr_NbIsRead,
};
enum FriendReadnote
{
	eFr_NRNNoteID, eFr_NRNSender, eFr_NRNNote, eFr_NRNItemIdx,
};
enum FriendDeletenote
{
	eFr_NdNoteID, eFr_NdbLast,
};

//weiye 角色排行数据库字段标志定�?2018-01-16
/*
enum TopListInfo
{
     eTopListObjName1,eTopListObjName2,eTopListStage, eTopListValue1,eTopListValue2,
};

//weiye 角色排行数据库字段标志定�?2018-01-16

enum TopMunpListInfo
{
     eTopMunpListObjName1,eTopMunpListObjName2, eTopMunpListValue1,eTopMunpListValue2,
};
*/
enum TopListInfo
{
     eTopListObjectIDX,eTopListObjectIndex,/*eTopListIDX,*/eTopListChange,eTopListStage,eTopListObjName1,eTopListValue1,eTopListValue2,eTopListObjName2,
};

//miningmso ½ǉ«ŅАʽ¾ݿ◖¶αꖾ¶¨ҥ Զ¼ӥTopMunpMarkIDX °ﻡ±ꖾ±�?018-12-31

enum TopMunpListInfo
{
     eTopMunpMarkIDX,eTopMunpListObjIndex,/*eTopMunpListIDX,*/eTopMunpListObjChange,eTopMunpListObjName1, eTopMunpListValue1,eTopMunpListValue2,eTopMunpListObjName2,
};
enum TopDungeonListInfo
{
	eTopDungeonListObjectIDX, eTopDungeonListObjectIndex,/*eTopDungeonListIDX,*/eTopDungeonListChange, eTopDungeonListStage, eTopDungeonListObjName1, eTopDungeonListValue1, eTopDungeonListValue2, eTopDungeonListObjName2,
};
enum TopFameListInfo
{
     eTopFameListObjectIDX,eTopFameListObjectIndex,/*eTopFameListIDX,*/eTopFameListChange,eTopFameListStage,eTopFameListObjName1,eTopFameListValue1,eTopFameListValue2,eTopFameListObjName2,
};
// 2015-01-26 赌博系统数据库字段枚�?
enum eGambleInfo
{
    eGamble_Year,eGamble_Month,eGamble_Day,eGamble_Hour,eGamble_Min,eGamble_Num1,eGamble_Num2,eGamble_Num3,
};

// 2015-01-28 赌博系统奖励数据库字段枚�?

enum eGambleInfoOpen
{
    eGambleOpenId,eGambleOpenCharIdx,eGambleOpen_Year,eGambleOpen_Month,eGambleOpen_Day,eGambleOpen_Num1,eGambleOpen_Num2,eGambleOpen_Num3,eGambleOpen_Number1,eGambleOpen_Number2,eGambleOpen_Number3,
};

//-----------------------------------------------------------------------
enum FireInfoList
{
	e_FireInfoIndex,e_FireInfoIpAddr,e_FireInfoTime,
};

enum 
{
	eCharacterBaseQuery = 1,		/// 脛脡赂炉脜脥 卤芒潞禄脕陇潞赂赂娄 戮貌戮卯驴脗麓脵
	eCreateCharacter,			/// 脛鲁赂炉禄媒录潞
	eLoginCheckDelete,			/// 脕垄录脫脙录脜漏禄猫脕娄
	eDeleteCharacter,			/// 脛鲁赂炉脜脥 禄猫脕娄
	eNewCharacterCheckName,     /// 禄玫路脦驴卯 脛鲁赂炉脜脥 脌脤赂搂 脕脽潞鹿脙录脜漏 
	eForcedDisconnect,          /// 掳颅脕娄路脦 卤芒脕赂脌炉脌煤赂娄 虏梅脌陆.
	eSetCurMaxCount,			/// 脟枚脌莽脌炉脌煤驴脥 Max脌炉脌煤录枚 卤芒路脧
	eLoginMapInfoSearchForName, /// 脛鲁赂炉赂铆脌赂路脦 脕垄录脫驴漏潞脦鹿脳 赂脢鹿酶脠拢 戮貌戮卯驴脌卤芒
	eSavePoint,					/// SavePoint 
	eUnRegistLoginMapInfo,		/// 赂脢驴隆录颅 鲁陋掳隆赂茅 陆脟脟脿脟脩麓脵
	eMapUserUnRegistMapInfo,	/// 赂脢录颅鹿枚 脕戮路谩陆脙 陆脟脟脿
	eFriendGetTargetMemeberIDX,	/// 脛拢卤赂 碌卯路脧/禄猫脕娄陆脙 脜赂掳脵 脟脙路鹿脌脤戮卯 戮脝脌脤碌冒 戮貌戮卯驴脥录颅 赂脼陆脙脕枚 
	eFriendAdd,					/// 脛拢卤赂 碌卯路脧
	eFriendIsValidTarget,		/// 脛拢卤赂 碌卯路脧 脟脪录枚 脌脰麓脗脕枚

	eFriendDel,					/// 脛拢卤赂 禄猫脕娄
	eFriendDelID,				/// 脛拢卤赂 禄猫脕娄 (戮脝脌脤碌冒路脦)
	eFriendDelList,				/// 脛拢卤赂赂庐陆潞脝庐 禄猫脕娄(脛鲁赂炉脜脥 禄猫脕娄陆脙)
	eFriendNotifyLogin,			/// 路脦卤脳脌脦 戮脣赂虏
	eFriendGetFriendList,		/// 脛拢卤赂赂庐陆潞脝庐
	eFriendGetLoginFriends,		/// 路脦卤脳脌脦碌脟戮卯脌脰麓脗 脛拢卤赂 戮脣戮脝驴脌卤芒
	eNoteIsNewNote,
	eNoteDelAll,				/// 脗脢脕枚 赂冒碌脦 禄猫脕娄
	eNoteList,					/// 脗脢脕枚赂庐陆潞脝庐
	eNoteDelete,				/// 脗脢脕枚 禄猫脕娄
	eWantedDelete,				/// 脛鲁赂炉脜脥 禄猫脕娄路脦 脌脦脟脩 脙麓禄矛卤脟 掳隆脕酶 脛鲁赂炉脜脥 脕陇潞赂 潞炉掳忙

	eWantedDelWantInfo,			/// 脛鲁赂炉脜脥 禄猫脕娄路脦 脌脦脟脩 脟枚禄贸卤脻 脕陇潞赂 禄猫脕娄

	//For GM-Tool	
	eGM_BanCharacter,
	eGM_UpdateUserLevel,
	eGM_WhereIsCharacter,
	eGM_Login,
	eGM_GetGMPowerList,
	//	
	eAgentDown,					/// 驴隆脌脤脌眉脝庐 脜掳赂茅 脟脴麓莽 驴隆脌脤脌眉脝庐驴隆 录脫脟脩 戮脰碌茅 路脦卤脳 戮脝驴么 脙鲁赂庐
	//----China Billing
	eBilling_RemainTimeUpdate,
	eBilling_UpdateAll,
	//----
	eEventItemUse050106,		// 050106 event
	eCheckGuildMasterLogin,		// checkguildmasterlogin
	eEventItemUse050203,		// 050203 event

	eInsertBillingTable,		// for billling
	eDeleteBillingTable,

	eEventItemUse050322,		// 050322 event

	eJackpot_TotalMoney_Load,	// jackpot TotalMoneyLoad

	eCheckGuildFieldWarMoney,	// check guildfieldwarmoney
	eAddGuildFieldWarMoney,		// add guildfieldwarmoney

	eChaseFindUser,

	eJP_CharacterSlot,			// Get characterslot count

	eEventItemUse051108,			// 051108 event
	eEventItemUse2,

	eGM_UpdateUserState,
	eLogGMToolUse,					// 

	eNProtectBlock,
	eHackToolUser,

	// magi82 - 氍疙晿靸濌磤�?070123)
	eGuildTraineeDelete,

	// punish - autonote甏€�?

	ePunishListLoad,
	ePunishListAdd,
	ePunishCountAdd,

	eInsertLog_AutoBlock,

	eHeroTopList,           //weiye 大侠排行数据库标志定�?2018-01-10

	eMunpTopList,           //weiye 帮会排行数据库标志定�?2018-01-10
	eDungeonTopList,
	eRestpwd,

	eHackInsert,
	eHackLoad,

	eGambleInfoInsert,   // 2015-01-28 赌博系统数据库返回结果标�?

	eGambleInfoOpen,     // 2015-01-28 赌博系统开奖数据库返回结果标志

	eGambleUpdatePdAndGold, // 2014-01-30 赌博系统离线玩家奖励发送标�?

	eGambleClearData,       // 2014-01-30 清理投注数据 

	

	eReloadAllBin,

	eClearLoginEvent,

	

	MaxQuery
};

BOOL isValidQueryString( char * str );


char* BIG5ToGB2312(const char* szBIG5String);
char* UnicodeToGB2312(const wchar_t* szUnicodeString);
wchar_t* BIG5ToUnicode(const char* szBIG5String);
char* GB2312ToBIG5(const char* szGBString);
wchar_t* GB2312ToUnicode(const char* szGBString);
char* UnicodeToBIG5(const wchar_t* szUnicodeString);

BOOL CheckString( const char *strChk );

BOOL CheckMessageString(const char *strChk);


void CheatLog(DWORD CharacterIDX,DWORD CheatKind);
void UserIDXSendAndCharacterBaseInfo(DWORD UserIDX, DWORD AuthKey, DWORD dwConnectionIndex);
void LoginCheckDelete(DWORD UserID);//, DWORD dwConnectionIndex);
//void CreateCharacter(CHARACTERMAKEINFO* pMChar, DWORD dwConnectionIndex);
void CreateCharacter(CHARACTERMAKEINFO* pMChar, WORD ServerNo, DWORD dwConnectionIndex);

//void DeleteCharacter(DWORD dwPlayerID, DWORD dwConnectionIndex);
void DeleteCharacter(DWORD dwPlayerID, WORD ServerNo, DWORD dwConnectionIndex);

void CharacterNameCheck(char* pName, DWORD dwConnectionIndex);
void SetUserCurAndMaxCount(DWORD CurCount, DWORD MaxCount);
void SearchWhisperUserAndSend(DWORD dwPlayerID, char* CharacterName, DWORD dwKey );
void SearchPartyChatSendOnPlayer(DWORD dwPlayerID, DWORD dwKey );
//void SearchMunpaChatSendOnPlayer(DWORD dwPlayerID, DWORD dwKey );
void SaveMapChangePointUpdate(DWORD CharacterIDX, WORD MapChangePoint_Idx);
void UnRegistLoginMapInfo(DWORD CharacterIDX);
//void MapUserUnRegistLoginMapInfo(WORD MapPort);
void FriendGetUserIDXbyName(DWORD CharacterIDX, char* TargetName);
void FriendAddFriend(DWORD CharacterIDX, DWORD TargetID);
void FriendIsValidTarget(DWORD CharacterIDX, DWORD TargetID, char* FromName);
void FriendDelFriend(DWORD CharacterIDX, char* TargetName);
void FriendDelFriendID(DWORD CharacterIDX, DWORD TargetID, DWORD bLast);
void FriendNotifyLogintoClient(DWORD CharacterIDX);
void FriendGetLoginFriends(DWORD CharacterIDX);
void FriendGetFriendList(DWORD CharacterIDX);
void NoteIsNewNote(DWORD PlayerID);
void NoteSendtoPlayer(DWORD FromIDX, char* FromName, char* ToName, char* Note);
void NoteServerSendtoPlayer(DWORD FromIDX, char* FromName, char* ToName, char* Note);
void NoteSendtoPlayerID(DWORD FromIDX, char* FromName, DWORD ToIDX, char* Note);
void NoteDelAll(DWORD CharacterIDX);
void NoteList(DWORD CharacterIDX, WORD Page, WORD Mode);
void NoteRead(DWORD CharacterIDX, DWORD NoteIDX, DWORD IsFront);
void NoteDelete(DWORD PlayerID, DWORD NoteID, BOOL bLast);
void WantedDelete(DWORD CharacterIDX);
void WantedDelWantInfo(DWORD WantedIDX);
void ChaseFindUser(DWORD UserIdx, char* WantedName, DWORD dwItemIdx);

//For GM-Tool
void GM_WhereIsCharacter(DWORD dwID, char* CharacterName, DWORD dwSeacherID );
void GM_BanCharacter(DWORD dwID, char* CharacterName, DWORD dwSeacherID );
void GM_UpdateUserLevel(DWORD dwID, DWORD dwServerGroup, char* Charactername, BYTE UserLevel);
void GM_GetGMPowerList(DWORD dwStartIdx, DWORD dwFlag = 0 );
void GM_Login( DWORD dwConnectionIdx, char* strID, char* strPW, char* strIP );
//


void BillingRemainTimeUpdate(DWORD dwUserIdx, int nRemainTime );
void BillingUpdateAll( WORD wAgentNum );




// Query Result function
void RUserIDXSendAndCharacterBaseInfo(LPQUERY pData, LPDBMESSAGE pMessage);
void RCreateCharacter(LPQUERY pData, LPDBMESSAGE pMessage);
void RLoginCheck(LPQUERY pData, LPDBMESSAGE pMessage);
void RCharacterNameCheck(LPQUERY pData, LPDBMESSAGE pMessage);
void RSearchWhisperUserAndSend(LPQUERY pData, LPDBMESSAGE pMessage);
//void RSearchMunpaChatSendOnPlayer(LPQUERY pData, LPDBMESSAGE pMessage);
void RFriendDelFriend(LPQUERY pData, LPDBMESSAGE pMessage);
void RFriendDelFriendID(LPQUERY pData, LPDBMESSAGE pMessage);
void RFriendGetFriendList(LPQUERY pData, LPDBMESSAGE pMessage);
void RFriendAddFriend(LPQUERY pData, LPDBMESSAGE pMessage);
void RFriendNotifyLogintoClient(LPQUERY pData, LPDBMESSAGE pMessage);
void RNoteIsNewNote(LPQUERY pData, LPDBMESSAGE pMessage);
void RNoteSendtoPlayer(LPLARGEQUERY pData, LPDBMESSAGE pMessage);
void RNoteServerSendtoPlayer(LPLARGEQUERY pData, LPDBMESSAGE pMessage);
void RNoteList(LPQUERY pData, LPDBMESSAGE pMessage);
void RNoteRead(LPLARGEQUERY pData, LPDBMESSAGE pMessage);
void RNoteDelete(LPQUERY pData, LPDBMESSAGE pMessage);
void RDeleteCharacter(LPQUERY pData, LPDBMESSAGE pMessage);

void RFriendGetLoginFriends(LPQUERY pData, LPDBMESSAGE pMessage);
void RFriendIsValidTarget(LPQUERY pData, LPDBMESSAGE pMessage);
void RWantedDelete(LPQUERY pData, LPDBMESSAGE pMessage);

//For GM-Tool
void RGM_WhereIsCharacter(LPQUERY pData, LPDBMESSAGE pMessage);
void RGM_BanCharacter(LPQUERY pData, LPDBMESSAGE pMessage);
void RGM_UpdateUserLevel(LPQUERY pData, LPDBMESSAGE pMessage);
void RGM_GetGMPowerList(LPQUERY pData, LPDBMESSAGE pMessage);
void RGM_Login(LPQUERY pData, LPDBMESSAGE pMessage);
//

// guildfieldwar
void CheckGuildMasterLogin( DWORD dwConnectionIdx, DWORD dwPlayerIdx, char* pSearchName, DWORD dwMoney, BYTE Protocol );


void RCheckGuildMasterLogin( LPQUERY pData, LPDBMESSAGE pMessage );
void CheckGuildFieldWarMoney( DWORD dwConnectionIndex, DWORD dwSenderIdx, DWORD dwEnemyGuildIdx, DWORD dwMoney );
void RCheckGuildFieldWarMoney( LPQUERY pData, LPDBMESSAGE pMessage );
void AddGuildFieldWarMoney( DWORD dwConnectionIndex, DWORD dwPlayerIdx, DWORD dwMoney );
void RAddGuildFieldWarMoney( LPQUERY pData, LPDBMESSAGE pMessage );

// 050106 event
void EventItemUse050106( DWORD dwUserIdx );
// 050203 event
void EventItemUse050203( DWORD dwUserIdx, char* sCharName, DWORD dwServerNum );
// 050322 event
void EventItemUse050322( DWORD dwUserIdx );
// 051108 event
void EventItemUse051108( DWORD dwObjectIdx, DWORD dwUserIdx, char* sCharName, DWORD dwServerNum );
void REventItemUse051108( LPQUERY pData, LPDBMESSAGE pMessage );
void EventItemUse2( DWORD dwUserIdx, char* sCharName, DWORD dwServerNum );


//	jackpot
void JackpotLoadTotalMoney();
void RJackpotLoadTotalMoney( LPQUERY pData, LPDBMESSAGE pMessage );

void RChaseFindUser( LPQUERY pData, LPDBMESSAGE pMessage );

// for billing
void InsertBillingTable( DWORD dwUserIdx, DWORD dwBillType, DWORD dwAgentNo, DWORD dwServerSetNum );
void DeleteBillingTable( DWORD dwUserIdx, DWORD dwServerSetNum );

// Japan
void GetCharacterSlotCount( DWORD UserIdx, DWORD AuthKey, DWORD dwConnectionIndex );
void RGetCharacterSlotCount( LPQUERY pData, LPDBMESSAGE pMessage );

void GM_UpdateUserState(DWORD dwID, DWORD dwServerGroup, char* Charactername, BYTE UserState);
void RGM_UpdateUserState(LPQUERY pData, LPDBMESSAGE pMessage);

void GuildTraineeDelete(DWORD UserIdx);	// magi82 - 氍疙晿靸濌磤�?070123)

//weiye 排行系统公用函数定义 2016-03-20

void GetHeroTopList(int Max_Num);

void RGetHeroTopList(LPQUERY pData, LPDBMESSAGE pMessage);

void GetMunpTopList(int Max_Num);
void RGetMunpTopList(LPQUERY pData, LPDBMESSAGE pMessage);

void GetDungeonTopList(int Max_Num);
void RGetDungeonTopList(LPQUERY pData, LPDBMESSAGE pMessage);

void RestPwd(DWORD dwConnectionIndex);
void Rrestpwd(LPQUERY pData, LPDBMESSAGE pMessage);

void AddFireInfoToDB(char * IpAddr,int Time);
void LoadFireInfoFormDB();
void RAddFireInfoToDB(LPQUERY pData, LPDBMESSAGE pMessage);
void RLoadFireInfoFormDB(LPQUERY pData, LPDBMESSAGE pMessage);

void GambleInfoInsert(int Year,int Month,int Day,int Hour,int Min,int Number1,int Number2,int Number3);

void GambleInfoOpen(int Year,int Month,int Day);

void GambleUpdatePaoDianAndGold(DWORD CharacterIdx,DWORD PaoDian,DWORD Gold);

void GameleClearUserData(int Year,int Month,int Day);  // 开奖完成后，清理的当日投注数据!

void RGamebleInfoInsert(LPQUERY pData, LPDBMESSAGE pMessage);

void RGambleInfoOpen(LPQUERY pData, LPDBMESSAGE pMessage);


// GMlog
void LogGMToolUse( DWORD CharacterIdx, DWORD GMLogtype, DWORD Logkind, DWORD Param1, DWORD Param2 );

// punish
void PunishListLoad( DWORD dwUserIdx );
void RPunishListLoad( LPQUERY pData, LPDBMESSAGE pMessage );
void PunishListAdd( DWORD dwUserIdx, int nPunishKind, DWORD dwPunishTime );

void PunishCountAdd( DWORD dwAutoUserIdx, DWORD dwAutoCharacterIdx, char* pAutoCharacterName, int nPunishKind, int nPeriodDay, DWORD dwAskUserIdx, DWORD dwAskCharacterIdx, char* pAskCharacterName );
void RPunishCountAdd( LPQUERY pData, LPDBMESSAGE pMessage );

void InsertLog_AutoBlock( DWORD dwAutoUserIdx, DWORD dwAutoCharacterIdx, char* pAutoCharacterName, DWORD dwBlockTime, DWORD dwAskUserIdx, DWORD dwAskCharacterIdx, char* pAskCharacterName, int nServerSet );

#ifdef _NPROTECT_
void NProtectBlock(DWORD UserIdx, DWORD CharIdx, char* IP, DWORD BlockType);
void HackToolUser(DWORD UserIdx, DWORD CharIdx, char* CharacterName );
#endif

void ReloadAllBin();

void ClearLoginEventDB();

void BACKUP_DB();



#endif //__AGENTBMSGPARSER_H__


