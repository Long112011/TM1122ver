








// cIconGridDialog.cpp: implementation of the cIconGridDialog class.

//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "cIconGridDialog.h"
#include "cWindowManager.h"
#include "cIcon.h"
#include "../Input/Mouse.h"
#include "cScriptManager.h"
#include "./Input/UserInput.h"
#include "cFont.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
extern BOOL m_DebugEnabled;
extern char* WINIDSEARCH(int b);
cIconGridDialog::cIconGridDialog()
{
	m_nRow=m_nCol=0;

	m_IconDlgID=0;
	m_wCellBorderX=0;
	m_wCellBorderY=0;
	m_wCellWidth=0;
	m_wCellHeight=0;
	m_acceptableIconType=0xffffffff;			//all accept
	m_type = WT_ICONGRIDDIALOG;
	SetRect(&m_cellRect,0,0,0,0);
	m_lCurSelCellPos = -1;
	m_lCurDragOverPos = -1;
	m_nIconType	= WT_ICON;
	m_bItemDraged = FALSE;

	m_pIconGridCell = NULL;
	m_bShowGrid = FALSE;

#ifdef _JAPAN_LOCAL_
	m_DisableFromPos	= 0;
	m_DisableToPos		= 0;
#endif	// _JAPAN_LOCAL

#ifdef _HK_LOCAL_
	m_DisableFromPos	= 0;
	m_DisableToPos		= 0;
#endif	// _HK_LOCAL

#ifdef _KOR_LOCAL_
	m_DisableFromPos	= 0;
	m_DisableToPos		= 0;
#endif	// _HK_LOCAL



	*m_szTextDebug = 0;
	m_SavedXY.x = 0;
	m_SavedXY.y = 0;
	m_SavedID = 0;
	m_DebugBoolean[0] = FALSE;
	m_DebugBoolean[1] = FALSE;
	m_DebugBoolean[2] = FALSE;
	m_DebugBoolean[3] = FALSE;
	m_SavedWidthHeight[0] = 0;
	m_SavedWidthHeight[1] = 0;
}

cIconGridDialog::~cIconGridDialog()
{
	SAFE_DELETE_ARRAY(m_pIconGridCell);
}

void cIconGridDialog::InitDialog(LONG x, LONG y, WORD wid, WORD hei, cImage * basicImage, WORD col, WORD row, LONG ID)
{
	cDialog::Init(x,y,wid,hei,basicImage,ID);
	m_nRow = row;
	m_nCol = col;

	m_pIconGridCell = new cIconGridCell[m_nRow*m_nCol];
	for(int i = 0 ; i < m_nRow*m_nCol ; i++)
	{
		m_pIconGridCell[i].icon = NULL;
		m_pIconGridCell[i].use = NOTUSE;
	}
	

	
	m_wCellBorderX = DEFAULT_CELLBORDER;
	m_wCellBorderY = DEFAULT_CELLBORDER;
	m_wCellWidth =   DEFAULT_CELLSIZE;
	m_wCellHeight =  DEFAULT_CELLSIZE;
	
	SCRIPTMGR->GetImage( 137, &m_GridLockImage, PFT_HARDPATH );

	//if (m_DebugEnabled)
	{
		m_SavedXY.x = x;
		m_SavedXY.y = y;
		m_SavedID = ID;
		m_SavedWidthHeight[0] = wid;
		m_SavedWidthHeight[1] = hei;
		strncpy(m_szTextDebug, Vector2Text(m_SavedXY.x, m_SavedXY.y,  m_SavedWidthHeight[0], m_SavedWidthHeight[1]), strlen(Vector2Text(m_SavedXY.x, m_SavedXY.y,  m_SavedWidthHeight[0], m_SavedWidthHeight[1])) + 1);
	}
}
void cIconGridDialog::DebugPressed()
{
	m_DebugBoolean[DebugKDBActivated] = TRUE; m_DebugTimerKDBActivated = gCurTime;
}
void cIconGridDialog::InitGrid(LONG grid_x, LONG grid_y, WORD cellWid, WORD cellHei, WORD borderX, WORD borderY)
{
	m_gridPos.x = (float)grid_x;
	m_gridPos.y = (float)grid_y;
	m_wCellWidth = cellWid;

	m_wCellHeight = cellHei;
	m_wCellBorderX = borderX;
	m_wCellBorderY = borderY;

	m_cellRect.left = (LONG)m_gridPos.x;
	m_cellRect.right = m_cellRect.left + m_nCol*m_wCellWidth+m_wCellBorderX*(m_nCol+1);
	m_cellRect.top = (LONG)m_gridPos.y;
	m_cellRect.bottom = m_cellRect.top + m_nRow*m_wCellHeight+m_wCellBorderY*(m_nRow+1);
}

