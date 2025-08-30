#pragma once
#include "cWindowDef.h"
#include "IndexGenerator.h"
#include "PtrList.h"
class cWindow;
class cImage;
class cDialog;
class cIconDialog;
class cMultiLineText;
class CMousePointer;
class cScriptManager;
class cAnimationManager;
class cMsgBox;
class cDivideBox;
class cDivideBoxEx;  // 2014-10-30 cDivideBoxEx声明
class CKeyboard;
class CMouse;

#define WINDOWMGR	USINGTON(cWindowManager)
typedef void (*cbDROPPROCESSFUNC)(LONG curDragX, LONG curDragY, LONG id, LONG beforeDragX, LONG beforeDragY);

class cWindowManager  
{
public:
	cWindowManager();
	virtual ~cWindowManager();

	void Init();
	void AfterInit();
	void Preprocess();
	void DestroyWindowProcess();
	void AddListDestroyWindow(cWindow * window);
	void Process();
	void Render();
	cbDROPPROCESSFUNC cbDragnDropProcess;
	void SetcbDropProcess(cbDROPPROCESSFUNC func) { cbDragnDropProcess = func; }
	cDialog * GetWindowForID(LONG id);
	cWindow * GetWindowForIDEx(LONG id);
	cDialog * GetWindowForXY(LONG x, LONG y);
	cDialog * GetWindowForXYExceptIcon(LONG x, LONG y);
	cDialog * GetWindowForXYExceptID(LONG x, LONG y, LONG id);
	cIconDialog * GetIconDialogForXY(LONG x, LONG y);
	void CloseWindowForID(LONG id);
	void DestroyWindowAll();
	void DeleteWindowForID(LONG id);
	void DeleteWindow( cWindow* pWindow );
	void DeleteEditBoxListAll();
	void DeleteIconDlgListAll();
	void DeleteChatTooltipListAll();
	void DeleteSSTitletipListAll();
	void Release();

	void Init2();
	void Release2();
	
	cbWindowProcess cbProcess;

	cScriptManager * m_pScriptManager;
	// ResourceManager Related
	cWindow * GetDlgInfoFromFile(char * filePath, char* mode = "rt");

	cImage * GetImagePath(int idx, char * path, int size)
	{
		//return m_pScriptManager->GetImagePath(idx,path,size);
		return NULL;
	}
	/*
	cImageBase * GetImageOfFile(char * filePath, int layer)
	{
		return m_pScriptManager->GetImageOfFile(filePath, layer);
	}

	*/
	cImage * GetImageOfFile(char * filePath, int layer)
	{
		//return m_pScriptManager->GetImageOfFile(filePath, layer);
		return NULL;
	}
//	MAKESINGLETON(cWindowManager);

	///////////////////////////////////////////////////////////
	// it's called different function according to game state
	void CreateMainBarDlg();
	void CreateGameIn();
	void CreateCharDlg();
	void CreateMugongSuryunDlg();
	void CreateInventoryDlg();
	void CreateExchangeDlg();
	void CreateRecvExchangeDlg();
	void CreateMixDlg();
	void CreateTitanPartsMakeDlg();		// magi82 - Titan(070112)
	void CreateTitanMixDlg();			// magi82 - Titan(070119)	
	void CreateTitanRepairDlg();		// 2007. 9. 11. CBH - 鸥捞藕 荐府芒
	void CreateTitanRecallDlg();		// 2007. 9. 13. CBH - 鸥捞藕 家券霸捞瘤
	void CreateTitanUpgradeDlg();		// magi82 - Titan(070119)
	void CreateTitanBreakDlg();			// magi82 - Titan(070119)
	void CreateTitanChangePreViewDlg();	// magi82 - Titan(070212)
	void CreateTitanInventoryDlg();		// magi82 - Titan(070222)
	void CreateTitanGuageDlg();			// magi82 - Titan(070305)
	//void CreateTitanMugongMixDlg();		// magi82 - Titan(070611) 鸥捞藕 公傍函券 林籍贸府
	void CreateTitanRegisterDlg();		// magi82 - Titan(070320)

