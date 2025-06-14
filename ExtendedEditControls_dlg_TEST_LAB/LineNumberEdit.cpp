#include "stdafx.h"
#include "LineNumberEdit.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
extern UINT urm_SELECTLINE;
CLineNumberEdit::CLineNumberEdit()
{
	m_hWnd = NULL;
	m_line.m_hWnd = NULL;
	m_zero.cx = 0;
	m_zero.cy = 0;
	m_format = _T( "%3i" );
	m_LineDelta = 1;
	m_maxval = 0;
	m_bUseEnabledSystemColours = FALSE;
	m_bUseDisabledSystemColours = TRUE;
	m_EnabledFgCol = RGB(255, 45, 48); 
	m_EnabledBgCol = RGB(30, 30, 30);
	m_DisabledFgCol = GetSysColor( COLOR_GRAYTEXT );
	m_DisabledBgCol = GetSysColor( COLOR_3DFACE );
	SetWindowColour();
}
CLineNumberEdit::~CLineNumberEdit()
{
}
BEGIN_MESSAGE_MAP(CLineNumberEdit, CEdit)
	ON_CONTROL_REFLECT(EN_CHANGE, OnChange)
	ON_WM_VSCROLL()
	ON_CONTROL_REFLECT(EN_VSCROLL, OnVscroll)
	ON_MESSAGE(WM_SETFONT, OnSetFont)
	ON_WM_SIZE()
	ON_MESSAGE(WM_SETTEXT, OnSetText)
	ON_WM_SYSCOLORCHANGE()
	ON_WM_ENABLE()
	ON_MESSAGE(EM_LINESCROLL, OnLineScroll)
	ON_REGISTERED_MESSAGE(urm_SELECTLINE, OnSelectLine)
