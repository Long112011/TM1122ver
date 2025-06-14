







// CharacterDialog.cpp: implementation of the CCharacterDialog class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CharacterDialog.h"
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


#include "GameIn.h"
#include "MainBarDialog.h"
#include "ChatManager.h"

#include "famemanager.h"
#ifdef _JAPAN_LOCAL_
#include "Hero.h"

#endif
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

/*
//SW0508 ������ �޸� JAPAN Edition ������ ���� ��ȣ�� ����.
*/


CCharacterDialog::CCharacterDialog()
{
	m_type = WT_CHARINFODIALOG;
	m_nocoriPoint = 0;
	m_MinusPoint = -1;
}

CCharacterDialog::~CCharacterDialog()
{

}

void CCharacterDialog::Linking()
{
	m_pMunpaMarkDlg = (CMunpaMarkDialog*)GetWindowForID(CI_MUNPAMARK);
	m_ppStatic.munpa = (cStatic *)GetWindowForID(CI_CHARMUNPA);
	m_ppStatic.jikwe = (cStatic *)GetWindowForID(CI_CHARJIKWE);
#ifndef _JAPAN_LOCAL_
	m_ppStatic.fame = (cStatic *)GetWindowForID(CI_CHARFAME);	// ��
	m_ppStatic.spname = (cStatic *)GetWindowForID(CI_SPCHARNAME);	// ��ȣ
#else
	m_ppStatic.attrib = (cStatic *)GetWindowForID(CI_CHARATTRIB);
#endif
	m_ppStatic.badfame = (cStatic *)GetWindowForID(CI_CHARBADFAME);
	m_ppStatic.name = (cStatic *)GetWindowForID(CI_CHARNAME);
	m_ppStatic.stage = (cStatic *)GetWindowForID(CI_CHARSTAGE);		// ����

//	m_ppStatic.job = (cStatic *)GetWindowForID(CI_CHARJOB);		// ����
//	m_ppStatic.ideology = (cStatic *)GetWindowForID(CI_CHARIDEOLOGY);	// ��ġ��
	
	m_ppStatic.genGoal = (cStatic *)GetWindowForID(CI_CHARGENGOAL);
	m_ppStatic.simmak = (cStatic *)GetWindowForID(CI_CHARSIMMAK);
	m_ppStatic.minchub = (cStatic *)GetWindowForID(CI_CHARDEX);
	m_ppStatic.cheryuk = (cStatic *)GetWindowForID(CI_CHARSTA);
	m_ppStatic.level = (cStatic *)GetWindowForID(CI_CHARLEVEL);
	m_ppStatic.expPercent = (cStatic *)GetWindowForID(CI_CHAREXPPERCENT);
	m_ppStatic.point = (cStatic *)GetWindowForID(CI_CHARPOINT);		// �ܿ�����Ʈ
	
	m_ppStatic.meleeattack = (cStatic *)GetWindowForID(CI_CHARATTACK);	// ���ݷ�
	m_ppStatic.rangeattack = (cStatic *)GetWindowForID(CI_LONGATTACK);	// ���ݷ�
	m_ppStatic.defense = (cStatic *)GetWindowForID(CI_CHARDEFENSE);
//	m_ppStatic.kyeongkong = (cStatic *)GetWindowForID(CI_CHARKYEONGKONG);
//	m_ppStatic.move = (cStatic *)GetWindowForID(CI_CHARMOVE);
	m_ppStatic.life = (cStatic *)GetWindowForID(CI_CHARLIFE);
	m_ppStatic.Shield = (cStatic *)GetWindowForID(CI_HOSINDEFENSE);
	m_ppStatic.naeryuk = (cStatic *)GetWindowForID(CI_CHARNAERYUK);
	
	m_ppStatic.critical = (cStatic *)GetWindowForID(CI_CRITICAL);	
	m_ppStatic.attackdistance = (cStatic *)GetWindowForID(CI_DISTANCE);	

	m_ppStatic.reset=(cStatic *)GetWindowForID(CI_CHARRESET);         // reset idx set 2014-05-06

	m_ppStatic.idx=(cStatic *)GetWindowForID(CI_CHARIDX);
	
	m_pPointBtn[GENGOL_POINT] = (cButton *)GetWindowForID(CI_GENGOL_POINT);
	m_pPointBtn[SIMMEK_POINT] = (cButton *)GetWindowForID(CI_SIMMEK_POINT);
	m_pPointBtn[MINCHUB_POINT] = (cButton *)GetWindowForID(CI_MINCHUB_POINT);
	m_pPointBtn[CHERYUK_POINT] = (cButton *)GetWindowForID(CI_CHERYUK_POINT);
	
	m_pPointMinusBtn[GENGOL_POINT] = (cButton *)GetWindowForID(CI_GENGOL_POINT2);
	m_pPointMinusBtn[SIMMEK_POINT] = (cButton *)GetWindowForID(CI_SIMMEK_POINT2);
	m_pPointMinusBtn[MINCHUB_POINT] = (cButton *)GetWindowForID(CI_MINCHUB_POINT2);
	m_pPointMinusBtn[CHERYUK_POINT] = (cButton *)GetWindowForID(CI_CHERYUK_POINT2);

	for(int n=0;n<ATTR_MAX;++n)
	{
		m_AttrDefComponent.pStatic.SetElement_Val(ATTR_FIRE+n,(cStatic *)GetWindowForID(CI_CHARHWA+n));
		m_AttrDefComponent.pGuage.SetElement_Val(ATTR_FIRE+n,(cGuagen *)GetWindowForID(CI_DEFENCE_HWA+n));
	}
}
void CCharacterDialog::Init(LONG x, LONG y, WORD wid, WORD hei, cImage * basicImage, LONG ID)
{
	cDialog::Init(x,y,wid,hei,basicImage,ID);
	//m_type = WT_CHARINFODIALOG;
}

