#include "stdafx.h"
#include <tlhelp32.h> 
#include "ItemManager.h"
#include "DealDialog.h"
#include "ExchangeDialog.h"
#include "MixDialog.h"
#include <MMSystem.h>   
#include "./Interface/cIcon.h"
#include "./Interface/cImage.h"
#include "GameIn.h"
#include "WindowIDEnum.h"
#include "./Interface/cWindowManager.h"
#include "./Interface/cScriptManager.h"
#include "cDivideBox.h"
#include "ObjectManager.h"
#include "DebugDlg.h"
#include "AppearanceManager.h"
#include "GameDef.h"
#include "QuickManager.h"
#include "QuickItem.h"
#include "ExchangeItem.h"
#include "SkillManager_Client.h"
#include "ChatManager.h"
#include "./Audio/MHAudioManager.h"
#include "mhFile.h"
#include "GuildWarehouseDialog.h"
#include "InventoryExDialog.h"
#include "PyogukDialog.h"
#include "QuickDialog.h"
#include "ChatDialog.h"
#include "GameEventManager.h"
#include "UpgradeDlg.h"
#include "ReinforceDlg.h"
#include "MHMap.h"
#include "DissolutionDialog.h"
#include "MugongManager.h"
#include "ItemShopDialog.h"
#include "StatusIconDlg.h"
#include "MoveDialog.h"
#include "CharacterDialog.h"
#include "cMsgBox.h"
#include "ObjectStateManager.h"
#include "MoveManager.h"
#include "EventMapInfo.h"
#include "ChaseDialog.h"
#include "NameChangeDialog.h"
#include "ReviveDialog.h"
#include "CharChangeDlg.h"
#include "GMNotifyManager.h"
#include "SealDialog.h"
#include "SiegeWarMgr.h"
#include "ChangeJobDialog.h"
#include "ReinforceResetDlg.h"
#include "RareCreateDialog.h"
#include "ReinforceManager.h"
#include "../StatsCalcManager.h"
#include "TitanPartsMakeDlg.h"
#include "TitanMixDlg.h"
#include "TitanUpgradeDlg.h"
#include "TitanBreakDlg.h"
#include "TitanRepairDlg.h"
#include "TitanInventoryDlg.h"
#include "TitanRegisterDlg.h"
#include "TitanDissolutionDlg.h"
#include "TitanGuageDlg.h"
#include "PetManager.h"
#include "PetInventoryDlg.h"
#include "UniqueItemCurseCancellationDlg.h"
#include "UniqueItemMixDlg.h"
#include "SkinSelectDialog.h"
#include "CostumeSkinSelectDialog.h"
#include "StreetStallManager.h"
#include "FortWarDialog.h"
#include "FortWarManager.h"
#include "LuckerDialog.h"   
#include "OtherCharacterEquip.h" 
#include "OtherCharacterMoneyDlg.h" 
#include "OtherCharacterAmuletDlg.h" 
#include "ItemStoneDkDialog.h"   
#include "ItemStoneXqDialog.h"   
#include "ItemStepReinforceDialog.h"
#include "OfficialUpGradeDlg.h"
#include "ItemQualityDlg.h"
#include "ItemQualityChangeDlg.h"
#include "ItemLockManager.h"         
#include "ItemUnLockManager.h"       
#include "ItemGambleManager.h"       
//#include "SafeNumberDialog.h"         
#include "SafeNumberIcon.h"            
//#include "SafeNumberLockDialog.h"  
//#include "SafeNumberChangeDialog.h"  
#include "SendEquipSelectDialog.h"  
#include "MainbarDialog.h"
#include "GoldShopDialog.h"
#include "AdPointDlg.h"
#include "shlwapi.h"
#include "./Input/UserInput.h"
#include "famemanager.h"
#include "GameCheckManager.h"
#pragma comment(lib,"shlwapi.lib") 
#include "TitanManager.h"
#include "ErrorMsg.h"
#include <fstream>
char JACKJACK[64] = "                                               ";

#define PetDbIdx	NULL//117
GLOBALTON(CItemManager)
CItemManager::CItemManager()
{
	m_ItemHash.Initialize(200/*128*/);
	m_ItemInfoList.Initialize(MAX_ITEM_NUM);
	m_ItemLuckerList.Initialize(84);
	m_ItemGrowInfo =new ITEM_GROW_SET;
	m_GameCheckList.Initialize(30);
	p_GameCheckLastTime=0;  
	m_UnfitItemInfoListForHide.Initialize(100);
	m_AutoUseBuffItem.Initialize(100);

	m_EventMap.Initialize(100);
	m_EventMapItemBlock.Initialize(100);

	m_CostumeHairSet.Initialize(100);
	m_ItemToolTipTextList.Initialize(64);		
	m_ItemOptionList.Initialize(MAX_ITEM_OPTION_NUM);
	m_ItemStoneOptionList.Initialize(MAX_ITEM_OPTION_NUM); 
	m_ItemRareOptionList.Initialize(MAX_ITEM_OPTION_NUM);
	m_SetItemQualityOptionList.Initialize(500);
	m_UsedItemList.Initialize(32);
	m_IconIndexCreator.Init(IG_MUGONG_STARTINDEX-IG_ITEM_STARTINDEX, IG_ITEM_STARTINDEX);//m_IconIndexCreator.Init(MAX_ITEMICON_NUM, IG_ITEM_STARTINDEX);
	m_bAddPrice = FALSE;
	m_RareItemInfoTable.Initialize(500);  
	memset(&m_TempDeleteItem, 0, sizeof(ITEMBASE));
	m_SetItemOptionList.Initialize(MAX_SETITEM_KIND_NUM); 
	//m_StageLogoList.Initialize(100);
	m_nItemUseCount = 0;
	StoredPDPoint = 0;
	m_SShout = FALSE;

	ZeroMemory(HeroName, sizeof(HeroName));
	ZeroMemory(ItemOpened, sizeof(ItemOpened));
	ZeroMemory(ItemObtained, sizeof(ItemObtained));
#ifdef _OLDGOLDDIALOG_
	m_GoldShopInfoList.Initialize(896);
#endif
}
CItemManager::~CItemManager()
{
	Release();
	m_IconIndexCreator.Release();
	ITEM_INFO * pInfo = NULL;
	m_ItemInfoList.SetPositionHead();
	while(pInfo = m_ItemInfoList.GetData())
		delete pInfo;
	m_ItemInfoList.RemoveAll();
	ITEM_LUCKER_INFO * pLuckerInfo=NULL;
	m_ItemLuckerList.SetPositionHead();
	while(pLuckerInfo=m_ItemLuckerList.GetData())
	{
		delete pLuckerInfo;
	}
	m_ItemLuckerList.RemoveAll();
	m_ItemGrowInfo=NULL;
	GAMECHECK_LIST * pGameCheckList=NULL;
	m_GameCheckList.SetPositionHead();
	for(int i=0;i<m_GameCheckList.GetDataNum();i++)
	{
		pGameCheckList=m_GameCheckList.GetData();
		delete pGameCheckList;
	}
	m_GameCheckList.RemoveAll();
	m_UnfitItemInfoListForHide.RemoveAll();

	ITEM_INFO * pAutoUseBuffItem=NULL;

	m_AutoUseBuffItem.SetPositionHead();
	while(pAutoUseBuffItem=m_AutoUseBuffItem.GetData())
	{
		delete pAutoUseBuffItem;
	}
	m_AutoUseBuffItem.RemoveAll();
	ITEM_INFO * pEventMap = NULL;
	m_EventMap.SetPositionHead();
	while (pEventMap = m_EventMap.GetData())
	{
		delete pEventMap;
	}
	m_EventMap.RemoveAll();
	ITEM_INFO * pEventMapItemBlock = NULL;
	m_EventMapItemBlock.SetPositionHead();
	while (pEventMapItemBlock = m_EventMapItemBlock.GetData())
	{
		delete pEventMapItemBlock;
	}
	m_EventMapItemBlock.RemoveAll();

	m_CostumeHairSet.RemoveAll();
	TOOLTIP_TEXT* pToolTipText = NULL;
	TOOLTIP_TEXT* pTemp = NULL;
	m_ItemToolTipTextList.SetPositionHead();
	while( pToolTipText = m_ItemToolTipTextList.GetData() )
	{
		do
		{
			pTemp = pToolTipText->pNext;
			if( pToolTipText->strToolTip )
				delete[] pToolTipText->strToolTip;
			delete pToolTipText;
			pToolTipText = pTemp;
		} while( pToolTipText );
	}
	m_ItemToolTipTextList.RemoveAll();
	SHOPITEMBASE* pShopItemInfo = NULL;
	m_UsedItemList.SetPositionHead();
	while(pShopItemInfo = m_UsedItemList.GetData())
		delete pShopItemInfo;
	m_UsedItemList.RemoveAll();
	sRareItemInfo* pList = NULL;
	m_RareItemInfoTable.SetPositionHead();
	while( pList = m_RareItemInfoTable.GetData() )
	{
		delete pList;
		pList = NULL;
	}
	m_RareItemInfoTable.RemoveAll();
	SET_ITEM_OPTION* pSetItemOption = NULL;
	m_SetItemOptionList.SetPositionHead();
	while(pSetItemOption = m_SetItemOptionList.GetData())
		delete pSetItemOption;
	m_SetItemOptionList.RemoveAll();
	ITEM_STONE_OPTION_INFO * pStoneInfo=NULL;
	m_ItemStoneOptionList.SetPositionHead();
	while(pStoneInfo=m_ItemStoneOptionList.GetData())
	{
		delete pStoneInfo;
	}
	m_ItemStoneOptionList.RemoveAll();
	//m_MixItemInfo.clear();//kiv by jack 1644 06042022
	/*STAGELOGO * pStageLogo=NULL;
	m_StageLogoList.SetPositionHead();
	while(pStageLogo=m_StageLogoList.GetData())
	{
		delete pStageLogo;
	}
	m_StageLogoList.RemoveAll();*/
	m_ClearPacketInfoList.clear();
	//////////////////////////////////////////////////////////////
	SET_ITEMQUALITY_OPTION* pSetItemQualityOption = NULL;
	m_SetItemQualityOptionList.SetPositionHead();
	while (pSetItemQualityOption = m_SetItemQualityOptionList.GetData())
	{
		delete pSetItemQualityOption;
		pSetItemQualityOption = NULL;
	}
	m_SetItemQualityOptionList.RemoveAll();

	//////////////////////////////////////////////////////////////
#ifdef _OLDGOLDDIALOG_
	ITEM_GOLD_SHOP * pGoldShopInfo = NULL;
	m_GoldShopInfoList.SetPositionHead();
	while (pGoldShopInfo = m_GoldShopInfoList.GetData())
	{
		delete pGoldShopInfo;
	}
	m_GoldShopInfoList.RemoveAll();
#endif
}
#ifdef _OLDGOLDDIALOG_
void CItemManager::LoadGoldShopItem()
{
	CMHFile file;

#ifdef _GOLDDLGONEBTN_
	if (!file.Init("Resource/ItemGoldList_N1.bin", "rb"))
#else
	if (!file.Init("Resource/ItemGoldList.bin", "rb"))
#endif
		return;
	ITEM_GOLD_SHOP * pInfo = NULL;
	while (1)
	{
		if (file.IsEOF())
			break;
		ASSERT(!pInfo);
		pInfo = new ITEM_GOLD_SHOP;
		pInfo->TableIdx = file.GetWord();
		pInfo->Idx = file.GetWord();
		pInfo->ItemIdx = file.GetDword();
		pInfo->ItemGoldPrice = file.GetDword();
#ifndef _GOLDDLGONEBTN_
		pInfo->ItemMallPrice = file.GetDword();
		pInfo->ItemMoneyPrice = file.GetDword();
#endif
		pInfo->ItemCount = file.GetDword();
		ASSERT(m_GoldShopInfoList.GetData(pInfo->ItemIdx) == FALSE);
		m_GoldShopInfoList.Add(pInfo, pInfo->ItemIdx);
		pInfo = NULL;
	}
	file.Release();
}
ITEM_GOLD_SHOP * CItemManager::GetGoldShopItem(WORD ItemIdx)
{
	return m_GoldShopInfoList.GetData(ItemIdx);
}
ITEM_GOLD_SHOP * CItemManager::GetGoldShopItemByTableIdx(WORD TableIdx, WORD Idx)
{
	m_GoldShopInfoList.SetPositionHead();
	ITEM_GOLD_SHOP* pInfo = NULL;
	while (pInfo = m_GoldShopInfoList.GetData())
	{
		if (pInfo->TableIdx == TableIdx && pInfo->Idx == Idx)
		{
			return pInfo;
		}
	}
	return NULL;
}
ITEM_GOLD_SHOP * CItemManager::GetGoldShopItemByIdxAndItemIdx(WORD TableIdx, WORD ItemIdx, WORD Idx)
{
	m_GoldShopInfoList.SetPositionHead();
	ITEM_GOLD_SHOP* pInfo = NULL;
	while (pInfo = m_GoldShopInfoList.GetData())
	{
		if (pInfo->TableIdx == TableIdx && pInfo->ItemIdx == ItemIdx && pInfo->Idx == Idx)
		{
			return pInfo;
		}
	}
	return NULL;
}
DWORD CItemManager::GetGoldShopItemCount()
{
	return m_GoldShopInfoList.GetDataNum();
}

#endif
void CItemManager::LoadMixItemInfo()//kiv by jack 1644 06042022
{
	/*CMHFile file;
	if(file.Init("./Resource/MixItemInfo.bin","rb")==FALSE)
		return;
	char buff[64] = {0,};
	while(1)
	{
		if(file.IsEOF())
			break;
		WORD t_wTmp = file.GetWord();
		file.GetString(buff);
		m_MixItemInfo[t_wTmp] = buff;
	}
	file.Release();*/
}
#include "..\[CC]MixManager\MixManager.h"
BOOL CItemManager::CanMixItem(WORD wItemIndex)
{
	const ITEM_MIX_INFO * pMixItemInfo = MIXMGR->GetMixItemInfo(wItemIndex);
	if( pMixItemInfo )
		return true;
	else
		return false;

	//std::map<WORD,std::string>::iterator iter;//kiv by jack 1644 06042022
	//iter = m_MixItemInfo.find(wItemIndex);
	//if(iter!=m_MixItemInfo.end())
	//	return TRUE;
	//else
	//	return FALSE;
}
cImage * CItemManager::GetIconImage(WORD ItemIdx, cImage * pImageGet)
{
	SCRIPTMGR->GetImage( GetItemInfo(ItemIdx)->Image2DNum, pImageGet, PFT_ITEMPATH );	
	return pImageGet;
}
CItem* CItemManager::MakeNewItem(ITEMBASE* pBaseInfo, char* strKind)
{
	ITEM_INFO* pItemInfo = GetItemInfo(pBaseInfo->wIconIdx);
	if(pItemInfo == NULL)
	{
		return NULL;
	}
	if(pBaseInfo->dwDBIdx == 0)
		return NULL;
	CItem* pItem = NULL;
	if(!(pItem = GetItem(pBaseInfo->dwDBIdx)))
	{
		DWORD ItemID = m_IconIndexCreator.GenerateIndex();
		pItem = new CItem(pBaseInfo);
		m_ItemHash.Add(pItem,pItem->GetDBIdx());						
		cImage tmpIconImage;
		pItem->Init(0,0,DEFAULT_ICONSIZE,DEFAULT_ICONSIZE,
					GetIconImage(pItem->GetItemIdx(), &tmpIconImage),
					ItemID);
		pItem->SetIconType(eIconType_AllItem);
		pItem->SetData(pItem->GetItemIdx());
		pItem->SetLinkPosition( 0 );
		WINDOWMGR->AddWindow(pItem);
	}
	else
	{
	}
	return pItem;
}
void CItemManager::SetQuestItemToolTip(cIcon * pIcon, ITEM_INFO * pInfo, DWORD dwItemDBIdx, BOOL IsGoldShop)
{
	WORD index = pIcon->GetData();
	eITEM_KINDBIT bits = GetItemKind(index);

	char line[128];
	sprintf(line,CHATMGR->GetChatMsg(2174),CHATMGR->GetChatMsg(2217));  
	pIcon->AddToolTipLine(line,TTTC_ITEM_LEVEL);
	pIcon->AddToolTipLine("");
	pIcon->AddToolTipLine("");
	if( FALSE == IsPetSummonItem(index) )
		pIcon->AddToolTipLine( CHATMGR->GetChatMsg(260), TTTC_QUESTITEM );
	if( IsPetSummonItem(index) )
	{
		SetPetSummonItemToolTip(pIcon, pInfo, dwItemDBIdx, IsGoldShop);
	}
	AddItemDescriptionToolTip( pIcon, pInfo->ItemTooltipIdx );
}
void CItemManager::SetShopItemToolTipMall( cIcon * pIcon, ITEM_INFO * pInfo)
{
	char line[128];
	sprintf(line,CHATMGR->GetChatMsg(2176),CHATMGR->GetChatMsg(2217)); 
	pIcon->AddToolTipLine(line,TTTC_ITEM_LEVEL); 
	pIcon->AddToolTipLine(JACKJACK);

	if(pInfo->ItemKind == eSHOP_ITEM_PET)
	{
		pIcon->AddToolTipLine("");
		const ITEMBASE* pBaseInfo = ((CItem*)pIcon)->GetItemBaseInfo();
		if( IsPetSummonItem(pInfo->ItemIdx) )
		{
			SetPetSummonItemToolTip(pIcon,pInfo,PetDbIdx,TRUE);
		}
	}
	else
	{
		AddItemDescriptionToolTip( pIcon, pInfo->ItemTooltipIdx,RGB_HALF(82, 255, 204) );
		AddShopItemToolTip( (cIcon*)pIcon, pInfo );
	}
}
void CItemManager::SetShopItemToolTip( cIcon * pIcon, ITEM_INFO * pInfo,DWORD dwItemDBIdx)
{
	WORD index = pIcon->GetData();
	eITEM_KINDBIT bits = GetItemKind(index);

	DWORD dwColor = TTTC_DEFAULT;
	
	char line[128];
	char line2[64];
	char line3[64];

	const ITEMBASE* pBaseInfoStatic = ((CItem*)pIcon)->GetItemBaseInfo();
	if(pBaseInfoStatic->ItemStatic==ITEM_STATIC_PD)
	{
		sprintf(line2,CHATMGR->GetChatMsg(2582));
		wsprintf( line3, "                                   %s",line2);
		sprintf(line,CHATMGR->GetChatMsg(2176),CHATMGR->GetChatMsg(2217));
		pIcon->AddToolTipLine2(line,line3,TTTC_ITEM_LEVEL,TTCLR_ENERMY);
	}
	else
	{
		sprintf(line,CHATMGR->GetChatMsg(2176),CHATMGR->GetChatMsg(2217));
		pIcon->AddToolTipLine(line,TTTC_ITEM_LEVEL);
	}
	pIcon->AddToolTipLine( "" );
	if( pInfo->ItemKind == eSHOP_ITEM_INCANTATION && pInfo->LimitJob )
	{				
		if( pInfo->LimitGender == 0 && HERO->GetLevel() > 50 )
		{
			dwColor = TTCLR_ENERMY;
		}
		else if( pInfo->LimitGender == 1 && (HERO->GetLevel() <51 || HERO->GetLevel() > 70 ) )
		{
			dwColor = TTCLR_ENERMY;
		}
		else if( pInfo->LimitGender == 2 && (HERO->GetLevel() <71 || HERO->GetLevel() > 90 ) )
		{
			dwColor = TTCLR_ENERMY;
		}
	}
	else if( HERO->GetLevel() < pInfo->NaeRyukRecoverRate )
	{
		dwColor = TTTC_LIMIT;
	}
	if( pInfo->NaeRyukRecoverRate != 0 )
	{
		pIcon->AddToolTipLine("");
		sprintf(line, CHATMGR->GetChatMsg(238), (int)pInfo->NaeRyukRecoverRate);
		if( HERO->GetCharacterTotalInfo()->Level < pInfo->NaeRyukRecoverRate )
			pIcon->AddToolTipLine( line, TTCLR_ENERMY );
		else
			pIcon->AddToolTipLine( line, TTTC_FREELIMIT );
	}
	
	if( pIcon->GetType() == WT_EXCHANGEITEM || pIcon->GetType() == WT_STALLITEM || pInfo->ItemKind == eSHOP_ITEM_PET || pIcon->GetType() == WT_BUYITE)
	{
		pIcon->AddToolTipLine("");
		pIcon->AddToolTipLine("");
		CExchangeItem* pExItem = (CExchangeItem*)pIcon;
		if( pExItem->GetItemParam() & ITEM_PARAM_SEAL )
		{			
			if( IsPetSummonItem(index) )
			{
				SetPetSummonItemToolTip(pIcon,pInfo,dwItemDBIdx);
			}
			AddItemDescriptionToolTip( pIcon, pInfo->ItemTooltipIdx );
		}
		else
		{
			const ITEMBASE* pBaseInfo = ((CItem*)pIcon)->GetItemBaseInfo();
			if( IsPetSummonItem(index) )
			{
				SetPetSummonItemToolTip(pIcon,pInfo,dwItemDBIdx);
			}
			AddItemDescriptionToolTip( pIcon, pInfo->ItemTooltipIdx );
		}
		return;
	}
	//
	CItem* pItem = (CItem*)pIcon;
	if( pInfo->ItemKind == eSHOP_ITEM_MAKEUP || pInfo->ItemKind == eSHOP_ITEM_DECORATION )
	{
		if( !(pItem->GetItemParam() & ITEM_PARAM_SEAL) )
		{
			SHOPITEMBASE* pShopItemBase = m_UsedItemList.GetData( pItem->GetItemIdx() );
			if( pShopItemBase ) 
				AddUsedAvatarItemToolTip( pShopItemBase );
			return;
		}				
	}
	if( pInfo->ItemKind == eSHOP_ITEM_EQUIP )
	{
		if( !(pItem->GetItemParam() & ITEM_PARAM_SEAL) && pIcon->GetType()==WT_DEALITEM)
		{
			SHOPITEMBASE* pShopItemBase = m_UsedItemList.GetData( pItem->GetItemIdx() );
			if( pShopItemBase )
				AddUsedShopEquipItemToolTip( pShopItemBase );
			return;
		}
	}
	if( pInfo->ItemKind == eSHOP_ITEM_PET_EQUIP )
	{
		if( !(pItem->GetItemParam() & ITEM_PARAM_SEAL) )
		{
			SHOPITEMBASE* pShopItemBase = m_UsedItemList.GetData( pItem->GetItemIdx() );
			if( pShopItemBase )
				AddUsedPetEquipItemToolTip( pShopItemBase );
			return;
		}
	}
	pIcon->AddToolTipLine(JACKJACK);
	if( pIcon->GetType()==WT_ITEM && (pItem->GetItemParam() & ITEM_PARAM_SEAL) )
	{
		AddItemDescriptionToolTip( pIcon, 6000,RGB_HALF(82, 255, 204) );
		AddShopItemToolTip( (cIcon*)pItem, pInfo );
		PrintShopItemUseTime( pItem, pInfo );
	}
	else
	{
		AddItemDescriptionToolTip( pIcon, pInfo->ItemTooltipIdx );
		AddShopItemToolTip( (cIcon*)pItem, pInfo );                
		if( pInfo->ItemType == 11 )
		{				
			SHOPITEMBASE* pShopItemBase = m_UsedItemList.GetData( pItem->GetItemIdx() );
			if( pShopItemBase )
			{
				if( pInfo->SellPrice == eShopItemUseParam_Realtime )
				{
					pItem->AddToolTipLine( CHATMGR->GetChatMsg(766), TTTC_DEFAULT );
					stTIME time;
					char buf[128] = { 0, };
					time.value = pShopItemBase->Remaintime;
					wsprintf( buf, CHATMGR->GetChatMsg(767), time.GetYear()+15, time.GetMonth(), time.GetDay(), time.GetHour(), time.GetMinute() );
					pItem->AddToolTipLine( buf, TTTC_QUESTITEM );
				}
				else 
				{
					pItem->AddToolTipLine( CHATMGR->GetChatMsg(1442), TTTC_DEFAULT );
					pItem->AddToolTipLine( CHATMGR->GetChatMsg(1444), TTTC_QUESTITEM );
				}
			}				
		}
		else
		{
			PrintShopItemUseTime( pItem, pInfo );
		}
	}
}

void CItemManager::SetChangeItemToolTip( cIcon * pIcon, ITEM_INFO * pInfo)
{
	WORD index = pIcon->GetData();
	eITEM_KINDBIT bits = GetItemKind(index);

	CExchangeItem* pExcItem = NULL;
	CItem * pcItem = NULL;
	if( pIcon->GetType() == WT_EXCHANGEITEM || pIcon->GetType() == WT_STALLITEM || pInfo->ItemKind == eSHOP_ITEM_PET ||pIcon->GetType() == WT_BUYITE)
		pExcItem = (CExchangeItem*)pIcon;
	else
		pcItem = (CItem*)pIcon;

	char line[128];

	sprintf(line,CHATMGR->GetChatMsg(2175),CHATMGR->GetChatMsg(2217));  
	pIcon->AddToolTipLine(line,TTTC_ITEM_LEVEL);
	if( bits == eCHANGE_ITEM_LOCK )
	{
		sprintf(line, CHATMGR->GetChatMsg(238), pInfo->ItemType);
		if( HERO->GetCharacterTotalInfo()->Level < pInfo->ItemType )
			pIcon->AddToolTipLine( line, TTCLR_ENERMY );
		else
			pIcon->AddToolTipLine( line, TTTC_FREELIMIT );
	}
	else
	{
		pIcon->AddToolTipLine( JACKJACK );
	}
	AddItemDescriptionToolTip( pIcon, pInfo->ItemTooltipIdx );
}
void CItemManager::SetToolTipIcon(
								  cIcon * pIcon,
								  ITEM_OPTION_INFO * pOptionInfo,
								  ITEM_RARE_OPTION_INFO* pRareOptionInfo, 
								  DWORD dwItemDBIdx,
								  ITEM_STONE_OPTION_INFO * pStoneOptionInfo,
								  BOOL bIsQuey,
								  BYTE bNormalTip,
								  BYTE bGotTime,
								  BOOL IsGoldShop, DWORD Grade30)
{
	WORD index = pIcon->GetData();
	eITEM_KINDBIT bits = GetItemKind(index);
	pIcon->IniTipLineCount();

	cImage imgHeader;
	SCRIPTMGR->GetImage(25, &imgHeader, PFT_JACKPATH );
	pIcon->SetHeaderImage(&imgHeader);
	cImage imgToolTip;
	SCRIPTMGR->GetImage(26, &imgToolTip, PFT_JACKPATH );
	pIcon->SetToolTip( "", RGBA_MAKE(255, 255, 255, 255), &imgToolTip, TTCLR_ITEM_CANEQUIP );
	pIcon->SetToolModel(1); 
	ITEM_INFO * pInfo = GetItemInfo(index);
	if (pInfo == NULL) return;

	cImage * imgEquipInfo;
	imgEquipInfo=pIcon->GetBasicImage();
	if(imgEquipInfo)
	{
		pIcon->AddToolTipImageEquip(imgEquipInfo);
	}
	else
	{
		pIcon->AddToolTipLine( "" );
	}



	if(bits & eEQUIP_ITEM)
	{
		if(bNormalTip==0)
		{
			char line[256];
			CExchangeItem* pExcItem = NULL;
			CItem* pcItem = NULL;
			int quality = -1;
			DWORD color = TTTC_DEFAULT;
			const char* qualityName = "";

			// 获取 Item 对象并取得品质
			if (pIcon->GetType() == WT_EXCHANGEITEM || pIcon->GetType() == WT_STALLITEM || pIcon->GetType() == WT_BUYITE)
			{
				pExcItem = (CExchangeItem*)pIcon;
				quality = pExcItem->GetQuality();
			}
			else
			{
				pcItem = (CItem*)pIcon;
				quality = pcItem->GetQuality();
			}

			// 映射品质值到 S~SSSSS
			switch (quality)
			{
			case 0: qualityName = "·普通"; color = TTTC_DEFAULT; break;
			case 1: qualityName = "·优秀"; color = TTTC_ITEMGROW0; break;
			case 2: qualityName = "·精良"; color = TTTC_ITEMGROW2; break;
			case 3: qualityName = "·传奇"; color = TTTC_ITEMGROW4; break;
			case 4: qualityName = "·神话"; color = TTTC_ITEMGROW5; break;
			default: qualityName = ""; break;
			}

			// 构建名称字符串
			if (Grade30 > 0)
			{
				if (pOptionInfo)
					sprintf(line, "[%s +%d*]%s", pInfo->ItemName, Grade30, qualityName);
				else
					sprintf(line, "[%s +%d]%s", pInfo->ItemName, Grade30, qualityName);
			}
			else
			{
				if (pOptionInfo)
					sprintf(line, "[%s*]%s", pInfo->ItemName, qualityName);
				else
					sprintf(line, "[%s]%s", pInfo->ItemName, qualityName);
			}
			int nLen = strlen( pInfo->ItemName );
			if(pRareOptionInfo && pRareOptionInfo->dwItemDBIdx)
				pIcon->AddToolTipLine( line, TTTC_RAREITEM,NULL,6);
			else if(pInfo->wSetItemKind != 0)
				pIcon->AddToolTipLine( line, TTTC_SETITEM_NAME,NULL,6);
#ifdef _LIKEPLAYPARK
			else if(pInfo->ItemGrade>=10&&pInfo->ItemGrade<=19)
				pIcon->AddToolTipLine( line, RGB_HALF(255,212,234),NULL,6);
#endif
			else if(pInfo->ItemKind == eEQUIP_ITEM_UNIQUE)	
				pIcon->AddToolTipLine( line, TTTC_MUGONGKIND,NULL,6);
#ifdef _LIKEPLAYPARK
			else if(pStoneOptionInfo && pStoneOptionInfo->dwStoneOptionIdx)
				pIcon->AddToolTipLine( line, RGB_HALF(255,212,234),NULL,6);
#endif
			else
				pIcon->AddToolTipLine( line, color,NULL,6);

			pIcon->AddToolTipLine( JACKJACK );
			SetEquipItemToolTip(pIcon,pInfo,pOptionInfo,pRareOptionInfo,pStoneOptionInfo,bIsQuey, Grade30);
			pIcon->AddToolTipLine( JACKJACK );
		}

		if(bNormalTip==1 || bNormalTip==2)
		{
			char line[256];
			cImage imgToolLine;
			CExchangeItem* pExcItem = NULL;
			CItem* pcItem = NULL;
			

			// 获取品质值（优先从 ExchangeItem，其次从 Item）
			int quality = -1;
			if (pExcItem)
				quality = pExcItem->GetQuality();
			else if (pcItem)
				quality = pcItem->GetQuality();
			DWORD color = TTTC_DEFAULT;
			// 获取品质文字与颜色
			const char* qualityName = "";
			switch (quality)
			{
			case 0: qualityName = "·普通"; color = TTTC_DEFAULT; break;
			case 1: qualityName = "·优秀"; color = TTTC_ITEMGROW0; break;
			case 2: qualityName = "·精良"; color = TTTC_ITEMGROW2; break;
			case 3: qualityName = "·传奇"; color = TTTC_ITEMGROW4; break;
			case 4: qualityName = "·神话"; color = TTTC_ITEMGROW5; break;
			default: qualityName = ""; break;
			}

			// 构建最终文字
			if (Grade30 > 0)
			{
				if (pOptionInfo)
					sprintf(line, "[%s +%d*]%s", pInfo->ItemName, Grade30, qualityName);
				else
					sprintf(line, "[%s +%d]%s", pInfo->ItemName, Grade30, qualityName);
			}
			else
			{
				if (pOptionInfo)
					sprintf(line, "[%s*]%s", pInfo->ItemName, qualityName);
				else
					sprintf(line, "[%s]%s", pInfo->ItemName, qualityName);
			}
			int nLen = strlen(pInfo->ItemName);
			if(pRareOptionInfo && pRareOptionInfo->dwItemDBIdx)
				pIcon->AddToolTipLine( line, TTTC_RAREITEM,NULL,6);
			else if(pInfo->wSetItemKind != 0)
				pIcon->AddToolTipLine( line, TTTC_SETITEM_NAME,NULL,6);
#ifdef _LIKEPLAYPARK
			else if(pInfo->ItemGrade>=10&&pInfo->ItemGrade<=19)
				pIcon->AddToolTipLine( line, RGB_HALF(255,212,234),NULL,6);
#endif
			else if(pInfo->ItemKind == eEQUIP_ITEM_UNIQUE)	
				pIcon->AddToolTipLine( line, TTTC_MUGONGKIND,NULL,6);
#ifdef _LIKEPLAYPARK
			else if(pStoneOptionInfo && pStoneOptionInfo->dwStoneOptionIdx)
				pIcon->AddToolTipLine( line, RGB_HALF(255,212,234),NULL,6);
#endif
			else
				pIcon->AddToolTipLine( line,TTTC_DEFAULT,NULL,6);
			pIcon->AddToolTipLine( JACKJACK );
			SetEquipItemToolTip(pIcon,pInfo,0,0,0,TRUE);
			pIcon->AddToolTipLine( JACKJACK );
		}
	}
	else
	{
		char line[128];
		wsprintf( line, "[%s]", pInfo->ItemName );			
		pIcon->AddToolTipLine( line,TTTC_DEFAULT,NULL,6);
		pIcon->AddToolTipLine( JACKJACK );
	}

	if(bits & eMUGONG_ITEM)
	{
		SetMugongItemToolTip( pIcon, pInfo );
		pIcon->AddToolTipLine( JACKJACK );
	}
	else if(bits & eYOUNGYAK_ITEM)
	{
		SetYoungyakItemToolTip( pIcon, pInfo );
		pIcon->AddToolTipLine( JACKJACK );
	}
	else if(bits & eEXTRA_ITEM || bits & eEXTRA_ITEM_UPGRADE)
	{
		SetExtraItemToolTip( pIcon, pInfo );
		pIcon->AddToolTipLine( JACKJACK );
	}
	else if( bits & eQUEST_ITEM )
	{
		SetQuestItemToolTip(pIcon, pInfo, dwItemDBIdx, IsGoldShop);
		pIcon->AddToolTipLine( JACKJACK );
	}
	else if( bits & eCHANGE_ITEM )
	{
		SetChangeItemToolTip(pIcon,pInfo);
		pIcon->AddToolTipLine( JACKJACK );
	}
	else if( bits & eSHOP_ITEM )
	{
		if(bNormalTip==0)
		{
			//pIcon->AddToolTipLine( "" );
			SetShopItemToolTip(pIcon,pInfo);
			pIcon->AddToolTipLine( JACKJACK );
		}
		if(bNormalTip==1 || bNormalTip==2)
		{
			SetShopItemToolTipMall(pIcon,pInfo);
		}
	}
	else if( bits & eTITAN_EQUIPITEM || bits == eTITAN_ITEM_RECIPE || bits == eTITAN_ITEM_PAPER || bits == eTITAN_ITEM_PARTS )
	{
		SetTitanToolTip(pIcon,pInfo,dwItemDBIdx);
		pIcon->AddToolTipLine( JACKJACK );
	}
}

//---------------------------------------------------------------------------------
void CItemManager::SetGoldShopToolTipIcon(cIcon * pIcon)
{
	//CExchangeItem* pExcItem = NULL;
	//CItem * pcItem = NULL;
	//if( pIcon->GetType() == WT_EXCHANGEITEM || pIcon->GetType() == WT_STALLITEM || pInfo->ItemKind == eSHOP_ITEM_PET ||pIcon->GetType() == WT_BUYITE)
	//	pExcItem = (CExchangeItem*)pIcon;
	///else
	//pcItem = (CItem*)pIcon;

	char buf[256] = { 0, };
	WORD index = (WORD)pIcon->GetData();
	eITEM_KINDBIT bits = GetItemKind(index);
	pIcon->IniTipLineCount();
	
	cImage imgHeader;
	SCRIPTMGR->GetImage(25, &imgHeader, PFT_JACKPATH );
	pIcon->SetHeaderImage(&imgHeader);
	cImage imgToolTip;
	SCRIPTMGR->GetImage(26, &imgToolTip, PFT_JACKPATH );
	pIcon->SetToolTip( "", RGBA_MAKE(255, 255, 255, 255), &imgToolTip, TTCLR_ITEM_CANEQUIP );
	pIcon->SetToolModel(1);


	/*if(bits & eEQUIP_ITEM)
	{
		ITEM_INFO * pInfo = GetItemInfo(index);
		if(pInfo)
			SetEquipItemToolTip( pIcon, pInfo,NULL, NULL,NULL );
	}*/
	/*else if(bits & eMUGONG_ITEM)
	{
		ITEM_INFO * pInfo = GetItemInfo(index);
		if(pInfo)
			SetMugongItemToolTip( pIcon, pInfo );
	}*/
	/*else if(bits & eYOUNGYAK_ITEM)
	{
		ITEM_INFO * pInfo = GetItemInfo(index);
		if(pInfo)
			SetYoungyakItemToolTip( pIcon, pInfo );
	}
	else if(bits & eEXTRA_ITEM || bits & eEXTRA_ITEM_UPGRADE)
	{
		ITEM_INFO * pInfo = GetItemInfo(index);
		if(pInfo)
			SetExtraItemToolTip( pIcon, pInfo );
	}
	else*/ if( bits & eQUEST_ITEM )
	{
		ITEM_INFO * pInfo = GetItemInfo(index);
		if( pInfo )
		{
			char line[128];
			wsprintf( line, "[%s]", pInfo->ItemName );
			pIcon->AddToolTipLine( line );
			pIcon->AddToolTipLine( "" );

			if( FALSE == IsPetSummonItem(index) )
				pIcon->AddToolTipLine( CHATMGR->GetChatMsg(260), TTTC_QUESTITEM );

			if( IsPetSummonItem(index) )
			{
				SetPetSummonItemToolTip(pIcon,pInfo,PetDbIdx);
			}

			AddItemDescriptionToolTip( pIcon, pInfo->ItemTooltipIdx );
		}
	}
	else if( bits & eCHANGE_ITEM )
	{
		ITEM_INFO * pInfo = GetItemInfo(index);
		if( pInfo )
		{
			cImage * imgExtraIteminfo;

			imgExtraIteminfo=pIcon->GetBasicImage();

			if(imgExtraIteminfo)
			{
				pIcon->AddToolTipImageEquip(imgExtraIteminfo);
			}
			CExchangeItem* pExcItem = NULL;
			CItem * pcItem = NULL;
			if( pIcon->GetType() == WT_EXCHANGEITEM || pIcon->GetType() == WT_STALLITEM || pInfo->ItemKind == eSHOP_ITEM_PET ||pIcon->GetType() == WT_BUYITE)
				pExcItem = (CExchangeItem*)pIcon;
			else
				pcItem = (CItem*)pIcon;
			char line[128];
			wsprintf( line, "[%s]", pInfo->ItemName );			
			pIcon->AddToolTipLine( line );

			pIcon->AddToolTipLine(CHATMGR->GetChatMsg(2240),NULL);

			if(pcItem)
			{
				if(pcItem->GetStatic()==1)
				{
					sprintf(line,CHATMGR->GetChatMsg(1608));
					pIcon->AddToolTipLine(line,TTTC_LIMIT);
				}
				else
					pIcon->AddToolTipLine("");
			}
			else
				pIcon->AddToolTipLine("");

			//pIcon->AddImagTopLine(&imgTopLine,pIcon->GetTipLineCount());
			if( bits == eCHANGE_ITEM_LOCK )
			{
				sprintf(line, CHATMGR->GetChatMsg(238), pInfo->ItemType);
				if( HERO->GetCharacterTotalInfo()->Level < pInfo->ItemType )
					pIcon->AddToolTipLine( line, TTTC_LIMIT );
				else
					pIcon->AddToolTipLine( line, TTTC_FREELIMIT );
			}
			else
				pIcon->AddToolTipLine("");
			AddItemDescriptionToolTip( pIcon, pInfo->ItemTooltipIdx );
		}
	}
	else if( bits & eSHOP_ITEM )
	{
		ITEM_INFO* pInfo = GetItemInfo(index);
		if(pInfo)
		{
			CExchangeItem* pExcItem = NULL;
			CItem * pcItem = NULL;
			if( pIcon->GetType() == WT_EXCHANGEITEM || pIcon->GetType() == WT_STALLITEM || pInfo->ItemKind == eSHOP_ITEM_PET||pIcon->GetType() == WT_BUYITE )
				pExcItem = (CExchangeItem*)pIcon;
			else
				pcItem = (CItem*)pIcon;

			char line[128];
			wsprintf( line, "[%s]", pInfo->ItemName );
			DWORD dwColor = TTTC_DEFAULT;
			if( pInfo->ItemKind == eSHOP_ITEM_INCANTATION && pInfo->LimitJob )
			{				
				if( pInfo->LimitGender == 0 && HERO->GetLevel() > 50 )
				{
					dwColor = TTTC_LIMIT;
				}
				else if( pInfo->LimitGender == 1 && (HERO->GetLevel() <51 || HERO->GetLevel() > 70 ) )
				{
					dwColor = TTTC_LIMIT;
				}
				else if( pInfo->LimitGender == 2 && (HERO->GetLevel() <71 || HERO->GetLevel() > 90 ) )
				{
					dwColor = TTTC_LIMIT;
				}
			}
			else if( HERO->GetLevel() < pInfo->NaeRyukRecoverRate )
			{
				dwColor = TTTC_LIMIT;
			}
			pIcon->AddToolTipLine( line, dwColor );
			if (pInfo->ItemKind==eSHOP_ITEM_PET||pInfo->ItemKind==eSHOP_ITEM_PET_EQUIP)
			{
				pIcon->AddToolTipLine(CHATMGR->GetChatMsg(2241));
				if( pInfo->ItemKind == eSHOP_ITEM_PET )
				{
					if( pInfo->ItemType == 11 )
					{
						if( !(((CItem*)pIcon)->GetItemBaseInfo()->ItemParam & ITEM_PARAM_SEAL) )
						{
							sprintf(line,CHATMGR->GetChatMsg(1608));
							pIcon->AddToolTipLine(line,TTTC_LIMIT);
						}
						else
							pIcon->AddToolTipLine("");
					}
				}
			}
			else if (pInfo->ItemKind==eSHOP_ITEM_MAKEUP||pInfo->ItemKind==eSHOP_ITEM_DECORATION)
			{
				pIcon->AddToolTipLine(CHATMGR->GetChatMsg(2243));
				if(pcItem)
				{
					if(pcItem->GetStatic()==1)
					{
						sprintf(line,CHATMGR->GetChatMsg(1608));
						pIcon->AddToolTipLine(line,TTTC_LIMIT);
					}
					else
						pIcon->AddToolTipLine("");
				}
				else
					pIcon->AddToolTipLine("");
			}
			else if (pInfo->ItemKind==eSHOP_ITEM_CHARM||pInfo->ItemKind==eSHOP_ITEM_HERB	||
				pInfo->ItemKind==eSHOP_ITEM_SUNDRIES||pInfo->ItemKind==eSHOP_ITEM_GETIN_ITEM ||
				pInfo->ItemKind==eSHOP_ITEM_IMAGENAME ||pInfo->ItemKind==eSHOP_ITEM_MARRYCHARM ||
				pInfo->ItemKind==eSHOP_ITEM_GOLDITEM ||pInfo->ItemKind==eSHOP_ITEM_GROWITEM)
			{
				pIcon->AddToolTipLine(CHATMGR->GetChatMsg(2244));
				if(pcItem)
				{
					if(pcItem->GetStatic()==1)
					{
						sprintf(line,CHATMGR->GetChatMsg(1608));
						pIcon->AddToolTipLine(line,TTTC_LIMIT);
					}
					else
						pIcon->AddToolTipLine("");
				}
				else
					pIcon->AddToolTipLine("");
			}
			else if (pInfo->ItemKind==eSHOP_ITEM_INCANTATION)
			{
				pIcon->AddToolTipLine(CHATMGR->GetChatMsg(2245));
				if(pcItem)
				{
					if(pcItem->GetStatic()==1)
					{
						sprintf(line,CHATMGR->GetChatMsg(1608));
						pIcon->AddToolTipLine(line,TTTC_LIMIT);
					}
					else
						pIcon->AddToolTipLine("");
				}
				else
					pIcon->AddToolTipLine("");
				pIcon->AddToolTipLine("");
			}
			else 
			{
				pIcon->AddToolTipLine(CHATMGR->GetChatMsg(2246));
				if(pcItem)
				{
					if(pcItem->GetStatic()==1)
					{
						sprintf(line,CHATMGR->GetChatMsg(1608));
						pIcon->AddToolTipLine(line,TTTC_LIMIT);
					}
					else
						pIcon->AddToolTipLine("");
				}
				else
					pIcon->AddToolTipLine("");
				pIcon->AddToolTipLine("");
			}
			if( pInfo->NaeRyukRecoverRate != 0 )
			{

				sprintf(line, CHATMGR->GetChatMsg(238), (int)pInfo->NaeRyukRecoverRate);
				if( HERO->GetCharacterTotalInfo()->Level < pInfo->NaeRyukRecoverRate )
					pIcon->AddToolTipLine( line, TTTC_LIMIT );
				else
					pIcon->AddToolTipLine( line, TTTC_FREELIMIT );
			}
			else
				pIcon->AddToolTipLine("");


			if( pIcon->GetType() == WT_EXCHANGEITEM || pIcon->GetType() == WT_STALLITEM || pInfo->ItemKind == eSHOP_ITEM_PET ||pIcon->GetType() == WT_BUYITE)
			{
				CExchangeItem* pExItem = (CExchangeItem*)pIcon;
				cImage * imgGoldShopinfo;
				imgGoldShopinfo=pIcon->GetBasicImage();
				if(imgGoldShopinfo)
				{
					pIcon->AddToolTipImageEquip(imgGoldShopinfo);
				}
				if( pExItem->GetItemParam() & ITEM_PARAM_SEAL )
				{
					if( IsPetSummonItem(index) )
					{
						SetPetSummonItemToolTip(pIcon,pInfo,PetDbIdx);
					}
					AddItemDescriptionToolTip( pIcon, 6000 );
					AddItemDescriptionToolTip( pIcon, pInfo->ItemTooltipIdx );
				}
				else
				{
					const ITEMBASE* pBaseInfo = ((CItem*)pIcon)->GetItemBaseInfo();
					if( IsPetSummonItem(index) )
					{
						SetPetSummonItemToolTip(pIcon,pInfo,PetDbIdx);

					}

					AddItemDescriptionToolTip( pIcon, pInfo->ItemTooltipIdx );
				}
				return;
			}

			CItem* pItem = (CItem*)pIcon;
			if( pInfo->ItemKind == eSHOP_ITEM_MAKEUP || pInfo->ItemKind == eSHOP_ITEM_DECORATION )
			{
				if( !(pItem->GetItemParam() & ITEM_PARAM_SEAL) )
				{
					AddGoldShopToolTip( (cIcon*)pItem, pInfo ); 
					return;
				}				
			}
			if( pIcon->GetType()==WT_ITEM && (pItem->GetItemParam() & ITEM_PARAM_SEAL) )
			{
				AddGoldShopToolTip( (cIcon*)pItem, pInfo );
				AddItemDescriptionToolTip( pIcon, 6000 );
				PrintShopItemUseTime( pItem, pInfo );
			}
			else
			{
				AddGoldShopToolTip( (cIcon*)pItem, pInfo );  
				AddItemDescriptionToolTip( pIcon, pInfo->ItemTooltipIdx );

				if( pInfo->ItemType == 11 )
				{					
					SHOPITEMBASE* pShopItemBase = m_UsedItemList.GetData( pItem->GetItemIdx() );
					if( pShopItemBase )
					{
						if( pInfo->SellPrice == eShopItemUseParam_Realtime )
						{
							pItem->AddToolTipLine("");
							pItem->AddToolTipLine( CHATMGR->GetChatMsg(766), TTTC_DEFAULT );

							stTIME time;
							char buf[128] = { 0, };
							time.value = pShopItemBase->Remaintime;	
							wsprintf( buf, CHATMGR->GetChatMsg(767), time.GetYear(), time.GetMonth(), time.GetDay(), time.GetHour(), time.GetMinute() );
							pItem->AddToolTipLine( buf, TTTC_QUESTITEM );
						}
						else 
						{
							pItem->AddToolTipLine("");
							pItem->AddToolTipLine( CHATMGR->GetChatMsg(1442), TTTC_DEFAULT );
							pItem->AddToolTipLine( CHATMGR->GetChatMsg(1444), TTTC_QUESTITEM );

						}
					}					
				}
				else{
					PrintShopItemUseTime( pItem, pInfo );
				}

			}
		}
	}
}
void CItemManager::AddGoldShopToolTip( cIcon* pIcon, ITEM_INFO* pItemInfo )
{
	char line[128];	
	char buf[256] = { 0, };

	cImage  imgTopLine;
	SCRIPTMGR->GetImage(217,&imgTopLine,PFT_HARDPATH);

	cImage * imgGoldShopinfo;
	imgGoldShopinfo=pIcon->GetBasicImage();
	if(imgGoldShopinfo)
	{
		pIcon->AddToolTipImageEquip(imgGoldShopinfo);
	}
	//[分割线][By:十里坡剑传奇][QQ:112582793][2017/11/28]
//	pIcon->AddImagTopLine(&imgTopLine,pIcon->GetTipLineCount());
	
	if( pItemInfo->GenGol != 0 )
	{
		if( pItemInfo->ItemKind == eSHOP_ITEM_PREMIUM )
		{
			sprintf(line, CHATMGR->GetChatMsg(1395), pItemInfo->GenGol);
			pIcon->AddToolTipLine( line, TTTC_EXTRAATTR );
		}
		if( pItemInfo->ItemKind == eSHOP_ITEM_CHARM || 
			pItemInfo->ItemKind == eSHOP_ITEM_DECORATION||
			pItemInfo->ItemKind == eSHOP_ITEM_MAKEUP||
			pItemInfo->ItemKind == eSHOP_ITEM_IMAGENAME||
			pItemInfo->ItemKind == eSHOP_ITEM_MARRYCHARM)  
		{
			sprintf(line, CHATMGR->GetChatMsg(1437), pItemInfo->GenGol);
			pIcon->AddToolTipLine( line, TTTC_EXTRAATTR );
		}
		if( pItemInfo->ItemKind == eSHOP_ITEM_HERB )
		{
			sprintf(line, CHATMGR->GetChatMsg(1409), pItemInfo->GenGol);
			pIcon->AddToolTipLine( line, TTTC_EXTRAATTR );
		}
		if( pItemInfo->ItemKind == eSHOP_ITEM_INCANTATION )
		{
			sprintf(line, CHATMGR->GetChatMsg(1470), pItemInfo->GenGol);
			pIcon->AddToolTipLine( line, TTTC_EXTRAATTR );
		}
	}
	if( pItemInfo->MinChub != 0 )
	{
		if( pItemInfo->ItemKind == eSHOP_ITEM_PREMIUM )
		{
			sprintf(line, CHATMGR->GetChatMsg(1403), pItemInfo->MinChub);
			pIcon->AddToolTipLine( line, TTTC_EXTRAATTR );
		}
		if( pItemInfo->ItemKind == eSHOP_ITEM_CHARM || 
			pItemInfo->ItemKind == eSHOP_ITEM_DECORATION||
			pItemInfo->ItemKind == eSHOP_ITEM_MAKEUP||
			pItemInfo->ItemKind == eSHOP_ITEM_IMAGENAME||
			pItemInfo->ItemKind == eSHOP_ITEM_MARRYCHARM)  
		{
			sprintf(line, CHATMGR->GetChatMsg(1438), pItemInfo->MinChub);
			pIcon->AddToolTipLine( line, TTTC_EXTRAATTR );
		}
		if( pItemInfo->ItemKind == eSHOP_ITEM_HERB )
		{
			sprintf(line, CHATMGR->GetChatMsg(1433), pItemInfo->MinChub);
			pIcon->AddToolTipLine( line, TTTC_EXTRAATTR );
		}
	}
	if( pItemInfo->CheRyuk != 0 )
	{
		if( pItemInfo->ItemKind == eSHOP_ITEM_PREMIUM )
		{
			sprintf(line, CHATMGR->GetChatMsg(1430), pItemInfo->CheRyuk);
			pIcon->AddToolTipLine( line, TTTC_EXTRAATTR );
		}
		if( pItemInfo->ItemKind == eSHOP_ITEM_CHARM || 
			pItemInfo->ItemKind == eSHOP_ITEM_DECORATION||
			pItemInfo->ItemKind == eSHOP_ITEM_MAKEUP||
			pItemInfo->ItemKind == eSHOP_ITEM_IMAGENAME||
			pItemInfo->ItemKind == eSHOP_ITEM_MARRYCHARM)  
		{
			sprintf(line, CHATMGR->GetChatMsg(1441), pItemInfo->CheRyuk);
			pIcon->AddToolTipLine( line, TTTC_EXTRAATTR );
		}
		if( pItemInfo->ItemKind == eSHOP_ITEM_HERB )
		{
			sprintf(line, CHATMGR->GetChatMsg(1399), pItemInfo->CheRyuk);
			pIcon->AddToolTipLine( line, TTTC_EXTRAATTR );
		}
		if( pItemInfo->ItemKind == eSHOP_ITEM_INCANTATION )
		{
			sprintf(line, CHATMGR->GetChatMsg(1396), pItemInfo->CheRyuk);
			pIcon->AddToolTipLine( line, TTTC_EXTRAATTR );
		}
	}
	if( pItemInfo->CheRyuk == 1 )
	{
		if( pItemInfo->ItemKind == eSHOP_ITEM_SUNDRIES )
		{
			sprintf(line, CHATMGR->GetChatMsg(1402));
			pIcon->AddToolTipLine( line, TTTC_EXTRAATTR );
		}
	}

	if( pItemInfo->SimMek != 0 )
	{
		if( pItemInfo->ItemKind == eSHOP_ITEM_PREMIUM )
		{
			sprintf(line, CHATMGR->GetChatMsg(1431), pItemInfo->SimMek);
			pIcon->AddToolTipLine( line, TTTC_EXTRAATTR );
		}
		if( pItemInfo->ItemKind == eSHOP_ITEM_CHARM || 
			pItemInfo->ItemKind == eSHOP_ITEM_DECORATION||
			pItemInfo->ItemKind == eSHOP_ITEM_MAKEUP||
			pItemInfo->ItemKind == eSHOP_ITEM_IMAGENAME||
			pItemInfo->ItemKind == eSHOP_ITEM_MARRYCHARM) 
		{
			sprintf(line, CHATMGR->GetChatMsg(1439), pItemInfo->SimMek);
			pIcon->AddToolTipLine( line, TTTC_EXTRAATTR );
		}
		if( pItemInfo->ItemKind == eSHOP_ITEM_HERB )
		{
			sprintf(line, CHATMGR->GetChatMsg(1421), pItemInfo->SimMek);
			pIcon->AddToolTipLine( line, TTTC_EXTRAATTR );
		}
	}
	if( pItemInfo->Life != 0 )
	{
		if( pItemInfo->ItemKind == eSHOP_ITEM_CHARM  ||
			pItemInfo->ItemKind == eSHOP_ITEM_IMAGENAME||
			pItemInfo->ItemKind == eSHOP_ITEM_MARRYCHARM)
		{
			sprintf(line, CHATMGR->GetChatMsg(1397), pItemInfo->Life);
			pIcon->AddToolTipLine( line, TTTC_EXTRAATTR );
		}
		if( pItemInfo->ItemKind == eSHOP_ITEM_HERB || 
			pItemInfo->ItemKind == eSHOP_ITEM_DECORATION||
			pItemInfo->ItemKind == eSHOP_ITEM_MAKEUP)  
		{
			sprintf(line, CHATMGR->GetChatMsg(1419), pItemInfo->Life);
			pIcon->AddToolTipLine( line, TTTC_EXTRAATTR );
		}
		if( pItemInfo->ItemKind == eSHOP_ITEM_INCANTATION )
		{
			sprintf(line, CHATMGR->GetChatMsg(1417), pItemInfo->Life);
			pIcon->AddToolTipLine( line, TTTC_EXTRAATTR );
		}
	}
	if( pItemInfo->Shield != 0 )
	{
		if( pItemInfo->ItemKind == eSHOP_ITEM_CHARM ||
			pItemInfo->ItemKind == eSHOP_ITEM_IMAGENAME||
			pItemInfo->ItemKind == eSHOP_ITEM_MARRYCHARM)
		{
			sprintf(line, CHATMGR->GetChatMsg(1416), pItemInfo->Shield);
			pIcon->AddToolTipLine( line, TTTC_EXTRAATTR );
		}
		if( pItemInfo->ItemKind == eSHOP_ITEM_HERB || 
			pItemInfo->ItemKind == eSHOP_ITEM_DECORATION||
			pItemInfo->ItemKind == eSHOP_ITEM_MAKEUP)  
		{
			sprintf(line, CHATMGR->GetChatMsg(1420), pItemInfo->Shield);
			pIcon->AddToolTipLine( line, TTTC_EXTRAATTR );
		}
	}
	if( pItemInfo->NaeRyuk != 0 )
	{
		if( pItemInfo->ItemKind == eSHOP_ITEM_CHARM ||
			pItemInfo->ItemKind == eSHOP_ITEM_IMAGENAME||
			pItemInfo->ItemKind == eSHOP_ITEM_MARRYCHARM)
		{
			sprintf(line, CHATMGR->GetChatMsg(1428), pItemInfo->NaeRyuk);
			pIcon->AddToolTipLine( line, TTTC_EXTRAATTR );
		}
		if( pItemInfo->ItemKind == eSHOP_ITEM_HERB || 
			pItemInfo->ItemKind == eSHOP_ITEM_DECORATION||
			pItemInfo->ItemKind == eSHOP_ITEM_MAKEUP)  
		{
			sprintf(line, CHATMGR->GetChatMsg(1418), pItemInfo->NaeRyuk);
			pIcon->AddToolTipLine( line, TTTC_EXTRAATTR );
		}
	}
	if( pItemInfo->LimitJob != 0 )
	{
		if( pItemInfo->ItemKind == eSHOP_ITEM_CHARM ||
			pItemInfo->ItemKind == eSHOP_ITEM_IMAGENAME||
			pItemInfo->ItemKind == eSHOP_ITEM_MARRYCHARM)
		{
			sprintf(line, CHATMGR->GetChatMsg(1422), pItemInfo->LimitJob);
			pIcon->AddToolTipLine( line, TTTC_EXTRAATTR );
		}
		if( pItemInfo->ItemKind == eSHOP_ITEM_INCANTATION  )
		{            
			sprintf(line, CHATMGR->GetChatMsg(1432), pItemInfo->LimitJob);
			pIcon->AddToolTipLine( line, TTTC_EXTRAATTR );
		}
		if( pItemInfo->ItemKind == eSHOP_ITEM_DECORATION||
			pItemInfo->ItemKind == eSHOP_ITEM_MAKEUP) 
		{
			sprintf(line, CHATMGR->GetChatMsg(1397), pItemInfo->LimitJob);
			pIcon->AddToolTipLine( line, TTTC_EXTRAATTR );
		}
	}
	if( pItemInfo->LimitGender != 0 )
	{
		if( pItemInfo->ItemKind == eSHOP_ITEM_CHARM  ||
			pItemInfo->ItemKind == eSHOP_ITEM_IMAGENAME||
			pItemInfo->ItemKind == eSHOP_ITEM_MARRYCHARM)
		{
			sprintf(line, CHATMGR->GetChatMsg(1423), pItemInfo->LimitGender);
			pIcon->AddToolTipLine( line, TTTC_EXTRAATTR );
		}
		if( pItemInfo->ItemKind == eSHOP_ITEM_DECORATION||
			pItemInfo->ItemKind == eSHOP_ITEM_MAKEUP)  
		{
			sprintf(line, CHATMGR->GetChatMsg(1416), pItemInfo->LimitGender);
			pIcon->AddToolTipLine( line, TTTC_EXTRAATTR );
		}
	}
	if( pItemInfo->LimitLevel != 0 )
	{
		if( pItemInfo->ItemKind == eSHOP_ITEM_CHARM ||
			pItemInfo->ItemKind == eSHOP_ITEM_IMAGENAME||
			pItemInfo->ItemKind == eSHOP_ITEM_MARRYCHARM)
		{
			sprintf(line, CHATMGR->GetChatMsg(1424), pItemInfo->LimitLevel);
			pIcon->AddToolTipLine( line, TTTC_EXTRAATTR );
		}
		if( pItemInfo->ItemKind == eSHOP_ITEM_DECORATION||
			pItemInfo->ItemKind == eSHOP_ITEM_MAKEUP) 
		{
			sprintf(line, CHATMGR->GetChatMsg(1426), pItemInfo->LimitLevel);
			pIcon->AddToolTipLine( line, TTTC_EXTRAATTR );
		}
	}
	if( pItemInfo->LimitGenGol != 0 )
	{
		if( pItemInfo->ItemKind == eSHOP_ITEM_CHARM||
			pItemInfo->ItemKind == eSHOP_ITEM_IMAGENAME||
			pItemInfo->ItemKind == eSHOP_ITEM_MARRYCHARM)
		{
			sprintf(line, CHATMGR->GetChatMsg(1406), pItemInfo->LimitGenGol);
			pIcon->AddToolTipLine( line, TTTC_EXTRAATTR );
		}
		if( pItemInfo->ItemKind == eSHOP_ITEM_DECORATION||
			pItemInfo->ItemKind == eSHOP_ITEM_MAKEUP) 
		{
			sprintf(line, CHATMGR->GetChatMsg(1427), pItemInfo->LimitGenGol);
			pIcon->AddToolTipLine( line, TTTC_EXTRAATTR );
		}
		if( pItemInfo->ItemKind == eSHOP_ITEM_PET_EQUIP  )
		{
			sprintf(line, CHATMGR->GetChatMsg(1450), pItemInfo->LimitGenGol);
			pIcon->AddToolTipLine( line, TTTC_EXTRAATTR );
		}
	}
	if( pItemInfo->LimitMinChub != 0 )
	{
		if( pItemInfo->ItemKind == eSHOP_ITEM_CHARM||
			pItemInfo->ItemKind == eSHOP_ITEM_IMAGENAME||
			pItemInfo->ItemKind == eSHOP_ITEM_MARRYCHARM)
		{
			sprintf(line, CHATMGR->GetChatMsg(1410), pItemInfo->LimitMinChub);
			pIcon->AddToolTipLine( line, TTTC_EXTRAATTR );
		}
		if( pItemInfo->ItemKind == eSHOP_ITEM_DECORATION||
			pItemInfo->ItemKind == eSHOP_ITEM_MAKEUP)  
		{
			sprintf(line, CHATMGR->GetChatMsg(1425), pItemInfo->LimitMinChub);
			pIcon->AddToolTipLine( line, TTTC_EXTRAATTR );
		}
		if( pItemInfo->ItemKind == eSHOP_ITEM_PET_EQUIP  )
		{
			sprintf(line, CHATMGR->GetChatMsg(1451), pItemInfo->LimitMinChub);
			pIcon->AddToolTipLine( line, TTTC_EXTRAATTR );
		}

	}
	if( pItemInfo->LimitCheRyuk != 0 )
	{
		if( pItemInfo->ItemKind == eSHOP_ITEM_CHARM || 
			pItemInfo->ItemKind ==eSHOP_ITEM_DECORATION||
			pItemInfo->ItemKind == eSHOP_ITEM_MAKEUP ||
			pItemInfo->ItemKind == eSHOP_ITEM_IMAGENAME||
			pItemInfo->ItemKind == eSHOP_ITEM_MARRYCHARM) 
		{
			sprintf(line, CHATMGR->GetChatMsg(1400), pItemInfo->LimitCheRyuk);
			pIcon->AddToolTipLine( line, TTTC_EXTRAATTR );
		}
		if( pItemInfo->ItemKind == eSHOP_ITEM_PET_EQUIP  )
		{
			sprintf(line, CHATMGR->GetChatMsg(1448), pItemInfo->LimitCheRyuk);
			pIcon->AddToolTipLine( line, TTTC_EXTRAATTR );
		}
	}
	if( pItemInfo->LimitSimMek != 0 )
	{
		if( pItemInfo->ItemKind == eSHOP_ITEM_CHARM ||
			pItemInfo->ItemKind == eSHOP_ITEM_IMAGENAME||
			pItemInfo->ItemKind == eSHOP_ITEM_MARRYCHARM)
		{
			sprintf(line, CHATMGR->GetChatMsg(1395), pItemInfo->LimitSimMek);
			pIcon->AddToolTipLine( line, TTTC_EXTRAATTR );
		}
		if( pItemInfo->ItemKind == eSHOP_ITEM_DECORATION||
			pItemInfo->ItemKind == eSHOP_ITEM_MAKEUP) 
		{
			sprintf(line, CHATMGR->GetChatMsg(1434), pItemInfo->LimitSimMek);
			pIcon->AddToolTipLine( line, TTTC_EXTRAATTR );
		}
		if( pItemInfo->ItemKind == eSHOP_ITEM_PET_EQUIP  )
		{
			sprintf(line, CHATMGR->GetChatMsg(1448), pItemInfo->LimitSimMek);
			pIcon->AddToolTipLine( line, TTTC_EXTRAATTR );
		}
	}
	if( pItemInfo->ItemGrade != 0 )
	{
		if( pItemInfo->ItemKind == eSHOP_ITEM_CHARM||
			pItemInfo->ItemKind == eSHOP_ITEM_IMAGENAME||
			pItemInfo->ItemKind == eSHOP_ITEM_MARRYCHARM)
		{
			sprintf(line, CHATMGR->GetChatMsg(1429), pItemInfo->ItemGrade);
			pIcon->AddToolTipLine( line, TTTC_EXTRAATTR );
		}
		if( pItemInfo->ItemKind == eSHOP_ITEM_DECORATION||
			pItemInfo->ItemKind == eSHOP_ITEM_MAKEUP) 
		{
			sprintf(line, CHATMGR->GetChatMsg(1394), pItemInfo->ItemGrade);
			pIcon->AddToolTipLine( line, TTTC_EXTRAATTR );
		}
		if( pItemInfo->ItemKind == eSHOP_ITEM_PET_EQUIP  )
		{
			sprintf(line, CHATMGR->GetChatMsg(1447), pItemInfo->ItemGrade);
			pIcon->AddToolTipLine( line, TTTC_EXTRAATTR );
		}
	}
	if( pItemInfo->RangeType != 0 )
	{
		if( pItemInfo->ItemKind == eSHOP_ITEM_CHARM ||
			pItemInfo->ItemKind == eSHOP_ITEM_IMAGENAME||
			pItemInfo->ItemKind == eSHOP_ITEM_MARRYCHARM)
		{
			sprintf(line, CHATMGR->GetChatMsg(1452), pItemInfo->RangeType);
			pIcon->AddToolTipLine( line, TTTC_EXTRAATTR );
		}
		if( pItemInfo->ItemKind == eSHOP_ITEM_DECORATION||
			pItemInfo->ItemKind == eSHOP_ITEM_MAKEUP) 
		{
			sprintf(line, CHATMGR->GetChatMsg(1405), pItemInfo->RangeType);
			pIcon->AddToolTipLine( line, TTTC_EXTRAATTR );
		}
		if( pItemInfo->ItemKind == eSHOP_ITEM_PET_EQUIP  )
		{
			sprintf(line, CHATMGR->GetChatMsg(1551), pItemInfo->RangeType);
			pIcon->AddToolTipLine( line, TTTC_EXTRAATTR );
		}
	}
	if( pItemInfo->EquipKind != 0 )
	{
		if( pItemInfo->ItemKind == eSHOP_ITEM_DECORATION||
			pItemInfo->ItemKind == eSHOP_ITEM_MAKEUP) 
		{
			sprintf(line, CHATMGR->GetChatMsg(1393), pItemInfo->RangeType);
			pIcon->AddToolTipLine( line, TTTC_EXTRAATTR );
		}
	}
	if( pItemInfo->MeleeAttackMin != 0 )
	{
		if( pItemInfo->ItemKind == eSHOP_ITEM_DECORATION||
			pItemInfo->ItemKind == eSHOP_ITEM_MAKEUP) 
		{
			sprintf(line, CHATMGR->GetChatMsg(1449), pItemInfo->MeleeAttackMin);
			pIcon->AddToolTipLine( line, TTTC_EXTRAATTR );
		}
	}
	if( pItemInfo->RangeAttackMin != 0 )
	{
		if( pItemInfo->ItemKind == eSHOP_ITEM_CHARM||
			pItemInfo->ItemKind == eSHOP_ITEM_IMAGENAME||
			pItemInfo->ItemKind == eSHOP_ITEM_MARRYCHARM)
		{
			sprintf(line, CHATMGR->GetChatMsg(1392));
			pIcon->AddToolTipLine( line, TTTC_EXTRAATTR );
		}
		if( pItemInfo->ItemKind == eSHOP_ITEM_DECORATION||
			pItemInfo->ItemKind == eSHOP_ITEM_MAKEUP) 
		{
			sprintf(line, CHATMGR->GetChatMsg(1401), pItemInfo->RangeAttackMin);
			pIcon->AddToolTipLine( line, TTTC_EXTRAATTR );
		}
	}
	if( pItemInfo->RangeAttackMax != 0 )
	{
		if( pItemInfo->ItemKind == eSHOP_ITEM_CHARM||
			pItemInfo->ItemKind == eSHOP_ITEM_IMAGENAME||
			pItemInfo->ItemKind == eSHOP_ITEM_MARRYCHARM)
		{
			sprintf(line, CHATMGR->GetChatMsg(1394), pItemInfo->RangeAttackMax);
			pIcon->AddToolTipLine( line, TTTC_EXTRAATTR );
		}
		if( pItemInfo->ItemKind == eSHOP_ITEM_DECORATION||
			pItemInfo->ItemKind == eSHOP_ITEM_MAKEUP)
		{
			sprintf(line, CHATMGR->GetChatMsg(1474), pItemInfo->RangeAttackMax);
			pIcon->AddToolTipLine( line, TTTC_EXTRAATTR );
		}
	}
	if( pItemInfo->CriticalPercent != 0 )
	{
		if( pItemInfo->ItemKind == eSHOP_ITEM_CHARM||
			pItemInfo->ItemKind == eSHOP_ITEM_IMAGENAME||
			pItemInfo->ItemKind == eSHOP_ITEM_MARRYCHARM)
		{
			sprintf(line, CHATMGR->GetChatMsg(1390), pItemInfo->CriticalPercent);
			pIcon->AddToolTipLine( line, TTTC_EXTRAATTR );
		}
		if( pItemInfo->ItemKind == eSHOP_ITEM_DECORATION||
			pItemInfo->ItemKind == eSHOP_ITEM_MAKEUP) 
		{
			sprintf(line, CHATMGR->GetChatMsg(1681), pItemInfo->CriticalPercent);
			pIcon->AddToolTipLine( line, TTTC_EXTRAATTR );
		}
	}
	if( pItemInfo->PhyDef != 0 )
	{
		if( pItemInfo->ItemKind == eSHOP_ITEM_CHARM ||
			pItemInfo->ItemKind == eSHOP_ITEM_IMAGENAME||
			pItemInfo->ItemKind == eSHOP_ITEM_MARRYCHARM)
		{
			sprintf(line, CHATMGR->GetChatMsg(1415), pItemInfo->PhyDef);
			pIcon->AddToolTipLine( line, TTTC_EXTRAATTR );
		}
	}
	if( pItemInfo->Plus_MugongIdx != 0 )
	{
		if( pItemInfo->ItemKind == eSHOP_ITEM_CHARM||
			pItemInfo->ItemKind == eSHOP_ITEM_IMAGENAME||
			pItemInfo->ItemKind == eSHOP_ITEM_MARRYCHARM)
		{
			sprintf(line, CHATMGR->GetChatMsg(1419), pItemInfo->Plus_MugongIdx);
			pIcon->AddToolTipLine( line, TTTC_EXTRAATTR );
		}
		if( pItemInfo->ItemKind == eSHOP_ITEM_DECORATION||
			pItemInfo->ItemKind == eSHOP_ITEM_MAKEUP)  
		{
			sprintf(line, CHATMGR->GetChatMsg(1407), pItemInfo->Plus_MugongIdx);
			pIcon->AddToolTipLine( line, TTTC_EXTRAATTR );
		}
	}
	if( pItemInfo->Plus_Value != 0 )
	{
		if( pItemInfo->ItemKind == eSHOP_ITEM_CHARM||
			pItemInfo->ItemKind == eSHOP_ITEM_IMAGENAME||
			pItemInfo->ItemKind == eSHOP_ITEM_MARRYCHARM)
		{
			sprintf(line, CHATMGR->GetChatMsg(1420), pItemInfo->Plus_Value);
			pIcon->AddToolTipLine( line, TTTC_EXTRAATTR );
		}
		if( pItemInfo->ItemKind == eSHOP_ITEM_DECORATION||
			pItemInfo->ItemKind == eSHOP_ITEM_MAKEUP)  
		{
			sprintf(line, CHATMGR->GetChatMsg(1408), pItemInfo->Plus_Value);
			pIcon->AddToolTipLine( line, TTTC_EXTRAATTR );
		}
	}
	if( pItemInfo->AllPlus_Kind != 0 )
	{
		if( pItemInfo->ItemKind == eSHOP_ITEM_CHARM||
			pItemInfo->ItemKind == eSHOP_ITEM_IMAGENAME||
			pItemInfo->ItemKind == eSHOP_ITEM_MARRYCHARM)
		{
			sprintf(line, CHATMGR->GetChatMsg(1418), pItemInfo->AllPlus_Kind);
			pIcon->AddToolTipLine( line, TTTC_EXTRAATTR );
		}
		if( pItemInfo->ItemKind == eSHOP_ITEM_DECORATION||
			pItemInfo->ItemKind == eSHOP_ITEM_MAKEUP) 
		{
			sprintf(line, CHATMGR->GetChatMsg(1392));
			pIcon->AddToolTipLine( line, TTTC_EXTRAATTR );
		}
	}
	if( pItemInfo->NaeRyukRecover != 0 )
	{
		if( pItemInfo->ItemKind == eSHOP_ITEM_PREMIUM || pItemInfo->ItemKind == eSHOP_ITEM_CHARM ||
			pItemInfo->ItemKind == eSHOP_ITEM_INCANTATION || pItemInfo->ItemKind == eSHOP_ITEM_MAKEUP ||
			pItemInfo->ItemKind == eSHOP_ITEM_MAKEUP || pItemInfo->ItemKind == eSHOP_ITEM_DECORATION ||
			pItemInfo->ItemKind == eSHOP_ITEM_EQUIP || pItemInfo->ItemKind == eSHOP_ITEM_PET_EQUIP||
			pItemInfo->ItemKind == eSHOP_ITEM_IMAGENAME||
			pItemInfo->ItemKind == eSHOP_ITEM_MARRYCHARM)  
		{
			sprintf(line, CHATMGR->GetChatMsg(1682), pItemInfo->NaeRyukRecover);
			pIcon->AddToolTipLine( line, TTTC_EXTRAATTR );
		}
	}
	if( pItemInfo->BuyPrice  == 1 )
	{
		if( pItemInfo->ItemKind == eSHOP_ITEM_EQUIP || pItemInfo->ItemKind == eSHOP_ITEM_PET )
		{
			sprintf(line, CHATMGR->GetChatMsg(1391), CHATMGR->GetChatMsg(1465));
			pIcon->AddToolTipLine( line, TTTC_EXTRAATTR );
		}
	}
	if( pItemInfo->BuyPrice  == 0 )
	{
		if( pItemInfo->ItemKind == eSHOP_ITEM_EQUIP || pItemInfo->ItemKind == eSHOP_ITEM_PET )
		{
			if( pItemInfo->ItemType == 11 )
			{
				if( !(((CItem*)pIcon)->GetItemBaseInfo()->ItemParam & ITEM_PARAM_SEAL) )
					sprintf(line, CHATMGR->GetChatMsg(1391), CHATMGR->GetChatMsg(1465));
				else
					sprintf(line, CHATMGR->GetChatMsg(1391), CHATMGR->GetChatMsg(1464));
			}
			else
				sprintf(line, CHATMGR->GetChatMsg(1391), CHATMGR->GetChatMsg(1464));

			pIcon->AddToolTipLine( line, TTTC_EXTRAATTR );
		}
	}
	if( pItemInfo->MeleeAttackMax == 0 )
	{
		if( pItemInfo->ItemKind == eSHOP_ITEM_EQUIP || pItemInfo->ItemKind == eSHOP_ITEM_PET )
		{

			if( pItemInfo->ItemType == 11 )
			{
				if( !(((CItem*)pIcon)->GetItemBaseInfo()->ItemParam & ITEM_PARAM_SEAL) )
					sprintf(line, CHATMGR->GetChatMsg(1391), CHATMGR->GetChatMsg(1465));
				else
					sprintf(line, CHATMGR->GetChatMsg(1391), CHATMGR->GetChatMsg(1464));
			}
			else
				sprintf(line, CHATMGR->GetChatMsg(1391), CHATMGR->GetChatMsg(1464));

			pIcon->AddToolTipLine( line, TTTC_EXTRAATTR );
		}
	}
	if( pItemInfo->MeleeAttackMax == 1 )
	{
		if( pItemInfo->ItemKind == eSHOP_ITEM_EQUIP || pItemInfo->ItemKind == eSHOP_ITEM_PET )
		{
			sprintf(line, CHATMGR->GetChatMsg(1391), CHATMGR->GetChatMsg(1465));
			pIcon->AddToolTipLine( line, TTTC_EXTRAATTR );
		}
	}
	//[分割线][By:十里坡剑传奇][QQ:112582793][2017/11/28]
//	pIcon->AddImagTopLine(&imgTopLine,pIcon->GetTipLineCount());
		//天墨技术团 PVP妮┦陪ボ
	float fPVP = 100 * pItemInfo->PVPCri;
	if (fPVP != 0)
	{
		pIcon->AddToolTipLine("");   // 添一空行
		sprintf(line, "< %s>", CHATMGR->GetChatMsg(2735));
		pIcon->AddToolTipLine(line, RGB_HALF(255, 0, 0));
		sprintf(line, CHATMGR->GetChatMsg(2729), (int)fPVP);
		pIcon->AddToolTipLine(line, RGB_HALF(0, 255, 0));
}
	fPVP = 100 * pItemInfo->PVPAttack;
	if (fPVP != 0)
	{
		sprintf(line, CHATMGR->GetChatMsg(2730), (int)fPVP);
		pIcon->AddToolTipLine(line, RGB_HALF(0, 255, 0));
	}
	fPVP = 100 * pItemInfo->PVPDef;
	if (fPVP != 0)
	{
		sprintf(line, CHATMGR->GetChatMsg(2731), (int)fPVP);
		pIcon->AddToolTipLine(line, RGB_HALF(0, 255, 0));
	}
	fPVP = 100 * pItemInfo->PVPADef;
	if (fPVP != 0)
	{
		sprintf(line, CHATMGR->GetChatMsg(2732), (int)fPVP);
		pIcon->AddToolTipLine(line, RGB_HALF(0, 255, 0));
	}
	fPVP = 100 * pItemInfo->PVPHit;
	if (fPVP != 0)
	{
		sprintf(line, CHATMGR->GetChatMsg(2733), (int)fPVP);
		pIcon->AddToolTipLine(line, RGB_HALF(0, 255, 0));
	}
	fPVP = 100 * pItemInfo->PVPADodge;
	if (fPVP != 0)
	{
		sprintf(line, CHATMGR->GetChatMsg(2734), (int)fPVP);
		pIcon->AddToolTipLine(line, RGB_HALF(0, 255, 0));
	}
	fPVP = 100 * pItemInfo->PVPStunResist;
	if (fPVP != 0)
	{
		sprintf(line, CHATMGR->GetChatMsg(2736), (int)fPVP);
		pIcon->AddToolTipLine(line, RGB_HALF(0, 255, 0));
	}
	fPVP = 100 * pItemInfo->PVPStunTimeReduce;
	if (fPVP != 0)
	{
		sprintf(line, CHATMGR->GetChatMsg(2737), (int)fPVP);
		pIcon->AddToolTipLine(line, RGB_HALF(0, 255, 0));
	}
}

void CItemManager::SetExtraItemToolTip( cIcon* pIcon, ITEM_INFO* pInfo )
{
	CExchangeItem* pExcItem = NULL;
	CItem * pcItem = NULL;
	if( pIcon->GetType() == WT_EXCHANGEITEM || pIcon->GetType() == WT_STALLITEM || pInfo->ItemKind == eSHOP_ITEM_PET ||pIcon->GetType() == WT_BUYITE)
		pExcItem = (CExchangeItem*)pIcon;
	else
		pcItem = (CItem*)pIcon;

	char line[128]= {0,};

	switch( pInfo->ItemKind )
	{
	case eEXTRA_ITEM_JEWEL:				sprintf(line,"          %s",CHATMGR->GetChatMsg(2207)); break;  
	case eEXTRA_ITEM_MATERIAL:			sprintf(line,"          %s",CHATMGR->GetChatMsg(2208)); break;
	case eEXTRA_ITEM_METAL:				sprintf(line,"          %s",CHATMGR->GetChatMsg(2209)); break;
	case eEXTRA_ITEM_BOOK:				sprintf(line,"          %s",CHATMGR->GetChatMsg(2210)); break;
	case eEXTRA_ITEM_HERB:				sprintf(line,"          %s",CHATMGR->GetChatMsg(2211)); break;
	case eEXTRA_ITEM_ETC:				sprintf(line,"          %s",CHATMGR->GetChatMsg(2212)); break;
	case eEXTRA_ITEM_UPGRADE_ATTACK:	sprintf(line,"          %s",CHATMGR->GetChatMsg(2213)); break;
	case eEXTRA_ITEM_UPGRADE_DEFENSE:	sprintf(line,"          %s",CHATMGR->GetChatMsg(2214)); break;
	case eEXTRA_ITEM_COMP:				sprintf(line,"          %s",CHATMGR->GetChatMsg(2215)); break;
	case eEXTRA_ITEM_QUEST:				sprintf(line,"          %s",CHATMGR->GetChatMsg(2216)); break;
#ifdef _ITEMFUSE
	case eEXTRA_ITEM_FUSE:				sprintf(line,"          %s",CHATMGR->GetChatMsg(2487)); break;
#endif
	}
	pIcon->AddToolTipLine(line,TTTC_ITEM_LEVEL);
	pIcon->AddToolTipLine("");
	AddItemDescriptionToolTip( pIcon, pInfo->ItemTooltipIdx );
	//pIcon->AddToolTipLine(JACKJACK);
}
void CItemManager::SetYoungyakItemToolTip( cIcon* pIcon, ITEM_INFO* pInfo )
{
	CExchangeItem* pExcItem = NULL;
	CItem * pcItem = NULL;
	if( pIcon->GetType() == WT_EXCHANGEITEM || pIcon->GetType() == WT_STALLITEM || pInfo->ItemKind == eSHOP_ITEM_PET ||pIcon->GetType() == WT_BUYITE)
		pExcItem = (CExchangeItem*)pIcon;
	else
		pcItem = (CItem*)pIcon;
	char line[128];
	//wsprintf( line, "[%s]", pInfo->ItemName );
   // pIcon->AddToolTipLine( line,TTTC_DEFAULT,NULL,6);
	//pIcon->AddToolTipLine( "" );
	sprintf(line,CHATMGR->GetChatMsg(2171),CHATMGR->GetChatMsg(2217)); 
	pIcon->AddToolTipLine(line,TTTC_ITEM_LEVEL);
	pIcon->AddToolTipLine( "" );
	pIcon->AddToolTipLine( JACKJACK );
	if( pInfo->GenGol != 0)
	{
		wsprintf(line, "%s +%d", CHATMGR->GetChatMsg(261), pInfo->GenGol);
		pIcon->AddToolTipLine( line, TTTC_EXTRAATTR );
	}
	if( pInfo->MinChub != 0)
	{
		wsprintf(line, "%s +%d", CHATMGR->GetChatMsg(262), pInfo->MinChub);
		pIcon->AddToolTipLine( line, TTTC_EXTRAATTR );
	}
	if( pInfo->CheRyuk != 0)
	{
		wsprintf(line, "%s +%d", CHATMGR->GetChatMsg(263), pInfo->CheRyuk);
		pIcon->AddToolTipLine( line, TTTC_EXTRAATTR );
	}
	if( pInfo->SimMek != 0)
	{
		wsprintf(line, "%s +%d", CHATMGR->GetChatMsg(264), pInfo->SimMek);
		pIcon->AddToolTipLine( line, TTTC_EXTRAATTR );
	}
	if( (int)(pInfo->AttrRegist.GetElement_Val(ATTR_FIRE)) !=0 )
	{
		wsprintf(line, "%s +%d%%", CHATMGR->GetChatMsg(265), (int)(pInfo->AttrRegist.GetElement_Val(ATTR_FIRE)));
		pIcon->AddToolTipLine( line, TTTC_EXTRAATTR );
	}
	if( (int)(pInfo->AttrRegist.GetElement_Val(ATTR_WATER)) !=0 )
	{
		wsprintf(line, "%s +%d%%", CHATMGR->GetChatMsg(266), (int)(pInfo->AttrRegist.GetElement_Val(ATTR_WATER)));
		pIcon->AddToolTipLine( line, TTTC_EXTRAATTR );
	}
	if( (int)(pInfo->AttrRegist.GetElement_Val(ATTR_TREE)) !=0 )
	{
		wsprintf(line, "%s +%d%%", CHATMGR->GetChatMsg(267), (int)(pInfo->AttrRegist.GetElement_Val(ATTR_TREE)));
		pIcon->AddToolTipLine( line, TTTC_EXTRAATTR );
	}
	if( (int)(pInfo->AttrRegist.GetElement_Val(ATTR_IRON)) !=0 )
	{
		wsprintf(line, "%s +%d%%", CHATMGR->GetChatMsg(268), (int)(pInfo->AttrRegist.GetElement_Val(ATTR_IRON)));
		pIcon->AddToolTipLine( line, TTTC_EXTRAATTR );
	}
	if( (int)(pInfo->AttrRegist.GetElement_Val(ATTR_EARTH)) !=0 )
	{
		wsprintf(line, "%s +%d%%", CHATMGR->GetChatMsg(269), (int)(pInfo->AttrRegist.GetElement_Val(ATTR_EARTH)));
		pIcon->AddToolTipLine( line, TTTC_EXTRAATTR );
	}
	if( pInfo->LifeRecover !=0 )
	{
		if(pInfo->ItemKind == eYOUNGYAK_ITEM_PET)
		{
			wsprintf(line, "%s %d", CHATMGR->GetChatMsg(1276), pInfo->LifeRecover);
		}
		else
		{
			wsprintf(line, "%s %d", CHATMGR->GetChatMsg(270), pInfo->LifeRecover);
		}
		pIcon->AddToolTipLine( line, TTTC_EXTRAATTR );
	}
	if( (int)(pInfo->LifeRecoverRate*100) !=0 )
	{
		wsprintf(line, "%s %d%%", CHATMGR->GetChatMsg(270), (int)(pInfo->LifeRecoverRate*100));
		pIcon->AddToolTipLine( line, TTTC_EXTRAATTR );
	}
	if( pInfo->NaeRyukRecover !=0 )
	{
		wsprintf(line, "%s %d", CHATMGR->GetChatMsg(271), pInfo->NaeRyukRecover);
		pIcon->AddToolTipLine( line, TTTC_EXTRAATTR );
	}
	if( (int)(pInfo->NaeRyukRecoverRate*100) !=0 )
	{
		wsprintf(line, "%s %d%%", CHATMGR->GetChatMsg(271), (int)(pInfo->NaeRyukRecoverRate*100));
		pIcon->AddToolTipLine( line, TTTC_EXTRAATTR );
	}
	if( pInfo->ItemIdx == 53102 )
	{
		wsprintf(line, "%s %d", CHATMGR->GetChatMsg(270), 1000);	
		pIcon->AddToolTipLine( line, TTTC_EXTRAATTR );
		wsprintf(line, "%s %d", CHATMGR->GetChatMsg(271), 1000);	
		pIcon->AddToolTipLine( line, TTTC_EXTRAATTR );
		wsprintf(line, "%s %d", CHATMGR->GetChatMsg(181), 1000);	
		pIcon->AddToolTipLine( line, TTTC_EXTRAATTR );
	}
	else if( pInfo->ItemIdx == 53031 || pInfo->ItemIdx == 53094 )
	{
		wsprintf(line, "%s %d%%", CHATMGR->GetChatMsg(181), 100);
		pIcon->AddToolTipLine( line, TTTC_EXTRAATTR );
	}
	else if( pInfo->ItemIdx == 53216 || pInfo->ItemIdx == 53222 )
	{
		wsprintf(line, "%s %d%%", CHATMGR->GetChatMsg(181), 100);
		pIcon->AddToolTipLine( line, TTTC_EXTRAATTR );
	}
	else if( pInfo->ItemIdx == 53032 || pInfo->ItemIdx == 53095 
		|| pInfo->ItemIdx == 53103 || pInfo->ItemIdx == 53217 || pInfo->ItemIdx == 53223)
	{
		wsprintf(line, "%s %d%%", CHATMGR->GetChatMsg(270), 100);
		pIcon->AddToolTipLine( line, TTTC_EXTRAATTR );
		wsprintf(line, "%s %d%%", CHATMGR->GetChatMsg(271), 100);
		pIcon->AddToolTipLine( line, TTTC_EXTRAATTR );
		wsprintf(line, "%s %d%%", CHATMGR->GetChatMsg(181), 100);
		pIcon->AddToolTipLine( line, TTTC_EXTRAATTR );
	}	
	AddItemDescriptionToolTip( pIcon, pInfo->ItemTooltipIdx );
}
void CItemManager::SetMugongItemToolTip( cIcon* pIcon, ITEM_INFO* pInfo )
{
	CExchangeItem* pExcItem = NULL;
	CItem * pcItem = NULL;
	if( pIcon->GetType() == WT_EXCHANGEITEM || pIcon->GetType() == WT_STALLITEM || pInfo->ItemKind == eSHOP_ITEM_PET ||pIcon->GetType() == WT_BUYITE)
		pExcItem = (CExchangeItem*)pIcon;
	else
		pcItem = (CItem*)pIcon;
	//char line[128];
	char line[128];
	char line2[128];
	char line3[128];
	//wsprintf( line, "[%s]", pInfo->ItemName );
	//pIcon->AddToolTipLine( line,TTTC_DEFAULT,NULL,6);
	//pIcon->AddToolTipLine(JACKJACK);
	switch( pInfo->ItemKind )
	{
	case 1025:						{wsprintf(line,CHATMGR->GetChatMsg(2218) ,CHATMGR->GetChatMsg(2217),CHATMGR->GetChatMsg(274)); }break;
	case 1026:						{wsprintf(line,CHATMGR->GetChatMsg(2218) ,CHATMGR->GetChatMsg(2217),CHATMGR->GetChatMsg(275)); }break;
	case 1027:						{wsprintf(line,CHATMGR->GetChatMsg(2218) ,CHATMGR->GetChatMsg(2217),CHATMGR->GetChatMsg(276)); }break;
	case 1028:						{wsprintf(line,CHATMGR->GetChatMsg(2218) ,CHATMGR->GetChatMsg(2217),CHATMGR->GetChatMsg(277)); }break;
	case 1029:						{wsprintf(line,CHATMGR->GetChatMsg(2218) ,CHATMGR->GetChatMsg(2217),CHATMGR->GetChatMsg(278)); }break;
	case 1030:						{wsprintf(line,CHATMGR->GetChatMsg(2218) ,CHATMGR->GetChatMsg(2217),CHATMGR->GetChatMsg(283)); }break;
	case 1031:						{wsprintf(line,CHATMGR->GetChatMsg(2218) ,CHATMGR->GetChatMsg(2217),CHATMGR->GetChatMsg(284)); }break;
	case 1032:						{wsprintf(line,CHATMGR->GetChatMsg(2218) ,CHATMGR->GetChatMsg(2217),CHATMGR->GetChatMsg(285)); }break;
	case 1033:						{wsprintf(line,CHATMGR->GetChatMsg(2218) ,CHATMGR->GetChatMsg(2217),CHATMGR->GetChatMsg(286)); }break;
	case 1034:						{wsprintf(line,CHATMGR->GetChatMsg(2218) ,CHATMGR->GetChatMsg(2217),CHATMGR->GetChatMsg(287)); }break;
	case 1035:						{wsprintf(line,CHATMGR->GetChatMsg(2218) ,CHATMGR->GetChatMsg(2217),CHATMGR->GetChatMsg(288)); }break;
	case 1036:						{wsprintf(line,CHATMGR->GetChatMsg(2218) ,CHATMGR->GetChatMsg(2217),CHATMGR->GetChatMsg(289)); }break;
	case eMUGONG_ITEM_SIMBUB:		{wsprintf(line,CHATMGR->GetChatMsg(2218) ,CHATMGR->GetChatMsg(2217),CHATMGR->GetChatMsg(243)); }break;
	case eMUGONG_ITEM_JINBUB:		{wsprintf(line,CHATMGR->GetChatMsg(2218) ,CHATMGR->GetChatMsg(2217),CHATMGR->GetChatMsg(244)); }break;
	case eMUGONG_ITEM_KYUNGGONG:	{wsprintf(line,CHATMGR->GetChatMsg(2218) ,CHATMGR->GetChatMsg(2217),CHATMGR->GetChatMsg(245)); }break;
	case 1040:						{wsprintf(line,CHATMGR->GetChatMsg(2218) ,CHATMGR->GetChatMsg(2217),CHATMGR->GetChatMsg(1362)); }break;
	case eMUGONG_ITEM_JOB:			{wsprintf(line,CHATMGR->GetChatMsg(2218) ,CHATMGR->GetChatMsg(2217),CHATMGR->GetChatMsg(1669)); }break;		
	}

	pIcon->AddToolTipLine(line,TTTC_ITEM_LEVEL);

	//wsprintf(line2, "          %s%s","test", line);
	//wsprintf(line3, "                               %s",line2);
	//pIcon->AddToolTipLine2( line, line, TTTC_ITEM_LEVEL,TTTC_FREELIMIT,NULL,0,1 );
//--------------------------------------------------
	SetItemToolTipForStage( pIcon, pInfo );
//--------------------------------------------------
//	BYTE bStage = HERO->GetStage();
//	switch( bStage )
//	{
//	case eStage_Normal:
//		{
//			switch( pInfo->LimitJob )
//			{
//			case eItemStage_ChangeStage:	pIcon->AddToolTipLine2( line, CHATMGR->GetChatMsg(888), TTTC_ITEM_LEVEL,TTTC_LIMIT,NULL,0,1 );/*pIcon->AddToolTipLine( CHATMGR->GetChatMsg(888),	TTTC_LIMIT );*/	break;
//			case eItemStage_ChangeStage2:	pIcon->AddToolTipLine2( line, CHATMGR->GetChatMsg(1354), TTTC_ITEM_LEVEL,TTTC_LIMIT,NULL,0,1 );/*pIcon->AddToolTipLine( CHATMGR->GetChatMsg(1354),	TTTC_LIMIT );*/	break;
//			case eItemStage_OverHwa:		pIcon->AddToolTipLine2( line, CHATMGR->GetChatMsg(894), TTTC_ITEM_LEVEL,TTTC_LIMIT,NULL,0,1 );/*pIcon->AddToolTipLine( CHATMGR->GetChatMsg(894),	TTTC_LIMIT );*/	break;
//			case eItemStage_Hwa:			pIcon->AddToolTipLine2( line, CHATMGR->GetChatMsg(895), TTTC_ITEM_LEVEL,TTTC_LIMIT,NULL,0,1 );/*pIcon->AddToolTipLine( CHATMGR->GetChatMsg(895),	TTTC_LIMIT );*/	break;
//			case eItemStage_Hyun:			pIcon->AddToolTipLine2( line, CHATMGR->GetChatMsg(896), TTTC_ITEM_LEVEL,TTTC_LIMIT,NULL,0,1 );/*pIcon->AddToolTipLine( CHATMGR->GetChatMsg(896),	TTTC_LIMIT );*/	break;
//			case eItemStage_OverGeuk:		pIcon->AddToolTipLine2( line, CHATMGR->GetChatMsg(897), TTTC_ITEM_LEVEL,TTTC_LIMIT,NULL,0,1 );/*pIcon->AddToolTipLine( CHATMGR->GetChatMsg(897),	TTTC_LIMIT );*/	break;
//			case eItemStage_Geuk:			pIcon->AddToolTipLine2( line, CHATMGR->GetChatMsg(898), TTTC_ITEM_LEVEL,TTTC_LIMIT,NULL,0,1 );/*pIcon->AddToolTipLine( CHATMGR->GetChatMsg(898),	TTTC_LIMIT );*/	break;
//			case eItemStage_Tal:			pIcon->AddToolTipLine2( line, CHATMGR->GetChatMsg(899), TTTC_ITEM_LEVEL,TTTC_LIMIT,NULL,0,1 );/*pIcon->AddToolTipLine( CHATMGR->GetChatMsg(899),	TTTC_LIMIT );*/	break;
//			}
//		}
//		break;
//	case eStage_Hwa:
//		{
//			switch( pInfo->LimitJob )
//			{
//			case eItemStage_ChangeStage:	pIcon->AddToolTipLine2( line, CHATMGR->GetChatMsg(888), TTTC_ITEM_LEVEL,TTTC_FREELIMIT,NULL,0,1 );/*pIcon->AddToolTipLine( CHATMGR->GetChatMsg(888),	TTTC_LIMIT );*/	break;
//			case eItemStage_ChangeStage2:	pIcon->AddToolTipLine2( line, CHATMGR->GetChatMsg(1354), TTTC_ITEM_LEVEL,TTTC_LIMIT,NULL,0,1 );/*pIcon->AddToolTipLine( CHATMGR->GetChatMsg(1354),	TTTC_LIMIT );*/	break;
//			case eItemStage_OverHwa:		pIcon->AddToolTipLine2( line, CHATMGR->GetChatMsg(894), TTTC_ITEM_LEVEL,TTTC_FREELIMIT,NULL,0,1 );/*pIcon->AddToolTipLine( CHATMGR->GetChatMsg(894),	TTTC_LIMIT );*/	break;
//			case eItemStage_Hwa:			pIcon->AddToolTipLine2( line, CHATMGR->GetChatMsg(895), TTTC_ITEM_LEVEL,TTTC_FREELIMIT,NULL,0,1 );/*pIcon->AddToolTipLine( CHATMGR->GetChatMsg(895),	TTTC_LIMIT );*/	break;
//			case eItemStage_Hyun:			pIcon->AddToolTipLine2( line, CHATMGR->GetChatMsg(896), TTTC_ITEM_LEVEL,TTTC_LIMIT,NULL,0,1 );/*pIcon->AddToolTipLine( CHATMGR->GetChatMsg(896),	TTTC_LIMIT );*/	break;
//			case eItemStage_OverGeuk:		pIcon->AddToolTipLine2( line, CHATMGR->GetChatMsg(897), TTTC_ITEM_LEVEL,TTTC_LIMIT,NULL,0,1 );/*pIcon->AddToolTipLine( CHATMGR->GetChatMsg(897),	TTTC_LIMIT );*/	break;
//			case eItemStage_Geuk:			pIcon->AddToolTipLine2( line, CHATMGR->GetChatMsg(898), TTTC_ITEM_LEVEL,TTTC_LIMIT,NULL,0,1 );/*pIcon->AddToolTipLine( CHATMGR->GetChatMsg(898),	TTTC_LIMIT );*/	break;
//			case eItemStage_Tal:			pIcon->AddToolTipLine2( line, CHATMGR->GetChatMsg(899), TTTC_ITEM_LEVEL,TTTC_LIMIT,NULL,0,1 );/*pIcon->AddToolTipLine( CHATMGR->GetChatMsg(899),	TTTC_LIMIT );*/	break;
//			/*case eItemStage_ChangeStage:	pIcon->AddToolTipLine( CHATMGR->GetChatMsg(888), TTTC_FREELIMIT );	break;
//			case eItemStage_ChangeStage2:	pIcon->AddToolTipLine( CHATMGR->GetChatMsg(1354), TTTC_LIMIT);	break;
//			case eItemStage_OverHwa:	pIcon->AddToolTipLine( CHATMGR->GetChatMsg(894), TTTC_FREELIMIT );	break;
//			case eItemStage_Hwa:		pIcon->AddToolTipLine( CHATMGR->GetChatMsg(895), TTTC_FREELIMIT );	break;
//			case eItemStage_Hyun:		pIcon->AddToolTipLine( CHATMGR->GetChatMsg(896), TTTC_LIMIT );	break;
//			case eItemStage_OverGeuk:	pIcon->AddToolTipLine( CHATMGR->GetChatMsg(897), TTTC_LIMIT );	break;
//			case eItemStage_Geuk:		pIcon->AddToolTipLine( CHATMGR->GetChatMsg(898), TTTC_LIMIT );	break;
//			case eItemStage_Tal:		pIcon->AddToolTipLine( CHATMGR->GetChatMsg(899), TTTC_LIMIT );	break;*/
//			}
//		}
//		break;
//	case eStage_Hyun:
//		{
//			switch( pInfo->LimitJob )
//			{
//			case eItemStage_ChangeStage:	pIcon->AddToolTipLine2( line, CHATMGR->GetChatMsg(888), TTTC_ITEM_LEVEL,TTTC_FREELIMIT,NULL,0,1 );/*pIcon->AddToolTipLine( CHATMGR->GetChatMsg(888),	TTTC_LIMIT );*/	break;
//			case eItemStage_ChangeStage2:	pIcon->AddToolTipLine2( line, CHATMGR->GetChatMsg(1354), TTTC_ITEM_LEVEL,TTTC_FREELIMIT,NULL,0,1 );/*pIcon->AddToolTipLine( CHATMGR->GetChatMsg(1354),	TTTC_LIMIT );*/	break;
//			case eItemStage_OverHwa:		pIcon->AddToolTipLine2( line, CHATMGR->GetChatMsg(894), TTTC_ITEM_LEVEL,TTTC_FREELIMIT,NULL,0,1 );/*pIcon->AddToolTipLine( CHATMGR->GetChatMsg(894),	TTTC_LIMIT );*/	break;
//			case eItemStage_Hwa:			pIcon->AddToolTipLine2( line, CHATMGR->GetChatMsg(895), TTTC_ITEM_LEVEL,TTTC_FREELIMIT,NULL,0,1 );/*pIcon->AddToolTipLine( CHATMGR->GetChatMsg(895),	TTTC_LIMIT );*/	break;
//			case eItemStage_Hyun:			pIcon->AddToolTipLine2( line, CHATMGR->GetChatMsg(896), TTTC_ITEM_LEVEL,TTTC_FREELIMIT,NULL,0,1 );/*pIcon->AddToolTipLine( CHATMGR->GetChatMsg(896),	TTTC_LIMIT );*/	break;
//			case eItemStage_OverGeuk:		pIcon->AddToolTipLine2( line, CHATMGR->GetChatMsg(897), TTTC_ITEM_LEVEL,TTTC_LIMIT,NULL,0,1 );/*pIcon->AddToolTipLine( CHATMGR->GetChatMsg(897),	TTTC_LIMIT );*/	break;
//			case eItemStage_Geuk:			pIcon->AddToolTipLine2( line, CHATMGR->GetChatMsg(898), TTTC_ITEM_LEVEL,TTTC_LIMIT,NULL,0,1 );/*pIcon->AddToolTipLine( CHATMGR->GetChatMsg(898),	TTTC_LIMIT );*/	break;
//			case eItemStage_Tal:			pIcon->AddToolTipLine2( line, CHATMGR->GetChatMsg(899), TTTC_ITEM_LEVEL,TTTC_LIMIT,NULL,0,1 );/*pIcon->AddToolTipLine( CHATMGR->GetChatMsg(899),	TTTC_LIMIT );*/	break;
//			/*case eItemStage_ChangeStage:	pIcon->AddToolTipLine( CHATMGR->GetChatMsg(888), TTTC_FREELIMIT );	break;
//			case eItemStage_ChangeStage2:	pIcon->AddToolTipLine( CHATMGR->GetChatMsg(1354), TTTC_FREELIMIT);	break;
//			case eItemStage_OverHwa:	pIcon->AddToolTipLine( CHATMGR->GetChatMsg(894), TTTC_FREELIMIT );	break;
//			case eItemStage_Hwa:		pIcon->AddToolTipLine( CHATMGR->GetChatMsg(895), TTTC_FREELIMIT );	break;
//			case eItemStage_Hyun:		pIcon->AddToolTipLine( CHATMGR->GetChatMsg(896), TTTC_FREELIMIT );	break;
//			case eItemStage_OverGeuk:	pIcon->AddToolTipLine( CHATMGR->GetChatMsg(897), TTTC_LIMIT );	break;
//			case eItemStage_Geuk:		pIcon->AddToolTipLine( CHATMGR->GetChatMsg(898), TTTC_LIMIT );	break;
//			case eItemStage_Tal:		pIcon->AddToolTipLine( CHATMGR->GetChatMsg(899), TTTC_LIMIT );	break;*/
//			}
//		}
//		break;
//	case eStage_Geuk:
//		{
//			switch( pInfo->LimitJob )
//			{
//			case eItemStage_ChangeStage:	pIcon->AddToolTipLine2( line, CHATMGR->GetChatMsg(888), TTTC_ITEM_LEVEL,TTTC_FREELIMIT,NULL,0,1 );/*pIcon->AddToolTipLine( CHATMGR->GetChatMsg(888),	TTTC_LIMIT );*/	break;
//			case eItemStage_ChangeStage2:	pIcon->AddToolTipLine2( line, CHATMGR->GetChatMsg(1354), TTTC_ITEM_LEVEL,TTTC_LIMIT,NULL,0,1 );/*pIcon->AddToolTipLine( CHATMGR->GetChatMsg(1354),	TTTC_LIMIT );*/	break;
//			case eItemStage_OverHwa:		pIcon->AddToolTipLine2( line, CHATMGR->GetChatMsg(894), TTTC_ITEM_LEVEL,TTTC_LIMIT,NULL,0,1 );/*pIcon->AddToolTipLine( CHATMGR->GetChatMsg(894),	TTTC_LIMIT );*/	break;
//			case eItemStage_Hwa:			pIcon->AddToolTipLine2( line, CHATMGR->GetChatMsg(895), TTTC_ITEM_LEVEL,TTTC_LIMIT,NULL,0,1 );/*pIcon->AddToolTipLine( CHATMGR->GetChatMsg(895),	TTTC_LIMIT );*/	break;
//			case eItemStage_Hyun:			pIcon->AddToolTipLine2( line, CHATMGR->GetChatMsg(896), TTTC_ITEM_LEVEL,TTTC_LIMIT,NULL,0,1 );/*pIcon->AddToolTipLine( CHATMGR->GetChatMsg(896),	TTTC_LIMIT );*/	break;
//			case eItemStage_OverGeuk:		pIcon->AddToolTipLine2( line, CHATMGR->GetChatMsg(897), TTTC_ITEM_LEVEL,TTTC_FREELIMIT,NULL,0,1 );/*pIcon->AddToolTipLine( CHATMGR->GetChatMsg(897),	TTTC_LIMIT );*/	break;
//			case eItemStage_Geuk:			pIcon->AddToolTipLine2( line, CHATMGR->GetChatMsg(898), TTTC_ITEM_LEVEL,TTTC_FREELIMIT,NULL,0,1 );/*pIcon->AddToolTipLine( CHATMGR->GetChatMsg(898),	TTTC_LIMIT );*/	break;
//			case eItemStage_Tal:			pIcon->AddToolTipLine2( line, CHATMGR->GetChatMsg(899), TTTC_ITEM_LEVEL,TTTC_LIMIT,NULL,0,1 );/*pIcon->AddToolTipLine( CHATMGR->GetChatMsg(899),	TTTC_LIMIT );*/	break;
//			/*case eItemStage_ChangeStage:	pIcon->AddToolTipLine( CHATMGR->GetChatMsg(888), TTTC_FREELIMIT );	break;
//			case eItemStage_ChangeStage2:	pIcon->AddToolTipLine( CHATMGR->GetChatMsg(1354), TTTC_LIMIT);	break;
//			case eItemStage_OverHwa:	pIcon->AddToolTipLine( CHATMGR->GetChatMsg(894), TTTC_LIMIT );	break;
//			case eItemStage_Hwa:		pIcon->AddToolTipLine( CHATMGR->GetChatMsg(895), TTTC_LIMIT );	break;
//			case eItemStage_Hyun:		pIcon->AddToolTipLine( CHATMGR->GetChatMsg(896), TTTC_LIMIT );	break;
//			case eItemStage_OverGeuk:	pIcon->AddToolTipLine( CHATMGR->GetChatMsg(897), TTTC_FREELIMIT );	break;
//			case eItemStage_Geuk:		pIcon->AddToolTipLine( CHATMGR->GetChatMsg(898), TTTC_FREELIMIT );	break;
//			case eItemStage_Tal:		pIcon->AddToolTipLine( CHATMGR->GetChatMsg(899), TTTC_LIMIT );	break;*/
//			}
//		}
//		break;
//	case eStage_Tal:
//		{
//			switch( pInfo->LimitJob )
//			{
//			case eItemStage_ChangeStage:	pIcon->AddToolTipLine2( line, CHATMGR->GetChatMsg(888), TTTC_ITEM_LEVEL,TTTC_FREELIMIT,NULL,0,1 );/*pIcon->AddToolTipLine( CHATMGR->GetChatMsg(888),	TTTC_LIMIT );*/	break;
//			case eItemStage_ChangeStage2:	pIcon->AddToolTipLine2( line, CHATMGR->GetChatMsg(1354), TTTC_ITEM_LEVEL,TTTC_FREELIMIT,NULL,0,1 );/*pIcon->AddToolTipLine( CHATMGR->GetChatMsg(1354),	TTTC_LIMIT );*/	break;
//			case eItemStage_OverHwa:		pIcon->AddToolTipLine2( line, CHATMGR->GetChatMsg(894), TTTC_ITEM_LEVEL,TTTC_LIMIT,NULL,0,1 );/*pIcon->AddToolTipLine( CHATMGR->GetChatMsg(894),	TTTC_LIMIT );*/	break;
//			case eItemStage_Hwa:			pIcon->AddToolTipLine2( line, CHATMGR->GetChatMsg(895), TTTC_ITEM_LEVEL,TTTC_LIMIT,NULL,0,1 );/*pIcon->AddToolTipLine( CHATMGR->GetChatMsg(895),	TTTC_LIMIT );*/	break;
//			case eItemStage_Hyun:			pIcon->AddToolTipLine2( line, CHATMGR->GetChatMsg(896), TTTC_ITEM_LEVEL,TTTC_LIMIT,NULL,0,1 );/*pIcon->AddToolTipLine( CHATMGR->GetChatMsg(896),	TTTC_LIMIT );*/	break;
//			case eItemStage_OverGeuk:		pIcon->AddToolTipLine2( line, CHATMGR->GetChatMsg(897), TTTC_ITEM_LEVEL,TTTC_FREELIMIT,NULL,0,1 );/*pIcon->AddToolTipLine( CHATMGR->GetChatMsg(897),	TTTC_LIMIT );*/	break;
//			case eItemStage_Geuk:			pIcon->AddToolTipLine2( line, CHATMGR->GetChatMsg(898), TTTC_ITEM_LEVEL,TTTC_FREELIMIT,NULL,0,1 );/*pIcon->AddToolTipLine( CHATMGR->GetChatMsg(898),	TTTC_LIMIT );*/	break;
//			case eItemStage_Tal:			pIcon->AddToolTipLine2( line, CHATMGR->GetChatMsg(899), TTTC_ITEM_LEVEL,TTTC_FREELIMIT,NULL,0,1 );/*pIcon->AddToolTipLine( CHATMGR->GetChatMsg(899),	TTTC_LIMIT );*/	break;
//			/*case eItemStage_ChangeStage:	pIcon->AddToolTipLine( CHATMGR->GetChatMsg(888), TTTC_FREELIMIT );	break;
//			case eItemStage_ChangeStage2:	pIcon->AddToolTipLine( CHATMGR->GetChatMsg(1354), TTTC_FREELIMIT);	break;
//			case eItemStage_OverHwa:	pIcon->AddToolTipLine( CHATMGR->GetChatMsg(894), TTTC_LIMIT );	break;
//			case eItemStage_Hwa:		pIcon->AddToolTipLine( CHATMGR->GetChatMsg(895), TTTC_LIMIT );	break;
//			case eItemStage_Hyun:		pIcon->AddToolTipLine( CHATMGR->GetChatMsg(896), TTTC_LIMIT );	break;
//			case eItemStage_OverGeuk:	pIcon->AddToolTipLine( CHATMGR->GetChatMsg(897), TTTC_FREELIMIT );	break;
//			case eItemStage_Geuk:		pIcon->AddToolTipLine( CHATMGR->GetChatMsg(898), TTTC_FREELIMIT );	break;
//			case eItemStage_Tal:		pIcon->AddToolTipLine( CHATMGR->GetChatMsg(899), TTTC_FREELIMIT );	break;*/
//			}
//		}
//		break;
//	//default:
//		//	pIcon->AddToolTipLine(line,TTTC_ITEM_LEVEL);
//			//break;
//	}
//--------------------------------------------------
	switch( pInfo->LimitGender )
	{
	case 1:
		pIcon->AddToolTipLine(CHATMGR->GetChatMsg(236), TTCLR_ENERMY );
		break;
	case 2:
		pIcon->AddToolTipLine( CHATMGR->GetChatMsg(237), TTCLR_ENERMY );
		break;
	}
	if( pInfo->LimitLevel != 0 )
	{
		wsprintf(line, CHATMGR->GetChatMsg(238), pInfo->LimitLevel);
		if( HERO->GetCharacterTotalInfo()->Level < pInfo->LimitLevel )
			pIcon->AddToolTipLine( line, TTCLR_ENERMY );
		else
			pIcon->AddToolTipLine( line, TTTC_FREELIMIT );
	}
	if( pInfo->LimitGenGol != 0 )
	{
		wsprintf(line, CHATMGR->GetChatMsg(239), pInfo->LimitGenGol);
		if( HERO->GetHeroTotalInfo()->wGenGol < pInfo->LimitGenGol )
			pIcon->AddToolTipLine( line, TTCLR_ENERMY );
		else
			pIcon->AddToolTipLine( line, TTTC_FREELIMIT );
	}
	if( pInfo->LimitMinChub != 0 )
	{
		wsprintf(line, CHATMGR->GetChatMsg(240), pInfo->LimitMinChub);
		if( HERO->GetHeroTotalInfo()->wMinChub < pInfo->LimitMinChub )
			pIcon->AddToolTipLine( line, TTCLR_ENERMY );
		else
			pIcon->AddToolTipLine( line, TTTC_FREELIMIT );
	}
	if( pInfo->LimitCheRyuk != 0 )
	{
		wsprintf(line, CHATMGR->GetChatMsg(241), pInfo->LimitCheRyuk);
		if( HERO->GetHeroTotalInfo()->wCheRyuk < pInfo->LimitCheRyuk )
			pIcon->AddToolTipLine( line, TTCLR_ENERMY );
		else
			pIcon->AddToolTipLine( line, TTTC_FREELIMIT );
	}
	if( pInfo->LimitSimMek != 0 )
	{
		wsprintf(line, CHATMGR->GetChatMsg(242), pInfo->LimitSimMek);
		if( HERO->GetHeroTotalInfo()->wSimMek < pInfo->LimitSimMek )
			pIcon->AddToolTipLine( line, TTCLR_ENERMY );
		else
			pIcon->AddToolTipLine( line, TTTC_FREELIMIT );
	}
	if(pInfo->ItemKind == 1040)
	{
		SKILLOPTION* pOption = SKILLMGR->GetSkillOptionByItemIndex(pInfo->ItemIdx);
		if(pOption)
		{
			pIcon->AddToolTipLine( JACKJACK );
			if(pOption->Range > 0)
			{
				char ToolTipText[32];
				wsprintf(ToolTipText, CHATMGR->GetChatMsg(1316), pOption->Range);
				pIcon->AddToolTipLine( ToolTipText, TTTC_TRANSMUGONGOPTION );
			}
			if(pOption->Range < 0)
			{
				char ToolTipText[32];
				wsprintf(ToolTipText, CHATMGR->GetChatMsg(1320), -pOption->Range);
				pIcon->AddToolTipLine( ToolTipText, TTTC_TRANSMUGONGOPTION );
			}
			if(pOption->ReduceNaeRyuk > 0)
			{
				char ToolTipText[32];
				wsprintf(ToolTipText, CHATMGR->GetChatMsg(1317), (int)(pOption->ReduceNaeRyuk * 100 + 0.5));
				pIcon->AddToolTipLine( ToolTipText, TTTC_TRANSMUGONGOPTION );
			}
			if(pOption->ReduceNaeRyuk < 0)
			{
				char ToolTipText[32];
				wsprintf(ToolTipText, CHATMGR->GetChatMsg(1321), (int)(-pOption->ReduceNaeRyuk * 100 + 0.5));
				pIcon->AddToolTipLine( ToolTipText, TTTC_TRANSMUGONGOPTION );
			}
			if(pOption->PhyAtk > 0)
			{
				char ToolTipText[32];
				wsprintf(ToolTipText, CHATMGR->GetChatMsg(1318), (int)(pOption->PhyAtk * 100 + 0.5));
				pIcon->AddToolTipLine( ToolTipText, TTTC_TRANSMUGONGOPTION );
			}
			if(pOption->PhyAtk < 0)
			{
				char ToolTipText[32];
				wsprintf(ToolTipText, CHATMGR->GetChatMsg(1322), (int)(-pOption->PhyAtk * 100 + 0.5));
				pIcon->AddToolTipLine( ToolTipText, TTTC_TRANSMUGONGOPTION );
			}
			if(pOption->BaseAtk > 0)
			{
			char ToolTipText[32];
			wsprintf(ToolTipText, CHATMGR->GetChatMsg(1740), (int)(pOption->BaseAtk * 100));
			pIcon->AddToolTipLine( ToolTipText, TTTC_TRANSMUGONGOPTION );
			}
			if(pOption->AttAtk > 0)
			{
				char ToolTipText[32];
				wsprintf(ToolTipText, CHATMGR->GetChatMsg(1319), (int)(pOption->AttAtk * 100 + 0.5));
				pIcon->AddToolTipLine( ToolTipText, TTTC_TRANSMUGONGOPTION );
			}
			if(pOption->AttAtk < 0)
			{
				char ToolTipText[32];
				wsprintf(ToolTipText, CHATMGR->GetChatMsg(1323), (int)(-pOption->AttAtk * 100 + 0.5));
				pIcon->AddToolTipLine( ToolTipText, TTTC_TRANSMUGONGOPTION );
			}
			if(pOption->Life > 0)	
			{
				char ToolTipText[32];
				wsprintf(ToolTipText, CHATMGR->GetChatMsg(1340), pOption->Life);
				pIcon->AddToolTipLine( ToolTipText, TTTC_TRANSMUGONGOPTION );
			}
			if(pOption->Life < 0)	
			{
				char ToolTipText[32];
				wsprintf(ToolTipText, CHATMGR->GetChatMsg(1341), -pOption->Life);
				pIcon->AddToolTipLine( ToolTipText, TTTC_TRANSMUGONGOPTION );
			}
			if(pOption->NaeRyuk > 0)
			{
				char ToolTipText[32];
				wsprintf(ToolTipText, CHATMGR->GetChatMsg(1342), pOption->NaeRyuk);
				pIcon->AddToolTipLine( ToolTipText, TTTC_TRANSMUGONGOPTION );
			}
			if(pOption->NaeRyuk < 0)
			{
				char ToolTipText[32];
				wsprintf(ToolTipText, CHATMGR->GetChatMsg(1343), -pOption->NaeRyuk);
				pIcon->AddToolTipLine( ToolTipText, TTTC_TRANSMUGONGOPTION );
			}
			if(pOption->Shield > 0)
			{
				char ToolTipText[32];
				wsprintf(ToolTipText, CHATMGR->GetChatMsg(1344), pOption->Shield);
				pIcon->AddToolTipLine( ToolTipText, TTTC_TRANSMUGONGOPTION );
			}
			if(pOption->Shield < 0)
			{
				char ToolTipText[32];
				wsprintf(ToolTipText, CHATMGR->GetChatMsg(1345), -pOption->Shield);
				pIcon->AddToolTipLine( ToolTipText, TTTC_TRANSMUGONGOPTION );
			}
			if(pOption->PhyDef > 0)
			{
				char ToolTipText[32];
				wsprintf(ToolTipText, CHATMGR->GetChatMsg(1348), (int)(pOption->PhyDef * 100 + 0.5));
				pIcon->AddToolTipLine( ToolTipText, TTTC_TRANSMUGONGOPTION );
			}
			if(pOption->PhyDef < 0)
			{
				char ToolTipText[32];
				wsprintf(ToolTipText, CHATMGR->GetChatMsg(1349), (int)(-pOption->PhyDef * 100 + 0.5));
				pIcon->AddToolTipLine( ToolTipText, TTTC_TRANSMUGONGOPTION );
			}
			if(pOption->AttDef > 0)
			{
				char ToolTipText[32];
				wsprintf(ToolTipText, CHATMGR->GetChatMsg(1324), (int)(pOption->AttDef * 100 + 0.5));
				pIcon->AddToolTipLine( ToolTipText, TTTC_TRANSMUGONGOPTION );
			}
			if(pOption->AttDef < 0)
			{
				char ToolTipText[32];
				wsprintf(ToolTipText, CHATMGR->GetChatMsg(1325), (int)(-pOption->AttDef * 100 + 0.5));
				pIcon->AddToolTipLine( ToolTipText, TTTC_TRANSMUGONGOPTION );
			}
			if(pOption->Duration > 0)
			{
				char ToolTipText[32];
				wsprintf(ToolTipText, CHATMGR->GetChatMsg(1346), pOption->Duration);
				pIcon->AddToolTipLine( ToolTipText, TTTC_TRANSMUGONGOPTION );
			}
			if(pOption->Duration < 0)
			{
				char ToolTipText[32];
				wsprintf(ToolTipText, CHATMGR->GetChatMsg(1347), pOption->Duration);
				pIcon->AddToolTipLine( ToolTipText, TTTC_TRANSMUGONGOPTION );
			}
		}
		//pIcon->AddToolTipLine( JACKJACK );
	}
	AddItemDescriptionToolTip( pIcon, pInfo->ItemTooltipIdx );
	//pIcon->AddToolTipLine(JACKJACK);
}
DWORD CalStatusGradeInt(DWORD Status, WORD Grade)
{
	float getdmgmin = Status;
	for (int i = 0; i < Grade; i++)
	{
		getdmgmin = getdmgmin + (getdmgmin * 2.80 / 100);//狂人2.5 //财传奇3.5
	}
	int CalGrade = static_cast<int>(getdmgmin);
	return CalGrade;
}
void CItemManager::SetEquipItemToolTip( cIcon* pIcon, ITEM_INFO* pInfo, ITEM_OPTION_INFO * pOptionInfo, ITEM_RARE_OPTION_INFO* pRareOptionInfo,ITEM_STONE_OPTION_INFO * pStoneOptionInfo,BOOL bIsQuey, DWORD Grade30)
{
	CExchangeItem* pExcItem = NULL;
	CItem* pcItem = NULL;

	if (pIcon->GetType() == WT_EXCHANGEITEM || pIcon->GetType() == WT_STALLITEM ||
		pInfo->ItemKind == eSHOP_ITEM_PET || pIcon->GetType() == WT_BUYITE)
		pExcItem = (CExchangeItem*)pIcon;
	else
		pcItem = (CItem*)pIcon;

	// 获取品质值（优先从 ExchangeItem，其次从 Item）
	int quality = -1;
	if (pExcItem)
		quality = pExcItem->GetQuality();
	else if (pcItem)
		quality = pcItem->GetQuality();
	DWORD color = TTTC_DEFAULT;
	// 获取品质文字与颜色
	const char* qualityName = "";
	switch (quality)
	{
	case 0: qualityName = "·普通"; color = TTTC_DEFAULT; break;
	case 1: qualityName = "·优秀"; color = TTTC_ITEMGROW0; break;
	case 2: qualityName = "·精良"; color = TTTC_ITEMGROW2; break;
	case 3: qualityName = "·传奇"; color = TTTC_ITEMGROW4; break;
	case 4: qualityName = "·神话"; color = TTTC_ITEMGROW5; break;
	default: qualityName = ""; break;
	}


//	pIcon->AddToolTipLine(line3, color);


	//int nLen = strlen(pInfo->ItemName);

	char line[256];
	char line2[256];

	switch(pInfo->ItemKind)
	{
		case eEQUIP_ITEM_DRESS: sprintf(line,CHATMGR->GetChatMsg(2156) ,CHATMGR->GetChatMsg(2161),pInfo->LimitLevel);break;
		case eEQUIP_ITEM_HAT: sprintf(line,CHATMGR->GetChatMsg(2156) ,CHATMGR->GetChatMsg(2162),pInfo->LimitLevel);break;			
		case eEQUIP_ITEM_SHOES: sprintf(line,CHATMGR->GetChatMsg(2156) ,CHATMGR->GetChatMsg(2163),pInfo->LimitLevel);break;			
		case eEQUIP_ITEM_RING: sprintf(line,CHATMGR->GetChatMsg(2156) ,CHATMGR->GetChatMsg(2164),pInfo->LimitLevel);break;	
		case eEQUIP_ITEM_CAPE: sprintf(line,CHATMGR->GetChatMsg(2156) ,CHATMGR->GetChatMsg(2166),pInfo->LimitLevel);break;
		case eEQUIP_ITEM_NECKLACE: sprintf(line,CHATMGR->GetChatMsg(2156) ,CHATMGR->GetChatMsg(2165),pInfo->LimitLevel);break;
		case eEQUIP_ITEM_ARMLET: sprintf(line,CHATMGR->GetChatMsg(2156) ,CHATMGR->GetChatMsg(2168),pInfo->LimitLevel);break;
		case eEQUIP_ITEM_BELT: sprintf(line,CHATMGR->GetChatMsg(2156) ,CHATMGR->GetChatMsg(2167),pInfo->LimitLevel);break;
		case eEQUIP_ITEM_UNIQUE: sprintf(line,CHATMGR->GetChatMsg(2156) ,CHATMGR->GetChatMsg(2170),pInfo->LimitLevel);break;
		case eEQUIP_ITEM_WEAPON:
			{
				switch(pInfo->WeaponType)
				{
					case 0: sprintf(line2, CHATMGR->GetChatMsg(406));break;
					case WP_GUM: sprintf(line2, CHATMGR->GetChatMsg(406));break;					
					case WP_GWUN: sprintf(line2, CHATMGR->GetChatMsg(407));break;
					case WP_DO: sprintf(line2, CHATMGR->GetChatMsg(408));break;
					case WP_CHANG: sprintf(line2, CHATMGR->GetChatMsg(409));break;
					case WP_GUNG: sprintf(line2, CHATMGR->GetChatMsg(410));break;
					case WP_AMGI: sprintf(line2, CHATMGR->GetChatMsg(411));break;
						case 7: sprintf(line2, CHATMGR->GetChatMsg(406));break;
				}
				if( pInfo->ItemIdx==EVENT_ITEM_GLOVE )
					sprintf(line2, CHATMGR->GetChatMsg(2548));
				if( pInfo->ItemIdx==EVENT_ITEM_SNOW )
					sprintf(line2, CHATMGR->GetChatMsg(2549));
				switch( pInfo->RangeType )
				{					
					case 0:sprintf(line,"          %s | %s",CHATMGR->GetChatMsg(272),line2);break;			
					case 1:sprintf(line,"          %s | %s",CHATMGR->GetChatMsg(273),line2);break;
				}
			}
			break;
	}
	pIcon->AddToolTipLine(line,TTTC_ITEM_LEVEL,NULL,0,1);
	float attrvalue = 0;
	float attroptvalue = 0;
	float attrRareOptValue = 0;

	SetItemToolTipForStage( pIcon, pInfo );

	switch( pInfo->LimitGender )
	{
		case 1:pIcon->AddToolTipLine( CHATMGR->GetChatMsg(236), TTCLR_ENERMY );
			break;			
		case 2:pIcon->AddToolTipLine( CHATMGR->GetChatMsg(237), TTCLR_ENERMY );
			break;
	}
	if( pInfo->LimitLevel != 0 )
	{
		sprintf(line, CHATMGR->GetChatMsg(238), pInfo->LimitLevel);
		if( HERO->GetCharacterTotalInfo()->Level < pInfo->LimitLevel )
			pIcon->AddToolTipLine( line, TTCLR_ENERMY );
		else
			pIcon->AddToolTipLine( line, TTTC_FREELIMIT );
	}
	pIcon->AddToolTipLine( JACKJACK );
	if( pInfo->LimitGenGol != 0 )
	{
		sprintf(line, CHATMGR->GetChatMsg(239), pInfo->LimitGenGol);
		if( HERO->GetHeroTotalInfo()->wGenGol < pInfo->LimitGenGol )
			pIcon->AddToolTipLine( line, TTTC_LIMIT );
		else
			pIcon->AddToolTipLine( line, TTTC_FREELIMIT );
	}
	if( pInfo->LimitMinChub != 0 )
	{
		sprintf(line, CHATMGR->GetChatMsg(240), pInfo->LimitMinChub);
		if( HERO->GetHeroTotalInfo()->wMinChub < pInfo->LimitMinChub )
			pIcon->AddToolTipLine( line, TTTC_LIMIT );
		else
			pIcon->AddToolTipLine( line, TTTC_FREELIMIT );
	}
	if( pInfo->LimitCheRyuk != 0 )
	{
		sprintf(line, CHATMGR->GetChatMsg(241), pInfo->LimitCheRyuk);
		if( HERO->GetHeroTotalInfo()->wCheRyuk < pInfo->LimitCheRyuk )
			pIcon->AddToolTipLine( line, TTTC_LIMIT );
		else
			pIcon->AddToolTipLine( line, TTTC_FREELIMIT );
	}
	if( pInfo->LimitSimMek != 0 )
	{
		sprintf(line, CHATMGR->GetChatMsg(242), pInfo->LimitSimMek);
		if( HERO->GetHeroTotalInfo()->wSimMek < pInfo->LimitSimMek )
			pIcon->AddToolTipLine( line, TTTC_LIMIT );
		else
			pIcon->AddToolTipLine( line, TTTC_FREELIMIT );
	}
	pIcon->AddToolTipLine("");
	//装备品质
	if (pExcItem)
		AddSetItemQualityToolTip(pIcon, pExcItem->GetQuality(), pExcItem->GetEntry1(), pExcItem->GetEntry2(), color);
	else
		AddSetItemQualityToolTip(pIcon, pcItem->GetQuality(), pcItem->GetEntry1(), pcItem->GetEntry2(), color);
	pIcon->AddToolTipLine("");
	//=======================================================GenGol
	if (pInfo->GenGol != 0)
	{
		sprintf(line, "%s +%d", CHATMGR->GetChatMsg(382), CalStatusGradeInt(pInfo->GenGol, Grade30));
		pIcon->AddToolTipLine(line, TTTC_EXTRAATTR);
	}

	if (pInfo->MinChub != 0)
	{
		sprintf(line, "%s +%d", CHATMGR->GetChatMsg(383), CalStatusGradeInt(pInfo->MinChub, Grade30));
		pIcon->AddToolTipLine(line, TTTC_EXTRAATTR);
	}

	if (pInfo->CheRyuk != 0)
	{
		sprintf(line, "%s +%d", CHATMGR->GetChatMsg(384), CalStatusGradeInt(pInfo->CheRyuk, Grade30));
		pIcon->AddToolTipLine(line, TTTC_EXTRAATTR);
	}

	if (pInfo->SimMek != 0)
	{
		sprintf(line, "%s +%d", CHATMGR->GetChatMsg(385), CalStatusGradeInt(pInfo->SimMek, Grade30));
		pIcon->AddToolTipLine(line, TTTC_EXTRAATTR);
	}


	if (pInfo->Life != 0)
	{
		sprintf(line, "%s +%d", CHATMGR->GetChatMsg(386), CalStatusGradeInt(pInfo->Life, Grade30));
		pIcon->AddToolTipLine(line, TTTC_EXTRAATTR);
	}

	if (pInfo->NaeRyuk != 0)
	{
		sprintf(line, "%s +%d", CHATMGR->GetChatMsg(387), CalStatusGradeInt(pInfo->NaeRyuk, Grade30));
		pIcon->AddToolTipLine(line, TTTC_EXTRAATTR);
	}

	if (pInfo->Shield != 0)
	{
		sprintf(line, "%s +%d", CHATMGR->GetChatMsg(388), CalStatusGradeInt(pInfo->Shield, Grade30));
		pIcon->AddToolTipLine(line, TTTC_EXTRAATTR);
	}

	for (int i = ATTR_FIRE; i <= ATTR_MAX; ++i)
	{
		attrvalue = 100 * pInfo->AttrRegist.GetElement_Val(i);
		if ((int)(attrvalue) != 0)
		{
			sprintf(line, "%s +%d%%", CHATMGR->GetChatMsg(265 + i - 1), (int)(CalStatusGradeInt((int)attrvalue, Grade30)));
			pIcon->AddToolTipLine(line, TTTC_EXTRAATTR);
		}
	}

	if (pInfo->MeleeAttackMin || pInfo->MeleeAttackMax)
	{
		if (pInfo->ItemKind != eEQUIP_ITEM_ARMLET)
		{
			if (pInfo->MeleeAttackMin == pInfo->MeleeAttackMax)
			{
				sprintf(line, "%s %d", CHATMGR->GetChatMsg(389), (CalStatusGradeInt(pInfo->MeleeAttackMin, Grade30)));
			}
			else
			{
				sprintf(line, "%s %d ~ %d", CHATMGR->GetChatMsg(389), (CalStatusGradeInt(pInfo->MeleeAttackMin, Grade30)), (CalStatusGradeInt(pInfo->MeleeAttackMax, Grade30)));
			}

			pIcon->AddToolTipLine(line, TTTC_attack);
		}
		else
		{
			if (pInfo->MeleeAttackMin == pInfo->MeleeAttackMax)
			{
				sprintf(line, "%s %d%%", CHATMGR->GetChatMsg(389), (CalStatusGradeInt(pInfo->MeleeAttackMin, Grade30)));
			}
			else
			{
				sprintf(line, "%s %d%% ~ %d%%", CHATMGR->GetChatMsg(389), (CalStatusGradeInt(pInfo->MeleeAttackMin, Grade30)), (CalStatusGradeInt(pInfo->MeleeAttackMax, Grade30)));
			}
			pIcon->AddToolTipLine(line, TTTC_attack);
		}
	}

	if (pInfo->RangeAttackMin || pInfo->RangeAttackMax)
	{
		if (pInfo->ItemKind != eEQUIP_ITEM_ARMLET)
		{
			if (pInfo->RangeAttackMin == pInfo->RangeAttackMax)
			{
				sprintf(line, "%s %d", CHATMGR->GetChatMsg(391), (CalStatusGradeInt(pInfo->RangeAttackMax, Grade30)));
			}
			else
			{
				sprintf(line, "%s %d ~ %d", CHATMGR->GetChatMsg(391), (CalStatusGradeInt(pInfo->RangeAttackMin, Grade30)), (CalStatusGradeInt(pInfo->RangeAttackMax, Grade30)));
			}

			pIcon->AddToolTipLine(line, TTTC_attack);
		}
		else
		{
			if (pInfo->RangeAttackMin == pInfo->RangeAttackMax)
			{
				sprintf(line, "%s %d%%", CHATMGR->GetChatMsg(391), (CalStatusGradeInt(pInfo->RangeAttackMax, Grade30)));//RangeAttackMax
			}
			else
			{
				sprintf(line, "%s %d%% ~ %d%%", CHATMGR->GetChatMsg(391), (CalStatusGradeInt(pInfo->RangeAttackMin, Grade30)), (CalStatusGradeInt(pInfo->RangeAttackMax, Grade30)));//RangeAttackMin  RangeAttackMax
			}
			pIcon->AddToolTipLine(line, TTTC_attack);
		}
	}

	for (int j = ATTR_FIRE; j <= ATTR_MAX; ++j)
	{
		attrvalue = 100 * pInfo->AttrAttack.GetElement_Val(j);
		if ((int)(attrvalue) != 0)
		{
			sprintf(line, "%s +%d%%", CHATMGR->GetChatMsg(392 + j - 1), (int)(CalStatusGradeInt((int)attrvalue, Grade30)));
			pIcon->AddToolTipLine(line, TTTC_EXTRAATTR);
		}
	}

	if (pInfo->PhyDef != 0)
	{
		sprintf(line, "%s +%d", CHATMGR->GetChatMsg(397), CalStatusGradeInt(pInfo->PhyDef, Grade30));
		pIcon->AddToolTipLine(line, TTTC_EXTRAATTR);
	}
	//=======================================================PhyDefense
	if (pInfo->Plus_Value != 0)
	{
		sprintf(line, CHATMGR->GetChatMsg(398), pInfo->ItemName, CalStatusGradeInt(pInfo->Plus_Value, Grade30));
		pIcon->AddToolTipLine(line, TTTC_EXTRAATTR);
	}
	if (pInfo->AllPlus_Value != 0 && pInfo->AllPlus_Kind != 0)
	{
		char buf[32] = { 0, };
		switch (pInfo->AllPlus_Kind)
		{
		case 1: strcpy(buf, CHATMGR->GetChatMsg(406)); break;
		case 2: strcpy(buf, CHATMGR->GetChatMsg(407)); break;
		case 3: strcpy(buf, CHATMGR->GetChatMsg(408)); break;
		case 4: strcpy(buf, CHATMGR->GetChatMsg(409)); break;
		case 5: strcpy(buf, CHATMGR->GetChatMsg(410)); break;
		case 6: strcpy(buf, CHATMGR->GetChatMsg(411)); break;
		case 7: strcpy(buf, CHATMGR->GetChatMsg(412)); break;
		case 8: strcpy(buf, CHATMGR->GetChatMsg(413)); break;
		case 9: strcpy(buf, CHATMGR->GetChatMsg(414)); break;
		case 10: strcpy(buf, CHATMGR->GetChatMsg(415)); break;
		case 11: strcpy(buf, CHATMGR->GetChatMsg(416)); break;
		}
		sprintf(line, CHATMGR->GetChatMsg(417), buf, pInfo->AllPlus_Value);
		pIcon->AddToolTipLine(line, TTTC_EXTRAATTR);
	}
	////////////////////////////////////////////////////强化显示
	if (pOptionInfo && pOptionInfo->dwOptionIdx)
	{
		pIcon->AddToolTipLine("");   // 强化
		sprintf(line, "< %s >", CHATMGR->GetChatMsg(2273));
		pIcon->AddToolTipLine(line, RGB_HALF(255, 214, 150));
		if (pOptionInfo && pOptionInfo->GenGol != 0)
		{
			sprintf(line, "%s +%d", CHATMGR->GetChatMsg(382), pOptionInfo->GenGol);
			pIcon->AddToolTipLine(line, TTTC_EXTRAATTR);
		}
		if (pOptionInfo && pOptionInfo->MinChub != 0)
		{
			sprintf(line, "%s +%d", CHATMGR->GetChatMsg(383), pOptionInfo->MinChub);
			pIcon->AddToolTipLine(line, TTTC_EXTRAATTR);
		}
		if (pOptionInfo && pOptionInfo->CheRyuk != 0)
		{
			sprintf(line, "%s +%d", CHATMGR->GetChatMsg(384), pOptionInfo->CheRyuk);
			pIcon->AddToolTipLine(line, TTTC_EXTRAATTR);
		}
		if (pOptionInfo && pOptionInfo->SimMek != 0)
		{
			sprintf(line, "%s +%d", CHATMGR->GetChatMsg(385), pOptionInfo->SimMek);
			pIcon->AddToolTipLine(line, TTTC_EXTRAATTR);
		}
		if (pOptionInfo && pOptionInfo->Life != 0)
		{
			sprintf(line, "%s +%d", CHATMGR->GetChatMsg(386), pOptionInfo->Life);
			pIcon->AddToolTipLine(line, TTTC_EXTRAATTR);
		}
		if (pOptionInfo && pOptionInfo->NaeRyuk != 0)
		{
			sprintf(line, "%s +%d", CHATMGR->GetChatMsg(387), pOptionInfo->NaeRyuk);
			pIcon->AddToolTipLine(line, TTTC_EXTRAATTR);
		}
		if (pOptionInfo && pOptionInfo->Shield != 0)
		{
			sprintf(line, "%s +%d", CHATMGR->GetChatMsg(388), pOptionInfo->Shield);
			pIcon->AddToolTipLine(line, TTTC_EXTRAATTR);
		}
		for (int i = ATTR_FIRE; i <= ATTR_MAX; ++i)
		{
			if (pOptionInfo)
				attroptvalue = 100 * pOptionInfo->AttrRegist.GetElement_Val(i);
			if ((int)(attroptvalue) != 0)
			{
				sprintf(line, "%s +%d%%", CHATMGR->GetChatMsg(265 + i - 1), (int)(attroptvalue));
				pIcon->AddToolTipLine(line, TTTC_EXTRAATTR);
			}
		}
		if (pOptionInfo && pOptionInfo->PhyAttack != 0)
		{
			sprintf(line, "%s +%d ", CHATMGR->GetChatMsg(681), pOptionInfo->PhyAttack);
			pIcon->AddToolTipLine(line, TTTC_EXTRAATTR);
		}
		if (pOptionInfo && pOptionInfo->CriticalPercent != 0)
		{
			sprintf(line, "%s +%d ", CHATMGR->GetChatMsg(390), pOptionInfo->CriticalPercent);
			pIcon->AddToolTipLine(line, TTTC_EXTRAATTR);
		}
		for (int j = ATTR_FIRE; j <= ATTR_MAX; ++j)
		{
			if (pOptionInfo)
				attroptvalue = 100 * pOptionInfo->AttrAttack.GetElement_Val(j);
			if ((int)(attroptvalue) != 0)
			{
				sprintf(line, "%s +%d%%", CHATMGR->GetChatMsg(392 + j - 1), (int)(attroptvalue));
				pIcon->AddToolTipLine(line, TTTC_EXTRAATTR);
			}
		}
		if (pOptionInfo && pOptionInfo->PhyDefense != 0)
		{
			sprintf(line, "%s +%d", CHATMGR->GetChatMsg(397), pOptionInfo->PhyDefense);
			pIcon->AddToolTipLine(line, TTTC_EXTRAATTR);
		}
	}

	if (pRareOptionInfo && pRareOptionInfo->dwRareOptionIdx)
	{
		pIcon->AddToolTipLine("");   // 添一空行
		sprintf(line, "< %s >", CHATMGR->GetChatMsg(2274));
		pIcon->AddToolTipLine(line, RGB_HALF(255, 214, 150));
		DWORD RareState;
		if (pRareOptionInfo && pRareOptionInfo->GenGol)
			RareState = pRareOptionInfo->GenGol;
		else
			RareState = 0;
		if (pRareOptionInfo && pRareOptionInfo->GenGol != 0)
		{
			sprintf(line, "%s +%d ", CHATMGR->GetChatMsg(382), pRareOptionInfo->GenGol);
			pIcon->AddToolTipLine(line, TTTC_EXTRAATTR);
		}

		if (pRareOptionInfo && pRareOptionInfo->MinChub)
			RareState = pRareOptionInfo->MinChub;
		else
			RareState = 0;
		if (pRareOptionInfo && pRareOptionInfo->MinChub != 0)
		{
			sprintf(line, "%s +%d ", CHATMGR->GetChatMsg(383), pRareOptionInfo->MinChub);
			pIcon->AddToolTipLine(line, TTTC_EXTRAATTR);
		}

		if (pRareOptionInfo && pRareOptionInfo->CheRyuk)
			RareState = pRareOptionInfo->CheRyuk;
		else
			RareState = 0;
		if (pRareOptionInfo && pRareOptionInfo->CheRyuk != 0)
		{
			sprintf(line, "%s +%d ", CHATMGR->GetChatMsg(384), pRareOptionInfo->CheRyuk);
			pIcon->AddToolTipLine(line, TTTC_EXTRAATTR);
		}

		if (pRareOptionInfo && pRareOptionInfo->SimMek)
			RareState = pRareOptionInfo->SimMek;
		else
			RareState = 0;
		if (pRareOptionInfo && pRareOptionInfo->SimMek != 0)
		{
			sprintf(line, "%s +%d ", CHATMGR->GetChatMsg(385), pRareOptionInfo->SimMek);
			pIcon->AddToolTipLine(line, TTTC_EXTRAATTR);
		}

		if (pRareOptionInfo && pRareOptionInfo->Life)
			RareState = pRareOptionInfo->Life;
		else
			RareState = 0;
		if (pRareOptionInfo && pRareOptionInfo->Life != 0)
		{
			sprintf(line, "%s +%d ", CHATMGR->GetChatMsg(386), pRareOptionInfo->Life);
			pIcon->AddToolTipLine(line, TTTC_EXTRAATTR);
		}

		if (pRareOptionInfo && pRareOptionInfo->NaeRyuk)
			RareState = pRareOptionInfo->NaeRyuk;
		else
			RareState = 0;
		if (pRareOptionInfo && pRareOptionInfo->NaeRyuk != 0)
		{
			sprintf(line, "%s +%d ", CHATMGR->GetChatMsg(387), pRareOptionInfo->NaeRyuk);
			pIcon->AddToolTipLine(line, TTTC_EXTRAATTR);
		}

		if (pRareOptionInfo && pRareOptionInfo->Shield)
			RareState = pRareOptionInfo->Shield;
		else
			RareState = 0;
		if (pRareOptionInfo && pRareOptionInfo->Shield != 0)
		{
			sprintf(line, "%s +%d ", CHATMGR->GetChatMsg(388), pRareOptionInfo->Shield);
			pIcon->AddToolTipLine(line, TTTC_EXTRAATTR);
		}

		for (int i = ATTR_FIRE; i <= ATTR_MAX; ++i)
		{
			if (pRareOptionInfo)
			{
				attrRareOptValue = 100 * pRareOptionInfo->AttrRegist.GetElement_Val(i);
			}
			if (attrRareOptValue)
			{
				sprintf(line, "%s +%d%%", CHATMGR->GetChatMsg(265 + i - 1), (int)(attrRareOptValue));
				pIcon->AddToolTipLine(line, TTTC_EXTRAATTR);
			}
		}

		WORD RareOptPhyAttack;
		if (pRareOptionInfo && pRareOptionInfo->PhyAttack)
			RareOptPhyAttack = pRareOptionInfo->PhyAttack;
		else
			RareOptPhyAttack = 0;
		if (RareOptPhyAttack != 0)
		{
			sprintf(line, "%s +%d ", CHATMGR->GetChatMsg(681), RareOptPhyAttack);
			pIcon->AddToolTipLine(line, TTTC_EXTRAATTR);
		}

		for (int j = ATTR_FIRE; j <= ATTR_MAX; ++j)
		{
			if (pRareOptionInfo)
			{
				attrRareOptValue = 100 * pRareOptionInfo->AttrAttack.GetElement_Val(j);
			}
			else attrRareOptValue = 0;
			if (attrRareOptValue)
			{
				sprintf(line, "%s +%d%%", CHATMGR->GetChatMsg(392 + j - 1), (int)(attrRareOptValue));
				pIcon->AddToolTipLine(line, TTTC_EXTRAATTR);
			}
		}

		WORD RareOptPhyDef;
		if (pRareOptionInfo && pRareOptionInfo->PhyDefense)
			RareOptPhyDef = pRareOptionInfo->PhyDefense;
		else
			RareOptPhyDef = 0;
		if (RareOptPhyDef)
		{
			sprintf(line, "%s +%d ", CHATMGR->GetChatMsg(397), RareOptPhyDef);
			pIcon->AddToolTipLine(line, TTTC_EXTRAATTR);
		}
	}
///////////////////////////////////////////////////////////////////////////////////////
	pIcon->AddToolTipLine("");
//-------------------------------------------------start qq
	TOOLTIP_TEXT* pTooltipText;
	
	if(pStoneOptionInfo && pStoneOptionInfo->dwStoneOptionIdx)
	{
		static int cur;
		static int max;
		char lineqq[256];
		pIcon->AddToolTipLine( "" );

		//ZeroMemory(&lineqq, sizeof(lineqq));

		sprintf( lineqq, CHATMGR->GetChatMsg(2666),cur,max);
		

		pIcon->AddToolTipLine( lineqq, /*TTTC_EXTRAATTR*/TTTC_FREELIMIT );
		

		cImage  imgBaseInfo;		
		if(pStoneOptionInfo->dwItemStone0==1) 
		{
			SCRIPTMGR->GetImage(147 , &imgBaseInfo,PFT_HARDPATH);
			if(!imgBaseInfo.IsNull())
			{
			    pIcon->AddToolTipImageStone(&imgBaseInfo,0);
				max=1;
			}
		}
		else
		{
            if(pStoneOptionInfo->dwItemStone0>1)
			{
				ITEM_INFO * pItem=GetItemInfo(pStoneOptionInfo->dwItemStone0);
				if(pItem)
				{
					SCRIPTMGR->GetImage(pItem->Image2DNum,&imgBaseInfo,PFT_ITEMPATH);
					if(!imgBaseInfo.IsNull())
				    {
					  pIcon->AddToolTipImageStone(&imgBaseInfo,0);
					  cur=1;
				    }
					pTooltipText = GetItemToolTipInfo(pItem->ItemTooltipIdx);
             		while( pTooltipText )
					{
						pIcon->AddToolTipLine(pTooltipText->strToolTip, TTTC_DESCRIPTION );
						pTooltipText = pTooltipText->pNext;
						
					}
					pTooltipText=NULL;
				}
				pItem=NULL;
			}
		}
		if(pStoneOptionInfo->dwItemStone1==1) 
		{
			SCRIPTMGR->GetImage(147 , &imgBaseInfo,PFT_HARDPATH);
			if(!imgBaseInfo.IsNull())
			{
			    pIcon->AddToolTipImageStone(&imgBaseInfo,1);
				max=2;
			}
		}
		else
		{
            if(pStoneOptionInfo->dwItemStone1>1)
			{
				ITEM_INFO * pItem=GetItemInfo(pStoneOptionInfo->dwItemStone1);
				if(pItem)
				{
					SCRIPTMGR->GetImage(pItem->Image2DNum,&imgBaseInfo,PFT_ITEMPATH);
					if(!imgBaseInfo.IsNull())
				    {
					  pIcon->AddToolTipImageStone(&imgBaseInfo,1);
					  cur=2;
				    }
					pTooltipText = GetItemToolTipInfo(pItem->ItemTooltipIdx);
             		while( pTooltipText )
					{
						pIcon->AddToolTipLine(pTooltipText->strToolTip, TTTC_DESCRIPTION );
						pTooltipText = pTooltipText->pNext;
						
					}
					pTooltipText=NULL;
				}
				pItem=NULL;
			}
		}
		if(pStoneOptionInfo->dwItemStone2==1) 
		{
			SCRIPTMGR->GetImage(147 , &imgBaseInfo,PFT_HARDPATH);
			if(!imgBaseInfo.IsNull())
			{
			    pIcon->AddToolTipImageStone(&imgBaseInfo,2);
				max=3;
			}
		}
		else
		{
            if(pStoneOptionInfo->dwItemStone2>1)
			{
				ITEM_INFO * pItem=GetItemInfo(pStoneOptionInfo->dwItemStone2);
				if(pItem)
				{
					SCRIPTMGR->GetImage(pItem->Image2DNum,&imgBaseInfo,PFT_ITEMPATH);
					if(!imgBaseInfo.IsNull())
				    {
					  pIcon->AddToolTipImageStone(&imgBaseInfo,2);
					  cur=3;
				    }
					pTooltipText = GetItemToolTipInfo(pItem->ItemTooltipIdx);
             		while( pTooltipText )
					{
						pIcon->AddToolTipLine(pTooltipText->strToolTip, TTTC_DESCRIPTION );
						pTooltipText = pTooltipText->pNext;
						
					}
					pTooltipText=NULL;
				}
				pItem=NULL;
			}
		}
		if(pStoneOptionInfo->dwItemStone3==1) 
		{
			SCRIPTMGR->GetImage(147 , &imgBaseInfo,PFT_HARDPATH);
			if(!imgBaseInfo.IsNull())
			{
			    pIcon->AddToolTipImageStone(&imgBaseInfo,3);
				max=4;
			}
		}
		else
		{
            if(pStoneOptionInfo->dwItemStone0>3)
			{
				ITEM_INFO * pItem=GetItemInfo(pStoneOptionInfo->dwItemStone3);
				if(pItem)
				{
					SCRIPTMGR->GetImage(pItem->Image2DNum,&imgBaseInfo,PFT_ITEMPATH);
					if(!imgBaseInfo.IsNull())
				    {
					  pIcon->AddToolTipImageStone(&imgBaseInfo,3);
					  cur=4;
				    }
					pTooltipText = GetItemToolTipInfo(pItem->ItemTooltipIdx);
             		while( pTooltipText )
					{
						pIcon->AddToolTipLine(pTooltipText->strToolTip, TTTC_DESCRIPTION );
						pTooltipText = pTooltipText->pNext;
						
					}
					pTooltipText=NULL;
				}
				pItem=NULL;
			}
		}
		if(pStoneOptionInfo->dwItemStone4==1) 
		{
			SCRIPTMGR->GetImage(147 , &imgBaseInfo,PFT_HARDPATH);
			if(!imgBaseInfo.IsNull())
			{
			    pIcon->AddToolTipImageStone(&imgBaseInfo,4);
				max=5;
			}
		}
		else
		{
            if(pStoneOptionInfo->dwItemStone4>1)
			{
				ITEM_INFO * pItem=GetItemInfo(pStoneOptionInfo->dwItemStone4);
				if(pItem)
				{
					SCRIPTMGR->GetImage(pItem->Image2DNum,&imgBaseInfo,PFT_ITEMPATH);
					if(!imgBaseInfo.IsNull())
				    {
					  pIcon->AddToolTipImageStone(&imgBaseInfo,4);
					  cur=5;
				    }
					pTooltipText = GetItemToolTipInfo(pItem->ItemTooltipIdx);
             		while( pTooltipText )
					{
						pIcon->AddToolTipLine(pTooltipText->strToolTip, TTTC_DESCRIPTION );
						pTooltipText = pTooltipText->pNext;
						
					}
					pTooltipText=NULL;
				}
				pItem=NULL;
			}
		}

		strcpy( lineqq, lineqq);

		
	}
//-------------------------------------------------end qq
	if(pInfo->ItemKind & eSHOP_ITEM)
	{
		AddShopItemToolTip( (cIcon*)pIcon, pInfo );
		//pIcon->AddToolTipLine("TEST");
	}
	if(pInfo->wSetItemKind != 0)
	{
		AddSetItemToolTip( (cIcon*)pIcon, pInfo); 
	}
	if(pInfo->ItemKind == eEQUIP_ITEM_UNIQUE)
	{
		AddUniqueItemToolTip( (cIcon*)pIcon, pInfo);
	}
	if( pInfo->ItemKind & eEQUIP_ITEM && pInfo->ItemKind < eEQUIP_ITEM_UNIQUE )
	{
		if((pRareOptionInfo && pRareOptionInfo->dwItemDBIdx)||(pOptionInfo&&pOptionInfo->dwOptionIdx!=0))
		{
			pIcon->AddToolTipLine("");
		}
	}


	/*eITEM_KINDBIT bits = GetItemKind(pInfo->ItemIdx);
	if (bits == eEQUIP_ITEM_WEAPON)
	{
		char GrowMsg[128];
		if (pIcon->GetType() == WT_EXCHANGEITEM || pIcon->GetType() == WT_STALLITEM || pInfo->ItemKind == eSHOP_ITEM_PET)
		{
			CExchangeItem* pExItem = (CExchangeItem*)pIcon;
			if (pExItem)
			{
				sprintf(GrowMsg, CHATMGR->GetChatMsg(2128), pExItem->GetGrow());
				pIcon->AddToolTipLine("");
				pIcon->AddToolTipLine(GrowMsg, TTTC_ITEMGROW);
			}
		}
		else
		{
			CItem* pBaseItem = (CItem*)pIcon;
			if (pIcon->GetType() != WT_ICON)
			{
				sprintf(GrowMsg, CHATMGR->GetChatMsg(2128), pBaseItem->GetGrow());
				pIcon->AddToolTipLine("");
				pIcon->AddToolTipLine(GrowMsg, TTTC_ITEMGROW);
			}
		}
	}*/
	///////////////////////////////////pvp 属性
	float fPVP = 100 * pInfo->PVPCri;
	if (fPVP != 0)
	{
		//pIcon->AddToolTipLine("");   // 添一空行
		sprintf(line, "< %s>", CHATMGR->GetChatMsg(2735));
		pIcon->AddToolTipLine(line, RGB_HALF(255, 0, 0));
		sprintf(line, CHATMGR->GetChatMsg(2729), (int)fPVP);
		pIcon->AddToolTipLine(line, RGB_HALF(0, 255, 0));
	}
	fPVP = 100 * pInfo->PVPAttack;
	if (fPVP != 0)
	{
		sprintf(line, CHATMGR->GetChatMsg(2730), (int)fPVP);
		pIcon->AddToolTipLine(line, RGB_HALF(0, 255, 0));
	}
	fPVP = 100 * pInfo->PVPDef;
	if (fPVP != 0)
	{
		sprintf(line, CHATMGR->GetChatMsg(2731), (int)fPVP);
		pIcon->AddToolTipLine(line, RGB_HALF(0, 255, 0));
	}
	fPVP = 100 * pInfo->PVPADef;
	if (fPVP != 0)
	{
		sprintf(line, CHATMGR->GetChatMsg(2732), (int)fPVP);
		pIcon->AddToolTipLine(line, RGB_HALF(0, 255, 0));
	}
	fPVP = 100 * pInfo->PVPHit;
	if (fPVP != 0)
	{
		sprintf(line, CHATMGR->GetChatMsg(2733), (int)fPVP);
		pIcon->AddToolTipLine(line, RGB_HALF(0, 255, 0));
	}
	fPVP = 100 * pInfo->PVPADodge;
	if (fPVP != 0)
	{
		sprintf(line, CHATMGR->GetChatMsg(2734), (int)fPVP);
		pIcon->AddToolTipLine(line, RGB_HALF(0, 255, 0));
	}
	fPVP = 100 * pInfo->PVPStunResist;
	if (fPVP != 0)
	{
		sprintf(line, CHATMGR->GetChatMsg(2736), (int)fPVP);  //// 新增晕眩抗性 & 晕眩时间减少
		pIcon->AddToolTipLine(line, RGB_HALF(0, 255, 0));
	}
	fPVP = 100 * pInfo->PVPStunTimeReduce;
	if (fPVP != 0)
	{
		sprintf(line, CHATMGR->GetChatMsg(2737), (int)fPVP);  //// 新增晕眩抗性 & 晕眩时间减少
		pIcon->AddToolTipLine(line, RGB_HALF(0, 255, 0));
	}
	/////////////////////////////////////////pvp 属性
	CItem * pOrigItem = (CItem *)pIcon;
	if( pInfo->ItemKind & eEQUIP_ITEM && pInfo->ItemKind < eEQUIP_ITEM_UNIQUE )
	{
		char SafeLine[47];
		if( pIcon->GetType() == WT_EXCHANGEITEM || pIcon->GetType() == WT_STALLITEM || pInfo->ItemKind == eSHOP_ITEM_PET )
		{
			const ITEMBASE* pExItem = ((CItem*)pIcon)->GetItemBaseInfo();
			if(pExItem)
			{
				//pIcon->AddToolTipLine("eSHOP_ITEM_PET");
			}
		}
		else
		{
			const ITEMBASE* pBaseInfo = ((CItem*)pIcon)->GetItemBaseInfo();
			if(pIcon->GetType()!=WT_ICON)
			{
				if(pRareOptionInfo && pRareOptionInfo->dwItemDBIdx)
				{
					char BlessMsg[47];
					int LineSize=strlen(CHATMGR->GetChatMsg(2484));
					SafeStrCpy(SafeLine,JACKJACK, LineSize+2 );
					if(pBaseInfo->PowerUp!=NULL)
					{
						if(strcmp(pBaseInfo->PowerUp,"monster")==0)
						{
							sprintf(BlessMsg,"%s%s",SafeLine,CHATMGR->GetChatMsg(2483));
							pIcon->AddToolTipLine2(CHATMGR->GetChatMsg(2484),BlessMsg,ICONTEXT_USABLE,TTCLR_ENERMY);
						}
						else
						{
							sprintf(BlessMsg,"%s%s",SafeLine,pBaseInfo->PowerUp);
							pIcon->AddToolTipLine2(CHATMGR->GetChatMsg(2484),BlessMsg,ICONTEXT_USABLE,ICONTEXT_USABLE);
						}
						ZeroMemory(&BlessMsg, sizeof(BlessMsg));
					}
				}
				if(pOptionInfo&&pOptionInfo->dwOptionIdx!=0)
				{
					char CraftMsg[47];
					int LineSize=strlen(CHATMGR->GetChatMsg(2485));
					SafeStrCpy(SafeLine,JACKJACK, LineSize+2 );
					if(pBaseInfo->Green!=NULL)
					{
						if(strcmp(pBaseInfo->Green,"monster")==0)
						{
							sprintf(CraftMsg,"%s%s",SafeLine,CHATMGR->GetChatMsg(2483));
							pIcon->AddToolTipLine2(CHATMGR->GetChatMsg(2485),CraftMsg,ICONTEXT_USABLE,TTCLR_ENERMY);
						}
						else
						{
							sprintf(CraftMsg,"%s%s",SafeLine,pBaseInfo->Green);
							pIcon->AddToolTipLine2(CHATMGR->GetChatMsg(2485),CraftMsg,ICONTEXT_USABLE,ICONTEXT_USABLE);
						}
						ZeroMemory(&CraftMsg, sizeof(CraftMsg));
					}
				}
			}
		}
	}
	//if (pInfo->ItemKind == eEQUIP_ITEM_WEAPON || pInfo->ItemKind == eEQUIP_ITEM_UNIQUE)
	//{
	//	pIcon->AddToolTipLine("");
	//	//sprintf(line, "武器专属", pInfo->ItemName, pInfo->Plus_Value);
	//	//pIcon->AddToolTipLine( line, RGB_HALF(252,180,93) );
	//}
	switch (pInfo->WeaponType)
	{




	case WP_GUM:
	{
		pIcon->AddToolTipLine("");   // 添一空行
		sprintf(line, "< 武器特殊加成>");
		pIcon->AddToolTipLine(line, RGB_HALF(255, 0, 0));
		sprintf(line, "- 命中+10");
		pIcon->AddToolTipLine(line, RGBA_MERGE(RGB_HALF(180, 0, 255), 255));
		sprintf(line, "- 奋力一击几率+15");
		pIcon->AddToolTipLine(line, RGBA_MERGE(RGB_HALF(180, 0, 255), 255));
	}
	break;
	case WP_GWUN:
	{
		pIcon->AddToolTipLine("");   // 添一空行
		sprintf(line, "< 武器特殊加成>");
		pIcon->AddToolTipLine(line, RGB_HALF(255, 0, 0));
		sprintf(line, "- 每10点体质属性，攻击力增加1点");
		pIcon->AddToolTipLine(line, RGBA_MERGE(RGB_HALF(180, 0, 255), 255));
	}
	break;
	case WP_DO:
	{
		pIcon->AddToolTipLine("");   // 添一空行
		sprintf(line, "< 武器特殊加成>");
		pIcon->AddToolTipLine(line, RGB_HALF(255, 0, 0));
		sprintf(line, "- 使用技能(含内功技能)时，奋力一击伤害增加15%%");
		pIcon->AddToolTipLine(line, RGBA_MERGE(RGB_HALF(180, 0, 255), 255));
	}
	break;
	case WP_CHANG:
	{
		pIcon->AddToolTipLine("");   // 添一空行
		sprintf(line, "< 武器特殊加成>");
		pIcon->AddToolTipLine(line, RGB_HALF(255, 0, 0));
		sprintf(line, "- 闪避+10");
		pIcon->AddToolTipLine(line, RGBA_MERGE(RGB_HALF(180, 0, 255), 255));
		sprintf(line, "- 最终伤害减少5%%");
		pIcon->AddToolTipLine(line, RGBA_MERGE(RGB_HALF(180, 0, 255), 255));
	}
	break;
	case WP_GUNG:
	{
		pIcon->AddToolTipLine("");   // 添一空行
		sprintf(line, "< 武器特殊加成>");
		pIcon->AddToolTipLine(line, RGB_HALF(255, 0, 0));
		sprintf(line, "- 使用弓职业技能时，破甲增加15%%");
		pIcon->AddToolTipLine(line, RGBA_MERGE(RGB_HALF(180, 0, 255), 255));
	}
	break;
	case WP_AMGI:
	{
		pIcon->AddToolTipLine("");   // 添一空行
		sprintf(line, "< 武器特殊加成>");
		pIcon->AddToolTipLine(line, RGB_HALF(255, 0, 0));
		sprintf(line, "- PVP破魔增加5%%");
		pIcon->AddToolTipLine(line, RGBA_MERGE(RGB_HALF(180, 0, 255), 255));
		sprintf(line, "- PVE破魔增加15%%");
		pIcon->AddToolTipLine(line, RGBA_MERGE(RGB_HALF(180, 0, 255), 255));
		sprintf(line, "- 使用技能(含内功技能)时，技能伤害增加5%%，");
		pIcon->AddToolTipLine(line, RGBA_MERGE(RGB_HALF(180, 0, 255), 255));
		sprintf(line, "  当角色血量< 50%%时，技能伤害增加15%%");
		pIcon->AddToolTipLine(line, RGBA_MERGE(RGB_HALF(180, 0, 255), 255));
	}
	break;
		
	}
	

	AddItemDescriptionToolTip( pIcon, pInfo->ItemTooltipIdx );
	if(!bIsQuey )
	{
		int ItemPos = -1;
		if(pcItem)
			ItemPos = pcItem->GetPosition();
		else if(pExcItem)
			ItemPos = pExcItem->GetPosition();
		if(ItemPos!=-1&&ItemPos>=80&&ItemPos<=89)
		{
			pIcon->SetRelationToolTip(NULL);
			pIcon->SetRelationToolTip1(NULL);
			SetItemRelationTip(pIcon->GetToolTip(),ItemPos);			
		}
		else
		{
			CItem * pTemp = NULL;
			CItem * pTemp1 = NULL;
			BOOL CompareMsg = false;
			if (pInfo->ItemKind == eEQUIP_ITEM_WEAPON)
				pTemp = GAMEIN->GetInventoryDialog()->GetItemForPos(81);
			else if (pInfo->ItemKind == eEQUIP_ITEM_DRESS)
				pTemp = GAMEIN->GetInventoryDialog()->GetItemForPos(82);
			else if (pInfo->ItemKind == eEQUIP_ITEM_HAT)
				pTemp = GAMEIN->GetInventoryDialog()->GetItemForPos(80);
			else if (pInfo->ItemKind == eEQUIP_ITEM_SHOES)
				pTemp = GAMEIN->GetInventoryDialog()->GetItemForPos(83);
			else if (pInfo->ItemKind == eEQUIP_ITEM_CAPE)
				pTemp = GAMEIN->GetInventoryDialog()->GetItemForPos(86);
			else if (pInfo->ItemKind == eEQUIP_ITEM_NECKLACE)
				pTemp = GAMEIN->GetInventoryDialog()->GetItemForPos(87);
			else if (pInfo->ItemKind == eEQUIP_ITEM_ARMLET)
				pTemp = GAMEIN->GetInventoryDialog()->GetItemForPos(88);
			else if (pInfo->ItemKind == eEQUIP_ITEM_BELT)
				pTemp = GAMEIN->GetInventoryDialog()->GetItemForPos(89);
			else if (pInfo->ItemKind == eEQUIP_ITEM_RING)
			{
				pTemp = GAMEIN->GetInventoryDialog()->GetItemForPos(84);
				pTemp1 = GAMEIN->GetInventoryDialog()->GetItemForPos(85);
			}
			if (pTemp)
			{
				pIcon->SetRelationToolTip(((cIcon*)pTemp)->GetToolTip());
				CompareMsg = true;
			}
			else
			{
				pIcon->SetRelationToolTip(NULL);
			}
			if (pTemp1)
			{
				pIcon->SetRelationToolTip1(((cIcon*)pTemp1)->GetToolTip());
				CompareMsg = true;
			}
			else
			{
				pIcon->SetRelationToolTip1(NULL);
			}

			if (CompareMsg)
			{
				pIcon->AddToolTipLine("");
				pIcon->AddToolTipLine(CHATMGR->GetChatMsg(2684), RGB(0, 255, 0));
			}
		}
	}
	else
	{
		CItem * pTemp = NULL;
		CItem * pTemp1 = NULL;
		BOOL CompareMsg = false;
		if (pInfo->ItemKind == eEQUIP_ITEM_WEAPON)
			pTemp = GAMEIN->GetInventoryDialog()->GetItemForPos(81);
		else if (pInfo->ItemKind == eEQUIP_ITEM_DRESS)
			pTemp = GAMEIN->GetInventoryDialog()->GetItemForPos(82);
		else if (pInfo->ItemKind == eEQUIP_ITEM_HAT)
			pTemp = GAMEIN->GetInventoryDialog()->GetItemForPos(80);
		else if (pInfo->ItemKind == eEQUIP_ITEM_SHOES)
			pTemp = GAMEIN->GetInventoryDialog()->GetItemForPos(83);
		else if (pInfo->ItemKind == eEQUIP_ITEM_CAPE)
			pTemp = GAMEIN->GetInventoryDialog()->GetItemForPos(86);
		else if (pInfo->ItemKind == eEQUIP_ITEM_NECKLACE)
			pTemp = GAMEIN->GetInventoryDialog()->GetItemForPos(87);
		else if (pInfo->ItemKind == eEQUIP_ITEM_ARMLET)
			pTemp = GAMEIN->GetInventoryDialog()->GetItemForPos(88);
		else if (pInfo->ItemKind == eEQUIP_ITEM_BELT)
			pTemp = GAMEIN->GetInventoryDialog()->GetItemForPos(89);
		else if (pInfo->ItemKind == eEQUIP_ITEM_RING)
		{
			pTemp = GAMEIN->GetInventoryDialog()->GetItemForPos(84);
			pTemp1 = GAMEIN->GetInventoryDialog()->GetItemForPos(85);
		}
		if (pTemp)
		{
			pIcon->SetRelationToolTip(((cIcon*)pTemp)->GetToolTip());
			CompareMsg = true;
		}
		else
		{
			pIcon->SetRelationToolTip(NULL);
		}
			
		if (pTemp1)
		{
			pIcon->SetRelationToolTip1(((cIcon*)pTemp1)->GetToolTip());
			CompareMsg = true;
		}
		else
		{
			pIcon->SetRelationToolTip1(NULL);
		}


		if (CompareMsg)
		{
			pIcon->AddToolTipLine("");
			pIcon->AddToolTipLine(CHATMGR->GetChatMsg(2684), RGB(0, 255, 0));
		}
		
	}
	//pIcon->AddToolTipLine(JACKJACK);
}
void CItemManager::SetItemToolTipForStage( cIcon * pIcon, ITEM_INFO * pInfo )
{
	BYTE bStage = HERO->GetStage();
	switch( bStage )
	{
	case eStage_Normal:
		{
			switch( pInfo->LimitJob )
			{
			case eItemStage_ChangeStage:	pIcon->AddToolTipLine( CHATMGR->GetChatMsg(888),	TTTC_LIMIT );	break;
			case eItemStage_ChangeStage2:	pIcon->AddToolTipLine( CHATMGR->GetChatMsg(1354),	TTTC_LIMIT );	break;
			case eItemStage_OverHwa:		pIcon->AddToolTipLine( CHATMGR->GetChatMsg(894),	TTTC_LIMIT );	break;
			case eItemStage_Hwa:			pIcon->AddToolTipLine( CHATMGR->GetChatMsg(895),	TTTC_LIMIT );	break;
			case eItemStage_Hyun:			pIcon->AddToolTipLine( CHATMGR->GetChatMsg(896),	TTTC_LIMIT );	break;
			case eItemStage_OverGeuk:		pIcon->AddToolTipLine( CHATMGR->GetChatMsg(897),	TTTC_LIMIT );	break;
			case eItemStage_Geuk:			pIcon->AddToolTipLine( CHATMGR->GetChatMsg(898),	TTTC_LIMIT );	break;
			case eItemStage_Tal:			pIcon->AddToolTipLine( CHATMGR->GetChatMsg(899),	TTTC_LIMIT );	break;
			}
		}
		break;
	case eStage_Hwa:
		{
			switch( pInfo->LimitJob )
			{
			case eItemStage_ChangeStage:	pIcon->AddToolTipLine( CHATMGR->GetChatMsg(888), TTTC_FREELIMIT );	break;
			case eItemStage_ChangeStage2:	pIcon->AddToolTipLine( CHATMGR->GetChatMsg(1354), TTTC_LIMIT);	break;
			case eItemStage_OverHwa:	pIcon->AddToolTipLine( CHATMGR->GetChatMsg(894), TTTC_FREELIMIT );	break;
			case eItemStage_Hwa:		pIcon->AddToolTipLine( CHATMGR->GetChatMsg(895), TTTC_FREELIMIT );	break;
			case eItemStage_Hyun:		pIcon->AddToolTipLine( CHATMGR->GetChatMsg(896), TTTC_LIMIT );	break;
			case eItemStage_OverGeuk:	pIcon->AddToolTipLine( CHATMGR->GetChatMsg(897), TTTC_LIMIT );	break;
			case eItemStage_Geuk:		pIcon->AddToolTipLine( CHATMGR->GetChatMsg(898), TTTC_LIMIT );	break;
			case eItemStage_Tal:		pIcon->AddToolTipLine( CHATMGR->GetChatMsg(899), TTTC_LIMIT );	break;
			}
		}
		break;
	case eStage_Hyun:
		{
			switch( pInfo->LimitJob )
			{
			case eItemStage_ChangeStage:	pIcon->AddToolTipLine( CHATMGR->GetChatMsg(888), TTTC_FREELIMIT );	break;
			case eItemStage_ChangeStage2:	pIcon->AddToolTipLine( CHATMGR->GetChatMsg(1354), TTTC_FREELIMIT);	break;
			case eItemStage_OverHwa:	pIcon->AddToolTipLine( CHATMGR->GetChatMsg(894), TTTC_FREELIMIT );	break;
			case eItemStage_Hwa:		pIcon->AddToolTipLine( CHATMGR->GetChatMsg(895), TTTC_FREELIMIT );	break;
			case eItemStage_Hyun:		pIcon->AddToolTipLine( CHATMGR->GetChatMsg(896), TTTC_FREELIMIT );	break;
			case eItemStage_OverGeuk:	pIcon->AddToolTipLine( CHATMGR->GetChatMsg(897), TTTC_LIMIT );	break;
			case eItemStage_Geuk:		pIcon->AddToolTipLine( CHATMGR->GetChatMsg(898), TTTC_LIMIT );	break;
			case eItemStage_Tal:		pIcon->AddToolTipLine( CHATMGR->GetChatMsg(899), TTTC_LIMIT );	break;
			}
		}
		break;
	case eStage_Geuk:
		{
			switch( pInfo->LimitJob )
			{
			case eItemStage_ChangeStage:	pIcon->AddToolTipLine( CHATMGR->GetChatMsg(888), TTTC_FREELIMIT );	break;
			case eItemStage_ChangeStage2:	pIcon->AddToolTipLine( CHATMGR->GetChatMsg(1354), TTTC_LIMIT);	break;
			case eItemStage_OverHwa:	pIcon->AddToolTipLine( CHATMGR->GetChatMsg(894), TTTC_LIMIT );	break;
			case eItemStage_Hwa:		pIcon->AddToolTipLine( CHATMGR->GetChatMsg(895), TTTC_LIMIT );	break;
			case eItemStage_Hyun:		pIcon->AddToolTipLine( CHATMGR->GetChatMsg(896), TTTC_LIMIT );	break;
			case eItemStage_OverGeuk:	pIcon->AddToolTipLine( CHATMGR->GetChatMsg(897), TTTC_FREELIMIT );	break;
			case eItemStage_Geuk:		pIcon->AddToolTipLine( CHATMGR->GetChatMsg(898), TTTC_FREELIMIT );	break;
			case eItemStage_Tal:		pIcon->AddToolTipLine( CHATMGR->GetChatMsg(899), TTTC_LIMIT );	break;
			}
		}
		break;
	case eStage_Tal:
		{
			switch( pInfo->LimitJob )
			{
			case eItemStage_ChangeStage:	pIcon->AddToolTipLine( CHATMGR->GetChatMsg(888), TTTC_FREELIMIT );	break;
			case eItemStage_ChangeStage2:	pIcon->AddToolTipLine( CHATMGR->GetChatMsg(1354), TTTC_FREELIMIT);	break;
			case eItemStage_OverHwa:	pIcon->AddToolTipLine( CHATMGR->GetChatMsg(894), TTTC_LIMIT );	break;
			case eItemStage_Hwa:		pIcon->AddToolTipLine( CHATMGR->GetChatMsg(895), TTTC_LIMIT );	break;
			case eItemStage_Hyun:		pIcon->AddToolTipLine( CHATMGR->GetChatMsg(896), TTTC_LIMIT );	break;
			case eItemStage_OverGeuk:	pIcon->AddToolTipLine( CHATMGR->GetChatMsg(897), TTTC_FREELIMIT );	break;
			case eItemStage_Geuk:		pIcon->AddToolTipLine( CHATMGR->GetChatMsg(898), TTTC_FREELIMIT );	break;
			case eItemStage_Tal:		pIcon->AddToolTipLine( CHATMGR->GetChatMsg(899), TTTC_FREELIMIT );	break;
			}
		}
		break;
	}
}
void CItemManager::SetPetSummonItemToolTip(cIcon * pIcon, ITEM_INFO * pInfo, DWORD dwItemDBIdx, BOOL IsGoldShop)
{
	char line[128];
	DWORD	ItemDBIdx = 0;
	const ITEMBASE* pBaseInfo = ((CItem*)pIcon)->GetItemBaseInfo();
	if(dwItemDBIdx)
	{
		ItemDBIdx = dwItemDBIdx;
	}
	else
	{
		ItemDBIdx = pBaseInfo->dwDBIdx;
	}

	PET_TOTALINFO* pPetInfo = PETMGR->GetPetInfo(ItemDBIdx);

	BASE_PET_LIST* pList = NULL;
	if(pPetInfo)
	{
		pList = GAMERESRCMNGR->GetPetListInfo(pPetInfo->PetKind);
	}

	if (IsGoldShop)
	{
		pIcon->AddToolTipLine("");
	}

	PET_BUFF_LIST* pBuffList = NULL;
	if( pPetInfo )
	{
		if( !pPetInfo->bAlive )
		{
			DWORD RedFilter = RGBA_MAKE(255,10,10,255);
			SetIconColorFilter(pIcon,RedFilter);
		}

		wsprintf( line, CHATMGR->GetChatMsg(1244), pPetInfo->PetGrade );
		pIcon->AddToolTipLine( line, TTTC_QUESTITEM );//kiv

		if(0 < pPetInfo->PetFriendly && pPetInfo->PetFriendly <= 100000)
		{
			wsprintf( line, CHATMGR->GetChatMsg(1245), 1 );
		}
		else
		{
			wsprintf( line, CHATMGR->GetChatMsg(1245), pPetInfo->PetFriendly/100000 );
		}
	}
	else
	{
		wsprintf( line, CHATMGR->GetChatMsg(1244), PET_DEFAULT_GRADE );//kiv
		pIcon->AddToolTipLine( line, TTTC_QUESTITEM );//kiv
		DWORD friendly = GAMERESRCMNGR->GetPetRule()->DefaultFriendship;//kiv
		wsprintf( line, CHATMGR->GetChatMsg(1245), friendly/100000);//kiv
		
	}
	pIcon->AddToolTipLine( line, TTTC_QUESTITEM );
	//BOOL IsGoldShop = true;
	
	if (IsGoldShop && pInfo->ItemIdx == 55889)//传奇鵰
	{
		pList = new BASE_PET_LIST;
		pPetInfo = new PET_TOTALINFO;
		pPetInfo->PetGrade = 1;
		pPetInfo->PetFriendly = 10000000;
		pList->BuffList[0][0] = 46;
		pList->BuffList[0][1] = 13;
		pList->BuffList[0][2] = 1;
		pList->InventoryTapNum[0] = 3;
	}
	else if (IsGoldShop && pInfo->ItemIdx == 55890)	//灵猫
	{
		pList = new BASE_PET_LIST;
		pPetInfo = new PET_TOTALINFO;
		pPetInfo->PetGrade = 1;
		pPetInfo->PetFriendly = 10000000;
		pList->BuffList[0][0] = 46;
		pList->BuffList[0][1] = 13;
		pList->BuffList[0][2] = 10;
		pList->InventoryTapNum[0] = 3;
	}
	else if (IsGoldShop && pInfo->ItemIdx == 55891)//金瞳猴
	{
		pList = new BASE_PET_LIST;
		pPetInfo = new PET_TOTALINFO;
		pPetInfo->PetGrade = 1;
		pPetInfo->PetFriendly = 10000000;
		pList->BuffList[0][0] = 46;
		pList->BuffList[0][1] = 13;
		pList->BuffList[0][2] = 25;
		pList->InventoryTapNum[0] = 3;
	}
	else if (IsGoldShop && pInfo->ItemIdx == 55892)//吊睛白额虎
	{
		pList = new BASE_PET_LIST;
		pPetInfo = new PET_TOTALINFO;
		pPetInfo->PetGrade = 1;
		pPetInfo->PetFriendly = 10000000;
		pList->BuffList[0][0] = 46;
		pList->BuffList[0][1] = 13;
		pList->BuffList[0][2] = 4;
		pList->InventoryTapNum[0] = 3;
	}
	else if (IsGoldShop && pInfo->ItemIdx == 55893)//恶魔
	{
		pList = new BASE_PET_LIST;
		pPetInfo = new PET_TOTALINFO;
		pPetInfo->PetGrade = 1;
		pPetInfo->PetFriendly = 10000000;
		pList->BuffList[0][0] = 46;
		pList->BuffList[0][1] = 13;
		pList->BuffList[0][2] = 4;
		pList->InventoryTapNum[0] = 3;
	}
	else if (IsGoldShop && pInfo->ItemIdx == 55894)//雪人
	{
		pList = new BASE_PET_LIST;
		pPetInfo = new PET_TOTALINFO;
		pPetInfo->PetGrade = 1;
		pPetInfo->PetFriendly = 10000000;
		pList->BuffList[0][0] = 46;
		pList->BuffList[0][1] = 13;
		pList->BuffList[0][2] = 28;
		pList->InventoryTapNum[0] = 3;
	}
	else if (IsGoldShop && pInfo->ItemIdx == 55895)//小黑
	{
		pList = new BASE_PET_LIST;
		pPetInfo = new PET_TOTALINFO;
		pPetInfo->PetGrade = 1;
		pPetInfo->PetFriendly = 10000000;
		pList->BuffList[0][0] = 46;
		pList->BuffList[0][1] = 13;
		pList->BuffList[0][2] = 25;
		pList->InventoryTapNum[0] = 3;
	}
	else if (IsGoldShop && pInfo->ItemIdx == 55896)//天竺鼠
	{
		pList = new BASE_PET_LIST;
		pPetInfo = new PET_TOTALINFO;
		pPetInfo->PetGrade = 1;
		pPetInfo->PetFriendly = 10000000;
		pList->BuffList[0][0] = 46;
		pList->BuffList[0][1] = 13;
		pList->BuffList[0][2] = 1;
		pList->InventoryTapNum[0] = 3;
	}
	else if (IsGoldShop && pInfo->ItemIdx == 55897)//白狼
	{
		pList = new BASE_PET_LIST;
		pPetInfo = new PET_TOTALINFO;
		pPetInfo->PetGrade = 1;
		pPetInfo->PetFriendly = 10000000;
		pList->BuffList[0][0] = 46;
		pList->BuffList[0][1] = 13;
		pList->BuffList[0][2] = 10;
		pList->InventoryTapNum[0] = 3;
	}
	else if (IsGoldShop && pInfo->ItemIdx == 55898)//地狱野猪
	{
		pList = new BASE_PET_LIST;
		pPetInfo = new PET_TOTALINFO;
		pPetInfo->PetGrade = 1;
		pPetInfo->PetFriendly = 10000000;
		pList->BuffList[0][0] = 46;
		pList->BuffList[0][1] = 13;
		pList->BuffList[0][2] = 25;
		pList->InventoryTapNum[0] = 3;
	}
	else if (IsGoldShop && pInfo->ItemIdx == 55899)//大角羊
	{
		pList = new BASE_PET_LIST;
		pPetInfo = new PET_TOTALINFO;
		pPetInfo->PetGrade = 1;
		pPetInfo->PetFriendly = 10000000;
		pList->BuffList[0][0] = 46;
		pList->BuffList[0][1] = 13;
		pList->BuffList[0][2] = 4;
		pList->InventoryTapNum[0] = 3;
	}
	else if (IsGoldShop && pInfo->ItemIdx == 55900)//鹈鹕
	{
		pList = new BASE_PET_LIST;
		pPetInfo = new PET_TOTALINFO;
		pPetInfo->PetGrade = 1;
		pPetInfo->PetFriendly = 10000000;
		pList->BuffList[0][0] = 46;
		pList->BuffList[0][1] = 13;
		pList->BuffList[0][2] = 4;
		pList->InventoryTapNum[0] = 3;
	}
	else if (IsGoldShop && pInfo->ItemIdx == 55901)//红姬
	{
		pList = new BASE_PET_LIST;
		pPetInfo = new PET_TOTALINFO;
		pPetInfo->PetGrade = 1;
		pPetInfo->PetFriendly = 10000000;
		pList->BuffList[0][0] = 3;
		pList->BuffList[0][1] = 7;
		pList->BuffList[0][2] = 0;
		pList->InventoryTapNum[0] = 3;
	}
	else if (IsGoldShop && pInfo->ItemIdx == 55902)//卧龙
	{
		pList = new BASE_PET_LIST;
		pPetInfo = new PET_TOTALINFO;
		pPetInfo->PetGrade = 1;
		pPetInfo->PetFriendly = 10000000;
		pList->BuffList[0][0] = 10;
		pList->BuffList[0][1] = 7;
		pList->BuffList[0][2] = 0;
		pList->InventoryTapNum[0] = 3;
	}
	else if (IsGoldShop && pInfo->ItemIdx == 55903)//凤灵
	{
		pList = new BASE_PET_LIST;
		pPetInfo = new PET_TOTALINFO;
		pPetInfo->PetGrade = 1;
		pPetInfo->PetFriendly = 10000000;
		pList->BuffList[0][0] = 7;
		pList->BuffList[0][1] = 3;
		pList->BuffList[0][2] = 16;
		pList->InventoryTapNum[0] = 3;
	}
	else if (IsGoldShop && pInfo->ItemIdx == 55904)//花仙
	{
		pList = new BASE_PET_LIST;
		pPetInfo = new PET_TOTALINFO;
		pPetInfo->PetGrade = 1;
		pPetInfo->PetFriendly = 10000000;
		pList->BuffList[0][0] = 7;
		pList->BuffList[0][1] = 22;
		pList->BuffList[0][2] = 19;
		pList->InventoryTapNum[0] = 3;
	}
	else if (IsGoldShop && pInfo->ItemIdx == 55905)//影瞳
	{
		pList = new BASE_PET_LIST;
		pPetInfo = new PET_TOTALINFO;
		pPetInfo->PetGrade = 1;
		pPetInfo->PetFriendly = 10000000;
		pList->BuffList[0][0] = 16;
		pList->BuffList[0][1] = 25;
		pList->BuffList[0][2] = 28;
		pList->InventoryTapNum[0] = 3;
	}
	else if (IsGoldShop && pInfo->ItemIdx == 55906)//妮娜猫(365天)
	{
		pList = new BASE_PET_LIST;
		pPetInfo = new PET_TOTALINFO;
		pPetInfo->PetGrade = 1;
		pPetInfo->PetFriendly = 10000000;
		pList->BuffList[0][0] = 31;
		pList->BuffList[0][1] = 0;
		pList->BuffList[0][2] = 0;
		pList->InventoryTapNum[0] = 3;
	}
	else if (IsGoldShop && pInfo->ItemIdx == 55907)//熊喵(永久)
	{
		pList = new BASE_PET_LIST;
		pPetInfo = new PET_TOTALINFO;
		pPetInfo->PetGrade = 1;
		pPetInfo->PetFriendly = 10000000;
		pList->BuffList[0][0] = 7;
		pList->BuffList[0][1] = 10;
		pList->BuffList[0][2] = 22;
		pList->InventoryTapNum[0] = 3;
	}
	else if (IsGoldShop && pInfo->ItemIdx == 55919)//巨陵魂魄
	{
		pList = new BASE_PET_LIST;
		pPetInfo = new PET_TOTALINFO;
		pPetInfo->PetGrade = 1;
		pPetInfo->PetFriendly = 10000000;
		pList->BuffList[0][0] = 3;
		pList->BuffList[0][1] = 4;
		pList->BuffList[0][2] = 8;
		pList->InventoryTapNum[0] = 3;
	}
	else if (IsGoldShop && pInfo->ItemIdx == 55920)//蚩牛魂魄
	{
		pList = new BASE_PET_LIST;
		pPetInfo = new PET_TOTALINFO;
		pPetInfo->PetGrade = 1;
		pPetInfo->PetFriendly = 10000000;
		pList->BuffList[0][0] = 3;
		pList->BuffList[0][1] = 4;
		pList->BuffList[0][2] = 8;
		pList->InventoryTapNum[0] = 3;
	}
	else if (IsGoldShop && pInfo->ItemIdx == 55921)//飞龙座骑
	{
		pList = new BASE_PET_LIST;
		pPetInfo = new PET_TOTALINFO;
		pPetInfo->PetGrade = 1;
		pPetInfo->PetFriendly = 10000000;
		pList->BuffList[0][0] = 3;
		pList->BuffList[0][1] = 4;
		pList->BuffList[0][2] = 8;
		pList->InventoryTapNum[0] = 3;
	}
	

	if(pList)
	{
		wsprintf( line, CHATMGR->GetChatMsg(1266), pList->InventoryTapNum[pPetInfo->PetGrade-1]);
		pIcon->AddToolTipLine( line, TTTC_QUESTITEM );
	}
	else
	{
	
		AddItemDescriptionToolTip( pIcon, pInfo->wSetItemKind,TTTC_QUESTITEM,0 );//
		pIcon->AddToolTipLine( "" );
		return;
	}

	if(pList->BuffList[0])
	{
		pIcon->AddToolTipLine( CHATMGR->GetChatMsg(1267), TTTC_QUESTITEM );
		char BuffName[32] = {0,};
		for( int i = 0; i < MAX_PET_BUFF_NUM; ++i )
		{
			WORD BuffIdx = pList->BuffList[pPetInfo->PetGrade-1][i];
			if(0 == BuffIdx) continue;
			pBuffList = GAMERESRCMNGR->GetPetBuffInfo(BuffIdx);
			if(pBuffList && pBuffList->BuffValueData)
			{
				if( pBuffList->BuffKind == ePB_Demage_Percent )
				{
					wsprintf( BuffName, CHATMGR->GetChatMsg(1270), pBuffList->BuffValueData );
				}
				else if( pBuffList->BuffKind == ePB_Dodge )
				{
					wsprintf( BuffName, CHATMGR->GetChatMsg(1271), pBuffList->BuffSuccessProb );
				}
				else if( pBuffList->BuffKind == ePB_MasterAllStatUp )
				{
					wsprintf( BuffName, CHATMGR->GetChatMsg(1286), pBuffList->BuffValueData );
				}
				else if( pBuffList->BuffKind == ePB_Item_DoubleChance )
				{
					wsprintf( BuffName, CHATMGR->GetChatMsg(1305), pBuffList->BuffSuccessProb );
				}
				else if( pBuffList->BuffKind == ePB_NoForeAtkMonster )
				{
					wsprintf( BuffName, CHATMGR->GetChatMsg(1350) );
				}
				else if( pBuffList->BuffKind == ePB_ReduceCriticalDmg )
				{
					wsprintf( BuffName, CHATMGR->GetChatMsg(1388), pBuffList->BuffValueData );
				}
				else if( pBuffList->BuffKind == ePB_MasterAllStatRound )
				{
					wsprintf( BuffName, CHATMGR->GetChatMsg(1389) );
				}
				else if( pBuffList->BuffKind == ePB_Item_RareProbUp )
				{
					wsprintf( BuffName, CHATMGR->GetChatMsg(1476) );
				}
				else if( pBuffList->BuffKind == ePB_MussangTimeIncrease )
				{
					wsprintf( BuffName, CHATMGR->GetChatMsg(1477) );
				}

				//--------------------------
//				else if (pBuffList->BuffKind == ePB_ReduceCriticalRate)
//				{
//					wsprintf(BuffName, CHATMGR->GetChatMsg(2410), pBuffList->BuffSuccessProb, pBuffList->BuffValueData);
//				}
//				else if (pBuffList->BuffKind == ePB_ReduceDemageRate)
//				{
//					wsprintf(BuffName, CHATMGR->GetChatMsg(2411), pBuffList->BuffSuccessProb, pBuffList->BuffValueData);
//				}
//				else if (pBuffList->BuffKind == ePB_ReduceCriticalDmgPlayer)
//				{
//					wsprintf(BuffName, CHATMGR->GetChatMsg(2412), pBuffList->BuffSuccessProb, pBuffList->BuffValueData);
//				}
				//--------------------------
			}
			wsprintf( line, CHATMGR->GetChatMsg(1268), BuffName );
			pIcon->AddToolTipLine( line, TTTC_QUESTITEM );
		}
	}
	
	wsprintf( line, CHATMGR->GetChatMsg(1281), pInfo->LimitLevel );
	pIcon->AddToolTipLine( line, TTTC_QUESTITEM	);
	if (IsGoldShop)
	{
		pIcon->AddToolTipLine("");
	}
}
void CItemManager::SetTitanEquipItemToolTip(cIcon * pIcon, ITEM_INFO * pInfo)
{
	char line[128];
	CItem* pItem = (CItem*)pIcon;
	if( pInfo->LimitLevel != 0 )
	{		
		sprintf(line, CHATMGR->GetChatMsg(238), pInfo->LimitLevel);
		if( HERO->GetCharacterTotalInfo()->Level < pInfo->LimitLevel )
			pIcon->AddToolTipLine( line, TTTC_LIMIT );
		else
			pIcon->AddToolTipLine( line, TTTC_FREELIMIT );				
	}
	if(HERO->InTitan() == TRUE)
		pIcon->AddToolTipLine( CHATMGR->GetChatMsg(1655), TTTC_FREELIMIT );
	else
		pIcon->AddToolTipLine( CHATMGR->GetChatMsg(1655), TTTC_LIMIT );
	pIcon->AddToolTipLine("");
	if(pInfo->MeleeAttackMin > 0)
	{
		ZeroMemory(&line, sizeof(line));
		wsprintf( line, CHATMGR->GetChatMsg(1521), pInfo->MeleeAttackMin);
		pIcon->AddToolTipLine( line, TTTC_EXTRAATTR );
	}
	if(pInfo->MeleeAttackMax > 0)
	{
		ZeroMemory(&line, sizeof(line));
		wsprintf( line, CHATMGR->GetChatMsg(1522), pInfo->MeleeAttackMax);
		pIcon->AddToolTipLine( line, TTTC_EXTRAATTR );
	}
	if(pInfo->RangeAttackMin > 0)
	{
		ZeroMemory(&line, sizeof(line));
		wsprintf( line, CHATMGR->GetChatMsg(1523), pInfo->RangeAttackMin);
		pIcon->AddToolTipLine( line, TTTC_EXTRAATTR );
	}
	if(pInfo->RangeAttackMax > 0)
	{
		ZeroMemory(&line, sizeof(line));
		wsprintf( line, CHATMGR->GetChatMsg(1524), pInfo->RangeAttackMax);
		pIcon->AddToolTipLine( line, TTTC_EXTRAATTR );
	}
	if(pInfo->CriticalPercent > 0)
	{
		ZeroMemory(&line, sizeof(line));
		wsprintf( line, CHATMGR->GetChatMsg(1525), pInfo->CriticalPercent);
		pIcon->AddToolTipLine( line, TTTC_EXTRAATTR );
	}
	if(pInfo->MugongNum > 0)
	{
		ZeroMemory(&line, sizeof(line));
		wsprintf( line, CHATMGR->GetChatMsg(1526), pInfo->MugongNum);
		pIcon->AddToolTipLine( line, TTTC_EXTRAATTR );
	}
	if(pInfo->PhyDef > 0)
	{
		ZeroMemory(&line, sizeof(line));
		wsprintf( line, CHATMGR->GetChatMsg(1527), pInfo->PhyDef);
		pIcon->AddToolTipLine( line, TTTC_EXTRAATTR );
	}
	if(pInfo->Life > 0)
	{
		ZeroMemory(&line, sizeof(line));
		wsprintf( line, CHATMGR->GetChatMsg(1528), pInfo->Life);
		pIcon->AddToolTipLine( line, TTTC_EXTRAATTR );
	}
	if(pInfo->AttrAttack.GetElement_Val(ATTR_FIRE) > 0)
	{
		ZeroMemory(&line, sizeof(line));
		sprintf( line, CHATMGR->GetChatMsg(1667), pInfo->AttrAttack.GetElement_Val(ATTR_FIRE));
		pIcon->AddToolTipLine( line, TTTC_EXTRAATTR );
	}
	if(pInfo->AttrRegist.GetElement_Val(ATTR_FIRE) > 0)
	{
		ZeroMemory(&line, sizeof(line));
		sprintf( line, CHATMGR->GetChatMsg(1530), pInfo->AttrRegist.GetElement_Val(ATTR_FIRE));
		pIcon->AddToolTipLine( line, TTTC_EXTRAATTR );
	}
	if(pInfo->AttrRegist.GetElement_Val(ATTR_WATER) > 0)
	{
		ZeroMemory(&line, sizeof(line));
		sprintf( line, CHATMGR->GetChatMsg(1531), pInfo->AttrRegist.GetElement_Val(ATTR_WATER));
		pIcon->AddToolTipLine( line, TTTC_EXTRAATTR );
	}
	if(pInfo->AttrRegist.GetElement_Val(ATTR_TREE) > 0)
	{
		ZeroMemory(&line, sizeof(line));
		sprintf( line, CHATMGR->GetChatMsg(1532), pInfo->AttrRegist.GetElement_Val(ATTR_TREE));
		pIcon->AddToolTipLine( line, TTTC_EXTRAATTR );
	}
	if(pInfo->AttrRegist.GetElement_Val(ATTR_IRON) > 0)
	{
		ZeroMemory(&line, sizeof(line));
		sprintf( line, CHATMGR->GetChatMsg(1533), pInfo->AttrRegist.GetElement_Val(ATTR_IRON));
		pIcon->AddToolTipLine( line, TTTC_EXTRAATTR );
	}
	if(pInfo->AttrRegist.GetElement_Val(ATTR_EARTH) > 0)
	{
		ZeroMemory(&line, sizeof(line));
		sprintf( line, CHATMGR->GetChatMsg(1534), pInfo->AttrRegist.GetElement_Val(ATTR_EARTH));
		pIcon->AddToolTipLine( line, TTTC_EXTRAATTR );
	}
	if(pInfo->WeaponType > 0)
	{		
		switch(pInfo->WeaponType)
		{
		case WP_GUM:	
			sprintf(line, CHATMGR->GetChatMsg(1661));
			break;
		case WP_GWUN:	
			sprintf(line, CHATMGR->GetChatMsg(1663));
			break;
		case WP_DO:		
			sprintf(line, CHATMGR->GetChatMsg(1662));
			break;
		case WP_CHANG:	
			sprintf(line, CHATMGR->GetChatMsg(1664));
			break;
		case WP_GUNG:	
			sprintf(line, CHATMGR->GetChatMsg(1665));
			break;
		case WP_AMGI:	
			sprintf(line, CHATMGR->GetChatMsg(1666));
			break;
		}
		pIcon->AddToolTipLine( line );
	}	
}
void CItemManager::AddItemDescriptionToolTip( cWindow* pWindow, WORD wIndex, DWORD dwTxtColor, int nSpaceLine )
{
	pWindow->AddToolTipLine("");
	const TOOLTIP_TEXT* pTooltipText = m_ItemToolTipTextList.GetData( wIndex );
	/*if( pTooltipText )
	{
		//pWindow->AddToolTipLine("");
		for( int i = 0 ; i < nSpaceLine ; ++i )
		{
			pWindow->AddToolTipLine("");
		}
	}*/
	while( pTooltipText )
	{
		/*for( int i = 0 ; i < nSpaceLine ; ++i )
		{
			pWindow->AddToolTipLine("");
		}*/
		pWindow->AddToolTipLine( pTooltipText->strToolTip, dwTxtColor );
		pTooltipText = pTooltipText->pNext;
	}
	//pWindow->AddToolTipLine("");
}
void CItemManager::ItemDelete(CItem* pItem)
{
	m_ItemHash.Remove(pItem->GetDBIdx());	
	m_IconIndexCreator.ReleaseIndex(pItem->GetID());
	if( pItem->GetLinkItem() )
	{
		GAMEIN->GetExchangeDialog()->DelItem( 0, pItem->GetLinkItem()->GetPosition() );
	}
	WINDOWMGR->AddListDestroyWindow( pItem );
}
void CItemManager::Release()
{
	CItem* pItem;
	m_ItemHash.SetPositionHead();
	while(pItem = m_ItemHash.GetData())
	{
		ItemDelete(pItem);
	}
	m_ItemHash.RemoveAll();
	ITEM_OPTION_INFO * pOInfo = NULL;
	m_ItemOptionList.SetPositionHead();
	while(pOInfo = m_ItemOptionList.GetData())
		delete pOInfo;
	m_ItemOptionList.RemoveAll();
	ITEM_RARE_OPTION_INFO* pRareInfo = NULL;
	m_ItemRareOptionList.SetPositionHead();
	while(pRareInfo = m_ItemRareOptionList.GetData())
		delete pRareInfo;
	m_ItemRareOptionList.RemoveAll();
	GAMEIN->GetInventoryDialog()->ReleaseInventory();
	m_nItemUseCount = 0;
}
CItem* CItemManager::GetItem(DWORD dwDBIdx)
{
	CItem* pItem;
	pItem = m_ItemHash.GetData(dwDBIdx);
	return pItem;
}
void CItemManager::CombineItem( MSG_ITEM_COMBINE_ACK * pMsg )
{	
	WORD FromTableIdx = GetTableIdxForAbsPos(pMsg->FromPos);
	WORD ToTableIdx = GetTableIdxForAbsPos(pMsg->ToPos);
	CItem * pFromItem = GetItemofTable(FromTableIdx, pMsg->FromPos);
	CItem * pToItem = GetItemofTable(ToTableIdx, pMsg->ToPos);
	if( !pFromItem || !pToItem )
	{
		//ASSERTMSG(0, "Item Combine Failed : pToItem=NULL or pFromItem=NULL");
		return;
	}
	if( ( pFromItem->GetItemIdx() != pToItem->GetItemIdx() ) || ( pFromItem->GetItemIdx() != pMsg->wItemIdx) )
	{
		//ASSERTMSG(0, "Item Combine Failed : FromItemIdx != ToItemIdx");
		return;
	}
	pFromItem->SetDurability( pMsg->FromDur );
	pToItem->SetDurability( pMsg->ToDur );
	if( pMsg->FromDur == 0 )
	{
		DeleteItemofTable(FromTableIdx, pFromItem->GetPosition());
	}
	if( pMsg->ToDur == 0 )
	{
		DeleteItemofTable(FromTableIdx, pFromItem->GetPosition());
	}
}
void CItemManager::MoveItem( MSG_ITEM_MOVE_ACK * pMsg )
{
 	BYTE tableIdx = GetTableIdxForAbsPos(pMsg->ToPos);
	if(tableIdx == eItemTable_Pyoguk)
		MoveItemToPyoguk(pMsg);
	else if(tableIdx == eItemTable_GuildWarehouse)
		MoveItemToGuild(pMsg);
	else if(tableIdx == eItemTable_Shop)
		MoveItemToShop(pMsg);
	else if(tableIdx == eItemTable_ShopInven)
		MoveItemToShopInven(pMsg);
	else if(tableIdx == eItemTable_PetInven || tableIdx == eItemTable_PetWeared )
	{
		MoveItemToPetInven(pMsg);
	}
	else if(tableIdx == eItemTable_Titan)
		MoveItemToTitanInven(pMsg);
	else if(tableIdx == eItemTable_TitanShopItem)
		MoveItemToTitanShopInven(pMsg);
	else if(tableIdx == eItemTable_Inventory || tableIdx == eItemTable_Weared)
	{
		CInventoryExDialog * pInven = GAMEIN->GetInventoryDialog();
		CPyogukDialog* pPyoguk = GAMEIN->GetPyogukDialog();
		CGuildWarehouseDialog* pGuild = GAMEIN->GetGuildWarehouseDlg();
		CItemShopDialog* pShop = GAMEIN->GetItemShopDialog();
		CPetInventoryDlg* pPetInven = GAMEIN->GetPetInventoryDialog();		
		CTitanInventoryDlg* pTitanInven = GAMEIN->GetTitanInventoryDlg();
		CItem * FromItem = NULL;
		CItem * ToItem = NULL;
		pInven->DeleteItem(pMsg->ToPos, &ToItem);
		WORD FromTableIdx = GetTableIdxForAbsPos(pMsg->FromPos);
		if(FromTableIdx == eItemTable_Pyoguk)
		{
			pPyoguk->DeleteItem( pMsg->FromPos, &FromItem );
		}
		else if(FromTableIdx == eItemTable_GuildWarehouse)
		{
			pGuild->DeleteItem( pMsg->FromPos, &FromItem );
		}
		else if(FromTableIdx == eItemTable_Shop)
		{
			pShop->DeleteItem( pMsg->FromPos, &FromItem );
		}
		else if(FromTableIdx == eItemTable_PetInven)
		{
			pPetInven->DeleteItem( pMsg->FromPos, &FromItem );
		}
		else if(FromTableIdx == eItemTable_Titan)
		{
            pTitanInven->DeleteItem( pMsg->FromPos, &FromItem );
			HERO->GetTitanAppearInfo()->WearedItemIdx[pMsg->FromPos-TP_TITANWEAR_START] = 0;
			TITANMGR->RemoveTitanUsingEquipItemList(FromItem->GetDBIdx());
		}
		else 
			pInven->DeleteItem( pMsg->FromPos , &FromItem );
		if(ToItem)
		{
			ToItem->SetPosition( pMsg->FromPos );
			ITEM_INFO* pinfo = GetItemInfo( ToItem->GetItemBaseInfo()->wIconIdx );
			if( pinfo )
			{
				POSTYPE pos = 0;
				if( (pinfo->ItemKind & eEQUIP_ITEM) && pInven->GetBlankPositionRestrictRef( pos ) )
				{
					if( TP_SHOPINVEN_START <= pMsg->FromPos && pMsg->FromPos < TP_SHOPINVEN_END/*-40*/ )
					{
						FromTableIdx = eItemTable_Inventory;                        
						ToItem->SetPosition( pos );						
					}
				}
				else if( (pinfo->ItemKind == eSHOP_ITEM_EQUIP) && pInven->GetShopInven()->GetBlankPositionRestrictRef( pos ) )
				{
					if( TP_INVENTORY_START <= pMsg->FromPos && pMsg->FromPos < TP_INVENTORY_END )
					{
						FromTableIdx = eItemTable_ShopInven;
						ToItem->SetPosition( pos );
					}
				}
			}
			if(FromTableIdx == eItemTable_Pyoguk)
			{
				if(ToItem->GetQuickPosition())
					QUICKMGR->RemQuickItem(ToItem->GetQuickPosition());
				pPyoguk->AddItem(ToItem);
			}
			else if(FromTableIdx == eItemTable_GuildWarehouse)
			{
				if(ToItem->GetQuickPosition())
					QUICKMGR->RemQuickItem(ToItem->GetQuickPosition());
				pGuild->AddItem(ToItem);
				QUICKMGR->RefreshQickItem();
			}
			else if(FromTableIdx == eItemTable_PetInven)
			{
				if(POSTYPE Pos = ToItem->GetQuickPosition())
					QUICKMGR->RemQuickItem(Pos);
				pPetInven->AddItem(ToItem);
			}
			else if(FromTableIdx == eItemTable_Titan)
			{
				if(POSTYPE Pos = ToItem->GetQuickPosition())
					QUICKMGR->RemQuickItem(Pos);
				pTitanInven->AddItem(ToItem);
				if( HERO->IsTitanPreView() )
				{
					HERO->GetTitanPreViewInfo()->WearedItemIdx[pMsg->FromPos-TP_TITANWEAR_START] = ToItem->GetItemIdx();
				}
			}
			else
				pInven->AddItem(ToItem);
		}
		if(FromItem == 0)
		{
			ASSERT(0);
			return;
		}
		FromItem->SetPosition(pMsg->ToPos);
		if (pMsg->FromPos >= 80 && pMsg->FromPos <= 89)
			SetItemRelationTip(NULL, pMsg->FromPos);

		pInven->AddItem(FromItem);
		QUICKMGR->RefreshQickItem();
		APPEARANCEMGR->InitAppearance( HERO );
		if(HERO->InTitan() == TRUE)
		{			
			GAMEIN->GetTitanGuageDlg()->SetLife(TITANMGR->GetCurRidingTitan()->GetTitanTotalInfo()->Fuel);
		}
		if(((ToItem != NULL) && (ToItem->GetItemInfo()->wSetItemKind != 0)) || (FromItem->GetItemInfo()->wSetItemKind != 0))
		{
			RefreshAllItem();		
		}		
	}
}
void CItemManager::MoveItemToGuild( MSG_ITEM_MOVE_ACK* pMsg )
{	
	CGuildWarehouseDialog * pGuildDlg	= GAMEIN->GetGuildWarehouseDlg();
	CInventoryExDialog * pInvenDlg = GAMEIN->GetInventoryDialog();
	WORD FromTableIdx = GetTableIdxForAbsPos(pMsg->FromPos);
	WORD ToTableIdx = GetTableIdxForAbsPos(pMsg->ToPos);
	CItem * FromItem = NULL;
	CItem * ToItem = NULL;
	pGuildDlg->DeleteItem(pMsg->ToPos , &ToItem);
	if(FromTableIdx == eItemTable_GuildWarehouse)
	{
		pGuildDlg->DeleteItem( pMsg->FromPos, &FromItem);
	}
	else if(FromTableIdx == eItemTable_Inventory || FromTableIdx == eItemTable_Weared)
	{
		pInvenDlg->DeleteItem( pMsg->FromPos, &FromItem);
	}
	else 
		ASSERT(0);
	if(ToItem)
	{
		ToItem->SetPosition( pMsg->FromPos );
		if(FromTableIdx == eItemTable_Inventory)
			pInvenDlg->AddItem(ToItem);
		else
			pGuildDlg->AddItem(ToItem);
	}
	ReLinkQuickPosition(FromItem);
	if(FromItem->GetQuickPosition())
		QUICKMGR->RemQuickItem(FromItem->GetQuickPosition());
	ASSERT(FromItem);
	FromItem->SetPosition(pMsg->ToPos);
	FromItem->SetQuickPosition(0);
	pGuildDlg->AddItem(FromItem);
	QUICKMGR->RefreshQickItem();
	SetDisableDialog(FALSE, eItemTable_Inventory);
	SetDisableDialog(FALSE, eItemTable_Pyoguk);
	SetDisableDialog(FALSE, eItemTable_GuildWarehouse);
}
void CItemManager::MoveItemToPyoguk(MSG_ITEM_MOVE_ACK* pMsg)
{
	CInventoryExDialog * pInven = GAMEIN->GetInventoryDialog();
	CPyogukDialog* pPyoguk = GAMEIN->GetPyogukDialog();
	CItem * FromItem = NULL;
	CItem * ToItem = NULL;
	pPyoguk->DeleteItem(pMsg->ToPos, &ToItem);
	WORD FromTableIdx = GetTableIdxForAbsPos(pMsg->FromPos);
	if(FromTableIdx == eItemTable_Inventory || FromTableIdx == eItemTable_Weared)
	{
		pInven->DeleteItem( pMsg->FromPos, &FromItem );				
	}
	else if(FromTableIdx == eItemTable_Pyoguk)
	{
		pPyoguk->DeleteItem( pMsg->FromPos, &FromItem );
	}
	else
	{
		ASSERT(0);
		return;
	}
	if(ToItem)
	{
		ToItem->SetPosition( pMsg->FromPos );
		if(FromTableIdx == eItemTable_Inventory || FromTableIdx == eItemTable_Weared)
			pInven->AddItem(ToItem);
		else
			pPyoguk->AddItem(ToItem);
		if(ToItem->GetQuickPosition())
			QUICKMGR->RemQuickItem(ToItem->GetQuickPosition());
	}
	ReLinkQuickPosition(FromItem);
	if(FromItem->GetQuickPosition())
		QUICKMGR->RemQuickItem(FromItem->GetQuickPosition());
	ASSERT(FromItem);
	FromItem->SetPosition(pMsg->ToPos);
	FromItem->SetQuickPosition(0);
	pPyoguk->AddItem(FromItem);
	QUICKMGR->RefreshQickItem();
}
void CItemManager::MoveItemToShop(MSG_ITEM_MOVE_ACK* pMsg)
{
	CInventoryExDialog * pInven = GAMEIN->GetInventoryDialog();
	CItemShopDialog* pShop = GAMEIN->GetItemShopDialog();
	CItem * FromItem = NULL;
	CItem * ToItem = NULL;
	pShop->DeleteItem(pMsg->ToPos, &ToItem);
	WORD FromTableIdx = GetTableIdxForAbsPos(pMsg->FromPos);
	if( FromTableIdx == eItemTable_Shop )
	{
		pShop->DeleteItem( pMsg->FromPos, &FromItem );				
	}
	else if( FromTableIdx == eItemTable_ShopInven )
	{
		pInven->DeleteItem( pMsg->FromPos, &FromItem );
	}
	if(ToItem)
	{
		ToItem->SetPosition( pMsg->FromPos );		
		if( FromTableIdx == eItemTable_Shop )		
			pShop->AddItem(ToItem);	
		else if( FromTableIdx == eItemTable_ShopInven )		
			pInven->AddItem(ToItem);
		if(ToItem->GetQuickPosition())
			QUICKMGR->RemQuickItem(ToItem->GetQuickPosition());
	}
	ReLinkQuickPosition(FromItem);
	if(FromItem->GetQuickPosition())
		QUICKMGR->RemQuickItem(FromItem->GetQuickPosition());
	ASSERT(FromItem);
	FromItem->SetPosition(pMsg->ToPos);
	FromItem->SetQuickPosition(0);
	pShop->AddItem(FromItem);
	QUICKMGR->RefreshQickItem();
}
void CItemManager::MoveItemToShopInven(MSG_ITEM_MOVE_ACK* pMsg)
{
	CInventoryExDialog * pInven = GAMEIN->GetInventoryDialog();
	CItemShopDialog* pShop = GAMEIN->GetItemShopDialog();	
	CPetInventoryDlg* pPetInven = GAMEIN->GetPetInventoryDialog();
	CTitanInventoryDlg* pTitanShopInven = GAMEIN->GetTitanInventoryDlg();	
	CItem * FromItem = NULL;
	CItem * ToItem = NULL;
	pInven->DeleteItem(pMsg->ToPos, &ToItem);
	WORD FromTableIdx = GetTableIdxForAbsPos(pMsg->FromPos);
	if(FromTableIdx == eItemTable_Shop)
	{
		pShop->DeleteItem( pMsg->FromPos, &FromItem );
	}	
	else if(FromTableIdx == eItemTable_ShopInven)
	{
		pInven->DeleteItem( pMsg->FromPos, &FromItem );
	}
	else if(FromTableIdx == eItemTable_Weared )
	{
		pInven->DeleteItem( pMsg->FromPos, &FromItem );
	}
	else if(FromTableIdx == eItemTable_PetWeared)
	{
		pPetInven->DeleteItem( pMsg->FromPos, &FromItem );
	}
	else if(FromTableIdx == eItemTable_TitanShopItem)
	{
		pTitanShopInven->DeleteItem( pMsg->FromPos, &FromItem );
	}
	if(ToItem)
	{
		ToItem->SetPosition( pMsg->FromPos );
		if(FromTableIdx == eItemTable_Shop)		
			pShop->AddItem(ToItem);
		else if(FromTableIdx == eItemTable_ShopInven)		
			pInven->AddItem(ToItem);
		if(ToItem->GetQuickPosition())
			QUICKMGR->RemQuickItem(ToItem->GetQuickPosition());
	}
	ReLinkQuickPosition(FromItem);
	if(FromItem->GetQuickPosition())//kiv 1217 27042022
		QUICKMGR->RemQuickItem(FromItem->GetQuickPosition());//kiv 1217 27042022
	ASSERT(FromItem);
	FromItem->SetPosition(pMsg->ToPos);
	FromItem->SetQuickPosition(0);
	pInven->AddItem(FromItem);	

	//QUICKMGR->SetQuickItemReal(FromItem->GetQuickPosition(),pMsg->FromPos,pMsg->wToItemIdx);
	QUICKMGR->RefreshQickItem();
}
void CItemManager::MoveItemToPetInven(MSG_ITEM_MOVE_ACK* pMsg)
{
	CInventoryExDialog* pInven = GAMEIN->GetInventoryDialog();
	CPetInventoryDlg*	pPetInven = GAMEIN->GetPetInventoryDialog();
	CItem* FromItem	= NULL;
	CItem* ToItem = NULL;
	pPetInven->DeleteItem(pMsg->ToPos, &ToItem);
	WORD FromTableIdx = GetTableIdxForAbsPos(pMsg->FromPos);
	if(FromTableIdx == eItemTable_Inventory || FromTableIdx == eItemTable_Weared || FromTableIdx == eItemTable_ShopInven )
	{
		pInven->DeleteItem( pMsg->FromPos, &FromItem );
	}
	else if(FromTableIdx == eItemTable_PetInven || FromTableIdx == eItemTable_PetWeared)
	{
		pPetInven->DeleteItem( pMsg->FromPos, &FromItem );
	}
	else
	{
		ASSERT(0);
		return;
	}
	if(ToItem)
	{
		ToItem->SetPosition( pMsg->FromPos );
		if(FromTableIdx == eItemTable_Inventory || FromTableIdx == eItemTable_Weared || FromTableIdx == eItemTable_ShopInven)
		{
			pInven->AddItem(ToItem);
		}
		else if(FromTableIdx == eItemTable_PetInven || FromTableIdx == eItemTable_PetWeared)
		{
			pPetInven->AddItem(ToItem);
		}
		if(ToItem->GetQuickPosition())
		{
			ASSERT(0);	
			QUICKMGR->RemQuickItem(ToItem->GetQuickPosition());
			ToItem->SetQuickPosition(0);
		}
	}
	ASSERT(FromItem);
	if(FromItem->GetQuickPosition())
	{
		QUICKMGR->RemQuickItem(FromItem->GetQuickPosition());
	}
	FromItem->SetPosition(pMsg->ToPos);
	FromItem->SetQuickPosition(0);
	pPetInven->AddItem(FromItem);
	QUICKMGR->RefreshQickItem();
}
void CItemManager::MoveItemToTitanInven(MSG_ITEM_MOVE_ACK* pMsg)
{
	CInventoryExDialog* pInven = GAMEIN->GetInventoryDialog();	
	CTitanInventoryDlg*	pTitanInven = GAMEIN->GetTitanInventoryDlg();
	CItem* FromItem	= NULL;
	CItem* ToItem = NULL;
	pTitanInven->DeleteItem(pMsg->ToPos, &ToItem);
	WORD FromTableIdx = GetTableIdxForAbsPos(pMsg->FromPos);
	if( FromTableIdx == eItemTable_Inventory || FromTableIdx == eItemTable_Weared || FromTableIdx == eItemTable_ShopInven )
	{
		pInven->DeleteItem( pMsg->FromPos, &FromItem );
	}
	else if( FromTableIdx == eItemTable_Titan )
	{
		pTitanInven->DeleteItem( pMsg->FromPos, &FromItem );
	}
	else
	{
		ASSERT(0);
		return;
	}
	if(ToItem)
	{
		TITANMGR->RemoveTitanUsingEquipItemList(ToItem->GetDBIdx());
		ToItem->SetPosition( pMsg->FromPos );
		if(FromTableIdx == eItemTable_Inventory || FromTableIdx == eItemTable_Weared || FromTableIdx == eItemTable_ShopInven)
		{
			pInven->AddItem(ToItem);
		}
		if(ToItem->GetQuickPosition())
		{
			ASSERT(0);
			QUICKMGR->RemQuickItem(ToItem->GetQuickPosition());
			ToItem->SetQuickPosition(0);
		}
	}
	ASSERT(FromItem);
	if(FromItem->GetQuickPosition())
	{
		QUICKMGR->RemQuickItem(FromItem->GetQuickPosition());
	}
	FromItem->SetPosition(pMsg->ToPos);
	FromItem->SetQuickPosition(0);
	pTitanInven->AddItem(FromItem);
	HERO->GetTitanAppearInfo()->WearedItemIdx[pMsg->ToPos-TP_TITANWEAR_START] = pMsg->wFromItemIdx;
	APPEARANCEMGR->InitAppearance( HERO );	
	GAMEIN->GetTitanGuageDlg()->SetLife(TITANMGR->GetCurRidingTitan()->GetTitanTotalInfo()->Fuel); 
	TITAN_ENDURANCE_ITEMINFO* pInfo = TITANMGR->GetTitanEnduranceInfo(FromItem->GetDBIdx());
	if(pInfo)
		TITANMGR->AddTitanUsingEquipItemList(pInfo);
	else
	{
		ASSERT(0);
	}
	QUICKMGR->RefreshQickItem();
}
void CItemManager::MoveItemToTitanShopInven(MSG_ITEM_MOVE_ACK* pMsg)
{
	CInventoryExDialog * pInven = GAMEIN->GetInventoryDialog();
	CTitanInventoryDlg* pTitanInven = GAMEIN->GetTitanInventoryDlg();
	CItem * FromItem = NULL;
	CItem * ToItem = NULL;
	pTitanInven->DeleteItem(pMsg->ToPos, &ToItem);
	WORD FromTableIdx = GetTableIdxForAbsPos(pMsg->FromPos);
	if( FromTableIdx == eItemTable_TitanShopItem )
	{
		pTitanInven->DeleteItem( pMsg->FromPos, &FromItem );
	}
	else if( FromTableIdx == eItemTable_ShopInven )
	{
		pInven->DeleteItem( pMsg->FromPos, &FromItem );
	}
	if(ToItem)
	{
		ToItem->SetPosition( pMsg->FromPos );		
		if( FromTableIdx == eItemTable_Shop )		
			pTitanInven->AddItem(ToItem);	
		else if( FromTableIdx == eItemTable_ShopInven )		
			pInven->AddItem(ToItem);
		else if( FromTableIdx == eItemTable_TitanShopItem )
			pTitanInven->AddItem(ToItem);
		if(ToItem->GetQuickPosition())
			QUICKMGR->RemQuickItem(ToItem->GetQuickPosition());
	}
	ReLinkQuickPosition(FromItem);
	if(FromItem->GetQuickPosition())
		QUICKMGR->RemQuickItem(FromItem->GetQuickPosition());
	ASSERT(FromItem);
	FromItem->SetPosition(pMsg->ToPos);
	FromItem->SetQuickPosition(0);
	pTitanInven->AddItem(FromItem);
	QUICKMGR->RefreshQickItem();
}
void CItemManager::DeleteItem( POSTYPE absPos, CItem** ppItemOut, ITEM_OPTION_INFO * pItemOptionInfoOut, ITEM_RARE_OPTION_INFO* pItemRareOptionInfoOut, WORD RemainOptionFlag )
{
	BYTE TableIdx = GetTableIdxForAbsPos(absPos);
	if(pItemOptionInfoOut)
	{
		memset(pItemOptionInfoOut, 0, sizeof(ITEM_OPTION_INFO));
	}
	if(pItemRareOptionInfoOut)
	{
		memset(pItemRareOptionInfoOut, 0, sizeof(ITEM_RARE_OPTION_INFO));
	}
	switch(TableIdx)
	{
	case eItemTable_Inventory:
	case eItemTable_Weared:
	case eItemTable_ShopInven:
		{
			CInventoryExDialog * pInven = GAMEIN->GetInventoryDialog();
			pInven->DeleteItem( absPos, ppItemOut );
		}break;
	case eItemTable_Pyoguk:
		{
			CPyogukDialog* pPyoguk = GAMEIN->GetPyogukDialog();
			pPyoguk->DeleteItem( absPos, ppItemOut );
		}break;
	case eItemTable_GuildWarehouse:
		{
			CGuildWarehouseDialog* pGuild = GAMEIN->GetGuildWarehouseDlg();
			pGuild->DeleteItem( absPos, ppItemOut );
		}break;
	case eItemTable_PetInven:
		{
			CPetInventoryDlg* pPetInven = GAMEIN->GetPetInventoryDialog();
			pPetInven->DeleteItem( absPos, ppItemOut );
		}break;
	}
	if((*ppItemOut))
	{
		if(IsOptionItem((*ppItemOut)->GetItemIdx(), (*ppItemOut)->GetDurability()) && !(RemainOptionFlag & eRemainReinforceOption))
		{
			RemoveItemOption((*ppItemOut)->GetDurability(), pItemOptionInfoOut);
		}
		if(IsRareOptionItem((*ppItemOut)->GetItemIdx(), (*ppItemOut)->GetRareness()) && !(RemainOptionFlag & eRemainRareOption))
		{
			RemoveItemRareOption((*ppItemOut)->GetRareness(), pItemRareOptionInfoOut);
		}
		if( IsTitanCallItem((*ppItemOut)->GetItemIdx()) )
		{
			TITANMGR->RemoveTitanInfo((*ppItemOut)->GetDBIdx());
		}
		if( IsTitanEquipItem((*ppItemOut)->GetItemIdx()) )
		{
			TITANMGR->RemoveTitanEquipInfo((*ppItemOut)->GetDBIdx());
		}
		else
		{
		}
		if((*ppItemOut)->GetQuickPosition())
		{
			QUICKMGR->RemQuickItem((*ppItemOut)->GetQuickPosition());
		}
		ItemDelete( (*ppItemOut) );
	}
}
void CItemManager::DeleteItemofTable(WORD TableIdx, POSTYPE absPos, ITEM_OPTION_INFO * pItemOptionInfoOut, ITEM_RARE_OPTION_INFO* pItemRareOptionInfoOut )
{
	CItem * pItemOut = NULL;
	if(pItemOptionInfoOut)
	{
		memset(pItemOptionInfoOut, 0, sizeof(ITEM_OPTION_INFO));
	}
	if(pItemRareOptionInfoOut)
	{
		memset(pItemRareOptionInfoOut, 0, sizeof(ITEM_RARE_OPTION_INFO));
	}
	switch(TableIdx)
	{
	case eItemTable_Inventory:
	case eItemTable_Weared:
	case eItemTable_ShopInven:
		{
			CInventoryExDialog * pDlg = GAMEIN->GetInventoryDialog();
			if( !pDlg->DeleteItem( absPos, &pItemOut ) )
			{
				ASSERTMSG(0, "Item Delete Failed : DeleteItem return FALSE");
				return;
			}
			if(pItemOut)
			{
				if( pItemOut->GetItemInfo()->ItemType == 11 )
				{
					if( !(pItemOut->GetItemParam() & ITEM_PARAM_SEAL) )
						RemoveUsedItemInfo( pItemOut->GetItemIdx(), pItemOut->GetItemBaseInfo()->dwDBIdx );
					if( pItemOut->GetItemInfo()->ItemIdx == eIncantation_MemoryMoveExtend || 
						pItemOut->GetItemInfo()->ItemIdx == eIncantation_MemoryMoveExtend7 ||
						pItemOut->GetItemInfo()->ItemIdx == eIncantation_MemoryMove2 ||
						pItemOut->GetItemInfo()->ItemIdx == eIncantation_MemoryMoveExtend30 )
						HERO->CalcShopItemOption( eIncantation_MemoryMoveExtend, FALSE );
				}
			}
		}
		break;
	case eItemTable_Pyoguk:
		{
			CPyogukDialog * pDlg = GAMEIN->GetPyogukDialog();
			if( !pDlg->DeleteItem( absPos, &pItemOut ) )
			{
				ASSERTMSG(0, "Item Delete Failed : DeleteItem return FALSE");
				return;
			}
		}
		break;
	case eItemTable_GuildWarehouse:
		{
			CGuildWarehouseDialog * pDlg = GAMEIN->GetGuildWarehouseDlg();
			if( !pDlg->DeleteItem( absPos, &pItemOut ) )
			{
				ASSERTMSG(0, "Item Delete Failed : DeleteItem return FALSE");
				return;
			}			
		}
		break;
	case eItemTable_Shop:
		{
			CItemShopDialog* pDlg = GAMEIN->GetItemShopDialog();
			if( !pDlg->DeleteItem( absPos, &pItemOut ) )
			{
				ASSERTMSG(0, "Item Delete Failed : DeleteItem return FALSE");
				return;
			}
			if(pItemOut)
			{
				if( pItemOut->GetItemInfo()->ItemType == 11 )
				{
					RemoveUsedItemInfo( pItemOut->GetItemInfo()->ItemIdx, pItemOut->GetItemBaseInfo()->dwDBIdx );
				}
			}
		}
		break;
	case eItemTable_PetWeared:
	case eItemTable_PetInven:
		{
			CPetInventoryDlg* pDlg = GAMEIN->GetPetInventoryDialog();
			if( !pDlg->DeleteItem( absPos, &pItemOut ) )
			{
				//ASSERTMSG(0, "Item Delete Failed : Pet Inven DeleteItem return FALSE");
				return;
			}
		}
		break;
	case eItemTable_Titan:	
	case eItemTable_TitanShopItem:	
		{			
			CTitanInventoryDlg* pDlg = GAMEIN->GetTitanInventoryDlg();
			if( !pDlg->DeleteItem( absPos, &pItemOut ) )
			{
				//ASSERTMSG(0, "Item Delete Failed : Titan Inven DeleteItem return FALSE");
				return;
			}
			if( HERO->IsTitanPreView() )
			{
				HERO->GetTitanPreViewInfo()->WearedItemIdx[absPos-TP_TITANWEAR_START] = 0;
				APPEARANCEMGR->InitAppearance( HERO );
			}
		}
		break;
	}
	if(pItemOut)
	{
		if(IsOptionItem(pItemOut->GetItemIdx(), pItemOut->GetDurability()))
		{
			RemoveItemOption(pItemOut->GetDurability(), pItemOptionInfoOut);
		}
		if(IsRareOptionItem(pItemOut->GetItemIdx(), pItemOut->GetRareness()))
		{
			RemoveItemRareOption(pItemOut->GetRareness(), pItemRareOptionInfoOut);
		}
		if(IsPetSummonItem(pItemOut->GetItemIdx()))
		{
			PETMGR->RemovePetInfo(pItemOut->GetDBIdx());
		}
		else
		{
			ReLinkQuickPosition(pItemOut);
		}
		if(pItemOut->GetItemKind() == eTITAN_ITEM_PAPER)
		{
			TITANMGR->RemoveTitanInfo(pItemOut->GetDBIdx());
			if(pItemOut->GetDBIdx() != TITANMGR->GetRegistedTitanItemDBIdx())
			{
				TITANMGR->SetRegistedTitanItemDBIdx(0);
			}
		}
		if(pItemOut->GetQuickPosition())
		{
			QUICKMGR->RemQuickItem(pItemOut->GetQuickPosition());
		}
		ItemDelete( pItemOut );
		if(pItemOut->GetItemKind() & eTITAN_EQUIPITEM)
		{
			if( TableIdx == eItemTable_Titan )
			{
				TITANMGR->RemoveTitanUsingEquipItemList(pItemOut->GetDBIdx());
			}
			TITANMGR->RemoveTitanEquipInfo(pItemOut->GetDBIdx());
		}		
	}
	if( (pItemOut != NULL) && (pItemOut->GetItemInfo()->wSetItemKind != 0) )
	{
		RefreshAllItem();		
	}
}
void CItemManager::InitItemRareOption(ITEM_RARE_OPTION_INFO* pInfo, WORD num)
{
	for(int i = 0; i < num; ++ i)
	{
		AddItemRareOption(&pInfo[i]);
	}
}
void CItemManager::AddItemRareOption(ITEM_RARE_OPTION_INFO* pInfo)
{
	if( m_ItemRareOptionList.GetData(pInfo->dwRareOptionIdx) )
	{
		return;
	}
	ITEM_RARE_OPTION_INFO* pNewInfo = new ITEM_RARE_OPTION_INFO;
	memcpy(pNewInfo, pInfo, sizeof(ITEM_RARE_OPTION_INFO));
	m_ItemRareOptionList.Add(pNewInfo, pNewInfo->dwRareOptionIdx);
}
ITEM_RARE_OPTION_INFO* CItemManager::GetItemRareOption(DWORD dwRareOptionIdx)
{
	ITEM_RARE_OPTION_INFO* pInfo = m_ItemRareOptionList.GetData(dwRareOptionIdx);
	if(pInfo)
		return pInfo;
	return NULL;
}
void CItemManager::RemoveItemRareOption(DWORD dwRareOptionIdx, ITEM_RARE_OPTION_INFO* pItemRareOptionInfoOut)
{
	ITEM_RARE_OPTION_INFO* pNewInfo = m_ItemRareOptionList.GetData(dwRareOptionIdx);
	if( pNewInfo == NULL)
	{
		//ASSERT(0);
		return;
	}
	if(pItemRareOptionInfoOut)
		*pItemRareOptionInfoOut = *pNewInfo;
	SAFE_DELETE( pNewInfo );
	m_ItemRareOptionList.Remove(dwRareOptionIdx);
}
void CItemManager::RemoveItemOption(DWORD dwOptionIdx, ITEM_OPTION_INFO * pItemOptionInfoOut)
{
	ITEM_OPTION_INFO * pNewInfo = m_ItemOptionList.GetData(dwOptionIdx);
	if( pNewInfo == NULL )
	{
		//ASSERT(0);		
		return;
	}
	if(pItemOptionInfoOut)
		*pItemOptionInfoOut = *pNewInfo;
	SAFE_DELETE( pNewInfo );
	m_ItemOptionList.Remove(dwOptionIdx);
}
void CItemManager::AddItemOption(ITEM_OPTION_INFO * pInfo)
{
	if( m_ItemOptionList.GetData(pInfo->dwOptionIdx) )
	{
		return;
	}
	ITEM_OPTION_INFO * pNewInfo = new ITEM_OPTION_INFO;
	memcpy(pNewInfo, pInfo, sizeof(ITEM_OPTION_INFO));
	m_ItemOptionList.Add(pNewInfo, pNewInfo->dwOptionIdx);
}
ITEM_OPTION_INFO * CItemManager::GetItemOption(DWORD dwOptionIdx)
{
	ITEM_OPTION_INFO* pInfo = m_ItemOptionList.GetData(dwOptionIdx);
	if(pInfo)
		return pInfo;
	static ITEM_OPTION_INFO NULLINFO;
	memset(&NULLINFO,0,sizeof(ITEM_OPTION_INFO));
	return &NULLINFO;
}
void CItemManager::InitItemOption(ITEM_OPTION_INFO * pInfo, WORD num)
{
	for(int i = 0 ; i < num ; ++i)
	{
		AddItemOption(&pInfo[i]);
	}
}
void CItemManager::NetworkMsgParse(BYTE Protocol,void* pMsg)
{
	switch(Protocol)
	{
	case MP_ITEM_TOTALINFO_LOCAL:
		{
			ITEM_TOTALINFO * msg = (ITEM_TOTALINFO *)pMsg;
			for(int i = 0 ; i < SLOT_INVENTORY_NUM ; i++)
			{
				GAMEIN->GetInventoryDialog()->AddItem(&msg->Inventory[i]);	
				if(msg->Inventory[i].QuickPosition != 0)
				{
					if( EVENTMAP->IsEventMap() )
					{
						ITEM_INFO* pItemInfo = GetItemInfo(msg->Inventory[i].wIconIdx);
						if( pItemInfo )
							if( pItemInfo->ItemKind != eYOUNGYAK_ITEM )
								QUICKMGR->SetQuickItemReal(msg->Inventory[i].QuickPosition,msg->Inventory[i].Position, msg->Inventory[i].wIconIdx);
					}
					else
					{
						QUICKMGR->SetQuickItemReal(msg->Inventory[i].QuickPosition,msg->Inventory[i].Position, msg->Inventory[i].wIconIdx);
					}
				}
			}
			for(int i = 0 ; i < (SLOT_SHOPINVEN_NUM + TABCELL_SHOPINVEN_PLUS_NUM)/*/2*/ ; i++)
			{
				GAMEIN->GetInventoryDialog()->AddItem(&msg->ShopInventory[i]);
				if(msg->ShopInventory[i].QuickPosition != 0)
				{
					QUICKMGR->SetQuickItemReal(msg->ShopInventory[i].QuickPosition,msg->ShopInventory[i].Position, msg->ShopInventory[i].wIconIdx);
				}
			}
			for(int i = 0 ; i < eWearedItem_Max ; i++)
			{
				GAMEIN->GetInventoryDialog()->AddItem(&msg->WearedItem[i]);
				if(msg->WearedItem[i].QuickPosition != 0)
				{
					QUICKMGR->SetQuickItemReal(msg->WearedItem[i].QuickPosition,msg->WearedItem[i].Position,msg->WearedItem[i].wIconIdx);
				}
			}
			for(int i = 0 ; i < SLOT_PETWEAR_NUM ; i++)
			{
				GAMEIN->GetPetInventoryDialog()->AddItem(&msg->PetWearedItem[i]);
				if(msg->PetWearedItem[i].QuickPosition != 0)
				{
					QUICKMGR->SetQuickItemReal(msg->PetWearedItem[i].QuickPosition,msg->PetWearedItem[i].Position,msg->PetWearedItem[i].wIconIdx);
				}
			}
			for(int i = 0 ; i < SLOT_TITANWEAR_NUM ; i++)
			{				
				GAMEIN->GetTitanInventoryDlg()->AddItem(&msg->TitanWearedItem[i]);
				if(msg->TitanWearedItem[i].QuickPosition != 0)
				{
					QUICKMGR->SetQuickItemReal(msg->TitanWearedItem[i].QuickPosition,msg->TitanWearedItem[i].Position,msg->TitanWearedItem[i].wIconIdx);
				}
				DWORD itemDBIdx = msg->TitanWearedItem[i].dwDBIdx;
				if(itemDBIdx)
				{
					TITAN_ENDURANCE_ITEMINFO* pInfo = TITANMGR->GetTitanEnduranceInfo(itemDBIdx);
					if(pInfo)
					{
						TITANMGR->AddTitanUsingEquipItemList(pInfo);
					}
				}
			}
			for(int i = 0 ; i < SLOT_TITANSHOPITEM_NUM ; i++)
			{
				GAMEIN->GetTitanInventoryDlg()->AddItem(&msg->TitanShopItem[i]);
				if(msg->TitanShopItem[i].QuickPosition != 0)
				{
					QUICKMGR->SetQuickItemReal(msg->TitanShopItem[i].QuickPosition,msg->TitanShopItem[i].Position,msg->TitanShopItem[i].wIconIdx);
				}
			}
			QUICKMGR->RefreshQickItem();
		}
		break;
	case MP_ITEM_PYOGUK_ITEM_INFO_ACK:
		{
			SEND_PYOGUK_ITEM_INFO * pmsg = (SEND_PYOGUK_ITEM_INFO*)pMsg;
			CAddableInfoIterator iter(&pmsg->AddableInfo);
			BYTE AddInfoKind;
			while((AddInfoKind = iter.GetInfoKind()) != CAddableInfoList::None)
			{
				switch(AddInfoKind)
				{
				case(CAddableInfoList::ItemOption):
					{
						ITEM_OPTION_INFO OptionInfo[SLOT_PYOGUK_NUM];
						iter.GetInfoData(&OptionInfo);
						ITEMMGR->InitItemOption(OptionInfo, pmsg->wOptionCount);
					}
					break;
				case(CAddableInfoList::ItemRareOption):
					{
						ITEM_RARE_OPTION_INFO RareOptionInfo[SLOT_PYOGUK_NUM];
						iter.GetInfoData(&RareOptionInfo);
						ITEMMGR->InitItemRareOption(RareOptionInfo, pmsg->wRareOptionCount);
					}
					break;
				case(CAddableInfoList::ItemStoneOption):  
					{
						ITEM_STONE_OPTION_INFO StoneOptionInfo[SLOT_PYOGUK_NUM];
						iter.GetInfoData(&StoneOptionInfo);
						ITEMMGR->InitItemStoneOption(StoneOptionInfo, pmsg->wStoneOptionCount);
					}
					break;
				case(CAddableInfoList::PetTotalInfo):
					{
						PET_TOTALINFO PetInfo[SLOT_PYOGUK_NUM];
						iter.GetInfoData(&PetInfo);
						PETMGR->InitPetInfo(PetInfo, pmsg->wPetInfoCount);
					}
					break;
				case(CAddableInfoList::TitanTotalInfo):
					{
						TITAN_TOTALINFO TitanInfo[SLOT_PYOGUK_NUM];
						iter.GetInfoData(&TitanInfo);
						TITANMGR->InitTitanInfo(TitanInfo, pmsg->wTitanItemCount);
					}
					break;
				case(CAddableInfoList::TitanEndrncInfo):
					{
						TITAN_ENDURANCE_ITEMINFO TItanEquipInfo[SLOT_PYOGUK_NUM];
						iter.GetInfoData(&TItanEquipInfo);
						TITANMGR->InitTitanEquipItemEnduranceInfo(TItanEquipInfo, pmsg->wTitanEquipItemCount);
					}
					break;
				}
				iter.ShiftToNextData();
			}
			for(int i=0; i<SLOT_PYOGUK_NUM; ++i)
			{
				if(pmsg->PyogukItem[i].dwDBIdx != 0)
					GAMEIN->GetPyogukDialog()->AddItem(&pmsg->PyogukItem[i]);
			}
			GAMEIN->GetPyogukDialog()->SetPyogukMoney(pmsg->money);		
			GAMEIN->GetPyogukDialog()->SetItemInit(TRUE);
			GAMEIN->GetInventoryDialog()->SetActive(TRUE);
			GAMEIN->GetPyogukDialog()->SetActive(TRUE);
			if(HERO->GetPyogukNum() == 0)
			{	
			}
			else
			{
				GAMEIN->GetPyogukDialog()->ShowPyogukMode(ePyogukMode_PyogukWare1);
				GAMEIN->GetPyogukDialog()->AddPyogukMode(ePyogukMode_PyogukWare1);
			}	
		}
		break;
	case MP_ITEM_PETINVEN_INFO_ACK:
		{
			SEND_PETINVEN_ITEM_INFO* pmsg = (SEND_PETINVEN_ITEM_INFO*)pMsg;
			CAddableInfoIterator iter(&pmsg->AddableInfo);
			BYTE AddInfoKind;
			while((AddInfoKind = iter.GetInfoKind()) != CAddableInfoList::None)
			{
				switch(AddInfoKind)
				{
				case(CAddableInfoList::ItemOption):
					{
						ITEM_OPTION_INFO OptionInfo[SLOT_PETINVEN_NUM];
						iter.GetInfoData(&OptionInfo);
						ITEMMGR->InitItemOption(OptionInfo, pmsg->wOptionCount);
					}
					break;
				case(CAddableInfoList::ItemRareOption):
					{
						ITEM_RARE_OPTION_INFO RareOptionInfo[SLOT_PETINVEN_NUM];
						iter.GetInfoData(&RareOptionInfo);
						ITEMMGR->InitItemRareOption(RareOptionInfo, pmsg->wRareOptionCount);
					}
					break;
				}
				iter.ShiftToNextData();
			}
			for( int i = 0; i < SLOT_PETINVEN_NUM; ++i )
			{
				if(pmsg->PetInvenItem[i].dwDBIdx != 0)
				{
					GAMEIN->GetPetInventoryDialog()->AddItem(&pmsg->PetInvenItem[i]);
				}
			}
			GAMEIN->GetPetInventoryDialog()->SetItemInfoInit(TRUE);
			PETMGR->SetPetValidInvenTab();
			GAMEIN->GetPetInventoryDialog()->SetActive(TRUE);
		}
		break;
	case MP_ITEM_MONSTER_RAREOBTAIN_NOTIFY:
		{
			MSG_ITEM_RAREITEM_GET* pmsg = (MSG_ITEM_RAREITEM_GET*)pMsg;
			AddItemRareOption(&pmsg->RareInfo);
			CItem* pItem = NULL;
			if( pItem = GAMEIN->GetInventoryDialog()->GetItemForPos(pmsg->RareItemBase.Position) )
			{
				pItem->SetItemBaseInfo(&pmsg->RareItemBase);
			}
			else
			{
				pItem = MakeNewItem(&pmsg->RareItemBase, "MP_ITEM_MONSTER_RAREOBTAIN_NOTIFY" );
				BOOL rt = GAMEIN->GetInventoryDialog()->AddItem(pItem);
				ASSERT(rt);
			}
			GAMEEVENTMGR->AddEvent(eGameEvent_ObtainItem);
			cDialog* pDlg = WINDOWMGR->GetWindowForID( DBOX_DIVIDE_INV );
			if( pDlg )
			{
				((cDivideBox*)pDlg)->ExcuteDBFunc( 0 );	
			}
			if (pItem)
			{
				if (pItem->GetItemKind() & eEQUIP_ITEM || pItem->GetItemKind() == eEQUIP_ITEM_UNIQUE)
				{
					if (pItem->GetQuality() == 4)
					{
						CHATMGR->AddMsg(CTC_GETITEM, CHATMGR->GetChatMsg(2770), pItem->GetItemInfo()->ItemName);
					}
					else if (pItem->GetQuality() == 3)
					{
						CHATMGR->AddMsg(CTC_GETITEM, CHATMGR->GetChatMsg(2769), pItem->GetItemInfo()->ItemName);
					}
					else if (pItem->GetQuality() == 2)
					{
						CHATMGR->AddMsg(CTC_GETITEM, CHATMGR->GetChatMsg(2768), pItem->GetItemInfo()->ItemName);
					}
					else if (pItem->GetQuality() == 1)
					{
						CHATMGR->AddMsg(CTC_GETITEM, CHATMGR->GetChatMsg(2767), pItem->GetItemInfo()->ItemName);
					}
					else if (pItem->GetQuality() == 0)
					{
						CHATMGR->AddMsg(CTC_GETITEM, CHATMGR->GetChatMsg(2766), pItem->GetItemInfo()->ItemName);
					}
				}
				else
				{
					CHATMGR->AddMsg(CTC_GETITEM, CHATMGR->GetChatMsg(121), pItem->GetItemInfo()->ItemName);
				}
				//EFFECTMGR->StartHeroEffectProcess(eEffect_GetItem);
				ItemDropEffect(pItem->GetItemIdx());
			}



				if(ITEMMGR->IsClearPacketItemCheck(pItem->GetItemIdx()))
				{

					//WINDOWMGR->DestroyWindowProcess();
					//WINDOWMGR->AddListDestroyWindow( pItem );
					ITEMMGR->FakeDeleteItem(pItem);
					ITEMMGR->SendDeleteItem();
					CHATMGR->AddMsg( CTC_KILLED, CHATMGR->GetChatMsg( 2094 ), pItem->GetItemInfo()->ItemName );
					break;
				}

				QUICKMGR->RefreshQickItem();

		}
		break;
	case MP_ITEM_REINFORCE_SUCCESS_ACK:
		{
			MSG_ITEM_REINFORCE_ACK * pmsg = (MSG_ITEM_REINFORCE_ACK*)pMsg;
			AddItemOption(&pmsg->OptionInfo);
			CItem * ItemOut;
			for(int i = 1 ; i < MAX_REINFORCEGRID_NUM+1 ; ++i)
			{
				CVirtualItem * pVItem = GAMEIN->GetReinforceDialog()->GetVirtualItem(i);
				if(pVItem)
					DeleteItem(pVItem->GetSrcPosition(), &ItemOut);
			}
			REINFORCEMGR->SetRareMaterialTotalGravity(0);
			GAMEIN->GetReinforceDialog()->SetGravityText();
			CItem * pTItem = NULL;
			CVirtualItem * pVItem = GAMEIN->GetReinforceDialog()->GetVirtualItem(0);
			if( pVItem )
			{
				pTItem = GAMEIN->GetInventoryDialog()->GetItemForPos(pVItem->GetSrcPosition());
				if( pTItem )
				{
					pTItem->SetDurability(pmsg->OptionInfo.dwOptionIdx);
					SetToolTipIcon(pTItem, &pmsg->OptionInfo, ITEMMGR->GetItemRareOption(pTItem->GetRareness()),0,ITEMMGR->GetItemStoneOption(pTItem->GetStoneIdx()),0,0,0,0, pTItem->GetGrade());
					GAMEIN->GetReinforceDialog()->AddVirtualItemWrap(0, pTItem);
					pTItem->SetLock(TRUE);
				}
			}
			GAMEIN->GetReinforceDialog()->Release();
			CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(225) );
			QUICKMGR->RefreshQickItem();
			GAMEIN->GetReinforceDialog()->SetDisable(FALSE);
			if( GAMEIN->GetReinforceDialog()->IsShopItemUse() )
				GAMEIN->GetReinforceDialog()->SetActiveRecursive( FALSE );
			else if( pTItem )
			{
				GAMEIN->GetReinforceDialog()->AddVirtualItemWrap(0, pTItem);
				pTItem->SetLock(TRUE);
			}
		}
		break;
	case MP_ITEM_REINFORCE_FAILED_ACK:
		{
			MSG_ITEM_REINFORCE_SYN * pmsg = (MSG_ITEM_REINFORCE_SYN*)pMsg;
			REINFORCEMGR->SetRareMaterialTotalGravity(0);
			GAMEIN->GetReinforceDialog()->SetGravityText();
			CItem * ItemOut;
			for(int i = 1 ; i < MAX_REINFORCEGRID_NUM+1 ; ++i)
			{
				CVirtualItem * pVItem = GAMEIN->GetReinforceDialog()->GetVirtualItem(i);
				if(pVItem)
					DeleteItem(pVItem->GetSrcPosition(), &ItemOut);
			}
			GAMEIN->GetReinforceDialog()->Release(eReinforceViewRelease);
			CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(226) );
			QUICKMGR->RefreshQickItem();
			GAMEIN->GetReinforceDialog()->SetDisable(FALSE);
		}
		break;
	case MP_ITEM_REINFORCE_NACK:
		{
			//REINFORCEMGR->SetRareMaterialTotalGravity(0);
			//GAMEIN->GetReinforceDialog()->SetGravityText();
			//GAMEIN->GetReinforceDialog()->SetDisable(FALSE);
			MSG_ITEM_ERROR* pmsg = (MSG_ITEM_ERROR*)pMsg;
			REINFORCEMGR->SetRareMaterialTotalGravity(0);
			GAMEIN->GetReinforceDialog()->SetGravityText();

			GAMEIN->GetReinforceDialog()->SetDisable(FALSE);
			if (pmsg->ECode == 64)
				CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(2257));
			if (pmsg->ECode == 265)
				CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(728));
		}
		break;
	case MP_ITEM_REINFORCE_WITHSHOPITEM_NACK:
		{
			MSG_ITEM_ERROR* pmsg = (MSG_ITEM_ERROR*)pMsg;
			char buf[256];
			memset( buf, 0, sizeof(char)*256 );
			switch( pmsg->ECode )
			{
			case 3:				
				sprintf( buf, CHATMGR->GetChatMsg(1081) );
				break;
			case 4:				
				sprintf( buf, CHATMGR->GetChatMsg(1082) );
				break;
			case 10:			
				{					
					CItem * ItemOut;
					for(int i = 1 ; i < MAX_REINFORCEGRID_NUM+1 ; ++i)
					{
						CVirtualItem * pVItem = GAMEIN->GetReinforceDialog()->GetVirtualItem(i);
						if(pVItem)
							DeleteItem(pVItem->GetSrcPosition(), &ItemOut);
					}
					CVirtualItem * pVItem = GAMEIN->GetReinforceDialog()->GetVirtualItem(0);
					if( pVItem )
					{
						CItem * pTItem = GAMEIN->GetInventoryDialog()->GetItemForPos( pVItem->GetSrcPosition() );
						if( pTItem )
						{							
							GAMEIN->GetReinforceDialog()->AddVirtualItemWrap(0, pTItem);
							pTItem->SetLock(TRUE);
						}
					}
					GAMEIN->GetReinforceDialog()->Release();
					QUICKMGR->RefreshQickItem();
					GAMEIN->GetReinforceDialog()->SetDisable(FALSE);
					sprintf( buf, CHATMGR->GetChatMsg(1083) );
				}
				break;
			case 11:			
				sprintf( buf, CHATMGR->GetChatMsg(1084) );
				break;
			case 12:			
				sprintf( buf, CHATMGR->GetChatMsg(1085) );
				break;				
			case 13:			
				sprintf( buf, CHATMGR->GetChatMsg(1089) );
				break;
			case 14:			
				sprintf( buf, CHATMGR->GetChatMsg(1090) );
				break;
			case 64:
				sprintf(buf, CHATMGR->GetChatMsg(2257));
				break;
			case 256:
				sprintf(buf, CHATMGR->GetChatMsg(728));
				break;
			default:
				break;
			}
			CHATMGR->AddMsg( CTC_SYSMSG, buf );
			GAMEIN->GetReinforceDialog()->SetDisable( FALSE );
			GAMEIN->GetReinforceDialog()->SetActiveRecursive( FALSE );
		}
		break;
	case MP_ITEM_MOVE_ACK:		
		{
			SetDisableDialog(FALSE, eItemTable_Inventory);
			SetDisableDialog(FALSE, eItemTable_Pyoguk);
			SetDisableDialog(FALSE, eItemTable_GuildWarehouse);
			SetDisableDialog(FALSE, eItemTable_PetInven);	
			MoveItem( (MSG_ITEM_MOVE_ACK *)pMsg );			
			WINDOWMGR->DragWindowNull();
		}
		break;
	case MP_ITEM_COMBINE_ACK:	
		{
			CombineItem( (MSG_ITEM_COMBINE_ACK *)pMsg );	
			SetDisableDialog(FALSE, eItemTable_Inventory);
			SetDisableDialog(FALSE, eItemTable_Pyoguk);
			SetDisableDialog(FALSE, eItemTable_GuildWarehouse);
			SetDisableDialog(FALSE, eItemTable_PetInven);
			WINDOWMGR->BackDragWindow();
		}
		break;
	case MP_ITEM_DIVIDE_ACK:
		{
			SetDisableDialog(FALSE, eItemTable_Inventory);
			SetDisableDialog(FALSE, eItemTable_Pyoguk);
			SetDisableDialog(FALSE, eItemTable_GuildWarehouse);
			SetDisableDialog(FALSE, eItemTable_ShopInven);
			SetDisableDialog(FALSE, eItemTable_PetInven);
			ITEMOBTAINARRAY * pmsg = (ITEMOBTAINARRAY *)pMsg;
			CItem* pItem = NULL;
			CItem* pPreItem = NULL;
			for(int i = 0 ; i < pmsg->ItemNum ; ++i)
			{
				pPreItem = NULL;
				DeleteItem(pmsg->GetItem(i)->Position, &pPreItem);
				pItem = MakeNewItem(pmsg->GetItem(i),"MP_ITEM_DIVIDE_ACK");	
				if(!AddItem(pItem))
				{
					ASSERT(0);
				}
				if(pItem->GetQuickPosition() != 0)
				{
					WORD QuickPosList[4];
					CQuickManager::AnalyzeQuickPosition(pItem->GetQuickPosition(), QuickPosList);
					for(WORD SheetNum = 0; SheetNum < 4; SheetNum++)
					{
						if(QuickPosList[SheetNum])
							QUICKMGR->AddQuickItemReal(((SheetNum * 10) + (QuickPosList[SheetNum] - 1)),pItem->GetPosition(),pItem->GetItemIdx());
					}
				}
			}
			QUICKMGR->RefreshQickItem();
			WINDOWMGR->DragWindowNull();
		}
		break;
	case MP_ITEM_MONSTER_OBTAIN_NOTIFY:
		{
			ITEMOBTAINARRAY * pmsg = (ITEMOBTAINARRAY *)pMsg;
			CItem* pItem = NULL;
			for(int i = 0 ; i < pmsg->ItemNum ; ++i)
			{
				if( pItem = GAMEIN->GetInventoryDialog()->GetItemForPos( pmsg->GetItem(i)->Position ) )
				{
					pItem->SetItemBaseInfo(pmsg->GetItem(i));
				}
				else
				{
					pItem = MakeNewItem(pmsg->GetItem(i),"MP_ITEM_MONSTER_OBTAIN_NOTIFY");	
					BOOL rt = GAMEIN->GetInventoryDialog()->AddItem(pItem);
					if(!rt)
					{
						ASSERT(0);
					}
				}
				GAMEEVENTMGR->AddEvent(eGameEvent_ObtainItem);
			}
			cDialog* pDlg = WINDOWMGR->GetWindowForID( DBOX_DIVIDE_INV );
			if( pDlg )
			{
				((cDivideBox*)pDlg)->ExcuteDBFunc( 0 );	
			}
			if (pItem)
			{
				if (pItem->GetItemKind() & eEQUIP_ITEM || pItem->GetItemKind() == eEQUIP_ITEM_UNIQUE)
				{
					if (pItem->GetQuality() == 4)
					{
						CHATMGR->AddMsg(CTC_GETITEM, CHATMGR->GetChatMsg(2770), pItem->GetItemInfo()->ItemName);
					}
					else if (pItem->GetQuality() == 3)
					{
						CHATMGR->AddMsg(CTC_GETITEM, CHATMGR->GetChatMsg(2769), pItem->GetItemInfo()->ItemName);
					}
					else if (pItem->GetQuality() == 2)
					{
						CHATMGR->AddMsg(CTC_GETITEM, CHATMGR->GetChatMsg(2768), pItem->GetItemInfo()->ItemName);
					}
					else if (pItem->GetQuality() == 1)
					{
						CHATMGR->AddMsg(CTC_GETITEM, CHATMGR->GetChatMsg(2767), pItem->GetItemInfo()->ItemName);
					}
					else if (pItem->GetQuality() == 0)
					{
						CHATMGR->AddMsg(CTC_GETITEM, CHATMGR->GetChatMsg(2766), pItem->GetItemInfo()->ItemName);
					}
				}
				else
				{
					CHATMGR->AddMsg(CTC_GETITEM, CHATMGR->GetChatMsg(121), pItem->GetItemInfo()->ItemName);
				}
				//EFFECTMGR->StartHeroEffectProcess(eEffect_GetItem);
				ItemDropEffect(pItem->GetItemIdx());
			}

				if(ITEMMGR->IsClearPacketItemCheck(pItem->GetItemIdx()))
				{
					//WINDOWMGR->DestroyWindowProcess();
					ITEMMGR->FakeDeleteItem(pItem);
					ITEMMGR->SendDeleteItem();
					CHATMGR->AddMsg( CTC_KILLED, CHATMGR->GetChatMsg( 2094 ), pItem->GetItemInfo()->ItemName );
					break;
				}
				QUICKMGR->RefreshQickItem();
		}

		
		break;
	case MP_ITEM_MOVE_NACK:
		{
			MSG_WORD* pmsg = (MSG_WORD*)pMsg;
			switch(pmsg->wData)
			{
			case 7:
				break;
			default:
				ASSERT(pmsg->wData == 3);	
			}
			SetDisableDialog(FALSE, eItemTable_Inventory);
			SetDisableDialog(FALSE, eItemTable_Pyoguk);
			SetDisableDialog(FALSE, eItemTable_PetInven);
			SetDisableDialog(FALSE, eItemTable_GuildWarehouse);
			WINDOWMGR->BackDragWindow();
		}
		break;
	case MP_ITEM_DIVIDE_NACK:
		{
			MSG_WORD* pmsg = (MSG_WORD*)pMsg;
			SetDisableDialog(FALSE, eItemTable_Inventory);
			SetDisableDialog(FALSE, eItemTable_Pyoguk);
			SetDisableDialog(FALSE, eItemTable_GuildWarehouse);
			SetDisableDialog(FALSE, eItemTable_ShopInven);
			SetDisableDialog(FALSE, eItemTable_PetInven);
			WINDOWMGR->BackDragWindow();
		}
		break;
	case MP_ITEM_COMBINE_NACK:
		{
			MSG_WORD* pmsg = (MSG_WORD*)pMsg;
			SetDisableDialog(FALSE, eItemTable_Inventory);
			SetDisableDialog(FALSE, eItemTable_Pyoguk);
			SetDisableDialog(FALSE, eItemTable_GuildWarehouse);
			SetDisableDialog(FALSE, eItemTable_PetInven);
			ASSERT(pmsg->wData == 0);
			ASSERT(0);
			WINDOWMGR->BackDragWindow();
		}
		break;
	case MP_ITEM_DISCARD_NACK:
		{
			SetDisableDialog(FALSE, eItemTable_Inventory);
			SetDisableDialog(FALSE, eItemTable_Pyoguk);
			SetDisableDialog(FALSE, eItemTable_GuildWarehouse);
			SetDisableDialog(FALSE, eItemTable_PetInven);
			ASSERT(0);
			GAMEIN->GetInventoryDialog()->SetDisable( FALSE );
			MSG_ITEM_ERROR * pmsg = (MSG_ITEM_ERROR*)pMsg;
			if(pmsg->ECode != 4)
				WINDOWMGR->BackDragWindow();
			CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(799));
		}
		break;
	case MP_ITEM_DISCARD_ACK:
		{
			MSG_ITEM_DISCARD_ACK* pmsg = (MSG_ITEM_DISCARD_ACK*)pMsg;
			WORD wTableIdx = GetTableIdxForAbsPos(pmsg->TargetPos);
			DeleteItemofTable(wTableIdx, pmsg->TargetPos);
			SetDisableDialog(FALSE, eItemTable_Inventory);
			SetDisableDialog(FALSE, eItemTable_Pyoguk);
			SetDisableDialog(FALSE, eItemTable_GuildWarehouse);
			SetDisableDialog(FALSE, eItemTable_PetInven);
			QUICKMGR->RefreshQickItem();
		}
		break;
	case MP_ITEM_USE_NOTIFY:
		{
			MSG_DWORDBYTE* pmsg = (MSG_DWORDBYTE*)pMsg;
			CObject* pObject = OBJECTMGR->GetObject( pmsg->dwData );
			if( pObject == NULL ) break;
			if( pObject->GetObjectKind() != eObjectKind_Player ) break;
			CPlayer* pPlayer = (CPlayer*)pObject;
			switch( pmsg->bData )
			{
			case 1:	
				{
					if(pPlayer->InTitan())
					{
						OBJECTEFFECTDESC desc(FindEffectNum("t_ba_070.beff"));
						pPlayer->AddObjectEffect( LIFE_RECOVER_EFFECTID, &desc, 1 );
					}
					else
					{
						OBJECTEFFECTDESC desc(FindEffectNum("m_ba_070.beff"));
						pPlayer->AddObjectEffect( LIFE_RECOVER_EFFECTID, &desc, 1 );
					}
				}
				break;
			case 2:	
				{
					if(pPlayer->InTitan())
					{
						OBJECTEFFECTDESC desc(FindEffectNum("t_ba_071.beff"));
						pPlayer->AddObjectEffect( MANA_RECOVER_EFFECTID, &desc, 1 );
					}
					else
					{
						OBJECTEFFECTDESC desc(FindEffectNum("m_ba_071.beff"));
						pPlayer->AddObjectEffect( MANA_RECOVER_EFFECTID, &desc, 1 );
					}
				}
				break;
			case 3:	
				{
					if(pPlayer->InTitan())
					{
						OBJECTEFFECTDESC desc(FindEffectNum("t_ba_077.beff"));
						pPlayer->AddObjectEffect( SHIELD_RECOVER_EFFECTID, &desc, 1 );
					}
					else
					{
						OBJECTEFFECTDESC desc(FindEffectNum("m_ba_077.beff"));
						pPlayer->AddObjectEffect( SHIELD_RECOVER_EFFECTID, &desc, 1 );
					}
				}
				break;
			case 4:	
				{
					if(pPlayer->InTitan())
					{
						OBJECTEFFECTDESC desc(FindEffectNum("t_ba_078.beff"));
						pPlayer->AddObjectEffect( ALL_RECOVER_EFFECTID, &desc, 1 );
					}
					else
					{
						OBJECTEFFECTDESC desc(FindEffectNum("m_ba_078.beff"));
						pPlayer->AddObjectEffect( ALL_RECOVER_EFFECTID, &desc, 1 );
					}
				}
				break;
			}
		}
		break;
	case MP_ITEM_USE_ACK:
		{
			MSG_ITEM_USE_ACK* pmsg = (MSG_ITEM_USE_ACK*)pMsg;
			WORD wTableIdx = GetTableIdxForAbsPos(pmsg->TargetPos);
			m_nItemUseCount--;
			switch(wTableIdx)
			{
			case eItemTable_Inventory:
			case eItemTable_Weared:
				{
					CItem * item = GAMEIN->GetInventoryDialog()->GetItemForPos(pmsg->TargetPos);
					eITEM_KINDBIT bits = item->GetItemKind();
					if(bits & eYOUNGYAK_ITEM)
					{
						if(item->GetDurability() > 1)
						{
							GAMEIN->GetInventoryDialog()->UpdateItemDurabilityAdd(pmsg->TargetPos,-1);
							YOUNGYAK_ITEM_DELAY_TYPE* Tmp = NULL;
							Tmp = GAMERESRCMNGR->GetYoungYakDelay()->GetData(item->GetItemIdx());
							if (Tmp)
							{
								HERO->GetDelayGroup()->AddYoungYakPotionDelay(Tmp->DelayTime, Tmp->wtype);   //[设置药品回气时间][2017/11/27]
							}
						}
						else
						{
							CItem* pOutItem = NULL;
							DeleteItem( pmsg->TargetPos, &pOutItem);
							if( pOutItem )
								ReLinkQuickPosition( pOutItem );
						}
						if( item )
						{
							if( item->GetItemIdx() == 53032 || item->GetItemIdx() == 53095 ||
								item->GetItemIdx() == 53103 || item->GetItemIdx() == 53102 ||
								item->GetItemIdx() == 53217 || item->GetItemIdx() == 53223 ||
								item->GetItemIdx() == 53232 || item->GetItemIdx() == 53234 )
							{
								if( HERO->InTitan() == TRUE )
                                    EFFECTMGR->StartHeroEffectProcess(eEffect_Titan_UseItem_FullYoungYak);
								else
									EFFECTMGR->StartHeroEffectProcess(eEffect_UseItem_FullYoungYak);
							}
							else
							{
								if( !(item->GetItemKind() & eYOUNGYAK_ITEM) )	return;
								if( item->GetItemKind() == eYOUNGYAK_ITEM )	
								{
									if( item->GetItemInfo()->LifeRecover || item->GetItemInfo()->LifeRecoverRate )
									{
										if( HERO->InTitan() == TRUE )
											EFFECTMGR->StartHeroEffectProcess(eEffect_Titan_UseItem_LifeYoungYak);
										else
											EFFECTMGR->StartHeroEffectProcess(eEffect_UseItem_LifeYoungYak);
									}
									if( item->GetItemInfo()->NaeRyukRecover || item->GetItemInfo()->NaeRyukRecoverRate )
									{
										if( HERO->InTitan() == TRUE )
											EFFECTMGR->StartHeroEffectProcess(eEffect_Titan_UseItem_ManaYoungYak);
										else
											EFFECTMGR->StartHeroEffectProcess(eEffect_UseItem_ManaYoungYak);
									}								
									if( item->GetItemIdx() == 53031 || item->GetItemIdx() == 53094 || item->GetItemIdx() == 53109 )										
									{
										if( HERO->InTitan() == TRUE )
											EFFECTMGR->StartHeroEffectProcess(eEffect_Titan_UseItem_HosinYoungYak);
										else
											EFFECTMGR->StartHeroEffectProcess(eEffect_UseItem_HosinYoungYak);
									}
									if( item->GetItemIdx() == 53216 || item->GetItemIdx() == 53222 )
									{
										if( HERO->InTitan() == TRUE )
											EFFECTMGR->StartHeroEffectProcess(eEffect_Titan_UseItem_HosinYoungYak);
										else
											EFFECTMGR->StartHeroEffectProcess(eEffect_UseItem_HosinYoungYak);
									}
								}
								else if( item->GetItemKind() == eYOUNGYAK_ITEM_PET )
								{
									CPet* pPet = PETMGR->GetCurSummonPet();
									if(pPet)
									{
										TARGETSET set;
										set.pTarget = pPet;
										EFFECTMGR->StartEffectProcess(eEffect_FeedUp,pPet,&set,0,pPet->GetID());
									}
								}
								else if( item->GetItemKind() == eYOUNGYAK_ITEM_TITAN )
								{
#define TITAN_POTION_DELAY 30000
									DWORD dwTime = TITAN_POTION_DELAY;
									if(TITANMGR->GetTitanShopitemOption()->dwEPReduceRate > 0.0f)
									{
										dwTime = (DWORD)(dwTime * TITANMGR->GetTitanShopitemOption()->dwEPReduceRate);
									}
                                    HERO->GetDelayGroup()->AddTitanPotionDelay( dwTime );
								}
							}
						}
					}
					else if(bits & eCHANGE_ITEM)
					{
						if(item->GetDurability() > 1)
						{
							GAMEIN->GetInventoryDialog()->UpdateItemDurabilityAdd(pmsg->TargetPos,-1);
						}
						else
						{
							CItem* pOutItem = NULL;
							DeleteItem( pmsg->TargetPos, &pOutItem);
							if( pOutItem )
								ReLinkQuickPosition( pOutItem );
						}
					}
					else if(bits & eEXTRA_ITEM)
					{
						if(item->GetDurability() > 1)
						{
							GAMEIN->GetInventoryDialog()->UpdateItemDurabilityAdd(pmsg->TargetPos,-1);
						}
						else
						{
							CItem* pOutItem = NULL;
							DeleteItem( pmsg->TargetPos, &pOutItem);
							if( pOutItem )
								ReLinkQuickPosition( pOutItem );
						}
					}
					else if(bits & eMUGONG_ITEM)
					{
						CItem* pOutItem = NULL;
						DeleteItem( pmsg->TargetPos, &pOutItem);
						AUDIOMGR->Play(56, HERO);
					}
					else if( bits & eQUEST_ITEM )
					{
						if( bits == eQUEST_ITEM_START )
						{
							if( item->GetDurability() > 1 )
								GAMEIN->GetInventoryDialog()->UpdateItemDurabilityAdd(pmsg->TargetPos,-1);
							else
							{
								CItem* pOutItem = NULL;
								DeleteItem( pmsg->TargetPos, &pOutItem);
								if( pOutItem )
									ReLinkQuickPosition( pOutItem );
							}
						}
					}
					else if (bits == eQUEST_INSDUNGEON_ITEM_USE)
					{
						if (item->GetDurability() > 1)
						{
							//∑o A㈢
							GAMEIN->GetInventoryDialog()->UpdateItemDurabilityAdd(pmsg->TargetPos, -1);
						}
						else
						{
							CItem* pOutItem = NULL;
							DeleteItem(pmsg->TargetPos, &pOutItem);
							if (pOutItem)
								ReLinkQuickPosition(pOutItem);
						}
					}
					else if( bits & eTITAN_ITEM )
					{
					}
					else
					{
						//ASSERT(0);
					}
				}
				break;
			case eItemTable_StreetStall:
				{
					//ASSERT(0);
				}
				break;
			case eItemTable_ShopInven:
				{
					CItem * item = GAMEIN->GetInventoryDialog()->GetItemForPos(pmsg->TargetPos);
					eITEM_KINDBIT bits = item->GetItemKind();
					if( bits & eSHOP_ITEM )
					{
						if(item->GetDurability() > 1)
						{
							GAMEIN->GetInventoryDialog()->UpdateItemDurabilityAdd(pmsg->TargetPos,-1);
						}
						else
						{
							CItem* pOutItem = NULL;
							DeleteItem( pmsg->TargetPos, &pOutItem);
							if( pOutItem )
								ReLinkQuickPosition( pOutItem );
						}
						AUDIOMGR->Play(68, HERO);
					}
				}
				break;				
			default:
				{
					//ASSERT(0);
				}
			}
			QUICKMGR->RefreshQickItem();
		}
		break;
	case MP_ITEM_MIX_ADDITEM_ACK:
		{
			SetDisableDialog(FALSE, eItemTable_Inventory);
			SetDisableDialog(FALSE, eItemTable_Pyoguk);
			SetDisableDialog(FALSE, eItemTable_GuildWarehouse);
			SetDisableDialog(FALSE, eItemTable_PetInven);
			MSG_ITEM* pmsg = (MSG_ITEM*)pMsg;
			CItem* pItem = GAMEIN->GetInventoryDialog()->GetItemForPos(pmsg->ItemInfo.Position);			
			if(pItem)
				GAMEIN->GetMixDialog()->AddRealItem( pItem );
			GAMEIN->GetMixDialog()->DieCheck();
		}
		break;
	case MP_ITEM_MIX_ADDITEM_NACK:
		{
			SetDisableDialog(FALSE, eItemTable_Inventory);
			SetDisableDialog(FALSE, eItemTable_Pyoguk);
			SetDisableDialog(FALSE, eItemTable_GuildWarehouse);			
			SetDisableDialog(FALSE, eItemTable_PetInven);
			MSG_WORD2* pmsg = (MSG_WORD2*)pMsg;
			CItem* pItem = GAMEIN->GetInventoryDialog()->GetItemForPos(pmsg->wData1);
			if( pItem )
				pItem->SetLock( FALSE );
			GAMEIN->GetMixDialog()->Release( eMixAllRelease, FALSE );
			GAMEIN->GetMixDialog()->DieCheck();
		}
		break;
	case MP_ITEM_MIX_SUCCESS_ACK:
		{
			MSG_ITEM_MIX_ACK * pmsg = (MSG_ITEM_MIX_ACK *)pMsg;
			GAMEIN->GetMixDialog()->Release(eMixAllRelease, FALSE);
			CItem * BasicItemOut;
			DeleteItem(pmsg->BasicItemPos, &BasicItemOut, NULL, NULL, eRemainAllOption);
			CItem * pMatItemOut;
			for(int i = 0 ; i < pmsg->wMaterialNum ; ++i)
			{
				if(IsDupItem(pmsg->Material[i].wItemIdx))
				{
					CItem * pItem = GAMEIN->GetInventoryDialog()->GetItemForPos(pmsg->Material[i].ItemPos);
					if(pItem)
						if(pItem->GetDurability() - pmsg->Material[i].Dur != 0)
						{
							pItem->SetDurability(pItem->GetDurability() - pmsg->Material[i].Dur);
							continue;
						}
				}
				DeleteItem(pmsg->Material[i].ItemPos, &pMatItemOut);
			}
			ITEMBASE NewItem;
			NewItem = *(BasicItemOut->GetItemBaseInfo());
			NewItem.Position = pmsg->BasicItemPos;
			NewItem.wIconIdx = pmsg->wResultItemIdx;
			NewItem.RareIdx	= pmsg->dwRareIdx;
			NewItem.QuickPosition = 0;
			if( !IsDupItem( NewItem.wIconIdx ) )
				NewItem.Durability = 0;
			ITEM_INFO* piteminfo = ITEMMGR->GetItemInfo( NewItem.wIconIdx );
			GAMEIN->GetMixDialog()->Release(eMixBasicRelease, FALSE);
			if( piteminfo && !(piteminfo->ItemKind & eSHOP_ITEM) )
			{
				GAMEIN->GetInventoryDialog()->AddItem(&NewItem);
				CItem * pResultItem = GAMEIN->GetInventoryDialog()->GetItemForPos(pmsg->BasicItemPos);
				if (!(piteminfo->ItemKind&eCHANGE_ITEM))//kiv
				{
					GAMEIN->GetMixDialog()->AddVirtualItemWrap(eMixPos_Result, pResultItem);
					pResultItem->SetLock(TRUE);
				}
			}
			CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(207), piteminfo->ItemName );
			QUICKMGR->RefreshQickItem();
			//ITEMMGR->RefreshItemToolTip(NewItem.dwDBIdx);//kiv
			GAMEIN->GetMixDialog()->DieCheck();
			if( GAMEIN->GetMixDialog()->IsShopItemUse() )
			{
				GAMEIN->GetMixDialog()->Release();
				GAMEIN->GetMixDialog()->SetActiveRecursive( FALSE );
			}
		}
		break;
	case MP_ITEM_MIX_BIGFAILED_ACK:
		{
			MSG_ITEM_MIX_ACK * pmsg = (MSG_ITEM_MIX_ACK *)pMsg;
			GAMEIN->GetMixDialog()->Release();
			CItem * BasicItemOut;
			DeleteItem(pmsg->BasicItemPos, &BasicItemOut);
			CItem * pMatItemOut;
			for(int i = 0 ; i < pmsg->wMaterialNum ; ++i)
			{
				if(IsDupItem(pmsg->Material[i].wItemIdx))
				{
					CItem * pItem = GAMEIN->GetInventoryDialog()->GetItemForPos(pmsg->Material[i].ItemPos);
					if(pItem->GetDurability() - pmsg->Material[i].Dur != 0)
					{
						pItem->SetDurability(pItem->GetDurability() - pmsg->Material[i].Dur);
						continue;
					}
				}
				DeleteItem(pmsg->Material[i].ItemPos, &pMatItemOut);
			}
			GAMEIN->GetMixDialog()->Release(eMixBasicRelease);
			CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(208));
			QUICKMGR->RefreshQickItem();
			GAMEIN->GetMixDialog()->DieCheck();
			if( GAMEIN->GetMixDialog()->IsShopItemUse() )
				GAMEIN->GetMixDialog()->SetActiveRecursive( FALSE );
		}
		break;
	case MP_ITEM_MIX_FAILED_ACK:
		{
			GAMEIN->GetMixDialog()->ResetCurSelCellPos();
			MSG_ITEM_MIX_ACK * pmsg = (MSG_ITEM_MIX_ACK *)pMsg;
			CItem * pMatItemOut;
			for(int i = 0 ; i < pmsg->wMaterialNum ; ++i)
			{
				if(IsDupItem(pmsg->Material[i].wItemIdx))
				{
					CItem * pItem = GAMEIN->GetInventoryDialog()->GetItemForPos(pmsg->Material[i].ItemPos);
					if(pItem->GetDurability() - pmsg->Material[i].Dur != 0)
					{
						pItem->SetDurability(pItem->GetDurability() - pmsg->Material[i].Dur);
						continue;
					}
				}
				DeleteItem(pmsg->Material[i].ItemPos, &pMatItemOut);
			}
			CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(209));
			QUICKMGR->RefreshQickItem();
			GAMEIN->GetMixDialog()->DieCheck();
			if( GAMEIN->GetMixDialog()->IsShopItemUse() )
			{
				GAMEIN->GetMixDialog()->Release();
				GAMEIN->GetMixDialog()->SetActiveRecursive( FALSE );
			}
		}
		break;
	case MP_ITEM_MIX_MSG:
		{
			MSG_DWORD2* pmsg = (MSG_DWORD2*)pMsg;
			char buf[256];
			memset( buf, 0, sizeof(char)*256 );
			switch( pmsg->dwData1 )
			{
			case 20:			
				sprintf( buf, CHATMGR->GetChatMsg(1086) );
				break;
			case 21:			
				{
					CItem * BasicItemOut;
					DeleteItem( (POSTYPE)pmsg->dwData2, &BasicItemOut );
					sprintf( buf, CHATMGR->GetChatMsg(1088) );
				}
				break;
			case 22:			
				sprintf( buf, CHATMGR->GetChatMsg(1087) );
				break;
			case 23:			
				sprintf( buf, CHATMGR->GetChatMsg(1082) );
				break;
			case 256:
				sprintf(buf, CHATMGR->GetChatMsg(728));
				break;
			/*case 1000:
			case 1001:
				sprintf( buf, "check MP_ITEM_MIX" );
				break;
			default:
				sprintf( buf, "MP_ITEM_MIX" );
				break;*/
			}
			CHATMGR->AddMsg( CTC_SYSMSG, buf );
			GAMEIN->GetMixDialog()->Release();
			GAMEIN->GetMixDialog()->DieCheck();
			GAMEIN->GetMixDialog()->SetActiveRecursive( FALSE );
		}
		break;
	case MP_ITEM_MIX_SUCCESS_MSGTOCLIENT:
	{//FROM map 
		MSG_ITEM_QUALITY_MSG* pmsg = (MSG_ITEM_QUALITY_MSG*)pMsg;
		CItem* pItem = NULL;

		pItem = GAMEIN->GetInventoryDialog()->GetItemForPos(pmsg->ItemTargetPos);
		pItem->SetItemBaseInfo(&pmsg->QualityItemBase);//设置装备信息
		//装备组合成功在此提示，取消前面提示信息
		if (pItem->GetItemKind() & eEQUIP_ITEM)
		{
			if (pItem->GetQuality() == 4)
			{
				CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(2770), pItem->GetItemInfo()->ItemName);
			}
			else if (pItem->GetQuality() == 3)
			{
				CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(2769), pItem->GetItemInfo()->ItemName);
			}
			else if (pItem->GetQuality() == 2)
			{
				CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(2768), pItem->GetItemInfo()->ItemName);
			}
			else if (pItem->GetQuality() == 1)
			{
				CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(2767), pItem->GetItemInfo()->ItemName);
			}
			else if (pItem->GetQuality() == 0)
			{
				CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(2766), pItem->GetItemInfo()->ItemName);
			}
		}
		else
			CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(207), pItem->GetItemInfo()->ItemName);//物品组合成功提示

		ITEMMGR->RefreshItem(pItem);
	}
	break;
	case MP_ITEM_TPM_ADDITEM_ACK:
		{
			SetDisableDialog(FALSE, eItemTable_Inventory);
			SetDisableDialog(FALSE, eItemTable_Pyoguk);
			SetDisableDialog(FALSE, eItemTable_GuildWarehouse);
			SetDisableDialog(FALSE, eItemTable_PetInven);
			MSG_ITEM* pmsg = (MSG_ITEM*)pMsg;
			CItem* pItem = GAMEIN->GetInventoryDialog()->GetItemForPos(pmsg->ItemInfo.Position);			
			if(pItem)
				GAMEIN->GetTitanPartsMakeDlg()->AddRealItem( pItem );
			GAMEIN->GetTitanPartsMakeDlg()->DieCheck();
		}
		break;
	case MP_ITEM_TPM_ADDITEM_NACK:
		{
			SetDisableDialog(FALSE, eItemTable_Inventory);
			SetDisableDialog(FALSE, eItemTable_Pyoguk);
			SetDisableDialog(FALSE, eItemTable_GuildWarehouse);			
			SetDisableDialog(FALSE, eItemTable_PetInven);
			MSG_WORD2* pmsg = (MSG_WORD2*)pMsg;
			CItem* pItem = GAMEIN->GetInventoryDialog()->GetItemForPos(pmsg->wData1);
			if( pItem )
				pItem->SetLock( FALSE );
			GAMEIN->GetTitanPartsMakeDlg()->Release( eTPMAllRelease, FALSE );
			GAMEIN->GetTitanPartsMakeDlg()->DieCheck();
		}
		break;
	case MP_ITEM_TPM_SUCCESS_ACK:
		{
			MSG_ITEM_MIX_ACK * pmsg = (MSG_ITEM_MIX_ACK *)pMsg;
			TITAN_ENDURANCE_ITEMINFO EnduranceInfo;
			ITEM_INFO* pBaseInfo = GetItemInfo(pmsg->wBasicItemIdx);
			if(!pBaseInfo)
				break;
			if( (eITEM_KINDBIT)pBaseInfo->ItemKind & eTITAN_EQUIPITEM )
			{
				ITEMBASE* pItemBase = (ITEMBASE*)GetItemInfoAbsIn(HERO, pmsg->BasicItemPos);
				if( !pItemBase )
					break;
                TITAN_ENDURANCE_ITEMINFO* pEnduranceInfo = TITANMGR->GetTitanEnduranceInfo(pItemBase->dwDBIdx);
				CopyMemory( &EnduranceInfo, pEnduranceInfo, sizeof(TITAN_ENDURANCE_ITEMINFO) );
			}
			GAMEIN->GetTitanPartsMakeDlg()->Release(eTPMAllRelease, FALSE);
			CItem * BasicItemOut;
			DeleteItem(pmsg->BasicItemPos, &BasicItemOut, NULL, NULL, eRemainAllOption);
			CItem * pMatItemOut;
			for(int i = 0 ; i < pmsg->wMaterialNum ; ++i)
			{
				if(IsDupItem(pmsg->Material[i].wItemIdx))
				{
					CItem * pItem = GAMEIN->GetInventoryDialog()->GetItemForPos(pmsg->Material[i].ItemPos);
					if(pItem)
						if(pItem->GetDurability() - pmsg->Material[i].Dur != 0)
						{
							pItem->SetDurability(pItem->GetDurability() - pmsg->Material[i].Dur);
							continue;
						}
				}
				DeleteItem(pmsg->Material[i].ItemPos, &pMatItemOut);
			}
			ITEMBASE NewItem;
			NewItem = *(BasicItemOut->GetItemBaseInfo());
			NewItem.Position = pmsg->BasicItemPos;
			NewItem.wIconIdx = pmsg->wResultItemIdx;
			NewItem.RareIdx	= pmsg->dwRareIdx;
			NewItem.QuickPosition = 0;
			if( !IsDupItem( NewItem.wIconIdx ) )
				NewItem.Durability = 0;
			GAMEIN->GetInventoryDialog()->AddItem(&NewItem);
			CItem * pResultItem = GAMEIN->GetInventoryDialog()->GetItemForPos(pmsg->BasicItemPos);
			GAMEIN->GetTitanPartsMakeDlg()->AddVirtualItemWrap(eTPMPos_Result, pResultItem);
			pResultItem->SetLock(TRUE);
			CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(207), pResultItem->GetItemInfo()->ItemName );
			QUICKMGR->RefreshQickItem();
			if( (eITEM_KINDBIT)pBaseInfo->ItemKind & eTITAN_EQUIPITEM )
			{
                TITANMGR->AddTitanEquipItemEnduranceInfo(&EnduranceInfo);
				RefreshItemToolTip(EnduranceInfo.ItemDBIdx);
			}
			GAMEIN->GetTitanPartsMakeDlg()->DieCheck();
			if( GAMEIN->GetTitanPartsMakeDlg()->IsShopItemUse() )
			{
				GAMEIN->GetTitanPartsMakeDlg()->Release();
				GAMEIN->GetTitanPartsMakeDlg()->SetActiveRecursive( FALSE );
			}
		}
		break;
	case MP_ITEM_TPM_BIGFAILED_ACK:
		{
			MSG_ITEM_MIX_ACK * pmsg = (MSG_ITEM_MIX_ACK *)pMsg;
			GAMEIN->GetTitanPartsMakeDlg()->Release();
			CItem * BasicItemOut;
			DeleteItem(pmsg->BasicItemPos, &BasicItemOut);
			CItem * pMatItemOut;
			for(int i = 0 ; i < pmsg->wMaterialNum ; ++i)
			{
				if(IsDupItem(pmsg->Material[i].wItemIdx))
				{
					CItem * pItem = GAMEIN->GetInventoryDialog()->GetItemForPos(pmsg->Material[i].ItemPos);
					if(pItem->GetDurability() - pmsg->Material[i].Dur != 0)
					{
						pItem->SetDurability(pItem->GetDurability() - pmsg->Material[i].Dur);
						continue;
					}
				}
				DeleteItem(pmsg->Material[i].ItemPos, &pMatItemOut);
			}
			GAMEIN->GetTitanPartsMakeDlg()->Release(eTPMBasicRelease);
			CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(208));
			QUICKMGR->RefreshQickItem();
			GAMEIN->GetTitanPartsMakeDlg()->DieCheck();
			if( GAMEIN->GetTitanPartsMakeDlg()->IsShopItemUse() )
				GAMEIN->GetTitanPartsMakeDlg()->SetActiveRecursive( FALSE );
		}
		break;
	case MP_ITEM_TPM_FAILED_ACK:
		{
			GAMEIN->GetTitanPartsMakeDlg()->ResetCurSelCellPos();
			MSG_ITEM_MIX_ACK * pmsg = (MSG_ITEM_MIX_ACK *)pMsg;
			CItem * pMatItemOut;
			for(int i = 0 ; i < pmsg->wMaterialNum ; ++i)
			{
				if(IsDupItem(pmsg->Material[i].wItemIdx))
				{
					CItem * pItem = GAMEIN->GetInventoryDialog()->GetItemForPos(pmsg->Material[i].ItemPos);
					if(pItem->GetDurability() - pmsg->Material[i].Dur != 0)
					{
						pItem->SetDurability(pItem->GetDurability() - pmsg->Material[i].Dur);
						continue;
					}
				}
				DeleteItem(pmsg->Material[i].ItemPos, &pMatItemOut);
			}
			CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(209));
			QUICKMGR->RefreshQickItem();
			GAMEIN->GetTitanPartsMakeDlg()->DieCheck();
			if( GAMEIN->GetTitanPartsMakeDlg()->IsShopItemUse() )
			{
				GAMEIN->GetTitanPartsMakeDlg()->Release();
				GAMEIN->GetTitanPartsMakeDlg()->SetActiveRecursive( FALSE );
			}
		}
		break;
	case MP_ITEM_TPM_MSG:
		{
			MSG_DWORD2* pmsg = (MSG_DWORD2*)pMsg;
			char buf[256];
			memset( buf, 0, sizeof(char)*256 );
			switch( pmsg->dwData1 )
			{
			case 20:			
				sprintf( buf, CHATMGR->GetChatMsg(1086) );
				break;
			case 21:			
				{
					CItem * BasicItemOut;
					DeleteItem( (POSTYPE)pmsg->dwData2, &BasicItemOut );
					sprintf( buf, CHATMGR->GetChatMsg(1088) );
				}
				break;
			case 22:			
				sprintf( buf, CHATMGR->GetChatMsg(1087) );
				break;
			case 23:			
				sprintf( buf, CHATMGR->GetChatMsg(1082) );
				break;
			}
			CHATMGR->AddMsg( CTC_SYSMSG, buf );
			GAMEIN->GetTitanPartsMakeDlg()->Release();
			GAMEIN->GetTitanPartsMakeDlg()->DieCheck();
			GAMEIN->GetTitanPartsMakeDlg()->SetActiveRecursive( FALSE );
		}
		break;
	case MP_ITEM_MALL_MONEY:
		{
			MSG_MONEY* pmsg = (MSG_MONEY*)pMsg;
			char money[16] = { 0, };
			strcpy(money, AddComma(pmsg->dwTotalMoney));
			if (pmsg->bFlag==1)  
			{
				ITEM_INFO * pItem = ITEMMGR->GetItemInfo(pmsg->ItemIdx);
				HERO->SetMallMoney(HERO->GetMallMoney()-(pmsg->dwTotalMoney));
				//CHATMGR->AddMsg( CTC_GETITEM, CHATMGR->GetChatMsg(2066),pItem->ItemName,money );
				if (pItem==NULL)
					CHATMGR->AddMsg(CTC_GETITEM, "Spent %s",  money);
				else
					CHATMGR->AddMsg(CTC_GETITEM, CHATMGR->GetChatMsg(2066), pItem->ItemName, money);
			}
			if(pmsg->bFlag==2) 
			{
				if(pmsg->dwTotalMoney>0)
				{
					HERO->SetMallMoney(HERO->GetMallMoney()+(pmsg->dwTotalMoney));
					CHATMGR->AddMsg( CTC_ALERT_YELLOW, CHATMGR->GetChatMsg(2095),money );
					AUDIOMGR->Play(66, HERO);
					MoneyDropEffect();
					StoredPDPoint = pmsg->dwTotalMoney;
					if( !GAMEIN->GetGoldShopDialog()->IsActive() )
					{
						GAMEIN->GetMainInterfaceDialog()->SetAlram( OPT_MALLNOTICEDLGICON, TRUE );
						GAMEIN->GetMainInterfaceDialog()->RefreshBBPoint();
					}
				}
			}
			if (pmsg->bFlag==3)  
			{
				ITEM_INFO * pItem=ITEMMGR->GetItemInfo(pmsg->ItemIdx);
				if (pItem == NULL)
					CHATMGR->AddMsg(CTC_GETITEM, "2580 Spent %s", money);
				else
					CHATMGR->AddMsg(CTC_GETITEM, CHATMGR->GetChatMsg(2580), pItem->ItemName, money);				
			}
		}
		break;
	case MP_ITEM_GOLD_MONEY:
		{
			MSG_MONEY* pmsg = (MSG_MONEY*)pMsg;
			char money[16] = { 0, };
			strcpy(money, AddComma(pmsg->dwTotalMoney));
			if (pmsg->bFlag==1)  
			{
				ITEM_INFO * pItem=ITEMMGR->GetItemInfo(pmsg->ItemIdx);
				HERO->SetGoldMoney(HERO->GetGoldMoney()-(pmsg->dwTotalMoney));
				CHATMGR->AddMsg( CTC_GETITEM, CHATMGR->GetChatMsg(2067),pItem->ItemName,money );
			}
			if(pmsg->bFlag==2)  
			{
                CHATMGR->AddMsg( CTC_GETITEM, CHATMGR->GetChatMsg(2082),money );
				HERO->SetGoldMoney(HERO->GetGoldMoney()-(pmsg->dwTotalMoney));
			}
			if(pmsg->bFlag==3)  
			{
                CHATMGR->AddMsg( CTC_GETITEM, CHATMGR->GetChatMsg(2096),money );
				HERO->SetGoldMoney(HERO->GetGoldMoney()-(pmsg->dwTotalMoney));
			}
			if(pmsg->bFlag==4)  
			{
                CHATMGR->AddMsg( CTC_GETITEM, CHATMGR->GetChatMsg(2101),money );
				HERO->SetGoldMoney(HERO->GetGoldMoney()+(pmsg->dwTotalMoney));
			}
			if(pmsg->bFlag == 5)   
			{
				HERO->SetGoldMoney(pmsg->dwRealMoney);
				CHATMGR->AddMsg( CTC_QUEST, CHATMGR->GetChatMsg(2101),money );
				AUDIOMGR->Play(66, HERO);
				MoneyDropEffect();
				if( !GAMEIN->GetGoldShopDialog()->IsActive() )
					GAMEIN->GetMainInterfaceDialog()->SetAlram( OPT_MALLNOTICEDLGICON, TRUE );
			}
			if(pmsg->bFlag == 6) 
			{
                CHATMGR->AddMsg( CTC_GETITEM, CHATMGR->GetChatMsg(2276),money );
				HERO->SetGoldMoney(pmsg->dwRealMoney);
			}
		}
		break;
	case MP_ITEM_SW_MONEY:
		{
			 	MSG_MONEY* pmsg = (MSG_MONEY*)pMsg;
				if (pmsg->bFlag==1)  
				{
					ITEM_INFO * pItem=ITEMMGR->GetItemInfo(pmsg->ItemIdx);
					CHATMGR->AddMsg( CTC_GETITEM, CHATMGR->GetChatMsg(2068),pItem->ItemName,pmsg->dwTotalMoney );
				}
				HERO->SetFame(HERO->GetFame()-(pmsg->dwTotalMoney));
		}
		break;
	case MP_ITEM_TITANMIX_ACK:
		{
			SetDisableDialog(FALSE, eItemTable_Deal);
			SetDisableDialog(FALSE, eItemTable_Inventory);
			SetDisableDialog(FALSE, eItemTable_Pyoguk);
			SetDisableDialog(FALSE, eItemTable_GuildWarehouse);
			ITEMOBTAINARRAY * pmsg = (ITEMOBTAINARRAY *)pMsg;
			CItem* pItem = NULL;
			CItem* pPreItem = NULL;
			for(int i = 0 ; i < pmsg->ItemNum ; ++i)
			{
				if( pPreItem = GAMEIN->GetInventoryDialog()->GetItemForPos( pmsg->GetItem(i)->Position ) )
				{
					DeleteItem(pmsg->GetItem(i)->Position, &pPreItem);
					//ASSERT(pPreItem);
				}
				pItem = MakeNewItem(pmsg->GetItem(i),"MP_ITEM_BUY_ACK");	
				BOOL rt = GAMEIN->GetInventoryDialog()->AddItem(pItem);
				if(!rt)
				{
					//ASSERT(0);					
				}
			}
			CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1556) );
		}
		break;
	case MP_ITEM_TITANMIX_ACK_DELETEPARTS:
		{
			MSG_ITEM_DISCARD_ACK* pmsg = (MSG_ITEM_DISCARD_ACK*)pMsg;
			GAMEIN->GetTitanMixDlg()->Release( eTMixAllRelease, FALSE );
			GAMEIN->GetTitanMixDlg()->UpdateData(NULL, FALSE);
			WORD wTableIdx = GetTableIdxForAbsPos(pmsg->TargetPos);
			DeleteItemofTable(wTableIdx, pmsg->TargetPos);
			SetDisableDialog(FALSE, eItemTable_Inventory);
			SetDisableDialog(FALSE, eItemTable_Pyoguk);
			SetDisableDialog(FALSE, eItemTable_GuildWarehouse);
			SetDisableDialog(FALSE, eItemTable_PetInven);
		}
		break;
	case MP_ITEM_TITANMIX_ADDITEM_ACK:
		{
			SetDisableDialog(FALSE, eItemTable_Inventory);
			SetDisableDialog(FALSE, eItemTable_Pyoguk);
			SetDisableDialog(FALSE, eItemTable_GuildWarehouse);
			SetDisableDialog(FALSE, eItemTable_PetInven);
			MSG_ITEM* pmsg = (MSG_ITEM*)pMsg;
			CItem* pItem = GAMEIN->GetInventoryDialog()->GetItemForPos(pmsg->ItemInfo.Position);			
			GAMEIN->GetTitanMixDlg()->DieCheck();
		}
		break;
	case MP_ITEM_TITANMIX_ADDITEM_NACK:
		{
			SetDisableDialog(FALSE, eItemTable_Inventory);
			SetDisableDialog(FALSE, eItemTable_Pyoguk);
			SetDisableDialog(FALSE, eItemTable_GuildWarehouse);			
			SetDisableDialog(FALSE, eItemTable_PetInven);
			MSG_WORD2* pmsg = (MSG_WORD2*)pMsg;
			CItem* pItem = GAMEIN->GetInventoryDialog()->GetItemForPos(pmsg->wData1);
			if( pItem )
				pItem->SetLock( FALSE );
			GAMEIN->GetTitanMixDlg()->Release( eTMixAllRelease, FALSE );
			GAMEIN->GetTitanMixDlg()->DieCheck();
		}
		break;
	case MP_ITEM_TITANUPGRADE_ADDITEM_ACK:
		{
			SetDisableDialog(FALSE, eItemTable_Inventory);
			SetDisableDialog(FALSE, eItemTable_Pyoguk);
			SetDisableDialog(FALSE, eItemTable_GuildWarehouse);
			SetDisableDialog(FALSE, eItemTable_PetInven);
			MSG_ITEM* pmsg = (MSG_ITEM*)pMsg;
			CItem* pItem = GAMEIN->GetInventoryDialog()->GetItemForPos(pmsg->ItemInfo.Position);			
			GAMEIN->GetTitanUpgradeDlg()->DieCheck();
		}
		break;
	case MP_ITEM_TITANUPGRADE_ADDITEM_NACK:
		{
			SetDisableDialog(FALSE, eItemTable_Inventory);
			SetDisableDialog(FALSE, eItemTable_Pyoguk);
			SetDisableDialog(FALSE, eItemTable_GuildWarehouse);			
			SetDisableDialog(FALSE, eItemTable_PetInven);
			MSG_WORD2* pmsg = (MSG_WORD2*)pMsg;
			CItem* pItem = GAMEIN->GetInventoryDialog()->GetItemForPos(pmsg->wData1);
			if( pItem )
				pItem->SetLock( FALSE );
			switch(pmsg->wData2)
			{
			case 0:
				break;
			case 1:
				break;
			case 2:
				break;
			case 3:
				break;
			case 4:
				break;
			case 5:
				{
					CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(1581));
				}
				break;
			}
			GAMEIN->GetTitanUpgradeDlg()->Release(FALSE);
			GAMEIN->GetTitanUpgradeDlg()->DieCheck();
		}
		break;
	case MP_ITEM_TITANUPGRADE_ACK:
	case MP_ITEM_TITANBREAK_ACK:
		{
			SetDisableDialog(FALSE, eItemTable_Deal);
			SetDisableDialog(FALSE, eItemTable_Inventory);
			SetDisableDialog(FALSE, eItemTable_Pyoguk);
			SetDisableDialog(FALSE, eItemTable_GuildWarehouse);
			ITEMOBTAINARRAY * pmsg = (ITEMOBTAINARRAY *)pMsg;
			CItem* pItem = NULL;
			CItem* pPreItem = NULL;
			for(int i = 0 ; i < pmsg->ItemNum ; ++i)
			{
				if( pPreItem = GAMEIN->GetInventoryDialog()->GetItemForPos( pmsg->GetItem(i)->Position ) )
				{
					DeleteItem(pmsg->GetItem(i)->Position, &pPreItem);
					ASSERT(pPreItem);
				}
				pItem = MakeNewItem(pmsg->GetItem(i),"MP_ITEM_BUY_ACK");	
				BOOL rt = GAMEIN->GetInventoryDialog()->AddItem(pItem);
				if(!rt)
				{
					ASSERT(0);					
				}
				else
				{
					if(pmsg->Protocol == MP_ITEM_TITANBREAK_ACK)
					{
						CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1501), pItem->GetItemInfo()->ItemName, pItem->GetDurability());
					}
				}
			}
		}
		break;
	case MP_ITEM_TITANUPGRADE_NACK:
		{
			MSG_BYTE* pmsg = (MSG_BYTE*)pMsg;
			switch(pmsg->bData)
			{
			case 0:
				break;
			case 1:
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(205) );
				break;
			case 2:
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1502) );
				break;
			case 3:
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(117) );
				break;
			}
		}
		break;
	case MP_ITEM_TITANBREAK_ACK_DELETEITEM:
		{
			MSG_ITEM_DISCARD_ACK* pmsg = (MSG_ITEM_DISCARD_ACK*)pMsg;
			GAMEIN->GetTitanBreakDlg()->Release();
			SetDisableDialog(FALSE, eItemTable_Inventory);
			SetDisableDialog(FALSE, eItemTable_Pyoguk);
			SetDisableDialog(FALSE, eItemTable_GuildWarehouse);
			SetDisableDialog(FALSE, eItemTable_PetInven);
			CItem * pItemOut;
			if(IsDupItem(pmsg->wItemIdx))
			{
				CItem * pItem = GAMEIN->GetInventoryDialog()->GetItemForPos(pmsg->TargetPos);
				if( pItem && (pItem->GetDurability() - pmsg->ItemNum != 0) )
				{
					pItem->SetDurability(pItem->GetDurability() - pmsg->ItemNum);
					break;
				}
			}
			DeleteItem(pmsg->TargetPos, &pItemOut);
		}
		break;
	case MP_ITEM_TITANBREAK_ADDITEM_ACK:
		{
			SetDisableDialog(FALSE, eItemTable_Inventory);
			SetDisableDialog(FALSE, eItemTable_Pyoguk);
			SetDisableDialog(FALSE, eItemTable_GuildWarehouse);
			SetDisableDialog(FALSE, eItemTable_PetInven);
			MSG_ITEM* pmsg = (MSG_ITEM*)pMsg;
			CItem* pItem = GAMEIN->GetInventoryDialog()->GetItemForPos(pmsg->ItemInfo.Position);			
			GAMEIN->GetTitanBreakDlg()->DieCheck();
		}
		break;
	case MP_ITEM_TITANBREAK_ADDITEM_NACK:
		{
			SetDisableDialog(FALSE, eItemTable_Inventory);
			SetDisableDialog(FALSE, eItemTable_Pyoguk);
			SetDisableDialog(FALSE, eItemTable_GuildWarehouse);			
			SetDisableDialog(FALSE, eItemTable_PetInven);
			MSG_WORD2* pmsg = (MSG_WORD2*)pMsg;
			CItem* pItem = GAMEIN->GetInventoryDialog()->GetItemForPos(pmsg->wData1);
			if( pItem )
				pItem->SetLock( FALSE );
			GAMEIN->GetTitanBreakDlg()->Release(FALSE);
			GAMEIN->GetTitanBreakDlg()->DieCheck();
		}
		break;
	case MP_ITEM_TITANUPGRADE_ACK_DELETEMATERIAL:
		{
			MSG_ITEM_DISCARD_ACK* pmsg = (MSG_ITEM_DISCARD_ACK*)pMsg;
			GAMEIN->GetTitanUpgradeDlg()->Release();
			SetDisableDialog(FALSE, eItemTable_Inventory);
			SetDisableDialog(FALSE, eItemTable_Pyoguk);
			SetDisableDialog(FALSE, eItemTable_GuildWarehouse);
			SetDisableDialog(FALSE, eItemTable_PetInven);
			CItem * pMatItemOut;
			if(IsDupItem(pmsg->wItemIdx))
			{
				CItem * pItem = GAMEIN->GetInventoryDialog()->GetItemForPos(pmsg->TargetPos);
				if( pItem && (pItem->GetDurability() - pmsg->ItemNum != 0) )
				{
					pItem->SetDurability(pItem->GetDurability() - pmsg->ItemNum);
					break;
				}
			}
			DeleteItem(pmsg->TargetPos, &pMatItemOut);
		}
		break;
	case MP_ITEM_TITAN_REGISTER_ADDITEM_ACK:
		{
			SetDisableDialog(FALSE, eItemTable_Inventory);
			SetDisableDialog(FALSE, eItemTable_Pyoguk);
			SetDisableDialog(FALSE, eItemTable_GuildWarehouse);
			SetDisableDialog(FALSE, eItemTable_PetInven);
			MSG_ITEM* pmsg = (MSG_ITEM*)pMsg;
			CItem* pItem = GAMEIN->GetInventoryDialog()->GetItemForPos(pmsg->ItemInfo.Position);			
		}
		break;
	case MP_ITEM_TITAN_REGISTER_ADDITEM_NACK:
		{
			SetDisableDialog(FALSE, eItemTable_Inventory);
			SetDisableDialog(FALSE, eItemTable_Pyoguk);
			SetDisableDialog(FALSE, eItemTable_GuildWarehouse);			
			SetDisableDialog(FALSE, eItemTable_PetInven);
			MSG_WORD2* pmsg = (MSG_WORD2*)pMsg;
			CItem* pItem = GAMEIN->GetInventoryDialog()->GetItemForPos(pmsg->wData1);
			if( pItem )
				pItem->SetLock( FALSE );
			GAMEIN->GetTitanRegisterDlg()->Release(FALSE);
		}
		break;
	case MP_ITEM_TITAN_REGISTER_ACK:
		{
			SetDisableDialog(FALSE, eItemTable_Inventory);
			SetDisableDialog(FALSE, eItemTable_Pyoguk);
			SetDisableDialog(FALSE, eItemTable_GuildWarehouse);			
			SetDisableDialog(FALSE, eItemTable_PetInven);
			MSG_DWORD2* pmsg = (MSG_DWORD2*)pMsg;
			CItem* pItem = GAMEIN->GetInventoryDialog()->GetItemForPos((POSTYPE)pmsg->dwData2);
			if( pItem )
				pItem->SetLock( FALSE );
			GAMEIN->GetTitanRegisterDlg()->SetActive(FALSE);
			GAMEIN->GetTitanRegisterDlg()->DieCheck();
			TITAN_TOTALINFO* pInfo = TITANMGR->GetTitanInfo(pItem->GetDBIdx());
			if(!pInfo)
			{
				ASSERT(0);
				break;
			}
			pInfo->RegisterCharacterIdx = pmsg->dwData1;
			if(pInfo->RegisterCharacterIdx > 0)
			{
				TITANMGR->SetRegistedTitanItemDBIdx(pItem->GetDBIdx());
				STATSMGR->CalcTitanStats(pItem->GetDBIdx());
				ITEMPARAM Param = pItem->GetItemBaseInfo()->ItemParam;
				if(!(Param & ITEM_PARAM_TITAN_REGIST))
				{
					Param |= ITEM_PARAM_TITAN_REGIST;
					pItem->SetItemParam( Param );
				}
			}
			CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1511));
			RefreshItemToolTip(pItem->GetDBIdx());
		}
		break;
	case MP_ITEM_TITAN_REGISTER_NACK:
		{
			SetDisableDialog(FALSE, eItemTable_Inventory);
			SetDisableDialog(FALSE, eItemTable_Pyoguk);
			SetDisableDialog(FALSE, eItemTable_GuildWarehouse);			
			SetDisableDialog(FALSE, eItemTable_PetInven);
			MSG_DWORD2* pmsg = (MSG_DWORD2*)pMsg;
			CItem* pItem = GAMEIN->GetInventoryDialog()->GetItemForPos((POSTYPE)pmsg->dwData2);
			if( pItem )
				pItem->SetLock( FALSE );
			GAMEIN->GetTitanRegisterDlg()->SetActive(FALSE);
			GAMEIN->GetTitanRegisterDlg()->DieCheck();
		}
		break;
	case MP_ITEM_TITAN_DISSOLUTION_ADDITEM_ACK:
		{
			SetDisableDialog(FALSE, eItemTable_Inventory);
			SetDisableDialog(FALSE, eItemTable_Pyoguk);
			SetDisableDialog(FALSE, eItemTable_GuildWarehouse);
			SetDisableDialog(FALSE, eItemTable_PetInven);
			MSG_ITEM* pmsg = (MSG_ITEM*)pMsg;
			CItem* pItem = GAMEIN->GetInventoryDialog()->GetItemForPos(pmsg->ItemInfo.Position);			
		}
		break;
	case MP_ITEM_TITAN_DISSOLUTION_ADDITEM_NACK:
		{
			SetDisableDialog(FALSE, eItemTable_Inventory);
			SetDisableDialog(FALSE, eItemTable_Pyoguk);
			SetDisableDialog(FALSE, eItemTable_GuildWarehouse);			
			SetDisableDialog(FALSE, eItemTable_PetInven);
			MSG_WORD2* pmsg = (MSG_WORD2*)pMsg;
			CItem* pItem = GAMEIN->GetInventoryDialog()->GetItemForPos(pmsg->wData1);
			if( pItem )
				pItem->SetLock( FALSE );
			GAMEIN->GetTitanDissolutionDlg()->Release(FALSE);
		}
		break;
	case MP_ITEM_TITAN_DISSOLUTION_ACK:
		{
			SetDisableDialog(FALSE, eItemTable_Inventory);
			SetDisableDialog(FALSE, eItemTable_Pyoguk);
			SetDisableDialog(FALSE, eItemTable_GuildWarehouse);			
			SetDisableDialog(FALSE, eItemTable_PetInven);
			MSG_DWORD2* pmsg = (MSG_DWORD2*)pMsg;
			CItem* pItem = GAMEIN->GetInventoryDialog()->GetItemForPos((POSTYPE)pmsg->dwData2);
			if( pItem )
				pItem->SetLock( FALSE );
			GAMEIN->GetTitanDissolutionDlg()->SetActive(FALSE);
			GAMEIN->GetTitanDissolutionDlg()->DieCheck();
			TITAN_TOTALINFO* pInfo = TITANMGR->GetTitanInfo(pItem->GetDBIdx());
			if(!pInfo)
			{
				ASSERT(0);
				break;
			}
			pInfo->RegisterCharacterIdx = pmsg->dwData1;
			if(pInfo->RegisterCharacterIdx == 0)
			{
				TITANMGR->SetRegistedTitanItemDBIdx(0);
				ITEMPARAM Param = pItem->GetItemBaseInfo()->ItemParam;
				if(Param & ITEM_PARAM_TITAN_REGIST)
				{
					Param ^= ITEM_PARAM_TITAN_REGIST;
					pItem->SetItemParam( Param );
				}
			}
			CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1512));
			RefreshItemToolTip(pItem->GetDBIdx());
		}
		break;
	case MP_ITEM_TITAN_DISSOLUTION_NACK:
		{
			SetDisableDialog(FALSE, eItemTable_Inventory);
			SetDisableDialog(FALSE, eItemTable_Pyoguk);
			SetDisableDialog(FALSE, eItemTable_GuildWarehouse);			
			SetDisableDialog(FALSE, eItemTable_PetInven);
			MSG_DWORD2* pmsg = (MSG_DWORD2*)pMsg;
			CItem* pItem = GAMEIN->GetInventoryDialog()->GetItemForPos((POSTYPE)pmsg->dwData2);
			if( pItem )
				pItem->SetLock( FALSE );
			GAMEIN->GetTitanDissolutionDlg()->SetActive(FALSE);
			GAMEIN->GetTitanDissolutionDlg()->DieCheck();
		}
		break;
	case MP_ITEM_UPGRADE_SUCCESS_ACK:
	{
		MSG_ITEM_UPGRADE_ACK* pmsg = (MSG_ITEM_UPGRADE_ACK*)pMsg;

		// 输出调试信息
		ITEM_INFO* pInfo = ITEMMGR->GetItemInfo(pmsg->wItemIdx);
		CHATMGR->AddMsg(CTC_SYSMSG, "收到强化结果：ItemPos=%d, MaterialPos=%d, ItemIdx=%d",
			pmsg->ItemPos, pmsg->MaterialItemPos, pmsg->wItemIdx);
		if (pInfo)
			CHATMGR->AddMsg(CTC_SYSMSG, "目标物品：%s", pInfo->ItemName);

		CItem* itemOut;
		CItem* MaterialItemOut;
		ITEM_OPTION_INFO OptionInfo;
		DeleteItem(pmsg->ItemPos, &itemOut, &OptionInfo);
		DeleteItem(pmsg->MaterialItemPos, &MaterialItemOut);

		int grade = pmsg->wItemIdx - itemOut->GetItemIdx();
		if (grade > 0)
			CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(176), grade);
		else if (grade == 0)
			CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(177));
		else
			CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(178), grade);

		if (OptionInfo.dwOptionIdx != 0)
			AddItemOption(&OptionInfo);

		ITEMBASE NewItem;
		NewItem = *(itemOut->GetItemBaseInfo());
		NewItem.Position = pmsg->ItemPos;
		NewItem.wIconIdx = pmsg->wItemIdx;
		NewItem.QuickPosition = 0;

		GAMEIN->GetInventoryDialog()->AddItem(&NewItem);
		GAMEIN->GetUpgradeDialog()->Release();

		CItem* pResultItem = GAMEIN->GetInventoryDialog()->GetItemForPos(pmsg->ItemPos);
		GAMEIN->GetUpgradeDialog()->AddVirtualItemWrap(eRESULTITEM_POS, pResultItem);
		pResultItem->SetLock(TRUE);
	}
	break;

	case MP_ITEM_UPGRADE_NACK:
		{
			ASSERT(0);
			CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(176) );
		}
		break;
	case MP_ITEM_USE_NACK:
		{			
			SetDisableDialog(FALSE, eItemTable_Inventory);
			SetDisableDialog(FALSE, eItemTable_Pyoguk);
			SetDisableDialog(FALSE, eItemTable_GuildWarehouse);
			MSG_ITEM_ERROR * pmsg = (MSG_ITEM_ERROR*)pMsg;
			if(pmsg->ECode == eItemUseErr_PreInsert)
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(678));
			else if( pmsg->ECode == eItemUseErr_TitanItemTime )
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(909));
			m_nItemUseCount--;
		}
		break;
	case MP_ITEM_SELL_ACK:
		{
			MSG_ITEM_SELL_ACK * pmsg = ( MSG_ITEM_SELL_ACK * )pMsg;	
			CItem * pItem = GAMEIN->GetInventoryDialog()->GetItemForPos(pmsg->TargetPos);
			if(IsDupItem(pmsg->wSellItemIdx) && (pItem->GetDurability() - pmsg->SellItemNum) > 0)
			{
				pItem->SetDurability( pItem->GetDurability() - pmsg->SellItemNum );
				DWORD SellPrice = SWPROFIT->CalTexRateForSell( pItem->GetItemInfo()->SellPrice );
				SellPrice = FORTWARMGR->CalTexRateForSell( pItem->GetItemInfo()->SellPrice );
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(218), pItem->GetItemInfo()->ItemName, pmsg->SellItemNum, AddComma( SellPrice*pmsg->SellItemNum ) );
			}
			else
			{
				DWORD SellPrice = SWPROFIT->CalTexRateForSell( pItem->GetItemInfo()->SellPrice );
				SellPrice = FORTWARMGR->CalTexRateForSell( pItem->GetItemInfo()->SellPrice );
				if (pmsg->SellItemNum == 0 || IsOptionItem(pItem->GetItemIdx(), pItem->GetDurability()))
				{
					if (pItem->GetItemKind() & eEQUIP_ITEM || pItem->GetItemKind() == eEQUIP_ITEM_UNIQUE)
					{
						if (pItem->GetQuality() == 4)
						{
							CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(2775), pItem->GetItemInfo()->ItemName, AddComma(SellPrice));
						}
						if (pItem->GetQuality() == 3)
						{
							CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(2774), pItem->GetItemInfo()->ItemName, AddComma(SellPrice));
						}
						if (pItem->GetQuality() == 2)
						{
							CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(2773), pItem->GetItemInfo()->ItemName, AddComma(SellPrice));
						}
						if (pItem->GetQuality() == 1)
						{
							CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(2772), pItem->GetItemInfo()->ItemName, AddComma(SellPrice));
						}
						if (pItem->GetQuality() == 0)
						{
							CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(2771), pItem->GetItemInfo()->ItemName, AddComma(SellPrice));
						}
					}
					else
						CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(217), pItem->GetItemInfo()->ItemName, AddComma(SellPrice));
				}
				else
					CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(218), pItem->GetItemInfo()->ItemName, pmsg->SellItemNum, AddComma(SellPrice * pmsg->SellItemNum));				DeleteItemofTable(GetTableIdxForAbsPos(pmsg->TargetPos), pmsg->TargetPos);
			}
			GAMEIN->GetInventoryDialog()->SetDisable( FALSE );
			GAMEIN->GetDealDialog()->SetDisable( FALSE );
			QUICKMGR->RefreshQickItem();
		}
		break;
	case MP_ITEM_SELL_NACK:
		{
			MSG_ITEM_ERROR* pmsg = (MSG_ITEM_ERROR*)pMsg;
			if( pmsg->ECode == 9 )
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(355) );
			else if(pmsg->ECode == 11)
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1552) );
			else if(pmsg->ECode == 133)
				cMsgBox* pMsgBox = WINDOWMGR->MsgBox( MBI_OTHERUSER_CONNECTTRY, MBT_OK, CHATMGR->GetChatMsg( 2212 ));
			GAMEIN->GetInventoryDialog()->SetDisable( FALSE );
			GAMEIN->GetDealDialog()->SetDisable( FALSE );
		}
		break;
	case MP_ITEM_BUY_ACK:     
		{
			SetDisableDialog(FALSE, eItemTable_Deal);
			SetDisableDialog(FALSE, eItemTable_Inventory);
			SetDisableDialog(FALSE, eItemTable_Pyoguk);
			SetDisableDialog(FALSE, eItemTable_GuildWarehouse);
			BOOL SetQuickLink = FALSE; 
			ITEMOBTAINARRAY * pmsg = (ITEMOBTAINARRAY *)pMsg;
			CItem* pItem = NULL;
			CItem* pPreItem = NULL;
			for(int i = 0 ; i < pmsg->ItemNum ; ++i)
			{
				if( pPreItem = GAMEIN->GetInventoryDialog()->GetItemForPos( pmsg->GetItem(i)->Position ) )
				{
					DeleteItem(pmsg->GetItem(i)->Position, &pPreItem);
					ASSERT(pPreItem);
				}
				pItem = MakeNewItem(pmsg->GetItem(i),"MP_ITEM_BUY_ACK");	
				BOOL rt = GAMEIN->GetInventoryDialog()->AddItem(pItem);
				if(!rt)
				{
					ASSERT(0);					
				}
				if(pItem->GetQuickPosition() != 0)
				{
					if(SetQuickLink == TRUE)
					{
						char buf[256];
						sprintf(buf, "Set QuickPosition again! CharacterIDX : %d, ItemDBIdx : %d", HEROID, pItem->GetItemBaseInfo()->dwDBIdx);
						ASSERTMSG(0, buf);
					}
					WORD QuickPosList[4];
					CQuickManager::AnalyzeQuickPosition(pItem->GetQuickPosition(), QuickPosList);
					for(WORD SheetNum = 0; SheetNum < 4; SheetNum++)
					{
						if(QuickPosList[SheetNum])
							QUICKMGR->AddQuickItemReal(((SheetNum * 10) + (QuickPosList[SheetNum] - 1)),pItem->GetPosition(),pItem->GetItemIdx());
					}
					SetQuickLink = TRUE;
				}
			}
			MONEYTYPE BuyPrice;
			if (pmsg->BuyType == 1 || pmsg->BuyType == 2 || pmsg->BuyType == 3)
			{
				return;
			}
			else
			{
				BuyPrice = pItem->GetItemInfo()->BuyPrice;
				if(MAP->IsVillage() == FALSE)	   
				{		
					BuyPrice = (MONEYTYPE)(BuyPrice * 1.2);	
				}
				BuyPrice = SWPROFIT->CalTexRateForBuy( BuyPrice );
				BuyPrice = FORTWARMGR->CalTexRateForBuy( BuyPrice );
			}
			if( pmsg->wObtainCount > 1 )
			{
				CHATMGR->AddMsg( CTC_GETITEM, CHATMGR->GetChatMsg(216), pItem->GetItemInfo()->ItemName, pmsg->wObtainCount,
					AddComma( BuyPrice * pmsg->wObtainCount ) );
			}
			else
			{
				CHATMGR->AddMsg( CTC_GETITEM, CHATMGR->GetChatMsg(215), pItem->GetItemInfo()->ItemName, AddComma( BuyPrice ) );
			}
			QUICKMGR->RefreshQickItem();
		}
		break;
	case MP_ITEM_BUY_NACK:
		{
			MSG_ITEM_ERROR* pmsg = (MSG_ITEM_ERROR*)pMsg;
			SetDisableDialog(FALSE, eItemTable_Inventory);
			SetDisableDialog(FALSE, eItemTable_Pyoguk);
			SetDisableDialog(FALSE, eItemTable_GuildWarehouse);
			SetDisableDialog(FALSE, eItemTable_Deal);
			switch(pmsg->ECode)
			{
			case 201:
			case 202:
			case 203:
			case 204:
			case 205:
			case 206:
			case 207:
			case 208:
			case 209:
				CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(2725), pmsg->ECode - 200);
				break;
			case 210:
				CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(2726), pmsg->ECode - 200);
				break;
			case NOT_MONEY:	
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(117) );
				break;
			case NOT_SPACE:
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(354) );
				break;
			case NOT_EXIST:
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(356) );
				break;
			case NOT_PLAYER:
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(357) );
				break;
			case NO_DEMANDITEM:
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1639) );
				break;
			case NOT_REMAINITEM:
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1632) );
				break;
			case NOT_LIMITTIMES:
				CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(2646));
				break;
			default:
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(358) );
				break;
			}
		}
		break;
	case MP_ITEM_MALL_GOLD_GET_NACK:
		{
			MSG_DWORD3* pmsg = (MSG_DWORD3*)pMsg;
			char buf[256] = {0,};
			DWORD dwData=pmsg->dwData1;
			DWORD hour = dwData / ( 60 * 60 * 1000 );
			DWORD minute = ( dwData / ( 60 * 1000 ) ) % 60;
			DWORD second = ( dwData /  1000 ) % 60;
			wsprintf( buf, CHATMGR->GetChatMsg( 2640 ), hour, minute ,second) ;
			WINDOWMGR->MsgBox( MBI_INVEN_FULL_ALERT, MBT_OK, buf);
		}
		break;
	case MP_ITEM_DEALER_ACK:
		{			
			MSG_WORD* pmsg = (MSG_WORD*)pMsg;
			if( HERO->GetState() != eObjectState_Die)
				GAMEIN->GetDealDialog()->ShowDealer(pmsg->wData);
		}
		break;
	case MP_ITEM_DEALER_NACK:
		{
		}
		break;
	case MP_ITEM_APPEARANCE_CHANGE:
		{
			APPEARANCE_INFO* pmsg = (APPEARANCE_INFO*)pMsg;
			CObject* pObject = (CObject*)OBJECTMGR->GetObject(pmsg->PlayerID);
			if( pObject && pObject->GetObjectKind() == eObjectKind_Player )
			{
				CPlayer* pPlayer = (CPlayer*)pObject;
				for(int n=0;n<eWearedItem_Max;++n)
					pPlayer->SetWearedItemIdx(n,pmsg->WearedItem[n]);
				APPEARANCEMGR->AddCharacterPartChange(pObject->GetID());
			}
			else
			{
				ASSERT(0);
			}
		}
		break;
	case MP_ITEM_TITAN_APPEARANCE_CHANGE:
		{
			SEND_TITAN_APPEARANCEINFO* pmsg = (SEND_TITAN_APPEARANCEINFO*)pMsg;
			CObject* pObject = (CObject*)OBJECTMGR->GetObject(pmsg->OwnerID);
			if( pObject && pObject->GetObjectKind() == eObjectKind_Player )
			{
				CPlayer* pPlayer = (CPlayer*)pObject;
				pPlayer->SetTitanAppearanceInfo(&pmsg->titanAppearanceInfo);
				APPEARANCEMGR->InitAppearance( pPlayer );				
				APPEARANCEMGR->AddCharacterPartChange(pPlayer->GetID());				
				pPlayer->SetBaseMotion();
			}
		}
		break;
	case MP_ITEM_MONEY:
		{
			MSG_MONEY* pmsg = (MSG_MONEY*)pMsg;
			MONEYTYPE OriMoney = HERO->GetMoney();
			MONEYTYPE PrintMoney = 0;
			switch(pmsg->bFlag)
			{
			case 64:
				{
					PrintMoney = pmsg->dwTotalMoney - OriMoney;
					if( PrintMoney )
					{
						CHATMGR->AddMsg( CTC_GETMONEY, CHATMGR->GetChatMsg( 120 ), AddComma( PrintMoney ) );
						if( HERO->InTitan() == TRUE )
							EFFECTMGR->StartHeroEffectProcess(eEffect_Titan_GetMoney);
						else
							EFFECTMGR->StartHeroEffectProcess(eEffect_GetMoney);
					}
				}
				break;
			case MF_NOMAL:
				{
				}
				break;
			case MF_OBTAIN: 
				{
					PrintMoney = pmsg->dwTotalMoney - OriMoney;
					if( PrintMoney )
					{
						CHATMGR->AddMsg( CTC_GETMONEY, CHATMGR->GetChatMsg( 120 ), AddComma( PrintMoney ) );
						if( HERO->InTitan() == TRUE )
							EFFECTMGR->StartHeroEffectProcess(eEffect_Titan_GetMoney);
						else
							EFFECTMGR->StartHeroEffectProcess(eEffect_GetMoney);
					}
				}
				break;
			case MF_LOST:
				{
					PrintMoney = OriMoney - pmsg->dwTotalMoney;
					if( PrintMoney )
					{
						CHATMGR->AddMsg( CTC_GETMONEY, CHATMGR->GetChatMsg( 118 ), AddComma( PrintMoney) );
					}
				}
				break;
			default:
				break;
			}
			HERO->SetMoney(pmsg->dwTotalMoney);
		}
		break;

	case MP_ITEM_MONEY_ERROR:
		{
			CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(115) );
		}
		break;
	case MP_ITEM_OBTAIN_MONEY:
		{
			ASSERT(0);
			MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;
			DWORD OriMoney = HERO->GetMoney();
			DWORD PrintMoney = pmsg->dwData - OriMoney;
			if( PrintMoney )
			{
				CHATMGR->AddMsg( CTC_GETMONEY, CHATMGR->GetChatMsg( 120 ), PrintMoney );
				if( HERO->InTitan() == TRUE )
					EFFECTMGR->StartHeroEffectProcess(eEffect_Titan_GetMoney);
				else
					EFFECTMGR->StartHeroEffectProcess(eEffect_GetMoney);
			}
			HERO->SetMoney(pmsg->dwData);
		}
		break;
	case MP_ITEM_DIVIDE_NEWITEM_NOTIFY:
		{
			MSG_ITEM* pmsg = (MSG_ITEM*)pMsg;
			CItem* pItem = MakeNewItem(&pmsg->ItemInfo,"MP_ITEM_DIVIDE_NEWITEM_NOTIFY");
			int tbidx = GetTableIdxForAbsPos(pItem->GetItemBaseInfo()->Position);
			if(tbidx == eItemTable_Inventory || tbidx == eItemTable_ShopInven)
				BOOL rt = GAMEIN->GetInventoryDialog()->AddItem(pItem);
			else if(tbidx == eItemTable_Pyoguk)
				BOOL rt = GAMEIN->GetPyogukDialog()->AddItem(pItem);
			else if(tbidx == eItemTable_GuildWarehouse)
				BOOL rt = GAMEIN->GetGuildWarehouseDlg()->AddItem(pItem);
			CHATMGR->AddMsg( CTC_GETITEM, CHATMGR->GetChatMsg( 123 ));
		}
		break;
	case MP_ITEM_GUILD_MOVE_ACK:
		{
			MoveItemToGuild((MSG_ITEM_MOVE_ACK*)pMsg);
			WINDOWMGR->DragWindowNull();
		}
		break;
	case MP_ITEM_GUILD_MOVE_NACK:
		{
			SetDisableDialog(FALSE, eItemTable_Inventory);
			SetDisableDialog(FALSE, eItemTable_Pyoguk);
			SetDisableDialog(FALSE, eItemTable_GuildWarehouse);
			WINDOWMGR->BackDragWindow();
		}
		break;
	case MP_ITEM_GUILD_WAREHOUSE_INFO_ACK:
		{
			SEND_MUNPA_WAREHOUSE_ITEM_INFO* pmsg = (SEND_MUNPA_WAREHOUSE_ITEM_INFO*)pMsg;
			CAddableInfoIterator iter(&pmsg->AddableInfo);
			BYTE AddInfoKind;
			while((AddInfoKind = iter.GetInfoKind()) != CAddableInfoList::None)
			{
				switch(AddInfoKind)
				{
				case(CAddableInfoList::ItemOption):
					{
						ITEM_OPTION_INFO OptionInfo[TABCELL_GUILDWAREHOUSE_NUM];
						iter.GetInfoData(&OptionInfo);
						ITEMMGR->InitItemOption(OptionInfo, pmsg->wOptionCount);
					}
					break;
				case(CAddableInfoList::ItemRareOption):
					{
						ITEM_RARE_OPTION_INFO RareOptionInfo[TABCELL_GUILDWAREHOUSE_NUM];
						iter.GetInfoData(&RareOptionInfo);
						ITEMMGR->InitItemRareOption(RareOptionInfo, pmsg->wRareOptionCount);
					}
					break;
				}
				iter.ShiftToNextData();
			}
			GAMEIN->GetGuildWarehouseDlg()->SetWarehouseInfo(pmsg);
			RefreshAllItem();
		}
		break;
	case MP_ITEM_GUILD_WAREHOUSE_INFO_NACK:
		{
			ASSERTMSG(0, "warehouse_info_nack");
			GAMEIN->GetGuildWarehouseDlg()->SetLock(FALSE);
		}
		break;
	case MP_ITEM_DELETEFROM_GUILDWARE_NOTIFY:
		{
			MSG_ITEM * pItembase = (MSG_ITEM*)pMsg;
			CItem* pItem = NULL;
			if(GAMEIN->GetGuildWarehouseDlg()->DeleteItem(pItembase->ItemInfo.Position, &pItem) == TRUE)
				ITEMMGR->ItemDelete(pItem);
			else
			{
				ASSERT(0);
			}
		}
		break;
	case MP_ITEM_ADDTO_GUILDWARE_NOTIFY:
		{
			MSG_ITEM_WITH_OPTION * pItem = (MSG_ITEM_WITH_OPTION*)pMsg;
			if(pItem->IsOptionItem == TRUE)
			{
				if(IsOptionItem(pItem->ItemInfo.wIconIdx, pItem->ItemInfo.Durability) == TRUE)
				{
					AddItemOption(&pItem->OptionInfo);
				}
			}
			if(pItem->IsRareOptionItem == TRUE)
			{
				if(IsRareOptionItem(pItem->ItemInfo.wIconIdx, pItem->ItemInfo.RareIdx) == TRUE)
				{
					AddItemRareOption(&pItem->RareOptionInfo);
				}
			}
			GAMEIN->GetGuildWarehouseDlg()->AddItem(&pItem->ItemInfo);
		}
		break;
	case MP_ITEM_PYOGUK_MOVE_ACK:
		{
			MoveItemToPyoguk( (MSG_ITEM_MOVE_ACK*)pMsg );
			WINDOWMGR->DragWindowNull();
		}
		break;
	case MP_ITEM_DISSOLUTION_GETITEM:
		{
			ITEMOBTAINARRAY * pmsg = (ITEMOBTAINARRAY *)pMsg;
			CItem* pItem	= NULL;
			CItem* pPreItem = NULL;
			for( int i = 0 ; i < pmsg->ItemNum ; ++i )
			{
				if( pPreItem = GAMEIN->GetInventoryDialog()->GetItemForPos( pmsg->GetItem(i)->Position ) )
				{
					DeleteItem( pmsg->GetItem(i)->Position, &pPreItem );
					ASSERT( pPreItem );
				}
				pItem = MakeNewItem( pmsg->GetItem(i),"MP_ITEM_DISSOLUTION_GETITEM" );	
				BOOL rt = GAMEIN->GetInventoryDialog()->AddItem( pItem );
				if( !rt )
				{
					ASSERT(0);					
				}
			}		
		}
		break;
	case MP_ITEM_DISSOLUTION_ACK:
		{
			if( GAMEIN->GetDissolutionDialog() )
				GAMEIN->GetDissolutionDialog()->EndDissolution( TRUE );
			CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg( 692 ) );
		}
		break;
	case MP_ITEM_DISSOLUTION_NACK:
		{
			if( GAMEIN->GetDissolutionDialog() )
				GAMEIN->GetDissolutionDialog()->EndDissolution( FALSE );
		}
		break;
	case MP_ITEM_ERROR_NACK:
		{
			SetDisableDialog(FALSE, eItemTable_Inventory);
			SetDisableDialog(FALSE, eItemTable_Pyoguk);
			SetDisableDialog(FALSE, eItemTable_GuildWarehouse);
			SetDisableDialog(FALSE, eItemTable_PetInven);
			WINDOWMGR->BackDragWindow();
			MSG_ITEM_ERROR * pmsg = (MSG_ITEM_ERROR*)pMsg;
			if(pmsg->ECode == eItemUseErr_PreInsert)
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(678));
			else if(pmsg->ECode == eItemUseErr_Discard)
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(799));
			else if(pmsg->ECode == eItemUseErr_AlreadyUse)
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(752) );
			else if(pmsg->ECode == eItemUseErr_Mix)
			{
				if( GAMEIN->GetMixDialog() )
					GAMEIN->GetMixDialog()->SetNowMixing( FALSE );
			}
			else if (pmsg->ECode == eItemUseErr_MixNotPace)
			{
				CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(2259));
			}
			else if(pmsg->ECode == eItemUseErr_TitanPartsMake)
			{
				if( GAMEIN->GetTitanPartsMakeDlg() )
					GAMEIN->GetTitanPartsMakeDlg()->SetNowMixing( FALSE );
			}
		}
		break;
	case MP_ITEM_CHANGEITEM_NACK: 
		{
			MSG_WORD * pmsg = (MSG_WORD*)pMsg;
			CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(354) );
		}
		break;
	case MP_ITEM_USE_CHANGEITEM_ACK:
		{
			ITEMOBTAINARRAY* pmsg = (ITEMOBTAINARRAY *)pMsg;
			CItem* pItem = NULL;
			for(int i = 0 ; i < pmsg->ItemNum ; ++i)
			{
				if( pItem = GAMEIN->GetInventoryDialog()->GetItemForPos( pmsg->GetItem(i)->Position ) )
				{
					pItem->SetItemBaseInfo(pmsg->GetItem(i));
				}
				else
				{
					pItem = MakeNewItem(pmsg->GetItem(i),"MP_ITEM_MONSTER_OBTAIN_NOTIFY");	
					BOOL rt = GAMEIN->GetInventoryDialog()->AddItem(pItem);
					if(!rt)
					{
						ASSERT(0);
					}
				}
			}
			cDialog* pDlg = WINDOWMGR->GetWindowForID( DBOX_DIVIDE_INV );
			if( pDlg )
			{
				((cDivideBox*)pDlg)->ExcuteDBFunc( 0 );	
			}
			if (pItem)
			{
				if (pItem->GetItemKind() & eEQUIP_ITEM || pItem->GetItemKind() == eEQUIP_ITEM_UNIQUE)
				{
					if (pItem->GetQuality() == 4)
					{
						CHATMGR->AddMsg(CTC_GETITEM, CHATMGR->GetChatMsg(2770), pItem->GetItemInfo()->ItemName);
					}
					else if (pItem->GetQuality() == 3)
					{
						CHATMGR->AddMsg(CTC_GETITEM, CHATMGR->GetChatMsg(2769), pItem->GetItemInfo()->ItemName);
					}
					else if (pItem->GetQuality() == 2)
					{
						CHATMGR->AddMsg(CTC_GETITEM, CHATMGR->GetChatMsg(2768), pItem->GetItemInfo()->ItemName);
					}
					else if (pItem->GetQuality() == 1)
					{
						CHATMGR->AddMsg(CTC_GETITEM, CHATMGR->GetChatMsg(2767), pItem->GetItemInfo()->ItemName);
					}
					else if (pItem->GetQuality() == 0)
					{
						CHATMGR->AddMsg(CTC_GETITEM, CHATMGR->GetChatMsg(2766), pItem->GetItemInfo()->ItemName);
					}
				}
				else
				{
					CHATMGR->AddMsg(CTC_GETITEM, CHATMGR->GetChatMsg(121), pItem->GetItemInfo()->ItemName);
				}
				//EFFECTMGR->StartHeroEffectProcess(eEffect_GetItem);
				ItemDropEffect(pItem->GetItemIdx());
			} 

				if(ITEMMGR->IsClearPacketItemCheck(pItem->GetItemIdx()))
				{
					//WINDOWMGR->DestroyWindowProcess();
					ITEMMGR->FakeDeleteItem(pItem);
					ITEMMGR->SendDeleteItem();
					CHATMGR->AddMsg( CTC_KILLED, CHATMGR->GetChatMsg( 2094 ), pItem->GetItemInfo()->ItemName );
					break;
				}


				if (m_SShout)
				{
					SafeStrCpy(ItemObtained, pItem->GetItemInfo()->ItemName, MAX_NAME_LENGTH + 1);
					//ShoutGetItem();
				}
			
		      	QUICKMGR->RefreshQickItem();

		}
		break;
	case MP_ITEM_SHOPITEM_INFO_ACK:
		{
			SEND_SHOPITEM_INFO* pmsg = (SEND_SHOPITEM_INFO*)pMsg;
			if( pmsg->ItemCount )
			{
				GAMEIN->GetItemShopDialog()->SetItemInfo(pmsg);
			}
			else
			{
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(785) );
			}
			GAMEIN->GetItemShopDialog()->SetActive(TRUE);
			GAMEIN->GetInventoryDialog()->SetActive( TRUE );
			GAMEIN->GetInventoryDialog()->OnShopItemBtn();
			GAMEIN->GetInventoryDialog()->SetShopItemInvenBtn( TRUE );			
		}
		break;
	case MP_ITEM_SHOPITEM_USEDINFO:
		{
			SEND_SHOPITEM_USEDINFO* pmsg = (SEND_SHOPITEM_USEDINFO*)pMsg;
			for(int i=0; i<pmsg->ItemCount; i++)
			{
				AddUsedItemInfo(&pmsg->Item[i]);
				if( pmsg->Item[i].Param == eShopItemUseParam_EquipAvatar )
				{
					CItem* pItem = GetItem( pmsg->Item[i].ItemBase.dwDBIdx );
					if(pItem)
						pItem->SetUseParam( 1 );
				}
				if( pmsg->Item[i].ItemBase.wIconIdx == eIncantation_MemoryMoveExtend ||
					pmsg->Item[i].ItemBase.wIconIdx == eIncantation_MemoryMoveExtend7 ||
					pmsg->Item[i].ItemBase.wIconIdx == eIncantation_MemoryMove2 ||
					pmsg->Item[i].ItemBase.wIconIdx == eIncantation_MemoryMoveExtend30 )
					HERO->CalcShopItemOption(pmsg->Item[i].ItemBase.wIconIdx, TRUE);
			}
		}
		break;
	case MP_ITEM_SHOPITEM_MPINFO:
		{
			SEND_MOVEDATA_INFO* pmsg = (SEND_MOVEDATA_INFO*)pMsg;			
			if( pmsg->Count == 0 && pmsg->bInited )
			{
				SetDisableDialog(FALSE, eItemTable_Inventory);
				SetDisableDialog(FALSE, eItemTable_Pyoguk);
				SetDisableDialog(FALSE, eItemTable_MunpaWarehouse);
				SetDisableDialog(FALSE, eItemTable_Shop);
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(794) );
				OBJECTSTATEMGR->EndObjectState( HERO, eObjectState_Deal );
			}
			GAMEIN->GetMoveDialog()->SetMoveInfo( pmsg );
			if( pmsg->bInited )
				GAMEIN->GetMoveDialog()->SetActive( TRUE );
		}
		break;
	case MP_ITEM_SHOPITEM_USEEND:
		{
			MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;
			ITEM_INFO* pInfo = GetItemInfo((WORD)pmsg->dwData );
			if( !pInfo )		return;
			SHOPITEMBASE* pShopItemBase = m_UsedItemList.GetData((WORD)pmsg->dwData);
			if( !pShopItemBase )		return;
			if(pInfo->ItemType ==eSHOP_ITEM_IMAGENAME)
			{
				HERO->SetImageName(0);
			}
			if( pInfo->ItemType == 11 )
			{
				BOOL bfind = FALSE;
				for(int i=0; i<(SLOT_SHOPINVEN_NUM + TABCELL_SHOPINVEN_PLUS_NUM)/*/2*/; i++)
				{
					ITEMBASE* pItemBase = (ITEMBASE*)GetItemInfoAbsIn( HERO, i+TP_SHOPINVEN_START );					
					if( pItemBase && pItemBase->dwDBIdx == pShopItemBase->ItemBase.dwDBIdx )
					{
						CItem* pOutItem = NULL;
						DeleteItem( i+TP_SHOPINVEN_START, &pOutItem );
						if( pOutItem )
						{
							char buf[256] = { 0, };
							sprintf( buf, CHATMGR->GetChatMsg(750), pOutItem->GetItemInfo()->ItemName );
							CHATMGR->AddMsg(CTC_SYSMSG, buf);
						}
						bfind = TRUE;
						break;
					}
				}
				if( !bfind )
				{
					for(int i=0; i<SLOT_WEAR_NUM; i++)
					{
						ITEMBASE* pItemBase = (ITEMBASE*)GetItemInfoAbsIn( HERO, i+TP_WEAR_START );					
						if( pItemBase && pItemBase->dwDBIdx == pShopItemBase->ItemBase.dwDBIdx )
						{
							CItem* pOutItem = NULL;
							DeleteItem( i+TP_WEAR_START, &pOutItem );
							if( pOutItem )
							{
								char buf[256] = { 0, };
								sprintf( buf, CHATMGR->GetChatMsg(750), pOutItem->GetItemInfo()->ItemName );
								CHATMGR->AddMsg(CTC_SYSMSG, buf);
							}
							bfind = TRUE;
							break;
						}
					}
				}
			}
			RemoveUsedItemInfo((WORD)pmsg->dwData, pShopItemBase->ItemBase.dwDBIdx);
			HERO->CalcShopItemOption((WORD)pmsg->dwData, FALSE);
		}
		break;
	case MP_ITEM_SHOPITEM_ONEMINUTE:
		{
			MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;
			STATUSICONDLG->SetOneMinuteToShopItem(pmsg->dwData);
		}
		break;		
	case MP_ITEM_SHOPITEM_USE_ACK:
		{
			SEND_SHOPITEM_BASEINFO* pmsg = (SEND_SHOPITEM_BASEINFO*)pMsg;
			CItem * item = GAMEIN->GetInventoryDialog()->GetItemForPos(pmsg->ShopItemPos);
			if( item )
			{
				eITEM_KINDBIT bits = item->GetItemKind();
				if( bits & eSHOP_ITEM )
				{
					ITEM_INFO* pInfo = GetItemInfo( item->GetItemBaseInfo()->wIconIdx );
					if( bits != eSHOP_ITEM_MAKEUP && bits != eSHOP_ITEM_DECORATION && bits != eSHOP_ITEM_PET && bits != eSHOP_ITEM_IMAGENAME)//图片称号增加
					{
						if( item->GetDurability() > 1)
						{
							GAMEIN->GetInventoryDialog()->UpdateItemDurabilityAdd(pmsg->ShopItemPos,-1);
						}
						else
						{					//化名卷使用				
							if( pInfo && pInfo->ItemType == 10 && pInfo->ItemIdx != eIncantation_ChangeName &&
								pInfo->ItemIdx != eIncantation_ChangeName_Dntrade )
							{
								CItem* pOutItem = NULL;
								DeleteItem( pmsg->ShopItemPos, &pOutItem);
								if( pOutItem )
									ReLinkQuickPosition( pOutItem );
							}
						}
						char buf[256] = { 0, };
						wsprintf( buf, CHATMGR->GetChatMsg(795), item->GetItemInfo()->ItemName );
						CHATMGR->AddMsg( CTC_SYSMSG, buf );
					}
					if( pInfo->ItemType == 10 && pmsg->ShopItemBase.Param )
						AddUsedItemInfo(&pmsg->ShopItemBase);
					HERO->CalcShopItemOption(pmsg->ShopItemIdx, TRUE);
				}
				QUICKMGR->RefreshQickItem();
			}
		}
		break;
	case MP_ITEM_SHOPITEM_USE_NACK:
		{
			MSG_ITEM_ERROR* pmsg = (MSG_ITEM_ERROR*)pMsg;
			SetDisableDialog(FALSE, eItemTable_Inventory);
			SetDisableDialog(FALSE, eItemTable_Pyoguk);
			SetDisableDialog(FALSE, eItemTable_MunpaWarehouse);
			SetDisableDialog(FALSE, eItemTable_Shop);
			if( HERO->GetState() == eObjectState_Deal )			
				OBJECTSTATEMGR->EndObjectState( HERO, eObjectState_Deal );
			switch(pmsg->ECode)
			{
			case eItemUseErr_Lock:
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(751) );
				break;
			case eItemUseErr_AlreadyUse:
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(752) );
				break;
			case eItemUseErr_Unabletime:
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(757) );
				break;
			case eItemUseErr_AlredyChange:
				break;
			case eItemuseErr_DontUseToday:
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1079) );
				break;
			case eItemUseErr_DontDupUse:
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1076) );
				break;
			case eItemUseErr_UseFull:
				WINDOWMGR->MsgBox( MBI_INVEN_FULL_ALERT, MBT_OK, CHATMGR->GetChatMsg(1435) );	
				break;
			}
		}
		break;
	case MP_ITEM_SHOPITEM_MSG:
		{
			MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;
		}
		break;
	case MP_ITEM_SHOPITEM_UNSEAL_ACK:
		{
			MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;
			GAMEIN->GetInventoryDialog()->ItemUnSealing(pmsg->dwData);
			QUICKMGR->RefreshQickItem();
		}
		break;
	case MP_ITEM_SHOPITEM_UNSEAL_NACK:
		{
			MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;
			CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(777) );
		}
		break;
	case MP_ITEM_SHOPITEM_MONEYPROTECT:
		{
			MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;
			CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(753) );
			RemoveUsedItemInfo( (WORD)pmsg->dwData );
		}
		break;
	case MP_ITEM_SHOPITEM_EXPPROTECT:
		{
			MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;
			CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(754) );
			RemoveUsedItemInfo( (WORD)pmsg->dwData );
		}
		break;
	case MP_ITEM_SHOPITEM_MIXUP:
		{
			MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;
			CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(755) );
			RemoveUsedItemInfo( (WORD)pmsg->dwData );
		}
		break;
	case MP_ITEM_SHOPITEM_STATEMINUS:
		{
			MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;
			GAMEIN->GetCharacterDialog()->RefreshPointInfo();
		}
		break;
	case MP_ITEM_SHOPITEM_SAVEPOINT_ADD_ACK:
		{
			SEND_MOVEDATA_SIMPLE* pmsg = (SEND_MOVEDATA_SIMPLE*)pMsg;
			SetDisableDialog(FALSE, eItemTable_Inventory);
			SetDisableDialog(FALSE, eItemTable_Pyoguk);
			SetDisableDialog(FALSE, eItemTable_MunpaWarehouse);
			SetDisableDialog(FALSE, eItemTable_Shop);
			if( HERO->GetState() == eObjectState_Deal )
				OBJECTSTATEMGR->EndObjectState( HERO, eObjectState_Deal );
			GAMEIN->GetMoveDialog()->AddMoveInfo( &pmsg->Data );
			CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(792) );
		}
		break;
	case MP_ITEM_SHOPITEM_SAVEPOINT_ADD_NACK:
		{
			SEND_MOVEDATA_SIMPLE* pmsg = (SEND_MOVEDATA_SIMPLE*)pMsg;
			SetDisableDialog(FALSE, eItemTable_Inventory);
			SetDisableDialog(FALSE, eItemTable_Pyoguk);
			SetDisableDialog(FALSE, eItemTable_MunpaWarehouse);
			SetDisableDialog(FALSE, eItemTable_Shop);
			if( HERO->GetState() == eObjectState_Deal )
				OBJECTSTATEMGR->EndObjectState( HERO, eObjectState_Deal );
			CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(793) );
		}
		break;
	case MP_ITEM_SHOPITEM_SAVEPOINT_UPDATE_ACK:
		{
			SEND_MOVEDATA_SIMPLE* pmsg = (SEND_MOVEDATA_SIMPLE*)pMsg;
			GAMEIN->GetMoveDialog()->UpdateMoveInfo( &pmsg->Data );
		}
		break;
	case MP_ITEM_SHOPITEM_SAVEPOINT_DEL_ACK:
		{
			SEND_MOVEDATA_SIMPLE* pmsg = (SEND_MOVEDATA_SIMPLE*)pMsg;
			GAMEIN->GetMoveDialog()->DelMoveInfo( &pmsg->Data );
		}
		break;
	case MP_ITEM_SHOPITEM_REVIVEOTHER_SYN:
		{
			SEND_REVIVEOTHER* pmsg = (SEND_REVIVEOTHER*)pMsg;
			if( !GAMEIN->GetReviveDialog()->IsActive() )
			{
				MSG_DWORD2 msg;
				msg.Category = MP_ITEM;
				msg.Protocol = MP_ITEM_SHOPITEM_REVIVEOTHER_NACK;
				msg.dwObjectID = HEROID;
				msg.dwData1 = pmsg->TargetID;
				msg.dwData2 = eShopItemErr_Revive_NotReady;			
				NETWORK->Send( &msg, sizeof(msg) );
				return;
			}
			GAMEIN->GetInventoryDialog()->SetReviveData( pmsg->TargetID, 0, 0 );
			char buf[128] = { 0, };
			wsprintf( buf, CHATMGR->GetChatMsg(762), pmsg->TargetName );
			WINDOWMGR->MsgBox( MBI_REVIVECONFIRM, MBT_YESNO, buf );
		}
		break;
	case MP_ITEM_SHOPITEM_REVIVEOTHER_ACK:
		{
			MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;
			SetDisableDialog(FALSE, eItemTable_Inventory);
			SetDisableDialog(FALSE, eItemTable_Pyoguk);
			SetDisableDialog(FALSE, eItemTable_MunpaWarehouse);
			SetDisableDialog(FALSE, eItemTable_Shop);	
			CPlayer* pPlayer = (CPlayer*)OBJECTMGR->GetObject(pmsg->dwData);
			if(!pPlayer)		return;
		}
		break;
	case MP_ITEM_SHOPITEM_REVIVEOTHER_NACK:
		{
			MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;
			SetDisableDialog(FALSE, eItemTable_Inventory);
			SetDisableDialog(FALSE, eItemTable_Pyoguk);
			SetDisableDialog(FALSE, eItemTable_MunpaWarehouse);
			SetDisableDialog(FALSE, eItemTable_Shop);
			switch( pmsg->dwData )
			{
			case eShopItemErr_Revive_Fail:
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(765) );
				break;
			case eShopItemErr_Revive_NotDead:
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(765) );
				break;
			case eShopItemErr_Revive_NotUse:
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(786) );
				break;
			case eShopItemErr_Revive_Refuse:
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(787) );
				break;
			case eShopItemErr_Revive_TooFar:
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(789) );
				break;
			case eShopItemErr_Revive_TimeOver:
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(790) );
				break;
			case eShopItemErr_Revive_NotReady:
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(920) );
				break;
			}
			cDialog* pDlg = WINDOWMGR->GetWindowForID( MBI_REVIVECONFIRM );
			if( pDlg )
				WINDOWMGR->AddListDestroyWindow( pDlg );
		}
		break;
	case MP_ITEM_SHOPITEM_AVATAR_PUTON:
		{
			MSG_DWORD2* pmsg = (MSG_DWORD2*)pMsg;
			CObject* pObject = (CObject*)OBJECTMGR->GetObject(pmsg->dwData1);
			if( pObject && pObject->GetObjectKind() == eObjectKind_Player )
			{
				APPEARANCEMGR->SetAvatarItem( (CPlayer*)pObject, (WORD)pmsg->dwData2 );				
			}
			else
			{
				ASSERT(0);
			}
		}
		break;
	case MP_ITEM_SHOPITEM_AVATAR_TAKEOFF:
		{
			MSG_DWORD2* pmsg = (MSG_DWORD2*)pMsg;
			CObject* pObject = (CObject*)OBJECTMGR->GetObject(pmsg->dwData1);
			if( pObject && pObject->GetObjectKind() == eObjectKind_Player )
			{
				APPEARANCEMGR->SetAvatarItem( (CPlayer*)pObject, (WORD)pmsg->dwData2, FALSE );
			}
			else
			{
				ASSERT(0);
			}
		}
		break;
	case MP_ITEM_SHOPITEM_AVATAR_USE:	
		{
			SEND_SHOPITEM_USEONE* pmsg = (SEND_SHOPITEM_USEONE*)pMsg;
			AddUsedItemInfo(&pmsg->ShopItemBase);
		}
		break;
	case MP_ITEM_SHOPITEM_AVATAR_INFO:
		{
			SEND_AVATARITEM_INFO* pmsg = (SEND_AVATARITEM_INFO*)pMsg;
			CPlayer* pPlayer = (CPlayer*)OBJECTMGR->GetObject(pmsg->PlayerId);
			if(!pPlayer)		return;
			AVATARITEM* pInfo= GAMERESRCMNGR->m_AvatarEquipTable.GetData( pmsg->ItemIdx );
			if(!pInfo)		return;	
			if( pmsg->PlayerId == HERO->GetID() )
				HERO->CheckShopItemUseInfoToInventory( pmsg->ItemIdx, pmsg->ItemPos );
			if( pInfo->Position == eAvatar_Hair )
			{
				int MotionIdx = -1;
				WORD Weapontype = pPlayer->GetWeaponEquipType();
				if( Weapontype==WP_GUM || Weapontype==WP_GWUN || Weapontype==WP_GUNG || Weapontype==WP_AMGI ||
					Weapontype==WP_EVENT || Weapontype == WP_EVENT_HAMMER )
					MotionIdx = eMotion_Item_ChangeHair_1;
				else if( Weapontype==WP_DO )
					MotionIdx = eMotion_Item_ChangeHair_2;
				else if( Weapontype==WP_CHANG )
					MotionIdx = eMotion_Item_ChangeHair_3;
				if( pPlayer->GetID() == HERO->GetID() )
					MOVEMGR->HeroMoveStop();
				if( pPlayer->GetState() == eObjectState_None )
				{
					pPlayer->SetItemUseMotionIdx(MotionIdx);
					pPlayer->GetEngineObject()->ChangeMotion(MotionIdx, FALSE);
				}
				OBJECTSTATEMGR->StartObjectState(pPlayer, eObjectState_ItemUse);
				OBJECTSTATEMGR->EndObjectState(pPlayer, eObjectState_ItemUse, pPlayer->GetEngineObject()->GetAnimationTime(MotionIdx));				
				memcpy( pPlayer->GetShopItemStats()->Avatar, pmsg->Avatar, sizeof(WORD)*eAvatar_Max );
			}
			else
			{
				memcpy( pPlayer->GetShopItemStats()->Avatar, pmsg->Avatar, sizeof(WORD)*eAvatar_Max );
				APPEARANCEMGR->InitAppearance( pPlayer );
			}
			pPlayer->CalcAvatarOption();
			ITEM_INFO* Item = GetItemInfo( pmsg->ItemIdx );
			if(Item->ItemKind==eSHOP_ITEM_IMAGENAME)	//图片称号
			{
				if(pmsg->Avatar[8]==0)
					pPlayer->SetImageNameBalloon(0);
				else
					pPlayer->SetImageNameBalloon(Item->wSetItemKind);
			}
		}
		break;
	case MP_ITEM_SHOPITEM_AVATAR_USE_ACK:
		{
			MSG_WORD2* pmsg = (MSG_WORD2*)pMsg;
			CItem* pItem = GAMEIN->GetInventoryDialog()->GetItemForPos( pmsg->wData2 );
			if( !pItem )		return;
			pItem->SetUseParam( 1 );
			char buf[256] = { 0, };
			wsprintf( buf, CHATMGR->GetChatMsg(779), pItem->GetItemInfo()->ItemName );
			CHATMGR->AddMsg( CTC_SYSMSG, buf );
		}
		break;
	case MP_ITEM_SHOPITEM_AVATAR_USE_ACKDB:
		{
			SEND_SHOPITEM_BASEINFO* pmsg = (SEND_SHOPITEM_BASEINFO*)pMsg;
			CItem* pItem = GAMEIN->GetInventoryDialog()->GetItemForPos( pmsg->ShopItemPos );
			if( !pItem )		return;
			pItem->SetUseParam( 1 );
			char buf[256] = { 0, };
			wsprintf( buf, CHATMGR->GetChatMsg(779), pItem->GetItemInfo()->ItemName );
			CHATMGR->AddMsg( CTC_SYSMSG, buf );
			AddUsedItemInfo( &pmsg->ShopItemBase );
		}
		break;
	case MP_ITEM_SHOPITEM_AVATAR_USE_NACK:
		{
			MSG_WORD2* pmsg = (MSG_WORD2*)pMsg;
			CItem* pItem = GAMEIN->GetInventoryDialog()->GetItemForPos( pmsg->wData2 );
			if( !pItem )		return;
			pItem->SetUseParam( 0 );
			char buf[256] = { 0, };
			wsprintf( buf, CHATMGR->GetChatMsg(780), pItem->GetItemInfo()->ItemName );
			CHATMGR->AddMsg( CTC_SYSMSG, buf );
		}
		break;
	case MP_ITEM_SHOPITEM_AVATAR_DISCARD:
		{
			SEND_AVATARITEM_INFO* pmsg = (SEND_AVATARITEM_INFO*)pMsg;
			CPlayer* pPlayer = (CPlayer*)OBJECTMGR->GetObject(pmsg->PlayerId);
			if(!pPlayer)		return;
			memcpy( pPlayer->GetShopItemStats()->Avatar, pmsg->Avatar, sizeof(WORD)*eAvatar_Max );
			APPEARANCEMGR->InitAppearance( pPlayer );
		}
		break;
	case MP_ITEM_SHOPITEM_EQUIP_USEINFODB:
		{
			SEND_SHOPITEM_BASEINFO* pmsg = (SEND_SHOPITEM_BASEINFO*)pMsg;
			CItem* pItem = GAMEIN->GetInventoryDialog()->GetItemForPos( pmsg->ShopItemPos );
			if( !pItem )
			{
				pItem = GAMEIN->GetPetInventoryDialog()->GetItemForPos( pmsg->ShopItemPos );
			}
			if( !pItem )
				return;
			AddUsedItemInfo( &pmsg->ShopItemBase );
		}
		break;
	case MP_ITEM_SHOPITEM_PET_USE:
		{
			SEND_SHOPITEM_USEONE* pmsg = (SEND_SHOPITEM_USEONE*)pMsg;
			AddUsedItemInfo(&pmsg->ShopItemBase);
		}
		break;
	case MP_ITEM_SHOPITEM_EFFECT:
		{
			MSG_DWORD2* pmsg = (MSG_DWORD2*)pMsg;
			CPlayer* pPlayer = (CPlayer*)OBJECTMGR->GetObject(pmsg->dwData1);
			if(!pPlayer)		return;
			EFFECTMGR->StartPlayerEffectProcess( pPlayer, pmsg->dwData2 );
		}
		break;
	case MP_ITEM_SHOPITEM_SLOTOVERITEM:
		{
			MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;
			if( pmsg->dwData > SLOT_SHOPITEM_IMSI )
			{				
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(796) );
			}
		}
		break;
	case MP_ITEM_SHOPITEM_PROTECTALL:
		{
			MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;
			CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(729) );								
			HERO->GetShopItemStats()->ProtectCount = (char)pmsg->dwData;
		}
		break;
	case MP_ITEM_SHOPITEM_CHASE_ACK:
		{
			SEND_CHASEINFO* pmsg = (SEND_CHASEINFO*)pMsg;
			if( MAP->IsMapSame(eRunningMap) || MAP->GetMapNum() == PKEVENTMAP )
			{
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1457) );
				return;
			}
			VECTOR3 pos;
			pmsg->Pos.Decompress( &pos );
			if( GAMEIN->GetChaseDlg()->InitMiniMap( pmsg->MapNum, (WORD)pos.x, (WORD)pos.z, pmsg->WandtedName, pmsg->EventMapNum ) )
			{
				GAMEIN->GetChaseDlg()->SetActive( TRUE );
			}
			else
			{
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(908) );
			}
		}
		break;
	case MP_ITEM_SHOPITEM_CHASE_NACK:
		{
			MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;
			GAMEIN->GetChaseDlg()->SetActive( FALSE );
			if(pmsg->dwData == 1)
				WINDOWMGR->MsgBox( MBI_CHASE_NACK, MBT_OK, CHATMGR->GetChatMsg(907) );
			else if(pmsg->dwData == 2)
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(908) );
			else if(pmsg->dwData == 3 )
				WINDOWMGR->MsgBox( MBI_CHASE_NACK, MBT_OK, CHATMGR->GetChatMsg(921) );
		}
		break;
	case MP_ITEM_SHOPITEM_CHASE_TRACKING:
		{
		}
		break;
	case MP_ITEM_ADDPOINT_NACK:     
		{
			MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;
			if(pmsg->dwData == 1)
				CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(2255));
			else
				CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(2256));
		}
		break;
	case MP_ITEM_ADDPOINT_ACK:  
		{
			SEND_SHOPITEM_BASEINFO* pmsg = (SEND_SHOPITEM_BASEINFO*)pMsg;
			CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(2257));
			for(int i=0; i<(SLOT_SHOPINVEN_NUM + TABCELL_SHOPINVEN_PLUS_NUM)/*/2*/; i++)
			{
				ITEMBASE* pItemBase = (ITEMBASE*)GetItemInfoAbsIn( HERO, i+TP_SHOPINVEN_START );
				if( !pItemBase )	continue;
				if( pItemBase->dwDBIdx == GAMEIN->GetAdPointDlg()->GetItemDBIdx() )
				{	
					if(pItemBase->Durability>1)
					{
						GAMEIN->GetInventoryDialog()->UpdateItemDurabilityAdd(i+TP_SHOPINVEN_START,-1);
						GAMEIN->GetAdPointDlg()->Zero();
						break;
					}
					CItem* pItem = NULL;
					DeleteItem( i+TP_SHOPINVEN_START, &pItem );
					break;
				}
			}
		}
		break;
	case MP_ITEM_GOLDITEMUSE_NACK:    
		{
			MSGGOLDITEM* pmsg = (MSGGOLDITEM*)pMsg;
			for(int i=0; i<(SLOT_SHOPINVEN_NUM + TABCELL_SHOPINVEN_PLUS_NUM)/*/2*/; i++)
			{
				ITEMBASE* pItemBase = (ITEMBASE*)GetItemInfoAbsIn( HERO, i+TP_SHOPINVEN_START );
				if( !pItemBase )	continue;
				if( pItemBase->dwDBIdx == pmsg->dwItemDBidx )
				{	
					if(pItemBase->Durability>1)
					{
						GAMEIN->GetInventoryDialog()->UpdateItemDurabilityAdd(i+TP_SHOPINVEN_START,-1);
						break;
					}
					CItem* pItem = NULL;
					DeleteItem( i+TP_SHOPINVEN_START, &pItem );
					break;
				}
			}
			CHATMGR->AddMsg( CTC_SYSMSG,CHATMGR->GetChatMsg(2172));
		}
		break;
	case MP_ITEM_GOLDITEMUSE_ACK:    
		{
			MSGGOLDITEM* pmsg = (MSGGOLDITEM*)pMsg;
			for(int i=0; i<(SLOT_SHOPINVEN_NUM + TABCELL_SHOPINVEN_PLUS_NUM)/*/2*/; i++)
			{
				ITEMBASE* pItemBase = (ITEMBASE*)GetItemInfoAbsIn( HERO, i+TP_SHOPINVEN_START );
				if( !pItemBase )	continue;
				if( pItemBase->dwDBIdx == pmsg->dwItemDBidx )
				{	
					if(pItemBase->Durability>1)
					{
						GAMEIN->GetInventoryDialog()->UpdateItemDurabilityAdd(i+TP_SHOPINVEN_START,-1);
						break;
					}
					CItem* pItem = NULL;
					DeleteItem( i+TP_SHOPINVEN_START, &pItem );
					break;
				}
			}
			CHATMGR->AddMsg( CTC_SYSMSG,CHATMGR->GetChatMsg(2248),pmsg->dwGoldMoney );
		}
		break;
	case MP_ITEM_SHOPITEM_NCHANGE_ACK:
		{
			MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;
			if( pmsg->dwObjectID == HEROID )
			{
				HERO->SetName(pmsg->AesText2);
				APPEARANCEMGR->AddCharacterPartChange( HEROID );
			}
			else
			{
				CPlayer* pPlayer = (CPlayer*)OBJECTMGR->GetObject( pmsg->dwObjectID );
				if( !pPlayer )	return;

				pPlayer->SetName(pmsg->AesText2);		
				APPEARANCEMGR->AddCharacterPartChange( pmsg->dwObjectID );
			}
			WINDOWMGR->MsgBox( MBI_CHANGENAME_ACK, MBT_OK, CHATMGR->GetChatMsg(917) );
			for(int i=0; i<(SLOT_SHOPINVEN_NUM + TABCELL_SHOPINVEN_PLUS_NUM)/*/2*/; i++)
			{
				ITEMBASE* pItemBase = (ITEMBASE*)GetItemInfoAbsIn( HERO, i+TP_SHOPINVEN_START );
				if( !pItemBase )	continue;
				if( pItemBase->dwDBIdx == GAMEIN->GetNameChangeDlg()->GetItemDBIdx() )
				{
					CItem* pItem = NULL;
					DeleteItem( i+TP_SHOPINVEN_START, &pItem );
					break;
				}
			}
		}
		break;
	case MP_ITEM_SHOPITEM_NCHANGE_NACK:
		{
			MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;
			switch( pmsg->dwData )
			{
			case 6:
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(912) );
				break;
			case 5:
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(916) );
				break;
			case 4:
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(913) );
				break;
			case 3:
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(914) );
				break;
			case 2:
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(915) );
				break;
			}
		}
		break;
	case MP_ITEM_SHOPITEM_CHARCHANGE_ACK:
		{
			MSGBASE* pmsg = (MSGBASE*)pMsg;
			SetDisableDialog(FALSE, eItemTable_Inventory);
			SetDisableDialog(FALSE, eItemTable_Pyoguk);
			SetDisableDialog(FALSE, eItemTable_MunpaWarehouse);
			SetDisableDialog(FALSE, eItemTable_Shop);
			OBJECTSTATEMGR->EndObjectState( HERO, eObjectState_Deal );
			GAMEIN->GetCharChangeDlg()->Reset( TRUE );
			GAMEIN->GetCharChangeDlg()->SetActive( FALSE );
		}
		break;
	case MP_ITEM_SHOPITEM_CHARCHANGE_NACK:
		{
			MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;
			switch( pmsg->dwData )
			{
			case 1:
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(786) );
				break;
			case 2:
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1185) );
				break;
			case 3:
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1185) );
				break;
			case 4:
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1185) );
				break;
			case 5:				
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(786) );
				break;
			case 6:
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1184) );
				break;
			}
			SetDisableDialog(FALSE, eItemTable_Inventory);
			SetDisableDialog(FALSE, eItemTable_Pyoguk);
			SetDisableDialog(FALSE, eItemTable_MunpaWarehouse);
			SetDisableDialog(FALSE, eItemTable_Shop);
			OBJECTSTATEMGR->EndObjectState( HERO, eObjectState_Deal );
			GAMEIN->GetCharChangeDlg()->Reset( FALSE );
			GAMEIN->GetCharChangeDlg()->SetActive( FALSE );
		}
		break;
	case MP_ITEM_SHOPITEM_CHARCHANGE:
		{
			SEND_CHARACTERCHANGE_INFO* pmsg = (SEND_CHARACTERCHANGE_INFO*)pMsg;
			CPlayer* pPlayer = (CPlayer*)OBJECTMGR->GetObject( pmsg->Param );
			if(!pPlayer)		return;
			pPlayer->SetCharChangeInfo( &pmsg->Info );
		}
		break;
	case MP_ITEM_SHOPITEM_SEAL_ACK:
		{
			MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;
			if( GAMEIN->GetSealDlg() )
				GAMEIN->GetSealDlg()->ItemSealAck();
		}
		break;
	case MP_ITEM_SHOPITEM_SEAL_NACK:
		{
			MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;
			if( GAMEIN->GetSealDlg() )
				GAMEIN->GetSealDlg()->ItemSealNAck();
		}
		break;
	case MP_ITEM_SHOPITEM_JOBCHANGE_ACK:
		{
			MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;
			WINDOWMGR->MsgBox( MBI_JOBCHANGE_ACK, MBT_OK, CHATMGR->GetChatMsg(1143) );
			CChangeJobDialog* pDlg = GAMEIN->GetChangeJobDlg();
			if( !pDlg )		return;
			for(int i=0; i<(SLOT_SHOPINVEN_NUM + TABCELL_SHOPINVEN_PLUS_NUM)/*/2*/; i++)
			{
				ITEMBASE* pItemBase = (ITEMBASE*)GetItemInfoAbsIn( HERO, i+TP_SHOPINVEN_START );
				if( !pItemBase )	continue;
				if( pItemBase->dwDBIdx == pDlg->GetItemDBIdx() && pItemBase->Position == pDlg->GetItemPos() )
				{
					CItem* pItem = NULL;
					DeleteItem( i+TP_SHOPINVEN_START, &pItem );
					break;
				}
			}
		}
		break;
	case MP_ITEM_SHOPITEM_JOBCHANGE_NACK:
		{
			MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;
			switch( pmsg->dwData )
			{
			case 1:			
				break;
			case 2:			
				break;
			}
			CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(786) );
			GAMEIN->GetChangeJobDlg()->CancelChangeJob();
		}
		break;
	case MP_ITEM_SHOPITEM_REINFORCERESET_ACK:
		{
			MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;
			if( GAMEIN->GetReinforceResetDlg() )
				GAMEIN->GetReinforceResetDlg()->ItemResetAck();
		}
		break;
	case MP_ITEM_SHOPITEM_REINFORCERESET_NACK:
		{
			MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;
			if( GAMEIN->GetReinforceResetDlg() )
				GAMEIN->GetReinforceResetDlg()->ItemResetNAck();
			CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1233) );
		}
		break;
	case MP_ITEM_SHOPITEM_GRADECHANGE_ACK: // 武器升阶值转移卷
	{
		// 将 pMsg 强制转换为对应的协议结构
		SEND_SHOPITEM_BASEINFO1* pRecvMsg = (SEND_SHOPITEM_BASEINFO1*)pMsg;

		// 根据物品位置获取对应的物品对象
		CItem* MainItem = GAMEIN->GetInventoryDialog()->GetItemForPos(pRecvMsg->ShopItemPosFrom);
		CItem* MainItem2 = GAMEIN->GetInventoryDialog()->GetItemForPos(pRecvMsg->ShopItemPosTo);

		// 检查物品是否有效
		if (MainItem && MainItem2)
		{
			// 使用服务端传来的等级信息
			MainItem->SetGrade(pRecvMsg->GradeFrom); // 设置源物品等级
			MainItem2->SetGrade(pRecvMsg->GradeTo); // 设置目标物品等级

			// 刷新工具提示
			ITEMMGR->RefreshItemToolTip(MainItem->GetDBIdx());
			ITEMMGR->RefreshItemToolTip(MainItem2->GetDBIdx());

			// 刷新快速物品栏
			QUICKMGR->RefreshQickItem();
		}

		// 添加系统消息
		CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(2639)); // 显示系统消息ID 2639 的内容

		break;
	}
	break;
	case MP_ITEM_SHOPITEM_GRADECHANGE_NACK: // 武器升阶值转移卷
	{
		MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;
		if (pmsg->dwData == 5) // 转换物品类型错误
		{
			CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(2640));
		}
		else if (pmsg->dwData == 7) // 等级不足错误
		{
			CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(2642)); // 新增错误消息编号 2642
		}
		else // 其他错误
		{
			CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(2641));
		}
	}
	break;
	case MP_ITEM_SHOPITEM_RARECREATE_ACK:
		{
			MSG_ITEM_RAREITEM_GET* pmsg = (MSG_ITEM_RAREITEM_GET*)pMsg;
			CItem* pItem = NULL;
			pItem = GAMEIN->GetInventoryDialog()->GetItemForPos(pmsg->RareItemBase.Position);
			if( !pItem )
			{
				GAMEIN->GetRareCreateDlg()->ItemRareCreateNAck();
				return;
			}
			ITEMBASE* pItemBase = (ITEMBASE*)pItem->GetItemBaseInfo();
			if( !pItemBase )
			{
				GAMEIN->GetRareCreateDlg()->ItemRareCreateNAck();
				return;
			}
			if( pItemBase->RareIdx )
				RemoveItemRareOption( pItem->GetItemBaseInfo()->RareIdx );			
			pItemBase->RareIdx = pmsg->RareInfo.dwRareOptionIdx;
			AddItemRareOption(&pmsg->RareInfo);
			GAMEIN->GetRareCreateDlg()->ItemRareCreateAck();
			QUICKMGR->RefreshQickItem();
			CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1235) );
		}
		break;
	case MP_ITEM_SHOPITEM_RARECREATE_NACK:
		{
			MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;
			GAMEIN->GetRareCreateDlg()->ItemRareCreateNAck();
			CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1236) );
		}
		break;
	case MP_ITEM_SHOPITEM_SHOUT_SENDACK:
		{
			SEND_SHOUTRECEIVE* pmsg = (SEND_SHOUTRECEIVE*)pMsg;
		}
		break;
	case MP_ITEM_SHOPITEM_SHOUT_NACK:
		{
		}
		break;
#ifdef TAIWAN_LOCAL
	case MP_ITEM_EVENTITEM_USE:
		{
			if( pmsg->dwData2 == 53074 )
			{
				EFFECTMGR->StartPlayerEffectProcess( pPlayer, FindEffectNum("m_ba_079.beff") );
			}
			if( pmsg->dwData2 == EVENT_ITEM_FIRECRACKER )
			{
				EFFECTMGR->StartPlayerEffectProcess( pPlayer, EVENT_EFFECT_FIRECRACKER );
			}
			if( pmsg->dwData2 = 53151 )
			{
				EFFECTMGR->StartPlayerEffectProcess( pPlayer, FindEffectNum("m_ba_082.beff") );
			}
		}
		break;
#else
	case MP_ITEM_EVENTITEM_USE:
		{
			MSG_DWORD2* pmsg = (MSG_DWORD2*)pMsg;
			CPlayer* pPlayer = (CPlayer*)OBJECTMGR->GetObject(pmsg->dwData1);
			if( pmsg->dwData2 == EVENT_ITEM_FIRECRACKER )
			{
				EFFECTMGR->StartPlayerEffectProcess( pPlayer, EVENT_EFFECT_FIRECRACKER );
			}
			if( pmsg->dwData2 == 53151 )
			{
				EFFECTMGR->StartPlayerEffectProcess( pPlayer, FindEffectNum("m_ba_082.beff") );
			}
		}
		break;
#endif
	case MP_ITEM_FULLINVEN_ALERT:
		{
			WINDOWMGR->MsgBox( MBI_INVEN_FULL_ALERT, MBT_OK, CHATMGR->GetChatMsg(1435) );	
		}
		break;
	case MP_ITEM_TITAN_DISCARD_EQUIPITEM:
		{
			MSG_ITEM_DISCARD_ACK* pmsg = (MSG_ITEM_DISCARD_ACK*)pMsg;
			WORD wTableIdx = GetTableIdxForAbsPos(pmsg->TargetPos);
			DeleteItemofTable(wTableIdx, pmsg->TargetPos);
			SetDisableDialog(FALSE, eItemTable_Inventory);
			SetDisableDialog(FALSE, eItemTable_Pyoguk);
			SetDisableDialog(FALSE, eItemTable_GuildWarehouse);
			SetDisableDialog(FALSE, eItemTable_PetInven);
			QUICKMGR->RefreshQickItem();
		}
		break;
	case MP_ITEM_TITAN_EQUIP_REPAIR_ACK:
		{
			MSG_TITAN_REPAIR_EQUIPITEM_SYN* pmsg = (MSG_TITAN_REPAIR_EQUIPITEM_SYN*)pMsg;
			TITAN_ENDURANCE_ITEMINFO* pEndurance = TITANMGR->GetTitanEnduranceInfo(pmsg->RepairInfo.ItemDBIdx);
			if( !pEndurance )
			{
				return;
			}
			CopyMemory( pEndurance, &pmsg->RepairInfo, sizeof(TITAN_ENDURANCE_ITEMINFO) );
			RefreshItemToolTip(pmsg->RepairInfo.ItemDBIdx);
			TITANMGR->SetTitanEnduranceView(pmsg->RepairInfo.ItemDBIdx);
			CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(1540));
		}
		break;
	case MP_ITEM_TITAN_EQUIP_REPAIR_NACK:
		{
			MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;
			if(pmsg->dwData == 0)
			{
				CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(1579));
			}
			else if(pmsg->dwData == 1)
			{
				CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(1541));
			}
			else if(pmsg->dwData == 2)
			{
				CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(1582));
			}
		}
		break;
	case MP_ITEM_TITAN_EQUIP_REPAIR_TOTAL_ACK:
		{
			MSG_TITAN_REPAIR_TOTAL_EQUIPITEM_ACK* pmsg = (MSG_TITAN_REPAIR_TOTAL_EQUIPITEM_ACK*)pMsg;
			for( int i = 0; i < pmsg->wRepairNum; i++ )
			{
				TITAN_ENDURANCE_ITEMINFO* pEndurance = TITANMGR->GetTitanEnduranceInfo(pmsg->RepairInfo[i].ItemDBIdx);
				CopyMemory(pEndurance, &pmsg->RepairInfo[i], sizeof(TITAN_ENDURANCE_ITEMINFO));
				RefreshItemToolTip(pmsg->RepairInfo[i].ItemDBIdx);
				TITANMGR->SetTitanEnduranceView(pmsg->RepairInfo[i].ItemDBIdx);
			}
			CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(1542));
		}
		break;
	case MP_ITEM_LUCKER_ACK:   
		{
            MSG_DWORD * pmsg=(MSG_DWORD*)pMsg;
			CLuckerDialog* pHandler = (CLuckerDialog*)GAMEIN->GetLuckerDialog();
			cDialog * pDlg= WINDOWMGR->GetWindowForID(LUCKERDIALOG);
			if(pDlg)
			{
	          //  cButton * m_pStart = (cButton*)pDlg->GetWindowForID( LUCKER_BTN_START );
				//m_pStart->SetActive(TRUE);
				pHandler->SetGridSelect(pmsg->dwData-1);
			}
			pHandler->EndProcess();
		}
		break;
	case MP_ITEM_LUCKER_NACK:
		{
			MSG_DWORD * pmsg=(MSG_DWORD*)pMsg;
			if( pmsg->dwData == 1 )
			{
				CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(2755));
			}
			else if( pmsg->dwData == 2 )
			{
				CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(2754));
			}
			else if( pmsg->dwData == 3 )
			{
				CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(2756));
			}
			else if (pmsg->dwData == 4)
			{
				CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(2757));
			}
			CLuckerDialog* pHandler = (CLuckerDialog*)GAMEIN->GetLuckerDialog();
			cDialog * pDlg= WINDOWMGR->GetWindowForID(LUCKERDIALOG);
			pHandler->EndProcess();
		}
		break;
	case MP_ITEM_QUERY_EQUI_ACK:
		{
			MSG_QUERY_EQUIPINF* pmsg = (MSG_QUERY_EQUIPINF*)pMsg;

			for (int i=0;i<eWearedItem_Max;i++)
			{
				if (HERO->m_OtherPlayerEquip[i] != NULL)
				{
					GAMEIN->GetCharacterEquipInfoDlg()->DeleteItem( &HERO->m_OtherPlayerEquip[i]);
					ITEMMGR->ItemDelete(HERO->m_OtherPlayerEquip[i]);
					HERO->m_OtherPlayerEquip[i]=NULL;
				}

				CItem* pItem = MakeNewItem(&pmsg->WearedItem[i],"MP_ITEM_QUERY_EQUI_ACK");
				GAMEIN->GetCharacterEquipInfoDlg()->AddItem(pItem);
				if (!pItem)
				{
					continue;
				}

				ITEMMGR->AddItemRareOption(&pmsg->WearedItemRateInfo[i]);
				ITEMMGR->AddItemOption(&pmsg->WearedItemOptionInfo[i]);
				ITEMMGR->SetToolTipIcon(pItem,GetItemOption(pItem->GetDurability()), GetItemRareOption(pItem->GetRareness()),pItem->GetItemBaseInfo()->dwDBIdx,GetItemStoneOption(pItem->GetStoneIdx()), TRUE,0,0,0, pItem->GetGrade());
				HERO->m_OtherPlayerEquip[i] = pItem;
			}

			GAMEIN->GetCharacterEquipInfoDlg()->SetActive(TRUE);
		}
		break;
	case MP_ITEM_QUERY_CHARACTERMONEY_ACK:
		{
            MSG_DWORD3* pmsg = (MSG_DWORD3*)pMsg;
			COtherCharacerMoneyDlg * pDlg = GAMEIN->GetOtherCharacterMoneyDlg();
			if(pDlg)
			{
				pDlg->SetMoney(pmsg->dwData1);
				pDlg->SetMallMoney(pmsg->dwData2);
				pDlg->SetGoldMoney(pmsg->dwData3);
                pDlg->SetActive(TRUE);
			}
		}
		break;
	 case MP_ITEM_QUERY_CHARACTERAMULET_ACK:
		{
            SEND_SHOPITEM_USEDINFO* pmsg = (SEND_SHOPITEM_USEDINFO*)pMsg;
			COtherCharacterAmuletDlg * pDlg = GAMEIN->GetOtherCharacterAmuletDlg();
			if(pDlg)
			{
				pDlg->ClearAmuletInfo();
				for(int i=0; i<pmsg->ItemCount; i++)
				{
					pDlg->SetAmuletInfo(i,&pmsg->Item[i]);
				}
			}
			pDlg->SetActive(TRUE);
		}
		break;
	 case MP_ITEM_STONE_DK_ACK:  
		 {
			 MSG_ITEM_STONE_DATA* pmsg=(MSG_ITEM_STONE_DATA*)pMsg;
			 AddItemStoneOption(&pmsg->StoneInfo);
			 CItem * pStoneItem=GAMEIN->GetItemStoneDkDlg()->GetItemStone();
			if(pStoneItem)
			{
				ITEM_INFO* pStoneInfo = ITEMMGR->GetItemInfo(pStoneItem->GetItemIdx());
				MSG_ITEM_USE_SYN msg;
				msg.Category = MP_ITEM;
				msg.Protocol = MP_ITEM_USE_SYN;
				msg.dwObjectID = HEROID;
				msg.TargetPos = pStoneItem->GetPosition();
				msg.wItemIdx =  pStoneItem->GetItemIdx();
				NETWORK->Send(&msg,sizeof(msg));
				CHATMGR->AddMsg(CTC_SYSMSG,CHATMGR->GetChatMsg(1915),pStoneInfo->ItemName);  
			}
			CItem * pTargetItem = NULL;
			pTargetItem = GAMEIN->GetInventoryDialog()->GetItemForPos(pmsg->TargetPos);
			if( pTargetItem )
			{
				pTargetItem->SetStoneIdx(pmsg->StoneInfo.dwStoneOptionIdx);
				if( pTargetItem->GetDurability() != 0 && !IsDupItem(pTargetItem->GetItemIdx()) )
				{
					SetToolTipIcon( pTargetItem, ITEMMGR->GetItemOption(pTargetItem->GetDurability()),ITEMMGR->GetItemRareOption(pTargetItem->GetRareness()),0,ITEMMGR->GetItemStoneOption(pTargetItem->GetStoneIdx()) ); 
				}
				else
				{
			        SetToolTipIcon( pTargetItem, NULL,ITEMMGR->GetItemRareOption(pTargetItem->GetRareness()),0,ITEMMGR->GetItemStoneOption(pTargetItem->GetStoneIdx()) ); 
				}
				pTargetItem->SetActive(TRUE);
			}
			GAMEIN->GetItemStoneDkDlg()->Release();
			CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1913) );
		 }
		 break;
		 case MP_ITEM_STONE_DK_NACK:  
		 {
			MSG_DWORD4 * pmsg=(MSG_DWORD4*)pMsg;
			CItem * pStoneItem=GAMEIN->GetItemStoneDkDlg()->GetItemStone();
			if(pStoneItem)
			{
				if(pmsg->dwData1==0)  
				{		
					ITEM_INFO* pStoneInfo = ITEMMGR->GetItemInfo(pStoneItem->GetItemIdx());
					MSG_ITEM_USE_SYN msg;
					msg.Category = MP_ITEM;
					msg.Protocol = MP_ITEM_USE_SYN;
					msg.dwObjectID = HEROID;
					msg.TargetPos = pStoneItem->GetPosition();
					msg.wItemIdx =  pStoneItem->GetItemIdx();
					NETWORK->Send(&msg,sizeof(msg));
					CHATMGR->AddMsg(CTC_SYSMSG,CHATMGR->GetChatMsg(1915),pStoneInfo->ItemName);  
				}
			}
			CItem * pTargetItem = NULL;
			pTargetItem = GAMEIN->GetInventoryDialog()->GetItemForPos(pmsg->dwData2);
			if( pTargetItem )
			{
				pTargetItem->SetActive(TRUE);
			}
			GAMEIN->GetItemStoneDkDlg()->Release();
			CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1914) );
		 }
		 break;
		 case MP_ITEM_STONE_XQ_ACK:  
		 {
			 MSG_ITEM_STONE_DATA* pmsg=(MSG_ITEM_STONE_DATA*)pMsg;
			 AddItemStoneOption(&pmsg->StoneInfo);
			 CItem * pStoneItem=GAMEIN->GetItemStoneXqDlg()->GetXqItem();
			if(pStoneItem)
			{
				ITEM_INFO* pStoneInfo = ITEMMGR->GetItemInfo(pStoneItem->GetItemIdx());
				MSG_ITEM_USE_SYN msg;
				msg.Category = MP_ITEM;
				msg.Protocol = MP_ITEM_USE_SYN;
				msg.dwObjectID = HEROID;
				msg.TargetPos = pStoneItem->GetPosition();
				msg.wItemIdx =  pStoneItem->GetItemIdx();
				NETWORK->Send(&msg,sizeof(msg));
				CHATMGR->AddMsg(CTC_SYSMSG,CHATMGR->GetChatMsg(2115),pStoneInfo->ItemName);  
			}
			GAMEIN->GetItemStoneXqDlg()->DeleteXqStoneAll();  
			CItem * pTargetItem = NULL;
			pTargetItem = GAMEIN->GetInventoryDialog()->GetItemForPos(pmsg->TargetPos);
			if( pTargetItem )
			{
				pTargetItem->SetStoneIdx(pmsg->StoneInfo.dwStoneOptionIdx);
				if( pTargetItem->GetDurability() != 0 && !IsDupItem(pTargetItem->GetItemIdx()) )
				{
					SetToolTipIcon( pTargetItem, ITEMMGR->GetItemOption(pTargetItem->GetDurability()),ITEMMGR->GetItemRareOption(pTargetItem->GetRareness()),0,ITEMMGR->GetItemStoneOption(pTargetItem->GetStoneIdx()) ); 
				}
				else
				{
			        SetToolTipIcon( pTargetItem, NULL,ITEMMGR->GetItemRareOption(pTargetItem->GetRareness()),0,ITEMMGR->GetItemStoneOption(pTargetItem->GetStoneIdx()) ); 
				}
				pTargetItem->SetActive(TRUE);
			}
			GAMEIN->GetItemStoneXqDlg()->Release();
			CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1924) );
		 }
		 break;
		 case MP_ITEM_STONE_XQ_NACK:  
		 {
			MSG_DWORD6 * pmsg=(MSG_DWORD6*)pMsg;
			CItem * pStoneItem=GAMEIN->GetItemStoneXqDlg()->GetXqItem();
			pStoneItem->SetLock(FALSE);
			CItem * pTargetItem = NULL;
			pTargetItem = GAMEIN->GetInventoryDialog()->GetItemForPos(pmsg->dwData5);
			if( pTargetItem )
			{
				pTargetItem->SetActive(TRUE);
			}
			GAMEIN->GetItemStoneXqDlg()->Release();
			CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(2116) );
		 }
		 break;

	 case MP_ITEM_LOCKEX_ACK:
		 {
             MSG_DWORD2* pmsg=(MSG_DWORD2*)pMsg;
			 CItem * pStoneItem=GAMEIN->GetItemLockDialog()->GetLockItem();
			if(pStoneItem)
			{
				ITEM_INFO* pStoneInfo = ITEMMGR->GetItemInfo(pStoneItem->GetItemIdx());
				MSG_ITEM_USE_SYN msg;
				msg.Category = MP_ITEM;
				msg.Protocol = MP_ITEM_USE_SYN;
				msg.dwObjectID = HEROID;
				msg.TargetPos = pStoneItem->GetPosition();
				msg.wItemIdx =  pStoneItem->GetItemIdx();
				NETWORK->Send(&msg,sizeof(msg));
				CHATMGR->AddMsg(CTC_SYSMSG,CHATMGR->GetChatMsg(2118),pStoneInfo->ItemName);  
			}
			CItem * pTargetItem = NULL;
			pTargetItem = GAMEIN->GetInventoryDialog()->GetItemForPos(pmsg->dwData2);
			if( pTargetItem )
			{
				pTargetItem->SetStatic(3);  
				pTargetItem->SetActive(TRUE);
			}
			GAMEIN->GetItemLockDialog()->Release();  
			CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(2119) );
		 }
		 break;
	 case MP_ITEM_LOCKEX_NACK:  
		 {
             MSG_DWORD4* pmsg=(MSG_DWORD4*)pMsg;
			 CItem * pStoneItem=GAMEIN->GetItemLockDialog()->GetLockItem();
			if(pStoneItem)
			{
				ITEM_INFO* pStoneInfo = ITEMMGR->GetItemInfo(pStoneItem->GetItemIdx());
				MSG_ITEM_USE_SYN msg;
				msg.Category = MP_ITEM;
				msg.Protocol = MP_ITEM_USE_SYN;
				msg.dwObjectID = HEROID;
				msg.TargetPos = pStoneItem->GetPosition();
				msg.wItemIdx =  pStoneItem->GetItemIdx();
				NETWORK->Send(&msg,sizeof(msg));
				CHATMGR->AddMsg(CTC_SYSMSG,CHATMGR->GetChatMsg(2118),pStoneInfo->ItemName);  
			}
			CItem * pTargetItem = NULL;
			pTargetItem = GAMEIN->GetInventoryDialog()->GetItemForPos(pmsg->dwData2);
			if( pTargetItem )
			{
				pTargetItem->SetStatic(0);  
				pTargetItem->SetActive(TRUE);
			}
			GAMEIN->GetItemLockDialog()->Release();  
			CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(2120) );
		 }
		 break;
	 case MP_ITEM_UNLOCKEX_ACK:
		 {
             MSG_DWORD2* pmsg=(MSG_DWORD2*)pMsg;
			 CItem * pStoneItem=GAMEIN->GetItemUnLockDialog()->GetLockItem();
			if(pStoneItem)
			{
				ITEM_INFO* pStoneInfo = ITEMMGR->GetItemInfo(pStoneItem->GetItemIdx());
				MSG_ITEM_USE_SYN msg;
				msg.Category = MP_ITEM;
				msg.Protocol = MP_ITEM_USE_SYN;
				msg.dwObjectID = HEROID;
				msg.TargetPos = pStoneItem->GetPosition();
				msg.wItemIdx =  pStoneItem->GetItemIdx();
				NETWORK->Send(&msg,sizeof(msg));
				CHATMGR->AddMsg(CTC_SYSMSG,CHATMGR->GetChatMsg(2118),pStoneInfo->ItemName);  
			}
			CItem * pTargetItem = NULL;
			pTargetItem = GAMEIN->GetInventoryDialog()->GetItemForPos(pmsg->dwData2);
			if( pTargetItem )
			{
				pTargetItem->SetStatic(0);  
				pTargetItem->SetActive(TRUE);
			}
			GAMEIN->GetItemUnLockDialog()->Release();  
			CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(2122) );
		 }
		 break;
	 case MP_ITEM_UNLOCKEX_NACK:  
		 {
             MSG_DWORD4* pmsg=(MSG_DWORD4*)pMsg;
			 CItem * pStoneItem=GAMEIN->GetItemUnLockDialog()->GetLockItem();
			if(pStoneItem)
			{
				ITEM_INFO* pStoneInfo = ITEMMGR->GetItemInfo(pStoneItem->GetItemIdx());
				MSG_ITEM_USE_SYN msg;
				msg.Category = MP_ITEM;
				msg.Protocol = MP_ITEM_USE_SYN;
				msg.dwObjectID = HEROID;
				msg.TargetPos = pStoneItem->GetPosition();
				msg.wItemIdx =  pStoneItem->GetItemIdx();
				NETWORK->Send(&msg,sizeof(msg));
				CHATMGR->AddMsg(CTC_SYSMSG,CHATMGR->GetChatMsg(2118),pStoneInfo->ItemName);  
			}
			CItem * pTargetItem = NULL;
			pTargetItem = GAMEIN->GetInventoryDialog()->GetItemForPos(pmsg->dwData2);
			if( pTargetItem )
			{
				pTargetItem->SetStatic(3);  
				pTargetItem->SetActive(TRUE);
			}
			GAMEIN->GetItemUnLockDialog()->Release();  
			CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(2123) );
		 }
		 break;
	 case MP_ITEM_GROW_ACK:     
		 {
            MSG_DWORD3* pmsg=(MSG_DWORD3*)pMsg;
            CItem * pTargetItem = GAMEIN->GetInventoryDialog()->GetItemForPos(pmsg->dwData2);
			if(pTargetItem)
			{
				pTargetItem->SetGrow(pmsg->dwData3);
				ITEMMGR->RefreshItem( pTargetItem);
			    STATSMGR->CalcItemStats(HERO);
			    GAMEIN->GetCharacterDialog()->SetAttackRate();
			    GAMEIN->GetCharacterDialog()->SetDefenseRate();
			    GAMEIN->GetCharacterDialog()->SetGenGol();
			    GAMEIN->GetCharacterDialog()->SetMinChub();
			    GAMEIN->GetCharacterDialog()->SetCheRyuk();
			    GAMEIN->GetCharacterDialog()->SetSimMek();
			    GAMEIN->GetCharacterDialog()->SetCritical();
			    GAMEIN->GetCharacterDialog()->UpdateData();
				APPEARANCEMGR->InitAppearance( HERO );
				CHATMGR->AddMsg(CTC_SYSMSG,CHATMGR->GetChatMsg(2127),pmsg->dwData3);  
				if( pTargetItem->GetDurability() != 0 && !IsDupItem(pTargetItem->GetItemIdx()) )
				{
					SetToolTipIcon( pTargetItem, ITEMMGR->GetItemOption(pTargetItem->GetDurability()),ITEMMGR->GetItemRareOption(pTargetItem->GetRareness()),0,ITEMMGR->GetItemStoneOption(pTargetItem->GetStoneIdx()) ); 
				}
				else
				{
			        SetToolTipIcon( pTargetItem, NULL,ITEMMGR->GetItemRareOption(pTargetItem->GetRareness()),0,ITEMMGR->GetItemStoneOption(pTargetItem->GetStoneIdx()) ); 
				}
			}
		 }
		 break;
	case MP_ITEM_GAMBLE_PUBLIC_ACK: 
		{
			CItemGambleDialog * Dlg =GAMEIN->GetItemGambleDialog();
			if(Dlg)
			{
				MSG_DWORD3 * pmsg =(MSG_DWORD3 *)pMsg;
				int * Number =new int[3];
				Number[0]=pmsg->dwData1;
				Number[1]=pmsg->dwData2;
				Number[2]=pmsg->dwData3;
				Dlg->SetGamblePublic(Number);
			}
		  }
		  break;
	 case MP_ITEM_GAMBLE_SEND_ACK:
		 {
			 MSG_DWORD3* pmsg=(MSG_DWORD3*)pMsg;
			 CItemGambleDialog *pDlg = (CItemGambleDialog*)GAMEIN->GetItemGambleDialog();
             if(pDlg)
			 {
				int * Number =new int[3];
				Number[0]=pmsg->dwData1;
				Number[1]=pmsg->dwData2;
				Number[2]=pmsg->dwData3;
				pDlg->SetGamblePrivate(Number);
				CHATMGR->AddMsg(CTC_SYSMSG,CHATMGR->GetChatMsg(2130),pmsg->dwData1,pmsg->dwData2,pmsg->dwData3);  
			 }
		 }
		 break;
	 case MP_ITEM_GAMBLE_MAP_ACK:
		 {
			 MSG_DWORD3* pmsg=(MSG_DWORD3*)pMsg;
			 CItemGambleDialog *pDlg = (CItemGambleDialog*)GAMEIN->GetItemGambleDialog();
             if(pDlg)
			 {
				int * Number =new int[3];
				Number[0]=pmsg->dwData1;
				Number[1]=pmsg->dwData2;
				Number[2]=pmsg->dwData3;
				pDlg->SetGamblePrivate(Number);
			 }
		 }
		 break;
	 case MP_ITEM_GAMBLE_OPEN_ACK:
		 {
            MSG_DWORD2 * pmsg=(MSG_DWORD2*)pMsg;
			CHATMGR->AddMsg(CTC_SYSMSG,CHATMGR->GetChatMsg(2135),pmsg->dwData1,pmsg->dwData2); 
		 }
         break;
	 case MP_ITEM_GAMBLE_SEND_NACK:
		 {
              MSG_DWORD* pmsg=(MSG_DWORD*)pMsg;
			  if(pmsg->dwData==0)
			  {
                  CHATMGR->AddMsg(CTC_SYSMSG,CHATMGR->GetChatMsg(2131));   
			  }
			  if(pmsg->dwData==1)
			  {
				  CHATMGR->AddMsg(CTC_SYSMSG,CHATMGR->GetChatMsg(2133));   
			  }
			  if(pmsg->dwData==2)
			  {
                  CHATMGR->AddMsg(CTC_SYSMSG,CHATMGR->GetChatMsg(2132));   
			  }
			  if(pmsg->dwData==3)
			  {
                  CHATMGR->AddMsg(CTC_SYSMSG,CHATMGR->GetChatMsg(2134));   
			  }
			  if(pmsg->dwData==4)
			  {
                  CHATMGR->AddMsg(CTC_SYSMSG,CHATMGR->GetChatMsg(2132));   
			  }
		 }
		  break;
	 case MP_ITEM_GAMECHECK_ACK:  
		 {
			 MSG_GAMECHECK_LIST * pmsg =(MSG_GAMECHECK_LIST*)pMsg;
			 SetGameCheckList(pmsg);
		 }
		 break;
	case MP_ITEM_SAFE_CHECK_ACK:  
		{
            MSG_WORD * pmsg = (MSG_WORD*)pMsg;
			CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(2590));
			if(HERO)
			{
				HERO->SetSafeStatic(1);  
			}
			//GAMEIN->GetSafeNumberDialog()->SetActive(FALSE);
			//GAMEIN->GetSafeNumberLockDialog()->SetActive(FALSE);
            //GAMEIN->GetSafeNumberIcon()->SetSafeIconShow();
			GAMEIN->GetSafeNumberIcon()->ButtonLong(2);
		}
		break;
	 case MP_ITEM_SAFE_CHECK_NACK:  
		{
            MSG_WORD * pmsg = (MSG_WORD*)pMsg;
            CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(2592));
			GAMEIN->GetSafeNumberIcon()->SetLock();
            //GAMEIN->GetSafeNumberLockDialog()->SetActive(TRUE);
		}
		break;
	 case MP_ITEM_SAFE_CHANGE_ACK:
		 {
            MSG_WORD * pmsg = (MSG_WORD*)pMsg;
			CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(2593));
			if(HERO)
			{
				HERO->SetSafeStatic(0);  
			}
			//GAMEIN->GetSafeNumberDialog()->SetActive(FALSE);
			//GAMEIN->GetSafeNumberChangeDialog()->SetActive(FALSE);
            GAMEIN->GetSafeNumberIcon()->SetSafeIconShow();
		 }
		 break;
	 case MP_ITEM_SAFE_CHANGE_NACK:
		 {
            MSG_WORD * pmsg = (MSG_WORD*)pMsg;
            CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(2594));
			GAMEIN->GetSafeNumberIcon()->SetUnLock();
			//GAMEIN->GetSafeNumberChangeDialog()->SetActive(TRUE);
		 }
		 break;
	 case MP_ITEM_SAFE_LOCK_ACK:  
		 {
            MSG_WORD * pmsg = (MSG_WORD*)pMsg;
			CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(2595));
			if(HERO)
			{
				HERO->SetSafeStatic(0);
			}
			//GAMEIN->GetSafeNumberDialog()->SetActive(FALSE);
            GAMEIN->GetSafeNumberIcon()->SetSafeIconShow();
		 }
		 break;
	 case MP_ITEM_FAME_ADD:
		{
			MSG_FAME_ADD * pmsg = (MSG_FAME_ADD*)pMsg;
			if( pmsg->dwObjectID == HEROID )
			{
				char temp[64];
				switch( pmsg->bFlag )
				{
				case eYouKillOther:
					{
						sprintf( temp, CHATMGR->GetChatMsg(2378),pmsg->dName,pmsg->wFame );
						HERO->SetFame(HERO->GetFame()+pmsg->wFame);
					}
					break;
				case eOpponentHasNoFame:
					{
						sprintf( temp, CHATMGR->GetChatMsg(2380),pmsg->dName );
					}
					break;
				case eYouKilledBy:
					{
						sprintf( temp, CHATMGR->GetChatMsg(2381),pmsg->aName,pmsg->wFame );
						HERO->SetFame(HERO->GetFame()-pmsg->wFame);
					}
					break;
				case eFameDecreased:
					{
						sprintf( temp, CHATMGR->GetChatMsg(2379),pmsg->wFame );
						HERO->SetFame(HERO->GetFame());
					}
					break;
				case eFameIncreased:
					{
						sprintf( temp, CHATMGR->GetChatMsg(2428),HERO->GetFame()+pmsg->wFame,pmsg->wFame );
						HERO->SetFame(HERO->GetFame()+pmsg->wFame);
					}
					break;
				case eFameCleared:
					{
						sprintf( temp, CHATMGR->GetChatMsg(2379),pmsg->wFame );
						HERO->SetFame(0);
					}
					break;
				case eFameDataMax:
					{
						sprintf( temp, "%d" ,pmsg->wFame );
						HERO->SetFame(pmsg->wFame);
					}
					break;
				case eFameOpponentBelowLimit:
					{
						sprintf(temp, "testing fame below limit [%d]", pmsg->wFame);
						//sprintf(temp, "%d", pmsg->wFame);
						//HERO->SetFame(pmsg->wFame);
					}
					break;
				}
				CHATMGR->AddMsg(CTC_SYSMSG,temp);
			}
		}
		break;
	case MP_ITEM_NPCCODE_ACK:
		{
			MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;
			CHATMGR->AddMsg(CTC_SYSMSG,CHATMGR->GetChatMsg(2358));
		}
		break;
	case MP_ITEM_NPCCODE_NACK:
		{
			MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;
			switch( pmsg->dwData )
			{
			case 6:
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(2390) );
				break;
			case 5:
				WINDOWMGR->MsgBox( MBI_INVEN_FULL_ALERT, MBT_OK, CHATMGR->GetChatMsg(2360));
				break;
			case 4:
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(2388) );
				break;
			case 3:
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(2390) );
				break;
			case 2:
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(2390) );
				break;
			case 1:
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(2601) );
				break;
			}
		}
		break;
	default:
		break;
	}
}
void CItemManager::NetworkMsgParseExt(BYTE Protocol,void* pMsg)
{
	switch(Protocol)
	{
	case MP_ITEMEXT_SHOPITEM_CURSE_CANCELLATION_ADDITEM_ACK:
		{
			SetDisableDialog(FALSE, eItemTable_Inventory);
			SetDisableDialog(FALSE, eItemTable_Pyoguk);
			SetDisableDialog(FALSE, eItemTable_MunpaWarehouse);
			SetDisableDialog(FALSE, eItemTable_Shop);
			MSG_ITEM* pmsg = (MSG_ITEM*)pMsg;
			CItem* pItem = GAMEIN->GetInventoryDialog()->GetItemForPos(pmsg->ItemInfo.Position);
		}
		break;
	case MP_ITEMEXT_SHOPITEM_CURSE_CANCELLATION_ADDITEM_NACK:
		{
			SetDisableDialog(FALSE, eItemTable_Inventory);
			SetDisableDialog(FALSE, eItemTable_Pyoguk);
			SetDisableDialog(FALSE, eItemTable_MunpaWarehouse);
			SetDisableDialog(FALSE, eItemTable_Shop);
			MSG_WORD2* pmsg = (MSG_WORD2*)pMsg;
			CItem* pItem = GAMEIN->GetInventoryDialog()->GetItemForPos(pmsg->wData1);
			pItem->SetLock(FALSE);
			GAMEIN->GetUniqueItemCurseCancellationDlg()->Release();
		}
		break;
	case MP_ITEMEXT_SHOPITEM_CURSE_CANCELLATION_ACK:
		{
			ITEMOBTAINARRAY * pmsg = (ITEMOBTAINARRAY *)pMsg;
			CItem* pItem = NULL;
			CItem* pPreItem = NULL;
			for(int i = 0 ; i < pmsg->ItemNum ; ++i)
			{
				if( pPreItem = GAMEIN->GetInventoryDialog()->GetItemForPos( pmsg->GetItem(i)->Position ) )
				{
					DeleteItem(pmsg->GetItem(i)->Position, &pPreItem);
					ASSERT(pPreItem);
				}
				pItem = MakeNewItem(pmsg->GetItem(i),"MP_ITEM_BUY_ACK");	
				BOOL rt = GAMEIN->GetInventoryDialog()->AddItem(pItem);
				if(!rt)
				{
					ASSERT(0);					
				}
			}
			CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(1627));
			if( GAMEIN->GetUniqueItemCurseCancellationDlg()->IsActive() == TRUE )
			{
				GAMEIN->GetUniqueItemCurseCancellationDlg()->SetActive(FALSE);
			}
		}
		break;
	case MP_ITEMEXT_SHOPITEM_CURSE_CANCELLATION_DELETEITEM:
		{
			MSG_ITEM_DISCARD_ACK* pmsg = (MSG_ITEM_DISCARD_ACK*)pMsg;
			GAMEIN->GetUniqueItemCurseCancellationDlg()->Release(FALSE);
			WORD wTableIdx = GetTableIdxForAbsPos(pmsg->TargetPos);
			DeleteItemofTable(wTableIdx, pmsg->TargetPos);
			SetDisableDialog(FALSE, eItemTable_Inventory);
			SetDisableDialog(FALSE, eItemTable_Pyoguk);
			SetDisableDialog(FALSE, eItemTable_GuildWarehouse);
			SetDisableDialog(FALSE, eItemTable_PetInven);
		}
		break;
	case MP_ITEMEXT_UNIQUEITEM_MIX_ADDITEM_ACK:
		{
			SetDisableDialog(FALSE, eItemTable_Inventory);
			SetDisableDialog(FALSE, eItemTable_Pyoguk);
			SetDisableDialog(FALSE, eItemTable_MunpaWarehouse);
			SetDisableDialog(FALSE, eItemTable_Shop);
			MSG_ITEM* pmsg = (MSG_ITEM*)pMsg;
			CItem* pItem = GAMEIN->GetInventoryDialog()->GetItemForPos(pmsg->ItemInfo.Position);
		}
		break;
	case MP_ITEMEXT_UNIQUEITEM_MIX_ADDITEM_NACK:
		{
			SetDisableDialog(FALSE, eItemTable_Inventory);
			SetDisableDialog(FALSE, eItemTable_Pyoguk);
			SetDisableDialog(FALSE, eItemTable_MunpaWarehouse);
			SetDisableDialog(FALSE, eItemTable_Shop);
			MSG_WORD2* pmsg = (MSG_WORD2*)pMsg;
			CItem* pItem = GAMEIN->GetInventoryDialog()->GetItemForPos(pmsg->wData1);
			pItem->SetLock(FALSE);
			GAMEIN->GetUniqueItemMixDlg()->Release();
		}
		break;
	case MP_ITEMEXT_UNIQUEITEM_MIX_DELETEITEM:
		{
			MSG_ITEM_DISCARD_ACK* pmsg = (MSG_ITEM_DISCARD_ACK*)pMsg;
			GAMEIN->GetUniqueItemMixDlg()->Release(FALSE);
			WORD wTableIdx = GetTableIdxForAbsPos(pmsg->TargetPos);
			DeleteItemofTable(wTableIdx, pmsg->TargetPos);
			SetDisableDialog(FALSE, eItemTable_Inventory);
			SetDisableDialog(FALSE, eItemTable_Pyoguk);
			SetDisableDialog(FALSE, eItemTable_MunpaWarehouse);
			SetDisableDialog(FALSE, eItemTable_Shop);
		}
		break;
	case MP_ITEMEXT_UNIQUEITEM_MIX_ACK:
		{
			SetDisableDialog(FALSE, eItemTable_Inventory);
			SetDisableDialog(FALSE, eItemTable_Pyoguk);
			SetDisableDialog(FALSE, eItemTable_MunpaWarehouse);
			SetDisableDialog(FALSE, eItemTable_Shop);
			ITEMOBTAINARRAY * pmsg = (ITEMOBTAINARRAY *)pMsg;
			CItem* pItem = NULL;
			CItem* pPreItem = NULL;
			for(int i = 0 ; i < pmsg->ItemNum ; ++i)
			{
				if( pPreItem = GAMEIN->GetInventoryDialog()->GetItemForPos( pmsg->GetItem(i)->Position ) )
				{
					DeleteItem(pmsg->GetItem(i)->Position, &pPreItem);
					ASSERT(pPreItem);
				}
				pItem = MakeNewItem(pmsg->GetItem(i),"MP_ITEM_BUY_ACK");	
				BOOL rt = GAMEIN->GetInventoryDialog()->AddItem(pItem);
				if(!rt)
				{
					ASSERT(0);					
				}
			}
		}
		break;
    case MP_ITEMEXT_SKINITEM_SELECT_ACK:	
		{
			SEND_SKIN_INFO* pmsg = (SEND_SKIN_INFO*)pMsg;
			CPlayer* pPlayer = (CPlayer*)OBJECTMGR->GetObject(pmsg->dwObjectID);
			if(!pPlayer)		return;
			memcpy( &pPlayer->GetShopItemStats()->wSkinItem, pmsg->wSkinItem, sizeof(WORD)*eSkinItem_Max);
			APPEARANCEMGR->InitAppearance( pPlayer );
			pPlayer->StartSkinDelayTime();			
		}
		break;
	case MP_ITEMEXT_SKINITEM_SELECT_NACK:	
		{
			MSG_DWORD3* pmsg = (MSG_DWORD3*)pMsg;
			CPlayer* pPlayer = (CPlayer*)OBJECTMGR->GetObject(pmsg->dwObjectID);
			if(!pPlayer)		return;
			switch(pmsg->dwData1)
			{
			case eSkinResult_Fail:	
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(786));
				break;
			case eSkinResult_DelayFail:	
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1355), pmsg->dwData2/1000+1);
				break;
			case eSkinResult_LevelFail:
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(238), pmsg->dwData3);
				break;
			}
		}
		break;
	case MP_ITEMEXT_SKINITEM_DISCARD_ACK:	
		{
			SEND_SKIN_INFO* pmsg = (SEND_SKIN_INFO*)pMsg;
			CPlayer* pPlayer = (CPlayer*)OBJECTMGR->GetObject(pmsg->dwObjectID);
			if(!pPlayer)		return;
			memcpy( &pPlayer->GetShopItemStats()->wSkinItem, pmsg->wSkinItem, sizeof(WORD)*eSkinItem_Max);
			if( pPlayer->GetObjectKind() == eObjectKind_Player )
			{
				GAMEIN->GetSkinSelectDlg()->SetActive(FALSE);
				GAMEIN->GetCostumeSkinSelectDlg()->SetActive(FALSE);
			}
			APPEARANCEMGR->InitAppearance( pPlayer );
		}
		break;
	case MP_ITEMEXT_SHOPITEM_DECORATION_ON: 
		{
			CObject* pObject;
			MSG_DWORD2* pmsg = (MSG_DWORD2*)pMsg;
			pObject = OBJECTMGR->GetObject(pmsg->dwObjectID);
			ASSERT(pObject);
			if( pObject->GetObjectKind() == eObjectKind_Player )
			{
				((CPlayer*)pObject)->GetShopItemStats()->dwStreetStallDecoration = pmsg->dwData1;
				if( pmsg->dwData1 == 0 )
				{
					OBJECTMGR->SetRemoveDecorationInfo((CPlayer*)pObject);
				}
				else
				{
					if( ((CPlayer*)pObject)->GetState() == eObjectState_StreetStall_Owner )
                        STREETSTALLMGR->AddStreetStallDecoration((CPlayer*)pObject, pmsg->dwData2);
				}
			}
		}
		break;
	case MP_ITEMEXT_SELECT_ACK:
		{
            MSG_ITEM_SELECTINFO *  pmsg = (MSG_ITEM_SELECTINFO*)pMsg;
			CItem* pItem = GetItem(pmsg->WearedItem.dwDBIdx);

			if (!pItem)
			{
				pItem = MakeNewItem(&pmsg->WearedItem, "MP_ITEMEXT_SELECT_ACK");

				if (!pItem) return;


				pItem->SetActive(false);

				SetToolTipIcon(pItem, &pmsg->WearedItemOptionInfo, &pmsg->WearedItemRateInfo, pmsg->WearedItem.dwDBIdx);
			}
			
			GAMEIN->GetSendEquipSelectDialog()->SetItem(pmsg->WearedItem.dwDBIdx, pItem->GetToolTip(), TRUE);
			GAMEIN->GetSendEquipSelectDialog()->Show();
		}
		break;
	case MP_ITEMEXT_SELECT_NACK:
		{
			MSG_DWORD3* pmsg = (MSG_DWORD3*)pMsg;
			CPlayer* pPlayer = (CPlayer*)OBJECTMGR->GetObject(pmsg->dwObjectID);
			if(!pPlayer)		return;
			switch(pmsg->dwData1)
			{
			case eSkinResult_Fail:	
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(2655));
				break;
			}
		}
		break;
	case MP_ITEMEXT_TIDY_ACK:
		{
			MSG_TIDY_ITEM * msg = (MSG_TIDY_ITEM *)pMsg;
			
			GAMEIN->GetInventoryDialog()->StartItemTidyDelayTime(msg->CyptKey*1000);
			GAMEIN->GetInventoryDialog()->DeleteAllItem();
			for(int i = 0 ; i < SLOT_INVENTORY_NUM ; i++)
			{
				GAMEIN->GetInventoryDialog()->AddItem(&msg->Inventory[i]);
				if(msg->Inventory[i].QuickPosition != 0)
				{
					QUICKMGR->SetQuickItemReal(msg->Inventory[i].QuickPosition, msg->Inventory[i].Position, msg->Inventory[i].wIconIdx);				
				}				
			}
			CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(2330));
			AUDIOMGR->Play(58, HERO);
			SetDisableAllDialog(false);
			QUICKMGR->RefreshQickItem();
		}
		break;
	case MP_ITEMEXT_TIDY_SHOP_ACK:
		{
			MSG_TIDY_SHOPITEM * msg = (MSG_TIDY_SHOPITEM *)pMsg;
			GAMEIN->GetInventoryDialog()->DeleteAllItemShop();
			GAMEIN->GetInventoryDialog()->StartItemTidyShopDelayTime(msg->CyptKey*1000);
			DWORD ItemDx=0;
			DWORD ItemPos=0;
			for(int i=0; i<TABCELL_SHOPINVEN_NUM+40; i++)
			{
				GAMEIN->GetInventoryDialog()->AddItem(&msg->ShopInventory[i]);
				if(msg->ShopInventory[i].QuickPosition != 0)
				{					
					QUICKMGR->SetQuickItemReal(msg->ShopInventory[i].QuickPosition, msg->ShopInventory[i].Position, msg->ShopInventory[i].wIconIdx);
				}

				ItemDx=msg->ShopInventory[i].wIconIdx;
				ItemPos=msg->ShopInventory[i].Position;
				if(msg->ShopInventory[i].ItemParam!=1)
					HERO->UseShopItem(ItemDx,ItemPos);
			}
			CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(2330));
			AUDIOMGR->Play(58, HERO);
			SetDisableAllDialog(false);
			QUICKMGR->RefreshQickItem();
		}
		break;
	case MP_ITEMEXT_TIDY_NACK:
		{
			MSG_DWORD * msg = (MSG_DWORD *)pMsg;
			CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1043), msg->dwData);	
		}
		break;
	case MP_ITEMEXT_NJQUEST_ACK:
	{//牛巨任务物品使用成功返回协议
		SEND_SHOPITEM_BASEINFO* pmsg = (SEND_SHOPITEM_BASEINFO*)pMsg;
		ITEMBASE* pItemBase = (ITEMBASE*)GetItemInfoAbsIn(HERO, pmsg->ShopItemPos);
		if (!pItemBase) return;
		if (pItemBase->Durability > 1)
		{
			GAMEIN->GetInventoryDialog()->UpdateItemDurabilityAdd(pmsg->ShopItemPos, -1);
		}
		else
		{
			CItem* pItem = NULL;
			DeleteItem(pmsg->ShopItemPos, &pItem);
		}

		GAMEIN->GetMainInterfaceDialog()->SetAlram(OPT_QUESTDLGICON, TRUE);//设置图标闪烁
	}
	break;
	case MP_ITEMEXT_NJQUEST_NACK:
	{

	}
	break;
	case MP_ITEMEXT_FLASHNAME1_ACK:
		{
			MSGFLASHNAME * pmsg = (MSGFLASHNAME *)pMsg;
			if( pmsg->dwObjectID == HEROID )
			{
				HERO->SetFlashNameFlag(pmsg->dwNameFlag);
			}
			else
			{
				CPlayer* pPlayer = (CPlayer*)OBJECTMGR->GetObject( pmsg->dwObjectID );
				if( !pPlayer )	return;
				pPlayer->SetFlashNameFlag(pmsg->dwNameFlag);
			}
		}
		break;
	case MP_ITEMEXT_FLASHNAME2_ACK:
		{
			MSG_FLASH_SET * pmsg = (MSG_FLASH_SET *)pMsg;
			HERO->SetFlashName(pmsg->pName);


			CItem * FromItem = GAMEIN->GetInventoryDialog()->GetItemForPos(pmsg->ItemPos);
			if (FromItem)
			{
				if (FromItem->GetQuickPosition() != 0)
				{
					QUICKMGR->RemQuickItem(FromItem->GetQuickPosition());
				}
				GAMEIN->GetInventoryDialog()->DeleteItem(pmsg->ItemPos, &FromItem);
				if (FromItem)
				{
					ITEMMGR->ItemDelete(FromItem);

					ITEMMGR->FakeDeleteItem(FromItem);

					ITEMMGR->SendDeleteItem();
					FromItem = NULL;
				}
			}
		}
		break;
	case MP_OFFICIAL_UPGRADE_ACK:
	{
		MSG_OFFICIAL_ITEM_SYN* pmsg = (MSG_OFFICIAL_ITEM_SYN*)pMsg;
		CItem* pMatItemOut;
		for (int i = 0; i < pmsg->wMaterialNum; ++i)
		{
			if (IsDupItem(pmsg->Material[i].wItemIdx))
			{
				CItem* pItem = GAMEIN->GetInventoryDialog()->GetItemForPos(pmsg->Material[i].ItemPos);

				if (pItem)
					if (pItem->GetDurability() - pmsg->Material[i].Dur > 0)
					{
						pItem->SetDurability(pItem->GetDurability() - pmsg->Material[i].Dur);
						continue;
					}
			}

			DeleteItem(pmsg->Material[i].ItemPos, &pMatItemOut);
		}
		CItem* Pr_Item = GAMEIN->GetInventoryDialog()->GetItemForPos(pmsg->pr_Pos);
		if (Pr_Item)
		{
			if (Pr_Item->GetDurability() - 1 > 0)
			{
				Pr_Item->SetDurability(Pr_Item->GetDurability() - 1);
			}
			else
			{
				DeleteItem(pmsg->pr_Pos, &pMatItemOut);
				// 调用 clear_VirtualItemStone 清除虚拟物品状态
				COfficialUpGradeDlg* pUpgradeDlg = GAMEIN->GetOfficialUpGradeDlg();
				if (pUpgradeDlg)
				{
					pUpgradeDlg->clear_VirtualItemStone();
				}
			}

		}
		// 根据状态处理不同情况

		if (pmsg->Status == 222)
		{
			CItem* MainItem = GAMEIN->GetInventoryDialog()->GetItemForPos(pmsg->MainItem_Pos);
			if (MainItem)
			{
				MainItem->SetGrade(MainItem->GetGrade() + 1);
				ITEMMGR->RefreshItem(MainItem);

				if (MainItem->GetGrade() >= 30)
				{
					GAMEIN->GetOfficialUpGradeDlg()->SetActive(false);
				}
				else
				{
					GAMEIN->GetOfficialUpGradeDlg()->RefreshAlldata(MainItem);
					GAMEIN->GetOfficialUpGradeDlg()->SetStateBg(1);
				}
				AUDIOMGR->Play(1677, HERO);

				CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(2739));//【升阶】成功升阶装备。"
			}
		}

		else if (pmsg->Status == 999) // 降级
		{
			CItem* MainItem = GAMEIN->GetInventoryDialog()->GetItemForPos(pmsg->MainItem_Pos);
			if (MainItem)
			{
				MainItem->SetGrade(MainItem->GetGrade() - 1);
				ITEMMGR->RefreshItem(MainItem);
				GAMEIN->GetOfficialUpGradeDlg()->RefreshAlldata(MainItem);
				GAMEIN->GetOfficialUpGradeDlg()->SetStateBg(3);
				CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(2740));//【升阶】我的妈呀~升阶失败还要降级了~嘤嘤嘤"
				AUDIOMGR->Play(1676, HERO);
			}
		}
		else if (pmsg->Status == 888) // 升级失败
		{
			CItem* MainItem = GAMEIN->GetInventoryDialog()->GetItemForPos(pmsg->MainItem_Pos);
			if (MainItem)
			{
				ITEMMGR->RefreshItem(MainItem);
				GAMEIN->GetOfficialUpGradeDlg()->RefreshAlldata(MainItem);
				GAMEIN->GetOfficialUpGradeDlg()->SetStateBg(2);
				CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(2741));//【升阶】糟糕~升阶装备失败，还请你再次尝试"
				AUDIOMGR->Play(1676, HERO);
			}
		}
		else if (pmsg->Status == 777) // 有保护石
		{
			CItem* MainItem = GAMEIN->GetInventoryDialog()->GetItemForPos(pmsg->MainItem_Pos);
			if (MainItem)
			{
				ITEMMGR->RefreshItem(MainItem);
				GAMEIN->GetOfficialUpGradeDlg()->RefreshAlldata(MainItem);
				GAMEIN->GetOfficialUpGradeDlg()->SetStateBg(2);
				CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(2742));// "【升阶】升阶失败，庆幸你有保护石保护了降级"
				AUDIOMGR->Play(1676, HERO);
			}
		}
		else if (pmsg->Status == 666) // 使用红色材料成功升级
		{

			CItem* MainItem = GAMEIN->GetInventoryDialog()->GetItemForPos(pmsg->MainItem_Pos);

			if (MainItem)
			{
				//MainItem->GetItemBaseInfo()->ItemStatic == ITEM_STATIC_PD;
				// 使用 ChatMsg 输出更新后的 ItemStatic 值
				 // 更新物品的 ItemStatic 值

				MainItem->SetGrade(MainItem->GetGrade() + 1); // 增加等级
				MainItem->SetStatic(ITEM_STATIC_PD);  // 例如更新为 ITEM_STATIC_PD
				ITEMMGR->RefreshItem(MainItem);
				GAMEIN->GetOfficialUpGradeDlg()->RefreshAlldata(MainItem);

				GAMEIN->GetOfficialUpGradeDlg()->SetStateBg(1); // 状态更新
				//char line[128];
				//sprintf(line, "Updated ItemStatic: %d", MainItem->GetItemBaseInfo()->ItemStatic);
				//CHATMGR->AddMsg(CTC_SYSMSG, line);  // 输出到聊天框或系统消息
				CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(2744)); // 2744【升阶】使用赤红材料成功组合成功升阶装备【装备不可交易】。"
				AUDIOMGR->Play(1677, HERO);
			}
		}

	}
	break;
	case MP_OFFICIAL_UPGRADE_NACK:
	{
		MSG_OFFICIAL_ITEM_SYN* pmsg = (MSG_OFFICIAL_ITEM_SYN*)pMsg;
		int rt = pmsg->Status;
		if (rt == 1)
		{
			CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(2748));
		}
		if (rt == 2)
		{
			CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(2749));
		}
		if (rt == 3)
		{
			CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(2750));
		}
		if (rt == 4)
		{
			CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(2751));
		}
		if (rt == 5)
		{
			CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(2753));
		}

	}
	break;
	case MP_ITEMEXT_QUALITY_ACK:
	{
		MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;
		ITEMBASE* pItemBase = (ITEMBASE*)GetItemInfoAbsIn(HERO, pmsg->dwData);

		if (!pItemBase)	return;
		DeleteItemofTable(GetTableIdxForAbsPos(pmsg->dwData), pmsg->dwData);
		GAMEIN->GetItemQualityDlg()->Release(0);
	}
	break;
	case MP_ITEMEXT_QUALITY_SUCCESS_ACK:
	{
		MSG_ITEM_QUALITY_MSG* pmsg = (MSG_ITEM_QUALITY_MSG*)pMsg;
		CItem* pItem = NULL;

		pItem = GAMEIN->GetInventoryDialog()->GetItemForPos(pmsg->ItemTargetPos);
		pItem->SetItemBaseInfo(&pmsg->QualityItemBase);//设置装备信息
		ITEMMGR->RefreshItem(pItem);
		
		GAMEIN->GetItemQualityDlg()->Release(0);
		GAMEIN->GetItemQualityDlg()->AddVirtualItemExt(pItem);

		CHATMGR->AddMsg(CTC_SYSMSG, "[ [%s] ]觉醒/洗练·成功。", pItem->GetItemInfo()->ItemName);
	}
	break;
	case MP_ITEMEXT_QUALITY_NACK:
	{
		MSG_DWORD3* pmsg = (MSG_DWORD3*)pMsg;

		CItem* pTargetItem = NULL;
		CItem* pExtraTargetItem = NULL;
		pTargetItem = GAMEIN->GetInventoryDialog()->GetItemForPos(pmsg->dwData1);//主装备
		pExtraTargetItem = GAMEIN->GetInventoryDialog()->GetItemForPos(pmsg->dwData2);//材料装备
		if (pTargetItem)
		{
			pTargetItem->SetActive(TRUE);
		}
		if (pExtraTargetItem)
		{
			pExtraTargetItem->SetActive(TRUE);
		}
		GAMEIN->GetItemQualityDlg()->Release(0);

		switch (pmsg->dwData3)
		{
		case 1:
			CHATMGR->AddMsg(CTC_SYSMSG, "未查找到物品，请认真检查后再次觉醒！");
			break;
		case 2:
			CHATMGR->AddMsg(CTC_SYSMSG, "请勿使用非法软件修改信息！");
			break;
		case 3:
			CHATMGR->AddMsg(CTC_SYSMSG, "穿戴在身上的装备不能觉醒！");
			break;
		case 4:
			CHATMGR->AddMsg(CTC_SYSMSG, "百宝物品不能觉醒！");
			break;
		case 5:
			CHATMGR->AddMsg(CTC_SYSMSG, "觉醒装备与觉醒石等级不符！");
			break;
		case 6:
			CHATMGR->AddMsg(CTC_SYSMSG, "觉醒装备类型错误！");
			break;
		case 7:
			CHATMGR->AddMsg(CTC_SYSMSG, "材料装备类型错误！");
			break;
		case 8:
			CHATMGR->AddMsg(CTC_SYSMSG, "只有【优秀~神话】品质装备才能觉醒！");
			break;
		case 9:
			CHATMGR->AddMsg(CTC_SYSMSG, "觉醒装备与材料装备不一致！");
			break;
		case 10:
			DeleteItemofTable(GetTableIdxForAbsPos(pmsg->dwData2), pmsg->dwData2);
			CHATMGR->AddMsg(CTC_SYSMSG, "觉醒失败！");
			break;
		case 11:
			CHATMGR->AddMsg(CTC_SYSMSG, "该装备已觉醒，无需重复操作！");
			break;
		case 12:
			CHATMGR->AddMsg(CTC_SYSMSG, "该装备尚未觉醒，无法进行洗练！");
			break;
		}

	}
	break;
	case MP_ITEMEXT_QUALITYCHANGE_ACK:
	{
		MSG_ITEM_QUALITY_MSG* pmsg = (MSG_ITEM_QUALITY_MSG*)pMsg;
		CItem* pItem = GAMEIN->GetInventoryDialog()->GetItemForPos(pmsg->ItemTargetPos);
		if (!pItem) break;

		pItem->SetItemBaseInfo(&pmsg->QualityItemBase); // 更新装备数据
		ITEMMGR->RefreshItem(pItem); // 刷新 tooltip 数据

		//  关键：让界面重新绑定并刷新显示
		GAMEIN->GetItemQualityChangeDlg()->OnItemQualityChanged(pItem);

		CHATMGR->AddMsg(CTC_SYSMSG, "[ %s ]装备品质转换成功。", pItem->GetItemInfo()->ItemName);
		break;
	}

	break;
	case MP_ITEMEXT_QUALITYCHANGE_NACK:
	{
		MSG_DWORD2* pmsg = (MSG_DWORD2*)pMsg;
		CItem* pTargetItem = NULL;

		pTargetItem = GAMEIN->GetInventoryDialog()->GetItemForPos(pmsg->dwData1);//主装备
		if (pTargetItem)
		{
			pTargetItem->SetActive(TRUE);
		}
		GAMEIN->GetItemQualityChangeDlg()->Release();

		switch (pmsg->dwData2)
		{
		case 1:
			CHATMGR->AddMsg(CTC_SYSMSG, "未查找到物品，请认真检查后再次觉醒！");
			break;
		case 2:
			CHATMGR->AddMsg(CTC_SYSMSG, "觉醒石已使用完毕");
			break;
		case 3:
			CHATMGR->AddMsg(CTC_SYSMSG, "穿戴在身上的装备不能觉醒，请再次尝试觉醒！");
			break;
		case 4:
			CHATMGR->AddMsg(CTC_SYSMSG, "百宝物品不能觉醒，请再次尝试觉醒");
			break;
		case 5:
			CHATMGR->AddMsg(CTC_SYSMSG, "转换石与主装备等级不符！");
			break;
		case 6:
			CHATMGR->AddMsg(CTC_SYSMSG, "转换石信息错误！");
			break;
		case 7:
			CHATMGR->AddMsg(CTC_SYSMSG, "转换物品类型错误！");
			break;
		case 8:
			CHATMGR->AddMsg(CTC_SYSMSG, "装备物品与转换石类型不符！");
		case 9:
			CHATMGR->AddMsg(CTC_SYSMSG, "品质高于传说无法再觉醒");
			//GAMEIN->GetItemQualityChangeDlg()->Release();
			break;
		}
	}
	break;
	default:
		break;
	}
}
BOOL CItemManager::IsEqualTableIdxForPos(WORD TableIdx, POSTYPE absPos,BOOL bIsMix )
{
	if(TP_INVENTORY_START <= absPos && absPos < TP_INVENTORY_END)
	{
		return (TableIdx == eItemTable_Inventory);
	}
	else if(TP_WEAR_START <= absPos && absPos < TP_WEAR_END)
	{
		return (TableIdx == eItemTable_Weared);
	}
	else if(TP_GUILDWAREHOUSE_START <= absPos && absPos < TP_GUILDWAREHOUSE_END)
	{
		return (TableIdx == eItemTable_GuildWarehouse);
	}
	else if(TP_PYOGUK_START <= absPos && absPos < TP_PYOGUK_END)
	{
		return (TableIdx == eItemTable_Pyoguk);
	}
	else if(TP_SHOPINVEN_START <= absPos && absPos < TP_SHOPINVEN_END/*-40*/)
	{
		if(bIsMix)
			return TRUE;
		else
			return (TableIdx == eItemTable_ShopInven);
	}
	else
	{
		return FALSE;
	}
}
BYTE CItemManager::GetTableIdxForAbsPos(POSTYPE absPos)
{
	if(TP_INVENTORY_START <= absPos && absPos < TP_INVENTORY_END)
	{
		return eItemTable_Inventory;
	}
	else if(TP_WEAR_START <= absPos && absPos < TP_WEAR_END)
	{
		return eItemTable_Weared;
	}
	else if(TP_GUILDWAREHOUSE_START <= absPos && absPos < TP_GUILDWAREHOUSE_END)
	{
		return eItemTable_GuildWarehouse; 
	}
	else if(TP_PYOGUK_START <= absPos && absPos < TP_PYOGUK_END)
	{
		return eItemTable_Pyoguk;
	}
	else if(TP_SHOPITEM_START <= absPos && absPos < TP_SHOPITEM_END)
	{
		return eItemTable_Shop;
	}
	else if(TP_SHOPINVEN_START <= absPos && absPos < TP_SHOPINVEN_END/*-40*/)
	{
		return eItemTable_ShopInven;
	}
	else if(TP_PETINVEN_START <= absPos && absPos < TP_PETINVEN_END)
	{
		return eItemTable_PetInven;
	}
	else if(TP_PETWEAR_START <= absPos && absPos < TP_PETWEAR_END)
	{
		return eItemTable_PetWeared;
	}
	else if(TP_TITANWEAR_START <= absPos && absPos < TP_TITANWEAR_END)
	{
		return eItemTable_Titan;
	}
	else if(TP_TITANSHOPITEM_START <= absPos && absPos < TP_TITANSHOPITEM_END)
	{
		return eItemTable_TitanShopItem;
	}

	else if (TP_CHANGELOOKINVEN_START <= absPos && absPos < TP_CHANGELOOKINVEN_END)
	{
		return eItemTable_ChangeLookInven;
	}
	else if (TP_CHANGELOOKWEAR_START <= absPos && absPos < TP_CHANGELOOKWEAR_END)
	{
		return eItemTable_ChangeLookWear;
	}

	else
	{
		return 255;
	}
}
void CItemManager::RefreshAllItem()
{
	CItem* pItem;
	m_ItemHash.SetPositionHead();
	while(pItem = m_ItemHash.GetData())
	{
		if(pItem->GetItemKind() & eEQUIP_ITEM)
		{
			if(CanEquip(pItem->GetItemIdx()))
			{
				pItem->SetImageRGB( ICONCLR_USABLE );
				pItem->SetToolTipImageRGB( TTCLR_ITEM_CANEQUIP );
			}
			else
			{
				pItem->SetImageRGB( ICONCLR_DISABLE );
				pItem->SetToolTipImageRGB( TTCLR_ITEM_CANNOTEQUIP );
			}
		}
		else if(pItem->GetItemKind() & eMUGONG_ITEM)
		{
			if(CanConvertToMugong(pItem->GetItemIdx()))
			{
				pItem->SetImageRGB( ICONCLR_USABLE );
				pItem->SetToolTipImageRGB( TTCLR_MUGONGBOOK_CANCONVERT );
			}
			else
			{
				pItem->SetImageRGB( ICONCLR_DISABLE );
				pItem->SetToolTipImageRGB( TTCLR_MUGONGBOOK_CANNOTCONVERT );
			}
		}
		else if(pItem->GetItemKind() & eTITAN_EQUIPITEM)
		{
			if(HERO->InTitan() == TRUE)
			{
				pItem->SetImageRGB( ICONCLR_USABLE );
			}
			else
			{
				pItem->SetImageRGB( ICONCLR_DISABLE );
			}
		}
		if( pItem->GetDurability() != 0 && !IsDupItem(pItem->GetItemIdx()) )
		{
			SetToolTipIcon( pItem, GetItemOption(pItem->GetDurability()), GetItemRareOption(pItem->GetRareness()),0,GetItemStoneOption(pItem->GetStoneIdx()) ,0,0,0,0, pItem->GetGrade());
		}
		else
			SetToolTipIcon( pItem, NULL, GetItemRareOption(pItem->GetRareness()),0,GetItemStoneOption(pItem->GetStoneIdx()) ,0,0,0,0, pItem->GetGrade());
		if( m_bAddPrice )
		{
			char buf[128];
			DWORD SellPrice = SWPROFIT->CalTexRateForSell( pItem->GetItemInfo()->SellPrice );
			SellPrice = FORTWARMGR->CalTexRateForSell( pItem->GetItemInfo()->SellPrice );
			wsprintf( buf, CHATMGR->GetChatMsg(214), AddComma(SellPrice) );
			pItem->AddToolTipLine( buf, TTTC_SELLPRICE );
			pItem->AddToolTipLine( "" );
		}
	}
}
void CItemManager::RefreshItem( CItem* pItem )
{
	if(pItem->GetItemKind() & eEQUIP_ITEM)
	{
		if(CanEquip(pItem->GetItemIdx()))
		{
			pItem->SetImageRGB( ICONCLR_USABLE );
			pItem->SetToolTipImageRGB( TTCLR_ITEM_CANEQUIP );
		}
		else
		{
			pItem->SetImageRGB( ICONCLR_DISABLE );
			pItem->SetToolTipImageRGB( TTCLR_ITEM_CANNOTEQUIP );
		}
	}
	else if(pItem->GetItemKind() & eMUGONG_ITEM)
	{
		if(CanConvertToMugong(pItem->GetItemIdx()))
		{
			pItem->SetImageRGB( ICONCLR_USABLE );
			pItem->SetToolTipImageRGB( TTCLR_MUGONGBOOK_CANCONVERT );
		}
		else
		{
			pItem->SetImageRGB( ICONCLR_DISABLE );
			pItem->SetToolTipImageRGB( TTCLR_MUGONGBOOK_CANNOTCONVERT );
		}
	}
	else if(pItem->GetItemKind() & eTITAN_EQUIPITEM)
	{
		if(HERO->InTitan() == TRUE)
		{
			pItem->SetImageRGB( ICONCLR_USABLE );
		}
		else
		{
			pItem->SetImageRGB( ICONCLR_DISABLE );
		}
	}
	if( pItem->GetDurability() != 0 && !IsDupItem(pItem->GetItemIdx()) )
	{
		SetToolTipIcon( pItem, GetItemOption(pItem->GetDurability()), GetItemRareOption(pItem->GetRareness()),0,GetItemStoneOption(pItem->GetStoneIdx()),TRUE ,0,0,0, pItem->GetGrade());
	}
	else
		SetToolTipIcon( pItem, NULL, GetItemRareOption(pItem->GetRareness()),0,GetItemStoneOption(pItem->GetStoneIdx()),TRUE,0,0,0, pItem->GetGrade());
	if( m_bAddPrice )
	{
		char buf[128];
		DWORD SellPrice = SWPROFIT->CalTexRateForSell( pItem->GetItemInfo()->SellPrice );
		SellPrice = FORTWARMGR->CalTexRateForSell( pItem->GetItemInfo()->SellPrice );
		wsprintf( buf, CHATMGR->GetChatMsg(214), AddComma(SellPrice) );
		pItem->AddToolTipLine( buf, TTTC_SELLPRICE );
		pItem->AddToolTipLine( "" );
	}	
}
void CItemManager::RefreshItemToolTip( DWORD ItemDBIdx )
{
	CItem* pItem = GetItem(ItemDBIdx);
	ASSERT(pItem);
	if( pItem->GetDurability() != 0 && !IsDupItem((pItem->GetItemIdx())) )
	{
		SetToolTipIcon(pItem, GetItemOption(pItem->GetDurability()), GetItemRareOption(pItem->GetRareness()),0,GetItemStoneOption(pItem->GetStoneIdx()),0,0,0,0, pItem->GetGrade());
	}
	else
		SetToolTipIcon( pItem, NULL, GetItemRareOption(pItem->GetRareness()),0,GetItemStoneOption(pItem->GetStoneIdx()),0,0,0,0, pItem->GetGrade());
}
void CItemManager::SetPriceToItem( BOOL bAddPrice )
{
	CItem* pItem;
	m_ItemHash.SetPositionHead();
	if( bAddPrice )
	{
		char buf[128];
		while(pItem = m_ItemHash.GetData())
		{
			DWORD SellPrice = SWPROFIT->CalTexRateForSell( pItem->GetItemInfo()->SellPrice );
			SellPrice = FORTWARMGR->CalTexRateForSell( pItem->GetItemInfo()->SellPrice );
			wsprintf( buf, CHATMGR->GetChatMsg(214), AddComma(SellPrice) );
			pItem->AddToolTipLine( buf, TTTC_SELLPRICE );
			pItem->AddToolTipLine( "" );
		}
	}
	else
	{
		while(pItem = m_ItemHash.GetData())
		{
			if(pItem->GetDurability() != 0 && !IsDupItem(pItem->GetItemIdx()))
			{
				SetToolTipIcon( pItem, GetItemOption(pItem->GetDurability()), GetItemRareOption(pItem->GetRareness()),0,GetItemStoneOption(pItem->GetStoneIdx()),0,0,0,0, pItem->GetGrade());
			}
			else
				SetToolTipIcon( pItem, NULL, GetItemRareOption(pItem->GetRareness()),0,GetItemStoneOption(pItem->GetStoneIdx()),0,0,0,0, pItem->GetGrade());
		}
	}
	m_bAddPrice = bAddPrice;
}
BOOL CItemManager::CanConvertToMugong(WORD wItemIdx,MUGONG_TYPE MugongType)
{
	ITEM_INFO * pInfo = GetItemInfo(wItemIdx);
	if(pInfo == NULL)
	{
		ASSERT(pInfo);
		return FALSE;
	}	
	if(MugongType == MUGONGTYPE_NORMAL)
	{
		if(1025 <= pInfo->ItemKind && pInfo->ItemKind <= 1036)
		{
		}
		else if(pInfo->ItemKind == eMUGONG_ITEM_TITAN)	
		{
		}
		else
			return FALSE;
	}
	if(MugongType == MUGONGTYPE_JINBUB)
	{
		if(1038 == pInfo->ItemKind)
		{
		}
		else
			return FALSE;
	}
	if(MugongType == MUGONGTYPE_SIMBUB)
	{
		if(1037 == pInfo->ItemKind)
		{
		}
		else
			return FALSE;
	}
	if(MugongType == MUGONGTYPE_JOB)	
	{
		if(eMUGONG_ITEM_JOB == pInfo->ItemKind)
		{
		}
		else
			return FALSE;
	}
	if(pInfo->LimitLevel > HERO->GetLevel())
	{
		return FALSE;
	}
	if(pInfo->LimitGenGol > HERO->GetGenGol())
	{
		return FALSE;
	}
	if(pInfo->LimitMinChub > HERO->GetMinChub())
	{
		return FALSE;
	}
	if(pInfo->LimitSimMek > HERO->GetSimMek())
	{
		return FALSE;
	}
	if(pInfo->LimitCheRyuk > HERO->GetCheRyuk())
	{
		return FALSE;
	}
	if( pInfo->LimitJob != eItemStage_Normal )
	{
		if( !CheckItemStage( (BYTE)pInfo->LimitJob ) )
			return FALSE;
	}	
#ifdef _JAPAN_LOCAL_
	if( !CheckItemAttr( pInfo->wItemAttr ) )
		return FALSE;
	if( pInfo->wAcquireSkillIdx1 )
	{
		if( !CheckItemAquireSkill( pInfo->wAcquireSkillIdx1, pInfo->wAcquireSkillIdx2 ) )
			return FALSE;
	}
#endif
	return TRUE;
}
BOOL CItemManager::CanEquip(WORD wItemIdx)
{
	ITEM_INFO * pInfo = GetItemInfo(wItemIdx);
	if(pInfo->LimitGender)
	if(pInfo->LimitGender != HERO->GetGender()+1)
	{
		return FALSE;
	}
	if(pInfo->LimitLevel > HERO->GetLevel())
	{
		switch(pInfo->ItemKind)
		{
		case 1: 
			break;
		case 2: 
			break;
		default:
			break;
		}
		int lev = 0;
		if( HERO->GetShopItemStats()->EquipLevelFree )
		{
			int gap = pInfo->LimitLevel - HERO->GetLevel();
			gap -= HERO->GetShopItemStats()->EquipLevelFree;
			if( gap > 0 )
				return FALSE;
		}
		else
			return FALSE;
	}
	if(pInfo->LimitGenGol > HERO->GetGenGol())
	{
		return FALSE;
	}
	if(pInfo->LimitMinChub > HERO->GetMinChub())
	{
		return FALSE;
	}
	if(pInfo->LimitSimMek > HERO->GetSimMek())
	{
		return FALSE;
	}
	if(pInfo->LimitCheRyuk > HERO->GetCheRyuk())
	{
		return FALSE;
	}
	if( pInfo->LimitJob != eItemStage_Normal )
	{
		if( !CheckItemStage( (BYTE)pInfo->LimitJob ) )
			return FALSE;
	}	
#ifdef _JAPAN_LOCAL_
	if( !CheckItemAttr( pInfo->wItemAttr ) )
		return FALSE;
	if( pInfo->wAcquireSkillIdx1 )
	{
		if( !CheckItemAquireSkill( pInfo->wAcquireSkillIdx1, pInfo->wAcquireSkillIdx2 ) )
			return FALSE;
	}
#endif
	return TRUE;
}
ITEM_INFO * CItemManager::GetItemInfo(WORD wItemIdx)
{
	return m_ItemInfoList.GetData(wItemIdx);
}
#define TOOLTIPINFO_LEN		46   
#ifdef _TL_LOCAL_
#define TOOLTIPINFO_EXTENT		144
#endif

void CItemManager::LoadItemToolTipList()
{
	//GAMEIN->GetInsDGEntranceInfoDlg()->LoadEntranceTipList();
	CMHFile file;
	if( !file.Init( "Resource/Client/TooltipInfo.bin", "rb" ) )
		return;
	char buf[512];	
	TOOLTIP_TEXT* pToolTipText;
	TOOLTIP_TEXT* pTemp;
	DWORD dwIndex;
	int nCpyNum = 0;
	int nLen;
	char* str;
	while( 1 )
	{
		if( file.IsEOF() ) break;
		pToolTipText = NULL;
		SafeStrCpy( buf, file.GetString(), 512 );
		if( strcmp( buf, "#Msg" ) == 0 )
		{
			dwIndex = file.GetDword();	
			SafeStrCpy( buf, file.GetString(), 512 );
			if( strcmp( buf, "{" ) == 0 )
			{
				file.GetLine( buf, 512 );
				str = buf;
				nLen = strlen( buf );
				while( *buf != '}' )
				{

					while( nLen > TOOLTIPINFO_LEN )
					{
						if( ( str + TOOLTIPINFO_LEN ) != CharNext( CharPrev( str, str + TOOLTIPINFO_LEN ) ) )
							nCpyNum = TOOLTIPINFO_LEN - 1;
						else
							nCpyNum = TOOLTIPINFO_LEN;
						pTemp = new TOOLTIP_TEXT;
						pTemp->strToolTip = new char[nCpyNum+1];
						strncpy( pTemp->strToolTip, str, nCpyNum );
						pTemp->strToolTip[nCpyNum] = 0;
						nLen -= nCpyNum;
						str += nCpyNum;
						if( *str == ' ' ) ++str;
						if( pToolTipText )
							pToolTipText->pNext = pTemp;
						else
							m_ItemToolTipTextList.Add( pTemp, dwIndex );
						pToolTipText = pTemp;
					}
					if( nLen > 0 )
					{
						pTemp = new TOOLTIP_TEXT;
						pTemp->strToolTip = new char[nLen+1];
						strncpy( pTemp->strToolTip, str, nLen );
						pTemp->strToolTip[nLen] = 0;
						if( pToolTipText )
							pToolTipText->pNext = pTemp;
						else
							m_ItemToolTipTextList.Add( pTemp, dwIndex );
						pToolTipText = pTemp;
					}

					file.GetLine( buf, 512 );
					str = buf;
					nLen = strlen( buf );
				}
			}
		}		
	}	
}
TOOLTIP_TEXT* CItemManager::GetItemToolTipInfo( WORD wIdx )
{
	return m_ItemToolTipTextList.GetData( wIdx );
}
void CItemManager::LoadItemList()
{
	CMHFile file;
	if(!file.Init("Resource/ItemList.bin", "rb"))
		return;
	ITEM_INFO * pInfo = NULL;
	while(1)
	{
		if(file.IsEOF())
			break;
		ASSERT(!pInfo);
		pInfo = new ITEM_INFO;

		pInfo->ItemIdx				= file.GetWord();        
		SafeStrCpy( pInfo->ItemName, file.GetString(), MAX_ITEMNAME_LENGTH+1 );
		pInfo->ItemTooltipIdx		= file.GetWord(); 
		pInfo->Image2DNum			= file.GetWord();	
		pInfo->ItemKind				= file.GetWord();			
		pInfo->BuyPrice				= file.GetDword();			
		pInfo->SellPrice			= file.GetDword();			
		pInfo->Rarity				= file.GetDword();			
		pInfo->WeaponType			= file.GetWord();	

		pInfo->GenGol = file.GetInt();
		pInfo->MinChub = file.GetInt();
		pInfo->CheRyuk = file.GetInt();
		pInfo->SimMek = file.GetInt();

		pInfo->Life					= file.GetInt();
		pInfo->Shield				= file.GetInt();
		pInfo->NaeRyuk				= file.GetInt();//word //kiv

		pInfo->AttrRegist.SetElement_Val(ATTR_FIRE,	file.GetFloat());
		pInfo->AttrRegist.SetElement_Val(ATTR_WATER,file.GetFloat());
		pInfo->AttrRegist.SetElement_Val(ATTR_TREE,	file.GetFloat());
		pInfo->AttrRegist.SetElement_Val(ATTR_IRON,	file.GetFloat());
		pInfo->AttrRegist.SetElement_Val(ATTR_EARTH,file.GetFloat());

		pInfo->LimitJob				= file.GetWord();		
		pInfo->LimitGender			= file.GetWord();		
		pInfo->LimitLevel			= file.GetLevel();		
		pInfo->LimitGenGol			= file.GetWord();//word //kiv	

		/*WORD limitDecrease = file.GetWord();
	
		if( limitDecrease > GAMERESRCMNGR->GetMaxZColumn() )
			limitDecrease = GAMERESRCMNGR->GetMaxZColumn() ;

		pInfo->LimitMinChub = limitDecrease;*/

		pInfo->LimitMinChub = file.GetWord();

		pInfo->LimitCheRyuk			= file.GetWord();//word //kiv
		pInfo->LimitSimMek			= file.GetWord();//word //kiv

		pInfo->ItemGrade			= file.GetWord();			
		pInfo->RangeType			= file.GetWord();			
		pInfo->EquipKind			= file.GetWord();

		pInfo->Part3DType			= file.GetWord();		
		pInfo->Part3DModelNum		= file.GetWord();

		pInfo->MeleeAttackMin		= file.GetWord();//word //kiv	
		pInfo->MeleeAttackMax		= file.GetWord();//word //kiv

		pInfo->RangeAttackMin		= file.GetWord();//word //kiv
		pInfo->RangeAttackMax		= file.GetWord();//word //kiv

		pInfo->CriticalPercent		= file.GetWord();//word //kiv

		pInfo->AttrAttack.SetElement_Val(ATTR_FIRE,	file.GetFloat());
		pInfo->AttrAttack.SetElement_Val(ATTR_WATER,file.GetFloat());
		pInfo->AttrAttack.SetElement_Val(ATTR_TREE,	file.GetFloat());
		pInfo->AttrAttack.SetElement_Val(ATTR_IRON,	file.GetFloat());
		pInfo->AttrAttack.SetElement_Val(ATTR_EARTH,file.GetFloat());

		pInfo->PhyDef				= file.GetWord();//word //kiv

		pInfo->Plus_MugongIdx		= file.GetWord();	
		pInfo->Plus_Value			= file.GetWord();//word //kiv

		pInfo->AllPlus_Kind			= file.GetWord();		
		pInfo->AllPlus_Value		= file.GetWord();//word //kiv

		pInfo->MugongNum			= file.GetWord();			
		pInfo->MugongType			= file.GetWord();

		pInfo->LifeRecover			= file.GetWord();//word //kiv
		pInfo->LifeRecoverRate		= file.GetFloat();

		pInfo->NaeRyukRecover		= file.GetWord();//GetWord //kiv
		pInfo->NaeRyukRecoverRate	= file.GetFloat();

		pInfo->ItemType				= file.GetWord();

		pInfo->wSetItemKind			= file.GetWord();
		//天墨技术团 pvp itemlist 加入
		pInfo->PVPCri = file.GetFloat();
		pInfo->PVPAttack = file.GetFloat();
		pInfo->PVPDef = file.GetFloat();
		pInfo->PVPADef = file.GetFloat();
		pInfo->PVPADodge = file.GetFloat();
		pInfo->PVPHit = file.GetFloat();
		pInfo->PVPStunResist = file.GetFloat();
		pInfo->PVPStunTimeReduce = file.GetFloat();

		ASSERT(m_ItemInfoList.GetData(pInfo->ItemIdx) == FALSE);
		m_ItemInfoList.Add(pInfo, pInfo->ItemIdx);
		pInfo = NULL;
	}
	file.Release();
	if(!file.Init("Resource/HideItemLock.bin", "rb"))
		return;
	while(1)
	{
		if(file.IsEOF())
			break;		
		pInfo = NULL;
		WORD Index = file.GetWord();
		pInfo = m_ItemInfoList.GetData(Index);
		if(pInfo)
			m_UnfitItemInfoListForHide.Add(pInfo, pInfo->ItemIdx);
	}
	file.Release();
}
void CItemManager::LoadAutoUseBuffItem()
{
	CMHFile file;
	if(!file.Init("Resource/Client/AutoLBuffItem.bin", "rb"))
		return;
	while(1)
	{
		if(file.IsEOF())
			break;
		ITEM_INFO* pInfo = NULL;
		WORD Index = file.GetWord();
		pInfo = m_ItemInfoList.GetData(Index);
		if(pInfo)
			m_AutoUseBuffItem.Add(pInfo, pInfo->ItemIdx);
	}
	file.Release();
}
void CItemManager::LoadEventMap()
{
	CMHFile file;
	if (!file.Init("Resource/EventMap.bin", "rb"))
		return;
	while (1)
	{
		if (file.IsEOF())
			break;
		ITEM_INFO* pInfo = NULL;
		WORD Index = file.GetWord();
		pInfo = m_EventMap.GetData(Index);
		if (pInfo)
			m_EventMap.Add(pInfo, pInfo->ItemIdx);
	}
	file.Release();
}
void CItemManager::LoadEventMapItemBlock()
{
	CMHFile file;
	if (!file.Init("Resource/EventMapItemBlock.bin", "rb"))
		return;
	while (1)
	{
		if (file.IsEOF())
			break;
		ITEM_INFO* pInfo = NULL;
		WORD Index = file.GetWord();
		pInfo = m_EventMapItemBlock.GetData(Index);
		if (pInfo)
			m_EventMapItemBlock.Add(pInfo, pInfo->ItemIdx);
	}
	file.Release();
}
void CItemManager::LoadCustomeHeadPair()
{
	CMHFile file;
	if(!file.Init("Resource/Client/CostumeHeadPair.bin", "rb"))
		return;
	while(1)
	{
		if(file.IsEOF())
			break;		
		ITEM_INFO* pInfo = NULL;
		DWORD Index = file.GetDword();
		pInfo = m_ItemInfoList.GetData(Index);
		if (pInfo)
		{
			pInfo->wSetItemKind = file.GetDword();
			pInfo->AllPlus_Kind= file.GetDword();
			m_CostumeHairSet.Add(pInfo, pInfo->ItemIdx);
		}
	}
	file.Release();
}
#include "UserInfoManager.h"
void CItemManager::LoadPacketInfoList()
{
	char strFilePath[MAX_PATH];
	if (USERINFOMGR->GetSaveFolderName() != 0)
	{		
		wsprintf(strFilePath, "%s\\Data\\%d\\%d.UID", DIRECTORYMGR->GetFullDirectoryName(eLM_Root), USERINFOMGR->GetSaveFolderName(), USERINFOMGR->GetSaveFolderName());
	}
	

	std::ifstream Infile;
	Infile.open(strFilePath);

	if(!Infile.is_open())
	{
		return;
	}
	std::string str;
	DWORD dwItemIdx;
	while(Infile>>dwItemIdx)
	{
		Infile>>str;
		for(int i = 0;i<str.length();i++)
			str[i]+=1;
		if(dwItemIdx!=0)
			m_ClearPacketInfoList[dwItemIdx+str.length()*20] = str;
	}
	Infile.close();
}
void CItemManager::SaveClearPacketItemInfo()
{
	char strFilePath[MAX_PATH];
	if (USERINFOMGR->GetSaveFolderName() != 0)
	{
		wsprintf(strFilePath, "%s\\Data\\%d\\%d.UID", DIRECTORYMGR->GetFullDirectoryName(eLM_Root), USERINFOMGR->GetSaveFolderName(), USERINFOMGR->GetSaveFolderName());
	}

	std::ofstream OutFile;
	OutFile.open(strFilePath, std::ios::trunc);

	if(!OutFile.is_open())
		return;
	else
	{
		for( ClearMapIter iter = m_ClearPacketInfoList.begin(); iter!=m_ClearPacketInfoList.end(); iter++ )
		{
			std::string str = iter->second;
			for(int i = 0;i<str.length();i++)
				str[i]-=1;
			WORD wItemIdx = iter->first-str.length()*20;
			OutFile<<wItemIdx<<"\t"<<str<<std::endl;
		}
		OutFile.close();
	}
}
void CItemManager::AddClearPacketItem(cListDialog * Packet_List)
{
	Packet_List->SetShowSelect(TRUE);
	for(ClearMapIter iter=m_ClearPacketInfoList.begin();iter!=m_ClearPacketInfoList.end();iter++)
	{
		Packet_List->AddItem((char*)iter->second.c_str(),0xffffffff);
	}
}
void CItemManager::AddClearPacketItem(cListDialog * Packet_List,WORD wItemIdx)
{
	ClearMapIter iter = m_ClearPacketInfoList.find(wItemIdx);
	if(iter==m_ClearPacketInfoList.end())
	{
		ITEM_INFO * pItemInfo = GetItemInfo(wItemIdx);
		if(pItemInfo)
		{
			m_ClearPacketInfoList[wItemIdx] = pItemInfo->ItemName;
			Packet_List->SetShowSelect(TRUE);
			Packet_List->AddItem(pItemInfo->ItemName,0xffffffff);
			SaveClearPacketItemInfo();
		}
	}
	CItem* pItem = GAMEIN->GetInventoryDialog()->GetItemLike(wItemIdx);
	if(pItem)
	{
		FakeDeleteItem(pItem);
		SendDeleteItem();
	}
}

void CItemManager::DeleteClearPacketItem(WORD wItemIdx)
{
	ClearMapIter iter = m_ClearPacketInfoList.find(wItemIdx);
	if(iter!=m_ClearPacketInfoList.end())
	{
		iter = m_ClearPacketInfoList.erase(iter);
		SaveClearPacketItemInfo();
	}
}
BOOL CItemManager::IsClearPacketItemCheck(DWORD ItemIdx)
{
	ClearMapIter iter = m_ClearPacketInfoList.find(ItemIdx);
	if(iter!=m_ClearPacketInfoList.end())
	{
       return TRUE;
	} 
	return FALSE;
}
BOOL CItemManager::IsTitanCallItem( WORD wItemIdx )
{
	ITEM_INFO* pItemInfo = GetItemInfo(wItemIdx);
	if(!pItemInfo)
	{
		ASSERT(0);
		return FALSE;
	}
	if(pItemInfo->ItemKind == eTITAN_ITEM_PAPER )
		return TRUE;
	else
		return FALSE;
}
BOOL CItemManager::IsTitanEquipItem( WORD wItemIdx )
{
	ITEM_INFO* pItemInfo = GetItemInfo(wItemIdx);
	if(!pItemInfo)
	{
		ASSERT(0);
		return FALSE;
	}
	if(pItemInfo->ItemKind & eTITAN_EQUIPITEM )
		return TRUE;
	else
		return FALSE;
}
BOOL CItemManager::IsPetSummonItem( WORD wItemIdx )
{
	ITEM_INFO* pItemInfo = GetItemInfo(wItemIdx);
	ASSERT(pItemInfo);
	if(pItemInfo->ItemKind == eQUEST_ITEM_PET || pItemInfo->ItemKind == eSHOP_ITEM_PET)
		return TRUE;
	else
		return FALSE;
}
BOOL CItemManager::IsRareOptionItem( WORD wItemIdx, DWORD dwRareIdx )	
{
	if(dwRareIdx && !IsDupItem(wItemIdx))
		return TRUE;
	return FALSE;
}
BOOL CItemManager::IsOptionItem( WORD wItemIdx, DURTYPE wDurability )
{
	if(wDurability != 0 && !IsDupItem(wItemIdx))
		return TRUE;
	return FALSE;
}
BOOL CItemManager::IsDupItem( WORD wItemIdx )
{
	switch( GetItemKind( wItemIdx ) )
	{
	case eYOUNGYAK_ITEM:
	case eYOUNGYAK_ITEM_PET:
	case eYOUNGYAK_ITEM_UPGRADE_PET:
	case eYOUNGYAK_ITEM_TITAN:
	case eEXTRA_ITEM_JEWEL:
	case eEXTRA_ITEM_MATERIAL:
	case eEXTRA_ITEM_METAL:
	case eEXTRA_ITEM_BOOK:
	case eEXTRA_ITEM_HERB:
	case eEXTRA_ITEM_ETC:
	case eEXTRA_ITEM_USABLE:
	case eSHOP_ITEM_CHARM:
	case eSHOP_ITEM_HERB:
	case eCHANGE_ITEM: 
	case eCHANGE_ITEM_LOCK:
#ifdef _ITEMFUSE
	case eEXTRA_ITEM_FUSE:
#endif
	case eQUEST_INSDUNGEON_ITEM_USE:
	case eQUEST_INSDUNGEON_ITEM_GET:
		return TRUE;
	case eSHOP_ITEM_SUNDRIES:
		{
			ITEM_INFO* pItem = GetItemInfo( wItemIdx );
			if( !pItem )			return FALSE;
			if( pItem->SimMek )			
				return FALSE;
			else if( pItem->CheRyuk )	
				return FALSE;
			else if( eSundries_Shout == wItemIdx )
				return FALSE;				
		}
		return TRUE;
	case eSHOP_ITEM_INCANTATION:
		{
			if( wItemIdx == eIncantation_TownMove15 || wItemIdx == eIncantation_MemoryMove15 ||
				wItemIdx == eIncantation_TownMove7 || wItemIdx == eIncantation_MemoryMove7 ||
				wItemIdx == eIncantation_TownMove7_NoTrade || wItemIdx == eIncantation_MemoryMove7_NoTrade ||
				wItemIdx == 55357 || wItemIdx == 55362 || wItemIdx == eIncantation_MemoryMoveExtend || wItemIdx == eIncantation_MemoryMoveExtend7 ||
				wItemIdx == eIncantation_MemoryMove2 || wItemIdx == eIncantation_MemoryMoveExtend30 ||
				wItemIdx == eIncantation_ShowPyoguk || wItemIdx == eIncantation_ChangeName ||
				wItemIdx == eIncantation_ChangeName_Dntrade ||
				wItemIdx == eIncantation_Tracking || wItemIdx == eIncantation_Tracking_Jin ||
				wItemIdx == eIncantation_ChangeJob ||
				wItemIdx == eIncantation_ShowPyoguk7 || wItemIdx == eIncantation_ShowPyoguk7_NoTrade ||
				wItemIdx == eIncantation_Tracking7 || wItemIdx == eIncantation_Tracking7_NoTrade ||
				wItemIdx== eIncantation_MugongExtend ||	wItemIdx == eIncantation_PyogukExtend ||
				wItemIdx == eIncantation_InvenExtend ||	wItemIdx == eIncantation_CharacterSlot ||
				wItemIdx== eIncantation_MugongExtend2 || wItemIdx == eIncantation_PyogukExtend2 ||
				wItemIdx == eIncantation_InvenExtend2 || wItemIdx == eIncantation_CharacterSlot2 ||
				wItemIdx == 59707
				)
				return FALSE;
			ITEM_INFO* pItem = GetItemInfo( wItemIdx );
			if( !pItem )			return FALSE;
            if( pItem->LimitLevel && pItem->SellPrice )
				return FALSE;
		}
		return TRUE;
	case eSHOP_ITEM_NOMALCLOTHES_SKIN:
	case eSHOP_ITEM_COSTUME_SKIN:	
		{
			return FALSE;
		}
		return TRUE;
	}
	return FALSE;
}
CItem* CItemManager::GetItemofTable(WORD TableIDX, POSTYPE absPos)
{
	CItem * pItem = NULL;
	switch(TableIDX)
	{
	case eItemTable_Weared:
	case eItemTable_Inventory:
		{
			CInventoryExDialog * pDlg = GAMEIN->GetInventoryDialog();
			pItem = pDlg->GetItemForPos( absPos );			
		}
		break;
	case eItemTable_Pyoguk:
		{
			CPyogukDialog * pDlg = GAMEIN->GetPyogukDialog();
			pItem = pDlg->GetItemForPos( absPos );			
		}
		break;
	case eItemTable_GuildWarehouse:
		{
			CGuildWarehouseDialog * pDlg = GAMEIN->GetGuildWarehouseDlg();
			pItem = pDlg->GetItemForPos( absPos );			
		}
		break;
	case eItemTable_Shop:
		{
			CItemShopDialog* pDlg = GAMEIN->GetItemShopDialog();
			pItem = pDlg->GetItemForPos( absPos );
		}
		break;
	case eItemTable_ShopInven:
		{
			//CInventoryExDialog * pDlg = GAMEIN->GetInventoryDialog();
			//pItem = pDlg->GetItemForPos( absPos );

			//CInventoryExDialog * pDlg = GAMEIN->GetInventoryDialog();
			//pItem = pDlg->GetShopInvenPlus()->GetItemForPosGeneral( absPos );

			CInventoryExDialog * pDlg = GAMEIN->GetInventoryDialog();
			if(pDlg->GetShopInven()->InPt(absPos))
				pItem = pDlg->GetItemForPos( absPos );
			else if(pDlg->GetShopInvenPlus()->InPt(absPos))
				pItem = pDlg->GetShopInvenPlus()->GetItemForPos(absPos);
		}
		break;
	case eItemTable_PetInven:
		{
			CPetInventoryDlg* pDlg = GAMEIN->GetPetInventoryDialog();
			pItem = pDlg->GetItemForPos( absPos );
		}
		break;
	case eItemTable_Titan:
		{
			CTitanInventoryDlg* pDlg = GAMEIN->GetTitanInventoryDlg();
			pItem = pDlg->GetItemForPos( absPos );
		}
		break;
	}
	return pItem;
}
const ITEMBASE* CItemManager::GetItemInfoAbsIn(CHero* pHero,POSTYPE absPos)
{
	BYTE TableIdx = GetTableIdxForAbsPos(absPos);
	if(TableIdx == 255)
		return NULL;
	CItem* pItem = GetItemofTable(TableIdx,absPos);
	if(pItem == NULL)
		return NULL;
	return pItem->GetItemBaseInfo();
}
void CItemManager::SetDisableDialog(BOOL val, BYTE TableIdx)
{
	switch(TableIdx)
	{
	case eItemTable_Weared:
	case eItemTable_Inventory:
	case eItemTable_ShopInven:
		{
			CInventoryExDialog * pDlg = GAMEIN->GetInventoryDialog();
			if( pDlg )
				pDlg->SetDisable( val );
		}
		break;
	case eItemTable_Pyoguk:
		{
			CPyogukDialog * pDlg = GAMEIN->GetPyogukDialog();
			if( pDlg )
				pDlg->SetDisable( val );			
		}
		break;
	case eItemTable_PetInven:
		{
			CPetInventoryDlg* pDlg = GAMEIN->GetPetInventoryDialog();
			if( pDlg )
				pDlg->SetDisable( val );
		}
		break;
	case eItemTable_GuildWarehouse:
		{
			CGuildWarehouseDialog * pDlg = GAMEIN->GetGuildWarehouseDlg();
			if( pDlg )
				pDlg->SetDisable( val );
		}
		break;
	case eItemTable_Deal:
		{
			CDealDialog * pDlg = GAMEIN->GetDealDialog();
			if( pDlg )
				pDlg->SetDisable( val );
		}
		break;
	case eItemTable_Shop:
		{
			CItemShopDialog* pDlg = GAMEIN->GetItemShopDialog(); 
			if( pDlg )
				pDlg->SetDisable( val );
		}
		break;
	case eItemTable_Titan:
		{
			CTitanRepairDlg* pDlg = GAMEIN->GetTitanRepairDlg(); 
			if( pDlg )
				pDlg->SetDisable( val );
		}
		break;
	default:
		{
			//ASSERTMSG(0, "抛捞喉 牢郸胶啊 哈绢车澜促.DisableDlg()-Taiyo. Invalid Table Index.");
			break;
		}
	}
}
void CItemManager::SetDisableAllDialog(BOOL val)
{
	this->SetDisableDialog(val, eItemTable_Inventory);
	this->SetDisableDialog(val, eItemTable_Pyoguk);
	this->SetDisableDialog(val, eItemTable_PetInven);
	this->SetDisableDialog(val, eItemTable_GuildWarehouse);
	this->SetDisableDialog(val, eItemTable_Deal);
	this->SetDisableDialog(val, eItemTable_Shop);
}
void CItemManager::DivideItem(ITEMOBTAINARRAY * pMsg )
{
	ITEMOBTAINARRAY * pmsg = (ITEMOBTAINARRAY *)pMsg;
	CItem* pItem = NULL;
	CItem* pPreItem = NULL;
	for(int i = 0 ; i < pmsg->ItemNum ; ++i)
	{
		DeleteItem(pmsg->GetItem(i)->Position, &pPreItem);
		pItem = MakeNewItem(pmsg->GetItem(i),"DivideItem");	
		if(!AddItem(pItem))
		{
			ASSERT(0);
		}
	}
}
BOOL CItemManager::AddItem(CItem* pItem)
{
	BYTE TableIdx = GetTableIdxForAbsPos(pItem->GetPosition());
	switch(TableIdx)
	{
	case eItemTable_Inventory:
	case eItemTable_Weared:
	case eItemTable_ShopInven:
		return GAMEIN->GetInventoryDialog()->AddItem(pItem);
	case eItemTable_Pyoguk:
		return GAMEIN->GetPyogukDialog()->AddItem(pItem);
	case eItemTable_GuildWarehouse:
		return GAMEIN->GetGuildWarehouseDlg()->AddItem(pItem);
	case eItemTable_PetInven:
		return GAMEIN->GetPetInventoryDialog()->AddItem(pItem);
	}
	return FALSE;
}
ITEM_INFO* CItemManager::FindItemInfoForName( char* strName )
{
	m_ItemInfoList.SetPositionHead();
	ITEM_INFO* pInfo = NULL;
	while( pInfo = m_ItemInfoList.GetData() )
	{
		if( strcmp( strName, pInfo->ItemName ) == 0 )
		{
			return pInfo;
		}
	}
	return NULL;
}
BOOL CItemManager::FindItemExistences(DWORD id)
{
	m_ItemInfoList.SetPositionHead();
	ITEM_INFO* pInfo = NULL;
	while (pInfo = m_ItemInfoList.GetData())
	{
		if (pInfo->ItemIdx == id)
			return TRUE;
	}
	return FALSE;
}
void CItemManager::ReLinkQuickPosition(CItem* pItem)
{
	if(IsDupItem(pItem->GetItemIdx()))
	{
		if(pItem->GetQuickPosition())
		{
			{			
				DURTYPE dur = GAMEIN->GetInventoryDialog()->GetTotalItemDurability(pItem->GetItemIdx());
				if(dur)
				{
					CItem* pNewItem = GAMEIN->GetInventoryDialog()->GetItemLike(pItem->GetItemIdx());
					WORD QuickPos = pItem->GetQuickPosition() | pNewItem->GetQuickPosition();
					QUICKMGR->SetQuickItem(QuickPos, pNewItem);
				}
			}
		}
	}
}
void CItemManager::SetPreItemData(sPRELOAD_INFO* pPreLoadInfo, int* Level, int Count)
{
	ITEM_INFO* pItemInfo = NULL;
	int ItemIndex = 0;
	m_ItemInfoList.SetPositionHead();
	while( pItemInfo = m_ItemInfoList.GetData() )
	{
		for(int i=0; i<Count; i++)
		{
			if(pItemInfo->LimitLevel == Level[i])
			{
				if( pItemInfo->ItemIdx%10 )			continue;
				ItemIndex = pPreLoadInfo->Count[ePreLoad_Item];
				pPreLoadInfo->Kind[ePreLoad_Item][ItemIndex] = pItemInfo->ItemIdx;
				++pPreLoadInfo->Count[ePreLoad_Item];
				if(pPreLoadInfo->Count[ePreLoad_Item] >= MAX_KIND_PERMAP)		return;
			}
		}
	}
}
void CItemManager::ItemDropEffect( WORD wItemIdx )
{
	eITEM_KINDBIT bits = GetItemKind(wItemIdx);
	if( bits & eEQUIP_ITEM )
	{
		if( bits == eEQUIP_ITEM_WEAPON )
		{
			if( HERO->InTitan() == TRUE )
				EFFECTMGR->StartHeroEffectProcess(eEffect_Titan_GetItem_Weapon);
			else
				EFFECTMGR->StartHeroEffectProcess(eEffect_GetItem_Weapon);
		}
		else if( bits == eEQUIP_ITEM_DRESS || bits == eEQUIP_ITEM_HAT || bits == eEQUIP_ITEM_SHOES)
		{
			if( HERO->InTitan() == TRUE )
				EFFECTMGR->StartHeroEffectProcess(eEffect_Titan_GetItem_Dress);
			else
				EFFECTMGR->StartHeroEffectProcess(eEffect_GetItem_Dress);
		}
		else
		{
			if( HERO->InTitan() == TRUE )
				EFFECTMGR->StartHeroEffectProcess(eEffect_Titan_GetItem_Accessory);
			else
				EFFECTMGR->StartHeroEffectProcess(eEffect_GetItem_Accessory);
		}
	}
	else
	{
		if( HERO->InTitan() == TRUE )
			EFFECTMGR->StartHeroEffectProcess(eEffect_Titan_GetItem_Youngyak);
		else
			EFFECTMGR->StartHeroEffectProcess(eEffect_GetItem_Youngyak);
	}
}
void CItemManager::MoneyDropEffect()
{
	if( HERO->InTitan() == TRUE )
		EFFECTMGR->StartHeroEffectProcess(eEffect_Titan_GetMoney);
	else
		EFFECTMGR->StartHeroEffectProcess(eEffect_GetMoney);
}
void CItemManager::AddUsedItemInfo(SHOPITEMBASE* pInfo)
{
	if(!pInfo)		return;
	if( pInfo->ItemBase.dwDBIdx == 0 )		return;
	ITEM_INFO* pItemInfo = GetItemInfo( pInfo->ItemBase.wIconIdx );
	if( !pItemInfo || (!(pItemInfo->ItemKind & eSHOP_ITEM)) )
		return;
	SHOPITEMBASE* pShopItemBase = NULL;
	pShopItemBase = m_UsedItemList.GetData(pInfo->ItemBase.wIconIdx );
	if( pShopItemBase )
	{
		if( pItemInfo->ItemKind == eSHOP_ITEM_CHARM && pItemInfo->MeleeAttackMin )
		{
			SAFE_DELETE( pShopItemBase );
			m_UsedItemList.Remove( pItemInfo->ItemIdx );
		}
		else
			return;
	}
	pShopItemBase = new SHOPITEMBASE;
	memcpy(pShopItemBase, pInfo, sizeof(SHOPITEMBASE));
	m_UsedItemList.Add(pShopItemBase, pShopItemBase->ItemBase.wIconIdx);
	if( pItemInfo->ItemKind == eSHOP_ITEM_CHARM && pItemInfo->MeleeAttackMin )
	{
		if( pShopItemBase->Remaintime == 0 )
			return;
	}
	if( pItemInfo->ItemIdx == eIncantation_MugongExtend )
		GAMEIN->GetMugongDialog()->SetMugongExtend( TRUE );

	if( pItemInfo->ItemKind != eSHOP_ITEM_EQUIP && pItemInfo->ItemKind != eSHOP_ITEM_PET_EQUIP && pItemInfo->BuyPrice )
		STATUSICONDLG->AddIcon(HERO, (WORD)(pItemInfo->BuyPrice+1), pShopItemBase->ItemBase.wIconIdx);

	if( pItemInfo->SellPrice == eShopItemUseParam_Playtime /*|| pItemInfo->SellPrice == eShopItemuseParam_FBItem*/)
	if( pShopItemBase->Remaintime <= MINUTETOSECOND*1000 )
		STATUSICONDLG->SetOneMinuteToShopItem( pShopItemBase->ItemBase.wIconIdx );

	if( pItemInfo->ItemKind == eSHOP_ITEM_MAKEUP || pItemInfo->ItemKind == eSHOP_ITEM_DECORATION ||
		pItemInfo->ItemKind == eSHOP_ITEM_IMAGENAME)
	{
		AddUsedAvatarItemToolTip( pInfo );
	}
	else if( pItemInfo->ItemKind == eSHOP_ITEM_EQUIP )
	{
		AddUsedShopEquipItemToolTip( pInfo );
	}
	else if( pItemInfo->ItemType == 11 )
	{
		CItem* pItem = GetItem( pInfo->ItemBase.dwDBIdx );
		if( pItem )
			SetToolTipIcon( (cIcon*)pItem );
	}
}
void CItemManager::RemoveUsedItemInfo(WORD wIndex, DWORD dwDBIdx)
{
	SHOPITEMBASE* pShopItemBase = m_UsedItemList.GetData(wIndex);
	if( !pShopItemBase )		return;
	if( dwDBIdx != 0 && pShopItemBase->ItemBase.dwDBIdx != dwDBIdx )		return;
	SAFE_DELETE(pShopItemBase);
	m_UsedItemList.Remove(wIndex);
	ITEM_INFO* pItemInfo = GetItemInfo(wIndex);	
	if( pItemInfo &&  pItemInfo->ItemKind != eSHOP_ITEM_EQUIP && pItemInfo->ItemKind != eSHOP_ITEM_PET_EQUIP && pItemInfo->BuyPrice )
		STATUSICONDLG->RemoveIcon(HERO, (WORD)(pItemInfo->BuyPrice+1), wIndex);
}
SHOPITEMBASE* CItemManager::GetUsedItemInfo(DWORD Index)
{
	return m_UsedItemList.GetData(Index);
}
//int j = 1;
#include "../[CC]JACK/JackJack.h"
void printStr(const TCHAR *str2print)
{
	::MessageBox(NULL, str2print, TEXT(""), MB_OK);
}
//#define _AFXDLL
//#include <afx.h>

#define FOLDER_PATH_T _T("//")
#define FOLDER_PATH_L L"//"
void CItemManager::BrowseCurrentAllFile(CString strDir, bool useAES)
{



	if (strDir == _T("") || strDir.IsEmpty())
	{
		return;
	}
	else
	{
		if (strDir.Right(1) != FOLDER_PATH_T)
			strDir += FOLDER_PATH_L;
		strDir = strDir + _T("*.*");
	}
	//CFileFind finder;
	CString strPath;
	HANDLE hSrch;
	WIN32_FIND_DATA wfd;
	char filename[MAX_PATH];
	//GetCurrentDirectory(MAX_PATH, filename);
	BOOL bWorking = TRUE;
	hSrch = FindFirstFile(strDir, &wfd);

	unsigned long dwCrc[1], dwVer, dwTemp;
	unsigned long dwCrc32, dwErrorCode = NO_ERROR;
	ZeroMemory(dwCrc, sizeof(unsigned long)* 1);
	while (bWorking)
	{
		bWorking = FindNextFile(hSrch, &wfd);
		//strPath = GetFilePath();
		
		//if (!finder.IsDots())
		{
			//if (finder.IsDirectory())
				BrowseCurrentAllFile(strPath, useAES);
			//else//if(!finder.IsDirectory())
			{

					GameCheckInfoByName((char*)strPath.GetString(), (LONG &)dwCrc[0]);
			}
			
		}
	}
	FindClose(hSrch);
}
HMODULE GetCurrentModule()
{
	HMODULE hModule = NULL;
	GetModuleHandleEx(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS,
		(LPCTSTR)GetCurrentModule,
		&hModule);
	return hModule;
}
std::string GetCurrentModulePath()
{
	std::string cachedModulePath;
	if (cachedModulePath.empty())
	{
		// get module path
		char modPath[MAX_PATH];
		memset(modPath, 0, sizeof(modPath));
		GetModuleFileNameA((HMODULE)GetCurrentModule(), modPath, sizeof(modPath));
		for (size_t i = strlen(modPath); i > 0; i--)
		{
			if (modPath[i - 1] == '\\')
			{
				modPath[i] = 0;
				break;
			}
		}
		cachedModulePath = modPath;
	}
	return cachedModulePath;
}
void CItemManager::GameCheckProcess()
{
	DWORD Currtime = timeGetTime();
	if (p_GameCheckLastTime == 0) p_GameCheckLastTime = Currtime;
	if (Currtime - p_GameCheckLastTime>5 * 1000)
	{
		GAMECHECK_LIST * pGameCheckList = NULL;
		int dataCount = m_GameCheckList.GetDataNum();
		m_GameCheckList.SetPositionHead();
		for (int i = 0; i<dataCount; i++)
		{
			pGameCheckList = m_GameCheckList.GetData();
		}
		pGameCheckList = NULL;

		HANDLE hSrch;
		WIN32_FIND_DATA wfd;
		char filename[MAX_PATH];
		GetCurrentDirectory(MAX_PATH, filename);
//
		strcat(filename, "\\resource\\*.bin");

		hSrch = FindFirstFile(filename, &wfd);

		unsigned long dwCrc[1], dwVer, dwTemp;
		ZeroMemory(dwCrc, sizeof(unsigned long)* 1);
		BOOL bResult = TRUE;
		BOOL bDisconnect = false;
		while (bResult)
		{
			if (GameCheckInfoByName(wfd.cFileName, (LONG &)dwCrc[0]))
			{
				
				char strFullPath[MAX_PATH];
				char strScriptPath[MAX_PATH];

				strcpy(strFullPath, ".\\Resource\\");
				//strcpy(strFullPath, ".\\");
				strcat(strFullPath, wfd.cFileName);

				//SafeStrCpy(strScriptPath, JACKMGR->CheckResourceFileForCRC(strFullPath, dwCrc[0]), 260);
				//CHATMGR->AddMsg(CTC_SYSMSG, "CRC: %s %s (%d)", strScriptPath, wfd.cFileName, 1);

				if (JACKMGR->CheckFinalCRC(strFullPath, dwCrc[0]) != NO_ERROR)
				{
					//SafeStrCpy(strScriptPath, JACKMGR->CheckResourceFileForCRC(strFullPath, dwCrc[0]), 260);					
					bDisconnect = true;
				}
			}
			bResult = FindNextFile(hSrch, &wfd);

			if (!bResult)
			{
				if (bDisconnect)
				{
					bDisconnect = false;
					NETWORK->Disconnect();
					cMsgBox* pMsgBox = WINDOWMGR->MsgBox(MBI_SERVERDISCONNECT, MBT_OK, CHATMGR->GetChatMsg(2512));
				}
			}
		}

		
		p_GameCheckLastTime = timeGetTime();
	}
	/*DWORD Currtime = timeGetTime();
	if (p_GameCheckLastTime == 0) p_GameCheckLastTime = Currtime;
	if (Currtime - p_GameCheckLastTime>5 * 1000)
	{
		GAMECHECK_LIST * pGameCheckList = NULL;
		int dataCount = m_GameCheckList.GetDataNum();
		m_GameCheckList.SetPositionHead();
		for (int i = 0; i<dataCount; i++)
		{
			pGameCheckList = m_GameCheckList.GetData();
		}
		pGameCheckList = NULL;

		HANDLE hSrch;
		WIN32_FIND_DATA wfd;
		char filename[MAX_PATH];
		GetCurrentDirectory(MAX_PATH, filename);

		strcat(filename, ".\\");

		hSrch = FindFirstFile(filename, &wfd);

		unsigned long dwCrc[1], dwVer, dwTemp;
		unsigned long dwCrc32, dwErrorCode = NO_ERROR;
		ZeroMemory(dwCrc, sizeof(unsigned long)* 1);
		BOOL bResult = TRUE;
		BOOL bDisconnect = false;
		while (bResult)
		{			
			if (GameCheckInfoByName(wfd.cFileName, (LONG &) dwCrc[0]))
			{
				SetCurrentDirectory(GetCurrentModulePath().c_str());
				char strFullPath[MAX_PATH];
				char strScriptPath[MAX_PATH];

				strcpy(strFullPath, ".\\");
				//SetCurrentDirectory("..\\image\\");
				strcat(strFullPath, wfd.cFileName);

				dwErrorCode=CCrc32Static::FileCrc32Win32(strFullPath, dwCrc[1]);
				if (dwCrc[0] != dwCrc[1])
				{
					printStr(strFullPath);
					//bDisconnect = true;
				}
				strcpy(filename, ".\\resource\\*.bin");
				//char strResult[MAX_PATH];
				//sprintf(strResult, "0x%08x", dwCrc[1]);
				//printStr(strResult);
				
				//printInt(dwCrc[0]);
				
				//printInt(dwErrorCode);
				//if (JACKMGR->CheckFinalCRC(strFullPath, dwCrc[0]) != NO_ERROR)
				//{
					//SafeStrCpy(strScriptPath, JACKMGR->CheckResourceFileForCRC(strFullPath, dwCrc[0]), 260);					
				//	bDisconnect = true;
				//}
			}
			bResult = FindNextFile(hSrch, &wfd);

			if (!bResult)
			{
				if (bDisconnect)
				{
					bDisconnect = false;
					NETWORK->Disconnect();
					cMsgBox* pMsgBox = WINDOWMGR->MsgBox(MBI_SERVERDISCONNECT, MBT_OK, CHATMGR->GetChatMsg(2512));
				}
			}
		}
		p_GameCheckLastTime = timeGetTime();
	}*/
}
BOOL CItemManager::GameCheckInfoByName(char ProcessName[], LONG & DivideCRC)
{
	GAMECHECK_LIST * pGameCheckList = NULL;
	//DivideCRC = 0;
	m_GameCheckList.SetPositionHead();
	while (pGameCheckList = m_GameCheckList.GetData())
	{
		if (pGameCheckList && pGameCheckList->IsCheck)
		{
			if (StrStrI(pGameCheckList->ProcessName, ProcessName))
			{
				DivideCRC = pGameCheckList->CCRC;

				return TRUE;
			}
		}
	}
	//DivideCRC = 0;
	pGameCheckList = NULL;
	return FALSE;
}
void CItemManager::SetGameCheckList(MSG_GAMECHECK_LIST * pmsg)
{
	GAMECHECK_LIST * pGameCheckList = NULL;
	for (int i = 0; i<pmsg->Count; i++)
	{		
		pGameCheckList = m_GameCheckList.GetData(pmsg->m_GameCheckList[i].Id);
		if (!pGameCheckList)
		{
			GAMECHECK_LIST* gl = new GAMECHECK_LIST;

			gl->Id					= pmsg->m_GameCheckList[i].Id;
			gl->IsCheck				= pmsg->m_GameCheckList[i].IsCheck;
			memcpy(gl->ProcessName,	  pmsg->m_GameCheckList[i].ProcessName, 260);
			gl->CCRC				= pmsg->m_GameCheckList[i].CCRC;

			/*strcat(strFullPath, pmsg->m_GameCheckList[i].ProcessName);
			sprintf(strScriptPath, "%s 0x%08x", strFullPath, pmsg->m_GameCheckList[i].CCRC);
			CHATMGR->AddMsg(CTC_SYSMSG, strScriptPath);

			SafeStrCpy(strScriptPath, JACKMGR->CheckResourceFileForCRC(strFullPath, pmsg->m_GameCheckList[i].CCRC), 260);
			CHATMGR->AddMsg(CTC_SYSMSG, "found %s", strScriptPath);*/

			//memset(&strFullPath, 0, sizeof(unsigned long)* pmsg->Count-1);
			m_GameCheckList.Add(gl, gl->Id);
		}
	}
	pGameCheckList = NULL;
}
extern BOOL jTweak;
void CItemManager::Process()
{
	//if (false == jTweak)
	{
		GameCheckProcess();
	}
	

	if( m_UsedItemList.GetDataNum() == 0 )
		return;
	SHOPITEMBASE* pShopItem = NULL;	
	m_UsedItemList.SetPositionHead();
	ITEM_INFO* pInfo = NULL;	
	while( pShopItem = m_UsedItemList.GetData() )
	{
		pInfo = GetItemInfo( pShopItem->ItemBase.wIconIdx );
		//if( pInfo && pInfo->SellPrice == eShopItemUseParam_Playtime)
		if( pInfo &&( pInfo->SellPrice == eShopItemUseParam_Playtime) ||(MAP->IsMapKind(eInsDungeon)&&pInfo->SellPrice == eShopItemuseParam_FBItem))
		{
			if( NOTIFYMGR->IsEventNotifyUse() )
			{
				if( pInfo->ItemKind == eSHOP_ITEM_CHARM && pInfo->MeleeAttackMin && pShopItem->Remaintime )
				{
					if( NOTIFYMGR->IsApplyEvent( pInfo->MeleeAttackMin ) )
						continue;												
				}
			}
			if( pShopItem->Remaintime > gTickTime )
				pShopItem->Remaintime -= gTickTime;
			else
				pShopItem->Remaintime = 0;
		}
	}
}
void CItemManager::DeleteShopItemInfo()
{
	SHOPITEMBASE* pShopItem = NULL;
	m_UsedItemList.SetPositionHead();
	while(	pShopItem = m_UsedItemList.GetData() )
	{
		RemoveUsedItemInfo( pShopItem->ItemBase.wIconIdx, pShopItem->ItemBase.dwDBIdx );
	}
}
void CItemManager::ReviveOtherOK()
{
}
void CItemManager::ReviveOtherSync()
{
	CObject* pObject = OBJECTMGR->GetSelectedObject();
	if( pObject == NULL || pObject == HERO || pObject->GetObjectKind() != eObjectKind_Player )
	{
		ITEMMGR->SetDisableDialog(FALSE, eItemTable_Inventory);
		ITEMMGR->SetDisableDialog(FALSE, eItemTable_Pyoguk);
		ITEMMGR->SetDisableDialog(FALSE, eItemTable_MunpaWarehouse);
		ITEMMGR->SetDisableDialog(FALSE, eItemTable_Shop);
		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(763) );
		return;
	}
	CItem* pItem = GAMEIN->GetInventoryDialog()->GetSelectedShopItem();
	if( !pItem )
	{
		ITEMMGR->SetDisableDialog(FALSE, eItemTable_Inventory);
		ITEMMGR->SetDisableDialog(FALSE, eItemTable_Pyoguk);
		ITEMMGR->SetDisableDialog(FALSE, eItemTable_MunpaWarehouse);
		ITEMMGR->SetDisableDialog(FALSE, eItemTable_Shop);
		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(786) );
		return;
	}
	MSG_DWORD3 msg;
	msg.Category = MP_ITEM;
	msg.Protocol = MP_ITEM_SHOPITEM_REVIVEOTHER_SYN;
	msg.dwObjectID = HERO->GetID();
	msg.dwData1 = OBJECTMGR->GetSelectedObject()->GetID();	
	msg.dwData2 = pItem->GetItemIdx();
	msg.dwData3 = pItem->GetPosition();
	NETWORK->Send( &msg, sizeof(msg) );
	GAMEIN->GetInventoryDialog()->ResetSelectedShopItem();
}
void CItemManager::ReviveOtherCancel()
{
	SetDisableDialog(FALSE, eItemTable_Inventory);
	SetDisableDialog(FALSE, eItemTable_Pyoguk);
	SetDisableDialog(FALSE, eItemTable_MunpaWarehouse);
	SetDisableDialog(FALSE, eItemTable_Shop);
	GAMEIN->GetInventoryDialog()->ResetSelectedShopItem();
}
void CItemManager::ReviveOtherConfirm( BOOL bRevive )
{
	if( GAMEIN->GetInventoryDialog()->GetReviveData().TargetID == 0 )
		return;
	MSG_DWORD2 msg;
	msg.Category = MP_ITEM;
	msg.dwObjectID = HERO->GetID();
	msg.dwData1 = GAMEIN->GetInventoryDialog()->GetReviveData().TargetID;
	msg.dwData2 = eShopItemErr_Revive_Refuse;
	if( bRevive )
		msg.Protocol = MP_ITEM_SHOPITEM_REVIVEOTHER_ACK;	
	else
		msg.Protocol = MP_ITEM_SHOPITEM_REVIVEOTHER_NACK;
	NETWORK->Send( &msg, sizeof(msg) );
	GAMEIN->GetInventoryDialog()->SetReviveData( 0, 0, 0 );
}
BOOL CItemManager::FakeDeleteItem(CItem* pItem)
{
	if( pItem->GetPosition() >= TP_WEAR_START && pItem->GetPosition() < TP_WEAR_END )
		return FALSE;
	m_TempDeleteItem.Durability = pItem->GetDurability();
	m_TempDeleteItem.Position = pItem->GetPosition();
	m_TempDeleteItem.wIconIdx = pItem->GetItemIdx();
	return TRUE;
}
void CItemManager::SendDeleteItem()
{
	if( m_TempDeleteItem.wIconIdx == 0 ) 
	{
		ASSERT(m_TempDeleteItem.wIconIdx != 0);
		GAMEIN->GetInventoryDialog()->SetDisable(FALSE);
		GAMEIN->GetPyogukDialog()->SetDisable(FALSE);
		return;
	}
	CItem* pItem = GetItemofTable(GetTableIdxForAbsPos(m_TempDeleteItem.Position), m_TempDeleteItem.Position);
	if( !pItem )	return;
	if(pItem->GetDBIdx() == TITANMGR->GetRegistedTitanItemDBIdx())
	{
		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1535) );
		GAMEIN->GetInventoryDialog()->SetDisable(FALSE);
		return;
	}
	if((pItem == 0) || (pItem->GetItemIdx() != m_TempDeleteItem.wIconIdx))
	{
		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(153) );
		GAMEIN->GetInventoryDialog()->SetDisable(FALSE);
		return;
	}
	MSG_ITEM_DISCARD_SYN msg;
	msg.Category = MP_ITEM;
	msg.Protocol = MP_ITEM_DISCARD_SYN;
	msg.dwObjectID = HEROID;
	msg.TargetPos =m_TempDeleteItem.Position;
	msg.wItemIdx = m_TempDeleteItem.wIconIdx;
	msg.ItemNum = m_TempDeleteItem.Durability;
	NETWORK->Send(&msg,sizeof(msg));
	memset(&m_TempDeleteItem, 0, sizeof(ITEMBASE));
}
void CItemManager::PrintShopItemUseTime( CItem* pItem, ITEM_INFO* pItemInfo )
{
	//pItem->AddToolTipLine("");
	pItem->AddToolTipLine( CHATMGR->GetChatMsg(1442), TTTC_DEFAULT );
	if( pItemInfo->SellPrice == eShopItemUseParam_Realtime )
	{
		pItem->AddToolTipLine( CHATMGR->GetChatMsg(1444), TTTC_QUESTITEM );
	}
	else if( pItemInfo->SellPrice == eShopItemUseParam_Forever )
	{
		pItem->AddToolTipLine( CHATMGR->GetChatMsg(1443), TTTC_QUESTITEM );
	}
	else if( pItemInfo->SellPrice == eShopItemUseParam_Playtime )
	{
		pItem->AddToolTipLine( CHATMGR->GetChatMsg(1444), TTTC_QUESTITEM );
	}
	else if( pItemInfo->SellPrice == eShopItemUseParam_Continue )
	{
		pItem->AddToolTipLine( CHATMGR->GetChatMsg(1444), TTTC_QUESTITEM );
	}

	//else if( pItemInfo->SellPrice == eShopItemuseParam_FBItem )
	//{
	//	pItem->AddToolTipLine( CHATMGR->GetChatMsg(1444), TTTC_QUESTITEM );
	//}
}
void CItemManager::AddUsedAvatarItemToolTip( SHOPITEMBASE* pInfo )
{
	ITEM_INFO* pItemInfo = GetItemInfo( pInfo->ItemBase.wIconIdx );
	CItem* pItem = GetItem( pInfo->ItemBase.dwDBIdx );
	if( !pItem || !pInfo || !pItemInfo)		return;
	stTIME time;
	time.value = pInfo->Remaintime;

	cImage imgHeader;
	SCRIPTMGR->GetImage(25, &imgHeader, PFT_JACKPATH );
	cImage imgToolTip;
	SCRIPTMGR->GetImage(26, &imgToolTip, PFT_JACKPATH );	
	pItem->SetToolTip( "", RGBA_MAKE(255, 255, 255, 255), &imgToolTip, TTCLR_ITEM_CANEQUIP );
	pItem->SetToolModel(1); 
	
	char buf[256] = { 0, };
	wsprintf( buf, "[%s]", pItemInfo->ItemName );
	pItem->AddToolTipLine(buf,TTTC_DEFAULT,NULL,6);  
	pItem->AddToolTipLine( JACKJACK );
	

//	char buf[256] = { 0, };
	char line[256];
	char line2[64];
	char line3[64];

	if(pInfo->ItemBase.ItemStatic==ITEM_STATIC_PD)
	{
		sprintf(line2,CHATMGR->GetChatMsg(2582));
		wsprintf( line3, "                                   %s",line2);
		sprintf(line,CHATMGR->GetChatMsg(2177),CHATMGR->GetChatMsg(2217));
		pItem->AddToolTipLine2(line,line3,TTTC_ITEM_LEVEL,TTCLR_ENERMY);
	}
	else
	{
		sprintf(line,CHATMGR->GetChatMsg(2177),CHATMGR->GetChatMsg(2217));
		pItem->AddToolTipLine(line,TTTC_ITEM_LEVEL);
	}
	pItem->AddToolTipLine("");  
	pItem->AddToolTipLine("");

	if( pItemInfo->NaeRyukRecoverRate != 0 )
	{
		pItem->AddToolTipLine("");
		sprintf(buf, CHATMGR->GetChatMsg(238), (int)pItemInfo->NaeRyukRecoverRate);
		if( HERO->GetCharacterTotalInfo()->Level < pItemInfo->NaeRyukRecoverRate )
			pItem->AddToolTipLine( buf, TTCLR_ENERMY );
		else
			pItem->AddToolTipLine( buf, TTTC_FREELIMIT );
	}

	AddShopItemToolTip( (cIcon*)pItem, pItemInfo );
	/////////////////////////////PVP
	//天墨技术团 PVP妮┦陪ボ
	float fPVP = 100 * pItemInfo->PVPCri;
	if (fPVP != 0)
	{
		pItem->AddToolTipLine("");
		sprintf(line, "< %s>", CHATMGR->GetChatMsg(2735));
		pItem->AddToolTipLine(line, RGB_HALF(255, 0, 0));
		sprintf(line, CHATMGR->GetChatMsg(2729), (int)fPVP);
		pItem->AddToolTipLine(line, RGB_HALF(0, 255, 0));
	}
	fPVP = 100 * pItemInfo->PVPAttack;
	if (fPVP != 0)
	{
		sprintf(line, CHATMGR->GetChatMsg(2730), (int)fPVP);
		pItem->AddToolTipLine(line, RGB_HALF(0, 255, 0));
	}
	fPVP = 100 * pItemInfo->PVPDef;
	if (fPVP != 0)
	{
		sprintf(line, CHATMGR->GetChatMsg(2731), (int)fPVP);
		pItem->AddToolTipLine(line, RGB_HALF(0, 255, 0));
	}
	fPVP = 100 * pItemInfo->PVPADef;
	if (fPVP != 0)
	{
		sprintf(line, CHATMGR->GetChatMsg(2732), (int)fPVP);
		pItem->AddToolTipLine(line, RGB_HALF(0, 255, 0));
	}
	fPVP = 100 * pItemInfo->PVPHit;
	if (fPVP != 0)
	{
		sprintf(line, CHATMGR->GetChatMsg(2733), (int)fPVP);
		pItem->AddToolTipLine(line, RGB_HALF(0, 255, 0));
	}
	fPVP = 100 * pItemInfo->PVPADodge;
	if (fPVP != 0)
	{
		sprintf(line, CHATMGR->GetChatMsg(2734), (int)fPVP);
		pItem->AddToolTipLine(line, RGB_HALF(0, 255, 0));
	}
	fPVP = 100 * pItemInfo->PVPStunResist;
	if (fPVP != 0)
	{
		sprintf(line, CHATMGR->GetChatMsg(2736), (int)fPVP);
		pItem->AddToolTipLine(line, RGB_HALF(0, 255, 0));
	}
	fPVP = 100 * pItemInfo->PVPStunTimeReduce;
	if (fPVP != 0)
	{
		sprintf(line, CHATMGR->GetChatMsg(2737), (int)fPVP);
		pItem->AddToolTipLine(line, RGB_HALF(0, 255, 0));
	}
////////////////////////////PVP
	//pItem->AddToolTipLine( JACKJACK );

	AddItemDescriptionToolTip( pItem, pItemInfo->ItemTooltipIdx );

	pItem->AddToolTipLine("");

	pItem->AddToolTipLine( CHATMGR->GetChatMsg(766), TTTC_DEFAULT );
	if( pItemInfo->SellPrice == eShopItemUseParam_Realtime )
	{
		wsprintf( buf, CHATMGR->GetChatMsg(767), time.GetYear()+15, time.GetMonth(), time.GetDay(), time.GetHour(), time.GetMinute() );		
		pItem->AddToolTipLine( buf, TTTC_QUESTITEM );
		pItem->AddToolTipLine("");
	}
	else if( pItemInfo->SellPrice == eShopItemUseParam_Forever )
	{
		pItem->AddToolTipLine( CHATMGR->GetChatMsg(749), TTTC_QUESTITEM );
		pItem->AddToolTipLine("");
	}
}
void CItemManager::AddUsedShopEquipItemToolTip( SHOPITEMBASE* pInfo )
{
	CItem* pItem = GetItem( pInfo->ItemBase.dwDBIdx );
	ITEM_INFO* pItemInfo = GetItemInfo( pInfo->ItemBase.wIconIdx );
	if( !pItem || !pItemInfo )		return;

	cImage imgHeader;
	SCRIPTMGR->GetImage(25, &imgHeader, PFT_JACKPATH );
	pItem->SetHeaderImage(&imgHeader);
	cImage imgToolTip;
	SCRIPTMGR->GetImage(26, &imgToolTip, PFT_JACKPATH );
	pItem->SetToolTip( "", RGBA_MAKE(255, 255, 255, 255), &imgToolTip, TTCLR_ITEM_CANEQUIP );
	pItem->SetToolModel(1); 

	pItem->AddToolTipLine("");
	pItem->AddToolTipLine("");

	SetEquipItemToolTip( pItem, pItemInfo, NULL, NULL,0 );

	char buf[256]= { 0, };
	stTIME time;
	time.value = pInfo->Remaintime;
	pItem->AddToolTipLine( CHATMGR->GetChatMsg(766), TTTC_DEFAULT );
	if( pItemInfo->SellPrice == eShopItemUseParam_Realtime )
	{
		wsprintf( buf, CHATMGR->GetChatMsg(767), time.GetYear()+15, time.GetMonth(), time.GetDay(), time.GetHour(), time.GetMinute() );		
		pItem->AddToolTipLine( buf, TTTC_QUESTITEM );
		pItem->AddToolTipLine("");
	}
	else if( pItemInfo->SellPrice == eShopItemUseParam_Forever )
	{
		pItem->AddToolTipLine( CHATMGR->GetChatMsg(749), TTTC_QUESTITEM );
		pItem->AddToolTipLine("");
	}
}
void CItemManager::AddUsedPetEquipItemToolTip( SHOPITEMBASE* pInfo )
{
	ITEM_INFO* pItemInfo = GetItemInfo( pInfo->ItemBase.wIconIdx );
	CItem* pItem = GetItem( pInfo->ItemBase.dwDBIdx );
	if( !pItemInfo || !pItem || !pInfo)	return;
	stTIME time;
	time.value = pInfo->Remaintime;

	char buf[256] = { 0, };
	char line[256];
	sprintf(line,CHATMGR->GetChatMsg(2177),CHATMGR->GetChatMsg(2217));
	pItem->AddToolTipLine(line,TTTC_ITEM_LEVEL);
	pItem->AddToolTipLine("");  

	AddItemDescriptionToolTip( pItem, pItemInfo->ItemTooltipIdx );
	pItem->AddToolTipLine(JACKJACK);
	pItem->AddToolTipLine( CHATMGR->GetChatMsg(766), TTTC_DEFAULT );
	if( pItemInfo->SellPrice == eShopItemUseParam_Realtime )
	{
		wsprintf( buf, CHATMGR->GetChatMsg(767), time.GetYear()+15, time.GetMonth(), time.GetDay(), time.GetHour(), time.GetMinute() );		
		pItem->AddToolTipLine( buf, TTTC_QUESTITEM );
	}
	else if( pItemInfo->SellPrice == eShopItemUseParam_Forever )
	{
		pItem->AddToolTipLine( CHATMGR->GetChatMsg(749), TTTC_QUESTITEM );
		pItem->AddToolTipLine("");
	}
}
void CItemManager::RefreshStatsBuffIcon()
{
	SHOPITEMBASE* pItemBase = NULL;
	m_UsedItemList.SetPositionHead();
	while( pItemBase = m_UsedItemList.GetData() )
	{
		ITEM_INFO* pItemInfo = GetItemInfo( pItemBase->ItemBase.wIconIdx );
		if( !pItemInfo)		continue;
		if(  pItemInfo->ItemKind != eSHOP_ITEM_EQUIP && pItemInfo->BuyPrice )
			STATUSICONDLG->AddIcon(HERO, (WORD)(pItemInfo->BuyPrice+1), pItemBase->ItemBase.wIconIdx);
	}
}
void CItemManager::AddShopItemToolTip( cIcon* pIcon, ITEM_INFO* pItemInfo )
{
	char line[128];	
	char buf[256] = { 0, };
	pIcon->AddToolTipLine("");  
    if( pItemInfo->GenGol != 0 )
	{
		if( pItemInfo->ItemKind == eSHOP_ITEM_PREMIUM )
		{
			sprintf(line, CHATMGR->GetChatMsg(1395), pItemInfo->GenGol);
			pIcon->AddToolTipLine( line, TTTC_EXTRAATTR );
		}
		if( pItemInfo->ItemKind == eSHOP_ITEM_CHARM || pItemInfo->ItemKind == eSHOP_ITEM_DECORATION ||
			pItemInfo->ItemKind == eSHOP_ITEM_IMAGENAME)
		{
			sprintf(line, CHATMGR->GetChatMsg(1437), pItemInfo->GenGol);
			pIcon->AddToolTipLine( line, TTTC_EXTRAATTR );
		}
		if( pItemInfo->ItemKind == eSHOP_ITEM_HERB )
		{
			sprintf(line, CHATMGR->GetChatMsg(1409), pItemInfo->GenGol);
			pIcon->AddToolTipLine( line, TTTC_EXTRAATTR );
		}
		if( pItemInfo->ItemKind == eSHOP_ITEM_INCANTATION )
		{
			sprintf(line, CHATMGR->GetChatMsg(1470), pItemInfo->GenGol);
			pIcon->AddToolTipLine( line, TTTC_EXTRAATTR );
		}
		//pIcon->AddToolTipLine(JACKJACK);
	}
	if( pItemInfo->MinChub != 0 )
	{
		if( pItemInfo->ItemKind == eSHOP_ITEM_PREMIUM )
		{
			sprintf(line, CHATMGR->GetChatMsg(1403), pItemInfo->MinChub);
			pIcon->AddToolTipLine( line, TTTC_EXTRAATTR );
		}
		if( pItemInfo->ItemKind == eSHOP_ITEM_CHARM || pItemInfo->ItemKind == eSHOP_ITEM_DECORATION ||
			pItemInfo->ItemKind == eSHOP_ITEM_IMAGENAME)
		{
			sprintf(line, CHATMGR->GetChatMsg(1438), pItemInfo->MinChub);
			pIcon->AddToolTipLine( line, TTTC_EXTRAATTR );
		}
		if( pItemInfo->ItemKind == eSHOP_ITEM_HERB )
		{
			sprintf(line, CHATMGR->GetChatMsg(1433), pItemInfo->MinChub);
			pIcon->AddToolTipLine( line, TTTC_EXTRAATTR );
		}
		//pIcon->AddToolTipLine(JACKJACK);
	}
	if( pItemInfo->CheRyuk != 0 )
	{
		if( pItemInfo->ItemKind == eSHOP_ITEM_PREMIUM )
		{
			sprintf(line, CHATMGR->GetChatMsg(1430), pItemInfo->CheRyuk);
			pIcon->AddToolTipLine( line, TTTC_EXTRAATTR );
		}
		if( pItemInfo->ItemKind == eSHOP_ITEM_CHARM || pItemInfo->ItemKind == eSHOP_ITEM_DECORATION ||
			pItemInfo->ItemKind == eSHOP_ITEM_IMAGENAME)
		{
			sprintf(line, CHATMGR->GetChatMsg(1441), pItemInfo->CheRyuk);
			pIcon->AddToolTipLine( line, TTTC_EXTRAATTR );
		}
		if( pItemInfo->ItemKind == eSHOP_ITEM_HERB )
		{
			sprintf(line, CHATMGR->GetChatMsg(1399), pItemInfo->CheRyuk);
			pIcon->AddToolTipLine( line, TTTC_EXTRAATTR );
		}
		if( pItemInfo->ItemKind == eSHOP_ITEM_INCANTATION )
		{
			sprintf(line, CHATMGR->GetChatMsg(1396), pItemInfo->CheRyuk);
			pIcon->AddToolTipLine( line, TTTC_EXTRAATTR );
		}
		//pIcon->AddToolTipLine(JACKJACK);
	}
	if( pItemInfo->CheRyuk == 1 )
	{
		if( pItemInfo->ItemKind == eSHOP_ITEM_SUNDRIES )
		{
			sprintf(line, CHATMGR->GetChatMsg(1402));
			pIcon->AddToolTipLine( line, TTTC_EXTRAATTR );
		}
		//pIcon->AddToolTipLine(JACKJACK);
	}
	if( pItemInfo->SimMek != 0 )
	{
		if( pItemInfo->ItemKind == eSHOP_ITEM_PREMIUM )
		{
			sprintf(line, CHATMGR->GetChatMsg(1431), pItemInfo->SimMek);
			pIcon->AddToolTipLine( line, TTTC_EXTRAATTR );
		}
		if( pItemInfo->ItemKind == eSHOP_ITEM_CHARM || pItemInfo->ItemKind == eSHOP_ITEM_DECORATION ||
			pItemInfo->ItemKind == eSHOP_ITEM_IMAGENAME)
		{
			sprintf(line, CHATMGR->GetChatMsg(1439), pItemInfo->SimMek);
			pIcon->AddToolTipLine( line, TTTC_EXTRAATTR );
		}
		if( pItemInfo->ItemKind == eSHOP_ITEM_HERB )
		{
			sprintf(line, CHATMGR->GetChatMsg(1421), pItemInfo->SimMek);
			pIcon->AddToolTipLine( line, TTTC_EXTRAATTR );
		}
		//pIcon->AddToolTipLine(JACKJACK);
	}
	if( pItemInfo->Life != 0 )
	{
		if( pItemInfo->ItemKind == eSHOP_ITEM_CHARM )
		{
			sprintf(line, CHATMGR->GetChatMsg(1397), pItemInfo->Life);
			pIcon->AddToolTipLine( line, TTTC_EXTRAATTR );
		}
		if( pItemInfo->ItemKind == eSHOP_ITEM_HERB || pItemInfo->ItemKind == eSHOP_ITEM_DECORATION ||
			pItemInfo->ItemKind == eSHOP_ITEM_IMAGENAME)
		{
			sprintf(line, CHATMGR->GetChatMsg(1419), pItemInfo->Life);
			pIcon->AddToolTipLine( line, TTTC_EXTRAATTR );
		}
		if( pItemInfo->ItemKind == eSHOP_ITEM_INCANTATION )
		{
			sprintf(line, CHATMGR->GetChatMsg(1417), pItemInfo->Life);
			pIcon->AddToolTipLine( line, TTTC_EXTRAATTR );
		}
		//pIcon->AddToolTipLine(JACKJACK);
	}
	if( pItemInfo->Shield != 0 )
	{
		if( pItemInfo->ItemKind == eSHOP_ITEM_CHARM )
		{
			sprintf(line, CHATMGR->GetChatMsg(1416), pItemInfo->Shield);
			pIcon->AddToolTipLine( line, TTTC_EXTRAATTR );
		}
		if( pItemInfo->ItemKind == eSHOP_ITEM_HERB || pItemInfo->ItemKind == eSHOP_ITEM_DECORATION ||
			pItemInfo->ItemKind == eSHOP_ITEM_IMAGENAME)
		{
			sprintf(line, CHATMGR->GetChatMsg(1420), pItemInfo->Shield);
			pIcon->AddToolTipLine( line, TTTC_EXTRAATTR );
		}
		//pIcon->AddToolTipLine(JACKJACK);
	}
	if( pItemInfo->NaeRyuk != 0 )
	{
		if( pItemInfo->ItemKind == eSHOP_ITEM_CHARM )
		{
			sprintf(line, CHATMGR->GetChatMsg(1428), pItemInfo->NaeRyuk);
			pIcon->AddToolTipLine( line, TTTC_EXTRAATTR );
		}
		if( pItemInfo->ItemKind == eSHOP_ITEM_HERB || pItemInfo->ItemKind == eSHOP_ITEM_DECORATION ||
			pItemInfo->ItemKind == eSHOP_ITEM_IMAGENAME)
		{
			sprintf(line, CHATMGR->GetChatMsg(1418), pItemInfo->NaeRyuk);
			pIcon->AddToolTipLine( line, TTTC_EXTRAATTR );
		}
		//pIcon->AddToolTipLine(JACKJACK);
	}
	if( pItemInfo->LimitJob != 0 )
	{
		if( pItemInfo->ItemKind == eSHOP_ITEM_CHARM )
		{
			sprintf(line, CHATMGR->GetChatMsg(1422), pItemInfo->LimitJob);
			pIcon->AddToolTipLine( line, TTTC_EXTRAATTR );
		}
		if( pItemInfo->ItemKind == eSHOP_ITEM_INCANTATION  )
		{            
			sprintf(line, CHATMGR->GetChatMsg(1432), pItemInfo->LimitJob);
			pIcon->AddToolTipLine( line, TTTC_EXTRAATTR );
		}
 		if( pItemInfo->ItemKind == eSHOP_ITEM_DECORATION  ||
			pItemInfo->ItemKind == eSHOP_ITEM_IMAGENAME)
		{
			sprintf(line, CHATMGR->GetChatMsg(1397), pItemInfo->LimitJob);
			pIcon->AddToolTipLine( line, TTTC_EXTRAATTR );
		}
		//pIcon->AddToolTipLine(JACKJACK);
	}
	if( pItemInfo->LimitGender != 0 )
	{
		if( pItemInfo->ItemKind == eSHOP_ITEM_CHARM )
		{
			sprintf(line, CHATMGR->GetChatMsg(1423), pItemInfo->LimitGender);
			pIcon->AddToolTipLine( line, TTTC_EXTRAATTR );
		}
 		if( pItemInfo->ItemKind == eSHOP_ITEM_DECORATION  ||
			pItemInfo->ItemKind == eSHOP_ITEM_IMAGENAME)
		{
			sprintf(line, CHATMGR->GetChatMsg(1416), pItemInfo->LimitGender);
			pIcon->AddToolTipLine( line, TTTC_EXTRAATTR );
		}
		//pIcon->AddToolTipLine(JACKJACK);
	}
	if( pItemInfo->LimitLevel != 0 )
	{
		if( pItemInfo->ItemKind == eSHOP_ITEM_CHARM )
		{
			sprintf(line, CHATMGR->GetChatMsg(1424), pItemInfo->LimitLevel);
			pIcon->AddToolTipLine( line, TTTC_EXTRAATTR );
		}
 		if( pItemInfo->ItemKind == eSHOP_ITEM_DECORATION  ||
			pItemInfo->ItemKind == eSHOP_ITEM_IMAGENAME)
		{
			sprintf(line, CHATMGR->GetChatMsg(1426), pItemInfo->LimitLevel);
			pIcon->AddToolTipLine( line, TTTC_EXTRAATTR );
		}
		//pIcon->AddToolTipLine(JACKJACK);
	}
	if( pItemInfo->LimitGenGol != 0 )
	{
        if( pItemInfo->ItemKind == eSHOP_ITEM_CHARM )
		{
			sprintf(line, CHATMGR->GetChatMsg(1406), pItemInfo->LimitGenGol);
			pIcon->AddToolTipLine( line, TTTC_EXTRAATTR );
		}
		if( pItemInfo->ItemKind == eSHOP_ITEM_DECORATION ||
			pItemInfo->ItemKind == eSHOP_ITEM_IMAGENAME )
		{
			sprintf(line, CHATMGR->GetChatMsg(1427), pItemInfo->LimitGenGol);
			pIcon->AddToolTipLine( line, TTTC_EXTRAATTR );
		}
 		if( pItemInfo->ItemKind == eSHOP_ITEM_PET_EQUIP  )
		{
			sprintf(line, CHATMGR->GetChatMsg(1450), pItemInfo->LimitGenGol);
			pIcon->AddToolTipLine( line, TTTC_EXTRAATTR );
		}
		//pIcon->AddToolTipLine(JACKJACK);
	}
	if( pItemInfo->LimitMinChub != 0 )
	{
        if( pItemInfo->ItemKind == eSHOP_ITEM_CHARM )
		{
			sprintf(line, CHATMGR->GetChatMsg(1410), pItemInfo->LimitMinChub);
			pIcon->AddToolTipLine( line, TTTC_EXTRAATTR );
		}
		if( pItemInfo->ItemKind == eSHOP_ITEM_DECORATION  ||
			pItemInfo->ItemKind == eSHOP_ITEM_IMAGENAME)
		{
			sprintf(line, CHATMGR->GetChatMsg(1425), pItemInfo->LimitMinChub);
			pIcon->AddToolTipLine( line, TTTC_EXTRAATTR );
		}
 		if( pItemInfo->ItemKind == eSHOP_ITEM_PET_EQUIP  )
		{
			sprintf(line, CHATMGR->GetChatMsg(1451), pItemInfo->LimitMinChub);
			pIcon->AddToolTipLine( line, TTTC_EXTRAATTR );
		}
		//pIcon->AddToolTipLine(JACKJACK);
	}
	if( pItemInfo->LimitCheRyuk != 0 )
	{
        if( pItemInfo->ItemKind == eSHOP_ITEM_CHARM || eSHOP_ITEM_DECORATION ||
			pItemInfo->ItemKind == eSHOP_ITEM_IMAGENAME)
		{
			sprintf(line, CHATMGR->GetChatMsg(1400), pItemInfo->LimitCheRyuk);
			pIcon->AddToolTipLine( line, TTTC_EXTRAATTR );
		}
 		if( pItemInfo->ItemKind == eSHOP_ITEM_PET_EQUIP  )
		{
			sprintf(line, CHATMGR->GetChatMsg(1448), pItemInfo->LimitCheRyuk);
			pIcon->AddToolTipLine( line, TTTC_EXTRAATTR );
		}
		//pIcon->AddToolTipLine(JACKJACK);
	}
	if( pItemInfo->LimitSimMek != 0 )
	{
        if( pItemInfo->ItemKind == eSHOP_ITEM_CHARM )
		{
			sprintf(line, CHATMGR->GetChatMsg(1395), pItemInfo->LimitSimMek);
			pIcon->AddToolTipLine( line, TTTC_EXTRAATTR );
		}
		if( pItemInfo->ItemKind == eSHOP_ITEM_DECORATION  ||
			pItemInfo->ItemKind == eSHOP_ITEM_IMAGENAME)
		{
			sprintf(line, CHATMGR->GetChatMsg(1434), pItemInfo->LimitSimMek);
			pIcon->AddToolTipLine( line, TTTC_EXTRAATTR );
		}
 		if( pItemInfo->ItemKind == eSHOP_ITEM_PET_EQUIP  )
		{
			sprintf(line, CHATMGR->GetChatMsg(1448), pItemInfo->LimitSimMek);
			pIcon->AddToolTipLine( line, TTTC_EXTRAATTR );
		}
		//pIcon->AddToolTipLine(JACKJACK);
	}
	if( pItemInfo->ItemGrade != 0 )
	{
        if( pItemInfo->ItemKind == eSHOP_ITEM_CHARM )
		{
			sprintf(line, CHATMGR->GetChatMsg(1429), pItemInfo->ItemGrade);
			pIcon->AddToolTipLine( line, TTTC_EXTRAATTR );
		}
		if( pItemInfo->ItemKind == eSHOP_ITEM_DECORATION  ||
			pItemInfo->ItemKind == eSHOP_ITEM_IMAGENAME)
		{
			sprintf(line, CHATMGR->GetChatMsg(1394), pItemInfo->ItemGrade);
			pIcon->AddToolTipLine( line, TTTC_EXTRAATTR );
		}
 		if( pItemInfo->ItemKind == eSHOP_ITEM_PET_EQUIP  )
		{
			sprintf(line, CHATMGR->GetChatMsg(1447), pItemInfo->ItemGrade);
			pIcon->AddToolTipLine( line, TTTC_EXTRAATTR );
		}
		//pIcon->AddToolTipLine(JACKJACK);
	}
	if( pItemInfo->RangeType != 0 )
	{
        if( pItemInfo->ItemKind == eSHOP_ITEM_CHARM )
		{
			sprintf(line, CHATMGR->GetChatMsg(1452), pItemInfo->RangeType);
			pIcon->AddToolTipLine( line, TTTC_EXTRAATTR );
		}
		if( pItemInfo->ItemKind == eSHOP_ITEM_DECORATION  ||
			pItemInfo->ItemKind == eSHOP_ITEM_IMAGENAME)
		{
			sprintf(line, CHATMGR->GetChatMsg(1405), pItemInfo->RangeType);
			pIcon->AddToolTipLine( line, TTTC_EXTRAATTR );
		}
		if( pItemInfo->ItemKind == eSHOP_ITEM_PET_EQUIP  )
		{
			sprintf(line, CHATMGR->GetChatMsg(1551), pItemInfo->RangeType);
			pIcon->AddToolTipLine( line, TTTC_EXTRAATTR );
		}
		//pIcon->AddToolTipLine(JACKJACK);
	}
	if( pItemInfo->EquipKind != 0 )
	{
		if( pItemInfo->ItemKind == eSHOP_ITEM_DECORATION  ||
			pItemInfo->ItemKind == eSHOP_ITEM_IMAGENAME)
		{
			sprintf(line, CHATMGR->GetChatMsg(1393), pItemInfo->RangeType);
			pIcon->AddToolTipLine( line, TTTC_EXTRAATTR );
		}
		//pIcon->AddToolTipLine(JACKJACK);
	}
	if( pItemInfo->MeleeAttackMin != 0 )
	{
		if( pItemInfo->ItemKind == eSHOP_ITEM_DECORATION ||
			pItemInfo->ItemKind == eSHOP_ITEM_IMAGENAME )
		{
			sprintf(line, CHATMGR->GetChatMsg(1449), pItemInfo->MeleeAttackMin);
			pIcon->AddToolTipLine( line, TTTC_EXTRAATTR );
		}
		//pIcon->AddToolTipLine(JACKJACK);
	}
	if( pItemInfo->RangeAttackMin != 0 )
	{
        if( pItemInfo->ItemKind == eSHOP_ITEM_CHARM )
		{
			sprintf(line, CHATMGR->GetChatMsg(1392));
			pIcon->AddToolTipLine( line, TTTC_EXTRAATTR );
		}
		if( pItemInfo->ItemKind == eSHOP_ITEM_DECORATION ||
			pItemInfo->ItemKind == eSHOP_ITEM_IMAGENAME )
		{
			sprintf(line, CHATMGR->GetChatMsg(1401), pItemInfo->RangeAttackMin);
			pIcon->AddToolTipLine( line, TTTC_EXTRAATTR );
		}
		//pIcon->AddToolTipLine(JACKJACK);
	}
	if( pItemInfo->RangeAttackMax != 0 )
	{
        if( pItemInfo->ItemKind == eSHOP_ITEM_CHARM )
		{
			sprintf(line, CHATMGR->GetChatMsg(1394), pItemInfo->RangeAttackMax);
			pIcon->AddToolTipLine( line, TTTC_EXTRAATTR );
		}
		if( pItemInfo->ItemKind == eSHOP_ITEM_DECORATION ||
			pItemInfo->ItemKind == eSHOP_ITEM_IMAGENAME)
		{
			sprintf(line, CHATMGR->GetChatMsg(1474), pItemInfo->RangeAttackMax);
			pIcon->AddToolTipLine( line, TTTC_EXTRAATTR );
		}
		//pIcon->AddToolTipLine(JACKJACK);
	}
	if( pItemInfo->CriticalPercent != 0 )
	{
        if( pItemInfo->ItemKind == eSHOP_ITEM_CHARM )
		{
			sprintf(line, CHATMGR->GetChatMsg(1390), pItemInfo->CriticalPercent);
			pIcon->AddToolTipLine( line, TTTC_EXTRAATTR );
		}
		if( pItemInfo->ItemKind == eSHOP_ITEM_DECORATION ||
			pItemInfo->ItemKind == eSHOP_ITEM_IMAGENAME )
		{
			sprintf(line, CHATMGR->GetChatMsg(1681), pItemInfo->CriticalPercent);
			pIcon->AddToolTipLine( line, TTTC_EXTRAATTR );
		}
		//pIcon->AddToolTipLine(JACKJACK);
	}
	if( pItemInfo->PhyDef != 0 )
	{
        if( pItemInfo->ItemKind == eSHOP_ITEM_CHARM )
		{
			sprintf(line, CHATMGR->GetChatMsg(1415), pItemInfo->PhyDef);
			pIcon->AddToolTipLine( line, TTTC_EXTRAATTR );
		}
		//pIcon->AddToolTipLine(JACKJACK);
	}
	if( pItemInfo->Plus_MugongIdx != 0 )
	{
        if( pItemInfo->ItemKind == eSHOP_ITEM_CHARM )
		{
			sprintf(line, CHATMGR->GetChatMsg(1419), pItemInfo->Plus_MugongIdx);
			pIcon->AddToolTipLine( line, TTTC_EXTRAATTR );
		}
		if( pItemInfo->ItemKind == eSHOP_ITEM_DECORATION ||
			pItemInfo->ItemKind == eSHOP_ITEM_IMAGENAME )
		{
			sprintf(line, CHATMGR->GetChatMsg(1407), pItemInfo->Plus_MugongIdx);
			pIcon->AddToolTipLine( line, TTTC_EXTRAATTR );
		}
		//pIcon->AddToolTipLine(JACKJACK);
	}
	if( pItemInfo->Plus_Value != 0 )
	{
        if( pItemInfo->ItemKind == eSHOP_ITEM_CHARM )
		{
			sprintf(line, CHATMGR->GetChatMsg(1420), pItemInfo->Plus_Value);
			pIcon->AddToolTipLine( line, TTTC_EXTRAATTR );
		}
		if( pItemInfo->ItemKind == eSHOP_ITEM_DECORATION  ||
			pItemInfo->ItemKind == eSHOP_ITEM_IMAGENAME)
		{
			sprintf(line, CHATMGR->GetChatMsg(1408), pItemInfo->Plus_Value);
			pIcon->AddToolTipLine( line, TTTC_EXTRAATTR );
		}
		//pIcon->AddToolTipLine(JACKJACK);
	}
	if( pItemInfo->AllPlus_Kind != 0 )
	{
        if( pItemInfo->ItemKind == eSHOP_ITEM_CHARM )
		{
			sprintf(line, CHATMGR->GetChatMsg(1418), pItemInfo->AllPlus_Kind);
			pIcon->AddToolTipLine( line, TTTC_EXTRAATTR );
		}
		if( pItemInfo->ItemKind == eSHOP_ITEM_DECORATION  ||
			pItemInfo->ItemKind == eSHOP_ITEM_IMAGENAME)
		{
			sprintf(line, CHATMGR->GetChatMsg(1392));
			pIcon->AddToolTipLine( line, TTTC_EXTRAATTR );
		}
		//pIcon->AddToolTipLine(JACKJACK);
	}
	if( pItemInfo->NaeRyukRecover != 0 )
	{
		if( pItemInfo->ItemKind == eSHOP_ITEM_PREMIUM || pItemInfo->ItemKind == eSHOP_ITEM_CHARM ||
			pItemInfo->ItemKind == eSHOP_ITEM_INCANTATION || pItemInfo->ItemKind == eSHOP_ITEM_MAKEUP ||
			pItemInfo->ItemKind == eSHOP_ITEM_MAKEUP || pItemInfo->ItemKind == eSHOP_ITEM_DECORATION ||
			pItemInfo->ItemKind == eSHOP_ITEM_EQUIP || pItemInfo->ItemKind == eSHOP_ITEM_PET_EQUIP ||
			pItemInfo->ItemKind == eSHOP_ITEM_IMAGENAME)
		{
			sprintf(line, CHATMGR->GetChatMsg(1682), pItemInfo->NaeRyukRecover);
			pIcon->AddToolTipLine( line, TTTC_EXTRAATTR );
		}
		//pIcon->AddToolTipLine(JACKJACK);
	}
	/*if( pItemInfo->BuyPrice  == 1 )
	{
	}
	if( pItemInfo->BuyPrice  == 0 )
	{
	}
	if( pItemInfo->MeleeAttackMax == 0 )
	{
	}
	if( pItemInfo->MeleeAttackMax == 1 )
	{
	}*/
	/*if(pItemInfo!=NULL)
	{
		pIcon->AddToolTipLine(JACKJACK);
	}*/
	if( pItemInfo->GenGol != 0 ||
	 pItemInfo->MinChub != 0||
	 pItemInfo->CheRyuk != 0||
	 pItemInfo->CheRyuk == 1||
	 pItemInfo->SimMek != 0||
	 pItemInfo->Life != 0||
	 pItemInfo->Shield != 0||
	 pItemInfo->NaeRyuk != 0||
	 pItemInfo->LimitJob != 0||
	 pItemInfo->LimitGender != 0||
	 pItemInfo->LimitLevel != 0||
	 pItemInfo->LimitGenGol != 0||
	 pItemInfo->LimitMinChub != 0||
	 pItemInfo->LimitCheRyuk != 0||
	 pItemInfo->LimitSimMek != 0||
	 pItemInfo->ItemGrade != 0||
	 pItemInfo->RangeType != 0||
	 pItemInfo->EquipKind != 0||
	 pItemInfo->MeleeAttackMin != 0||
	 pItemInfo->RangeAttackMin != 0||
	 pItemInfo->RangeAttackMax != 0||
	 pItemInfo->CriticalPercent != 0||
	 pItemInfo->PhyDef != 0||
	 pItemInfo->Plus_MugongIdx != 0||
	 pItemInfo->Plus_Value != 0||
	 pItemInfo->AllPlus_Kind != 0||
	 pItemInfo->NaeRyukRecover != 0
	 )
	 pIcon->AddToolTipLine(JACKJACK);
	//天墨技术团 PVP妮┦陪ボ
	float fPVP = 100 * pItemInfo->PVPCri;
	if (fPVP != 0)
	{
		pIcon->AddToolTipLine("");   // 添一空行
		sprintf(line, "< %s>", CHATMGR->GetChatMsg(2735));
		pIcon->AddToolTipLine(line, RGB_HALF(255, 0, 0));
		sprintf(line, CHATMGR->GetChatMsg(2729), (int)fPVP);
		pIcon->AddToolTipLine(line, RGB_HALF(0, 255, 0));
	}
	fPVP = 100 * pItemInfo->PVPAttack;
	if (fPVP != 0)
	{
		sprintf(line, CHATMGR->GetChatMsg(2730), (int)fPVP);
		pIcon->AddToolTipLine(line, RGB_HALF(0, 255, 0));
	}
	fPVP = 100 * pItemInfo->PVPDef;
	if (fPVP != 0)
	{
		sprintf(line, CHATMGR->GetChatMsg(2731), (int)fPVP);
		pIcon->AddToolTipLine(line, RGB_HALF(0, 255, 0));
	}
	fPVP = 100 * pItemInfo->PVPADef;
	if (fPVP != 0)
	{
		sprintf(line, CHATMGR->GetChatMsg(2732), (int)fPVP);
		pIcon->AddToolTipLine(line, RGB_HALF(0, 255, 0));
	}
	fPVP = 100 * pItemInfo->PVPHit;
	if (fPVP != 0)
	{
		sprintf(line, CHATMGR->GetChatMsg(2733), (int)fPVP);
		pIcon->AddToolTipLine(line, RGB_HALF(0, 255, 0));
	}
	fPVP = 100 * pItemInfo->PVPADodge;
	if (fPVP != 0)
	{
		sprintf(line, CHATMGR->GetChatMsg(2734), (int)fPVP);
		pIcon->AddToolTipLine(line, RGB_HALF(0, 255, 0));
	}
	fPVP = 100 * pItemInfo->PVPStunResist;
	if (fPVP != 0)
	{
		sprintf(line, CHATMGR->GetChatMsg(2736), (int)fPVP);
		pIcon->AddToolTipLine(line, RGB_HALF(0, 255, 0));
	}
	fPVP = 100 * pItemInfo->PVPStunTimeReduce;
	if (fPVP != 0)
	{
		sprintf(line, CHATMGR->GetChatMsg(2737), (int)fPVP);
		pIcon->AddToolTipLine(line, RGB_HALF(0, 255, 0));
	}
	//[分割线][2017/11/28]

	
}
void CItemManager::AddAvatarItemOptionTooltip( cIcon* pIcon, ITEM_INFO* pItemInfo )
{
}
BOOL CItemManager::CheckItemStage( BYTE bItemStage )
{
	if( bItemStage == eItemStage_Normal )		return TRUE;
	BYTE stage = HERO->GetStage();
	if( stage == eStage_Normal )				return FALSE;
	if( bItemStage == eItemStage_ChangeStage )	return TRUE;
	switch( stage )
	{
	case eStage_Hwa:
		{
			if( bItemStage == eItemStage_OverHwa || bItemStage == eItemStage_Hwa )
				return TRUE;
		}
		break;
	case eStage_Hyun:
		{
			if( bItemStage == eItemStage_ChangeStage2 || bItemStage == eItemStage_OverHwa || bItemStage == eItemStage_Hyun )
				return TRUE;
		}
		break;
	case eStage_Geuk:
		{
			if( bItemStage == eItemStage_OverGeuk || bItemStage == eItemStage_Geuk )
				return TRUE;
		}
		break;
	case eStage_Tal:
		{
			if( bItemStage == eItemStage_ChangeStage2 || bItemStage == eItemStage_OverGeuk || bItemStage == eItemStage_Tal )
				return TRUE;
		}
		break;
	}
	return FALSE;
}
BOOL CItemManager::LoadRareItemInfo()
{
	CMHFile fp;
	char szBuf[256] = {0,};
	char line[512];
	char FileName[256];
#ifdef _FILE_BIN_
	sprintf(FileName, "Resource/Item_RareItemInfo.bin");
	if( !fp.Init(FileName,"rb") ) return FALSE;
#else
	sprintf(FileName, "Resource/Item_RareItemInfo.txt");
	if( !fp.Init(FileName,"rt") ) return FALSE;
#endif
	while( !fp.IsEOF() )
	{
		fp.GetString(szBuf);
		if( szBuf[0] == '@' )
		{
			fp.GetLineX(line,512);
			continue;
		}
		if( szBuf[0] == '*' )
		{
			sRareItemInfo* pRareItemInfo = new sRareItemInfo;
			pRareItemInfo->ItemIdx = fp.GetDword();
			pRareItemInfo->RareProb = fp.GetDword();
			ASSERT(!m_RareItemInfoTable.GetData(pRareItemInfo->ItemIdx));
			m_RareItemInfoTable.Add(pRareItemInfo, pRareItemInfo->ItemIdx);
		}
	}
	fp.Release();
	return TRUE;
}
void CItemManager::SetIconColorFilter( DWORD dwDBIdx, DWORD clrRGBA )
{
	CItem* pItem = GetItem(dwDBIdx);
	pItem->SetImageRGB(clrRGBA);
}
void CItemManager::SetIconColorFilter( cIcon* pIcon, DWORD clrRGBA )
{
	pIcon->SetImageRGB(clrRGBA);
}
#ifdef _JAPAN_LOCAL_
BOOL CItemManager::CheckItemAttr( WORD wItemAttr )
{
	if( wItemAttr == eIA_All )	return TRUE;
	int nMainAttr = HERO->GetMainCharAttr();
	int	nSubAttr = HERO->GetSubCharAttr();
	int nRelation = wItemAttr - 10;
	if( nRelation > 0 )
	{
		if( nMainAttr == nRelation )	return TRUE;
	}
	else
	{
		if( nMainAttr == wItemAttr || nSubAttr == wItemAttr )	return TRUE;
	}
	return FALSE;
}
BOOL CItemManager::CheckItemAquireSkill( WORD wAcquireSkill1, WORD wAcquireSkill2 )
{
	if( wAcquireSkill1 == 0 )	return TRUE;
	CMugongBase* pMugong = NULL;
	for( int i = wAcquireSkill1; i < wAcquireSkill2+1; ++i )
	{
		pMugong = MUGONGMGR->GetMugongByMugongIdx( i );
		if( pMugong )
		if( pMugong->GetSung() > 0 )
			return TRUE;
	}
	return FALSE;
}
#endif	
void CItemManager::CheckInvenFullForAlert(int flg)
{
#ifndef TAIWAN_LOCAL
#ifndef _JAPAN_LOCAL_
#ifndef _HK_LOCAL_
#ifndef _TL_LOCAL_
	return;
#endif
#endif
#endif
#endif
	CInventoryExDialog* pInven = GAMEIN->GetInventoryDialog();
	CItemShopInven* pSInven = pInven->GetShopInven();
	cShopItemInventPlusDlg* pSInvenPlus = pInven->GetShopInvenPlus();
	char buf[256] = {0,};
	if( flg & eCBS_Inven )
	{
		if( 0 == pInven->GetBlankNum() )
		{
			sprintf( buf, "%s %s", CHATMGR->GetChatMsg(1481), CHATMGR->GetChatMsg(122) );
			CHATMGR->AddMsg(CTC_ALERT_YELLOW, buf );
			WINDOWMGR->MsgBox( MBI_INVEN_FULL_ALERT, MBT_OK, CHATMGR->GetChatMsg(122) );
		}
	}
	if( flg & eCBS_SInven )
	{
		if( 0 == pSInven->GetBlankNum() )
		{
			sprintf( buf, "%s %s", CHATMGR->GetChatMsg(1481), CHATMGR->GetChatMsg(1480) );
			CHATMGR->AddMsg(CTC_ALERT_YELLOW, buf );
			WINDOWMGR->MsgBox( MBI_SHOPINVEN_FULL_ALERT, MBT_OK, CHATMGR->GetChatMsg(1480) );
		}
	}
	if(flg&eCBS_SInvenPlus)
	{
		if( 0 == pSInvenPlus->GetBlankNum() )
		{
			sprintf( buf, "%s %s", CHATMGR->GetChatMsg(1481), CHATMGR->GetChatMsg(1480) );
			CHATMGR->AddMsg(CTC_ALERT_YELLOW, buf );
			WINDOWMGR->MsgBox( MBI_SHOPINVEN_FULL_ALERT, MBT_OK, CHATMGR->GetChatMsg(1480) );
		}
	}
}
void CItemManager::GetItemOptionsAndToolTipFromInfoMsg( cIcon* pIcon, MSG_LINKITEMOPTIONS* pOptionsMsg )
{
	CAddableInfoIterator iter(&pOptionsMsg->AddableInfo);
	BYTE AddInfoKind;
	ITEM_OPTION_INFO OptionInfo;
	memset(&OptionInfo,0,sizeof(OptionInfo));
	ITEM_RARE_OPTION_INFO RareOptionInfo;
	ITEM_STONE_OPTION_INFO StoneOptionInfo;    
    memset(&StoneOptionInfo,0,sizeof(StoneOptionInfo));
	PET_TOTALINFO PetInfo;
	TITAN_TOTALINFO TitanInfo;
	TITAN_ENDURANCE_ITEMINFO ItemInfo;
	while( (AddInfoKind = iter.GetInfoKind()) != CAddableInfoList::None )
	{
		switch(AddInfoKind)
		{
		case(CAddableInfoList::ItemOption):
			{
				iter.GetInfoData(&OptionInfo);
				ITEMMGR->AddItemOption(&OptionInfo);
			}
			break;
		case(CAddableInfoList::ItemRareOption):
			{
				iter.GetInfoData(&RareOptionInfo);
				ITEMMGR->AddItemRareOption(&RareOptionInfo);
			}
			break;
		case(CAddableInfoList::ItemStoneOption):       
			{
				iter.GetInfoData(&StoneOptionInfo);
				ITEMMGR->AddItemStoneOption(&StoneOptionInfo);
			}
			break;
		case(CAddableInfoList::PetTotalInfo):
			{
				iter.GetInfoData(&PetInfo);
				PETMGR->AddPetInfo(&PetInfo);
			}
			break;
		case(CAddableInfoList::TitanTotalInfo):
			{
				iter.GetInfoData(&TitanInfo);
				TITANMGR->AddTitanInfo(&TitanInfo);
			}
			break;
		case(CAddableInfoList::TitanEndrncInfo):
			{
				iter.GetInfoData(&ItemInfo);
				TITANMGR->AddTitanEquipItemEnduranceInfo(&ItemInfo);
			}
			break;
		}
		iter.ShiftToNextData();
	}
	int optionsKind = pOptionsMsg->eOptionKind;
	if(optionsKind & eOPTS_ItemOption)
	{
		SetToolTipIcon( pIcon, &OptionInfo, NULL,0,NULL); 
	}
	if(optionsKind & eOPTS_ItemRareOption)
	{
		SetToolTipIcon( pIcon, &OptionInfo, &RareOptionInfo,0,NULL);
	}
	if(optionsKind & eOPTS_ItemStoneOption) 
	{
        SetToolTipIcon( pIcon, &OptionInfo, &RareOptionInfo,0,&StoneOptionInfo);
	}
	if(optionsKind & eOPTS_PetTotalInfo)
	{
		SetToolTipIcon( pIcon, NULL, NULL, pOptionsMsg->ItemInfo.dwDBIdx );
	}
	if(optionsKind & eOPTS_TitanTotalInfo)
	{
		SetToolTipIcon( pIcon );
	}
	if(optionsKind & eOPTS_TitanEquipEndurance)
	{
		SetToolTipIcon( pIcon );
	}
	if(0 == optionsKind)
	{
		SetToolTipIcon( pIcon );
	}
}
BOOL CItemManager::LoadSetItemOption()
{
	CMHFile file;
#ifdef _FILE_BIN_
	if(!file.Init("Resource/setitem_opt.bin", "rb"))
		return FALSE;
#else
	if(!file.Init("Resource/setitem_opt.txt", "rt"))
		return FALSE;
#endif	
	SET_ITEM_OPTION* pInfo = NULL;
	while(!file.IsEOF())
	{
		ASSERT(!pInfo);
		pInfo = new SET_ITEM_OPTION;
		pInfo->wIndex = file.GetWord();
		pInfo->wSetItemKind = file.GetWord();
		SafeStrCpy( pInfo->szSetItemName, file.GetString(), MAX_NAME_LENGTH+1 );
		pInfo->wGenGol = file.GetWord();
		pInfo->wMinChub = file.GetWord();
		pInfo->wCheRyuk = file.GetWord();
		pInfo->wSimMek = file.GetWord();
		pInfo->dwLife = file.GetDword();
		pInfo->dwShield = file.GetDword();
		pInfo->dwNaeRyuk = file.GetDword();
		pInfo->AttrRegist.SetElement_Val(ATTR_FIRE,file.GetFloat());
		pInfo->AttrRegist.SetElement_Val(ATTR_WATER,file.GetFloat());
		pInfo->AttrRegist.SetElement_Val(ATTR_TREE,file.GetFloat());
		pInfo->AttrRegist.SetElement_Val(ATTR_IRON,file.GetFloat());
		pInfo->AttrRegist.SetElement_Val(ATTR_EARTH,file.GetFloat());
		pInfo->wLimitLevel = file.GetWord();
		SafeStrCpy( pInfo->szEffect, file.GetString(), 100 );		 
        pInfo->wLimitGenGol = file.GetWord();
		pInfo->wLimitMinChub = file.GetWord();
		pInfo->wLimitCheRyuk = file.GetWord();
		pInfo->wLimitSimMek = file.GetWord();
		pInfo->wMeleeAttackMin = file.GetWord();
		pInfo->wMeleeAttackMax = file.GetWord();
		pInfo->wRangeAttackMin = file.GetWord();
		pInfo->wRangeAttackMax = file.GetWord();
		pInfo->wCriticalPercent = file.GetWord();        		
		pInfo->AttrAttack.SetElement_Val(ATTR_FIRE,file.GetFloat());
		pInfo->AttrAttack.SetElement_Val(ATTR_WATER,file.GetFloat());
		pInfo->AttrAttack.SetElement_Val(ATTR_TREE,file.GetFloat());
		pInfo->AttrAttack.SetElement_Val(ATTR_IRON,file.GetFloat());
		pInfo->AttrAttack.SetElement_Val(ATTR_EARTH,file.GetFloat());
		pInfo->wPhyDef = file.GetWord();
		pInfo->wLifeRecover = file.GetWord();
		pInfo->fLifeRecoverRate = file.GetFloat();
		pInfo->wNaeRyukRecover = file.GetWord();
		pInfo->fNaeRyukRecoverRate = file.GetFloat();
		pInfo->wSetValue = file.GetWord();
		pInfo->wApplicationValue = file.GetWord();
		ASSERT(!m_SetItemOptionList.GetData(pInfo->wIndex));
		m_SetItemOptionList.Add(pInfo, pInfo->wIndex);
		pInfo = NULL;
	}
	file.Release();
	return TRUE;
}
SET_ITEM_OPTION* CItemManager::GetSetItemOption(WORD wSetItemKind, WORD wSetValue)
{
	SET_ITEM_OPTION* pSetItemOption = NULL;
	m_SetItemOptionList.SetPositionHead();
	while(pSetItemOption = m_SetItemOptionList.GetData())
	{
		if(pSetItemOption->wSetItemKind == wSetItemKind)
		{
			if(wSetValue == pSetItemOption->wSetValue)
			{				
				return pSetItemOption;
			}
		}
	}
	return NULL;
}
SET_ITEM_OPTION* CItemManager::GetSetItemOption(WORD wSetItemKind)
{
	SET_ITEM_OPTION* pSetItemOption = NULL;
	m_SetItemOptionList.SetPositionHead();
	while(pSetItemOption = m_SetItemOptionList.GetData())
	{
		if(pSetItemOption->wSetItemKind == wSetItemKind)
		{
			if(pSetItemOption->wSetValue == pSetItemOption->wApplicationValue)
			{
				return pSetItemOption;
			}
		}
	}
	return NULL;
}
void CItemManager::RemoveSetItemOption(WORD wIndex, SET_ITEM_OPTION* pSetItemOptionOut)
{
	SET_ITEM_OPTION* pInfo = m_SetItemOptionList.GetData(wIndex);
	if( pInfo == NULL )
	{
		ASSERT(0);		
		return;
	}
	if(pSetItemOptionOut)
		*pSetItemOptionOut = *pInfo;
	SAFE_DELETE( pInfo );
	m_SetItemOptionList.Remove(wIndex);
}
void CItemManager::AddSetItemToolTip(cIcon* pIcon, ITEM_INFO* pItemInfo )
{
	char line[128];	
	DWORD dwSetItemCount = 0;
	dwSetItemCount = GetSetItemEquipValue(pItemInfo);	
	m_SetItemOptionList.SetPositionHead();	
	SET_ITEM_OPTION* pSetItemOption = NULL;
	while( pSetItemOption = m_SetItemOptionList.GetData() )
	{
		if(pSetItemOption->wSetItemKind == pItemInfo->wSetItemKind)
		{
			WORD wSetValue = pSetItemOption->wSetValue;            
			sprintf( line, CHATMGR->GetChatMsg(1577), wSetValue);
			pIcon->AddToolTipLine(line);
			if(dwSetItemCount >= wSetValue)
			{
				SetSetItemToolTip( pIcon, pItemInfo, TRUE, wSetValue );
			}
			else
			{
				SetSetItemToolTip( pIcon, pItemInfo, FALSE, wSetValue );
			}
		}		
	}
}
DWORD CItemManager::GetSetItemEquipValue(ITEM_INFO* pItemInfo)
{
	CItem* pItem = NULL;
	DWORD dwSetItemCount = 0;
	for(unsigned int ui = TP_WEAR_START ; ui < TP_WEAR_END ; ui++)
	{
		pItem = GAMEIN->GetInventoryDialog()->GetItemForPos( ui );
		if( pItem )
		{
			if(pItem->GetItemInfo()->wSetItemKind == pItemInfo->wSetItemKind)
			{
				dwSetItemCount++;
			}		
		}
		pItem = NULL;
	}
	return dwSetItemCount;
}
void CItemManager::SetSetItemToolTip( cIcon* pIcon, ITEM_INFO* pItemInfo, BOOL bSetItemEnable, WORD wSetValue)
{
	char line[128];
	DWORD dwColor;
	float attrvalue = 0;	
	if(bSetItemEnable == TRUE)
	{
		dwColor = TTTC_SETITEMOPTION_ENABLE;
	}
	else
	{
		dwColor = TTTC_SETITEMOPTION_DISABLE;
	}
	SET_ITEM_OPTION* pSetItemOption = NULL;	
	pSetItemOption = GetSetItemOption(pItemInfo->wSetItemKind, wSetValue);
	if(pSetItemOption == NULL)
	{
		return;
	}	
	if( pSetItemOption->wGenGol != 0 )
	{
		sprintf(line, "%s +%d", CHATMGR->GetChatMsg(382), pSetItemOption->wGenGol);		
		pIcon->AddToolTipLine( line, dwColor );
	}
	if( pSetItemOption->wMinChub != 0 )
	{
		sprintf(line, "%s +%d", CHATMGR->GetChatMsg(383), pSetItemOption->wMinChub);
		pIcon->AddToolTipLine( line, dwColor );
	}
	if( pSetItemOption->wCheRyuk != 0 )
	{
		sprintf(line, "%s +%d", CHATMGR->GetChatMsg(384), pSetItemOption->wCheRyuk);
		pIcon->AddToolTipLine( line, dwColor );
	}
	if( pSetItemOption->wSimMek != 0 )
	{
		sprintf(line, "%s +%d", CHATMGR->GetChatMsg(385), pSetItemOption->wSimMek);
		pIcon->AddToolTipLine( line, dwColor );
	}
	if( pSetItemOption->dwLife != 0 )
	{
		sprintf(line, "%s +%d", CHATMGR->GetChatMsg(386), pSetItemOption->dwLife);
		pIcon->AddToolTipLine( line, dwColor );
	}
	if( pSetItemOption->dwShield != 0 )
	{
		sprintf(line, "%s +%d", CHATMGR->GetChatMsg(388), pSetItemOption->dwShield);
		pIcon->AddToolTipLine( line, dwColor );
	}
	if( pSetItemOption->dwNaeRyuk != 0 )
	{
		sprintf(line, "%s +%d", CHATMGR->GetChatMsg(387), pSetItemOption->dwNaeRyuk);
		pIcon->AddToolTipLine( line, dwColor );
	}
	for(int i = ATTR_FIRE; i <= ATTR_MAX; ++i )
	{
		attrvalue = pSetItemOption->AttrRegist.GetElement_Val(i);
		if(attrvalue != 0)
		{
			sprintf( line, "%s +%d%%", CHATMGR->GetChatMsg(265+i-1), (int) (attrvalue * 100) );
			pIcon->AddToolTipLine( line, dwColor );
		}
		attrvalue = 0;
	}
	if( pSetItemOption->wMeleeAttackMin != 0 || pSetItemOption->wMeleeAttackMax != 0)
	{
		sprintf( line, "%s %d ~ %d", CHATMGR->GetChatMsg(389), pSetItemOption->wMeleeAttackMin, pSetItemOption->wMeleeAttackMax );
		pIcon->AddToolTipLine( line, dwColor );
	}	
	if( pSetItemOption->wRangeAttackMin != 0 || pSetItemOption->wRangeAttackMax != 0)
	{
		sprintf( line, "%s %d ~ %d", CHATMGR->GetChatMsg(391), pSetItemOption->wRangeAttackMin, pSetItemOption->wRangeAttackMax );
		pIcon->AddToolTipLine( line, dwColor );
	}	
	if( pSetItemOption->wCriticalPercent != 0 )
	{
		sprintf(line, "%s +%d", CHATMGR->GetChatMsg(390), pSetItemOption->wCriticalPercent);
		pIcon->AddToolTipLine( line, dwColor );
	}
	for(int i = ATTR_FIRE; i <= ATTR_MAX; ++i )
	{
		attrvalue = pSetItemOption->AttrAttack.GetElement_Val(i);
		if(attrvalue != 0)
		{
			sprintf( line, "%s +%d%%", CHATMGR->GetChatMsg(392+i-1), (int) (attrvalue * 100) );
			pIcon->AddToolTipLine( line, dwColor );
		}
		attrvalue = 0;
	}		
	if( pSetItemOption->wPhyDef != 0 )
	{
		sprintf(line, "%s +%d", CHATMGR->GetChatMsg(397), pSetItemOption->wPhyDef);
		pIcon->AddToolTipLine( line, dwColor );
	}
	if(pSetItemOption->wLifeRecover != 0)
	{
		sprintf(line, CHATMGR->GetChatMsg(1564), pSetItemOption->wLifeRecover);
		pIcon->AddToolTipLine( line, dwColor );
	}
	if(pSetItemOption->fLifeRecoverRate != 0)
	{
		sprintf(line, CHATMGR->GetChatMsg(1566), (WORD)(pSetItemOption->fLifeRecoverRate * 100));
		pIcon->AddToolTipLine( line, dwColor );
	}
	if(pSetItemOption->wNaeRyukRecover != 0)
	{
		sprintf(line, CHATMGR->GetChatMsg(1565), pSetItemOption->wNaeRyukRecover);
		pIcon->AddToolTipLine( line, dwColor );
	}
	if(pSetItemOption->fNaeRyukRecoverRate != 0)
	{
		sprintf(line, CHATMGR->GetChatMsg(1567), (WORD)(pSetItemOption->fNaeRyukRecoverRate * 100));
		pIcon->AddToolTipLine( line, dwColor );
	}
}
CYHHashTable<SET_ITEM_OPTION>* CItemManager::GetSetItemOptionList()
{
	return &m_SetItemOptionList;
}
void CItemManager::SetItemEffectProcess(CPlayer* pPlayer)
{
	SET_ITEM_INFO setItemInfo[MAX_SETITEM_KIND_NUM];
	memset(setItemInfo, 0, sizeof(SET_ITEM_INFO) * MAX_SETITEM_KIND_NUM);		
	if( (pPlayer->InTitan() == TRUE) && (pPlayer->GetSetItemEffectID() != 0) )
	{
		pPlayer->RemoveObjectEffect( pPlayer->GetSetItemEffectID() );
		pPlayer->SetSetItemEffectID( 0 );							
		return;
	}
	CHARACTER_TOTALINFO* pCharacterinfo = pPlayer->GetCharacterTotalInfo();
	for(unsigned int i = 0 ; i < eWearedItem_Max ; i++)
	{
		WORD wWearedItemIdx = pCharacterinfo->WearedItemIdx[i];
		ITEM_INFO* pItemInfo = GetItemInfo(wWearedItemIdx);
		if( !pItemInfo )
			continue;
		if(pItemInfo->wSetItemKind != 0)
		{
			for(unsigned int ui = 0 ; ui < MAX_SETITEM_KIND_NUM ; ui++)
			{
				if(setItemInfo[ui].wKind == pItemInfo->wSetItemKind)
				{
					setItemInfo[ui].wCount++;
					break;
				}
				else
				{
					if(setItemInfo[ui].wKind == 0)
					{
						setItemInfo[ui].wKind = pItemInfo->wSetItemKind;
						setItemInfo[ui].wCount++;
						break;
					}					
				}				
			}
		}
	}	
	SET_ITEM_OPTION* pSetItemOption = NULL;		
	for(unsigned int ui = 0 ; ui < MAX_SETITEM_KIND_NUM ; ui++)
	{
		if(setItemInfo[ui].wCount != 0)
		{			
			pSetItemOption = GetSetItemOption(setItemInfo[ui].wKind);
			if(pSetItemOption != NULL)
			{
				if( setItemInfo[ui].wCount == pSetItemOption->wSetValue )
				{
					if( pPlayer->GetSetItemEffectID() != SETITEM_STARTEFFECTID+pSetItemOption->wIndex )
					{
						if( pPlayer->GetSetItemEffectID() != 0 )
						{							
							pPlayer->RemoveObjectEffect( pPlayer->GetSetItemEffectID() );
							pPlayer->SetSetItemEffectID( 0 );							
						}
						if(pPlayer->InTitan() == FALSE)
						{
							OBJECTEFFECTDESC desc(FindEffectNum(pSetItemOption->szEffect));
							pPlayer->AddObjectEffect( SETITEM_STARTEFFECTID+pSetItemOption->wIndex, &desc, 1 );
							pPlayer->SetSetItemEffectID( SETITEM_STARTEFFECTID+pSetItemOption->wIndex );
						}						
					}
				}
				else
				{
					if( pPlayer->GetSetItemEffectID() == SETITEM_STARTEFFECTID+pSetItemOption->wIndex )
					{
						pPlayer->RemoveObjectEffect( pPlayer->GetSetItemEffectID() );
						pPlayer->SetSetItemEffectID( 0 );
					}
				}
			}		
		}		
	}	
}
void CItemManager::AddUniqueItemToolTip(cIcon* pIcon, ITEM_INFO* pItemInfo )
{
	char line[128];	
	char buf[256] = { 0, };
	UNIQUE_ITEM_OPTION_INFO* pUniqueInfo = GAMERESRCMNGR->GetUniqueItemOptionList(pItemInfo->ItemIdx);
	if( !pUniqueInfo )
	{
		return;
	}
	pIcon->AddToolTipLine("");
	if(pUniqueInfo->MixFlag == 1)
	{
		sprintf(line, CHATMGR->GetChatMsg(1609));
		pIcon->AddToolTipLine( line, TTTC_MUGONGKIND );
	}
	else
	{
		sprintf(line, CHATMGR->GetChatMsg(1610));
		pIcon->AddToolTipLine( line, TTCLR_ENERMY );
	}
	sprintf(line, CHATMGR->GetChatMsg(1611), pUniqueInfo->nHp);
	if(pUniqueInfo->nHp > 0)
		pIcon->AddToolTipLine( line, TTTC_MUGONGKIND );
	else if( pUniqueInfo->nHp < 0)
		pIcon->AddToolTipLine( line, TTCLR_ENERMY );
	sprintf(line, CHATMGR->GetChatMsg(1612), pUniqueInfo->nMp);
	if(pUniqueInfo->nMp > 0)
		pIcon->AddToolTipLine( line, TTTC_MUGONGKIND );
	else if( pUniqueInfo->nMp < 0)
		pIcon->AddToolTipLine( line, TTCLR_ENERMY );
	sprintf(line, CHATMGR->GetChatMsg(1613), pUniqueInfo->nShield);
	if(pUniqueInfo->nShield > 0)
		pIcon->AddToolTipLine( line, TTTC_MUGONGKIND );
	else if( pUniqueInfo->nShield < 0)
		pIcon->AddToolTipLine( line, TTCLR_ENERMY );
	sprintf(line, CHATMGR->GetChatMsg(1614), pUniqueInfo->nPhyDamage);
	if(pUniqueInfo->nPhyDamage > 0)
		pIcon->AddToolTipLine( line, TTTC_MUGONGKIND );
	else if( pUniqueInfo->nPhyDamage < 0)
		pIcon->AddToolTipLine( line, TTCLR_ENERMY );
	sprintf(line, CHATMGR->GetChatMsg(1615), pUniqueInfo->nCriDamage);
	if(pUniqueInfo->nCriDamage > 0)
		pIcon->AddToolTipLine( line, TTTC_MUGONGKIND );
	else if( pUniqueInfo->nCriDamage < 0)
		pIcon->AddToolTipLine( line, TTCLR_ENERMY );
	sprintf(line, CHATMGR->GetChatMsg(1616), pUniqueInfo->nCriRate);
	if(pUniqueInfo->nCriRate > 0)
		pIcon->AddToolTipLine( line, TTTC_MUGONGKIND );
	else if( pUniqueInfo->nCriRate < 0)
		pIcon->AddToolTipLine( line, TTCLR_ENERMY );
	sprintf(line, CHATMGR->GetChatMsg(1617), pUniqueInfo->nGengol);
	if(pUniqueInfo->nGengol > 0)
		pIcon->AddToolTipLine( line, TTTC_MUGONGKIND );
	else if( pUniqueInfo->nGengol < 0)
		pIcon->AddToolTipLine( line, TTCLR_ENERMY );
	sprintf(line, CHATMGR->GetChatMsg(1618), pUniqueInfo->nMinChub);
	if(pUniqueInfo->nMinChub > 0)
		pIcon->AddToolTipLine( line, TTTC_MUGONGKIND );
	else if( pUniqueInfo->nMinChub < 0)
		pIcon->AddToolTipLine( line, TTCLR_ENERMY );
	sprintf(line, CHATMGR->GetChatMsg(1619), pUniqueInfo->nCheRyuk);
	if(pUniqueInfo->nCheRyuk > 0)
		pIcon->AddToolTipLine( line, TTTC_MUGONGKIND );
	else if( pUniqueInfo->nCheRyuk < 0)
		pIcon->AddToolTipLine( line, TTCLR_ENERMY );
	sprintf(line, CHATMGR->GetChatMsg(1620), pUniqueInfo->nSimMek);
	if(pUniqueInfo->nSimMek > 0)
		pIcon->AddToolTipLine( line, TTTC_MUGONGKIND );
	else if( pUniqueInfo->nSimMek < 0)
		pIcon->AddToolTipLine( line, TTCLR_ENERMY );
	sprintf(line, CHATMGR->GetChatMsg(1621), pUniqueInfo->nDefen);
	if(pUniqueInfo->nDefen > 0)
		pIcon->AddToolTipLine( line, TTTC_MUGONGKIND );
	else if( pUniqueInfo->nDefen < 0)
		pIcon->AddToolTipLine( line, TTCLR_ENERMY );
	sprintf(line, CHATMGR->GetChatMsg(1622), pUniqueInfo->nRange);
	if(pUniqueInfo->nRange > 0)
		pIcon->AddToolTipLine( line, TTTC_MUGONGKIND );
	else if( pUniqueInfo->nRange < 0)
		pIcon->AddToolTipLine( line, TTCLR_ENERMY );
	sprintf(line, CHATMGR->GetChatMsg(1623), pUniqueInfo->nAttR);
	if(pUniqueInfo->nAttR > 0)
		pIcon->AddToolTipLine( line, TTTC_MUGONGKIND );
	else if( pUniqueInfo->nAttR < 0)
		pIcon->AddToolTipLine( line, TTCLR_ENERMY );
	sprintf(line, CHATMGR->GetChatMsg(1624), pUniqueInfo->nEnemyDefen);
	if(pUniqueInfo->nEnemyDefen > 0)
		pIcon->AddToolTipLine( line, TTTC_MUGONGKIND );
	else if( pUniqueInfo->nEnemyDefen < 0)
		pIcon->AddToolTipLine( line, TTCLR_ENERMY );
}
void CItemManager::LoadItemLucker()
{
    CMHFile file;
	#ifdef _FILE_BIN_
		if(!file.Init("Resource/ItemLucker.bin", "rb"))
			return;
	#else
		if(!file.Init("Resource/ItemLucker.bin", "rt"))
			return;
	#endif	
		ITEM_LUCKER_INFO * pInfo = NULL;
		while(1)
		{
			if(file.IsEOF())
				break;
			ASSERT(!pInfo);
			pInfo = new ITEM_LUCKER_INFO;
			pInfo->Idx=file.GetWord();
			pInfo->ItemIdx=file.GetDword();
			pInfo->ItemRate=file.GetInt();
			pInfo->ItemMsg=file.GetBool();
			ASSERT(m_ItemLuckerList.GetData(pInfo->Idx) == FALSE);
			m_ItemLuckerList.Add(pInfo,pInfo->Idx);
			pInfo=NULL;
		}
		file.Release();
}
ITEM_LUCKER_INFO* CItemManager::GetItemLuckerInfo(WORD Idx)
{
	return  m_ItemLuckerList.GetData(Idx);   
}
int CItemManager::GetItemLuckerNum()
{
	return  m_ItemLuckerList.GetDataNum();
}
void CItemManager::AddItemStoneOption(ITEM_STONE_OPTION_INFO * pInfo)
{
	if(pInfo == 0)
	{
		return;
	}
	if(m_ItemStoneOptionList.GetData(pInfo->dwStoneOptionIdx))
	{
		m_ItemStoneOptionList.Remove(pInfo->dwStoneOptionIdx);  
	}
	ITEM_STONE_OPTION_INFO * pNewInfo = new ITEM_STONE_OPTION_INFO;
	memcpy(pNewInfo, pInfo, sizeof(ITEM_STONE_OPTION_INFO));
	m_ItemStoneOptionList.Add(pNewInfo, pNewInfo->dwStoneOptionIdx);
}
BOOL CItemManager::RemoveItemStoneOption(DWORD dwStoneIdx)
{
	ASSERT(m_ItemStoneOptionList.GetData(dwStoneIdx));
	ITEM_STONE_OPTION_INFO * pNewInfo = m_ItemStoneOptionList.GetData(dwStoneIdx);
	if( pNewInfo )
	{
		SAFE_DELETE(pNewInfo);
		m_ItemStoneOptionList.Remove(dwStoneIdx);
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}
ITEM_STONE_OPTION_INFO * CItemManager::GetItemStoneOption(DWORD dwStoneIdx)
{
	ITEM_STONE_OPTION_INFO* pInfo = m_ItemStoneOptionList.GetData(dwStoneIdx);
	if(pInfo)
	{
		return pInfo;
	}
	else
	{
		return NULL;
	}
}
void CItemManager::InitItemStoneOption(ITEM_STONE_OPTION_INFO* pInfo, WORD num)
{
	for(int i = 0; i < num; ++ i)
	{
		AddItemStoneOption(&pInfo[i]);
	}
}
BOOL CItemManager::IsStoneOptionItem(DWORD wItemIdx,eITEM_KINDBIT bits)
{
	if(IsDupItem(wItemIdx))  
	{
	   return FALSE;
	}
   	switch(bits)
	{
	case eEQUIP_ITEM_DRESS:
		{
           return TRUE;
		}
		break;
	case eEQUIP_ITEM_HAT:
		{
		   return TRUE;
		}
		break;
    case eEQUIP_ITEM_SHOES:
		{
		   return TRUE;
		}
		break;
	 case eEQUIP_ITEM_WEAPON:
		{
		   return TRUE;
		}
	    break;
	 case eEQUIP_ITEM_RING:
		{
			return TRUE;
		}
		break;
	 case eEQUIP_ITEM_CAPE:
		{
			 return TRUE;
		}
		break;
	 case eEQUIP_ITEM_NECKLACE:
		 {
			 return TRUE;
		 }
	    break;
	 case eEQUIP_ITEM_ARMLET:
		 {
			 return TRUE;
		 }
	    break;
	 case eEQUIP_ITEM_BELT:
		 {
             return TRUE;
		 }
	}
	return FALSE;
}
BOOL CItemManager::LoadItemGrowSet()
{
	CMHFile file;
#ifdef _FILE_BIN_
	if(!file.Init("Resource/ItemGrow_Set.bin", "rb"))
		return FALSE;
#else
	if(!file.Init("Resource/ItemGrow_Set.txt", "rt"))
		return FALSE;
#endif	
	ITEM_GROW_SET * pInfo;
	m_ItemGrowInfo->GrowRate=0;
	m_ItemGrowInfo->Life=0;
	m_ItemGrowInfo->Shield=0;
	m_ItemGrowInfo->NaeRyuk=0;
	m_ItemGrowInfo->GenGol=0;
	m_ItemGrowInfo->MinChub=0;
	m_ItemGrowInfo->CheRyuk=0;
	m_ItemGrowInfo->SimMek=0;
	m_ItemGrowInfo->PhyDef=0;
	while(!file.IsEOF())
	{
		pInfo=new ITEM_GROW_SET;
		ASSERT(!pInfo);
		pInfo->GrowRate= file.GetDword();
		pInfo->Life =file.GetDword();
		pInfo->Shield=file.GetDword();
		pInfo->NaeRyuk=file.GetDword();
		pInfo->GenGol=file.GetFloat();
		pInfo->MinChub=file.GetFloat();
		pInfo->CheRyuk=file.GetFloat();
		pInfo->SimMek =file.GetFloat();
		pInfo->PhyDef =file.GetFloat();
		m_ItemGrowInfo=pInfo;
		break;  
	}
	file.Release();
	pInfo=NULL;
	return TRUE;
}
ITEM_GROW_SET * CItemManager::GetItemGrowSet()
{
	return m_ItemGrowInfo;
}

int CItemManager::GetGrowLevel(DWORD grow)
{
	int ItemLevel = (grow/m_ItemGrowInfo->GrowRate);
	return ItemLevel;
}
float CItemManager::GetNeedLevelPre(DWORD grow)
{
	int pre=0;
	if(grow>m_ItemGrowInfo->GrowRate)
	{
	    pre= grow%m_ItemGrowInfo->GrowRate;
	}
	else
	{
		pre = grow;
	}
	float prevalue= (float)pre/(float)m_ItemGrowInfo->GrowRate;
	return prevalue*100;
}
DWORD CItemManager::GetGrowLife(DWORD grow)
{
	return GetGrowLevel(grow)*m_ItemGrowInfo->Life;
}
DWORD CItemManager::GetGrowShield(DWORD grow)
{
	return GetGrowLevel(grow)*m_ItemGrowInfo->Shield;
}
DWORD CItemManager::GetGrowNaeRyuk(DWORD grow)
{
	return GetGrowLevel(grow)*m_ItemGrowInfo->NaeRyuk;
}
DWORD CItemManager::GetGrowGenGol(DWORD grow)
{
	return GetGrowLevel(grow)*m_ItemGrowInfo->GenGol;
}
DWORD CItemManager::GetGrowMinChub(DWORD grow)
{
	return GetGrowLevel(grow)*m_ItemGrowInfo->MinChub;
}
DWORD CItemManager::GetGrowCheRyuk(DWORD grow)
{
	return GetGrowLevel(grow)*m_ItemGrowInfo->CheRyuk;
}
DWORD CItemManager::GetGrowSimMaek(DWORD grow)
{
	return GetGrowLevel(grow)*m_ItemGrowInfo->SimMek;
}
void CItemManager::DisableDialog()
{
	ITEMMGR->SetDisableDialog(TRUE, eItemTable_Inventory);
	ITEMMGR->SetDisableDialog(TRUE, eItemTable_Pyoguk);
	ITEMMGR->SetDisableDialog(TRUE, eItemTable_MunpaWarehouse);
	ITEMMGR->SetDisableDialog(TRUE, eItemTable_Shop);
}
void CItemManager::ReenableDialog()
{
	ITEMMGR->SetDisableDialog(FALSE, eItemTable_Inventory);
	ITEMMGR->SetDisableDialog(FALSE, eItemTable_Pyoguk);
	ITEMMGR->SetDisableDialog(FALSE, eItemTable_MunpaWarehouse);
	ITEMMGR->SetDisableDialog(FALSE, eItemTable_Shop);
}
void CItemManager::SetItemRelationTip(cMultiLineText* ToolTip,int pos)
{
	if(pos == 80)
	{
		CItem * tmp = NULL;
		for(int i=0;i<80;i++)
		{
			tmp = GAMEIN->GetInventoryDialog()->GetItemForPos(i);
			if(tmp)
			{
				if(tmp->GetItemKind()==eEQUIP_ITEM_HAT)
					tmp->SetRelationToolTip(ToolTip);
			}
		}
	}
	if(pos == 81)
	{
		CItem * tmp = NULL;
		for(int i=0;i<80;i++)
		{
			tmp = GAMEIN->GetInventoryDialog()->GetItemForPos(i);
			if(tmp)
			{
				if(tmp->GetItemKind()==eEQUIP_ITEM_WEAPON)
					tmp->SetRelationToolTip(ToolTip);
			}
		}
	}
	if(pos == 82)
	{
		CItem * tmp = NULL;
		for(int i=0;i<80;i++)
		{
			tmp = GAMEIN->GetInventoryDialog()->GetItemForPos(i);
			if(tmp)
			{
				if(tmp->GetItemKind()==eEQUIP_ITEM_DRESS)
					tmp->SetRelationToolTip(ToolTip);
			}
		}
	}
	if(pos == 83)
	{
		CItem * tmp = NULL;
		for(int i=0;i<80;i++)
		{
			tmp = GAMEIN->GetInventoryDialog()->GetItemForPos(i);
			if(tmp)
			{
				if(tmp->GetItemKind()==eEQUIP_ITEM_SHOES)
					tmp->SetRelationToolTip(ToolTip);
			}
		}
	}
	if(pos == 84)
	{
		CItem * tmp = NULL;
		for(int i=0;i<80;i++)
		{
			tmp = GAMEIN->GetInventoryDialog()->GetItemForPos(i);
			if(tmp)
			{
				if(tmp->GetItemKind()==eEQUIP_ITEM_RING)
					tmp->SetRelationToolTip(ToolTip);
			}
		}
	}
	if(pos == 85)
	{
		CItem * tmp = NULL;
		for(int i=0;i<80;i++)
		{
			tmp = GAMEIN->GetInventoryDialog()->GetItemForPos(i);
			if(tmp)
			{
				if(tmp->GetItemKind()==eEQUIP_ITEM_RING)
					tmp->SetRelationToolTip1(ToolTip);
			}
		}
	}
	if(pos == 86)
	{
		CItem * tmp = NULL;
		for(int i=0;i<80;i++)
		{
			tmp = GAMEIN->GetInventoryDialog()->GetItemForPos(i);
			if(tmp)
			{
				if(tmp->GetItemKind()==eEQUIP_ITEM_CAPE)
					tmp->SetRelationToolTip(ToolTip);
			}
		}
	}
	if(pos == 87)
	{
		CItem * tmp = NULL;
		for(int i=0;i<80;i++)
		{
			tmp = GAMEIN->GetInventoryDialog()->GetItemForPos(i);
			if(tmp)
			{
				if(tmp->GetItemKind()==eEQUIP_ITEM_NECKLACE)
					tmp->SetRelationToolTip(ToolTip);
			}
		}
	}
	if(pos == 88)
	{
		CItem * tmp = NULL;
		for(int i=0;i<80;i++)
		{
			tmp = GAMEIN->GetInventoryDialog()->GetItemForPos(i);
			if(tmp)
			{
				if(tmp->GetItemKind()==eEQUIP_ITEM_ARMLET)
					tmp->SetRelationToolTip(ToolTip);
			}
		}
	}
	if(pos == 89)
	{
		CItem * tmp = NULL;
		for(int i=0;i<80;i++)
		{
			tmp = GAMEIN->GetInventoryDialog()->GetItemForPos(i);
			if(tmp)
			{
				if(tmp->GetItemKind()==eEQUIP_ITEM_BELT)
					tmp->SetRelationToolTip(ToolTip);
			}
		}
	}
}

void CItemManager::SetTitanToolTip( cIcon * pIcon, ITEM_INFO * pInfo,DWORD dwItemDBIdx)
{
	WORD index = pIcon->GetData();
	eITEM_KINDBIT bits = GetItemKind(index);

	char line[128];
	
	//sprintf(line,CHATMGR->GetChatMsg(2237),CHATMGR->GetChatMsg(2217));
	//pIcon->AddToolTipLine(line,TTTC_ITEM_LEVEL);
	//pIcon->AddToolTipLine(line,TTTC_ITEM_LEVEL);
	pIcon->AddToolTipLine("");
	if(bits & eTITAN_EQUIPITEM)
	{
		DWORD dwDBIdx = 0;
		if( pIcon->GetType() == WT_EXCHANGEITEM || pIcon->GetType() == WT_STALLITEM ||pIcon->GetType() == WT_BUYITE)
		{
			CExchangeItem* pItem = (CExchangeItem*)pIcon;
			dwDBIdx = pItem->GetDBId();
		}
		else
		{
			CItem* pItem = (CItem*)pIcon;
			dwDBIdx = pItem->GetDBIdx();
		}
		SetTitanEquipItemToolTip(pIcon, pInfo);
		TITAN_ENDURANCE_ITEMINFO* pEnduranceInfo = NULL;
		if(dwItemDBIdx > 0)
		{
			pEnduranceInfo = TITANMGR->GetTitanEnduranceInfo(dwItemDBIdx);
		}
		else
		{
			if(dwDBIdx)
			{
				pEnduranceInfo = TITANMGR->GetTitanEnduranceInfo(dwDBIdx);
			}
		}
		if(pEnduranceInfo)
		{
			float fEnduranceRate = (float)pEnduranceInfo->Endurance * 100 / TITAN_EQUIPITEM_ENDURANCE_MAX;
			if(fEnduranceRate > 0.0f && fEnduranceRate < 1.0f)
			{
				fEnduranceRate = 1.0f;
			}
			ZeroMemory(&line, sizeof(line));
			wsprintf( line, CHATMGR->GetChatMsg(1516), (DWORD)fEnduranceRate);
			pIcon->AddToolTipLine( line, TTTC_EXTRAATTR );
		}
	}
	if(bits == eTITAN_ITEM_PAPER )
	{
		DWORD dwDBIdx = 0;
		if( pIcon->GetType() == WT_EXCHANGEITEM || pIcon->GetType() == WT_STALLITEM ||pIcon->GetType() == WT_BUYITE)
		{
			CExchangeItem* pItem = (CExchangeItem*)pIcon;
			dwDBIdx = pItem->GetDBId();
		}
		else
		{
			CItem* pItem = (CItem*)pIcon;
			dwDBIdx = pItem->GetDBIdx();
		}
		ZeroMemory(&line, sizeof(line));
		wsprintf( line, CHATMGR->GetChatMsg(1519), pInfo->Plus_Value );
		pIcon->AddToolTipLine( line );
		if( dwDBIdx != 0 && dwDBIdx == TITANMGR->GetRegistedTitanItemDBIdx() )
		{
			ZeroMemory(&line, sizeof(line));
			wsprintf( line, CHATMGR->GetChatMsg(1520) );
			pIcon->AddToolTipLine( line );
		}
	}
	//pIcon->AddToolTipLine("");
	//AddItemDescriptionToolTip( pIcon, pInfo->ItemTooltipIdx );
}
void CItemManager::ShoutGetItem()
{
	if (!m_SShout)return;
	CHATMGR->AddMsg(CTC_SHOUTITEM, CHATMGR->GetChatMsg(2455), HeroName, ItemOpened, ItemObtained);
	m_SShout = FALSE;
}
BOOL CItemManager::LoadSetItemQualityOption()
{
	CMHFile file;
#ifdef _FILE_BIN_
	if (!file.Init("Resource/setitem_Quality.bin", "rb"))
		return FALSE;
#else
	if (!file.Init("Resource/setitem_Quality.txt", "rt"))
		return FALSE;
#endif	// _FILE_BIN_

	SET_ITEMQUALITY_OPTION* pInfo = NULL;
	while (!file.IsEOF())
	{
		ASSERT(!pInfo);
		pInfo = new SET_ITEMQUALITY_OPTION;

		pInfo->wIndex = file.GetDword();
		pInfo->ItemQuality = file.GetDword();
		pInfo->ItemEntry1 = file.GetDword();
		pInfo->ItemEntry2 = file.GetDword();

		SafeStrCpy(pInfo->szSetItemName, file.GetString(), MAX_NAME_LENGTH + 1);
		pInfo->RareVal = file.GetFloat();

		pInfo->wGenGol = file.GetDword();
		pInfo->wMinChub = file.GetDword();
		pInfo->wCheRyuk = file.GetDword();
		pInfo->wSimMek = file.GetDword();
		pInfo->dwLife = file.GetDword();
		pInfo->dwShield = file.GetDword();
		pInfo->dwNaeRyuk = file.GetDword();
		pInfo->AttrRegistDef = file.GetDword();
		pInfo->wPhyDef = file.GetDword();
		pInfo->NaegongDamage = file.GetDword();
		pInfo->WoigongDamage = file.GetDword();

		pInfo->wDodgeRate = file.GetDword();
		pInfo->PlayerPhyDefDown = file.GetDword();
		pInfo->PlayerAttrDefDown = file.GetDword();
		pInfo->TargetPhyDefDown = file.GetDword();
		pInfo->TargetAttrDefDown = file.GetDword();
		pInfo->fDodgeRate = file.GetDword();
		pInfo->MallMoneyPuls = file.GetDword();
		pInfo->KyunggongSpeed = file.GetDword();
		pInfo->AttMonsterDamage = file.GetDword();
		pInfo->AttPlayerDamage = file.GetDword();
		pInfo->RealDamageDown = file.GetDword();
		pInfo->PVPLifePlus = file.GetDword();
		pInfo->Resurrected = file.GetDword();
		pInfo->Critical = file.GetDword();
		pInfo->Decisive = file.GetDword();
		pInfo->CriticalDamage = file.GetDword();
		pInfo->DecisiveDamage = file.GetDword();
		pInfo->ContinueAttAttack = file.GetDword();

		ASSERT(!m_SetItemQualityOptionList.GetData(pInfo->wIndex));

		m_SetItemQualityOptionList.Add(pInfo, pInfo->wIndex);
		pInfo = NULL;
	}
	file.Release();

	return TRUE;
}

SET_ITEMQUALITY_OPTION* CItemManager::GetSetItemQualityOption(WORD ItemQuality, WORD ItemEntry1, WORD ItemEntry2)
{
	SET_ITEMQUALITY_OPTION* pSetItemQualityOption = NULL;

	m_SetItemQualityOptionList.SetPositionHead();
	while (pSetItemQualityOption = m_SetItemQualityOptionList.GetData())
	{
		if (pSetItemQualityOption->ItemQuality == ItemQuality)
		{
			if (pSetItemQualityOption->ItemEntry1 == ItemEntry1 && pSetItemQualityOption->ItemEntry2 == ItemEntry2)
			{
				return pSetItemQualityOption;
			}
		}
	}

	return NULL;
}

void CItemManager::RemoveSetItemQualityOption(WORD wIndex, SET_ITEMQUALITY_OPTION* pSetItemOptionOut)
{
	SET_ITEMQUALITY_OPTION* pInfo = m_SetItemQualityOptionList.GetData(wIndex);

	if (pInfo == NULL)
	{
		ASSERT(0);
		return;
	}

	if (pSetItemOptionOut)
		*pSetItemOptionOut = *pInfo;
	SAFE_DELETE(pInfo);
	m_SetItemQualityOptionList.Remove(wIndex);
}

void CItemManager::AddSetItemQualityToolTip(cIcon* pIcon, WORD ItemQuality, WORD ItemEntry1, WORD ItemEntry2, DWORD color)
{
	char line[128];

	m_SetItemQualityOptionList.SetPositionHead();
	SET_ITEMQUALITY_OPTION* pSetItemQualityOption = NULL;
	while (pSetItemQualityOption = m_SetItemQualityOptionList.GetData())
	{
		if (pSetItemQualityOption->ItemQuality == ItemQuality && pSetItemQualityOption->ItemEntry1 == ItemEntry1 && pSetItemQualityOption->ItemEntry2 == ItemEntry2)
		{
			//sprintf( line, "觉醒属性");
			//pIcon->AddToolTipLine(line,color);
			//pIcon->AddToolTipLine("");
			SetItemQualityToolTip(pIcon, ItemQuality, ItemEntry1, ItemEntry2, color);
			return;
		}
	}
}
void CItemManager::SetItemQualityToolTip(cIcon* pIcon, WORD ItemQuality, WORD ItemEntry1, WORD ItemEntry2, DWORD dwColor)
{
	char line[128];
	float attrvalue = 0;

	SET_ITEMQUALITY_OPTION* pSetItemOption = NULL;
	pSetItemOption = GetSetItemQualityOption(ItemQuality, ItemEntry1, ItemEntry2);

	if (pSetItemOption == NULL)
	{
		return;
	}
	if (ItemQuality == 0)
	{
		if (pSetItemOption->RareVal != 0)
		{
			sprintf(line, "【·普通】最大祝福倍率+%0.1f", pSetItemOption->RareVal);
			pIcon->AddToolTipLine(line, dwColor);
		}
	}
	if (ItemQuality == 1)
	{
		if (pSetItemOption->RareVal != 0)
		{
			sprintf(line, "【·优秀】最大祝福倍率+%0.1f", pSetItemOption->RareVal);
			pIcon->AddToolTipLine(line, dwColor);
		}
		if (pSetItemOption->wGenGol != 0 && ItemEntry1 == 1 && ItemEntry2 == 0)
		{
			sprintf(line, "【·优秀】力量+%d", pSetItemOption->wGenGol);
			pIcon->AddToolTipLine(line, dwColor);
		}
		if (pSetItemOption->wMinChub != 0 && ItemEntry1 == 2 && ItemEntry2 == 0)
		{
			sprintf(line, "【·优秀】敏捷+%d", pSetItemOption->wMinChub);
			pIcon->AddToolTipLine(line, dwColor);
		}
		if (pSetItemOption->wCheRyuk != 0 && ItemEntry1 == 3 && ItemEntry2 == 0)
		{
			sprintf(line, "【·优秀】体质+%d", pSetItemOption->wCheRyuk);
			pIcon->AddToolTipLine(line, dwColor);
		}
		if (pSetItemOption->wSimMek != 0 && ItemEntry1 == 4 && ItemEntry2 == 0)
		{
			sprintf(line, "【·优秀】心脉+%d", pSetItemOption->wSimMek);
			pIcon->AddToolTipLine(line, dwColor);
		}
		if (pSetItemOption->dwLife != 0 && ItemEntry1 == 5 && ItemEntry2 == 0)
		{
			sprintf(line, "【·优秀】生命+%d%%", pSetItemOption->dwLife);
			pIcon->AddToolTipLine(line, dwColor);
		}
		if (pSetItemOption->dwShield != 0 && ItemEntry1 == 6 && ItemEntry2 == 0)
		{
			sprintf(line, "【·优秀】护体+%d%%", pSetItemOption->dwShield);
			pIcon->AddToolTipLine(line, dwColor);
		}
		if (pSetItemOption->dwNaeRyuk != 0 && ItemEntry1 == 7 && ItemEntry2 == 0)
		{
			sprintf(line, "【·优秀】内力+%d%%", pSetItemOption->dwNaeRyuk);
			pIcon->AddToolTipLine(line, dwColor);
		}
		if (pSetItemOption->AttrRegistDef != 0 && ItemEntry1 == 8 && ItemEntry2 == 0)
		{
			sprintf(line, "【·优秀】属性防御+%d%%", pSetItemOption->AttrRegistDef);
			pIcon->AddToolTipLine(line, dwColor);
		}
		if (pSetItemOption->wPhyDef != 0 && ItemEntry1 == 9 && ItemEntry2 == 0)
		{
			sprintf(line, "【·优秀】物理防御+%d%%", pSetItemOption->wPhyDef);
			pIcon->AddToolTipLine(line, dwColor);
		}
		if (pSetItemOption->NaegongDamage != 0 && ItemEntry1 == 10 && ItemEntry2 == 0)
		{
			sprintf(line, "【·优秀】内功伤害+%d%%", pSetItemOption->NaegongDamage);
			pIcon->AddToolTipLine(line, dwColor);
		}
		if (pSetItemOption->WoigongDamage != 0 && ItemEntry1 == 11 && ItemEntry2 == 0)
		{
			sprintf(line, "【·优秀】外功伤害+%d%%", pSetItemOption->WoigongDamage);
			pIcon->AddToolTipLine(line, dwColor);
		}

	}
	if (ItemQuality == 2)
	{
		if (pSetItemOption->RareVal != 0)
		{
			sprintf(line, "【·精良】最大祝福倍率+%0.1f", pSetItemOption->RareVal);
			pIcon->AddToolTipLine(line, dwColor);
		}
		if (pSetItemOption->wGenGol != 0 && ItemEntry1 == 1 && ItemEntry2 == 0)
		{
			sprintf(line, "【·精良】力量+%d", pSetItemOption->wGenGol);
			pIcon->AddToolTipLine(line, dwColor);
		}
		if (pSetItemOption->wMinChub != 0 && ItemEntry1 == 2 && ItemEntry2 == 0)
		{
			sprintf(line, "【·精良】敏捷+%d", pSetItemOption->wMinChub);
			pIcon->AddToolTipLine(line, dwColor);
		}
		if (pSetItemOption->wCheRyuk != 0 && ItemEntry1 == 3 && ItemEntry2 == 0)
		{
			sprintf(line, "【·精良】体质+%d", pSetItemOption->wCheRyuk);
			pIcon->AddToolTipLine(line, dwColor);
		}
		if (pSetItemOption->wSimMek != 0 && ItemEntry1 == 4 && ItemEntry2 == 0)
		{
			sprintf(line, "【·精良】心脉+%d", pSetItemOption->wSimMek);
			pIcon->AddToolTipLine(line, dwColor);
		}
		if (pSetItemOption->dwLife != 0 && ItemEntry1 == 5 && ItemEntry2 == 0)
		{
			sprintf(line, "【·精良】生命+%d%%", pSetItemOption->dwLife);
			pIcon->AddToolTipLine(line, dwColor);
		}
		if (pSetItemOption->dwShield != 0 && ItemEntry1 == 6 && ItemEntry2 == 0)
		{
			sprintf(line, "【·精良】护体+%d%%", pSetItemOption->dwShield);
			pIcon->AddToolTipLine(line, dwColor);
		}
		if (pSetItemOption->dwNaeRyuk != 0 && ItemEntry1 == 7 && ItemEntry2 == 0)
		{
			sprintf(line, "【·精良】内力+%d%%", pSetItemOption->dwNaeRyuk);
			pIcon->AddToolTipLine(line, dwColor);
		}
		if (pSetItemOption->AttrRegistDef != 0 && ItemEntry1 == 8 && ItemEntry2 == 0)
		{
			sprintf(line, "【·精良】属性防御+%d%%", pSetItemOption->AttrRegistDef);
			pIcon->AddToolTipLine(line, dwColor);
		}
		if (pSetItemOption->wPhyDef != 0 && ItemEntry1 == 9 && ItemEntry2 == 0)
		{
			sprintf(line, "【·精良】物理防御+%d%%", pSetItemOption->wPhyDef);
			pIcon->AddToolTipLine(line, dwColor);
		}
		if (pSetItemOption->NaegongDamage != 0 && ItemEntry1 == 10 && ItemEntry2 == 0)
		{
			sprintf(line, "【·精良】内功伤害+%d%%", pSetItemOption->NaegongDamage);
			pIcon->AddToolTipLine(line, dwColor);
		}
		if (pSetItemOption->WoigongDamage != 0 && ItemEntry1 == 11 && ItemEntry2 == 0)
		{
			sprintf(line, "【·精良】外功伤害+%d%%", pSetItemOption->WoigongDamage);
			pIcon->AddToolTipLine(line, dwColor);
		}

	}
	if (ItemQuality == 3)
	{
		if (pSetItemOption->RareVal != 0)
		{
			sprintf(line, "【·传奇】最大祝福倍率+%0.1f", pSetItemOption->RareVal);
			pIcon->AddToolTipLine(line, dwColor);
		}
		if (pSetItemOption->wGenGol != 0 && ItemEntry1 == 1 && ItemEntry2 == 0)
		{
			sprintf(line, "【·传奇】力量+%d", pSetItemOption->wGenGol);
			pIcon->AddToolTipLine(line, dwColor);
		}
		if (pSetItemOption->wMinChub != 0 && ItemEntry1 == 2 && ItemEntry2 == 0)
		{
			sprintf(line, "【·传奇】敏捷+%d", pSetItemOption->wMinChub);
			pIcon->AddToolTipLine(line, dwColor);
		}
		if (pSetItemOption->wCheRyuk != 0 && ItemEntry1 == 3 && ItemEntry2 == 0)
		{
			sprintf(line, "【·传奇】体质+%d", pSetItemOption->wCheRyuk);
			pIcon->AddToolTipLine(line, dwColor);
		}
		if (pSetItemOption->wSimMek != 0 && ItemEntry1 == 4 && ItemEntry2 == 0)
		{
			sprintf(line, "【·传奇】心脉+%d", pSetItemOption->wSimMek);
			pIcon->AddToolTipLine(line, dwColor);
		}
		if (pSetItemOption->dwLife != 0 && ItemEntry1 == 5 && ItemEntry2 == 0)
		{
			sprintf(line, "【·传奇】生命+%d%%", pSetItemOption->dwLife);
			pIcon->AddToolTipLine(line, dwColor);
		}
		if (pSetItemOption->dwShield != 0 && ItemEntry1 == 6 && ItemEntry2 == 0)
		{
			sprintf(line, "【·传奇】护体+%d%%", pSetItemOption->dwShield);
			pIcon->AddToolTipLine(line, dwColor);
		}
		if (pSetItemOption->dwNaeRyuk != 0 && ItemEntry1 == 7 && ItemEntry2 == 0)
		{
			sprintf(line, "【·传奇】内力+%d%%", pSetItemOption->dwNaeRyuk);
			pIcon->AddToolTipLine(line, dwColor);
		}
		if (pSetItemOption->AttrRegistDef != 0 && ItemEntry1 == 8 && ItemEntry2 == 0)
		{
			sprintf(line, "【·传奇】属性防御+%d%%", pSetItemOption->AttrRegistDef);
			pIcon->AddToolTipLine(line, dwColor);
		}
		if (pSetItemOption->wPhyDef != 0 && ItemEntry1 == 9 && ItemEntry2 == 0)
		{
			sprintf(line, "【·传奇】物理防御+%d%%", pSetItemOption->wPhyDef);
			pIcon->AddToolTipLine(line, dwColor);
		}
		if (pSetItemOption->NaegongDamage != 0 && ItemEntry1 == 10 && ItemEntry2 == 0)
		{
			sprintf(line, "【·传奇】内功伤害+%d%%", pSetItemOption->NaegongDamage);
			pIcon->AddToolTipLine(line, dwColor);
		}
		if (pSetItemOption->WoigongDamage != 0 && ItemEntry1 == 11 && ItemEntry2 == 0)
		{
			sprintf(line, "【·传奇】外功伤害+%d%%", pSetItemOption->WoigongDamage);
			pIcon->AddToolTipLine(line, dwColor);
		}
	}
	if (ItemQuality == 4)
	{
		if (pSetItemOption->RareVal != 0)
		{
			sprintf(line, "【·神话】最大祝福倍率+%0.1f", pSetItemOption->RareVal);
			pIcon->AddToolTipLine(line, dwColor);
		}
		if (pSetItemOption->wGenGol != 0 && ItemEntry1 == 1)
		{
			sprintf(line, "【·神话】力量+%d", pSetItemOption->wGenGol);
			pIcon->AddToolTipLine(line, dwColor);
		}
		if (pSetItemOption->wMinChub != 0 && ItemEntry1 == 2)
		{
			sprintf(line, "【·神话】敏捷+%d", pSetItemOption->wMinChub);
			pIcon->AddToolTipLine(line, dwColor);
		}
		if (pSetItemOption->wCheRyuk != 0 && ItemEntry1 == 3)
		{
			sprintf(line, "【·神话】体质+%d", pSetItemOption->wCheRyuk);
			pIcon->AddToolTipLine(line, dwColor);
		}
		if (pSetItemOption->wSimMek != 0 && ItemEntry1 == 4)
		{
			sprintf(line, "【·神话】心脉+%d", pSetItemOption->wSimMek);
			pIcon->AddToolTipLine(line, dwColor);
		}
		if (pSetItemOption->dwLife != 0 && ItemEntry1 == 5)
		{
			sprintf(line, "【·神话】生命+%d%%", pSetItemOption->dwLife);
			pIcon->AddToolTipLine(line, dwColor);
		}
		if (pSetItemOption->dwShield != 0 && ItemEntry1 == 6)
		{
			sprintf(line, "【·神话】护体+%d%%", pSetItemOption->dwShield);
			pIcon->AddToolTipLine(line, dwColor);
		}
		if (pSetItemOption->dwNaeRyuk != 0 && ItemEntry1 == 7)
		{
			sprintf(line, "【·神话】内力+%d%%", pSetItemOption->dwNaeRyuk);
			pIcon->AddToolTipLine(line, dwColor);
		}
		if (pSetItemOption->AttrRegistDef != 0 && ItemEntry1 == 8)
		{
			sprintf(line, "【·神话】属性防御+%d%%", pSetItemOption->AttrRegistDef);
			pIcon->AddToolTipLine(line, dwColor);
		}
		if (pSetItemOption->wPhyDef != 0 && ItemEntry1 == 9)
		{
			sprintf(line, "【·神话】物理防御+%d%%", pSetItemOption->wPhyDef);
			pIcon->AddToolTipLine(line, dwColor);
		}
		if (pSetItemOption->NaegongDamage != 0 && ItemEntry1 == 10)
		{
			sprintf(line, "【·神话】内功伤害+%d%%", pSetItemOption->NaegongDamage);
			pIcon->AddToolTipLine(line, dwColor);
		}
		if (pSetItemOption->WoigongDamage != 0 && ItemEntry1 == 11)
		{
			sprintf(line, "【·神话】外功伤害+%d%%", pSetItemOption->WoigongDamage);
			pIcon->AddToolTipLine(line, dwColor);
		}
		if (ItemEntry2 == 0)
		{
			sprintf(line, "【·神话】未觉醒");
			pIcon->AddToolTipLine(line, RGB_HALF(187, 187, 187));
		}
		else
		{
			if (pSetItemOption->wDodgeRate != 0 && ItemEntry2 == 1)
			{
				sprintf(line, "【·神话】闪避+%d%%", pSetItemOption->wDodgeRate);
				pIcon->AddToolTipLine(line, dwColor);
			}
			if (pSetItemOption->PlayerPhyDefDown != 0 && ItemEntry2 == 2)
			{
				sprintf(line, "【·神话】PVP破甲+%d%%", pSetItemOption->PlayerPhyDefDown);
				pIcon->AddToolTipLine(line, dwColor);
			}
			if (pSetItemOption->PlayerAttrDefDown != 0 && ItemEntry2 == 3)
			{
				sprintf(line, "【·神话】PVP破魔+%d%%", pSetItemOption->PlayerAttrDefDown);
				pIcon->AddToolTipLine(line, dwColor);
			}
			if (pSetItemOption->TargetPhyDefDown != 0 && ItemEntry2 == 4)
			{
				sprintf(line, "【·神话】PVE破甲+%d%%", pSetItemOption->TargetPhyDefDown);
				pIcon->AddToolTipLine(line, dwColor);
			}
			if (pSetItemOption->TargetAttrDefDown != 0 && ItemEntry2 == 5)
			{
				sprintf(line, "【·神话】PVE破魔+%d%%", pSetItemOption->TargetAttrDefDown);
				pIcon->AddToolTipLine(line, dwColor);
			}
			if (pSetItemOption->fDodgeRate != 0 && ItemEntry2 == 6)
			{
				sprintf(line, "【·神话】命中+%d%%", pSetItemOption->fDodgeRate);
				pIcon->AddToolTipLine(line, dwColor);
			}
			if (pSetItemOption->MallMoneyPuls != 0 && ItemEntry2 == 7)
			{
				sprintf(line, "【·神话】泡点加成+%d", pSetItemOption->MallMoneyPuls);
				pIcon->AddToolTipLine(line, dwColor);
			}
			if (pSetItemOption->KyunggongSpeed != 0 && ItemEntry2 == 8)
			{
				sprintf(line, "【·神话】轻功速度+%d", pSetItemOption->KyunggongSpeed);
				pIcon->AddToolTipLine(line, dwColor);
			}
			if (pSetItemOption->AttMonsterDamage != 0 && ItemEntry2 == 9)
			{
				sprintf(line, "【·神话】PVE最终伤害+%d%%", pSetItemOption->AttMonsterDamage);
				pIcon->AddToolTipLine(line, dwColor);
			}
			if (pSetItemOption->AttPlayerDamage != 0 && ItemEntry2 == 10)
			{
				sprintf(line, "【·神话】PVP最终伤害+%d%%", pSetItemOption->AttPlayerDamage);
				pIcon->AddToolTipLine(line, dwColor);
			}
			if (pSetItemOption->RealDamageDown != 0 && ItemEntry2 == 11)
			{
				sprintf(line, "【·神话】受到伤害减少%d%%", pSetItemOption->RealDamageDown);
				pIcon->AddToolTipLine(line, dwColor);
			}
			if (pSetItemOption->PVPLifePlus != 0 && ItemEntry2 == 12)
			{
				sprintf(line, "【·神话】PVP吸血几率+%d%%", pSetItemOption->PVPLifePlus);
				pIcon->AddToolTipLine(line, dwColor);
			}
			if (pSetItemOption->Resurrected != 0 && ItemEntry2 == 13)
			{
				sprintf(line, "【·神话】%d%%几率满血复活", pSetItemOption->Resurrected);
				pIcon->AddToolTipLine(line, dwColor);
			}
			if (pSetItemOption->Critical != 0 && ItemEntry2 == 14)
			{
				sprintf(line, "【·神话】外功暴击率+%d%%", pSetItemOption->Critical);
				pIcon->AddToolTipLine(line, dwColor);
			}
			if (pSetItemOption->Decisive != 0 && ItemEntry2 == 15)
			{
				sprintf(line, "【·神话】内功暴击率+%d%%", pSetItemOption->Decisive);
				pIcon->AddToolTipLine(line, dwColor);
			}
			if (pSetItemOption->CriticalDamage != 0 && ItemEntry2 == 16)
			{
				sprintf(line, "【·神话】外功暴击伤害+%d%%", pSetItemOption->CriticalDamage);
				pIcon->AddToolTipLine(line, dwColor);
			}
			if (pSetItemOption->DecisiveDamage != 0 && ItemEntry2 == 17)
			{
				sprintf(line, "【·神话】内功暴击伤害+%d%%", pSetItemOption->DecisiveDamage);
				pIcon->AddToolTipLine(line, dwColor);
			}
			if (pSetItemOption->ContinueAttAttack != 0 && ItemEntry2 == 18)
			{
				sprintf(line, "【·神话】持续伤害+%d%%", pSetItemOption->ContinueAttAttack);
				pIcon->AddToolTipLine(line, dwColor);
			}
		}
	}
}

CYHHashTable<SET_ITEMQUALITY_OPTION>* CItemManager::GetSetItemQualityOptionList()
{
	return &m_SetItemQualityOptionList;
}