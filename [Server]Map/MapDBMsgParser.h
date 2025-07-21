




#ifndef __MAPDBMSGPARSER_H__
#define __MAPDBMSGPARSER_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//#include ".\module\db.h"
//#include "CommonDBMsgParser.h"
//#include "Player.h"

class CPlayer;
class CParty;
class CAbilityGroup;
enum   DBMESSAGEIDFUNC 
{
	eNull,
	eCharacterInfoQuery,	
	eCharacterMugongQuery,		
	eCharacterItemSlotQuery,	
	eCharacterItemQuery,	
	eCharacterItemOptionQuery, 
	eCharacterItemOptionDeleteQuery,
	eCharacterItemRareOptionQuery,
	eCharacterItemRareOptionDeleteQuery,
	ePyogukItemOptionQuery,
	ePyogukItemRareOptionQuery,
	eShopItemInfoQuery,
	eMarryAdd,
	eMarryDel, 
	eMallListInfoQuery,
	eSSItemInsert,				
	eItemOptionInsert,
	eItemRareInsert,
	eItemUpdate,
	eItemCombineUpdate,
	eItemMoveUpdate,
	eItemMovePyogukUpdate,
	eItemMoveGuildUpdate,
	eItemMovePetInvenUpdate,
	eItemInsert,
	eItemDelete,
	eItemShopInvenInfo,
	eItemShopItemUseInfo,
	eItemShopItemUsing,
	eItemShopItemDelete,
	eItemShopItemUpdatetime,
	eItemShopItemUpdateParam,
	eItemShopItemUpdateUseParam,
	eItemShopItemUpdateUseInfo,
	eItemShopItemMPInfo,
	eItemShopMPUpdate,
	eItemShopMPDel,
	eItemShopGetItem,
	eItemMoveShopUpdate,
	eItemShopChangeName,
	eItemCharChange,
	eItemShopRareCreate,
	eItemShopIsUseableAvatar,
	eItemShopIsUseableEquip,
	eItemShopUpdatCharacterIdx,
	eItemShopUpdateAllInfo,

	eRegistLoginMapInfo,
	eUnRegistLoginMapInfo,

	eMugongUpdate2,
	eMugongMoveUpdate2,
	eMugongInsert,
	eMugongDelete,				

	eBaseEconomy,				
	eAuctionRegist,				
	eCharacterUpdate,			
	eHeroInfoUpdate,
	eTotalInfoUpdate,		
	eBadFameUpdate,
	
	eAuctionCheck,
	eAuctionPageList,
	eAuctionConfirm,
	ePartyRegistMapServer,		
	ePartyInfoByUserLogin,	
	ePartyCreate,
	ePartyBreakup,
	ePartyCharacterTbUpdate,
	ePartyAddMember,			
	ePartyDelMember,
	ePartyChangeMaster,

	eSavePoint,				
	eSaveMapChangePoint,	

	ePyogukBuyPyoguk,
	eCharacterPyogukInfo,
	eCharacterPyogukItemInfo,
	ePyogukMoneyUpdate,

	eFriendNotifyLogout,
	eFameCharacterUpdate,
	eBadFameCharacterUpdate,
	ePKCharacterUpdate,

	eWantedLoad,
	eWantedGetList,
	eWantedBuyRight,
	eWantedRegist,
	eWantedGiveupRight,
	eWantedComplete,
	eWantedDestroyed,
	eWantedOrderList,
	eWantedMyList,
	eWantedSearch,
	
	eQuestTotalInfo,
	eQuestUpdate,
	eQuestMainQuestLoad,
	eQuestSubQuestLoad,
	eQuestItemLoad,
	eQuestMainQuestInsert,
	eQuestSubQuestInsert,
	eQuestItemInsert,
	eQuestMainQuestUpdate,
	eQuestSubQuestUpdate,
	eQuestItemUpdate,
	eQuestMainQuestDelete,
	eQuestSubQuestDelete,
	eQuestSubQuestDeleteAll,
	eQuestItemDelete,
	eQuestItemDeleteAll,
	eQuestMainQuestUpdateCheckTime,

	eJournalGetList,
	eJournalInsertQuest,
	eJournalInsertWanted,
	eJournalInsertLevel,
	eJournalSaveUpdate,
	eJournalDelete,

	eLogCharacter,
	eLogExp,
	eLogMoney,
	eLogMoneyWrong,
	eLogMugong,
	eLogMugongExp,
	eLogItemMoney,
	eLogItemReinforce,
	eLogItemRare,
	eLogCheat,
	eInsertLogTool,
	eLogGuild,
	eLogPet,
	eLogGuildPoint,
	eLogGMToolUse,
	
	eCharacterInfoByTime,
	eMugongExpByTime,
	eSpeedHackCheck,

	eGameLogItem,

	eGuildLoadGuild,
	eGuildLoadMember,
	eGuildCreate,
	eGuildBreakUp,
	eGuildDeleteMember,
	eGuildAddMember,
	eGuildSecedeMember,
	eGuildLevelUp,
	eGuildChangeMemberRank,
	eGuildItemLoad,
	eGuildItemOption,
	eGuildItemRareOption,
	eGuildMoneyUpdate,
	eGuildGiveNickName,
	
	eConfirmUserOut,
	eSaveChrInfoBeforeOut,

	eCharacterInfoQueryToEventMap,

	eLoadGuildWarRecord,
	eDeleteGuildWarRecord,
	eLoadGuildFieldWar,
	eInsertGuildFieldWar,
	eDeleteGuildFieldWar,	
	eUpdateGuildFieldWarRecord,
	
	eGuildTournament_LoadAll,
	eGuildTournament_CancelLoad,
	eGuildTournament_CancelInsert,
	eGuildTournament_Insert,
	eGuildTournament_Delete,
	eGuildTournament_UpdateGuildInfo,
	eGuildTournament_UpdateTournamentInfo,	

	eJackpot_TotalMoney_Update,
	eJackpot_PrizeInfo_Update,

	eGuildUnionLoad,
	eGuildUnionCreate,
	eGuildUnionDestroy,
	eGuildUnionAddGuild,
	eGuildUnionRemoveGuild,
	eGuildUnionSecedeGuild,

	eSiegeWarInfoLoad,
	eSiegeWarInfoInsert,
	eSiegeWarInfoUpdate,
	eSiegeWarGuildInfoLoad,
	eSiegeWarGuildInsert,

	eSiegeWarGuildUpdate,
	eSiegeWarGuildDelete,

	eChangeCharacterAttr,

	eSiegeWarProfitInfoLoad,
	eSiegeWarProfitUpdateGuild,
	eSiegeWarProfitUpdateTaxRate,
	eSiegeWarProfitUpdateProfitMoney,

	eChangeGuildLocation,

	eChangeCharacterStageAbility,

	eCharacterPetInfoQuery,	
	ePetInsert,
	ePetUpdate,
	ePetDelete,

	ePetInvenItemOptionInfoQuery,	
	ePetInvenItemRareOptionInfoQuery,	
	ePetInvenItemInfoQuery,	

	eGuildLoadNotice,
	eGuildUpdateNotice,

	eQuestEventCheck,
	eQuestEventEnd,

	ePetWearItemInfoQuery,
	eGMEvent01,

	eGuildPointInfoLoad,
	eGuildPlustimeInfoLoad,
	eGuildAddHuntedMonsterCount,
	eGuildGetHuntedMonsterTotalCountWithInit,
	eGuildAddGuildPoint,
	eGuildUseGuildPoint,
	eGuildPlustimeEnd,
	eGuildTournamentAddLog,
	eSiegeWarAddLog,

	eGuildItemLoadInNeed,
	eGuildItemOptionInNeed,
	eGuildItemRareOptionInNeed,
	eCharacterExpFlag,

	eGuildTraineeInfo,
	eGuildTraineeInsert,
	eGuildTraineeDelete,

	eGuildTraineeGuildInfo,
	eGuildTraineeGuildDelete,

	eGuildTraineeDeletebyGuild,

	eGuildAddStudent,

	eGuildStudentLvUpCount,	
	eGuildStudentLvUpInfoInit,

	eTitanWearItemInfo,

	eCharacterTitanInfoQuery,	
	eCharacterTitanEquipItemEnduranceInfoQuery,	
	eTitanInsert,
	eTitanUpdate,
	eTitanDelete,

	eTitanEquipInfoInsert,
	eTitanEquipInfoUpdate,
	eTitanEquipInfoDelete,


	eTestGameQuery,

	eTestLogQuery,

	ePyogukTitanEnduranceOptionQuery,

	eQuest_EndQuest_New,
	eQuest_DeleteQuest_New,
	eQuest_EndSubQuest_New,


	eTitanEquipInfoInsertException,
	eTitanEquipInfoInsertExceptionPyoguk,

	eCharacterSkinInfoUpdate,			

	eCharacterUpdateResetStatusPoint,

	eCharacterSkinInfo,			

	eItemLimitInfoLoadAll,
	eItemLimitInfoUpdate,	

	eAutoNoteListLoad,		
	eAutoNoteListAdd,

	eMapItemDropListSelect,
	eMapItemDropListUpdate,
	eMapItemDropListInit,

	eFortWarInfoLoad,
	eFortWarInfoUpdate,
	eFortWarProfitMoneyUpdate,
	eFortWarItemLoad,
	eFortWarItemInsertToDB,
	eFortWarItemMoveToDB,	

	eCharacterItemStoneQuery, 

	eItemStoneInsert,      

	eItemStoneUpdate,

	eItemPyogukStone, 

	eItemUpdateLock,  

	eItemUpdateUnLock,

	eItemUpdateGrow,   

	eGambleUserInfoInsert,

	eGambleUserInfoSelect, 

	eGambleInfoSelect,  

	eSafePinCheck,		  //weiye 安全码检查回调函数定义 2017-12-04

	eSafePinChange,		  //weiye 安全码修改回调函数定义 2017-12-07

	eSafeUpdate,          //weiye 安全码更新回调函数     2017-12-05

	eLastMapDC,

	eFbTimeUpdate,        //weiye  副本时间回调标志      2018-02-06

	eItemSelect,		         //weiye 发送物品查询回调标志   2018-03-11

	eItemTidy,		     	 //weiye 整理包袱数据库回调函数 2018-03-24

	eItemTidyLoad,        //weiye 整理物品重载函数定义     2018-03-24

	eRebornInfoUpdate,

	eNPCCODE,

	eItemTidyShop,
	eItemTidyLoadShop,

	eClearFort,

	eItemFlashUpdate1,
	eItemFlashUpdate2,

	eGetDBGoldMoney,

	eForbidChatUpdate,
	eForbidChatLoad,

	eLoginEventUpdate,

	eInstanceDungeonRankUpdate,
	eInstanceDungeonRankLoad,
    eUpDateGradeItem,
	eItemQualityUpdate,
	eItemQualityChangeUpdate,
	MaxQuery
};
#define STORED_TEST_QUERY	"UP_GAME_TEST_QUERY"
#define STORED_CHARACTER_SELECT				"dbo.MP_CHARACTER_SelectByCharacterIDX"	
#define STORED_CHARACTER_KYUNGGONG			"dbo.MP_CHARACTER_KyungGong"				
#define STORED_CHARACTER_NAEGONG			"dbo.MP_CHARACTER_NaeGong"					
#define STORED_CHARACTER_MUGONGINFO			"dbo.MP_CHARACTER_MugongInfo"				
#define STORED_CHARACTER_ITEMSLOTINFO		"dbo.MP_CHARACTER_ItemSlotInfo_JP"	
#define STORED_CHARACTER_ITEMINFO			"dbo.MP_CHARACTER_ItemInfo"				
#define STORED_CHARACTER_ITEMRAREOPTIONINFO	"dbo.MP_CHARACTER_ItemRareOptionInfo"
#define STORED_CHARACTER_ITEMOPTIONINFO		"dbo.MP_CHARACTER_ItemOptionInfo"		
#define STORED_CHARACTER_SKILLINFO			"dbo.MP_CHARACTER_SkillInfo"		
#define STORED_CHARACTER_ITEMSTONE			"dbo.MP_CHARACTER_ItemStoneInfo"	

#define STORED_CHARACTER_EXPFLAG			"dbo.MP_CHARACTER_UpdateExpFlag"


#define STORED_CHARACTER_BASEECONOMY		"dbo.MP_MAP_BaseEconomy"				
#define STORED_CHARACTER_AUCTIONREGIST		"dbo.MP_Auction_Regist"					
#define STORED_CHARACTER_AUCTIONONEPAGE		"dbo.MP_Auction_PageCalculate"	

#define STORED_CHARACTER_HEROINFOUPDATE		"dbo.MP_CHARACTER_HeroInfoUpdate"
#define STORED_CHARACTER_TOTALINFOUPDATE	"dbo.MP_CHARACTER_TotalInfoUpdate"
#define STORED_CHARACTER_BADFAMEUPDATE		"dbo.MP_CHARACTER_BadFameUpdate"
#define STORED_CHARACTER_REGISTLOGINMAPINFO "dbo.MP_LogInMapInfo_Regist"				
#define STORED_CHARACTER_UNREGISTLOGINMAPINFO "dbo.MP_LogInMapInfo_UnRegist"		
#define STORED_CHARACTER_SAVEINFOBEFORELOGOUT "dbo.MP_CHARACTER_SaveInfoBeforeLogOut"

#define STORED_ITEM_UPDATE					"dbo.MP_ITEM_Update"
#define STORED_ITEM_COMBINEUPDATE			"dbo.MP_ITEM_CombineUpdate"
#define STORED_ITEM_MOVEUPDATE_INVEN		"dbo.MP_ITEM_MoveUpdate"
#define STORED_ITEM_MOVEUPDATE_PYOGUK		"dbo.MP_ITEM_MoveUpdatePyoguk"
#define STORED_ITEM_MOVEUPDATE_GUILD		"dbo.MP_ITEM_MoveUpdateMunpa"
#define STORED_ITEM_INSERT_INVEN			"dbo.MP_ITEM_Insert"
#define STORED_ITEM_DELETE					"dbo.MP_ITEM_Delete"
#define STORED_ITEM_RARE_OPTION_INSERT		"dbo.MP_ITEM_RARE_OPTION_Insert"
#define STORED_ITEM_RARE_OPTION_DELETE		"dbo.MP_ITEM_RARE_OPTION_Delete"
#define STORED_ITEM_OPTION_INSERT			"dbo.MP_ITEM_OPTION_Insert"
#define STORED_ITEM_OPTION_DELETE			"dbo.MP_ITEM_OPTION_Delete"

#define STORED_ITEM_STONE_INSERT			"dbo.MP_ITEM_STONE_Insert" 

#define STORED_ITME_STONE_UPDATE			"dbo.MP_ITEM_STONE_Update" 

#define STORED_ITEM_UPDATE_LOCK				"dbo.MP_ITEM_Update_Lock"

#define STORED_ITEM_UPDATE_UNLOCK			"dbo.MP_ITEM_Update_UnLock"

#define STORED_ITEM_UPDATE_GROW             "dbo.MP_ITEM_Update_Grow"

#define STORED_GAMBLE_USER_INFO_INSERT      "dbo.MP_GambleUser_Insert"

#define STORED_GAMBLE_USER_INFO_SELECT      "dbo.MP_GambleUser_Select"

#define STORED_GAMBLE_INFO_SELECT			"dbo.MP_Gamble_Select"

#define STORED_MUGONG_UPDATE				"dbo.MP_MUGONG_Update"
#define STORED_MUGONG_MOVEUPDATE			"dbo.MP_MUGONG_MoveUpdate"
#define STORED_MUGONG_INSERT				"dbo.MP_MUGONG_Insert"
#define STORED_MUGONG_DELETE				"dbo.MP_MUGONG_Delete"


#define STORED_AUCTION_CHECK		"dbo.MP_AUCTION_CHECK"

#define STORED_AUCTION_SEARCH		"dbo.MP_AUCTION_SEARCH"

#define STORED_AUCTION_SORT			"dbo.MP_AUCTION_SORT"

#define STORED_AUCTION_REGISTER		"dbo.MP_AUCTION_REGISTER"

#define STORED_AUCTION_JOIN			"dbo.MP_AUCTION_JOIN"

#define STORED_AUCTION_CANCEL		"dbo.MP_AUCTION__REGISTER_CANCEL"