	void CreateTitanDissolution();		// magi82 - titan(070321)
	void CreateDealDlg();

	// 2014-05-01 Create MallShop Window 

	//void CreateMallShopDialog();

	// 2014-08-16 Create GoldShop Window

	void CreateGoldShopDialog();

	void CreateMarryMiniDialog();   // Create Marrymini window 2014-05-08

	void CreateMiniShiTuDialog();  // Create MiniShiTu window 2014-05-10

	void CreateControlDialog();   // Create Bar Control window 2014-05-23

	void CreateBillboardDialog(); // Create BillboardDialog window 2014-05-25

	void CreateBillboardMunpDialog(); //weiye 2018-01-12 创建帮会排行窗口

	void CreateLuckerDialog();    //  Create LuckerDialog   window 2014-06-02

	void CreateMoneyPacketDialog(); // Create MoneyPacketDialog window 2014-11-09

	void CreateOtherCharacterMoneyDlg(); // Create OtherCharacterMoneyDlg window 2014-11-10

	void CreateOtherCharacterAmuletDlg(); // Create OtherCharacterAmuletDlg window 2014-11-10

	void CreateItemStoneDkDialog();     // Create ItemStoneDkDlg 2014-11-14 !

	void CreateItemStoneXqDialog();     // Create ItemStoneXqDlg 2014-11-18 !

	void CreateItemStepReinforceDialog();


	void CreateOtherCharacterInfo();  // Create Other CharacterInfo 2014-08-11

	void CreateOtherCharacterEquip(); // Create Other CharacterEquip pag 2014-08-11

	void CreateItemLockDlg();         // Create item lock Dialog 2014-12-01

	void CreateItemUnLockDlg();       // Create item lock Dialog 2014-12-07

	void CreateItemGambleDlg();       // 赌博 2015-01-16


	void CreateChatDlg();
//SW050722 attach to minimap
//	//SW050715
//	void CreateJackpotDlg();
	//void CreateDebugdlg();
	void CreateHeroGuage();
	void CreateQuickDlg();
	void CreatePartyDlg();
	void CreatePartyCreateDlg();
	void CreatePartyInviteDlg();
	void CreateGuildCreateDlg();
	void CreateGuildDlg();
	void CreateGuildInviteDlg();
	void CreateGuildMarkDlg();
	void CreateGuildLevelupDlg();
	void CreateGuildNickNameDlg();
	void CreateGuildRankDlg();
	void CreateGuildWarehouseDlg();
	void CreatePyogukDlg();
	void CreateNoteDlg();
	void CreateFriendDlg();
	void CreateMiniFriendDlg();
	void CreateMiniMapDlg();
	void CreateMiniNoteDlg();
	void CreateReviveDlg();
	void CreateWantNpcDlg();
	void CreateWantRegistDlg();
	void CreateQuestTotalDlg();
	void CreateMPRegistDlg();
	void CreateMPMissionDlg();
	void CreateMPNoticeDlg();
	void CreateMPGuageDlg();

	//SW051129 Pet
	void CreatePetStateDlg();
	void CreatePetStateMiniDlg();
	void CreatePetInvenDlg();
	void CreatePetUpgradeDlg();
	void CreatePetRevivalDlg();

	void CreatePetMultiDlg();
	
	// LYJ 051017 备涝畴痢惑 眠啊
	void CreateStallKindSelectDlg();
	void CreateStreetBuyStallDlg();
	void CreateBuyRegDlg();

	// 畴痢 八祸 芒 眠啊 by Stiner(8)
	void CreateStallFindDlg();

// LBS 03.10.16//////////////////
	void CreateStreetStallDlg();
	void CreateNpcScriptDlg();
	void CreateHelpDlg();
/////////////////////////////////

//KES
	void CreateMacroDlg();
//	void CreateMacroModeDlg();
	void CreateChatOptionDlg();
	void CreateOptionDlg();
	void CreateExitDlg();
	void CreateCharStateDlg();
	void CreateMenuSlotDlg();

