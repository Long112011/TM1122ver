#include "stdafx.h"
#include "GlobalEventFunc.h"
#include "MainGame.h"
#include "CharMake.h"
#include "CharSelect.h"
#include "GameIn.h"
#include "MainTitle.h"
#include "cComboBoxEx.h"
#include "cComboBox.h"
#include "ObjectManager.h"
#include "MoveManager.h"
#include "AppearanceManager.h"
#include "WindowIdEnum.h"
#include "./interface/cWindowManager.h"
#include "PartyManager.h"
#include "GuildManager.h"
#include "PyogukManager.h"
#include "FriendManager.h"
#include "NoteManager.h"
#include "ChatManager.h"
#include "ItemManager.h"
#include "ExchangeManager.h"
#include "StreetStallManager.h"
#include "VimuManager.h"
#include "FilteringTable.h"
#include "CharMakeManager.h"
#include "WantedManager.h"
#include "PetManager.h"
#include "../[CC]Suryun/SuryunManager_client.h"
#include "ChannelDialog.h"
#include "CharacterDialog.h"
#include "InventoryExDialog.h"
#include "MugongDialog.h"
#include "StallKindSelectDlg.h"
#include "StreetBuyStall.h"
#include "BuyRegDialog.h"
#include "StreetStall.h"
#include "BuyItem.h"
#include "cListDialogEx.h"
#include "NpcScriptDialog.h"
#include "HelpDialog.h"
#include "HelperManager.h"
#include "HelperSpeechDlg.h"
#include "MoneyDlg.h"
#include "DealDialog.h"
#include "QuickDialog.h"
#include "MiniFriendDialog.h"
#include "FriendDialog.h"
#include "MiniNoteDialog.h"
#include "ChatDialog.h"
#include "NoteDialog.h"
#include "MacroDialog.h"
#include "ChatOptionDialog.h"
#include "OptionDialog.h"
#include "ExchangeDialog.h"
#include "CharStateDialog.h"
#include "MenuSlotDialog.h"
#include "PKLootingDialog.h"
#include "PyogukDialog.h"
#include "MiniMapDlg.h"
#include "CharMakeDialog.h"
#include "MugongSuryunDialog.h"
#include "SuryunDialog.h"
#include "ExitDialog.h"
#include "UpgradeDlg.h"
#include "MixDialog.h"
#include "ReinforceDlg.h"
#include "WantNpcDialog.h"
#include "WantRegistDialog.h"
#include "QuestDialog.h"
#include "JournalDialog.h"
#include "PartyCreateDlg.h"
#include "PartyInviteDlg.h"
#include "QuestManager.h"
#include "MPRegistDialog.h"
#include "GuildDialog.h"
#include "GuildCreateDialog.h"
#include "GuildMunhaDialog.h"
#include "GuildRankDialog.h"
#include "GuildMarkDialog.h"
#include "GuildNickNameDialog.h"
#include "GuildWarehouseDialog.h"
#include "TitanPartsMakeDlg.h"		
#include "TitanMixDlg.h"			
#include "TitanRepairDlg.h"			
#include "TitanRecallDlg.h"			
#include "TitanInventoryDlg.h"		
#include "TitanGuageDlg.h"			
#include "TitanRegisterDlg.h"		
#include "TitanDissolutionDlg.h"	
#include "TitanUpgradeDlg.h"
#include "TitanBreakDlg.h"
#include "TitanManager.h"
#include "MHMap.h"
#include "MHCamera.h"
#include "cMsgBox.h"
#include "cDivideBox.h"
#include "./input/UserInput.h"
#include "./Audio/MHAudioManager.h"
#include "cImeEx.h"
#include "PKManager.h"
#include "ObjectStateManager.h"
#include "ReviveDialog.h"
#include "MainBarDialog.h"
#include "UserInfoManager.h"
#include "JournalManager.h"
#include "ExitManager.h"
#include "BailDialog.h"
#include "InventoryExDialog.h"
#include "PartyIconManager.h"
#include "ExchangeItem.h"
#include "DissolutionDialog.h"
#include "GuildMarkManager.h"
#include "ItemShopDialog.h"
#include "MoveDialog.h"
#include "PointSaveDialog.h"
#include "GuildFieldWarDialog.h"
#include "GuildFieldWar.h"
#include "MallNoticeDialog.h"
#include "Shellapi.h"
#include "SkillPointRedist.h"
#include "SkillPointNotify.h"
#include "BarIcon.h"
#include "MainBarDialog.h"
#include "GameEventManager.h"
#include "PartyWar.h"
#include "ShoutDialog.h"
#include "ChaseDialog.h"
#include "ChaseinputDialog.h"
#include "NameChangeDialog.h"
#include "NameChangeNotifyDlg.h"
#include "GTRegistDialog.h"
#include "GTRegistcancelDialog.h"
#include "GTStandingDialog.h"
#include "GTBattleListDialog.h"
#include "GTScoreInfoDialog.h"
#include "GuildUnion.h"
#include "GuildUnionMarkMgr.h"
#include "SeigeWarDialog.h"
#include "CharChangeDlg.h"
#include "SealDialog.h"
#include "ChangeJobDialog.h"
#include "SiegeWarMgr.h"
#include "GuageDialog.h"
#include "ReinforceResetDlg.h"
#include "RareCreateDialog.h"
#include "ReinforceDataGuideDlg.h"
#include "TipBrowserDlg.h"
#include "PetStateDlg.h"
#include "PetStateMiniDlg.h"
#include "PetInventoryDlg.h"
#include "PetUpgradeDialog.h"
#include "PetRevivalDialog.h"
#include "MNStgPlayRoom.h"
#include "MNPlayRoomDialog.h"
#include "MNStgChannel.h"
#include "MNChannelDialog.h"
#include "MurimNet.h"
#include "MNPlayerManager.h"
#include "MNPlayer.h"
#include "ServerListDialog.h"
#include "AbilityManager.h"
#include "../Interface/cResourceManager.h"
#include "GuildNoteDlg.h"
#include "UnionNoteDlg.h"
#include "GuildNoticeDlg.h"
#include "SkillOptionChangeDlg.h"
#include "SkillOptionClearDlg.h"
#include "guildjoindialog.h"
#include "GuildTraineeDialog.h"
#include "GuildPlusTimeDialog.h"
#include "GuildMunhaDialog.h"
#include "UniqueItemCurseCancellationDlg.h"	
#include "UniqueItemMixDlg.h"				
#include "UniqueItemMixProgressBarDlg.h"	
#include "TitanMixProgressBarDlg.h"	
#include "TitanPartsProgressBarDlg.h"	
#include "SkinSelectDialog.h"	
#include "CostumeSkinSelectDialog.h"	
#include "SOSDialog.h"
#include "SkillPointResetDlg.h"	
#include "NumberPadDialog.h"	
#include "StallFindDlg.h"	
#include "StreetStallItemViewDlg.h"
#include "AutoNoteDlg.h"	
#include "AutoAnswerDlg.h"
#include "FortWarDialog.h"
#include "PartyMatchingDlg.h"
#include "GoldShopDialog.h"
#include "GoldShopBuyDialog.h"
#include "MiniMarryDialog.h"
#include "MiniBaishiDialog.h"
#include "ControlDialog.h"
#include "RankButtonDialog.h" 
#include "TopDialog.h"
#include "TopMunpDialog.h"
#include "LuckerDialog.h"
#include "OtherCharacterMoneyDlg.h"
#include "OtherCharacterAmuletDlg.h"
#include "ItemStoneDkDialog.h"  
#include "ItemStoneXqDialog.h"  
#include "ItemStepReinforceDialog.h"  
#include "ItemLockManager.h"
#include "ItemUnLockManager.h"
#include "ItemGambleManager.h"
#include "MHTimeManager.h"
//#include "SafeNumberDialog.h"
#include "SafeNumberImg.h"  
#include "Intro.h"
#include <wininet.h>
//#include "SafeNumberDialog.h"
#include "SafeNumberIcon.h"
//#include "SafeNumberLockDialog.h"  
#include "TopIcon.h"   
#include "FBTimeDialog.h"  
#include "FameGoldDialog.h" 
//#include "CharacterControlDialog.h"  
#include "SendEquipDialog.h"           
//#include "SafeNumberChangeDialog.h"  
#include "VideoCaptureDlg.h"           
#include "VideoCaptureIcon.h"          
#include "Reconnect.h"
#include "HackCheck.h"
#include "GuildWarManager.h"
#include "HideBuff.h"

#include "AdPointDlg.h"

#include "ReconnectDialog.h"

#include "RegID.h"
#include "RestPwd.h"

#include "CharacterRBTN.h"
#include "CodeDialog.h"

#include "InsDGEntranceInfoDlg.h"


#include "LuckyDrawIcon.h"

#include "FortWarManager.h"
#include ".\rarecreatedialog.h"

#include "ChangeLookDialog.h"

#include "ItemDropInfoDialog.h"
#include "SendEquipSelectDialog.h"

#include "StatusIconDlg.h"

#include ".\petmultidlg.h"

#include "FPSDialog.h"

