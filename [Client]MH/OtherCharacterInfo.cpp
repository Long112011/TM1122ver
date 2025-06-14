#include "stdafx.h"
#include "OtherCharacterInfo.h"
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
#include "itemManager.h"

#ifdef _JAPAN_LOCAL_
#include "Hero.h"
#endif
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

/*
//SW0508 惫郴客 崔府 JAPAN Edition 俊辑绰 疙己苞 喊龋啊 绝促.
*/

CCharactorInfoDlg::CCharactorInfoDlg()
{
	m_type = WT_SHOWCHARACTORLISTDLG;

}

CCharactorInfoDlg::~CCharactorInfoDlg()
{

}

void CCharactorInfoDlg::Linking()
{


	m_pMunpaMarkDlg = (CMunpaMarkDialog*)GetWindowForID(CI_TARGETMUNPAMARK);//111
	m_ppStatic.munpa = (cStatic *)GetWindowForID(CI_TARGETCHARMUNPA);//11
	m_ppStatic.jikwe = (cStatic *)GetWindowForID(CI_TARGETCHARJIKWE);//111

	m_ppStatic.fame = (cStatic *)GetWindowForID(CI_TARGETCHARFAME);	// 11
	m_ppStatic.spname = (cStatic *)GetWindowForID(CI_TARGETSPCHARNAME);	// 111

	m_ppStatic.badfame = (cStatic *)GetWindowForID(CI_TARGETCHARBADFAME);//11
	m_ppStatic.name = (cStatic *)GetWindowForID(CI_TARGETCHARNAME);//111
	m_ppStatic.stage = (cStatic *)GetWindowForID(CI_TARGETCHARSTAGE);		// 111

	
	m_ppStatic.genGoal = (cStatic *)GetWindowForID(CI_TARGETCHARGENGOAL);//11
	m_ppStatic.simmak = (cStatic *)GetWindowForID(CI_TARGETCHARSIMMAK);//11
	m_ppStatic.minchub = (cStatic *)GetWindowForID(CI_TARGETCHARDEX);//11
	m_ppStatic.cheryuk = (cStatic *)GetWindowForID(CI_TARGETCHARSTA);//111CI_TARGETCHARDEX
	m_ppStatic.level = (cStatic *)GetWindowForID(CI_TARGETCHARLEVEL );//111
	//m_ppStatic.expPercent = (cStatic *)GetWindowForID(CI_TARGETCHARSIMMAK);//111
	//m_ppStatic.point = (cStatic *)GetWindowForID(CI_TARGETCHAREXPPERCENT);		// 111
	
	m_ppStatic.meleeattack = (cStatic *)GetWindowForID(CI_TARGETCHARATTACK);	// 111	
	m_ppStatic.critical = (cStatic *)GetWindowForID(CI_TARGETCRITICAL);	//111

	
	m_ppStatic.rangeattack = (cStatic *)GetWindowForID(CI_TARGETLONGATTACK);	// 111kong
	//m_ppStatic.attackdistance = (cStatic *)GetWindowForID(CI_TARGETDISTANCE);	//111kong
	
	m_ppStatic.defense = (cStatic *)GetWindowForID(CI_TARGETCHARDEFENSE);//111kong
	m_ppStatic.life = (cStatic *)GetWindowForID(CI_TARGETCHARLIFE);///111kong
	m_ppStatic.Shield = (cStatic *)GetWindowForID(CI_TARGETHOSINDEFENSE);//111kong
	m_ppStatic.naeryuk = (cStatic *)GetWindowForID(CI_TARGETCHARNAERYUK);//111kong

    // 2014-02-26 属性ID转生 

	m_ppStatic.idx=(cStatic *)GetWindowForID(CI_TARGETCHARIDX);

	m_ppStatic.reset=(cStatic *)GetWindowForID(CI_TARGETCHARRESTAGE);



	for(int n=0;n<ATTR_MAX;++n)
	{
		m_AttrDefComponent.pStatic.SetElement_Val(ATTR_FIRE+n,(cStatic *)GetWindowForID(CI_TARGETCHARHWA+n));
		m_AttrDefComponent.pGuage.SetElement_Val(ATTR_FIRE+n,(cGuagen *)GetWindowForID(CI_TARGET_DEFENCE_HWA+n));
	}

}
void CCharactorInfoDlg::Init(LONG x, LONG y, WORD wid, WORD hei, cImage * basicImage, LONG ID)
{
	cDialog::Init(x,y,wid,hei,basicImage,ID);
}

