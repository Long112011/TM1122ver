#include "stdafx.h"
#include "LineNumberStatic.h"
UINT urm_SELECTLINE = ::RegisterWindowMessage(_T("_LINE_NUMBER_EDIT_SELECTLINE_"));
CLineNumberStatic::CLineNumberStatic()
{
	m_bgcol = RGB(255, 45, 48); 
	m_fgcol = RGB(30, 30, 30);
    m_format = _T( "%3i" );
    m_topline = 0;
    m_bottomline = 0;
}
CLineNumberStatic::~CLineNumberStatic()
{
}
BEGIN_MESSAGE_MAP(CLineNumberStatic, CStatic)
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()
void CLineNumberStatic::OnPaint() 
{
	CPaintDC dcPaint( this );
	CRect rect;
	GetClientRect( &rect );
	CDC dc;
	dc.CreateCompatibleDC( &dcPaint );
	int saved = dc.SaveDC();
	CBitmap bmp;
	CPen pen;
	bmp.CreateCompatibleBitmap( &dcPaint, rect.Width(), rect.Height() );
	pen.CreatePen( PS_NULL, 1, m_fgcol );
	dc.SelectObject( &bmp );
	dc.SelectObject( &pen );
	dc.FillSolidRect( &rect, m_bgcol );
	dc.MoveTo( rect.right - 1, 0 );
	dc.LineTo( rect.right - 1, rect.bottom );
	dc.SetTextColor( m_fgcol );
	dc.SetBkColor( m_bgcol );
	dc.SelectObject( GetParent()->GetFont() );
	if( m_bottomline )
	{
		int lineheight = dc.GetTextExtent( _T( "0" ) ).cy;
		for( int t = m_topline ; t < m_bottomline ; t++ )
		{
			CString output;
			output.Format( m_format, t );
			int topposition = m_topmargin + lineheight * ( t - m_topline );
			dc.TextOut( 2, topposition, output );
		}
	}
	dcPaint.BitBlt( 0, 0, rect. right, rect.bottom, &dc, 0, 0, SRCCOPY );
	dc.RestoreDC( saved );
}
BOOL CLineNumberStatic::OnEraseBkgnd( CDC* ) 
{
	return TRUE;
}
void CLineNumberStatic::OnLButtonDown( UINT nFlags, CPoint point )
{
	CClientDC	dc( this );
	dc.SelectObject( GetParent()->GetFont() );
	int lineheight = dc.GetTextExtent( _T( "0" ) ).cy;
	int lineno = ( int ) ( ( double ) point.y / ( double ) lineheight );
	GetParent()->SendMessage( urm_SELECTLINE, lineno );
	CStatic::OnLButtonDown( nFlags, point );
}
void CLineNumberStatic::SetBgColor( COLORREF col, BOOL redraw )
{
	m_bgcol = col;
	if( m_hWnd && redraw )
		RedrawWindow();
}
void CLineNumberStatic::SetFgColor( COLORREF col, BOOL redraw )
{
	m_fgcol = col;
	if( m_hWnd && redraw )
		RedrawWindow();
}
void CLineNumberStatic::SetTopAndBottom( int topline, int bottomline )
{
	m_topline = topline;
	m_bottomline = bottomline;
	if( m_hWnd )
		RedrawWindow();
}
void CLineNumberStatic::SetTopMargin( int topmargin )
{
	m_topmargin = topmargin;
}
void CLineNumberStatic::SetLineNumberFormat( CString format )
{
	m_format = format;
	if( m_hWnd )
		RedrawWindow();
}