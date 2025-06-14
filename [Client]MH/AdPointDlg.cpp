#include "stdafx.h"
#include "AdPointDlg.h"
#include "ObjectManager.h"
#include "ChatManager.h"
#include "GameIn.h"
#include "CharacterDialog.h"
#include "InventoryExDialog.h"
#include "cMsgBox.h"
#include "cWindowManager.h"

AdPointDlg::AdPointDlg(void)
{
		m_type=WT_ADDPOINT_DLG;
		m_AddPointOk=	NULL;
		m_Residue	=	NULL;
		m_Power		=	NULL;
		m_Dex		=	NULL;
		m_Con		=	NULL;
		m_Xm		=	NULL;

		m_count			= 0;
		m_ResidueCount	= 0;
		m_PowerCount	= 0;
		m_DexCount		= 0;
		m_ConCount		= 0;
		m_XmCount		= 0;
		m_dwDBIdx		= 0;
}


AdPointDlg::~AdPointDlg(void)
{

}
void AdPointDlg::Process()
{
	static DWORD dwNewTick;
	if(gCurTime-dwNewTick>500)
	{
		dwNewTick=gCurTime;

		m_PowerCount   = atoi(m_Power->GetEditText());
		m_DexCount	   = atoi(m_Dex->GetEditText());
		m_ConCount	   = atoi(m_Con->GetEditText());
		m_XmCount	   = atoi(m_Xm->GetEditText());


		//if((m_PowerCount || m_DexCount || m_ConCount || m_XmCount))
		{
			//m_count-=(m_PowerCount+m_DexCount+m_ConCount+m_XmCount);
			
			m_count=m_count-m_PowerCount;
			m_count=m_count-m_DexCount;
			m_count=m_count-m_ConCount;
			m_count=m_count-m_XmCount;

			/*m_count-=m_PowerCount;
			m_count-=m_DexCount;
			m_count-=m_ConCount;
			m_count-=m_XmCount;*/

			
		}
		/*else
		{
			m_count+=(m_PowerCount+m_DexCount+m_ConCount+m_XmCount);
		}*/

		char tmp[10];
		memset(tmp,0,10);
		itoa(m_count,tmp,10);
		m_Residue->SetEditText(tmp);
	}		
}
void AdPointDlg::Linking()
{
	m_AddPointOk = (cButton*)GetWindowForID(OTI_EDIT_ADDPOINT_OK);
	
	m_Residue	= (cEditBox*)GetWindowForID(OTI_EDIT_POINT_RESIDUE);
	m_Power		= (cEditBox*)GetWindowForID(OTI_EDIT_POINT_POWER);
	m_Dex		= (cEditBox*)GetWindowForID(OTI_EDIT_POINT_DEX);
	m_Con		= (cEditBox*)GetWindowForID(OTI_EDIT_POINT_CON);
	m_Xm		= (cEditBox*)GetWindowForID(OTI_EDIT_POINT_XM);
}
void AdPointDlg::Zero()
{
	char tmp[10];
	memset(tmp,0,10);
	itoa(m_ResidueCount,tmp,10);
	m_Residue->SetEditText(tmp);   //剩余属性点编辑框  by:胡汉三 QQ:112582793
	m_Residue->SetReadOnly(TRUE);
	m_Residue->SetActive(TRUE);

	memset(tmp,0,10);
	itoa(m_PowerCount,tmp,10);
	m_Power->SetEditText(tmp);		//力量属性点编辑框  by:胡汉三 QQ:112582793
	m_Power->SetActive(TRUE);
	if(m_ResidueCount==0)
		m_Power->SetReadOnly(TRUE);
	else
		m_Power->SetReadOnly(FALSE);
	
	memset(tmp,0,10);
	itoa(m_DexCount,tmp,10);
	m_Dex->SetEditText(tmp);			//敏捷属性点编辑框  by:胡汉三 QQ:112582793
	m_Dex->SetActive(TRUE);
	if(m_ResidueCount == 0)
		m_Dex->SetReadOnly(TRUE);
	else
		m_Dex->SetReadOnly(FALSE);

	memset(tmp,0,10);
	itoa(m_ConCount,tmp,10);
	m_Con->SetEditText(tmp);			//体质属性点编辑框  by:胡汉三 QQ:112582793
	m_Con->SetActive(TRUE);
	if(m_ResidueCount == 0)
		m_Con->SetReadOnly(TRUE);
	else
		m_Con->SetReadOnly(FALSE);
		
	memset(tmp,0,10);
	itoa(m_XmCount,tmp,10);
	m_Xm->SetEditText(tmp);		     //心脉属性点编辑框  by:胡汉三 QQ:112582793
	m_Xm->SetActive(TRUE);
	if(m_ResidueCount == 0)
		m_Xm->SetReadOnly(TRUE);
	else
		m_Xm->SetReadOnly(FALSE);
}
void AdPointDlg::Show(int m_ResidueCount)
{
	m_PowerCount   = 0;
	m_DexCount	   = 0;
	m_ConCount	   = 0;
	m_XmCount	   = 0;

	SetActive(TRUE);
	

	m_ResidueCount = HERO->GetHeroLevelUpPoint();

	m_count=m_ResidueCount;

	m_AddPointOk->SetActive(TRUE); //加点按钮  by:胡汉三 QQ:112582793
  
	char tmp[10];
	memset(tmp,0,10);
	itoa(m_ResidueCount,tmp,10);
	m_Residue->SetEditText(tmp);   //剩余属性点编辑框  by:胡汉三 QQ:112582793
	m_Residue->SetReadOnly(TRUE);
	m_Residue->SetActive(TRUE);

	memset(tmp,0,10);
	itoa(m_PowerCount,tmp,10);
	m_Power->SetEditText(tmp);		//力量属性点编辑框  by:胡汉三 QQ:112582793
	m_Power->SetActive(TRUE);
	if(m_ResidueCount==0)
		m_Power->SetReadOnly(TRUE);
	else
		m_Power->SetReadOnly(FALSE);
	
	memset(tmp,0,10);
	itoa(m_DexCount,tmp,10);
	m_Dex->SetEditText(tmp);			//敏捷属性点编辑框  by:胡汉三 QQ:112582793
	m_Dex->SetActive(TRUE);
	if(m_ResidueCount == 0)
		m_Dex->SetReadOnly(TRUE);
	else
		m_Dex->SetReadOnly(FALSE);

	memset(tmp,0,10);
	itoa(m_ConCount,tmp,10);
	m_Con->SetEditText(tmp);			//体质属性点编辑框  by:胡汉三 QQ:112582793
	m_Con->SetActive(TRUE);
	if(m_ResidueCount == 0)
		m_Con->SetReadOnly(TRUE);
	else
		m_Con->SetReadOnly(FALSE);
		
	memset(tmp,0,10);
	itoa(m_XmCount,tmp,10);
	m_Xm->SetEditText(tmp);		     //心脉属性点编辑框  by:胡汉三 QQ:112582793
	m_Xm->SetActive(TRUE);
	if(m_ResidueCount == 0)
		m_Xm->SetReadOnly(TRUE);
	else
		m_Xm->SetReadOnly(FALSE);
}


