#include "StdAfx.h"
#include "VipDialog.h"
#include "MHFile.h"
#include "WindowIDEnum.h"
#include "./Interface/cWindowManager.h"
#include "./interface/cButton.h"
#include "cScriptManager.h"
#include "ItemManager.h"
#include "ChatManager.h"
#include "ObjectManager.h"
#include "InventoryExDialog.h"
#include "GameIn.h"
#include "VipDealItem.h"
//VIPϵͳ����ʵ��

VipDialog::VipDialog()
{
	m_VipInfoList.Initialize(MAX_VIP_LEVEL);  //Vip�������֧��10��

	pDlg = NULL;

	pVipImage = NULL;

	pVipMsg = NULL;

	pVipProgress = NULL;

	pVipUpRate = NULL;

	pVipMsg2 = NULL;

	for (int i = 0; i < MAX_VIP_LEVEL; i++)
	{
		pTableDlg[i] = NULL;

		pVipGrid[i] = NULL;

		pVipMall[i] = NULL;

		pVipGold[i] = NULL;

		pVipGetButton[i] = NULL;
	}
	m_TotalGold = 0;

	m_VipLevel = 0;
	memset(m_VipValue, 0, sizeof(m_VipValue));
}

VipDialog::~VipDialog()
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

	pDlg = NULL;

	pVipImage = NULL;

	pVipMsg = NULL;

	pVipProgress = NULL;

	pVipUpRate = NULL;

	pVipMsg2 = NULL;

	for (int i = 0; i < MAX_VIP_LEVEL; i++)
	{
		pTableDlg[i] = NULL;

		pVipGrid[i] = NULL;

		pVipMall[i] = NULL;

		pVipGold[i] = NULL;

		pVipGetButton[i] = NULL;
	}
	m_TotalGold = 0;

	m_VipLevel = 0;
	memset(m_VipValue, 0, sizeof(m_VipValue));
}

void VipDialog::Add(cWindow* window)
{
	if (window->GetType() == WT_PUSHUPBUTTON)
		AddTabBtn(curIdx1++, (cPushupButton*)window);
	else if (window->GetType() == WT_DIALOG)
		AddTabSheet(curIdx2++, window);
	else
		cTabDialog::Add(window);
}

void VipDialog::Linking()
{
	pDlg = WINDOWMGR->GetWindowForID(VIP_DLG);

	pVipImage = ((cStatic*)pDlg->GetWindowForID(VIP_LEVEL_IMAGE));

	pVipMsg = ((cStatic*)pDlg->GetWindowForID(VIP_LEVEL_MSG));

	pVipProgress = ((CObjectGuagen*)pDlg->GetWindowForID(VIP_LEVEL_GUAGE));

	pVipProgress->SetValue(0, 0);

	pVipUpRate = ((cStatic*)pDlg->GetWindowForID(VIP_LEVEL_UPRATE));

	pVipMsg2 = ((cStatic*)pDlg->GetWindowForID(VIP_LEVEL_MSG2));

	for (int i = 0; i < MAX_VIP_LEVEL; i++)
	{
		pTableDlg[i] = (cDialog*)GetTabSheet(i);

		if (pTableDlg[i])
		{
			pVipGrid[i] = (cIconGridDialog*)pTableDlg[i]->GetWindowForID(VIP_GRID_ITEM1 + i);
			pVipMall[i] = (cStatic*)pTableDlg[i]->GetWindowForID(VIP_MALL1 + i);
			pVipGold[i] = (cStatic*)pTableDlg[i]->GetWindowForID(VIP_GOLD1 + i);
			pVipGetButton[i] = ((cButton*)pTableDlg[i]->GetWindowForID(VIP_GETBTN1 + i));
		}
	}
}

BOOL  VipDialog::LoadVipInfo()
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

