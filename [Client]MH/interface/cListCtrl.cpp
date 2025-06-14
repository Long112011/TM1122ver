#include "stdafx.h"
#include "cListCtrl.h"
#include "cFont.h"
#include "../Input/Mouse.h"
cListCtrl::cListCtrl()
{
	m_bHeadLineHeight=0;
	m_bBodyLineHeight=0;
	m_wLinePerPage = 0;
	SetRect(&m_textRelRect, 3,4,0,0);
	m_type = WT_LISTCTRL;
	m_SelectedRowIdx = -1;
	m_lTopListItemIdx = 0;
	m_OverImageScale.x = 1;
	m_OverImageScale.y = 1;
	m_nOverIdx = -1;
	m_fGap = 0.0f;
	m_wSelectOption = 0;
	m_nAlign=1;
	IsRenderImage=FALSE;
	IsRenderColumnHeader=FALSE;
	IsRenderTooltip=FALSE;
}
cListCtrl::~cListCtrl()
{
}
void cListCtrl::Init(LONG x, LONG y, WORD wid, WORD hei, cImage * basicImage, LONG ID)
{
	cWindow::Init(x,y,wid,hei,basicImage,ID);
}
void cListCtrl::InitListCtrl(WORD wMaxColumns, WORD wLinePerPage)
{
	m_wLinePerPage = wLinePerPage;
}
void cListCtrl::InitListCtrlImage(cImage * HeadImage, BYTE HHeight, cImage * BodyImage, BYTE BHeight, cImage * OverImage )
{
	m_HeadImage=*HeadImage;
	m_BodyImage=*BodyImage;
	m_bHeadLineHeight=HHeight;
	m_bBodyLineHeight=BHeight;
	m_OverImage = *OverImage;
	if( m_bBodyLineHeight > 10)
		m_fGap = ((float)m_bBodyLineHeight-10.0f)/2.0f;
}
void cListCtrl::Render()
{
	VECTOR2 m_tmpPos = m_absPos;
	float  headRowOffset=m_tmpPos.x;
	if(!IsRenderColumnHeader)
	{
	     m_HeadImage.RenderSprite(NULL,NULL,0,&m_tmpPos,RGBA_MERGE(m_dwImageRGB, m_alpha * m_dwOptionAlpha / 100));
	}
	else
	{
		for(int l = 0 ; l < m_Cols ; l++)
		{
			if(!PColumnHeaderImg[l].IsNull())  
			{
				VECTOR2 rectHeaderRow;
				rectHeaderRow.x=headRowOffset;
				rectHeaderRow.y=m_tmpPos.y-3.0f;
				PColumnHeaderImg[l].RenderSprite(NULL,NULL,0,&rectHeaderRow,RGBA_MERGE(m_dwImageRGB, m_alpha * m_dwOptionAlpha / 100));
			}
			headRowOffset=headRowOffset+pColumnSize[l];  
		}
	}
	m_tmpPos.y += m_bHeadLineHeight;
	for(int k = 0 ; k < m_wLinePerPage ; k++)
	{
		m_BodyImage.RenderSprite(NULL,NULL,0,&m_tmpPos,RGBA_MERGE(m_dwImageRGB, m_alpha * m_dwOptionAlpha / 100));
		m_tmpPos.y += m_bBodyLineHeight;
	}
	if( m_wSelectOption == 0 )
	{
		if( m_nOverIdx != -1 )
		{
			m_tmpPos.x = m_absPos.x;
			m_tmpPos.y = m_absPos.y + m_textRelRect.top + m_bHeadLineHeight + (((float)m_nOverIdx)*m_bBodyLineHeight ) - m_fGap;
			m_OverImage.RenderSprite(&m_OverImageScale,NULL,0,&m_tmpPos,RGBA_MERGE(m_dwImageRGB, m_alpha * m_dwOptionAlpha / 100));
		}
	}
	else if( m_wSelectOption == 1 )
	{
		if( m_nOverIdx != 0 )
		{
			m_tmpPos.x = m_absPos.x;
			m_tmpPos.y = m_absPos.y + m_textRelRect.top + m_bHeadLineHeight + (((float)m_SelectedRowIdx)*m_bBodyLineHeight ) - m_fGap;
			m_OverImage.RenderSprite(&m_OverImageScale,NULL,0,&m_tmpPos,RGBA_MERGE(m_dwImageRGB, m_alpha * m_dwOptionAlpha / 100));
		}
	}
	RECT headRect = {(LONG)m_absPos.x+m_textRelRect.left, (LONG)m_absPos.y+m_textRelRect.top,0,0};
	for(int l = 0 ; l < m_Cols ; l++)
	{
		headRect.right = headRect.left + 1;
		headRect.bottom = headRect.top + 1;
		if(m_nAlign & TXT_CENTER) 
		{
			RECT recttemp;
			recttemp.left= headRect.left+(pColumnSize[l]-CFONT_OBJ->GetTextExtentEx( m_wFontIdx, pszColumnHeader[l], strlen(pszColumnHeader[l])))/2;
			recttemp.right=headRect.right;
			recttemp.top=headRect.top-2.0f;
			recttemp.bottom=headRect.bottom;
			CFONT_OBJ->RenderFont(m_wFontIdx,pszColumnHeader[l],lstrlen(pszColumnHeader[l]),&recttemp,RGBA_MERGE(pColumnTextColor[l], m_alpha * m_dwOptionAlpha / 100));
		}
		if(m_nAlign & TXT_LEFT)
		{
			CFONT_OBJ->RenderFont(m_wFontIdx,pszColumnHeader[l],lstrlen(pszColumnHeader[l]),&headRect,RGBA_MERGE(pColumnTextColor[l], m_alpha * m_dwOptionAlpha / 100));
		}
		headRect.left += pColumnSize[l];
	}
	POS pos = m_pItemList->GetIndexPos(m_lTopListItemIdx);
	cRITEMEx * ritem = NULL;
	LONG i = 0;
	while (pos)
    {
        ritem = (cRITEMEx*) m_pItemList->GetNextPos(pos);   
		if(i < m_wLinePerPage)
		{
			RECT rect={(LONG)m_absPos.x+m_textRelRect.left, (LONG)m_absPos.y+m_bHeadLineHeight+m_textRelRect.top+(m_bBodyLineHeight*i), 0,0}; 
			for(int j = 0 ; j < m_Cols ; j++)
			{
				if(IsRenderImage &&  !ritem->ItemImg[j].IsNull() && j<MAX_IMGITEM_NUM) 
				{
					VECTOR2  m_scaling;
					VECTOR2  m_Pos;
					m_Pos.x=rect.left+(pColumnSize[j]-(ritem->ItemImg[j].GetImageSrcRect()->right-ritem->ItemImg[j].GetImageSrcRect()->left))/3; 
					m_Pos.y=rect.top-2;
					m_scaling.x=m_scaling.y=1;
					ritem->ItemImg[j].RenderSprite(&m_scaling,NULL, 0,&m_Pos,RGBA_MERGE(m_dwImageRGB, m_alpha * m_dwOptionAlpha / 100));
				}
				if(m_nAlign & TXT_CENTER) 
				{   
					if(pColumnSize[j]>CFONT_OBJ->GetTextExtentEx( m_wFontIdx, ritem->pString[j], strlen(ritem->pString[j])))
					{
						RECT recttemp;
						recttemp.left= rect.left+(pColumnSize[j]-CFONT_OBJ->GetTextExtentEx( m_wFontIdx, ritem->pString[j], strlen(ritem->pString[j])))/2;
						recttemp.right=rect.right;
						recttemp.top=rect.top;
						recttemp.bottom=rect.bottom;
						CFONT_OBJ->RenderFont(m_wFontIdx,ritem->pString[j],lstrlen(ritem->pString[j]),&recttemp,RGBA_MERGE(ritem->rgb[j], m_alpha * m_dwOptionAlpha / 100));
					} 
					else
					{
						CFONT_OBJ->RenderFont(m_wFontIdx,ritem->pString[j],lstrlen(ritem->pString[j]),&rect,RGBA_MERGE(ritem->rgb[j], m_alpha * m_dwOptionAlpha / 100));
					}
				}
				if(m_nAlign & TXT_LEFT)
				{
					CFONT_OBJ->RenderFont(m_wFontIdx,ritem->pString[j],lstrlen(ritem->pString[j]),&rect,RGBA_MERGE(ritem->rgb[j], m_alpha * m_dwOptionAlpha / 100));
				}
				rect.left += pColumnSize[j];
			}
			i++;
		}
		else
			break;
	}
	if(IsRenderTooltip)
	{
		if(m_nOverIdx!=-1)
		{
			cRITEMEx * ritemEx =(cRITEMEx*) m_pItemList->GetAt(m_nOverIdx);
			if(ritemEx)
			{
				ritemEx->Render();
			}
		}
	}
	cWindow::Render();
}
DWORD cListCtrl::ActionEvent(CMouse * mouseInfo)
{
	DWORD we = WE_NULL;
	if( !m_bActive ) return we;
	LONG x = mouseInfo->GetMouseX();
	LONG y = mouseInfo->GetMouseY();
	BOOL LeftDown = mouseInfo->LButtonDown();
	we = cWindow::ActionEvent(mouseInfo);
	if( we & WE_MOUSEOVER )
	{
		m_nOverIdx = PtIdxInRow(x, y);
		if( m_nOverIdx > m_wLinePerPage || m_pItemList->GetCount() <= m_nOverIdx )
		{
			m_nOverIdx = -1;
		}
	}
	else
	{
		m_nOverIdx = -1;
	}
	if( we & WE_LBTNCLICK )
	{
		LONG x = mouseInfo->GetMouseEventX();
		LONG y = mouseInfo->GetMouseEventY();
		int selIdx = PtIdxInRow(x, y);
		if(selIdx < m_wLinePerPage)
		{
			m_SelectedRowIdx = selIdx;
			cbWindowFunc(m_ID, m_pParent, WE_ROWCLICK);	
			we |= WE_ROWCLICK;
		}
		else
		{
			m_SelectedRowIdx = 0;
		}
	}
	else if( we & WE_LBTNDBLCLICK )
	{
		LONG x = mouseInfo->GetMouseEventX();
		LONG y = mouseInfo->GetMouseEventY();
		int selIdx = PtIdxInRow(x, y);
		if(selIdx < m_wLinePerPage)
		{
			m_SelectedRowIdx = selIdx;
			cbWindowFunc(m_ID, m_pParent, WE_ROWDBLCLICK);	
			we |= WE_ROWDBLCLICK;
		}
		else
		{
			m_SelectedRowIdx = 0;
		}
	}
	else if( we & WE_RBTNCLICK )
	{
		LONG x = mouseInfo->GetMouseEventX();
		LONG y = mouseInfo->GetMouseEventY();
		int selIdx = PtIdxInRow(x, y);
		if(selIdx < m_wLinePerPage)
		{
			m_SelectedRowIdx = selIdx;
		}
		else
		{
			m_SelectedRowIdx = 0;
		}
	}
	return we;
}
void cListCtrl::SetMargin(const RECT * textRelRect)
{
	CopyRect(&m_textRelRect, textRelRect);
}
WORD cListCtrl::PtIdxInRow(LONG x, LONG y)
{
	for(int i = 0 ; i < m_wLinePerPage ; i++)
	{
		if( m_absPos.x < x 
			&& m_absPos.y+m_bHeadLineHeight < y 
			&& x < m_absPos.x+m_width 
			&& y < m_absPos.y+m_bHeadLineHeight+(i+1)*m_bBodyLineHeight)
			return i;
	}
	return m_wLinePerPage+1;
}