void CCharacterDialog::UpdateData()
{
	BASEOBJECT_INFO ObjInfo;
	CHARACTER_TOTALINFO ChaInfo;
	HERO_TOTALINFO HeroInfo;
	OBJECTMGR->GetHero()->GetBaseObjectInfo(&ObjInfo);
	OBJECTMGR->GetHero()->GetCharacterTotalInfo(&ChaInfo);
	OBJECTMGR->GetHero()->GetHeroTotalInfo(&HeroInfo);

	SetIdx();
	
	m_ppStatic.fame->SetStaticValue(HeroInfo.Fame);

	m_ppStatic.name->SetStaticText(ObjInfo.ObjectName);

	m_ppStatic.badfame->SetStaticValue(ChaInfo.BadFame);

	for(int n=0;n<ATTR_MAX;++n)
	{
		WORD value = (WORD)(HERO->GetAttDefense(ATTR_FIRE+n)*100.5);
		value += HERO->GetShopItemStats()->RegistAttr;
		
		float val = 1 + HERO->GetSkillStatsOption()->AttDef;

		if( val < 0 )
			val = 0.0f;

		value = (WORD)(value * val + 0.5);
	
		if(HERO->GetShopItemStats()->RegistAttr)
			m_AttrDefComponent.SetValue(ATTR_FIRE+n, value, TTTC_EXTRAATTR);
		else
			m_AttrDefComponent.SetValue(ATTR_FIRE+n, value, RGB_HALF(221, 217, 158));

	}
	
	if( HeroInfo.KyungGongGrade == 0)
	{
	}
	else
	{
		CKyungGongInfo* pKyungGongInfo = KYUNGGONGMGR->GetKyungGongInfo(HeroInfo.KyungGongGrade);
	}

	SetCritical();
	SetAttackRange();
	SetAttackRate();
	SetDefenseRate();

	if( IsActive() == TRUE )
		RefreshPointInfo();
}

void CCharacterDialog::UpdateForStageAbility()

{
	STATSMGR->CalcCharStats(HERO);
	SetGenGol();
	SetMinChub();
	SetCheRyuk();
	SetSimMek();
}