void AdPointDlg::Close()
{
	//Zero();
	SetActive(FALSE);
}
void AdPointDlg::AddPoint()    //快速加点函数  by:胡汉三 QQ:112582793
{
	if(m_dwDBIdx == 0) return;

	m_ResidueCount = HERO->GetHeroLevelUpPoint();

	m_PowerCount   = atoi(m_Power->GetEditText());
	m_DexCount	   = atoi(m_Dex->GetEditText());
	m_ConCount	   = atoi(m_Con->GetEditText());
	m_XmCount	   = atoi(m_Xm->GetEditText());

	if((m_PowerCount+m_DexCount+m_ConCount+m_XmCount)>m_ResidueCount)
	{
		CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(2237));
		return;
	}
	if((m_PowerCount+m_DexCount+m_ConCount+m_XmCount)==0)
	{
		CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(2238));
		return;
	}
	else
	{
		MSG_ADDPOINT msg;
		msg.Category = MP_ITEM;
		msg.Protocol = MP_ITEM_ADDPOINT_SYN;
		msg.dwObjectID = HERO->GetID();
		msg.GenGol = m_PowerCount;
		msg.MinChub = m_DexCount;
		msg.CheRyuk = m_ConCount;
		msg.SimMek = m_XmCount;
		msg.m_dwDBIdx  = m_dwDBIdx;
		m_ResidueCount -= m_PowerCount+m_DexCount+m_ConCount+m_XmCount;
		msg.LevelUpPoint = m_ResidueCount;
		NETWORK->Send(&msg, sizeof(msg));
	}
}

void AdPointDlg::OnActionEvent(LONG lId, void * p, DWORD we)
{
	if( we & WE_BTNCLICK )
	{
		if(lId == TB_CANCELBTN)
		{
			Close();
		}
		else if( lId == OTI_EDIT_ADDPOINT_OK ) 
		{
			CItem* pItem = GAMEIN->GetInventoryDialog()->GetCurSelectedItem(SHOPIVN);
			if( pItem )
			{
				SetItemDBIdx(pItem->GetDBIdx());
				WINDOWMGR->MsgBox( MBI_ISADDPIONT,MBT_YESNO, CHATMGR->GetChatMsg(2239) );
			}
			else
			{
				pItem = GAMEIN->GetInventoryDialog()->GetCurSelectedItem(SHOPIVNPLUS);
				if(pItem)
				{
					SetItemDBIdx(pItem->GetDBIdx());
					WINDOWMGR->MsgBox( MBI_ISADDPIONT,MBT_YESNO, CHATMGR->GetChatMsg(2239) );
				}
			}
		}
	}
}
