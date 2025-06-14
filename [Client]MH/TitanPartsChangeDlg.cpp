







// TitanPartsChangeDlg.cpp: implementation of the CTitanPartsChangeDlg class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "TitanPartsChangeDlg.h"
#include "ObjectManager.h"
#include "ObjectStateManager.h"
#include "WindowIDEnum.h"
#include "cWindowManager.h"
#include "TitanManager.h"
#include "ChatManager.h"
#include "cMsgBox.h"
#include "item.h"
#include "GameIn.h"
#include "TitanChangePreView.h"
#include "ItemManager.h"

#include "WearedExDialog.h"
#include "InventoryExDialog.h"
#include "MHAudioManager.h"
#include "MouseCursor.h"


CTitanPartsChangeDlg::CTitanPartsChangeDlg()
{
	m_pTitanPartsWearedDlg = NULL;
	ZeroMemory( m_dwWearInfoArray, sizeof(m_dwWearInfoArray) );
}

CTitanPartsChangeDlg::~CTitanPartsChangeDlg()
{

}

void CTitanPartsChangeDlg::Linking()
{
	m_pTitanPartsWearedDlg					= (CWearedExDialog *)GetWindowForID(TitanPartsChange_WEAREDDLG);
	m_TitanPartsChangeCtrl.titanType		= (cStatic*)GetWindowForID(TitanPartsChange_TYPE);
	m_TitanPartsChangeCtrl.titanAtt			= (cStatic*)GetWindowForID(TitanPartsChange_CHARATTACK);
	m_TitanPartsChangeCtrl.titanLongAtt		= (cStatic*)GetWindowForID(TitanPartsChange_LONGATTACK);
	m_TitanPartsChangeCtrl.titanMagicAtt	= (cStatic*)GetWindowForID(TitanPartsChange_HOSINDEFENSE);	// magi82 - �ӽ�(ȣ�Ű��⸦ ���°������� �ٲ����)
	m_TitanPartsChangeCtrl.titanCritical	= (cStatic*)GetWindowForID(TitanPartsChange_CRITICAL);
	m_TitanPartsChangeCtrl.titanDistance	= (cStatic*)GetWindowForID(TitanPartsChange_DISTANCE);
	m_TitanPartsChangeCtrl.titanDef			= (cStatic*)GetWindowForID(TitanPartsChange_CHARDEFENSE);
	m_TitanPartsChangeCtrl.titanLife		= (cStatic*)GetWindowForID(TitanPartsChange_CHARLIFE);
	m_TitanPartsChangeCtrl.titanMana		= (cStatic*)GetWindowForID(TitanPartsChange_CHARNAERYUK);



	for( int i = 0; i < ATTR_MAX; i++ )
	{
		m_TitanPartsChangeCtrl.TitanAttrDef.pStatic.SetElement_Val(ATTR_FIRE+i,(cStatic *)GetWindowForID(TitanPartsChange_CHARHWA+i));
		m_TitanPartsChangeCtrl.TitanAttrDef.pGuage.SetElement_Val(ATTR_FIRE+i,(cGuagen *)GetWindowForID(TitanPartsChange_DEFENCE_HWA+i));
	}
}

void CTitanPartsChangeDlg::SetActive( BOOL val )
{
	if(WINDOWMGR->GetWindowForID(MBI_TITAN_REPAIR) == NULL)
	{
		cDialog::SetActiveRecursive( val );


		ZeroMemory( m_dwWearInfoArray, sizeof(m_dwWearInfoArray) );

		for( int i = 0; i < eTitanWearedItem_Max; i++ )
		{
			CItem * pItem = (CItem *)m_pTitanPartsWearedDlg->GetIconForIdx(i);
			if( pItem )
			{
				ITEM_INFO* pinfo = ITEMMGR->GetItemInfo(pItem->GetItemIdx());
				m_dwWearInfoArray[i] = pItem->GetItemIdx();
			}
		}

		if( !val )
		{
			CURSOR->SetCursor(eCURSOR_DEFAULT);

			if( HERO->GetState() == eObjectState_Deal )
				OBJECTSTATEMGR->EndObjectState(HERO, eObjectState_Deal);

			HERO->ReleaseTitanPreView();
		}
	}
}

