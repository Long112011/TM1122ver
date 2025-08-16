#include "stdafx.h"
#include "GradeChangeDlg.h"
#include "GameIn.h"
#include "ChatManager.h"
#include "InventoryExDialog.h"
#include "ItemManager.h"
#include "DealItem.h"
#include "cWindowManager.h"
#include "cScriptManager.h"
#include "ObjectManager.h"
#include "ObjectStateManager.h"
CGradeChangeDlg::CGradeChangeDlg()
{
    cImage imgToolTip;
    SCRIPTMGR->GetImage(63, &imgToolTip, PFT_HARDPATH);
    m_FORM_VITEM.SetToolTip("", RGB_HALF(255, 255, 255), &imgToolTip);
    m_FORM_VITEM.SetMovable(FALSE);
    m_TO_VITEM.SetToolTip("", RGB_HALF(255, 255, 255), &imgToolTip);
    m_TO_VITEM.SetMovable(FALSE);

    m_ItemIdx = 0;
    m_ItemPos = 0;
}
CGradeChangeDlg::~CGradeChangeDlg()
{
}
void CGradeChangeDlg::Linking()
{
    ICON_FORM_ITEM = (cIconGridDialog*)GetWindowForID(GGD_ICON_FROM_ITEM);
    ICON_TO_ITEM = (cIconGridDialog*)GetWindowForID(GGD_ICON_TO_ITEM);
    ICON_LAST_ITEM = (cIconGridDialog*)GetWindowForID(GGD_ICON_LAST_ITEM);
    GradeColor1 = (cStatic*)GetWindowForID(GGD_Static_GradeColor);
}
void CGradeChangeDlg::SetActive(BOOL val)
{
    if (!val)
    {
        if (ICON_FORM_ITEM)
        {
            CVirtualItem* pVItem = NULL;

            ICON_FORM_ITEM->DeleteIcon(0, (cIcon**)&pVItem);
            if (pVItem)
                pVItem->GetLinkItem()->SetLock(FALSE);

            m_FORM_VITEM.SetLinkItem(NULL);
        }
        if (ICON_TO_ITEM)
        {
            CVirtualItem* pVItem = NULL;

            ICON_TO_ITEM->DeleteIcon(0, (cIcon**)&pVItem);
            if (pVItem)
                pVItem->GetLinkItem()->SetLock(FALSE);

            m_TO_VITEM.SetLinkItem(NULL);
        }
        if (ICON_LAST_ITEM)
        {
            CVirtualItem* pVItem = NULL;

            ICON_LAST_ITEM->DeleteIcon(0, (cIcon**)&pVItem);
            m_TO_VITEM.SetLinkItem(NULL);
        }
        CItem* pOriItem = ITEMMGR->GetItemofTable(eItemTable_ShopInven, (POSTYPE)m_ItemPos);
        if (pOriItem)
            pOriItem->SetLock(FALSE);

        m_ItemIdx = 0;
        m_ItemPos = 0;
        GradeColor1->SetActive(false);

        ITEMMGR->SetDisableDialog(FALSE, eItemTable_Inventory);
        ITEMMGR->SetDisableDialog(FALSE, eItemTable_Pyoguk);
        ITEMMGR->SetDisableDialog(FALSE, eItemTable_MunpaWarehouse);
        ITEMMGR->SetDisableDialog(FALSE, eItemTable_Shop);

        OBJECTSTATEMGR->EndObjectState(HERO, eObjectState_Deal);
    }
    cDialog::SetActive(val);
}
void CGradeChangeDlg::OnActionEvent(LONG lId, void* p, DWORD we)
{
    if (lId == GGD_BTN_CLOSE)	//关闭按钮
    {
        SetActive(FALSE);
    }
    else if (lId == GGD_BTN_OK)	//确定按钮
    {
        if (!m_FORM_VITEM.GetLinkItem() || !m_TO_VITEM.GetLinkItem())
        {
            CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(2840));
            return;
        }
        CItem* pFromItem = ITEMMGR->GetItem(m_FORM_VITEM.GetLinkItem()->GetDBIdx());
        CItem* pToItem = ITEMMGR->GetItem(m_TO_VITEM.GetLinkItem()->GetDBIdx());

        if (!pFromItem || !pToItem) return;

        MSG_DWORD6	msg;
        SetProtocol(&msg, MP_ITEM, MP_ITEM_SHOPITEM_GRADECHANGE_SYN);
        msg.dwObjectID = HEROID;
        msg.dwData0 = m_ItemIdx;
        msg.dwData1 = m_ItemPos;
        msg.dwData2 = pFromItem->GetItemBaseInfo()->wIconIdx;
        msg.dwData3 = pFromItem->GetItemBaseInfo()->Position;
        msg.dwData4 = pToItem->GetItemBaseInfo()->wIconIdx;
        msg.dwData5 = pToItem->GetItemBaseInfo()->Position;

        //// 使用 sprintf 格式化字符串
        //char buffer[256];
        //sprintf(buffer, "数据: dwData0=%d, dwData1=%d, dwData2=%d, dwData3=%d, dwData4=%d, dwData5=%d",
        //	msg.dwData0, msg.dwData1, msg.dwData2, msg.dwData3, msg.dwData4, msg.dwData5);

        //// 输出炔烃信息及数据
        //CHATMGR->AddMsg(CTC_SYSMSG, "发出协议 MP_ITEM_SHOPITEM_GRADECHANGE_SYN");
        //CHATMGR->AddMsg(CTC_SYSMSG, buffer);

        NETWORK->Send(&msg, sizeof(msg));
        SetActive(FALSE);
    }
}

