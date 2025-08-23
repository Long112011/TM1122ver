







// ExchangeDialog.cpp: implementation of the CExchangeDialog class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ExchangeDialog.h"
#include "WindowIDEnum.h"
#include "./Interface/cWindowManager.h"
#include "./Interface/cScriptManager.h"
#include "./Interface/cIconGridDialog.h"
#include "./Interface/cPushupButton.h"
#include "./Interface/cEditBox.h"
#include "./Interface/cStatic.h"

#include "ExchangeManager.h"
#include "ChatManager.h"
#include "ObjectManager.h"
#include "GameIn.h"
#include "ExchangeItem.h"
#include "Item.h"

#include "InventoryExDialog.h"
#include "./Input/UserInput.h"

#include "MainBarDialog.h"
#include "ItemManager.h"
#include "QuickManager.h"
#include "PetManager.h"
#include "TitanManager.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CExchangeDialog::CExchangeDialog()
{
	m_bInit				= FALSE;
	m_pMyIconGridDlg	= NULL;
	m_pOpIconGridDlg	= NULL;
	m_pMyMoneyEditBox	= NULL;
	m_pOpMoneyEditBox	= NULL;
	m_pMyGoldEditBox	= NULL;   // 2014-10-29 ±‡º≠øÚ
	m_pOpGoldEditBox	= NULL;   // 2014-10-29 ±‡º≠øÚ
	m_pLockBtn			= NULL;
	m_pExchangeBtn		= NULL;
	m_pInputMoneyBtn	= NULL;
	m_pInputGoldBtn		= NULL;   // 2014-10-29  ‰»Î±‡º≠øÚ

	m_pMyNameStc		= NULL;
	m_pOpNameStc		= NULL;


	m_bMyLock			= FALSE;
	m_bOpLock			= FALSE;

	m_dwMoney			= 0;
	m_dwOpMoney			= 0;
	
	m_dwGold			=0;      // 2014-10-29 
	m_dwOpGold			=0;      // 2014-10-29 

	m_bExchangable		= TRUE;

	m_nMyItemNum		= 0;
	m_nOpItemNum		= 0;	
}

CExchangeDialog::~CExchangeDialog()
{

}

