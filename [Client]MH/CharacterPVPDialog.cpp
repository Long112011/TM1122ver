







// CharacterDialog.cpp: implementation of the CCharacterDialog class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CharacterPVPDialog.h"
#include "ObjectManager.h"
#include "./Interface/CWindowManager.h"
#include "./Interface/cStatic.h"
#include "./Interface/cButton.h"
#include "WindowIDEnum.h"
#include "GuildManager.h"
#include "KyungGongManager.h"
#include "MunpaMarkDialog.h"
#include "SkillManager_Client.h"
#include "CheatMsgParser.h"
#include "StatsCalcManager.h"

#include "Hero.h"
#include "Player.h"
#include "PetManager.h"
#include "GameIn.h"
#include "MainBarDialog.h"
#include "ChatManager.h"
#include "cScriptManager.h"
#include "cPushupButton.h"
#include "CharacterDialog.h"
#ifdef _JAPAN_LOCAL_
#include "Hero.h"

#endif
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


CCharacterPvpDialog::CCharacterPvpDialog()
{
	m_type = WT_CHARINFOPVPDIALOG;
	m_nocoriPoint = 0;
	m_MinusPoint = -1;
}

CCharacterPvpDialog::~CCharacterPvpDialog()
{

}

void CCharacterPvpDialog::Linking()
{
	m_ppStatic.PVPDef = (cStatic*)GetWindowForID(MY_INFO_PVP_STC_PVP_DEFENSE);//PVP·ÀÓùÁ¦
	m_ppStatic.PVPADef = (cStatic*)GetWindowForID(MY_INFO_PVP_STC_PVP_RESIST);//PVPµÖ¿¹Á¦
	m_ppStatic.PVPCri = (cStatic*)GetWindowForID(MY_INFO_PVP_STC_PVP_CRITICAL_AND_RESIST);//PVPÒ»»÷ / Ò»»÷µÖ¿¹
	m_ppStatic.PVPCriticalDamage = (cStatic*)GetWindowForID(MY_INFO_PVP_STC_PVP_CRITICAL_DAMAGE_INC_AND_DEC);//PVPÒ»»÷ÉËº¦Ôö¼Ó / ¼õÉÙ
	m_ppStatic.PVPADodge = (cStatic*)GetWindowForID(MY_INFO_PVP_STC_PVP_DODGE_AND_ACCURACY);//PVP»Ø±Ü / ÃüÖÐ
	m_ppStatic.PVPDAMAGE_Des = (cStatic*)GetWindowForID(MY_INFO_PVP_STC_PVP_DAMAGE_INC_AND_DEC);//PVPÉËº¦Ôö¼Ó / ¼õÉÙ



	m_ppStatic.meleeattack = (cStatic*)GetWindowForID(MY_INFO_PVP_STC_ATTACK);
	m_ppStatic.defense = (cStatic*)GetWindowForID(MY_INFO_PVP_STC_DEFENSE);

	m_AttrDefComponent.pStatic.SetElement_Val(ATTR_FIRE, (cStatic*)GetWindowForID(MY_INFO_PVP_STC_FIRE_RESIST));
	m_AttrDefComponent.pStatic.SetElement_Val(ATTR_WATER, (cStatic*)GetWindowForID(MY_INFO_PVP_STC_WATER_RESIST));
	m_AttrDefComponent.pStatic.SetElement_Val(ATTR_IRON, (cStatic*)GetWindowForID(MY_INFO_PVP_STC_IRON_RESIST));

	m_AttrOffComponent.pStatic.SetElement_Val(ATTR_FIRE, (cStatic*)GetWindowForID(MY_INFO_PVP_STC_FIRE_ATT_ATTACK));
	m_AttrOffComponent.pStatic.SetElement_Val(ATTR_WATER, (cStatic*)GetWindowForID(MY_INFO_PVP_STC_WATER_ATT_ATTACK));
	m_AttrOffComponent.pStatic.SetElement_Val(ATTR_IRON, (cStatic*)GetWindowForID(MY_INFO_PVP_STC_IRON_ATT_ATTACK));

	m_ppStatic.critical = (cStatic*)GetWindowForID(MY_INFO_PVP_STC_CRITICAL_AND_RESIST);//Ò»»÷ / Ò»»÷µÖ¿¹
	m_ppStatic.criticaldamage = (cStatic*)GetWindowForID(MY_INFO_PVP_STC_CRITICAL_DAMAGE_INC_AND_DEC);//Ò»»÷ÉËº¦Ôö¼Ó/¼õÉÙ
	m_ppStatic.dodge = (cStatic*)GetWindowForID(MY_INFO_PVP_STC_DODGE_AND_ACCURACY);//»Ø±Ü / ÃüÖÐ
	m_ppStatic.stunpvp = (cStatic*)GetWindowForID(MY_INFO_PVP_STC_STUN_AND_RESIST);// ÔÎÑ£ / ÔÎÑ£µÖ¿¹
	m_ppStatic.phyandattdamage = (cStatic*)GetWindowForID(MY_INFO_PVP_STC_DAMAGE_INC_PHY_AND_ATT);//Ò»»÷ / Ò»»÷µÖ¿¹
}
void CCharacterPvpDialog::Init(LONG x, LONG y, WORD wid, WORD hei, cImage* basicImage, LONG ID)
{
	cDialog::Init(x, y, wid, hei, basicImage, ID);
	//m_type = WT_CHARINFODIALOG;
}

