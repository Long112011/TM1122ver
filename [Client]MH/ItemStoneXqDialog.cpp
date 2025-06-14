#include "stdafx.h"
#include "ItemStoneXqDialog.h"
#include "WindowIDEnum.h"
#include "./Interface/cIconDialog.h"
#include "./Interface/cWindowManager.h"
#include "./Interface/cStatic.h"
#include "./Interface/cIcon.h"
#include "ChatManager.h"
#include "ObjectStateManager.h"
#include "ObjectManager.h"
#include "ItemManager.h"

// 2014-11-18 系统开发

CItemStoneXqDialog::CItemStoneXqDialog()
{
	pDlg=NULL;
	pRate=	NULL;
	pXqItem=NULL;
	m_pIconDlgEquip=NULL;
	m_pIconDlgStone0=NULL;
	m_pIconDlgStone1=NULL;
	m_pIconDlgStone2=NULL;
	m_pIconDlgStone3=NULL;
	m_pIconDlgStone4=NULL;

    m_IconStone0=NULL;
    m_IconStone1=NULL;
    m_IconStone2=NULL;
    m_IconStone3=NULL;
    m_IconStone4=NULL;

}

CItemStoneXqDialog::~CItemStoneXqDialog()
{
	pDlg=NULL;
	pRate=	NULL;
	pXqItem=NULL;
	m_pIconDlgEquip=NULL;
	m_pIconDlgStone0=NULL;
	m_pIconDlgStone1=NULL;
	m_pIconDlgStone2=NULL;
	m_pIconDlgStone3=NULL;
	m_pIconDlgStone4=NULL;

    m_IconStone0=NULL;
    m_IconStone1=NULL;
    m_IconStone2=NULL;
    m_IconStone3=NULL;
    m_IconStone4=NULL;
}

void CItemStoneXqDialog::Linking()
{
    pDlg = WINDOWMGR->GetWindowForID( ITEMSTONEXQ_DLG );
	pRate= (cStatic*)GetWindowForID(ITEMSTONEXQ_RATE);
	m_pIconDlgEquip  = (cIconDialog*)GetWindowForID(ITEMSTONEXQ_ICON);
	m_pIconDlgStone0 = (cIconDialog*)GetWindowForID(ITEMSTONEXQ_ICON0);
	m_pIconDlgStone1 = (cIconDialog*)GetWindowForID(ITEMSTONEXQ_ICON1);
	m_pIconDlgStone2 = (cIconDialog*)GetWindowForID(ITEMSTONEXQ_ICON2);
	m_pIconDlgStone3 = (cIconDialog*)GetWindowForID(ITEMSTONEXQ_ICON3);
	m_pIconDlgStone4 = (cIconDialog*)GetWindowForID(ITEMSTONEXQ_ICON4);

}

void CItemStoneXqDialog::SetXqItem(CItem * pItem)
{
	pXqItem=pItem;
}

CItem * CItemStoneXqDialog::GetXqItem()
{
    return pXqItem;
}

