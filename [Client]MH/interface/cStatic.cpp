







// cStatic.cpp: implementation of the cStatic class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"

#include "cStatic.h"
#include "cFont.h"
#include "cWindowFunc.h"
#include "MHFile.h"


#include "cScriptManager.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
#include "./Input/Mouse.h"
#include "cWindowManager.h"
#include "./Input/Keyboard.h"
#include "./Input/UserInput.h"
extern BOOL m_DebugEnabled;
extern char* WINIDSEARCH(int b);

cStatic::cStatic()
{
	m_type = WT_STATIC;
	m_fMultiLine = FALSE;
	*m_szStaticText = 0;
	m_textXY.x = 0;
	m_textXY.y = 0;
	m_fgColor = RGB_HALF(255,255,255);
	m_bShadow = FALSE;
	m_gFontOutline = FALSE;
	m_shadowTextXY.x = 1;
	m_shadowTextXY.y = 1;
	m_shadowColor = RGB_HALF(10,10,10);

	m_bFaceImage		= FALSE;
	m_glistenCount		= 150;      
	m_glisttenTime		= 0;

	m_bFlicker2		= FALSE;
	m_bFlActive2	= FALSE;
	m_dwFlickerSwapTime2	= 0;
	count2=0;
	SetFlickerRoar(FALSE);
	m_glistenCount2		= 0;
	m_glisttenTime2		= 0;


	m_gIsMarquee = false;
	m_gMarqueeSize = 0;

	m_gMarqueeSpeed = 0;


	*m_szTextDebug = 0;
	m_SavedXY.x = 0;
	m_SavedXY.y = 0;
	m_SavedID = 0;
	m_DebugBoolean[0] = FALSE;
	m_DebugBoolean[1] = FALSE;
	m_DebugBoolean[2] = FALSE;
	m_DebugBoolean[3] = FALSE;
	m_SavedWidthHeight[0] = 0;
	m_SavedWidthHeight[1] = 0;
}

cStatic::~cStatic()
{
	m_StaticText.Release();
}
void cStatic::Init(LONG x, LONG y, WORD wid, WORD hei, cImage * basicImage,LONG ID)
{
	cWindow::Init(x, y, wid, hei, basicImage, ID);
	m_type = WT_STATIC;
	//m_CheckBoxImage = *checkBoxImage;
	//m_CheckImage = *checkImage;
	//if (Func != NULL)
	//	cbWindowFunc = Func;

	//if (m_DebugEnabled)
	{
		m_SavedXY.x = x;
		m_SavedXY.y = y;
		m_SavedID = ID;
		m_SavedWidthHeight[0] = wid;
		m_SavedWidthHeight[1] = hei;
		strncpy(m_szTextDebug, Vector2Text(m_SavedXY.x, m_SavedXY.y,  m_SavedWidthHeight[0], m_SavedWidthHeight[1]), strlen(Vector2Text(m_SavedXY.x, m_SavedXY.y,  m_SavedWidthHeight[0], m_SavedWidthHeight[1])) + 1);
	}
}
DWORD cStatic::ActionEvent(CMouse * mouseInfo)
{

	DWORD we = WE_NULL;
	if( !m_bActive ) return we;
	we = cWindow::ActionEvent(mouseInfo);
	if (m_bDisable)
	{
		return we;
	}
	if (m_DebugEnabled)
	{
		if (we & (WE_LBTNCLICK | WE_LBTNDBLCLICK))
		{
			m_DebugBoolean[DebugIsStillDown] = TRUE;			
		}		
		if (mouseInfo->LButtonUp())
		{			
			m_DebugBoolean[DebugIsStillDown] = FALSE;
		}
		if (mouseInfo->LButtonPressed())
		{
			
			if (m_DebugBoolean[DebugClicked])
			if (m_DebugBoolean[DebugIsStillDown])
			if (!WINDOWMGR->IsDragWindow())
			{			
				SetAbsXY(m_absPos.x - MOUSE->GetMouseAxisX(), m_absPos.y - MOUSE->GetMouseAxisY());
				m_SavedXY.x = m_SavedXY.x - MOUSE->GetMouseAxisX();
				m_SavedXY.y = m_SavedXY.y - MOUSE->GetMouseAxisY();
				SetRelXY(m_SavedXY.x, m_SavedXY.y);
				strncpy(m_szTextDebug, Vector2Text(m_SavedXY.x, m_SavedXY.y,  m_SavedWidthHeight[0], m_SavedWidthHeight[1]), strlen(Vector2Text(m_SavedXY.x, m_SavedXY.y,  m_SavedWidthHeight[0], m_SavedWidthHeight[1])) + 1);				
			}
		}
		if (we & WE_MOUSEOVER)
		{
			if (g_UserInput.GetKeyboard()->GetKeyPressed(KEY_CONTROL) && mouseInfo->LButtonDown())
			{
				if (m_DebugBoolean[DebugClicked])
				{
					m_DebugBoolean[DebugClicked] = FALSE;
					return we;
				}
				else
				{
					m_DebugBoolean[DebugClicked] = TRUE;
				}
			}
		}
		else
		{
			m_DebugBoolean[DebugHover] = FALSE;
		}
		if (m_DebugBoolean[DebugClicked])
		{
			return we;
		}
	}
	//if(m_upBtn->ActionEvent(mouseInfo) & WE_BTNCLICK)
	//	(*cbWindowFunc)(m_ID, m_pParent, WE_SPINBTNUP);
	//if(m_downBtn->ActionEvent(mouseInfo) & WE_BTNCLICK)
	//
		//(*cbWindowFunc)(m_ID, m_pParent, WE_SPINBTNDOWN);
	
	return we;
}
void cStatic::InitMultiLine()
{
	if(m_fMultiLine)
	{
		m_StaticText.Init(m_wFontIdx, m_fgColor, NULL);
	}
}