void CExchangeDialog::Linking()
{
	m_pMyIconGridDlg	= (cIconGridDialog*)GetWindowForID( XCI_ICD_MINE );
	m_pOpIconGridDlg	= (cIconGridDialog*)GetWindowForID( XCI_ICD_OPPOSITE );
	m_pMyMoneyEditBox	= (cEditBox*)GetWindowForID( XCI_EB_MYMONEY );
	m_pMyMoneyEditBox->SetReadOnly( TRUE );				//Ω∫≈©∏≥∆Æ∑Œ
	m_pMyMoneyEditBox->SetAlign( TXT_RIGHT );
//	m_pMyMoneyEditBox->SetValidCheck( VCM_NUMBER );
	m_pOpMoneyEditBox	= (cEditBox*)GetWindowForID( XCI_EB_OPMONEY );
	m_pMyMoneyEditBox->SetReadOnly( TRUE );
	m_pOpMoneyEditBox->SetAlign( TXT_RIGHT );

	// 2014-10-29 

	m_pMyGoldEditBox    =(cEditBox*)GetWindowForID( XCI_EB_MYGOLD );

	m_pMyGoldEditBox->SetReadOnly(TRUE);

	m_pMyGoldEditBox->SetAlign(TXT_RIGHT);

	m_pOpGoldEditBox    =(cEditBox*)GetWindowForID( XCI_EB_OPGOLD );

	m_pOpGoldEditBox->SetReadOnly(TRUE);

	m_pOpGoldEditBox->SetAlign(TXT_RIGHT);

	m_pLockBtn			= (cPushupButton*)GetWindowForID( XCI_BTN_LOCK );
	m_pExchangeBtn		= (cPushupButton*)GetWindowForID( XCI_BTN_EXCHANGE );
	m_pInputMoneyBtn	= (cButton*)GetWindowForID( XCI_BTN_INPUTMONEY );
	m_pInputGoldBtn 	= (cButton*)GetWindowForID(  XCI_BTN_INPUTGOLD ); // 2014-10-29 ∞¥≈•
	
	m_pMyNameStc		= (cStatic*)GetWindowForID( XCI_INFO2 );
	m_pOpNameStc		= (cStatic*)GetWindowForID( XCI_INFO1 );
	//m_pLockImage		= WINDOWMGR->GetImageOfFile( "./image/GameIn/28/120210.tif", 28 );
	SCRIPTMGR->GetImage( 65, &m_LockImage, PFT_HARDPATH );

	m_bInit				= TRUE;
}
/*
DWORD CExchangeDialog::ActionEvent( CMouse* mouseInfo, CKeyboard* keyInfo )
{
	DWORD we = WE_NULL;
	if( !m_bActive ) return we;

//	LONG x = mouseInfo->GetClickDownX();
//	LONG y = mouseInfo->GetClickDownY();

//	if( mouseInfo->LButtonClick() )
//	{
//		we = m_pInputMoneyBtn->ActionEvent( mouseInfo, keyInfo );
//		if( !( we & WE_LBTNCLICK ) )
//		{
//			we = m_pMyMoneyEditBox->ActionEvent( mouseInfo, keyInfo );
//			if( !( we & WE_LBTNCLICK ) )
//				InputMoney( 0, m_dwMoney );		//√ﬂ∞° : ∫Ø∞Ê¿Ã æ»µ«æ˙¿∏∏È «“« ø‰ æ¯¿Ω
//		}
//	}
	
	return cDialog::ActionEvent(mouseInfo, keyInfo);
}
*/
void CExchangeDialog::OnActionEvent(LONG lId, void * p, DWORD we)
{
	if( we & WE_BTNCLICK )
	{
		if( lId == XCI_BTN_EXIT || lId == XCI_BTN_CLOSE )

		{
			MSGBASE msg;
			msg.Category	= MP_EXCHANGE;
			msg.Protocol	= MP_EXCHANGE_CANCEL_SYN;
			msg.dwObjectID	= HERO->GetID();

			NETWORK->Send( &msg, sizeof( MSGBASE ) );
			SetDisable( TRUE );
			//¿ÃªÛ«œ¥Ÿ! ø©±‚º≠ cancel∏ﬁºº¡ˆ∏¶ ∫∏≥ª∞Ì
			//ackπﬁ¿∏∏È æ∆∑°∑Œ. confirm
//			SetActive( FALSE );
		}
		else if( lId == XCI_BTN_INPUTMONEY )
		{
			if( m_bMyLock )
			{
				//"¿·±›ªÛ≈¬ø°º≠¥¬ µ∑ ¿‘∑¬¿Ã æ»µÀ¥œ¥Ÿ."
			}
			else
			{
				//∏∏æ‡ ∂Ù ∞…æ˙¥Ÿ∏È ∏Æ≈œ. //πˆ∆∞ æ»¥≠∑Ø¡ˆ∞‘«œ±‚.
				LONG x = MOUSE->GetMouseEventX();
				LONG y = MOUSE->GetMouseEventY();
				
				cDivideBox* pDivideBox = WINDOWMGR->DivideBox( DBOX_EXCHANGE_MONEY,
								x, y, OnInputMoney, OnInputCancel, this, NULL, CHATMGR->GetChatMsg(198) );
				SetDisable( TRUE );
				//pDivideBox->SetMaxValue( MAX_MONEY );?
			}
		}
		// 2014-10-29  ¬º˛”≥…‰
		else if( lId == XCI_BTN_INPUTGOLD )
		{
			if( m_bMyLock )
			{
				//"¿·±›ªÛ≈¬ø°º≠¥¬ µ∑ ¿‘∑¬¿Ã æ»µÀ¥œ¥Ÿ."
			}
			else
			{
				//∏∏æ‡ ∂Ù ∞…æ˙¥Ÿ∏È ∏Æ≈œ. //πˆ∆∞ æ»¥≠∑Ø¡ˆ∞‘«œ±‚.
				LONG x = MOUSE->GetMouseEventX();
				LONG y = MOUSE->GetMouseEventY();
				
				cDivideBox* pDivideBox = WINDOWMGR->DivideBox( DBOX_EXCHANGE_GOLD,
								x, y, OnInputGold, OnInputGoldCancel, this, NULL, CHATMGR->GetChatMsg(2099) );
				SetDisable( TRUE );
				//pDivideBox->SetMaxValue( MAX_MONEY );?
			}
		}
	}
	if( we & WE_PUSHDOWN )
	{
		if( lId == XCI_BTN_LOCK )
		{
			if( EXCHANGEMGR->IsLockSyncing() )
			{
				m_pLockBtn->SetPush( FALSE );	//DOWNµ«¡ˆ ∏ªæ∆æﬂ «—¥Ÿ.
			}
			else
			{
				MyLock( TRUE );
				EXCHANGEMGR->Lock( TRUE );	//NetworkMsg
			}
		}
		else if( lId == XCI_BTN_EXCHANGE )
		{
			if( m_bMyLock && !EXCHANGEMGR->IsLockSyncing() && m_bOpLock )
			{
				//¿Œ∫•≈‰∏Æ¿« ªÛ≈¬ ∆ƒæ«..
				int nBlankNum = GAMEIN->GetInventoryDialog()->GetBlankNum();
				if( nBlankNum < m_nOpItemNum - m_nMyItemNum )
				{
					//√ﬂ∞° : ±≥»Ø∫“∞°¥… ∏ﬁºº¡ˆ
					//¿Œ∫•≈‰∏Æ ∞¯∞£¿Ã ∫Œ¡∑«’¥œ¥Ÿ.
					CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg( 354 ) );

					m_pExchangeBtn->SetPush( FALSE );

				}
				//2007. 8. 29. CBH - ±›æ◊ ∞¸∑√ ¿⁄∑·«¸ ø¿πˆ«√∑ŒøÏ ∞≥º±
				//else if( m_dwOpMoney + HERO->GetMoney() > MAX_INVENTORY_MONEY ) //µ∑√º≈©
				else if( m_dwOpMoney > MAX_INVENTORY_MONEY - HERO->GetMoney() ) //µ∑√º≈©
				{
					CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(339) );
					m_pExchangeBtn->SetPush( FALSE );
				}
				// 2014-10-29 »∑»œ«∞£¨∂‘ ‰»ÎºÏ≤È
				else if( m_dwOpGold > MAX_INVENTORY_GOLD - HERO->GetGoldMoney())
				{
					CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(2100) );
					m_pExchangeBtn->SetPush( FALSE );
				}
				else
				{
					//√ﬂ∞° : ¿Œ∫•≈‰∏Æ ¿·Ω√ ∏ÿ√„	//quick√¢¿∫.. ¿Œ∫•¿Ã ∏ÿ√Ë¿∏∏È ªÁøÎ ∫“¥…?
					//GAMEIN->GetInventoryDialog()->SetDisable( TRUE );
					//SetDisable( TRUE );
					EXCHANGEMGR->Exchange();
				}
			}
			else
			{
				//æÁ ∏µŒ ∂Ù¿ª «ÿæﬂ ±≥»Ø «œΩ« ºˆ ¿÷Ω¿¥œ¥Ÿ.
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(330) );
				m_pExchangeBtn->SetPush( FALSE );
			} 
		}
	}
	if( we & WE_PUSHUP )

	{
		if( lId == XCI_BTN_LOCK )
		{
#ifdef _HK_LOCAL_
				m_pLockBtn->SetPush( TRUE );	//UPµ«¡ˆ ∏ªæ∆æﬂ «—¥Ÿ.
#else
			if( EXCHANGEMGR->IsLockSyncing() || EXCHANGEMGR->IsExchangeSyncing() )

			{
				m_pLockBtn->SetPush( TRUE );	//UPµ«¡ˆ ∏ªæ∆æﬂ «—¥Ÿ.
			}
			else
			{
				EXCHANGEMGR->Lock( FALSE );	//network msg
			}
#endif
		}
		else if( lId == XCI_BTN_EXCHANGE )
		{
			m_pExchangeBtn->SetPush( TRUE );
		}
	}
	if( we & WE_LBTNCLICK )
	{
		if( lId == XCI_ICD_MINE )
		{
//			CItem* pItem = GAMEIN->GetInventoryDialog()->GetCurSelectedItem( INV );
//			cIconGridDialog* pIGD = (cIconGridDialog*)GetWindowForID( XCI_ICD_MINE );
//			if( pItem )
//				pIGD->AddIcon( 0, 0, pItem );
		}
	}
	
}

