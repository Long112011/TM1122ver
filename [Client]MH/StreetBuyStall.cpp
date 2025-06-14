







// StreetBuyStall.cpp: implementation of the CStreetBuyStall class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "BuyRegDialog.h"
#include "StallKindSelectDlg.h"
#include "StreetBuyStall.h"
#include "WindowIDEnum.h"
#include "ObjectManager.h"
#include "GameIn.h"
#include "cMsgBox.h"
#include "./Interface/cWindowManager.h"
#include "./Interface/cSpin.h"
#include "./Interface\cIcon.h"
#include "./Interface\cIconGridDialog.h"
#include "./Interface\cEditBox.h"
#include "./Interface\cTextArea.h"
#include "./Interface\cButton.h"
#include "InventoryExDialog.h"

#include "StreetStallManager.h"

#include "item.h"
#include "itemmanager.h"
#include "BuyItem.h"
#include "StreetStallTitleTip.h"
#include "cIMEex.h"
#include "cDivideBox.h"
#include "ChatManager.h"
#include "./Input/Mouse.h"

#include "MainBarDialog.h"
#include "QuickManager.h"
#include "AppearanceManager.h"
#include "cScriptManager.h"                // 2014-12-15 깊헙栗都賈痰庿


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CStreetBuyStall::CStreetBuyStall()
{
	m_type = WT_STREETBUYSTALLDIALOG;
	m_DlgState = eBSDS_NOT_OPENED;

//	m_nUsable = DEFAULT_USABLE_INVENTORY;

	memset(m_MoneyArray, 0, SLOT_STREETBUYSTALL_NUM*sizeof(DWORD));
	memset(m_GoldArray, 0, SLOT_STREETBUYSTALL_NUM*sizeof(DWORD));
	memset(&m_FakeRegInfo, 0, sizeof(BUY_REG_INFO));

	m_nCurSelectedItem = -1;
	m_pData = NULL;
	m_pSellItem = NULL;
}

CStreetBuyStall::~CStreetBuyStall()
{

}

void CStreetBuyStall::Linking()
{
	m_pStallGrid = (cIconGridDialog*)GetWindowForID(BS_ICONGRID);
	m_ImageGrid =(cIconGridDialog*)GetWindowForID(BS_ICONGRID_IMAGE);   // 2014-12-15 깊헙움

	m_pImgBtn   =(cButton *)GetWindowForID(BS_BTN_IMAGE);     // 2015-01-05 깊헙객큐

	m_pTitleEdit = (cEditBox *)GetWindowForID(BS_TITLEEDIT);
	m_pTitleEdit->GetIME()->SetEnterAllow(FALSE);
	m_pTitleEdit->GetIME()->SetLimitLine(2);

	m_pEnterBtn	= (cButton*)GetWindowForID(BS_ENTER);
	m_pMoneyEdit = (cEditBox *)GetWindowForID(BS_MONEYEDIT);
	m_pMoneyEdit->SetValidCheck( VCM_NUMBER );
	m_pMoneyEdit->SetAlign( TXT_RIGHT );
	m_pMoneyEdit->SetReadOnly( TRUE );

	m_pGoldEdit = (cEditBox *)GetWindowForID(BS_GOLDEDIT);
	m_pGoldEdit->SetValidCheck( VCM_NUMBER );
	m_pGoldEdit->SetAlign( TXT_RIGHT );
	m_pGoldEdit->SetReadOnly( TRUE );

	m_pRegistBtn = (cButton *)GetWindowForID(BS_REGBTN);
	m_pSellBtn = (cButton *)GetWindowForID(BS_SELLBTN);
	
	m_pTitleEdit->SetEditText( DEFAULT_TITLE_TEXT );
	strcpy( m_OldTitle, m_pTitleEdit->GetEditText() );
	
	m_pMoneyEdit->SetEditText( DEFAULT_MONEY_TEXT );
	m_pGoldEdit->SetEditText( DEFAULT_MONEY_TEXT);

	LoadFaceList();  // 2015-01-04 깊헙

	m_ImageGrid->SetActive(FALSE);  // 2014-01-05 칵훰밑균깊헙朞嶝움

	m_pTitleEdit->SetRenderFaceImg(TRUE); // 2015-01-04 뚤깊헙斡횿
}

DWORD CStreetBuyStall::ActionEvent(CMouse * mouseInfo)
{
	if( !m_bActive ) return WE_NULL;

	DWORD we = WE_NULL;
	we |= cWindow::ActionEvent(mouseInfo);

	if( !m_bDisable )
	{
		we |= ActionEventWindow(mouseInfo);

		WORD pos;

		m_pStallGrid->GetPositionForXYRef(mouseInfo->GetMouseEventX(), mouseInfo->GetMouseEventY(), pos);
/*
		if(pos >= m_nUsable && pos <= SLOT_STREETSTALL_NUM)
			return we;
*/
		we |= ActionEventComponent(mouseInfo);
	}
	return we;	

}