BOOL cIconGridDialog::PtInCell(LONG x, LONG y)
{
	/*
	// this is not precise calc
	// must be modified at last
	if(m_absPos.x+m_cellRect.left > x || m_absPos.y+m_cellRect.top > y || m_absPos.x+m_cellRect.right < x || m_absPos.y+m_cellRect.bottom < y)
		return FALSE;
	else
		return TRUE;
	*/
	for(int i = 0 ; i < m_nCol*m_nRow ; i++)
		if(m_pIconGridCell[i].use == USE && m_pIconGridCell[i].icon && m_pIconGridCell[i].icon->IsDepend())
		{
			if(m_pIconGridCell[i].icon->PtInWindow(x,y))
				return TRUE;
		}
	return FALSE;
}

void cIconGridDialog::SetCellRect(RECT * rect)

{
	CopyRect(&m_cellRect, rect); 
}

void cIconGridDialog::SetAbsXY(LONG x, LONG y)
{
	LONG tmpX = x - (LONG)m_absPos.x;
	LONG tmpY = y - (LONG)m_absPos.y;

	for(int i = 0 ; i < m_nCol*m_nRow ; i++)
		if(m_pIconGridCell[i].use == USE && m_pIconGridCell[i].icon && m_pIconGridCell[i].icon->IsDepend())
		{
			m_pIconGridCell[i].icon->SetAbsXY(tmpX+(LONG)m_pIconGridCell[i].icon->GetAbsX(), 
											  tmpY+(LONG)m_pIconGridCell[i].icon->GetAbsY());
		}
	cDialog::SetAbsXY(x,y);

}

