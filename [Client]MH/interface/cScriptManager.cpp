







// cScriptManager.cpp: implementation of the cScriptManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "cScriptManager.h"

#include "../GlobalEventFunc.h"
#include "../WindowIDEnum.h"
#include "cResourceManager.h"

#include "../CharacterDialog.h"
#include "../MugongSuryunDialog.h"
#include "../MugongDialog.h"
#include "../SuryunDialog.h"
#include "../InventoryExDialog.h"
#include "../ChatDialog.h"
//SW050718
//#include "../cJackpotDialog.h"
#include "../MoneyDlg.h"
#include "../DealDialog.h"
#include "../QuickDialog.h"
#include "../PartyMemberDlg.h"
#include "../PartyBtnDlg.h"
#include "../PartyCreateDlg.h"
#include "../PartyInviteDlg.h"
#include "../MousePointer.h"
#include "../MiniMapDlg.h"
#include "../BigMapDlg.h"
#include "../PyogukDialog.h"
#include "../FriendDialog.h"
#include "../NoteDialog.h"
#include "../MiniNoteDialog.h"
#include "../MiniFriendDialog.h"
#include "../ReviveDialog.h"
#include "../MacroDialog.h"
#include "../ChatOptionDialog.h"
#include "../OptionDialog.h"
#include "../ExitDialog.h"
#include "../ExchangeDialog.h"
#include "../CharStateDialog.h"
#include "../MenuSlotDialog.h"
#include "../PKLootingDialog.h"
#include "../StreetStall.h"
// LYJ 051017 ±¸ÀÔ³ëÁ¡»ó Ãß°¡
#include "../StallKindSelectDlg.h"
#include "../StreetBuyStall.h"
#include "../BuyRegDialog.h"

#include "../NpcScriptDialog.h"
#include "../HelpDialog.h"
#include "../HelperSpeechDlg.h"
#include "../MainBarDialog.h"
#include "../ObjectGuagen.h"
#include "../cComboBoxEx.h"
#include "../CharMakeDialog.h"
#include "../MonsterGuageDlg.h"
#include "../PrivateWarehouseDialog.h"
#include "../MunpaMarkDialog.h"
#include "../cDivideBox.h"
#include "../LoadingDlg.h"
#include "../UpgradeDlg.h"
#include "../MixDialog.h"
#include "../TitanPartsMakeDlg.h"	// magi82 - Titan(070112)
#include "../TitanMixDlg.h"			// magi82 - Titan(070119)
#include "../TitanRepairDlg.h"		// 2007. 9. 11. CBH
#include "../TitanRecallDlg.h"		// 2007. 9. 13. CBH
#include "../TitanUpgradeDlg.h"		// magi82 - Titan(070119)
#include "../TitanBreakDlg.h"		// magi82 - Titan(070119)
#include "../TitanGuageDlg.h"		// magi82 - Titan(070305)
//#include "../TitanMugongMixDlg.h"	// magi82 - Titan(070611) Å¸ÀÌÅº ¹«°øº¯È¯ ÁÖ¼®Ã³¸®
#include "../TitanRegisterDlg.h"	// magi82 - Titan(070320)
#include "../TitanDissolutionDlg.h"	// magi82 - Titan(070321)
#include "../TitanChangePreView.h"
#include "../TitanInventoryDlg.h"
#include "../ReinforceDlg.h"
#include "../ChannelDialog.h"
#include "../ServerListDialog.h"
#include "../WantNpcDialog.h"
#include "../WantRegistDialog.h"
#include "../QuestTotalDialog.h"
#include "../QuestDialog.h"
#include "../WantedDialog.h"
#include "../JournalDialog.h"
#include "../MPRegistDialog.h"
#include "../MPMissionDialog.h"
#include "../MPNoticeDialog.h"
#include "../MPGuageDialog.h"
#include "../ChinaAdviceDlg.h"
#include "../BailDialog.h"
#include "../EventNotifyDialog.h"
#include "../MallNoticeDialog.h"
#include "../DissolutionDialog.h"
#include "../GuildCreateDialog.h"
#include "../GuildDialog.h"
#include "../GuildInviteDialog.h"
#include "../GuildMarkDialog.h"
#include "../GuildLevelupDialog.h"
#include "../GuildNickNameDialog.h"
#include "../GuildRankDialog.h"
#include "../GuildWarehouseDialog.h"
#include "../ItemShopDialog.h"

#include "../ItemShopGridDialog.h"
#include "../ItemShopInven.h"
#include "../MoveDialog.h"
#include "../PointSaveDialog.h"
#include "../SkillPointRedist.h"
#include "../SkillPointNotify.h"
//#include "../MainDialog.h"
//#include "../WearedExDialog.h"

#include "../ShoutDialog.h"
#include "../ShoutchatDialog.h"
#include "../ChaseDialog.h"
#include "../ChaseinputDialog.h"
#include "../NameChangeDialog.h"
#include "../NameChangeNotifyDlg.h"

// Guild Tournament
#include "../GTRegistDialog.h"
#include "../GTRegistcancelDialog.h"
#include "../GTStandingDialog.h"
#include "../GTBattleListDialog.h"
#include "../GTScoreInfoDialog.h"

#include "../ItemManager.h"
//////////
//MurimNet
#include "../MNPlayRoomDialog.h"
#include "../MNChannelDialog.h"
#include "../GuildFieldWarDialog.h"
#include "../PartyWarDialog.h"
#include "../SeigeWarDialog.h"

// Character Change
#include "../CharChangeDlg.h"
#include "../SealDialog.h"
#include "../ChangeJobDialog.h"
#include "../ReinforceResetDlg.h"
#include "../GuageDialog.h"
#include "../RareCreateDialog.h"
// 06. 02. °­È­ Àç·á ¾È³» ÀÎÅÍÆäÀÌ½º Ãß°¡ - ÀÌ¿µÁØ
#include "../ReinforceDataGuideDlg.h"
// 06. 02. ³»Á¤º¸Ã¢ ÆÁº¸±â Ãß°¡ - ÀÌ¿µÁØ
#include "../TipBrowserDlg.h"
#include "../KeySettingTipDlg.h"


// Pet
#include "../PetStateDlg.h"
#include "../PetStateMiniDlg.h"
#include "../PetInventoryDlg.h"
#include "../PetUpgradeDialog.h"
#include "../PetRevivalDialog.h"

#include "../GuildNoteDlg.h"
#include "../UnionNoteDlg.h"
#include "../GuildNoticeDlg.h"

#include "../SkillOptionChangeDlg.h"
#include "../SkillOptionClearDlg.h"

#include "../GuildJoinDialog.h"

// ScreenShot
#include "../ScreenShotDlg.h"

#include "../GuildPlusTimeDialog.h"
#include "../GuildMunhaDialog.h"
#include "../GuildTraineeDialog.h"

#include "../SurvivalCountDialog.h"
#include "../UniqueItemCurseCancellationDlg.h"
#include "../UniqueItemMixDlg.h"

#include "../SOSDialog.h"
#include "../IntroReplayDlg.h"
#include "../UniqueItemMixProgressBardlg.h"		// 2007. 10. 22. CBH
#include "../TitanMixProgressBardlg.h"		// 2007. 10. 22. CBH
#include "../TitanPartsProgressBardlg.h"		// 2007. 10. 23. CBH

#include "../SkinSelectDialog.h"	// 2007. 12. 4. CBH
#include "../CostumeSkinSelectDialog.h"	// 2008. 1. 16. CBH

#include "../SkillPointResetDlg.h"

#include "../NumberPadDialog.h"	// 2008. 3. 17. CBH
#include "../StreetStallItemViewDlg.h"

#include "../StallFindDlg.h"	// ³ëÁ¡°Ë»ö Ãß°¡ by Stiner(8)
#include "../AutoNoteDlg.h"		// AutoNote
#include "../AutoAnswerDlg.h"

#include "FortWarDialog.h"

#include "../PartyMatchingDlg.h"	// 2008. 5. 23. CBH

#include "GoldShopDialog.h"         // include the GoleShop Header file  2014-08-16 QQ:^.^
#include "GoldShopBuyDialog.h"
 
#include "MiniMarryDialog.h"        // include the marrydialog Header file

#include "MiniBaishiDialog.h"       // include the shitudialog Header file

#include "ControlDialog.h"          // include the control Header file 
#include "RankButtonDialog.h" 

#include "TopDialog.h"              //weiye ÅÅÐÐÍ·ÎÄ¼þ 2018-01-10

#include "TopMunpDialog.h"          //weiye °ï»áÅÅÐÐÍ·ÎÄ¼þ 2018-01-10

#include "LuckerDialog.h"           // include the lucker Header file 

#include "MoneyPacketDialog.h"      // include the MoneyPacketDialog Header file  2014-11-08

#include "OtherCharacterMoneyDlg.h"  // include the OtherCharacterMoneyDlg Header file 2014-11-10

#include "OtherCharacterAmuletDlg.h"  // include the OtherCharacterMuletDlg Header file 2014-11-10

#include "ItemStoneDkDialog.h"       // 2014-11-14 Í·

#include "ItemStoneXqDialog.h"       // 2014-11-18 Í·
#include "ItemStepReinforceDialog.h"

#include "OtherCharacterInfo.h"     // include the character info file !

#include "OtherCharacterEquip.h"    // include the Character Equip file !

#include "cDivideBoxEx.h"           // include the cDivideBoxEx class ! 2014-10-30

#include "ItemLockManager.h"         // include the ItemLock Dialog  2014-12-01

#include "ItemUnLockManager.h"       // include the UnItemLock Dialog 2014-12-07

#include "ItemGambleManager.h"        // ÒýÓÃ¶Ä²©Í· 2015-01-16

         //weiye °²È«ËøÍ·ÎÄ¼þ¶¨Òå       2017-12-07

#include "SafeNumberIcon.h"            //weiye °²È«ËøÍ¼±ê½Ó¿Ú¡¡¡¡¡¡ 2017-12-07

#include "SafeNumberImg.h"            //weiye °²È«ËøÍ¼±ê½Ó¿Ú¡¡¡¡¡¡ 2018-01-29

//#include "SafeNumberDialog.h"
//#include "SafeNumberChangeDialog.h"  //weiye  °²È«ÂëÐÞ¸Ä´°¿Ú 2018-04-08
//#include "SafeNumberLockDialog.h"      //weiye   °²È«ÂëËø¶¨´°¿Ú 2018-04-08

#include "topIcon.h"                  //weiye Ð¡µØÍ¼ÏÂ·½Í¼±ê´°¿Ú¶¨Òå 2018-01-16

#include "FBTimeDialog.h"			 //weiye ¸±±¾Í·ÎÄ¼þÒýÓÃ 2018-02-06

#include "FameGoldDialog.h"          //weiye Ôª±¦Ï´EÍ·ÎÄ¼þ¶¨Òå 2018-02-10

//#include "CharacterControlDialog.h"  //weiye  ½ÇÉ«¹¦ÄÜ´°¿ÚÒýÓÃ  2018-02-13

#include "SendEquipDialog.h"           //weiye  ·¢ËÍ×°±¸´°¿ÚÍ·ÎÄ¼þ 2018-03-14

#include "SendEquipSelectDialog.h" //weiye  ·¢ËÍ×°±¸´°¿ÚÏÔÊ¾Í·ÎÄ¼þ 2018-03-14

#include "VideoCaptureDlg.h"          //weiye  ÆÁÄ»Â¼Ïñ´°¿Ú±äÁ¿¶¨Òå 2018-05-12

#include "VideoCaptureIcon.h"        //weiye  ÆÁÄ»Â¼Ïñ´°¿Ú±äÁ¿Í¼±ê¶¨Òå 2018-05-12

#include "GuildWarDialog.h"

#include "CodeDialog.h"

#include "EventPlayTimeDialog.h"

#include "InsFUBENMissionString.h"
#include "InsDGEntranceInfoDlg.h"

#include "JackFlashRoar.h"
#include "HideBuff.h"

#include "AdPointDlg.h"

#include "ReconnectDialog.h"

#include "RegID.h"
#include "RestPwd.h"

#include "CharacterRBTN.h"

#include "cShopItemInventPlusDlg.h"

#include "LuckyDrawIcon.h"

#include "BloodStormIcon.h"

#include "ChangeLookDialog.h"

#include "ItemDropInfoDialog.h"

#include ".\petmultidlg.h"

#include "FPSDialog.h"

#include "JackRebornInfo.h"

#include "InstancesDungeon/InsDGInfoDialog.h"		//2008. 7. 24. CBH
#include "InstancesDungeon/InsDGMissionInfoDialog.h"		//2008. 7. 25. CBH
#include "InstancesDungeon/InsDGEntranceInfoDialog.h"		//2008. 7. 29. CBH
#include "InstancesDungeon/InsDGPartyMakeDialog.h"	//2008. 8. 5. CBH
#include "InstancesDungeon/InsDGRankDialog.h"			//2008. 9. 2. CBH
#include "InstancesDungeon/InsDGRankInfoDialog.h"			//2008. 9. 5. CBH

#include "../VSyncDialog.h"

#include "cBrowser.h"
#include "cHousingWebDlg.h"

#include "FadeDlg.h"
#include "TopDungeon.h"
#include "CharacterPVPDialog.h"

#include "OfficialUpGradeDlg.h"//+30 
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
GLOBALTON(cScriptManager);
cScriptManager::cScriptManager()
{	
}

cScriptManager::~cScriptManager()
{
	ReleaseParseTypeData();
	ReleaseImagePathTable();
}

void cScriptManager::ReleaseParseTypeData()
{
	m_ParseTypeTable.SetPositionHead();
	PARSE_TYPE_INFO* pParseTypeInfo = NULL;
	while(pParseTypeInfo = m_ParseTypeTable.GetData())
	{
		m_ParseTypeTable.StartGetMultiData(pParseTypeInfo->nKeyValue);
		PARSE_TYPE_INFO* pParseTypeSubInfo = NULL;
		while( pParseTypeSubInfo = m_ParseTypeTable.GetMultiData() )
		{
			m_ParseTypeTable.Remove(pParseTypeSubInfo->nKeyValue);
			SAFE_DELETE(pParseTypeSubInfo);
		}		
	}
	m_ParseTypeTable.RemoveAll();
}

void cScriptManager::ReleaseImagePathTable()
{
	m_ImageHardPath.SetPositionHead();
	sIMAGHARDPATH* pImageHardPath = NULL;
	while(pImageHardPath = m_ImageHardPath.GetData())
	{		
		SAFE_DELETE(pImageHardPath);
	}	
	m_ImageHardPath.RemoveAll();

	m_ItemHardPath.SetPositionHead();
	sIMAGHARDPATH* pItemHardPath = NULL;
	while(pItemHardPath = m_ItemHardPath.GetData())
	{		
		SAFE_DELETE(pItemHardPath);
	}	
	m_ItemHardPath.RemoveAll();

	m_MugongHardPath.SetPositionHead();
	sIMAGHARDPATH* pMugongHardPath = NULL;
	while(pMugongHardPath = m_MugongHardPath.GetData())
	{		
		SAFE_DELETE(pMugongHardPath);
	}	
	m_MugongHardPath.RemoveAll();

	m_AbilityHardPath.SetPositionHead();
	sIMAGHARDPATH* pAbilityHardPath = NULL;

	while(pAbilityHardPath = m_AbilityHardPath.GetData())
	{		
		SAFE_DELETE(pAbilityHardPath);
	}	
	m_AbilityHardPath.RemoveAll();

	m_BuffHardPath.SetPositionHead();	
	sIMAGHARDPATH* pBuffHardPath = NULL;
	while(pBuffHardPath = m_BuffHardPath.GetData())
	{		
		SAFE_DELETE(pBuffHardPath);
	}	
	m_BuffHardPath.RemoveAll();

	m_MiniMapHardPath.SetPositionHead();
	sIMAGHARDPATH* pMiniMapHardPath = NULL;
	while(pMiniMapHardPath = m_MiniMapHardPath.GetData())
	{		
		SAFE_DELETE(pMiniMapHardPath);
	}	
	m_MiniMapHardPath.RemoveAll();

	m_JackPotHardPath.SetPositionHead();
	sIMAGHARDPATH* pJackPotHardPath = NULL;
	while(pJackPotHardPath = m_JackPotHardPath.GetData())
	{		
		SAFE_DELETE(pJackPotHardPath);
	}	
	m_JackPotHardPath.RemoveAll();

	m_ImageJackPath.SetPositionHead();
	sIMAGHARDPATH* pJackPathHardPath = NULL;
	while(pJackPathHardPath = m_ImageJackPath.GetData())
	{		
		SAFE_DELETE(pJackPathHardPath);
	}	
	m_ImageJackPath.RemoveAll();

	m_ImageEmojiPath.SetPositionHead();
	sEMOJIPATH* pEmojiPath = NULL;
	while(pEmojiPath = m_ImageEmojiPath.GetData())
	{		
		SAFE_DELETE(pEmojiPath);
	}	
	m_ImageEmojiPath.RemoveAll();
}

char * cScriptManager::GetMsg( int idx )
{
	return RESRCMGR->GetMsg(idx);
}
void cScriptManager::GetImage( CMHFile * fp, cImage * pImage )
{
	//ASSERT(pImage);
	char buff[255];

	int	idx = 0;
	cImageRect rect = {0,0,0,0};

	fp->GetString(buff);
	if(buff[0] != '(')		return ;
	idx						= fp->GetInt();
	rect.left				= fp->GetLong();
	rect.top				= fp->GetLong();
	rect.right				= fp->GetLong();
	rect.bottom				= fp->GetLong();
	fp->GetString(buff);
	if(buff[0] != ')')		return ;

	GetImage( idx, pImage, &rect );	
}

void cScriptManager::GetImage( int idx, cImage * pImage, cImageRect * rect )
{
	//ASSERT(pImage);

	pImage->SetSpriteObject(RESRCMGR->GetImageInfo(idx));
	IMAGE_NODE* pNode = RESRCMGR->GetInfo(idx);
	if(pNode)
		pImage->SetImageSrcSize(&pNode->size);
	pImage->SetImageSrcRect(rect);
}

void cScriptManager::InitScriptManager()
{
	m_ImageNameInfo.Initialize(400);
	m_ImageHardPath.Initialize(2000);
	m_ItemHardPath.Initialize(1200);
	m_MugongHardPath.Initialize(100);
	m_AbilityHardPath.Initialize(100);
    m_BuffHardPath.Initialize(200);
	m_MiniMapHardPath.Initialize(40);
    m_JackPotHardPath.Initialize(10);

	m_ImageJackPath.Initialize(200); //BY JACK
	m_ImageEmojiPath.Initialize(70); //BY JACK

	CMHFile file;
	sIMAGHARDPATH* pPath = NULL;
	IMAGENAMEINFO* pImageInfo = NULL;
	int index = 0;
	if( file.Init( FILE_IMAGE_HARD_PATH, "rb" ) == TRUE )
	{
		while( !file.IsEOF() )
		{
			pPath = new sIMAGHARDPATH;
			index = file.GetInt();
			pPath->idx = file.GetInt();
			pPath->left = file.GetLong();
			pPath->top = file.GetLong();
			pPath->right = file.GetLong();
			pPath->bottom = file.GetLong();

			if( m_ImageHardPath.GetData( index ) )
			{
				SAFE_DELETE(pPath);
				continue;
			}

			m_ImageHardPath.Add( pPath, index );
		}

		file.Release();
	}
	if( file.Init( FILE_IMAGE_ITEM_PATH, "rb" ) == TRUE )
	{
		while( !file.IsEOF() )
		{
			pPath = new sIMAGHARDPATH;
			index = file.GetInt();
			pPath->idx = file.GetInt();
			pPath->left = file.GetLong();
			pPath->top = file.GetLong();
			pPath->right = file.GetLong();
			pPath->bottom = file.GetLong();

			if( m_ItemHardPath.GetData( index ) )
			{
				SAFE_DELETE(pPath);
				continue;
			}

			m_ItemHardPath.Add( pPath, index );
		}

		file.Release();
	}
	if( file.Init( FILE_IMAGE_MUGONG_PATH, "rb" ) == TRUE )
	{
		while( !file.IsEOF() )
		{
			pPath = new sIMAGHARDPATH;
			index = file.GetInt();
			pPath->idx = file.GetInt();
			pPath->left = file.GetLong();
			pPath->top = file.GetLong();
			pPath->right = file.GetLong();
			pPath->bottom = file.GetLong();

			if( m_MugongHardPath.GetData( index ) )
			{
				SAFE_DELETE(pPath);
				continue;
			}

			m_MugongHardPath.Add( pPath, index );
		}

		file.Release();
	}
	if( file.Init( FILE_IMAGE_ABILITY_PATH, "rb" ) == TRUE )
	{
		while( !file.IsEOF() )
		{
			pPath = new sIMAGHARDPATH;
			index = file.GetInt();
			pPath->idx = file.GetInt();
			pPath->left = file.GetLong();
			pPath->top = file.GetLong();
			pPath->right = file.GetLong();
			pPath->bottom = file.GetLong();

			if( m_AbilityHardPath.GetData( index ) )
			{
				SAFE_DELETE(pPath);
				continue;				
			}

			m_AbilityHardPath.Add( pPath, index );
		}

		file.Release();
	}
	if( file.Init( FILE_IMAGE_BUFF_PATH, "rb" ) == TRUE )
	{		
		while( !file.IsEOF() )
		{
			pPath = new sIMAGHARDPATH;
			index = file.GetInt();
			pPath->idx = file.GetInt();
			pPath->left = file.GetLong();
			pPath->top = file.GetLong();
			pPath->right = file.GetLong();
			pPath->bottom = file.GetLong();

			if( m_BuffHardPath.GetData( index ) )
			{
				SAFE_DELETE(pPath);
				continue;
			}

			m_BuffHardPath.Add( pPath, index );
		}

		file.Release();
	}
	if( file.Init( FILE_IMAGE_MINIMAP_PATH, "rb" ) == TRUE )
	{
		while( !file.IsEOF() )
		{
			pPath = new sIMAGHARDPATH;
			index = file.GetInt();
			pPath->idx = file.GetInt();
			pPath->left = file.GetLong();
			pPath->top = file.GetLong();
			pPath->right = file.GetLong();
			pPath->bottom = file.GetLong();

			if( m_MiniMapHardPath.GetData( index ) )
			{
				SAFE_DELETE(pPath);
				continue;
			}

			m_MiniMapHardPath.Add( pPath, index );
		}

		file.Release();
	}
	if( file.Init( FILE_IMAGE_JACKPOT_PATH, "rb" ) == TRUE )
	{
		while( !file.IsEOF() )
		{
			pPath = new sIMAGHARDPATH;
			index = file.GetInt();
			pPath->idx = file.GetInt();
			pPath->left = file.GetLong();
			pPath->top = file.GetLong();
			pPath->right = file.GetLong();
			pPath->bottom = file.GetLong();

			if( m_JackPotHardPath.GetData( index ) )
			{
				SAFE_DELETE(pPath);
				continue;
			}

			m_JackPotHardPath.Add( pPath, index );
		}

		file.Release();
	}


//---------------------------------------JACK
	if(file.Init("./resource/client/TCImgList.bin","rb") == TRUE)
	{
		while(!file.IsEOF())
		{
			pImageInfo = new IMAGENAMEINFO;
			pImageInfo->idx = file.GetInt();
			pImageInfo->ImageCount = file.GetLong();
			if(pImageInfo->ImageCount>50)
			{
				pImageInfo->ImageCount=50;  
			}
			for(int i=0;i<pImageInfo->ImageCount;i++)
			{
				pImageInfo->Sticker[i] = file.GetLong();
			}
			pImageInfo->Width = file.GetInt();
			pImageInfo->Height = file.GetInt();
			pImageInfo->Speed = file.GetInt();
			if(m_ImageNameInfo.GetData(pImageInfo->idx))
			{
				SAFE_DELETE(pImageInfo);
				continue;
			}
			m_ImageNameInfo.Add(pImageInfo,pImageInfo->idx);
		}
		file.Release();
	}
	if( file.Init( "./image/image_jack_path.bin", "rb" ) == TRUE ) //BY JACK
	{
		while( !file.IsEOF() )
		{
			pPath = new sIMAGHARDPATH;
			index = file.GetInt();
			pPath->idx = file.GetInt();
			pPath->left = file.GetLong();
			pPath->top = file.GetLong();
			pPath->right = file.GetLong();
			pPath->bottom = file.GetLong();

			if( m_ImageJackPath.GetData( index ) )
			{
				SAFE_DELETE(pPath);
				continue;
			}

			m_ImageJackPath.Add( pPath, index );
		}
		file.Release();
	}
	sEMOJIPATH* pInfo = NULL;
	if( file.Init( "./image/image_emoji_icon.bin", "rb" ) == TRUE ) //BY JACK
	{
		while( !file.IsEOF() )
		{
			pInfo = new sEMOJIPATH;
			index = file.GetDword();
			//pInfo->ItemIdx=file.GetDword();
			pInfo->ImageIdx=file.GetInt();
			pInfo->ImageCount=file.GetInt();  
			if(pInfo->ImageCount>20)
			{
				pInfo->ImageCount=20;  
			}
			for(int i=0;i<pInfo->ImageCount;i++)
			{
				pInfo->rect[i].left=file.GetLong();
				pInfo->rect[i].top=file.GetLong();
				pInfo->rect[i].right=file.GetLong();
				pInfo->rect[i].bottom=file.GetLong();
			}
			if( m_ImageEmojiPath.GetData( index ) )
			{
				SAFE_DELETE(pInfo);
				continue;
			}
			m_ImageEmojiPath.Add( pInfo, index );
			//pInfo=NULL;
		}
		file.Release();
	}
}


