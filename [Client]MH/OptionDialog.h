



#ifndef _OPTIONDIALOG_H
#define _OPTIONDIALOG_H



#include "./Interface/cTabDialog.h"
#include "./Interface/cCheckBox.h"   // include for clearpacket
#include "./Interface/cStatic.h"     // include for clearpacket
#include "OptionManager.h"
#include "AISetManager.h"
//class cButton;
#define OPTIONDLG USINGTON(COptionDialog)
class cListDialog;
class COptionDialog : public cTabDialog
{

protected:


	sGAMEOPTION		m_GameOption;
	//sGAMEAI			m_GameAiOption;

	sGAMEAI			m_GameAI;
//	cButton*		m_pBtnPreview;

	cListDialog * Packet_List;

	VECTOR2			SavePos;


	DWORD					m_BtnPushstartTime;	
	DWORD					m_BtnPushDelayTime;
	BOOL					m_bPushTabWithMouseOver;

protected:


	void UpdateData( BOOL bSave );
public:
   
	VECTOR3 m_TmpPos;   // the pos is define for AI 2014-05-19

	COptionDialog();
	virtual ~COptionDialog();

	virtual void Add(cWindow * window);

	virtual void SetActive(BOOL val);

	//void Render();


	void Linking();
	void OnActionEvent(LONG lId, void * p, DWORD we);
	//virtual DWORD ActionEvent(CMouse * mouseInfo);

	void DisableGraphicTab(BOOL bDisable);


	// 2005.12.28ÀÏ Ãß°¡ ´« On/Off
	int GetEffectSnow() { return m_GameOption.nEffectSnow;}

	void DeleteClearItem();
	void AddClearItemList(cIcon * icon);
	virtual BOOL FakeMoveIcon(LONG x, LONG y, cIcon * icon);

	LONG GetFovVal();

	void Test();

	int GetPacketSelectNum();
	void CheckPacketSelectScrollbar();
};
EXTERNGLOBALTON(COptionDialog)
#endif