void CItemStoneXqDialog::DeleteXqStoneAll()
{
	if(m_VirtualItemStone0.GetLinkItem())
	{
		MSG_ITEM_USE_SYN msg;
		msg.Category = MP_ITEM;
		msg.Protocol = MP_ITEM_USE_SYN;
		msg.dwObjectID = HEROID;
		msg.TargetPos = m_VirtualItemStone0.GetLinkItem()->GetPosition();
		msg.wItemIdx =  m_VirtualItemStone0.GetLinkItem()->GetItemIdx();
		NETWORK->Send(&msg,sizeof(msg));
	}
	if(m_VirtualItemStone1.GetLinkItem())
	{
		MSG_ITEM_USE_SYN msg;
		msg.Category = MP_ITEM;
		msg.Protocol = MP_ITEM_USE_SYN;
		msg.dwObjectID = HEROID;
		msg.TargetPos = m_VirtualItemStone1.GetLinkItem()->GetPosition();
		msg.wItemIdx =  m_VirtualItemStone1.GetLinkItem()->GetItemIdx();
		NETWORK->Send(&msg,sizeof(msg));
	}
	if(m_VirtualItemStone2.GetLinkItem())
	{
		MSG_ITEM_USE_SYN msg;
		msg.Category = MP_ITEM;
		msg.Protocol = MP_ITEM_USE_SYN;
		msg.dwObjectID = HEROID;
		msg.TargetPos = m_VirtualItemStone2.GetLinkItem()->GetPosition();
		msg.wItemIdx =  m_VirtualItemStone2.GetLinkItem()->GetItemIdx();
		NETWORK->Send(&msg,sizeof(msg));
	}
	if(m_VirtualItemStone3.GetLinkItem())
	{
		MSG_ITEM_USE_SYN msg;
		msg.Category = MP_ITEM;
		msg.Protocol = MP_ITEM_USE_SYN;
		msg.dwObjectID = HEROID;
		msg.TargetPos = m_VirtualItemStone3.GetLinkItem()->GetPosition();
		msg.wItemIdx =  m_VirtualItemStone3.GetLinkItem()->GetItemIdx();
		NETWORK->Send(&msg,sizeof(msg));
	}
	if(m_VirtualItemStone4.GetLinkItem())
	{
		MSG_ITEM_USE_SYN msg;
		msg.Category = MP_ITEM;
		msg.Protocol = MP_ITEM_USE_SYN;
		msg.dwObjectID = HEROID;
		msg.TargetPos = m_VirtualItemStone4.GetLinkItem()->GetPosition();
		msg.wItemIdx =  m_VirtualItemStone4.GetLinkItem()->GetItemIdx();
		NETWORK->Send(&msg,sizeof(msg));
	}
}

BOOL CItemStoneXqDialog::FakeMoveIcon(LONG x,LONG y,cIcon * icon)
{

GotoReset:

    if( icon->GetType() != WT_ITEM )	return FALSE;

	if( m_bDisable )	return FALSE;

	CItem* pOrigItem = (CItem*)icon;

	if( pOrigItem->IsLocked() )	return FALSE;

	ITEM_INFO* pItemInfo = ITEMMGR->GetItemInfo( pOrigItem->GetItemIdx());

	if(!pItemInfo)	return FALSE;

	if(m_VirtualItemEquip.GetData()==0)
	{

		eITEM_KINDBIT bits = pOrigItem->GetItemKind();

	   if(!ITEMMGR->IsStoneOptionItem(pOrigItem->GetItemIdx(),bits))
	   {
			CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1920));

			return FALSE;
	   }

	   ITEM_STONE_OPTION_INFO * pStone=ITEMMGR->GetItemStoneOption(pOrigItem->GetStoneIdx());

	   if(!pStone)
	   {
			CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1921));

			return FALSE;
	   }
 
	   if( !ITEMMGR->IsEqualTableIdxForPos(eItemTable_Inventory, pOrigItem->GetPosition()) && !ITEMMGR->IsEqualTableIdxForPos(eItemTable_ShopInven, pOrigItem->GetPosition()))
	   {
		    return FALSE;
	   }

	    pOrigItem->SetLock(TRUE);

	    AddVirtualItem( pOrigItem );

		AddVirtualIcon( pOrigItem);

	}
	else
	{
		eITEM_KINDBIT bits = pOrigItem->GetItemKind();

		// 2014-01-24 为 4098 且  51004< itemidx <52000  为宝石

		if(bits==eEXTRA_ITEM_MATERIAL && pOrigItem->GetItemIdx()>= 51004  && pOrigItem->GetItemIdx()<= 51100)
		{

		  if(pOrigItem->GetDurability()>1)
		  {
			  return FALSE;
		  }

	      pOrigItem->SetLock(TRUE);

	      AddVirtualItem( pOrigItem );
		}
		else
		{
			ITEM_STONE_OPTION_INFO * pStone=ITEMMGR->GetItemStoneOption(pOrigItem->GetStoneIdx());

			if(pStone)
			{
                ResetItemInfo();

			    goto GotoReset;
			}
			else
			{
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1923));
				return FALSE;
			}
		}
	}	
	//ITEMMGR->SetDisableDialog(TRUE, eItemTable_Inventory);          // 2014-01-09 当拖入时可以使用
	ITEMMGR->SetDisableDialog(TRUE, eItemTable_Pyoguk);
	ITEMMGR->SetDisableDialog(TRUE, eItemTable_GuildWarehouse);
	ITEMMGR->SetDisableDialog(TRUE, eItemTable_Shop);
	ITEMMGR->SetDisableDialog(TRUE, eItemTable_PetInven);
	return FALSE;
}

