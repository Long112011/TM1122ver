#include "stdafx.h"
#include "OtherCharacterAmuletDlg.h"
#include "WindowIDEnum.h"
#include "./Interface/cWindowManager.h"
#include "./Interface/cIconGridDialog.h"
#include "./Interface/cIcon.h"
#include "ItemManager.h"
#include "cScriptManager.h"
#include "ChatManager.h"

// 2014-11-10 增益功开发！

COtherCharacterAmuletDlg::COtherCharacterAmuletDlg()
{
    m_LastTime=0;

	p_Dlg=NULL;

	p_Name=NULL;

	p_Grid=NULL;
}

COtherCharacterAmuletDlg::~COtherCharacterAmuletDlg()
{
	p_Dlg=NULL;

	p_Name=NULL;

	p_Grid=NULL;
}

DWORD COtherCharacterAmuletDlg::GetLastTime()
{
   return  m_LastTime;
}
void COtherCharacterAmuletDlg::SetLastTime(DWORD Val)
{
   m_LastTime=Val;
}
void COtherCharacterAmuletDlg::Linking()
{
   	p_Dlg = WINDOWMGR->GetWindowForID( OTHER_AMULET_DLG );

	p_Name= (cStatic*)GetWindowForID(OTHER_AMULET_NAME);

	p_Grid= (cIconGridDialog*)GetWindowForID(OTHER_AMULET_GRID);

}

void COtherCharacterAmuletDlg::SetName(char * Name)
{
	char TempMsg[256];

	sprintf(TempMsg,CHATMGR->GetChatMsg(2111),Name);

	p_Name->SetStaticText(TempMsg);
}

void COtherCharacterAmuletDlg::ClearAmuletInfo()
{
    if(p_Dlg)
	{
		if(p_Grid)
		{
			cIcon* pIcon;

			for(int n=0;n<p_Grid->m_nCol*p_Grid->m_nRow;++n)
			{
				pIcon = p_Grid->GetIconForIdx(n);
				if(pIcon != NULL)
				{
					p_Grid->DeleteIcon(pIcon);
					pIcon->SetActive( FALSE );
					WINDOWMGR->AddListDestroyWindow( pIcon );
					pIcon = NULL;
				}

			}
		}
	}
}

void COtherCharacterAmuletDlg::SetAmuletInfo(int Pos,SHOPITEMBASE * pItem)
{

	if(p_Dlg)
	{
		if(p_Grid)
		{

			ITEM_INFO * pInfo= ITEMMGR->GetItemInfo(pItem->ItemBase.wIconIdx);

			if(pInfo)
			{

				cIcon * pIcon;

				pIcon=new cIcon;

				pIcon->Init(0,0,DEFAULT_ICONSIZE,DEFAULT_ICONSIZE, ITEMMGR->GetIconImage(pItem->ItemBase.wIconIdx, &tmpImage),IG_DEALITEM_START+3000+Pos);
				
				pIcon->SetData(pItem->ItemBase.wIconIdx);

				cImage imgToolTip;

				SCRIPTMGR->GetImage( 63, &imgToolTip, PFT_HARDPATH );

				pIcon->SetToolTip("", RGBA_MAKE(255, 255, 255, 255), &imgToolTip, TTCLR_ITEM_CANEQUIP );

				TOOLTIP_TEXT* tipInfo= ITEMMGR->GetItemToolTipInfo(pInfo->ItemTooltipIdx);

				//pIcon->AddToolTipLine(pInfo->ItemName,TTTC_SETITEM_NAME);

				//ITEMMGR->SetMallAndGoldShopToolTipIcon(pIcon);
				ITEMMGR->SetToolTipIcon( pIcon, 
					NULL,
					NULL,
					0,
					NULL,
					0,
					1
					);

				pIcon->SetMovable(FALSE);

				/*if( tipInfo )
	            {
					pIcon->AddToolTipLine("");
						
					while( tipInfo )		
					{
						pIcon->AddToolTipLine( tipInfo->strToolTip, TTTC_DESCRIPTION );
			
						tipInfo = tipInfo->pNext;
		
					}
				}*/

				p_Grid->AddIcon(Pos,pIcon);
			}
		 
		}
	}

}