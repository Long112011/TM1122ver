#include "stdafx.h"
#include "FameManager.h"
#include "MapDBMsgParser.h"
#include "Player.h"
#include "..\[CC]Header\GameResourceManager.h"
#include "PackedData.h"
#include "mhFile.h"
CFameManager::CFameManager()
{
	m_bIsUpdated = FALSE;
	m_StartUpdateTime = 0;
	m_UpdatedDate = 0;
	
	m_FameLogoList.Initialize(100);
}
CFameManager::~CFameManager()
{
	FAMEStruct * pStageLogo=NULL;
	m_FameLogoList.SetPositionHead();
	while(pStageLogo=m_FameLogoList.GetData())
	{
		delete pStageLogo;
	}
	m_FameLogoList.RemoveAll();
}


void CFameManager::LoadFameStage()
{
	CMHFile file;
	if(!file.Init("Resource/FameList.bin", "rb"))
		return;

	char line[256] = { 0, };
	FAMEStruct * pInfo;
	while (file.IsEOF() == FALSE)
	{
		file.GetString(line);
		if (line[0] == '@')
		{
			file.GetLineX(line, 256);
			continue;
		}
		if (strcmp(line, "#SUBSTAGE") == 0)
		{
			pInfo = new FAMEStruct;

			pInfo->Idx			= file.GetInt();
			pInfo->FameVal		= file.GetInt();
			SafeStrCpy(pInfo->FameName, file.GetStringInQuotation(), MAX_NAME_LENGTH + 1);
			pInfo->PowerSwitch	= file.GetLong();

			if (pInfo->PowerSwitch)
			{
				for (int i = 0; i<4; i++)
				{
					pInfo->Power[i] = file.GetLong();
				}
			}
			else
			{
				for (int i = 0; i<4; i++)
				{
					pInfo->Power[i] = 0;
				}
			}
			m_FameLogoList.Add(pInfo, pInfo->Idx);
		}
		
		
	}
	//g_Console.LOG(4, "loaded");
	file.Release();

	
}
FAMEStruct * CFameManager::GetFameStage(int ItemIdx)
{
	return m_FameLogoList.GetData(ItemIdx);
}
LONG CFameManager::GetSTR(FAMETYPE pFame)
{
	int fMax=GetFameStage(0)->FameVal;
	for( WORD i = 0; i < fMax+1; ++i )
	{
		if( pFame < GetFameStage(i)->FameVal+1 )
			return GetFameStage(i)->Power[0];

		else if(pFame >= GetFameStage(fMax)->FameVal)
			return GetFameStage(fMax)->Power[0];
	}
}

LONG CFameManager::GetAGI(FAMETYPE pFame)
{
	int fMax=GetFameStage(0)->FameVal;
	for( WORD i = 0; i < fMax+1; ++i )
	{
		if( pFame < GetFameStage(i)->FameVal+1 )
			return GetFameStage(i)->Power[1];

		else if(pFame >= GetFameStage(fMax)->FameVal)
			return GetFameStage(fMax)->Power[1];
	}
}

LONG CFameManager::GetCON(FAMETYPE pFame)
{
	int fMax=GetFameStage(0)->FameVal;
	for( WORD i = 0; i < fMax+1; ++i )
	{
		if( pFame < GetFameStage(i)->FameVal+1 )
			return GetFameStage(i)->Power[2];

		else if(pFame >= GetFameStage(fMax)->FameVal)
			return GetFameStage(fMax)->Power[2];
	}
}

LONG CFameManager::GetINT(FAMETYPE pFame)
{
	int fMax=GetFameStage(0)->FameVal;
	for( WORD i = 0; i < fMax+1; ++i )
	{
		if( pFame < GetFameStage(i)->FameVal+1 )
			return GetFameStage(i)->Power[3];

		else if(pFame >= GetFameStage(fMax)->FameVal)
			return GetFameStage(fMax)->Power[3];
	}
}













