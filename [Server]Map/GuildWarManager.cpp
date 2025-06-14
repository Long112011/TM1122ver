
#include "stdafx.h"
#include "GuildWarManager.h"
#include "MapDBMsgParser.h"
#include "UserTable.h"
#include "SkillManager_server.h"
#include "ItemManager.h"
#include "..\[CC]Header\GameResourceManager.h"
#include "GuildManager.h"
#include "Network.h"
#include "Player.h"
#include "Guild.h"
#include "GuildUnionManager.h"
#include "GuildUnion.h"



//////////////////////////////////////////////////////////////////////
//    ��սϵͳ
//////////////////////////////////////////////////////////////////////

CGuildWarManager::CGuildWarManager()
{
	pWarState = eGuildState_Wait;
	pPlayerInfoTable.Initialize(1024);
	pGuildInfoTable.Initialize(1024);
	pGuildWarRandInfo = NULL;
	for(int i = 0;i<7;i++)
	{
		pWarLast[i] = FALSE;
		pLog[i] = FALSE;
	}
	pKillSpotfMoneyJB = 0;//��ɱһ�˽�����Ԫ��
	pKillSpotfMoneyPD = 0;;//��ɱһ�˽������ݵ�
	pKillSpotfMoney = 0;;//��ɱһ�˽�������Ϸ��
	pKillSpotfGuildPoint = 0;;//��ɱһ�˽��������ɾ���
	pGuildRandSprotfCount=0;
	pPlayerRandSprotfCount = 0;
	pWarStartTime=0;
	pWarTime=0;
	memset(&pWarMapInfo,0,sizeof(GUILD_WAR_MAPINFO));
	pIsWarMap = FALSE;
}

CGuildWarManager::~CGuildWarManager()
{
	GUILD_WAR_PLAYER_INFO* pPlayerInfo = NULL;
	pPlayerInfoTable.SetPositionHead();
	while(pPlayerInfo = pPlayerInfoTable.GetData())
		pPlayerInfo = NULL;
	pPlayerInfoTable.RemoveAll();

	GUILD_WAR_GUILD_INFO* pGuildInfo = NULL;
	pGuildInfoTable.SetPositionHead();
	while(pGuildInfo = pGuildInfoTable.GetData())
		pGuildInfo = NULL;
	pGuildInfoTable.RemoveAll();

}
void CGuildWarManager::GetGuildSprotf(CMHFile* file)
{
	pGuildRandSprotfCount = file->GetWord();
	if(pGuildRandSprotfCount)
		pGuildWarRandInfo = new GUILD_WAR_RAND_INFO[pGuildRandSprotfCount];
	else
		pGuildWarRandInfo = NULL;//û���������н���
	char pStr[MAX_PATH];

	while(1)
	{
		if (file->IsEOF())
			break;
		file->GetString(pStr);

		if(strcmp(pStr,"{") == 0)
			continue;
		else if(strcmp(pStr,"}") == 0)
			break;
		else if(pStr[0] == '@')
			continue;
		else if(strcmp(pStr,"#GUILDPOINT") == 0)
		{
			for(int i =0;i<pGuildRandSprotfCount;i++)
			{	
				pGuildWarRandInfo[i].pGuildPoint = file->GetDword();//���ɾ���
			}
		}
		else if(strcmp(pStr,"#MASTER") == 0)
		{
			file->GetString();
			for(int i =0;i<pGuildRandSprotfCount;i++)
			{	
				pGuildWarRandInfo[i].pMaster.pMoneyJB = file->GetDword();//Ԫ��
				pGuildWarRandInfo[i].pMaster.pMoneyPD = file->GetDword();//�ݵ�
				pGuildWarRandInfo[i].pMaster.pMoney = file->GetDword();//��Ϸ��
			}
			file->GetString();
		}	
		else if(strcmp(pStr,"#OTHER") == 0)
		{
			file->GetString();
			for(int i =0;i<pGuildRandSprotfCount;i++)
			{	
				pGuildWarRandInfo[i].pOther.pMoneyJB = file->GetDword();//Ԫ��
				pGuildWarRandInfo[i].pOther.pMoneyPD = file->GetDword();//�ݵ�
				pGuildWarRandInfo[i].pOther.pMoney = file->GetDword();//��Ϸ��
			}
			file->GetString();
		}	
	}
}