#define STORED_PARTY_REGIST_MAPSERVER	"dbo.MP_PARTY_Regist"
#define STORED_PARTY_INFO_BY_USERLOGIN	"dbo.MP_PARTY_PartyInfoByUserLogin"
#define STORED_PARTY_CREATE				"dbo.MP_PARTY_CreateParty"
#define STORED_PARTY_BREAK				"dbo.MP_PARTY_BreakupParty"
#define STORED_PARTY_CHARACTERUPDATE	"dbo.MP_PARTY_DelPartyidxinCharacterTB"
#define STORED_PARTY_UPDATE				"dbo.MP_PARTY_UpdateMember"
#define STORED_PARTY_ADDMEMBER			"dbo.MP_PARTY_AddMember"
#define STORED_PARTY_DELMEMBER			"dbo.MP_PARTY_DelMember"
#define STORED_PARTY_CHANGEMASTER		"dbo.MP_PARTY_ChangeMaster"

#define STORED_GUILD_CREATE				"dbo.MP_GUILD_Create"
#define STORED_GUILD_BREAKUP			"dbo.MP_GUILD_BreakUp"
#define STORED_GUILD_DELETEMEMBER		"dbo.MP_GUILD_DeleteMember"
#define STORED_GUILD_ADDMEMBER			"dbo.MP_GUILD_AddMember"
#define STORED_GUILD_LOADGUILD			"dbo.MP_GUILD_LoadGuild"
#define STORED_GUILD_LOADNOTICE			"dbo.MP_GUILD_LoadNotice"
#define STORED_GUILD_UPDATENOTICE		"dbo.MP_GUILD_UpdateNotice"
#define STORED_GUILD_LOADMEMBER			"dbo.MP_GUILD_LoadMember"
#define STORED_GUILD_MARKREGIST			"dbo.MP_GUILD_MarkRegist"	
#define STORED_GUILD_MARKUPDATE			"dbo.MP_GUILD_MarkUpdate"
#define STORED_GUILD_LOADMARK			"dbo.MP_GUILD_LoadMark"
#define STORED_GUILD_LEVELUP			"dbo.MP_GUILD_LevelUp"
#define STORED_GUILD_CHANGRANK			"dbo.MP_GUILD_ChangeRank"
#define STORED_GUILD_LOADITEM			"dbo.MP_GUILD_LoadItem"
#define STORED_GUILD_GIVENICKNAME		"dbo.MP_GUILD_GiveMemberNickName"
#define STORED_GUILD_MONEYUPDATE		"dbo.MP_GUILD_MoneyUpdate"
#define STORED_GUILD_ITEMOPTION			"dbo.MP_GUILD_ItemOption_Info"
#define STORED_GUILD_MARKREGIST			"dbo.MP_GUILD_MarkRegist"

#define STORED_GUILD_TRAINEE_INFO		"dbo.MP_GUILD_TRAINEE_Info"
#define STORED_GUILD_TRAINEE_INSERT		"dbo.MP_GUILD_TRAINEE_Insert"
#define STORED_GUILD_TRAINEE_DELETE		"dbo.MP_GUILD_TRAINEE_Delete"

#define STORED_GUILD_TRAINEE_GUILDINFO			"dbo.MP_GUILD_TRAINEE_GuildInfo"
#define STORED_GUILD_TRAINEE_GUILDINSERT		"dbo.MP_GUILD_TRAINEE_GuildInsert"
#define STORED_GUILD_TRAINEE_GUILDDELETE		"dbo.MP_GUILD_TRAINEE_GuildDelete"

#define STORED_GUILD_TRAINEE_DELETEBYGUILD		"dbo.MP_GUILD_TRAINEE_DeletebyGuild"
#define STORED_GUILD_ADD_STUDENT				"dbo.MP_GUILD_AddStudent"

#define STORED_GUILD_TRAINEE_GUILDINTROINFO		"dbo.MP_GUILD_TRAINEE_GuildIntroInfo"
#define  STORED_GUILD_TRAINEE_GUILDINTROUPDATE	"dbo.MP_GUILD_TRAINEE_GuildIntroUpdate"

#define	STORED_TITAN_WEARITEMINFO	"dbo.MP_TITAN_WearItemInfo"	

#define STORED_PYOGUK_BUYPYOGUK			"dbo.MP_PYOGUK_Buy"
#define STORED_PYOGUK_MONEYUPDATE		"dbo.MP_PYOGUK_MoneyUpdate"
#define STORED_PYOGUK_TITAN_ENDURANCE_INFO	"dbo.MP_PYOGUK_Titan_Endurance_Info"	

#define STORED_PYOGUK_INFO				"dbo.MP_PYOGUK_Info"
#define STORED_PYOGUK_ITEMINFO			"dbo.MP_PYOGUK_ItemInfo"
#define STORED_PYOGUK_MONEYUPDATE		"dbo.MP_PYOGUK_MoneyUpdate"

#define STORED_FRIEND_NOTIFYLOGOUT		"dbo.MP_FRIEND_NotifyLogout"

#define STORED_SHOPINVEN_INFO			"dbo.MP_SHOPITEM_InvenInfo"
#define STORED_SHOPITEM_INFO			"dbo.MP_SHOPITEM_ItemInfo"
#define STORED_SHOPITEM_USEINFO			"dbo.MP_SHOPITEM_UseInfo"
#define STORED_SHOPITEM_USING			"dbo.MP_SHOPITEM_Using"
#define STORED_SHOPITEM_USING_JP		"dbo.MP_SHOPITEM_Using_JP"
#define STORED_SHOPITEM_DELETE			"dbo.MP_SHOPITEM_Delete"
#define STORED_SHOPITEM_UPDATETIME		"dbo.MP_SHOPITEM_Updatetime"
#define STORED_SHOPITEM_UPDATEPARAM		"dbo.MP_SHOPITEM_UpdateParam"
#define STORED_SHOPITEM_UPDATEUSEPARAM	"dbo.MP_SHOPITEM_UpdateUseParam"
#define STORED_SHOPITEM_UPDATEUSEPARAM_JP	"dbo.MP_SHOPITEM_UpdateUseParam_JP"
#define STORED_SHOPITEM_GETITEM			"dbo.MP_SHOPITEM_GetItem"
#define STORED_ITEM_MOVEUPDATE_SHOP		"dbo.MP_ITEM_MoveUpdateShop"
#define STORED_SHOPITEM_CHANGENAME		"dbo.MP_character_rename"
#define STORED_SHOPITEM_CHARCHANGE		"dbo.MP_SHOPITEM_CharChange"
#define STORED_SHOPITEM_MPINFO			"dbo.MP_MOVEPOINT_GetInfo"
#define STORED_SHOPITEM_MPINSERT		"dbo.MP_MOVEPOINT_Insert"
#define STORED_SHOPITEM_MPUPDATE		"dbo.MP_MOVEPOINT_Update"
#define STORED_SHOPITEM_MPDEL			"dbo.MP_MOVEPOINT_Delete"
#define STORED_SHOPITEM_UPDATEUSEINFO	"dbo.MP_SHOPITEM_UpdateUseInfo"
#define STORED_SHOPITEM_AVATARABLE		"dbo.MP_SHOPITEM_IsAbleAvatar"
#define STORED_SHOPITEM_EQUIPABLE		"dbo.MP_SHOPITEM_IsAbleEquip"
#define STORED_SHOPITEM_UPDAETALL		"dbo.MP_SHOPITEM_UpdateAll"

#define STORED_FAME_CHARACTERUPDATE		"dbo.MP_FAME_CharacterUpdate"

#define STORED_BADFAME_CHARACTERUPDATE	"dbo.MP_BADFAME_CharacterUpdate"
#define STORED_PK_CHARACTERUPDATE		"dbo.MP_PK_CharacterUpdate"

#define STORED_WANTED_LOAD				"dbo.MP_WANTED_LoadList"
#define STORED_WANTED_INFO				"dbo.MP_WANTED_InfoByUserLogIn"
#define STORED_WANTED_BUYRIGHT			"dbo.MP_WANTED_BuyRight"
#define STORED_WANTED_REGIST			"dbo.MP_WANTED_Regist"
#define STORED_WANTED_GIVEUP			"dbo.MP_WANTED_GiveUpRight"
#define STORED_WANTED_COMPLETE			"dbo.MP_WANTED_Complete"
#define STORED_WANTED_DESTROYED			"dbo.MP_WANTED_Destroyed"
#define STORED_WANTED_ORDERLIST			"dbo.MP_WANTED_OrderList"
#define STORED_WANTED_MYLIST			"dbo.MP_WANTED_MyList"
#define STORED_WANTED_SEARCH			"dbo.MP_WANTED_Search"

#define STORED_QUEST_MAINQUEST_LOAD		"dbo.MP_MAINQUEST_Load"
#define STORED_QUEST_MAINQUEST_INSERT	"dbo.MP_MAINQUEST_Insert"
#define STORED_QUEST_MAINQUEST_DELETE	"dbo.MP_MAINQUEST_Delete"
#define STORED_QUEST_MAINQUEST_UPDATE	"dbo.MP_MAINQUEST_Update"
#define STORED_QUEST_SUBQUEST_LOAD		"dbo.MP_SUBQUEST_Load"
#define STORED_QUEST_SUBQUEST_INSERT	"dbo.MP_SUBQUEST_Insert"
#define STORED_QUEST_SUBQUEST_DELETE_ALL "dbo.MP_SUBQUEST_Delete_All"
#define STORED_QUEST_SUBQUEST_DELETE	"dbo.MP_SUBQUEST_Delete"
#define STORED_QUEST_SUBQUEST_UPDATE	"dbo.MP_SUBQUEST_Update"
#define STORED_QUEST_ITEM_LOAD			"dbo.MP_QUESTITEM_Load"
#define STORED_QUEST_ITEM_INSERT		"dbo.MP_QUESTITEM_Insert"
#define STORED_QUEST_ITEM_DELETE		"dbo.MP_QUESTITEM_Delete"
#define STORED_QUEST_ITEM_DELETE_ALL	"dbo.MP_QUESTITEM_Delete_All"
#define STORED_QUEST_ITEM_UPDATE		"dbo.MP_QUESTITEM_Update"

#define STORED_JOURNAL_GETLIST			"dbo.MP_JOURNAL_GetList"
#define STORED_JOURNAL_INSERTQUEST		"dbo.MP_JOURNAL_InsertQuest"
#define STORED_JOURNAL_INSERTWANTED		"dbo.MP_JOURNAL_InsertWanted"
#define STROED_JOURNAL_INSERTLEVEL		"dbo.MP_JOURNAL_InsertLevel"
#define STROED_JOURNAL_SAVEUPDATE		"dbo.MP_JOURNAL_SaveUpdate"
#define STROED_JOURNAL_DELETE			"dbo.MP_JOURNAL_Delete"

#define STORED_MAPCHANGE_CHARINFO		"dbo.MP_MAPCHANGE_CharInfo"
#define STORED_MAPCHANGE_MUGONGINFO		"dbo.MP_CHARACTER_MugongInfo"

#define STORED_SAVE_MAPCHAGEPOINT		"dbo.MP_CHARACTER_MapchangePointUpdate"
#define STORED_SAVE_MAPCHAGEPOINTRET	"dbo.MP_CHARACTER_MapchangePointReturn"
#define STORED_SAVE_LOGINPOINT			"dbo.MP_CHARACTER_LoginPointUpdate"
#define STORED_MAP_SAVEPOINT			"dbo.MP_MAP_SavePoint"

#define STORED_GT_LOADALL				"dbo.MP_GT_LoadAll"
#define STORED_GT_CANCELLOAD			"dbo.MP_GT_CancelLoad"
#define STORED_GT_CANCELINSERT			"dbo.MP_GT_CancelInsert"
#define STORED_GT_TOURNAMENTINFO_UPDATE	"dbo.MP_GT_TrInfoUpdate"
#define STORED_GT_GUILDINFO_INSERT		"dbo.MP_GT_Insert"
#define STORED_GT_GUILDINFO_DELETE		"dbo.MP_GT_Delete"
#define STORED_GT_GUILDINFO_UPDATE		"dbo.MP_GT_Update"

#define STORED_SIEGE_INFOLOAD			"dbo.MP_SIEGEWAR_InfoLoad"
#define STORED_SIEGE_INFOINSERT			"dbo.MP_SIEGEWAR_InfoInsert"
#define STORED_SIEGE_INFOUPDATE			"dbo.MP_SIEGEWAR_InfoUpdate"
#define STORED_SIEGE_GUILDLOAD			"dbo.MP_SIEGEWAR_GuildInfoLoad"
#define STORED_SIEGE_GUILDINSERT		"dbo.MP_SIEGEWAR_GuildInfoInsert"
#define STORED_SIEGE_GUILDUPDATE		"dbo.MP_SIEGEWAR_GuildInfoUpdate"
#define STORED_SIEGE_GUILDDELETE		"dbo.MP_SIEGEWAR_GuildInfoDelete"

#define STORED_CHARACTER_SKININFO		"dbo.MP_CHARACTER_UpdateSkinInfo"

#define STORED_CHARACTER_UPDATE_RESETSTATUSPOINT	"dbo.MP_CHARACTER_UpdateResetStatusPoint"

//weiye 2017-12-07      安全码相关存储过程定义 

#define STORED_SAFEPIN_CHECK     "dbo.MP_SAFEPIN_CHECK"

#define STORED_SAFEPIN_CHANGE	 "dbo.MP_SAFEPIN_CHANGE"

#define STORED_SAFEPIN_UPDATE    "dbo.MP_SAFEPIN_UPDATE"

//weiye 2018-02-06      FB副本时间更新存储过程定义

#define STORED_FBTIME_UPDATE      "dbo.MP_FBTIME_Update"

//weiye 2018-03-15  包袱物品查询存储过程定义

#define STORED_ITEM_SELECT       "dbo.MP_CHARACTER_ItemInfo_Select"

//weiye 2018-03-26   整理包袱数据库存储过程定义

#define STORED_ITEM_TIDY	     "dbo.JACK_ITEM_TIDY"
#define STORED_ITEM_TIDY_SHOP	"dbo.JACK_ITEM_TIDY_SHOP"


#define STORED_CHARACTER_REBORN		"dbo.JACK_REBORN"

#define STORED_CLEAR_FORTWAR_INFO	"dbo.TB_FORTWARINFO"

#define STORED_GTRANK_UPDATE		"dbo.JACK_GTRANK_UPDATE"
#define STORED_CLEAR_GTRANK_INFO	"dbo.JACK_GTRANK_CLEAR"
#define STORED_NPCCODE				"dbo.MP_NPCCODE"

#define STORED_ITEM_FLASH1_UPDATE "dbo.MP_ITEM_FLASHNAME1_UPDATE"
#define STORED_ITEM_FLASH2_UPDATE "dbo.MP_ITEM_FLASHNAME2_UPDATE"


#define STORED_LASTMAP_UPDATE    "dbo.JACK_LASTMAP_UPDATE"

#define STORED_LOGINEVENT_UPDATE		"dbo.JACK_LOGINEVENT_UPDATE"

enum AuctionPage
{
	eAT_DBIdx, eAT_ItemDBIdx, eAT_ItemIdx, eAT_SellNum,  eAT_SellerID, eAT_EndDate, eAT_EndTime,
	eAT_StartPrice, eAT_ImmediatePrice, eAT_CurrentPrice, eAT_BidderNum, eAT_CurrentBidder, eAT_RegDate
};
enum AuctionList
{
	ePRO_NAME, ePRO_Amount, eBID_High_Price, eBID_Immediate_Price, eBID_DueDate, eHigh_Price_Name, eJOIN_Amount, 
	eBID_Left_Time, eAuctioneer,ePRO_Index ,eBID_Starttime,eREG_Index, 
};

enum AuctionSuccess
{
	sePRO_NAME, sePRO_Amount, seAuctioneer,
	seREG_Index

};
enum AuctionConfirm
{
	bAuctionConfirm
};

