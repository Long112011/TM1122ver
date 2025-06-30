#include "stdafx.h"
#include "cWindowManager.h"
#include "cScriptManager.h"
#include "GameResourceManager.h"
#include "../GlobalEventFunc.h"
#include "..\CharMake.h"
#include "..\MainDialog.h"
#include "..\GameIn.h"
#include "..\WindowIDEnum.h"
#include "..\ObjectManager.h"
#include "..\CharacterDialog.h"
#include "..\MugongManager.h"
#include "..\ItemManager.h"
#include "..\InventoryExDialog.h"
#include "..\MoneyDlg.h"
#include "cTabDialog.h"
#include "..\AlertDlg.h"
#include "cWindowFunc.h"
#include "../MousePointer.h"
#include "../input/cIMEWnd.h"
#include "../input/Mouse.h"
#include "cAnimationManager.h"
#include "cJackAnimationManager.h"
#include "../MiniMapDlg.h"
#include "../BigMapDlg.h"
#include "../MHMap.h"
#include "../cMsgBox.h"
#include "../cDivideBox.h"
#include "cWindowSystemFunc.h"
#include "mhFile.h"
#include "SuryunDialog.h"
#include "../MugongSuryunDialog.h"
#include "MainBarDialog.h"
#include "GuildCreateDialog.h"
#include "GuildDialog.h"
#include "GuildInviteDialog.h"
#include "GuildMarkDialog.h"
#include "GuildLevelUpDialog.h"
#include "GuildNickNameDialog.h"
#include "GuildRankDialog.h"
#include "GuildWarehouseDialog.h"
#include "PyogukDialog.h"
#include "MacroDialog.h"
#include "ChatOptionDialog.h"
#include "ChatDialog.h"
#include "OptionDialog.h"
#include "CharStateDialog.h"
#include "MenuSlotDialog.h"
#include "NoteDialog.h"
#include "FriendDialog.h"
#include "MiniFriendDialog.h"
#include "MiniNoteDialog.h"
#include "ReviveDialog.h"
#include "ExchangeDialog.h"
#include "StallKindSelectDlg.h"
#include "StreetBuyStall.h"
#include "BuyRegDialog.h"
#include "StallFindDlg.h"
#include "StreetStall.h"
#include "NpcScriptDialog.h"
#include "HelpDialog.h"
#include "DealDialog.h"
#include "QuickDialog.h"
#include "InventoryExDialog.h"
#include "UpgradeDlg.h"
#include "MixDialog.h"
#include "TitanPartsMakeDlg.h"	
#include "TitanMixDlg.h"		
#include "TitanRepairDlg.h"
#include "TitanRecallDlg.h"
#include "TitanUpgradeDlg.h"	
#include "TitanBreakDlg.h"		
#include "TitanGuageDlg.h"		
#include "TitanRegisterDlg.h"	
#include "TitanDissolutionDlg.h"	
#include "TitanChangePreView.h"
#include "TitanInventoryDlg.h"
#include "ReinforceDlg.h"
#include "DissloveDlg.h"
#include "WantNpcDialog.h"
#include "WantRegistDialog.h"
#include "QuestTotalDialog.h"
#include "QuestDialog.h"
#include "WantedDialog.h"
#include "JournalDialog.h"
#include "PartyCreateDlg.h"
#include "PartyInviteDlg.h"
#include "BailDialog.h"
#include "MPRegistDialog.h"
#include "MPMissionDialog.h"
#include "MPNoticeDialog.h"
#include "MPGuageDialog.h"
#include "DissolutionDialog.h"
#include "EventNotifyDialog.h"

#include "../VSyncDialog.h"

#include "ItemShopDialog.h"
#include "MoveDialog.h"
#include "PointSaveDialog.h"
#include "./Input/UserInput.h"
#include "MouseCursor.h"
#include "StatusIconDlg.h"
#include "GuildFieldWarDialog.h"
#include "SkillPointRedist.h"
#include "SkillPointNotify.h"
#include "PartyWarDialog.h"
#include "ShoutDialog.h"
#include "ShoutchatDialog.h"
#include "ChaseDialog.h"
#include "ChaseinputDialog.h"
#include "NameChangeDialog.h"
#include "NameChangeNotifyDlg.h"
#include "GTRegistDialog.h"
#include "GTRegistcancelDialog.h"
#include "GTStandingDialog.h"
#include "GTBattleListDialog.h"
#include "GTScoreInfoDialog.h"
#include "SeigeWarDialog.h"
#include "CharChangeDlg.h"
#include "SealDialog.h"
#include "ChangeJobDialog.h"
#include "GuageDialog.h"
#include "ReinforceResetDlg.h"
#include "RareCreateDialog.h"
#include "PetManager.h"
#include "PetStateDlg.h"
#include "PetStateMiniDlg.h"
#include "PetInventoryDlg.h"
#include "PetUpgradeDialog.h"
#include "PetRevivalDialog.h"
#include "ReinforceDataGuideDlg.h"
#include "TipBrowserDlg.h"
#include "KeySettingTipDlg.h"
#include "cIMEEx.h"
#include "GuildNoteDlg.h"
#include "UnionNoteDlg.h"
#include "GuildNoticeDlg.h"
#include "SkillOptionChangeDlg.h"
#include "SkillOptionClearDlg.h"
#include "guildjoindialog.h"
#ifdef _HACK_SHIELD_
#include "HackShieldManager.h"
#endif
#include "ScreenShotDlg.h"
#include "GuildPlusTimeDialog.h"
#include "GuildMunhaDialog.h"
#include "GuildTraineeDialog.h"
#include "SurvivalModeManager.h"
#include "SurvivalCountDialog.h"
#include "UniqueItemCurseCancellationDlg.h"
#include "UniqueItemMixDlg.h"
#include "MHCamera.h"
#include "SOSDialog.h"
#include "UniqueItemMixProgressBarDlg.h"	 
#include "TitanMixProgressBarDlg.h"			 
#include "TitanPartsProgressBarDlg.h"		
#include "SkinSelectDialog.h"	
#include "CostumeSkinSelectDialog.h"	
#include "SkillPointResetDlg.h"
#include "NumberPadDialog.h"	
#include "StreetStallItemViewDlg.h"
#include "AutoNoteDlg.h"	
#include "AutoAnswerDlg.h"
#include "PartyMatchingDlg.h"
#include "FortWarDialog.h"
#include "GoldShopDialog.h"   
#include "GoldShopBuyDialog.h"
#include "MiniMarryDialog.h"  
#include "MiniBaishiDialog.h" 
#include "ControlDialog.h"    
#include "RankButtonDialog.h" 
#include "TopDialog.h"          
#include "TopMunpDialog.h"      
#include "LuckerDialog.h"     
#include "MoneyPacketDialog.h" 
#include "OtherCharacterMoneyDlg.h" 
#include "OtherCharacterAmuletDlg.h" 
#include "ItemStoneDkDialog.h"     
#include "ItemStoneXqDialog.h"
#include "ItemStepReinforceDialog.h"
#include "OtherCharacterInfo.h" 
#include "OtherCharacterEquip.h"  
#include "../cDivideBoxEx.h"        
#include "ItemLockManager.h"         
#include "ItemUnLockManager.h"       
#include "ItemGambleManager.h"        
#include "SafeNumberIcon.h"            
#include "SafeNumberImg.h"            
#include "TopIcon.h"                       
#include "FBTimeDialog.h"			     
#include "FameGoldDialog.h"			  
#include "SendEquipDialog.h"           
#include "SendEquipSelectDialog.h" 
#include "VideoCaptureDlg.h"         
#include "VideoCaptureIcon.h"        
#include "CodeDialog.h"
#include "EventPlayTimeDialog.h"
#include "InsFUBENMissionString.h"
#include "InsDGEntranceInfoDlg.h"
#include "JackFlashRoar.h"
#include "HideBuff.h"
#include "AdPointDlg.h"
#include "CharacterRBTN.h"
#include "LuckyDrawIcon.h"
#include "BloodStormIcon.h"
#include "ChangeLookDialog.h"
#include "ItemDropInfoDialog.h"
#include ".\petmultidlg.h"

#include "FPSDialog.h"

#include "JackRebornInfo.h"


#include "InstancesDungeon/InsDGInfoDialog.h"
#include "InstancesDungeon/InsDGMissionInfoDialog.h"
#include "InstancesDungeon/InsDGEntranceInfoDialog.h"
#include "InstancesDungeon/InsDGPartyMakeDialog.h"
#include "InstancesDungeon/InsDGRankDialog.h"
#include "InstancesDungeon/InsDGRankInfoDialog.h"

#include "cHousingWebDlg.h"

#include "FadeDlg.h"
#include "TopDungeon.h"
#include "CharacterPVPDialog.h"
#include "NewUpGrareAlexXDlg.h"

extern HWND _g_hWnd;
extern BOOL m_SafeIconShow;
#define FONTPATH ".\\Image\\InterfaceScript\\gamefont.ttc"
extern BOOL g_bDisplayFPS;
extern double CurFPS;
bool g_bDisplayFPSWindownMgrBool;












GLOBALTON(cWindowManager);

#define _DEBUGTICK
char DebugTickTemp[128];
void cWindowManager::CreateGameIn()
{
	g_bDisplayFPSWindownMgrBool = FALSE;
#ifdef	_DEBUGTICK
	DWORD	dwStartTick = GetTickCount();
#endif
	CreateMainBarDlg();
	CreateMonsterGuageDlg();
	CreateCharDlg();
	CreateMugongSuryunDlg();
	CreateInventoryDlg();
	CreateRecvExchangeDlg();
	CreateHeroGuage();
	CreateDealDlg();
	CreateChatDlg();
	CreateStallKindSelectDlg();
	CreateStreetBuyStallDlg();
	CreateBuyRegDlg();
	CreateStallFindDlg();
	CreateStreetStallDlg();
	CreateNpcScriptDlg();
	CreateHelpDlg();
	CreateQuickDlg();
	CreatePartyDlg();
	CreatePartyCreateDlg();
	CreatePartyInviteDlg();
	CreateGuildCreateDlg();
	CreateGuildNickNameDlg();
	CreateGuildRankDlg();
	CreateGuildDlg();
	CreateGuildInviteDlg();
	CreateGuildMarkDlg();
	CreateGuildLevelupDlg();
	CreateGuildWarehouseDlg();
	CreateMiniMapDlg();
	CreatePyogukDlg();
	CreateFriendDlg();
	CreateMiniFriendDlg();
	CreateNoteDlg();
	CreateMiniNoteDlg();
	CreateReviveDlg();
	CreateWantNpcDlg();
	CreateWantRegistDlg();
	CreateQuestTotalDlg();
	CreateMPRegistDlg();
	CreateOptionDlg();
	CreateEventNotifyDlg();
	CreateVsyncNotifyDlg();

	CreateGoldShopDialog();
	CreateGoldShopBuyDialog();
	CreateMarryMiniDialog();
	CreateMiniShiTuDialog();
	CreateControlDialog();
	CreateBillboardDialog();
	CreateBillboardMunpDialog();
	CreateLuckerDialog();
	CreateMoneyPacketDialog();
	CreateOtherCharacterMoneyDlg();
	CreateOtherCharacterAmuletDlg();
	CreateItemStoneDkDialog();
	CreateItemStoneXqDialog();
	CreateItemStepReinforceDialog();
	CreateOtherCharacterInfo();
	CreateOtherCharacterEquip();
	CreateItemLockDlg();
	CreateItemUnLockDlg();
	CreateItemGambleDlg();
	CreateExitDlg();
	CreateExchangeDlg();
	CreateUpgradeDlg();
	CreateMixDlg();
	CreateTitanPartsProgressBarDlg();
	CreateTitanPartsMakeDlg();
	CreateTitanMixProgressBarDlg();
	CreateTitanMixDlg();
	CreateTitanRepairDlg();
	CreateTitanRecallDlg();
	CreateTitanUpgradeDlg();
	CreateTitanBreakDlg();
	CreateTitanChangePreViewDlg();
	CreateTitanInventoryDlg();
	CreateTitanGuageDlg();
	CreateTitanRegisterDlg();
	CreateTitanDissolution();
	CreateReinforceDlg();
	CreateReinforceDataGuideDlg();
	CreateItemShopDlg();
	CreateMoveDlg();
	CreatePointSaveDlg();
	CreateBailDlg();
	CreateDissolutionDlg();
	CreateGuildFieldWarDlg();
	CreateSkPointDlg();
	CreateSkPointNotifyDlg();
	CreatePartyWarDlg();
	CreateShoutDlg();
	CreateShoutchatDlg();
	CreateChaseDlg();
	CreateChaseinputDlg();
	CreateNameChangeDlg();
	CreateNameChangeNotifyDlg();
	CreateGTRegistDlg();
	CreateGTRegistcancelDlg();
	CreateGTStandingDlg();
	CreateGTBattleListDlg();
	CreateGTScoreInfoDlg();
	CreateSeigeWarDlg();
	CreateCharChangeDlg();
	CreateItemSealDlg();
	CreateChangeJobDlg();
	CreateReinforceResetDlg();
	CreateRareCreateDlg();
	CreatePetStateDlg();
	CreatePetStateMiniDlg();
	CreatePetInvenDlg();
	CreatePetUpgradeDlg();
	CreatePetRevivalDlg();
	CreatePetMultiDlg();
	CreateTipBrowserDlgDlg();
	CreateKeySettingTipDlg();
	CreateGuildNoteDlg();
	CreateUnionNoteDlg();
	CreateGuildNoticeDlg();
	CreateSkillOptionChangeDlg();
	CreateSkillOptionClearDlg();
	CreateGuildInvitationKindSelectionDlg();
	CreateGuildMunhaDlg();
	CreateGuildTraineeDlg();
	ScreenShotDlg();
	CreateSurvivalCountDlg();
	CreateUniqueItemMixProgressBarDlg();
	CreateUniqueItemCurseCancellationDlg();
	CreateUniqueItemMixDlg();
	CreateSOSDlg();
	CreateSkinSelectDlg();
	CreateCostumeSkinSelectDlg();
	CreateSkillPointResetDlg();
	CreateStreetStallItemViewDlg();
	CreateChannelDlg();
	CreatePartyMatchingDlg();
	CreateSafeNumberIcon();
	CreateSafeNumberImg();
	CreateTopIcon();
	CreateFBTimeDialog();
	CreateFameGoldDialog();
	CreateSendEquipDialog();
	CreateSendEquipSelectDialog();
	CreateVideoCaptureDialog();
	CreateVideoCaptureIcon();
	CreateGuildWarWarDlg();
	CreateCodeDialog();
	CreateRankButtonDialog();
	CreateLoginEventDialog();
	CreateDungeonScriptDlg();
	CreateInsFUBENEntranceInfoDlg();
	CreateJackFlashDlg();
	CreateHideBuffDlg();
	CreateAdPointDlg();
	CreateReconnectDlg();
	CreateCharacterRBTN();
	CreateLuckyDrawIcon();
	CreateBloodStormIcon();
	CreateItemDropDlg();
	CreateFPSDialog();

	CreateJackRebornInfo();

	CreateInsDGInfoDlg();	//2008. 7. 24. CBH - 牢带 沥焊 促捞倔肺弊
	CreateInsDGMissionInfoDlg(); //2008. 7. 25. CBH - 牢带 固记 沥焊 促捞倔肺弊
	CreateInsDGEntranceInfoDlg();	// 2008. 7. 29. CBH - 牢带 涝厘 炼扒 沥焊 促捞倔肺弊
	CreateInsDGPartyMakeDlg();		//2008. 8. 5. CBH - 牢带 涝厘矫 颇萍啊 绝阑锭 剁快绰 促捞倔肺弊	
	CreateInsDGRankDlg();

	CreateHousingWebDlg();

	CreateFadeDlg();

	CreateDungeonRankingDlg();
	CreateCharPvPDlg();
	CreateNewUpGrareAlexXDlg();

#ifdef	_DEBUGTICK
	DWORD	dwEndTick = GetTickCount();
	DWORD	dwFuncTick = dwEndTick - dwStartTick;

	char	buf[1024];
	char	buf2[1024];

	sprintf(buf, "Tick:%d\n", dwFuncTick);

	DWORD	miliseconds = dwFuncTick % 1000;
	dwFuncTick /= 1000;
	DWORD	seconds = dwFuncTick % 60;
	dwFuncTick /= 60;
	DWORD	minutes = dwFuncTick % 60;
	dwFuncTick /= 60;
	DWORD	hours = dwFuncTick;

	sprintf(buf2, "[MAP LOAD SEC]\n\n\n%d:%02d:%02d.%03d\n%s\n", hours, minutes, seconds, miliseconds, buf);
	//sprintf(buf, "%s() spent %d Ticks\n", __FUNCTION__, dwFuncTick);

	strcpy(DebugTickTemp, buf2);
	//RECT rect;
	//SetRect(&rect, 30, 360, 500, 450);
	//CFONT_OBJ->RenderFont(0, buf, strlen(buf), &rect, 0xffffffff);
	//OutputDebugString(buf);
#endif
}
cWindowManager::cWindowManager()
{
	m_pScriptManager		= NULL;
	m_pWindowList			= NULL;
	m_pIconDlgListRef		= NULL;
	m_pEditBoxListRef		= NULL;
	m_pDestroyWindowRef		= NULL;
	m_pScreenTextList		= NULL;
	m_OldDragFlag			= FALSE;
	m_pMousePointWindow		= NULL;
	m_pGuageWindow			= NULL;
	m_pMonsterGuageWindow	= NULL;
	m_fComposition			= FALSE;
	cbDragnDropProcess		= NULL;
	m_pFocusedEdit			= NULL;
	m_id					= -1;
	m_pDragDialog			= NULL;
	m_bHideInterface		= FALSE;
	m_pRelationTip			= NULL;
	m_pRelationTip1			= NULL;
	m_pSendItemInfo			= NULL;

	m_pStaticToolTip = NULL;
}
void cWindowManager::Release2()
{
	DestroyWindowAll();
	
	SAFE_DELETE_ARRAY(m_pScriptManager);
	SAFE_DELETE_ARRAY(m_pIconDlgListRef);
	SAFE_DELETE_ARRAY(m_pEditBoxListRef);
	SAFE_DELETE_ARRAY(m_pScreenTextList);
	SAFE_DELETE_ARRAY(m_pPlayerChatTooltip);
	SAFE_DELETE_ARRAY(m_pWindowList);
	SAFE_DELETE_ARRAY(m_pDestroyWindowRef);
	SAFE_DELETE_ARRAY(m_pMousePointWindow);
	SAFE_DELETE_ARRAY(m_pGuageWindow);
	SAFE_DELETE_ARRAY(m_pMonsterGuageWindow);

	Init2();
}
void cWindowManager::Init2()
{
	
	m_pScriptManager = new cScriptManager;
	m_pWindowList = new cPtrList;
	m_pEditBoxListRef = new cPtrList;
	m_pIconDlgListRef = new cPtrList;
	m_pDestroyWindowRef = new cPtrList;
	m_pScreenTextList = new cPtrList;
	m_pPlayerChatTooltip = new cPtrList;
	m_pDestroyWindowRef = new cPtrList;
	m_pScriptManager->InitParseTypeData();
	cMsgBox::InitMsgBox();
	
}



















