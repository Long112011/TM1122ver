#include "stdafx.h"
#include <string>  
#include "ctype.h" 
#include "cScriptManager.h" 
#include "OBalloonChat.h"
#include "./interface/cFont.h"
#define BALLOON_BORDER		10
COBalloonChat::COBalloonChat()
{
	m_fgColor			= RGB_HALF(70,70,70);
	m_lTall				= 0;
	m_wFontIdx			= 0;
	m_dwStartTime		= 0;
	m_dwDisappearTime	= 0;
	m_bShow				= FALSE;
	m_bActive			= TRUE;
	InitChatMsg();
	m_glistenCount2		= 0;
	m_glisttenTime2		= 0;
}
COBalloonChat::~COBalloonChat()
{
}
void COBalloonChat::InitChatMsg()
{
	for( int i = 0 ; i < MAX_CHATBALLON_TEXT_LINE ; ++i )
	{
		m_szChatMsg[i][0] = 0;
		m_ChatMsgLen[i] = 0;
	}
	m_nLineNum = 0;
}
LONG COBalloonChat::GetBalloonHeight()
{
	return m_nLineNum * 15 + 2 + BALLOON_BORDER * 2;
}
void COBalloonChat::SetDisappearTime(DWORD disappearTime)
{ 
	m_dwDisappearTime	= disappearTime; 
	m_dwStartTime		= gCurTime; 
}
void COBalloonChat::SetChat(char * chatMsg)
{
	if( chatMsg == NULL )	return;
	if( *chatMsg == 0 )		return;
	int nLen = strlen( chatMsg );
	if( nLen > MAX_CHAT_LENGTH ) return;
	int nCpyNum = 0;
	m_nLineNum = 0;
#ifdef _TL_LOCAL_
	BYTE brk[512];
	int nBrkNum = g_TBreak.FindThaiWordBreak( chatMsg, nLen, brk, 512, FTWB_SEPARATESYMBOL );
	int nCutPos = 0;
	int nCount = 0;
	for( int i = 0 ; i < nBrkNum ; ++i )
	{
		++nCount;
		nCutPos += brk[i];
		if( CFONT_OBJ->GetTextExtentEx(0, chatMsg, nCutPos ) > MAX_CHATBALLON_TEXT_EXTENT )
		{
			if( nCount == 1 )	
			{
				for( int k = 1 ; k <= nCutPos ; ++k )
				{
					if( CFONT_OBJ->GetTextExtentEx(0, chatMsg, k ) > MAX_CHATBALLON_TEXT_EXTENT )
					{
						nCutPos = nCutPos - brk[i] + k-1; 
						strncpy( m_szChatMsg[m_nLineNum], chatMsg, nCutPos );	
						m_szChatMsg[m_nLineNum][nCutPos] = 0;
						m_ChatMsgLen[m_nLineNum] = nCutPos;
						chatMsg += nCutPos;
						brk[i] -= nCutPos;
						nCutPos = 0;
						nCount = 0;
						--i;
						break;
					}
				}
			}
			else
			{
				nCutPos -= brk[i];
				strncpy( m_szChatMsg[m_nLineNum], chatMsg, nCutPos );	
				m_szChatMsg[m_nLineNum][nCutPos] = 0;
				m_ChatMsgLen[m_nLineNum] = nCutPos;
				--i;
				chatMsg += nCutPos;
				nCutPos = 0;
				nCount = 0; 
			}
			if( *chatMsg == ' ' )
			{
				++chatMsg;
				++i;
			}
			if( ++m_nLineNum >= MAX_CHATBALLON_TEXT_LINE )
			{
				break;
			}
		}
	}
	if( nCutPos > 0 && m_nLineNum < MAX_CHATBALLON_TEXT_LINE )
	{
		strncpy( m_szChatMsg[m_nLineNum], chatMsg, nCutPos );
		m_szChatMsg[m_nLineNum][nCutPos] = 0;
		m_ChatMsgLen[m_nLineNum] = nCutPos;
		++m_nLineNum;
	}
	int Width = 0;
	int strPixelLen = 0;
	for( int i = 0 ; i < m_nLineNum ; ++i )
	{
		Width = CFONT_OBJ->GetTextExtentEx(m_wFontIdx, m_szChatMsg[i], m_ChatMsgLen[i]);
		if( Width > strPixelLen )
			strPixelLen = Width;
	}
	SetPosX(-strPixelLen/2);
#else
	while( nLen > MAX_CHATBALLON_TEXT_LENGTH )
	{
		if( ( chatMsg + MAX_CHATBALLON_TEXT_LENGTH ) != CharNext( CharPrev( chatMsg, chatMsg + MAX_CHATBALLON_TEXT_LENGTH ) ) )
			nCpyNum = MAX_CHATBALLON_TEXT_LENGTH - 1;
		else
			nCpyNum = MAX_CHATBALLON_TEXT_LENGTH;
		strncpy( m_szChatMsg[m_nLineNum], chatMsg, nCpyNum );
		m_szChatMsg[m_nLineNum][nCpyNum] = 0;
		m_ChatMsgLen[m_nLineNum] = nCpyNum;
		nLen -= nCpyNum;
		chatMsg += nCpyNum;
		if( *chatMsg == ' ' ) ++chatMsg;
		if( ++m_nLineNum >= MAX_CHATBALLON_TEXT_LINE-1 )
		{
			break;
		}
	}
	if( nLen > 0 )
	{
		strncpy( m_szChatMsg[m_nLineNum], chatMsg, MAX_CHATBALLON_TEXT_LENGTH );
		m_szChatMsg[m_nLineNum][MAX_CHATBALLON_TEXT_LENGTH] = 0;
		m_ChatMsgLen[m_nLineNum] = nLen;
		++m_nLineNum;
	}
	int strPixelLen = CFONT_OBJ->GetTextExtentEx(m_wFontIdx, m_szChatMsg[0], m_ChatMsgLen[0]);
	SetPosX(-strPixelLen/2);
#endif
	cBalloonOutline::SetBalloonSize(strPixelLen+8, 30, m_nLineNum);
}
void COBalloonChat::SetActive( BOOL val )
{
	m_bActive	= val;
	if( !val )
		Show( FALSE );
}
void COBalloonChat::Show( BOOL val )
{
	m_bShow		= val;
	if( !val )
	{
		m_dwStartTime = m_dwDisappearTime = 0;
	}
}
BOOL COBalloonChat::Render(LONG absX, LONG absY)
{
	if(!m_bActive) return FALSE;
	if( m_dwStartTime + m_dwDisappearTime > gCurTime )
	{
		m_bShow = TRUE;
	}
	else
	{
		m_dwStartTime = m_dwDisappearTime = 0;
		InitChatMsg();
		m_bShow = FALSE;
	}
	if(!m_bShow) return FALSE;
	if( m_nLineNum == 0 ) return FALSE;
	LONG lLineHeight = 15;
	LONG lMiddleTotalHeight = 2 + lLineHeight * m_nLineNum;
	LONG lPosX = absX + m_lPosX;
	LONG lPosY = absY + m_lTall - lMiddleTotalHeight - 10;	
	cBalloonOutline::RenderOutlineSimple( (float)lPosX, (float)lPosY, (float)absX, lMiddleTotalHeight, TRUE );
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
			CFONT_OBJ->RenderFont(m_wFontIdx, strBuf2, m_ChatMsgLen[i], &rect, RGBA_MERGE( m_fgColor, 255 ) );
		}	
	}
	return TRUE;
}
void COBalloonChat::RenderFaceImage(LONG X,LONG Y,char * ChatMsg)
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
void COBalloonChat::ClearFaceImg(char * StaticText,int StrLen)
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