void CCharactorInfoDlg::SetActive( BOOL val )
{
	if( m_bDisable ) return;
	
	CObject* pLastSelectedObject = OBJECTMGR->GetSelectedObject();
	if(pLastSelectedObject && pLastSelectedObject->GetObjectKind() == eObjectKind_Player) 
	{
		CPlayer* pPlayer = (CPlayer*)pLastSelectedObject;
		BASEOBJECT_INFO ObjInfo;
		CHARACTER_TOTALINFO ChaInfo;
		pPlayer->GetBaseObjectInfo(&ObjInfo);
		pPlayer->GetCharacterTotalInfo(&ChaInfo);
		
		BYTE rank = pPlayer->GetGuildMemberRank();
		m_ppStatic.munpa->SetStaticText(pPlayer->GetGuildName());

		m_pMunpaMarkDlg->SetActive(FALSE); //绝局具瘤...
		m_ppStatic.jikwe->SetStaticText( GUILDMGR->GetRankName(pPlayer->GetGuildMemberRank()) );
		

		m_ppStatic.name->SetStaticText(ObjInfo.ObjectName);
		m_ppStatic.badfame->SetStaticValue(ChaInfo.BadFame);
		SetLevel(pPlayer->GetLevel());
		SetLife(pPlayer);
		SetShield(pPlayer->GetShield());
		SetNaeRyuk(pPlayer->GetNaeRyuk());
		SetGenGol(ChaInfo.genGoal);
		SetMinChub(ChaInfo.minchub);
		SetCheRyuk(ChaInfo.cheryuk);
		SetSimMek(ChaInfo.simmak);

		// 2014-02-26 装备转生ID

		SetReStage(ChaInfo.ReStage);

		SetIdx(ObjInfo.dwObjectID);

		//SetExpPointPercent();
		//SetAttackRate();
		//SetDefenseRate();

		SetCritical(ChaInfo.critical);
		//SetAttackRange();
		SetFame(ChaInfo.Fame);
		SetBadFame(pPlayer->GetBadFame());
		SetStage(pPlayer->GetStage());	  
		{
			char buf1[66];
			DWORD minv,maxv;
			minv = ChaInfo.minAttack;
			maxv = ChaInfo.maxAttack;
			wsprintf(buf1, "%d ~ %d", minv, maxv);
			m_ppStatic.meleeattack->SetStaticText(buf1);

		}
		
		{
			WORD value = (WORD)ChaInfo.defenseRate;
			m_ppStatic.defense->SetStaticValue(value);
			m_ppStatic.defense->SetFGColor(RGB_HALF(221, 217, 158));
		}
		
	}

	cDialog::SetActiveRecursive(val);
}


void CCharactorInfoDlg::SetStage( BYTE Stage )
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

