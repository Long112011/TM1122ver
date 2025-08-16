







// ObjectBalloon.cpp: implementation of the CObjectBalloon class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ObjectBalloon.h"
#include "OBalloonName.h"
#include "OBalloonChat.h"
#include "StreetStallTitleTip.h"
#include "MunpaMark.h"
#include "Object.h"
#include "GameResourceManager.h"
#include "./Interface/cWindowManager.h"
#include "MunpaMarkManager.h"
#include "GuildMarkManager.h"
#include "GuildMark.h"
#include "Player.h"
#include "ChatManager.h"
#include "GuildUnionMarkMgr.h"
#include "./Interface/cScriptManager.h"
#include "Npc.h"
#include "./Interface/cFont.h"
#include "ObjectManager.h"
#include "BillBoardManager.h"

extern int g_nServerSetNum;

WORD NPCIMAGE[] = {
	24, 16, 14, 15, 17, 19, 24, 24, 20, 24,
	18, 33, 23, 21, 23, 22, 24, 24, 24, 24,
	24, 24, 24, 24, 24, 24, 24, 24, 24, 24,
	24, 24, 24, 24, 24, 24, 24, 24, 24, 24,
	24, 24, 24
};
CObjectBalloon::CObjectBalloon()
{
	m_pName			= NULL;
	m_pChat			= NULL;
	m_pSSTitle		= NULL;
	m_pSBSTitle		= NULL;
	m_pObject		= NULL;
	m_pGuildMark	= NULL;
	m_pNpcMark = NULL;
	//InitTopListFlg();
}
CObjectBalloon::~CObjectBalloon()
{
	Release();
}
extern BOOL bSelect;
extern WORD mapNum;
void CObjectBalloon::InitBalloon(CObject * pParent, LONG tall, BYTE flag)
{
	cWindow::Init(0,0,0,0,NULL,pParent->GetID());
	m_pObject = pParent;
	if( pParent->GetObjectKind() == eObjectKind_Player )
	{
		tall = (LONG)(tall * ((CPlayer*)pParent)->GetCharacterTotalInfo()->Height);
		sBOARDMGR->SetHeroTopListFlg((CPlayer*)pParent);
	}
	if (pParent->GetObjectKind() == eObjectKind_Npc)
	{
		if (((CNpc*)pParent)->GetNpcKind() == 31 )
		{
			m_pNpcMark = new cImage;
			//SCRIPTMGR->GetImage(NPCIMAGE[((CNpc*)pParent)->GetNpcKind()], m_pNpcMark, PFT_MINIMAPPATH);
			SCRIPTMGR->GetImage(146, m_pNpcMark, PFT_JACKPATH);
		}
	}

	m_lTall = tall;
	if(flag & ObjectBalloon_Name)
	{
		ASSERT(!m_pName);
		m_pName = new COBalloonName;
		if(m_pObject->GetObjectKind() == eObjectKind_Player)
			m_pName->SetFontIndex(FONT5);
		else
			m_pName->SetFontIndex(FONT5);//FONT0	
	}
	if(flag & ObjectBalloon_Chat)
	{
		ASSERT(!m_pChat);
		m_pChat = new COBalloonChat;
	}
	if(flag & ObjectBalloon_Title)
	{
		ASSERT(!m_pSSTitle);
		ASSERT(!m_pSBSTitle);
		m_pSSTitle = new CStreetStallTitleTip;
		m_pSBSTitle = new CStreetStallTitleTip;
	}
	m_bActive	= TRUE;
	WINDOWMGR->AddChatTooltip( this );

	
}
#include "FortWarManager.h"
void CObjectBalloon::Release()
{
	WINDOWMGR->RemoveChatTooltipWindow( this );
	SAFE_DELETE(m_pName);
	SAFE_DELETE(m_pChat);
	SAFE_DELETE(m_pSSTitle);
	SAFE_DELETE(m_pSBSTitle);
	m_pObject		= NULL;
	m_pGuildMark	= NULL;
}
void CObjectBalloon::Render()
{
	if( !m_pObject )									return;
	if( !m_pObject->IsInited() )						return;
	if( !m_pObject->GetEngineObject() )					return;
	if( !m_pObject->GetEngineObject()->IsVisible() )	return;
	if( !m_bActive )									return;
#ifndef _GMTOOL_
	if( m_pObject->GetObjectKind() == eObjectKind_Player )
	{
		if(((CPlayer*)m_pObject)->GetCharacterTotalInfo()->bVisible == FALSE)
			return;
	}
#endif
	BOOL bRender = FALSE;
	LONG lTall = m_lTall + 20;	
	if( m_pName )
	{
		if( m_pName->IsShow() )
			bRender = TRUE;
	}
	if( m_pChat )
	{
		if( m_pChat->IsShow() )
			bRender = TRUE;
	}
	if( m_pSSTitle )						   
	{
		if( m_pSSTitle->IsShow() )
		{
			bRender = TRUE;
			lTall	= m_lTall - 50;
		}
	}
	if( m_pSBSTitle )							
	{
		if( m_pSBSTitle->IsShow() )
		{
			bRender = TRUE;
			lTall	= m_lTall - 50;
		}
	}
	if( m_pObject->GetState() == eObjectState_Ungijosik )
		lTall = m_lTall - 50;
	else if( m_pObject->GetState() == eObjectState_Die )
		lTall = m_lTall - 100;
	if( !bRender ) return;
	VECTOR3 OutPos3, inPos3;
	m_pObject->GetPosition(&inPos3);
	
	

	inPos3.y += lTall;

	GetScreenXYFromXYZ(g_pExecutive->GetGeometry(), 0, &inPos3, &OutPos3);
	if(OutPos3.x < 0 || OutPos3.x > 1 || OutPos3.y < 0 || OutPos3.y > 1) return;
		
	m_absPos.x = (GAMERESRCMNGR->m_GameDesc.dispInfo.dwWidth*OutPos3.x) + 0.25f;
	m_absPos.y = (GAMERESRCMNGR->m_GameDesc.dispInfo.dwHeight*OutPos3.y) -0.75f;

	/*{//-------------------------------dancing float name
		float fMultipleTick = 0.001f;
		float fChangeDist = sqrt(pow(m_absPos.x - m_OldAbsPos.x, 2) + pow(m_absPos.y - m_OldAbsPos.y, 2));
		float fTick = gTickTime * fMultipleTick * fChangeDist;

		if (fTick > 1 || fChangeDist > 1000)
			fTick = 1;

		m_OldAbsPos.x = m_OldAbsPos.x + (m_absPos.x - m_OldAbsPos.x) * fTick;
		m_OldAbsPos.y = m_OldAbsPos.y + (m_absPos.y - m_OldAbsPos.y) * fTick;
		m_absPos = m_OldAbsPos;
	}*/

	if( m_pSSTitle )
	{
		if( m_pSSTitle->Render((long)m_absPos.x, (long)(m_absPos.y) ) )
		{			
			m_absPos.y -= 25.0f + 15.0f*m_pSSTitle->GetLineCount();
		}
	}
	if( m_pSBSTitle )
	{
		if( m_pSBSTitle->Render((long)m_absPos.x, (long)(m_absPos.y) ) )
		{			
			m_absPos.y -= 25.0f + 15.0f*m_pSBSTitle->GetLineCount();
		}
	}
	if( m_pChat )
	{
		if( m_pChat->Render((long)m_absPos.x,(long)(m_absPos.y) ) )
			m_absPos.y -= m_pChat->GetBalloonHeight();
	}
	if( m_pName )
	{
		if (FORTWARMGR->IsWarStart())
		{
			if (m_pName->RenderFort((long)m_absPos.x, (long)m_absPos.y, m_pObject->GetObjectKind()))
			{
				if (m_pObject->GetObjectKind() == eObjectKind_Npc && ((CNpc*)m_pObject)->GetNpcUniqueIdx() == mapNum)
				{
					LONG nposx = m_pName->GetNickPosX() >= m_pName->GetPosX() ? m_pName->GetPosX() : m_pName->GetNickPosX();
					if (m_pNpcMark && bSelect)
					{
						VECTOR2 pos2;
						pos2.x = m_absPos.x + nposx - 30.0f;
						pos2.y = m_absPos.y + m_pName->GetTall() - 10.0f;

						VECTOR2 scale;
						scale.x = 1.0f;
						scale.y = 1.0f;

						m_pNpcMark->RenderSprite(&scale, NULL, 0, &pos2, 0xffffffff);
					}
				}
			}
		}
		else
		{
			if (m_pName->Render((long)m_absPos.x, (long)m_absPos.y))
			{
				if (m_pObject->GetObjectKind() == eObjectKind_Npc && ((CNpc*)m_pObject)->GetNpcUniqueIdx() == mapNum)
				{
					LONG nposx = m_pName->GetNickPosX() >= m_pName->GetPosX() ? m_pName->GetPosX() : m_pName->GetNickPosX();
					if (m_pNpcMark && bSelect)
					{
						VECTOR2 pos2;
						pos2.x = m_absPos.x + nposx - 30.0f;
						pos2.y = m_absPos.y + m_pName->GetTall() - 10.0f;

						VECTOR2 scale;
						scale.x = 1.0f;
						scale.y = 1.0f;

						m_pNpcMark->RenderSprite(&scale, NULL, 0, &pos2, 0xffffffff);
					}
				}
				else if (m_pObject->GetObjectKind() == eObjectKind_Player)
				{
					DWORD NameItemIdx = strlen(((CPlayer*)m_pObject)->GetMarryName()) * 2;
					LONG nposx = m_pName->GetNickPosX() >= m_pName->GetPosX() ? m_pName->GetPosX() : m_pName->GetNickPosX();
					if (((CPlayer*)m_pObject)->GetGuildMarkName() != 0)
					{//帮会图标位置渲染
						if (m_pGuildMark)
						{
							VECTOR2 pos2;
							pos2.x = m_absPos.x + nposx - 18.0f;//- 40.0f;
							//						pos2.x = m_absPos.x + m_pName->GetPosX() - 18.0f;//- 40.0f;
							// 
							if (((CPlayer*)m_pObject)->GetVipLevel() > 0)
								pos2.x -= 20.f;

							pos2.y = m_absPos.y + m_pName->GetTall() - 22.0f;//- 2.0f;
							//						pos2.y = m_absPos.y + m_pName->GetTall() - 2.0f;//- 5.0f;

							m_pGuildMark->Render(&pos2);
						}
					}
					DWORD dwGuildUnionIdx = ((CPlayer*)m_pObject)->GetGuildUnionIdx();
					DWORD dwGuildUnionMarkIdx = ((CPlayer*)m_pObject)->GetGuildUnionMarkIdx();
					if (dwGuildUnionIdx != 0 && dwGuildUnionMarkIdx != 0)
					{//同盟图标位置渲染
						CGuildUnionMark* pMark = GUILDUNIONMARKMGR->GetGuildUnionMark(g_nServerSetNum, dwGuildUnionIdx, dwGuildUnionMarkIdx);
						if (pMark)
						{
						VECTOR2 pos2;
						pos2.x = m_absPos.x + nposx - 18.0f;

						if (((CPlayer*)m_pObject)->GetVipLevel() > 0)
							pos2.x -= 20.f;

//						pos2.x = m_absPos.x + m_pName->GetPosX() - 18.0f;
						pos2.y = m_absPos.y + m_pName->GetTall() - 38.0f;	
//						pos2.y = m_absPos.y + m_pName->GetTall() - 18.0f;	
						pMark->Render( &pos2 );
						}
					}
				}

			}
		}
	}
}
void CObjectBalloon::SetOverInfoOption( DWORD dwOption )
{
	if( dwOption )	
	{
		if( m_pName )		m_pName->SetActive( TRUE );
		if( m_pChat )		m_pChat->SetActive( TRUE );
		if( m_pSSTitle )	m_pSSTitle->SetActive( TRUE );
		if( m_pSBSTitle )	m_pSBSTitle->SetActive( TRUE );
	}
	else
	{
		if( m_pName )		m_pName->SetActive( FALSE );
		if( m_pChat )		m_pChat->SetActive( FALSE );
		if( m_pSSTitle )	m_pSSTitle->SetActive( FALSE );
		if( m_pSBSTitle )	m_pSBSTitle->SetActive( FALSE );
	}
}
void CObjectBalloon::ShowObjectName( BOOL bShow, DWORD dwColor )
{
	if( !m_pName ) return;
	m_pName->SetFGColor( dwColor );
	m_pName->Show( bShow );
}
void CObjectBalloon::ShowChatBalloon( BOOL bShow, char* chatMsg, DWORD dwColor, DWORD dwAliveTime )
{
	if( !m_pChat ) return;
	m_pChat->SetFGColor( dwColor );
	m_pChat->Show( bShow );
	if( bShow )
	{
		m_pChat->SetChat( chatMsg );
		m_pChat->SetDisappearTime( dwAliveTime );
	}
}
void CObjectBalloon::ShowStreeStallTitle( BOOL bShow, char* strTitle, DWORD dwColor )
{
	if( !m_pSSTitle ) return;
	m_pSSTitle->SetFGColor( dwColor );
	m_pSSTitle->Show( bShow );
	if( bShow )
	{
		if( strTitle )
			m_pSSTitle->SetTitle( strTitle );
		else
			m_pSSTitle->SetTitle( CHATMGR->GetChatMsg(366) );
	}
}
void CObjectBalloon::ShowStreeBuyStallTitle( BOOL bShow, char* strTitle, DWORD dwColor )
{
	if( !m_pSBSTitle ) return;
	m_pSBSTitle->SetFGColor( dwColor );
	m_pSBSTitle->Show( bShow );
	if( bShow )
	{
		if( strTitle )
			m_pSBSTitle->SetTitle( strTitle );
		else
			m_pSBSTitle->SetTitle( CHATMGR->GetChatMsg(366) );
	}
}
BOOL CObjectBalloon::SetGuildMark( DWORD GuildIdx )
{
	m_pGuildMark = GUILDMARKMGR->GetGuildMark( GuildIdx );
	if( m_pGuildMark )	return TRUE;
	else				return FALSE;
}
void CObjectBalloon::SetName(char* Name)
{
	m_pName->SetName(Name);	
}
void CObjectBalloon::SetFortName(char* Name)
{
	m_pName->SetFortName(Name);
}
void CObjectBalloon::SetNickName(char* NickName)
{
	m_pName->SetNickName(NickName);	
}
void CObjectBalloon::SetSiegeName( DWORD NameType, DWORD dwLength )
{
	if( m_pName )
		m_pName->SetSiegeName( NameType, dwLength );
}
void CObjectBalloon::SetMasterName( char* MasterName )
{
	if( m_pName )
		m_pName->SetPetMasterName(MasterName);
}
void CObjectBalloon::SetMarryName(char* MarryName)
{
	if( m_pName )
		m_pName->SetMarryName(MarryName);
}
void CObjectBalloon::SetShiTuName(char* name)
{
	if( m_pName )
		m_pName->SetShiTuName(name);
}