	void CreateUpgradeDlg();
	void CreateReinforceDlg();
	void CreateDissolveDlg();
	void CreateBailDlg();
	
	void CreateDissolutionDlg();
	void CreateEventNotifyDlg();
	void CreateVsyncNotifyDlg();

	void CreateItemShopDlg();
	void CreateMoveDlg();
	void CreatePointSaveDlg();
	
	// guildfieldwar
	void CreateGuildFieldWarDlg();

	void CreateSkPointDlg();
	void CreateSkPointNotifyDlg();
	
	// partywar
	void CreatePartyWarDlg();
	
	// shout
	void CreateShoutDlg();
	void CreateShoutchatDlg();

	//
	void CreateChaseDlg();
	void CreateChaseinputDlg();

	//
	void CreateNameChangeDlg();
	void CreateNameChangeNotifyDlg();

	void CreateGTRegistDlg();
	void CreateGTRegistcancelDlg();
	void CreateGTStandingDlg();
	void CreateGTBattleListDlg();
	void CreateGTScoreInfoDlg();


	void CreateSeigeWarDlg();

	// 
	void CreateCharChangeDlg();
	void CreateItemSealDlg();
	void CreateChangeJobDlg();
	void CreateReinforceResetDlg();
	void CreateRareCreateDlg();

	// 06. 02. 碍拳 犁丰 救郴 牢磐其捞胶 眠啊 - 捞康霖
	void CreateReinforceDataGuideDlg();
	// 06. 02. 郴沥焊芒 屏焊扁 眠啊 - 捞康霖
	void CreateTipBrowserDlgDlg();
	void CreateKeySettingTipDlg();

	void CreateGuildNoteDlg();
	void CreateUnionNoteDlg();
	// 06. 03. 巩颇傍瘤 - 捞康霖
	void CreateGuildNoticeDlg();

	///////////////////////////////////////////////////
	// 06. 06. 2瞒 傈流 - 捞康霖
	void CreateSkillOptionChangeDlg();
	void CreateSkillOptionClearDlg();
	///////////////////////////////////////////////////

	void CreateGuildInvitationKindSelectionDlg();
	void CreateGuildMunhaDlg();

	void CreateGuildTraineeDlg();

	//SW061019 辑官捞国葛靛
	void CreateSurvivalCountDlg();

	// magi82 - UniqueItem(070703)
	void CreateUniqueItemCurseCancellationDlg();

	// magi82 - UniqueItem(070709)
	void CreateUniqueItemMixDlg();

	// magi82 - SOS(070724)
	void CreateSOSDlg();	

	// 2007. 10. 22. CBH - 蜡聪农 酒捞牌 炼钦 贸府 官
	void CreateUniqueItemMixProgressBarDlg();
	// 2007. 10. 22. CBH - 鸥捞藕 炼钦 霸捞瘤 促捞倔肺弊
	void CreateTitanMixProgressBarDlg();
	// 2007. 10. 22. CBH - 鸥捞藕 颇明 炼钦 霸捞瘤 促捞倔肺弊
	void CreateTitanPartsProgressBarDlg();

	// 2007. 12. 4. CBH - 胶挪 急琶 促捞倔肺弊
	void CreateSkinSelectDlg();
	// 2008. 1. 16. CBH - 内胶片 胶挪 急琶 促捞倔肺弊
	void CreateCostumeSkinSelectDlg();

	// magi82(42) - 讥酒捞袍 眠啊(荐访摹 檬扁拳)
	void CreateSkillPointResetDlg();

	// 2008. 3. 17. CBH - 焊救锅龋 促捞倔肺弊
	void CreateNumberPadDlg();

	// magi82(47) - 畴痢惑 酒捞袍 八祸芒
	void CreateStreetStallItemViewDlg();

