#include "stdafx.h"
#include "MHMap.h"
#include "WindowIDEnum.h"
#include "ChatManager.h"
#include "FBTimeDialog.h"
#include "InsFUBENMissionString.h"
#include "gamein.h"
#include "ObjectManager.h"
#include "./interface/cResourceManager.h"
#include "./interface/cButton.h"
#include "./input/UserInput.h"
#include "DungeonMgr.h"
CFBTimeDialog::CFBTimeDialog()
{
    pDlg=NULL;

	ZeroMemory( timeBuffer, sizeof(timeBuffer) );
	ZeroMemory( infoBuffer, sizeof(infoBuffer) );

	pFBStatic[FbTime]=NULL;
	pFBStatic[FbTargetInfo]=NULL;

	pFbMissionBtn=NULL;

	mCount=0;

	m_dwStoredTime=0;
	m_dwHitTime=0;

	CanStartTimer=false;
	bFlag=false;
}
CFBTimeDialog::~CFBTimeDialog()
{
    pDlg=NULL;

	pFBStatic[FbTime]=NULL;
	pFBStatic[FbTargetInfo]=NULL;

	pFbMissionBtn=NULL;

	mCount=0;

	m_dwStoredTime=0;
	m_dwHitTime=0;

	CanStartTimer=false;
	bFlag=false;
}
void CFBTimeDialog::Linking()
{
    pDlg=(cDialog *)GetWindowForID(INSFUBENINFO_DLG);

	pFbMissionBtn = (cButton *)GetWindowForID(INSFUBENINFO_MISSION_BTN);
	pFBStatic[FbTargetInfo] = (cStatic *)GetWindowForID(INSFUBENINFO_MISSION_COUNT);
	pFBStatic[FbTime] = (cStatic *)GetWindowForID(INSFUBENINFO_PROGRESS_TIME);
	pFBStatic[FbTime]->SetStaticText("00:00");
}
void CFBTimeDialog::SetActive()
{
	cDialog::SetActive(MAP->IsMapKind(eInsDungeon));
}
void CFBTimeDialog::SetFBTime(DWORD m_time)
{
	if(m_time==0 || m_time<0)
		bFlag=false;

	CanStartTimer=true;
	DWORD hour = m_time / ( 60 * 60 * 1000 );
	DWORD minute = ( m_time / ( 60 * 1000 ) ) % 60;
	DWORD second = ( m_time /  1000 ) % 60;

	sprintf(timeBuffer,"%02d:%02d",minute,second);
	pFBStatic[FbTime]->SetStaticText(timeBuffer);

	m_dwStoredTime=m_time;
	m_dwHitTime=gCurTime;	
}
void CFBTimeDialog::Process()
{
	if(!DungeonMGR->IsDungeonMap(MAP->GetMapNum())) return;
	if(!CanStartTimer) return;

	if(!bFlag&&(m_dwStoredTime==0 || m_dwStoredTime<=0))
	{
		bFlag=true;
		CanStartTimer=false;
		SetFBTime(0);
		SendOut();//kiv //20042022 2052
		//DungeonMGR->StartLeave();//kiv //20042022 2052
		//if(MAP->GetMapNum()!=17)
		//	g_UserInput.GetHeroMove()->MapChange(17,0,0,eMapChange_GuildWar);
		//return;
	}
	if(MAP->IsMapKind(eInsDungeon,MAP->GetMapNum()))
	{		
		if( gCurTime - m_dwHitTime >= 1000 )
		{
			m_dwStoredTime-=1000;

			SetFBTime(m_dwStoredTime);

			m_dwHitTime=gCurTime;
		}
	}	
}
void CFBTimeDialog::SendOut()
{
	static DWORD dwTiming;
	if(gCurTime-dwTiming>3000)
	{
		dwTiming=gCurTime;

		gChannelNum = 1;
		g_UserInput.GetHeroMove()->MapChange(DungeonMGR->GetInsDungeonInfoData(MAP->GetMapNum())->dgMapChange);
	}
}
void CFBTimeDialog::CreateFBTime()
{
	/*MSG_DWORD msg;
	msg.Category=MP_CHAR;
	msg.Protocol=MP_CHAR_FB_TIME_SYN;
	msg.dwData=1;
	msg.dwObjectID=HEROID;
    NETWORK->Send( &msg, sizeof(msg));*/
}
void CFBTimeDialog::OnActionEvent(LONG lId, void * p, DWORD we)
{
	switch(lId)
	{
	case INSFUBENINFO_MISSION_BTN:
		{
			GAMEIN->GetInsDGMissionDialog()->SetActive(!GAMEIN->GetInsDGMissionDialog()->IsActive());
		}
		break;
	default:
		//ASSERT(0);
		break;
	}
}


void CFBTimeDialog::SetMonsterCount(int Count,BYTE bFlag)
{
	mCount=Count;
	mCountMsg=bFlag;
	
	sprintf(infoBuffer,"%s %d/%d",RESRCMGR->GetMsg(37),mCount,mCountMsg);
	pFBStatic[FbTargetInfo]->SetStaticText(infoBuffer);
}
void CFBTimeDialog::AddMonsterCount(int Count)
{
	mCount=Count;
	sprintf(infoBuffer,"%s %d/%d",RESRCMGR->GetMsg(37),mCount,mCountMsg);
	pFBStatic[FbTargetInfo]->SetStaticText(infoBuffer);
}

void CFBTimeDialog::AddNpcPortal( float x, float z,DWORD MapKind)
{
	CObject* pObject = OBJECTMGR->GetObject( STATICNPC_ID_START + 100000 );
	if( pObject )
		return;

	NPC_TOTALINFO tinfo;
	BASEOBJECT_INFO binfo;
	BASEMOVE_INFO minfo;

	binfo.dwObjectID = STATICNPC_ID_START + 100000;
	binfo.ObjectState = 0;
	SafeStrCpy( binfo.ObjectName, "Dungeon", MAX_NAME_LENGTH+1 );
	
	tinfo.MapNum = MAP->GetMapNum();
	tinfo.NpcKind = 88;
	tinfo.NpcUniqueIdx = MapKind;

	tinfo.NpcJob = 88;

	minfo.bMoving = FALSE;
	minfo.KyungGongIdx = 0;
	minfo.MoveMode = 0;
	minfo.CurPosition.x = x;
	minfo.CurPosition.y = 0;
	minfo.CurPosition.z = z;

	CNpc* pNpc = OBJECTMGR->AddNpc(&binfo,&minfo,&tinfo);
	if( pNpc == NULL )
		return;

	float fDir = 0;
	pNpc->SetAngle( DEGTORAD( fDir ) );

	GAMEIN->GetMiniMapDialog()->AddStaticNpcIcon(pNpc);

	TARGETSET set;
	set.pTarget = pNpc;
	EFFECTMGR->StartEffectProcess(eEffect_MonRegen,pNpc,&set,0,pNpc->GetID());
}
void CFBTimeDialog::RemoveNpcPortal()
{
	CObject* pObject = OBJECTMGR->GetObject( STATICNPC_ID_START + 100000 );
	if( pObject == NULL )
		return;
	OBJECTMGR->AddGarbageObject( pObject );

	GAMEIN->GetMiniMapDialog()->RemoveIcon(pObject->GetID());
}