BOOL CTitanPartsChangeDlg::OnActionEvent(LONG lId, void * p, DWORD we)
{
	switch(we)
	{
	case WE_LBTNCLICK:
		{
			CItem * pItem = (CItem*)m_pTitanPartsWearedDlg->GetIconForIdx(m_pTitanPartsWearedDlg->GetCurSelCellPos());
			
			if(!pItem)
				break;

			if(CURSOR->GetCursor() == eCURSOR_TITANREPAIR)
			{
				if(pItem->GetItemKind() & eTITAN_EQUIPITEM)
				{

					TITAN_ENDURANCE_ITEMINFO* pEnduranceInfo = TITANMGR->GetTitanEnduranceInfo(pItem->GetDBIdx());
					if( !pEnduranceInfo )
						break;

					if( pEnduranceInfo->Endurance >= TITAN_EQUIPITEM_ENDURANCE_MAX )
					{
						CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(1541));
						break;
					}

					float fEnduranceRate = (float)pEnduranceInfo->Endurance * 100 / TITAN_EQUIPITEM_ENDURANCE_MAX;
					if( fEnduranceRate > 0.0f && fEnduranceRate < 1.0f )
					{
						fEnduranceRate = 1.0f;
					}

					DWORD dwRepairPrice = (DWORD)( (100 - (DWORD)fEnduranceRate) * (pItem->GetItemInfo()->BuyPrice / 100) * 0.09f );

					TITANMGR->SetTitanRepairItem(pItem);
					WINDOWMGR->MsgBox( MBI_TITAN_REPAIR, MBT_YESNO, CHATMGR->GetChatMsg(1538), dwRepairPrice );
					ITEMMGR->SetDisableDialog(TRUE, eItemTable_Titan);
					ITEMMGR->SetDisableDialog(TRUE, eItemTable_Inventory);
				}
				else
				{
					CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1539) );
				}
			}
		}
		break;
	case WE_LBTNDBLCLICK:
		{
			CItem * pItem = NULL;
			pItem = (CItem*)m_pTitanPartsWearedDlg->GetIconForIdx((WORD)m_pTitanPartsWearedDlg->GetCurSelCellPos());
			if( !pItem ) return FALSE;

			if(CURSOR->GetCursor() != eCURSOR_TITANREPAIR)
			{
				GAMEIN->GetInventoryDialog()->UseItem(pItem);
			}
		}
		break;
	case WE_CLOSEWINDOW:
		{
			CTitanPartsChangeDlg* pTitanPartsChangeDlg = (CTitanPartsChangeDlg*)WINDOWMGR->GetWindowForID(lId);
			//pTitanPartsChangeDlg->UpdateData(NULL, FALSE);
			//pTitanPartsChangeDlg->Release();
			if( HERO->GetState() == eObjectState_Deal )
				OBJECTSTATEMGR->EndObjectState(HERO, eObjectState_Deal);

			return TRUE;
		}
		break;
	}

	switch( lId )
	{
	case TitanPartsChange_REPAIR:	// ��������
		{
			if(CURSOR->GetCursor() == eCURSOR_TITANREPAIR)
			{
				CURSOR->SetCursor(eCURSOR_DEFAULT);
			}
			else if(CURSOR->GetCursor() == eCURSOR_DEFAULT)
			{
				CURSOR->SetCursor(eCURSOR_TITANREPAIR);
			}
		}
		break;
	case TitanPartsChange_ALLREPAIR:// ��ü����
		{
			MSG_TITAN_REPAIR_TOTAL_EQUIPITEM_SYN msg;
			msg.Init();
			DWORD dwMoney = TITANMGR->GetTitanEnduranceTotalInfo(&msg, TRUE);

			if(dwMoney == 0)
			{
				CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(1582));
			}
			else
			{
				WINDOWMGR->MsgBox( MBI_TITAN_TOTAL_REPAIR, MBT_YESNO, CHATMGR->GetChatMsg(1543), dwMoney );
			}
		}
		break;
	case TitanPartsChange_LOOKBTN:	// �̸�����
		{
			TITAN_TOTALINFO* pinfo = TITANMGR->GetTitanInfo( TITANMGR->GetRegistedTitanItemDBIdx() );

			TITAN_APPEARANCEINFO	titaninfo;
			memset( &titaninfo, 0, sizeof(TITAN_APPEARANCEINFO) );
			titaninfo.TitanGrade = pinfo->TitanGrade;
			titaninfo.TitanKind = pinfo->TitanKind;
			titaninfo.TitanScale = pinfo->Scale;

			CItem* pitem = NULL;
			for( int i=0; i<eTitanWearedItem_Max; ++i )
			{
				if( pitem = (CItem*)m_pTitanPartsWearedDlg->GetIconForIdx(i) )
					titaninfo.WearedItemIdx[i] = pitem->GetItemIdx();
			}

			HERO->SetTitanPreView( &titaninfo );
		}
		break;
	}

	return TRUE;
}

