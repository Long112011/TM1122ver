#include "stdafx.h"
#include "ItemStepReinforceDialog.h"
#include "GlobalEventFunc.h"
#include "WindowIDEnum.h"
#include "./Interface/cIconDialog.h"
#include "./Interface/cWindowManager.h"
#include "./Interface/cStatic.h"
#include "./Interface/cIcon.h"
#include "ChatManager.h"
#include "ObjectStateManager.h"
#include "ObjectManager.h"
#include "ItemManager.h"
#include "CommonGameFunc.h"
#include "InventoryExDialog.h"
#include "./Interface/cFont.h"
#define PERCENTAGE_STONE_1	0.033525
#define PERCENTAGE_STONE_2	0.4065//99.8659
#define PERCENTAGE_STONE_3	50.9001
#define PERCENTAGE_STONE_4	70.9001
#define PERCENTAGE_MAX		100.000
CItemStepReinforceDialog::CItemStepReinforceDialog()
{
	pDlg=NULL;


	memset(pStatic, 0, sizeof(cStatic*)* 19);
	memset(pButton, 0, sizeof(cButton*)* 14);
	/*for (int i = 0; i < 18; i++)
	{
		pStatic[i]=NULL;
	}
	for (int i = 0; i < 13; i++)
	{
		pButton[i]=NULL;
	}*/

	pXqItem=NULL;
	m_pIconDlgEquip0=NULL;
	m_pIconDlgEquip1=NULL;
	

    m_IconStone0=NULL;
    m_IconStone1=NULL;
    m_IconStone2=NULL;
    m_IconStone3=NULL;
    m_IconStone4=NULL;


	

	pCheckBox = NULL;

	pTextArea = NULL;

	pGuagene = NULL;

	m_MaxItemCount[0] = 0;
	m_MaxItemCount[1] = 0;
	m_MaxItemCount[2] = 0;
	m_MaxItemCount[3] = 0;

	m_bItemGrade = 0;
	m_bItemNextGrade = 0;

	m_RenderwhichButton = -1;
	m_StartRenderSecondProcess = FALSE;
}

