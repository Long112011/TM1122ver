#include "stdafx.h"
#include "TopMunpDialog.h"
#include "cWindowManager.h"
#include "cScriptManager.h"
#include "WindowIDEnum.h"
#include "ChatManager.h"
#include "ObjectManager.h"
#include "TopIcon.h"   
#include "GameIn.h"
#include "./interface/cEditBox.h"
#include "./Interface/cWindowManager.h" 
#include "./Interface/cPushupButton.h"
#include "./Interface/cListCtrl.h"
#include "./Interface/cStatic.h"
#include "cRitemEx.h"

#include "GuildMark.h"           
#include "GuildMarkManager.h"    
#include "GuildMarkImg.h"        
#include "cImageSelf.h"  

#include "RankButtonDialog.h"


#include "./Input/UserInput.h"
#include "CharacterRBTN.h"
#define PAGELISTNUM	20  
CTopMunpDialog::CTopMunpDialog()
{
  pDlg=NULL;
  pMunpList=NULL;
  m_StartPos=0;
  m_EndPos=PAGELISTNUM;
  for(int i=0;i<5;i++)
  {
	  pPageList[i]=NULL;
  }
  for (int i = 0; i<8; i++)
  {
	  pColumnStatic[i] = NULL;
  }
  memset(&pMunpData,0,sizeof(SEND_TOPLIST));
  m_TableIdx = 0;
}
CTopMunpDialog::~CTopMunpDialog()
{
  pDlg=NULL;
  pMunpList=NULL;
  for(int i=0;i<5;i++)
  {
	  pPageList[i]=NULL;
  }
  for (int i = 0; i<8; i++)
  {
	  pColumnStatic[i] = NULL;
  }
  memset(&pMunpData,0,sizeof(SEND_TOPLIST));
}
void CTopMunpDialog::Linking()
{
	pDlg = WINDOWMGR->GetWindowForID(RANKINGGUILD_DLG);
   

	pUpdateTime = new cStatic;
	pUpdateTime->Init(410, 5, 0, 0, NULL, TOP_MUNP_UPDATE_TIME);
	pUpdateTime->SetFontIdx(0);
	pUpdateTime->SetStaticText("");
	pUpdateTime->SetFGColor(RGB_HALF(135, 210, 255));
	Add(pUpdateTime);
	//pUpdateTime=((cStatic*)pDlg->GetWindowForID(TOP_MUNP_UPDATE_TIME));

	//pMunpList = ((cListCtrl*)pDlg->GetWindowForID(TOP_MUNP_LIST));
	//pMunpPos   =((cListCtrl*)pDlg->GetWindowForID(TOP_MUNP_POS));
	for (int i = 0; i<8; i++)
	{
		pColumnStatic[i] = (cStatic*)GetWindowForID(RKG_STC_RANK_NUM + i);
	}
	LoadList();
	//LoadHeroList();
	for(int i=0;i<5;i++)
	{
		pPageList[i] = ((cPushupButton*)pDlg->GetWindowForID(RKG_BTN_PAGE01 + i));
	}
}
void CTopMunpDialog::LoadList()
{
	pMunpList = new cListCtrl;
	pMunpList->Init(5, 25, 495, 450, NULL, -1);
	pMunpList->Create(8);
	pMunpList->InitListCtrl(8, 20);
	cImage tempImage;
	cImage BodyImage;
	cImage OverImage;
	pMunpList->InitListCtrlImage(&tempImage, 20, &BodyImage, 16, &OverImage);
	for (int n = 0; n < 8; n++)
	{
		pMunpList->InsertColoumn(n, "", pColumnStatic[n]->GetWidth());
	}
	Add(pMunpList);
}
void CTopMunpDialog::LoadHeroList()
{
	pMunpPos = new cListCtrl;
	pMunpPos->Init(7, 417, 495, 0, NULL, -1);
	pMunpPos->Create(8);
	pMunpPos->InitListCtrl(8, 1);
	cImage tempImage;
	cImage BodyImage;
	cImage OverImage;
	pMunpPos->InitListCtrlImage(&tempImage, 1, &BodyImage, 1, &OverImage);
	for (int n = 0; n < 8; n++)
	{
		pMunpPos->InsertColoumn(n, "", pColumnStatic[n]->GetWidth());
	}
	Add(pMunpPos);
}
DWORD CTopMunpDialog::ActionEvent(CMouse* mouseInfo)
{
	DWORD we = cDialog::ActionEvent(mouseInfo);
	if (!m_bActive) return we;

	if (we & WE_MOUSEOVER)
	{		
	}
	if (we & WE_LBTNCLICK)
	{
	}

	if (we & WE_RBTNCLICK)
	{
		int b = pMunpList->PtIdxInRow(mouseInfo->GetMouseEventX(), mouseInfo->GetMouseEventY());
		if (WE_MOUSEOVER & pColumnStatic[7]->ActionEvent(mouseInfo))
		{
			if (checkName(pMunpData.TopInfo[b].szName2))
			{
				if (strcmp(pMunpData.TopInfo[b].szName2, HERO->GetObjectName()) != 0)
					GAMEIN->GetCharacterRBTNDlg()->ShowChatMenu(pMunpData.TopInfo[b].szName2);
			}
		}
	}
	return we;
}
bool CTopMunpDialog::checkName(char * c)
{
	for (int i = m_StartPos; i < m_EndPos; i++)
	{
		if (pMunpData.TopInfo[i].szName2 == c) return 1;
	}
	return 0;
}
void CTopMunpDialog::OnActionEvent(LONG lId, void * p, DWORD we)
{
	if (we & WE_PUSHDOWN)
	{
		if (RKG_BTN_PAGE01 <= lId && lId <= RKG_BTN_PAGE05)
		{

			for (int i = 0; i<5; i++)
			{
				pPageList[i]->SetPush(false);
			}
			m_TableIdx = lId - RKG_BTN_PAGE01;


			pPageList[m_TableIdx]->SetPush(true);
			SetPage(m_TableIdx + 1);

		}


	}
	else if (we & WE_PUSHUP)
	{
		if (RKG_BTN_PAGE01 <= lId && lId <= RKG_BTN_PAGE05)
		{
			pPageList[lId - RKG_BTN_PAGE01]->SetPush(true);
		}


	}
}