void CItemStoneXqDialog::AddVirtualItem(CItem * pItem)
{
	if(m_VirtualItemEquip.GetData()==0)
	{
		cIcon* pIcon=(cIcon*)pItem;
		m_VirtualItemEquip.SetData( pItem->GetItemIdx());
		m_VirtualItemEquip.SetLinkItem(pItem);
		m_pIconDlgEquip->AddIcon( 0, (cIcon*)&m_VirtualItemEquip );
		m_pIconDlgEquip->SetDisable(TRUE);
    }
	else
	{
		CItem * pTargetItem = ITEMMGR->GetItem(m_VirtualItemEquip.GetLinkItem()->GetDBIdx());

		if(!pTargetItem) return;

		ITEM_STONE_OPTION_INFO * pStoneInfo = ITEMMGR->GetItemStoneOption(pTargetItem->GetStoneIdx());

		if(!pStoneInfo) return;  // 未 允许放入石头

		if(pStoneInfo->dwItemStone0==1 && m_VirtualItemStone0.GetData()==0)
		{
			cIcon* pIcon=(cIcon*)pItem;
	        m_VirtualItemStone0.SetData( pItem->GetItemIdx());
	        m_VirtualItemStone0.SetLinkItem(pItem);
	        m_pIconDlgStone0->AddIcon( 0, (cIcon*)&m_VirtualItemStone0 );
			m_pIconDlgStone0->SetDepend(TRUE);
	        ITEMMGR->SetToolTipIcon((cIcon*)&m_VirtualItemStone0,NULL,NULL,0,NULL);
			pItem->SetLock(TRUE);
			return;
		}
		if(pStoneInfo->dwItemStone1==1 && m_VirtualItemStone1.GetData()==0)
		{
			cIcon* pIcon=(cIcon*)pItem;
	        m_VirtualItemStone1.SetData( pItem->GetItemIdx());
	        m_VirtualItemStone1.SetLinkItem(pItem);
	        m_pIconDlgStone1->AddIcon( 0, (cIcon*)&m_VirtualItemStone1 );
			m_pIconDlgStone1->SetDepend(TRUE);
	        ITEMMGR->SetToolTipIcon((cIcon*)&m_VirtualItemStone1,NULL,NULL,0,NULL);
			pItem->SetLock(TRUE);
			return;
		}
		if(pStoneInfo->dwItemStone2==1 && m_VirtualItemStone2.GetData()==0)
		{
			cIcon* pIcon=(cIcon*)pItem;
	        m_VirtualItemStone2.SetData( pItem->GetItemIdx());
	        m_VirtualItemStone2.SetLinkItem(pItem);
	        m_pIconDlgStone2->AddIcon( 0, (cIcon*)&m_VirtualItemStone2 );
			m_pIconDlgStone2->SetDepend(TRUE);
	        ITEMMGR->SetToolTipIcon((cIcon*)&m_VirtualItemStone2,NULL,NULL,0,NULL);
			pItem->SetLock(TRUE);
			return;
		}
		if(pStoneInfo->dwItemStone3==1 && m_VirtualItemStone3.GetData()==0)
		{
			cIcon* pIcon=(cIcon*)pItem;
	        m_VirtualItemStone3.SetData( pItem->GetItemIdx());
	        m_VirtualItemStone3.SetLinkItem(pItem);
	        m_pIconDlgStone3->AddIcon( 0, (cIcon*)&m_VirtualItemStone3 );
			m_pIconDlgStone3->SetDepend(TRUE);
	        ITEMMGR->SetToolTipIcon((cIcon*)&m_VirtualItemStone3,NULL,NULL,0,NULL);
			pItem->SetLock(TRUE);
			return;
		}
		if(pStoneInfo->dwItemStone4==1 && m_VirtualItemStone4.GetData()==0)
		{
			cIcon* pIcon=(cIcon*)pItem;
	        m_VirtualItemStone4.SetData( pItem->GetItemIdx());
	        m_VirtualItemStone4.SetLinkItem(pItem);
	        m_pIconDlgStone4->AddIcon( 0, (cIcon*)&m_VirtualItemStone4 );
			m_pIconDlgStone4->SetDepend(TRUE);
	        ITEMMGR->SetToolTipIcon((cIcon*)&m_VirtualItemStone4,NULL,NULL,0,NULL);
			pItem->SetLock(TRUE);
			return;
		}

		if(GetStoneXqCount(pStoneInfo)<5)
		{
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(2129));
				pItem->SetLock(FALSE);
				return ;
		}
		else
		{

				if(pStoneInfo->dwItemStone0>1)  // 从这里孔
				{
					CVirtualItem* pVItem = NULL;
					m_pIconDlgStone0->DeleteIcon( 0, (cIcon**)&pVItem );
					m_VirtualItemStone0.SetLinkItem( NULL );
					cIcon* pIcon=(cIcon*)pItem;
					m_VirtualItemStone0.SetData( pItem->GetItemIdx());
					m_VirtualItemStone0.SetLinkItem(pItem);
					m_pIconDlgStone0->AddIcon( 0, (cIcon*)&m_VirtualItemStone0 );
					m_pIconDlgStone0->SetDepend(TRUE);
					ITEMMGR->SetToolTipIcon((cIcon*)&m_VirtualItemStone0,NULL,NULL,0,NULL);
					pItem->SetLock(TRUE);
					return;
				}
				if(pStoneInfo->dwItemStone1>1)
				{
					CVirtualItem* pVItem = NULL;
					m_pIconDlgStone1->DeleteIcon( 0, (cIcon**)&pVItem );
					m_VirtualItemStone1.SetLinkItem( NULL );
					cIcon* pIcon=(cIcon*)pItem;
					m_VirtualItemStone1.SetData( pItem->GetItemIdx());
					m_VirtualItemStone1.SetLinkItem(pItem);
					m_pIconDlgStone1->AddIcon( 0, (cIcon*)&m_VirtualItemStone1 );
					m_pIconDlgStone1->SetDepend(TRUE);
					ITEMMGR->SetToolTipIcon((cIcon*)&m_VirtualItemStone1,NULL,NULL,0,NULL);
					pItem->SetLock(TRUE);
					return;

				}
				if(pStoneInfo->dwItemStone2>1)
				{
					CVirtualItem* pVItem = NULL;
					m_pIconDlgStone2->DeleteIcon( 0, (cIcon**)&pVItem );
					m_VirtualItemStone2.SetLinkItem( NULL );
					cIcon* pIcon=(cIcon*)pItem;
					m_VirtualItemStone2.SetData( pItem->GetItemIdx());
					m_VirtualItemStone2.SetLinkItem(pItem);
					m_pIconDlgStone2->AddIcon( 0, (cIcon*)&m_VirtualItemStone2 );
					m_pIconDlgStone2->SetDepend(TRUE);
					ITEMMGR->SetToolTipIcon((cIcon*)&m_VirtualItemStone2,NULL,NULL,0,NULL);
					pItem->SetLock(TRUE);
					return;
				}
				if(pStoneInfo->dwItemStone3>1)
				{
					CVirtualItem* pVItem = NULL;
					m_pIconDlgStone3->DeleteIcon( 0, (cIcon**)&pVItem );
					m_VirtualItemStone3.SetLinkItem( NULL );
					cIcon* pIcon=(cIcon*)pItem;
					m_VirtualItemStone3.SetData( pItem->GetItemIdx());
					m_VirtualItemStone3.SetLinkItem(pItem);
					m_pIconDlgStone3->AddIcon( 0, (cIcon*)&m_VirtualItemStone3 );
					m_pIconDlgStone3->SetDepend(TRUE);
					ITEMMGR->SetToolTipIcon((cIcon*)&m_VirtualItemStone3,NULL,NULL,0,NULL);
					pItem->SetLock(TRUE);
					return;
				}
				if(pStoneInfo->dwItemStone0>1)
				{
					CVirtualItem* pVItem = NULL;
					m_pIconDlgStone4->DeleteIcon( 0, (cIcon**)&pVItem );
					m_VirtualItemStone4.SetLinkItem( NULL );
					cIcon* pIcon=(cIcon*)pItem;
					m_VirtualItemStone4.SetData( pItem->GetItemIdx());
					m_VirtualItemStone4.SetLinkItem(pItem);
					m_pIconDlgStone4->AddIcon( 0, (cIcon*)&m_VirtualItemStone4 );
					m_pIconDlgStone4->SetDepend(TRUE);
					ITEMMGR->SetToolTipIcon((cIcon*)&m_VirtualItemStone4,NULL,NULL,0,NULL);
					pItem->SetLock(TRUE);
					return;
				}
		}

    }

}