	// 2008. 5. 23. CBH - 规颇概莫 促捞倔肺弊
	void CreatePartyMatchingDlg();

	
	//screenshot
	void cWindowManager::ScreenShotDlg();

	cMsgBox* MsgBox( LONG lMBId, int nMBType, char* pStrMsg, ... );	
	cDivideBox * DivideBox( LONG lId, LONG x, LONG y, cbDivideFUNC cbFc1, cbDivideFUNC cbFc2, void * vData1, void * vData2, char* strTitle );

	cDivideBoxEx * DivideBoxEx( LONG lId, LONG x, LONG y, cbDivideExFUNC cbFc1, cbDivideExFUNC cbFc2, void * vData1, void * vData2, char* strTitle );//  2014-10-30  cDivideBoxEx声明
//


//pjs

	void CreatMousePoint();

	//void CreateSafeNumberDialog();       //weiye 2017-12-04 创建安全码窗口

	void CreateSafeNumberIcon();          //weiye 2017-12-04 创建安全码图标窗口

	void CreateSafeNumberImg();          //weiye 2017-12-04 创建安全码静态图标窗口

	//void CreateSafeNumberChangeDialog();

	//void CreateSafeNumberLockDialog();

	void CreateTopIcon();                //weiye 2017-10-22 创建小地图下方图标窗口

	void CreateFBTimeDialog();           //weiye 2018-02-06 副本记时窗口定义

	void CreateFameGoldDialog();         //weiye 元宝洗E窗口定义   2018-02-10
	void CreateGoldShopBuyDialog();

	//void CreateCharacterControlDialog(); //weiye 创建角色功能窗口  2018-02-12

	void CreateSendEquipDialog();             //weiye 创建发送装备窗口 2018-03-14

	void CreateSendEquipSelectDialog();    //weiye 创建发送装备显示窗口 2018-03-14

	void CreateVideoCaptureDialog();    //weiye  创建录像窗口 2018-05-12

	void CreateVideoCaptureIcon();    //weiye  创建录像窗口 2018-05-14

	void CreateGuildWarWarDlg();

	void CreateCodeDialog();

	

	void CreateRankButtonDialog();

	void CreateLoginEventDialog();

	void CreateDungeonScriptDlg();
	void CreateInsFUBENEntranceInfoDlg();

	void CreateJackFlashDlg();
	void CreateHideBuffDlg();

	void CreateAdPointDlg();

	void CreateCharacterRBTN();

	void CreateLuckyDrawIcon();

	void CreateBloodStormIcon();

	void CreateChangeLookDlg();

	void CreateItemDropDlg();

	void CreateFPSDialog();

	void CreateJackRebornInfo();






	void CreateInsDGInfoDlg();
	void CreateInsDGMissionInfoDlg();
	void CreateInsDGEntranceInfoDlg();
	void CreateInsDGPartyMakeDlg();
	void CreateInsDGRankDlg();
	///////////////////////////////////////////////////////////
	void CreateHousingWebDlg();

	void CreateFadeDlg();

	void CreateDungeonRankingDlg();
	void CreateCharPvPDlg();

	void CreateOfficialUpGradeDlg();
	void CreateItemQualityDlg();           //装备觉醒窗口
	void CreateItemQualityChangeDlg();      //装备品质转换窗口
	void CreateGradeChangeDlg();			//武器升阶值转移卷

