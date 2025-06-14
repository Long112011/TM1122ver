#include "Stdafx.h"
#include "InsDGEntranceInfoDlg.h"
#include "WindowIDEnum.h"
#include "cWindowManager.h"
#include "MHFile.h"
#include "./input/UserInput.h"
#include "objectstatemanager.h"
#include "ObjectManager.h"
#include "Dungeonmgr.h"
#include "chatmanager.h"
#include "./Interface/cListDialog.h"
#include "./Interface/cButton.h"
#include "aimanager.h"
//MAPTYPE tempMap=0;
//MAPTYPE tempMoveMap=0;
CInsDGEntranceInfoDlg::CInsDGEntranceInfoDlg()
{
	pDlg=NULL;

//	bCloseBtn=NULL;
//	bConnectBtn=NULL;
//	bCancelBtn=NULL;

	//wMapChangeNum=0;
	//wMoveMapNum=0;
	bEntranceInfo=NULL;

	/*bStaticLevel=NULL;
	bStaticMoney=NULL;
	bStaticQuest=NULL;
	bStaticItem=NULL;*/

	for(int i=0;i<MaxStatic;i++)
	{
		bStaticEntrance[i]=NULL;
	}

	m_EntranceTipTextList.Initialize(64);
}

CInsDGEntranceInfoDlg::~CInsDGEntranceInfoDlg()
{
	pDlg=NULL;

	//bCloseBtn=NULL;
	//bConnectBtn=NULL;
	//bCancelBtn=NULL;

	bButton[3]=NULL;

	//wMapChangeNum=0;
	//wMoveMapNum=0;

	for(int i=0;i<MaxStatic;i++)
	{
		bStaticEntrance[i]=NULL;
	}

	/*bEntranceInfo=NULL;

	bStaticLevel=NULL;
	bStaticMoney=NULL;
	bStaticQuest=NULL;
	bStaticItem=NULL;*/


	ENTRANCETIP_TEXT* pEnterTipText = NULL;
	ENTRANCETIP_TEXT* pTemp = NULL;
	m_EntranceTipTextList.SetPositionHead();
	while( pEnterTipText = m_EntranceTipTextList.GetData() )
	{
		do
		{
			pTemp = pEnterTipText->pNext;
			if( pEnterTipText->strToolTip )
				delete[] pEnterTipText->strToolTip;
			delete pEnterTipText;
			pEnterTipText = pTemp;
		} while( pEnterTipText );
	}
	m_EntranceTipTextList.RemoveAll();
}

void CInsDGEntranceInfoDlg::Linking()
{
    pDlg = WINDOWMGR->GetWindowForID( INSFUBEN_ENTRANCEINFO_DLG );


	bButton[0]=(cButton *)GetWindowForID(INSFUBEN_ENTRANCEINFO_CLOSE);
	bButton[1]=(cButton *)GetWindowForID(INSFUBEN_ENTRANCEINFO_CONNECT);
	bButton[2]=(cButton *)GetWindowForID(INSFUBEN_ENTRANCEINFO_CANCEL);

	bEntranceInfo=(cListDialog *)GetWindowForID(INSFUBEN_ENTRANCEINFO_LIST);

	bStaticEntrance[Level]=(cStatic *)GetWindowForID(INSFUBEN_ENTRANCEINFO_LEVEL);
	bStaticEntrance[Money]=(cStatic *)GetWindowForID(INSFUBEN_ENTRANCEINFO_MONEY);
	bStaticEntrance[Quest]=(cStatic *)GetWindowForID(INSFUBEN_ENTRANCEINFO_QUEST);
	bStaticEntrance[Item] =(cStatic *)GetWindowForID(INSFUBEN_ENTRANCEINFO_ITEM);

	//bStaticLevel=(cStatic *)GetWindowForID(INSDG_ENTRANCEINFO_LEVEL);
	//bStaticMoney=(cStatic *)GetWindowForID(INSDG_ENTRANCEINFO_MONEY);
	//bStaticQuest=(cStatic *)GetWindowForID(INSDG_ENTRANCEINFO_QUEST);
	//bStaticItem=(cStatic *)GetWindowForID(INSDG_ENTRANCEINFO_ITEM);
	LoadEntranceTipList();
	DungeonMGR->LoadInsDungeonInfo();

	bButton[0]->SetDisable(false);
	
}

