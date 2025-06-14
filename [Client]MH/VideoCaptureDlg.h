#pragma once
#include "./interface/cDialog.h"

class cComboBox;
class cCheckBox;
class cButton;
enum{SIZECombo=0,OPTTCombo=1,};
class CVideoCaptureDlg : public cDialog
{
	cDialog * m_Dlg;
	cComboBox* m_pComboBox[2];
	//ComboBox* m_pSizeCombo;		
	//cComboBox* m_pOptCombo;			
	cCheckBox* m_pNoCursor;			

public:
	enum eVideoCaptureSize
	{
		eVideoCaptureSize_Default,
		eVideoCaptureSize_500x375,
		eVideoCaptureSize_800x600,
		eVideoCaptureSize_1024x768,
		eVideoCaptureSize_1280x800,
		eVideoCaptureSize_1920x1080,
		eVideoCaptureSize_Max,
	};
	enum eVideoCaptureOpt
	{
		eVideoCaptureOpt_High,
		eVideoCaptureOpt_Low,
		eVideoCaptureOpt_Max,
	};

	CVideoCaptureDlg();
	virtual ~CVideoCaptureDlg();
    void   Linking();
	void  OnActionEvent(LONG lId, void * p, DWORD we);
	
	const char* GetTextVideoCaptureSize( eVideoCaptureSize eSize );			
	const char* GetTextVideoCaptureOpt( eVideoCaptureOpt eOpt );		
	int GetSelectedSize();													
	int GetSelectedOpt();													
	BOOL IsIncludeCursor();	
};