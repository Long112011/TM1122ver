#include "stdafx.h"
#include ".\rarecreatedialog.h"
#include "WindowIDEnum.h"
#include "./Interface/cIcon.h"
#include "./Interface/cIconDialog.h"
#include "Item.h"
#include "ChatManager.h"
#include "ItemManager.h"
#include "./Interface/cScriptManager.h"
#include "ObjectStateManager.h"
#include "ObjectManager.h"
#include "GameIn.h"
#include "InventoryExDialog.h"
#include "MHFile.h"
#include "cWindowManager.h"

CRareCreateDialog::CRareCreateDialog(void)
{
	m_type = WT_ITEM_RARECREATE_DLG;

	cImage imgToolTip;
	SCRIPTMGR->GetImage(63, &imgToolTip, PFT_HARDPATH);
	m_VirtualItem.SetToolTip("", RGB_HALF(255, 255, 255), &imgToolTip);
	m_VirtualItem.SetMovable(FALSE);

	m_ItemIdx = 0;
	m_ItemPos = 0;
	m_pIconDlg = NULL;
	Rare_Info = NULL;

	LoadRareItemOptionInfo();
}

CRareCreateDialog::~CRareCreateDialog(void)
{
	Rare_Info->SetStaticText(" ");
}

void CRareCreateDialog::Linking()
{
	m_pIconDlg = (cIconDialog*)GetWindowForID(RareCreate_ICON);
	Rare_Info = (cStatic*)GetWindowForID(ITMD_RareCreateItemInfo);
	Rare_Info->SetMultiLine(TRUE);
	Rare_Info->InitMultiLine();
}

BOOL CRareCreateDialog::FakeMoveIcon(LONG x, LONG y, cIcon* pOrigIcon)
{
	Rare_Info->SetStaticText(" ");
	if (pOrigIcon->GetType() != WT_ITEM) return FALSE;
	if (m_bDisable) return FALSE;

	CItem* pOrigItem = (CItem*)pOrigIcon;

	if (pOrigItem->IsLocked()) return FALSE;

	ITEM_INFO* pBaseItemInfo = ITEMMGR->GetItemInfo(pOrigItem->GetItemIdx());
	if (!pBaseItemInfo)
		return FALSE;

	// 厘厚酒捞袍 眉农
	eITEM_KINDBIT bits = pOrigItem->GetItemKind();
	if (!(bits & eEQUIP_ITEM))
		return FALSE;

	// 06.09.25 RaMa - 捞亥飘酒捞袍阑 碍拳, 饭绢甫 给 父甸档废
	WORD wType = pBaseItemInfo->WeaponType;

	if (!(
		(wType >= WP_GUM && wType <= WP_KEY) ||
		wType == WP_AXE ||
		wType == WP_DAGGER
		))
	{
		CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(1455));
		return FALSE;
	}


	//HK080529	//itemidex 啊 捞固 炼钦包访 %10狼 逢阑 玻促.
	// 炼钦等扒瘤 眉农
	if (bits <= eEQUIP_ITEM_SHOES && pOrigItem->GetItemIdx() % 10)
	{
		CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(1234));
		return FALSE;
	}


	// 碍拳 酒捞袍篮 给 棵扼埃促.
	ITEM_OPTION_INFO* pOptionInfo = ITEMMGR->GetItemOption(pOrigItem->GetDurability());
	if (pOrigItem->GetDurability() != 0 || pOptionInfo->dwOptionIdx != 0)
	{
		CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(1234));
		return FALSE;
	}
	// 饭绢啊 啊瓷茄瘤 魄喊
	if (!ITEMMGR->IsRareItemAble(pOrigItem->GetItemIdx()))
	{
		CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(1234));
		return FALSE;
	}

	// 牢亥配府俊辑 坷绰瘤 眉农
	ITEM_RARE_OPTION_INFO* RareInfo = ITEMMGR->GetItemRareOption(pOrigItem->GetRareness());
	if (RareInfo)
	{
		SetRareInfo(RareInfo, pOrigItem->GetItemIdx());
	}
	if (!ITEMMGR->IsEqualTableIdxForPos(eItemTable_Inventory, pOrigItem->GetPosition()))
		return FALSE;

	ITEM_INFO* pShopItem = ITEMMGR->GetItemInfo((WORD)m_ItemIdx);
	if (!pShopItem)
		return FALSE;

	if (pBaseItemInfo->LimitLevel < pShopItem->GenGol || pBaseItemInfo->LimitLevel > pShopItem->MinChub)
	{
		Rare_Info->SetStaticText(" ");
		CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(1082));
		return FALSE;
	}
	if (m_VirtualItem.GetSrcItemKind() != eKIND_ITEM_MAX)
	{
		CItem* pItem = ITEMMGR->GetItem(m_VirtualItem.GetLinkItem()->GetDBIdx());
		if (pItem)
		{
			if (m_pIconDlg)
			{
				CVirtualItem* pVItem = NULL;
				m_pIconDlg->DeleteIcon(0, (cIcon**)&pVItem);
				if (pVItem)
					pVItem->GetLinkItem()->SetLock(FALSE);
			}
		}
	}

	pOrigItem->SetLock(TRUE);
	AddVirtualItem(pOrigItem);

	//ITEMMGR->SetDisableDialog(TRUE, eItemTable_Inventory);
	//ITEMMGR->SetDisableDialog(TRUE, eItemTable_Pyoguk);
	//ITEMMGR->SetDisableDialog(TRUE, eItemTable_GuildWarehouse);
	//ITEMMGR->SetDisableDialog(TRUE, eItemTable_Shop);


	return FALSE;
}