void CObjectBalloon::SetImageName(WORD ImageNum)
{
	if(m_pName)
		m_pName->SetImageName(ImageNum);
}
void CObjectBalloon::SetFame(char* Fame)
{
	if( m_pName )
		m_pName->SetFame(Fame);
}
void CObjectBalloon::SetStageLogo(char* name)
{
	if( m_pName )
		m_pName->SetStageLogo(name);
}

void CObjectBalloon::SetTopListFlg(WORD val)
{
	if(m_pName)
		m_pName->SetTopListFlg(val);
}
void CObjectBalloon::InitTopListFlg()
{
	if(m_pName)
		m_pName->InitTopListFlg();
}
void CObjectBalloon::SetFameRank(char* fame)
{
	if(m_pName)
		m_pName->SetFameRank(fame);
}
//闪名
void CObjectBalloon::SetObjectBalloonFlgName(WORD Flag)
{
	if (m_pName)
		m_pName->SetOBalloonFlgName(Flag);
}

//自定义称号
void CObjectBalloon::SetCustomizingName(char* CustomizingName)
{
	if (m_pName)
		m_pName->SetCustomizingName(CustomizingName);
}

void CObjectBalloon::SetKillCount(char * kill)
{
	if (m_pName)
	{
		m_pName->SetKillCount(kill);
	}
}
//VIP图标
void CObjectBalloon::SetObjectBalloonVIPImage(int val)
{
	if (m_pName)
		m_pName->SetOBalloonVIPImage(val);
}
	