BOOL CTitanPartsChangeDlg::FakeMoveIcon(LONG x, LONG y, cIcon * icon)
{
	// ���������� üũ
	if(icon->GetType() != WT_ITEM)	return FALSE;
	if( m_bDisable ) return FALSE;

	CItem* pFromItem = (CItem*)icon;
	if( pFromItem->IsLocked() )	return FALSE;


	// �Ӽ��� Ÿ��ź ���������� üũ
	eITEM_KINDBIT bits = pFromItem->GetItemKind();
	if( !(bits & eTITAN_EQUIPITEM) )
	{
		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1506) );
		return FALSE;
	}

	// ���콺 �̺�Ʈ �߻��Ѱ��� ����� üũ
	WORD ToPos=0;
	if( !GetPositionForXYRef( x, y, ToPos ) )	// ������ġ �Ѿ� ��
		return FALSE;

	// ������ ���������� �������� �̵��������� üũ
	if( CanBeMoved( pFromItem, ToPos ) == FALSE )
		return FALSE;

	// ���콺 �̺�Ʈ �߻��� ���Կ� �������� �̹� �ִ��� üũ
	CItem * pToItem = (CItem *)m_pTitanPartsWearedDlg->GetIconForIdx(ToPos-TP_TITANWEAR_START);
	if( pToItem )
		if( pToItem->IsLocked() ) return FALSE;

	// ���� ������ �����ǿ� �������������� �ش�������� �ű���� �������� �����ǿ� �̵��������� üũ
	if( pToItem && GAMEIN->GetInventoryDialog()->CanBeMoved( pToItem, pFromItem->GetPosition() ) == FALSE )
	{
		return FALSE;
	}

	BYTE FromIdx = ITEMMGR->GetTableIdxForAbsPos( pFromItem->GetPosition() );
	if(FromIdx >= eItemTable_Max) return FALSE;

	// â�� -> Ÿ��ź ���� �Ǹ� �ȵ�
	if(FromIdx == eItemTable_Pyoguk)	return FALSE;

	// �꽽�� -> Ÿ��ź ���� �Ǹ� �ȵ�
	if(FromIdx == eItemTable_PetInven)	return FALSE;
    
	if(pToItem && (FromIdx == eItemTable_MunpaWarehouse))

	{
		if(FromIdx == eItemTable_GuildWarehouse)
			CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(53) );

		return FALSE;
	}

	//sound
	if(ITEMMGR->GetTableIdxForAbsPos(ToPos)==eItemTable_Weared)
		AUDIOMGR->Play(59, HERO);
	else
		AUDIOMGR->Play(58, HERO);


	BYTE ToIdx = ITEMMGR->GetTableIdxForAbsPos( ToPos );
	if(ToIdx >= eItemTable_Max) return FALSE;


	MSG_ITEM_MOVE_SYN msg;
	msg.Category			= MP_ITEM;
	msg.Protocol			= MP_ITEM_MOVE_SYN;
	msg.dwObjectID			= HEROID;

	msg.FromPos				= pFromItem->GetPosition();
	msg.wFromItemIdx		= pFromItem->GetItemIdx();
	msg.ToPos				= ToPos;
	msg.wToItemIdx			= (pToItem?pToItem->GetItemIdx():0);

	if( msg.FromPos == msg.ToPos )
		return FALSE;

	ITEMMGR->SetDisableDialog(TRUE, eItemTable_Inventory);
	ITEMMGR->SetDisableDialog(TRUE, eItemTable_Pyoguk);
	ITEMMGR->SetDisableDialog(TRUE, eItemTable_GuildWarehouse);
	ITEMMGR->SetDisableDialog(TRUE, eItemTable_PetInven);

	NETWORK->Send( &msg, sizeof(msg) );

	return TRUE;
}