void CGuildWarManager::GetSprotf(CMHFile* file)
{
	char pStr[MAX_PATH];
	while(1)
	{
		if (file->IsEOF())
			break;
		file->GetString(pStr);

		if(strcmp(pStr,"{") == 0)
			continue;
		else if(strcmp(pStr,"}") == 0)
			break;
		else if(pStr[0] == '@')
			continue;
		else if(strcmp(pStr,"#KILL") == 0)
		{//��ɱ����
			file->GetString();
			pKillSpotfMoneyJB = file->GetDword();
			pKillSpotfMoneyPD = file->GetDword();
			pKillSpotfMoney = file->GetDword();
			pKillSpotfGuildPoint = file->GetDword();
			file->GetString();
		}
		else if(strcmp(pStr,"#PLAYERRAND") == 0)
		{
			pPlayerRandSprotfCount = file->GetWord();
			if(pPlayerRandSprotfCount)
				pPlayerWarRandInfo = new PLAYER_WAR_RAND_INFO[pPlayerRandSprotfCount];
			else
				pPlayerWarRandInfo = NULL;//û���������н���

			file->GetString();
			for(int j = 0;j<pPlayerRandSprotfCount;j++)
			{
				pPlayerWarRandInfo[j].pMoneyJB = file->GetDword();
				pPlayerWarRandInfo[j].pMoneyPD = file->GetDword();
				pPlayerWarRandInfo[j].pMoney = file->GetDword();
			}
			file->GetString();
		}
		else if(strcmp(pStr,"#GUILDRAND") == 0)
		{
			GetGuildSprotf(file);
			//pGuildRandSprotfCount = file->GetWord();
			//if(pGuildRandSprotfCount)
			//	pGuildWarRandInfo = new GUILD_WAR_RAND_INFO[pGuildRandSprotfCount];
			//else
			//	pGuildWarRandInfo = NULL;//û���������н���

			//file->GetString();
			//for(int j = 0;j<pGuildRandSprotfCount;j++)
			//{
			//	pGuildWarRandInfo[j].pMoneyJB = file->GetDword();
			//	pGuildWarRandInfo[j].pMoneyPD = file->GetDword();
			//	pGuildWarRandInfo[j].pMoney = file->GetDword();
			//	pGuildWarRandInfo[j].pGuildPoint = file->GetDword();
			//}
			//file->GetString();
		}
	}
}

//���ػ��Ϣ
void CGuildWarManager::Init()
{
	CMHFile file;
	file.Init("Resource/Server/GuildWarInfo.bin","rb");
	if(file.IsInited() == FALSE)
	{
		ASSERT(0);
		return ;
	}

	char pStr[MAX_PATH];

	while (1)
	{
		if(file.IsEOF())
			break;

		file.GetString(pStr);

		if(strcmp(pStr,"{") == 0)
			continue;
		else if(strcmp(pStr,"}") == 0)
			break;
		else if(pStr[0] == '@')
			continue;
		else if(strcmp(pStr,"#MAP") == 0)
		{//WarMap
			file.GetString();
			pWarMapInfo.pMapNum = file.GetWord();
			pWarMapInfo.pChannnelNum = file.GetWord();
			pWarMapInfo.pLimitZhuanSheng = file.GetWord();
			pWarMapInfo.pLimitLevel = file.GetWord();
			pWarMapInfo.pClearMonster = file.GetWord();
			pWarMapInfo.pRangeLevel = file.GetWord();
			file.GetString();
			if(g_pServerSystem->GetMapNum() != pWarMapInfo.pMapNum)
			{
				pIsWarMap = FALSE;
				GetWarState();
			}
			else
				pIsWarMap = TRUE;

			if(pIsWarMap)
				ChangeWarState(eGuildState_Wait,TRUE,pWarMapInfo.pMapNum,pWarMapInfo.pChannnelNum);

		}
		else if(strcmp(pStr,"#STARTTIME") == 0)
		{//Process time
			file.GetString();
			for (int i = 0;i < 7 ;i++)
			{
				file.GetWord();
				pWarTimeInfo[i].pOpen = file.GetBool();
				WORD pHour = file.GetWord();
				WORD pMin	= file.GetWord();
				WORD pSen	= file.GetWord();
				pWarTimeInfo[i].pStartTime.SetTime(0,0,0,pHour,pMin,pSen);
				pWarTimeInfo[i].pWarTime = file.GetWord() * 60000;//min to sec
			}
			file.GetString();
		}
		else if(strcmp(pStr,"#SPROTF") == 0)
		{//Reward
			GetSprotf(&file);
		}
	}

	file.Release();

}	