void CCharacterDialog::SetExpPointPercent(float perc)
{
	static char buf[66];
	sprintf(buf, "[%.2f%%]", perc);

#ifdef _huhu_	
	if(CHEATMGR->IsCheatEnable())
	{
		char buf1[66];
		wsprintf(buf1, " : %I64d/%I64d", HERO->GetExpPoint(), HERO->GetMaxExpPoint());
		strcat(buf, buf1);
	}
#endif

	m_ppStatic.expPercent->SetStaticText(buf);
}
void CCharacterDialog::SetLevel(WORD level)

{
	m_ppStatic.level->SetStaticValue(level);
}
void CCharacterDialog::SetLife(DWORD life)
{
	//GuageOverCheck
	if(life > HERO->GetMaxLife())
		life = HERO->GetMaxLife();

#ifdef _DEBUG
	char buf1[66];
	wsprintf(buf1, "%d/%d", life, HERO->GetMaxLife());
	m_ppStatic.life->SetStaticText(buf1);


#else
	m_ppStatic.life->SetStaticValue(life);
#endif 
	
}
void CCharacterDialog::SetShield(DWORD Shield)

{
	//GuageOverCheck
	if(Shield > HERO->GetMaxShield())
		Shield = HERO->GetMaxShield();

#ifdef _DEBUG
	char buf1[66];
	wsprintf(buf1, "%d/%d", Shield, HERO->GetMaxShield());
	m_ppStatic.Shield->SetStaticText(buf1);

#else
	m_ppStatic.Shield->SetStaticValue(Shield);
#endif 
	
}
void CCharacterDialog::SetNaeRyuk(DWORD naeryuk)
{
	//GuageOverCheck
	if(naeryuk > HERO->GetMaxNaeRyuk())
		naeryuk = HERO->GetMaxNaeRyuk();

#ifdef _DEBUG
	char buf1[66];
	wsprintf(buf1, "%d/%d", naeryuk, HERO->GetMaxNaeRyuk());
	m_ppStatic.naeryuk->SetStaticText(buf1);

#else
	m_ppStatic.naeryuk->SetStaticValue(naeryuk);
#endif 
}

void CCharacterDialog::SetGenGol()
{
	WORD resVal = HERO->GetGenGol();
	m_ppStatic.genGoal->SetStaticValue(resVal);


	if(HERO->GetShopItemStats()->Gengol || HERO->GetAvatarOption()->Gengol)
		m_ppStatic.genGoal->SetFGColor(TTTC_EXTRAATTR/*TTTC_SHOPITEMOPTION*/);

	else
		m_ppStatic.genGoal->SetFGColor(RGB_HALF(221, 217, 158));
}
void CCharacterDialog::SetMinChub()
{
	int resVal = HERO->GetMinChub();
	m_ppStatic.minchub->SetStaticValue(resVal);
	
	if(HERO->GetShopItemStats()->Minchub || HERO->GetAvatarOption()->Minchub)
		m_ppStatic.minchub->SetFGColor(TTTC_EXTRAATTR/*TTTC_SHOPITEMOPTION*/);

	else
		m_ppStatic.minchub->SetFGColor(RGB_HALF(221, 217, 158));
}
void CCharacterDialog::SetCheRyuk()
{
	WORD resVal = HERO->GetCheRyuk();
	m_ppStatic.cheryuk->SetStaticValue(resVal);

	if(HERO->GetShopItemStats()->Cheryuk || HERO->GetAvatarOption()->Cheryuk)
		m_ppStatic.cheryuk->SetFGColor(TTTC_EXTRAATTR/*TTTC_SHOPITEMOPTION*/);
	else
		m_ppStatic.cheryuk->SetFGColor(RGB_HALF(221, 217, 158));
}
void CCharacterDialog::SetSimMek()
{
	WORD resVal = HERO->GetSimMek();
	m_ppStatic.simmak->SetStaticValue(resVal);

	if(HERO->GetShopItemStats()->Simmek || HERO->GetAvatarOption()->Simmek)
		m_ppStatic.simmak->SetFGColor(TTTC_EXTRAATTR/*TTTC_SHOPITEMOPTION*/);
	else
		m_ppStatic.simmak->SetFGColor(RGB_HALF(221, 217, 158));
}
void CCharacterDialog::SetFame(FAMETYPE fame)
{
	m_ppStatic.fame->SetStaticValue(fame); 

	CHAR famerank[MAX_MASTERNAME_LENGTH + 1];
	ZeroMemory(&famerank, sizeof(famerank));
	sprintf(famerank,"%s",FAMEMGR->GetFameNameString(fame));
	m_ppStatic.spname->SetStaticText(famerank);

	STATSMGR->CalcItemStats(HERO);//KIV
	RefreshInfo();//KIV
}
void CCharacterDialog::SetBadFame(FAMETYPE badfame)
{
	m_ppStatic.badfame->SetStaticValue(badfame); 
}

