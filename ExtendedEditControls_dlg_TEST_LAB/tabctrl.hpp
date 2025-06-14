#ifndef _TABCTRL_HPP
#define _TABCTRL_HPP

#pragma once

#include <afxwin.h>
#include <afxcmn.h>
#include <windows.h>

// Standard C++ includes
#include <vector>

/************************************************
*
*                   TAB CONTROL
*
************************************************/

class TabControl: public CTabCtrl
{
public:
  TabControl();
  virtual ~TabControl();
  
  // Overriden CTabCtrl/CWnd members
  virtual LRESULT WindowProc( UINT message, WPARAM wParam, LPARAM lParam );
  afx_msg void OnNewFolder();
  afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
  // Command/Notification Handlers
  afx_msg void OnLButtonDown( UINT nFlags, CPoint point );
  afx_msg void OnLButtonUp( UINT nFlags, CPoint point );
  afx_msg void OnMouseMove( UINT nFlags, CPoint point );
  afx_msg void OnCaptureChanged( CWnd* );
//  afx_msg void OnPaint();
  
private:
  typedef std::vector<unsigned int> TabOrder;

  bool  m_bRedirect;     // Specifies whether redirection should take place.
  bool  m_bDragging;     // Specifies that whether drag 'n drop is in progress.
  UINT  m_nSrcTab;       // Specifies the source tab that is going to be moved.
  UINT  m_nDstTab;       // Specifies the destination tab (drop position).
  bool  m_bHotTracking;  // Specifies the state of whether the tab control has hot tracking enabled.

  CRect m_InsertPosRect;

  CSpinButtonCtrl * m_pSpinCtrl;
  
  TabOrder m_TabOrder; // Contains the current order of the tabs within the tab control.
  
  // Utility members
  bool DrawIndicator( CPoint point );
  bool ReorderTab( unsigned int nSrcTab, unsigned int nDstTab );
  unsigned int Redirect( unsigned int nTab );
  
  DECLARE_MESSAGE_MAP()
};

#endif _TABCTRL_HPP

// END TABCTRL.HPP