enum CHSelectInfo
{
	eCS_ObjectID = 0, eCS_UserID, eCS_PartyID,eCS_Gender, eCS_Gengoal, eCS_Life, eCS_Shield, eCS_Energe, eCS_Dex, eCS_Sta, eCS_Simmak, eCS_Fame, 
	eCS_Karma, eCS_Name, eCS_Repute, eCS_Stage, eCS_Grade, eCS_Expoint, eCS_GradeUpPoint, eCS_Money, 
	eCS_QuickPace, eCS_Vitality, eCS_Map, eCS_FaceType, eCS_HeadType, eCS_Hat, eCS_Weapon, eCS_Dress, eCS_Shoes, 
	eCS_Ring1, eCS_Ring2, eCS_Cape, eCS_Necklace, eCS_Armlet, eCS_Belt, eCS_MunpaIDX, eCS_GuildPositon, eCS_GuildName, eCS_BadFame,
	eCS_MapChangePoint, eCS_LoginPoint, eCS_AbilityExp, eCS_Height, eCS_Width, eCS_Playtime, eCS_LastPKEndTime, eCS_MaxLevel, eCS_MarkName, eCS_MunpaCanEntryDate,
	eCS_NickName, eCS_ExpFlag,eCS_SkinInfo,eCS_MallMoney,eCS_GoldMoney,eCS_ReSet,eCS_MarryName,eCS_ShiTuId,eCS_ShiTuName,eCS_PdTime,eCS_FNameFlag,
	eCsSafe/*,eCS_FBTime*/,eCsFName,
	eCS_TurnOne,eCS_TurnTwo,eCS_TurnThree,

	eCS_LoginEventTime,eCS_LoginEventItem,

	eCS_KillPlayerTimes,

	eCS_SkillSlot
	//eCS_ForbidChat,
};

enum CHMugong	
{
	eCM_ObjectID = 0, eCM_DBIDX, eCM_IDX, eCM_ExpPoint, eCM_Sung, eCM_Position, eCM_QPosition, eCM_Wear, eCM_Option
};

enum CHItem	
{
	eCI_ObjectID = 0, eCI_DBIDX, eCI_IDX, eCI_Position, eCI_QPosition, eCI_Durability, eCI_Param, eCI_RareIdx,eCI_StoneIdx, eCI_Static,eCI_Grow,
	eCI_PowerUp,eCI_Green, eCI_Grade30,  eCI_Quality, eCI_Entry1, eCI_Entry2, eCI_Entry3//1
};

enum CHItemRare
{
	eCIR_ObjectID, eCIR_ItemIdx, eCIR_Position, eCIR_RareID, eCIR_ItemDBID,
	eCIR_GenGol, eCIR_MinChub, eCIR_CheRyuk, eCIR_SimMek, eCIR_Life, eCIR_NaeRyuk, eCIR_Shield, 
	eCIR_FireAttrRegist, eCIR_WaterAttrRegist, eCIR_TreeAttrRegist,  eCIR_GoldAttrRegist, eCIR_EarthAttrRegist,
	eCIR_PhyAttack,
	eCIR_FireAttrAttack, eCIR_WaterAttrAttack, eCIR_TreeAttrAttack,  eCIR_GoldAttrAttack, eCIR_EarthAttrAttack,
	eCIR_PhyDefense,
	eCIR_Green//2
};

enum CHItemOption
{
	eCIO_ObjectID, eCIO_ItemIdx, eCIO_OptionID, eCIO_ItemDBID, eCIO_GenGol, eCIO_MinChub, eCIO_CheRyuk, eCIO_SimMek, eCIO_Life, eCIO_NaeRyuk, eCIO_Shield, 
	eCIO_FireAttrRegist, eCIO_WaterAttrRegist, eCIO_TreeAttrRegist,  eCIO_GoldAttrRegist, eCIO_EarthAttrRegist,
	eCIO_PhyAttack, eCIO_Critical, 
	eCIO_FireAttrAttack, eCIO_WaterAttrAttack, eCIO_TreeAttrAttack,  eCIO_GoldAttrAttack, eCIO_EarthAttrAttack,
	eCIO_PhyDefense,
	eCIO_PowerUp//2
};

enum CHItemRareOptionInfo
{
	eCIRO_ObjectID,  eCIRO_RareOptionID, eCIRO_ItemDBID, eCIRO_GenGol, eCIRO_MinChub, eCIRO_CheRyuk, eCIRO_SimMek, eCIRO_Life, eCIRO_NaeRyuk, eCIRO_Shield, 
	eCIRO_FireAttrRegist, eCIRO_WaterAttrRegist, eCIRO_TreeAttrRegist,  eCIRO_GoldAttrRegist, eCIRO_EarthAttrRegist,
	eCIRO_PhyAttack, 
	eCIRO_FireAttrAttack, eCIRO_WaterAttrAttack, eCIRO_TreeAttrAttack,  eCIRO_GoldAttrAttack, eCIRO_EarthAttrAttack,
	eCIRO_PhyDefense,

	eCIRO_Green,//3
	eCIRO_PowerUp//3
};
enum CHItemOptionInfo
{
	eCIOI_ObjectID, eCIOI_OptionID, eCIOI_ItemDBID, eCIOI_GenGol, eCIOI_MinChub, eCIOI_CheRyuk, eCIOI_SimMek, eCIOI_Life, eCIOI_NaeRyuk, eCIOI_Shield, 
	eCIOI_FireAttrRegist, eCIOI_WaterAttrRegist, eCIOI_TreeAttrRegist,  eCIOI_GoldAttrRegist, eCIOI_EarthAttrRegist,
	eCIOI_PhyAttack, eCIOI_Critical, 
	eCIOI_FireAttrAttack, eCIOI_WaterAttrAttack, eCIOI_TreeAttrAttack,  eCIOI_GoldAttrAttack, eCIOI_EarthAttrAttack,
	eCIOI_PhyDefense,

	eCIOI_PowerUp//4
};

enum CPyogukItemRareOptionInfo
{

	ePROI_RareOptionID, ePROI_ItemDBID, ePROI_GenGol, ePROI_MinChub, ePROI_CheRyuk, ePROI_SimMek, ePROI_Life, ePROI_NaeRyuk, ePROI_Shield, 
	ePROI_FireAttrRegist, ePROI_WaterAttrRegist, ePROI_TreeAttrRegist,  ePROI_GoldAttrRegist, ePROI_EarthAttrRegist,
	ePROI_PhyAttack,
	ePROI_FireAttrAttack, ePROI_WaterAttrAttack, ePROI_TreeAttrAttack,  ePROI_GoldAttrAttack, ePROI_EarthAttrAttack,
	ePROI_PhyDefense 
};

enum CPyogukItemOptionInfo
{
	ePOI_OptionID, ePOI_ItemDBID, ePOI_GenGol, ePOI_MinChub, ePOI_CheRyuk, ePOI_SimMek, ePOI_Life, ePOI_NaeRyuk, ePOI_Shield, 
	ePOI_FireAttrRegist, ePOI_WaterAttrRegist, ePOI_TreeAttrRegist,  ePOI_GoldAttrRegist, ePOI_EarthAttrRegist,
	ePOI_PhyAttack, ePOI_Critical, 
	ePOI_FireAttrAttack, ePOI_WaterAttrAttack, ePOI_TreeAttrAttack,  ePOI_GoldAttrAttack, ePOI_EarthAttrAttack,
	ePOI_PhyDefense 
};
enum CMunpaItemOptionInfo
{
	eMOI_OptionID, eMOI_ItemDBID, eMOI_GenGol, eMOI_MinChub, eMOI_CheRyuk, eMOI_SimMek, eMOI_Life, eMOI_NaeRyuk, eMOI_Shield, 
	eMOI_FireAttrRegist, eMOI_WaterAttrRegist, eMOI_TreeAttrRegist,  eMOI_GoldAttrRegist, eMOI_EarthAttrRegist,
	eMOI_PhyAttack, eMOI_Critical, 
	eMOI_FireAttrAttack, eMOI_WaterAttrAttack, eMOI_TreeAttrAttack,  eMOI_GoldAttrAttack, eMOI_EarthAttrAttack,
	eMOI_PhyDefense 
};

enum CGuildItemRareOptionInfo
{
	eGORI_RareOptionID, eGORI_ItemDBID, eGORI_GenGol, eGORI_MinChub, eGORI_CheRyuk, eGORI_SimMek, eGORI_Life, eGORI_NaeRyuk, eGORI_Shield, 
	eGORI_FireAttrRegist, eGORI_WaterAttrRegist, eGORI_TreeAttrRegist,  eGORI_GoldAttrRegist, eGORI_EarthAttrRegist,
	eGORI_PhyAttack,
	eGORI_FireAttrAttack, eGORI_WaterAttrAttack, eGORI_TreeAttrAttack,  eGORI_GoldAttrAttack, eGORI_EarthAttrAttack,
	eGORI_PhyDefense
};

enum eGuildItemOptionInfo
{
	eGIOI_GuildIdx,
	eGIOI_OptionID, eGIOI_ItemDBID, eGIOI_GenGol, eGIOI_MinChub, eGIOI_CheRyuk, eGIOI_SimMek, eGIOI_Life, eGIOI_NaeRyuk, eGIOI_Shield, 
	eGIOI_FireAttrRegist, eGIOI_WaterAttrRegist, eGIOI_TreeAttrRegist,  eGIOI_GoldAttrRegist, eGIOI_EarthAttrRegist,

	eGIOI_PhyAttack, eGIOI_Critical, 
	eGIOI_FireAttrAttack, eGIOI_WaterAttrAttack, eGIOI_TreeAttrAttack,  eGIOI_GoldAttrAttack, eGIOI_EarthAttrAttack,
	eGIOI_PhyDefense
};

enum eGuildItemRareOptionInfo
{
	eGIROI_GuildIdx,
	eGIROI_RareOptionID, eGIROI_ItemDBID, eGIROI_GenGol, eGIROI_MinChub, eGIROI_CheRyuk, eGIROI_SimMek, eGIROI_Life, eGIROI_NaeRyuk, eGIROI_Shield, 
	eGIROI_FireAttrRegist, eGIROI_WaterAttrRegist, eGIROI_TreeAttrRegist,  eGIROI_GoldAttrRegist, eGIROI_EarthAttrRegist,
	eGIROI_PhyAttack,
	eGIROI_FireAttrAttack, eGIROI_WaterAttrAttack, eGIROI_TreeAttrAttack,  eGIROI_GoldAttrAttack, eGIROI_EarthAttrAttack,
	eGIROI_PhyDefense
};

enum eGuildTraineeInfoIndex
{
    eGTII_DBIdx, eGTII_GuildIdx, eGTII_UserIdx, eGTII_UserName, eGTII_JoinTime
};

enum eGuildTraineeGuildInfoIndex
{
	eGTGII_DBIdx, eGTGII_GuildIdx, eGTGII_GuildRank
};

enum eGuildAddStudentIndex
{
	eGASI_CharIdx, eGASI_Level, eGASI_Name, eGASI_GuildIdx
};

enum eGuildTraineeGuildIntroInfoIndex
{
    eGTGIII_DBIdx, eGTGIII_Intro,
};

enum CPetInvenItemOptionInfo
{
	ePIOI_OptionID, ePIOI_ItemDBID, ePIOI_GenGol, ePIOI_MinChub, ePIOI_CheRyuk, ePIOI_SimMek, ePIOI_Life, ePIOI_NaeRyuk, ePIOI_Shield, 
	ePIOI_FireAttrRegist, ePIOI_WaterAttrRegist, ePIOI_TreeAttrRegist,  ePIOI_GoldAttrRegist, ePIOI_EarthAttrRegist,
	ePIOI_PhyAttack, ePIOI_Critical, 
	ePIOI_FireAttrAttack, ePIOI_WaterAttrAttack, ePIOI_TreeAttrAttack,  ePIOI_GoldAttrAttack, ePIOI_EarthAttrAttack,
	ePIOI_PhyDefense
};

enum CPetInvenItemRareOptionInfo
{
	ePIROI_RareOptionID, ePIROI_ItemDBID, ePIROI_GenGol, ePIROI_MinChub, ePIROI_CheRyuk, ePIROI_SimMek, ePIROI_Life, ePIROI_NaeRyuk, ePIROI_Shield, 
	ePIROI_FireAttrRegist, ePIROI_WaterAttrRegist, ePIROI_TreeAttrRegist,  ePIROI_GoldAttrRegist, ePIROI_EarthAttrRegist,
	ePIROI_PhyAttack,
	ePIROI_FireAttrAttack, ePIROI_WaterAttrAttack, ePIROI_TreeAttrAttack,  ePIROI_GoldAttrAttack, ePIROI_EarthAttrAttack,
	ePIROI_PhyDefense	
};

enum CPetTotalInfo
{
	ePTI_PetDBIdx, ePTI_UserID, ePTI_PetSummonItemDBIdx, ePTI_PetKind, ePTI_PetGrade, ePTI_PetStamina, ePTI_PetFriendship, 
	ePTI_Alive,  ePTI_Summonning, ePTI_Rest,
};

enum CPetInsertInfo
{
	ePII_PetSummonItemIdx, ePII_PetDBIdx, ePII_MasterObjectID, ePII_UserIdx, ePII_PetSummonItemDBIdx, ePII_PetKind, ePII_PetGrade, ePII_PetStamina, ePII_PetFriendship, 
	ePII_Alive, ePII_Summonning, ePII_Rest,
};

enum GuildPointInfoInit
{
	eGPII_GuildDBIdx, eGPII_GuildIdx, eGPII_GuildPoint, eGPII_GuildHuntedMonsterTotalCount, eGPII_EndTimePerGuildPlustimeKind,
};

enum eGuildHuntedMonsterCount
{
	eGHMC_TotalCount, eGHMC_DB_Date,
};

enum GuildConvertHuntedMonsterCountWithInit
{
	eGCHMC_TotalCount, eGCHMC_DB_Date,
};

enum Guild_AddGuildPoint
{
	eGAGP_AddPoint, eGAGP_TotalPoint, eGAGP_AddKind, eGAGP_AdditionalData, 
};

enum Guild_UseGuildPoint
{
	eGUGP_UsePoint, eGUGP_TotalPoint, eGUGP_UseKind, eGUGP_KindIdx, eGUGP_Endtime,
};

enum CHSkill
{
	eCK_ObjectID = 0, eCK_DBIDX, eCK_IDX, eCK_Position, eCK_QPosition, eCK_Durability
};

enum CHItemGroundDrop	
{
	eIG_ObjectID, eIG_DBIDX, eIG_IDX, eIG_POSX, eIG_POSZ
};

enum MMapBaseEconomy		
{
	eME_MEPNUM, eME_ORIGINNUM, eME_ORIGINPRICE, eME_ORIGINAMOUNT, eME_REQUIRENUM, eME_REQUIREPRICE, eME_REQUIREAMOUNT,
	eME_BUYRATES, eME_SELLRATES 
};

enum PartyInfoMap
{
	ePA_IDX, 
};
enum Partyinfobyuserlogin
{
	ePA_LIMemberId, ePA_LIMemberName, ePA_LIMemberCurrentMap, ePA_LIMemberLevel, ePA_LIOption,
	//2008. 5. 22. CBH - 规颇 眠啊 可记 包访 荐沥
	ePA_MinLevel, ePA_MaxLevel, ePA_Public, ePA_LimitCount, ePA_Theme,
};
enum Partychangemaster
{
	ePA_CMErr, ePA_CMTargetID,
};

enum PartyMember 
{
	 ePA_ERR,ePA_PARTYID, ePA_MASTER, ePA_OPTION,
	 //2008. 5. 22. CBH - 规颇 眠啊 可记 包访 荐沥
	 ePA_MINLEVEL, ePA_MAXLEVEL, ePA_PUBLIC, ePA_LIMITCOUNT, ePA_THEME,
};
enum Partydelmember
{
	ePA_DMErr, ePA_DMTargetID,
};
enum Munpainfo //E々o∑ AAUCO ⒁∽ ōA ㈢āA dì 
{
	eMu_MunpaID, eMunpaMasterID, eMu_MunpaName, eFamous, eMemberNum, eMunpaRank, eMunpaMoney
};