void CExchangeDialog::SetPlayerName( char* pMyName, char* pOpName )
{
	m_pMyNameStc->SetStaticText( pMyName );
	m_pOpNameStc->SetStaticText( pOpName );
}

void CExchangeDialog::SetActive( BOOL val )
{
	if( m_bDisable )		return;
	if( m_bActive == val )	return;

//	cDialog::SetActiveRecursive( val );	//µ∑¿‘∑¬√¢∂ßπÆ¿Ãæ˙¥¬µ•.... æ»«ÿµµ µ…µÌ confirm
	cDialog::SetActive( val );
	
	if( val )
	{
		m_pMyMoneyEditBox->SetEditText( "0" );
		m_pOpMoneyEditBox->SetEditText( "0" );

		//  2014-10-29

		m_pMyGoldEditBox->SetEditText("0");
		m_pOpGoldEditBox->SetEditText("0");

		m_pLockBtn->SetPush( FALSE );
		m_pExchangeBtn->SetPush( FALSE );

		
		//SetDisable( FALSE );
	}

	else
	{
		if( !m_bInit ) return;	//Link() »£√‚ »ƒ ∫Œ≈Õ

		//æ∆¿Ã≈€ ¡ˆøÏ±‚
//		cIcon *pIcon = NULL;
		int i = 0;
		int nMyItemNum = m_nMyItemNum;
		int nOpItemNum = m_nOpItemNum;

		for( i = 0 ; i < nMyItemNum ; ++i )
			DelItem( 0, 1 );	//√ﬂ∞° : ºˆ¡§ø‰
		for( i = 0 ; i < nOpItemNum ; ++i )
			DelItem( 1, 1 );	//√ﬂ∞° : ºˆ¡§ø‰

		m_bMyLock	= FALSE;
		m_bOpLock	= FALSE;

		m_dwMoney	= 0;
		m_dwOpMoney = 0;

		//  2014-10-29

		m_dwGold	= 0;
        m_dwOpGold	= 0;


		m_bExchangable = TRUE;

		m_nMyItemNum		= 0;
		m_nOpItemNum		= 0;



//		ø©±‚ø° ø÷ ¿÷¥¬∞≈¡ˆ? confirm
//		MSGBASE msg;
//		msg.Category	= MP_EXCHANGE;
//		msg.Protocol	= MP_EXCHANGE_CANCEL_SYN;
//		msg.dwObjectID	= HERO->GetID();


//		NETWORK->Send( &msg, sizeof( MSGBASE ) );

		EXCHANGEMGR->Init();	//¿Ã∞Õµµ...

		cDialog* pDlg = WINDOWMGR->GetWindowForID( DBOX_EXCHANGE_MONEY );
		if( pDlg )
		{
			WINDOWMGR->AddListDestroyWindow( pDlg );
		}

		// œ˙ªŸ ‰»Î 2014-10-29

		cDialog* pDlg2 = WINDOWMGR->GetWindowForID( DBOX_EXCHANGE_GOLD );
		if( pDlg2 )
		{
			WINDOWMGR->AddListDestroyWindow( pDlg2 );
		}
	}

//--- main bar icon
//	CMainBarDialog* pDlg = GAMEIN->GetMainInterfaceDialog();
//	if( pDlg )
//		pDlg->SetPushBarIcon( OPT_EXCHANGEDLGICON, m_bActive );
}

