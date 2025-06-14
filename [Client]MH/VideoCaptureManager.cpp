#include "stdafx.h"
#include "VideoCaptureManager.h"
#include "cWindowManager.h"
#include "ChatManager.h"
#include "VideoCaptureDlg.h"
#include "GameIn.h"
#define _GAMEDX8_
#ifdef _GAMEDX8_
#include "d3d8.h"
#endif
#ifdef _GAMEDX9_
#include "d3d9.h"
#endif
CVideoCaptureManager::CVideoCaptureManager()
{
}
CVideoCaptureManager::~CVideoCaptureManager()
{
}
BOOL CVideoCaptureManager::IsCapturing()
{
	return m_bandiCaptureLibrary.IsCapturing();
}
BOOL CVideoCaptureManager::CaptureStart()
{
	if (m_bandiCaptureLibrary.IsCapturing())
	{
		CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(1993));
		return FALSE;
	}
#ifdef _GAMEDX8_
	void* pd3d8Device = NULL;
	if (!g_pExecutive->GetRenderer()->GetD3DDevice(IID_IDirect3DDevice8, &pd3d8Device))
	{
		return FALSE;
	}
#endif
#ifdef _GAMEDX9_
	void* pd3d9Device = NULL;
	if (!g_pExecutive->GetRenderer()->GetD3DDevice(IID_IDirect3DDevice9, &pd3d9Device))
	{
		return FALSE;
	}
#endif
	if (m_bandiCaptureLibrary.IsCreated() == FALSE)
	{
		if (FAILED(m_bandiCaptureLibrary.Create(BANDICAP_RELEASE_DLL_FILE_NAME)))
		{
			CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(1994));
			return FALSE;
		}
		if (FAILED(m_bandiCaptureLibrary.Verify("EYA_LUNAPLUS_20100218", "e5b68af6")))
		{
		}
	}
	if (m_bandiCaptureLibrary.IsCreated())
	{
		BCAP_CONFIG cfg;
		m_bandiCaptureLibrary.SetMinMaxFPS(15, 320);
		SetCaptureOpt();
		char strDirectory[MAX_PATH];
		sprintf(strDirectory, "%sScreenShot", DIRECTORYMGR->GetFullDirectoryName(eLM_Root));
		if (!::CreateDirectory(strDirectory, NULL) &&
			GetLastError() != ERROR_ALREADY_EXISTS)
		{
			//CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg( 2233 ) );		
			return FALSE;
		}
		TCHAR pathName[MAX_PATH];
		m_bandiCaptureLibrary.MakePathnameByDate(strDirectory, _T("Capture"), _T("mp4"), pathName, MAX_PATH);
		CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(2417), pathName);
#ifdef _GAMEDX8_
		HRESULT hr = m_bandiCaptureLibrary.Start(pathName, NULL, BCAP_MODE_D3D8, (LONG_PTR)pd3d8Device);
		if (FAILED(hr))
			return FALSE;
#endif
#ifdef _GAMEDX9_
		HRESULT hr = m_bandiCaptureLibrary.Start(pathName, NULL, BCAP_MODE_D3D9, (LONG_PTR)pd3d9Device);
		if (FAILED(hr))
			return FALSE;
#endif
	}
	return TRUE;
}
void CVideoCaptureManager::CaptureStop()
{
	m_bandiCaptureLibrary.Stop();
	CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(2414));
}
void CVideoCaptureManager::Process()
{
	if (!m_bandiCaptureLibrary.IsCapturing())
		return;
#ifdef _GAMEDX8_
	void* pd3d8Device = NULL;
	if (!g_pExecutive->GetRenderer()->GetD3DDevice(IID_IDirect3DDevice8, &pd3d8Device))
	{
		return;
	}
	m_bandiCaptureLibrary.Work((LONG_PTR)pd3d8Device);
#endif
#ifdef _GAMEDX9_
	void* pd3d9Device = NULL;
	if (!g_pExecutive->GetRenderer()->GetD3DDevice(IID_IDirect3DDevice9, &pd3d9Device))
	{
		return;
	}
	m_bandiCaptureLibrary.Work((LONG_PTR)pd3d9Device);
#endif
	
}
void CVideoCaptureManager::SetCaptureOpt()
{
	CVideoCaptureDlg* pVideoCaptureDlg = GAMEIN->GetVideoCaptureDlg();
	if (!pVideoCaptureDlg)
		return;
	BCAP_CONFIG cfg;
	m_bandiCaptureLibrary.GetConfig(&cfg);
	switch ((CVideoCaptureDlg::eVideoCaptureSize)pVideoCaptureDlg->GetSelectedSize())
	{
	case CVideoCaptureDlg::eVideoCaptureSize_Default:

	case CVideoCaptureDlg::eVideoCaptureSize_500x375:

	case CVideoCaptureDlg::eVideoCaptureSize_800x600:

	case CVideoCaptureDlg::eVideoCaptureSize_1024x768:

	case CVideoCaptureDlg::eVideoCaptureSize_1280x800:

	case CVideoCaptureDlg::eVideoCaptureSize_1920x1080:

	case CVideoCaptureDlg::eVideoCaptureSize_Max:
	{
													cfg.VideoSizeW = WINDOWMGR->GetScreenX();
													cfg.VideoSizeH = WINDOWMGR->GetScreenY();
	}
		break;
	}
	switch ((CVideoCaptureDlg::eVideoCaptureOpt)pVideoCaptureDlg->GetSelectedOpt())
	{
	case CVideoCaptureDlg::eVideoCaptureOpt_High:
	{
													cfg.VideoCodec = FOURCC_MJPG;// FOURCC_MP4V;
													cfg.VideoQuality = 90;
													cfg.VideoFPS = 320.0f;
													cfg.AudioChannels = 2;
													cfg.AudioCodec = WAVETAG_PCM;// WAVETAG_MP2;
													cfg.AudioSampleRate = 44100;
	}
		break;
	case CVideoCaptureDlg::eVideoCaptureOpt_Low:
	{
												   cfg.VideoCodec = FOURCC_MP4V;
												   cfg.VideoQuality = 90;// 70;
												   cfg.VideoFPS = 320.0f;
												   cfg.AudioChannels = 2;//1;
												   cfg.AudioCodec = WAVETAG_PCM;
												   cfg.AudioSampleRate = 44100;// 22050;
	}
		break;
	}
	cfg.VideoFPS = 320.0f;
	cfg.AdjustAudioMixer = TRUE;
	cfg.IncludeCursor = pVideoCaptureDlg->IsIncludeCursor();
	m_bandiCaptureLibrary.CheckConfig(&cfg);
	m_bandiCaptureLibrary.SetConfig(&cfg);
}
BOOL CVideoCaptureManager::GetVideoWorkStatic()
{
	if (m_bandiCaptureLibrary.IsCapturing())
	{
		return TRUE;
	}
	return FALSE;
}