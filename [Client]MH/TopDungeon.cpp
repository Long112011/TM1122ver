#include "stdafx.h"
#include "TopDungeon.h"
#include "cWindowManager.h"
#include "WindowIDEnum.h"
#include "ObjectManager.h"
#include "GameIn.h"
#include "./Interface/cPushupButton.h"
#include "./Interface/cListCtrl.h"
#include "./Interface/cStatic.h"
#include "cRitemEx.h"
#include "TopIcon.h"
#include "BillBoardManager.h"
#include "RankButtonDialog.h"
#include "itemManager.h"
#include "ChatManager.h"
#include "./Input/UserInput.h"
#include "CharacterRBTN.h"
#define PAGELISTNUM	20  
CTopDungeon::CTopDungeon()
{
	pDlg = NULL;
	pHeroList = NULL;
	m_StartPos = 0;
	m_EndPos = PAGELISTNUM;
	
	memset(pPageList, NULL, sizeof(cPushupButton*)* 5);
	memset(pColumnStatic, NULL, sizeof(cStatic*)* 7);
	memset(&pHeroData, 0, sizeof(SEND_TOPLIST));
	m_TableIdx = 0;
}
CTopDungeon::~CTopDungeon()
{
	pDlg = NULL;
	pHeroList = NULL;
	memset(pPageList, NULL, sizeof(cPushupButton*)* 5);
	memset(pColumnStatic, NULL, sizeof(cStatic*)* 7);
	
	memset(&pHeroData, 0, sizeof(SEND_TOPLIST));
}
void CTopDungeon::Linking()
{
	pDlg = WINDOWMGR->GetWindowForID(RANKING_DG_DLG);
	pUpdateTime = new cStatic;
	pUpdateTime->Init(410, 5, 0, 0, NULL, TOPDG_UPDATE_TIME);
	pUpdateTime->SetFontIdx(0);
	pUpdateTime->SetStaticText("");
	pUpdateTime->SetFGColor(RGB_HALF(135, 210, 255));
	Add(pUpdateTime);
	for (int i = 0; i<7; i++)
	{
		pColumnStatic[i] = (cStatic*)GetWindowForID(RK_DG_STC_RANK_NUM + i);
	}
	LoadList();
	LoadHeroList();
	for (int i = 0; i<5; i++)
	{
		pPageList[i] = ((cPushupButton*)pDlg->GetWindowForID(RK_DG_BTN_PAGE01 + i));
	}
}