enum Munpaloadsyn
{
	eMu_MLStotalnum, eMu_MLSmunpaid, eMu_MLSmunpaname, eMu_MLSmastername, eMu_MLSmunpafamous, eMu_MLSmembernum, eMu_MLSKind
};
enum Munpacreate
{
	eMu_MCErr, eMu_MCMunpaid, eMu_MCMasterid, eMu_MCMunpaname, eMu_MCMunpaRank, eMu_MCMasterFame
};
enum Munpabreakup
{
	eMu_MBerr, eMu_MBmasterid, eMu_MBmasterfame,
};
enum Munpabreakupmember
{
	eMu_MBmemberid, eMu_MBmemberfame,
};
enum Munpahomeinfo
{
	eMu_MHIMunpaid, eMu_MHIIntroMsg, eMu_MHIMunpaKind, 
	eMu_MHIMunpaName, eMu_MHIMasterName, eMu_MHIFamous, 
	eMu_MHICreateDate, eMu_MHIMemberNum, eMu_MHIFieldWarWin, 
	eMu_MHIFieldWarLoss, eMu_MHIBuildingWarWin, eMu_MHIBuildingWarLoss, 
	eMu_MHICastleWarWin, eMu_MHICastleWarLoss, eMu_MHIBoardIDX, 
	eMu_MHIRSRank, eMu_MHIRCRank, eMu_MHIWCRank, 
	eMu_MHIDCRank, eMu_MHIBoardName, 
};
enum Munpamembersinfo
{
	eMu_MMTotalNum, eMu_MMunpaid, eMu_MMemberid, 
	eMu_MMemberName, eMu_MMemberRank, eMu_MMemberLevel, 
	eMu_MMemberFamous, eMu_MMember_LastVisitDay, eMu_MMember_EntryDay, 
	eMu_MMember_bLogin,
};
enum Munpachangememberank
{
	eMu_MCRchangeerr, eMu_MCRtargetid, eMu_MCRmunpaid, eMu_MCRposition, eMu_MCRtargetname, eMu_MCRfame, 
};
enum Munpabanmember
{
	eMu_MBMerr, eMu_MBMtargetid, eMu_MBMmunpaid, eMu_MBMblast, eMu_MBMtargetname, eMu_MBMfame, 
};
enum Munpadenymember
{
	eMu_MDMerr,  eMu_MDMtargetid, eMu_MDMmunpaid, eMu_MDMbLast, eMu_MDMtargetname,  
};
enum Munpaacceptmember
{
	eMu_MACerr, eMu_MACmunpaid, eMu_MACtargetid, eMu_MACblast, eMu_MACtargetname, eMu_MACMunpaname
};
enum MunpaBattleInfo
{
	eMu_MMunpaName, eMu_MMasterName, eMu_MMunpaFamous, eMu_MMemberNum, eMu_MCreateDate,
	eMu_MFBattleTotal, eMu_MBBattleTotal, eMu_MCBattleTotal, eMu_MTBattleTotal,
	eMu_MFBattleWin, eMu_MBBattleWin, eMu_MCBattleWin, eMu_MTBattleWin, 
	eMu_MFBattleLoss, eMu_MBBattleLoss, eMu_MCBattleLoss, eMu_MTBattleLoss
};
enum Munpajoinmemberinfo
{
	eMu_MJMTotalNum, eMu_MJMMunpaid, eMu_MJMCharacterID, 
	eMu_MJMCharacterName, eMu_MJMCharacterLevel, eMu_MJMCharacterFame, 
	eMu_MJMRequestJoinDay
};
enum Munpaboardinfo
{
	eMu_MCurBoardNum, eMu_MBoardsIdx, eMu_MBoardsName, eMu_MTotalNum,
};

enum Munpaitem
{
	eMu_IMunpaID, eMu_IDBIDX, eMu_IIDX, eMu_IPosition, eMu_IDurability, eMu_IRareIdx,eMu_IStoneIdx,eMu_IStatic,eMu_IGrow, eMu_Grade30,  eMu_Quality, eMu_Entry1, eMu_Entry2, eMu_Entry3 //weiye 2015-10-25 仓库和成长数据
};

enum Munpacreateboardinfo
{
	eMu_MBoardidx, eMu_MRSubjectRank, eMu_MRContentRank, eMu_MWContentRank, eMu_MDContentRank, eMu_MBoardName
};
enum Munpaboardlistinfo
{
	eMu_MBLITotalPage, eMu_MBLIContentIDx, eMu_MBLIContentNum, eMu_MBLISubject, eMu_MBLIWriter, eMu_MBLICount, eMu_MBLIRegDate, eMu_MBLIRegTime, eMu_MBLIDepth
};
enum Munpaboardcontentsinfo
{
	eMu_MBCErr, eMu_MBCIContent, eMu_MBCIContentIDx, eMu_MBCIContentNum, eMu_MBCISubject, eMu_MBCIWriter, eMu_MBCICount, eMu_MBCIRegDate, eMu_MBCDCRank, eMu_MBCWCRank
};
enum Munpamoney
{
	eMu_MWMunpaID, eMu_MWMunpaMoney, eMu_MWPutInOut,
};

enum Pyogukbuy
{
	ePyo_errbuy, ePyo_Kind
};

enum PyogukInfo
{
	ePyo_PyogukNum, ePyo_Money,
};

enum PyogukItemInfo
{
	ePI_DBIdx, ePI_IconIdx, ePI_Positon, ePI_QPosition, ePI_Durability, ePI_RareIdx,ePI_StoneIdx,ePI_Static,ePI_Grow,  // 2014-11-15 数据附加 , 数据, 成长数据

	ePI_PowerUp,ePI_Green, ePI_Grade30, ePI_Quality, ePI_Entry1, ePI_Entry2, ePI_Entry3
};

enum Pyogukitem
{
	ePyo_Userid, ePyo_MaxPosition, ePyo_Durability, ePyo_Idx, ePyo_DBIdx, ePyo_Position, 
};

enum ePetInvenItemInfo
{
	//ePIII_DBIdx, ePIII_IconIdx, ePIII_Position, ePIII_Durability, ePIII_RareIdx,
	ePIII_ObjectID = 0, ePIII_DBIDX, ePIII_IDX, ePIII_Position, ePIII_QPosition, ePIII_Durability, ePIII_MunpaIdx, ePIII_PyogukIdx, ePIII_ShopIdx, ePIII_Param, ePIII_RareIdx,ePIII_StoneIdx,ePIII_STATIC,ePIII_Grow, // 数据附加到宠物包袱! 数据，成长数据附加 2014-11-19
	ePIII_PowerUp,ePIII_Green, ePII_Grade30, ePIII_Quality, ePIII_Entry1, ePIII_Entry2, ePIII_Entry3
};

enum Friendlogout
{
	eFr_LToPlayerID, eFr_LLogoutName,
};
enum Friendnotelist
{
	eFr_NLFromName, eFr_NLNote, eFr_NLSendDate, eFr_NLSendTime, eFr_NLNoteID
};
enum Friendlist
{
	eFr_FLFriendid, eFr_FLFriendname, eFr_FLIsLogin,
};
enum Famemunpaupdate
{
	eFa_MunpaIDX, eFa_Fameval,
};
enum Wantedload
{
	eWa_LWantedIDX, eWa_LPlayerID, eWa_LPlayerName, eWa_LTotlaPrize, eWa_LRegistChridx, eWa_LTotalVolunteerNum,

};
enum Wantedregist
{
	eWa_RWanted_idx, eWa_RWantedChr_idx, eWa_RWanted_name, eWa_RTotalPrize, 
};
enum Wantedcomplete
{
	eWa_CCharacteridx, eWa_CCompleterName, eWa_CCompleteType,
};
enum Wantedorderlist
{
	eWa_OLTotal, eWa_OLWantedIDX, eWa_OLWantedChrName, eWa_OLPrize, eWa_OLVolunteerNum,
};
enum Wantedlogininfo
{
	eWa_LIMyWantedIDX, eWa_LIWantedIDX, eWa_LIWantedChrID, eWa_LIWantedName, eWa_LIRegistDate, 
};
enum QuestInfo
{
	eQi_QuestIdx, eQi_QuestState, eQi_EndParam, eQi_RegistTime, eQi_CheckType, eQi_CheckTime,
};
enum QuestSubDataInfo
{
	eQsi_QuestIdx, eQsi_SubQuestIdx, eQsi_Data, eQsi_RegistTime,
};
enum QuestItem
{
	eQit_ItemIdx, eQit_ItemCount, eQit_QuestIdx,
};
enum Journal
{
	eJnl_Index, eJnl_Kind, eJnl_Param, eJnl_Param_2, eJnl_Param_3, eJnl_bSaved, eJnl_ParamName, eJnl_RegDate,
};
enum Guildload
{
	eGu_LGIdx, eGu_LGMasterIdx, eGu_LGMasterName, eGu_LGName, eGu_LGLevel, eGu_LGLocation, eGu_LGUnionIdx, eGu_LGMoney, eGu_LGMarkName,
};
enum Guildloadmember
{
	eGu_LMIdx, eGu_LMName, eGu_LMLevel, eGu_LMGuildIdx, eGu_LMRank, //龋莫 眠啊
};
enum Guilcreate
{
	eGu_CIsSuccess, eGu_CGuildIdx, eGu_CName, eGu_CMapNum, eGu_CMasterName, eGu_CMasterlvl,
};
enum ShopItem
{
	eMItm_ItemDBIdx, eMItm_ItemIdx, eMItm_Position, eMItm_Durability, eMItm_Param,eMItm_Static,eMItm_Grow,  // 和成长附加 2015-01-14

	eMI_PowerUp,eMI_Green, eMItm_Grade30, eMItm_Quality, eMItm_Entry1, eMItm_Entry2, eMItm_Entry3
};
enum ShopItemUseInfo
{
	eUMtm_ItemIdx, eUMtm_DBIdx, eUMtm_Param, eUMtm_BTime, eUMtm_RTime, eUMtm_Position,
};
enum MovePointInfo
{
	eMPt_DBIdx, eMPt_Name, eMPt_MapNum, eMPt_MapPoint,
};
enum AvatarItemOption
{
	eAO_OptionIdx, 
};
enum eGTInfo
{
	eGT_GuildIdx, eGT_Position, eGT_Ranking, eGT_Process, eGT_WaitTime,
};
enum eSW_Info
{
	eSWI_SiegeWarIdx, eSWI_RegistTime, eSWI_SiegeWarTime, eSWI_SiegeWarLevel,
};
enum eSW_GuildInfo
{
	eSWGI_GuildDBIdx, eSWGI_GuildIdx, eSWGI_GuildType,
};

enum eGuildNotice
{
	eGu_GNDBIdx, eGu_GNGuildIdx, eGu_GNGuildNotice,
};

enum eMapDropItem	// 甘 靛而 酒捞袍 DB包访 眠啊 by Stiner(2008/06/05)-MapDropItem
{
	eMdi_MapNum, eMdi_Channel, eMdi_ItemIDX, eMdi_DropCount, eMdi_MaxDropCount
};
// marry 2014-05-10
enum eMarryDelData
{
	eMarryDel_vtFromid , eMarryDel_vtTargetid , eMarryDel_vtvailderr, 
};

enum eItemStoneInfo
{
	eCIST_ObjectID, eCIST_StoneID, eCIST_ItemDBID, eCIST_Stone0,eCIST_Stone1,eCIST_Stone2,eCIST_Stone3,eCIST_Stone4, eCIST_ItemPos,
};
enum eItemStonePyogukInfo
{
    ePyST_StoneID, ePyST_ItemDBID, ePyST_Stone0,ePyST_Stone1,ePyST_Stone2,ePyST_Stone3,ePyST_Stone4,
};

// 2015-01-26 赌博系统数据库枚举
enum eGambleInfo
{
    eGamble_Year,eGamble_Month,eGamble_Day,eGamble_Hour,eGamble_Min,eGamble_Num1,eGamble_Num2,eGamble_Num3,eGamble_CharIdx,
};

// 2015-01-26 赌博系统投注枚举
enum eGambleUserInfo
{
   eGamble_Idx, eGamble_UserCharIdx,eGamble_UserYear,eGamble_UserMonth,eGamble_UserDay,eGamble_UserNum1,eGamble_UserNum2,eGamble_UserNum3,eGamble_InsertFlag,
};

//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////

// DB·I Query㈢; ?【⒁∽ A COoレe dAC
void TestQuery();
void CharacterNumSendAndCharacterInfo(DWORD CharacterIDX, DWORD Protocol);
void CharacterNumSendAndCharacterInfo2(DWORD CharacterIDX, DWORD dwTeam);
void CharacterMugongInfo(DWORD CharacterIDX, DWORD Protocol);
void CharacterItemSlotInfo(DWORD CharacterIDX);
void CharacterItemInfo(DWORD CharacterIDX, DWORD Protocol);
void CharacterItemRareOptionInfo(DWORD CharacterIDX, DWORD Protocol);
void CharacterItemOptionInfo(DWORD CharacterIDX, DWORD Protocol);
void CharacterItemStoneOptionInfo(DWORD CharacterIDX,DWORD Protocol);  // 2014-11-14  新打控系统开发，数据库访问!
void PyogukItemRareOptionInfo(DWORD CharacterIDX, DWORD UserID, DWORD StartDBIdx);
void PyogukItemOptionInfo(DWORD CharacterIDX, DWORD UserID, DWORD StartDBIdx);
void PyogukItemStoneInfo(DWORD CharacterIDX, DWORD UserID, DWORD StartDBIdx);   // 2015-01-04 访问仓库数据
void CharacterAbilityInfo(DWORD CharacterIDX, DWORD Protocol);
//SW051129 Pet
void CharacterPetInfo(DWORD CharacterIDX, DWORD UserID);
void CharacterTitanInfo(DWORD CharacterIDX, DWORD UserID);
void CharacterTitanEquipItemEnduranceInfo(DWORD CharacterIDX);
void PetInvenItemOptionInfo(DWORD CharacterIDX, DWORD UserID, WORD StartPos, WORD EndPos);
void PetInvenItemRareOptionInfo(DWORD CharacterIDX, DWORD UserID, WORD StartPos, WORD EndPos);
void PetInvenItemInfo(DWORD CharacterIDX, WORD StartPos, WORD EndPos);
void PetWearItemInfo( DWORD CharacterIdx, POSTYPE FromPos, POSTYPE ToPos );

void CharacterItemOptionDelete(DWORD OptionIdx, DWORD dwItemDBIdx);
//SW050920 Rare
void CharacterItemRareOptionDelete(DWORD RareIdx, DWORD dwItemDBIdx);

//void CharacterGMControlInfo(DWORD CharacterIDX, DWORD Protocol);
		//TDN
//void CharacterGMControlInsert(DWORD GMChrIdx,DWORD CharacterIDX, DWORD UserIdx, int kind, );
//void CharacterGMControlDelete(DWORD GMChrIdx,DWORD ControlIdx);

void AbilityLevelupLog(CPlayer* pPlayer,WORD AbilityIdx,LEVELTYPE AbilityLevel,EXPTYPE FromExp,EXPTYPE ToExp);

void MapBaseEconomy(BYTE MapNum);
void AuctionRegist(BYTE ItemIdx, DWORD SellerID, DWORD EndDate, DWORD EndTime, DWORD StartPrice, DWORD ImmediatePrice, char Memo[256]);
void AuctionPageQuery(DWORD CharacterIDX, BYTE PageNum);

void SSItemInsert(DWORD CharacterIdx, WORD wItemIdx, DURTYPE Durability, POSTYPE bPosition, DWORD FromChrIdx, WORD bSeal=0,WORD bStatic=0,DWORD bGrow=0, WORD ItemQuality = 0, WORD ItemEntry1 = 0, WORD ItemEntry2 = 0, WORD ItemEntry3 = 0);  // 数据和成长数据附加 2015-01-15