void CItemStoneXqDialog::AddVirtualIcon(CItem * pItem)
{
	ITEM_STONE_OPTION_INFO * pStone=ITEMMGR->GetItemStoneOption(pItem->GetStoneIdx());

	ITEM_INFO * pInfo;

	cImage tmpImage;

	if(pStone)
	{
		if(pStone->dwItemStone0>1)
		{
            pInfo=ITEMMGR->GetItemInfo(pStone->dwItemStone0);

			if(pInfo)
			{
               m_IconStone0=new cIcon;
			   m_IconStone0->Init(0,0,DEFAULT_ICONSIZE,DEFAULT_ICONSIZE, ITEMMGR->GetIconImage(pInfo->ItemIdx, &tmpImage),IG_DEALITEM_START+4000+0);
			   m_IconStone0->SetData(pInfo->ItemIdx);
			   m_pIconDlgStone0->AddIcon(0,m_IconStone0);
			   pInfo=NULL;

			}
		}
		if(pStone->dwItemStone1>1)
		{
            pInfo=ITEMMGR->GetItemInfo(pStone->dwItemStone1);

			if(pInfo)
			{
               m_IconStone1=new cIcon;
			   m_IconStone1->Init(0,0,DEFAULT_ICONSIZE,DEFAULT_ICONSIZE, ITEMMGR->GetIconImage(pInfo->ItemIdx, &tmpImage),IG_DEALITEM_START+4000+1);
			   m_IconStone1->SetData(pInfo->ItemIdx);
			   m_pIconDlgStone1->AddIcon(0,m_IconStone1);
			   pInfo=NULL;

			}
		}
		if(pStone->dwItemStone2>1)
		{
            pInfo=ITEMMGR->GetItemInfo(pStone->dwItemStone2);

			if(pInfo)
			{
               m_IconStone2=new cIcon;
			   m_IconStone2->Init(0,0,DEFAULT_ICONSIZE,DEFAULT_ICONSIZE, ITEMMGR->GetIconImage(pInfo->ItemIdx, &tmpImage),IG_DEALITEM_START+4000+2);
			   m_IconStone2->SetData(pInfo->ItemIdx);
			   m_pIconDlgStone2->AddIcon(0,m_IconStone2);
			   pInfo=NULL;

			}
		}
		if(pStone->dwItemStone3>1)
		{
            pInfo=ITEMMGR->GetItemInfo(pStone->dwItemStone3);

			if(pInfo)
			{
               m_IconStone3=new cIcon;
			   m_IconStone3->Init(0,0,DEFAULT_ICONSIZE,DEFAULT_ICONSIZE, ITEMMGR->GetIconImage(pInfo->ItemIdx, &tmpImage),IG_DEALITEM_START+4000+3);
			   m_IconStone3->SetData(pInfo->ItemIdx);
			   m_pIconDlgStone3->AddIcon(0,m_IconStone3);
			   pInfo=NULL;

			}
		}
		if(pStone->dwItemStone4>1)
		{
            pInfo=ITEMMGR->GetItemInfo(pStone->dwItemStone4);

			if(pInfo)
			{
               m_IconStone4=new cIcon;
			   m_IconStone4->Init(0,0,DEFAULT_ICONSIZE,DEFAULT_ICONSIZE, ITEMMGR->GetIconImage(pInfo->ItemIdx, &tmpImage),IG_DEALITEM_START+4000+4);
			   m_IconStone4->SetData(pInfo->ItemIdx);
			   m_pIconDlgStone4->AddIcon(0,m_IconStone4);
			   pInfo=NULL;

			}
		}
	}
}