void CExchangeDialog::Render()
{
	cDialog::Render();

	if( !m_LockImage.IsNull() )
	{

		VECTOR2 vPos;

		if( m_bMyLock )
		{
//			VECTOR2 vPos = { m_pMyIconGridDlg->m_absPos.x + 3, m_pMyIconGridDlg->m_absPos.y + 3 };
//			m_LockImage.RenderSprite( &vScale, NULL, 0, &vPos,
//										RGBA_MERGE(m_dwImageRGB, m_alpha * m_dwOptionAlpha / 100) );

			for( int i = 0 ; i < 10 ; ++i )
			{
				if( !m_pMyIconGridDlg->IsAddable( i ) ) continue;
				vPos.x = ( m_pMyIconGridDlg->m_absPos.x + 5 ) + ( i % 5 ) * 45 ;
				vPos.y = ( m_pMyIconGridDlg->m_absPos.y + 5 ) + ( i / 5 ) * 45 ;

				m_LockImage.RenderSprite( NULL, NULL, 0.0f, &vPos,
										  RGBA_MERGE(m_dwImageRGB, m_alpha * m_dwOptionAlpha / 100) );

			}
		}

		if( m_bOpLock )
		{
//			VECTOR2 vPos = { m_pOpIconGridDlg->m_absPos.x + 3, m_pOpIconGridDlg->m_absPos.y + 3 };
//			m_LockImage.RenderSprite( &vScale, NULL, 0, &vPos, 
//										RGBA_MERGE(m_dwImageRGB, m_alpha * m_dwOptionAlpha / 100) );

			for( int i = 0 ; i < 10 ; ++i )
			{
				if( !m_pOpIconGridDlg->IsAddable( i ) ) continue;
				
				vPos.x = ( m_pOpIconGridDlg->m_absPos.x + 5 ) + ( i % 5 ) * 45 ;
				vPos.y = ( m_pOpIconGridDlg->m_absPos.y + 5 ) + ( i / 5 ) * 45 ;

				m_LockImage.RenderSprite( NULL, NULL, 0.0f, &vPos,
										  RGBA_MERGE(m_dwImageRGB, m_alpha * m_dwOptionAlpha / 100) );


			}
		}
	}
}

