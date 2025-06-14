#include "stdafx.h"
#include "DistributeWay.h"
#include "Party.h"
#include "ItemDrop.h"
#include "ItemManager.h"
#include "GuildManager.h"
#include "..\[CC]Header\GameResourceManager.h"
#include "MapItemDrop.h"	
#include "FortWarManager.h"
#include "ChannelSystem.h"
#define MONSTER_LEVEL 5
CDistributeWay::CDistributeWay()
{
}
CDistributeWay::~CDistributeWay()
{
}
void CDistributeWay::SendToPersonalExp(CPlayer* pReceivePlayer, DWORD Exp)
{
	Exp = (DWORD)(Exp*gEventRate[eEvent_ExpRate]);
	DWORD AddExpFromGuildPlustime = 0;
	if( pReceivePlayer->GetGuildIdx() )
	{
		AddExpFromGuildPlustime = Exp;
		if( FALSE == GUILDMGR->GetGuildPlustimeRt( pReceivePlayer->GetGuildIdx(), eGPT_Exp, &AddExpFromGuildPlustime ) )
		{
			AddExpFromGuildPlustime = 0;
		}
	}
	if( pReceivePlayer->GetShopItemStats()->AddExp )
	{
		DWORD temp = Exp * pReceivePlayer->GetShopItemStats()->AddExp;
		Exp = Exp + temp / 100 + ( temp > temp / 100 * 100 ? 1 : 0 );
	}
	if( pReceivePlayer->GetShopItemStats()->PlustimeExp )
	{
		float tmp = (100+pReceivePlayer->GetShopItemStats()->PlustimeExp)*0.01f;
		Exp = (DWORD)(Exp*tmp);
	}
	Exp += AddExpFromGuildPlustime;
	if (!CHANNELSYSTEM->IsInsDungeonMap())
	Exp = Exp*(CHANNELSYSTEM->GetExpRate(pReceivePlayer->GetChannelID())+0.1f);
	pReceivePlayer->AddPlayerExpPoint(Exp);
}
void CDistributeWay::SendToPersonalAbil(CPlayer* pReceivePlayer, LEVELTYPE MonsterLevel)
{
	if(pReceivePlayer->GetState() == eObjectState_Die)
		return;
	DWORD AbilityExp;
	AbilityExp = CalcObtainAbilityExp(pReceivePlayer->GetMaxLevel(),MonsterLevel);
	if(AbilityExp)
	{
		if( pReceivePlayer->GetStage() != eStage_Normal )
			AbilityExp += 10;
		AbilityExp = (DWORD)(AbilityExp*gEventRate[eEvent_AbilRate]);
		DWORD AddAbilExpFromGuildPlustime = 0;
		if(pReceivePlayer->GetGuildIdx())
		{
			AddAbilExpFromGuildPlustime = AbilityExp;
			if( FALSE == GUILDMGR->GetGuildPlustimeRt( pReceivePlayer->GetGuildIdx(), eGPT_SuRyun, &AddAbilExpFromGuildPlustime ) )
			{
				AddAbilExpFromGuildPlustime = 0;
			}
		}
		if( pReceivePlayer->GetShopItemStats()->AddAbility )
		{
			DWORD temp = AbilityExp * pReceivePlayer->GetShopItemStats()->AddAbility;
			AbilityExp = AbilityExp + temp / 100 + ( temp > temp / 100 * 100 ? 1 : 0 );
		}
		if( pReceivePlayer->GetShopItemStats()->PlustimeAbil )
		{
			float tmp = (100+pReceivePlayer->GetShopItemStats()->PlustimeAbil)*0.01f;
			AbilityExp = (DWORD)(AbilityExp*tmp);
		}
		AbilityExp += AddAbilExpFromGuildPlustime;
		if (!CHANNELSYSTEM->IsInsDungeonMap())
		AbilityExp=AbilityExp*(CHANNELSYSTEM->GetAbilityRate(pReceivePlayer->GetChannelID())+0.1f);
		pReceivePlayer->AddAbilityExp(AbilityExp);
	}
}
DWORD CDistributeWay::CalcObtainAbilityExp(LEVELTYPE KillerLevel,LEVELTYPE MonsterLevel)
{
	if (MonsterLevel + MONSTER_LEVEL < KillerLevel)
		return 0;
	if(KillerLevel + 9 <MonsterLevel)
		MonsterLevel = KillerLevel+ 9;
	return (MonsterLevel - KillerLevel + MONSTER_LEVEL) * 10;
}
void CDistributeWay::SendToPersonalMoney(CPlayer* pPlayer, MONEYTYPE Money, WORD MonsterKind)
{
	DWORD dwMoney = FORTWARMGR->AddProfitMoneyFromMonster( Money );
	if (!CHANNELSYSTEM->IsInsDungeonMap())
	dwMoney =dwMoney*(CHANNELSYSTEM->GetMoneyRate(pPlayer->GetChannelID())+0.1f);
	if( pPlayer->SetMoney(dwMoney, MONEY_ADDITION, 1, eItemTable_Inventory, eMoneyLog_GetMonster, MonsterKind) != dwMoney )
	{
		MSGBASE msg;
		msg.Category = MP_ITEM;
		msg.Protocol = MP_ITEM_MONEY_ERROR;
		msg.dwObjectID = pPlayer->GetID();
		pPlayer->SendMsg(&msg, sizeof(msg));
	}
}
void CDistributeWay::ItemChangeAtLv(WORD& DropItemId)
{
	if((DropItemId%10) != 0) return;
	if(DropItemId >= 11000 && DropItemId <= 28759)
	{
		ITEM_INFO* pItemInfo = ITEMMGR->GetItemInfo(DropItemId);
		if(pItemInfo->ItemKind == eEQUIP_ITEM_WEAPON || pItemInfo->ItemKind == eEQUIP_ITEM_DRESS	||
		   pItemInfo->ItemKind == eEQUIP_ITEM_HAT	 || pItemInfo->ItemKind == eEQUIP_ITEM_SHOES)		
		{
			if(pItemInfo->ItemKind != eEQUIP_ITEM_WEAPON)
			{
				unsigned int selectRatio = 100000;
				unsigned char Data[4];
				Data[0]=rand()%256;
				Data[1]=rand()%256;
				Data[2]=rand()%256;
				Data[3]=rand()%256;
				DWORD *dwRandData=(DWORD *)Data;
				DWORD RandRate = ((*dwRandData)%selectRatio);
				DWORD FromPercent = 0;
				DWORD ToPercent = 0;
				int listIdx = GAMERESRCMNGR->CheckAttribItemIndex(DropItemId);
				if(listIdx != -1)
				{
					for( WORD i = 0; i < 5; ++i )
					{
						FromPercent = ToPercent;
						ToPercent += GAMERESRCMNGR->GetItemChangeRateAttribList()[listIdx].attribinfo[i].changeRate;
						if( FromPercent <= RandRate && RandRate < ToPercent )
						{
							WORD idx = GAMERESRCMNGR->GetItemChangeRateAttribList()[listIdx].attribinfo[i].changeIndex;
							if(idx != 0)
							{
								DropItemId = idx; 
								break;
							}
						}
					}
				}
			}
			if(GAMERESRCMNGR->GetItemChangeRateOfLvList().size() == 0)
				::MessageBox(NULL,"GetItemChangeRateOfLvList","Error",MB_OK);
			if(pItemInfo->LimitLevel <= GAMERESRCMNGR->GetItemChangeRateOfLvList().size())
			{
				unsigned int selectRatio = 100000;
				unsigned char Data[4];
				Data[0]=rand()%256;
				Data[1]=rand()%256;
				Data[2]=rand()%256;
				Data[3]=rand()%256;
				DWORD *dwRandData=(DWORD *)Data;
				DWORD RandRate = ((*dwRandData)%selectRatio); 
				DWORD FromPercent = 0;
				DWORD ToPercent = 0;
				for( WORD i = 0; i < 10; ++i)
				{
					FromPercent = ToPercent;
					ToPercent += GAMERESRCMNGR->GetItemChangeRateOfLvList()[pItemInfo->LimitLevel-1].Rate[i];
					if( FromPercent <= RandRate && RandRate < ToPercent )
					{
						DropItemId = DropItemId + i;
						break;
					}
				}
			}
		}
	}
}
void CDistributeWay::SendToPersonalItem(CPlayer* pPlayer, WORD DropItemId, DWORD DropItemRatio, MONSTEREX_LIST * pMonInfo, WORD MonsterKind)
{
	if(ChkGetItemLevel(pPlayer->GetLevel(), pMonInfo->Level) == FALSE)
		return;
	if(DropItemId == 0)
	{
		MONEYTYPE money = ITEMDROP_OBJ->MoneyItemNoItemPercentCalculator(pMonInfo, pPlayer, MonsterKind);
		MAPITEMDROP_OBJ->CalculateDropRate( pPlayer, MonsterKind );
		if(0 == money) return;
		SendToPersonalMoney(pPlayer, money, MonsterKind);
		BOOL bGetTwice = FALSE;
		pPlayer->GetPetManager()->GetPetBuffResultRt(ePB_Item_DoubleChance, &bGetTwice);
		if(bGetTwice)
		{
			SendToPersonalMoney(pPlayer, money, MonsterKind);
		}
	}
	else
	{
		if( DropItemRatio )
		if( ( rand()%100 ) % ( 100 / DropItemRatio ) == 0 )
		{
			ITEMMGR->MonsterObtainItem(pPlayer,DropItemId,MonsterKind);
			/*sTIPITEMINFO * GetItem = NULL;
			GetItem = ITEMMGR->GetTipItem(DropItemId);
			BASE_MONSTER_LIST* pMonster = GAMERESRCMNGR->GetMonsterListInfo(MonsterKind);
			if(GetItem&&pMonster)
			{
				MSG_TIP msg;
				msg.Category = MP_CLIENT;
				msg.Protocol = MP_CLIENT_MSG_AGENT;
				msg.Flag = eMonsterDropItemTip;
				SafeStrCpy(msg.Name1,pPlayer->GetObjectName(),MAX_NAME_LENGTH+1);
				SafeStrCpy(msg.ItemName,pMonster->Name,MAX_ITEMNAME_LENGTH+1);
				SafeStrCpy(msg.ItemName1,GetItem->ItemName,MAX_ITEMNAME_LENGTH+1);
				pPlayer->SendMsgToAllAgent(&msg, sizeof(msg));
			}*/
			MAPITEMDROP_OBJ->CalculateDropRate( pPlayer, MonsterKind );
		}
	}
}
DWORD dwCheckBit = eTitanMap | eInsDungeon;
BOOL CDistributeWay::ChkGetItemLevel(LEVELTYPE PlayerLvl, LEVELTYPE MonsterLvl)
{
	if( g_pServerSystem->GetMap()->IsMapKind(dwCheckBit) )
	{
		return TRUE;
	}
	else
	{
		if(PlayerLvl > MonsterLvl + 6)
			return FALSE;
		else
			return TRUE;
	}
}
void CDistributeWay::CalcAbilandSend( LEVELTYPE MonsterLevel, PARTY_RECEIVE_MEMBER* pMemberInfo, LEVELTYPE MaxLevel)
{
	DWORD tempoint = 0;
	DWORD AbilPoint = 0;
	DWORD CalAbilPoint = 0;
	BYTE bStage = 0;	
	int nMemberNum = pMemberInfo->count;
	if( nMemberNum == 0 )	return;
	if( MonsterLevel+5 < MaxLevel )	return;
	if( MaxLevel+9 >= MonsterLevel )
	{
		tempoint = (MonsterLevel - MaxLevel + 5)*10;
	}
	else if( MaxLevel+10 == MonsterLevel )
	{
		if( nMemberNum < 3 )
			tempoint = 140;
		else
			tempoint = 150;
	}
	else if( MaxLevel+11 == MonsterLevel )
	{
		if( nMemberNum < 3 )
			tempoint = 140;
		else if( nMemberNum < 5 )
			tempoint = 150;
		else
			tempoint = 160;
	}
	else if( MaxLevel+12 <= MonsterLevel )
	{
		if( nMemberNum < 3 )
			tempoint = 140;
		else if( nMemberNum < 5 )
			tempoint = 150;
		else if( nMemberNum < 7 )
			tempoint = 160;
		else
			tempoint = 170;
	}
	AbilPoint = (DWORD)(( tempoint * (0.9 + (0.1*nMemberNum)) ) / nMemberNum);
	if( AbilPoint == 0 ) return;
	CalAbilPoint = AbilPoint;
	DWORD AddAbilExpFromGuildPlustime = 0;
	CPlayer* pPlayer = NULL;
	for(int i=0; i<nMemberNum; ++i)
	{
		bStage = ((CPlayer*)pMemberInfo->pPlayer[i])->GetStage();
		if( bStage != eStage_Normal )
			CalAbilPoint += 10;
		CalAbilPoint = (DWORD)(CalAbilPoint*gEventRate[eEvent_AbilRate]);
		pPlayer = ((CPlayer*)pMemberInfo->pPlayer[i]);
		if(pPlayer->GetGuildIdx())
		{
			AddAbilExpFromGuildPlustime = CalAbilPoint;
			if( FALSE == GUILDMGR->GetGuildPlustimeRt( pPlayer->GetGuildIdx(), eGPT_SuRyun, &AddAbilExpFromGuildPlustime ) )
			{
				AddAbilExpFromGuildPlustime = 0;
			}
		}
		if( ((CPlayer*)pMemberInfo->pPlayer[i])->GetShopItemStats()->AddAbility )
		{
			DWORD temp = CalAbilPoint * ((CPlayer*)pMemberInfo->pPlayer[i])->GetShopItemStats()->AddAbility;
			CalAbilPoint = CalAbilPoint + temp / 100 + ( temp > temp / 100 * 100 ? 1 : 0 ); 
		}
		if( ((CPlayer*)pMemberInfo->pPlayer[i])->GetShopItemStats()->PlustimeAbil )
		{
			float tmp = (100+((CPlayer*)pMemberInfo->pPlayer[i])->GetShopItemStats()->PlustimeAbil)*0.01f;
			CalAbilPoint = (DWORD)(CalAbilPoint*tmp);
		}
		CalAbilPoint += AddAbilExpFromGuildPlustime;
		((CPlayer*)pMemberInfo->pPlayer[i])->AddAbilityExp(CalAbilPoint);
		CalAbilPoint = AbilPoint;
	}
}
DWORD cRand_Big::RandomVal()
{
	DWORD randValue;
	randValue = ((DWORD(rand() << 16)) + DWORD(rand()));
	return randValue;
}