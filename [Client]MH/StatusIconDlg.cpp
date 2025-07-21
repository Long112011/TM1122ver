// StatusIconDlg.cpp: implementation of the CStatusIconDlg class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "StatusIconDlg.h"
#include "./Interface/cWindowManager.h"
#include "./Interface/cScriptManager.h"
#include "./Input/UserInput.h"
#include "./Interface/cFont.h"
#include "MHFile.h"
#include "ItemManager.h"
#include "ChatManager.h"
#include "ObjectManager.h"
#include "Quest.h"
#include "QuestManager.h"
#include "GuildManager.h"
#include "PartyManager.h"
#include "cWindowSystemFunc.h"
#include "aimanager.h"
#include "HideBuff.h"
#include "GameIn.h"
#include "cMsgBox.h"
#include "WindowIDEnum.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
extern BOOL jTweakToggle;
GLOBALTON(CStatusIconDlg)
CStatusIconDlg::CStatusIconDlg()
{
	m_pDescriptionArray = NULL;
	m_pObject = NULL;
	memset(m_IconCount,0,sizeof(WORD)*eStatusIcon_Max);
	memset(m_StatusIcon,0,sizeof(cImage*)*eStatusIcon_Max);
	memset(m_IconInfo,0,sizeof(ICONRENDERINFO)*eStatusIcon_Max);

	m_CurIconNum = 0;

	memset(m_dwRemainTime,0,sizeof(DWORD)*eStatusIcon_Max);
	memset(m_dwStartTime,0,sizeof(DWORD)*eStatusIcon_Max);

	m_nQuestIconCount = 0;

	m_Check = TRUE;
	m_Show = TRUE;

	//bAppearBtn=TRUE;
}

CStatusIconDlg::~CStatusIconDlg()
{
	if(m_pDescriptionArray)
	{
		delete [] m_pDescriptionArray;
	}
}


void CStatusIconDlg::AddIcon(CObject* pObject,WORD StatusIconNum, WORD ItemIdx, DWORD dwRemainTime )
{
	if(pObject != m_pObject)
		return;
	//sCPlayerAI.SetNotMainSkillClose(StatusIconNum);
	if(StatusIconNum >= eStatusIcon_Max)
	{
		ASSERT(0);
		return;
	}
	
	++m_IconCount[StatusIconNum];

	if( m_IconCount[StatusIconNum] == 1 )
		++m_CurIconNum;

	m_dwRemainTime[StatusIconNum] = dwRemainTime;
	m_dwStartTime[StatusIconNum] = gCurTime;
	m_IconInfo[StatusIconNum].bAlpha = FALSE;

	if( ItemIdx )
	{
		m_IconInfo[StatusIconNum].ItemIndex = ItemIdx;
		m_IconInfo[StatusIconNum].bPlus = TRUE;
		m_IconInfo[StatusIconNum].bAlpha = FALSE;
		m_IconInfo[StatusIconNum].Alpha = 0;
	}
}

void CStatusIconDlg::AddQuestTimeIcon( CObject* pObject,WORD StatusIconNum )
{
	if( pObject != m_pObject )	return;
	if(StatusIconNum >= eStatusIcon_Max)
		__asm int 3;
	
	++m_IconCount[StatusIconNum];

	if( m_IconCount[StatusIconNum] == 1 )
		++m_CurIconNum;

//	m_IconCount[StatusIconNum] = 1;

//	m_dwRemainTime[StatusIconNum] = dwRemainTime;
//	m_dwStartTime[StatusIconNum] = dwQuestIdx;
	m_IconInfo[StatusIconNum].bAlpha = FALSE;
}

void CStatusIconDlg::RemoveIcon(CObject* pObject,WORD StatusIconNum, WORD ItemIdx)
{
	if(pObject != m_pObject)
		return;

	if (StatusIconNum >= eStatusIcon_Max)
	{
		return;
		//__asm int 3;
	}
		

	if(m_IconCount[StatusIconNum] == 0)
		return;
	
	--m_IconCount[StatusIconNum];

	if( m_IconCount[StatusIconNum] == 0 )
	{
		--m_CurIconNum;

		m_dwRemainTime[StatusIconNum] = 0;
		m_dwStartTime[StatusIconNum] = 0;
		m_IconInfo[StatusIconNum].bAlpha = FALSE;
		m_Check = TRUE;
	}
	else
	{
		m_Check = FALSE;
	}
	//sCPlayerAI.SetNotMainSkillOpen(StatusIconNum,m_Check);

	if( ItemIdx )
	{
		m_IconInfo[StatusIconNum].ItemIndex = 0;
		m_IconInfo[StatusIconNum].bPlus = FALSE;
		m_IconInfo[StatusIconNum].bAlpha = FALSE;
		m_IconInfo[StatusIconNum].Alpha = 0;
	}
}

