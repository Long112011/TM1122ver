#include "stdafx.h"
#include "GMNotifyManager.h"
#include "Interface/cFont.h"
#include "MainGame.h"
#include "GameResourceManager.h" 
#include "cScriptManager.h"
#include "GuildMarkManager.h"
#include "GuildMark.h"

#include "GameIn.h"
#include "EventNotifyDialog.h"

//#include "../[CC]JACK/HantarManager.h"
GLOBALTON(CGMNotifyManager)
CGMNotifyManager::CGMNotifyManager()
{
	Init( MAX_NOTIFYMSG_LINE );
	SetPosition( NOTIFYMSG_DEFAULT_X, NOTIFYMSG_DEFAULT_Y, NOTIFYMSG_DEFAULT_WIDTH );
	SetLineSpace( NOTIFYMSG_DEFAULT_LINESPACE );
	SetFont( NOTIFYMSG_DEFAULT_FONT );
	memset( m_ApplyEventList, 0, sizeof(BOOL)*eEvent_Max );
	m_pGuildMark=NULL;
	m_pGuildMark2=NULL;
}
CGMNotifyManager::~CGMNotifyManager()
{
	Release();
}
void CGMNotifyManager::NetworkMsgParseGet(BYTE Protocol, void* pMsg)
{
	//HANTARMGR->NetworkMsgParseGet(Protocol, pMsg);
	
}

