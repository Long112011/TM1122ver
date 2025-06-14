#include "stdafx.h"
#include "AISetManager.h"
#include "aimanager.h"
#include "cDialog.h"
#include "./interface/cCheckbox.h"
#include "./Interface/cPushupButton.h"
#include "cSpin.h"
#include "OptionDialog.h"
#include "GameIn.h"
#include "QuickManager.h"
#include "QuickItem.h"
#include "QuickDialog.h"
#include "ChatManager.h"
#include "SkillManager_client.h"
#include "MHMap.h"
#include "WindowIDEnum.h"
#include "UserInfoManager.h"
#include "cIMEEX.h"
GLOBALTON(CAiSetManager);
CAiSetManager::CAiSetManager()
{
}
CAiSetManager::~CAiSetManager()
{
}
void CAiSetManager::InitForGameIn()
{
	if (!LoadGameAI())
		SetDefaultOption();
}
void CAiSetManager::SetGameAI(sGAMEAI* pOptionAI)
{
	m_GameAI = *pOptionAI;
	SaveGameOptionAI();
}
bool CAiSetManager::FindErrorPath()
{
	COptionDialog * m_Dlg = GAMEIN->GetOptionDialog();
	cDialog* pDlg;
	VECTOR3	vDestPosAuto;
	vDestPosAuto.x = atoi(((cSpin*)((cDialog*)m_Dlg->GetTabSheet(4))->GetWindowForID(OTI_SP_AUTOHUNT_LOCA_X))->GetEditText()) * 100;
	vDestPosAuto.y = 0;
	vDestPosAuto.z = atoi(((cSpin*)((cDialog*)m_Dlg->GetTabSheet(4))->GetWindowForID(OTI_SP_AUTOHUNT_LOCA_Y))->GetEditText()) * 100;
	if (MAP->CollisionCheck_OneTile(&vDestPosAuto) == FALSE)
	{
		SetErrorPathInfo(true);
		return true;
	}
	else
	{
		SetErrorPathInfo(false);
	}
	return false;
}
bool CAiSetManager::FindErrorPathFromAISet()
{
	COptionDialog * m_Dlg = GAMEIN->GetOptionDialog();
	cDialog* pDlg;
	VECTOR3	vDestPosAuto;
	vDestPosAuto.x = (float)AISETMGR->GetGameAI()->sPage5.spPosX * 100;
	vDestPosAuto.y = 0;
	vDestPosAuto.z = (float)AISETMGR->GetGameAI()->sPage5.spPosY * 100;
	if (MAP->CollisionCheck_OneTile(&vDestPosAuto) == FALSE)
	{
		SetErrorPathInfo(true);
		return true;
	}
	else
	{
		SetErrorPathInfo(false);
	}
	return false;
}
void CAiSetManager::SetErrorPathInfo(BOOL bDisable)
{
	COptionDialog * m_Dlg = GAMEIN->GetOptionDialog();
	//cDialog* pDlg;
	DWORD FullColor = 0xffffffff;
	DWORD HalfColor = RGB_HALF(255, 20, 20);
	if (bDisable)
	{
		FullColor = HalfColor;
	}
	((cSpin*)((cDialog*)m_Dlg->GetTabSheet(4))->GetWindowForID(OTI_SP_AUTOHUNT_LOCA_X))->SetNonactiveTextColor(FullColor);
	((cSpin*)((cDialog*)m_Dlg->GetTabSheet(4))->GetWindowForID(OTI_SP_AUTOHUNT_LOCA_Y))->SetNonactiveTextColor(FullColor);
	((cSpin*)((cDialog*)m_Dlg->GetTabSheet(4))->GetWindowForID(OTI_SP_AUTOHUNT_LOCA_X))->SetActiveTextColor(FullColor);
	((cSpin*)((cDialog*)m_Dlg->GetTabSheet(4))->GetWindowForID(OTI_SP_AUTOHUNT_LOCA_Y))->SetActiveTextColor(FullColor);
	((cStatic*)((cDialog*)m_Dlg->GetTabSheet(4))->GetWindowForID(OTI_STC_AUTOHUNT_LOCA))->SetFGColor(FullColor);
}
BOOL CAiSetManager::LoadGameAI()
{
	char strFilePath[MAX_PATH];
	wsprintf(strFilePath, "%s\\Data\\%d\\%d.UIE", DIRECTORYMGR->GetFullDirectoryName(eLM_Root), USERINFOMGR->GetSaveFolderName(), USERINFOMGR->GetSaveFolderName());
	HANDLE	hFile = CreateFile(strFilePath, GENERIC_READ, 0, NULL,
							   OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile == INVALID_HANDLE_VALUE)
		return FALSE;
	DWORD dwRead = 0;
	DWORD dwVersion;
	if (!ReadFile(hFile, &dwVersion, sizeof(DWORD), &dwRead, NULL))
	{
		CloseHandle(hFile);
		return FALSE;
	}
	if (dwVersion != 0x00061208)
	{
		CloseHandle(hFile);
		return FALSE;
	}
	if (!ReadFile(hFile, &m_GameAI, sizeof(m_GameAI), &dwRead, NULL))
	{
		CloseHandle(hFile);
		return FALSE;
	}
	if (dwRead > sizeof(m_GameAI))
	{
		CloseHandle(hFile);
		return FALSE;
	}
	CloseHandle(hFile);
	return TRUE;
}
void CAiSetManager::SaveGameOptionAI()
{
	char strFilePath[MAX_PATH];
	wsprintf(strFilePath, "%s\\Data\\%d\\%d.UIE", DIRECTORYMGR->GetFullDirectoryName(eLM_Root), USERINFOMGR->GetSaveFolderName(), USERINFOMGR->GetSaveFolderName());
	HANDLE	hFile = CreateFile(strFilePath, GENERIC_WRITE, 0, NULL,
							   CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile == INVALID_HANDLE_VALUE)
		return;
	DWORD dwWritten;
	DWORD dwVersion = 0x00061208;
	WriteFile(hFile, &dwVersion, sizeof(DWORD), &dwWritten, NULL);
	WriteFile(hFile, &m_GameAI, sizeof(m_GameAI), &dwWritten, NULL);
	CloseHandle(hFile);
}
void CAiSetManager::SetDefaultOption()
{
	m_GameAI.sPage5.pbAutoLeveling = FALSE;
	m_GameAI.sPage5.pbSpawnHere = 1;
	m_GameAI.sPage5.cbAreaXY = FALSE;
	m_GameAI.sPage5.cbAutoRespawn = FALSE;
	m_GameAI.sPage5.cbAutoRespawnTimeLimit = FALSE;
	m_GameAI.sPage5.cbAutoSitHP = FALSE;
	m_GameAI.sPage5.cbAutoSitMP = FALSE;
	m_GameAI.sPage5.cbAutoSitSP = FALSE;
	m_GameAI.sPage5.cbSitNoTarget = FALSE;
	m_GameAI.sPage5.cbSitAttacked = FALSE;
	m_GameAI.sPage5.cbNoMonsterWall = FALSE;
	m_GameAI.sPage5.spPosX = 250;
	m_GameAI.sPage5.spPosY = 250;
	m_GameAI.sPage5.spRange = 50;
	m_GameAI.sPage5.spSitHP = 30;
	m_GameAI.sPage5.spSitMP = 30;
	m_GameAI.sPage5.spSitSP = 30;
	m_GameAI.sPage5.spReviveTimeLimit = 8;
	m_GameAI.sPage5.spReviveTime = 1;
	m_GameAI.sPage5.spSitAttacked = 2;
	m_GameAI.sPage5.cbIgnoreBoss = FALSE;
	m_GameAI.sPage6.bSkill1 = FALSE;
	m_GameAI.sPage6.bSkill2 = FALSE;
	m_GameAI.sPage6.bSkill3 = FALSE;
	m_GameAI.sPage6.bSkill4 = FALSE;
	m_GameAI.sPage6.bSkill5 = FALSE;
	m_GameAI.sPage6.bSkill1key = 3;
	m_GameAI.sPage6.bSkill2key = 4;
	m_GameAI.sPage6.bSkill3key = 5;
	m_GameAI.sPage6.bSkill4key = 6;
	m_GameAI.sPage6.bSkill5key = 7;
	m_GameAI.sPage6.bSkill1page = 1;
	m_GameAI.sPage6.bSkill2page = 1;
	m_GameAI.sPage6.bSkill3page = 1;
	m_GameAI.sPage6.bSkill4page = 1;
	m_GameAI.sPage6.bSkill5page = 1;
	m_GameAI.sPage6.bSkill1time = 2;
	m_GameAI.sPage6.bSkill2time = 2;
	m_GameAI.sPage6.bSkill3time = 2;
	m_GameAI.sPage6.bSkill4time = 2;
	m_GameAI.sPage6.bSkill5time = 2;
	m_GameAI.sPage6.bHelpSkill1 = FALSE;
	m_GameAI.sPage6.bHelpSkill2 = FALSE;
	m_GameAI.sPage6.bHelpSkill3 = FALSE;
	m_GameAI.sPage6.bHelpSkill4 = FALSE;
	m_GameAI.sPage6.bHelpSkill5 = FALSE;
	m_GameAI.sPage6.bHelpSkill6 = FALSE;
	m_GameAI.sPage6.bHelpSkill7 = FALSE;
	m_GameAI.sPage6.bHelpSkill8 = FALSE;
	m_GameAI.sPage6.bHelpSkill9 = FALSE;
	m_GameAI.sPage6.bHelpSkill10 = FALSE;
	m_GameAI.sPage6.bTunaSpin = 75;
	m_GameAI.sPage7.ProtectLifeOpen1 = FALSE;
	m_GameAI.sPage7.ProtectLifeOpen2 = FALSE;
	m_GameAI.sPage7.ProtectLifeOpen3 = FALSE;
	m_GameAI.sPage7.ProtectMagicOpen1 = FALSE;
	m_GameAI.sPage7.ProtectMagicOpen2 = FALSE;
	m_GameAI.sPage7.ProtectShieldOpen1 = FALSE;
	m_GameAI.sPage7.ProtectShieldOpen2 = FALSE;
	m_GameAI.sPage7.ProtectLifePecent1 = 65;
	m_GameAI.sPage7.ProtectLifePecent2 = 45;
	m_GameAI.sPage7.ProtectLifePecent3 = 20;
	m_GameAI.sPage7.ProtectLifeTime1 = 4;
	m_GameAI.sPage7.ProtectLifeTime2 = 4;
	m_GameAI.sPage7.ProtectLifeTime3 = 4;
	m_GameAI.sPage7.ProtectLifeKey1 = 8;
	m_GameAI.sPage7.ProtectLifeKey2 = 8;
	m_GameAI.sPage7.ProtectLifeKey3 = 8;
	m_GameAI.sPage7.ProtectLifePage1 = 1;
	m_GameAI.sPage7.ProtectLifePage2 = 1;
	m_GameAI.sPage7.ProtectLifePage3 = 1;
	m_GameAI.sPage7.ProtectMagicPecent1 = 60;
	m_GameAI.sPage7.ProtectMagicPecent2 = 30;
	m_GameAI.sPage7.ProtectMagicTime1 = 4;
	m_GameAI.sPage7.ProtectMagicTime2 = 4;
	m_GameAI.sPage7.ProtectMagicKey1 = 9;
	m_GameAI.sPage7.ProtectMagicKey2 = 9;
	m_GameAI.sPage7.ProtectMagicPage1 = 1;
	m_GameAI.sPage7.ProtectMagicPage2 = 1;
	m_GameAI.sPage7.ProtectShieldPecent1 = 60;
	m_GameAI.sPage7.ProtectShieldPecent2 = 30;
	m_GameAI.sPage7.ProtectShieldTime1 = 4;
	m_GameAI.sPage7.ProtectShieldTime2 = 4;
	m_GameAI.sPage7.ProtectShieldKey1 = 0;
	m_GameAI.sPage7.ProtectShieldKey2 = 0;
	m_GameAI.sPage7.ProtectShieldPage1 = 1;
	m_GameAI.sPage7.ProtectShieldPage2 = 1;
	m_GameAI.sPage8.m_AutoSkill1 = FALSE;
	m_GameAI.sPage8.m_AutoSkill2 = FALSE;
	m_GameAI.sPage8.m_AutoSkill3 = FALSE;
	m_GameAI.sPage8.m_AutoSkill4 = FALSE;
	m_GameAI.sPage8.m_AutoSkill5 = FALSE;
	m_GameAI.sPage8.m_AutoSkill6 = FALSE;
	m_GameAI.sPage8.h_AutoSkill1 = FALSE;
	m_GameAI.sPage8.h_AutoSkill2 = FALSE;
	m_GameAI.sPage8.h_AutoSkill3 = FALSE;
	m_GameAI.sPage8.h_AutoSkill4 = FALSE;
	m_GameAI.sPage8.h_AutoSkill5 = FALSE;
	m_GameAI.sPage8.h_AutoSkill6 = FALSE;
	m_GameAI.sPage8.isReConnServer = FALSE;
	m_GameAI.sPage8.isAutoCallPet = FALSE;
	m_GameAI.sPage8.isAutoUseBuffItem = FALSE;
	m_GameAI.sPage8.isAutoCloseGetItemWin = FALSE;
	m_GameAI.sPage8.isAutoJoinGroup = FALSE;
	m_GameAI.sPage8.isAutoWant = FALSE;
	m_GameAI.sPage8.WantMoney = 0;
	m_GameAI.sPage8.isAutoAttrPlay = FALSE;
	m_GameAI.sPage8.FuoMoo = FALSE;
	m_GameAI.sPage8.cbReconnectMap = FALSE;
	m_GameAI.sPage8.spReconnectMap = 1;
	m_GameAI.sPage8.spPetEatPercent = 100;
}
static int dataCheck = 9999;
void CAiSetManager::SaveData()
{

	//int dataSpin = -1;
	COptionDialog * m_Dlg = GAMEIN->GetOptionDialog();
	cDialog* pDlg;
	pDlg = (cDialog*)m_Dlg->GetTabSheet(4);

	dataCheck = ((cPushupButton*)pDlg->GetWindowForID(OTI_PB_AUTOHUNT_REVIVE_PRESENT))->IsPushed(); if (m_GameAI.sPage5.pbSpawnHere != dataCheck)m_GameAI.sPage5.pbSpawnHere = dataCheck; else dataCheck = 9999;
	dataCheck = ((cPushupButton*)pDlg->GetWindowForID(OTI_PB_AUTOHUNT_ON))->IsPushed(); if (m_GameAI.sPage5.pbAutoLeveling != dataCheck)m_GameAI.sPage5.pbAutoLeveling = dataCheck; else dataCheck = 9999;
	dataCheck = ((cCheckBox*)pDlg->GetWindowForID(OTI_CB_AUTOHUNT_LOCA))->IsChecked(); if (m_GameAI.sPage5.cbAreaXY != dataCheck) m_GameAI.sPage5.cbAreaXY = dataCheck; else dataCheck = 9999;
	dataCheck = ((cCheckBox*)pDlg->GetWindowForID(OTI_CB_AUTOUNGI))->IsChecked(); if (m_GameAI.sPage5.cbAutoSitHP != dataCheck) m_GameAI.sPage5.cbAutoSitHP = dataCheck; else dataCheck = 9999;
	dataCheck = ((cCheckBox*)pDlg->GetWindowForID(OTI_CB_AUTOHUNT_UNGI_MP))->IsChecked(); if (m_GameAI.sPage5.cbAutoSitMP != dataCheck) m_GameAI.sPage5.cbAutoSitMP = dataCheck; else dataCheck = 9999;
	dataCheck = ((cCheckBox*)pDlg->GetWindowForID(OTI_CB_AUTOHUNT_UNGI_SP))->IsChecked(); if (m_GameAI.sPage5.cbAutoSitSP != dataCheck) m_GameAI.sPage5.cbAutoSitSP = dataCheck; else dataCheck = 9999;
	dataCheck = ((cCheckBox*)pDlg->GetWindowForID(OTI_CB_AUTOHUNT_REVIVE))->IsChecked(); if (m_GameAI.sPage5.cbAutoRespawn != dataCheck)m_GameAI.sPage5.cbAutoRespawn = dataCheck; else dataCheck = 9999;
	dataCheck = ((cCheckBox*)pDlg->GetWindowForID(OTI_CB_AUTOHUNT_REVIVE_LIMITCOUNT))->IsChecked(); if (m_GameAI.sPage5.cbAutoRespawnTimeLimit != dataCheck) m_GameAI.sPage5.cbAutoRespawnTimeLimit = dataCheck; else dataCheck = 9999;
	dataCheck = ((cCheckBox*)pDlg->GetWindowForID(OTI_CB_AUTOHUNT_UNGI_AT_NOTARGET))->IsChecked(); if (m_GameAI.sPage5.cbSitNoTarget != dataCheck) m_GameAI.sPage5.cbSitNoTarget = dataCheck; else dataCheck = 9999;
	dataCheck = ((cCheckBox*)pDlg->GetWindowForID(OTI_CB_AUTOHUNT_UNGI_ATTACKED))->IsChecked(); if (m_GameAI.sPage5.cbSitAttacked != dataCheck) m_GameAI.sPage5.cbSitAttacked = dataCheck; else dataCheck = 9999;
	dataCheck = ((cCheckBox*)pDlg->GetWindowForID(OTI_CB_AUTOHUNT_MONSTERWALL))->IsChecked(); if (m_GameAI.sPage5.cbNoMonsterWall != dataCheck)m_GameAI.sPage5.cbNoMonsterWall = dataCheck; else dataCheck = 9999;
	dataCheck = ((cCheckBox*)pDlg->GetWindowForID(OTI_CB_IGNORE_BOSSES))->IsChecked(); if (m_GameAI.sPage5.cbIgnoreBoss != dataCheck)m_GameAI.sPage5.cbIgnoreBoss = dataCheck; else dataCheck = 9999;
	dataCheck = atoi(((cSpin*)pDlg->GetWindowForID(OTI_SP_AUTOHUNT_LOCA_X))->GetEditText()); if (m_GameAI.sPage5.spPosX != dataCheck)m_GameAI.sPage5.spPosX = dataCheck; else dataCheck = 9999;
	dataCheck = atoi(((cSpin*)pDlg->GetWindowForID(OTI_SP_AUTOHUNT_LOCA_Y))->GetEditText()); if (m_GameAI.sPage5.spPosY != dataCheck) m_GameAI.sPage5.spPosY = dataCheck; else dataCheck = 9999;
	dataCheck = atoi(((cSpin*)pDlg->GetWindowForID(OTI_SP_AUTOHUNT_RANGE))->GetEditText()); if (m_GameAI.sPage5.spRange != dataCheck) m_GameAI.sPage5.spRange = dataCheck; else dataCheck = 9999;
	dataCheck = atoi(((cSpin*)pDlg->GetWindowForID(OTI_SP_AUTOHUNT_UNGI_HP))->GetEditText()); if (m_GameAI.sPage5.spSitHP != dataCheck) m_GameAI.sPage5.spSitHP = dataCheck; else dataCheck = 9999;
	dataCheck = atoi(((cSpin*)pDlg->GetWindowForID(OTI_SP_AUTOHUNT_UNGI_MP))->GetEditText()); if (m_GameAI.sPage5.spSitMP != dataCheck) m_GameAI.sPage5.spSitMP = dataCheck; else dataCheck = 9999;
	dataCheck = atoi(((cSpin*)pDlg->GetWindowForID(OTI_SP_AUTOHUNT_UNGI_SP))->GetEditText()); if (m_GameAI.sPage5.spSitSP != dataCheck) m_GameAI.sPage5.spSitSP = dataCheck; else dataCheck = 9999;
	dataCheck = atoi(((cSpin*)pDlg->GetWindowForID(OTI_SP_AUTOHUNT_REVIVE_TIME))->GetEditText()); if (m_GameAI.sPage5.spReviveTime != dataCheck)m_GameAI.sPage5.spReviveTime = dataCheck; else dataCheck = 9999;
	dataCheck = atoi(((cSpin*)pDlg->GetWindowForID(OTI_SP_AUTOHUNT_REVIVE_LIMITCOUNT))->GetEditText()); if (m_GameAI.sPage5.spReviveTimeLimit != dataCheck)m_GameAI.sPage5.spReviveTimeLimit = dataCheck; else dataCheck = 9999;
	dataCheck = atoi(((cSpin*)pDlg->GetWindowForID(OTI_SP_AUTOHUNT_UNGI_ATTACKED))->GetEditText()); if (m_GameAI.sPage5.spSitAttacked != dataCheck)m_GameAI.sPage5.spSitAttacked = dataCheck; else dataCheck = 9999;
	/*m_GameAI.sPage5.pbAutoLeveling = ((cPushupButton*)pDlg->GetWindowForID(OTI_PB_AUTOHUNT_ON))->IsPushed();
	m_GameAI.sPage5.cbAreaXY = ((cCheckBox*)pDlg->GetWindowForID(OTI_CB_AUTOHUNT_LOCA))->IsChecked();
	m_GameAI.sPage5.cbAutoSitHP = ((cCheckBox*)pDlg->GetWindowForID(OTI_CB_AUTOUNGI))->IsChecked();
	m_GameAI.sPage5.cbAutoSitMP = ((cCheckBox*)pDlg->GetWindowForID(OTI_CB_AUTOHUNT_UNGI_MP))->IsChecked();
	m_GameAI.sPage5.cbAutoSitSP = ((cCheckBox*)pDlg->GetWindowForID(OTI_CB_AUTOHUNT_UNGI_SP))->IsChecked();
	m_GameAI.sPage5.cbAutoRespawn = ((cCheckBox*)pDlg->GetWindowForID(OTI_CB_AUTOHUNT_REVIVE))->IsChecked();
	m_GameAI.sPage5.cbAutoRespawnTimeLimit = ((cCheckBox*)pDlg->GetWindowForID(OTI_CB_AUTOHUNT_REVIVE_LIMITCOUNT))->IsChecked();
	m_GameAI.sPage5.cbSitNoTarget = ((cCheckBox*)pDlg->GetWindowForID(OTI_CB_AUTOHUNT_UNGI_AT_NOTARGET))->IsChecked();
	m_GameAI.sPage5.cbSitAttacked = ((cCheckBox*)pDlg->GetWindowForID(OTI_CB_AUTOHUNT_UNGI_ATTACKED))->IsChecked();
	m_GameAI.sPage5.cbNoMonsterWall = ((cCheckBox*)pDlg->GetWindowForID(OTI_CB_AUTOHUNT_MONSTERWALL))->IsChecked();
	m_GameAI.sPage5.spPosX = atoi(((cSpin*)pDlg->GetWindowForID(OTI_SP_AUTOHUNT_LOCA_X))->GetEditText());
	m_GameAI.sPage5.spPosY = atoi(((cSpin*)pDlg->GetWindowForID(OTI_SP_AUTOHUNT_LOCA_Y))->GetEditText());
	m_GameAI.sPage5.spRange = atoi(((cSpin*)pDlg->GetWindowForID(OTI_SP_AUTOHUNT_RANGE))->GetEditText());
	m_GameAI.sPage5.spSitHP = atoi(((cSpin*)pDlg->GetWindowForID(OTI_SP_AUTOHUNT_UNGI_HP))->GetEditText());
	m_GameAI.sPage5.spSitMP = atoi(((cSpin*)pDlg->GetWindowForID(OTI_SP_AUTOHUNT_UNGI_MP))->GetEditText());
	m_GameAI.sPage5.spSitSP = atoi(((cSpin*)pDlg->GetWindowForID(OTI_SP_AUTOHUNT_UNGI_SP))->GetEditText());
	m_GameAI.sPage5.spReviveTime = atoi(((cSpin*)pDlg->GetWindowForID(OTI_SP_AUTOHUNT_REVIVE_TIME))->GetEditText());
	m_GameAI.sPage5.spReviveTimeLimit = atoi(((cSpin*)pDlg->GetWindowForID(OTI_SP_AUTOHUNT_REVIVE_LIMITCOUNT))->GetEditText());
	m_GameAI.sPage5.spSitAttacked = atoi(((cSpin*)pDlg->GetWindowForID(OTI_SP_AUTOHUNT_UNGI_ATTACKED))->GetEditText());
	m_GameAI.sPage5.pbSpawnHere = ((cPushupButton*)pDlg->GetWindowForID(OTI_PB_AUTOHUNT_REVIVE_PRESENT))->IsPushed();
	m_GameAI.sPage5.cbIgnoreBoss = ((cCheckBox*)pDlg->GetWindowForID(OTI_CB_IGNORE_BOSSES))->IsChecked();*/

	pDlg = (cDialog*)m_Dlg->GetTabSheet(5);

	dataCheck = ((cCheckBox*)pDlg->GetWindowForID(OTI_CB_AUTOHUNT_TIMER_01))->IsChecked(); if (m_GameAI.sPage6.bSkill1 != dataCheck)m_GameAI.sPage6.bSkill1 = dataCheck; else dataCheck = 9999;
	dataCheck = ((cCheckBox*)pDlg->GetWindowForID(OTI_CB_AUTOHUNT_TIMER_02))->IsChecked(); if (m_GameAI.sPage6.bSkill2 != dataCheck)m_GameAI.sPage6.bSkill2 = dataCheck; else dataCheck = 9999;
	dataCheck = ((cCheckBox*)pDlg->GetWindowForID(OTI_CB_AUTOHUNT_TIMER_03))->IsChecked(); if (m_GameAI.sPage6.bSkill3 != dataCheck)m_GameAI.sPage6.bSkill3 = dataCheck; else dataCheck = 9999;
	dataCheck = ((cCheckBox*)pDlg->GetWindowForID(OTI_CB_AUTOHUNT_TIMER_04))->IsChecked(); if (m_GameAI.sPage6.bSkill4 != dataCheck)m_GameAI.sPage6.bSkill4 = dataCheck; else dataCheck = 9999;
	dataCheck = ((cCheckBox*)pDlg->GetWindowForID(OTI_CB_AUTOHUNT_TIMER_05))->IsChecked(); if (m_GameAI.sPage6.bSkill5 != dataCheck)m_GameAI.sPage6.bSkill5 = dataCheck; else dataCheck = 9999;
	dataCheck = ((cCheckBox*)pDlg->GetWindowForID(OTI_CB_AUTODOWNDEFENCE))->IsChecked(); if (m_GameAI.sPage6.bHelpSkill1 != dataCheck)m_GameAI.sPage6.bHelpSkill1 = dataCheck; else dataCheck = 9999;
	dataCheck = ((cCheckBox*)pDlg->GetWindowForID(OTI_CB_AUTODOWNATTACK))->IsChecked(); if (m_GameAI.sPage6.bHelpSkill2 != dataCheck)m_GameAI.sPage6.bHelpSkill2 = dataCheck; else dataCheck = 9999;
	dataCheck = ((cCheckBox*)pDlg->GetWindowForID(OTI_CB_AUTOFIRE))->IsChecked(); if (m_GameAI.sPage6.bHelpSkill3 != dataCheck)m_GameAI.sPage6.bHelpSkill3 = dataCheck; else dataCheck = 9999;
	dataCheck = ((cCheckBox*)pDlg->GetWindowForID(OTI_CB_AUTOWATER))->IsChecked(); if (m_GameAI.sPage6.bHelpSkill4 != dataCheck)m_GameAI.sPage6.bHelpSkill4 = dataCheck; else dataCheck = 9999;
	dataCheck = ((cCheckBox*)pDlg->GetWindowForID(OTI_CB_AUTOTREE))->IsChecked(); if (m_GameAI.sPage6.bHelpSkill5 != dataCheck)m_GameAI.sPage6.bHelpSkill5 = dataCheck; else dataCheck = 9999;
	dataCheck = ((cCheckBox*)pDlg->GetWindowForID(OTI_CB_AUTOIRON))->IsChecked(); if (m_GameAI.sPage6.bHelpSkill6 != dataCheck)m_GameAI.sPage6.bHelpSkill6 = dataCheck; else dataCheck = 9999;
	dataCheck = ((cCheckBox*)pDlg->GetWindowForID(OTI_CB_AUTOEARTH))->IsChecked(); if (m_GameAI.sPage6.bHelpSkill7 != dataCheck)m_GameAI.sPage6.bHelpSkill7 = dataCheck; else dataCheck = 9999;
	dataCheck = ((cCheckBox*)pDlg->GetWindowForID(OTI_CB_AUTOEARTHGUARD))->IsChecked(); if (m_GameAI.sPage6.bHelpSkill8 != dataCheck)m_GameAI.sPage6.bHelpSkill8 = dataCheck; else dataCheck = 9999;
	dataCheck = ((cCheckBox*)pDlg->GetWindowForID(OTI_CB_AUTOSKIPEFFECT))->IsChecked(); if (m_GameAI.sPage6.bHelpSkill9 != dataCheck)m_GameAI.sPage6.bHelpSkill9 = dataCheck; else dataCheck = 9999;
	dataCheck = ((cCheckBox*)pDlg->GetWindowForID(OTI_CB_AUTOADDLIFESKILL))->IsChecked(); if (m_GameAI.sPage6.bHelpSkill10 != dataCheck)m_GameAI.sPage6.bHelpSkill10 = dataCheck; else dataCheck = 9999;
	dataCheck = atoi(((cSpin*)pDlg->GetWindowForID(OTI_SP_AUTOHUNT_TIMER_TAB_01))->GetEditText()); if (m_GameAI.sPage6.bSkill1page != dataCheck)m_GameAI.sPage6.bSkill1page = dataCheck; else dataCheck = 9999;
	dataCheck = atoi(((cSpin*)pDlg->GetWindowForID(OTI_SP_AUTOHUNT_TIMER_TAB_02))->GetEditText()); if (m_GameAI.sPage6.bSkill2page != dataCheck)m_GameAI.sPage6.bSkill2page = dataCheck; else dataCheck = 9999;
	dataCheck = atoi(((cSpin*)pDlg->GetWindowForID(OTI_SP_AUTOHUNT_TIMER_TAB_03))->GetEditText()); if (m_GameAI.sPage6.bSkill3page != dataCheck)m_GameAI.sPage6.bSkill3page = dataCheck; else dataCheck = 9999;
	dataCheck = atoi(((cSpin*)pDlg->GetWindowForID(OTI_SP_AUTOHUNT_TIMER_TAB_04))->GetEditText()); if (m_GameAI.sPage6.bSkill4page != dataCheck)m_GameAI.sPage6.bSkill4page = dataCheck; else dataCheck = 9999;
	dataCheck = atoi(((cSpin*)pDlg->GetWindowForID(OTI_SP_AUTOHUNT_TIMER_TAB_05))->GetEditText()); if (m_GameAI.sPage6.bSkill5page != dataCheck)m_GameAI.sPage6.bSkill5page = dataCheck; else dataCheck = 9999;
	dataCheck = atoi(((cSpin*)pDlg->GetWindowForID(OTI_SP_AUTOHUNT_TIMER_SLOT_01))->GetEditText()); if (m_GameAI.sPage6.bSkill1key != dataCheck)m_GameAI.sPage6.bSkill1key = dataCheck; else dataCheck = 9999;
	dataCheck = atoi(((cSpin*)pDlg->GetWindowForID(OTI_SP_AUTOHUNT_TIMER_SLOT_02))->GetEditText()); if (m_GameAI.sPage6.bSkill2key != dataCheck)m_GameAI.sPage6.bSkill2key = dataCheck; else dataCheck = 9999;
	dataCheck = atoi(((cSpin*)pDlg->GetWindowForID(OTI_SP_AUTOHUNT_TIMER_SLOT_03))->GetEditText()); if (m_GameAI.sPage6.bSkill3key != dataCheck)m_GameAI.sPage6.bSkill3key = dataCheck; else dataCheck = 9999;
	dataCheck = atoi(((cSpin*)pDlg->GetWindowForID(OTI_SP_AUTOHUNT_TIMER_SLOT_04))->GetEditText()); if (m_GameAI.sPage6.bSkill4key != dataCheck)m_GameAI.sPage6.bSkill4key = dataCheck; else dataCheck = 9999;
	dataCheck = atoi(((cSpin*)pDlg->GetWindowForID(OTI_SP_AUTOHUNT_TIMER_SLOT_05))->GetEditText()); if (m_GameAI.sPage6.bSkill5key != dataCheck)m_GameAI.sPage6.bSkill5key = dataCheck; else dataCheck = 9999;
	dataCheck = atoi(((cSpin*)pDlg->GetWindowForID(OTI_SP_AUTOHUNT_TIMER_TIME_01))->GetEditText()); if (m_GameAI.sPage6.bSkill1time != dataCheck)m_GameAI.sPage6.bSkill1time = dataCheck; else dataCheck = 9999;
	dataCheck = atoi(((cSpin*)pDlg->GetWindowForID(OTI_SP_AUTOHUNT_TIMER_TIME_02))->GetEditText()); if (m_GameAI.sPage6.bSkill2time != dataCheck)m_GameAI.sPage6.bSkill2time = dataCheck; else dataCheck = 9999;
	dataCheck = atoi(((cSpin*)pDlg->GetWindowForID(OTI_SP_AUTOHUNT_TIMER_TIME_03))->GetEditText()); if (m_GameAI.sPage6.bSkill3time != dataCheck)m_GameAI.sPage6.bSkill3time = dataCheck; else dataCheck = 9999;
	dataCheck = atoi(((cSpin*)pDlg->GetWindowForID(OTI_SP_AUTOHUNT_TIMER_TIME_04))->GetEditText()); if (m_GameAI.sPage6.bSkill4time != dataCheck)m_GameAI.sPage6.bSkill4time = dataCheck; else dataCheck = 9999;
	dataCheck = atoi(((cSpin*)pDlg->GetWindowForID(OTI_SP_AUTOHUNT_TIMER_TIME_05))->GetEditText()); if (m_GameAI.sPage6.bSkill5time != dataCheck)m_GameAI.sPage6.bSkill5time = dataCheck; else dataCheck = 9999;
	dataCheck = atoi(((cSpin*)pDlg->GetWindowForID(OTI_CB_TNLIFE))->GetEditText()); if (m_GameAI.sPage6.bTunaSpin != dataCheck)m_GameAI.sPage6.bTunaSpin = dataCheck; else dataCheck = 9999;




	/*else
	{
	CHATMGR->AddMsg(CTC_SYSMSG, "NOTHING changed  %d", dataCheck);
	}*/

	/*m_GameAI.sPage6.bSkill1 = ((cCheckBox*)pDlg->GetWindowForID(OTI_CB_AUTOHUNT_TIMER_01))->IsChecked();
	m_GameAI.sPage6.bSkill2 = ((cCheckBox*)pDlg->GetWindowForID(OTI_CB_AUTOHUNT_TIMER_02))->IsChecked();
	m_GameAI.sPage6.bSkill3 = ((cCheckBox*)pDlg->GetWindowForID(OTI_CB_AUTOHUNT_TIMER_03))->IsChecked();
	m_GameAI.sPage6.bSkill4 = ((cCheckBox*)pDlg->GetWindowForID(OTI_CB_AUTOHUNT_TIMER_04))->IsChecked();
	m_GameAI.sPage6.bSkill5 = ((cCheckBox*)pDlg->GetWindowForID(OTI_CB_AUTOHUNT_TIMER_05))->IsChecked();

	m_GameAI.sPage6.bHelpSkill1 = ((cCheckBox*)pDlg->GetWindowForID(OTI_CB_AUTODOWNDEFENCE))->IsChecked();
	m_GameAI.sPage6.bHelpSkill2 = ((cCheckBox*)pDlg->GetWindowForID(OTI_CB_AUTODOWNATTACK))->IsChecked();
	m_GameAI.sPage6.bHelpSkill3 = ((cCheckBox*)pDlg->GetWindowForID(OTI_CB_AUTOFIRE))->IsChecked();
	m_GameAI.sPage6.bHelpSkill4 = ((cCheckBox*)pDlg->GetWindowForID(OTI_CB_AUTOWATER))->IsChecked();
	m_GameAI.sPage6.bHelpSkill5 = ((cCheckBox*)pDlg->GetWindowForID(OTI_CB_AUTOTREE))->IsChecked();
	m_GameAI.sPage6.bHelpSkill6 = ((cCheckBox*)pDlg->GetWindowForID(OTI_CB_AUTOIRON))->IsChecked();
	m_GameAI.sPage6.bHelpSkill7 = ((cCheckBox*)pDlg->GetWindowForID(OTI_CB_AUTOEARTH))->IsChecked();
	m_GameAI.sPage6.bHelpSkill8 = ((cCheckBox*)pDlg->GetWindowForID(OTI_CB_AUTOEARTHGUARD))->IsChecked();
	m_GameAI.sPage6.bHelpSkill9 = ((cCheckBox*)pDlg->GetWindowForID(OTI_CB_AUTOSKIPEFFECT))->IsChecked();
	m_GameAI.sPage6.bHelpSkill10 = ((cCheckBox*)pDlg->GetWindowForID(OTI_CB_AUTOADDLIFESKILL))->IsChecked();

	m_GameAI.sPage6.bSkill1page = atoi(((cSpin*)pDlg->GetWindowForID(OTI_SP_AUTOHUNT_TIMER_TAB_01))->GetEditText());
	m_GameAI.sPage6.bSkill2page = atoi(((cSpin*)pDlg->GetWindowForID(OTI_SP_AUTOHUNT_TIMER_TAB_02))->GetEditText());
	m_GameAI.sPage6.bSkill3page = atoi(((cSpin*)pDlg->GetWindowForID(OTI_SP_AUTOHUNT_TIMER_TAB_03))->GetEditText());
	m_GameAI.sPage6.bSkill4page = atoi(((cSpin*)pDlg->GetWindowForID(OTI_SP_AUTOHUNT_TIMER_TAB_04))->GetEditText());
	m_GameAI.sPage6.bSkill5page = atoi(((cSpin*)pDlg->GetWindowForID(OTI_SP_AUTOHUNT_TIMER_TAB_05))->GetEditText());
	m_GameAI.sPage6.bSkill1key = atoi(((cSpin*)pDlg->GetWindowForID(OTI_SP_AUTOHUNT_TIMER_SLOT_01))->GetEditText());
	m_GameAI.sPage6.bSkill2key = atoi(((cSpin*)pDlg->GetWindowForID(OTI_SP_AUTOHUNT_TIMER_SLOT_02))->GetEditText());
	m_GameAI.sPage6.bSkill3key = atoi(((cSpin*)pDlg->GetWindowForID(OTI_SP_AUTOHUNT_TIMER_SLOT_03))->GetEditText());
	m_GameAI.sPage6.bSkill4key = atoi(((cSpin*)pDlg->GetWindowForID(OTI_SP_AUTOHUNT_TIMER_SLOT_04))->GetEditText());
	m_GameAI.sPage6.bSkill5key = atoi(((cSpin*)pDlg->GetWindowForID(OTI_SP_AUTOHUNT_TIMER_SLOT_05))->GetEditText());
	m_GameAI.sPage6.bSkill1time = atoi(((cSpin*)pDlg->GetWindowForID(OTI_SP_AUTOHUNT_TIMER_TIME_01))->GetEditText());
	m_GameAI.sPage6.bSkill2time = atoi(((cSpin*)pDlg->GetWindowForID(OTI_SP_AUTOHUNT_TIMER_TIME_02))->GetEditText());
	m_GameAI.sPage6.bSkill3time = atoi(((cSpin*)pDlg->GetWindowForID(OTI_SP_AUTOHUNT_TIMER_TIME_03))->GetEditText());
	m_GameAI.sPage6.bSkill4time = atoi(((cSpin*)pDlg->GetWindowForID(OTI_SP_AUTOHUNT_TIMER_TIME_04))->GetEditText());
	m_GameAI.sPage6.bSkill5time = atoi(((cSpin*)pDlg->GetWindowForID(OTI_SP_AUTOHUNT_TIMER_TIME_05))->GetEditText());
	m_GameAI.sPage6.bTunaSpin = atoi(((cSpin*)pDlg->GetWindowForID(OTI_CB_TNLIFE))->GetEditText());*/



	pDlg = (cDialog*)m_Dlg->GetTabSheet(6);
	{
		m_GameAI.sPage7.ProtectLifeOpen1 = ((cCheckBox*)pDlg->GetWindowForID(OTI_CHECK_AUTO_LIFE1_CHECK))->IsChecked();
		m_GameAI.sPage7.ProtectLifePecent1 = atoi(((cSpin*)pDlg->GetWindowForID(OTI_CHECK_AUTO_LIFE1_VALUE))->GetEditText());
		m_GameAI.sPage7.ProtectLifeTime1 = atoi(((cSpin*)pDlg->GetWindowForID(OTI_CHECK_AUTO_LIFE1_TIME))->GetEditText());
		m_GameAI.sPage7.ProtectLifePage1 = atoi(((cSpin*)pDlg->GetWindowForID(OTI_CHECK_AUTO_LIFE1_PAGE))->GetEditText());
		m_GameAI.sPage7.ProtectLifeKey1 = atoi(((cSpin*)pDlg->GetWindowForID(OTI_CHECK_AUTO_LIFE1_KEY))->GetEditText());
		m_GameAI.sPage7.ProtectLifeOpen2 = ((cCheckBox*)pDlg->GetWindowForID(OTI_CHECK_AUTO_LIFE2_CHECK))->IsChecked();
		m_GameAI.sPage7.ProtectLifePecent2 = atoi(((cSpin*)pDlg->GetWindowForID(OTI_CHECK_AUTO_LIFE2_VALUE))->GetEditText());
		m_GameAI.sPage7.ProtectLifeTime2 = atoi(((cSpin*)pDlg->GetWindowForID(OTI_CHECK_AUTO_LIFE2_TIME))->GetEditText());
		m_GameAI.sPage7.ProtectLifePage2 = atoi(((cSpin*)pDlg->GetWindowForID(OTI_CHECK_AUTO_LIFE2_PAGE))->GetEditText());
		m_GameAI.sPage7.ProtectLifeKey2 = atoi(((cSpin*)pDlg->GetWindowForID(OTI_CHECK_AUTO_LIFE2_KEY))->GetEditText());
		m_GameAI.sPage7.ProtectLifeOpen3 = ((cCheckBox*)pDlg->GetWindowForID(OTI_CHECK_AUTO_LIFE3_CHECK))->IsChecked();
		m_GameAI.sPage7.ProtectLifePecent3 = atoi(((cSpin*)pDlg->GetWindowForID(OTI_CHECK_AUTO_LIFE3_VALUE))->GetEditText());
		m_GameAI.sPage7.ProtectLifeTime3 = atoi(((cSpin*)pDlg->GetWindowForID(OTI_CHECK_AUTO_LIFE3_TIME))->GetEditText());
		m_GameAI.sPage7.ProtectLifePage3 = atoi(((cSpin*)pDlg->GetWindowForID(OTI_CHECK_AUTO_LIFE3_PAGE))->GetEditText());
		m_GameAI.sPage7.ProtectLifeKey3 = atoi(((cSpin*)pDlg->GetWindowForID(OTI_CHECK_AUTO_LIFE3_KEY))->GetEditText());
		m_GameAI.sPage7.ProtectMagicOpen1 = ((cCheckBox*)pDlg->GetWindowForID(OTI_CHECK_AUTO_MAGIC1_CHECK))->IsChecked();
		m_GameAI.sPage7.ProtectMagicPecent1 = atoi(((cSpin*)pDlg->GetWindowForID(OTI_CHECK_AUTO_MAGIC1_VALUE))->GetEditText());
		m_GameAI.sPage7.ProtectMagicTime1 = atoi(((cSpin*)pDlg->GetWindowForID(OTI_CHECK_AUTO_MAGIC1_TIME))->GetEditText());
		m_GameAI.sPage7.ProtectMagicPage1 = atoi(((cSpin*)pDlg->GetWindowForID(OTI_CHECK_AUTO_MAGIC1_PAGE))->GetEditText());
		m_GameAI.sPage7.ProtectMagicKey1 = atoi(((cSpin*)pDlg->GetWindowForID(OTI_CHECK_AUTO_MAGIC1_KEY))->GetEditText());
		m_GameAI.sPage7.ProtectMagicOpen2 = ((cCheckBox*)pDlg->GetWindowForID(OTI_CHECK_AUTO_MAGIC2_CHECK))->IsChecked();
		m_GameAI.sPage7.ProtectMagicPecent2 = atoi(((cSpin*)pDlg->GetWindowForID(OTI_CHECK_AUTO_MAGIC2_VALUE))->GetEditText());
		m_GameAI.sPage7.ProtectMagicTime2 = atoi(((cSpin*)pDlg->GetWindowForID(OTI_CHECK_AUTO_MAGIC2_TIME))->GetEditText());
		m_GameAI.sPage7.ProtectMagicPage2 = atoi(((cSpin*)pDlg->GetWindowForID(OTI_CHECK_AUTO_MAGIC2_PAGE))->GetEditText());
		m_GameAI.sPage7.ProtectMagicKey2 = atoi(((cSpin*)pDlg->GetWindowForID(OTI_CHECK_AUTO_MAGIC2_KEY))->GetEditText());
		m_GameAI.sPage7.ProtectShieldOpen1 = ((cCheckBox*)pDlg->GetWindowForID(OTI_CHECK_AUTO_SHIELD1_CHECK))->IsChecked();
		m_GameAI.sPage7.ProtectShieldPecent1 = atoi(((cSpin*)pDlg->GetWindowForID(OTI_CHECK_AUTO_SHIELD1_VALUE))->GetEditText());
		m_GameAI.sPage7.ProtectShieldTime1 = atoi(((cSpin*)pDlg->GetWindowForID(OTI_CHECK_AUTO_SHIELD1_TIME))->GetEditText());
		m_GameAI.sPage7.ProtectShieldPage1 = atoi(((cSpin*)pDlg->GetWindowForID(OTI_CHECK_AUTO_SHIELD1_PAGE))->GetEditText());
		m_GameAI.sPage7.ProtectShieldKey1 = atoi(((cSpin*)pDlg->GetWindowForID(OTI_CHECK_AUTO_SHIELD1_KEY))->GetEditText());
		m_GameAI.sPage7.ProtectShieldOpen2 = ((cCheckBox*)pDlg->GetWindowForID(OTI_CHECK_AUTO_SHIELD2_CHECK))->IsChecked();
		m_GameAI.sPage7.ProtectShieldPecent2 = atoi(((cSpin*)pDlg->GetWindowForID(OTI_CHECK_AUTO_SHIELD2_VALUE))->GetEditText());
		m_GameAI.sPage7.ProtectShieldTime2 = atoi(((cSpin*)pDlg->GetWindowForID(OTI_CHECK_AUTO_SHIELD2_TIME))->GetEditText());
		m_GameAI.sPage7.ProtectShieldPage2 = atoi(((cSpin*)pDlg->GetWindowForID(OTI_CHECK_AUTO_SHIELD2_PAGE))->GetEditText());
		m_GameAI.sPage7.ProtectShieldKey2 = atoi(((cSpin*)pDlg->GetWindowForID(OTI_CHECK_AUTO_SHIELD2_KEY))->GetEditText());
	}
	pDlg = (cDialog*)m_Dlg->GetTabSheet(7);
	{
		m_GameAI.sPage8.isAutoAttrPlay = ((cCheckBox*)pDlg->GetWindowForID(OTI_CB_AUTOHUNT_PK_COUNTER))->IsChecked();
		m_GameAI.sPage8.isAutoCloseGetItemWin = ((cCheckBox*)pDlg->GetWindowForID(OTI_CB_AUTOHUNT_PK_LOOTING))->IsChecked();
		m_GameAI.sPage8.isAutoJoinGroup = ((cCheckBox*)pDlg->GetWindowForID(OTI_CB_AUTOHUNT_PARTY))->IsChecked();
		m_GameAI.sPage8.isAutoUseBuffItem = ((cCheckBox*)pDlg->GetWindowForID(OTI_CB_AUTOHUNT_BUFF_ITEM))->IsChecked();
		m_GameAI.sPage8.isReConnServer = ((cCheckBox*)pDlg->GetWindowForID(OTI_PET_EAT_CHK))->IsChecked();
		m_GameAI.sPage8.isAutoCallPet = ((cCheckBox*)pDlg->GetWindowForID(OTI_PET_RECALL_CHK))->IsChecked();
		m_GameAI.sPage8.isAutoWant = ((cCheckBox*)pDlg->GetWindowForID(OTI_CB_AUTOHUNT_BOUNTY))->IsChecked();
		m_GameAI.sPage8.WantMoney = atoi(((cEditBox*)pDlg->GetWindowForID(OTI_EDIT_BOUNTY))->GetEditText());
		m_GameAI.sPage8.cbReconnectMap = ((cCheckBox*)pDlg->GetWindowForID(OTI_CB_RECONNECT_MAP))->IsChecked();
		m_GameAI.sPage8.spReconnectMap = atoi(((cSpin*)pDlg->GetWindowForID(OTI_SP_RECONNECT_MAP))->GetEditText());
		m_GameAI.sPage8.spPetEatPercent = atoi(((cSpin*)pDlg->GetWindowForID(OTI_SP_PET_EAT))->GetEditText());
	}
	pDlg = (cDialog*)m_Dlg->GetTabSheet(9);
	{
		m_GameAI.sPage8.m_AutoSkill1 = ((cCheckBox*)pDlg->GetWindowForID(OTI_CB_M_AUTOSKILL_BH))->IsChecked();
		m_GameAI.sPage8.m_AutoSkill2 = ((cCheckBox*)pDlg->GetWindowForID(OTI_CB_M_AUTOSKILL_LX))->IsChecked();
		m_GameAI.sPage8.m_AutoSkill3 = ((cCheckBox*)pDlg->GetWindowForID(OTI_CB_M_AUTOSKILL_JYING))->IsChecked();
		m_GameAI.sPage8.m_AutoSkill4 = ((cCheckBox*)pDlg->GetWindowForID(OTI_CB_M_AUTOSKILL_HM))->IsChecked();
		m_GameAI.sPage8.m_AutoSkill5 = ((cCheckBox*)pDlg->GetWindowForID(OTI_CB_M_AUTOSKILL_TM))->IsChecked();
		m_GameAI.sPage8.m_AutoSkill6 = ((cCheckBox*)pDlg->GetWindowForID(OTI_CB_M_AUTOSKILL_XX))->IsChecked();
		m_GameAI.sPage8.h_AutoSkill1 = ((cCheckBox*)pDlg->GetWindowForID(OTI_CB_H_AUTOSKILL_JG))->IsChecked();
		m_GameAI.sPage8.h_AutoSkill2 = ((cCheckBox*)pDlg->GetWindowForID(OTI_CB_H_AUTOSKILL_SZ))->IsChecked();
		m_GameAI.sPage8.h_AutoSkill3 = ((cCheckBox*)pDlg->GetWindowForID(OTI_CB_H_AUTOSKILL_JYANG))->IsChecked();
		m_GameAI.sPage8.h_AutoSkill4 = ((cCheckBox*)pDlg->GetWindowForID(OTI_CB_H_AUTOSKILL_TX))->IsChecked();
		m_GameAI.sPage8.h_AutoSkill5 = ((cCheckBox*)pDlg->GetWindowForID(OTI_CB_H_AUTOSKILL_BM))->IsChecked();
		m_GameAI.sPage8.h_AutoSkill6 = ((cCheckBox*)pDlg->GetWindowForID(OTI_CB_H_AUTOSKILL_FM))->IsChecked();
		m_GameAI.sPage8.FuoMoo = ((cCheckBox*)pDlg->GetWindowForID(OTI_CB_H_AUTOSKILL_FUMO))->IsChecked();
	}



	SetGameAI(&m_GameAI);
}
void CAiSetManager::ReadData()
{
	COptionDialog * m_Dlg = GAMEIN->GetOptionDialog();
	cDialog* pDlg;
	pDlg = (cDialog*)m_Dlg->GetTabSheet(4);
	char tmp[10];
	((cCheckBox*)pDlg->GetWindowForID(OTI_CB_AUTOHUNT_LOCA))->SetChecked(m_GameAI.sPage5.cbAreaXY);
	((cCheckBox*)pDlg->GetWindowForID(OTI_CB_AUTOUNGI))->SetChecked(m_GameAI.sPage5.cbAutoSitHP);
	((cCheckBox*)pDlg->GetWindowForID(OTI_CB_AUTOHUNT_UNGI_MP))->SetChecked(m_GameAI.sPage5.cbAutoSitMP);
	((cCheckBox*)pDlg->GetWindowForID(OTI_CB_AUTOHUNT_UNGI_SP))->SetChecked(m_GameAI.sPage5.cbAutoSitSP);
	((cCheckBox*)pDlg->GetWindowForID(OTI_CB_AUTOHUNT_UNGI_ATTACKED))->SetChecked(m_GameAI.sPage5.cbSitAttacked);
	((cCheckBox*)pDlg->GetWindowForID(OTI_CB_AUTOHUNT_MONSTERWALL))->SetChecked(m_GameAI.sPage5.cbNoMonsterWall);
	((cCheckBox*)pDlg->GetWindowForID(OTI_CB_AUTOHUNT_REVIVE))->SetChecked(m_GameAI.sPage5.cbAutoRespawn);
	((cCheckBox*)pDlg->GetWindowForID(OTI_CB_AUTOHUNT_REVIVE_LIMITCOUNT))->SetChecked(m_GameAI.sPage5.cbAutoRespawnTimeLimit);
	((cCheckBox*)pDlg->GetWindowForID(OTI_CB_AUTOHUNT_UNGI_AT_NOTARGET))->SetChecked(m_GameAI.sPage5.cbSitNoTarget);
	((cCheckBox*)pDlg->GetWindowForID(OTI_CB_IGNORE_BOSSES))->SetChecked(m_GameAI.sPage5.cbIgnoreBoss);
	((cPushupButton*)pDlg->GetWindowForID(OTI_PB_AUTOHUNT_ON))->SetPush(m_GameAI.sPage5.pbAutoLeveling);
	((cPushupButton*)pDlg->GetWindowForID(OTI_PB_AUTOHUNT_REVIVE_LOGIN))->SetPush(!m_GameAI.sPage5.pbSpawnHere);
	((cPushupButton*)pDlg->GetWindowForID(OTI_PB_AUTOHUNT_REVIVE_PRESENT))->SetPush(m_GameAI.sPage5.pbSpawnHere);
	memset(tmp, 0, 10); itoa(m_GameAI.sPage5.spPosX, tmp, 10);
	((cSpin*)pDlg->GetWindowForID(OTI_SP_AUTOHUNT_LOCA_X))->SetEditText(tmp);
	((cSpin*)pDlg->GetWindowForID(OTI_SP_AUTOHUNT_LOCA_X))->SetReadOnly(FALSE);
	((cSpin*)pDlg->GetWindowForID(OTI_SP_AUTOHUNT_LOCA_X))->SetMinMax(10, 500);
	memset(tmp, 0, 10); itoa(m_GameAI.sPage5.spPosY, tmp, 10);
	((cSpin*)pDlg->GetWindowForID(OTI_SP_AUTOHUNT_LOCA_Y))->SetEditText(tmp);
	((cSpin*)pDlg->GetWindowForID(OTI_SP_AUTOHUNT_LOCA_Y))->SetReadOnly(FALSE);
	((cSpin*)pDlg->GetWindowForID(OTI_SP_AUTOHUNT_LOCA_Y))->SetMinMax(10, 500);
	memset(tmp, 0, 10); itoa(m_GameAI.sPage5.spRange, tmp, 10);
	((cSpin*)pDlg->GetWindowForID(OTI_SP_AUTOHUNT_RANGE))->SetEditText(tmp);
	((cSpin*)pDlg->GetWindowForID(OTI_SP_AUTOHUNT_RANGE))->SetReadOnly(TRUE);
	((cSpin*)pDlg->GetWindowForID(OTI_SP_AUTOHUNT_RANGE))->SetMinMax(5, 100);
	memset(tmp, 1, 10); itoa(m_GameAI.sPage5.spSitHP, tmp, 10);
	((cSpin*)pDlg->GetWindowForID(OTI_SP_AUTOHUNT_UNGI_HP))->SetEditText(tmp);
	((cSpin*)pDlg->GetWindowForID(OTI_SP_AUTOHUNT_UNGI_HP))->SetMinMax(5, 90);
	((cSpin*)pDlg->GetWindowForID(OTI_SP_AUTOHUNT_UNGI_HP))->SetReadOnly(TRUE);
	memset(tmp, 1, 10); itoa(m_GameAI.sPage5.spSitMP, tmp, 10);
	((cSpin*)pDlg->GetWindowForID(OTI_SP_AUTOHUNT_UNGI_MP))->SetEditText(tmp);
	((cSpin*)pDlg->GetWindowForID(OTI_SP_AUTOHUNT_UNGI_MP))->SetMinMax(5, 90);
	((cSpin*)pDlg->GetWindowForID(OTI_SP_AUTOHUNT_UNGI_MP))->SetReadOnly(TRUE);
	memset(tmp, 1, 10); itoa(m_GameAI.sPage5.spSitSP, tmp, 10);
	((cSpin*)pDlg->GetWindowForID(OTI_SP_AUTOHUNT_UNGI_SP))->SetEditText(tmp);
	((cSpin*)pDlg->GetWindowForID(OTI_SP_AUTOHUNT_UNGI_SP))->SetMinMax(5, 90);
	((cSpin*)pDlg->GetWindowForID(OTI_SP_AUTOHUNT_UNGI_SP))->SetReadOnly(TRUE);
	memset(tmp, 0, 10); itoa(m_GameAI.sPage5.spReviveTime, tmp, 10);
	((cSpin*)pDlg->GetWindowForID(OTI_SP_AUTOHUNT_REVIVE_TIME))->SetEditText(tmp);
	((cSpin*)pDlg->GetWindowForID(OTI_SP_AUTOHUNT_REVIVE_TIME))->SetReadOnly(TRUE);
	((cSpin*)pDlg->GetWindowForID(OTI_SP_AUTOHUNT_REVIVE_TIME))->SetMinMax(0, 99);
	memset(tmp, 0, 10); itoa(m_GameAI.sPage5.spReviveTimeLimit, tmp, 10);
	((cSpin*)pDlg->GetWindowForID(OTI_SP_AUTOHUNT_REVIVE_LIMITCOUNT))->SetEditText(tmp);
	((cSpin*)pDlg->GetWindowForID(OTI_SP_AUTOHUNT_REVIVE_LIMITCOUNT))->SetReadOnly(TRUE);
	((cSpin*)pDlg->GetWindowForID(OTI_SP_AUTOHUNT_REVIVE_LIMITCOUNT))->SetMinMax(0, 99);
	memset(tmp, 0, 10); itoa(m_GameAI.sPage5.spSitAttacked, tmp, 10);
	((cSpin*)pDlg->GetWindowForID(OTI_SP_AUTOHUNT_UNGI_ATTACKED))->SetEditText(tmp);
	((cSpin*)pDlg->GetWindowForID(OTI_SP_AUTOHUNT_UNGI_ATTACKED))->SetReadOnly(TRUE);
	((cSpin*)pDlg->GetWindowForID(OTI_SP_AUTOHUNT_UNGI_ATTACKED))->SetMinMax(0, 99);
	pDlg = (cDialog*)m_Dlg->GetTabSheet(5);
	((cCheckBox*)pDlg->GetWindowForID(OTI_CB_AUTOHUNT_TIMER_01))->SetChecked(m_GameAI.sPage6.bSkill1);
	((cCheckBox*)pDlg->GetWindowForID(OTI_CB_AUTOHUNT_TIMER_02))->SetChecked(m_GameAI.sPage6.bSkill2);
	((cCheckBox*)pDlg->GetWindowForID(OTI_CB_AUTOHUNT_TIMER_03))->SetChecked(m_GameAI.sPage6.bSkill3);
	((cCheckBox*)pDlg->GetWindowForID(OTI_CB_AUTOHUNT_TIMER_04))->SetChecked(m_GameAI.sPage6.bSkill4);
	((cCheckBox*)pDlg->GetWindowForID(OTI_CB_AUTOHUNT_TIMER_05))->SetChecked(m_GameAI.sPage6.bSkill5);
	memset(tmp, 1, 10);	itoa(m_GameAI.sPage6.bSkill1key, tmp, 10);
	((cSpin*)pDlg->GetWindowForID(OTI_SP_AUTOHUNT_TIMER_SLOT_01))->SetEditText(tmp);
	((cSpin*)pDlg->GetWindowForID(OTI_SP_AUTOHUNT_TIMER_SLOT_01))->SetMinMax(0, 9);
	((cSpin*)pDlg->GetWindowForID(OTI_SP_AUTOHUNT_TIMER_SLOT_01))->SetReadOnly(TRUE);
	memset(tmp, 1, 10);	itoa(m_GameAI.sPage6.bSkill2key, tmp, 10);
	((cSpin*)pDlg->GetWindowForID(OTI_SP_AUTOHUNT_TIMER_SLOT_02))->SetEditText(tmp);
	((cSpin*)pDlg->GetWindowForID(OTI_SP_AUTOHUNT_TIMER_SLOT_02))->SetMinMax(0, 9);
	((cSpin*)pDlg->GetWindowForID(OTI_SP_AUTOHUNT_TIMER_SLOT_02))->SetReadOnly(TRUE);
	memset(tmp, 1, 10);	itoa(m_GameAI.sPage6.bSkill3key, tmp, 10);
	((cSpin*)pDlg->GetWindowForID(OTI_SP_AUTOHUNT_TIMER_SLOT_03))->SetEditText(tmp);
	((cSpin*)pDlg->GetWindowForID(OTI_SP_AUTOHUNT_TIMER_SLOT_03))->SetMinMax(0, 9);
	((cSpin*)pDlg->GetWindowForID(OTI_SP_AUTOHUNT_TIMER_SLOT_03))->SetReadOnly(TRUE);
	memset(tmp, 1, 10);	itoa(m_GameAI.sPage6.bSkill4key, tmp, 10);
	((cSpin*)pDlg->GetWindowForID(OTI_SP_AUTOHUNT_TIMER_SLOT_04))->SetEditText(tmp);
	((cSpin*)pDlg->GetWindowForID(OTI_SP_AUTOHUNT_TIMER_SLOT_04))->SetMinMax(0, 9);
	((cSpin*)pDlg->GetWindowForID(OTI_SP_AUTOHUNT_TIMER_SLOT_04))->SetReadOnly(TRUE);
	memset(tmp, 1, 10);	itoa(m_GameAI.sPage6.bSkill5key, tmp, 10);
	((cSpin*)pDlg->GetWindowForID(OTI_SP_AUTOHUNT_TIMER_SLOT_05))->SetEditText(tmp);
	((cSpin*)pDlg->GetWindowForID(OTI_SP_AUTOHUNT_TIMER_SLOT_05))->SetMinMax(0, 9);
	((cSpin*)pDlg->GetWindowForID(OTI_SP_AUTOHUNT_TIMER_SLOT_05))->SetReadOnly(TRUE);
	memset(tmp, 1, 10);	itoa(m_GameAI.sPage6.bSkill1page, tmp, 10);
	((cSpin*)pDlg->GetWindowForID(OTI_SP_AUTOHUNT_TIMER_TAB_01))->SetEditText(tmp);
	((cSpin*)pDlg->GetWindowForID(OTI_SP_AUTOHUNT_TIMER_TAB_01))->SetMinMax(1, 4);
	((cSpin*)pDlg->GetWindowForID(OTI_SP_AUTOHUNT_TIMER_TAB_01))->SetReadOnly(TRUE);
	memset(tmp, 1, 10);	itoa(m_GameAI.sPage6.bSkill2page, tmp, 10);
	((cSpin*)pDlg->GetWindowForID(OTI_SP_AUTOHUNT_TIMER_TAB_02))->SetEditText(tmp);
	((cSpin*)pDlg->GetWindowForID(OTI_SP_AUTOHUNT_TIMER_TAB_02))->SetMinMax(1, 4);
	((cSpin*)pDlg->GetWindowForID(OTI_SP_AUTOHUNT_TIMER_TAB_02))->SetReadOnly(TRUE);
	memset(tmp, 1, 10);	itoa(m_GameAI.sPage6.bSkill3page, tmp, 10);
	((cSpin*)pDlg->GetWindowForID(OTI_SP_AUTOHUNT_TIMER_TAB_03))->SetEditText(tmp);
	((cSpin*)pDlg->GetWindowForID(OTI_SP_AUTOHUNT_TIMER_TAB_03))->SetMinMax(1, 4);
	((cSpin*)pDlg->GetWindowForID(OTI_SP_AUTOHUNT_TIMER_TAB_03))->SetReadOnly(TRUE);
	memset(tmp, 1, 10);	itoa(m_GameAI.sPage6.bSkill4page, tmp, 10);
	((cSpin*)pDlg->GetWindowForID(OTI_SP_AUTOHUNT_TIMER_TAB_04))->SetEditText(tmp);
	((cSpin*)pDlg->GetWindowForID(OTI_SP_AUTOHUNT_TIMER_TAB_04))->SetMinMax(1, 4);
	((cSpin*)pDlg->GetWindowForID(OTI_SP_AUTOHUNT_TIMER_TAB_04))->SetReadOnly(TRUE);
	memset(tmp, 1, 10);	itoa(m_GameAI.sPage6.bSkill5page, tmp, 10);
	((cSpin*)pDlg->GetWindowForID(OTI_SP_AUTOHUNT_TIMER_TAB_05))->SetEditText(tmp);
	((cSpin*)pDlg->GetWindowForID(OTI_SP_AUTOHUNT_TIMER_TAB_05))->SetMinMax(1, 4);
	((cSpin*)pDlg->GetWindowForID(OTI_SP_AUTOHUNT_TIMER_TAB_05))->SetReadOnly(TRUE);
	memset(tmp, 1, 10);	itoa(m_GameAI.sPage6.bSkill1time, tmp, 10);
	((cSpin*)pDlg->GetWindowForID(OTI_SP_AUTOHUNT_TIMER_TIME_01))->SetEditText(tmp);
	((cSpin*)pDlg->GetWindowForID(OTI_SP_AUTOHUNT_TIMER_TIME_01))->SetReadOnly(TRUE);
	memset(tmp, 1, 10);	itoa(m_GameAI.sPage6.bSkill2time, tmp, 10);
	((cSpin*)pDlg->GetWindowForID(OTI_SP_AUTOHUNT_TIMER_TIME_02))->SetEditText(tmp);
	((cSpin*)pDlg->GetWindowForID(OTI_SP_AUTOHUNT_TIMER_TIME_02))->SetReadOnly(TRUE);
	memset(tmp, 1, 10);	itoa(m_GameAI.sPage6.bSkill3time, tmp, 10);
	((cSpin*)pDlg->GetWindowForID(OTI_SP_AUTOHUNT_TIMER_TIME_03))->SetEditText(tmp);
	((cSpin*)pDlg->GetWindowForID(OTI_SP_AUTOHUNT_TIMER_TIME_03))->SetReadOnly(TRUE);
	memset(tmp, 1, 10);	itoa(m_GameAI.sPage6.bSkill4time, tmp, 10);
	((cSpin*)pDlg->GetWindowForID(OTI_SP_AUTOHUNT_TIMER_TIME_04))->SetEditText(tmp);
	((cSpin*)pDlg->GetWindowForID(OTI_SP_AUTOHUNT_TIMER_TIME_04))->SetReadOnly(TRUE);
	memset(tmp, 1, 10);	itoa(m_GameAI.sPage6.bSkill5time, tmp, 10);
	((cSpin*)pDlg->GetWindowForID(OTI_SP_AUTOHUNT_TIMER_TIME_05))->SetEditText(tmp);
	((cSpin*)pDlg->GetWindowForID(OTI_SP_AUTOHUNT_TIMER_TIME_05))->SetReadOnly(TRUE);
	((cCheckBox*)pDlg->GetWindowForID(OTI_CB_AUTODOWNDEFENCE))->SetChecked(m_GameAI.sPage6.bHelpSkill1);
	((cCheckBox*)pDlg->GetWindowForID(OTI_CB_AUTODOWNATTACK))->SetChecked(m_GameAI.sPage6.bHelpSkill2);
	((cCheckBox*)pDlg->GetWindowForID(OTI_CB_AUTOFIRE))->SetChecked(m_GameAI.sPage6.bHelpSkill3);
	((cCheckBox*)pDlg->GetWindowForID(OTI_CB_AUTOWATER))->SetChecked(m_GameAI.sPage6.bHelpSkill4);
	((cCheckBox*)pDlg->GetWindowForID(OTI_CB_AUTOTREE))->SetChecked(m_GameAI.sPage6.bHelpSkill5);
	((cCheckBox*)pDlg->GetWindowForID(OTI_CB_AUTOIRON))->SetChecked(m_GameAI.sPage6.bHelpSkill6);
	((cCheckBox*)pDlg->GetWindowForID(OTI_CB_AUTOEARTH))->SetChecked(m_GameAI.sPage6.bHelpSkill7);
	((cCheckBox*)pDlg->GetWindowForID(OTI_CB_AUTOEARTHGUARD))->SetChecked(m_GameAI.sPage6.bHelpSkill8);
	((cCheckBox*)pDlg->GetWindowForID(OTI_CB_AUTOSKIPEFFECT))->SetChecked(m_GameAI.sPage6.bHelpSkill9);
	((cCheckBox*)pDlg->GetWindowForID(OTI_CB_AUTOADDLIFESKILL))->SetChecked(m_GameAI.sPage6.bHelpSkill10);
	memset(tmp, 1, 10);	itoa(m_GameAI.sPage6.bTunaSpin, tmp, 10);
	((cSpin*)pDlg->GetWindowForID(OTI_CB_TNLIFE))->SetEditText(tmp);
	((cSpin*)pDlg->GetWindowForID(OTI_CB_TNLIFE))->SetMinMax(10, 80);
	((cSpin*)pDlg->GetWindowForID(OTI_CB_TNLIFE))->SetReadOnly(TRUE);
	pDlg = (cDialog*)m_Dlg->GetTabSheet(6);
	((cCheckBox*)pDlg->GetWindowForID(OTI_CHECK_AUTO_LIFE1_CHECK))->SetChecked(m_GameAI.sPage7.ProtectLifeOpen1);
	memset(tmp, 0, 10);	itoa(m_GameAI.sPage7.ProtectLifePecent1, tmp, 10);	((cSpin*)pDlg->GetWindowForID(OTI_CHECK_AUTO_LIFE1_VALUE))->SetEditText(tmp);
	memset(tmp, 0, 10);	itoa(m_GameAI.sPage7.ProtectLifeTime1, tmp, 10);		((cSpin*)pDlg->GetWindowForID(OTI_CHECK_AUTO_LIFE1_TIME))->SetEditText(tmp);
	memset(tmp, 0, 10);	itoa(m_GameAI.sPage7.ProtectLifePage1, tmp, 10);		((cSpin*)pDlg->GetWindowForID(OTI_CHECK_AUTO_LIFE1_PAGE))->SetEditText(tmp);
	memset(tmp, 0, 10);	itoa(m_GameAI.sPage7.ProtectLifeKey1, tmp, 10);		((cSpin*)pDlg->GetWindowForID(OTI_CHECK_AUTO_LIFE1_KEY))->SetEditText(tmp);
	((cCheckBox*)pDlg->GetWindowForID(OTI_CHECK_AUTO_LIFE2_CHECK))->SetChecked(m_GameAI.sPage7.ProtectLifeOpen2);
	memset(tmp, 0, 10);	itoa(m_GameAI.sPage7.ProtectLifePecent2, tmp, 10);	((cSpin*)pDlg->GetWindowForID(OTI_CHECK_AUTO_LIFE2_VALUE))->SetEditText(tmp);
	memset(tmp, 0, 10);	itoa(m_GameAI.sPage7.ProtectLifeTime2, tmp, 10);		((cSpin*)pDlg->GetWindowForID(OTI_CHECK_AUTO_LIFE2_TIME))->SetEditText(tmp);
	memset(tmp, 0, 10);	itoa(m_GameAI.sPage7.ProtectLifePage2, tmp, 10);		((cSpin*)pDlg->GetWindowForID(OTI_CHECK_AUTO_LIFE2_PAGE))->SetEditText(tmp);
	memset(tmp, 0, 10);	itoa(m_GameAI.sPage7.ProtectLifeKey2, tmp, 10);		((cSpin*)pDlg->GetWindowForID(OTI_CHECK_AUTO_LIFE2_KEY))->SetEditText(tmp);
	((cCheckBox*)pDlg->GetWindowForID(OTI_CHECK_AUTO_LIFE3_CHECK))->SetChecked(m_GameAI.sPage7.ProtectLifeOpen3);
	memset(tmp, 0, 10);	itoa(m_GameAI.sPage7.ProtectLifePecent3, tmp, 10);	((cSpin*)pDlg->GetWindowForID(OTI_CHECK_AUTO_LIFE3_VALUE))->SetEditText(tmp);
	memset(tmp, 0, 10);	itoa(m_GameAI.sPage7.ProtectLifeTime3, tmp, 10);		((cSpin*)pDlg->GetWindowForID(OTI_CHECK_AUTO_LIFE3_TIME))->SetEditText(tmp);
	memset(tmp, 0, 10);	itoa(m_GameAI.sPage7.ProtectLifePage3, tmp, 10);		((cSpin*)pDlg->GetWindowForID(OTI_CHECK_AUTO_LIFE3_PAGE))->SetEditText(tmp);
	memset(tmp, 0, 10);	itoa(m_GameAI.sPage7.ProtectLifeKey3, tmp, 10);		((cSpin*)pDlg->GetWindowForID(OTI_CHECK_AUTO_LIFE3_KEY))->SetEditText(tmp);
	((cCheckBox*)pDlg->GetWindowForID(OTI_CHECK_AUTO_MAGIC1_CHECK))->SetChecked(m_GameAI.sPage7.ProtectMagicOpen1);
	memset(tmp, 0, 10);	itoa(m_GameAI.sPage7.ProtectMagicPecent1, tmp, 10);	((cSpin*)pDlg->GetWindowForID(OTI_CHECK_AUTO_MAGIC1_VALUE))->SetEditText(tmp);
	memset(tmp, 0, 10);	itoa(m_GameAI.sPage7.ProtectMagicTime1, tmp, 10);		((cSpin*)pDlg->GetWindowForID(OTI_CHECK_AUTO_MAGIC1_TIME))->SetEditText(tmp);
	memset(tmp, 0, 10);	itoa(m_GameAI.sPage7.ProtectMagicPage1, tmp, 10);		((cSpin*)pDlg->GetWindowForID(OTI_CHECK_AUTO_MAGIC1_PAGE))->SetEditText(tmp);
	memset(tmp, 0, 10);	itoa(m_GameAI.sPage7.ProtectMagicKey1, tmp, 10);		((cSpin*)pDlg->GetWindowForID(OTI_CHECK_AUTO_MAGIC1_KEY))->SetEditText(tmp);
	((cCheckBox*)pDlg->GetWindowForID(OTI_CHECK_AUTO_MAGIC2_CHECK))->SetChecked(m_GameAI.sPage7.ProtectMagicOpen2);
	memset(tmp, 0, 10);	itoa(m_GameAI.sPage7.ProtectMagicPecent2, tmp, 10);	((cSpin*)pDlg->GetWindowForID(OTI_CHECK_AUTO_MAGIC2_VALUE))->SetEditText(tmp);
	memset(tmp, 0, 10);	itoa(m_GameAI.sPage7.ProtectMagicTime2, tmp, 10);		((cSpin*)pDlg->GetWindowForID(OTI_CHECK_AUTO_MAGIC2_TIME))->SetEditText(tmp);
	memset(tmp, 0, 10);	itoa(m_GameAI.sPage7.ProtectMagicPage2, tmp, 10);		((cSpin*)pDlg->GetWindowForID(OTI_CHECK_AUTO_MAGIC2_PAGE))->SetEditText(tmp);
	memset(tmp, 0, 10);	itoa(m_GameAI.sPage7.ProtectMagicKey2, tmp, 10);		((cSpin*)pDlg->GetWindowForID(OTI_CHECK_AUTO_MAGIC2_KEY))->SetEditText(tmp);
	((cCheckBox*)pDlg->GetWindowForID(OTI_CHECK_AUTO_SHIELD1_CHECK))->SetChecked(m_GameAI.sPage7.ProtectShieldOpen1);
	memset(tmp, 0, 10);	itoa(m_GameAI.sPage7.ProtectShieldPecent1, tmp, 10);	((cSpin*)pDlg->GetWindowForID(OTI_CHECK_AUTO_SHIELD1_VALUE))->SetEditText(tmp);
	memset(tmp, 0, 10);	itoa(m_GameAI.sPage7.ProtectShieldTime1, tmp, 10);	((cSpin*)pDlg->GetWindowForID(OTI_CHECK_AUTO_SHIELD1_TIME))->SetEditText(tmp);
	memset(tmp, 0, 10);	itoa(m_GameAI.sPage7.ProtectShieldPage1, tmp, 10);	((cSpin*)pDlg->GetWindowForID(OTI_CHECK_AUTO_SHIELD1_PAGE))->SetEditText(tmp);
	memset(tmp, 0, 10);	itoa(m_GameAI.sPage7.ProtectShieldKey1, tmp, 10);		((cSpin*)pDlg->GetWindowForID(OTI_CHECK_AUTO_SHIELD1_KEY))->SetEditText(tmp);
	((cCheckBox*)pDlg->GetWindowForID(OTI_CHECK_AUTO_SHIELD2_CHECK))->SetChecked(m_GameAI.sPage7.ProtectShieldOpen2);
	memset(tmp, 0, 10);	itoa(m_GameAI.sPage7.ProtectShieldPecent2, tmp, 10);	((cSpin*)pDlg->GetWindowForID(OTI_CHECK_AUTO_SHIELD2_VALUE))->SetEditText(tmp);
	memset(tmp, 0, 10);	itoa(m_GameAI.sPage7.ProtectShieldTime2, tmp, 10);	((cSpin*)pDlg->GetWindowForID(OTI_CHECK_AUTO_SHIELD2_TIME))->SetEditText(tmp);
	memset(tmp, 0, 10);	itoa(m_GameAI.sPage7.ProtectShieldPage2, tmp, 10);	((cSpin*)pDlg->GetWindowForID(OTI_CHECK_AUTO_SHIELD2_PAGE))->SetEditText(tmp);
	memset(tmp, 0, 10);	itoa(m_GameAI.sPage7.ProtectShieldKey2, tmp, 10);		((cSpin*)pDlg->GetWindowForID(OTI_CHECK_AUTO_SHIELD2_KEY))->SetEditText(tmp);
	pDlg = (cDialog*)m_Dlg->GetTabSheet(7);
	((cCheckBox*)pDlg->GetWindowForID(OTI_CB_AUTOHUNT_PK_COUNTER))->SetChecked(m_GameAI.sPage8.isAutoAttrPlay);
	((cCheckBox*)pDlg->GetWindowForID(OTI_CB_AUTOHUNT_PK_LOOTING))->SetChecked(m_GameAI.sPage8.isAutoCloseGetItemWin);
	((cCheckBox*)pDlg->GetWindowForID(OTI_CB_AUTOHUNT_PARTY))->SetChecked(m_GameAI.sPage8.isAutoJoinGroup);
	((cCheckBox*)pDlg->GetWindowForID(OTI_CB_AUTOHUNT_BUFF_ITEM))->SetChecked(m_GameAI.sPage8.isAutoUseBuffItem);
	((cCheckBox*)pDlg->GetWindowForID(OTI_PET_EAT_CHK))->SetChecked(m_GameAI.sPage8.isReConnServer);
	((cCheckBox*)pDlg->GetWindowForID(OTI_PET_RECALL_CHK))->SetChecked(m_GameAI.sPage8.isAutoCallPet);
	((cCheckBox*)pDlg->GetWindowForID(OTI_CB_AUTOHUNT_BOUNTY))->SetChecked(m_GameAI.sPage8.isAutoWant);
	memset(tmp, 0, 10);	itoa(m_GameAI.sPage8.WantMoney, tmp, 10);
	((cEditBox*)pDlg->GetWindowForID(OTI_EDIT_BOUNTY))->SetValidCheck(VCM_NUMBER);
	((cEditBox*)pDlg->GetWindowForID(OTI_EDIT_BOUNTY))->SetEditText(tmp);
	((cCheckBox*)pDlg->GetWindowForID(OTI_CB_RECONNECT_MAP))->SetChecked(m_GameAI.sPage8.cbReconnectMap);
	memset(tmp, 0, 10);	itoa(m_GameAI.sPage8.spReconnectMap, tmp, 10);
	((cSpin*)pDlg->GetWindowForID(OTI_SP_RECONNECT_MAP))->SetValidCheck(VCM_NUMBER);
	((cSpin*)pDlg->GetWindowForID(OTI_SP_RECONNECT_MAP))->SetEditText(tmp);
	memset(tmp, 0, 10);
	((cSpin*)pDlg->GetWindowForID(OTI_SP_PET_EAT))->SetMinMax(10, 90);
	((cSpin*)pDlg->GetWindowForID(OTI_SP_PET_EAT))->SetReadOnly(TRUE);
	if (m_GameAI.sPage8.spPetEatPercent < ((cSpin*)pDlg->GetWindowForID(OTI_SP_PET_EAT))->GetMin())
		m_GameAI.sPage8.spPetEatPercent = ((cSpin*)pDlg->GetWindowForID(OTI_SP_PET_EAT))->GetMin();
	if (m_GameAI.sPage8.spPetEatPercent >((cSpin*)pDlg->GetWindowForID(OTI_SP_PET_EAT))->GetMax())
		m_GameAI.sPage8.spPetEatPercent = ((cSpin*)pDlg->GetWindowForID(OTI_SP_PET_EAT))->GetMax();
	itoa(m_GameAI.sPage8.spPetEatPercent, tmp, 10);
	((cSpin*)pDlg->GetWindowForID(OTI_SP_PET_EAT))->SetEditText(tmp);
	pDlg = (cDialog*)m_Dlg->GetTabSheet(9);
	((cCheckBox*)pDlg->GetWindowForID(OTI_CB_M_AUTOSKILL_BH))->SetChecked(m_GameAI.sPage8.m_AutoSkill1);
	((cCheckBox*)pDlg->GetWindowForID(OTI_CB_M_AUTOSKILL_LX))->SetChecked(m_GameAI.sPage8.m_AutoSkill2);
	((cCheckBox*)pDlg->GetWindowForID(OTI_CB_M_AUTOSKILL_JYING))->SetChecked(m_GameAI.sPage8.m_AutoSkill3);
	((cCheckBox*)pDlg->GetWindowForID(OTI_CB_M_AUTOSKILL_HM))->SetChecked(m_GameAI.sPage8.m_AutoSkill4);
	((cCheckBox*)pDlg->GetWindowForID(OTI_CB_M_AUTOSKILL_TM))->SetChecked(m_GameAI.sPage8.m_AutoSkill5);
	((cCheckBox*)pDlg->GetWindowForID(OTI_CB_M_AUTOSKILL_XX))->SetChecked(m_GameAI.sPage8.m_AutoSkill6);
	((cCheckBox*)pDlg->GetWindowForID(OTI_CB_H_AUTOSKILL_JG))->SetChecked(m_GameAI.sPage8.h_AutoSkill1);
	((cCheckBox*)pDlg->GetWindowForID(OTI_CB_H_AUTOSKILL_SZ))->SetChecked(m_GameAI.sPage8.h_AutoSkill2);
	((cCheckBox*)pDlg->GetWindowForID(OTI_CB_H_AUTOSKILL_JYANG))->SetChecked(m_GameAI.sPage8.h_AutoSkill3);
	((cCheckBox*)pDlg->GetWindowForID(OTI_CB_H_AUTOSKILL_TX))->SetChecked(m_GameAI.sPage8.h_AutoSkill4);
	((cCheckBox*)pDlg->GetWindowForID(OTI_CB_H_AUTOSKILL_BM))->SetChecked(m_GameAI.sPage8.h_AutoSkill5);
	((cCheckBox*)pDlg->GetWindowForID(OTI_CB_H_AUTOSKILL_FM))->SetChecked(m_GameAI.sPage8.h_AutoSkill6);
	((cCheckBox*)pDlg->GetWindowForID(OTI_CB_H_AUTOSKILL_FUMO))->SetChecked(m_GameAI.sPage8.FuoMoo);
}
void CAiSetManager::TestQuickKey(DWORD AutoSkillBtnIdx)
{
	COptionDialog * m_Dlg = GAMEIN->GetOptionDialog();
	cDialog* pDlg;
	int iPushKey = 0;
	int iPushPage = 0;
	if (AutoSkillBtnIdx == OTI_BTN_AUTOHUNT_TIMER_TEST_01)
	{
		iPushKey = atoi(((cSpin*)((cDialog*)m_Dlg->GetTabSheet(5))->GetWindowForID(OTI_SP_AUTOHUNT_TIMER_SLOT_01))->GetEditText());
		iPushPage = atoi(((cSpin*)((cDialog*)m_Dlg->GetTabSheet(5))->GetWindowForID(OTI_SP_AUTOHUNT_TIMER_TAB_01))->GetEditText());
	}
	if (AutoSkillBtnIdx == OTI_BTN_AUTOHUNT_TIMER_TEST_02)
	{
		iPushKey = atoi(((cSpin*)((cDialog*)m_Dlg->GetTabSheet(5))->GetWindowForID(OTI_SP_AUTOHUNT_TIMER_SLOT_02))->GetEditText());
		iPushPage = atoi(((cSpin*)((cDialog*)m_Dlg->GetTabSheet(5))->GetWindowForID(OTI_SP_AUTOHUNT_TIMER_TAB_02))->GetEditText());
	}
	if (AutoSkillBtnIdx == OTI_BTN_AUTOHUNT_TIMER_TEST_03)
	{
		iPushKey = atoi(((cSpin*)((cDialog*)m_Dlg->GetTabSheet(5))->GetWindowForID(OTI_SP_AUTOHUNT_TIMER_SLOT_03))->GetEditText());
		iPushPage = atoi(((cSpin*)((cDialog*)m_Dlg->GetTabSheet(5))->GetWindowForID(OTI_SP_AUTOHUNT_TIMER_TAB_03))->GetEditText());
	}
	if (AutoSkillBtnIdx == OTI_BTN_AUTOHUNT_TIMER_TEST_04)
	{
		iPushKey = atoi(((cSpin*)((cDialog*)m_Dlg->GetTabSheet(5))->GetWindowForID(OTI_SP_AUTOHUNT_TIMER_SLOT_04))->GetEditText());
		iPushPage = atoi(((cSpin*)((cDialog*)m_Dlg->GetTabSheet(5))->GetWindowForID(OTI_SP_AUTOHUNT_TIMER_TAB_04))->GetEditText());
	}
	if (AutoSkillBtnIdx == OTI_BTN_AUTOHUNT_TIMER_TEST_05)
	{
		iPushKey = atoi(((cSpin*)((cDialog*)m_Dlg->GetTabSheet(5))->GetWindowForID(OTI_SP_AUTOHUNT_TIMER_SLOT_05))->GetEditText());
		iPushPage = atoi(((cSpin*)((cDialog*)m_Dlg->GetTabSheet(5))->GetWindowForID(OTI_SP_AUTOHUNT_TIMER_TAB_05))->GetEditText());
	}
	CQuickDialog * pQuickDlg = GAMEIN->GetQuickDialog();
	if (!pQuickDlg) return;
	int selectedQuickAbsPos = (iPushPage - 1) * TABCELL_QUICK_NUM + (iPushKey - 1);
	CQuickItem * pQuickItem = pQuickDlg->GetQuickItem(selectedQuickAbsPos);
	if (!pQuickItem)
	{
		CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(2542));
		return;
	}
	CSkillInfo* pInfo = NULL;
	pInfo = SKILLMGR->GetSkillInfo(pQuickItem->GetSrcIdx());
	if (!pInfo)
	{
		CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(2543));
		return;
	}
	QUICKMGR->UseQuickItem(iPushPage - 1, iPushKey - 1);
}