void CCharacterPvpDialog::UpdateData()
{
	BASEOBJECT_INFO ObjInfo;
	CHARACTER_TOTALINFO ChaInfo;
	HERO_TOTALINFO HeroInfo;
	OBJECTMGR->GetHero()->GetBaseObjectInfo(&ObjInfo);
	OBJECTMGR->GetHero()->GetCharacterTotalInfo(&ChaInfo);
	OBJECTMGR->GetHero()->GetHeroTotalInfo(&HeroInfo);

	SetPVPDefens();
	SetPVPATTDefens();
	SetPVPCritical();
	SetPVPCriticalDamage();
	SetPVPDodge();
	SetPVPDAMAGE_DES();
	SetAttackRate();
	SetALLPhyDefense();
	SetALLATTDefense();
	SetALLCritical();
	SetALLCriticalDamage();
	SetALLDodge();
	SetALLStun();
	SetALLAttack();
	for (int n = 0; n < ATTR_MAX; ++n)
	{
		if (n == 2 || n == 4) continue;
		//WORD value = (WORD)(HERO->GetAttDefense(ATTR_FIRE + n) * 100.5);
		//value += HERO->GetShopItemStats()->RegistAttr;

		//float val = 1 + HERO->GetSkillStatsOption()->AttDef;

		//if (val < 0)
		//	val = 0.0f;

		//value = (WORD)(value * val + 0.5);


		WORD AttMin = HERO->DoGetAttAttackPowerMin(ATTR_FIRE + n);
		WORD AttMax = HERO->DoGetAttAttackPowerMax(ATTR_FIRE + n);
		//m_AttrDefComponent.SetValue(ATTR_FIRE + n, value, RGB_HALF(255, 210, 95));
		m_AttrOffComponent.SetValue(ATTR_FIRE + n, AttMin, AttMax, RGB_HALF(255, 210, 95));
	}


	//SetCritical();
	//SetAttackRange();
	//SetAttackRate();
	//SetDefenseRate();
	//SetCriticalDamage();
	//SetDodge();
	//SetPhyAndAttDamage();
	//SetStun();


}