/*void CGMNotifyManager::NetworkMsgParseGetCheat(BYTE Protocol, void* pMsg)
{
	switch (Protocol)
	{
	case MP_CHEAT_EVENTNOTIFYGET_ON:
		{
			MSG_EVENTNOTIFY_ON* pmsg = (MSG_EVENTNOTIFY_ON*)pMsg;
			NOTIFYMGR->SetEventNotifyGetStr(pmsg->strTitle, pmsg->strContext);
			NOTIFYMGR->SetEventNotify(TRUE);
			if (GAMEIN->GetEventNotifyDialog())
			{
				GAMEIN->GetEventNotifyDialog()->SetTitle(pmsg->strTitle);
				GAMEIN->GetEventNotifyDialog()->SetContext(pmsg->strContext);
				GAMEIN->GetEventNotifyDialog()->Open();
				NOTIFYMGR->SetEventNotifyChanged(FALSE);
			}
			else
			{
				NOTIFYMGR->SetEventNotifyChanged(TRUE);
			}
			
		}
		break;
	case MP_CHEAT_EVENTNOTIFYGET_OFF:
		{
			MSGBASE* pmsg = (MSGBASE*)pMsg;
			NOTIFYMGR->SetEventNotify(FALSE);
			NOTIFYMGR->SetEventNotifyChanged(FALSE);
			if (GAMEIN->GetEventNotifyDialog())
			{
				GAMEIN->GetEventNotifyDialog()->Close();
			}
		}
		break;
	}
}*/
void CGMNotifyManager::Init( int nMaxLine )
{
	m_nMaxLine			= nMaxLine;
	m_bUseEventNotify	= FALSE;
	ZeroMemory( m_strEventNotifyTitle, 32 );
	ZeroMemory( m_strEventNotifyContext, 128 );
	m_bNotifyChanged	= FALSE;
	m_pGuildMark=NULL;
	m_pGuildMark2=NULL;
}
void CGMNotifyManager::Release()
{
	PTRLISTPOS pos = m_ListNotify.GetHeadPosition();
	while(pos)
	{
		sGMNOTIFY* pNotify = (sGMNOTIFY *)m_ListNotify.GetNext(pos);
		SAFE_DELETE(pNotify);
	}
	m_ListNotify.RemoveAll();
}
void CGMNotifyManager::PushMsg( char* pMsg, int nClass)
{
	sGMNOTIFY* pNotify = new sGMNOTIFY;
	pNotify->nStrLen = strlen(pMsg);
	if( pNotify->nStrLen > MAX_NOTIFYMSG_LENGTH )
	{
		strncpy( pNotify->strMsg, pMsg, MAX_NOTIFYMSG_LENGTH );
		pNotify->strMsg[MAX_NOTIFYMSG_LENGTH] = NULL;
		pNotify->nStrLen = MAX_NOTIFYMSG_LENGTH;
	}
	else
	{
		strcpy( pNotify->strMsg, pMsg );
	}
	pNotify->dwReceiveTime	= gCurTime;
	pNotify->dwColor		= dwNotifyColor[nClass];
	m_ListNotify.AddTail( pNotify );
	if( m_ListNotify.GetCount() > m_nMaxLine ) 
		PopMsg();
}
void CGMNotifyManager::PopMsg()
{
	if( m_ListNotify.GetCount() )
	{
		sGMNOTIFY* pNotify = (sGMNOTIFY*)m_ListNotify.GetHead();
		SAFE_DELETE( pNotify );
		m_ListNotify.RemoveHead();
	}
}
void CGMNotifyManager::AddMsg( char* pMsg, int nClass )
{
	int nLen = strlen( pMsg );
	char buf[128];
	int nCpyNum = 0;
	while( nLen > MAX_NOTIFYMSG_PER_LINE )
	{
		if( ( pMsg + MAX_NOTIFYMSG_PER_LINE ) != CharNext( CharPrev( pMsg, pMsg + MAX_NOTIFYMSG_PER_LINE ) ) )
			nCpyNum = MAX_NOTIFYMSG_PER_LINE - 1;
		else
			nCpyNum = MAX_NOTIFYMSG_PER_LINE;
		strncpy( buf, pMsg, nCpyNum );
		buf[nCpyNum] = 0;
		PushMsg( buf, nClass );
		nLen -= nCpyNum;
		pMsg += nCpyNum;
		if( *pMsg == ' ' ) ++pMsg;	
	}
	if( nLen > 0 )
		PushMsg( pMsg, nClass );	
}
void CGMNotifyManager::Render()
{
	if( m_ListNotify.GetCount() == 0 ) return;
	if( MAINGAME->GetCurStateNum() != eGAMESTATE_GAMEIN )
	{
		return;
	}
	sGMNOTIFY* pNotify = (sGMNOTIFY*)m_ListNotify.GetHead();
	if( gCurTime - pNotify->dwReceiveTime > NOTIFYMSG_LAST_TIME )
		PopMsg();
	LONG	lPosY = m_rcPos.top;
	LONG	lPosX;
	LONG	lTextWidth;
	RECT	rect;
	PTRLISTPOS pos = m_ListNotify.GetHeadPosition();
	int fontoffset=1;
	char strBuf2[256];
	while(pos)
	{
		sGMNOTIFY* pNotify = (sGMNOTIFY *)m_ListNotify.GetNext(pos);
		lTextWidth = CFONT_OBJ->GetTextExtentEx( m_nFontIdx, pNotify->strMsg, pNotify->nStrLen );
		lPosX = ((GAMERESRCMNGR->m_GameDesc.dispInfo.dwWidth/2)-lTextWidth/2);
		memset(strBuf2,0,256);
		memcpy(strBuf2,pNotify->strMsg,pNotify->nStrLen);
		RenderFaceImage(lPosX,lPosY,pNotify->strMsg);
		ClearFaceImg(strBuf2, pNotify->nStrLen);
#ifndef _JSZHENG_
		SetRect( &rect, lPosX, lPosY, lPosX + lTextWidth, lPosY + 1 );
		CFONT_OBJ->RenderFont(m_nFontIdx,strBuf2, pNotify->nStrLen, &rect, RGBA_MERGE(RGB_HALF(10,10,10),255));
		OffsetRect( &rect, -1, -1);
		CFONT_OBJ->RenderFont(m_nFontIdx,strBuf2, pNotify->nStrLen, &rect, RGBA_MERGE( pNotify->dwColor, 255 ) );
#else
		SetRect( &rect, lPosX, lPosY,lPosX + lTextWidth, lPosY+1 );
		CFONT_OBJ->RenderNoticeMsg(m_nFontIdx, strBuf2, pNotify->nStrLen, &rect,RGBA_MERGE( pNotify->dwColor, 255 ),RGBA_MERGE( RGB_HALF(28 ,28 ,28), 255 ));	
#endif
		lPosY += m_nLineSpace + 10;
	}
}
void CGMNotifyManager::SetPosition( LONG lx, LONG ly, LONG lWidth )
{
	m_rcPos.left	= lx;
#ifndef _JSZHENG_
	m_rcPos.right	= GAMERESRCMNGR->m_GameDesc.dispInfo.dwWidth-280;
	m_rcPos.top		= GAMERESRCMNGR->m_GameDesc.dispInfo.dwHeight/10;
	m_rcPos.bottom	= m_rcPos.top + 1;
#else
	m_rcPos.right	= GAMERESRCMNGR->m_GameDesc.dispInfo.dwWidth-200;
	m_rcPos.top		= GAMERESRCMNGR->m_GameDesc.dispInfo.dwHeight/8;
	m_rcPos.bottom	= m_rcPos.top;
#endif
}
void CGMNotifyManager::SetEventNotifyStr( char* pStrTitle, char* pStrContext )
{
	SafeStrCpy( m_strEventNotifyTitle, pStrTitle, 32 );
	SafeStrCpy( m_strEventNotifyContext, pStrContext, 128 );
}
void CGMNotifyManager::SetEventNotifyGetStr(char* pStrTitle, char* pStrContext)
{
	SafeStrCpy(m_strEventNotifyGetTitle, pStrTitle, 32);
	SafeStrCpy(m_strEventNotifyGetContext, pStrContext, 128);
}
void CGMNotifyManager::SetEventNotify( BOOL bUse )
{
	m_bUseEventNotify = bUse;
}
void CGMNotifyManager::RenderFaceImage(LONG X,LONG Y,char * StaticText)
{
	std::string Buffer=StaticText;
	if(Buffer.length()>0)
	{
		int FindStartPos=0;
		while(FindStartPos>=0 && FindStartPos< Buffer.length())
		{
			int FindPos= Buffer.find('&',FindStartPos);   
			int FindPos2= Buffer.find('*',FindStartPos);
			//int FindPos3= Buffer.find('$',FindStartPos);

			int FindPos4= Buffer.find('+',FindStartPos);
			int FindPos5= Buffer.find('^',FindStartPos);

			if(FindPos>=0)
			{
				std::basic_string <char> ImgStr = Buffer.substr(FindPos+1,2);
				std::basic_string <char> FlagStr= Buffer.substr(0,FindPos);
				int ImageIdx= atoi(ImgStr.c_str());
				if(ImageIdx>=0 && ImageIdx<56)
				{
					cImage m_Image;
					VECTOR2 p_Scal,p_Pos;
					p_Scal.x=p_Scal.y=1.0f;
					p_Pos.y = Y-2;
					p_Pos.x = X+3.0f+(FlagStr.length())*9;			
					SCRIPTMGR->GetImage(ImageIdx+148 , &m_Image, PFT_HARDPATH);
					m_Image.RenderSprite(&p_Scal,0,0,&p_Pos,0xffffffff);
				}
                FindStartPos = FindPos+2;
			}
			if(FindPos2>=0)
			{
				std::basic_string <char> ImgStr = Buffer.substr(FindPos2+1,2);
				std::basic_string <char> FlagStr= Buffer.substr(0,FindPos2);
				int ImageIdx= atoi(ImgStr.c_str());
				if(ImageIdx!=0)
				{
					m_pGuildMark = GUILDMARKMGR->GetGuildMark( (DWORD)ImageIdx );
					VECTOR2 scaleRate,Pos;	
					scaleRate.x=scaleRate.y=0.8f;
					Pos.x=X+1.0f+(FlagStr.length())*9;
					Pos.y=Y+1.0f;
					m_pGuildMark->Render(&Pos);	
				}
                FindStartPos = FindPos2+1;//2 //kiv
			}
			/*if(FindPos3>=0)
			{
				std::basic_string <char> ImgStr = Buffer.substr(FindPos3+1,2);
				std::basic_string <char> FlagStr= Buffer.substr(0,FindPos3);
				int ImageIdx= atoi(ImgStr.c_str());
				if(ImageIdx==1||ImageIdx==2)
				{
					cImage m_Image;
					VECTOR2 p_Scal,p_Pos;
					p_Scal.x=p_Scal.y=1;
					p_Pos.y = Y+1.0f;
					p_Pos.x=X+1.0f+(FlagStr.length())*9;	
					SCRIPTMGR->GetImage(33+ImageIdx , &m_Image, PFT_JACKPATH);
					m_Image.RenderSprite(&p_Scal,0,0,&p_Pos,0xffffffff);
				}
                FindStartPos = FindPos3+1;//2 //kiv
			}*/

			if(FindPos4>=0)
			{
				std::basic_string <char> ImgStr = Buffer.substr(FindPos4,1);
				std::basic_string <char> FlagStr= Buffer.substr(0,FindPos4);
			//	int ImageIdx= atoi(ImgStr.c_str());
			//if(ImageIdx==1||ImageIdx==2)
				//if(strcmp(ImgStr.c_str(),"+")==0)
				if(*ImgStr.c_str()=='+')
				{
					cImage m_Image;
					VECTOR2 p_Scal,p_Pos;
					p_Scal.x=p_Scal.y=1;
					p_Pos.y = Y+1.0f;
					p_Pos.x=X-1.6f+(FlagStr.length())*9;	
					SCRIPTMGR->GetImage(34 , &m_Image, PFT_JACKPATH);
					m_Image.RenderSprite(&p_Scal,0,0,&p_Pos,RGBA_MERGE(0x00ffffff, 255 ));
				}
                FindStartPos = FindPos4+1;//2 //kiv
			}
			if(FindPos5>=0)
			{
				std::basic_string <char> ImgStr = Buffer.substr(FindPos5,1);
				std::basic_string <char> FlagStr= Buffer.substr(0,FindPos5);
				//int ImageIdx= atoi(ImgStr.c_str());
				//if(ImageIdx==1||ImageIdx==2)
				//if(strcmp(ImgStr.c_str(),"^")==0)
				if(*ImgStr.c_str()=='^')
				{
					cImage m_Image;
					VECTOR2 p_Scal,p_Pos;
					p_Scal.x=p_Scal.y=1;
					p_Pos.y = Y+1.0f;
					p_Pos.x=X-1.6f+(FlagStr.length())*9;	
					SCRIPTMGR->GetImage(35 , &m_Image, PFT_JACKPATH);
					m_Image.RenderSprite(&p_Scal,0,0,&p_Pos,RGBA_MERGE(0x00ffffff, 255 ));
				}
                FindStartPos = FindPos5+1;//2 //kiv
			}
			else
			{
				break;
			}		
		}
	}
}
void CGMNotifyManager::ClearFaceImg(char * StaticText,int StrLen)
{
    std::string Buffer=StaticText;
	if(Buffer.length()>0)
	{
		int FindStartPos=0;
		while(FindStartPos>=0 && FindStartPos< Buffer.length())
		{
			int FindPos= Buffer.find('&',FindStartPos); 
			int FindPos2= Buffer.find('*',FindStartPos);
			//int FindPos3= Buffer.find('$',FindStartPos);

			int FindPos4= Buffer.find('+',FindStartPos);
			int FindPos5= Buffer.find('^',FindStartPos);

			if(FindPos>=0)
			{
				Buffer.replace(FindPos,3,"   ");
				FindStartPos = FindPos+2;
			}
			if(FindPos2>=0)
			{
				Buffer.replace(FindPos2,2,"  ");
				FindStartPos = FindPos2+1;
			}
			/*if(FindPos3>=0)
			{
				Buffer.replace(FindPos3,2,"  ");
				FindStartPos = FindPos3+1;
			}*/
			if(FindPos4>=0)
			{
				Buffer.replace(FindPos4,1," ");
				FindStartPos = FindPos4+1;
			}
			if(FindPos5>=0)
			{
				Buffer.replace(FindPos5,1," ");
				FindStartPos = FindPos5+1;
			}


			else
			{
				break;
			}
		}
	}
	memset(StaticText,0,StrLen);
	memcpy(StaticText,Buffer.c_str(),StrLen);
}