#include "TopDungeon.h"
#include "InstancesDungeon/InsDGInfoDialog.h"
#include "InstancesDungeon/InsDGEntranceInfoDialog.h"
#include "InstancesDungeon/InsDGPartyMakeDialog.h"
#include "InstancesDungeon/InsDGRankDialog.h"
#include "CharacterPVPDialog.h"
#include "GradeChangeDlg.h"		//武器升阶值转移卷
#include "OfficialUpGradeDlg.h"
#include "VipDialog.h"              //VIP头文件引用
#include "CustomizingNameDlg.h" //自定义名字
#ifdef  _MUTIPET_
#include "PetMixDlg.h"//刀哥  3pet
#endif //  _MUTIPET_
extern HWND _g_hWnd;
extern BOOL jTweak;
//extern BOOL   g_IsExit;
extern char g_DistributeAddr[16];
extern WORD g_DistributePort;
extern int	g_nServerSetNum; 
//extern int m_ServerIndex;   
//extern int m_CharIndex;     
FUNC g_mt_func[] =
{
	{MT_LogInOkBtnFunc,"MT_LogInOkBtnFunc"},
	{MT_EditReturnFunc, "MT_EditReturnFunc"},
	{MT_ExitBtnFunc,"MT_ExitBtnFunc"},
	{MT_DynamicBtnFunc,"MT_DynamicBtnFunc"},
	{MT_DynamicEditReturnFunc, "MT_DynamicEditReturnFunc"},
	{CS_BtnFuncCreateChar,"CS_BtnFuncCreateChar"}, 
	{CS_BtnFuncDeleteChar,"CS_BtnFuncDeleteChar"},	 
	{CS_BtnFuncFirstChar,"CS_BtnFuncFirstChar"},	 
	{CS_BtnFuncSecondChar, "CS_BtnFuncSecondChar"},
	{CS_BtnFuncThirdChar,"CS_BtnFuncThirdChar"},
	{CS_BtnFuncFourthchar,"CS_BtnFuncFourthchar"},
	{CS_BtnFuncFifthchar,"CS_BtnFuncFifthchar"},
	{CS_BtnFuncEnter,"CS_BtnFuncEnter"},
	{CS_FUNC_OkISee,"CS_FUNC_OkISee"},
	{CS_BtnFuncLogOut,"CS_BtnFuncLogOut"},
	{MI_CharBtnFunc, "MI_CharBtnFunc"},
	{MI_ExchangeBtnFunc, "MI_ExchangeBtnFunc"},
	{MI_InventoryBtnFunc, "MI_InventoryBtnFunc"},
	{MI_MugongBtnFunc, "MI_MugongBtnFunc"},
	{MI_ChatEditBoxFunc, "MI_ChatEditBoxFunc"},
	{IN_CreateStreetStallFunc, "IN_CreateStreetStallFunc"},
	{IN_DlgFunc, "IN_DlgFunc"}, 
	{MGI_DlgFunc, "MGI_DlgFunc"},	
	{MGI_SuryunDlgFunc, "MGI_SuryunDlgFunc"},	
	{MGI_MugongDlgFunc, "MGI_MugongDlgFunc"},	
	{CI_DlgFunc, "CI_DlgFunc"},
	{CI_AddExpPoint, "CI_AddExpPoint"},
	{PA_DlgFunc, "PA_DlgFunc"},
	{SO_DlgFunc, "SO_DlgFunc"},
	{BRS_DlgFunc, "BRS_DlgFunc"},
	{BRS_CloseFunc, "BRS_CloseFunc"},
	{BRS_DeleteFunc, "BRS_DeleteFunc"},
	{BRS_TypeListFunc, "BRS_TypeListFunc"},
	{BRS_ItemListFunc, "BRS_ItemListFunc"},
	{BRS_ClassListFunc, "BRS_ClassListFunc"},
	{BRS_VolumeEditFunc, "BRS_VolumeEditFunc"},
	{BRS_MoneyEditFunc, "BRS_MoneyEditFunc"},
	{BRS_REGBtnFunc, "BRS_REGBtnFunc"},
	{BS_DlgFunc, "BS_DlgFunc"},
	{BS_TitleEditBoxFunc, "BS_TitleEditBoxFunc"},
	{BS_MoneyEditBoxFunc, "BS_MoneyEditBoxFunc"},
	{BS_SellBtnFunc, "BS_SellBtnFunc"}, 
	{BS_REGBtnFunc, "BS_REGBtnFunc"}, 
	{BS_SPFunc, "BS_SPFunc"},	
	{SSI_DlgFunc, "SSI_DlgFunc"},
	{SSI_TitleEditBoxFunc, "SSI_TitleEditBoxFunc"},
	{SSI_MoneyEditBoxFunc, "SSI_MoneyEditBoxFunc"},
	{SSI_BuyBtnFunc, "SSI_BuyBtnFunc"}, 
	{SSI_RegistBtnFunc, "SSI_RegistBtnFunc"}, 
	{SSI_EditBtnFunc, "SSI_EditBtnFunc"}, 
	{NSI_DlgFunc, "NSI_DlgFunc"}, 
	{NSI_HyperLinkFunc, "NSI_HyperLinkFunc"},
	{HI_DlgFunc, "HI_DlgFunc"}, 
	{HI_HyperLinkFunc, "HI_HyperLinkFunc"},
	{HI_BtnFunc, "HI_BtnFunc"},
	{HSI_ExitBtnFunc, "HSI_ExitBtnFunc"},
	{CMI_MoneyOkFunc, "CMI_MoneyOkFunc"},
	{CMI_MoneySpinFunc, "CMI_MoneySpinFunc"}, 
	{DE_BuyItemFunc, "DE_BuyItemFunc"},
	{DE_SellItemFunc, "DE_SellItemFunc"},
	{DE_DlgFunc, "DE_DlgFunc"},
	{DE_CloseHideBtn, "DE_CloseHideBtn"},
	{PA_BtnFunc, "PA_BtnFunc"},
	{QI_QuickDlgFunc, "QI_QuickDlgFunc"},
	{CM_OverlapCheckBtnFunc, "CM_OverlapCheckBtnFunc"},
	{CM_CharMakeBtnFunc, "CM_CharMakeBtnFunc"},
	{CM_CharCancelBtnFunc, "CM_CharCancelBtnFunc"}, 
	{CM_ComboBoxCheckFunc,"CM_ComboBoxCheckFunc"}, 
	{CMFUNC_OkISee,"CMFUNC_OkISee"}, 
	{CM_OptionSelectBtnFunc, "CM_OptionSelectBtnFunc"},
	{PYO_DlgBtnFunc, "PYO_DlgBtnFunc"},
	{Note_DlgBtnFunc, "Note_DlgBtnFunc"},
	{Friend_DlgBtnFunc, "Friend_DlgBtnFunc"},
	{CR_DlgBtnFunc, "CR_DlgBtnFunc"},
	{MNM_DlgFunc,"MNM_DlgFunc"},
	{ITD_DlgFunc,"ITD_DlgFunc"},
	{ITMD_DlgFunc,"ITMD_DlgFunc"},  
	{ITR_DlgFunc, "ITR_DlgFunc"}, 
	{CHA_DlgBtnFunc, "CHA_DlgBtnFunc"},
	{MAC_DlgFunc, "MAC_DlgFunc"},
	{CTI_DlgFunc, "CTI_DlgFunc"},	
	{COI_DlgFunc, "COI_DlgFunc"},	
	{OTI_DlgFunc, "OTI_DlgFunc"},	
	{EXT_DlgFunc, "EXT_DlgFunc"},	
	{XCI_DlgFunc, "XCI_DlgFunc"},	
	{CSS_DlgFunc, "CSS_DlgFunc"},	
	{MSI_MenuSlotDlgFunc, "MSI_MenuSlotDlgFunc"},	
	{PLI_DlgFunc, "PLI_DlgFunc"},
	{CNA_BtnOkFunc, "CNA_BtnOkFunc"},
	{DIS_DlgFunc, "DIS_DlgFunc"},	
	{MI_DlgFunc, "MI_DlgFunc"},
	{DIVIDEBOX_Func, "DIVIDEBOX_Func"},	
	{MNPRI_DlgFunc, "MNPRI_DlgFunc"},
	{MNCNL_DlgFunc, "MNCNL_DlgFunc"}, 
	{SL_DlgBtnFunc, "SL_DlgBtnFunc" },
	{WANTNPC_DlgFunc, "WANTNPC_DlgFunc"},
	{WANTREG_DlgFunc, "WANTREG_DlgFunc"},
	{QUE_QuestDlgFunc, "QUE_QuestDlgFunc"},
	{QUE_JournalDlgFunc, "QUE_JournalDlgFunc"},	
	{MP_RegistDlgFunc, "MP_RegistDlgFunc"},
	{BAIL_BailDlgFunc, "BAIL_BailDlgFunc"},
	{ITMALL_DlgBtnFunc, "ITMALL_DlgBtnFunc"},
	{SA_DlgBtnFunc, "SA_DlgBtnFunc"},
	{CHA_DlgFunc, "CHA_DlgFunc"},
	{GD_DlgFunc, "GD_DlgFunc"},
	{ITEM_DlgFunc, "ITEM_DlgFunc" },
	{TC_DlgFunc, "TC_DlgFunc" },  
	{TCBuy_DlgFunc,"TCBuy_DlgFunc"},
	{GFW_DlgFunc, "GFW_DlgFunc" },
	{SK_DlgBtnFunc, "SK_DlgBtnFunc"},
	{PW_DlgFunc, "PW_DlgFunc"},
	{CHS_DlgFunc, "CHS_DlgFunc"},
	{CHAN_DlgFunc, "CHAN_DlgFunc"},
	{GDT_DlgFunc, "GDT_DlgFunc"},
	{SW_DlgFunc, "SW_DlgFunc" },
	{CHA_ChangeDlgFunc, "CHA_ChangeDlgFunc" },
	{IT_DlgFunc, "IT_DlgFunc"},
	{CJOB_DlgFunc, "CJOB_DlgFunc"},
	{CG_DlgFunc, "CG_DlgFunc"},
	{RFDefault_DlgFunc, "RFDefault_DlgFunc"},
	{RareCreate_DlgFunc, "RareCreate_DlgFunc"},
	{ItemStoneDk_DlgFunc,"ItemStoneDk_DlgFunc"},  
	{ItemStoneXq_DlgFunc,"ItemStoneXq_DlgFunc"},  
	{PET_DlgFunc, "PET_DlgFunc"},
	{PET_MiniDlgFunc, "PET_MiniDlgFunc"},
	{ PET_InvenDlgFunc, "PET_InvenDlgFunc" },
	{ PET_UpgradeDlgFunc, "PET_UpgradeDlgFunc" },
	{ PET_RevivalDlgFunc, "PET_RevivalDlgFunc" },
	{ RFDGUIDE_DlgFunc, "RFDGUIDE_DlgFunc" },
	{ TB_DlgFunc, "TB_DlgFunc" },
	{ TB_STATE_DlgFunc, "TB_STATE_DlgFunc" },
	{ GN_DlgBtnFunc, "GN_DlgBtnFunc" },
	{ AN_DlgBtnFunc, "AN_DlgBtnFunc" },
	{ GNotice_DlgBtnFunc, "GNotice_DlgBtnFunc" },
	{ GuildPlusTime_DlgFunc, "GuildPlusTime_DlgFunc" },
	{ SkillTrans_DlgFunc, "SkillTrans_DlgFunc" },
	{ TDefault_DlgFunc, "TDefault_DlgFunc" },
	{ JO_DlgFunc, "JO_DlgFunc" },
	{ GU_DlgFunc, "GU_DlgFunc" },
	{ GT_DlgFunc, "GT_DlgFunc" },
	{ TPMD_DlgFunc, "TPMD_DlgFunc" },
	{ TitanMix_DlgFunc, "TitanMix_DlgFunc" },
	{ Titan_Repair_DlgFunc, "Titan_Repair_DlgFunc" },
	{ Titan_Recall_DlgFunc, "Titan_Recall_DlgFunc" },
	{ TitanUpgrade_DlgFunc, "TitanUpgrade_DlgFunc" },
	{ TitanBreak_DlgFunc, "TitanBreak_DlgFunc" },
	{ titan_inventory_DlgFunc, "titan_inventory_DlgFunc" },
	{ Titan_guage_DlgFunc, "Titan_guage_DlgFunc" },
	{ Titan_Use_DlgFunc, "titan_Use_DlgFunc" },
	{ Titan_Bongin_DlgFunc, "Titan_Bongin_DlgFunc" },
	{ UniqueItemCurseCancellationDlgFunc, "UniqueItemCurseCancellationDlgFunc" },
	{ UniqueItemMixDlgFunc, "UniqueItemMixDlgFunc" },
	{ SOSDlgFunc, "SOS_DlgFunc" },
	{ UniqueItemMix_ProgressBar_DlgFunc, "UniqueItemMix_ProgressBar_DlgFunc" },
	{ TitanMix_ProgressBar_DlgFunc, "TitanMix_ProgressBar_DlgFunc" },
	{ TitanParts_ProgressBar_DlgFunc, "TitanParts_ProgressBar_DlgFunc" },
	{ SkinSelect_DlgFunc, "SkinSelect_DlgFunc" },
	{ CostumeSkinSelect_DlgFunc, "CostumeSkinSelect_DlgFunc" },
	{ SK_Reset_DlgFunc, "SK_Reset_DlgFunc" },
	{ NumberPad_DlgFunc, "NumberPad_DlgFunc" },
	{ MT_LOGINDLGFunc, "MT_LOGINDLGFunc" },
	{ SVD_DlgFunc, "SVD_DlgFunc" },
	{ SFR_DlgFunc, "SFR_DlgFunc" },
	{ AutoNoteDlg_Func, "AutoNoteDlg_Func" },
	{ AutoAnswerDlg_Func, "AutoAnswerDlg_Func" },
	{ PA_CreateBtnFunc, "PA_CreateBtnFunc" },
	{ Party_MatchingList_DlgFunc, "Party_MatchingList_DlgFunc" },
	{ FW_DlgFunc, "FW_DlgFunc" },
	{ FD_WareHouseDlgFunc, "FD_WareHouseDlgFunc" },
	{ Marry_DlgBtnFunc, "Marry_DlgBtnFunc" },
	{ ShiTu_DlgBtnFunc, "ShiTu_DlgBtnFunc" },
	{ BARCONTROL_DIALOGFunc, "BARCONTROL_DIALOGFunc" },
	{ Ranking_DlgFunc, "Ranking_DlgFunc" },
	{ DGRanking_DlgFunc, "DGRanking_DlgFunc" },
	{ RankingGuild_DlgFunc, "RankingGuild_DlgFunc" },
	{ Lucker_DlgFunc, "Lucker_DlgFunc" },
	{ IT_LOCKDlgFunc, "IT_LOCKDlgFunc" },
	{ IT_UNLOCKDlgFunc, "IT_UNLOCKDlgFunc" },
	{ CG_TargetObjectBtnFunc, "CG_TargetObjectBtnFunc" },
	{ ITEMGAMBLE_DlgFunc, "ITEMGAMBLE_DlgFunc" },
	//{Safe_DlgBtnFunc,"Safe_DlgBtnFunc"},           
	{ Safe_ICONBtnFunc, "Safe_ICONBtnFunc" },
	{ Safe_IMGBtnFunc, "Safe_IMGBtnFunc" },
	//{SafeLock_DlgBtnFunc,"SafeLock_DlgBtnFunc"},  
	//{SafeChange_DlgBtnFunc,"SafeChange_DlgBtnFunc"}, 
	{ TOP_ICONBtnFunc, "TOP_ICONBtnFunc" },
	{ FAMEGOLD_DlgFunc, "FAMEGOLD_DlgFunc" },
	//{CG_CHARCONTROLBtnFunc,"CG_CHARCONTROLBtnFunc"}, 
	{ SENDEQUIP_BtnFunc, "SENDEQUIP_BtnFunc" },
	{ VIDEOCAPTUREDlg_Func, "VIDEOCAPTUREDlg_Func" },
	{ VIDEOCAPTUREICON_Func, "VIDEOCAPTUREICON_Func" },
	{ CD_DlgFunc, "CD_DlgFunc" },
	{ RANKBUTTON_DIALOGFunc, "RANKBUTTON_DIALOGFunc" },
	{ InsFUBENInfo_DlgFunc, "InsFUBENInfo_DlgFunc" },

	{ HIDE_BuffDlgFunc, "HIDE_BuffDlgFunc" },
	{ TB_AdPointDlgFunc, "TB_AdPointDlgFunc" },

	{ Reconnect_DlgFunc, "Reconnect_DlgFunc" },

	{ CI_RegIDDlgFunc, "CI_RegIDDlgFunc" },

	{ RCM_DlgFunc, "RCM_DlgFunc" },

	{ InsFUBENEntranceInfo_DlgFunc, "InsFUBENEntranceInfo_DlgFunc" },

	{ LuckyDraw_DlgFunc, "LuckyDraw_DlgFunc" },

	{ ChangeLook_DlgFunc, "ChangeLook_DlgFunc" },

	{ ItemDrop_DlgFunc, "ItemDrop_DlgFunc" },

	//{ ItemLink_DlgFunc, "ItemLink_DlgFunc" },

	{ PetMulti_DlgFunc, "PetMulti_DlgFunc" },

	{ FPSDialog_Func, "FPSDialog_Func" },

	{ InsDGInfo_DlgFunc, "InsDGInfo_DlgFunc" },
	{ InsDGEntranceInfo_DlgFunc, "InsDGEntranceInfo_DlgFunc" },
	{ InsDGPartyMake_DlgFunc, "InsDGPartyMake_DlgFunc" },
	{ InsDGRank_DlgFunc, "InsDGRank_DlgFunc" },

	{ ItemStepReinforce_DlgFunc, "ItemStepReinforce_DlgFunc" },

	{ LoginIDDialog_hide, "LoginIDDialog_hide" },

	{CS_BtnChangePass,"CS_BtnChangePass"},
	{ OfficialUpGradeDlg_Func, "OfficialUpGradeDlg_Func" },
	{ ItemQuality_DlgFunc,"ItemQuality_DlgFunc" },
	{ ItemQualityChange_DlgFunc,"ItemQualityChange_DlgFunc" },
	{ GGD_Func, "GGD_Func" }, //武器升阶值转移卷
	{ VIP_BtnFunc,"VIP_BtnFunc" },                      //VIP驱动函数定义
	{ Customizing_DlgFunc, "Customizing_DlgFunc" },//自定义名字
#ifdef _MUTIPET_
	{ PetMerge_DlgFunc, "PetMerge_DlgFunc" },//刀哥 3pet
#endif // _MUTIPET_

	{NULL, ""},	
};
int FUNCSEARCH(char * funcName)
{
	for(int i = 0 ;; i++)
	{
		if(g_mt_func[i]._func == NULL)
			break;
		if(strcmp(funcName, g_mt_func[i]._funcName)==0)
				return i;
	}
	return -1; 
}
extern void CMFUNC_OkISee(LONG lId, void* p, DWORD we)
{
	cStatic * CharacterMessageDlg = (cStatic *)WINDOWMGR->GetWindowForIDEx(CMID_MessageBox);
	CharacterMessageDlg->SetActive(FALSE);
}
void CS_BtnFuncLogOut(LONG lId, void* p, DWORD we)
{
	CHARSELECT->BackToMainTitle();
	TITLE->SetServerList();
	CAMERA->ResetInterPolation();
}
void CS_BtnFuncCreateChar(LONG lId, void* p, DWORD we)
{
	if(CHARSELECT->IsFull() == FALSE)
	{
		MAINGAME->SetGameState(eGAMESTATE_CHARMAKE);
		CAMERA->MoveStart(eCP_CHARMAKE);
	}
	else
	{		
		CHARSELECT->DisplayNotice(17);		
	}
}
void CS_BtnFuncFirstChar(LONG lId, void* p, DWORD we)
{
	if( we & WE_PUSHDOWN )
	{
		CHARSELECT->SelectPlayer(0);
		REDISCONN->SetCharIdx(0);
		//m_CharIndex=0;  
	}
	else if( we & WE_PUSHUP )
		((cPushupButton*)((cDialog*)p)->GetWindowForID(lId))->SetPush( TRUE );
}
void CS_BtnFuncSecondChar(LONG lId, void* p, DWORD we)
{
	if( we & WE_PUSHDOWN )
	{
		CHARSELECT->SelectPlayer(1);
		REDISCONN->SetCharIdx(1);
		//m_CharIndex=1;  
	}
	else if( we & WE_PUSHUP )
		((cPushupButton*)((cDialog*)p)->GetWindowForID(lId))->SetPush( TRUE );
}
void CS_BtnFuncThirdChar(LONG lId, void* p, DWORD we)
{
	if( we & WE_PUSHDOWN )
	{
		CHARSELECT->SelectPlayer(2);
		REDISCONN->SetCharIdx(2);
		//m_CharIndex=2;  
	}
	else if( we & WE_PUSHUP )
		((cPushupButton*)((cDialog*)p)->GetWindowForID(lId))->SetPush( TRUE );
}
void CS_BtnFuncFourthchar(LONG lId, void* p, DWORD we)
{
	if( we & WE_PUSHDOWN )
	{
		CHARSELECT->SelectPlayer(3);
		REDISCONN->SetCharIdx(3);
		//m_CharIndex=3;  
	}
	else if( we & WE_PUSHUP )
		((cPushupButton*)((cDialog*)p)->GetWindowForID(lId))->SetPush( TRUE );
}
void CS_BtnFuncFifthchar(LONG lId, void* p, DWORD we)
{
	if( we & WE_PUSHDOWN )
	{
		CHARSELECT->SelectPlayer(4);
		REDISCONN->SetCharIdx(4);
		//m_CharIndex=4;  
	}
	else if( we & WE_PUSHUP )
		((cPushupButton*)((cDialog*)p)->GetWindowForID(lId))->SetPush( TRUE );
}
void CS_BtnFuncEnter(LONG lId, void* p, DWORD we)
{
	if(	CHARSELECT->GetCurSelectedPlayer() == NULL)
		CHARSELECT->DisplayNotice(18);
	else
	{
		CHARSELECT->SendMsgGetChannelInfo();
		CHARSELECT->SetDisablePick(FALSE);
	} 
}
void CS_FUNC_OkISee(LONG lId, void* p, DWORD we)	 
{
	cStatic * CharacterSelectMessageDlg = (cStatic *)WINDOWMGR->GetWindowForIDEx(CSID_MessageBox);
	CharacterSelectMessageDlg->SetActive(FALSE);
}
void CS_BtnFuncDeleteChar(LONG lId, void* p, DWORD we)
{
	int nChar = CHARSELECT->GetCurSelectedPlayerNum();
	int MaxChar = MAX_CHARACTER_NUM-1;
#ifdef _JAPAN_LOCAL_
	MaxChar = GIVEN_CHARACTER_SLOT-1+CHARSELECT->GetExtraCharacterSlot();
	int msgposx[MAX_CHARACTER_NUM] = { 130, 370, 550, 500, 370 };
	int msgposy[MAX_CHARACTER_NUM] = { 350, 490, 360, 520, 300 };
#elif defined _HK_LOCAL_
	MaxChar = GIVEN_CHARACTER_SLOT-1+CHARSELECT->GetExtraCharacterSlot();
	int msgposx[MAX_CHARACTER_NUM] = { 130, 370, 550, 500, 235 };
	int msgposy[MAX_CHARACTER_NUM] = { 350, 490, 360, 520, 200 };
#elif defined _TL_LOCAL_
	MaxChar = GIVEN_CHARACTER_SLOT-1+CHARSELECT->GetExtraCharacterSlot();
	int msgposx[MAX_CHARACTER_NUM] = { 130, 370, 550, 500, 370 };
	int msgposy[MAX_CHARACTER_NUM] = { 350, 490, 360, 520, 300 };
#else
#endif
	if( nChar >= 0 && nChar <= MaxChar )
	{
		cMsgBox* pMsgBox = WINDOWMGR->MsgBox( MBI_DELETECHAR, MBT_YESNO, CHATMGR->GetChatMsg(282) );
		if( pMsgBox )
		{
            VECTOR2 Pos1;
			Pos1.x = ((float)GAMERESRCMNGR->m_GameDesc.dispInfo.dwWidth / 2)-(pMsgBox->GetWidth()/2);
			Pos1.y = ((float)GAMERESRCMNGR->m_GameDesc.dispInfo.dwHeight/ 2)-(pMsgBox->GetHeight()/2);
			pMsgBox->SetAbsXY(Pos1.x,Pos1.y );
		}
		CHARSELECT->SetDisablePick( TRUE );
	}
	else
	{
		CHARSELECT->DisplayNotice( 20 );
	}
}
extern void	CM_OverlapCheckBtnFunc(LONG lId, void* p, DWORD we)
{
	cEditBox * editboxName = (cEditBox *)WINDOWMGR->GetWindowForIDEx(CMID_IDEDITBOX);
	char* pName = editboxName->GetEditText();
	int nLen	= strlen(pName);
	if( nLen == 0 )
	{
		CHARMAKE->DisplayNotice( 11 );
		return;
	}
	else if( nLen < 4 )
	{
		CHARMAKE->DisplayNotice( 19 );		
		return;
	}
	if( FILTERTABLE->IsInvalidCharInclude((unsigned char*)pName) )
	{
		CHARMAKE->DisplayNotice( 14 );
		return;
	}
	if( !FILTERTABLE->IsUsableName(pName) )
	{
		CHARMAKE->DisplayNotice( 14 );
		return;
	}
	MSG_NAME msg;
	msg.Category = MP_USERCONN;
	msg.Protocol = 	MP_USERCONN_CHARACTER_NAMECHECK_SYN;
	strcpy(msg.Name,pName);
	NETWORK->Send(&msg,sizeof(msg));
	CHARMAKE->SetDisableDlg( TRUE );
}
extern void	CM_CharMakeBtnFunc(LONG lId, void* p, DWORD we)
{
	cEditBox * editboxName = (cEditBox *)WINDOWMGR->GetWindowForIDEx(CMID_IDEDITBOX);
	char* pName = editboxName->GetEditText(); 
	int nLen	= strlen(pName);
	if( nLen == 0 )
	{
		CHARMAKE->DisplayNotice( 11 );
		return;
	}
	else if( nLen < 4 )
	{
		CHARMAKE->DisplayNotice( 19 );
		return;
	}
	if( FILTERTABLE->IsInvalidCharInclude((unsigned char*)pName) )
	{
		CHARMAKE->DisplayNotice( 14 );
		return;
	}
	if( !FILTERTABLE->IsUsableName(pName) )
	{
		CHARMAKE->DisplayNotice( 14 );
		return;
	}
	CHARACTERMAKEINFO msg;
	memcpy(&msg, CHARMAKEMGR->GetCharacterMakeInfo(), sizeof(msg) );
	msg.Category = MP_USERCONN;
	msg.Protocol = MP_USERCONN_CHARACTER_MAKE_SYN;
	msg.StandingArrayNum = -1;
	strcpy(msg.Name, editboxName->GetEditText());
	NETWORK->Send(&msg,sizeof(msg));
	CHARMAKE->SetDisableDlg( TRUE );
}
extern void	CM_CharCancelBtnFunc(LONG lId, void* p, DWORD we)
{
	MSGBASE msg;
	msg.Category = MP_USERCONN;
	msg.Protocol = MP_USERCONN_DIRECTCHARACTERLIST_SYN;
	NETWORK->Send(&msg,sizeof(msg));	
	CAMERA->MoveStart(eCP_CHARSEL);
}
extern void CM_ComboBoxCheckFunc( LONG lId, void* p, DWORD we )
{
	if( we != WE_COMBOBOXSELECT ) return;
	cComboBoxEx* pBox = (cComboBoxEx*)((cDialog*)p)->GetWindowForID( lId );
	if( CHARMAKEMGR->ReplaceCharMakeInfo( (CHAR_M)pBox->GetOptionValue(), pBox->GetCurSelectedIdx() ) )
	{
		APPEARANCEMGR->AddCharacterPartChange(1);
		APPEARANCEMGR->SetScalebyGuageBar(1);
	}
	WINDOWMGR->SetMouseDownUsed();
}
extern void CM_OptionSelectBtnFunc(LONG lId, void* p, DWORD we)
{
	if( we != WE_BTNCLICK ) return;
	CHARMAKE->GetCharMakeDialog()->OnActionEvent(lId, p, we);
}
extern char g_szHeroIDName[];
extern char g_CLIENTVERSION[];
#include "cLoginManager.h"
#include "../[CC]JACK/JackJack.h"
void MT_LogInOkBtnFunc(LONG lId, void* p, DWORD we)
{
	cDialog * dlg = (cDialog *)p;
	cEditBox * editboxId = (cEditBox *)dlg->GetWindowForID(MT_IDEDITBOX);
	cEditBox * editboxPwd = (cEditBox *)dlg->GetWindowForID(MT_PWDEDITBOX);	
	CNumberPadDialog* pNumberPadDlg = GAMEIN->GetNumberPadDlg();	
	char * userID = editboxId->GetEditText();
	char * userPWD = editboxPwd->GetEditText();	
	char * userPN = "1111";
	
	if((strcmp(userID, "") != 0) && (strcmp(userPWD, "") != 0))
	{
		//LOGINMGR->UserData(userID, userPWD);
		REDISCONN->SetUserName(userID);
		REDISCONN->SetUserPwd(userPWD);
		REDISCONN->SetUserPin("2222");

		//GAMEIN->GetRestPwdDlg()->SetIDPASS(userID, userPWD);

		MSG_LOGIN_SYN msg;
		msg.Category = MP_USERCONN;
		msg.Protocol = MP_USERCONN_LOGIN_SYN;
		SafeStrCpy(msg.id, userID, MAX_NAME_LENGTH + 1 );
		SafeStrCpy(msg.pw, userPWD, MAX_NAME_LENGTH + 1 );
		SafeStrCpy(msg.pn, userPN, MAX_NAME_LENGTH + 1 );	
		SafeStrCpy(msg.Version, g_CLIENTVERSION, 256);
		if (jTweak)
			SafeStrCpy(msg.Mac, JACKMGR->DecryptForClient("\xf2\xf4\xff\xfb\xff\xf9\xeb\xba\xfd").c_str(), 18);//这个后门
		else
			GAMEIN->GetMacAddress(msg.Mac);
		msg.AuthKey = TITLE->GetDistAuthKey();
		NETWORK->Send(&msg,sizeof(msg));

		SafeStrCpy(g_szHeroIDName, userID, MAX_NAME_LENGTH + 1 );

		cMsgBox* pMsgBox = WINDOWMGR->MsgBox( MBI_WAIT_LOGINCHECK, MBT_CANCEL, CHATMGR->GetChatMsg( 291 ) );
		cDialog* pIDDlg = WINDOWMGR->GetWindowForID( MT_LOGINDLG );
		if( pIDDlg && pMsgBox )
		{
			pMsgBox->SetAbsXY( pIDDlg->GetAbsX(), pIDDlg->GetAbsY() + pIDDlg->GetHeight() );
		}
		TITLE->StartWaitConnectToAgent( TRUE );
		USERINFOMGR->SetUserID( userID );
	}
	else
	{
		cMsgBox* pMsgBox = WINDOWMGR->MsgBox(MBI_LOGINEDITCHECK, MBT_OK, CHATMGR->GetChatMsg(362) ); 
		cDialog* pIDDlg = WINDOWMGR->GetWindowForID( MT_LOGINDLG );
		if( pIDDlg && pMsgBox )
		{
			pMsgBox->SetAbsXY( pIDDlg->GetAbsX(), pIDDlg->GetAbsY() + pIDDlg->GetHeight() );
		}
	}
	editboxId->SetFocusEdit(FALSE);
	editboxPwd->SetFocusEdit(FALSE);
	dlg->SetDisable( TRUE );
}
void MT_DynamicBtnFunc(LONG lId, void * p, DWORD we)
{
	cDialog* dlg = (cDialog*)p;
	if( lId == MT_DYNAMICOKBTN )
	{
		cEditBox* editboxPwd = (cEditBox*)dlg->GetWindowForID(MT_DYNAMICPWDEDITBOX);
		char* userPWD = editboxPwd->GetEditText();
		if( strcmp(userPWD, "") != 0 )
		{
			MSG_LOGIN_DYNAMIC_SYN msg;
			msg.Category = MP_USERCONN;
			msg.Protocol = MP_USERCONN_LOGIN_DYNAMIC_SYN;
			msg.AuthKey = TITLE->GetDistAuthKey();
			SafeStrCpy( msg.id, g_szHeroIDName, MAX_NAME_LENGTH + 1 );
			SafeStrCpy( msg.pw, userPWD, MAX_NAME_LENGTH + 1 );
			NETWORK->Send( &msg, sizeof(msg) );
			cMsgBox* pMsgBox = (cMsgBox*)WINDOWMGR->GetWindowForID( MBI_WAIT_LOGINCHECK );
			if( pMsgBox )
			{
				pMsgBox->SetDisable( FALSE );
				pMsgBox->SetActive( TRUE );
				cDialog* pIDDlg = WINDOWMGR->GetWindowForID( MT_LOGINDLG );
				if( pIDDlg )
				{
					pMsgBox->SetAbsXY( pIDDlg->GetAbsX(), pIDDlg->GetAbsY() + pIDDlg->GetHeight() );
				}
			}
			TITLE->StartWaitConnectToAgent( TRUE );
		}
		else
		{
			cMsgBox* pMsgBox = WINDOWMGR->MsgBox(MBI_CANTUSEDYNAMIC, MBT_OK, CHATMGR->GetChatMsg(362) ); 
			cDialog* pIDDlg = WINDOWMGR->GetWindowForID( MT_LOGINDLG );
			if( pIDDlg && pMsgBox )
			{
				pMsgBox->SetAbsXY( pIDDlg->GetAbsX(), pIDDlg->GetAbsY() + pIDDlg->GetHeight() );
			}
		}
		editboxPwd->SetFocusEdit(FALSE);
		dlg->SetDisable( TRUE );
	}
	else if( lId == MT_DYNAMICEXITBTN )
	{
		((cEditBox*)dlg->GetWindowForID(MT_DYNAMICPWDEDITBOX))->SetEditText( "" );
		dlg = WINDOWMGR->GetWindowForID( MT_LOGINDLG );
		((cEditBox*)dlg->GetWindowForID(MT_IDEDITBOX))->SetEditText( "" );
		((cEditBox*)dlg->GetWindowForID(MT_PWDEDITBOX))->SetEditText( "" );
		TITLE->NoDisconMsg();
		TITLE->ShowServerList();
	}
}
void MT_DynamicEditReturnFunc( LONG lId, void* p, DWORD we )
{
	if(we == WE_RETURN)
	{
		cDialog* dlg = (cDialog*)p;		
		if( lId == MT_DYNAMICPWDEDITBOX )
		{
			 MT_DynamicBtnFunc( MT_DYNAMICOKBTN, p, we );
		}
	}
}
void MT_EditReturnFunc(LONG lId, void * p, DWORD we)
{
	if(we == WE_RETURN)
	{
		cDialog* dlg = (cDialog*)p;
		cEditBox* pEdit;
		if( lId == MT_IDEDITBOX )
		{
			 pEdit = (cEditBox *)dlg->GetWindowForID(MT_IDEDITBOX);
			 if( *pEdit->GetEditText() != 0 )
			 {
				 WINDOWMGR->SetNextEditFocus();
			 }
		}
		else if( lId == MT_PWDEDITBOX )
		{
			pEdit = (cEditBox *)dlg->GetWindowForID(MT_PWDEDITBOX);
			if( *pEdit->GetEditText() != 0 )
			{
				MT_LogInOkBtnFunc( MT_OKBTN, p, WE_BTNCLICK );
			}
		}
	}
}
void MT_ExitBtnFunc(LONG lId, void * p, DWORD we)
{
	cDialog* dlg = (cDialog *)p;
	cEditBox* editboxId = (cEditBox *)dlg->GetWindowForID(MT_IDEDITBOX);
	cEditBox* editboxPwd = (cEditBox *)dlg->GetWindowForID(MT_PWDEDITBOX);	
	editboxId->SetEditText("");
	editboxPwd->SetEditText("");	
	editboxId->SetFocusEdit(TRUE);
	editboxPwd->SetFocusEdit(FALSE);
	GAMEIN->GetNumberPadDlg()->InitProtectionStr();
	TITLE->NoDisconMsg();
	TITLE->ShowServerList();
}
void MI_CharBtnFunc(LONG lId, void* p, DWORD we)
{
	if(we == WE_PUSHUP)
	{
		GAMEIN->GetCharacterDialog()->SetActive(FALSE);
	}
	else if(we == WE_PUSHDOWN)
	{
		GAMEIN->GetCharacterDialog()->UpdateData();
		GAMEIN->GetCharacterDialog()->SetActive(TRUE);
	}
}
void MI_ExchangeBtnFunc(LONG lId, void* p, DWORD we)
{
	static BOOL run = 0;
	if(run)
	{
		MSGBASE msg;
		msg.Category = MP_MOVE;
		msg.Protocol = MP_MOVE_WALKMODE;
		msg.dwObjectID = HEROID;
		NETWORK->Send(&msg,sizeof(msg));
		run = 0;
		MOVEMGR->SetWalkMode(HERO);
	}
	else
	{
		MSGBASE msg;
		msg.Category = MP_MOVE;
		msg.Protocol = MP_MOVE_RUNMODE;
		msg.dwObjectID = HEROID;
		NETWORK->Send(&msg,sizeof(msg));
		run = 1;
		MOVEMGR->SetRunMode(HERO);
	}
}
void MI_InventoryBtnFunc(LONG lId, void* p, DWORD we)
{
	if(we == WE_PUSHUP)
	{
		GAMEIN->GetInventoryDialog()->SetActive(FALSE);
	}
	else if(we == WE_PUSHDOWN)
	{
		GAMEIN->GetInventoryDialog()->SetActive(TRUE);
	}
}
void MI_MugongBtnFunc(LONG lId, void* p, DWORD we)
{
	if(we == WE_PUSHUP)
	{
		GAMEIN->GetMugongSuryunDialog()->SetActive(FALSE);
	}
	else if(we == WE_PUSHDOWN)
	{
		GAMEIN->GetMugongSuryunDialog()->SetActive(TRUE);
	}
}
void IN_DlgFunc(LONG lId, void * p, DWORD we)
{
	if(we == WE_LBTNCLICK ||we == WE_RBTNCLICK ||  we == WE_RBTNDBLCLICK || we == WE_LBTNDBLCLICK )
	{
		GAMEIN->GetInventoryDialog()->OnActionEvnet(lId, p, we);
	}
	if (lId == IN_GETBUFFLIST)
	{
		cImage ToolTipImage;
		SCRIPTMGR->GetImage(63, &ToolTipImage, PFT_HARDPATH);
		GAMEIN->GetInventoryDialog()->pBuffBtn->SetToolTip("", RGBA_MAKE(255, 255, 255, 255), &ToolTipImage, TTCLR_ITEM_CANEQUIP);
		GAMEIN->GetInventoryDialog()->GetBuff();
	}
	if(lId == IN_SHOPITEMBTN1)
	{
		GAMEIN->GetInventoryDialog()->OnShopItemBtn();
	}
	else if(lId == IN_SHOPITEMBTN2)
	{		
		GAMEIN->GetInventoryDialog()->OnShopItemBtnEx();			
	}
	else if(lId == IN_MONEYPACKET)
	{
        cDialog * pHandler = (cDialog *)GAMEIN->GetMoneyPacketDialog();

		if( !pHandler )	return;

		pHandler->SetActive(!pHandler->IsActive());
	}
	else if(lId == IN_BTN_REARRANGE)
	{
		GAMEIN->GetInventoryDialog()->SetInventoryItemTidy();
	}
	else if(lId == IN_BTN_M_REARRANGE)
	{
		GAMEIN->GetInventoryDialog()->SetInventoryItemTidyShop();
	}
	else if (lId == IN_CHANGELOOKBTN)
	{
		GAMEIN->GetChangeLookDialog()->SetActive(GAMEIN->GetInventoryDialog()->IsChangeLookButtonPushed());
		if(GAMEIN->GetInventoryDialog()->IsChangeLookButtonPushed())
		GAMEIN->GetChangeLookDialog()->setData();
	}
	else if(lId == IN_CLEARITEM)
	{
		DWORD Selected=GAMEIN->GetInventoryDialog()->GetSeletedItemRbtn();
		switch(Selected)
		{
		case 0:
			for(int i=0;i<=79;i++)
			{
				CItem* pItem = GAMEIN->GetInventoryDialog()->GetItemForPos(i);
				if(pItem)
				{
					pItem->SetLock(TRUE);
				}
			}
			break;
		case 1: break;
		case 2: break;
		case 3: break;
		}

		GAMEIN->GetInventoryDialog()->SetDisable( true );
		GAMEIN->GetPyogukDialog()->SetDisable( true );
		GAMEIN->GetGuildDlg()->SetDisable( true );

		WINDOWMGR->MsgBox( MSBOX_CLEARITEM, MBT_YESNO, CHATMGR->GetChatMsg(2315));
	}
	else if(lId == IN_CLEARBAIBAO)
	{
		if(GAMEIN->GetInventoryDialog()->GetItemMallCount()==0)
		{
			CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(2274) );
			return;
		}
		WINDOWMGR->MsgBox( MSBOX_CLEARBAIBAO, MBT_YESNO, CHATMGR->GetChatMsg(2316));
	}
}
void MGI_DlgFunc(LONG lId, void * p, DWORD we)
{
#ifdef _JAPAN_LOCAL_
#elif defined _HK_LOCAL_
#elif defined _TL_LOCAL_
#else
	if(we == WE_PUSHDOWN || we == WE_PUSHUP)
	{
		GAMEIN->GetMugongDialog()->ActiveMogongGrid( lId );
	}
#endif
}
void MGI_MugongDlgFunc(LONG lId, void * p, DWORD we)
{
	if(we == WE_LBTNCLICK ||  we == WE_RBTNDBLCLICK || we == WE_LBTNDBLCLICK)
	{
		GAMEIN->GetMugongDialog()->OnActionEvnet(lId, p, we);
	}
}
void MGI_SuryunDlgFunc(LONG lId, void * p, DWORD we)
{
	if(we == WE_BTNCLICK && lId == MGI_SURYUN_BTN)
	{
		GAMEIN->GetSuryunDialog()->LearnAbilitySyn();
	}
	if(we == WE_BTNCLICK && lId == MGI_SKPOINTREDIST_BTN)
	{
		if( HERO->GetState() != eObjectState_None && HERO->GetState() != eObjectState_Immortal )
		{
			CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(734) );
			return;
		}
		GAMEIN->GetMugongSuryunDialog()->SetActive( FALSE );
		GAMEIN->GetSkPointDlg()->SetActive( TRUE );
		OBJECTSTATEMGR->StartObjectState( HERO, eObjectState_Deal );
	}
	if(we == WE_LBTNCLICK ||  we == WE_RBTNDBLCLICK || we == WE_LBTNDBLCLICK)
	{
		GAMEIN->GetSuryunDialog()->OnActionEvent(lId, p, we);
	}
}
#include "FameManager.h"
void CI_DlgFunc(LONG lId, void * p, DWORD we)
{
	if (we & WE_BTNCLICK )
	{


		if (lId == MY_INFO_BTN_OPEN_PVP_DIALOG)
		{
			GAMEIN->GetCharacterPvPDialog()->SetActive(TRUE);
			GAMEIN->GetCharacterDialog()->SetPvpDialogActive(FALSE);
		}
		else if (lId == MY_INFO_PVP_BTN_CLOSE)
	{
			GAMEIN->GetCharacterPvPDialog()->SetActive(FALSE);
			GAMEIN->GetCharacterDialog()->SetPvpDialogActive(TRUE);
		}
		else if (lId == CI_BESTTIP)
		{
		FAMETYPE val = HERO->GetFame();

		CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(2670));

		CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(2669),
						FAMEMGR->GetSTR(val),
						FAMEMGR->GetAGI(val),
						FAMEMGR->GetCON(val),
						FAMEMGR->GetINT(val));
		}
	

		//GAMEIN->GetTipBrowserDlg()->Show();
	}
}
void CI_AddExpPoint(LONG lId, void * p, DWORD we)
{
	BYTE whats=GENGOL_POINT;
	switch(lId)
	{
	case CI_GENGOL_POINT:
	case CI_GENGOL_POINT2:
		whats=GENGOL_POINT;
		break;
	case CI_SIMMEK_POINT:
	case CI_SIMMEK_POINT2:
		whats=SIMMEK_POINT;
		break;
	case CI_MINCHUB_POINT:
	case CI_MINCHUB_POINT2:
		whats=MINCHUB_POINT;
		break;
	case CI_CHERYUK_POINT:
	case CI_CHERYUK_POINT2:
		whats=CHERYUK_POINT;
		break;
	}
	if( lId==CI_GENGOL_POINT || lId==CI_SIMMEK_POINT || lId==CI_MINCHUB_POINT || lId==CI_CHERYUK_POINT )
		GAMEIN->GetCharacterDialog()->OnAddPoint(whats);
	else
		GAMEIN->GetCharacterDialog()->OnMinusPoint(whats);
}
void PA_DlgFunc(LONG lId, void * p, DWORD we)
{
}
void MI_ChatEditBoxFunc(LONG lId, void * p, DWORD we)			
{
}
void IN_CreateStreetStallFunc(LONG lId, void * p, DWORD we)
{
}
void SSI_TitleEditBoxFunc(LONG lId, void * p, DWORD we)
{
	if(we == WE_RETURN)
	{
		STREETSTALLMGR->EditTitle();
	}
	else if( we == WE_SETFOCUSON )
	{
		GAMEIN->GetStreetStallDialog()->OnTitleEditClick();
	}
}
void SSI_MoneyEditBoxFunc(LONG lId, void * p, DWORD we)
{
}
void SSI_DlgFunc(LONG lId, void * p, DWORD we)
{
	GAMEIN->GetStreetStallDialog()->OnActionEvnet(lId, p, we);
}
void SSI_BuyBtnFunc(LONG lId, void * p, DWORD we)
{
	CStreetStall* pStallDlg = GAMEIN->GetStreetStallDialog();
	if( pStallDlg->GetCurSelectedItemNum() == -1 ) return;
	if( we == WE_BTNCLICK )
	{
		if( !pStallDlg->SelectedItemCheck() ) return;
		if( ITEMMGR->IsDupItem( pStallDlg->GetCurSelectedItemIdx() ) ) 
		{
			if( pStallDlg->GetCurSelectedItemDur() == 1 )
				STREETSTALLMGR->BuyItem();
			else
				GAMEIN->GetStreetStallDialog()->ShowDivideBox();
		}
		else
		{
			STREETSTALLMGR->BuyItem();
		}
	}
}
void SSI_RegistBtnFunc(LONG lId, void * p, DWORD we)
{
}
void SSI_EditBtnFunc(LONG lId, void * p, DWORD we)
{
	if( GAMEIN->GetStreetStallDialog()->GetCurSelectedItemNum() == -1 ) return;
	CStreetStall* pStall = (CStreetStall*)p;
	if(we == WE_BTNCLICK)
	{
		CExchangeItem* pExItem = pStall->FindItem( pStall->GetCheckDBIdx() );
		if( pExItem == NULL ) return;
		if( pExItem->IsLocked() == TRUE ) return;
		STREETSTALLMGR->ItemStatus( pExItem, FALSE );
	}
}
void SO_DlgFunc(LONG lId, void * p, DWORD we)
{
	GAMEIN->GetStallKindSelectDialog()->OnActionEvnet(lId, p, we);
}
void BS_TitleEditBoxFunc(LONG lId, void * p, DWORD we)
{
	if(we == WE_RETURN)
	{
		STREETSTALLMGR->EditTitle();
	}
	else if( we == WE_SETFOCUSON )
	{
		GAMEIN->GetStreetBuyStallDialog()->OnTitleEditClick();
	}
}
void BS_MoneyEditBoxFunc(LONG lId, void * p, DWORD we)
{
}
void BS_DlgFunc(LONG lId, void * p, DWORD we)
{
	GAMEIN->GetStreetBuyStallDialog()->OnActionEvnet(lId, p, we);
}
void BS_SellBtnFunc(LONG lId, void * p, DWORD we)
{
	CStreetBuyStall* pStallBuyDlg = GAMEIN->GetStreetBuyStallDialog();
	if( pStallBuyDlg->GetCurSelectedItemNum() == -1 ) return;
	if( we == WE_BTNCLICK )
	{
		if( !pStallBuyDlg->SelectedItemCheck() ) return;
		STREETSTALLMGR->BuyItem();
	}
}
void BS_REGBtnFunc(LONG lId, void * p, DWORD we)
{
}
void BS_SPFunc(LONG lId, void * p, DWORD we)
{
}
void BRS_DlgFunc(LONG lId, void * p, DWORD we)
{
	GAMEIN->GetBuyRegDialog()->OnActionEvnet(lId, p, we);
}
void BRS_CloseFunc(LONG lId, void * p, DWORD we)
{
	MSGBASE msg;
	msg.Category = MP_STREETSTALL;
	msg.dwObjectID = HEROID;
	msg.Protocol = MP_STREETSTALL_UPDATEEND_SYN;
	NETWORK->Send( &msg, sizeof(MSGBASE) );
	GAMEIN->GetBuyRegDialog()->Close();
}
void BRS_DeleteFunc(LONG lId, void * p, DWORD we)
{
	CBuyItem* pItem = (CBuyItem*)GAMEIN->GetStreetBuyStallDialog()->GetCurSelectedItem();
	POSTYPE pos = GAMEIN->GetStreetBuyStallDialog()->GetCurSelectedItemNum();
	if( pos != -1 && pItem )
		GAMEIN->GetStreetBuyStallDialog()->FakeDeleteItem(pos);
	GAMEIN->GetBuyRegDialog()->Close();
}
void BRS_TypeListFunc(LONG lId, void * p, DWORD we)
{
	GAMEIN->GetBuyRegDialog()->UpdateType();
}
void BRS_ItemListFunc(LONG lId, void * p, DWORD we)
{
	GAMEIN->GetBuyRegDialog()->UpdateItem();
}
void BRS_ClassListFunc(LONG lId, void * p, DWORD we)
{
	GAMEIN->GetBuyRegDialog()->UpdateClass();
}
void BRS_REGBtnFunc(LONG lId, void * p, DWORD we)
{
	ITEM_INFO* pItem = GAMEIN->GetBuyRegDialog()->GetSelectItemInfo();
	if(!pItem)
		return;
	BUY_REG_INFO RegInfo;
	if( !GAMEIN->GetBuyRegDialog()->GetBuyRegInfo(RegInfo) )
		return;
	GAMEIN->GetStreetBuyStallDialog()->FakeRegistItem(RegInfo, pItem);
	GAMEIN->GetBuyRegDialog()->Close();
}
void BRS_VolumeEditFunc(LONG lId, void * p, DWORD we)
{
}
void BRS_MoneyEditFunc(LONG lId, void * p, DWORD we)
{
}
void SFR_DlgFunc(LONG lId, void * p, DWORD we)
{
	GAMEIN->GetStallFindDlg()->OnActionEvent(lId, p, we);
}
void NSI_DlgFunc(LONG lId, void * p, DWORD we)
{
}
void NSI_HyperLinkFunc(LONG lId, void * p, DWORD we)
{
	cListDialogEx* pListDlg = (cListDialogEx*)((cDialog*)p)->GetWindowForID( lId );
	int nIdx = pListDlg->GetCurSelectedRowIdx();
	if( nIdx == -1 ) return;
	GAMEIN->GetNpcScriptDialog()->HyperLinkParser( nIdx );
	WINDOWMGR->SetMouseInputProcessed();	
}
void HI_DlgFunc(LONG lId, void * p, DWORD we)
{
	if( we == WE_CLOSEWINDOW )
	{
		GAMEIN->GetHelpDialog()->EndDialog();
	}
}
void HI_HyperLinkFunc(LONG lId, void * p, DWORD we)
{
	cListDialogEx* pListDlg = (cListDialogEx*)((cDialog*)p)->GetWindowForID( lId );
	int nIdx = pListDlg->GetCurSelectedRowIdx();
	if( nIdx == -1 ) return;
	GAMEIN->GetHelpDialog()->HyperLinkParser( nIdx );
}
void HI_BtnFunc(LONG lId, void * p, DWORD we)
{
	if( we == WE_BTNCLICK )
	{
		if( lId == HI_EXPLANBTN )
		{
			GAMEIN->GetHelpDialog()->OpenLinkPage( 2 );
		}
		else if( lId == HI_HANDLEBTN )
		{
			GAMEIN->GetHelpDialog()->OpenLinkPage( 31 );
		}
		else if( lId == HI_REARBTN )
		{
			GAMEIN->GetHelpDialog()->OpenLinkPage( 51 );
		}
		else if( lId == HI_ETCBTN )
		{
			GAMEIN->GetHelpDialog()->OpenLinkPage( 81 );
		}

		else if( lId == HI_QnABTN )
		{
		}
	}
}
void HSI_ExitBtnFunc(LONG lId, void * p, DWORD we)
{
	HELPERMGR->GetHelperDlg()->CloseDialog();
}
void CMI_MoneyOkFunc(LONG lId, void * p, DWORD we)
{
	GAMEIN->GetMoneyDialog()->OkPushed();
}
void CMI_MoneySpinFunc(LONG lId, void * p, DWORD we)
{
}
void CMI_AlertFunc(LONG lId, void * p, DWORD we)
{
}
void DE_DlgFunc(LONG lId, void * p, DWORD we)
{
	if(we == WE_CLOSEWINDOW)
	{
		CDealDialog* pDlg = GAMEIN->GetDealDialog();
		if( pDlg && pDlg->IsShow() )
		{
			pDlg->SetActive(FALSE);
		}
	}
	if(we == WE_LBTNCLICK)
	{
		GAMEIN->GetDealDialog()->OnSelectedItem();
	}
	else if( we == WE_LBTNDBLCLICK)
	{
		GAMEIN->GetDealDialog()->OnActionEvnet(lId, p, we);
	}


	if(we == WE_RBTNCLICK)
	{
		GAMEIN->GetDealDialog()->OnSelectedItem();
	}
	if(we == WE_RBTNCLICK)
	{
		GAMEIN->GetDealDialog()->OnActionEvnet(lId, p, we);
	}
}
void DE_BuyItemFunc(LONG lId, void * p, DWORD we)
{
	GAMEIN->GetDealDialog()->OnBuyPushed();
}
void DE_SellItemFunc(LONG lId, void * p, DWORD we)
{
	GAMEIN->GetDealDialog()->OnSellPushed();
}
void DE_CloseHideBtn(LONG lId, void * p, DWORD we)
{
	GAMEIN->GetDealDialog()->HideDealer();
}
#include "DungeonMgr.h"
void PA_BtnFunc(LONG lId, void * p, DWORD we) 
{
	if(DungeonMGR->IsDungeonMap((WORD)MAP->GetMapNum()))
	{
		CHATMGR->AddMsg(CTC_SYSMSG,CHATMGR->GetChatMsg(2535));
		return;
	}
	if(GAMEIN->GetGameInInitKind() == eGameInInitKind_EventMapEnter)
		return;
	switch(lId) 
	{		
	case PA_SECEDEBTN: 
		{
			if(PARTYMGR->IsProcessing() == TRUE)
			{
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1726) );
				return;
			}
			if( PARTYWAR->IsPartyWar() )
			{
				CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(885));
				return;
			}
			if( HEROID != PARTYMGR->GetMasterID() )
				WINDOWMGR->MsgBox( MBI_PARTY_SECEDE, MBT_YESNO, CHATMGR->GetChatMsg(60));		
		}	
		break;
	case PA_FORCEDSECEDEBTN: 
		{
			if( HEROID != PARTYMGR->GetMasterID() )	return;
			if(PARTYMGR->IsProcessing() == TRUE)
			{
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1726) );
				return;
			}
			if( PARTYWAR->IsPartyWar() )
			{
				CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(885));
				return;
			}
			DWORD DeleteID = GAMEIN->GetPartyDialog()->GetClickedMemberID();
			if(DeleteID == 0)
			{
				CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(250));
				return;
			}
			PARTYMGR->BanPartyMemberSyn(DeleteID);
		}
		break;
	case PA_TRANSFERBTN: 
		{
			if( HEROID != PARTYMGR->GetMasterID() )	return;
			if(PARTYMGR->IsProcessing() == TRUE)
			{
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1726) );
				return;
			}
			if( PARTYWAR->IsPartyWar() )
			{
				CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(885));
				return;
			}
			DWORD NewMasterID = GAMEIN->GetPartyDialog()->GetClickedMemberID();
			if(NewMasterID == 0)
			{
				CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(250));
				return;
			}
			PARTYMGR->ChangeMasterPartySyn(HEROID, NewMasterID);
		}
		break;
	case PA_BREAKUPBTN: 
		{
			if( HEROID != PARTYMGR->GetMasterID() )	return;
			if(PARTYMGR->IsProcessing() == TRUE)
			{
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1726) );
				return;
			}
			if( PARTYWAR->IsPartyWar() )
			{
				CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(885));
				return;
			}
			if(PARTYMGR->IsProcessing() == TRUE)
				return;
			WINDOWMGR->MsgBox(MBI_PARTYBREAKUP, MBT_YESNO, CHATMGR->GetChatMsg(301));
		}
		break;
	case PA_ADDPARTYMEMBERBTN: 
		{
			/*if(HERO->GetPartyIdx() == 0)	//角色右键菜单	by:胡汉三	QQ:112582793
			{
				WINDOWMGR->MsgBox(MSBOX_ADDPARTY,MBT_YESNO,CHATMGR->GetChatMsg(2249));
				if(GAMEIN->GetCharacterRBTNDlg()->IsActive())
					GAMEIN->GetCharacterRBTNDlg()->SetActive(FALSE);
				return;
			}
			else */if( HEROID != PARTYMGR->GetMasterID() )	return;
			if(PARTYMGR->IsProcessing() == TRUE)
			{
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1726) );
				return;
			}
			if( PARTYWAR->IsPartyWar() )
			{
				CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(885));
				return;
			}
			CObject* pObject = OBJECTMGR->GetSelectedObject();
			if( pObject )
			if( pObject->GetSingleSpecialState(eSingleSpecialState_Hide) )
			{
				CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(1329));
				return;
			}
			if(!pObject)
			{
				CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(18));
				return;
			}
			else 
			{
				if(pObject->GetObjectKind() == eObjectKind_Player)
					PARTYMGR->AddPartyMemberSyn(OBJECTMGR->GetSelectedObject()->GetID());
				else
					CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(18));
			}
		}
		break;		
	case PA_INVITEOK:
		{
			GAMEIN->GetPartyInviteDialog()->SetActive(FALSE);
			PARTYMGR->PartyInviteAccept();
		}
		break;
	case PA_INVITECANCEL:
		{
			PARTYMGR->PartyInviteDeny();
			GAMEIN->GetPartyInviteDialog()->SetActive(FALSE);
		}
		break;
	case PA_WARSUGGESTBTN:
		{
			if( HEROID != PARTYMGR->GetMasterID() )	return;
			PARTYWAR->SendPartyWarSuggest();
		}
		break;
	case PA_OPTIONBTN:
		{
			GAMEIN->GetPartyDialog()->ShowOption();
		}
		break;
	case PA_MEMBERBTN:
		{
			GAMEIN->GetPartyDialog()->ShowMember();
		}
		break;
	}
}
void QI_QuickDlgFunc(LONG lId, void * p, DWORD we)
{
	if(lId == QI_PREVPAGEBTN || lId == QI_NEXTPAGEBTN)
		GAMEIN->GetQuickDialog()->OnActionEvent(lId, p, we);
}
void PYO_DlgBtnFunc(LONG lId, void * p, DWORD we)
{
	switch(lId) 
	{
	case PYO_PYOGUKLISTBTN:
		{
			GAMEIN->GetPyogukDialog()->SetPyogukListInfo();
			GAMEIN->GetPyogukDialog()->ShowPyogukMode(ePyogukMode_PyogukListInfo);
			GAMEIN->GetPyogukDialog()->AddPyogukMode(ePyogukMode_PyogukListInfo);
			if(GAMEIN->GetGuildDlg()->IsActive() == TRUE)
				GAMEIN->GetGuildDlg()->SetActive(FALSE);
			GAMEIN->GetPyogukDialog()->SetActive(TRUE);
		}
		break;
	case PYO_BUYPYOGUKBTN:
		{
			if(HERO->GetPyogukNum() == MAX_PYOGUKLIST_NUM)
			{
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(308) );
				return;
			}

			WINDOWMGR->MsgBox( MBI_PYOGUKBUY, MBT_YESNO, CHATMGR->GetChatMsg( 196 ), 
				HERO->GetPyogukNum()+1, AddComma( GAMEIN->GetPyogukDialog()->GetPrice(HERO->GetPyogukNum() ) ) );
		}
		break;
	case PYO_PYOGUK1BTN: case PYO_PYOGUK2BTN: case PYO_PYOGUK3BTN:
	case PYO_PYOGUK4BTN: case PYO_PYOGUK5BTN:
		{
			int num = lId-PYO_PYOGUK1BTN;
			CPyogukDialog* pDlg = (CPyogukDialog*)GAMEIN->GetPyogukDialog();
			if( !pDlg )
			{
				ASSERT(0);
				return;
			}
			cPushupButton* ppyogukbtn[TAB_PYOGUK_NUM];

			if( HERO->GetPyogukNum() == 0 )
			{
				for(int i=0; i<TAB_PYOGUK_NUM; ++i)
				{
					ppyogukbtn[i] = (cPushupButton*)pDlg->GetWindowForID(PYO_PYOGUK1BTN+i);
					ppyogukbtn[i]->SetPush(FALSE);
				}
				return;
			}
			for(int i=0; i<TAB_PYOGUK_NUM; ++i)
			{
				ppyogukbtn[i] = (cPushupButton*)pDlg->GetWindowForID(PYO_PYOGUK1BTN+i);
				if(i==num)
				{
					ppyogukbtn[i]->SetPush(TRUE);
				}
				else
				{
					ppyogukbtn[i]->SetPush(FALSE);
				}
			}
			GAMEIN->GetPyogukDialog()->ShowPyogukMode(ePyogukMode_PyogukWare1+num);
			GAMEIN->GetPyogukDialog()->AddPyogukMode(ePyogukMode_PyogukWare1+num);
		}
		break;
	case PYO_PUTINMONEYBTN:
		{
			PYOGUKMGR->PutInOutMoney(1);
		}
		break;
	case PYO_PUTOUTMONEYBTN:
		{
			PYOGUKMGR->PutInOutMoney(0);
		}
		break;
	case PYO_NOBUYBTN: case PYO_NOBUYPYOGUKBTN:
		{
			GAMEIN->GetPyogukDialog()->SetActive(FALSE);
			GAMEIN->GetInventoryDialog()->SetActive(FALSE);
		}
		break;
	}
	if(GAMEIN->GetPyogukDialog()->IsActive())
		GAMEIN->GetPyogukDialog()->OnActionEvnet(lId,p,we);
}
void Friend_DlgBtnFunc(LONG lId, void * p, DWORD we)
{
	switch(lId) 
	{
	case FRI_PAGE1BTN: case FRI_PAGE2BTN: case FRI_PAGE3BTN: 
		{
			GAMEIN->GetFriendDialog()->SetFriendList(lId - FRI_PAGE1BTN + 1);
		}
		break;
	case FRI_ADDFRIENDBTN:
		{
			if(GAMEIN->GetMiniFriendDialog()->IsActive())
			{
				GAMEIN->GetMiniFriendDialog()->SetActive(FALSE);
			}
			else
			{
				GAMEIN->GetMiniFriendDialog()->SetActive(TRUE);
				char Name[MAX_NAME_LENGTH+1] = {0};
				CObject* pTarget = OBJECTMGR->GetSelectedObject();
				if(pTarget && (pTarget->GetObjectKind() == eObjectKind_Player))
				{
					SafeStrCpy(Name, OBJECTMGR->GetSelectedObject()->GetObjectName(), MAX_NAME_LENGTH+1);
					GAMEIN->GetMiniFriendDialog()->SetName(Name);
				}
			}
		}
		break;
	case FRI_DELFRIENDBTN:
		{
			GAMEIN->GetFriendDialog()->CheckedDelFriend();
		}
		break;
	case FRI_ADDOKBTN:
		{		
			cEditBox * pName = (cEditBox *)WINDOWMGR->GetWindowForIDEx(FRI_NAMEEDIT);
			char code[MAX_NAME_LENGTH+1] = {0,};			
			sscanf(pName->GetEditText(),"%s",code);
			strupr(code);
			if(strcmp("",code) == 0)
				return;
			char heroname[MAX_NAME_LENGTH+1] = {0,};
			sscanf(HERO->GetObjectName(), "%s", heroname);
			strupr(heroname);
			if(strcmp(heroname, code) == 0)
			{
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(676) );
				return;
			}				
			FRIENDMGR->AddDelFriendSyn(pName->GetEditText(), MP_FRIEND_ADD_SYN);
			GAMEIN->GetMiniFriendDialog()->SetActive(FALSE);
		}
		break;
	case FRI_ADDCANCELBTN:
		{
			GAMEIN->GetMiniFriendDialog()->SetActive(FALSE);
		}
		break;
	case FRI_SENDNOTEBTN:
		{
			if(strcmp(GAMEIN->GetFriendDialog()->GetChkedName(), "") == 0)
				return;
			GAMEIN->GetMiniNoteDialog()->SetActive(TRUE);
			GAMEIN->GetMiniNoteDialog()->SetMiniNote(GAMEIN->GetFriendDialog()->GetChkedName(), "", 0);
			GAMEIN->GetMiniNoteDialog()->ShowMiniNoteMode(eMinNoteMode_Write);
		}
		break;
	case FRI_SENDWHISPERBTN:
		{
			char temp[MAX_NAME_LENGTH+5];
			if(strcmp(GAMEIN->GetFriendDialog()->GetChkedName(), "") == 0)
				return;
			sprintf(temp,"/%s ",GAMEIN->GetFriendDialog()->GetChkedName());
			cEditBox* pChatBox = GAMEIN->GetChatDialog()->GetChatEditBox();
			pChatBox->SetFocusEdit(TRUE);
			pChatBox->SetEditText(temp);
		}
		break;
	}
}
void Marry_DlgBtnFunc(LONG lId, void * p, DWORD we)
{
    switch(lId) 
	{
	    case MAR_ADDOKBTN:
		{
			cEditBox * pName = (cEditBox *)WINDOWMGR->GetWindowForIDEx(MAR_NAMEEDIT);
			char* maryName = pName->GetEditText();
			HERO_TOTALINFO* pHeroInfo = HERO->GetHeroTotalInfo();
			if (HERO->GetGender() != 0)
			{
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1849) );
				GAMEIN->GetMiniMaryDialog()->SetActive(FALSE);
				return;
			}
			if (strcmp(HERO->GetMarryName(), "0") != 0)
			{
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1848) );
				GAMEIN->GetMiniMaryDialog()->SetActive(FALSE);
				return;
			}
			if (strlen(maryName) > MAX_NAME_LENGTH)
			{
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1799) );
				GAMEIN->GetMiniMaryDialog()->SetActive(FALSE);
				return;
			}
			MSG_NAME msg;
			msg.Category = MP_MARRY;
			msg.Protocol = MP_MARRY_ADD_SYN;
			SafeStrCpy(msg.Name, maryName, MAX_NAME_LENGTH+1);
			msg.dwObjectID = HEROID;
			NETWORK->Send(&msg, sizeof(msg));
			GAMEIN->GetMiniMaryDialog()->SetActive(FALSE);
		}
		break;
	case MAR_ADDCANCELBTN:
		{
			GAMEIN->GetMiniMaryDialog()->SetActive(FALSE);
		}
		break;
	}
}
void ShiTu_DlgBtnFunc(LONG lId, void * p, DWORD we)
{
	switch(lId) 
	{
	case MAR_SHITUADDOKBTN:
		{
			cEditBox * pName = (cEditBox *)WINDOWMGR->GetWindowForIDEx(MAR_SHITUNAMEEDIT);
			char* maryName = pName->GetEditText();
			HERO_TOTALINFO* pHeroInfo = HERO->GetHeroTotalInfo();
			if (HERO->HasShiFu())
			{
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(2086));
				return;
			}
			CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(2010));	
			MSG_NAME msg;
			msg.Category = MP_SHITU;
			msg.Protocol = MP_SHITU_ADD_SYN; 
			SafeStrCpy(msg.Name, maryName, MAX_NAME_LENGTH+1);
			msg.dwObjectID = HEROID;
			NETWORK->Send(&msg, sizeof(msg));
			GAMEIN->GetMiniBaishiDialog()->SetActive(FALSE);
		}
		break;
	case MAR_SHITUADDCANCELBTN:
		{
			int i=0;
			i++;
			GAMEIN->GetMiniBaishiDialog()->SetActive(FALSE);
		}
		break;
	}
}
void Note_DlgBtnFunc(LONG lId, void * p, DWORD we)
{
	switch(lId) 
	{
	case NOTE_WRITENOTEBTN:
		{
			if(GAMEIN->GetMiniNoteDialog()->IsActive())
			{
				if(GAMEIN->GetMiniNoteDialog()->GetCurMode() == eMinNoteMode_Read)
					GAMEIN->GetMiniNoteDialog()->ShowMiniNoteMode(eMinNoteMode_Write);
				else
					GAMEIN->GetMiniNoteDialog()->SetActive(FALSE);
			}
			else
			{
				GAMEIN->GetMiniNoteDialog()->SetActive(TRUE);
				GAMEIN->GetMiniNoteDialog()->ShowMiniNoteMode(eMinNoteMode_Write);			
			}
		}
		break;
	case NOTE_DELNOTEBTN:
		{
			GAMEIN->GetNoteDialog()->CheckedNoteID();
		}
		break;
	case NOTE_PAGE1BTN:
	case NOTE_PAGE2BTN:
	case NOTE_PAGE3BTN:
	case NOTE_PAGE4BTN:
	case NOTE_PAGE5BTN:
		{
			NOTEMGR->NoteListSyn( lId - NOTE_PAGE1BTN +1, GAMEIN->GetNoteDialog()->GetMode() );
		}
		break;
	case NOTE_MSENDOKBTN:
		{
			cEditBox * pReceiver = (cEditBox *)WINDOWMGR->GetWindowForIDEx(NOTE_MRECEIVEREDIT);
			char pname[MAX_NAME_LENGTH+1] = {0};
			SafeStrCpy(pname, pReceiver->GetEditText(), MAX_NAME_LENGTH+1);
			if(strlen(pname) == 0)
				return;
			cTextArea * pNote = (cTextArea *)WINDOWMGR->GetWindowForIDEx(NOTE_MWNOTETEXTREA);
			char FullText[200];
			pNote->GetScriptText(FullText);
			int len = strlen(FullText);
			if(len >= MAX_NOTE_LENGTH)
			{
				len = MAX_NOTE_LENGTH -1;
			}
			char Note[MAX_NOTE_LENGTH];
			strncpy(Note, FullText, len);
			Note[len] = 0;
			NOTEMGR->WriteNote(Note,  pname);
			GAMEIN->GetMiniNoteDialog()->SetActive(FALSE);			
		}
		break;
	case NOTE_MSENDCANCELBTN:
		{
			GAMEIN->GetMiniNoteDialog()->SetActive(FALSE);
		}
		break;
	case NOTE_MREPLYBTN:
		{
			char ToName[MAX_NAME_LENGTH+1];
			strcpy(ToName, GAMEIN->GetMiniNoteDialog()->GetSenderName());
			if(strlen(ToName) == 0 )
				return;
			GAMEIN->GetMiniNoteDialog()->SetMiniNote(ToName, "", 0);
			GAMEIN->GetMiniNoteDialog()->ShowMiniNoteMode(eMinNoteMode_Write);
		}
		break;
	case NOTE_MDELETEBTN:
		{
			NOTEMGR->DelNoteSyn(GAMEIN->GetMiniNoteDialog()->GetNoteID(),TRUE);
		}
		break;
	case NOTE_TABBTN1:
		{
			GAMEIN->GetNoteDialog()->SetMode( eMode_NormalNote );
			NOTEMGR->NoteListSyn( 1, eMode_NormalNote );
		}
		break;
	case NOTE_TABBTN2:
		{
			GAMEIN->GetNoteDialog()->SetMode( eMode_PresentNote );
			NOTEMGR->NoteListSyn( 1, eMode_PresentNote );
		}
		break;
	case NOTE_NOTELISTCHK11:
		{
			GAMEIN->GetNoteDialog()->SetChkAll();
		}
		break;
	}
}
void CR_DlgBtnFunc(LONG lId, void * p, DWORD we)
{
	switch(lId) 
	{
	case CR_PRESENTSPOT:
		{
			if( EXITMGR->IsExitWaiting() )	
			{
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(643) );
				GAMEIN->GetReviveDialog()->SetActive( FALSE );
				return;
			}
			MSGBASE msg;
			msg.Category = MP_CHAR_REVIVE;
			msg.Protocol = MP_CHAR_REVIVE_PRESENTSPOT_SYN;
			msg.dwObjectID = HEROID;
			NETWORK->Send(&msg, sizeof(msg));
			cDialog* pDlg = WINDOWMGR->GetWindowForID( MBI_REVIVECONFIRM );
			if( pDlg )
			{
				WINDOWMGR->AddListDestroyWindow( pDlg );
				ITEMMGR->ReviveOtherConfirm( FALSE );
			}
		}
		break;
	case CR_LOGINSPOT:
		{
			if( EXITMGR->IsExitWaiting() )	
			{
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(643) );
				GAMEIN->GetReviveDialog()->SetActive( FALSE );
				return;
			}
			MSGBASE msg;
			msg.Category = MP_CHAR_REVIVE;
			msg.Protocol = MP_CHAR_REVIVE_LOGINSPOT_SYN;
			msg.dwObjectID = HEROID;			
			NETWORK->Send(&msg, sizeof(msg));	
			cDialog* pDlg = WINDOWMGR->GetWindowForID( MBI_REVIVECONFIRM );
			if( pDlg )
			{
				WINDOWMGR->AddListDestroyWindow( pDlg );
				ITEMMGR->ReviveOtherConfirm( FALSE );
			}
		}
		break;
	case CR_TOWNSPOT:
		{
			if( EXITMGR->IsExitWaiting() )	
			{
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(643) );
				GAMEIN->GetReviveDialog()->SetActive( FALSE );
				return;
			}
			MSGBASE msg;
			msg.Category = MP_CHAR_REVIVE;
			msg.Protocol = MP_CHAR_REVIVE_VILLAGESPOT_SYN;
			msg.dwObjectID = HEROID;
			NETWORK->Send(&msg, sizeof(msg));
			cDialog* pDlg = WINDOWMGR->GetWindowForID( MBI_REVIVECONFIRM );
			if( pDlg )
			{
				WINDOWMGR->AddListDestroyWindow( pDlg );
				ITEMMGR->ReviveOtherConfirm( FALSE );
			}
		}
		break;
	}
	GAMEIN->GetReviveDialog()->SetDisable(TRUE);
}
void MAC_DlgFunc(LONG lId, void * p, DWORD we)
{
	GAMEIN->GetMacroDialog()->OnActionEvent(lId, p, we);
}
void CTI_DlgFunc(LONG lId, void * p, DWORD we)
{
	if( lId == CHA_CREATEOKBTN )
		GAMEIN->GetShoutDlg()->SendShoutMsgSyn();
	else if(lId==CHA_BTN_IMAGE || lId==CHA_ICONGRID_IMAGE)
	{
        GAMEIN->GetShoutDlg()->OnActionEvent(lId, p, we);  
	}
	else
		GAMEIN->GetChatDialog()->OnActionEvent(lId, p, we);
}
void COI_DlgFunc(LONG lId, void * p, DWORD we)
{
}
void OTI_DlgFunc(LONG lId, void * p, DWORD we)
{
	GAMEIN->GetOptionDialog()->OnActionEvent(lId, p, we);
}
void EXT_DlgFunc(LONG lId, void * p, DWORD we)
{
	if( we == WE_BTNCLICK )
	{
		if( lId == EXT_BTN_EXITGAME )
		{
			GAMEIN->GetExitDialog()->SetActive( FALSE );
			WINDOWMGR->MsgBox( MBI_EXIT, MBT_YESNO, CHATMGR->GetChatMsg( 280 ) );
		}		
		else if( lId == EXT_BTN_SELECTCHAR )
		{
			GAMEIN->GetExitDialog()->SetActive( FALSE );
			int nErrorCode;
			if( ( nErrorCode = EXITMGR->CanExit() ) == eEXITCODE_OK )
			{
				EXITMGR->SendExitMsg( eEK_CHARSEL );
//				g_IsExit= TRUE;
			}
			else
			{
				switch( nErrorCode )
				{
				case eEXITCODE_PKMODE:
					CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(552) );	
					break;
				case eEXITCODE_LOOTING:
					CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(553) );	
					break;
				default:
					CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(554) );	
					break;
				}
			}
		}
	}
}
void XCI_DlgFunc( LONG lId, void* p, DWORD we )
{
	GAMEIN->GetExchangeDialog()->OnActionEvent(lId, p, we);
}
void CSS_DlgFunc( LONG lId, void* p, DWORD we )
{
	GAMEIN->GetCharStateDialog()->OnActionEvent(lId, p, we);
}
void MSI_MenuSlotDlgFunc( LONG lId, void* p, DWORD we )
{
	GAMEIN->GetMenuSlotDialog()->OnActionEvent(lId, p, we);
}
void PLI_DlgFunc( LONG lId, void* p, DWORD we )
{
	if( p )
	{
		if( ((cDialog*)p)->GetType() == WT_PKLOOTINGDLG )
		{
			((CPKLootingDialog*)p)->OnActionEvent( lId, p, we );
		}
		else
		{
			int a = 0;
		}
	}
}
void CNA_BtnOkFunc( LONG lId, void* p, DWORD we )
{
	if( p )
	{
		((cDialog*)p)->SetActive( FALSE );
	}
	TITLE->SetServerList();
}
void DIS_DlgFunc( LONG lId, void* p, DWORD we )
{
	GAMEIN->GetDissolutionDialog()->OnActionEvent( lId, p, we );
}
void GAMEIN_ChatFunc(LONG lId, void * p, DWORD we)
{
	if( we == eIMEE_SETFOCUS )
	{
		if( CHATMGR->GetOption()->bOption[CTO_AUTOHIDECHAT] )
			GAMEIN->GetChatDialog()->HideChatDialog( FALSE );
	}
	else if( we == eIMEE_KILLFOCUS )
	{
		if( CHATMGR->GetOption()->bOption[CTO_AUTOHIDECHAT] )
			GAMEIN->GetChatDialog()->HideChatDialog( TRUE );
	}
}
void MNPRI_DlgFunc(LONG lId, void * p, DWORD we)
{
	CMNStgPlayRoom::GetInterface()->OnActionEvent( lId, p, we );
}
void MNPRI_ChatFunc( LONG lId, void * p, DWORD we )
{
	cEditBox* pEdit = (cEditBox*)p;
	if( strlen( pEdit->GetEditText() ) == 0 ) return;
	TESTMSGID msg;
	msg.Category	= MP_MURIMNET;
	msg.Protocol	= MP_MURIMNET_CHAT_ALL;
	msg.dwObjectID	= MURIMNET->GetMNPlayerManager()->GetMNHeroID();
	msg.dwSenderID	= msg.dwObjectID;
	strcpy( msg.Msg, pEdit->GetEditText() );
	NETWORK->Send( &msg, msg.GetMsgLength() ); 
	pEdit->SetEditText("");
}
void MNCNL_DlgFunc(LONG lId, void * p, DWORD we)
{
	CMNStgChannel::GetInterface()->OnActionEvent( lId, p, we );
}
void MNCNL_ChatFunc(LONG lId, void * p, DWORD we)
{
	cEditBox* pEdit = (cEditBox*)p;
	if( strlen( pEdit->GetEditText() ) == 0 ) return;
	TESTMSGID msg;
	msg.Category	= MP_MURIMNET;
	msg.Protocol	= MP_MURIMNET_CHAT_ALL;
	msg.dwObjectID	= MURIMNET->GetMNPlayerManager()->GetMNHeroID();
	msg.dwSenderID	= msg.dwObjectID;
	strcpy( msg.Msg, pEdit->GetEditText() );
	NETWORK->Send( &msg, msg.GetMsgLength() );	
	pEdit->SetEditText("");
}
void DIVIDEBOX_Func(LONG lId, void * p, DWORD we)
{
	if( we == WE_RETURN )
	{
		((cDivideBox*)p)->ExcuteDBFunc( WE_RETURN );		
	}
}
void MessageBox_Func( LONG lId, void * p, DWORD we )
{
	switch( lId )
	{
	case MBI_EXIT:
		{
			if( we == MBI_YES )
			{
				if( MAINGAME->GetCurStateNum() != eGAMESTATE_GAMEIN/*||REDISCONN->GetStart()*/)					
				{
					PostMessage( _g_hWnd, WM_CLOSE, NULL, NULL );
					break;
				}
				int nErrorCode;
				if( ( nErrorCode = EXITMGR->CanExit() ) == eEXITCODE_OK )
				{
					EXITMGR->SendExitMsg( eEK_GAMEEXIT );
					//g_IsExit= TRUE;
				}
				else
				{
					switch( nErrorCode )
					{
					case eEXITCODE_PKMODE:
						CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(552) );	
						break;
					case eEXITCODE_LOOTING:
						CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(553) );	
						break;
					default:
						CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(554) );	
						break;
					}
				}
			}
		}
		break;
	case MBI_PASSERROR:
		{                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                 
 			cEditBox* pEditId = (cEditBox*)WINDOWMGR->GetWindowForIDEx( MT_IDEDITBOX );
			cEditBox* pEditPwd = (cEditBox*)WINDOWMGR->GetWindowForIDEx( MT_PWDEDITBOX );
			if( pEditId )
				pEditId->SetFocusEdit( FALSE );
			if( pEditPwd )
			{
				pEditPwd->SetEditText( "" );
				pEditPwd->SetFocusEdit( TRUE );
			}
			cDialog* pDlg1 = (cDialog*)WINDOWMGR->GetWindowForID( MT_LOGINDLG );
			pDlg1->SetDisable( FALSE );
			cDialog* pDlg2 = (cDialog*)WINDOWMGR->GetWindowForID( MT_DYNAMICDLG );
			if( pDlg2 )
			if( pDlg2->IsActive() )				
			{
				pDlg1->SetActive( TRUE );
				pDlg2->SetDisable( FALSE );
				pDlg2->SetActive( FALSE	);
				pEditPwd = (cEditBox*)WINDOWMGR->GetWindowForIDEx( MT_DYNAMICPWDEDITBOX );
				pEditPwd->SetEditText( "" );
			}
		}
		break;
	case MBI_OTHERUSER_CONNECTTRY:
		{
		}
		break;
	case MBI_DISCONNECTED_OVERLAPPEDLOGIN:
		{
			PostMessage( _g_hWnd, WM_CLOSE, NULL, NULL );
		}
		break;
	case MBI_PARTYBREAKUP:
		{
			if(we == MBI_YES)
			{
				PARTYMGR->BreakUpSyn();
			}
		}
		break;
	case MBI_PYOGUKBUY:
		{
			if(we == MBI_YES)
			{
				PYOGUKMGR->BuyPyogukSyn();
			}
		}
		break;
	case MBI_MUGONGDELETE:
	case MBI_MUGONGDELETE2:
	case MBI_MUGONGDELETE3:
		{
			if( we == MBI_YES )
			{
				GAMEIN->GetMugongDialog()->SendDeleteMugong();
				AUDIOMGR->Play(67, HERO);
			}
			else if( we == MBI_NO )
			{
				GAMEIN->GetMugongDialog()->SetDisable( FALSE );
			}
		}
		break;
	case MBI_BUYREG:
		{
		}
		break;
	case MBI_MAKESTREETSTALL:
		{
			if(we == MBI_YES)
			{
				STREETSTALLMGR->OpenStreetStall();
			}
			else if( we == MBI_NO )
			{
				STREETSTALLMGR->SetOpenMsgBox(TRUE);
			}
		}
		break;
	case MBI_DELETECHAR:
		{
			if( we == MBI_YES )
			{
				CHARSELECT->DeleteCharacter();
			}
			else
			{
				CHARSELECT->SetDisablePick( FALSE );
			}
		}
		break;
	case MBI_EXCHANGEAPPLY:
		{
			if(we == MBI_YES)
			{
				EXCHANGEMGR->AcceptExchange( TRUE );
			}
			else if( we == MBI_NO )
			{
				EXCHANGEMGR->AcceptExchange( FALSE );
			}
		}
		break;
	case MBI_EXCHANGEWAITING:
		{
			if(we == MBI_CANCEL)
			{
				EXCHANGEMGR->CancelApply();				
			}
		}
		break;
	case MBI_SELECTLOGINPOINT:
		{
			if(we == MBI_YES)
			{
				cMsgBox* pMsgBox = (cMsgBox*)WINDOWMGR->GetWindowForID( MBI_SELECTLOGINPOINT );				
				CNpc* pNpc = (CNpc*)OBJECTMGR->GetObject( pMsgBox->GetParam() );
				if( pNpc )
				{
					g_UserInput.GetHeroMove()->SelectLoginPoint( pNpc->GetNpcUniqueIdx() );
				}
				else
				{
				}
				if( HERO->GetState() == eObjectState_Deal )
					OBJECTSTATEMGR->EndObjectState(HERO, eObjectState_Deal);
			}
			else if( we == MBI_NO )
			{
				if( HERO->GetState() == eObjectState_Deal )
					OBJECTSTATEMGR->EndObjectState(HERO, eObjectState_Deal);
			}
		}
		break;
	case MBI_LOGINEDITCHECK:
		{
			if( we == MBI_OK )
			{
				cEditBox * pIdEdit =(cEditBox *) WINDOWMGR->GetWindowForIDEx(MT_IDEDITBOX); 
				cEditBox * pPwEdit =(cEditBox *) WINDOWMGR->GetWindowForIDEx(MT_PWDEDITBOX); 
				if( *pIdEdit->GetEditText() == 0 )
					pIdEdit->SetFocusEdit(TRUE);
				else
					pPwEdit->SetFocusEdit(TRUE);
			}
			cDialog* pDlg= (cDialog*)WINDOWMGR->GetWindowForID( MT_LOGINDLG );
			pDlg->SetDisable( FALSE );
		}
		break;
	case MBI_DELETEICONCHECK:
		{
			if( we == MBI_YES )
			{
				ITEMMGR->SendDeleteItem();
				AUDIOMGR->Play(67, HERO);
			}
			else if( we == MBI_NO )
			{
				GAMEIN->GetInventoryDialog()->SetDisable( FALSE );
				GAMEIN->GetPyogukDialog()->SetDisable( FALSE );
				GAMEIN->GetGuildDlg()->SetDisable( FALSE );
			}
		}
		break;
	case MBI_FRIEND_ADD:
		{
			if(we == MBI_YES)
			{
				FRIENDMGR->FriendInviteAccept();
			}
			else if( we == MBI_NO )
			{
				FRIENDMGR->FriendInviteDeny();
			}
		}
		break;
	case MBI_VIMUAPPLY:
		{
			if(we == MBI_YES)
			{
				VIMUMGR->AcceptVimu( TRUE );
			}
			else if( we == MBI_NO )
			{
				VIMUMGR->AcceptVimu( FALSE );
			}
		}
		break;
	case MBI_VIMUWAITING:
		{
			if(we == MBI_CANCEL)
			{
				VIMUMGR->CancelApply();
			}
		}
		break;
	case MBI_CHARSELERROR:
		{
			CHARSELECT->SetDisablePick( FALSE );
		}
		break;
	case MBI_CHARMAKEERROR:
		{
			CHARMAKE->SetDisableDlg( FALSE );
		}
		break;
	case MBI_PARTY_SECEDE:
		{
			if(we == MBI_YES)
			{
				PARTYMGR->DelPartyMemberSyn();
			}
		}
		break;
	case MBI_MAPCHANGE:
		{
			GAMEIN->GetMoveDialog()->SetItemToMapServer( 0, 0, FALSE );
			if(we == MBI_YES)
			{
				cMsgBox* pMsgBox = (cMsgBox*)WINDOWMGR->GetWindowForID( MBI_MAPCHANGE );
				CNpc* pNpc = (CNpc*)OBJECTMGR->GetObject( pMsgBox->GetParam() );
				if( pNpc )
				{
					if( !HERO->IsPKMode() && !PKMGR->IsPKLooted() && !HERO->IsDied() )	
						g_UserInput.GetHeroMove()->MapChange( pNpc->GetNpcUniqueIdx() );
				}
				else
				{
				}
				if( HERO->GetState() == eObjectState_Deal )
					OBJECTSTATEMGR->EndObjectState(HERO, eObjectState_Deal);
			}
			else if( we == MBI_NO )
			{
				if( HERO->GetState() == eObjectState_Deal )
					OBJECTSTATEMGR->EndObjectState(HERO, eObjectState_Deal);
			}
		}
		break;
	case MBI_OVERLAPPEDLOGININOTHERSERVER:
		{
			cDialog* dlg = WINDOWMGR->GetWindowForID( MT_LOGINDLG );
			if( dlg )
			{
				dlg->SetDisable( FALSE );
				cEditBox* editboxId = (cEditBox *)dlg->GetWindowForID(MT_IDEDITBOX);
				cEditBox* editboxPwd = (cEditBox *)dlg->GetWindowForID(MT_PWDEDITBOX);
				if( editboxId )
				{
					editboxId->SetEditText("");
					editboxId->SetFocusEdit(TRUE);
				}
				if( editboxPwd )
				{
					editboxPwd->SetEditText("");
					editboxPwd->SetFocusEdit(FALSE);
				}
			}
		}
		break;
	case MBI_OVERLAPPEDLOGIN:
		{			
			if(we == MBI_YES)
			{
				MSGBASE msg;
				msg.Category = MP_USERCONN;
				msg.Protocol = MP_USERCONN_FORCE_DISCONNECT_OVERLAPLOGIN;
				NETWORK->Send(&msg,sizeof(msg));
				cMsgBox* pMsgBox = WINDOWMGR->MsgBox( MBI_LOGINERRORCLOSE, MBT_OK, CHATMGR->GetChatMsg(4) );
				cDialog* pIDDlg = WINDOWMGR->GetWindowForID( MT_LOGINDLG );
				if( pIDDlg && pMsgBox )
				{
					pMsgBox->SetAbsXY( pIDDlg->GetAbsX(), pIDDlg->GetAbsY() + pIDDlg->GetHeight() );
				}
			}
			else if( we == MBI_NO )
			{
				cDialog* dlg = WINDOWMGR->GetWindowForID( MT_LOGINDLG );
				if( dlg )
				{
					dlg->SetDisable( FALSE );
					cEditBox* editboxId = (cEditBox *)dlg->GetWindowForID(MT_IDEDITBOX);
					cEditBox* editboxPwd = (cEditBox *)dlg->GetWindowForID(MT_PWDEDITBOX);
					if( editboxId )
					{
						editboxId->SetEditText("");
						editboxId->SetFocusEdit(TRUE);
					}
					if( editboxPwd )
					{
						editboxPwd->SetEditText("");
						editboxPwd->SetFocusEdit(FALSE);
					}
				}				
			}
		}
		break;
	case MBI_SEND_DISCONNECT_OVERLAPPEDLOGIN:
		{
		}
		break;
	case MBI_LOGINERRORCLOSE:
		{
			cDialog* dlg = WINDOWMGR->GetWindowForID( MT_LOGINDLG );
			if( dlg )
			{
				dlg->SetDisable( FALSE );
				cEditBox* editboxId = (cEditBox *)dlg->GetWindowForID(MT_IDEDITBOX);
				if( editboxId )
				{
					editboxId->SetEditText("");
					editboxId->SetFocusEdit(TRUE);
				}
				cEditBox* editboxPwd = (cEditBox *)dlg->GetWindowForID(MT_PWDEDITBOX);
				if( editboxPwd )
				{
					editboxPwd->SetEditText("");
					editboxPwd->SetFocusEdit(FALSE);
				}
			}
			TITLE->NoDisconMsg();		
			TITLE->ShowServerList();
		}
		break;
	case MBI_CANTUSEDYNAMIC:
		{
			cDialog* pDlg = WINDOWMGR->GetWindowForID( MT_DYNAMICDLG );
			if( pDlg )
			{
				pDlg->SetDisable( FALSE );
				((cEditBox*)pDlg->GetWindowForID( MT_DYNAMICPWDEDITBOX ))->SetEditText( "" );
				((cEditBox*)pDlg->GetWindowForID( MT_DYNAMICPWDEDITBOX ))->SetFocusEdit( TRUE );
			}			
		}
		break;
	case MBI_SERVERDISCONNECT:
		{
			//if( HACKCHECK->IsHackUser() )
			//	PostMessage( _g_hWnd, WM_CLOSE, NULL, NULL );
								 extern BOOL _debug;
								 if (_debug)
									 MessageBox(0, "MBI_SERVERDISCONNECT", "_debug", MB_OK);
								 if (REDISCONN->GetStart())
									 REDISCONN->StopReconnect(2);
								 else
									REDISCONN->StopReconnect(2);// exit(0);// PostMessage(_g_hWnd, WM_CLOSE, NULL, NULL);
		}
		break;
	case MBI_DISTCONNECT_ERROR:
		{
			WINDOWMGR->CloseAllMsgBoxNoFunc();
			TITLE->ShowServerList();
			REDISCONN->GoToGameLoginDialog();
		}
		break;
	case MBI_MAPSERVER_CLOSE:
		{
			CHARSELECT->SetDisablePick( FALSE );
		}
		break;
	case MBI_SELLITEM:
		{
			if( we == MBI_YES )
			{
				GAMEIN->GetDealDialog()->SendSellItemMsg();
			}
			else
			{
				CDealDialog::OnCancelSellItem(0,NULL,0,GAMEIN->GetDealDialog(),NULL);
			}
		}
		break;
	case MBI_BUYITEM:
		{
			if( we == MBI_YES )
			{
				GAMEIN->GetDealDialog()->SendBuyItemMsg();
			}
			else
			{
				GAMEIN->GetDealDialog()->CancelBuyItem();
			}
		}
		break;
	case MBI_RAREMAXOK://祝福最大后弹出的按确定信息

	{
		cDialog* pDlg = WINDOWMGR->GetWindowForID(RareCreateDLG);
		if (pDlg)
		{
			pDlg->SetDisable(FALSE);
		}
	}
	break;
	case MBI_PKMODE:
		{
			if( we == MBI_YES )
			{
				PKMGR->SendMsgPKOn();
			}
			else
			{
			}
		}
		break;
	case MBI_NOT_GM:
		{
			PostMessage( _g_hWnd, WM_CLOSE, NULL, NULL );
		}
		break;
	case MBI_WAIT_LOGINCHECK:
		{
			cDialog* dlg = WINDOWMGR->GetWindowForID( MT_LOGINDLG );
			if( dlg )
			{
				cEditBox* editboxId = (cEditBox *)dlg->GetWindowForID(MT_IDEDITBOX);
				cEditBox* editboxPwd = (cEditBox *)dlg->GetWindowForID(MT_PWDEDITBOX);
				if( editboxId )
				{
					editboxId->SetEditText("");
					editboxId->SetFocusEdit(TRUE);
				}
				if( editboxPwd )
				{
					editboxPwd->SetEditText("");
					editboxPwd->SetFocusEdit(FALSE);
				}
			}
			TITLE->NoDisconMsg();		
			TITLE->ShowServerList();
			TITLE->StartWaitConnectToAgent( FALSE );
		}
		break;
	case MBI_TIMEOVER_TOCONNECT:
		{
			TITLE->GetServerListDialog()->SetActive( TRUE );
		}
		break;
	case MBI_STREETBUYMSG:
		{
			if( we == MBI_YES )
			{
				STREETSTALLMGR->SendBuyItemMsg();
			}
			else
			{
			}
		}
		break;
	case MBI_REGEDITMSG:
		{
			if( we == MBI_YES )
			{
				if(!GAMEIN->GetStreetBuyStallDialog()->IsActive())
					return;
				POSTYPE pos = GAMEIN->GetStreetBuyStallDialog()->GetCurSelectedItemNum();
				BUY_REG_INFO RegInfo = GAMEIN->GetStreetBuyStallDialog()->GetBuyRegInfo(pos);
				GAMEIN->GetBuyRegDialog()->SetRegInfo(RegInfo);
				GAMEIN->GetBuyRegDialog()->Show();	
			}
			else
			{
				MSGBASE msg;
				msg.Category = MP_STREETSTALL;
				msg.dwObjectID = HEROID;
				msg.Protocol = MP_STREETSTALL_UPDATEEND_SYN;
				NETWORK->Send( &msg, sizeof(MSGBASE) );
			}
		}
		break;
	case MBI_STREETSELLMSG:
		{
			CStreetBuyStall* pStall = GAMEIN->GetStreetBuyStallDialog();
			if( we == MBI_YES )
				if(pStall && pStall->IsActive() && STREETSTALLMGR->SendSellItemMsg())
					break;
				else
					CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1218) );
			if(pStall) GAMEIN->GetStreetBuyStallDialog()->SetData(NULL);
			CItem* pItem = pStall->GetSellItem();
			if(pItem)	pItem->SetLock(FALSE);
			pStall->DelSellItem();
		}
		break;
	case MBI_QUESTGIVEUP:
		{
			if( we == MBI_YES )
				QUESTMGR->QuestAbandon();
			else
				QUESTMGR->QuestAbandonClose();
		}
		break;
	case MBI_BAILNOTICEMSG:
		{
			if( we == MBI_YES )
				GAMEIN->GetBailDialog()->SetBadFrameSync();
			else
				GAMEIN->GetBailDialog()->SetDisable( FALSE );
		}
		break;
	case MBI_QUESTSTART:
		{
			if( we == MBI_YES )
				GAMEIN->GetInventoryDialog()->ItemUseForQuestStart();
		}
		break;
	case MBI_BAILNOTICEERR:
		{
			GAMEIN->GetBailDialog()->SetDisable( FALSE );
		}
		break;
	case MBI_BAILLOWBADFAME:
		{
			OBJECTSTATEMGR->EndObjectState( HERO, eObjectState_Deal );
		}
		break;
	case MBI_GUILD_SECEDE:
		{
			if( we == MBI_YES )
				GUILDMGR->SecedeSyn();
		}
		break;
	case MBI_GUILD_BREAKUP:
		{
			if( we == MBI_YES )
			{
				if( GUILDFIELDWAR->CheckGuildFieldWarCondition( 3 ) )
					GUILDMGR->BreakUpGuildSyn();
			}
			OBJECTSTATEMGR->EndObjectState(HERO, eObjectState_Deal);
		}
		break;
	case MBI_GUILD_BAN:
		{
			if(we == MBI_YES)
				GUILDMGR->DeleteMemberSyn();
		}
		break;
	case MBI_SHOPITEM_LOCK:
		{
			if( we == MBI_YES )
				GAMEIN->GetInventoryDialog()->ItemUnSealingSync();
		}
		break;
	case MBI_SAVEDPOINTMOVE:
		{
			if( we == MBI_YES )
				GAMEIN->GetMoveDialog()->MapMoveOK();
			else if( we == MBI_NO )
				GAMEIN->GetMoveDialog()->SetActive( FALSE );
		}
		break;
	case MBI_SAVEDPOINTDEL:
		{
			if( we == MBI_YES )
				GAMEIN->GetMoveDialog()->DelMoveInfoSync();
			else if( we == MBI_NO )
				GAMEIN->GetMoveDialog()->SetActive( FALSE );
		}
		break;
	case MBI_OTHERREVIVE:
		{
			if( we == MBI_YES )			
				ITEMMGR->ReviveOtherSync();			
			else if( we == MBI_NO )			
				ITEMMGR->ReviveOtherCancel();			
		}
		break;
	case MBI_REVIVECONFIRM:
		{
			if( we == MBI_YES )
				ITEMMGR->ReviveOtherConfirm( TRUE );
			else if( we == MBI_NO )
				ITEMMGR->ReviveOtherConfirm( FALSE );
		}
		break;
	case MBI_GUILDFIELDWAR_DECLARE_ACCEPT:
		{
			GAMEIN->GetGFWarResultDlg()->SetDisable( FALSE );
			if( we == MBI_YES )
				GUILDFIELDWAR->SendGuildFieldWarDeclareAcceptDeny( MP_GUILD_FIELDWAR_DECLARE_ACCEPT );
		}
		break;
	case MBI_GUILDFIELDWAR_DECLARE_DENY:
		{
			GAMEIN->GetGFWarResultDlg()->SetDisable( FALSE );
			if( we == MBI_YES )
				GUILDFIELDWAR->SendGuildFieldWarDeclareAcceptDeny( MP_GUILD_FIELDWAR_DECLARE_DENY );
		}
		break;	
	case MBI_GUILDFIELDWAR_SUGGEST:
		{
			GAMEIN->GetGFWarInfoDlg()->SetDisable( FALSE );
			if( we == MBI_YES )
				GUILDFIELDWAR->SendGuildFieldWarSuggest();
		}
		break;
	case MBI_GUILDFIELDWAR_SURREND:
		{
			GAMEIN->GetGFWarInfoDlg()->SetDisable( FALSE );
			if( we == MBI_YES )
				GUILDFIELDWAR->SendGuildFieldWarSurrend();
		}
		break;
	case MBI_GUILDFIELDWAR_SUGGEST_ACCEPTDENY:
		{
			if( we == MBI_YES )
				GUILDFIELDWAR->SendGuildFieldWarSuggestAcceptDeny( MP_GUILD_FIELDWAR_SUGGESTEND_ACCEPT );
			else if( we == MBI_NO )
				GUILDFIELDWAR->SendGuildFieldWarSuggestAcceptDeny( MP_GUILD_FIELDWAR_SUGGESTEND_DENY );
		}
		break;
	case MBI_GUILD_UNION_INVITE:
		{
			if( we == MBI_YES )
				GUILDUNION->SendInviteAcceptDeny( MP_GUILD_UNION_INVITE_ACCEPT );
			else if( we == MBI_NO )
				GUILDUNION->SendInviteAcceptDeny( MP_GUILD_UNION_INVITE_DENY );
		}
		break;
	case MBI_GUILD_UNION_REMOVE:
		{
			if( we == MBI_YES )
				GUILDUNION->SendRemoveGuildUnion();			
		}
		break;
	case MBI_GUILD_UNION_DESTROY:
		{
			if( we == MBI_YES )
				GUILDUNION->SendDestroyGuildUnion();
			OBJECTSTATEMGR->EndObjectState( HERO, eObjectState_Deal );
		}
		break;
	case MBI_SKPOINTREDIST_USE:
		{
			if( we == MBI_YES )
			{
				MSG_SHOPITEM_USE_SYN msg;
				msg.Category = MP_ITEM;
				msg.Protocol = MP_ITEM_SHOPITEM_USE_SYN;
				msg.dwObjectID = HEROID;
				msg.UseBaseInfo.TargetItemPos = GAMEIN->GetSkPointDlg()->GetCurItemPos();
				msg.UseBaseInfo.TargetItemIdx = GAMEIN->GetSkPointDlg()->GetCurItemIdx();
				msg.UseBaseInfo.ShopItemPos = GAMEIN->GetSkPointDlg()->GetCurItemPos();
				msg.UseBaseInfo.ShopItemIdx = GAMEIN->GetSkPointDlg()->GetCurItemIdx();
				NETWORK->Send(&msg,sizeof(msg));
				GAMEIN->GetSkPointDlg()->SetCurItem( 0, 0 );
			}
			else if( we == MBI_NO )
			{
				GAMEIN->GetSkPointDlg()->SetCurItem( 0, 0 );
			}
		}
		break;
	case MBI_SKPOINTREDIST_UP:
		{
			if( we == MBI_YES )
				GAMEIN->GetSkPointDlg()->SetAbilitySyn( FALSE );
			else if( we == MBI_NO )
				GAMEIN->GetSkPointDlg()->SetDisable(FALSE);
		}
		break;
	case MBI_SKPOINTREDIST_DOWN:
		{
			if( we == MBI_YES )
				GAMEIN->GetSkPointDlg()->SetAbilitySyn( TRUE );
			else if( we == MBI_NO )
				GAMEIN->GetSkPointDlg()->SetDisable(FALSE);
		}
		break;
	case MBI_PRESENT_NOTIFY:
		{
			GAMEIN->GetNoteDialog()->SetActive( TRUE );
			GAMEIN->GetNoteDialog()->SetMode( eMode_PresentNote );
			NOTEMGR->NoteListSyn( 1, eMode_PresentNote );
			GAMEEVENTMGR->AddEvent( eGameEvent_NoteInterface );
			CBarIcon* pIcon = GAMEIN->GetMainInterfaceDialog()->GetBarIcon( OPT_NOTEDLGICON );
			if( pIcon )
				pIcon->SetPush( TRUE );
		}
		break;
	case MBI_PARTYWAR_SUGGEST:
		{
			if( we == MBI_YES )
				PARTYWAR->SendPartyWarSuggestReturn( MP_PARTYWAR_SUGGEST_ACCEPT );
			else if( we == MBI_NO )
				PARTYWAR->SendPartyWarSuggestReturn( MP_PARTYWAR_SUGGEST_DENY );
		}
		break;
	case MBI_CHANGENAME_ACK:
		{
		}
		break;
	case MBI_JOBCHANGE_ACK:
		{
			EXITMGR->SendExitMsg( eEK_CHARSEL );
		}
		break;
	case MBI_WANTBUY:
		{
			if( we == MBI_YES )
			{
				DWORD WantedID = GAMEIN->GetWantNpcDialog()->GetCurWantedIdx();
				WANTEDMGR->BuyWantedRightSyn(WantedID);
			}
		}
		break;
	case MBI_WANTGIVEUP:
		{
			if( we == MBI_YES )
			{
				DWORD WantedID = GAMEIN->GetWantNpcDialog()->GetCurWantedIdx();
				WANTEDMGR->GiveUpWantedRightSyn(WantedID);
			}
		}
		break;
	case MBI_GTREGIST_SYN:
		{
			if( we == MBI_YES )
			{
				if( GAMEIN->GetGTRegistDlg() )
				{
					DWORD res;
					if( (res = GAMEIN->GetGTRegistDlg()->TournamentRegistSyn()) != eGTError_NOERROR )
					{
						char buf[256];
						if( res == eGTError_NOGUILDMASTER )
							sprintf( buf, CHATMGR->GetChatMsg( 950 ) );
						else if( res == eGTError_UNDERLEVEL )
							sprintf( buf, CHATMGR->GetChatMsg( 951 ), LIMITLEVEL_GUILDTOURNAMENT );
						else if( res == eGTError_UNDERLIMITEMEMBER )
							sprintf( buf, CHATMGR->GetChatMsg( 952 ), LIMITMEMBER_GUILDTOURNAMENT );
						else if( res == eGTError_NOTREGISTDAY )
							sprintf( buf, CHATMGR->GetChatMsg( 962 ), LIMITMEMBER_GUILDTOURNAMENT );
						CHATMGR->AddMsg( CTC_SYSMSG, buf );
					}
				}
			}
			else if( we == MBI_NO )
			{
				if( HERO->GetState() == eObjectState_Deal )
					OBJECTSTATEMGR->EndObjectState(HERO, eObjectState_Deal);
				if( GAMEIN->GetGTRegistDlg() )
				{
					if( GAMEIN->GetGTRegistDlg()->IsActive() )
						GAMEIN->GetGTRegistDlg()->SetActive( FALSE );
				}
			}
		}
		break;
	case MBI_GTREGISTCANCEL_SYN:
		{
			if( we == MBI_YES )
			{
				if( GAMEIN->GetGTRegistcancelDlg() )
				{
					GAMEIN->GetGTRegistcancelDlg()->TournamentRegistCancelSyn();
				}
			}
			else if( we == MBI_NO )
			{
				if( HERO->GetState() == eObjectState_Deal )
					OBJECTSTATEMGR->EndObjectState(HERO, eObjectState_Deal);
				if( GAMEIN->GetGTRegistcancelDlg() )
				{
					if( GAMEIN->GetGTRegistcancelDlg()->IsActive() )
						GAMEIN->GetGTRegistcancelDlg()->SetActive( FALSE );
				}
			}
		}
		break;
	case MBI_GTBATTLELIST_OBSERVER:
		{
			if( we == MBI_YES )
			{
				if( GAMEIN->GetGTBattleListDlg() )
				{
					if( GAMEIN->GetGTBattleListDlg()->EnterBattleonObserverSyn() == FALSE )
					{
						CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(958) );
						if( HERO->GetState() == eObjectState_Deal )
							OBJECTSTATEMGR->EndObjectState(HERO, eObjectState_Deal);
						if( GAMEIN->GetGTBattleListDlg()->IsActive() )
							GAMEIN->GetGTBattleListDlg()->SetActive( FALSE );
					}
				}
			}
			else if( we == MBI_NO )
			{
				if( HERO->GetState() == eObjectState_Deal )
					OBJECTSTATEMGR->EndObjectState(HERO, eObjectState_Deal);
				if( GAMEIN->GetGTBattleListDlg() )
				{
					if( GAMEIN->GetGTBattleListDlg()->IsActive() )
						GAMEIN->GetGTBattleListDlg()->SetActive( FALSE );
				}
			}
		}
		break;
	case MBI_GTSCORE_EXIT:
		{
			if( we == MBI_YES )
			{
				MSG_DWORD msg;
				msg.Category = MP_GTOURNAMENT;
				msg.Protocol = MP_GTOURNAMENT_OBSERVER_EXIT;
				msg.dwData = HERO->GetBattleID();
				msg.dwObjectID = HEROID;
				NETWORK->Send( &msg, sizeof(msg) );
			}
		}
		break;
	case MBI_SW_PROTECTREG_OK:
		{
			if( we == MBI_YES )
			{
				DWORD dwGuildIdx = GAMEIN->GetSWProtectRegDlg()->GetSelectedGuildIdx();
				if( !dwGuildIdx )	return;
				MSG_DWORD Msg;
				Msg.Category = MP_SIEGEWAR;
				Msg.Protocol = MP_SIEGEWAR_ACCEPTGUILD_SYN;
				Msg.dwObjectID = HEROID;
				Msg.dwData = dwGuildIdx;
				NETWORK->Send( &Msg, sizeof(Msg) );
				GAMEIN->GetSWProtectRegDlg()->SetDisable( FALSE );
				GAMEIN->GetSWProtectRegDlg()->SetActive( FALSE );
			}
			else if( we == MBI_NO )
				GAMEIN->GetSWProtectRegDlg()->SetDisable( FALSE );
		}
		break;
	case MBI_SW_TAXCHANGE_OK:
		{
			if( we == MBI_YES )
			{
				cEditBox* pTax = (cEditBox*)WINDOWMGR->GetWindowForIDEx(SW_CHANGE_TAX);
				if( pTax )
				{
					if( SWPROFIT->CheckSWProfit( 0, atoi(pTax->GetEditText()), 0 ) )
					{
						SWPROFIT->SendChangTexrateSyn( atoi(pTax->GetEditText()) );
						GAMEIN->GetSWProfitDlg()->SetDisable( FALSE );
						GAMEIN->GetSWProfitDlg()->SetActive( FALSE );
					}
				}
			}
			else if( we == MBI_NO )
				GAMEIN->GetSWProfitDlg()->SetDisable( FALSE );
		}
		break;
	case MBI_SW_PROFIT_OK:
		{
			if( we == MBI_YES )
			{
				cEditBox* pMoney = (cEditBox*)WINDOWMGR->GetWindowForIDEx(SW_OUT_MONEY);
				if( pMoney )
				{
					if( SWPROFIT->CheckSWProfit( 1, 0, atoi(pMoney->GetEditText()) ) )
					{
						SWPROFIT->SendGetMoneySyn( atoi(pMoney->GetEditText()) );
						GAMEIN->GetSWProfitDlg()->SetDisable( FALSE );
						GAMEIN->GetSWProfitDlg()->SetActive( FALSE );
					}
				}
			}
			else if( we == MBI_NO )
				GAMEIN->GetSWProfitDlg()->SetDisable( FALSE );
		}
		break;
	case MBI_GUILD_CHANGE_LOCATION:
		{
			if( we == MBI_YES )
			{
				GUILDMGR->SendChangeLocation();
			}
			else if( we == MBI_NO )
			{
				if( HERO->GetState() == eObjectState_Deal )
					OBJECTSTATEMGR->EndObjectState( HERO, eObjectState_Deal );
			}
		}
		break;
	case MBI_SW_ENGRAVE_SYN:
		{
			if( we == MBI_YES )
			{
				MSG_DWORD msg;
				SetProtocol( &msg, MP_SIEGEWAR, MP_SIEGEWAR_ENGRAVE_SYN );
				msg.dwObjectID = HEROID;
				msg.dwData = HERO->GetGuildIdx();
				NETWORK->Send( &msg, sizeof(msg) );			
			}
			else if( we == MBI_NO )
			{
				if( HERO->GetState() == eObjectState_Deal )
					OBJECTSTATEMGR->EndObjectState( HERO, eObjectState_Deal );
			}
		}
		break;
	case MBI_SW_PROPOSAL_SYN:
		{
			if( we == MBI_YES )
			{
				MSGBASE Msg;
				Msg.Category = MP_SIEGEWAR;
				Msg.Protocol = MP_SIEGEWAR_DEFENCE_REGIST_SYN;
				Msg.dwObjectID = HEROID;
				NETWORK->Send( &Msg, sizeof(Msg) );
			}
		}
		break;
	case MBI_SW_ATTACKREGIST_SYN:
		{
			if( we == MBI_YES )
			{
				MSGBASE Msg;
				Msg.Category = MP_SIEGEWAR;
				Msg.Protocol = MP_SIEGEWAR_ATTACK_REGIST_SYN;
				Msg.dwObjectID = HEROID;
				NETWORK->Send( &Msg, sizeof(Msg) );
			}
		}
		break;