CItemStepReinforceDialog::~CItemStepReinforceDialog()
{
	pDlg=NULL;
	
	memset(pStatic, 0, sizeof(cStatic*)* 19);
	memset(pButton, 0, sizeof(cButton*)* 14);

	pXqItem=NULL;
	m_pIconDlgEquip0=NULL;
	m_pIconDlgEquip1=NULL;

    m_IconStone0=NULL;
    m_IconStone1=NULL;
    m_IconStone2=NULL;
    m_IconStone3=NULL;
    m_IconStone4=NULL;

	

	pCheckBox = NULL;

	pTextArea = NULL;

	pGuagene = NULL;

	m_MaxItemCount[0] = 0;
	m_MaxItemCount[1] = 0;
	m_MaxItemCount[2] = 0;
	m_MaxItemCount[3] = 0;

	m_bItemGrade = 0;
	m_bItemNextGrade = 0;

	m_RenderwhichButton = -1;
	m_StartRenderSecondProcess = FALSE;
}
void CItemStepReinforceDialog::SetActive(BOOL flag)
{
	cDialog::SetActive(flag);
	ResetItemInfo();
}
void CItemStepReinforceDialog::OnActionEvent( LONG lId, void * p, DWORD we )
{	
	switch( lId )
	{		
		case ITEM_STEP_REINFORCE_BTN_CLOSE:
		{			
			this->SetActive(FALSE);
		}
		break;
		//------------------------------------------------------------------------------------increase
		case ITEM_STEP_REINFORCE_BTN_MATERIAL_INC_01:
		{			
			LONG number=pStatic[2]->GetStaticValue();
			if (number < m_MaxItemCount[0] && (float)gval*(GUAGEVAL)PERCENTAGE_MAX < (GUAGEVAL)PERCENTAGE_MAX)
			{
				number++;
			}
			else
			{
			//	CHATMGR->AddMsg(CTC_SYSMSG, "you only got this");
			}
			
			pStatic[2]->SetStaticValue(number);// ->SetActive(FALSE);
		}
		break;
		case ITEM_STEP_REINFORCE_BTN_MATERIAL_INC_02:
		{			
			LONG number=pStatic[3]->GetStaticValue();
			if (number<m_MaxItemCount[1] && (float)gval*(GUAGEVAL)PERCENTAGE_MAX < (GUAGEVAL)PERCENTAGE_MAX)
			number++;
			pStatic[3]->SetStaticValue(number);// ->SetActive(FALSE);
		}
		break;
		case ITEM_STEP_REINFORCE_BTN_MATERIAL_INC_03:
		{			
			LONG number=pStatic[4]->GetStaticValue();
			if (number<m_MaxItemCount[2] && (float)gval*(GUAGEVAL)PERCENTAGE_MAX < (GUAGEVAL)PERCENTAGE_MAX)
			number++;
			pStatic[4]->SetStaticValue(number);// ->SetActive(FALSE);
		}
		break;
		case ITEM_STEP_REINFORCE_BTN_MATERIAL_INC_04:
		{			
			LONG number=pStatic[5]->GetStaticValue();
			if (number<m_MaxItemCount[3] && (float)gval*(GUAGEVAL)PERCENTAGE_MAX < (GUAGEVAL)PERCENTAGE_MAX)
			number++;
			pStatic[5]->SetStaticValue(number);// ->SetActive(FALSE);
		}
		break;
		//------------------------------------------------------------------------------------decrease
		case ITEM_STEP_REINFORCE_BTN_MATERIAL_DEC_01:
		{			
			LONG number=pStatic[2]->GetStaticValue();
			if (number>0)
			number--;
			pStatic[2]->SetStaticValue(number);// ->SetActive(FALSE);
		}
		break;
		case ITEM_STEP_REINFORCE_BTN_MATERIAL_DEC_02:
		{			
			LONG number=pStatic[3]->GetStaticValue();
			if (number>0)
			number--;
			pStatic[3]->SetStaticValue(number);// ->SetActive(FALSE);
		}
		break;
		case ITEM_STEP_REINFORCE_BTN_MATERIAL_DEC_03:
		{			
			LONG number=pStatic[4]->GetStaticValue();
			if (number>0)
			number--;
			pStatic[4]->SetStaticValue(number);// ->SetActive(FALSE);
		}
		break;
		case ITEM_STEP_REINFORCE_BTN_MATERIAL_DEC_04:
		{			
			LONG number=pStatic[5]->GetStaticValue();
			if (number>0)
			number--;
			pStatic[5]->SetStaticValue(number);// ->SetActive(FALSE);
		}
		break;

		//------------------------------------------------------------------------------------max
		case ITEM_STEP_REINFORCE_BTN_MATERIAL_MAX_01:
		{			
			//LONG number=pStatic[2]->GetStaticValue();
			if (m_MaxItemCount[0]>0)
			//number--;
			StartRenderSecondProcess(0);
			//ItemStepReinforce_DlgFunc(ITEM_STEP_REINFORCE_BTN_MATERIAL_INC_01, NULL, 0);
			//pStatic[2]->SetStaticValue(m_MaxItemCount[0]);// ->SetActive(FALSE);
		}
		break;
		case ITEM_STEP_REINFORCE_BTN_MATERIAL_MAX_02:
		{			
			//LONG number=pStatic[3]->GetStaticValue();
			if (m_MaxItemCount[1]>0)
			//number--;
			StartRenderSecondProcess(1);
			//ItemStepReinforce_DlgFunc(ITEM_STEP_REINFORCE_BTN_MATERIAL_INC_02, NULL, 0);
			//pStatic[3]->SetStaticValue(m_MaxItemCount[1]);// ->SetActive(FALSE);
		}
		break;
		case ITEM_STEP_REINFORCE_BTN_MATERIAL_MAX_03:
		{			
			//LONG number=pStatic[4]->GetStaticValue();
			if (m_MaxItemCount[2]>0)
			//number--;
			StartRenderSecondProcess(2);
			//ItemStepReinforce_DlgFunc(ITEM_STEP_REINFORCE_BTN_MATERIAL_INC_03, NULL, 0);
			//pStatic[4]->SetStaticValue(m_MaxItemCount[2]);// ->SetActive(FALSE);
		}
		break;
		case ITEM_STEP_REINFORCE_BTN_MATERIAL_MAX_04:
		{			
			//LONG number=pStatic[5]->GetStaticValue();
			if (m_MaxItemCount[3] > 0)
			//number--;
			StartRenderSecondProcess(3);
			//ItemStepReinforce_DlgFunc(ITEM_STEP_REINFORCE_BTN_MATERIAL_INC_04, NULL, 0);
			//pStatic[5]->SetStaticValue(m_MaxItemCount[3]);// ->SetActive(FALSE);
		}
		break;
	}

	//if (pStatic[2]->GetStaticValue()!=0 || pStatic[3]->GetStaticValue()!=0 || pStatic[4]->GetStaticValue()!=0 || pStatic[5]->GetStaticValue()!=0)
	
	
}
#define KIRIKANAN	190
#define ATASBAWAH	80
#define FONTIDX		8
void CItemStepReinforceDialog::Render()
{
	if (!m_bActive) return;
	cDialog::RenderWindow();
	cDialog::RenderComponent();


	DWORD color2 = RGBA_MAKE(70,70,70,255);
	static char nums[32];

	sprintf(nums,"%1.4f",(float)gval*(GUAGEVAL)PERCENTAGE_MAX);
	RECT rect={(LONG)m_absPos.x+3+ATASBAWAH, (LONG)m_absPos.y+28+KIRIKANAN, 1,1};
	CFONT_OBJ->RenderFont( 3, nums, strlen(nums), &rect, color2 );
	RECT rect2={(LONG)m_absPos.x+2+ATASBAWAH, (LONG)m_absPos.y+27+KIRIKANAN, 1,1};
	CFONT_OBJ->RenderFont( 3, nums, strlen(nums), &rect2, 0xffffffff );

	//-----------------------------------------------------------------------------
	//current level
	if (m_bItemGrade != 0)
	{
		color2 = RGBA_MAKE(70,255,70,255);
		sprintf(nums,"+%d",m_bItemGrade);
		rect={(LONG)m_absPos.x+3, (LONG)m_absPos.y+28, 1,1};
		CFONT_OBJ->RenderFont( FONTIDX, nums, strlen(nums), &rect, color2 );
		rect2={(LONG)m_absPos.x+2, (LONG)m_absPos.y+27, 1,1};
		CFONT_OBJ->RenderFont( FONTIDX, nums, strlen(nums), &rect2, color2 );
	}
	
	//-----------------------------------------------------------------------------
	//next level
	if (m_bItemNextGrade != 0)
	{
		color2 = RGBA_MAKE(0,255,255,255);
		sprintf(nums,"+%d",m_bItemNextGrade);
		rect={(LONG)m_absPos.x+3+50, (LONG)m_absPos.y+28, 1,1};
		CFONT_OBJ->RenderFont( FONTIDX, nums, strlen(nums), &rect, color2 );
		rect2={(LONG)m_absPos.x+2+50, (LONG)m_absPos.y+27, 1,1};
		CFONT_OBJ->RenderFont( FONTIDX, nums, strlen(nums), &rect2, color2 );
	}
	
	//-----------------------------------------------------------------------------
	{
		GUAGEVAL gval1 = pStatic[2]->GetStaticValue()*PERCENTAGE_STONE_1;
		GUAGEVAL gval2 = pStatic[3]->GetStaticValue()*PERCENTAGE_STONE_2;
		GUAGEVAL gval3 = pStatic[4]->GetStaticValue()*PERCENTAGE_STONE_3;
		GUAGEVAL gval4 = pStatic[5]->GetStaticValue()*PERCENTAGE_STONE_4;

		GUAGEVAL gValTotal = (gval1 + gval2 + gval3 + gval4);
		if (gValTotal > (GUAGEVAL)PERCENTAGE_MAX)
		{
			gValTotal = (GUAGEVAL)PERCENTAGE_MAX;
		}
		gval = gValTotal / (GUAGEVAL)PERCENTAGE_MAX;
		//CHATMGR->AddMsg(CTC_SYSMSG, "%1.4f", gval);
		pGuagene->SetValue(gval);
	}
	RenderSecondProcess();

	JPRender();
	//cDialog::Render();
	//cWindow::Render();
}
void CItemStepReinforceDialog::StartRenderSecondProcess(WORD whichButton)
{
	m_RenderwhichButton = whichButton;
	m_StartRenderSecondProcess = TRUE;
}
void CItemStepReinforceDialog::RenderSecondProcess()
{
	if (m_RenderwhichButton == -1) return;
	if (!m_StartRenderSecondProcess) return;
	LONG number=pStatic[2+m_RenderwhichButton]->GetStaticValue();
	if (number < m_MaxItemCount[0+m_RenderwhichButton] && (float)gval*(GUAGEVAL)PERCENTAGE_MAX < (GUAGEVAL)PERCENTAGE_MAX)
	{
		number++;
		pStatic[2+m_RenderwhichButton]->SetStaticValue(number);
	}
	else
	{
		m_RenderwhichButton = -1;
		m_StartRenderSecondProcess = FALSE;
	}
			
	
}
void CItemStepReinforceDialog::Linking()
{
    pDlg = WINDOWMGR->GetWindowForID( ITEM_STEP_REINFORCE_DLG );

	for (int i = 0; i < 18; i++)
	{
		pStatic[0+i]=			(cStatic*)GetWindowForID(ITEM_STEP_REINFORCE_STC_COST+i);
	}
	

	pStatic[0]->SetStaticText(AddComma(10000));
	
	pStatic[2]->SetStaticValue(0);
	pStatic[3]->SetStaticValue(0);
	pStatic[4]->SetStaticValue(0);
	pStatic[5]->SetStaticValue(0);

	m_pIconDlgEquip0 = (cIconDialog*)GetWindowForID(ITEM_STEP_REINFORCE_ICON_BASE);
	m_pIconDlgEquip1 = (cIconDialog*)GetWindowForID(ITEM_STEP_REINFORCE_ICON_PROTECTION);
	

	for (int i = 0; i < 18; i++)
	{
		pButton[0+i]=			(cButton*)GetWindowForID(ITEM_STEP_REINFORCE_BTN_CLOSE+i);
	}
	

	pCheckBox =			(cCheckBox*)GetWindowForID(ITEM_STEP_REINFORCE_CHECK_BOX_USE_RED_MIX_STONE);

	pTextArea =			(cTextArea*)GetWindowForID(ITEM_STEP_REINFORCE_TEXTAREA_WARNING);

	pGuagene =			(cGuagen*)GetWindowForID(ITEM_STEP_REINFORCE_GAUGE_PROBABILITY);

	//GUAGEVAL gval = 10 / (GUAGEVAL)100;
	pGuagene->SetValue(0);

	AdaStone(1);
	AdaStone(2);
	AdaStone(3);
	AdaStone(4);
	
	JPInit();
	InitNumImage();
}
void CItemStepReinforceDialog::AdaStone(int Stone,BOOL ada)
{
	switch (Stone)
	{
	case 1:
	pStatic[6]->SetActive(!ada);
	pStatic[10]->SetActive(ada);//biru
	pStatic[14]->SetActive(ada);//merah
	break;
	case 2:
	pStatic[7]->SetActive(!ada);
	pStatic[11]->SetActive(ada);
	pStatic[15]->SetActive(ada);
	break;
	case 3:
	pStatic[8]->SetActive(!ada);
	pStatic[12]->SetActive(ada);
	pStatic[16]->SetActive(ada);
	break;
	case 4:
	pStatic[9]->SetActive(!ada);
	pStatic[13]->SetActive(ada);
	pStatic[17]->SetActive(ada);
	break;
	}
	//GUAGEVAL gval = 0.9001 / (GUAGEVAL)100.000;
	//pGuagene->SetValue(gval);
}
void CItemStepReinforceDialog::SetXqItem(CItem * pItem)
{
	pXqItem=pItem;
}