void CTitanPartsChangeDlg::UpdateData(BOOL bFlag)
{
	if( bFlag == TRUE )
	{
		char buf[64] = { 0, };
		titan_calc_stats* TitanStatsInfo = TITANMGR->GetTitanStats();	// Ÿ��ź ������������ �������� ����� ����Ʈ

		// Ÿ��ź Ÿ��
		ZeroMemory(buf, sizeof(buf));
		sprintf(buf, CHATMGR->GetChatMsg(1500), TITANMGR->GetTitanGrade(TITANMGR->GetRegistedTitanItemDBIdx()));
		m_TitanPartsChangeCtrl.titanType->SetStaticText(buf);

		// Ÿ��ź �ּ�/�ִ� ���ݷ�
		ZeroMemory(buf, sizeof(buf));
		sprintf(buf, "%d ~ %d", TitanStatsInfo->MeleeAttackPowerMin, TitanStatsInfo->MeleeAttackPowerMax);
		m_TitanPartsChangeCtrl.titanAtt->SetStaticText(buf);

		// Ÿ��ź ���Ÿ� ���ݷ�
		ZeroMemory(buf, sizeof(buf));

		sprintf(buf, "%d ~ %d", TitanStatsInfo->RangeAttackPowerMin, TitanStatsInfo->RangeAttackPowerMax);
		m_TitanPartsChangeCtrl.titanLongAtt->SetStaticText(buf);

		// Ÿ��ź ��/���� �ϰ�
		ZeroMemory(buf, sizeof(buf));
		sprintf(buf, "%d", TitanStatsInfo->Critical);
		m_TitanPartsChangeCtrl.titanCritical->SetStaticText(buf);

		// Ÿ��ź �����Ÿ�
		ZeroMemory(buf, sizeof(buf));
		sprintf(buf, "%.2f", TitanStatsInfo->AttackRange);
		m_TitanPartsChangeCtrl.titanDistance->SetStaticText(buf);

		// Ÿ��ź ����

		ZeroMemory(buf, sizeof(buf));
		sprintf(buf, "%d", TitanStatsInfo->PhysicalDefense);
		m_TitanPartsChangeCtrl.titanDef->SetStaticText(buf);

		// Ÿ��ź ����
		ZeroMemory(buf, sizeof(buf));
		sprintf(buf, "%d", TitanStatsInfo->MaxFuel);
		m_TitanPartsChangeCtrl.titanLife->SetStaticText(buf);

		// Ÿ��ź ����
		ZeroMemory(buf, sizeof(buf));
		sprintf(buf, "%d", TitanStatsInfo->MaxSpell);
		m_TitanPartsChangeCtrl.titanMana->SetStaticText(buf);


		// Ÿ��ź ��������(ȭ �� �� �� �� ���� ������ ��ġ�� �־����Ƿ� � ���� �־�������)
		ZeroMemory(buf, sizeof(buf));
		sprintf(buf, "%.2f", TitanStatsInfo->AttributeAttack.GetElement_Val(ATTR_FIRE));
		m_TitanPartsChangeCtrl.titanMagicAtt->SetStaticText(buf);


		// Ÿ��ź ���׷�
		m_TitanPartsChangeCtrl.TitanAttrDef.SetValue(ATTR_FIRE, (WORD)TitanStatsInfo->AttributeResist.GetElement_Val(ATTR_FIRE), RGB_HALF(255,255,255));
		m_TitanPartsChangeCtrl.TitanAttrDef.SetValue(ATTR_WATER, (WORD)TitanStatsInfo->AttributeResist.GetElement_Val(ATTR_WATER), RGB_HALF(255,255,255));
		m_TitanPartsChangeCtrl.TitanAttrDef.SetValue(ATTR_TREE, (WORD)TitanStatsInfo->AttributeResist.GetElement_Val(ATTR_TREE), RGB_HALF(255,255,255));
		m_TitanPartsChangeCtrl.TitanAttrDef.SetValue(ATTR_IRON, (WORD)TitanStatsInfo->AttributeResist.GetElement_Val(ATTR_IRON), RGB_HALF(255,255,255));
		m_TitanPartsChangeCtrl.TitanAttrDef.SetValue(ATTR_EARTH, (WORD)TitanStatsInfo->AttributeResist.GetElement_Val(ATTR_EARTH), RGB_HALF(255,255,255));
	}
	else	// ���̾�α�â�� �����Ҷ� ��� ������ �ʱ�ȭ�Ѵ�.
	{
		m_TitanPartsChangeCtrl.titanType->SetStaticText("");
		m_TitanPartsChangeCtrl.titanAtt->SetStaticText("");
		m_TitanPartsChangeCtrl.titanLongAtt->SetStaticText("");
		m_TitanPartsChangeCtrl.titanMagicAtt->SetStaticText("");
		m_TitanPartsChangeCtrl.titanCritical->SetStaticText("");
		m_TitanPartsChangeCtrl.titanDistance->SetStaticText("");

		m_TitanPartsChangeCtrl.titanDef->SetStaticText("");

		m_TitanPartsChangeCtrl.titanLife->SetStaticText("");
		m_TitanPartsChangeCtrl.titanMana->SetStaticText("");

		// Ÿ��ź ���׷�
		m_TitanPartsChangeCtrl.TitanAttrDef.SetValue(ATTR_FIRE, 0, RGB_HALF(255,255,255));
		m_TitanPartsChangeCtrl.TitanAttrDef.SetValue(ATTR_WATER, 0, RGB_HALF(255,255,255));
		m_TitanPartsChangeCtrl.TitanAttrDef.SetValue(ATTR_TREE, 0, RGB_HALF(255,255,255));
		m_TitanPartsChangeCtrl.TitanAttrDef.SetValue(ATTR_IRON, 0, RGB_HALF(255,255,255));
		m_TitanPartsChangeCtrl.TitanAttrDef.SetValue(ATTR_EARTH, 0, RGB_HALF(255,255,255));
	}
}