void CItemStoneXqDialog::SendItemStoneXqMsg()
{
	if(!m_VirtualItemEquip.GetLinkItem())	return;

	if(GetStoneVirtualCount()==0)
	{
        CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1921));
	}

	CItem* pItem = ITEMMGR->GetItem( m_VirtualItemEquip.GetLinkItem()->GetDBIdx());

	if(!pItem)
	{
		if( m_pIconDlgEquip )
		{
			CVirtualItem* pVItem = NULL;
			m_pIconDlgEquip->DeleteIcon( 0, (cIcon**)&pVItem );
			if( pVItem )
				pVItem->GetLinkItem()->SetLock( FALSE );
		}
		ITEMMGR->SetDisableDialog(FALSE, eItemTable_Inventory);
		ITEMMGR->SetDisableDialog(FALSE, eItemTable_Pyoguk);
		ITEMMGR->SetDisableDialog(FALSE, eItemTable_GuildWarehouse);
		ITEMMGR->SetDisableDialog(FALSE, eItemTable_Shop);
		ITEMMGR->SetDisableDialog(FALSE, eItemTable_PetInven);
		OBJECTSTATEMGR->EndObjectState( HERO, eObjectState_Deal );
		return;
	}

	MSG_ITEM_XQ_DATA	msg;
	msg.Category = MP_ITEM;
	msg.Protocol = MP_ITEM_STONE_XQ_SYN;
	msg.dwObjectID = HEROID;
	msg.ItemXqStoneIdx=pXqItem->GetItemIdx();
	msg.ItemXqStonePos=pXqItem->GetPosition();
	msg.ItemTargetIdx =m_VirtualItemEquip.GetLinkItem()->GetItemIdx();
	msg.ItemTargetPos =m_VirtualItemEquip.GetLinkItem()->GetPosition();

	ITEM_STONE_OPTION_INFO * pStone=ITEMMGR->GetItemStoneOption(pItem->GetStoneIdx());

	if(m_VirtualItemStone0.GetLinkItem())
	{
		msg.ItemXqItemIdx0=m_VirtualItemStone0.GetLinkItem()->GetItemIdx();
		msg.ItemXqItemPos0=m_VirtualItemStone0.GetLinkItem()->GetPosition();
	}
	else
	{
		msg.ItemXqItemIdx0 =pStone->dwItemStone0;
		msg.ItemXqItemPos0 =0;
	}
	if(m_VirtualItemStone1.GetLinkItem())
	{
		msg.ItemXqItemIdx1=m_VirtualItemStone1.GetLinkItem()->GetItemIdx();
		msg.ItemXqItemPos1=m_VirtualItemStone1.GetLinkItem()->GetPosition();
	}
	else
	{
        msg.ItemXqItemIdx1 =pStone->dwItemStone1;
		msg.ItemXqItemPos1 =0;
	}
	if(m_VirtualItemStone2.GetLinkItem())
	{
		msg.ItemXqItemIdx2=m_VirtualItemStone2.GetLinkItem()->GetItemIdx();
		msg.ItemXqItemPos2=m_VirtualItemStone2.GetLinkItem()->GetPosition();
	}
	else
	{
		msg.ItemXqItemIdx2 =pStone->dwItemStone2;
		msg.ItemXqItemPos2 =0;
	}
	if(m_VirtualItemStone3.GetLinkItem())
	{
		msg.ItemXqItemIdx3=m_VirtualItemStone3.GetLinkItem()->GetItemIdx();
		msg.ItemXqItemPos3=m_VirtualItemStone3.GetLinkItem()->GetPosition();
	}
	else
	{
		msg.ItemXqItemIdx3 =pStone->dwItemStone3;
		msg.ItemXqItemPos3 =0;
	}
	if(m_VirtualItemStone4.GetLinkItem())
	{
		msg.ItemXqItemIdx4=m_VirtualItemStone4.GetLinkItem()->GetItemIdx();
		msg.ItemXqItemPos4=m_VirtualItemStone4.GetLinkItem()->GetPosition();
	}
	else
	{
		msg.ItemXqItemIdx4 =pStone->dwItemStone4;
		msg.ItemXqItemPos4 =0;

	}

	NETWORK->Send( &msg, sizeof(msg) );

	SetActive( FALSE );
}