CItem * CItemStepReinforceDialog::GetXqItem()
{
    return pXqItem;
}

void CItemStepReinforceDialog::DeleteXqStoneAll()
{
	
}

BOOL CItemStepReinforceDialog::FakeMoveIcon(LONG x,LONG y,cIcon * icon)
{
	
	
GotoReset:

    if( icon->GetType() != WT_ITEM )	return FALSE;

	if( m_bDisable )	return FALSE;

	CItem* pOrigItem = (CItem*)icon;

	if( pOrigItem->IsLocked() )	return FALSE;

	ITEM_INFO* pItemInfo = ITEMMGR->GetItemInfo( pOrigItem->GetItemIdx());

	//ITEM_INFO * pInfo = ITEMMGR->GetItemInfo(index);
	//if (pInfo == NULL) return;

	if(!pItemInfo)	return FALSE;

	

	ITEM_INFO* pItemInfo2 = ITEMMGR->GetItemInfo( pOrigItem->GetItemIdx()+1);
	if(!pItemInfo2)	return FALSE;

	
	

	//CHATMGR->AddMsg(CTC_SYSMSG, "grade %d",pItemInfo->ItemGrade);

	if(m_VirtualItemEquip.GetData()==0)
	{

		eITEM_KINDBIT bits = pOrigItem->GetItemKind();

	   if(!ITEMMGR->IsStoneOptionItem(pOrigItem->GetItemIdx(),bits))
	   {
			CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1920));

			return FALSE;
	   }

	   /*ITEM_STONE_OPTION_INFO * pStone=ITEMMGR->GetItemStoneOption(pOrigItem->GetStoneIdx());

	   if(!pStone)
	   {
			CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1921));

			return FALSE;
	   }*/
 
	   if( !ITEMMGR->IsEqualTableIdxForPos(eItemTable_Inventory, pOrigItem->GetPosition()) && !ITEMMGR->IsEqualTableIdxForPos(eItemTable_ShopInven, pOrigItem->GetPosition()))
	   {
		    return FALSE;
	   }

	    pOrigItem->SetLock(TRUE);

		ResetItemInfo();
		for (int i = 0; i < 3; i++)
		{
			AdaStone(1+i);
			m_MaxItemCount[0+i] = 0;
		}
			/*AdaStone(1);
			AdaStone(2);
			AdaStone(3);
			AdaStone(4);
			m_MaxItemCount[0] = 0;
			m_MaxItemCount[1] = 0;
			m_MaxItemCount[2] = 0;
			m_MaxItemCount[3] = 0;*/

	   

		m_bItemGrade = pItemInfo->ItemGrade;

		if (m_bItemGrade < 19/* && (pItemInfo2->ItemGrade - 1 == pItemInfo->ItemGrade)*/)
		{
			
			 AddVirtualItem( pOrigItem );

			AddVirtualIcon( pOrigItem);
	
			m_bItemNextGrade = pItemInfo2->ItemGrade;
			for (int i = TP_INVENTORY_START; i < TP_INVENTORY_END; i++)
			{
				CItem * PetFood = GAMEIN->GetInventoryDialog()->GetItemForPos(i);
				if (PetFood)
				{
					//ITEM_INFO * pInfo = ITEMMGR->GetItemInfo(PetFood->GetItemBaseInfo()->Durability);
					if (PetFood->GetItemIdx() == 8502)
					{
						//GAMEIN->GetInventoryDialog()->UseItem(PetFood);
					//	CHATMGR->AddMsg(CTC_SYSMSG, "8502 %d",PetFood->GetItemBaseInfo()->Durability);
						m_MaxItemCount[2] = m_MaxItemCount[2] + PetFood->GetItemBaseInfo()->Durability;
						AdaStone(3, TRUE);
						//break;
					}
					else if (PetFood->GetItemIdx() == 8501 )
					{
					//	CHATMGR->AddMsg(CTC_SYSMSG, "8501 %d",PetFood->GetItemBaseInfo()->Durability);
						m_MaxItemCount[1] = m_MaxItemCount[1] + PetFood->GetItemBaseInfo()->Durability;
						AdaStone(2, TRUE);
						//GAMEIN->GetInventoryDialog()->UseItem(PetFood);
						//break;
					}
					else if (PetFood->GetItemIdx() == 8500)
					{
						//PetFood->GetItemBaseInfo()->Durability;
					//	CHATMGR->AddMsg(CTC_SYSMSG, "8500 %d",PetFood->GetItemBaseInfo()->Durability);
						m_MaxItemCount[0] = m_MaxItemCount[0] + PetFood->GetItemBaseInfo()->Durability;
						AdaStone(1, TRUE);
						//GAMEIN->GetInventoryDialog()->UseItem(PetFood);
						//break;
					}
				}
			}
			pStatic[1]->SetStaticText(pItemInfo->ItemName);
		}
		else
		{
			CHATMGR->AddMsg(CTC_SYSMSG, "return max item?");
			return FALSE;
		}

	}
	else
	{
		eITEM_KINDBIT bits = pOrigItem->GetItemKind();


		if(bits==eEXTRA_ITEM_MATERIAL && pOrigItem->GetItemIdx()>= 51004  && pOrigItem->GetItemIdx()<= 51100)
		{

		  if(pOrigItem->GetDurability()>1)
		  {
			  return FALSE;
		  }

	      pOrigItem->SetLock(TRUE);

	      AddVirtualItem( pOrigItem );
		}
		else
		{
			ITEM_STONE_OPTION_INFO * pStone=ITEMMGR->GetItemStoneOption(pOrigItem->GetStoneIdx());

			if(pStone)
			{
                ResetItemInfo();

			    goto GotoReset;
			}
			else
			{
				 ResetItemInfo();

			    goto GotoReset;

				//CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1923));
				//return FALSE;
			}
		}
	}	
	ITEMMGR->SetDisableDialog(TRUE, eItemTable_Pyoguk);
	ITEMMGR->SetDisableDialog(TRUE, eItemTable_GuildWarehouse);
	ITEMMGR->SetDisableDialog(TRUE, eItemTable_Shop);
	ITEMMGR->SetDisableDialog(TRUE, eItemTable_PetInven);

	return FALSE;
}

