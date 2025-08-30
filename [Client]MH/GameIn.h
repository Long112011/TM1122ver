



// GameIn.h: interface for the CGameIn class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GAMEIN_H__E9C036AE_2344_473B_BEFE_21C4555A0685__INCLUDED_)
#define AFX_GAMEIN_H__E9C036AE_2344_473B_BEFE_21C4555A0685__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "GameState.h"
#include "PartyDialog.h"
#include "GuildWarDialog.h"
class CMugongSuryunDialog;
class CMugongDialog;
class CSuryunDialog;
class CInventoryExDialog;
class CExchangeDialog;
class CMixDialog;
class CTitanPartsMakeDlg;		// magi82 - Titan(070112)
class CTitanMixDlg;				// magi82 - Titan(070119)
class CTitanRepairDlg;			// 2007. 9. 13. CBH - Å¸ÀÌÅº ¼ö¸®Ã¢
class CTitanRecallDlg;			// 2007. 9. 11. CBH - Å¸ÀÌÅº ¼ÒÈ¯ °ÔÀÌÁö
class CTitanUpgradeDlg;			// magi82 - Titan(070119)
class CTitanBreakDlg;			// magi82 - Titan(070119)
class CTitanChangePreViewDlg;	// magi82 - Titan(070212)
class CTitanInventoryDlg;		// magi82 - Titan(070222)
class CTitanGuageDlg;			// magi82 - Titan(070305)
//class CTitanMugongMixDlg;		// magi82 - Titan(070611) Å¸ÀÌÅº ¹«°øº¯È¯ ÁÖ¼®Ã³¸®
class CTitanRegisterDlg;		// magi82 - Titan(070320)
class CTitanDissolutionDlg;		// magi82 - Titan(070321)
class CDealDialog;
class CCharacterDialog;
class CChatDialog;

class cJackpotDialog;

class CMainBarDialog;
class CDebugDlg;

// LYJ 051017 ±¸ÀÔ³ëÁ¡»ó Ãß°¡
class CStallKindSelectDlg;
class CStreetBuyStall;
class CBuyRegDialog;

// ³ëÁ¡»ó °Ë»ö ´ÙÀÌ¾ó·Î±× Ãß°¡ by Stiner(8)
class CStallFindDlg;

class CStreetStall;
class cNpcScriptDialog;
class cHelpDialog;

class CMacroDialog;
class CChatOptionDialog;
class COptionDialog;
class cDialog;
class CCharStateDialog;
class CMenuSlotDialog;

class cAuctionBoard;
class CMoneyDlg;
class CQuickDialog;
class CExitDialog;

class CGuildCreateDialog;
class CGuildDialog;
class CGuildInviteDialog;
class CGuildMarkDialog;
class CGuildLevelUpDialog;
class CGuildNickNameDialog;

class CGuildRankDialog;
class CGuildWarehouseDialog;
class CGuildUnionCreateDialog;
class CMiniMapDlg;
class CPyogukDialog;
class CNoteDialog;
class CFriendDialog;
class CMiniNoteDialog;
class CMiniFriendDialog;
class CReviveDialog;
class CMonsterGuageDlg;
class CUpgradeDlg;
class CReinforceDlg;
class CDissloveDlg;
class CWantNpcDialog;
class CWantRegistDialog;
class CQuestTotalDialog;
class CQuestDialog;
class CWantedDialog;
class CJournalDialog;
class CPartyCreateDlg;
class CPartyInviteDlg;
class CBailDialog;
class CDissolutionDialog;
class CEventNotifyDialog;
class cVSyncDialog;
class CItemShopDialog;
class CMoveDialog;
class CPointSaveDialog;

class CMPRegistDialog;
class CMPMissionDialog;
class CMPNoticeDialog;
class CMPGuageDialog;
class CMHMap;
class CMHCamera;

class CGFWarDeclareDlg;
class CGFWarResultDlg;
class CGFWarInfoDlg;
class CGuildWarInfoDlg;
class CGuildPlusTimeDlg;

class CSkillPointRedist;

class CSkillPointNotify;

class CMallShopDialog;  // 2014-05-01 Create MallItem Window

class CGoldShopDialog;  // 2014-08-16 Create ShopItem Window
class CGoldShopBuyDialog;

class CMiniMaryDialog;  // 2014-05-08 Create minimarry window

class CBaiShiDialog;   // 2014-05-10 Create miniBaishi window

class CControlDialog;  // 2014-05-23 Create control window

class CRankButtonDialog;

class CTopDialog;     // 2014-05-24 Create billboard window 
class CTopDungeon;
class CTopMunpDialog; //weiye °ï»áÅÅÐÐÍ·ÎÄ¼þ 2018-01-12

class CLuckerDialog;    // 2014-06-02 Create LuckerDialog window 

class CItemDropInfoDialog;

class CMoneyPacketDialog; // 2014-11-09 Create MoneyPacketDialog window

class COtherCharacerMoneyDlg; // 2014-11-10 Create OtherCharacterMoneyDlg window

class COtherCharacterAmuletDlg; // 2014-11-10 Create OtherCharacterAmuletDlg window

class CItemStoneDkDialog;     // 2014-11-14 

class CItemStoneXqDialog;     // 2014-11-18 

class CItemStepReinforceDialog;

class CCharactorInfoDlg; // 2014-08-11 Create CharacterInfoDlg window 

class CQuipInfoDlg;      // 2014-08-11 Create CharacterEquip Window

class CItemLockDialog;   // 2014-12-01 Create ItemLock window !

class CItemUnLockDialog; // 2014-12-07 Create UnItemlock window!

class CItemGambleDialog; // 2015-01-16 ¶Ä²©ÏµÍ³

//class CSafeNumberDialog;   //weiye °²È«ËøÏµÍ³  2017-12-07

//class CSafeNumberDialog;   //weiye °²È«ËøÏµÍ³  2017-12-07

class CSafeNumberIcon;      //weiye  °²È«ËøÏµÍ³  2017-12-06

class CSafeNumberImg;       //weiye  °²È«ËøÍ¼±ê  2018-02-01

//class CSafeNumberChangeDialog;  //weiye ÐÞ¸Ä°²È«Âë´°¿Ú 2018-04-08

//class CSafeNumberLockDialog;     //weiye ´´½¨Ëø¶¨°²È«Âë´°¿Ú 2018-04-08

class CTopIcon;             //weiye Ð¡µØÍ¼ÏÂ·½Í¼±ê¶¨Òå 2018-01-16

class CFBTimeDialog;        //weiye ¸±±¾ÀàÒýÓÃ 2018-02-06

class CFameGoldDialog;      //weiye Ôª±¦Ï´E´°¿ÚÀà 2018-02-10

//class CharacterControlDialog;  //weiye   ½ÇÉ«¹¦ÄÜ´°¿ÚÒýÓÃ  2018-02-13

class CSendEquipDialog;          //weiye  ·¢ËÍ×°±¸´°¿Ú 2018-03-14

class CSendEquipSelectDialog; //weiye  ·¢ËÍ×°±¸ÏÔÊ¾´°¿Ú 2018-03-14

class CVideoCaptureDlg;          //weiye  ÆÁÄ»Â¼Ïñ´°¿ÚÉèÖÃ  2018-05-02

class CVideoCaptureIcon;        //weiye  ÆÁÄ»Â¼Ïñ´°¿ÚÍ¼±ê 2018-05-14

class CPartyWarDialog;

class CShoutDialog;
class CShoutchatDialog;
class CChaseDialog;
class CChaseinputDialog;
class CNameChangeDialog;
class CNameChangeNotifyDlg;

class CBigMapDlg;

class CGTRegistDialog;
class CGTRegistcancelDialog;
class CGTStandingDialog;
class CGTBattleListDialog;
class CGTScoreInfoDialog;

class CSWTimeRegDlg;
class CSWProtectRegDlg;
class CSWInfoDlg;

class CCharChangeDlg;
class CSealDialog;
class CSWProfitDlg;
class CSWEngraveDialog;
class CSWTimeDialog;

class CChangeJobDialog;

class CSWStartTimeDialog;
class CGuageDialog;
class CReinforceResetDlg;
class CRareCreateDialog;

class CPetStateDlg;
class CPetStateMiniDlg;
class CPetInventoryDlg;
class CPetUpgradeDialog;
class CPetRevivalDialog;
class CPetMultiDlg;

// 06. 02. °­È­ Àç·á ¾È³» ÀÎÅÍÆäÀÌ½º Ãß°¡ - ÀÌ¿µÁØ
class CReinforceDataGuideDlg;
// 06. 02. ³»Á¤º¸Ã¢ ÆÁº¸±â Ãß°¡ - ÀÌ¿µÁØ
class CTipBrowserDlg;
class CKeySettingTipDlg;

class CGuildNoteDlg;
class CUnionNoteDlg;

class CGuildNoticeDlg;
class CSkillOptionChangeDlg;
class CSkillOptionClearDlg;

class CGuildJoinDialog;

class CScreenShotDlg;
class CGuildMunhaDialog;
class CGuildTraineeDialog;
class CSurvivalCountDialog;

class CUniqueItemCurseCancellationDlg;	// magi82 - UniqueItem(070703)
class CUniqueItemMixDlg;				// magi82 - UniqueItem(070709)

class CSOSDlg;	// magi82 - SOS(070724)

class CUniqueItemMixProgressBarDlg;			// 2007. 10. 19. CBH - À¯´ÏÅ© ¾ÆÀÌÅÆ Ã³¸® Ã³¸® ¹Ù °ÔÀÌÁö
class CTitanMixProgressBarDlg;			// 2007. 10. 22. CBH - Å¸ÀÌÅº Á¶ÇÕ °ÔÀÌÁö
class CTitanPartsProgressBarDlg;		// 2007. 10. 23. CBH - Å¸ÀÌÅº ÆÄÃ÷ Á¶ÇÕ °ÔÀÌÁö

class CSkinSelectDialog;		//2007. 12. 4. CBH - ½ºÅ² ¼±ÅÃ ´ÙÀÌ¾ó·Î±×
class CCostumeSkinSelectDialog;		//2008. 1. 16. CBH - ÄÚ½ºÆ¬ ½ºÅ² ¼±ÅÃ ´ÙÀÌ¾ó·Î±×

class CSkillPointResetDlg;	// magi82(42) - ¼¥¾ÆÀÌÅÛ Ãß°¡(¼ö·ÃÄ¡ ÃÊ±âÈ­)

class CNumberPadDialog;		//2008. 3. 17. CBH - º¸¾È¹øÈ£ ´ÙÀÌ¾ó·Î±×

// magi82(47)
class CStreetStallItemViewDlg;

class CAutoNoteDlg;
class CAutoAnswerDlg;

class CPartyMatchingDlg;		//2008. 5. 23. CBH - ¹æÆÄ ¸ÅÄª ´ÙÀÌ¾ó·Î±×
// ¿ä»õÀü
class CFWEngraveDialog;
class CFWTimeDialog;
class CFWWareHouseDialog;



class CCodeDialog;

class CEventPlayTimeDlg;

class CInsFUBENMissionString;
class CInsDGEntranceInfoDlg;

class CJackFlashMsg;
class CHideBuff;

class AdPointDlg;

class CReconnectDialog;

class CharacterRBTN;
//class CGuildWarDialog;


class RegID;
class RestPwd;

class CLuckyDrawIcon;

class CBloodStormIcon;

class cChangeLookDialog;

class CFPSDialog;

class CJackRebornInfo;


class CInsDGInfoDialog;
class CInsDGMissionInfoDialog;
class CInsDGEntranceInfoDialog;
class CInsDGPartyMakeDialog;
class CInsDGRankDialog;
class CInsDGRankInfoDialog;

class cHousingWebDlg;

class CFadeDlg;

class CCharacterPvpDialog;
class COfficialUpGradeDlg;
class CItemQualityDlg;          //¾õÐÑ×°±¸
class CItemQualityChangeDlg;     //×°±¸Æ·ÖÊ×ª»»
class CGradeChangeDlg;		//ÎäÆ÷Éý½×Öµ×ªÒÆ¾í
class VipDialog;               //VIPÏµÍ³ÀàÒýÓÃ
class CCustomizingNameDialog;   //×Ô¶¨Òå³ÆºÅ
#ifdef _MUTIPET_
class PetMixDlg;
#endif
#define GAMEIN USINGTON(CGameIn)