cWindowManager::~cWindowManager()
{
}
void cWindowManager::Release()
{
	DestroyWindowAll();
	CFONT_OBJ->Release();
	RemoveFontResourceEx(FONTPATH, FR_PRIVATE, 0);
	SAFE_DELETE(m_pScriptManager);
	SAFE_DELETE(m_pIconDlgListRef);
	SAFE_DELETE(m_pEditBoxListRef);
	SAFE_DELETE(m_pScreenTextList);
	SAFE_DELETE(m_pPlayerChatTooltip);
	SAFE_DELETE(m_pWindowList);
	SAFE_DELETE(m_pDestroyWindowRef);
	SAFE_DELETE(m_pMousePointWindow);
	SAFE_DELETE(m_pGuageWindow);
	SAFE_DELETE(m_pMonsterGuageWindow);
}
void cWindowManager::Init()
{
	m_pScriptManager = new cScriptManager;
	m_pWindowList = new cPtrList;
	m_pEditBoxListRef = new cPtrList;
	m_pIconDlgListRef = new cPtrList;
	m_pDestroyWindowRef = new cPtrList;
	m_pScreenTextList = new cPtrList;
	m_pPlayerChatTooltip = new cPtrList;
	m_pScriptManager->InitParseTypeData();
	AddFontResourceEx("./Resource/Client/gamefont.ttc", FR_PRIVATE, 0);
	LOGFONT	font;
	extern HWND _g_hWnd;
	//font.lfHeight = -MulDiv(9, GetDeviceCaps(GetDC( _g_hWnd ), LOGPIXELSY), 72);
	font.lfHeight = -MulDiv(9, 96, 72);	//直接指定默认96 DPI
	//font.lfHeight = -12;

	font.lfWidth = -font.lfHeight / 2;
	font.lfEscapement = 0;
	font.lfOrientation = 0;
	font.lfWeight = 400;
	font.lfItalic = 0;
	font.lfUnderline = 0;
	font.lfStrikeOut = 0;

	// auto check the charset 2014-05-10!  CHINESEBIG5_CHARSET  GB2312_CHARSET 
#ifdef _BIG5SYSTEM_
	font.lfCharSet = CHINESEBIG5_CHARSET;
#else
	font.lfCharSet = GB2312_CHARSET;	//默认设定作简体中文,支援双语言
#endif
	font.lfOutPrecision = 0;
	font.lfQuality = PROOF_QUALITY;
	font.lfPitchAndFamily = 0;
	lstrcpy(font.lfFaceName, "MS Sans Serif");
	CFONT_OBJ->CreateFontObject(&font, cFont::FONT0);

	//font.lfHeight = -MulDiv(9,GetDeviceCaps(GetDC(_g_hWnd),LOGPIXELSY),72);
	//font.lfHeight = -12;
	font.lfHeight = -14;
	font.lfWeight = 500;
	font.lfWidth = -font.lfHeight / 2;
	font.lfQuality = PROOF_QUALITY;
	font.lfPitchAndFamily = FIXED_PITCH | FF_SWISS;
	lstrcpy(font.lfFaceName, "MS Sans Serif");
	CFONT_OBJ->CreateFontObject(&font, cFont::FONT5);

	//font.lfHeight = -MulDiv(9,GetDeviceCaps(GetDC(_g_hWnd),LOGPIXELSY),72);
	//font.lfHeight = -9;
	font.lfHeight = -MulDiv(9, 96, 72);	//直接指定默认96 DPI
	font.lfWidth = -font.lfHeight / 2;
	font.lfUnderline = 1;
	font.lfWeight = 600;
	//font.lfQuality = PROOF_QUALITY;
	font.lfWeight = PROOF_QUALITY;
	lstrcpy(font.lfFaceName, "MingLiU");
	CFONT_OBJ->CreateFontObject(&font, cFont::FONT4);

	font.lfUnderline = 0;
	font.lfHeight = -14;
	font.lfWidth = -font.lfHeight / 2;
	font.lfQuality = PROOF_QUALITY;
	lstrcpy(font.lfFaceName, "MS Sans Serif");
	CFONT_OBJ->CreateFontObject(&font, cFont::FONT1);

	/*	font.lfUnderline = 0;
		font.lfHeight = -14;
		font.lfWidth = -font.lfHeight / 2;
		font.lfQuality = PROOF_QUALITY;
		lstrcpy(font.lfFaceName, "宋体");
		CFONT_OBJ->CreateFontObject(&font, cFont::FONT2);*/

	font.lfUnderline = 0;
	font.lfHeight = -14;
	font.lfWeight = FW_BOLD;
	font.lfWidth = -font.lfHeight / 2;
	lstrcpy(font.lfFaceName, "MS Sans Serif");
	CFONT_OBJ->CreateFontObject(&font, cFont::FONT2);

	font.lfHeight = -35;
	font.lfWeight = FW_HEAVY;
	font.lfWidth = -font.lfHeight / 2;
	font.lfQuality = PROOF_QUALITY;
	font.lfPitchAndFamily = FIXED_PITCH | FF_SWISS;
	lstrcpy(font.lfFaceName, "MS Sans Serif");
	CFONT_OBJ->CreateFontObject(&font, cFont::FONT3);

	font.lfHeight = -14;
	font.lfWeight = FW_HEAVY;
	font.lfWidth = -font.lfHeight / 2;
	font.lfQuality = PROOF_QUALITY;
	font.lfPitchAndFamily = FIXED_PITCH | FF_SWISS;
	lstrcpy(font.lfFaceName, "MS Sans Serif");
	CFONT_OBJ->CreateFontObject(&font, cFont::FONT6);

	font.lfHeight = -14;
	font.lfWeight = 500;
	font.lfWidth = -font.lfHeight / 2;
	font.lfQuality = PROOF_QUALITY;
	font.lfPitchAndFamily = FIXED_PITCH | FF_SWISS;
	lstrcpy(font.lfFaceName, "MS Sans Serif");
	CFONT_OBJ->CreateFontObject(&font, cFont::FONT7);

	font.lfHeight = -14;
	font.lfWeight = FW_HEAVY;
	font.lfWidth = -font.lfHeight / 2;
	font.lfQuality = PROOF_QUALITY;
	font.lfPitchAndFamily = FIXED_PITCH | FF_SWISS;
	lstrcpy(font.lfFaceName, "MS Sans Serif");
	CFONT_OBJ->CreateFontObject(&font, cFont::FONT8);

	/*	font.lfHeight = -14;
		font.lfWeight = FW_NORMAL;
		font.lfWidth = -font.lfHeight / 2;
		font.lfQuality = PROOF_QUALITY;
		font.lfPitchAndFamily = FIXED_PITCH | FF_SWISS;
		lstrcpy(font.lfFaceName, "宋体");
		CFONT_OBJ->CreateFontObject(&font, cFont::FONT9);	//过图提示公屏字体 2020-07-03*/

	font.lfHeight = -MulDiv(9, GetDeviceCaps(GetDC(_g_hWnd), LOGPIXELSY), 72);
	font.lfWeight = 550;
	font.lfWidth = -font.lfHeight / 2;
	font.lfQuality = PROOF_QUALITY;
	font.lfPitchAndFamily = FIXED_PITCH | FF_SWISS;
	lstrcpy(font.lfFaceName, "MS Sans Serif");
	CFONT_OBJ->CreateFontObject(&font, cFont::FONT9);

	font.lfHeight = -14;
	font.lfWeight = FW_NORMAL;
	font.lfWidth = -font.lfHeight / 2;
	font.lfQuality = PROOF_QUALITY;
	font.lfPitchAndFamily = FIXED_PITCH | FF_SWISS;
	lstrcpy(font.lfFaceName, "MS Sans Serif");
	CFONT_OBJ->CreateFontObject(&font, cFont::FONT10);

	font.lfUnderline = 0;	//新增字体(11) 测试
	font.lfHeight = -14;
	font.lfWidth = -font.lfHeight / 2;
	font.lfQuality = PROOF_QUALITY;
	lstrcpy(font.lfFaceName, "MS Sans Serif");

	CFONT_OBJ->CreateFontObject(&font, cFont::FONT11);

	font.lfUnderline = 0;
	font.lfHeight = -10;
	font.lfWeight = FW_BOLD;
	font.lfWidth = -font.lfHeight / 2;
	lstrcpy(font.lfFaceName, "MS Sans Serif");
	CFONT_OBJ->CreateFontObject(&font, cFont::FONT12);

	font.lfHeight = -14;
	font.lfWeight = FW_BOLD;
	font.lfWidth = -font.lfHeight / 2;
	font.lfQuality = PROOF_QUALITY;
	font.lfPitchAndFamily = FIXED_PITCH | FF_SWISS;
	lstrcpy(font.lfFaceName, "MS Sans Serif");
	CFONT_OBJ->CreateFontObject(&font, cFont::FONT13);

	font.lfHeight = -MulDiv(9, GetDeviceCaps(GetDC(_g_hWnd), LOGPIXELSY), 72);
	font.lfWeight = 900;
	font.lfWidth = 0;
	font.lfQuality = CLEARTYPE_QUALITY;
	font.lfPitchAndFamily = FIXED_PITCH | FF_SWISS;
	lstrcpy(font.lfFaceName, "MingLiU");

	CFONT_OBJ->CreateFontObject(&font, cFont::FONT14);

	font.lfHeight = -MulDiv(11, GetDeviceCaps(GetDC(_g_hWnd), LOGPIXELSY), 68);	//GMNotifyManagerそ驰杠砰
	font.lfWidth = -font.lfHeight / 2;
	font.lfWeight = 400;
	font.lfOutPrecision = 0;
	font.lfQuality = PROOF_QUALITY;
	font.lfPitchAndFamily = 0;
	lstrcpy(font.lfFaceName, "MS Sans Serif");	//灿砰
	CFONT_OBJ->CreateFontObject(&font, cFont::FONT15);

	font.lfUnderline = 0;
	font.lfHeight = -11;
	font.lfWeight = FW_HEAVY;
	font.lfWidth = -font.lfHeight / 2;
	font.lfPitchAndFamily = FIXED_PITCH | FF_SWISS;
	lstrcpy(font.lfFaceName, "MS Sans Serif");
	CFONT_OBJ->CreateFontObject(&font, cFont::FONT16);

	font.lfUnderline = 0;
	font.lfHeight = -18;
	font.lfWeight = FW_HEAVY;
	font.lfWidth = -font.lfHeight / 2;
	font.lfPitchAndFamily = FIXED_PITCH | FF_SWISS;
	lstrcpy(font.lfFaceName, "MS Sans Serif");
	CFONT_OBJ->CreateFontObject(&font, cFont::FONT17);

	font.lfHeight = -MulDiv(11, GetDeviceCaps(GetDC(_g_hWnd), LOGPIXELSY), 68);	//CH材
	font.lfWidth = -font.lfHeight / 2;
	font.lfWeight = 551;
	font.lfOutPrecision = 0;
	font.lfQuality = PROOF_QUALITY;
	font.lfPitchAndFamily = 0;
	lstrcpy(font.lfFaceName, "MS Sans Serif");	//灿砰
	CFONT_OBJ->CreateFontObject(&font, cFont::FONT18);

	font.lfHeight = -MulDiv(35, GetDeviceCaps(GetDC(_g_hWnd), LOGPIXELSY), 72);
	font.lfWidth = -font.lfHeight / 2;
	lstrcpy(font.lfFaceName, "MS Sans Serif");
	CFONT_OBJ->CreateFontObject(&font, cFont::FONT19);

	font.lfHeight = -30;
	font.lfWeight = FW_HEAVY;
	font.lfWidth = -font.lfHeight / 2;
	font.lfQuality = PROOF_QUALITY;
	font.lfPitchAndFamily = FIXED_PITCH | FF_SWISS;
	lstrcpy(font.lfFaceName, "MS Sans Serif");
	CFONT_OBJ->CreateFontObject(&font, cFont::FONT20);

	cMsgBox::InitMsgBox();
}

BOOL cWindowManager::CreateFontObjectToFile()
{
	char line[256] = { 0, };
	CMHFile fp;
	if (!fp.Init("./Image/Font.bin", "rb"))
		return FALSE;
	LOGFONT	font;
	int		nIdx = 0;
	font.lfEscapement = 0;
	font.lfOrientation = 0;
	//font.lfStrikeOut = 0;
	font.lfCharSet = DEFAULT_CHARSET;
	font.lfOutPrecision = 0;
	font.lfClipPrecision = 0;
	font.lfQuality = PROOF_QUALITY;
	font.lfPitchAndFamily = 0;
	while (fp.IsEOF() == FALSE)
	{
		fp.GetString(line);
		if (line[0] == '@')
		{
			fp.GetLineX(line, 256);
			continue;
		}
		if (strcmp(line, "#FONTCHARSET") == 0)
			font.lfCharSet = fp.GetInt();
		else if (strcmp(line, "#FONT") == 0)
		{
			if (nIdx == FONTMAX - 1)
			{
				MessageBox(0, "More than it should detected!...", fp.GetFileName(), MB_OK);
				continue;
			}

			nIdx = fp.GetInt();
			fp.GetStringInQuotation(line);
			lstrcpy(font.lfFaceName, line);
			int fontsize = fp.GetInt();
			font.lfHeight = -MulDiv(fontsize, 96, 72);
			font.lfWidth = -font.lfHeight / 2;
			font.lfWeight = fp.GetInt();
			font.lfUnderline = fp.GetInt();
			font.lfItalic = fp.GetInt();
			font.lfStrikeOut = fp.GetByte();
			CFONT_OBJ->CreateFontObject(&font, nIdx);
		}

	}
	if (nIdx < FONTMAX - 1)
	{
		MessageBox(0, "Font line tak complete \nPlease check!...", fp.GetFileName(), MB_OK);
	}
	fp.Release();
	return TRUE;
}

