#include "Stdafx.h"
#include "JackRebornInfo.h"
#include "WindowIDEnum.h"
#include "cWindowManager.h"
#include "./Interface/cStatic.h"
#include "./Interface/cScriptManager.h"
#include "ChatManager.h"
#include "GameResourceManager.h"
#include "ObjectManager.h"
#include "ItemManager.h"
extern char JACKJACK[64];
CJackRebornInfo::CJackRebornInfo()
{
	pDlg = NULL;
	StageOne = NULL;
	StageTwo = NULL;
	StageThree = NULL;
	MaxTurn = NULL;

	OneAttribute = NULL;
	OneLevelStart = NULL;
	//OneLevelEnd=NULL;
	OneRewardItem = NULL;
	OneMustNaked = NULL;
	TwoAttribute = NULL;
	TwoLevelStart = NULL;
	//TwoLevelEnd=NULL;
	TwoRewardItem = NULL;
	TwoMustNaked = NULL;
	ThreeAttribute = NULL;
	ThreeLevelStart = NULL;
	//ThreeLevelEnd=NULL;
	ThreeRewardItem = NULL;
	ThreeMustNaked = NULL;

	//m_pItemIconStatic[eStageOne] = NULL;
	//m_pItemIconStatic[eStageTwo] = NULL;
	//m_pItemIconStatic[eStageThree] = NULL;

	ZeroMemory(ItemName, sizeof(ItemName));

	SCRIPTMGR->GetImage(25, &imgHeader, PFT_JACKPATH);
	SCRIPTMGR->GetImage(26, &imgToolTip, PFT_JACKPATH);
}
CJackRebornInfo::~CJackRebornInfo()
{
	/*m_pItemIconStatic[eStageOne] = NULL;
	m_pItemIconStatic[eStageTwo] = NULL;
	m_pItemIconStatic[eStageThree] = NULL;*/
}
void CJackRebornInfo::Linking()
{
	pDlg = WINDOWMGR->GetWindowForID(JACK_REBORN_INFO);
	StageOne = (cStatic *)GetWindowForID(JACK_REBORN_STAGE1);
	StageTwo = (cStatic *)GetWindowForID(JACK_REBORN_STAGE2);
	StageThree = (cStatic *)GetWindowForID(JACK_REBORN_STAGE3);

	OneAttribute = (cStatic *)GetWindowForID(JACK_REBORN_ONEATTRIBUTE);
	OneLevelStart = (cStatic *)GetWindowForID(JACK_REBORN_ONELEVELSTART);
	//OneLevelEnd=(cStatic *)GetWindowForID(JACK_REBORN_ONELEVELEND);
	OneRewardItem = (cStatic *)GetWindowForID(JACK_REBORN_ONEREWARDITEM);
	OneMustNaked = (cStatic *)GetWindowForID(JACK_REBORN_ONEMUSTNAKED);


	TwoAttribute = (cStatic *)GetWindowForID(JACK_REBORN_TWOATTRIBUTE);
	TwoLevelStart = (cStatic *)GetWindowForID(JACK_REBORN_TWOLEVELSTART);
	//TwoLevelEnd=(cStatic *)GetWindowForID(JACK_REBORN_TWOLEVELEND);
	TwoRewardItem = (cStatic *)GetWindowForID(JACK_REBORN_TWOREWARDITEM);
	TwoMustNaked = (cStatic *)GetWindowForID(JACK_REBORN_TWOMUSTNAKED);


	ThreeAttribute = (cStatic *)GetWindowForID(JACK_REBORN_THREEATTRIBUTE);
	ThreeLevelStart = (cStatic *)GetWindowForID(JACK_REBORN_THREELEVELSTART);
	//ThreeLevelEnd=(cStatic *)GetWindowForID(JACK_REBORN_THREELEVELEND);
	ThreeRewardItem = (cStatic *)GetWindowForID(JACK_REBORN_THREEREWARDITEM);
	ThreeMustNaked = (cStatic *)GetWindowForID(JACK_REBORN_THREEMUSTNAKED);



	MaxTurn = (cStatic *)GetWindowForID(JACK_REBORN_MAXTURN);



	/*m_pItemIconStatic[eStageOne] = new cStatic;
	m_pItemIconStatic[eStageOne]->Init(OneRewardItem->GetRelX(), OneRewardItem->GetRelY(), OneRewardItem->GetWidth(), OneRewardItem->GetHeight(), NULL, -1);
	Add(m_pItemIconStatic[eStageOne]);

	m_pItemIconStatic[eStageTwo] = new cStatic;
	m_pItemIconStatic[eStageTwo]->Init(TwoRewardItem->GetRelX(), TwoRewardItem->GetRelY(), TwoRewardItem->GetWidth(), TwoRewardItem->GetHeight(), NULL, -1);
	Add(m_pItemIconStatic[eStageTwo]);

	m_pItemIconStatic[eStageThree] = new cStatic;
	m_pItemIconStatic[eStageThree]->Init(ThreeRewardItem->GetRelX(), ThreeRewardItem->GetRelY(), ThreeRewardItem->GetWidth(), ThreeRewardItem->GetHeight(), NULL, -1);
	Add(m_pItemIconStatic[eStageThree]);*/
}
void CJackRebornInfo::SetActive(BOOL val)
{
	cDialog::SetActive(val);
	//if (val)
	{

		char TempOne[128] = { 0, };

		char TempTwo[128] = { 0, };
		char TempThree[128] = { 0, };

		char TempMax[128] = { 0, };

		char TempProcAttribute1[128] = { 0, };
		char TempProcAttribute2[128] = { 0, };
		char TempProcAttribute3[128] = { 0, };

		char LevelRange1[128] = { 0, };
		char LevelRange2[128] = { 0, };
		char LevelRange3[128] = { 0, };

		char RewardItem1[128] = { 0, };
		char RewardItem2[128] = { 0, };
		char RewardItem3[128] = { 0, };

		char MustNaked1[128] = { 0, };
		char MustNaked2[128] = { 0, };
		char MustNaked3[128] = { 0, };

		DWORD Temp1 = 0, Temp2 = 0, Temp3 = 0, Temp4 = 0;

		Temp1 = HERO->GetTurnOne();
		Temp2 = HERO->GetTurnTwo();
		Temp3 = HERO->GetTurnThree();
		Temp4 = GAMERESRCMNGR->GetRebornSet(REBIRTH_MAX);

		int RebirthOneAttribute = GAMERESRCMNGR->GetRebornSet(STAGE_ONE_ATTRI);
		int RebirthOneLevelStart = GAMERESRCMNGR->GetRebornSet(STAGE_ONE_LevelStart);
		int RebirthOneLevelEnd = GAMERESRCMNGR->GetRebornSet(STAGE_ONE_LevelEnd);
		int RebirthOneRewardItem = GAMERESRCMNGR->GetRebornSet(STAGE_ONE_REWARD);

		int RebirthOneMustNaked = GAMERESRCMNGR->GetRebornSet(STAGE_ONE_EQTAKEOFF);

		sprintf(TempProcAttribute1, "%u", RebirthOneAttribute);
		OneAttribute->SetStaticText(TempProcAttribute1);
		sprintf(LevelRange1, "%u ~ %u", RebirthOneLevelStart, RebirthOneLevelEnd);
		OneLevelStart->SetStaticText(LevelRange1);

		//sprintf(RewardItem1, "%u", RebirthOneRewardItem);
		//OneRewardItem->SetStaticText(RewardItem1);

		if (RebirthOneMustNaked == 1)
			sprintf(MustNaked1, CHATMGR->GetChatMsg(2453));
		else if (RebirthOneMustNaked == 0)
			sprintf(MustNaked1, CHATMGR->GetChatMsg(2454));
		OneMustNaked->SetStaticText(MustNaked1);


		int RebirthTwoAttribute = GAMERESRCMNGR->GetRebornSet(STAGE_TWO_ATTRI);
		int RebirthTwoLevelStart = GAMERESRCMNGR->GetRebornSet(STAGE_TWO_LevelStart);
		int RebirthTwoLevelEnd = GAMERESRCMNGR->GetRebornSet(STAGE_TWO_LevelEnd);
		int RebirthTwoRewardItem = GAMERESRCMNGR->GetRebornSet(STAGE_TWO_REWARD);

		int RebirthTwoMustNaked = GAMERESRCMNGR->GetRebornSet(STAGE_TWO_EQTAKEOFF);

		sprintf(TempProcAttribute2, "%d", RebirthTwoAttribute);
		TwoAttribute->SetStaticText(TempProcAttribute2);
		sprintf(LevelRange2, "%d ~ %d", RebirthTwoLevelStart, RebirthTwoLevelEnd);
		TwoLevelStart->SetStaticText(LevelRange2);

		//sprintf(RewardItem2, "%d", RebirthTwoRewardItem);
		//TwoRewardItem->SetStaticText(RewardItem2);

		if (RebirthTwoMustNaked == 1)
			sprintf(MustNaked2, CHATMGR->GetChatMsg(2453));
		else if (RebirthTwoMustNaked == 0)
			sprintf(MustNaked2, CHATMGR->GetChatMsg(2454));

		TwoMustNaked->SetStaticText(MustNaked2);


		int RebirthThreeAttribute = GAMERESRCMNGR->GetRebornSet(STAGE_THREE_ATTRI);
		int RebirthThreeLevelStart = GAMERESRCMNGR->GetRebornSet(STAGE_THREE_LevelStart);
		int RebirthThreeLevelEnd = GAMERESRCMNGR->GetRebornSet(STAGE_THREE_LevelEnd);
		int RebirthThreeRewardItem = GAMERESRCMNGR->GetRebornSet(STAGETHREEREWARD);

		int RebirthThreeMustNaked = GAMERESRCMNGR->GetRebornSet(STAGE_THREE_EQTAKEOFF);

		sprintf(TempProcAttribute3, "%d", RebirthThreeAttribute);
		ThreeAttribute->SetStaticText(TempProcAttribute3);
		sprintf(LevelRange3, "%d ~ %d", RebirthThreeLevelStart, RebirthThreeLevelEnd);
		ThreeLevelStart->SetStaticText(LevelRange3);

		//sprintf(RewardItem3, "%d", RebirthThreeRewardItem);
		//ThreeRewardItem->SetStaticText(RewardItem3);

		if (RebirthThreeMustNaked == 1)
			sprintf(MustNaked3, CHATMGR->GetChatMsg(2453));
		else if (RebirthThreeMustNaked == 0)
			sprintf(MustNaked3, CHATMGR->GetChatMsg(2454));
		ThreeMustNaked->SetStaticText(MustNaked3);



		//sprintf(Temp,"TOTAL REBORN:%d Stage:One=%d Two=%d Three=%d",Temp4,Temp1,Temp2,Temp3);
		sprintf(TempOne, "%d", Temp1);
		StageOne->SetStaticText(TempOne);

		sprintf(TempTwo, "%d", Temp2);
		StageTwo->SetStaticText(TempTwo);

		sprintf(TempThree, "%d", Temp3);
		StageThree->SetStaticText(TempThree);

		sprintf(TempMax, CHATMGR->GetChatMsg(2452), Temp4);
		MaxTurn->SetStaticText(TempMax);


		cImage itemImage;
		cImageScale imageScale;
		imageScale.x = 0.8;
		imageScale.y = 0.8;

		ITEM_INFO * pInfo[3] = { NULL, NULL, NULL };

		pInfo[eStageOne] = ITEMMGR->GetItemInfo(RebirthOneRewardItem);
		if (pInfo[eStageOne])
		{
			if (ITEMMGR->GetIconImage(RebirthOneRewardItem, &itemImage))
			{
				OneRewardItem->AddToolTipImageEquip(&itemImage);
			}
			else
			{
				OneRewardItem->AddToolTipLine(" ");
			}
			OneRewardItem->SetScale(&imageScale);
			OneRewardItem->SetBasicImage(&itemImage);
			OneRewardItem->SetHeaderImage(&imgHeader);
			OneRewardItem->SetToolTip("", RGBA_MAKE(255, 255, 255, 255), &imgToolTip, TTCLR_ITEM_CANEQUIP);
			OneRewardItem->SetToolModel(1);

			char buf3[MAX_ITEMNAME_LENGTH + 1];
			wsprintf(buf3, "[%s]", ITEMMGR->GetItemInfo(RebirthOneRewardItem)->ItemName);
			OneRewardItem->AddToolTipLine(buf3, TTTC_DEFAULT, NULL, 6);
			OneRewardItem->AddToolTipLine(JACKJACK);

			ITEMMGR->SetExtraItemToolTip((cIcon*)OneRewardItem, pInfo[eStageOne]);
			OneRewardItem->AddToolTipLine(" ");
		}

		// reward stage two
		pInfo[eStageTwo] = ITEMMGR->GetItemInfo(RebirthTwoRewardItem);
		if (pInfo[eStageTwo])
		{
			if (ITEMMGR->GetIconImage(RebirthTwoRewardItem, &itemImage))
			{
				TwoRewardItem->AddToolTipImageEquip(&itemImage);
			}
			else
			{
				TwoRewardItem->AddToolTipLine(" ");
			}
			TwoRewardItem->SetScale(&imageScale);
			TwoRewardItem->SetBasicImage(&itemImage);
			TwoRewardItem->SetHeaderImage(&imgHeader);
			TwoRewardItem->SetToolTip("", RGBA_MAKE(255, 255, 255, 255), &imgToolTip, TTCLR_ITEM_CANEQUIP);
			TwoRewardItem->SetToolModel(1);

			char buf3[MAX_ITEMNAME_LENGTH + 1];
			wsprintf(buf3, "[%s]", ITEMMGR->GetItemInfo(RebirthTwoRewardItem)->ItemName);
			TwoRewardItem->AddToolTipLine(buf3, TTTC_DEFAULT, NULL, 6);
			TwoRewardItem->AddToolTipLine(JACKJACK);

			ITEMMGR->SetExtraItemToolTip((cIcon*)TwoRewardItem, pInfo[eStageTwo]);
			TwoRewardItem->AddToolTipLine(" ");
		}

		// reward stage three
		pInfo[eStageThree] = ITEMMGR->GetItemInfo(RebirthThreeRewardItem);
		if (pInfo[eStageThree])
		{
			if (ITEMMGR->GetIconImage(RebirthThreeRewardItem, &itemImage))
			{
				ThreeRewardItem->AddToolTipImageEquip(&itemImage);
			}
			else
			{
				ThreeRewardItem->AddToolTipLine(" ");
			}
			ThreeRewardItem->SetScale(&imageScale);
			ThreeRewardItem->SetBasicImage(&itemImage);
			ThreeRewardItem->SetHeaderImage(&imgHeader);
			ThreeRewardItem->SetToolTip("", RGBA_MAKE(255, 255, 255, 255), &imgToolTip, TTCLR_ITEM_CANEQUIP);
			ThreeRewardItem->SetToolModel(1);

			char buf3[MAX_ITEMNAME_LENGTH + 1];
			wsprintf(buf3, "[%s]", ITEMMGR->GetItemInfo(RebirthThreeRewardItem)->ItemName);
			ThreeRewardItem->AddToolTipLine(buf3, TTTC_DEFAULT, NULL, 6);
			ThreeRewardItem->AddToolTipLine(JACKJACK);

			ITEMMGR->SetExtraItemToolTip((cIcon*)ThreeRewardItem, pInfo[eStageThree]);
			ThreeRewardItem->AddToolTipLine(" ");
		}


	}
}
//extern char JACKJACK[64];
void CJackRebornInfo::ExRender()
{
	cDialog::Render();
	return;
	/*ITEM_INFO * pInfo = ITEMMGR->GetItemInfo(55140);
	if (IsActive())
	if (pInfo)
	{
	char buf3[MAX_ITEMNAME_LENGTH + 1];
	cImage imgToolTip;
	cImage imgHeader;
	cImage * imgEquipInfo;
	SCRIPTMGR->GetImage(25, &imgHeader, PFT_JACKPATH);
	SCRIPTMGR->GetImage(26, &imgToolTip, PFT_JACKPATH);
	m_pItemIconStatic[eStageOne]->SetHeaderImage(&imgHeader);
	m_pItemIconStatic[eStageOne]->SetToolTip("", RGBA_MAKE(255, 255, 255, 255), &imgToolTip, TTCLR_ITEM_CANEQUIP);
	m_pItemIconStatic[eStageOne]->SetToolModel(1);
	imgEquipInfo = m_pItemIconStatic[eStageOne]->GetBasicImage();
	if (imgEquipInfo)
	{
	m_pItemIconStatic[eStageOne]->AddToolTipImageEquip(imgEquipInfo);
	}
	else
	{
	m_pItemIconStatic[eStageOne]->AddToolTipLine(" ");
	}
	wsprintf(buf3, "[%s]", ItemName);
	m_pItemIconStatic[eStageOne]->AddToolTipLine(buf3, TTTC_DEFAULT, NULL, 6);
	m_pItemIconStatic[eStageOne]->AddToolTipLine(JACKJACK);
	ITEMMGR->SetExtraItemToolTip((cIcon*)m_pItemIconStatic, pInfo);
	m_pItemIconStatic[eStageOne]->AddToolTipLine(" ");
	}*/

}