void CGuildWarManager::Process()
{
	if(!pIsWarMap)
		return;

	AutoRevive();

	SYSTEMTIME st;
	GetLocalTime(&st);
	stTIME pLocTime;
	pLocTime.SetTime(st.wYear - 2000,st.wMonth,st.wDay,st.wHour,st.wMinute,st.wSecond);


	switch(pWarState)
	{
	case eGuildState_Wait:
		{//wait.....
			if(!pWarTimeInfo[st.wDayOfWeek].pOpen)
			{
				if(!pLog[st.wDayOfWeek])
				{
					for(int i = 0;i<7;i++)
						pLog[i] = FALSE;
					pLog[st.wDayOfWeek] = TRUE;
					char pTemp[256]={0};
					sprintf(pTemp,"%d��%d�ղ�������ս",st.wMonth,st.wDay);
					g_Console.LOG(4,pTemp);
				}
				return;//no event available
			}
			stTIME pStartTime;
			pStartTime.SetTime(st.wYear - 2000,st.wMonth,st.wDay,
				pWarTimeInfo[st.wDayOfWeek].pStartTime.GetHour(),
				pWarTimeInfo[st.wDayOfWeek].pStartTime.GetMinute(),
				pWarTimeInfo[st.wDayOfWeek].pStartTime.GetSecond());

			if(!pLog[st.wDayOfWeek])
			{
				for(int i = 0;i<7;i++)
					pLog[i] = FALSE;
				pLog[st.wDayOfWeek] = TRUE;
				char pTemp[256]={0};
				sprintf(pTemp,"%d��%d����սʱ��:%d:%d:%d",st.wMonth,st.wDay,pStartTime.GetHour(),pStartTime.GetMinute(),pStartTime.GetSecond());
				g_Console.LOG(4,pTemp);
			}
			if(pLocTime > pStartTime && !pWarLast[st.wDayOfWeek])
			{//Times up
				if((pLocTime.value - pStartTime.value) * 1000 > pWarTimeInfo[st.wDayOfWeek].pWarTime)
				{
					pWarLast[st.wDayOfWeek] = TRUE;
				}
				else
				{
					RemoveMonster();
					//Clear the previous ranking record
					ClearGuildWarInfo();

					ChangeWarState(eGuildState_WarIng,TRUE,pWarMapInfo.pMapNum,pWarMapInfo.pChannnelNum);

					pSendAllInfoTime = gCurTime;
					//Send opening announcement
					MSG_DWORD3 msg;
					msg.Category = MP_GUILDWAR;
					msg.Protocol = MP_GUILDWAR_NOTE_AGENT;
					msg.dwData1 = pWarState;
					msg.dwData2 = pWarMapInfo.pMapNum;
					msg.dwData3 = pWarMapInfo.pChannnelNum;
					g_Network.Broadcast2AgentServer((char*)&msg,sizeof(msg));

					for(int i = 0;i<7;i++)
						pWarLast[i] = FALSE;
					for(int i = 0;i<3;i++)
						pNote[i] = FALSE;

					pWarStartTime = gCurTime;
					pWarTime = pWarTimeInfo[st.wDayOfWeek].pWarTime - ((pLocTime.value - pStartTime.value) * 1000);//����Ļʱ��
					SendWarInfo();
				}

			}
			else if (pStartTime.value - pLocTime.value < 300 && !pNote[0])
			{
				//���Ϳ�������
				MSG_DWORD msg;
				msg.Category = MP_GUILDWAR;
				msg.Protocol = MP_GUILDWAR_NOTE_AGENT;
				msg.dwData = 300;
				g_Network.Broadcast2AgentServer((char*)&msg,sizeof(msg));
				pNote[0] = TRUE;
			}
			else if (pStartTime.value - pLocTime.value < 900 && !pNote[1])
			{
				//���Ϳ�������
				MSG_DWORD msg;
				msg.Category = MP_GUILDWAR;
				msg.Protocol = MP_GUILDWAR_NOTE_AGENT;
				msg.dwData = 900;
				g_Network.Broadcast2AgentServer((char*)&msg,sizeof(msg));
				pNote[1] = TRUE;
			}
			else if (pStartTime.value - pLocTime.value < 1800 && !pNote[2])
			{
				//���Ϳ�������
				MSG_DWORD msg;
				msg.Category = MP_GUILDWAR;
				msg.Protocol = MP_GUILDWAR_NOTE_AGENT;
				msg.dwData = 1800;
				g_Network.Broadcast2AgentServer((char*)&msg,sizeof(msg));
				pNote[2] = TRUE;
			}
		}
		break;
	case eGuildState_WarIng:
		{//���
			if (gCurTime - pWarStartTime >= pWarTime)
			{//�ʱ�䵽��

				ChangeWarState(eGuildState_End,TRUE,pWarMapInfo.pMapNum,pWarMapInfo.pChannnelNum);

			}
			else if (gCurTime - pSendAllInfoTime > 5000)
			{
				pSendAllInfoTime = gCurTime;
				SendWarInfo();
			}

		}
		break;
	case eGuildState_End:
		{//�����

			MSG_DWORD msg;
			msg.Category = MP_GUILDWAR;
			msg.Protocol = MP_GUILDWAR_NOTE_AGENT;
			msg.dwData = pWarState;
			g_Network.Broadcast2AgentServer((char*)&msg,sizeof(msg));

			//���˰��¼���ݿ�
			SavePlayerKillList();
			//�������н���
			WarEndSpotf();

			pWarLast[st.wDayOfWeek] = TRUE;//��¼���ջ��ִ��

			pRemoveTime = gCurTime;
			ChangeWarState(eGuildState_Remove,TRUE,pWarMapInfo.pMapNum,pWarMapInfo.pChannnelNum);
		}
		break;
	case eGuildState_Remove:
		{
			if(gCurTime - pRemoveTime >= 60000)
			{
				RemoveAllPlayer();

				ChangeWarState(eGuildState_Wait,TRUE,pWarMapInfo.pMapNum,pWarMapInfo.pChannnelNum);
				pRemoveTime = 0;
				//��ձ��λ����
				Relase();
			}
		}
		break;
	}

}