BOOL CExchangeDialog::FakeMoveIcon( LONG x, LONG y, cIcon* icon )
{
	if( m_bDisable ) return FALSE;	//±≥»Ø√¢ ∏ÿ√„¡ﬂ¿Ã∏È ∏Æ≈œ

	if( icon->GetType() != WT_ITEM )
		return FALSE;

/*	// πÆ∆ƒ¡ˆ ∞≈∑°ø°∑Ø ±‰±ﬁºˆ¡§
	if(((CItem*)icon)->GetItemKind() == eEXTRA_ITEM_USABLE)
	{
		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(349) );
		return FALSE;
	}
*/
	if( ( GetTableIdxPosition( ((CItem*)icon)->GetPosition() ) != eItemTable_Inventory ) &&
		( GetTableIdxPosition( ((CItem*)icon)->GetPosition() ) != eItemTable_ShopInven ) )
	{
		if( GetTableIdxPosition( ((CItem*)icon)->GetPosition() ) == eItemTable_Weared )
			CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(315) );
		
		return FALSE;
	}

	// 2014-12-07 
	//if( (((CItem*)icon)->GetItemBaseInfo()->ItemStatic == ITEM_STATIC_PD) && !ITEMMGR->IsDupItem(((CItem*)icon)->GetItemBaseInfo()->wIconIdx))
	//  
	if( (((CItem*)icon)->GetItemBaseInfo()->ItemStatic == ITEM_STATIC_PD)||(((CItem*)icon)->GetItemBaseInfo()->ItemStatic == ITEM_STATIC_NEWBIE))
	{
		//≈›µ„π∫¬ÚŒÔ∆∑Œﬁ∑®Ωª“◊
		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(2126) );

		return FALSE;
	}

	// 2014-12-07 

	if( (((CItem*)icon)->GetItemBaseInfo()->ItemStatic == ITEM_STATIC_LUCK))
	{//À¯∂®ŒÔ∆∑Œﬁ∑®Ωª“◊
		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(2124) );

		return FALSE;
	}

	if( ((CItem*)icon)->GetItemKind() & eSHOP_ITEM )
	{
		// ±‚«—¡¶ æ∆¿Ã¿Ã ≥≤¥¬ æ∆¿Ã≈€¿Ã ∫¿¿Œ«Æ∏Æ∏È ∞≈∑° æ»µ«µµ∑œ
		ITEM_INFO* pInfo = ITEMMGR->GetItemInfo( ((CItem*)icon)->GetItemIdx() );
		if( !pInfo )			return FALSE;
		if( pInfo->ItemType == 11 )
		{
			if( !(((CItem*)icon)->GetItemBaseInfo()->ItemParam & ITEM_PARAM_SEAL) )
			{
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1223) );
				return FALSE;
			}
		}
		// ∞≈∑° æ»µ«¥¬ æ∆¿Ã≈€
		if( ((CItem*)icon)->GetItemKind() == eSHOP_ITEM_EQUIP || ((CItem*)icon)->GetItemKind() == eSHOP_ITEM_PET_EQUIP )
		{

			if( pInfo->BuyPrice )
			{
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1223) );
				return FALSE;
			}
		}
		else if( pInfo->MeleeAttackMax )
		{
			CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1223) );
			return FALSE;
		}
	}

//	if( ((CItem*)icon)->GetItemKind() == eQUEST_ITEM_PET || ((CItem*)icon)->GetItemKind() == eSHOP_ITEM_PET)
	if( ITEMMGR->IsPetSummonItem( ((CItem*)icon)->GetItemIdx() ) )
	{
		//«ÿ¥Á ∆Í ¡§∫∏ ¡ﬂ ƒ£π–µµ ∞ÀªÁ. 30%¿Ã«œ¿Œ¡ˆ..
		const ITEMBASE* itembase = ((CItem*)icon)->GetItemBaseInfo();
		if(!PETMGR->CheckDefaultFriendship(itembase->dwDBIdx))
		{
			CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1275) );
			return FALSE;
		}
		//º“»Ø ¡ﬂ¿Œ ∆Í æ∆¿Ã≈€¿Œ¡ˆ
		if( PETMGR->IsCurPetSummonItem(itembase->dwDBIdx) )
		{
			CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1247) );
			return FALSE;
		}
	}

	// changeitem ¡ﬂø° ∞≈∑°æ»µ«¥¬∞≈
	if( ((CItem*)icon)->GetItemKind() == eCHANGE_ITEM_LOCK )
	{
		return FALSE;

	}
	
	if( ((CItem*)icon)->GetItemKind() == eYOUNGYAK_ITEM ||
		((CItem*)icon)->GetItemKind() == eSHOP_ITEM_INCANTATION ||
		((CItem*)icon)->GetItemKind() == eSHOP_ITEM_HERB ||
		((CItem*)icon)->GetItemKind() == eSHOP_ITEM_CHARM ||
		((CItem*)icon)->GetItemKind() == eSHOP_ITEM_FLGNAME ||//…¡√˚25
		((CItem*)icon)->GetItemKind() == eSHOP_ITEM_SUNDRIES )
	{
		if( QUICKMGR->CheckQPosForItemIdx(((CItem*)icon)->GetItemIdx()) == FALSE )
		{
			CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(317) );
			return FALSE;
		}
	}
	else
	{
		if( ((CItem*)icon)->GetQuickPosition() != 0 )
		{
			CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(317) );
			return FALSE;
		}
	}

	// magi82 - Titan(070618)

	// µÓ∑œµ» ≈∏¿Ã≈∫ ¡ıº≠¥¬ ±≥»Ø«“ºˆ æ¯¿Ω
	if( ((CItem*)icon)->GetDBIdx() == TITANMGR->GetRegistedTitanItemDBIdx() )
	{
		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1583) );
		return FALSE;
	}

	// magi82 - UniqueItem(070628)
	eITEM_KINDBIT bits = ((CItem*)icon)->GetItemKind();
	if( bits == eEQUIP_ITEM_UNIQUE && GAMERESRCMNGR->GetUniqueItemOptionList(((CItem*)icon)->GetItemIdx())->ExchangeFlag == 0 )
	{
		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1626) );
		return FALSE;
	}

	if( m_pMyIconGridDlg->PtInWindow( x, y ) && m_bMyLock == FALSE )
	{
		if( EXCHANGEMGR->LinkItem( (CItem*)icon ) )
			SetDisable( TRUE );
	}
	return FALSE;
}