void cScriptManager::GetImage( int hard_idx, cImage * pImage , ePATH_FILE_TYPE type )
{
	sIMAGHARDPATH* pData = NULL;
	switch( type )

	{
	case PFT_HARDPATH:	pData = m_ImageHardPath.GetData( hard_idx );	break;
	case PFT_ITEMPATH:	pData = m_ItemHardPath.GetData( hard_idx );	break;
	case PFT_MUGONGPATH:	pData = m_MugongHardPath.GetData( hard_idx );	break;
	case PFT_ABILITYPATH:	pData = m_AbilityHardPath.GetData( hard_idx );	break;
	case PFT_BUFFPATH:	pData = m_BuffHardPath.GetData( hard_idx );	break;
	case PFT_MINIMAPPATH:	pData = m_MiniMapHardPath.GetData( hard_idx );	break;
	case PFT_JACKPOTPATH:	pData = m_JackPotHardPath.GetData( hard_idx );	break;

		case PFT_JACKPATH:	pData = m_ImageJackPath.GetData(hard_idx);break;  //BY JACK
	}

	if( pData == NULL )
	{
		pImage = NULL;
		return;
	}

	cImageRect rect = {0,0,0,0};
	rect.right = pData->right;
	rect.top = pData->top;
	rect.left = pData->left;
	rect.bottom = pData->bottom;

	GetImage( pData->idx, pImage, &rect );
}
sEMOJIPATH * cScriptManager::GetEmoji(DWORD ItemIdx)
{
	return m_ImageEmojiPath.GetData(ItemIdx);
}
/*
void cScriptManager::GetImage( int hard_idx, cImage * pImage , ePATH_FILE_TYPE type )
{
	CMHFile fp;
	//--------------------------------------
#ifdef _FILE_BIN_
	char msg[255];
	switch( type )
	{
	case PFT_HARDPATH:
		fp.Init( FILE_IMAGE_HARD_PATH, "rb" );
		break;
	case PFT_ITEMPATH:
		fp.Init( FILE_IMAGE_ITEM_PATH, "rb" );
		break;
	case PFT_MUGONGPATH:
		fp.Init( FILE_IMAGE_MUGONG_PATH, "rb" );
		break;
	case PFT_ABILITYPATH:
		fp.Init( FILE_IMAGE_ABILITY_PATH, "rb");
		break;
	case PFT_BUFFPATH:
		fp.Init( FILE_IMAGE_BUFF_PATH, "rb");
		break;
	case PFT_MINIMAPPATH:
		fp.Init( FILE_IMAGE_MINIMAP_PATH, "rb");
		break;
	case PFT_JACKPOTPATH:
		fp.Init( FILE_IMAGE_JACKPOT_PATH, "rb");
		break;
	}
	for( int i = 0 ; i < hard_idx ; ++i )
	{
		fp.GetLine(msg, 255);
	}
	fp.GetInt();
//	fp.Seek(1);
#else
	char msg[255];
	switch( type )
	{
	case PFT_HARDPATH:
		fp.Init( FILE_IMAGE_HARD_PATH, "rt" );
		break;
	case PFT_ITEMPATH:
		fp.Init( FILE_IMAGE_ITEM_PATH, "rt" );
		break;
	case PFT_MUGONGPATH:
		fp.Init( FILE_IMAGE_MUGONG_PATH, "rt" );
		break;
	case PFT_ABILITYPATH:
		fp.Init( FILE_IMAGE_ABILITY_PATH, "rt");
		break;
	case PFT_BUFFPATH:
		fp.Init( FILE_IMAGE_BUFF_PATH, "rt");
		break;
	case PFT_MINIMAPPATH:
		fp.Init( FILE_IMAGE_MINIMAP_PATH, "rt");
		break;
	case PFT_JACKPOTPATH:
		fp.Init( FILE_IMAGE_JACKPOT_PATH, "rt");
		break;
	}
	for( int i = 0 ; i < hard_idx ; ++i )
	{
		fp.GetLine(msg, 255);
	}
	fp.GetInt();
	fp.Seek(1);
#endif
	//--------------------------------------
	
	int	idx = 0;
	cImageRect rect = {0,0,0,0};
	idx						= fp.GetInt();
	rect.left				= fp.GetLong();
	rect.top				= fp.GetLong();
	rect.right				= fp.GetLong();
	rect.bottom				= fp.GetLong();
	
	if( rect.right - rect.left == 0 || rect.top - rect.bottom == 0 )
		return;

	GetImage( idx, pImage, &rect );
}*/



