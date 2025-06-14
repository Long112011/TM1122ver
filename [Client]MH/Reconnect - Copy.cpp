#include "stdafx.h"
#include "Reconnect.h"
#include "CharSelect.h"
#include "MainTitle.h"
#include "MainGame.h"
#include "Gamein.h"
#include "GameState.h"
#include "MHCamera.h"
#include "ObjectManager.h"
#include "MoveManager.h"
#include "ObjectStateManager.h"
#include "GlobalEventFunc.h"
#include "ServerListDialog.h"
#include "NumberPadDialog.h"
#include "WindowIDEnum.h"
#include "cResourceManager.h"
#include "cDialog.h"
#include "./Interface/CButton.h"
#include "cMsgBox.h"
#include "cWindowManager.h"
#include "ChannelDialog.h"
#include "MHTimeManager.h"
#include "OptionManager.h"
#include "ChatManager.h"
#include "UserInfoManager.h"
//#include "GameAIManager.h"            //weiye �ڹ�ͷ�ļ� 2017-12-07
#include "petmanager.h"
#include "SkillManager_client.h"
#include "MHMap.h"
#include "SafeNumberLockDialog.h"

//weiye ��������ϵͳ���� 2017-11-22

extern HWND _g_hWnd;           //weiye ��Ϸ�����ڽ��̾�� 2017-12-05

extern char g_szHeroIDName[];

extern char g_CLIENTVERSION[];

extern int m_CharIndex;      //weiye �����������Ǹ���ɫ 2018-04-15

extern int  m_ServerIndex;  //weiye �����������Ǹ���· 2018-04-16


GLOBALTON(CReDisConnManager);


CReDisConnManager::CReDisConnManager()
{
	dwHeroMapNum=0;
	m_HeroChannel=0;

    m_dwStartTime=0;

	m_Start=FALSE;

	m_ServerListStep=FALSE;

	m_MovePlayAIMap=FALSE;

	m_MovePlayAIMapStatic=FALSE;

	m_MovePlayAIPos=FALSE;

	m_MovePlayAIPosStatic=FALSE;

	m_ServerIndex=0;       //weiye Ĭ�����ӵ�һ����·  2018-04-16

	m_CharIndex = 0;        //weiye ��Ĭ��ѡ���һ����ɫ  Init ����ʼ��
 
	m_ChannelIndex=0;     //weiye Ĭ��ѡ���һ������   Init ����ʼ��

	m_dwMoveTime=0;       //weiye ����������ʱ���־   Init ����ʼ��

	m_dwMovePosTime=0;    //weiye �ƶ����һ��ص�����ʱ���־

	
}

CReDisConnManager::~CReDisConnManager()
{

}

void CReDisConnManager::Init()
{
	//MOVEMGR->HeroMoveStop();

	HERO->GetPosition(&m_Heropos);
	dwHeroMapNum=MAP->GetMapNum();
	m_HeroChannel=gChannelNum;

    m_dwStartTime=0;

	m_Start=FALSE;

	m_ServerListStep=FALSE;

	m_MovePlayAIMap=FALSE;

	m_MovePlayAIMapStatic=FALSE;

	m_MovePlayAIPos=FALSE;

	m_MovePlayAIPosStatic=FALSE;

	m_dwMoveTime=0;       //weiye ����������ʱ���־

	m_dwMovePosTime=0;    //weiye �ƶ����һ��ص�����ʱ���־

	
}


void CReDisConnManager::StartProcess()
{
	m_Start=TRUE;
	m_ServerListStep=TRUE;
	//WORD dwCurTime = MHTIMEMGR->GetNewCalcCurTime();
	m_dwStartTime= MHTIMEMGR->GetNewCalcCurTime() - 1000;//dwCurTime - 3000;	
}
void CReDisConnManager::GoToGameLoginDialog()
{
	if( MAINGAME->GetCurStateNum() == eGAMESTATE_TITLE  )
		TITLE->ShowServerList();
	else
	{
		if(OBJECTMGR->GetHero())
		{
			HERO->DisableAutoAttack();					
			HERO->SetNextAction(NULL);	
			HERO->RemoveAllObjectEffect();  //weiye ��ֹ�������Ӻ󿨼��� 2016-08-22
			HERO->SetCurComboNum(0);
			HERO->SetMovingAction(NULL);	//weiye ֹͣ��ɫ�ƶ�״̬ 
			if(PETMGR->GetCurSummonPet())	//ֹͣ��ɫ�����ƶ� 
			{
				MOVEMGR->PetMoveStop();
			}
			OBJECTSTATEMGR->StartObjectState( HERO, eObjectState_Exit );
			OBJECTMGR->RemoveAllObject();
		}

		MAINGAME->SetGameState( eGAMESTATE_TITLE );
		NETWORK->ReleaseKey();
		SKILLMGR->ReLoadSkillInfo();        //weiye �������ڴ�  
		TITLE->NoDisconMsg();
		TITLE->SetServerList();
	}
}