class CGameIn : public CGameState  
{
	///////////////////// Dialog Pointer //////////////////////////////////
	CMugongSuryunDialog	* m_pMugongSuryunDlg;
public :
	CMugongDialog		* m_pMugongDlg;
private:
	CSuryunDialog		* m_pSuryunDlg;

	CInventoryExDialog	* m_pInventoryDlg;
	CExchangeDialog		* m_pExchangeDlg;
//	CRecvExchangeDialog	* m_pRecvExchangeDlg;
	CMixDialog			* m_pMixDlg;
	CTitanPartsMakeDlg	* m_pTitanPartsMakeDlg;			// magi82 - Titan(070112)
	CTitanMixDlg		* m_pTitanMixDlg;				// magi82 - Titan(070119)	
	CTitanRepairDlg		*m_pTitanRepairDlg;				//2007. 9. 11. CBH - Å¸ÀÌÅº ¼ö¸®Ã¢
	CTitanRecallDlg		*m_pTitanRecallDlg;				//2007. 9. 13. CBH - Å¸ÀÌÅº ¼ÒÈ¯ °ÔÀÌÁö
	CTitanUpgradeDlg	* m_pTitanUpgradeDlg;			// magi82 - Titan(070119)
	CTitanBreakDlg		* m_pTitanBreakDlg;				// magi82 - Titan(070119)
	CTitanChangePreViewDlg	* m_pTitanChangePreViewDlg;	// magi82 - Titan(070212)
	CTitanInventoryDlg	* m_pTitanInventoryDlg;			// magi82 - Titan(070222)
	CTitanGuageDlg		* m_pTitanGuageDlg;				// magi82 - Titan(070305)
	//CTitanMugongMixDlg	* m_pTitanMugongMixDlg;			// magi82 - Titan(070611) Å¸ÀÌÅº ¹«°øº¯È¯ ÁÖ¼®Ã³¸®
	CTitanRegisterDlg	* m_pTitanRegisterDlg;			// magi82 - Titan(070320)
	CTitanDissolutionDlg* m_pTitanDissolutionDlg;		// magi82 - Titan(070321)

	CDealDialog			* m_pDealDlg;

	CCharacterDialog	* m_pCharDlg;
	CCharacterPvpDialog* m_pCharPvPDlg;	
	CChatDialog			* m_pChatDlg;

	// SW050715
//	cJackpotDialog		* m_pJackpotDlg;
	
	CMainBarDialog			* m_pMainDlg;
	CDebugDlg			* m_pDebugDlg;

// LYJ 051017 ±¸ÀÔ³ëÁ¡»ó Ãß°¡
	CStallKindSelectDlg * m_pStallKindSelectDlg;
	CStreetBuyStall		* m_pStreetBuyStallDlg;
	CBuyRegDialog		* m_pBuyRegDlg;


// ³ëÁ¡»ó °Ë»ö ´ÙÀÌ¾ó·Î±× Ãß°¡ by Stiner(8)
	CStallFindDlg		* m_pStallFindDlg;

// LBS 03.10.16
	CStreetStall		* m_pStreetStallDlg;
	cNpcScriptDialog	* m_pNpcScriptDlg;
	cHelpDialog			* m_pHelpDlg;

//KES
	CMacroDialog		* m_pMacroDlg;
//	CMacroModeDialog	* m_pMacroModeDlg;
	CChatOptionDialog	* m_pChatOptionDlg;
	COptionDialog		* m_pOptionDlg;
	CExitDialog			* m_pExitDlg;
	CCharStateDialog	* m_pCharStateDlg;
	CMenuSlotDialog		* m_pMenuSlotDlg;
	CDissolutionDialog	* m_pDissolutionDlg;
	CEventNotifyDialog	* m_pEventNotifyDlg;

	cVSyncDialog		* m_pVsyncDlg;

	cAuctionBoard		* m_pAuctionBoard;

	CMoneyDlg			* m_MoneyDlg;
	CQuickDialog		* m_pQuickDlg;
	CGuageDialog		* m_pGuageDlg;

	CPartyDialog		m_PartyDlg;
	CPartyCreateDlg		* m_pPartyCreateDlg;
	CPartyInviteDlg		* m_pPartyInviteDlg;

	CGuildCreateDialog	* m_pGuildCreateDlg;
	CGuildDialog		* m_pGuildDlg;
	CGuildInviteDialog	* m_pGuildInviteDlg;

	CGuildMarkDialog	* m_pGuildMarkDlg;
	CGuildLevelUpDialog	* m_pGuildLevelUpDlg;
	CGuildNickNameDialog* m_pGuildNickNameDlg;
	CGuildRankDialog	* m_pGuildRankDlg;
	CGuildWarehouseDialog * m_pGuildWarehouseDlg;
	CGuildUnionCreateDialog * m_pGuildUnionCreateDlg;

	CMiniMapDlg			* m_pMiniMapDlg;
	CBigMapDlg			* m_pBigMapDlg;

	CPyogukDialog		* m_pPyogukDialog;
	CNoteDialog			* m_pNoteDialog;
	CFriendDialog		* m_pFriendDialog;
	CMiniNoteDialog		* m_pMiniNoteDialog;
	CMiniFriendDialog	* m_pMiniFriendDialog;


	CMiniMaryDialog     *  m_pMiniMaryDialog;  // minimarry dialog define 2014-05-08

	CBaiShiDialog		* m_pMiniBaishiDialog; // miniBaishi dialog define 2014-05-10

	CControlDialog      * m_pControlDialog;    // bar Control dialog define 2014-05-23

	CRankButtonDialog	* m_pRankButtonDialog;

	CTopDialog          * m_pTopDialog;  // billboard dialog define 2014-05-25
	CTopDungeon          * m_pTopDungeon;
	CTopMunpDialog      * m_pTopMunpDialog;  //weiye °ï»áÅÅÐÐÍ·ÎÄ¼þÒýÓÃ 2018-01-12

	CLuckerDialog       * m_pLuckerDialog;     // Lucker dialog define 2014-06-02

	CItemDropInfoDialog	* m_pItemDropInfoDialog;

	CMoneyPacketDialog  * m_pMoneyPacketDialog; // MoneyPacketDialog define 2014-11-09

	COtherCharacerMoneyDlg * m_pOtherCharacterMoneyDlg; // other character money define 2014-11-10

	COtherCharacterAmuletDlg * m_pOtherCharacterAmuletDlg; // other character Amulet define 2014-11-10

	CItemStoneDkDialog     *   m_pItemStoneDkDlg;   // £¬ 2014-11-14

	CItemStoneXqDialog	   *   m_pItemStoneXqDlg;   // £¬ 2014-11-18

	CItemStepReinforceDialog	*	m_pItemStepReinforceDlg;

	CCharactorInfoDlg	* m_pCharactorInfo;     // Other CharacterInfo define 2014-08-11

	CQuipInfoDlg		* m_pCharacterEquipInfo;// Other CharacterEquip define 2014-08-11

	CItemLockDialog     * m_pItemLockDialog;    // the item lock window define 2014-12-01

	CItemUnLockDialog   * m_pItemUnLockDialog;  // the item unlock window define 2014-12-07

	CItemGambleDialog   * m_pItemGambleDialog;  // ¶Ä²©ÏµÍ³ 2015-01-16

	CReviveDialog		* m_pReviveDialog;
	CWantNpcDialog		* m_pWantNpcDialog;
	CWantRegistDialog	* m_pWantRegistDialog;
	CMonsterGuageDlg	* m_pMonsterGuageDlg;
	CUpgradeDlg			* m_pUpgradeDlg;
	CReinforceDlg		* m_pReinforceDlg;
	CDissloveDlg		* m_pDissolveDlg;

	CQuestTotalDialog			* m_pQuestTotalDlg;
	CQuestDialog				* m_pQuestDlg;
	CWantedDialog				* m_pWantedDlg;
	CJournalDialog				* m_pJournalDlg;

	CBailDialog					* m_pBailDlg;

	CMPRegistDialog		* m_pMPRegistDialog;
	CMPMissionDialog	* m_pMPMissionDialog;
	CMPNoticeDialog		* m_pMPNoticeDialog;
	CMPGuageDialog		* m_pMPGuageDialog;
	
	CItemShopDialog		* m_pItemShopDialog;
	CMoveDialog			* m_pMoveDialog;
	CPointSaveDialog	* m_pPointSaveDialog;

	// guildfieldwar
	CGFWarDeclareDlg*	m_pGFWarDeclareDlg;
	CGFWarResultDlg*	m_pGFWarResultDlg;
	CGFWarInfoDlg*		m_pGFWarInfoDlg;
	CGuildWarInfoDlg*	m_pGuildWarInfoDlg;
	CGuildPlusTimeDlg*  m_pGuildPlusTimeDlg;//Add 060803 by wonju
	///////////////////////////////////////////////////////////////////////
	CGuildMunhaDialog*	m_pGuildMunhaDlg;
	CGuildTraineeDialog*	m_pGuildTraineeDlg;

	//
	CSkillPointRedist*	m_pSkPointDlg;
	CSkillPointNotify*	m_pSkPointNotifyDlg;

	// partywar
	CPartyWarDialog*	m_pPartyWarDlg;

	// ¿ÜÄ¡±â
	CShoutDialog*		m_pShoutDlg;
	CShoutchatDialog*	m_pShoutchatDlg;

	//
	CChaseDialog*		m_pChaseDlg;
	CChaseinputDialog*	m_pChaseinputDlg;

	//
	CNameChangeDialog*	m_pNameChangeDlg;
	CNameChangeNotifyDlg*	m_pNameChangeNotifyDlg;

	// Guild Tournament
	CGTRegistDialog*			m_pGTRegistDlg;
	CGTRegistcancelDialog*		m_pGTRegistcancelDlg;
	CGTStandingDialog*			m_pGTStandingDlg;
	CGTBattleListDialog*		m_pGTBattleListDlg;
	CGTScoreInfoDialog*			m_pGTScoreInfoDlg;

	// seigewar
	CSWTimeRegDlg*				m_pSWTimeRegDlg;
	CSWProtectRegDlg*			m_pSWProtectRegDlg;
	CSWInfoDlg*					m_pSWInfoDlg;
	CSWStartTimeDialog*			m_pSWStartTimeDlg;

	// Character change
	CCharChangeDlg*			m_pCharChangeDlg;
	
	// ItemSeal
	CSealDialog*			m_pSealDlg;

	CSWProfitDlg*				m_pSWProfitDlg;
	CSWEngraveDialog*			m_pEngraveDlg;
	CSWTimeDialog*				m_pSWTimeDlg;

	//
	CChangeJobDialog*			m_pChangeJobDlg;

	//
	CReinforceResetDlg*			m_pReinforceResetDlg;
	CRareCreateDialog*			m_pRareCreateDlg;

	//SW051129 Pet
	CPetStateDlg*				m_pPetStateDlg;
	CPetStateMiniDlg*			m_pPetStateMiniDlg;
	CPetInventoryDlg*			m_pPetInventoryDlg;
	CPetUpgradeDialog*			m_pPetUpgradeDlg;
	CPetRevivalDialog*			m_pPetRevivalDlg;

	CPetMultiDlg*				m_pPetMultiDlg;

	// 06. 02. °­È­ Àç·á ¾È³» ÀÎÅÍÆäÀÌ½º Ãß°¡ - ÀÌ¿µÁØ
	CReinforceDataGuideDlg*		m_pReinforceDataGuideDlg;
	// 06. 02. ³»Á¤º¸Ã¢ ÆÁº¸±â Ãß°¡ - ÀÌ¿µÁØ
	CTipBrowserDlg*				m_pTipBrowserDlg;
	CKeySettingTipDlg*			m_pKeySettingTipDlg;

	CGuildNoteDlg*				m_pGuildNoteDlg;
	CUnionNoteDlg*				m_pUnionNoteDlg;

