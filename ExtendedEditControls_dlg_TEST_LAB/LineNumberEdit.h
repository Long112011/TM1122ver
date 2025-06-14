#include "LineNumberStatic.h"
class CLineNumberEdit : public CEdit
{
public:
	CLineNumberEdit();
	virtual ~CLineNumberEdit();
	void SetMarginForegroundColor( COLORREF col, BOOL redraw = TRUE, BOOL bEnabled = TRUE );
	void SetMarginBackgroundColor( COLORREF col, BOOL redraw = TRUE, BOOL bEnabled = TRUE );
	void SetLineNumberFormat( CString format );
    void SetLineNumberRange( UINT nMin, UINT nMax = 0 );
    void UseSystemColours( BOOL bUseEnabled = TRUE, BOOL bUseDisabled = TRUE );
	void SetLineNumberEnable(BOOL bflag){ m_enablelinenum = bflag; UseSystemColours(); }
	virtual afx_msg void OnSysColorChange();
protected:
	virtual void PreSubclassWindow();
	virtual afx_msg void OnEnable( BOOL bEnable );
	virtual afx_msg void OnChange();
	virtual afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	virtual afx_msg void OnVscroll();
	virtual afx_msg void OnSize(UINT nType, int cx, int cy);
	virtual afx_msg LRESULT OnSetFont(WPARAM wParam, LPARAM lParam); 
	virtual afx_msg LRESULT OnSetText(WPARAM wParam, LPARAM lParam); 
	virtual afx_msg LRESULT OnLineScroll(WPARAM wParam, LPARAM lParam); 
	virtual afx_msg LRESULT OnSelectLine(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
private:
	void Prepare(BOOL bEnable );
	int CalcLineNumberWidth();
	void UpdateTopAndBottom();
	void SetWindowColour( BOOL bEnable = TRUE );
	BOOL				m_enablelinenum;
	BOOL				m_bUseEnabledSystemColours;
	COLORREF			m_EnabledFgCol;
	COLORREF			m_EnabledBgCol;
	BOOL				m_bUseDisabledSystemColours;
	COLORREF			m_DisabledFgCol;
	COLORREF			m_DisabledBgCol;
	CLineNumberStatic	m_line;
	CSize				m_zero;
	int					m_maxval;
	CString				m_format;
    int                 m_LineDelta; 
};