BOOL CTitanPartsChangeDlg::GetPositionForXYRef(LONG x, LONG y, WORD& pos)
{
	WORD position=0;										// ��� ��ġ

	if(m_pTitanPartsWearedDlg->GetPositionForXYRef(x, y, position))
	{
		pos = position+TP_TITANWEAR_START;					// -> ���� ��ġ�� �ٲ�
		return TRUE;
	}

	return FALSE;
}

BOOL CTitanPartsChangeDlg::DeleteItem(POSTYPE Pos,CItem** ppItem)
{
	if( TP_TITANWEAR_START <= Pos && Pos < TP_TITANWEAR_END )
	{
		if(!m_pTitanPartsWearedDlg->IsAddable(Pos - TP_TITANWEAR_START))
		{
			return m_pTitanPartsWearedDlg->DeleteItem(Pos - TP_TITANWEAR_START, (cIcon**)ppItem);
		}
		else
			return FALSE;
	}
	else
		return FALSE;
}

BOOL CTitanPartsChangeDlg::AddItem( ITEMBASE* pItemBase )
{
	if( pItemBase->dwDBIdx == 0 )
	{
		//		ASSERTMSG( 0, "PetInven NewItem DBIdx is Null");
		return FALSE;
	}
	CItem* pNewItem = ITEMMGR->MakeNewItem( pItemBase, "CTitanPartsChangeDlg::AddItem" );
	if(pNewItem == NULL)
		return FALSE;

	return AddItem(pNewItem);
}

BOOL CTitanPartsChangeDlg::AddItem(CItem* pItem)
{
	if( NULL == pItem )
	{
		ASSERT(pItem);
		return FALSE;
	}

	ITEMMGR->RefreshItem(pItem);

	int Pos = pItem->GetPosition();

	if( TP_TITANWEAR_START <= Pos && Pos < TP_TITANWEAR_END )
	{
		return m_pTitanPartsWearedDlg->AddItem(Pos - TP_TITANWEAR_START, pItem);
	}
	else
	{
		return FALSE;
	}
}

BOOL CTitanPartsChangeDlg::CanBeMoved(CItem* pItem,POSTYPE pos)
{
	ASSERT(pItem);

	if(TP_TITANWEAR_START <= pos && pos < TP_TITANWEAR_END)
	{
		// Ÿ��ź ������������ �ƴϸ� FALSE
		if( ( pItem->GetItemKind() & eTITAN_EQUIPITEM ) )

		{
			// ���� ���ڸ��� �ƴϸ� FALSE
			ITEM_INFO * pInfo = ITEMMGR->GetItemInfo(pItem->GetItemIdx());
			if(!pInfo) return FALSE;

			if(pos-TP_TITANWEAR_START != pInfo->ItemKind-eTITAN_EQUIPITEM_HELMET)
			{
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1505) );
				return FALSE;
			}

			return TRUE;
		}
	}

	return FALSE;
}

void CTitanPartsChangeDlg::SetTitanStatsInfo()
{
	if(TITANMGR->GetRegistedTitanItemDBIdx())
	{

		UpdateData( TRUE );
	}
}

CItem* CTitanPartsChangeDlg::GetItemForPos(POSTYPE absPos)
{
	if(TP_TITANWEAR_START <= absPos && absPos < TP_TITANWEAR_END)
	{
		return (CItem*)m_pTitanPartsWearedDlg->GetIconForIdx(absPos-TP_TITANWEAR_START);
	}
	else
		return NULL;
}

CItem* CTitanPartsChangeDlg::GetCurSelectItem()
{
	if(m_pTitanPartsWearedDlg->GetCurSelCellPos() == -1)
		return NULL;

	return (CItem*)m_pTitanPartsWearedDlg->GetIconForIdx((WORD)m_pTitanPartsWearedDlg->GetCurSelCellPos());
}




