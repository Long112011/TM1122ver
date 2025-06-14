#define ZOOMRATE -1.0f
#define FLYRATE -1.0f
const float K_ZOOM_MIN_VALUE = 300.0f;
BOOL bHasil=FALSE;
void CameraWheelZoom(CKeyboard* Keyboard,CMouse* Mouse)
{
	/*BOOL bResult = FALSE;
	PTRLISTPOS pos = WINDOWMGR->m_pWindowList->GetHeadPosition();

	for(cWindow* pWnd = (cWindow*)WINDOWMGR->m_pWindowList->GetNext(pos);
		0 < pWnd;
		pWnd = (cWindow*)WINDOWMGR->m_pWindowList->GetNext(pos))
	{
		if(FALSE == pWnd->IsActive())
		{
			continue;
		}
		else if(pWnd->PtInWindow(MOUSE->GetMouseX(), MOUSE->GetMouseY()))
		{
			bResult = TRUE ;
			break;
		}
	}

	if(bHasil)
		return;*/
	cWindow* pWnd = NULL;
	cObject* pParentWnd = NULL;
	PTRLISTPOS pos = WINDOWMGR->m_pWindowList->GetHeadPosition();
	BOOL bResult = FALSE;

	while (pWnd = (cWindow*)WINDOWMGR->m_pWindowList->GetNext(pos))
	{
		if (pWnd)
		{			
			pParentWnd = pWnd;
			while (pParentWnd->GetParent())
			{
				pParentWnd = pParentWnd->GetParent();
			}

			if (pParentWnd->IsActive() && pWnd->IsActive())
			{
				LONG xPos = Mouse->GetMouseX();
				LONG yPos = Mouse->GetMouseY();

				if (pWnd->PtInWindow(xPos, yPos))
				{
					bResult = TRUE;
				}
			}
		}
	}

	//if(FALSE == bHasil)
	if (!bResult)
	{
		float Wheel = static_cast<float>(Mouse->GetWheel());
		if (Wheel)
		{
			Wheel = -Mouse->GetWheel() / abs(Mouse->GetWheel()) * (K_ZOOM_MIN_VALUE * 2.0f);
			//CAMERA->MouseZoom(Wheel);
			if (CAMERA->GetCameraMode() == eCM_EyeView)
			{
				CAMERA->Fly(
					Wheel);
			}
			else
			{
				CAMERA->ZoomDistance(
					0,
					Wheel,
					0);
			}
		}
		
	}
}