void CTopMunpDialog::SetMunpTopListInfo(SEND_TOPLIST * pListInfo)
{
	pMunpList->DeleteAllItems();
    memset(&pMunpData,0,sizeof(SEND_TOPLIST));
	memcpy(&pMunpData,pListInfo,sizeof(SEND_TOPLIST)); 
	if(pMunpData.wCount>0)
	{
		GAMEIN->GetTopIcon()->SetMunpFirst(pMunpData.TopInfo[0].szName1);
	}
	if(pDlg->IsActive())
		SetPage(1);
}
void CTopMunpDialog::GetMunpTopListInfo()
{
	MSG_DWORD msg;
	msg.Category=MP_USERCONN;
	msg.Protocol=MP_USERCONN_MUNP_TOPLIST_SYN;
	msg.dwObjectID= HEROID;
	msg.dwData=1;
	NETWORK->Send( &msg,sizeof(msg));  
}
void CTopMunpDialog::ShowMunpList()
{
	if(pDlg)
	{
		RANKBTNDLG->SetFlicker(false , GRANK);
		if(pUpdateTime)
		{
			pUpdateTime->SetStaticText(pMunpData.updateTime);
		}
		if(pMunpList)
		{
			pMunpList->DeleteAllItems();
			pMunpList->SetRenderImage(TRUE);             
			pMunpList->SetRenderColumnHeader(TRUE);      
			pMunpList->SetAlign(TXT_CENTER);             
     		cRITEMEx *ritem=NULL;
			if(pMunpData.wCount==0) return;  
			for(int i=m_StartPos; i<m_EndPos;i++)
			{
				cRITEMEx *ritem = new cRITEMEx(8);
				if(pMunpData.TopInfo[i].Key==1)  
				{
					GAMEIN->GetTopIcon()->SetMunpFirst(pMunpData.TopInfo[i].szName1);
				}
				if(pMunpData.TopInfo[i].Key<=3 )
				{
					SCRIPTMGR->GetImage(29+pMunpData.TopInfo[i].Key-1, &ritem->ItemImg[0], PFT_JACKPATH );
					sprintf(ritem->pString[0],"");
				}
				else if(pMunpData.TopInfo[i].Key>3 && pMunpData.TopInfo[i].Key<=8)
				{
					SCRIPTMGR->GetImage(32, &ritem->ItemImg[0], PFT_JACKPATH );
					sprintf(ritem->pString[0],"%d",pMunpData.TopInfo[i].Key);
				}
				else
				{
					sprintf(ritem->pString[0],"%d",pMunpData.TopInfo[i].Key);
				}


				if(pMunpData.TopInfo[i].Change>0)
				{
					SCRIPTMGR->GetImage(102, &ritem->ItemImg[1], PFT_JACKPATH );
				}
				if(pMunpData.TopInfo[i].Change==0)
				{
					SCRIPTMGR->GetImage(33, &ritem->ItemImg[1], PFT_JACKPATH );
				}
				if(pMunpData.TopInfo[i].Change<0)
				{
					SCRIPTMGR->GetImage(103, &ritem->ItemImg[1], PFT_JACKPATH );
				}
				sprintf(ritem->pString[1],"  %d",pMunpData.TopInfo[i].Change);



				if(pMunpData.TopInfo[i].Stage==eStage_Hwa)
				{
					SCRIPTMGR->GetImage(34, &ritem->ItemImg[2], PFT_JACKPATH );
				}
				if(pMunpData.TopInfo[i].Stage==eStage_Hyun)
				{
					SCRIPTMGR->GetImage(34, &ritem->ItemImg[2], PFT_JACKPATH );
				}
				if(pMunpData.TopInfo[i].Stage==eStage_Geuk)
				{
					SCRIPTMGR->GetImage(35, &ritem->ItemImg[2], PFT_JACKPATH );
				}
				if(pMunpData.TopInfo[i].Stage==eStage_Tal)
				{
					SCRIPTMGR->GetImage(35, &ritem->ItemImg[2], PFT_JACKPATH );
				}
				if(pMunpData.TopInfo[i].Stage==0)
				{
					SCRIPTMGR->GetImage(33, &ritem->ItemImg[2], PFT_JACKPATH );
				}
				sprintf(ritem->pString[2],"");
				int MarkTemp=pMunpData.TopInfo[i].ObjectId;
				if(MarkTemp!=0||MarkTemp!=NULL)
				{
					CGuildMark * Mark = GUILDMARKMGR->GetGuildMark((MARKNAMETYPE)pMunpData.TopInfo[i].ObjectId);
					if(Mark)
					{
						CGuildMarkImg * MarkImg= Mark->GetGuildMarmImg();
						if(MarkImg)
						{
							ritem->ItemImg[3].SetSpriteObject(MarkImg->GetMarkImg()->GetSpriteObject());
							ritem->ItemImg[3].SetImageSrcRect(MarkImg->GetMarkImg()->GetImageRect());
							ritem->ItemImg[3].SetImageSrcSize(MarkImg->GetMarkImg()->GetImageSize());
						}
					}
					sprintf(ritem->pString[3],"");
				}
				else
					sprintf(ritem->pString[3],"");



				if(pMunpData.TopInfo[i].Key<=10)
				{
			   		if(pMunpData.TopInfo[i].Key<=3)
					{
					   ritem->rgb[4] = RGBA_MAKE(255,100+50*i,100+60*i,m_alpha * m_dwOptionAlpha / 100);
					}
					else if(pMunpData.TopInfo[i].Key<=8)
					{
					   ritem->rgb[4] = RGBA_MAKE(249,252,225,m_alpha * m_dwOptionAlpha / 100);
					}
				}

				ritem->rgb[5] = RGBA_MAKE(255,250,205,255);
				ritem->rgb[6] = RGBA_MAKE(255,250,205,255);
				ritem->rgb[7] = RGBA_MAKE(255,255,255,255);

				sprintf(ritem->pString[4],pMunpData.TopInfo[i].szName1);
				sprintf(ritem->pString[5],"%d",pMunpData.TopInfo[i].nData1);				
				sprintf(ritem->pString[6],"%d",pMunpData.TopInfo[i].nData2);				
				sprintf(ritem->pString[7],pMunpData.TopInfo[i].szName2);



				pMunpList->InsertItem(i, ritem);
			}
			int Count =pMunpData.wCount;
			int PageNum= Count/PAGELISTNUM;
			int PageNext=Count%PAGELISTNUM; 
			for(int i=0;i<5;i++)
			{
				pPageList[i]->SetActive(FALSE);
			}
			if(PageNext!=0)
			{
				PageNum=PageNum+1;
			}
			for(int i=0;i<PageNum;i++)
			{
				pPageList[i]->SetActive(TRUE);
			}
			//SetMunpPos();
		}
	}
}
void CTopMunpDialog::InitMunpList()
{
	m_StartPos=0;
	if(pMunpData.wCount<PAGELISTNUM)
	{
		m_EndPos=pMunpData.wCount;
	}
	else
	{
		m_EndPos=PAGELISTNUM;
	}
}
void CTopMunpDialog::SetMunpPage( int PageIndex)
{
	int Count =pMunpData.wCount;
	int PageNum= Count/PAGELISTNUM;
	int PageNext=Count%PAGELISTNUM; 
	m_StartPos=(PageIndex-1)*PAGELISTNUM;
	if(PageNext==0)
	{
	   m_EndPos=(PageIndex)*PAGELISTNUM;
	}
	else
	{
	   m_EndPos=(PageNum)*PAGELISTNUM+PageNext;
	}
}
void CTopMunpDialog::SetPage(int PageIndex)
{
	SetMunpPage(PageIndex);
	ShowMunpList();
}
void CTopMunpDialog::OpenLastTab()
{
	if (m_TableIdx != 0)
	{
		pPageList[m_TableIdx]->SetPush(true);
		SetPage(m_TableIdx + 1);
	}
	else
		pPageList[0]->SetPush(true);
}