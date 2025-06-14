#pragma once
#include "./interface/cDialog.h"
enum {NormalBtn=0,StopBtn=1,};
class cButton;
class CVideoCaptureIcon: public cDialog
{
	cButton * m_IconBtn[2];
public:	
	static CVideoCaptureIcon& Instance()
	{
		static CVideoCaptureIcon instance;

		return instance;
	}
	CVideoCaptureIcon();
	~CVideoCaptureIcon();
	void Linking();
	void OnActionEvent(LONG lId, void * p, DWORD we);
	void ShowNormalBtn();
	void ShowStopBtn();
	void ShowVideoIconStatic();
	virtual DWORD ActionEvent(CMouse * mouseInfo);
	void SetFPS(char * c);
};