void CRareCreateDialog::ReleaseItem()
{
	if (m_pIconDlg)
	{
		CVirtualItem* pVItem = NULL;

		m_pIconDlg->DeleteIcon(0, (cIcon**)&pVItem);
		if (pVItem)
			pVItem->GetLinkItem()->SetLock(FALSE);

		m_VirtualItem.SetLinkItem(NULL);
	}

	CItem* pOriItem = ITEMMGR->GetItemofTable(eItemTable_ShopInven, (POSTYPE)m_ItemPos);
	if (pOriItem)
		pOriItem->SetLock(FALSE);

	m_ItemIdx = 0;
	m_ItemPos = 0;


	ITEMMGR->SetDisableDialog(FALSE, eItemTable_Inventory);
	ITEMMGR->SetDisableDialog(FALSE, eItemTable_Pyoguk);
	ITEMMGR->SetDisableDialog(FALSE, eItemTable_MunpaWarehouse);
	ITEMMGR->SetDisableDialog(FALSE, eItemTable_Shop);

	OBJECTSTATEMGR->EndObjectState(HERO, eObjectState_Deal);

	Rare_Info->SetStaticText(" ");
	SetActive(FALSE);
}

void CRareCreateDialog::AddVirtualItem(CItem* pItem)
{
	m_VirtualItem.SetData(pItem->GetItemIdx());
	m_VirtualItem.SetLinkItem(pItem);
	if (pItem->GetDurability() != 0 && !ITEMMGR->IsDupItem(pItem->GetItemIdx()))//添加绿化时候显示物品的渲染文字
	{
		ITEMMGR->SetToolTipIcon((cIcon*)&m_VirtualItem, ITEMMGR->GetItemOption(pItem->GetDurability())
			, ITEMMGR->GetItemRareOption(pItem->GetRareness()));
	}
	else
		ITEMMGR->SetToolTipIcon((cIcon*)&m_VirtualItem, NULL, ITEMMGR->GetItemRareOption(pItem->GetRareness()));

	SHOPITEMBASE* pShopItemInfo = NULL;
	pShopItemInfo = ITEMMGR->GetUsedItemInfo(pItem->GetItemBaseInfo()->wIconIdx);
	if (pShopItemInfo)
		ITEMMGR->AddUsedAvatarItemToolTip(pShopItemInfo);
	m_pIconDlg->AddIcon(0, (cIcon*)&m_VirtualItem);
}