//  
void CGuildWarManager::AutoRevive()
{
	CPlayer* pPlayer = NULL;
	g_pUserTable->SetPositionUserHead();
	while(pPlayer = (CPlayer*)g_pUserTable->GetData())
	{
		if(pPlayer->GetObjectKind() == eObjectKind_Player)
		{
			if(pPlayer->GetChannelID() != pWarMapInfo.pChannnelNum)
				continue;
			if (pPlayer->GetState() != eObjectState_Die)
				continue;
			if(gCurTime - pPlayer->GetGuildWarDieTime() >= 60000)
			{
				ReturnToMap(pPlayer);
			}
		}
	}
}


void CGuildWarManager::RemoveMonster()
{
	if(!IsWarMap())
		return;

	CObject* pObject = NULL;
	g_pUserTable->SetPositionUserHead();
	while(pObject = g_pUserTable->GetData())
	{
		if(pObject->GetObjectKind() & eObjectKind_Monster)
		{
			if(pObject->GetGridID() != pWarMapInfo.pChannnelNum)
				continue;

			g_pServerSystem->RemoveMonster(pObject->GetID());
		}
	}
}
//��ͣˢ��
BOOL CGuildWarManager::SuspendMonster(WORD pChannel)
{
	if(!pWarMapInfo.pClearMonster)
		return FALSE;
	if(IsWarMap() && pChannel == pWarMapInfo.pChannnelNum)
		return TRUE;
	return FALSE;
}

void CGuildWarManager::ReturnToMap(CPlayer* pPlayer)
{
	/*MSG_DWORD msg;
	msg.Category = MP_SURVIVAL;
	msg.Protocol = MP_SURVIVAL_RETURNTOMAP;
	msg.dwData = nanju;
	g_pServerSystem->RemovePlayer( pPlayer->GetID() );*/

	/*MSG_DWORD2 msg;
	msg.Category=MP_CHAR;
	msg.Protocol=MP_CHAR_FB_TIME_NACK;
	msg.dwData1=-1;
	msg.dwData2=nanju;

	pPlayer->SendMsg(&msg, sizeof(msg));*/

	MAPCHANGE_INFO* ChangeInfo = GAMERESRCMNGR->GetMapChangeInfoForDestMapNum( nanju );
	if( !ChangeInfo ) return;
	SaveMapChangePointUpdate( pPlayer->GetID(), ChangeInfo->Kind );

	pPlayer->UpdateLogoutToDB();
			
	g_pServerSystem->RemovePlayer( pPlayer->GetID() );

	MSG_WORD2 msg;
	msg.Category	= MP_USERCONN;
	msg.Protocol	= MP_USERCONN_MAP_OUT_WITHMAPNUM;
	msg.dwObjectID	= pPlayer->GetID();
	msg.wData1		= nanju;
	msg.wData2		= 1;

	g_Network.Broadcast2AgentServer( (char*)&msg, sizeof(msg));

}

void CGuildWarManager::RemoveAllPlayer()
{
	CPlayer* pPlayer = NULL;
	g_pUserTable->SetPositionUserHead();
	while(pPlayer = (CPlayer*)g_pUserTable->GetData())
	{
		if(pPlayer->GetObjectKind() != eObjectKind_Player)
			continue;
		if(pPlayer->GetChannelID() != pWarMapInfo.pChannnelNum)
			continue;
		ReturnToMap(pPlayer);
	}
}

void CGuildWarManager::GetWarState()
{
	MSG_DWORD msg;
	msg.Category = MP_GUILDWAR;
	msg.Protocol = MP_GUILDWAR_GET_WARSTATE;
	g_Network.Broadcast2AgentServer((char*)&msg,sizeof(msg));
}

void CGuildWarManager::PushWarState()
{
	if(!IsWarMap())
		return;
	ChangeWarState(pWarState,TRUE,pWarMapInfo.pMapNum,pWarMapInfo.pChannnelNum);
}
//ͬ����ս״̬��Ϣ
void CGuildWarManager::ChangeWarState(BYTE pState,BOOL pSend,WORD pMapNum,WORD pChannelNum)
{
	pWarState = pState;

	if(pMapNum)
		pWarMapInfo.pMapNum = pMapNum;

	if(pChannelNum)
		pWarMapInfo.pChannnelNum = pChannelNum;

	if (pSend)
	{
		MSG_DWORD3 msg;
		msg.Category = MP_GUILDWAR;
		msg.Protocol = MP_GUILDWAR_CHANGESTATE;
		msg.dwData1 = pWarState;
		msg.dwData2 = pWarMapInfo.pMapNum;;
		msg.dwData3 = pWarMapInfo.pChannnelNum;
		g_Network.Broadcast2AgentServer((char*)&msg,sizeof(msg));
	}
}