	CGuildNoticeDlg*			m_pGuildNoticeDlg;

	/////////////////////////////////////////////////////////////
	// 06. 06. 2Â÷ ÀüÁ÷ - ÀÌ¿µÁØ
	// ¹«°ø º¯È¯
	CSkillOptionChangeDlg*		m_pSkillOptionChangeDlg;
	CSkillOptionClearDlg*		m_pSkillOptionClearDlg;
	/////////////////////////////////////////////////////////////

	//SW060713 ¹®ÇÏ»ý
	CGuildJoinDialog* m_pGuildInvitationKindSelectDlg;

	//½ºÅ©¸°¼¦
	CScreenShotDlg*			m_pScreenShotDlg;

	//SW061019 ¼­¹ÙÀÌ¹ú¸ðµå
	CSurvivalCountDialog*	m_pSurvivalCountDlg;
	
	// magi82 - UniqueItem(070703)
	CUniqueItemCurseCancellationDlg* m_pUniqueItemCurseCancellationDlg;
	// magi82 - UniqueItem(070709)
	CUniqueItemMixDlg* m_pUniqueItemMixDlg;

	// magi82 - SOS(070724)
	CSOSDlg* m_pSOSDlg;

	// autonote
	CAutoNoteDlg*	m_pAutoNoteDlg;
	CAutoAnswerDlg*	m_pAutoAnswerDlg;

	// ¿ä»õÀü
	CFWEngraveDialog*	m_pFWEngraveDlg;
	CFWTimeDialog*		m_pFWTimeDlg;
	CFWWareHouseDialog*	m_pFWWareHouseDlg;

	CUniqueItemMixProgressBarDlg*	m_pUniqueItemMixProgressBarDlg;		//2007. 10. 22. CBH - À¯´ÏÅ© ¾ÆÀÌÅÆ Á¶ÇÕ Ã³¸® ¹Ù °ÔÀÌÁö
	CTitanMixProgressBarDlg*		m_pTitanMixProgressBarDlg;		//2007. 10. 22. CBH - Å¸ÀÌÅº Á¶ÇÕ Ã³¸® ¹Ù °ÔÀÌÁö
	CTitanPartsProgressBarDlg*		m_pTitanPartsProgressBarDlg;		//2007. 10. 23. CBH - Å¸ÀÌÅº ÆÄÃ÷ Á¶ÇÕ Ã³¸® ¹Ù °ÔÀÌÁö

	CSkinSelectDialog* m_pSkinSelectDlg;	//2007. 12. 4. CBH - ½ºÅ² ¼±ÅÃ ´ÙÀÌ¾ó·Î±×
	CCostumeSkinSelectDialog* m_pCostumeSkinSelectDlg;	//2008. 1. 16. CBH - ÄÚ½ºÆ¬ ½ºÅ² ¼±ÅÃ ´ÙÀÌ¾ó·Î±×

	CSkillPointResetDlg* m_pSkillPointResetDlg;	// magi82(42) - ¼¥¾ÆÀÌÅÛ Ãß°¡(¼ö·ÃÄ¡ ÃÊ±âÈ­)

	CNumberPadDialog* m_pNumberPadDlg;		//2008. 3. 17. CBH - º¸¾È¹øÈ£ ´ÙÀÌ¾ó·Î±×

	// magi82(47)
	CStreetStallItemViewDlg* m_pStreetStallItemViewDlg;

	CPartyMatchingDlg* m_pPartyMatchingDlg;		//2008. 5. 23. CBH - ¹æÆÄ ¸ÅÄª ´ÙÀÌ¾ó·Î±×

	CMallShopDialog* m_MallShopDialog;          // 2014-05-01 Create MallItem Window

	CGoldShopDialog* m_GoldShopDialog;			// 2014-08-16 Create GoldItem Window
	CGoldShopBuyDialog * m_GoldShopBuyDialog;

	//CSafeNumberDialog  * m_SafeNumberDialog;    //weiye ´´½¨°²È«Âë´°¿Ú 2017-12-07

	CSafeNumberIcon    * m_SafeNumberIcon;         //weiye ´´½¨°²È«Âë´°¿Ú 2017-12-07

	CSafeNumberImg	   * m_SafeNumberImg;          //weiye ´´½¨°²È«ËøÍ¼±ê 2018-02-01

	//CSafeNumberChangeDialog * m_SafeNumberChangeDialog;   //weiye ´´½¨ÐÞ¸Ä°²È«ËøÃÜÂë´°¿Ú 2018-04-08

	//CSafeNumberLockDialog * m_SafeNumberLockDialog;             //weiye ´´½¨Ëø¶¨°²È«ËøÃÜÂë´°¿Ú 2018-04-08

	CTopIcon           * m_TopIcon;               //weiye ´´½¨Ð¡µØÍ¼ÏÂ·½Í¼±ê´°¿Ú 2018-01-15

	CFBTimeDialog	   * m_FBTimeDialog;          //weiye ¸±±¾Ê±¼ä¼ÇÊ±´°¿Ú¶¨Òå 2018-02-06

	CFameGoldDialog	   * m_FameGoldDialog;        //weiye Ôª±¦Ï´E´°¿Ú¶¨Òå 2018-02-10

//	CharacterControlDialog   *  m_CharacterControlDialog; //weiye ´´½¨½ÇÉ«¹¦ÄÜ´°¿Ú 2018-02-12

	CSendEquipDialog *  m_SendEquipDialog;        //weiye ´´½¨·¢ËÍ×°±¸´°¿Ú 2018-03-14

	CSendEquipSelectDialog *  m_SendEquipSelectDialog;        //weiye ´´½¨·¢ËÍ×°±¸ÏÔÊ¾´°¿Ú 2018-03-14

	CVideoCaptureDlg  *          m_VideoCaptureDialog;         //weiye ÆÁÄ»Â¼Ïñ´°¿Ú±äÁ¿ 2018-05-12

	CVideoCaptureIcon *         m_VideoCaptureIcon;         //weiye ÆÁÄ»Â¼Ïñ´°¿ÚÍ¼±ê 2018-05-14

	CGuildWarDialog		* m_pGuildWarDialog;


	CCodeDialog	*	m_CodeDialog;

	CEventPlayTimeDlg	*	m_EventPlayTimeDlg;

	CInsFUBENMissionString* m_InsDGMissionString;

	CInsDGEntranceInfoDlg * m_InsDGEntranceInfoDlg;

	CJackFlashMsg				* m_JackFlashDialog;
	
	CHideBuff	*	m_bHideBuffDlg;

	AdPointDlg*					m_pAdPointDlg;

	CReconnectDialog*						m_ReconnectDlgDLG;

	RegID*						m_RegIDDLG;
	RestPwd*					m_RestPwdDLG;

	CharacterRBTN*				m_CharacterRBtn;

	CLuckyDrawIcon*		m_LuckyDrawIcon;

	CBloodStormIcon*	m_BloodStormIcon;

	cChangeLookDialog*	m_ChangeLookDialog;

	CFPSDialog *         m_FPSDialog;

	CJackRebornInfo *         m_JackRebornInfo;

	CInsDGInfoDialog* m_pInsDGInfoDlg;	//2008. 7. 24. CBH - ÀÎ´ø Á¤º¸ ´ÙÀÌ¾ó·Î±×
	CInsDGMissionInfoDialog* m_pInsDGMissionInfoDlg;	//2008. 7. 25. CBH - ÀÎ´ø ¹Ì¼Ç Á¤º¸ ´ÙÀÌ¾ó·Î±×
	CInsDGEntranceInfoDialog* m_pInsDGEntranceInfoDlg;		//2008. 7. 29. CBH - ÀÎ´ø ÀÔÀå Á¶°Ç Á¤º¸ ´ÙÀÌ¾ó·Î±×
	CInsDGPartyMakeDialog* m_pInsDGPartyMakeDlg;		//2008. 8. 5. CBH - ÀÎ´ø ÀÔÀå½Ã ÆÄÆ¼°¡ ¾ùÀ»¶§ ¶ç¿ì´Â ´ÙÀÌ¾ó·Î±×
	CInsDGRankDialog* m_pInsDGRankDialog;		//2008. 9. 2. CBH - ÀÎ½ºÅÏ½º ´øÀü ·©Å© ´ÙÀÌ¾ó·Î±×
	CInsDGRankInfoDialog* m_pInsDGRankInfoDialog;
	COfficialUpGradeDlg* m_OfficialUpGradeDlg;
	cHousingWebDlg*			m_pHousingWebDlg;

	CFadeDlg*				m_pFadeDlg;
	CItemQualityDlg* m_QualityDlg;                //×°±¸¾õÐÑ
	CItemQualityChangeDlg* m_QualityChangeDlg;     //×°±¸×ª»»
	CGradeChangeDlg* m_GGDDlg;				//ÎäÆ÷Éý½×Öµ×ªÒÆ¾í

	VipDialog* m_VipDialog;          //VIP´°¿Ú¶¨Òå
	CCustomizingNameDialog* m_pCustomizingDlg;    //×Ô¶¨Òå³ÆºÅ
#ifdef _MUTIPET_
	PetMixDlg* m_PetMixDlg;//µ¶¸ç  3petºÏ³É´°¿Ú
#endif
	BOOL	m_bInitForGame;
	int		m_GameInInitKind;	
	DWORD	m_MovePoint;
	DWORD	m_MoveMap;
	DWORD	m_GuildIdx;		// Guild Tournament
	DWORD	m_BattleIdx;	// Guild Tournament - ObserverMode
	DWORD	m_SiegeWarMapNum;		// SiegeWar MapNum

public:	

	BOOL m_bGameInAcked;
	CGameIn();
	virtual ~CGameIn();

//	//MAKESINGLETON(CGameIn)
	BOOL Init(void* pInitParam);
	void Release(CGameState* pNextGameState);

//KES
	BOOL InitForGame();			//°ÔÀÓ·Îµù->°ÔÀÓ ÀÏ¶§ ÃÊ±âÈ­
	BOOL GetInitForGame()	{	return m_bInitForGame;	}		//2007. 6. 18. CBH - °ÔÀÓÀÏ¶§ ±¸ºÐ
	void ReleaseForGame();		//°ÔÀÓ->°ÔÀÓ·Îµù ÀÏ¶§ ¸±¸®½º
//

	void Process();
	void BeforeRender();
	void AfterRender();

	void NetworkMsgParse(BYTE Category,BYTE Protocol,void* pMsg);

	void SetMovePoint(DWORD point){ m_MovePoint = point; }
	
	BOOL IsGameInAcked()		{	return m_bGameInAcked;	}
	int GetGameInInitKind()	{ return m_GameInInitKind; 	}

	virtual void OnDisconnect();
	virtual BOOL IsDisconnect();

	DWORD GetMoveMap()			{	 return m_MoveMap;	}
	void SetGuildIdx( DWORD GuildIdx )		{	m_GuildIdx = GuildIdx;	}
	void SetMoveMap( DWORD MapNum )			{	m_MoveMap = MapNum;		}
	void SetBattleIdx( DWORD BattleIdx )	{	m_BattleIdx = BattleIdx;	}
	void SetSiegeWarMapNum( DWORD MapNum )	{	m_SiegeWarMapNum = MapNum;	}
	
	///// custom
	CMonsterGuageDlg	*	GetMonsterGuageDlg()	{ return m_pMonsterGuageDlg;	}
	void	SetMonsterGuageDlg(CMonsterGuageDlg	* dlg)	{ m_pMonsterGuageDlg = dlg;	}
	CCharacterPvpDialog* GetCharacterPvPDialog() { return m_pCharPvPDlg; } //pvpÊôÐÔ
	void SetCharacterPvPDialog(CCharacterPvpDialog* dlg) { m_pCharPvPDlg = dlg; }
	CCharacterDialog * GetCharacterDialog(){ return m_pCharDlg; }
	void SetCharacterDialog(CCharacterDialog * dlg){ m_pCharDlg=dlg; }
	CMainBarDialog	* GetMainInterfaceDialog(){ return m_pMainDlg; }
	void SetMainInterfaceDialog(CMainBarDialog	* Dlg){ m_pMainDlg=Dlg; }