void CItemStoneXqDialog::ResetItemInfo()
{
	CVirtualItem* pVItem = NULL;

	if( m_pIconDlgEquip )
	{
		
		m_pIconDlgEquip->DeleteIcon( 0, (cIcon**)&pVItem );

		if( pVItem )
			pVItem->GetLinkItem()->SetLock( FALSE );

		m_VirtualItemEquip.SetLinkItem( NULL );
		m_VirtualItemEquip.SetData(0);
	}

	if(m_pIconDlgStone0)
	{
		if(m_IconStone0)
		{
			m_pIconDlgStone0->DeleteIcon(0,&m_IconStone0);
		}
		if(m_VirtualItemStone0.GetData()!=0)
		{
			m_pIconDlgStone0->DeleteIcon( 0, (cIcon**)&pVItem );
			if( pVItem )
				pVItem->GetLinkItem()->SetLock( FALSE );
			m_VirtualItemStone0.SetLinkItem( NULL );
			m_VirtualItemStone0.SetData(0);
		}
	}
	if(m_pIconDlgStone1)
	{
		if(m_IconStone1)
		{
			m_pIconDlgStone1->DeleteIcon(0,&m_IconStone1);
		}
		if(m_VirtualItemStone1.GetData()!=0)
		{
			m_pIconDlgStone1->DeleteIcon( 0, (cIcon**)&pVItem );
			if( pVItem )
				pVItem->GetLinkItem()->SetLock( FALSE );
			m_VirtualItemStone1.SetLinkItem( NULL );
			m_VirtualItemStone1.SetData(0);
		}
	}

	if(m_pIconDlgStone2)
	{
		if(m_IconStone2)
		{
			m_pIconDlgStone2->DeleteIcon(0,&m_IconStone2);
		}
		if(m_VirtualItemStone2.GetData()!=0)
		{
			m_pIconDlgStone2->DeleteIcon( 0, (cIcon**)&pVItem );
			if( pVItem )
				pVItem->GetLinkItem()->SetLock( FALSE );
			m_VirtualItemStone2.SetLinkItem( NULL );
			m_VirtualItemStone2.SetData(0);
		}
	}

	if(m_pIconDlgStone3)
	{
		if(m_IconStone3)
		{
			m_pIconDlgStone3->DeleteIcon(0,&m_IconStone3);
		}
		if(m_VirtualItemStone3.GetData()!=0)
		{
			m_pIconDlgStone3->DeleteIcon( 0, (cIcon**)&pVItem );
			if( pVItem )
				pVItem->GetLinkItem()->SetLock( FALSE );
			m_VirtualItemStone3.SetLinkItem( NULL );
			m_VirtualItemStone3.SetData(0);
		}
	}
	if(m_pIconDlgStone4)
	{
		if(m_IconStone4)
		{
			m_pIconDlgStone4->DeleteIcon(0,&m_IconStone4);
		}
		if(m_VirtualItemStone4.GetData()!=0)
		{
			m_pIconDlgStone4->DeleteIcon( 0, (cIcon**)&pVItem );
			if( pVItem )
				pVItem->GetLinkItem()->SetLock( FALSE );
			m_VirtualItemStone4.SetLinkItem( NULL );
			m_VirtualItemStone4.SetData(0);
		}
	}
}
void CItemStoneXqDialog::Release()
{

	CVirtualItem* pVItem = NULL;

	if( m_pIconDlgEquip )
	{
		
		m_pIconDlgEquip->DeleteIcon( 0, (cIcon**)&pVItem );

		if( pVItem )
			pVItem->GetLinkItem()->SetLock( FALSE );

		m_VirtualItemEquip.SetLinkItem( NULL );
		m_VirtualItemEquip.SetData(0);
	}

	if(m_pIconDlgStone0)
	{
		if(m_IconStone0)
		{
			m_pIconDlgStone0->DeleteIcon(0,&m_IconStone0);
		}
		if(m_VirtualItemStone0.GetData()!=0)
		{
			m_pIconDlgStone0->DeleteIcon( 0, (cIcon**)&pVItem );
			if( pVItem )
				pVItem->GetLinkItem()->SetLock( FALSE );
			m_VirtualItemStone0.SetLinkItem( NULL );
			m_VirtualItemStone0.SetData(0);
		}
	}
	if(m_pIconDlgStone1)
	{
		if(m_IconStone1)
		{
			m_pIconDlgStone1->DeleteIcon(0,&m_IconStone1);
		}
		if(m_VirtualItemStone1.GetData()!=0)
		{
			m_pIconDlgStone1->DeleteIcon( 0, (cIcon**)&pVItem );
			if( pVItem )
				pVItem->GetLinkItem()->SetLock( FALSE );
			m_VirtualItemStone1.SetLinkItem( NULL );
			m_VirtualItemStone1.SetData(0);
		}
	}

	if(m_pIconDlgStone2)
	{
		if(m_IconStone2)
		{
			m_pIconDlgStone2->DeleteIcon(0,&m_IconStone2);
		}
		if(m_VirtualItemStone2.GetData()!=0)
		{
			m_pIconDlgStone2->DeleteIcon( 0, (cIcon**)&pVItem );
			if( pVItem )
				pVItem->GetLinkItem()->SetLock( FALSE );
			m_VirtualItemStone2.SetLinkItem( NULL );
			m_VirtualItemStone2.SetData(0);
		}
	}

	if(m_pIconDlgStone3)
	{
		if(m_IconStone3)
		{
			m_pIconDlgStone3->DeleteIcon(0,&m_IconStone3);
		}
		if(m_VirtualItemStone3.GetData()!=0)
		{
			m_pIconDlgStone3->DeleteIcon( 0, (cIcon**)&pVItem );
			if( pVItem )
				pVItem->GetLinkItem()->SetLock( FALSE );
			m_VirtualItemStone3.SetLinkItem( NULL );
			m_VirtualItemStone3.SetData(0);
		}
	}
	if(m_pIconDlgStone4)
	{
		if(m_IconStone4)
		{
			m_pIconDlgStone4->DeleteIcon(0,&m_IconStone4);
		}
		if(m_VirtualItemStone4.GetData()!=0)
		{
			m_pIconDlgStone4->DeleteIcon( 0, (cIcon**)&pVItem );
			if( pVItem )
				pVItem->GetLinkItem()->SetLock( FALSE );
			m_VirtualItemStone4.SetLinkItem( NULL );
			m_VirtualItemStone4.SetData(0);
		}
	}

	ITEMMGR->SetDisableDialog(FALSE, eItemTable_Inventory);
	ITEMMGR->SetDisableDialog(FALSE, eItemTable_Pyoguk);
	ITEMMGR->SetDisableDialog(FALSE, eItemTable_GuildWarehouse);
	ITEMMGR->SetDisableDialog(FALSE, eItemTable_Shop);
	ITEMMGR->SetDisableDialog(FALSE, eItemTable_PetInven);
	OBJECTSTATEMGR->EndObjectState( HERO, eObjectState_Deal );
	SetActive( FALSE );

    if(pXqItem)
	{
		pXqItem->SetLock(FALSE);
	}
	
}

