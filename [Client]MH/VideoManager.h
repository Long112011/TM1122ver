#if !defined(AFX_VIDEOMGR_H__3565EDD2_97D4_4E4F_8535_23A911D95094__INCLUDED_)
#define AFX_VIDEOMGR_H__3565EDD2_97D4_4E4F_8535_23A911D95094__INCLUDED_

#include <vfw.h>   

class  VideoManager
{
public:

	VideoManager();

	virtual ~VideoManager();

	BOOL  CreateAVIFile(char * pAviName); 

private:

	PAVIFILE	pf	; // AVI File.
	PAVISTREAM  ps	; // AVI Stream.
	PAVISTREAM  psCompressed	; // AVI Stream for Commpression.
	HRESULT	hr	; 
	BOOL	br	;
};

#endif