	void SetChatDialog(CChatDialog	* Dlg){ m_pChatDlg=Dlg; }
	CChatDialog * GetChatDialog(){ return m_pChatDlg; }

	//SW050722 attach to minimap
//	void SetJackpotDialog(cJackpotDialog * Dlg){ m_pJackpotDlg=Dlg; }
//	cJackpotDialog* GetJackpotDialog(){ return m_pJackpotDlg; }


	void SetDebugDlg(CDebugDlg* Dlg){m_pDebugDlg = Dlg;}
	CDebugDlg* GetDebugDlg(){return m_pDebugDlg;}

	CMugongSuryunDialog * GetMugongSuryunDialog(){ return m_pMugongSuryunDlg; }
	void SetMugongSuryunDialog(CMugongSuryunDialog * dlg){ m_pMugongSuryunDlg=dlg; }

	CMugongDialog * GetMugongDialog(){ return m_pMugongDlg; }
	void SetMugongDialog(CMugongDialog * dlg){ m_pMugongDlg=dlg; }
	CSuryunDialog * GetSuryunDialog(){ return m_pSuryunDlg; }
	void SetSuryunDialog(CSuryunDialog * dlg){ m_pSuryunDlg=dlg; }

    CUpgradeDlg * GetUpgradeDialog(){ return m_pUpgradeDlg; }
	void SetUpgradeDialog(CUpgradeDlg * dlg){ m_pUpgradeDlg=dlg; }
	CInventoryExDialog * GetInventoryDialog(){ return m_pInventoryDlg; }
	void SetInventoryDialog(CInventoryExDialog * dlg){ m_pInventoryDlg=dlg; }
	CExchangeDialog * GetExchangeDialog(){ return m_pExchangeDlg; }
	void SetExchangeDialog(CExchangeDialog * dlg){ m_pExchangeDlg=dlg; }
//	CRecvExchangeDialog * GetRecvExchangeDialog(){ return m_pRecvExchangeDlg; }
//	void SetRecvExchangeDialog(CRecvExchangeDialog * dlg){ m_pRecvExchangeDlg=dlg; }
	CMixDialog * GetMixDialog(){ return m_pMixDlg; }
	void SetMixDialog(CMixDialog * dlg){ m_pMixDlg=dlg; }
	// magi82 - Titan(070112) ////////////////////////////////////////////////
	CTitanPartsMakeDlg * GetTitanPartsMakeDlg(){ return m_pTitanPartsMakeDlg; }
	void SetTitanPartsMakeDlg(CTitanPartsMakeDlg * dlg){ m_pTitanPartsMakeDlg=dlg; }
	//////////////////////////////////////////////////////////////////////////

	// magi82 - Titan(070119) ////////////////////////////////////////////////
	CTitanMixDlg * GetTitanMixDlg(){ return m_pTitanMixDlg; }
	void SetTitanMixDlg(CTitanMixDlg * dlg){ m_pTitanMixDlg=dlg; }
	//////////////////////////////////////////////////////////////////////////

	// 2007. 9. 11. CBH - Å¸ÀÌÅº ¼ö¸®Ã¢ ////////////////////////////////////////////////
	CTitanRepairDlg* GetTitanRepairDlg(){ return m_pTitanRepairDlg; }
	void SetTitanRepairDlg(CTitanRepairDlg * dlg){ m_pTitanRepairDlg=dlg; }
	//////////////////////////////////////////////////////////////////////////

	// 2007. 9. 13. CBH - Å¸ÀÌÅº ¼ÒÈ¯ °ÔÀÌÁö ////////////////////////////////////////////////
	CTitanRecallDlg* GetTitanRecallDlg(){ return m_pTitanRecallDlg; }
	void SetTitanRecallDlg(CTitanRecallDlg * dlg){ m_pTitanRecallDlg=dlg; }
	//////////////////////////////////////////////////////////////////////////

	// magi82 - Titan(070119) ////////////////////////////////////////////////
	CTitanUpgradeDlg* GetTitanUpgradeDlg(){ return m_pTitanUpgradeDlg; }
	void SetTitanUpgradeDlg(CTitanUpgradeDlg * dlg){ m_pTitanUpgradeDlg=dlg; }
	//////////////////////////////////////////////////////////////////////////

	// magi82 - Titan(070119) ////////////////////////////////////////////////
	CTitanBreakDlg* GetTitanBreakDlg(){ return m_pTitanBreakDlg; }
	void SetTitanBreakDlg(CTitanBreakDlg * dlg){ m_pTitanBreakDlg=dlg; }
	//////////////////////////////////////////////////////////////////////////

	// magi82 - Titan(070212) ////////////////////////////////////////////////
	CTitanChangePreViewDlg* GetTitanChangePreViewDlg(){ return m_pTitanChangePreViewDlg; }
	void SetTitanChangePreViewDlg(CTitanChangePreViewDlg * dlg){ m_pTitanChangePreViewDlg=dlg; }
	//////////////////////////////////////////////////////////////////////////

	// magi82 - Titan(070222) ////////////////////////////////////////////////
	CTitanInventoryDlg* GetTitanInventoryDlg(){ return m_pTitanInventoryDlg; }
	void SetTitanInventoryDlg(CTitanInventoryDlg * dlg){ m_pTitanInventoryDlg=dlg; }
	//////////////////////////////////////////////////////////////////////////

	// magi82 - Titan(070305) ////////////////////////////////////////////////
	CTitanGuageDlg* GetTitanGuageDlg(){ return m_pTitanGuageDlg; }
	void SetTitanGuageDlg(CTitanGuageDlg * dlg){ m_pTitanGuageDlg=dlg; }
	//////////////////////////////////////////////////////////////////////////

	// magi82 - Titan(070611) Å¸ÀÌÅº ¹«°øº¯È¯ ÁÖ¼®Ã³¸®
	/*
	CTitanMugongMixDlg* GetTitanMugongMixDlg(){ return m_pTitanMugongMixDlg; }
	void SetTitanMugongMixDlg(CTitanMugongMixDlg * dlg){ m_pTitanMugongMixDlg=dlg; }
	*/

	// magi82 - Titan(070320) ////////////////////////////////////////////////
	CTitanRegisterDlg* GetTitanRegisterDlg(){ return m_pTitanRegisterDlg; }
	void SetTitanRegisterDlg(CTitanRegisterDlg * dlg){ m_pTitanRegisterDlg=dlg; }
	//////////////////////////////////////////////////////////////////////////

	// magi82 - Titan(070320) ////////////////////////////////////////////////
	CTitanDissolutionDlg* GetTitanDissolutionDlg(){ return m_pTitanDissolutionDlg; }
	void SetTitanDissolutionDlg(CTitanDissolutionDlg * dlg){ m_pTitanDissolutionDlg=dlg; }
	//////////////////////////////////////////////////////////////////////////

	CReinforceDlg * GetReinforceDialog(){ return m_pReinforceDlg; }
	void SetReinforceDialog(CReinforceDlg * dlg){ m_pReinforceDlg=dlg; }
	CDissloveDlg * GetDissolveDialog(){ return m_pDissolveDlg; }
	void SetDissolveDialog(CDissloveDlg * dlg){ m_pDissolveDlg=dlg; }

	// 2014-05-01 Create MallItem Window

	void SetMallShopDialog( CMallShopDialog* pDlg ) { m_MallShopDialog = pDlg;  }
	CMallShopDialog* GetMallShopDialog() { return m_MallShopDialog; }

	// 2014-08-16 Create ShopItem Window

	void SetGoldShopDialog( CGoldShopDialog* pDlg ) { m_GoldShopDialog = pDlg;  }
	CGoldShopDialog* GetGoldShopDialog() { return m_GoldShopDialog; }

	CGoldShopBuyDialog * GetGoldShopBuyDialog(){return  m_GoldShopBuyDialog;}
	void SetGoldShopBuyDialog(CGoldShopBuyDialog * dlg){m_GoldShopBuyDialog=dlg;}

		
	CDealDialog * GetDealDialog(){ return m_pDealDlg; }
	void SetDealDialog(CDealDialog * dlg){ m_pDealDlg=dlg; }

	// LYJ 051017 ±¸ÀÔ³ëÁ¡»ó Ãß°¡
	CStallKindSelectDlg * GetStallKindSelectDialog(){ return m_pStallKindSelectDlg; }
	void SetStallKindSelectDialog(CStallKindSelectDlg * dlg){ m_pStallKindSelectDlg=dlg; }	
	CStreetBuyStall * GetStreetBuyStallDialog(){ return m_pStreetBuyStallDlg; }
	void SetStreetBuyStallDialog(CStreetBuyStall * dlg){ m_pStreetBuyStallDlg=dlg; }
	CBuyRegDialog * GetBuyRegDialog(){ return m_pBuyRegDlg; }
	void SetBuyRegDialog(CBuyRegDialog * dlg){ m_pBuyRegDlg=dlg; }
	
	//////////////////////////////////////////////////////////////////////////
	// ³ëÁ¡»ó °Ë»ö ´ÙÀÌ¾ó·Î±× by Stiner(8)
	CStallFindDlg * GetStallFindDlg(){ return m_pStallFindDlg; };
	void SetStallFindDlg(CStallFindDlg * dlg){ m_pStallFindDlg = dlg; };

// LBS
	CStreetStall * GetStreetStallDialog(){ return m_pStreetStallDlg; }
	void SetStreetStallDialog(CStreetStall * dlg){ m_pStreetStallDlg=dlg; }
	cNpcScriptDialog* GetNpcScriptDialog() { return m_pNpcScriptDlg; }
	void SetNpcScriptDialog(cNpcScriptDialog* dlg) { m_pNpcScriptDlg = dlg; }
	cHelpDialog* GetHelpDialog() { return m_pHelpDlg; }
	void SetHelpDialog(cHelpDialog* dlg) { m_pHelpDlg = dlg; }
////
	CMoneyDlg* GetMoneyDialog()	{	return m_MoneyDlg;	}
	void SetMoneyDialog(CMoneyDlg* dlg)	{	m_MoneyDlg = dlg;	}
	CQuickDialog* GetQuickDialog()	{	return m_pQuickDlg;	}
	void SetQuickDialog(CQuickDialog* dlg)	{	m_pQuickDlg = dlg;	}

//KES
	CMacroDialog * GetMacroDialog(){ return m_pMacroDlg; }
	void SetMacroDialog(CMacroDialog * dlg){ m_pMacroDlg=dlg; }
//	CMacroModeDialog * GetMacroModeDialog(){ return m_pMacroModeDlg; }
//	void SetMacroModeDialog(CMacroModeDialog * dlg){ m_pMacroModeDlg=dlg; }
	CChatOptionDialog * GetChatOptionDialog() { return m_pChatOptionDlg; }
	void SetChatOptionDialog(CChatOptionDialog * dlg) { m_pChatOptionDlg = dlg; }	
	COptionDialog * GetOptionDialog() { return m_pOptionDlg; }
	void SetOptionDialog(COptionDialog * dlg) { m_pOptionDlg = dlg; }
	CExitDialog * GetExitDialog() { return m_pExitDlg; }
	void SetExitDialog(CExitDialog * dlg) { m_pExitDlg = dlg; }
	CCharStateDialog* GetCharStateDialog() { return m_pCharStateDlg; }
	void SetCharStateDialog(CCharStateDialog* dlg) { m_pCharStateDlg = dlg; }

	void SetMenuSlotDialog( CMenuSlotDialog* pDlg ) { m_pMenuSlotDlg = pDlg; }
	CMenuSlotDialog* GetMenuSlotDialog() { return m_pMenuSlotDlg; }

	void SetDissolutionDialog( CDissolutionDialog* pDlg ) { m_pDissolutionDlg = pDlg; }
	CDissolutionDialog* GetDissolutionDialog() { return m_pDissolutionDlg; }

	void SetEventNotifyDialog( CEventNotifyDialog* pDlg ) { m_pEventNotifyDlg = pDlg; }
	CEventNotifyDialog* GetEventNotifyDialog() { return m_pEventNotifyDlg; }
	

