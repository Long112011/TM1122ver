#include "stdafx.h"
#include "ChatDialog.h"
#include "WindowIdEnum.h"
#include "./Interface/cListDialog.h"
#include "./Interface/cEditBox.h"
#include "./Interface/cPushupButton.h"
#include "cImeEx.h"
#include "GlobalEventFunc.h"
#include "./Input/UserInput.h"
#include "./Input/Mouse.h"
#include "./Input/Keyboard.h"
#include "ChatManager.h"
#include "GameIn.h"
#include "OptionDialog.h"
#include "ShoutchatDialog.h"
#include "cScriptManager.h"                
#include "ItemManager.h"                   
#include "SendEquipDialog.h"            
#include "./Interface/cFont.h"
#include "input/UserInput.h"
#include "CharacterRBTN.h"
#include "cWindowManager.h"
#include "ObjectManager.h"
extern HWND _g_hWnd;
CChatDialog::CChatDialog()
{
	m_type				= WT_CHATDIALOG;
	m_pChatEditBox		= NULL;
	m_bHideChatDialog	= FALSE;
	m_bShowGuildTab		= FALSE;
	m_cPreWord[0] = 0;
	m_cPreWord[1] = '@';
	m_cPreWord[2] = '#';
	m_cPreWord[3] = '$';
	m_cPreWord[4] = '%';
	dwSelectedItem=0;
	m_ChatDlgSizeBtn[ChatExtend]= NULL;
	m_ChatDlgSizeBtn[ChatReduction]= NULL;
}
CChatDialog::~CChatDialog()
{
}
BOOL CChatDialog::IsChatFocus()
{
	if(m_pChatEditBox)
		return m_pChatEditBox->IsFocus();
	else
		return FALSE;
}
void CChatDialog::Linking()
{
	pDlg	= WINDOWMGR->GetWindowForID( CTI_DLG );
	m_pChatEditBox	= (cEditBox *)GetWindowForID(MI_CHATEDITBOX);
	m_pChatEditBox->SetValidCheck( VCM_DEFAULT );
	m_pChatEditBox->SetEditFunc( GAMEIN_ChatFunc );
	m_ImageGrid =(cIconGridDialog*)GetWindowForID(CTI_ICONGRID_IMAGE);
	m_pSheet[WHOLE]		= (cListDialog*)GetWindowForID(CTI_SHEET1);
	m_pSheet[PARTY]		= (cListDialog*)GetWindowForID(CTI_SHEET2);
	m_pSheet[GUILD]		= (cListDialog*)GetWindowForID(CTI_SHEET3);
	m_pSheet[ALLIANCE]	= (cListDialog*)GetWindowForID(CTI_SHEET4);
	m_pSheet[SHOUT]		= (cListDialog*)GetWindowForID(CTI_SHEET5);
	m_pPBMenu[WHOLE]	= (cPushupButton*)GetWindowForID(CTI_BTN_WHOLE);
	m_pPBMenu[PARTY]	= (cPushupButton*)GetWindowForID(CTI_BTN_PARTY);
	m_pPBMenu[GUILD]	= (cPushupButton*)GetWindowForID(CTI_BTN_MUNPA);
	m_pPBMenu[ALLIANCE]	= (cPushupButton*)GetWindowForID(CTI_BTN_ALLMUNPA);
	m_pPBMenu[SHOUT]	= (cPushupButton*)GetWindowForID(CTI_BTN_WORLD);
	m_pAllShout = (cPushupButton*)GetWindowForID( CTI_BTN_ALLWORLD1 );
	m_pAllShout->SetPush( TRUE );
	m_pSheet[WHOLE]->SetActive( TRUE );
	m_pSheet[PARTY]->SetActive( FALSE );
	m_pSheet[GUILD]->SetActive( FALSE );
	m_pSheet[ALLIANCE]->SetActive( FALSE );
	m_pSheet[SHOUT]->SetActive( FALSE );
	m_nCurSheetNum = WHOLE;
	SelectMenu( m_nCurSheetNum );
    LoadFaceList();
	m_pChatEditBox->SetRenderFaceImg(TRUE);
	m_pSheet[WHOLE]->SetRenderFaceImg( TRUE );
	m_pSheet[PARTY]->SetRenderFaceImg(TRUE );
	m_pSheet[GUILD]->SetRenderFaceImg(TRUE);
	m_pSheet[ALLIANCE]->SetRenderFaceImg(TRUE );
	m_pSheet[SHOUT]->SetRenderFaceImg( TRUE );
	m_ChatDlgSizeBtn[ChatExtend]		= (cButton*)GetWindowForID(CTI_BTN_TALLER);
	m_ChatDlgSizeBtn[ChatReduction]		= (cButton*)GetWindowForID(CTI_BTN_SMALLER);
}
BOOL CChatDialog::IsChatEditBoxActive()
{
	return m_pChatEditBox->IsFocus();
}
void CChatDialog::LoadFaceList()
{
	for(int i=0;i<CHARFACELIST; i++)
	{
		if(SCRIPTMGR->GetEmoji(i))
		{
			SCRIPTMGR->GetImage(SCRIPTMGR->GetEmoji(i)->ImageIdx,&m_ImageFace[i],&SCRIPTMGR->GetEmoji(i)->rect[0]);
		}
		cIcon * pIcon;
		pIcon=new cIcon;
		pIcon->Init(0,0,16,16,&m_ImageFace[i] ,IG_DEALITEM_START+5000+i);				
		pIcon->SetData(i+FACELISTSTARTIDX);
		pIcon->SetDisable(TRUE);
		pIcon->SetDepend(FALSE);
		pIcon->SetMovable(false);
		m_ImageGrid->AddIcon(i,pIcon);
		char bTtemp[16]={0,};
		sprintf(bTtemp,"'&%02d'",i);
		cImage ToolTipImage;
		SCRIPTMGR->GetImage( 63, &ToolTipImage, PFT_HARDPATH );
		pIcon->SetToolTip(bTtemp,RGBA_MAKE(255, 255, 0, 255), &ToolTipImage, TTCLR_DEFAULT);
	}
	m_ImageGrid->SetActive(FALSE);
}
DWORD CChatDialog::ActionEvent( CMouse * mouseInfo )
{
	DWORD we = WE_NULL;
	static DWORD dwLebar=420;
	if (!IsActive()) return we;// WE_NULL;
	if (!mouseInfo) return we;// WE_NULL;
	we |= cDialog::ActionEvent( mouseInfo );
	LONG mouseX=(LONG)mouseInfo->GetMouseX();
	LONG mouseY=(LONG)mouseInfo->GetMouseY();
	
	if( m_ChatDlgSizeBtn[0]->IsClickInside() )
	{
		OnSize(1);
		return we;
	}
	else if( m_ChatDlgSizeBtn[1]->IsClickInside() )
	{
		OnSize(0);
		return we;
	}
	for(int i=WHOLE; i< SHOUT; i++)
	{
		cListDialog* pCurSheet = m_pSheet[i];
		ITEM* pItem1 = pCurSheet->GetSelectedITEM(mouseInfo);
		BYTE maxline = pCurSheet->GetMaxLine();
		DWORD height = pDlg->cWindow::GetHeight()+pCurSheet->GetHeight(maxline);
		if(we && pCurSheet->PtInWindow(mouseX, mouseY) )
		{
			char buf[256];
			//if (mouseInfo->LButtonDown() || mouseInfo->LButtonPressed())
			//{
			//	dwLebar = 420;
			//}
			if(mouseInfo->LButtonUp())
			{
				if (pItem1 && pItem1->Value)
				{
					CHARRBTN->RequestMsgForItemLink(pItem1->Value);
				}					
			}
			if(mouseInfo->RButtonUp())
			{
				if (pItem1)
				{
					GetSelectedName(mouseInfo);
				}
			}

			int Wheel = mouseInfo->GetWheel();
			if (Wheel)

			if (Wheel > 0)
			{
				pCurSheet->OnUpwardItem();
			}
			else
			{
				pCurSheet->OnDownwardItem();
			}

			//if (pItem1)
			//{
			//	sprintf(buf, "pCharacterIDX %d %d", pItem1->pCharacterIDX, pCurSheet->GetCurSelectedRowIdx());
			//	pCurSheet->SetToolTip(buf);
			//}			
		}

		
		if (mouseInfo->LButtonUp() || 
			mouseInfo->LButtonDown() ||
			mouseInfo->LButtonPressed()||
			mouseInfo->RButtonUp()||
			mouseInfo->RButtonDown()||
			mouseInfo->RButtonPressed())
		{
			if (pItem1 && pItem1->Value)
			{
				dwLebar = 420;
			}
			else
			{
				dwLebar = 14;
			}
		}
		else
		{
			dwLebar = 420;
			//CHATMGR->AddMsg(CTC_SYSMSG, "2");
		}



		//pDlg->SetWH(14, height);
		pCurSheet->SetWH(dwLebar,height);
	}
	pDlg->SetWidth(dwLebar);
	//pCurSheet->SetWidth(dwLebar);
	return we;
}
extern char LinkFirst[128];
extern char LinkSecond[8];
void CChatDialog::RemoveMsg(char* str)
{
	//GetSheet(WHOLE)->RemoveItem(str);
	//int pos = GetSheet(WHOLE)->RemoveItem2(str);
	GetSheet(WHOLE)->RemoveItem(str);
	//CHATMGR->AddMsg(CTC_SYSMSG, "pos %d GetCurSelectedRowIdx %d", pos, GetSheet(WHOLE)->GetCurSelectedRowIdx());
	//GetSheet(WHOLE)->AddItem("test", 0x00ff00, GetSheet(WHOLE)->GetCurSelectedRowIdx());
}
void CChatDialog::AddMsg( BYTE ChatLimit, DWORD MsgColor, char* str )
{
	int nLen = strlen( str );
	char buf[256];
	int nCpyNum = 0;
	BOOL btwoline = FALSE;
	while( nLen > CHATLIST_TEXTLEN )
	{
		if( ( str + CHATLIST_TEXTLEN ) != CharNext( CharPrev( str, str + CHATLIST_TEXTLEN ) ) )
			nCpyNum = CHATLIST_TEXTLEN - 1;
		else
			nCpyNum = CHATLIST_TEXTLEN;
		strncpy( buf, str, nCpyNum );
		buf[nCpyNum] = 0;
		GetSheet(ChatLimit)->AddItem( buf, MsgColor );
		nLen -= nCpyNum;
		str += nCpyNum;
		if( *str == ' ' ) ++str;
		btwoline = TRUE;
	}
	if( nLen > 0 )
	{   
		std::string Buffer=str;
		DWORD dwItemDBIdx=0;
		DWORD dwItemIdx=0;
		DWORD dwHeroIdx=0;
		int Pos =Buffer.find(LinkFirst,0);
		if(Pos>=0)
		{
			int PosName= Buffer.find(":",0);
			std::string CharName= Buffer.substr(0,PosName);
			std::string ItemInfo=Buffer.substr(Pos+10,Buffer.length());
            int ItemPos= ItemInfo.find('¡ú',0);
			if(ItemPos>=0)
			{
				std::string ItemDBIdx=ItemInfo.substr(0,ItemPos);
				std::string ItemIdx=ItemInfo.substr(ItemPos+1,ItemInfo.length());
				dwItemDBIdx= atoi(ItemDBIdx.c_str());
				dwItemIdx= atoi(ItemIdx.c_str());
				if(dwItemDBIdx>0&& dwItemIdx>0)
				{
					ITEM_INFO * pItem =ITEMMGR->GetItemInfo(dwItemIdx);
					if(pItem)
					{
						if(CharName.length()+strlen(pItem->ItemName)<128)
						{
							memset(str,0,strlen(str));
							wsprintf(str,"%s: [%s]",CharName.c_str(),pItem->ItemName);
						}
					}
				}
			}
		}
		GetSheet(ChatLimit)->AddItem( str, MsgColor, -1,0,dwItemDBIdx);
		//cMultiLineText m_toolTip;
		//GetSheet(ChatLimit)->AddItem(str, MsgColor, m_toolTip, -1, dwItemDBIdx, 2);
		//sprintf(buf, "%d", GetSheet(ChatLimit)->GetItemCount());
		//GetSheet(ChatLimit)->SetToolTip(buf);
	}
}
void CChatDialog::AddMsgAllRecall(DWORD MsgColor, char* str)
{
	int nLen = strlen(str);
	char buf[128];
	int nCpyNum = 0;
	while (nLen > CHATLIST_TEXTLEN)
	{
		if ((str + CHATLIST_TEXTLEN) != CharNext(CharPrev(str, str + CHATLIST_TEXTLEN)))
			nCpyNum = CHATLIST_TEXTLEN - 1;
		else
			nCpyNum = CHATLIST_TEXTLEN;
		strncpy(buf, str, nCpyNum);
		buf[nCpyNum] = 0;
		GetSheet(WHOLE)->AddItem(buf, MsgColor);
		GetSheet(PARTY)->AddItem(buf, MsgColor);
		GetSheet(GUILD)->AddItem(buf, MsgColor);
		nLen -= nCpyNum;
		str += nCpyNum;
		if (*str == ' ') ++str;
	}
	if (nLen > 0)
	{
		GetSheet(WHOLE)->AddItem(str, MsgColor);
		GetSheet(PARTY)->AddItem(str, MsgColor);
		GetSheet(GUILD)->AddItem(str, MsgColor);
	}

	//GetSheet(WHOLE)->AddToolTipLine3("test");
}
void CChatDialog::AddMsgAll( DWORD MsgColor, char* str )
{
	int nLen = strlen( str );
	char buf[128];
	int nCpyNum = 0;
	while( nLen > CHATLIST_TEXTLEN )
	{
		if( ( str + CHATLIST_TEXTLEN ) != CharNext( CharPrev( str, str + CHATLIST_TEXTLEN ) ) )
			nCpyNum = CHATLIST_TEXTLEN - 1;
		else
			nCpyNum = CHATLIST_TEXTLEN;
		strncpy( buf, str, nCpyNum );
		buf[nCpyNum] = 0;
		GetSheet(WHOLE)->AddItem( buf, MsgColor );
		GetSheet(PARTY)->AddItem( buf, MsgColor );
		GetSheet(GUILD)->AddItem( buf, MsgColor );
		nLen -= nCpyNum;
		str += nCpyNum;
		if( *str == ' ' ) ++str;
	}
	if( nLen > 0 )
	{
		GetSheet(WHOLE)->AddItem( str, MsgColor );
		GetSheet(PARTY)->AddItem( str, MsgColor );
		GetSheet(GUILD)->AddItem( str, MsgColor );
	}
	
	//GetSheet(WHOLE)->AddToolTipLine3("test");
}
void CChatDialog::AddMsgAll(int MsgColor, char* str)
{
	int nLen = strlen(str);
	char buf[128];
	int nCpyNum = 0;
	while (nLen > CHATLIST_TEXTLEN)
	{
		if ((str + CHATLIST_TEXTLEN) != CharNext(CharPrev(str, str + CHATLIST_TEXTLEN)))
			nCpyNum = CHATLIST_TEXTLEN - 1;
		else
			nCpyNum = CHATLIST_TEXTLEN;
		strncpy(buf, str, nCpyNum);
		buf[nCpyNum] = 0;
		GetSheet(WHOLE)->AddItem(buf, MsgColor/* 5*/);
		GetSheet(PARTY)->AddItem(buf, MsgColor);
		GetSheet(GUILD)->AddItem(buf, MsgColor);
		nLen -= nCpyNum;
		str += nCpyNum;
		if (*str == ' ') ++str;
	}
	if (nLen > 0)
	{
		GetSheet(WHOLE)->AddItem(str, MsgColor/*, 5*/);
		GetSheet(PARTY)->AddItem(str, MsgColor);
		GetSheet(GUILD)->AddItem(str, MsgColor);
	}

	//GetSheet(WHOLE)->AddToolTipLine3("test");
}
void CChatDialog::OnSize(BOOL bFlag)
{
	m_pSheet[WHOLE]->onSize( bFlag );
	m_pSheet[PARTY]->onSize( bFlag );
	m_pSheet[GUILD]->onSize( bFlag );
	m_pSheet[ALLIANCE]->onSize( bFlag );
	m_pSheet[SHOUT]->onSize( bFlag );
	if( GAMEIN->GetShoutchatDlg() )
		GAMEIN->GetShoutchatDlg()->RefreshPosition();
}
void CChatDialog::OnActionEvent(LONG lId, void * p, DWORD we)
{
	if( we & WE_BTNCLICK )
	{
		switch( lId )
		{
			case CTI_BTN_TALLER:	OnSize(TRUE);	break;
			case CTI_BTN_SMALLER:	OnSize(FALSE);	break;
			case CTI_BTN_IMAGE:	m_ImageGrid->SetActive(!m_ImageGrid->IsActive());	break;
		}
	}
	else if( we & WE_PUSHDOWN  )
	{
		switch( lId )
		{
			case CTI_BTN_WHOLE:			SelectMenu(WHOLE);			break;			
			case CTI_BTN_PARTY:			SelectMenu(PARTY);			break;
			case CTI_BTN_MUNPA:			SelectMenu(GUILD);			break;
			case CTI_BTN_ALLMUNPA:		SelectMenu(ALLIANCE);		break;
			case CTI_BTN_WORLD:			SelectMenu(SHOUT);			break;
			case CTI_BTN_ALLWORLD1:		SetAllShoutBtnPushed(TRUE);	break;
		}
		m_pChatEditBox->SetFocusEdit( TRUE );		
	}
	else if( we & WE_PUSHUP)
	{
		SelectMenu(m_nCurSheetNum);
		m_pChatEditBox->SetFocusEdit( TRUE );
		switch( lId )
		{
			case CTI_BTN_ALLWORLD1:		SetAllShoutBtnPushed(false);	break;			
		}
	}
	else if( we & WE_LBTNCLICK)
	{
        switch(lId)
		{
			case CTI_ICONGRID_IMAGE:
			{
				int Pos=m_ImageGrid->GetCurSelCellPos();
				if(Pos>=0)
				{
					char ImgCode[10];
					sprintf(ImgCode,"&%02d",Pos);
					m_pChatEditBox->SetEditText(strcat(m_pChatEditBox->GetEditText(),ImgCode));
					m_pChatEditBox->SetFocusEdit( TRUE );
				}
			}
		}
	}
}
void CChatDialog::SetEditBoxPreWord()
{
	char buf[256] = {0,};
	SafeStrCpy( buf, m_pChatEditBox->GetEditText(), 128 );
	int nLen = strlen( buf );
	if( nLen == 0 )
	{
		buf[0] = m_cPreWord[m_nCurSheetNum];
	}
	else if( buf[0] != '/' )
	{
		if( m_nCurSheetNum != WHOLE )
		{
			if( IsPreWord( buf[0] ) == FALSE )
			{
				memmove( buf+1, buf, nLen );
			}
			buf[0] = m_cPreWord[m_nCurSheetNum];
		}
		else
		{
			if( IsPreWord( buf[0] ) == TRUE )
			{
				memmove( buf, buf+1, nLen );
			}
		}
	}
	m_pChatEditBox->SetEditText( buf );
}
BOOL CChatDialog::IsPreWord( char c )
{
	for( int i = PARTY ; i <= SHOUT ; ++i )
	{
		if( m_cPreWord[i] == c ) return TRUE;
	}
	return FALSE;		
}
void CChatDialog::SelectMenu( int nSheet )
{
	/*if( nSheet >= MAX_CHAT_COUNT ) return;
	for( int i = 0 ; i < MAX_CHAT_COUNT ; ++i )
	{
		if( nSheet == i )
			m_pPBMenu[i]->SetPush(TRUE);
		else
			m_pPBMenu[i]->SetPush(FALSE);
	}*/
	for (int i = 0; i < MAX_CHAT_COUNT; ++i)
	{
		m_pPBMenu[i]->SetPush(FALSE);
	}
	
	m_nCurSheetNum = nSheet;
	m_pPBMenu[m_nCurSheetNum]->SetPush(TRUE);
	SetEditBoxPreWord();
}
void CChatDialog::HideChatDialog( BOOL bHide )
{
	m_bHideChatDialog = bHide;
	if( bHide )
	{
		m_pSheet[WHOLE]->SetActive( FALSE );
	}
	else
	{
		m_pSheet[WHOLE]->SetActive( TRUE );
	}
}
void CChatDialog::ShowGuildTab( BOOL bShow )
{
}
BYTE CChatDialog::GetLineNum()
{ 
	return m_pSheet[m_nCurSheetNum]->GetMiddleImageNum();
}
WORD CChatDialog::GetSheetPosY()
{
	return (WORD)m_pSheet[m_nCurSheetNum]->GetAbsY();
}
WORD CChatDialog::GetSheetHeight()
{
	DWORD height = 0;
	height += m_pSheet[m_nCurSheetNum]->GetHeight( LD_TOP );
	height += m_pSheet[m_nCurSheetNum]->GetHeight( LD_DOWN );
	height += ( m_pSheet[m_nCurSheetNum]->GetHeight( LD_MIDDLE )*m_pSheet[m_nCurSheetNum]->GetMiddleImageNum() );
	return (WORD)height;	
}
void CChatDialog::SetAllShoutBtnPushed( BOOL val )
{
	if( m_pAllShout )
	{
		m_pAllShout->SetPush( val );
		GAMEIN->GetShoutchatDlg()->SetActive(val);
	}
}
#include <string>
std::string ReplaceAll2(std::string str, const std::string& from, const std::string& to) {
	size_t start_pos = 0;
	while ((start_pos = str.find(from, start_pos)) != std::string::npos) {
		str.replace(start_pos, from.length(), to);
		start_pos += to.length(); // Handles case where 'to' is a substring of 'from'
	}
	return str;
}
static inline void ReplaceAll3(std::string &str, const std::string& from, const std::string& to)
{
	size_t start_pos = 0;
	while ((start_pos = str.find(from, start_pos)) != std::string::npos) {
		str.replace(start_pos, from.length(), to);
		start_pos += to.length(); // Handles case where 'to' is a substring of 'from'
	}
}
void CChatDialog::GetSelectedName(CMouse* mouseInfo)
{
	for(int b=WHOLE; b< SHOUT; b++)
	{
		cListDialog* pCurSheet = m_pSheet[b];
		ITEM* pItem = pCurSheet->GetSelectedITEM(mouseInfo);
		//GetSheet(WHOLE)->AddItem((ITEM*)pItem, (LONG)3);
		if(!pItem)
			return;
   		if( pItem->rgb != dwChatTextColor[CTC_GENERALCHAT] && 
			pItem->rgb != dwChatTextColor[CTC_PARTYCHAT] && 
			pItem->rgb != dwChatTextColor[CTC_GUILDCHAT] && 
			pItem->rgb != dwChatTextColor[CTC_ALLIANCE] && 
			pItem->rgb != dwChatTextColor[CTC_SHOUT] && 
			pItem->rgb != dwChatTextColor[CTC_WHISPER] )
			return;
		int nCpyNum = 0;
		int nLen = strlen(pItem->string);
		int nLenChat = strlen(pItem->string);
		char* pStr = NULL;
		char* pStrChat = NULL;
		if (nLen)
		{
			pStr = pItem->string;
			pStrChat = pItem->string;
		}			
		
		if( *pStr == '[' )
		{
			++pStr;
		}
		else
		{
			return;
		}

		//BOOL bNeedCpy = FALSE;
		BOOL bDevelopid = FALSE;
		BOOL bIsYou = FALSE;
		for( int i = 0; i < nLen; ++i )
		{
			if( pStr[i] == ']' )
			{		
				char tbuf[MAX_NAME_LENGTH + 1] = { 0, };
				SafeStrCpy( tbuf, pStr, nCpyNum+1 );
				//-----------------------------------------------------------------------------------------
				char obj[MAX_CHAT_LENGTH + 1];
				sprintf(obj, "[%s]: ", tbuf);
				//CHATMGR->AddMsg(CTC_SYSMSG, obj);
				std::string str = pStrChat;
				ReplaceAll3(str, std::string(obj), std::string(""));
				//std::string str = ReplaceAll2(std::string(pStrChat), std::string(obj), std::string(""));
				//std::string str = pStrChat;
				char *cstr = new char[str.length() + 1];
				strcpy(cstr, str.c_str());
				// do stuff
				//CHATMGR->AddMsg(CTC_SYSMSG, cstr);

				if (strcmp(tbuf, HERO->GetObjectName()) == 0)
				{
					bIsYou = TRUE;
					GAMEIN->GetCharacterRBTNDlg()->SetRecallMsg(cstr);
				}
				
				GAMEIN->GetCharacterRBTNDlg()->ShowChatMenu(tbuf, bIsYou);
				
				//delete[] cstr;
				//-----------------------------------------------------------------------------------------
				
				break;
			}
			
			
			++nCpyNum;
		}		
		//return;
	}
}