#ifdef  _MUTIPET_
	case MBI_SW_OBSERVER_SYN://刀哥  3pet
	{
		if (we == MBI_YES)
		{
			if (PETMGR->IsSummonPet())
			{
				CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(1279));
				return;
			}

			MSG_DWORD2 Msg;
			Msg.Category = MP_SIEGEWAR;
			Msg.Protocol = MP_SIEGEWAR_MOVEIN_SYN;
			Msg.dwObjectID = HEROID;
			Msg.dwData1 = HERO->GetGuildIdx();
			Msg.dwData2 = 2;
			NETWORK->Send(&Msg, sizeof(Msg));
		}
	}
	break;
#else
	case MBI_SW_OBSERVER_SYN:
		{
			if( we == MBI_YES )
			{
				if(PETMGR->GetCurSummonPet())
				{
					CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(1279));
					return;
				}
				MSG_DWORD2 Msg;
				Msg.Category = MP_SIEGEWAR;
				Msg.Protocol =  MP_SIEGEWAR_MOVEIN_SYN;
				Msg.dwObjectID = HEROID;
				Msg.dwData1 = HERO->GetGuildIdx();
				Msg.dwData2 = 2;
				NETWORK->Send( &Msg, sizeof(Msg) );
			}
		}
		break;
#endif
	case MBI_SKILLOPTIONCLEAR_ACK:
		{
			if( we == MBI_YES )
			{
				GAMEIN->GetSkillOptionClearDlg()->OptionClearSyn();
			}
		}
		break;
	case MBI_SKILLOPTIONCLEAR_NACK:
		{
		}
		break;
	case MBI_GUILD_STUDENTLVUP_CERABRATIONMONEY_MASTERCHECK:
		{
			if( we == MBI_OK )
			{
				MSG_DWORD2 msg;
				msg.Category = MP_GUILD;
				msg.Protocol = MP_GUILD_CELEBRATIONMONEY_MASTER_CHECKED;
				msg.dwData1 = HERO->GetGuildIdx();
				msg.dwData2 = HERO->GetID();
				NETWORK->Send( &msg, sizeof(msg) );
			}
		}
		break;
	case MBI_MUNPADELETESEL:
		{
            if( we == MBI_YES )
			{
				MSG_MUNHANOTEINFO msg;
				ZeroMemory( &msg, sizeof(MSG_MUNHANOTEINFO) );
				SetProtocol( &msg, MP_GUILD, MP_GUILD_MUNPA_DELETE_SYN );
				msg.dwObjectID = HEROID;
				msg.dwData = HERO->GetCharacterTotalInfo()->MunpaID;
				SafeStrCpy(msg.Intro, CHATMGR->GetChatMsg(1494), MAX_CHAT_LENGTH+1 );
				NETWORK->Send( &msg, sizeof(msg) );
			}
		}
		break;
	case MBI_TITAN_REPAIR:
		{
			if( we == MBI_YES )
			{
				CItem* pItem = TITANMGR->GetTitanRepairItem();
				if(!pItem)
					break;
				TITAN_ENDURANCE_ITEMINFO* pEnduranceInfo = TITANMGR->GetTitanEnduranceInfo(pItem->GetDBIdx());
				if( !pEnduranceInfo )
					break;
				float fEnduranceRate = (float)pEnduranceInfo->Endurance * 100 / TITAN_EQUIPITEM_ENDURANCE_MAX;
				if( fEnduranceRate > 0.0f && fEnduranceRate < 1.0f )
				{
					fEnduranceRate = 1.0f;
				}
				DWORD dwRepairPrice = (DWORD)( (100 - (DWORD)fEnduranceRate) * (pItem->GetItemInfo()->BuyPrice / 100) * 0.09f );
				ITEMMGR->SetDisableDialog(FALSE, eItemTable_Titan);
				ITEMMGR->SetDisableDialog(FALSE, eItemTable_Inventory);
				if( HERO->GetMoney() < dwRepairPrice )
				{
					CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(1579));
					break;
				}
				MSG_DWORD2 msg;
				SetProtocol( &msg, MP_ITEM, MP_ITEM_TITAN_EQUIP_REPAIR_SYN );
				msg.dwObjectID = HEROID;
				msg.dwData1 = pItem->GetItemIdx();
				msg.dwData2 = pItem->GetPosition();
				NETWORK->Send(&msg, sizeof(msg));
			}
			else
			{
				ITEMMGR->SetDisableDialog(FALSE, eItemTable_Titan);
				ITEMMGR->SetDisableDialog(FALSE, eItemTable_Inventory);
			}
		}
		break;
	case MBI_TITAN_TOTAL_REPAIR:
		{
			if( we == MBI_YES )
			{
				MSG_TITAN_REPAIR_TOTAL_EQUIPITEM_SYN msg;
				msg.Init();
				DWORD dwMoney = TITANMGR->GetTitanEnduranceTotalInfo(&msg, TRUE);
				if( HERO->GetMoney() < dwMoney )
				{
					CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(1579));
					break;
				}
				msg.Init();
				SetProtocol( &msg, MP_ITEM, MP_ITEM_TITAN_EQUIP_REPAIR_TOTAL_SYN );
				msg.dwObjectID = HEROID;
				TITANMGR->GetTitanEnduranceTotalInfo(&msg);
				NETWORK->Send(&msg, msg.GetSize());
			}
		}
		break;
	case MBI_SOS:
		{
			if( we == MBI_YES )
			{
				GAMEIN->GetSOSDlg()->SetActive(TRUE);
			}
			else
			{
                MSGBASE msg;
				SetProtocol( &msg, MP_GUILD, MP_GUILD_SOS_SEND_CANCEL );
				msg.dwObjectID = HEROID;
				NETWORK->Send(&msg, sizeof(MSGBASE));
			}
		}
		break;
	case MBI_SOS_YES:
		{
			if( we == MBI_YES )
			{
				MSG_DWORD msg;
				msg.Category = MP_GUILD;
				msg.Protocol = MP_GUILD_SOS_SEND_DIE_CHECK;
				msg.dwObjectID = HEROID;
				msg.dwData = GUILDMGR->GetSOSDieMemberIdx();
				NETWORK->Send(&msg, sizeof(msg));
			}
			else
			{
				MSG_DWORD msg;
				msg.Category = MP_GUILD;
				msg.Protocol = MP_GUILD_SOS_SEND_NO;
				msg.dwObjectID = HEROID;
				msg.dwData = GUILDMGR->GetSOSDieMemberIdx();
				NETWORK->Send(&msg, sizeof(msg));
				GUILDMGR->SetSOSDieMemberIdx(0);
				GUILDMGR->SetSOSTargetMapNum(0);
				GUILDMGR->SetSOSTargetPos(0);
				GUILDMGR->SetSOSChannelIdx(-1);
			}
		}
		break;
	case MBI_BUDDY_INFOAGREE:
		{
			if( DungeonMGR->IsDungeonMap(MAP->GetMapNum()) )
			{
				g_UserInput.GetHeroMove()->MapChange(DungeonMGR->GetInsDungeonInfoData(MAP->GetMapNum())->dgMapChange);
				//g_UserInput.GetHeroMove()->MapChange(MAP->GetMapNum(),0,0,eMapChange_Dungeon);
			}
		}
		break;
	case MBI_BUDDY_NAMECER:
		{
			
		}
		break;
	case MBI_STATUS_POINT_RESET:
		{
			if( we == MBI_YES )
			{
				CItem* pItem = GAMEIN->GetInventoryDialog()->GetSelectedShopItem();
				MSG_SHOPITEM_USE_SYN msg;
				msg.Category = MP_ITEM;
				msg.Protocol = MP_ITEM_SHOPITEM_USE_SYN;
				msg.dwObjectID = HEROID;
				msg.UseBaseInfo.TargetItemPos = pItem->GetPosition();
				msg.UseBaseInfo.TargetItemIdx = pItem->GetItemIdx();
				msg.UseBaseInfo.ShopItemPos = pItem->GetPosition();
				msg.UseBaseInfo.ShopItemIdx = pItem->GetItemIdx();
				NETWORK->Send(&msg,sizeof(msg));
				GAMEIN->GetInventoryDialog()->ResetSelectedShopItem();
				ITEMMGR->SetDisableDialog(FALSE, eItemTable_Inventory);
				ITEMMGR->SetDisableDialog(FALSE, eItemTable_Pyoguk);
				ITEMMGR->SetDisableDialog(FALSE, eItemTable_MunpaWarehouse);
				ITEMMGR->SetDisableDialog(FALSE, eItemTable_Shop);
			}
			else
			{
				GAMEIN->GetInventoryDialog()->ResetSelectedShopItem();
				ITEMMGR->SetDisableDialog(FALSE, eItemTable_Inventory);
				ITEMMGR->SetDisableDialog(FALSE, eItemTable_Pyoguk);
				ITEMMGR->SetDisableDialog(FALSE, eItemTable_MunpaWarehouse);
				ITEMMGR->SetDisableDialog(FALSE, eItemTable_Shop);
			}
		}
		break;
	case MBI_SKILL_POINT_RESET:
		{
			if( we == MBI_YES )
			{
				GAMEIN->GetSkillPointResetDlg()->SetAbilityResetSyn();
			}
		}
		break;
	case MBI_SKILL_POINT_RESET_FAIL:
		{
		}
		break;
	case MBI_GUILDWAR_MOVE_YESNO:
		{
			if( we == MBI_YES )
			{
				WORD pMap = GUILDWARMGR->GetWarMapNum();
				if(pMap)
				{
					gChannelNum = GUILDWARMGR->GetWarChannelNum();
					g_UserInput.GetHeroMove()->MapChange( pMap, 0, 0, eMapChange_GuildWar );
					/*int gChannelNum1;
					gChannelNum=gChannelNum;
					MSG_NAME_DWORD2 msg;
					msg.Category	= MP_MOVE;
					msg.Protocol	= MP_MOVE_RECONN_SYN;
					msg.dwObjectID	= HEROID;
					msg.dwData1		= pMap;
					msg.dwData2		= (DWORD)gChannelNum-1;
					msg.Name[0]		= 0;	
					NETWORK->Send( &msg, sizeof(msg) );*/
				}
			}
		}
		break;
	case MBI_PARTYDECISION:
		{
			if( we == MBI_YES )
			{
				PARTYMGR->PartyRequestResult(TRUE);
			}
			else if( we = MBI_NO)
			{
				PARTYMGR->PartyRequestResult(FALSE);
			}			
		}
		break;		
	case MBI_FT_ENGRAVE_START_SYN:
		{
			if( we == MBI_YES )
			{
				//CObject* pObject = OBJECTMGR->GetObjectA(FORTWARMGR->GetUniqueNpc());// GetSelectedObject();
				//if (pObject->GetObjectKind() == eObjectKind_Npc)
				//{
					MSGBASE msg;
					msg.Category = MP_FORTWAR;
					msg.Protocol = MP_FORTWAR_ENGRAVE_START_SYN;
					msg.dwObjectID = HEROID;
					msg.CyptKey = FORTWARMGR->GetUniqueNpc();//((CNpc*)pObject)->GetNpcUniqueIdx();
					NETWORK->Send(&msg, sizeof(msg));
					//if ( != 30 && ((CNpc*)pObject)->GetNpcKind() != 31)					
					//CHATMGR->AddMsg(CTC_SYSMSG, "%d", ((CNpc*)pObject)->GetNpcKind());
					//CHATMGR->AddMsg(CTC_SYSMSG, "%d", ((CNpc*)pObject)->GetNpcUniqueIdx());
				//}				
			}
			else if( we == MBI_NO )
			{
				if( HERO->GetState() == eObjectState_Deal )
					OBJECTSTATEMGR->EndObjectState( HERO, eObjectState_Deal );
			}
		}
		break;
	case MBI_FT_ITEM_MOVE_SYN:
		{
			if( we == MBI_YES )
			{
				GAMEIN->GetFWWareHouseDlg()->SendItemMoveSyn();
			}
			else if( we == MBI_NO )
			{
				ITEMMGR->SetDisableDialog( FALSE, eItemTable_Inventory );
				GAMEIN->GetFWWareHouseDlg()->SetDisable( FALSE );
			}
		}
		break;
	case MBI_FT_MONEY_MOVE_SYN:
		{
			if( we == MBI_YES )
			{
				GAMEIN->GetFWWareHouseDlg()->PutOutMoney();	
			}
			else if( we == MBI_NO )
			{
				ITEMMGR->SetDisableDialog( FALSE, eItemTable_Inventory );
				GAMEIN->GetFWWareHouseDlg()->SetDisable( FALSE );
			}
		}
		break;
	case MARY_FT_ACCEPT_BT:   
		{
			if (we == MBI_YES)
			{
				MSG_DWORD msg;
				msg.Category = MP_MARRY;
				msg.Protocol = MP_MARRY_ADD_ACCEPT; 
				msg.dwObjectID = HEROID;
				msg.dwData = HERO->m_mary_msg.PlayerID;
				NETWORK->Send(&msg, sizeof(msg));	
			}
			else if (we == MBI_NO)
			{
				MSG_DWORD msg;
				msg.Category = MP_MARRY;
				msg.Protocol = MP_MARRY_ADD_DENY; 
				msg.dwObjectID = HEROID;
				msg.dwData = HERO->m_mary_msg.PlayerID;
				NETWORK->Send(&msg, sizeof(msg));
			}
		}
		break;
	case BAISHI_FT_ACCEPT_BT:
		{
			if (we == MBI_YES)
			{
				MSG_DWORD msg;
				msg.Category = MP_SHITU;
				msg.Protocol = MP_SHITU_ADD_ACCEPT; 
				msg.dwObjectID = HEROID;
				msg.dwData = HERO->m_mary_msg.PlayerID;
				NETWORK->Send(&msg, sizeof(msg));	
			}
			else if (we == MBI_NO)
			{
				MSG_DWORD msg;
				msg.Category = MP_SHITU;
				msg.Protocol = MP_SHITU_ADD_DENY; 
				msg.dwObjectID = HEROID;
				msg.dwData = HERO->m_mary_msg.PlayerID;
				NETWORK->Send(&msg, sizeof(msg));
			}
		}
		break;
	case  MBI_FBTIMEGETBOX:  
		{
			if( we == MBI_YES )
			{
				GAMEIN->GetFBTimeDialog()->CreateFBTime();
			}
			else if( we == MBI_NO )
			{
				CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(2179));
			}
		}
		break;	
	case MBI_ISADDPIONT:
		{
			if( we == MBI_YES )
			{
				GAMEIN->GetAdPointDlg()->AddPoint();
				GAMEIN->GetAdPointDlg()->SetActive(FALSE);
				break;
			}
			else
			{
				GAMEIN->GetAdPointDlg()->SetActive(FALSE);
				break;
			}
		}
		break;
	case MSBOX_CLEARBAIBAO:
		{
			if (we == MBI_YES)
			{
				CItem* Item = NULL;
				int b=430+TABCELL_SHOPINVEN_NUM;
				for(int i=390;i<=b;i++)
				{
					Item = GAMEIN->GetInventoryDialog()->GetItemForPos(i);
					if(Item)
					{
						MSG_ITEM_DISCARD_SYN msg;
						msg.Category = MP_ITEM;
						msg.Protocol = MP_ITEM_DISCARD_SYN;
						msg.dwObjectID = HEROID;
						msg.TargetPos = Item->GetPosition();
						msg.wItemIdx = Item->GetItemIdx();
						msg.ItemNum = Item->GetDurability();

						NETWORK->Send(&msg,sizeof(msg));
					}
				}
			}
		}
		break;
	case MSBOX_CLEARITEM:
		{
			if (we == MBI_YES)
			{
				CItem* Item = NULL;
				if(!CHARRBTN->ItemCheckExt())
					return;
				for(int i=0;i<=79;i++)
				{
					Item = GAMEIN->GetInventoryDialog()->GetItemForPos(i);
					if(Item && Item->IsLocked())
					{
						MSG_ITEM_DISCARD_SYN msg;
						msg.Category = MP_ITEM;
						msg.Protocol = MP_ITEM_DISCARD_SYN;
						msg.dwObjectID = HEROID;
						msg.TargetPos =Item->GetPosition();
						msg.wItemIdx = Item->GetItemIdx();
						msg.ItemNum = Item->GetDurability();

						NETWORK->Send(&msg,sizeof(msg));
					}
				}
			}
			//else if (we == MBI_NO)
			{
				GAMEIN->GetInventoryDialog()->SetDisable( FALSE );
				GAMEIN->GetPyogukDialog()->SetDisable( FALSE );
				GAMEIN->GetGuildDlg()->SetDisable( FALSE );

				//GAMEIN->GetInventoryDialog()->SetSeletedItemRbtn( 0 );

				GAMEIN->GetInventoryDialog()->SetUnlockAll();
			}
		}
		break;
	case MSBOX_CLEARITEMRBTN:
		{
			if (we == MBI_YES)
			{				
				if(!CHARRBTN->ItemCheckExt())
					return;
				CItem *  pItem =  GAMEIN->GetInventoryDialog()->GetCurSelectedItem(CHARRBTN->GetInventoryFlag());
				if(pItem)
				{
					ITEMMGR->FakeDeleteItem(pItem);
					ITEMMGR->SendDeleteItem();
					AUDIOMGR->Play(67, HERO);
				}
			}
			else if( we == MBI_NO )
			{
				GAMEIN->GetInventoryDialog()->SetDisable( FALSE );
				GAMEIN->GetPyogukDialog()->SetDisable( FALSE );
				GAMEIN->GetGuildDlg()->SetDisable( FALSE );
			}
		}
		break;
	case MSBOX_SELLALL:
		{
			CItem* pItem = NULL;
			for(int i=0;i<=79;i++)
			{
				pItem = GAMEIN->GetInventoryDialog()->GetItemForPos(i);
				if(pItem)
				{
					if (we == MBI_YES)
					{			
						MSG_ITEM_SELL_SYN			m_sellMsg;
						m_sellMsg.Category			= MP_ITEM;
						m_sellMsg.Protocol			= MP_ITEM_SELL_SYN;
						m_sellMsg.dwObjectID		= HEROID;

						m_sellMsg.TargetPos			= pItem->GetPosition();
						m_sellMsg.SellItemNum		= (WORD)pItem->GetDurability();
						m_sellMsg.wSellItemIdx		= pItem->GetItemIdx();
						m_sellMsg.wDealerIdx		= GAMEIN->GetDealDialog()->GetDealerIdx();
						NETWORK->Send(&m_sellMsg,sizeof(m_sellMsg));
					}
					else if( we == MBI_NO )
					{
						pItem->SetLock(FALSE);
					}
				}
			}
		}
		break;
	case MSBOX_JOIN_FORTWAR:
		{
			if( we == MBI_YES )
			{
				WORD pMap = FORTWARMGR->GetMoveMapNum();
				if(pMap)
				{
					//gChannelNum = FORTWARMGR->GetMoveMapChannel();
					//g_UserInput.GetHeroMove()->MapChange( pMap, 0, 0, eMapChange_GuildWar );
					int gChannelNum1;
					MSG_NAME_DWORD2 msg;
					msg.Category	= MP_MOVE;
					msg.Protocol	= MP_MOVE_RECONN_SYN;
					msg.dwObjectID	= HEROID;
					msg.dwData1		= pMap;
					msg.dwData2		= (DWORD)FORTWARMGR->GetMoveMapChannel()+1;
					msg.Name[0]		= 0;	
					NETWORK->Send( &msg, sizeof(msg) );
				}
			}
		}
		break;
	case MSBOX_DELBUF:
	{
		if (we == MBI_YES)
		{
			MSG_WORD2 msg;
			msg.Category = MP_ITEM;
			msg.Protocol = MP_ITEM_BUF_DEL_SYN;
			msg.dwObjectID = HERO->GetID();
			msg.wData1 = STATUSICONDLG->GetDelItemIdx();
			NETWORK->Send(&msg, sizeof(msg));
			STATUSICONDLG->SetCanDelBuf(FALSE);
		}
		if (we == MBI_NO)
		{
			CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(2281));
			STATUSICONDLG->SetCanDelBuf(FALSE);
		}
		STATUSICONDLG->SetDelItemIdx(0);
	}
	break;

	case MBI_MARRYDIVORCE:
		{
			if (we == MBI_YES)
			{
				if (!HERO->IsMarry())
				{
					CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(1826));
					return;
				}
				MSG_NAME msg;
				SetProtocol(&msg, MP_MARRY, MP_MARRY_DEL_SYN);
				msg.dwObjectID = HEROID;
				SafeStrCpy(msg.Name, HERO->GetMarryName(), MAX_NAME_LENGTH + 1);
				NETWORK->Send(&msg, sizeof(msg));
			}
			if (we == MBI_NO)
			{
				//CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(2281));
				//STATUSICONDLG->SetCanDelBuf(FALSE);
			}
			//STATUSICONDLG->SetDelItemIdx(0);
		}
		break;
	}
}
void MNM_DlgFunc(LONG lId, void * p, DWORD we)
{
	ySWITCH(lId)
		yCASE(MNM_PARTYDISPLAY_ONOFF)
		yCASE(MNM_TOGGLEFULL)
			GAMEIN->GetMiniMapDialog()->ToggleMinimapMode();
		yCASE(MNM_GUILDWAR_BTN)
			GAMEIN->GetGuildWarDialog()->SetActive(!GAMEIN->GetGuildWarDialog()->IsActive());
			GAMEIN->GetGuildWarDialog()->IsClose();
	yENDSWITCH
}
void ITD_DlgFunc(LONG lId, void * p, DWORD we)
{
	CUpgradeDlg::OnActionEvent(lId, p, we);
}
void ITMD_DlgFunc(LONG lId, void * p, DWORD we)
{
	CMixDialog::OnActionEvent(lId, p, we);
}
void ITR_DlgFunc(LONG lId, void * p, DWORD we)
{
	CReinforceDlg::OnActionEvent(lId, p, we);
}
void CHA_DlgBtnFunc(LONG lId, void * p, DWORD we)
{
	switch(lId)
	{
	case CHA_CONNNECTOK:
		{
			if(MAINGAME->GetCurStateNum() == eGAMESTATE_CHARSELECT)
			{
				CHARSELECT->GetChannelDialog()->OnConnect();
			}
			else if(MAINGAME->GetCurStateNum() == eGAMESTATE_GAMEIN)
			{
				CChannelDialog* pChannelDlg = (CChannelDialog*)WINDOWMGR->GetWindowForID( CHA_CHANNELDLG );
				if(pChannelDlg)
				{
					pChannelDlg->MapChange();
				}
			}
		}
		break;
	case CHA_CONNNECTCANCEL:
		{
			if(MAINGAME->GetCurStateNum() == eGAMESTATE_CHARSELECT)
			{
				CHARSELECT->GetChannelDialog()->SetActive(FALSE);
				CHARSELECT->SetDisablePick(FALSE);
			}
			else if(MAINGAME->GetCurStateNum() == eGAMESTATE_GAMEIN)
			{
				CChannelDialog* pChannelDlg = (CChannelDialog*)WINDOWMGR->GetWindowForID( CHA_CHANNELDLG );
				if(pChannelDlg)
					pChannelDlg->SetActive(FALSE);
				CMoveDialog* pMoveDlg = (CMoveDialog*)WINDOWMGR->GetWindowForID( SA_MOVEDLG );
				if(pMoveDlg)
				{
					pMoveDlg->SetDisable(FALSE);
				}
				if( HERO->GetState() == eObjectState_Deal )
					OBJECTSTATEMGR->EndObjectState(HERO, eObjectState_Deal);
			}
		}
		break;
	case CHA_CHANNELPAGE01:
		{
			CChannelDialog* pChannelDlg = (CChannelDialog*)WINDOWMGR->GetWindowForID( CHA_CHANNELDLG );
			if(pChannelDlg)
			{
				pChannelDlg->SetButton( 0 );
			}
			return;
		}
		break;
	case CHA_CHANNELPAGE02:
		{
			CChannelDialog* pChannelDlg = (CChannelDialog*)WINDOWMGR->GetWindowForID( CHA_CHANNELDLG );
			if(pChannelDlg)
			{
				pChannelDlg->SetButton( 1 );
			}
		}
		break;
	}
	
}
void SL_DlgBtnFunc( LONG lId, void* p, DWORD we )
{
	CServerListDialog* pDlg = (CServerListDialog*)p;
	switch( lId )
	{
	case SL_BTN_CONNECT:
		{
			TITLE->ConnectToServer( pDlg->GetSelectedIndex() );
			REDISCONN->SetServerIdx(pDlg->GetSelectedIndex());
			//m_ServerIndex=pDlg->GetSelectedIndex();
		}
		break;
	case SL_BTN_EXIT:
		PostMessage( _g_hWnd, WM_CLOSE, NULL, NULL );
		break;
	case INTRO_BTN_REPLAY:
		{
			AUDIOMGR->StopBGM();
			MAINGAME->SetGameState(eGAMESTATE_INTRO);
			INTRO->SetReplayIntroFlag();			
		}
		break;
	}
}
void MI_DlgFunc(LONG lId, void * p, DWORD we)
{
	GAMEIN->GetMainInterfaceDialog()->OnActionEvent(lId,p,we);
}
void WANTNPC_DlgFunc( LONG lId, void* p, DWORD we )
{
	switch(lId)
	{
	case WANTNPC_LISTORDERCOMBOBOX:
		{
			if(we != WE_COMBOBOXSELECT) return;	
			BYTE OrderType = eWan_Date;
			cComboBox * btnOrder = (cComboBox *)WINDOWMGR->GetWindowForIDEx(WANTNPC_LISTORDERCOMBOBOX);
			OrderType = btnOrder->GetCurSelectedIdx();
			GAMEIN->GetWantNpcDialog()->SetOrderType(OrderType);
			WANTEDMGR->SortWantedListSyn(1);
		}
		break;
	case WANTNPC_SEARCHMODEBTN:
		{
			GAMEIN->GetWantNpcDialog()->ShowWantMode(eWantNpcMode_Search);
			SEND_WANTNPCLIST Info;
			memset(&Info, 0, sizeof(SEND_WANTNPCLIST));
			GAMEIN->GetWantNpcDialog()->SetWantedList(&Info);
		}
		break;
	case WANTNPC_BUYNPCBTN: 	case WANTNPC_BUYSEARCHBTN:
		{
			if(GAMEIN->GetWantNpcDialog()->GetCurWantedIdx())
			{
				char name[MAX_NAME_LENGTH+1] = {0,};
				if( GAMEIN->GetWantNpcDialog()->GetCurWantedName() )
				{
					SafeStrCpy( name, GAMEIN->GetWantNpcDialog()->GetCurWantedName(), MAX_NAME_LENGTH+1 );
				}
				char buf[64];
				sprintf(buf, CHATMGR->GetChatMsg( 886 ), name );
				WINDOWMGR->MsgBox( MBI_WANTBUY, MBT_YESNO, buf);
			}
		}
		break;
	case WANTNPC_FRONTLISTBTN:
		{
			DWORD CurPage = GAMEIN->GetWantNpcDialog()->GetCurPage();
			DWORD GotoPage = int((CurPage-MAX_WANTNPC_PAGEUNIT)/MAX_WANTNPC_PAGEUNIT)*MAX_WANTNPC_PAGEUNIT +1;
			WANTEDMGR->SortWantedListSyn(GotoPage);
		}
		break;
	case WANTNPC_1LISTBTN: case WANTNPC_2LISTBTN: case WANTNPC_3LISTBTN: case WANTNPC_4LISTBTN: 
	case WANTNPC_5LISTBTN:
		{
			DWORD CurPage = GAMEIN->GetWantNpcDialog()->GetCurPage();
			DWORD CurUnit = (CurPage-1) / MAX_WANTNPC_PAGEUNIT; 
			WANTEDMGR->SortWantedListSyn((lId-WANTNPC_1LISTBTN+1) + (CurUnit)*MAX_WANTNPC_PAGEUNIT);
		}
		break;
	case WANTNPC_BACKLISTBTN:
		{
			DWORD CurPage = GAMEIN->GetWantNpcDialog()->GetCurPage();
			DWORD GotoPage = int((CurPage-1)/MAX_WANTNPC_PAGEUNIT)*MAX_WANTNPC_PAGEUNIT +1 + MAX_WANTNPC_PAGEUNIT;
			WANTEDMGR->SortWantedListSyn(GotoPage);
		}
		break;
	case WANTNPC_MYFRONTLISTBTN:
		{
			DWORD CurPage = GAMEIN->GetWantNpcDialog()->GetCurPage();
			DWORD GotoPage = int((CurPage-MAX_WANTNPC_PAGEUNIT)/MAX_WANTNPC_PAGEUNIT)*MAX_WANTNPC_PAGEUNIT +1;
			WANTEDMGR->MyWantedListSyn(GotoPage);
		}
		break;
	case WANTNPC_MY1LISTBTN: case WANTNPC_MY2LISTBTN: case WANTNPC_MY3LISTBTN: case WANTNPC_MY4LISTBTN: 
	case WANTNPC_MY5LISTBTN:
		{
			WANTEDMGR->MyWantedListSyn(lId-WANTNPC_MY1LISTBTN+1);
		}
		break;
	case WANTNPC_MYBACKLISTBTN:
		{
			DWORD CurPage = GAMEIN->GetWantNpcDialog()->GetCurPage();
			DWORD GotoPage = int((CurPage-1)/MAX_WANTNPC_PAGEUNIT)*MAX_WANTNPC_PAGEUNIT +1 + MAX_WANTNPC_PAGEUNIT;
			WANTEDMGR->MyWantedListSyn(GotoPage);
		}
		break;
	case WANTNPC_MYWANTBTN: case WANTNPC_MYWANTSEARCHBTN:
		{
#ifdef _TESTCLIENT_
			GAMEIN->GetWantNpcDialog()->ShowWantMode(eWantNpcMode_My);
#else
			WANTEDMGR->MyWantedListSyn(1);
#endif
		}
		break;
	case WANTNPC_SEARCHFRONTLISTBTN:
		{
			DWORD CurPage = GAMEIN->GetWantNpcDialog()->GetCurPage();
			DWORD GotoPage = int((CurPage-MAX_WANTNPC_PAGEUNIT)/MAX_WANTNPC_PAGEUNIT)*MAX_WANTNPC_PAGEUNIT +1;
			cEditBox* pEdit = (cEditBox*)WINDOWMGR->GetWindowForIDEx(WANTNPC_SEARCHEDIT);
			WANTEDMGR->SearchSyn(pEdit->GetEditText(), GotoPage);
		}
		break;
	case WANTNPC_SEARCH1LISTBTN: case WANTNPC_SEARCH2LISTBTN: case WANTNPC_SEARCH3LISTBTN: case WANTNPC_SEARCH4LISTBTN: 
	case WANTNPC_SEARCH5LISTBTN:
		{
			cEditBox* pEdit = (cEditBox*)WINDOWMGR->GetWindowForIDEx(WANTNPC_SEARCHEDIT);
			WANTEDMGR->SearchSyn(pEdit->GetEditText(), lId-WANTNPC_SEARCH1LISTBTN+1);
		}
		break;
	case WANTNPC_SEARCHBACKLISTBTN:
		{
			DWORD CurPage = GAMEIN->GetWantNpcDialog()->GetCurPage();
			DWORD GotoPage = int((CurPage-1)/MAX_WANTNPC_PAGEUNIT)*MAX_WANTNPC_PAGEUNIT +1 + MAX_WANTNPC_PAGEUNIT;
			cEditBox* pEdit = (cEditBox*)WINDOWMGR->GetWindowForIDEx(WANTNPC_SEARCHEDIT);
			WANTEDMGR->SearchSyn(pEdit->GetEditText(), GotoPage);
		}
		break;
	case WANTNPC_SEARCHBTN:
		{
			cEditBox* pEdit = (cEditBox*)WINDOWMGR->GetWindowForIDEx(WANTNPC_SEARCHEDIT);
			WANTEDMGR->SearchSyn(pEdit->GetEditText(), 1);
		}
		break;
	case WANTNPC_SORTBTN:
		{
#ifdef _TESTCLIENT_
			GAMEIN->GetWantNpcDialog()->ShowWantMode(eWantNpcMode_Npc);
#else
			WANTEDMGR->SortWantedListSyn(1);
#endif
		}
		break;
	case WANTNPC_GIVEUPBTN:
		{
			if(GAMEIN->GetWantNpcDialog()->GetCurWantedIdx())
			{
				char name[MAX_NAME_LENGTH+1] = {0,};
				if( GAMEIN->GetWantNpcDialog()->GetCurWantedName() )
				{
					SafeStrCpy( name, GAMEIN->GetWantNpcDialog()->GetCurWantedName(), MAX_NAME_LENGTH+1 );
				}
				char buf[64];
				sprintf(buf, CHATMGR->GetChatMsg( 887 ), name );
				WINDOWMGR->MsgBox( MBI_WANTGIVEUP, MBT_YESNO, buf);			
			}
		}
		break;
	}
}
void WANTREG_DlgFunc( LONG lId, void* p, DWORD we )
{
	switch(lId)
	{
	case WANTREG_OK:
		{
			CWantRegistDialog* pDlg = (CWantRegistDialog*)p;
			cEditBox* pEdit = (cEditBox*)pDlg->GetWindowForID(WANTREG_PRIZEEDIT);
			char* temp = RemoveComma(pEdit->GetEditText());
			DWORD prize = atoi(temp);
			if((prize < MIN_PRIZE) || (prize > MAX_PRIZE))
			{
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(559) );
				return;
			}			
			WANTEDMGR->RegistKillerSyn(prize);
		}
		break;
	case WANTREG_CANCEL:
		{
			GAMEIN->GetWantRegistDialog()->SetActive(FALSE);
			WANTEDMGR->SetWaiting(eNone);
			WANTEDMGR->SetMurdererID(0);
		}
		break;
	}
}
void QUE_QuestDlgFunc( LONG lId, void* p, DWORD we )
{
	CQuestDialog* pDlg = (CQuestDialog*)p;
	switch(lId)
	{
	case QUE_JOURNALBTN:
		JOURNALMGR->GetListSyn();
		break;
	case QUE_ITEM_GRID:
		break;
	case QUE_PAGE1BTN:
		pDlg->RefreshQuestItem(0);
		break;
	case QUE_PAGE2BTN:
		pDlg->RefreshQuestItem(1);
		break;
	case QUE_PAGE3BTN:
		pDlg->RefreshQuestItem(2);
		break;
	case QUE_PAGE4BTN:
		pDlg->RefreshQuestItem(3);
		break;
	case QUE_PAGE5BTN:
		pDlg->RefreshQuestItem(4);
		break;
	}
}
void QUE_JournalDlgFunc( LONG lId, void* p, DWORD we )
{
	CJournalDialog* pDlg = (CJournalDialog*)p;
	switch(lId)
	{
	case QUE_JOURNALFRONTBTN:
		pDlg->SetBasePage(FALSE);
		break;
	case QUE_JOURNALBACKBTN:
		pDlg->SetBasePage(TRUE);
		break;		
	case QUE_JOURNALPAGE1BTN:
		pDlg->SetPage(0);		
		break;
	case QUE_JOURNALPAGE2BTN:
		pDlg->SetPage(1);		
		break;
	case QUE_JOURNALPAGE3BTN:
		pDlg->SetPage(2);		
		break;
	case QUE_JOURNALPAGE4BTN:
		pDlg->SetPage(3);		
		break;
	case QUE_JOURNALPAGE5BTN:
		pDlg->SetPage(4);		
		break;
	case QUE_JOURNALCHECKBOX1BTN:
		pDlg->SetItemCheck(0);
		break;
	case QUE_JOURNALCHECKBOX2BTN:
		pDlg->SetItemCheck(1);
		break;
	case QUE_JOURNALCHECKBOX3BTN:
		pDlg->SetItemCheck(2);
		break;
	case QUE_JOURNALCHECKBOX4BTN:
		pDlg->SetItemCheck(3);
		break;
	case QUE_JOURNALCHECKBOX5BTN:
		pDlg->SetItemCheck(4);
		break;
	case QUE_JOURNALSAVE:
		if(pDlg->IsSavedJournal())
			pDlg->SelectedJournalDelete();
		else
			pDlg->SelectedJournalSave();
		break;
	case QUE_JOURNALSAVEDLIST:
		pDlg->ViewJournalListToggle();
		break;
	}
}
void BAIL_BailDlgFunc( LONG lId, void* p, DWORD we )
{
	CBailDialog* pDlg = (CBailDialog*)p;
	switch(lId)
	{
	case BAIL_OK:
			pDlg->SetFame();
		break;
	case BAIL_CANCEL:
			pDlg->Close();
		break;
	}
}
#include "AutoPath.h"
#include "AIManager.h"
#include "JackFlashRoar.h"
void MP_RegistDlgFunc( LONG lId, void* p, DWORD we )
{
	switch(lId)
	{
	case MP_ROKBTN:
		{
#ifdef  _MUTIPET_
		if (PETMGR->IsSummonPet())
		{
			CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(1277));
			return;
		}
#else
			if(PETMGR->GetCurSummonPet())
			{
				CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(1252));//1277//kiv 1132 26042022
				return;
			}
#endif //  _MUTIPET_
			GAMEIN->GetMPRegistDialog()->SetDisable(TRUE);
			GAMEIN->GetInventoryDialog()->SetDisable(TRUE);
			GAMEIN->GetMugongSuryunDialog()->SetDisable(TRUE);
//----------------------------------------
		//	AUTOPATH->SetFindPath( false );
		//	sCPlayerAI.SetOpen(FALSE);
		//	AISETMGR->GetGameAI()->sPage5.pbAutoLeveling==false;
		//	GAMEIN->GetJackFlashDlg()->SetMapChange(true);
		//	sCPlayerAI.IsRun=FALSE;

			sCPlayerAI.BeforeMapChange();
//----------------------------------------
			SURYUNMGR->GoSuryunMapSyn();
		}
		break;
	case MP_RCANCELBTN:
		{
			GAMEIN->GetMPRegistDialog()->SetActive(FALSE);
		}
	}	
}
void GD_DlgFunc( LONG lId, void* p, DWORD we )
{
	switch(lId)
	{
	case GD_CCREATEOKBTN:
		{	
			if(HERO->GetGuildIdx())	
			{
				MSG_MUNHAINFOUPDATE msg;
				SetProtocol( &msg, MP_GUILD, MP_GUILD_INFOUPDATE_SYN );
				msg.dwObjectID = HEROID;
				msg.Index = GUILDMGR->GetGuild()->GetGuildIdx();
				char strInfo[MAX_GUILD_INTRO+1] = { 0, };
				cTextArea* pIntro = (cTextArea *)WINDOWMGR->GetWindowForIDEx(GD_CINTROTEXT);
				pIntro->GetScriptText(strInfo);
				SafeStrCpy(msg.Intro, strInfo, MAX_GUILD_INTRO+1);
				int len = strlen(msg.Intro);
				if(len > MAX_GUILD_INTRO)
				{
					CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(32));
					return;
				}
				NETWORK->Send(&msg, sizeof(msg));
				GAMEIN->GetGuildCreateDlg()->SetActive( FALSE );
			}
			else	
			{
				cEditBox * pGuildName = (cEditBox *)WINDOWMGR->GetWindowForIDEx(GD_CNAME);
				if(strcmp(pGuildName->GetEditText(), "") == 0)
					return;
				cTextArea * pIntro = (cTextArea *)WINDOWMGR->GetWindowForIDEx(GD_CINTROTEXT);
				char FullText[100];
				pIntro->GetScriptText(FullText);
				int len = strlen(FullText);
				if(len > MAX_GUILD_INTRO)
				{
					CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(32));
					return;
				}
				GUILDMGR->CreateGuildSyn(pGuildName->GetEditText(), FullText);
			}
		}
		break;
	case GD_IOKBTN:
		{
			if( GUILDMGR->GetInvitedKind() == AsMember )
			{
				GUILDMGR->AddMemberAccept();
			}
			else if( GUILDMGR->GetInvitedKind() == AsStudent )
			{
				GUILDMGR->AddStudentAccept();
			}
		}
		break;
	case GD_ICANCELBTN:
		{
			GUILDMGR->AddMemberDeny();
		}
		break;	
	case GD_MEMBERINFOPUSHUPBTN:
		{
			GAMEIN->GetGuildDlg()->ShowGuildMode(eGuildMode_GuildInfo);
		}
		break;
	case GD_GUILFUNCPUSHUPBTN:
		{
			GAMEIN->GetGuildDlg()->ShowGuildMode(eGuildMode_Func);
		}
		break;
	case GD_BANMEMBER:
		{
			if(HERO->GetGuildMemberRank() < GUILD_MASTER)
				return;
			if((GUILDMGR->GetSelectedMemberID() == 0) || (GUILDMGR->GetSelectedMemberID() == HEROID))
			{
				CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(714));
			}
			else
			{
				char msg[128];
				sprintf(msg, CHATMGR->GetChatMsg( 46 ), GUILDMGR->GetSelectedMemberName());
				WINDOWMGR->MsgBox( MBI_GUILD_BAN, MBT_YESNO, msg);
				GUILDMGR->SetActiveFunc(TRUE);
			}				
		}
		break;
	case GD_INVITEMEMBER:
		{
#ifdef _HK_LOCAL_	
			CObject * targetObj = OBJECTMGR->GetSelectedObject();
			if(targetObj)
			{
				if(targetObj->GetObjectKind()  == eObjectKind_Player)
				{
					if(((CPlayer*)targetObj)->GetGuildIdx())
					{
						CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(38));
						return;
					}
					else
						GUILDMGR->AddMemberSyn(targetObj->GetID());
				}
			}
