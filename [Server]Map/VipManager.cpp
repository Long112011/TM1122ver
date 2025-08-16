#include "StdAfx.h"
#include "VipManager.h"
#include "MHFile.h"
#include "MapDBMsgParser.h"
#include "ItemManager.h"
#include "Player.h"


GLOBALTON(VipManager)
VipManager::VipManager()
{
	m_VipInfoList.Initialize(MAX_VIP_LEVEL);  //Vip级别最多支持8级
}

VipManager::~VipManager()
{
	VIP_INFO* m_VipInfo = NULL;

	m_VipInfoList.SetPositionHead();

	while (m_VipInfo = m_VipInfoList.GetData())
	{
		if (m_VipInfo)
		{
			delete m_VipInfo;

			m_VipInfo = NULL;
		}
	}
	m_VipInfoList.RemoveAll();
}
void VipManager::ReLoadVipInfo()
{//重加载VIP列表
	VIP_INFO* m_VipInfo = NULL;

	m_VipInfoList.SetPositionHead();

	while (m_VipInfo = m_VipInfoList.GetData())
	{
		if (m_VipInfo)
		{
			delete m_VipInfo;

			m_VipInfo = NULL;
		}
	}
	m_VipInfoList.RemoveAll();
	LoadVipInfo();
}
BOOL VipManager::LoadVipInfo()
{
	char filename[64];
	CMHFile file;

	sprintf(filename, "Resource/VipInfo.bin");
	if (!file.Init(filename, "rb"))
		return FALSE;


	VIP_INFO* m_VipInfo = NULL;

	while (1)
	{
		if (file.IsEOF())
			break;

		if (m_VipInfoList.GetDataNum() > MAX_VIP_LEVEL)
		{
			break;
		}

		ASSERT(!m_VipInfo);

		m_VipInfo = new VIP_INFO;

		m_VipInfo->VipLevel = file.GetWord();

		m_VipInfo->VipMaxGold = file.GetDword();

		m_VipInfo->VipImgIdx = file.GetDword();

		m_VipInfo->VipMallMoney = file.GetDword();

		m_VipInfo->VipGoldMoney = file.GetDword();

		m_VipInfo->VipItemCount = file.GetDword();

		for (int i = 0; i < m_VipInfo->VipItemCount; i++)
		{
			m_VipInfo->VipItemIdx[i] = file.GetDword();
		}

		ASSERT(m_VipInfoList.GetData(m_VipInfo->VipLevel) == FALSE);

		m_VipInfoList.Add(m_VipInfo, m_VipInfo->VipLevel);

		m_VipInfo = NULL;
	}

	file.Release();
}
void VipManager::LoadVipInfoFromDB(DWORD CharIdx)
{
	int  VipMaxValue[MAX_VIP_LEVEL];

	VIP_INFO* m_VipInfo = NULL;

	for (int i = 0; i < MAX_VIP_LEVEL; i++)
	{
		m_VipInfo = m_VipInfoList.GetData(i + 1);

		if (m_VipInfo)
		{
			VipMaxValue[i] = m_VipInfo->VipMaxGold;
		}
		else
		{
			VipMaxValue[i] = MAX_DEFAULT_MAX_GOLD;  //设置最大累计元宝9.9E
		}
	}

	LoadVipGoldInfo(CharIdx, VipMaxValue);
}

WORD VipManager::GetVIpLevel(DWORD TotalGold)
{
	if (TotalGold == 0)  //如果没有任何元宝,则VIP等级为0
	{
		return 0;
	}

	VIP_INFO* m_VipInfo = NULL;

	m_VipInfoList.SetPositionHead();  //按顺序读取，第一个满足添加你的VIP

	while (m_VipInfo = m_VipInfoList.GetData())
	{
		if (TotalGold <= m_VipInfo->VipMaxGold)
		{
			return m_VipInfo->VipLevel - 1;
		}
	}

	return MAX_VIP_LEVEL;  //如果超过最高VIP级别  设置获取最高VIP级别
}
void VipManager::DoVipGetItem(DWORD CharIdx, WORD VipLevel)
{
	VIP_INFO* m_VipInfo = m_VipInfoList.GetData(VipLevel);

	if (m_VipInfo)
	{
		LoadVipGoldGetItem(CharIdx, m_VipInfo->VipLevel);
	}
}
void VipManager::DoVipGetItemRet(CPlayer* pPlayer, WORD VipLevel, int Val)
{
	if (!pPlayer)
	{
		return;
	}

	if (Val <= 0)
	{
		MSG_DWORD msg;
		msg.Category = MP_CHAR;
		msg.Protocol = MP_CHAR_VIP_GET_NACK;
		msg.dwObjectID = pPlayer->GetID();
		msg.dwData = Val;
		pPlayer->SendMsg(&msg, sizeof(msg));
	}
	else
	{
		VIP_INFO* m_VipInfo = m_VipInfoList.GetData(VipLevel);

		if (m_VipInfo)
		{
			for (int i = 0; i < m_VipInfo->VipItemCount; i++)
			{
				ITEM_INFO* pItemInfo = ITEMMGR->GetItemInfo(m_VipInfo->VipItemIdx[i]);

				if (!pItemInfo)	continue;

				if (pItemInfo->ItemKind & eSHOP_ITEM)
				{
					ITEMMGR->CheatObtainShopItem(pPlayer, pItemInfo->ItemIdx, 1); //发放数量1 严格意义上要判断包袱格子
				}
				else
				{
					ITEMMGR->CheatObtainItem(pPlayer, pItemInfo->ItemIdx, 1);  //发放数量1 严格意义上要判断包袱格子
				}
			}

			pPlayer->SetMallMoney(m_VipInfo->VipMallMoney, 5);
			pPlayer->SetGoldMoney(m_VipInfo->VipGoldMoney, eVipGet);
			MSG_DWORD2 msg;
			msg.Category = MP_CHAR;
			msg.Protocol = MP_CHAR_VIP_GET_ACK;
			msg.dwObjectID = pPlayer->GetID();
			msg.dwData1 = VipLevel;
			msg.dwData2 = Val;
			pPlayer->SendMsg(&msg, sizeof(msg));
		}
	}
}