void CReDisConnManager::Process()
{
	//weiye �κ�ʱ�򶼶�������  sCPlayerAI.m_OtherSet.AutoReconn  2018-05-02

	if(m_Start)  //weiye �Ƿ�������� ����Ϸ����ʱ����(��Ҫ�����������������Զ�����) 2016-08-22
	{
		if(m_ServerListStep)
		{
			//if(MOVEMGR->IsMoving(HERO))
			if(HERO->GetState() != eObjectState_Immortal || HERO->GetState() !=eObjectState_None )
				return;
            GoToGameLoginDialog();      //weiye ��ת����½���� 2016-08-22

			SetServerListStep(FALSE);   //weiye ȷ��ֻ���ص���½����һ�� 2016-08-25

		}
		if( gCurTime - m_dwStartTime >= 5000 )	//weiye  ���5��������״̬
	    {
			m_dwStartTime = m_dwStartTime + 4000;  //weiye �ӳ�5������������ 2016-08-25

			cDialog* m_ServerListDlg = WINDOWMGR->GetWindowForID( SL_SERVERLISTDLG); //weiye  �ж���·�����Ƿ��Ѿ����� 2016-08-22
	
			if(m_ServerListDlg)
			{
				if(m_ServerListDlg->IsActive())
				{
					m_ServerListDlg->SetDisable(TRUE);
					TITLE->ConnectToServer( m_ServerIndex );
                  /* LPARAM lParam = MAKEWPARAM((int)m_ServerListDlg->GetAbsX()+135,(int)m_ServerListDlg->GetAbsY()+65+m_ServerIndex*20); //weiye ��������ʹ����Ϣ��ʵ�ֵ���������б� 2016-08-25

				   ::PostMessage(_g_hWnd,WM_LBUTTONDOWN,1,lParam);

				   ::PostMessage(_g_hWnd,WM_LBUTTONUP,0,lParam);

				   LPARAM lParam2 = MAKEWPARAM((int)m_ServerListDlg->GetAbsX()+75,(int)m_ServerListDlg->GetAbsY()+360);

				   ::PostMessage(_g_hWnd,WM_LBUTTONDOWN,1,lParam2);

				   ::PostMessage(_g_hWnd,WM_LBUTTONUP,0,lParam2);*/

				}

			}

			cDialog * m_LoginError =   WINDOWMGR->GetWindowForID(MBI_LOGINERRORCLOSE);  //weiye �˴��ڳ���ʱ���޵�¼���� 2018-04-05

			if(m_LoginError)
			{
				if(m_LoginError->IsActive())  //weiye �˴�ֻ��ͨ����Ϣ������ʵ�� 2018-04-05
				{
					WINDOWMGR->CloseAllMsgBoxNoFunc();
					WINDOWMGR->CloseAllMsgBox();
						/*LPARAM lParam1 = MAKEWPARAM((int)m_LoginError->GetAbsX()+100,(int)m_LoginError->GetAbsY()+120);

						::PostMessage(_g_hWnd,WM_LBUTTONDOWN,1,lParam1);

						::PostMessage(_g_hWnd,WM_LBUTTONUP,0,lParam1);*/
				}
			}

			cDialog* m_LoginDlg = WINDOWMGR->GetWindowForID( MT_LOGINDLG );  //weiye ��½���� 2016-08-22

			if(m_LoginDlg)
			{
				if(m_LoginDlg->IsActive())
				{
					ExecuteLogin(m_LoginDlg);  //weiye ִ�е�½����

					/*cDialog* m_LoginOnline			= WINDOWMGR->GetWindowForID(MBI_OVERLAPPEDLOGIN);
					if(m_LoginOnline)
					{
						if(m_LoginOnline->IsActive())
						{
							MSGBASE msg;
							msg.Category = MP_USERCONN;
							msg.Protocol = MP_USERCONN_FORCE_DISCONNECT_OVERLAPLOGIN;
							NETWORK->Send(&msg,sizeof(msg));
							m_LoginOnline->SetActive(FALSE);
							GoToGameLoginDialog();
						}
					}*/
				
					cDialog* m_AgentErrorDlg = WINDOWMGR->GetWindowForID(MBI_LOGINERRORCLOSE);  //weiye ����ǲ���Agent����

					if(m_AgentErrorDlg)
					{
						if(m_AgentErrorDlg->IsActive()) 
						{
							WINDOWMGR->CloseAllMsgBoxNoFunc();
							WINDOWMGR->CloseAllMsgBox();
							/*LPARAM lParam1 = MAKEWPARAM((int)m_AgentErrorDlg->GetAbsX()+100,(int)m_AgentErrorDlg->GetAbsY()+120);

							::PostMessage(_g_hWnd,WM_LBUTTONDOWN,1,lParam1);

							::PostMessage(_g_hWnd,WM_LBUTTONUP,0,lParam1);*/
						}
					}

					cDialog* m_LoginWait = WINDOWMGR->GetWindowForID(MBI_WAIT_LOGINCHECK);

					if(m_LoginWait)
					{
						if(m_LoginWait->IsActive())  
						{
							/*LPARAM lParam1 = MAKEWPARAM((int)m_LoginWait->GetAbsX()+100,(int)m_LoginWait->GetAbsY()+120);

							::PostMessage(_g_hWnd,WM_LBUTTONDOWN,1,lParam1);

							::PostMessage(_g_hWnd,WM_LBUTTONUP,0,lParam1);*/
						}
					}

					cDialog* m_LoginOnline=WINDOWMGR->GetWindowForID(MBI_OVERLAPPEDLOGIN);   //weiye �����ɫ�������ϣ�ǿ���˳����µ�¼

					if(m_LoginOnline)
					{
						if(m_LoginOnline->IsActive())
						{

							MSGBASE msg;
							msg.Category = MP_USERCONN;
							msg.Protocol = MP_USERCONN_FORCE_DISCONNECT_OVERLAPLOGIN;
							NETWORK->Send(&msg,sizeof(msg));
							/*
							LPARAM lParam1 = MAKEWPARAM((int)m_LoginOnline->GetAbsX()+68,(int)m_LoginOnline->GetAbsY()+120);
							::PostMessage(_g_hWnd,WM_LBUTTONDOWN,1,lParam1);
							::PostMessage(_g_hWnd,WM_LBUTTONUP,0,lParam1);
							*/
						}

					}

					cDialog* m_LoginCheck = WINDOWMGR->GetWindowForID(MBI_LOGINEDITCHECK); 

					if(m_LoginCheck)
					{

						if(m_LoginCheck->IsActive())
						{
							WINDOWMGR->CloseAllMsgBoxNoFunc();
							WINDOWMGR->CloseAllMsgBox();
							/*LPARAM lParam1 = MAKEWPARAM((int)m_LoginCheck->GetAbsX()+100,(int)m_LoginCheck->GetAbsY()+120);

							::PostMessage(_g_hWnd,WM_LBUTTONDOWN,1,lParam1);

							::PostMessage(_g_hWnd,WM_LBUTTONUP,0,lParam1);*/
						}

					}
				
			     }

			}

			cDialog* m_CharSelectDlg = WINDOWMGR->GetWindowForID(CS_CHARSELECTDLG);  //weiye ѡ���ɫ���� 2016-08-20

			if(m_CharSelectDlg)
			{
			        if(m_CharSelectDlg->IsActive())
					{

						 CHARSELECT->SelectPlayer(m_CharIndex);

						if(	CHARSELECT->GetCurSelectedPlayer() == NULL)
						{
							CHARSELECT->DisplayNotice(18);
						}
						else
						{
							CHARSELECT->SendMsgGetChannelInfo();
							CHARSELECT->SetDisablePick(TRUE);
						}

						cDialog* m_MapClose = WINDOWMGR->GetWindowForID(MBI_MAPSERVER_CLOSE);

						if(m_MapClose)
						{
							if(m_MapClose->IsActive())
							{
								/*LPARAM lParam1 = MAKEWPARAM((int)m_MapClose->GetAbsX()+100,(int)m_MapClose->GetAbsY()+120);

								::PostMessage(_g_hWnd,WM_LBUTTONDOWN,1,lParam1);

								::PostMessage(_g_hWnd,WM_LBUTTONUP,0,lParam1);*/
								WINDOWMGR->CloseAllMsgBoxNoFunc();
								WINDOWMGR->CloseAllMsgBox();
							}

						}
						cDialog* m_ChannelDlg = WINDOWMGR->GetWindowForID(CHA_CHANNELDLG); 
						if(m_ChannelDlg)
						{
							if(m_ChannelDlg->IsActive())
							{
								if(MAINGAME->GetCurStateNum() == eGAMESTATE_CHARSELECT)
								{
									LPARAM  lParam = MAKEWPARAM((int)m_ChannelDlg->GetAbsX()+135,(int)m_ChannelDlg->GetAbsY()+365);

									::PostMessage(_g_hWnd,WM_LBUTTONDOWN,1,lParam);

									::PostMessage(_g_hWnd,WM_LBUTTONUP,0,lParam);
								}
					
		    				}
							else
							{
								LPARAM  lParam2 = MAKEWPARAM((int)m_ChannelDlg->GetAbsX()+100,(int)m_ChannelDlg->GetAbsY()+270);

								::PostMessage(_g_hWnd,WM_LBUTTONDOWN,1,lParam2);
				
								::PostMessage(_g_hWnd,WM_LBUTTONUP,0,lParam2);
							}
						}

						/*CChannelDialog* m_ChannelDlg = (CChannelDialog*)WINDOWMGR->GetWindowForID(CHA_CHANNELDLG); 
						if(m_ChannelDlg)
						{
							if(m_ChannelDlg->IsActive())
							{
								if(MAINGAME->GetCurStateNum() == eGAMESTATE_CHARSELECT)
								{
									CHARSELECT->SelectPlayer(m_CharIndex);
								}
								if( CHARSELECT->GetCurSelectedPlayer() )
								{
									CHARSELECT->SendMsgGetChannelInfo();
								}
								if(m_ChannelDlg)
								{
									if(m_ChannelDlg->IsActive())
									{
										m_ChannelDlg->SelectChannel(m_ChannelIndex);
										m_ChannelDlg->OnConnect();						
									}
								}					
		    				}
							else
							{
								m_ChannelDlg->MapChange();
							}
						}*/
			       }
			}
			m_dwStartTime=gCurTime;

		}

	}

    if(m_MovePlayAIMap)
	{
		DWORD curtime1 = MHTIMEMGR->GetNewCalcCurTime() ;

		if( curtime1 -  m_dwMoveTime >= 1000 )	//weiye  10��������� 2016-08-22
	    {

			//if(sCPlayerAI.m_AutoSet.m_ObjSet.AutoOpen) //weiye ����ڹһ�״̬ ����еڶ��ε�ͼת�� 
			//{
				if(HERO->GetState() == eObjectState_Immortal || HERO->GetState() ==eObjectState_None )
				{
					int gChannelNum1;
					gChannelNum=m_HeroChannel;

					MSG_NAME_DWORD2 msg;
					msg.Category	= MP_MOVE;
					msg.Protocol	= MP_MOVE_RECONN_SYN;
					msg.dwObjectID	= HEROID;
					msg.dwData1		= dwHeroMapNum;//(DWORD)sCPlayerAI.m_AutoSet.m_ObjSet.LastMap;
					msg.dwData2		= (DWORD)gChannelNum-1;
					msg.Name[0]		= 0;	//hero
					NETWORK->Send( &msg, sizeof(msg) );
					m_MovePlayAIMap=FALSE;
					m_MovePlayAIMapStatic=TRUE;
				}

			//}
			//else
			//{
			//	 m_Start=FALSE;
			//}
            m_dwMoveTime=MHTIMEMGR->GetNewCalcCurTime();
		}
	}

	//weiye �ƶ����һ��� 2016-08-30

	if(m_MovePlayAIPos)
	{
		DWORD curtime2 = MHTIMEMGR->GetNewCalcCurTime() ;

		if( curtime2 -  m_dwMovePosTime >= 1000 )	//weiye 10 ����ƶ����һ��ص�
	    {
			//if(sCPlayerAI.m_AutoSet.m_ObjSet.AutoOpen) //weiye ����ڹһ�״̬ ����еڶ��ε�ͼת�� 
			//{
				if(HERO->GetState() == eObjectState_Immortal || HERO->GetState() ==eObjectState_None )
				{
					if (MAP->CollisionCheck_OneTile(&m_Heropos) == TRUE)
					{
						MOVE_POS_USER msg;
						msg.Category	= MP_MOVE;
						msg.Protocol	= MP_MOVE_RECONN_POS_SYN;
						msg.dwObjectID	= HEROID;
						SafeStrCpy( msg.Name, HERO->GetObjectName(), MAX_NAME_LENGTH + 1 );
						msg.cpos.Compress(&m_Heropos);
						NETWORK->Send(&msg,sizeof(msg));

						MOVEMGR->SetHeroTarget(&m_Heropos,1.f);
					}             
				}

				CSafeNumberLockDialog* SafeDialog=GAMEIN->GetSafeNumberLockDialog();
				if(SafeDialog->IsActive())
				{
					if(!SafeDialog->CheckDialog())
					{
						SafeDialog->SetPwdStr(SafePwd);
						//CHATMGR->AddMsg( CTC_SYSMSG, "m_MovePlayAIPos return");
						SafeDialog->CheckDialog();
						//return;
					}
				}
				m_MovePlayAIPos=FALSE;
				m_MovePlayAIPosStatic=TRUE;
				m_Start=FALSE;

				//CHATMGR->AddMsg( CTC_SYSMSG, "m_MovePlayAIPos passed");


			//}
			//else
			//{
			//	m_Start=FALSE;
			//}

		    m_dwMovePosTime= MHTIMEMGR->GetNewCalcCurTime() ;
		}
	}


}