void CItemStepReinforceDialog::AddVirtualItem(CItem * pItem)
{
	if(m_VirtualItemEquip.GetData()==0)
	{
		cIcon* pIcon=(cIcon*)pItem;
		m_VirtualItemEquip.SetData( pItem->GetItemIdx());
		m_VirtualItemEquip.SetLinkItem(pItem);
		m_pIconDlgEquip0->AddIcon( 0, (cIcon*)&m_VirtualItemEquip );
		m_pIconDlgEquip0->SetDisable(TRUE);

	//	m_pIconDlgStone2->Init(m_pIconDlgEquip0->GetAbsX(), m_pIconDlgEquip0->GetAbsY(), m_pIconDlgEquip0->GetWidth(), m_pIconDlgEquip0->GetHeight(),
	//						  m_pIconDlgEquip0->GetBasicImage(), -1);
    }
	else
	{
		/*CItem * pTargetItem = ITEMMGR->GetItem(m_VirtualItemEquip.GetLinkItem()->GetDBIdx());
		if(!pTargetItem) return;
		ITEM_STONE_OPTION_INFO * pStoneInfo = ITEMMGR->GetItemStoneOption(pTargetItem->GetStoneIdx());
		if(!pStoneInfo) return;
		if(pStoneInfo->dwItemStone0==1 && m_VirtualItemStone0.GetData()==0)
		{
			cIcon* pIcon=(cIcon*)pItem;
	        m_VirtualItemStone0.SetData( pItem->GetItemIdx());
	        m_VirtualItemStone0.SetLinkItem(pItem);
	        m_pIconDlgStone0->AddIcon( 0, (cIcon*)&m_VirtualItemStone0 );
			m_pIconDlgStone0->SetDepend(TRUE);
	        ITEMMGR->SetToolTipIcon((cIcon*)&m_VirtualItemStone0,NULL,NULL,0,NULL);
			pItem->SetLock(TRUE);
			return;
		}
		if(pStoneInfo->dwItemStone1==1 && m_VirtualItemStone1.GetData()==0)
		{
			cIcon* pIcon=(cIcon*)pItem;
	        m_VirtualItemStone1.SetData( pItem->GetItemIdx());
	        m_VirtualItemStone1.SetLinkItem(pItem);
	        m_pIconDlgStone1->AddIcon( 0, (cIcon*)&m_VirtualItemStone1 );
			m_pIconDlgStone1->SetDepend(TRUE);
	        ITEMMGR->SetToolTipIcon((cIcon*)&m_VirtualItemStone1,NULL,NULL,0,NULL);
			pItem->SetLock(TRUE);
			return;
		}
		if(pStoneInfo->dwItemStone2==1 && m_VirtualItemStone2.GetData()==0)
		{
			cIcon* pIcon=(cIcon*)pItem;
	        m_VirtualItemStone2.SetData( pItem->GetItemIdx());
	        m_VirtualItemStone2.SetLinkItem(pItem);
	        m_pIconDlgStone2->AddIcon( 0, (cIcon*)&m_VirtualItemStone2 );
			m_pIconDlgStone2->SetDepend(TRUE);
	        ITEMMGR->SetToolTipIcon((cIcon*)&m_VirtualItemStone2,NULL,NULL,0,NULL);
			pItem->SetLock(TRUE);
			return;
		}
		if(pStoneInfo->dwItemStone3==1 && m_VirtualItemStone3.GetData()==0)
		{
			cIcon* pIcon=(cIcon*)pItem;
	        m_VirtualItemStone3.SetData( pItem->GetItemIdx());
	        m_VirtualItemStone3.SetLinkItem(pItem);
	        m_pIconDlgStone3->AddIcon( 0, (cIcon*)&m_VirtualItemStone3 );
			m_pIconDlgStone3->SetDepend(TRUE);
	        ITEMMGR->SetToolTipIcon((cIcon*)&m_VirtualItemStone3,NULL,NULL,0,NULL);
			pItem->SetLock(TRUE);
			return;
		}
		if(pStoneInfo->dwItemStone4==1 && m_VirtualItemStone4.GetData()==0)
		{
			cIcon* pIcon=(cIcon*)pItem;
	        m_VirtualItemStone4.SetData( pItem->GetItemIdx());
	        m_VirtualItemStone4.SetLinkItem(pItem);
	        m_pIconDlgStone4->AddIcon( 0, (cIcon*)&m_VirtualItemStone4 );
			m_pIconDlgStone4->SetDepend(TRUE);
	        ITEMMGR->SetToolTipIcon((cIcon*)&m_VirtualItemStone4,NULL,NULL,0,NULL);
			pItem->SetLock(TRUE);
			return;
		}
		if(GetStoneXqCount(pStoneInfo)<5)
		{
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(2129));
				pItem->SetLock(FALSE);
				return ;
		}
		else
		{
				if(pStoneInfo->dwItemStone0>1)  // ´ÓÕâÀï¿×
				{
					CVirtualItem* pVItem = NULL;
					m_pIconDlgStone0->DeleteIcon( 0, (cIcon**)&pVItem );
					m_VirtualItemStone0.SetLinkItem( NULL );
					cIcon* pIcon=(cIcon*)pItem;
					m_VirtualItemStone0.SetData( pItem->GetItemIdx());
					m_VirtualItemStone0.SetLinkItem(pItem);
					m_pIconDlgStone0->AddIcon( 0, (cIcon*)&m_VirtualItemStone0 );
					m_pIconDlgStone0->SetDepend(TRUE);
					ITEMMGR->SetToolTipIcon((cIcon*)&m_VirtualItemStone0,NULL,NULL,0,NULL);
					pItem->SetLock(TRUE);
					return;
				}
				if(pStoneInfo->dwItemStone1>1)
				{
					CVirtualItem* pVItem = NULL;
					m_pIconDlgStone1->DeleteIcon( 0, (cIcon**)&pVItem );
					m_VirtualItemStone1.SetLinkItem( NULL );
					cIcon* pIcon=(cIcon*)pItem;
					m_VirtualItemStone1.SetData( pItem->GetItemIdx());
					m_VirtualItemStone1.SetLinkItem(pItem);
					m_pIconDlgStone1->AddIcon( 0, (cIcon*)&m_VirtualItemStone1 );
					m_pIconDlgStone1->SetDepend(TRUE);
					ITEMMGR->SetToolTipIcon((cIcon*)&m_VirtualItemStone1,NULL,NULL,0,NULL);
					pItem->SetLock(TRUE);
					return;
				}
				if(pStoneInfo->dwItemStone2>1)
				{
					CVirtualItem* pVItem = NULL;
					m_pIconDlgStone2->DeleteIcon( 0, (cIcon**)&pVItem );
					m_VirtualItemStone2.SetLinkItem( NULL );
					cIcon* pIcon=(cIcon*)pItem;
					m_VirtualItemStone2.SetData( pItem->GetItemIdx());
					m_VirtualItemStone2.SetLinkItem(pItem);
					m_pIconDlgStone2->AddIcon( 0, (cIcon*)&m_VirtualItemStone2 );
					m_pIconDlgStone2->SetDepend(TRUE);
					ITEMMGR->SetToolTipIcon((cIcon*)&m_VirtualItemStone2,NULL,NULL,0,NULL);
					pItem->SetLock(TRUE);
					return;
				}
				if(pStoneInfo->dwItemStone3>1)
				{
					CVirtualItem* pVItem = NULL;
					m_pIconDlgStone3->DeleteIcon( 0, (cIcon**)&pVItem );
					m_VirtualItemStone3.SetLinkItem( NULL );
					cIcon* pIcon=(cIcon*)pItem;
					m_VirtualItemStone3.SetData( pItem->GetItemIdx());
					m_VirtualItemStone3.SetLinkItem(pItem);
					m_pIconDlgStone3->AddIcon( 0, (cIcon*)&m_VirtualItemStone3 );
					m_pIconDlgStone3->SetDepend(TRUE);
					ITEMMGR->SetToolTipIcon((cIcon*)&m_VirtualItemStone3,NULL,NULL,0,NULL);
					pItem->SetLock(TRUE);
					return;
				}
				if(pStoneInfo->dwItemStone0>1)
				{
					CVirtualItem* pVItem = NULL;
					m_pIconDlgStone4->DeleteIcon( 0, (cIcon**)&pVItem );
					m_VirtualItemStone4.SetLinkItem( NULL );
					cIcon* pIcon=(cIcon*)pItem;
					m_VirtualItemStone4.SetData( pItem->GetItemIdx());
					m_VirtualItemStone4.SetLinkItem(pItem);
					m_pIconDlgStone4->AddIcon( 0, (cIcon*)&m_VirtualItemStone4 );
					m_pIconDlgStone4->SetDepend(TRUE);
					ITEMMGR->SetToolTipIcon((cIcon*)&m_VirtualItemStone4,NULL,NULL,0,NULL);
					pItem->SetLock(TRUE);
					return;
				}
		}*/
    }
}

