#if !defined(AFX_SHOUTDIALOG_H__2851FDE7_5AC8_4835_8F46_900FC49814D7__INCLUDED_)
#define AFX_SHOUTDIALOG_H__2851FDE7_5AC8_4835_8F46_900FC49814D7__INCLUDED_
#if _MSC_VER > 1000
#pragma once
#endif 
#include "./Interface/cDialog.h"
#include "./Interface/cIcon.h"
#include "./Interface/cIconGridDialog.h"   
#define CHARFACELIST        70         
#define FACELISTSTARTIDX	0        
class cEditBox;
class cListDialog;
class cIcon;        
class CShoutDialog : public cDialog
{
	DWORD			m_dwItemIdx;
	DWORD			m_dwItemPos;
	DWORD           m_dwItemType;
	cEditBox*		m_pMsgBox;
	cImage          m_ImageFace[CHARFACELIST];   
	cIcon           m_IconFace[CHARFACELIST];    
	cIconGridDialog * m_ImageGrid;     
public:
	CShoutDialog();
	virtual ~CShoutDialog();
	void Linking();
	BOOL SendShoutMsgSyn();
	void SetItemInfo( DWORD ItemIdx, DWORD ItemPos,DWORD ItemType=0 )			{	m_dwItemIdx = ItemIdx;	m_dwItemPos = ItemPos;	m_dwItemType=ItemType;}
	void LoadFaceList();  
	void OnActionEvent(LONG lId, void * p, DWORD we);  
	void SetShouMsg(char * pMsg);
};
#endif 