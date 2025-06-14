#include "stdafx.h"
#include <string>   
#include "ctype.h"  
#include "cScriptManager.h"     
#include "StreetStallTitleTip.h"
#include "./interface/cFont.h"
#define BALLOON_BORDER		10
CStreetStallTitleTip::CStreetStallTitleTip()
{
	m_fgColor	= RGB_HALF(0,200,0);
	m_TitleLen	= 0;
	m_lTall		= 0;
	m_wFontIdx	= 0;
	m_bActive	= TRUE;
	m_bShow		= FALSE;


	m_glistenCount2		= 0;
	m_glisttenTime2		= 0;
}
CStreetStallTitleTip::~CStreetStallTitleTip()
{
}
void CStreetStallTitleTip::SetTitle(char * szTitle)
{
	if( szTitle == NULL )	return;
	if( *szTitle == 0 )		return;
	int nLen = strlen( szTitle );
	if( nLen <= 0 || nLen > MAX_STREETSTALL_TITLELEN ) return;
	int nCpyNum = 0;
	m_nLineNum = 0;
	while( nLen > SS_TITLE_LENGTH )
	{
		if( *szTitle == 0 ) break;
		if( ( szTitle + SS_TITLE_LENGTH ) != CharNext( CharPrev( szTitle, szTitle + SS_TITLE_LENGTH ) ) )
			nCpyNum = SS_TITLE_LENGTH - 1;
		else
			nCpyNum = SS_TITLE_LENGTH;
		strncpy( m_szChatMsg[m_nLineNum], szTitle, nCpyNum );
		m_szChatMsg[m_nLineNum][nCpyNum] = 0;
		m_ChatMsgLen[m_nLineNum] = nCpyNum;
		nLen -= nCpyNum;
		szTitle += nCpyNum;
		if( *szTitle == ' ' ) ++szTitle;	
		if( ++m_nLineNum >= SS_TITLE_LINE-1 ) break;
	}
	if( nLen > 0 && m_nLineNum < SS_TITLE_LINE )
	{
		strncpy( m_szChatMsg[m_nLineNum], szTitle, SS_TITLE_LENGTH );
		m_szChatMsg[m_nLineNum][SS_TITLE_LENGTH] = 0;
		if( nLen > SS_TITLE_LENGTH )
			m_ChatMsgLen[m_nLineNum] = nCpyNum;
		else
			m_ChatMsgLen[m_nLineNum] = nLen;
		++m_nLineNum;
	}
	int strPixelLen = CFONT_OBJ->GetTextExtentEx(m_wFontIdx, m_szChatMsg[0], m_ChatMsgLen[0]);
	SetPosX(-strPixelLen/2);
	cBalloonOutline::SetBalloonSize(strPixelLen+8, 30, m_nLineNum);
}
BOOL CStreetStallTitleTip::Render(LONG absX, LONG absY)
{
	if(!m_bActive) return FALSE;
	if(!m_bShow) return FALSE;
	if( m_nLineNum <= 0 || m_nLineNum > 2 ) return FALSE;
	LONG lLineHeight = 15;
	LONG lMiddleTotalHeight = 2 + lLineHeight * m_nLineNum;
	LONG lPosX = absX + m_lPosX;
	LONG lPosY = absY + m_lTall - lMiddleTotalHeight - 10;	
	cBalloonOutline::RenderOutlineSimple( (float)lPosX, (float)lPosY, (float)absX, lMiddleTotalHeight );
	char strBuf2[256];
	RECT rect;
	for( int i = 0 ; i < m_nLineNum ; ++i )
	{
		if( m_szChatMsg[i][0] != 0 )
		{
			SetRect( &rect, (long)(lPosX + BALLOON_BORDER),
							(long)(lPosY + BALLOON_BORDER + lLineHeight * i ), 1, 1 );
			memset(strBuf2,0,256);
		    memcpy(strBuf2,m_szChatMsg[i],m_ChatMsgLen[i]);
			RenderFaceImage( (long)(lPosX + BALLOON_BORDER),(long)(lPosY + BALLOON_BORDER + lLineHeight * i ),strBuf2);
			ClearFaceImg(strBuf2, m_ChatMsgLen[i]);
			CFONT_OBJ->RenderFont(m_wFontIdx, strBuf2, m_ChatMsgLen[i], &rect, RGBA_MERGE( RGB_HALF(70,70,70), 255 ) );
			//RenderFaceImage( (long)(lPosX + BALLOON_BORDER),(long)(lPosY + BALLOON_BORDER + lLineHeight * i ),m_szChatMsg[i]);
			//ClearFaceImg(strBuf2, m_ChatMsgLen[i]);
			//CFONT_OBJ->RenderFont(m_wFontIdx, m_szChatMsg[i], m_ChatMsgLen[i], &rect, RGBA_MERGE( RGB_HALF(70,70,70), 255 ) );
		}	
	}
	return TRUE;
}
void CStreetStallTitleTip::RenderFaceImage(LONG X,LONG Y,char * ChatMsg)
{
	std::string Buffer=ChatMsg;
	if(Buffer.length()>0)
	{
		int FindStartPos=0;
		while(FindStartPos>=0 && FindStartPos< Buffer.length())
		{
			int FindPos= Buffer.find('&',FindStartPos);  
			if(FindPos>=0)
			{
				std::basic_string <char> ImgStr = Buffer.substr(FindPos+1,2);
				std::basic_string <char> FlagStr= Buffer.substr(0,FindPos);
				int ImageIdx= atoi(ImgStr.c_str());
				if(ImageIdx>=0 && ImageIdx<70)
				{
					if(SCRIPTMGR->GetEmoji(ImageIdx)->ImageCount>1)
					{
						int  Rate=50;
						DWORD Cur_glistenTime= GetTickCount();
						if(Cur_glistenTime-m_glisttenTime2>1)
						{
							if(m_glistenCount2>= SCRIPTMGR->GetEmoji(ImageIdx)->ImageCount*Rate-5)
							{
								m_glistenCount2=0;
							}
							else
							{
								m_glistenCount2=m_glistenCount2+5;
							}
							Cur_glistenTime=0;
						}
						int Index= m_glistenCount2/Rate;  
						SCRIPTMGR->GetImage(SCRIPTMGR->GetEmoji(ImageIdx)->ImageIdx,&pNameImage[Index],&SCRIPTMGR->GetEmoji(ImageIdx)->rect[Index]);
						if(!pNameImage[Index].IsNull())
						{   
							VECTOR2 p_Scal,p_Pos;
							p_Scal.x=p_Scal.y=0.9f;
							p_Pos.y = Y-2.5f;
							p_Pos.x = X+1.0f+(FlagStr.length())*6;
							pNameImage[Index].RenderSprite(0,NULL,0,&p_Pos,0xffffffff);
							m_glisttenTime2=GetTickCount();
						}
					}
					else
					{
						if(SCRIPTMGR->GetEmoji(ImageIdx))
						{
							int Index= 0; 
							SCRIPTMGR->GetImage(SCRIPTMGR->GetEmoji(ImageIdx)->ImageIdx,&pNameImage[Index],&SCRIPTMGR->GetEmoji(ImageIdx)->rect[Index]);
							if(!pNameImage[Index].IsNull())
							{							
								VECTOR2 p_Scal,p_Pos;
								p_Scal.x=p_Scal.y=0.9f;
								p_Pos.y = Y-2.5f;
								p_Pos.x = X+1.0f+(FlagStr.length())*6;
								pNameImage[Index].RenderSprite(0,NULL,0,&p_Pos,0xffffffff);
							}
						}
					}
				}
                FindStartPos = FindPos+2;
			}
			else
			{
				break;
			}
		}
	}
}
void CStreetStallTitleTip::ClearFaceImg(char * StaticText,int StrLen)
{
    std::string Buffer=StaticText;
	if(Buffer.length()>0)
	{
		int FindStartPos=0;
		while(FindStartPos>=0 && FindStartPos< Buffer.length())
		{
			int FindPos= Buffer.find('&',FindStartPos);
			if(FindPos>=0)
			{
				Buffer.replace(FindPos,3,"   ");
				FindStartPos = FindPos+2;
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