BOOL CGradeChangeDlg::FakeMoveIcon(LONG x, LONG y, cIcon* pOrigIcon)
{
    if (pOrigIcon->GetType() != WT_ITEM) return FALSE;

    ITEM_INFO* pShopItem = ITEMMGR->GetItemInfo((WORD)m_ItemIdx);
    if (!pShopItem)
        return FALSE;

    // X = 左，Y = 上
    WORD PosX = -1;
    WORD PosY = -1;

    // ICON_FORM_ITEM 区域
    ICON_FORM_ITEM->GetCellPosition(x, y, PosX, PosY);
    if (PosX == 0)
    {
        CDealItem* pItemCheck = (CDealItem*)ICON_FORM_ITEM->GetIconForIdx(0);
        if (pItemCheck)
            return FALSE;

        CItem* pItem = (CItem*)pOrigIcon;

        eITEM_KINDBIT bits = GetItemKind(pItem->GetItemIdx());
        if (bits == eEQUIP_ITEM_WEAPON || bits == eEQUIP_ITEM_DRESS || bits == eEQUIP_ITEM_HAT
            || bits == eEQUIP_ITEM_SHOES || bits == eEQUIP_ITEM_RING || bits == eEQUIP_ITEM_CAPE || bits == eEQUIP_ITEM_BELT
            || bits == eEQUIP_ITEM_NECKLACE || bits == eEQUIP_ITEM_ARMLET )
        {
            // 等级检查：小于 5 的物品不能放入
            if (pItem->GetGrade() < 5)
            {
                CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(2846));  // 等级不足提示
                return FALSE;
            }

            if (pItem->IsLocked()) return FALSE;
            char line[128];
            if (!ITEMMGR->IsEqualTableIdxForPos(eItemTable_Inventory, pItem->GetPosition())) return FALSE;

            pItem->SetLock(TRUE);

            m_FORM_VITEM.SetData(pItem->GetItemIdx());
            m_FORM_VITEM.SetLinkItem(pItem);
            SHOPITEMBASE* pShopItemInfo = NULL;
            pShopItemInfo = ITEMMGR->GetUsedItemInfo(pItem->GetItemBaseInfo()->wIconIdx);
            if (pShopItemInfo)
                ITEMMGR->AddUsedAvatarItemToolTip(pShopItemInfo);
            ICON_FORM_ITEM->AddIcon(0, (cIcon*)&m_FORM_VITEM);

            CDealItem* pItemCheck2 = (CDealItem*)ICON_TO_ITEM->GetIconForIdx(0);
            if (pItemCheck2)
            {
                ITEMMGR->SetDisableDialog(TRUE, eItemTable_Inventory);
                ITEMMGR->SetDisableDialog(TRUE, eItemTable_Pyoguk);
                ITEMMGR->SetDisableDialog(TRUE, eItemTable_GuildWarehouse);
                ITEMMGR->SetDisableDialog(TRUE, eItemTable_Shop);
            }
            sprintf(line, "+%d", (pItem->GetGrade() - 5));
            GradeColor1->SetStaticText(line);
            GradeColor1->SetActive(true);
            return FALSE;
        }
        else
        {
            CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(2842));  // 非装备物品提示
        }
    }

    PosX = -1;
    PosY = -1;

    // ICON_TO_ITEM 区域
    ICON_TO_ITEM->GetCellPosition(x, y, PosX, PosY);
    if (PosX == 0)
    {
        CDealItem* pItemCheck = (CDealItem*)ICON_TO_ITEM->GetIconForIdx(0);
        if (pItemCheck)
            return FALSE;

        CItem* pItem = (CItem*)pOrigIcon;

        eITEM_KINDBIT bits = GetItemKind(pItem->GetItemIdx());
        if (bits == eEQUIP_ITEM_WEAPON || bits == eEQUIP_ITEM_DRESS || bits == eEQUIP_ITEM_HAT
            || bits == eEQUIP_ITEM_SHOES || bits == eEQUIP_ITEM_RING || bits == eEQUIP_ITEM_CAPE || bits == eEQUIP_ITEM_BELT
            || bits == eEQUIP_ITEM_NECKLACE || bits == eEQUIP_ITEM_ARMLET )
        {
            // 等级检查：大于等于 1 的物品不能放入
            if (pItem->GetGrade() >= 1)
            {
                CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(2847));  // 等级大于等于 1 提示
                return FALSE;
            }

            if (pItem->IsLocked()) return FALSE;

            if (!ITEMMGR->IsEqualTableIdxForPos(eItemTable_Inventory, pItem->GetPosition())) return FALSE;

            pItem->SetLock(TRUE);

            m_TO_VITEM.SetData(pItem->GetItemIdx());
            m_TO_VITEM.SetLinkItem(pItem);
            SHOPITEMBASE* pShopItemInfo = NULL;
            pShopItemInfo = ITEMMGR->GetUsedItemInfo(pItem->GetItemBaseInfo()->wIconIdx);
            if (pShopItemInfo)
                ITEMMGR->AddUsedAvatarItemToolTip(pShopItemInfo);
            ICON_TO_ITEM->AddIcon(0, (cIcon*)&m_TO_VITEM);

            // 更新 ICON_LAST_ITEM 的内容
            m_LAST_VITEM.SetData(pItem->GetItemIdx());
            m_LAST_VITEM.SetLinkItem(pItem);

            // **设置 ICON_LAST_ITEM 的等级为当前等级 - 5**
            CItem* pLastItem = (CItem*)&m_LAST_VITEM;
            pLastItem->SetGrade(pItem->GetGrade() - 5);

            ICON_LAST_ITEM->AddIcon(0, (cIcon*)&m_LAST_VITEM);

            // 禁用其他界面
            CDealItem* pItemCheck2 = (CDealItem*)ICON_FORM_ITEM->GetIconForIdx(0);
            if (pItemCheck2)
            {
                ITEMMGR->SetDisableDialog(TRUE, eItemTable_Inventory);
                ITEMMGR->SetDisableDialog(TRUE, eItemTable_Pyoguk);
                ITEMMGR->SetDisableDialog(TRUE, eItemTable_GuildWarehouse);
                ITEMMGR->SetDisableDialog(TRUE, eItemTable_Shop);
            }
            return FALSE;
        }
        else
        {
            CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(2842));  // 非装备物品提示
        }
    }

    return FALSE;
}