DWORD cIconGridDialog::ActionEvent(CMouse * mouseInfo)
{
	if(!m_bActive) return WE_NULL;
	DWORD we = WE_NULL;
	DWORD we2 = WE_NULL;
	BOOL selCell = FALSE;
	POSTYPE pos;


	we = cDialog::ActionEvent(mouseInfo);
	
	if( !m_bDisable )
	{
		if (m_DebugEnabled)
		{
			if (we & (WE_LBTNCLICK | WE_LBTNDBLCLICK))
			{
				m_DebugBoolean[DebugIsStillDown] = TRUE;
				//return we;
			}		
			if (mouseInfo->LButtonUp())
			{			
				m_DebugBoolean[DebugIsStillDown] = FALSE;
				//return we;
			}
			if (mouseInfo->LButtonPressed())
			{			
				if (m_DebugBoolean[DebugClicked])
				if (m_DebugBoolean[DebugIsStillDown])
				if (!WINDOWMGR->IsDragWindow())
				{			
					SetAbsXY(m_absPos.x - MOUSE->GetMouseAxisX(), m_absPos.y - MOUSE->GetMouseAxisY());
					m_SavedXY.x = m_SavedXY.x - MOUSE->GetMouseAxisX();
					m_SavedXY.y = m_SavedXY.y - MOUSE->GetMouseAxisY();
					SetRelXY(m_SavedXY.x, m_SavedXY.y);
					strncpy(m_szTextDebug, Vector2Text(m_SavedXY.x, m_SavedXY.y,  m_SavedWidthHeight[0], m_SavedWidthHeight[1]), strlen(Vector2Text(m_SavedXY.x, m_SavedXY.y,  m_SavedWidthHeight[0], m_SavedWidthHeight[1])) + 1);				
				}
				//return we;
			}
			if (we & WE_MOUSEOVER)
			{
				m_DebugBoolean[DebugHover] = TRUE;
				if (g_UserInput.GetKeyboard()->GetKeyPressed(KEY_CONTROL) && mouseInfo->LButtonDown())
				{
					if (m_DebugBoolean[DebugClicked])
					{
						m_DebugBoolean[DebugClicked] = FALSE;
						return we;
					}
					else
					{
						m_DebugBoolean[DebugClicked] = TRUE;
						return we;
					}
				}
			}
			else
			{
				m_DebugBoolean[DebugHover] = FALSE;
			}
			if (m_DebugBoolean[DebugClicked])
			{
				return we;
			}
		}

		//-------------------------------------------------------------------------------------------
		if(we & WE_LBTNCLICK)
		{
			if(GetPositionForXYRef(mouseInfo->GetMouseEventX(),mouseInfo->GetMouseEventY(),pos))
			{
				m_lCurSelCellPos = pos;
				cbWindowFunc(m_ID, m_pParent, WE_LBTNCLICK);	//PPP
			}
		}
		else if(we & WE_LBTNDBLCLICK)
		{
			if(GetPositionForXYRef(mouseInfo->GetMouseEventX(),mouseInfo->GetMouseEventY(),pos))
			{
				m_lCurSelCellPos = pos;
				cbWindowFunc(m_ID, m_pParent, WE_LBTNDBLCLICK);	//PPP
			}
		}
		else if(we & WE_RBTNDBLCLICK)
		{
			if(GetPositionForXYRef(mouseInfo->GetMouseEventX(),mouseInfo->GetMouseEventY(),pos))
			{
				m_lCurSelCellPos = pos;
				cbWindowFunc(m_ID, this, WE_RBTNDBLCLICK);			
			}
		}
		else
		{
			if( mouseInfo->LButtonDrag() )
			{
				if( ( we & WE_MOUSEOVER ) && IsDragOverDraw()
					&& GetPositionForXYRef(mouseInfo->GetMouseX(),mouseInfo->GetMouseY(),pos) )
				{
					m_lCurDragOverPos = pos;
					m_bItemDraged = TRUE;
				}
				else
				{
					m_lCurDragOverPos = -1;
					m_bItemDraged = FALSE;
				}
			}
			else if( mouseInfo->RButtonDown() )
			{
				if(  we
					&& GetPositionForXYRef(mouseInfo->GetMouseEventX(),mouseInfo->GetMouseEventY(),pos))
				{
					m_lCurSelCellPos = pos;
					//cbWindowFunc(m_ID, m_pParent, WE_RBTNCLICK);	//PPP
				}
			}
			else if( mouseInfo->RButtonUp() )
			{
				if(  we
					&& GetPositionForXYRef(mouseInfo->GetMouseEventX(),mouseInfo->GetMouseEventY(),pos))
				{
					//m_lCurSelCellPos = pos;
					if(m_lCurSelCellPos==pos)
					cbWindowFunc(m_ID, m_pParent, WE_RBTNCLICK);	//PPP
				}
			}
			else
			{
				if( m_bItemDraged )	//µå·¡±×ÈÄ¿¡ ³¡³ÂÀ½.
				{
					if( GetPositionForXYRef(mouseInfo->GetMouseEventX(),mouseInfo->GetMouseEventY(),pos) )
						m_lCurSelCellPos = pos;

					m_bItemDraged = FALSE;
				}

				m_lCurDragOverPos = -1;				
			}
		}
	}

	for(int i = 0 ; i < m_nCol*m_nRow ; i++)
	{
		if(m_pIconGridCell[i].use == USE && m_pIconGridCell[i].icon->IsActive() && m_pIconGridCell[i].icon && m_pIconGridCell[i].icon->IsDepend())
			m_pIconGridCell[i].icon->ActionEvent(mouseInfo);
	}

	return we;
}