void CStreetBuyStall::OnCloseStall(BOOL bDelOption)
{
	STREETSTALLMGR->SetStallKind(eSK_NULL);

	SetDisable(FALSE);
	cDialog::SetActive(FALSE);

	m_pMoneyEdit->SetReadOnly( TRUE );
	m_pMoneyEdit->SetFocusEdit( FALSE );

	m_pGoldEdit->SetReadOnly(TRUE);
	m_pGoldEdit->SetFocusEdit(FALSE);

	// 전체 Icon 삭제 
	WORD num = m_pStallGrid->GetCellNum();
	DeleteItemAll(bDelOption);

	// 갿변숿초기홿
	m_pStallGrid->SetCurSelCellPos(-1);
	m_nCurSelectedItem = -1;
	memset(m_MoneyArray, 0, SLOT_STREETBUYSTALL_NUM*sizeof(DWORD));
	memset(m_GoldArray, 0, SLOT_STREETBUYSTALL_NUM*sizeof(DWORD));

	m_pMoneyEdit->SetEditText( DEFAULT_MONEY_TEXT );
	m_pGoldEdit->SetEditText(DEFAULT_MONEY_TEXT);
	m_pTitleEdit->SetEditText( DEFAULT_TITLE_TEXT );

	m_pStallGrid->SetShowGrid( FALSE );
	strcpy( m_OldTitle, m_pTitleEdit->GetEditText() );

	m_DlgState = eBSDS_NOT_OPENED;

	m_dwOwnnerId = HERO->GetID();

	if(m_pData)
	{
		CItem* item = (CItem*)m_pData;
		item->SetLock( FALSE );
	}
	m_pData = NULL;

	cDivideBox * pDivideBox = (cDivideBox *)WINDOWMGR->GetWindowForID( DBOX_DIVIDE_STREETBUYSTALL );
	if( pDivideBox )
	{
		pDivideBox->SetDisable(TRUE);
		pDivideBox->SetActive(FALSE);
		WINDOWMGR->AddListDestroyWindow( pDivideBox );
	}
}

void CStreetBuyStall::ShowBuyStall()
{
	SetActive(TRUE);

	m_DlgState = eBSDS_OPENED;

	m_pTitleEdit->SetReadOnly(FALSE);

	m_pEnterBtn->SetDisable(FALSE);
	m_pRegistBtn->SetDisable(FALSE);
	m_pRegistBtn->SetActive(TRUE);
	m_pSellBtn->SetActive(FALSE);
/*
	//////////////////////////////////////////////////////////////////////////
	// 추가인벤 적용
	SHOPITEMOPTION*	pShopItemOption = HERO->GetShopItemStats();
	m_nUsable = DEFAULT_USABLE_INVENTORY + pShopItemOption->BuyStallInven;
	//////////////////////////////////////////////////////////////////////////
	
	for(int i = m_nUsable; i < SLOT_STREETSTALL_NUM; i++)
	{
		CBuyItem* pItem = new CBuyItem;

		pItem->Init(0, 0, DEFAULT_ICONSIZE, DEFAULT_ICONSIZE, NULL );

		pItem->SetLock(TRUE);
		pItem->SetDisable(TRUE);

		m_pStallGrid->AddIcon(i, pItem);
	}
*/	
	return;
}

void CStreetBuyStall::SetDisable(BOOL val)
{	// 손님으로 참여싿노점창을 사용할수 있게 할때
	// 등록 버튼돿사용핿숿있게 되는것을 막음
	cDialog::SetDisable(val);

	if(m_DlgState == eBSDS_SELL)
	{
		m_pEnterBtn->SetDisable( TRUE );
		
		m_pRegistBtn->SetActive(FALSE);
	}
	else if(m_DlgState == eBSDS_OPENED)

	{
		m_pSellBtn->SetActive(FALSE);
	}
}

void CStreetBuyStall::ShowSellStall()
{
	SetActive(TRUE);

	m_DlgState = eBSDS_SELL;

	m_pTitleEdit->SetReadOnly(TRUE);
	m_pEnterBtn->SetDisable( TRUE );
//	m_pRegistBtn->SetDisable(TRUE);
	m_pRegistBtn->SetActive(FALSE);
	m_pSellBtn->SetActive(TRUE);

	for(int i = 0; i < SLOT_STREETBUYSTALL_NUM; i++)
	{
		cIcon* pIcon = m_pStallGrid->GetIconForIdx( i );

		if( !pIcon )
			continue;

		if( pIcon->IsLocked() )
			pIcon->SetActive( FALSE );
	}

	ResetDlgData();
	return;
}

BOOL CStreetBuyStall::AddItem( POSTYPE pos, cIcon* pItem )
{
//	if( pos >= m_nUsable) return FALSE;

//	m_nCurSelectedItem = pos;
	
	if( pos == m_pStallGrid->GetCellNum() ) return FALSE;

	//덥어 씌우긿위해 기존 아이템을 삭제해야 하는덿
	//클라이언트에 저장된 금액 정보와 등록정보륿백업읿떿두어앿한다
	BUY_REG_INFO temp;
	DWORD money = m_MoneyArray[pos];
	DWORD gold  = m_GoldArray[pos];
	memcpy( &temp, &m_RegInfoArray[pos], sizeof(BUY_REG_INFO) );
	
	DeleteItem(pos);

	m_MoneyArray[pos] = money;
	m_GoldArray[pos]= gold;
	memcpy( &m_RegInfoArray[pos], &temp, sizeof(BUY_REG_INFO) );

	return m_pStallGrid->AddIcon( pos, pItem );
}