void CExchangeDialog::FakeDeleteItem( POSTYPE pos )
{
	if( m_bDisable ) return; //±≥»Ø√¢ ∏ÿ√„¡ﬂ¿Ã∏È ∏Æ≈œ

	MSG_WORD msg;
	msg.Category	= MP_EXCHANGE;
	msg.Protocol	= MP_EXCHANGE_DELITEM_SYN;
	msg.dwObjectID	= HERO->GetID();
	msg.wData		= pos;

	NETWORK->Send( &msg, sizeof( MSG_WORD ) );

	SetDisable( TRUE );
	//√ﬂ∞° : ±≥»Ø√¢ ∏ÿ√„
}


void CExchangeDialog::AddItem( int nType, POSTYPE pos, cIcon* pItem )
{
	if( nType == 0 )
	{
		m_pMyIconGridDlg->AddIcon( pos-1, pItem );
		++m_nMyItemNum;
	}
	else
	{
		m_pOpIconGridDlg->AddIcon( pos-1, pItem );
		++m_nOpItemNum;
	}

	//√ﬂ∞° : ±≥»Ø√¢ ∏ÿ√„ «Æ±‚
}


void CExchangeDialog::DelItemOptionInfoAll()
{
	//√Îº“µ«æ˙¿ª∂ß	: itemOptioninfo∏¶ ªË¡¶«—¥Ÿ.

	CExchangeItem*	pIcon = NULL;
	ITEM_INFO*		pInfo = NULL;
	for( int i = 1 ; i <= m_nOpItemNum ; ++i )
	{
		DelItemOptionInfo( (POSTYPE)i );
	}
}

void CExchangeDialog::DelItemOptionInfo( POSTYPE pos )
{
	CExchangeItem* pIcon = (CExchangeItem*)m_pOpIconGridDlg->GetIconForIdx( pos - 1 );
	if( !pIcon ) return;

	if( ITEMMGR->IsOptionItem( (WORD)pIcon->GetData(), pIcon->GetDurability() ) )
	{
		ITEMMGR->RemoveItemOption( pIcon->GetDurability() );
	}
}
//SW050920 Rare
void CExchangeDialog::DelItemRareOptionInfo( POSTYPE pos )
{
	CExchangeItem* pIcon = (CExchangeItem*)m_pOpIconGridDlg->GetIconForIdx( pos - 1 );
	if( !pIcon ) return;

	if( ITEMMGR->IsRareOptionItem( (WORD)pIcon->GetData(), pIcon->GetRareness() ) )
	{
		ITEMMGR->RemoveItemRareOption( pIcon->GetRareness() );
	}
}

// 2014-12-15 

void CExchangeDialog::DelItemStoneOptionAll()
{
	CExchangeItem*	pIcon = NULL;
	ITEM_INFO*		pInfo = NULL;
	for( int i = 1 ; i <= m_nOpItemNum ; ++i )
	{
		DelItemStoneOptionInfo( (POSTYPE)i );
	}
}

// 2014-12-15 
void CExchangeDialog::DelItemStoneOptionInfo(POSTYPE pos)
{
	CExchangeItem* pIcon = (CExchangeItem*)m_pOpIconGridDlg->GetIconForIdx( pos - 1 );
	if( !pIcon ) return;

	if( ITEMMGR->GetItemStoneOption(pIcon->GetStoneIdx()))
	{
		ITEMMGR->RemoveItemStoneOption( pIcon->GetStoneIdx());
	}
}

void CExchangeDialog::DelPetItemInfo( POSTYPE pos )
{
	CExchangeItem* pIcon = (CExchangeItem*)m_pOpIconGridDlg->GetIconForIdx( pos - 1 );
	if( !pIcon ) return;

	DWORD itemDBIdx = pIcon->GetDBId();
	if( ITEMMGR->IsPetSummonItem( (WORD)pIcon->GetData() ) )
	{
		PETMGR->RemovePetInfo(itemDBIdx);
	}
}