//BOOL cWindowManager::CreateFontObjectToFile()
//{
//	char line[256] = { 0, };
//	CMHFile fp;
//	if (!fp.Init("./Image/Font.bin", "rb"))
//		return FALSE;
//	LOGFONT	font;
//	int		nIdx = 0;
//	font.lfEscapement = 0;
//	font.lfOrientation = 0;
//	//font.lfStrikeOut = 0;
//	font.lfCharSet = DEFAULT_CHARSET;
//	font.lfOutPrecision =  0;
//	font.lfClipPrecision = 0;
//	font.lfQuality = PROOF_QUALITY;
//	font.lfPitchAndFamily =  0;
//	while (fp.IsEOF() == FALSE)
//	{
//		fp.GetString(line);
//		if (line[0] == '@')
//		{
//			fp.GetLineX(line, 256);
//			continue;
//		}
//		if (strcmp(line, "#FONTCHARSET") == 0)
//			font.lfCharSet = fp.GetInt();
//		else if (strcmp(line, "#FONT") == 0)
//		{
//			if (nIdx == FONTMAX - 1)
//			{
//				MessageBox(0, "More than it should detected!...", fp.GetFileName(), MB_OK);
//				continue;
//			}
//			
//			nIdx = fp.GetInt();
//			fp.GetStringInQuotation(line);
//			lstrcpy(font.lfFaceName, line);
//			int fontsize = fp.GetInt();
//			font.lfHeight = -MulDiv(fontsize, 96, 72);
//			font.lfWidth = -font.lfHeight / 2;
//			font.lfWeight = fp.GetInt();
//			font.lfUnderline = fp.GetInt();
//			font.lfItalic = fp.GetInt();
//			font.lfStrikeOut = fp.GetByte();
//			CFONT_OBJ->CreateFontObject(&font, nIdx);
//		}
//		 
//	}
//	if (nIdx < FONTMAX - 1)
//	{
//		MessageBox(0, "Font line tak complete \nPlease check!...", fp.GetFileName(), MB_OK);
//	}
//	fp.Release();
//	return TRUE;
//}
DWORD cWindowManager::GetScreenX()
{
	return GAMERESRCMNGR->m_GameDesc.dispInfo.dwWidth;
	//return GAMERESRCMNGR->GetPrintScressInfo(GAMERESRCMNGR->GetResolutionIndex())->x;
}
DWORD cWindowManager::GetScreenY()
{
	return GAMERESRCMNGR->m_GameDesc.dispInfo.dwHeight;
	//return GAMERESRCMNGR->GetPrintScressInfo(GAMERESRCMNGR->GetResolutionIndex())->y;
}
void cWindowManager::CreatMousePoint()
{
}
void cWindowManager::CreateMainBarDlg()
{
	cWindow * window = GetDlgInfoFromFile("./image/InterfaceScript/15.bin", "rb");
	//VECTOR2 Pos1;
	//Pos1.x = (float)GAMERESRCMNGR->m_GameDesc.dispInfo.dwWidth - window->GetWidth();
	//Pos1.y = (float)GAMERESRCMNGR->m_GameDesc.dispInfo.dwHeight - window->GetHeight();
	//window->SetAbsXY(Pos1.x, Pos1.y);
	AddWindow(window);
	GAMEIN->SetMainInterfaceDialog((CMainBarDialog *)window);
	GAMEIN->GetMainInterfaceDialog()->InitBar();
}
void cWindowManager::CreatePartyWarDlg()
{
	//cWindow* window = NULL;
	cWindow* window = GetDlgInfoFromFile("./image/InterfaceScript/PartyWarDlg.bin", "rb");
	AddWindow(window);
	GAMEIN->SetPartyWarDlg((CPartyWarDialog*)window);
	GAMEIN->GetPartyWarDlg()->Linking();
}
void cWindowManager::CreateGuildFieldWarDlg()
{
	cWindow* window = NULL;
	window = GetDlgInfoFromFile("./image/InterfaceScript/GFWarDeclare.bin", "rb");
	AddWindow(window);
	GAMEIN->SetGFWarDeclareDlg((CGFWarDeclareDlg*)window);
	GAMEIN->GetGFWarDeclareDlg()->Linking();
	window = GetDlgInfoFromFile("./image/InterfaceScript/GFWarResult.bin", "rb");
	AddWindow(window);
	GAMEIN->SetGFWarResultDlg((CGFWarResultDlg*)window);
	GAMEIN->GetGFWarResultDlg()->Linking();
	window = GetDlgInfoFromFile("./image/InterfaceScript/GFWarInfo.bin", "rb");
	AddWindow(window);
	GAMEIN->SetGFWarInfoDlg((CGFWarInfoDlg*)window);
	GAMEIN->GetGFWarInfoDlg()->Linking();
	window = GetDlgInfoFromFile("./image/InterfaceScript/GuildWarInfo.bin", "rb");
	VECTOR2 Pos1;
	Pos1.x = ((float)GET_MAINWIN_W / 2 - window->GetWidth() / 2);
	Pos1.y = ((float)GET_MAINWIN_H / 2 - window->GetHeight() / 2);
	window->SetAbsXY(Pos1.x, Pos1.y);
	AddWindow(window);
	GAMEIN->SetGuildWarInfoDlg((CGuildWarInfoDlg*)window);
	GAMEIN->GetGuildWarInfoDlg()->Linking();

	window = GetDlgInfoFromFile("./image/InterfaceScript/GuildPlusTime.bin", "rb");
	AddWindow(window);
	GAMEIN->SetGuildPlusTimeDlg((CGuildPlusTimeDlg*)window);
	GAMEIN->GetGuildPlusTimeDlg()->Linking();

}
void cWindowManager::CreateDissolveDlg()
{
	cWindow * window = GetDlgInfoFromFile("./image/InterfaceScript/58.bin", "rb");
	AddWindow(window);
	GAMEIN->SetDissolveDialog((CDissloveDlg *)window);
	GAMEIN->GetDissolveDialog()->Linking();
}
void cWindowManager::CreateUpgradeDlg()
{
	cWindow * window = GetDlgInfoFromFile("./image/InterfaceScript/55.bin", "rb");
	AddWindow(window);
	GAMEIN->SetUpgradeDialog((CUpgradeDlg *)window);
	GAMEIN->GetUpgradeDialog()->Linking();
}
void cWindowManager::CreateMixDlg()
{
	cWindow * window = GetDlgInfoFromFile("./image/InterfaceScript/56.bin", "rb");
	AddWindow(window);
	GAMEIN->SetMixDialog((CMixDialog *)window);
	GAMEIN->GetMixDialog()->Linking();
}
void cWindowManager::CreateTitanPartsMakeDlg()
{
	cWindow * window = GetDlgInfoFromFile("./image/InterfaceScript/60.bin", "rb");
	AddWindow(window);
	GAMEIN->SetTitanPartsMakeDlg((CTitanPartsMakeDlg *)window);
	GAMEIN->GetTitanPartsMakeDlg()->Linking();
}
void cWindowManager::CreateTitanMixDlg()
{
	cWindow * window = GetDlgInfoFromFile("./image/InterfaceScript/Titanmix.bin", "rb");
	AddWindow(window);
	GAMEIN->SetTitanMixDlg((CTitanMixDlg *)window);
	GAMEIN->GetTitanMixDlg()->Linking();
}
void cWindowManager::CreateTitanRepairDlg()
{
	cWindow * window = GetDlgInfoFromFile("./image/InterfaceScript/Titan_Repair.bin", "rb");
	AddWindow(window);
	GAMEIN->SetTitanRepairDlg((CTitanRepairDlg *)window);
	GAMEIN->GetTitanRepairDlg()->Linking();
}
void cWindowManager::CreateTitanRecallDlg()
{
	cWindow * window = GetDlgInfoFromFile("./image/InterfaceScript/Titan_recall.bin", "rb");
	AddWindow(window);
	GAMEIN->SetTitanRecallDlg((CTitanRecallDlg *)window);
	GAMEIN->GetTitanRecallDlg()->Linking();
}
void cWindowManager::CreateTitanUpgradeDlg()
{
	cWindow * window = GetDlgInfoFromFile("./image/InterfaceScript/TitanUpgrade.bin", "rb");
	AddWindow(window);
	GAMEIN->SetTitanUpgradeDlg((CTitanUpgradeDlg *)window);
	GAMEIN->GetTitanUpgradeDlg()->Linking();
}
void cWindowManager::CreateTitanBreakDlg()
{
	cWindow * window = GetDlgInfoFromFile("./image/InterfaceScript/TitanBreak.bin", "rb");
	AddWindow(window);
	GAMEIN->SetTitanBreakDlg((CTitanBreakDlg *)window);
	GAMEIN->GetTitanBreakDlg()->Linking();
}
void cWindowManager::CreateTitanChangePreViewDlg()
{
	cWindow * window = GetDlgInfoFromFile("./image/InterfaceScript/TitanPartsChangePreview.bin", "rb");
	AddWindow(window);
	GAMEIN->SetTitanChangePreViewDlg((CTitanChangePreViewDlg *)window);
	GAMEIN->GetTitanChangePreViewDlg()->Linking();
}
void cWindowManager::CreateTitanInventoryDlg()
{
	cWindow * window = GetDlgInfoFromFile("./image/InterfaceScript/Titan_inventory.bin", "rb");
	AddWindow(window);
	GAMEIN->SetTitanInventoryDlg((CTitanInventoryDlg *)window);
	GAMEIN->GetTitanInventoryDlg()->Linking();
}
void cWindowManager::CreateTitanGuageDlg()
{
	cWindow * window = GetDlgInfoFromFile("./image/InterfaceScript/Titan_guage.bin", "rb");
	AddWindow(window);
	GAMEIN->SetTitanGuageDlg((CTitanGuageDlg *)window);
	GAMEIN->GetTitanGuageDlg()->Linking();
}
void cWindowManager::CreateTitanRegisterDlg()
{
	cWindow * window = GetDlgInfoFromFile("./image/InterfaceScript/Titan_use.bin", "rb");
	AddWindow(window);
	GAMEIN->SetTitanRegisterDlg((CTitanRegisterDlg *)window);
	GAMEIN->GetTitanRegisterDlg()->Linking();
}
void cWindowManager::CreateTitanDissolution()
{
	cWindow * window = GetDlgInfoFromFile("./image/InterfaceScript/Titan_Bongin.bin", "rb");
	AddWindow(window);
	GAMEIN->SetTitanDissolutionDlg((CTitanDissolutionDlg *)window);
	GAMEIN->GetTitanDissolutionDlg()->Linking();
}
void cWindowManager::CreateReinforceDlg()
{
	cWindow * window = GetDlgInfoFromFile("./image/InterfaceScript/57.bin", "rb");
	AddWindow(window);
	GAMEIN->SetReinforceDialog((CReinforceDlg *)window);
	GAMEIN->GetReinforceDialog()->Linking();
}
void cWindowManager::CreateHeroGuage()
{
	cWindow * window = GetDlgInfoFromFile("./image/InterfaceScript/23.bin", "rb");
	AddWindow(window);
	GAMEIN->SetGuageDlg((CGuageDialog *)window);
	GAMEIN->GetGuageDlg()->Linking();
}
void cWindowManager::CreateQuickDlg()
{
	cWindow * window = GetDlgInfoFromFile("./image/InterfaceScript/14.bin", "rb");
	ASSERT(window);
	//VECTOR2 Pos1;
	//Pos1.x = ((float)GET_MAINWIN_W - window->GetWidth());
	//Pos1.y = ((float)GET_MAINWIN_H / 2 - window->GetHeight() / 2) + 85;
	//window->SetAbsXY(Pos1.x, Pos1.y);
	AddWindow(window);
	GAMEIN->SetQuickDialog((CQuickDialog *)window);
}
void cWindowManager::CreateMenuSlotDlg()
{
	cWindow * window = GetDlgInfoFromFile("./image/MenuSlot.bin", "rb");
	ASSERT(window);
	AddWindow(window);
	GAMEIN->SetMenuSlotDialog((CMenuSlotDialog *)window);
	GAMEIN->GetMenuSlotDialog()->Linking();
}
void cWindowManager::CreatePartyDlg()
{
	cWindow * member1window = GetDlgInfoFromFile("./image/InterfaceScript/partymember1dlg.bin", "rb");
	cWindow * member2window = GetDlgInfoFromFile("./image/InterfaceScript/partymember2dlg.bin", "rb");
	cWindow * member3window = GetDlgInfoFromFile("./image/InterfaceScript/partymember3dlg.bin", "rb");
	cWindow * member4window = GetDlgInfoFromFile("./image/InterfaceScript/partymember4dlg.bin", "rb");
	cWindow * member5window = GetDlgInfoFromFile("./image/InterfaceScript/partymember5dlg.bin", "rb");
	cWindow * member6window = GetDlgInfoFromFile("./image/InterfaceScript/partymember6dlg.bin", "rb");
	cWindow * partybtnwindow = GetDlgInfoFromFile("./image/InterfaceScript/PartyBtnDialog.bin", "rb");
	ASSERT(member1window);	ASSERT(member2window);	ASSERT(member3window);
	ASSERT(member4window);	ASSERT(member5window);	ASSERT(member6window);
	ASSERT(partybtnwindow);
	AddWindow(member1window);	AddWindow(member2window);	AddWindow(member3window);
	AddWindow(member4window);	AddWindow(member5window);	AddWindow(member6window);
	AddWindow(partybtnwindow);
	GAMEIN->GetPartyDialog()->RegistMemberDlg(0, (CPartyMemberDlg*)member1window);
	GAMEIN->GetPartyDialog()->RegistMemberDlg(1, (CPartyMemberDlg*)member2window);
	GAMEIN->GetPartyDialog()->RegistMemberDlg(2, (CPartyMemberDlg*)member3window);
	GAMEIN->GetPartyDialog()->RegistMemberDlg(3, (CPartyMemberDlg*)member4window);
	GAMEIN->GetPartyDialog()->RegistMemberDlg(4, (CPartyMemberDlg*)member5window);
	GAMEIN->GetPartyDialog()->RegistMemberDlg(5, (CPartyMemberDlg*)member6window);
	GAMEIN->GetPartyDialog()->RegistBtnDlg((CPartyBtnDlg*)partybtnwindow);
}
void cWindowManager::CreatePartyCreateDlg()
{
	cWindow * window = GetDlgInfoFromFile("./image/InterfaceScript/PartyCreate.bin", "rb");
	ASSERT(window);
	AddWindow(window);
	GAMEIN->SetPartyCreateDialog((CPartyCreateDlg *)window);
	GAMEIN->GetPartyCreateDialog()->Linking();
}
void cWindowManager::CreatePartyInviteDlg()
{
	cWindow * window = GetDlgInfoFromFile("./image/InterfaceScript/PartyInvite.bin", "rb");
	ASSERT(window);
	AddWindow(window);
	GAMEIN->SetPartyInviteDialog((CPartyInviteDlg *)window);
	GAMEIN->GetPartyInviteDialog()->Linking();
}
void cWindowManager::CreateGuildCreateDlg()
{
	cWindow * window = GetDlgInfoFromFile("./image/InterfaceScript/GuildCreate.bin", "rb");
	ASSERT(window);
	VECTOR2 Pos1;
	Pos1.x = ((float)GET_MAINWIN_W / 2 - window->GetWidth() / 2);
	Pos1.y = ((float)GET_MAINWIN_H / 2 - window->GetHeight() / 2);
	window->SetAbsXY(Pos1.x, Pos1.y);
	AddWindow(window);
	GAMEIN->SetGuildCreateDlg((CGuildCreateDialog *)window);
	GAMEIN->GetGuildCreateDlg()->Linking();
	window = GetDlgInfoFromFile("./image/InterfaceScript/GuildUnionCreate.bin", "rb");
	ASSERT(window);
	AddWindow(window);
	GAMEIN->SetGuildUnionCreateDlg((CGuildUnionCreateDialog *)window);
	GAMEIN->GetGuildUnionCreateDlg()->Linking();
}
void cWindowManager::CreateGuildDlg()
{
	cWindow * window = GetDlgInfoFromFile("./image/InterfaceScript/Guild.bin", "rb");
	ASSERT(window);
	VECTOR2 Pos1;
	Pos1.x = ((float)GET_MAINWIN_W / 2 - window->GetWidth() / 2);
	Pos1.y = ((float)GET_MAINWIN_H / 2 - window->GetHeight() / 2);
	window->SetAbsXY(Pos1.x, Pos1.y);
	AddWindow(window);
	GAMEIN->SetGuildDlg((CGuildDialog *)window);
	GAMEIN->GetGuildDlg()->Linking();
}
void cWindowManager::CreateGuildInviteDlg()
{
	cWindow * window = GetDlgInfoFromFile("./image/InterfaceScript/GuildInvite.bin", "rb");
	ASSERT(window);
	VECTOR2 Pos1;
	Pos1.x = ((float)GET_MAINWIN_W / 2 - window->GetWidth() / 2);
	Pos1.y = ((float)GET_MAINWIN_H / 2 - window->GetHeight() / 2);
	window->SetAbsXY(Pos1.x, Pos1.y);
	AddWindow(window);
	GAMEIN->SetGuildInviteDlg((CGuildInviteDialog *)window);
	GAMEIN->GetGuildInviteDlg()->Linking();
}
void cWindowManager::CreateGuildMarkDlg()
{
	cWindow * window = GetDlgInfoFromFile("./image/InterfaceScript/GuildMark.bin", "rb");
	ASSERT(window);
	VECTOR2 Pos1;
	Pos1.x = ((float)GET_MAINWIN_W / 2 - window->GetWidth() / 2);
	Pos1.y = ((float)GET_MAINWIN_H / 2 - window->GetHeight() / 2);
	window->SetAbsXY(Pos1.x, Pos1.y);
	AddWindow(window);
	GAMEIN->SetGuildMarkDlg((CGuildMarkDialog *)window);
	GAMEIN->GetGuildMarkDlg()->Linking();
}
void cWindowManager::CreateGuildLevelupDlg()
{
	cWindow * window = GetDlgInfoFromFile("./image/InterfaceScript/GuildLevelUp.bin", "rb");
	ASSERT(window);
	VECTOR2 Pos1;
	Pos1.x = ((float)GET_MAINWIN_W / 2 - window->GetWidth() / 2);
	Pos1.y = ((float)GET_MAINWIN_H / 2 - window->GetHeight() / 2);
	window->SetAbsXY(Pos1.x, Pos1.y);
	AddWindow(window);
	GAMEIN->SetGuildLevelUpDlg((CGuildLevelUpDialog *)window);
	GAMEIN->GetGuildLevelUpDlg()->Linking();
}
void cWindowManager::CreateGuildNickNameDlg()
{
	cWindow * window = GetDlgInfoFromFile("./image/InterfaceScript/GuildNickName.bin", "rb");
	ASSERT(window);
	VECTOR2 Pos1;
	Pos1.x = ((float)GET_MAINWIN_W / 2 - window->GetWidth() / 2);
	Pos1.y = ((float)GET_MAINWIN_H / 2 - window->GetHeight() / 2);
	window->SetAbsXY(Pos1.x, Pos1.y);
	AddWindow(window);
	GAMEIN->SetGuildNickNameDlg((CGuildNickNameDialog *)window);
	GAMEIN->GetGuildNickNameDlg()->Linking();
}
void cWindowManager::CreateGuildRankDlg()
{
	cWindow * window = GetDlgInfoFromFile("./image/InterfaceScript/GuildRank.bin", "rb");
	ASSERT(window);
	VECTOR2 Pos1;
	Pos1.x = ((float)GET_MAINWIN_W / 2 - window->GetWidth() / 2);
	Pos1.y = ((float)GET_MAINWIN_H / 2 - window->GetHeight() / 2);
	window->SetAbsXY(Pos1.x, Pos1.y);
	AddWindow(window);
	GAMEIN->SetGuildRankDlg((CGuildRankDialog *)window);
	GAMEIN->GetGuildRankDlg()->Linking();
}
void cWindowManager::CreateGuildWarehouseDlg()
{
	cWindow * window = GetDlgInfoFromFile("./image/InterfaceScript/GuildWarehouse.bin", "rb");
	ASSERT(window);
	VECTOR2 Pos1;
	Pos1.x = ((float)GET_MAINWIN_W / 2 - window->GetWidth() / 2);
	Pos1.y = ((float)GET_MAINWIN_H / 2 - window->GetHeight() / 2);
	window->SetAbsXY(Pos1.x, Pos1.y);
	AddWindow(window);
	GAMEIN->SetGuildWarehouseDlg((CGuildWarehouseDialog *)window);
	GAMEIN->GetGuildWarehouseDlg()->Linking();
}
void cWindowManager::CreatePyogukDlg()
{
	cWindow * window = GetDlgInfoFromFile("./image/InterfaceScript/Pyoguk.bin", "rb");
	VECTOR2 Pos1;
	Pos1.x = ((float)GET_MAINWIN_W / 2 - window->GetWidth() / 2);
	Pos1.y = ((float)GET_MAINWIN_H / 2 - window->GetHeight() / 2);
	window->SetAbsXY(Pos1.x, Pos1.y);
	AddWindow(window);
	GAMEIN->SetPyogukDialog((CPyogukDialog *)window);
	GAMEIN->GetPyogukDialog()->Linking();
}
void cWindowManager::CreateWantNpcDlg()
{
	cWindow * window = GetDlgInfoFromFile("./image/InterfaceScript/WantNpc.bin", "rb");
	VECTOR2 Pos1;
	Pos1.x = ((float)GET_MAINWIN_W / 2 - window->GetWidth() / 2);
	Pos1.y = ((float)GET_MAINWIN_H / 2 - window->GetHeight() / 2);
	window->SetAbsXY(Pos1.x, Pos1.y);
	AddWindow(window);
	GAMEIN->SetWantNpcDialog((CWantNpcDialog *)window);
	GAMEIN->GetWantNpcDialog()->Linking();
}
void cWindowManager::CreateWantRegistDlg()
{
	cWindow * window = GetDlgInfoFromFile("./image/InterfaceScript/WantRegist.bin", "rb");
	ASSERT(window);
	VECTOR2 Pos1;
	Pos1.x = ((float)GET_MAINWIN_W / 2 - window->GetWidth() / 2);
	Pos1.y = ((float)GET_MAINWIN_H / 2 - window->GetHeight() / 2);
	window->SetAbsXY(Pos1.x, Pos1.y);
	AddWindow(window);
	GAMEIN->SetWantRegistDialog((CWantRegistDialog *)window);
	GAMEIN->GetWantRegistDialog()->Linking();
}
void cWindowManager::CreateMiniMapDlg()
{
	cWindow * window = GetDlgInfoFromFile("./image/InterfaceScript/17.bin", "rb");

	ASSERT(window);

	//VECTOR2 Pos1;
	//Pos1.x = ((float)GET_MAINWIN_W - window->GetWidth());
	//Pos1.y = window->GetAbsY();
	//window->SetAbsXY(Pos1.x, Pos1.y);

	AddWindow(window);

	GAMEIN->SetMiniMapDialog((CMiniMapDlg *)window);
	GAMEIN->GetMiniMapDialog()->Linking();
	GAMEIN->GetMiniMapDialog()->InitMiniMap(MAP->GetMapNum());



	cWindow * window2 = GetDlgInfoFromFile("./image/InterfaceScript/bigmap.bin", "rb");
	ASSERT(window2);
	VECTOR2 Pos2;
	Pos2.x = ((float)GET_MAINWIN_W / 2) - 280;
	Pos2.y = ((float)GET_MAINWIN_H / 2) - 280;
	window2->SetAbsXY(Pos2.x, Pos2.y);
	AddWindow(window2);
	GAMEIN->SetBigMapDialog((CBigMapDlg *)window2);
	if (GAMEIN->GetBigMapDialog())
	{
		GAMEIN->GetBigMapDialog()->Linking();
		GAMEIN->GetBigMapDialog()->InitBigMap(MAP->GetMapNum());
	}
}
void cWindowManager::CreateMacroDlg()
{
#ifdef _FILE_BIN_
	cWindow* window = GetDlgInfoFromFile("./image/16.bin", "rb");
#else
	cWindow* window = GetDlgInfoFromFile("./image/16.txt");
#endif
	AddWindow(window);
	GAMEIN->SetMacroDialog((CMacroDialog*)window);
	GAMEIN->GetMacroDialog()->Linking();
}
void cWindowManager::CreateChatDlg()
{
	cWindow * window = GetDlgInfoFromFile("./image/InterfaceScript/22.bin", "rb");
	//VECTOR2 Pos1;
	//Pos1.x = window->GetAbsX();
	//Pos1.y = ((float)GET_MAINWIN_H - window->GetHeight());
	//window->SetAbsXY(Pos1.x, Pos1.y);
	window->SetWidth(420);
	AddWindow(window);
	GAMEIN->SetChatDialog((CChatDialog *)window);
	GAMEIN->GetChatDialog()->Linking();
}
void cWindowManager::CreateOptionDlg()
{
	cWindow * window = GetDlgInfoFromFile("./image/InterfaceScript/21.bin", "rb");
	//VECTOR2 Pos1;
	//Pos1.x = ((float)GET_MAINWIN_W / 2 - window->GetWidth() / 2);
	//Pos1.y = ((float)GET_MAINWIN_H / 2 - window->GetHeight() / 2);
	//window->SetAbsXY(Pos1.x, Pos1.y);
	AddWindow(window);
	GAMEIN->SetOptionDialog((COptionDialog *)window);
	GAMEIN->GetOptionDialog()->Linking();
}
void cWindowManager::CreateExitDlg()
{
	cWindow * window = GetDlgInfoFromFile("./image/InterfaceScript/25.bin", "rb");
	//VECTOR2 Pos1;
	//Pos1.x = ((float)GET_MAINWIN_W / 2 - window->GetWidth() / 2);
	//Pos1.y = ((float)GET_MAINWIN_H / 2 - window->GetHeight() / 2);
	//window->SetAbsXY(Pos1.x, Pos1.y);
	AddWindow(window);
	GAMEIN->SetExitDialog((CExitDialog *)window);
}
void cWindowManager::CreateCharStateDlg()
{
	cWindow * window = GetDlgInfoFromFile("./image/InterfaceScript/29.bin", "rb");
	AddWindow(window);
	GAMEIN->SetCharStateDialog((CCharStateDialog*)window);
	GAMEIN->GetCharStateDialog()->Linking();
}
void cWindowManager::CreateNoteDlg()
{
	cWindow * window = GetDlgInfoFromFile("./image/InterfaceScript/Note.bin", "rb");
	ASSERT(window);
	VECTOR2 Pos1;
	Pos1.x = ((float)GET_MAINWIN_W / 2 - window->GetWidth() / 2);
	Pos1.y = ((float)GET_MAINWIN_H / 2 - window->GetHeight() / 2);
	window->SetAbsXY(Pos1.x, Pos1.y);
	AddWindow(window);
	GAMEIN->SetNoteDialog((CNoteDialog *)window);
	GAMEIN->GetNoteDialog()->Linking();
}
void cWindowManager::CreateFriendDlg()
{
	cWindow * window = GetDlgInfoFromFile("./image/InterfaceScript/Friend.bin", "rb");
	ASSERT(window);
	VECTOR2 Pos1;
	Pos1.x = ((float)GET_MAINWIN_W / 2 - window->GetWidth() / 2);
	Pos1.y = ((float)GET_MAINWIN_H / 2 - window->GetHeight() / 2);
	window->SetAbsXY(Pos1.x, Pos1.y);
	AddWindow(window);
	GAMEIN->SetFriendDialog((CFriendDialog *)window);
	GAMEIN->GetFriendDialog()->Linking();
}
void cWindowManager::CreateMiniFriendDlg()
{
	cWindow * window = GetDlgInfoFromFile("./image/InterfaceScript/MiniFriend.bin", "rb");
	ASSERT(window);
	VECTOR2 Pos1;
	Pos1.x = ((float)GET_MAINWIN_W / 2 - window->GetWidth() / 2);
	Pos1.y = ((float)GET_MAINWIN_H / 2 - window->GetHeight() / 2);
	window->SetAbsXY(Pos1.x, Pos1.y);
	AddWindow(window);
	GAMEIN->SetMiniFriendDialog((CMiniFriendDialog *)window);
	GAMEIN->GetMiniFriendDialog()->Linking();
}
void cWindowManager::CreateMiniNoteDlg()
{
	cWindow * window = GetDlgInfoFromFile("./image/InterfaceScript/MiniNote.bin", "rb");
	ASSERT(window);
	VECTOR2 Pos1;
	Pos1.x = ((float)GET_MAINWIN_W / 2 - window->GetWidth() / 2);
	Pos1.y = ((float)GET_MAINWIN_H / 2 - window->GetHeight() / 2);
	window->SetAbsXY(Pos1.x, Pos1.y);
	AddWindow(window);
	GAMEIN->SetMiniNoteDialog((CMiniNoteDialog *)window);
	GAMEIN->GetMiniNoteDialog()->Linking();
}
void cWindowManager::CreateMarryMiniDialog()
{
	cWindow * window = GetDlgInfoFromFile("./image/InterfaceScript/MiniMarry.bin", "rb");
	ASSERT(window);
	VECTOR2 Pos1;
	Pos1.x = ((float)GET_MAINWIN_W / 2 - window->GetWidth() / 2);
	Pos1.y = ((float)GET_MAINWIN_H / 2 - window->GetHeight() / 2);
	window->SetAbsXY(Pos1.x, Pos1.y);
	AddWindow(window);
	CMiniMaryDialog* pDlg = (CMiniMaryDialog*)window;
	GAMEIN->SetMiniMaryDialog(pDlg);
	GAMEIN->GetMiniMaryDialog()->Linking();
}
void cWindowManager::CreateMiniShiTuDialog()
{
	cWindow * window = GetDlgInfoFromFile("./image/InterfaceScript/MiniShiTu.bin", "rb");
	ASSERT(window);
	VECTOR2 Pos1;
	Pos1.x = ((float)GET_MAINWIN_W / 2 - window->GetWidth() / 2);
	Pos1.y = ((float)GET_MAINWIN_H / 2 - window->GetHeight() / 2);
	window->SetAbsXY(Pos1.x, Pos1.y);
	AddWindow(window);
	CBaiShiDialog* pDlg = (CBaiShiDialog*)window;
	GAMEIN->SetMiniBaishiDialog(pDlg);
	GAMEIN->GetMiniBaishiDialog()->Linking();
}
void cWindowManager::CreateControlDialog()
{
#ifdef _FILE_BIN_
	cWindow * window = GetDlgInfoFromFile("./image/InterfaceScript/Control.bin", "rb");
#else
	cWindow * window = GetDlgInfoFromFile("./image/InterfaceScript/Control.txt", "rb");
#endif
	ASSERT(window);
	VECTOR2 Pos1;
	Pos1.x = window->GetAbsX() + ((float)GAMERESRCMNGR->m_GameDesc.dispInfo.dwWidth - MIDRSLTN_W) / 2;
	Pos1.y = window->GetAbsY() + ((float)GAMERESRCMNGR->m_GameDesc.dispInfo.dwHeight - MIDRSLTN_H) / 2;
	window->SetAbsXY(Pos1.x, Pos1.y);
	AddWindow(window);
	CControlDialog* pDlg = (CControlDialog*)window;
	GAMEIN->SetControlDialog(pDlg);
	GAMEIN->GetControlDialog()->Linking();
}
void cWindowManager::CreateBillboardDialog()
{
	cWindow * window = GetDlgInfoFromFile("./image/InterfaceScript/RankingDlg.bin", "rb");
	ASSERT(window);
	VECTOR2 Pos1;
	Pos1.x = ((float)GET_MAINWIN_W / 2 - window->GetWidth() / 2);
	Pos1.y = ((float)GET_MAINWIN_H / 2 - window->GetHeight() / 2);
	window->SetAbsXY(Pos1.x, Pos1.y);
	AddWindow(window);
	CTopDialog * pDlg = (CTopDialog*)window;
	GAMEIN->SetTopDialog(pDlg);
	GAMEIN->GetTopDialog()->Linking();
}
void cWindowManager::CreateBillboardMunpDialog()
{
	cWindow * window = GetDlgInfoFromFile("./image/InterfaceScript/GuildRankingDlg.bin", "rb");
	ASSERT(window);
	VECTOR2 Pos1;
	Pos1.x = ((float)GET_MAINWIN_W / 2 - window->GetWidth() / 2);
	Pos1.y = ((float)GET_MAINWIN_H / 2 - window->GetHeight() / 2);
	window->SetAbsXY(Pos1.x, Pos1.y);
	AddWindow(window);
	CTopMunpDialog * pDlg = (CTopMunpDialog*)window;
	GAMEIN->SetTopMunpDialog(pDlg);
	GAMEIN->GetTopMunpDialog()->Linking();
}
void cWindowManager::CreateLuckerDialog()
{
	cWindow * window = GetDlgInfoFromFile("./image/InterfaceScript/LuckerDlg.bin", "rb");
	VECTOR2 Pos1;
	Pos1.x = ((float)GET_MAINWIN_W / 2 - window->GetWidth() / 2);
	Pos1.y = ((float)GET_MAINWIN_H / 2 - window->GetHeight() / 2);
	window->SetAbsXY(Pos1.x, Pos1.y);
	AddWindow(window);
	CLuckerDialog* pDlg = (CLuckerDialog*)window;
	GAMEIN->SetLuckerDialog(pDlg);
	pDlg->Linking();
}
void cWindowManager::CreateMoneyPacketDialog()
{
	cWindow * window = GetDlgInfoFromFile("./image/InterfaceScript/MoneyPacketDlg.bin", "rb");
	ASSERT(window);
	AddWindow(window);
	CMoneyPacketDialog* pDlg = (CMoneyPacketDialog*)window;
	GAMEIN->SetMoneyPacketDialog(pDlg);
	GAMEIN->GetMoneyPacketDialog()->Linking();
}
void cWindowManager::CreateOtherCharacterMoneyDlg()
{
	cWindow * window = GetDlgInfoFromFile("./image/InterfaceScript/OtherCharacterMoneyDlg.bin", "rb");
	ASSERT(window);
	VECTOR2 Pos1;
	Pos1.x = ((float)GET_MAINWIN_W / 2 - window->GetWidth() / 2);
	Pos1.y = ((float)GET_MAINWIN_H / 2 - window->GetHeight() / 2);
	window->SetAbsXY(Pos1.x, Pos1.y);
	AddWindow(window);
	COtherCharacerMoneyDlg* pDlg = (COtherCharacerMoneyDlg*)window;
	GAMEIN->SetOtherCharacterMoneyDlg(pDlg);
	GAMEIN->GetOtherCharacterMoneyDlg()->Linking();
}
void cWindowManager::CreateOtherCharacterAmuletDlg()
{
	cWindow * window = GetDlgInfoFromFile("./image/InterfaceScript/OtherCharacterAmuletDlg.bin", "rb");
	ASSERT(window);
	AddWindow(window);
	COtherCharacterAmuletDlg* pDlg = (COtherCharacterAmuletDlg*)window;
	GAMEIN->SetOtherCharacterAmuletDlg(pDlg);
	GAMEIN->GetOtherCharacterAmuletDlg()->Linking();
}
void cWindowManager::CreateItemStoneDkDialog()
{
	cWindow * window = GetDlgInfoFromFile("./image/InterfaceScript/ItemStoneDk.bin", "rb");
	ASSERT(window);
	VECTOR2 Pos1;
	Pos1.x = ((float)GET_MAINWIN_W / 2 - window->GetWidth() / 2);
	Pos1.y = ((float)GET_MAINWIN_H / 2 - window->GetHeight() / 2);
	window->SetAbsXY(Pos1.x, Pos1.y);
	AddWindow(window);
	CItemStoneDkDialog * pDlg = (CItemStoneDkDialog*)window;
	GAMEIN->SetItemStoneDkDlg(pDlg);
	GAMEIN->GetItemStoneDkDlg()->Linking();
}
void cWindowManager::CreateItemStoneXqDialog()
{
	cWindow * window = GetDlgInfoFromFile("./image/InterfaceScript/ItemStoneXq.bin", "rb");
	VECTOR2 Pos1;
	Pos1.x = ((float)GET_MAINWIN_W / 2 - window->GetWidth() / 2);
	Pos1.y = ((float)GET_MAINWIN_H / 2 - window->GetHeight() / 2);
	window->SetAbsXY(Pos1.x, Pos1.y);
	AddWindow(window);
	CItemStoneXqDialog * pDlg = (CItemStoneXqDialog*)window;
	GAMEIN->SetItemStoneXqDlg(pDlg);
	GAMEIN->GetItemStoneXqDlg()->Linking();
}
void cWindowManager::CreateItemStepReinforceDialog()
{
	cWindow * window = GetDlgInfoFromFile("./image/InterfaceScript/ItemStepReinforce.bin", "rb");
	VECTOR2 Pos1;
	Pos1.x = ((float)GET_MAINWIN_W / 2 - window->GetWidth() / 2);
	Pos1.y = ((float)GET_MAINWIN_H / 2 - window->GetHeight() / 2);
	window->SetAbsXY(Pos1.x, Pos1.y);
	AddWindow(window);
	CItemStepReinforceDialog * pDlg = (CItemStepReinforceDialog*)window;
	GAMEIN->SetItemStepReinforceDlg(pDlg);
	GAMEIN->GetItemStepReinforceDlg()->Linking();
}
void cWindowManager::CreateReviveDlg()
{
	cWindow * window = GetDlgInfoFromFile("./image/InterfaceScript/Revive.bin", "rb");
	ASSERT(window);
	VECTOR2 Pos1;
	Pos1.x = ((float)GET_MAINWIN_W / 2 - window->GetWidth() / 2);
	Pos1.y = ((float)GET_MAINWIN_H / 2 - window->GetHeight() / 2);
	window->SetAbsXY(Pos1.x, Pos1.y);
	AddWindow(window);
	GAMEIN->SetReviveDialog((CReviveDialog *)window);
	GAMEIN->GetReviveDialog()->Linking();
}
void cWindowManager::CreateCharDlg()
{
	cWindow * window = GetDlgInfoFromFile("./image/InterfaceScript/24.bin", "rb");
	VECTOR2 Pos1;
	Pos1.x = ((float)GET_MAINWIN_W / 2 - window->GetWidth() / 2);
	Pos1.y = ((float)GET_MAINWIN_H / 2 - window->GetHeight() / 2);
	window->SetAbsXY(Pos1.x, Pos1.y);
	AddWindow(window);
	GAMEIN->SetCharacterDialog((CCharacterDialog *)window);
	GAMEIN->GetCharacterDialog()->Linking();
}
void cWindowManager::CreateCharPvPDlg()
{
	cWindow* window = GetDlgInfoFromFile("./image/InterfaceScript/MyInfoPVP.bin", "rb");
	VECTOR2 Pos1;
	Pos1.x = ((float)GET_MAINWIN_W / 2 - window->GetWidth() / 2);
	Pos1.y = ((float)GET_MAINWIN_H / 2 - window->GetHeight() / 2);
	window->SetAbsXY(Pos1.x, Pos1.y);
	AddWindow(window);
	GAMEIN->SetCharacterPvPDialog((CCharacterPvpDialog*)window);
	GAMEIN->GetCharacterPvPDialog()->Linking();
}
void cWindowManager::CreateMugongSuryunDlg()
{
	cWindow * window = GetDlgInfoFromFile("./image/InterfaceScript/10.bin", "rb");
	VECTOR2 Pos1;
	Pos1.x = ((float)GET_MAINWIN_W / 2 - window->GetWidth() / 2);
	Pos1.y = ((float)GET_MAINWIN_H / 2 - window->GetHeight() / 2);
	window->SetAbsXY(Pos1.x, Pos1.y);
	AddWindow(window);
	CMugongSuryunDialog * pDlg = (CMugongSuryunDialog *)window;
	GAMEIN->SetMugongSuryunDialog(pDlg);
	CMugongDialog* pMugongDlg = (CMugongDialog*)pDlg->GetTabSheet(0);
	pMugongDlg->Linking();
	GAMEIN->SetMugongDialog(pMugongDlg);
	MUGONGMGR->SetMugongDialog(pMugongDlg);
	CSuryunDialog* pSuryunDlg = (CSuryunDialog*)pDlg->GetTabSheet(1);
	GAMEIN->SetSuryunDialog(pSuryunDlg);
	pSuryunDlg->Linking();
}
void cWindowManager::CreateMPRegistDlg()
{
	cWindow * window = GetDlgInfoFromFile("./image/InterfaceScript/MPRegist.bin", "rb");
	VECTOR2 Pos1;
	Pos1.x = ((float)GET_MAINWIN_W / 2 - window->GetWidth() / 2);
	Pos1.y = ((float)GET_MAINWIN_H / 2 - window->GetHeight() / 2);
	window->SetAbsXY(Pos1.x, Pos1.y);
	AddWindow(window);
	GAMEIN->SetMPRegistDialog((CMPRegistDialog *)window);
	GAMEIN->GetMPRegistDialog()->Linking();
}
void cWindowManager::CreateMPMissionDlg()
{
	cWindow * window = GetDlgInfoFromFile("./image/InterfaceScript/MPMission.bin", "rb");
	VECTOR2 Pos1;
	Pos1.x = ((float)GET_MAINWIN_W / 2 - window->GetWidth() / 2);
	Pos1.y = ((float)GET_MAINWIN_H / 2 - window->GetHeight() / 2);
	window->SetAbsXY(Pos1.x, Pos1.y);
	AddWindow(window);
	GAMEIN->SetMPMissionDialog((CMPMissionDialog *)window);
	GAMEIN->GetMPMissionDialog()->Linking();
}
void cWindowManager::CreateMPNoticeDlg()
{
	cWindow * window = GetDlgInfoFromFile("./image/InterfaceScript/MPNotice.bin", "rb");
	VECTOR2 Pos1;
	Pos1.x = ((float)GET_MAINWIN_W / 2 - window->GetWidth() / 2);
	Pos1.y = ((float)GET_MAINWIN_H / 2 - window->GetHeight() / 2);
	window->SetAbsXY(Pos1.x, Pos1.y);
	AddWindow(window);
	GAMEIN->SetMPNoticeDialog((CMPNoticeDialog *)window);
	GAMEIN->GetMPNoticeDialog()->Linking();
}
void cWindowManager::CreateMPGuageDlg()
{
	cWindow * window = GetDlgInfoFromFile("./image/InterfaceScript/MPGuage.bin", "rb");
	VECTOR2 Pos1;
	Pos1.x = ((float)GET_MAINWIN_W - window->GetWidth());
	Pos1.y = window->GetAbsY();
	window->SetAbsXY(Pos1.x, Pos1.y);
	AddWindow(window);
	GAMEIN->SetMPGuageDialog((CMPGuageDialog *)window);
	GAMEIN->GetMPGuageDialog()->Linking();
}
void cWindowManager::CreatePetStateDlg()
{
	cWindow* window = GetDlgInfoFromFile("./image/InterfaceScript/PetState.bin", "rb");
	VECTOR2 Pos1;
	Pos1.x = ((float)GET_MAINWIN_W - window->GetWidth()) - 150;
	Pos1.y = window->GetAbsY();
	window->SetAbsXY(Pos1.x, Pos1.y);
	AddWindow(window);
	GAMEIN->SetPetStateDialog((CPetStateDlg*)window);
	GAMEIN->GetPetStateDialog()->Linking();
	PETMGR->SetPetStateDlg((CPetStateDlg*)window);
}
void cWindowManager::CreatePetStateMiniDlg()
{
	cWindow* window = GetDlgInfoFromFile("./image/InterfaceScript/PetStateMini.bin", "rb");
	VECTOR2 Pos1;
	Pos1.x = window->GetAbsX() + (float)(GAMERESRCMNGR->m_GameDesc.dispInfo.dwWidth - MIDRSLTN_W);
	Pos1.y = window->GetAbsY();
	window->SetAbsXY(Pos1.x, Pos1.y);
	AddWindow(window);
	GAMEIN->SetPetStateMiniDialog((CPetStateMiniDlg*)window);
	GAMEIN->GetPetStateMiniDialog()->Linking();
	PETMGR->SetPetStateMiniDlg((CPetStateMiniDlg*)window);
}
void cWindowManager::CreatePetInvenDlg()
{
	cWindow* window = GetDlgInfoFromFile("./image/InterfaceScript/PetInven.bin", "rb");
	VECTOR2 Pos1;
	Pos1.x = ((float)GET_MAINWIN_W / 2 - window->GetWidth() / 2);
	Pos1.y = ((float)GET_MAINWIN_H / 2 - window->GetHeight() / 2);
	window->SetAbsXY(Pos1.x, Pos1.y);
	AddWindow(window);
	GAMEIN->SetPetInventoryDialog((CPetInventoryDlg*)window);
	GAMEIN->GetPetInventoryDialog()->Linking();
	PETMGR->SetPetInventoryDlg((CPetInventoryDlg*)window);
}
void cWindowManager::CreatePetUpgradeDlg()
{
	cWindow* window = GetDlgInfoFromFile("./image/InterfaceScript/PetUpgrade.bin", "rb");
	VECTOR2 Pos1;
	Pos1.x = ((float)GET_MAINWIN_W / 2 - window->GetWidth() / 2);
	Pos1.y = ((float)GET_MAINWIN_H / 2 - window->GetHeight() / 2);
	window->SetAbsXY(Pos1.x, Pos1.y);
	AddWindow(window);
	GAMEIN->SetPetUpgradeDialog((CPetUpgradeDialog*)window);
	GAMEIN->GetPetUpgradeDialog()->Linking();
}
void cWindowManager::CreatePetRevivalDlg()
{
	cWindow* window = GetDlgInfoFromFile("./image/InterfaceScript/PetRevival.bin", "rb");
	VECTOR2 Pos1;
	Pos1.x = ((float)GET_MAINWIN_W / 2 - window->GetWidth() / 2);
	Pos1.y = ((float)GET_MAINWIN_H / 2 - window->GetHeight() / 2);
	window->SetAbsXY(Pos1.x, Pos1.y);
	AddWindow(window);
	GAMEIN->SetPetRevivalDialog((CPetRevivalDialog*)window);
	GAMEIN->GetPetRevivalDialog()->Linking();
}