void CCharacterDialog::SetAttackRate()
{
	char buf1[66];
	DWORD minv,maxv;
	minv = HERO->GetPhyAttackPowerMin();
	maxv = HERO->GetPhyAttackPowerMax();

	//////////////////////////////////////////////////////////////////////////
	// 06. 06. 2�� ���� - �̿���
	// ���� ��ȯ �߰�
	// �Ϲ� ���ݷ� ���
	float val = 1 + HERO->GetSkillStatsOption()->BaseAtk;


	if( val < 0 )
		val = 0.0f;

	minv = (DWORD)((minv * val) + 0.5);

	maxv = (DWORD)((maxv * val) + 0.5);
	//////////////////////////////////////////////////////////////////////////
	wsprintf(buf1, "%d ~ %d", minv, maxv);

	if( HERO->GetWeaponEquipType() != WP_GUNG &&
		HERO->GetWeaponEquipType() != WP_AMGI )
	{
		// 2005 ũ�������� �̺�Ʈ �ڵ�
		if( HERO->GetWeaponEquipType() == WP_EVENT || HERO->GetWeaponEquipType() == WP_EVENT_HAMMER )
		{
			m_ppStatic.meleeattack->SetStaticText(buf1);
			m_ppStatic.meleeattack->SetStaticText("-");
		}
		else
		{	
			m_ppStatic.rangeattack->SetStaticText("-");

			m_ppStatic.meleeattack->SetStaticText(buf1);
		}
	}
	else
	{

		m_ppStatic.meleeattack->SetStaticText("-");
		m_ppStatic.rangeattack->SetStaticText(buf1);
	}

	if( HERO->GetAvatarOption()->Attack )
	{
		m_ppStatic.rangeattack->SetFGColor(TTTC_EXTRAATTR/*TTTC_SHOPITEMOPTION*/);
		m_ppStatic.meleeattack->SetFGColor(TTTC_EXTRAATTR/*TTTC_SHOPITEMOPTION*/);
	}
	else
	{
		m_ppStatic.rangeattack->SetFGColor(RGB_HALF(221, 217, 158));
		m_ppStatic.meleeattack->SetFGColor(RGB_HALF(221, 217, 158));
	}
}
void CCharacterDialog::SetDefenseRate()
{
	WORD value = (WORD)HERO->GetPhyDefense();
	value += (value*HERO->GetShopItemStats()->RegistPhys)/100;

	//////////////////////////////////////////////////////////////////////////
	// 06. 06. 2�� ���� - �̿���
	// ���� ��ȯ �߰�
	// ��������
	float val = 1 + HERO->GetSkillStatsOption()->PhyDef;

	if( val < 0 )
		val = 0.0f;

	value = (WORD)(value * val + 0.5);
	//////////////////////////////////////////////////////////////////////////

	m_ppStatic.defense->SetStaticValue(value);

	if(HERO->GetShopItemStats()->RegistPhys)
		m_ppStatic.defense->SetFGColor(TTTC_EXTRAATTR/*TTTC_SHOPITEMOPTION*/);
	else
		m_ppStatic.defense->SetFGColor(RGB_HALF(221, 217, 158));
}

// 2014-05-06 Reset idx Set 2014-05-06

