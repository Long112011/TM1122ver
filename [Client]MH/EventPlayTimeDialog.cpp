#include "stdafx.h"
#include "EventPlayTimeDialog.h"
//#include "MHMap.h"
//#include "MHFile.h"
#include "WindowIDEnum.h"
#include "./Interface/cWindowManager.h"
//#include "./interface/cStatic.h"
//#include "./interface/cButton.h"


#include "cScriptManager.h"
//#include "./Interface/cIconGridDialog.h"
//#include "./Interface/cPushupButton.h"
//#include "./Interface/cIcon.h"
//#include "cMsgBox.h"
//#include "cDivideBox.h"
//#include "Item.h"
#include "ChatManager.h"
#include "ItemManager.h"
//#include "ObjectManager.h"
//#include "ObjectStateManager.h"
//#include "ExchangeItem.h"
//#include "./interface/cFont.h"
//#include "DealItem.h"
extern char JACKJACK[64];
bool b_WaitFirstEvent;
CEventPlayTimeDlg::CEventPlayTimeDlg()
{
	pDlg=NULL;
	m_pEngraveGuagen=NULL;
	m_pPlayTimeStatic=NULL;
	m_pRemaintimeStatic=NULL;
	
	TempImage=NULL;
	QuantityStatic=NULL;
	m_dwProcessTime = 0;
	//m_fBasicTime = 1.0f;
	//TempImage=NULL;
	m_dwItemIdx=0;
	m_dwNFlag=1;
	m_bCheckSum=0;
	m_dwItemKey=0;

	//pDitem=NULL;

	m_StoredTime=0;

	nLimitTime=0;

	//iTemp=0;
	value=0;

	m_dwItemQty=0;

	m_dwTotalTime=0;
	dwTempTime=0;

	dwTiming=0;
	//ZeroMemory( timeBuffer, sizeof(timeBuffer) );
	ZeroMemory( ItemName, sizeof(ItemName) );
	//ZeroMemory( IconItemName, sizeof(IconItemName) );

	//b_WaitFirstEvent=true;
}
CEventPlayTimeDlg::~CEventPlayTimeDlg()
{
	pDlg=NULL;
	m_pEngraveGuagen=NULL;
	m_pPlayTimeStatic=NULL;
	m_pRemaintimeStatic=NULL;
	
	TempImage=NULL;
	QuantityStatic=NULL;
}
void CEventPlayTimeDlg::Linking()
{
	pDlg				= WINDOWMGR->GetWindowForID( EVENT_PLAY_TIME_DLG );
	m_pEngraveGuagen	= (CObjectGuagen*)GetWindowForID( EVENT_PLAY_TIME_GUAGE_TIME );
	m_pPlayTimeStatic	= (cStatic*)GetWindowForID( EVENT_PLAY_TIME_STATIC_PERIOD );
	m_pRemaintimeStatic = (cStatic*)GetWindowForID( EVENT_PLAY_TIME_STATIC_TIME );
	
	TempImage = new cStatic; 
	TempImage->Init( 7, 29, DEFAULT_ICONSIZE,DEFAULT_ICONSIZE, NULL, -1 );
	Add( TempImage );

	QuantityStatic = new cStatic;
	QuantityStatic->Init( 34, 57, 1,1, NULL, -1 );
	Add( QuantityStatic );
	
}