void CRareCreateDialog::ItemRareCreateSyn()
{
	if (!m_VirtualItem.GetLinkItem())		return;

	CItem* pItem = ITEMMGR->GetItem(m_VirtualItem.GetLinkItem()->GetDBIdx());

	if (!pItem)
	{
		if (m_pIconDlg)
		{
			CVirtualItem* pVItem = NULL;
			m_pIconDlg->DeleteIcon(0, (cIcon**)&pVItem);
			if (pVItem)
				pVItem->GetLinkItem()->SetLock(FALSE);
		}

		m_ItemIdx = 0;
		m_ItemPos = 0;

		ITEMMGR->SetDisableDialog(FALSE, eItemTable_Inventory);
		ITEMMGR->SetDisableDialog(FALSE, eItemTable_Pyoguk);
		ITEMMGR->SetDisableDialog(FALSE, eItemTable_MunpaWarehouse);
		ITEMMGR->SetDisableDialog(FALSE, eItemTable_Shop);

		OBJECTSTATEMGR->EndObjectState(HERO, eObjectState_Deal);
		return;
	}

	MSG_DWORD4	msg;
	SetProtocol(&msg, MP_ITEM, MP_ITEM_SHOPITEM_RARECREATE_SYN);
	msg.dwObjectID = HEROID;
	msg.dwData1 = m_ItemIdx;
	msg.dwData2 = m_ItemPos;
	msg.dwData3 = pItem->GetItemBaseInfo()->wIconIdx;
	msg.dwData4 = pItem->GetItemBaseInfo()->Position;
	NETWORK->Send(&msg, sizeof(msg));

	//SetActive( FALSE );
}
#include "cMsgBox.h"