void CStatusIconDlg::RemoveQuestTimeIcon( CObject* pObject,WORD StatusIconNum )
{
	if(pObject != m_pObject)	return;

	if(StatusIconNum >= eStatusIcon_Max)
		__asm int 3;

	if(m_IconCount[StatusIconNum] == 0)	return;
	
	--m_IconCount[StatusIconNum];
	if( m_IconCount[StatusIconNum] == 0 )
	{
		--m_CurIconNum;
		m_dwRemainTime[StatusIconNum] = 0;
		m_dwStartTime[StatusIconNum] = 0;
		m_IconInfo[StatusIconNum].bAlpha = FALSE;
	}
}

void CStatusIconDlg::RemoveAllQuestTimeIcon()
{
	if( m_IconCount[eStatusIcon_QuestTime] > 0 )
	{
		--m_CurIconNum;
	}

	m_IconCount[eStatusIcon_QuestTime] = 0;
	m_dwRemainTime[eStatusIcon_QuestTime] = 0;
	m_dwStartTime[eStatusIcon_QuestTime] = 0;
	m_IconInfo[eStatusIcon_QuestTime].bAlpha = FALSE;
}

void CStatusIconDlg::LoadDescription()
{
	CMHFile file;
	if(file.Init("image/buf_msg.bin","rb",MHFILE_FLAG_DONOTDISPLAY_NOTFOUNDERROR) == FALSE)

		return;
	
	char buf[256];
	m_MaxDesc = file.GetInt();
	file.GetLine(buf,256);	// read enter
	m_pDescriptionArray = new StaticString[m_MaxDesc+1];
	for(int n=1;n<=m_MaxDesc;++n)	// 아이 번호가 1부터 시작한다.
	{
		file.GetLine(buf,256);
		m_pDescriptionArray[n] = buf;
	}

	file.Release();
}

void CStatusIconDlg::Init(CObject* pObject,VECTOR2* pDrawPosition,int MaxIconPerLine)
{
	memset(m_IconCount,0,sizeof(WORD)*eStatusIcon_Max);
	memset(m_StatusIcon,0,sizeof(cImage*)*eStatusIcon_Max);

	m_pObject = pObject;
	m_DrawPosition = *pDrawPosition;
	m_MaxIconPerLine = MaxIconPerLine;

	// image load
	for(int n=1;n<eStatusIcon_Max;++n)
	{
		SCRIPTMGR->GetImage( n-1, &m_StatusIcon[n] , PFT_BUFFPATH );
	}
	
	//Description Load
	if(m_pDescriptionArray == NULL)
	{
		LoadDescription();
	}

	cImage ToolTipImg;
	SCRIPTMGR->GetImage(63, &ToolTipImg, PFT_HARDPATH);
	//SCRIPTMGR->GetImage(284, &ToolTipImg, PFT_HARDPATH); 
	m_toolTip.Init( 0, TTTC_DEFAULT, &ToolTipImg, TTCLR_ITEM_CANEQUIP );
	//m_toolTip.Init( 0, RGBA_MAKE(255, 255, 255, 255), &ToolTipImg, TTCLR_DEFAULT );
	m_toolTip.SetImageAlpha( TOOLTIP_ALPHA );

	//cImage ToolTipImg;
	//SCRIPTMGR->GetImage( 63, &ToolTipImg, PFT_HARDPATH );
	//m_toolTip2.Init( 0, TTTC_DEFAULT, &ToolTipImg, TTCLR_ITEM_CANEQUIP );
	//m_toolTip2.Init( 0, RGBA_MAKE(255, 255, 255, 255), &ToolTipImg, TTCLR_DEFAULT );
	//m_toolTip2.SetImageAlpha( TOOLTIP_ALPHA );

	//cImage ToolTipImage;
	//SCRIPTMGR->GetImage( 63, &ToolTipImage, PFT_HARDPATH );
	//m_pVolumeEdit->SetToolTip( CHATMGR->GetChatMsg(1215), RGBA_MAKE(255, 255, 255, 255), &ToolTipImage, TTCLR_DEFAULT );

	m_CurIconNum = 0;

	if( m_nQuestIconCount > 0 )
	{
		m_IconCount[eStatusIcon_QuestTime] = m_nQuestIconCount;
		++m_CurIconNum;
		m_nQuestIconCount = 0;
	}
}