void CExchangeDialog::DelPetItemInfoAll()
{
	for( int i = 1; i <= m_nOpItemNum; ++i )
	{
		DelPetItemInfo( (POSTYPE)i );
	}
}
	
void CExchangeDialog::DelItemRareOptionInfoAll()
{
	for( int i = 1; i <= m_nOpItemNum; ++i )
	{
		DelItemRareOptionInfo( (POSTYPE)i );
	}
}

void CExchangeDialog::DelItem( int nType, POSTYPE pos )
{
	cIcon* pIcon = NULL;

	if( nType == 0 )
	{
		if( m_pMyIconGridDlg->DeleteIcon( pos-1, &pIcon ) )
		{
			if( pIcon == NULL) return;
			
			EXCHANGEMGR->UnLinkItem( 0, pIcon );

			for( int i = pos ; i < m_nMyItemNum ; ++i )
			{
				m_pMyIconGridDlg->DeleteIcon( i, &pIcon );
				if( pIcon == NULL ) break;

				((CExchangeItem*)pIcon)->SetPosition( i );
				m_pMyIconGridDlg->AddIcon( i - 1, pIcon );
			}

			//linkitemµµ «œ≥™ ¡Ÿ¿Ã±‚.////////////
			--m_nMyItemNum;
		}
	}
	else
	{

		if( m_pOpIconGridDlg->DeleteIcon( pos-1, &pIcon ) )
		{
			if( pIcon == NULL) return;

			EXCHANGEMGR->UnLinkItem( 1, pIcon );

			for( int i = pos ; i < m_nOpItemNum ; ++i )
			{
				m_pOpIconGridDlg->DeleteIcon( i, &pIcon );
				if( pIcon == NULL ) break;

				m_pOpIconGridDlg->AddIcon( i - 1, pIcon );
			}
			--m_nOpItemNum;

		}
	}

	//√ﬂ∞° : ±≥»Ø√¢ ∏ÿ√„ «Æ±‚	
}

DWORD CExchangeDialog::InputMoney( int nType, DWORD dwMoney )
{
	if( nType == 0 )
	{
		m_dwMoney = dwMoney;
		m_pMyMoneyEditBox->SetEditText( AddComma( dwMoney ) );
	}
	else
	{
		m_dwOpMoney = dwMoney;
#ifndef _HK_LOCAL_
		m_pOpMoneyEditBox->SetEditText( AddComma( dwMoney ) ); //»´ƒ·¿∫ Ω«Ω√∞£¿Ã æ∆¥œ∂Û ∂Ù∞…∂ß∏∏ ∫∏ø©¡ÿ¥Ÿ.
#endif
	}

	return dwMoney;
}
// 2014-10-29  ‰»Î
DWORD CExchangeDialog::InputGold( int nType, DWORD dwGold )
{
	if( nType == 0 )
	{
		m_dwGold = dwGold;
		m_pMyGoldEditBox->SetEditText( AddComma( dwGold ) );
	}
	else
	{
		m_dwOpGold = dwGold;
#ifndef _HK_LOCAL_
		m_pOpGoldEditBox->SetEditText( AddComma(  dwGold ) ); //»´ƒ·¿∫ Ω«Ω√∞£¿Ã æ∆¥œ∂Û ∂Ù∞…∂ß∏∏ ∫∏ø©¡ÿ¥Ÿ.
#endif
	}

	return dwGold;
}

void CExchangeDialog::OpLock( BOOL bLock )
{
	m_bOpLock = bLock;


	int i;
	for( i = 0 ; i < m_nOpItemNum ; ++i )
	{
		if( m_pOpIconGridDlg->GetIconForIdx( i ) )
			m_pOpIconGridDlg->GetIconForIdx( i )->SetLock( bLock );

	}

	if( bLock == FALSE )	//ªÛ¥Î∞° ∂Ù¿ª «Æ∏È.. ≥™¿« ªÛ≈¬µµ πŸ≤€¥Ÿ.
	{
		//√ﬂ∞° : Syncingø° ¥Î«— πÆ¡¶ ª˝∞¢«œ±‚
		m_pExchangeBtn->SetPush( FALSE );

		//√ﬂ∞° : ¿€µø ∞°¥…
#ifdef _HK_LOCAL_
		m_pOpMoneyEditBox->SetEditText( "0" );
		m_pOpGoldEditBox->SetEditText( "0" );  // 2014-10-29 
#endif


	/////////MyLock
		m_bMyLock = FALSE;

		for( i = 0 ; i < m_nMyItemNum ; ++i )
		{
			if( m_pMyIconGridDlg->GetIconForIdx( i ) )
				m_pMyIconGridDlg->GetIconForIdx( i )->SetLock( FALSE );
		}
		
		m_pInputMoneyBtn->SetDisable( FALSE );

		m_pInputGoldBtn->SetDisable(FALSE);   // 2014-10-29  ‰»Î∞¥≈•

		m_pLockBtn->SetPush( FALSE );
	/////////////
	}
#ifdef _HK_LOCAL_
	else	//lock
	{
		m_pOpMoneyEditBox->SetEditText( AddComma( m_dwOpMoney ) );

		m_pOpGoldEditBox->SetEditText( AddComma( m_dwOpGold ) ); // 2014-10-29  ‰»Î
	}
#endif
}