void CCharacterPvpDialog::SetPVPDefens()
{

	WORD value = (WORD)HERO->GetItemStats()->PVPDef;
	value += (WORD)HERO->GetAvatarOption()->PVPDef;
	char buf1[66];
	wsprintf(buf1, "%d%%", value);
	m_ppStatic.PVPDef->SetStaticText(buf1);
}
void CCharacterPvpDialog::SetPVPATTDefens()
{

	WORD value = (WORD)HERO->GetItemStats()->PVPADef;
	value += (WORD)HERO->GetAvatarOption()->PVPADef;
	char buf1[66];
	wsprintf(buf1, "%d%%", value);
	m_ppStatic.PVPADef->SetStaticText(buf1);
}
void CCharacterPvpDialog::SetPVPCritical()
{

	WORD value = (WORD)HERO->GetItemStats()->PVPCri;
	value += (WORD)HERO->GetAvatarOption()->PVPCri;
	char buf1[66];
	wsprintf(buf1, "%d%% / %d%%", value, 0);
	m_ppStatic.PVPCri->SetStaticText(buf1);
}
void CCharacterPvpDialog::SetPVPCriticalDamage()
{

	WORD value = (WORD)HERO->GetItemStats()->PVPCri;
	value += (WORD)HERO->GetAvatarOption()->PVPCri;
	char buf1[66];
	wsprintf(buf1, "%d%% / %d%%", 0, 0);
	m_ppStatic.PVPCriticalDamage->SetStaticText(buf1);
}
void CCharacterPvpDialog::SetPVPDodge()
{
	WORD value = 0, value2 = 0;
	value = (WORD)HERO->GetItemStats()->PVPADodge;
	value += (WORD)HERO->GetAvatarOption()->PVPADodge;

	value2 += HERO->GetItemStats()->PVPHit;
	value2 += HERO->GetAvatarOption()->PVPHit;

	char buf1[66];
	wsprintf(buf1, "%d%% / %d%%", value, value2);
	m_ppStatic.PVPADodge->SetStaticText(buf1);
}
void CCharacterPvpDialog::SetPVPDAMAGE_DES()
{
	WORD value = 0, value2 = 0;
	value = (WORD)HERO->GetItemStats()->PVPAttack;
	value += (WORD)HERO->GetAvatarOption()->PVPAttack;

	char buf1[66];
	wsprintf(buf1, "%d%% / %d%%", value, 0);
	m_ppStatic.PVPDAMAGE_Des->SetStaticText(buf1);
}
void CCharacterPvpDialog::SetAttackRate()//¹¥»÷Á¦
{
	char buf1[66];
	DWORD minv, maxv;
	minv = HERO->GetPhyAttackPowerMin();
	maxv = HERO->GetPhyAttackPowerMax();

	float val = 1 + HERO->GetSkillStatsOption()->BaseAtk;


	if (val < 0)
		val = 0.0f;

	minv = (DWORD)((minv * val) + 0.5);

	maxv = (DWORD)((maxv * val) + 0.5);

	wsprintf(buf1, "%d ~ %d", minv, maxv);

	m_ppStatic.meleeattack->SetStaticText(buf1);


}
void CCharacterPvpDialog::SetActive(BOOL val)
{
	cDialog::SetActive(val);
	cDialog::SetAbsXY(GAMEIN->GetCharacterDialog()->GetAbsX() + 303, GAMEIN->GetCharacterDialog()->GetAbsY() + 18);
	//RefreshPointInfo();

	//CMainBarDialog* pDlg = GAMEIN->GetMainInterfaceDialog();
	//if (pDlg)
	//{
	//	if (val == TRUE)
	//		pDlg->SetAlram(OPT_CHARACTERDLGICON, FALSE);
	//}
}

