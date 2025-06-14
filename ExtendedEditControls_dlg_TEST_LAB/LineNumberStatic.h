class CLineNumberStatic : public CStatic
{
public:
	CLineNumberStatic();
	virtual ~CLineNumberStatic();
public:
	void SetFgColor( COLORREF col, BOOL redraw );
	void SetBgColor( COLORREF col, BOOL redraw );
	void SetTopAndBottom( int topline, int bottomline );
	void SetTopMargin( int topmargin );
	void SetLineNumberFormat( CString format );
protected:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown( UINT nFlags, CPoint point );
	DECLARE_MESSAGE_MAP()
private:
	COLORREF			m_fgcol;
	COLORREF			m_bgcol;
	CString				m_format;
	int m_topmargin;	
	int m_topline;		
	int m_bottomline;	
};