void CCharacterDialog::SetReSet()
{
	WORD resetStage = HERO->GetReset();

	m_ppStatic.reset->SetStaticValue(resetStage);
}

void CCharacterDialog::SetIdx()
{
	m_ppStatic.idx->SetStaticValue(HERO->GetID());
}

void CCharacterDialog::SetCritical()
{
	DWORD resVal1 = HERO->GetCritical();
	DWORD resVal2 = HERO->GetDecisive();
	resVal1 += HERO->GetShopItemStats()->Critical;
	resVal1 += HERO->GetAvatarOption()->Critical;
	resVal2 += HERO->GetShopItemStats()->Decisive;
	resVal2 += HERO->GetAvatarOption()->Decisive;

	char buf1[66];
#ifndef _HK_LOCAL_	//hk block
	wsprintf(buf1, "%d/%d", resVal1, resVal2);
#else

	wsprintf(buf1, "%d", resVal1);
#endif
	m_ppStatic.critical->SetStaticText(buf1);
	
	if(HERO->GetShopItemStats()->Critical || HERO->GetAvatarOption()->Critical
		|| HERO->GetAvatarOption()->Decisive)		

		m_ppStatic.critical->SetFGColor(TTTC_EXTRAATTR/*TTTC_SHOPITEMOPTION*/);
	else
		m_ppStatic.critical->SetFGColor(RGB_HALF(221, 217, 158));
}
void CCharacterDialog::SetAttackRange()
{
	float addRange = HERO->GetAddAttackRange();
	float range;

	if( HERO->GetWeaponEquipType() == WP_GUNG )
	{
		range = SKILLMGR->GetSkillInfo(COMBO_GUNG_MIN)->GetSkillRange();
		m_ppStatic.attackdistance->SetStaticValue((LONG)(addRange + range));
	}
	else if(HERO->GetWeaponEquipType() == WP_AMGI)
	{
		range = SKILLMGR->GetSkillInfo(COMBO_AMGI_MIN)->GetSkillRange();
		m_ppStatic.attackdistance->SetStaticValue((LONG)(addRange + range));
	}
	else if( HERO->GetWeaponEquipType() == WP_EVENT || HERO->GetWeaponEquipType() == WP_EVENT_HAMMER )
	{
		m_ppStatic.attackdistance->SetStaticValue((LONG)(1000));
	}
	else
	{
		m_ppStatic.attackdistance->SetStaticText("-");
	}
		
}
void CCharacterDialog::SetPointLeveling(BOOL val, WORD point)
{
	m_nocoriPoint = point;	
	
	m_ppStatic.point->SetStaticValue( m_nocoriPoint + HERO->GetShopItemStats()->UseStatePoint );
	for(int i = 0 ; i < MAX_BTN_POINT ; i++)
		m_pPointBtn[i]->SetActive(val);
	
	HERO->SetHeroLevelupPoint( point );
}
void CCharacterDialog::SetPointLevelingHide()
{
	WORD lvpoint = 0;
	if( m_MinusPoint != -1 )
		lvpoint = HERO->GetShopItemStats()->UseStatePoint;

	m_ppStatic.point->SetStaticValue( lvpoint );
	for(int i = 0 ; i < MAX_BTN_POINT ; i++)
		m_pPointBtn[i]->SetActive(FALSE);
	
	HERO->SetHeroLevelupPoint( 0 );
}