#else
			CObject * targetObj = OBJECTMGR->GetSelectedObject();
			if(targetObj)
			{
				if(targetObj->GetObjectKind()  == eObjectKind_Player)
				{
					if(((CPlayer*)targetObj)->GetGuildIdx())
					{
						CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(38));
						return;
					}
					else
						GAMEIN->GetGuildJoinDialog()->SetActive(TRUE);
				}
			}
#endif
		}
		break;
	case GD_GIVEMEMBERNICK:
		{	
			if(GUILDMGR->GetGuildLevel() < GUILD_2LEVEL)
			{
				CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(723));
				return;
			}
			GAMEIN->GetGuildNickNameDlg()->SetActive(TRUE);
		}
		break;
	case GD_GIVEMEMBERRANK:
		{
			if(HERO->GetGuildMemberRank() < GUILD_VICEMASTER)
				return;
			GAMEIN->GetGuildRankDlg()->SetActive(TRUE);
		}
		break;
	case GD_SECEDE:
		{
			if(HERO->GetGuildIdx() == 0)
				return;
			if(( GUILD_STUDENT <= HERO->GetGuildMemberRank() ) && (HERO->GetGuildMemberRank() <= GUILD_VICEMASTER) )
				WINDOWMGR->MsgBox( MBI_GUILD_SECEDE, MBT_YESNO, CHATMGR->GetChatMsg( 347 ) );
		}
		break;
	case GD_NOTICE:
		{
			GAMEIN->GetGuildNoticeDlg()->SetActive(TRUE);			
		}
		break;
	case GD_INVITEUNION:
		{
			CObject* targetObj = OBJECTMGR->GetSelectedObject();
			if( targetObj )
			{
				if( targetObj->GetObjectKind() == eObjectKind_Player )
				{
					GUILDUNION->SendInviteGuildUnion( (CPlayer*)targetObj );
				}
			}
		}
		break;			
	case GD_SECEDEUNION:
		{
			GUILDUNION->SendSecedeGuildUnion();
		}
		break;
	case GD_REMOVEUNION:
		{
			GAMEIN->GetGFWarInfoDlg()->ShowGuildUnion();
		}
		break;
	case GD_DECLARE:
		{
			if( GUILDFIELDWAR->CheckGuildFieldWarCondition( 0 ) )
				GAMEIN->GetGFWarDeclareDlg()->SetActive( TRUE );
		}
		break;
	case GD_SUGGEST:
		{
			if( GUILDFIELDWAR->CheckGuildFieldWarCondition( 1 ) )
				GAMEIN->GetGFWarInfoDlg()->ShowSuggest();
		}
		break;
	case GD_SURRENDER:
		{
			if( GUILDFIELDWAR->CheckGuildFieldWarCondition( 2 ) )
				GAMEIN->GetGFWarInfoDlg()->ShowSurrend();
		}
		break;
	case GD_POSITION:
		{
			GAMEIN->GetGuildDlg()->SortMemberListbyPosition();
		}
		break;
	case GD_RANK:    
		{
			GAMEIN->GetGuildDlg()->SortMemberListbyLevel();
		}
		break;
	case GD_PLUSTIME: 
		{
			GAMEIN->GetGuildPlusTimeDlg()->SetActive( TRUE ); 
		}
		break;
	case GD_RELATIONBTN: 
		{
			GAMEIN->GetGuildWarInfoDlg()->SetActive( TRUE );
		}
		break;
	case GD_MUNHA:
		{
			GAMEIN->GetGuildMunhaDialog()->SetActive( TRUE );
		}	
		break;
	case GDM_REGISTOKBTN:
		{
			cEditBox* pMarkName = (cEditBox*)WINDOWMGR->GetWindowForIDEx(GDM_NAMEEDIT);
			if(strcmp(pMarkName->GetEditText(), "") == 0)
				return;
			if( GUILDMARKMGR->ChangeGuildMarkSyn(HERO->GetGuildIdx(), pMarkName->GetEditText()) == FALSE )
				CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(709));
			GAMEIN->GetGuildMarkDlg()->SetActive(FALSE);			
		}
		break;
	case GUM_REGISTOKBTN:
		{
			cEditBox* pMarkName = (cEditBox*)WINDOWMGR->GetWindowForIDEx( GDM_NAMEEDIT );
			if( strcmp(pMarkName->GetEditText(), "") == 0 )	return;
			if( GUILDUNION->CheckGuildUnionCondition( 5 ) )
			{
				if( !GUILDUNIONMARKMGR->RegistGuildUnionMark( g_nServerSetNum, HERO->GetGuildUnionIdx(), 
					HERO->GetGuildUnionMarkIdx(), pMarkName->GetEditText() ) )
					CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(709));
				GAMEIN->GetGuildMarkDlg()->SetActive(FALSE);			
			}
		}
		break;
	case GD_LUOKBTN:
		{
			GUILDMGR->LevelUpSyn();
		}
		break;
	case GD_NREGISTOKBTN:
		{
			if(GUILDMGR->GetGuildLevel() < GUILD_2LEVEL)
			{
				CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(723));
				return;
			}
			if(GUILDMGR->GetSelectedMemberID())
			{
				cEditBox* pName = (cEditBox*)WINDOWMGR->GetWindowForIDEx(GD_NICKNAMEEDIT);
				if(strcmp(pName->GetEditText(), "") == 0)
					return;
				if(strlen(pName->GetEditText()) > MAX_GUILD_NICKNAME)
				{
					CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(98));
					return;
				}
				if((  FILTERTABLE->IsUsableName(pName->GetEditText(), FALSE ) == FALSE ) || (FILTERTABLE->IsCharInString(pName->GetEditText(), " '") == TRUE))
				{
					CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(96));
					return;
				}
				GUILDMGR->GiveNickNameSyn(pName->GetEditText());
				GAMEIN->GetGuildNickNameDlg()->SetActive(FALSE);
			}
			else
				CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(714));
		}
		break;
	case GDR_OKBTN:
		{
			GAMEIN->GetGuildRankDlg()->SetActive(FALSE);
			BYTE Rank=0;
			cComboBox* pRankCombo = (cComboBox*)WINDOWMGR->GetWindowForIDEx(GDR_RANKCOMBOBOX);
			char buff[16];
			strcpy(buff,pRankCombo->GetComboText());
			CMD_ST(buff)
			CMD_CS( CHATMGR->GetChatMsg(155) )
			Rank = GUILD_MEMBER;
			CMD_CS( CHATMGR->GetChatMsg(156) )
			Rank = GUILD_SENIOR;
			CMD_CS( CHATMGR->GetChatMsg(157) )
			Rank = GUILD_VICEMASTER;
			CMD_EN
			GUILDMGR->ChangeMemberRank(Rank);
		}
		break;
	case GDR_DOKBTN:
		{
			GAMEIN->GetGuildRankDlg()->SetActive(FALSE);
			BYTE DRank;
			cComboBox* pDRankCombo = (cComboBox*)WINDOWMGR->GetWindowForIDEx(GDR_DRANKCOMBOBOX);
			char buff[16];
			strcpy(buff,pDRankCombo->GetComboText());
			CMD_ST(buff)
			CMD_CS( CHATMGR->GetChatMsg(713) )
			DRank = GUILD_MEMBER;
			CMD_CS( CHATMGR->GetChatMsg(712) )
			DRank = GUILD_VICEMASTER;
			CMD_EN			
			GUILDMGR->ChangeMemberRank(DRank);
		}
		break;
	case GDW_WAREHOUSE1BTN: case GDW_WAREHOUSE2BTN:
		{
			GAMEIN->GetGuildWarehouseDlg()->OnActionEvent(lId, p, we);
		}
		break;
	case GDW_PUTINMONEYBTN:
		{
			GAMEIN->GetGuildWarehouseDlg()->PutInOutMoney(1);
		}
		break;
	case GDW_PUTOUTMONEYBTN:
		{
			GAMEIN->GetGuildWarehouseDlg()->PutInOutMoney(0);
		}
		break;
	case GDU_OKBTN:
		{
			cEditBox* pGuildUnionName = (cEditBox*)WINDOWMGR->GetWindowForIDEx(GDU_NAME);
			if( strcmp(pGuildUnionName->GetEditText(), "") == 0 )	return;
			if( strlen(pGuildUnionName->GetEditText()) > MAX_GUILD_NAME )	return;
			GUILDUNION->SendCreateGuildUnion( pGuildUnionName->GetEditText() );
		}
		break;
	default:
		{
			int a = 0;
		}
		break;
	}
}
void ITMALL_DlgBtnFunc(LONG lId, void * p, DWORD we)
{
	if(lId == ITMALL_BTN1 || lId == ITMALL_BTN2 || lId == ITMALL_BTN3)
		GAMEIN->GetItemShopDialog()->TabChange(lId-ITMALL_BTN1);
	if(GAMEIN->GetItemShopDialog()->IsActive())//[右键仓储][BY:十里坡剑传奇][QQ:112582793][2019-4-16][21:31:30]
		GAMEIN->GetItemShopDialog()->OnActionEvnet(lId,p,we);
}
void SA_DlgBtnFunc(LONG lId, void * p, DWORD we)
{
	if( lId == SA_MOVETABBTN1 )
	{
		GAMEIN->GetMoveDialog()->SetButton( 0 );
		return;
	}
	if( lId == SA_MOVETABBTN2 )
	{
		GAMEIN->GetMoveDialog()->SetButton( 1 );
		return;
	}
	if( !GAMEIN->GetMoveDialog()->IsTownMove() && !GAMEIN->GetMoveDialog()->GetSelectedDBIdx() )
		return;
	switch(lId)
	{
	case SA_CHANGNAME:
		{
			GAMEIN->GetPointSaveDialog()->SetDialogStatus( FALSE );			
			GAMEIN->GetPointSaveDialog()->SetActive( TRUE );
			GAMEIN->GetMoveDialog()->SetDisable(TRUE);
		}		
		break;
	case SA_MOVEOK:		
		{
#ifdef _KOR_LOCAL_
			GAMEIN->GetMoveDialog()->MapMoveOK();		
#else
			if( GAMEIN->GetMoveDialog()->IsTownMove() )
				WINDOWMGR->MsgBox( MBI_SAVEDPOINTMOVE, MBT_YESNO, CHATMGR->GetChatMsg(781) );
			else
				WINDOWMGR->MsgBox( MBI_SAVEDPOINTMOVE, MBT_YESNO, CHATMGR->GetChatMsg(759) );		
#endif		
		}
		break;
	case SA_MOVEDEL:
		WINDOWMGR->MsgBox( MBI_SAVEDPOINTDEL, MBT_YESNO, CHATMGR->GetChatMsg(760) );
		break;
	case CMI_CLOSEBTN:
		GAMEIN->GetMoveDialog()->SetItemToMapServer( 0, 0, FALSE );
		break;
	}
}
extern void CHA_DlgFunc(LONG lId, void * p, DWORD we)
{
	switch(lId)
	{
	case CHA_NAME_OKBTN:
		GAMEIN->GetPointSaveDialog()->ChangePointName();
		GAMEIN->GetMoveDialog()->SetDisable(FALSE);
		break;
	case CHA_NAME_CANCELBTN:
		GAMEIN->GetPointSaveDialog()->CancelPointName();
		GAMEIN->GetMoveDialog()->SetDisable(FALSE);
		break;
	}
}
extern void GFW_DlgFunc( LONG lId, void* p, DWORD we )
{
	switch( lId )
	{
	case GFW_DECLARE_OKBTN:
		{
			cEditBox* pName = (cEditBox*)WINDOWMGR->GetWindowForIDEx( GFW_DECLARE_EDITBOX );
			cEditBox* pMoney = (cEditBox*)WINDOWMGR->GetWindowForIDEx( GFW_MONEY_EDITBOX );
			GUILDFIELDWAR->SendGuildFieldWarDeclare( pName->GetEditText(), RemoveComma(pMoney->GetEditText()) );
		}
		break;
	case GFW_DECLARE_CANCELBTN:
		{
			GAMEIN->GetGFWarDeclareDlg()->SetActive( FALSE );
		}
		break;
	case GFW_MONEY_CHK:
		{
			GAMEIN->GetGFWarDeclareDlg()->ShowMoneyEdit();
		}
		break;
	case GFW_RESULT_OKBTN:
		{
			if( GUILDFIELDWAR->CheckGuildFieldWarCondition( 4 ) )
			{
				GAMEIN->GetGFWarResultDlg()->SetDisable( TRUE );
				WINDOWMGR->MsgBox( MBI_GUILDFIELDWAR_DECLARE_ACCEPT, MBT_YESNO, CHATMGR->GetChatMsg( 813 ) );
			}
		}
		break;
	case GFW_RESULT_CANCELBTN:
		{
			GAMEIN->GetGFWarResultDlg()->SetDisable( TRUE );
			WINDOWMGR->MsgBox( MBI_GUILDFIELDWAR_DECLARE_DENY, MBT_YESNO, CHATMGR->GetChatMsg( 814 ) );
		}
		break;
	case GFW_RESULT_CONFIRMBTN:
		{
			GAMEIN->GetGFWarResultDlg()->SetActive( FALSE );
		}
		break;
	case GFW_INFO_SUGGEST_OKBTN:
		{
			GAMEIN->GetGFWarInfoDlg()->SetDisable( TRUE );
			GUILDINFO* pInfo = GUILDFIELDWAR->GetEnemyGuildInfoFromListIdx( GAMEIN->GetGFWarInfoDlg()->GetSelectedListIdx() );
			if( pInfo )
				WINDOWMGR->MsgBox( MBI_GUILDFIELDWAR_SUGGEST, MBT_YESNO, CHATMGR->GetChatMsg( 831 ), pInfo->GuildName );
			else
			{
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg( 847 ) );
				GAMEIN->GetGFWarInfoDlg()->SetDisable( FALSE );
			}
		}
		break;
	case GFW_INFO_SURREND_OKBTN:
		{
			GAMEIN->GetGFWarInfoDlg()->SetDisable( TRUE );
			GUILDINFO* pInfo = GUILDFIELDWAR->GetEnemyGuildInfoFromListIdx( GAMEIN->GetGFWarInfoDlg()->GetSelectedListIdx() );
			if( pInfo )
				WINDOWMGR->MsgBox( MBI_GUILDFIELDWAR_SURREND, MBT_YESNO, CHATMGR->GetChatMsg( 825 ), pInfo->GuildName );
			else
			{
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg( 847 ) );
				GAMEIN->GetGFWarInfoDlg()->SetDisable( FALSE );
			}
		}
		break;
	case GFW_GDU_REMOVE_OKBTN:	
		{
			char* pName = GUILDUNION->GetGuildUnionNameFromListIdx( GAMEIN->GetGFWarInfoDlg()->GetSelectedListIdx() );
			if( pName )
				WINDOWMGR->MsgBox( MBI_GUILD_UNION_REMOVE, MBT_YESNO, CHATMGR->GetChatMsg(1116), pName );
			else
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg( 847 ) );
		}
		break;
	case GFW_INFO_CANCELBTN:
		{
			GAMEIN->GetGFWarInfoDlg()->SetActive( FALSE );
		}
		break;
	case GW_INFOCLOSEBTN:
		{
			GAMEIN->GetGuildWarInfoDlg()->SetActive( FALSE );
		}
		break;
	}
}
extern void SK_DlgBtnFunc( LONG lId, void* p, DWORD we )
{
	switch( lId )
	{
	case SK_UPBTN:
		{
			char buf[256];
			if( GAMEIN->GetSkPointDlg()->GetCurAbilityName() )
			{
				int Level = GAMEIN->GetSkPointDlg()->GetCurAbilityLevel();
				if( Level == -1 )					return;
				CAbilityInfo* pInfo = GAMEIN->GetSkPointDlg()->GetCurAbilityInfo();
				if( !pInfo )	return;
				if(ABILITYMGR->CanUpgradeSkPoint(pInfo,HERO->GetAbilityGroup()) == FALSE)		return;
#ifdef _TL_LOCAL_
				sprintf( buf, CHATMGR->GetChatMsg(739), Level+1, GAMEIN->GetSkPointDlg()->GetCurAbilityName(), Level );
#else
				sprintf( buf, CHATMGR->GetChatMsg(739), GAMEIN->GetSkPointDlg()->GetCurAbilityName(), Level, Level+1 );
#endif
				WINDOWMGR->MsgBox( MBI_SKPOINTREDIST_UP, MBT_YESNO, buf );
				GAMEIN->GetSkPointDlg()->SetDisable( TRUE );
			}			
		}
		break;
	case SK_DOWNBTN:
		{
			char buf[256];
			if( GAMEIN->GetSkPointDlg()->GetCurAbilityName() )
			{
				int Level = GAMEIN->GetSkPointDlg()->GetCurAbilityLevel();				
				if( Level > 0 )
				{
#ifdef _TL_LOCAL_
					sprintf( buf, CHATMGR->GetChatMsg(739), Level-1, GAMEIN->GetSkPointDlg()->GetCurAbilityName(), Level );
#else
					sprintf( buf, CHATMGR->GetChatMsg(739), GAMEIN->GetSkPointDlg()->GetCurAbilityName(), Level, Level-1 );
#endif
					WINDOWMGR->MsgBox( MBI_SKPOINTREDIST_DOWN, MBT_YESNO, buf );
					GAMEIN->GetSkPointDlg()->SetDisable( TRUE );
				}
			}
		}
		break;
	case SK_OKBTN:
		{			
			GAMEIN->GetSkPointDlg()->SetActive( FALSE );
		}
		break;
	case SK_STARTBTN:
		{
			MOVEMGR->HeroMoveStop();
			GAMEIN->GetSkPointNotifyDlg()->SetActive( FALSE );
			GAMEIN->GetSkPointDlg()->SetActive( TRUE );
			OBJECTSTATEMGR->StartObjectState( HERO, eObjectState_Deal );
		}
		break;
	case SK_POINTAGAIN1BTN:
	case SK_POINTAGAIN2BTN:
	case SK_POINTAGAIN3BTN:
		{
			if( (lId-SK_POINTAGAIN1BTN) == GAMEIN->GetSkPointDlg()->GetTabNumber() )
				return;
			GAMEIN->GetSkPointDlg()->SetTabNumber( lId-SK_POINTAGAIN1BTN );
		}
		break;
	}
}
void PW_DlgFunc( LONG lId, void* p, DWORD we )
{
	switch( lId )
	{
	case PW_CB_MEMBER0:	case PW_CB_MEMBER1:	case PW_CB_MEMBER2:	case PW_CB_MEMBER3:	case PW_CB_MEMBER4:
	case PW_CB_MEMBER5:	case PW_CB_MEMBER6:	case PW_CB_MEMBER7:	case PW_CB_MEMBER8:	case PW_CB_MEMBER9:
	case PW_CB_MEMBER10:	case PW_CB_MEMBER11:	case PW_CB_MEMBER12:	case PW_CB_MEMBER13:
		{
			PARTYWAR->SendPartyWarMember( lId - PW_CB_MEMBER0 );
		}
		break;
	case PW_BTN_LOCK:	PARTYWAR->SendPartyWarLock( TRUE );		break;
	case PW_BTN_UNLOCK:	PARTYWAR->SendPartyWarLock( FALSE );	break;
	case PW_BTN_START:	PARTYWAR->SendPartyWarStart();			break;
	case PW_BTN_CANCEL:	PARTYWAR->SendPartyWarCancel();			break;
	}
}
void CHS_DlgFunc( LONG lId, void* p, DWORD we )
{
	switch( lId )
	{
	case CHASE_SEEOKBTN:
		GAMEIN->GetChaseDlg()->SetActive( FALSE );
		break;
	case CHASE_OKBTN:
		GAMEIN->GetChaseinputDlg()->WantedChaseSyn();
		break;
	case CHASE_CANCELBTN:
		GAMEIN->GetChaseinputDlg()->SetActive( FALSE );
		break;
	}
}
void CHAN_DlgFunc(LONG lId, void * p, DWORD we)
{
	switch( lId )
	{
	case CH_NAME_CHANGE_OKBTN:
		{
			GAMEIN->GetInventoryDialog()->SetDisable( FALSE );
			GAMEIN->GetNameChangeDlg()->NameChangeSyn();
		}		
		break;
	case CH_NAME_CHANGE_CANCELBTN:
		{
			GAMEIN->GetInventoryDialog()->SetDisable( FALSE );
			GAMEIN->GetNameChangeDlg()->SetItemDBIdx( 0 );
			GAMEIN->GetNameChangeDlg()->SetActive( FALSE );
		}
		break;
	case CH_NAME_CHANGE_STARTBTN:
		{
			/*CItem* pItem = GAMEIN->GetInventoryDialog()->GetCurSelectedItem(SHOPIVN);
			if( pItem )
			{
				ITEM_INFO * pInfo = ITEMMGR->GetItemInfo(pItem->GetItemIdx());
				GAMEIN->GetNameChangeDlg()->SetItemFlag( pInfo->SellPrice );
				GAMEIN->GetNameChangeDlg()->SetItemDBIdx( pItem->GetDBIdx() );
				GAMEIN->GetNameChangeDlg()->SetActive( TRUE );
				GAMEIN->GetNameChangeNotifyDlg()->SetActive( FALSE );
			}*/
			CItem* pItem = GAMEIN->GetInventoryDialog()->GetCurSelectedItem(SHOPIVN);
			if( pItem )
			{
				//GAMEIN->GetNameChangeDlg()->SetItemDBIdx( pItem->GetDBIdx() );
				//GAMEIN->GetNameChangeDlg()->SetActive( TRUE );
				//GAMEIN->GetNameChangeNotifyDlg()->SetActive( FALSE );
				ITEM_INFO * pInfo = ITEMMGR->GetItemInfo(pItem->GetItemIdx());
				GAMEIN->GetNameChangeDlg()->SetItemFlag( pInfo->SellPrice );
				GAMEIN->GetNameChangeDlg()->SetItemDBIdx( pItem->GetDBIdx() );
				GAMEIN->GetNameChangeDlg()->SetActive( TRUE );
				GAMEIN->GetNameChangeNotifyDlg()->SetActive( FALSE );
			}
			else
			{
				pItem = GAMEIN->GetInventoryDialog()->GetCurSelectedItem(SHOPIVNPLUS);
				if(pItem)
				{
					ITEM_INFO * pInfo = ITEMMGR->GetItemInfo(pItem->GetItemIdx());
					GAMEIN->GetNameChangeDlg()->SetItemFlag( pInfo->SellPrice );
					GAMEIN->GetNameChangeDlg()->SetItemDBIdx( pItem->GetDBIdx() );
					GAMEIN->GetNameChangeDlg()->SetActive( TRUE );
					GAMEIN->GetNameChangeNotifyDlg()->SetActive( FALSE );
					//GAMEIN->GetNameChangeDlg()->SetItemDBIdx( pItem->GetDBIdx() );
					//GAMEIN->GetNameChangeDlg()->SetActive( TRUE );
					//GAMEIN->GetNameChangeNotifyDlg()->SetActive( FALSE );
				}
			}
		}
		break;
	}
}
void GDT_DlgFunc(LONG lId, void * p, DWORD we)
{
	switch( lId )
	{
	case GDT_ENTRYBTN:
		{
			WINDOWMGR->MsgBox( MBI_GTREGIST_SYN, MBT_YESNO, CHATMGR->GetChatMsg(960) );
		}
		break;
	case GDT_CANCELBTN:
		{
			WINDOWMGR->MsgBox( MBI_GTREGISTCANCEL_SYN, MBT_YESNO, CHATMGR->GetChatMsg(964) );
		}
		break;
	case GDT_WARVIEWBTN:
		{
			if( GAMEIN->GetGTBattleListDlg() )
			{
				if( GAMEIN->GetGTBattleListDlg()->GetSelectedBattle() > -1 )
					WINDOWMGR->MsgBox( MBI_GTBATTLELIST_OBSERVER, MBT_YESNO, CHATMGR->GetChatMsg(956) );
				else
					CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(957) );
			}
		}
		break;
	case GDT_VIEWCANCELBTN:
		{
			if( GAMEIN->GetGTBattleListDlg() )
			{
				if( GAMEIN->GetGTBattleListDlg()->IsActive() )
					GAMEIN->GetGTBattleListDlg()->SetActive( FALSE );
			}
		}
		break;
	case GDT_SEMIFINAL_PUSHUPBTN:
		{
			if( GAMEIN->GetGTStandingDlg() )			
				GAMEIN->GetGTStandingDlg()->SetCurPage( 0 );
		}
		break;
	case GDT_AGROUP_PUSHUPBTN:
		{
			if( GAMEIN->GetGTStandingDlg() )			
				GAMEIN->GetGTStandingDlg()->SetCurPage( 1 );
		}
		break;
	case GDT_BGROUP_PUSHUPBTN:
		{
			if( GAMEIN->GetGTStandingDlg() )			
				GAMEIN->GetGTStandingDlg()->SetCurPage( 2 );
		}
		break;
	case GDT_CGROUP_PUSHUPBTN:
		{
			if( GAMEIN->GetGTStandingDlg() )			
				GAMEIN->GetGTStandingDlg()->SetCurPage( 3 );
		}
		break;
	case GDT_DGROUP_PUSHUPBTN:
		{
			if( GAMEIN->GetGTStandingDlg() )			
				GAMEIN->GetGTStandingDlg()->SetCurPage( 4 );
		}
		break;
	case GDT_OUTBTN:
		{
			WINDOWMGR->MsgBox( MBI_GTSCORE_EXIT, MBT_YESNO, CHATMGR->GetChatMsg(969) );
		}
		break;
	}
}
void SW_DlgFunc(LONG lId, void* p, DWORD we)
{
	switch( lId )
	{
	case SW_TIMEREG_OKBTN:
		{
			cEditBox* pYear = (cEditBox*)WINDOWMGR->GetWindowForIDEx(SW_TIME_YEAR);
			cEditBox* pMon = (cEditBox*)WINDOWMGR->GetWindowForIDEx(SW_TIME_MON);
			cEditBox* pDay = (cEditBox*)WINDOWMGR->GetWindowForIDEx(SW_TIME_DAY);
			cEditBox* pHour = (cEditBox*)WINDOWMGR->GetWindowForIDEx(SW_TIME_HOUR);
			if( !pYear || !pMon || !pDay || !pHour )
			{
				GAMEIN->GetSWTimeRegDlg()->SetActive( FALSE );
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1120) );
				return;
			}
			if( (strcmp(pYear->GetEditText(), "") == 0) || (strcmp(pMon->GetEditText(), "") == 0) ||
				(strcmp(pDay->GetEditText(), "") == 0) || (strcmp(pHour->GetEditText(), "") == 0) )
			{
				GAMEIN->GetSWTimeRegDlg()->SetActive( FALSE );
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1120) );
				return;
			}
			DWORD dwYear = (DWORD)atoi( pYear->GetEditText() );
			DWORD dwMon = (DWORD)atoi( pMon->GetEditText() );
			DWORD dwDay = (DWORD)atoi( pDay->GetEditText() );
			DWORD dwHour = (DWORD)atoi( pHour->GetEditText() );
			if( (dwYear<2005) || (dwMon>12||dwMon<=0) || (dwDay>31||dwDay<=0) || (dwHour>23||dwHour<0) )
			{
				GAMEIN->GetSWTimeRegDlg()->SetActive( FALSE );
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1120) );
				return;
			}
			if( dwYear%4 == 0 )
				if( dwDay > DayOfMonth_Yundal[dwMon] )	return;
			else
				if( dwDay > DayOfMonth[dwMon] )			return;
			stTIME time;
			time.SetTime( dwYear-2015, dwMon, dwDay, dwHour, 0, 0 );	
			MSG_DWORD msg;
			msg.Category = MP_SIEGEWAR;
			msg.Protocol = MP_SIEGEWAR_REGISTTIME_SYN;
			msg.dwObjectID = HEROID;
			msg.dwData = time.value;
			NETWORK->Send( &msg, sizeof(msg) );
			GAMEIN->GetSWTimeRegDlg()->SetActive( FALSE );
		}
		break;
	case SW_PROTECTREG_OKBTN:
		{
			int nIdx = GAMEIN->GetSWProtectRegDlg()->GetSelectedListIdx();
			if( nIdx > -1 )
			{
				WINDOWMGR->MsgBox( MBI_SW_PROTECTREG_OK, MBT_YESNO, CHATMGR->GetChatMsg(1121), GAMEIN->GetSWProtectRegDlg()->GetSelectedGuildName( nIdx ) );
				GAMEIN->GetSWProtectRegDlg()->SetDisable( TRUE );
			}
			else
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1118) );				
		}
		break;
	case SW_INFOCLOSEBTN:
		{
			GAMEIN->GetSWInfoDlg()->SetActive( FALSE );
		}
		break;
	case SW_TAXCHANGE_OKBTN:
		{
			cEditBox* pTax = (cEditBox*)WINDOWMGR->GetWindowForIDEx(SW_CHANGE_TAX);
			if( pTax )
			{
				if( SWPROFIT->CheckSWProfit( 0, atoi(pTax->GetEditText()), 0 ) )
				{
					WINDOWMGR->MsgBox( MBI_SW_TAXCHANGE_OK, MBT_YESNO, CHATMGR->GetChatMsg(1029), atoi(pTax->GetEditText()) );
					GAMEIN->GetSWProfitDlg()->SetDisable( TRUE );
				}
			}
		}
		break;
	case SW_PROFIT_OKBTN:
		{
			cEditBox* pMoney = (cEditBox*)WINDOWMGR->GetWindowForIDEx(SW_OUT_MONEY);
			if( pMoney )
			{
				if( SWPROFIT->CheckSWProfit( 1, 0, atoi(pMoney->GetEditText()) ) )
				{
					WINDOWMGR->MsgBox( MBI_SW_PROFIT_OK, MBT_YESNO, CHATMGR->GetChatMsg(1030), atoi(pMoney->GetEditText()) );
					GAMEIN->GetSWProfitDlg()->SetDisable( TRUE );
				}
			}
		}
		break;
	case SW_PROFIT_CANCELBTN:
		{
			GAMEIN->GetSWProfitDlg()->SetActive( FALSE );
		}
		break;
	case SW_ENGRAVECANCEL:
		{
			if( GAMEIN->GetSWEngraveDlg() )
				GAMEIN->GetSWEngraveDlg()->CancelEngraveSyn();
		}
		break;
	case SW_STARTTIME_OKBTN:
		{
			GAMEIN->GetSWStartTimeDlg()->SetActive( FALSE );
		}
		break;
	}
}
#include "AiManager.h"
void CHA_ChangeDlgFunc(LONG lId, void* p, DWORD we)
{
	CCharChangeDlg* pDlg = (CCharChangeDlg*)p;
	switch( lId )
	{
	case CHA_CharMake:
		pDlg->CharacterChangeSyn();

		WINDOWMGR->ShowBaseWindow();
		GAMEIN->GetInventoryDialog()->SetActive(true);
		sCPlayerAI.HideObject();
		GAMEIN->GetQuickDialog()->SelectPageDirect(0);

		break;
	case CHA_CharCancel:
		pDlg->Reset( FALSE );
		pDlg->SetActive( FALSE );

		WINDOWMGR->ShowBaseWindow();
		GAMEIN->GetInventoryDialog()->SetActive(true);
		sCPlayerAI.HideObject();
		GAMEIN->GetQuickDialog()->SelectPageDirect(0);

		break;
	case CHA_SexType1:
		pDlg->ChangeSexType( TRUE );
		break;
	case CHA_SexType2:
		pDlg->ChangeSexType( FALSE );
		break;
	case CHA_HairType1:
		pDlg->ChangeHairType( TRUE );
		break;
	case CHA_HairType2:
		pDlg->ChangeHairType( FALSE );
		break;
	case CHA_FaceType1:
		pDlg->ChangeFaceType( TRUE );
		break;
	case CHA_FaceType2:
		pDlg->ChangeFaceType( FALSE );
		break;
	}
}
void IT_DlgFunc(LONG lId, void* p, DWORD we)
{
	CSealDialog* pDlg = (CSealDialog*)p;
	switch( lId )
	{
	case IT_LOCKOKBTN:
		pDlg->ItemSealSyn();
		break;
	case IT_LOCKCANCERBTN:
		pDlg->ReleaseItem();
		break;
	}
}
void CJOB_DlgFunc(LONG lId, void* p, DWORD we)
{
	CChangeJobDialog* pDlg = (CChangeJobDialog*)p;
	switch( lId )
	{
	case CJOB_OKBTN:
		pDlg->ChangeJobSyn();
		break;
	case CJOB_CANCELBTN:
		pDlg->CancelChangeJob();
		break;		
	}
}
void CG_DlgFunc(LONG lId, void *p, DWORD we)
{
	GAMEIN->GetGuageDlg()->OnActionEvent(lId, p, we);
}
void RFDefault_DlgFunc(LONG lId, void *p, DWORD we)
{
	CReinforceResetDlg* pDlg = (CReinforceResetDlg*)p;
	switch( lId )
	{
	case RF_DefaultOKBTN:
		pDlg->ItemResetSyn();
		break;
	case RF_DefaultCANCERBTN:
		pDlg->ReleaseItem();
		break;
	}
}
void RareCreate_DlgFunc(LONG lId, void *p, DWORD we)
{
    CRareCreateDialog* pDlg = (CRareCreateDialog*)p;
	switch( lId )
	{
	case RareCreate_OKBTN:
		pDlg->ItemRareCreateSyn();
		break;
	case RareCreate_CANCERBTN:
		pDlg->ReleaseItem();
		break;
	}
}
void ItemStoneDk_DlgFunc(LONG lId, void *p, DWORD we)
{
	CItemStoneDkDialog* pDlg=(CItemStoneDkDialog*)p;
	switch(lId)
	{
	case ITEMSTONEDK_BTOK:
		{
			pDlg->SendItemStoneDkMsg();
		}
		break;
	case ITEMSTONEDK_BTCANCER:
		{
			pDlg->Release();
		}
		break;
	}
}
void ItemStoneXq_DlgFunc(LONG lId, void *p, DWORD we)
{
	CItemStoneXqDialog* pDlg=(CItemStoneXqDialog*)p;
	switch(lId)
	{
	case ITEMSTONEXQ_BTOK:
		{
			pDlg->SendItemStoneXqMsg();
		}
		break;
	case ITEMSTONEXQ_BTCANCER:
		{
			pDlg->Release();
		}
		break;
	}
}
void PET_DlgFunc(LONG lId, void *p, DWORD we)
{
	GAMEIN->GetPetStateDialog()->OnActionEvent(lId, p, we);
}
void PET_MiniDlgFunc(LONG lId, void *p, DWORD we)
{
	GAMEIN->GetPetStateMiniDialog()->OnActionEvent(lId, p, we);
}
void RFDGUIDE_DlgFunc(LONG lId, void *p, DWORD we)
{
	GAMEIN->GetReinforceDataGuideDlg()->OnActionEvent(lId, p, we);
}
void TB_DlgFunc(LONG lId, void *p, DWORD we)
{
	GAMEIN->GetTipBrowserDlg()->OnActionEvent(lId, p, we);
}
void TB_STATE_DlgFunc(LONG lId, void *p, DWORD we)
{
	GAMEIN->GetTipBrowserDlg()->OnActionEvent(lId, p, we);
}
void PET_InvenDlgFunc(LONG lId, void *p, DWORD we)
{
	GAMEIN->GetPetInventoryDialog()->OnActionEvent(lId, p, we);
}
void PET_UpgradeDlgFunc(LONG lId, void *p, DWORD we)
{
	CPetUpgradeDialog* pDlg = (CPetUpgradeDialog*)p;
	switch( lId )
	{
	case PetUpgrade_OKBTN:
		{
			pDlg->PetUpgradeSyn();
		}
		break;
	case PetUpgrade_CANCERBTN:
		{
			pDlg->ReleaseItem();
		}
		break;
	}
}
void PET_RevivalDlgFunc(LONG lId, void *p, DWORD we)
{
	CPetRevivalDialog* pDlg = (CPetRevivalDialog*)p;
	switch( lId )
	{
	case PetRevival_OKBTN:
		{
			pDlg->PetRevivalSyn();	
		}
		break;
	case PetRevival_CANCERBTN:
		{
			pDlg->ReleaseItem();
		}
		break;
	}
}
void GN_DlgBtnFunc(LONG lId, void *p, DWORD we)
{
	GAMEIN->GetGuildNoteDlg()->OnActionEvnet(lId, p, we);
}
void AN_DlgBtnFunc(LONG lId, void *p, DWORD we)
{
	GAMEIN->GetUnionNoteDlg()->OnActionEvnet(lId, p, we);
}
void GNotice_DlgBtnFunc(LONG lId, void *p, DWORD we)
{
	GAMEIN->GetGuildNoticeDlg()->OnActionEvnet(lId, p, we);
}
void GuildPlusTime_DlgFunc(LONG IId, void* p, DWORD we)
{
	GAMEIN->GetGuildPlusTimeDlg()->OnActionEvnet(IId,p,we);
}
void SkillTrans_DlgFunc(LONG lId, void *p, DWORD we)
{
	GAMEIN->GetSkillOptionChangeDlg()->OnActionEvnet(lId, p, we);
}
void TDefault_DlgFunc(LONG lId, void *p, DWORD we)
{
	GAMEIN->GetSkillOptionClearDlg()->OnActionEvnet(lId, p, we);
}
void JO_DlgFunc(LONG lId, void* p, DWORD we)
{
	GAMEIN->GetGuildJoinDialog()->OnActionEvent(lId, p, we);
}
void GU_DlgFunc(LONG lId, void* p, DWORD we)
{
	GAMEIN->GetGuildTraineeDialog()->OnActionEvent(lId, p, we);
}
void GT_DlgFunc(LONG lId, void* p, DWORD we)
{
	GAMEIN->GetGuildMunhaDialog()->OnActionEvent(lId, p, we);
}
void TPMD_DlgFunc(LONG lId, void * p, DWORD we)
{
	GAMEIN->GetTitanPartsMakeDlg()->OnActionEvent(lId, p, we);
}
void TitanMix_DlgFunc(LONG lId, void * p, DWORD we)
{
	GAMEIN->GetTitanMixDlg()->OnActionEvent(lId, p, we);
}
void Titan_Repair_DlgFunc(LONG lId, void * p, DWORD we)
{
	GAMEIN->GetTitanRepairDlg()->OnActionEvent(lId, p, we);
}
void Titan_Recall_DlgFunc(LONG lId, void * p, DWORD we)
{
	GAMEIN->GetTitanRecallDlg()->OnActionEvent(lId, p, we);
}
void TitanUpgrade_DlgFunc(LONG lId, void * p, DWORD we)
{
	GAMEIN->GetTitanUpgradeDlg()->OnActionEvent(lId, p, we);
}
void TitanBreak_DlgFunc(LONG lId, void * p, DWORD we)
{
	GAMEIN->GetTitanBreakDlg()->OnActionEvent(lId, p, we);
}
void titan_inventory_DlgFunc(LONG lId, void * p, DWORD we)
{
	GAMEIN->GetTitanInventoryDlg()->OnActionEvent(lId, p, we);
}
void Titan_guage_DlgFunc(LONG lId, void * p, DWORD we)
{
	GAMEIN->GetTitanGuageDlg()->OnActionEvent(lId, p, we);
}
void Titan_Use_DlgFunc(LONG lId, void * p, DWORD we)
{
	GAMEIN->GetTitanRegisterDlg()->OnActionEvent(lId, p, we);
}
void Titan_Bongin_DlgFunc(LONG lId, void * p, DWORD we)
{
	GAMEIN->GetTitanDissolutionDlg()->OnActionEvent(lId, p, we);
}
void UniqueItemCurseCancellationDlgFunc(LONG lId, void * p, DWORD we)
{
	GAMEIN->GetUniqueItemCurseCancellationDlg()->OnActionEvent(lId, p, we);
}
void UniqueItemMixDlgFunc(LONG lId, void * p, DWORD we)
{
	GAMEIN->GetUniqueItemMixDlg()->OnActionEvent(lId, p, we);
}
void SOSDlgFunc(LONG lId, void * p, DWORD we)
{
	GAMEIN->GetSOSDlg()->OnActionEvent(lId, p, we);
}
void UniqueItemMix_ProgressBar_DlgFunc(LONG lId, void * p, DWORD we)
{
	GAMEIN->GetUniqueItemMixProgressBarDlg()->OnActionEvent(lId, p, we);
}
void TitanMix_ProgressBar_DlgFunc(LONG lId, void * p, DWORD we)
{
	GAMEIN->GetTitanMixProgressBarDlg()->OnActionEvent(lId, p, we);
}
void TitanParts_ProgressBar_DlgFunc(LONG lId, void * p, DWORD we)
{
	GAMEIN->GetTitanPartsProgressBarDlg()->OnActionEvent(lId, p, we);
}
void SkinSelect_DlgFunc(LONG lId, void * p, DWORD we)
{
	GAMEIN->GetSkinSelectDlg()->OnActionEvent(lId, p, we);
}
void CostumeSkinSelect_DlgFunc(LONG lId, void * p, DWORD we)
{
	GAMEIN->GetCostumeSkinSelectDlg()->OnActionEvent(lId, p, we);
}
void SK_Reset_DlgFunc(LONG lId, void* p, DWORD we)
{
	GAMEIN->GetSkillPointResetDlg()->OnActionEvent(lId, p, we);
}
void NumberPad_DlgFunc(LONG lId, void * p, DWORD we)
{
	GAMEIN->GetNumberPadDlg()->OnActionEvent(lId, p, we);
}
void MT_LOGINDLGFunc(LONG lId, void * p, DWORD we)
{
	char Buffer[64];
	DWORD dwColor;

	cComboBox* pCombo = (cComboBox*)WINDOWMGR->GetWindowForIDEx(MT_LISTCOMBOBOX);

	if(lId==MT_LISTCOMBOBOX)
	{
		for (int i = 0; i<pCombo->GetMaxLine(); i++)
		{
			TITLE->ReadConfig(i);
		}		
	}
	if(lId==MT_LISTCOMBOBOXBTN)
	{
		dwColor=0xFF0000;
		sprintf(Buffer,RESRCMGR->GetMsg(1471));

		bool bFlag=false;
		char stringtoread[200];
		goto TestBuffer;
	}
	if(we == WE_COMBOBOXSELECT)
	{		
		
		int nIndex = pCombo->GetCurSelectedIdx();
		cDialog * dlg = (cDialog *)p;
		cEditBox * editboxId = (cEditBox *)dlg->GetWindowForID(MT_IDEDITBOX);
		cEditBox * editboxPwd = (cEditBox *)dlg->GetWindowForID(MT_PWDEDITBOX);
		char * userID = editboxId->GetEditText();
		char * userPWD = editboxPwd->GetEditText();

		if(KEYBOARD->GetKeyPressed(KEY_SHIFT))
		{

			if(TITLE->WriteConfig(nIndex,userID,userPWD))
			{
				dwColor=0x00FF00;
				sprintf(Buffer,RESRCMGR->GetMsg(1472));
				goto TestBuffer;
			}
			else
			{
				if(MessageBox(_g_hWnd,"delete!","dialog!",MB_YESNO)==IDNO)
				{
					
					TITLE->WriteConfig(nIndex,"NULL","NULL",false);
					dwColor=0xFF0000;
					sprintf(Buffer,"failed to save");
					goto TestBuffer;
				}
				else
				{
					TITLE->WriteConfig(nIndex,"NULL","NULL",true);

					dwColor=0x00FF00;
					sprintf(Buffer,"deleted");
					goto TestBuffer;
				}
			}
			


			
		}
		editboxId->SetEditText("");
		editboxPwd->SetEditText("");
		TITLE->ReadConfig(nIndex);
		char stringtoread[200];
		memset(stringtoread,0,200);
		SafeStrCpy(stringtoread, TITLE->GetUserName(), MAX_NAME_LENGTH + 1 );
		editboxId->SetEditText(stringtoread);
		USERINFOMGR->SetUserID(stringtoread);
		SafeStrCpy(stringtoread, TITLE->GetUserPwd(), MAX_NAME_LENGTH + 1 );
		editboxPwd->SetEditText(stringtoread);

		editboxPwd->SetFocusEdit(TRUE);
	}
	return;
TestBuffer:
	cDialog * m_pDlg = WINDOWMGR->GetWindowForID(MT_LOGINDLG);
	if(m_pDlg)
	{
		cStatic * m_pTitle = (cStatic*)m_pDlg->GetWindowForID(MT_DIALOGINFO);
		if(m_pTitle)
		{
			m_pTitle->SetFGColor(dwColor);
			m_pTitle->SetStaticText(Buffer);
		}
	}
}
void SVD_DlgFunc(LONG lId, void * p, DWORD we)
{
	GAMEIN->GetStreetStallItemViewDlg()->OnActionEvent(lId, p, we);
}
void AutoNoteDlg_Func(LONG lId, void * p, DWORD we)
{
	GAMEIN->GetAutoNoteDlg()->OnActionEvent(lId, p, we);
}
void AutoAnswerDlg_Func(LONG lId, void * p, DWORD we)
{
	GAMEIN->GetAutoAnswerDlg()->OnActionEvent(lId, p, we);
}
void PA_CreateBtnFunc(LONG lId, void * p, DWORD we) 
{
	GAMEIN->GetPartyCreateDialog()->OnActionEvent(lId, p, we);
}
void Party_MatchingList_DlgFunc(LONG lId, void * p, DWORD we)
{
	GAMEIN->GetPartyMatchingDlg()->OnActionEvent(lId, p, we);
}
void FW_DlgFunc(LONG lId, void* p, DWORD we)
{
	switch( lId )
	{
	case FW_ENGRAVECANCEL:	GAMEIN->GetFWEngraveDlg()->OnActionEvent(lId, p, we);	break;
	}
}
void FD_WareHouseDlgFunc( LONG lId, void* p, DWORD we )
{
	switch( lId )
	{
	case FW_PUTOUTMONEYBTN:	GAMEIN->GetFWWareHouseDlg()->PutOutMoneyMsgBox();	break;
	}
}
void ITEM_DlgFunc(LONG lId, void * p, DWORD we)
{
} 
void TC_DlgFunc(LONG lId, void * p, DWORD we)
{
    GAMEIN->GetGoldShopDialog()->OnActionEvent(lId, p, we);
}
void TCBuy_DlgFunc(LONG lId, void * p, DWORD we)
{
    GAMEIN->GetGoldShopBuyDialog()->OnActionEvent(lId, p, we);
}
void BARCONTROL_DIALOGFunc(LONG lId, void * p, DWORD we)
{
	GAMEIN->GetControlDialog()->OnActionEvent(lId, p, we);
}