void CGuildWarManager::SavePlayerKillList()
{

	GUILD_WAR_PLAYER_INFO* pPlayerInfo = NULL;
	pPlayerInfoTable.SetPositionHead();
	while(pPlayerInfo = pPlayerInfoTable.GetData())
	{
		if(pPlayerInfo->pPlayerKillCount)
		{
			if(pPlayerInfo->pUniconIDX)
				InitGuildWarPlayerInfo(pPlayerInfo->pCharacterIDX,pPlayerInfo->pCharacterName,GUILDUNIONMGR->GetGuildUnion(pPlayerInfo->pUniconIDX)->GetGuildUnionName(),pPlayerInfo->pPlayerKillCount,pPlayerInfo->pDieCount);
		}
	}

	GUILD_WAR_GUILD_INFO* pGuildInfo = NULL;
	pGuildInfoTable.SetPositionHead();
	while(pGuildInfo = pGuildInfoTable.GetData())
	{
		if(pGuildInfo->pGuildKillCount && pGuildInfo->pGuildUnion)
		{
			InitGuildWarUnionInfo(pGuildInfo->pGuildUnion->GetGuildUnionIdx(),pGuildInfo->pGuildUnion->GetGuildUnionName(),pGuildInfo->pGuildKillCount,pGuildInfo->pGuildDieCount);
		}
	}
}

//����� ���㽱��
void CGuildWarManager::WarEndSpotf()
{
	if(pPlayerRandSprotfCount)
	{//�����������һ�ɱ����
		GUILD_WAR_PLAYER_INFO* pPlayerInfo = NULL;
		GUILD_WAR_PLAYER_INFO* pPlayerRandList = new GUILD_WAR_PLAYER_INFO[pPlayerRandSprotfCount];
		DWORD pMaxKill = 0;
		GUILD_WAR_PLAYER_INFO* pMaxKillPlayerInfo = NULL;
		for (int i=0;i<pPlayerRandSprotfCount;i++)
		{
			pPlayerRandList[i].pPlayer = NULL;

			pMaxKill = 0;
			pPlayerInfoTable.SetPositionHead();
			while(pPlayerInfo = pPlayerInfoTable.GetData())
			{
				if(pPlayerInfo->pPlayerKillCount >= pMaxKill)
				{
					pMaxKill = pPlayerInfo->pPlayerKillCount;
					pMaxKillPlayerInfo = pPlayerInfo;
				}
			}
			if(pMaxKill && pMaxKillPlayerInfo && pMaxKillPlayerInfo->pPlayer)
			{
				pPlayerRandList[i].pPlayer = pMaxKillPlayerInfo->pPlayer;
				pPlayerRandList[i].pPlayerKillCount = pMaxKillPlayerInfo->pPlayerKillCount;
				pPlayerRandList[i].pDieCount = pMaxKillPlayerInfo->pDieCount;
				pPlayerInfoTable.Remove(pMaxKillPlayerInfo->pPlayer->GetID());
			}
			pMaxKillPlayerInfo = NULL;
		}

		MSG_GUILD_WAR_PLAYER_INFO msg;
		msg.Category = MP_GUILDWAR;
		msg.Protocol = MP_GUILDWAR_PLAYERRANDLIST;
		for(int i = 0;i<pPlayerRandSprotfCount;i++)
		{
			if(pPlayerRandList[i].pPlayer)
			{
				SafeStrCpy(msg.pPlayerInfo[msg.pCount].pPlayerName,pPlayerRandList[i].pPlayer->GetObjectName(),MAX_NAME_LENGTH+1);
				msg.pPlayerInfo[msg.pCount].pKillCount = pPlayerRandList[i].pPlayerKillCount;

				if(pPlayerWarRandInfo[msg.pCount].pMoneyJB)
				{//����Ԫ������
					pPlayerRandList[i].pPlayer->SetGoldMoney(pPlayerWarRandInfo[msg.pCount].pMoneyJB,eGetGoldMoney);
				}
				if(pPlayerWarRandInfo[msg.pCount].pMoneyPD)
				{//�����ݵ㽱��
					pPlayerRandList[i].pPlayer->SetMallMoney(pPlayerWarRandInfo[msg.pCount].pMoneyPD,2);
				}
				if(pPlayerWarRandInfo[msg.pCount].pMoney)
				{//������Ϸ�ҽ���
					pPlayerRandList[i].pPlayer->SetMoney(pPlayerWarRandInfo[msg.pCount].pMoney,MONEY_ADDITION);
				}
				msg.pCount++;
			}
		}
		//������һ�ɱ���е�ȫ������
		g_Network.Broadcast2AgentServer((char*)&msg,msg.GetSize());

	}
	if(pGuildRandSprotfCount)
	{//������������ɻ�ɱ����
		GUILD_WAR_GUILD_INFO* pGuildInfo = NULL;
		GUILD_WAR_GUILD_INFO* pGuildRandList = new GUILD_WAR_GUILD_INFO[pGuildRandSprotfCount];
		DWORD pMaxKill = 0;
		for (int i=0;i<pGuildRandSprotfCount;i++)
		{
			pGuildRandList[i].pGuildUnion = NULL;

			pMaxKill = 0;
			pGuildInfoTable.SetPositionHead();
			while(pGuildInfo = pGuildInfoTable.GetData())
			{
				if(pGuildInfo->pGuildKillCount && pGuildInfo->pGuildKillCount >= pMaxKill)
				{
					pMaxKill = pGuildInfo->pGuildKillCount;
					pGuildRandList[i].pGuildUnion = pGuildInfo->pGuildUnion;
					pGuildRandList[i].pGuildKillCount = pGuildInfo->pGuildKillCount;
				}
			}
			if(pMaxKill && pGuildRandList[i].pGuildUnion)
			{
				pGuildInfoTable.Remove(pGuildRandList[i].pGuildUnion->GetGuildUnionIdx());
			}
		}

		MSG_GUILD_WAR_GUILD_INFO msg;
		msg.Category = MP_GUILDWAR;
		msg.Protocol = MP_GUILDWAR_GUILDRANDLIST;
		for(int i = 0;i<pGuildRandSprotfCount;i++)
		{
			if(pGuildRandList[i].pGuildUnion)
			{
				SafeStrCpy(msg.pGuildInfo[msg.pCount].pGuildName,pGuildRandList[i].pGuildUnion->GetGuildUnionName(),MAX_GUILD_NAME+1);
				msg.pGuildInfo[msg.pCount].pKillCount = pGuildRandList[i].pGuildKillCount;
				msg.pGuildInfo[msg.pCount].pGuildIDX = pGuildRandList[i].pGuildUnion->GetGuildUnionIdx();
				pGuildRandList[i].pGuildUnion->GuildUnionWarSprotf(&pGuildWarRandInfo[msg.pCount]);

				msg.pCount++;
			}
		}
		//������һ�ɱ���е�ȫ������
		g_Network.Broadcast2AgentServer((char*)&msg,msg.GetSize());
	}
}