void CStatusIconDlg::Release()
{
	m_pObject = NULL;
	m_CurIconNum = 0;
	memset(m_IconCount,0,sizeof(WORD)*eStatusIcon_Max);
	memset(m_StatusIcon,0,sizeof(cImage*)*eStatusIcon_Max);

	m_nQuestIconCount = 0;
}

#define ALPHASETTING(Alpha, bPlus)			{ \
if(bPlus) Alpha+=(int)(gTickTime*0.3f);	else Alpha-=(int)(gTickTime*0.3f); \
if(Alpha>255)	{ Alpha=255; bPlus=0;	} else if(Alpha<70) { Alpha=70; bPlus=1;	} }

#include "DungeonMgr.h"
#include "MHMap.h"
void CStatusIconDlg::Render()
{
	if(m_pObject == NULL) return;
	if(!m_Show) return;
		
#ifdef _GMTOOL_
	if( WINDOWMGR->IsInterfaceHided() ) return;
#endif

	int mx = g_UserInput.GetMouse()->GetMouseX();
	int my = g_UserInput.GetMouse()->GetMouseY();
	
	int curx = 0;
	int cury = 0;
	VECTOR2 curpos;
	int IconNum = 0;
	for(int n=1;n<eStatusIcon_Max;++n)
	{
		if (m_IconCount[n] == 0)
		{
			GAMEIN->GetHideBuffDlg()->SetShow(false);
			continue;
		}
			
		if(m_StatusIcon[n].IsNull())
			continue;

		
		GAMEIN->GetHideBuffDlg()->SetShow(TRUE);

		curpos.x = m_DrawPosition.x+12 + curx*33;
		curpos.y = m_DrawPosition.y + cury*33;

		if(m_IconInfo[n].bAlpha)
		{
			ALPHASETTING(m_IconInfo[n].Alpha, m_IconInfo[n].bPlus);
			m_StatusIcon[n].RenderSprite(NULL,NULL,0,&curpos, RGBA_MAKE(255, 255, 255, m_IconInfo[n].Alpha));
		}
		else
		{
			m_StatusIcon[n].RenderSprite(NULL,NULL,0,&curpos, 0xffffffff);
		}


		DWORD elapsed = gCurTime - m_dwStartTime[n];
		DWORD countdown = ( m_dwRemainTime[n]- elapsed )/1000+1;

		if( countdown > 0 && countdown < 10000 )
		{			
			m_StatusIcon[n].RenderSprite(NULL,NULL,0,&curpos, 0xF56969);

			char buf2[16];
			wsprintf(buf2,"%4d", countdown );
			RECT rect={(LONG)curpos.x+5, (LONG)curpos.y+2, 1,1};

			DWORD color1=RGBA_MERGE( RGB_HALF(250 ,250 ,250), 240 );
			DWORD color2=RGBA_MERGE( RGB_HALF(10 ,10 ,10), 255 );

			CFONT_OBJ->RenderNoticeMsg(14, buf2, strlen(buf2), &rect, color1,color2);
		}
		if(++curx >= m_MaxIconPerLine)
		{
			curx = 0;
			++cury;
		}

		static int preselected = 0;
		char buf[128] = { 0, };
		if( !WINDOWMGR->IsMouseOverUsed() )
		{
			int dx = int(mx - curpos.x);
			int dy = int(my - curpos.y);
			if(0 < dx && dx < 33 && 0 < dy && dy < 33)
			{
				if(m_IconInfo[n].ItemIndex)
				{
					m_toolTip.Release();
					ITEM_INFO* pItemInfo = ITEMMGR->GetItemInfo((WORD)m_IconInfo[n].ItemIndex);
					if(!pItemInfo)		continue;
					char itemname[64] = { 0, };

					//by JACK
					/*cImage imgToolTip2;
					SCRIPTMGR->GetImage(219, &imgToolTip2, PFT_HARDPATH);
					m_toolTip.AddImageTop(&imgToolTip2);

					cImage imgEquipInfo;
					imgEquipInfo = m_StatusIcon[n];
					if (&imgEquipInfo)
					{
						m_toolTip.IsBuffIcon = TRUE;
						m_toolTip.AddImageEquip(&imgEquipInfo);
					}*/
					//

					wsprintf( itemname, "[%s]", pItemInfo->ItemName );
					m_toolTip.AddLine(itemname, TTTC_DEFAULT);
					m_toolTip.AddLine("");

					TOOLTIP_TEXT* pTooltip = ITEMMGR->GetItemToolTipInfo( pItemInfo->ItemTooltipIdx );
					while( pTooltip )
					{
						m_toolTip.AddLine( pTooltip->strToolTip, TTTC_DESCRIPTION );
						pTooltip = pTooltip->pNext;
					}
					m_toolTip.AddLine("");
										
					SHOPITEMBASE* pInfo = ITEMMGR->GetUsedItemInfo(m_IconInfo[n].ItemIndex);
					if(pInfo)
					{
						int day, hour, minute;
						if( pItemInfo->SellPrice == eShopItemUseParam_Realtime )
						{
							stTIME EndTime;
							EndTime.SetTime(pInfo->Remaintime);

							m_toolTip.AddLine(CHATMGR->GetChatMsg(766), TTTC_DEFAULT);
							sprintf(buf, CHATMGR->GetChatMsg(767),

							EndTime.GetYear()+15, EndTime.GetMonth(), EndTime.GetDay(), EndTime.GetHour(), EndTime.GetMinute());
							m_toolTip.AddLine(buf, TTTC_QUESTITEM);
							
							if( pItemInfo->ItemKind == eSHOP_ITEM_INCANTATION  )
							{
								if( pItemInfo->CheRyuk )
								{
									m_toolTip.AddLine("");
									sprintf( buf, CHATMGR->GetChatMsg(730), HERO->GetShopItemStats()->ProtectCount );
									m_toolTip.AddLine( buf, TTTC_QUESTITEM );
								}
							}
						}
						else if( pItemInfo->SellPrice == eShopItemUseParam_Playtime )
						{
							int temp = pInfo->Remaintime;
							day = pInfo->Remaintime/(DAYTOSECOND*1000);

							temp -= day*(DAYTOSECOND*1000);
							hour = temp/(HOURTOSECOND*1000);
							temp -= hour*(HOURTOSECOND*1000);
							minute = temp/(MINUTETOSECOND*1000);
							temp -= minute*(MINUTETOSECOND*1000);
				
							m_toolTip.AddLine(CHATMGR->GetChatMsg(768), TTTC_DEFAULT);
							if(day)
								sprintf(buf, CHATMGR->GetChatMsg(769), day, hour, minute+1);
							else if(hour)
								sprintf(buf, CHATMGR->GetChatMsg(770), hour, minute+1);
							else if(minute)
								sprintf(buf, CHATMGR->GetChatMsg(771), minute+1);
							else
								sprintf(buf, CHATMGR->GetChatMsg(772));
							
							m_toolTip.AddLine(buf, TTTC_QUESTITEM);
						}
//----------------------------------------------------------------------------
						else if( pItemInfo->SellPrice == eShopItemuseParam_FBItem )
						{
							int temp = pInfo->Remaintime;
							day = pInfo->Remaintime/(DAYTOSECOND*1000);

							temp -= day*(DAYTOSECOND*1000);
							hour = temp/(HOURTOSECOND*1000);
							temp -= hour*(HOURTOSECOND*1000);
							minute = temp/(MINUTETOSECOND*1000);
							temp -= minute*(MINUTETOSECOND*1000);
				
							m_toolTip.AddLine(CHATMGR->GetChatMsg(768), TTTC_DEFAULT);
							if(day)
								sprintf(buf, CHATMGR->GetChatMsg(769), day, hour, minute+1);
							else if(hour)
								sprintf(buf, CHATMGR->GetChatMsg(770), hour, minute+1);
							else if(minute)
								sprintf(buf, CHATMGR->GetChatMsg(771), minute+1);
							else
								sprintf(buf, CHATMGR->GetChatMsg(772));
							

							m_toolTip.AddLine(buf, TTTC_QUESTITEM);
						}
//----------------------------------------------------------------------------
						else if( pItemInfo->SellPrice == eShopItemUseParam_Forever )
						{
							if( pItemInfo->ItemKind == eSHOP_ITEM_INCANTATION )
							{
								if( pItemInfo->CheRyuk )
								{
									m_toolTip.AddLine( CHATMGR->GetChatMsg(749), TTTC_QUESTITEM );

									m_toolTip.AddLine("");
									sprintf( buf, CHATMGR->GetChatMsg(730), HERO->GetShopItemStats()->ProtectCount );
									m_toolTip.AddLine( buf, TTTC_QUESTITEM );
								}
							}
						}
					}
				}
				else
				{
					if( n == eStatusIcon_QuestTime )
					{
						m_toolTip.Release();

						int count = 0;
						stTIME* pTime = NULL;
						char* str = NULL;
						CQuest* pQuest = NULL;
						CYHHashTable<CQuest>* pTable = QUESTMGR->GetQuestTable();
						pTable->SetPositionHead();
						while( pQuest = pTable->GetData() )
						{
							if( pQuest->IsTimeCheck() )
							{
								pTime = pQuest->GetCheckTime();
								str = QUESTMGR->GetQuestTitle( pQuest->GetQuestIdx() );
								if( pQuest->GetCheckType() == 1 || pQuest->GetCheckType() == 2 )
								{
									if( str )
										sprintf( buf, CHATMGR->GetChatMsg(1154), str );
									else
										sprintf( buf, CHATMGR->GetChatMsg(1154), "" );
								}
								else if( pQuest->GetCheckType() == 3 )
								{
									if( str )
										sprintf( buf, CHATMGR->GetChatMsg(1152), str );
									else
										sprintf( buf, CHATMGR->GetChatMsg(1152), "" );
								}
								m_toolTip.AddLine( buf,TTTC_DEFAULT );
								sprintf( buf, CHATMGR->GetChatMsg(767), pTime->GetYear(), pTime->GetMonth(), pTime->GetDay(), pTime->GetHour(), pTime->GetMinute() );
								m_toolTip.AddLine( buf, TTTC_QUESTITEM );

								++count;
							}
						}

						if( count == 0 )
							RemoveAllQuestTimeIcon();

						count = 0;
					}
					else
					{
						if( n <= m_MaxDesc )
						{
							m_toolTip = m_pDescriptionArray[n];
						
							//남은시간을 보여주자!	
							DWORD elapsed = gCurTime - m_dwStartTime[n];
							if( elapsed < m_dwRemainTime[n] )
							{
								if( m_dwRemainTime[n] - elapsed >= 60000 )
								{
									DWORD sec = ((m_dwRemainTime[n] - elapsed) % 60000) / 1000 + 1;
									DWORD min = (m_dwRemainTime[n] - elapsed) / 60000;

									sprintf( buf, CHATMGR->GetChatMsg(1363), min, sec);
								}
								else
									sprintf( buf, CHATMGR->GetChatMsg(581), ( m_dwRemainTime[n] - elapsed ) / 1000 + 1);

								m_toolTip.AddLine(buf, TTTC_QUESTITEM);
							
								if( m_dwRemainTime[n] - elapsed <= 5000 )
									m_IconInfo[n].bAlpha = TRUE;
							}
						}
						else if( n == eStatusIcon_SkipEffect || n == eStatusIcon_Hide || n == eStatusIcon_Detect )
						{
							//1356 "은신상태"
							//1357 "혜안상태"
							//1358 "무초상태"

							m_toolTip.Release();

							switch(n)
							{
							case eStatusIcon_SkipEffect:
								m_toolTip.AddLine( CHATMGR->GetChatMsg(1358),TTTC_DEFAULT );
								break;
							case eStatusIcon_Hide:
								m_toolTip.AddLine( CHATMGR->GetChatMsg(1356),TTTC_DEFAULT );
								break;
							case eStatusIcon_Detect:
								m_toolTip.AddLine( CHATMGR->GetChatMsg(1357),TTTC_DEFAULT );
								break;
							}
							
							//남은시간을 보여주자!	
							DWORD elapsed = gCurTime - m_dwStartTime[n];
							if( elapsed < m_dwRemainTime[n] )
							{
								if( m_dwRemainTime[n] - elapsed >= 60000 )
								{
									DWORD sec = ((m_dwRemainTime[n] - elapsed) % 60000) / 1000 + 1;
									DWORD min = (m_dwRemainTime[n] - elapsed) / 60000;

									sprintf( buf, CHATMGR->GetChatMsg(1363), min, sec);
								}
								else
									sprintf( buf, CHATMGR->GetChatMsg(581), ( m_dwRemainTime[n] - elapsed ) / 1000 + 1);
								
								m_toolTip.AddLine(buf, TTTC_QUESTITEM);

								if( m_dwRemainTime[n] - elapsed <= 5000 )
									m_IconInfo[n].bAlpha = TRUE;
							}
						}
						//SW060719 문파포인트
						else if( n == eStatusIcon_GuildPlustime_SuRyun || n == eStatusIcon_GuildPlustime_MuGong
							|| n == eStatusIcon_GuildPlustime_Exp || n == eStatusIcon_GuildPlustime_DamageUp )
						{
							m_toolTip.Release();

							GUILDPLUSTIME_INFO* pGPlustimeInfo = GUILDMGR->GetGuildPointPlustimeList();

							DWORD GPlustimeKind = 0;
							char* GPlustimeName = NULL;
							switch( n )
							{
							case eStatusIcon_GuildPlustime_SuRyun:
								{
									GPlustimeKind = eGPT_SuRyun;
								}
								break;
							case eStatusIcon_GuildPlustime_MuGong:
								{
									GPlustimeKind = eGPT_MuGong;
								}
								break;
							case eStatusIcon_GuildPlustime_Exp:
								{
									GPlustimeKind = eGPT_Exp;
								}
								break;
							case eStatusIcon_GuildPlustime_DamageUp:
								{
									GPlustimeKind = eGPT_DamageUp;
								}
								break;
							default:
								{
									ASSERT(0);
								}
								break;
							}

							char Buf[128] = {0,};
							GUILDMGR->GetGuildPlustimeNameFromKind( GPlustimeKind, Buf );
							sprintf( buf, CHATMGR->GetChatMsg(1381), Buf ); 
							m_toolTip.AddLine(buf, TTTC_QUESTITEM);

							DWORD dwEndTime = GUILDMGR->GetGuildPlustimeEndTimeFromKind(GPlustimeKind);

							stTIME EndTime;
							EndTime.SetTime(dwEndTime);
							m_toolTip.AddLine(CHATMGR->GetChatMsg(766), TTTC_DEFAULT);
							sprintf(buf, CHATMGR->GetChatMsg(767),EndTime.GetYear()+15, EndTime.GetMonth(), EndTime.GetDay(), EndTime.GetHour(), EndTime.GetMinute());
							m_toolTip.AddLine(buf, TTTC_QUESTITEM);
						}
						// 방파 플러스타임
						else if( n == 148 )
						{
							m_toolTip.Release();
							m_toolTip.AddLine( m_pDescriptionArray[54], TTTC_QUESTITEM );
							m_toolTip.AddLine( "" );

							char buf[256] = { 0, };						
							if( PARTYMGR->GetPartyEventRate( ePartyEvent_ItemRate ) )
							{
								sprintf( buf, CHATMGR->GetChatMsg(1415), PARTYMGR->GetPartyEventRate( ePartyEvent_ItemRate ) );
                                m_toolTip.AddLine( buf, TTTC_DEFAULT );
							}
							if( PARTYMGR->GetPartyEventRate( ePartyEvent_NaeRyukRate ) )
							{
								sprintf( buf, CHATMGR->GetChatMsg(1400), PARTYMGR->GetPartyEventRate( ePartyEvent_NaeRyukRate ) );
								m_toolTip.AddLine( buf, TTTC_DEFAULT );
							}
							if( PARTYMGR->GetPartyEventRate( ePartyEvent_MussangCharge ) )
							{
								sprintf( buf, CHATMGR->GetChatMsg(1405), PARTYMGR->GetPartyEventRate( ePartyEvent_MussangCharge ) );
								m_toolTip.AddLine( buf, TTTC_DEFAULT );
							}
							if( PARTYMGR->GetPartyEventRate( ePartyEvent_DefenceRate ) )
							{
								sprintf( buf, CHATMGR->GetChatMsg(1406), PARTYMGR->GetPartyEventRate( ePartyEvent_DefenceRate ) );
								m_toolTip.AddLine( buf, TTTC_DEFAULT );
							}
							if( PARTYMGR->GetPartyEventRate( ePartyEvent_CriticalRate ) )
							{
								sprintf( buf, CHATMGR->GetChatMsg(1423), PARTYMGR->GetPartyEventRate( ePartyEvent_CriticalRate ) );
								m_toolTip.AddLine( buf, TTTC_DEFAULT );
							}
						}
						// magi82(23)
						else if( n == eStatusIcon_Titan_General || n == eStatusIcon_Titan_Fantasy )
						{
							//1675 "타이탄 탑승상태"
							m_toolTip.Release();
							m_toolTip.AddLine( CHATMGR->GetChatMsg(1675),TTTC_DEFAULT );

							//남은시간을 보여주자!	
							DWORD elapsed = gCurTime - m_dwStartTime[n];
							if( elapsed < m_dwRemainTime[n] )
							{
								if( m_dwRemainTime[n] - elapsed >= 60000 )
								{
									DWORD sec = ((m_dwRemainTime[n] - elapsed) % 60000) / 1000 + 1;
									DWORD min = (m_dwRemainTime[n] - elapsed) / 60000;

									sprintf( buf, CHATMGR->GetChatMsg(1363), min, sec);
								}
								else
									sprintf( buf, CHATMGR->GetChatMsg(581), ( m_dwRemainTime[n] - elapsed ) / 1000 + 1);

								m_toolTip.AddLine(buf, TTTC_QUESTITEM);

								if( m_dwRemainTime[n] - elapsed <= 5000 )
									m_IconInfo[n].bAlpha = TRUE;
							}
						}
					}





					// m_IconCount[n] = 0;
					//m_toolTip.Render();
					//CFONT_OBJ->RenderFont( 0, m_pDescriptionArray[n], strlen(m_pDescriptionArray[n]), &rect, 0xffffffff );
				}

				if(jTweakToggle)
				{
					char tesst[64] = { 0, };
					wsprintf( tesst, "[m_IconCount[n] %d] [IconNum %d] [n %d]", m_IconCount[n],IconNum,n );
					m_toolTip.AddLine(tesst,TTTC_DEFAULT);
				}
				m_toolTip.SetXY( mx+TOOLTIP_MOUSESIZE_X, my+TOOLTIP_MOUSESIZE_Y );
				WINDOWMGR->SetToolTipWindow( &m_toolTip );
			}
		}
		//if (CanDelBuf)
		{
			if (g_UserInput.GetMouse()->RButtonDoubleClick() &&
				!CanDelBuf)
			{
				int dx = int(mx - curpos.x);
				int dy = int(my - curpos.y);
				if (0 < dx && dx < 33 && 0 < dy && dy < 33)
				{
					if (DelItemIdx != m_IconInfo[n].ItemIndex)
					{
						if (m_IconInfo[n].ItemIndex)
						{
							DelItemIdx = m_IconInfo[n].ItemIndex;
							ITEM_INFO* pItemInfo = ITEMMGR->GetItemInfo(m_IconInfo[n].ItemIndex);
							if (!pItemInfo)		continue;

							SHOPITEMBASE* pInfo = ITEMMGR->GetUsedItemInfo(m_IconInfo[n].ItemIndex);
							if (pInfo)
							{
								CanDelBuf = true;
								WINDOWMGR->MsgBox(MSBOX_DELBUF, MBT_YESNO, CHATMGR->GetChatMsg(363), pItemInfo->ItemName);
								return;
							}
						}
					}
				}
			}
		}
		if( ++IconNum >= m_CurIconNum )
			break;
	}
}