void cWindowManager::CreatePetMultiDlg()
{
	cWindow* window = GetDlgInfoFromFile("./image/InterfaceScript/PetMulti.bin", "rb");
	VECTOR2 Pos1;
	Pos1.x = ((float)GET_MAINWIN_W - window->GetWidth()) - 150;
	Pos1.y = window->GetAbsY();
	window->SetAbsXY(Pos1.x, Pos1.y);
	AddWindow(window);
	GAMEIN->SetPetMultiDialog((CPetMultiDlg*)window);
	GAMEIN->GetPetMultiDialog()->Linking();
	PETMGR->SetPetMultiDlg((CPetMultiDlg*)window);
}

void cWindowManager::CreateInventoryDlg()
{
	cWindow * window = GetDlgInfoFromFile("./image/InterfaceScript/11.bin", "rb");
	VECTOR2 Pos1;
	Pos1.x = ((float)GET_MAINWIN_W / 2 - window->GetWidth() / 2);
	Pos1.y = ((float)GET_MAINWIN_H / 2 - window->GetHeight() / 2);
	window->SetAbsXY(Pos1.x, Pos1.y);
	AddWindow(window);
	GAMEIN->SetInventoryDialog((CInventoryExDialog *)window);
	GAMEIN->GetInventoryDialog()->Linking();
	cWindow * window2 = GetDlgInfoFromFile("./image/InterfaceScript/ChangeLook.bin", "rb");
	AddWindow(window2);
	GAMEIN->SetChangeLookDialog((cChangeLookDialog *)window2);
	GAMEIN->GetChangeLookDialog()->Linking();
}
void cWindowManager::CreateOtherCharacterInfo()
{
	cWindow * window = WINDOWMGR->GetDlgInfoFromFile("./image/InterfaceScript/OtherCharacterInfo.bin", "rb");
	VECTOR2 Pos1;
	Pos1.x = ((float)GET_MAINWIN_W / 2 - window->GetWidth() / 2);
	Pos1.y = ((float)GET_MAINWIN_H / 2 - window->GetHeight() / 2);
	window->SetAbsXY(Pos1.x, Pos1.y);
	AddWindow(window);
	GAMEIN->SetCharacterInfoDialog((CCharactorInfoDlg *)window);
	GAMEIN->GetCharacterInfoDlg()->Linking();
}
void cWindowManager::CreateOtherCharacterEquip()
{
	cWindow * window = WINDOWMGR->GetDlgInfoFromFile("./image/InterfaceScript/OtherCharacterEquip.bin", "rb");
	VECTOR2 Pos1;
	Pos1.x = ((float)GET_MAINWIN_W / 2 - window->GetWidth() / 2);
	Pos1.y = ((float)GET_MAINWIN_H / 2 - window->GetHeight() / 2);
	window->SetAbsXY(Pos1.x, Pos1.y);
	AddWindow(window);
	GAMEIN->SetShowEquipInfoDlg((CQuipInfoDlg *)window);
	GAMEIN->GetCharacterEquipInfoDlg()->Linking();
}
void cWindowManager::CreateExchangeDlg()
{
	cWindow* window = GetDlgInfoFromFile("./image/InterfaceScript/28.bin", "rb");
	VECTOR2 Pos1;
	Pos1.x = ((float)GET_MAINWIN_W / 2 - window->GetWidth() / 2);
	Pos1.y = ((float)GET_MAINWIN_H / 2 - window->GetHeight() / 2);
	window->SetAbsXY(Pos1.x, Pos1.y);
	AddWindow(window);
	GAMEIN->SetExchangeDialog((CExchangeDialog*)window);
	GAMEIN->GetExchangeDialog()->Linking();
}
void cWindowManager::CreateRecvExchangeDlg()
{
}
void cWindowManager::CreateStallKindSelectDlg()
{
	cWindow * window = GetDlgInfoFromFile("./image/InterfaceScript/StallOption.bin", "rb");
	VECTOR2 Pos1;
	Pos1.x = ((float)GET_MAINWIN_W / 2 - window->GetWidth() / 2);
	Pos1.y = ((float)GET_MAINWIN_H / 2 - window->GetHeight() / 2);
	window->SetAbsXY(Pos1.x, Pos1.y);
	AddWindow(window);
	GAMEIN->SetStallKindSelectDialog((CStallKindSelectDlg *)window);
	GAMEIN->GetStallKindSelectDialog()->Linking();
}
void cWindowManager::CreateStreetBuyStallDlg()
{
	cWindow * window = GetDlgInfoFromFile("./image/InterfaceScript/30.bin", "rb");
	VECTOR2 Pos1;
	Pos1.x = ((float)GET_MAINWIN_W / 2 - window->GetWidth() / 2);
	Pos1.y = ((float)GET_MAINWIN_H / 2 - window->GetHeight() / 2);
	window->SetAbsXY(Pos1.x, Pos1.y);
	AddWindow(window);
	GAMEIN->SetStreetBuyStallDialog((CStreetBuyStall *)window);
	GAMEIN->GetStreetBuyStallDialog()->Linking();
}
void cWindowManager::CreateBuyRegDlg()
{
	cWindow * window = GetDlgInfoFromFile("./image/InterfaceScript/32.bin", "rb");
	VECTOR2 Pos1;
	Pos1.x = ((float)GET_MAINWIN_W / 2 - window->GetWidth() / 2);
	Pos1.y = ((float)GET_MAINWIN_H / 2 - window->GetHeight() / 2);
	window->SetAbsXY(Pos1.x, Pos1.y);
	AddWindow(window);
	GAMEIN->SetBuyRegDialog((CBuyRegDialog *)window);
	GAMEIN->GetBuyRegDialog()->Linking();
}
void cWindowManager::CreateStallFindDlg()
{
	cWindow * window = GetDlgInfoFromFile("./image/InterfaceScript/ItemSearch.bin", "rb");
	VECTOR2 Pos1;
	Pos1.x = ((float)GET_MAINWIN_W / 2 - window->GetWidth() / 2);
	Pos1.y = ((float)GET_MAINWIN_H / 2 - window->GetHeight() / 2);
	window->SetAbsXY(Pos1.x, Pos1.y);
	AddWindow(window);
	GAMEIN->SetStallFindDlg((CStallFindDlg *)window);
	GAMEIN->GetStallFindDlg()->Linking();
}
void cWindowManager::CreateStreetStallDlg()
{
	cWindow * window = GetDlgInfoFromFile("./image/InterfaceScript/27.bin", "rb");
	VECTOR2 Pos1;
	Pos1.x = ((float)GET_MAINWIN_W / 2 - window->GetWidth() / 2);
	Pos1.y = ((float)GET_MAINWIN_H / 2 - window->GetHeight() / 2);
	window->SetAbsXY(Pos1.x, Pos1.y);
	AddWindow(window);
	GAMEIN->SetStreetStallDialog((CStreetStall *)window);
	GAMEIN->GetStreetStallDialog()->Linking();
}
void cWindowManager::CreateNpcScriptDlg()
{
	cWindow * window = GetDlgInfoFromFile("./image/InterfaceScript/31.bin", "rb");
	VECTOR2 Pos1;
	Pos1.x = ((float)GET_MAINWIN_W / 2 - window->GetWidth() / 2);
	Pos1.y = ((float)GET_MAINWIN_H / 2 - window->GetHeight() / 2);
	window->SetAbsXY(Pos1.x, Pos1.y);
	AddWindow(window);
	GAMEIN->SetNpcScriptDialog((cNpcScriptDialog *)window);
	GAMEIN->GetNpcScriptDialog()->Linking();
}
void cWindowManager::CreateHelpDlg()
{
	cWindow * window = GetDlgInfoFromFile("./image/InterfaceScript/33.bin", "rb");
	AddWindow(window);
	GAMEIN->SetHelpDialog((cHelpDialog*)window);
	GAMEIN->GetHelpDialog()->Linking();
}
void cWindowManager::CreateDealDlg()
{
	cWindow * window = GetDlgInfoFromFile("./image/InterfaceScript/19.bin", "rb");
	VECTOR2 Pos1;
	Pos1.x = ((float)GET_MAINWIN_W / 2 - window->GetWidth() / 2);
	Pos1.y = ((float)GET_MAINWIN_H / 2 - window->GetHeight() / 2);
	window->SetAbsXY(Pos1.x, Pos1.y);
	AddWindow(window);
	GAMEIN->SetDealDialog((CDealDialog *)window);
	GAMEIN->GetDealDialog()->Linking();
	CMHFile fp;
	fp.Init("./Resource/DealItem.bin", "rb");
	GAMEIN->GetDealDialog()->LoadDealerItem(&fp);
	fp.Release();
}
void cWindowManager::CreateQuestTotalDlg()
{
	cWindow * window = GetDlgInfoFromFile("./image/InterfaceScript/QuestTotal.bin", "rb");
	VECTOR2 Pos1;
	Pos1.x = ((float)GET_MAINWIN_W / 2 - window->GetWidth() / 2);
	Pos1.y = ((float)GET_MAINWIN_H / 2 - window->GetHeight() / 2);
	window->SetAbsXY(Pos1.x, Pos1.y);
	AddWindow(window);
	CQuestTotalDialog * pDlg = (CQuestTotalDialog *)window;
	GAMEIN->SetQuestTotalDialog(pDlg);
	CQuestDialog* pQuestDlg = (CQuestDialog*)pDlg->GetTabSheet(0);
	GAMEIN->SetQuestDialog(pQuestDlg);
	pQuestDlg->Linking();
	CWantedDialog* pWantedDlg = (CWantedDialog*)pDlg->GetTabSheet(1);
	GAMEIN->SetWantedDialog(pWantedDlg);
	pWantedDlg->Linking();
	CJournalDialog* pJournalDlg = (CJournalDialog*)pDlg->GetTabSheet(2);
	GAMEIN->SetJournalDialog(pJournalDlg);
	pJournalDlg->Linking();

	CInsDGRankInfoDialog* pInsDGRankInfoDlg = (CInsDGRankInfoDialog*)pDlg->GetTabSheet(3);
	GAMEIN->SetInsDGRankInfoDialog(pInsDGRankInfoDlg);
	pInsDGRankInfoDlg->Linking();
}
void cWindowManager::CreateBailDlg()
{
	cWindow * window = GetDlgInfoFromFile("./image/InterfaceScript/bail.bin", "rb");
	VECTOR2 Pos1;
	Pos1.x = ((float)GET_MAINWIN_W / 2 - window->GetWidth() / 2);
	Pos1.y = ((float)GET_MAINWIN_H / 2 - window->GetHeight() / 2);
	window->SetAbsXY(Pos1.x, Pos1.y);
	AddWindow(window);
	CBailDialog* pDlg = (CBailDialog*)window;
	GAMEIN->SetBailDialog(pDlg);
	pDlg->Linking();
}
void cWindowManager::CreateEventNotifyDlg()
{
	cWindow * window = GetDlgInfoFromFile("./image/InterfaceScript/GMOpenTalk.bin", "rb");
	ASSERT(window);
	VECTOR2 Pos1;
	Pos1.x = ((float)GET_MAINWIN_W - window->GetWidth()) - 53;
	Pos1.y = ((float)GET_MAINWIN_H / 2 - window->GetHeight() / 2) - 98;
	window->SetAbsXY(Pos1.x, Pos1.y);
	AddWindow(window);
	CEventNotifyDialog* pDlg = (CEventNotifyDialog*)window;
	GAMEIN->SetEventNotifyDialog(pDlg);
	pDlg->Linking();
}
void cWindowManager::CreateVsyncNotifyDlg()
{
	cWindow * window = GetDlgInfoFromFile("./image/InterfaceScript/VsyncNotifyDLG.bin", "rb");
	ASSERT(window);
	VECTOR2 Pos1;
	Pos1.x = ((float)GET_MAINWIN_W - window->GetWidth()) - 53;
	Pos1.y = ((float)GET_MAINWIN_H / 2 - window->GetHeight() / 2) - 98;
	window->SetAbsXY(Pos1.x, Pos1.y);
	AddWindow(window);
	cVSyncDialog* pDlg = (cVSyncDialog*)window;
	GAMEIN->SetVsyncDialog(pDlg);
	pDlg->Linking();
}
#include "GameCheckManager.h"
void cWindowManager::CreateGoldShopDialog()
{
#ifndef _OLDGOLDDIALOG_
	cWindow * window = GetDlgInfoFromFile("./image/InterfaceScript/TCDL.bin", "rb");
#else
	cWindow * window = GetDlgInfoFromFile("./image/InterfaceScript/TCDL_OLD.bin", "rb");
#endif
	VECTOR2 Pos1;
	Pos1.x = ((float)GET_MAINWIN_W / 2 - window->GetWidth() / 2);
	Pos1.y = ((float)GET_MAINWIN_H / 2 - window->GetHeight() / 2);
	window->SetAbsXY(Pos1.x, Pos1.y);
	AddWindow(window);
	CGoldShopDialog* pDlg = (CGoldShopDialog*)window;
	GAMEIN->SetGoldShopDialog(pDlg);
	pDlg->Linking();
#ifndef _OLDGOLDDIALOG_
	GOLDSHOPDLG->CallBin();
#endif
	sGAMECHECK->LoadCheckList();
}
void cWindowManager::CreateGoldShopBuyDialog()
{
	cWindow * window = GetDlgInfoFromFile("./image/InterfaceScript/TCBuyDlg.bin", "rb");
	ASSERT(window);
	VECTOR2 Pos1;
	Pos1.x = ((float)GAMERESRCMNGR->m_GameDesc.dispInfo.dwWidth / 2 - window->GetWidth() / 2);
	Pos1.y = ((float)GAMERESRCMNGR->m_GameDesc.dispInfo.dwHeight / 2 - window->GetHeight() / 2);
	window->SetAbsXY(Pos1.x, Pos1.y);
	AddWindow(window);
	CGoldShopBuyDialog * pDlg = (CGoldShopBuyDialog*)window;
	GAMEIN->SetGoldShopBuyDialog(pDlg);
	pDlg->Linking();
}
void cWindowManager::CreateDissolutionDlg()
{
	cWindow * window = GetDlgInfoFromFile("./image/InterfaceScript/58.bin", "rb");
	VECTOR2 Pos1;
	Pos1.x = ((float)GET_MAINWIN_W / 2 - window->GetWidth() / 2);
	Pos1.y = ((float)GET_MAINWIN_H / 2 - window->GetHeight() / 2);
	window->SetAbsXY(Pos1.x, Pos1.y);
	AddWindow(window);
	CDissolutionDialog* pDlg = (CDissolutionDialog*)window;
	GAMEIN->SetDissolutionDialog(pDlg);
	pDlg->Linking();
}
void cWindowManager::CreateItemShopDlg()
{
	cWindow * window = GetDlgInfoFromFile("./image/InterfaceScript/ItemShop.bin", "rb");
	VECTOR2 Pos1;
	Pos1.x = ((float)GET_MAINWIN_W / 2 - window->GetWidth() / 2);
	Pos1.y = ((float)GET_MAINWIN_H / 2 - window->GetHeight() / 2);
	window->SetAbsXY(Pos1.x, Pos1.y);
	AddWindow(window);
	CItemShopDialog* pDlg = (CItemShopDialog*)window;
	GAMEIN->SetItemShopDialog(pDlg);
	if (pDlg)
		pDlg->Linking();
}
void cWindowManager::CreateMoveDlg()
{
	cWindow * window = GetDlgInfoFromFile("./image/InterfaceScript/SaveMove.bin", "rb");
	VECTOR2 Pos1;
	Pos1.x = ((float)GET_MAINWIN_W / 2 - window->GetWidth() / 2);
	Pos1.y = ((float)GET_MAINWIN_H / 2 - window->GetHeight() / 2);
	window->SetAbsXY(Pos1.x, Pos1.y);
	AddWindow(window);
	CMoveDialog* pDlg = (CMoveDialog*)window;
	GAMEIN->SetMoveDialog(pDlg);
	if (pDlg)
		pDlg->Linking();
}
void cWindowManager::CreatePointSaveDlg()
{
	cWindow * window = GetDlgInfoFromFile("./image/InterfaceScript/ChangeName.bin", "rb");
	VECTOR2 Pos1;
	Pos1.x = ((float)GET_MAINWIN_W / 2 - window->GetWidth() / 2);
	Pos1.y = ((float)GET_MAINWIN_H / 2 - window->GetHeight() / 2);
	window->SetAbsXY(Pos1.x, Pos1.y);
	AddWindow(window);
	CPointSaveDialog* pDlg = (CPointSaveDialog*)window;
	GAMEIN->SetPointSaveDialog(pDlg);
	if (pDlg)
		pDlg->Linking();
}
void cWindowManager::CreateSkPointDlg()
{
	cWindow * window = GetDlgInfoFromFile("./image/InterfaceScript/Skillpointagain.bin", "rb");
	AddWindow(window);
	CSkillPointRedist* pDlg = (CSkillPointRedist*)window;
	GAMEIN->SetSkPointDlg(pDlg);
	if (pDlg)
		pDlg->Linking();
}
void cWindowManager::CreateSkPointNotifyDlg()
{
	cWindow * window = GetDlgInfoFromFile("./image/InterfaceScript/Skillpointop.bin", "rb");
	AddWindow(window);
	CSkillPointNotify* pDlg = (CSkillPointNotify*)window;
	GAMEIN->SetSkPointNotifydlg(pDlg);
	if (pDlg)
	{
		pDlg->Linking();
		pDlg->InitTextArea();
	}
}
void cWindowManager::CreateShoutDlg()
{
	cWindow * window = GetDlgInfoFromFile("./image/InterfaceScript/ch_allworldcreate.bin", "rb");
	VECTOR2 Pos1;
	Pos1.x = window->GetAbsX();
	if (!GAMERESRCMNGR->IsFullScreen())
	{
		Pos1.y = window->GetAbsY() + ((float)GET_MAINWIN_H - MIDRSLTN_H);
	}
	else
	{
		Pos1.y = window->GetAbsY() + ((float)GET_MAINWIN_H - MIDRSLTN_H);
	}
	window->SetAbsXY(Pos1.x, Pos1.y);
	AddWindow(window);
	CShoutDialog* pDlg = (CShoutDialog*)window;
	GAMEIN->SetShoutDlg(pDlg);
	pDlg->Linking();
}
void cWindowManager::CreateShoutchatDlg()
{
	cWindow * window = GetDlgInfoFromFile("./image/InterfaceScript/ch_allworld.bin", "rb");
	VECTOR2 Pos1;
	Pos1.x = window->GetAbsX();
	if (!GAMERESRCMNGR->IsFullScreen())
	{
		Pos1.y = window->GetAbsY() + ((float)GET_MAINWIN_H - MIDRSLTN_H);
	}
	else
	{
		Pos1.y = window->GetAbsY() + ((float)GET_MAINWIN_H - MIDRSLTN_H);
	}
	window->SetAbsXY(Pos1.x, Pos1.y);
	AddWindow(window);
	CShoutchatDialog* pDlg = (CShoutchatDialog*)window;
	GAMEIN->SetShoutchatDlg(pDlg);
	pDlg->Linking();
}
void cWindowManager::CreateChaseDlg()
{
	cWindow * window = GetDlgInfoFromFile("./image/InterfaceScript/seechase.bin", "rb");
	AddWindow(window);
	CChaseDialog* pDlg = (CChaseDialog*)window;
	GAMEIN->SetChaseDlg(pDlg);
	pDlg->Linking();
}
void cWindowManager::CreateChaseinputDlg()
{
	cWindow * window = GetDlgInfoFromFile("./image/InterfaceScript/chase.bin", "rb");
	AddWindow(window);
	CChaseinputDialog* pDlg = (CChaseinputDialog*)window;
	GAMEIN->SetChaseinputDlg(pDlg);
	pDlg->Linking();
}
void cWindowManager::CreateNameChangeDlg()
{
	cWindow * window = GetDlgInfoFromFile("./image/InterfaceScript/chnamechange.bin", "rb");
	AddWindow(window);
	CNameChangeDialog* pDlg = (CNameChangeDialog*)window;
	GAMEIN->SetNameChangeDlg(pDlg);
	pDlg->Linking();
}
void cWindowManager::CreateNameChangeNotifyDlg()
{
	cWindow * window = GetDlgInfoFromFile("./image/InterfaceScript/opchnamechange.bin", "rb");
	AddWindow(window);
	CNameChangeNotifyDlg* pDlg = (CNameChangeNotifyDlg*)window;
	GAMEIN->SetNameChangeNotifyDlg(pDlg);
}
void cWindowManager::CreateGTRegistDlg()
{
	cWindow * window = GetDlgInfoFromFile("./image/InterfaceScript/GDTENTRY.bin", "rb");
	VECTOR2 Pos1;
	Pos1.x = ((float)GET_MAINWIN_W / 2 - window->GetWidth() / 2);
	Pos1.y = ((float)GET_MAINWIN_H / 2 - window->GetHeight() / 2);
	window->SetAbsXY(Pos1.x, Pos1.y);
	AddWindow(window);
	CGTRegistDialog* pDlg = (CGTRegistDialog*)window;
	GAMEIN->SetGTRegistDlg(pDlg);
	pDlg->Linking();
}
void cWindowManager::CreateGTRegistcancelDlg()
{
	cWindow * window = GetDlgInfoFromFile("./image/InterfaceScript/GDTCANCEL.bin", "rb");
	VECTOR2 Pos1;
	Pos1.x = ((float)GET_MAINWIN_W / 2 - window->GetWidth() / 2);
	Pos1.y = ((float)GET_MAINWIN_H / 2 - window->GetHeight() / 2);
	window->SetAbsXY(Pos1.x, Pos1.y);
	AddWindow(window);
	CGTRegistcancelDialog* pDlg = (CGTRegistcancelDialog*)window;
	GAMEIN->SetGTRegistcancelDlg(pDlg);
	pDlg->Linking();
}
void cWindowManager::CreateGTStandingDlg()
{
	cWindow * window = GetDlgInfoFromFile("./image/InterfaceScript/GDTournament.bin", "rb");
	VECTOR2 Pos1;
	Pos1.x = ((float)GET_MAINWIN_W / 2 - window->GetWidth() / 2);
	Pos1.y = ((float)GET_MAINWIN_H / 2 - window->GetHeight() / 2);
	window->SetAbsXY(Pos1.x, Pos1.y);
	AddWindow(window);
	CGTStandingDialog* pDlg = (CGTStandingDialog*)window;
	GAMEIN->SetGTStandingDlg(pDlg);
	pDlg->Linking();
}
void cWindowManager::CreateGTBattleListDlg()
{
	cWindow * window = GetDlgInfoFromFile("./image/InterfaceScript/GDViewTournament.bin", "rb");
	VECTOR2 Pos1;
	Pos1.x = ((float)GET_MAINWIN_W / 2 - window->GetWidth() / 2);
	Pos1.y = ((float)GET_MAINWIN_H / 2 - window->GetHeight() / 2);
	window->SetAbsXY(Pos1.x, Pos1.y);
	AddWindow(window);
	CGTBattleListDialog* pDlg = (CGTBattleListDialog*)window;
	GAMEIN->SetGTBattleListDlg(pDlg);
	pDlg->Linking();
}
void cWindowManager::CreateGTScoreInfoDlg()
{
	cWindow * window = GetDlgInfoFromFile("./image/InterfaceScript/EventMapCount.bin", "rb");
	VECTOR2 Pos1;
	Pos1.x = ((float)GET_MAINWIN_W - window->GetWidth());
	Pos1.y = window->GetAbsY();
	window->SetAbsXY(Pos1.x, Pos1.y);
	AddWindow(window);
	CGTScoreInfoDialog* pDlg = (CGTScoreInfoDialog*)window;
	GAMEIN->SetGTScoreInfoDlg(pDlg);
	pDlg->Linking();
}
void cWindowManager::CreateSeigeWarDlg()
{
	cWindow* window = NULL;
	window = GetDlgInfoFromFile("./image/InterfaceScript/SWTimeReg.bin", "rb");
	AddWindow(window);
	CSWTimeRegDlg* pTimeRegDlg = (CSWTimeRegDlg*)window;
	GAMEIN->SetSWTimeRegDlg(pTimeRegDlg);
	pTimeRegDlg->Linking();


	window = GetDlgInfoFromFile("./image/InterfaceScript/SWProtectReg.bin", "rb");
	AddWindow(window);
	CSWProtectRegDlg* pProRegDlg = (CSWProtectRegDlg*)window;
	GAMEIN->SetSWProtectRegDlg(pProRegDlg);
	pProRegDlg->Linking();


	window = GetDlgInfoFromFile("./image/InterfaceScript/SWInfo.bin", "rb");
	AddWindow(window);
	CSWInfoDlg* pInfoDlg = (CSWInfoDlg*)window;
	GAMEIN->SetSWInfoDlg(pInfoDlg);
	pInfoDlg->Linking();


	window = GetDlgInfoFromFile("./image/InterfaceScript/SWProfit.bin", "rb");
	AddWindow(window);
	CSWProfitDlg* pProfitDlg = (CSWProfitDlg*)window;
	GAMEIN->SetSWProfitDlg(pProfitDlg);
	pProfitDlg->Linking();


	window = GetDlgInfoFromFile("./image/InterfaceScript/Engrave.bin", "rb");
	AddWindow(window);
	CSWEngraveDialog* pEngraveDlg = (CSWEngraveDialog*)window;
	GAMEIN->SetSWEngraveDlg(pEngraveDlg);
	pEngraveDlg->Linking();


	window = GetDlgInfoFromFile("./image/InterfaceScript/SWCount.bin", "rb");
	AddWindow(window);
	CSWTimeDialog* pSWTimeDlg = (CSWTimeDialog*)window;
	GAMEIN->SetSWTimeDlg(pSWTimeDlg);
	pSWTimeDlg->Linking();


	window = GetDlgInfoFromFile("./image/InterfaceScript/SWStartTime.bin", "rb");
	AddWindow(window);
	CSWStartTimeDialog* pSTDlg = (CSWStartTimeDialog*)window;
	GAMEIN->SetSWStartTimeDlg(pSTDlg);
	pSTDlg->Linking();


	window = GetDlgInfoFromFile("./image/InterfaceScript/FortWarEngraveDlg.bin", "rb");
	AddWindow(window);
	CFWEngraveDialog* pFWEngraveDlg = (CFWEngraveDialog*)window;
	GAMEIN->SetFWEngraveDlg(pFWEngraveDlg);
	pFWEngraveDlg->Linking();


	window = GetDlgInfoFromFile("./image/InterfaceScript/FortWarTimeDlg.bin", "rb");
	AddWindow(window);
	CFWTimeDialog* pFWTimeDlg = (CFWTimeDialog*)window;
	GAMEIN->SetFWTimeDlg(pFWTimeDlg);
	pFWTimeDlg->Linking();


	window = GetDlgInfoFromFile("./image/InterfaceScript/FortWarWareHouseDlg.bin", "rb");
	AddWindow(window);
	CFWWareHouseDialog* pFWWareHouseDlg = (CFWWareHouseDialog*)window;
	GAMEIN->SetFWWareHouseDlg(pFWWareHouseDlg);
	pFWWareHouseDlg->Linking();
}
void cWindowManager::CreateCharChangeDlg()
{
	//cWindow* window = NULL;
	cWindow* window = GetDlgInfoFromFile("./image/InterfaceScript/CharChange.bin", "rb");
	VECTOR2 Pos1;
	Pos1.x = ((float)GET_MAINWIN_W / 2 - window->GetWidth() / 2) - 300;
	Pos1.y = ((float)GET_MAINWIN_H / 2 - window->GetHeight() / 2);
	window->SetAbsXY(Pos1.x, Pos1.y);
	AddWindow(window);
	CCharChangeDlg* pDlg = (CCharChangeDlg*)window;
	GAMEIN->SetCharChangeDlg(pDlg);
	pDlg->Linking();
}
void cWindowManager::CreateItemSealDlg()
{
	////cWindow* window = NULL;
	cWindow* window = GetDlgInfoFromFile("./image/InterfaceScript/ItemLock.bin", "rb");
	VECTOR2 Pos1;
	Pos1.x = ((float)GET_MAINWIN_W / 2 - window->GetWidth() / 2);
	Pos1.y = ((float)GET_MAINWIN_H / 2 - window->GetHeight() / 2);
	window->SetAbsXY(Pos1.x, Pos1.y);
	AddWindow(window);
	CSealDialog* pDlg = (CSealDialog*)window;
	GAMEIN->SetSealDlg(pDlg);
	pDlg->Linking();
}
void cWindowManager::CreateItemLockDlg()
{
	////cWindow* window = NULL;
	cWindow* window = GetDlgInfoFromFile("./image/InterfaceScript/ItemLockEx.bin", "rb");
	VECTOR2 Pos1;
	Pos1.x = ((float)GET_MAINWIN_W / 2 - window->GetWidth() / 2);
	Pos1.y = ((float)GET_MAINWIN_H / 2 - window->GetHeight() / 2);
	window->SetAbsXY(Pos1.x, Pos1.y);
	AddWindow(window);
	CItemLockDialog* pDlg = (CItemLockDialog*)window;
	GAMEIN->SetItemLockDialog(pDlg);
	pDlg->Linking();
}
void cWindowManager::CreateItemUnLockDlg()
{
	////cWindow* window = NULL;
	cWindow* window = GetDlgInfoFromFile("./image/InterfaceScript/ItemUnLockEx.bin", "rb");
	VECTOR2 Pos1;
	Pos1.x = ((float)GET_MAINWIN_W / 2 - window->GetWidth() / 2);
	Pos1.y = ((float)GET_MAINWIN_H / 2 - window->GetHeight() / 2);
	window->SetAbsXY(Pos1.x, Pos1.y);
	AddWindow(window);
	CItemUnLockDialog* pDlg = (CItemUnLockDialog*)window;
	GAMEIN->SetItemUnLockDialog(pDlg);
	pDlg->Linking();
}
void cWindowManager::CreateItemGambleDlg()
{
	////cWindow* window = NULL;
	cWindow* window = GetDlgInfoFromFile("./image/InterfaceScript/ItemGambleDlg.bin", "rb");
	VECTOR2 Pos1;
	Pos1.x = ((float)GET_MAINWIN_W / 2) - (window->GetWidth() / 2);
	Pos1.y = ((float)GET_MAINWIN_H / 2) - (window->GetHeight() / 2);
	window->SetAbsXY(Pos1.x, Pos1.y);
	AddWindow(window);
	CItemGambleDialog* pDlg = (CItemGambleDialog*)window;
	GAMEIN->SetItemGambleDialog(pDlg);
	pDlg->Linking();
}
void cWindowManager::CreateChangeJobDlg()
{
	////cWindow* window = NULL;
	cWindow* window = GetDlgInfoFromFile("./image/InterfaceScript/Changejob.bin", "rb");
	AddWindow(window);
	CChangeJobDialog* pDlg = (CChangeJobDialog*)window;
	GAMEIN->SetChangeJobDlg(pDlg);
}
void cWindowManager::CreateReinforceResetDlg()
{
	////cWindow* window = NULL;
	cWindow* window = GetDlgInfoFromFile("./image/InterfaceScript/ReinforceDefault.bin", "rb");
	AddWindow(window);
	if (window)
	{
		CReinforceResetDlg* pDlg = (CReinforceResetDlg*)window;
		pDlg->Linking();
		GAMEIN->SetReinforceResetDlg(pDlg);
	}
}
void cWindowManager::CreateRareCreateDlg()
{
	////cWindow* window = NULL;
	cWindow* window = GetDlgInfoFromFile("./image/InterfaceScript/RareCreate.bin", "rb");
	VECTOR2 Pos1;
	Pos1.x = ((float)GET_MAINWIN_W / 2 - window->GetWidth() / 2);
	Pos1.y = ((float)GET_MAINWIN_H / 2 - window->GetHeight() / 2);
	window->SetAbsXY(Pos1.x, Pos1.y);
	AddWindow(window);
	CRareCreateDialog* pDlg = (CRareCreateDialog*)window;
	pDlg->Linking();
	GAMEIN->SetRareCreateDialog(pDlg);
}
void cWindowManager::CreateReinforceDataGuideDlg()
{
	////cWindow* window = NULL;
	cWindow* window = GetDlgInfoFromFile("./image/InterfaceScript/RFDataGuide.bin", "rb");
	VECTOR2 Pos1;
	Pos1.x = ((float)GET_MAINWIN_W / 2 - window->GetWidth() / 2);
	Pos1.y = ((float)GET_MAINWIN_H / 2 - window->GetHeight() / 2);
	window->SetAbsXY(Pos1.x, Pos1.y);
	AddWindow(window);
	CReinforceDataGuideDlg* pDlg = (CReinforceDataGuideDlg*)window;
	pDlg->Linking();
	GAMEIN->SetReinforceDataGuideDlg(pDlg);
}
void cWindowManager::CreateTipBrowserDlgDlg()
{
	////cWindow* window = NULL;
	cWindow* window = GetDlgInfoFromFile("./image/InterfaceScript/TipBrowser.bin", "rb");
	VECTOR2 Pos1;
	Pos1.x = ((float)GET_MAINWIN_W / 2 - window->GetWidth() / 2);
	Pos1.y = ((float)GET_MAINWIN_H / 2 - window->GetHeight() / 2);
	window->SetAbsXY(Pos1.x, Pos1.y);
	AddWindow(window);
	CTipBrowserDlg* pDlg = (CTipBrowserDlg*)window;
	pDlg->Linking();
	GAMEIN->SetTipBrowserDlg(pDlg);
}
void cWindowManager::CreateKeySettingTipDlg()
{
	////cWindow* window = NULL;
	cWindow* window = GetDlgInfoFromFile("./image/InterfaceScript/KeySetting.bin", "rb");
	VECTOR2 Pos1;
	Pos1.x = ((float)GET_MAINWIN_W / 2 - window->GetWidth() / 2);
	Pos1.y = ((float)GET_MAINWIN_H / 2 - window->GetHeight() / 2);
	window->SetAbsXY(Pos1.x, Pos1.y);
	AddWindow(window);
	CKeySettingTipDlg* pDlg = (CKeySettingTipDlg*)window;
	pDlg->Linking();
	GAMEIN->SetKeySettingTipDlg(pDlg);
}
void cWindowManager::CreateGuildNoteDlg()
{
	////cWindow* window = NULL;
	cWindow* window = GetDlgInfoFromFile("./image/InterfaceScript/GuildNote.bin", "rb");
	VECTOR2 Pos1;
	Pos1.x = ((float)GET_MAINWIN_W / 2 - window->GetWidth() / 2);
	Pos1.y = ((float)GET_MAINWIN_H / 2 - window->GetHeight() / 2);
	window->SetAbsXY(Pos1.x, Pos1.y);
	AddWindow(window);
	CGuildNoteDlg* pDlg = (CGuildNoteDlg*)window;
	pDlg->Linking();
	GAMEIN->SetGuildNoteDlg(pDlg);
}
void cWindowManager::CreateUnionNoteDlg()
{
	////cWindow* window = NULL;
	cWindow* window = GetDlgInfoFromFile("./image/InterfaceScript/AllyNote.bin", "rb");
	AddWindow(window);
	CUnionNoteDlg* pDlg = (CUnionNoteDlg*)window;
	pDlg->Linking();
	GAMEIN->SetUnionNoteDlg(pDlg);
}
void cWindowManager::CreateGuildNoticeDlg()
{
	////cWindow* window = NULL;
	cWindow* window = GetDlgInfoFromFile("./image/InterfaceScript/GuildNotice.bin", "rb");
	VECTOR2 Pos1;
	Pos1.x = ((float)GET_MAINWIN_W / 2 - window->GetWidth() / 2);
	Pos1.y = ((float)GET_MAINWIN_H / 2 - window->GetHeight() / 2);
	window->SetAbsXY(Pos1.x, Pos1.y);
	AddWindow(window);
	CGuildNoticeDlg* pDlg = (CGuildNoticeDlg*)window;
	pDlg->Linking();
	GAMEIN->SetGuildNoticeDlg(pDlg);
}
void cWindowManager::CreateSkillOptionChangeDlg()
{
	////cWindow* window = NULL;
	cWindow* window = GetDlgInfoFromFile("./image/InterfaceScript/SkillTrans.bin", "rb");
	AddWindow(window);
	CSkillOptionChangeDlg* pDlg = (CSkillOptionChangeDlg*)window;
	pDlg->Linking();
	GAMEIN->SetSkillOptionChangeDlg(pDlg);
}
void cWindowManager::CreateSkillOptionClearDlg()
{
	////cWindow* window = NULL;
	cWindow* window = GetDlgInfoFromFile("./image/InterfaceScript/TransDefault.bin", "rb");
	AddWindow(window);
	CSkillOptionClearDlg* pDlg = (CSkillOptionClearDlg*)window;
	pDlg->Linking();
	GAMEIN->SetSkillOptionClearDlg(pDlg);
}
void cWindowManager::ScreenShotDlg()
{
	cWindow * window = GetDlgInfoFromFile("./image/InterfaceScript/screenshotdlg.bin", "rb");
	//VECTOR2 Pos1;
	//Pos1.x = window->GetAbsX() + (GET_MAINWIN_W - MIDRSLTN_W);
	//Pos1.y = window->GetAbsY() + ((float)GET_MAINWIN_H - MIDRSLTN_H);
	//window->SetAbsXY(Pos1.x, Pos1.y);
	AddWindow(window);
	CScreenShotDlg* pDlg = (CScreenShotDlg*)window;
	GAMEIN->SetScreenShotDlg(pDlg);
	pDlg->Linking();
}
void cWindowManager::CreateUniqueItemMixProgressBarDlg()
{
	cWindow * window = GetDlgInfoFromFile("./image/InterfaceScript/UniqueItemProgressBarDlg.bin", "rb");
	AddWindow(window);
	GAMEIN->SetUniqueItemMixProgressBarDlg((CUniqueItemMixProgressBarDlg *)window);
	GAMEIN->GetUniqueItemMixProgressBarDlg()->Linking();
}
void cWindowManager::CreateTitanMixProgressBarDlg()
{
	cWindow * window = GetDlgInfoFromFile("./image/InterfaceScript/TitanmixProgressBarDlg.bin", "rb");
	AddWindow(window);
	GAMEIN->SetTitanMixProgressBarDlg((CTitanMixProgressBarDlg *)window);
	GAMEIN->GetTitanMixProgressBarDlg()->Linking();
}
void cWindowManager::CreateTitanPartsProgressBarDlg()
{
	cWindow * window = GetDlgInfoFromFile("./image/InterfaceScript/TitanPartsProgressBarDlg.bin", "rb");
	AddWindow(window);
	GAMEIN->SetTitanPartsProgressBarDlg((CTitanPartsProgressBarDlg *)window);
	GAMEIN->GetTitanPartsProgressBarDlg()->Linking();
}
void cWindowManager::CreateSkinSelectDlg()
{
	cWindow * window = GetDlgInfoFromFile("./image/InterfaceScript/SkinSelectDlg.bin", "rb");
	AddWindow(window);
	GAMEIN->SetSkinSelectDlg((CSkinSelectDialog *)window);
	GAMEIN->GetSkinSelectDlg()->Linking();
}
void cWindowManager::CreateCostumeSkinSelectDlg()
{
	cWindow * window = GetDlgInfoFromFile("./image/InterfaceScript/CostumeSkinSelectDlg.bin", "rb");
	AddWindow(window);
	GAMEIN->SetCostumeSkinSelectDlg((CCostumeSkinSelectDialog *)window);
	GAMEIN->GetCostumeSkinSelectDlg()->Linking();
}
void cWindowManager::CreateSkillPointResetDlg()
{
	cWindow* window = GetDlgInfoFromFile("./image/InterfaceScript/SkillPointReset.bin", "rb");
	AddWindow(window);
	GAMEIN->SetSkillPointResetDlg((CSkillPointResetDlg*)window);
	GAMEIN->GetSkillPointResetDlg()->Linking();
}
void cWindowManager::CreateStreetStallItemViewDlg()
{
	cWindow* window = GetDlgInfoFromFile("./image/InterfaceScript/SearchChr.bin", "rb");
	AddWindow(window);
	GAMEIN->SetStreetStallItemViewDlg((CStreetStallItemViewDlg*)window);
	GAMEIN->GetStreetStallItemViewDlg()->Linking();
}
void cWindowManager::CreatePartyMatchingDlg()
{
	cWindow * window = GetDlgInfoFromFile("./image/InterfaceScript/PartyMatchingDlg.bin", "rb");
	VECTOR2 Pos3;
	Pos3.x = ((float)GET_MAINWIN_W / 2 - window->GetWidth() / 2);
	Pos3.y = ((float)GET_MAINWIN_H / 2 - window->GetHeight() / 2);
	window->SetAbsXY(Pos3.x, Pos3.y);
	AddWindow(window);
	GAMEIN->SetPartyMatchingDlg((CPartyMatchingDlg *)window);
	GAMEIN->GetPartyMatchingDlg()->Linking();
}
void cWindowManager::CreateNumberPadDlg()
{
	cWindow * window = GetDlgInfoFromFile("./image/InterfaceScript/NumberPad.bin", "rb");
	VECTOR2 Pos1;
	Pos1.x = window->GetAbsX() + ((float)GAMERESRCMNGR->m_GameDesc.dispInfo.dwWidth - MIDRSLTN_W) / 2;
	Pos1.y = window->GetAbsY() + ((float)GAMERESRCMNGR->m_GameDesc.dispInfo.dwHeight - MIDRSLTN_H) / 2;
	window->SetAbsXY(Pos1.x, Pos1.y);
	AddWindow(window);
	GAMEIN->SetNumberPadDlg((CNumberPadDialog *)window);
	GAMEIN->GetNumberPadDlg()->Linking();
}
void cWindowManager::AfterInit()
{
	PTRLISTPOS pos = m_pWindowList->GetHeadPosition();
	while (pos)
	{
		cDialog * win = (cDialog *)m_pWindowList->GetNext(pos);
		if (win->GetType() & WT_ICONDIALOG || win->GetType() & WT_ICONGRIDDIALOG)
			m_pIconDlgListRef->AddTail(win);
		else if (win->GetType() & WT_TABDIALOG)
		{
			cTabDialog * tabDlg = (cTabDialog *)win;
			int i = 0;
			for (int i = 0; i < tabDlg->GetTabNum(); ++i)
			{
				if (tabDlg->GetTabSheet(i)->GetType() & WT_ICONDIALOG || tabDlg->GetTabSheet(i)->GetType() & WT_ICONGRIDDIALOG)
					m_pIconDlgListRef->AddTail(tabDlg->GetTabSheet(i));
			}
		}
		PTRLISTPOS pos2 = win->m_pComponentList->GetHeadPosition();
		while (pos2)
		{
			cWindow * win2 = (cWindow *)win->m_pComponentList->GetNext(pos2);
			if (win2->GetType() == WT_EDITBOX)
			{
				m_pEditBoxListRef->AddTail((void *)win2);
			}
			else if (win2->GetType() & WT_ICONDIALOG || win2->GetType() & WT_ICONGRIDDIALOG)
			{
				m_pIconDlgListRef->AddTail(win2);
			}
		}
	}
}
void cWindowManager::KeyboardInput(CKeyboard* keyInfo)
{
	PTRLISTPOS pos = m_pWindowList->GetTailPosition();
	while (pos)
	{
		cWindow * window = (cWindow *)m_pWindowList->GetAt(pos);
		if (window->IsActive() && !window->IsDepend())
		{
			DWORD Action = window->ActionKeyboardEvent(keyInfo);
			if (Action & WE_CLOSEWINDOW)
				window->SetActive(FALSE);
			if (Action != WE_NULL) break;
		}
		m_pWindowList->GetPrev(pos);
	}
}
void cWindowManager::SetNextEditFocus()
{
	if (m_pFocusedEdit)
	{
		PTRLISTPOS posCur = m_pEditBoxListRef->Find(m_pFocusedEdit);
		PTRLISTPOS pos = posCur;
		m_pEditBoxListRef->GetNext(pos);
		while (pos != posCur)
		{
			if (pos == NULL)
				pos = m_pEditBoxListRef->GetHeadPosition();
			cEditBox * win = (cEditBox *)m_pEditBoxListRef->GetNext(pos);
			if (win->IsActive() && win->GetParent() == m_pFocusedEdit->GetParent())
			{
				m_pFocusedEdit->SetFocus(FALSE);
				win->SetFocusEdit(TRUE);
				break;
			}
		}
	}
}
void cWindowManager::BackDragWindow()
{
	cDialog * srcDlg = GetWindowForID(m_id);
	if (!srcDlg) return;
	srcDlg->SetAbsXY((LONG)m_posBeforeDrag.x, (LONG)m_posBeforeDrag.y);
	DragWindowNull();
}
void cWindowManager::Preprocess()
{
	if (m_OldDragFlag)
	{
		BOOL LeftUp = MOUSE->LButtonUp();
		if (LeftUp)
		{
			LONG ClickUpX = MOUSE->GetMouseEventX();
			LONG ClickUpY = MOUSE->GetMouseEventY();
			if (cbDragnDropProcess)
				cbDragnDropProcess(ClickUpX, ClickUpY, m_id, (LONG)m_posBeforeDrag.x, (LONG)m_posBeforeDrag.y);
			SetDragEnd();
		}
		else if (m_pDragDialog)
		{
			LONG x = MOUSE->GetMouseX();
			LONG y = MOUSE->GetMouseY();
			m_pDragDialog->SetAbsXY(x - m_OldX, y - m_OldY);
			cWindow* window = GetWindowForIDEx(m_id);
			//if (window->GetType() == WT_CHARINFODIALOG)
			//	GAMEIN->GetCharacterPvPDialog()->SetAbsXY(m_pDragDialog->GetAbsX() + 303, m_pDragDialog->GetAbsY() + 18);

			DISPLAY_INFO	dispInfo;
			GAMERESRCMNGR->GetDispInfo(&dispInfo);
			RECT rect = { 0, 0, dispInfo.dwWidth, dispInfo.dwHeight };
			VECTOR2 magPos;
			magPos.x = m_pDragDialog->GetAbsX();
			magPos.y = m_pDragDialog->GetAbsY();
			if (cAnimationManager::MagProcess(&rect, (cWindow*)m_pDragDialog, &magPos))
				m_pDragDialog->SetAbsXY(magPos.x, magPos.y);
			if (cJackAnimationManager::MagProcess(&rect, (cWindow*)m_pDragDialog, &magPos))
				m_pDragDialog->SetAbsXY(magPos.x, magPos.y);
			if (window->GetType() == WT_CHARINFODIALOG)
				GAMEIN->GetCharacterPvPDialog()->SetAbsXY(magPos.x + 303, magPos.y + 18);
		}
	}
}
void cWindowManager::AddListDestroyWindow(cWindow * window)
{
	if (window == NULL) return;
	PTRLISTSEARCHSTART((*m_pDestroyWindowRef), cWindow*, pWindow)
	if (pWindow == window)
		return;
	PTRLISTSEARCHEND
		m_pDestroyWindowRef->AddTail(window);
	window->SetDestroy();
}
void cWindowManager::DestroyWindowProcess()
{
	PTRLISTPOS pos = m_pDestroyWindowRef->GetHeadPosition();
	while (pos)
	{
		cWindow * window = (cWindow *)m_pDestroyWindowRef->GetNext(pos);
		DeleteWindow(window);
	}
	m_pDestroyWindowRef->RemoveAll();
}
void cWindowManager::OnWindowEvent(LONG lId, void * p, DWORD we)
{
	if (we & WE_CLOSEWINDOW)
	{
		GAMEIN->GetMainInterfaceDialog()->PushMenuID(lId, FALSE);
	}
}
void cWindowManager::Process()
{
	ANIMGR->FadeProcess();
	PTRLISTPOS pos = m_pPlayerChatTooltip->GetHeadPosition();
	while (pos)
	{
		cWindow * window = (cWindow *)m_pPlayerChatTooltip->GetAt(pos);
		window->ActionEvent(MOUSE);
		if (!window->IsActive())
			m_pPlayerChatTooltip->RemoveAt(pos);
		m_pPlayerChatTooltip->GetNext(pos);
	}
}
void cWindowManager::MouseInput(CMouse* pMouse)
{
	m_bMouseOverUsed = FALSE;
	m_bMouseDownUsed = FALSE;
	m_bMouseUpUsed = FALSE;
	cWindow* pMouseOverWindow = NULL;
	m_MouseInputProcessed = FALSE;
	DestroyWindowProcess();
	Preprocess();
	cWindow * TopWindow = NULL;
	PTRLISTPOS TopPos = NULL;
	PTRLISTPOS pos = m_pWindowList->GetTailPosition();
	cWindow * window;
	while (pos)
	{
		window = (cWindow *)m_pWindowList->GetAt(pos);
		LONG Action = 0;
		if (window->IsActive())
		{
			if (!window->IsDepend())
				Action = window->ActionEvent(pMouse);
		}
		if ((Action & WE_CLOSEWINDOW))
			window->SetActive(FALSE);
		else if (Action & WE_TOPWINDOW)
		{
			TopPos = pos;
			TopWindow = window;
		}
		if (Action & WE_LBTNCLICK || Action & WE_RBTNCLICK ||
			Action & WE_LBTNDBLCLICK || Action & WE_RBTNDBLCLICK ||
			Action & WE_COMBOBOXSELECT)
		{
			m_MouseInputProcessed = TRUE;
			m_bMouseDownUsed = TRUE;
		}
		if (Action & WE_MOUSEOVER)
		{
			m_MouseInputProcessed = TRUE;
			m_bMouseOverUsed = TRUE;
			pMouseOverWindow = window;
		}
		m_pWindowList->GetPrev(pos);
	}
	if (TopWindow && TopPos)
	{
		m_pWindowList->RemoveAt(TopPos);
		if (TopWindow->IsAlwaysTop())
			m_pWindowList->AddTail(TopWindow);
		else
		{
			pos = m_pWindowList->GetTailPosition();
			while (pos)
			{
				cWindow* pWindow = (cWindow*)m_pWindowList->GetAt(pos);
				if (pWindow)
				if (!pWindow->IsAlwaysTop())	break;
				m_pWindowList->GetPrev(pos);
			}
			if (pos)
				m_pWindowList->InsertAfter(pos, TopWindow);
			else
				m_pWindowList->AddHead(TopWindow);
		}
	}
	if (m_pGuageWindow)
	{
		m_pGuageWindow->ActionEvent(pMouse);
	}
	if (m_pMonsterGuageWindow)
	{
		m_pMonsterGuageWindow->ActionEvent(pMouse);
	}
	if (m_pMousePointWindow)
	{
		m_pMousePointWindow->SetAbsXY(pMouse->GetMouseX(), pMouse->GetMouseY());
	}
}
#include <iostream>
#include "MHTimeManager.h"
extern int	PingMs;
void cWindowManager::Render()
{
	//extern double CurFPS;
	//CurFPS = CMHTimeManager::Instance().CalculateFrameRate();

	PTRLISTPOS pos = NULL;
	if (m_pScreenTextList != NULL)
	{
		pos = m_pScreenTextList->GetHeadPosition();
		while (pos)
		{
			cMultiLineText* mlwindow = (cMultiLineText*)m_pScreenTextList->GetNext(pos);
			mlwindow->Render();
		}
	}
	if (m_pPlayerChatTooltip != NULL)
	{
		pos = m_pPlayerChatTooltip->GetHeadPosition();
		while (pos)
		{
			cWindow* window = (cWindow*)m_pPlayerChatTooltip->GetNext(pos);
			window->Render();
		}
	}
	SetToolTipWindow(NULL);
	SetRelationTipWindow(NULL);
	SetRelationTipWindow1(NULL);

	SetStaticToolTip(NULL);

	STATUSICONDLG->Render();
	HIDEBUFFDLG->Render();
	
	if (m_pWindowList != NULL)
	{
		pos = m_pWindowList->GetHeadPosition();
		if (m_OldDragFlag)
		{
			BOOL bRenderDragDlg = FALSE;
			while (pos)
			{
				cWindow* window = (cWindow *)m_pWindowList->GetNext(pos);
				if (window->IsAlwaysTop())
				if (window->IsActive())
				if (!bRenderDragDlg)
				if (!m_pDragDialog->IsAlwaysTop())
				{
					m_pDragDialog->Render();
					bRenderDragDlg = TRUE;
				}
				if (!window->IsDepend() && window->GetID() != m_id)
					window->Render();
			}
			if (!bRenderDragDlg && m_pDragDialog)
				m_pDragDialog->Render();
		}
		else
		{
			while (pos)
			{
				cWindow* window = (cWindow *)m_pWindowList->GetNext(pos);
				if (!window->IsDepend())
					window->Render();
			}
		}
	}
	if (m_pStaticToolTip)
	{
		m_pStaticToolTip->Render();
	}
	if (m_pSendItemInfo)
	{
		m_pSendItemInfo->Render();
	}
	if (!m_OldDragFlag)
	if (m_pToolTipWindow)
	{
		m_pToolTipWindow->SetShowCurItem(FALSE);
		m_pToolTipWindow->Render();
	}
	if (m_pRelationTip)
	{
		m_pRelationTip->Render();
	}
	if (m_pRelationTip1)
	{
		m_pRelationTip1->Render();
	}
	if (m_pGuageWindow)
		m_pGuageWindow->Render();
	if (m_pMonsterGuageWindow)
		m_pMonsterGuageWindow->Render();
	if (m_pMousePointWindow)
		m_pMousePointWindow->Render();

	if (MAP->IsMapKind(eSurvivalMap))
	{
		SVVMODEMGR->Render();
	}
	
	//ITEMLINKDLG->Render();








	
	if (g_bDisplayFPS)
	{
		if (g_bDisplayFPSWindownMgrBool) return;









		{
			cImage			m_JackIcon;
			SCRIPTMGR->GetImage(161, &m_JackIcon, PFT_JACKPATH);
			VECTOR2 vScale = { 1.0f, 1.0f };
			VECTOR2 pos = {  25, 25 };
			m_JackIcon.RenderSprite(&vScale, NULL, 0.0f, &pos, 0xffffffff);
		}
		RECT rect;		
		char temp[256];
		VECTOR3 pos;
		static int MinFPS = INT_MAX;
		static int MaxFPS = INT_MIN;
		//static int FinalFPS;

		HERO->GetPosition(&pos);

		SetRect(&rect, 30, 300, 500, 150);
		sprintf(temp, "[POSITION]\n\n\nx: %f z : %f", pos.x, pos.z);
		CFONT_OBJ->RenderFont(0, temp, strlen(temp), &rect, 0xffffffff);

		MHCAMERADESC* pcam = CAMERA->GetCameraDesc();
		SetRect(&rect, 30, 330, 500, 250);		
		sprintf(temp, "[CAMERA]\n\n\n\ncam_x: %f  cam_z: %f  cam_y: %f", pcam->m_CameraPos.x, pcam->m_CameraPos.z, pcam->m_CameraPos.y);
		CFONT_OBJ->RenderFont(0, temp, strlen(temp), &rect, 0xffffffff);

		SetRect(&rect, 30, 360, 500, 350);
		sprintf(temp, "[ANGLE]\n\n\n\n\nang_deg_x: %f  ang_deg_y: %f", CAMERA->GetCameraDesc()->m_AngleX.ToDeg(), CAMERA->GetCameraDesc()->m_AngleY.ToDeg());
		CFONT_OBJ->RenderFont(0, temp, strlen(temp), &rect, 0xffffffff);
		
		SetRect(&rect, 30, 390, 500, 450);
		CFONT_OBJ->RenderFont(0, DebugTickTemp, strlen(DebugTickTemp), &rect, 0xffffffff);

		SetRect(&rect, 30, 420, 500, 550);

		int data, num, count;
		data = (int)CurFPS;
		if (data < MinFPS)
			MinFPS = data;
		if (data > MaxFPS)
			MaxFPS = data;
		sprintf(temp, "[FRAME]\n\n\n\n\nmin:%d	current:%d	max:%d	", MinFPS, (int)CurFPS, MaxFPS);
		CFONT_OBJ->RenderFont(0, temp, strlen(temp), &rect, 0xffffffff);


		SetRect(&rect, 30, 450, 500, 650);
		sprintf(temp, "[NETWORK]\n\n\n\n%d ms", PingMs);
		CFONT_OBJ->RenderFont(0, temp, strlen(temp), &rect, 0xffffffff);
		
		
	}

	//MAP->RenderTileData_Debug();
}