void CStreetBuyStall::DeleteItem( POSTYPE pos )
{
//	if( pos >= m_nUsable) return;
	
//	CBuyItem* pItem;
	
	if( pos == m_pStallGrid->GetCellNum() ) return;

	cIcon* pIcon = NULL;
	if( m_pStallGrid->DeleteIcon( pos, &pIcon ) )
	{
		if( m_DlgState == eBSDS_SELL )
		{
			if( pIcon ) WINDOWMGR->AddListDestroyWindow( pIcon );
		}
		else
		{
			if( pIcon ) STREETSTALLMGR->UnlinkItem( (CBuyItem*)pIcon );
		}

		m_pStallGrid->DeleteIcon( pos, &pIcon );
	
		m_MoneyArray[pos] = 0;
		m_GoldArray[pos]=0;
		memset( &m_RegInfoArray[pos], 0, sizeof(BUY_REG_INFO) );
	}

	ResetDlgData();
}

void CStreetBuyStall::ResetItemInfo( POSTYPE pos, WORD volume )
{
//	if( pos >= m_nUsable) return;
	
	CBuyItem* pItem = (CBuyItem*)m_pStallGrid->m_pIconGridCell[pos].icon;

	if( !pItem )	return;
	if( pos == m_pStallGrid->GetCellNum() ) return;
	
	pItem->SetVolume( volume );
}

void CStreetBuyStall::DeleteItemAll(BOOL bDelOption)
{
	cIcon* pIcon;

	for( int i=0;i<SLOT_STREETBUYSTALL_NUM;++i)
	{
		m_pStallGrid->DeleteIcon( i, &pIcon );
		
		if( m_DlgState == eBSDS_SELL )
		{
			if( pIcon ) WINDOWMGR->AddListDestroyWindow( pIcon );
		}
		else
		{
			if( pIcon ) STREETSTALLMGR->UnlinkItem( (CBuyItem*)pIcon );
		}
	}
}

void CStreetBuyStall::FakeRegistItem( BUY_REG_INFO RegInfo, ITEM_INFO* pItemInfo)
{
	ITEMBASE Item;

	memset(&Item, 0, sizeof(ITEMBASE));

	Item.wIconIdx = pItemInfo->ItemIdx;
	
	if(m_nCurSelectedItem == -1)
		m_nCurSelectedItem = 0;

	ITEM_INFO * pInfo = ITEMMGR->GetItemInfo(Item.wIconIdx);
	
	if(!pInfo)
		return;
	//////////////////////////////////////////////////////////////////////////
	// 등록하려늿아이템이 변혿꾸미긿아이템일경우 봉인됿것을 등록한다
//	if( pInfo->ItemKind == eSHOP_ITEM_MAKEUP || 
//		pInfo->ItemKind == eSHOP_ITEM_DECORATION )
	if( pInfo->ItemKind <= eSHOP_ITEM_EQUIP )
	{
		Item.ItemParam = ITEM_PARAM_SEAL;
	}
	
	memcpy( &m_FakeRegInfo, &RegInfo, sizeof(BUY_REG_INFO) );
	STREETSTALLMGR->RegistItemEx(Item, m_nCurSelectedItem, RegInfo.Volume, RegInfo.Money,RegInfo.Gold);
	SetDisable( TRUE );

}

BOOL CStreetBuyStall::SellError(WORD errCode)

{
	CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(errCode) );
	
	return FALSE;
}

//////////////////////////////////////////////////////////////////////////
// 팔기 버튼곿더블클릭으로 판매 방식 추가롿
// 기존 드래귿방식 제거
BOOL CStreetBuyStall::FakeMoveIcon(LONG x, LONG y, cIcon * icon)
{
	return FALSE;
}