DWORD CTopDungeon::ActionEvent(CMouse* mouseInfo)
{
	DWORD we = cDialog::ActionEvent(mouseInfo);

	if (!m_bActive) return we;

	
	if (we & WE_RBTNCLICK)
	{
		int b = pHeroList->PtIdxInRow(mouseInfo->GetMouseEventX(), mouseInfo->GetMouseEventY());	
		if ( WE_MOUSEOVER & pColumnStatic[3]->ActionEvent(mouseInfo) )
		{
			if (checkName(pHeroData.TopInfo[b].szName1))
			{
				if (strcmp(pHeroData.TopInfo[b].szName1, HERO->GetObjectName()) != 0)
					GAMEIN->GetCharacterRBTNDlg()->ShowChatMenu(pHeroData.TopInfo[b].szName1);
			}				
		}				
	}
	return we;
}
bool CTopDungeon::checkName(char * c)
{
	for (int i = m_StartPos; i < m_EndPos; i++)
	{
		if (pHeroData.TopInfo[i].szName1 == c ) return 1;			
	}
	return 0;
}
void CTopDungeon::OnActionEvent(LONG lId, void * p, DWORD we)
{
	if (we & WE_PUSHDOWN)
	{
		if (RK_DG_BTN_PAGE01 <= lId && lId <= RK_DG_BTN_PAGE05)
		{
			for (int i = 0; i<5; i++)
			{
				pPageList[i]->SetPush(false);
			}
			m_TableIdx = lId - RK_DG_BTN_PAGE01;
			pPageList[m_TableIdx]->SetPush(true);
			SetPage(m_TableIdx + 1);
		}
	}
	else if (we & WE_PUSHUP)
	{
		if (RK_DG_BTN_PAGE01 <= lId && lId <= RK_DG_BTN_PAGE05)
		{
			pPageList[lId - RK_DG_BTN_PAGE01]->SetPush(true);
		}
	}
}
void CTopDungeon::SetHeroTopListInfo(SEND_TOPLIST * pListInfo)
{
	pHeroList->DeleteAllItems();
	memset(&pHeroData, 0, sizeof(SEND_TOPLIST));
	memcpy(&pHeroData, pListInfo, sizeof(SEND_TOPLIST));
	if (pHeroData.wCount>0)
	{
		GAMEIN->GetTopIcon()->SetHeroFirst(pHeroData.TopInfo[0].szName1);
		sBOARDMGR->SetTopListInfo(&pHeroData);
		sBOARDMGR->SetHeroTopListFlg((CPlayer*)HERO);
	}
	if (pDlg->IsActive())
		SetPage(1);
}
void CTopDungeon::GetHeroTopListInfo()
{
	MSG_DWORD msg;
	msg.Category = MP_USERCONN;
	msg.Protocol = MP_USERCONN_DUNGEON_TOPLIST_SYN;
	msg.dwObjectID = HEROID;
	msg.dwData = 1;
	NETWORK->Send(&msg, sizeof(msg));
}
void CTopDungeon::LoadList()
{
	pHeroList = new cListCtrl;
	pHeroList->Init(5, 25, 495, 450, NULL, -1);
	pHeroList->Create(7);
	pHeroList->InitListCtrl(7, 20);
	cImage tempImage;
	cImage BodyImage;
	cImage OverImage;
	pHeroList->InitListCtrlImage(&tempImage, 20, &BodyImage, 16, &OverImage);
	for (int n = 0; n < 7; n++)
	{
		pHeroList->InsertColoumn(n, "", pColumnStatic[n]->GetWidth());
	}
	Add(pHeroList);
}
void CTopDungeon::LoadHeroList()
{
	pHeroPos = new cListCtrl;
	pHeroPos->Init(7, 417, 495, 0, NULL, -1);
	pHeroPos->Create(7);
	pHeroPos->InitListCtrl(7, 1);
	cImage tempImage;
	cImage BodyImage;
	cImage OverImage;
	pHeroPos->InitListCtrlImage(&tempImage, 1, &BodyImage, 1, &OverImage);
	for (int n = 0; n < 7; n++)
	{
		pHeroPos->InsertColoumn(n, "", pColumnStatic[n]->GetWidth());
	}
	Add(pHeroPos);
}
void CTopDungeon::ShowAllList()
{
	if (pDlg)
	{
		RANKBTNDLG->SetFlicker(false, HRANK);
		if (pUpdateTime)
		{
			pUpdateTime->SetStaticText(pHeroData.updateTime);
		}
		if (pHeroList)
		{
			pHeroList->DeleteAllItems();
			pHeroList->SetRenderImage(TRUE);
			pHeroList->SetRenderColumnHeader(true);
			pHeroList->SetAlign(TXT_CENTER);
			cRITEMEx *ritem = NULL;
			if (0 == pHeroData.wCount) return;
			for (int i = m_StartPos; i < m_EndPos; i++)
			{
				cRITEMEx *ritem = new cRITEMEx(7);
				if (pHeroData.TopInfo[i].Key == 1)
				{
					GAMEIN->GetTopIcon()->SetHeroFirst(pHeroData.TopInfo[i].szName1);
				}
				if (pHeroData.TopInfo[i].Key <= 3)
				{
					SCRIPTMGR->GetImage(29 + pHeroData.TopInfo[i].Key - 1, &ritem->ItemImg[0], PFT_JACKPATH);
					sprintf(ritem->pString[0], "");
				}
				else if (pHeroData.TopInfo[i].Key>3 && pHeroData.TopInfo[i].Key <= 8)
				{
					SCRIPTMGR->GetImage(32, &ritem->ItemImg[0], PFT_JACKPATH);
					sprintf(ritem->pString[0], "%d", pHeroData.TopInfo[i].Key);
				}
				else
				{
					sprintf(ritem->pString[0], "%d", pHeroData.TopInfo[i].Key);
				}
				if (pHeroData.TopInfo[i].Change > 0)	SCRIPTMGR->GetImage(102, &ritem->ItemImg[1], PFT_JACKPATH);
				if (pHeroData.TopInfo[i].Change == 0)	SCRIPTMGR->GetImage(33, &ritem->ItemImg[1], PFT_JACKPATH);
				if (pHeroData.TopInfo[i].Change < 0)	SCRIPTMGR->GetImage(103, &ritem->ItemImg[1], PFT_JACKPATH);
				switch (pHeroData.TopInfo[i].Stage)
				{
				case eStage_Normal:	SCRIPTMGR->GetImage(33, &ritem->ItemImg[2], PFT_JACKPATH);	break;
				case eStage_Hwa:
				case eStage_Hyun:	SCRIPTMGR->GetImage(34, &ritem->ItemImg[2], PFT_JACKPATH);	break;
				case eStage_Geuk:
				case eStage_Tal:	SCRIPTMGR->GetImage(35, &ritem->ItemImg[2], PFT_JACKPATH);	break;
				}
				std::string s = std::to_string(pHeroData.TopInfo[i].nData1);
				char* c = strcpy(new char[s.length() + 1], s.c_str());
				std::string f = std::to_string(pHeroData.TopInfo[i].nData2);
				char* b = strcpy(new char[f.length() + 1], f.c_str());
				std::string g = std::to_string(pHeroData.TopInfo[i].Change);
				char* h = strcpy(new char[g.length() + 1], g.c_str());
				strcpy(ritem->pString[1], "  ");
				strcat(ritem->pString[1], h);
				strcpy(ritem->pString[2], "");
				strcpy(ritem->pString[3], pHeroData.TopInfo[i].szName1);
				strcpy(ritem->pString[4], c);
				strcpy(ritem->pString[5], b);
				strcpy(ritem->pString[6], pHeroData.TopInfo[i].szName2);
				if (pHeroData.TopInfo[i].Key <= 10)
				{
					if (pHeroData.TopInfo[i].Key <= 3)
					{
						ritem->rgb[3] = RGBA_MAKE(255, 100 + 50 * i, 100 + 60 * i, m_alpha * m_dwOptionAlpha / 100);
					}
					else if (pHeroData.TopInfo[i].Key <= 8)
					{
						ritem->rgb[3] = RGBA_MAKE(249, 252, 225, m_alpha * m_dwOptionAlpha / 100);
					}
				}
				ritem->rgb[4] = RGBA_MAKE(255, 250, 205, 255);
				ritem->rgb[5] = RGBA_MAKE(255, 255, 255, 255);
				pHeroList->InsertItem(i, ritem);
				if (strcmp(pHeroData.TopInfo[i].szName1, HERO->GetObjectName()) == 0)
				{
					pHeroPos->DeleteAllItems();
					pHeroPos->SetRenderImage(TRUE);
					pHeroPos->SetRenderColumnHeader(TRUE);
					pHeroPos->SetAlign(TXT_CENTER);
					cRITEMEx *ritem2 = new cRITEMEx(7);
					if (pHeroData.TopInfo[i].Key <= 3)
					{
						SCRIPTMGR->GetImage(29 + pHeroData.TopInfo[i].Key - 1, &ritem2->ItemImg[0], PFT_JACKPATH);
					}
					else if (pHeroData.TopInfo[i].Key>3 && pHeroData.TopInfo[i].Key <= 8)
					{
						SCRIPTMGR->GetImage(32, &ritem2->ItemImg[0], PFT_JACKPATH);
					}
					if (pHeroData.TopInfo[i].Change > 0)	SCRIPTMGR->GetImage(102, &ritem2->ItemImg[1], PFT_JACKPATH);
					if (pHeroData.TopInfo[i].Change == 0)	SCRIPTMGR->GetImage(33, &ritem2->ItemImg[1], PFT_JACKPATH);
					if (pHeroData.TopInfo[i].Change < 0)	SCRIPTMGR->GetImage(103, &ritem2->ItemImg[1], PFT_JACKPATH);
					switch (pHeroData.TopInfo[i].Stage)
					{
					case eStage_Normal:	SCRIPTMGR->GetImage(33, &ritem2->ItemImg[2], PFT_JACKPATH);	break;
					case eStage_Hwa:
					case eStage_Hyun:	SCRIPTMGR->GetImage(34, &ritem2->ItemImg[2], PFT_JACKPATH);	break;
					case eStage_Geuk:
					case eStage_Tal:	SCRIPTMGR->GetImage(35, &ritem2->ItemImg[2], PFT_JACKPATH);	break;
					}
					for (int ii = 0; ii < 6; ii++)
					{
						strcpy(ritem2->pString[ii], ritem->pString[ii]);
					}
					/*strcpy(ritem2->pString[0], ritem->pString[0]);
					strcpy(ritem2->pString[1], ritem->pString[1]);
					strcpy(ritem2->pString[2], ritem->pString[2]);
					strcpy(ritem2->pString[3], ritem->pString[3]);
					strcpy(ritem2->pString[4], ritem->pString[4]);
					strcpy(ritem2->pString[5], ritem->pString[5]);
					strcpy(ritem2->pString[6], ritem->pString[6]);*/
					pHeroPos->InsertItem(i, ritem2);
				}
			}
			int Count = pHeroData.wCount;
			int PageNum = Count / PAGELISTNUM;
			int PageNext = Count % PAGELISTNUM;
			for (int i = 0; i<5; i++)
			{
				pPageList[i]->SetActive(FALSE);
			}
			if (PageNext != 0)
			{
				PageNum = PageNum + 1;
			}
			for (int i = 0; i<PageNum; i++)
			{
				pPageList[i]->SetActive(TRUE);
			}
		}
	}
}
void CTopDungeon::InitHeroList()
{
	m_StartPos = 0;
	if (pHeroData.wCount<PAGELISTNUM)
	{
		m_EndPos = pHeroData.wCount;
	}
	else
	{
		m_EndPos = PAGELISTNUM;
	}
}
void CTopDungeon::SetHeroPage(int PageIndex)
{
	int Count = pHeroData.wCount;
	int PageNum = Count / PAGELISTNUM;
	int PageNext = Count%PAGELISTNUM;
	m_StartPos = (PageIndex - 1)*PAGELISTNUM;
	if (PageNext == 0)
	{
		m_EndPos = (PageIndex)*PAGELISTNUM;
	}
	else
	{
		m_EndPos = (PageNum)*PAGELISTNUM + PageNext;
	}
}
void CTopDungeon::SetPage(int PageIndex)
{
	SetHeroPage(PageIndex);
	ShowAllList();
}
void CTopDungeon::OpenLastTab()
{
	if (m_TableIdx != 0)
	{
		pPageList[m_TableIdx]->SetPush(true);
		SetPage(m_TableIdx + 1);
	}
	else
		pPageList[0]->SetPush(true);
}