void cWindowManager::DestroyWindowAll()
{
	if (!m_pWindowList)return;

	if (g_bDisplayFPS)
		g_bDisplayFPSWindownMgrBool = TRUE;
	DestroyWindowProcess();
	PTRLISTPOS pos = m_pWindowList->GetHeadPosition();
	while (pos)
	{
		cWindow * win = (cWindow *)m_pWindowList->GetNext(pos);
		SAFE_DELETE(win);
	}
	m_pWindowList->RemoveAll();
	if (m_pGuageWindow)
	{
		SAFE_DELETE(m_pGuageWindow);
	}
	if (m_pMonsterGuageWindow)
	{
		SAFE_DELETE(m_pMonsterGuageWindow);
	}
	DeleteEditBoxListAll();
	DeleteIconDlgListAll();
	DeleteChatTooltipListAll();
	m_pDragDialog = NULL;
}
void cWindowManager::CloseWindowForID(LONG id)
{
	if (!m_pWindowList) return;
	PTRLISTPOS pos = m_pWindowList->GetHeadPosition();
	while (pos)
	{
		cWindow * win = (cWindow *)m_pWindowList->GetNext(pos);
		if (win->GetID() == id)
		{
			win->SetActive(FALSE);
			break;
		}
	}
}
void cWindowManager::DeleteIconDlgListAll()
{
	if (!m_pIconDlgListRef)return;
	m_pIconDlgListRef->RemoveAll();
}
void cWindowManager::DeleteEditBoxListAll()
{
	if (!m_pEditBoxListRef)return;
	m_pEditBoxListRef->RemoveAll();
}
void cWindowManager::DeleteChatTooltipListAll()
{
	if (!m_pPlayerChatTooltip)return;
	m_pPlayerChatTooltip->RemoveAll();
}
void cWindowManager::DeleteWindowForID(LONG id)
{
	if (!m_pWindowList) return;
	PTRLISTPOS pos = m_pWindowList->GetHeadPosition();
	while (pos)
	{
		cWindow * win = (cWindow *)m_pWindowList->GetAt(pos);
		if (win->GetID() == id)
		{
			m_pWindowList->RemoveAt(pos);
			SAFE_DELETE(win);
			if (m_id == id) SetDragEnd();
			return;
		}
		m_pWindowList->GetNext(pos);
	}
}
void cWindowManager::DeleteWindow(cWindow* pWindow)
{
	if (!m_pWindowList) return;
	PTRLISTPOS pos = m_pWindowList->GetHeadPosition();
	while (pos)
	{
		cWindow* win = (cWindow *)m_pWindowList->GetAt(pos);
		if (win == pWindow)
		{
			m_pWindowList->RemoveAt(pos);
			if (m_id == win->GetID()) SetDragEnd();
			SAFE_DELETE(win);
			return;
		}
		m_pWindowList->GetNext(pos);
	}
}
void cWindowManager::RemoveChatTooltipWindow(LONG playerID)
{
	if (!m_pPlayerChatTooltip) return;
	PTRLISTPOS pos = m_pPlayerChatTooltip->GetHeadPosition();
	while (pos)
	{
		cWindow * win = (cWindow *)m_pPlayerChatTooltip->GetAt(pos);
		if (win->GetID() == playerID)
		{
			m_pPlayerChatTooltip->RemoveAt(pos);
			break;
		}
		m_pPlayerChatTooltip->GetNext(pos);
	}
	return;
}
void cWindowManager::RemoveChatTooltipWindow(cWindow* pWindow)
{
	if (m_pPlayerChatTooltip)
		m_pPlayerChatTooltip->Remove(pWindow);
}
cDialog * cWindowManager::GetWindowForID(LONG id)
{
	PTRLISTPOS pos = m_pWindowList->GetHeadPosition();
	while (pos)
	{
		cDialog * win = (cDialog *)m_pWindowList->GetNext(pos);
		if (win->GetID() == id)
			return win;
	}
	return NULL;
}
cWindow * cWindowManager::GetWindowForIDEx(LONG id)
{
	PTRLISTPOS pos = m_pWindowList->GetHeadPosition();
	while (pos)
	{
		cDialog * win = (cDialog *)m_pWindowList->GetNext(pos);
		if (win->GetID() == id)
			return (cWindow *)win;
		else
		{
			PTRLISTPOS pos2 = win->m_pComponentList->GetHeadPosition();
			while (pos2)
			{
				cWindow * win2 = (cWindow *)win->m_pComponentList->GetNext(pos2);
				if (win2->GetID() == id)
				{
					return win2;
				}
			}
		}
	}
	return NULL;
}
cDialog * cWindowManager::GetWindowForXY(LONG x, LONG y)
{
	PTRLISTPOS pos = m_pWindowList->GetHeadPosition();
	while (pos)
	{
		cDialog * win = (cDialog *)m_pWindowList->GetNext(pos);
		if (win->IsActive() == FALSE)
			continue;
		if (win->PtInWindow(x, y))
			return win;
	}
	return NULL;
}
cDialog * cWindowManager::GetWindowForXYExceptIcon(LONG x, LONG y)
{
	PTRLISTPOS pos = m_pWindowList->GetTailPosition();
	while (pos)
	{
		cDialog * win = (cDialog *)m_pWindowList->GetPrev(pos);
		if (win->IsActive() == FALSE || win->GetType() & WT_ICON)
			continue;
		if (win->PtInWindow(x, y))
			return win;
	}
	return NULL;
}
cDialog * cWindowManager::GetWindowForXYExceptID(LONG x, LONG y, LONG id)
{
	PTRLISTPOS pos = m_pWindowList->GetHeadPosition();
	while (pos)
	{
		cDialog * win = (cDialog *)m_pWindowList->GetNext(pos);
		if (win->GetID() == id || win->IsActive() == FALSE)
			continue;
		if (win->PtInWindow(x, y))
			return win;
	}
	return NULL;
}
cIconDialog * cWindowManager::GetIconDialogForXY(LONG x, LONG y)
{
	PTRLISTPOS pos = m_pIconDlgListRef->GetHeadPosition();
	while (pos)
	{
		cDialog * win = (cDialog *)m_pIconDlgListRef->GetNext(pos);
		if (win->IsActive() == FALSE)
			continue;
		if (win->PtInWindow(x, y))
			return (cIconDialog *)win;
	}
	return NULL;
}
void cWindowManager::SetOptionAlpha(DWORD dwAlpha)
{
	PTRLISTPOS pos = m_pWindowList->GetHeadPosition();
	while (pos)
	{
		cWindow * win = (cWindow *)m_pWindowList->GetNext(pos);
		win->SetOptionAlpha(dwAlpha);
	}
}
cMsgBox* cWindowManager::MsgBox(LONG lMBId, int nMBType, char* pStrMsg, ...)
{
	if (!cMsgBox::IsInit()) return NULL;
	if (WINDOWMGR->GetWindowForID(lMBId) != NULL)
		return NULL;
	cMsgBox* pMsgBox = new cMsgBox;
	char msg[256] = { 0, };
	if (pStrMsg != NULL)
	{
		va_list argList;
		va_start(argList, pStrMsg);
		vsprintf(msg, pStrMsg, argList);
		va_end(argList);
	}
	pMsgBox->MsgBox(lMBId, nMBType, msg, MessageBox_Func);
	pMsgBox->SetAlwaysTop(TRUE);
	AddWindow(pMsgBox);
	LONG lX = (LONG)pMsgBox->GetAbsX();
	LONG lY = (LONG)pMsgBox->GetAbsY();
	PositioningDlg(lX, lY);
	pMsgBox->SetAbsXY(lX, lY);
	pMsgBox->SetActive(TRUE);
	return pMsgBox;
}
void cWindowManager::PositioningDlg(LONG& rX, LONG& rY)
{
	int nCount = 0;
	PTRLISTPOS pos = m_pWindowList->GetHeadPosition();
	while (pos)
	{
		cWindow* pWindow = (cWindow*)m_pWindowList->GetNext(pos);
		if ((LONG)pWindow->GetAbsX() == rX)
		if ((LONG)pWindow->GetAbsY() == rY)
		{
			rX += 20;
			rY += 20;
			if (++nCount >= 10)
			{
				rX -= 200;
				rY -= 200;
				nCount = 0;
			}
		}
	}
}
cDivideBox * cWindowManager::DivideBox(LONG lId, LONG x, LONG y, cbDivideFUNC cbFc1, cbDivideFUNC cbFc2, void * vData1, void* vData2, char * strTitle)
{
	if (WINDOWMGR->GetWindowForID(lId) != NULL)
		return NULL;
	cDivideBox * window = (cDivideBox *)GetDlgInfoFromFile("./image/InterfaceScript/DivideBox.bin", "rb");
	window->CreateDivideBox(x, y, lId, cbFc1, cbFc2, vData1, vData2, strTitle);
	window->SetActive(TRUE);
	window->SetAlwaysTop(TRUE);
	AddWindow(window);

	/*POINT point = { LONG(x)+50, LONG(y)+50 };
	ClientToScreen(
		_g_hWnd,
		&point);
	SetCursorPos(point.x, point.y);*/

	return window;
}
cDivideBoxEx * cWindowManager::DivideBoxEx(LONG lId, LONG x, LONG y, cbDivideExFUNC cbFc1, cbDivideExFUNC cbFc2, void * vData1, void* vData2, char * strTitle)
{
	if (WINDOWMGR->GetWindowForID(lId) != NULL)
		return NULL;
	cDivideBoxEx * window = (cDivideBoxEx *)GetDlgInfoFromFile("./image/InterfaceScript/DivideBoxEx.bin", "rb");
	window->CreateDivideBox(x, y, lId, cbFc1, cbFc2, vData1, vData2, strTitle);
	window->SetActive(TRUE);
	window->SetAlwaysTop(TRUE);
	AddWindow(window);
	return window;
}
void cWindowManager::SetWindowTop(cWindow* win)
{
	PTRLISTPOS pos = m_pWindowList->Find(win);
	if (pos == NULL) return;
	m_pWindowList->RemoveAt(pos);
	if (win->IsAlwaysTop())
		m_pWindowList->AddTail(win);
	else
	{
		pos = m_pWindowList->GetTailPosition();
		while (pos)
		{
			cWindow* pWindow = (cWindow*)m_pWindowList->GetAt(pos);
			if (pWindow)
			if (!pWindow->IsAlwaysTop())	break;
			m_pWindowList->GetPrev(pos);
		}
		if (pos)
			m_pWindowList->InsertAfter(pos, win);
		else
			m_pWindowList->AddHead(win);
	}
}
void cWindowManager::SetFocusedEdit(cWindow* pEdit, BOOL bFocus)
{
	if (bFocus)
	{
		if (m_pFocusedEdit == pEdit) return;
		m_pFocusedEdit = pEdit;
	}
	else
	{
		if (m_pFocusedEdit == pEdit)
		{
			m_pFocusedEdit = NULL;
		}
	}
}
BOOL cWindowManager::CloseAllAutoCloseWindows()
{
	PTRLISTPOS pos = m_pWindowList->GetHeadPosition();
	cWindow* pWin;
	BOOL bClose = FALSE;
	while (pos)
	{
		pWin = (cWindow*)m_pWindowList->GetNext(pos);
		if (pWin->IsAutoClose() && pWin->IsActive())
		{
			pWin->SetActive(FALSE);
			bClose = TRUE;
		}
	}
	return bClose;
}
cWindow* cWindowManager::GetDlgInfoFromFile(char * filePath, char* mode)
{
	return m_pScriptManager->GetDlgInfoFromFile(filePath, mode);
}
cMsgBox* cWindowManager::GetFirstMsgBox()
{
	PTRLISTPOS pos = m_pWindowList->GetTailPosition();
	cWindow * window;
	while (pos)
	{
		window = (cWindow *)m_pWindowList->GetPrev(pos);
		if (!window) continue;
		if (window->GetType() == WT_MSGBOX)
			return (cMsgBox*)window;
	}
	return NULL;
}
void cWindowManager::CloseAllMsgBox()
{
	PTRLISTSEARCHSTART((*m_pWindowList), cWindow*, pWindow)
	if (pWindow)
	if (pWindow->GetType() == WT_MSGBOX)
	{
		((cMsgBox*)pWindow)->ForceClose();
	}
	PTRLISTSEARCHEND
}
void cWindowManager::CloseAllMsgBoxNoFunc()
{
	PTRLISTSEARCHSTART((*m_pWindowList), cWindow*, pWindow)
	if (pWindow)
	if (pWindow->GetType() == WT_MSGBOX)
	{
		AddListDestroyWindow(pWindow);
	}
	PTRLISTSEARCHEND
}
BOOL cWindowManager::ReposAllWindow(BOOL bFlag)
{
	PTRLISTSEARCHSTART((*m_pWindowList), cWindow*, pWindow)
	if (pWindow->IsActive() == TRUE)
	{
		//pWindow->SetDisable(bFlag);
		if (pWindow->GetWindowBase()!=0)
		{

			DISPLAY_INFO	dispInfo;
			GAMERESRCMNGR->GetDispInfo(&dispInfo);

			LONG lXpos = 0;
			LONG lYpos = 0;
			LONG lWidth = 0;
			LONG lHeight = 0;

			LONG x = pWindow->GetAbsX();
			LONG y = pWindow->GetAbsY();
			WORD wid = pWindow->GetWidth();
			WORD hei = pWindow->GetHeight();

			VECTOR2 Pos1;

			switch (pWindow->GetWindowBase())
			{
			case 1:
				break;
			case 2:
				break;
			case 3:
			{
					  x = ((float)dispInfo.dwWidth - wid);
					  y = pWindow->GetAbsY();
			}
				break;

			case 4:
				break;
			case 5:
			{
					  x = pWindow->GetAbsX() + ((float)dispInfo.dwWidth - MIDRSLTN_W) / 2;
					  y = pWindow->GetAbsY() + ((float)dispInfo.dwHeight - MIDRSLTN_H) / 2;
			}
				break;
			case 6:
			{
					  x = ((float)dispInfo.dwWidth - wid);
					  y = ((float)dispInfo.dwHeight / 2 - hei / 2) + 85;
			}
				break;

			case 7:
			{
					  x = pWindow->GetAbsX();
					  y = ((float)dispInfo.dwHeight - hei);
			}
				break;
			case 8:
				break;
			case 9:
			{
					  x = (float)dispInfo.dwWidth - wid;
					  y = (float)dispInfo.dwHeight - hei;
			}
				break;
			}


			pWindow->SetAbsXY(x, y);
			pWindow->SetRelXY(x, y);
			pWindow->SetWH(wid, hei);
		}
		else
		{
			DISPLAY_INFO	dispInfo;
			GAMERESRCMNGR->GetDispInfo(&dispInfo);

			LONG lXpos = 0;
			LONG lYpos = 0;
			LONG lWidth = 0;
			LONG lHeight = 0;

			//x += (LONG)m_vDeltaPos[0].x;
			//y += (LONG)m_vDeltaPos[0].y;
			WORD wid = pWindow->GetWidth();
			WORD hei = pWindow->GetHeight();

			pWindow->SetAbsXY(pWindow->GetAbsX(), pWindow->GetAbsY());
			pWindow->SetRelXY(pWindow->GetAbsX(), pWindow->GetAbsY());
			pWindow->SetWH(wid, hei);
		}
	}
	PTRLISTSEARCHEND
		return bFlag;
}
BOOL cWindowManager::DisableAllWindow(BOOL bFlag)
{
	PTRLISTSEARCHSTART((*m_pWindowList), cWindow*, pWindow)
	if (pWindow->IsActive() == TRUE)
	{
		pWindow->SetDisable(bFlag);
	}
	PTRLISTSEARCHEND
		return bFlag;
}
BOOL cWindowManager::CloseAllWindow()
{
	GAMEIN->GetGuageDlg()->SetActive(FALSE);
	BOOL rt = FALSE;
	PTRLISTSEARCHSTART((*m_pWindowList), cWindow*, pWindow)
	if (pWindow->IsActive() == TRUE)
	{
		rt = TRUE;
		pWindow->SetActive(FALSE);
		m_bHideInterface = TRUE;
	}
	PTRLISTSEARCHEND
		return rt;
}
void cWindowManager::ShowBaseWindow()
{
	GAMEIN->GetChatDialog()->SetActive(TRUE);
	GAMEIN->GetGuageDlg()->SetActive(TRUE);
	GAMEIN->GetMainInterfaceDialog()->SetActive(TRUE);
	GAMEIN->GetQuickDialog()->SetActive(TRUE);
	GAMEIN->GetMiniMapDialog()->SetActive(true);
	m_bHideInterface = FALSE;
}
void cWindowManager::ToggleShowInterface()
{
	if (CloseAllWindow() == FALSE)
		ShowBaseWindow();
}
void cWindowManager::CreateGuildInvitationKindSelectionDlg()
{
	////cWindow* window = NULL;
	cWindow* window = GetDlgInfoFromFile("./image/InterfaceScript/JoinOption.bin", "rb");
	VECTOR2 Pos1;
	Pos1.x = ((float)GET_MAINWIN_W / 2 - window->GetWidth() / 2);
	Pos1.y = ((float)GET_MAINWIN_H / 2 - window->GetHeight() / 2);
	window->SetAbsXY(Pos1.x, Pos1.y);
	AddWindow(window);
	CGuildJoinDialog* pDlg = (CGuildJoinDialog*)window;
	pDlg->Linking();
	GAMEIN->SetGuildJoinDialog(pDlg);
}
void cWindowManager::CreateGuildMunhaDlg()
{
	////cWindow* window = NULL;
	cWindow* window = GetDlgInfoFromFile("./image/InterfaceScript/GuildTraineeInfo.bin", "rb");
	VECTOR2 Pos1;
	Pos1.x = ((float)GET_MAINWIN_W / 2 - window->GetWidth() / 2);
	Pos1.y = ((float)GET_MAINWIN_H / 2 - window->GetHeight() / 2);
	window->SetAbsXY(Pos1.x, Pos1.y);
	AddWindow(window);
	CGuildMunhaDialog* pDlg = (CGuildMunhaDialog*)window;
	pDlg->Linking();
	GAMEIN->SetGuildMunhaDialog(pDlg);
}
void cWindowManager::CreateGuildTraineeDlg()
{
	////cWindow* window = NULL;
	cWindow* window = GetDlgInfoFromFile("./image/InterfaceScript/GuildTrainee.bin", "rb");
	VECTOR2 Pos1;
	Pos1.x = ((float)GET_MAINWIN_W / 2 - window->GetWidth() / 2);
	Pos1.y = ((float)GET_MAINWIN_H / 2 - window->GetHeight() / 2);
	window->SetAbsXY(Pos1.x, Pos1.y);
	AddWindow(window);
	CGuildTraineeDialog* pDlg = (CGuildTraineeDialog*)window;
	pDlg->Linking();
	GAMEIN->SetGuildTraineeDialgo(pDlg);
}
void cWindowManager::CreateSurvivalCountDlg()
{
	//cWindow* window = NULL;
	cWindow* window = GetDlgInfoFromFile("./image/InterfaceScript/SurvivalDlg.bin", "rb");
	AddWindow(window);
	CSurvivalCountDialog* pDlg = (CSurvivalCountDialog*)window;
	GAMEIN->SetSurvivalCountDialog(pDlg);
	pDlg->Linking();
	GAMEIN->GetSurvivalCountDlg()->InitSurvivalCountDlg(MAP->GetMapNum());
}
void cWindowManager::CreateUniqueItemCurseCancellationDlg()
{
	//cWindow* window = NULL;
	cWindow* window = GetDlgInfoFromFile("./image/InterfaceScript/UniqueItemCurseCancellation.bin", "rb");
	AddWindow(window);
	GAMEIN->SetUniqueItemCurseCancellationDlg((CUniqueItemCurseCancellationDlg*)window);
	GAMEIN->GetUniqueItemCurseCancellationDlg()->Linking();
}
void cWindowManager::CreateUniqueItemMixDlg()
{
	//cWindow* window = NULL;
	cWindow* window = GetDlgInfoFromFile("./image/InterfaceScript/UniqueItemMixDlg.bin", "rb");
	AddWindow(window);
	GAMEIN->SetUniqueItemMixDlg((CUniqueItemMixDlg*)window);
	GAMEIN->GetUniqueItemMixDlg()->Linking();
}
void cWindowManager::CreateSOSDlg()
{
	//cWindow* window = NULL;
	cWindow* window = GetDlgInfoFromFile("./image/InterfaceScript/SOScall.bin", "rb");
	AddWindow(window);
	GAMEIN->SetSOSDlg((CSOSDlg*)window);
	GAMEIN->GetSOSDlg()->Linking();
	window = GetDlgInfoFromFile("./image/InterfaceScript/AutoNoteDlg.bin", "rb");
	AddWindow(window);
	GAMEIN->SetAutoNoteDlg((CAutoNoteDlg*)window);
	GAMEIN->GetAutoNoteDlg()->Linking();
	window = GetDlgInfoFromFile("./image/InterfaceScript/AutoAnswerDlg.bin", "rb");
	AddWindow(window);
	GAMEIN->SetAutoAnswerDlg((CAutoAnswerDlg*)window);
	GAMEIN->GetAutoAnswerDlg()->Linking();
}
void cWindowManager::CreateTopIcon()
{
	cWindow * window = GetDlgInfoFromFile("./image/InterfaceScript/TopMain.bin", "rb");
	ASSERT(window);
	VECTOR2 Pos1;
	Pos1.x = ((float)GET_MAINWIN_W - window->GetWidth()) - 171;
	Pos1.y = ((float)GET_MAINWIN_H / 2 - window->GetHeight() / 2);
	window->SetAbsXY(Pos1.x, Pos1.y);
	AddWindow(window);
	CTopIcon * pDlg = (CTopIcon*)window;
	GAMEIN->SetTopIcon(pDlg);
	pDlg->Linking();
}
void cWindowManager::CreateVideoCaptureDialog()
{
	cWindow * window = GetDlgInfoFromFile("./image/InterfaceScript/VideoCaptureDialog.bin", "rb");
	ASSERT(window);
	VECTOR2 Pos1;
	Pos1.x = ((float)GET_MAINWIN_W / 2 - window->GetWidth() / 2);
	Pos1.y = ((float)GET_MAINWIN_H / 2 - window->GetHeight() / 2);
	window->SetAbsXY(Pos1.x, Pos1.y);
	AddWindow(window);
	CVideoCaptureDlg * pDlg = (CVideoCaptureDlg*)window;
	GAMEIN->SetVideoCaptureDlg(pDlg);
	pDlg->Linking();
}
void cWindowManager::CreateVideoCaptureIcon()
{
	cWindow * window = GetDlgInfoFromFile("./image/InterfaceScript/VideoCaptureIcon.bin", "rb");
	ASSERT(window);
	VECTOR2 Pos1;
	Pos1.x = ((float)GAMERESRCMNGR->m_GameDesc.dispInfo.dwWidth - window->GetWidth());
	Pos1.y = window->GetAbsY();
	window->SetAbsXY(Pos1.x - GAMEIN->GetMiniMapDialog()->GetWidth(), Pos1.y);
	AddWindow(window);
	CVideoCaptureIcon * pDlg = (CVideoCaptureIcon*)window;
	GAMEIN->SetVideoCaptureIcon(pDlg);
	pDlg->Linking();
}
void cWindowManager::CreateGuildWarWarDlg()
{
	cWindow * window = GetDlgInfoFromFile("./image/InterfaceScript/GuildWarDlg.bin", "rb");
	AddWindow(window);
	VECTOR2 Pos1;
	Pos1.x = ((float)GAMERESRCMNGR->m_GameDesc.dispInfo.dwWidth - GAMEIN->GetMiniMapDialog()->GetWidth() - window->GetWidth());
	Pos1.y = window->GetAbsY();
	window->SetAbsXY(Pos1.x, Pos1.y);
	GAMEIN->SetGuildWarDialog((CGuildWarDialog *)window);
	GAMEIN->GetGuildWarDialog()->Linking();
}
void cWindowManager::CreateCodeDialog()
{
	cWindow * window = GetDlgInfoFromFile("./image/InterfaceScript/CodeDlg.bin", "rb");
	VECTOR2 Pos1;
	Pos1.x = ((float)GET_MAINWIN_W / 2 - window->GetWidth() / 2);
	Pos1.y = ((float)GET_MAINWIN_H / 2 - window->GetHeight() / 2);
	window->SetAbsXY(Pos1.x, Pos1.y);
	AddWindow(window);
	CCodeDialog* pDlg = (CCodeDialog*)window;
	GAMEIN->SetCodeDialog(pDlg);
	pDlg->Linking();
}
void cWindowManager::CreateRankButtonDialog()
{
	cWindow * window = GetDlgInfoFromFile("./image/InterfaceScript/RankButtonDialog.bin", "rb");
	VECTOR2 Pos1;
	Pos1.x = ((float)GET_MAINWIN_W - window->GetWidth()) - 145;
	Pos1.y = window->GetAbsY() + 214;
	window->SetAbsXY(Pos1.x, Pos1.y);
	AddWindow(window);
	CRankButtonDialog* pDlg = (CRankButtonDialog*)window;
	GAMEIN->SetRankButtonDialog(pDlg);
	pDlg->Linking();
}
void cWindowManager::CreateLoginEventDialog()
{
	cWindow * window = GetDlgInfoFromFile("./image/InterfaceScript/EventPlayTime.bin", "rb");
	VECTOR2 Pos1;
	Pos1.x = (float)GET_MAINWIN_W - window->GetWidth();
	Pos1.y = (float)GET_MAINWIN_H - window->GetHeight();
	window->SetAbsXY(Pos1.x - GAMEIN->GetQuickDialog()->GetWidth() - 10, Pos1.y - GAMEIN->GetMainInterfaceDialog()->GetHeight() - 10);
	AddWindow(window);
	CEventPlayTimeDlg* pDlg = (CEventPlayTimeDlg*)window;
	GAMEIN->SetEventPlayTimeDlg(pDlg);
	pDlg->Linking();
}
void cWindowManager::CreateSendEquipDialog()
{
	cWindow * window = GetDlgInfoFromFile("./image/InterfaceScript/SendEquipDlg.bin", "rb");
	ASSERT(window);
	VECTOR2 Pos1;
	Pos1.x = GetScreenX() / 2 - window->GetWidth() / 2;
	Pos1.y = GetScreenY() / 2 - window->GetHeight() / 2;
	window->SetAbsXY(Pos1.x, Pos1.y);
	AddWindow(window);
	CSendEquipDialog * pDlg = (CSendEquipDialog*)window;
	GAMEIN->SetSendEquipDialog(pDlg);
	pDlg->Linking();
}
void cWindowManager::CreateSendEquipSelectDialog()
{
	cWindow * window = GetDlgInfoFromFile("./image/InterfaceScript/ItemLinkTooltip.bin", "rb");
	ASSERT(window);
	//VECTOR2 Pos1;
	//Pos1.x = GetScreenX() / 2 - window->GetWidth() / 2;
	//Pos1.y = GetScreenY() / 2 - window->GetHeight() / 2;
	//window->SetAbsXY(Pos1.x, Pos1.y);
	AddWindow(window);
	CSendEquipSelectDialog * pDlg = (CSendEquipSelectDialog*)window;
	GAMEIN->SetSendEquipSelectDialog(pDlg);
	//pDlg->Linking();
}
void cWindowManager::CreateFBTimeDialog()
{
	cWindow * window = WINDOWMGR->GetDlgInfoFromFile("./image/InterfaceScript/InsFUBENInfoDlg.bin", "rb");
	VECTOR2 Pos1;
	Pos1.x = ((float)GET_MAINWIN_W - window->GetWidth());
	Pos1.y = window->GetAbsY();
	window->SetAbsXY(Pos1.x, Pos1.y);
	AddWindow(window);
	GAMEIN->SetFBTimeDialog((CFBTimeDialog *)window);
	GAMEIN->GetFBTimeDialog()->Linking();
}
void cWindowManager::CreateFameGoldDialog()
{
	cWindow * window = GetDlgInfoFromFile("./image/InterfaceScript/FameGoldDialog.bin", "rb");
	VECTOR2 Pos1;
	Pos1.x = GetScreenX() / 2 - window->GetWidth() / 2;
	Pos1.y = GetScreenY() / 2 - window->GetHeight() / 2;
	window->SetAbsXY(Pos1.x, Pos1.y);
	AddWindow(window);
	CFameGoldDialog * pDlg = (CFameGoldDialog*)window;
	GAMEIN->SetFameGoldDialog(pDlg);
	pDlg->Linking();
}
void cWindowManager::CreateSafeNumberIcon()
{
	cWindow * window = GetDlgInfoFromFile("./image/InterfaceScript/SafeInfoDlg.bin", "rb");
	ASSERT(window);
	VECTOR2 Pos1;
	Pos1.x = GetScreenX() - window->GetWidth() - 60;
	Pos1.y = GetScreenY() - window->GetHeight() - 50;
	window->SetAbsXY(Pos1.x, Pos1.y);
	AddWindow(window);
	CSafeNumberIcon * pDlg = (CSafeNumberIcon*)window;
	GAMEIN->SetSafeNumberIcon(pDlg);
	pDlg->Linking();
	if (m_SafeIconShow)
	{
		GAMEIN->GetSafeNumberIcon()->SetActive(TRUE);
		GAMEIN->GetSafeNumberIcon()->SetActiveStatic(TRUE);
	}
	else
	{
		GAMEIN->GetSafeNumberIcon()->SetActive(FALSE);
		GAMEIN->GetSafeNumberIcon()->SetActiveStatic(FALSE);
	}
}
void cWindowManager::CreateSafeNumberImg()
{
	cWindow * window = GetDlgInfoFromFile("./image/InterfaceScript/SafeFloatBtn.bin", "rb");
	ASSERT(window);
	VECTOR2 Pos1;
	Pos1.x = GetScreenX() - window->GetWidth() - 60;
	Pos1.y = GetScreenY() - window->GetHeight() - 50;
	window->SetAbsXY(Pos1.x, Pos1.y);
	AddWindow(window);
	CSafeNumberImg * pDlg = (CSafeNumberImg*)window;
	GAMEIN->SetSafeNumberImg(pDlg);
	pDlg->Linking();
}
void cWindowManager::CreateDungeonScriptDlg()
{
	cWindow * window = GetDlgInfoFromFile("./image/InterfaceScript/InsFUBENMissionInfoDlg.bin", "rb");
	VECTOR2 Pos1;
	Pos1.x = ((float)GAMERESRCMNGR->m_GameDesc.dispInfo.dwWidth - window->GetWidth());
	Pos1.y = window->GetAbsY();
	window->SetAbsXY(Pos1.x - GAMEIN->GetFBTimeDialog()->GetWidth(), Pos1.y);
	AddWindow(window);
	GAMEIN->SetInsDGMissionDialog((CInsFUBENMissionString *)window);
	GAMEIN->GetInsDGMissionDialog()->Linking();
}
void cWindowManager::CreateInsFUBENEntranceInfoDlg()
{
	cWindow * window = GetDlgInfoFromFile("./image/InterfaceScript/InsFUBENEntranceInfoDlg.bin", "rb");
	ASSERT(window);
	VECTOR2 Pos1;
	Pos1.x = ((float)GET_MAINWIN_W / 2 - window->GetWidth() / 2);
	Pos1.y = ((float)GET_MAINWIN_H / 2 - window->GetHeight() / 2);
	window->SetAbsXY(Pos1.x, Pos1.y);
	AddWindow(window);
	CInsDGEntranceInfoDlg* pDlg = (CInsDGEntranceInfoDlg*)window;
	GAMEIN->SetInsFUBENEntranceInfoDlg(pDlg);
	GAMEIN->GetInsFUBENEntranceInfoDlg()->Linking();
}
void cWindowManager::CreateJackFlashDlg()
{
	CJackFlashMsg* window = (CJackFlashMsg*)GetDlgInfoFromFile("./image/InterfaceScript/JackFlash.bin", "rb");
	if (!window)
	{
		return;
	}
	VECTOR2 Pos1;
	Pos1.x = ((float)GET_MAINWIN_W / 2 - window->GetWidth() / 2) - 115;
	Pos1.y = ((float)GET_MAINWIN_H / 2 - window->GetHeight() / 2) + 180;
	window->SetAbsXY(Pos1.x, Pos1.y);
	AddWindow(window);
	GAMEIN->SetJackFlashDlg(window);
	GAMEIN->GetJackFlashDlg()->Linking();
}
void cWindowManager::CreateHideBuffDlg()
{
	cWindow * window = GetDlgInfoFromFile("./image/InterfaceScript/HideBuff.bin", "rb");
	AddWindow(window);
	GAMEIN->SetHideBuffDlg((CHideBuff *)window);
	GAMEIN->GetHideBuffDlg()->Linking();
}
void cWindowManager::CreateAdPointDlg()
{
	cWindow * window = GetDlgInfoFromFile("./image/InterfaceScript/AdPointDlg.bin", "rb");
	VECTOR2 Pos1;
	Pos1.x = ((float)GET_MAINWIN_W / 2 - window->GetWidth() / 2);
	Pos1.y = ((float)GET_MAINWIN_H / 2 - window->GetHeight() / 2);
	window->SetAbsXY(Pos1.x, Pos1.y);
	AddWindow(window);
	AdPointDlg* pDlg = (AdPointDlg*)window;
	pDlg->Linking();
	GAMEIN->SetAdPointDlg(pDlg);
}
void cWindowManager::CreateCharacterRBTN()
{
	cWindow * window = GetDlgInfoFromFile("./image/InterfaceScript/RCMDlg.bin", "rb");
	AddWindow(window);
	GAMEIN->SetCharacterRBTN((CharacterRBTN *)window);
	GAMEIN->GetCharacterRBTNDlg()->Linking();
}
void cWindowManager::CreateLuckyDrawIcon()
{
	cWindow * window = GetDlgInfoFromFile("./image/InterfaceScript/LuckyDrawIcon.bin", "rb");
	ASSERT(window);
	VECTOR2 Pos1;
	Pos1.x = ((float)GET_MAINWIN_W - window->GetWidth()) - 190;
	Pos1.y = window->GetAbsY();
	window->SetAbsXY(Pos1.x, Pos1.y);
	AddWindow(window);
	CLuckyDrawIcon* pDlg = dynamic_cast< CLuckyDrawIcon* >(window);
	//CLuckyDrawIcon * pDlg = (CLuckyDrawIcon*)window;
	GAMEIN->SetLuckyDrawIcon(pDlg);
	pDlg->Linking();
}
void cWindowManager::CreateBloodStormIcon()
{
	cWindow * window = GetDlgInfoFromFile("./image/InterfaceScript/BloodStormIcon.bin", "rb");
	ASSERT(window);
	VECTOR2 Pos1;
	Pos1.x = ((float)GET_MAINWIN_W - window->GetAbsX());
	Pos1.y = ((float)window->GetAbsY());
	window->SetAbsXY(Pos1.x, Pos1.y);
	AddWindow(window);
	CBloodStormIcon * pDlg = (CBloodStormIcon*)window;
	GAMEIN->SetBloodStormIcon(pDlg);
	pDlg->Linking();
}
void cWindowManager::CreateChangeLookDlg()
{
}
void cWindowManager::CreateItemDropDlg()
{
	cWindow * window = GetDlgInfoFromFile("./image/InterfaceScript/ItemDropDlg.bin", "rb");
	VECTOR2 Pos1;
	Pos1.x = ((float)GET_MAINWIN_W / 2 - window->GetWidth() / 2);
	Pos1.y = ((float)GET_MAINWIN_H / 2 - window->GetHeight() / 2);
	window->SetAbsXY(Pos1.x, Pos1.y);
	AddWindow(window);
	CItemDropInfoDialog* pDlg = dynamic_cast< CItemDropInfoDialog* >(window);
	//CItemDropInfoDialog* pDlg = (CItemDropInfoDialog*)window;
	GAMEIN->SetItemDropInfoDialog(pDlg);
	pDlg->Linking();
}
void cWindowManager::CreateFPSDialog()
{
	cWindow * window = GetDlgInfoFromFile("./image/InterfaceScript/FPSDialog.bin", "rb");
	ASSERT(window);
	//VECTOR2 Pos1;
	//Pos1.x = ((float)GAMERESRCMNGR->m_GameDesc.dispInfo.dwWidth - window->GetWidth());
	//Pos1.y = window->GetAbsY();
	//window->SetAbsXY(Pos1.x - GAMEIN->GetMiniMapDialog()->GetWidth(), Pos1.y);
	AddWindow(window);
	CFPSDialog * pDlg = (CFPSDialog*)window;
	GAMEIN->SetFPSDialog(pDlg);
	pDlg->Linking();
}