void CFameManager::ChangePlayerFame(CPlayer* pPlayer, BYTE eFameCase)
{
	FAMETYPE fameval = 0;
	switch(eFameCase)
	{
	case eFame_beMaster:
		{
			fameval = pPlayer->GetFame() + 30;
		}
		break;
	case eFame_beMember:
		{
			fameval = pPlayer->GetFame() + 10;
		}
		break;
	case eFame_beMembertoSenior:
		{
			fameval = pPlayer->GetFame() + 5;
		}
		break;
	case eFame_beMembertoViceMaster:
		{
			fameval = pPlayer->GetFame() + 15;
		}
		break;
	case eFame_beSeniortoViceMaster:
		{
			fameval = pPlayer->GetFame() + 10;
		}
		break;
	case eFame_beSeniortoMember:
		{
			fameval = pPlayer->GetFame() - 10;
		}
		break;
	case eFame_beViceMastertoSenior:
		{
			fameval = pPlayer->GetFame() - 15;
		}
		break;
	case eFame_beViceMastertoMember:
		{
			fameval = pPlayer->GetFame() - 25;
		}
		break;
	case eFame_beVicemastertoNotmember:
		{
			fameval = pPlayer->GetFame() - 25;
		}
		break;
	case eFame_beSeniortoNotmember:
		{
			fameval = pPlayer->GetFame() - 20;
		}
		break;
	case eFame_beMembertoNotmember:
		{
			fameval = pPlayer->GetFame() - 15;
		}
		break;
	case eFame_BreakupMaster:
		{
			fameval = pPlayer->GetFame() - 70;
		}
		break;
	case eFame_BreakupViceMaster:
		{
			fameval = pPlayer->GetFame() - 30;
		}
		break;
	case eFame_BreakupSenior:
		{
			fameval = pPlayer->GetFame() - 20;
		}
		break;
	case eFame_BreakupMember:
		{
			fameval = pPlayer->GetFame() - 15;
		}
		break;
	}
	SetPlayerFame(pPlayer, fameval);
	FameCharacterUpdate(pPlayer->GetID(), fameval); 
}
void CFameManager::ChangePlayersVSPlayersFame(CPlayer** ppWinPlayers, int WinerNum, CPlayer** ppLosePlayer, int LoserNum, BYTE FameCase)
{
}
void CFameManager::Process()
{
}
BOOL CFameManager::IsTimetoFameUpdate()
{	
	SYSTEMTIME ti;
	GetLocalTime(&ti);
	if(((BYTE)ti.wDay != m_UpdatedDate) && (m_bIsUpdated == TRUE)) 
		m_bIsUpdated = FALSE;
	if((m_bIsUpdated == FALSE) && (ti.wHour >= m_StartUpdateTime))
	{
		m_UpdatedDate = (BYTE)ti.wDay;
		return TRUE;
	}
	return FALSE;
}
void CFameManager::MunpaFameUpdate(WORD MapNum)
{
	m_bIsUpdated = TRUE;
}
void CFameManager::SetPlayerFame(CPlayer* pPlayer, FAMETYPE FameVal)
{
	pPlayer->SetFame(FameVal);
	SendFame(pPlayer, FameVal);	
}
void CFameManager::SendFame(CPlayer* pPlayer, FAMETYPE FameVal)
{
	MSG_FAME msg;
	msg.Category = MP_CHAR;
	msg.Protocol = MP_CHAR_FAME_NOTIFY;
	msg.Fame = FameVal;
	msg.dwObjectID = pPlayer->GetID();
	PACKEDDATA_OBJ->QuickSend( pPlayer, &msg, sizeof(msg) );
}
void CFameManager::SetMunpaFame(DWORD MunpaIDX, FAMETYPE Val)
{
}
void CFameManager::ChangePlayerBadFame(CPlayer* pPlayer, int eBadFameKind)
{
	FAMETYPE BadFame = pPlayer->GetBadFame() + eBadFameKind;
	if( BadFame < 0 ) BadFame = 0;
	else if( BadFame > 1000000000 ) BadFame = 1000000000;
	pPlayer->SetFame( BadFame );
	BadFameCharacterUpdate( pPlayer->GetID(), BadFame ); 
	SendBadFameMsg( pPlayer, BadFame );
}
void CFameManager::SendBadFameMsg(CPlayer* pPlayer, FAMETYPE BadFameVal)
{
	MSG_FAME msg;
	msg.Category	= MP_CHAR;
	msg.Protocol	= MP_CHAR_BADFAME_NOTIFY;
	msg.dwObjectID	= pPlayer->GetID();
	msg.Fame		= BadFameVal;
	PACKEDDATA_OBJ->QuickSend( pPlayer, &msg, sizeof(msg) );
}
void CFameManager::SetPlayerBadFame(CPlayer* pPlayer, FAMETYPE BadFameVal)
{
	pPlayer->SetBadFame(BadFameVal);
}
BOOL CFameManager::BailBadFame(CPlayer* pPlayer, FAMETYPE BailBadFame)
{
	if( pPlayer->GetBadFame() < MIN_BADFAME_FOR_BAIL + BailBadFame )
		return FALSE;
	MONEYTYPE CurMoney = pPlayer->GetMoney();
	if( CurMoney < (MONEYTYPE)(BailBadFame * BAIL_PRICE) )
		return FALSE;
	return TRUE;
}
BOOL CFameManager::SetBadFameByGold(CPlayer* pPlayer, FAMETYPE BailBadFame)
{
	   int  m_BadFame= pPlayer->GetBadFame()-BailBadFame;
       if(BailBadFame<=0)
	   {
		   return FALSE;    
	   }
	   if(pPlayer->GetBadFame()-BailBadFame<0)
	   {
		   return FALSE;   
	   }
	   if(m_BadFame<0)     
	   {
		   return FALSE; 
	   }
	  BadFameCharacterUpdate( pPlayer->GetID(), m_BadFame ); 
	  SetPlayerBadFame(pPlayer,m_BadFame);
	  SendBadFameMsg( pPlayer, m_BadFame);
	  return TRUE;
}