void CInsDGEntranceInfoDlg::OnActionEvent(LONG lId, void * p, DWORD we)
{
	switch( lId )
	{
	case INSFUBEN_ENTRANCEINFO_CONNECT:
		{
			bButton[1]->SetDisable(true);
			g_UserInput.GetHeroMove()->MapChange(wMoveMapNum,0,wMapChangeNum,eMapChange_Dungeon);
		}
		break;
	case INSFUBEN_ENTRANCEINFO_CANCEL:
	case INSFUBEN_ENTRANCEINFO_CLOSE:
		{
			bButton[1]->SetDisable(false);
			pDlg->SetActive(FALSE);
			if( HERO->GetState() == eObjectState_Deal )
				OBJECTSTATEMGR->EndObjectState(HERO, eObjectState_Deal);

			MSG_DWORD3 msg;
			msg.Category=MP_CHAR;
			msg.Protocol=MP_CHAR_FB_TIME_SYN;
			msg.dwData1=5;
			msg.dwData2=wMoveMapNum;
			msg.dwObjectID=HEROID;
			NETWORK->Send( &msg, sizeof(msg));

			sCPlayerAI.IsRun=true;
		}
		break;
	/*case INSDG_ENTRANCEINFO_QUEST:
		{
		}
		break;
	case INSDG_ENTRANCEINFO_LIST:
		{
		}
		break;*/
	}
}
void CInsDGEntranceInfoDlg::NpcClick(WORD UniqueIdx,LEVELTYPE MinLevel,LEVELTYPE MaxLevel,MONEYTYPE FubenFee,BYTE Key,BYTE StrKey)
{
	//FB_MAP_INFO*FbInfo=DungeonMGR->GetFBEntranceData(UniqueIdx);
	//if(FbInfo)
	{
		char Temp[256]={0};
		sprintf(Temp,CHATMGR->GetChatMsg(2530),
			MinLevel,
			MaxLevel
			);

		bStaticEntrance[Level]->SetStaticText(Temp);

		char money[16] = { 0, };
		strcpy(money, AddComma(FubenFee));
		sprintf(Temp,CHATMGR->GetChatMsg(2533),
			money	
			);

		bStaticEntrance[Money]->SetStaticText(Temp);

		MAPCHANGE_INFO* pMCInfo = NULL;
		pMCInfo = GAMERESRCMNGR->GetMapChangeInfo(UniqueIdx);
		int destmapnumber = pMCInfo->MoveMapNum;

		wMoveMapNum=destmapnumber;

		wMapChangeNum=UniqueIdx;

		SetEntranceMsg(StrKey);

		SetActive(TRUE);

		bButton[1]->SetDisable(false);		
	}	
	if(this->IsActive())
		OBJECTSTATEMGR->StartObjectState( HERO, eObjectState_Deal );
}
void CInsDGEntranceInfoDlg::SetEntranceMsg(int line)
{
	bEntranceInfo->RemoveAll();
	ENTRANCETIP_TEXT* pTooltipText = GetEntranceTipInfo(line);
	while( pTooltipText )
	{
		bEntranceInfo->AddItem( pTooltipText->strToolTip, RGB_HALF(255,255,0)/*TTTC_DESCRIPTION*/);
		pTooltipText = pTooltipText->pNext;
	}
}
void CInsDGEntranceInfoDlg::LoadEntranceTipList()
{
	CMHFile file;
	if( !file.Init( "Resource/InsDungeonScript/InsDG_EntranceInfoList.bin", "rb" ) )
		return;
	char buf[512];	
	ENTRANCETIP_TEXT* pEnterTipText;
	ENTRANCETIP_TEXT* pTemp;
	DWORD dwIndex;
	int nCpyNum = 0;
	int nLen;
	char* str;
	while( 1 )
	{
		if( file.IsEOF() ) break;
		pEnterTipText = NULL;
		file.GetString(buf);
		START_STRING_COMPARE(buf)

			COMPARE("#Msg")
			dwIndex = file.GetDword();	
			file.GetString(buf);
			if( strcmp( buf, "{" ) == 0 )
			{
				file.GetLine( buf, 512 );
				str = buf;
				nLen = strlen( buf );
				while( *buf != '}' )
				{

					while( nLen > ENTRANCETIPINFO_LEN )
					{
						if( ( str + ENTRANCETIPINFO_LEN ) != CharNext( CharPrev( str, str + ENTRANCETIPINFO_LEN ) ) )
						{
							nCpyNum = ENTRANCETIPINFO_LEN - 1;
						}
						else
						{
							nCpyNum = ENTRANCETIPINFO_LEN;
						}
						pTemp = new ENTRANCETIP_TEXT;
						pTemp->strToolTip = new char[nCpyNum+1];
						strncpy( pTemp->strToolTip, str, nCpyNum );
						pTemp->strToolTip[nCpyNum] = 0;
						nLen -= nCpyNum;
						str += nCpyNum;
						if( *str == '  ' ) ++str;
						if( pEnterTipText )
						{
							pEnterTipText->pNext = pTemp;
						}
						else
						{
							m_EntranceTipTextList.Add( pTemp, dwIndex );
						}
						pEnterTipText = pTemp;
					}
					if( nLen > 0 )
					{
						pTemp = new ENTRANCETIP_TEXT;
						pTemp->strToolTip = new char[nLen+1];
						strncpy( pTemp->strToolTip, str, nLen );
						pTemp->strToolTip[nLen] = 0;
						if( pEnterTipText )
						{
							pEnterTipText->pNext = pTemp;
						}
						else
						{
							m_EntranceTipTextList.Add( pTemp, dwIndex );
						}
						pEnterTipText = pTemp;
					}

					file.GetLine( buf, 512 );
					str = buf;
					nLen = strlen( buf );
				}
			END_COMPARE
		}
	}
	file.Release();
}
ENTRANCETIP_TEXT* CInsDGEntranceInfoDlg::GetEntranceTipInfo( WORD wIdx )
{
	return m_EntranceTipTextList.GetData( wIdx );
}