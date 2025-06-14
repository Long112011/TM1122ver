#include "stdafx.h"
#include "cLoginManager.h"

#include "GameIn.h"
#include "MainTitle.h"
#include "Reconnect.h"
#include "ChatManager.h"
#include "cMsgBox.h"
#include "WindowIdEnum.h"
#include "./interface/cWindowManager.h"
#include "UserInfoManager.h"

extern char g_CLIENTVERSION[];
extern char g_szHeroIDName[];

GLOBALTON(cLoginManager)
cLoginManager::cLoginManager()
{
}
cLoginManager::~cLoginManager()
{
}
void cLoginManager::UserData(char * Username, char * Userpass)
{
	char * Userpin;
	if (REDISCONN->GetStart())
		strcpy(Userpin, "2222");
	else
		strcpy(Userpin, "1111");

	/*if (!m_crypt.EncryptUser(Username, strlen(Username)))
	{
		return;
	}
	m_crypt.GetEnCRCConvertCharUser();*/
	REDISCONN->SetUserName(Username);
	REDISCONN->SetUserPwd(Userpass);
	REDISCONN->SetUserPin(Userpin);

	MSG_LOGIN_SYN msg;
	msg.Category = MP_USERCONN;
	msg.Protocol = MP_USERCONN_LOGIN_SYN;
	SafeStrCpy(msg.id, Username, MAX_NAME_LENGTH + 1);
	SafeStrCpy(msg.pw, Userpass, MAX_NAME_LENGTH + 1);
	SafeStrCpy(msg.pn, Userpin, MAX_NAME_LENGTH + 1);
	SafeStrCpy(msg.Version, g_CLIENTVERSION, 256);
	GAMEIN->GetMacAddress(msg.Mac);
	msg.AuthKey = TITLE->GetDistAuthKey();
	NETWORK->Send(&msg, sizeof(msg));

	SafeStrCpy(g_szHeroIDName, Username, MAX_NAME_LENGTH + 1);

	cMsgBox* pMsgBox = WINDOWMGR->MsgBox(MBI_WAIT_LOGINCHECK, MBT_CANCEL, CHATMGR->GetChatMsg(291));
	cDialog* pIDDlg = WINDOWMGR->GetWindowForID(MT_LOGINDLG);
	if (pIDDlg && pMsgBox)
	{
		pMsgBox->SetAbsXY(pIDDlg->GetAbsX(), pIDDlg->GetAbsY() + pIDDlg->GetHeight());
	}
	TITLE->StartWaitConnectToAgent(TRUE);
	USERINFOMGR->SetUserID(Username);
}