void VipDialog::SetVipItem()
{
	cVipDealItem* pItem = NULL;

	cImage tmpImage;
	char   line[128];
	int    Index = 0;
	VIP_INFO* m_VipInfo = NULL;
	for (int i = 1; i <= MAX_VIP_LEVEL; i++)  //vip �����Ŵ�1��ʼ
	{
		m_VipInfo = m_VipInfoList.GetData(i);
		if (m_VipInfo)
		{
			for (int m = 0; m < m_VipInfo->VipItemCount; m++)
			{
				if (m >= 10)  //VIP������Ʒ�������5��
				{
					break;
				}
				ITEM_INFO* pInfo = ITEMMGR->GetItemInfo(m_VipInfo->VipItemIdx[m]);

				if (pInfo && pVipGrid[i - 1])
				{
					pItem = new cVipDealItem;
					pItem->Init(0, 0, DEFAULT_ICONSIZE, DEFAULT_ICONSIZE, ITEMMGR->GetIconImage(pInfo->ItemIdx, &tmpImage), IG_VIPITEM_STARTINDEX + Index);
					pItem->SetData(pInfo->ItemIdx);
					pItem->SetItemIdx(pInfo->ItemIdx);
					pItem->SetMovable(FALSE);
					ITEMMGR->SetGoldShopToolTipIcon(pItem);
					pVipGrid[i - 1]->AddIcon(m, (cIcon*)pItem);
					Index = Index + 1;
				}
			}
			if (pVipMall[i - 1])
			{
				pVipMall[i - 1]->SetStaticValue(m_VipInfo->VipMallMoney);
			}
			if (pVipGold[i - 1])
			{
				pVipGold[i - 1]->SetStaticValue(m_VipInfo->VipGoldMoney);
			}
		}
	}
}
void VipDialog::SetVipInfo(DWORD TotalGold, WORD VipLevel, int* VipValue)
{
	m_TotalGold = TotalGold;

	m_VipLevel = VipLevel;

	memcpy(&m_VipValue, VipValue, sizeof(int) * MAX_VIP_LEVEL);

	VIP_INFO* m_VipInfo = m_VipInfoList.GetData(m_VipLevel);
	//CHATMGR->AddMsg(CTC_SYSMSG, "[VIP] SetVipInfo called: TotalGold=%u, Level=%hu", TotalGold, VipLevel);

	if (m_VipInfo)
	{
		SelectTab(m_VipLevel);
		if (pVipImage)
		{
			cImage VipImage;

			SCRIPTMGR->GetImage(m_VipInfo->VipImgIdx, &VipImage, PFT_HARDPATH);

			if (!VipImage.IsNull())
			{
				if (m_VipLevel > 0 && m_VipLevel <= MAX_VIP_LEVEL)  //�޸�û��VIP������߳�����󼶱�����ͼ��
				{
					pVipImage->SetBasicImage(&VipImage);
				}
			}
		}
		DWORD  LevelOverGold = m_TotalGold - m_VipInfo->VipMaxGold;
		DWORD  LevelNeedGold = GetPreviousMaxGold(m_VipLevel) - m_TotalGold;
		DWORD  LevelFullGold = (GetPreviousMaxGold(m_VipLevel)) - m_VipInfo->VipMaxGold;
		float        ProcessOverRate = (float)LevelOverGold / (float)LevelFullGold;
		int          ProcessPercent = (int)(ProcessOverRate * 100);
		if (pVipMsg)
		{
			char vipmsg[128];
			if (m_VipLevel >= 10)
			{
				sprintf(vipmsg, "��ǰ��ԱΪ%d��,������,�ۼƳ�ֵ%dԪ��", m_VipLevel, m_TotalGold);
			}
			else
				sprintf(vipmsg, "��ǰ��ԱΪ%d��,�ۼƳ�ֵ%dԪ��,�������ֵ%dԪ��", m_VipLevel, m_TotalGold, LevelNeedGold);
			pVipMsg->SetStaticText(vipmsg);
		}
		if (pVipProgress)
		{
			pVipProgress->SetValue(ProcessOverRate, 1);
			if (pVipUpRate)
			{
				char rateText[64];
				sprintf(rateText, "%d%%", ProcessPercent);
				pVipUpRate->SetStaticText(rateText);
			}
		}
		if (pVipMsg2)
		{
			if (m_VipLevel >= 10)
			{
				pVipMsg2->SetActive(FALSE);
			}
			else
			{
				char vipmsg2[128];
				sprintf(vipmsg2, "��ֵ�ﵽ��Ӧ��Ա����,�ɵ����ȡ��Ʒ���ݵ㼰Ԫ������!");
				pVipMsg2->SetStaticText(vipmsg2);
			}
		}
	}
	else
	{
		if (pVipMsg)
		{
			char vipmsg[128];
			sprintf(vipmsg, "�Բ���,������δ��ֵԪ��,û�л�Ա�׼�");
			pVipMsg->SetStaticText(vipmsg);
		}
		if (pVipProgress)
		{
			pVipProgress->SetValue(0, 1);
			if (pVipUpRate)
			{
				char rateText[64];
				sprintf(rateText, "%d%%", 0);
				pVipUpRate->SetStaticText(rateText);
			}
		}
		if (pVipMsg2)
		{
			char vipmsg2[128];
			sprintf(vipmsg2, "��ֵ�ﵽ��Ӧ��Ա����,�ɵ����ȡ��Ʒ���ݵ㼰Ԫ������!");
			pVipMsg2->SetStaticText(vipmsg2);
		}
	}
	SetButtonStatic(m_VipValue);
}