void Ranking_DlgFunc(LONG lId, void * p, DWORD we)
{
	GAMEIN->GetTopDialog()->OnActionEvent(lId, p, we);
}
void DGRanking_DlgFunc(LONG lId, void * p, DWORD we)
{
	GAMEIN->GetTopDungeon()->OnActionEvent(lId, p, we);
}
void RankingGuild_DlgFunc(LONG lId, void * p, DWORD we)
{
	GAMEIN->GetTopMunpDialog()->OnActionEvent(lId, p, we);
	/*if( we == WE_BTNCLICK )
	{
	   if(lId>=TOP_MUNP_PAGE1 && lId<=TOP_MUNP_PAGE5)
	   {
		   CTopMunpDialog * pDlg = GAMEIN->GetTopMunpDialog();
		   if(pDlg)
		   {
			   pDlg->SetPage(lId-TOP_MUNP_PAGE1+1);
		   }
	   }
	}*/
}
void Lucker_DlgFunc(LONG lId,void * p,DWORD we)
{
	GAMEIN->GetLuckerDialog()->OnActionEvent(lId, p, we);
}
void IT_LOCKDlgFunc(LONG lId,void * p,DWORD we)
{
    CItemLockDialog* pHandler = (CItemLockDialog*)GAMEIN->GetItemLockDialog();
	if(!pHandler) return;
	switch(lId)
	{
	case IT_LOCKOKBTNEX:
		{
			if(!pHandler->VirtualItemIsNull())
			{
				pHandler->SendSynMsg();
			}
			pHandler->SetActive(FALSE);
		}
		break;
	case IT_LOCKCANCERBTNEX:
		{
			if(pHandler->IsActive())
			{
				pHandler->Release();
			}
		}
		break;
	}
}
void IT_UNLOCKDlgFunc(LONG lId,void * p,DWORD we)
{
    CItemUnLockDialog* pHandler = (CItemUnLockDialog*)GAMEIN->GetItemUnLockDialog();
	if(!pHandler) return;
	switch(lId)
	{
	case IT_UNLOCKOKBTNEX:
		{
			if(!pHandler->VirtualItemIsNull())
			{
				pHandler->SendSynMsg();
			}
			pHandler->SetActive(FALSE);
		}
		break;
	case IT_UNLOCKCANCERBTNEX:
		{
			if(pHandler->IsActive())
			{
				pHandler->Release();
			}
		}
		break;
	}
}
#include "ItemDropManager.h"
void CG_TargetObjectBtnFunc( LONG lId, void* p, DWORD we )
{
	switch( lId )
	{
	case CG_VIEWTARGETEQ:
		{
			cDialog *pDlg = (cDialog*)GAMEIN->GetCharacterEquipInfoDlg();
			if (!pDlg)
				return;
			if (pDlg->IsActive())
			{
				pDlg->SetActive(FALSE);
			}
			else
			{
				MSG_DWORD msg;
				msg.Category = MP_ITEM;
				msg.Protocol = MP_ITEM_QUERY_EQUI_SYN;
				msg.dwObjectID = HERO->GetID();
				msg.dwData = OBJECTMGR->GetSelectedObject()->GetID();
				NETWORK->Send(&msg, sizeof(msg));
				pDlg->SetActive(TRUE);
			}
		}
		break;
	case CG_GETTARGETINFO:
		{
			
				if (DISABLEROPDIALOG == 1)
				{
					return;
				}
				if (OBJECTMGR->GetSelectedObject()->GetObjectKind() & eObjectKind_Monster)
				{
					//CHATMGR->AddMsg(CTC_SYSMSG, "test");
					DWORD idx = 0;
					idx = ((CMonster*)OBJECTMGR->GetSelectedObject())->GetSInfo()->MonsterKind;
					ITEMDROPMGR->DevMonsterDrop(idx);
					//return;
				}
				

				
				/*if (OBJECTMGR->GetSelectedObject()->GetObjectKind() == eObjectKind_Monster)
				{
					idx = ((CMonster*)OBJECTMGR->GetSelectedObject())->GetSInfo()->MonsterKind;

					if (ITEMDROPMGR->GetKind() != idx )
					{
						if (ITEMDROPINFODLG->IsActive())
							return;
						else
							break;
					}
					else
						ITEMDROPMGR->DevMonsterDrop(idx);
				}
				else
				{
					idx = 0;
				}*/
				
			
		}
		break;
		case CG_VIEWMONEY: 
		{
			COtherCharacerMoneyDlg *pDlg = (COtherCharacerMoneyDlg*)GAMEIN->GetOtherCharacterMoneyDlg();
			if (!pDlg)
				return;
            pDlg->SetChr(OBJECTMGR->GetSelectedObject()->GetObjectName());
			if (pDlg->IsActive())
			{
				pDlg->SetActive(FALSE);
			}
			else
			{
				DWORD curtime =  MHTIMEMGR->GetNewCalcCurTime();
				if(curtime-pDlg->GetLastTime()>10000)
				{
					MSG_DWORD msg;
					msg.Category = MP_ITEM;
					msg.Protocol = MP_ITEM_QUERY_CHARACTERMONEY_SYN;
					msg.dwObjectID = HERO->GetID();
					msg.dwData = OBJECTMGR->GetSelectedObject()->GetID();
					NETWORK->Send(&msg, sizeof(msg));
                    pDlg->SetLastTime(curtime);
				}
				else
				{
                    CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(2110) );
				}
			}
		}
		break;
		case CG_VIEWADD:  
		{
           	COtherCharacterAmuletDlg *pDlg = (COtherCharacterAmuletDlg*)GAMEIN->GetOtherCharacterAmuletDlg();
			if (!pDlg)
				return;
            pDlg->SetName(OBJECTMGR->GetSelectedObject()->GetObjectName());
			if (pDlg->IsActive())
			{
				pDlg->SetActive(FALSE);
			}
			else
			{
				DWORD curtime =  MHTIMEMGR->GetNewCalcCurTime();
				if(curtime-pDlg->GetLastTime()>10000)
				{
					MSG_DWORD msg;
					msg.Category = MP_ITEM;
					msg.Protocol = MP_ITEM_QUERY_CHARACTERAMULET_SYN;
					msg.dwObjectID = HERO->GetID();
					msg.dwData = OBJECTMGR->GetSelectedObject()->GetID();
					NETWORK->Send(&msg, sizeof(msg));
                    pDlg->SetLastTime(curtime);
				}
				else
				{
                    CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(2110) );
				}
			}
		}
		break;
	}
}
void ITEMGAMBLE_DlgFunc( LONG lId, void* p, DWORD we )
{
	switch( lId )
	{
	case ITEMGAMBLE_BTN_TZ:
		{
           	CItemGambleDialog *pDlg = (CItemGambleDialog*)GAMEIN->GetItemGambleDialog();
			if(pDlg)
			{
				pDlg->SendGambleInfo();
			}
		}
		break;
	}
}
//void Safe_DlgBtnFunc(LONG lId, void * p, DWORD we)
//{
   //GAMEIN->GetSafeNumberDialog()->OnActionEvent(lId, p, we);
