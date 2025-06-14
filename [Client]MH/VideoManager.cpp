
#include "StdAfx.h"
#include "VideoManager.h" 
#pragma comment(lib,"Vfw32.lib") 

VideoManager::VideoManager()
{
	pf=NULL	; 
	ps=NULL	; 
	PAVISTREAM  psCompressed=NULL; 
	hr	=NULL; 
	BOOL	br=TRUE;
	AVIFileInit();  //liuwei ≥ı ºªØAVIø‚ 2018-01-08
}

VideoManager::~VideoManager()
{

}

BOOL VideoManager::CreateAVIFile(char * pAviName)
{
	hr = AVIFileOpen(&pf, pAviName, OF_WRITE | OF_CREATE, NULL); 

	if (hr != 0)
	{
		return FALSE;
	}
	return TRUE;
}