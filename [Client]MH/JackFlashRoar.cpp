#include "Stdafx.h"
#include "JackFlashRoar.h"
#include "cWindowManager.h"
#include "cScriptManager.h"
#include "WindowIDEnum.h"
#include "ChatManager.h"
#include "aimanager.h"
#include "MHTimeManager.h"
#define ALPHA_PROCESS_TIME	500
CJackFlashMsg::CJackFlashMsg()
{
	m_type = WT_JACFLASH_DLG;
	pDlg = NULL;
	ShowText = NULL;
	BackImage = NULL;
	bo = 0;
	boStop = false;
	GoRender = false;
	FinalFadeInit = false;
	FinalFade = false;
	//pMessageTable.Initialize(1024);
	ZeroMemory(TxtFirst, sizeof(TxtFirst));
	ZeroMemory(TxtSecond, sizeof(TxtSecond));
	//TxtFirst=NULL;
}
CJackFlashMsg::~CJackFlashMsg()
{
	
}
void CJackFlashMsg::Linking()
{
	SCRIPTMGR->GetImage(47, &m_Image, PFT_JACKPATH);
	pDlg = WINDOWMGR->GetWindowForID(JACK_FLASH_ROAR);
	pDlg->SetActive(false);
	pDlg->SetAlpha(0);
	BackImage = new cStatic;
	BackImage->Init(0, 0, 0, 0, NULL, -1);
	BackImage->SetBasicImage(&m_Image);
	Add(BackImage);
	ShowText = new cStatic;
	ShowText->Init(110, 10, 0, 0, NULL, -1);
	ShowText->SetShadow(TRUE);
	ShowText->SetFontIdx(0);
	ShowText->SetAlign(4);
	ShowText->SetStaticText("");
	ShowText->SetAlpha(0);
	Add(ShowText);
}
void CJackFlashMsg::Render()
{
	if (!sCPlayerAI.IsRun)return;
	AlphaRender();
	cDialog::Render();
}
void CJackFlashMsg::SetMapChange(BOOL val)
{
	if (val)
		Kill();
	cDialog::SetActive(!val);
}
void CJackFlashMsg::AlphaRender()
{
	//if(false==GoRender) return;
	/*switch(boStop)
	{
	case FALSE:
	{
	if(bo<150)
	bo=bo+13;
	else if(bo<255)
	bo=bo+3;
	else
	boStop=true;
	break;
	}
	case TRUE:
	{
	if(bo>5)
	bo=bo-15;
	else
	{
	bo=0;
	pDlg->SetActive(false);
	GoRender=false;
	boStop=false;
	if(*TxtFirst!=0)
	{
	//AddMsg(TxtFirst);
	ZeroMemory( TxtFirst, sizeof(TxtFirst) );
	}
	if(*TxtSecond!=0)
	{
	AddMsg(TxtSecond);
	ZeroMemory( TxtSecond, sizeof(TxtSecond) );
	}
	//for(int i=0;i<65533;i++)
	//{
	//	if(TempMsg(i)!=NULL)
	//		AddMsg(TempMsg(i),TempColor(i));
	//}
	}
	break;
	}
	}*/
	pDlg->SetAlpha(bo);
	if (GoRender)
	{
		DWORD dwElapsed = MHTIMEMGR->GetNewCalcCurTime() - m_dwStartTime;
		if (dwElapsed > (ALPHA_PROCESS_TIME-50))
		{
			bo = 255;
			//pDlg->SetAlpha(bo);
			GoRender = FALSE;
			FinalFadeInit = TRUE;
		}
		else
		{
			bo = (BYTE)(0 + (dwElapsed * 255 / (ALPHA_PROCESS_TIME-50)));
			//pDlg->SetAlpha(bo);
		}
	}
	if (FinalFadeInit)
	{
		FinalFadeInit = FALSE;
		m_dwStartTime = MHTIMEMGR->GetNewCalcCurTime();
		FinalFade = TRUE;
	}
	if (FinalFade)
	{
		DWORD dwElapsed = MHTIMEMGR->GetNewCalcCurTime() - m_dwStartTime;
		if (dwElapsed > (ALPHA_PROCESS_TIME+50))
		{
			FinalFade = FALSE;
			bo = 0;
			//pDlg->SetAlpha(bo);
			pDlg->SetActive(false);
			//WINDOWMGR->AddListDestroyWindow(m_pLogoWindow);
			//m_pLogoWindow = NULL;
			//m_bServerList = TRUE;
		}
		else
		{
			bo = (BYTE)(255 - (dwElapsed * 255 / (ALPHA_PROCESS_TIME+50)));
			//pDlg->SetAlpha(bo);
		}
	}
	//pDlg->SetAlpha(bo);
}
void CJackFlashMsg::AddMsg(char * pName, DWORD color)
{
	//if(GoRender)
	//if (pDlg->IsActive())
	if (bo != 0)
	{
		//BackupMessage(pName,color,GetIndex());
		SafeStrCpy(TxtSecond, pName, 128);
		return;
	}
	static DWORD dwTimeTick;
	if (gCurTime - dwTimeTick >= ALPHA_PROCESS_TIME)
	{
		//if (*TxtFirst == 0)
		SafeStrCpy(TxtFirst, pName, 128);
		GoRender = true;
		m_dwStartTime = MHTIMEMGR->GetNewCalcCurTime();
		dwTimeTick = gCurTime;
		pDlg->SetActive(true);
		ShowText->SetFGColor(color);
		ShowText->SetStaticText(TxtFirst);
		pDlg->SetAlwaysTop(TRUE);
	}
}