void CCharacterDialog::RefreshPointInfo()
{
	if( HERO == FALSE )
		return;

	BOOL mval = IsActive();
	if( HERO->GetShopItemStats()->StatePoint < 1 )
		mval = FALSE;

	BOOL pval = IsActive();
	if( pval && (HERO->GetHeroLevelUpPoint() || HERO->GetShopItemStats()->UseStatePoint) )
		pval = TRUE;
	else
		pval = FALSE;

	for(int i = 0 ; i < MAX_BTN_POINT ; i++)
	{
		m_pPointBtn[i]->SetActive( pval );
		m_pPointMinusBtn[i]->SetActive( mval );
	}

	WORD LevelupPoint = (WORD)HERO->GetHeroLevelUpPoint();
	LevelupPoint += HERO->GetShopItemStats()->UseStatePoint;
	m_ppStatic.point->SetStaticValue( LevelupPoint );

	// 
	if( HERO->GetShopItemStats()->StatePoint > 0  )
	{
		m_ppStatic.point->SetFGColor(TTTC_EXTRAATTR/*TTTC_SHOPITEMOPTION*/);
		m_MinusPoint = HERO->GetShopItemStats()->StatePoint;
	}
	else
	{
		m_ppStatic.point->SetStaticValue( HERO->GetHeroLevelUpPoint()+HERO->GetShopItemStats()->UseStatePoint );
		m_ppStatic.point->SetFGColor(RGB_HALF(221, 217, 158));
		m_MinusPoint = -1;
	}
}
void CCharacterDialog::OnAddPoint(BYTE whatsPoint)
{
	MSG_WORD msg;
	msg.Category = MP_CHAR;
	msg.Protocol = MP_CHAR_POINTADD_SYN;
	msg.dwObjectID = HEROID;
	msg.wData = whatsPoint;
	NETWORK->Send(&msg,sizeof(MSG_WORD));
}
void CCharacterDialog::OnMinusPoint(BYTE whatsPoint)
{
	MSG_WORD msg;
	msg.Category = MP_CHAR;
	msg.Protocol = MP_CHAR_POINTMINUS_SYN;
	msg.dwObjectID = HEROID;
	msg.wData = whatsPoint;

	NETWORK->Send(&msg,sizeof(MSG_WORD));
}


void CCharacterDialog::RefreshGuildInfo()
{
	BYTE rank = HERO->GetGuildMemberRank();
	m_ppStatic.munpa->SetStaticText(HERO->GetGuildName());
	if(rank == GUILD_NOTMEMBER)
	{
		m_ppStatic.jikwe->SetStaticText("");
		m_pMunpaMarkDlg->SetActive(FALSE);
	}
	else
	{
		m_pMunpaMarkDlg->SetActive(FALSE); //���־���...
		m_ppStatic.jikwe->SetStaticText( GUILDMGR->GetRankName(HERO->GetGuildMemberRank()) );
	}
#ifndef _JAPAN_LOCAL_
	m_ppStatic.fame->SetStaticValue(HERO->GetFame());
#endif
}

void CCharacterDialog::SetActive( BOOL val )
{
	cDialog::SetActive( val );
	RefreshPointInfo();

	STATSMGR->CalcItemStats(HERO);//KIV
	RefreshInfo();//KIV

//--- main bar icon
	CMainBarDialog* pDlg = GAMEIN->GetMainInterfaceDialog();
	if( pDlg )
	{
//		pDlg->SetPushBarIcon( OPT_CHARACTERDLGICON, m_bActive );

		if( val == TRUE )
			pDlg->SetAlram( OPT_CHARACTERDLGICON, FALSE );
	}	
}

void CCharacterDialog::RefreshInfo()
{
	SetGenGol();
	SetMinChub();
	SetCheRyuk();
	SetSimMek();
	SetAttackRate();

	STATSMGR->CalcCharStats(HERO);
	SetDefenseRate();
	SetCritical();
	SetReSet();   // refresh reset idx 2014-05-06
	SetIdx();
}

void CCharacterDialog::SetStage( BYTE Stage )
{
	switch( Stage )
	{
	case eStage_Normal:		m_ppStatic.stage->SetStaticText( CHATMGR->GetChatMsg(889) );		break;
	case eStage_Hwa:		m_ppStatic.stage->SetStaticText( CHATMGR->GetChatMsg(890) );		break;
	case eStage_Hyun:		m_ppStatic.stage->SetStaticText( CHATMGR->GetChatMsg(891) );		break;
	case eStage_Geuk:		m_ppStatic.stage->SetStaticText( CHATMGR->GetChatMsg(892) );		break;
	case eStage_Tal:		m_ppStatic.stage->SetStaticText( CHATMGR->GetChatMsg(893) );		break;
	}
}