cWindow * cScriptManager::GetDlgInfoFromFile(char * filePath, char* mode)
{
	CMHFile fp;
	if(!fp.Init(filePath, mode))
		return NULL;
	char buff[256]={0,};
	cWindow * dlg = NULL;


	while(1)
	{
		fp.GetString(buff);
		if(fp.IsEOF())
			break;
		if(buff[0] == '@')
		{
			fp.GetLineX(buff, 256);
			continue;
		}
				
		////////// 2007. 5. 18. CBH - Parse Type List °ü·Ã Ã³¸® Ãß°¡ /////////
		PARSE_TYPE eParseType = GetParseType(buff);		
		////////////////////////////////////////////////////////////////

		{			
			////////////////////////////////////////////////////////////////
			/// 2007. 5. 21. CBH - CMD_ST, CMD_CS -> swich, case ¹®À¸·Î º¯°æ   ///
			////////////////////////////////////////////////////////////////
			switch(eParseType)
			{
			case eDLG:
				{
					if((fp.GetString())[0] == '{')
						dlg = (cDialog *)GetInfoFromFile((cWindow*)(new cDialog), &fp);
					else
						__asm int 3;
					break;
				}				
			case eCHARGUAGEDLG:
				{
					if((fp.GetString())[0] == '{')
						dlg = (cDialog *)GetInfoFromFile((cWindow*)(new CGuageDialog), &fp);
					else
						__asm int 3;
					break;
				}
			case eLISTDLG:
				{
					if((fp.GetString())[0] == '{')
						dlg = (cDialog *)GetInfoFromFile((cWindow*)(new cListDialog), &fp);
					else
						__asm int 3;
					break;
				}
			case eLISTDLGEX:
				{
					if((fp.GetString())[0] == '{')
						dlg = (cDialog *)GetInfoFromFile((cWindow*)(new cListDialogEx), &fp);
					else
						__asm int 3;
					break;
				}
			case eCHARINFODLG:
				{
					if((fp.GetString())[0] == '{')
						dlg = (cDialog *)GetInfoFromFile((cWindow*)(new CCharacterDialog), &fp);
					else
						__asm int 3;
					break;
				}
			case eCHARINFOPVPDLG:
			{
				if ((fp.GetString())[0] == '{')
					dlg = (cDialog*)GetInfoFromFile((cWindow*)(new CCharacterPvpDialog), &fp);
				else
					__asm int 3;
				break;
			}
			case eMUGONGSURYUNDLG:
				{
					if((fp.GetString())[0] == '{')
						dlg = (cDialog *)GetInfoFromFile((cWindow*)(new CMugongSuryunDialog), &fp);
					else
						__asm int 3;
					break;
				}
			case eMAINDLG:
				{
					if((fp.GetString())[0] == '{')
						dlg = (cDialog *)GetInfoFromFile((cWindow*)(new CMainBarDialog), &fp);
					else
						__asm int 3;
					break;
				}
			case eINVENTORYDLG:
				{
					if((fp.GetString())[0] == '{')
						dlg = (cDialog *)GetInfoFromFile((cWindow*)(new CInventoryExDialog), &fp);
					else
						__asm int 3;
					break;
				}
			case eCHATDLG:
				{
					if((fp.GetString())[0] == '{')
						dlg = (cDialog *)GetInfoFromFile((cWindow*)(new CChatDialog), &fp);
					else
						__asm int 3;
					break;
				}				
			//SW050722 attach to minimap
			//		//SW050718
			//		CMD_CS("$JACKPOT")
			//			if((fp.GetString())[0] == '{')
			//				dlg = (cDialog *)GetInfoFromFile((cWindow*)(new cJackpotDialog), &fp);
			//			else
			//				__asm int 3;
			// LYJ 051017 ±¸ÀÔ³ëÁ¡»ó Ãß°¡
			case eSTALLOPTIONDLG:
				{
					if((fp.GetString())[0] == '{')
						dlg = (cDialog *)GetInfoFromFile((cWindow*)(new CStallKindSelectDlg), &fp);
					else
						__asm int 3;
					break;
				}
			case eBUYSTALLDLG:
				{
					if((fp.GetString())[0] == '{')
						dlg = (cDialog *)GetInfoFromFile((cWindow*)(new CStreetBuyStall), &fp);
					else
						__asm int 3;
					break;
				}
			case eBUYREGDLG:
				{
					if((fp.GetString())[0] == '{')
						dlg = (cDialog *)GetInfoFromFile((cWindow*)(new CBuyRegDialog), &fp);
					else
						__asm int 3;
					break;
				}
			case eSTREETSTALLDLG:
				{
					if((fp.GetString())[0] == '{')
						dlg = (cDialog *)GetInfoFromFile((cWindow*)(new CStreetStall), &fp);
					else
						__asm int 3;
					break;
				}
			case eNPCSCRIPTDLG:
				{
					if((fp.GetString())[0] == '{')
						dlg = (cDialog *)GetInfoFromFile((cWindow*)(new cNpcScriptDialog), &fp);
					else
						__asm int 3;
					break;
				}
			case eHELPDLG:
				{
					if((fp.GetString())[0] == '{')
						dlg = (cDialog *)GetInfoFromFile((cWindow*)(new cHelpDialog), &fp);
					else
						__asm int 3;
					break;
				}
			case eCHARMAKEDLG:
				{
					if((fp.GetString())[0] == '{')
						dlg = (cCharMakeDlg *)GetInfoFromFile((cWindow*)(new cCharMakeDlg), &fp);
					else
						__asm int 3;
					break;
				}				
			case eHELPERSPEECHDLG:
				{
					if((fp.GetString())[0] == '{')
						dlg = (cHelperSpeechDlg *)GetInfoFromFile((cWindow*)(new cHelperSpeechDlg), &fp);
					else
						__asm int 3;
					break;
				}
			case eMONEYDLG:
				{
					if((fp.GetString())[0] == '{')
						dlg = (cDialog *)GetInfoFromFile((cWindow*)(new CMoneyDlg), &fp);
					else
						__asm int 3;
					break;
				}
			case eDEALDLG:
				{
					if((fp.GetString())[0] == '{')
						dlg = (cDialog *)GetInfoFromFile((cWindow*)(new CDealDialog), &fp);
					else
						__asm int 3;	
					break;
				}
			case eQUICKDIALOG:
				{
					if((fp.GetString())[0] == '{')
						dlg = (cDialog *)GetInfoFromFile((cWindow*)(new CQuickDialog), &fp);
					else
						__asm int 3;	
					break;
				}

			case ePA_MEMBER1DLG:
				{
					if((fp.GetString())[0] == '{')
						dlg = (CPartyMemberDlg *)GetInfoFromFile((cWindow*)(new CPartyMemberDlg), &fp);
					else
						__asm int 3;
					break;
				}
			case ePA_MEMBER2DLG:
				{
					if((fp.GetString())[0] == '{')
						dlg = (CPartyMemberDlg *)GetInfoFromFile((cWindow*)(new CPartyMemberDlg), &fp);
					else
						__asm int 3;
					break;
				}
			case ePA_MEMBER3DLG:
				{
					if((fp.GetString())[0] == '{')
						dlg = (CPartyMemberDlg *)GetInfoFromFile((cWindow*)(new CPartyMemberDlg), &fp);
					else
						__asm int 3;
					break;
				}
			case ePA_MEMBER4DLG:
				{
					if((fp.GetString())[0] == '{')
						dlg = (CPartyMemberDlg *)GetInfoFromFile((cWindow*)(new CPartyMemberDlg), &fp);
					else
						__asm int 3;
					break;
				}
			case ePA_MEMBER5DLG:
				{
					if((fp.GetString())[0] == '{')
						dlg = (CPartyMemberDlg *)GetInfoFromFile((cWindow*)(new CPartyMemberDlg), &fp);
					else
						__asm int 3;
					break;
				}
			case ePA_MEMBER6DLG:
				{
					if((fp.GetString())[0] == '{')
						dlg = (CPartyMemberDlg *)GetInfoFromFile((cWindow*)(new CPartyMemberDlg), &fp);
					else
						__asm int 3;
					break;
				}

			case ePA_BTNDLG:
				{
					if((fp.GetString())[0] == '{')
						dlg = (CPartyBtnDlg *)GetInfoFromFile((cWindow*)(new CPartyBtnDlg), &fp);
					else
						__asm int 3;	
					break;
				}
			case ePA_CREATEDLG:
				{
					if((fp.GetString())[0] == '{')
						dlg = (CPartyCreateDlg *)GetInfoFromFile((cWindow*)(new CPartyCreateDlg), &fp);
					else
						__asm int 3;			
					break;
				}
			case ePA_INVITEDLG:
				{
					if((fp.GetString())[0] == '{')
						dlg = (CPartyInviteDlg *)GetInfoFromFile((cWindow*)(new CPartyInviteDlg), &fp);
					else
						__asm int 3;
					break;
				}
			case eANI:
				{
					if((fp.GetString())[0] == '{')
						dlg = (cAni *)GetInfoFromFile((cWindow*)(new cAni), &fp);
					else
						__asm int 3;	
					break;
				}

			case eMOUSEPOINTER:
				{
					if((fp.GetString())[0] == '{')
						dlg = (CMousePointer *)GetInfoFromFile((cWindow*)(new CMousePointer), &fp);
					else
						__asm int 3;
					break;
				}
			case ePYOGUKDLG:
				{
					if((fp.GetString())[0] == '{')
						dlg = (CPyogukDialog *)GetInfoFromFile((cWindow*)(new CPyogukDialog), &fp);
					else
						__asm int 3;
					break;
				}
			case eFRIENDDLG:
				{
					if((fp.GetString())[0] == '{')
						dlg = (CFriendDialog *)GetInfoFromFile((cWindow*)(new CFriendDialog), &fp);
					else
						__asm int 3;
					break;
				}
			case eNOTEDLG:
				{
					if((fp.GetString())[0] == '{')
						dlg = (CNoteDialog *)GetInfoFromFile((cWindow*)(new CNoteDialog), &fp);
					else
						__asm int 3;
					break;
				}
			case eMINNOTEDLG:
				{
					if((fp.GetString())[0] == '{')
						dlg = (CMiniNoteDialog *)GetInfoFromFile((cWindow*)(new CMiniNoteDialog), &fp);
					else
						__asm int 3;
					break;
				}
			case eMINFRIENDDLG:
				{
					if((fp.GetString())[0] == '{')
						dlg = (CMiniFriendDialog *)GetInfoFromFile((cWindow*)(new CMiniFriendDialog), &fp);
					else
						__asm int 3;	
					break;
				}
			case eREVIVEDLG:
				{
					if((fp.GetString())[0] == '{')
						dlg = (CReviveDialog*)GetInfoFromFile((cWindow*)(new CReviveDialog), &fp);
					else
						__asm int 3;			
					break;
				}
			case eCHANNELDLG:
				{
					if((fp.GetString())[0] == '{')
						dlg = (CChannelDialog*)GetInfoFromFile((cWindow*)(new CChannelDialog), &fp);
					else
						__asm int 3;			
					break;
				}
			case eWANTNPCDLG:
				{
					if((fp.GetString())[0] == '{')
						dlg = (CWantNpcDialog*)GetInfoFromFile((cWindow*)(new CWantNpcDialog), &fp);
					else
						__asm int 3;
					break;
				}
			case eWANTREGISTDLG:
				{
					if((fp.GetString())[0] == '{')
						dlg = (CWantRegistDialog*)GetInfoFromFile((cWindow*)(new CWantRegistDialog), &fp);
					else
						__asm int 3;
					break;
				}				
			/////		
			//KES
			case eEXITDLG:
				{
					if((fp.GetString())[0] == '{')
						dlg = GetInfoFromFile((cWindow*)(new CExitDialog), &fp);
					else
						__asm int 3;
					break;
				}
			case eMACRODLG:
				{
					if((fp.GetString())[0] == '{')
						dlg = GetInfoFromFile((cWindow*)(new CMacroDialog), &fp);
					else
						__asm int 3;
					break;
				}
			case eOPTIONDLG:
				{
					if((fp.GetString())[0] == '{')
						dlg = GetInfoFromFile((cWindow*)(new COptionDialog), &fp);
					else
						__asm int 3;
					break;
				}
			case eEXCHANGEDLG:
				{
					if((fp.GetString())[0] == '{')
						dlg = GetInfoFromFile((cWindow*)(new CExchangeDialog), &fp);
					else
						__asm int 3;
					break;
				}
			case eCHARSTATEDLG:
				{
					if((fp.GetString())[0] == '{')
						dlg = GetInfoFromFile((cWindow*)(new CCharStateDialog), &fp);
					else
						__asm int 3;
					break;
				}
			case eMENUSLOTDIALOG:
				{
					if((fp.GetString())[0] == '{')
						dlg = GetInfoFromFile((cWindow*)(new CMenuSlotDialog), &fp);
					else
						__asm int 3;
					break;
				}
			case ePKLOOTINGDLG:
				{
					if((fp.GetString())[0] == '{')
						dlg = GetInfoFromFile((cWindow*)(new CPKLootingDialog), &fp);
					else
						__asm int 3;
					break;
				}
			case eCNADVICEDLG:
				{
					if((fp.GetString())[0] == '{')
						dlg = GetInfoFromFile((cWindow*)(new CChinaAdviceDlg), &fp);
					else
						__asm int 3;
					break;
				}
			case eDISSOLUTIONDLG:
				{

					if((fp.GetString())[0] == '{')
						dlg = GetInfoFromFile((cWindow*)(new CDissolutionDialog), &fp);
					else
						__asm int 3;
					break;
				}					
			//////////
			//MurimNet
			case eMNPLAYROOMDLG:
				{
					if((fp.GetString())[0] == '{')
						dlg = GetInfoFromFile((cWindow*)(new CMNPlayRoomDialog), &fp);
					else
						__asm int 3;
					break;
				}
			case eMNCHANNELDLG:
				{
					if((fp.GetString())[0] == '{')
						dlg = GetInfoFromFile((cWindow*)(new CMNChannelDialog), &fp);
					else
						__asm int 3;			
					break;
				}
			///////////////////////////////////////
			case eMINIMAPDLG:
				{
					if((fp.GetString())[0] == '{')
						dlg = (cDialog *)GetInfoFromFile((cWindow*)(new CMiniMapDlg), &fp);
					else
						__asm int 3;
					break;
				}
			case eBIGMAPDLG:
				{
					if((fp.GetString())[0] == '{')
						dlg = (cDialog *)GetInfoFromFile((cWindow*)(new CBigMapDlg), &fp);
					else
						__asm int 3;
					break;
				}
			case eMONSTERGUAGEDLG:
				{
					if((fp.GetString())[0] == '{')
						dlg = (cDialog *)GetInfoFromFile((cWindow*)(new CMonsterGuageDlg), &fp);
					else
						__asm int 3;
					break;
				}
			case eDIVIDEBOX:
				{
					if((fp.GetString())[0] == '{')
						dlg = (cDialog *)GetInfoFromFile((cWindow*)(new cDivideBox), &fp);
					else
						__asm int 3;
					break;
				}
			case eDIVIDEBOXEX: // 2014-10-31 ×¢²á eDIVIDEBOXEX 
				{
					if((fp.GetString())[0] == '{')
						dlg = (cDialog *)GetInfoFromFile((cWindow*)(new cDivideBoxEx), &fp);
					else
						__asm int 3;
					break;
				}
			case eLOADINGDLG:
				{
					if((fp.GetString())[0] == '{')
						dlg = (cDialog *)GetInfoFromFile((cWindow*)(new CLoadingDlg), &fp);
					else
						__asm int 3;
					break;
				}
			case eUPGRADEDLG:
				{
					if((fp.GetString())[0] == '{')
						dlg = (cDialog *)GetInfoFromFile((cWindow*)(new CUpgradeDlg), &fp);
					else
						__asm int 3;
					break;
				}
			case eMIXDLG:
				{
					if((fp.GetString())[0] == '{')
						dlg = (cDialog *)GetInfoFromFile((cWindow*)(new CMixDialog), &fp);
					else
						__asm int 3;
					break;
				}
			case eREINFORCEDLG:
				{
					if((fp.GetString())[0] == '{')
						dlg = (cDialog *)GetInfoFromFile((cWindow*)(new CReinforceDlg), &fp);
					else
						__asm int 3;
					break;
				}
			// JSD-------------------------------------------------------------------------------------
			case eSERVERLISTDLG:
				{
					if( (fp.GetString())[0] == '{' )
						dlg = (cDialog*)GetInfoFromFile( (cWindow*)(new CServerListDialog), &fp );
					else
						__asm int 3;
					break;
				}
			case eQUESTTOTALDLG:
				{
					if((fp.GetString())[0] == '{')

						dlg = (cDialog *)GetInfoFromFile((cWindow*)(new CQuestTotalDialog), &fp);
					else
						__asm int 3;
					break;
				}
			case eMPREGISTDLG:
				{
					if((fp.GetString())[0] == '{')
						dlg = (cDialog *)GetInfoFromFile((cWindow*)(new CMPRegistDialog), &fp);
					else
						__asm int 3;
					break;
				}
			case eMPMISSIONDLG:
				{
					if((fp.GetString())[0] == '{')
						dlg = (cDialog *)GetInfoFromFile((cWindow*)(new CMPMissionDialog), &fp);
					else
						__asm int 3;
					break;
				}
			case eMPNOTICEDLG:
				{
					if((fp.GetString())[0] == '{')
						dlg = (cDialog *)GetInfoFromFile((cWindow*)(new CMPNoticeDialog), &fp);
					else
						__asm int 3;
					break;
				}
			case eMPGUAGEDLG:
				{
					if((fp.GetString())[0] == '{')
						dlg = (cDialog *)GetInfoFromFile((cWindow*)(new CMPGuageDialog), &fp);
					else
						__asm int 3;
					break;
				}
			case eBAILDLG:
				{
					if((fp.GetString())[0] == '{')
						dlg = (cDialog *)GetInfoFromFile((cWindow*)(new CBailDialog), &fp);
					else
						__asm int 3;
					break;
				}
			case eGMOPENTALKDLG:
				{
					if((fp.GetString())[0] == '{')
						dlg = (cDialog *)GetInfoFromFile((cWindow*)(new CEventNotifyDialog), &fp);
					else				
						__asm int 3;
					break;
				}
			case eVsyncDLG:
				{
					if ((fp.GetString())[0] == '{')
						dlg = (cDialog *)GetInfoFromFile((cWindow*)(new cVSyncDialog), &fp);
					else
						__asm int 3;
					break;
				}
			case eGUILDCREATEDLG:
				{
					if((fp.GetString())[0] == '{')
						dlg = (cDialog *)GetInfoFromFile((cWindow*)(new CGuildCreateDialog), &fp);
					else
						__asm int 3;	
					break;
				}
			case eGUILDUNIONCREATEDLG:
				{
					if((fp.GetString())[0] == '{')
						dlg = (cDialog*)GetInfoFromFile((cWindow*)(new CGuildUnionCreateDialog), &fp);
					else
						__asm int 3;
					break;
				}
			case eGUILDDLG:
				{
					if((fp.GetString())[0] == '{')
						dlg = (cDialog *)GetInfoFromFile((cWindow*)(new CGuildDialog), &fp);
					else
						__asm int 3;	
					break;
				}
			case eGUILDINVITEDlG:
				{
					if((fp.GetString())[0] == '{')
						dlg = (cDialog *)GetInfoFromFile((cWindow*)(new CGuildInviteDialog), &fp);
					else
						__asm int 3;
					break;
				}
			case eGUILDMARKDLG:
				{
					if((fp.GetString())[0] == '{')
						dlg = (cDialog *)GetInfoFromFile((cWindow*)(new CGuildMarkDialog), &fp);
					else
						__asm int 3;
					break;
				}
			case eGUILDLEVELUPDLG:
				{
					if((fp.GetString())[0] == '{')
						dlg = (cDialog *)GetInfoFromFile((cWindow*)(new CGuildLevelUpDialog), &fp);
					else
						__asm int 3;
					break;
				}
			case eGUILDNICKNAMEDLG:
				{
					if((fp.GetString())[0] == '{')
						dlg = (cDialog *)GetInfoFromFile((cWindow*)(new CGuildNickNameDialog), &fp);
					else
						__asm int 3;
					break;
				}
			case eGUILDRANKDLG:
				{
					if((fp.GetString())[0] == '{')
						dlg = (cDialog *)GetInfoFromFile((cWindow*)(new CGuildRankDialog), &fp);
					else
						__asm int 3;
					break;
				}
			case eGUILDWAREHOUSEDLG:
				{
					if((fp.GetString())[0] == '{')
						dlg = (cDialog *)GetInfoFromFile((cWindow*)(new CGuildWarehouseDialog), &fp);
					else
						__asm int 3;
					break;
				}
			case eITEMALLBASE:
				{
					if((fp.GetString())[0] == '{')
						dlg = (cDialog *)GetInfoFromFile((cWindow*)(new CItemShopDialog), &fp);
					else
						__asm int 3;
					break;
				}
			case eSAVEMOVEDLG:
				{
					if((fp.GetString())[0] == '{')
						dlg = (cDialog *)GetInfoFromFile((cWindow*)(new CMoveDialog), &fp);

					else
						__asm int 3;
					break;
				}
			case eCHANGSAVEDLG:
				{
					if((fp.GetString())[0] == '{')
						dlg = (cDialog *)GetInfoFromFile((cWindow*)(new CPointSaveDialog), &fp);
					else
						__asm int 3;
					break;
				}
			// guildfieldwar --------------------------------------------------------------------------------
			case eGFWARDECLAREDLG:
				{
					if((fp.GetString())[0] == '{')
						dlg = (cDialog *)GetInfoFromFile((cWindow*)(new CGFWarDeclareDlg), &fp);
					else
						__asm int 3;
					break;
				}
			case eGFWARRESULTDLG:
				{
					if((fp.GetString())[0] == '{')
						dlg = (cDialog *)GetInfoFromFile((cWindow*)(new CGFWarResultDlg), &fp);
					else
						__asm int 3;
					break;
				}
			case eGFWARINFODLG:
				{
					if((fp.GetString())[0] == '{')
						dlg = (cDialog *)GetInfoFromFile((cWindow*)(new CGFWarInfoDlg), &fp);
					else
						__asm int 3;
					break;
				}
			case eGUILDWARINFODLG:
				{
					if((fp.GetString())[0] == '{')
						dlg = (cDialog *)GetInfoFromFile((cWindow*)(new CGuildWarInfoDlg), &fp);
					else
						__asm int 3;
					break;
				}
			//------------------------------------------------------------------------------------------------
			case eSKILLPOINTAGAINDLG:
				{
					if((fp.GetString())[0] == '{')
						dlg = (cDialog *)GetInfoFromFile((cWindow*)(new CSkillPointRedist), &fp);
					else
						__asm int 3;
					break;
				}
			case eSKILLPOINTOPDLG:
				{
					if((fp.GetString())[0] == '{')
						dlg = (cDialog *)GetInfoFromFile((cWindow*)(new CSkillPointNotify), &fp);
					else
						__asm int 3;
					break;
				}
			// partywar -----------------------------
			case ePARTYWARDLG:
				{
					if((fp.GetString())[0] == '{')
						dlg = (cDialog *)GetInfoFromFile((cWindow*)(new CPartyWarDialog), &fp);
					else
						__asm int 3;
					break;
				}
			//---------------------------------------
			case eCHALLWORLDCREATEDLG:
				{
					if((fp.GetString())[0] == '{')
						dlg = (cDialog *)GetInfoFromFile((cWindow*)(new CShoutDialog), &fp);
					else
						__asm int 3;
					break;
				}
			case eCHATALLDLG:
				{
					if((fp.GetString())[0] == '{')
						dlg = (cDialog *)GetInfoFromFile((cWindow*)(new CShoutchatDialog), &fp);
					else
						__asm int 3;
					break;
				}
			case eSEECHASEDLG:
				{
					if((fp.GetString())[0] == '{')
						dlg = (cDialog *)GetInfoFromFile((cWindow*)(new CChaseDialog), &fp);
					else
						__asm int 3;
					break;
				}
			case eCHASEDLG:
				{
					if((fp.GetString())[0] == '{')
						dlg = (cDialog *)GetInfoFromFile((cWindow*)(new CChaseinputDialog), &fp);
					else
						__asm int 3;			
					break;
				}
			case eCHNAMECHANGEDLG:
				{
					if((fp.GetString())[0] == '{')
						dlg = (cDialog *)GetInfoFromFile((cWindow*)(new CNameChangeDialog), &fp);
					else
						__asm int 3;
					break;
				}
			case eOPCHNAMECHANGEDLG:
				{
					if((fp.GetString())[0] == '{')
						dlg = (cDialog *)GetInfoFromFile((cWindow*)(new CNameChangeNotifyDlg), &fp);
					else
						__asm int 3;
					break;
				}
			case eGDTENTRYDLG:
				{
					if((fp.GetString())[0] == '{')
						dlg = (cDialog *)GetInfoFromFile((cWindow*)(new CGTRegistDialog), &fp);
					else
						__asm int 3;
					break;
				}
			case eGDTENTRYCANCELDLG:
				{
					if((fp.GetString())[0] == '{')
						dlg = (cDialog *)GetInfoFromFile((cWindow*)(new CGTRegistcancelDialog), &fp);
					else
						__asm int 3;
					break;
				}
			case eGDTOURNAMENTDLG:
				{
					if((fp.GetString())[0] == '{')
						dlg = (cDialog *)GetInfoFromFile((cWindow*)(new CGTStandingDialog), &fp);
					else
						__asm int 3;
					break;
				}
			case eGDTVIEWDLG:
				{

					if((fp.GetString())[0] == '{')
						dlg = (cDialog *)GetInfoFromFile((cWindow*)(new CGTBattleListDialog), &fp);
					else
						__asm int 3;
					break;
				}
			case eEVENTMAPCOUNTDLG:
				{
					if((fp.GetString())[0] == '{')
						dlg = (cDialog *)GetInfoFromFile((cWindow*)(new CGTScoreInfoDialog), &fp);
					else
						__asm int 3;			
					break;
				}
			// seigewar
			case eSEIGEWARTIMEREGDLG:
				{
					if((fp.GetString())[0] == '{')
						dlg = (cDialog *)GetInfoFromFile((cWindow*)(new CSWTimeRegDlg), &fp);
					else
						__asm int 3;		
					break;
				}
			case eSEIGEWARPROTECTREGDLG:
				{
					if((fp.GetString())[0] == '{')
						dlg = (cDialog *)GetInfoFromFile((cWindow*)(new CSWProtectRegDlg), &fp);
					else
						__asm int 3;	
					break;
				}
			case eSEIGEWARINFODLG:
				{
					if((fp.GetString())[0] == '{')
						dlg = (cDialog *)GetInfoFromFile((cWindow*)(new CSWInfoDlg), &fp);
					else
						__asm int 3;
					break;
				}
			case eSWSTARTTIMEDLG:
				{
					if((fp.GetString())[0] == '{')
						dlg = (cDialog *)GetInfoFromFile((cWindow*)(new CSWStartTimeDialog), &fp);
					else
						__asm int 3;
					break;
				}
			// Character change
			case eCHARCHANGEEDLG:
				{
					if((fp.GetString())[0] == '{')
						dlg = (cDialog *)GetInfoFromFile((cWindow*)(new CCharChangeDlg), &fp);
					else
						__asm int 3;
					break;
				}
			// ItemSeal
			case eITEMLOCKDLG:
				{
					if((fp.GetString())[0] == '{')
						dlg = (cDialog *)GetInfoFromFile((cWindow*)(new CSealDialog), &fp);
					else
						__asm int 3;
					break;
				}
			case eSWPROFITDLG:
				{
					if((fp.GetString())[0] == '{')
						dlg = (cDialog *)GetInfoFromFile((cWindow*)(new CSWProfitDlg), &fp);
					else
						__asm int 3;
					break;
				}
			case eENGRAVEDLG:
				{
					if((fp.GetString())[0] == '{')
						dlg = (cDialog *)GetInfoFromFile((cWindow*)(new CSWEngraveDialog), &fp);
					else
						__asm int 3;
					break;
				}
			case eSWCOUNTDLG:
				{
					if((fp.GetString())[0] == '{')
						dlg = (cDialog *)GetInfoFromFile((cWindow*)(new CSWTimeDialog), &fp);
					else
						__asm int 3;
					break;
				}
			case eCHANGEJOBDLG:
				{
					if((fp.GetString())[0] == '{')
						dlg = (cDialog *)GetInfoFromFile((cWindow*)(new CChangeJobDialog), &fp);
					else
						__asm int 3;
					break;
				}
			case eRFDefaultDlg:
				{
					if((fp.GetString())[0] == '{')
						dlg = (cDialog *)GetInfoFromFile((cWindow*)(new CReinforceResetDlg), &fp);
					else
						__asm int 3;
					break;
				}
			case eRareCreateDlg:
				{
					if((fp.GetString())[0] == '{')
						dlg = (cDialog *)GetInfoFromFile((cWindow*)(new CRareCreateDialog), &fp);
					else
						__asm int 3;
					break;
				}
			case ePETSTATEDLG:
				{
					if((fp.GetString())[0] == '{')
						dlg = (cDialog*)GetInfoFromFile((cWindow*)(new CPetStateDlg), &fp);
					else
						_asm int 3;
					break;
				}
			case ePETSTATEMINIDLG:
				{
					if((fp.GetString())[0] == '{')
						dlg = (cDialog*)GetInfoFromFile((cWindow*)(new CPetStateMiniDlg), &fp);
					else
						_asm int 3;
					break;
				}
			case ePETINVENTORYDLG:
				{
					if((fp.GetString())[0] == '{')
						dlg = (cDialog*)GetInfoFromFile((cWindow*)(new CPetInventoryDlg), &fp);
					else
						_asm int 3;
					break;
				}
			case ePETUPGRADEDLG:
				{
					if((fp.GetString())[0] == '{')
						dlg = (cDialog*)GetInfoFromFile((cWindow*)(new CPetUpgradeDialog), &fp);
					else
						_asm int 3;
					break;
				}
			case ePETREVIVALDLG:
				{
					if((fp.GetString())[0] == '{')
						dlg = (cDialog*)GetInfoFromFile((cWindow*)(new CPetRevivalDialog), &fp);
					else
						_asm int 3;
					break;
				}
			case ePETMULTIDLG:
				{
					if ((fp.GetString())[0] == '{')
						dlg = (cDialog*)GetInfoFromFile((cWindow*)(new CPetMultiDlg), &fp);
					else
						_asm int 3;
					break;
				}
			case  eFPSDialog:
				{
					if ((fp.GetString())[0] == '{')
						dlg = (cDialog *)GetInfoFromFile((cWindow*)(new  CFPSDialog), &fp);
					else
						__asm int 3;
					break;
				}
			case  eJackRebornInfo:
				{
					if ((fp.GetString())[0] == '{')
						dlg = (cDialog *)GetInfoFromFile((cWindow*)(new  CJackRebornInfo), &fp);
					else
						__asm int 3;
					break;
				}




			// 06. 02. °­È­ Àç·á ¾È³» ÀÎÅÍÆäÀÌ½º Ãß°¡ - ÀÌ¿µÁØ
			case eRFDATAGUIDEDLG:
				{
					if((fp.GetString())[0] == '{')
						dlg = (cDialog *)GetInfoFromFile((cWindow*)(new CReinforceDataGuideDlg), &fp);
					else
						__asm int 3;
					break;
				}
			// 06. 02. ³»Á¤º¸Ã¢ ÆÁº¸±â Ãß°¡ - ÀÌ¿µÁØ
			case eTIPBROWSERDLG:
				{
					if((fp.GetString())[0] == '{')
						dlg = (cDialog *)GetInfoFromFile((cWindow*)(new CTipBrowserDlg), &fp);
					else
						__asm int 3;
					break;
				}
			case eKSDLG:
				{
					if((fp.GetString())[0] == '{')
						dlg = (cDialog *)GetInfoFromFile((cWindow*)(new CKeySettingTipDlg), &fp);
					else
						__asm int 3;
					break;
				}
			case eGUILDNOTEDLG:
				{
					if((fp.GetString())[0] == '{')
						dlg = (CGuildNoteDlg *)GetInfoFromFile((cWindow*)(new CGuildNoteDlg), &fp);
					else
						__asm int 3;
					break;
				}
			case eRECONNDLG:		
				{
					if((fp.GetString())[0] == '{')
						dlg = (cDialog *)GetInfoFromFile((cWindow*)(new CReconnectDialog), &fp);
					else
						__asm int 3;
					break;
				}
			case eADVANCEDDLG:    //¿ìËÙ¼Óµã  by:ºúººÈý QQ:112582793
				{
					if((fp.GetString())[0] == '{')
						dlg = (cDialog *)GetInfoFromFile((cWindow*)(new AdPointDlg), &fp);
					else
						__asm int 3;
					break;
				}
			case eRBTNDlg:	//½ÇÉ«ÓÒ¼ü²Ëµ¥	by:ºúººÈý	QQ:112582793
				{
					if((fp.GetString())[0] == '{')
						dlg = (cDialog *)GetInfoFromFile((cWindow*)(new CharacterRBTN), &fp);
					else
						__asm int 3;
					break;
				}
			case eREGDLG:		
				{
					if((fp.GetString())[0] == '{')
						dlg = (cDialog *)GetInfoFromFile((cWindow*)(new RegID), &fp);
					else
						__asm int 3;
					break;
				}
			case eRESTPWDDLG:	
				{
					if((fp.GetString())[0] == '{')
						dlg = (cDialog *)GetInfoFromFile((cWindow*)(new RestPwd), &fp);
					else
						__asm int 3;
					break;
				}
			case eINSDGINFO_DLG:
			{
								   if ((fp.GetString())[0] == '{')
									   dlg = (cDialog*)GetInfoFromFile((cWindow*)(new CInsDGInfoDialog), &fp);
								   else
									   __asm int 3;
								   break;
			}
			case eINSDG_MISSIONINFO_DLG:
			{
										   if ((fp.GetString())[0] == '{')
											   dlg = (cDialog*)GetInfoFromFile((cWindow*)(new CInsDGMissionInfoDialog), &fp);
										   else
											   __asm int 3;
										   break;
			}
			case eINSDG_ENTRANCEINFO_DLG:
			{
											if ((fp.GetString())[0] == '{')
												dlg = (cDialog*)GetInfoFromFile((cWindow*)(new CInsDGEntranceInfoDialog), &fp);
											else
												__asm int 3;
											break;
			}
			case eINSDGPARTYMAKE_DLG:
			{
										if ((fp.GetString())[0] == '{')
											dlg = (cDialog*)GetInfoFromFile((cWindow*)(new CInsDGPartyMakeDialog), &fp);
										else
											__asm int 3;
										break;
			}
			case eINSDG_RANK_DLG:
			{
									if ((fp.GetString())[0] == '{')
										dlg = (cDialog*)GetInfoFromFile((cWindow*)(new CInsDGRankDialog), &fp);
									else
										__asm int 3;
									break;
			}
			}
		}
		{
			switch(eParseType)
			{
			case eALLYNOTEDLG:
				{
					if((fp.GetString())[0] == '{')
						dlg = (CUnionNoteDlg *)GetInfoFromFile((cWindow*)(new CUnionNoteDlg), &fp);
					else
						__asm int 3;
					break;
				}
				// 06. 03. ¹®ÆÄ°øÁö - ÀÌ¿µÁØ
			case eGUILDNOTICEDLG:
				{
					if((fp.GetString())[0] == '{')
						dlg = (CGuildNoticeDlg *)GetInfoFromFile((cWindow*)(new CGuildNoticeDlg), &fp);
					else
						__asm int 3;
					break;
				}	
			case eGUILDPLUSTIMEDLG:
				{
					if((fp.GetString())[0] == '{')
						dlg = (cDialog *)GetInfoFromFile((cWindow*)(new CGuildPlusTimeDlg), &fp);
					else
						__asm int 3;
					break;
				}
				/////////////////////////////////////////////////////////////
				// 06. 06. 2Â÷ ÀüÁ÷ - ÀÌ¿µÁØ
				// ¹«°ø º¯È¯
			case eSkillTransDlg:
				{
					if((fp.GetString())[0] == '{')
						dlg = (CSkillOptionChangeDlg *)GetInfoFromFile((cWindow*)(new CSkillOptionChangeDlg), &fp);
					else
						__asm int 3;
					break;
				}
			case eTDefaultDlg:
				{
					if((fp.GetString())[0] == '{')
						dlg = (CSkillOptionClearDlg *)GetInfoFromFile((cWindow*)(new CSkillOptionClearDlg), &fp);
					else
						__asm int 3;
					break;
				}
				/////////////////////////////////////////////////////////////
			case eSCREENSHOTDLG:  //½ºÅ©¸°¼¦
				{
					if((fp.GetString())[0] == '{')
						dlg = (cDialog *)GetInfoFromFile((cWindow*)(new CScreenShotDlg), &fp);
					else
						__asm int 3;
					break;
				}
				//SW060713 ¹®ÇÏ»ý
			case eJOINOPTIONDLG:
				{
					if((fp.GetString())[0] == '{')
						dlg = (cDialog*)GetInfoFromFile((cWindow*)(new CGuildJoinDialog), &fp);
					else
						_asm int 3;
					break;
				}
				//SW061019 ¼­¹ÙÀÌ¹ú¸ðµå
			case eSURVCOUNTERDLG:
				{
					if((fp.GetString())[0] == '{')
						dlg = (cDialog*)GetInfoFromFile((cWindow*)(new CSurvivalCountDialog), &fp);
					else
						_asm int 3;
					break;
				}
			case eGUILDTRAINEEINFODLG:
				{
					if((fp.GetString())[0] == '{')
						dlg = (cDialog*)GetInfoFromFile((cWindow*)(new CGuildMunhaDialog), &fp);
					else
						_asm int 3;
					break;
				}
			case eGUILDTRAINEEDLG:
				{
					if((fp.GetString())[0] == '{')
						dlg = (cDialog*)GetInfoFromFile((cWindow*)(new CGuildTraineeDialog), &fp);
					else
						_asm int 3;
					break;
				}
				// magi82 - Titan(070112) ////////////////////////////////////////////////
			case eTITANPARTSMAKEDLG:
				{
					if((fp.GetString())[0] == '{')
						dlg = (cDialog *)GetInfoFromFile((cWindow*)(new CTitanPartsMakeDlg), &fp);
					else
						__asm int 3;
					break;
				}
				//////////////////////////////////////////////////////////////////////////

				// magi82 - Titan(070119) ////////////////////////////////////////////////
			case eTITANMIXDLG:
				{
					if((fp.GetString())[0] == '{')
						dlg = (cDialog *)GetInfoFromFile((cWindow*)(new CTitanMixDlg), &fp);
					else
						__asm int 3;
					break;
				}			
			case eTITANUPGRADEDLG:
				{
					if((fp.GetString())[0] == '{')
						dlg = (cDialog *)GetInfoFromFile((cWindow*)(new CTitanUpgradeDlg), &fp);
					else
						__asm int 3;
					break;
				}
			case eTITANBREAKDLG:
				{
					if((fp.GetString())[0] == '{')
						dlg = (cDialog *)GetInfoFromFile((cWindow*)(new CTitanBreakDlg), &fp);
					else
						__asm int 3;
					break;
				}
				//////////////////////////////////////////////////////////////////////////
				// magi82 - Titan(070212) ////////////////////////////////////////////////
			case eTITANPARTSCHANGEPREVIEWDLG:
				{
					if((fp.GetString())[0] == '{')
						dlg = (cDialog *)GetInfoFromFile((cWindow*)(new CTitanChangePreViewDlg), &fp);
					else
						__asm int 3;
					break;
				}
				//////////////////////////////////////////////////////////////////////////
				// magi82 - Titan(070222) ////////////////////////////////////////////////
			case eTITAN_INVENTORY_DLG:
				{
					if((fp.GetString())[0] == '{')
						dlg = (cDialog *)GetInfoFromFile((cWindow*)(new CTitanInventoryDlg), &fp);
					else
						__asm int 3;
					break;
				}
				//////////////////////////////////////////////////////////////////////////
				// magi82 - Titan(070305) ////////////////////////////////////////////////
			case eTITAN_GUAGE_DLG:
				{
					if((fp.GetString())[0] == '{')
						dlg = (cDialog *)GetInfoFromFile((cWindow*)(new CTitanGuageDlg), &fp);
					else
						__asm int 3;
					break;
				}
				//////////////////////////////////////////////////////////////////////////
				// magi82 - Titan(070611) Å¸ÀÌÅº ¹«°øº¯È¯ ÁÖ¼®Ã³¸®
				/*
			case eTITAN_MUGONGMIX_DLG:
				{
					if((fp.GetString())[0] == '{')
						dlg = (cDialog *)GetInfoFromFile((cWindow*)(new CTitanMugongMixDlg), &fp);
					else
						__asm int 3;
					break;
				}
				*/
				// magi82 - Titan(070320) ////////////////////////////////////////////////
			case eTITAN_USE_DLG:
				{
					if((fp.GetString())[0] == '{')
						dlg = (cDialog *)GetInfoFromFile((cWindow*)(new CTitanRegisterDlg), &fp);
					else
						__asm int 3;
					break;
				}
				//////////////////////////////////////////////////////////////////////////
				// magi82 - Titan(070321) ////////////////////////////////////////////////
			case eTITAN_BONGIN_DLG:
				{
					if((fp.GetString())[0] == '{')
						dlg = (cDialog *)GetInfoFromFile((cWindow*)(new CTitanDissolutionDlg), &fp);
					else
						__asm int 3;
					break;
				}
				//////////////////////////////////////////////////////////////////////////
				// magi82 - UniqueItem(070703) ////////////////////////////////////////////////
			case eUNIQUEITEM_CURSECANCELLATION_DLG:

				{
					if((fp.GetString())[0] == '{')
						dlg = (cDialog *)GetInfoFromFile((cWindow*)(new CUniqueItemCurseCancellationDlg), &fp);
					else
						__asm int 3;
					break;
				}
				//////////////////////////////////////////////////////////////////////////
				// magi82 - UniqueItem(070709) ////////////////////////////////////////////////
			case eUNIQUEITEM_MIX_DLG:
				{
					if((fp.GetString())[0] == '{')
						dlg = (cDialog *)GetInfoFromFile((cWindow*)(new CUniqueItemMixDlg), &fp);
					else
						__asm int 3;
					break;
				}
				//////////////////////////////////////////////////////////////////////////
				// magi82 - SOS(070724) ////////////////////////////////////////////////
			case eSOS_DLG:
				{
					if((fp.GetString())[0] == '{')
						dlg = (cDialog *)GetInfoFromFile((cWindow*)(new CSOSDlg), &fp);
					else
						__asm int 3;
					break;
				}
				//////////////////////////////////////////////////////////////////////////
				// magi82 - Intro(070802)
			case eINTROREPLAY_DLG:
				{
					if( (fp.GetString())[0] == '{' )
						dlg = (cDialog*)GetInfoFromFile( (cWindow*)(new CIntroReplayDlg), &fp );
					else
						__asm int 3;
					break;
				}
				//////////////////////////////////////////////////////////////////////////
				// 2007. 9. 11. CBH - Å¸ÀÌÅº ¼ö¸®Ã¢
			case eTITANREPAIR_DLG:
				{
					if( (fp.GetString())[0] == '{' )
						dlg = (cDialog*)GetInfoFromFile( (cWindow*)(new CTitanRepairDlg), &fp );
					else
						__asm int 3;
					break;
				}
				/////////////////////////////////////////////////////////////////////////////
				// 2007. 9. 13. CBH - Å¸ÀÌÅº ¼ÒÈ¯ °ÔÀÌÁö
			case eTITANRECALL_DLG:
				{
					if( (fp.GetString())[0] == '{' )
						dlg = (cDialog*)GetInfoFromFile( (cWindow*)(new CTitanRecallDlg), &fp );
					else
						__asm int 3;
					break;
				}
				/////////////////////////////////////////////////////////////////////////////
				// 2007. 10. 19. CBH - À¯´ÏÅ© ¾ÆÀÌÅÆ Á¶ÇÕ °ÔÀÌÁö
			case eUNIQUEITEMMIX_PROGRESSBAR_DLG:
				{
					if( (fp.GetString())[0] == '{' )
						dlg = (cDialog*)GetInfoFromFile( (cWindow*)(new CUniqueItemMixProgressBarDlg), &fp );
					else
						__asm int 3;
					break;
				}
				/////////////////////////////////////////////////////////////////////////////
				// 2007. 10. 22. CBH - Å¸ÀÌÅº Á¶ÇÕ °ÔÀÌÁö
			case eTITANMIX_PROGRESSBAR_DLG:
				{
					if( (fp.GetString())[0] == '{' )
						dlg = (cDialog*)GetInfoFromFile( (cWindow*)(new CTitanMixProgressBarDlg), &fp );
					else
						__asm int 3;
					break;
				}
				/////////////////////////////////////////////////////////////////////////////
				// 2007. 10. 23. CBH - Å¸ÀÌÅº ÆÄÃ÷ Á¶ÇÕ °ÔÀÌÁö
			case eTITANPARTS_PROGRESSBAR_DLG:
				{
					if( (fp.GetString())[0] == '{' )
						dlg = (cDialog*)GetInfoFromFile( (cWindow*)(new CTitanPartsProgressBarDlg), &fp );
					else
						__asm int 3;
					break;
				}
				/////////////////////////////////////////////////////////////////////////////
				// 2007. 12. 4. CBH - ½ºÅ² ¼±ÅÃ ´ÙÀÌ¾ó·Î±×
			case eSKIN_SELECT_DLG:
				{
					if( (fp.GetString())[0] == '{' )
						dlg = (cDialog*)GetInfoFromFile( (cWindow*)(new CSkinSelectDialog), &fp );
					else
						__asm int 3;
					break;
				}
				/////////////////////////////////////////////////////////////////////////////
				// 2008. 1. 16. CBH - ÄÚ½ºÆ¬ ½ºÅ² ¼±ÅÃ ´ÙÀÌ¾ó·Î±×
			case eCOSTUMESKIN_SELECT_DLG:
				{
					if( (fp.GetString())[0] == '{' )
						dlg = (cDialog*)GetInfoFromFile( (cWindow*)(new CCostumeSkinSelectDialog), &fp );
					else
						__asm int 3;
					break;
				}
				/////////////////////////////////////////////////////////////////////////////
				// magi82(42) - ¼¥¾ÆÀÌÅÛ Ãß°¡(¼ö·ÃÄ¡ ÃÊ±âÈ­)
			case eSKILL_POINT_RESET_DLG:
				{
					if( (fp.GetString())[0] == '{' )
						dlg = (cDialog*)GetInfoFromFile( (cWindow*)(new CSkillPointResetDlg), &fp );
					else
						__asm int 3;
					break;
				}
				// 2008. 3. 17. CBH - º¸¾È¹øÈ£ ´ÙÀÌ¾ó·Î±×
			case eNUMBERPAD_DLG:
				{
					if( (fp.GetString())[0] == '{' )
						dlg = (cDialog*)GetInfoFromFile( (cWindow*)(new CNumberPadDialog), &fp );
					else
						__asm int 3;
					break;
				}
				/////////////////////////////////////////////////////////////////////////////
				// magi82(47)
			case eSTALLVIEWDLG:
				{
					if( (fp.GetString())[0] == '{' )
						dlg = (cDialog*)GetInfoFromFile( (cWindow*)(new CStreetStallItemViewDlg), &fp );
					else
						__asm int 3;
					break;
				}
				//////////////////////////////////////////////////////////////////////////
				// ³ëÁ¡»ó °Ë»ö Ãß°¡ by Stiner(8)
			case eSTALLFINDDLG:
				{
					if( (fp.GetString())[0] == '{' )
						dlg = (cDialog*)GetInfoFromFile( (cWindow*)(new CStallFindDlg), &fp );
					else
						__asm int 3;
					break;
				}
			// autonote
			case eAUTONOTEDLG:
				{
					if( (fp.GetString())[0] == '{' )
						dlg = (cDialog*)GetInfoFromFile( (cWindow*)(new CAutoNoteDlg), &fp );
					else
						__asm int 3;
					break;
				}
			case eAUTOANSWERDLG:
				{
					if( (fp.GetString())[0] == '{' )
						dlg = (cDialog*)GetInfoFromFile( (cWindow*)(new CAutoAnswerDlg), &fp );
					else
						__asm int 3;
					break;
				}

			case ePARTYMATCHING_DLG:
				{
					if( (fp.GetString())[0] == '{' )
						dlg = (cDialog*)GetInfoFromFile( (cWindow*)(new CPartyMatchingDlg), &fp );
					else
						__asm int 3;
					break;
				}
			// ¿ä»õÀü
			case eFWENGRAVEDLG:
				{
					if( (fp.GetString())[0] == '{' )
						dlg = (cDialog*)GetInfoFromFile( (cWindow*)(new CFWEngraveDialog), &fp );
					else
						__asm int 3;
					break;
				}
			case eFWTIMEDLG:
				{
					if( (fp.GetString())[0] == '{' )
						dlg = (cDialog*)GetInfoFromFile( (cWindow*)(new CFWTimeDialog), &fp );
					else
						__asm int 3;
					break;
				}
			case eFWWAREHOUSEDLG:
				{
					if( (fp.GetString())[0] == '{' )
						dlg = (cDialog*)GetInfoFromFile( (cWindow*)(new CFWWareHouseDialog), &fp );
					else
						__asm int 3;
					break;
				}
			//case eITEM_MALLDLG:   // ÅÝµãÉÌ³Ç
			//	{
			//		if((fp.GetString())[0] == '{')
			//			dlg = (cDialog *)GetInfoFromFile((cWindow*)(new CMallShopDialog), &fp);
			//		else				
			//			__asm int 3;
			//		break;
			//	}
			case eMiniMarryDialog: // 2014-05-08 Create MimiMarry window 
				{
					if((fp.GetString())[0] == '{')
						dlg = (cDialog *)GetInfoFromFile((cWindow*)(new CMiniMaryDialog), &fp);
					else				
						__asm int 3;
					break;
				}
			case eMiniShiTuDialog: // 2014-05-08 Create MiniShiTu window 
				{
					if((fp.GetString())[0] == '{')
						dlg = (cDialog *)GetInfoFromFile((cWindow*)(new CBaiShiDialog), &fp);
					else				
						__asm int 3;
					break;
				}
			case eControlDialog: // 2014-05-08 Create bar control window 
				{
					if((fp.GetString())[0] == '{')
						dlg = (cDialog *)GetInfoFromFile((cWindow*)(new CControlDialog), &fp);
					else				
						__asm int 3;
					break;
				}
			case eRankButtonDialog: // 2014-05-08 Create bar control window 
				{
					if((fp.GetString())[0] == '{')
						dlg = (cDialog *)GetInfoFromFile((cWindow*)(new CRankButtonDialog), &fp);
					else				
						__asm int 3;
					break;
				}
			case eLoginEventDlg: // 2014-05-08 Create bar control window 
				{
					if((fp.GetString())[0] == '{')
						dlg = (cDialog *)GetInfoFromFile((cWindow*)(new CEventPlayTimeDlg), &fp);
					else				
						__asm int 3;
					break;
				}
			case eDungeonScript: // 2014-05-08 Create bar control window 
				{
					if((fp.GetString())[0] == '{')
						dlg = (cDialog *)GetInfoFromFile((cWindow*)(new CInsFUBENMissionString), &fp);
					else				
						__asm int 3;
					break;
				}
			case eInsDGEntranceInfoDlg:
				{
					if((fp.GetString())[0] == '{')
						dlg = (cDialog *)GetInfoFromFile((cWindow*)(new CInsDGEntranceInfoDlg), &fp);
					else				
						__asm int 3;
					break;
				}
			case eJACKFLASHDLG:
				{
					if((fp.GetString())[0] == '{')
						dlg = (cDialog *)GetInfoFromFile((cWindow*)(new CJackFlashMsg), &fp);
					else
						__asm int 3;
					break;
				}
			case eHIDEBUFFDLG:
				{
					if((fp.GetString())[0] == '{')
						dlg = (cDialog *)GetInfoFromFile((cWindow*)(new CHideBuff), &fp);
					else
						__asm int 3;
					break;
				}
			case eTopDialog: // 2014-05-08 Create billboard window 
				{
					if((fp.GetString())[0] == '{')
						dlg = (cDialog *)GetInfoFromFile((cWindow*)(new CTopDialog), &fp);
					else				
						__asm int 3;
					break;
				}
			case eTopDungeon: // 2014-05-08 Create billboard window 
				{
					if ((fp.GetString())[0] == '{')
						dlg = (cDialog *)GetInfoFromFile((cWindow*)(new CTopDungeon), &fp);
					else
						__asm int 3;
					break;
				}
			case eTopMunpDialog: // 2014-05-08 Create billboard window 
				{
					if((fp.GetString())[0] == '{')
						dlg = (cDialog *)GetInfoFromFile((cWindow*)(new CTopMunpDialog), &fp);
					else				
						__asm int 3;
					break;
				}
			case eLuckerDialog: // 2014-05-08 Create LuckerDialog window 
				{
					if((fp.GetString())[0] == '{')
						dlg = (cDialog *)GetInfoFromFile((cWindow*)(new CLuckerDialog), &fp);
					else				
						__asm int 3;
					break;
				}
			case eItemDropDialog: // 2014-05-08 Create LuckerDialog window 
				{
					if ((fp.GetString())[0] == '{')
						dlg = (cDialog *)GetInfoFromFile((cWindow*)(new CItemDropInfoDialog), &fp);
					else
						__asm int 3;
					break;
				}
			case eMoneyPacketDialog: // 2014-11-09 Create MoneyPacketDialog window 
				{
					if((fp.GetString())[0] == '{')
						dlg = (cDialog *)GetInfoFromFile((cWindow*)(new CMoneyPacketDialog), &fp);
					else				
						__asm int 3;
					break;
				}
			case eOtherCharacterMoneyDlg: // 2014-11-10 Create othercharactermoneydlg window 
				{
					if((fp.GetString())[0] == '{')
						dlg = (cDialog *)GetInfoFromFile((cWindow*)(new COtherCharacerMoneyDlg), &fp);
					else				
						__asm int 3;
					break;
				}
			case eOtherCharacterAmuletDlg: // 2014-11-10 Create othercharacterAmuletdlg window 
				{
					if((fp.GetString())[0] == '{')
						dlg = (cDialog *)GetInfoFromFile((cWindow*)(new COtherCharacterAmuletDlg), &fp);
					else				
						__asm int 3;
					break;
				}
			case eCodeDialog:
				{
					if((fp.GetString())[0] == '{')
						dlg = (cDialog *)GetInfoFromFile((cWindow*)(new CCodeDialog), &fp);
					else				
						__asm int 3;
					break;
				}
			case eOtherCharacterInfo: // 2014-08-11 Create Character Info window!
				{
					if((fp.GetString())[0] == '{')
						dlg = (cDialog *)GetInfoFromFile((cWindow*)(new CCharactorInfoDlg), &fp);
					else				
						__asm int 3;
					break;
				}
			case eOtherCharacterEquip: // 2014-08-11 Create Character Equip window!
				{
					if((fp.GetString())[0] == '{')
						dlg = (cDialog *)GetInfoFromFile((cWindow*)(new CQuipInfoDlg), &fp);
					else				
						__asm int 3;
					break;
				}
			case eItemGoldDlg: // 2014-08-16 Create GoldShop window!
				{
					if((fp.GetString())[0] == '{')
						dlg = (cDialog *)GetInfoFromFile((cWindow*)(new CGoldShopDialog), &fp);
					else				
						__asm int 3;
					break;
				}
			case eItemStoneDkDlg: // 2014-11-14 
				{
					if((fp.GetString())[0] == '{')
						dlg = (cDialog *)GetInfoFromFile((cWindow*)(new CItemStoneDkDialog), &fp);
					else				
						__asm int 3;
					break;
				}
			case eItemStoneXqDlg: // 2014-11-18 
				{
					if((fp.GetString())[0] == '{')
						dlg = (cDialog *)GetInfoFromFile((cWindow*)(new CItemStoneXqDialog), &fp);
					else				
						__asm int 3;
					break;
				}
			case eITEMSTEPREINFORCEDLG: // 2014-11-18 
				{
					if((fp.GetString())[0] == '{')
						dlg = (cDialog *)GetInfoFromFile((cWindow*)(new CItemStepReinforceDialog), &fp);
					else				
						__asm int 3;
					break;
				}
			case eItemLockDLG: // 2014-12-01 Create eItemLockDLG
				{
					if((fp.GetString())[0] == '{')
						dlg = (cDialog *)GetInfoFromFile((cWindow*)(new CItemLockDialog), &fp);
					else				
						__asm int 3;
					break;
				}
			case eItemUnLockDLG: // 2014-12-07 Create eItemUnLockDLG
				{
					if((fp.GetString())[0] == '{')
						dlg = (cDialog *)GetInfoFromFile((cWindow*)(new CItemUnLockDialog), &fp);
					else				
						__asm int 3;
					break;
				}
			case eItemGambleDLG: // 2015-01-16 ¶Ä²©
				{
					if((fp.GetString())[0] == '{')
						dlg = (cDialog *)GetInfoFromFile((cWindow*)(new CItemGambleDialog), &fp);
					else				
						__asm int 3;
					break;
				}
			
			case  eSafeNumberIcon:  //weiye 2017-12-10 ´´½¨°²È«ËøÍ¼±ê´°¿Ú
				{
					if((fp.GetString())[0] == '{')
						dlg = (cDialog *)GetInfoFromFile((cWindow*)(new CSafeNumberIcon), &fp);
					else				
						__asm int 3;
					break;
				}
			case  eSafeNumberImg:  //weiye 2017-12-10 ´´½¨°²È«Ëø¾²Ì¬Í¼±ê´°¿Ú
				{
					if((fp.GetString())[0] == '{')
						dlg = (cDialog *)GetInfoFromFile((cWindow*)(new CSafeNumberImg), &fp);
					else				
						__asm int 3;
					break;
				}


			/*case  eSafeNumberDialog: //weiye 2017-12-08 ´´½¨°²È«Ëø´°¿Ú
				{
					if((fp.GetString())[0] == '{')
						dlg = (cDialog *)GetInfoFromFile((cWindow*)(new CSafeNumberDialog), &fp);
					else				
						__asm int 3;
					break;
				}
			case  eSafeNumberChange:  //weiye 2018-04-08 ´´½¨°²È«ËøÐÞ¸ÄÃÜÂë´°¿Ú
				{
					if((fp.GetString())[0] == '{')
						dlg = (cDialog *)GetInfoFromFile((cWindow*)(new CSafeNumberChangeDialog), &fp);
					else				
						__asm int 3;
					break;
				}
			case  eSafeNumberLock:  //weiye 2017-12-10 ´´½¨°²È«ËøÐÞ¸ÄÃÜÂë´°¿Ú
				{
					if((fp.GetString())[0] == '{')
						dlg = (cDialog *)GetInfoFromFile((cWindow*)(new CSafeNumberLockDialog), &fp);
					else				
						__asm int 3;
					break;
				}*/
			case  eTopIcon:        //weiye 2017-10-22 ´´½¨Ð¡µØÍ¼ÏÂ·ÂÍ¼±ê´°¿Ú
				{
					if((fp.GetString())[0] == '{')
						dlg = (cDialog *)GetInfoFromFile((cWindow*)(new CTopIcon), &fp);
					else				
						__asm int 3;
					break;
				}
			case  eFBTimeDLG:    //weiye 2018-02-06 ´´½¨FBÊ±¼ä´°¿Ú
				{
					if((fp.GetString())[0] == '{')
						dlg = (cDialog *)GetInfoFromFile((cWindow*)(new CFBTimeDialog), &fp);
					else				
						__asm int 3;
					break;
				}
			case  eFameGoldDialog:  //weiye 2017-12-13 ´´½¨Ôª±¦Ï´E´°¿Ú
				{
					if((fp.GetString())[0] == '{')
						dlg = (cDialog *)GetInfoFromFile((cWindow*)(new CFameGoldDialog), &fp);
					else				
						__asm int 3;
					break;
				}
			/*case  eCharacterControlDialog:  //weiye 2018-02-12 ´´½¨½ÇÉ«¹¦ÄÜ´°¿Ú
				{
					if((fp.GetString())[0] == '{')
						dlg = (cDialog *)GetInfoFromFile((cWindow*)(new  CharacterControlDialog), &fp);
					else				
						__asm int 3;
					break;
				}*/
			case  eSendEquipDialog:  //weiye 2018-03-12 ´´½¨·¢ËÍ×°±¸´°¿Ú
				{
					if((fp.GetString())[0] == '{')
						dlg = (cDialog *)GetInfoFromFile((cWindow*)(new  CSendEquipDialog), &fp);
					else				
						__asm int 3;
					break;
				}
			case  eSendEquipSelectDialog:  //weiye 2018-03-12 ´´½¨·¢ËÍ×°±¸ÏÔÊ¾´°¿Ú
				{
					if((fp.GetString())[0] == '{')
						dlg = (cDialog *)GetInfoFromFile((cWindow*)(new  CSendEquipSelectDialog), &fp);
					else				
						__asm int 3;
					break;
				}
			case  eVideoCaptureDialog:  //weiye  2018-05-12 ÆÁÄ»Â¼Ïñ´°¿Ú¶¨Òå 
				{
					if((fp.GetString())[0] == '{')
						dlg = (cDialog *)GetInfoFromFile((cWindow*)(new  CVideoCaptureDlg), &fp);
					else				
						__asm int 3;
					break;
				}
			case  eVideoCaptureIcon:
				{
					if((fp.GetString())[0] == '{')
						dlg = (cDialog *)GetInfoFromFile((cWindow*)(new  CVideoCaptureIcon), &fp);
					else				
						__asm int 3;
					break;
				}
			case  eLuckyDrawIcon:
				{
					if((fp.GetString())[0] == '{')
						dlg = (cDialog *)GetInfoFromFile((cWindow*)(new  CLuckyDrawIcon), &fp);
					else				
						__asm int 3;
					break;
				}
			case  eBloodStormIcon:
				{
					if((fp.GetString())[0] == '{')
						dlg = (cDialog *)GetInfoFromFile((cWindow*)(new  CBloodStormIcon), &fp);
					else				
						__asm int 3;
					break;
				}
			case  eChangeLookDlg:
				{
					if ((fp.GetString())[0] == '{')
						dlg = (cDialog *)GetInfoFromFile((cWindow*)(new  cChangeLookDialog), &fp);
					else
						__asm int 3;
					break;
				}
			case eGUILDWARDLG:
				{
					if((fp.GetString())[0]=='{')
						dlg = (cDialog *)GetInfoFromFile((cWindow*)(new CGuildWarDialog), &fp);
					else
						__asm int 3;
					break;
				}			
			case eGoldShopBuyDialog:
				{
					if((fp.GetString())[0] == '{')
						dlg = (cDialog *)GetInfoFromFile((cWindow*)(new CGoldShopBuyDialog), &fp);
					else				
						__asm int 3;
					break;
				}
//------------------------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------put new code below-----------------------------
			case eHousingWebDlg:
				{
					if((fp.GetString())[0] == '{')
						dlg = (cDialog *)GetInfoFromFile((cWindow*)(new cHousingWebDlg), &fp);
					else				
						__asm int 3;
					break;
				}
			case eFADEDLG:
			{
				if ((fp.GetString())[0] == '{')
					dlg = GetInfoFromFile(new CFadeDlg, &fp);
				else
					__asm int 3;
				break;
				}
			case eOfficialUpGradeDlg:	//Daily ?????????????
			{
				if ((fp.GetString())[0] == '{')
					dlg = (cDialog*)GetInfoFromFile((cWindow*)(new COfficialUpGradeDlg), &fp);
				else
					__asm int 3;
				break;
			}








			default:
				{
					break;
				}
			}
		}
	}
			
	fp.Release();
	
	return dlg;
}
cWindow * cScriptManager::GetCtrlInfo(cWindow * wnd, char * buff, CMHFile * fp)
{	
	
	////////// 2007. 5. 18. CBH - Parse Type List °ü·Ã Ã³¸® Ãß°¡ /////////	
	PARSE_TYPE eParseType = GetParseType(buff);

	//LONG lID;
	//if(eParseType == -1)
	//{
	//	lID = wnd->GetID();
	//}
	////////////////////////////////////////////////////////////////
	
	switch(eParseType)
	{
	case eBTN:
		{
			cWindow * window = GetInfoFromFile((cWindow*)(new cButton), fp);
			//ASSERT(wnd);
			wnd->Add(window);
			break;
		}	
	case eEDITBOX:
		{
			cWindow * window = GetInfoFromFile((cWindow*)(new cEditBox), fp);
			//ASSERT(wnd);
			wnd->Add(window);
			//((cDialog*)wnd)->Add(window);
			break;
		}
	case eSTATIC:
		{
			cWindow * window = GetInfoFromFile((cWindow*)(new cStatic), fp);
			//ASSERT(wnd);
			wnd->Add(window);
			//((cDialog*)wnd)->Add(window);
			break;
		}
	case ePUSHUPBTN:
		{
			cWindow * window = GetInfoFromFile((cWindow*)(new cPushupButton), fp);
			//ASSERT(wnd);
			wnd->Add(window);
			//((cDialog*)wnd)->Add(window);
			break;
		}
	case eCHECKBOX:
		{
			cWindow * window = GetInfoFromFile((cWindow*)(new cCheckBox), fp);
			//ASSERT(wnd);
			wnd->Add(window);
			break;
		}
	case eCOMBOBOX:
		{
			cWindow * window = GetInfoFromFile((cWindow*)(new cComboBox), fp);
			//ASSERT(wnd);
			wnd->Add(window);
			//((cDialog*)wnd)->Add(window);
			break;
		}
	case eCOMBOBOXEX:
		{
			cWindow * window = GetInfoFromFile((cWindow*)(new cComboBoxEx), fp);
			//ASSERT(wnd);
			wnd->Add(window);
			break;
		}
	case eSPIN:
		{
			cWindow * window = GetInfoFromFile((cWindow*)(new cSpin), fp);
			//ASSERT(wnd);
			wnd->Add(window);

			break;
		}
	case eLISTCTRL:
		{
			cWindow * window = GetInfoFromFile((cWindow*)(new cListCtrl), fp);
			//ASSERT(wnd);
			wnd->Add(window);
			break;
		}
	case eLISTCTRLEX:
		{
			cWindow * window = GetInfoFromFile((cWindow*)(new cListCtrlEx), fp);
			//ASSERT(wnd);
			wnd->Add(window);
			//((cDialog*)wnd)->Add(window);
			break;
		}
	case eICONDLG:
		{
			cWindow * window = GetInfoFromFile((cWindow*)(new cIconDialog), fp);
			//ASSERT(wnd);
			wnd->Add(window);
			//((cDialog*)wnd)->Add(window);
			break;
		}
	case eICONGRIDDLG:
		{
			cWindow * window = GetInfoFromFile((cWindow*)(new cIconGridDialog), fp);
			//ASSERT(wnd);
			wnd->Add(window);
			//((cDialog*)wnd)->Add(window);
			break;
		}
	case eLISTDLGEX:
		{
			cWindow * window = GetInfoFromFile((cWindow*)(new cListDialogEx), fp);
			//ASSERT(wnd);
			wnd->Add(window);
			//((cDialog*)wnd)->Add(window);
			break;
		}
	case eLISTDLG:
		{
			cWindow * window = GetInfoFromFile((cWindow*)(new cListDialog), fp);
			//ASSERT(wnd);
			wnd->Add(window);
			//((cDialog*)wnd)->Add(window);
			break;
		}
	case eWEAREDDLG:
		{
			cWindow * window = GetInfoFromFile((cWindow*)(new CWearedExDialog), fp);
			//ASSERT(wnd);
			wnd->Add(window);
			break;
		}
	case eLIST:
		{
			cWindow * window = GetInfoFromFile((cWindow*)(new cList), fp);
			//ASSERT(wnd);
			wnd->Add(window);
			//((cDialog*)wnd)->Add(window);
			break;
		}
	case eGUAGEBAR:
		{
			cWindow * window = GetInfoFromFile((cWindow*)(new cGuageBar), fp);
			//ASSERT(wnd);
			wnd->Add(window);
			break;
		}
	case eANI:
		{
			cWindow * window = GetInfoFromFile((cWindow*)(new cAni), fp);
			//ASSERT(wnd);
			wnd->Add(window);
		}
	case eGUAGE:
		{
			cWindow * window = GetInfoFromFile((cWindow*)(new cGuage), fp);
			//ASSERT(wnd);
			wnd->Add(window);
			break;
		}
	case eTEXTAREA:
		{
			cWindow * window = GetInfoFromFile((cWindow*)(new cTextArea), fp);
			//ASSERT(wnd);
			wnd->Add(window);
			break;
		}
	case eGUAGEN:
		{
			cWindow * window = GetInfoFromFile((cWindow*)(new cGuagen), fp);
			//ASSERT(wnd);
			wnd->Add(window);
			break;
		}
	case eGUAGENE:
		{
			cWindow * window = GetInfoFromFile((cWindow*)(new CObjectGuagen), fp);
 			//ASSERT(wnd);
			wnd->Add(window);
			break;
		}
	//KES 030821 Ãß°¡
	case eDLG:
		{
			cWindow * window = GetInfoFromFile((cWindow*)(new cDialog), fp);
			//ASSERT(wnd);
			wnd->Add(window);
			break;
		}
	//SW051112 ¹«½Ö¸ðµå
	case eCHARGUAGEDLG:
		{
			cWindow * window = GetInfoFromFile((cWindow*)(new CGuageDialog), fp);
			//ASSERT(wnd);
			wnd->Add(window);
			break;
		}
	//HSÃß°¡
	case eMUNPAMARKDLG:
		{
			cWindow * window = GetInfoFromFile((cWindow*)(new CMunpaMarkDialog), fp);
			//ASSERT(wnd);
			wnd->Add(window);
			break;
		}
	case ePRIVATEWAREHOUSEDLG:
		{
			cWindow * window = GetInfoFromFile((cWindow*)(new CPrivateWarehouseDialog), fp);
			//ASSERT(wnd);
			wnd->Add(window);
			break;
		}
	case eMUGONGDLG:
		{
			cWindow * window = GetInfoFromFile((cWindow*)(new CMugongDialog), fp);
			//ASSERT(wnd);
			wnd->Add(window);
			break;
		}
	case eSURYUNDLG:
		{
			cWindow * window = GetInfoFromFile((cWindow*)(new CSuryunDialog), fp);
			//ASSERT(wnd);
			wnd->Add(window);
			break;
		}
	case eQUESTDLG:
		{
			cWindow * window = GetInfoFromFile((cWindow*)(new CQuestDialog), fp);
			//ASSERT(wnd);
			wnd->Add(window);
			break;
		}
	case eWANTEDDLG:
		{
			cWindow * window = GetInfoFromFile((cWindow*)(new CWantedDialog), fp);
			//ASSERT(wnd);
			wnd->Add(window);
			break;
		}
	case eJOURNALDLG:
		{
			cWindow * window = GetInfoFromFile((cWindow*)(new CJournalDialog), fp);
			//ASSERT(wnd);
			wnd->Add(window);
			break;
		}
	case eITEMSHOPGRIDDLG:
		{
			cWindow * window = GetInfoFromFile((cWindow*)(new CItemShopGridDialog), fp);
			//ASSERT(wnd);
			wnd->Add(window); 
			break;
		}
	case eSHOPITEMINVENGRID:
		{
			cWindow * window = GetInfoFromFile((cWindow*)(new CItemShopInven), fp);
			//ASSERT(wnd);
			wnd->Add(window);
			break;
		}
	case eShopItemInventPlusDlg:
		{//[ShopInventoryPlus][By:Ê®ÀïÆÂ½£Éñ][QQ:112582793][2018/10/9]
			cWindow * window = GetInfoFromFile((cWindow*)(new cShopItemInventPlusDlg), fp);
			//ASSERT(wnd);
			wnd->Add(window);
			break;
		}
	case eINSDG_RANK_INFO_DLG:
		{
			cWindow* window = GetInfoFromFile((cWindow*)(new CInsDGRankInfoDialog), fp);
			//ASSERT(wnd);
			wnd->Add(window);
			break;
		}
	case eBROWSER:	//$BROWSER
		{
			cWindow* window = GetInfoFromFile((cWindow*)(new cBrowser), fp) ;
			wnd->Add(window) ;
			break;
		}
	case eHOUSINGWEBDLG:	//$HOUSINGWEBDLG
		{
			cWindow* window = GetInfoFromFile((cWindow*)(new cHousingWebDlg), fp) ;
			wnd->Add(window) ;
			break;
		}
			
	default:
		{	
			break;
		}
	}	
	return wnd;
}