/*	Delete...
BOOL CStreetBuyStall::FakeMoveIcon(LONG x, LONG y, cIcon * icon)
{
	// 노점 상태 체크
	if( GetDlgState() != eBSDS_SELL ) return FALSE;
	if( m_bDisable == TRUE || m_pData ) return SellError();

	////////////////////////////////////////////////////////////////////////////////////////////
	//// 아이템에대핿유효섿체크
	if( icon->GetType() != WT_ITEM )
		return SellError();
	
	// changeitem 중에 거래안되는거
	if( ((CItem*)icon)->GetItemKind() == eCHANGE_ITEM_LOCK )
		return SellError();
	
	// 인벤토리 아이템인지 체크한다.
	if( ( GetTableIdxPosition( ((CItem*)icon)->GetPosition() ) != eItemTable_Inventory ) &&
		( GetTableIdxPosition( ((CItem*)icon)->GetPosition() ) != eItemTable_ShopInven ) )
	{
		if( GetTableIdxPosition( ((CItem*)icon)->GetPosition() ) == eItemTable_Weared )
			return SellError(eSE_WEARED);
	}


	if( ((CItem*)icon)->GetItemKind() & eSHOP_ITEM )
	{
		// 기한젿사용중인 아이텿체크
		if( ((CItem*)icon)->GetItemInfo()->ItemType == 11 )
		{
			if( !(((CItem*)icon)->GetItemParam() & ITEM_PARAM_SEAL) )
			{
				return SellError(eSE_NOT_ALLOW);
			}
		}
		// 거래 불가 아이텿
		if( ((CItem*)icon)->GetItemInfo()->MeleeAttackMax )
		{
			return SellError(eSE_NOT_ALLOW);
		}
	}
	
	// 퀵다이얼로그엿링크가 있는지 체크한다.
	if( ((CItem*)icon)->GetItemKind() == eYOUNGYAK_ITEM ||
		((CItem*)icon)->GetItemKind() == eSHOP_ITEM_INCANTATION ||

		((CItem*)icon)->GetItemKind() == eSHOP_ITEM_HERB ||
		((CItem*)icon)->GetItemKind() == eSHOP_ITEM_CHARM ||
		((CItem*)icon)->GetItemKind() == eSHOP_ITEM_SUNDRIES )
	{
		if( QUICKMGR->CheckQPosForItemIdx(((CItem*)icon)->GetItemIdx()) == FALSE )
		{
			return SellError(eSE_QUICK);
		}
	}
	else
	{
		if( ((CItem*)icon)->GetQuickPosition() != 0 )
		{
			return SellError(eSE_QUICK);
		}
	}

	CItem* pItem = (CItem*)icon;
	
	//레어아이텿판매 불가
	if( ITEMMGR->IsRareOptionItem(pItem->GetItemIdx(), pItem->GetRareness()) )
		return SellError(eSE_RARE);

	//강화 아이템도 판매 불가!
	if( ITEMMGR->IsOptionItem(pItem->GetItemIdx(), pItem->GetDurability()) )
		return SellError(eSE_OPTION);
	
	// 링크아이템인지 확인한다.
	if( icon->GetIconType() == eIconType_LinkedItem ) return SellError();
	if( pItem->GetLinkPosition() ) return SellError(); // 아이템의 링크상태 확인

	CBuyItem* pBuyItem = NULL;
		
	if( m_pStallGrid->PtInWindow( x, y ) ) 
	{
		WORD pos;

		m_pStallGrid->GetPositionForXYRef(x, y, pos);

		if(pos >= 0 && pos < SLOT_STREETBUYSTALL_NUM)
			pBuyItem = (CBuyItem*)GetItem(pos);

		 // 드래귿위치읿아이템과 팔려늿아이템이 일치하지 않으멿
		 // 선택됿아이템이 없는것이닿
		if(pBuyItem)
			if( pBuyItem->GetItemIdx() != pItem->GetItemIdx() )
				 pBuyItem = NULL;
			else
				m_nCurSelectedItem = pos;
	}
	

	// 선택됿아이템이 없을 경우 앞에섿부턿검색한닿
	if(!pBuyItem)
	{
		for(int n = 0; n < SLOT_STREETBUYSTALL_NUM; n++)
		{
			pBuyItem = (CBuyItem*)GetItem(n);
			if( !pBuyItem ) continue;
			
			if( !pBuyItem->IsActive() ) continue;

			if( pBuyItem->IsLocked() ) continue;
			
			if( pBuyItem->GetItemIdx() == pItem->GetItemIdx() )
			{
				m_nCurSelectedItem = n;		
				break;
			}
		}

		if(n == SLOT_STREETBUYSTALL_NUM)
		{
			return SellError(eSE_NOT_EXIST);
		}
	}
	
	//	선택됿아이템을 가져와섿다시 검사한닿
	pBuyItem = (CBuyItem*)GetCurSelectedItem();
	
	if( !pBuyItem->IsActive() ) 
		return SellError(eSE_NOT_EXIST);
	
	if( pBuyItem->IsLocked() )
		return SellError();
	
	pItem->SetLock( TRUE );

	m_pSellItem = pItem;

	if( ITEMMGR->IsDupItem((WORD)icon->GetData()) )
	{
		if( pItem->GetDurability() == 1)
			pItem->SetLock( STREETSTALLMGR->SellItem() );
		else
			pItem->SetLock( ShowDivideBox((WORD)pItem->GetDurability()) );
	}
	else
	{
		pItem->SetLock( STREETSTALLMGR->SellItem() );
	}
	
	return FALSE;
}
*/
BOOL CStreetBuyStall::ShowDivideBox(WORD Volume)
{
	CBuyItem* pBuyItem = (CBuyItem*)m_pStallGrid->GetIconForIdx( m_nCurSelectedItem );
	
	if( pBuyItem == NULL ) return FALSE;
	
	cDivideBox * pDivideBox = WINDOWMGR->DivideBox( DBOX_DIVIDE_STREETBUYSTALL, (LONG)pBuyItem->GetAbsX(), (LONG)pBuyItem->GetAbsY(), OnDivideItem, OnDivideItemCancel, this, NULL, CHATMGR->GetChatMsg(186) );
	if( !pDivideBox )
	{
		pDivideBox = (cDivideBox *)WINDOWMGR->GetWindowForID( DBOX_DIVIDE_STREETBUYSTALL );
	}
	pDivideBox->ChangeKind( 0 );
	
	pDivideBox->SetMaxValue( Volume );

	pDivideBox->SetValue(0);	
	
	m_pData = pBuyItem;
	
	pDivideBox->m_pSpin->SetFocusEdit(TRUE);

	return TRUE;
}