void cIconGridDialog::Render()
{
	cDialog::Render();

	VECTOR2 start_pos;
	if(m_lCurSelCellPos != -1 && !m_SelectedBGImage.IsNull())
	{
		start_pos.x = m_absPos.x+m_cellRect.left+(m_wCellBorderX*(m_lCurSelCellPos%m_nCol+1))+(m_lCurSelCellPos%m_nCol*m_wCellWidth)-2;		// ÇÏµå ÄÚµù -2(taiyo temp)
		start_pos.y = m_absPos.y+m_cellRect.top+(m_wCellBorderY*(m_lCurSelCellPos/m_nCol+1))+(m_lCurSelCellPos/m_nCol*m_wCellHeight)-2;		// ÇÏµå ÄÚµù -2(taiyo temp)
		m_SelectedBGImage.RenderSprite(NULL,NULL,0,&start_pos,RGBA_MERGE(m_dwImageRGB, m_alpha * m_dwOptionAlpha / 100));

	}

	if(m_lCurDragOverPos != -1 && !m_DragOverBGImage.IsNull())
//	if(IsDragOverDraw())
	{
		start_pos.x = m_absPos.x+m_cellRect.left+(m_wCellBorderX*(m_lCurDragOverPos%m_nCol+1))+(m_lCurDragOverPos%m_nCol*m_wCellWidth)-2;		// ÇÏµå ÄÚµù -2(taiyo temp)

		start_pos.y = m_absPos.y+m_cellRect.top+(m_wCellBorderY*(m_lCurDragOverPos/m_nCol+1))+(m_lCurDragOverPos/m_nCol*m_wCellHeight)-2;		// ÇÏµå ÄÚµù -2(taiyo temp)
		m_DragOverBGImage.RenderSprite(NULL,NULL,0,&start_pos,RGBA_MERGE(m_dwImageRGB, m_alpha * m_dwOptionAlpha / 100));
	}

	// inverse order for tooltip
	for(int i = m_nCol*m_nRow-1 ; i >= 0  ; --i)
		if(m_pIconGridCell[i].use == USE && m_pIconGridCell[i].icon && m_pIconGridCell[i].icon->IsDepend())
			m_pIconGridCell[i].icon->Render();


#ifdef _JAPAN_LOCAL_

	for(i=m_DisableFromPos; i<m_DisableToPos; ++i)
	{
		if( !m_GridLockImage.IsNull() )
		{
			start_pos.x = m_absPos.x+m_cellRect.left+(m_wCellBorderX*(i%m_nCol+1))+(i%m_nCol*m_wCellWidth)-2;
			start_pos.y = m_absPos.y+m_cellRect.top+(m_wCellBorderY*(i/m_nCol+1))+(i/m_nCol*m_wCellHeight)-2;
			m_GridLockImage.RenderSprite(NULL,NULL,0,&start_pos,RGBA_MERGE(m_dwImageRGB, m_alpha * m_dwOptionAlpha / 100));
		}
	}

#endif	// _JAPAN_LOCAL_

#ifdef _HK_LOCAL_

	for(i=m_DisableFromPos; i<m_DisableToPos; ++i)
	{
		if( !m_GridLockImage.IsNull() )
		{
			start_pos.x = m_absPos.x+m_cellRect.left+(m_wCellBorderX*(i%m_nCol+1))+(i%m_nCol*m_wCellWidth)-2;
			start_pos.y = m_absPos.y+m_cellRect.top+(m_wCellBorderY*(i/m_nCol+1))+(i/m_nCol*m_wCellHeight)-2;
			m_GridLockImage.RenderSprite(NULL,NULL,0,&start_pos,RGBA_MERGE(m_dwImageRGB, m_alpha * m_dwOptionAlpha / 100));
		}
	}

#endif	// _HK_LOCAL_


#ifdef _KOR_LOCAL_

	for(int i=m_DisableFromPos; i<m_DisableToPos; ++i)
	{
		if( !m_GridLockImage.IsNull() )
		{
			start_pos.x = m_absPos.x+m_cellRect.left+(m_wCellBorderX*(i%m_nCol+1))+(i%m_nCol*m_wCellWidth)-2;
			start_pos.y = m_absPos.y+m_cellRect.top+(m_wCellBorderY*(i/m_nCol+1))+(i/m_nCol*m_wCellHeight)-2;

			m_GridLockImage.RenderSprite(NULL,NULL,0,&start_pos,RGBA_MERGE(m_dwImageRGB, m_alpha * m_dwOptionAlpha / 100));
		}
	}

#endif	// _HK_LOCAL_

	if (m_DebugEnabled)
	if (m_DebugBoolean[DebugHover] || m_DebugBoolean[DebugClicked])
	{
		DWORD inLine = RGBA_MERGE(RGB_HALF(255, 0, 0), 255);
		VECTOR2	TopLeft, TopRight, BottomLeft, BottomRight;
		GetVec2(m_absPos, m_width, m_height, TopLeft, TopRight, BottomLeft, BottomRight);
		if (m_DebugBoolean[DebugHover])
		{
			g_pExecutive->GetRenderer()->RenderLine(&TopLeft, &TopRight, inLine);
			g_pExecutive->GetRenderer()->RenderLine(&TopRight, &BottomRight, inLine);
			g_pExecutive->GetRenderer()->RenderLine(&BottomRight, &BottomLeft, inLine);
			g_pExecutive->GetRenderer()->RenderLine(&BottomLeft, &TopLeft, inLine);
		}
		if (m_DebugBoolean[DebugClicked])
		{
			inLine = RGBA_MERGE(RGB_HALF(0, 0, 255), 255);
			
			g_pExecutive->GetRenderer()->RenderLine(&TopLeft, &TopRight, inLine);
			g_pExecutive->GetRenderer()->RenderLine(&TopRight, &BottomRight, inLine);
			g_pExecutive->GetRenderer()->RenderLine(&BottomRight, &BottomLeft, inLine);
			g_pExecutive->GetRenderer()->RenderLine(&BottomLeft, &TopLeft, inLine);
			if (*m_szTextDebug != 0)
			{
				LONG tempX = m_absPos.x - 11;
				LONG tempY = m_absPos.y - 41;
				WORD nameFontID = 1;
				WORD nameFontPOS = 0;
			
				char IDtemp2[64] = { 0 };

				RECT rectDebugID = { (LONG)tempX, (LONG)tempY, 1, 1 };

				RECT rectDebug = { (LONG)tempX, (LONG)tempY + CFONT_OBJ->GetTextHeight(nameFontID), 1, 1 };

				sprintf(IDtemp2, "%s", (char*)WINIDSEARCH(m_SavedID));
				
				CFONT_OBJ->RenderFontWithShadow(nameFontID, IDtemp2, strlen(IDtemp2), &rectDebugID, inLine);

				inLine = RGBA_MERGE(RGB_HALF(255, 0, 0), 255);
				
				CFONT_OBJ->RenderFontWithShadow(nameFontPOS, m_szTextDebug, strlen(m_szTextDebug), &rectDebug, inLine);
			}
			if (m_DebugBoolean[DebugKDBActivated])
			{
				if (gCurTime - m_DebugTimerKDBActivated > 500)
				{
					if (g_UserInput.GetKeyboard()->GetKeyPressed(KEY_LEFT))
					{
						SetAbsXY(m_absPos.x - 1, m_absPos.y);
						m_SavedXY.x = m_SavedXY.x - 1;
						strncpy(m_szTextDebug, Vector2Text(m_SavedXY.x, m_SavedXY.y,  m_SavedWidthHeight[0], m_SavedWidthHeight[1]), strlen(Vector2Text(m_SavedXY.x, m_SavedXY.y,  m_SavedWidthHeight[0], m_SavedWidthHeight[1])) + 1);
					}
					if (g_UserInput.GetKeyboard()->GetKeyPressed(KEY_RIGHT))
					{
						SetAbsXY(m_absPos.x + 1, m_absPos.y);
						m_SavedXY.x = m_SavedXY.x + 1;
						strncpy(m_szTextDebug, Vector2Text(m_SavedXY.x, m_SavedXY.y,  m_SavedWidthHeight[0], m_SavedWidthHeight[1]), strlen(Vector2Text(m_SavedXY.x, m_SavedXY.y,  m_SavedWidthHeight[0], m_SavedWidthHeight[1])) + 1);
					}
					if (g_UserInput.GetKeyboard()->GetKeyPressed(KEY_UP))
					{
						SetAbsXY(m_absPos.x, m_absPos.y - 1);
						m_SavedXY.y = m_SavedXY.y - 1;
						strncpy(m_szTextDebug, Vector2Text(m_SavedXY.x, m_SavedXY.y,  m_SavedWidthHeight[0], m_SavedWidthHeight[1]), strlen(Vector2Text(m_SavedXY.x, m_SavedXY.y,  m_SavedWidthHeight[0], m_SavedWidthHeight[1])) + 1);
					}
					if (g_UserInput.GetKeyboard()->GetKeyPressed(KEY_DOWN))
					{
						SetAbsXY(m_absPos.x, m_absPos.y + 1);
						m_SavedXY.y = m_SavedXY.y + 1;
						strncpy(m_szTextDebug, Vector2Text(m_SavedXY.x, m_SavedXY.y,  m_SavedWidthHeight[0], m_SavedWidthHeight[1]), strlen(Vector2Text(m_SavedXY.x, m_SavedXY.y,  m_SavedWidthHeight[0], m_SavedWidthHeight[1])) + 1);
					}
					SetRelXY(m_SavedXY.x, m_SavedXY.y);
				}
			}
		}
	}
}