void CGuildWarManager::Relase()
{
	GUILD_WAR_PLAYER_INFO* pPlayerInfo = NULL;
	pPlayerInfoTable.SetPositionHead();
	while(pPlayerInfo = pPlayerInfoTable.GetData())
	{
		pPlayerInfo->pPlayerKillCount = 0;
		pPlayerInfo->pDieCount = 0;
	}
	GUILD_WAR_GUILD_INFO* pGuildInfo = NULL;
	pGuildInfoTable.SetPositionHead();
	while(pGuildInfo = pGuildInfoTable.GetData())
		pGuildInfo->pGuildKillCount = 0;

}


void CGuildWarManager::KillPlayer(CPlayer* pAttackPlayer,CPlayer* pDiePlayer)
{//a player was killed
	if(!pIsWarMap)
		return;
	if(pWarState != eGuildState_WarIng)
		return;
	if(pAttackPlayer->GetChannelID() != pWarMapInfo.pChannnelNum || pDiePlayer->GetChannelID() != pWarMapInfo.pChannnelNum  )
		return;

	pDiePlayer->SetGuildWarDieTime(gCurTime);

	if(abs(pAttackPlayer->GetLevel() - pDiePlayer->GetLevel()) > pWarMapInfo.pRangeLevel)
		return;

	GUILD_WAR_PLAYER_INFO* pPlayerInfo = pPlayerInfoTable.GetData(pAttackPlayer->GetID());
	if(!pPlayerInfo)
	{
		pPlayerInfo = new GUILD_WAR_PLAYER_INFO;
		pPlayerInfo->pPlayerKillCount = 0;
		pPlayerInfo->pDieCount = 0;
		pPlayerInfoTable.Add(pPlayerInfo,pAttackPlayer->GetID());
	}
	pPlayerInfo->pPlayerKillCount++;//Killer kills++

	GUILD_WAR_PLAYER_INFO* pDiePlayerInfo = pPlayerInfoTable.GetData(pDiePlayer->GetID());
	if(!pDiePlayerInfo)
	{
		pDiePlayerInfo = new GUILD_WAR_PLAYER_INFO;
		pDiePlayerInfo->pPlayerKillCount = 0;
		pDiePlayerInfo->pDieCount = 0;
		pPlayerInfoTable.Add(pDiePlayerInfo,pDiePlayer->GetID());
	}
	pDiePlayerInfo->pDieCount++;//number of deaths++

	if(pAttackPlayer->GetGuildUnionIdx())
	{
		CGuildUnion* pGuildUnion = GUILDUNIONMGR->GetGuildUnion(pAttackPlayer->GetGuildUnionIdx());
		if(pGuildUnion)
		{
			GUILD_WAR_GUILD_INFO* pGuildUnionInfo = pGuildInfoTable.GetData(pGuildUnion->GetGuildUnionIdx());
			if(!pGuildUnionInfo)
			{
				pGuildUnionInfo = new GUILD_WAR_GUILD_INFO;
				pGuildUnionInfo->pGuildKillCount = 0;
				pGuildUnionInfo->pGuildDieCount = 0;
				pGuildUnionInfo->pGuildUnion = pGuildUnion;
				pGuildInfoTable.Add(pGuildUnionInfo,pGuildUnion->GetGuildUnionIdx());
			}
			pGuildUnionInfo->pGuildKillCount++;//Killer Faction Kills++



			if(pKillSpotfGuildPoint)
			{//Rewards Killer Sect Gangs
				CGuild* pGuild = GUILDMGR->GetGuild(pAttackPlayer->GetGuildIdx());
				if(pGuild)
					GuildAddGuildPoint(pGuild->GetIdx(),pKillSpotfGuildPoint,eGuildWar,pAttackPlayer->GetID());
			}
		}
	}
	if(pDiePlayer->GetGuildUnionIdx())
	{
		CGuildUnion* pGuildUnion = GUILDUNIONMGR->GetGuildUnion(pDiePlayer->GetGuildUnionIdx());
		if(pGuildUnion)
		{
			GUILD_WAR_GUILD_INFO* pGuildInfo = pGuildInfoTable.GetData(pGuildUnion->GetGuildUnionIdx());
			if(!pGuildInfo)
			{
				pGuildInfo = new GUILD_WAR_GUILD_INFO;
				pGuildInfo->pGuildKillCount = 0;
				pGuildInfo->pGuildDieCount = 0;
				pGuildInfo->pGuildUnion = pGuildUnion;
				pGuildInfoTable.Add(pGuildInfo,pGuildUnion->GetGuildUnionIdx());
			}
			pGuildInfo->pGuildDieCount++;//Killer Faction Kills++
		}
	}


	if(pKillSpotfMoneyJB)
	{//Rewards Killer ingots
		pAttackPlayer->SetGoldMoney(pKillSpotfMoneyJB,eGetGoldMoney);
	}
	if(pKillSpotfMoneyPD)
	{//Rewards Killer Bubble Points
		pAttackPlayer->SetMallMoney(pKillSpotfMoneyPD,2);
	}
	if(pKillSpotfMoney)
	{//Rewards Killer Coins
		pAttackPlayer->SetMoney(pKillSpotfMoney,MONEY_ADDITION);
	}
	//pDiePlayer->SetGuildWarDieTime(gCurTime);//kiv
}