void CExchangeDialog::MyLock( BOOL bLock )
{
	m_bMyLock = bLock;

	int i;
	for( i = 0 ; i < m_nMyItemNum ; ++i )
	{
		if( m_pMyIconGridDlg->GetIconForIdx( i ) )
			m_pMyIconGridDlg->GetIconForIdx( i )->SetLock( bLock );
	}
	
	if( bLock )
	{
		m_pInputMoneyBtn->SetDisable( TRUE );

		m_pInputGoldBtn->SetDisable(TRUE); // 2014-10-29  ‰»Î∞¥≈•

		cDialog* pDlg = WINDOWMGR->GetWindowForID( DBOX_EXCHANGE_MONEY );
		if( pDlg )
		{
			WINDOWMGR->AddListDestroyWindow( pDlg );
		}
        // 2014-10-29 œ˙ªŸ ‰»Î
		cDialog* pDlg2 = WINDOWMGR->GetWindowForID( DBOX_EXCHANGE_GOLD );
		if( pDlg2 )
		{
			WINDOWMGR->AddListDestroyWindow( pDlg2 );
		}
	}
	else
	{
		m_bOpLock = bLock;
		for( i = 0 ; i < m_nOpItemNum ; ++i )		//«‘ºˆ∑Œ ∂ÙACK¿œ∂ß Ω««‡«—¥Ÿ?
		{
			if( m_pOpIconGridDlg->GetIconForIdx( i ) )
				m_pOpIconGridDlg->GetIconForIdx( i )->SetLock( bLock );
		}

		m_pInputMoneyBtn->SetDisable( FALSE );

		m_pInputGoldBtn->SetDisable(FALSE);     // 2014-10-29  ‰»Î∞¥≈•
	}

	m_pLockBtn->SetPush( bLock );
}


void CExchangeDialog::OnInputMoney( LONG iId, void* p, DWORD param1, void * vData1, void * vData2 )
{
	CExchangeDialog* pDlg = (CExchangeDialog*)vData1;
	if( param1 == pDlg->GetMoney() )
	{
		pDlg->SetDisable( FALSE );
		return;
	}

	
	if( !pDlg->IsActive() ) return;
	
	if( pDlg->IsMyLock() )
	{	//∂Ù∞…∏∞ ªÛ≈¬∂Û∏È
		return;
	}

	DWORD dwCurMoney = HERO->GetMoney();

	if( dwCurMoney < param1 )
		param1 = dwCurMoney;

	MSG_DWORD msg;
	msg.Category	= MP_EXCHANGE;
	msg.Protocol	= MP_EXCHANGE_INPUTMONEY_SYN;
	msg.dwObjectID	= HERO->GetID();
	msg.dwData		= param1;
	NETWORK->Send( &msg, sizeof( MSG_DWORD ) );
}


void CExchangeDialog::OnInputCancel( LONG iId, void* p, DWORD param1, void * vData1, void * vData2 )
{
	CExchangeDialog* pDlg = (CExchangeDialog*)vData1;
	pDlg->SetDisable( FALSE );
}


void CExchangeDialog::OnInputGold( LONG iId, void* p, DWORD param1, void * vData1, void * vData2 )
{
	CExchangeDialog* pDlg = (CExchangeDialog*)vData1;
	if( param1 == pDlg->GetGold())
	{
		pDlg->SetDisable( FALSE );
		return;
	}

	
	if( !pDlg->IsActive() ) return;
	
	if( pDlg->IsMyLock() )
	{	//∂Ù∞…∏∞ ªÛ≈¬∂Û∏È
		return;
	}

	DWORD dwCurMoney = HERO->GetGoldMoney();

	if( dwCurMoney < param1 )
		param1 = dwCurMoney;

	MSG_DWORD msg;
	msg.Category	= MP_EXCHANGE;
	msg.Protocol	= MP_EXCHANGE_INPUTGOLD_SYN;
	msg.dwObjectID	= HERO->GetID();
	msg.dwData		= param1;
	NETWORK->Send( &msg, sizeof( MSG_DWORD ) );
}


void CExchangeDialog::OnInputGoldCancel( LONG iId, void* p, DWORD param1, void * vData1, void * vData2 )
{
	CExchangeDialog* pDlg = (CExchangeDialog*)vData1;
	pDlg->SetDisable( FALSE );
}