//}
void Safe_ICONBtnFunc(LONG lId, void * p, DWORD we)
{
   GAMEIN->GetSafeNumberIcon()->OnActionEvent(lId, p, we);
}
void Safe_IMGBtnFunc(LONG lId, void * p, DWORD we)
{
   GAMEIN->GetSafeNumberImg()->OnActionEvent(lId, p, we);
}
//void SafeLock_DlgBtnFunc(LONG lId, void * p, DWORD we)
//{
	//GAMEIN->GetSafeNumberLockDialog()->OnActionEvent(lId, p, we);
//}
//void SafeChange_DlgBtnFunc(LONG lId, void * p, DWORD we)
//{
	//GAMEIN->GetSafeNumberChangeDialog()->OnActionEvent(lId, p, we);
//}
void TOP_ICONBtnFunc(LONG lId, void * p, DWORD we)
{
   GAMEIN->GetTopIcon()->OnActionEvent(lId, p, we);
}
void FAMEGOLD_DlgFunc(LONG lId, void * p, DWORD we)
{
	GAMEIN->GetFameGoldDialog()->OnActionEvent(lId, p, we);
}
/*
void CG_CHARCONTROLBtnFunc(LONG lId, void * p, DWORD we)
{
	GAMEIN->GetCharacterControlDialog()->OnActionEvent(lId, p, we);
}*/
void SENDEQUIP_BtnFunc(LONG lId, void * p, DWORD we)
{
	GAMEIN->GetSendEquipDialog()->OnActionEvent(lId,p,we); 
}
void VIDEOCAPTUREDlg_Func(LONG lId, void * p, DWORD we)
{
	GAMEIN->GetVideoCaptureDlg()->OnActionEvent(lId,p,we); 
}
void VIDEOCAPTUREICON_Func(LONG lId, void * p, DWORD we)
{
	GAMEIN->GetVideoCaptureIcon()->OnActionEvent(lId,p,we); 
}