void CGuildWarManager::SendWarInfo(CPlayer* pPlayer)
{//Send activity scores
	MSG_GUILD_WAR_GUILD_INFO msg;
	msg.Category = MP_GUILDWAR;
	msg.Protocol = MP_GUILDWAR_GUILDWARINFO;
	msg.pState = pWarState;
	msg.pLostTime = (pWarTime - (gCurTime - pWarStartTime))/1000;

	GUILD_WAR_GUILD_INFO* pGuildInfo = NULL;
	pGuildInfoTable.SetPositionHead();
	while(pGuildInfo = pGuildInfoTable.GetData())
	{
		msg.pGuildInfo[msg.pCount].pGuildIDX = pGuildInfo->pGuildUnion->GetGuildUnionIdx();
		msg.pGuildInfo[msg.pCount].pKillCount = pGuildInfo->pGuildKillCount;
		msg.pGuildInfo[msg.pCount].pDieCount = pGuildInfo->pGuildDieCount;
		SafeStrCpy(msg.pGuildInfo[msg.pCount++].pGuildName,pGuildInfo->pGuildUnion->GetGuildUnionName(),MAX_GUILD_NAME+1);
	}
	//�����������ɵ�ǰ����
	pPlayer->SendMsg(&msg,msg.GetSize());


	GUILD_WAR_PLAYER_INFO* pPlayrInfo= pPlayerInfoTable.GetData(pPlayer->GetID());
	if(pPlayrInfo)
	{
		MSG_DWORD3 pPlayerMsg;
		pPlayerMsg.Category = MP_GUILDWAR;
		pPlayerMsg.Protocol = MP_GUILDWAR_CPLAYERWARINFO;
		pPlayerMsg.dwData1 = pPlayrInfo->pPlayerKillCount;
		pPlayerMsg.dwData2 = pPlayrInfo->pDieCount;
		pPlayerMsg.dwData3 = pWarState;
		//������Ҹ��˷���
		pPlayer->SendMsg(&pPlayerMsg,sizeof(pPlayerMsg));
	}

}

void CGuildWarManager::SendWarInfo()
{//�������˷��ͻ����
	CPlayer* pPlayer = NULL;
	g_pUserTable->SetPositionHead();
	while(pPlayer = (CPlayer*)g_pUserTable->GetData())
	{
		if(pPlayer->GetObjectKind() != eObjectKind_Player)
			continue;
		if(pPlayer->GetChannelID() != pWarMapInfo.pChannnelNum)
			continue;
		SendWarInfo(pPlayer);
	}

}