void CStreetBuyStall::OnDivideItem( LONG iId, void* p, DWORD param1, void * vData1, void * vData2 )
{
	CStreetBuyStall* pStall = (CStreetBuyStall*)vData1;
	CBuyItem* pBuyItem = (CBuyItem*)pStall->GetCurSelectedItem();
	
	if(!pStall || !pStall->IsActive() || !pBuyItem || !pBuyItem->IsActive() || param1 == 0)
	{
		pStall->SellError();
				
		OnDivideItemCancel(iId, p, param1, vData1, vData2);
		return; 
	}
	
	cDivideBox* pDivideBox = (cDivideBox*)p;

	if( pDivideBox == NULL )
	{
		pStall->SetDisable(FALSE);
		return;
	}

	if( !STREETSTALLMGR->SellItemDur( param1 ) )
	{		
		pStall->SellError();
		
		CItem* pItem = pStall->GetSellItem();
		if(pItem)	pItem->SetLock(FALSE);
	}
}

void CStreetBuyStall::OnDivideItemCancel( LONG iId, void* p, DWORD param1, void * vData1, void * vData2 )
{
	CStreetBuyStall* pStall = (CStreetBuyStall*)vData1;
	if( pStall == NULL ) return;

	pStall->SetData( NULL );
	pStall->SetDisable( FALSE );

	CItem* pItem = pStall->GetSellItem();
	if(pItem)	pItem->SetLock(FALSE);
}

void CStreetBuyStall::FakeDeleteItem( POSTYPE pos )
{
	CBuyItem* pBuyItem = (CBuyItem*)m_pStallGrid->GetIconForIdx( pos );

	if( pBuyItem == NULL ) return;

	MSG_LINKITEM msg;
	msg.Category	= MP_STREETSTALL;
	msg.Protocol	= MP_STREETSTALL_DELETEITEM_SYN;
	msg.dwObjectID	= HERO->GetID();
	

	memset(&msg.ItemInfo, 0, sizeof(msg.ItemInfo));

	msg.ItemInfo.wIconIdx = pBuyItem->GetItemIdx();
	msg.wAbsPosition = pos;

	NETWORK->Send( &msg, sizeof( MSG_LINKITEM ) );
}

DWORD CStreetBuyStall::ActionEventWindow(CMouse * mouseInfo)
{
	DWORD we = WE_NULL;

	if( m_bDisable ) return we;
	we = cDialog::ActionEventWindow( mouseInfo );

	if( mouseInfo->LButtonDown() )
	if( !PtInWindow( mouseInfo->GetMouseX(), mouseInfo->GetMouseY() ) )

	{
		m_pStallGrid->SetShowGrid( FALSE );
	}
	else
	{
	}

	return we;
}

void CStreetBuyStall::OnRegBtn()
{
	//손님으로 들어갿경우늿리턴
	if(m_DlgState == eBSDS_SELL)
		return;

	
	// 선택됿pos륿가져온닿
	POSTYPE pos = (POSTYPE)m_pStallGrid->GetCurSelCellPos();
	
	//선택됿pos가 없어섿쓰레긿값이 넘어왔을 경우 첫번짿셀읿선택해준닿
	if(pos < 0 || pos >= SLOT_STREETBUYSTALL_NUM)
		pos = 0;
	
	m_nCurSelectedItem = pos;
	
	CBuyItem * pItem = (CBuyItem*)m_pStallGrid->GetIconForIdx( pos );
	
	if( !pItem ) // 선택됿곳에 아이템이 없는경우
	{
		GAMEIN->GetBuyRegDialog()->Show(); // 등록창을 연다
		return;
	}
	else
	{
     // 서버엿업데이트 요청
		MSG_WORD msg;
		msg.Category = MP_STREETSTALL;
		msg.dwObjectID = HEROID;

		msg.Protocol = MP_STREETSTALL_UPDATE_SYN;
		msg.wData = pos;
		
		NETWORK->Send( &msg, sizeof(MSG_WORD) );
		
		return;
	}
}