void CStatusIconDlg::SetOneMinuteToShopItem(DWORD ItemIdx)
{
	ITEM_INFO* pItemInfo = ITEMMGR->GetItemInfo( (WORD)ItemIdx );
	if(!pItemInfo)		return;
	
	if( pItemInfo->SellPrice == eShopItemUseParam_Playtime )
	{
		SHOPITEMBASE* pInfo = ITEMMGR->GetUsedItemInfo(m_IconInfo[pItemInfo->BuyPrice+1].ItemIndex);
		if(!pInfo)			return;	
		pInfo->Remaintime = MINUTETOSECOND*1000;
	}

	if( pItemInfo->ItemKind & eSHOP_ITEM )
	{
		m_IconInfo[pItemInfo->BuyPrice+1].bAlpha = TRUE;
		m_IconInfo[pItemInfo->BuyPrice+1].bPlus = TRUE;
		m_IconInfo[pItemInfo->BuyPrice+1].Alpha = 0;
	}
}
BOOL CStatusIconDlg::CheckStatusIcon(WORD StatusIconNum)
{
	if(StatusIconNum >= eStatusIcon_Max)
	{
		return FALSE;
	}

	if(m_IconCount[StatusIconNum]==0)
	{
		return FALSE;
	}

	return TRUE;
}