	void SetVsyncDialog(cVSyncDialog* pDlg) { m_pVsyncDlg = pDlg; }
	cVSyncDialog* GetVsyncDialog() { return m_pVsyncDlg; }

	cAuctionBoard * GetAuctionBoard(){ return m_pAuctionBoard; }
	void SetAuctionBoard(cAuctionBoard * dlg){ m_pAuctionBoard=dlg; }

	CGuageDialog * GetGuageDlg(){ return m_pGuageDlg; }
	void SetGuageDlg(CGuageDialog * dlg){ m_pGuageDlg=dlg; }

	CPartyDialog* GetPartyDialog() { return &m_PartyDlg; }
	
	CPartyCreateDlg* GetPartyCreateDialog() { return m_pPartyCreateDlg;	}
	void SetPartyCreateDialog(CPartyCreateDlg* dlg)	{ m_pPartyCreateDlg = dlg;	}

	CPartyInviteDlg* GetPartyInviteDialog() { return m_pPartyInviteDlg;	}
	void SetPartyInviteDialog(CPartyInviteDlg* dlg)	{ m_pPartyInviteDlg = dlg;	}
	
	CGuildCreateDialog* GetGuildCreateDlg() { return m_pGuildCreateDlg; }
	void SetGuildCreateDlg(CGuildCreateDialog* dlg) { m_pGuildCreateDlg = dlg;	}

	CGuildUnionCreateDialog* GetGuildUnionCreateDlg() { return m_pGuildUnionCreateDlg; }
	void SetGuildUnionCreateDlg(CGuildUnionCreateDialog* dlg) { m_pGuildUnionCreateDlg = dlg;	}	
	
	CGuildDialog* GetGuildDlg() { return m_pGuildDlg; }
	void SetGuildDlg(CGuildDialog* dlg) { m_pGuildDlg = dlg;	}

	CGuildInviteDialog* GetGuildInviteDlg() { return m_pGuildInviteDlg; }
	void SetGuildInviteDlg(CGuildInviteDialog* dlg) { m_pGuildInviteDlg = dlg;	}
	
	CGuildMarkDialog* GetGuildMarkDlg() { return m_pGuildMarkDlg; }
	void SetGuildMarkDlg(CGuildMarkDialog* dlg) { m_pGuildMarkDlg = dlg;	}
	
	CGuildLevelUpDialog* GetGuildLevelUpDlg() { return m_pGuildLevelUpDlg; }
	void SetGuildLevelUpDlg(CGuildLevelUpDialog* dlg) { m_pGuildLevelUpDlg = dlg;	}

	CGuildNickNameDialog* GetGuildNickNameDlg() { return m_pGuildNickNameDlg; }
	void SetGuildNickNameDlg(CGuildNickNameDialog* dlg) { m_pGuildNickNameDlg = dlg;	}

	CGuildRankDialog* GetGuildRankDlg() { return m_pGuildRankDlg; }
	void SetGuildRankDlg(CGuildRankDialog* dlg) { m_pGuildRankDlg = dlg;	}
	
	CGuildWarehouseDialog* GetGuildWarehouseDlg() { return m_pGuildWarehouseDlg; }
	void SetGuildWarehouseDlg(CGuildWarehouseDialog* dlg) { m_pGuildWarehouseDlg = dlg;	}

	CMiniMapDlg* GetMiniMapDialog() { return m_pMiniMapDlg; 	}
	void SetMiniMapDialog(CMiniMapDlg* dlg) { m_pMiniMapDlg = dlg; 	}

	CBigMapDlg* GetBigMapDialog() { return m_pBigMapDlg; 	}
	void SetBigMapDialog(CBigMapDlg* dlg) { m_pBigMapDlg = dlg; 	}

	CPyogukDialog* GetPyogukDialog() { return m_pPyogukDialog;	}
	void SetPyogukDialog(CPyogukDialog* dlg) { m_pPyogukDialog = dlg; 	}
	
	CFriendDialog* GetFriendDialog() { return m_pFriendDialog; }
	void SetFriendDialog(CFriendDialog* dlg) { m_pFriendDialog = dlg; }

	CNoteDialog* GetNoteDialog() { return m_pNoteDialog; }
	void SetNoteDialog(CNoteDialog* dlg) { m_pNoteDialog = dlg;	}
	
	CMiniNoteDialog* GetMiniNoteDialog() { return m_pMiniNoteDialog;	}
	void SetMiniNoteDialog(CMiniNoteDialog* dlg) { m_pMiniNoteDialog = dlg;	}
	
	CMiniFriendDialog * GetMiniFriendDialog() { return m_pMiniFriendDialog; 	}
	void SetMiniFriendDialog(CMiniFriendDialog* dlg) { m_pMiniFriendDialog = dlg;	}

	// marry window define 2014-05-08

	CMiniMaryDialog * GetMiniMaryDialog() { return m_pMiniMaryDialog; 	}
	void SetMiniMaryDialog(CMiniMaryDialog* dlg) { m_pMiniMaryDialog = dlg;	}

	// Baishi window define 2014-05-10

	CBaiShiDialog* GetMiniBaishiDialog() { return m_pMiniBaishiDialog ;}
	void SetMiniBaishiDialog(CBaiShiDialog* dlg) { m_pMiniBaishiDialog = dlg; }

	// Bar Control Window define 2014-05-23

	CControlDialog* GetControlDialog() {return m_pControlDialog;}

	void SetControlDialog(CControlDialog* dlg){m_pControlDialog=dlg;}

	CRankButtonDialog* GetRankButtonDialog() {return m_pRankButtonDialog;}
	void SetRankButtonDialog(CRankButtonDialog* dlg){m_pRankButtonDialog=dlg;}

	// Billboard Window define 2014-05-25

	CTopDialog * GetTopDialog(){return m_pTopDialog;}

	void SetTopDialog(CTopDialog* dlg){m_pTopDialog=dlg;}

	CTopDungeon * GetTopDungeon(){ return m_pTopDungeon; }

	void SetTopDungeon(CTopDungeon* dlg){ m_pTopDungeon = dlg; }
	// Billboard Window define 2018-01-10

	CTopMunpDialog * GetTopMunpDialog(){return m_pTopMunpDialog;}

	void SetTopMunpDialog(CTopMunpDialog* dlg){m_pTopMunpDialog=dlg;}

	// LuckerDialog window define 2014-06-02

	CLuckerDialog * GetLuckerDialog(){return m_pLuckerDialog;}

	void SetLuckerDialog(CLuckerDialog* dlg){m_pLuckerDialog=dlg;}

	CItemDropInfoDialog * GetItemDropInfoDialog(){ return m_pItemDropInfoDialog; }

	void SetItemDropInfoDialog(CItemDropInfoDialog* dlg){ m_pItemDropInfoDialog = dlg; }

	// MoneyPacketDialog window define 2014-11-09

	CMoneyPacketDialog * GetMoneyPacketDialog(){return m_pMoneyPacketDialog;}

	void SetMoneyPacketDialog(CMoneyPacketDialog *dlg){m_pMoneyPacketDialog=dlg;}

    // OtherCharacterMoneyDlg window define 2014-11-10

	COtherCharacerMoneyDlg * GetOtherCharacterMoneyDlg(){return m_pOtherCharacterMoneyDlg;}

	void SetOtherCharacterMoneyDlg(COtherCharacerMoneyDlg *dlg){m_pOtherCharacterMoneyDlg=dlg;}

    // OtherCharacterAmuletDlg window define 2014-11-10

	COtherCharacterAmuletDlg * GetOtherCharacterAmuletDlg(){return m_pOtherCharacterAmuletDlg;}

	void SetOtherCharacterAmuletDlg(COtherCharacterAmuletDlg *dlg){m_pOtherCharacterAmuletDlg=dlg;}

	// ÏµÍ³ 2014-11-14 

	CItemStoneDkDialog * GetItemStoneDkDlg(){return  m_pItemStoneDkDlg;}

	void SetItemStoneDkDlg(CItemStoneDkDialog *dlg){m_pItemStoneDkDlg=dlg;}

	// ÏµÍ³ 2014-11-18

	CItemStoneXqDialog * GetItemStoneXqDlg(){return m_pItemStoneXqDlg;}
	void SetItemStoneXqDlg(CItemStoneXqDialog * dlg){m_pItemStoneXqDlg=dlg;}

	CItemStepReinforceDialog * GetItemStepReinforceDlg(){return m_pItemStepReinforceDlg;}
	void SetItemStepReinforceDlg(CItemStepReinforceDialog * dlg){m_pItemStepReinforceDlg=dlg;}

	// 2014-08-11 Create OtherCharacterinfo Window

	CCharactorInfoDlg* GetCharacterInfoDlg(){return m_pCharactorInfo;}
	CQuipInfoDlg*	GetCharacterEquipInfoDlg(){return m_pCharacterEquipInfo;}
	void SetCharacterInfoDialog(CCharactorInfoDlg * dlg){ m_pCharactorInfo = dlg; }
	void SetShowEquipInfoDlg(CQuipInfoDlg *dlg) { m_pCharacterEquipInfo = dlg;};

	// 2014-12-01 Create ItemLockDlalog  Window

	CItemLockDialog * GetItemLockDialog(){return m_pItemLockDialog;}

	void SetItemLockDialog(CItemLockDialog *dlg){m_pItemLockDialog=dlg;}

	// 2014-12-07 Create ItemUnLockDialog Window

	CItemUnLockDialog * GetItemUnLockDialog(){return m_pItemUnLockDialog;}

	void SetItemUnLockDialog(CItemUnLockDialog *dlg){m_pItemUnLockDialog=dlg;}

	// 2015-01-16 ¶Ä²©ÏµÍ³!

	CItemGambleDialog * GetItemGambleDialog(){return m_pItemGambleDialog;}

	void SetItemGambleDialog(CItemGambleDialog * dlg){m_pItemGambleDialog=dlg;}

	 //weiye 2017-12-05  °²È«Âë´°¿Ú¶¨Òå¡¡

	//CSafeNumberDialog * GetSafeNumberDialog(){return m_SafeNumberDialog;}
	//void SetSafeNumberDialog(CSafeNumberDialog * dlg){m_SafeNumberDialog=dlg;}

	//weiye 2017-12-05  °²È«Âë´°¿ÚÍ¼±ê¶¨Òå

	CSafeNumberIcon * GetSafeNumberIcon(){return m_SafeNumberIcon;}

	void SetSafeNumberIcon(CSafeNumberIcon * dlg){m_SafeNumberIcon=dlg;}

	//weiye 2018-02-01 °²È«ËøÍ¼±ê¶¨Òå 

	CSafeNumberImg  * GetSafeNumberImg(){return m_SafeNumberImg;}

	void SetSafeNumberImg(CSafeNumberImg * dlg){m_SafeNumberImg=dlg;}

	//weiye ´´½¨ÐÞ¸Ä°²È«ËøÃÜÂë´°¿Ú 2018-04-08

	//CSafeNumberChangeDialog * GetSafeNumberChangeDialog(){return m_SafeNumberChangeDialog;}
	//void SetSafeNumberChangeDialog(CSafeNumberChangeDialog * dlg){m_SafeNumberChangeDialog=dlg;}

    //weiye ´´½¨Ëø¶¨°²È«Ëø´°¿Ú 2018-04-08

	//CSafeNumberLockDialog * GetSafeNumberLockDialog(){return m_SafeNumberLockDialog;}
	//void SetSafeNumberLockDialog(CSafeNumberLockDialog * dlg){m_SafeNumberLockDialog=dlg;}

    //weiye 2017-10-22 ´´½¨Ð¡µØÍ¼ÏÂ·½´°¿Ú  2018-01-15

	CTopIcon * GetTopIcon(){return m_TopIcon;}

	void SetTopIcon(CTopIcon * dlg){m_TopIcon=dlg;}

	//weiye 2017-02-06 ´´½¨FBÊ±¼ä´°¿Ú 

	CFBTimeDialog * GetFBTimeDialog(){return m_FBTimeDialog;}

	void SetFBTimeDialog(CFBTimeDialog* dlg){m_FBTimeDialog=dlg;}

	//weiye 2018-02-10  Ôª±¦Ï´E´°¿Ú·ÃÎÊ

