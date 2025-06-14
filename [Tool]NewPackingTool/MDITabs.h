/****************************************************************************\
Datei  : MDITabs.h
Projekt: MDITabs, a tabcontrol for switching between MDI-views
Inhalt : EMDITabStyles, CMDITabs declaration
Datum  : 03.10.2001
Autor  : Christian Rodemeyer
Hinweis: 2001 by Christian Rodemeyer
\****************************************************************************/
#pragma once
#ifndef __MDITABS_H
#define __MDITABS_H
enum EMDITabStyles
{
  MT_BOTTOM        = 0x0000,
  MT_TOP           = 0x0001,
  MT_IMAGES        = 0x0002,
  MT_HIDEWLT2VIEWS = 0x0004,
  MT_TOOLTIPS      = 0x0008,
  MT_BUTTONS       = 0x0010,
  MT_AUTOSIZE      = 0x0020,
  MT_TASKBAR       = 0x0038
};
enum EWMMTGetInfo
{
  WM_GETTABTIPTEXT = WM_APP + 0x0393,
  WM_GETTABSYSMENU = WM_APP + 0x0394
};
class CMDITabs : public CTabCtrl
{
public:
  CMDITabs();
  void Create(CFrameWnd* pMainFrame, DWORD dwStyle = MT_BOTTOM|MT_IMAGES);
  void Update();
  void SetMinViews(int minViews) {m_minViews = minViews;}
private:
  HWND       m_mdiClient;
  int        m_height;
  int        m_width;
  CImageList m_images;
  int        m_minViews;
  bool       m_bImages;
  bool       m_bTop;
public:
public:
protected:
  afx_msg void OnSelChange(NMHDR* pNMHDR, LRESULT* pResult);
  afx_msg void OnPaint();
  afx_msg void OnNcPaint();
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
  afx_msg LRESULT OnSizeParent(WPARAM, LPARAM lParam);
  DECLARE_MESSAGE_MAP()
};
#endif