void cWindowManager::CreateJackRebornInfo()
{
	cWindow * window = GetDlgInfoFromFile("./image/InterfaceScript/JackRebornInfo.bin", "rb");
	ASSERT(window);
	//VECTOR2 Pos1;
	//Pos1.x = ((float)GAMERESRCMNGR->m_GameDesc.dispInfo.dwWidth - window->GetWidth());
	//Pos1.y = window->GetAbsY();
	//window->SetAbsXY(Pos1.x - GAMEIN->GetMiniMapDialog()->GetWidth(), Pos1.y);
	AddWindow(window);
	CJackRebornInfo * pDlg = (CJackRebornInfo*)window;
	GAMEIN->SetJackRebornInfo(pDlg);
	pDlg->Linking();
}

void cWindowManager::CreateInsDGInfoDlg()
{
	cWindow* window = GetDlgInfoFromFile("./image/InterfaceScript/InsDGInfoDlg.bin", "rb");
	AddWindow(window);
	GAMEIN->SetInsDGInfoDlg((CInsDGInfoDialog*)window);
	GAMEIN->GetInsDGInfoDlg()->Linking();
}
void cWindowManager::CreateInsDGMissionInfoDlg()
{
	cWindow* window = GetDlgInfoFromFile("./image/InterfaceScript/InsDGMissionInfoDlg.bin", "rb");
	VECTOR2 Pos1;
	Pos1.x = ((float)GAMERESRCMNGR->m_GameDesc.dispInfo.dwWidth - window->GetWidth());
	Pos1.y = window->GetAbsY();
	window->SetAbsXY(Pos1.x - GAMEIN->GetInsDGInfoDlg()->GetWidth(), Pos1.y);
	AddWindow(window);
	GAMEIN->SetInsDGMissionInfoDlg((CInsDGMissionInfoDialog*)window);
	GAMEIN->GetInsDGMissionInfoDlg()->Linking();
}
void cWindowManager::CreateInsDGEntranceInfoDlg()
{
	cWindow* window = GetDlgInfoFromFile("./image/InterfaceScript/InsDGEntranceInfoDlg.bin", "rb");
	VECTOR2 Pos1;
	Pos1.x = ((float)GET_MAINWIN_W / 2 - window->GetWidth() / 2);
	Pos1.y = ((float)GET_MAINWIN_H / 2 - window->GetHeight() / 2);
	window->SetAbsXY(Pos1.x, Pos1.y);
	AddWindow(window);
	GAMEIN->SetInsDGEntranceInfoDlg((CInsDGEntranceInfoDialog*)window);
	GAMEIN->GetInsDGEntranceInfoDlg()->Linking();
}
void cWindowManager::CreateInsDGPartyMakeDlg()
{
	cWindow* window = GetDlgInfoFromFile("./image/InterfaceScript/InsDGPartyMakeDlg.bin", "rb");
	VECTOR2 Pos1;
	Pos1.x = ((float)GET_MAINWIN_W / 2 - window->GetWidth() / 2);
	Pos1.y = ((float)GET_MAINWIN_H / 2 - window->GetHeight() / 2);
	window->SetAbsXY(Pos1.x, Pos1.y);
	AddWindow(window);
	GAMEIN->SetInsDGPartyMakeDlg((CInsDGPartyMakeDialog*)window);
	GAMEIN->GetInsDGPartyMakeDlg()->Linking();
}
void cWindowManager::CreateInsDGRankDlg()
{
	cWindow* window = GetDlgInfoFromFile("./image/InterfaceScript/InsDGRankDlg.bin", "rb");
	VECTOR2 Pos1;
	Pos1.x = ((float)GET_MAINWIN_W / 2 - window->GetWidth() / 2);
	Pos1.y = ((float)GET_MAINWIN_H / 2 - window->GetHeight() / 2);
	window->SetAbsXY(Pos1.x, Pos1.y);
	AddWindow(window);
	GAMEIN->SetInsDGRankDialog((CInsDGRankDialog*)window);
	GAMEIN->GetInsDGRankDialog()->Linking();
}
void cWindowManager::CreateHousingWebDlg()
{
	cWindow* window = GetDlgInfoFromFile( "./image/InterfaceScript/HousingWebDlg.bin", "rb" );
	AddWindow(window);
	GAMEIN->SetHousingWebDlg( (cHousingWebDlg*)window );
	GAMEIN->GetHousingWebDlg()->Linking();
}