	CFameGoldDialog * GetFameGoldDialog(){return  m_FameGoldDialog;}

	void SetFameGoldDialog(CFameGoldDialog* dlg){m_FameGoldDialog=dlg;}

	//weiye 2017-11-27 ´´½¨½ÇÉ«¹¦ÄÜ´°¿Ú 2017-11-27

//	CharacterControlDialog * GetCharacterControlDialog(){return m_CharacterControlDialog;}

//	void   SetCharacterControlDialog(CharacterControlDialog * dlg){m_CharacterControlDialog=dlg;}

	//weiye 2018-03-14 ·¢ËÍ×°±¸´°¿Ú  

	CSendEquipDialog * GetSendEquipDialog(){return m_SendEquipDialog;}

	void SetSendEquipDialog(CSendEquipDialog* dlg){m_SendEquipDialog=dlg;}

    //weiye 2018-03-14 ·¢ËÍ×°±¸ÏÔÊ¾´°¿Ú  

	CSendEquipSelectDialog * GetSendEquipSelectDialog(){return m_SendEquipSelectDialog;}

	void SetSendEquipSelectDialog(CSendEquipSelectDialog* dlg){m_SendEquipSelectDialog=dlg;}

	//weiye 2018-05-12 ÆÁÄ»Â¼Ïñ´°¿Ú 2018-05-12  

	CVideoCaptureDlg * GetVideoCaptureDlg(){return m_VideoCaptureDialog;}

	void SetVideoCaptureDlg(CVideoCaptureDlg* dlg){m_VideoCaptureDialog=dlg;}

	//weiye 2018-05-14 ÆÁÄ»Â¼Ïñ´°¿ÚÍ¼±ê  2018-05-14  

	CVideoCaptureIcon * GetVideoCaptureIcon(){return m_VideoCaptureIcon;}

	void SetVideoCaptureIcon(CVideoCaptureIcon* dlg){m_VideoCaptureIcon=dlg;}
	
	CReviveDialog * GetReviveDialog() { return m_pReviveDialog; 	}

	void SetReviveDialog(CReviveDialog* dlg) { m_pReviveDialog = dlg;	}

	CWantNpcDialog * GetWantNpcDialog() { return m_pWantNpcDialog; 	}
	void SetWantNpcDialog(CWantNpcDialog* dlg) { m_pWantNpcDialog = dlg; }
	

	CWantRegistDialog * GetWantRegistDialog() { return m_pWantRegistDialog; }
	void SetWantRegistDialog(CWantRegistDialog * dlg) { m_pWantRegistDialog = dlg;	}

	CQuestTotalDialog * GetQuestTotalDialog() { return m_pQuestTotalDlg;	}
	void SetQuestTotalDialog(CQuestTotalDialog * dlg) { m_pQuestTotalDlg = dlg;	}
	CQuestDialog* GetQuestDialog()			{ return m_pQuestDlg;	}
	void SetQuestDialog(CQuestDialog* dlg)	{ m_pQuestDlg = dlg;	}
	CWantedDialog* GetWantedDialog() { return m_pWantedDlg; }
	void SetWantedDialog(CWantedDialog* dlg) { m_pWantedDlg = dlg;	}
	CJournalDialog* GetJournalDialog() { return m_pJournalDlg;	}
	void SetJournalDialog(CJournalDialog* dlg) { m_pJournalDlg = dlg; }

	void SetMPRegistDialog(CMPRegistDialog * dlg) { m_pMPRegistDialog = dlg; }
	CMPRegistDialog* GetMPRegistDialog() { return m_pMPRegistDialog;	}
	void SetMPMissionDialog(CMPMissionDialog* dlg) { m_pMPMissionDialog = dlg; }
	CMPMissionDialog* GetMPMissionDialog() { return m_pMPMissionDialog;	}
	void SetMPNoticeDialog(CMPNoticeDialog* dlg) { m_pMPNoticeDialog = dlg; }
	CMPNoticeDialog* GetMPNoticeDialog() { return m_pMPNoticeDialog;	}
	void SetMPGuageDialog(CMPGuageDialog* dlg) { m_pMPGuageDialog = dlg; }
	CMPGuageDialog* GetMPGuageDialog() { return m_pMPGuageDialog;	}

	void SetPetStateDialog(CPetStateDlg* dlg) { m_pPetStateDlg = dlg; }
	CPetStateDlg* GetPetStateDialog() { return m_pPetStateDlg; }
	void SetPetStateMiniDialog(CPetStateMiniDlg* dlg) { m_pPetStateMiniDlg = dlg; }
	CPetStateMiniDlg* GetPetStateMiniDialog() { return m_pPetStateMiniDlg; }
	void SetPetInventoryDialog(CPetInventoryDlg* dlg) { m_pPetInventoryDlg = dlg; }
	CPetInventoryDlg* GetPetInventoryDialog() { return m_pPetInventoryDlg; }
	void SetPetUpgradeDialog(CPetUpgradeDialog* dlg) {	m_pPetUpgradeDlg = dlg;	}
	CPetUpgradeDialog* GetPetUpgradeDialog() {	return m_pPetUpgradeDlg;	}
	void SetPetRevivalDialog(CPetRevivalDialog* dlg) {	m_pPetRevivalDlg = dlg;	}
	CPetRevivalDialog* GetPetRevivalDialog() {	return m_pPetRevivalDlg;	}


	//---------------------------------------------------------------------------------
	void SetPetMultiDialog(CPetMultiDlg* dlg) { m_pPetMultiDlg = dlg; }
	CPetMultiDlg* GetPetMultiDialog() { return m_pPetMultiDlg; }


	//---------------------------------------------------------------------------------


	CBailDialog* GetBailDialog()			{	return m_pBailDlg;	}
	void SetBailDialog(CBailDialog* dlg)	{	m_pBailDlg = dlg; }

	CItemShopDialog* GetItemShopDialog()			{	return m_pItemShopDialog;	}
	void SetItemShopDialog(CItemShopDialog* dlg)	{	m_pItemShopDialog = dlg; 	}
	
	CMoveDialog* GetMoveDialog()					{	return m_pMoveDialog;	}
	void SetMoveDialog(CMoveDialog* dlg)			{	m_pMoveDialog = dlg;	}
	CPointSaveDialog* GetPointSaveDialog()			{	return m_pPointSaveDialog;	}
	void SetPointSaveDialog(CPointSaveDialog* dlg)	{	m_pPointSaveDialog = dlg;	}

	// guildfieldwar
	CGFWarDeclareDlg* GetGFWarDeclareDlg()			{	return m_pGFWarDeclareDlg;	}
	void SetGFWarDeclareDlg(CGFWarDeclareDlg* dlg)	{	m_pGFWarDeclareDlg = dlg;	}
	CGFWarResultDlg* GetGFWarResultDlg()			{	return m_pGFWarResultDlg;	}
	void SetGFWarResultDlg(CGFWarResultDlg* dlg)	{	m_pGFWarResultDlg = dlg;	}
	CGFWarInfoDlg* GetGFWarInfoDlg()				{	return m_pGFWarInfoDlg;	}
	void SetGFWarInfoDlg(CGFWarInfoDlg* dlg)		{	m_pGFWarInfoDlg = dlg;	}
	CGuildWarInfoDlg* GetGuildWarInfoDlg()			{	return m_pGuildWarInfoDlg;	}
	void SetGuildWarInfoDlg(CGuildWarInfoDlg* dlg)	{	m_pGuildWarInfoDlg = dlg;	}
	CGuildPlusTimeDlg* GetGuildPlusTimeDlg()			{	return m_pGuildPlusTimeDlg;	}//Add 060803 by wonju
	void SetGuildPlusTimeDlg(CGuildPlusTimeDlg* dlg)	{	m_pGuildPlusTimeDlg = dlg;	}//Add 060803 by wonju
	
	// 
	CSkillPointRedist*	GetSkPointDlg()					{	return m_pSkPointDlg;	}
	void SetSkPointDlg(CSkillPointRedist* dlg)			{	m_pSkPointDlg = dlg;	}
	CSkillPointNotify*	GetSkPointNotifyDlg()			{	return m_pSkPointNotifyDlg;	}
	void SetSkPointNotifydlg(CSkillPointNotify* dlg)	{	m_pSkPointNotifyDlg = dlg;	}

	// partywar
	CPartyWarDialog*	GetPartyWarDlg()				{	return m_pPartyWarDlg;	}
	void SetPartyWarDlg(CPartyWarDialog* dlg)			{	m_pPartyWarDlg = dlg;	}

	// ¿ÜÄ¡±â
	CShoutDialog*	GetShoutDlg()						{	return m_pShoutDlg;		}
	void SetShoutDlg(CShoutDialog* dlg)					{	m_pShoutDlg = dlg;		}
	CShoutchatDialog*	GetShoutchatDlg()				{	return m_pShoutchatDlg;		}
	void SetShoutchatDlg(CShoutchatDialog* dlg)			{	m_pShoutchatDlg = dlg;		}

	//
	CChaseDialog*	GetChaseDlg()						{	return m_pChaseDlg;		}
	void SetChaseDlg(CChaseDialog* dlg)					{	m_pChaseDlg = dlg;		}
	CChaseinputDialog*	GetChaseinputDlg()					{	return m_pChaseinputDlg;	}
	void SetChaseinputDlg(CChaseinputDialog* dlg)			{	m_pChaseinputDlg = dlg;		}

	//
	CNameChangeDialog*	GetNameChangeDlg()				{	return m_pNameChangeDlg;	}
	void SetNameChangeDlg(CNameChangeDialog* dlg)		{	m_pNameChangeDlg = dlg;		}
	CNameChangeNotifyDlg*	GetNameChangeNotifyDlg()		{	return m_pNameChangeNotifyDlg;	}
	void SetNameChangeNotifyDlg(CNameChangeNotifyDlg* dlg)	{	m_pNameChangeNotifyDlg = dlg;		}

	//
	CRareCreateDialog*	GetRareCreateDlg()				{	return m_pRareCreateDlg;	}
	void SetRareCreateDialog( CRareCreateDialog* dlg )	{	m_pRareCreateDlg = dlg;		}

	//½ºÅ©¸°¼¦
	CScreenShotDlg*	GetScreenShotDlg()				{	return m_pScreenShotDlg;	}
	void SetScreenShotDlg( CScreenShotDlg* dlg )	{	m_pScreenShotDlg = dlg;		}

	//SW061019 ¼­¹ÙÀÌ¹ú¸ðµå

	CSurvivalCountDialog* GetSurvivalCountDlg()		{	return m_pSurvivalCountDlg;	}
	void SetSurvivalCountDialog(CSurvivalCountDialog* dlg)	{ m_pSurvivalCountDlg = dlg;	}

	BOOL	m_bDieInBossMap;
	int		m_nCountInBossMap;

	DWORD	m_dwTimeInBossMap;

	void	ProcessInBossMap();


	// Guild Tournament
	CGTRegistDialog*	GetGTRegistDlg()					{	return m_pGTRegistDlg;			}
	void SetGTRegistDlg(CGTRegistDialog* dlg)				{	m_pGTRegistDlg = dlg;			}
	CGTRegistcancelDialog*	GetGTRegistcancelDlg()			{	return m_pGTRegistcancelDlg;	}
	void SetGTRegistcancelDlg(CGTRegistcancelDialog* dlg)	{	m_pGTRegistcancelDlg = dlg;		}
	CGTStandingDialog*	GetGTStandingDlg()					{	return m_pGTStandingDlg;		}
	void SetGTStandingDlg(CGTStandingDialog* dlg)			{	m_pGTStandingDlg = dlg;			}
	CGTBattleListDialog*	GetGTBattleListDlg()			{	return m_pGTBattleListDlg;		}
	void SetGTBattleListDlg(CGTBattleListDialog* dlg)		{	m_pGTBattleListDlg = dlg;		}
	CGTScoreInfoDialog*	GetGTScoreInfoDlg()					{	return m_pGTScoreInfoDlg;		}
	void SetGTScoreInfoDlg(CGTScoreInfoDialog* dlg)			{	m_pGTScoreInfoDlg = dlg;		}