void CCharacterPvpDialog::RefreshInfo()
{


	STATSMGR->CalcCharStats(HERO);
	SetPVPDefens();
	SetPVPATTDefens();
	SetPVPCritical();
	SetPVPCriticalDamage();
	SetPVPDodge();
	SetPVPDAMAGE_DES();
	SetAttackRate();
	SetALLPhyDefense();
	SetALLATTDefense();
	SetALLCritical();
	SetALLCriticalDamage();
	SetALLDodge();
	SetALLStun();
	SetALLAttack();

}
void CCharacterPvpDialog::SetALLPhyDefense()//ÌìÄ«¼¼ÊõÐÞ¸Ä 2023 - 12 - 29 ÐÞ¸´65535 ¼«ÏÞ
{
	DWORD value = (DWORD)HERO->GetPhyDefense();
	value += (value * HERO->GetShopItemStats()->RegistPhys) / 100;

	float val = 1 + HERO->GetSkillStatsOption()->PhyDef;

	if (val < 0)
		val = 0.0f;

	value = (DWORD)(value * val + 0.5);

	DWORD value1 = (DWORD)HERO->GetItemStats()->PVPDef;
	value1 += (DWORD)HERO->GetAvatarOption()->PVPDef;

	DWORD PhyDefense = value + value * (value1 / 100);
	char buf1[66];
	wsprintf(buf1, "%d%", PhyDefense);

	m_ppStatic.defense->SetStaticText(buf1);
}
void CCharacterPvpDialog::SetALLATTDefense()
{
	for (int n = 0; n < ATTR_MAX; ++n)
	{
		if (n == 2 || n == 4) continue;
		WORD value = (WORD)(HERO->GetAttDefense(ATTR_FIRE + n) * 100.5);

		value += HERO->GetShopItemStats()->RegistAttr;

		float val = 1 + HERO->GetSkillStatsOption()->AttDef;

		if (val < 0)
			val = 0.0f;

		value = (WORD)(value * val + 0.5);


		WORD value1 = (WORD)HERO->GetItemStats()->PVPADef;
		value1 += (WORD)HERO->GetAvatarOption()->PVPADef;
		value += value * value1 / 100;



		m_AttrDefComponent.SetValue(ATTR_FIRE + n, value, RGB_HALF(255, 210, 95));

	}
}
void CCharacterPvpDialog::SetALLCritical()
{
	DWORD resVal1 = HERO->GetCritical();
	DWORD resVal2 = HERO->GetDecisive();
	resVal1 += HERO->GetShopItemStats()->Critical;
	resVal1 += HERO->GetAvatarOption()->Critical;
	resVal2 += HERO->GetShopItemStats()->Decisive;
	resVal2 += HERO->GetAvatarOption()->Decisive;

	WORD value = (WORD)HERO->GetItemStats()->PVPCri;
	value += (WORD)HERO->GetAvatarOption()->PVPCri;

	resVal1 += resVal1 * value / 100;


	char buf1[66];
	wsprintf(buf1, "%d / %d", resVal1, 0);

	m_ppStatic.critical->SetStaticText(buf1);
}
void CCharacterPvpDialog::SetALLCriticalDamage()
{
	//DWORD resVal1 = HERO->GetCritical();
	//DWORD resVal2 = HERO->GetDecisive();
	//resVal1 += HERO->GetShopItemStats()->;
	//resVal1 += HERO->GetAvatarOption()->Critical;
	//resVal2 += HERO->GetShopItemStats()->Decisive;
	//resVal2 += HERO->GetAvatarOption()->Decisive;

	char buf1[66];
	wsprintf(buf1, "%d%% / %d%%", 0, 0);

	m_ppStatic.criticaldamage->SetStaticText(buf1);
}
void CCharacterPvpDialog::SetALLDodge()
{
	WORD value = 0, value2 = 0;
	value = (WORD)HERO->GetItemStats()->PVPADodge;
	value += (WORD)HERO->GetAvatarOption()->PVPADodge;

	value2 += HERO->GetItemStats()->PVPHit;
	value2 += HERO->GetAvatarOption()->PVPHit;
	char buf1[66];
	DWORD PetBuffSuccessProb = 0;
#ifdef  _MUTIPET_
	for (int i = 0; i < 3; ++i)//µ¶¸ç  3pet
	{
		CPet* pPet = HERO->GetPet(i);
		if (pPet)
			if (pPet)
			{
				BASE_PET_LIST* pList = GAMERESRCMNGR->GetPetListInfo(pPet->GetPetKind());
				PET_BUFF_LIST* pBuffList = NULL;
				for (int i = 0; i < MAX_PET_BUFF_NUM; ++i)
				{
					WORD BuffIdx = pList->BuffList[pPet->GetPetCurGrade() - 1][i];

					if (0 == BuffIdx) continue;

					pBuffList = GAMERESRCMNGR->GetPetBuffInfo(BuffIdx);

					if (pBuffList && pBuffList->BuffValueData)
					{
						if (pBuffList->BuffKind == ePB_Dodge)
						{
							PetBuffSuccessProb = pBuffList->BuffSuccessProb;
							//wsprintf(buf1, "%d / %d", pBuffList->BuffSuccessProb, 0);
						}
					}
				}
			}
	}
#else
	CPet* pPet = HERO->GetPet();
	if (pPet)
	{
		BASE_PET_LIST* pList = GAMERESRCMNGR->GetPetListInfo(pPet->GetPetKind());
		PET_BUFF_LIST* pBuffList = NULL;
		for (int i = 0; i < MAX_PET_BUFF_NUM; ++i)
		{
			WORD BuffIdx = pList->BuffList[pPet->GetPetCurGrade() - 1][i];

			if (0 == BuffIdx) continue;

			pBuffList = GAMERESRCMNGR->GetPetBuffInfo(BuffIdx);

			if (pBuffList && pBuffList->BuffValueData)
			{
				if (pBuffList->BuffKind == ePB_Dodge)
				{
					PetBuffSuccessProb = pBuffList->BuffSuccessProb;
					//wsprintf(buf1, "%d / %d", pBuffList->BuffSuccessProb, 0);
				}
			}
		}
	}
#endif //  _MUTIPET_
	value += PetBuffSuccessProb;

	wsprintf(buf1, "%d% / %d%", value, value2);
	m_ppStatic.dodge->SetStaticText(buf1);
}
void CCharacterPvpDialog::SetALLStun()
{
	WORD value = 0, value2 = 0;
	value = (WORD)HERO->GetItemStats()->PVPStunResist;
	value += (WORD)HERO->GetAvatarOption()->PVPStunResist;


	value2 += HERO->GetItemStats()->PVPStunTimeReduce;
	value2 += HERO->GetAvatarOption()->PVPStunTimeReduce;


	char buf1[66];
	wsprintf(buf1, "%d%% / %d%%", value, value2);
	m_ppStatic.stunpvp->SetStaticText(buf1);

}
void CCharacterPvpDialog::SetALLAttack()
{
	DWORD resVal1 = 0, resVal2 = 0, value = 0;
	resVal1 += HERO->GetShopItemStats()->NeagongDamage;
	resVal1 += HERO->GetAvatarOption()->NeagongDamage;
	resVal2 += HERO->GetShopItemStats()->WoigongDamage;
	resVal2 += HERO->GetAvatarOption()->WoigongDamage;


	value = (WORD)HERO->GetItemStats()->PVPAttack;
	value += (WORD)HERO->GetAvatarOption()->PVPAttack;

	resVal1 += value;
	resVal2 += value;
	char buf1[66];
	wsprintf(buf1, "%d%% / %d%%", resVal2, resVal1);

	m_ppStatic.phyandattdamage->SetStaticText(buf1);
}
void CCharacterPvpDialog::SetCriticalDamage()
{
	//DWORD resVal1 = HERO->GetCritical();
	//DWORD resVal2 = HERO->GetDecisive();
	//resVal1 += HERO->GetShopItemStats()->;
	//resVal1 += HERO->GetAvatarOption()->Critical;
	//resVal2 += HERO->GetShopItemStats()->Decisive;
	//resVal2 += HERO->GetAvatarOption()->Decisive;

	char buf1[66];
	wsprintf(buf1, "%d%% / %d%%", 0, 0);

	m_ppStatic.criticaldamage->SetStaticText(buf1);
}
void CCharacterPvpDialog::SetDodge()
{

     #ifdef  _MUTIPET_
	char buf1[66] = {};
	DWORD val = 0;
	for (int i = 0; i < 3; ++i)//µ¶¸ç  3pet
	{
		CPet* pPet = HERO->GetPet(i);
		if (pPet)
		{
			BASE_PET_LIST* pList = GAMERESRCMNGR->GetPetListInfo(pPet->GetPetKind());
			PET_BUFF_LIST* pBuffList = NULL;
			for (int i = 0; i < MAX_PET_BUFF_NUM; ++i)
			{
				WORD BuffIdx = pList->BuffList[pPet->GetPetCurGrade() - 1][i];

				if (0 == BuffIdx) continue;

				pBuffList = GAMERESRCMNGR->GetPetBuffInfo(BuffIdx);

				if (pBuffList && pBuffList->BuffValueData)
				{
					if (pBuffList->BuffKind == ePB_Dodge)
					{
						val += pBuffList->BuffSuccessProb;//µ¶¸ç  3pet
					}
				}
			}
		}
         #else
	char buf1[66] = {};
	CPet* pPet = HERO->GetPet();
	if (pPet)
	{
		BASE_PET_LIST* pList = GAMERESRCMNGR->GetPetListInfo(pPet->GetPetKind());
		PET_BUFF_LIST* pBuffList = NULL;
		for (int i = 0; i < MAX_PET_BUFF_NUM; ++i)
		{
			WORD BuffIdx = pList->BuffList[pPet->GetPetCurGrade() - 1][i];

			if (0 == BuffIdx) continue;

			pBuffList = GAMERESRCMNGR->GetPetBuffInfo(BuffIdx);

			if (pBuffList && pBuffList->BuffValueData)
			{
				if (pBuffList->BuffKind == ePB_Dodge)
				{
					wsprintf(buf1, "%d / %d", pBuffList->BuffSuccessProb, 0);
				}
			}
		}
	}
#endif //  _MUTIPET_

	else
	{
		wsprintf(buf1, "%d / %d", 0, 0);
	}
	}
	m_ppStatic.dodge->SetStaticText(buf1);
}

//void CCharacterPvpDialog::SetStun()
//{
//	char buf1[66];
//	wsprintf(buf1, "%d / %d", 0, 0);
//	m_ppStatic.stun->SetStaticText(buf1);
//}
void CCharacterPvpDialog::SetPhyAndAttDamage()
{
	DWORD resVal1 = 0, resVal2 = 0;
	resVal1 += HERO->GetShopItemStats()->NeagongDamage;
	resVal1 += HERO->GetAvatarOption()->NeagongDamage;
	resVal2 += HERO->GetShopItemStats()->WoigongDamage;
	resVal2 += HERO->GetAvatarOption()->WoigongDamage;

	char buf1[66];
	wsprintf(buf1, "%d%% / %d%%", resVal2, resVal1);
	m_ppStatic.phyandattdamage->SetStaticText(buf1);
}