void cWindowManager::CreateFadeDlg()
{
	// 다이얼로그 정보를 받는다.
	CFadeDlg* pDlg = NULL;
	pDlg = (CFadeDlg*)GetDlgInfoFromFile("./image/InterfaceScript/FadeDlg.bin", "rb");

	if (!pDlg) return;

	// 윈도우 매니져에 다이얼로그를 등록한다.
	AddWindow(pDlg);

	// GAMEIN에 다이얼로그를 등록한다.
	GAMEIN->SetFadeDlg(pDlg);

	CFadeDlg* pFadeDlg = NULL;
	pFadeDlg = GAMEIN->GetFadeDlg();

	if (!pFadeDlg) return;

	pFadeDlg->Initialize();
}
void cWindowManager::CreateDungeonRankingDlg()
{
	cWindow * window = GetDlgInfoFromFile("./image/InterfaceScript/InsRankingDlg.bin", "rb");
	ASSERT(window);
	VECTOR2 Pos1;
	Pos1.x = ((float)GET_MAINWIN_W / 2 - window->GetWidth() / 2);
	Pos1.y = ((float)GET_MAINWIN_H / 2 - window->GetHeight() / 2);
	window->SetAbsXY(Pos1.x, Pos1.y);
	AddWindow(window);
	CTopDungeon * pDlg = (CTopDungeon*)window;
	GAMEIN->SetTopDungeon(pDlg);
	GAMEIN->GetTopDungeon()->Linking();
}
void cWindowManager::CreateNewUpGrareAlexXDlg()
{
	cWindow* window = NULL;
	window = GetDlgInfoFromFile("./image/InterfaceScript/NewUpGrareAlexXDlg.bin", "rb");
	ASSERT(window);

	VECTOR2 Pos1;
	Pos1.x = ((float)GET_MAINWIN_W / 2 - window->GetWidth() / 2);
	Pos1.y = ((float)GET_MAINWIN_H / 2 - window->GetHeight() / 2);
	window->SetAbsXY(Pos1.x, Pos1.y);
	AddWindow(window);
	CNewUpGrareAlexXDlg* pDlg = (CNewUpGrareAlexXDlg*)window;
	GAMEIN->SetNewUpGrareAlexXDlg(pDlg);
	GAMEIN->GetNewUpGrareAlexXDlg()->Linking();

}