//--------------------------------------------------------------------------------------------------------------------------------------
//Marquee string
//--------------------------------------------------------------------------------------------------------------------------------------
/*std::string marquee_limit(std::string& text, size_t limit)
{
	std::string temp = text;
	text.erase(0, 1);
	text += temp[0];
	return text.substr(0, limit);
}*/
cWindow * cScriptManager::GetInfoFromFile(cWindow * wnd, CMHFile * fp)
{
	cPoint point={0,0,0,0};

	VECTOR2 point1 = { 0, 0 };
	VECTOR2 point2 = { 0, 0 };
	VECTOR2 point3 = { 0, 0 };

	cPoint listBarPoint={0,0,0,0};
	char toolTipMsg[256]={0,};
	char buff[256]={0,};
	char funcName[256]={0,};
	char IDName[256]={0,};
	char staticText[256]={0,};
	char imagePath[256]={0,};

	RGBCOLOR color={0,0,0};
	BOOL fMovable = TRUE;
	BOOL fActive = TRUE;
	BOOL fPush = FALSE;
	cImage BasicImage;
	cImage OverImage;
	cImage ListOverImage;
	cImage PressImage	;
	cImage ToolTipImage	;
	cImage TopImage		;
	cImage MiddleImage	;
	cImage DownImage	;
	cImage HeadImage	;
	cImage BodyImage	;
	cImage ListBarImage	;
	cImage FocusImage	;
	cImage SelectedImage;

	cImage AniSpriteImage	;
	cImage GuagePieceImage	;
	cImage CheckImage		;
	cImage CheckBoxImage	;
	cImage selectedBGImage	;
	cImage dragoverBGImage	;
	cImage guageEffectPieceImage	;

	cImage BalloonImage[10];
	FLOAT  BalloonBorder = 8;

	int	   toolTipLineSize	= 0;

	BOOL	bChecked = FALSE;
	WORD	HeadHei = 0;
	WORD	BodyHei = 0;
	int		nLeftMargin = 0;
	int		nTopMargin = 0;

	WORD EditBoxSize = 0;
	WORD EditBoxEableTypingSize = 0;
	BOOL fSecret = FALSE;
	RGBCOLOR activeTextColor={0,0,0};
	RGBCOLOR nonactiveTextColor={0,0,0};
	RGBCOLOR fgColor={0,0,0};
	RGBCOLOR comboTextColor={0,0,0};
	char szBtnText[256]={0,};
	RGBCOLOR btnTextBasicCol={0,0,0};
	RGBCOLOR btnTextOverCol={0,0,0};
	RGBCOLOR btnTextPressCol={0,0,0};
	BYTE alphaDepth = 255;
	cCoord coord;
	RECT captionRect={0,0,0,0};
	RECT textRect={0,0,0,0};
	RECT imgSrcRect={0,0,0,0};
	BYTE cols=0;
	BYTE rows=0;
	BYTE tabNum=0;
	BYTE middleNum=0;
	WORD topHei = 0;
	WORD middleHei = 0;
	WORD downHei = 0;
	LONG minValue = 0;
	LONG maxValue = 100;
	LONG spinUnit = 10;
	BOOL lineUpward = TRUE;
	int	 interval = 0;
	WORD maxLine = 0;
	LONG listMaxHeight = 15;
	BYTE minMiddleNum=0;
	BYTE maxMiddleNum=0;
	WORD gridBorderX = 0;
	WORD gridBorderY = 0;
	cPoint gridPos ={0,0,0,0};
	cCoord validXY={0,0};
	BOOL vertical = FALSE;
	BOOL fSpriteloop = TRUE;
	WORD travelingType = 0;

	float guageMinVal=0;
	float guageMaxVal=0;
	float guageCurVal=0;
	cCoord guagePos = {0,0};
	WORD selectComboIdx = 0;

	BOOL bAutoClose = FALSE;
	cCoord textXY = {5,6};
	WORD	fontIdx = 0;
	BOOL bReadOnly = FALSE;
	LONG limitLineNum = 20;
	int	soundIdx = -1;
	WORD	wOption = 0;
//KES 030916
	int	nLimitBytes = 256;
	int nTextAlign = TXT_LEFT | TXT_TOP;
	BYTE btnTextAniX = 1;
	BYTE btnTextAniY = 1;

	BOOL bAutoScroll = TRUE;
	BOOL bPassive = FALSE;

	int tt_idx = -1;
//
	cImageScale scale={1,1};
	cImageScale ListScale={1,1};

	float guageWidth = 0.f;
	float guagePieceWidth = 0.f;
	float guageEffectPieceWidth = 0.f;
	float guagePieceHeightScaleY = 1.f;
	float guageEffectPieceHeightScaleY = 1.f;
	DWORD	guageEstTime = 0;

	LONG		shadowTextX=0;
	LONG		shadowTextY=0;
	RGBCOLOR	dwShadowColor={10,10,10};
	BOOL		bShadow = FALSE;

	//cCoord OffSet = {0,0};

	DWORD m_OffSetX=0;
	DWORD m_OffSetY=0;

	bool bDisability=false;

	bool bEnableMarquee = false;

	DWORD MarqueeSize = 0;
	DWORD MarqueeSpeed = 0;

	BOOL nFontOutline = FALSE;

	BOOL nDoubleClick = FALSE;

	BYTE nWindowBase = 5;

	while(1)
	{
		fp->GetString(buff);
		if(buff[0] == '}'|| fp->IsEOF())
			break;
		if(buff[0] == '{')
			continue;
		if(buff[0] == '$')	
		{
			GetCtrlInfo(wnd, buff, fp);
			continue;
		}
		if(buff[0] == '@')
		{
			fp->GetLineX(buff, 256);
			continue;
		}
		
		////////// 2007. 5. 18. Parse Type List °ü·Ã Ã³¸® Ãß°¡ /////////		
		PARSE_TYPE eParseType = GetParseType(buff);

		//LONG lID;
		//if(eParseType == -1)
		//{
		//	lID = wnd->GetID();
		//}
		////////////////////////////////////////////////////////////////
		//wnd->SaveWindowBase(nWindowBase);
		switch(eParseType)
		{
		case ePOINT:
			{
				point.x = fp->GetDword();
				point.y = fp->GetDword();
				point.w = fp->GetDword();
				point.h = fp->GetDword();

				wnd->cWindow::SetAbsXY(point.x, point.y);
				wnd->cWindow::SetRelXY(point.x, point.y);
				//wnd->cWindow::SetAbsXY(point.x, point.y);
				
				

				break;
			}
		//SW061111 ÀúÇØ»óµµ¿É¼Ç
		case ePOINT_:
			{
					//------------------------------------------------------------------
					//use below
					/*point1.x = fp->GetFloat();
					point1.y = fp->GetFloat();

					point2.x = fp->GetFloat();
					point2.y = fp->GetFloat();

					point3.x = fp->GetFloat();
					point3.y = fp->GetFloat();
					wnd->SaveDispPoint(point1, point2, point3);*/
					//------------------------------------------------------------------

			        /*
					point.x = fp->GetDword();
					point.y = fp->GetDword();
					point.w = fp->GetDword();
					point.h = fp->GetDword();
					//CheckPosForResolution(&point);

					wnd->cWindow::SetAbsXY(point.x, point.y);
					wnd->cWindow::SetRelXY(point.x, point.y);
				}
					*/
				break;
			}
		case eFUNC:
			{
				fp->GetString(funcName);
				break;
			}
		case eID:
			{
				fp->GetString(IDName);
				break;
			}
		case eFONTIDX:
			{
				fontIdx = fp->GetWord();
				break;
			}
		case eAUTOCLOSE:
			{
				bAutoClose = fp->GetBool();
				break;
			}
		case eTOOLTIPIMAGE:
			{
				GetImage(fp, &ToolTipImage);
				break;
			}
		case eTOOLTIPMSG:
			{
				strcpy( toolTipMsg, GetMsg(fp->GetInt()));
				break;
			}
		case eTOOLTIPCOL:
			{
				color.r = fp->GetByte();
				color.g = fp->GetByte();
				color.b = fp->GetByte();
				break;
			}
		case eTEXTXY:
			{
				textXY.x = fp->GetDword();
				textXY.y = fp->GetDword();
				break;
			}
		case eMOVEABLE:
			{
				fMovable = fp->GetBool();
				break;
			}
		case eACTIVE:
			{
				fActive = fp->GetBool();
				break;
			}
		case eBASICIMAGE:
			{
				GetImage(fp, &BasicImage);
				break;
			}
		//SW061111 ÀúÇØ»óµµ¿É¼Ç
		case eBASICIMAGE_:
			{
				// ´Ë´¦½øÐÐ 2015-12-07 
				//GetImage(fp, &BasicImage);
				break;
			}
		case eOVERIMAGE:
			{
				GetImage(fp, &OverImage);
				break;
			}
		case eLISTOVERIMAGE:
			{
				GetImage(fp, &ListOverImage);
				break;
			}
		case eSELECTIMAGE:
			{
				GetImage(fp, &SelectedImage);
				break;
			}
		case ePRESSIMAGE:
			{
				GetImage(fp, &PressImage);
				break;
			}
		case eFOCUSIMAGE:
			{
				GetImage(fp, &FocusImage);
				break;
			}
		case eBTNTEXT:
			{
				strcpy( szBtnText, GetMsg(fp->GetInt()) );
				btnTextBasicCol.r = fp->GetByte();
				btnTextBasicCol.g = fp->GetByte();
				btnTextBasicCol.b = fp->GetByte();

				btnTextOverCol.r = fp->GetByte();
				btnTextOverCol.g = fp->GetByte();
				btnTextOverCol.b = fp->GetByte();

				btnTextPressCol.r = fp->GetByte();
				btnTextPressCol.g = fp->GetByte();
				btnTextPressCol.b = fp->GetByte();
				break;
			}
		case eBTNTEXTANI:
			{
				btnTextAniX	= fp->GetByte();
				btnTextAniY	= fp->GetByte();
				break;
			}
		case eIMAGESRCRECT:
			{
				imgSrcRect.left =  fp->GetLong();
				imgSrcRect.top = fp->GetLong();
				imgSrcRect.right = fp->GetLong();
				imgSrcRect.bottom = fp->GetLong();
				break;
			}
		case eALPHA:
			{
				alphaDepth = fp->GetByte();
				break;
			}
		case eEDITSIZE:
			{
				EditBoxSize = fp->GetWord();
				EditBoxEableTypingSize = fp->GetWord();
				break;
			}
		case eSPINSIZE:
			{
				EditBoxSize = fp->GetWord();
				EditBoxEableTypingSize = fp->GetWord();
				break;
			}
		case eSECRET:
			{
				fSecret = fp->GetBool();
				break;
			}
		case eTEXTCOLOR:	
			{
				activeTextColor.r = fp->GetByte();
				activeTextColor.g = fp->GetByte();
				activeTextColor.b = fp->GetByte();
				nonactiveTextColor.r = fp->GetByte();
				nonactiveTextColor.g = fp->GetByte();
				nonactiveTextColor.b = fp->GetByte();
				break;
			}
		case eCOORD:
			{
				coord.x = fp->GetDword();
				coord.y = fp->GetDword();
				break;
			}
		case eFGCOLOR:
			{
				fgColor.r = fp->GetByte();
				fgColor.g = fp->GetByte();
				fgColor.b = fp->GetByte();
				break;
			}
		case eTEXT:
			{
				int msg_idx = fp->GetInt();
				strcpy( staticText, GetMsg(msg_idx) );
				break;
			}
		case eITEMTOOLTIP:
			{
				tt_idx = fp->GetInt();
				break;
			}
		case eSHADOW:
			{
				bShadow = fp->GetBool();
				break;
			}
		case eSHADOWCOLOR:
			{
				dwShadowColor.r = fp->GetByte();
				dwShadowColor.g = fp->GetByte();
				dwShadowColor.b = fp->GetByte();
				break;
			}
		case eSHADOWTEXTXY:
			{
				shadowTextX = fp->GetLong();
				shadowTextY = fp->GetLong();
				break;
			}
		case eMESSAGE:
			{
				int msg_idx =  fp->GetInt();
				strcpy( staticText, GetMsg(msg_idx) );
				fgColor.r = fp->GetByte();
				fgColor.g = fp->GetByte();
				fgColor.b = fp->GetByte();
				break;
			}
		case eTEXTRECT:
			{
				textRect.left =  fp->GetLong();
				textRect.top = fp->GetLong();
				textRect.right = fp->GetLong();
				textRect.bottom = fp->GetLong();
				break;
			}
		case ePUSHUP:
			{
				fPush = fp->GetBool();
				break;
			}
		case eVALIDXY:
			{
				validXY.x = fp->GetDword();
				validXY.y = fp->GetDword();
				break;
			}
		case eCAPTIONRECT:
			{
				captionRect.left = fp->GetLong();
				captionRect.top = fp->GetLong();
				captionRect.right = fp->GetLong();
				captionRect.bottom = fp->GetLong();
				break;
			}
		case eICONCELLNUM:
			{
				cIconDialog * win = (cIconDialog *)wnd;
				win->SetCellNum(fp->GetByte());				
				break;
			}
		case eICONCELL:
			{
				RECT iconCellRect={0,0,0,0};
				iconCellRect.left = fp->GetLong();
				iconCellRect.top = fp->GetLong();
				iconCellRect.right = fp->GetLong();
				iconCellRect.bottom = fp->GetLong();
				cIconDialog * win = (cIconDialog *)wnd;
				win->AddIconCell(&iconCellRect);
				break;
			}
		case eSELECTOPTION:
			{
				wOption = fp->GetWord();
				break;
			}
		case eCELLRECT:
			{
				RECT CellRect={0,0,0,0};
				CellRect.left = fp->GetLong();
				CellRect.top = fp->GetLong();
				CellRect.right = fp->GetLong();
				CellRect.bottom = fp->GetLong();
				cIconGridDialog * win = (cIconGridDialog *)wnd;
				win->SetCellRect(&CellRect);
				break;
			}
		case eLIMITLINENUM:
			{
				limitLineNum = fp->GetLong();
				break;
			}
		//KES 030916
		case eLIMITBYTES:
			{
				nLimitBytes = fp->GetInt();
				break;
			}
		case eTEXTALIGN:
			{
				nTextAlign = fp->GetInt();
				if( nTextAlign == 0 )	nTextAlign = TXT_LEFT;
				else if( nTextAlign == 1 )	nTextAlign = TXT_RIGHT;
				else if( nTextAlign == 2 )	nTextAlign = TXT_CENTER;
				break;
			}
		case eAUTOSCROLL:
			{
				bAutoScroll = fp->GetByte();
				break;
			}
		case ePASSIVE:
			{
				bPassive = fp->GetByte();
				break;
			}
//
		case eTABNUM:
			{
				cTabDialog * win = (cTabDialog *)wnd;
				tabNum = fp->GetByte();
				win->InitTab(tabNum);
				break;
			}
		case eICONCELLBGIMAGE:
			{
				GetImage(fp, &selectedBGImage);
				break;
			}
		case eDRAGOVERBGIMAGE:
			{
				GetImage(fp, &dragoverBGImage);
				break;
			}
		case eCOLS:
			{
				cols = fp->GetByte();
				break;
			}
		case eROWS:
			{
				rows = fp->GetByte();
				break;
			}
		case eMIDDLENUM:
			{
				middleNum = fp->GetByte();
				break;
			}
		case eTOPIMAGE:
			{
				GetImage(fp, &TopImage);
				topHei = fp->GetWord();
				break;
			}
		case eTOPHEIGHT:
			{
				topHei = fp->GetWord();
				break;
			}
		case eDOWNHEIGHT:
			{
				downHei = fp->GetWord();
				break;
			}
		case eMIDDELIMAGE:
			{
				GetImage(fp, &MiddleImage);
				middleHei = fp->GetWord();
				break;
			}
		case eMIDDLEHEIGHT:
			{
				middleHei = fp->GetWord();
				break;
			}
		case eDOWNIMAGE:
			{
				GetImage(fp, &DownImage);
				downHei = fp->GetWord();
				break;
			}
		case eCOMBOTEXTCOLOR:
			{
				comboTextColor.r = fp->GetByte();
				comboTextColor.g = fp->GetByte();
				comboTextColor.b = fp->GetByte();
				break;
			}
		case eSELECTCOMBOINDEX:
			{
				selectComboIdx = fp->GetWord();
				break;
			}
		case eREADONLY:
			{
				bReadOnly = fp->GetBool();
				break;
			}
		case eADDSTRING:
			{
				int msg_idx =  fp->GetInt();
				char msg2[255]={0,};
				strcpy( msg2, GetMsg(msg_idx) );
				RGBCOLOR comboListColor={0,0,0};
				comboListColor.r = fp->GetByte();
				comboListColor.g = fp->GetByte();
				comboListColor.b = fp->GetByte();

				switch(wnd->GetType())
				{
				case WT_LIST:
					{
						cList * win = (cList *)wnd;
						ITEM* pItem = new ITEM;
						strcpy(pItem->string, msg2);
						pItem->rgb =  RGB_HALF(comboListColor.r, comboListColor.g, comboListColor.b);
						win->AddItem(pItem);
					}
					break;
				case WT_LISTDIALOG:
					{
						cListDialog * win = (cListDialog *)wnd;
						win->AddItem(msg2, RGB_HALF(comboListColor.r, comboListColor.g, comboListColor.b));
					}
					break;
				case WT_COMBOBOX:
					{
						cComboBox * win = (cComboBox *)wnd;
						ITEM* pItem = new ITEM;
						strcpy(pItem->string, msg2);
						pItem->rgb =  RGB_HALF(comboListColor.r, comboListColor.g, comboListColor.b);
						win->AddItem(pItem);
					}
					break;
				}
				break;
			}
		case eINITCOMBOLIST:
			{
				cComboBox * win = (cComboBox *)wnd;
				WORD listWidth = fp->GetWord();
				win->SetMaxLine(fp->GetWord());
				GetImage(fp, &TopImage);
				topHei = fp->GetWord();
				GetImage(fp, &MiddleImage);
				middleHei = fp->GetWord();
				GetImage(fp, &DownImage);
				downHei = fp->GetWord();
				GetImage(fp, &ListOverImage);
				win->InitComboList(listWidth, &TopImage, topHei, &MiddleImage, middleHei, &DownImage, downHei, &ListOverImage);
				break;
			}
		// LISTCTRL¿¡¼­ »ç¿ëÇÏ´Â ¼º
		case eCREATE:
			{
				cListCtrl * win = (cListCtrl *)wnd;
				win->Create(fp->GetInt());
				break;
			}
		case eHEADIMAGE:
			{
				GetImage(fp, &HeadImage);
				break;
			}
		case eBODYIMAGE:
			{
				GetImage(fp, &BodyImage);
				break;
			}
		case eHEADHEIGHT:
			{
				HeadHei = fp->GetWord();
				break;
			}
		case eBODYHEIGHT:
			{
				BodyHei = fp->GetWord();
				break;
			}
		case eINSERTCOLUMN:
			{
				cListCtrl * win = (cListCtrl *)wnd;
				int col_idx =  fp->GetInt();
				int msg_idx =  fp->GetInt();
				char msg2[255]={0,};
				strcpy( msg2, GetMsg(msg_idx) );
				int colSize = fp->GetInt();
				win->InsertColoumn(col_idx, msg2, colSize);
				break;
			}
		case eSETITEMTEXT:
			{
				cListCtrl * win = (cListCtrl *)wnd;
				int col_idx =  fp->GetInt();
				int row_idx =  fp->GetInt();
				int msg_idx =  fp->GetInt();
				char msg2[255]={0,};
				strcpy( msg2, GetMsg(msg_idx) );
				win->SetItemText(col_idx, row_idx, msg2);
				break;
			}
		case eSPINMINMAX:
			{
				minValue = fp->GetLong();
				maxValue = fp->GetLong();
				break;
			}
		case eSPINUNIT:
			{
				spinUnit = fp->GetLong();
				break;
			}
		case eLINEUPWARD:
			{
				lineUpward = fp->GetBool();
				break;
			}
		case eLISTBARINTERVAL:
			{
				interval = fp->GetInt();
				break;
			}
		case eLISTBARPOINT:
			{
				listBarPoint.x = fp->GetDword();
				listBarPoint.y = fp->GetDword();
				listBarPoint.w = fp->GetDword();
				listBarPoint.h = fp->GetDword();
				break;
			}
		case eLISTBARIMAGE:
			{
				GetImage(fp, &ListBarImage);
				//listBarPoint = fp->
				break;
			}
		case eLISTMAXLINE:
			{
				maxLine = fp->GetWord();
				break;
			}
		case eLINEHEIGHT:
			{
				listMaxHeight = fp->GetLong();
				break;
			}
		case eBTNCLICKSOUND:
			{
				soundIdx = fp->GetInt();
				break;
			}
		case eMINMAXMIDDLENUM:
			{
				minMiddleNum = fp->GetByte();

				maxMiddleNum = fp->GetByte();
				break;
			}
		case eINITGRID:
			{
				gridPos.x = fp->GetDword();
				gridPos.y = fp->GetDword();
				gridPos.w = fp->GetDword();
				gridPos.h = fp->GetDword();
				gridBorderX = fp->GetWord();
				gridBorderY = fp->GetWord();
				break;
			}
		case eINITGUAGEBAR:
			{
				cGuageBar * win = (cGuageBar *)wnd;
				interval = fp->GetLong();
				vertical = fp->GetBool();
				win->InitGuageBar(interval, vertical);
				break;
			}
		case eMAXSPRITE:
			{
				cAni * win = (cAni *)wnd;
				win->SetMaxSprite(fp->GetWord());
				break;
			}
		case eSPRITELOOP:
			{
				fSpriteloop = fp->GetBool();
				break;
			}
		case eTRAVELINGTYPE:
			{
				travelingType = fp->GetWord();
				break;
			}
		case eADDSPRITE:
			{
				cAni * win = (cAni *)wnd;
				GetImage(fp, &AniSpriteImage);
				WORD delayTime = fp->GetWord();
				win->AddSprite(&AniSpriteImage, delayTime);
				break;
			}
		case eGUAGEPIECEIMAGE:
			{
				//cGuage * win = (cGuage *)wnd;
				GetImage(fp, &GuagePieceImage);
				//win->SetPieceImage(&GuagePieceImage);
				break;
			}
		case eGUAGEMINVALUE:
			{
				guageMinVal = fp->GetFloat();
				break;
			}
		case eGUAGEMAXVALUE:
			{
				guageMaxVal = fp->GetFloat();
				break;
			}
		case eGUAGECURVALUE:
			{
				guageCurVal = fp->GetFloat();
				break;
			}
		case eGUAGENEVALUE:
			{
				guageCurVal		= fp->GetFloat();
				guageEstTime	= fp->GetDword();
				break;
			}
		case eGUAGEPOS:
			{
				guagePos.x = fp->GetLong();
				guagePos.y = fp->GetLong();
				break;
			}
		case eCHECKBOXIMAGE:
			{
				GetImage(fp, &CheckBoxImage);
				break;
			}
		case eCHECKIMAGE:
			{
				GetImage(fp, &CheckImage);
				break;
			}
		case eSETCHECK:
			{
				bChecked = fp->GetBool();
				break;
			}
		case eSCALE:
			{
				scale.x = fp->GetFloat();
				scale.y = fp->GetFloat();
				break;
			}
		case eLISTSCALE:
			{
				ListScale.x = fp->GetFloat();
				ListScale.y = fp->GetFloat();
				break;
			}
		case eGUAGEWIDTH:
			{
				guageWidth = fp->GetFloat();
				break;
			}
		case eGUAGEPIECEWIDTH:
			{
				guagePieceWidth = fp->GetFloat();
				break;
			}
		case eGUAGEEFFECTPIECEIMAGE:
			{
				GetImage(fp, &guageEffectPieceImage);
				break;
			}
		case eGUAGEEFFECTPIECEWIDTH:
			{
				guageEffectPieceWidth = fp->GetFloat();
				break;
			}
		case eGUAGEPIECEHEIGHTSCALE:
			{
				guagePieceHeightScaleY = fp->GetFloat();
				break;
			}
		case eGUAGEEFFECTPIECEHEIGHTSCALE:
			{
				guageEffectPieceHeightScaleY = fp->GetFloat();
				break;
			}
		case eBALLOONIMAGETOP:
			{
				GetImage(fp, &BalloonImage[0]);
				GetImage(fp, &BalloonImage[1]);
				GetImage(fp, &BalloonImage[2]);
				break;
			}
		case eBALLOONIMAGEMIDDLE:
			{
				GetImage(fp, &BalloonImage[3]);
				GetImage(fp, &BalloonImage[4]);
				GetImage(fp, &BalloonImage[5]);
				break;
			}
		case eBALLOONIMAGEBOTTOM:
			{
				GetImage(fp, &BalloonImage[6]);
				GetImage(fp, &BalloonImage[7]);
				GetImage(fp, &BalloonImage[8]);
				break;
			}
		case eBALLOONIMAGETAIL:
			{
				GetImage(fp, &BalloonImage[9]);
				break;
			}
		case eBALLOONIMAGEBORDER:
			{
				BalloonBorder = fp->GetFloat();
				break;
			}
		case eToolTipLineSize:
			{
				toolTipLineSize = fp->GetDword();
				break;
			}
		case eOFFSET:
			{
				//OffSet.x = fp->GetLong();
				//OffSet.y = fp->GetLong();

				m_OffSetX = fp->GetDword();
				m_OffSetY = fp->GetDword();

				wnd->SetOffSetXY(m_OffSetX, m_OffSetY);
				break;
			}
		case eDisabilityDlg:
			{
				bDisability=fp->GetBool();
				break;
			}
		case eMarquee:
			{
				bEnableMarquee = fp->GetBool();
				break;
			}
		case eMarqueeSize:
			{
				MarqueeSize = fp->GetDword();
				break;
			}
		case eMarqueeSpeed:
			{
				MarqueeSpeed = fp->GetDword();
				break;
			}
		case eStaticFontOutline:
			{
				nFontOutline = fp->GetBool();
				break;
			}
		case eDoubleClick:
			{
				nDoubleClick = fp->GetBool();
				break;
			}
		case eWindowBase:
			{
				nWindowBase = fp->GetByte();
				wnd->SaveWindowBase(nWindowBase);

				/*DISPLAY_INFO	dispInfo ;
				GAMERESRCMNGR->GetDispInfo( &dispInfo ) ;

				VECTOR2 point[1];
				point[0].x = fp->GetInt();
				point[0].y = 0;// fp->GetInt();
				switch (nWindowBase)
				{
				case 1:
					{
						  point[0].x = 0;
						  point[0].y = 0;
					}
					break;
				case 2:
					{
						  point[0].x = ((float)dispInfo.dwWidth / 2 - wid / 2);
						  point[0].y = 0;
					}
					break;
				case 3:
					{
						  point[0].x = ((float)dispInfo.dwWidth - wid);
						  point[0].y = m_absPos.y;
					}
					break;
				case 4:
					{
						  point[0].x = 0;
						  point[0].y = ((float)dispInfo.dwHeight / 2 - hei / 2);
					}
					break;
				case 5:
					{
						  point[0].x = ((float)dispInfo.dwWidth / 2 - wid / 2);
						  point[0].y = ((float)dispInfo.dwHeight / 2 - hei / 2) ;
					}
					break;
				case 6:
					{
						  point[0].x = ((float)dispInfo.dwWidth - wid);
						  point[0].y = ((float)dispInfo.dwHeight / 2 - hei / 2);
					}
					break;
				case 7:
					{
						  point[0].x = m_absPos.x;
						  point[0].y = ((float)dispInfo.dwHeight - hei);
					}
					break;
				case 8://kiv
					{
						  point[0].x = ((float)dispInfo.dwWidth / 2 - wid / 2);
						  point[0].y = (float)dispInfo.dwHeight - hei;
					}
					break;
				case 9:
					{
						  point[0].x = (float)dispInfo.dwWidth - wid;
						  point[0].y = (float)dispInfo.dwHeight - hei;
					}
					break;
				}

				
				wnd->cWindow::SetDeltaPos(0, point[0]);*/
				


				break;
			}
		default:
			//_asm int 3;
			{				
				break;
			}
		}
	}

	
	wnd->SetDisable(bDisability);
	wnd->SetAutoClose(bAutoClose);
	wnd->SetFontIdx(fontIdx);
	wnd->SetImageSrcRect(&imgSrcRect);
	wnd->SetScale(&scale);
	wnd->OnCreate(fActive);

	//wnd->SetAbsXY(wnd->GetAbsX()+OffSet.x%80,wnd->GetAbsY()+OffSet.y%80);
	//wnd->SetOffSetXY(OffSet.x,OffSet.y);

	SWINDOWTYPE(wnd->GetType())
	/*CASE(WT_DIVIDEBOX)
		cDivideBox * win = (cDivideBox *)wnd;
	*/	
	CASE(WT_GUAGE)
		cGuage * win = (cGuage *)wnd;
		win->Init(point.x, point.y, point.w, point.h, &BasicImage, IDSEARCH(IDName));
		win->InitGuage(guagePos.x,guagePos.y);
		win->SetMinValue(guageMinVal);
		win->SetMaxValue(guageMaxVal);
		win->SetCurValue(guageCurVal);
		win->SetPieceImage(&GuagePieceImage);
		
	BREAK
	CASE(WT_GUAGEN)
		cGuagen * win = (cGuagen *)wnd;
		win->Init(point.x, point.y, point.w, point.h, &BasicImage, IDSEARCH(IDName));
		win->SetGuageImagePos(guagePos.x,guagePos.y);
		win->SetValue(guageCurVal);
		win->SetPieceImage(&GuagePieceImage);
		win->SetGuageWidth(guageWidth);
		win->SetGuagePieceWidth(guagePieceWidth);
	BREAK
	CASE(WT_GUAGENE)
		CObjectGuagen * win = (CObjectGuagen *)wnd;
		win->Init(point.x, point.y, point.w, point.h, &BasicImage, IDSEARCH(IDName));
		win->SetGuageImagePos(guagePos.x,guagePos.y);
		win->SetValue(guageCurVal, guageEstTime);
		win->SetPieceImage(&GuagePieceImage);
		win->SetGuageWidth(guageWidth);
		win->SetGuagePieceWidth(guagePieceWidth);
		win->SetGuagePieceHeightScale(guagePieceHeightScaleY);


		win->SetGuageEffectPieceWidth(guageEffectPieceWidth);
		win->SetEffectPieceImage(&guageEffectPieceImage);
		win->SetGuageEffectPieceHeightScale(guageEffectPieceHeightScaleY);
	BREAK
	CASE(WT_ANI)
		cAni * win = (cAni *)wnd;
		win->Init(point.x, point.y, point.w, point.h, &BasicImage, IDSEARCH(IDName));
		win->SetValidXY(validXY.x,validXY.y);
		win->SetLoop(fSpriteloop);
		win->SetTravelingType(travelingType);
		win->SetAlpha(alphaDepth);
	BREAK
	CASE(WT_SPIN)
		cSpin * win = (cSpin *)wnd;
		if(FUNCSEARCH(funcName) == -1)
			win->Init(point.x, point.y, point.w, point.h, &BasicImage, NULL, IDSEARCH(IDName));
		else
			win->Init(point.x, point.y, point.w, point.h, &BasicImage, g_mt_func[FUNCSEARCH(funcName)]._func, IDSEARCH(IDName));
		win->SetValidXY(validXY.x,validXY.y);
		win->InitSpin(EditBoxSize, EditBoxEableTypingSize);
		win->SetActiveTextColor(RGB_HALF(activeTextColor.r,activeTextColor.g,activeTextColor.b));
		win->SetNonactiveTextColor(RGB_HALF(nonactiveTextColor.r,nonactiveTextColor.g,nonactiveTextColor.b));
		win->SetMinMax(minValue, maxValue);
		win->SetUnit(spinUnit);
		win->SetAlpha(alphaDepth);
	BREAK
	CASE(WT_STATIC)
		cStatic * win = (cStatic *)wnd;
		win->Init(point.x, point.y, point.w, point.h, &BasicImage, IDSEARCH(IDName));
		win->SetValidXY(validXY.x,validXY.y);
		win->SetTextXY(textXY.x, textXY.y);
		win->SetMarqueeSpeed(MarqueeSpeed);
		if (bEnableMarquee)
		{
			//std::string test = staticText;
			//win->SetStaticText((char *)marquee_limit(test, MarqueeSize).c_str());

			char strBuf2[256];
			memset(strBuf2,0,256);
			memcpy(strBuf2, staticText, strlen(staticText));
			sprintf(strBuf2, "%s ", staticText);
			win->SetStaticText(strBuf2, true, MarqueeSize);
		}
		else
		{
			win->SetStaticText(staticText);
		}
		win->SetFontOutline(nFontOutline);
		win->SetAlign( nTextAlign );
		win->SetFGColor(RGB_HALF(fgColor.r, fgColor.g, fgColor.b));
		win->SetShadowTextXY(shadowTextX, shadowTextY);
		win->SetShadowColor(RGB_HALF(dwShadowColor.r, dwShadowColor.g, dwShadowColor.b));
		win->SetShadow(bShadow);

		if( toolTipLineSize )
		{
			win->SetAutoLineToolTip( toolTipMsg, RGBA_MAKE(color.r,color.g,color.b, 255), &ToolTipImage, toolTipLineSize );
		}
		else
		{
			win->SetToolTip(toolTipMsg, RGBA_MAKE(color.r,color.g,color.b, 255), &ToolTipImage);
		}
		//win->SetToolTip(toolTipMsg, RGBA_MAKE(color.r,color.g,color.b, 255), &ToolTipImage);

		if( tt_idx >= 0 )
		{
			cImage imgToolTip;
			SCRIPTMGR->GetImage( 63, &imgToolTip, PFT_HARDPATH );
			win->SetToolTip( "", RGBA_MAKE(255, 255, 255, 255), &imgToolTip, TTCLR_DEFAULT );
			ITEMMGR->AddItemDescriptionToolTip( win, tt_idx, TTTC_DEFAULT, 0 );
		}

	BREAK
	CASE(WT_PUSHUPBUTTON)
		cPushupButton * win = (cPushupButton *)wnd;
		win->SetValidXY(validXY.x,validXY.y);
		if(FUNCSEARCH(funcName) == -1)
			win->Init(point.x, point.y, point.w, point.h, &BasicImage, &OverImage, &PressImage, NULL, IDSEARCH(IDName));
		else	
			win->Init(point.x, point.y, point.w, point.h, &BasicImage, &OverImage, &PressImage, g_mt_func[FUNCSEARCH(funcName)]._func, IDSEARCH(IDName));
		//win->SetToolTip(toolTipMsg, RGBA_MAKE(color.r,color.g,color.b, 255), &ToolTipImage);		

		if( toolTipLineSize )
		{
			win->SetAutoLineToolTip( toolTipMsg, RGBA_MAKE(color.r,color.g,color.b, 255), &ToolTipImage, toolTipLineSize );
		}
		else
		{
			win->SetToolTip(toolTipMsg, RGBA_MAKE(color.r,color.g,color.b, 255), &ToolTipImage);
		}

		win->SetDoubleClick(nDoubleClick);
		win->SetPush(fPush);
		win->SetText(szBtnText, RGB_HALF(btnTextBasicCol.r,btnTextBasicCol.g,btnTextBasicCol.b), RGB_HALF(btnTextOverCol.r,btnTextOverCol.g,btnTextOverCol.b), RGB_HALF(btnTextPressCol.r,btnTextPressCol.g,btnTextPressCol.b)); 
		win->SetTextXY(textXY.x,textXY.y);
		win->SetAlpha(alphaDepth);
		win->SetTextAni(btnTextAniX, btnTextAniY );

		win->SetAlign( nTextAlign );
		win->SetShadowTextXY(shadowTextX, shadowTextY);
		win->SetShadowColor(RGB_HALF(dwShadowColor.r, dwShadowColor.g, dwShadowColor.b));
		win->SetShadow(bShadow);
		win->SetPassive(bPassive);
	BREAK
	CASE(WT_COMBOBOX)
		cComboBox * win = (cComboBox *)wnd;
		win->SetValidXY(validXY.x,validXY.y);
		win->Init(point.x, point.y, point.w, point.h, &BasicImage, NULL, IDSEARCH(IDName));
		win->SetComboTextColor(RGB_HALF(comboTextColor.r, comboTextColor.g, comboTextColor.b));
		win->SetAlpha(alphaDepth);
		win->SelectComboText(selectComboIdx);

		win->SetOverImageScale(ListScale.x, ListScale.y); // over image Ãß°¡
		if(-1 != FUNCSEARCH(funcName))
			win->SetcbFunc(g_mt_func[FUNCSEARCH(funcName)]._func);
		//win->Init()
	BREAK
	CASE(WT_COMBOBOXEX)
		cComboBoxEx * win = (cComboBoxEx *)wnd;
		win->SetValidXY(validXY.x,validXY.y);
		win->Init(point.x, point.y, point.w, point.h, &BasicImage, NULL, IDSEARCH(IDName));
		win->SetComboTextColor(RGB_HALF(comboTextColor.r, comboTextColor.g, comboTextColor.b));
		win->SetAlpha(alphaDepth);
		win->SelectComboText(selectComboIdx);
		win->SetOverImageScale(ListScale.x, ListScale.y); // over image Ãß°¡
		if(-1 != FUNCSEARCH(funcName))
			win->SetcbFunc(g_mt_func[FUNCSEARCH(funcName)]._func);
		//win->Init()
	BREAK
	CASE(WT_BUTTON)
		cButton * win = (cButton *)wnd;
		//btn->Init(point.x, point.y, point.w, point.h, pBtnImageBasic, pBtnImageOver, pBtnImagePress, mt_func[MT_FUNCSEARCH(funcName)], MT_IDSEARCH(IDName));
		if(FUNCSEARCH(funcName) == -1)
			win->Init(point.x, point.y, point.w, point.h, &BasicImage, &OverImage, &PressImage, NULL, IDSEARCH(IDName));
		else 
			win->Init(point.x, point.y, point.w, point.h, &BasicImage, &OverImage, &PressImage, g_mt_func[FUNCSEARCH(funcName)]._func, IDSEARCH(IDName));

		if(FUNCSEARCH(funcName) == -1)
		{
			int a = 0;
		}

		//win->SetToolTip(toolTipMsg, RGBA_MAKE(color.r,color.g,color.b, 255), &ToolTipImage);

		if( toolTipLineSize )
		{
			win->SetAutoLineToolTip( toolTipMsg, RGBA_MAKE(color.r,color.g,color.b, 255), &ToolTipImage, toolTipLineSize );
		}
		else
		{
			win->SetToolTip(toolTipMsg, RGBA_MAKE(color.r,color.g,color.b, 255), &ToolTipImage);
		}

		if(*szBtnText != 0)
		
		win->SetText(szBtnText, RGB_HALF(btnTextBasicCol.r,btnTextBasicCol.g,btnTextBasicCol.b), RGB_HALF(btnTextOverCol.r,btnTextOverCol.g,btnTextOverCol.b), RGB_HALF(btnTextPressCol.r,btnTextPressCol.g,btnTextPressCol.b)); 
		win->SetTextXY(textXY.x,textXY.y);
		win->SetAlpha(alphaDepth);
		win->SetTextAni(btnTextAniX, btnTextAniY );
		win->SetAlign( nTextAlign );

		win->SetShadowTextXY(shadowTextX, shadowTextY);
		win->SetShadowColor(RGB_HALF(dwShadowColor.r, dwShadowColor.g, dwShadowColor.b));
		win->SetShadow(bShadow);
		win->SetDisable(bDisability);

		if( soundIdx != -1 )
			win->SetClickSound( soundIdx );
	BREAK
	CASE(WT_CHECKBOX)	
		cCheckBox * win = (cCheckBox *)wnd;
		if(FUNCSEARCH(funcName) == -1)
			win->Init(point.x, point.y, point.w, point.h, &BasicImage, &CheckBoxImage, &CheckImage, NULL, IDSEARCH(IDName));
		else
			win->Init(point.x, point.y, point.w, point.h, &BasicImage, &CheckBoxImage, &CheckImage, g_mt_func[FUNCSEARCH(funcName)]._func, IDSEARCH(IDName));
		win->SetChecked(bChecked);
		win->SetCheckBoxMsg(staticText, RGB_HALF(fgColor.r,fgColor.g,fgColor.b));

		if( toolTipLineSize )
		{
			win->SetAutoLineToolTip( toolTipMsg, RGBA_MAKE(color.r,color.g,color.b, 255), &ToolTipImage, toolTipLineSize );
		}
		else
		{
			win->SetToolTip(toolTipMsg, RGBA_MAKE(color.r,color.g,color.b, 255), &ToolTipImage);
		}
	BREAK
	CASE(WT_EDITBOX)
		cEditBox * win = (cEditBox *)wnd;
		//editBox->Init(point.x, point.y, point.w, point.h, &BasicImage, MT_IDSEARCH(IDName));
		win->Init(point.x, point.y, point.w, point.h, &BasicImage, &FocusImage, IDSEARCH(IDName));
		win->InitEditbox(EditBoxSize, EditBoxEableTypingSize);
		win->SetAlign( nTextAlign );
		//win->SetToolTip(toolTipMsg, RGBA_MAKE(color.r,color.g,color.b, 255), &ToolTipImage);
		if( toolTipLineSize )
		{
			win->SetAutoLineToolTip( toolTipMsg, RGBA_MAKE(color.r,color.g,color.b, 255), &ToolTipImage, toolTipLineSize );
		}
		else
		{
			win->SetToolTip(toolTipMsg, RGBA_MAKE(color.r,color.g,color.b, 255), &ToolTipImage);
		}
		if(-1 != FUNCSEARCH(funcName))
			win->SetcbFunc(g_mt_func[FUNCSEARCH(funcName)]._func);
		//win->SetcbFunc(mt_func[FUNCSEARCH(funcName)]);
		win->SetSecret(fSecret);
		win->SetActiveTextColor(RGB_HALF(activeTextColor.r,activeTextColor.g,activeTextColor.b));
		win->SetNonactiveTextColor(RGB_HALF(nonactiveTextColor.r,nonactiveTextColor.g,nonactiveTextColor.b));
		win->SetAlpha(alphaDepth);
		win->SetReadOnly(bReadOnly);
	BREAK
	CASE(WT_GUAGEBAR)
		cGuageBar * win = (cGuageBar *)wnd;
		win->SetValidXY(validXY.x,validXY.y);
		win->Init(point.x, point.y, point.w, point.h, &BasicImage, IDSEARCH(IDName));
		win->InitValue(guageMinVal, guageMaxVal, guageCurVal);
		win->SetAlpha(alphaDepth);
		//if(-1 != FUNCSEARCH(funcName))
		//	win->SetcbFunc(g_mt_func[FUNCSEARCH(funcName)]._func);
	BREAK
	CASE(WT_TEXTAREA)
		cTextArea * win = (cTextArea *)wnd;
		win->SetValidXY(validXY.x,validXY.y);
		win->Init(point.x, point.y, point.w, point.h, &BasicImage, IDSEARCH(IDName));
		win->InitTextArea(&textRect, nLimitBytes, &TopImage, topHei, &MiddleImage, middleHei, &DownImage, downHei );
		win->SetLimitLine( limitLineNum );	//inittextareaº¸´Ù ¾Æ·¡¿¡ ÀÖ¾î¾ßÇÑ´Ù.
		win->SetAlpha(alphaDepth);
		win->SetReadOnly(bReadOnly);
		win->SetMiddleScale( ListScale );
		win->SetScriptText(staticText);
		
		//if(-1 != FUNCSEARCH(funcName))
		//	win->SetcbFunc(g_mt_func[FUNCSEARCH(funcName)]._func);
	BREAK
	CASE(WT_LIST)
		cList * win = (cList *)wnd;
		win->SetValidXY(validXY.x,validXY.y);
		win->Init(point.x, point.y, point.w, point.h, &TopImage, IDSEARCH(IDName));
		win->InitList(maxLine, &textRect);
		win->SetLineUpward(lineUpward);
		win->SetListBarImage(&ListBarImage);
		win->SetListBar(listBarPoint.x, listBarPoint.y, listBarPoint.w, listBarPoint.h, interval);
		win->SetMaxLine(maxLine);
		win->SetAlpha(alphaDepth);
		
		//if(-1 != FUNCSEARCH(funcName))
		//	win->SetcbFunc(g_mt_func[FUNCSEARCH(funcName)]._func);
	BREAK
	CASE(WT_LISTCTRL)
		cListCtrl * win = (cListCtrl *)wnd;
		win->SetValidXY(validXY.x,validXY.y);
		win->Init(point.x, point.y, point.w, point.h, &BasicImage, IDSEARCH(IDName));
		win->InitListCtrl(cols, rows);
		win->InitListCtrlImage(&HeadImage, HeadHei, &BodyImage, BodyHei, &ListOverImage );
		win->SetOverImageScale(ListScale.x, ListScale.y); // over image Ãß°¡
		win->SetAlpha(alphaDepth);
		win->SetSelectOption( wOption );
	BREAK
	CASE(WT_LISTCTRLEX)
		cListCtrlEx * win = (cListCtrlEx *)wnd;
		win->SetValidXY(validXY.x,validXY.y);
		win->Init(point.x, point.y, point.w, point.h, &BasicImage, IDSEARCH(IDName));
		win->InitListCtrlEx( cols, middleNum, middleHei, nLeftMargin, nTopMargin );
		win->InitListCtrlExImage( &HeadImage, &BodyImage, &DownImage, &SelectedImage, &ListOverImage );
		win->SetAlpha(alphaDepth);
	BREAK
	CASE(WT_LISTDIALOG)
		cListDialog * win = (cListDialog *)wnd;
		win->SetValidXY(validXY.x,validXY.y);
		win->SetMinMaxMiddleNum(minMiddleNum, maxMiddleNum);
		win->Init(point.x, point.y, point.w, &TopImage, topHei, &MiddleImage, middleHei, &DownImage, downHei, &ListOverImage, middleNum, IDSEARCH(IDName));
		win->SetLineHeight(listMaxHeight);
		win->InitList(maxLine, &textRect);
		win->SetMaxLine(maxLine);
		win->SetOverImageScale(ListScale.x, ListScale.y); // over image Ãß°¡
		//win->SetListBarImage(&ListBarImage);
		//win->SetListBar(listBarPoint.x, listBarPoint.y, listBarPoint.w, listBarPoint.h, interval);
		win->SetAlpha(alphaDepth);
		win->SetAutoScroll( bAutoScroll );

		if(-1 != FUNCSEARCH(funcName))
			win->SetcbFunc(g_mt_func[FUNCSEARCH(funcName)]._func);
		//dlg->Init(point.x, point.y, point.w, point.h, &BasicImage, MT_IDSEARCH(IDName));
		//win->Init(point.x, point.y, point.w, point.h, &BasicImage, IDSEARCH(IDName));
		//win->SetMovable(fMovable);
		//win->SetCaptionRect(&captionRect);
	BREAK
	CASE(WT_CHARMAKEDIALOG)
		cCharMakeDlg * win = (cCharMakeDlg *)wnd;
		win->SetValidXY(validXY.x,validXY.y);
		//dlg->Init(point.x, point.y, point.w, point.h, &BasicImage, MT_IDSEARCH(IDName));
		win->Init(point.x, point.y, point.w, point.h, &BasicImage, IDSEARCH(IDName));
		win->SetMovable(fMovable);
		win->SetCaptionRect(&captionRect);
		win->SetAlpha(alphaDepth);
		if(-1 != FUNCSEARCH(funcName))
			win->SetcbFunc(g_mt_func[FUNCSEARCH(funcName)]._func);
	BREAK
	CASE(WT_DIALOG)
		cDialog * win = (cDialog *)wnd;
		win->SetValidXY(validXY.x,validXY.y);
		//dlg->Init(point.x, point.y, point.w, point.h, &BasicImage, MT_IDSEARCH(IDName));
		win->Init(point.x, point.y, point.w, point.h, &BasicImage, IDSEARCH(IDName));
		win->SetMovable(fMovable);
		win->SetCaptionRect(&captionRect);
		win->SetAlpha(alphaDepth);
		if(-1 != FUNCSEARCH(funcName))
			win->SetcbFunc(g_mt_func[FUNCSEARCH(funcName)]._func);
	BREAK
	CASE(WT_TABDIALOG)
		cTabDialog * win = (cTabDialog *)wnd;
		win->SetValidXY(validXY.x,validXY.y);
		win->Init(point.x, point.y, point.w, point.h, &BasicImage, IDSEARCH(IDName));
		win->SetMovable(fMovable);
		win->SetCaptionRect(&captionRect);
		if(-1 != FUNCSEARCH(funcName))
			win->SetcbFunc(g_mt_func[FUNCSEARCH(funcName)]._func);
	BREAK
	CASE(WT_CHATDIALOG)
		CChatDialog * win = (CChatDialog *)wnd;
		win->SetValidXY(validXY.x,validXY.y);
		win->Init(point.x, point.y, point.w, point.h, &BasicImage, IDSEARCH(IDName));
		win->SetMovable(fMovable);
		win->SetCaptionRect(&captionRect);
		win->SetAlpha(alphaDepth);
		if(-1 != FUNCSEARCH(funcName))
			win->SetcbFunc(g_mt_func[FUNCSEARCH(funcName)]._func);		
	BREAK
	CASE(WT_ICONDIALOG)
		cIconDialog * win = (cIconDialog *)wnd;
		win->SetValidXY(validXY.x,validXY.y);
		win->Init(point.x, point.y, point.w, point.h, &BasicImage, IDSEARCH(IDName));
		//win->SetMovable(fMovable);
		win->SetCaptionRect(&captionRect);
		win->SetAlpha(alphaDepth);
		win->SetIconCellBGImage(&selectedBGImage);
		win->SetDragOverBGImage(&dragoverBGImage);
		if(-1 != FUNCSEARCH(funcName))
			win->SetcbFunc(g_mt_func[FUNCSEARCH(funcName)]._func);
	BREAK
	CASE(WT_WEAREDDIALOG)
		CWearedExDialog * win = (CWearedExDialog *)wnd;
		win->SetValidXY(validXY.x,validXY.y);
		win->Init(point.x, point.y, point.w, point.h, &BasicImage, IDSEARCH(IDName));
		//win->SetMovable(fMovable);
		win->SetCaptionRect(&captionRect);
		win->SetAlpha(alphaDepth);
		win->SetIconCellBGImage(&selectedBGImage);
		win->SetDragOverBGImage(&dragoverBGImage);
		if(-1 != FUNCSEARCH(funcName))
			win->SetcbFunc(g_mt_func[FUNCSEARCH(funcName)]._func);
	BREAK
	CASE(WT_ICONGRIDDIALOG)
		cIconGridDialog * win = (cIconGridDialog *)wnd;

		win->SetValidXY(validXY.x,validXY.y);	
		//dlg->Init(point.x, point.y, point.w, point.h, &BasicImage, MT_IDSEARCH(IDName));
		win->InitDialog(point.x, point.y, point.w, point.h, &BasicImage, cols, rows, IDSEARCH(IDName));
		//win->SetMovable(fMovable);
		win->SetCaptionRect(&captionRect);
		win->InitGrid(gridPos.x,gridPos.y,gridPos.w,gridPos.h,gridBorderX,gridBorderY);
		win->SetAlpha(alphaDepth);
		win->SetIconCellBGImage(&selectedBGImage);
		win->SetDragOverBGImage(&dragoverBGImage);
		if(-1 != FUNCSEARCH(funcName))
			win->SetcbFunc(g_mt_func[FUNCSEARCH(funcName)]._func);
	BREAK
	CASE(WT_MONSTERGUAGEDLG)
		cDialog * win = (cDialog *)wnd;
		win->SetValidXY(validXY.x,validXY.y);
		//dlg->Init(point.x, point.y, point.w, point.h, &BasicImage, MT_IDSEARCH(IDName));
		win->Init(point.x, point.y, point.w, point.h, &BasicImage, IDSEARCH(IDName));
		win->SetMovable(fMovable);
		win->SetCaptionRect(&captionRect);
		win->SetAlpha(alphaDepth);
		if(-1 != FUNCSEARCH(funcName))
			win->SetcbFunc(g_mt_func[FUNCSEARCH(funcName)]._func);
	BREAK
	CASE(WT_QUICKDIALOG)
		CQuickDialog * win = (CQuickDialog *)wnd;
		win->SetValidXY(validXY.x,validXY.y);		
		//dlg->Init(point.x, point.y, point.w, point.h, &BasicImage, MT_IDSEARCH(IDName));
		win->Init(point.x, point.y, point.w, point.h, &BasicImage, IDSEARCH(IDName));
		//win->SetMovable(fMovable);
		win->SetCaptionRect(&captionRect);
		win->SetAlpha(alphaDepth);
		if(-1 != FUNCSEARCH(funcName))
			win->SetcbFunc(g_mt_func[FUNCSEARCH(funcName)]._func);
	BREAK
	CASE(WT_CHARINFODIALOG)
		CCharacterDialog * win = (CCharacterDialog *)wnd;
		win->SetValidXY(validXY.x,validXY.y);
		win->Init(point.x, point.y, point.w, point.h, &BasicImage, IDSEARCH(IDName));
		win->SetMovable(fMovable);
		win->SetCaptionRect(&captionRect);
		win->SetAlpha(alphaDepth);
		if(-1 != FUNCSEARCH(funcName))
			win->SetcbFunc(g_mt_func[FUNCSEARCH(funcName)]._func);
	BREAK
		CASE(WT_CHARINFOPVPDIALOG)
		CCharacterDialog* win = (CCharacterDialog*)wnd;
	win->SetValidXY(validXY.x, validXY.y);
	win->Init(point.x, point.y, point.w, point.h, &BasicImage, IDSEARCH(IDName));
	win->SetMovable(fMovable);
	win->SetCaptionRect(&captionRect);
	win->SetAlpha(alphaDepth);
	if (-1 != FUNCSEARCH(funcName))
		win->SetcbFunc(g_mt_func[FUNCSEARCH(funcName)]._func);
	BREAK
		
	CASE(WT_MUGONGSURYUNDIALOG)
		CMugongSuryunDialog * win = (CMugongSuryunDialog *)wnd;
		win->SetValidXY(validXY.x,validXY.y);
		win->Init(point.x, point.y, point.w, point.h, &BasicImage, IDSEARCH(IDName));
		win->SetMovable(fMovable);
		win->SetCaptionRect(&captionRect);
		win->SetAlpha(alphaDepth);
		win->SetActive(fActive);
		if(-1 != FUNCSEARCH(funcName))
			win->SetcbFunc(g_mt_func[FUNCSEARCH(funcName)]._func);
	BREAK
	CASE(WT_MUGONGDIALOG)
		CMugongDialog * win = (CMugongDialog *)wnd;
		win->SetValidXY(validXY.x,validXY.y);
		win->Init(point.x, point.y, point.w, point.h, &BasicImage, IDSEARCH(IDName));
		win->SetMovable(fMovable);
		win->SetCaptionRect(&captionRect);
		win->SetAlpha(alphaDepth);
		win->SetIconCellBGImage(&selectedBGImage);
		win->SetDragOverBGImage(&dragoverBGImage);
		win->SetActive(fActive);
		if(-1 != FUNCSEARCH(funcName))
			win->SetcbFunc(g_mt_func[FUNCSEARCH(funcName)]._func);
	BREAK
	CASE(WT_SURYUNDIALOG)
		CSuryunDialog * win = (CSuryunDialog *)wnd;
		win->SetValidXY(validXY.x,validXY.y);
		win->Init(point.x, point.y, point.w, point.h, &BasicImage, IDSEARCH(IDName));
		win->SetMovable(fMovable);
		win->SetCaptionRect(&captionRect);
		win->SetAlpha(alphaDepth);
		win->SetActive(fActive);
		if(-1 != FUNCSEARCH(funcName))
			win->SetcbFunc(g_mt_func[FUNCSEARCH(funcName)]._func);
	BREAK
	CASE(WT_MAINDIALOG)
		CMugongDialog * win = (CMugongDialog *)wnd;	
		win->SetValidXY(validXY.x,validXY.y);
		win->Init(point.x, point.y, point.w, point.h, &BasicImage, IDSEARCH(IDName));
		win->SetMovable(fMovable);
		win->SetCaptionRect(&captionRect);
		win->SetAlpha(alphaDepth);
		if(-1 != FUNCSEARCH(funcName))
			win->SetcbFunc(g_mt_func[FUNCSEARCH(funcName)]._func);
	BREAK
	CASE(WT_INVENTORYDIALOG)
		CInventoryExDialog * win = (CInventoryExDialog *)wnd;
		win->SetValidXY(validXY.x,validXY.y);
		win->Init(point.x, point.y, point.w, point.h, &BasicImage, IDSEARCH(IDName));
		//win->InitDialog(point.x, point.y, point.w, point.h, &BasicImage, cols, rows, IDSEARCH(IDName));
		//win->InitItemGrid(eItemGrid_Inventory,MAX_INVENTORY_NUM);
		win->SetMovable(fMovable);
		//win->Init(point.x, point.y, point.w, point.h, &BasicImage, IDSEARCH(IDName))
		win->SetCaptionRect(&captionRect);
		win->SetAlpha(alphaDepth);
		if(-1 != FUNCSEARCH(funcName))
			win->SetcbFunc(g_mt_func[FUNCSEARCH(funcName)]._func);
	BREAK
	CASE(WT_HELPERSPEECHDIALOG)
		cHelperSpeechDlg* win = (cHelperSpeechDlg*)wnd;
		win->SetValidXY(validXY.x, validXY.y);
		win->Init(point.x, point.y, point.w, point.h, listMaxHeight, IDSEARCH(IDName) );
		win->Linking();
		win->InitBalloon( &BalloonImage[0], &BalloonImage[2], &BalloonImage[6], &BalloonImage[8], &BalloonImage[3],
			   &BalloonImage[5], &BalloonImage[1], &BalloonImage[7], &BalloonImage[4], &BalloonImage[9], BalloonBorder);
		win->SetTextRect(&textRect);
		win->SetCaptionRect(&captionRect);
		win->SetMovable(fMovable);
		win->SetAlpha(alphaDepth);
		if(-1 != FUNCSEARCH(funcName))
			win->SetcbFunc(g_mt_func[FUNCSEARCH(funcName)]._func);
	BREAK
	CASE(WT_STREETSTALLDIALOG)
		CStreetStall * win = (CStreetStall *)wnd;	
		win->SetValidXY(validXY.x,validXY.y);
		win->Init(point.x, point.y, point.w, point.h, &BasicImage, IDSEARCH(IDName));
		//win->InitDialog(point.x, point.y, point.w, point.h, &BasicImage, cols, rows, IDSEARCH(IDName));
		//win->InitItemGrid(eItemGrid_Inventory,MAX_INVENTORY_NUM);
		win->SetMovable(fMovable);
		win->SetCaptionRect(&captionRect);
		win->SetAlpha(alphaDepth);
		if(-1 != FUNCSEARCH(funcName))
			win->SetcbFunc(g_mt_func[FUNCSEARCH(funcName)]._func);
	BREAK
	CASE(WT_STREETBUYSTALLDIALOG)
		CStreetBuyStall * win = (CStreetBuyStall *)wnd;	
		win->SetValidXY(validXY.x,validXY.y);
		win->Init(point.x, point.y, point.w, point.h, &BasicImage, IDSEARCH(IDName));
		//win->InitDialog(point.x, point.y, point.w, point.h, &BasicImage, cols, rows, IDSEARCH(IDName));
		//win->InitItemGrid(eItemGrid_Inventory,MAX_INVENTORY_NUM);
		win->SetMovable(fMovable);
		win->SetCaptionRect(&captionRect);
		win->SetAlpha(alphaDepth);
		if(-1 != FUNCSEARCH(funcName))
			win->SetcbFunc(g_mt_func[FUNCSEARCH(funcName)]._func);
	BREAK
	CASE(WT_NPCSCRIPDIALOG)
		cNpcScriptDialog * win = (cNpcScriptDialog *)wnd;
		win->SetValidXY(validXY.x,validXY.y);
		win->Init(point.x, point.y, point.w, point.h, &BasicImage, IDSEARCH(IDName));
		//win->InitDialog(point.x, point.y, point.w, point.h, pBasicImage, cols, rows, IDSEARCH(IDName));
		//win->InitItemGrid(eItemGrid_Inventory,MAX_INVENTORY_NUM);
		win->SetMovable(fMovable);
		win->SetCaptionRect(&captionRect);
		win->SetAlpha(alphaDepth);

		if(-1 != FUNCSEARCH(funcName))
			win->SetcbFunc(g_mt_func[FUNCSEARCH(funcName)]._func);
	BREAK
	CASE(WT_HELPDIALOG)
		cHelpDialog * win = (cHelpDialog *)wnd;
		win->SetValidXY(validXY.x,validXY.y);
		win->Init(point.x, point.y, point.w, point.h, &BasicImage, IDSEARCH(IDName));
		//win->InitDialog(point.x, point.y, point.w, point.h, pBasicImage, cols, rows, IDSEARCH(IDName));
		//win->InitItemGrid(eItemGrid_Inventory,MAX_INVENTORY_NUM);
		win->SetMovable(fMovable);
		win->SetCaptionRect(&captionRect);
		win->SetAlpha(alphaDepth);
		if(-1 != FUNCSEARCH(funcName))
			win->SetcbFunc(g_mt_func[FUNCSEARCH(funcName)]._func);
	BREAK
	CASE(WT_MONEYDIALOG)
		CMoneyDlg * win = (CMoneyDlg *)wnd;
		win->SetValidXY(validXY.x,validXY.y);
		win->Init(point.x, point.y, point.w, point.h, &BasicImage, IDSEARCH(IDName));
		win->SetMovable(fMovable);
		win->SetCaptionRect(&captionRect);
		win->SetAlpha(alphaDepth);
		if(-1 != FUNCSEARCH(funcName))
			win->SetcbFunc(g_mt_func[FUNCSEARCH(funcName)]._func);
	BREAK
	CASE(WT_DEALDIALOG)
		CDealDialog * win = (CDealDialog *)wnd;
		win->SetValidXY(validXY.x,validXY.y);
		win->Init(point.x, point.y, point.w, point.h, &BasicImage, IDSEARCH(IDName));
		//win->InitDialog(point.x, point.y, point.w, point.h, &BasicImage, cols, rows, IDSEARCH(IDName));
		//win->InitItemGrid(eItemGrid_Inventory,MAX_INVENTORY_NUM);
		win->SetMovable(fMovable);
		//win->Init(point.x, point.y, point.w, point.h, &BasicImage, IDSEARCH(IDName))
		win->SetCaptionRect(&captionRect);
		win->SetAlpha(alphaDepth);
		if(-1 != FUNCSEARCH(funcName))
			win->SetcbFunc(g_mt_func[FUNCSEARCH(funcName)]._func);
	BREAK
	CASE(WT_MUNPAMARKDLG)
		CMunpaMarkDialog * win = (CMunpaMarkDialog*)wnd;
		win->SetValidXY(validXY.x,validXY.y);
		win->Init(point.x, point.y, point.w, point.h, &BasicImage, IDSEARCH(IDName));
		win->SetMovable(fMovable);
		win->SetCaptionRect(&captionRect);
		win->SetAlpha(alphaDepth);
		if(-1 != FUNCSEARCH(funcName))
			win->SetcbFunc(g_mt_func[FUNCSEARCH(funcName)]._func);
	BREAK
	CASE(WT_PYOGUKDLG)
		CPyogukDialog * win = (CPyogukDialog*)wnd;
		win->SetValidXY(validXY.x,validXY.y);
		win->Init(point.x, point.y, point.w, point.h, &BasicImage, IDSEARCH(IDName));
		win->SetMovable(fMovable);
		win->SetCaptionRect(&captionRect);
		win->SetAlpha(alphaDepth);
		if(-1 != FUNCSEARCH(funcName))
			win->SetcbFunc(g_mt_func[FUNCSEARCH(funcName)]._func);
	BREAK		
	CASE(WT_NOTEDLG)
		CNoteDialog * win = (CNoteDialog*)wnd;
		win->SetValidXY(validXY.x,validXY.y);
		win->Init(point.x, point.y, point.w, point.h, &BasicImage, IDSEARCH(IDName));
		win->SetMovable(fMovable);
		win->SetCaptionRect(&captionRect);
		win->SetAlpha(alphaDepth);
		if(-1 != FUNCSEARCH(funcName))
			win->SetcbFunc(g_mt_func[FUNCSEARCH(funcName)]._func);
	BREAK		
	CASE(WT_MININOTEDLG)
		CMiniNoteDialog * win = (CMiniNoteDialog*)wnd;
		win->SetValidXY(validXY.x,validXY.y);
		win->Init(point.x, point.y, point.w, point.h, &BasicImage, IDSEARCH(IDName));
		win->SetMovable(fMovable);
		win->SetCaptionRect(&captionRect);
		win->SetAlpha(alphaDepth);
		if(-1 != FUNCSEARCH(funcName))
			win->SetcbFunc(g_mt_func[FUNCSEARCH(funcName)]._func);
	BREAK
	CASE(WT_FRIENDDLG)
		CFriendDialog * win = (CFriendDialog*)wnd;
		win->SetValidXY(validXY.x,validXY.y);
		win->Init(point.x, point.y, point.w, point.h, &BasicImage, IDSEARCH(IDName));
		win->SetMovable(fMovable);
		win->SetCaptionRect(&captionRect);
		win->SetAlpha(alphaDepth);
		if(-1 != FUNCSEARCH(funcName))
			win->SetcbFunc(g_mt_func[FUNCSEARCH(funcName)]._func);
	BREAK		
	CASE(WT_MINIFRIENDDLG)
		CMiniFriendDialog * win = (CMiniFriendDialog*)wnd;
		win->SetValidXY(validXY.x,validXY.y);
		win->Init(point.x, point.y, point.w, point.h, &BasicImage, IDSEARCH(IDName));
		win->SetMovable(fMovable);
		win->SetCaptionRect(&captionRect);
		win->SetAlpha(alphaDepth);
		if(-1 != FUNCSEARCH(funcName))
			win->SetcbFunc(g_mt_func[FUNCSEARCH(funcName)]._func);
	BREAK		
	CASE(WT_WANTNPCDIALOG)
		CWantNpcDialog * win = (CWantNpcDialog*)wnd;
		win->SetValidXY(validXY.x,validXY.y);
		win->Init(point.x, point.y, point.w, point.h, &BasicImage, IDSEARCH(IDName));
		win->SetMovable(fMovable);
		win->SetCaptionRect(&captionRect);
		win->SetAlpha(alphaDepth);
		if(-1 != FUNCSEARCH(funcName))
			win->SetcbFunc(g_mt_func[FUNCSEARCH(funcName)]._func);

	BREAK
	CASE(WT_WANTREGISTDIALOG)
		CWantRegistDialog * win = (CWantRegistDialog*)wnd;
		win->SetValidXY(validXY.x,validXY.y);
		win->Init(point.x, point.y, point.w, point.h, &BasicImage, IDSEARCH(IDName));
		win->SetMovable(fMovable);
		win->SetCaptionRect(&captionRect);
		win->SetAlpha(alphaDepth);
		if(-1 != FUNCSEARCH(funcName))
			win->SetcbFunc(g_mt_func[FUNCSEARCH(funcName)]._func);

	BREAK
	CASE(WT_QUESTTOTALDIALOG)
		CQuestTotalDialog * win = (CQuestTotalDialog*)wnd;
		win->SetValidXY(validXY.x,validXY.y);
		win->Init(point.x, point.y, point.w, point.h, &BasicImage, IDSEARCH(IDName));
		win->SetMovable(fMovable);

		win->SetCaptionRect(&captionRect);
		win->SetAlpha(alphaDepth);
		win->SetActive(fActive);
		if(-1 != FUNCSEARCH(funcName))
			win->SetcbFunc(g_mt_func[FUNCSEARCH(funcName)]._func);
	BREAK
	CASE(WT_QUESTDIALOG)
		CQuestDialog * win = (CQuestDialog*)wnd;
		win->SetValidXY(validXY.x,validXY.y);
		win->Init(point.x, point.y, point.w, point.h, &BasicImage, IDSEARCH(IDName));
		win->SetMovable(fMovable);
		win->SetCaptionRect(&captionRect);
		win->SetAlpha(alphaDepth);
		win->SetIconCellBGImage(&selectedBGImage);
		win->SetDragOverBGImage(&dragoverBGImage);
		win->SetActive(fActive);
		if(-1 != FUNCSEARCH(funcName))
			win->SetcbFunc(g_mt_func[FUNCSEARCH(funcName)]._func);
	BREAK
	CASE(WT_WANTEDDIALOG)
		CWantedDialog * win = (CWantedDialog*)wnd;
		win->SetValidXY(validXY.x,validXY.y);
		win->Init(point.x, point.y, point.w, point.h, &BasicImage, IDSEARCH(IDName));
		win->SetMovable(fMovable);
		win->SetCaptionRect(&captionRect);
		win->SetAlpha(alphaDepth);
		if(-1 != FUNCSEARCH(funcName))
			win->SetcbFunc(g_mt_func[FUNCSEARCH(funcName)]._func);

	BREAK
	CASE(WT_JOURNALDIALOG)
		CJournalDialog * win = (CJournalDialog*)wnd;
		win->SetValidXY(validXY.x,validXY.y);
		win->Init(point.x, point.y, point.w, point.h, &BasicImage, IDSEARCH(IDName));
		win->SetMovable(fMovable);
		win->SetCaptionRect(&captionRect);
		win->SetAlpha(alphaDepth);
		if(-1 != FUNCSEARCH(funcName))
			win->SetcbFunc(g_mt_func[FUNCSEARCH(funcName)]._func);

	BREAK
	CASE(WT_PKLOOTINGDLG)
		CPKLootingDialog* win = (CPKLootingDialog*)wnd;
		win->SetValidXY(validXY.x,validXY.y);
		win->Init(point.x, point.y, point.w, point.h, &BasicImage, -1);
		win->SetMovable(fMovable);
		win->SetCaptionRect(&captionRect);
		win->SetAlpha(alphaDepth);
		if(-1 != FUNCSEARCH(funcName))
			win->SetcbFunc(g_mt_func[FUNCSEARCH(funcName)]._func);
	BREAK
	CASE(WT_MPREGISTDIALOG)
		CMPRegistDialog* win = (CMPRegistDialog*)wnd;
		win->SetValidXY(validXY.x,validXY.y);
		win->Init(point.x, point.y, point.w, point.h, &BasicImage, IDSEARCH(IDName));
		win->SetMovable(fMovable);
		win->SetCaptionRect(&captionRect);
		win->SetAlpha(alphaDepth);
		if(-1 != FUNCSEARCH(funcName))
			win->SetcbFunc(g_mt_func[FUNCSEARCH(funcName)]._func);
	BREAK
	CASE(WT_MPMISSIONDLG)
		CMPMissionDialog* win = (CMPMissionDialog*)wnd;
		win->SetValidXY(validXY.x,validXY.y);
		win->Init(point.x, point.y, point.w, point.h, &BasicImage, IDSEARCH(IDName));
		win->SetMovable(fMovable);
		win->SetCaptionRect(&captionRect);
		win->SetAlpha(alphaDepth);
		if(-1 != FUNCSEARCH(funcName))
			win->SetcbFunc(g_mt_func[FUNCSEARCH(funcName)]._func);
	BREAK
	CASE(WT_MPNOTICEDIALOG)
		CMPNoticeDialog* win = (CMPNoticeDialog*)wnd;
		win->SetValidXY(validXY.x,validXY.y);
		win->Init(point.x, point.y, point.w, point.h, &BasicImage, IDSEARCH(IDName));
		win->SetMovable(fMovable);
		win->SetCaptionRect(&captionRect);
		win->SetAlpha(alphaDepth);
		if(-1 != FUNCSEARCH(funcName))
			win->SetcbFunc(g_mt_func[FUNCSEARCH(funcName)]._func);
	BREAK
	CASE(WT_MPGUAGEDLG)
		CMPGuageDialog * win = (CMPGuageDialog*)wnd;
		win->SetValidXY(validXY.x,validXY.y);
		win->Init(point.x, point.y, point.w, point.h, &BasicImage, IDSEARCH(IDName));
		win->SetMovable(fMovable);
		win->SetCaptionRect(&captionRect);
		win->SetAlpha(alphaDepth);
		win->SetActive(fActive);
		if(-1 != FUNCSEARCH(funcName))
			win->SetcbFunc(g_mt_func[FUNCSEARCH(funcName)]._func);
	BREAK
	CASE(WT_PARTYCREATEDLG)
		CPartyCreateDlg * win = (CPartyCreateDlg*)wnd;
		win->SetValidXY(validXY.x,validXY.y);
		win->Init(point.x, point.y, point.w, point.h, &BasicImage, IDSEARCH(IDName));
		win->SetMovable(fMovable);
		win->SetCaptionRect(&captionRect);
		win->SetAlpha(alphaDepth);
		win->SetActive(fActive);
		if(-1 != FUNCSEARCH(funcName))
			win->SetcbFunc(g_mt_func[FUNCSEARCH(funcName)]._func);
	BREAK
	CASE(WT_PARTYINVITEDLG)
		CPartyInviteDlg * win = (CPartyInviteDlg*)wnd;
		win->SetValidXY(validXY.x,validXY.y);
		win->Init(point.x, point.y, point.w, point.h, &BasicImage, IDSEARCH(IDName));
		win->SetMovable(fMovable);
		win->SetCaptionRect(&captionRect);
		win->SetAlpha(alphaDepth);
		win->SetActive(fActive);
		if(-1 != FUNCSEARCH(funcName))
			win->SetcbFunc(g_mt_func[FUNCSEARCH(funcName)]._func);
	BREAK
	CASE(WT_GUILDCREATEDLG)
		CGuildCreateDialog * win = (CGuildCreateDialog*)wnd;
		win->SetValidXY(validXY.x,validXY.y);
		win->Init(point.x, point.y, point.w, point.h, &BasicImage, IDSEARCH(IDName));
		win->SetMovable(fMovable);
		win->SetCaptionRect(&captionRect);
		win->SetAlpha(alphaDepth);
		if(-1 != FUNCSEARCH(funcName))
			win->SetcbFunc(g_mt_func[FUNCSEARCH(funcName)]._func);		
	BREAK
	CASE(WT_GUILDDLG)
		CGuildDialog * win = (CGuildDialog*)wnd;
		win->SetValidXY(validXY.x,validXY.y);
		win->Init(point.x, point.y, point.w, point.h, &BasicImage, IDSEARCH(IDName));
		win->SetMovable(fMovable);
		win->SetCaptionRect(&captionRect);
		win->SetAlpha(alphaDepth);
		win->SetActive(fActive);
		if(-1 != FUNCSEARCH(funcName))
			win->SetcbFunc(g_mt_func[FUNCSEARCH(funcName)]._func);		
	BREAK
	CASE(WT_GUILDINVITEDLG)
		CGuildInviteDialog * win = (CGuildInviteDialog *)wnd;
		win->SetValidXY(validXY.x,validXY.y);
		win->Init(point.x, point.y, point.w, point.h, &BasicImage, IDSEARCH(IDName));
		win->SetMovable(fMovable);
		win->SetCaptionRect(&captionRect);
		win->SetAlpha(alphaDepth);
		win->SetActive(fActive);
		if(-1 != FUNCSEARCH(funcName))
			win->SetcbFunc(g_mt_func[FUNCSEARCH(funcName)]._func);		
	BREAK
	CASE(WT_GUILDMARKDLG)
		CGuildMarkDialog * win = (CGuildMarkDialog *)wnd;
		win->SetValidXY(validXY.x,validXY.y);
		win->Init(point.x, point.y, point.w, point.h, &BasicImage, IDSEARCH(IDName));
		win->SetMovable(fMovable);
		win->SetCaptionRect(&captionRect);
		win->SetAlpha(alphaDepth);
		if(-1 != FUNCSEARCH(funcName))
			win->SetcbFunc(g_mt_func[FUNCSEARCH(funcName)]._func);		
	BREAK
	CASE(WT_GUILDLEVELUPDLG)
		CGuildLevelUpDialog * win = (CGuildLevelUpDialog *)wnd;
		win->SetValidXY(validXY.x,validXY.y);
		win->Init(point.x, point.y, point.w, point.h, &BasicImage, IDSEARCH(IDName));
		win->SetMovable(fMovable);
		win->SetCaptionRect(&captionRect);
		win->SetAlpha(alphaDepth);
		win->SetActive(fActive);
		if(-1 != FUNCSEARCH(funcName))
			win->SetcbFunc(g_mt_func[FUNCSEARCH(funcName)]._func);		
	BREAK
	CASE(WT_GUILDNICKNAMEDLG)
		CGuildNickNameDialog * win = (CGuildNickNameDialog *)wnd;
		win->SetValidXY(validXY.x,validXY.y);
		win->Init(point.x, point.y, point.w, point.h, &BasicImage, IDSEARCH(IDName));
		win->SetMovable(fMovable);
		win->SetCaptionRect(&captionRect);
		win->SetAlpha(alphaDepth);
		if(-1 != FUNCSEARCH(funcName))
			win->SetcbFunc(g_mt_func[FUNCSEARCH(funcName)]._func);		
	BREAK
	CASE(WT_GUILDRANKDLG)
		CGuildRankDialog * win = (CGuildRankDialog *)wnd;
		win->SetValidXY(validXY.x,validXY.y);
		win->Init(point.x, point.y, point.w, point.h, &BasicImage, IDSEARCH(IDName));
		win->SetMovable(fMovable);
		win->SetCaptionRect(&captionRect);
		win->SetAlpha(alphaDepth);
		win->SetActive(fActive);
		if(-1 != FUNCSEARCH(funcName))
			win->SetcbFunc(g_mt_func[FUNCSEARCH(funcName)]._func);		
	BREAK
	CASE(WT_GUILDWAREHOUSEDLG)
		CGuildWarehouseDialog * win = (CGuildWarehouseDialog *)wnd;
		win->SetValidXY(validXY.x,validXY.y);
		win->Init(point.x, point.y, point.w, point.h, &BasicImage, IDSEARCH(IDName));
		win->SetMovable(fMovable);
		win->SetCaptionRect(&captionRect);
		win->SetAlpha(alphaDepth);
		win->SetActive(fActive);
		if(-1 != FUNCSEARCH(funcName))
			win->SetcbFunc(g_mt_func[FUNCSEARCH(funcName)]._func);
	BREAK
	CASE(WT_GUILDUNIONCREATEDLG)
		CGuildUnionCreateDialog * win = (CGuildUnionCreateDialog *)wnd;
		win->SetValidXY(validXY.x,validXY.y);
		win->Init(point.x, point.y, point.w, point.h, &BasicImage, IDSEARCH(IDName));
		win->SetMovable(fMovable);
		win->SetCaptionRect(&captionRect);
		win->SetAlpha(alphaDepth);
		win->SetActive(fActive);
		if(-1 != FUNCSEARCH(funcName))
			win->SetcbFunc(g_mt_func[FUNCSEARCH(funcName)]._func);
	BREAK
	CASE(WT_ITEMSHOPDIALOG)
		CItemShopDialog * win = (CItemShopDialog*)wnd;
		win->SetValidXY(validXY.x,validXY.y);
		win->Init(point.x, point.y, point.w, point.h, &BasicImage, IDSEARCH(IDName));
		win->SetMovable(fMovable);
		win->SetCaptionRect(&captionRect);
		win->SetAlpha(alphaDepth);
		if(-1 != FUNCSEARCH(funcName))
			win->SetcbFunc(g_mt_func[FUNCSEARCH(funcName)]._func);
	BREAK
	CASE(WT_MOVEDIALOG)
		CMoveDialog * win = (CMoveDialog*)wnd;
		win->SetValidXY(validXY.x,validXY.y);
		win->Init(point.x, point.y, point.w, point.h, &BasicImage, IDSEARCH(IDName));		
//		win->InitListCtrl(cols, rows);
		win->SetMovable(fMovable);
		win->SetCaptionRect(&captionRect);
		win->SetAlpha(alphaDepth);
		if(-1 != FUNCSEARCH(funcName))
			win->SetcbFunc(g_mt_func[FUNCSEARCH(funcName)]._func);
	BREAK
// guildfieldwar-------------------------------------------------------
	CASE(WT_GUILDFIELDWAR_DECLARE_DLG)
		CGFWarDeclareDlg* win = (CGFWarDeclareDlg*)wnd;
		win->SetValidXY(validXY.x,validXY.y);
		win->Init(point.x, point.y, point.w, point.h, &BasicImage, IDSEARCH(IDName));		
		win->SetMovable(fMovable);
		win->SetCaptionRect(&captionRect);
		win->SetAlpha(alphaDepth);
		if(-1 != FUNCSEARCH(funcName))
			win->SetcbFunc(g_mt_func[FUNCSEARCH(funcName)]._func);
	BREAK
	CASE(WT_GUILDFIELDWAR_RESULT_DLG)
		CGFWarResultDlg* win = (CGFWarResultDlg*)wnd;
		win->SetValidXY(validXY.x,validXY.y);
		win->Init(point.x, point.y, point.w, point.h, &BasicImage, IDSEARCH(IDName));		
		win->SetMovable(fMovable);
		win->SetCaptionRect(&captionRect);
		win->SetAlpha(alphaDepth);
		if(-1 != FUNCSEARCH(funcName))
			win->SetcbFunc(g_mt_func[FUNCSEARCH(funcName)]._func);
	BREAK
	CASE(WT_GUILDFIELDWAR_INFO_DLG)
		CGFWarInfoDlg* win = (CGFWarInfoDlg*)wnd;
		win->SetValidXY(validXY.x,validXY.y);
		win->Init(point.x, point.y, point.w, point.h, &BasicImage, IDSEARCH(IDName));		
		win->SetMovable(fMovable);
		win->SetCaptionRect(&captionRect);
		win->SetAlpha(alphaDepth);
		if(-1 != FUNCSEARCH(funcName))
			win->SetcbFunc(g_mt_func[FUNCSEARCH(funcName)]._func);
	BREAK
	CASE(WT_GUILDWAR_INFO_DLG)
		CGuildWarInfoDlg* win = (CGuildWarInfoDlg*)wnd;
		win->SetValidXY(validXY.x,validXY.y);
		win->Init(point.x, point.y, point.w, point.h, &BasicImage, IDSEARCH(IDName));		
		win->SetMovable(fMovable);
		win->SetCaptionRect(&captionRect);
		win->SetAlpha(alphaDepth);
		if(-1 != FUNCSEARCH(funcName))
			win->SetcbFunc(g_mt_func[FUNCSEARCH(funcName)]._func);
	BREAK
//---------------------------------------------------------------------
	CASE(WT_SKPOINTDIALOG)
		CSkillPointRedist * win = (CSkillPointRedist*)wnd;
		win->SetValidXY(validXY.x,validXY.y);
		win->Init(point.x, point.y, point.w, point.h, &BasicImage, IDSEARCH(IDName));
		win->SetMovable(fMovable);
		win->SetCaptionRect(&captionRect);
		win->SetAlpha(alphaDepth);
		if(-1 != FUNCSEARCH(funcName))
			win->SetcbFunc(g_mt_func[FUNCSEARCH(funcName)]._func);
	BREAK
	CASE(WT_SKPOINTNOTIFYDIALOG)
		CSkillPointNotify * win = (CSkillPointNotify*)wnd;
		win->SetValidXY(validXY.x,validXY.y);
		win->Init(point.x, point.y, point.w, point.h, &BasicImage, IDSEARCH(IDName));
		win->SetMovable(fMovable);
		win->SetCaptionRect(&captionRect);
		win->SetAlpha(alphaDepth);
		if(-1 != FUNCSEARCH(funcName))
			win->SetcbFunc(g_mt_func[FUNCSEARCH(funcName)]._func);
	BREAK
// partywar-----------------------
	CASE(WT_PARTYWAR_DLG)
		CPartyWarDialog* win = (CPartyWarDialog*)wnd;
		win->SetValidXY(validXY.x,validXY.y);
		win->Init(point.x, point.y, point.w, point.h, &BasicImage, IDSEARCH(IDName));		
		win->SetMovable(fMovable);
		win->SetCaptionRect(&captionRect);
		win->SetAlpha(alphaDepth);
		if(-1 != FUNCSEARCH(funcName))
			win->SetcbFunc(g_mt_func[FUNCSEARCH(funcName)]._func);
	BREAK		
	CASE(WT_SHOUT_DLG)
		CShoutDialog* win = (CShoutDialog*)wnd;
		win->SetValidXY(validXY.x,validXY.y);
		win->Init(point.x, point.y, point.w, point.h, &BasicImage, IDSEARCH(IDName));		
		win->SetMovable(fMovable);
		win->SetCaptionRect(&captionRect);
		win->SetAlpha(alphaDepth);
		if(-1 != FUNCSEARCH(funcName))
			win->SetcbFunc(g_mt_func[FUNCSEARCH(funcName)]._func);
	BREAK		
	CASE(WT_SHOUTCHAT_DLG)
		CShoutchatDialog* win = (CShoutchatDialog*)wnd;
		win->SetValidXY(validXY.x,validXY.y);
		win->Init(point.x, point.y, point.w, point.h, &BasicImage, IDSEARCH(IDName));		
		win->SetMovable(fMovable);
		win->SetCaptionRect(&captionRect);
		win->SetAlpha(alphaDepth);
		if(-1 != FUNCSEARCH(funcName))
			win->SetcbFunc(g_mt_func[FUNCSEARCH(funcName)]._func);
	BREAK		
	CASE(WT_CHASE_DLG)
		CChaseDialog* win = (CChaseDialog*)wnd;
		win->SetValidXY(validXY.x,validXY.y);
		win->Init(point.x, point.y, point.w, point.h, &BasicImage, IDSEARCH(IDName));		
		win->SetMovable(fMovable);
		win->SetCaptionRect(&captionRect);
		win->SetAlpha(alphaDepth);
		if(-1 != FUNCSEARCH(funcName))
			win->SetcbFunc(g_mt_func[FUNCSEARCH(funcName)]._func);
	BREAK		
	CASE(WT_CHASEINPUT_DLG)
		CChaseinputDialog* win = (CChaseinputDialog*)wnd;
		win->SetValidXY(validXY.x,validXY.y);
		win->Init(point.x, point.y, point.w, point.h, &BasicImage, IDSEARCH(IDName));		
		win->SetMovable(fMovable);
		win->SetCaptionRect(&captionRect);
		win->SetAlpha(alphaDepth);
		if(-1 != FUNCSEARCH(funcName))
			win->SetcbFunc(g_mt_func[FUNCSEARCH(funcName)]._func);
	BREAK		
	CASE(WT_NAMECHANGE_DLG)
		CNameChangeDialog* win = (CNameChangeDialog*)wnd;
		win->SetValidXY(validXY.x,validXY.y);
		win->Init(point.x, point.y, point.w, point.h, &BasicImage, IDSEARCH(IDName));		
		win->SetMovable(fMovable);
		win->SetCaptionRect(&captionRect);
		win->SetAlpha(alphaDepth);
		if(-1 != FUNCSEARCH(funcName))
			win->SetcbFunc(g_mt_func[FUNCSEARCH(funcName)]._func);
	BREAK		
	CASE(WT_NAMECHANGENOTIFY_DLG)
		CNameChangeNotifyDlg* win = (CNameChangeNotifyDlg*)wnd;
		win->SetValidXY(validXY.x,validXY.y);
		win->Init(point.x, point.y, point.w, point.h, &BasicImage, IDSEARCH(IDName));		
		win->SetMovable(fMovable);
		win->SetCaptionRect(&captionRect);
		win->SetAlpha(alphaDepth);
		if(-1 != FUNCSEARCH(funcName))
			win->SetcbFunc(g_mt_func[FUNCSEARCH(funcName)]._func);
	BREAK
	CASE(WT_GTREGIST_DLG)
		CGTRegistDialog* win = (CGTRegistDialog*)wnd;
		win->SetValidXY(validXY.x,validXY.y);
		win->Init(point.x, point.y, point.w, point.h, &BasicImage, IDSEARCH(IDName));		
		win->SetMovable(fMovable);
		win->SetCaptionRect(&captionRect);
		win->SetAlpha(alphaDepth);
		if(-1 != FUNCSEARCH(funcName))
			win->SetcbFunc(g_mt_func[FUNCSEARCH(funcName)]._func);
	BREAK
	CASE(WT_GTREGISTCANCEL_DLG)
		CGTRegistcancelDialog* win = (CGTRegistcancelDialog*)wnd;
		win->SetValidXY(validXY.x,validXY.y);
		win->Init(point.x, point.y, point.w, point.h, &BasicImage, IDSEARCH(IDName));		
		win->SetMovable(fMovable);
		win->SetCaptionRect(&captionRect);
		win->SetAlpha(alphaDepth);
		if(-1 != FUNCSEARCH(funcName))
			win->SetcbFunc(g_mt_func[FUNCSEARCH(funcName)]._func);
	BREAK
	CASE(WT_GTSTANDING_DLG)
		CGTStandingDialog* win = (CGTStandingDialog*)wnd;
		win->SetValidXY(validXY.x,validXY.y);
		win->Init(point.x, point.y, point.w, point.h, &BasicImage, IDSEARCH(IDName));		
		win->SetMovable(fMovable);
		win->SetCaptionRect(&captionRect);
		win->SetAlpha(alphaDepth);
		if(-1 != FUNCSEARCH(funcName))
			win->SetcbFunc(g_mt_func[FUNCSEARCH(funcName)]._func);
	BREAK
	CASE(WT_GTBATTLELIST_DLG)
		CGTBattleListDialog* win = (CGTBattleListDialog*)wnd;
		win->SetValidXY(validXY.x,validXY.y);
		win->Init(point.x, point.y, point.w, point.h, &BasicImage, IDSEARCH(IDName));		
		win->SetMovable(fMovable);
		win->SetCaptionRect(&captionRect);
		win->SetAlpha(alphaDepth);
		if(-1 != FUNCSEARCH(funcName))
			win->SetcbFunc(g_mt_func[FUNCSEARCH(funcName)]._func);
	BREAK
	CASE(WT_GTSCOREINFO_DLG)
		CGTScoreInfoDialog* win = (CGTScoreInfoDialog*)wnd;
		win->SetValidXY(validXY.x,validXY.y);
		win->Init(point.x, point.y, point.w, point.h, &BasicImage, IDSEARCH(IDName));		
		win->SetMovable(fMovable);
		win->SetCaptionRect(&captionRect);
		win->SetAlpha(alphaDepth);
		if(-1 != FUNCSEARCH(funcName))
			win->SetcbFunc(g_mt_func[FUNCSEARCH(funcName)]._func);
	BREAK
	CASE(WT_SEIGEWAR_TIMEREG_DLG)
		CSWTimeRegDlg* win = (CSWTimeRegDlg*)wnd;
		win->SetValidXY(validXY.x,validXY.y);
		win->Init(point.x, point.y, point.w, point.h, &BasicImage, IDSEARCH(IDName));		
		win->SetMovable(fMovable);
		win->SetCaptionRect(&captionRect);
		win->SetAlpha(alphaDepth);
		if(-1 != FUNCSEARCH(funcName))
			win->SetcbFunc(g_mt_func[FUNCSEARCH(funcName)]._func);
	BREAK
	CASE(WT_SEIGEWAR_PROTECTREG_DLG)
		CSWProtectRegDlg* win = (CSWProtectRegDlg*)wnd;
		win->SetValidXY(validXY.x,validXY.y);
		win->Init(point.x, point.y, point.w, point.h, &BasicImage, IDSEARCH(IDName));		
		win->SetMovable(fMovable);
		win->SetCaptionRect(&captionRect);
		win->SetAlpha(alphaDepth);
		if(-1 != FUNCSEARCH(funcName))
			win->SetcbFunc(g_mt_func[FUNCSEARCH(funcName)]._func);
	BREAK
	CASE(WT_SEIGEWAR_INFO_DLG)
		CSWInfoDlg* win = (CSWInfoDlg*)wnd;
		win->SetValidXY(validXY.x,validXY.y);
		win->Init(point.x, point.y, point.w, point.h, &BasicImage, IDSEARCH(IDName));		
		win->SetMovable(fMovable);
		win->SetCaptionRect(&captionRect);
		win->SetAlpha(alphaDepth);
		if(-1 != FUNCSEARCH(funcName))
			win->SetcbFunc(g_mt_func[FUNCSEARCH(funcName)]._func);
	BREAK
	CASE(WT_CHARCHANGE_DLG)
		CCharChangeDlg* win = (CCharChangeDlg*)wnd;
		win->SetValidXY(validXY.x,validXY.y);
		win->Init(point.x, point.y, point.w, point.h, &BasicImage, IDSEARCH(IDName));		
		win->SetMovable(fMovable);
		win->SetCaptionRect(&captionRect);
		win->SetAlpha(alphaDepth);
		if(-1 != FUNCSEARCH(funcName))
			win->SetcbFunc(g_mt_func[FUNCSEARCH(funcName)]._func);
	BREAK
	CASE(WT_ITEM_SEAL_DLG)
		CSealDialog* win = (CSealDialog*)wnd;
		win->SetValidXY(validXY.x,validXY.y);
		win->Init(point.x, point.y, point.w, point.h, &BasicImage, IDSEARCH(IDName));		
		win->SetMovable(fMovable);
		win->SetCaptionRect(&captionRect);
		win->SetAlpha(alphaDepth);
		if(-1 != FUNCSEARCH(funcName))
			win->SetcbFunc(g_mt_func[FUNCSEARCH(funcName)]._func);
	BREAK	
	CASE(WT_SEIGEWAR_PROFIT_DLG)
		CSWProfitDlg* win = (CSWProfitDlg*)wnd;
		win->SetValidXY(validXY.x,validXY.y);
		win->Init(point.x, point.y, point.w, point.h, &BasicImage, IDSEARCH(IDName));		
		win->SetMovable(fMovable);
		win->SetCaptionRect(&captionRect);
		win->SetAlpha(alphaDepth);
		if(-1 != FUNCSEARCH(funcName))
			win->SetcbFunc(g_mt_func[FUNCSEARCH(funcName)]._func);
	BREAK
	CASE(WT_SIEGEWAR_ENGRAVE_DLG)
		CSWEngraveDialog* win = (CSWEngraveDialog*)wnd;
		win->SetValidXY(validXY.x,validXY.y);
		win->Init(point.x, point.y, point.w, point.h, &BasicImage, IDSEARCH(IDName));		
		win->SetMovable(fMovable);
		win->SetCaptionRect(&captionRect);
		win->SetAlpha(alphaDepth);
		if(-1 != FUNCSEARCH(funcName))
			win->SetcbFunc(g_mt_func[FUNCSEARCH(funcName)]._func);
	BREAK
	CASE(WT_SIEGEWAR_TIME_DLG)
		CSWTimeDialog* win = (CSWTimeDialog*)wnd;
		win->SetValidXY(validXY.x,validXY.y);
		win->Init(point.x, point.y, point.w, point.h, &BasicImage, IDSEARCH(IDName));		
		win->SetMovable(fMovable);
		win->SetCaptionRect(&captionRect);
		win->SetAlpha(alphaDepth);
		if(-1 != FUNCSEARCH(funcName))
			win->SetcbFunc(g_mt_func[FUNCSEARCH(funcName)]._func);
	BREAK		
	CASE(WT_ITEM_CHANGEJOB_DLG)
		CChangeJobDialog* win = (CChangeJobDialog*)wnd;
		win->SetValidXY(validXY.x,validXY.y);
		win->Init(point.x, point.y, point.w, point.h, &BasicImage, IDSEARCH(IDName));		
		win->SetMovable(fMovable);
		win->SetCaptionRect(&captionRect);
		win->SetAlpha(alphaDepth);
		if(-1 != FUNCSEARCH(funcName))
			win->SetcbFunc(g_mt_func[FUNCSEARCH(funcName)]._func);
	BREAK	
	CASE(WT_SW_STARTTIME_DLG)
		CSWStartTimeDialog* win = (CSWStartTimeDialog*)wnd;
		win->SetValidXY(validXY.x,validXY.y);
		win->Init(point.x, point.y, point.w, point.h, &BasicImage, IDSEARCH(IDName));		
		win->SetMovable(fMovable);
		win->SetCaptionRect(&captionRect);
		win->SetAlpha(alphaDepth);
		if(-1 != FUNCSEARCH(funcName))
			win->SetcbFunc(g_mt_func[FUNCSEARCH(funcName)]._func);
	BREAK
	CASE(WT_ITEM_REINFORCERESET_DLG)
		CReinforceResetDlg* win = (CReinforceResetDlg*)wnd;
		win->SetValidXY(validXY.x,validXY.y);
		win->Init(point.x, point.y, point.w, point.h, &BasicImage, IDSEARCH(IDName));		
		win->SetMovable(fMovable);
		win->SetCaptionRect(&captionRect);
		win->SetAlpha(alphaDepth);
		if(-1 != FUNCSEARCH(funcName))
			win->SetcbFunc(g_mt_func[FUNCSEARCH(funcName)]._func);
	BREAK
	CASE(WT_ITEM_RARECREATE_DLG)
		CRareCreateDialog* win = (CRareCreateDialog*)wnd;
		win->SetValidXY(validXY.x,validXY.y);
		win->Init(point.x, point.y, point.w, point.h, &BasicImage, IDSEARCH(IDName));		
		win->SetMovable(fMovable);
		win->SetCaptionRect(&captionRect);
		win->SetAlpha(alphaDepth);
		if(-1 != FUNCSEARCH(funcName))
			win->SetcbFunc(g_mt_func[FUNCSEARCH(funcName)]._func);
	BREAK
	CASE(WT_SCREENSHOT_DLG)
		CScreenShotDlg* win = (CScreenShotDlg*)wnd;
		win->SetValidXY(validXY.x,validXY.y);
		win->Init(point.x, point.y, point.w, point.h, &BasicImage, IDSEARCH(IDName));		
		win->SetMovable(fMovable);
		win->SetCaptionRect(&captionRect);
		win->SetAlpha(alphaDepth);
		if(-1 != FUNCSEARCH(funcName))
			win->SetcbFunc(g_mt_func[FUNCSEARCH(funcName)]._func);
	BREAK
	CASE(WT_INSDGRANKINFODIALOG)
		CInsDGRankInfoDialog* win = (CInsDGRankInfoDialog*)wnd;
		win->SetValidXY(validXY.x, validXY.y);
		win->Init(point.x, point.y, point.w, point.h, &BasicImage, IDSEARCH(IDName));
		win->SetMovable(fMovable);
		win->SetCaptionRect(&captionRect);
		win->SetAlpha(alphaDepth);
		if (-1 != FUNCSEARCH(funcName))
			win->SetcbFunc(g_mt_func[FUNCSEARCH(funcName)]._func);
	BREAK
	CASE(WT_GUILDMUNHADLG)
		CGuildMunhaDialog* win = (CGuildMunhaDialog*)wnd;
		win->SetValidXY(validXY.x,validXY.y);
		win->Init(point.x, point.y, point.w, point.h, &BasicImage, IDSEARCH(IDName));		
		win->SetMovable(fMovable);
		win->SetCaptionRect(&captionRect);
		win->SetAlpha(alphaDepth);
		if(-1 != FUNCSEARCH(funcName))
			win->SetcbFunc(g_mt_func[FUNCSEARCH(funcName)]._func);
	BREAK
	CASE(WT_SHOWCHARACTORLISTDLG) // the create info 2014-08-11
		CCharactorInfoDlg * win = (CCharactorInfoDlg*)wnd;
		win->SetValidXY(validXY.x,validXY.y);
		win->Init(point.x, point.y, point.w, point.h, &BasicImage, IDSEARCH(IDName));
		win->SetMovable(fMovable);
		win->SetCaptionRect(&captionRect);
		win->SetAlpha(alphaDepth);
		if(-1 != FUNCSEARCH(funcName))
			win->SetcbFunc(g_mt_func[FUNCSEARCH(funcName)]._func);
	BREAK
	CASE(WT_SHOWEQUIPDLG)       // the create equip 2014-08-11
		CQuipInfoDlg * win = (CQuipInfoDlg*)wnd;
		win->SetValidXY(validXY.x,validXY.y);
		win->Init(point.x, point.y, point.w, point.h, &BasicImage, IDSEARCH(IDName));
		win->SetMovable(fMovable);
		win->SetCaptionRect(&captionRect);
		win->SetAlpha(alphaDepth);
		if(-1 != FUNCSEARCH(funcName))
		win->SetcbFunc(g_mt_func[FUNCSEARCH(funcName)]._func);
	BREAK
	CASE(WT_GODLSHOP_DLG)  //ÔÚÏßÉÌ³Ç  by:ºúººÈý  QQ:112582793
		CGoldShopDialog* win = (CGoldShopDialog*)wnd;
		win->SetValidXY(validXY.x,validXY.y);
		win->Init(point.x, point.y, point.w, point.h, &BasicImage, IDSEARCH(IDName));		
		win->SetMovable(fMovable);
		win->SetCaptionRect(&captionRect);
		win->SetAlpha(alphaDepth);
		if(-1 != FUNCSEARCH(funcName))
			win->SetcbFunc(g_mt_func[FUNCSEARCH(funcName)]._func);
	BREAK;
	CASE(WT_ADDPOINT_DLG)  
		AdPointDlg* win = (AdPointDlg*)wnd;
		win->SetValidXY(validXY.x,validXY.y);
		win->Init(point.x, point.y, point.w, point.h, &BasicImage, IDSEARCH(IDName));		
		win->SetMovable(fMovable);
		win->SetCaptionRect(&captionRect);
		win->SetAlpha(alphaDepth);
		if(-1 != FUNCSEARCH(funcName))
			win->SetcbFunc(g_mt_func[FUNCSEARCH(funcName)]._func);
	BREAK;
	CASE(WT_JACFLASH_DLG)  
		CJackFlashMsg* win = (CJackFlashMsg*)wnd;
		win->SetValidXY(validXY.x,validXY.y);
		win->Init(point.x, point.y, point.w, point.h, &BasicImage, IDSEARCH(IDName));		
		win->SetMovable(fMovable);
		win->SetCaptionRect(&captionRect);
		win->SetAlpha(alphaDepth);
		if(-1 != FUNCSEARCH(funcName))
			win->SetcbFunc(g_mt_func[FUNCSEARCH(funcName)]._func);
	BREAK;
	CASE(WT_HIDEBUFF_DLG)  
		CHideBuff* win = (CHideBuff*)wnd;
		win->SetValidXY(validXY.x,validXY.y);
		win->Init(point.x, point.y, point.w, point.h, &BasicImage, IDSEARCH(IDName));		
		win->SetMovable(fMovable);
		win->SetCaptionRect(&captionRect);
		win->SetAlpha(alphaDepth);
		if(-1 != FUNCSEARCH(funcName))
			win->SetcbFunc(g_mt_func[FUNCSEARCH(funcName)]._func);
	BREAK;
	CASE(WT_VIDICON_DLG)  
		CVideoCaptureIcon* win = (CVideoCaptureIcon*)wnd;
		win->SetValidXY(validXY.x,validXY.y);
		win->Init(point.x, point.y, point.w, point.h, &BasicImage, IDSEARCH(IDName));		
		win->SetMovable(fMovable);
		win->SetCaptionRect(&captionRect);
		win->SetAlpha(alphaDepth);
		if(-1 != FUNCSEARCH(funcName))
			win->SetcbFunc(g_mt_func[FUNCSEARCH(funcName)]._func);
	BREAK;
	CASE(WT_RECONNECT_DLG)	
		CReconnectDialog* win = (CReconnectDialog*)wnd;
		win->SetValidXY(validXY.x,validXY.y);
		win->Init(point.x, point.y, point.w, point.h, &BasicImage, IDSEARCH(IDName));		
		win->SetMovable(fMovable);
		win->SetCaptionRect(&captionRect);
		win->SetAlpha(alphaDepth);
		win->SetAlwaysTop(TRUE);
		if(-1 != FUNCSEARCH(funcName))
			win->SetcbFunc(g_mt_func[FUNCSEARCH(funcName)]._func);
	BREAK;
	CASE(WT_SAFENUM_IMG)	
		CSafeNumberImg* win = (CSafeNumberImg*)wnd;
		win->SetValidXY(validXY.x,validXY.y);
		win->Init(point.x, point.y, point.w, point.h, &BasicImage, IDSEARCH(IDName));		
		win->SetMovable(fMovable);
		win->SetCaptionRect(&captionRect);
		win->SetAlpha(alphaDepth);
		//win->SetAlwaysTop(TRUE);
		if(-1 != FUNCSEARCH(funcName))
			win->SetcbFunc(g_mt_func[FUNCSEARCH(funcName)]._func);
	BREAK;
	CASE(WT_REG_DLG)	
		RegID* win = (RegID*)wnd;
		win->SetValidXY(validXY.x,validXY.y);
		win->Init(point.x, point.y, point.w, point.h, &BasicImage, IDSEARCH(IDName));		
		win->SetMovable(fMovable);
		win->SetCaptionRect(&captionRect);
		win->SetAlpha(alphaDepth);
		if(-1 != FUNCSEARCH(funcName))
			win->SetcbFunc(g_mt_func[FUNCSEARCH(funcName)]._func);
	BREAK;
	CASE(WT_REPWD_DLG)	
		RestPwd* win = (RestPwd*)wnd;
		win->SetValidXY(validXY.x,validXY.y);
		win->Init(point.x, point.y, point.w, point.h, &BasicImage, IDSEARCH(IDName));		
		win->SetMovable(fMovable);
		win->SetCaptionRect(&captionRect);
		win->SetAlpha(alphaDepth);
		if(-1 != FUNCSEARCH(funcName))
			win->SetcbFunc(g_mt_func[FUNCSEARCH(funcName)]._func);
	BREAK;
	CASE(WT_CHARRBTN_DLG)
		CharacterRBTN* win = (CharacterRBTN*)wnd;
		win->SetValidXY(validXY.x,validXY.y);
		win->Init(point.x, point.y, point.w, point.h, &BasicImage, IDSEARCH(IDName));		
		win->SetMovable(fMovable);
		win->SetCaptionRect(&captionRect);
		win->SetAlpha(alphaDepth);
		if(-1 != FUNCSEARCH(funcName))
			win->SetcbFunc(g_mt_func[FUNCSEARCH(funcName)]._func);
	BREAK;
	CASE(WT_LUCKYICON_DLG)
		CLuckyDrawIcon* win = (CLuckyDrawIcon*)wnd;
		win->SetValidXY(validXY.x,validXY.y);
		win->Init(point.x, point.y, point.w, point.h, &BasicImage, IDSEARCH(IDName));		
		win->SetMovable(fMovable);
		win->SetCaptionRect(&captionRect);
		win->SetAlpha(alphaDepth);
		if(-1 != FUNCSEARCH(funcName))
			win->SetcbFunc(g_mt_func[FUNCSEARCH(funcName)]._func);
	BREAK;
	CASE(WT_BLOODSTORM_DLG)
		CBloodStormIcon* win = (CBloodStormIcon*)wnd;
		win->SetValidXY(validXY.x,validXY.y);
		win->Init(point.x, point.y, point.w, point.h, &BasicImage, IDSEARCH(IDName));		
		win->SetMovable(fMovable);
		win->SetCaptionRect(&captionRect);
		win->SetAlpha(alphaDepth);
		if(-1 != FUNCSEARCH(funcName))
			win->SetcbFunc(g_mt_func[FUNCSEARCH(funcName)]._func);
	BREAK;
	CASE(WT_CHANGELOOK_DLG)
		cChangeLookDialog* win = (cChangeLookDialog*)wnd;
		win->SetValidXY(validXY.x, validXY.y);
		win->Init(point.x, point.y, point.w, point.h, &BasicImage, IDSEARCH(IDName));
		win->SetMovable(fMovable);
		win->SetCaptionRect(&captionRect);
		win->SetAlpha(alphaDepth);
		if (-1 != FUNCSEARCH(funcName))
			win->SetcbFunc(g_mt_func[FUNCSEARCH(funcName)]._func);
	BREAK;
	CASE(WT_WALLET_DLG)
		CMoneyPacketDialog* win = (CMoneyPacketDialog*)wnd;
		win->SetValidXY(validXY.x, validXY.y);
		win->Init(point.x, point.y, point.w, point.h, &BasicImage, IDSEARCH(IDName));
		win->SetMovable(fMovable);
		win->SetCaptionRect(&captionRect);
		win->SetAlpha(alphaDepth);
		if (-1 != FUNCSEARCH(funcName))
			win->SetcbFunc(g_mt_func[FUNCSEARCH(funcName)]._func);
	BREAK;
	CASE(WT_BROWSER)
		cBrowser * win = (cBrowser*)wnd;
		win->Init(point.x, point.y, point.w, point.h, NULL, IDSEARCH(IDName));
		win->SetMovable(fMovable);
		win->SetActive(fActive);
		if(-1 != FUNCSEARCH(funcName))
			win->SetcbFunc(g_mt_func[FUNCSEARCH(funcName)]._func);
	BREAK 
	

	EWINDOWTYPE	

	return wnd;
}