/* New Item Qurey Func */
void ItemUpdateToDB(DWORD CharacterIdx, DWORD dwDBIdx, WORD wItemIdx, DURTYPE Durability, POSTYPE bPosition, WORD qPosition, DWORD RareIdx=0, WORD bStatic = 0, WORD ItemQuality = 0, WORD ItemEntry1 = 0, WORD ItemEntry2 = 0, WORD ItemEntry3 = 0);
//void ItemPyogukMunpaUpdateToDB(DWORD UserIDX, DWORD MunpaIDX, DWORD dwDBIdx, WORD wItemIdx, DURTYPE Durability, POSTYPE bPosition, WORD qPosition);
void ItemCombineUpdateToDB(DWORD dwfromDBIdx, DURTYPE fromDur, DWORD dwToDBIdx, DURTYPE toDur);
void ItemMoveUpdateToDB(DWORD CharacterIDX, DWORD dwfromDBIdx, POSTYPE frompos, DWORD dwtoDBIdx, POSTYPE topos);
void ItemMovePyogukUpdateToDB(DWORD CharacterIDX, DWORD UserIDX, DWORD dwfromDBIdx, POSTYPE frompos, DWORD dwtoDBIdx, POSTYPE topos);
void ItemMoveGuildUpdateToDB(DWORD CharacterIDX, DWORD MunpaIDX, DWORD dwfromDBIdx, POSTYPE frompos, DWORD dwtoDBIdx, POSTYPE topos);
void ItemMovePetInvenUpdateToDB(DWORD CharacterIDX, DWORD dwfromDBIdx, POSTYPE frompos, DWORD dwtoDBIdx, POSTYPE topos);
//SW050920 RareIdx 眠啊
void ItemInsertToDB(DWORD CharacterIdx, WORD wItemIdx, DURTYPE Durability, POSTYPE bPosition, DWORD dwKey, WORD bSeal=0,WORD bStatic=0,DWORD dwGrow=0, WORD ItemQuality = 0, WORD ItemEntry1 = 0, WORD ItemEntry2 = 0, WORD ItemEntry3 = 0);
void ItemDeleteToDB(DWORD dwDBIdx);
void ItemOptionInsertToDB(DWORD CharacterIdx, WORD wItemIdx, POSTYPE Pos, ITEM_OPTION_INFO * pOptionInfo);
//SW050920
void ItemRareInsertToDB(DWORD CharacterIdx, WORD wItemIdx, /*DURTYPE Durability,*/ POSTYPE bPosition, DWORD dwKey, ITEM_RARE_OPTION_INFO* pRareOptionInfo, WORD ItemQuality = 0, WORD ItemEntry1 = 0, WORD ItemEntry2 = 0, WORD ItemEntry3 = 0);

void ItemStoneInsertToDB(DWORD CharacterIdx,DWORD wItemIdx, DWORD wItemDBIdx,DWORD wStoneValue); // 系统数据 2014-11-14

void ItemStoneUpdateToDB(DWORD CharacterIdx,int wItemPos, DWORD wItemStoneIdx,DWORD wItemXqIdx0,DWORD wItemXqIdx1,DWORD wItemXqIdx2,DWORD wItemXqIdx3,DWORD wItemXqIdx4);// 数据 2014-11-19

void ShopItemRareInsertToDB(DWORD CharacterIdx, DWORD ItemIdx, DWORD ItemPos, DWORD ItemDBIdx, ITEM_RARE_OPTION_INFO* pRareOptionInfo);
void RShopItemRareInsertToDB(LPQUERY pData,  LPDBMESSAGE pMessage);

//SW051129
void PetInsertToDB(DWORD UserIdx, DWORD CharacterIdx, DWORD dwItemIdx, PET_TOTALINFO* pPetTotalInfo);
void PetUpdateToDB(DWORD UserIdx, const PET_TOTALINFO* pPetTotalInfo);
void PetDeleteToDB(DWORD dwPetDBIdx);

//SW070207
void TitanInsertToDB(DWORD UserIdx, DWORD CharacterIdx, DWORD dwItemIdx, TITAN_TOTALINFO* pTitanTotalInfo);
void RTitanInsertToDB(LPQUERY pData, LPDBMESSAGE pMessage);
void TitanUpdateToDB(DWORD MasterIdx, const TITAN_TOTALINFO* pTitanTotalInfo);
void TitanDeleteToDB(DWORD dwCallItemDBIdx);

void TitanEquipItemInfoInsertToDB(TITAN_ENDURANCE_ITEMINFO* pInfo);
void RTitanEquipItemInfoInsertToDB(LPQUERY pData, LPDBMESSAGE pMessage);
void TitanEquipItemInfoUpdateToDB(TITAN_ENDURANCE_ITEMINFO* pInfo);
void TitanEquipItemInfoDeleteToDB(DWORD dwTitanEquipDBIdx);

// magi82(33)
void TitanEquipInsertExceptionToDB(TITAN_ENDURANCE_ITEMINFO* pInfo);
void RTitanEquipInsertExceptionToDB(LPQUERY pData, LPDBMESSAGE pMessage);
void TitanEquipInsertExceptionPyogukToDB(DWORD CharacterIdx, TITAN_ENDURANCE_ITEMINFO* pInfo);
void RTitanEquipInsertExceptionPyogukToDB(LPQUERY pData, LPDBMESSAGE pMessage);

/* New Mugong Qurey Func */
void MugongUpdateToDB(MUGONGBASE* msg, char* type);
void MugongMoveUpdateToDB(DWORD dwfromDBIdx, POSTYPE frompos, DWORD dwtoDBIdx, POSTYPE topos, char* type);
void MugongInsertToDB(DWORD CharacterIDX, WORD MugongIDX, POSTYPE MugongPos, BYTE bWeared, BYTE bSung, WORD Option = 0);
void MugongDeleteToDB(DWORD dwDBIdx);


//pjs ㄏ【aA ACUTION 【々·AレC Ai⒁∷i㈢ YE? A㈢ COo 【
void CheckAuction(MSGBASE* msg);
void AuctionSearch(SEARCHLIST* msg);
void AuctionSort(SORTLIST* sort);
void AuctionRegister(REGISTERAUCTION* msg);
void AuctionJoin(JOINAUCTION* msg);
void AuctionCancel(AUCTIONCANCEL *msg);
//////////////////////////////////////////////////////////////////////////
// āAā ∑u·A DBAo㈢
void PartyLoad(DWORD PartyIDX);
void PartyInfoByUserLogin(DWORD PartyIDX, DWORD CharacterIDX);
void PartyBreakup(DWORD PartyIDX);
//2008. 5. 21. CBH - 规颇 积己 可记 眠啊 包访 窃荐 荐沥
//void PartyCreate(DWORD MasterIDX, BYTE Option);
void PartyCreate(DWORD MasterIDX, PARTY_ADDOPTION* pAddOption);
///////////////////////////////////////////////////////////////
void PartyMemberUpdate(CParty* pParty, DWORD PartyIDx);
void PartyAddMember(DWORD PartyIDX, DWORD TargetID);
void PartyDelMember(DWORD PartyIDX, DWORD TargetID);
void PartyChangeMaster(DWORD PartyID, DWORD PlayerID, DWORD TargetPlayerID);

//////////////////////////////////////////////////////////////////////////
// MapChange
void ChangeMapGetCharacterInfo(DWORD CharacterIDX, DWORD dwConnectionIndex);
void ChangeMapGetCharacterMugongInfo(DWORD CharacterIDX, DWORD dwConnectionIndex);
//////////////////////////////////////////////////////////////////////////
//㈢āA ∑u·A DB Ao㈢
/*
void MunpaLoad(DWORD MapNum, DWORD StartMunpaID);
void MunpaItemLoad(DWORD MapNum, DWORD StartItemDBIDx);
void MunpaItemOption(DWORD MapNum, DWORD StartDBIDx);
void MunpaLoadSyn(DWORD PlayerID, MAPTYPE MapNum, DWORD GotoPage, DWORD PageUnit, char* OrderType);
void MunpaSearchMunpa(DWORD PlayerID, BYTE MapNum, char* Key, DWORD GotoPage);
void MunpaCreate(MUNPA_CREATE_INFO* pmsg, char* MasterName);
void MunpaBreakUp(DWORD PlayerID, DWORD MunpaID);
void MunpaBreakUpMembers(DWORD StartPlayerID, DWORD MunpaID);
void MunapBreakupClr(DWORD MunpaID);
void MunpaChangeMaster(DWORD FromPlayerID, DWORD ToPlayerID);
void MunpaChangeMemberRank(DWORD MasterID, DWORD PlayerID, int Position, DWORD MunpaID);
void MunpaBanMember(DWORD MasterID, DWORD TargetID, DWORD MunpaID, BOOL bLast);
void MunpaSecede(DWORD PlayerID, DWORD MunpaID);
void MunpaAcceptMember(DWORD MasterID, DWORD TargetID, DWORD MunpaID, BOOL bLast);
void MunpaDenyMember(DWORD MasterID, DWORD TargetID, DWORD MunpaID, BOOL bLast);
void MunpaJoinMember(DWORD PlayerID, DWORD TargetMunpaID);
void MunpaJoinMemberCancel(DWORD PlayerID, DWORD MunpaIDX);
void MunpaHomeInfo(DWORD PlayerID, DWORD MunpaID);
void MunpaRegularMembersInfo(DWORD PlayerID, DWORD MunpaID, DWORD GotoPage);
void MunpaJoinMembersInfo(DWORD PlayerID, DWORD MunpaID, DWORD GotoPage);
void MunpaBoardInfo(DWORD PlayerID, DWORD MunpaID);
void MunpaCreateBoard(SEND_MUNPA_BOARD_RANK_INFO* msg);
void MunpaDeleteBoard(DWORD MunpaID, DWORD BoardID, DWORD MasterID);
void MunpaBoardRankInfo(DWORD MasterID, DWORD MunpaID, DWORD BoardID);
void MunpaBoardRankInfoUpdate(DWORD CharacterIDX, DWORD MunpaID, SEND_MUNPA_BOARD_RANK_INFO* msg);
void MunpaBoardBackContent(DWORD PlayerID, DWORD BoardIdx, DWORD StartContentID, DWORD MyMunpaRank);
void MunpaBoardFrontContent(DWORD PlayerID, DWORD BoardIdx, DWORD StartContentID, DWORD MyMunpaRank);
void MunpaBoardCurrentContent(DWORD PlayerID, DWORD BoardIdx, DWORD StartContentID, DWORD MyMunpaRank);
void MunpaBoardWrite(DWORD PlayerID, DWORD BoardIDX, char* Subject, char* Content, DWORD RootContentsID, DWORD MyMunpaRank);
void MunpaBoardModify(DWORD PlayerID, DWORD BoardIDX, char* Subject, char* Content, DWORD OriginalContentsID);
void MunpaBoardSubjectList(DWORD PlayerID, DWORD BoardID, DWORD CurPage, DWORD PageUnit);
void MunpaBoardDeleteContents(DWORD PlayerID, DWORD ContentIDx, DWORD MyRank);
void MunpaMoneyPutInOut(DWORD PlayerID, DWORD TargetMunpaID, DWORD Money, BOOL bPutIn); //1: putin, 0: putout
void MunpaModifyIntro(DWORD CharacterIDX, DWORD MunpaID, char* pIntroMsg);*/

//////////////////////////////////////////////////////////////////////////
//guild
void GuildLoadGuild(DWORD StartGuildIdx);
void GuildLoadMember(DWORD StartCharacterIdx);
void GuildCreate(DWORD CharacterIdx, char* CharacterName, LEVELTYPE CharacterLvl, char* GuildName, char* Intro);
void GuildBreakUp(DWORD GuildIdx, DWORD MasterIdx);
void GuildDeleteMember(DWORD GuildIdx, DWORD MemberIDX);
void GuildSecedeMember(DWORD GuildIdx, DWORD PlayerIdx);
void GuildAddMember(DWORD GuildIdx, DWORD MemberIDX, char* GuildName, BYTE Rank);
void GuildMarkRegist(DWORD GuildIdx, char* ImgData, DWORD CharacterIdx);
void GuildLevelUp(DWORD GuildIdx, BYTE Level);
void GuildChangeMemberRank(DWORD GuildIdx, DWORD MemberIdx, BYTE Rank);
void GuildItemLoad(DWORD MapNum, DWORD StartItemDBIDx);
void GuildItemOption(DWORD MapNum, DWORD StartDBIdx);
void GuildItemRareOption(DWORD MapNum, DWORD StartDBIdx);
void GuildMoneyUpdate(DWORD GuildIdx, MONEYTYPE Money);
void GuildGiveMemberNickName(DWORD TargetId, char * NickName);
//SW060526 巩颇芒绊 酒捞袍 沥焊历厘 矫痢 函版

void GuildItemLoadInNeed(DWORD MapNum, DWORD GuildIdx);
void GuildItemOptionInNeed(DWORD MapNum, DWORD GuildIdx,DWORD StartDBIdx);
void GuildItemRareOptionInNeed(DWORD MapNum, DWORD GuildIdx, DWORD StartDBIdx);
void RGuildItemLoadInNeed( LPQUERY pData, LPDBMESSAGE pMessage );
void RGuildItemOptionInNeed(LPQUERY pData, LPDBMESSAGE pMessage);
void RGuildItemRareOptionInNeed(LPQUERY pData, LPDBMESSAGE pMessage);
// 06. 03. 巩颇傍瘤 - 捞康霖
void GuildLoadNotice(DWORD StartDBIdx);
void GuildUpdateNotice(DWORD GuildIdx, char* Notice);

//SW060719 巩颇器牢飘
//void GuildHuntedMonsterCountLastDBUpdateDate(WORD MapNum);	//甘 辑滚 何泼矫 弥辆 沥魂老阑 啊廉客 m_wCurDateCheckForConverCount 技泼茄促.
//void RGuildHuntedMonsterCountLastDBUpdateDate();	
void GuildLoadGuildPointInfo(DWORD startGuildDBIdx);	//沥评厘丛瘤矫 巩颇DB牢郸胶
void GuildLoadGuildPlustimeInfo(DWORD startDBIdx);
void RGuildLoadGuildPointInfo(LPQUERY pData, LPDBMESSAGE pMessage);
void RGuildLoadGuildPlustimeInfo(LPQUERY pData, LPDBMESSAGE pMessage);
void GuildAddHuntedMonsterCount(DWORD GuildIdx, DWORD MonsterCount);	//10盒 付促 沥焊 盎脚
void GuildGetHuntedMonsterTotalCountWithInit(DWORD GuildIdx);	//概老 磊沥 巩颇器牢飘肺 沥魂

void RGuildAddHuntedMonsterCount(LPQUERY pData, LPDBMESSAGE pMessage);
void RGuildGetHuntedMonsterTotalCountWithInit(LPQUERY pData, LPDBMESSAGE pMessage);

void GuildAddGuildPoint(DWORD GuildIdx, DWORD rAddPoint, DWORD rAddKind, DWORD rAdditionalData);	//巩颇 器牢飘 眠啊 r# 弊措肺 府畔罐酒 镜 函荐
void RGuildAddGuildPoint(LPQUERY pData, LPDBMESSAGE pMessage);
void GuildUseGuildPoint(DWORD GuildIdx, DWORD rUsePoint, DWORD rUseKind, DWORD rKindIdx, DWORD rEndtime);
void RGuildUseGuildPoint(LPQUERY pData, LPDBMESSAGE pMessage);
void GuildPlustimeEnd(DWORD GuildIdx, DWORD rPlusTimeIdx);
void RGuildPlustimeEnd(LPQUERY pData, LPDBMESSAGE pMessage);

void LogGuildPoint(DWORD GuildIdx, DWORD TotalGuildPoint, BOOL bAdd, int eKind, DWORD GuildPoint, DWORD AddData, DWORD CharacterIdx);
//////////////////////////////////////////////////////////////////////////
//C⊥【
void PyogukBuyPyoguk(DWORD PlayerIDX);


/* C⊥【 dì Qurey od */
void CharacterPyogukInfo(DWORD UserIdx, DWORD CharacterIdx);
void CharacterPyogukItemInfo(DWORD CharacterIdx, DWORD StartDBIdx);
void PyogukMoneyUpdateToDB(DWORD UserIdx, MONEYTYPE PyogukMoney);


//////////////////////////////////////////////////////////////////////////
//A√【
void FriendNotifyLogouttoClient(DWORD PlayerID);
//////////////////////////////////////////////////////////////////////////
//iì
//void FameMunpaUpdate(DWORD StartMunpaIDX, DWORD MapNum);
void FameCharacterUpdate(DWORD PlayerIDX, FAMETYPE val);
void BadFameCharacterUpdate(DWORD PlayerIDX, FAMETYPE val);
void PKCharacterUpdate(DWORD PlayerIDX, DWORD val);
//////////////////////////////////////////////////////////////////////////
// SavePoint
void SavePointUpdate(DWORD CharacterIDX, WORD LoginPoint_Idx, WORD MapNum);
void SaveMapChangePointUpdate(DWORD CharacterIDX, WORD MapChangePoint_Idx);
 //甘 捞悼矫 呈公 蠢妨 敲饭捞绢啊 促弗 甘俊 甸绢啊扁 傈俊 诀单捞飘 犬牢 饶 甘 盲牢瘤 橇肺配妮阑 焊晨