void cIconGridDialog::SetActive(BOOL val)
{
	if( m_bDisable ) return;
	

	cDialog::SetActiveRecursive(val);

/// need confirm
	m_lCurSelCellPos = -1;
///
	for(int i = 0 ; i < m_nCol*m_nRow ; i++)
		if(m_pIconGridCell[i].use == USE && m_pIconGridCell[i].icon && m_pIconGridCell[i].icon->IsDepend())
			m_pIconGridCell[i].icon->SetActive(val);
}


void cIconGridDialog::SetAlpha(BYTE al)
{ 
	cDialog::SetAlpha(al);
	for(int i = 0 ; i < m_nCol*m_nRow ; i++)
	{
		if(m_pIconGridCell[i].use == USE && m_pIconGridCell[i].icon)
			m_pIconGridCell[i].icon->SetAlpha(al);
	}
}
/**/
/**/
/**/
/**/
// 1-dimension
BOOL cIconGridDialog::IsAddable(WORD pos)
{
	ASSERT(pos < m_nCol*m_nRow);

	if(m_pIconGridCell[pos].use == USE)
		return FALSE;
	return TRUE;
}
BOOL cIconGridDialog::AddIcon(WORD pos, cIcon * icon)
{
	ASSERT(pos < m_nCol*m_nRow);
	WORD cellX = pos%m_nCol;
	WORD cellY = pos/m_nCol;

//	ASSERT(m_pIconGridCell[pos].use == USE);
	if(m_pIconGridCell[pos].use == USE) return FALSE;
	icon->SetAbsXY((LONG)(m_absPos.x+m_cellRect.left+(m_wCellBorderX*(cellX+1))+((cellX)*m_wCellWidth)), (LONG)(m_absPos.y+m_cellRect.top+(m_wCellBorderY*(cellY+1))+((cellY)*m_wCellHeight)));
	icon->SetDepend(TRUE);
	icon->SetActive(m_bActive);
	m_pIconGridCell[pos].icon = icon;
	

	m_pIconGridCell[pos].use = USE;
	

	icon->SetParent(this);
	//ÇöÀç Ã¢ÀÌ µð½º¿¡ÀÌºí»óÅÂ¸é ¾ÆÀÌµµ µð½º¿¡ÀÌºí·Î
	if( m_bDisable )
		icon->SetDisable( TRUE );

	icon->SetCellPosition(cellX,cellY);
	return TRUE;
}