void CEventPlayTimeDlg::ExRender()
{
	nLimitTime = ( m_dwProcessTime - gCurTime ) / 1000;

	static int last;
	if( last != nLimitTime )
	{
		if( nLimitTime > -1 )
		{
			dwTiming++;

			char buf[256] = {0,};

			//int Hour = nLimitTime / 3600;
			//int Min  = ( nLimitTime - 3600 * Hour )/ 60;
			//int Sec  = nLimitTime - Min * 60;

//-------------------------------------

			int seconds, hours, minutes;

			seconds = nLimitTime;
			minutes = seconds / 60;
			hours	= minutes / 60;

			if( int(hours) > 1191 && int(minutes%60) != 0 && int(seconds%60) != 0 )
			{
				return;
			}
			if( hours )
			{
				wsprintf( buf, CHATMGR->GetChatMsg( 2663 ), int(hours), int(minutes%60), int(seconds%60) ) ;
			}
			else if( minutes )
			{
				wsprintf( buf, CHATMGR->GetChatMsg( 2507 ), int(minutes%60), int(seconds%60) ) ;
			}
			else if( seconds )
			{
				wsprintf( buf, CHATMGR->GetChatMsg( 2568 ), int(seconds%60) ) ;
			}
			else
			{
				wsprintf( buf, CHATMGR->GetChatMsg( 2664 ) ) ;
			}

			ITEM_INFO * pInfo = ITEMMGR->GetItemInfo(m_dwItemKey);
			if(pInfo)
			{
				char buf3[MAX_ITEMNAME_LENGTH+1];
				cImage imgToolTip;
				cImage imgHeader;
				cImage * imgEquipInfo;
				SCRIPTMGR->GetImage(25, &imgHeader, PFT_JACKPATH );
				SCRIPTMGR->GetImage(26, &imgToolTip, PFT_JACKPATH );

				TempImage->SetHeaderImage(&imgHeader);
				TempImage->SetToolTip( "", RGBA_MAKE(255, 255, 255, 255), &imgToolTip, TTCLR_ITEM_CANEQUIP );
				TempImage->SetToolModel(1);
				
				imgEquipInfo=TempImage->GetBasicImage();
				if(imgEquipInfo)
				{
					TempImage->AddToolTipImageEquip(imgEquipInfo);
				}
				else
				{
					TempImage->AddToolTipLine( " " );
				}
				wsprintf( buf3, "[%s]", ItemName );
				TempImage->AddToolTipLine( buf3 ,TTTC_DEFAULT,NULL,6);
				TempImage->AddToolTipLine( JACKJACK );
				ITEMMGR->SetExtraItemToolTip((cIcon*)TempImage,pInfo);
				TempImage->AddToolTipLine( " " );				
			}			

			static char mData[MAX_ITEMNAME_LENGTH+1];
			wsprintf(mData,"%4d", m_dwItemQty);
			QuantityStatic->SetStaticText(mData);

			//m_pRemaintimeStatic->SetWH(122,20);
			m_pRemaintimeStatic->SetStaticText( buf );

			last	= nLimitTime;			
			value = (double)dwTiming/(double)m_dwTotalTime;
			m_pEngraveGuagen->SetValue((GUAGEVAL)value, 0) ;

			//iTemp	=(DWORD)m_StoredTime-last;
			//value	= (double)iTemp/(double)m_StoredTime;

			/*int dwTempMaster=(5000/m_dwTotalTime)*dwTiming;
			static int dwTempMinus;
			static int dwTempMinus2;

			dwTempMinus=dwTempMaster;//(1500/m_dwTotalTime)*dwTiming;
			CHATMGR->AddMsg(CTC_SYSMSG,"[total:%d] [average:%d]",dwTempMaster,dwTempMinus-dwTempMinus2);
			dwTempMinus=0;
			dwTempMinus2=dwTempMaster;//(1500/m_dwTotalTime)*dwTiming;*/


			//m_pEngraveGuagen->SetValue( (GUAGEVAL)value, 500.f) ;//kiv
			//m_pEngraveGuagen->SetValue((float)nLimitTime/(float)m_dwTotalTime, (1500/m_dwTotalTime)*nLimitTime) ;//nice gak

			
			

			
			
			//m_pEngraveGuagen->SetValue(58.f/60.f, 0) ;

			

			//CHATMGR->AddMsg(CTC_SYSMSG,"[nLimitTime:%d] [m_dwTotalTime:%d] [value:%d] [iTemp:%d] [last:%d] ",
			//							nLimitTime,m_dwTotalTime,value,iTemp,last);


			//m_pEngraveGuagen->SetValue( (GUAGEVAL)last/m_fBasicTime, last );//backup kalau buat mcmni .. dia akan kira < menurun/desc

			//contoh yg boleh direfer
			//if( m_pLifeGuage )
			//{
			//	if(type == 0)
			//		m_pLifeGuage->SetValue((float)curVal/(float)maxVal, 0);
			//	else
			//		m_pLifeGuage->SetValue((float)curVal/(float)maxVal, (1500/maxVal)*curVal);
			//}
		}		
	}
	

	
	
	//static char mData[MAX_ITEMNAME_LENGTH+1];
	//wsprintf(mData,"%4d", m_dwItemQty);
	//RECT rect={(LONG)GetAbsX(), (LONG)GetAbsY(), 1,1};
	//CFONT_OBJ->RenderFont(0,mData,strlen(mData),&rect,RGBA_MERGE(m_dwImageRGB, m_alpha * m_dwOptionAlpha / 100 ));

	//cDialog::Render();
}
void CEventPlayTimeDlg::SetEventTime(DWORD m_time,BYTE CheckSum,DWORD ItemKey,char*Text,DWORD dwQty,DWORD dwTotalTime)
{
	ZeroMemory( ItemName, sizeof(ItemName) );

	m_dwTotalTime=dwTotalTime/*-1*/;//kiv
	m_StoredTime=m_time;

	dwTiming=m_dwTotalTime-m_StoredTime;


	//m_dwProcessTime=m_time;



	//CHATMGR->AddMsg(CTC_SYSMSG,"[m_time:%d] [m_dwTotalTime:%d] [dwTiming:%d]",m_time,dwTotalTime,dwTiming);

	/*if(b_WaitFirstEvent)
	{
		b_WaitFirstEvent=false;
		CHATMGR->AddMsg(CTC_SYSMSG,CHATMGR->GetChatMsg(2662));
	}*/
	
	//m_dwProcessTime=0;
	//m_dwProcessTime = m_time;	
	m_bCheckSum=CheckSum;
	m_dwItemKey=ItemKey;
	m_dwItemQty=dwQty;

	//CHATMGR->AddMsg(CTC_SYSMSG,"QTY:%d",dwQty);
	cImage itemImage;
	ITEMMGR->GetIconImage(m_dwItemKey, &itemImage);
	TempImage->SetBasicImage(&itemImage);
	//Item(ItemKey);
	//DWORD dwFlagTemp=0;
	if(m_bCheckSum==0)
	{
		//dwFlagTemp=gCurTime;
		cImage tempImage;
		cImage tempImage2;
		SCRIPTMGR->GetImage(87, &tempImage, PFT_JACKPATH);
		SCRIPTMGR->GetImage(88, &tempImage2, PFT_JACKPATH);
		m_pEngraveGuagen->SetPieceImage(&tempImage);
		m_pEngraveGuagen->SetEffectPieceImage(&tempImage2);
		m_pPlayTimeStatic->SetFGColor(0x00FF00);		
	}
	if(m_bCheckSum==1)
	{
		//dwFlagTemp=gCurTime;//0;//kiv
		cImage tempImage;
		cImage tempImage2;
		SCRIPTMGR->GetImage(84, &tempImage, PFT_JACKPATH);
		SCRIPTMGR->GetImage(85, &tempImage2, PFT_JACKPATH);
		strcpy(Text,CHATMGR->GetChatMsg( 2546 )/*,17*/);
		m_pEngraveGuagen->SetPieceImage(&tempImage);
		m_pEngraveGuagen->SetEffectPieceImage(&tempImage2);
		m_pPlayTimeStatic->SetFGColor(0xFD0019);
		//m_pEngraveGuagen->SetDisable(TRUE);
	}
	
	//m_dwProcessTime = ( gCurTime + m_time ) * 1000 ;
	m_dwProcessTime = gCurTime + m_time * 1000 ;
	//m_fBasicTime = (GUAGEVAL)m_time;

	ITEM_INFO * pInfo = ITEMMGR->GetItemInfo(m_dwItemKey);
	if(pInfo)
	{
		SafeStrCpy(ItemName,pInfo->ItemName,MAX_ITEMNAME_LENGTH+1);
	}
	
	if(!pDlg->IsActive() && b_WaitFirstEvent)
	{
		b_WaitFirstEvent=false;
		CHATMGR->AddMsg(CTC_SYSMSG,CHATMGR->GetChatMsg(2662));
	}
	pDlg->SetActive(TRUE);

	m_pPlayTimeStatic->SetStaticText(Text);
}
void CEventPlayTimeDlg::Close()
{
	if(cDialog::IsActive())
		cDialog::SetActive(FALSE);
}