void SaveMapChangePointReturn(DWORD CharacterIDX, WORD MapChangePoint_Idx, DWORD dwConnectionIdx, LEVELTYPE lvl, EXPTYPE ExpPoint, MONEYTYPE PyogukMoney);
//////////////////////////////////////////////////////////////////////////
//Co№o【Y
void WantedLoad(WANTEDTYPE StartWantedIDX);
void WantedInfoByUserLogIn(DWORD CharacterIDX);
void WantedBuyRight(DWORD CharacterIDX, WANTEDTYPE WantedIDX, MONEYTYPE AddPrize);
void WantedRegist(DWORD CharacterIDX, DWORD TargetIDX, MONEYTYPE Prize);
void WantedGiveUpRight(DWORD CharacterIDX, DWORD WantedIDX);
void WantedComplete(DWORD CharacterIDX, char* TargetName, WANTEDTYPE WantedIDX);
void WantedDestroyed(DWORD WantedIDX); // destroyed by time
void WantedOrderedList(DWORD CharacterIDX, DWORD Page, char* OrderType, BYTE PageUnit);
void WantedMyList(DWORD CharacterIDX, DWORD Page, BYTE PageUnit);
void WantedSearch(DWORD CharacterIDX, char* WantedName, DWORD Page, BYTE PageUnit);
//////////////////////////////////////////////////////////////////////////
// Update㈢
//void CharacterMugongUpdate(MUGONGBASE* msg);
//void CharacterMugongMoveUpdate(DWORD toDBIdx, POSTYPE toPos, DWORD fromDBIdx, POSTYPE fromPos);
//void CharacterInfoUpdate(CPlayer* pPlayer, DWORD dwConnectionIndex);
void CharacterHeroInfoUpdate(CPlayer* pPlayer);
void CharacterTotalInfoUpdate(CPlayer* pPlayer);
void CharacterBadFameUpdate(DWORD PlayerID, int BadFame, DWORD Money);
//void CharacterQuickInfoUpdate(CPlayer * pPlayer);
void CharacterMugongInfoUpdate(CPlayer* pPlayer);
void CharacterTablePartyIdxUpdate(DWORD PlayerID);
void PyogukMoneyUpdate(DWORD PlayerID, BYTE PyogukNum, DWORD Money);
void RegistLoginMapInfo(DWORD CharacterIDX, char* CharacterName, BYTE MapNum, WORD MapPort);
void UnRegistLoginMapInfo(DWORD CharacterIDX);
void CharacterLvlExpUpdate(DWORD PlayerID, LEVELTYPE level, EXPTYPE exppoint);
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
// Auìā㈢ (AEìAU ∑AIレa ā±CO)
void QuestTotalInfo(DWORD PlayerID);
void QuestUpdateToDB(DWORD PlayerID, DWORD QuestIdx, QSTATETYPE State, BYTE bEnd);
//RaMa
void QuestItemload(DWORD PlayerID, int QuestIdx = -1 );
void QuestItemInsert(DWORD PlayerID, DWORD dwQuestIdx, DWORD ItemIdx, DWORD dwItemNum );
void QuestItemDelete(DWORD PlayerID, DWORD ItemIdx);
void QuestItemDeleteAll(DWORD PlayerID, DWORD dwQuestIdx);
void QuestItemUpdate(DWORD PlayerID, DWORD dwQuestIdx, DWORD ItemIdx, DWORD ItemNum);

void QuestMainQuestLoad(DWORD PlayerID, int QuestIdx = -1 );
void QuestMainQuestUpdateToDB(DWORD PlayerID, DWORD mQuestIdx, QSTATETYPE State, DWORD EndParam, DWORD Time);
void QuestMainQuestDelete(DWORD PlayerID, DWORD mQuestIdx);
void QuestMainQuestInsert(DWORD PlayerID, DWORD mQuestIdx, QSTATETYPE State, DWORD Time);

void QuestSubQuestLoad(DWORD PlayerID, int QuestIdx = -1 );
void QuestSubQuestInsert(DWORD PlayerID, DWORD mQuestIdx, DWORD sQuestIdx, QSTATETYPE Data, QSTATETYPE Time);
void QuestSubQuestUpdateToDB(DWORD PlayerID, DWORD mQuestIdx, DWORD sQuestIdx, QSTATETYPE Data, QSTATETYPE Time);
void QuestSubQuestDelete(DWORD PlayerID, DWORD mQuestIdx, DWORD sQuestIdx);
void QuestSubQuestDeleteAll(DWORD PlayerID, DWORD mQuestIdx);

void QuestMainQuestUpdateCheckTime(DWORD dwCharIdx, DWORD dwQuestIdx, DWORD dwCheckType, DWORD dwCheckTime );

void Quest_EndQuest_New( DWORD PlayerID, DWORD mQuestIdx, QSTATETYPE State, DWORD EndParam, DWORD Time );
void Quest_DeleteQuest_New( DWORD PlayerID, DWORD mQuestIdx );
void Quest_EndSubQuest_New( DWORD PlayerID, DWORD mQuestIdx, DWORD sQuestIdx, QSTATETYPE Data, QSTATETYPE Time );

//////////////////////////////////////////////////////////////////////////
//journal
void JournalGetList(DWORD CharacterIDX);
void JouranlInsertQuest(DWORD CharacterIDX, BYTE Kind, DWORD QuestIDX, DWORD SubQuestIDX, int bCompleted);
void JouranlInsertWanted(DWORD CharacterIDX, BYTE Kind, DWORD Param, char* ParamName);
void JournalInsertLevel(DWORD CharacterIDX, BYTE Kind, DWORD Level);
void JournalSaveUpdate(DWORD CharacterIDX, DWORD JournalIndex);
void JournalDelete(DWORD CharacterIDX, DWORD JournalIndex);
//////////////////////////////////////////////////////////////////////////
// ShopItem
void ShopItemInvenInfo( DWORD CharacterIdx );
void CharacterShopItemInfo(DWORD CharacterIDX, DWORD StartDBIdx);
void UsingShopItemInfo( DWORD CharacterIdx );
void SavedMovePointInfo( DWORD CharacterIdx );
void SavedMovePointInsert( DWORD CharacterIdx, char* Name, DWORD MapIdx, DWORD Position );
void SavedMovePointUpdate( DWORD CharacterIdx, DWORD DBIdx, char* Name );
void SavedMovePointDelete( DWORD CharacterIdx, DWORD DBIdx );
void UsingShopItemUpdateToDB( DWORD UserIdx, DWORD CharacterIdx, DWORD dwItemIdx, DWORD dwDBIdx, DWORD Param, DWORD StartTime, DWORD RemainTime );
void ShopItemDeleteToDB( DWORD CharacterIdx, DWORD dwDBIdx );
void ShopItemUpdatetimeToDB( DWORD CharacterIdx, DWORD dwItemIdx, DWORD RemainTime );
void ShopItemParamUpdateToDB( DWORD CharacterIdx, DWORD dwItemIdx, DWORD Param );
void ShopItemUseParamUpdateToDB( DWORD UserIdx, DWORD CharacterIdx, DWORD dwItemIdx, DWORD Param );
void ShopItemGetItem( DWORD CharacterIdx, WORD wItemIdx, DWORD Dur );
void ItemMoveShopUpdateToDB(DWORD CharacterIDX, DWORD UserIDX, DWORD dwfromDBIdx, POSTYPE frompos, DWORD dwtoDBIdx, POSTYPE topos);
void ShopItemUpdateCharacterIdx( DWORD CharacterIdx, DWORD dwDBIdx );
void ShopItemUpdateUseInfoToDB( DWORD CharacterIdx, DWORD dwDBIdx, DWORD Param, DWORD RemainTime );
void CharacterChangeName( DWORD CharacterIdx, char* Name, DWORD DBIdx );
void CharacterChangeInfoToDB( DWORD CharacterIdx, DWORD Gender, DWORD HairType, DWORD FaceType, float Height, float Width );
void IsUseAbleShopAvatarItem( DWORD CharacterIdx, DWORD ItemDBIdx, DWORD ItemIdx, DWORD ItemPos );
void IsAbleShopEquipItem( DWORD CharacterIdx, DWORD ItemDBIdx, DWORD FromItemIdx, DWORD FromPos, DWORD ToItemIdx, DWORD ToPos );
void ShopItemAllUseInfoUpdateToDB( DWORD ItemDBIdx, DWORD CharacterIdx, WORD ItemIdx, DWORD Param, DWORD Remaintime );



//////////////////////////////////////////////////////////////////////////
// Log ∑u·A E√Aa COo
void InsertLogCharacter( DWORD charIdx, LEVELTYPE level, HERO_TOTALINFO* pTotalInfo );
void InsertLogExp( BYTE bType, DWORD charIdx, LEVELTYPE level, DWORD changeValue, EXPTYPE NowExp, WORD MurdererKind, DWORD MurdererIdx, DWORD CurAbilPoint);
void InsertLogMoney( BYTE type, DWORD charIdx, MONEYTYPE InvenMoney, MONEYTYPE ChangeValue, DWORD TargetID, MONEYTYPE TargetMoney );
void InsertLogWorngMoney( BYTE type, DWORD charIdx, MONEYTYPE changeValue, MONEYTYPE InvenMoney, MONEYTYPE PyogukMoney, DWORD targetIdx );
void InsertLogMugong( WORD type, DWORD charIdx, DWORD mugongIdx, DWORD mugongDBIdx, LEVELTYPE mugongSung, EXPTYPE mugongExp );
void InsertLogMugongExp( DWORD charIdx, EXPTYPE changeValue, DWORD mugongIdx, DWORD mugongDBIdx, LEVELTYPE mugongSung, EXPTYPE mugongExp, POSTYPE mugongPos );
void LogItemMoney(DWORD FromChrIdx, char* FromChrName, DWORD ToChrIdx, char* ToChrName,
				WORD LogType, MONEYTYPE FromTotalMoney, MONEYTYPE ToTotalMoney, MONEYTYPE ChangeMoney,
				DWORD ItemIdx, DWORD ItemDBIdx, POSTYPE ItemFromPos, POSTYPE ItemToPos,
				DURTYPE ItemDur, EXPTYPE ExpPoint);
void LogItemOption(DWORD CharacterIdx, DWORD ItemDBIdx, ITEM_OPTION_INFO* pOptionInfo);
//SW050920 Rare
void LogItemRareOption(DWORD CharacterIdx, DWORD ItemDBidx, ITEM_RARE_OPTION_INFO* pRareOptionInfo);
void LogCheat(DWORD Character_Idx, BYTE LogType, MONEYTYPE Param, DWORD ItemIdx, DWORD ItemDBIdx);
void InsertLogTool( DWORD dwLogType, DWORD dwLogKind, DWORD dwOperIdx, char* sOperName, DWORD dwTargetIdx, char* sTargetName,
				    DWORD dwParam1, DWORD dwParam2, DWORD dwParam3, DWORD dwParam4 );
void LogGuild(DWORD CharacterIdx, DWORD GuildIdx, WORD LogType, DWORD Param1 = 0, DWORD Param2 = 0);
void LogPet(DWORD CharacterIdx, DWORD UserIdx, BYTE LogType, DWORD PetDBIdx, DWORD PetSummonItemDBIdx, DWORD PetGrade, DWORD PetStamina, DWORD PetFriendship, BYTE PetAlive);
void LogGMToolUse( DWORD CharacterIdx, DWORD GMLogtype, DWORD Logkind, DWORD Param1, DWORD Param2 );
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
// A∑√ì∑ ±レ⊥AIā㈢ COo
void UpdateCharacterInfoByTime(DWORD charIdx, EXPTYPE exp, DWORD abilityExp, MONEYTYPE money, DWORD time, DWORD LastPKEndTime );
void UpdateMugongExpByTime(DWORD charIdx, DWORD DBIdx, EXPTYPE exp);

void InsertSpeedHackCheck(DWORD charIdx, char* charName, DWORD Distance, DWORD skillDelay);
//////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////
// DB·IìIAI ōAAo| ōā々 A㈢CIA COoレe.
void ReceiveMessageFromDB(LPQUERY pData,  LPDBMESSAGE pMessage);
void RTestQuery(LPQUERY pData, LPDBMESSAGE pMessage);
void RCharacterNumSendAndCharacterInfo(LPQUERY pData, LPDBMESSAGE pMessage);
void RCharacterInfoQueryToEventMap(LPQUERY pData, LPDBMESSAGE pMessage);
void RCharacterMugongInfo(LPQUERY pData, LPDBMESSAGE pMessage);
void RCharacterItemSlotInfo(LPQUERY pData, LPDBMESSAGE pMessage);
void RCharacterItemInfo(LPQUERY pData, LPDBMESSAGE pMessage);
void RCharacterItemRareOptionInfo(LPQUERY pData, LPDBMESSAGE pMessage);
void RCharacterItemOptionInfo(LPQUERY pData, LPDBMESSAGE pMessage);
void RCharacterItemStoneOptionInfo(LPQUERY pData, LPDBMESSAGE pMessage);  // 2014-11-14  新打控系统开发，数据库访问!
void RItemStoneInsertToDB(LPQUERY pData, LPDBMESSAGE pMessage);           // 2014-11-15  数据库！
void RItemStoneUpdateToDB(LPQUERY pData, LPDBMESSAGE pMessage);           // 2014-11-15  数据库！
void RCharacterAbilityInfo(LPMIDDLEQUERY pData, LPDBMESSAGE pMessage);
void RCharacterPetInfo(LPQUERY pData, LPDBMESSAGE pMessage);
void RCharacterTitanInfo(LPQUERY pData, LPDBMESSAGE pMessage);
void RCharacterTitanEquipItemEnduranceInfo(LPQUERY pData, LPDBMESSAGE pMessage);
void RPetInvenItemOptionInfo(LPQUERY pData, LPDBMESSAGE pMessage);
void RPetInvenItemRareOptionInfo(LPQUERY pData, LPDBMESSAGE pMessage);
void RPetInvenItemInfo(LPQUERY pData, LPDBMESSAGE pMessage);
void RPetWearItemInfo(LPQUERY pData, LPDBMESSAGE pMessage);

void RPetInsert__(DWORD CharacterIdx, PET_TOTALINFO* pPetTotalInfo);

//void RCharacterGMControlInfo(LPMIDDLEQUERY pData, LPDBMESSAGE pMessage);

//void RCharacterGMControlInsert(LPMIDDLEQUERY pData, LPDBMESSAGE pMessage);

void RMapBaseEconomy(LPQUERY pData, LPDBMESSAGE pMessage);
void RAuctionPageQuery(LPQUERY pData, LPDBMESSAGE pMessage);

/* Item return func */
void RCharacterItemGetBuyIDX(LPQUERY pData, LPDBMESSAGE pMessage);
void RCharacterItemInsert(LPQUERY pData, LPDBMESSAGE pMessage);
void RSSItemInsert(LPQUERY pData, LPDBMESSAGE pMessage); // LBS 03.10.09
void RCharacterItemGetCheatIDX(LPQUERY pData, LPDBMESSAGE pMessage);
void RCharacterItemGetDivideIDX(LPQUERY pData, LPDBMESSAGE pMessage);
void RCharacterDivideNewItemIDX(LPQUERY pData, LPDBMESSAGE pMessage);
void RCharacterItemGetHuntIDX(LPQUERY pData, LPDBMESSAGE pMessage);
void RCharacterDupItemInsertIDX(LPQUERY pData, LPDBMESSAGE pMessage);
void RShopItemInvenInfo( LPQUERY pData, LPDBMESSAGE pMessage );
void RUsingShopItemInfo( LPQUERY pData, LPDBMESSAGE pMessage );
void RSavedMovePointInfo( LPMIDDLEQUERY pData, LPDBMESSAGE pMessage );
void RSavedMovePointInsert( LPMIDDLEQUERY pData, LPDBMESSAGE pMessage );
void RCharacterChangeName( LPQUERY pData, LPDBMESSAGE pMessage );
void RIsUseAbleShopAvatarItem( LPQUERY pData, LPDBMESSAGE pMessage );
void RIsAbleShopEquipItem( LPQUERY pData, LPDBMESSAGE pMessage );

/* New Item Return Func */
void RItemInsert(LPQUERY pData, LPDBMESSAGE pMessage);
void RItemOptionInsert(LPQUERY pData, LPDBMESSAGE pMessage);
void RItemRareInsert(LPQUERY pData, LPDBMESSAGE pMessage);