BOOL CStreetBuyStall::OnSellBtn()
{
	//주인으로 들어갿경우늿리턴
	if(m_DlgState == eBSDS_OPENED)
		return FALSE;
	
	//선택됿위치륿가져온닿
	POSTYPE pos = (POSTYPE)m_pStallGrid->GetCurSelCellPos();
	
	if(pos < 0 || pos >= SLOT_STREETBUYSTALL_NUM)
		return SellError(eSE_NOT_SELECT);	// 선택된게 없다!

	m_nCurSelectedItem = pos;
	
	//선택됿위치엿등록됿구매정보륿가져온닿
	CBuyItem * pBuyItem = (CBuyItem*)m_pStallGrid->GetIconForIdx( pos );

	if(( !pBuyItem ) || ( !pBuyItem->IsActive() ))
		return SellError(eSE_NOT_EXIST);	// 해당 아이템이 없다!

	if( pBuyItem->IsLocked() )
		return SellError();	// 거래불가 상태닿


	//인벤토리륿검색한닿

	CItem* pItem = GAMEIN->GetInventoryDialog()->GetItemLike( pBuyItem->GetItemIdx() );
	
	if( !pItem )
		return SellError(eSE_NOT_HAVE);	// 가지곿있는 아이템이 아니닿

	if( pItem->GetItemKind() & eSHOP_ITEM )
	{
		// 기한젿사용중인 아이텿체크
		if( pItem->GetItemInfo()->ItemType == 11 )
		{
			if( !(pItem->GetItemParam() & ITEM_PARAM_SEAL) )
			{
				return SellError(eSE_NOT_ALLOW);
			}
		}
		// 거래 불가 아이텿
		if( pItem->GetItemKind() == eSHOP_ITEM_EQUIP || pItem->GetItemKind() == eSHOP_ITEM_PET_EQUIP )
		{
			if( pItem->GetItemInfo()->BuyPrice )
				return SellError(eSE_NOT_ALLOW);
		}
		else if( pItem->GetItemInfo()->MeleeAttackMax )
			return SellError(eSE_NOT_ALLOW);
	}
	else
	{
		//레어아이텿판매 불가
		if( ITEMMGR->IsRareOptionItem(pItem->GetItemIdx(), pItem->GetRareness()) )
			return SellError(eSE_RARE);

		//강화 아이템도 판매 불가!
		if( ITEMMGR->IsOptionItem(pItem->GetItemIdx(), pItem->GetDurability()) )
			return SellError(eSE_OPTION);
	}

	// 링크아이템인지 확인한다.
	if( pItem->GetIconType() == eIconType_LinkedItem ) return SellError();
	if( pItem->GetLinkPosition() ) return SellError(); // 아이템의 링크상태 확인

	// 퀵다이얼로그엿링크가 있는지 체크한다.
	if( pItem->GetItemKind() == eYOUNGYAK_ITEM ||
		pItem->GetItemKind() == eSHOP_ITEM_INCANTATION ||
		pItem->GetItemKind() == eSHOP_ITEM_HERB ||
		pItem->GetItemKind() == eSHOP_ITEM_CHARM ||
		pItem->GetItemKind()== eSHOP_ITEM_MARRYCHARM||
		pItem->GetItemKind() == eSHOP_ITEM_SUNDRIES )
	{
		if( QUICKMGR->CheckQPosForItemIdx(pItem->GetItemIdx()) == FALSE )
		{
			return SellError(eSE_QUICK);
		}
	}
	else
	{
		if( pItem->GetQuickPosition() != 0 )
		{
			return SellError(eSE_QUICK);
		}
	}

	if( pItem->IsLocked() ) return SellError();

	pItem->SetLock( TRUE );
	
	m_pSellItem = pItem;
	
	if( ITEMMGR->IsDupItem( pItem->GetItemIdx() ) )
	{
		if( pItem->GetDurability() == 1)
			pItem->SetLock( STREETSTALLMGR->SellItem() );
		else
			pItem->SetLock( ShowDivideBox( (WORD)pItem->GetDurability() ) );
	}
	else
	{
		pItem->SetLock( STREETSTALLMGR->SellItem() );
	}

	return TRUE;
}

void CStreetBuyStall::OnActionEvnet(LONG lId, void * p, DWORD we)
{
	// 셀 선택
	if( we == WE_LBTNCLICK )
	{
		if(lId== BS_ICONGRID_IMAGE)  // 2014-01-05 듐샌깊헙객큐뇰랙
		{
			m_ImageGrid->SetMovable(FALSE);
			int Pos=m_ImageGrid->GetCurSelCellPos();

			if(Pos!=0)
			{
				char ImgCode[10];

				sprintf(ImgCode,"{{%02d",Pos);

				m_pTitleEdit->SetEditText(strcat(m_pTitleEdit->GetEditText(),ImgCode));
			}
			return;
		}

		POSTYPE pos = (POSTYPE)m_pStallGrid->GetCurSelCellPos();
		
		m_nCurSelectedItem = pos;
	}
	// 주인일때 더블클릭이거낿등록버튼 클릭싿
	else if( ( ( we == WE_LBTNDBLCLICK ) && (m_DlgState == eBSDS_OPENED) )
			|| ( we & WE_BTNCLICK && lId == BS_REGBTN ) )
		OnRegBtn();

	
	// 손님일때 더블클릭이거낿판매버튼 클릭싿
	else if( ( ( we == WE_LBTNDBLCLICK ) && (m_DlgState == eBSDS_SELL) )
			|| ( we & WE_BTNCLICK && lId == BS_SELLBTN ) )
		OnSellBtn();
	
	// 노점몿변겿
	else if( we & WE_BTNCLICK && lId == BS_ENTER )
		STREETSTALLMGR->EditTitle();
	
	// 노점 닫기
	else if( we & WE_BTNCLICK && lId == BS_CLOSEBTN )
	{
		GAMEIN->GetBuyRegDialog()->Close();
		SetActive( FALSE );	
	}
	else if( we & WE_BTNCLICK && lId == BS_BTN_IMAGE ) // 2015-01-01 겠覡櫓왠齡깊헙충겿
	{
		m_ImageGrid->SetActive(!m_ImageGrid->IsActive());
		/*if(m_ImageGrid->IsActive())
		{
			m_ImageGrid->SetActive(FALSE);
		}
		else
		{
			m_ImageGrid->SetActive(TRUE);
		}*/
	}
}