void cScriptManager::CheckPosForResolution( cPoint* pPos )
{
	int right = pPos->x + pPos->w;
	int bottom = pPos->y + pPos->h;

	int Wgap = right - LOWRSLTN_W;
	int Hgap = bottom - LOWRSLTN_H;

	if( Wgap > 0 )
	{
		pPos->x -= Wgap;
	}
	if( Hgap > 0 )
	{
		pPos->y -= Hgap;
	}

	if( pPos->x < 0 || Wgap > LOWRSLTN_W )
	{
		pPos->x = 0;
	}
	if( pPos->y < 0 || Hgap > LOWRSLTN_H )
	{
		pPos->y = 0;
	}
}

/////////////////// 2007. 5. 18. CBH - Parse Type °ü·Ã ÇÔ¼ö Ãß°¡ ///////////////
BOOL cScriptManager::InitParseTypeData()
{
	int nMaxNum = eMAX_PARSETYPE_NUM;

	m_ParseTypeTable.Initialize(nMaxNum); //ParseTypeÀúÀå Å×ÀÌºí ÃÊ±âÈ­
	
	CMHFile fp;
	PARSE_TYPE_INFO* pParseTypeInfo;

	if(!fp.Init("./image/ParseTypeList.bin", "rb"))
		return FALSE;
	
	while(!fp.IsEOF())
	{
		pParseTypeInfo = NULL;
		pParseTypeInfo = new PARSE_TYPE_INFO;

		pParseTypeInfo->nType = fp.GetInt();
		SafeStrCpy(pParseTypeInfo->cpTypeName, fp.GetString(), MAX_PARSE_TYPE_NAME_LENGTH+1 );		
		int nKeyValue = ChangeHashKey(pParseTypeInfo->cpTypeName, nMaxNum);
		pParseTypeInfo->nKeyValue = nKeyValue;
		if(nKeyValue < 0)
		{
			return FALSE;
		}
		m_ParseTypeTable.Add(pParseTypeInfo, nKeyValue);
	}
	
	return TRUE;
}