void CRareCreateDialog::ItemRareCreateAck()//祝福成功之后，删除道具栏的物品
{
	CVirtualItem* pVItem = NULL;

	CItem* pItemEx = GAMEIN->GetInventoryDialog()->GetItemLike(m_ItemIdx);

	if (pItemEx)
	{
		m_ItemIdx = m_ItemIdx;
		m_ItemPos = pItemEx->GetPosition();
		CItem* pItem = ITEMMGR->GetItem(m_VirtualItem.GetLinkItem()->GetDBIdx());
		if (pItem)
		{
			ITEMMGR->SetToolTipIcon((cIcon*)pItem, NULL, ITEMMGR->GetItemRareOption(pItem->GetRareness()));

			if (pItem->GetDurability() != 0 && !ITEMMGR->IsDupItem(pItem->GetItemIdx()))//添加绿化时候显示物品的渲染文字
				ITEMMGR->SetToolTipIcon((cIcon*)&m_VirtualItem, ITEMMGR->GetItemOption(pItem->GetDurability()), ITEMMGR->GetItemRareOption(pItem->GetRareness()), 0, ITEMMGR->GetItemStoneOption(pItem->GetStoneIdx()));
			else
				ITEMMGR->SetToolTipIcon((cIcon*)&m_VirtualItem, NULL, ITEMMGR->GetItemRareOption(pItem->GetRareness()), 0, ITEMMGR->GetItemStoneOption(pItem->GetStoneIdx()));
			ITEM_RARE_OPTION_INFO* pRareItemInfo = ITEMMGR->GetItemRareOption(pItem->GetRareness());
			if (pRareItemInfo)
			{
				SetRareInfo(pRareItemInfo, pItem->GetItemIdx());
			}
			WORD vale = 0;
			if (pItem->GetQuality() == 4)//神话
				vale = 3;
			else if (pItem->GetQuality() == 3)
				vale = 2;
			else if (pItem->GetQuality() == 2)
				vale = 1.5;
			else if (pItem->GetQuality() == 1)
				vale = 1.2;
			else if (pItem->GetQuality() == 0)
				vale = 1.0;

			ITEM_RARE_OPTION_INFO* m_ItemRareOptionMaxInfo;//物品祝福最大值
			m_ItemRareOptionMaxInfo = GetRareItemMaxValue(pItem->GetItemIdx());

			if (pItem->GetItemKind() == eEQUIP_ITEM_WEAPON)
			{
				if (pRareItemInfo->PhyAttack >= m_ItemRareOptionMaxInfo->PhyAttack * vale ||
					pRareItemInfo->Life >= m_ItemRareOptionMaxInfo->Life * vale ||
					pRareItemInfo->Shield >= m_ItemRareOptionMaxInfo->Shield * vale ||
					pRareItemInfo->NaeRyuk >= m_ItemRareOptionMaxInfo->NaeRyuk * vale)
				{
					this->SetDisable(TRUE);
					cMsgBox* pMsgBox = WINDOWMGR->MsgBox(MBI_RAREMAXOK, MBT_OK, "装备至少有一条属性达到满值，点击确认后可继续祝福");
					if (pMsgBox)
					{
						pMsgBox->SetAbsXY(this->GetAbsX(), this->GetAbsY() + this->GetHeight() / 2);
					}
				}
			}
			if (pItem->GetItemKind() == eEQUIP_ITEM_DRESS || pItem->GetItemKind() == eEQUIP_ITEM_HAT || pItem->GetItemKind() == eEQUIP_ITEM_SHOES)
			{
				if (pRareItemInfo->Life >= m_ItemRareOptionMaxInfo->Life * vale ||
					pRareItemInfo->Shield >= m_ItemRareOptionMaxInfo->Shield * vale ||
					pRareItemInfo->NaeRyuk >= m_ItemRareOptionMaxInfo->NaeRyuk * vale ||
					pRareItemInfo->PhyDefense >= m_ItemRareOptionMaxInfo->PhyDefense * vale)
				{
					this->SetDisable(TRUE);
					cMsgBox* pMsgBox = WINDOWMGR->MsgBox(MBI_RAREMAXOK, MBT_OK, "装备至少有一条属性达到满值，点击确认后可继续祝福");
					if (pMsgBox)
					{
						pMsgBox->SetAbsXY(this->GetAbsX(), this->GetAbsY() + this->GetHeight() / 2);
					}
				}
				for (int i = ATTR_FIRE; i <= ATTR_MAX; ++i)
				{
					if (100 * pRareItemInfo->AttrAttack.GetElement_Val(i) >= m_ItemRareOptionMaxInfo->AttrAttack.GetElement_Val(i) * vale ||
						100 * pRareItemInfo->AttrRegist.GetElement_Val(i) >= m_ItemRareOptionMaxInfo->AttrRegist.GetElement_Val(i) * vale)
					{
						this->SetDisable(TRUE);
						cMsgBox* pMsgBox = WINDOWMGR->MsgBox(MBI_RAREMAXOK, MBT_OK, "装备至少有一条属性达到满值，点击确认后可继续祝福");
						if (pMsgBox)
						{
							pMsgBox->SetAbsXY(this->GetAbsX(), this->GetAbsY() + this->GetHeight() / 2);
						}
					}
				}
			}
			if (pItem->GetItemKind() == eEQUIP_ITEM_RING || pItem->GetItemKind() == eEQUIP_ITEM_ARMLET ||
				pItem->GetItemKind() == eEQUIP_ITEM_CAPE || pItem->GetItemKind() == eEQUIP_ITEM_BELT ||
				pItem->GetItemKind() == eEQUIP_ITEM_NECKLACE)
			{
				if (pRareItemInfo->GenGol >= m_ItemRareOptionMaxInfo->GenGol * vale ||
					pRareItemInfo->MinChub >= m_ItemRareOptionMaxInfo->MinChub * vale ||
					pRareItemInfo->CheRyuk >= m_ItemRareOptionMaxInfo->CheRyuk * vale ||
					pRareItemInfo->SimMek >= m_ItemRareOptionMaxInfo->SimMek * vale)
				{
					this->SetDisable(TRUE);
					cMsgBox* pMsgBox = WINDOWMGR->MsgBox(MBI_RAREMAXOK, MBT_OK, "装备至少有一条属性达到满值，点击确认后可继续祝福");
					if (pMsgBox)
					{
						pMsgBox->SetAbsXY(this->GetAbsX(), this->GetAbsY() + this->GetHeight() / 2);
					}
				}
			}
		}
	}
	else
	{
		m_ItemIdx = 0;
		m_ItemPos = 0;

		CItem* pItem = ITEMMGR->GetItem(m_VirtualItem.GetLinkItem()->GetDBIdx());
		if (pItem)
		{
			pItem->SetLock(FALSE);

			if (m_pIconDlg)
				m_pIconDlg->DeleteIcon(0, (cIcon**)&pVItem);

			ITEMMGR->SetToolTipIcon((cIcon*)pItem, NULL, ITEMMGR->GetItemRareOption(pItem->GetRareness()));

			m_VirtualItem.SetLinkItem(NULL);
		}
	}

	if (!GAMEIN->GetRareCreateDlg()->IsActive())
	{
		ITEMMGR->SetDisableDialog(FALSE, eItemTable_Inventory);
		ITEMMGR->SetDisableDialog(FALSE, eItemTable_Pyoguk);
		ITEMMGR->SetDisableDialog(FALSE, eItemTable_MunpaWarehouse);
		ITEMMGR->SetDisableDialog(FALSE, eItemTable_Shop);

		OBJECTSTATEMGR->EndObjectState(HERO, eObjectState_Deal);
	}
}
void CRareCreateDialog::ItemRareCreateNAck()
{
	CVirtualItem* pVItem = NULL;

	/*m_ItemIdx = 0;
	m_ItemPos = 0;*/
	CItem* pItem = ITEMMGR->GetItem(m_VirtualItem.GetLinkItem()->GetDBIdx());
	if (pItem)
	{
		pItem->SetLock(FALSE);

		if (m_pIconDlg)
			m_pIconDlg->DeleteIcon(0, (cIcon**)&pVItem);

		m_VirtualItem.SetLinkItem(NULL);
	}

	ITEMMGR->SetDisableDialog(FALSE, eItemTable_Inventory);
	ITEMMGR->SetDisableDialog(FALSE, eItemTable_Pyoguk);
	ITEMMGR->SetDisableDialog(FALSE, eItemTable_MunpaWarehouse);
	ITEMMGR->SetDisableDialog(FALSE, eItemTable_Shop);

	OBJECTSTATEMGR->EndObjectState(HERO, eObjectState_Deal);
}

