#include "stdafx.h"
#include <string>   
#include "ctype.h"  
#include "cScriptManager.h"     
#include "cListDialog.h"
#include "cButton.h"
#include "cGuageBar.h"
#include "cFont.h"
#include "../Input/Mouse.h"
#include "MHTimeManager.h"
#include "GameIn.h"
#include "ChatManager.h"
#include "ObjectManager.h"
#include "WindowIDEnum.h"
#include "OptionDialog.h"
static const DWORD g_GlistenColor[2] = {0XCC3333,0xFF3333};
//COptionDialog * Odlg;
cListDialog::cListDialog() :
mToolTipIcon(0)
{
	m_lLineHeight		= LINE_HEIGHT;		
	m_lLineNum			= 0;
	m_lTopListItemIdx	= 0;
	memset(&m_textRelRect, 0, sizeof(RECT));
	m_minMiddleNum		= 0;
	m_maxMiddleNum		= 0;
	m_wMaxLine			= 50;
	m_middleNum			= 0;
	m_topHeight			= m_downHeight = m_middleHeight = 0;
	m_type				= WT_LISTDIALOG;
	m_pLengthLongBtn	= m_pLengthShortBtn = NULL;
	m_pUpwardBtn		= m_pDownwardBtn = NULL;
	m_pGuageBar			= NULL;
	m_SelectedRowIdx	= -1;
	m_ClickSelected		= -1;
	m_fGap				= 0.0f;
	m_bAutoScroll		= TRUE;
	m_bShowSelect		= FALSE;
	m_bFaceImage		= FALSE;  
	m_glistenCount		= 150;      
	m_glisttenTime		= 0;      
	SCRIPTMGR->GetImage(89 , &m_ItemLink, PFT_JACKPATH);   
	m_fgColor	= RGB_HALF(255,255,255);
	m_bFlicker2		= FALSE;
	m_bFlActive2	= FALSE;
	m_dwFlickerSwapTime2	= 0;
	count2=0;
	SetFlickerRoar(FALSE);
	m_glistenCount2		= 0;
	m_glisttenTime2		= 0;
	m_fgColor2 = RGB_HALF(255,0,0);

	memset(toolTipMsg, 0, sizeof(toolTipMsg));
	cImage ToolTipImg;
	SCRIPTMGR->GetImage(63, &ToolTipImg, PFT_HARDPATH);
	m_toolTip.Init(0, TTTC_DEFAULT, &ToolTipImg, TTCLR_ITEM_CANEQUIP);
	m_toolTip.SetImageAlpha(TOOLTIP_ALPHA);
}
cListDialog::~cListDialog()
{
	m_toolTip.Release();
}
void cListDialog::AddToolTipLine3(char* msg, DWORD color, cImage * m_LineImage, WORD m_FontIdx)
{
	m_toolTip.AddLine(msg, color, m_LineImage, m_FontIdx);
}
void cListDialog::Add(cWindow * btn)
{
	if(btn->GetType() == WT_BUTTON)
	{
		if(!m_pUpwardBtn)
		{
			m_pUpwardBtn = (cButton *)btn;
		}
		else if(!m_pDownwardBtn)
		{
			m_pDownwardBtn = (cButton *)btn;
		}
		else if(!m_pLengthLongBtn)
		{
			m_pLengthLongBtn = (cButton *)btn;
		}
		else if(!m_pLengthShortBtn)
		{
			m_pLengthShortBtn = (cButton *)btn;
		}
	}
	else if(btn->GetType() == WT_GUAGEBAR)
	{
		m_pGuageBar = (cGuageBar *)btn;
	}
	cDialog::Add(btn);
}
void cListDialog::Init(LONG x, LONG y, WORD wid, cImage * topImage, WORD topHei, cImage * middleImage, WORD middleHei, cImage * downImage, WORD downHei, cImage * overImage, BYTE num, LONG ID)
{
	m_TopImage		= *topImage; 
	m_MiddleImage	= *middleImage; 
	m_DownImage		= *downImage;
	m_OverImage		= *overImage;
	m_topHeight		= topHei; m_middleHeight = middleHei; m_downHeight = downHei;
	m_middleNum		= num;
	int hei			= topHei + downHei + ( num * middleHei );
	cDialog::Init(x,y,wid,hei,NULL,ID);
}
void cListDialog::InitList(WORD wMaxLine, RECT * textClippingRect)
{
	m_wMaxLine = wMaxLine;
	CopyRect(&m_textRelRect, textClippingRect);
	Resize( m_middleNum );
}
BOOL cListDialog::Resize(BYTE middleNum)
{
	if(middleNum > m_maxMiddleNum) return FALSE;
	if(middleNum < m_minMiddleNum) return FALSE;
	int var_len = (middleNum - m_middleNum)*m_middleHeight;
	m_middleNum = middleNum;
	m_textRelRect.bottom += var_len;
	m_height = m_topHeight+m_downHeight+(m_middleNum*m_middleHeight);
	m_lLineNum = (m_textRelRect.bottom-m_textRelRect.top)/m_lLineHeight;
	return TRUE;
}
BOOL cListDialog::IsMaxLineOver()
{
	if(m_lLineNum > GetItemCount())
		return FALSE;
	else
		return TRUE;
}
extern BOOL bHasil;
DWORD cListDialog::ActionEvent(CMouse * mouseInfo)
{
	DWORD we = WE_NULL;
	if( !m_bActive ) return we;
	if( IsMaxLineOver() )
	{	
		if(m_pUpwardBtn)
		{
			m_pUpwardBtn->SetActive(TRUE);
		}
		if(m_pDownwardBtn)
		{
			m_pDownwardBtn->SetActive(TRUE);
		}
		if(m_pGuageBar)
		{
			//if(Odlg)
			//	m_pGuageBar->SetActive(FALSE);
			//else
				m_pGuageBar->SetActive(TRUE);
			m_pGuageBar->ActionEvent(mouseInfo);
		}
	}
	else if( m_bAutoScroll )
	{
		if(m_pUpwardBtn)
		{
			m_pUpwardBtn->SetActive(FALSE);
		}
		if(m_pDownwardBtn)
		{
			m_pDownwardBtn->SetActive(FALSE);
		}
		if(m_pGuageBar)
		{
			m_pGuageBar->SetActive(FALSE);
		}
	}
	DWORD we2 = WE_NULL;
	if(m_pLengthLongBtn)
	{
		we2 = m_pLengthLongBtn->ActionEvent(mouseInfo);
		if(we2 & WE_BTNCLICK)
			onSize(TRUE);
	}
	if(m_pLengthShortBtn)
	{
		we2 = m_pLengthShortBtn->ActionEvent(mouseInfo);
		if(we2 & WE_BTNCLICK)
			onSize(FALSE);
	}
	if(m_pUpwardBtn)
	{
		we2 = m_pUpwardBtn->ActionEvent(mouseInfo);
		if(we2 & WE_BTNCLICK)
			OnUpwardItem();
	}
	if(m_pDownwardBtn)
	{
		we2 = m_pDownwardBtn->ActionEvent(mouseInfo);
		if(we2 & WE_BTNCLICK)
			OnDownwardItem();
	}
	LONG cnt = GetItemCount();
	int itemCnt = cnt - m_lLineNum;
	if(itemCnt > 0)
	{
		//m_pGuageBar->SetActive(TRUE);
		//if(Odlg)
		//if(m_pGuageBar)
		//{
		//	m_pGuageBar->SetActive(TRUE);
		//}
		if(m_pGuageBar)
		if(m_pGuageBar->IsDrag() )
		{
			m_lTopListItemIdx = (LONG)(((float)itemCnt)*m_pGuageBar->GetCurRate());
		}
	}
	else
	{
		//if(Odlg)
		//if(m_pGuageBar)
		//{
		//	m_pGuageBar->SetActive(FALSE);
		//}
		//m_pGuageBar->SetActive(FALSE);
		m_lTopListItemIdx = 0;
	}
		
	we |= cDialog::ActionEvent(mouseInfo);
	if(we && PtInWindow((LONG)mouseInfo->GetMouseX(), (LONG)mouseInfo->GetMouseY()) )
	{		
		int Wheel = mouseInfo->GetWheel();
		if(Wheel)
		{
			bHasil=true;
			if( Wheel > 0 )
			{
				OnUpwardItem();
			}
			else
			{
				OnDownwardItem();
			}
		}
	}
	else
		bHasil=false;

	ListMouseCheck( mouseInfo );
	return we;
}
BOOL cListDialog::PtInWindow(LONG x, LONG y)
{
	if( !m_bActive ) return FALSE;
	WORD height = m_topHeight + m_middleNum * m_middleHeight; 
	if(m_absPos.x > x || m_absPos.y > y || m_absPos.x+m_width < x || m_absPos.y+height < y)
		return FALSE;
	else
		return TRUE;
}
#include "cWindowManager.h"
void cListDialog::ListMouseCheck( CMouse * mouseInfo )
{
	//LONG x = mouseInfo->GetMouseX();
	//LONG y = mouseInfo->GetMouseY();
	//int selIdx = PtIdxInRow(x, y);
	const int selIdx = PtIdxInRow(mouseInfo->GetMouseX(), mouseInfo->GetMouseY());

	mToolTipIcon = 0;

	if( selIdx < m_lLineNum && selIdx != -1)
	{
		m_SelectedRowIdx = selIdx;

		PTRLISTPOS pos = m_ListItem.FindIndex(selIdx + m_lTopListItemIdx);
		if (pos == NULL) return;

		const ITEM* item = (ITEM*)m_ListItem.GetAt(pos);

		if (item)
		{
			if (mouseInfo->LButtonDown() &&
				!IsDisable() &&
				!WINDOWMGR->IsMouseDownUsed())
			{
				m_ClickSelected = selIdx + m_lTopListItemIdx;

				cbWindowFunc(m_ID, m_pParent, WE_ROWCLICK);

				//if (m_pComboBtn)
				//{
				//	Resize(1);
				//	SetShowScroll(FALSE);
				//	m_lTopListItemIdx = m_ClickSelected;
				//	m_pComboBtn->SetActive(TRUE);
				//}
			}

			for (
				ToolTipList::iterator it = mToolTipList.begin();
				mToolTipList.end() != it;
			++it)
			{
				ToolTip& toolTip = *it;

				if (toolTip.nPos == selIdx + m_lTopListItemIdx)
				{
					mToolTipIcon = toolTip.mIcon;
					mToolTipIcon->SetAbsXY(mouseInfo->GetMouseX(), mouseInfo->GetMouseY());
					break;
				}
			}
		}
		//if (mouseInfo->LButtonDown() || mouseInfo->LButtonDoubleClick())
		//{
			//ITEM* item = NULL;
			//LONG i = 0;
			//m_ClickSelected = selIdx + m_lTopListItemIdx;
			//PTRLISTPOS pos = m_ListItem.FindIndex(m_lTopListItemIdx + m_SelectedRowIdx);
			//if (pos == NULL) return;
			//item = (ITEM*)m_ListItem.GetAt(pos);
			//if (item)
			//{
			//	cbWindowFunc(m_ID, m_pParent, WE_ROWCLICK);
			//}
		//}		
	}
	else
	{
		m_SelectedRowIdx = -1;
	}
}
void cListDialog::CheckScrollbar()
{
	LONG cnt = GetItemCount();
	int itemCnt = cnt - m_lLineNum;
	if(itemCnt > 0)
	{
		//m_pGuageBar->SetActive(TRUE);
		//if(Odlg)
		if(m_pGuageBar)
		{
			m_pGuageBar->SetActive(TRUE);
		}
		/*if(m_pGuageBar)
		if(m_pGuageBar->IsDrag() )
		{
			m_lTopListItemIdx = (LONG)(((float)itemCnt)*m_pGuageBar->GetCurRate());
		}*/
	}
	else
	{
		//if(Odlg)
		if(m_pGuageBar)
		{
			m_pGuageBar->SetActive(FALSE);
		}
		//m_pGuageBar->SetActive(FALSE);
		//m_lTopListItemIdx = 0;
	}
}
#include "./Input/UserInput.h"
#include "./Interface/cWindowManager.h"
void cListDialog::Render()
{
	if(!m_bActive) return;
	cDialog::RenderWindow();			
	cDialog::ToolTipRender();
	FlickerRoar();
	static VECTOR2 m_tmpPos;
	m_TopImage.RenderSprite(NULL,NULL,0,&m_absPos,RGBA_MERGE(m_dwImageRGB, m_alpha * m_dwOptionAlpha / 100));
	m_tmpPos.x = m_absPos.x;
	m_tmpPos.y = m_absPos.y+m_topHeight;
	for(int j = 0 ; j < m_middleNum ; j++)
	{
		m_MiddleImage.RenderSprite(NULL,NULL,0,&m_tmpPos,RGBA_MERGE(m_dwImageRGB, m_alpha * m_dwOptionAlpha / 100));
		m_tmpPos.y += m_middleHeight;
	}
		m_DownImage.RenderSprite(NULL,NULL,0,&m_tmpPos,RGBA_MERGE(m_dwImageRGB, m_alpha * m_dwOptionAlpha / 100));
	if( m_SelectedRowIdx != -1 )
	{
		m_tmpPos.x = m_absPos.x + 2;
		m_tmpPos.y = m_absPos.y + m_textRelRect.top + (((float)m_SelectedRowIdx)*m_lLineHeight) - m_fGap;
		m_OverImage.RenderSprite(&m_OverImageScale,NULL,0,&m_tmpPos,RGBA_MERGE(m_dwImageRGB, m_alpha * m_dwOptionAlpha / 100));
	}
	PTRLISTPOS pos = m_ListItem.FindIndex( m_lTopListItemIdx );
	ITEM* item = NULL;
	LONG i = 0;
	int bline = -1;
	int blinelen = 0;
	int line = -1;
	int linelen = 0;
	DWORD dwFontColor = 0xffffffff;//0xff000000;

	while(pos)
	{
		item = (ITEM*)m_ListItem.GetNext(pos);
		if(line < m_lLineNum-1)
		{
			if(CFONT_OBJ->IsCreateFont(m_wFontIdx))
			{
				if(item->line != -1 && item->line == bline)
				{					
					linelen += blinelen;
					blinelen = strlen(item->string)*CFONT_OBJ->GetTextWidth(m_wFontIdx);
				}
				else if(item->line != -1 && item->line != bline)
				{
					linelen = 0;
					++line;
					bline = item->line;
					blinelen = strlen(item->string)*CFONT_OBJ->GetTextWidth(m_wFontIdx);
				}
				else
				{					
					linelen = 0;
					++line;
				}
				RECT rect=
				{
					(LONG)m_absPos.x + m_textRelRect.left + linelen,
					(LONG)m_absPos.y + m_textRelRect.top + (m_lLineHeight*line),
					1, 
					1 
				};
				if( (m_ClickSelected-m_lTopListItemIdx) == line && (m_bShowSelect == TRUE))
				{
					if(m_bFaceImage)
					{
						char strBuf2[256];
						memset(strBuf2,0,256);
						memcpy(strBuf2,item->string,lstrlen(item->string));
						RenderFaceImage
						(
							(LONG)m_absPos.x + m_textRelRect.left + linelen,
							(LONG)m_absPos.y + m_textRelRect.top + (m_lLineHeight*line),
							strBuf2
						);
						ClearFaceImg(strBuf2,lstrlen(item->string));
						CFONT_OBJ->RenderFont(m_wFontIdx,strBuf2,lstrlen(item->string),&rect,RGBA_MERGE(0xffffff00, m_alpha * m_dwOptionAlpha / 100));
					}
					else
					{
						if (item->IsMenyerong)
							CFONT_OBJ->RenderFont(m_wFontIdx,item->string,lstrlen(item->string),&rect,RGBA_MERGE(0xffffff00, m_alpha * m_dwOptionAlpha / 100));
						else
							CFONT_OBJ->RenderFont(m_wFontIdx, item->string, lstrlen(item->string), &rect, RGBA_MERGE(0xffffff00, m_alpha * m_dwOptionAlpha / 100));
					}

					/*if (m_toolTip.IsValid())
					{
						int mx = g_UserInput.GetMouse()->GetMouseX();
						int my = g_UserInput.GetMouse()->GetMouseY();
						if (WINDOWMGR->IsMouseOverUsed())
						{
							m_toolTip.SetXY(mx + TOOLTIP_MOUSESIZE_X, my + TOOLTIP_MOUSESIZE_Y);
							WINDOWMGR->SetToolTipWindow(&m_toolTip);
						}
					}*/
				}
				else
				{
					if(item->Isglisten) 
					{	
						if(m_bFaceImage)
						{
							char strBuf2[256];
							memset(strBuf2,0,256);
							memcpy(strBuf2,item->string,lstrlen(item->string));
							RenderFaceImage((LONG)m_absPos.x+m_textRelRect.left+linelen,(LONG)m_absPos.y+m_textRelRect.top + (m_lLineHeight*line),strBuf2);  
							ClearFaceImg(strBuf2,lstrlen(item->string));
							CFONT_OBJ->RenderNoticeMsg(m_wFontIdx,strBuf2,lstrlen(item->string),&rect,RGBA_MERGE( RGB_HALF(255 ,255 ,255), 255 ),RGBA_MERGE(m_fgColor,255));
						}
						else
						{
							CFONT_OBJ->RenderFont(m_wFontIdx,item->string,lstrlen(item->string),&rect,m_fgColor);
						}
					}
					else
					{
						if(m_bFaceImage)
						{
							char strBuf2[256];
							memset(strBuf2,0,256);
							memcpy(strBuf2,item->string,lstrlen(item->string));
							RenderFaceImage((LONG)m_absPos.x+m_textRelRect.left+linelen,(LONG)m_absPos.y+m_textRelRect.top + (m_lLineHeight*line),strBuf2);  
							ClearFaceImg(strBuf2,lstrlen(item->string));
							CFONT_OBJ->RenderFont(m_wFontIdx,strBuf2,lstrlen(item->string),&rect,RGBA_MERGE(item->rgb, m_alpha * m_dwOptionAlpha / 100));
						}
						else
						{
							CFONT_OBJ->RenderFont(m_wFontIdx,item->string,lstrlen(item->string),&rect,RGBA_MERGE(item->rgb, m_alpha * m_dwOptionAlpha / 100));
						}
					}
					if(item->Value!=0)   
					{
						if(!m_ItemLink.IsNull())
						{
							VECTOR2 p_Scal,p_Pos;
							p_Scal.x=p_Scal.y=1.0;
							p_Pos.x= rect.left+(float)0.8f+ strlen(item->string)*CFONT_OBJ->GetTextWidth(m_wFontIdx);
							p_Pos.y= rect.top-2;
							m_ItemLink.RenderSprite(&p_Scal,NULL,NULL,&p_Pos,0xffffffff);
						}
					}
				}
			}
		}
		else
			break;
	}
	if (mToolTipIcon)
	{
		mToolTipIcon->Render();
		mToolTipIcon->ToolTipForcedRender();
	}
	cDialog::RenderComponent();			
}
void cListDialog::onSize(BOOL fSize)
{
	if(fSize)
	{
		if(!Resize(m_middleNum+1)) return;
		SetAbsY((LONG)(GetAbsY()-m_middleHeight));
		SetHeight(cWindow::GetHeight()+m_middleHeight);
		m_pGuageBar->SetAbsY((LONG)(m_pGuageBar->GetAbsY()-m_middleHeight));
		m_pGuageBar->SetHeight(m_pGuageBar->GetHeight()+m_middleHeight);
		m_pGuageBar->SetInterval(m_pGuageBar->GetInterval()+m_middleHeight);
		m_pUpwardBtn->SetAbsY((LONG)(m_pUpwardBtn->GetAbsY()-m_middleHeight));
		OnUpwardItem();	
	}
	else
	{
		if(!Resize(m_middleNum-1)) return;
		SetAbsY((LONG)((GetAbsY()+m_middleHeight)));
		SetHeight(cWindow::GetHeight()-m_middleHeight);
		m_pGuageBar->SetAbsY((LONG)(m_pGuageBar->GetAbsY()+m_middleHeight));
		m_pGuageBar->SetHeight(m_pGuageBar->GetHeight()-m_middleHeight);
		m_pGuageBar->SetInterval(m_pGuageBar->GetInterval()-m_middleHeight);
		m_pUpwardBtn->SetAbsY((LONG)(m_pUpwardBtn->GetAbsY()+m_middleHeight));
		OnDownwardItem();	
	}
}
void cListDialog::OnUpwardItem() 
{ 
	LONG cnt = GetItemCount();
	if(cnt < m_lLineNum) return;
	if(m_lTopListItemIdx < 1) return;
	m_lTopListItemIdx--;
	if( m_pGuageBar )
	m_pGuageBar->SetCurRate( (float)m_lTopListItemIdx/(float)(cnt-m_lLineNum) );
}
void cListDialog::OnDownwardItem() 
{ 
	LONG cnt = GetItemCount();
	if(cnt < m_lLineNum) return;
	if((cnt-m_lTopListItemIdx-1) < m_lLineNum) return;
	m_lTopListItemIdx++;
	if( m_pGuageBar )
	m_pGuageBar->SetCurRate( (float)m_lTopListItemIdx/(float)(cnt-m_lLineNum) );
}
void cListDialog::AddItem(const char * str, DWORD color, int line, BOOL Isglisten, DWORD Value,int GeneratedId)
{
	ITEM* pItem = new ITEM;
	strcpy( pItem->string, str);
	pItem->rgb = color;
	pItem->line = line;
	pItem->Isglisten=Isglisten;
	pItem->Value=Value;
	if (line==-1)
		cListItem::AddItem(pItem);
	else
		cListItem::AddItem(pItem, line);
	if( m_pGuageBar )
	if( m_lTopListItemIdx == GetItemCount() - m_lLineNum -1 )
	{
		m_pGuageBar->SetCurValue(m_pGuageBar->GetMaxValue());
		m_lTopListItemIdx = GetItemCount() - m_lLineNum;
	}
}
void cListDialog::AddItemRecall(const char * str, DWORD color, int line, BOOL Recall)
{
	ITEM* pItem = new ITEM;
	strcpy(pItem->string, str);
	pItem->rgb = color;
	pItem->line = line;
	pItem->Isglisten = FALSE;
	pItem->Value = 0;
	pItem->IsMenyerong = Recall;
	if (line == -1)
		cListItem::AddItem(pItem);
	else
		cListItem::AddItem(pItem, line);
	if (m_pGuageBar)
	if (m_lTopListItemIdx == GetItemCount() - m_lLineNum - 1)
	{
		m_pGuageBar->SetCurValue(m_pGuageBar->GetMaxValue());
		m_lTopListItemIdx = GetItemCount() - m_lLineNum;
	}
}
void cListDialog::SetTopListItemIdx(int Idx)
{
	m_lTopListItemIdx=Idx;
	LONG cnt = GetItemCount();
	if( m_pGuageBar )
	{
		float rate = (float)m_lTopListItemIdx/(float)(cnt-m_lLineNum);
		if(rate > 1.f)		rate = 1.f;
		else if(rate< 0.f)	rate = 0.f;
		m_pGuageBar->SetCurRate( rate );		
	}	
}
int cListDialog::PtIdxInRow(LONG x, LONG y)
{
	LONG cnt = GetItemCount();
	for(int i = 0 ; i < cnt ; i++)
	{
		if( m_absPos.x < x 
			&& m_absPos.y < y
			&& x < m_absPos.x+m_width 
			&& y < m_absPos.y+m_textRelRect.top+(i+1)*m_lLineHeight
			&& y < m_absPos.y+m_textRelRect.bottom )	
			return i;
	}
	return -1;
}
int cListDialog::GetCurSelectedRowIdx()
{
	if( m_SelectedRowIdx == -1 ) 
		return m_SelectedRowIdx;
	return m_SelectedRowIdx+m_lTopListItemIdx;
}
void cListDialog::SetCurSelectedRowIdx(int Idx)
{
	if(Idx < 0)
	{
		SetTopListItemIdx(0);
		m_SelectedRowIdx = -1;
		m_ClickSelected = -1;
	}
	else
	{
		if(Idx <= GetItemCount() - m_lLineNum)
		{
			SetTopListItemIdx(Idx);
			m_SelectedRowIdx = 0;
			m_ClickSelected = Idx;
		}
		else
		{
			SetTopListItemIdx(GetItemCount() - m_lLineNum);
			m_SelectedRowIdx = Idx - GetItemCount();
			m_ClickSelected = Idx;
		}
	}
}
void cListDialog::ResetGuageBarPos()
{ 
	if( m_pGuageBar )
	{
		m_pGuageBar->SetCurValue(0); 
		int itemCnt = GetItemCount() - m_lLineNum;
		if(itemCnt > 0)
			m_lTopListItemIdx = (LONG)(((float)itemCnt)*m_pGuageBar->GetCurRate());
		else
			m_lTopListItemIdx = 0;
	}
	m_SelectedRowIdx = -1;
	m_ClickSelected = -1;
}
void cListDialog::SetLineHeight( LONG height )
{ 
	m_lLineHeight = height; 
	if( height > 10 )
		m_fGap = ((float)height - 10.0f)/2.0f; 
	else
		m_fGap = 0.0f;
}
void cListDialog::SetShowSelect(BOOL val)
{
	m_bShowSelect = val;
}
ITEM* cListDialog::GetSelectedITEM( CMouse * mouseInfo )
{
	LONG x = mouseInfo->GetMouseX();
	LONG y = mouseInfo->GetMouseY();
	int selIdx = PtIdxInRow(x, y);
	if( selIdx < m_lLineHeight && selIdx != -1 )
	{
		ITEM* pItem = NULL;
		PTRLISTPOS pos = m_ListItem.FindIndex( m_lTopListItemIdx + selIdx );
		if( NULL == pos )
		{
			return NULL;
		}
		pItem = (ITEM*)m_ListItem.GetAt(pos);
		if( pItem )
		{
			return pItem;
		}
	}
	return NULL;
}
ITEM* cListDialog::GetSelectedITEM(int IdxRow) 
{
	int selIdx = IdxRow;
	if( selIdx < m_lLineHeight && selIdx != -1 )
	{
		ITEM* pItem = NULL;
		PTRLISTPOS pos = m_ListItem.FindIndex( m_lTopListItemIdx + selIdx );
		if( NULL == pos )
		{
			return NULL;
		}
		pItem = (ITEM*)m_ListItem.GetAt(pos);
		if( pItem )
		{
			return pItem;
		}
	}
	return NULL;
}
void cListDialog::RenderFaceImage(LONG X,LONG Y,char * StaticText)
{
	std::string Buffer=StaticText;
	if(Buffer.length()>0)
	{
		int FindStartPos=0;
		while(FindStartPos>=0 && FindStartPos< Buffer.length())
		{
			int FindPos= Buffer.find('&',FindStartPos);
			if(FindPos>=0)
			{
				std::basic_string <char> ImgStr = Buffer.substr(FindPos+1,2);
				std::basic_string <char> FlagStr= Buffer.substr(0,FindPos);
				int ImageIdx= atoi(ImgStr.c_str());
				if(ImageIdx>=0 && ImageIdx<70)
				{
					if(SCRIPTMGR->GetEmoji(ImageIdx)->ImageCount>1)
					{
						int  Rate=50;
						DWORD Cur_glistenTime= GetTickCount();
						if(Cur_glistenTime-m_glisttenTime2>0)
						{
							if(m_glistenCount2>= SCRIPTMGR->GetEmoji(ImageIdx)->ImageCount*Rate-5)
							{
								m_glistenCount2=0;
							}
							else
							{
								m_glistenCount2=m_glistenCount2+5;
								m_glisttenTime2=GetTickCount();
							}
							Cur_glistenTime=0;
						}
						int Index= m_glistenCount2/Rate;  
						SCRIPTMGR->GetImage(SCRIPTMGR->GetEmoji(ImageIdx)->ImageIdx,&pNameImage[Index],&SCRIPTMGR->GetEmoji(ImageIdx)->rect[Index]);
						if(!pNameImage[Index].IsNull())
						{							
							VECTOR2 p_Scal,p_Pos;
							p_Scal.x=p_Scal.y=0.9f;
							p_Pos.x = X+1.0f+(FlagStr.length())*6;
							p_Pos.y = Y-2.6f;
							
							pNameImage[Index].RenderSprite(0,NULL,0,&p_Pos,0xffffffff);
						}
					}
					else
					{
						if(SCRIPTMGR->GetEmoji(ImageIdx))
						{				
							int Index= 0; 
							SCRIPTMGR->GetImage(SCRIPTMGR->GetEmoji(ImageIdx)->ImageIdx,&pNameImage[Index],&SCRIPTMGR->GetEmoji(ImageIdx)->rect[Index]);
							if(!pNameImage[Index].IsNull())
							{							
								VECTOR2 p_Scal,p_Pos;
								p_Scal.x=p_Scal.y=0.9f;
								p_Pos.x = X+1.0f+(FlagStr.length())*6;
								p_Pos.y = Y-2.6f;
								
								pNameImage[Index].RenderSprite(0,NULL,0,&p_Pos,0xffffffff);
							}
						}
					}
				}
                FindStartPos = FindPos+2;
			}
			else
			{
				break;
			}
		}
	}
}
void cListDialog::SetRenderFaceImg(BOOL val)
{
	m_bFaceImage=val; 
}
void cListDialog::ClearFaceImg(char * StaticText,int StrLen)
{
    std::string Buffer=StaticText;
	if(Buffer.length()>0)
	{
		int FindStartPos=0;
		while(FindStartPos>=0 && FindStartPos< Buffer.length())
		{
			int FindPos= Buffer.find('&',FindStartPos);
			//int FindPos4= Buffer.find('}',FindStartPos);
			if(FindPos>=0)
			{
				Buffer.replace(FindPos,3,"   ");
				FindStartPos = FindPos+2;
			}
			/*if(FindPos4>=0)
			{
				Buffer.replace(FindPos4,3,"   ");
				FindStartPos = FindPos4+2;
			}*/
			else
			{
				break;
			}
		}
	}
	memset(StaticText,0,StrLen);
	memcpy(StaticText,Buffer.c_str(),StrLen);
}
void cListDialog::SetFlickerRoar( BOOL bFlicker )
{
	m_bFlicker2 = bFlicker;
	m_dwFlickerSwapTime2 = gCurTime;
	m_glisttenTime = gCurTime;
	count2=0;
}
void cListDialog::FlickerRoar()
{
	if(!m_bFlicker2)	return;
	if( gCurTime - m_dwFlickerSwapTime2 > 22 )
	{	
		if(m_glistenCount>=255)
		{
			m_glistenCount=180;
			if(count2>=400)
			{
				SetFlickerRoar( FALSE );
				m_fgColor = g_GlistenColor[0];
			}
		}
		else
		{
			count2=count2+1;
			m_fgColor = RGBA_MERGE(RGB_HALF(m_glistenCount,0,0),255);
			m_glistenCount=m_glistenCount+6;
		}
		m_bFlActive2 = !m_bFlActive2;
		m_dwFlickerSwapTime2 = gCurTime;
	}
}