	// seigewar
	CSWTimeRegDlg*		GetSWTimeRegDlg()					{ return m_pSWTimeRegDlg; }
	void				SetSWTimeRegDlg( CSWTimeRegDlg* dlg )	{ m_pSWTimeRegDlg = dlg; }
	CSWProtectRegDlg*	GetSWProtectRegDlg()				{ return m_pSWProtectRegDlg; }
	void				SetSWProtectRegDlg( CSWProtectRegDlg* dlg )	{ m_pSWProtectRegDlg = dlg; }
	CSWInfoDlg*			GetSWInfoDlg()						{ return m_pSWInfoDlg; }
	void				SetSWInfoDlg( CSWInfoDlg* dlg )		{ m_pSWInfoDlg = dlg; }
	CSWProfitDlg*		GetSWProfitDlg()					{ return m_pSWProfitDlg; }
	void				SetSWProfitDlg( CSWProfitDlg* dlg )	{ m_pSWProfitDlg = dlg; }
	CSWEngraveDialog*	GetSWEngraveDlg()					{ return m_pEngraveDlg;	}
	void				SetSWEngraveDlg( CSWEngraveDialog* dlg)	{	m_pEngraveDlg = dlg;	}
	CSWTimeDialog*		GetSWTimeDlg()						{ return m_pSWTimeDlg;	}
	void				SetSWTimeDlg( CSWTimeDialog* dlg)	{ m_pSWTimeDlg = dlg;	}
	CSWStartTimeDialog*	GetSWStartTimeDlg()					{ return m_pSWStartTimeDlg;	}
	void				SetSWStartTimeDlg( CSWStartTimeDialog* dlg )	{ m_pSWStartTimeDlg = dlg; }
	
	// Character change
	CCharChangeDlg* GetCharChangeDlg()						{	return m_pCharChangeDlg;		}
	void SetCharChangeDlg( CCharChangeDlg* dlg )			{	m_pCharChangeDlg = dlg;			}

	// ItemSeal
	CSealDialog* GetSealDlg()								{	return m_pSealDlg;			}
	void SetSealDlg( CSealDialog* dlg )						{	m_pSealDlg = dlg;			}

	// 
	CChangeJobDialog*	GetChangeJobDlg()					{	return m_pChangeJobDlg;		}
	void				SetChangeJobDlg( CChangeJobDialog* dlg )		{	m_pChangeJobDlg = dlg;		}

	// ReinforceReset
	CReinforceResetDlg*	GetReinforceResetDlg()				{	return m_pReinforceResetDlg;	}
	void				SetReinforceResetDlg( CReinforceResetDlg* dlg )		{	m_pReinforceResetDlg = dlg;		}

	// 06. 02. °­È­ Àç·á ¾È³» ÀÎÅÍÆäÀÌ½º Ãß°¡ - ÀÌ¿µÁØ
	CReinforceDataGuideDlg* GetReinforceDataGuideDlg()	{ return m_pReinforceDataGuideDlg; }
	void SetReinforceDataGuideDlg(CReinforceDataGuideDlg* dlg) { m_pReinforceDataGuideDlg = dlg; }

	// 06. 02. ³»Á¤º¸Ã¢ ÆÁº¸±â Ãß°¡ - ÀÌ¿µÁØ
	CTipBrowserDlg* GetTipBrowserDlg() { return m_pTipBrowserDlg; }
	void SetTipBrowserDlg(CTipBrowserDlg* dlg) { m_pTipBrowserDlg = dlg; }

	CKeySettingTipDlg* GetKeySettingTipDlg() { return m_pKeySettingTipDlg; }
	void SetKeySettingTipDlg(CKeySettingTipDlg* dlg) { m_pKeySettingTipDlg = dlg; }

	CGuildNoteDlg*	GetGuildNoteDlg() { return	m_pGuildNoteDlg; }
	void SetGuildNoteDlg(CGuildNoteDlg* dlg) { m_pGuildNoteDlg = dlg; }
	CUnionNoteDlg*	GetUnionNoteDlg() { return	m_pUnionNoteDlg; }
	void SetUnionNoteDlg(CUnionNoteDlg* dlg) { m_pUnionNoteDlg = dlg; }

	// 06. 03. ¹®ÆÄ°øÁö - ÀÌ¿µÁØ
	CGuildNoticeDlg* GetGuildNoticeDlg() { return m_pGuildNoticeDlg; }
	void SetGuildNoticeDlg(CGuildNoticeDlg *dlg) { m_pGuildNoticeDlg = dlg; }

	////////////////////////////////////////////////////////////////////////
	// 06. 06. 2Â÷ ÀüÁ÷ - ÀÌ¿µÁØ
	// ¹«°ø º¯È¯
	CSkillOptionChangeDlg* GetSkillOptionChangeDlg() { return m_pSkillOptionChangeDlg; }
	void SetSkillOptionChangeDlg(CSkillOptionChangeDlg *dlg) { m_pSkillOptionChangeDlg = dlg; }
	
	CSkillOptionClearDlg* GetSkillOptionClearDlg() { return m_pSkillOptionClearDlg; }
	void SetSkillOptionClearDlg(CSkillOptionClearDlg *dlg) { m_pSkillOptionClearDlg = dlg; }
	////////////////////////////////////////////////////////////////////////

	//SW060713 ¹®ÇÏ»ý
	CGuildJoinDialog* GetGuildJoinDialog() {	return m_pGuildInvitationKindSelectDlg;	}
	void SetGuildJoinDialog(CGuildJoinDialog* dlg)	{	m_pGuildInvitationKindSelectDlg = dlg;	}
	CGuildMunhaDialog* GetGuildMunhaDialog()				{	return m_pGuildMunhaDlg;	}
	void SetGuildMunhaDialog(CGuildMunhaDialog* dlg)		{	m_pGuildMunhaDlg = dlg;		}
	CGuildTraineeDialog* GetGuildTraineeDialog()			{	return m_pGuildTraineeDlg;	}
	void SetGuildTraineeDialgo(CGuildTraineeDialog* dlg)	{	m_pGuildTraineeDlg = dlg;	}

	// magi82 - UniqueItem(070703)
	CUniqueItemCurseCancellationDlg* GetUniqueItemCurseCancellationDlg() {	return m_pUniqueItemCurseCancellationDlg;	}
	void SetUniqueItemCurseCancellationDlg(CUniqueItemCurseCancellationDlg* dlg) {	m_pUniqueItemCurseCancellationDlg = dlg;	}

	// magi82 - UniqueItem(070709)
	CUniqueItemMixDlg* GetUniqueItemMixDlg() {	return m_pUniqueItemMixDlg;	}
	void SetUniqueItemMixDlg(CUniqueItemMixDlg* dlg) {	m_pUniqueItemMixDlg = dlg;	}

	// magi82 - SOS(070724)
	CSOSDlg* GetSOSDlg()	{	return m_pSOSDlg;	}
	void SetSOSDlg(CSOSDlg* dlg)	{	m_pSOSDlg = dlg;	}	

	// 2007. 10. 22. CBH - À¯´ÏÅ© ¾ÆÀÌÅÆ Ã³¸® ¹Ù °ÔÀÌÁö /////////////////////////////////
	CUniqueItemMixProgressBarDlg* GetUniqueItemMixProgressBarDlg(){ return m_pUniqueItemMixProgressBarDlg; }
	void SetUniqueItemMixProgressBarDlg(CUniqueItemMixProgressBarDlg * dlg){ m_pUniqueItemMixProgressBarDlg=dlg; }
	//////////////////////////////////////////////////////////////////////////	

	// 2007. 10. 22. CBH - Å¸ÀÌÅº Á¶ÇÕ °ÔÀÌÁö /////////////////////////////////
	CTitanMixProgressBarDlg* GetTitanMixProgressBarDlg(){ return m_pTitanMixProgressBarDlg; }
	void SetTitanMixProgressBarDlg(CTitanMixProgressBarDlg * dlg){ m_pTitanMixProgressBarDlg=dlg; }
	//////////////////////////////////////////////////////////////////////////	

	// 2007. 10. 23. CBH - Å¸ÀÌÅº ÆÄÃ÷ Á¶ÇÕ °ÔÀÌÁö /////////////////////////////////
	CTitanPartsProgressBarDlg* GetTitanPartsProgressBarDlg(){ return m_pTitanPartsProgressBarDlg; }
	void SetTitanPartsProgressBarDlg(CTitanPartsProgressBarDlg * dlg){ m_pTitanPartsProgressBarDlg=dlg; }
	//////////////////////////////////////////////////////////////////////////	

	// 2007. 12. 4. CBH - ½ºÅ² ¼±ÅÃ ´ÙÀÌ¾ó·Î±× /////////////////////////////////
	CSkinSelectDialog* GetSkinSelectDlg(){ return m_pSkinSelectDlg; }

	void SetSkinSelectDlg(CSkinSelectDialog * dlg){ m_pSkinSelectDlg=dlg; }
	//////////////////////////////////////////////////////////////////////////	

	// 2008. 1. 16. CBH - ÄÚ½ºÆ¬ ½ºÅ² ¼±ÅÃ ´ÙÀÌ¾ó·Î±× /////////////////////////////////
	CCostumeSkinSelectDialog* GetCostumeSkinSelectDlg(){ return m_pCostumeSkinSelectDlg; }
	void SetCostumeSkinSelectDlg(CCostumeSkinSelectDialog * dlg){ m_pCostumeSkinSelectDlg=dlg; }
	//////////////////////////////////////////////////////////////////////////	

    // magi82(42) - ¼¥¾ÆÀÌÅÛ Ãß°¡(¼ö·ÃÄ¡ ÃÊ±âÈ­)
	CSkillPointResetDlg* GetSkillPointResetDlg()	{	return m_pSkillPointResetDlg;	}
	void SetSkillPointResetDlg(CSkillPointResetDlg* dlg)	{	m_pSkillPointResetDlg = dlg;	}

	// magi82(47)
	CStreetStallItemViewDlg* GetStreetStallItemViewDlg()	{	return m_pStreetStallItemViewDlg;	}
	void SetStreetStallItemViewDlg(CStreetStallItemViewDlg* dlg)	{	m_pStreetStallItemViewDlg = dlg;	}

	// 2008. 3. 17. CBH - º¸¾È¹øÈ£ ´ÙÀÌ¾ó·Î±× /////////////////////////////////
	CNumberPadDialog* GetNumberPadDlg(){ return m_pNumberPadDlg; }
	void SetNumberPadDlg(CNumberPadDialog * dlg){ m_pNumberPadDlg=dlg; }

	// autonote
	CAutoNoteDlg* GetAutoNoteDlg()				{ return m_pAutoNoteDlg; }
	void SetAutoNoteDlg( CAutoNoteDlg* dlg )	{ m_pAutoNoteDlg = dlg; }
	CAutoAnswerDlg* GetAutoAnswerDlg()				{ return m_pAutoAnswerDlg; }
	void SetAutoAnswerDlg( CAutoAnswerDlg* dlg )	{ m_pAutoAnswerDlg = dlg; }

	// ¿ä»õÀü
	CFWEngraveDialog* GetFWEngraveDlg()				{ return m_pFWEngraveDlg; }
	void SetFWEngraveDlg( CFWEngraveDialog* dlg )	{ m_pFWEngraveDlg = dlg; }
	CFWTimeDialog* GetFWTimeDlg()					{ return m_pFWTimeDlg; }
	void SetFWTimeDlg( CFWTimeDialog* dlg )			{ m_pFWTimeDlg = dlg; }
	CFWWareHouseDialog* GetFWWareHouseDlg()				{ return m_pFWWareHouseDlg; }
	void SetFWWareHouseDlg( CFWWareHouseDialog* dlg )	{ m_pFWWareHouseDlg = dlg; }
	//////////////////////////////////////////////////////////////////////////	

	// 2008. 5. 23. CBH - ¹æÆÄ ¸ÅÄª ´ÙÀÌ¾ó·Î±× /////////////////////////////////
	CPartyMatchingDlg* GetPartyMatchingDlg(){ return m_pPartyMatchingDlg; }
	void SetPartyMatchingDlg(CPartyMatchingDlg * dlg){ m_pPartyMatchingDlg=dlg; }
	//////////////////////////////////////////////////////////////////////////	