void CRareCreateDialog::SetRareInfo(ITEM_RARE_OPTION_INFO* pRareOptionInfo, DWORD ItemIdx)
{
	ITEM_RARE_OPTION_INFO* m_ItemRareOptionMaxInfo;//物品祝福最大值
	ITEM_INFO* pInfo = ITEMMGR->GetItemInfo(ItemIdx);
	float attrRareOptValue = 0;
	float attrRareOptMaxValue = 0;
	m_ItemRareOptionMaxInfo = GetRareItemMaxValue(ItemIdx);
	float RareRate = 1.0f;

	char line[128] = { 0, };
	char szDescText[384] = { 0, };
	sprintf(line, "[%s] ^n^n", pInfo->ItemName);
	strcat(szDescText, line);

	DWORD RareState;

	if (m_ItemRareOptionMaxInfo->GenGol > 0)
	{
		if (pRareOptionInfo->GenGol != 0)
			RareState = pRareOptionInfo->GenGol;
		else
			RareState = 0;

		if (RareState)
		{
			wsprintf(line, "%s  	+%d  /  %d ^n", CHATMGR->GetChatMsg(382), RareState, m_ItemRareOptionMaxInfo->GenGol);
			strcat(szDescText, line);
		}
	}
	if (m_ItemRareOptionMaxInfo->MinChub > 0)
	{
		if (pRareOptionInfo->MinChub != 0)
			RareState = pRareOptionInfo->MinChub;
		else
			RareState = 0;

		if (RareState)
		{
			wsprintf(line, "%s  	+%d  /  %d ^n", CHATMGR->GetChatMsg(383), RareState, m_ItemRareOptionMaxInfo->MinChub);
			strcat(szDescText, line);
		}
	}
	if (m_ItemRareOptionMaxInfo->CheRyuk > 0)
	{
		if (pRareOptionInfo->CheRyuk != 0)
			RareState = pRareOptionInfo->CheRyuk;
		else
			RareState = 0;

		if (RareState)
		{
			wsprintf(line, "%s  	+%d  /  %d ^n", CHATMGR->GetChatMsg(384), RareState, m_ItemRareOptionMaxInfo->CheRyuk);
			strcat(szDescText, line);
		}
	}
	if (m_ItemRareOptionMaxInfo->SimMek > 0)
	{
		if (pRareOptionInfo->SimMek != 0)
			RareState = pRareOptionInfo->SimMek;
		else
			RareState = 0;

		if (RareState)
		{
			wsprintf(line, "%s  	+%d  /  %d ^n", CHATMGR->GetChatMsg(385), RareState, m_ItemRareOptionMaxInfo->SimMek);
			strcat(szDescText, line);
		}
	}
	if (m_ItemRareOptionMaxInfo->Life > 0)
	{
		if (pRareOptionInfo->Life != 0)
			RareState = pRareOptionInfo->Life;
		else
			RareState = 0;

		if (RareState)
		{
			wsprintf(line, "%s  	+%d  /  %d ^n", CHATMGR->GetChatMsg(386), RareState, m_ItemRareOptionMaxInfo->Life);
			strcat(szDescText, line);
		}
	}
	if (m_ItemRareOptionMaxInfo->NaeRyuk > 0)
	{
		if (pRareOptionInfo->NaeRyuk != 0)
			RareState = pRareOptionInfo->NaeRyuk;
		else
			RareState = 0;

		if (RareState)
		{
			wsprintf(line, "%s  	+%d  /  %d ^n", CHATMGR->GetChatMsg(387), RareState, m_ItemRareOptionMaxInfo->NaeRyuk);
			strcat(szDescText, line);
		}
	}
	if (m_ItemRareOptionMaxInfo->Shield > 0)
	{
		if (pRareOptionInfo->Shield != 0)
			RareState = pRareOptionInfo->Shield;
		else
			RareState = 0;

		if (RareState)
		{
			wsprintf(line, "%s  	+%d  /  %d ^n", CHATMGR->GetChatMsg(388), RareState, m_ItemRareOptionMaxInfo->Shield);
			strcat(szDescText, line);
		}
	}
	for (int i = ATTR_FIRE; i <= ATTR_MAX; ++i)
	{
		if (pRareOptionInfo)
		{
			attrRareOptValue = 100 * pRareOptionInfo->AttrRegist.GetElement_Val(i);
		}
		else
			attrRareOptValue = 0;

		attrRareOptMaxValue = m_ItemRareOptionMaxInfo->AttrRegist.GetElement_Val(i) * RareRate;

		if (attrRareOptValue)
		{
			wsprintf(line, "%s  	+%d%%  /  %d%% ^n", CHATMGR->GetChatMsg(265 + i - 1), (int)(attrRareOptValue), (int)(attrRareOptMaxValue));
			strcat(szDescText, line);
		}

	}
	if (m_ItemRareOptionMaxInfo->PhyAttack > 0)
	{
		if (pInfo->MeleeAttackMin || pInfo->MeleeAttackMax)
		{
			if (pRareOptionInfo)
				RareState = pRareOptionInfo->PhyAttack;
			else
				RareState = 0;

			if (RareState)
			{
				wsprintf(line, "%s  	+%d  /  %d ^n", CHATMGR->GetChatMsg(389), RareState, m_ItemRareOptionMaxInfo->PhyAttack);
				strcat(szDescText, line);
			}
		}
		if (pInfo->RangeAttackMin || pInfo->RangeAttackMax)
		{
			if (pRareOptionInfo)
				RareState = pRareOptionInfo->PhyAttack;
			else
				RareState = 0;

			if (RareState)
			{
				wsprintf(line, "%s  	+%d  /  %d ^n", CHATMGR->GetChatMsg(391), RareState, m_ItemRareOptionMaxInfo->PhyAttack);
				strcat(szDescText, line);
			}
		}
	}
	for (int j = ATTR_FIRE; j <= ATTR_MAX; ++j)
	{

		if (pRareOptionInfo)
		{
			attrRareOptValue = 100 * pRareOptionInfo->AttrAttack.GetElement_Val(j);
		}
		else
			attrRareOptValue = 0;
		attrRareOptMaxValue = m_ItemRareOptionMaxInfo->AttrAttack.GetElement_Val(j) * RareRate;

		if (attrRareOptValue)
		{
			wsprintf(line, "%s	+%d%%  /  %d%% ^n", CHATMGR->GetChatMsg(392 + j - 1), (int)(attrRareOptValue), (int)(attrRareOptMaxValue));
			strcat(szDescText, line);
		}
	}
	if (m_ItemRareOptionMaxInfo->PhyDefense > 0)
	{
		if (pRareOptionInfo)
			RareState = pRareOptionInfo->PhyDefense;
		else
			RareState = 0;

		if (RareState)
		{
			wsprintf(line, "%s	+%d  /  %d ^n", CHATMGR->GetChatMsg(397), RareState, m_ItemRareOptionMaxInfo->PhyDefense);
			strcat(szDescText, line);
		}
	}
	Rare_Info->SetStaticText(szDescText);
}