void CReDisConnManager::ExecuteLogin(cDialog * p)
{
	cDialog * dlg = (cDialog *)p;
	cEditBox * editboxId = (cEditBox *)dlg->GetWindowForID(MT_IDEDITBOX);
	cEditBox * editboxPwd = (cEditBox *)dlg->GetWindowForID(MT_PWDEDITBOX);
	cEditBox * editboxPin=(cEditBox *)dlg->GetWindowForID(MT_PNSTATIC);

	editboxId->SetEditText(UserName);
	editboxPwd->SetEditText(UserPwd);
	editboxPin->SetEditText(UserPin);

	char * userID = editboxId->GetEditText();
	char * userPWD = editboxPwd->GetEditText();	
	char * userPN = "1111";//pNumberPadDlg->GetProtectionStr();
	
	//MT_LogInOkBtnFunc( MT_OKBTN, p, WE_BTNCLICK );
    /*LPARAM lParam = MAKEWPARAM((int)p->GetAbsX()+70,(int)p->GetAbsY()+110);
	::PostMessage(_g_hWnd,WM_LBUTTONDOWN,1,lParam);
	::PostMessage(_g_hWnd,WM_LBUTTONUP,0,lParam);*/

	if((strcmp(userID, "") != 0) && (strcmp(userPWD, "") != 0))
	{
		MSG_LOGIN_SYN msg;
		msg.Category = MP_USERCONN;
		msg.Protocol = MP_USERCONN_LOGIN_SYN;
		SafeStrCpy(msg.id, userID, MAX_NAME_LENGTH + 1 );
		SafeStrCpy(msg.pw, userPWD, MAX_NAME_LENGTH + 1 );
		SafeStrCpy(msg.pn, userPN, MAX_NAME_LENGTH + 1 );	//2008. 3. 18. CBH - �Էµ� ���ȹ�ȣ �޼����� ����
		SafeStrCpy(msg.Version,g_CLIENTVERSION, 16 );
		msg.gate=0;
		msg.AuthKey = TITLE->GetDistAuthKey();
		NETWORK->Send(&msg,sizeof(msg));
		TITLE->StartWaitConnectToAgent( TRUE );
		USERINFOMGR->SetUserID( userID );
	}
}