// void CCharactorInfoDlg::SetExpPointPercent(float perc)
// {
// 	static char buf[66];
// 	sprintf(buf, "[%.2f%%]", perc);
// 
// #ifdef _CHEATENABLE_	
// 	if(CHEATMGR->IsCheatEnable())
// 	{
// 		char buf1[66];
// 		wsprintf(buf1, " : %I64d/%I64d", HERO->GetExpPoint(), HERO->GetMaxExpPoint());
// 		strcat(buf, buf1);
// 	}
// #endif
// 
// 	m_ppStatic.expPercent->SetStaticText(buf);
// }
void CCharactorInfoDlg::SetLevel(WORD level)
{
	m_ppStatic.level->SetStaticValue(level);
}
void CCharactorInfoDlg::SetLife(CPlayer* pPlayer)
{
	//GuageOverCheck
// 	if(life > HERO->GetMaxLife())
// 		life = HERO->GetMaxLife();

#ifdef _DEBUG
	char buf1[66];
	wsprintf(buf1, "%d/%d", pPlayer->GetLife(), pPlayer->GetMaxLife());
	m_ppStatic.life->SetStaticText(buf1);

#else
	m_ppStatic.life->SetStaticValue(pPlayer->GetLife());
#endif 

}
void CCharactorInfoDlg::SetShield(DWORD Shield)
{
	//GuageOverCheck
// #ifdef _DEBUG
// 	char buf1[66];
// 	wsprintf(buf1, "%d/%d", Shield, HERO->GetMaxShield());
// 	m_ppStatic.Shield->SetStaticText(buf1);
// 
// #else
	m_ppStatic.Shield->SetStaticValue(Shield);
//#endif 

}
void CCharactorInfoDlg::SetNaeRyuk(DWORD naeryuk)
{
	//GuageOverCheck
// 
// #ifdef _DEBUG
// 	char buf1[66];
// 	wsprintf(buf1, "%d/%d", naeryuk, HERO->GetMaxNaeRyuk());
// 	m_ppStatic.naeryuk->SetStaticText(buf1);
// 
// #else
	m_ppStatic.naeryuk->SetStaticValue(naeryuk);
//#endif 
}

void CCharactorInfoDlg::SetGenGol(WORD resVal)
{
//	WORD resVal = HERO->GetGenGol();
	m_ppStatic.genGoal->SetStaticValue(resVal);

// 	if(HERO->GetShopItemStats()->Gengol || HERO->GetAvatarOption()->Gengol)
// 		m_ppStatic.genGoal->SetFGColor(TTTC_SHOPITEMOPTION);
// 	else
 		m_ppStatic.genGoal->SetFGColor(RGB_HALF(221, 217, 158));
}
void CCharactorInfoDlg::SetMinChub(WORD resVal)
{
	//int resVal = HERO->GetMinChub();
	m_ppStatic.minchub->SetStaticValue(resVal);

// 	if(HERO->GetShopItemStats()->Minchub || HERO->GetAvatarOption()->Minchub)
// 		m_ppStatic.minchub->SetFGColor(TTTC_SHOPITEMOPTION);
// 	else
		m_ppStatic.minchub->SetFGColor(RGB_HALF(221, 217, 158));
}
void CCharactorInfoDlg::SetCheRyuk(WORD resVal)
{

	m_ppStatic.cheryuk->SetStaticValue(resVal);


		m_ppStatic.cheryuk->SetFGColor(RGB_HALF(221, 217, 158));
}
void CCharactorInfoDlg::SetSimMek(WORD resVal)
{
	
	m_ppStatic.simmak->SetStaticValue(resVal);


		m_ppStatic.simmak->SetFGColor(RGB_HALF(221, 217, 158));
}
void CCharactorInfoDlg::SetFame(FAMETYPE fame)
{
	m_ppStatic.fame->SetStaticValue(fame); 
}
void CCharactorInfoDlg::SetBadFame(FAMETYPE badfame)
{
	m_ppStatic.badfame->SetStaticValue(badfame); 
}

void CCharactorInfoDlg::SetAttackRate()
{

}
void CCharactorInfoDlg::SetDefenseRate(WORD value)
{


	m_ppStatic.defense->SetStaticValue(value);

	m_ppStatic.defense->SetFGColor(RGB_HALF(221, 217, 158));
}
// 转生 装备 2014-02-26

void CCharactorInfoDlg::SetReStage(BYTE ReStage)
{
	m_ppStatic.reset->SetStaticValue(ReStage);
}
void CCharactorInfoDlg::SetIdx(DWORD Idx)
{
	m_ppStatic.idx->SetStaticValue(Idx);
}