void CItemStepReinforceDialog::AddVirtualIcon(CItem * pItem)
{
	
}

void CItemStepReinforceDialog::SendItemStoneXqMsg()
{
	
}

void CItemStepReinforceDialog::ResetItemInfo()
{
	m_bItemGrade = 0;
	m_bItemNextGrade = 0;
	pGuagene->SetValue(0);

	AdaStone(1);
	AdaStone(2);
	AdaStone(3);
	AdaStone(4);

	CVirtualItem* pVItem = NULL;

	if( m_pIconDlgEquip0 )
	{
		

		m_pIconDlgEquip0->DeleteIcon( 0, (cIcon**)&pVItem );

		if( pVItem )
			pVItem->GetLinkItem()->SetLock( FALSE );

		m_VirtualItemEquip.SetLinkItem( NULL );
		m_VirtualItemEquip.SetData(0);
	}
}
void CItemStepReinforceDialog::Release()
{

}

int  CItemStepReinforceDialog::GetStoneVirtualCount()
{
	int Count=0;

	return Count;
	
}

int  CItemStepReinforceDialog::GetStoneXqCount(ITEM_STONE_OPTION_INFO * pStoneInfo)
{
	int Count=0;

	return Count;
}
void CItemStepReinforceDialog::Process()
{
	JPProcess();
}
#include "./Interface/cScriptManager.h"
LONG NumImgRelpos2[] =
{
	135, 191,
	119, 191,
	103, 191,
	87, 191,
	71, 191,
	55, 191,
	39, 191,
	23, 191,
	7, 191,
};
void CItemStepReinforceDialog::InitNumImage()
{
	cImage* pImg;
	for (int n = 0; n < IMAGE_COUNT2; ++n)
	{
		pImg = new cImage;
		SCRIPTMGR->GetImage(n, pImg, PFT_JACKPOTPATH);
		m_stNumImage[n].pImage = pImg;
	}
}
void CItemStepReinforceDialog::ReleaseNumImage()
{
	for (int n = 0; n < IMAGE_COUNT2; ++n)
	{
		if (m_stNumImage[n].pImage)
		{
			delete m_stNumImage[n].pImage;
			m_stNumImage[n].pImage = NULL;
		}
	}
}
void CItemStepReinforceDialog::SetNumImagePos()
{
	float fDialPosX = this->GetAbsX();
	float fDialPosY = this->GetAbsY();
	for (int i = 0; i < CIPHER_NUM2; ++i)
	{
		m_vPos[i].x = fDialPosX + (float)NumImgRelpos2[i * 2];
		m_vPos[i].y = fDialPosY + (float)NumImgRelpos2[i * 2 + 1];
	}
	m_vDotPos[0].x = fDialPosX + 98;
	m_vDotPos[0].y = fDialPosY + 205;
	m_vDotPos[1].x = fDialPosX + 50;
	m_vDotPos[1].y = fDialPosY + 205;
}
void CItemStepReinforceDialog::ConvertCipherNum()
{
	int n = 1;
	DWORD d = 10;
	while (m_dwTotalMoney / d > 0)
	{
		d *= 10;
		n++;
	}
	m_dwMaxCipher = n;
	DWORD dec = 10;
	DWORD tmp = m_dwTotalMoney;
	for (int i = 0; i < n; ++i)
	{
		m_stCipherNum[i].dwNumber = tmp % dec;
		m_stCipherNum[i].bIsAni = TRUE;
		tmp /= dec;
	}
	while (n < CIPHER_NUM2)
	{
		m_stCipherNum[n].dwNumber = DEFAULT_IMAGE2;
		m_stCipherNum[n].bIsAni = FALSE;
		n++;
	}
}
bool CItemStepReinforceDialog::IsNumChanged()
{
	if (m_dwOldTotalMoney != m_dwTotalMoney)
	{
		if (m_dwTotalMoney < m_dwOldTotalMoney || m_dwOldTotalMoney == 0)
		{
			m_bDoSequenceAni = FALSE;
		}
		else m_bDoSequenceAni = TRUE;
		m_dwTempMoney = m_dwOldTotalMoney;
		m_dwOldTotalMoney = m_dwTotalMoney;
		return TRUE;
	}
	else
		return FALSE;
}
void CItemStepReinforceDialog::DoAni()
{
	if (!m_bIsAnimationing) return;
	DWORD curtime = gCurTime;
	if (curtime - m_dwNumChangeTime > NUM_CHANGE_TIMELENGTH)
	{
		for (int i = 0; i < CIPHER_NUM2; ++i)
		{
			if (m_stCipherNum[i].bIsAni)
			{
				m_stCipherNum[i].dwNumber++;
				if (m_stCipherNum[i].dwNumber > 9)
				{
					m_stCipherNum[i].dwNumber = 0;
				}
			}
		}
	}
	if (curtime - m_dwAniStartTime < BASIC_ANI_TIMELENGTH) return;
	if (curtime - m_dwIntervalAniTime > BETWEEN_ANI_TIMELENGTH)
	{
		if (m_stCipherNum[m_dwCipherCount].bIsAni)
		{
			m_stCipherNum[m_dwCipherCount].dwNumber = m_stCipherNum[m_dwCipherCount].dwRealCipherNum;
			m_stCipherNum[m_dwCipherCount].bIsAni = FALSE;
		}
		if (m_dwCipherCount == m_dwMaxCipher)
		{
			m_bIsAnimationing = FALSE;
			return;
		}
		m_dwIntervalAniTime = curtime;
		m_dwCipherCount++;
	}
}
void CItemStepReinforceDialog::DoSequenceAni()
{
	if (!m_bIsAnimationing)
	{
		return;
	}
	DWORD EllipsedTime = gCurTime - m_dwAniStartTime;
	DWORD durMoney = EllipsedTime * 33;
	m_dwTotalMoney = m_dwTempMoney + durMoney;
	if (m_dwTotalMoney >= m_dwOldTotalMoney)
	{
		m_dwTotalMoney = m_dwOldTotalMoney;
		m_bIsAnimationing = FALSE;
	}
}
void CItemStepReinforceDialog::JPRender()
{
	for (int i = 0; i < CIPHER_NUM2; ++i)
	{
		if (m_stCipherNum[i].dwNumber == DEFAULT_IMAGE2)
			continue;
		m_stNumImage[m_stCipherNum[i].dwNumber].pImage->RenderSprite(NULL, NULL, 0.0f, &m_vPos[i], 0xffffffff);
	}
	if (m_dwMaxCipher < 4 || m_dwMaxCipher > 9) return;
	int j = (m_dwMaxCipher - 1) / 3;
	while (j)
	{
		m_stNumImage[IMG_DOT].pImage->RenderSprite(NULL, NULL, 0.0f, &m_vDotPos[j - 1], 0xffffffff);
		j--;
	}
}
void CItemStepReinforceDialog::JPProcess()
{
	m_dwTotalMoney = (float)((float)gval*(GUAGEVAL)PERCENTAGE_MAX) * 10000;// HERO->GetLevel();// JACKPOTMGR->GetJPTotalMoney();
	if (m_dwTotalMoney > 999999999)
	{
		m_dwTotalMoney = 999999999;
	}
	SetNumImagePos();
	if (IsNumChanged())
	{
		InitForSequenceAni();
	}
	DoSequenceAni();
	ConvertCipherNum();
}
void CItemStepReinforceDialog::InitForAni()
{
	for (int i = 0; i < CIPHER_NUM2; ++i)
	{
		m_stCipherNum[i].dwRealCipherNum = m_stCipherNum[i].dwNumber;
	}
	m_bIsAnimationing = TRUE;
	m_dwCipherCount = 0;
	m_dwAniStartTime = gCurTime;
}
void CItemStepReinforceDialog::InitForSequenceAni()
{
	if (!m_bDoSequenceAni) return;
	m_bIsAnimationing = TRUE;
	m_dwAniStartTime = gCurTime;
	m_dwTotalMoney = m_dwOldTotalMoney;
}
void CItemStepReinforceDialog::JPInit()
{
	memset(m_vPos, 0, sizeof(VECTOR2)*CIPHER_NUM2);
	m_dwTotalMoney = 0;
	m_dwOldTotalMoney = 0;
	m_dwTempMoney = 0;
	m_dwAniStartTime = 0;
	m_dwNumChangeTime = 0;
	m_dwIntervalAniTime = 0;
	m_dwMaxCipher = 0;
	m_dwCipherCount = 0;
	m_bIsAnimationing = FALSE;
	m_bDoSequenceAni = FALSE;
	ConvertCipherNum();
}