void CD_DlgFunc(LONG lId, void * p, DWORD we)
{
	switch(lId) 
	{
	    case CD_BTN_OK:
		{
			cEditBox * pName = (cEditBox *)WINDOWMGR->GetWindowForIDEx(CD_EDT_CODE);
			char* codeName = pName->GetEditText();
			if (strlen(codeName) > MAX_NAME_LENGTH)
			{
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(2360) );
				GAMEIN->GetCodeDialog()->SetActive(FALSE);
				return;
			}
			SEND_CHANGENAMEBASE msg;
			msg.Category = MP_ITEM;
			msg.Protocol = MP_ITEM_NPCCODE_SYN;
			msg.dwObjectID = HERO->GetID();
			msg.DBIdx = 0;
			SafeStrCpy(msg.Name, codeName, MAX_NAME_LENGTH+1);
			NETWORK->Send( &msg, sizeof(msg) );
			GAMEIN->GetCodeDialog()->SetActive(FALSE);
			OBJECTSTATEMGR->EndObjectState( HERO, eObjectState_Deal );
		}
		break;
	case CD_BTN_CANCEL:
		{
			GAMEIN->GetCodeDialog()->SetActive(FALSE);
			OBJECTSTATEMGR->EndObjectState( HERO, eObjectState_Deal );
		}
		break;
	}
}