DWORD VipDialog::GetPreviousMaxGold(WORD VipLevel)
{
	VIP_INFO* m_VipInfo = m_VipInfoList.GetData(m_VipLevel + 1);

	if (m_VipInfo)
	{
		return m_VipInfo->VipMaxGold;
	}
	else
	{
		return 0;
	}
}

void VipDialog::SetButtonStatic(int* VipValue)
{
	if (m_VipLevel == 0)  //VIP����Ϊ0
	{
		char btntext[64];
		sprintf(btntext, "δ�ﵽ");
		for (int i = 0; i < MAX_VIP_LEVEL; i++)
		{
			pVipGetButton[i]->SetDisable(TRUE);
			pVipGetButton[i]->SetText(btntext, RGB_HALF(225, 225, 225));
		}
	}
	else
	{
		for (int i = 0; i < MAX_VIP_LEVEL; i++)
		{
			if (m_VipValue[i] == 0)
			{
				char btntext[64];
				sprintf(btntext, "δ���");
				pVipGetButton[i]->SetDisable(TRUE);
				pVipGetButton[i]->SetText(btntext, RGB_HALF(255, 0, 0)); //[δ��� ��ɫ]
			}
			if (m_VipValue[i] == 1)
			{
				char btntext[64];
				sprintf(btntext, "��ȡ");
				pVipGetButton[i]->SetDisable(FALSE);
				pVipGetButton[i]->SetText(btntext, RGB_HALF(50, 205, 50));  //[��ȡ��ɫ]
			}
			if (m_VipValue[i] == 2)
			{
				char btntext[64];
				sprintf(btntext, "����ȡ");
				pVipGetButton[i]->SetDisable(TRUE);
				pVipGetButton[i]->SetText(btntext, RGB_HALF(255, 215, 0));  //[����ȡ��ɫ] 
			}
		}
	}
}
BOOL VipDialog::OnActionEvent(LONG lId, void* p, DWORD we)
{
	if (we & WE_BTNCLICK)
	{
		if (lId >= VIP_GETBTN1 && lId <= VIP_GETBTN1 + MAX_VIP_LEVEL)  //�ж��������˻�ȡ������ť
		{
			LONG  BtnOffsetId = lId - VIP_GETBTN1 + 1;

			if (BtnOffsetId > m_VipLevel)
			{
				CHATMGR->AddMsg(CTC_SYSMSG, "��Ա����δ�ﵽ��������,�޷���ȡ");

				return FALSE;
			}
			if (GAMEIN->GetInventoryDialog()->GetBlankNum() == 0)
			{
				// MsgBox ó�� (�κ��� �����ִ�.)354
				CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(354));
				return FALSE;
			}
			int Flag = m_VipValue[BtnOffsetId - 1];
			if (Flag == 1)
			{
				MSG_DWORD msg;
				msg.Category = MP_CHAR;
				msg.Protocol = MP_CHAR_VIP_GET_SYN;
				msg.dwObjectID = HERO->GetID();
				msg.dwData = BtnOffsetId;//viplevel ��ȡ��ǰ������
				NETWORK->Send(&msg, sizeof(msg));
			}
			else
			{
				CHATMGR->AddMsg(CTC_SYSMSG, "��Ա��������ȡ,�޷��ظ���ȡ�ý���");

				return FALSE;
			}
		}
	}
	return TRUE;
}

void VipDialog::SetVipInfoByLevel(WORD VipLevel, int Flag)
{
	m_VipValue[VipLevel - 1] = Flag;
	SetButtonStatic(m_VipValue);
}
