#ifndef __COMMONDEFINE_H__
#define __COMMONDEFINE_H__

#define USINGTON(className)			(&g_##className)
#define EXTERNGLOBALTON(className)	extern className g_##className;
#define GLOBALTON(className)		className g_##className;
#define MAKESINGLETON(classname)	static classname* GetInstance()	{	static classname gInstance;		return &gInstance;	}
#define GETINSTANCE(ClassName) static ClassName * GetInstance(){ static ClassName g_Instance; return &g_Instance; }
#define SAFE_DELETE(a)			if((a))	{ delete (a); (a)=NULL; }
#define SAFE_DELETE_ARRAY(a)	if((a))	{ delete [] (a); (a)=NULL; }
#define SAFE_RELEASE(a)			if((a))	{ (a)->Release(); (a)=NULL; }
#define START_STRING_COMPARE(src)	{	char* _compare_src_ = src;	if(0)	{
#define COMPARE(des)				}	else if(strcmp(_compare_src_,des)==0)	{	
#define END_COMPARE					}	}

#define EVENT_TYPE(a)			switch(a)	{	case -1:	{
#define EVENT_END				}	return;		}
#define EVENTOF(a)				}	return;		case (a):	{


#define RGBA_MAKE(r, g, b, a)   ((DWORD) (((a) << 24) | ((r) << 16) | ((g) << 8) | (b)))
#define RGBA_MERGE(rgb, a)		((DWORD) (((a) << 24) | (rgb)))
#define  _PlayerAI_Config_ _T(".\\NewPackingTool.cfg")
#define SAVED_XML_FILE _T(".\\test.config.xml")
#define TOTAL_STYLE 16
#define GET_BACK_COLOR(c) theApp.GetGameAI()->sPage1.fontBackColor[c]
#define GET_FORE_COLOR(c) theApp.GetGameAI()->sPage1.fontTextColor[c]


#define REGNAMED	_T("TrinityMs By JACK")
#define REGFOLDER	_T("Init")
#define REGTHEME	_T("Theme")
#define MAX_NAME_LENGTH				256

#define MIN_WINDOW_SIZE_X 800
#define MIN_WINDOW_SIZE_Y 600

#define TT GetLineMsg(__LINE__,__FUNCSIG__, __FILE__);

#define GETFILEEXTENSION 42

#define FOLDER_PATH_T _T("//")
#define FOLDER_PATH_L L"//"

#define FAST_SWITCH(a,b) case a: b; break;
#define FAST_SWITCH_RETURN(a,b,c) case a: b; return c;

const UINT WM_REFRESHBUTTONSTATES = WM_APP + 1;
const UINT WM_REFRESHBUTTONSTATES_EOL = WM_APP + 2;
const UINT WM_CONTROLPLUSF = WM_APP + 3;
const UINT WM_CONTROLCLEARLIST = WM_APP + 4;

#ifdef _USRDLL
#define AFXMANAGESTATE AfxGetStaticModuleState
#else
#define AFXMANAGESTATE AfxGetAppModuleState
#endif


#endif