	void CreateVipDialog();                //VIP系统
	void CreatCustomizingDlg();           //自定义称号
#ifdef _MUTIPET_
	void CreatePetMixDlg();//刀哥 3pet
#endif // _MUTIPET_
	//++++ Common dialog +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// dynamic create and auto release
//	cWindow * CreateMoneyDlg();
//	cWindow * CreateAlertDlg(char * text, WORD wType, void (*cbFunc)(LONG lId, void * p, DWORD we), void * object);
	cPtrList * m_pDestroyWindowRef;					// reference
//	CIndexGenerator m_CommonDlgIDIndex;
	//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	LONG ID_SEED;
	inline void AddWindow(cWindow * window)
	{ 
#ifdef _DEBUG
		if(!window)
		{
			char buff[256];
			sprintf(buff, "window有NULL。脚本的脚本错误或PATH确认。");//pjslocal
			LOGEX(buff, PT_MESSAGEBOX);
			return;
		}
		PTRLISTPOS pos = m_pWindowList->GetHeadPosition();
		while(pos)
		{
			
			cWindow * win = (cWindow *)m_pWindowList->GetAt(pos);
			if(win == window)
			{

				char buff[256];
				sprintf(buff, "[AddWindow()] : 已存在的对白。");
				LOGEX(buff, PT_MESSAGEBOX);
			}
			m_pWindowList->GetNext(pos);
		}
#endif 
		m_pWindowList->AddTail((void *)window); 
	}


	void AddChatTooltip(cWindow * window)
	{
		m_pPlayerChatTooltip->AddTail(window);
	}
	void RemoveChatTooltipWindow(LONG playerID);	//鞘夸绝绢瘤搁 瘤款促. confirm
	void RemoveChatTooltipWindow( cWindow* pWindow );


	cPtrList * m_pWindowList;					
	cPtrList * m_pIconDlgListRef;					// reference
	cPtrList * m_pEditBoxListRef;					// for tab and shift+tab key	// reference
	cPtrList * m_pScreenTextList;					// 概 橇饭烙付促 眠啊登绊 昏力凳
	cPtrList * m_pPlayerChatTooltip;

	void SetDragStart(LONG sX, LONG sY, LONG x, LONG y, LONG id)
	{
		m_id = id;
		m_pDragDialog = GetWindowForID( id );
		m_OldX = x; m_OldY = y;
		m_posBeforeDrag.x=(float)sX;
		m_posBeforeDrag.y=(float)sY;
		m_OldDragFlag = TRUE;
	}
	void SetDragEnd()
	{
		//m_id=-1;
		//m_pDragDialog = NULL;
		m_OldX = -1; m_OldY = -1;
		m_OldDragFlag = FALSE;
	}

	void BackDragWindow();
	void DragWindowNull()
	{
		m_pDragDialog = NULL;
	}

	BOOL IsDragWindow() { return m_OldDragFlag;	}
	
	BOOL ReposAllWindow(BOOL bFlag = true);
	BOOL DisableAllWindow(BOOL bFlag=true);
	BOOL CloseAllWindow();
	void ShowBaseWindow();


public:
	void ToggleShowInterface();
	
	BOOL m_OldDragFlag;
	void SetComposition(BOOL val){ m_fComposition = val; }
	BOOL IsComposition(){ return m_fComposition; }
	CMousePointer * GetMouseWindow() { return m_pMousePointWindow;	}

//KES
	cWindow* GetFocusedEdit() { return m_pFocusedEdit; }
	void SetFocusedEdit( cWindow* pEdit, BOOL bFocus );
	void SetNextEditFocus();
	void SetOptionAlpha( DWORD dwAlpha );
	void SetWindowTop( cWindow* win );
	void SetToolTipWindow( cMultiLineText* toolTip ) { m_pToolTipWindow = toolTip; }

	void SetRelationTipWindow	( cMultiLineText* toolTip ){m_pRelationTip  = toolTip;}
	void SetRelationTipWindow1	( cMultiLineText* toolTip ){m_pRelationTip1 = toolTip;}
	void SetSendItemInfoTip		( cMultiLineText* toolTip ){m_pSendItemInfo = toolTip;}

	void SetStaticToolTip(cMultiLineText* toolTip){ m_pStaticToolTip = toolTip; }

//KES INTERFACE 031008
	BOOL IsMouseInputProcessed()	{ return m_MouseInputProcessed; }