void RANKBUTTON_DIALOGFunc(LONG lId, void * p, DWORD we)
{
	
	GAMEIN->GetRankButtonDialog()->OnActionEvent(lId, p, we);
	
}
void InsFUBENInfo_DlgFunc(LONG lId, void * p, DWORD we)
{
	
	GAMEIN->GetFBTimeDialog()->OnActionEvent(lId, p, we);
	
}
void HIDE_BuffDlgFunc(LONG lId,void *p,DWORD we)
{
	GAMEIN->GetHideBuffDlg()->OnActionEvent(lId,p,we);
}
void TB_AdPointDlgFunc(LONG lId,void *p,DWORD we)
{
	GAMEIN->GetAdPointDlg()->OnActionEvent(lId,p,we);
}
void Reconnect_DlgFunc(LONG lId,void *p,DWORD we)		//注册账号  by:胡汉三  QQ:112582793
{
	GAMEIN->GetReconnectDlg()->OnActionEvent(lId, p, we);
	//if(we & WE_BTNCLICK && lId == RECONNECT_BTN_CANCEL)
		//GAMEIN->GetReconnectDlg()->Close();	
}
void CI_RegIDDlgFunc(LONG lId,void *p,DWORD we)		//注册账号  by:胡汉三  QQ:112582793
{
	//cDialog* pDlg1 = (cDialog*)WINDOWMGR->GetWindowForID( MT_LOGINDLG );
	//if (!WINDOWMGR->IsMouseOverUsed() && pDlg1->PtInWindow(MOUSE->GetMouseX(), MOUSE->GetMouseY()))
	//if (we & WE_MOUSEOVER && lId == MT_REGBTN)
	/*if (we & WE_MOUSEOVER)
	{
		//if ()
		{
			MessageBox(0, "test", "test", MB_OK);
		}
	}
	if (we & WE_BTNCLICK)
	{
		switch (lId)
		{
			case MT_RESTPWD:
		case MT_REGBTN:		GAMEIN->GetRegID()->Show();break;
			case RESTPWD_NO:
		case REGNO:			GAMEIN->GetRegID()->Close();break;
			//case RESTPWD_OK:
		case REGOK:			GAMEIN->GetRegID()->OnActionEvent(lId,p,we);break;
	//	case MT_RESTPWD:	GAMEIN->GetRestPwdDlg()->Show();break;
	//	case RESTPWD_NO:	GAMEIN->GetRestPwdDlg()->Close();break;
		case RESTPWD_OK:	GAMEIN->GetRestPwdDlg()->OnActionEvent(lId,p,we);break;
		}
	}*/
	/*if(we & WE_BTNCLICK )
	{
		if (lId == MT_REGBTN)
			GAMEIN->GetRegID()->Show();
		if (lId == REGNO)
			GAMEIN->GetRegID()->Close();
	}*/
	if(we & WE_BTNCLICK && lId == MT_REGBTN)
		GAMEIN->GetRegID()->Show();
	if(we & WE_BTNCLICK && lId == REGNO)
		GAMEIN->GetRegID()->Close();
	if(we & WE_BTNCLICK && lId == REGOK)
		GAMEIN->GetRegID()->OnActionEvent(lId,p,we);
	if(we & WE_BTNCLICK && lId == MT_RESTPWD)
		GAMEIN->GetRestPwdDlg()->Show();
	if(we & WE_BTNCLICK && lId == RESTPWD_NO)
		GAMEIN->GetRestPwdDlg()->Close();
	if(we & WE_BTNCLICK && lId == RESTPWD_OK)
		GAMEIN->GetRestPwdDlg()->OnActionEvent(lId,p,we);
}
void RCM_DlgFunc(LONG lId, void * p, DWORD we)
{
	GAMEIN->GetCharacterRBTNDlg()->OnActionEvent(lId, p, we);
}
void InsFUBENEntranceInfo_DlgFunc(LONG lId, void * p, DWORD we)
{
	GAMEIN->GetInsFUBENEntranceInfoDlg()->OnActionEvent(lId, p, we);
}
void LuckyDraw_DlgFunc(LONG lId, void * p, DWORD we)
{
	GAMEIN->GetLuckyDrawIcon()->OnActionEvent(lId, p, we);
}
void ChangeLook_DlgFunc(LONG lId, void * p, DWORD we)
{
	GAMEIN->GetChangeLookDialog()->OnActionEvent(lId, p, we);
}
void ItemDrop_DlgFunc(LONG lId, void * p, DWORD we)
{
	GAMEIN->GetItemDropInfoDialog()->OnActionEvent(lId, p, we);
}
/*void ItemLink_DlgFunc(LONG lId, void * p, DWORD we)
{
	GAMEIN->GetSendEquipSelectDialog()->OnActionEvent(lId, p, we);
}*/
void PetMulti_DlgFunc(LONG lId, void * p, DWORD we)
{
	GAMEIN->GetPetMultiDialog()->OnActionEvent(lId, p, we);
}
void FPSDialog_Func(LONG lId, void * p, DWORD we)
{
	GAMEIN->GetFPSDialog()->OnActionEvent(lId, p, we);
}



void InsDGInfo_DlgFunc(LONG lId, void* p, DWORD we)
{
	GAMEIN->GetInsDGInfoDlg()->OnActionEvent(lId, p, we);
}

void InsDGEntranceInfo_DlgFunc(LONG lId, void* p, DWORD we)
{
	GAMEIN->GetInsDGEntranceInfoDlg()->OnActionEvent(lId, p, we);
}

void InsDGPartyMake_DlgFunc(LONG lId, void* p, DWORD we)
{
	GAMEIN->GetInsDGPartyMakeDlg()->OnActionEvent(lId, p, we);
}

void InsDGRank_DlgFunc(LONG lId, void* p, DWORD we)
{
	GAMEIN->GetInsDGRankDialog()->OnActionEvent(lId, p, we);
}
void ItemStepReinforce_DlgFunc(LONG lId, void* p, DWORD we)
{
	GAMEIN->GetItemStepReinforceDlg()->OnActionEvent(lId, p, we);
}
float SavedXX;
float SavedYY;
void LoginIDDialog_hide(LONG lId, void * p, DWORD we)
{
	cDialog* pDlg1 = (cDialog*)WINDOWMGR->GetWindowForID( MT_LOGINDLG );
	if (!pDlg1) return;
	switch(lId) 
	{
	case 1:
		{
			SavedXX = pDlg1->GetAbsX();
			SavedYY = pDlg1->GetAbsY();
			pDlg1->SetAbsXY( GAMERESRCMNGR->m_GameDesc.dispInfo.dwWidth,GAMERESRCMNGR->m_GameDesc.dispInfo.dwHeight );
		}
		break;
	case 0:
		{
			pDlg1->SetAbsXY( SavedXX,SavedYY );
		}
		break;	
	}
}
void CS_BtnChangePass(LONG lId, void* p, DWORD we)
{
	if(we & WE_BTNCLICK && lId == MT_RESTPWD)
		GAMEIN->GetRestPwdDlg()->Show();
}
void OfficialUpGradeDlg_Func(LONG lId, void* p, DWORD we)
{
	GAMEIN->GetOfficialUpGradeDlg()->CheckStatusItem();
	GAMEIN->GetOfficialUpGradeDlg()->OnActionEvent(lId, p, we);
}
//装备觉醒
#include "ItemQualityDlg.h"
void ItemQuality_DlgFunc(LONG lId, void* p, DWORD we)
{
	CItemQualityDlg* pDlg = (CItemQualityDlg*)GAMEIN->GetItemQualityDlg();

	switch (lId)
	{
	case ITEM_QUALITY_BTNOK:
	{
		pDlg->SendItemQualityMsg();
	}
	break;
	case ITEM_QUALITY_BTNCANCER:
	{
		pDlg->ReleaseExt();
		pDlg->Release(1);
		pDlg->SetActive(FALSE);
	}
	break;
	}
}
//装备觉醒
#include "ItemQualityChangeDlg.h"
void ItemQualityChange_DlgFunc(LONG lId, void* p, DWORD we)
{
	CItemQualityChangeDlg* pDlg = (CItemQualityChangeDlg*)GAMEIN->GetItemQualityChangeDlg();

	switch (lId)
	{
	case ITEM_QUALITYCHANGE_BTNOK:
	{
		pDlg->SendItemQualityMsg();
	}
	break;
	case ITEM_QUALITYCHANGE_BTNCANCER:
	{
		pDlg->Release();
		pDlg->SetActive(FALSE);
	}
	break;
	}
}
void GGD_Func(LONG lId, void* p, DWORD we)	//武器升阶值转移卷
{
	GAMEIN->GetGradeChangeDlg()->OnActionEvent(lId, p, we);
}
//Vip系统驱动函数定义
void VIP_BtnFunc(LONG lId, void* p, DWORD we)
{
	GAMEIN->GetVipDialog()->OnActionEvent(lId, p, we);
}
//自定义称号
void Customizing_DlgFunc(LONG lId, void* p, DWORD we)
{
	switch (lId)
	{
	case CH_CONTOMIZINGNAME_OKBTN:
	{
		GAMEIN->GetInventoryDialog()->SetDisable(FALSE);
		GAMEIN->GetCustomizingDlg()->CreatCustomizingNameSyn();
	}
	break;
	case CH_CONTOMIZINGNAME_CANCELBTN:
	{
		GAMEIN->GetInventoryDialog()->SetDisable(FALSE);
		GAMEIN->GetCustomizingDlg()->SetActive(FALSE);
	}
	break;
	}
}
#ifdef  _MUTIPET_
void PetMerge_DlgFunc(LONG lId, void* p, DWORD we)//刀哥  3pet
{
	GAMEIN->GetPetMixDlg()->OnActionEvent(lId, p, we);
}
#endif //  _MUTIPET_