//-------------------------------------------------------------------------------------------
/*void cListDialog::RemoveAll()
{
	cListItem::RemoveAll();

	m_ClickSelected = -1;
	m_SelectedRowIdx = -1;

	mToolTipList.clear();

	{
		for (
			ToolTipList::iterator it = mToolTipList.begin();
			mToolTipList.end() != it;
		++it)
		{
			const ToolTip& toolTip = *it;

			WINDOWMGR->DeleteWindow(toolTip.mIcon);
		}

		mToolTipIcon = 0;
	}
}
*/

/*void cListDialog::RemoveItem(DWORD index)
{
	cListItem::RemoveItem(index);

	for (
		ToolTipList::iterator it = mToolTipList.begin();
		mToolTipList.end() != it;
	++it)
	{
		const ToolTip& toolTip = *it;

		if (toolTip.nPos == (int)index)
		{
			if (mToolTipIcon == toolTip.mIcon)
			{
				mToolTipIcon = 0;
			}

			WINDOWMGR->DeleteWindow(toolTip.mIcon);

			mToolTipList.erase(it);
			break;
		}
	}
}*/
/*
void cListDialog::AddItem(const char* text, DWORD color, const cMultiLineText& tipTextList, int line, DWORD dwData, WORD wImageIdx)
{
	AddItem(text, (DWORD)color, (int)line, (BOOL)dwData);

	//if (!tipTextList.empty())
	{
		ToolTip toolTip;
		toolTip.nPos = m_ListItem.GetCount() - 1;

		toolTip.mIcon = new cIcon;
		{
			cImage image;
			SCRIPTMGR->GetImage(wImageIdx, &image, PFT_HARDPATH);
			toolTip.mIcon->SetID(GD_MEMBER_JOB_TOOLTIP);
			toolTip.mIcon->SetToolTip("", RGB_HALF(255, 255, 255), &image);

		
		}

		WINDOWMGR->AddWindow(toolTip.mIcon);

		mToolTipList.push_back(toolTip);
	}
}*/
/*void cListDialog::RemoveItem(const char* text)
{
	int nIndex = cListItem::RemoveItem((char*)text);
	if (-1 < nIndex)
	{
		for (
			ToolTipList::iterator it = mToolTipList.begin();
			mToolTipList.end() != it;
		++it)
		{
			const ToolTip& toolTip = *it;

			if (toolTip.nPos == nIndex)
			{
				if (mToolTipIcon == toolTip.mIcon)
				{
					mToolTipIcon = 0;
				}

				WINDOWMGR->DeleteWindow(toolTip.mIcon);

				mToolTipList.erase(it);
				break;
			}
		}
	}
}*/