void cStatic::SetFontIdx(WORD fontIdx)
{ 
	m_wFontIdx = fontIdx;
	m_StaticText.SetFontIdx(fontIdx);
}

void cStatic::Render()
{
	cWindow::Render();

	if (m_DebugEnabled)
	if (m_DebugBoolean[DebugHover] || m_DebugBoolean[DebugClicked])
	{
		DWORD inLine = RGBA_MERGE(RGB_HALF(0, 0, 255), 255);
		VECTOR2	TopLeft, TopRight, BottomLeft, BottomRight;
		VECTOR2	BottomLeft2, BottomRight2;
		GetVec2(m_absPos, m_width, m_height, TopLeft, TopRight, BottomLeft, BottomRight/*, BottomLeft2, BottomRight2*/);
		if (m_DebugBoolean[DebugHover])
		{
			g_pExecutive->GetRenderer()->RenderLine(&TopLeft, &TopRight, inLine);
			g_pExecutive->GetRenderer()->RenderLine(&TopRight, &BottomRight, inLine);
			g_pExecutive->GetRenderer()->RenderLine(&BottomRight2, &BottomLeft2, inLine);
			g_pExecutive->GetRenderer()->RenderLine(&BottomLeft, &TopLeft, inLine);
		}
		if (m_DebugBoolean[DebugClicked])
		{
			inLine = RGBA_MERGE(RGB_HALF(0, 0, 255), 255);
			g_pExecutive->GetRenderer()->RenderLine(&TopLeft, &TopRight, inLine);
			g_pExecutive->GetRenderer()->RenderLine(&TopRight, &BottomRight, inLine);
			g_pExecutive->GetRenderer()->RenderLine(&BottomRight2, &BottomLeft2, inLine);
			g_pExecutive->GetRenderer()->RenderLine(&BottomLeft, &TopLeft, inLine);
			if (*m_szTextDebug != 0)
			{
				LONG tempX = m_absPos.x - 11;
				LONG tempY = m_absPos.y - 41;
				WORD nameFontID = 1;
				WORD nameFontPOS = 0;
			
				char IDtemp[64] = { 0 };
				char IDtemp2[64] = { 0 };

				RECT rectDebugID = { (LONG)tempX, (LONG)tempY, 1, 1 };

				RECT rectDebug = { (LONG)tempX, (LONG)tempY + CFONT_OBJ->GetTextHeight(nameFontID), 1, 1 };

				sprintf(IDtemp2, "%s", (char*)WINIDSEARCH(m_SavedID));
				
				CFONT_OBJ->RenderFontWithShadow(nameFontID, IDtemp2, strlen(IDtemp2), &rectDebugID, inLine);

				inLine = RGBA_MERGE(RGB_HALF(255, 0, 0), 255);
				
				CFONT_OBJ->RenderFontWithShadow(nameFontPOS, m_szTextDebug, strlen(m_szTextDebug), &rectDebug, inLine);
			}
			if (m_DebugBoolean[DebugKDBActivated])
			{
				if (gCurTime - m_DebugTimerKDBActivated > 500)
				{
					if (g_UserInput.GetKeyboard()->GetKeyPressed(KEY_LEFT))
					{
						SetAbsXY(m_absPos.x - 1, m_absPos.y);
						m_SavedXY.x = m_SavedXY.x - 1;
						strncpy(m_szTextDebug, Vector2Text(m_SavedXY.x, m_SavedXY.y,  m_SavedWidthHeight[0], m_SavedWidthHeight[1]), strlen(Vector2Text(m_SavedXY.x, m_SavedXY.y,  m_SavedWidthHeight[0], m_SavedWidthHeight[1])) + 1);
					}
					if (g_UserInput.GetKeyboard()->GetKeyPressed(KEY_RIGHT))
					{
						SetAbsXY(m_absPos.x + 1, m_absPos.y);
						m_SavedXY.x = m_SavedXY.x + 1;
						strncpy(m_szTextDebug, Vector2Text(m_SavedXY.x, m_SavedXY.y,  m_SavedWidthHeight[0], m_SavedWidthHeight[1]), strlen(Vector2Text(m_SavedXY.x, m_SavedXY.y,  m_SavedWidthHeight[0], m_SavedWidthHeight[1])) + 1);
					}
					if (g_UserInput.GetKeyboard()->GetKeyPressed(KEY_UP))
					{
						SetAbsXY(m_absPos.x, m_absPos.y - 1);
						m_SavedXY.y = m_SavedXY.y - 1;
						strncpy(m_szTextDebug, Vector2Text(m_SavedXY.x, m_SavedXY.y,  m_SavedWidthHeight[0], m_SavedWidthHeight[1]), strlen(Vector2Text(m_SavedXY.x, m_SavedXY.y,  m_SavedWidthHeight[0], m_SavedWidthHeight[1])) + 1);
					}
					if (g_UserInput.GetKeyboard()->GetKeyPressed(KEY_DOWN))
					{
						SetAbsXY(m_absPos.x, m_absPos.y + 1);
						m_SavedXY.y = m_SavedXY.y + 1;
						strncpy(m_szTextDebug, Vector2Text(m_SavedXY.x, m_SavedXY.y,  m_SavedWidthHeight[0], m_SavedWidthHeight[1]), strlen(Vector2Text(m_SavedXY.x, m_SavedXY.y,  m_SavedWidthHeight[0], m_SavedWidthHeight[1])) + 1);
					}
					SetRelXY(m_SavedXY.x, m_SavedXY.y);
				}
			}
		}
	}

	FlickerRoar();

	char strBuf2[256];

	if(!m_fMultiLine)
	{	
		if(*m_szStaticText != 0)
		{
			LONG TxtPosX;

			if( m_nAlign & TXT_LEFT )
			{
				TxtPosX = (LONG)m_absPos.x + m_textXY.x;
			}
			else if( m_nAlign & TXT_RIGHT )
			{
				TxtPosX = (LONG)m_absPos.x + m_width - m_textXY.x  - CFONT_OBJ->GetTextExtentEx( m_wFontIdx, m_szStaticText, strlen(m_szStaticText) );
			}
			else //TXT_CENTER
			{
				TxtPosX = (LONG)m_absPos.x + ( m_width - CFONT_OBJ->GetTextExtentEx( m_wFontIdx, m_szStaticText, strlen(m_szStaticText) ) ) / 2;					     
			}

			RECT rect={ TxtPosX, (LONG)m_absPos.y + m_textXY.y, 1, 1 };

			if (m_bShadow && !m_gFontOutline)
			{
				RECT rcShadow = rect;
				OffsetRect( &rcShadow, m_shadowTextXY.x, m_shadowTextXY.y );
				CFONT_OBJ->RenderFont(m_wFontIdx,m_szStaticText,lstrlen(m_szStaticText),&rcShadow,RGBA_MERGE(m_shadowColor, m_alpha * m_dwOptionAlpha / 100));				
			}
			if (m_gIsMarquee)
			{
				static int TempTime;
				if (gCurTime - TempTime > m_gMarqueeSpeed)
				{
					TempTime = gCurTime;
					std::string test = m_szStaticText;
					strcpy(m_szStaticText, (char *)marquee_limit(test, m_gMarqueeSize).c_str());
				}
			}

			if (!m_gFontOutline)
				CFONT_OBJ->RenderFont(m_wFontIdx, m_szStaticText, lstrlen(m_szStaticText), &rect, RGBA_MERGE(m_fgColor, m_alpha * m_dwOptionAlpha / 100));
			else
				CFONT_OBJ->RenderNoticeMsg(m_wFontIdx, m_szStaticText, lstrlen(m_szStaticText), &rect, RGBA_MERGE(m_fgColor, m_alpha * m_dwOptionAlpha / 100), RGBA_MERGE(m_shadowColor, m_alpha * m_dwOptionAlpha / 100));
			
				
			//if (m_bFaceImage)//KIV
			{				
				memset(strBuf2,0,256);
				memcpy(strBuf2,m_szStaticText,strlen(m_szStaticText));
				RenderFaceImage(TxtPosX,(LONG)m_absPos.y + m_textXY.y,m_szStaticText);
				ClearFaceImg(strBuf2,strlen(m_szStaticText));
			}			
		}
	}
	else
	{
		if(m_StaticText.IsValid())
		{
			if(CFONT_OBJ->IsCreateFont(m_wFontIdx))
			{
				m_StaticText.SetXY((LONG)m_absPos.x+m_textXY.x, (LONG)m_absPos.y+m_textXY.y);
				m_StaticText.Render();
			}
		}
	}
}