void CCharactorInfoDlg::SetCritical(DWORD resVal1)
{

// 
 	char buf1[66];

	wsprintf(buf1, "%d", resVal1);
//#endif
	m_ppStatic.critical->SetStaticText(buf1);

	m_ppStatic.critical->SetFGColor(RGB_HALF(221, 217, 158));
}
void CCharactorInfoDlg::SetAttackRange(WORD val)
{
	m_ppStatic.attackdistance->SetStaticValue((LONG)(val));
}
void CCharactorInfoDlg::ShowDialog(CPlayer * pPlayer)
{
       if(pPlayer)
	   {
			BASEOBJECT_INFO ObjInfo;
			CHARACTER_TOTALINFO ChaInfo;
			pPlayer->GetBaseObjectInfo(&ObjInfo);
			pPlayer->GetCharacterTotalInfo(&ChaInfo);
		
			BYTE rank = pPlayer->GetGuildMemberRank();
			m_ppStatic.munpa->SetStaticText(pPlayer->GetGuildName());

			m_pMunpaMarkDlg->SetActive(FALSE); //绝局具瘤...
			m_ppStatic.jikwe->SetStaticText( GUILDMGR->GetRankName(pPlayer->GetGuildMemberRank()) );
		

			m_ppStatic.name->SetStaticText(ObjInfo.ObjectName);
			m_ppStatic.badfame->SetStaticValue(ChaInfo.BadFame);
			SetLevel(pPlayer->GetLevel());
			SetLife(pPlayer);
			SetShield(pPlayer->GetShield());
			SetNaeRyuk(pPlayer->GetNaeRyuk());
			SetGenGol(ChaInfo.genGoal);
			SetMinChub(ChaInfo.minchub);
			SetCheRyuk(ChaInfo.cheryuk);
			SetSimMek(ChaInfo.simmak);

			// 2014-02-26 装备转生ID

			SetReStage(ChaInfo.ReStage);

			SetIdx(ObjInfo.dwObjectID);

			//SetExpPointPercent();
			//SetAttackRate();
			//SetDefenseRate();

			SetCritical(ChaInfo.critical);
			//SetAttackRange();
			SetFame(ChaInfo.Fame);
			SetBadFame(pPlayer->GetBadFame());
			SetStage(pPlayer->GetStage());	  
			{
				char buf1[66];
				DWORD minv,maxv;
				minv = ChaInfo.minAttack;
				maxv = ChaInfo.maxAttack;
				wsprintf(buf1, "%d ~ %d", minv, maxv);
				m_ppStatic.meleeattack->SetStaticText(buf1);

			}

			{
				WORD value = (WORD)ChaInfo.defenseRate;
				m_ppStatic.defense->SetStaticValue(value);
				m_ppStatic.defense->SetFGColor(RGB_HALF(221, 217, 158));
			}


			for(int n=0;n<ATTR_MAX;++n)
			{
				WORD value = (WORD)(pPlayer->GetAttDefense(ATTR_FIRE+n)*100.5);

				value += pPlayer->GetShopItemStats()->RegistAttr;   //miningmso 百宝五行属性值叠加 2019-10-30
			
				float val = 1 + pPlayer->GetSkillStatsOption()->AttDef;

				if( val < 0 )
				{
					val = 0.0f;
				}

				value = (WORD)(value * val + 0.5);
			
				//miningmso 百宝五行属性状态附加  2019-10-30
		
				if(pPlayer->GetShopItemStats()->RegistAttr)
				{
					m_AttrDefComponent.SetValue(ATTR_FIRE+n, value, TTTC_SHOPITEMOPTION);	
				}
				else
				{
					m_AttrDefComponent.SetValue(ATTR_FIRE+n, value, RGB_HALF(221, 217, 158));
				}
			}


			cDialog::SetActiveRecursive(TRUE);
		}
}