/* New Pet Return Func */
void RPetInsert(LPQUERY pData, LPDBMESSAGE pMessage);

/* New Mugong Return Func */
void RMugongInsert(LPQUERY pData, LPDBMESSAGE pMessage);

//pjs ㄏ【aA ACUTION 【々·AレC Ai⒁∷i㈢ YE? A㈢ COo 【..
void RAuctionCheck(LPQUERY pData, LPDBMESSAGE pMessage);
void RAuctionPageList(LPQUERY pData, LPDBMESSAGE pMessage);
void RAuctionConfirm(LPQUERY pData, LPDBMESSAGE pMessage);
//////////////////////////////////////////////////////////////////////////
//āAā
void RPartyLoad(LPQUERY pData, LPDBMESSAGE pMessage);
//--- 2008. 5. 21. CBH - 规颇 积己 眠啊 可记 包访 贸府 荐沥 -------
//void RPartyCreate(LPQUERY pData, LPDBMESSAGE pMessage);
//void RPartyInfoByUserLogin(LPQUERY pData, LPDBMESSAGE pMessage);
void RPartyCreate(LPMIDDLEQUERY pData, LPDBMESSAGE pMessage);
void RPartyInfoByUserLogin(LPMIDDLEQUERY pData, LPDBMESSAGE pMessage);
//-----------------------------------------------------------------
void RPartyChangeMaster(LPQUERY pData, LPDBMESSAGE pMessage);
void RPartyDelMember(LPQUERY pData, LPDBMESSAGE pMessage);
//////////////////////////////////////////////////////////////////////////

void RCharacterStatsHistory(LPQUERY pData, LPDBMESSAGE pMessage);
void RSavePointUpdate(LPQUERY pData, LPDBMESSAGE pMessage);

//////////////////////////////////////////////////////////////////////////
//㈢āA 
/*
void RMunpaLoad(LPQUERY pData, LPDBMESSAGE pMessage);
void RMunpaItemLoad(LPQUERY pData, LPDBMESSAGE pMessage);
void RMunpaItemOption(LPQUERY pData, LPDBMESSAGE pMessage);
void RMunpaLoadSyn(LPQUERY pData, LPDBMESSAGE pMessage);
void RMunpaCreate(LPLARGEQUERY pData, LPDBMESSAGE pMessage);
void RMunpaBreakUp(LPQUERY pData, LPDBMESSAGE pMessage);
void RMunpaBreakUpMembers(LPQUERY pData, LPDBMESSAGE pMessage);
void RMunpaChangeMemberRank(LPQUERY pData, LPDBMESSAGE pMessage);
void RMunpaAcceptMember(LPQUERY pData, LPDBMESSAGE pMessage);
void RMunpaBanMember(LPQUERY pData, LPDBMESSAGE pMessage);
void RMunpaSecede(LPQUERY pData, LPDBMESSAGE pMessage);
void RMunpaDenyMember(LPQUERY pData, LPDBMESSAGE pMessage);
void RMunpaJoinMember(LPQUERY pData, LPDBMESSAGE pMessage);
void RMunpaJoinMemberCancel(LPQUERY pData, LPDBMESSAGE pMessage);
void RMunpaRegularMembersInfo(LPQUERY pData, LPDBMESSAGE pMessage);
void RMunpaJoinMembersInfo(LPQUERY pData, LPDBMESSAGE pMessage);
void RMunpaBoardInfo(LPQUERY pData, LPDBMESSAGE pMessage);
void RMunpaCreateBoard(LPQUERY pData, LPDBMESSAGE pMessage);
void RMunpaBoardRankInfo(LPQUERY pData, LPDBMESSAGE pMessage);
void RMunpaBoardRankInfoUpdate(LPQUERY pData, LPDBMESSAGE pMessage);
void RMunpaBoardFrontContent(LPQUERY pData, LPDBMESSAGE pMessage);
void RMunpaBoardDeleteContents(LPQUERY pData, LPDBMESSAGE pMessage);
void RMunpaMoneyPutInOut(LPQUERY pData, LPDBMESSAGE pMessage);

//Middle Query
void RMunpaHomeInfo(LPMIDDLEQUERY pData, LPDBMESSAGE pMessage);
void RMunpaBoardSubjectList(LPMIDDLEQUERY pData, LPDBMESSAGE pMessage);

// Large Query
void RMunpaBoardReadContent(LPLARGEQUERY pData, LPDBMESSAGE pMessage);
void RMunpaBoardModify(LPLARGEQUERY pData, LPDBMESSAGE pMessage);
void RMunpaBoardWrite(LPLARGEQUERY pData, LPDBMESSAGE pMessage);
*/

//////////////////////////////////////////////////////////////////////////
//guild
void RGuildLoadGuild(LPQUERY pData, LPDBMESSAGE pMessage);
void RGuildLoadMember(LPQUERY pData, LPDBMESSAGE pMessage);
void RGuildLoadMark(LPLARGEQUERY pData, LPDBMESSAGE pMessage);
void RGuildCreate(LPQUERY pData, LPDBMESSAGE pMessage);
void RGuildBreakUp( LPQUERY pData, LPDBMESSAGE pMessage );
void RGuildDeleteMember( LPQUERY pData, LPDBMESSAGE pMessage );
void RGuildSecedeMember( LPQUERY pData, LPDBMESSAGE pMessage );
void RGuildItemLoad( LPQUERY pData, LPDBMESSAGE pMessage );
void RGuildItemOption(LPQUERY pData, LPDBMESSAGE pMessage);
void RGuildItemRareOption(LPQUERY pData, LPDBMESSAGE pMessage);
void RGuildMarkRegist(LPLARGEQUERY pData, LPDBMESSAGE pMessage);
void RGuildLoadNotice(LPMIDDLEQUERY pData, LPDBMESSAGE pMessage);
//////////////////////////////////////////////////////////////////////////
//C⊥【
void RCharacterPyogukInfo(LPQUERY pData, LPDBMESSAGE pMessage);
void RCharacterPyogukItemInfo(LPQUERY pData, LPDBMESSAGE pMessage);
//SW050920 Rare
void RPyogukItemRareOptionInfo(LPQUERY pData, LPDBMESSAGE pMessage);
void RPyogukItemOptionInfo(LPQUERY pData, LPDBMESSAGE pMessage);
void RPyogukItemStoneInfo(LPQUERY pData, LPDBMESSAGE pMessage);  // 2015-01-05 数据访问仓库回调函数

//////////////////////////////////////////////////////////////////////////
//A√【
void RFriendNotifyLogouttoClient(LPQUERY pData, LPDBMESSAGE pMessage);

//////////////////////////////////////////////////////////////////////////
//iì
//void RFameMunpaUpdate(LPQUERY pData, LPDBMESSAGE pMessage);

//////////////////////////////////////////////////////////////////////////
//Co№o【Y
void RWantedLoad(LPQUERY pData, LPDBMESSAGE pMessage);
void RWantedInfoByUserLogIn(LPQUERY pData, LPDBMESSAGE pMessage);
void RWantedRegist(LPQUERY pData, LPDBMESSAGE pMessage);
void RWantedComplete(LPQUERY pData, LPDBMESSAGE pMessage);
void RWantedDestroyed(LPQUERY pData, LPDBMESSAGE pMessage);
void RWantedOrderedList(LPQUERY pData, LPDBMESSAGE pMessage);
void RWantedMyList(LPQUERY pData, LPDBMESSAGE pMessage);
void RWantedSearch(LPQUERY pData, LPDBMESSAGE pMessage);
//////////////////////////////////////////////////////////////////////////
// Auìā㈢ (AEìAU ∑AIレa ā±CO)
void RQuestTotalInfo(LPQUERY pData, LPDBMESSAGE pMessage);
void RQuestMainQuestLoad(LPQUERY pData, LPDBMESSAGE pMessage);
void RQuestSubQuestLoad(LPQUERY pData, LPDBMESSAGE pMessage);
void RQuestItemLoad(LPQUERY pData, LPDBMESSAGE pMessage);
//////////////////////////////////////////////////////////////////////////
void RJournalGetList(LPQUERY pData, LPDBMESSAGE pMessage);
//////////////////////////////////////////////////////////////////////////
void RSaveMapChangePointReturn(LPQUERY pData, LPDBMESSAGE pMessage);
//////////////////////////////////////////////////////////////////////////
//
void RCharacterShopItemInfo(LPQUERY pData, LPDBMESSAGE pMessage);

//void UpdateCharacterMoney( DWORD dwCharIdx, MONEYTYPE money, BYTE flag );
//void RUpdateCharacterMoney( LPQUERY pData, LPDBMESSAGE pMessage );

void ConfirmUserOut( DWORD dwConnectIdx, DWORD dwUserIdx, DWORD dwCharIdx, DWORD dwKind );
void RConfirmUserOut( LPQUERY pData, LPDBMESSAGE pMessage );


void SaveCharInfoBeforeLogout(DWORD PlayerIdx, DWORD dwConnectionIndex, DWORD UserIdx, MONEYTYPE PyogukMoney);
void RSaveCharInfoBeforeLogout( LPQUERY pData, LPDBMESSAGE pMessage );

// guildfieldwar
void LoadGuildWarRecord( DWORD dwIdx );
void RLoadGuildWarRecord( LPQUERY pData, LPDBMESSAGE pMessage );
void DeleteGuildWarRecord( DWORD dwGuildIdx );
void LoadGuildFieldWar( DWORD dwIdx );
void RLoadGuildFieldWar( LPQUERY pData, LPDBMESSAGE pMessage );
void InsertGuildFieldWar( DWORD dwGuildIdx1, DWORD dwGuildIdx2, DWORD dwMoney );
void DeleteGuildFieldWar( DWORD dwGuildIdx1, DWORD dwGuildIdx2 );
void UpdateGuildFieldWarRecordToDB( DWORD dwGuildIdx, DWORD dwVitory, DWORD dwDraw, DWORD dwLoose );

// Guild Tournament
void GuildTournamentInfoLoad();
void GuildTournamentCancelIdxLoad( DWORD TournamentIdx );
void GuildTournamentCancelIdxInsert( DWORD TournamentIdx, DWORD GuildIdx );
void GuildTournamentGuildInfoInsert( DWORD GuildIdx, char* GuildName, DWORD Position );
void GuildTournamentGuildInfoDelete( DWORD GuildIdx );
void GuildTournamentGuildInfoUpdate( DWORD GuildIdx, DWORD Ranking, DWORD ProcessTournament );
void GuildTournamentInfoUpdate( DWORD TournamentIdx, DWORD TournamentFight, DWORD WaitTime );
void RGuildTournamentInfoLoad( LPQUERY pData, LPDBMESSAGE pMessage );
void RGuildTournamentCancelIdxLoad( LPQUERY pData, LPDBMESSAGE pMessage );
void GuildTournamentAddLog( DWORD TournamentIdx, DWORD GuildIdx, DWORD Logkind, DWORD Ranking );

// Jackpot
//void JackpotLoadTotalMoney();
//void RJackpotLoadTotalMoney( LPQUERY pData, LPDBMESSAGE pMessage );
void JackpotAddTotalMoney( MONEYTYPE MapMoney );
//void RJackpotAddTotalMoney( LPQUERY pData, LPDBMESSAGE pMessage );
void JackpotPrizeInfo( MONEYTYPE MapMoney, DWORD PlayerID, DWORD PrizeKind, DWORD PrizePercentage, DWORD dwAbsMoney = 0);
void RJackpotPrizeInfo( LPQUERY pData, LPDBMESSAGE pMessage );

// guildunion
void GuildUnionLoad( DWORD dwStartGuildUnionIdx );
void RGuildUnionLoad( LPQUERY pData, LPDBMESSAGE pMessage );
void GuildUnionLoadMark( DWORD dwMarkIdx );
void RGuildUnionLoadMark( LPLARGEQUERY pData, LPDBMESSAGE pMessage );

void GuildUnionCreate( DWORD dwCharacterIdx, DWORD dwGuildIdx, char* sUnionName );
void RGuildUnionCreate( LPQUERY pData, LPDBMESSAGE pMessage );
void GuildUnionDestroy( DWORD dwGuildUnionIdx, DWORD dwMarkIdx, DWORD dwMasterIdx, DWORD dwTime );
void GuildUnionAddGuild( DWORD dwGuildUnionIdx, int nIndex, DWORD dwGuildIdx );
void GuildUnionRemoveGuild( DWORD dwGuildUnionIdx, int nIndex, DWORD dwGuildIdx );
void GuildUnionSecedeGuild( DWORD dwGuildUnionIdx, int nIndex, DWORD dwGuildIdx, DWORD dwTime );

void GuildUnionMarkRegist( DWORD dwCharacterIdx, DWORD dwMarkIdx, DWORD dwGuildUnionIdx, char* pImgData );
void RGuildUnionMarkRegist( LPLARGEQUERY pData, LPDBMESSAGE pMessage );

//ΞΜΜΜΜΜΜΜΜΜΜΜΜΜΜΜΜΜΜΜΜΜΜΜΜΜΜΜΜΜΜΜΟ
//ΝSiegeWar													  Ν
//ΡΜΜΜΜΜΜΜΜΜΜΜΜΜΜΜΜΜΜΜΜΜΜΜΜΜΜΜΜΜΜΜΠ
void SiegeWarInfoLoad( DWORD MapNum );
void RSiegeWarInfoLoad( LPQUERY pData, LPDBMESSAGE pMessage );
void SiegeWarInfoInsert( DWORD SiegeWarIdx, DWORD RegistTime, DWORD SiegeWarTime, DWORD Level, DWORD MapNum );
void SiegeWarInfoUpdate( DWORD SiegeWarIdx, DWORD RegistTime, DWORD SiegeWarTime, DWORD Level, DWORD MapNum );
void SiegeWarGuildInfoLoad( DWORD StartDBIdx, DWORD SiegeWarIdx, DWORD MapNum );
void RSiegeWarGuildInfoLoad( LPQUERY pData, LPDBMESSAGE pMessage );
void SiegeWarGuildInsert( DWORD SiegeWarIdx, DWORD GuildIdx, DWORD Type, DWORD MapNum );
void SiegeWarGuildUpdate( DWORD SiegeWarIdx, DWORD GuildIdx, DWORD Type, DWORD MapNum );
void SiegeWarGuildDelete( DWORD SiegeWarIdx, DWORD GuildIdx, DWORD MapNum );
void SiegeWarAddLog( DWORD SiegeWarIdx, DWORD MapNum, DWORD GuildIdx, DWORD Logkind, DWORD Value );

// For JAPAN
void ChangeCharacterAttr( DWORD dwPlayerIdx, DWORD dwAttr );

// siegewarprofit
void SiegeWarProfitInfoLoad();
void RSiegeWarProfitInfoLoad( LPQUERY pData, LPDBMESSAGE pMessage );
void SiegeWarProfitUpdateGuild( int nMapNum, DWORD dwGuildIdx );
void SiegeWarProfitUpdateTaxRate( int nMapNum, int nTaxRate, DWORD dwRegDate );
void SiegeWarProfitUpdateProfitMoney( int nMapNum, DWORD dwMoney, int nType = 0 );
void RSiegeWarProfitUpdateProfitMoney( LPQUERY pData, LPDBMESSAGE pMessage );

void ChangeGuildLocation( DWORD dwGuildIdx, WORD wMapNum );


void ChangeCharacterStageAbility( DWORD dwCharacterIdx, BYTE stage, CAbilityGroup* pGroup );

void QuestEventCheck( DWORD dwUserIdx, DWORD dwCharacterIdx, DWORD dwNpcIdx, DWORD dwQuestIdx, char* pTime );
void RQuestEventCheck( LPQUERY pData, LPDBMESSAGE pMessage );

void QuestEventEnd( DWORD dwUserIdx, DWORD dwCharacterIdx, char* pTime );

// event 060627 - 款康评 捞亥飘
void GMEvent01( DWORD dwUserIdx, DWORD dwCharacterIdx, DWORD dwLevel, int nServerSetNum );

// magi82 //////////////////////////////////////////////////////////////////////////
void UCharacterExpFlag(DWORD CharacterIDX, BYTE ExpFlag);
////////////////////////////////////////////////////////////////////////////////////