void CGuildWarManager::AddPlayer(CPlayer* pPlayer)
{//����볡  ���͵�ǰ�����
	if(!pIsWarMap || pPlayer->GetChannelID() != pWarMapInfo.pChannnelNum)
	{
		SYSTEMTIME st;
		GetLocalTime(&st);
		if(pWarState == eGuildState_WarIng)
		{
			MSG_DWORD3 msg;
			msg.Category = MP_GUILDWAR;
			msg.Protocol = MP_GUILDWAR_NOTE_USER;
			msg.dwData1 = eGuildState_WarIng;
			msg.dwData2 = pWarMapInfo.pMapNum;
			msg.dwData3 = pWarMapInfo.pChannnelNum;
			pPlayer->SendMsg(&msg,sizeof(msg));
		}
		else if(pWarTimeInfo[st.wDayOfWeek].pOpen && st.wHour < pWarTimeInfo[st.wDayOfWeek].pStartTime.GetHour())
		{//��������ս
			MSG_DWORD2 msg;
			msg.Category = MP_GUILDWAR;
			msg.Protocol = MP_GUILDWAR_DAY_INFO_SYN;
			msg.dwData1 = pWarTimeInfo[st.wDayOfWeek].pStartTime.GetHour();
			msg.dwData2 = pWarTimeInfo[st.wDayOfWeek].pStartTime.GetMonth();
			pPlayer->SendMsg(&msg,sizeof(msg));
		}
		return;
	}

	GUILD_WAR_PLAYER_INFO* pInfo = pPlayerInfoTable.GetData(pPlayer->GetID());
	if(pInfo)
	{
		pInfo->pPlayer = pPlayer;
		SafeStrCpy(pInfo->pCharacterName,pPlayer->GetObjectName(),MAX_NAME_LEN+1);
		pInfo->pUniconIDX = pPlayer->GetGuildUnionIdx();
	}
	else
	{
		pInfo = new GUILD_WAR_PLAYER_INFO;
		pInfo->pCharacterIDX = pPlayer->GetID();
		SafeStrCpy(pInfo->pCharacterName,pPlayer->GetObjectName(),MAX_NAME_LEN+1);
		pInfo->pUniconIDX = pPlayer->GetGuildUnionIdx();
		pInfo->pPlayer = pPlayer;
		pInfo->pPlayerKillCount = 0;
		pInfo->pDieCount = 0;
		pPlayerInfoTable.Add(pInfo,pPlayer->GetID());
	}
	if(pPlayer->GetGuildUnionIdx())
	{
		GUILD_WAR_GUILD_INFO* pGuildInfo = pGuildInfoTable.GetData(pPlayer->GetGuildUnionIdx());
		if(!pGuildInfo)
		{
			pGuildInfo = new GUILD_WAR_GUILD_INFO;
			pGuildInfo->pGuildUnion = GUILDUNIONMGR->GetGuildUnion(pPlayer->GetGuildUnionIdx());
			if(pGuildInfo->pGuildUnion)
			{
				pGuildInfo->pGuildKillCount = 0;
				pGuildInfo->pGuildDieCount = 0;
				pGuildInfoTable.Add(pGuildInfo,pPlayer->GetGuildUnionIdx());
			}

		}
		else
		{
			pGuildInfo->pGuildUnion = GUILDUNIONMGR->GetGuildUnion(pPlayer->GetGuildUnionIdx());
		}
	}

	////ǿ�ƿ���
	//if(pPlayer->PKModeOn() == ePKCODE_OK)
	//{
	//	MSG_DWORD PKmsg;
	//	PKmsg.Category	= MP_PK;
	//	PKmsg.Protocol	= MP_PK_PKON_ACK;
	//	PKmsg.dwData		= pPlayer->GetID();
	//	PKmsg.dwObjectID	= pPlayer->GetID();
	//	pPlayer->SendMsg( &PKmsg, sizeof(PKmsg) );	

	//}

	SendWarInfo(pPlayer);
}

void CGuildWarManager::RemovePlayer(CPlayer* pPlayer)
{
	if(!pIsWarMap || pPlayer->GetChannelID() != pWarMapInfo.pChannnelNum)
		return;

	GUILD_WAR_PLAYER_INFO* pInfo = pPlayerInfoTable.GetData(pPlayer->GetID());
	if(pInfo)
		pInfo->pPlayer = NULL;
}

BOOL CGuildWarManager::MoveMap(CPlayer* pPlayer,WORD pMapNum,WORD pChannelIDX,BYTE pType)
{
	if(pMapNum != pWarMapInfo.pMapNum)
		return TRUE;
	if(pChannelIDX != pWarMapInfo.pChannnelNum)
		return TRUE;

	WORD pError = 0;
	WORD pData = 0;
	if(pWarState != eGuildState_WarIng)
		pError = 4;//�ǽ���ʱ��

	if(!pPlayer->GetGuildUnionIdx())
		pError =  1;//û��ͬ�˲��ܽ���

	if(pPlayer->GetReSet() < pWarMapInfo.pLimitZhuanSheng)
	{
		pError =  2;//ת������
		pData = pWarMapInfo.pLimitZhuanSheng;
	}

	if (pPlayer->GetLevel() < pWarMapInfo.pLimitLevel)
	{
		pError =  3;//�ȼ�����
		pData = pWarMapInfo.pLimitLevel;
	}
	if( eMapChange_Item == pType)
	{
		pError =  5;
		pData = pWarMapInfo.pLimitLevel;
	}

	if(pError)
	{
		MSG_DWORD2 pPlayerMsg;
		pPlayerMsg.Category = MP_GUILDWAR;
		pPlayerMsg.Protocol = MP_GUILDWAR_MOVE_MAP_NACK;
		pPlayerMsg.dwData1 = pError;
		pPlayerMsg.dwData2 = pData;
		pPlayer->SendMsg(&pPlayerMsg,sizeof(pPlayerMsg));
		return FALSE;
	}

	return TRUE;
}