void CRareCreateDialog::Clear()
{
	Rare_Info->SetStaticText(" ");
}

ITEM_RARE_OPTION_INFO* CRareCreateDialog::GetRareItemMaxValue(DWORD ItemIdx)//获取物品祝福最大值
{
	ITEM_INFO* pInfo = ITEMMGR->GetItemInfo(ItemIdx);

	if (!pInfo)  return NULL;

	eITEM_KINDBIT eItemKind = GetItemKind(ItemIdx);

	if (!(eItemKind & eEQUIP_ITEM)) return NULL;

	DWORD RareItemKind = 0;

	if (eItemKind == eEQUIP_ITEM_WEAPON)
	{
		RareItemKind = pInfo->WeaponType - 1;
	}
	else if (eItemKind == eEQUIP_ITEM_DRESS)			RareItemKind = eRI_DRESS;
	else if (eItemKind == eEQUIP_ITEM_HAT)				RareItemKind = eRI_HAT;
	else if (eItemKind == eEQUIP_ITEM_SHOES)			RareItemKind = eRI_SHOES;
	else if (eItemKind == eEQUIP_ITEM_RING)			RareItemKind = eRI_RING;
	else if (eItemKind == eEQUIP_ITEM_CAPE)			RareItemKind = eRI_CAPE;
	else if (eItemKind == eEQUIP_ITEM_NECKLACE)		RareItemKind = eRI_NECKLACE;
	else if (eItemKind == eEQUIP_ITEM_ARMLET)			RareItemKind = eRI_ARMLET;
	else if (eItemKind == eEQUIP_ITEM_BELT)			RareItemKind = eRI_BELT;

	if (RareItemKind >= RareItemKindMAX) return NULL;

	return (ITEM_RARE_OPTION_INFO*)&m_RereItemInfo[RareItemKind];
}