//magi82 - 巩窍积 啊涝祈府矫胶袍 ///////////////////////////////////////////////////
void GuildTraineeInfo(DWORD StartDBIdx);
void RGuildTraineeInfo(LPQUERY pData, LPDBMESSAGE pMessage);
void GuildTraineeInsert(DWORD GuildIdx, DWORD UserIdx, char UserName[], DWORD JoinTime);
void GuildTraineeDelete(DWORD UserIdx);

void GuildTraineeGuildInfo(DWORD StartDBIdx);
void RGuildTraineeGuildInfo(LPQUERY pData, LPDBMESSAGE pMessage);
void GuildTraineeGuildInsert(DWORD GuildIdx);
void RGuildTraineeGuildInsert(LPMIDDLEQUERY pData, LPDBMESSAGE pMessage);
void GuildTraineeGuildDelete(DWORD GuildIdx);

void GuildTraineeDeletebyGuild(DWORD GuildIdx);

void GuildAddStudent(DWORD GuildIdx, DWORD MemberIDX, char* GuildName, BYTE Rank);
void RGuildAddStudent(LPQUERY pData, LPDBMESSAGE pMessage);

void GuildTraineeGuildIntroInfo(DWORD StartDBIdx);
void RGuildTraineeGuildIntroInfo(LPMIDDLEQUERY pData, LPDBMESSAGE pMessage);

void GuildTraineeGuildIntroUpdate(DWORD GuildIdx, char* GuildIntro);
////////////////////////////////////////////////////////////////////////////////////

//SW070103 巩颇器牢飘俺急	//巩窍积 肪诀俊 蝶弗 器牢飘 穿利. 绵窍陛
void GuildStudentLvUpCtUpdate(DWORD GuildIdx, DWORD AddPoint, DWORD ToPoint);	//AddPoint - 啊魂痢, ToPoint - 厚背痢
void RGuildStudentLvUpCtUpdate(LPQUERY pData, LPDBMESSAGE pMessage);

void GuildStudentLvUpCtInit(DWORD GuildIdx, DWORD MasterIDX);
void RGuildStudentLvUpCtInit(LPQUERY pData, LPDBMESSAGE pMessage);

// magi82 - Titan(070209)
void TitanWearItemInfo( DWORD CharacterIdx, POSTYPE FromPos, POSTYPE ToPos );
void RTitanWearItemInfo(LPQUERY pData, LPDBMESSAGE pMessage);
//////////////////////////////////////////////////////////////////////////

void TestGameQuery( DWORD CharacterIdx );
void RTestGameQuery( LPQUERY pData, LPDBMESSAGE pMessage );
void TestLogQuery( DWORD CharacterIdx );
void RTestLogQuery( LPQUERY pData, LPDBMESSAGE pMessage );


void PyogukTitanEnduranceInfo(DWORD CharacterIdx, DWORD UserIdx, DWORD StartDBIdx);	// magi82 - Titan(071015) 芒绊俊 鸥捞藕 厘厚 包访
void RPyogukTitanEnduranceInfo( LPQUERY pData, LPDBMESSAGE pMessage );

//2007. 12. 10. CBH - 胶挪 历厘
void CharacterSkinInfoUpdate(CPlayer* pPlayer);
//2008. 1. 23. CBH - 胶挪 肺靛
void CharacterSkinInfo(DWORD dwCharacterIndex);
void RCharacterSkinInfo(LPQUERY pData, LPDBMESSAGE pMessage);


// magi82(41) - 讥酒捞袍 眠啊(胶泡 檬扁拳 林巩辑)
void CharacterUpdateResetStatusPoint(DWORD dwCharacterIDX, DWORD dwStatusPoint);

// itemlimit
void ItemLimitInfoLoadAll( DWORD dwStartDBIdx );
void RItemLimitInfoLoadAll( LPQUERY pData, LPDBMESSAGE pMessage );
void ItemLimitInfoUpdate( DWORD dwItemIdx, int nItemLimitCount, int nItemCurrentCount );
void RItemLimitInfoUpdate( LPQUERY pData, LPDBMESSAGE pMessage );

// autonote
void AutoNoteListLoad( DWORD dwCharacterIdx );
void RAutoNoteListLoad( LPQUERY pData, LPDBMESSAGE pMessage );	
void AutoNoteListAdd( DWORD dwCharacterIdx, DWORD dwAutoUserIdx, DWORD dwAutoCharacterIdx, char* pStrAutoName );
void RAutoNoteListAdd( LPQUERY pData, LPDBMESSAGE pMessage );

// 甘 靛而 酒捞袍 眠啊 by Stiner(2008/05/29)-MapDropItem
void MapItemDropLog( WORD wUserIDX, WORD wCharacterIDX, WORD wItemIDX, WORD wMap, WORD wChannel, WORD wDropCount, WORD wMaxDropCount );
void MapItemDropListSelect( WORD wMapNum );
void MapItemDropListUpdate( DWORD dwCharacterIdx, WORD wMapNum, WORD wChannel, WORD wItemIDX, WORD wDropCount, WORD wMaxDropCount );
void MapItemDropListInit( WORD wMapNum );
void RMapItemDropListSelect( LPQUERY pData, LPDBMESSAGE pMessage );
void RMapItemDropListUpdate( LPQUERY pData, LPDBMESSAGE pMessage );

// 夸货傈
void FortWarInfoLoad();
void RFortWarInfoLoad( LPQUERY pData, LPDBMESSAGE pMessage );
void FortWarInfoUpdate( int nWarIDX, DWORD dwCharacterIdx, char* pCharacterName, DWORD dwGuildIdx );
void FortWarProfitMoneyUpdate( int nWarIDX, DWORD dwMoney, int nType );
void RFortWarProfitMoneyUpdate( LPQUERY pData, LPDBMESSAGE pMessage );
void FortWarItemLoad( int nWarIDX );
void RFortWarItemLoad( LPQUERY pData, LPDBMESSAGE pMessage );
void FortWarItemInsertToDB( int nWarIDX, DWORD dwItemIdx, DURTYPE Durability, POSTYPE wPos, DWORD dwParam );
void RFortWarItemInsertToDB( LPQUERY pData, LPDBMESSAGE pMessage );
void FortWarItemMoveToDB( DWORD dwCharacterIdx, DWORD dwItemDBIdx, WORD wPos, int nWarIDX );

//weiye  结婚相关函数定义  2018-05-01

void MarryAdd(DWORD dwCharacterIdx, char * pTargetName, DWORD dwTargetIdx, char * pCharacterName);
void RMarryAdd(LPQUERY pData, LPDBMESSAGE pMessage);
void MarryDel(DWORD dwCharacterIdx, char* Name);
void RMarryDel( LPQUERY pData, LPDBMESSAGE pMessage ); 

// 2014-12-06 装备数据库函数

void ItemLockUpdate(DWORD dwCharacterIdx,DWORD wdItemDBIdx,WORD wParam); 

void RItemLockUpdate(LPQUERY pData, LPDBMESSAGE pMessage);

// 2014-12-07 装备解锁数据库函数

void ItemUnLockUpdate(DWORD dwCharacterIdx,DWORD wdItemDBIdx,WORD wParam); 

void RItemUnLockUpdate(LPQUERY pData, LPDBMESSAGE pMessage);

// 2015-01-15 成长系统数据库访问函数

void ItemGrowUpdate(DWORD dwCharacterIdx, DWORD dwItemDBIdx, DWORD dwGrow);

void RItemGrowUpdate(LPQUERY pData, LPDBMESSAGE pMessage);

// 2015-01-16 赌博系统相关数据库函数

void GambleUserInfoInsert(DWORD dwCharacterIdx,int Year,int Month,int Day,int Number1,int Number2,int Number3);

void CharacterGambleInfo(DWORD CharacterIDX,int Year,int Month, int Day,DWORD Protocol); 

void CharacterGamblePublicInfo(DWORD CharacterIDX,int Year,int Month, int Day,DWORD Protocol);

void RGambleUserInfoInsert(LPQUERY pData, LPDBMESSAGE pMessage);

void RCharacterGambleInfo(LPQUERY pData, LPDBMESSAGE pMessage);

void RCharacterGamblePublicInfo(LPQUERY pData, LPDBMESSAGE pMessage);

BOOL isValidQueryString( char * str );

BOOL CheckString( const char *strChk );

//weiye 2017-10-21 安全码相关函数定义

void CheckSafePin(DWORD dwCharacterIdx, char * pSafePin);

void RCheckSafePin(LPQUERY pData, LPDBMESSAGE pMessage);

void ChangeSafePin(DWORD dwCharacterIdx, char * pSafePin,char * pNSafePin,WORD Type);

void RChangeSafePin(LPQUERY pData, LPDBMESSAGE pMessage);

void UpdateSafeStatic(DWORD dwCharacterIdx,int Flag);
void RUpdateSafeStatic(LPQUERY pData, LPDBMESSAGE pMessage);

void UpdateLastMapInfo(DWORD dwCharacterIdx,int Flag);
void RUpdateLastMapInfo(LPQUERY pData, LPDBMESSAGE pMessage);

//weiye 2018-02-06 FB副本时间更新函数 

void FBTimeUpdate(DWORD dwCharacterIdx,DWORD fbTime);

void RFBTimeUpdate(LPQUERY pData, LPDBMESSAGE pMessage);

//weiye 发送物品数据库函数定义 2018-03-10

void ItemLinkSelectItemInfo(DWORD dwItemDBIdx,DWORD SelectIdx);

void RItemLinkSelectItemInfo(LPQUERY pData, LPDBMESSAGE pMessage);

//weiye 整理包袱数据库函数定义 2018-03-24

void TidyItem(DWORD dwCharacterIdx,DWORD MaxStack);
void RTidyItem(LPQUERY pData, LPDBMESSAGE pMessage);
void LoadTidyItemInfo(DWORD dwCharacterIdx,DWORD Protocol);
void RLoadTidyItemInfo(LPQUERY pData, LPDBMESSAGE pMessage);

void TidyItemShop(DWORD dwCharacterIdx,DWORD MaxStack);
void RTidyItemShop(LPQUERY pData, LPDBMESSAGE pMessage);
void LoadTidyItemShopInfo(DWORD dwCharacterIdx,DWORD Protocol);
void RLoadTidyItemShopInfo(LPQUERY pData, LPDBMESSAGE pMessage);

void CharacterHeroRebornUpdate(CPlayer* pPlayer);


void InitGuildWarPlayerInfo(DWORD characterIdx,char* pName,char* UnionName,DWORD pKillCount,DWORD pDieCount);
void InitGuildWarUnionInfo(DWORD GuildUnionIdx,char* UnionName,DWORD pKillCount,DWORD pDieCount);
void ClearGuildWarInfo();


void NPCCODE( DWORD CharacterIdx, char* Name, DWORD DBIdx );
void RNPCCODE( LPQUERY pData, LPDBMESSAGE pMessage );


void ItemFlashNameSet(DWORD dwCharacterIdx,DWORD NameFlag);
void RItemFlashNameSet(LPQUERY pData, LPDBMESSAGE pMessage);

void ItemFlashNameSet2(DWORD dwCharacterIdx,DWORD ItemPos,char * pName);
void RItemFlashNameSet2(LPQUERY pData, LPDBMESSAGE pMessage);

void ClearFortWarData();


void RUpdateUserCredit(LPQUERY pData, LPDBMESSAGE pMessage);  //在线充值元宝刷新数据库回调处理函数定义	by:胡汉三	QQ:112582793
void CharacterHeroGoldInfoUpdate(DWORD ID,DWORD GoldMoney); 

// 牢带包访
void DungeonEntrance(DWORD dwConnectionIndex, DWORD dwPlayerMapNum, DWORD dwPlayerIndex, DWORD dwPartyIndex, DWORD dwKeyIndex, DWORD dwItemIndex, DWORD dwItemSLot, WORD wDungeonMapNum, eDIFFICULTY difficulty,DWORD dwMode);
void RDungeonEntrance(LPMIDDLEQUERY query, LPDBMESSAGE dbMessage);
void DungeonUpdate(DWORD dwPlayerIndex, DWORD wDungeonMapNum, DWORD wTotalVisit, DWORD wDailyVisit, BYTE byDoRollBack = 0);

void MallBuyItem( DWORD dwConnectionIndex, CPlayer* pPlayer,MSG_ITEM_BUY_SYN * bMsg);
void RMallBuyItem(LPMIDDLEQUERY query, LPDBMESSAGE dbMessage);
void MallBuyItemUpdate(DWORD dwPlayerIndex, DWORD wDungeonMapNum, DWORD wTotalVisit, DWORD wDailyVisit, BYTE byDoRollBack = 0);


void MallListInfo(DWORD dwCharacterIdx,DWORD Type);
void RMallListInfo(LPQUERY pData, LPDBMESSAGE pMessage );

void RUpdateUserCredit(LPQUERY pData, LPDBMESSAGE pMessage);  //在线充值元宝刷新数据库回调处理函数定义	by:胡汉三	QQ:112582793
void CharacterHeroGoldInfoUpdate(DWORD ID,DWORD GoldMoney); 
void LogGoldMoney(WORD LogType,WORD FromChrID,DWORD FromTotalGold,WORD ToChrID,DWORD ToChrTotalGold,DWORD ChangeGold,DWORD BuyItemIdx,DWORD Durability=0);



void DGEntrance(DWORD dwConnectionIndex, DWORD dwPlayerMapNum, DWORD dwPlayerIndex, DWORD dwPartyIndex, WORD wDungeonMapNum,DWORD dwChannelIndex);
void RDGEntrance(LPMIDDLEQUERY query, LPDBMESSAGE dbMessage);
void DGUpdate(DWORD dwPartyIndex, DWORD wDungeonMapNum, DWORD wTotalVisit, DWORD wTimerCount, DWORD wSubMission ,BYTE byDoRollBack = 0);
void RDGUpdate(LPMIDDLEQUERY query, LPDBMESSAGE dbMessage);


void ForbidChatUpdate( DWORD dwPlayerIdx, DWORD dwTargetIdx, const char* pReason, DWORD dwForbidTime, BOOL bIsForbidAdd );
void RForbidChatUpdate( LPQUERY pData, LPDBMESSAGE pMessage );
void ForbidChatLoad( DWORD dwPlayerIdx );
void RForbidChatLoad( LPQUERY pData, LPDBMESSAGE pMessage );


void CharacterLoginEventUpdate(CPlayer* pPlayer);

void UpdateInstanceDungeonRank(DWORD dwCharacterIdx, INSDG_RANK_INFO* pRankInfo);
void LoadInstanceDungeonRank(DWORD dwPlayerID);
void RLoadInstanceDungeonRank(LPQUERY pData, LPDBMESSAGE pMessage);
void UpdateGradeItem(DWORD dwPlayerID, DWORD ItemDbIdx, DWORD Grade, DWORD ItemStatic);
void RUpdateGradeItem(LPQUERY pData, LPDBMESSAGE pMessage);

void ItemQualityUpdateToDB(DWORD CharacterIdx, DWORD dwDBIdx, WORD wItemIdx, DURTYPE Durability, POSTYPE bPosition, WORD qPosition, DWORD RareIdx = 0, WORD bStatic = 0, WORD Grade = 0 ,WORD ItemQuality = 0, WORD ItemEntry1 = 0, WORD ItemEntry2 = 0, WORD ItemEntry3 = 0);
void RItemQualityUpdate(LPQUERY pData, LPDBMESSAGE pMessage);
void ItemQualityChangeUpdateToDB(DWORD CharacterIdx, DWORD dwDBIdx, WORD wItemIdx, DURTYPE Durability, POSTYPE bPosition, WORD qPosition, DWORD RareIdx/*=0*/, WORD bStatic /*= 0*/, WORD Grade, WORD ItemQuality/*=0*/, WORD ItemEntry1/*=0*/, WORD ItemEntry2/*=0*/, WORD ItemEntry3/*=0*/);
void RItemQualityChangeUpdate(LPQUERY pData, LPDBMESSAGE pMessage);
//商城使用日志
void ItemShopUseLog(WORD Type, DWORD dwChrID, char* CharName, WORD wItemIdx, char* ItemName, WORD wItemNum, DWORD TotalMall, DWORD UseMall, DWORD LastMall, DWORD TotalGold, DWORD UseGold, DWORD LastGold, DWORD TotalMoney, DWORD UseMoney, DWORD LastMoney);


#endif //__MAPBMSGPARSER_H__