	BOOL IsMouseOverUsed()			{ return m_bMouseOverUsed; }
	BOOL IsMouseDownUsed()			{ return m_bMouseDownUsed; }
	BOOL IsMouseUpUsed()			{ return m_bMouseUpUsed; }
//	BOOL IsKeyInputUsed()			{ return m_bKeyInputUsed; }
	
	void SetMouseInputProcessed()	{ m_MouseInputProcessed = TRUE; }
	void SetMouseOverUsed()			{ m_bMouseOverUsed = TRUE; }
	void SetMouseDownUsed()			{ m_bMouseDownUsed = TRUE; }
	void SetMouseUpUsed()			{ m_bMouseUpUsed = TRUE; }

	void KeyboardInput( CKeyboard* keyInfo );
	void MouseInput( CMouse* pMouse );
	
	cMsgBox* GetFirstMsgBox();
	void CloseAllMsgBox();
	void CloseAllMsgBoxNoFunc();
	void PositioningDlg( LONG& rX, LONG& rY );
///////////

	BOOL CloseAllAutoCloseWindows();
/////
//	cDialog* GetDragDlg() { return GetWindowForID( m_id ); }
	cDialog* GetDragDlg() { return m_pDragDialog; }

	//weiye 屏幕分辨率相关函数定义 2017-12-23

	BOOL CreateFontObjectToFile();

	DWORD GetScreenX();

	DWORD GetScreenY();

	

	// 扩档快甸狼 捞亥飘甫 罐绰 何盒
	static void OnWindowEvent(LONG lId, void * p, DWORD we);

protected:

	BOOL			m_MouseInputProcessed;
	BOOL			m_fComposition;

	cDialog*		m_pGuageWindow;
	cWindow*		m_pMonsterGuageWindow;
	CMousePointer*	m_pMousePointWindow;

//KES 030819
	cWindow*		m_pFocusedEdit;
	cMultiLineText* m_pToolTipWindow;
	cMultiLineText* m_pRelationTip;
	cMultiLineText* m_pRelationTip1;
	cMultiLineText* m_pSendItemInfo;

	cMultiLineText*	m_pStaticToolTip;
	BOOL			m_bMouseOverUsed;
	BOOL			m_bMouseDownUsed;
	BOOL			m_bMouseUpUsed;
	
//	BOOL			m_bKeyInputUsed;
//

	//for drag
	LONG		m_id;
	cDialog*	m_pDragDialog;	//KES眠啊

	LONG		m_OldX;
	LONG		m_OldY;
	VECTOR2		m_posBeforeDrag;

//==== for rec.
	BOOL		m_bHideInterface;

public:
	BOOL		IsInterfaceHided() { return m_bHideInterface; }
	
protected:

	BOOL	m_bOpenStatus;

public:
	void SetOpenStatus( BOOL bOpen ) { m_bOpenStatus = bOpen; }

	BOOL		m_bMouseInWindow ;
	BOOL		m_IsHideMainInterface;
	BOOL		m_IsOpenedAllWindows ;
public :
	inline	void SetMouseInWindow(BOOL val)			{ m_bMouseInWindow = val;			}
	inline	BOOL IsMouseInWindow()					{ return m_bMouseInWindow;			}
	inline	void SetOpenedAllWindow( BOOL IsOpen )	{ m_IsOpenedAllWindows = IsOpen;	}
	inline	BOOL IsOpendAllWindows()				{ return m_IsOpenedAllWindows;		}
	inline	void SetHideInterface( BOOL val )		{ m_IsHideMainInterface = val;		}
	inline	BOOL IsHideInterface()					{ return m_IsHideMainInterface;		}

//	BOOL CloseAllAutoCloseWindows();
//	BOOL CloseAllWindow();	
	void ShowMainBaseWindow();
	void HideMainBaseWindow();
	void SaveAllActivedWindow( cWindow* win );
	void ShowAllActivedWindow();
//	void ToggleShowInterface();
};
EXTERNGLOBALTON(cWindowManager);