void cStatic::SetStaticText(char * text)
{
	if(!m_fMultiLine)
	{
		char buf[256]={0,};
		char * aa = buf;

		_parsingKeywordString(text, &aa);
		strcpy(m_szStaticText, buf); 
	}
	else
	{
		m_StaticText = text;
	}
}
void cStatic::SetStaticText(char * text, bool IsMove,int mSize)
{
	if (!m_fMultiLine)
	{
		char buf[256] = { 0, };
		char * aa = buf;

		_parsingKeywordString(text, &aa);
		strcpy(m_szStaticText, buf); 

		m_gIsMarquee = IsMove;
		m_gMarqueeSize = mSize;
	}
	else
	{
		m_StaticText = text;
	}
}
std::string cStatic::marquee_limit(std::string& text, size_t limit)
{
	std::string temp = text;
	text.erase(0, 1);
	text += temp[0];
	return text.substr(0, limit);
}
void cStatic::RenderFaceImage(LONG X,LONG Y,char * StaticText)
{
#ifndef _JSKEN2_
	std::string Buffer=StaticText;
	if(Buffer.length()>0)
	{
		int FindStartPos=0;
		while(FindStartPos>=0 && FindStartPos< Buffer.length())
		{
			int FindPos= Buffer.find('¡ú',FindStartPos);
			//int FindPos2= Buffer.find('|',FindStartPos);
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
						if(Cur_glistenTime-m_glisttenTime2>0)
						{
							if(m_glistenCount2>= SCRIPTMGR->GetEmoji(ImageIdx)->ImageCount*Rate-5)
							{
								m_glistenCount2=0;
							}
							else
							{
								m_glistenCount2=m_glistenCount2+5;
								m_glisttenTime2=GetTickCount();
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
				if(ImageIdx==-1)
				{
					cImage m_IEmoji;
					SCRIPTMGR->GetImage(29 , &m_IEmoji, PFT_JACKPATH);
					VECTOR2 p_Scal,p_Pos;
					p_Scal.x=p_Scal.y=1;
					p_Pos.y = Y-2.f;
					p_Pos.x = X-5.5f+(FlagStr.length())*CFONT_OBJ->GetTextWidth(m_wFontIdx);
					m_IEmoji.RenderSprite(0,NULL,0,&p_Pos,0xffffffff);
				}
				if(ImageIdx==-2)
				{
					cImage m_IEmoji;
					SCRIPTMGR->GetImage(30 , &m_IEmoji, PFT_JACKPATH);
					VECTOR2 p_Scal,p_Pos;
					p_Scal.x=p_Scal.y=1;
					p_Pos.y = Y-2.f;
					p_Pos.x = X-5.5f+(FlagStr.length())*CFONT_OBJ->GetTextWidth(m_wFontIdx);
					m_IEmoji.RenderSprite(0,NULL,0,&p_Pos,0xffffffff);
				}
				if(ImageIdx==-3)
				{
					cImage m_IEmoji;
					SCRIPTMGR->GetImage(31 , &m_IEmoji, PFT_JACKPATH);
					VECTOR2 p_Scal,p_Pos;
					p_Scal.x=p_Scal.y=1;
					p_Pos.y = Y-2.f;
					p_Pos.x = X-5.5f+(FlagStr.length())*CFONT_OBJ->GetTextWidth(m_wFontIdx);
					m_IEmoji.RenderSprite(0,NULL,0,&p_Pos,0xffffffff);
				}
                FindStartPos = FindPos+2;
			}
			else
			{
				break;
			}
		}
	}
#endif
}
void cStatic::SetRenderFaceImg(BOOL val)
{
	m_bFaceImage=val; 
}
void cStatic::ClearFaceImg(char * StaticText,int StrLen)
{
#ifndef _JSKEN2_
    std::string Buffer=StaticText;
	if(Buffer.length()>0)
	{
		int FindStartPos=0;
		while(FindStartPos>=0 && FindStartPos< Buffer.length())
		{
			int FindPos= Buffer.find('¡ú',FindStartPos);   
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
#endif
}
void cStatic::SetFlickerRoar( BOOL bFlicker )
{
	m_bFlicker2 = bFlicker;
	m_dwFlickerSwapTime2 = gCurTime;
	m_glisttenTime = gCurTime;
	count2=0;
}
DWORD  g_GlistenColor5[2] ={0XCC3333,0xFF3333};
void cStatic::FlickerRoar()
{
	if(!m_bFlicker2)	return;
	if( gCurTime - m_dwFlickerSwapTime2 > 1 )
	{	
		if(m_glistenCount>=255)
		{
			m_glistenCount=180;
			if(count2>=400)
			{
				SetFlickerRoar( FALSE );
				m_fgColor = g_GlistenColor5[0];
			}
		}
		else
		{
			count2=count2+1;
			m_fgColor = RGBA_MERGE(RGB_HALF(m_glistenCount,0,0),255);
			m_glistenCount=m_glistenCount+6;
		}
		m_bFlActive2 = !m_bFlActive2;
		m_dwFlickerSwapTime2 = gCurTime;
	}
}