

#include "stdafx.h"
#include <time.h>
#include "CommonDBMsgParser.h"
#include "MapDBMsgParser.h"
#include "UserTable.h"
#include "Network.h"
#include "ItemManager.h"
#include "MugongManager.h"
#include "Economy.h"
#include "PartyManager.h"
#include "Party.h"
#include "Pet.h"
#include "..\[CC]Header\GameResourceManager.h"
#include "CharMove.h"
#include "Guild.h"
#include "GuildManager.h"
#include "StreetStallManager.h"
#include "FameManager.h"
#include "CQuestBase.h"
#include "TileManager.h"
#include "AbilityManager.h"
#include "PyoGukManager.h"
#include "WantedManager.h"
#include "GuildFieldWarMgr.h"

#include "QuestManager.h"
#include "ShopItemManager.h"
#include "GuildTournamentMgr.h"
#include "GuildUnionManager.h"
#include "SiegeWarMgr.h"
#include "SiegeWarProfitMgr.h"
#include "cJackpotManager.h"
#include "QuestManager.h"
#include "GuildManager.h"	// magi82 - 巩窍积包访(070123)
#include "PackedData.h"

#include "ItemLimitManager.h"
#include "FortWarManager.h"

#include "MapItemDrop.h"	// 甘 酒捞袍 靛而 眠啊 by Stiner(2008/05/29)-MapDropItem

#include "StatsCalcManager.h"  // 立即刷新角色 为成长系统 2015-01-17

#include "GambleManager.h"     // 赌博系统头引用

#include "Dungeon/DungeonMgr.h"

#include "InsDungeonManager.h"
#include "ChannelSystem.h"

#include "shlwapi.h"
#pragma comment(lib,"shlwapi.lib")


extern BOOL g_bPlusTime;


//////////////////////////////////////////////////////////////////
// DB⒋I¨〓IAI ∷〓¨々¨oAAo∷〓| ㄏi∷i¨u¨¨uㄏ ∷〓¨〓¨∷CI∷⊥A CO¨uo⊥ie.
DBMsgFunc g_DBMsgFunc[MaxQuery] =
{
	NULL,
	RCharacterNumSendAndCharacterInfo,		/// CIㄏ┆¨√AC Aㄏ┆∷〓?d¨〓∷〓∷〓ㄏ√⊥iI∷〓| ¨uo¨ui∷?A∷⊥U.
	RCharacterMugongInfo,		/// ㄏoiāㄏǎd¨〓∷〓∷〓| ¨uo¨ui∷?A∷⊥U
	RCharacterItemSlotInfo,					// eCharacterItemSlotQuery
	RCharacterItemInfo,		/// ¨u¨AIAU d¨〓∷〓∷〓| ¨uo¨ui∷?A∷⊥U
	RCharacterItemOptionInfo,
	NULL,						// eCharacterItemOptionDeleteQuery
	RCharacterItemRareOptionInfo,	// eCharacterItemRareOptionQuery
	NULL,						// eCharacterItemRareOptionDeleteQuery
	RPyogukItemOptionInfo,
	RPyogukItemRareOptionInfo,
	RCharacterShopItemInfo,		// eShopItemInfoQuery
//	RCharacterPetInfo,

	RMarryAdd,                 //weiye 结婚系统回调函数 2018-05-01
    RMarryDel,					// marry set 2014-05-10
	RMallListInfo,
	RSSItemInsert,
	RItemOptionInsert,
	RItemRareInsert,		
	/* New Item Work */
	NULL,						// ItemUpdateToDB
	NULL,						// ItemCombineUpdateToDB
	NULL,						// ItemMoveUpdateToDB
	NULL,						// ItemMovePyogukUpdate
	NULL,						// ItemMoveMunpaUpdate
	NULL,						// ItemMovePetInvenUpdateToDB
	RItemInsert,				// ItemInsertToDB
	NULL,						// ItemDeleteToDB
	RShopItemInvenInfo,			// eItemShopInvenInfo,
	RUsingShopItemInfo,			// eItemShopItemUseInfo
	NULL,						// eItemShopItemUsing    RaMa - 04.11.05
	NULL,						// eItemShopItemDelete
	NULL,						// eItemShopItemUpdatetime
	NULL,						// eItemShopItemUpdateParam
	NULL,						// eItemShopItemUpdateUseParam
	NULL,						// eItemShopItemUpdateUseInfo
	NULL,						// eItemShopItemMPInfo
	NULL,						// eItemShopMPUpdate
	NULL,						// eItemShopMPDel
	NULL,						// eItemShopGetItem
	NULL,						// eItemMoveShopUpdate
	RCharacterChangeName,		// eItemShopChangeName
	NULL,						// eItemCharChange
	RShopItemRareInsertToDB,	// eItemShopRareCreate
	RIsUseAbleShopAvatarItem,	// eItemShopIsUseableAvatar
	RIsAbleShopEquipItem,		// eItemShopIsUseableEquip
	NULL,						// eItemShopUpdatCharacterIdx
	NULL,						// eItemShopUpdateAllInfo

	NULL,						// RegistLoginMapInfo
	NULL,						// UnRegistLoginMapInfo
	
	/* New Mugong Work */
	NULL,						// MugongUpdateToDB
	NULL,						// MugongMoveUpdateToDB
	RMugongInsert,				// MugongInsertToDB
	NULL,						// MugongDeleteToDB

//	NULL,				/// ㄏoiāㄏǎief
	RMapBaseEconomy,				/// aA¨ǎ¨oA¨u¨uCIAo∷〓∷c
	NULL,				/// āㄏ∷〓A⊥ii⒋ICwI¨oAn
	NULL,			/// Aㄏ┆∷〓?d¨〓∷〓 ¨u%iIAI¨∷c
	NULL,			/// HeroInfo ¨u%iIAI¨∷c
	NULL,			/// TotalInfo ¨u%iIAI¨∷c
	NULL,			// eBadFameUpdate,
	
	RAuctionCheck,
	RAuctionPageList,
	RAuctionConfirm,
	RPartyLoad,	//¨A¨¨u		

	///// 2008. 6. CBH - 颇萍概莫 眠啊等 可记 包访 荐沥 ///////////////////
	//RPartyInfoByUserLogin,
	//RPartyCreate,
	NULL,
	NULL,
	///////////////////////////////////////////////////////////////////////////
	NULL,
	NULL,
	NULL,
	RPartyDelMember,	// PartyDelMember
	RPartyChangeMaster, //PartyChangeMaster
	
	NULL,
	RSaveMapChangePointReturn,
	
	/*
	RMunpaLoad, //ㄏo∷c¨A 1
	RMunpaItemLoad,
	RMunpaItemOption,
	RMunpaLoadSyn,
	RMunpaLoadSyn,
	RMunpaBreakUp,
	RMunpaBreakUpMembers,
	NULL,
	NULL,
	RMunpaChangeMemberRank, // eMunpaChangeMemberRank
	RMunpaBanMember,
	RMunpaSecede,
	RMunpaAcceptMember, 
	RMunpaDenyMember,
	RMunpaJoinMember,
	RMunpaJoinMemberCancel,
	RMunpaRegularMembersInfo,
	RMunpaJoinMembersInfo,
	RMunpaBoardInfo,
	RMunpaCreateBoard,
	NULL, 
	RMunpaBoardRankInfo, 
	RMunpaBoardRankInfoUpdate,
	NULL,
	RMunpaBoardDeleteContents,
	RMunpaMoneyPutInOut,
	*/
	

	NULL,
	RCharacterPyogukInfo,			// eCharacterPyogukInfo
	RCharacterPyogukItemInfo,		// eCharacterPyogukItemInfo	
	NULL,					// PyogukMoneyUpdateToDB

	RFriendNotifyLogouttoClient,	
//	RFameMunpaUpdate,
	NULL,
	NULL,					//eBadFameCharacterUpdate
	NULL,					//ePKCharacterUpdate

	RWantedLoad,
	RWantedInfoByUserLogIn,
	NULL,
	RWantedRegist,
	NULL,
	RWantedComplete,
	RWantedDestroyed,
	RWantedOrderedList,
	RWantedMyList,
	RWantedSearch,

	RQuestTotalInfo,		// eQuestTotalInfo                         
	NULL,					// eQuestUpdate
	RQuestMainQuestLoad,	// eQuestMainQuestLoad
	RQuestSubQuestLoad,		// eQuestSubQuestLoad
	RQuestItemLoad,			// eQuestItemLoad
	NULL,					// eQuestMainQuestInsert
	NULL,					// eQuestSubQuestInsert
	NULL,					// eQuestItemInsert
	NULL,					// eQuestMainQuestUpdate
	NULL,					// eQuestSubQuestUpdate
	NULL,					// eQuestItemUpdate
	NULL,					// eQuestMainQuestDelete
	NULL,					// eQuestSubQuestDelete
	NULL,					// eQuestSubQuestDeleteAll	
	NULL,					// eQuestItemDelete
	NULL,					// eQuestItemDeleteAll
	NULL,					// eQuestMainQuestUpdateCheckTime

	RJournalGetList,		// eJournalGetList,
	NULL,					// eJournalInsertQuest,
	NULL,					// eJournalInsertWanted,
	NULL,					// eJournalInsertLevel,
	NULL,					// eJournalSaveUpdate,
	NULL,					// eJournalDelete,
	/////////////////////////////////////////////
	// Log āu⒋A 
	NULL,					// eLogCharacter,
	NULL,					// eLogExp,
	NULL,					// eLogMoney,
	NULL,					// eLogMoneyWrong,
	NULL,					// eLogMugong,
	NULL,					// eLogMugongExp,
	NULL,					// eLogItemMoney,
	NULL,					// eLogItemReinforce,
	NULL,					// eLogItemRare,
	NULL,					// eLogCheat,
	NULL,					// eInsertLogTool	
	NULL,					// eLogGuild
	NULL,					// eLogPet
	NULL,					// eLogGuildPoint,
	NULL,					// eLogGMToolUse
	/////////////////////////////////////////////

	NULL,					// eCharacterInfoByTime,
	NULL,					// eMugongExpByTime,
	NULL,					// eSpeedHackCheck,

	NULL,					// eGameLogItem,

	RGuildLoadGuild,		// eGuildLoadGuild, 
	RGuildLoadMember,		// eGuildLoadMember,
	RGuildCreate,			// eGuildCreate,
	RGuildBreakUp,			// eGuildBreakUp,
	RGuildDeleteMember,		// eGuildDeleteMember,
	NULL,					// eGuildAddMember,
	RGuildSecedeMember,		// eGuildSecedeMember
	NULL,					// eGuildLevelUp
	NULL,					// eGuildChangeMemberRank
	RGuildItemLoad,			// eGuildItemLoad
	RGuildItemOption,		// eGuildItemOption
	RGuildItemRareOption,	// eGuildItemRareOption
	NULL,					// eGuildMoneyPutInOut
	NULL,					// eGuildGiveNickName
	
	RConfirmUserOut,		// eConfirmUserOut
	RSaveCharInfoBeforeLogout,

	RCharacterInfoQueryToEventMap, //eCharacterInfoQueryToEventMap
	
	RLoadGuildWarRecord,	// eLoadGuildWarRecord	
	NULL,					// eGuildWarRecordDelete
	RLoadGuildFieldWar,		// eLoadGuildFieldWar
	NULL,					// eInsertGuildFieldWar
	NULL,					// eDeleteGuildFieldWar
	NULL,					// eUpdateGuildFieldWarRecord
	
	RGuildTournamentInfoLoad,			// eGuildTournament_LoadAll,
	RGuildTournamentCancelIdxLoad,		// eGuildTournament_CancelLoad,
	NULL,								// eGuildTournament_CancelInsert
	NULL,								// eGuildTournament_Insert,
	NULL,								// eGuildTournament_Delete,
	NULL,								// eGuildTournament_UpdateGuildInfo,
	NULL,								// eGuildTournament_UpdateTournamentInfo,
	
//	NULL,	//	RJackpotLoadTotalMoney,				// eJackpot_TotalMoney_Load
	NULL,	//	RJackpotAddTotalMoney,				//	eJackpot_MapMoney_Update,
	RJackpotPrizeInfo,					//	eJackpot_PrizeInfo_Send,

	RGuildUnionLoad,					// eGuildUnionLoad
	RGuildUnionCreate,					// eGuildUnionCreate
	NULL,								// eGuildUnionDestroy	
	NULL,								// eGuildUnionAddGuild
	NULL,								// eGuildUnionRemoveGuild
	NULL,								// eGuildUnionSecedeGuild

	RSiegeWarInfoLoad,					// eSiegeWarInfoLoad
	NULL,								// eSiegeWarInfoInsert
	NULL,								// eSiegeWarInfoUpdate,
	RSiegeWarGuildInfoLoad,				// eSiegeWarGuildInfoLoad
	NULL,								// eSiegeWarGuildInsert,
	NULL,								// eSiegeWarGuildUpdate,
	NULL,								// eSiegeWarGuildDelete,

	NULL,								// eChangeCharacterAttr

	RSiegeWarProfitInfoLoad,			// eSiegeWarProfitInfoLoad
	NULL,								// eSiegeWarProfitUpdateGuild
	NULL,								// eSiegeWarProfitUpdateTaxRate
	RSiegeWarProfitUpdateProfitMoney,	// eSiegeWarProfitUpdateProfitMoney

	NULL,								// eChangeGuildLocation
	NULL,								// eChangeCharacterStageAbility

	RCharacterPetInfo,					// eCharacterPetInfoQuery
	RPetInsert,							// ePetInsert
	NULL,								// ePetUpdate
	NULL,								// ePetDelete
	
	RPetInvenItemOptionInfo,			// ePetInvenItemOptionInfoQuery
	RPetInvenItemRareOptionInfo,		// ePetInvenItemRareOptionInfoQuery
	RPetInvenItemInfo,					// ePetInvenItemInfoQuery

	NULL,								// eGuildLoadNotice,
	NULL,								// eGuildUpdateNotice,

	RQuestEventCheck,					// eQuestEventCheck
	NULL,								// eQuestEventEnd

	RPetWearItemInfo,					// ePetWearItemInfoQuery

	NULL,								//eGMEvent01

	RGuildLoadGuildPointInfo,			//eGuildPointInfoLoad
	RGuildLoadGuildPlustimeInfo,		//eGuildPlustimeInfoLoad
	RGuildAddHuntedMonsterCount,		//eGuildAddHuntedMonsterCount
	RGuildGetHuntedMonsterTotalCountWithInit,	//eGuildConvertHuntedMonsterCount
	RGuildAddGuildPoint,				//eGuildAddGuildPoint
	RGuildUseGuildPoint,				//eGuildUseGuildPoint
	RGuildPlustimeEnd,					//eGuildPlustimeEnd
	NULL,								// eGuildTournamentAddLog
	NULL,								// eSiegeWarAddLog

	RGuildItemLoadInNeed,				//eGuildItemLoadInNeed
	RGuildItemOptionInNeed,				//eGuildItemOptionInNeed
	RGuildItemRareOptionInNeed,			//eGuildItemRareOptionInNeed

	// magi82 //////////////////////////////////////////////////////////////////////////
	NULL,								//eCharacterExpFlag
	////////////////////////////////////////////////////////////////////////////////////

	//magi82 - 巩窍积 啊涝祈府矫胶袍 /////////////////////////////////////////
    RGuildTraineeInfo,					//eGuildTraineeInfo
	NULL,								//eGuildTraineeInsert
	NULL,								//eGuildTraineeDelete

	RGuildTraineeGuildInfo,				//eGuildTraineeGuildInfo
	NULL,								//eGuildTraineeGuildDelete
	
	NULL,								// eGuildTraineeDeletebyGuild
	RGuildAddStudent,					// eGuildAddStudent
	//////////////////////////////////////////////////////////////////////////

	RGuildStudentLvUpCtUpdate,			//eGuildStudentLvUpCount,
	RGuildStudentLvUpCtInit,			//eGuildStudentLvUpInfoInit,

	// magi82 - Titan(070209)
	RTitanWearItemInfo,					// eTitanWearItemInfo,

	//SW070127 鸥捞藕
	RCharacterTitanInfo,				//eCharacterTitanInfoQuery,
	RCharacterTitanEquipItemEnduranceInfo,	//eCharacterTitanEquipItemEnduranceInfoQuery
	RTitanInsertToDB,					//eTitanInsert,
	NULL,								//eTitanUpdate,
	NULL,								//eTitanDelete,

	RTitanEquipItemInfoInsertToDB,		//eTitanEquipInfoInsert,
	NULL,								//eTitanEquipInfoUpdate,
	NULL,								//eTitanEquipInfoDelete,

	RTestGameQuery,							// eTestGameQuery
	RTestLogQuery,							// eTestLogQuery

	// magi82 - Titan(071015) 芒绊俊 鸥捞藕 厘厚 包访
	RPyogukTitanEnduranceInfo,			// ePyogukTitanEnduranceOptionQuery

	NULL,								// eQuest_EndQuest_New
	NULL,								// eQuest_DeleteQuest_New
	NULL,								// eQuest_EndSubQuest_New
	// magi82(33)
	RTitanEquipInsertExceptionToDB,		//eTitanEquipInfoInsertException
	RTitanEquipInsertExceptionPyogukToDB,		//eTitanEquipInfoInsertExceptionPyoguk
	NULL,		//eCharacterSkinInfoUpdate

	// magi82(41) - 讥酒捞袍 眠啊(胶泡 檬扁拳 林巩辑)
	NULL,		//eCharacterUpdateResetStatusPoint

	RCharacterSkinInfo,	//eCharacterSkinInfo

	RItemLimitInfoLoadAll,	// eItemLimitInfo_LoadAll,	// tamoo
	RItemLimitInfoUpdate,	// eItemLimitInfo_Update,	// tamoo

	RAutoNoteListLoad,		// eAutoNoteListLoad
	RAutoNoteListAdd,		// eAutoNoteListAdd

	// 甘 酒捞袍 靛而 眠啊 by Stiner(2008/05/28)-MapItemDrop
	RMapItemDropListSelect,		// eMapItemDropListSelect
	RMapItemDropListUpdate,		// eMapItemDropListUpdate
	NULL,						// eMapItemDropListInit

	RFortWarInfoLoad,		// eFortWarInfoLoad
	NULL,					// eFortWarInfoUpdate
	RFortWarProfitMoneyUpdate,	// eFortWarProfitMoneyUpdate
	RFortWarItemLoad,		// eFortWarItemLoad
	RFortWarItemInsertToDB,	// eFortWarItemInsertToDB
	NULL,	
	RCharacterItemStoneOptionInfo,  // 新返回存储函数 2014-11-14
	RItemStoneInsertToDB,           // 执行完毕回调函数 2014-11-15
	RItemStoneUpdateToDB,           // 执行完毕回调函数 2014-11-18
	RPyogukItemStoneInfo,			// 访问仓库数据回调函数 2015-01-06
	RItemLockUpdate,	    //  2014-12-06 函数回调函数 
	RItemUnLockUpdate,      //  2014-12-07 解锁函数回调函数 
	RItemGrowUpdate,        //  2015-01-15 成长回调函数  
	RGambleUserInfoInsert,  //  2015-01-27 赌博投注回调函数
	RCharacterGambleInfo,   //  2015-01-28 赌博系统投注信息查询回调函数
	RCharacterGamblePublicInfo, // 2015-01-28 赌博系统开奖信息查询回调函数!
	RCheckSafePin,            //weiye 安全码检查数据库回调函数 2017-12-04
	RChangeSafePin,         //liuwe  安全码检查数据库回调函数 2017-12-07
	RUpdateSafeStatic,      //weiye 安全码状态更新函数   2017-12-06
	RUpdateLastMapInfo,
	RFBTimeUpdate,           //weiye 副本时间处理         2018-02-06
	RItemLinkSelectItemInfo,           //weiye 物品查询回调          2018-03-11
	RTidyItem,				      //weiye 整理包袱数据库回调    2018-03-22
	RLoadTidyItemInfo,       //weiye 整理包袱载入物品函数 2018-03-20

	

	NULL,//eRebornInfoUpdate
	
	RNPCCODE,

	RTidyItemShop,
	RLoadTidyItemShopInfo,

	NULL,//clearfort


	RItemFlashNameSet,
	RItemFlashNameSet2,

	RUpdateUserCredit,

	

//	NULL,//CharacterHeroGoldInfoUpdate //eGetDBGoldMoney //player.cpp

//	NULL,


	RForbidChatUpdate,
	RForbidChatLoad,

	NULL,//eLoginEventUpdate

	NULL,						//eInstanceDungeonRankUpdate,
	RLoadInstanceDungeonRank,	//eInstanceDungeonRankLoad,
    RUpdateGradeItem
};


BOOL CheckString( const char *strChk )
{

	if(StrStrI(strChk,"select")!=NULL ||
	 StrStrI(strChk,"truncate")!=NULL || 
	   StrStrI(strChk,"delete")!=NULL || 
	   StrStrI(strChk,"insert")!=NULL ||
	   StrStrI(strChk,"update")!=NULL ||
	     StrStrI(strChk,"drop")!=NULL ||
	   StrStrI(strChk,"create")!=NULL ||
	     StrStrI(strChk,"exec")!=NULL ||
	    StrStrI(strChk,"alter")!=NULL ||
		 StrStrI(strChk,"like")!=NULL ||
		 StrStrI(strChk,"join")!=NULL ||
		StrStrI(strChk,"union")!=NULL ||
		StrStrI(strChk,"where")!=NULL ||
		  StrStrI(strChk,"and")!=NULL ||
		     StrStrI(strChk,"or")!=NULL)

	{
		return TRUE;
	}

	return FALSE;
}


BOOL isValidQueryString( char * str )
{
	while( *str != '\0' )
		if( *(str++) == '\'' )
			return FALSE;

	return TRUE;
}


char txt[512];	

void CheckAuction(MSGBASE* msg)
{
//	char txt[128];
	int CharacterIDX = msg->dwObjectID;
	sprintf(txt, "EXEC %s %d", STORED_AUCTION_CHECK, CharacterIDX);	
	g_DB.Query(eQueryType_FreeQuery,eAuctionCheck,CharacterIDX,txt);

}
//∷〓∷c¨i∷i¨∷c āEio ㄏi|i【ㄏoy; CASEㄏo∷c8⒋I..
void AuctionSearch(SEARCHLIST* msg)
{
//	char txt[128];
	int searchtype;
	char name[15];

	strcpy(name,((char*)msg->name));
	searchtype = msg->search_type;
	int CharacterIDX = msg->dwObjectID;

	// 2014-06-25 check string param !

	if( !isValidQueryString( name ))
	{
	   return;
	}

	if( CheckString( name ) )
	{
		return;
	}

	sprintf(txt, "EXEC %s %d \'%s\'", STORED_AUCTION_SEARCH, searchtype, name );
	g_DB.Query(eQueryType_FreeQuery,eAuctionPageList,CharacterIDX,txt);
}
//∷〓∷c¨i∷i¨∷c d⒋A; CASEㄏo∷c8⒋I..
void AuctionSort(SORTLIST* msg)
{
//	char txt[128];
	int type;
	int page;
	type = msg->sort_type;
	page = msg->pagenum;
	int CharacterIDX = msg->dwObjectID;

	sprintf(txt, "EXEC %s %d \'%s\'", STORED_AUCTION_SEARCH, type, page );
	g_DB.Query(eQueryType_FreeQuery,eAuctionPageList,CharacterIDX,txt);
}
//āㄏ∷〓A ⊥ii⒋I 
void AuctionRegister(REGISTERAUCTION* msg)
{
//	char txt[128];
	int  index = msg->PRO_Index;
	int  amount = msg->PRO_Amount;
	char duedate[10];
	strcpy(duedate,msg->BID_DUEDATE);
	int  price = msg->BID_HighPrice;
	int  immediate = msg->BID_ImmediatePrice;
	char name[15];
	strcpy(name,msg->Auctioneer);
	
	DWORD characterIDX = msg->dwObjectID;
	int CharacterIDX = msg->dwObjectID;

    // 2014-06-25 check string param !

	if( !isValidQueryString( duedate ) ||   !isValidQueryString( name ))
	{
	   return;
	}

	if( CheckString( duedate ) || CheckString( name ))
	{
		return;
	}

	sprintf(txt, "EXEC %s %d %d %s %d %d %s", STORED_AUCTION_REGISTER,index,amount,duedate,price,immediate,name);
	g_DB.Query(eQueryType_FreeQuery,eAuctionConfirm,CharacterIDX,txt);
}
//āㄏ∷〓A Au∷?¨I
void AuctionJoin(JOINAUCTION* msg)
{
//	char txt[128];
	int index = msg->REG_Index;
	int price = msg->BID_Price;
	char name[15];
	strcpy(name,msg->JOIN_Name);
	int CharacterIDX = msg->dwObjectID;

	// 2014-06-25 check string param !

	if( !isValidQueryString( name ))
	{
	   return;
	}

	if( CheckString( name ) )
	{
		return;
	}

	sprintf(txt, "EXEC %s %d %d %s", STORED_AUCTION_JOIN, index, price, name);
	g_DB.Query(eQueryType_FreeQuery,eAuctionConfirm,CharacterIDX,txt);
}
//āㄏ∷〓A ⊥ii⒋I, Au∷?¨I∷〓| CASE⒋I 
void AuctionCancel(AUCTIONCANCEL *msg)
{
//	char txt[128];
	int type = 0 ;
	int regindex = 0;
	char name[15];
	int CharacterIDX = msg->dwObjectID;

	// 2014-06-25 check string param !

	if( !isValidQueryString( name ))
	{
	   return;
	}

	if( CheckString( name ) )
	{
		return;
	}

	sprintf(txt, "EXEC %s %d %d %s", STORED_AUCTION_CANCEL, type, regindex, name);
	g_DB.Query(eQueryType_FreeQuery,eAuctionConfirm,CharacterIDX,txt);
}


void CharacterNumSendAndCharacterInfo(DWORD CharacterIDX, DWORD Protocol)
{
#ifdef _JAPAN_LOCAL_
	sprintf(txt, "EXEC dbo.MP_CHARACTER_SelectByCharacterIDX_JP %d", CharacterIDX);
	g_DB.Query(eQueryType_FreeQuery, eCharacterInfoQuery, Protocol, txt);
#elif defined _HK_LOCAL_
	sprintf(txt, "EXEC dbo.MP_CHARACTER_SelectByCharacterIDX_JP %d", CharacterIDX);
	g_DB.Query(eQueryType_FreeQuery, eCharacterInfoQuery, Protocol, txt);
#elif defined _TL_LOCAL_
	sprintf(txt, "EXEC dbo.MP_CHARACTER_SelectByCharacterIDX_JP %d", CharacterIDX);
	g_DB.Query(eQueryType_FreeQuery, eCharacterInfoQuery, Protocol, txt);
#else
	sprintf(txt, "EXEC %s %d", STORED_CHARACTER_SELECT, CharacterIDX);
	g_DB.Query(eQueryType_FreeQuery, eCharacterInfoQuery, Protocol, txt);
#endif
}

void CharacterNumSendAndCharacterInfo2(DWORD CharacterIDX, DWORD Protocol)
{
#ifdef _JAPAN_LOCAL_
	sprintf(txt, "EXEC dbo.MP_CHARACTER_SelectByCharacterIDX_JP %d", CharacterIDX);
	g_DB.Query(eQueryType_FreeQuery, eCharacterInfoQueryToEventMap, Protocol, txt);
#elif defined _HK_LOCAL_
	sprintf(txt, "EXEC dbo.MP_CHARACTER_SelectByCharacterIDX_JP %d", CharacterIDX);
	g_DB.Query(eQueryType_FreeQuery, eCharacterInfoQueryToEventMap, Protocol, txt);
#elif defined _TL_LOCAL_
	sprintf(txt, "EXEC dbo.MP_CHARACTER_SelectByCharacterIDX_JP %d", CharacterIDX);
	g_DB.Query(eQueryType_FreeQuery, eCharacterInfoQueryToEventMap, Protocol, txt);
#else
	sprintf(txt, "EXEC %s %d", STORED_CHARACTER_SELECT, CharacterIDX);
	g_DB.Query(eQueryType_FreeQuery, eCharacterInfoQueryToEventMap, Protocol, txt);
#endif
}

void CharacterMugongInfo(DWORD CharacterIDX, DWORD Protocol)
{
//	char txt[128];
	sprintf(txt, "EXEC %s %d", STORED_MAPCHANGE_MUGONGINFO, CharacterIDX);
	g_DB.Query(eQueryType_FreeQuery, eCharacterMugongQuery, Protocol, txt);
}

void CharacterItemSlotInfo(DWORD CharacterIDX)
{
	sprintf(txt, "EXEC %s %d", STORED_CHARACTER_ITEMSLOTINFO, CharacterIDX);
	g_DB.Query(eQueryType_FreeQuery, eCharacterItemSlotQuery, CharacterIDX, txt);
}

void CharacterItemInfo(DWORD CharacterIDX, DWORD Protocol)
{
//	char txt[128];
	sprintf(txt, "EXEC %s %d", STORED_CHARACTER_ITEMINFO, CharacterIDX);
	g_DB.Query(eQueryType_FreeQuery, eCharacterItemQuery, Protocol, txt);
}

void CharacterItemRareOptionInfo(DWORD CharacterIDX, DWORD Protocol)
{
	sprintf(txt, "EXEC %s %d",
		"dbo.MP_CHARACTER_ItemRareOptionInfo", //STORED_CHARACTER_ITEMRAREOPTIONINFO,
		CharacterIDX);
	g_DB.Query(eQueryType_FreeQuery, eCharacterItemRareOptionQuery, Protocol, txt);
}

void CharacterItemOptionInfo(DWORD CharacterIDX, DWORD Protocol)
{
//	char txt[128];
	sprintf(txt, "EXEC %s %d", STORED_CHARACTER_ITEMOPTIONINFO, CharacterIDX);
	g_DB.Query(eQueryType_FreeQuery, eCharacterItemOptionQuery, Protocol, txt);
}

// 2014-11-14  新打控系统开发，数据库访问!

void CharacterItemStoneOptionInfo(DWORD CharacterIDX,DWORD Protocol)
{
	sprintf(txt,"EXEC %s %d", STORED_CHARACTER_ITEMSTONE,CharacterIDX);

	g_DB.Query(eQueryType_FreeQuery,eCharacterItemStoneQuery,Protocol,txt);
}

// 2015-01-28 赌博系统发送角色投注信息

void CharacterGambleInfo(DWORD CharacterIDX,int Year,int Month, int Day,DWORD Protocol)
{
	sprintf(txt,"EXEC %s %d,%d,%d,%d", STORED_GAMBLE_USER_INFO_SELECT,CharacterIDX,Year,Month,Day);

	g_DB.Query(eQueryType_FreeQuery,eGambleUserInfoSelect,Protocol,txt);
}

// 2015-01-28 赌博系统获取系统开奖信息

void CharacterGamblePublicInfo(DWORD CharacterIDX,int Year,int Month, int Day,DWORD Protocol)
{
	sprintf(txt,"EXEC %s %d,%d,%d,%d", STORED_GAMBLE_INFO_SELECT,CharacterIDX,Year,Month,Day);

	g_DB.Query(eQueryType_FreeQuery,eGambleInfoSelect,Protocol,txt);
}

void PyogukItemRareOptionInfo(DWORD CharacterIDX, DWORD UserID, DWORD StartDBIdx)
{
	sprintf(txt, "EXEC dbo.MP_PYOGUK_ITEMRAREOPTION_INFO %d, %d", UserID, StartDBIdx);
	g_DB.Query(eQueryType_FreeQuery, ePyogukItemRareOptionQuery, CharacterIDX, txt);
}

void PyogukItemOptionInfo(DWORD CharacterIDX, DWORD UserID, DWORD StartDBIdx)
{
//	char txt[128];
	sprintf(txt, "EXEC dbo.MP_PYOGUK_ITEMOPTION_INFO %d, %d", UserID, StartDBIdx);
	g_DB.Query(eQueryType_FreeQuery, ePyogukItemOptionQuery, CharacterIDX, txt);
}
// 2015-01-04 数据访问仓库
void PyogukItemStoneInfo(DWORD CharacterIDX, DWORD UserID, DWORD StartDBIdx)
{
	sprintf(txt, "EXEC MP_PYOGUK_ItemStone_info %d, %d", UserID, StartDBIdx);
	g_DB.Query(eQueryType_FreeQuery, eItemPyogukStone, CharacterIDX, txt);
}

void PetInvenItemOptionInfo( DWORD CharacterIDX, DWORD UserID, WORD StartPos, WORD EndPos )
{
	sprintf(txt, "EXEC dbo.MP_PETINVEN_ITEMOPTION_INFO %d, %d, %d", UserID, StartPos, EndPos);
	g_DB.Query(eQueryType_FreeQuery, ePetInvenItemOptionInfoQuery, CharacterIDX, txt);
}

void PetInvenItemRareOptionInfo( DWORD CharacterIDX, DWORD UserID, WORD StartPos, WORD EndPos )
{
	sprintf(txt, "EXEC dbo.MP_PETINVEN_ITEMRAREOPTION_INFO %d, %d, %d", UserID, StartPos, EndPos);
	g_DB.Query(eQueryType_FreeQuery, ePetInvenItemRareOptionInfoQuery, CharacterIDX, txt);
}

void PetInvenItemInfo( DWORD CharacterIDX, WORD StartPos, WORD EndPos )
{
	sprintf(txt, "EXEC dbo.MP_PETINVEN_ITEMINFO %d, %d, %d", CharacterIDX, StartPos, EndPos);
	g_DB.Query(eQueryType_FreeQuery, ePetInvenItemInfoQuery, CharacterIDX, txt);
}

void CharacterItemOptionDelete(DWORD OptionIdx, DWORD dwItemDBIdx)
{
//	char txt[128];
	sprintf(txt, "EXEC dbo.MP_ITEM_OPTION_Delete %d, %d", OptionIdx, dwItemDBIdx);
	g_DB.Query(eQueryType_FreeQuery, eCharacterItemOptionDeleteQuery, 0, txt);
}

void CharacterItemRareOptionDelete(DWORD RareIdx, DWORD dwItemDBIdx)
{
	sprintf(txt, "EXEC dbo.MP_ITEM_RARE_OPTION_Delete %d, %d", RareIdx, dwItemDBIdx);
	g_DB.Query(eQueryType_FreeQuery, eCharacterItemRareOptionDeleteQuery, 0, txt);
}

//
void ShopItemInvenInfo( DWORD CharacterIdx)
{
	sprintf(txt, "EXEC %s %d", STORED_SHOPINVEN_INFO, CharacterIdx);
	g_DB.Query(eQueryType_FreeQuery, eItemShopInvenInfo, CharacterIdx, txt);
}

void PetWearItemInfo( DWORD CharacterIdx, POSTYPE FromPos, POSTYPE ToPos )
{
	sprintf(txt, "EXEC dbo.MP_PET_WEARITEMINFO %d, %d, %d", CharacterIdx, FromPos, ToPos);
	g_DB.Query(eQueryType_FreeQuery, ePetWearItemInfoQuery, CharacterIdx, txt);
}

// RaMa 04.11.04
void CharacterShopItemInfo(DWORD CharacterIDX, DWORD StartDBIdx)
{
	sprintf(txt, "EXEC %s %d, %d", STORED_SHOPITEM_INFO, CharacterIDX, StartDBIdx);
	g_DB.Query(eQueryType_FreeQuery, eShopItemInfoQuery, CharacterIDX, txt);
}

////////////////////////////////////////////////////////////////////////////////
// RaMa 04.11.05
void UsingShopItemInfo( DWORD CharacterIdx )
{
	sprintf(txt, "EXEC %s %d", STORED_SHOPITEM_USEINFO, CharacterIdx);
	g_DB.Query(eQueryType_FreeQuery, eItemShopItemUseInfo, CharacterIdx, txt);
}

void SavedMovePointInfo( DWORD CharacterIdx )
{	
	g_DB.FreeMiddleQuery( RSavedMovePointInfo, CharacterIdx, 
		"EXEC %s %d", STORED_SHOPITEM_MPINFO, CharacterIdx );
}

void SavedMovePointInsert( DWORD CharacterIdx, char* Name, DWORD MapIdx, DWORD Position )
{	
    // 2014-06-25 check string param !

	if( !isValidQueryString( Name ))
	{
	   return;
	}

	if( CheckString( Name ) )
	{
		return;
	}

	g_DB.FreeMiddleQuery( RSavedMovePointInsert, CharacterIdx, 
		"EXEC %s %d, \'%s\', %d, %d", STORED_SHOPITEM_MPINSERT, CharacterIdx, Name, MapIdx, Position );
}

void SavedMovePointUpdate( DWORD CharacterIdx, DWORD DBIdx, char* Name )
{
	 // 2014-06-25 check string param !

	if( !isValidQueryString( Name ))
	{
	   return;
	}

	if( CheckString( Name ) )
	{
		return;
	}

	sprintf(txt, "EXEC %s %d, %d, \'%s\'", STORED_SHOPITEM_MPUPDATE, CharacterIdx, DBIdx, Name);
	g_DB.Query(eQueryType_FreeQuery, eItemShopMPUpdate, 0, txt);
}

void SavedMovePointDelete( DWORD CharacterIdx, DWORD DBIdx )
{
	sprintf(txt, "EXEC %s %d, %d", STORED_SHOPITEM_MPDEL, CharacterIdx, DBIdx);
	g_DB.Query(eQueryType_FreeQuery, eItemShopMPDel, 0, txt);
}

void UsingShopItemUpdateToDB( DWORD UserIdx, DWORD CharacterIdx, DWORD dwItemIdx, DWORD dwDBIdx, DWORD Param, QSTATETYPE StartTime, QSTATETYPE RemainTime )
{
#ifdef _JAPAN_LOCAL_
	sprintf(txt, "EXEC %s %d, %d, %d, %d, %d, %d, %d", STORED_SHOPITEM_USING_JP, UserIdx, CharacterIdx, dwItemIdx, dwDBIdx, Param, StartTime, RemainTime);
#elif defined _HK_LOCAL_
	sprintf(txt, "EXEC %s %d, %d, %d, %d, %d, %d, %d", STORED_SHOPITEM_USING_JP, UserIdx, CharacterIdx, dwItemIdx, dwDBIdx, Param, StartTime, RemainTime);
#elif defined _TL_LOCAL_
	sprintf(txt, "EXEC %s %d, %d, %d, %d, %d, %d, %d", STORED_SHOPITEM_USING, UserIdx, CharacterIdx, dwItemIdx, dwDBIdx, Param, StartTime, RemainTime);
#else
	sprintf(txt, "EXEC %s %d, %d, %d, %d, %d, %d", STORED_SHOPITEM_USING, CharacterIdx, dwItemIdx, dwDBIdx, Param, StartTime, RemainTime);
#endif
	g_DB.Query(eQueryType_FreeQuery, eItemShopItemUsing, 0, txt);
}
void ShopItemDeleteToDB( DWORD CharacterIdx, DWORD dwDBIdx )
{
	sprintf(txt, "EXEC %s %d, %d", STORED_SHOPITEM_DELETE, CharacterIdx, dwDBIdx);
	g_DB.Query(eQueryType_FreeQuery, eItemShopItemDelete, 0, txt);
}

void ShopItemUpdatetimeToDB( DWORD CharacterIdx, DWORD dwItemIdx, DWORD RemainTime )
{
	sprintf(txt, "EXEC %s %d, %d, %d", STORED_SHOPITEM_UPDATETIME, CharacterIdx, dwItemIdx, RemainTime);
	g_DB.Query(eQueryType_FreeQuery, eItemShopItemUpdatetime, 0, txt);
}

void ShopItemParamUpdateToDB( DWORD CharacterIdx, DWORD dwItemDBIdx, DWORD Param )
{
	sprintf(txt, "EXEC %s %d, %d, %d", STORED_SHOPITEM_UPDATEPARAM, CharacterIdx, dwItemDBIdx, Param);
	g_DB.Query(eQueryType_FreeQuery, eItemShopItemUpdateParam, 0, txt);
}

void ShopItemUseParamUpdateToDB( DWORD UserIdx, DWORD CharacterIdx, DWORD dwItemIdx, DWORD Param )
{
#ifdef _JAPAN_LOCAL_
	sprintf(txt, "EXEC %s %d, %d, %d, %d", STORED_SHOPITEM_UPDATEUSEPARAM_JP, UserIdx, CharacterIdx, dwItemIdx, Param);
#elif defined _HK_LOCAL_
	sprintf(txt, "EXEC %s %d, %d, %d, %d", STORED_SHOPITEM_UPDATEUSEPARAM_JP, UserIdx, CharacterIdx, dwItemIdx, Param);
#elif defined _TL_LOCAL_
	sprintf(txt, "EXEC %s %d, %d, %d, %d", STORED_SHOPITEM_UPDATEUSEPARAM, UserIdx, CharacterIdx, dwItemIdx, Param);
#else
	sprintf(txt, "EXEC %s %d, %d, %d", STORED_SHOPITEM_UPDATEUSEPARAM, CharacterIdx, dwItemIdx, Param);
#endif
	g_DB.Query(eQueryType_FreeQuery, eItemShopItemUpdateUseParam, 0, txt);
}

void ShopItemUpdateUseInfoToDB( DWORD CharacterIdx, DWORD dwDBIdx, DWORD Param, DWORD RemainTime )
{
	sprintf(txt, "EXEC %s %d, %d, %d, %d", STORED_SHOPITEM_UPDATEUSEINFO, CharacterIdx, dwDBIdx, Param, RemainTime);
	g_DB.Query(eQueryType_FreeQuery, eItemShopItemUpdateUseInfo, 0, txt);
}

void ShopItemGetItem( DWORD CharacterIdx, WORD wItemIdx, DWORD Dur )
{
	sprintf(txt, "EXEC %s %d, %d, %d", STORED_SHOPITEM_GETITEM, CharacterIdx, wItemIdx, Dur);
	g_DB.Query(eQueryType_FreeQuery, eItemShopGetItem, 0, txt);
}

void ItemMoveShopUpdateToDB(DWORD CharacterIDX, DWORD UserIDX, DWORD dwfromDBIdx, POSTYPE frompos, DWORD dwtoDBIdx, POSTYPE topos)
{
	sprintf(txt, "EXEC %s %d, %d, %d, %d, %d, %d", STORED_ITEM_MOVEUPDATE_SHOP, CharacterIDX, UserIDX, dwfromDBIdx, frompos, dwtoDBIdx, topos);
	g_DB.Query(eQueryType_FreeQuery, eItemMoveShopUpdate, 0, txt);
}

void ShopItemUpdateCharacterIdx( DWORD CharacterIdx, DWORD dwDBIdx )
{
	sprintf(txt, "EXEC %s %d, %d", "MP_SHOPITEM_UpdateCharacterIdx", CharacterIdx, dwDBIdx);
	g_DB.Query(eQueryType_FreeQuery, eItemShopUpdatCharacterIdx, 0, txt);	
}

//

void CharacterAbilityInfo(DWORD CharacterIDX, DWORD Protocol)
{
	g_DB.FreeMiddleQuery(RCharacterAbilityInfo, Protocol, 
		"MP_CHARACTER_SelectAbility %d",CharacterIDX);
}

void CharacterPetInfo( DWORD CharacterIDX, DWORD UserID )
{
	sprintf(txt, "EXEC %s %d", "dbo.MP_CHARACTER_PetInfo", UserID);
	g_DB.Query(eQueryType_FreeQuery, eCharacterPetInfoQuery, CharacterIDX, txt);
}

//SW070127 鸥捞藕
void CharacterTitanInfo( DWORD CharacterIDX, DWORD UserID )
{
	sprintf(txt, "EXEC dbo.MP_CHARACTER_TitanInfo %d", UserID);
	g_DB.Query(eQueryType_FreeQuery, eCharacterTitanInfoQuery, CharacterIDX, txt);
}

void RCharacterTitanInfo(LPQUERY pData, LPDBMESSAGE pMessage)
{
	CPlayer* pPlayer = NULL;
	pPlayer = (CPlayer*)g_pUserTable->FindUser(pMessage->dwID);
	if(NULL == pPlayer)	return;

	TITAN_TOTALINFO TitanInfo;


	for( DWORD i = 0; i < pMessage->dwResult; ++i )
	{
		TitanInfo.TitanMasterUserID		= atoi((char*)pData[i].Data[0]);
		TitanInfo.TitanDBIdx			= atoi((char*)pData[i].Data[1]);
		TitanInfo.TitanCallItemDBIdx	= atoi((char*)pData[i].Data[2]);
		TitanInfo.TitanKind				= atoi((char*)pData[i].Data[3]);
		TitanInfo.TitanGrade			= atoi((char*)pData[i].Data[4]);
		TitanInfo.Fuel					= atoi((char*)pData[i].Data[5]);
		TitanInfo.Spell					= atoi((char*)pData[i].Data[6]);
		TitanInfo.RecallTime			= atoi((char*)pData[i].Data[7]);
		TitanInfo.RegisterCharacterIdx	= atoi((char*)pData[i].Data[8]);
		TitanInfo.bRiding				= atoi((char*)pData[i].Data[9]);
		TitanInfo.Scale					= atoi((char*)pData[i].Data[10]);
		TitanInfo.MaintainTime			= atoi((char*)pData[i].Data[11]);	// magi82(23)

		pPlayer->GetTitanManager()->AddTitanTotalInfo(&TitanInfo);
		if( TitanInfo.RecallTime )
			pPlayer->GetTitanManager()->SetRecallCheckTime( TitanInfo.RecallTime );

		if( TitanInfo.RegisterCharacterIdx == pMessage->dwID )
			pPlayer->GetTitanManager()->SetRegistTitanCallItemDBIdx(TitanInfo.TitanCallItemDBIdx);
	}

	CharacterTitanEquipItemEnduranceInfo(pMessage->dwID);

	//pPlayer->SetInitState(PLAYERINITSTATE_TITAN_INFO, MP_USERCONN_GAMEIN_SYN);
}

void CharacterTitanEquipItemEnduranceInfo( DWORD CharacterIDX )
{
	sprintf(txt, "EXEC dbo.MP_CHARACTER_TitanEquipItemEnduranceInfo %d", CharacterIDX);
	g_DB.Query(eQueryType_FreeQuery, eCharacterTitanEquipItemEnduranceInfoQuery, CharacterIDX, txt);
}

void RCharacterTitanEquipItemEnduranceInfo( LPQUERY pData, LPDBMESSAGE pMessage )
{
	CPlayer* pPlayer = NULL;
	pPlayer = (CPlayer*)g_pUserTable->FindUser(pMessage->dwID);
	if(NULL == pPlayer)	return;

	TITAN_ENDURANCE_ITEMINFO EnduranceInfo;

	EnduranceInfo.CharacterID = pMessage->dwID;

	for( DWORD i = 0; i < pMessage->dwResult; ++i )
	{
		EnduranceInfo.ItemDBIdx = atoi((char*)pData[i].Data[0]);
		EnduranceInfo.ItemIdx	= atoi((char*)pData[i].Data[1]);
		EnduranceInfo.Endurance = atoi((char*)pData[i].Data[2]);
		EnduranceInfo.UserIdx = atoi((char*)pData[i].Data[3]);

		pPlayer->GetTitanManager()->AddTitanEquipItemEnduranceInfo(&EnduranceInfo);
	}

	pPlayer->SetInitState(PLAYERINITSTATE_TITAN_INFO, MP_USERCONN_GAMEIN_SYN);
}


void AbilityLevelupLog(CPlayer* pPlayer,WORD AbilityIdx,LEVELTYPE AbilityLevel,EXPTYPE FromExp,EXPTYPE ToExp)
{
	sprintf(txt,
"INSERT TB_AbilityLog \
(character_idx,ability_idx,ability_level_to,ability_exp_from,ability_exp_to,logdate) \
values (%d, %d, %d, %u, %u, getdate())", 
pPlayer->GetID(), AbilityIdx, AbilityLevel, FromExp, ToExp);

	g_DB.LogQuery(eQueryType_FreeQuery, 0, 0, txt);
}

void SSItemInsert(DWORD CharacterIdx, WORD wItemIdx, DURTYPE Durability, POSTYPE bPosition, DWORD FromChrIdx, WORD bSeal,WORD bStatic,DWORD bGrow) // 和成长 2015-01-14
{
	sprintf(txt, "EXEC %s %d, %d, %d, %d, %d, %d, %d", STORED_ITEM_INSERT_INVEN, CharacterIdx, wItemIdx, Durability, bPosition, bSeal,bStatic,bGrow);

	g_DB.Query(eQueryType_FreeQuery, eSSItemInsert, FromChrIdx, txt);
}

void ItemUpdateToDB(DWORD CharacterIdx, DWORD dwDBIdx, WORD wItemIdx, DURTYPE Durability, POSTYPE bPosition, WORD qPosition, DWORD RareIdx)
{
	if( RareIdx < 0 )
	{
		//char temp[128];
		//sprintf(temp, "RareIdxError!(<0) ItemDBIdx : %d, PlayerID : %d", dwDBIdx, CharacterIdx );
		//ASSERTMSG(0, temp);

		RareIdx = 0;
	}

	sprintf(txt, "EXEC %s %d, %d, %d, %d, %d, %d, %d", STORED_ITEM_UPDATE, 
					CharacterIdx, dwDBIdx, wItemIdx, Durability, bPosition, qPosition, RareIdx );
	g_DB.Query(eQueryType_FreeQuery, eItemUpdate, 0, txt);
}

void ItemCombineUpdateToDB(DWORD dwfromDBIdx, DURTYPE fromDur, DWORD dwToDBIdx, DURTYPE toDur)
{
//	char txt[128];
	sprintf(txt, "EXEC %s %d, %d, %d, %d", STORED_ITEM_COMBINEUPDATE, dwfromDBIdx, fromDur, dwToDBIdx, toDur);
	g_DB.Query(eQueryType_FreeQuery, eItemCombineUpdate, 0, txt);
}

void ItemMoveUpdateToDB(DWORD CharacterIDX, DWORD dwfromDBIdx, POSTYPE frompos, DWORD dwtoDBIdx, POSTYPE topos)
{
//	char txt[128];
	sprintf(txt, "EXEC %s %d, %d, %d, %d, %d", STORED_ITEM_MOVEUPDATE_INVEN, dwfromDBIdx, frompos, dwtoDBIdx, topos, CharacterIDX);
	g_DB.Query(eQueryType_FreeQuery, eItemMoveUpdate, 0, txt);
}

void ItemMovePyogukUpdateToDB(DWORD CharacterIDX, DWORD UserIDX, DWORD dwfromDBIdx, POSTYPE frompos, DWORD dwtoDBIdx, POSTYPE topos)
{
//	char txt[128];
	sprintf(txt, "EXEC %s %d, %d, %d, %d, %d, %d", STORED_ITEM_MOVEUPDATE_PYOGUK, CharacterIDX, UserIDX, dwfromDBIdx, frompos, dwtoDBIdx, topos);
	g_DB.Query(eQueryType_FreeQuery, eItemMovePyogukUpdate, 0, txt);
}

void ItemMoveGuildUpdateToDB(DWORD CharacterIDX, DWORD MunpaIDX, DWORD dwfromDBIdx, POSTYPE frompos, DWORD dwtoDBIdx, POSTYPE topos)
{
//	char txt[128];
	sprintf(txt, "EXEC %s %d, %d, %d, %d, %d, %d", STORED_ITEM_MOVEUPDATE_GUILD, CharacterIDX, MunpaIDX, dwfromDBIdx, frompos, dwtoDBIdx, topos);
	g_DB.Query(eQueryType_FreeQuery, eItemMoveGuildUpdate, 0, txt);
}

void ItemMovePetInvenUpdateToDB( DWORD CharacterIDX, DWORD dwfromDBIdx, POSTYPE frompos, DWORD dwtoDBIdx, POSTYPE topos )
{
	sprintf(txt, "EXEC dbo.MP_ITEM_MoveUpdatePetInven %d, %d, %d, %d, %d", CharacterIDX, dwfromDBIdx, frompos, dwtoDBIdx, topos);
	g_DB.Query(eQueryType_FreeQuery, eItemMovePetInvenUpdate, 0, txt);
}

/* dwKeyā¨√: DB∷?∷㈢ io⒋I ㄏ┆O; ItemAI ARRAYAC ㄏ┆∷㈢ ItemAIAo∷?I ArrayIdx∷〓| ㄏ┆O∷⊥A∷⊥U.
   CIㄏ┆¨√AC ARRAYā∷㈢ DB∷?∷㈢ ∷〓ㄏ√⊥iI ∷〓¨u¨〓 ⊥iC¨uu;∷O】∷〓∷〓 ∷⊥U= AU¨uA; ¨uoCaCIa'C¨ǎ¨uㄏ CE∷?a
   0AI∷〓e ~⒋a ㄏ┆¨√∷〓OAo∷⊥A āe¨uO AㄏǎCa (āㄏo¨uo∷〓| CI¨oACIa⊥i⊥i CN∷⊥U.)
   LOWORD():EndValue, HIWORD(): ArrayIdx
*/
void ItemInsertToDB(DWORD CharacterIdx, WORD wItemIdx, DURTYPE Durability, POSTYPE bPosition, DWORD dwKey, WORD bSeal,WORD bStatic,DWORD dwGrow) // 2014-12-01  默认为0普通购买 成长
{
	// 2014-12-01 插入时
    //	char txt[128];
	sprintf(txt, "EXEC %s %d, %d, %d, %d, %d,%d,%d", STORED_ITEM_INSERT_INVEN, CharacterIdx, wItemIdx, Durability, bPosition, bSeal,bStatic,dwGrow);
	g_DB.Query(eQueryType_FreeQuery, eItemInsert, dwKey, txt);
}

// 2014-12-06 数据库函数实现
void ItemLockUpdate(DWORD dwCharacterIdx,DWORD wdItemDBIdx,WORD wParam)
{
	sprintf(txt, "EXEC %s %d, %d, %d", STORED_ITEM_UPDATE_LOCK, dwCharacterIdx,wdItemDBIdx,wParam);
	g_DB.Query(eQueryType_FreeQuery, eItemUpdateLock,0, txt);
}

// 2014-12-06 解锁数据库函数实现  使用同一个存储
void ItemUnLockUpdate(DWORD dwCharacterIdx,DWORD wdItemDBIdx,WORD wParam)
{
	sprintf(txt, "EXEC %s %d, %d, %d", STORED_ITEM_UPDATE_UNLOCK, dwCharacterIdx,wdItemDBIdx,wParam);
	g_DB.Query(eQueryType_FreeQuery, eItemUpdateUnLock,0, txt);
}

// 2015-01-15 成长系统数据库函数实现

void ItemGrowUpdate(DWORD dwCharacterIdx, DWORD dwItemDBIdx, DWORD dwGrow)
{
	sprintf(txt, "EXEC %s %d, %d, %d", STORED_ITEM_UPDATE_GROW, dwCharacterIdx,dwItemDBIdx,dwGrow);
	g_DB.Query(eQueryType_FreeQuery,eItemUpdateGrow,0, txt);
}

// 2015-01-28 赌博系统数据库访问
void GambleUserInfoInsert(DWORD dwCharacterIdx,int Year,int Month,int Day,int Number1,int Number2,int Number3)
{
    sprintf(txt, "EXEC %s %d,%d, %d, %d, %d,%d,%d",STORED_GAMBLE_USER_INFO_INSERT,dwCharacterIdx,Year,Month,Day,Number1,Number2,Number3);
	g_DB.Query(eQueryType_FreeQuery,eGambleUserInfoInsert,0, txt);
   
}

void ItemOptionInsertToDB(DWORD CharacterIdx, WORD wItemIdx, POSTYPE Pos, ITEM_OPTION_INFO * pOptionInfo)
{
//	char txt[256];
	sprintf(txt, "EXEC %s %d, %d, %d, %d, %d, %d, %d, %d, %d, %d,  %d, %d, %d, %d, %d,  %d, %d,  %d, %d, %d, %d, %d,  %d", STORED_ITEM_OPTION_INSERT,
		CharacterIdx, 
		wItemIdx,
		pOptionInfo->dwItemDBIdx,
		pOptionInfo->GenGol,					
		pOptionInfo->MinChub,				
		pOptionInfo->CheRyuk,				
		pOptionInfo->SimMek,				
		pOptionInfo->Life,					
		pOptionInfo->NaeRyuk,			
		pOptionInfo->Shield,				
		(int)(pOptionInfo->AttrRegist.GetElement_Val(ATTR_FIRE)*100+0.00001),
		(int)(pOptionInfo->AttrRegist.GetElement_Val(ATTR_WATER)*100+0.00001),
		(int)(pOptionInfo->AttrRegist.GetElement_Val(ATTR_TREE)*100+0.00001),
		(int)(pOptionInfo->AttrRegist.GetElement_Val(ATTR_IRON)*100+0.00001),
		(int)(pOptionInfo->AttrRegist.GetElement_Val(ATTR_EARTH)*100+0.00001),
		pOptionInfo->PhyAttack,
		pOptionInfo->CriticalPercent,
		(int)(pOptionInfo->AttrAttack.GetElement_Val(ATTR_FIRE)*100+0.00001),
		(int)(pOptionInfo->AttrAttack.GetElement_Val(ATTR_WATER)*100+0.00001),
		(int)(pOptionInfo->AttrAttack.GetElement_Val(ATTR_TREE)*100+0.00001),
		(int)(pOptionInfo->AttrAttack.GetElement_Val(ATTR_IRON)*100+0.00001),
		(int)(pOptionInfo->AttrAttack.GetElement_Val(ATTR_EARTH)*100+0.00001),
		pOptionInfo->PhyDefense);
	g_DB.Query(eQueryType_FreeQuery, eItemOptionInsert, Pos, txt);
	
	/*
	g_DB.FreeQuery(RItemOptionInsert,Pos,
		"EXEC %s %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %.2f, %.2f, %.2f, %.2f, %.2f, %d, %d, %.2f, %.2f, %.2f, %.2f, %.2f, %d", STORED_ITEM_OPTION_INSERT,
		CharacterIdx, 
		wItemIdx,
		pOptionInfo->dwItemDBIdx,
		pOptionInfo->GenGol,					
		pOptionInfo->MinChub,				
		pOptionInfo->CheRyuk,				
		pOptionInfo->SimMek,				
		pOptionInfo->Life,					
		pOptionInfo->NaeRyuk,			
		pOptionInfo->Shield,				
		pOptionInfo->AttrRegist.GetElement_Val(ATTR_FIRE),
		pOptionInfo->AttrRegist.GetElement_Val(ATTR_WATER),
		pOptionInfo->AttrRegist.GetElement_Val(ATTR_TREE),
		pOptionInfo->AttrRegist.GetElement_Val(ATTR_IRON),
		pOptionInfo->AttrRegist.GetElement_Val(ATTR_EARTH),
		pOptionInfo->PhyAttack,
		pOptionInfo->CriticalPercent,
		pOptionInfo->AttrAttack.GetElement_Val(ATTR_FIRE),
		pOptionInfo->AttrAttack.GetElement_Val(ATTR_WATER),
		pOptionInfo->AttrAttack.GetElement_Val(ATTR_TREE),
		pOptionInfo->AttrAttack.GetElement_Val(ATTR_IRON),
		pOptionInfo->AttrAttack.GetElement_Val(ATTR_EARTH),
		pOptionInfo->PhyDefense);
		*/
}
void ItemRareInsertToDB(DWORD CharacterIdx, WORD wItemIdx, POSTYPE bPosition, DWORD dwKey, ITEM_RARE_OPTION_INFO* pRareOptionInfo )
{
	char txt[1024];

	sprintf(txt, "EXEC %s %d, %d, %d,  %d, %d, %d, %d, %d, %d, %d,  %d, %d, %d, %d, %d,  %d,  %d, %d, %d, %d, %d,  %d", 
		"dbo.MP_ITEM_RARE_OPTION_Insert",//STORED_ITEM_RARE_OPTION_INSERT,
		CharacterIdx, 
		wItemIdx,
//		Durability,
		bPosition,
//		bSeal,

//		pRareOptionInfo->dwItemDBIdx,
		pRareOptionInfo->GenGol,					
		pRareOptionInfo->MinChub,				
		pRareOptionInfo->CheRyuk,				
		pRareOptionInfo->SimMek,				
		pRareOptionInfo->Life,					
		pRareOptionInfo->NaeRyuk,			
		pRareOptionInfo->Shield,				
		(int)(pRareOptionInfo->AttrRegist.GetElement_Val(ATTR_FIRE)*100+0.00001),
		(int)(pRareOptionInfo->AttrRegist.GetElement_Val(ATTR_WATER)*100+0.00001),
		(int)(pRareOptionInfo->AttrRegist.GetElement_Val(ATTR_TREE)*100+0.00001),
		(int)(pRareOptionInfo->AttrRegist.GetElement_Val(ATTR_IRON)*100+0.00001),
		(int)(pRareOptionInfo->AttrRegist.GetElement_Val(ATTR_EARTH)*100+0.00001),
		pRareOptionInfo->PhyAttack,
//		pRareOptionInfo->CriticalPercent,
		(int)(pRareOptionInfo->AttrAttack.GetElement_Val(ATTR_FIRE)*100+0.00001),
		(int)(pRareOptionInfo->AttrAttack.GetElement_Val(ATTR_WATER)*100+0.00001),
		(int)(pRareOptionInfo->AttrAttack.GetElement_Val(ATTR_TREE)*100+0.00001),
		(int)(pRareOptionInfo->AttrAttack.GetElement_Val(ATTR_IRON)*100+0.00001),
		(int)(pRareOptionInfo->AttrAttack.GetElement_Val(ATTR_EARTH)*100+0.00001),
		pRareOptionInfo->PhyDefense);

	g_DB.Query(eQueryType_FreeQuery, eItemRareInsert, dwKey, txt);
}

void ShopItemRareInsertToDB(DWORD CharacterIdx, DWORD ItemIdx, DWORD ItemPos, DWORD ItemDBIdx, ITEM_RARE_OPTION_INFO* pRareOptionInfo )
{
	char txt[1024];

	sprintf(txt, "EXEC %s %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d,  %d, %d, %d, %d, %d,  %d,  %d, %d, %d, %d, %d,  %d", 
		"dbo.MP_SHOPITEM_RARE_OPTION_Insert",//STORED_ITEM_RARE_OPTION_INSERT,
		CharacterIdx, ItemIdx, ItemPos,	ItemDBIdx,
		//		pRareOptionInfo->dwItemDBIdx,
		pRareOptionInfo->GenGol,					
		pRareOptionInfo->MinChub,				
		pRareOptionInfo->CheRyuk,				
		pRareOptionInfo->SimMek,				
		pRareOptionInfo->Life,					
		pRareOptionInfo->NaeRyuk,			
		pRareOptionInfo->Shield,				
		(int)(pRareOptionInfo->AttrRegist.GetElement_Val(ATTR_FIRE)*100+0.00001),
		(int)(pRareOptionInfo->AttrRegist.GetElement_Val(ATTR_WATER)*100+0.00001),
		(int)(pRareOptionInfo->AttrRegist.GetElement_Val(ATTR_TREE)*100+0.00001),
		(int)(pRareOptionInfo->AttrRegist.GetElement_Val(ATTR_IRON)*100+0.00001),
		(int)(pRareOptionInfo->AttrRegist.GetElement_Val(ATTR_EARTH)*100+0.00001),
		pRareOptionInfo->PhyAttack,
		//		pRareOptionInfo->CriticalPercent,
		(int)(pRareOptionInfo->AttrAttack.GetElement_Val(ATTR_FIRE)*100+0.00001),
		(int)(pRareOptionInfo->AttrAttack.GetElement_Val(ATTR_WATER)*100+0.00001),
		(int)(pRareOptionInfo->AttrAttack.GetElement_Val(ATTR_TREE)*100+0.00001),
		(int)(pRareOptionInfo->AttrAttack.GetElement_Val(ATTR_IRON)*100+0.00001),
		(int)(pRareOptionInfo->AttrAttack.GetElement_Val(ATTR_EARTH)*100+0.00001),
		pRareOptionInfo->PhyDefense);

	g_DB.Query(eQueryType_FreeQuery, eItemShopRareCreate, 0, txt);
}

// 2014-11-14 数据

void ItemStoneInsertToDB(DWORD CharacterIdx,DWORD wItemIdx, DWORD wItemDBIdx,DWORD wStoneValue)
{
   	char txt[1024];

	sprintf(txt, "EXEC %s %d,%d,%d,%d",STORED_ITEM_STONE_INSERT,CharacterIdx,wItemIdx,wItemDBIdx,wStoneValue);

	g_DB.Query(eQueryType_FreeQuery, eItemStoneInsert, 0, txt);
}
// 2014-11-14 数据

void ItemStoneUpdateToDB(DWORD CharacterIdx,int wItemPos, DWORD wItemStoneIdx,DWORD wItemXqIdx0,DWORD wItemXqIdx1,DWORD wItemXqIdx2,DWORD wItemXqIdx3,DWORD wItemXqIdx4)
{
   	char txt[1024];

	sprintf(txt, "EXEC %s %d,%d,%d,%d,%d,%d,%d,%d",STORED_ITME_STONE_UPDATE,CharacterIdx,wItemPos,wItemStoneIdx,wItemXqIdx0,wItemXqIdx1,wItemXqIdx2,wItemXqIdx3,wItemXqIdx4);

	g_DB.Query(eQueryType_FreeQuery, eItemStoneUpdate, 0, txt);
}

void IsUseAbleShopAvatarItem(DWORD CharacterIdx, DWORD ItemDBIdx, DWORD ItemIdx, DWORD ItemPos )
{
	sprintf(txt, "EXEC %s %d, %d, %d, %d", STORED_SHOPITEM_AVATARABLE, CharacterIdx, ItemDBIdx, ItemIdx, ItemPos );
	g_DB.Query(eQueryType_FreeQuery, eItemShopIsUseableAvatar, CharacterIdx, txt);
}

void IsAbleShopEquipItem( DWORD CharacterIdx, DWORD ItemDBIdx, DWORD FromItemIdx, DWORD FromPos, DWORD ToItemIdx, DWORD ToPos )
{
	sprintf(txt, "EXEC %s %d, %d, %d, %d, %d, %d", STORED_SHOPITEM_EQUIPABLE, CharacterIdx, ItemDBIdx, 
		FromItemIdx, FromPos, ToItemIdx, ToPos );
	g_DB.Query(eQueryType_FreeQuery, eItemShopIsUseableEquip, CharacterIdx, txt);
}


void ShopItemAllUseInfoUpdateToDB( DWORD ItemDBIdx, DWORD CharacterIdx, WORD ItemIdx, DWORD Param, DWORD Remaintime )
{
	sprintf(txt, "EXEC %s %d, %d, %d, %d, %d", STORED_SHOPITEM_UPDAETALL, ItemDBIdx, CharacterIdx, ItemIdx, 
		Param, Remaintime );
	g_DB.Query(eQueryType_FreeQuery, eItemShopUpdateAllInfo, CharacterIdx, txt);
}


void ItemDeleteToDB(DWORD dwDBIdx)
{
//	char txt[128];
	sprintf(txt, "EXEC %s %d", STORED_ITEM_DELETE, dwDBIdx);
	g_DB.Query(eQueryType_FreeQuery, eItemDelete, 0, txt);
}

void MugongUpdateToDB(MUGONGBASE* msg, char* type)
{
//	char txt[128];
	ASSERT(msg->Position);
	sprintf(txt, "EXEC %s %d, %d, %d, %d, %d, %d, %d, %d", STORED_MUGONG_UPDATE, 
		msg->dwDBIdx, msg->wIconIdx, msg->ExpPoint, msg->Sung, msg->Position, msg->QuickPosition, msg->bWear, msg->OptionIndex);
	g_DB.Query(eQueryType_FreeQuery, eMugongUpdate2, 0, txt);
	
	//诀单捞飘 沁澜
	msg->bWear = FALSE;
}

void MugongMoveUpdateToDB(DWORD dwfromDBIdx, POSTYPE frompos, DWORD dwtoDBIdx, POSTYPE topos, char* type)
{
//	char txt[128];
	sprintf(txt, "EXEC %s %d, %d, %d, %d", STORED_MUGONG_MOVEUPDATE, dwfromDBIdx, frompos, dwtoDBIdx, topos);
	g_DB.Query(eQueryType_FreeQuery, eMugongMoveUpdate2, 0, txt);
/*
	 frompos == 0 || topos == 0 )
	{
		char temp[256];
		FILE* fp = fopen("MGMoveUpdateToDB.txt","a+");
		sprintf(temp, "%s : [Fdbidx]%d, [Fpos]%d, [Tdbidx]%d, [Tpos]%d\n", type, dwfromDBIdx, frompos, dwtoDBIdx, topos);
		fprintf(fp,temp);
		fclose(fp);
	}
*/
}

void MugongInsertToDB(DWORD CharacterIDX, WORD MugongIDX, POSTYPE MugongPos, BYTE bWeared, BYTE bSung, WORD Option)
{
//	char txt[128];
	ASSERT(MugongPos);
	sprintf(txt, "EXEC %s %u, %d, %d, %d, %d, %d", STORED_MUGONG_INSERT, CharacterIDX, MugongIDX, MugongPos, bWeared, bSung, Option);
	g_DB.Query(eQueryType_FreeQuery, eMugongInsert, CharacterIDX, txt);
}

void MugongDeleteToDB(DWORD dwDBIdx)
{
//	char txt[128];
	sprintf(txt, "EXEC %s %d", STORED_MUGONG_DELETE, dwDBIdx);
	g_DB.Query(eQueryType_FreeQuery, eMugongDelete, 0, txt);
}

void MapBaseEconomy(BYTE MapNum)
{
//	char txt[128];
	sprintf(txt, "EXEC %s %d", STORED_CHARACTER_BASEECONOMY, MapNum);
	g_DB.Query(eQueryType_FreeQuery, eBaseEconomy, 0, txt);
}

void AuctionRegist(BYTE ItemIdx, DWORD SellerID, DWORD EndDate, DWORD EndTime, DWORD StartPrice, DWORD ImmediatePrice, char Memo[256])
{

   // 2014-06-25 check string param !

	if( !isValidQueryString( Memo ))
	{
	   return;
	}

	if( CheckString( Memo ) )
	{
		return;
	}

	sprintf(txt, "EXEC %s %d, %d, %d, %d, %d, %d, %s", STORED_CHARACTER_AUCTIONREGIST, 
		ItemIdx, SellerID, EndDate, EndTime, StartPrice, ImmediatePrice, Memo);
	g_DB.Query(eQueryType_FreeQuery, eAuctionRegist, SellerID, txt);
}

void AuctionPageQuery(DWORD CharacterIDX, BYTE PageNum)
{


}
/*
void CharacterItemGetHuntIDX(DWORD CharacterIDX, DWORD OwnerID, WORD ItemIDX, DURTYPE Durability, POSTYPE bPosition, BYTE TableIDX)
{
	char txt[128];
	sprintf(txt, "EXEC %s %d, %d, %d, %d, %d, %d", STORED_CHARACTER_ITEMHUNT, OwnerID, ItemIDX, bPosition, 0,Durability, TableIDX );
	g_DB.Query(eQueryType_FreeQuery, eItemHunt, CharacterIDX, txt);
	{
	}
}
void CharacterDivideNewItemIDX(DWORD CharacterIDX, DWORD OwnerID, WORD ItemIDX, DURTYPE Durability, POSTYPE bPosition, BYTE TableIDX)
{
	char txt[128];
	sprintf(txt, "EXEC %s %d, %d, %d, %d, %d, %d", STORED_CHARACTER_ITEMHUNT, OwnerID, ItemIDX, bPosition, 0,Durability, TableIDX );
	g_DB.Query(eQueryType_FreeQuery, eItemDivideNewItem, CharacterIDX, txt);
	{
	}
}
*/


// --------------------------------------------------------------------------------------------------
// taiyo code
// CharacterDupItemInsertAC wKey∷⊥A RCharacterDupItemInsert∷?∷㈢¨uㄏ EIAaAU∷〓| ∷〓¨〓¨∷CIa 'CN ∷〓¨〓¨∷AUAI∷⊥U
// 0āu 0AI∷?UAC ā¨√; ic∷?eCI∷⊥A BOOLā¨√AC CuAA∷〓| AeCN∷⊥U
/*
void CharacterDupItemInsert( DWORD dwKey, DWORD CharacterIDX, WORD ItemIDX, DURTYPE Durability, POSTYPE bPosition, BYTE TableIDX )
{
	char txt[128];
	sprintf( txt, "EXEC %s %d, %d, %d, %d, %d, %d", STORED_CHARACTER_ITEMHUNT, CharacterIDX, ItemIDX, bPosition, 0, Durability, TableIDX );
	g_DB.Query(eQueryType_FreeQuery, eItemDupInsert, dwKey, txt);
	{
	}
}
*/

void RegistLoginMapInfo(DWORD CharacterIDX, char* CharacterName, BYTE MapNum, WORD MapPort)
{
   // 2014-06-25 check string param !

	if( !isValidQueryString( CharacterName ))
	{
	   return;
	}

	if( CheckString( CharacterName ) )
	{
		return;
	}

	sprintf(txt, "EXEC %s %d, \'%s\', %d, %d", STORED_CHARACTER_REGISTLOGINMAPINFO, CharacterIDX, CharacterName, MapNum, MapPort);
	g_DB.Query(eQueryType_FreeQuery, eRegistLoginMapInfo, 0, txt);
}

void UnRegistLoginMapInfo(DWORD CharacterIDX)
{
//	char txt[128];
	sprintf(txt, "EXEC %s %d", STORED_CHARACTER_UNREGISTLOGINMAPINFO, CharacterIDX);
	g_DB.Query(eQueryType_FreeQuery, eUnRegistLoginMapInfo, 0, txt);
}

//////////////////////////////////////////////////////////////////////////
// ¨A¨¨u āu⒋A DBAo∷〓∷c
void PartyLoad(DWORD PartyIDX)
{
//	char txt[128];
	sprintf(txt, "EXEC %s %d ", STORED_PARTY_REGIST_MAPSERVER, PartyIDX);
	g_DB.Query(eQueryType_FreeQuery, ePartyRegistMapServer, 0, txt);
}

void PartyBreakup(DWORD PartyIDX) 
{
//	char txt[128];
	sprintf(txt, "EXEC %s %d", STORED_PARTY_BREAK, PartyIDX);
	g_DB.Query(eQueryType_FreeQuery, ePartyBreakup, PartyIDX, txt);
}

//2008. 5. 21. CBH - 规颇 积己 可记 眠啊 包访 窃荐 荐沥
void PartyInfoByUserLogin(DWORD PartyIDX, DWORD CharacterIDX) 
{
	//	char txt[128];
	sprintf(txt, "%d", PartyIDX);	

	g_DB.FreeMiddleQuery( RPartyInfoByUserLogin, CharacterIDX, 
		"EXEC %s %s", STORED_PARTY_INFO_BY_USERLOGIN, txt );	
}

void PartyCreate(DWORD MasterIDX, PARTY_ADDOPTION* pAddOption) 
{
	LEVELTYPE wMinLevel = pAddOption->wMinLevel;
	LEVELTYPE wMaxLevel = pAddOption->wMaxLevel;
	BYTE bPublic = pAddOption->bPublic;
	BYTE bOption = pAddOption->bOption;
	WORD wLimitCount = pAddOption->wLimitCount;
	char* szTheme = pAddOption->szTheme;

   // 2014-06-25 check string param !

	if( !isValidQueryString(szTheme ))
	{
	   return;
	}

	if( CheckString( szTheme ) )
	{
		return;
	}

	//	char txt[128];
	sprintf(txt, "%d, %d, %d, %d, %d, %d, '%s'", MasterIDX, bOption, 
		wMinLevel, wMaxLevel, bPublic, wLimitCount, szTheme);

	g_DB.FreeMiddleQuery( RPartyCreate, 0, "EXEC %s %s", STORED_PARTY_CREATE, txt );	
}


void GuildLoadGuild(DWORD StartGuildIdx)
{
	sprintf(txt, "EXEC %s %u", STORED_GUILD_LOADGUILD, StartGuildIdx);
	g_DB.Query(eQueryType_FreeQuery, eGuildLoadGuild, 0, txt);
}

// 06. 03. 巩颇傍瘤 - 捞康霖
void GuildLoadNotice(DWORD StartDBIdx)
{
//	sprintf(txt, "EXEC %s %u", STORED_GUILD_LOADNOTICE, StartDBIdx);
//	g_DB.Query(eQueryType_FreeQuery, , 0, txt);

	g_DB.FreeMiddleQuery( RGuildLoadNotice, eGuildLoadNotice,
		"EXEC %s %u", STORED_GUILD_LOADNOTICE, StartDBIdx );
}

void GuildUpdateNotice(DWORD GuildIdx, char* Notice)
{
    // 2014-06-25 check string param !

	if( !isValidQueryString(Notice ))
	{
	   return;
	}

	if( CheckString( Notice ) )
	{
		return;
	}

	sprintf(txt, "EXEC %s %d, \'%s\'", STORED_GUILD_UPDATENOTICE, GuildIdx, Notice);
	g_DB.Query(eQueryType_FreeQuery, eGuildUpdateNotice, 0, txt);
}

void GuildLoadMember(DWORD StartCharacterIdx)
{
	sprintf(txt, "EXEC %s %u", STORED_GUILD_LOADMEMBER, StartCharacterIdx);
	g_DB.Query(eQueryType_FreeQuery, eGuildLoadMember, 0, txt);
}

void GuildLoadMark(DWORD StartMarkName)
{
	char txt[128];
	sprintf(txt, "EXEC %s %u", STORED_GUILD_LOADMARK, StartMarkName);
	g_DB.FreeLargeQuery(RGuildLoadMark, 0, txt);
}

void GuildCreate(DWORD CharacterIdx, char* CharacterName, LEVELTYPE CharacterLvl, char* GuildName, char* Intro)
{

	// 2014-06-25 check string param !

	if( !isValidQueryString(CharacterName)|| !isValidQueryString(GuildName) || !isValidQueryString( Intro))
	{
	   return;
	}

	if( CheckString( CharacterName ) || CheckString( GuildName ) ||CheckString( Intro ) )
	{
		return;
	}


	sprintf(txt, "EXEC %s %u, \'%s\', %d, \'%s\', \'%s\', %d, %d", STORED_GUILD_CREATE, CharacterIdx, CharacterName, CharacterLvl, GuildName, Intro, GAMERESRCMNGR->GetLoadMapNum(), GUILD_1LEVEL);
	g_DB.Query(eQueryType_FreeQuery, eGuildCreate, CharacterIdx, txt);
}

void GuildBreakUp(DWORD GuildIdx, DWORD MasterIdx)
{
	sprintf(txt, "EXEC %s %d", STORED_GUILD_BREAKUP, GuildIdx);
	g_DB.Query(eQueryType_FreeQuery, eGuildBreakUp, MasterIdx, txt);
}

void GuildDeleteMember(DWORD GuildIdx, DWORD MemberIDX)
{
	sprintf(txt, "EXEC %s %d, %d", STORED_GUILD_DELETEMEMBER, GuildIdx, MemberIDX);
	g_DB.Query(eQueryType_FreeQuery, eGuildDeleteMember, GuildIdx, txt);
}

void GuildSecedeMember(DWORD GuildIdx, DWORD PlayerIdx)
{
	sprintf(txt, "EXEC %s %d, %d", STORED_GUILD_DELETEMEMBER, GuildIdx, PlayerIdx);
	g_DB.Query(eQueryType_FreeQuery, eGuildSecedeMember, GuildIdx, txt);
}

void GuildAddMember(DWORD GuildIdx, DWORD MemberIDX, char* GuildName, BYTE Rank)
{
	 // 2014-06-25 check string param !

	if( !isValidQueryString(GuildName ))
	{
	   return;
	}

	if( CheckString( GuildName ) )
	{
		return;
	}
	sprintf(txt, "EXEC %s %u, %u, \'%s\', %d", STORED_GUILD_ADDMEMBER, GuildIdx, MemberIDX, GuildName, Rank);
	g_DB.Query(eQueryType_FreeQuery, eGuildAddMember, 0, txt);
}

void GuildMarkRegist(DWORD GuildIdx, char* ImgData, DWORD CharacterIdx)
{
	if( !isValidQueryString(ImgData ))
	{
	   return;
	}

	if( CheckString( ImgData ) )
	{
		return;
	}

	char buf[1024];
	sprintf( buf, "EXEC %s %d, 0x", STORED_GUILD_MARKREGIST,GuildIdx);

	int curpos = strlen(buf);
	for(int n=0;n<GUILDMARK_BUFSIZE;++n)
	{
		sprintf(&buf[curpos],"%02x",(BYTE)ImgData[n]);
		curpos += 2;
	}

	g_DB.FreeLargeQuery(RGuildMarkRegist, CharacterIdx,	
		buf);
}

void GuildLevelUp(DWORD GuildIdx, BYTE Level)
{
	sprintf(txt, "EXEC %s %u, %d", STORED_GUILD_LEVELUP, GuildIdx, Level);
	g_DB.Query(eQueryType_FreeQuery, eGuildLevelUp, 0, txt);
}

void GuildChangeMemberRank(DWORD GuildIdx, DWORD MemberIdx, BYTE Rank)
{
	sprintf(txt, "EXEC %s %u, %u, %d", STORED_GUILD_CHANGRANK, GuildIdx, MemberIdx, Rank);
	g_DB.Query(eQueryType_FreeQuery, eGuildLevelUp, 0, txt);
}

void GuildItemLoad(DWORD MapNum, DWORD StartItemDBIDx)
{
	sprintf(txt, "EXEC %s %d, %u", STORED_GUILD_LOADITEM, MapNum, StartItemDBIDx);
	g_DB.Query(eQueryType_FreeQuery, eGuildItemLoad, 0, txt);
}

void GuildItemOption(DWORD MapNum, DWORD StartDBIdx)
{
	sprintf(txt, "EXEC %s %d, %u", STORED_GUILD_ITEMOPTION, MapNum, StartDBIdx);
	g_DB.Query(eQueryType_FreeQuery, eGuildItemOption, 0, txt);
}

//SW050920 Rare
void GuildItemRareOption(DWORD MapNum, DWORD StartDBIdx)
{
	sprintf(txt, "EXEC %s %d, %u", "dbo.MP_GUILD_ItemRareOption_Info", MapNum, StartDBIdx);
	g_DB.Query(eQueryType_FreeQuery, eGuildItemRareOption, 0, txt);
}

void GuildItemLoadInNeed( DWORD MapNum, DWORD GuildIdx )
{
	sprintf(txt, "EXEC dbo.MP_GUILD_LOADITEM_INNEED %d, %d", MapNum, GuildIdx);
	g_DB.Query(eQueryType_FreeQuery, eGuildItemLoadInNeed, GuildIdx, txt);
}

//沥评厘丛 瘤矫 "贸府窃荐 嘿咯初阑巴~"
void RGuildItemLoadInNeed( LPQUERY pData, LPDBMESSAGE pMessage )
{
	ITEMBASE guilditem;
	DWORD GuildIdx = pMessage->dwID;

	if( 0 == pMessage->dwResult )	//巩颇霉 积己捞芭唱 后芒绊老 版快.
	{
		GUILDMGR->SetGuildItemInfoInited(GuildIdx, TRUE);

		GUILDMGR->SendGuildItemInfoToAllWarehouseMember(GuildIdx);

		return;
	}

	for(DWORD i=0; i<pMessage->dwResult; ++i)
	{
		GuildIdx = atoi((char*)pData[i].Data[eMu_IMunpaID]);

		guilditem.Durability = atoi((char*)pData[i].Data[eMu_IDurability]);
		guilditem.wIconIdx = atoi((char*)pData[i].Data[eMu_IIDX]);
		guilditem.dwDBIdx = atoi((char*)pData[i].Data[eMu_IDBIDX]);
		guilditem.Position = atoi((char*)pData[i].Data[eMu_IPosition]);
		guilditem.RareIdx = atoi((char*)pData[i].Data[eMu_IRareIdx]);
		guilditem.QuickPosition = 0;
		guilditem.ItemParam = 0;	// magi82(28)
		guilditem.StoneIdx=atoi((char*)pData[i].Data[eMu_IStoneIdx]); //weiye 门派仓库，，成长数据附加! 2015-10-25
		guilditem.ItemStatic=atoi((char*)pData[i].Data[eMu_IStatic]);
		guilditem.ItemGrow=atoi((char*)pData[i].Data[eMu_IGrow]);
		guilditem.Grade30 = atoi((char*)pData[i].Data[eMu_Grade30]);
		GUILDMGR->RegistGuildItem(GuildIdx, &guilditem);
	}
	if(pMessage->dwResult < MAX_QUERY_RESULT)
	{
		GuildItemOptionInNeed(GAMERESRCMNGR->GetLoadMapNum(), GuildIdx, 0);
		//SW050920 Rare
		//GuildItemRareOptionInNeed(GAMERESRCMNGR->GetLoadMapNum(), GuildIdx, 0);
	}	
	else
	{
		// SLOT_GUILDWAREHOUSE_NUM < MAX_QUERY_RESULT 捞镑俊 甸绢棵 老 绝促.
		GuildItemLoad(GAMERESRCMNGR->GetLoadMapNum(), guilditem.dwDBIdx);
	}
}

void GuildItemOptionInNeed( DWORD MapNum, DWORD GuildIdx, DWORD StartDBIdx )
{
	sprintf(txt, "EXEC dbo.MP_GUILD_LOADITEMOPTION_INNEED %d, %d, %d", MapNum, GuildIdx, StartDBIdx);
	g_DB.Query(eQueryType_FreeQuery, eGuildItemOptionInNeed, 0, txt);
}

void RGuildItemOptionInNeed( LPQUERY pData, LPDBMESSAGE pMessage )
{
	ITEM_OPTION_INFO OptionInfo;
	DWORD GuildIdx;

	for(DWORD i=0; i<pMessage->dwResult; ++i)
	{
		GuildIdx = atoi((char*)pData[i].Data[eGIOI_GuildIdx]);

		OptionInfo.dwOptionIdx				= (DWORD)atoi((char*)pData[i].Data[eGIOI_OptionID]);
		OptionInfo.dwItemDBIdx				= (DWORD)atoi((char*)pData[i].Data[eGIOI_ItemDBID]);
		OptionInfo.GenGol					= (WORD)atoi((char*)pData[i].Data[eGIOI_GenGol]);
		OptionInfo.MinChub					= (WORD)atoi((char*)pData[i].Data[eGIOI_MinChub]);				
		OptionInfo.CheRyuk					= (WORD)atoi((char*)pData[i].Data[eGIOI_CheRyuk]);				
		OptionInfo.SimMek					= (WORD)atoi((char*)pData[i].Data[eGIOI_SimMek]);					
		OptionInfo.Life						= (DWORD)atoi((char*)pData[i].Data[eGIOI_Life]);					
		OptionInfo.NaeRyuk					= (WORD)atoi((char*)pData[i].Data[eGIOI_NaeRyuk]);				
		OptionInfo.Shield					= (DWORD)atoi((char*)pData[i].Data[eGIOI_Shield]);					
		OptionInfo.AttrRegist.SetElement_Val(ATTR_FIRE,		(float)atoi((char*)pData[i].Data[eGIOI_FireAttrRegist])/100.f );
		OptionInfo.AttrRegist.SetElement_Val(ATTR_WATER,	(float)atoi((char*)pData[i].Data[eGIOI_WaterAttrRegist])/100.f );
		OptionInfo.AttrRegist.SetElement_Val(ATTR_TREE,		(float)atoi((char*)pData[i].Data[eGIOI_TreeAttrRegist])/100.f );
		OptionInfo.AttrRegist.SetElement_Val(ATTR_IRON,		(float)atoi((char*)pData[i].Data[eGIOI_GoldAttrRegist])/100.f );
		OptionInfo.AttrRegist.SetElement_Val(ATTR_EARTH,	(float)atoi((char*)pData[i].Data[eGIOI_EarthAttrRegist])/100.f );
		OptionInfo.PhyAttack				= (WORD)atoi((char*)pData[i].Data[eGIOI_PhyAttack]);
		OptionInfo.CriticalPercent			= (WORD)atoi((char*)pData[i].Data[eGIOI_Critical]);
		OptionInfo.AttrAttack.SetElement_Val(ATTR_FIRE,		(float)atoi((char*)pData[i].Data[eGIOI_FireAttrAttack])/100.f );
		OptionInfo.AttrAttack.SetElement_Val(ATTR_WATER,	(float)atoi((char*)pData[i].Data[eGIOI_WaterAttrAttack])/100.f );
		OptionInfo.AttrAttack.SetElement_Val(ATTR_TREE,		(float)atoi((char*)pData[i].Data[eGIOI_TreeAttrAttack])/100.f );
		OptionInfo.AttrAttack.SetElement_Val(ATTR_IRON,		(float)atoi((char*)pData[i].Data[eGIOI_GoldAttrAttack])/100.f );
		OptionInfo.AttrAttack.SetElement_Val(ATTR_EARTH,	(float)atoi((char*)pData[i].Data[eGIOI_EarthAttrAttack])/100.f );
		OptionInfo.PhyDefense				= (WORD)atoi((char*)pData[i].Data[eGIOI_PhyDefense]);				
		
		GUILDMGR->AddGuildItemOption(&OptionInfo);
	}

	if(pMessage->dwResult >= MAX_QUERY_RESULT)
	{
		GuildItemOptionInNeed(GAMERESRCMNGR->GetLoadMapNum(), GuildIdx, OptionInfo.dwOptionIdx);
	}
	else
	{
		GuildItemRareOptionInNeed(GAMERESRCMNGR->GetLoadMapNum(), GuildIdx, 0);
	}
}

void GuildItemRareOptionInNeed( DWORD MapNum, DWORD GuildIdx, DWORD StartDBIdx )
{
	sprintf(txt, "EXEC dbo.MP_GUILD_LOADITEMRAREOPTION_INNEED %d, %d, %d", MapNum, GuildIdx, StartDBIdx);
	g_DB.Query(eQueryType_FreeQuery, eGuildItemRareOptionInNeed, 0, txt);
}

void RGuildItemRareOptionInNeed( LPQUERY pData, LPDBMESSAGE pMessage )
{
	ITEM_RARE_OPTION_INFO RareOptionInfo;
	DWORD GuildIdx;

	for( DWORD i = 0; i < pMessage->dwResult; ++i )
	{
		GuildIdx = atoi((char*)pData[i].Data[eGIROI_GuildIdx]);

		RareOptionInfo.dwRareOptionIdx				= (DWORD)atoi((char*)pData[i].Data[eGIROI_RareOptionID]);
		RareOptionInfo.dwItemDBIdx				= (DWORD)atoi((char*)pData[i].Data[eGIROI_ItemDBID]);
		RareOptionInfo.GenGol					= (WORD)atoi((char*)pData[i].Data[eGIROI_GenGol]);
		RareOptionInfo.MinChub					= (WORD)atoi((char*)pData[i].Data[eGIROI_MinChub]);				
		RareOptionInfo.CheRyuk					= (WORD)atoi((char*)pData[i].Data[eGIROI_CheRyuk]);				
		RareOptionInfo.SimMek					= (WORD)atoi((char*)pData[i].Data[eGIROI_SimMek]);					
		RareOptionInfo.Life						= (DWORD)atoi((char*)pData[i].Data[eGIROI_Life]);					
		RareOptionInfo.NaeRyuk					= (WORD)atoi((char*)pData[i].Data[eGIROI_NaeRyuk]);				
		RareOptionInfo.Shield					= (DWORD)atoi((char*)pData[i].Data[eGIROI_Shield]);					
		RareOptionInfo.AttrRegist.SetElement_Val(ATTR_FIRE,		(float)atoi((char*)pData[i].Data[eGIROI_FireAttrRegist])/100.f );
		RareOptionInfo.AttrRegist.SetElement_Val(ATTR_WATER,	(float)atoi((char*)pData[i].Data[eGIROI_WaterAttrRegist])/100.f );
		RareOptionInfo.AttrRegist.SetElement_Val(ATTR_TREE,		(float)atoi((char*)pData[i].Data[eGIROI_TreeAttrRegist])/100.f );
		RareOptionInfo.AttrRegist.SetElement_Val(ATTR_IRON,		(float)atoi((char*)pData[i].Data[eGIROI_GoldAttrRegist])/100.f );
		RareOptionInfo.AttrRegist.SetElement_Val(ATTR_EARTH,	(float)atoi((char*)pData[i].Data[eGIROI_EarthAttrRegist])/100.f );
		RareOptionInfo.PhyAttack				= (WORD)atoi((char*)pData[i].Data[eGIROI_PhyAttack]);
//		RareOptionInfo.CriticalPercent			= (WORD)atoi((char*)pData[i].Data[eGIROI_Critical]);
		RareOptionInfo.AttrAttack.SetElement_Val(ATTR_FIRE,		(float)atoi((char*)pData[i].Data[eGIROI_FireAttrAttack])/100.f );
		RareOptionInfo.AttrAttack.SetElement_Val(ATTR_WATER,	(float)atoi((char*)pData[i].Data[eGIROI_WaterAttrAttack])/100.f );
		RareOptionInfo.AttrAttack.SetElement_Val(ATTR_TREE,		(float)atoi((char*)pData[i].Data[eGIROI_TreeAttrAttack])/100.f );
		RareOptionInfo.AttrAttack.SetElement_Val(ATTR_IRON,		(float)atoi((char*)pData[i].Data[eGIROI_GoldAttrAttack])/100.f );
		RareOptionInfo.AttrAttack.SetElement_Val(ATTR_EARTH,	(float)atoi((char*)pData[i].Data[eGIROI_EarthAttrAttack])/100.f );
		RareOptionInfo.PhyDefense				= (WORD)atoi((char*)pData[i].Data[eGIROI_PhyDefense]);
		
		GUILDMGR->AddGuildItemRareOption(&RareOptionInfo);
	}

	if(pMessage->dwResult >= MAX_QUERY_RESULT)
	{
		GuildItemRareOptionInNeed(GAMERESRCMNGR->GetLoadMapNum(), GuildIdx, RareOptionInfo.dwRareOptionIdx);
	}
	else
	{
		GUILDMGR->SetGuildItemInfoInited(GuildIdx, TRUE);

		GUILDMGR->SendGuildItemInfoToAllWarehouseMember(GuildIdx);
	}
}

//SW060719 巩颇器牢飘
void GuildLoadGuildPointInfo( DWORD startGuildDBIdx )
{
	sprintf(txt, "EXEC dbo.MP_GUILD_LOAD_GUILDPOINTINFO %d", startGuildDBIdx);
	g_DB.Query(eQueryType_FreeQuery, eGuildPointInfoLoad, 0, txt);
}

void RGuildLoadGuildPointInfo( LPQUERY pData, LPDBMESSAGE pMessage )
{
	//if( !pMessage->dwResult )	return;

	GUILDPOINT_INFO GuildPointInfo;
	
	DWORD GuildDBIdx = 0;
	DWORD GuildIdx = 0;
	//if(0 == pMessage->dwResult)	return;

	for( DWORD i = 0; i < pMessage->dwResult; ++i )
	{
		GuildDBIdx = atoi((char*)pData[i].Data[eGPII_GuildDBIdx]);
		GuildIdx = atoi((char*)pData[i].Data[eGPII_GuildIdx]);
		GuildPointInfo.GuildPoint = atoi((char*)pData[i].Data[eGPII_GuildPoint]);
		//GuildPointInfo.GuildHuntedMonsterCount = 0;
		GuildPointInfo.GuildHuntedMonsterTotalCount = atoi((char*)pData[i].Data[eGPII_GuildHuntedMonsterTotalCount]);
		//GuildPointInfo.DBProcessTime = 0;
		//GuildPointInfo.GuildPlusTimeflg = 0;
		//蝶肺..
		//for( int plustimeKind = 0; plustimeKind < eGPT_Kind_Max; ++plustimeKind )
		//{
		//	GuildPointInfo.GuildUsingPlusTimeInfo[plustimeKind].PlusTimeEndTime = atoi((char*)pData[i].Data[eGPII_EndTimePerGuildPlustimeKind + plustimeKind]);
		//}

		GUILDMGR->InitGuildPointInfo(GuildIdx, &GuildPointInfo);
	}

	if( pMessage->dwResult >= MAX_QUERY_RESULT )
	{
		GuildLoadGuildPointInfo(GuildDBIdx);
	}
	else
	{
		GuildLoadGuildPlustimeInfo( 0 );
	}
}

void GuildLoadGuildPlustimeInfo( DWORD startDBIdx )
{
	sprintf(txt, "EXEC dbo.MP_GUILD_LOAD_GUILDPLUSTIMEINFO %d", startDBIdx);
	g_DB.Query(eQueryType_FreeQuery, eGuildPlustimeInfoLoad, 0, txt);
}

void RGuildLoadGuildPlustimeInfo( LPQUERY pData, LPDBMESSAGE pMessage )
{
	//if( !pMessage->dwResult )	return;
	
	DWORD GuildPlustimeDBIdx = 0;
	DWORD GuildIdx = 0;
	DWORD GuildUsePlustimeIdx = 0;
	DWORD GuildUsePlustimeEndtime = 0;

	for( DWORD i = 0; i < pMessage->dwResult; ++i )
	{
		GuildPlustimeDBIdx = atoi((char*)pData[i].Data[0]);
		GuildIdx = atoi((char*)pData[i].Data[1]);
		GuildUsePlustimeIdx = atoi((char*)pData[i].Data[2]);
		GuildUsePlustimeEndtime = atoi((char*)pData[i].Data[3]);

		GUILDMGR->InitGuildPlustimeInfo(GuildIdx, GuildUsePlustimeIdx, GuildUsePlustimeEndtime);
	}

	if( pMessage->dwResult >= MAX_QUERY_RESULT )
	{
		GuildLoadGuildPlustimeInfo(GuildPlustimeDBIdx);
	}
	else
	{
		//RSiegeWarProfitInfoLoad(.. 俊辑 捞悼
		ItemLimitInfoLoadAll( 0 );
		// 夸货傈
        FortWarInfoLoad();
		FORTWARMGR->TotalFortWarItemLoad();
		//

		g_pServerSystem->SetStart( TRUE );
	}
}

void GuildAddHuntedMonsterCount( DWORD GuildIdx, DWORD MonsterCount )
{
	sprintf(txt, "EXEC dbo.MP_GUILD_ADD_HUNTEDMONSTER_COUNT %d, %d", GuildIdx, MonsterCount);
	g_DB.Query(eQueryType_FreeQuery, eGuildAddHuntedMonsterCount, GuildIdx, txt);
}

void RGuildAddHuntedMonsterCount( LPQUERY pData, LPDBMESSAGE pMessage )
{
	//DB 醚 钦魂 蔼 罐酒客辑 辨靛 沥焊 技泼. 甘辑滚甸俊霸 傈颇.
	DWORD GuildIdx = pMessage->dwID;
	DWORD count = pMessage->dwResult;

	if(!count)	return;

	DWORD GuildHuntedMonsterTotalCount = atoi((char*)pData->Data[eGHMC_TotalCount]);
	//DWORD DBProcessTime = atoi((char*)pData->Data[eGHMC_DB_Date]);

	GUILDMGR->SendGuildHuntedMonsterTotalCountToMapServerExceptOneself(GuildIdx, GuildHuntedMonsterTotalCount);
}

void GuildGetHuntedMonsterTotalCountWithInit( DWORD GuildIdx )
{
	sprintf(txt, "EXEC dbo.MP_GUILD_CONVERT_HUNTEDMONSTER_COUNT %d", GuildIdx);
	g_DB.Query(eQueryType_FreeQuery, eGuildGetHuntedMonsterTotalCountWithInit, GuildIdx, txt);
}

void RGuildGetHuntedMonsterTotalCountWithInit( LPQUERY pData, LPDBMESSAGE pMessage )
{
	//!!!肺弊 巢败林扁(甘锅龋,醚钦,函券等器牢飘)
	//DB 醚 钦魂 蔼 罐酒客 器牢飘肺 沥魂
	DWORD GuildIdx = pMessage->dwID;
	DWORD count = pMessage->dwResult;

	if(!count)	return;

	DWORD GuildHuntedMonsterTotalCount = atoi((char*)pData->Data[0]);
	//DWORD GuildHuntedMonsterTotalCount = atoi((char*)pData->Data[eGCHMC_TotalCount]);
	//DWORD DBProcessTime = atoi((char*)pData->Data[eGCHMC_DB_Date]);
	if(0 == GuildHuntedMonsterTotalCount)	return;

	GUILDMGR->GuildConvertHuntedMonsterTotalCountToGuildPoint(GuildIdx, GuildHuntedMonsterTotalCount);
}

void GuildAddGuildPoint( DWORD GuildIdx, DWORD rAddPoint, DWORD rAddKind, DWORD rAdditionalData )
{
	sprintf(txt, "EXEC dbo.MP_GUILD_ADD_GUILDPOINT %d, %d, %d, %d", GuildIdx, rAddPoint, rAddKind, rAdditionalData);
	g_DB.Query(eQueryType_FreeQuery, eGuildAddGuildPoint, GuildIdx, txt);
}

void RGuildAddGuildPoint( LPQUERY pData, LPDBMESSAGE pMessage )
{
	DWORD GuildIdx = pMessage->dwID;
	DWORD count = pMessage->dwResult;

	if(!count)	return;

	DWORD GuildAddPoint =	atoi((char*)pData->Data[eGAGP_AddPoint]);
	DWORD GuildTotalPoint = atoi((char*)pData->Data[eGAGP_TotalPoint]);
	int AddKind =			atoi((char*)pData->Data[eGAGP_AddKind]);
	DWORD AdditionalData =	atoi((char*)pData->Data[eGAGP_AdditionalData]);

	GUILDMGR->AddGuildPointFromDBReturn(GuildIdx, GuildAddPoint, GuildTotalPoint, AddKind, AdditionalData);
}

void GuildUseGuildPoint( DWORD GuildIdx, DWORD rUsePoint, DWORD rUseKind, DWORD rKindIdx, DWORD rEndtime )
{
	sprintf(txt, "EXEC dbo.MP_GUILD_USE_GUILDPOINT %d, %d, %d, %d, %d", GuildIdx, rUsePoint, rUseKind, rKindIdx, rEndtime);
	g_DB.Query(eQueryType_FreeQuery, eGuildUseGuildPoint, GuildIdx, txt);
}

void RGuildUseGuildPoint( LPQUERY pData, LPDBMESSAGE pMessage )
{
	DWORD GuildIdx = pMessage->dwID;
	DWORD count = pMessage->dwResult;

	if(!count)	return;

	DWORD GuildUsePoint =	atoi((char*)pData->Data[eGUGP_UsePoint]);
	// 辨靛器牢飘荤侩矫 俊矾贸府
	if( GuildUsePoint <= 0 )
		return;
	DWORD GuildTotalPoint = atoi((char*)pData->Data[eGUGP_TotalPoint]);
	int UseKind =			atoi((char*)pData->Data[eGUGP_UseKind]);
	DWORD KindIdx =			atoi((char*)pData->Data[eGUGP_KindIdx]);
	DWORD Endtime =			atoi((char*)pData->Data[eGUGP_Endtime]);

	GUILDMGR->UseGuildPointFromDBReturn(GuildIdx, GuildUsePoint, GuildTotalPoint, UseKind, KindIdx, Endtime);
}

void GuildPlustimeEnd( DWORD GuildIdx, DWORD rPlusTimeIdx )
{
	sprintf(txt, "EXEC dbo.MP_GUILD_PLUSTIME_END %d, %d", GuildIdx, rPlusTimeIdx);
	g_DB.Query(eQueryType_FreeQuery, eGuildPlustimeEnd, GuildIdx, txt);
}

void RGuildPlustimeEnd(LPQUERY pData, LPDBMESSAGE pMessage)
{
	DWORD GuildIdx = pMessage->dwID;
	DWORD count = pMessage->dwResult;

	if(!count)	return;

	DWORD PlustimeIdx =		atoi((char*)pData->Data[0]);

	GUILDMGR->EndGuildPlustimeDBReturn(GuildIdx, PlustimeIdx);
}

void LogGuildPoint( DWORD GuildIdx, DWORD TotalGuildPoint, BOOL bAdd, int eKind, DWORD GuildPoint, DWORD AddData, DWORD CharacterIdx )
{
	sprintf(txt, "EXEC dbo.Up_GuildPointLog %d, %d, %d, %d, %d,  %d, %d",
		GuildIdx,
		TotalGuildPoint,
		bAdd,
		eKind,
		GuildPoint,
		AddData,
		CharacterIdx
		);
	g_DB.LogQuery(eQueryType_FreeQuery, eLogGuildPoint, 0, txt);
}

void GuildMoneyUpdate(DWORD GuildIdx, MONEYTYPE Money)
{
	sprintf(txt, "EXEC %s %u, %u", STORED_GUILD_MONEYUPDATE, GuildIdx, Money);
	g_DB.Query(eQueryType_FreeQuery, eGuildMoneyUpdate, 0, txt);
}

void GuildGiveMemberNickName(DWORD TargetId, char * NickName)
{
    // 2014-06-25 check string param !

	if( !isValidQueryString( NickName))
	{
	   return;
	}

	if( CheckString( NickName) )
	{
		return;
	}

	sprintf(txt, "EXEC %s %u, \'%s\'", STORED_GUILD_GIVENICKNAME, TargetId, NickName);
	g_DB.Query(eQueryType_FreeQuery, eGuildGiveNickName, 0, txt);
}

void PyogukBuyPyoguk(DWORD PlayerIDX)
{
	sprintf(txt, "EXEC %s %d", STORED_PYOGUK_BUYPYOGUK, PlayerIDX);
	g_DB.Query(eQueryType_FreeQuery, ePyogukBuyPyoguk, PlayerIDX, txt);
}

void PyogukMoneyUpdateToDB(DWORD UserIdx, MONEYTYPE PyogukMoney)
{
	sprintf(txt, "EXEC %s %u, %u", STORED_PYOGUK_MONEYUPDATE, UserIdx, PyogukMoney);
	g_DB.Query(eQueryType_FreeQuery, ePyogukMoneyUpdate, 0, txt);
}

void SaveCharInfoBeforeLogout(DWORD PlayerIdx, DWORD dwConnectionIndex, DWORD UserIdx, MONEYTYPE PyogukMoney)
{
	sprintf(txt, "EXEC %s %d, %u, %u", STORED_CHARACTER_SAVEINFOBEFORELOGOUT, dwConnectionIndex, UserIdx, PyogukMoney);
	g_DB.Query(eQueryType_FreeQuery, eSaveChrInfoBeforeOut, PlayerIdx, txt);
}


/* Aㄏ〓ā∷㈢EA ief
#define STORED_PYOCUK_INFO				"MP_PYOGUK_Info"
#define STORED_PYOCUK_ITEMINFO			"MP_PYOGUK_ItemInfo"
*/

/* CIㄏo d¨〓∷〓 ¨uod AU¨uA */
void CharacterPyogukInfo(DWORD UserIdx, DWORD CharacterIdx)
{
//	char txt[128];
	sprintf(txt, "EXEC %s %d", STORED_PYOGUK_INFO, UserIdx);
	g_DB.Query(eQueryType_FreeQuery, eCharacterPyogukInfo, CharacterIdx, txt);
}

void CharacterPyogukItemInfo(DWORD CharacterIdx, DWORD StartDBIdx)
{
//	char txt[128];
	sprintf(txt, "EXEC %s %d, %d", STORED_PYOGUK_ITEMINFO, CharacterIdx, StartDBIdx);
	g_DB.Query(eQueryType_FreeQuery, eCharacterPyogukItemInfo, CharacterIdx, txt);
}

void FriendNotifyLogouttoClient(DWORD PlayerID)
{
//	char txt[128];
	sprintf(txt, "EXEC %s %d", STORED_FRIEND_NOTIFYLOGOUT, PlayerID);
	g_DB.Query(eQueryType_FreeQuery, eFriendNotifyLogout, PlayerID, txt);
}

void WantedLoad(WANTEDTYPE StartWantedIDX)
{
//	char txt[128];
	sprintf(txt, "EXEC %s %d", STORED_WANTED_LOAD, StartWantedIDX);
	g_DB.Query(eQueryType_FreeQuery, eWantedLoad, 0, txt);	
}

void WantedInfoByUserLogIn(DWORD CharacterIDX)
{
//	char txt[128];
	sprintf(txt, "EXEC %s %d", STORED_WANTED_INFO, CharacterIDX);
	g_DB.Query(eQueryType_FreeQuery, eWantedGetList, CharacterIDX, txt);
}

void WantedBuyRight(DWORD CharacterIDX, WANTEDTYPE WantedIDX, MONEYTYPE AddPrize)
{
//	char txt[128];
	sprintf(txt, "EXEC %s %d, %d, %d", STORED_WANTED_BUYRIGHT, CharacterIDX, WantedIDX, AddPrize);
	g_DB.Query(eQueryType_FreeQuery, eWantedBuyRight, 0, txt);
}

void WantedRegist(DWORD CharacterIDX, DWORD TargetIDX, MONEYTYPE Prize)
{
//	char txt[128];
	sprintf(txt, "EXEC %s %d, %d, %d", STORED_WANTED_REGIST, CharacterIDX, TargetIDX, Prize);
	g_DB.Query(eQueryType_FreeQuery, eWantedRegist, CharacterIDX, txt);
}

void WantedGiveUpRight(DWORD CharacterIDX, DWORD WantedIDX)
{
	sprintf(txt, "EXEC %s %d, %d", STORED_WANTED_GIVEUP, CharacterIDX, WantedIDX);
	g_DB.Query(eQueryType_FreeQuery, eWantedGiveupRight, 0, txt);
}

void WantedComplete(DWORD CharacterIDX, char* TargetName, WANTEDTYPE WantedIDX)
{
	 // 2014-06-25 check string param !

	if( !isValidQueryString( TargetName))
	{
	   return;
	}

	if( CheckString( TargetName) )
	{
		return;
	}
	sprintf(txt, "EXEC %s %d, \'%s\', %d", STORED_WANTED_COMPLETE, CharacterIDX, TargetName, WantedIDX);
	g_DB.Query(eQueryType_FreeQuery, eWantedComplete, WantedIDX, txt);
}

void WantedDestroyed(DWORD WantedIDX)
{
	sprintf(txt, "EXEC %s %d", STORED_WANTED_DESTROYED, WantedIDX);
	g_DB.Query(eQueryType_FreeQuery, eWantedDestroyed, WantedIDX, txt);
}

void WantedOrderedList(DWORD CharacterIDX, DWORD Page, char* OrderType, BYTE PageUnit)
{
 	 // 2014-06-25 check string param !

	if( !isValidQueryString(OrderType))
	{
	   return;
	}

	if( CheckString( OrderType) )
	{
		return;
	}   
	sprintf(txt, "EXEC %s %d, \'%s\', %d", STORED_WANTED_ORDERLIST, Page, OrderType, PageUnit);
	g_DB.Query(eQueryType_FreeQuery, eWantedOrderList, CharacterIDX, txt);
}

void WantedMyList(DWORD CharacterIDX, DWORD Page, BYTE PageUnit)
{
//	char txt[128];
	sprintf(txt, "EXEC %s %d, %d, %d", STORED_WANTED_MYLIST, CharacterIDX, Page, PageUnit);
	g_DB.Query(eQueryType_FreeQuery, eWantedMyList, CharacterIDX, txt);
}

void WantedSearch(DWORD CharacterIDX, char* WantedName, DWORD Page, BYTE PageUnit)
{
    // 2014-06-25 check string param !

	if( !isValidQueryString(WantedName))
	{
	   return;
	}

	if( CheckString( WantedName) )
	{
		return;
	}  
	sprintf(txt, "EXEC %s \'%s\', %d, %d", STORED_WANTED_SEARCH, WantedName, Page, PageUnit);
	g_DB.Query(eQueryType_FreeQuery, eWantedSearch, CharacterIDX, txt);
}

void JournalGetList(DWORD CharacterIDX)
{
	sprintf(txt, "EXEC %s %d", STORED_JOURNAL_GETLIST, CharacterIDX);
	g_DB.Query(eQueryType_FreeQuery, eJournalGetList, CharacterIDX, txt);
}

void JouranlInsertQuest(DWORD CharacterIDX, BYTE Kind, DWORD QuestIDX, DWORD SubQuestIDX, int bCompleted)
{
	sprintf(txt, "EXEC %s %d, %d, %d, %d, %d", STORED_JOURNAL_INSERTQUEST, CharacterIDX, Kind, QuestIDX, SubQuestIDX, bCompleted);
	g_DB.Query(eQueryType_FreeQuery, eJournalInsertQuest, 0, txt);
}

void JouranlInsertWanted(DWORD CharacterIDX, BYTE Kind, DWORD Param, char* ParamName)
{
    // 2014-06-25 check string param !

	if( !isValidQueryString(ParamName))
	{
	   return;
	}

	if( CheckString( ParamName) )
	{
		return;
	}  
	sprintf(txt, "EXEC %s %d, %d, %d, \'%s\'", STORED_JOURNAL_INSERTWANTED, CharacterIDX, Kind, Param, ParamName);
	g_DB.Query(eQueryType_FreeQuery, eJournalInsertWanted, 0, txt);
}

void JournalInsertLevel(DWORD CharacterIDX, BYTE Kind, DWORD Level)
{
	sprintf(txt, "EXEC %s %d, %d, %d", STROED_JOURNAL_INSERTLEVEL, CharacterIDX, Kind, Level);
	g_DB.Query(eQueryType_FreeQuery, eJournalInsertLevel, 0, txt);
}
void JournalSaveUpdate(DWORD CharacterIDX, DWORD JournalIndex)
{
	sprintf(txt, "EXEC %s %d, %d", STROED_JOURNAL_SAVEUPDATE, CharacterIDX, JournalIndex);
	g_DB.Query(eQueryType_FreeQuery, eJournalSaveUpdate, 0, txt);
}

void JournalDelete(DWORD CharacterIDX, DWORD JournalIndex)
{
	sprintf(txt, "EXEC %s %d, %d", STROED_JOURNAL_DELETE, CharacterIDX, JournalIndex);
	g_DB.Query(eQueryType_FreeQuery, eJournalDelete, 0, txt);
}

void CharacterHeroInfoUpdate(CPlayer* pPlayer)
{
	if( !pPlayer->GetInited() )	return;

	BASEOBJECT_INFO baseinfo;
	HERO_TOTALINFO	heroinfo;
	
	pPlayer->GetBaseObjectInfo(&baseinfo);
	pPlayer->GetHeroTotalInfo(&heroinfo);

//	char txt[512];   add the mallmoney and goldmoney database write! 2014-05-05
	sprintf(txt, "EXEC  %s %d, %d, %d, %d, %d, %d, %d, %d, %I64d, %d, %u, %d, %u, %d, %d, %d,%d,%d,%d,%d,%d,%d,%d,%d",
		STORED_CHARACTER_HEROINFOUPDATE,
		baseinfo.dwObjectID, 
		heroinfo.wGenGol, 
		heroinfo.wNaeRyuk, 
		heroinfo.wMinChub, 
		heroinfo.wCheRyuk, 
		heroinfo.wSimMek, 
		heroinfo.Fame,
		heroinfo.wKarma, 
		heroinfo.ExpPoint, 
		heroinfo.LevelUpPoint, 
		heroinfo.Money, 
		heroinfo.KyungGongGrade,
		pPlayer->GetPlayerAbilityExpPoint(), 
		heroinfo.Playtime, 
		heroinfo.LastPKModeEndTime,
		heroinfo.MaxLevel,
		heroinfo.MallMoney,
		heroinfo.GoldMoney,
		heroinfo.ReSet,
		pPlayer->GetShiFuID(),
		pPlayer->GetPaoDianTime(),
		pPlayer->GetEventLastMapTime(),
		pPlayer->GetEventIDFlag(),
		heroinfo.dwKillPlayerTimes
		);
	g_DB.Query(eQueryType_FreeQuery, eHeroInfoUpdate, 0, txt);
}

void CharacterTotalInfoUpdate(CPlayer* pPlayer)
{
	if( !pPlayer->GetInited() )	return;

	CHARACTER_TOTALINFO totinfo;
	pPlayer->GetCharacterTotalInfo(&totinfo);

	// 06. 06 - 捞康霖 吝夸!!!
	//MP_CHARACTER_TotalInfoUpdate 荐沥 鞘夸窃
//	char txt[512];
	sprintf(txt, "EXEC  %s %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d",
		STORED_CHARACTER_TOTALINFOUPDATE,
		pPlayer->GetID(), totinfo.Gender,
		totinfo.Life,
		totinfo.Shield,
		totinfo.Stage,
		totinfo.Level,
		totinfo.LoginMapNum, totinfo.FaceType, totinfo.HairType, totinfo.WearedItemIdx[eWearedItem_Hat],
		totinfo.WearedItemIdx[eWearedItem_Weapon], totinfo.WearedItemIdx[eWearedItem_Dress],
		totinfo.WearedItemIdx[eWearedItem_Shoes], totinfo.WearedItemIdx[eWearedItem_Ring1],
		totinfo.WearedItemIdx[eWearedItem_Ring2], totinfo.WearedItemIdx[eWearedItem_Cape],
		totinfo.WearedItemIdx[eWearedItem_Necklace], totinfo.WearedItemIdx[eWearedItem_Armlet],
		totinfo.WearedItemIdx[eWearedItem_Belt],
		totinfo.BadFame);
	g_DB.Query(eQueryType_FreeQuery, eTotalInfoUpdate, 0, txt);
}

void CharacterBadFameUpdate(DWORD PlayerID, int BadFame, DWORD Money)
{	
	sprintf(txt, "EXEC  %s %d, %d, %u", STORED_CHARACTER_BADFAMEUPDATE, PlayerID, BadFame, Money);
	g_DB.Query(eQueryType_FreeQuery, eTotalInfoUpdate, 0, txt);
}

void CharacterLvlExpUpdate(DWORD PlayerID, LEVELTYPE level, EXPTYPE exppoint)
{
	sprintf(txt, "update TB_Character \
set character_grade = %d, character_expoint = %I64d \
where character_idx = %d", level, exppoint, PlayerID);
	g_DB.Query(eQueryType_FreeQuery, eTotalInfoUpdate, 0, txt);
}

void CharacterMugongInfoUpdate(CPlayer* pPlayer)
{
	MUGONGBASE* pMugong;

#ifdef _JAPAN_LOCAL_
 	for(int n=TP_MUGONG_START; n<TP_MUGONG_END; ++n)
   	{
   		pMugong = pPlayer->GetMugongBase(n);
 		if(pMugong->dwDBIdx==0 || pMugong->bWear==0)
   			continue;
   		MugongUpdateToDB(pMugong, "TOTALUPDATE");
   	}
   	
 	for(n=TP_JINBUB_START; n<TP_JINBUB_START+SLOT_JINBUB_NUM; ++n)
   	{
   		pMugong = pPlayer->GetMugongBase(n);
 		if(pMugong->dwDBIdx==0 || pMugong->bWear==0)
   			continue;
   		MugongUpdateToDB(pMugong, "TOTALUPDATE");
   	}
#elif defined _HK_LOCAL_
 	for(int n=TP_MUGONG_START; n<TP_MUGONG_END; ++n)
   	{
   		pMugong = pPlayer->GetMugongBase(n);
 		if(pMugong->dwDBIdx==0 || pMugong->bWear==0)
   			continue;
   		MugongUpdateToDB(pMugong, "TOTALUPDATE");
   	}
   	
 	for(n=TP_JINBUB_START; n<TP_JINBUB_START+SLOT_JINBUB_NUM; ++n)
   	{
   		pMugong = pPlayer->GetMugongBase(n);
 		if(pMugong->dwDBIdx==0 || pMugong->bWear==0)
   			continue;
   		MugongUpdateToDB(pMugong, "TOTALUPDATE");
   	}
#elif defined _TL_LOCAL_
 	for(int n=TP_MUGONG_START; n<TP_MUGONG_END; ++n)
   	{
   		pMugong = pPlayer->GetMugongBase(n);
 		if(pMugong->dwDBIdx==0 || pMugong->bWear==0)
   			continue;
   		MugongUpdateToDB(pMugong, "TOTALUPDATE");
   	}
   	
 	for(n=TP_JINBUB_START; n<TP_JINBUB_START+SLOT_JINBUB_NUM; ++n)
   	{
   		pMugong = pPlayer->GetMugongBase(n);
 		if(pMugong->dwDBIdx==0 || pMugong->bWear==0)
   			continue;
   		MugongUpdateToDB(pMugong, "TOTALUPDATE");
   	}

#else
	for(int n=TP_MUGONG1_START; n<TP_MUGONG2_END; ++n)
	{
		pMugong = pPlayer->GetMugongBase(n);
		if(pMugong == NULL || pMugong->dwDBIdx==0 || pMugong->bWear==0)
			continue;
		MugongUpdateToDB(pMugong, "TOTALUPDATE");
	}
	
	// magi82(2) - Titan(071022) 鸥捞藕 公傍 版氰摹 DB 历厘窍绰 何盒
	int n=TP_TITANMUGONG_START;
	for(n=TP_TITANMUGONG_START; n<TP_TITANMUGONG_END; ++n)
	{
		pMugong = pPlayer->GetMugongBase(n);
		if(pMugong == NULL || pMugong->dwDBIdx==0 || pMugong->bWear==0)
			continue;
		MugongUpdateToDB(pMugong, "TOTALUPDATE");
	}

	for(n=TP_JINBUB_START; n<TP_JINBUB_END; ++n)
	{
		pMugong = pPlayer->GetMugongBase(n);
		if(pMugong == NULL || pMugong->dwDBIdx==0 || pMugong->bWear==0)
			continue;
		MugongUpdateToDB(pMugong, "TOTALUPDATE");
	}
#endif

}

void CharacterTablePartyIdxUpdate(DWORD PlayerID) 
{
//	char txt[128];
	sprintf(txt, "EXEC %s %d", STORED_PARTY_CHARACTERUPDATE, PlayerID);
		
	g_DB.Query(eQueryType_FreeQuery, ePartyCharacterTbUpdate, 0, txt);
}

void PartyAddMember(DWORD PartyIDX, DWORD TargetID)
{
//	char txt[128];
	sprintf(txt, "EXEC %s %d, %d ", STORED_PARTY_ADDMEMBER, PartyIDX, TargetID);
	g_DB.Query(eQueryType_FreeQuery, ePartyAddMember, TargetID, txt);
}

void PartyDelMember(DWORD PartyIDX, DWORD TargetID)
{
//	char txt[128];
	sprintf(txt, "EXEC %s %d, %d ", STORED_PARTY_DELMEMBER, PartyIDX, TargetID);
	g_DB.Query(eQueryType_FreeQuery, ePartyDelMember, PartyIDX, txt);
}

void PartyChangeMaster(DWORD PartyID, DWORD PlayerID, DWORD TargetPlayerID)
{
//	char txt[128];
	sprintf(txt, "EXEC %s %d, %d, %d", STORED_PARTY_CHANGEMASTER, PartyID, PlayerID, TargetPlayerID);
	g_DB.Query(eQueryType_FreeQuery, ePartyChangeMaster, PartyID, txt);
}

void SavePointUpdate(DWORD CharacterIDX, WORD LoginPoint_Idx, WORD MapNum)
{
//	char txt[128];
	sprintf(txt, "EXEC  %s %d, %d, %d",	STORED_SAVE_LOGINPOINT, CharacterIDX, LoginPoint_Idx, MapNum);
	g_DB.Query(eQueryType_FreeQuery, eSavePoint, 0, txt);
}

void SaveMapChangePointUpdate(DWORD CharacterIDX, WORD MapChangePoint_Idx)
{
//	char txt[128];
	sprintf(txt, "EXEC  %s %d, %d",	STORED_SAVE_MAPCHAGEPOINT, CharacterIDX, MapChangePoint_Idx);
	g_DB.Query(eQueryType_FreeQuery, eSavePoint, 0, txt);
}
	 
void SaveMapChangePointReturn(DWORD CharacterIDX, WORD MapChangePoint_Idx, DWORD dwConnectionIdx, LEVELTYPE lvl, EXPTYPE ExpPoint, MONEYTYPE PyogukMoney)
{
//	char txt[128];
	sprintf(txt, "EXEC  %s %u, %d, %u, %d, %I64d, %u",	STORED_SAVE_MAPCHAGEPOINTRET, CharacterIDX, MapChangePoint_Idx, dwConnectionIdx, lvl, ExpPoint, PyogukMoney);
	g_DB.Query(eQueryType_FreeQuery, eSaveMapChangePoint, CharacterIDX, txt);
}


void FameCharacterUpdate(DWORD PlayerIDX, FAMETYPE val)
{
//	char txt[128];
	sprintf(txt, "EXEC  %s %d, %d",	STORED_FAME_CHARACTERUPDATE, PlayerIDX, val);
	g_DB.Query(eQueryType_FreeQuery, eFameCharacterUpdate, 0, txt);
}

void BadFameCharacterUpdate(DWORD PlayerIDX, FAMETYPE val)
{
//	char txt[128];
	sprintf(txt, "EXEC  %s %d, %d",	STORED_BADFAME_CHARACTERUPDATE, PlayerIDX, val);
	g_DB.Query(eQueryType_FreeQuery, eBadFameCharacterUpdate, 0, txt);
}

void PKCharacterUpdate(DWORD PlayerIDX, DWORD val)
{
//	char txt[128];
	sprintf(txt, "EXEC  %s %d, %d",	STORED_PK_CHARACTERUPDATE, PlayerIDX, val);
	g_DB.Query(eQueryType_FreeQuery, ePKCharacterUpdate, 0, txt);
}

void QuestTotalInfo(DWORD PlayerID)
{
//	char txt[128];
	sprintf(txt, "EXEC  %s %d",	"MP_QUEST_TotalInfo", PlayerID);
	g_DB.Query(eQueryType_FreeQuery, eQuestTotalInfo, PlayerID, txt);
}

void QuestUpdateToDB(DWORD PlayerID, DWORD QuestIdx, QSTATETYPE State, BYTE bEnd)
{
//	char txt[128];
	sprintf(txt, "EXEC  %s %d, %d, %d, %d",	"MP_QUEST_Update", PlayerID, QuestIdx, State, bEnd);
	g_DB.Query(eQueryType_FreeQuery, eQuestUpdate, 0, txt);
}

// RaMa
void QuestMainQuestLoad(DWORD PlayerID, int QuestIdx )
{
//	char txt[128];
	sprintf(txt, "EXEC  %s %d, %d", STORED_QUEST_MAINQUEST_LOAD, PlayerID, QuestIdx );
	g_DB.Query(eQueryType_FreeQuery, eQuestMainQuestLoad, PlayerID, txt);
}

void QuestMainQuestInsert(DWORD PlayerID, DWORD mQuestIdx, QSTATETYPE State, DWORD Time)
{
//	char txt[128];
	sprintf(txt, "EXEC  %s %d, %d, %d, %d", STORED_QUEST_MAINQUEST_INSERT, PlayerID, mQuestIdx, State, Time);
	g_DB.Query(eQueryType_FreeQuery, eQuestMainQuestInsert, PlayerID, txt);
}

void QuestMainQuestDelete(DWORD PlayerID, DWORD mQuestIdx)
{
//	char txt[128];
	sprintf(txt, "EXEC  %s %d, %d", STORED_QUEST_MAINQUEST_DELETE, PlayerID, mQuestIdx);
	g_DB.Query(eQueryType_FreeQuery, eQuestMainQuestDelete, PlayerID, txt);
}

void QuestMainQuestUpdateToDB(DWORD PlayerID, DWORD mQuestIdx, QSTATETYPE State, DWORD EndParam, DWORD Time)
{
//	char txt[128];
	sprintf(txt, "EXEC  %s %d, %d, %d, %d, %d", STORED_QUEST_MAINQUEST_UPDATE, PlayerID, mQuestIdx, State, EndParam, Time);
	g_DB.Query(eQueryType_FreeQuery, eQuestMainQuestUpdate, PlayerID, txt);
}

void QuestSubQuestLoad(DWORD PlayerID, int QuestIdx )
{
//	char txt[128];
	sprintf(txt, "EXEC  %s %d, %d", STORED_QUEST_SUBQUEST_LOAD, PlayerID, QuestIdx );
	g_DB.Query(eQueryType_FreeQuery, eQuestSubQuestLoad, PlayerID, txt);
}

void QuestItemload(DWORD PlayerID, int QuestIdx )
{
	//	char txt[128];
	sprintf(txt, "EXEC  %s %d, %d", STORED_QUEST_ITEM_LOAD, PlayerID, QuestIdx);
	g_DB.Query(eQueryType_FreeQuery, eQuestItemLoad, PlayerID, txt);
}

void QuestItemInsert(DWORD PlayerID, DWORD dwQuestIdx, DWORD ItemIdx, DWORD dwItemNum )
{
	//	char txt[128];
	sprintf(txt, "EXEC  %s %d, %d, %d, %d", STORED_QUEST_ITEM_INSERT, PlayerID, ItemIdx, dwItemNum, dwQuestIdx );
	g_DB.Query(eQueryType_FreeQuery, eQuestItemInsert, PlayerID, txt);
}

void QuestItemDelete(DWORD PlayerID, DWORD ItemIdx)
{
	//	char txt[128];
	sprintf(txt, "EXEC  %s %d, %d", STORED_QUEST_ITEM_DELETE, PlayerID, ItemIdx);
	g_DB.Query(eQueryType_FreeQuery, eQuestItemDelete, PlayerID, txt);
}

void QuestItemDeleteAll(DWORD PlayerID, DWORD dwQuestIdx)
{
	sprintf(txt, "EXEC  %s %d, %d", STORED_QUEST_ITEM_DELETE_ALL, PlayerID, dwQuestIdx);
	g_DB.Query(eQueryType_FreeQuery, eQuestItemDeleteAll, PlayerID, txt);
}

void QuestItemUpdate(DWORD PlayerID, DWORD dwQuestIdx, DWORD ItemIdx, DWORD ItemNum)
{
	//	char txt[128];
	sprintf(txt, "EXEC  %s %d, %d, %d, %d", STORED_QUEST_ITEM_UPDATE, PlayerID, ItemIdx, ItemNum, dwQuestIdx );
	g_DB.Query(eQueryType_FreeQuery, eQuestItemUpdate, PlayerID, txt);
}

void QuestSubQuestDelete(DWORD PlayerID, DWORD mQuestIdx, DWORD sQuestIdx)
{
//	char txt[128];
	sprintf(txt, "EXEC  %s %d, %d, %d", STORED_QUEST_SUBQUEST_DELETE, PlayerID, mQuestIdx, sQuestIdx);
	g_DB.Query(eQueryType_FreeQuery, eQuestSubQuestDelete, PlayerID, txt);
}

void QuestSubQuestDeleteAll( DWORD PlayerID, DWORD mQuestIdx )
{
	sprintf(txt, "EXEC  %s %d, %d", STORED_QUEST_SUBQUEST_DELETE_ALL, PlayerID, mQuestIdx);
	g_DB.Query(eQueryType_FreeQuery, eQuestSubQuestDeleteAll, PlayerID, txt);
}

void QuestSubQuestInsert(DWORD PlayerID, DWORD mQuestIdx, DWORD sQuestIdx, QSTATETYPE Data, QSTATETYPE Time)
{
//	char txt[128];
	sprintf(txt, "EXEC  %s %d, %d, %d, %d, %d", STORED_QUEST_SUBQUEST_INSERT, PlayerID, mQuestIdx, sQuestIdx, Data, Time);
	g_DB.Query(eQueryType_FreeQuery, eQuestSubQuestInsert, PlayerID, txt);
}

void QuestSubQuestUpdateToDB(DWORD PlayerID, DWORD mQuestIdx, DWORD sQuestIdx, QSTATETYPE Data, QSTATETYPE Time)
{
//	char txt[128];
	sprintf(txt, "EXEC  %s %d, %d, %d, %d, %d", STORED_QUEST_SUBQUEST_UPDATE, PlayerID, mQuestIdx, sQuestIdx, Data, Time);
	g_DB.Query(eQueryType_FreeQuery, eQuestSubQuestUpdate, PlayerID, txt);
}

void QuestMainQuestUpdateCheckTime( DWORD dwCharIdx, DWORD dwQuestIdx, DWORD dwCheckType, DWORD dwCheckTime )
{
	sprintf(txt, "dbo.MP_MAINQUEST_UpdateCheckTime %d, %d, %d, %d", dwCharIdx, dwQuestIdx, dwCheckType, dwCheckTime);
	g_DB.Query(eQueryType_FreeQuery, eQuestMainQuestUpdateCheckTime, dwCharIdx, txt);
}

void Quest_EndQuest_New( DWORD PlayerID, DWORD mQuestIdx, QSTATETYPE State, DWORD EndParam, DWORD Time )
{
	sprintf(txt, "dbo.MP_QUEST_ENDQUEST_NEW %d, %d, %d, %d, %d", PlayerID, mQuestIdx, State, EndParam, Time);
	g_DB.Query(eQueryType_FreeQuery, eQuestMainQuestUpdateCheckTime, PlayerID, txt);
}

void Quest_DeleteQuest_New( DWORD PlayerID, DWORD mQuestIdx )
{
	sprintf(txt, "dbo.MP_QUEST_DELETEQUEST_NEW %d, %d", PlayerID, mQuestIdx);
	g_DB.Query(eQueryType_FreeQuery, eQuestMainQuestUpdateCheckTime, PlayerID, txt);
}

void Quest_EndSubQuest_New( DWORD PlayerID, DWORD mQuestIdx, DWORD sQuestIdx, QSTATETYPE Data, QSTATETYPE Time )
{
	sprintf(txt, "dbo.MP_QUEST_ENDSUBQUEST_NEW %d, %d, %d, %d, %d", PlayerID, mQuestIdx, sQuestIdx, Data, Time);
	g_DB.Query(eQueryType_FreeQuery, eQuestMainQuestUpdateCheckTime, PlayerID, txt);
}


//////////////////////////////////////////////////////////////////////////
// Log āu⒋A EIAa CO¨uo
/* ⒋ㄏi∷i】¨u(oA, ¨o¨〓AE¨〓?Eㄏ¨oA EIAa */
void InsertLogCharacter( DWORD charIdx, LEVELTYPE level, HERO_TOTALINFO* pTotalInfo )
{
//	char txt[128];
	sprintf(txt, "EXEC  %s %d, %d, %d, %d, %d, %d, %d",	"dbo.up_CharacterLog",
						charIdx, level,
						pTotalInfo->wGenGol, pTotalInfo->wMinChub,
						pTotalInfo->wCheRyuk, pTotalInfo->wSimMek,
						pTotalInfo->LevelUpPoint);
	g_DB.LogQuery(eQueryType_FreeQuery, eLogCharacter, 0, txt);
}

/* ¨oAāI∷⊥e¨〓ā EIAa */
void InsertLogExp( BYTE bType, DWORD charIdx, LEVELTYPE level, DWORD changeValue, EXPTYPE NowExp, WORD MurdererKind, DWORD MurdererIdx, DWORD CurAbilPoint)
{
	sprintf(txt, "EXEC  %s %d, %d, %d, %u, %I64d, %d, %d, %d",	"dbo.up_ExpPointLog", bType, charIdx, level, changeValue, NowExp, MurdererKind, MurdererIdx, CurAbilPoint);
	g_DB.LogQuery(eQueryType_FreeQuery, eLogExp, 0, txt);
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////
void InsertLogMoney( BYTE type, DWORD charIdx, MONEYTYPE InvenMoney, MONEYTYPE ChangeValue, DWORD TargetID, MONEYTYPE TargetMoney )
{
//	sprintf(txt, "EXEC  %s %d, %d, %d, %d",	"up_MoneyLog", type, charIdx, InvenMoney, TargetMoney );
//	g_DB.Query(eQueryType_FreeQuery, eLogMoney, 0, txt);
	sprintf(txt, "EXEC  %s %d, %d, %u, %u, %d, %u",	"dbo.up_MoneyLog", type, charIdx, InvenMoney, ChangeValue, TargetID, TargetMoney );
	g_DB.LogQuery(eQueryType_FreeQuery, eLogMoney, 0, txt);
}

void InsertLogWorngMoney( BYTE type, DWORD charIdx, MONEYTYPE changeValue, MONEYTYPE InvenMoney, MONEYTYPE PyogukMoney, DWORD targetIdx )
{
	sprintf(txt, "EXEC  %s %d, %d, %u, %u, %u, %d",	"dbo.up_WrongMoneyLog", type, charIdx, changeValue, InvenMoney, PyogukMoney, targetIdx );
	g_DB.LogQuery(eQueryType_FreeQuery, eLogMoneyWrong, 0, txt);	
}

void LogItemMoney(DWORD FromChrIdx, char* FromChrName, DWORD ToChrIdx, char* ToChrName,
				WORD LogType, MONEYTYPE FromTotalMoney, MONEYTYPE ToTotalMoney, MONEYTYPE ChangeMoney,
				DWORD ItemIdx, DWORD ItemDBIdx, POSTYPE ItemFromPos, POSTYPE ItemToPos,
				DURTYPE ItemDur, EXPTYPE ExpPoint)
{

    // 2014-06-25 check string param !

	if( !isValidQueryString(FromChrName) || !isValidQueryString(ToChrName))
	{
	   return;
	}

	if( CheckString(FromChrName) ||  CheckString(ToChrName))
	{
		return;
	} 

	sprintf(txt, "EXEC  %s %u, \'%s\', %u, \'%s\', %d, %u, %u, %u, %d, %d, %d, %d, %u, %I64d", "dbo.up_ItemMoneyLog", 
		FromChrIdx, FromChrName, ToChrIdx, ToChrName, 
		LogType, FromTotalMoney, ToTotalMoney, ChangeMoney,
		ItemIdx, ItemDBIdx, ItemFromPos, ItemToPos,
		ItemDur, ExpPoint);
	g_DB.LogQuery(eQueryType_FreeQuery, eLogItemMoney, 0, txt);
}

void LogCheat(DWORD Character_Idx, BYTE LogType, MONEYTYPE Param, DWORD ItemIdx, DWORD ItemDBIdx)
{
	sprintf(txt, "EXEC %s %d, %d, %d, %d, %d", "dbo.UP_CHEATE_Log",
		Character_Idx, LogType, Param, ItemIdx, ItemDBIdx);
	g_DB.LogQuery(eQueryType_FreeQuery, eLogItemMoney, 0, txt);
}

void LogItemOption(DWORD CharacterIdx, DWORD ItemDBIdx, ITEM_OPTION_INFO* pOptionInfo)
{
	sprintf(txt, "EXEC %s %d, %d, %d, %d, %d, %d, %d, %d, %d, %d,  %d, %d, %d, %d, %d,  %d, %d,  %d, %d, %d, %d, %d,  %d", "dbo.UP_ItemOptionLog",
		CharacterIdx,
		pOptionInfo->dwOptionIdx,
		pOptionInfo->dwItemDBIdx,
		pOptionInfo->GenGol,					
		pOptionInfo->MinChub,				
		pOptionInfo->CheRyuk,				
		pOptionInfo->SimMek,				
		pOptionInfo->Life,					
		pOptionInfo->NaeRyuk,			
		pOptionInfo->Shield,				
		(int)(pOptionInfo->AttrRegist.GetElement_Val(ATTR_FIRE)*100+0.00001),
		(int)(pOptionInfo->AttrRegist.GetElement_Val(ATTR_WATER)*100+0.00001),
		(int)(pOptionInfo->AttrRegist.GetElement_Val(ATTR_TREE)*100+0.00001),
		(int)(pOptionInfo->AttrRegist.GetElement_Val(ATTR_IRON)*100+0.00001),
		(int)(pOptionInfo->AttrRegist.GetElement_Val(ATTR_EARTH)*100+0.00001),
		pOptionInfo->PhyAttack,
		pOptionInfo->CriticalPercent,
		(int)(pOptionInfo->AttrAttack.GetElement_Val(ATTR_FIRE)*100+0.00001),
		(int)(pOptionInfo->AttrAttack.GetElement_Val(ATTR_WATER)*100+0.00001),
		(int)(pOptionInfo->AttrAttack.GetElement_Val(ATTR_TREE)*100+0.00001),
		(int)(pOptionInfo->AttrAttack.GetElement_Val(ATTR_IRON)*100+0.00001),
		(int)(pOptionInfo->AttrAttack.GetElement_Val(ATTR_EARTH)*100+0.00001),
		pOptionInfo->PhyDefense);
			
	g_DB.LogQuery(eQueryType_FreeQuery, eLogItemReinforce, 0, txt);
}

void LogItemRareOption(DWORD CharacterIdx, DWORD ItemDBIdx, ITEM_RARE_OPTION_INFO* pRareOptionInfo)
{
	sprintf(txt, "EXEC dbo.UP_ItemRareOptionLog %d, %d, %d,  %d, %d, %d, %d,  %d, %d, %d,  %d, %d, %d, %d, %d,  %d,  %d, %d, %d, %d, %d,  %d",
		CharacterIdx,
		pRareOptionInfo->dwRareOptionIdx,
		pRareOptionInfo->dwItemDBIdx,
		pRareOptionInfo->GenGol,					
		pRareOptionInfo->MinChub,				
		pRareOptionInfo->CheRyuk,				
		pRareOptionInfo->SimMek,				
		pRareOptionInfo->Life,					
		pRareOptionInfo->NaeRyuk,			
		pRareOptionInfo->Shield,				
		(int)(pRareOptionInfo->AttrRegist.GetElement_Val(ATTR_FIRE)*100+0.00001),
		(int)(pRareOptionInfo->AttrRegist.GetElement_Val(ATTR_WATER)*100+0.00001),
		(int)(pRareOptionInfo->AttrRegist.GetElement_Val(ATTR_TREE)*100+0.00001),
		(int)(pRareOptionInfo->AttrRegist.GetElement_Val(ATTR_IRON)*100+0.00001),
		(int)(pRareOptionInfo->AttrRegist.GetElement_Val(ATTR_EARTH)*100+0.00001),
		pRareOptionInfo->PhyAttack,
//		pRareOptionInfo->CriticalPercent,
		(int)(pRareOptionInfo->AttrAttack.GetElement_Val(ATTR_FIRE)*100+0.00001),
		(int)(pRareOptionInfo->AttrAttack.GetElement_Val(ATTR_WATER)*100+0.00001),
		(int)(pRareOptionInfo->AttrAttack.GetElement_Val(ATTR_TREE)*100+0.00001),
		(int)(pRareOptionInfo->AttrAttack.GetElement_Val(ATTR_IRON)*100+0.00001),
		(int)(pRareOptionInfo->AttrAttack.GetElement_Val(ATTR_EARTH)*100+0.00001),
		pRareOptionInfo->PhyDefense);
			
	g_DB.LogQuery(eQueryType_FreeQuery, eLogItemRare, 0, txt);
}

void LogGuild(DWORD CharacterIdx, DWORD GuildIdx, WORD LogType, DWORD Param1, DWORD Param2)
{
	sprintf(txt, "EXEC  %s %d, %d, %d, %d, %d",	"dbo.up_GuildLog", CharacterIdx, GuildIdx, LogType, Param1, Param2);
	g_DB.LogQuery(eQueryType_FreeQuery, eLogGuild, 0, txt);
}

void LogPet(DWORD CharacterIdx, DWORD UserIdx, BYTE LogType, DWORD PetDBIdx, DWORD PetSummonItemDBIdx, DWORD PetGrade, DWORD PetStamina, DWORD PetFriendship, BYTE PetAlive)
{
	sprintf(txt, "EXEC dbo.Up_PetLog %d, %d, %d, %d, %d,  %d, %d, %d, %d",
		LogType,
		PetDBIdx,
		PetSummonItemDBIdx,
		UserIdx,
		CharacterIdx,

		PetGrade,
		PetFriendship,
		PetStamina,
		PetAlive
		);
	g_DB.LogQuery(eQueryType_FreeQuery, eLogPet, 0, txt);
}

void LogGMToolUse( DWORD CharacterIdx, DWORD GMLogtype, DWORD Logkind, DWORD Param1, DWORD Param2 )
{
	sprintf(txt, "EXEC dbo.Up_GMToolUseLog %d, %d, %d, %d, %d",
		CharacterIdx,
		GMLogtype,
		Logkind,
		Param1,
		Param2
		);
	g_DB.LogQuery( eQueryType_FreeQuery, eLogGMToolUse, 0, txt );
}

/* type : 0->Obtain, 1->Remove, 2->Change Sung */
void InsertLogMugong( WORD type, DWORD charIdx, DWORD mugongIdx, DWORD mugongDBIdx, LEVELTYPE mugongSung, EXPTYPE mugongExp )
{
//	char txt[128];
	sprintf(txt, "EXEC  %s %d, %d, %d, %d, %d, %u",	"dbo.up_MugongLog", type, charIdx, mugongIdx, mugongDBIdx, mugongSung, mugongExp);
	g_DB.LogQuery(eQueryType_FreeQuery, eLogMugong, 0, txt);
}

void InsertLogMugongExp( DWORD charIdx, EXPTYPE changeValue, DWORD mugongIdx, DWORD mugongDBIdx, LEVELTYPE mugongSung, EXPTYPE mugongExp, POSTYPE mugongPos ) 
{
//	char txt[128];
	sprintf(txt, "EXEC  %s %d, %u, %d, %d, %d, %d, %d",	"dbo.up_MungongExpLog", 
		charIdx, changeValue, mugongIdx, mugongDBIdx, mugongSung, mugongExp, mugongPos);
	g_DB.LogQuery(eQueryType_FreeQuery, eLogMugongExp, 0, txt);
}

void UpdateCharacterInfoByTime(DWORD charIdx, EXPTYPE exp, DWORD abilityExp, MONEYTYPE money, DWORD time, DWORD LastPKEndTime )
{
//	char txt[128];
	sprintf(txt, "EXEC  %s %d, %I64d, %u, %u, %d, %d",	"dbo.MP_CHARACTER_UpdateInfoByTime", 
		charIdx, exp, abilityExp, money, time, LastPKEndTime );
	g_DB.Query(eQueryType_FreeQuery, eCharacterInfoByTime, 0, txt);
}

void UpdateMugongExpByTime(DWORD charIdx, DWORD DBIdx, EXPTYPE exp)
{
//	char txt[128];
	sprintf(txt, "EXEC  %s %d, %d, %u",	"dbo.MP_MUGONG_UpdateExpBytime", 
		charIdx, DBIdx, exp);
	g_DB.Query(eQueryType_FreeQuery, eMugongExpByTime, 0, txt);
}

void InsertSpeedHackCheck(DWORD charIdx, char* charName, DWORD Distance, DWORD skillDelay)
{
    // 2014-06-25 check string param !

	if( !isValidQueryString(charName))
	{
	   return;
	}

	if( CheckString(charName))
	{
		return;
	} 
	sprintf(txt, "EXEC  %s %d, \'%s\', %d, %d",	"dbo.up_SpeedHackCheck", 
		charIdx, charName, Distance, skillDelay);
	g_DB.Query(eQueryType_FreeQuery, eSpeedHackCheck, 0, txt);
}

void InsertLogTool( DWORD dwLogType, DWORD dwLogKind, DWORD dwOperIdx, char* sOperName, DWORD dwTargetIdx, char* sTargetName,
				    DWORD dwParam1, DWORD dwParam2, DWORD dwParam3, DWORD dwParam4 )
{
	 // 2014-06-25 check string param !

	if( !isValidQueryString(sOperName) ||  !isValidQueryString(sTargetName))
	{
	   return;
	}

	if( CheckString(sOperName) || CheckString(sTargetName))
	{
		return;
	} 
	sprintf(txt, "EXEC dbo.Up_ToolLog %d, %d, %d, \'%s\', %d, \'%s\', %d, %d, %d, %d", dwLogType, dwLogKind, dwOperIdx, sOperName,
		dwTargetIdx, sTargetName, dwParam1, dwParam2, dwParam3, dwParam4 );	
	if(g_DB.LogQuery(eQueryType_FreeQuery, eInsertLogTool, 0, txt) == FALSE)
	{
	}
}

//////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////
// DB⒋I¨〓IAI ∷〓¨々¨oAAo∷〓| ㄏi∷i¨u¨¨uㄏ Aㄏ┆∷〓∷cCI∷⊥A CO¨uo⊥ie.

//pjs
void  RAuctionCheck(LPQUERY pData,LPDBMESSAGE PMessage)
{
	if(atoi((char*)pData->Data[0]) == 0)
	{
				
	}
	else
	{		
		AUCTIONSUCCESS msg;

		memset(&msg,NULL,sizeof(msg));

		for(int i = 0; i < 10 ; i++)
		{
		
			strcpy(msg.resultlist[i].PRO_Name,((char*)pData[i].Data[sePRO_NAME]));
//			msg.resultlist[i].PRO_Name = ((char*)pData[i].Data[sePRO_NAME]);
			msg.resultlist[i].PRO_Amount = atoi((char*)pData[i].Data[sePRO_Amount]);
			strcpy(msg.resultlist[i].Auctioneer,((char*)pData[i].Data[seAuctioneer]));
//			msg.resultlist[i].Auctioneer = ((char*)pData[i].Data[seAuctioneer]);
			msg.resultlist[i].PRO_Index = atoi((char*)pData[i].Data[seREG_Index]);
		}
		g_Console.Log(eLogDisplay, 4, "AAA");
	}
}

//Select  PRO_Name, PRO_Amount, Auctioneer,REG_Index
//sePRO_NAME, sePRO_Amount, seAuctioneer,	seREG_Index


void RAuctionPageList(LPQUERY pData,LPDBMESSAGE PMessage)
{
	if(atoi((char*)pData->Data[0]) == 0)
	{

				
	}
	else
	{		
		BASELIST msg;

		memset(&msg,NULL,sizeof(msg));

		for(int i = 0; i < 10 ; i++)
		{
			msg.imfolist[i].REG_Index = atoi((char*)pData[i].Data[eREG_Index]);
			strcpy((char*)(msg.imfolist[i].BID_DUEDATE),((char*)pData[i].Data[eBID_DueDate]));
			strcpy(msg.imfolist[i].Auctioneer,((char*)pData[i].Data[eAuctioneer]));
			strcpy(msg.imfolist[i].BID_HighName,((char*)pData[i].Data[eHigh_Price_Name]));
			msg.imfolist[i].BID_HighPrice = atoi((char*)pData[i].Data[eBID_High_Price]);
			msg.imfolist[i].BID_ImmediatePrice = atoi((char*)pData[i].Data[eBID_Immediate_Price]);
			msg.imfolist[i].BID_LeftTime = atoi((char*)pData[i].Data[eBID_Left_Time]);
			msg.imfolist[i].JOIN_Amount = atoi((char*)pData[i].Data[eJOIN_Amount]);
			msg.imfolist[i].PRO_Amount = atoi((char*)pData[i].Data[ePRO_Amount]);
			msg.imfolist[i].PRO_Index = atoi((char*)pData[i].Data[ePRO_Index]);
			strcpy(msg.imfolist[i].PRO_Name,((char*)pData[i].Data[ePRO_NAME]));
		}
	}
}

void RAuctionConfirm(LPQUERY pData, LPDBMESSAGE PMessage)
{

}

void RTestIDPWQuery(LPQUERY pData, LPDBMESSAGE pMessage)
{
	g_Console.Log(eLogDisplay,0 , "ID/PW Result : UserID -- %d", pData->Data[0]);
}

void RCharacterNumSendAndCharacterInfo(LPQUERY pData, LPDBMESSAGE pMessage)
{
	DWORD count = pMessage->dwResult;
	if(!count)
	{
		// Aㄏ┆∷〓?AI d¨〓∷〓ㄏi∷ia ¨oC¨¨∷∷?7u
		//ASSERTMSG(0,"DB∷?∷㈢ AE∷〓?AI ⊥iIAIA∷〓ā∷㈢ ¨uㄏǎ¨o4I∷⊥U.");
		return;
	}
	
	BASEOBJECT_INFO Objinfo;
	CHARACTER_TOTALINFO Totalinfo;
	HERO_TOTALINFO Heroinfo;
	BASEMOVE_INFO Moveinfo;
	memset(&Objinfo,0,sizeof(BASEOBJECT_INFO));
	memset(&Totalinfo,0,sizeof(CHARACTER_TOTALINFO));

	memset(&Heroinfo,0,sizeof(HERO_TOTALINFO));
	memset(&Moveinfo,0,sizeof(BASEMOVE_INFO));

	
	
	Objinfo.dwObjectID = atoi((char*)pData->Data[eCS_ObjectID]);
	Objinfo.dwUserID =  atoi((char*)pData->Data[eCS_UserID]);
	Objinfo.BattleID = 0;
	Objinfo.BattleTeam = 0;
	
	// UserIDX∷⊥A ㄏ┆NaAo ¨uE∷⊥A∷⊥U
	Heroinfo.PartyID = atoi((char*)pData->Data[eCS_PartyID]);
	
	Totalinfo.Gender = atoi((char*)pData->Data[eCS_Gender]);
	Heroinfo.wGenGol = atoi((char*)pData->Data[eCS_Gengoal]);
	Totalinfo.Life = atoi((char*)pData->Data[eCS_Life]);
	Totalinfo.Shield = atoi((char*)pData->Data[eCS_Shield]);
	Totalinfo.MaxLife = Totalinfo.Life;
	Totalinfo.MaxShield = Totalinfo.Shield;
	Heroinfo.wNaeRyuk = atoi((char*)pData->Data[eCS_Energe]);
	Heroinfo.wMinChub = atoi((char*)pData->Data[eCS_Dex]);
	Heroinfo.wCheRyuk = atoi((char*)pData->Data[eCS_Sta]);
	Heroinfo.wSimMek = atoi((char*)pData->Data[eCS_Simmak]);
	Heroinfo.Fame = atoi((char*)pData->Data[eCS_Fame]);
	Heroinfo.wKarma = atoi((char*)pData->Data[eCS_Karma]);
	SafeStrCpy( Objinfo.ObjectName, (char*)pData->Data[eCS_Name], MAX_NAME_LENGTH + 1 );
	Totalinfo.Stage = atoi((char*)pData->Data[eCS_Stage]);
	Totalinfo.Level = atoi((char*)pData->Data[eCS_Grade]);
	Heroinfo.ExpPoint = (EXPTYPE)_atoi64((char*)pData->Data[eCS_Expoint]);
	Heroinfo.LevelUpPoint = atoi((char*)pData->Data[eCS_GradeUpPoint]);
	Heroinfo.Money = _atoi64((char*)pData->Data[eCS_Money]);
	Totalinfo.CurMapNum = GAMERESRCMNGR->GetLoadMapNum();
	Totalinfo.LoginMapNum = atoi((char*)pData->Data[eCS_Map]);
	Totalinfo.FaceType = atoi((char*)pData->Data[eCS_FaceType]);
	Totalinfo.HairType = atoi((char*)pData->Data[eCS_HeadType]);
	Totalinfo.WearedItemIdx[eWearedItem_Hat] = atoi((char*)pData->Data[eCS_Hat]);
	Totalinfo.WearedItemIdx[eWearedItem_Weapon] = atoi((char*)pData->Data[eCS_Weapon]);
	Totalinfo.WearedItemIdx[eWearedItem_Dress] = atoi((char*)pData->Data[eCS_Dress]);
	Totalinfo.WearedItemIdx[eWearedItem_Shoes] = atoi((char*)pData->Data[eCS_Shoes]);
	Totalinfo.WearedItemIdx[eWearedItem_Ring1] = atoi((char*)pData->Data[eCS_Ring1]);
	Totalinfo.WearedItemIdx[eWearedItem_Ring2] = atoi((char*)pData->Data[eCS_Ring2]);
	Totalinfo.WearedItemIdx[eWearedItem_Cape] = atoi((char*)pData->Data[eCS_Cape]);
	Totalinfo.WearedItemIdx[eWearedItem_Necklace] = atoi((char*)pData->Data[eCS_Necklace]);
	Totalinfo.WearedItemIdx[eWearedItem_Armlet] = atoi((char*)pData->Data[eCS_Armlet]);
	Totalinfo.WearedItemIdx[eWearedItem_Belt] = atoi((char*)pData->Data[eCS_Belt]);
	Totalinfo.MunpaID = atoi((char*)pData->Data[eCS_MunpaIDX]);
	Totalinfo.PositionInMunpa = atoi((char*)pData->Data[eCS_GuildPositon]);
	SafeStrCpy(Totalinfo.GuildName, (char*)pData->Data[eCS_GuildName], MAX_GUILD_NAME+1);
	SafeStrCpy(Totalinfo.NickName, (char*)pData->Data[eCS_NickName], MAX_GUILD_NICKNAME+1);

	Totalinfo.BadFame = atoi((char*)pData->Data[eCS_BadFame]);

	// Set Mallmoney and GoldMoney

	Heroinfo.MallMoney =atoi((char*)pData->Data[eCS_MallMoney]);

	Heroinfo.GoldMoney =atoi((char*)pData->Data[eCS_GoldMoney]);

	Heroinfo.ReSet=atoi((char*)pData->Data[eCS_ReSet]);

    // set marryname 2014-05-09

	SafeStrCpy(Totalinfo.MarryName, (char*)pData->Data[eCS_MarryName], MAX_NAME_LENGTH + 1);

	// set ShiTuSet 2014-05-09

	Totalinfo.ShiTuId=atoi((char*)pData->Data[eCS_ShiTuId]);

	SafeStrCpy(Totalinfo.ShiTuName, (char*)pData->Data[eCS_ShiTuName], MAX_NAME_LENGTH + 1);

	// set character info and equip for look !
	
	Heroinfo.ReSet = atoi((char*)pData->Data[eCS_ReSet]);

	Totalinfo.ReStage=atoi((char*)pData->Data[eCS_ReSet]);  

	// 附加角色闪名标志信息 2015-01-22

	Totalinfo.FlashNameFlag=atoi((char*)pData->Data[eCS_FNameFlag]);

	SafeStrCpy(Totalinfo.FlashName,(char*)pData->Data[eCsFName],MAX_NAME_LENGTH +1);

	

	Totalinfo.SafeStatic = atoi((char*)pData->Data[eCsSafe]);

	Totalinfo.TurnStageOne=atoi((char*)pData->Data[eCS_TurnOne]);
	Totalinfo.TurnStageTwo=atoi((char*)pData->Data[eCS_TurnTwo]);
	Totalinfo.TurnStageThree=atoi((char*)pData->Data[eCS_TurnThree]);


	
	Totalinfo.MapChangePoint_Index = atoi((char*)pData->Data[eCS_MapChangePoint]);

	Totalinfo.LoginPoint_Index = atoi((char*)pData->Data[eCS_LoginPoint]);
	
	Heroinfo.KyungGongGrade = atoi((char*)pData->Data[eCS_QuickPace]);
	
	Heroinfo.AbilityExp = atoi((char*)pData->Data[eCS_AbilityExp]);
	
	Totalinfo.Height = (float)atof((char*)pData->Data[eCS_Height]);
	Totalinfo.Width = (float)atof((char*)pData->Data[eCS_Width]);
	Heroinfo.Playtime = atoi((char*)pData->Data[eCS_Playtime]);
	Heroinfo.LastPKModeEndTime = atoi((char*)pData->Data[eCS_LastPKEndTime]);

	Heroinfo.MaxLevel = atoi((char*)pData->Data[eCS_MaxLevel]);
	Totalinfo.MarkName	= atoi((char*)pData->Data[eCS_MarkName]);
	SafeStrCpy(Heroinfo.MunpaCanEntryDate, (char*)pData->Data[eCS_MunpaCanEntryDate], 11);


	Heroinfo.dwKillPlayerTimes = atoi((char*)pData->Data[eCS_KillPlayerTimes]);

	if(Heroinfo.MaxLevel < Totalinfo.Level)
		Heroinfo.MaxLevel = Totalinfo.Level;

	Heroinfo.ExpFlag = (BYTE)(atoi((char*)pData->Data[eCS_ExpFlag]));

	if(!CheckBit(Heroinfo.ExpFlag, 4))
	{
		if(Totalinfo.Level < Heroinfo.MaxLevel)
		{
			Heroinfo.ExpFlag = 0x0F;
		}
		else	// ==
		{
			EXPTYPE maxexp = GAMERESRCMNGR->GetMaxExpPoint( Totalinfo.Level );
			int count = (int)((((float)Heroinfo.ExpPoint / (float)maxexp)*100) / 20);

			for( int i = 0; i < count; i++ )
			{
				Heroinfo.LevelUpPoint++;	// magi82(27)
				SetOnBit(&Heroinfo.ExpFlag, i);
			}
		}

		SetOnBit(&Heroinfo.ExpFlag, 4);	// 茄锅 利侩 登菌栏骨肺 促澜何畔 利侩 救窃
		UCharacterExpFlag(Objinfo.dwObjectID, Heroinfo.ExpFlag);	// 累诀饶 DB盎脚
	}

	////////////////////////////////////////////////////////////////////////////////////

	CPlayer* pPlayer = g_pServerSystem->InitPlayerInfo(&Objinfo, &Totalinfo, &Heroinfo);
	if(pPlayer == NULL)
		return;

	if (CHANNELSYSTEM->IsInsDungeonMap() == TRUE)
	{
		DWORD num = CHANNELSYSTEM->CreateInsDungeonMapChannel(pPlayer);
		if (num == 0)
			return;

		pPlayer->SetBattleID(num);
		pPlayer->SetGridID(num);
		pPlayer->SetChannelID(num);

		//셋팅된 인전 정보를 Player에게 보낸다. 
		//반드시 CreateInsDungeonMapChannel 이후에 들어갈것!!
		INSDUNGEONMGR->SendInsDungeonInfo(pPlayer);
	}
	//pPlayer->GetShopItemStats()->dwSkinItemIndex = (DWORD)(atoi((char*)pData->Data[eCS_SkinInfo]));

	
	VECTOR3 pos,RandPos;
	GAMERESRCMNGR->GetRegenPoint(pos,RandPos,Totalinfo.LoginPoint_Index,Totalinfo.MapChangePoint_Index);	

	pPlayer->SetLoginTime(Heroinfo.Playtime);

	pPlayer->SetPaoDianTime(atoi((char*)pData->Data[eCS_PdTime]));  // read the mallmoney time 2014-05-30


	pPlayer->SetEventLastMapTime(atoi((char*)pData->Data[eCS_LoginEventTime]));

	pPlayer->SetEventIDFlag(atoi((char*)pData->Data[eCS_LoginEventItem]));

#ifdef _SKILLSLOT_
	pPlayer->SetExtraMugongSlot(atoi((char*)pData->Data[eCS_SkillSlot]));
#else
	pPlayer->SetExtraMugongSlot(7);
#endif



	/*if(DungeonMGR->IsDungeon(g_pServerSystem->GetMapNum()))
	{
		if(VECTOR3* const pStartPos = DungeonMGR->GetStartPosFromeservationList(pPlayer->GetID()))
		{
			pPlayer->GetMoveInfo()->CurPosition = *pStartPos;
		}
		else if(pPlayer->IsDungeonObserver())
		{				
			pPlayer->GetMoveInfo()->CurPosition = *pPlayer->GetDungeonObserverPos();
		}
	}*/

	
	DWORD Position = pPlayer->GetInitPoint();
	if( Position )
	{
		WORD x, z;
		stMOVEPOINT Mp;
		Mp.value = Position;
		Mp.GetMovePoint( x, z );
		pPlayer->SetInitPoint( 0 );
		RandPos.x = x;
		RandPos.z = z;

		CCharMove::SetPosition(pPlayer, &RandPos);
	}

	CCharMove::InitMove(pPlayer,&RandPos);

	// guildunion info setting
	GUILDUNIONMGR->SetGuildUnionInfoToCharacterTotalInfo( pPlayer, Totalinfo.MunpaID );

	pPlayer->SetInitState(PLAYERINITSTATE_HERO_INFO,pMessage->dwID);
   	CharacterPetInfo(Objinfo.dwObjectID, Objinfo.dwUserID);

	//SW070127 鸥捞藕
	CharacterTitanInfo(Objinfo.dwObjectID, Objinfo.dwUserID);

	CharacterItemInfo( Objinfo.dwObjectID, MP_USERCONN_GAMEIN_SYN );

	// autonotelist
	AutoNoteListLoad( Objinfo.dwObjectID );
}

void RCharacterInfoQueryToEventMap(LPQUERY pData, LPDBMESSAGE pMessage)
{
	DWORD dwTeam = pMessage->dwID;
	DWORD count = pMessage->dwResult;
	if( !count )	return;
	
	BASEOBJECT_INFO Objinfo;
	CHARACTER_TOTALINFO Totalinfo;
	HERO_TOTALINFO Heroinfo;
	BASEMOVE_INFO Moveinfo;
	memset(&Objinfo,0,sizeof(BASEOBJECT_INFO));
	memset(&Totalinfo,0,sizeof(CHARACTER_TOTALINFO));
	memset(&Heroinfo,0,sizeof(HERO_TOTALINFO));
	memset(&Moveinfo,0,sizeof(BASEMOVE_INFO));	
	
	Objinfo.dwObjectID = atoi((char*)pData->Data[eCS_ObjectID]);
	Objinfo.dwUserID =  atoi((char*)pData->Data[eCS_UserID]);
	Objinfo.BattleID = 0;
	Objinfo.BattleTeam = 0;
	
	Heroinfo.PartyID = atoi((char*)pData->Data[eCS_PartyID]);
	Totalinfo.Gender = atoi((char*)pData->Data[eCS_Gender]);
	Heroinfo.wGenGol = atoi((char*)pData->Data[eCS_Gengoal]);
	Totalinfo.Life = atoi((char*)pData->Data[eCS_Life]);
	Totalinfo.Shield = atoi((char*)pData->Data[eCS_Shield]);
	Totalinfo.MaxLife = Totalinfo.Life;
	Totalinfo.MaxShield = Totalinfo.Shield;
	Heroinfo.wNaeRyuk = atoi((char*)pData->Data[eCS_Energe]);
	Heroinfo.wMinChub = atoi((char*)pData->Data[eCS_Dex]);
	Heroinfo.wCheRyuk = atoi((char*)pData->Data[eCS_Sta]);
	Heroinfo.wSimMek = atoi((char*)pData->Data[eCS_Simmak]);
	Heroinfo.Fame = atoi((char*)pData->Data[eCS_Fame]);
	Heroinfo.wKarma = atoi((char*)pData->Data[eCS_Karma]);
	SafeStrCpy( Objinfo.ObjectName, (char*)pData->Data[eCS_Name], MAX_NAME_LENGTH + 1 );
	Totalinfo.Stage = atoi((char*)pData->Data[eCS_Stage]);
	Totalinfo.Level = atoi((char*)pData->Data[eCS_Grade]);
	Heroinfo.ExpPoint = (EXPTYPE)_atoi64((char*)pData->Data[eCS_Expoint]);
	Heroinfo.LevelUpPoint = atoi((char*)pData->Data[eCS_GradeUpPoint]);
	Heroinfo.Money = atoi((char*)pData->Data[eCS_Money]);
	Totalinfo.CurMapNum = GAMERESRCMNGR->GetLoadMapNum();
	Totalinfo.LoginMapNum = atoi((char*)pData->Data[eCS_Map]);
	Totalinfo.FaceType = atoi((char*)pData->Data[eCS_FaceType]);
	Totalinfo.HairType = atoi((char*)pData->Data[eCS_HeadType]);
	Totalinfo.WearedItemIdx[eWearedItem_Hat] = atoi((char*)pData->Data[eCS_Hat]);
	Totalinfo.WearedItemIdx[eWearedItem_Weapon] = atoi((char*)pData->Data[eCS_Weapon]);
	Totalinfo.WearedItemIdx[eWearedItem_Dress] = atoi((char*)pData->Data[eCS_Dress]);
	Totalinfo.WearedItemIdx[eWearedItem_Shoes] = atoi((char*)pData->Data[eCS_Shoes]);
	Totalinfo.WearedItemIdx[eWearedItem_Ring1] = atoi((char*)pData->Data[eCS_Ring1]);
	Totalinfo.WearedItemIdx[eWearedItem_Ring2] = atoi((char*)pData->Data[eCS_Ring2]);
	Totalinfo.WearedItemIdx[eWearedItem_Cape] = atoi((char*)pData->Data[eCS_Cape]);
	Totalinfo.WearedItemIdx[eWearedItem_Necklace] = atoi((char*)pData->Data[eCS_Necklace]);
	Totalinfo.WearedItemIdx[eWearedItem_Armlet] = atoi((char*)pData->Data[eCS_Armlet]);
	Totalinfo.WearedItemIdx[eWearedItem_Belt] = atoi((char*)pData->Data[eCS_Belt]);
	Totalinfo.MunpaID = atoi((char*)pData->Data[eCS_MunpaIDX]);
	Totalinfo.PositionInMunpa = atoi((char*)pData->Data[eCS_GuildPositon]);
	SafeStrCpy(Totalinfo.GuildName, (char*)pData->Data[eCS_GuildName], MAX_GUILD_NAME+1);
	SafeStrCpy(Totalinfo.NickName, (char*)pData->Data[eCS_NickName], MAX_GUILD_NICKNAME+1);

	Totalinfo.BadFame = atoi((char*)pData->Data[eCS_BadFame]);

	// Set Mallmoney and GoldMoney

	Heroinfo.MallMoney =atoi((char*)pData->Data[eCS_MallMoney]);

	Heroinfo.GoldMoney =atoi((char*)pData->Data[eCS_GoldMoney]);

	Heroinfo.ReSet=atoi((char*)pData->Data[eCS_ReSet]);

    // set marryname 2014-05-09

	SafeStrCpy(Totalinfo.MarryName, (char*)pData->Data[eCS_MarryName], MAX_NAME_LENGTH + 1);

	// set shituname 2014-05-11

	Totalinfo.ShiTuId=atoi((char*)pData->Data[eCS_ShiTuId]);

	// 附加角色闪名标志信息 2015-01-22

	Totalinfo.FlashNameFlag=atoi((char*)pData->Data[eCS_FNameFlag]);

	SafeStrCpy(Totalinfo.FlashName,(char*)pData->Data[eCsFName],MAX_NAME_LENGTH +1);

	/**/

	Totalinfo.SafeStatic = atoi((char*)pData->Data[eCsSafe]); //weiye 附加安全锁标志 2018-02-01

	Totalinfo.TurnStageOne=atoi((char*)pData->Data[eCS_TurnOne]);
	Totalinfo.TurnStageTwo=atoi((char*)pData->Data[eCS_TurnTwo]);
	Totalinfo.TurnStageThree=atoi((char*)pData->Data[eCS_TurnThree]);

	

	SafeStrCpy(Totalinfo.ShiTuName, (char*)pData->Data[eCS_ShiTuName], MAX_NAME_LENGTH + 1);

	Totalinfo.MapChangePoint_Index = atoi((char*)pData->Data[eCS_MapChangePoint]);

	Totalinfo.LoginPoint_Index = atoi((char*)pData->Data[eCS_LoginPoint]);
	
	Heroinfo.KyungGongGrade = atoi((char*)pData->Data[eCS_QuickPace]);
	
	Heroinfo.AbilityExp = atoi((char*)pData->Data[eCS_AbilityExp]);
	
	Totalinfo.Height = (float)atof((char*)pData->Data[eCS_Height]);
	Totalinfo.Width = (float)atof((char*)pData->Data[eCS_Width]);
	Heroinfo.Playtime = atoi((char*)pData->Data[eCS_Playtime]);
	Heroinfo.LastPKModeEndTime = atoi((char*)pData->Data[eCS_LastPKEndTime]);

	Heroinfo.MaxLevel = atoi((char*)pData->Data[eCS_MaxLevel]);
	Totalinfo.MarkName	= atoi((char*)pData->Data[eCS_MarkName]);
	SafeStrCpy(Heroinfo.MunpaCanEntryDate, (char*)pData->Data[eCS_MunpaCanEntryDate], 11);
		
	if(Heroinfo.MaxLevel < Totalinfo.Level)
		Heroinfo.MaxLevel = Totalinfo.Level;

#ifdef _JAPAN_LOCAL_
	DWORD dwAttr = (DWORD)atoi((char*)pData->Data[eCS_NickName+1]);
	Totalinfo.nMainAttr = (int)LOWORD(dwAttr);
	Totalinfo.nSubAttr = (int)HIWORD(dwAttr);
#endif

	VECTOR3 pos,RandPos;
	GAMERESRCMNGR->GetRegenPointFromEventMap(pos,RandPos,Totalinfo.LoginPoint_Index,Totalinfo.MapChangePoint_Index,dwTeam);

	CPlayer* pPlayer = g_pServerSystem->InitPlayerInfo(&Objinfo, &Totalinfo, &Heroinfo);
	if( pPlayer == NULL )	return;

	pPlayer->SetLoginTime(Heroinfo.Playtime);

	pPlayer->SetPaoDianTime(atoi((char*)pData->Data[eCS_PdTime]));     // read the mallmoney time 2014-05-30

	pPlayer->SetEventLastMapTime(atoi((char*)pData->Data[eCS_LoginEventTime]));

	pPlayer->SetEventIDFlag(atoi((char*)pData->Data[eCS_LoginEventItem]));

#ifdef _SKILLSLOT_
	pPlayer->SetExtraMugongSlot(atoi((char*)pData->Data[eCS_SkillSlot]));
#else
	pPlayer->SetExtraMugongSlot(7);
#endif

	//pPlayer->SetFBTimeTo(atoi((char*)pData->Data[eCS_FBTime]));       //weiye 设置FB副本时间 2018-02-06

	/*pPlayer->SetTurnOne(atoi((char*)pData->Data[eCS_TurnOne]));
	pPlayer->SetTurnTwo(atoi((char*)pData->Data[eCS_TurnTwo]));
	pPlayer->SetTurnThree(atoi((char*)pData->Data[eCS_TurnThree]));*/
	//

	if(DungeonMGR->IsDungeon(g_pServerSystem->GetMapNum()))
	{
		if(VECTOR3* const pStartPos = DungeonMGR->GetStartPosFromeservationList(pPlayer->GetID()))
		{
			pPlayer->GetMoveInfo()->CurPosition = *pStartPos;
		}
		else if(pPlayer->IsDungeonObserver())
		{				
			pPlayer->GetMoveInfo()->CurPosition = *pPlayer->GetDungeonObserverPos();
		}
	}

	CCharMove::InitMove(pPlayer,&RandPos);
	
	pPlayer->SetInitState( PLAYERINITSTATE_HERO_INFO, MP_USERCONN_GAMEIN_SYN );
}

void RCharacterMugongInfo(LPQUERY pData, LPDBMESSAGE pMessage)
{
	
	DWORD count = pMessage->dwResult;
//	if(count > MAX_TOTALMUGONG_NUM)
	if(count > SLOT_MUGONGTOTAL_NUM)
	{
		// ⊥iIAIAIā∷㈢ AEāu⊥i¨∷⊥U.
	}
	else if((count == 1) && (atoi((char*)pData->Data[0]) == 0))
	{
		// ⊥iIAIAIā∷㈢ ¨uㄏǎ¨ui¨uㄏ PlayerID∷〓| ㄏoYE?CO āㄏ∷?iāEic
		CPlayer* pPlayer = (CPlayer *)g_pUserTable->FindUser(atoi((char*)pData->Data[1]));
		if(pPlayer == NULL)
			return;

		MUGONG_TOTALINFO Mugonginfo;
		memset(&Mugonginfo, 0, sizeof(MUGONG_TOTALINFO));
		pPlayer->InitMugongTotalInfo(&Mugonginfo);
		pPlayer->SetInitState(PLAYERINITSTATE_MUGONG_INFO,pMessage->dwID);
	}
	else
	{
		CPlayer* pPlayer = (CPlayer *)g_pUserTable->FindUser((DWORD)atoi((char*)pData[0].Data[eCM_ObjectID]));
		if(pPlayer == NULL)
			return;

		MUGONG_TOTALINFO Mugonginfo;
		const int maxoverlap = 100;
		int overlapcount = 0;
		MUGONGBASE OverLapMugong[maxoverlap];
		memset(&OverLapMugong, 0, sizeof(MUGONGBASE)*maxoverlap);
		memset(&Mugonginfo, 0, sizeof(MUGONG_TOTALINFO));
//		QUICKITEM quickItem;

		for(DWORD i = 0; i < count; i++)
		{
			WORD Mugongidx = (WORD)atoi((char*)pData[i].Data[eCM_IDX]);
			
			POSTYPE MugongPos = ConvAbsPos2MugongPos(atoi((char*)pData[i].Data[eCM_Position]));
			MUGONGBASE* pMugongBase = &Mugonginfo.mugong[MugongPos];

			//公傍 器瘤记捞 般闷促.
			if(pMugongBase->dwDBIdx != 0 && overlapcount < maxoverlap)
			{
				// magi82 - Titan(070611) 鸥捞藕 公傍函券 林籍贸府
				// 鸥捞藕 公傍牢郸胶绰 某腐磐公傍牢郸胶+10000 捞促
				//if(Mugongidx == pMugongBase->wIconIdx+10000 )
				//{
				//	pMugongBase = &Mugonginfo.Titanmugong[MugongPos];
				//}
				//else
				{
					pMugongBase = &OverLapMugong[overlapcount];
					++overlapcount;
				}
			}

			pMugongBase->wIconIdx = Mugongidx;//atoi((char*)pData[i].Data[eCM_IDX]);
			pMugongBase->dwDBIdx = atoi((char*)pData[i].Data[eCM_DBIDX]);
			pMugongBase->ExpPoint = atoi((char*)pData[i].Data[eCM_ExpPoint]);
			pMugongBase->Sung = atoi((char*)pData[i].Data[eCM_Sung]);
			pMugongBase->Position = atoi((char*)pData[i].Data[eCM_Position]);
			pMugongBase->QuickPosition = atoi((char*)pData[i].Data[eCM_QPosition]);
			pMugongBase->bWear = FALSE;

			//////////////////////////////////////////////////////////////////////////
			// 06. 06. 2瞒 傈流 - 捞康霖
			// 公傍 函券 眠啊
			pMugongBase->OptionIndex = atoi((char*)pData[i].Data[eCM_Option]);
			//////////////////////////////////////////////////////////////////////////
		}
		
		//吝汗等 公傍捞 乐栏搁 后沫俊 持绢霖促.
		for(int n=0;n<overlapcount;++n)
		{
			int m=0;
			for(m=0;m<SLOT_MUGONG_NUM;++m)
			{
				//后沫捞搁 持绢霖促.
				if(Mugonginfo.mugong[m].dwDBIdx == 0)
				{
					Mugonginfo.mugong[m] = OverLapMugong[n];
				#ifdef _JAPAN_LOCAL_
					Mugonginfo.mugong[m].Position = TP_MUGONG_START + m;
				#elif defined _HK_LOCAL_
					Mugonginfo.mugong[m].Position = TP_MUGONG_START + m;
				#elif defined _TL_LOCAL_
					Mugonginfo.mugong[m].Position = TP_MUGONG_START + m;
				#else
					Mugonginfo.mugong[m].Position = TP_MUGONG1_START + m;
				#endif
					break;
				}
			}

			//父距 后沫捞 绝栏搁 歹捞惑 贸府窍瘤 臼澜
			if(m==SLOT_MUGONG_NUM)
				break;
		}

		// Player∷?∷㈢ ㄏoiāㄏǎd¨〓∷〓 Aㄏ〓ā∷㈢
		pPlayer->InitMugongTotalInfo(&Mugonginfo);
		pPlayer->SetInitState(PLAYERINITSTATE_MUGONG_INFO,pMessage->dwID);
	}
}


void RCharacterItemSlotInfo(LPQUERY pData, LPDBMESSAGE pMessage)
{
	CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser( pMessage->dwID );
	if( !pPlayer )			return;

#ifdef _JAPAN_LOCAL_
	DWORD InvenSlot = (atoi((char*)pData->Data[0]));
	DWORD PyogukSlot = (atoi((char*)pData->Data[1]));
	DWORD MugongSlot = (atoi((char*)pData->Data[2]));
	DWORD CharacterSlot = (atoi((char*)pData->Data[3]));

	pPlayer->SetExtraSlotCount( PyogukSlot, InvenSlot, MugongSlot );
	pPlayer->SetExtraCharacterSlot( CharacterSlot );
#endif

#ifdef _HK_LOCAL_
	DWORD InvenSlot 	= (atoi((char*)pData->Data[0]));
	DWORD PyogukSlot 	= (atoi((char*)pData->Data[1]));
	DWORD MugongSlot 	= (atoi((char*)pData->Data[2]));
	DWORD CharacterSlot = (atoi((char*)pData->Data[3]));

	pPlayer->SetExtraSlotCount( PyogukSlot, InvenSlot, MugongSlot );
	pPlayer->SetExtraCharacterSlot( CharacterSlot );
#endif

#ifdef _TL_LOCAL_
	DWORD InvenSlot 	= (atoi((char*)pData->Data[0]));
	DWORD PyogukSlot 	= (atoi((char*)pData->Data[1]));
	DWORD MugongSlot 	= (atoi((char*)pData->Data[2]));
	DWORD CharacterSlot = (atoi((char*)pData->Data[3]));

	pPlayer->SetExtraSlotCount( PyogukSlot, InvenSlot, MugongSlot );
	pPlayer->SetExtraCharacterSlot( CharacterSlot );
#endif

	CharacterItemInfo( pPlayer->GetID(), MP_USERCONN_GAMEIN_SYN );
}


void RCharacterItemInfo(LPQUERY pData, LPDBMESSAGE pMessage)
{
	CPlayer* pPlayer;
	DWORD count = pMessage->dwResult;
	if((count == 1) && (atoi((char*)pData->Data[0]) == 0))
	{
		pPlayer = (CPlayer *)g_pUserTable->FindUser(atoi((char*)pData->Data[1]));
		if(pPlayer == NULL)
			return;

		ITEM_TOTALINFO Iteminfo;
		memset(&Iteminfo, 0, sizeof(ITEM_TOTALINFO));
		pPlayer->InitItemTotalInfo(&Iteminfo);
		ShopItemInvenInfo( pPlayer->GetID() );
	}
	else
	{
		pPlayer = (CPlayer *)g_pUserTable->FindUser((DWORD)atoi((char*)pData[0].Data[eCI_ObjectID]));
		if(pPlayer == NULL)
			return;

		ITEM_TOTALINFO Iteminfo;
		const int maxoverlap = 100;
		int overlapcount = 0;
		ITEMBASE OverLapItem[maxoverlap];
		memset(&OverLapItem, 0, sizeof(ITEMBASE)*maxoverlap);
		memset(&Iteminfo, 0, sizeof(ITEM_TOTALINFO));
		for(DWORD  i = 0; i < count; i++)
		{
			POSTYPE ItemPos = atoi((char*)pData[i].Data[eCI_Position]);
			ITEMBASE* pItemBase = NULL;
			if(ItemPos >= TP_INVENTORY_START && ItemPos < TP_INVENTORY_END)
			{
				ItemPos -= TP_INVENTORY_START;
				pItemBase = &Iteminfo.Inventory[ItemPos];
			}
			else if(ItemPos >= TP_WEAR_START && ItemPos < TP_WEAR_END)
			{	
				POSTYPE ItemGrid = ItemPos - TP_WEAR_START;

				pItemBase = &Iteminfo.WearedItem[ItemGrid];

				if( pItemBase )
				{
					pItemBase->ItemParam = atoi((char*)pData[i].Data[eCI_Param]);
					if( pItemBase->ItemParam & ITEM_PARAM_SEAL )
					{
						ITEM_INFO* pItemInfo = ITEMMGR->GetItemInfo( pItemBase->wIconIdx );

						if( pItemInfo )
						{
							if( pItemInfo->ItemKind & eSHOP_ITEM )
							{
								pItemBase->dwDBIdx = atoi((char*)pData[i].Data[eCI_DBIDX]);
								ItemMoveUpdateToDB( pPlayer->GetID(), 0, 0, pItemBase->dwDBIdx, 240 );
								continue;
							}
							else
							{
								pItemBase->ItemParam &= ~ITEM_PARAM_SEAL;
							}
						}
					}
				}
			}

			if(pItemBase == NULL)
			{
				//ASSERT(0);
				continue;
			}
			
			if(pItemBase->dwDBIdx != 0 && overlapcount < maxoverlap)
			{
				pItemBase = &OverLapItem[overlapcount];
				++overlapcount;
			}

			pItemBase->dwDBIdx = atoi((char*)pData[i].Data[eCI_DBIDX]);
			pItemBase->wIconIdx = atoi((char*)pData[i].Data[eCI_IDX]);
			pItemBase->Position = atoi((char*)pData[i].Data[eCI_Position]);
			pItemBase->Durability = atoi((char*)pData[i].Data[eCI_Durability]);
			pItemBase->RareIdx = atoi((char*)pData[i].Data[eCI_RareIdx]);
			pItemBase->QuickPosition = atoi((char*)pData[i].Data[eCI_QPosition]);
			pItemBase->ItemParam = atoi((char*)pData[i].Data[eCI_Param]);
			pItemBase->StoneIdx  = atoi((char*)pData[i].Data[eCI_StoneIdx]);
			pItemBase->ItemStatic= atoi((char*)pData[i].Data[eCI_Static]);
			pItemBase->ItemGrow  = atoi((char*)pData[i].Data[eCI_Grow]);

			SafeStrCpy(pItemBase->PowerUp,(char*)pData[i].Data[eCI_PowerUp], MAX_NAME_LENGTH+1);
			SafeStrCpy(pItemBase->Green,(char*)pData[i].Data[eCI_Green], MAX_NAME_LENGTH+1);
			pItemBase->Grade30 = atoi((char*)pData[i].Data[eCI_Grade30]);

			ITEM_INFO* pInfo = ITEMMGR->GetItemInfo(pItemBase->wIconIdx);
			if( pInfo && pInfo->ItemKind & eTITAN_EQUIPITEM )
			{
				TITAN_ENDURANCE_ITEMINFO* pEndurance = pPlayer->GetTitanManager()->GetTitanItemEnduranceInfo(pItemBase->dwDBIdx);
				if( !pEndurance )
				{
					pPlayer->GetTitanManager()->MakeNewEnduranceInfo(pPlayer, pItemBase, eExceptionInven);				
				}
			}
		}

		for(int n=0;n<overlapcount;++n)
		{
			int m=TP_INVENTORY_START;
			for(m=TP_INVENTORY_START;m<TP_INVENTORY_END;++m)
			{
				if(Iteminfo.Inventory[m].dwDBIdx == 0)
				{
					Iteminfo.Inventory[m] = OverLapItem[n];
					Iteminfo.Inventory[m].Position = m;
					break;
				}
			}

			if(m==TP_INVENTORY_END)
				break;
		}
		
		pPlayer->InitItemTotalInfo(&Iteminfo);
		ShopItemInvenInfo( pPlayer->GetID() );

		///ITEMMGR_OBJ->QuitStreetStall(pPlayer,FALSE);		
	}
}

void RPyogukItemRareOptionInfo(LPQUERY pData, LPDBMESSAGE pMessage)
{
	// CIㄏo AEāi
	CPlayer* pPlayer = NULL;
	pPlayer = (CPlayer *)g_pUserTable->FindUser(pMessage->dwID);
	if(pPlayer == NULL)
		return;

	ITEM_RARE_OPTION_INFO RareOptionInfo;
	for( DWORD  i = 0 ; i < pMessage->dwResult ; ++i )
	{
		RareOptionInfo.dwRareOptionIdx			= (DWORD)atoi((char*)pData[i].Data[ePROI_RareOptionID]);
		RareOptionInfo.dwItemDBIdx			= (DWORD)atoi((char*)pData[i].Data[ePROI_ItemDBID]);
		RareOptionInfo.GenGol				= (WORD)atoi((char*)pData[i].Data[ePROI_GenGol]);
		RareOptionInfo.MinChub				= (WORD)atoi((char*)pData[i].Data[ePROI_MinChub]);				
		RareOptionInfo.CheRyuk				= (WORD)atoi((char*)pData[i].Data[ePROI_CheRyuk]);				
		RareOptionInfo.SimMek				= (WORD)atoi((char*)pData[i].Data[ePROI_SimMek]);					
		RareOptionInfo.Life					= (DWORD)atoi((char*)pData[i].Data[ePROI_Life]);					
		RareOptionInfo.NaeRyuk				= (WORD)atoi((char*)pData[i].Data[ePROI_NaeRyuk]);				
		RareOptionInfo.Shield				= (DWORD)atoi((char*)pData[i].Data[ePROI_Shield]);					
		RareOptionInfo.AttrRegist.SetElement_Val(ATTR_FIRE,		(float)atoi((char*)pData[i].Data[ePROI_FireAttrRegist])/100.f );
		RareOptionInfo.AttrRegist.SetElement_Val(ATTR_WATER,	(float)atoi((char*)pData[i].Data[ePROI_WaterAttrRegist])/100.f );
		RareOptionInfo.AttrRegist.SetElement_Val(ATTR_TREE,		(float)atoi((char*)pData[i].Data[ePROI_TreeAttrRegist])/100.f );
		RareOptionInfo.AttrRegist.SetElement_Val(ATTR_IRON,		(float)atoi((char*)pData[i].Data[ePROI_GoldAttrRegist])/100.f );
		RareOptionInfo.AttrRegist.SetElement_Val(ATTR_EARTH,	(float)atoi((char*)pData[i].Data[ePROI_EarthAttrRegist])/100.f );
		RareOptionInfo.PhyAttack			= (WORD)atoi((char*)pData[i].Data[ePROI_PhyAttack]);
//		RareOptionInfo.CriticalPercent		= (WORD)atoi((char*)pData[i].Data[ePROI_Critical]);
		RareOptionInfo.AttrAttack.SetElement_Val(ATTR_FIRE,		(float)atoi((char*)pData[i].Data[ePROI_FireAttrAttack])/100.f );
		RareOptionInfo.AttrAttack.SetElement_Val(ATTR_WATER,	(float)atoi((char*)pData[i].Data[ePROI_WaterAttrAttack])/100.f );
		RareOptionInfo.AttrAttack.SetElement_Val(ATTR_TREE,		(float)atoi((char*)pData[i].Data[ePROI_TreeAttrAttack])/100.f );
		RareOptionInfo.AttrAttack.SetElement_Val(ATTR_IRON,		(float)atoi((char*)pData[i].Data[ePROI_GoldAttrAttack])/100.f );
		RareOptionInfo.AttrAttack.SetElement_Val(ATTR_EARTH,	(float)atoi((char*)pData[i].Data[ePROI_EarthAttrAttack])/100.f );
		RareOptionInfo.PhyDefense			= (WORD)atoi((char*)pData[i].Data[ePROI_PhyDefense]);				

		pPlayer->AddItemRareOption(&RareOptionInfo);
	}

	if(pMessage->dwResult >= MAX_PYOGUKLOAD_ITEM) 
		PyogukItemRareOptionInfo(pMessage->dwID, pPlayer->GetUserID(), RareOptionInfo.dwItemDBIdx);
	else
		PyogukTitanEnduranceInfo(pMessage->dwID, pPlayer->GetUserID(), 0);
		//CharacterPyogukItemInfo(pMessage->dwID, 0);
}

void RPyogukItemOptionInfo(LPQUERY pData, LPDBMESSAGE pMessage)
{
	// CIㄏo AEāi
	CPlayer* pPlayer = NULL;
	pPlayer = (CPlayer *)g_pUserTable->FindUser(pMessage->dwID);
	if(pPlayer == NULL)
		return;

	ITEM_OPTION_INFO OptionInfo;
	for( DWORD  i = 0 ; i < pMessage->dwResult ; ++i )
	{
		OptionInfo.dwOptionIdx			= (DWORD)atoi((char*)pData[i].Data[ePOI_OptionID]);
		OptionInfo.dwItemDBIdx			= (DWORD)atoi((char*)pData[i].Data[ePOI_ItemDBID]);
		OptionInfo.GenGol				= (WORD)atoi((char*)pData[i].Data[ePOI_GenGol]);
		OptionInfo.MinChub				= (WORD)atoi((char*)pData[i].Data[ePOI_MinChub]);				
		OptionInfo.CheRyuk				= (WORD)atoi((char*)pData[i].Data[ePOI_CheRyuk]);				
		OptionInfo.SimMek				= (WORD)atoi((char*)pData[i].Data[ePOI_SimMek]);					
		OptionInfo.Life					= (DWORD)atoi((char*)pData[i].Data[ePOI_Life]);					
		OptionInfo.NaeRyuk				= (WORD)atoi((char*)pData[i].Data[ePOI_NaeRyuk]);				
		OptionInfo.Shield				= (DWORD)atoi((char*)pData[i].Data[ePOI_Shield]);					
		OptionInfo.AttrRegist.SetElement_Val(ATTR_FIRE,		(float)atoi((char*)pData[i].Data[ePOI_FireAttrRegist])/100.f );
		OptionInfo.AttrRegist.SetElement_Val(ATTR_WATER,	(float)atoi((char*)pData[i].Data[ePOI_WaterAttrRegist])/100.f );
		OptionInfo.AttrRegist.SetElement_Val(ATTR_TREE,		(float)atoi((char*)pData[i].Data[ePOI_TreeAttrRegist])/100.f );
		OptionInfo.AttrRegist.SetElement_Val(ATTR_IRON,		(float)atoi((char*)pData[i].Data[ePOI_GoldAttrRegist])/100.f );
		OptionInfo.AttrRegist.SetElement_Val(ATTR_EARTH,	(float)atoi((char*)pData[i].Data[ePOI_EarthAttrRegist])/100.f );
		OptionInfo.PhyAttack			= (WORD)atoi((char*)pData[i].Data[ePOI_PhyAttack]);
		OptionInfo.CriticalPercent		= (WORD)atoi((char*)pData[i].Data[ePOI_Critical]);
		OptionInfo.AttrAttack.SetElement_Val(ATTR_FIRE,		(float)atoi((char*)pData[i].Data[ePOI_FireAttrAttack])/100.f );
		OptionInfo.AttrAttack.SetElement_Val(ATTR_WATER,	(float)atoi((char*)pData[i].Data[ePOI_WaterAttrAttack])/100.f );
		OptionInfo.AttrAttack.SetElement_Val(ATTR_TREE,		(float)atoi((char*)pData[i].Data[ePOI_TreeAttrAttack])/100.f );
		OptionInfo.AttrAttack.SetElement_Val(ATTR_IRON,		(float)atoi((char*)pData[i].Data[ePOI_GoldAttrAttack])/100.f );
		OptionInfo.AttrAttack.SetElement_Val(ATTR_EARTH,	(float)atoi((char*)pData[i].Data[ePOI_EarthAttrAttack])/100.f );
		OptionInfo.PhyDefense			= (WORD)atoi((char*)pData[i].Data[ePOI_PhyDefense]);				

		pPlayer->AddItemOption(&OptionInfo);
	}

	if(pMessage->dwResult >= MAX_PYOGUKLOAD_ITEM)
	{
		PyogukItemOptionInfo(pMessage->dwID, pPlayer->GetUserID(), OptionInfo.dwItemDBIdx);
	}
	else
	{
		PyogukItemRareOptionInfo(pPlayer->GetID(), pPlayer->GetUserID(), 0);
	}
	//SW051007 PyogukItemRareOptionInfo 肺 颗辫.
//	else
//		CharacterPyogukItemInfo(pMessage->dwID, 0);
}

// 访问仓库回调函数 2015-01-05

void   RPyogukItemStoneInfo(LPQUERY pData, LPDBMESSAGE pMessage)
{
	CPlayer* pPlayer = NULL;
	pPlayer = (CPlayer *)g_pUserTable->FindUser(pMessage->dwID);
	if(pPlayer == NULL)
		return;

	ITEM_STONE_OPTION_INFO StoneInfo;

	for( DWORD  i = 0 ; i < pMessage->dwResult ; ++i )
	{
		StoneInfo.dwStoneOptionIdx	= (DWORD)atoi((char*)pData[i].Data[ePyST_StoneID]);

		StoneInfo.dwItemDBIdx       = (DWORD)atoi((char*)pData[i].Data[ePyST_ItemDBID]);

		StoneInfo.dwItemStone0		= (DWORD)atoi((char*)pData[i].Data[ePyST_Stone0]);

		StoneInfo.dwItemStone1		= (DWORD)atoi((char*)pData[i].Data[ePyST_Stone1]);

		StoneInfo.dwItemStone2		= (DWORD)atoi((char*)pData[i].Data[ePyST_Stone2]);

		StoneInfo.dwItemStone3		= (DWORD)atoi((char*)pData[i].Data[ePyST_Stone3]);

		StoneInfo.dwItemStone4		= (DWORD)atoi((char*)pData[i].Data[ePyST_Stone4]);

		pPlayer->AddItemStoneOption(&StoneInfo);
	}

	if(pMessage->dwResult >= MAX_PYOGUKLOAD_ITEM)
	{
		PyogukItemStoneInfo(pMessage->dwID, pPlayer->GetUserID(), StoneInfo.dwItemDBIdx);
	}
}

void	RCharacterItemRareOptionInfo(LPQUERY pData, LPDBMESSAGE pMessage)//first
{
	if(pMessage->dwResult == 0) return;

	CPlayer* pPlayer = NULL;
	pPlayer = (CPlayer*)g_pUserTable->FindUser(atoi((char*)pData->Data[eCIRO_ObjectID]));
	if(pPlayer == NULL)
		return;

	if(pMessage->dwResult == 1 && atoi((char*)pData->Data[eCIRO_RareOptionID]) == 0)
	{
		pPlayer->SetInitState(PLAYERINITSTATE_ITEM_RARE_OPTION_INFO, pMessage->dwID);
	}
	else
	{
		//ASSERTMSG(pMessage->dwResult <= 90, "Item Rare Option > 90 Error" );

		ITEM_RARE_OPTION_INFO RareOptionInfo;
		for( DWORD i = 0; i < pMessage->dwResult; ++ i )
		{
			RareOptionInfo.dwRareOptionIdx			= (DWORD)atoi((char*)pData[i].Data[eCIRO_RareOptionID]);
			RareOptionInfo.dwItemDBIdx			= (DWORD)atoi((char*)pData[i].Data[eCIRO_ItemDBID]);
			RareOptionInfo.GenGol				= (WORD)atoi((char*)pData[i].Data[eCIRO_GenGol]);
			RareOptionInfo.MinChub				= (WORD)atoi((char*)pData[i].Data[eCIRO_MinChub]);				
			RareOptionInfo.CheRyuk				= (WORD)atoi((char*)pData[i].Data[eCIRO_CheRyuk]);				
			RareOptionInfo.SimMek				= (WORD)atoi((char*)pData[i].Data[eCIRO_SimMek]);					
			RareOptionInfo.Life					= (DWORD)atoi((char*)pData[i].Data[eCIRO_Life]);					
			RareOptionInfo.NaeRyuk				= (WORD)atoi((char*)pData[i].Data[eCIRO_NaeRyuk]);				
			RareOptionInfo.Shield				= (DWORD)atoi((char*)pData[i].Data[eCIRO_Shield]);					
			RareOptionInfo.AttrRegist.SetElement_Val(ATTR_FIRE,		(float)atoi((char*)pData[i].Data[eCIRO_FireAttrRegist])/100.f );
			RareOptionInfo.AttrRegist.SetElement_Val(ATTR_WATER,	(float)atoi((char*)pData[i].Data[eCIRO_WaterAttrRegist])/100.f );
			RareOptionInfo.AttrRegist.SetElement_Val(ATTR_TREE,		(float)atoi((char*)pData[i].Data[eCIRO_TreeAttrRegist])/100.f );
			RareOptionInfo.AttrRegist.SetElement_Val(ATTR_IRON,		(float)atoi((char*)pData[i].Data[eCIRO_GoldAttrRegist])/100.f );
			RareOptionInfo.AttrRegist.SetElement_Val(ATTR_EARTH,	(float)atoi((char*)pData[i].Data[eCIRO_EarthAttrRegist])/100.f );
			RareOptionInfo.PhyAttack			= (WORD)atoi((char*)pData[i].Data[eCIRO_PhyAttack]);
//			RareOptionInfo.CriticalPercent		= (WORD)atoi((char*)pData[i].Data[eCIOI_Critical]);
			RareOptionInfo.AttrAttack.SetElement_Val(ATTR_FIRE,		(float)atoi((char*)pData[i].Data[eCIRO_FireAttrAttack])/100.f );
			RareOptionInfo.AttrAttack.SetElement_Val(ATTR_WATER,	(float)atoi((char*)pData[i].Data[eCIRO_WaterAttrAttack])/100.f );
			RareOptionInfo.AttrAttack.SetElement_Val(ATTR_TREE,		(float)atoi((char*)pData[i].Data[eCIRO_TreeAttrAttack])/100.f );
			RareOptionInfo.AttrAttack.SetElement_Val(ATTR_IRON,		(float)atoi((char*)pData[i].Data[eCIRO_GoldAttrAttack])/100.f );
			RareOptionInfo.AttrAttack.SetElement_Val(ATTR_EARTH,	(float)atoi((char*)pData[i].Data[eCIRO_EarthAttrAttack])/100.f );
			RareOptionInfo.PhyDefense			= (WORD)atoi((char*)pData[i].Data[eCIRO_PhyDefense]);

			ITEMBASE info;
			SafeStrCpy(info.PowerUp,(char*)pData[0].Data[eCIRO_PowerUp], MAX_NAME_LENGTH+1);
			SafeStrCpy(info.Green,(char*)pData[0].Data[eCIRO_Green], MAX_NAME_LENGTH+1);

			pPlayer->AddItemRareOption(&RareOptionInfo);
		}
		pPlayer->SetInitState(PLAYERINITSTATE_ITEM_RARE_OPTION_INFO,pMessage->dwID);

	}
}

void RCharacterItemOptionInfo(LPQUERY pData, LPDBMESSAGE pMessage)
{
	if(pMessage->dwResult == 0) return;

	CPlayer* pPlayer = NULL;
	pPlayer = (CPlayer *)g_pUserTable->FindUser(atoi((char*)pData->Data[eCIOI_ObjectID]));
	if(pPlayer == NULL)
		return;

	// AI¨〓IAa∷〓∷c
	if(pMessage->dwResult == 1 && atoi((char*)pData->Data[eCIOI_OptionID]) == 0)
	{
		pPlayer->SetInitState(PLAYERINITSTATE_ITEM_OPTION_INFO,pMessage->dwID);
	}
	else
	{
		//ASSERTMSG(pMessage->dwResult <= 90, "Item Option > 90 Error : ㄏoI∷?i; ¨〓O⒋?¨〓∷〓¨u¨∷?a~!");

		ITEM_OPTION_INFO OptionInfo;
		for( DWORD  i = 0 ; i < pMessage->dwResult ; ++i )
		{
			OptionInfo.dwOptionIdx			= (DWORD)atoi((char*)pData[i].Data[eCIOI_OptionID]);
			OptionInfo.dwItemDBIdx			= (DWORD)atoi((char*)pData[i].Data[eCIOI_ItemDBID]);
			OptionInfo.GenGol				= (WORD)atoi((char*)pData[i].Data[eCIOI_GenGol]);
			OptionInfo.MinChub				= (WORD)atoi((char*)pData[i].Data[eCIOI_MinChub]);				
			OptionInfo.CheRyuk				= (WORD)atoi((char*)pData[i].Data[eCIOI_CheRyuk]);				
			OptionInfo.SimMek				= (WORD)atoi((char*)pData[i].Data[eCIOI_SimMek]);					
			OptionInfo.Life					= (DWORD)atoi((char*)pData[i].Data[eCIOI_Life]);					
			OptionInfo.NaeRyuk				= (WORD)atoi((char*)pData[i].Data[eCIOI_NaeRyuk]);				
			OptionInfo.Shield				= (DWORD)atoi((char*)pData[i].Data[eCIOI_Shield]);					
			OptionInfo.AttrRegist.SetElement_Val(ATTR_FIRE,		(float)atoi((char*)pData[i].Data[eCIOI_FireAttrRegist])/100.f );
			OptionInfo.AttrRegist.SetElement_Val(ATTR_WATER,	(float)atoi((char*)pData[i].Data[eCIOI_WaterAttrRegist])/100.f );
			OptionInfo.AttrRegist.SetElement_Val(ATTR_TREE,		(float)atoi((char*)pData[i].Data[eCIOI_TreeAttrRegist])/100.f );
			OptionInfo.AttrRegist.SetElement_Val(ATTR_IRON,		(float)atoi((char*)pData[i].Data[eCIOI_GoldAttrRegist])/100.f );
			OptionInfo.AttrRegist.SetElement_Val(ATTR_EARTH,	(float)atoi((char*)pData[i].Data[eCIOI_EarthAttrRegist])/100.f );
			OptionInfo.PhyAttack			= (WORD)atoi((char*)pData[i].Data[eCIOI_PhyAttack]);
			OptionInfo.CriticalPercent		= (WORD)atoi((char*)pData[i].Data[eCIOI_Critical]);
			OptionInfo.AttrAttack.SetElement_Val(ATTR_FIRE,		(float)atoi((char*)pData[i].Data[eCIOI_FireAttrAttack])/100.f );
			OptionInfo.AttrAttack.SetElement_Val(ATTR_WATER,	(float)atoi((char*)pData[i].Data[eCIOI_WaterAttrAttack])/100.f );
			OptionInfo.AttrAttack.SetElement_Val(ATTR_TREE,		(float)atoi((char*)pData[i].Data[eCIOI_TreeAttrAttack])/100.f );
			OptionInfo.AttrAttack.SetElement_Val(ATTR_IRON,		(float)atoi((char*)pData[i].Data[eCIOI_GoldAttrAttack])/100.f );
			OptionInfo.AttrAttack.SetElement_Val(ATTR_EARTH,	(float)atoi((char*)pData[i].Data[eCIOI_EarthAttrAttack])/100.f );
			OptionInfo.PhyDefense			= (WORD)atoi((char*)pData[i].Data[eCIOI_PhyDefense]);				

			pPlayer->AddItemOption(&OptionInfo);

			ITEMBASE info;
			SafeStrCpy(info.PowerUp,(char*)pData[0].Data[eCIOI_PowerUp], MAX_NAME_LENGTH+1);
		}

		pPlayer->SetInitState(PLAYERINITSTATE_ITEM_OPTION_INFO,pMessage->dwID);
	}

}

// 新存储对象 2014-11-14

void RCharacterItemStoneOptionInfo(LPQUERY pData, LPDBMESSAGE pMessage)
{
	if(pMessage->dwResult == 0) return;

	CPlayer* pPlayer = NULL;
	pPlayer = (CPlayer *)g_pUserTable->FindUser(atoi((char*)pData->Data[eCIST_ObjectID]));
	if(pPlayer == NULL)
		return;

	if(pMessage->dwResult == 1 && atoi((char*)pData->Data[eCIST_StoneID]) == 0)
	{
		pPlayer->SetInitState(PLAYERINITSTATE_ITEM_STONE_INFO,pMessage->dwID);
	}
	else
	{
		//ASSERTMSG(pMessage->dwResult <= MAX_ITEM_OPTION_NUM, "Item Stone > MAX_ITEM_OPTION_NUM Error : The Item Stone Over 90");

		ITEM_STONE_OPTION_INFO StoneInfo;

		for( DWORD  i = 0 ; i < pMessage->dwResult ; ++i )
		{
			StoneInfo.dwStoneOptionIdx	= (DWORD)atoi((char*)pData[i].Data[eCIST_StoneID]);

			StoneInfo.dwItemDBIdx       = (DWORD)atoi((char*)pData[i].Data[eCIST_ItemDBID]);

			StoneInfo.dwItemStone0		= (DWORD)atoi((char*)pData[i].Data[eCIST_Stone0]);

			StoneInfo.dwItemStone1		= (DWORD)atoi((char*)pData[i].Data[eCIST_Stone1]);

			StoneInfo.dwItemStone2		= (DWORD)atoi((char*)pData[i].Data[eCIST_Stone2]);

			StoneInfo.dwItemStone3		= (DWORD)atoi((char*)pData[i].Data[eCIST_Stone3]);

			StoneInfo.dwItemStone4		= (DWORD)atoi((char*)pData[i].Data[eCIST_Stone4]);

			pPlayer->AddItemStoneOption(&StoneInfo);
		}

		pPlayer->SetInitState(PLAYERINITSTATE_ITEM_STONE_INFO,pMessage->dwID);
	}

}
void RItemStoneInsertToDB(LPQUERY pData, LPDBMESSAGE pMessage)
{
	if( pMessage->dwResult )
	{

		CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser((DWORD)atoi((char*)pData[0].Data[eCIST_ObjectID]));
		if(pPlayer == NULL)
			return;

	
		ITEM_STONE_OPTION_INFO StoneInfo;
		StoneInfo.dwStoneOptionIdx				= (DWORD)atoi((char*)pData[0].Data[eCIST_StoneID]);	
		StoneInfo.dwItemDBIdx					= (DWORD)atoi((char*)pData[0].Data[eCIST_ItemDBID]);	
		StoneInfo.dwItemStone0					= (DWORD)atoi((char*)pData[0].Data[eCIST_Stone0]);
		StoneInfo.dwItemStone1					= (DWORD)atoi((char*)pData[0].Data[eCIST_Stone1]);
		StoneInfo.dwItemStone2					= (DWORD)atoi((char*)pData[0].Data[eCIST_Stone2]);
		StoneInfo.dwItemStone3					= (DWORD)atoi((char*)pData[0].Data[eCIST_Stone3]);
		StoneInfo.dwItemStone4					= (DWORD)atoi((char*)pData[0].Data[eCIST_Stone4]);

		pPlayer->AddItemStoneOption(&StoneInfo);

		ITEMMGR->StoneItemDBResult(pPlayer, StoneInfo.dwStoneOptionIdx ,(DWORD)atoi((char*)pData[0].Data[eCIST_ItemPos]), &StoneInfo);
	}
	else
	{
		ASSERT(0);
	}
}
void RItemStoneUpdateToDB(LPQUERY pData, LPDBMESSAGE pMessage)
{
	if( pMessage->dwResult )
	{

		CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser((DWORD)atoi((char*)pData[0].Data[eCIST_ObjectID]));
		if(pPlayer == NULL)
			return;

	
		ITEM_STONE_OPTION_INFO StoneInfo;
		StoneInfo.dwStoneOptionIdx				= (DWORD)atoi((char*)pData[0].Data[eCIST_StoneID]);	
		StoneInfo.dwItemDBIdx					= (DWORD)atoi((char*)pData[0].Data[eCIST_ItemDBID]);	
		StoneInfo.dwItemStone0					= (DWORD)atoi((char*)pData[0].Data[eCIST_Stone0]);
		StoneInfo.dwItemStone1					= (DWORD)atoi((char*)pData[0].Data[eCIST_Stone1]);
		StoneInfo.dwItemStone2					= (DWORD)atoi((char*)pData[0].Data[eCIST_Stone2]);
		StoneInfo.dwItemStone3					= (DWORD)atoi((char*)pData[0].Data[eCIST_Stone3]);
		StoneInfo.dwItemStone4					= (DWORD)atoi((char*)pData[0].Data[eCIST_Stone4]);

		pPlayer->AddItemStoneOption(&StoneInfo);

		ITEMMGR->StoneItemDBResultEx(pPlayer, StoneInfo.dwStoneOptionIdx ,(DWORD)atoi((char*)pData[0].Data[eCIST_ItemPos]), &StoneInfo);
	}
	else
	{
		ASSERT(0);
	}
}

void RCharacterAbilityInfo(LPMIDDLEQUERY pData, LPDBMESSAGE pMessage)
{
	ASSERT(pMessage->dwResult == 1);
	DWORD CharacterIdx = atoi((char*)pData[0].Data[0]);
	CPlayer* pPlayer = (CPlayer *)g_pUserTable->FindUser(CharacterIdx);
	if(pPlayer==NULL)
		return;
	int datacount = 0;

	ABILITY_TOTALINFO TInfo;
	for(int kind = 0;kind<eAGK_Max;++kind)
	{
		memcpy(TInfo.AbilityDataArray[kind],(char*)pData[0].Data[kind+1],MAX_ABILITY_NUM_PER_GROUP+1);
		memcpy(TInfo.AbilityQuickPositionArray[kind],(char*)pData[0].Data[eAGK_Max+kind+1],MAX_ABILITY_NUM_PER_GROUP+1);
	}

	ABILITYMGR->InitAbility(pPlayer,&TInfo,pPlayer->GetAbilityGroup());

	pPlayer->SetInitState(PLAYERINITSTATE_ABILITY_INFO,pMessage->dwID);
}

void PetInsertToDB( DWORD CharacterIdx, DWORD UserIdx, DWORD dwItemIdx, PET_TOTALINFO* pPetTotalInfo )
{
	sprintf(txt, "EXEC %s %d,%d,%d,%d,%d, %d,%d,%d,%d,%d, %d", "dbo.MP_PET_Insert",
		CharacterIdx,
		UserIdx,
		dwItemIdx,
		//dwItemDBIdx,
		//pPetTotalInfo->PetMasterUserID,
		pPetTotalInfo->PetKind,
		pPetTotalInfo->PetGrade,
		pPetTotalInfo->PetStamina,

		pPetTotalInfo->PetFriendly,
		pPetTotalInfo->PetSummonItemDBIdx,
		pPetTotalInfo->bAlive,
		pPetTotalInfo->bSummonning,
		pPetTotalInfo->bRest
		);

	g_DB.Query(eQueryType_FreeQuery, ePetInsert, pPetTotalInfo->PetSummonItemDBIdx, txt);
}

void RPetInsert(LPQUERY pData, LPDBMESSAGE pMessage)
{
	//static DWORD dwPetObjectID = PET_ID_START;

	//WORD lastNo = LOWORD(pMessage->dwID);
	//WORD ArrayID = HIWORD(pMessage->dwID);

	//敲饭捞绢啊 乐绰瘤
	CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser(atoi((char*)pData->Data[ePII_MasterObjectID]));
	if( NULL == pPlayer )
		return;
	if( FALSE == pPlayer->GetInited() )
		return;

	//!!!家券酒捞袍捞 乐绰瘤
	//ePTI_PetSummonItemIdx

	PET_TOTALINFO PetInfo;
	if( pMessage->dwResult )
	{
		PetInfo.PetMasterUserID = atoi((char*)pData->Data[ePII_UserIdx]);
		PetInfo.PetDBIdx	= atoi((char*)pData->Data[ePII_PetDBIdx]);
		PetInfo.PetSummonItemDBIdx = atoi((char*)pData->Data[ePII_PetSummonItemDBIdx]);
		PetInfo.PetKind = atoi((char*)pData->Data[ePII_PetKind]);
		PetInfo.PetGrade = atoi((char*)pData->Data[ePII_PetGrade]);
		PetInfo.PetStamina = atoi((char*)pData->Data[ePII_PetStamina]);
		PetInfo.PetFriendly = atoi((char*)pData->Data[ePII_PetFriendship]);
		PetInfo.bAlive = atoi((char*)pData->Data[ePII_Alive]);
		PetInfo.bSummonning = atoi((char*)pData->Data[ePII_Summonning]);
		PetInfo.bRest = atoi((char*)pData->Data[ePII_Rest]);
		//dwPetObjectID = PetInfo.PetDBIdx + PET_ID_START;
	}
	else
	{
		ASSERT(0);
	}

	pPlayer->GetPetManager()->AddPetTotalInfo(&PetInfo,eServerNClient);
	//脐 积己苞 沥焊 檬扁拳
	/*
	CPet* pPet = g_pServerSystem->AddPet(pPlayer, dwPetObjectID, &PetInfo);
	if(pPet)
	{
		pPlayer->GetPetManager()->AddPet(pPet,PetInfo.PetSummonItemDBIdx);
	}*/
}

void PetDeleteToDB( DWORD dwPetDBIdx )
{
	sprintf(txt, "EXEC dbo.MP_PET_Delete %d", dwPetDBIdx);
	g_DB.Query(eQueryType_FreeQuery, ePetDelete, 0, txt);
}

void PetUpdateToDB( DWORD UserIdx, const PET_TOTALINFO* pPetTotalInfo )
{
	//盎脚等 脐 沥焊 DB 历厘.
	sprintf(txt, "EXEC dbo.MP_PET_Update %d, %d, %d, %d, %d,  %d, %d, %d",
		UserIdx,
		//dwItemDBIdx,
		//pPetTotalInfo->PetMasterID,
		pPetTotalInfo->PetDBIdx,
		//pPetTotalInfo->PetKind,
		pPetTotalInfo->PetGrade,
		pPetTotalInfo->PetStamina,
		pPetTotalInfo->PetFriendly,
		//pPetTotalInfo->PetSummonItemID,
		pPetTotalInfo->bAlive,
		pPetTotalInfo->bSummonning,
		pPetTotalInfo->bRest
		);

	g_DB.Query(eQueryType_FreeQuery, ePetUpdate, 0, txt);
}

void RCharacterPetInfo(LPQUERY pData, LPDBMESSAGE pMessage)
{
	CPlayer* pPlayer = NULL;
	pPlayer = (CPlayer*)g_pUserTable->FindUser(pMessage->dwID);
	if(pPlayer == NULL)
		return;

	PET_TOTALINFO PetInfo;
	DWORD dwPetObjectID = 0;

	for( DWORD  i = 0 ; i < pMessage->dwResult ; ++i )
	{
		PetInfo.PetMasterUserID			=	atoi((char*)pData[i].Data[ePTI_UserID]);
		PetInfo.PetDBIdx			=	atoi((char*)pData[i].Data[ePTI_PetDBIdx]);	
		PetInfo.PetSummonItemDBIdx	=	atoi((char*)pData[i].Data[ePTI_PetSummonItemDBIdx]);
		PetInfo.PetKind				=	atoi((char*)pData[i].Data[ePTI_PetKind]);
		PetInfo.PetGrade			=	atoi((char*)pData[i].Data[ePTI_PetGrade]);
		PetInfo.PetStamina			=	atoi((char*)pData[i].Data[ePTI_PetStamina]);
		PetInfo.PetFriendly			=	atoi((char*)pData[i].Data[ePTI_PetFriendship]);
		PetInfo.bAlive				=	atoi((char*)pData[i].Data[ePTI_Alive]);
		PetInfo.bSummonning			=	atoi((char*)pData[i].Data[ePTI_Summonning]);
		PetInfo.bRest				=	atoi((char*)pData[i].Data[ePTI_Rest]);

		pPlayer->GetPetManager()->AddPetTotalInfo(&PetInfo);

	}
	pPlayer->SetInitState(PLAYERINITSTATE_PET_INFO,MP_USERCONN_GAMEIN_SYN);

}

void RPetInvenItemOptionInfo( LPQUERY pData, LPDBMESSAGE pMessage )
{
	CPlayer* pPlayer = NULL;
	pPlayer = (CPlayer*)g_pUserTable->FindUser(pMessage->dwID);
	if(pPlayer == NULL)
		return;

	ITEM_OPTION_INFO OptionInfo;
	for( DWORD i = 0; i < pMessage->dwResult; ++i )
	{
		OptionInfo.dwOptionIdx			= (DWORD)atoi((char*)pData[i].Data[ePIOI_OptionID]);
		if(0 == OptionInfo.dwOptionIdx)		continue;
		OptionInfo.dwItemDBIdx			= (DWORD)atoi((char*)pData[i].Data[ePIOI_ItemDBID]);
		OptionInfo.GenGol				= (WORD)atoi((char*)pData[i].Data[ePIOI_GenGol]);
		OptionInfo.MinChub				= (WORD)atoi((char*)pData[i].Data[ePIOI_MinChub]);				
		OptionInfo.CheRyuk				= (WORD)atoi((char*)pData[i].Data[ePIOI_CheRyuk]);				
		OptionInfo.SimMek				= (WORD)atoi((char*)pData[i].Data[ePIOI_SimMek]);					
		OptionInfo.Life					= (DWORD)atoi((char*)pData[i].Data[ePIOI_Life]);					
		OptionInfo.NaeRyuk				= (WORD)atoi((char*)pData[i].Data[ePIOI_NaeRyuk]);				
		OptionInfo.Shield				= (DWORD)atoi((char*)pData[i].Data[ePIOI_Shield]);					
		OptionInfo.AttrRegist.SetElement_Val(ATTR_FIRE,		(float)atoi((char*)pData[i].Data[ePIOI_FireAttrRegist])/100.f );
		OptionInfo.AttrRegist.SetElement_Val(ATTR_WATER,	(float)atoi((char*)pData[i].Data[ePIOI_WaterAttrRegist])/100.f );
		OptionInfo.AttrRegist.SetElement_Val(ATTR_TREE,		(float)atoi((char*)pData[i].Data[ePIOI_TreeAttrRegist])/100.f );
		OptionInfo.AttrRegist.SetElement_Val(ATTR_IRON,		(float)atoi((char*)pData[i].Data[ePIOI_GoldAttrRegist])/100.f );
		OptionInfo.AttrRegist.SetElement_Val(ATTR_EARTH,	(float)atoi((char*)pData[i].Data[ePIOI_EarthAttrRegist])/100.f );
		OptionInfo.PhyAttack			= (WORD)atoi((char*)pData[i].Data[ePIOI_PhyAttack]);
		OptionInfo.CriticalPercent		= (WORD)atoi((char*)pData[i].Data[ePIOI_Critical]);
		OptionInfo.AttrAttack.SetElement_Val(ATTR_FIRE,		(float)atoi((char*)pData[i].Data[ePIOI_FireAttrAttack])/100.f );
		OptionInfo.AttrAttack.SetElement_Val(ATTR_WATER,	(float)atoi((char*)pData[i].Data[ePIOI_WaterAttrAttack])/100.f );
		OptionInfo.AttrAttack.SetElement_Val(ATTR_TREE,		(float)atoi((char*)pData[i].Data[ePIOI_TreeAttrAttack])/100.f );
		OptionInfo.AttrAttack.SetElement_Val(ATTR_IRON,		(float)atoi((char*)pData[i].Data[ePIOI_GoldAttrAttack])/100.f );
		OptionInfo.AttrAttack.SetElement_Val(ATTR_EARTH,	(float)atoi((char*)pData[i].Data[ePIOI_EarthAttrAttack])/100.f );
		OptionInfo.PhyDefense			= (WORD)atoi((char*)pData[i].Data[ePIOI_PhyDefense]);				

		pPlayer->AddItemOption(&OptionInfo);
	}

	if(pMessage->dwResult >= MAX_PETINVENLOAD_ITEM)
	{
		PetInvenItemOptionInfo(pMessage->dwID, pPlayer->GetUserID(), TP_PETINVEN_START, TP_PETINVEN_END);
	}
	else
	{
		PetInvenItemRareOptionInfo(pMessage->dwID, pPlayer->GetUserID(), TP_PETINVEN_START, TP_PETINVEN_END);
	}
}

void RPetInvenItemRareOptionInfo( LPQUERY pData, LPDBMESSAGE pMessage )
{
	CPlayer* pPlayer = NULL;
	pPlayer = (CPlayer *)g_pUserTable->FindUser(pMessage->dwID);
	if(pPlayer == NULL)
		return;

	ITEM_RARE_OPTION_INFO RareOptionInfo;
	for( DWORD  i = 0 ; i < pMessage->dwResult ; ++i )
	{
		RareOptionInfo.dwRareOptionIdx			= (DWORD)atoi((char*)pData[i].Data[ePIROI_RareOptionID]);
		if(0 == RareOptionInfo.dwRareOptionIdx)	continue;
		RareOptionInfo.dwItemDBIdx			= (DWORD)atoi((char*)pData[i].Data[ePIROI_ItemDBID]);
		RareOptionInfo.GenGol				= (WORD)atoi((char*)pData[i].Data[ePIROI_GenGol]);
		RareOptionInfo.MinChub				= (WORD)atoi((char*)pData[i].Data[ePIROI_MinChub]);				
		RareOptionInfo.CheRyuk				= (WORD)atoi((char*)pData[i].Data[ePIROI_CheRyuk]);				
		RareOptionInfo.SimMek				= (WORD)atoi((char*)pData[i].Data[ePIROI_SimMek]);					
		RareOptionInfo.Life					= (DWORD)atoi((char*)pData[i].Data[ePIROI_Life]);					
		RareOptionInfo.NaeRyuk				= (WORD)atoi((char*)pData[i].Data[ePIROI_NaeRyuk]);				
		RareOptionInfo.Shield				= (DWORD)atoi((char*)pData[i].Data[ePIROI_Shield]);					
		RareOptionInfo.AttrRegist.SetElement_Val(ATTR_FIRE,		(float)atoi((char*)pData[i].Data[ePIROI_FireAttrRegist])/100.f );
		RareOptionInfo.AttrRegist.SetElement_Val(ATTR_WATER,	(float)atoi((char*)pData[i].Data[ePIROI_WaterAttrRegist])/100.f );
		RareOptionInfo.AttrRegist.SetElement_Val(ATTR_TREE,		(float)atoi((char*)pData[i].Data[ePIROI_TreeAttrRegist])/100.f );
		RareOptionInfo.AttrRegist.SetElement_Val(ATTR_IRON,		(float)atoi((char*)pData[i].Data[ePIROI_GoldAttrRegist])/100.f );
		RareOptionInfo.AttrRegist.SetElement_Val(ATTR_EARTH,	(float)atoi((char*)pData[i].Data[ePIROI_EarthAttrRegist])/100.f );
		RareOptionInfo.PhyAttack			= (WORD)atoi((char*)pData[i].Data[ePIROI_PhyAttack]);
		RareOptionInfo.AttrAttack.SetElement_Val(ATTR_FIRE,		(float)atoi((char*)pData[i].Data[ePIROI_FireAttrAttack])/100.f );
		RareOptionInfo.AttrAttack.SetElement_Val(ATTR_WATER,	(float)atoi((char*)pData[i].Data[ePIROI_WaterAttrAttack])/100.f );
		RareOptionInfo.AttrAttack.SetElement_Val(ATTR_TREE,		(float)atoi((char*)pData[i].Data[ePIROI_TreeAttrAttack])/100.f );
		RareOptionInfo.AttrAttack.SetElement_Val(ATTR_IRON,		(float)atoi((char*)pData[i].Data[ePIROI_GoldAttrAttack])/100.f );
		RareOptionInfo.AttrAttack.SetElement_Val(ATTR_EARTH,	(float)atoi((char*)pData[i].Data[ePIROI_EarthAttrAttack])/100.f );
		RareOptionInfo.PhyDefense			= (WORD)atoi((char*)pData[i].Data[ePIROI_PhyDefense]);				

		pPlayer->AddItemRareOption(&RareOptionInfo);
	}

	if(pMessage->dwResult >= MAX_PETINVENLOAD_ITEM)
	{
		PetInvenItemRareOptionInfo(pMessage->dwID, pPlayer->GetUserID(), TP_PETINVEN_START, TP_PETINVEN_END);
	}
	else
	{
		PetInvenItemInfo(pMessage->dwID, TP_PETINVEN_START, TP_PETINVEN_END);
	}
}

void RPetInvenItemInfo( LPQUERY pData, LPDBMESSAGE pMessage )
{
	CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser(pMessage->dwID);
	if(!pPlayer) return;
	
	if(!pPlayer->GetInited())	return;

	PETINVEN_ITEM PetInvenItem;
	memset(&PetInvenItem, 0, sizeof(PETINVEN_ITEM));
	POSTYPE ItemPos = 0;
	if(atoi((char*)pData[0].Data[ePIII_DBIDX]) != 0)
	{
		for( DWORD i = 0; i < pMessage->dwResult; ++i )
		{
			ItemPos = atoi((char*)pData[i].Data[ePIII_Position]);
			if(TP_PETINVEN_START <= ItemPos && ItemPos < TP_PETINVEN_END)
			{
				ItemPos -= TP_PETINVEN_START;
				PetInvenItem.PetInven[ItemPos].dwDBIdx = atoi((char*)pData[i].Data[ePIII_DBIDX]);
				PetInvenItem.PetInven[ItemPos].wIconIdx = atoi((char*)pData[i].Data[ePIII_IDX]);
				PetInvenItem.PetInven[ItemPos].Position = atoi((char*)pData[i].Data[ePIII_Position]);
				PetInvenItem.PetInven[ItemPos].Durability	= atoi((char*)pData[i].Data[ePIII_Durability]);
				PetInvenItem.PetInven[ItemPos].RareIdx	= atoi((char*)pData[i].Data[ePIII_RareIdx]);
				PetInvenItem.PetInven[ItemPos].StoneIdx = atoi((char*)pData[i].Data[ePIII_StoneIdx]);     // 2014-11-19 数据附加到宠物仓库!
				PetInvenItem.PetInven[ItemPos].ItemStatic	= atoi((char*)pData[i].Data[ePIII_STATIC]);   // 2014-12-08 宠物仓库读取
				PetInvenItem.PetInven[ItemPos].ItemGrow     = atoi((char*)pData[i].Data[ePIII_Grow]);     // 2015-01-14 成长数据读取

				SafeStrCpy(PetInvenItem.PetInven[ItemPos].PowerUp,(char*)pData[i].Data[ePIII_PowerUp], MAX_NAME_LENGTH+1);
				SafeStrCpy(PetInvenItem.PetInven[ItemPos].Green,(char*)pData[i].Data[ePIII_Green], MAX_NAME_LENGTH+1);
				PetInvenItem.PetInven[ItemPos].Grade30 = atoi((char*)pData[i].Data[ePII_Grade30]);
			}
			else
			{
				ASSERT(0);
			}
		}
	}
	pPlayer->InitPetInvenItemInfo(&PetInvenItem);

	if( pMessage->dwResult >= MAX_PETINVENLOAD_ITEM )
	{
		//PetInvenItemInfo(pMessage->dwID, atoi((char*)pData[pMessage->dwResult-1].Data[ePIII_DBIdx]));
		//PetInvenItemInfo(pMessage->dwID, TP_PETINVEN_START, TP_PETINVEN_END);
		PetInvenItemInfo(pMessage->dwID, ItemPos+TP_PETINVEN_START, TP_PETINVEN_END);
	}
	else
	{
		pPlayer->SendPetInvenItemInfo();
	}
}

void RPetWearItemInfo( LPQUERY pData, LPDBMESSAGE pMessage )
{
	CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser(pMessage->dwID);
	if( !pPlayer )	return;

	ITEMBASE ItemBase[SLOT_PETWEAR_NUM];
	//ITEMBASE ItemOverlap[SLOT_PETWEAR_NUM];
	memset( ItemBase, 0, sizeof(ITEMBASE)*SLOT_PETWEAR_NUM );
	//memset( ItemOverlap, 0, sizeof(ITEMBASE)*SLOT_PETWEAR_NUM );
	//int OverlapCount=0;

	int ret = atoi((char*)pData[0].Data[0]);
	if( ret == 0 )
	{
		pPlayer->InitPetWearItemInfo( ItemBase );
		// magi82 - Titan(070209)
		//pPlayer->SetInitState(PLAYERINITSTATE_ITEM_INFO,MP_USERCONN_GAMEIN_SYN);
		TitanWearItemInfo(pPlayer->GetID(), TP_TITANWEAR_START, TP_TITANSHOPITEM_END);
		return;
	}

	if( pMessage->dwResult > SLOT_PETWEAR_NUM )
	{
	//	char Buf[64];
	//	sprintf(Buf, "PetWearItem OverNum CharacterID:%d", pPlayer->GetID());
	//	ASSERTMSG(0, Buf);
	}

	for(DWORD i = 0; i < pMessage->dwResult; i++)
	{
		POSTYPE ItemPos = atoi((char*)pData[i].Data[eCI_Position]);
		ITEMBASE* pItemBase = NULL;

		if(ItemPos >= TP_PETWEAR_START && ItemPos < TP_PETWEAR_END)
		{
			ItemPos -= TP_PETWEAR_START;

			pItemBase = &ItemBase[ItemPos];

			/*
			if( pItemBase->dwDBIdx )
			{
				pItemBase = &ItemOverlap[OverlapCount];
				++OverlapCount;
			}*/

			pItemBase->dwDBIdx = atoi((char*)pData[i].Data[eCI_DBIDX]);
			pItemBase->wIconIdx = atoi((char*)pData[i].Data[eCI_IDX]);
			pItemBase->Position = atoi((char*)pData[i].Data[eCI_Position]);
			pItemBase->Durability = atoi((char*)pData[i].Data[eCI_Durability]);
			pItemBase->QuickPosition = atoi((char*)pData[i].Data[eCI_QPosition]);
			pItemBase->ItemParam = atoi((char*)pData[i].Data[eCI_Param]);

			pPlayer->GetPetManager()->SetPetEquipOption(pItemBase->wIconIdx, TRUE);
		}
	}

	/*
	//吝汗等 酒捞袍捞 乐栏搁 后沫俊 持绢霖促.
	for(int n=0; n<OverlapCount; n++)
	{
		for(int m=0; m<SLOT_PETWEAR_NUM; m++)
		{
			//后沫捞搁 持绢霖促.
			if( ItemBase[m].dwDBIdx == 0)
			{
				ItemBase[m] = ItemOverlap[n];
				ItemBase[m].Position = m;
				break;
			}
		}

		//父距 后沫捞 绝栏搁 歹捞惑 贸府窍瘤 臼澜
		if( m==SLOT_PETWEAR_NUM )
			break;
	}*/

	pPlayer->InitPetWearItemInfo( ItemBase );
	// magi82 - Titan(070209)
	//pPlayer->SetInitState(PLAYERINITSTATE_ITEM_INFO,MP_USERCONN_GAMEIN_SYN);
	TitanWearItemInfo(pPlayer->GetID(), TP_TITANWEAR_START, TP_TITANSHOPITEM_END);
}

//SW070127 鸥捞藕
void TitanInsertToDB(DWORD UserIdx, DWORD CharacterIdx, DWORD dwItemIdx, TITAN_TOTALINFO* pTitanTotalInfo)
{
	sprintf(txt, "EXEC dbo.MP_TITAN_Insert %d,%d,%d,%d,%d, %d,%d,%d,%d,%d, %d,%d",
		UserIdx,
		//CharacterIdx,
		dwItemIdx,
		pTitanTotalInfo->TitanKind,
		pTitanTotalInfo->TitanGrade,
		pTitanTotalInfo->Fuel,

		pTitanTotalInfo->Spell,
		pTitanTotalInfo->RecallTime,
		pTitanTotalInfo->TitanCallItemDBIdx,
		pTitanTotalInfo->RegisterCharacterIdx,
		pTitanTotalInfo->bRiding,

		pTitanTotalInfo->Scale,
		pTitanTotalInfo->MaintainTime	// magi82(23)
		);

	g_DB.Query(eQueryType_FreeQuery, eTitanInsert, CharacterIdx, txt);

}

void RTitanInsertToDB(LPQUERY pData, LPDBMESSAGE pMessage)
{
	//CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser(atoi((char*)pData->Data[0]));
	CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser(pMessage->dwID);

	if( NULL == pPlayer )
		return;
	if( FALSE == pPlayer->GetInited() )
		return;

	TITAN_TOTALINFO TitanInfo;
	if( pMessage->dwResult )
	{
		TitanInfo.TitanMasterUserID = atoi((char*)pData->Data[0]);
		TitanInfo.TitanDBIdx = atoi((char*)pData->Data[1]);
		TitanInfo.TitanCallItemDBIdx = atoi((char*)pData->Data[2]);
		TitanInfo.TitanKind = atoi((char*)pData->Data[3]);
		TitanInfo.TitanGrade = atoi((char*)pData->Data[4]);
		TitanInfo.Fuel = atoi((char*)pData->Data[5]);
		TitanInfo.Spell = atoi((char*)pData->Data[6]);
		TitanInfo.RecallTime = atoi((char*)pData->Data[7]);
		TitanInfo.RegisterCharacterIdx = atoi((char*)pData->Data[8]);
		TitanInfo.bRiding = atoi((char*)pData->Data[9]);
		TitanInfo.Scale = atoi((char*)pData->Data[10]);
		TitanInfo.MaintainTime = atoi((char*)pData->Data[11]);	// magi82(23)
	}
	else
	{
		ASSERT(0);
	}

	pPlayer->GetTitanManager()->AddTitanTotalInfo(&TitanInfo, eServerNClient);

}

void TitanUpdateToDB(DWORD UserIdx, const TITAN_TOTALINFO* pTitanTotalInfo)
{
	sprintf(txt, "EXEC dbo.MP_TITAN_Update %d,%d,%d,%d,%d, %d,%d,%d,%d,%d",
		UserIdx,
		pTitanTotalInfo->TitanDBIdx,
		pTitanTotalInfo->TitanGrade,
		pTitanTotalInfo->Fuel,
		pTitanTotalInfo->Spell,

		pTitanTotalInfo->RecallTime,
		pTitanTotalInfo->RegisterCharacterIdx,
		pTitanTotalInfo->bRiding,
		pTitanTotalInfo->Scale,
		pTitanTotalInfo->MaintainTime	// magi82(23)
		);

	g_DB.Query(eQueryType_FreeQuery, eTitanUpdate, pTitanTotalInfo->TitanCallItemDBIdx, txt);

}

void TitanDeleteToDB( DWORD dwCallItemDBIdx )
{
	sprintf(txt, "EXEC dbo.MP_TITAN_Delete %d", dwCallItemDBIdx);
	g_DB.Query(eQueryType_FreeQuery, eTitanDelete, 0, txt);
}

void TitanEquipItemInfoInsertToDB( TITAN_ENDURANCE_ITEMINFO* pInfo )
{
	sprintf(txt, "EXEC dbo.MP_TITAN_EQUIP_ENDURANCE_Insert %d,%d,%d,%d,%d",
		//pInfo->OwnerUserID,
		//pInfo->OwnerCharacterID,
		pInfo->ItemDBIdx,
		pInfo->CharacterID,
		pInfo->ItemIdx,
		pInfo->Endurance,
		pInfo->UserIdx
//		pInfo->bEquip
		);

	g_DB.Query(eQueryType_FreeQuery, eTitanEquipInfoInsert, pInfo->CharacterID, txt);

}

void RTitanEquipItemInfoInsertToDB( LPQUERY pData, LPDBMESSAGE pMessage )
{
	CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser(pMessage->dwID);
	if( NULL == pPlayer )
		return;
	if( FALSE == pPlayer->GetInited() )
		return;

	TITAN_ENDURANCE_ITEMINFO TitanItemEnduranceInfo;
	if( pMessage->dwResult )

	{
		//TitanItemEnduranceInfo.OwnerUserID = atoi((char*)pData->Data[1]);
		//TitanItemEnduranceInfo.OwnerCharacterID = atoi((char*)pData->Data[0]);
		TitanItemEnduranceInfo.CharacterID = atoi((char*)pData->Data[0]);
		TitanItemEnduranceInfo.ItemDBIdx = atoi((char*)pData->Data[1]);
		TitanItemEnduranceInfo.ItemIdx	 = atoi((char*)pData->Data[2]);
		TitanItemEnduranceInfo.Endurance = atoi((char*)pData->Data[3]);
		TitanItemEnduranceInfo.UserIdx	= atoi((char*)pData->Data[4]);
		//TitanItemEnduranceInfo.bEquip = atoi((char*)pData->Data[5]);
	}
	else
	{
		ASSERT(0);
	}

	pPlayer->GetTitanManager()->AddTitanEquipItemEnduranceInfo(&TitanItemEnduranceInfo, eServerNClient);

}

void TitanEquipItemInfoUpdateToDB( TITAN_ENDURANCE_ITEMINFO* pInfo )
{
	sprintf(txt, "EXEC dbo.MP_TITAN_EQUIP_ENDURANCE_Update %d,%d,%d,%d",
		//pInfo->OwnerUserID,
		//pInfo->OwnerCharacterID,
		//pInfo->ItemIdx,
		pInfo->ItemDBIdx,
		pInfo->CharacterID,
		pInfo->Endurance,
		pInfo->UserIdx

		//pInfo->bEquip
		);

	g_DB.Query(eQueryType_FreeQuery, eTitanEquipInfoUpdate, 0, txt);
}

void TitanEquipItemInfoDeleteToDB(DWORD dwTitanEquipDBIdx)
{
	sprintf(txt, "EXEC dbo.MP_TITAN_EQUIP_ENDURANCE_Delete %d", dwTitanEquipDBIdx);
	g_DB.Query(eQueryType_FreeQuery, eTitanEquipInfoDelete, 0, txt);

}

// magi82(33)
void TitanEquipInsertExceptionToDB( TITAN_ENDURANCE_ITEMINFO* pInfo )
{
	sprintf(txt, "EXEC dbo.MP_TITAN_EQUIP_ENDURANCE_Insert %d,%d,%d,%d,%d",
		//pInfo->OwnerUserID,
		//pInfo->OwnerCharacterID,
		pInfo->ItemDBIdx,
		pInfo->CharacterID,
		pInfo->ItemIdx,
		pInfo->Endurance,
		pInfo->UserIdx
		//		pInfo->bEquip
		);

	g_DB.Query(eQueryType_FreeQuery, eTitanEquipInfoInsertException, pInfo->CharacterID, txt);

}

// magi82(33)
void RTitanEquipInsertExceptionToDB( LPQUERY pData, LPDBMESSAGE pMessage )
{
	CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser(pMessage->dwID);
	if( NULL == pPlayer )
		return;

	TITAN_ENDURANCE_ITEMINFO TitanItemEnduranceInfo;
	if( pMessage->dwResult )
	{
		//TitanItemEnduranceInfo.OwnerUserID = atoi((char*)pData->Data[1]);
		//TitanItemEnduranceInfo.OwnerCharacterID = atoi((char*)pData->Data[0]);
		TitanItemEnduranceInfo.CharacterID = atoi((char*)pData->Data[0]);
		TitanItemEnduranceInfo.ItemDBIdx = atoi((char*)pData->Data[1]);
		TitanItemEnduranceInfo.ItemIdx	 = atoi((char*)pData->Data[2]);
		TitanItemEnduranceInfo.Endurance = atoi((char*)pData->Data[3]);
		TitanItemEnduranceInfo.UserIdx	= atoi((char*)pData->Data[4]);
		//TitanItemEnduranceInfo.bEquip = atoi((char*)pData->Data[5]);
	}
	else
	{
		ASSERT(0);
	}

	pPlayer->GetTitanManager()->AddTitanEquipItemEnduranceInfo(&TitanItemEnduranceInfo);

}

// magi82(33)
void TitanEquipInsertExceptionPyogukToDB(DWORD CharacterIdx, TITAN_ENDURANCE_ITEMINFO* pInfo )
{
	sprintf(txt, "EXEC dbo.MP_TITAN_EQUIP_ENDURANCE_Insert %d,%d,%d,%d,%d",
		//pInfo->OwnerUserID,
		//pInfo->OwnerCharacterID,
		pInfo->ItemDBIdx,
		pInfo->CharacterID,
		pInfo->ItemIdx,
		pInfo->Endurance,
		pInfo->UserIdx
		//		pInfo->bEquip
		);

	g_DB.Query(eQueryType_FreeQuery, eTitanEquipInfoInsertExceptionPyoguk, CharacterIdx, txt);

}

// magi82(33)
void RTitanEquipInsertExceptionPyogukToDB( LPQUERY pData, LPDBMESSAGE pMessage )
{
	CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser(pMessage->dwID);
	if( NULL == pPlayer )
		return;

	TITAN_ENDURANCE_ITEMINFO TitanItemEnduranceInfo;
	if( pMessage->dwResult )
	{
		//TitanItemEnduranceInfo.OwnerUserID = atoi((char*)pData->Data[1]);
		//TitanItemEnduranceInfo.OwnerCharacterID = atoi((char*)pData->Data[0]);
		TitanItemEnduranceInfo.CharacterID = atoi((char*)pData->Data[0]);
		TitanItemEnduranceInfo.ItemDBIdx = atoi((char*)pData->Data[1]);
		TitanItemEnduranceInfo.ItemIdx	 = atoi((char*)pData->Data[2]);
		TitanItemEnduranceInfo.Endurance = atoi((char*)pData->Data[3]);
		TitanItemEnduranceInfo.UserIdx	= atoi((char*)pData->Data[4]);
		//TitanItemEnduranceInfo.bEquip = atoi((char*)pData->Data[5]);
	}
	else
	{
		ASSERT(0);
	}

	pPlayer->GetTitanManager()->AddTitanEquipItemEnduranceInfo(&TitanItemEnduranceInfo, eServerNClient);

}

/*
void RCharacterGMControlInfo(LPMIDDLEQUERY pData, LPDBMESSAGE pMessage)
{
	if(pMessage->dwResult == 0)
		return;

	DWORD CharacterIdx = atoi((char*)pData[0].Data[0]);
	CPlayer* pPlayer = (CPlayer *)g_pUserTable->FindUser(CharacterIdx);
	if(pPlayer==NULL)
		return;

	for(int n=0;n<pMessage->dwResult;++n)
	{
		//TDN
	//	int kind = atoi((char*)pData[n].Data[1])

	}

	pPlayer->SetInitState(PLAYERINITSTATE_GMCONTROL_INFO,pMessage->dwID);
}

void RCharacterGMControlInsert(LPMIDDLEQUERY pData, LPDBMESSAGE pMessage)
{
	ASSERT(pMessage->dwResult == 1);

	DWORD CharacterIdx = atoi((char*)pData[0].Data[0]);
	CPlayer* pPlayer = (CPlayer *)g_pUserTable->FindUser(CharacterIdx);
	if(pPlayer)
	{
		//TDN
//		int kind = atoi((char*)pData[n].Data[1]);

	}

	
	DWORD GMChrIdx = pMessage->dwID;
	CPlayer* pGM = (CPlayer *)g_pUserTable->FindUser(CharacterIdx);
	if(pGM==NULL)
		return;
	
		//TDN
}
*/

void RSSItemInsert(LPQUERY pData, LPDBMESSAGE pMessage)
{
	if((atoi((char*)pData->Data[0]) == 0))
	{
		// Insert ¨oC¨¨∷
		
	}
	else
	{
		// Insert ¨u¨〓āㄏǎ
		DWORD dwPlayerID = atoi((char*)pData->Data[eCI_ObjectID]);		// Aㄏ┆∷〓?¨u¨AI⊥iㄏ√
		ITEMBASE info;
		info.Position = atoi((char*)pData->Data[eCI_Position]);		// Item_Position
		info.QuickPosition = atoi((char*)pData->Data[eCI_QPosition]);
		info.dwDBIdx = atoi((char*)pData->Data[eCI_DBIDX]);		// Item_DBIDX
		info.Durability = atoi((char*)pData->Data[eCI_Durability]);	// Item_Durability
		info.wIconIdx = atoi((char*)pData->Data[eCI_IDX]);			// Item_IDX
		// magi82(28)
		info.ItemParam = 0;
		info.RareIdx = 0;
		info.ItemStatic=0;  //  2015-01-14
		info.ItemGrow =0;   // 成长 2015-01-14 

		SafeStrCpy(info.PowerUp,(char*)pData[0].Data[eCI_PowerUp], MAX_NAME_LENGTH+1);
		SafeStrCpy(info.Green,(char*)pData[0].Data[eCI_Green], MAX_NAME_LENGTH+1);

		STREETSTALLMGR->CreateDupItem( dwPlayerID, &info, pMessage->dwID);
	}	
}

void	RCharacterItemGetCheatIDX(LPQUERY pData, LPDBMESSAGE pMessage)
{
	if((atoi((char*)pData->Data[0]) == 0))
	{
		// Insert ¨oC¨¨∷
		
	}
	else
	{
		// Insert ¨u¨〓āㄏǎ
		DWORD dwPlayerID = atoi((char*)pData->Data[eCI_ObjectID]);		// Aㄏ┆∷〓?¨u¨AI⊥iㄏ√
		ITEMBASE info;
		info.Position = atoi((char*)pData->Data[eCI_Position]);		// Item_Position
		info.dwDBIdx = atoi((char*)pData->Data[eCI_DBIDX]);		// Item_DBIDX
		info.QuickPosition = atoi((char*)pData->Data[eCI_QPosition]);
		info.Durability = atoi((char*)pData->Data[eCI_Durability]);	// Item_Durability
		info.wIconIdx = atoi((char*)pData->Data[eCI_IDX]);			// Item_IDX
		// magi82(28)
		info.ItemParam = 0;
		info.RareIdx = 0;

		CPlayer* pPlayer = (CPlayer *)g_pUserTable->FindUser(dwPlayerID);
		if(pPlayer == NULL)
			return;
		
//		ITEMMGR_OBJ->CheatItemDBResult(pPlayer,&info);
	}
}
void	RCharacterItemGetDivideIDX(LPQUERY pData, LPDBMESSAGE pMessage)
{
	if((atoi((char*)pData->Data[0]) == 0))
	{
		// Insert ¨oC¨¨∷
	}
	else
	{
		// Insert ¨u¨〓āㄏǎ
		atoi((char*)pData->Data[eCI_ObjectID]);		// Aㄏ┆∷〓?¨u¨AI⊥iㄏ√
		atoi((char*)pData->Data[eCI_DBIDX]);		// Item_DBIDX
		atoi((char*)pData->Data[eCI_IDX]);			// Item_IDX
		atoi((char*)pData->Data[eCI_Position]);		// Item_Position
		atoi((char*)pData->Data[eCI_QPosition]);		// Item_QuickPosition
		atoi((char*)pData->Data[eCI_Durability]);	// Item_Durability
	}
}
void RItemOptionInsert(LPQUERY pData, LPDBMESSAGE pMessage)
{
	if( pMessage->dwResult )
	{
		// Insert ¨u¨〓āㄏǎ
		CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser((DWORD)atoi((char*)pData[0].Data[eCIO_ObjectID]));
		if(pPlayer == NULL)
			return;

		ITEM_OPTION_INFO OptionInfo;
		OptionInfo.dwOptionIdx				= (DWORD)atoi((char*)pData[0].Data[eCIO_OptionID]);
		OptionInfo.dwItemDBIdx				= (DWORD)atoi((char*)pData[0].Data[eCIO_ItemDBID]);
		OptionInfo.GenGol					= (WORD)atoi((char*)pData[0].Data[eCIO_GenGol]);
		OptionInfo.MinChub					= (WORD)atoi((char*)pData[0].Data[eCIO_MinChub]);				
		OptionInfo.CheRyuk					= (WORD)atoi((char*)pData[0].Data[eCIO_CheRyuk]);				
		OptionInfo.SimMek					= (WORD)atoi((char*)pData[0].Data[eCIO_SimMek]);					
		OptionInfo.Life						= (DWORD)atoi((char*)pData[0].Data[eCIO_Life]);					
		OptionInfo.NaeRyuk					= (WORD)atoi((char*)pData[0].Data[eCIO_NaeRyuk]);				
		OptionInfo.Shield					= (DWORD)atoi((char*)pData[0].Data[eCIO_Shield]);					
		OptionInfo.AttrRegist.SetElement_Val(ATTR_FIRE,		(float)atoi((char*)pData[0].Data[eCIO_FireAttrRegist])/100.f );
		OptionInfo.AttrRegist.SetElement_Val(ATTR_WATER,	(float)atoi((char*)pData[0].Data[eCIO_WaterAttrRegist])/100.f );
		OptionInfo.AttrRegist.SetElement_Val(ATTR_TREE,		(float)atoi((char*)pData[0].Data[eCIO_TreeAttrRegist])/100.f );
		OptionInfo.AttrRegist.SetElement_Val(ATTR_IRON,		(float)atoi((char*)pData[0].Data[eCIO_GoldAttrRegist])/100.f );
		OptionInfo.AttrRegist.SetElement_Val(ATTR_EARTH,	(float)atoi((char*)pData[0].Data[eCIO_EarthAttrRegist])/100.f );
		OptionInfo.PhyAttack				= (WORD)atoi((char*)pData[0].Data[eCIO_PhyAttack]);
		OptionInfo.CriticalPercent			= (WORD)atoi((char*)pData[0].Data[eCIO_Critical]);
		OptionInfo.AttrAttack.SetElement_Val(ATTR_FIRE,		(float)atoi((char*)pData[0].Data[eCIO_FireAttrAttack])/100.f );
		OptionInfo.AttrAttack.SetElement_Val(ATTR_WATER,	(float)atoi((char*)pData[0].Data[eCIO_WaterAttrAttack])/100.f );
		OptionInfo.AttrAttack.SetElement_Val(ATTR_TREE,		(float)atoi((char*)pData[0].Data[eCIO_TreeAttrAttack])/100.f );
		OptionInfo.AttrAttack.SetElement_Val(ATTR_IRON,		(float)atoi((char*)pData[0].Data[eCIO_GoldAttrAttack])/100.f );
		OptionInfo.AttrAttack.SetElement_Val(ATTR_EARTH,	(float)atoi((char*)pData[0].Data[eCIO_EarthAttrAttack])/100.f );
		OptionInfo.PhyDefense				= (WORD)atoi((char*)pData[0].Data[eCIO_PhyDefense]);				

		ITEMBASE info;
		SafeStrCpy(info.PowerUp,(char*)pData[0].Data[eCIO_PowerUp], MAX_NAME_LENGTH+1);

		pPlayer->AddItemOption(&OptionInfo);
		ITEMMGR->ReinforceItemDBResult(pPlayer, (WORD)atoi((char*)pData[0].Data[eCIO_ItemIdx]), (WORD)pMessage->dwID, &OptionInfo);
	}
	else
	{
		ASSERT(0);
	}
}

//!!!
void RItemRareInsert(LPQUERY pData, LPDBMESSAGE pMessage)
{
	WORD lastNo = LOWORD(pMessage->dwID);
	WORD ArrayID = HIWORD(pMessage->dwID);

	if( pMessage->dwResult )
	{
		CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser((DWORD)atoi((char*)pData[0].Data[eCIR_ObjectID]));
		if(pPlayer == NULL)
			return;
		if(pPlayer->GetInited() == FALSE)
			return;
		
		ITEMOBTAINARRAYINFO * pItemArrayInfo = pPlayer->GetArray(ArrayID);
		if(pItemArrayInfo == NULL)
			return;

		pItemArrayInfo->ItemArray.AddItem( atoi((char*)pData->Data[eCIR_ItemDBID]),	
							atoi((char*)pData->Data[eCIR_ItemIdx]),	
							0,	
							atoi((char*)pData->Data[eCIR_Position]),
							0,
							0,
							atoi((char*)pData->Data[eCIR_RareID])
							);	
		
		ITEM_RARE_OPTION_INFO RareOptionInfo;

		RareOptionInfo.dwRareOptionIdx				= (DWORD)atoi((char*)pData[0].Data[eCIR_RareID]);
		RareOptionInfo.dwItemDBIdx				= (DWORD)atoi((char*)pData[0].Data[eCIR_ItemDBID]);
		RareOptionInfo.GenGol					= (WORD)atoi((char*)pData[0].Data[eCIR_GenGol]);
		RareOptionInfo.MinChub					= (WORD)atoi((char*)pData[0].Data[eCIR_MinChub]);				
		RareOptionInfo.CheRyuk					= (WORD)atoi((char*)pData[0].Data[eCIR_CheRyuk]);				
		RareOptionInfo.SimMek					= (WORD)atoi((char*)pData[0].Data[eCIR_SimMek]);					
		RareOptionInfo.Life						= (DWORD)atoi((char*)pData[0].Data[eCIR_Life]);					
		RareOptionInfo.NaeRyuk					= (WORD)atoi((char*)pData[0].Data[eCIR_NaeRyuk]);				
		RareOptionInfo.Shield					= (DWORD)atoi((char*)pData[0].Data[eCIR_Shield]);					
		RareOptionInfo.AttrRegist.SetElement_Val(ATTR_FIRE,		(float)atoi((char*)pData[0].Data[eCIR_FireAttrRegist])/100.f );
		RareOptionInfo.AttrRegist.SetElement_Val(ATTR_WATER,	(float)atoi((char*)pData[0].Data[eCIR_WaterAttrRegist])/100.f );
		RareOptionInfo.AttrRegist.SetElement_Val(ATTR_TREE,		(float)atoi((char*)pData[0].Data[eCIR_TreeAttrRegist])/100.f );
		RareOptionInfo.AttrRegist.SetElement_Val(ATTR_IRON,		(float)atoi((char*)pData[0].Data[eCIR_GoldAttrRegist])/100.f );
		RareOptionInfo.AttrRegist.SetElement_Val(ATTR_EARTH,	(float)atoi((char*)pData[0].Data[eCIR_EarthAttrRegist])/100.f );
		RareOptionInfo.PhyAttack				= (WORD)atoi((char*)pData[0].Data[eCIR_PhyAttack]);
//		RareOptionInfo.CriticalPercent			= (WORD)atoi((char*)pData[0].Data[eCIO_Critical]);
		RareOptionInfo.AttrAttack.SetElement_Val(ATTR_FIRE,		(float)atoi((char*)pData[0].Data[eCIR_FireAttrAttack])/100.f );
		RareOptionInfo.AttrAttack.SetElement_Val(ATTR_WATER,	(float)atoi((char*)pData[0].Data[eCIR_WaterAttrAttack])/100.f );
		RareOptionInfo.AttrAttack.SetElement_Val(ATTR_TREE,		(float)atoi((char*)pData[0].Data[eCIR_TreeAttrAttack])/100.f );
		RareOptionInfo.AttrAttack.SetElement_Val(ATTR_IRON,		(float)atoi((char*)pData[0].Data[eCIR_GoldAttrAttack])/100.f );
		RareOptionInfo.AttrAttack.SetElement_Val(ATTR_EARTH,	(float)atoi((char*)pData[0].Data[eCIR_EarthAttrAttack])/100.f );
		RareOptionInfo.PhyDefense				= (WORD)atoi((char*)pData[0].Data[eCIR_PhyDefense]);

		ITEMBASE info;
		SafeStrCpy(info.Green,(char*)pData[0].Data[eCIR_Green], MAX_NAME_LENGTH+1);
		
		pPlayer->AddItemRareOption(&RareOptionInfo);

		if( lastNo == pItemArrayInfo->ItemArray.ItemNum )
		{
			pItemArrayInfo->CallBackEx( pPlayer, ArrayID, &RareOptionInfo );
			//ITEMMGR->ObtainItemDBResult( pPlayer, ArrayID);
		}
//		ITEMMGR->RareItemDBResult(pPlayer, (WORD)atoi((char*)pData[0].Data[eCIR_ItemIdx]), (WORD)pMessage->dwID, &RareOptionInfo);
	}
	else
	{
		ASSERT(0);
	}
}


/* New Item Return Func */
void RItemInsert(LPQUERY pData, LPDBMESSAGE pMessage)
{
	WORD lastNo = LOWORD(pMessage->dwID);
	WORD ArrayID = HIWORD(pMessage->dwID);

	if( pMessage->dwResult )
	{
		// Insert ¨u¨〓āㄏǎ

		DWORD dwObjectID = atoi((char*)pData->Data[eCI_ObjectID]);
		DWORD dwItemIdx = atoi((char*)pData->Data[eCI_IDX]);
		DWORD dwDura = atoi((char*)pData->Data[eCI_Durability]);
		DWORD dwPos = atoi((char*)pData->Data[eCI_Position]);
		DWORD dwQPos = atoi((char*)pData->Data[eCI_QPosition]);
		DWORD dwDBIdx = atoi((char*)pData->Data[eCI_DBIDX]);
		DWORD dwParam = atoi((char*)pData->Data[eCI_Param]);
		DWORD dwStatic= atoi((char*)pData->Data[eCI_Static]);   // 2014-12-01  默认0 此处写了后面没使用！
		DWORD dwGrow  = atoi((char*)pData->Data[eCI_Grow]);   // 2015-01-14 成长 默认0 此处写了后面没使用！
		DWORD dweGrade30 = atoi((char*)pData->Data[eCI_Grade30]);
		CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser(atoi((char*)pData->Data[eCI_ObjectID]));
		if(pPlayer == NULL)
			return;
		if(pPlayer->GetInited() == FALSE)
			return;

		ITEMOBTAINARRAYINFO * pItemArrayInfo = pPlayer->GetArray(ArrayID);
		if(pItemArrayInfo == NULL)
			return;

		pItemArrayInfo->ItemArray.AddItem( atoi((char*)pData->Data[eCI_DBIDX]), 
							atoi((char*)pData->Data[eCI_IDX]), 
							atoi((char*)pData->Data[eCI_Durability]), 
							atoi((char*)pData->Data[eCI_Position]), 
							atoi((char*)pData->Data[eCI_QPosition]),
							atoi((char*)pData->Data[eCI_Param]),
							0,
							atoi((char*)pData->Data[eCI_Static]),
							atoi((char*)pData->Data[eCI_Grow]),

			                atoi((char*)pData->Data[eCI_Grade30])


							);

		if( lastNo == pItemArrayInfo->ItemArray.ItemNum )
		{
			pItemArrayInfo->CallBack( pPlayer, ArrayID );
			//ITEMMGR->ObtainItemDBResult( pPlayer, ArrayID);
		}
		else
		{
			//¨u¨∷⊥O∷O】∷⊥A ¨ui∷OiāO Aㄏ┆∷〓∷cCOāIAIā∷㈢?
			//A┉∷OoAI¨uㄏ√¨∷c AI¨〓IAa∷〓∷cā∷㈢ ∷〓¨ǎAㄏ〓∷⊥A⊥iI.. ∷〓¨々¨oAAoā∷㈢ ¨uㄏǎ¨ui¨uㄏ.
		}
	}
	else
	{
		ASSERT(0);
	}
}

/* New Mugong Return Func */
void RMugongInsert(LPQUERY pData, LPDBMESSAGE pMessage)
{
	if((atoi((char*)pData->Data[0]) == 0))
	{
		// ¨oC¨¨∷
		/*
		MSG_MUGONG_ADD_NACK msg;
		msg.Category = MP_MUGONG;
		msg.Protocol = MP_MUGONG_ADD_NACK;
		msg.dwObjectID = atoi((char*)pData->Data[0]);

		CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser(msg.dwObjectID);
		if(pPlayer == NULL)
			return;
		MUGONGMGR_OBJ->AddMugongNACK(pPlayer);
		*/
	}
	else
	{
		CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser(atoi((char*)pData->Data[0]));
		if(pPlayer == NULL)
			return;

		MUGONGBASE sMugong;
		sMugong.dwDBIdx			= atoi((char*)pData->Data[eCM_DBIDX]);
		sMugong.wIconIdx		= atoi((char*)pData->Data[eCM_IDX]);
		sMugong.ExpPoint		= atoi((char*)pData->Data[eCM_ExpPoint]);
		sMugong.Sung			= atoi((char*)pData->Data[eCM_Sung]);
		sMugong.Position		= atoi((char*)pData->Data[eCM_Position]);
		sMugong.QuickPosition	= atoi((char*)pData->Data[eCM_QPosition]);
		sMugong.OptionIndex		= atoi((char*)pData->Data[eCM_Option]);
		sMugong.bWear			= FALSE;

		////////////////////////////////////////////////////////////////////////////////
		// 06. 06. 2瞒 傈流 - 捞康霖
		// 公傍 函券
		//sMugong.OptionIndex = 0;
		////////////////////////////////////////////////////////////////////////////////

		MUGONGMNGR->AddMugongDBResult(pPlayer, &sMugong);
	}
}

void RMapBaseEconomy(LPQUERY pData, LPDBMESSAGE pMessage)
{
	if((atoi((char*)pData->Data[0]) == 0))
	{
		// Ai∷OEi¨u¨oC¨¨∷
	}
	else
	{
		REGIST_BASEECONOMY msg;
		msg.MapNum = atoi((char*)pData->Data[eME_MEPNUM]);
		msg.OriginNum = atoi((char*)pData->Data[eME_ORIGINNUM]);
		msg.OriginPrice = atoi((char*)pData->Data[eME_ORIGINPRICE]);
		msg.OriginAmount = atoi((char*)pData->Data[eME_ORIGINAMOUNT]);
		msg.RequireNum = atoi((char*)pData->Data[eME_REQUIRENUM]);
		msg.RequirePrice = atoi((char*)pData->Data[eME_REQUIREPRICE]);
		msg.RequireAmount = atoi((char*)pData->Data[eME_REQUIREAMOUNT]);
		msg.BuyRates = atoi((char*)pData->Data[eME_BUYRATES]);
		msg.SellRates = atoi((char*)pData->Data[eME_SELLRATES]);
		
		ECONOMY_OBJ->SetRegistEconomy(&msg);
	}
}

void RAuctionPageQuery(LPQUERY pData, LPDBMESSAGE pMessage)
{
/*	if((atoi((char*)pData->Data[0]) == 0))
	{
		// Ai∷OEi¨u¨oC¨¨∷
	}
	else
*/	{
		AUCTION_ONPAGELIST msg;
		for(int i = 0; i < 10; i++)
		{
			msg.Auction_List[i].ItemIdx = atoi((char*)pData[i].Data[eAT_ItemDBIdx]);
			msg.Auction_List[i].SellNum = atoi((char*)pData[i].Data[eAT_SellNum]);
			msg.Auction_List[i].SellerID = atoi((char*)pData[i].Data[eAT_SellerID]);
			msg.Auction_List[i].EndDate = atoi((char*)pData[i].Data[eAT_EndDate]);
			msg.Auction_List[i].EndTime = atoi((char*)pData[i].Data[eAT_EndTime]);
			msg.Auction_List[i].StartPrice = atoi((char*)pData[i].Data[eAT_StartPrice]);
			msg.Auction_List[i].ImmediatePrice = atoi((char*)pData[i].Data[eAT_ImmediatePrice]);
		}
	}
}


void RPartyLoad(LPQUERY pData, LPDBMESSAGE pMessage)
{
	DWORD PartyIDX = 0;
	for(DWORD i=0; i<pMessage->dwResult;++i)
	{
		PartyIDX = atoi((char*)pData[i].Data[ePA_IDX]);
		PARTYMGR->RegistParty(PartyIDX);
	}
	if(pMessage->dwResult == MAX_PARTY_LOAD_NUM)
	{
		PartyLoad(PartyIDX);
	}
}

//2008. 5. 21. CBH - 规颇 积己 眠啊 可记 包访 贸府 荐沥
void RPartyCreate(LPMIDDLEQUERY pData, LPDBMESSAGE pMessage) 
{
	DWORD CreateErr, MasterIdx;
	
	PARTY_ADDOPTION AddOption;
	memset(&AddOption, 0, sizeof(PARTY_ADDOPTION));

	CreateErr = atoi((char*)pData->Data[ePA_ERR]);
	MasterIdx = atoi((char*)pData->Data[ePA_MASTER]);
	AddOption.bOption = atoi((char*)pData->Data[ePA_OPTION]);
	AddOption.wMinLevel = atoi((char*)pData->Data[ePA_MINLEVEL]);
	AddOption.wMaxLevel = atoi((char*)pData->Data[ePA_MAXLEVEL]);
	AddOption.bPublic = atoi((char*)pData->Data[ePA_PUBLIC]);
	AddOption.wLimitCount = atoi((char*)pData->Data[ePA_LIMITCOUNT]);
	SafeStrCpy(AddOption.szTheme, (char*)pData->Data[ePA_THEME], MAX_PARTY_NAME+1);	

	CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser(MasterIdx);
	if(!pPlayer)
		return;

	if(CreateErr == NULL)
	{
		PARTYMGR->SendErrMsg(MasterIdx, eErr_Create_DB, MP_PARTY_CREATE_NACK);
		return;	
	}
	else
	{
		AddOption.dwPartyIndex = atoi((char*)pData->Data[ePA_PARTYID]);
	}

	PARTYMGR->CreatePartyResult(pPlayer, &AddOption);
}

void RPartyInfoByUserLogin(LPMIDDLEQUERY pData, LPDBMESSAGE pMessage) 
{
	PARTYMEMBER member[MAX_PARTY_LISTNUM];
	BYTE opt;
	CPlayer* pPlayer;
	PARTY_ADDOPTION AddOption;
	
	memset(member, 0, sizeof(PARTYMEMBER)*MAX_PARTY_LISTNUM);
	memset(&AddOption, 0, sizeof(PARTY_ADDOPTION));

	pPlayer = (CPlayer*)g_pUserTable->FindUser(pMessage->dwID);
	if(pPlayer == NULL)
		return;

	if( !pMessage->dwResult )
	{
		pPlayer->SetPartyIdx(0);
		CharacterTablePartyIdxUpdate( pMessage->dwID );
		return;
	}

	for(DWORD n=0;n<pMessage->dwResult;++n)
	{
		member[n].MemberID = atoi((char*)pData[n].Data[ePA_LIMemberId]);
		SafeStrCpy(member[n].Name,(char*)pData[n].Data[ePA_LIMemberName], MAX_NAME_LENGTH+1);
		member[n].bLogged = atoi((char*)pData[n].Data[ePA_LIMemberCurrentMap]) ? TRUE : FALSE;
		member[n].LifePercent = 0;
		member[n].NaeRyukPercent = 0;
		member[n].Level = atoi((char*)pData[n].Data[ePA_LIMemberLevel]);
	}
	opt = atoi((char*)pData[0].Data[ePA_LIOption]);	
	AddOption.wMinLevel = atoi((char*)pData[0].Data[ePA_MinLevel]);
	AddOption.wMaxLevel = atoi((char*)pData[0].Data[ePA_MaxLevel]);
	AddOption.bPublic = atoi((char*)pData[0].Data[ePA_Public]);
	AddOption.wLimitCount = atoi((char*)pData[0].Data[ePA_LimitCount]);
	SafeStrCpy(AddOption.szTheme, (char*)pData[0].Data[ePA_Theme], MAX_PARTY_NAME);	
	AddOption.bOption = opt;
	AddOption.dwPartyIndex = pPlayer->GetPartyIdx();

	PARTYMGR->UserLogInAfterDBQuery(pMessage->dwID, pPlayer->GetPartyIdx(), member, AddOption);
}


void RPartyChangeMaster(LPQUERY pData, LPDBMESSAGE pMessage)
{
	CParty* pParty = PARTYMGR->GetParty(pMessage->dwID);
	if(!pParty)
	{
		ASSERT(0);
		return;
	}

	pParty->SetMasterChanging(FALSE);
		
	CPlayer* pMaster = (CPlayer*)g_pUserTable->FindUser(pParty->GetMasterID());
	if(atoi((char*)pData->Data[ePA_CMErr]) == 0) //change master ack
	{
		PARTYMGR->DoChangeMasterParty(pParty, atoi((char*)pData->Data[ePA_CMTargetID]));
		//pParty->SetMasterChanging(FALSE);
		PARTYMGR->NotifyChangesOtherMapServer(atoi((char*)pData->Data[ePA_CMTargetID]),pParty,MP_PARTY_NOTIFYCHANGEMASTER_TO_MAPSERVER, pMessage->dwID);
	}
	else //change master nack
	{
		if(!pMaster)
		{
		//	pParty->SetMasterChanging(FALSE);
			return;
		}

		MSGBASE msg;
		msg.Category = MP_PARTY;
		msg.Protocol = MP_PARTY_CHANGEMASTER_NACK;
		pMaster->SendMsg(&msg, sizeof(msg));
	}
}

void RPartyDelMember(LPQUERY pData, LPDBMESSAGE pMessage)
{
	CParty* pParty = PARTYMGR->GetParty(pMessage->dwID);
	if(!pParty)
	{
		ASSERT(0);
		return;
	}
	if(atoi((char*)pData->Data[ePA_DMErr]) == 0) //del member ack
	{
		PARTYMGR->DelMember(atoi((char*)pData->Data[ePA_DMTargetID]), pMessage->dwID);
	}
	else //del member nack
	{
		CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser(atoi((char*)pData->Data[ePA_DMTargetID]));
		if(!pPlayer)
			return;
		MSG_INT msg;
		msg.Category = MP_PARTY;
		msg.Protocol = MP_PARTY_DEL_NACK;
		msg.nData	 = atoi((char*)pData->Data[ePA_DMErr]);
		pPlayer->SendMsg(&msg, sizeof(msg));
	}
}


void RCharacterPyogukInfo(LPQUERY pData, LPDBMESSAGE pMessage)
{
	CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser(pMessage->dwID);
	if( !pPlayer)
		return;

	BYTE PyogukNum;

	if( pMessage->dwResult == 0 )
	{
		PyogukNum = 0;
	}
	else
	{
		PyogukNum = atoi((char*)pData->Data[ePyo_PyogukNum]);
	}

	pPlayer->InitPyogukInfo(PyogukNum, _atoi64((char*)pData->Data[ePyo_Money]));

	pPlayer->SetInitState(PLAYERINITSTATE_PYOGUK_INFO,MP_USERCONN_GAMEIN_SYN);
}

void RCharacterPyogukItemInfo(LPQUERY pData, LPDBMESSAGE pMessage)
{
	CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser(pMessage->dwID);
	if( !pPlayer ) return;

	if(pPlayer->GetInited() == FALSE)
		return;

	PYOGUK_ITEM PyogukItem;
	memset(&PyogukItem, 0, sizeof(PYOGUK_ITEM));

	POSTYPE ItemPos;
	BYTE i;
	if(atoi((char*)pData[0].Data[ePI_DBIdx]) != 0)
	{
		for( i = 0 ; i < pMessage->dwResult ; ++i )
		{	
			ItemPos = atoi((char*)pData[i].Data[ePI_Positon]);
			if(TP_PYOGUK_START<= ItemPos && ItemPos < TP_PYOGUK_END)
			{
				ItemPos -= TP_PYOGUK_START;
				PyogukItem.Pyoguk[ItemPos].dwDBIdx	= atoi((char*)pData[i].Data[ePI_DBIdx]);
				PyogukItem.Pyoguk[ItemPos].wIconIdx		= atoi((char*)pData[i].Data[ePI_IconIdx]);
				PyogukItem.Pyoguk[ItemPos].Position		= atoi((char*)pData[i].Data[ePI_Positon]);
				PyogukItem.Pyoguk[ItemPos].QuickPosition	= atoi((char*)pData[i].Data[ePI_QPosition]);
				PyogukItem.Pyoguk[ItemPos].Durability		= atoi((char*)pData[i].Data[ePI_Durability]);
				PyogukItem.Pyoguk[ItemPos].RareIdx		= atoi((char*)pData[i].Data[ePI_RareIdx]);
				PyogukItem.Pyoguk[ItemPos].StoneIdx     = atoi((char*)pData[i].Data[ePI_StoneIdx]); // 2014-12-08 仓库读取
				PyogukItem.Pyoguk[ItemPos].ItemStatic     = atoi((char*)pData[i].Data[ePI_Static]);  // 2014-12-08 仓库读取
				PyogukItem.Pyoguk[ItemPos].ItemGrow       = atoi((char*)pData[i].Data[ePI_Grow]);    // 2015-01-14 仓库成长数据读取

				SafeStrCpy(PyogukItem.Pyoguk[ItemPos].PowerUp,(char*)pData[i].Data[ePI_PowerUp], MAX_NAME_LENGTH+1);
				SafeStrCpy(PyogukItem.Pyoguk[ItemPos].Green,(char*)pData[i].Data[ePI_Green], MAX_NAME_LENGTH+1);
				PyogukItem.Pyoguk[ItemPos].Grade30 = atoi((char*)pData[i].Data[ePI_Grade30]);
				// magi82(33)
				ITEM_INFO* pInfo = ITEMMGR->GetItemInfo(PyogukItem.Pyoguk[ItemPos].wIconIdx);
				if( pInfo && pInfo->ItemKind & eTITAN_EQUIPITEM )
				{
					TITAN_ENDURANCE_ITEMINFO* pEndurance = pPlayer->GetTitanManager()->GetTitanItemEnduranceInfo(PyogukItem.Pyoguk[ItemPos].dwDBIdx);
					if( !pEndurance )
					{
						pPlayer->GetTitanManager()->MakeNewEnduranceInfo(pPlayer, &PyogukItem.Pyoguk[ItemPos], eExceptionPyoguk);				
					}
				}
			}
			else
			{
				//¨u¨AIAU ¨AAo¨uC ∷?∷㈢⒋?
				ASSERT(0);
			}
		}
	}
	pPlayer->InitPyogukItemInfo(&PyogukItem);
	
	if( pMessage->dwResult >= MAX_PYOGUKLOAD_ITEM)
		CharacterPyogukItemInfo(pMessage->dwID, atoi((char*)pData[pMessage->dwResult-1].Data[ePI_DBIdx]));
	else //∷⊥U A¨∷¨ui∷?8e A┉∷OoAI¨uㄏ√¨∷c∷?∷㈢ ¨〓∷〓ㄏ┆iAU. 
		PYOGUKMGR->SendPyogukItemInfo(pPlayer);
}

void RFriendNotifyLogouttoClient(LPQUERY pData, LPDBMESSAGE pMessage)
{
	for(DWORD i=0; i<pMessage->dwResult; ++i)
	{
		DWORD ID = atoi((char*)pData[i].Data[eFr_LToPlayerID]);
		CPlayer* pPlayer = (CPlayer *)g_pUserTable->FindUser(ID);
		if(!pPlayer)
		{
			MSG_NAME_DWORD cmsg;
			cmsg.Category = MP_FRIEND;
			cmsg.Protocol = MP_FRIEND_LOGOUT_NOTIFY_TO_AGENT;
			SafeStrCpy(cmsg.Name, (char*)pData[0].Data[eFr_LLogoutName], MAX_NAME_LENGTH+1);
			cmsg.dwData = pMessage->dwID;
			cmsg.dwObjectID = atoi((char*)pData[i].Data[eFr_LToPlayerID]);

			g_Network.Broadcast2AgentServer((char*)&cmsg,sizeof(cmsg));
		}
		else
		{
			MSG_NAME_DWORD msg;
			msg.Category = MP_FRIEND;
			msg.Protocol = MP_FRIEND_LOGOUT_NOTIFY_TO_CLIENT;
			SafeStrCpy(msg.Name, (char*)pData[0].Data[eFr_LLogoutName], MAX_NAME_LENGTH+1);
			msg.dwData = pMessage->dwID;			
			pPlayer->SendMsg(&msg, sizeof(msg));
		}
	}
}

void RWantedLoad(LPQUERY pData, LPDBMESSAGE pMessage)
{
	WANTEDINFO_LOAD info;
	memset(&info, 0, sizeof(WANTEDINFO_LOAD));
	
	DWORD i=0;
	for(i=0; i<pMessage->dwResult; ++i)
	{
		info.m_WantedInfo[i].WantedIDX = (DWORD)atoi((char*)pData[i].Data[eWa_LWantedIDX]);
		info.m_WantedInfo[i].WantedChrIDX = (DWORD)atoi((char*)pData[i].Data[eWa_LPlayerID]);
		info.m_WantedInfo[i].TotalPrize = (DWORD)atoi((char*)pData[i].Data[eWa_LTotlaPrize]);
		SafeStrCpy(info.m_WantedInfo[i].WantedChrName, (char*)pData[i].Data[eWa_LPlayerName], MAX_NAME_LENGTH+1);
		info.m_WantedInfo[i].RegistChrIDX = (DWORD)atoi((char*)pData[i].Data[eWa_LRegistChridx]);
		info.m_WantedInfo[i].TotalVolunteerNum = (int)atoi((char*)pData[i].Data[eWa_LTotalVolunteerNum]);
		info.count++;
	}
	
	WANTEDMGR->LoadWantedList(&info);	
	
	if(pMessage->dwResult >= MAX_QUERY_RESULT)
		WantedLoad(info.m_WantedInfo[i-1].WantedIDX);
	else
	//	g_pServerSystem->SetStart(TRUE);
		GuildLoadGuild(0);
}

void RWantedInfoByUserLogIn(LPQUERY pData, LPDBMESSAGE pMessage)
{
	CPlayer* pPlayer = (CPlayer *)g_pUserTable->FindUser(pMessage->dwID);
	if(!pPlayer)
		return;	

	SEND_WANTEDLIST msg;
	memset(&msg, 0, sizeof(msg));
	msg.Category = MP_WANTED;
	msg.Protocol = MP_WANTED_INFO_BY_USERLOGIN;
//	msg.WantedIdx = (WORD)atoi((char*)pData[0].Data[eWa_LIMyWantedIDX]);
	msg.WantedIdx = (WANTEDTYPE)atoi((char*)pData[0].Data[eWa_LIMyWantedIDX]);
	for(DWORD i=0; i<pMessage->dwResult; ++i)
	{ //ㄏoivāC 1āㄏ┆∷⊥A ㄏ┆N¨ui∷?A∷⊥U.
//		if((WORD)atoi((char*)pData[i].Data[eWa_LIWantedIDX]) != 0)
		if((WANTEDTYPE)atoi((char*)pData[i].Data[eWa_LIWantedIDX]) != 0)
		{
//			msg.List[i].WantedIDX = (WORD)atoi((char*)pData[i].Data[eWa_LIWantedIDX]);
			msg.List[i].WantedIDX = (WANTEDTYPE)atoi((char*)pData[i].Data[eWa_LIWantedIDX]);
			msg.List[i].WantedChrID = (DWORD)atoi((char*)pData[i].Data[eWa_LIWantedChrID]);
			SafeStrCpy(msg.List[i].WantedName, (char*)pData[i].Data[eWa_LIWantedName], MAX_NAME_LENGTH+1);
			SafeStrCpy(msg.List[i].RegistDate, (char*)pData[i].Data[eWa_LIRegistDate], 11);
//			pPlayer->AddWanted((WORD)atoi((char*)pData[i].Data[eWa_LIWantedIDX]));
			pPlayer->AddWanted((WANTEDTYPE)atoi((char*)pData[i].Data[eWa_LIWantedIDX]));
		}		
	}	
	pPlayer->SetWantedIdx(msg.WantedIdx);
	pPlayer->SendMsg(&msg, sizeof(msg));
}

void RWantedRegist(LPQUERY pData, LPDBMESSAGE pMessage)
{
	WANTEDINFO Info;
//	Info.WantedIDX =  (WORD)atoi((char*)pData->Data[eWa_RWanted_idx]);
	Info.WantedIDX =  (WANTEDTYPE)atoi((char*)pData->Data[eWa_RWanted_idx]);
	Info.TotalPrize =  (MONEYTYPE)atoi((char*)pData->Data[eWa_RTotalPrize]);
	Info.WantedChrIDX = (DWORD)atoi((char*)pData->Data[eWa_RWantedChr_idx]);
	SafeStrCpy(Info.WantedChrName, (char*)pData->Data[eWa_RWanted_name], MAX_NAME_LENGTH+1);
	Info.RegistChrIDX = pMessage->dwID;
	Info.TotalVolunteerNum = 0;
	WANTEDMGR->RegistKillerResult(&Info);
}

void RWantedComplete(LPQUERY pData, LPDBMESSAGE pMessage)
{
	CPlayer* pPlayer = NULL;

	SEND_WANTED_NOTCOMPLETE msg;
	msg.Category = MP_WANTED;
	msg.Protocol = MP_WANTED_NOTIFY_NOTCOMPLETE_TO_MAP;
	msg.WantedIDX = (WANTEDTYPE)pMessage->dwID;
	SafeStrCpy(msg.CompleterName, (char*)pData[0].Data[eWa_CCompleterName], MAX_NAME_LENGTH+1);
	msg.type = (BYTE)atoi((char*)pData[0].Data[eWa_CCompleteType]);
	
	for(DWORD i=0; i<pMessage->dwResult; ++i)
	{
		msg.dwObjectID = (DWORD)atoi((char*)pData[i].Data[eWa_CCharacteridx]);
		pPlayer = (CPlayer*)g_pUserTable->FindUser((DWORD)atoi((char*)pData[i].Data[eWa_CCharacteridx]));
		if(!pPlayer)
		{
			//broadcasting
			g_Network.Send2AgentServer((char*)&msg, sizeof(msg));
		}
		else
		{	
			WANTEDMGR->CanNotComplete(pPlayer, (WANTEDTYPE)pMessage->dwID, 
									(char*)pData[0].Data[eWa_CCompleterName], 
									(BYTE)atoi((char*)pData[0].Data[eWa_CCompleteType]));
		}
	}
}

void RWantedDestroyed(LPQUERY pData, LPDBMESSAGE pMessage)
{
	CPlayer* pPlayer = NULL;
	MSG_DWORD msg;
	msg.Category = MP_WANTED;
	msg.dwData = pMessage->dwID;

	for(DWORD i=0; i<pMessage->dwResult; ++i)
	{
		msg.dwObjectID = (DWORD)atol((char*)pData[i].Data[0]);
		pPlayer = (CPlayer*)g_pUserTable->FindUser(msg.dwObjectID);
		if(pPlayer == 0)
		{
			//broadcasting
			msg.Protocol = MP_WANTED_DESTROYED_TO_MAP;
			g_Network.Send2AgentServer((char*)&msg, sizeof(msg));
		}
		else
		{	
			if( pPlayer->IsWantedOwner((WANTEDTYPE)pMessage->dwID) == TRUE )
			{
				pPlayer->DeleteWanted((WANTEDTYPE)pMessage->dwID);
				msg.Protocol = MP_WANTED_DESTROYED_TO_CLIENT;
				pPlayer->SendMsg(&msg, sizeof(msg));
			}
		}
	}
}

void RWantedOrderedList(LPQUERY pData, LPDBMESSAGE pMessage)
{
	CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser(pMessage->dwID);
	if(!pPlayer)
		return;
	
	SEND_WANTNPCLIST msg;
	memset(&msg, 0, sizeof(msg));
	msg.Category = MP_WANTED;
	msg.Protocol = MP_WANTED_ORDERLIST_ACK;
	msg.TotalPage = (WORD)atoi((char*)pData[0].Data[eWa_OLTotal]);
	for(DWORD i=0; i<pMessage->dwResult; ++i)
	{
		msg.WantNpcList[i].WantedIDX = (WANTEDTYPE)atoi((char*)pData[i].Data[eWa_OLWantedIDX]);
		SafeStrCpy(msg.WantNpcList[i].WantedName, (char*)pData[i].Data[eWa_OLWantedChrName], MAX_NAME_LENGTH+1);
		msg.WantNpcList[i].Prize = (MONEYTYPE)atoi((char*)pData[i].Data[eWa_OLPrize]);
		msg.WantNpcList[i].VolunteerNum = (BYTE)atoi((char*)pData[i].Data[eWa_OLVolunteerNum]);
	}
	
	pPlayer->SendMsg(&msg, sizeof(msg));
}

void RWantedMyList(LPQUERY pData, LPDBMESSAGE pMessage)
{
	CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser(pMessage->dwID);
	if(!pPlayer)
		return;
	
	SEND_WANTNPCLIST msg;
	memset(&msg, 0, sizeof(msg));
	msg.Category = MP_WANTED;
	msg.Protocol = MP_WANTED_MYLIST_ACK;
	msg.TotalPage = (WORD)atoi((char*)pData[0].Data[eWa_OLTotal]);
	for(DWORD i=0; i<pMessage->dwResult; ++i)
	{
		msg.WantNpcList[i].WantedIDX = (WANTEDTYPE)atoi((char*)pData[i].Data[eWa_OLWantedIDX]);
		SafeStrCpy(msg.WantNpcList[i].WantedName, (char*)pData[i].Data[eWa_OLWantedChrName], MAX_NAME_LENGTH+1);
		msg.WantNpcList[i].Prize = (MONEYTYPE)atoi((char*)pData[i].Data[eWa_OLPrize]);
		msg.WantNpcList[i].VolunteerNum = (BYTE)atoi((char*)pData[i].Data[eWa_OLVolunteerNum]);
	}
	
	pPlayer->SendMsg(&msg, sizeof(msg));
}

void RWantedSearch(LPQUERY pData, LPDBMESSAGE pMessage)
{
	CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser(pMessage->dwID);
	if(!pPlayer)
		return;
	
	SEND_WANTNPCLIST msg;
	memset(&msg, 0, sizeof(msg));
	msg.Category = MP_WANTED;
	msg.Protocol = MP_WANTED_SEARCH_ACK;
	msg.TotalPage = (WORD)atoi((char*)pData[0].Data[eWa_OLTotal]);
	for(DWORD i=0; i<pMessage->dwResult; ++i)
	{
		msg.WantNpcList[i].WantedIDX = (WANTEDTYPE)atoi((char*)pData[i].Data[eWa_OLWantedIDX]);
		SafeStrCpy(msg.WantNpcList[i].WantedName, (char*)pData[i].Data[eWa_OLWantedChrName], MAX_NAME_LENGTH+1);
		msg.WantNpcList[i].Prize = (MONEYTYPE)atoi((char*)pData[i].Data[eWa_OLPrize]);
		msg.WantNpcList[i].VolunteerNum = (BYTE)atoi((char*)pData[i].Data[eWa_OLVolunteerNum]);
	}
	
	pPlayer->SendMsg(&msg, sizeof(msg));
}
/*
//////////////////////////////////////////////////////////////////

void RFameMunpaUpdate(LPQUERY pData, LPDBMESSAGE pMessage)
{
	DWORD MunpaIDX=0;
	FAMETYPE Val = 0;
	for(DWORD i=0; i<pMessage->dwResult; ++i)
	{
		MunpaIDX = (DWORD)atoi((char*)pData[i].Data[eFa_MunpaIDX]);
		Val = (WORD)atoi((char*)pData[i].Data[eFa_Fameval]);
		FAMEMGR->SetMunpaFame(MunpaIDX, Val);
	}
	
	if(pMessage->dwID >= MAX_MUNPALOAD_LIST)
	{
		FameMunpaUpdate(MunpaIDX, pMessage->dwID);
	}
}*/


// Quest
void RQuestTotalInfo(LPQUERY pData, LPDBMESSAGE pMessage)
{
	SEND_QUEST_TOTALINFO msg;
	memset(&msg.QuestList, 0, sizeof(msg.QuestList));

	CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser(pMessage->dwID);
	if( !pPlayer ) return;

	// client∷?∷㈢ ¨〓∷〓ㄏ┆¨u ∷〓vA¨u iy¨u¨〓..
	msg.Category = MP_QUEST;
	msg.Protocol = MP_QUEST_TOTALINFO;
	msg.dwObjectID = pMessage->dwID;

	if( pMessage->dwResult == 0 )
	{
		msg.wCount = 0;
		pPlayer->SendMsg( &msg, msg.GetSize() );
	}
	else
	{
		CQuestBase* pQuest;
		QFLAG flag;
		WORD QuestIdx;
		WORD count = 0;

		for( DWORD i = 0 ; i < pMessage->dwResult ; ++i )
		{			
			flag.value = (QSTATETYPE)atoi((char*)pData[i].Data[eQi_QuestState]);
			QuestIdx = (WORD)atoi((char*)pData[i].Data[eQi_QuestIdx]);

			if( !flag.IsSet(1) )
			{
				pQuest = new CQuestBase;
				pQuest->Init(QuestIdx, flag.value);

				pPlayer->AddQuest( pQuest );

				// A┉∷OoAI¨uㄏ√¨∷c ∷〓¨々¨u¨uAo ¨uA¨A..
				msg.QuestList[count].QuestIdx = QuestIdx;
				msg.QuestList[count].state = flag.value;
				count++;
			}
		}

		msg.wCount = count;
		pPlayer->SendMsg( &msg, msg.GetSize() );
	}
} 

void RQuestMainQuestLoad(LPQUERY pData, LPDBMESSAGE pMessage)
{
	SEND_MAINQUEST_DATA msg;
	memset(&msg.QuestList, 0, sizeof(msg.QuestList));
	CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser(pMessage->dwID);
	if( !pPlayer ) return;

	// client∷?∷㈢ ¨〓∷〓ㄏ┆¨u ∷〓vA¨u iy¨u¨〓..
	msg.Category = MP_QUEST;
	msg.Protocol = MP_QUEST_MAINDATA_LOAD;
	msg.dwObjectID = pMessage->dwID;				// UserID

	if( pMessage->dwResult == 0 )
	{
		msg.wCount = 0;
		pPlayer->SendMsg( &msg, msg.GetSize() );
	}
	else
	{
		DWORD state;
		WORD QuestIdx=0;
		DWORD EndParam=0;
		DWORD Time=0;
		BYTE bCheckType = 0;
		DWORD dwCheckTime = 0;
		WORD count = 0;

		for( DWORD i = 0 ; i < pMessage->dwResult ; ++i )
		{			
			QuestIdx = (WORD)atoi((char*)pData[i].Data[eQi_QuestIdx]);
			state = (QSTATETYPE)atoi((char*)pData[i].Data[eQi_QuestState]);
			EndParam = (DWORD)atoi((char*)pData[i].Data[eQi_EndParam]);
			Time = (DWORD)atoi((char*)pData[i].Data[eQi_RegistTime]);
			bCheckType = (BYTE)atoi((char*)pData[i].Data[eQi_CheckType]);
			dwCheckTime = (DWORD)atoi((char*)pData[i].Data[eQi_CheckTime]);
			
			// A┉∷OoAI¨uㄏ√¨∷c ∷〓¨々¨u¨uAo ¨uA¨A..
			msg.QuestList[count].QuestIdx = QuestIdx;
			msg.QuestList[count].state.value = state;
			msg.QuestList[count].EndParam = EndParam;
			msg.QuestList[count].Time = Time;
			msg.QuestList[count].CheckType = bCheckType;
			msg.QuestList[count].CheckTime = dwCheckTime;
						
			if( QUESTMGR->SetMainQuestData( pPlayer, QuestIdx, state, EndParam, Time, bCheckType, dwCheckTime ) == FALSE )
			{
				msg.QuestList[count].EndParam = 1;

				QuestMainQuestUpdateToDB( pMessage->dwID, QuestIdx, state, 1, Time );
			}
			count++;
		}
		
		msg.wCount = count;
		pPlayer->SendMsg( &msg, msg.GetSize() );		

		if( count >= 100 )
		{

			QuestMainQuestLoad( pMessage->dwID, (int)QuestIdx );
			return;
		}
	}	

	// 皋牢 涅胶飘 肺靛饶 涅胶飘酒捞袍沥焊 掘绢柯促...
	QuestItemload( pMessage->dwID );
//	QuestSubQuestLoad( pMessage->dwID );
}


void RQuestSubQuestLoad(LPQUERY pData, LPDBMESSAGE pMessage)
{
	SEND_SUBQUEST_DATA msg;
	memset(&msg.QuestList, 0, sizeof(msg.QuestList));

	CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser(pMessage->dwID);
	if( !pPlayer ) return;

	// client∷?∷㈢ ¨〓∷〓ㄏ┆¨u ∷〓vA¨u iy¨u¨〓..
	msg.Category = MP_QUEST;
	msg.Protocol = MP_QUEST_SUBDATA_LOAD;
	msg.dwObjectID = pMessage->dwID;				// UserID

	if( pMessage->dwResult == 0 )
	{
		msg.wCount = 0;
		pPlayer->SendMsg( &msg, msg.GetSize() );
	}
	else
	{
		WORD QuestIdx;
		WORD SubQuestIdx;
		DWORD state;
		DWORD time;
		WORD count = 0;

		for( DWORD i = 0 ; i < pMessage->dwResult ; ++i )
		{			
			QuestIdx = (WORD)atoi((char*)pData[i].Data[eQsi_QuestIdx]);
			SubQuestIdx = (WORD)atoi((char*)pData[i].Data[eQsi_SubQuestIdx]);
			state = (DWORD)atoi((char*)pData[i].Data[eQsi_Data]);
			time = (DWORD)atoi((char*)pData[i].Data[eQsi_RegistTime]);
			
			// A┉∷OoAI¨uㄏ√¨∷c ∷〓¨々¨u¨uAo ¨uA¨A..
			msg.QuestList[count].QuestIdx = QuestIdx;
			msg.QuestList[count].SubQuestIdx = SubQuestIdx;
			msg.QuestList[count].state = state;
			msg.QuestList[count].time = time;
			count++;

			QUESTMGR->SetSubQuestData( pPlayer, QuestIdx, SubQuestIdx, state, time );
		}

		msg.wCount = count;
		pPlayer->SendMsg( &msg, msg.GetSize() );

		if( count >= 100 )
		{
			QuestSubQuestLoad( pMessage->dwID, (int)QuestIdx );
			return;
		}
	}	

//	QuestItemload( pMessage->dwID );

	// 辑宏涅胶飘 肺靛饶 皋牢涅胶飘 肺靛茄促.
	QuestMainQuestLoad( pMessage->dwID );
}


void RQuestItemLoad(LPQUERY pData, LPDBMESSAGE pMessage)
{
	CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser(pMessage->dwID);
	if( !pPlayer ) return;

	SEND_QUESTITEM msg;
	memset(&msg.ItemList, 0, sizeof(msg.ItemList));
	// client隆??隆?垄莽 隆搂隆镁隆?隆镁篓?漏陋隆搂u 垄庐篓煤隆?隆镁vA隆搂u 垄庐iy隆搂u隆搂隆镁..
	msg.Category = MP_QUEST;
	msg.Protocol = MP_QUEST_ITEM_LOAD;
	msg.dwObjectID = pMessage->dwID;				// UserID

	if( pMessage->dwResult == 0 )
	{
		msg.wCount = 0;
		pPlayer->SendMsg( &msg, msg.GetSize() );
	}
	else
	{
		DWORD ItemIdx=0;
		int Count=0;
		WORD wcount=0;
		WORD QuestIdx=0;

		for( DWORD i = 0 ; i < pMessage->dwResult ; ++i )
		{
			ItemIdx = (DWORD)atoi((char*)pData[i].Data[eQit_ItemIdx]);
			Count = (int)atoi((char*)pData[i].Data[eQit_ItemCount]);			
			QuestIdx = (WORD)atoi((char*)pData[i].Data[eQit_QuestIdx]);
			
			// A┉∷OoAI¨uㄏ√¨∷c ∷〓¨々¨u¨uAo ¨uA¨A..
			msg.ItemList[wcount].ItemIdx = ItemIdx;
			msg.ItemList[wcount].Count = Count;
			msg.ItemList[wcount].QuestIdx = QuestIdx;
			wcount++;
			
			QUESTMGR->SetQuestItem( pPlayer, QuestIdx, ItemIdx, Count );
		}
		
		msg.wCount = wcount;
		pPlayer->SendMsg( &msg, msg.GetSize() );	

		if( wcount >= 100 )
		{
			QuestItemload( pMessage->dwID, (int)QuestIdx );
		}
	}
}

void RJournalGetList(LPQUERY pData, LPDBMESSAGE pMessage)
{
	CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser(pMessage->dwID);
	if( !pPlayer ) return;
	SEND_JOURNALLIST msg;
	memset(&msg, NULL, sizeof(msg));
	msg.Category = MP_JOURNAL;
	msg.Protocol = MP_JOURNAL_GETLIST_ACK;
	for(DWORD i=0; i<pMessage->dwResult; ++i)
	{
		msg.m_Info[i].Index = (DWORD)atoi((char*)pData[i].Data[eJnl_Index]);
		msg.m_Info[i].Kind = (BYTE)atoi((char*)pData[i].Data[eJnl_Kind]);
		msg.m_Info[i].Param = (DWORD)atoi((char*)pData[i].Data[eJnl_Param]);
		msg.m_Info[i].Param_2 = (DWORD)atoi((char*)pData[i].Data[eJnl_Param_2]);
		msg.m_Info[i].Param_3 = (DWORD)atoi((char*)pData[i].Data[eJnl_Param_3]);
		msg.m_Info[i].bSaved = (BYTE)atoi((char*)pData[i].Data[eJnl_bSaved]);
		SafeStrCpy(msg.m_Info[i].ParamName, (char*)pData[i].Data[eJnl_ParamName], MAX_NAME_LENGTH+1);
		SafeStrCpy(msg.m_Info[i].RegDate, (char*)pData[i].Data[eJnl_RegDate], 11);
		char buf[256]={ 0, };
		strcpy(buf, (char*)pData[i].Data[eJnl_RegDate]);
		msg.wCount++;
	}
	pPlayer->SendMsg(&msg, msg.GetSize());
}

void RSaveMapChangePointReturn(LPQUERY pData, LPDBMESSAGE pMessage)
{
	DWORD dwConnectionIndex = (DWORD)atoi((char*)pData->Data[0]);
	DWORD Map = (DWORD)atoi((char*)pData->Data[1]);

	MSG_DWORD msg;
	msg.Category = MP_USERCONN;
	msg.Protocol = MP_USERCONN_MAP_OUT;
	msg.dwObjectID = pMessage->dwID;
	msg.dwData = Map;
	
	g_Network.Send2Server(dwConnectionIndex, (char*)&msg, sizeof(msg));
}


// 娄庐娄卢娄卢娄卢娄卢娄卢娄卢娄卢娄卢娄卢娄卢娄卢娄卢娄卢娄卢娄卢娄卢娄卢娄卢娄卢娄卢娄卢娄卢娄卢娄卢娄卢娄卢娄卢娄卢娄卢娄炉
// 娄颅RaMa		-> ShopItemLoading                             娄颅
// 娄卤娄卢娄卢娄卢娄卢娄卢娄卢娄卢娄卢娄卢娄卢娄卢娄卢娄卢娄卢娄卢娄卢娄卢娄卢娄卢娄卢娄卢娄卢娄卢娄卢娄卢娄卢娄卢娄卢娄卢娄掳
void RCharacterShopItemInfo(LPQUERY pData, LPDBMESSAGE pMessage)
{
	CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser(pMessage->dwID);
	if( !pPlayer ) return;

	if(pPlayer->GetInited() == FALSE)
		return;
	if(pMessage->dwResult == 0)	
		return;

	// ?枚?莽陆?掳拢 ?煤?氓
	SHOP_ITEM	ShopItem;
	SHOP_ITEM	NewShopItem;
	memset(&ShopItem, 0, sizeof(SHOP_ITEM));
	memset(&NewShopItem, 0, sizeof(SHOP_ITEM));

	int itempos=0;
	DWORD nitemcnt=0;
	DWORD lditemcnt = 0;
	if( pMessage->dwResult<=SLOT_SHOPITEM_IMSI )
		lditemcnt = pMessage->dwResult;
	else
	{
		lditemcnt = SLOT_SHOPITEM_IMSI;
		pPlayer->GetShopItemManager()->SendMsgDwordToPlayer( MP_ITEM_SHOPITEM_SLOTOVERITEM, pMessage->dwResult );
	}

	// magi82 - 鞘夸绝绢辑 林籍贸府
	//stTIME curtime, endtime;
	//curtime.value = GetCurTime();

	for( DWORD i = 0 ; i < lditemcnt ; ++i )
	{
		itempos = atoi((char*)pData[i].Data[eMItm_Position]);

		if(itempos == 0)
		{
			NewShopItem.Item[nitemcnt].dwDBIdx = (DWORD)atoi((char*)pData[i].Data[eMItm_ItemDBIdx]);
			NewShopItem.Item[nitemcnt].wIconIdx = (WORD)atoi((char*)pData[i].Data[eMItm_ItemIdx]);
			NewShopItem.Item[nitemcnt].Position = 0;
			NewShopItem.Item[nitemcnt].Durability = (WORD)atoi((char*)pData[i].Data[eMItm_Durability]);
			NewShopItem.Item[nitemcnt].ItemParam = (ITEMPARAM)atoi((char*)pData[i].Data[eMItm_Param]);	
			NewShopItem.Item[nitemcnt].ItemStatic = (WORD)atoi((char*)pData[i].Data[eMItm_Static]); // 2014-12-7 商城读取
			NewShopItem.Item[nitemcnt].ItemGrow = (DWORD)atoi((char*)pData[i].Data[eMItm_Grow]);    // 2014-12-7 商城成长读取

			SafeStrCpy(ShopItem.Item[itempos].PowerUp,(char*)pData[i].Data[eMI_PowerUp], MAX_NAME_LENGTH+1);
			SafeStrCpy(ShopItem.Item[itempos].Green,(char*)pData[i].Data[eMI_Green], MAX_NAME_LENGTH+1);
			NewShopItem.Item[nitemcnt].Grade30 = (DWORD)atoi((char*)pData[i].Data[eMItm_Grade30]);
			++nitemcnt;
		}
		else
		{			
			itempos -= TP_SHOPITEM_START;

			if( ShopItem.Item[itempos].dwDBIdx != 0 )
			{
				NewShopItem.Item[nitemcnt].dwDBIdx = (DWORD)atoi((char*)pData[i].Data[eMItm_ItemDBIdx]);
				NewShopItem.Item[nitemcnt].wIconIdx = (WORD)atoi((char*)pData[i].Data[eMItm_ItemIdx]);
				NewShopItem.Item[nitemcnt].Position = 0;
				NewShopItem.Item[nitemcnt].Durability = (WORD)atoi((char*)pData[i].Data[eMItm_Durability]);
				NewShopItem.Item[nitemcnt].ItemParam = (ITEMPARAM)atoi((char*)pData[i].Data[eMItm_Param]);	
				NewShopItem.Item[nitemcnt].ItemStatic = (WORD)atoi((char*)pData[i].Data[eMItm_Static]); // 2014-12-7 商城读取
				NewShopItem.Item[nitemcnt].ItemGrow = (DWORD)atoi((char*)pData[i].Data[eMItm_Grow]);    // 2014-12-7 商城读取

				SafeStrCpy(ShopItem.Item[itempos].PowerUp,(char*)pData[i].Data[eMI_PowerUp], MAX_NAME_LENGTH+1);
				SafeStrCpy(ShopItem.Item[itempos].Green,(char*)pData[i].Data[eMI_Green], MAX_NAME_LENGTH+1);
				NewShopItem.Item[nitemcnt].Grade30 = (DWORD)atoi((char*)pData[i].Data[eMItm_Grade30]);
				++nitemcnt;

				continue;
			}
						
			ShopItem.Item[itempos].dwDBIdx = (DWORD)atoi((char*)pData[i].Data[eMItm_ItemDBIdx]);
			ShopItem.Item[itempos].wIconIdx = (WORD)atoi((char*)pData[i].Data[eMItm_ItemIdx]);
			ShopItem.Item[itempos].Position = (WORD)atoi((char*)pData[i].Data[eMItm_Position]);
			ShopItem.Item[itempos].Durability = (WORD)atoi((char*)pData[i].Data[eMItm_Durability]);
			ShopItem.Item[itempos].ItemParam = (ITEMPARAM)atoi((char*)pData[i].Data[eMItm_Param]);
			ShopItem.Item[itempos].ItemStatic = (WORD)atoi((char*)pData[i].Data[eMItm_Static]);         // 2014-12-7 商城读取
			ShopItem.Item[itempos].ItemGrow = (DWORD)atoi((char*)pData[i].Data[eMItm_Grow]);           // 2014-12-7 商城读取

			SafeStrCpy(ShopItem.Item[itempos].PowerUp,(char*)pData[i].Data[eMI_PowerUp], MAX_NAME_LENGTH+1);
			SafeStrCpy(ShopItem.Item[itempos].Green,(char*)pData[i].Data[eMI_Green], MAX_NAME_LENGTH+1);
			ShopItem.Item[nitemcnt].Grade30 = (DWORD)atoi((char*)pData[i].Data[eMItm_Grade30]);
		}


		// 荤侩窍妨搁 橇肺矫历俊 Order by item_dbidx眠啊
		//StartItemDBIdx = ShopItem.Item[itempos].dwDBIdx;
	}


	// 器瘤记捞 绝绰 货肺款 酒捞袍捞 乐栏搁.
	if( nitemcnt )
	{
		for(DWORD ni=0; ni<nitemcnt; ni++)
		{
			for(int si=0; si<SLOT_SHOPITEM_IMSI; si++)
			{
				// 磊府啊 绝绰巴捞 乐栏搁 悸泼秦霖促.
				if( ShopItem.Item[si].dwDBIdx == 0 )
				{
					ShopItem.Item[si] = NewShopItem.Item[ni];
					ShopItem.Item[si].Position = si+TP_SHOPITEM_START;

					// DB俊 Position阑 Update秦霖促.
					ItemMoveShopUpdateToDB( pPlayer->GetID(), pPlayer->GetUserID(), ShopItem.Item[si].dwDBIdx, ShopItem.Item[si].Position, 0, 0  );
					break;
				}
			}
		}
	}

	pPlayer->InitShopItemInfo( &ShopItem );
	pPlayer->GetShopItemManager()->CheckAvatarEndtime();
	pPlayer->SendShopItemInfo();
}


void RShopItemInvenInfo( LPQUERY pData, LPDBMESSAGE pMessage )
{
	CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser(pMessage->dwID);
	if( !pPlayer ) return;
	ITEMBASE ItemBase[TABCELL_SHOPINVEN_NUM+40];
	ITEMBASE ItemOverlap[TABCELL_SHOPINVEN_NUM+40];
	memset( ItemBase, 0, sizeof(ITEMBASE)*(TABCELL_SHOPINVEN_NUM+40) );
	memset( ItemOverlap, 0, sizeof(ITEMBASE)*(TABCELL_SHOPINVEN_NUM+40) );
	int OverlapCount=0;
	if( pMessage->dwResult == 0 )
	{
		pPlayer->InitShopInvenInfo( ItemBase );
		PetWearItemInfo(pPlayer->GetID(), TP_PETWEAR_START, TP_PETWEAR_END);
		return;
	}
	for(DWORD  i = 0; i < pMessage->dwResult; i++)
	{
		POSTYPE ItemPos = atoi((char*)pData[i].Data[eCI_Position]);
		if(ItemPos >= TP_SHOPINVEN_START && ItemPos < TP_SHOPINVEN_END/*-40*/)
		{
			ItemPos -= TP_SHOPINVEN_START;
			if( ItemPos >= (TABCELL_SHOPINVEN_NUM+40) || ItemBase[ItemPos].dwDBIdx )
			{
				ItemOverlap[OverlapCount].dwDBIdx = (DWORD)atoi((char*)pData[i].Data[eCI_DBIDX]);
				ItemOverlap[OverlapCount].wIconIdx = (WORD)atoi((char*)pData[i].Data[eCI_IDX]);
				ItemOverlap[OverlapCount].Position = (POSTYPE)atoi((char*)pData[i].Data[eCI_Position]);
				ItemOverlap[OverlapCount].QuickPosition = (POSTYPE)atoi((char*)pData[i].Data[eCI_QPosition]);
				ItemOverlap[OverlapCount].Durability = (DURTYPE)atoi((char*)pData[i].Data[eCI_Durability]);
				ItemOverlap[OverlapCount].ItemParam = (ITEMPARAM)atoi((char*)pData[i].Data[eCI_Param]);
				ItemOverlap[OverlapCount].ItemStatic = (WORD)atoi((char*)pData[i].Data[eCI_Static]);
				ItemOverlap[OverlapCount].ItemGrow = (DWORD)atoi((char*)pData[i].Data[eCI_Grow]);
				ItemOverlap[OverlapCount].Grade30 = (DWORD)atoi((char*)pData[i].Data[eCI_Grade30]);
				++OverlapCount;
			}
			else
			{
				ItemBase[ItemPos].dwDBIdx = (DWORD)atoi((char*)pData[i].Data[eCI_DBIDX]);
				ItemBase[ItemPos].wIconIdx = (WORD)atoi((char*)pData[i].Data[eCI_IDX]);
				ItemBase[ItemPos].Position = (POSTYPE)atoi((char*)pData[i].Data[eCI_Position]);
				ItemBase[ItemPos].QuickPosition = (POSTYPE)atoi((char*)pData[i].Data[eCI_QPosition]);
				ItemBase[ItemPos].Durability = (DURTYPE)atoi((char*)pData[i].Data[eCI_Durability]);
				ItemBase[ItemPos].ItemParam = (ITEMPARAM)atoi((char*)pData[i].Data[eCI_Param]);
				ItemBase[ItemPos].ItemStatic = (WORD)atoi((char*)pData[i].Data[eCI_Static]);
				ItemBase[ItemPos].ItemGrow = (DWORD)atoi((char*)pData[i].Data[eCI_Grow]);
				ItemBase[ItemPos].Grade30 = (DWORD)atoi((char*)pData[i].Data[eCI_Grade30]);
			}
		}
	}
	for(int n=0; n<OverlapCount; n++)
	{
		int m=0;
		for(m=0; m<(TABCELL_SHOPINVEN_NUM+40); m++)
		{
			if( ItemBase[m].dwDBIdx == 0 )
			{
				ItemBase[m] = ItemOverlap[n];
				ItemBase[m].Position = m+TP_SHOPINVEN_START;
				ItemMoveUpdateToDB( pPlayer->GetID(), 0, 0, ItemBase[m].dwDBIdx, ItemBase[m].Position );
				break;
			}
		}
		if( m == (TABCELL_SHOPINVEN_NUM+40) )
		{
			break;
		}
	}
	pPlayer->InitShopInvenInfo( ItemBase );
	PetWearItemInfo(pPlayer->GetID(), TP_PETWEAR_START, TP_PETWEAR_END);
}

// 娄庐娄卢娄卢娄卢娄卢娄卢娄卢娄卢娄卢娄卢娄卢娄卢娄卢娄卢娄卢娄卢娄卢娄卢娄卢娄卢娄卢娄卢娄卢娄卢娄卢娄卢娄卢娄卢娄卢娄卢娄炉
// 娄颅RaMa - 04.11.05  ->ShopItemUsing                          娄颅
// 娄卤娄卢娄卢娄卢娄卢娄卢娄卢娄卢娄卢娄卢娄卢娄卢娄卢娄卢娄卢娄卢娄卢娄卢娄卢娄卢娄卢娄卢娄卢娄卢娄卢娄卢娄卢娄卢娄卢娄卢娄掳
void RUsingShopItemInfo( LPQUERY pData, LPDBMESSAGE pMessage )
{
	CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser(pMessage->dwID);
	if( !pPlayer ) return;

	if( pMessage->dwResult == 0 )
	{
		pPlayer->SetInitState( PLAYERINITSTATE_SHOPITEM_USEDINFO, MP_USERCONN_GAMEIN_SYN );
		return;
	}

	SEND_SHOPITEM_USEDINFO msg;
	msg.Category = MP_ITEM;
	msg.Protocol = MP_ITEM_SHOPITEM_USEDINFO;


	for(DWORD i=0; i<pMessage->dwResult; i++)
	{
		msg.Item[msg.ItemCount].ItemBase.wIconIdx	= (WORD)atoi((char*)pData[i].Data[eUMtm_ItemIdx]);
		msg.Item[msg.ItemCount].ItemBase.dwDBIdx	= (DWORD)atoi((char*)pData[i].Data[eUMtm_DBIdx]);
		msg.Item[msg.ItemCount].Param				= (DWORD)atoi((char*)pData[i].Data[eUMtm_Param]);
		msg.Item[msg.ItemCount].BeginTime.value		= (DWORD)atoi((char*)pData[i].Data[eUMtm_BTime]);
		msg.Item[msg.ItemCount].Remaintime			= (DWORD)atoi((char*)pData[i].Data[eUMtm_RTime]);
		msg.Item[msg.ItemCount].ItemBase.Position	= (POSTYPE)atoi((char*)pData[i].Data[eUMtm_Position]);

		if( pPlayer->GetShopItemManager()->UsedShopItem(
			&msg.Item[msg.ItemCount].ItemBase,
			msg.Item[msg.ItemCount].Param,
			msg.Item[msg.ItemCount].BeginTime,
			msg.Item[msg.ItemCount].Remaintime) )
		{
			// 厘厚窍绊 乐绰 酒官鸥 酒捞袍
			if( msg.Item[msg.ItemCount].Param == eShopItemUseParam_EquipAvatar )
				pPlayer->GetShopItemManager()->PutOnAvatarItem( 
				msg.Item[msg.ItemCount].ItemBase.wIconIdx, msg.Item[msg.ItemCount].ItemBase.Position, FALSE );

			++msg.ItemCount;
		}
		else
		{
			memset( &msg.Item[msg.ItemCount], 0, sizeof(SHOPITEMBASE) );	//努扼捞攫飘肺 皋技瘤 哎鞘夸啊 绝促.
		}

		if( msg.ItemCount >= 100 )
		{
			g_Console.LOG( 4, "UsingShopItemInfo over 100 !!" );
			break;
		}

//		++msg.ItemCount;	//if巩 救栏肺 颗辫 KES
	}

	pPlayer->GetShopItemManager()->CalcPlusTime( 0, 0 );

	//
	pPlayer->SetInitState( PLAYERINITSTATE_SHOPITEM_USEDINFO, MP_USERCONN_GAMEIN_SYN );

	// 
	pPlayer->SendMsg(&msg, msg.GetSize());
}


void RSavedMovePointInfo( LPMIDDLEQUERY pData, LPDBMESSAGE pMessage )
{
	CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser(pMessage->dwID);
	if( !pPlayer ) return;

	pPlayer->GetShopItemManager()->ReleseMovePoint();

	SEND_MOVEDATA_INFO msg;
	msg.Category = MP_ITEM;
	msg.Protocol = MP_ITEM_SHOPITEM_MPINFO;

	for(DWORD i=0; i<pMessage->dwResult; i++)
	{
		msg.Data[i].DBIdx = (DWORD)atoi((char*)pData[i].Data[eMPt_DBIdx]);
		strncpy(msg.Data[i].Name, (char*)pData[i].Data[eMPt_Name], MAX_SAVEDMOVE_NAME-1);
		msg.Data[i].MapNum = (WORD)atoi((char*)pData[i].Data[eMPt_MapNum]);
		msg.Data[i].Point.value = (DWORD)atoi((char*)pData[i].Data[eMPt_MapPoint]);
		++msg.Count;

		//
		pPlayer->GetShopItemManager()->AddMovePoint( &msg.Data[i] );
	}

	// 甘俊 甸绢坷搁 茄锅 焊郴霖促.
	if( pPlayer->GetSavePointInit() )
	{
		msg.bInited = 1;
		pPlayer->SendMsg(&msg, msg.GetSize());
	}
	else
	{
		msg.bInited = 0;
		pPlayer->SendMsg(&msg, msg.GetSize());
		pPlayer->SetSavePointInit( TRUE );
	}
}

void RSavedMovePointInsert( LPMIDDLEQUERY pData, LPDBMESSAGE pMessage )
{
	CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser(pMessage->dwID);
	if( !pPlayer ) return;

	if( pMessage->dwResult == 0 )	
		return;
	
	SEND_MOVEDATA_SIMPLE msg;
	msg.Category = MP_ITEM;
	msg.Protocol = MP_ITEM_SHOPITEM_SAVEPOINT_ADD_ACK;

	for(DWORD i=0; i<pMessage->dwResult; i++)
	{
		msg.Data.DBIdx = (DWORD)atoi((char*)pData[i].Data[eMPt_DBIdx]);
		strncpy(msg.Data.Name, (char*)pData[i].Data[eMPt_Name], MAX_SAVEDMOVE_NAME-1);
		msg.Data.MapNum = (WORD)atoi((char*)pData[i].Data[eMPt_MapNum]);
		msg.Data.Point.value = (DWORD)atoi((char*)pData[i].Data[eMPt_MapPoint]);

		//
		pPlayer->GetShopItemManager()->AddMovePoint( &msg.Data );
	}

	//
	pPlayer->SendMsg(&msg, sizeof(msg));

}


void RIsUseAbleShopAvatarItem( LPQUERY pData, LPDBMESSAGE pMessage )
{
	CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser(pMessage->dwID);
	if( !pPlayer ) return;

	WORD itemidx = (WORD)atoi((char*)pData->Data[0]);
	DWORD dbidx = (DWORD)atoi((char*)pData->Data[1]);
	DWORD param = (DWORD)atoi((char*)pData->Data[2]);
	DWORD begintime = (DWORD)atoi((char*)pData->Data[3]);
	DWORD remaintime = (DWORD)atoi((char*)pData->Data[4]);
	WORD itempos = (WORD)atoi((char*)pData->Data[5]);

	if( param == 9999 )
	{
		MSG_WORD2 msg;
		SetProtocol( &msg, MP_ITEM, MP_ITEM_SHOPITEM_AVATAR_USE_NACK );
		msg.wData1 = itemidx;
		msg.wData2 = itempos;
		pPlayer->SendMsg( &msg, sizeof(msg) );
		return;
	}

	//
	const ITEMBASE* pitembase = ITEMMGR->GetItemInfoAbsIn( pPlayer, itempos );
	if( !pitembase->dwDBIdx )
	{
		MSG_WORD2 msg;
		SetProtocol( &msg, MP_ITEM, MP_ITEM_SHOPITEM_AVATAR_USE_NACK );
		msg.wData1 = itemidx;
		msg.wData2 = itempos;
		pPlayer->SendMsg( &msg, sizeof(msg) );
		return;
	}

	SHOPITEMBASE shopitem;
	shopitem.ItemBase = *pitembase;
	shopitem.Param = eShopItemUseParam_EquipAvatar;
	shopitem.BeginTime.value = begintime;
	shopitem.Remaintime = remaintime;
	pPlayer->GetShopItemManager()->AddShopItemUse( &shopitem );

	if( !pPlayer->GetShopItemManager()->PutOnAvatarItem( itemidx, itempos ) )
	{
		MSG_WORD2 msg;
		SetProtocol( &msg, MP_ITEM, MP_ITEM_SHOPITEM_AVATAR_USE_NACK );
		msg.wData1 = itemidx;
		msg.wData2 = itempos;
		pPlayer->SendMsg( &msg, sizeof(msg) );
		return;
	}


	SEND_SHOPITEM_BASEINFO msg;
	SetProtocol( &msg, MP_ITEM, MP_ITEM_SHOPITEM_AVATAR_USE_ACKDB );
	msg.ShopItemIdx = itemidx;
	msg.ShopItemPos = itempos;
	msg.ShopItemBase = shopitem;
	pPlayer->SendMsg( &msg, sizeof(msg) );
}


void RIsAbleShopEquipItem( LPQUERY pData, LPDBMESSAGE pMessage )
{
	CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser(pMessage->dwID);
	if( !pPlayer ) return;

	WORD itemidx = (WORD)atoi((char*)pData->Data[0]);
	DWORD dbidx = (DWORD)atoi((char*)pData->Data[1]);
	DWORD param = (DWORD)atoi((char*)pData->Data[2]);
	DWORD begintime = (DWORD)atoi((char*)pData->Data[3]);
	DWORD remaintime = (DWORD)atoi((char*)pData->Data[4]);
	WORD itempos = (WORD)atoi((char*)pData->Data[5]);
	WORD toitemidx = (WORD)atoi((char*)pData->Data[6]);
	WORD toitempos = (WORD)atoi((char*)pData->Data[7]);

	SHOPITEMBASE shopitem;

	int rt = 0;
	if( param == 9999 )
		goto _EquipMove_Failed;

	const ITEMBASE* pitembase = ITEMMGR->GetItemInfoAbsIn( pPlayer, itempos );
	if( !pitembase->dwDBIdx )
		goto _EquipMove_Failed;

	shopitem.ItemBase = *pitembase;
	shopitem.Param = param;
	shopitem.BeginTime.value = begintime;
	shopitem.Remaintime = remaintime;
	pPlayer->GetShopItemManager()->AddShopItemUse( &shopitem );

	if(EI_TRUE == (rt = ITEMMGR->MoveItem(pPlayer, itemidx, itempos, toitemidx, toitempos)))
	{
		MSG_ITEM_MOVE_ACK msg;
		SetProtocol( &msg, MP_ITEM, MP_ITEM_MOVE_ACK );
		msg.wFromItemIdx = itemidx;
		msg.FromPos = itempos;
		msg.wToItemIdx = toitemidx;
		msg.ToPos = toitempos;
		ITEMMGR->SendAckMsg(pPlayer, &msg, sizeof(msg));

		SEND_SHOPITEM_BASEINFO smsg;
		SetProtocol( &smsg, MP_ITEM, MP_ITEM_SHOPITEM_EQUIP_USEINFODB );
		smsg.ShopItemIdx = itemidx;
		smsg.ShopItemPos = toitempos;
		smsg.ShopItemBase = shopitem;
		pPlayer->SendMsg( &smsg, sizeof(smsg) );

		return;
	}

_EquipMove_Failed:
	MSG_ITEM_ERROR msg;
	SetProtocol( &msg, MP_ITEM, MP_ITEM_ERROR_NACK );
	msg.dwObjectID = pPlayer->GetID();
	msg.ECode = eItemUseErr_Move;
	ITEMMGR->SendErrorMsg(pPlayer, &msg, sizeof(msg), rt);
}



void RGuildLoadGuild(LPQUERY pData, LPDBMESSAGE pMessage)
{
	LOAD_GUILD LoadGuildInfo;
	DWORD dwTime = 0;
	memset(&LoadGuildInfo, 0, sizeof(LOAD_GUILD));
	for(DWORD i=0; i<pMessage->dwResult; ++i)
	{
		LoadGuildInfo.GuildInfo[i].GuildIdx = atoi((char*)pData[i].Data[eGu_LGIdx]);
		SafeStrCpy(LoadGuildInfo.GuildInfo[i].GuildName, (char*)pData[i].Data[eGu_LGName], MAX_GUILD_NAME + 1);
		LoadGuildInfo.GuildInfo[i].MapNum = atoi((char*)pData[i].Data[eGu_LGLocation]);
		LoadGuildInfo.GuildInfo[i].MasterIdx = atoi((char*)pData[i].Data[eGu_LGMasterIdx]);
		LoadGuildInfo.GuildInfo[i].GuildLevel = atoi((char*)pData[i].Data[eGu_LGLevel]);
		LoadGuildInfo.GuildInfo[i].UnionIdx = atoi((char*)pData[i].Data[eGu_LGUnionIdx]);
		SafeStrCpy(LoadGuildInfo.GuildInfo[i].MasterName, (char*)pData[i].Data[eGu_LGMasterName], MAX_NAME_LENGTH+1);
		LoadGuildInfo.GuildMoney[i] = atoi((char*)pData[i].Data[eGu_LGMoney]);
		LoadGuildInfo.GuildInfo[i].MarkName = atoi((char*)pData[i].Data[eGu_LGMarkName]);
		//SW070103 巩颇器牢飘俺急
		LoadGuildInfo.GuildInfo[i].LvUpCounter = atoi((char*)pData[i].Data[eGu_LGMarkName+2]);
		LoadGuildInfo.GuildInfo[i].bNeedMasterChecking = atoi((char*)pData[i].Data[eGu_LGMarkName+3]);

		GUILDMGR->RegistGuild(&LoadGuildInfo.GuildInfo[i], LoadGuildInfo.GuildMoney[i]);
		dwTime = (DWORD)atoi((char*)pData[i].Data[eGu_LGMarkName+1]);
		if( dwTime )
			GUILDUNIONMGR->AddEntryTime( LoadGuildInfo.GuildInfo[i].GuildIdx, dwTime );

	}

	ASSERT(pMessage->dwResult <= MAX_QUERY_RESULT);
	if(pMessage->dwResult >= MAX_QUERY_RESULT)
	{
		GuildLoadGuild(LoadGuildInfo.GuildInfo[99].GuildIdx);
	}
	else
	{
		GuildLoadNotice(0);
		GuildLoadMember(0);
		// RaMa - Guild Tournament - Guild沥焊 佬绢柯 饶俊 佬绰促.
		GuildTournamentInfoLoad();
		// RaMa - SiegeWar 眠啊
		if( SIEGEWARMGR->IsNeedLoadSiegeInfo() )		
			SiegeWarInfoLoad( SIEGEWARMGR->GetSiegeMapNum() );
		
		//magi82 - 巩窍积 啊涝祈府矫胶袍 /////////////////////////////////////////
		GuildTraineeInfo(0);

		GuildTraineeGuildInfo(0);

		GuildTraineeGuildIntroInfo(0);
		//////////////////////////////////////////////////////////////////////////
	}
}

// 06. 03. 巩颇傍瘤 - 捞康霖
void RGuildLoadNotice(LPMIDDLEQUERY pData, LPDBMESSAGE pMessage)
{
	LOAD_GUILDNOTICE GuildNotice;
	memset(&GuildNotice, 0, sizeof(LOAD_GUILDNOTICE));
	DWORD i=0;
	for(i=0; i<pMessage->dwResult; ++i)
	{
		GuildNotice.DBIdx[i] = atoi((char*)pData[i].Data[eGu_GNDBIdx]);
		GuildNotice.GuildIdx[i] = atoi((char*)pData[i].Data[eGu_GNGuildIdx]);
		strcpy(GuildNotice.GuildNotice[i], (char*)pData[i].Data[eGu_GNGuildNotice]);

		GUILDMGR->RegistGuildNotice(GuildNotice.GuildIdx[i], GuildNotice.GuildNotice[i]);
	}

//	ASSERT(pMessage->dwResult <= MAX_QUERY_RESULT);
	if(pMessage->dwResult == MAX_QUERY_RESULT)
	{
		GuildLoadNotice(GuildNotice.DBIdx[i]);
	}
}

void RGuildLoadMember(LPQUERY pData, LPDBMESSAGE pMessage)
{
	LOAD_GUILDMEMBER GuildMemberInfo;
	memset(&GuildMemberInfo, 0, sizeof(LOAD_GUILDMEMBER));
	for(DWORD i=0; i<pMessage->dwResult; ++i)
	{
		GuildMemberInfo.Member[i].MemberInfo.MemberIdx = atoi((char*)pData[i].Data[eGu_LMIdx]);
		SafeStrCpy(GuildMemberInfo.Member[i].MemberInfo.MemberName, (char*)pData[i].Data[eGu_LMName], MAX_NAME_LENGTH + 1);
		GuildMemberInfo.Member[i].GuildIdx = atoi((char*)pData[i].Data[eGu_LMGuildIdx]);
		GuildMemberInfo.Member[i].MemberInfo.Memberlvl = atoi((char*)pData[i].Data[eGu_LMLevel]);
		GuildMemberInfo.Member[i].MemberInfo.Rank = atoi((char*)pData[i].Data[eGu_LMRank]);
				
		GUILDMGR->LoadMembers(&GuildMemberInfo.Member[i]) ;
	}

	ASSERT(pMessage->dwResult <= MAX_QUERY_RESULT);
	if(pMessage->dwResult == MAX_QUERY_RESULT)
	{
		GuildLoadMember(GuildMemberInfo.Member[99].MemberInfo.MemberIdx);
	}
	else
	{
		GuildLoadMark(0);
	}	
}

void RGuildLoadMark(LPLARGEQUERY pData, LPDBMESSAGE pMessage)
{
	for(DWORD i=0; i<pMessage->dwResult; ++i)
	{
		GUILDMGR->LoadMark(atoi((char*)pData->Data[0]), (char*)pData->Data[1]);
	}
	if(pMessage->dwResult)
	{
		GuildLoadMark(atoi((char*)pData->Data[0]));
	}
	else
	{
	// server start!
	//	g_pServerSystem->SetStart(TRUE);
	
		//SW060526 巩颇芒绊 酒捞袍 沥焊历厘 矫痢 函版
		//GuildItemLoad(GAMERESRCMNGR->GetLoadMapNum(), 0);	//扁粮 橇肺矫廉绰 敌促. //GuildItemOption(.. //GuildItemRareOption(..
		LoadGuildWarRecord( 0 );
	}
}

void RGuildCreate(LPQUERY pData, LPDBMESSAGE pMessage)
{
	CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser(pMessage->dwID);
	
	if((int)atoi((char*)pData->Data[eGu_CIsSuccess]) != 0 ) // 0:success
	{
		if(pPlayer == 0)
			return;
		GUILDMGR->SendNackMsg(pPlayer, MP_GUILD_CREATE_NACK, eGuildErr_Create_Name);
		return;
	}

	GUILDINFO Info;
	Info.GuildIdx = atoi((char*)pData->Data[eGu_CGuildIdx]);
	Info.MapNum = atoi((char*)pData->Data[eGu_CMapNum]);
	Info.MasterIdx = pMessage->dwID;
	Info.GuildLevel = GUILD_1LEVEL;
	Info.UnionIdx = 0;
	Info.MarkName = 0;
	SafeStrCpy(Info.GuildName, (char*)pData->Data[eGu_CName], MAX_GUILD_NAME+1);
	SafeStrCpy(Info.MasterName, (char*)pData->Data[eGu_CMasterName], MAX_NAME_LENGTH+1);
	memset(Info.GuildNotice, 0, MAX_GUILD_NOTICE+1);

	//SW070103 巩颇器牢飘俺急
	Info.LvUpCounter = 0;
	Info.bNeedMasterChecking = FALSE;

	GUILDMGR->CreateGuildResult(atoi((char*)pData->Data[eGu_CMasterlvl]), &Info);
}

void RGuildMarkRegist(LPLARGEQUERY pData, LPDBMESSAGE pMessage)
{
	CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser(pMessage->dwID);
	if(pPlayer == 0) return;
	//markidx, guildidx, imgdate
	
	GUILDMGR->RegistMarkResult(pPlayer, atoi((char*)pData->Data[1]), atoi((char*)pData->Data[0]), (char*)pData->Data[2]);
}

/*
void UpdateCharacterMoney( DWORD dwCharIdx, MONEYTYPE money, BYTE flag )
{
	sprintf(txt, "EXEC RP_UpdateCharacterMoney %d, %d, %d", dwCharIdx, money, flag );
	g_DB.Query(eQueryType_FreeQuery, eUpdateCharacterMoney, 0, txt);
}

void RUpdateCharacterMoney( LPQUERY pData, LPDBMESSAGE pMessage )
{
	DWORD CharacterIdx = atoi((char*)pData[0].Data[0]);
	CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser(CharacterIdx);
	if( pPlayer == NULL ) return;
	pPlayer->RSetMoney( (MONEYTYPE)atoi((char*)pData[0].Data[1]), (BYTE)atoi((char*)pData[0].Data[2]) );
}
*/
void ConfirmUserOut( DWORD dwConnectIdx, DWORD dwUserIdx, DWORD dwCharIdx, DWORD dwKind )
{
	sprintf(txt, "EXEC RP_ConfirmUserOut %d, %d, %d", dwUserIdx, dwCharIdx, dwKind );
	g_DB.Query(eQueryType_FreeQuery, eConfirmUserOut, dwConnectIdx, txt);
}

void RConfirmUserOut( LPQUERY pData, LPDBMESSAGE pMessage )
{
	DWORD dwConnectIdx = pMessage->dwID;
	DWORD dwUserIdx = (DWORD)atoi((char*)pData[0].Data[0]);
	DWORD dwCharIdx = (DWORD)atoi((char*)pData[0].Data[1]);
	DWORD dwKind = (DWORD)atoi((char*)pData[0].Data[2]);

	switch( dwKind )
	{
	case 1:		// 某腐急琶芒
		{
			MSGBASE msg;
			msg.Category = MP_USERCONN;
			msg.Protocol = MP_USERCONN_BACKTOCHARSEL_ACK;
			msg.dwObjectID = dwCharIdx;

			g_Network.Send2Server(dwConnectIdx, (char*)&msg, sizeof(msg));
		}
		break;
	case 2:
		{
			MSGBASE msg;
			msg.Category = MP_USERCONN;
			msg.Protocol = MP_USERCONN_LOGINCHECK_DELETE;
			msg.dwObjectID = dwUserIdx;

			g_Network.Send2Server(dwConnectIdx, (char*)&msg, sizeof(msg));
		}
		break;
	}	
}

void RSaveCharInfoBeforeLogout( LPQUERY pData, LPDBMESSAGE pMessage )
{
	// 甘酒眶贸府
	CPlayer * pPlayer = (CPlayer*)g_pUserTable->FindUser(pMessage->dwID);
	if(pPlayer == 0)
		return;
	g_pServerSystem->RemovePlayer(pPlayer->GetID());
	//
	ConfirmUserOut( (DWORD)atoi((char*)pData->Data[0]), pPlayer->GetUserID(), pPlayer->GetID(), 1 );
}

void RGuildBreakUp( LPQUERY pData, LPDBMESSAGE pMessage )
{
	CPlayer* pMaster = (CPlayer*)g_pUserTable->FindUser(pMessage->dwID);
	GUILDMGR->BreakUpGuildResult(pMaster, atoi((char*)pData->Data[0]), (char*)pData->Data[1]);

	DeleteGuildWarRecord( (DWORD)atoi((char*)pData->Data[0]) );
}

void RGuildDeleteMember( LPQUERY pData, LPDBMESSAGE pMessage )
{
	if(atoi((char*)pData->Data[0]) == 0) //err
	{
		CPlayer * pMaster = (CPlayer*)g_pUserTable->FindUser( atoi((char*)pData->Data[1]) );
		if(pMaster)
			GUILDMGR->SendNackMsg(pMaster, MP_GUILD_DELETEMEMBER_NACK, eGuildErr_Err);
	}
	else
		GUILDMGR->DeleteMemberResult(pMessage->dwID, atoi((char*)pData->Data[0]), (char*)pData->Data[1]);
}

void RGuildSecedeMember( LPQUERY pData, LPDBMESSAGE pMessage )
{
	if(atoi((char*)pData->Data[0]) == 0) //err
	{
		CPlayer * pPlayer = (CPlayer*)g_pUserTable->FindUser(atoi((char*)pData->Data[1]));
		if(pPlayer)
			GUILDMGR->SendNackMsg(pPlayer, MP_GUILD_SECEDE_NACK, eGuildErr_Err);
	}
	else
		GUILDMGR->SecedeResult(pMessage->dwID, atoi((char*)pData->Data[0]), (char*)pData->Data[1]);
}

void RGuildItemLoad( LPQUERY pData, LPDBMESSAGE pMessage )
{
	ITEMBASE guilditem;
	DWORD GuildIdx;

	for(DWORD i=0; i<pMessage->dwResult; ++i)
	{
		GuildIdx = atoi((char*)pData[i].Data[eMu_IMunpaID]);
		
		guilditem.Durability = atoi((char*)pData[i].Data[eMu_IDurability]);
		guilditem.wIconIdx = atoi((char*)pData[i].Data[eMu_IIDX]);
		guilditem.dwDBIdx = atoi((char*)pData[i].Data[eMu_IDBIDX]);
		guilditem.Position = atoi((char*)pData[i].Data[eMu_IPosition]);
		guilditem.RareIdx = atoi((char*)pData[i].Data[eMu_IRareIdx]);
		guilditem.QuickPosition = 0;
		guilditem.ItemParam = 0;	// magi82(28)
	    guilditem.StoneIdx=atoi((char*)pData[i].Data[eMu_IStoneIdx]); //weiye 门派仓库，，成长数据附加! 2015-10-25
		guilditem.ItemStatic=atoi((char*)pData[i].Data[eMu_IStatic]);
		guilditem.ItemGrow=atoi((char*)pData[i].Data[eMu_IGrow]);
		GUILDMGR->RegistGuildItem(GuildIdx, &guilditem);
	}
	if(pMessage->dwResult < MAX_QUERY_RESULT)
	{
		GuildItemOption(GAMERESRCMNGR->GetLoadMapNum(), 0);
		//SW050920 Rare
		GuildItemRareOption(GAMERESRCMNGR->GetLoadMapNum(), 0);
	}	
	else
		GuildItemLoad(GAMERESRCMNGR->GetLoadMapNum(), guilditem.dwDBIdx);
}

void RGuildItemOption(LPQUERY pData, LPDBMESSAGE pMessage)
{
	ITEM_OPTION_INFO OptionInfo;
	for(DWORD i=0; i<pMessage->dwResult; ++i)
	{
		OptionInfo.dwOptionIdx				= (DWORD)atoi((char*)pData[i].Data[eMOI_OptionID]);
		OptionInfo.dwItemDBIdx				= (DWORD)atoi((char*)pData[i].Data[eMOI_ItemDBID]);
		OptionInfo.GenGol					= (WORD)atoi((char*)pData[i].Data[eMOI_GenGol]);
		OptionInfo.MinChub					= (WORD)atoi((char*)pData[i].Data[eMOI_MinChub]);				
		OptionInfo.CheRyuk					= (WORD)atoi((char*)pData[i].Data[eMOI_CheRyuk]);				
		OptionInfo.SimMek					= (WORD)atoi((char*)pData[i].Data[eMOI_SimMek]);					
		OptionInfo.Life						= (DWORD)atoi((char*)pData[i].Data[eMOI_Life]);					
		OptionInfo.NaeRyuk					= (WORD)atoi((char*)pData[i].Data[eMOI_NaeRyuk]);				
		OptionInfo.Shield					= (DWORD)atoi((char*)pData[i].Data[eMOI_Shield]);					
		OptionInfo.AttrRegist.SetElement_Val(ATTR_FIRE,		(float)atoi((char*)pData[i].Data[eMOI_FireAttrRegist])/100.f );
		OptionInfo.AttrRegist.SetElement_Val(ATTR_WATER,	(float)atoi((char*)pData[i].Data[eMOI_WaterAttrRegist])/100.f );
		OptionInfo.AttrRegist.SetElement_Val(ATTR_TREE,		(float)atoi((char*)pData[i].Data[eMOI_TreeAttrRegist])/100.f );
		OptionInfo.AttrRegist.SetElement_Val(ATTR_IRON,		(float)atoi((char*)pData[i].Data[eMOI_GoldAttrRegist])/100.f );
		OptionInfo.AttrRegist.SetElement_Val(ATTR_EARTH,	(float)atoi((char*)pData[i].Data[eMOI_EarthAttrRegist])/100.f );
		OptionInfo.PhyAttack				= (WORD)atoi((char*)pData[i].Data[eMOI_PhyAttack]);
		OptionInfo.CriticalPercent			= (WORD)atoi((char*)pData[i].Data[eMOI_Critical]);
		OptionInfo.AttrAttack.SetElement_Val(ATTR_FIRE,		(float)atoi((char*)pData[i].Data[eMOI_FireAttrAttack])/100.f );
		OptionInfo.AttrAttack.SetElement_Val(ATTR_WATER,	(float)atoi((char*)pData[i].Data[eMOI_WaterAttrAttack])/100.f );
		OptionInfo.AttrAttack.SetElement_Val(ATTR_TREE,		(float)atoi((char*)pData[i].Data[eMOI_TreeAttrAttack])/100.f );
		OptionInfo.AttrAttack.SetElement_Val(ATTR_IRON,		(float)atoi((char*)pData[i].Data[eMOI_GoldAttrAttack])/100.f );
		OptionInfo.AttrAttack.SetElement_Val(ATTR_EARTH,	(float)atoi((char*)pData[i].Data[eMOI_EarthAttrAttack])/100.f );
		OptionInfo.PhyDefense				= (WORD)atoi((char*)pData[i].Data[eMOI_PhyDefense]);				
		
		GUILDMGR->AddGuildItemOption(&OptionInfo);
	}

	if(pMessage->dwResult >= MAX_QUERY_RESULT)
	{
		GuildItemOption(GAMERESRCMNGR->GetLoadMapNum(), OptionInfo.dwOptionIdx);
	}
	//SW051007 RGuildItemRareOption 栏肺 颗辫.
//	else
//	{
////		g_pServerSystem->SetStart(TRUE);

//		
//		LoadGuildWarRecord( 0 );
//	}
}

//SW050920 Rare
void RGuildItemRareOption(LPQUERY pData, LPDBMESSAGE pMessage)
{
	ITEM_RARE_OPTION_INFO RareOptionInfo;
	for( DWORD i = 0; i < pMessage->dwResult; ++i )
	{
		RareOptionInfo.dwRareOptionIdx				= (DWORD)atoi((char*)pData[i].Data[eGORI_RareOptionID]);
		RareOptionInfo.dwItemDBIdx				= (DWORD)atoi((char*)pData[i].Data[eGORI_ItemDBID]);
		RareOptionInfo.GenGol					= (WORD)atoi((char*)pData[i].Data[eGORI_GenGol]);
		RareOptionInfo.MinChub					= (WORD)atoi((char*)pData[i].Data[eGORI_MinChub]);				
		RareOptionInfo.CheRyuk					= (WORD)atoi((char*)pData[i].Data[eGORI_CheRyuk]);				
		RareOptionInfo.SimMek					= (WORD)atoi((char*)pData[i].Data[eGORI_SimMek]);					
		RareOptionInfo.Life						= (DWORD)atoi((char*)pData[i].Data[eGORI_Life]);					
		RareOptionInfo.NaeRyuk					= (WORD)atoi((char*)pData[i].Data[eGORI_NaeRyuk]);				
		RareOptionInfo.Shield					= (DWORD)atoi((char*)pData[i].Data[eGORI_Shield]);					
		RareOptionInfo.AttrRegist.SetElement_Val(ATTR_FIRE,		(float)atoi((char*)pData[i].Data[eGORI_FireAttrRegist])/100.f );
		RareOptionInfo.AttrRegist.SetElement_Val(ATTR_WATER,	(float)atoi((char*)pData[i].Data[eGORI_WaterAttrRegist])/100.f );
		RareOptionInfo.AttrRegist.SetElement_Val(ATTR_TREE,		(float)atoi((char*)pData[i].Data[eGORI_TreeAttrRegist])/100.f );
		RareOptionInfo.AttrRegist.SetElement_Val(ATTR_IRON,		(float)atoi((char*)pData[i].Data[eGORI_GoldAttrRegist])/100.f );
		RareOptionInfo.AttrRegist.SetElement_Val(ATTR_EARTH,	(float)atoi((char*)pData[i].Data[eGORI_EarthAttrRegist])/100.f );
		RareOptionInfo.PhyAttack				= (WORD)atoi((char*)pData[i].Data[eGORI_PhyAttack]);
//		RareOptionInfo.CriticalPercent			= (WORD)atoi((char*)pData[i].Data[eGORI_Critical]);
		RareOptionInfo.AttrAttack.SetElement_Val(ATTR_FIRE,		(float)atoi((char*)pData[i].Data[eGORI_FireAttrAttack])/100.f );
		RareOptionInfo.AttrAttack.SetElement_Val(ATTR_WATER,	(float)atoi((char*)pData[i].Data[eGORI_WaterAttrAttack])/100.f );
		RareOptionInfo.AttrAttack.SetElement_Val(ATTR_TREE,		(float)atoi((char*)pData[i].Data[eGORI_TreeAttrAttack])/100.f );
		RareOptionInfo.AttrAttack.SetElement_Val(ATTR_IRON,		(float)atoi((char*)pData[i].Data[eGORI_GoldAttrAttack])/100.f );
		RareOptionInfo.AttrAttack.SetElement_Val(ATTR_EARTH,	(float)atoi((char*)pData[i].Data[eGORI_EarthAttrAttack])/100.f );
		RareOptionInfo.PhyDefense				= (WORD)atoi((char*)pData[i].Data[eGORI_PhyDefense]);
		
		GUILDMGR->AddGuildItemRareOption(&RareOptionInfo);
	}

	if(pMessage->dwResult >= MAX_QUERY_RESULT)
	{
		GuildItemRareOption(GAMERESRCMNGR->GetLoadMapNum(), RareOptionInfo.dwRareOptionIdx);
	}
	else
	{
		LoadGuildWarRecord( 0 );
	}
}

void LoadGuildWarRecord( DWORD dwIdx )
{
	sprintf(txt, "EXEC dbo.MP_GUILDWARRECORD_Load %d", dwIdx );
	g_DB.Query(eQueryType_FreeQuery, eLoadGuildWarRecord, 0, txt);
}

void RLoadGuildWarRecord( LPQUERY pData, LPDBMESSAGE pMessage )
{
	int nStartIdx = 0;
	for( DWORD i = 0; i < pMessage->dwResult; ++i )
	{
		nStartIdx = atoi((char*)pData[i].Data[0]);
		DWORD dwGuildIdx = (DWORD)atoi((char*)pData[i].Data[1]);
		DWORD dwGFWVictory = (DWORD)atoi((char*)pData[i].Data[2]);
		DWORD dwGFWDraw = (DWORD)atoi((char*)pData[i].Data[3]);
		DWORD dwGFWLoose = (DWORD)atoi((char*)pData[i].Data[4]);

		GUILDFIELDWARMGR->RegistGuildFieldWarFromRecord( dwGuildIdx, dwGFWVictory, dwGFWDraw, dwGFWLoose );
	}

	if( pMessage->dwResult >= MAX_QUERY_RESULT )
	{
		LoadGuildWarRecord( nStartIdx );
	}	
	else
	{
		LoadGuildFieldWar( 0 );
	}
}

void DeleteGuildWarRecord( DWORD dwGuildIdx )
{
	sprintf(txt, "EXEC dbo.MP_GUILDWARRECORD_Delete %d", dwGuildIdx );
	g_DB.Query(eQueryType_FreeQuery, eDeleteGuildWarRecord, 0, txt);
}

void LoadGuildFieldWar( DWORD dwIdx )
{
	sprintf(txt, "EXEC dbo.MP_GUILDFIELDWAR_Load %d", dwIdx );
	g_DB.Query(eQueryType_FreeQuery, eLoadGuildFieldWar, 0, txt);
}

void RLoadGuildFieldWar( LPQUERY pData, LPDBMESSAGE pMessage )
{
	int nStartIdx = 0;
	for( DWORD i = 0; i < pMessage->dwResult; ++i )
	{
		nStartIdx = atoi((char*)pData[i].Data[0]);
		DWORD dwGuildIdx1 = (DWORD)atoi((char*)pData[i].Data[1]);
		DWORD dwGuildIdx2 = (DWORD)atoi((char*)pData[i].Data[2]);
		DWORD dwMoney = (DWORD)atoi((char*)pData[i].Data[3]);

		GUILDFIELDWARMGR->RegistGuildFieldWar( dwGuildIdx1, dwGuildIdx2, dwMoney );
	}

	if( pMessage->dwResult >= MAX_QUERY_RESULT )
	{
		LoadGuildFieldWar( nStartIdx );
	}	
	else
	{
//		g_pServerSystem->SetStart( TRUE );
		GuildUnionLoad( 0 );
	}
}

void InsertGuildFieldWar( DWORD dwGuildIdx1, DWORD dwGuildIdx2, DWORD dwMoney )
{
	sprintf(txt, "EXEC dbo.MP_GUILDFIELDWAR_Insert %d, %d, %u", dwGuildIdx1, dwGuildIdx2, dwMoney );
	g_DB.Query(eQueryType_FreeQuery, eInsertGuildFieldWar, 0, txt);
}

void DeleteGuildFieldWar( DWORD dwGuildIdx1, DWORD dwGuildIdx2 )
{
	sprintf(txt, "EXEC dbo.MP_GUILDFIELDWAR_Delete %d, %d", dwGuildIdx1, dwGuildIdx2 );
	g_DB.Query(eQueryType_FreeQuery, eDeleteGuildFieldWar, 0, txt);
}

void UpdateGuildFieldWarRecordToDB( DWORD dwGuildIdx, DWORD dwVitory, DWORD dwDraw, DWORD dwLoose )
{
	sprintf(txt, "EXEC dbo.MP_GUILDFIELDWAR_Record %d, %d, %d, %d", dwGuildIdx, dwVitory, dwDraw, dwLoose );
	g_DB.Query(eQueryType_FreeQuery, eUpdateGuildFieldWarRecord, 0, txt);
}


void CharacterChangeName( DWORD CharacterIdx, char* Name, DWORD DBIdx )
{
    // 2014-06-25 check string param !

	if( !isValidQueryString(Name))
	{
	   return;
	}

	if( CheckString(Name))
	{
		return;
	} 

	sprintf(txt, "EXEC %s %d, \'%s\', %d", STORED_SHOPITEM_CHANGENAME, CharacterIdx, Name, DBIdx );
	g_DB.Query(eQueryType_FreeQuery, eItemShopChangeName, CharacterIdx, txt);
}


void RCharacterChangeName( LPQUERY pData, LPDBMESSAGE pMessage )
{
	CPlayer * pPlayer = (CPlayer*)g_pUserTable->FindUser( pMessage->dwID );
	if(!pPlayer)		return;

	DWORD Result = (DWORD)atoi((char*)pData->Data[0]);
	DWORD DBIdx = 0;


	MSG_DWORD msg;
	msg.Category = MP_ITEM;

	switch( Result )
	{
	case 0:		// 己傍
		{
			DBIdx = (DWORD)atoi((char*)pData->Data[1]);
			ITEMBASE* pItemBase = NULL;

			// Item Position 犬牢
			int i=0; 
			for(i=0; i<(SLOT_SHOPINVEN_NUM+TABCELL_SHOPINVEN_PLUS_NUM); i++)//for(i=0; i<SLOT_SHOPINVEN_NUM/*/2*/; i++)
			{
				pItemBase = (ITEMBASE*)ITEMMGR->GetItemInfoAbsIn( pPlayer, i+TP_SHOPINVEN_START );
				if( !pItemBase )	continue;
				
				if( pItemBase->dwDBIdx == DBIdx )
				{
					if( EI_TRUE != ITEMMGR->DiscardItem( pPlayer, i+TP_SHOPINVEN_START, pItemBase->wIconIdx, 1 ) )
					{
					//	char buf[64];
					//	sprintf(buf, "ChangeName OK, DiscardItem Fail : %d", pMessage->dwID);
					//	ASSERTMSG(0, buf);
					}

					break;
				}
			}
			
			if( i >= (SLOT_SHOPINVEN_NUM+TABCELL_SHOPINVEN_PLUS_NUM) )//if( i >= SLOT_SHOPINVEN_NUM/*/2*/ )
			{
			//	char buf[64];
			//	sprintf(buf, "ChangeName OK, DeleteItem Fail: %d", pMessage->dwID);
			//	ASSERTMSG(0, buf);
			}

			QSTATETYPE time = GetCurTime();
			UsingShopItemUpdateToDB( 0, pPlayer->GetID(), pItemBase->wIconIdx, pItemBase->dwDBIdx, 0, time, 0 );
			pPlayer->GetShopItemManager()->DeleteUsingShopItem( pItemBase->wIconIdx );
			
			SHOPITEMBASE ShopItemBase;
			ShopItemBase.BeginTime.value = time;
			ShopItemBase.Param = 0;
			memcpy( &ShopItemBase.ItemBase, pItemBase, sizeof(ITEMBASE) );
			pPlayer->GetShopItemManager()->AddShopItemUse( &ShopItemBase );

			msg.Protocol = MP_ITEM_SHOPITEM_NCHANGE_ACK;
			msg.dwData = Result;
			msg.dwObjectID = pPlayer->GetID();
			SafeStrCpy(msg.AesText2, pPlayer->GetObjectName(),17);

			// magi82 - 巩窍积包访(070123) /////////////////////////////////////////////

			DWORD dwGuildIdx = 0;
			BOOL bFlag = FALSE;
			PTRLISTPOS pos = GUILDMGR->GetGuildTraineeList()->GetHeadPosition();
			GUILD_TRAINEE_LIST* pTrainee;
			while(pos)
			{
				pTrainee = (GUILD_TRAINEE_LIST*)GUILDMGR->GetGuildTraineeList()->GetNext(pos);
				if( pTrainee )
				{
					if( pTrainee->dwUserIdx == pMessage->dwID)
					{
						bFlag = TRUE;
						GuildTraineeDelete(pTrainee->dwUserIdx);
						dwGuildIdx = pTrainee->dwGuildIdx;
						GUILDMGR->GetGuildTraineeList()->Remove(pTrainee);
						break;
					}
				}
			}

			if(!bFlag)
				break;

			// Brodcasting to Mapserver
			MSG_DWORD msg;
			SetProtocol( &msg, MP_GUILD, MP_GUILD_MUNHA_DELETE_BROD );
			msg.dwData = pMessage->dwID;
			PACKEDDATA_OBJ->SendToBroadCastMapServer( &msg, sizeof(msg) );

			// 巩林茄抛 焊郴霖促
			CGuild* pGuild = (CGuild*)GUILDMGR->GetGuild( dwGuildIdx );
			if( !pGuild )
				return;

			MSG_NAME_DWORD Sendmsg;
			SetProtocol( &Sendmsg, MP_GUILD, MP_GUILD_MUNHA_NAMECHANGE_SYN );
			Sendmsg.dwObjectID = pGuild->GetGuildInfo()->MasterIdx;
			Sendmsg.dwData = pGuild->GetGuildInfo()->GuildIdx;
			SafeStrCpy( Sendmsg.Name, pPlayer->GetObjectName(), MAX_NAME_LENGTH+1 );
			g_Network.Send2AgentServer((char*)&Sendmsg, sizeof(Sendmsg));

			// 何巩林唱 厘肺茄抛档 焊晨
			pos = pGuild->GetMemberList()->GetHeadPosition();
			while(pos)
			{
				GUILDMEMBERINFO* pInfo = (GUILDMEMBERINFO*)pGuild->GetMemberList()->GetNext(pos);

				if( pInfo )
				{
					if(pInfo->Rank == GUILD_SENIOR || pInfo->Rank == GUILD_VICEMASTER)
					{
						MSG_NAME_DWORD Sendmsg;
						SetProtocol( &Sendmsg, MP_GUILD, MP_GUILD_MUNHA_NAMECHANGE_SYN );
						Sendmsg.dwObjectID = pInfo->MemberIdx;
						Sendmsg.dwData = pGuild->GetGuildInfo()->GuildIdx;
						SafeStrCpy( Sendmsg.Name, pPlayer->GetObjectName(), MAX_NAME_LENGTH+1 );
						g_Network.Send2AgentServer((char*)&Sendmsg, sizeof(Sendmsg));
					}
				}
			}

			//////////////////////////////////////////////////////////////////////////
		}
		break;
	default:
		msg.Protocol = MP_ITEM_SHOPITEM_NCHANGE_NACK;
		msg.dwData = Result;
		msg.dwObjectID = 0;
		break;
	}

	pPlayer->SendMsg( &msg, sizeof(msg) );	
	//PACKEDDATA_OBJ->QuickSend( pPlayer, &msg, sizeof(msg) );
	//PACKEDDATA_OBJ->QuickSendExceptObjectSelf( pPlayer, &msg, sizeof(msg) );
}

void CharacterChangeInfoToDB( DWORD CharacterIdx, DWORD Gender, DWORD HairType, DWORD FaceType, float Height, float Width )
{
	sprintf(txt, "EXEC %s %d, %d, %d, %d, %f, %f", STORED_SHOPITEM_CHARCHANGE, CharacterIdx, Gender, HairType, FaceType, Height, Width );
	g_DB.Query(eQueryType_FreeQuery, eItemCharChange, 0, txt);
}


// Guild Tournament
void GuildTournamentInfoLoad()
{
	sprintf(txt, "EXEC %s", STORED_GT_LOADALL );
	g_DB.Query(eQueryType_FreeQuery, eGuildTournament_LoadAll, 0, txt);
}

void GuildTournamentCancelIdxLoad( DWORD TournamentIdx )
{
	sprintf(txt, "EXEC %s %d", STORED_GT_CANCELLOAD, TournamentIdx );
	g_DB.Query(eQueryType_FreeQuery, eGuildTournament_CancelLoad, 0, txt);
}

void GuildTournamentCancelIdxInsert( DWORD TournamentIdx, DWORD GuildIdx )
{
	sprintf(txt, "EXEC %s %d, %d", STORED_GT_CANCELINSERT, TournamentIdx, GuildIdx );
	g_DB.Query(eQueryType_FreeQuery, eGuildTournament_CancelInsert, 0, txt);
}

void GuildTournamentGuildInfoInsert( DWORD GuildIdx, char* GuildName, DWORD Position  )
{
    // 2014-06-25 check string param !

	if( !isValidQueryString(GuildName))
	{
	   return;
	}

	if( CheckString(GuildName))
	{
		return;
	} 

	sprintf(txt, "EXEC %s %d, \'%s\', %d", STORED_GT_GUILDINFO_INSERT, GuildIdx, GuildName, Position );
	g_DB.Query(eQueryType_FreeQuery, eGuildTournament_Insert, 0, txt);
}

void GuildTournamentGuildInfoDelete( DWORD GuildIdx )
{
	sprintf(txt, "EXEC %s %d", STORED_GT_GUILDINFO_DELETE, GuildIdx );
	g_DB.Query(eQueryType_FreeQuery, eGuildTournament_Delete, 0, txt);
}

void GuildTournamentGuildInfoUpdate( DWORD GuildIdx, DWORD Ranking, DWORD ProcessTournament )
{
	sprintf(txt, "EXEC %s %d, %d, %d", STORED_GT_GUILDINFO_UPDATE, GuildIdx, Ranking, ProcessTournament );
	g_DB.Query(eQueryType_FreeQuery, eGuildTournament_UpdateGuildInfo, 0, txt);
}

void GuildTournamentInfoUpdate( DWORD TournamentIdx, DWORD TournamentFight, DWORD WaitTime )
{
	sprintf(txt, "EXEC %s %d, %d, %d", STORED_GT_TOURNAMENTINFO_UPDATE, TournamentIdx, TournamentFight, WaitTime );
	g_DB.Query(eQueryType_FreeQuery, eGuildTournament_UpdateTournamentInfo, 0, txt);
}

void RGuildTournamentInfoLoad( LPQUERY pData, LPDBMESSAGE pMessage )
{
	DWORD TournamentIdx = 0;
	DWORD CurTournament = 0;
	DWORD WaitTime = 0;
	GTDBLoadInfo GuildInfo[MAXGUILD_INTOURNAMENT];
	memset( GuildInfo, 0, sizeof(GTDBLoadInfo)*MAXGUILD_INTOURNAMENT );

	int count = 0;
	for( DWORD i=0; i<pMessage->dwResult; ++i )
	{
		// magi82 巩颇配呈刚飘 -> 32评 捞惑狼 单捞磐啊 府畔登绰 版快甫 困茄 抗寇贸府(烙矫)
		// DB俊辑 器瘤记捞 般媚瘤绰 巩颇甸阑 贸府秦具茄促.
		if( count >= MAXGUILD_INTOURNAMENT )
		{
			if(IDYES == MessageBox(NULL, "Error - RGuildTournamentInfoLoad( LPQUERY pData, LPDBMESSAGE pMessage )", NULL, MB_YESNO|MB_TOPMOST))
				__asm int 3;
			return;
		}

		GuildInfo[count].GuildIdx = (DWORD)atoi((char*)pData[i].Data[eGT_GuildIdx]);
		if( GuildInfo[count].GuildIdx == 0 )
		{
			TournamentIdx = (DWORD)atoi((char*)pData[i].Data[eGT_Position]);
			CurTournament = (BYTE)atoi((char*)pData[i].Data[eGT_Process]);
			WaitTime = (DWORD)atoi((char*)pData[i].Data[eGT_WaitTime]);
			continue;
		}
		GuildInfo[count].Position = (BYTE)atoi((char*)pData[i].Data[eGT_Position]);
		GuildInfo[count].Ranking = (BYTE)atoi((char*)pData[i].Data[eGT_Ranking]);
		GuildInfo[count].ProcessTournament= (BYTE)atoi((char*)pData[i].Data[eGT_Process]);
		++count;
	}

	if( TournamentIdx == 0 )			return;

	GTMGR->SetTournamentInfo( TournamentIdx, CurTournament, WaitTime );
	GTMGR->SetGuildInfo( GuildInfo, pMessage->dwResult );
}

void RGuildTournamentCancelIdxLoad( LPQUERY pData, LPDBMESSAGE pMessage )
{
	for( DWORD i=0; i<pMessage->dwResult; ++i )
	{
		DWORD GuildIdx = (DWORD)atoi((char*)pData[i].Data[eGT_GuildIdx]);
		GTMGR->AddCancelGuild( GuildIdx );
	}
}


void GuildTournamentAddLog( DWORD TournamentIdx, DWORD GuildIdx, DWORD Logkind, DWORD Ranking )
{
	sprintf( txt, "EXEC dbo.Up_GT_AddLog %d, %d, %d, %d", TournamentIdx, GuildIdx, Logkind, Ranking );
	g_DB.LogQuery( eQueryType_FreeQuery, eGuildTournamentAddLog, 0, txt );
}

// jackpot
/*
void JackpotLoadTotalMoney()
{
	sprintf( txt, "EXEC dbo.MP_JACKPOT_LoadMoney" );
	g_DB.Query( eQueryType_FreeQuery, eJackpot_TotalMoney_Load, 0, txt );
}

void RJackpotLoadTotalMoney( LPQUERY pData, LPDBMESSAGE pMessage )
{
	if( pMessage->dwResult == 0 ) return;

	DWORD TotalMoney = (DWORD)atoi((char*)pData->Data[0]);
	JACKPOTMGR->SetTotalMoney( TotalMoney );
}*/

void JackpotAddTotalMoney( MONEYTYPE MapMoney )
{
	sprintf( txt, "EXEC dbo.MP_JACKPOT_AddTotalMoney %u", MapMoney );
	g_DB.Query( eQueryType_FreeQuery, eJackpot_TotalMoney_Update, 0, txt );
}
/*
void RJackpotAddTotalMoney( LPQUERY pData, LPDBMESSAGE pMessage )
{
	if( pMessage->dwResult == 0 ) return;

	DWORD TotalMoney = (DWORD)atoi((char*)pData->Data[0]);
	JACKPOTMGR->SendMsgMapUserTotalMoney( TotalMoney );
}*/

void JackpotPrizeInfo( MONEYTYPE MapMoney, DWORD PlayerID, DWORD PrizeKind, DWORD PrizePercentage, DWORD dwAbsMoney)
{
	sprintf( txt, "EXEC dbo.MP_JACKPOT_Prize %u, %d, %d, %d, %u", MapMoney, PlayerID, PrizeKind, PrizePercentage, dwAbsMoney );
	g_DB.Query( eQueryType_FreeQuery, eJackpot_PrizeInfo_Update, PlayerID, txt );
}

void RJackpotPrizeInfo( LPQUERY pData, LPDBMESSAGE pMessage )
{
	if( pMessage->dwResult == 0 ) return;
	
	DWORD TotalMoney = (DWORD)atoi((char*)pData->Data[0]);
	DWORD PrizeMoney = (DWORD)atoi((char*)pData->Data[1]);	
	DWORD PrizeKind = (DWORD)atoi((char*)pData->Data[2]);
//	CPlayer * pPlayer = (CPlayer*)g_pUserTable->FindUser( pMessage->dwID );
//	if(!pPlayer)		return;
	DWORD PlayerID = pMessage->dwID;

	JACKPOTMGR->SetPrizeInfo( TotalMoney, PrizeMoney, PlayerID, PrizeKind ); //滴啊瘤 老阑茄促. 秦寸 ID肺 PrizeMoney殿废苞 傈辑滚 傍瘤
}

// guildunion
void GuildUnionLoad( DWORD dwStartGuildUnionIdx )
{
	sprintf( txt, "EXEC dbo.MP_GUILD_UNION_Load %d", dwStartGuildUnionIdx );
	g_DB.Query( eQueryType_FreeQuery, eGuildUnionLoad, 0, txt );
}

void RGuildUnionLoad( LPQUERY pData, LPDBMESSAGE pMessage )
{
	DWORD dwStartGuildIdx = 0;
	for( DWORD i = 0; i < pMessage->dwResult; ++i )
	{
		dwStartGuildIdx = (DWORD)atoi((char*)pData[i].Data[0]);

		//SW070112 函版
/*
		GUILDUNIONMGR->LoadGuildUnion( (DWORD)atoi((char*)pData[i].Data[0])		// guildidx
			, (char*)pData[i].Data[1]					// name
			, (DWORD)atoi((char*)pData[i].Data[2])		// 0
				, (DWORD)atoi((char*)pData[i].Data[3])		// 1
				, (DWORD)atoi((char*)pData[i].Data[4])		// 2
				, (DWORD)atoi((char*)pData[i].Data[5])		// 3
				, (DWORD)atoi((char*)pData[i].Data[6])		// 4
				, (DWORD)atoi((char*)pData[i].Data[7])		// 5
				, (DWORD)atoi((char*)pData[i].Data[8])		// 6
				, (DWORD)atoi((char*)pData[i].Data[9]) );	// mark
			*/
		DWORD j = 0;
		DWORD UnionGuildIdx[MAX_GUILD_UNION_NUM] = {0,};
		while( j < MAX_GUILD_UNION_NUM )
		{
			UnionGuildIdx[j] = (DWORD)atoi((char*)pData[i].Data[j+2]);
			++j;
		}
		
		GUILDUNIONMGR->LoadGuildUnion( (DWORD)atoi((char*)pData[i].Data[0])
			, (char*)pData[i].Data[1]		//name
			, (DWORD)atoi((char*)pData[i].Data[9])
			, UnionGuildIdx
			);
	}
	if( pMessage->dwResult >= MAX_QUERY_RESULT )
	{
		GuildUnionLoad( dwStartGuildIdx );
	}	
	else
	{
		GuildUnionLoadMark( 0 );
	}	
}

void GuildUnionLoadMark( DWORD dwMarkIdx )
{
	sprintf( txt, "EXEC dbo.MP_GUILD_UNION_LoadMark %d", dwMarkIdx );
	g_DB.FreeLargeQuery( RGuildUnionLoadMark, 0, txt );
}

void RGuildUnionLoadMark( LPLARGEQUERY pData, LPDBMESSAGE pMessage )
{
	for( DWORD i = 0; i < pMessage->dwResult; ++i )
	{
		GUILDUNIONMGR->LoadGuildUnionMark( (DWORD)atoi((char*)pData->Data[0]), (DWORD)atoi((char*)pData->Data[1]), (char*)pData->Data[2] );
	}
	if( pMessage->dwResult )
		GuildUnionLoadMark( (DWORD)atoi((char*)pData->Data[0]) );
	else
	{
#ifdef _JAPAN_LOCAL_
		g_pServerSystem->SetStart( TRUE );
//		SiegeWarProfitInfoLoad();
#else
//		g_pServerSystem->SetStart( TRUE );
		SiegeWarProfitInfoLoad();
#endif
	}
}

void GuildUnionCreate( DWORD dwCharacterIdx, DWORD dwGuildIdx, char* sUnionName )
{
	// 2014-06-25 check string param !

	if( !isValidQueryString(sUnionName))
	{
	   return;
	}

	if( CheckString(sUnionName))
	{
		return;
	}
	sprintf( txt, "EXEC dbo.MP_GUILD_UNION_Create %d, %s", dwGuildIdx, sUnionName );
	g_DB.Query( eQueryType_FreeQuery, eGuildUnionCreate, dwCharacterIdx, txt );
}

void RGuildUnionCreate( LPQUERY pData, LPDBMESSAGE pMessage )
{
	if( !pMessage->dwResult )	return;
	if( atoi((char*)pData->Data[0]) == 0 )
	{
		CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser( pMessage->dwID );
		if( pPlayer )
			GUILDUNIONMGR->SendNackMsg( pPlayer, MP_GUILD_UNION_CREATE_NACK, eGU_Not_ValidName );
		return;
	}
	else
		GUILDUNIONMGR->GuildUnionCreateResult( (DWORD)atoi((char*)pData->Data[0]), (char*)pData->Data[1], (DWORD)atoi((char*)pData->Data[2]) );
}

void GuildUnionDestroy( DWORD dwGuildUnionIdx, DWORD dwMarkIdx, DWORD dwMasterIdx, DWORD dwTime )
{
	sprintf( txt, "EXEC dbo.MP_GUILD_UNION_Destroy %d, %d, %d, %d", dwGuildUnionIdx, dwMarkIdx, dwMasterIdx, dwTime );
	g_DB.Query( eQueryType_FreeQuery, eGuildUnionDestroy, 0, txt );
}

void GuildUnionAddGuild( DWORD dwGuildUnionIdx, int nIndex, DWORD dwGuildIdx )
{
	sprintf( txt, "EXEC dbo.MP_GUILD_UNION_AddGuild %d, %d, %d", dwGuildUnionIdx, nIndex, dwGuildIdx );
	g_DB.Query( eQueryType_FreeQuery, eGuildUnionAddGuild, 0, txt );
}

void GuildUnionRemoveGuild( DWORD dwGuildUnionIdx, int nIndex, DWORD dwGuildIdx )
{
	sprintf( txt, "EXEC dbo.MP_GUILD_UNION_RemoveGuild %d, %d, %d", dwGuildUnionIdx, nIndex, dwGuildIdx );
	g_DB.Query( eQueryType_FreeQuery, eGuildUnionRemoveGuild, 0, txt );
}

void GuildUnionSecedeGuild( DWORD dwGuildUnionIdx, int nIndex, DWORD dwGuildIdx, DWORD dwTime )
{
	sprintf( txt, "EXEC dbo.MP_GUILD_UNION_SecedeGuild %d, %d, %d, %d", dwGuildUnionIdx, nIndex, dwGuildIdx, dwTime );
	g_DB.Query( eQueryType_FreeQuery, eGuildUnionSecedeGuild, 0, txt );
}

void GuildUnionMarkRegist( DWORD dwCharacterIdx, DWORD dwMarkIdx, DWORD dwGuildUnionIdx, char* pImgData )
{
	char buf[1024];
	sprintf( buf, "EXEC dbo.MP_GUILD_UNION_MarkRegist %d, %d, %d, 0x", dwCharacterIdx, dwMarkIdx, dwGuildUnionIdx );

	int curpos = strlen(buf);
	for( int n = 0; n < GUILDUNIONMARK_BUFSIZE; ++n )
	{
		sprintf( &buf[curpos], "%02x", (BYTE)pImgData[n] );
		curpos += 2;
	}

	g_DB.FreeLargeQuery( RGuildUnionMarkRegist, 0, buf );
}

void RGuildUnionMarkRegist( LPLARGEQUERY pData, LPDBMESSAGE pMessage )
{
	if( !pMessage->dwResult )	return;
	GUILDUNIONMGR->GuildUnionMarkRegistResult( (DWORD)atoi((char*)pData->Data[0]), (DWORD)atoi((char*)pData->Data[1]),
		(DWORD)atoi((char*)pData->Data[2]), (char*)pData->Data[3] );
}

//ΞΜΜΜΜΜΜΜΜΜΜΜΜΜΜΜΜΜΜΜΜΜΜΜΜΜΜΜΜΜΜΜΟ
//Ν SiegeWar													  Ν
//ΡΜΜΜΜΜΜΜΜΜΜΜΜΜΜΜΜΜΜΜΜΜΜΜΜΜΜΜΜΜΜΜΠ
void SiegeWarInfoLoad( DWORD MapNum )
{
	sprintf( txt, "EXEC %s %d", STORED_SIEGE_INFOLOAD, MapNum );
	g_DB.Query( eQueryType_FreeQuery, eSiegeWarInfoLoad, 0, txt );
}

void RSiegeWarInfoLoad( LPQUERY pData, LPDBMESSAGE pMessage )
{
	DWORD SiegeWarIdx = 0;
	DWORD TaxRate = 0;
	DWORD RegistTime = 0;
	DWORD SiegeWarTime = 0;
	DWORD Level = 0;
	
	SiegeWarIdx = (DWORD)atoi((char*)pData->Data[eSWI_SiegeWarIdx]);
	RegistTime = (DWORD)atoi((char*)pData->Data[eSWI_RegistTime]);
	SiegeWarTime = (DWORD)atoi((char*)pData->Data[eSWI_SiegeWarTime]);
	Level = (DWORD)atoi((char*)pData->Data[eSWI_SiegeWarLevel]);

	if( SiegeWarIdx == 0 )			return;

	SIEGEWARMGR->SetSiegeWarInfo( SiegeWarIdx, TaxRate, RegistTime, SiegeWarTime, Level );
	SiegeWarGuildInfoLoad( 0, SiegeWarIdx, SIEGEWARMGR->GetSiegeMapNum() );
}

void SiegeWarInfoInsert( DWORD SiegeWarIdx, DWORD RegistTime, DWORD SiegeWarTime, DWORD Level, DWORD MapNum )
{
	sprintf( txt, "EXEC %s %d, %d, %d, %d, %d", STORED_SIEGE_INFOINSERT, SiegeWarIdx, RegistTime, SiegeWarTime, Level, MapNum );
	g_DB.Query( eQueryType_FreeQuery, eSiegeWarInfoInsert, 0, txt );
}

void SiegeWarInfoUpdate( DWORD SiegeWarIdx, DWORD RegistTime, DWORD SiegeWarTime, DWORD Level, DWORD MapNum )
{
	sprintf( txt, "EXEC %s %d, %d, %d, %d, %d", STORED_SIEGE_INFOUPDATE, SiegeWarIdx, RegistTime, SiegeWarTime, Level, MapNum );
	g_DB.Query( eQueryType_FreeQuery, eSiegeWarInfoUpdate, 0, txt );
}

void SiegeWarGuildInfoLoad( DWORD StartDBIdx, DWORD SiegeWarIdx, DWORD MapNum )
{
	sprintf( txt, "EXEC %s %d, %d, %d", STORED_SIEGE_GUILDLOAD, StartDBIdx, SiegeWarIdx, MapNum );
	g_DB.Query( eQueryType_FreeQuery, eSiegeWarGuildInfoLoad, 0, txt );
}

void RSiegeWarGuildInfoLoad( LPQUERY pData, LPDBMESSAGE pMessage )
{
	DWORD LastDBIdx = 0;
	SIEGEWAR_GUILDDBINFO GuildInfo[100];

	for( DWORD i=0; i<pMessage->dwResult; ++i )
	{
		LastDBIdx = (DWORD)atoi((char*)pData[i].Data[eSWGI_GuildDBIdx]);
		GuildInfo[i].GuildIdx = (DWORD)atoi((char*)pData[i].Data[eSWGI_GuildIdx]);
		GuildInfo[i].Type = (BYTE)atoi((char*)pData[i].Data[eSWGI_GuildType]);
	}

	SIEGEWARMGR->SetSiegeGuildInfo( GuildInfo, pMessage->dwResult );

	if( pMessage->dwResult >= 100 )
		SiegeWarGuildInfoLoad( LastDBIdx, SIEGEWARMGR->GetSiegeWarIdx(), SIEGEWARMGR->GetSiegeMapNum() );
	else
	{
		if( g_pServerSystem->GetMapNum() == SIEGEWARMGR->GetSiegeMapNum() )
			SIEGEWARMGR->CreateSiegeBattle();
	}
}


void SiegeWarGuildInsert( DWORD SiegeWarIdx, DWORD GuildIdx, DWORD Type, DWORD MapNum )
{
	sprintf( txt, "EXEC %s %d, %d, %d, %d", STORED_SIEGE_GUILDINSERT, SiegeWarIdx, GuildIdx, Type, MapNum );
	g_DB.Query( eQueryType_FreeQuery, eSiegeWarGuildInsert, 0, txt );
}

void SiegeWarGuildUpdate( DWORD SiegeWarIdx, DWORD GuildIdx, DWORD Type, DWORD MapNum )
{
	sprintf( txt, "EXEC %s %d, %d, %d, %d", STORED_SIEGE_GUILDUPDATE, SiegeWarIdx, GuildIdx, Type, MapNum );
	g_DB.Query( eQueryType_FreeQuery, eSiegeWarGuildUpdate, 0, txt );
}

void SiegeWarGuildDelete( DWORD SiegeWarIdx, DWORD GuildIdx, DWORD MapNum )
{
	sprintf( txt, "EXEC %s %d, %d, %d", STORED_SIEGE_GUILDDELETE, SiegeWarIdx, GuildIdx, MapNum );
	g_DB.Query( eQueryType_FreeQuery, eSiegeWarGuildDelete, 0, txt );
}

void SiegeWarAddLog( DWORD SiegeWarIdx, DWORD MapNum, DWORD GuildIdx, DWORD Logkind, DWORD Value )
{
	sprintf( txt, "EXEC dbo.Up_SW_AddlOg %d, %d, %d, %d, %d", SiegeWarIdx, MapNum, GuildIdx, Logkind, Value );
	g_DB.LogQuery( eQueryType_FreeQuery, eSiegeWarAddLog, 0, txt );
}


void ChangeCharacterAttr( DWORD dwPlayerIdx, DWORD dwAttr )
{
	sprintf( txt, "EXEC dbo.MP_CHARACTER_ChangeAttr_JP %d, %d", dwPlayerIdx, dwAttr );
	g_DB.Query( eQueryType_FreeQuery, eChangeCharacterAttr, 0, txt );
}

void SiegeWarProfitInfoLoad()
{
	sprintf( txt, "EXEC dbo.MP_SIEGEWAR_PROFIT_InfoLoad" );
	g_DB.Query( eQueryType_FreeQuery, eSiegeWarProfitInfoLoad, 0, txt );
}

void RSiegeWarProfitInfoLoad( LPQUERY pData, LPDBMESSAGE pMessage )
{
	sSWPROFIT sInfo;
	for( DWORD i = 0; i < pMessage->dwResult; ++i )
	{
		memset( &sInfo, 0, sizeof(sInfo) );
		sInfo.nSWMapNum = atoi((char*)pData[i].Data[0]);
		sInfo.dwGuildIdx = (DWORD)atoi((char*)pData[i].Data[1]);
		sInfo.nTaxRateForStore = atoi((char*)pData[i].Data[2]);
		sInfo.dwTotalMoney = (DWORD)atoi((char*)pData[i].Data[3]);
		sInfo.RegDate.value = (DWORD)atoi((char*)pData[i].Data[4]);
		SWPROFITMGR->AddProfitTable( sInfo );
	}	

	//
	//g_pServerSystem->SetStart( TRUE );

#ifdef _HK_LOCAL_	//hk block
	g_pServerSystem->SetStart( TRUE );
#elif defined _JAPAN_LOCAL_	//japan block
	g_pServerSystem->SetStart( TRUE );
	//GuildLoadGuildPointInfo(0);
#else
	//SW060719 巩颇器牢飘
	GuildLoadGuildPointInfo(0);
#endif
}

void SiegeWarProfitUpdateGuild( int nMapNum, DWORD dwGuildIdx )
{
	sprintf( txt, "EXEC dbo.MP_SIEGEWAR_PROFIT_UpdateGuild %d, %d", nMapNum, dwGuildIdx );
	g_DB.Query( eQueryType_FreeQuery, eSiegeWarProfitUpdateGuild, 0, txt );
}

void SiegeWarProfitUpdateTaxRate( int nMapNum, int nTaxRate, DWORD dwRegDate )
{
	sprintf( txt, "EXEC dbo.MP_SIEGEWAR_PROFIT_UpdateTaxRate %d, %d, %d", nMapNum, nTaxRate, dwRegDate );
	g_DB.Query( eQueryType_FreeQuery, eSiegeWarProfitUpdateTaxRate, 0, txt );
}

void SiegeWarProfitUpdateProfitMoney( int nMapNum, DWORD dwMoney, int nType )
{
	sprintf( txt, "EXEC dbo.MP_SIEGEWAR_PROFIT_UpdateProfitMoney %d, %u, %d", nMapNum, dwMoney, nType );
	g_DB.Query( eQueryType_FreeQuery, eSiegeWarProfitUpdateProfitMoney, 0, txt );
}

void RSiegeWarProfitUpdateProfitMoney( LPQUERY pData, LPDBMESSAGE pMessage )
{
	if( !pMessage->dwResult ) return;

	SWPROFITMGR->SetProfitMoney( atoi((char*)pData[0].Data[0]), (DWORD)atoi((char*)pData[0].Data[1]) );
}

void ChangeGuildLocation( DWORD dwGuildIdx, WORD wMapNum )
{
	sprintf( txt, "EXEC dbo.MP_GUILD_ChangeLocation %d, %d", dwGuildIdx, wMapNum );
	g_DB.Query( eQueryType_FreeQuery, eChangeGuildLocation, 0, txt );	
}

void ChangeCharacterStageAbility( DWORD dwCharacterIdx, BYTE stage, CAbilityGroup* pGroup )
{
	ABILITY_TOTALINFO TInfo;
	ABILITYMGR->GetAbilityTotalInfo( pGroup, &TInfo );

	char battle[MAX_ABILITY_NUM_PER_GROUP+1] = {0,};
	char hwa[6] = {0,};
	char geuk[6] = {0,};
	memcpy( battle, TInfo.AbilityDataArray[eAGK_Battle], MAX_ABILITY_NUM_PER_GROUP+1 );
	memcpy( hwa, &TInfo.AbilityDataArray[eAGK_Battle][17], 6 );
	memcpy( geuk, &TInfo.AbilityDataArray[eAGK_Battle][23], 6 );

	if( stage == eStage_Hwa )
	{
		memcpy( &battle[17], "AAAAAA", 6 );
		memcpy( &battle[23], hwa, 6 );
	}
	else if( stage == eStage_Geuk )
	{
		memcpy( &battle[17], geuk, 6 );
		memcpy( &battle[23], "AAAAAA", 6 );
	}
	else if( stage == eStage_Normal )
	{
		memcpy( &battle[17], "AAAAAA", 6 );
		memcpy( &battle[23], "AAAAAA", 6 );
	}

	sprintf( txt, "EXEC dbo.MP_CHARACTER_ChangeStage_Ability %d, \'%s\'", dwCharacterIdx, battle );
	g_DB.Query( eQueryType_FreeQuery, eChangeCharacterStageAbility, 0, txt );
}

void RShopItemRareInsertToDB(LPQUERY pData,  LPDBMESSAGE pMessage)
{
	DWORD characteridx = (DWORD)atoi((char*)pData[0].Data[eCIR_ObjectID]);
	DWORD pos = (DWORD)atoi((char*)pData[0].Data[eCIR_Position]);

	CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser( characteridx );
	if( !pPlayer )			return;
	
	ITEMBASE* pItem = (ITEMBASE*)ITEMMGR->GetItemInfoAbsIn( pPlayer, (POSTYPE)pos );
	if( !pItem )			return;

	if( pItem->RareIdx )
	{
		if( FALSE == pPlayer->RemoveItemRareOption( pItem->RareIdx ) )
		{
		//	char szBuf[128];
		//	sprintf( szBuf, "ShopItemRareInsert_Error! PlayerID : %d\t ItemDBIdx : %d\n", pPlayer->GetID(),
		//		pItem->dwDBIdx );
		//	ASSERTMSG(0, szBuf);
		}
	}


	ITEM_RARE_OPTION_INFO RareOptionInfo;
	RareOptionInfo.dwRareOptionIdx				= (DWORD)atoi((char*)pData[0].Data[eCIR_RareID]);	//!惑荐 蝶肺 救父甸绢档 瞪淀
	RareOptionInfo.dwItemDBIdx				= (DWORD)atoi((char*)pData[0].Data[eCIR_ItemDBID]);
	RareOptionInfo.GenGol					= (WORD)atoi((char*)pData[0].Data[eCIR_GenGol]);
	RareOptionInfo.MinChub					= (WORD)atoi((char*)pData[0].Data[eCIR_MinChub]);				
	RareOptionInfo.CheRyuk					= (WORD)atoi((char*)pData[0].Data[eCIR_CheRyuk]);				
	RareOptionInfo.SimMek					= (WORD)atoi((char*)pData[0].Data[eCIR_SimMek]);					
	RareOptionInfo.Life						= (DWORD)atoi((char*)pData[0].Data[eCIR_Life]);					
	RareOptionInfo.NaeRyuk					= (WORD)atoi((char*)pData[0].Data[eCIR_NaeRyuk]);				
	RareOptionInfo.Shield					= (DWORD)atoi((char*)pData[0].Data[eCIR_Shield]);					
	RareOptionInfo.AttrRegist.SetElement_Val(ATTR_FIRE,		(float)atoi((char*)pData[0].Data[eCIR_FireAttrRegist])/100.f );
	RareOptionInfo.AttrRegist.SetElement_Val(ATTR_WATER,	(float)atoi((char*)pData[0].Data[eCIR_WaterAttrRegist])/100.f );
	RareOptionInfo.AttrRegist.SetElement_Val(ATTR_TREE,		(float)atoi((char*)pData[0].Data[eCIR_TreeAttrRegist])/100.f );
	RareOptionInfo.AttrRegist.SetElement_Val(ATTR_IRON,		(float)atoi((char*)pData[0].Data[eCIR_GoldAttrRegist])/100.f );
	RareOptionInfo.AttrRegist.SetElement_Val(ATTR_EARTH,	(float)atoi((char*)pData[0].Data[eCIR_EarthAttrRegist])/100.f );
	RareOptionInfo.PhyAttack				= (WORD)atoi((char*)pData[0].Data[eCIR_PhyAttack]);
	//		RareOptionInfo.CriticalPercent			= (WORD)atoi((char*)pData[0].Data[eCIO_Critical]);
	RareOptionInfo.AttrAttack.SetElement_Val(ATTR_FIRE,		(float)atoi((char*)pData[0].Data[eCIR_FireAttrAttack])/100.f );
	RareOptionInfo.AttrAttack.SetElement_Val(ATTR_WATER,	(float)atoi((char*)pData[0].Data[eCIR_WaterAttrAttack])/100.f );
	RareOptionInfo.AttrAttack.SetElement_Val(ATTR_TREE,		(float)atoi((char*)pData[0].Data[eCIR_TreeAttrAttack])/100.f );
	RareOptionInfo.AttrAttack.SetElement_Val(ATTR_IRON,		(float)atoi((char*)pData[0].Data[eCIR_GoldAttrAttack])/100.f );
	RareOptionInfo.AttrAttack.SetElement_Val(ATTR_EARTH,	(float)atoi((char*)pData[0].Data[eCIR_EarthAttrAttack])/100.f );
	RareOptionInfo.PhyDefense				= (WORD)atoi((char*)pData[0].Data[eCIR_PhyDefense]);


	ITEMBASE info;
	SafeStrCpy(info.Green,(char*)pData[0].Data[eCIR_Green], MAX_NAME_LENGTH+1);

	pPlayer->AddItemRareOption(&RareOptionInfo);

	pItem->RareIdx = RareOptionInfo.dwRareOptionIdx;

	// Log扁废
	LogItemRareOption(pPlayer->GetID(), pItem->dwDBIdx, &RareOptionInfo);

	MSG_ITEM_RAREITEM_GET msg;
	SetProtocol( &msg, MP_ITEM, MP_ITEM_SHOPITEM_RARECREATE_ACK );
	msg.dwObjectID = pPlayer->GetID();	//pPlayer->GetID();
	msg.RareItemBase = *pItem;
	msg.RareInfo = RareOptionInfo;
	pPlayer->SendMsg(&msg, sizeof(msg));
}

void QuestEventCheck( DWORD dwUserIdx, DWORD dwCharacterIdx, DWORD dwNpcIdx, DWORD dwQuestIdx, char* pTime )
{
	// 2014-06-25 check string param !

	if( !isValidQueryString(pTime))
	{
	   return;
	}

	if( CheckString(pTime))
	{
		return;
	}
	sprintf( txt, "EXEC dbo.MP_QUEST_EventCheck %d, %d, %d, %d, \'%s\'", dwUserIdx, dwCharacterIdx, dwNpcIdx, dwQuestIdx, pTime );
	g_DB.Query( eQueryType_FreeQuery, eQuestEventCheck, dwCharacterIdx, txt );

/*
		CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser( dwCharacterIdx );
		if( !pPlayer )	return;
	
		MSG_DWORD2 Msg;
		Msg.Category = MP_QUEST; 
	
		DWORD dwRet = 0;
		switch( dwRet )
		{
		case 0:		// 且荐乐促
			{
				Msg.Protocol = MP_QUESTEVENT_NPCTALK_ACK;
	
			}
			break;
		case 1:		// 捞固 沁促
			{
				Msg.Protocol = MP_QUESTEVENT_NPCTALK_NACK;
			}
			break;
		case 2:		// 且 荐 绝促
			{
				Msg.Protocol = MP_QUESTEVENT_NPCTALK_NACK;
			}
			break;
		default:
			return;
		}
	
		Msg.dwData1 = dwQuestIdx;
		Msg.dwData2 = dwRet;
		pPlayer->SendMsg( &Msg, sizeof(Msg) );*/
	
}

void RQuestEventCheck( LPQUERY pData, LPDBMESSAGE pMessage )
{
	CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser( pMessage->dwID );
	if( !pPlayer )	return;

	MSG_DWORD2 Msg;
	Msg.Category = MP_QUEST; 

	DWORD dwNpcIdx = (DWORD)atoi((char*)pData[0].Data[0]);
	DWORD dwQuestIdx = (DWORD)atoi((char*)pData[0].Data[1]);
	DWORD dwRet = (DWORD)atoi((char*)pData[0].Data[2]);
	switch( dwRet )
	{
	case 0:		// 且荐乐促
		{
			Msg.Protocol = MP_QUESTEVENT_NPCTALK_ACK;
			
			// quest event
			CQuestEvent qe;
			qe.SetValue( eQuestEvent_NpcTalk, dwNpcIdx, dwQuestIdx );
			QUESTMGR->AddQuestEvent( pPlayer, &qe );
		}
		break;
	case 1:		// 捞固 沁促
		{
			Msg.Protocol = MP_QUESTEVENT_NPCTALK_NACK;
		}
		break;
	case 2:		// 且 荐 绝促
		{
			Msg.Protocol = MP_QUESTEVENT_NPCTALK_NACK;
		}
		break;
	default:
		return;
	}

	Msg.dwData1 = dwQuestIdx;
	Msg.dwData2 = dwRet;
	pPlayer->SendMsg( &Msg, sizeof(Msg) );
}

void QuestEventEnd( DWORD dwUserIdx, DWORD dwCharacterIdx, char* pTime )
{
	// 2014-06-25 check string param !

	if( !isValidQueryString(pTime))
	{
	   return;
	}

	if( CheckString(pTime))
	{
		return;
	}

	sprintf( txt, "EXEC dbo.MP_QUEST_EventEnd %d, %d, \'%s\'", dwUserIdx, dwCharacterIdx, pTime );
	g_DB.Query( eQueryType_FreeQuery, eQuestEventCheck, 0, txt );
}



// event 060627 - 款康评 捞亥飘
void GMEvent01( DWORD dwUserIdx, DWORD dwCharacterIdx, DWORD dwLevel, int nServerSetNum )
{
	sprintf( txt, "EXEC dbo.MP_GMEvent01 %d, %d, %d, %d", dwUserIdx, dwCharacterIdx, dwLevel, nServerSetNum );
	g_DB.Query( eQueryType_FreeQuery, eGMEvent01, 0, txt );
}

// magi82 //////////////////////////////////////////////////////////////////////////
void UCharacterExpFlag(DWORD CharacterIDX, BYTE ExpFlag)
{
	sprintf(txt, "EXEC %s %d, %d", STORED_CHARACTER_EXPFLAG, CharacterIDX, ExpFlag);
	g_DB.Query(eQueryType_FreeQuery, eCharacterExpFlag, CharacterIDX, txt);
}
////////////////////////////////////////////////////////////////////////////////////

//magi82 - 巩窍积 啊涝祈府矫胶袍 ///////////////////////////////////////////////////
void GuildTraineeInfo(DWORD StartDBIdx)
{
	sprintf(txt, "EXEC %s %u", STORED_GUILD_TRAINEE_INFO, StartDBIdx);
	g_DB.Query(eQueryType_FreeQuery, eGuildTraineeInfo, 0, txt);
}

void RGuildTraineeInfo(LPQUERY pData, LPDBMESSAGE pMessage)
{
	GUILD_TRAINEE_LIST* temp;
	for( DWORD i = 0; i < pMessage->dwResult; i++ )
	{
		temp = new GUILD_TRAINEE_LIST;
		ZeroMemory(temp, sizeof(GUILD_TRAINEE_LIST));

		temp->dwGuildIdx = atoi( (char*)pData[i].Data[eGTII_GuildIdx] );
		temp->dwUserIdx = atoi( (char*)pData[i].Data[eGTII_UserIdx] );
		SafeStrCpy( temp->strName, (char*)pData[i].Data[eGTII_UserName], MAX_NAME_LENGTH+1 );
		temp->dwTime = atoi( (char*)pData[i].Data[eGTII_JoinTime] );

		GUILDMGR->AddGuildTraineeInfo( temp );		
	}

	ASSERT(pMessage->dwResult <= MAX_QUERY_RESULT);
	if(pMessage->dwResult >= MAX_QUERY_RESULT)
	{
		DWORD dwTemp = atoi((char*)pData[99].Data[eGTII_DBIdx]);
		GuildTraineeInfo( dwTemp );
	}
}

void GuildTraineeInsert(DWORD GuildIdx, DWORD UserIdx, char UserName[], DWORD JoinTime)
{
    // 2014-06-25 check string param !

	if( !isValidQueryString(UserName))
	{
	   return;
	}

	if( CheckString(UserName))
	{
		return;
	}
	sprintf(txt, "EXEC %s %u, %u, '%s', %u", STORED_GUILD_TRAINEE_INSERT, GuildIdx, UserIdx, UserName, JoinTime);
	g_DB.Query(eQueryType_FreeQuery, eGuildTraineeInsert, 0, txt);
}

void GuildTraineeDelete(DWORD UserIdx) 
{
	sprintf(txt, "EXEC %s %u", STORED_GUILD_TRAINEE_DELETE, UserIdx);
	g_DB.Query(eQueryType_FreeQuery, eGuildTraineeDelete, 0, txt);
}

void GuildTraineeGuildInfo(DWORD StartDBIdx)
{
	sprintf(txt, "EXEC %s %u", STORED_GUILD_TRAINEE_GUILDINFO, StartDBIdx);
	g_DB.Query(eQueryType_FreeQuery, eGuildTraineeGuildInfo, 0, txt);
}

void RGuildTraineeGuildInfo(LPQUERY pData, LPDBMESSAGE pMessage)
{
	GUILD_TRAINEE_GUILDLIST* temp;
	for( DWORD i = 0; i < pMessage->dwResult; i++ )
	{
		temp = new GUILD_TRAINEE_GUILDLIST;
		ZeroMemory(temp, sizeof(GUILD_TRAINEE_GUILDLIST));

		temp->dwGuildIdx = atoi( (char*)pData[i].Data[eGTGII_GuildIdx] );
		temp->dwRank = atoi( (char*)pData[i].Data[eGTGII_GuildRank] ); 

		GUILDMGR->AddGuildTraineeGuildInfo( temp );		
	}

	ASSERT(pMessage->dwResult <= MAX_QUERY_RESULT);
	if(pMessage->dwResult >= MAX_QUERY_RESULT)
	{
		DWORD dwTemp = atoi((char*)pData[99].Data[eGTGII_DBIdx]);
		GuildTraineeGuildInfo( dwTemp );
	}
}

void GuildTraineeGuildInsert(DWORD GuildIdx)
{
	g_DB.FreeMiddleQuery( RGuildTraineeGuildInsert, 0, 
		"EXEC %s %u", STORED_GUILD_TRAINEE_GUILDINSERT, GuildIdx );
}

void RGuildTraineeGuildInsert(LPMIDDLEQUERY pData, LPDBMESSAGE pMessage)
{
	GUILD_INTRO_INFO* temp;
	temp = new GUILD_INTRO_INFO;
	ZeroMemory(temp, sizeof(GUILD_INTRO_INFO));

	temp->dwGuildIdx = atoi( (char*)pData[0].Data[eGTGIII_DBIdx] );
	SafeStrCpy( temp->strIntro, (char*)pData[0].Data[eGTGIII_Intro], MAX_GUILD_INTRO+1 );

	GUILDMGR->AddGuildTraineeGuildIntroInfo( temp );

	// Brodcasting to Mapserver
	MSG_MUNHAINFOUPDATE msg;
	SetProtocol( &msg, MP_GUILD, MP_GUILD_MUNPA_REGIST_INTRO_BROD );
	msg.Index = temp->dwGuildIdx;
	SafeStrCpy( msg.Intro, temp->strIntro, MAX_GUILD_INTRO+1 );
	PACKEDDATA_OBJ->SendToBroadCastMapServer( &msg, sizeof(msg) );
}

void GuildTraineeGuildDelete(DWORD GuildIdx)
{
	sprintf(txt, "EXEC %s %u", STORED_GUILD_TRAINEE_GUILDDELETE, GuildIdx);
	g_DB.Query(eQueryType_FreeQuery, eGuildTraineeGuildDelete, 0, txt);
}

void GuildTraineeDeletebyGuild(DWORD GuildIdx)
{
	sprintf(txt, "EXEC %s %u", STORED_GUILD_TRAINEE_DELETEBYGUILD, GuildIdx);
	g_DB.Query(eQueryType_FreeQuery, eGuildTraineeDeletebyGuild, 0, txt);
}

void GuildAddStudent(DWORD GuildIdx, DWORD MemberIDX, char* GuildName, BYTE Rank)
{
    // 2014-06-25 check string param !

	if( !isValidQueryString(GuildName))
	{
	   return;
	}

	if( CheckString(GuildName))
	{
		return;
	}
	sprintf(txt, "EXEC %s %u, %u, \'%s\', %d", STORED_GUILD_ADD_STUDENT, GuildIdx, MemberIDX, GuildName, Rank);
	g_DB.Query(eQueryType_FreeQuery, eGuildAddStudent, 0, txt);
}

void RGuildAddStudent(LPQUERY pData, LPDBMESSAGE pMessage)
{
	int targetID = atoi((char*)pData[0].Data[eGASI_CharIdx]);
	int targetLev = atoi((char*)pData[0].Data[eGASI_Level]);
	int guildID = atoi((char*)pData[0].Data[eGASI_GuildIdx]);
	char strName[MAX_NAME_LENGTH+1] = { 0, };
	SafeStrCpy( strName, (char*)pData[0].Data[eGASI_Name], MAX_NAME_LENGTH+1 );

	CGuild* pGuild = GUILDMGR->GetGuild(guildID);
	if( !pGuild )
		return;

	GUILDMEMBERINFO member;
	member.InitInfo(targetID, strName, targetLev, GUILD_STUDENT, TRUE);
	pGuild->AddMember(&member);
	pGuild->SetLogInfo(targetID, FALSE);

	//notify
	SEND_GUILD_MEMBER_INFO notify;
	notify.Category = MP_GUILD;
	notify.Protocol = MP_GUILD_ADDMEMBER_NOTIFY_TO_MAP;
	notify.GuildIdx = guildID;
	SafeStrCpy(notify.GuildName, pGuild->GetGuildName(), MAX_GUILD_NAME+1);
	notify.MemberInfo.MemberIdx = targetID;
	notify.MemberInfo.Memberlvl = targetLev;
	SafeStrCpy(notify.MemberInfo.MemberName, strName, MAX_NAME_LENGTH+1);
	notify.MemberInfo.Rank = GUILD_STUDENT;
	g_Network.Send2AgentServer((char*)&notify, sizeof(notify));
}

void GuildTraineeGuildIntroInfo( DWORD StartDBIdx )
{	
	g_DB.FreeMiddleQuery( RGuildTraineeGuildIntroInfo, 0, 
		"EXEC %s %u", STORED_GUILD_TRAINEE_GUILDINTROINFO, StartDBIdx );
}

void RGuildTraineeGuildIntroInfo(LPMIDDLEQUERY pData, LPDBMESSAGE pMessage)
{
	GUILD_INTRO_INFO* temp;
	for( DWORD i = 0; i < pMessage->dwResult; i++ )
	{
		temp = new GUILD_INTRO_INFO;
		ZeroMemory(temp, sizeof(GUILD_INTRO_INFO));

		temp->dwGuildIdx = atoi( (char*)pData[i].Data[eGTGIII_DBIdx] );
		SafeStrCpy( temp->strIntro, (char*)pData[i].Data[eGTGIII_Intro], MAX_GUILD_INTRO+1 );

		GUILDMGR->AddGuildTraineeGuildIntroInfo( temp );		
	}

	ASSERT(pMessage->dwResult <= MAX_QUERY_RESULT);
	if(pMessage->dwResult >= MAX_QUERY_RESULT)
	{
		DWORD dwTemp = atoi((char*)pData[99].Data[eGTGIII_DBIdx]);
		GuildTraineeGuildIntroInfo( dwTemp );
	}
}

void GuildTraineeGuildIntroUpdate(DWORD GuildIdx, char* GuildIntro)
{
    // 2014-06-25 check string param !

	if( !isValidQueryString(GuildIntro))
	{
	   return;
	}

	if( CheckString(GuildIntro))
	{
		return;
	}
	g_DB.FreeMiddleQuery( NULL, 0, 
		"EXEC %s %u, \'%s\'", STORED_GUILD_TRAINEE_GUILDINTROUPDATE, GuildIdx, GuildIntro );
}
////////////////////////////////////////////////////////////////////////////////////

void GuildStudentLvUpCtUpdate( DWORD GuildIdx, DWORD AddPoint, DWORD ToPoint )
{
	sprintf(txt, "EXEC dbo.MP_GUILD_Update_LvCounter %d, %d, %d", GuildIdx, AddPoint, ToPoint);
	g_DB.Query(eQueryType_FreeQuery, eGuildStudentLvUpCount, GuildIdx, txt);
}

void RGuildStudentLvUpCtUpdate( LPQUERY pData, LPDBMESSAGE pMessage )
{
	DWORD GuildIdx = pMessage->dwID;
	DWORD count = pMessage->dwResult;

	if(!count)	return;

	DWORD GuildStudentLvUpCount = atoi((char*)pData->Data[0]);
	BOOL bMasterChecking = atoi((char*)pData->Data[1]);

	GUILDMGR->SetGuildStudentLvUpCount(GuildIdx, GuildStudentLvUpCount, bMasterChecking);
}

void GuildStudentLvUpCtInit( DWORD GuildIdx, DWORD MasterIDX )
{
	sprintf(txt, "EXEC dbo.MP_GUILD_Init_LvCounter %d, %d", GuildIdx, MasterIDX);
	g_DB.Query(eQueryType_FreeQuery, eGuildStudentLvUpInfoInit, GuildIdx, txt);

}

void RGuildStudentLvUpCtInit( LPQUERY pData, LPDBMESSAGE pMessage )
{
	DWORD GuildIdx = pMessage->dwID;
	DWORD count = pMessage->dwResult;

	if(!count)	return;

	BOOL bInited = atoi((char*)pData->Data[0]);
	//巩颇俊霸 绵窍陛 瘤鞭
	if(bInited)
		GUILDMGR->GiveGuildCerebrationMoney(GuildIdx);
	else
		ASSERT(0);
}

// magi82 - Titan(070209)
void TitanWearItemInfo( DWORD CharacterIdx, POSTYPE FromPos, POSTYPE ToPos )
{
	sprintf(txt, "EXEC %s %d, %d, %d", STORED_TITAN_WEARITEMINFO, CharacterIdx, FromPos, ToPos);
	g_DB.Query(eQueryType_FreeQuery, eTitanWearItemInfo, CharacterIdx, txt);
}

void RTitanWearItemInfo( LPQUERY pData, LPDBMESSAGE pMessage )
{
	CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser(pMessage->dwID);
	if( !pPlayer )	return;

	ITEMBASE ItemBase[SLOT_TITANWEAR_NUM];
	memset( ItemBase, 0, sizeof(ITEMBASE)*SLOT_TITANWEAR_NUM );

	ITEMBASE ShopItemBase[SLOT_TITANSHOPITEM_NUM];
	memset( ShopItemBase, 0, sizeof(ITEMBASE)*SLOT_TITANSHOPITEM_NUM );

	int ret = atoi((char*)pData[0].Data[0]);
	if( ret == 0 )
	{
		pPlayer->InitTitanWearItemInfo( ItemBase );
		pPlayer->InitTitanShopItemInfo( ShopItemBase );
		pPlayer->SetInitState(PLAYERINITSTATE_ITEM_INFO,MP_USERCONN_GAMEIN_SYN);
	}
	else
	{
		if( pMessage->dwResult > SLOT_TITANWEAR_NUM + SLOT_TITANSHOPITEM_NUM )
		{
		//	char Buf[64];
		//	sprintf(Buf, "TitanWearItem/TitanShopItem OverNum CharacterID:%d", pPlayer->GetID());
		//	ASSERTMSG(0, Buf);
		}

		for(DWORD i = 0; i < pMessage->dwResult; i++)
		{
			WORD wIconIdx = atoi((char*)pData[i].Data[eCI_IDX]);
			ITEM_INFO* pInfo = ITEMMGR->GetItemInfo( wIconIdx );

			if(pInfo->ItemKind == eSHOP_ITEM_TITAN_EQUIP)
			{
				POSTYPE ItemPos = atoi((char*)pData[i].Data[eCI_Position]);
				ITEMBASE* pItemBase = NULL;

				if(ItemPos >= TP_TITANSHOPITEM_START && ItemPos < TP_TITANSHOPITEM_END)
				{
					ItemPos -= TP_TITANSHOPITEM_START;

					pItemBase = &ShopItemBase[ItemPos];

					pItemBase->dwDBIdx = atoi((char*)pData[i].Data[eCI_DBIDX]);
					pItemBase->wIconIdx = wIconIdx;
					pItemBase->Position = atoi((char*)pData[i].Data[eCI_Position]);
					pItemBase->Durability = atoi((char*)pData[i].Data[eCI_Durability]);
					pItemBase->QuickPosition = atoi((char*)pData[i].Data[eCI_QPosition]);
					pItemBase->ItemParam = atoi((char*)pData[i].Data[eCI_Param]);

					pPlayer->GetTitanManager()->SetTitanShopitemOption(pItemBase->wIconIdx, TRUE);	// magi82(26)
				}
			}
			else if(pInfo->ItemKind & eTITAN_EQUIPITEM)
			{
				POSTYPE ItemPos = atoi((char*)pData[i].Data[eCI_Position]);
				ITEMBASE* pItemBase = NULL;

				if(ItemPos >= TP_TITANWEAR_START && ItemPos < TP_TITANWEAR_END)
				{
					ItemPos -= TP_TITANWEAR_START;

					pItemBase = &ItemBase[ItemPos];

					pItemBase->dwDBIdx = atoi((char*)pData[i].Data[eCI_DBIDX]);
					pItemBase->wIconIdx = wIconIdx;
					pItemBase->Position = atoi((char*)pData[i].Data[eCI_Position]);
					pItemBase->Durability = atoi((char*)pData[i].Data[eCI_Durability]);
					pItemBase->QuickPosition = atoi((char*)pData[i].Data[eCI_QPosition]);
					pItemBase->ItemParam = atoi((char*)pData[i].Data[eCI_Param]);

					// magi82(33) 鸥捞藕 厘馒牢亥俊 乐绰 鸥捞藕 厘馒酒捞袍狼 抗寇贸府
					ITEM_INFO* pInfo = ITEMMGR->GetItemInfo(pItemBase->wIconIdx);
					if( pInfo && pInfo->ItemKind & eTITAN_EQUIPITEM )
					{
						TITAN_ENDURANCE_ITEMINFO* pEndurance = pPlayer->GetTitanManager()->GetTitanItemEnduranceInfo(pItemBase->dwDBIdx);
						if( !pEndurance )
						{
							pPlayer->GetTitanManager()->MakeNewEnduranceInfo(pPlayer, pItemBase, eExceptionInven);				
						}
					}

					pPlayer->SetTitanWearInfo(pItemBase->Position, pItemBase, TRUE);
				}
			}
		}

		pPlayer->InitTitanWearItemInfo( ItemBase );
		pPlayer->InitTitanShopItemInfo( ShopItemBase );
		pPlayer->SetInitState(PLAYERINITSTATE_ITEM_INFO,MP_USERCONN_GAMEIN_SYN);
	}
}
//////////////////////////////////////////////////////////////////////////


void TestGameQuery( DWORD CharacterIdx )
{
	sprintf(txt, "EXEC %s %d", "dbo.MP_TEST_GAMEQUERY", CharacterIdx);
	g_DB.Query(eQueryType_FreeQuery, eTestGameQuery, CharacterIdx, txt);

}

void RTestGameQuery( LPQUERY pData, LPDBMESSAGE pMessage )
{
	CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser(pMessage->dwID);
	if( !pPlayer )	return;

	g_pServerSystem->m_ProcessInfo.ProcessInfo.GameQueryReturntime = gCurTime - g_pServerSystem->m_ProcessInfo.StarTime;
	if( g_pServerSystem->m_ProcessInfo.ProcessInfo.GameQueryReturntime == 0 )
		g_pServerSystem->m_ProcessInfo.ProcessInfo.GameQueryReturntime = 1;

	if( g_pServerSystem->m_ProcessInfo.Count > 4 && g_pServerSystem->m_ProcessInfo.ProcessInfo.LogQueryReturntime )
	{
		g_pServerSystem->m_bCheckInfo = FALSE;
		pPlayer->SendMsg( &g_pServerSystem->m_ProcessInfo, sizeof(g_pServerSystem->m_ProcessInfo) );
	}
}

void TestLogQuery( DWORD CharacterIdx )
{
	sprintf(txt, "EXEC %s %d", "dbo.UP_TEST_LOGQUERY", CharacterIdx);
	g_DB.LogQuery(eQueryType_FreeQuery, eTestLogQuery, CharacterIdx, txt);

}

void RTestLogQuery( LPQUERY pData, LPDBMESSAGE pMessage )
{
	CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser(pMessage->dwID);
	if( !pPlayer )	return;

	g_pServerSystem->m_ProcessInfo.ProcessInfo.LogQueryReturntime = gCurTime - g_pServerSystem->m_ProcessInfo.StarTime;
	if( g_pServerSystem->m_ProcessInfo.ProcessInfo.LogQueryReturntime == 0 )
		g_pServerSystem->m_ProcessInfo.ProcessInfo.LogQueryReturntime = 1;

	if( g_pServerSystem->m_ProcessInfo.Count > 4 && g_pServerSystem->m_ProcessInfo.ProcessInfo.GameQueryReturntime )
	{
		g_pServerSystem->m_bCheckInfo = FALSE;
		pPlayer->SendMsg( &g_pServerSystem->m_ProcessInfo, sizeof(g_pServerSystem->m_ProcessInfo) );
	}
}

void PyogukTitanEnduranceInfo(DWORD CharacterIdx, DWORD UserIdx, DWORD StartDBIdx)
{
	sprintf(txt, "EXEC %s %d, %d", STORED_PYOGUK_TITAN_ENDURANCE_INFO, UserIdx, StartDBIdx);
	g_DB.Query(eQueryType_FreeQuery, ePyogukTitanEnduranceOptionQuery, CharacterIdx, txt);
}

void RPyogukTitanEnduranceInfo( LPQUERY pData, LPDBMESSAGE pMessage )
{
	// CIㄏo AEāi
	CPlayer* pPlayer = NULL;
	pPlayer = (CPlayer *)g_pUserTable->FindUser(pMessage->dwID);
	if(pPlayer == NULL)
		return;

	TITAN_ENDURANCE_ITEMINFO EnduranceInfo;
	for( DWORD  i = 0 ; i < pMessage->dwResult ; ++i )
	{
		EnduranceInfo.ItemDBIdx				= (DWORD)atoi((char*)pData[i].Data[0]);
		EnduranceInfo.CharacterID			= (DWORD)atoi((char*)pData[i].Data[1]);
		EnduranceInfo.ItemIdx				= (DWORD)atoi((char*)pData[i].Data[2]);
		EnduranceInfo.Endurance				= (DWORD)atoi((char*)pData[i].Data[3]);
		EnduranceInfo.UserIdx				= (DWORD)atoi((char*)pData[i].Data[4]);

		pPlayer->GetTitanManager()->AddTitanEquipItemEnduranceInfo(&EnduranceInfo);
	}

	if(pMessage->dwResult >= MAX_TITANENDURANCELOAD_ITEM) 
		PyogukTitanEnduranceInfo(pMessage->dwID, pPlayer->GetUserID(), EnduranceInfo.ItemDBIdx);
	else
		CharacterPyogukItemInfo(pMessage->dwID, 0);
}

//2007. 12. 10. CBH - 胶挪 历厘
void CharacterSkinInfoUpdate(CPlayer* pPlayer)
{
	WORD* pSkinItem = pPlayer->GetShopItemStats()->wSkinItem;

	sprintf(txt, "EXEC %s %d, %d, %d, %d, %d, %d, %d, %d", STORED_CHARACTER_SKININFO, pPlayer->GetID(), 
		pSkinItem[0], pSkinItem[1], pSkinItem[2], pSkinItem[3], pSkinItem[4], pSkinItem[5], pSkinItem[6]);
	g_DB.Query(eQueryType_FreeQuery, eCharacterSkinInfoUpdate, 0, txt);
}

//2008. 1. 23. CBH - 胶挪 肺靛
void CharacterSkinInfo(DWORD dwCharacterIndex)
{
	sprintf(txt, "EXEC MP_CHARACTER_SkinInfo %d", dwCharacterIndex);
	g_DB.Query(eQueryType_FreeQuery, eCharacterSkinInfo, 0, txt);
}

void RCharacterSkinInfo(LPQUERY pData, LPDBMESSAGE pMessage)
{
	CPlayer* pPlayer = NULL;
	pPlayer = (CPlayer *)g_pUserTable->FindUser((DWORD)atoi((char*)pData->Data[0]));
	if(pPlayer == NULL)
		return;

	pPlayer->GetShopItemStats()->wSkinItem[0] = (WORD)atoi((char*)pData->Data[1]);
	pPlayer->GetShopItemStats()->wSkinItem[1] = (WORD)atoi((char*)pData->Data[2]);
	pPlayer->GetShopItemStats()->wSkinItem[2] = (WORD)atoi((char*)pData->Data[3]);
	pPlayer->GetShopItemStats()->wSkinItem[3] = (WORD)atoi((char*)pData->Data[4]);
	pPlayer->GetShopItemStats()->wSkinItem[4] = (WORD)atoi((char*)pData->Data[5]);
	pPlayer->GetShopItemStats()->wSkinItem[5] = (WORD)atoi((char*)pData->Data[6]);
	pPlayer->GetShopItemStats()->wSkinItem[6] = (WORD)atoi((char*)pData->Data[7]);
}

// magi82(41) - 讥酒捞袍 眠啊(胶泡 檬扁拳 林巩辑)
void CharacterUpdateResetStatusPoint(DWORD dwCharacterIDX, DWORD dwStatusPoint)
{
	sprintf(txt, "EXEC %s %d, %d", STORED_CHARACTER_UPDATE_RESETSTATUSPOINT, dwCharacterIDX, dwStatusPoint);
	g_DB.Query(eQueryType_FreeQuery, eCharacterUpdateResetStatusPoint, dwCharacterIDX, txt);
}

void ItemLimitInfoLoadAll( DWORD dwStartDBIdx )
{
	sprintf(txt, "EXEC dbo.MP_ITEM_LIMITINFO_LOADALL %d", dwStartDBIdx);
	g_DB.Query(eQueryType_FreeQuery, eItemLimitInfoLoadAll, dwStartDBIdx, txt);
}

void RItemLimitInfoLoadAll( LPQUERY pData, LPDBMESSAGE pMessage )
{
	if( 0 == pMessage->dwResult )
		return;

	DWORD dwDBIdx = 0;
	DWORD dwItemIdx = 0;
	int nItemLimitCount = 0;
	int nItemCurrentCount = 0;
	for( DWORD i = 0; i < pMessage->dwResult; ++i )
	{
		dwDBIdx = (DWORD)atoi((char*)pData[i].Data[0]);
		dwItemIdx = (DWORD)atoi((char*)pData[i].Data[1]);
		nItemLimitCount = atoi((char*)pData[i].Data[2]);
		nItemCurrentCount = atoi((char*)pData[i].Data[3]);

        ITEMLIMITMGR->SetItemLimitInfoFromDB( dwItemIdx, nItemLimitCount, nItemCurrentCount );
	}

	if(pMessage->dwResult == MAX_QUERY_RESULT)
	{
		ItemLimitInfoLoadAll( dwDBIdx );
	}
}

void ItemLimitInfoUpdate( DWORD dwItemIdx, int nItemLimitCount, int nItemCurrentCount )
{
	sprintf(txt, "EXEC dbo.MP_ITEM_LIMITINFO_UPDATE %d, %d, %d", dwItemIdx, nItemLimitCount, nItemCurrentCount );
	g_DB.Query(eQueryType_FreeQuery, eItemLimitInfoUpdate, dwItemIdx, txt);
}

void RItemLimitInfoUpdate( LPQUERY pData, LPDBMESSAGE pMessage )
{
	if( 0 == pMessage->dwResult )
		return;

	DWORD dwItemIdx = (DWORD)atoi((char*)pData[0].Data[0]);
	int nItemLimitCount = atoi((char*)pData[0].Data[1]);
	int nItemCurrentCount = atoi((char*)pData[0].Data[2]);
    
    ITEMLIMITMGR->SetItemLimitInfoFromDB( dwItemIdx, nItemLimitCount, nItemCurrentCount );

	MSG_DWORD2 msg;
	msg.Category = MP_ITEMLIMIT;
	msg.Protocol = MP_ITEMLIMIT_ADDCOUNT_TO_MAP;
	msg.dwData1 = dwItemIdx;
	msg.dwData2 = (DWORD)nItemCurrentCount;

	g_Network.Send2AgentServer( (char*)&msg, sizeof(msg) );
}

// autonnote
void AutoNoteListLoad( DWORD dwCharacterIdx )
{
	sprintf(txt, "EXEC dbo.MP_AUTONOTELIST_LOAD %d", dwCharacterIdx );
	g_DB.Query(eQueryType_FreeQuery, eAutoNoteListLoad, dwCharacterIdx, txt);
}

void RAutoNoteListLoad( LPQUERY pData, LPDBMESSAGE pMessage )
{
	if( 0 == pMessage->dwResult )
		return;

	CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser( pMessage->dwID );
	if( !pPlayer )	return;

	MSG_AUTOLIST_ALL msg;
	msg.Category = MP_AUTONOTE;
	msg.Protocol = MP_AUTONOTE_LIST_ALL;
	msg.nCount = (int)pMessage->dwResult;

	for( DWORD i = 0; i < pMessage->dwResult; ++i )
	{
		SafeStrCpy( msg.row[i].Name, (char*)pData[i].Data[0], MAX_NAME_LENGTH+1 );
		SafeStrCpy( msg.row[i].Date, (char*)pData[i].Data[1], 20 );
	}

	pPlayer->SendMsg( &msg, msg.GetMsgLength() );
}

void AutoNoteListAdd( DWORD dwCharacterIdx, DWORD dwAutoUserIdx, DWORD dwAutoCharacterIdx, char* pStrAutoName )
{
    // 2014-06-25 check string param !

	if( !isValidQueryString(pStrAutoName))
	{
	   return;
	}

	if( CheckString(pStrAutoName))
	{
		return;
	}
	sprintf(txt, "EXEC dbo.MP_AUTONOTELIST_ADD %d, %d, %d, \'%s\'", dwCharacterIdx, dwAutoUserIdx, dwAutoCharacterIdx, pStrAutoName );
	g_DB.Query(eQueryType_FreeQuery, eAutoNoteListAdd, dwCharacterIdx, txt);
}

void RAutoNoteListAdd( LPQUERY pData, LPDBMESSAGE pMessage )
{
	if( 0 == pMessage->dwResult )
		return;

	CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser( pMessage->dwID );
	if( !pPlayer )	return;

	MSG_AUTOLIST_ADD msg;
	msg.Category = MP_AUTONOTE;
	msg.Protocol = MP_AUTONOTE_LIST_ADD;
	SafeStrCpy( msg.Name, (char*)pData[0].Data[0], MAX_NAME_LENGTH+1 );
	SafeStrCpy( msg.Date, (char*)pData[0].Data[1], 20 );

	pPlayer->SendMsg( &msg, sizeof(msg) );
}

/// 甘 靛而 酒捞袍 眠啊 累诀 by Stiner(2008/05/28)-MapDropItem
void MapItemDropLog( WORD wUserIDX, WORD wCharacterIDX, WORD wItemIDX, WORD wMap, WORD wChannel, WORD wDropCount, WORD wMaxDropCount )
{
	sprintf( txt, "EXEC dbo.UP_MapItemDropLog %d, %d, %d, %d, %d, %d, %d",
			wUserIDX, wCharacterIDX, wItemIDX, wMap, wChannel, wDropCount, wMaxDropCount );
	g_DB.LogQuery( eQueryType_FreeQuery, 0, 0, txt );
}

void MapItemDropListSelect( WORD wMapNum )
{
	sprintf( txt, "EXEC dbo.MP_MAPITEM_DropList_Select %d", wMapNum );
	g_DB.Query( eQueryType_FreeQuery, eMapItemDropListSelect, NULL, txt );
}

void MapItemDropListUpdate( DWORD dwCharacterIdx, WORD wMapNum, WORD wChannel, WORD wItemIDX, WORD wDropCount, WORD wMaxDropCount )
{
	sprintf( txt, "EXEC dbo.MP_MAPITEM_DropList_Update %d, %d, %d, %d, %d",
		wMapNum, wChannel, wItemIDX, wDropCount, wMaxDropCount );
	g_DB.Query( eQueryType_FreeQuery, eMapItemDropListUpdate, dwCharacterIdx, txt );
}

void MapItemDropListInit( WORD wMapNum )
{
	sprintf( txt, "EXEC dbo.MP_MAPITEM_DropList_Init %d", wMapNum );
	g_DB.Query( eQueryType_FreeQuery, eMapItemDropListInit, NULL, txt );
}

void RMapItemDropListSelect( LPQUERY pData, LPDBMESSAGE pMessage )
{
	// DB搬苞蔼栏肺 单捞磐 悼扁拳
	if( 0 == pMessage->dwResult )
		return;

	for( int row = 0 ; row < pMessage->dwResult ; row++ )
	{
		WORD wMapNum		= atoi((char*)pData[row].Data[eMdi_MapNum]);
		WORD wChannel		= atoi((char*)pData[row].Data[eMdi_Channel]);
		WORD wItemIDX		= atoi((char*)pData[row].Data[eMdi_ItemIDX]);
		WORD wDropCount		= atoi((char*)pData[row].Data[eMdi_DropCount]);
		WORD wMaxDropCount	= atoi((char*)pData[row].Data[eMdi_MaxDropCount]);

		MAPITEMDROP_OBJ->SetItemDropInfoByDB( wMapNum, wChannel, wItemIDX, wDropCount, wMaxDropCount );
	}
}

void RMapItemDropListUpdate( LPQUERY pData, LPDBMESSAGE pMessage )
{
	// DB搬苞蔼栏肺 单捞磐 悼扁拳
	if( 0 == pMessage->dwResult )
		return;

	for( int row = 0 ; row < pMessage->dwResult ; row++ )
	{
		WORD wMapNum		= atoi((char*)pData[row].Data[eMdi_MapNum]);
		WORD wChannel		= atoi((char*)pData[row].Data[eMdi_Channel]);
		WORD wItemIDX		= atoi((char*)pData[row].Data[eMdi_ItemIDX]);
		WORD wDropCount		= atoi((char*)pData[row].Data[eMdi_DropCount]);
		WORD wMaxDropCount	= atoi((char*)pData[row].Data[eMdi_MaxDropCount]);

		MAPITEMDROP_OBJ->SetItemDropInfoByDB( wMapNum, wChannel, wItemIDX, wDropCount, wMaxDropCount );
	}
}


// 夸货傈
void FortWarInfoLoad()
{
	sprintf(txt, "EXEC dbo.MP_FORTWARINFO_LOAD" );
	g_DB.Query(eQueryType_FreeQuery, eFortWarInfoLoad, 0, txt);
}

void RFortWarInfoLoad( LPQUERY pData, LPDBMESSAGE pMessage )
{
//	if( 0 == pMessage->dwResult )
//		return;

	sFortWarInfo info;

	for( DWORD i = 0; i < pMessage->dwResult; ++i )
	{
		info.nRelationFortWarIDX = atoi((char*)pData[i].Data[0]);
		info.dwCharacterIdx = (DWORD)atoi((char*)pData[i].Data[1]);
		SafeStrCpy( info.CharacterName, (char*)pData[i].Data[2], MAX_NAME_LENGTH+1 );
		info.dwGuildIdx = (DWORD)atoi((char*)pData[i].Data[3]);
		info.dwProfitMoney = (DWORD)_atoi64((char*)pData[i].Data[4]);

		FORTWARMGR->SetTotalFortWarInfoFromDB( info );
	}
	FORTWARMGR->CompleteLoadDataFromDB();
}

void FortWarInfoUpdate( int nWarIDX, DWORD dwCharacterIdx, char* pCharacterName, DWORD dwGuildIdx )
{
    // 2014-06-25 check string param !

	if( !isValidQueryString(pCharacterName))
	{
	   return;
	}

	if( CheckString(pCharacterName))
	{
		return;
	}
	sprintf(txt, "EXEC dbo.MP_FORTWARINFO_UPDATE %d, %d, \'%s\', %d", nWarIDX, dwCharacterIdx, pCharacterName, dwGuildIdx );
	g_DB.Query(eQueryType_FreeQuery, eFortWarInfoUpdate, dwCharacterIdx, txt);
}

void FortWarProfitMoneyUpdate( int nWarIDX, DWORD dwMoney, int nType )
{
	sprintf( txt, "EXEC dbo.MP_FORTWARINFO_PROFITMONEY_UPDATE %d, %u, %d", nWarIDX, dwMoney, nType );
	g_DB.Query( eQueryType_FreeQuery, eFortWarProfitMoneyUpdate, 0, txt );
}

void RFortWarProfitMoneyUpdate( LPQUERY pData, LPDBMESSAGE pMessage )
{
	if( !pMessage->dwResult ) return;
	int WarIdx = _atoi64((char*)pData[0].Data[0]);
	DWORD Money = _atoi64((char*)pData[0].Data[1]);
	FORTWARMGR->SetFortWarProfitMoneyFromDB( WarIdx,Money);

	//FORTWARMGR->SetFortWarProfitMoneyFromDB( atoi((char*)pData[0].Data[0]), (DWORD)atoi((char*)pData[0].Data[1]) );
}

void FortWarItemLoad( int nWarIDX )
{
	sprintf(txt, "EXEC dbo.MP_FORTWAR_ITEM_LOAD %d", nWarIDX );
	g_DB.Query(eQueryType_FreeQuery, eFortWarItemLoad, nWarIDX, txt);
}

void RFortWarItemLoad( LPQUERY pData, LPDBMESSAGE pMessage )
{
	if( pMessage->dwResult == 0 )
		return;

	int nWarIDX = atoi((char*)pData[0].Data[0]);

	for( int i = 0; i < pMessage->dwResult; ++i )
	{
		ITEMBASE itembase;
		memset( &itembase, 0, sizeof(itembase) );
		itembase.dwDBIdx = (DWORD)atoi((char*)pData[i].Data[1]);
		itembase.wIconIdx = (WORD)atoi((char*)pData[i].Data[2]);
		itembase.Position = (WORD)atoi((char*)pData[i].Data[3]);
		itembase.Durability = (DWORD)atoi((char*)pData[i].Data[4]);
		itembase.RareIdx = (DWORD)atoi((char*)pData[i].Data[5]);

		FORTWARMGR->SetTotalFortWarItemInfoFromDB( nWarIDX, &itembase );
	}
}

void FortWarItemInsertToDB( int nWarIDX, DWORD dwItemIdx, DURTYPE Durability, POSTYPE wPos, DWORD dwParam )
{
	sprintf( txt, "EXEC dbo.MP_FORTWAR_ITEM_INSERT %d, %d, %d, %d, %d", nWarIDX, dwItemIdx, Durability, wPos, dwParam );
	g_DB.Query(eQueryType_FreeQuery, eFortWarItemInsertToDB, nWarIDX, txt);
}

void RFortWarItemInsertToDB( LPQUERY pData, LPDBMESSAGE pMessage )
{
	if( pMessage->dwResult == 0 )
		return;

	int nWarIDX = (DWORD)atoi((char*)pData->Data[0]);
	ITEMBASE itembase;
	memset( &itembase, 0, sizeof(itembase) );
	itembase.dwDBIdx = (DWORD)atoi((char*)pData->Data[1]);
	itembase.wIconIdx = (WORD)atoi((char*)pData->Data[2]);
	itembase.Position = (WORD)atoi((char*)pData->Data[3]);
	itembase.Durability = (DWORD)atoi((char*)pData->Data[4]);
	itembase.ItemParam = (DWORD)atoi((char*)pData->Data[5]);

	FORTWARMGR->AddProfitItemFromDBResult( nWarIDX, &itembase );
}

void FortWarItemMoveToDB( DWORD dwCharacterIdx, DWORD dwItemDBIdx, WORD wPos, int nWarIDX )
{
	sprintf( txt, "EXEC dbo.MP_FORTWAR_ITEM_MOVE %u, %u, %d, %d", dwCharacterIdx, dwItemDBIdx, wPos, nWarIDX );
	g_DB.Query(eQueryType_FreeQuery, eFortWarItemMoveToDB, dwCharacterIdx, txt);
}


//weiye 结婚数据库执行函数定义  2017-08-28

void  MarryAdd(DWORD dwCharacterIdx, char * pTargetName, DWORD dwTargetIdx, char * pCharacterName)
{

    sprintf(txt, "EXEC dbo.MP_CHARACTER_MarryAdd %d, %d,\'%s\',\'%s\'",dwCharacterIdx,dwTargetIdx,pCharacterName, pTargetName);
	g_DB.Query(eQueryType_FreeQuery,eMarryAdd,0, txt);
		
}

//weiye  2017-08-29  结婚数据库回调函数

void RMarryAdd(LPQUERY pData, LPDBMESSAGE pMessage)
{
	if( pMessage->dwResult )
	{

		char InviterName[MAX_NAME_LENGTH+1];
		char TargetName[MAX_NAME_LENGTH+1];
		DWORD InviterIdx= atoi((char*)pData->Data[0]);
		SafeStrCpy(TargetName,(char*)pData->Data[1],MAX_NAME_LENGTH+1);
		DWORD TargetIdx=atoi((char*)pData->Data[2]);
		SafeStrCpy(InviterName,(char*)pData->Data[3],MAX_NAME_LENGTH+1);

		
		CPlayer* pInviter = (CPlayer*)g_pUserTable->FindUser(InviterIdx);

        CPlayer* pTarget = (CPlayer*)g_pUserTable->FindUser(TargetIdx);

		if(pInviter == NULL || pTarget==NULL)
		{
			return;
		}
		if(pInviter->GetInited() == FALSE || pTarget->GetInited()==FALSE)

		{
			return;
		}

		pTarget->SetMarryName(pInviter->GetObjectName());

		pInviter->SetMarryName(pTarget->GetObjectName());

		pInviter->SetMoney(CServerSystem::MarryMoney, MONEY_SUBTRACTION, 0, eItemTable_Inventory, eMoneyLog_LoseMarry, 0 );

		pInviter->SetGoldMoney(CServerSystem::MarryGold,2);

		LogGoldMoney(eLog_GoldMoneyMarryGold,pInviter->GetID(),(pInviter->GetGoldMoney()+CServerSystem::MarryGold),65533,pInviter->GetGoldMoney(),CServerSystem::MarryGold,0);

		MSG_MARRY_ADDMSG msg;
		SetProtocol(&msg, MP_MARRY, MP_MARRY_ADD_ACK);
        memset(msg.Name, 0, MAX_NAME_LENGTH+1);
		SafeStrCpy(msg.Name, pInviter->GetObjectName(), MAX_NAME_LENGTH+1);
		pTarget->SendMsg(&msg, sizeof(msg));

		MSG_MARRY_ADDMSG msg2;	
		SetProtocol(&msg2, MP_MARRY, MP_MARRY_ADD_ACK);
		memset(msg2.Name, 0, MAX_NAME_LENGTH+1);
		SafeStrCpy(msg2.Name, pTarget->GetObjectName(), MAX_NAME_LENGTH+1);
		pInviter->SendMsg(&msg2, sizeof(msg2));


		char Msg[1024] = {0,};
		WORD MsgLen = 0;

		pInviter->SetAddMsg(Msg,&MsgLen,0,TRUE);
		if( MsgLen )
			PACKEDDATA_OBJ->QuickSendExceptObjectSelf( pInviter, (MSGBASE*)Msg, MsgLen );
			
		ZeroMemory(Msg, 1024);
		pTarget->SetAddMsg(Msg,&MsgLen,0,TRUE);
		if( MsgLen )
			PACKEDDATA_OBJ->QuickSendExceptObjectSelf( pTarget, (MSGBASE*)Msg, MsgLen );


		MSG_CHAT_WORD msg1;
		msg1.Category	= MP_MARRY;
		msg1.Protocol	= MP_MARRY_NOTICE;
		msg1.dwObjectID	= pInviter->GetID();
#ifdef _MAPBIG5_
        sprintf(msg1.Msg, "[挡盉]跜纏續ぃ跜蒓及稲ぇ拟も打, 褐[%s]蒓[%s]痷み稲ッ禤穝盉ぇ尺", pInviter->GetObjectName(), pTarget->GetObjectName());
#else        
		sprintf(msg1.Msg, "[结婚]只羡鸳鸯不羡仙，与挚爱之人携手江湖, 祝福[%s]与[%s]真心相爱，永贺新婚之喜！", pInviter->GetObjectName(), pTarget->GetObjectName());
#endif			
		pInviter->SendMsg(&msg1, msg1.GetMsgLength());

	}
	else
	{
		ASSERT(0);
	}
}


// marry DEL 2014-05-10
void MarryDel(DWORD dwCharacterIdx, char* Name)
{
	if( !isValidQueryString( Name ) )	return;
	if( CheckString( Name ) )	return;

	sprintf( txt, "EXEC dbo.MP_CHARACTER_Marry_Del %u, %s", dwCharacterIdx, Name);
	g_DB.Query(eQueryType_FreeQuery, eMarryDel, dwCharacterIdx, txt);
}

void RMarryDel(LPQUERY pData, LPDBMESSAGE pMessage)
{
	//weiye 修改离婚系统回调处理 2018-04-29

	if( pMessage->dwResult)
	{
			CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser( pMessage->dwID );
			CPlayer* pTargetPlayer = (CPlayer*)g_pUserTable->FindUser(atoi((char*)pData->Data[eMarryDel_vtTargetid]));
			if( !pPlayer )	return;

			//weiye 扣除主张离婚者元宝，并还原未婚状态 2018-04-29

			pPlayer->SetGoldMoney(CServerSystem::MarryGold,2);
			pPlayer->SetMarryName("0");
			MSG_DWORD msg;
			msg.Category = MP_MARRY;
			msg.Protocol = MP_MARRY_DEL_ACK;
			msg.dwData = atoi((char*)pData->Data[eMarryDel_vtTargetid]);
			msg.dwObjectID=pPlayer->GetID();
			pPlayer->SendMsg(&msg, sizeof(msg));

			if(pTargetPlayer)  //weiye 如果在同一个地图，则直接发消息告知并还原状态 2018-04-29
			{
				pTargetPlayer->SetMarryName("0");
				MSG_DWORD msg;
				msg.Category = MP_MARRY;
				msg.Protocol = MP_MARRY_DEL_ACK;
				msg.dwData = atoi((char*)pData->Data[eMarryDel_vtFromid]);
				msg.dwObjectID=pTargetPlayer->GetID();
				pTargetPlayer->SendMsg(&msg, sizeof(msg));
			}
			else  //weiye   离婚如果不在同一个地图，通过AGENT转发 2018-04-29
			{
				MSG_DWORD msg;
				msg.Category=MP_MARRY;
				msg.Protocol =MP_MARRY_DEL_ACK_AGENT;
				msg.dwData = atoi((char*)pData->Data[eMarryDel_vtFromid]);
				msg.dwObjectID=atoi((char*)pData->Data[eMarryDel_vtTargetid]);
 				g_Network.Send2AgentServer((char*)&msg, sizeof(msg));
			}
	}
}


void MallListInfo(DWORD dwCharacterIdx, DWORD Type)
{
	//g_Console.LOG(4,"MallListInfo");
	sprintf(txt, "EXEC %s %d", "dbo.JACK_MALLLIST", Type );
	g_DB.Query(eQueryType_FreeQuery, eMallListInfoQuery,dwCharacterIdx, txt);	
}

void RMallListInfo( LPQUERY pData, LPDBMESSAGE pMessage )
{
	//g_Console.LOG(4,"RMallListInfo");
	CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser( pMessage->dwID );
	if (!pPlayer)
		return;

	if (pMessage->dwResult == 0)
		return;
	
	SEND_MALLLIST msg;
	//g_Console.LOG(4,"SEND_MALLLIST");
	SetProtocol(&msg, MP_MALLLIST, MP_MALLLIST_ACK);
	msg.nCharIdx = (WORD)atoi((char*)pData[0].Data[0]);
	for(DWORD i=0; i<pMessage->dwResult; ++i)
	{
		//g_Console.LOG(4,"pMessage->dwResult");
		
		msg.m_Info[i].nData1 = (DWORD)atoi((char*)pData[i].Data[1]);
		msg.m_Info[i].nData2 = (DWORD)atoi((char*)pData[i].Data[2]);
		msg.m_Info[i].nData3 = (DWORD)atoi((char*)pData[i].Data[3]);

		g_Console.LOG(4,"msg.m_Info[i].nData1 %d",msg.m_Info[i].nData1);
		msg.wCount++;
	}
	pPlayer->SendMsg(&msg, msg.GetSize());
}
// 2014-12-07 函数

void RItemLockUpdate(LPQUERY pData, LPDBMESSAGE pMessage)
{
    WORD lastNo = LOWORD(pMessage->dwID);
	WORD ArrayID = HIWORD(pMessage->dwID);

	if( pMessage->dwResult )
	{

		DWORD dwObjectID = atoi((char*)pData->Data[eCI_ObjectID]);
		DWORD dwItemIdx = atoi((char*)pData->Data[eCI_IDX]);
		DWORD dwDura = atoi((char*)pData->Data[eCI_Durability]);
		DWORD dwPos = atoi((char*)pData->Data[eCI_Position]);
		DWORD dwQPos = atoi((char*)pData->Data[eCI_QPosition]);
		DWORD dwDBIdx = atoi((char*)pData->Data[eCI_DBIDX]);
		DWORD dwParam = atoi((char*)pData->Data[eCI_Param]);
		DWORD dwStoneIdx=atoi((char*)pData->Data[eCI_StoneIdx]);
		DWORD dwStatic= atoi((char*)pData->Data[eCI_Static]);   // 2014-12-06  
		DWORD dwGrow=   atoi((char*)pData->Data[eCI_Grow]);     // 2015-01-14  成长
		DWORD dwGrade30 = atoi((char*)pData->Data[eCI_Grade30]);
		CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser(atoi((char*)pData->Data[eCI_ObjectID]));
		if(pPlayer == NULL)
			return;
		if(pPlayer->GetInited() == FALSE)
			return;
		CItemSlot * pTargetSlot = pPlayer->GetSlot(dwPos);

		if( pTargetSlot->UpdateItemAbs(pPlayer, dwPos, 0, 0, 0, 0, 0 , UB_LOCK ,SS_NONE,0) != EI_TRUE )
		{
		//	ASSERTMSG(0, "The ItemUpdateLock UpdateItemAbs is Error!");
			return ;
		}

		ITEMMGR->ItemLockDBResult(pPlayer,dwDBIdx, dwPos);  // 完发送消息到客户端 2014-12-06

	}
	else
	{
		ASSERT(0);
	}
}

// 2014-12-07 解锁函数

void RItemUnLockUpdate(LPQUERY pData, LPDBMESSAGE pMessage)
{
    WORD lastNo = LOWORD(pMessage->dwID);
	WORD ArrayID = HIWORD(pMessage->dwID);

	if( pMessage->dwResult )
	{

		DWORD dwObjectID = atoi((char*)pData->Data[eCI_ObjectID]);
		DWORD dwItemIdx = atoi((char*)pData->Data[eCI_IDX]);
		DWORD dwDura = atoi((char*)pData->Data[eCI_Durability]);
		DWORD dwPos = atoi((char*)pData->Data[eCI_Position]);
		DWORD dwQPos = atoi((char*)pData->Data[eCI_QPosition]);
		DWORD dwDBIdx = atoi((char*)pData->Data[eCI_DBIDX]);
		DWORD dwParam = atoi((char*)pData->Data[eCI_Param]);
		DWORD dwStone = atoi((char*)pData->Data[eCI_StoneIdx]);
		DWORD dwStatic= atoi((char*)pData->Data[eCI_Static]);   // 2014-12-06  解锁
		DWORD dwGrow=   atoi((char*)pData->Data[eCI_Grow]);     // 2015-01-14  成长
		DWORD dwGrade30 = atoi((char*)pData->Data[eCI_Grade30]);
		CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser(atoi((char*)pData->Data[eCI_ObjectID]));
		if(pPlayer == NULL)
			return;
		if(pPlayer->GetInited() == FALSE)
			return;
		CItemSlot * pTargetSlot = pPlayer->GetSlot(dwPos);

		if( pTargetSlot->UpdateItemAbs(pPlayer, dwPos, 0, 0, 0, 0, 0 , UB_UNLOCK ,SS_NONE,0) != EI_TRUE )
		{
		//	ASSERTMSG(0, "The ItemUpdateUnLock UpdateItemAbs is Error!");
			return ;
		}

		ITEMMGR->ItemUnLockDBResult(pPlayer,dwDBIdx, dwPos);  // 完发送消息到客户端 2014-12-06

	}
	else
	{
		ASSERT(0);
	}
}


void RItemGrowUpdate(LPQUERY pData, LPDBMESSAGE pMessage)
{
    WORD lastNo = LOWORD(pMessage->dwID);
	WORD ArrayID = HIWORD(pMessage->dwID);

	if( pMessage->dwResult )
	{

		DWORD dwObjectID = atoi((char*)pData->Data[eCI_ObjectID]);
		DWORD dwItemIdx = atoi((char*)pData->Data[eCI_IDX]);
		DWORD dwDura = atoi((char*)pData->Data[eCI_Durability]);
		DWORD dwPos = atoi((char*)pData->Data[eCI_Position]);
		DWORD dwQPos = atoi((char*)pData->Data[eCI_QPosition]);
		DWORD dwDBIdx = atoi((char*)pData->Data[eCI_DBIDX]);
		DWORD dwParam = atoi((char*)pData->Data[eCI_Param]);
		DWORD dwStoneIdx=atoi((char*)pData->Data[eCI_StoneIdx]);
		DWORD dwStatic= atoi((char*)pData->Data[eCI_Static]);   // 2014-12-06  
		DWORD dwGrow=   atoi((char*)pData->Data[eCI_Grow]);     // 2015-01-14  成长
		DWORD dwGrade30 = atoi((char*)pData->Data[eCI_Grade30]);
		CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser(atoi((char*)pData->Data[eCI_ObjectID]));
		if(pPlayer == NULL)
			return;
		if(pPlayer->GetInited() == FALSE)
			return;
		CItemSlot * pTargetSlot = pPlayer->GetSlot(dwPos);

		if( pTargetSlot->UpdateItemAbs(pPlayer, dwPos, 0, 0, 0, 0, 0 , UB_GROW ,SS_NONE,0,0,dwGrow) != EI_TRUE )
		{
		//	ASSERTMSG(0, "The ItemUpdateGrow UpdateItemAbs is Error!");
			return ;
		}

		STATSMGR->CalcItemStats(pPlayer);  // 刷新附加

		ITEMMGR->GrowItemDBResult(pPlayer,dwDBIdx,dwPos,dwGrow);   // 完发送成长消息到客户端 2014-12-06

	}
	else
	{
		ASSERT(0);
	}
}

// 2015-01-28 赌博系统数据库回调函数实现

void RGambleUserInfoInsert(LPQUERY pData, LPDBMESSAGE pMessage)
{
	if( pMessage->dwResult )
	{
		int InsertFlag= atoi((char*)pData->Data[eGamble_InsertFlag]);

		CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser(atoi((char*)pData->Data[eGamble_UserCharIdx]));

		if(InsertFlag==1)
		{

			GAMBLE_USER_INFO * pGambleUserInfo;

			pGambleUserInfo= new GAMBLE_USER_INFO;

			pGambleUserInfo->CharacterIdx =  atoi((char*)pData->Data[eGamble_UserCharIdx]); 

			pGambleUserInfo->Gamble_Year  =  atoi((char*)pData->Data[eGamble_UserYear]);

			pGambleUserInfo->Gamble_Month =  atoi((char*)pData->Data[eGamble_UserMonth]);

			pGambleUserInfo->Gamble_Day   =  atoi((char*)pData->Data[eGamble_UserDay]);

			pGambleUserInfo->Number1      =  atoi((char*)pData->Data[eGamble_UserNum1]);

			pGambleUserInfo->Number2      =  atoi((char*)pData->Data[eGamble_UserNum2]);

			pGambleUserInfo->Number3      =  atoi((char*)pData->Data[eGamble_UserNum3]);

			if(pPlayer == NULL)
			{
				return;
			}

			pPlayer->SetGambleInfo(pGambleUserInfo);

			GAMBLEMGR->SendGambleUserInfoSucceedMsg(pPlayer,pGambleUserInfo->Number1,pGambleUserInfo->Number2,pGambleUserInfo->Number3);

			if(pPlayer->GetGoldMoney()>=250)
			{
			   pPlayer->SetGoldMoney(250,2,0); // 2015-03-08 默认消耗250!
			}

			pGambleUserInfo=NULL;
		}

		if(InsertFlag==0)
		{
			if(pPlayer == NULL)
			{
				return;
			}

            GAMBLEMGR->SendGambleUserInfoErrorMsg(pPlayer,1);  // 返回投注消息!
		}
		
		if(InsertFlag==2)
		{
			if(pPlayer == NULL)
			{
				return;
			}
            GAMBLEMGR->SendGambleUserInfoErrorMsg(pPlayer,3);  // 返回开奖,不允许下注!
		}

	}
	else
	{
		
		CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser(atoi((char*)pData->Data[eCI_ObjectID]));

		if(pPlayer == NULL)
		{
			return;
		}

        GAMBLEMGR->SendGambleUserInfoErrorMsg(pPlayer,2);  // 数据库返回数据异常 返回错误为2
	}
}

void RCharacterGambleInfo(LPQUERY pData, LPDBMESSAGE pMessage)
{
   	if( pMessage->dwResult )
	{

		GAMBLE_USER_INFO * pGambleUserInfo;

		pGambleUserInfo= new GAMBLE_USER_INFO;

		pGambleUserInfo->CharacterIdx =  atoi((char*)pData->Data[eGamble_UserCharIdx]); 

		pGambleUserInfo->Gamble_Year  =  atoi((char*)pData->Data[eGamble_UserYear]);

		pGambleUserInfo->Gamble_Month =  atoi((char*)pData->Data[eGamble_UserMonth]);

		pGambleUserInfo->Gamble_Day   =  atoi((char*)pData->Data[eGamble_UserDay]);

		pGambleUserInfo->Number1      =  atoi((char*)pData->Data[eGamble_UserNum1]);

		pGambleUserInfo->Number2      =  atoi((char*)pData->Data[eGamble_UserNum2]);

		pGambleUserInfo->Number3      =  atoi((char*)pData->Data[eGamble_UserNum3]);


		CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser(atoi((char*)pData->Data[eGamble_UserCharIdx]));

		if(pPlayer == NULL)
		{
			return;
		}

		pPlayer->SetGambleInfo(pGambleUserInfo);

		GAMBLEMGR->SendGambleUserInfoMapMsg(pPlayer,pGambleUserInfo->Number1,pGambleUserInfo->Number2,pGambleUserInfo->Number3);

	    pGambleUserInfo=NULL;

	}
}

void RCharacterGamblePublicInfo(LPQUERY pData, LPDBMESSAGE pMessage)
{
   if(pMessage->dwResult)
   {
		CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser(atoi((char*)pData->Data[eGamble_CharIdx]));

		if(pPlayer == NULL)
		{
			return;
		}

		int Number1= atoi((char*)pData->Data[eGamble_Num1]); 

		int Number2= atoi((char*)pData->Data[eGamble_Num2]); 

		int Number3= atoi((char*)pData->Data[eGamble_Num3]); 

		GAMBLEMGR->SendGamblePublicInfoMsg(pPlayer,Number1,Number2,Number3);
   }
}

//weiye 2017-12-05 安全锁相关函数定义

void CheckSafePin(DWORD dwCharacterIdx, char * pSafePin)
{
	//weiye 数据库字符串安全检查, 若字符串为空 或者有数据库关键字 则停止执行 2016-03-08

	if(!isValidQueryString(pSafePin) || CheckString(pSafePin))
	{
        return ;
	}
    sprintf(txt, "EXEC %s %d,\'%s\'", STORED_SAFEPIN_CHECK,dwCharacterIdx,pSafePin);
	g_DB.Query(eQueryType_FreeQuery,eSafePinCheck,0, txt);
}

void RCheckSafePin(LPQUERY pData, LPDBMESSAGE pMessage)
{
	if( pMessage->dwResult )
	{	
		DWORD Char_Idx= atoi((char*)pData->Data[0]);
        int   Ret      =atoi((char*)pData->Data[1]);
		
		CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser(Char_Idx);

		if(pPlayer == NULL)
		{
			return;
		}

		if(Ret==1)
		{
			pPlayer->SetSafeStatic(TRUE);
		}
		else
		{
			MSG_WORD msg;
			SetProtocol(&msg, MP_ITEM,MP_ITEM_SAFE_CHECK_NACK);
			msg.dwObjectID=pPlayer->GetID();
			msg.wData=0;
			pPlayer->SendMsg(&msg, sizeof(msg));
		}

        return;
	}
	else
	{
		ASSERT(0);
	}
}

void ChangeSafePin(DWORD dwCharacterIdx, char * pSafePin,char * pNSafePin,WORD Type)
{
	if(!isValidQueryString(pSafePin) || CheckString(pSafePin) || !isValidQueryString(pNSafePin) || CheckString(pNSafePin))
	{
        return ;
	}
	sprintf(txt, "EXEC %s %d,\'%s\',\'%s\',%d", STORED_SAFEPIN_CHANGE,dwCharacterIdx,pSafePin,pNSafePin,Type);

	g_DB.Query(eQueryType_FreeQuery,eSafePinChange,0, txt);

	g_Console.LOC(0xFFFF66,4, "%s %d,\'%s\',\'%s\',%d",STORED_SAFEPIN_CHANGE,dwCharacterIdx,pSafePin,pNSafePin,Type);
}

void RChangeSafePin(LPQUERY pData, LPDBMESSAGE pMessage)
{
	if( pMessage->dwResult )
	{
		char SafeCode[16];
		DWORD Char_Idx= atoi((char*)pData->Data[0]);
        WORD   Ret     = atoi((char*)pData->Data[1]);
		SafeStrCpy(SafeCode,(char*)pData->Data[2],5);
		
		CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser(Char_Idx);

		if(pPlayer == NULL)
		{
			return;
		}
	    if(Ret==1)
		{
			pPlayer->SetSafeStaticValue(FALSE);
			MSG_WORD msg;
			SetProtocol(&msg, MP_ITEM,MP_ITEM_SAFE_CHANGE_ACK);
			msg.dwObjectID=pPlayer->GetID();
			msg.wData=1;
			pPlayer->SendMsg(&msg, sizeof(msg));
			//g_Console.LOC(0xFFFF66,4, "ACK");
		}
		else
		{
			MSG_WORD msg;
			SetProtocol(&msg, MP_ITEM,MP_ITEM_SAFE_CHANGE_NACK);
			msg.dwObjectID=pPlayer->GetID();
			msg.wData=Ret;
			pPlayer->SendMsg(&msg, sizeof(msg));
			//g_Console.LOC(0xFFFF66,4, "NACK");
		}

		return ;

	}
	else
	{
		ASSERT(0);
	}
}

void UpdateSafeStatic(DWORD dwCharacterIdx,int Flag)
{
	sprintf(txt, "EXEC %s %d,%d", STORED_SAFEPIN_UPDATE,dwCharacterIdx,Flag);
	g_DB.Query(eQueryType_FreeQuery,eSafeUpdate,0, txt);
}
void RUpdateSafeStatic(LPQUERY pData, LPDBMESSAGE pMessage)
{
   	if( pMessage->dwResult )
	{
		DWORD Char_Idx= atoi((char*)pData->Data[0]);
        int   Ret     = atoi((char*)pData->Data[1]);		
		CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser(Char_Idx);
		if(pPlayer == NULL)
		{
			return;
		}
		pPlayer->SetSafeStatic(Ret);
	}
	else
	{
          ASSERT(0);
	}
}

void UpdateLastMapInfo(DWORD dwCharacterIdx,int Flag)
{
	if(Flag == 555)
		sprintf(txt, "EXEC %s %d,%d,%d", STORED_LASTMAP_UPDATE,dwCharacterIdx,Flag,GAMERESRCMNGR->GetLoadMapNum());
	else
		sprintf(txt, "EXEC %s %d,%d,%d", STORED_LASTMAP_UPDATE,dwCharacterIdx,Flag,0);

	g_DB.Query(eQueryType_FreeQuery,eLastMapDC,0, txt);
}
void RUpdateLastMapInfo(LPQUERY pData, LPDBMESSAGE pMessage)
{
   /*	if( pMessage->dwResult )
	{
		DWORD Char_Idx= atoi((char*)pData->Data[0]);
       int   Ret     = atoi((char*)pData->Data[1]);		
		CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser(Char_Idx);
		if(pPlayer == NULL)
		{
			return;
		}
		pPlayer->SetSafeStatic(Ret);
	}
	else
	{
          ASSERT(0);
	}*/
}

void FBTimeUpdate(DWORD dwCharacterIdx,DWORD fbTime)
{
	sprintf(txt, "EXEC %s %d, %d", STORED_FBTIME_UPDATE, dwCharacterIdx,fbTime);
	g_DB.Query(eQueryType_FreeQuery,eFbTimeUpdate,0, txt);
}
void RFBTimeUpdate(LPQUERY pData, LPDBMESSAGE pMessage)
{
	if( pMessage->dwResult )
	{
		DWORD User_Idx= atoi((char*)pData->Data[0]);
		DWORD Char_Idx= atoi((char*)pData->Data[1]);
		DWORD FBTime =  atoi((char*)pData->Data[2]);
		
		CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser(Char_Idx);
		if(pPlayer == NULL)
			return;
		if(pPlayer->GetInited() == FALSE)
			return;

		pPlayer->SetFBTimeTo(FBTime);
	}
	else
	{
		ASSERT(0);
	}
}
void ItemLinkSelectItemInfo(DWORD ItemDBIdx,DWORD SelectIdx)
{//ITEMLINK
	//read data from db
	sprintf(txt, "EXEC %s %d,%d", STORED_ITEM_SELECT,ItemDBIdx,SelectIdx);
	g_DB.Query(eQueryType_FreeQuery,eItemSelect,0,txt);
}
void RItemLinkSelectItemInfo(LPQUERY pData, LPDBMESSAGE pMessage)
{//ITEMLINK
	//DWORD eErrorCode/*=0*/;
	//g_Console.LOG(4,"atoi((char*)pData->Data[0]) %d",atoi((char*)pData->Data[0]));
	if(atoi((char*)pData->Data[0]) == 0)//when player who send doesnt existed return to NACK
	{
		CPlayer * pSelect = (CPlayer *)g_pUserTable->FindUser(atoi((char*)pData->Data[1]));
		if(pSelect==NULL)
		{
			return;
		}
		else
		{
			//eErrorCode=1;

			//g_Console.LOG(4,"SEND USER OFFLINE");
			MSG_DWORD		msg;
			msg.Category	= MP_ITEMEXT;
			msg.Protocol	= MP_ITEMEXT_SELECT_NACK;
			msg.dwData		= pSelect->GetID();
//			msg.Code		= eErrorCode;
			pSelect->SendMsg(&msg,sizeof(msg));

			/*MSG_DWORD3		msg;
			msg.Category	= MP_ITEMEXT;
			msg.Protocol	= MP_ITEMEXT_SELECT_NACK;
			msg.dwObjectID	= pSelect->GetID();
			msg.dwData1		= eErrorCode;
			pSelect->SendMsg(&msg,sizeof(msg));*/
			return;
		}
	}
	else//if something existed
	{
	 	CPlayer * pPlayer = (CPlayer *)g_pUserTable->FindUser(atoi((char*)pData->Data[0]));	//pPlayer yang send ke chat
		CPlayer * pSelect = (CPlayer *)g_pUserTable->FindUser(atoi((char*)pData->Data[3]));	//pSelect yang nak tengok item
		POSTYPE   pos     =  atoi((char*)pData->Data[2]);									//item position

		if(pSelect==NULL)//player yg NAK TGK item tak wujud
		{
			return;
		}
		//itemlink success! player sama map
		if(pPlayer!=NULL /*&& pSelect!=NULL*/)//data player yg SEND item wujud dan player tu sendiri berada dimap yg SAMA
		{
			ITEM_RARE_OPTION_INFO  pRateInfo;
			ITEM_OPTION_INFO  pOptionInfo;
			const ITEMBASE * pItemBase = ITEMMGR->GetItemInfoAbsIn(pPlayer,pos);
			if(pItemBase)
			{
				memcpy(&pRateInfo,pPlayer->GetItemRareOption(pItemBase->RareIdx),sizeof(ITEM_RARE_OPTION_INFO));
				memcpy(&pOptionInfo,pPlayer->GetItemOption(pItemBase->Durability),sizeof(ITEM_OPTION_INFO));
				MSG_ITEM_SELECTINFO msg;
				msg.Category	= MP_ITEMEXT;
				msg.Protocol	= MP_ITEMEXT_SELECT_ACK;
				msg.SelectIdx	= pSelect->GetID();

				memcpy(&msg.WearedItemRateInfo,&pRateInfo,sizeof(ITEM_RARE_OPTION_INFO));
				memcpy(&msg.WearedItemOptionInfo,&pOptionInfo,sizeof(ITEM_OPTION_INFO));
				memcpy(&msg.WearedItem,pItemBase, sizeof(ITEMBASE));
				pSelect->SendMsg(&msg, sizeof(msg));
				return;
			}
		}
#ifdef _ITEMLINK_
		else//if(pPlayer==NULL && pSelect!=NULL)
		{	//data player yg SEND item tak wujud atau player tu sendiri berada di map lain
			//data akan dihantar ke Agent utk mencari player kot berada di map lain
			//ni adalah FIRST agent process

			//g_Console.LOG(4,"SEND USER OFFLINE");

			MSG_DWORD2		msg;
			msg.Category	= MP_ITEMEXT;
			msg.Protocol	= MP_ITEMEXT_SELECT_AGENT_SYN;
			msg.dwData1		= atoi((char*)pData->Data[0]);
			msg.dwData2		= pos ;
			msg.dwObjectID	= pSelect->GetID();
			g_Network.Send2AgentServer((char*)&msg,sizeof(msg));
			return;			
		}
#else
		return;
#endif
	}
}
void TidyItem(DWORD dwCharacterIdx,DWORD MaxStack)
{
    sprintf(txt, "EXEC %s %d,%d", STORED_ITEM_TIDY,dwCharacterIdx,MaxStack);
	g_DB.Query(eQueryType_FreeQuery,eItemTidy,0, txt);
}
void RTidyItem(LPQUERY pData, LPDBMESSAGE pMessage)
{
   	if( pMessage->dwResult )
	{
		DWORD Char_Idx= atoi((char*)pData->Data[0]);
		int   Flag    = atoi((char*)pData->Data[1]);
		CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser(Char_Idx);
		if(!pPlayer) return;
		if(Flag==1)
		{
			LoadTidyItemInfo(pPlayer->GetID(), MP_USERCONN_GAMEIN_SYN);
		}
	}
	else
	{
          ASSERT(0);
	}
}
void LoadTidyItemInfo(DWORD CharacterIDX, DWORD Protocol)
{
	//sprintf(txt, "EXEC %s %d", STORED_CHARACTER_ITEMINFO, CharacterIDX);
	//g_DB.Query(eQueryType_FreeQuery, eItemTidyLoad, Protocol, txt);

	sprintf(txt, "EXEC %s %d", "JACK_CHARACTER_ItemInfo_Tidy", CharacterIDX);
	g_DB.Query(eQueryType_FreeQuery, eItemTidyLoad, 0, txt);
}
void RLoadTidyItemInfo(LPQUERY pData, LPDBMESSAGE pMessage)
{
	CPlayer* pPlayer;


	DWORD count = pMessage->dwResult;
	ITEM_TOTALINFO Iteminfo;
	int overlapcount = 0;
	const int maxoverlap = MAX_YOUNGYAKITEM_DUPNUM;///100
	ITEMBASE OverLapItem[maxoverlap];
	memset(&OverLapItem, 0, sizeof(ITEMBASE)*maxoverlap);
	memset(&Iteminfo, 0, sizeof(ITEM_TOTALINFO));

	if((count == 1) && (atoi((char*)pData->Data[0]) == 0))
	{

		pPlayer = (CPlayer *)g_pUserTable->FindUser(atoi((char*)pData->Data[1]));
		if(pPlayer == NULL)
			return;
		//pPlayer->SetItemTotalInfo(OverLapItem);
		pPlayer->InitItemTotalInfo(&Iteminfo);


	//	pPlayer = (CPlayer *)g_pUserTable->FindUser(atoi((char*)pData->Data[1]));
	//	if(pPlayer == NULL)
	//		return;
	//	ITEM_TOTALINFO Iteminfo;
	//	memset(&Iteminfo, 0, sizeof(ITEM_TOTALINFO));
	//	pPlayer->InitItemTotalInfo(&Iteminfo);
	//	ShopItemInvenInfo( pPlayer->GetID() );
	}
	else
	{
		pPlayer = (CPlayer *)g_pUserTable->FindUser((DWORD)atoi((char*)pData[0].Data[eCI_ObjectID]));
		if(pPlayer == NULL)
			return;
		
		for(DWORD  i = 0; i < count; i++)
		{
			POSTYPE ItemPos = atoi((char*)pData[i].Data[eCI_Position]);
			ITEMBASE* pItemBase = NULL;
			if(ItemPos >= TP_INVENTORY_START && ItemPos < TP_INVENTORY_END)
			{
				ItemPos -= TP_INVENTORY_START;
				pItemBase = &Iteminfo.Inventory[ItemPos];
			}
			else if(ItemPos >= TP_WEAR_START && ItemPos < TP_WEAR_END)
			{	
				POSTYPE ItemGrid = ItemPos - TP_WEAR_START;
				pItemBase = &Iteminfo.WearedItem[ItemGrid];
				if( pItemBase )
				{
					pItemBase->ItemParam = atoi((char*)pData[i].Data[eCI_Param]);
					if( pItemBase->ItemParam & ITEM_PARAM_SEAL )
					{
						ITEM_INFO* pItemInfo = ITEMMGR->GetItemInfo( pItemBase->wIconIdx );
						if( pItemInfo )
						{
							if( pItemInfo->ItemKind & eSHOP_ITEM )
							{
								pItemBase->dwDBIdx = atoi((char*)pData[i].Data[eCI_DBIDX]);
								ItemMoveUpdateToDB( pPlayer->GetID(), 0, 0, pItemBase->dwDBIdx, 240 );
								continue;
							}
							else
							{
								pItemBase->ItemParam &= ~ITEM_PARAM_SEAL;
							}
						}
					}
				}
			}
			if(pItemBase == NULL)
			{
				ASSERT(0);
				continue;
			}
			if(pItemBase->dwDBIdx != 0 && overlapcount < maxoverlap)
			{
				pItemBase = &OverLapItem[overlapcount];
				++overlapcount;
			}
			pItemBase->dwDBIdx = atoi((char*)pData[i].Data[eCI_DBIDX]);
			pItemBase->wIconIdx = atoi((char*)pData[i].Data[eCI_IDX]);
			pItemBase->Position = atoi((char*)pData[i].Data[eCI_Position]);
			pItemBase->Durability = atoi((char*)pData[i].Data[eCI_Durability]);
			pItemBase->RareIdx = atoi((char*)pData[i].Data[eCI_RareIdx]);
			pItemBase->QuickPosition = atoi((char*)pData[i].Data[eCI_QPosition]);
			pItemBase->ItemParam = atoi((char*)pData[i].Data[eCI_Param]);
			pItemBase->ItemStatic = atoi((char*)pData[i].Data[eCI_Static]);///
			pItemBase->ItemGrow  = atoi((char*)pData[i].Data[eCI_Grow]);
			strcpy(pItemBase->PowerUp,(char*)pData[i].Data[eCI_PowerUp]);
			strcpy(pItemBase->Green,(char*)pData[i].Data[eCI_Green]);

			//strcpy
			ITEM_INFO* pInfo = ITEMMGR->GetItemInfo(pItemBase->wIconIdx);
			if( pInfo && pInfo->ItemKind & eTITAN_EQUIPITEM )
			{
				TITAN_ENDURANCE_ITEMINFO* pEndurance = pPlayer->GetTitanManager()->GetTitanItemEnduranceInfo(pItemBase->dwDBIdx);
				if( !pEndurance )
				{
					pPlayer->GetTitanManager()->MakeNewEnduranceInfo(pPlayer, pItemBase, eExceptionInven);				
				}
			}

			//pPlayer->SetItemTotalInfo(OverLapItem);
			pPlayer->InitItemTotalInfo(&Iteminfo);
		}
		for(int n=0;n<overlapcount;++n)
		{
			int m;
			for(m=TP_INVENTORY_START;m<TP_INVENTORY_END;++m)
			{
				if(Iteminfo.Inventory[m].dwDBIdx == 0)
				{
					Iteminfo.Inventory[m] = OverLapItem[n];
					Iteminfo.Inventory[m].Position = m;
					break;
				}
			}
			if(m==TP_INVENTORY_END)
				break;
		}
		//pPlayer->SetItemTotalInfo(OverLapItem);
		pPlayer->InitItemTotalInfo(&Iteminfo);
		pPlayer->SendItemTidyInfo();
	}


	/*CPlayer* pPlayer;
	DWORD count = pMessage->dwResult;
	ITEMBASE OverLapItem[80];
	memset(&OverLapItem, 0, sizeof(ITEMBASE)*80);
	if((count == 1) && (atoi((char*)pData->Data[0]) == 0))
	{
		pPlayer = (CPlayer *)g_pUserTable->FindUser(atoi((char*)pData->Data[1]));
		if(pPlayer == NULL)
			return;
		pPlayer->SetItemTotalInfo(OverLapItem);
	}
	else
	{
		pPlayer = (CPlayer *)g_pUserTable->FindUser((DWORD)atoi((char*)pData[0].Data[eCI_ObjectID]));
		if(pPlayer == NULL)
			return;
		for(DWORD  i = 0; i < count; i++)
		{
			OverLapItem[i].dwDBIdx			= atoi((char*)pData[i].Data[eCI_DBIDX]);
			OverLapItem[i].wIconIdx			= atoi((char*)pData[i].Data[eCI_IDX]);
			OverLapItem[i].Position			= atoi((char*)pData[i].Data[eCI_Position]);
			OverLapItem[i].Durability		= atoi((char*)pData[i].Data[eCI_Durability]);
			OverLapItem[i].RareIdx			= atoi((char*)pData[i].Data[eCI_RareIdx]);
			OverLapItem[i].QuickPosition	= atoi((char*)pData[i].Data[eCI_QPosition]);
			OverLapItem[i].ItemParam		= atoi((char*)pData[i].Data[eCI_Param]);
			OverLapItem[i].StoneIdx			= atoi((char*)pData[i].Data[eCI_StoneIdx]);
			OverLapItem[i].ItemStatic		= atoi((char*)pData[i].Data[eCI_Static]);
			OverLapItem[i].ItemGrow			= atoi((char*)pData[i].Data[eCI_Grow]);
			strcpy(OverLapItem[i].PowerUp,(char*)pData[i].Data[eCI_PowerUp]);
			strcpy(OverLapItem[i].Green,(char*)pData[i].Data[eCI_Green]);
		}
		pPlayer->SetItemTotalInfo(OverLapItem);
		MSG_TIDY_ITEM msg;
		//msg.dwObjectID=GetID();
		msg.Category=MP_ITEMEXT;
		msg.Protocol=MP_ITEMEXT_TIDY_ACK;
		//memcpy(&msg.Inventory, &OverLapItem,sizeof(msg.Inventory));
		memcpy(&msg.Inventory,OverLapItem,SLOT_INVENTORY_NUM * sizeof(ITEMBASE));
		pPlayer->SendMsg(&msg,sizeof(msg));
		//pPlayer->SendItemTidyInfo();
	}*/
}
void CharacterHeroRebornUpdate(CPlayer* pPlayer)
{
	if( !pPlayer->GetInited() )	return;

	sprintf(txt, "EXEC  %s %d, %d, %d, %d",
		STORED_CHARACTER_REBORN,
		pPlayer->GetID(),
		pPlayer->GetTurnOne(),
		pPlayer->GetTurnTwo(),
		pPlayer->GetTurnThree());
	g_DB.Query(eQueryType_FreeQuery, eRebornInfoUpdate, 0, txt);
}

void InitGuildWarPlayerInfo(DWORD characterIdx,char* pName,char* UnionName,DWORD pKillCount,DWORD pDieCount)
{
	return;
	sprintf(txt, "EXEC MP_GUILD_WAR_INIT_PLAYER_INFO %d,\'%s\',\'%s\',%d,%d",characterIdx,pName,UnionName,pKillCount,pDieCount);
	g_DB.Query(eQueryType_FreeQuery, eNull, 0, txt);	
}

void InitGuildWarUnionInfo(DWORD GuildUnionIdx,char* UnionName,DWORD pKillCount,DWORD pDieCount)
{
	return;
	sprintf(txt, "EXEC MP_GUILD_WAR_INIT_UNION_INFO %d,\'%s\',%d,%d",GuildUnionIdx,UnionName,pKillCount,pDieCount);
	g_DB.Query(eQueryType_FreeQuery, eNull, 0, txt);	
}

void ClearGuildWarInfo()
{
	return;
	sprintf(txt, "EXEC MP_GUILD_WAR_CLEAR ");
	g_DB.Query(eQueryType_FreeQuery, eNull, 0, txt);	
}


void NPCCODE( DWORD CharacterIdx, char* Name, DWORD DBIdx )
{
    // 2014-06-25 check string param !

	if( !isValidQueryString(Name))
	{
	   return;
	}

	if( CheckString(Name))
	{
		return;
	} 

	sprintf(txt, "EXEC %s %d, \'%s\', %d", STORED_NPCCODE, CharacterIdx, Name, DBIdx );
	g_DB.Query(eQueryType_FreeQuery, eNPCCODE, CharacterIdx, txt);
}
void RNPCCODE( LPQUERY pData, LPDBMESSAGE pMessage )
{
	CPlayer * pPlayer = (CPlayer*)g_pUserTable->FindUser( pMessage->dwID );
	if(!pPlayer)		return;

	DWORD Result = (DWORD)atoi((char*)pData->Data[0]);
	DWORD DBIdx = 0;
	MSG_DWORD msg;
	msg.Category = MP_ITEM;

	switch( Result )
	{
	case 0:
		{
			DBIdx = (DWORD)atoi((char*)pData->Data[1]);
			msg.Protocol = MP_ITEM_NPCCODE_ACK;
			msg.dwData = Result;
		}
		break;
	default:
		msg.Protocol = MP_ITEM_NPCCODE_NACK;
		msg.dwData = Result;
		break;
	}

	pPlayer->SendMsg( &msg, sizeof(msg) );	

}




void TidyItemShop(DWORD dwCharacterIdx,DWORD MaxStack)
{
    sprintf(txt, "EXEC %s %d,%d", STORED_ITEM_TIDY_SHOP,dwCharacterIdx,MaxStack);
	g_DB.Query(eQueryType_FreeQuery,eItemTidyShop,0, txt);
}
void LoadTidyItemShopInfo(DWORD CharacterIDX, DWORD Protocol)
{
//	char txt[128];
	sprintf(txt, "EXEC %s %d", STORED_SHOPINVEN_INFO, CharacterIDX);
	g_DB.Query(eQueryType_FreeQuery, eItemTidyLoadShop, Protocol, txt);
}
void RTidyItemShop(LPQUERY pData, LPDBMESSAGE pMessage)
{
   	if( pMessage->dwResult )
	{
		DWORD Char_Idx= atoi((char*)pData->Data[0]);
		int   Flag    = atoi((char*)pData->Data[1]);
		CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser(Char_Idx);
		if(!pPlayer) return;
		if(Flag==1)
		{
			LoadTidyItemShopInfo(pPlayer->GetID(), MP_USERCONN_GAMEIN_SYN);//miningmso 重新载入角色物品 2018-03-25
		}
	}
	else
	{
          ASSERT(0);
	}
}

void RLoadTidyItemShopInfo(LPQUERY pData, LPDBMESSAGE pMessage)
{
	/*CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser(pMessage->dwID);
	if( !pPlayer ) return;
	ITEMBASE ItemBase[TABCELL_SHOPINVEN_NUM+40];
	ITEMBASE ItemOverlap[TABCELL_SHOPINVEN_NUM+40];
	memset( ItemBase, 0, sizeof(ITEMBASE)*(TABCELL_SHOPINVEN_NUM+40) );
	memset( ItemOverlap, 0, sizeof(ITEMBASE)*(TABCELL_SHOPINVEN_NUM+40) );
	int OverlapCount=0;
	if( pMessage->dwResult == 0 )
	{
		pPlayer->InitShopInvenInfo( ItemBase );
		PetWearItemInfo(pPlayer->GetID(), TP_PETWEAR_START, TP_PETWEAR_END);
		return;
	}
	for(DWORD  i = 0; i < pMessage->dwResult; i++)
	{
		POSTYPE ItemPos = atoi((char*)pData[i].Data[eCI_Position]);
		if(ItemPos >= TP_SHOPINVEN_START && ItemPos < TP_SHOPINVEN_END)
		{
			ItemPos -= TP_SHOPINVEN_START;
			if( ItemPos >= (TABCELL_SHOPINVEN_NUM+40) || ItemBase[ItemPos].dwDBIdx )
			{
				ItemOverlap[OverlapCount].dwDBIdx = (DWORD)atoi((char*)pData[i].Data[eCI_DBIDX]);
				ItemOverlap[OverlapCount].wIconIdx = (WORD)atoi((char*)pData[i].Data[eCI_IDX]);
				ItemOverlap[OverlapCount].Position = (POSTYPE)atoi((char*)pData[i].Data[eCI_Position]);
				ItemOverlap[OverlapCount].QuickPosition = (POSTYPE)atoi((char*)pData[i].Data[eCI_QPosition]);
				ItemOverlap[OverlapCount].Durability = (DURTYPE)atoi((char*)pData[i].Data[eCI_Durability]);
				ItemOverlap[OverlapCount].ItemParam = (ITEMPARAM)atoi((char*)pData[i].Data[eCI_Param]);
				ItemOverlap[OverlapCount].ItemStatic = (WORD)atoi((char*)pData[i].Data[eCI_Static]);
				ItemOverlap[OverlapCount].ItemGrow = (DWORD)atoi((char*)pData[i].Data[eCI_Grow]);
				++OverlapCount;
			}
			else
			{
				ItemBase[ItemPos].dwDBIdx = (DWORD)atoi((char*)pData[i].Data[eCI_DBIDX]);
				ItemBase[ItemPos].wIconIdx = (WORD)atoi((char*)pData[i].Data[eCI_IDX]);
				ItemBase[ItemPos].Position = (POSTYPE)atoi((char*)pData[i].Data[eCI_Position]);
				ItemBase[ItemPos].QuickPosition = (POSTYPE)atoi((char*)pData[i].Data[eCI_QPosition]);
				ItemBase[ItemPos].Durability = (DURTYPE)atoi((char*)pData[i].Data[eCI_Durability]);
				ItemBase[ItemPos].ItemParam = (ITEMPARAM)atoi((char*)pData[i].Data[eCI_Param]);
				ItemBase[ItemPos].ItemStatic = (WORD)atoi((char*)pData[i].Data[eCI_Static]);
				ItemBase[ItemPos].ItemGrow = (DWORD)atoi((char*)pData[i].Data[eCI_Grow]);
			}
		}
	}
	for(int n=0; n<OverlapCount; n++)
	{
		int m=0;
		for(m=0; m<(TABCELL_SHOPINVEN_NUM+40); m++)
		{
			if( ItemBase[m].dwDBIdx == 0 )
			{
				ItemBase[m] = ItemOverlap[n];
				ItemBase[m].Position = m+TP_SHOPINVEN_START;
				ItemMoveUpdateToDB( pPlayer->GetID(), 0, 0, ItemBase[m].dwDBIdx, ItemBase[m].Position );
				break;
			}
		}
		if( m == (TABCELL_SHOPINVEN_NUM+40) )
		{
			break;
		}
	}
	pPlayer->InitShopInvenInfo( ItemBase );
	pPlayer->SendItemTidyShopInfo();
	PetWearItemInfo(pPlayer->GetID(), TP_PETWEAR_START, TP_PETWEAR_END);*/
//---------------------------------------------------------------------------------------------------------
	CPlayer* pPlayer;
	DWORD count = pMessage->dwResult;
	if((count == 1) && (atoi((char*)pData->Data[0]) == 0))
	{
		pPlayer = (CPlayer *)g_pUserTable->FindUser(atoi((char*)pData->Data[1]));
		if(pPlayer == NULL)
			return;
		ITEM_TOTALINFO Iteminfo;
		memset(&Iteminfo, 0, sizeof(ITEM_TOTALINFO));
		pPlayer->InitItemTotalInfo(&Iteminfo);
		ShopItemInvenInfo( pPlayer->GetID() );
	}
	else
	{
		pPlayer = (CPlayer *)g_pUserTable->FindUser((DWORD)atoi((char*)pData[0].Data[eCI_ObjectID]));
		if(pPlayer == NULL)
			return;
		ITEM_TOTALINFO Iteminfo;
		const int maxoverlap = MAX_YOUNGYAKITEM_DUPNUM;
		int OverlapCount = 0;
		ITEMBASE *pItemBase=NULL;
		memset(&Iteminfo, 0, sizeof(ITEM_TOTALINFO));
		ITEMBASE ItemBase[TABCELL_SHOPINVEN_NUM+40];
		ITEMBASE ItemOverlap[TABCELL_SHOPINVEN_NUM+40];
		memset( ItemBase, 0, sizeof(ITEMBASE)*(TABCELL_SHOPINVEN_NUM+40) );
		memset( ItemOverlap, 0, sizeof(ITEMBASE)*(TABCELL_SHOPINVEN_NUM+40) );
		for(DWORD  i = 0; i < count; i++)
		{
			POSTYPE ItemPos = atoi((char*)pData[i].Data[eCI_Position]);
			if(ItemPos >= TP_SHOPINVEN_START && ItemPos < TP_SHOPINVEN_END)
			{
				ItemPos -= TP_SHOPINVEN_START;
				if( ItemPos >= (TABCELL_SHOPINVEN_NUM+40) || ItemBase[ItemPos].dwDBIdx )
				{
					ItemOverlap[OverlapCount].dwDBIdx = (DWORD)atoi((char*)pData[i].Data[eCI_DBIDX]);
					ItemOverlap[OverlapCount].wIconIdx = (DWORD)atoi((char*)pData[i].Data[eCI_IDX]);
					ItemOverlap[OverlapCount].Position = (POSTYPE)atoi((char*)pData[i].Data[eCI_Position]);
					ItemOverlap[OverlapCount].QuickPosition = (POSTYPE)atoi((char*)pData[i].Data[eCI_QPosition]);
					ItemOverlap[OverlapCount].Durability = (DURTYPE)atoi((char*)pData[i].Data[eCI_Durability]);
					ItemOverlap[OverlapCount].ItemParam = (ITEMPARAM)atoi((char*)pData[i].Data[eCI_Param]);
					ItemOverlap[OverlapCount].ItemStatic = (WORD)atoi((char*)pData[i].Data[eCI_Static]);
					ItemOverlap[OverlapCount].ItemGrow = (DWORD)atoi((char*)pData[i].Data[eCI_Grow]);
					++OverlapCount;
				}
				else
				{
					ItemBase[ItemPos].dwDBIdx = (DWORD)atoi((char*)pData[i].Data[eCI_DBIDX]);
					ItemBase[ItemPos].wIconIdx = (DWORD)atoi((char*)pData[i].Data[eCI_IDX]);
					ItemBase[ItemPos].Position = (POSTYPE)atoi((char*)pData[i].Data[eCI_Position]);
					ItemBase[ItemPos].QuickPosition = (POSTYPE)atoi((char*)pData[i].Data[eCI_QPosition]);
					ItemBase[ItemPos].Durability = (DURTYPE)atoi((char*)pData[i].Data[eCI_Durability]);
					ItemBase[ItemPos].ItemParam = (ITEMPARAM)atoi((char*)pData[i].Data[eCI_Param]);
					ItemBase[ItemPos].ItemStatic = (WORD)atoi((char*)pData[i].Data[eCI_Static]);
					ItemBase[ItemPos].ItemGrow = (DWORD)atoi((char*)pData[i].Data[eCI_Grow]);
				}
			}
		}
		for(int n=0; n<OverlapCount; n++)
		{
			int m=0;
			for(m=0; m<(TABCELL_SHOPINVEN_NUM+40); m++)
			{
				if( ItemBase[m].dwDBIdx == 0 )
				{
					ItemBase[m] = ItemOverlap[n];
					ItemBase[m].Position = m+TP_SHOPINVEN_START;
					ItemMoveUpdateToDB( pPlayer->GetID(), 0, 0, ItemBase[m].dwDBIdx, ItemBase[m].Position );
					break;
				}
			}
			if( m == (TABCELL_SHOPINVEN_NUM+40) )
			{
				break;
			}
		}
		pPlayer->InitShopInvenInfo( ItemBase );
		pPlayer->SendItemTidyShopInfo();
	}
//---------------------------------------------------------------------------------------------------------
	/*CPlayer* pPlayer;
	DWORD count = pMessage->dwResult;
	ITEMBASE ItemOverlap[80];
	memset(&ItemOverlap, 0, sizeof(ITEMBASE)*80);
	if((count == 1) && (atoi((char*)pData->Data[0]) == 0))
	{
		pPlayer = (CPlayer *)g_pUserTable->FindUser(atoi((char*)pData->Data[1]));
		if(pPlayer == NULL)
			return;
		pPlayer->SetItemTotalInfo(ItemOverlap);
		//ITEM_TOTALINFO Iteminfo;
		//memset(&Iteminfo, 0, sizeof(ITEM_TOTALINFO));
		//pPlayer->InitItemTotalInfo(&Iteminfo);
		//ShopItemInvenInfo( pPlayer->GetID() );
	}
	else
	{
		pPlayer = (CPlayer *)g_pUserTable->FindUser((DWORD)atoi((char*)pData[0].Data[eCI_ObjectID]));
		if(pPlayer == NULL)
			return;
		//ITEM_TOTALINFO Iteminfo;
		//const int maxoverlap = MAX_YOUNGYAKITEM_DUPNUM;///100
		//int OverlapCount = 0;
		//ITEMBASE *pItemBase=NULL;
		//memset(&Iteminfo, 0, sizeof(ITEM_TOTALINFO));

		//ITEMBASE ItemBase[TABCELL_SHOPINVEN_NUM];
		//ITEMBASE ItemOverlap[TABCELL_SHOPINVEN_NUM];
		//memset( ItemBase, 0, sizeof(ITEMBASE)*TABCELL_SHOPINVEN_NUM );
		//memset( ItemOverlap, 0, sizeof(ITEMBASE)*TABCELL_SHOPINVEN_NUM );
		for(DWORD  i = 0; i < count; i++)
		{
			ItemOverlap[OverlapCount].dwDBIdx = (DWORD)atoi((char*)pData[i].Data[eCI_DBIDX]);
			ItemOverlap[OverlapCount].wIconIdx = (DWORD)atoi((char*)pData[i].Data[eCI_IDX]);
			ItemOverlap[OverlapCount].Position = (POSTYPE)atoi((char*)pData[i].Data[eCI_Position]);
			ItemOverlap[OverlapCount].QuickPosition = (POSTYPE)atoi((char*)pData[i].Data[eCI_QPosition]);
			ItemOverlap[OverlapCount].Durability = (DURTYPE)atoi((char*)pData[i].Data[eCI_Durability]);
			ItemOverlap[OverlapCount].ItemParam = (ITEMPARAM)atoi((char*)pData[i].Data[eCI_Param]);
			ItemOverlap[OverlapCount].ItemStatic = (WORD)atoi((char*)pData[i].Data[eCI_Static]);
			ItemOverlap[OverlapCount].ItemGrow = (DWORD)atoi((char*)pData[i].Data[eCI_Grow]);
			
		}
		
		pPlayer->SetItemTotalInfo(ItemOverlap);
		//pPlayer->InitShopInvenInfo( ItemBase );
		pPlayer->SendItemTidyShopInfo();
	}*/

}
void ClearFortWarData()
{
	g_Console.LOG(4, "ClearFortWarData");
	sprintf(txt, "DELETE %s", STORED_CLEAR_FORTWAR_INFO);
	g_DB.Query(eQueryType_FreeQuery, eClearFort, 0, txt);
}

void ItemFlashNameSet(DWORD dwCharacterIdx,DWORD NameFlag)
{
	sprintf(txt, "EXEC %s %d,%d", STORED_ITEM_FLASH1_UPDATE, dwCharacterIdx,NameFlag);
	g_DB.Query(eQueryType_FreeQuery,eItemFlashUpdate1,0, txt);
}

void RItemFlashNameSet(LPQUERY pData, LPDBMESSAGE pMessage)
{
	if(pMessage->dwResult)
	{
	 	CPlayer * pPlayer = (CPlayer *)g_pUserTable->FindUser(atoi((char*)pData->Data[0]));

		if(!pPlayer)
		{
			return;
		}

		//pPlayer->SetFlashNameFlag(atoi((char*)pData->Data[2]));

		/*MSG_DWORD2 msg;
		msg.Category=MP_ITEMEXT;
		msg.Protocol=MP_ITEMEXT_FLASHNAME1_ACK;
		msg.dwObjectID=pPlayer->GetID();
		msg.dwData1=atoi((char*)pData->Data[1]);
		msg.dwData2=atoi((char*)pData->Data[2]);*/

		/*MSGFLASHNAME msg;
		msg.Category=MP_ITEMEXT;
		msg.Protocol=MP_ITEMEXT_FLASHNAME1_ACK;
		msg.dwObjectID=pPlayer->GetID();
		msg.dwNameFlag=atoi((char*)pData->Data[1]);
		msg.dwData2=atoi((char*)pData->Data[2]);

		pPlayer->SendMsg(&msg, sizeof(msg));*/
	}
}

void ItemFlashNameSet2(DWORD dwCharacterIdx,DWORD ItemPos,char * pName)
{
	if( !isValidQueryString(pName))
	{
	   return;
	}

	if( CheckString(pName))
	{
		return;
	}

	sprintf(txt, "EXEC %s %d,%d,\'%s\'", STORED_ITEM_FLASH2_UPDATE, dwCharacterIdx,ItemPos,pName);
	g_DB.Query(eQueryType_FreeQuery,eItemFlashUpdate2,0, txt);
}

void RItemFlashNameSet2(LPQUERY pData, LPDBMESSAGE pMessage)
{
	DWORD DBIdx = 0;
	if(pMessage->dwResult)
	{
	 	CPlayer * pPlayer = (CPlayer *)g_pUserTable->FindUser(atoi((char*)pData->Data[0]));

		if(!pPlayer)
		{
			return;
		}

		//pPlayer->SetFlashName((char*)pData->Data[2]);
		DBIdx = (DWORD)atoi((char*)pData->Data[1]);
//-------------------------------------------------------------------
		ITEMBASE* pItemBase = NULL;

		// Item Position 犬牢
		int i=0; 
		for(i=0; i<(SLOT_SHOPINVEN_NUM+TABCELL_SHOPINVEN_PLUS_NUM)/*/2*/; i++)
		{
			pItemBase = (ITEMBASE*)ITEMMGR->GetItemInfoAbsIn( pPlayer, i+TP_SHOPINVEN_START );
			if( !pItemBase )	continue;
			
			if( pItemBase->dwDBIdx == DBIdx )
			{
				MSG_FLASH_SET msg;
				msg.Category=MP_ITEMEXT;
				msg.Protocol=MP_ITEMEXT_FLASHNAME2_ACK;
				msg.dwObjectID=pPlayer->GetID();
				msg.ItemPos=pItemBase->Position;//atoi((char*)pData->Data[1]);
				msg.ItemIdx=0;
				memcpy(msg.pName,(char*)pData->Data[2],MAX_NAME_LENGTH+1);
				pPlayer->SendMsg(&msg, sizeof(msg));
				if( EI_TRUE != ITEMMGR->DiscardItem( pPlayer, i+TP_SHOPINVEN_START, pItemBase->wIconIdx, 1 ) )
				{
				//	char buf[64];
				//	sprintf(buf, "ChangeName OK, DiscardItem Fail : %d", pMessage->dwID);
				//	ASSERTMSG(0, buf);
				}

				break;
			}
		}
		
		if( i >= (SLOT_SHOPINVEN_NUM+TABCELL_SHOPINVEN_PLUS_NUM)/*/2*/ )
		{
		//	char buf[64];
		//	sprintf(buf, "ChangeName OK, DeleteItem Fail: %d", pMessage->dwID);
		//	ASSERTMSG(0, buf);
		}

	//	QSTATETYPE time = GetCurTime();
	//	UsingShopItemUpdateToDB( 0, pPlayer->GetID(), pItemBase->wIconIdx, pItemBase->dwDBIdx, 0, time, 0 );
		pPlayer->GetShopItemManager()->DeleteUsingShopItem( pItemBase->wIconIdx );

	//	SHOPITEMBASE ShopItemBase;
	//	ShopItemBase.BeginTime.value = time;
	//	ShopItemBase.Param = 0;
	//	memcpy( &ShopItemBase.ItemBase, pItemBase, sizeof(ITEMBASE) );
	//	pPlayer->GetShopItemManager()->AddShopItemUse( &ShopItemBase );
//-------------------------------------------------------------------
		
	}
}



void DungeonEntrance(DWORD dwConnectionIndex, DWORD dwPlayerMapNum, DWORD dwPlayerIndex, DWORD dwPartyIndex, DWORD dwKeyIndex, DWORD dwItemIndex, DWORD dwItemSLot, WORD wDungeonMapNum, eDIFFICULTY difficulty,DWORD dwMode)
{
	g_DB.FreeMiddleQuery(
		RDungeonEntrance,
		dwPlayerIndex,
		"EXEC dbo.MP_DUNGEONENTRANCE %d, %d, %d, %d, %d, %d, %d, %d, %d , %d", dwConnectionIndex, dwPlayerMapNum, dwPlayerIndex, dwPartyIndex, dwKeyIndex, dwItemIndex, dwItemSLot, wDungeonMapNum, difficulty,dwMode);
}
void RDungeonEntrance(LPMIDDLEQUERY query, LPDBMESSAGE dbMessage)
{
	if(1 != dbMessage->dwResult)
	{
		return;
	}
	const MIDDLEQUERYST& record = query[ 0 ];
	DWORD dwAgentNum		= DWORD(atoi((char*)record.Data[1]));
	DWORD dwPlayerMapNum	= DWORD(atoi((char*)record.Data[2]));
	DWORD dwPlayerIndex		= DWORD(atoi((char*)record.Data[3]));
	DWORD dwNpcKind			= DWORD(atoi((char*)record.Data[5]));
	DWORD dwItemIndex		= DWORD(atoi((char*)record.Data[6]));
	DWORD dwItemSlot		= DWORD(atoi((char*)record.Data[7]));
	DWORD wDungeonMapNum	= DWORD(atoi((char*)record.Data[8]));
	DWORD dwTotalVisit		= DWORD(atoi((char*)record.Data[10]));
	DWORD dwDailyVisit		= DWORD(atoi((char*)record.Data[11]));
	DWORD dwElapsedMinute	= DWORD(atoi((char*)record.Data[12]));

	//DWORD dwModes = DWORD(atoi((char*)record.Data[13]));

	//g_Console.LOG(4, "dwModes:%d",dwModes);
	CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser(dwPlayerIndex);
	if(! pPlayer)
		return;

	/*DWORD dwError = eDungeonERR_None;
	if(!pPlayer->GetShopItemManager()->IsCanInPutFBMap()&&DungeonMGR->GetFubenKey(dwNpcKind)==1)
		dwError = eDungeonERR_NoKeyInfo;
	if(pPlayer->GetLevel() < DungeonMGR->EntraceConditionData(dwNpcKind)->MinLevel)
		dwError = eDungeonERR_NotEnoughLevel;
	if(DungeonMGR->EntraceConditionData(dwNpcKind)->MaxLevel < pPlayer->GetLevel())
		dwError = eDungeonERR_OverLevel;
	if(pPlayer->GetMoney() < DungeonMGR->EntraceConditionData(dwNpcKind)->FubenFee)
		dwError = eDungeonERR_CreateFailed;//eDungeonERR_NotEnoughGold;
	if(dwElapsedMinute > 1440)
		dwDailyVisit = 0;

	if(eDungeonERR_None != dwError)
	{
		MSG_DWORD		msg;
		msg.Category	= MP_DUNGEON;
		msg.Protocol	= MP_DUNGEON_ENTRANCE_NACK;
		msg.dwObjectID	= dwPlayerIndex;
		msg.dwData		= dwError;
		pPlayer->		SendMsg(&msg, sizeof(msg));
		return;
	}*/

	//DungeonUpdate(dwPlayerIndex, wDungeonMapNum, ++dwTotalVisit, ++dwDailyVisit);
	MSG_DWORD5		msg;
	msg.Category	= MP_DUNGEON;
	msg.Protocol	= MP_DUNGEON_ENTRANCE_FROM_SERVER_SYN;
	msg.dwObjectID	= dwPlayerIndex;
	msg.dwData1		= dwNpcKind;
	msg.dwData2		= dwPlayerMapNum;
	msg.dwData3		= pPlayer->GetPartyIdx();
	msg.dwData4		= (MAPTYPE)wDungeonMapNum;//dwItemIndex;
	msg.dwData5		= dwItemSlot;
	PACKEDDATA_OBJ->SendToMapServer(pPlayer->GetAgentNum(),/*dwAgentNum,*/(MAPTYPE)wDungeonMapNum, &msg, sizeof(msg));
	//pPlayer->SetMoney(DungeonMGR->EntraceConditionData(dwNpcKind)->FubenFee, MONEY_SUBTRACTION, MF_LOST, eItemTable_Inventory, eMoneyLog_GetCheatMoney, 0);
}

void DungeonUpdate(DWORD dwPlayerIndex, DWORD wDungeonMapNum, DWORD wTotalVisit, DWORD wDailyVisit, BYTE byDoRollBack)
{
	g_DB.FreeMiddleQuery(
		0,
		dwPlayerIndex,
		"EXEC dbo.MP_DUNGEONUPDATE %d, %d, %d, %d, %d", dwPlayerIndex, wDungeonMapNum, wTotalVisit, wDailyVisit, byDoRollBack);
}



void MallBuyItem( DWORD dwConnectionIndex, CPlayer* pPlayer,MSG_ITEM_BUY_SYN * bMsg)
{
	g_DB.FreeMiddleQuery(
		RMallBuyItem,
		pPlayer->GetID(),
		"EXEC dbo.JACK_ITEMMALLLIMIT %d, %d, %d, %d, %d, %d, %d, %d, %d, %d", 
		pPlayer->GetID(),
		bMsg->wBuyItemIdx,
		bMsg->wDealerIdx,
		bMsg->wIdxPos,
		bMsg->wBuyType,
		bMsg->wLimit,
		bMsg->BuyItemNum,

		bMsg->BuyRight,
		bMsg->LimitKind,
		bMsg->LimitCount
		
		);
}
void RMallBuyItem(LPMIDDLEQUERY query, LPDBMESSAGE dbMessage)
{
	if( dbMessage->dwResult )
	{
		DWORD dwBuyMsg	=	atoi((char*)query->Data[0]);//time countdown
		DWORD Char_Idx	=	atoi((char*)query->Data[1]);//userId mhcmember
		DWORD ItemIdx	=	atoi((char*)query->Data[2]);//itemidx
		DWORD DealerIdx	=	atoi((char*)query->Data[3]);//dealidx
		DWORD ItemPos	=	atoi((char*)query->Data[4]);//itemposition
		DWORD BuyType	=	atoi((char*)query->Data[5]);//item buy type //ingot , bubble , gold
		DWORD BuyLimit	=	atoi((char*)query->Data[6]);//no idea
		DWORD BuyCount	=	atoi((char*)query->Data[7]);//item buy count

		DWORD BuyRight	=	atoi((char*)query->Data[8]);//no idea
		DWORD LimitKind	=	atoi((char*)query->Data[9]);//no idea
		DWORD LimitCount=	atoi((char*)query->Data[10]);//limit item buy count

		g_Console.LOG( 4, "BuyCount:%d / LimitCount:%d / BuyRight:%d",BuyCount,LimitCount,BuyRight);

		CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser(Char_Idx);
		if(! pPlayer)
			return;

		int rt = 0;

		int BuyCountTemp=0;
		int BuyCountFinal=0;


		if(DealerIdx == MALLSHOPIDX)
		{
			if(dwBuyMsg!=0)
			{
				MSG_DWORD3 msg;
				msg.Category	= MP_ITEM;
				msg.Protocol	= MP_ITEM_MALL_GOLD_GET_NACK;//MP_ITEM_DEALER_NACK;
				msg.dwObjectID	= pPlayer->GetID();
				msg.dwData1		= dwBuyMsg;
				msg.dwData2		= BuyRight;
				msg.dwData3		= LimitCount;
				
				pPlayer->SendMsg( &msg, sizeof(msg) );
				g_Console.LOG( 4, "dwBuyMsg is %d %d",dwBuyMsg,pPlayer->GetID(),LimitCount);
				return;
			}
			else
			{
				if(LimitCount!=0)
				{
					if(BuyCount>LimitCount)
					{
						
						BuyCountTemp=BuyCount-LimitCount;

						BuyCountFinal=BuyCount-BuyCountTemp;
					}
				}
				else
					BuyCountFinal=BuyCount;
			}
			rt = ITEMMGR->BuyItemForGoldShop(pPlayer, ItemIdx, BuyCountFinal/*BuyCount*/, DealerIdx,ItemPos,BuyType);
		}
		else//utk npc shop
		{
			rt = ITEMMGR->BuyItem(pPlayer, ItemIdx, BuyCount, DealerIdx,ItemPos,BuyType);
		}

		if(EI_TRUE == rt)
		{
		}
		else
		{
			MSG_ITEM_ERROR msg;
			msg.Category = MP_ITEM;
			msg.Protocol = MP_ITEM_BUY_NACK;
			msg.dwObjectID = pPlayer->GetID();
			ITEMMGR->SendErrorMsg(pPlayer, &msg, sizeof(msg), rt);
		}
	}
	//else
	//{
	//	ASSERT(0);
	//}
}
void MallBuyItemUpdate(DWORD dwPlayerIndex, DWORD wDungeonMapNum, DWORD wTotalVisit, DWORD wDailyVisit, BYTE byDoRollBack)
{
	g_DB.FreeMiddleQuery(
		0,
		dwPlayerIndex,
		"EXEC dbo.MP_MALLSHOPLIMITUPDATE %d, %d, %d, %d, %d", dwPlayerIndex, wDungeonMapNum, wTotalVisit, wDailyVisit, byDoRollBack);
}

void RUpdateUserCredit(LPQUERY pData, LPDBMESSAGE pMessage)  //在线充值元宝刷新数据库回调处理函数定义	by:胡汉三	QQ:112582793
{
	if (pMessage->dwResult == 0)
	{
		return;
	}
	CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser( pMessage->dwID );
	if( !pPlayer )
	{		
		return;
	}
	DWORD CurGoldMoney = pPlayer->GetGoldMoney();
	DWORD DBGoldMoney = atoi((char*)pData->Data[0]);

	if(DBGoldMoney>CurGoldMoney)
	{
		DWORD ChangeMoney = DBGoldMoney-CurGoldMoney;
		pPlayer->UpdateGoldMoney(DBGoldMoney,ChangeMoney);
	}
}
void CharacterHeroGoldInfoUpdate(DWORD ID,DWORD GoldMoney)
{
	sprintf(txt,"EXEC %s %d,%d","MP_CHARACTER_HeroGoldMoneyInfoUPdate",ID,GoldMoney);//在线充值元宝刷新更新数据库	by:胡汉三	QQ:112582793
	g_DB.Query(eQueryType_FreeQuery, eHeroInfoUpdate, 0, txt);
}
void LogGoldMoney(WORD LogType,WORD FromChrID,DWORD FromTotalGold,WORD ToChrID,DWORD ToChrTotalGold,DWORD ChangeGold,DWORD BuyItemIdx,DWORD Durability)
{
	sprintf(txt, "EXEC  %s %d,%d,%d,%d,%d,%d,%d,%d", "dbo.up_GoldMoneyLog",LogType,FromChrID,FromTotalGold,ToChrID,ToChrTotalGold,ChangeGold,BuyItemIdx,Durability);
	g_DB.LogQuery(eQueryType_FreeQuery, eLogItemMoney, 0, txt);
}









void DGEntrance(DWORD dwConnectionIndex, DWORD dwPlayerMapNum, DWORD dwPlayerIndex, DWORD dwPartyIndex, WORD wDungeonMapNum,DWORD dwChannelIndex)
{
	g_DB.FreeMiddleQuery(
		RDGEntrance,
		dwPlayerIndex,
		"EXEC dbo.JACK_DUNGEONENTRANCE %d, %d, %d, %d, %d, %d", dwConnectionIndex, dwPlayerMapNum, dwPlayerIndex, dwPartyIndex, wDungeonMapNum,dwChannelIndex);
}
void RDGEntrance(LPMIDDLEQUERY query, LPDBMESSAGE dbMessage)
{
	if(1 != dbMessage->dwResult)
	{
		return;
	}
	const MIDDLEQUERYST& record = query[ 0 ];
	//1 FOR dbMessage->dwResult
	DWORD dwPlayerMapNum = DWORD(atoi((char*)record.Data[2]));
	DWORD dwPlayerIndex = DWORD(atoi((char*)record.Data[3]));
	//4 FOR PARTY DATA
	DWORD dwPartyIdx = DWORD(atoi((char*)record.Data[4]));
	DWORD wDungeonMapNum = WORD(atoi((char*)record.Data[5]));

	DWORD dwTimerCount = DWORD(atoi((char*)record.Data[6]));

	DWORD dwSubMission = DWORD(atoi((char*)record.Data[7]));
	DWORD dwMonsterKill = DWORD(atoi((char*)record.Data[8]));
	DWORD dwChannelIdx = DWORD(atoi((char*)record.Data[9]));

	CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser(dwPlayerIndex);
	if(! pPlayer)
		return;
	//g_Console.LOG( 4, "%d",dwMonsterKill);
//	g_Console.LOG( 4, "%d %d %d %d %d %d %d %d",
//		dwPlayerMapNum,
//		dwPlayerIndex,
//		dwPartyIdx,
//		wDungeonMapNum,
//		dwTimerCount,
//		dwSubMission,
//		dwMonsterKill,
//		dwChannelIdx
//		);

	DungeonMGR->SendKillCount(dwMonsterKill,pPlayer->GetChannelID());

	pPlayer->AddFBTimerMonsterNum(dwMonsterKill);
	pPlayer->SetFBMapArea(dwSubMission);

	FUBENMAPINFO*pEvent=DungeonMGR->GetFBMapAreaInfo(pPlayer->GetFBMapArea());
	DungeonMGR->SendNewMessage(DungeonMGR->GetInsDungeonInfoData()->dgStringIdx,dwSubMission,dwMonsterKill,pEvent->FBMapKillCount,0,pPlayer);

	/*DWORD dwNewTimer = DungeonMGR->CalculateTimer(pPlayer->GetChannelID());
	Sleep(1000);
	pPlayer->SetFBTimeTo(dwNewTimer);*/
}
void DGUpdate(DWORD dwPartyIndex, DWORD wDungeonMapNum, DWORD wChannelIndex, DWORD wTimerCount, DWORD wSubMission , BYTE byDoRollBack)
{//playerindex = party //total n daily visit = channel n timercount
	g_DB.FreeMiddleQuery(
		RDGUpdate,
		dwPartyIndex,
		"EXEC dbo.JACK_DUNGEONUPDATE %d, %d, %d, %d, %d, %d", dwPartyIndex, wDungeonMapNum, wChannelIndex, wTimerCount,wSubMission, byDoRollBack);
}
void RDGUpdate(LPMIDDLEQUERY query, LPDBMESSAGE dbMessage)
{
	if(1 != dbMessage->dwResult)
	{
		return;
	}
	const MIDDLEQUERYST& record = query[ 0 ];
	//1 FOR dbMessage->dwResult
	DWORD dwPlayerMapNum = DWORD(atoi((char*)record.Data[2]));
	DWORD dwPlayerIndex = DWORD(atoi((char*)record.Data[3]));
	//4 FOR PARTY DATA
	DWORD dwPartyIdx = DWORD(atoi((char*)record.Data[4]));
	DWORD wDungeonMapNum = WORD(atoi((char*)record.Data[5]));

	DWORD dwTimerCount = DWORD(atoi((char*)record.Data[6]));

	DWORD dwSubMission = DWORD(atoi((char*)record.Data[7]));
	DWORD dwMonsterKill = DWORD(atoi((char*)record.Data[8]));
	DWORD dwChannelIdx = DWORD(atoi((char*)record.Data[9]));

	DungeonMGR->SendKillCount(dwPlayerIndex,dwPlayerMapNum);
	DungeonMGR->SetMonsterKillCount(dwPlayerMapNum,dwPlayerIndex);
	DungeonMGR->SetFBMapArea(dwPlayerMapNum,/*pEvent->FBMapNextFlag*/dwPartyIdx);

	//g_Console.LOG( 4, "%d %d %d %d %d %d %d %d",
	//	dwPlayerMapNum,
	//	dwPlayerIndex,
	//	dwPartyIdx,
	//	wDungeonMapNum,
	//	dwTimerCount,
	//	dwSubMission,
	//	dwMonsterKill,
	//	dwChannelIdx
	//	);

	//CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser(dwPlayerIndex);
	//if(! pPlayer)
	//	return;
	//g_Console.LOG( 4, "%d",dwMonsterKill);


}









void ForbidChatUpdate( DWORD dwPlayerIdx, DWORD dwTargetIdx, const char* pReason, DWORD dwForbidTime, BOOL bIsForbidAdd )
{
	char txt[1024] = {0,};
	sprintf(txt, "EXEC dbo.MP_GM_FORBIDCHAT_UPDATE %d, %d, \'%s\', %d, %d", dwPlayerIdx, dwTargetIdx, pReason, dwForbidTime, (byte)bIsForbidAdd );
	//g_DB.Query( eQueryType_FreeQuery, eForbidChatUpdate, dwPlayerIdx, txt, dwPlayerIdx );

	g_DB.Query(eQueryType_FreeQuery,eForbidChatUpdate,dwPlayerIdx,txt);
}

void RForbidChatUpdate(LPQUERY pData, LPDBMESSAGE pMessage)
{
	if( !pMessage->dwResult )
	{
		return;
	}

	BYTE	bIsForbidChat 	= BYTE( atoi( (char*)pData->Data[0] ) );
	DWORD	dwTargetIdx		= DWORD( atoi( (char*)pData->Data[1] ) );
	__time64_t	time		= _atoi64( (char*)pData->Data[2] );

	CPlayer* const pTarget = (CPlayer*)g_pUserTable->FindUser( dwTargetIdx );
	if( !pTarget )
	{
		return;		
	}

	pTarget->SetForbidChatTime( time );

	stTime64t msg;
	ZeroMemory( &msg, sizeof(msg) );
	msg.Category	= MP_CHAT;
	msg.Protocol	= (BYTE)(bIsForbidChat? MP_CHAT_FORBIDCHAT : MP_CHAT_PERMITCHAT);
	msg.dwObjectID	= pTarget->GetID();
	msg.time64t		= time;
	pTarget->SendMsg(&msg, sizeof(msg));
}

void ForbidChatLoad( DWORD dwPlayerIdx )
{
	char txt[1024] = {0,};
	sprintf(txt, "EXEC dbo.MP_GM_FORBIDCHAT_LOAD %d", dwPlayerIdx );
	//g_DB.Query( eQueryType_FreeQuery, eForbidChatLoad, dwPlayerIdx, txt, dwPlayerIdx );

	g_DB.Query(eQueryType_FreeQuery,eForbidChatLoad,dwPlayerIdx,txt);
}

void RForbidChatLoad(LPQUERY pData, LPDBMESSAGE pMessage)
{
	if( !pMessage->dwID )
	{
		return;
	}

	DWORD		dwTargetIdx	= DWORD( atoi( (char*)pData->Data[0] ) );
	__time64_t	ReleaseTime	= _atoi64( (char*)pData->Data[1] );

	CPlayer* const pPlayer = (CPlayer*)g_pUserTable->FindUser( dwTargetIdx );
	if( !pPlayer )
	{
		return;		
	}

	__time64_t curTime;
	_time64( &curTime );
	if( curTime > ReleaseTime )
	{
		return;
	}

	pPlayer->SetForbidChatTime( ReleaseTime );


	//g_Console.LOG( 4, "RForbidChatLoad %d",ReleaseTime);

	stTime64t msg;
	ZeroMemory( &msg, sizeof(msg) );
	msg.Category	= MP_CHAT;
	msg.Protocol	= MP_CHAT_FORBIDCHAT;
	msg.dwObjectID	= pPlayer->GetID();
	msg.time64t		= ReleaseTime;
	pPlayer->SendMsg(&msg, sizeof(msg));
}
void CharacterLoginEventUpdate(CPlayer* pPlayer)
{
	if( !pPlayer->GetInited() )	return;

	sprintf(txt, "EXEC  %s %d, %d, %d",
		STORED_LOGINEVENT_UPDATE,
		pPlayer->GetUserID(),
		pPlayer->GetEventIDFlag(),
		pPlayer->GetEventLastMapTime());
	g_DB.Query(eQueryType_FreeQuery, eLoginEventUpdate, 0, txt);
}

void UpdateInstanceDungeonRank(DWORD dwCharacterIdx, INSDG_RANK_INFO* pRankInfo)
{
	if (pRankInfo == NULL)
		return;

	sprintf(txt, "EXEC dbo.MP_INSTANCE_DUNGEON_RANK_UPDATE %d, %d, %d, %d, %d", dwCharacterIdx, pRankInfo->dwInsDGIndex,
			pRankInfo->dwClearTime, pRankInfo->dwMonsterKillNum, pRankInfo->dwRankValue);
	g_DB.Query(eQueryType_FreeQuery, eInstanceDungeonRankUpdate, 0, txt);
}

void LoadInstanceDungeonRank(DWORD dwPlayerID)
{
	sprintf(txt, "EXEC dbo.MP_INSTANCE_DUNGEON_RANK_LOAD %d", dwPlayerID);
	g_DB.Query(eQueryType_FreeQuery, eInstanceDungeonRankLoad, dwPlayerID, txt);
}

void RLoadInstanceDungeonRank(LPQUERY pData, LPDBMESSAGE pMessage)
{
	if (pMessage->dwResult == 0)
		return;

	CPlayer* pPlayer = NULL;
	pPlayer = (CPlayer*)g_pUserTable->FindUser(pMessage->dwID);
	if (NULL == pPlayer)
		return;

	MSG_INSDG_RANK_TOTAL_INFO	msg;
	ZeroMemory(&msg, sizeof(MSG_INSDG_RANK_TOTAL_INFO));
	msg.Category = MP_INSDUNGEON;
	msg.Protocol = MP_INSDUNGEON_RANK_TOTAL_INFO;
	msg.dwObjectID = pMessage->dwID;
	msg.nCount = pMessage->dwResult;

	INSDG_RANK_INFO RankInfo[INSTANCE_DUNGEON_MAXNUM];
	memset(RankInfo, 0, sizeof(*RankInfo));

	for (int i = 0; i < pMessage->dwResult; i++)
	{
		RankInfo[i].dwInsDGIndex = (DWORD)atoi((char*)pData[i].Data[1]);
		RankInfo[i].dwClearTime = (DWORD)atoi((char*)pData[i].Data[2]);
		RankInfo[i].dwMonsterKillNum = (DWORD)atoi((char*)pData[i].Data[3]);
		RankInfo[i].dwRankValue = (DWORD)atoi((char*)pData[i].Data[4]);

		pPlayer->AddInsDGRankInfo(&RankInfo[i]);
	}

	memcpy(msg.RankInfo, RankInfo, sizeof(*msg.RankInfo) * pMessage->dwResult);

	pPlayer->SendMsg(&msg, msg.GetMsgLength());
}

void UpdateGradeItem(DWORD dwPlayerID, DWORD ItemDbIdx, DWORD Grade, DWORD ItemStatic)
{
	//	g_Console.LOG(4, "Query %d %d %d %d ", dwPlayerID, ItemDbIdx, Grade, ItemStatic);
	sprintf(txt, "EXEC dbo.MP_UpDateGradeItem %d,%d,%d,%d", dwPlayerID, ItemDbIdx, Grade, ItemStatic);
	g_DB.Query(eQueryType_FreeQuery, eUpDateGradeItem, dwPlayerID, txt);
}
void RUpdateGradeItem(LPQUERY pData, LPDBMESSAGE pMessage)
{
	CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser(pMessage->dwID);
	if (!pPlayer) {
		//	g_Console.LOG(4, "Player not found: dwID = %d", pMessage->dwID);
		return;
	}

	if (!pPlayer->GetInited()) {
		//		g_Console.LOG(4, "Player not initialized: dwID = %d", pMessage->dwID);
		return;
	}

	if (!pMessage->dwResult) {
		//	g_Console.LOG(4, "Database query failed: dwResult = %d", pMessage->dwResult);
		return;
	}

	// Log database data for debugging
	//g_Console.LOG(4, "Database data received: DBIdx = %s, ItemIdx = %s, Position = %s",
	//	(char*)pData->Data[1], (char*)pData->Data[2], (char*)pData->Data[3]);

	DWORD dwObjectID = atoi((char*)pData->Data[eCI_ObjectID]);
	DWORD dwItemIdx = atoi((char*)pData->Data[eCI_IDX]);
	DWORD dwDura = atoi((char*)pData->Data[eCI_Durability]);
	DWORD dwPos = atoi((char*)pData->Data[eCI_Position]);
	DWORD dwQPos = atoi((char*)pData->Data[eCI_QPosition]);
	DWORD dwDBIdx = atoi((char*)pData->Data[eCI_DBIDX]);
	DWORD dwParam = atoi((char*)pData->Data[eCI_Param]);
	//DWORD dwStoneIdx = atoi((char*)pData->Data[eCI_StoneIdx]);
	DWORD dwStatic = atoi((char*)pData->Data[eCI_Static]);
	DWORD dwGrow = atoi((char*)pData->Data[eCI_Grow]);
	//[耐久，制造者附加][2018/1/2]

	DWORD dwGrade30 = atoi((char*)pData->Data[eCI_Grade30]);

	CItemSlot* pTargetSlot = pPlayer->GetSlot(dwPos);
	if (!pTargetSlot) {
		//	g_Console.LOG(4, "Slot not found: Position = %d", Position);
		return;
	}

	ITEMBASE* pTargetItem = (ITEMBASE*)pTargetSlot->GetItemInfoAbs(dwPos);
	if (!pTargetItem) {
		//	g_Console.LOG(4, "Item not found in slot: Position = %d", Position);
		return;
	}
	//CItemSlot* pTargetSlot = pPlayer->GetSlot(dwPos);
	//Log the current state of the item
   //g_Console.LOG(4, "Item before update: DBIdx = %d, Grade = %d ,static = %d", pTargetItem->dwDBIdx, pTargetItem->Grade, pTargetItem->ItemStatic);

	if (pTargetSlot->UpdateItemAbs(pPlayer, dwPos, 0, 0, 0, 0, 0, UB_GRADE30, SS_NONE, 0, 0, 0,0, 0, dwGrade30) != EI_TRUE)
	{

		ASSERTMSG(0, "The ItemGradeAlexX UpdateItemAbs is Error!");
		return;
	}

	STATSMGR->CalcItemStats(pPlayer);  // 刷新附加
	// Log success of item update
//	g_Console.LOG(4, "Item successfully updated: DBIdx = %d, New Grade = %d ,static = %d", pTargetItem->dwDBIdx, pTargetItem->Grade, pTargetItem->ItemStatic);

	MSG_UPDATE_GRADE msg;
	msg.Category = MP_ITEMEXT;
	msg.Protocol = MP_UPDATE_GRADE;
	msg.ItemIdx = pTargetItem->wIconIdx;
	msg.Pos = pTargetItem->Position;
	msg.Grade = pTargetItem->Grade30;
	pPlayer->SendMsg(&msg, sizeof(msg));

	// Log message sent to client
	//g_Console.LOG(4, "MSG_UPDATE_GRADE sent: ItemIdx = %d, Pos = %d, Grade = %d",
	//	msg.ItemIdx, msg.Pos, msg.Grade);

}