BOOL cIconGridDialog::GetCellAbsPos(WORD pos, int& absX, int& absY)
{
	ASSERT(pos < m_nCol*m_nRow);
	WORD cellX = pos%m_nCol;

	WORD cellY = pos/m_nCol;

	if(m_pIconGridCell[pos].use == NOTUSE) return FALSE;

	absX = (int)(m_absPos.x+m_cellRect.left+(m_wCellBorderX*(cellX+1))+((cellX)*m_wCellWidth));
	absY = (int)(m_absPos.y+m_cellRect.top+(m_wCellBorderY*(cellY+1))+((cellY)*m_wCellHeight));

	return TRUE;
}

// 2-dimension
BOOL cIconGridDialog::GetCellPosition(LONG mouseX, LONG mouseY, WORD& cellX,WORD& cellY)
{

	for(int i = 0 ; i < m_nCol ; i++)
	{
		for(int j = 0 ; j < m_nRow ; j++)
		{
			int cellpX = (int)(m_absPos.x+m_cellRect.left+(m_wCellBorderX*(i+1))+(i*m_wCellWidth));
			int cellpY = (int)(m_absPos.y+m_cellRect.top+(m_wCellBorderY*(j+1))+(j*m_wCellHeight));

			// 2015-01-04 ´Ë´¦´úÂëÓÐbug. ±àÐ´±íÇéÏµÍ³Ê±·¢ÏÖ,Èô iconGridDialog  #INITGRID ³ß´çÊÇÄ¬ÈÏµÄ40*40 ,Ôò»ñÈ¡µ½µÄPosÎ»ÖÃÓÐÆ«²î! DEFAULT_CELLSIZE Ìæ»»Îª m_wCellWidth ,m_wCellHeight
			if(cellpX < mouseX && mouseX < cellpX + m_wCellWidth && cellpY < mouseY && mouseY < cellpY + m_wCellHeight)
			{
				cellX = i;
				cellY = j;
				return TRUE;
			}
		}
		
	}
	return FALSE;
}