void CStreetBuyStall::SetActive( BOOL val )
{
	if( !m_bDisable )
	if( m_bActive != val )
	{
		if(!val) STREETSTALLMGR->CloseStreetStall();

		cDialog::SetActiveRecursive( val );

		if(val)
		{
			m_ImageGrid->SetActive(FALSE); //CoNaN Ĭȏ¿ªʼ°ڌ¯ʱ£¬¹رձ퇩ѡԱæ°堲018-06-25
		}
	}
}

void CStreetBuyStall::RegistMoney()
{
	m_MoneyArray[m_nCurSelectedItem] = (MONEYTYPE)_atoi64(RemoveComma(m_pMoneyEdit->GetEditText()));//m_MoneyArray[m_nCurSelectedItem] = (DWORD)atoi( RemoveComma( m_pMoneyEdit->GetEditText() ));
}

void CStreetBuyStall::RegistMoney( POSTYPE pos, DWORD dwMoney )
{
	m_MoneyArray[pos] = dwMoney;
}
void CStreetBuyStall::RegistGold()
{
	m_GoldArray[m_nCurSelectedItem] = (DWORD)atoi( RemoveComma( m_pGoldEdit->GetEditText() ));
}
void CStreetBuyStall::RegistGold( POSTYPE pos, DWORD dwGold )
{
	m_GoldArray[pos] = dwGold;
}
void CStreetBuyStall::RegistTitle( char* title, BOOL bSucess )
{
	if( bSucess )
	{
		HERO->ShowStreetBuyStallTitle( TRUE, title );
		strcpy( m_OldTitle, title );
	}
	else
	{
		m_pTitleEdit->SetEditText( m_OldTitle );
	}
}

void CStreetBuyStall::OnMoneyEditClick()
{
}

void CStreetBuyStall::OnTitleEditClick()
{
}

void CStreetBuyStall::ChangeItemStatus( POSTYPE pos, WORD volume, DWORD money,DWORD gold)
{
	CBuyItem * pItem = (CBuyItem*)m_pStallGrid->GetIconForIdx( pos );
	if( !pItem ) return;
	char buf[128] = {0,};
	char buf2[128] = {0,};
	char buf3[128] = {0,};
	char buf4[128] = {0,};
	m_MoneyArray[pos] = money;
	m_GoldArray[pos]=gold;
	memcpy(&m_RegInfoArray[pos], &m_FakeRegInfo, sizeof(BUY_REG_INFO));

	pItem->SetMovable( FALSE );

	if(pItem->GetVolume() > 1)
	{
		wsprintf( buf, CHATMGR->GetChatMsg(199), AddComma( pItem->GetMoney() ) );
		wsprintf( buf3, CHATMGR->GetChatMsg(1896), AddComma( pItem->GetGold()) );
		wsprintf( buf2, CHATMGR->GetChatMsg(1206), AddComma( pItem->GetMoney() * pItem->GetVolume() ));
		wsprintf( buf4, CHATMGR->GetChatMsg(1897), AddComma(pItem->GetGold() * pItem->GetVolume()));
	}
	else
	{
		wsprintf( buf, CHATMGR->GetChatMsg(214), AddComma( pItem->GetMoney() ) );	
		wsprintf( buf3, CHATMGR->GetChatMsg(1898), AddComma( pItem->GetGold() ));  
	}
	
	pItem->AddToolTipLine( buf, TTTC_BUYPRICE );
	pItem->AddToolTipLine( buf3, TTTC_BUYPRICE );
	
	if(buf2[0])
		pItem->AddToolTipLine( buf2, TTTC_DEFAULT );
	if(buf4[0])
		pItem->AddToolTipLine( buf4, TTTC_BUYPRICE );

	pItem->AddToolTipLine( "" );
	// 가격표엿대핿처리 
	ResetDlgData();
}

BOOL CStreetBuyStall::SelectedItemCheck()
{
	m_pStallGrid->SetShowGrid( TRUE );	

	CBuyItem * pLinkItem = (CBuyItem*)m_pStallGrid->GetIconForIdx( m_nCurSelectedItem );

	if( !pLinkItem ) return FALSE;


	return !pLinkItem->IsLocked();
}