int cScriptManager::ChangeHashKey(const char* cpTypeName, int nMaxNum)
{
	int nKeyValue = -1;
	int nValue = 0;

	if(nMaxNum < 1)
	{
		return -1;
	}
	
	int nStrSize = strlen(cpTypeName);
	nKeyValue = ((cpTypeName[0] + cpTypeName[1] + nStrSize) * (cpTypeName[nStrSize] + cpTypeName[nStrSize-1])) % nMaxNum;

	if(nKeyValue > nMaxNum)
	{
		return -1;
	}

	return nKeyValue;
}

PARSE_TYPE cScriptManager::GetParseType(const char* str)
{
	int nMaxNum = eMAX_PARSETYPE_NUM;

	int nKeyValue = ChangeHashKey(str, nMaxNum);

	if(nKeyValue < 0)
	{
		return (PARSE_TYPE)-1;
	}		
	
	PARSE_TYPE_INFO* pParseTypeList = m_ParseTypeTable.GetData(nKeyValue);
	
	if(pParseTypeList == NULL)
	{
		return (PARSE_TYPE)-1;
	}	

	//°°Àº Å°°ªÀÇ Áßº¹ µÇ´Â µ¥ÀÌÅÍ Ã³¸®
	if(strcmp(pParseTypeList->cpTypeName, str) != 0)
	{
		PARSE_TYPE_INFO* pListTemp = NULL;
		pListTemp = pParseTypeList;
		m_ParseTypeTable.SetPositionHead();
		m_ParseTypeTable.StartGetMultiData(nKeyValue);

		while(strcmp(pListTemp->cpTypeName, str) != 0)
		{				
			pListTemp = m_ParseTypeTable.GetMultiData();

			if(pListTemp == NULL)
			{
				return (PARSE_TYPE)-1;
			}
		}
		pParseTypeList = pListTemp;		
	}

	return (PARSE_TYPE)pParseTypeList->nType;
}
///////////////////////////////////////////////////////////////////////////////////////