BOOL cIconGridDialog::GetPositionForXYRef(LONG mouseX, LONG mouseY, WORD& pos)
{
	

	WORD x,y;
	BOOL rt = GetCellPosition(mouseX, mouseY, x, y);
	pos = y*m_nCol+x;	//tmp) pos = GetPositionForCell(x,y);
	return rt;
}

WORD cIconGridDialog::GetPositionForCell(WORD cellX, WORD cellY)
{
	return (cellY)*m_nCol+(cellX);
}
BOOL cIconGridDialog::IsAddable(WORD cellX,WORD cellY,cIcon* pIcon)
{
	if(m_pIconGridCell[cellY*m_nCol+cellX].use == USE)
		return FALSE;
	if(!(pIcon->GetIconType() & m_acceptableIconType))

		return FALSE;

	return TRUE;
}

BOOL cIconGridDialog::AddIcon(WORD cellX, WORD cellY, cIcon * icon)
{
	return AddIcon(cellX+cellY*m_nCol, icon);
}

BOOL cIconGridDialog::MoveIcon(WORD cellX, WORD cellY, cIcon * icon)
{
//KES 040511 ¿¡·¯¸®ÅÏ ¼öÁ¤
	if( !IsAddable( GetPositionForCell( cellX, cellY ) ) )
		return FALSE;

	cIconGridDialog* pBeforeGrid = (cIconGridDialog*)icon->GetParent();

	WORD wOldCellX = icon->GetCellX();
	WORD wOldCellY = icon->GetCellY();

	if( pBeforeGrid->DeleteIcon(icon) == FALSE )
		return FALSE;

	if( AddIcon( cellX, cellY, icon ) )
	{
		return TRUE;
	}
	else
	{
		pBeforeGrid->AddIcon( wOldCellX, wOldCellY, icon );

		return FALSE;
	}
}

BOOL cIconGridDialog::DeleteIcon(cIcon * icon)
{
	WORD x = icon->GetCellX();
	WORD y = icon->GetCellY();
	
	cIcon * dummy;
	
	return DeleteIcon(x, y, &dummy);
}
BOOL cIconGridDialog::DeleteIcon(WORD pos, cIcon ** icon)
{
	ASSERT(pos < m_nCol*m_nRow);

	if(m_pIconGridCell[pos].use == NOTUSE)
	{
		if( icon ) 
		*icon = NULL;
		//ASSERT(0);
		return FALSE;
	}

	if( icon ) 
	*icon = m_pIconGridCell[pos].icon;
	m_pIconGridCell[pos].icon->SetCellPosition(0,0);

	m_pIconGridCell[pos].icon->SetRelationToolTip(NULL);//kiv
	m_pIconGridCell[pos].icon->SetRelationToolTip1(NULL);//kiv

	m_pIconGridCell[pos].icon->SetDepend(FALSE);
	m_pIconGridCell[pos].icon = NULL;
	m_pIconGridCell[pos].use = NOTUSE;


	return TRUE;
}


BOOL cIconGridDialog::DeleteIcon(WORD cellX, WORD cellY, cIcon ** iconOut)
{
	return DeleteIcon(cellY*m_nCol+cellX, iconOut);
}


void cIconGridDialog::SetDisable( BOOL val )
{
	cDialog::SetDisable( val );

	for(int i = 0 ; i < m_nRow*m_nCol ; i++)
	{
		if( m_pIconGridCell[i].icon )
			m_pIconGridCell[i].icon->SetDisable( val );
	}	
}

BOOL cIconGridDialog::IsDragOverDraw()
{
	if( !WINDOWMGR->IsDragWindow() ) return FALSE;

	cDialog* pDlg = WINDOWMGR->GetDragDlg();

	if( pDlg )
	if( pDlg->GetType() == m_nIconType )
		return TRUE;

	return FALSE;
}