void CStreetBuyStall::ResetDlgData()
{
	m_pStallGrid->SetCurSelCellPos( m_nCurSelectedItem );
	m_pStallGrid->SetShowGrid( TRUE );
	
	m_TotalMoney = 0;
	m_TotalGold  = 0;
	
	for(int i=0;i<m_pStallGrid->GetCellNum();++i)
	{
		CBuyItem* pItem = (CBuyItem*)m_pStallGrid->m_pIconGridCell[i].icon;
		DWORD Volume;
		
		if(pItem == NULL)
			continue;
		
		if( !pItem->IsActive() )
			continue;

		Volume = pItem->GetVolume();
		
		if(Volume == 0)
			Volume = 1;

		if(m_RegInfoArray[i].Volume != Volume)
		{	// 기존 등록 정보읿수량읿바뀌었다면
			// 수량 정보와 툴팁읿바꿔주어앿한다

			m_RegInfoArray[i].Volume = (WORD)Volume;

			char buf[128] = {0,};
			char buf2[128] = {0,};
			char buf3[128] = {0,};
			char buf4[128] = {0,};
			ITEMMGR->SetToolTipIcon(pItem);
			
			if(pItem->GetVolume() > 1)
			{
				wsprintf( buf, CHATMGR->GetChatMsg(199), AddComma( pItem->GetMoney() ) );
				wsprintf( buf3, CHATMGR->GetChatMsg(1896), AddComma( pItem->GetGold()) );
				wsprintf( buf2, CHATMGR->GetChatMsg(1206), AddComma( pItem->GetMoney() * pItem->GetVolume() ));
				wsprintf( buf4, CHATMGR->GetChatMsg(1897), AddComma(pItem->GetGold() * pItem->GetVolume()));
			}
			else
			{
				wsprintf( buf, CHATMGR->GetChatMsg(214), AddComma( pItem->GetMoney() ) );	
				wsprintf( buf3, CHATMGR->GetChatMsg(1898), AddComma( pItem->GetGold() ));  
			}
			
			pItem->AddToolTipLine( buf, TTTC_BUYPRICE );
			pItem->AddToolTipLine( buf3, TTTC_BUYPRICE );
			
			if(buf2[0])
				pItem->AddToolTipLine( buf2, TTTC_DEFAULT );
			if(buf4[0])
				pItem->AddToolTipLine( buf4, TTTC_BUYPRICE );    

			pItem->AddToolTipLine( "" );
		}
		m_TotalMoney += (m_MoneyArray[i] * Volume);
		m_TotalGold  += (m_GoldArray[i] * Volume);
	}
	
	m_pMoneyEdit->SetEditText( AddComma( m_TotalMoney ) );
	m_pGoldEdit->SetEditText(AddComma( m_TotalGold ));
}

void CStreetBuyStall::GetTitle( char* pStrTitle )
{
	strcpy(pStrTitle, m_pTitleEdit->GetEditText());
}

void CStreetBuyStall::EditTitle( char* title )
{
	m_pTitleEdit->SetEditText( title );
}

cIcon* CStreetBuyStall::GetCurSelectedItem() 
{ 
	if(m_nCurSelectedItem == -1) return NULL; 
	return m_pStallGrid->GetIconForIdx( m_nCurSelectedItem ); 
}


DWORD CStreetBuyStall::GetCurSelectedItemIdx() 
{ 
	if(m_nCurSelectedItem == -1) return 0; 
	return m_pStallGrid->GetIconForIdx(m_nCurSelectedItem)->GetData(); 
}

cIcon* CStreetBuyStall::GetItem( POSTYPE pos )

{
	return m_pStallGrid->GetIconForIdx( pos );
}

// 2014-01-04  깊헙!

void CStreetBuyStall::LoadFaceList()
{
	for(int i=0;i<CHARFACELIST; i++)
	{
		//SCRIPTMGR->GetImage(i+NEWFACELISTSTARTIDX , &m_ImageFace[i], PFT_HARDPATH);
		if(SCRIPTMGR->GetEmoji(i))
		{
			SCRIPTMGR->GetImage(SCRIPTMGR->GetEmoji(i)->ImageIdx,&m_ImageFace[i],&SCRIPTMGR->GetEmoji(i)->rect[0]);
		}
		cIcon * pIcon;
		pIcon=new cIcon;
		pIcon->Init(0,0,16,16,&m_ImageFace[i] ,IG_DEALITEM_START+7000+i);				
		pIcon->SetData(i+FACELISTSTARTIDX);
		pIcon->SetDisable(TRUE);
		pIcon->SetDepend(FALSE);  
		pIcon->SetMovable(FALSE);
		m_ImageGrid->SetMovable(FALSE);
		m_ImageGrid->AddIcon(i,pIcon);
	}
	m_ImageGrid->SetActive(FALSE);

	/*for(int i=0;i<CHARFACELIST; i++)
	{
		SCRIPTMGR->GetImage(i+FACELISTSTARTIDX , &m_ImageFace[i], PFT_HARDPATH);

		cIcon * pIcon;

		pIcon=new cIcon;

		pIcon->Init(0,0,16,16,&m_ImageFace[i] ,IG_DEALITEM_START+7000+i);
				
		pIcon->SetData(i+FACELISTSTARTIDX);

		pIcon->SetDisable(TRUE);

		pIcon->SetDepend(FALSE);
        
		m_ImageGrid->AddIcon(i,pIcon);
	}

	m_ImageGrid->SetActive(FALSE);*/
}