	CGuildWarDialog* GetGuildWarDialog(){return m_pGuildWarDialog;}
	void SetGuildWarDialog(CGuildWarDialog* dlg){m_pGuildWarDialog = dlg;}

	

	CCodeDialog * GetCodeDialog(){return  m_CodeDialog;}
	void SetCodeDialog(CCodeDialog* dlg){m_CodeDialog=dlg;}

	

	CEventPlayTimeDlg * GetEventPlayTimeDlg(){return m_EventPlayTimeDlg;}
	void SetEventPlayTimeDlg(CEventPlayTimeDlg*dlg){m_EventPlayTimeDlg=dlg;}

	CInsFUBENMissionString * GetInsDGMissionDialog(){ return  m_InsDGMissionString; }
	void SetInsDGMissionDialog(CInsFUBENMissionString* dlg){ m_InsDGMissionString = dlg; }

	CInsDGEntranceInfoDlg * GetInsFUBENEntranceInfoDlg(){return m_InsDGEntranceInfoDlg;}
	void SetInsFUBENEntranceInfoDlg(CInsDGEntranceInfoDlg* dlg){ m_InsDGEntranceInfoDlg = dlg; }

	CJackFlashMsg* GetJackFlashDlg() {return m_JackFlashDialog;}
	void SetJackFlashDlg(CJackFlashMsg* dlg) { m_JackFlashDialog = dlg; }


	CHideBuff* GetHideBuffDlg() {return m_bHideBuffDlg;}
	void SetHideBuffDlg(CHideBuff* dlg) { m_bHideBuffDlg = dlg; }

	AdPointDlg* GetAdPointDlg()	{return m_pAdPointDlg;}
	void SetAdPointDlg(AdPointDlg* dlg) { m_pAdPointDlg = dlg; }

	CReconnectDialog* GetReconnectDlg(){return m_ReconnectDlgDLG;}
	void SetReconnectDlg(CReconnectDialog* dlg){m_ReconnectDlgDLG=dlg;}


	RegID* GetRegID(){return m_RegIDDLG;}
	void SetRegID(RegID* dlg){m_RegIDDLG=dlg;}

	RestPwd* GetRestPwdDlg(){return m_RestPwdDLG;}
	void SetRestPwdDlg(RestPwd* dlg){m_RestPwdDLG=dlg;}

	CharacterRBTN*	GetCharacterRBTNDlg(){return m_CharacterRBtn;}
	void SetCharacterRBTN (CharacterRBTN* dlg){m_CharacterRBtn = dlg;}


	CLuckyDrawIcon * GetLuckyDrawIcon(){return m_LuckyDrawIcon;}
	void SetLuckyDrawIcon(CLuckyDrawIcon* dlg){m_LuckyDrawIcon=dlg;}

	CBloodStormIcon * GetBloodStormIcon(){return m_BloodStormIcon;}
	void SetBloodStormIcon(CBloodStormIcon* dlg){ m_BloodStormIcon = dlg; }

	cChangeLookDialog * GetChangeLookDialog(){ return m_ChangeLookDialog; }
	void SetChangeLookDialog(cChangeLookDialog* dlg){ m_ChangeLookDialog = dlg; }

	CFPSDialog * GetFPSDialog(){ return m_FPSDialog; }
	void SetFPSDialog(CFPSDialog* dlg){ m_FPSDialog = dlg; }

	CJackRebornInfo * GetJackRebornInfo(){ return m_JackRebornInfo; }
	void SetJackRebornInfo(CJackRebornInfo* dlg){ m_JackRebornInfo = dlg; }



	MAPTYPE GetInsDungeonMapNum() { return m_wInsDungeonMapNum; }
	void SetInsDungeonMapNum(MAPTYPE wDungeonMapNum) { m_wInsDungeonMapNum = wDungeonMapNum; }
	// 2008. 7. 24. CBH - ÀÎ´ø Á¤º¸ ´ÙÀÌ¾ó·Î±× /////////////////////////////////
	CInsDGInfoDialog* GetInsDGInfoDlg() { return m_pInsDGInfoDlg; }
	void SetInsDGInfoDlg(CInsDGInfoDialog* dlg) { m_pInsDGInfoDlg = dlg; }
	//////////////////////////////////////////////////////////////////////////	
	// 2008. 7. 25. CBH - ÀÎ´ø ¹Ì¼Ç Á¤º¸ ´ÙÀÌ¾ó·Î±× /////////////////////////////////
	CInsDGMissionInfoDialog* GetInsDGMissionInfoDlg() { return m_pInsDGMissionInfoDlg; }
	void SetInsDGMissionInfoDlg(CInsDGMissionInfoDialog* dlg) { m_pInsDGMissionInfoDlg = dlg; }
	//////////////////////////////////////////////////////////////////////////	
	// 2008. 7. 29. CBH - ÀÎ´ø ÀÔÀå Á¶°Ç Á¤º¸ ´ÙÀÌ¾ó·Î±× /////////////////////////////////
	CInsDGEntranceInfoDialog* GetInsDGEntranceInfoDlg() { return m_pInsDGEntranceInfoDlg; }
	void SetInsDGEntranceInfoDlg(CInsDGEntranceInfoDialog* dlg) { m_pInsDGEntranceInfoDlg = dlg; }
	//////////////////////////////////////////////////////////////////////////	
	// 2008. 8. 5. CBH - ÀÎ´ø ÀÔÀå½Ã ÆÄÆ¼°¡ ¾øÀ»¶§ ¶ç¿ì´Â ´ÙÀÌ¾ó·Î±× /////////////////////////////////
	CInsDGPartyMakeDialog* GetInsDGPartyMakeDlg() { return m_pInsDGPartyMakeDlg; }
	void SetInsDGPartyMakeDlg(CInsDGPartyMakeDialog* dlg) { m_pInsDGPartyMakeDlg = dlg; }

	// 2008. 9. 2. CBH - ÀÎ½ºÅÏ½º ´øÀü ·©Å© ´ÙÀÌ¾ó·Î±× /////////////////////////////////
	CInsDGRankDialog* GetInsDGRankDialog() { return m_pInsDGRankDialog; }
	void SetInsDGRankDialog(CInsDGRankDialog* dlg) { m_pInsDGRankDialog = dlg; }
	//////////////////////////////////////////////////////////////////////////

	// 2008. 9. 5. CBH - ÀÎ½ºÅÏ½º ´øÀü ·©Å© Á¤º¸ ´ÙÀÌ¾ó·Î±× /////////////////////////////////
	CInsDGRankInfoDialog* GetInsDGRankInfoDialog() { return m_pInsDGRankInfoDialog; }
	void SetInsDGRankInfoDialog(CInsDGRankInfoDialog* dlg) { m_pInsDGRankInfoDialog = dlg; }


	void SetHousingWebDlg( cHousingWebDlg* pDlg )		{ m_pHousingWebDlg = pDlg; }
	cHousingWebDlg* GetHousingWebDlg()					{ return m_pHousingWebDlg; }

	void SetFadeDlg(CFadeDlg* pDlg)							{ m_pFadeDlg = pDlg; }
	CFadeDlg* GetFadeDlg()										{ return m_pFadeDlg; }

	void UpdataGoldMoney();
	BOOL CopyToClipboard(const char* pszData, const int nDataLen);

	//×°±¸¾õÐÑ
	CItemQualityDlg* GetItemQualityDlg() { return m_QualityDlg; }
	void SetItemQualityDlg(CItemQualityDlg* dlg) { m_QualityDlg = dlg; }

	//×°±¸Æ·ÖÊ×ª»»
	CItemQualityChangeDlg* GetItemQualityChangeDlg() { return m_QualityChangeDlg; }
	void SetItemQualityChangeDlg(CItemQualityChangeDlg* dlg) { m_QualityChangeDlg = dlg; }
	//ÎäÆ÷Éý½×Öµ×ªÒÆ¾í
	CGradeChangeDlg* GetGradeChangeDlg() { return m_GGDDlg; }
	void SetGradeChangeDlg(CGradeChangeDlg* dlg) { m_GGDDlg = dlg; }
	//////////////////////////////////////////////////////////////////////////
	COfficialUpGradeDlg* GetOfficialUpGradeDlg() { return m_OfficialUpGradeDlg; }
	void SetOfficialUpGradeDlg(COfficialUpGradeDlg* dlg) { m_OfficialUpGradeDlg = dlg; }
	//´´½¨VIP´°¿Ú
	VipDialog* GetVipDialog() { return m_VipDialog; }
	void SetVipDialog(VipDialog* dlg) { m_VipDialog = dlg; }

	//×Ô¶¨Òå³ÆºÅ
	CCustomizingNameDialog* GetCustomizingDlg() { return m_pCustomizingDlg; }
	void SetCustomizingDlg(CCustomizingNameDialog* dlg) { m_pCustomizingDlg = dlg; }

#ifdef _MUTIPET_
	PetMixDlg* GetPetMixDlg() { return  m_PetMixDlg; }
	void SetPetMixDlg(PetMixDlg* dlg) { m_PetMixDlg = dlg; }

#endif

	SYSTEMTIME	GameinServerTime;			//¼­¹ö¿¡¼­ ¹Þ¾Æ¿Â Ä³¸¯ÅÍ ·Î±×ÀÎ ½Ã°£
	DWORD				m_Clientdate;		//Å¬¶óÀÌ¾ðÆ®¿¡¼­ ¹Þ¾Æ¿Â Ä³¸¯ÅÍ ·Î±×ÀÎ ³¯Â¥
	DWORD				m_Clienttime;		//Å¬¶óÀÌ¾ðÆ®¿¡¼­ ¹Þ¾Æ¿Â Ä³¸¯ÅÍ ·Î±×ÀÎ ½Ã°£

	void	SetLoginTime(SYSTEMTIME Time);
	SYSTEMTIME	GetLoginTime();

	void	GetClientLoginTime(DWORD& date, DWORD& time);
	void	SetClientLoginTime(DWORD date, DWORD time);



	UINT GetAdapterCharacteristics(char* adapter_name);
	int  GetMAC(BYTE mac[18]);
	void GetMacAddress( char* mac );
	//////////////////////////////////////////////////////////////////////////

	std::string GetInternetIP();
	void  InitInternetIP(){ m_CurClientIP.clear(); m_CurClientMac.clear(); }
	BOOL CheckIP(const char * szIP);
	void GetHWID(char* HWID);

	//Flag
	DWORD					m_UpGoldMoneyTime;
	unsigned				m_uiAccTick;
	int						m_iMilliSecend;
	unsigned				m_uiCurrentTime;
	unsigned				m_uiPreTime;
	float					m_fFramePerSec;
	int						m_iFPSCnt;
	std::string				m_CurClientIP;
	std::string				m_CurClientMac;

	MAPTYPE	m_wInsDungeonMapNum;
	//////////////////////////////////////////////////////////////////////////	

	// BY JACK --- Add functions to process network msg.
public :
	/// USER CONNECTION PART.
	void UserConn_NetworkMsgParse(BYTE Protocol,void* pMsg) ;

	void Quest_NetworkMsgParse(BYTE Protocol,void* pMsg) ;


	void NPC_NetworkMsgParse(BYTE Protocol,void* pMsg) ;



	void SIGNAL_NetworkMsgParse(BYTE Protocol,void* pMsg) ;

	void CLIENT_NetworkMsgParse(BYTE Protocol,void* pMsg) ;

	void MURIM_NetworkMsgParse(BYTE Protocol,void* pMsg) ;

	void ITEMLIMIT_NetworkMsgParse(BYTE Protocol,void* pMsg) ;

	void MARRY_NetworkMsgParse(BYTE Protocol,void* pMsg) ;
	void SHITU_NetworkMsgParse(BYTE Protocol,void* pMsg) ;
};


EXTERNGLOBALTON(CGameIn)


#endif // !defined(AFX_GAMEIN_H__E9C036AE_2344_473B_BEFE_21C4555A0685__INCLUDED_)