int  CItemStoneXqDialog::GetStoneVirtualCount()
{
	int Count=0;

	if(m_VirtualItemStone0.GetLinkItem())
	{
		Count=Count+1;
	}
	if(m_VirtualItemStone1.GetLinkItem())
	{
		Count=Count+1;
	}	
	if(m_VirtualItemStone2.GetLinkItem())
	{
		Count=Count+1;
	}	
	if(m_VirtualItemStone3.GetLinkItem())
	{
		Count=Count+1;
	}	
	if(m_VirtualItemStone4.GetLinkItem())
	{
		Count=Count+1;
	}
	return Count;
	
}

int  CItemStoneXqDialog::GetStoneXqCount(ITEM_STONE_OPTION_INFO * pStoneInfo)
{
	int Count=0;

	if(pStoneInfo)
	{
		if(pStoneInfo->dwItemStone0>0)
		{
			Count=Count+1;
		}
		if(pStoneInfo->dwItemStone1>0)
		{
			Count=Count+1;
		}
		if(pStoneInfo->dwItemStone2>0)
		{
			Count=Count+1;
		}
		if(pStoneInfo->dwItemStone3>0)
		{
			Count=Count+1;
		}
		if(pStoneInfo->dwItemStone4>0)
		{
			Count=Count+1;
		}
	}
	return Count;
}