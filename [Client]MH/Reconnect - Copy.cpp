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
//#include "GameAIManager.h"            //weiye 内挂头文件 2017-12-07
#include "petmanager.h"
#include "SkillManager_client.h"
#include "MHMap.h"
#include "SafeNumberLockDialog.h"

//weiye 断线重连系统开发 2017-11-22

extern HWND _g_hWnd;           //weiye 游戏主窗口进程句柄 2017-12-05

extern char g_szHeroIDName[];

extern char g_CLIENTVERSION[];

extern int m_CharIndex;      //weiye 断线重连到那个角色 2018-04-15

extern int  m_ServerIndex;  //weiye 断线重连到那个线路 2018-04-16


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

	m_ServerIndex=0;       //weiye 默认连接第一个线路  2018-04-16

	m_CharIndex = 0;        //weiye ，默认选择第一个角色  Init 不初始化
 
	m_ChannelIndex=0;     //weiye 默认选择第一个分流   Init 不初始化

	m_dwMoveTime=0;       //weiye 重连后阻塞时间标志   Init 不初始化

	m_dwMovePosTime=0;    //weiye 移动到挂机地点阻塞时间标志

	
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

	m_dwMoveTime=0;       //weiye 重连后阻塞时间标志

	m_dwMovePosTime=0;    //weiye 移动到挂机地点阻塞时间标志

	
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
			HERO->RemoveAllObjectEffect();  //weiye 防止重新连接后卡技能 2016-08-22
			HERO->SetCurComboNum(0);
			HERO->SetMovingAction(NULL);	//weiye 停止角色移动状态 
			if(PETMGR->GetCurSummonPet())	//停止角色宠物移动 
			{
				MOVEMGR->PetMoveStop();
			}
			OBJECTSTATEMGR->StartObjectState( HERO, eObjectState_Exit );
			OBJECTMGR->RemoveAllObject();
		}

		MAINGAME->SetGameState( eGAMESTATE_TITLE );
		NETWORK->ReleaseKey();
		SKILLMGR->ReLoadSkillInfo();        //weiye 清理技能内存  
		TITLE->NoDisconMsg();
		TITLE->SetServerList();
	}
}

void CReDisConnManager::Process()
{
	//weiye 任何时候都断线重连  sCPlayerAI.m_OtherSet.AutoReconn  2018-05-02

	if(m_Start)  //weiye 是否断线重连 当游戏掉线时触发(需要辅助功能里面启用自动重连) 2016-08-22
	{
		if(m_ServerListStep)
		{
			//if(MOVEMGR->IsMoving(HERO))
			if(HERO->GetState() != eObjectState_Immortal || HERO->GetState() !=eObjectState_None )
				return;
            GoToGameLoginDialog();      //weiye 跳转到登陆窗口 2016-08-22

			SetServerListStep(FALSE);   //weiye 确保只返回到登陆界面一次 2016-08-25

		}
		if( gCurTime - m_dwStartTime >= 5000 )	//weiye  间隔5秒检查网络状态
	    {
			m_dwStartTime = m_dwStartTime + 4000;  //weiye 延迟5秒后继续检查进程 2016-08-25

			cDialog* m_ServerListDlg = WINDOWMGR->GetWindowForID( SL_SERVERLISTDLG); //weiye  判断线路窗口是否已经弹出 2016-08-22
	
			if(m_ServerListDlg)
			{
				if(m_ServerListDlg->IsActive())
				{
					m_ServerListDlg->SetDisable(TRUE);
					TITLE->ConnectToServer( m_ServerIndex );
                  /* LPARAM lParam = MAKEWPARAM((int)m_ServerListDlg->GetAbsX()+135,(int)m_ServerListDlg->GetAbsY()+65+m_ServerIndex*20); //weiye 这里依旧使用消息来实现点击服务器列表 2016-08-25

				   ::PostMessage(_g_hWnd,WM_LBUTTONDOWN,1,lParam);

				   ::PostMessage(_g_hWnd,WM_LBUTTONUP,0,lParam);

				   LPARAM lParam2 = MAKEWPARAM((int)m_ServerListDlg->GetAbsX()+75,(int)m_ServerListDlg->GetAbsY()+360);

				   ::PostMessage(_g_hWnd,WM_LBUTTONDOWN,1,lParam2);

				   ::PostMessage(_g_hWnd,WM_LBUTTONUP,0,lParam2);*/

				}

			}

			cDialog * m_LoginError =   WINDOWMGR->GetWindowForID(MBI_LOGINERRORCLOSE);  //weiye 此窗口出现时，无登录窗口 2018-04-05

			if(m_LoginError)
			{
				if(m_LoginError->IsActive())  //weiye 此处只能通过消息驱动来实现 2018-04-05
				{
					WINDOWMGR->CloseAllMsgBoxNoFunc();
					WINDOWMGR->CloseAllMsgBox();
						/*LPARAM lParam1 = MAKEWPARAM((int)m_LoginError->GetAbsX()+100,(int)m_LoginError->GetAbsY()+120);

						::PostMessage(_g_hWnd,WM_LBUTTONDOWN,1,lParam1);

						::PostMessage(_g_hWnd,WM_LBUTTONUP,0,lParam1);*/
				}
			}

			cDialog* m_LoginDlg = WINDOWMGR->GetWindowForID( MT_LOGINDLG );  //weiye 登陆窗口 2016-08-22

			if(m_LoginDlg)
			{
				if(m_LoginDlg->IsActive())
				{
					ExecuteLogin(m_LoginDlg);  //weiye 执行登陆过程

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
				
					cDialog* m_AgentErrorDlg = WINDOWMGR->GetWindowForID(MBI_LOGINERRORCLOSE);  //weiye 检查是不是Agent正常

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

					cDialog* m_LoginOnline=WINDOWMGR->GetWindowForID(MBI_OVERLAPPEDLOGIN);   //weiye 如果角色卡在线上，强制退出重新登录

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

			cDialog* m_CharSelectDlg = WINDOWMGR->GetWindowForID(CS_CHARSELECTDLG);  //weiye 选择角色窗口 2016-08-20

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

		if( curtime1 -  m_dwMoveTime >= 1000 )	//weiye  10秒进程阻塞 2016-08-22
	    {

			//if(sCPlayerAI.m_AutoSet.m_ObjSet.AutoOpen) //weiye 如果在挂机状态 则进行第二次地图转换 
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

	//weiye 移动到挂机点 2016-08-30

	if(m_MovePlayAIPos)
	{
		DWORD curtime2 = MHTIMEMGR->GetNewCalcCurTime() ;

		if( curtime2 -  m_dwMovePosTime >= 1000 )	//weiye 10 秒后移动到挂机地点
	    {
			//if(sCPlayerAI.m_AutoSet.m_ObjSet.AutoOpen) //weiye 如果在挂机状态 则进行第二次地图转换 
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
		SafeStrCpy(msg.pn, userPN, MAX_NAME_LENGTH + 1 );	//2008. 3. 18. CBH - 涝仿等 焊救锅龋 皋技瘤俊 历厘
		SafeStrCpy(msg.Version,g_CLIENTVERSION, 16 );
		msg.gate=0;
		msg.AuthKey = TITLE->GetDistAuthKey();
		NETWORK->Send(&msg,sizeof(msg));
		TITLE->StartWaitConnectToAgent( TRUE );
		USERINFOMGR->SetUserID( userID );
	}
}