
//////////////////////////////////////////////////////////////////////////
// ¸¶¿ì½º ¿À¸¥ÂÊ ¹öÆ°ÀÌ ´­·ÈÀ»¶§ ½ÃÁ¡ ¹Ù²Ù±â

void CameraRotate(CKeyboard* Keyboard,CMouse* Mouse)
{
	static BOOL bLastRotating = FALSE;

	if( WINDOWMGR->IsDragWindow() )
		return;

	BOOL bCurRotating = Mouse->RButtonPressed();
	if(bCurRotating)
	{
		short x = Mouse->GetMouseAxisX();
		short y = Mouse->GetMouseAxisY();
		if(x || y)
		{
			CAMERA->MouseRotate(x,y);
		}
	}

	if(bCurRotating != bLastRotating)
	{
		bLastRotating = bCurRotating;
	}
	
//	if(Mouse->MButtonDoubleClick())
//	{
		/*
		CHero* pHero = (CHero*)EXECUTOR->GetHero();
		CDataBlock* pBlock = DATABLOCK->GetEmptyDataBlock();
		DWORD MoveCommand = eMCMD_CAMERA_ADJUST_TO_HERO;
		float angle = pHero->Move_GetCharAngleInDeg() + 180;
		*pBlock << MoveCommand;
		*pBlock << angle;
		EXECUTOR->CommandToCamera(eCMD_MOVECOMMAND_CHANGE,pBlock->GetDataBlockNum());
		*/
//	}
}