bool CRareCreateDialog::LoadRareItemOptionInfo()//客户端加载祝福信息
{
	CMHFile fp;

	char szBuf[256] = { 0, };
	char line[512];

	char FileName[256];

	sprintf(FileName, "Resource/Item_RareOptionInfo.bin");
	if (!fp.Init(FileName, "rb"))
		return FALSE;

	DWORD dwItemKindIdx = 0;
	while (!fp.IsEOF())
	{
		if (dwItemKindIdx == RareItemKindMAX)
			break;

		fp.GetString(szBuf);

		if (szBuf[0] == '@')
		{
			fp.GetLineX(line, 512);
			continue;
		}

		if (szBuf[0] == '#')
		{
			m_RereItemInfo[dwItemKindIdx].wRareRate = fp.GetWord();//新增几率控制

			m_RereItemInfo[dwItemKindIdx].GenGol_Min = fp.GetWord();
			m_RereItemInfo[dwItemKindIdx].GenGol = fp.GetWord();

			m_RereItemInfo[dwItemKindIdx].MinChub_Min = fp.GetWord();
			m_RereItemInfo[dwItemKindIdx].MinChub = fp.GetWord();
			m_RereItemInfo[dwItemKindIdx].CheRyuk_Min = fp.GetWord();
			m_RereItemInfo[dwItemKindIdx].CheRyuk = fp.GetWord();
			m_RereItemInfo[dwItemKindIdx].SimMek_Min = fp.GetWord();
			m_RereItemInfo[dwItemKindIdx].SimMek = fp.GetWord();
			m_RereItemInfo[dwItemKindIdx].Life_Min = fp.GetDword();
			m_RereItemInfo[dwItemKindIdx].Life = fp.GetDword();
			m_RereItemInfo[dwItemKindIdx].NaeRyuk_Min = fp.GetWord();
			m_RereItemInfo[dwItemKindIdx].NaeRyuk = fp.GetWord();
			m_RereItemInfo[dwItemKindIdx].Shield_Min = fp.GetDword();
			m_RereItemInfo[dwItemKindIdx].Shield = fp.GetDword();
			m_RereItemInfo[dwItemKindIdx].PhyAttack_Min = fp.GetWord();
			m_RereItemInfo[dwItemKindIdx].PhyAttack = fp.GetWord();
			m_RereItemInfo[dwItemKindIdx].PhyDefense_Min = fp.GetWord();
			m_RereItemInfo[dwItemKindIdx].PhyDefense = fp.GetWord();
			for (int i = ATTR_FIRE; i <= ATTR_MAX; ++i)
			{
				m_RereItemInfo[dwItemKindIdx].AttrAttack_Min.SetElement_Val(i, fp.GetFloat());
				m_RereItemInfo[dwItemKindIdx].AttrAttack.SetElement_Val(i, fp.GetFloat());
			}
			for (int j = ATTR_FIRE; j <= ATTR_MAX; ++j)
			{
				m_RereItemInfo[dwItemKindIdx].AttrRegist_Min.SetElement_Val(j, fp.GetFloat());
				m_RereItemInfo[dwItemKindIdx].AttrRegist.SetElement_Val(j, fp.GetFloat());
			}

			dwItemKindIdx++;
		}

	}

	return TRUE;
}