END_MESSAGE_MAP()
void CLineNumberEdit::PreSubclassWindow() 
{
	ASSERT( GetStyle() & ES_MULTILINE );
	SetLineNumberFormat( m_format );
}
void CLineNumberEdit::OnSysColorChange() 
{
	CEdit::OnSysColorChange();
	Invalidate();
    SetWindowColour( IsWindowEnabled() );
}
LRESULT CLineNumberEdit::OnSetText( WPARAM wParam, LPARAM lParam )
{
	LRESULT retval = DefWindowProc( WM_SETTEXT, wParam, lParam );
	UpdateTopAndBottom();
	return retval;
}
void CLineNumberEdit::OnChange() 
{
	UpdateTopAndBottom();
}
void CLineNumberEdit::OnVscroll() 
{
	UpdateTopAndBottom();
}
void CLineNumberEdit::OnVScroll( UINT nSBCode, UINT nPos, CScrollBar* pScrollBar ) 
{
	CEdit::OnVScroll( nSBCode, nPos, pScrollBar );
	UpdateTopAndBottom();
}
LRESULT CLineNumberEdit::OnLineScroll( WPARAM wParam, LPARAM lParam ) 
{
	LRESULT retval = DefWindowProc( EM_LINESCROLL, wParam, lParam );
	UpdateTopAndBottom();
	return retval;
}
LRESULT CLineNumberEdit::OnSetFont( WPARAM wParam, LPARAM lParam )
{
	DefWindowProc( WM_SETFONT, wParam, lParam );
	Prepare(m_enablelinenum);
	return 0;
}
void CLineNumberEdit::OnSize( UINT nType, int cx, int cy ) 
{
	CEdit::OnSize( nType, cx, cy );
	if( m_line.m_hWnd )
		Prepare(m_enablelinenum);
}
void CLineNumberEdit::OnEnable( BOOL bEnable ) 
{
	CEdit::OnEnable( bEnable );
    SetWindowColour( bEnable );
}
LRESULT CLineNumberEdit::OnSelectLine(WPARAM wParam, LPARAM  )
{
	int lineno = wParam + GetScrollPos( SB_VERT );
	int start = LineIndex( lineno );
	int end = LineIndex( lineno + 1 );
	SetSel(end - 1, start);
	return 0;
}
void CLineNumberEdit::SetWindowColour( BOOL bEnable  )
{
    if (m_bUseEnabledSystemColours)
    {
		m_EnabledFgCol = GetSysColor( COLOR_WINDOWTEXT );
		m_EnabledBgCol = GetSysColor( COLOR_WINDOW );
    }
    if (m_bUseDisabledSystemColours)
    {
		m_DisabledFgCol = GetSysColor( COLOR_GRAYTEXT );
		m_DisabledBgCol = GetSysColor( COLOR_3DFACE );
    }
    if (bEnable)
    {
        m_line.SetFgColor( m_EnabledFgCol, TRUE );
        m_line.SetBgColor( m_EnabledBgCol, TRUE );
    } else {
        m_line.SetFgColor( m_DisabledFgCol, TRUE );
        m_line.SetBgColor( m_DisabledBgCol, TRUE );
    }
}
void CLineNumberEdit::UseSystemColours( BOOL bUseEnabled , BOOL bUseDisabled  )
{
    m_bUseEnabledSystemColours = bUseEnabled;
    m_bUseDisabledSystemColours = bUseDisabled;
    BOOL bEnable = TRUE;
    if (::IsWindow(m_hWnd))
        bEnable = IsWindowEnabled();
    SetWindowColour( bEnable );
}
void CLineNumberEdit::Prepare(BOOL bEnable)
{
	int width = CalcLineNumberWidth();
	CRect rect;
	GetClientRect( &rect );
	CRect rectEdit( rect );
	rect.right = width;
	rectEdit.left = rect.right + 1;
	SetRect( &rectEdit );
	{
		if (m_line.m_hWnd)
		{
			m_line.MoveWindow(1.5f, 0, width, rect.Height());
		}
		else
		{
			if (bEnable)
			{
				m_line.Create(NULL, WS_CHILD | WS_VISIBLE | SS_NOTIFY, rect, this, 1);
			}
		}
	}
	GetRect( &rectEdit );
	m_line.SetTopMargin( rectEdit.top );
	UpdateTopAndBottom();
}
int CLineNumberEdit::CalcLineNumberWidth()
{
	CClientDC dc( this );
	CFont* font = GetFont();
	CFont* oldFont = dc.SelectObject( font );
	m_zero=dc.GetTextExtent( _T( "0" ) );
	CString format;
	int maxval = GetLimitText();
    if (m_maxval > 0)
        maxval = m_maxval + m_LineDelta;
	format.Format( m_format, maxval );
	CSize fmt = dc.GetTextExtent( format );
	dc.SelectObject( oldFont );
	return fmt.cx + 35;
}
void CLineNumberEdit::UpdateTopAndBottom() 
{
	CRect rect;
	GetClientRect( &rect );
	int maxline = GetLineCount() + m_LineDelta;
	int lineheight = m_zero.cy;
	int topline = GetFirstVisibleLine() + m_LineDelta;
	if( ( topline + ( rect.Height() / lineheight ) ) < maxline )
		maxline = topline + ( rect.Height() / lineheight );
    if ( m_maxval > 0 && maxline > m_maxval + m_LineDelta )
        maxline = m_maxval + m_LineDelta;
	m_line.SetTopAndBottom( topline, maxline );
}
void CLineNumberEdit::SetMarginForegroundColor( COLORREF col, BOOL redraw, BOOL bEnabled  )
{
	m_line.SetFgColor( col, redraw );
    if (bEnabled)
    {
        m_bUseEnabledSystemColours = FALSE;
        m_EnabledFgCol = col;
    } else {
        m_bUseDisabledSystemColours = FALSE;
        m_DisabledFgCol = col;
    }
}
void CLineNumberEdit::SetMarginBackgroundColor( COLORREF col, BOOL redraw, BOOL bEnabled  )
{
	m_line.SetBgColor( col, redraw );
    if (bEnabled)
    {
        m_bUseEnabledSystemColours = FALSE;
        m_EnabledBgCol = col;
    } else {
        m_bUseDisabledSystemColours = FALSE;
        m_DisabledBgCol = col;
    }
}
void CLineNumberEdit::SetLineNumberFormat( CString format )
{
	m_format = format;
	m_line.SetLineNumberFormat( format );
	if( m_hWnd )
		Prepare(m_enablelinenum);
}
void CLineNumberEdit::SetLineNumberRange( UINT nMin, UINT nMax  )
{
    m_LineDelta = ( int ) nMin;
    m_maxval = ( int ) nMax;
}
