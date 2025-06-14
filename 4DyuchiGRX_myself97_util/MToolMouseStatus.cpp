







// ToolMouseStatus.cpp: implementation of the CToolMouseStatus class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
//#include "4DyuchiGXMapEditor.h"

#include "MToolMouseStatus.h"
#include "../4dyuchigrx_common/typedef.h"
#include "../4DyuchiGXGFunc/global.h"

//#include "tool.h"

//extern CTool* g_pTool;


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


//	¸¶¿ì½º ÀÌº¥Æ®.
#define		MOUSE_NONE					0
#define		MOUSE_ROTATE				1
#define		MOUSE_MOVEHORIZON			2
#define		MOUSE_MOVEVERTICAL			3
#define		MOUSE_ROTATE_WITH_PIVOT		4

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

MToolMouseStatus::MToolMouseStatus()

{

}

MToolMouseStatus::~MToolMouseStatus()
{

}


BOOL MToolMouseStatus::Initialize( I4DyuchiGXGeometry* pGeometry)
{
	mfMoveHorizonStep		=	1.0f;
	mfMoveVerticalStep		=	10.0f;
//	fRotateStep		=	0.2f;	// Shift¿¡ ÀÇÇØ »óÅÂ°¡ ´Þ¶óÁö´Â È¸Àü, ÀÌµ¿ ¼Óµµ°ª.
	mfRotateStep		=	0.2f * PI / 180.0f;	// Shift¿¡ ÀÇÇØ »óÅÂ°¡ ´Þ¶óÁö´Â È¸Àü, ÀÌµ¿ ¼Óµµ°ª.
	mpGeometry = pGeometry;
	mbDoNotRotate		=	FALSE;

	miMouseX	=	-1;
	miMouseY	=	-1;
	miMouseZ	=	-1;

	miOldMouseX	=	-1;
	miOldMouseY	=	-1;

	mbLButtonDown	=	false;
	mbRButtonDown	=	false;
	mbMButtonDown	=	false;
	mbCtrlDown		=	false;
	mbShiftDown		=	false;		// ½¬ÇÁÆ® ¹öÆ°Àº °í¼Ó ÀÌµ¿.

	mdwMouseMode	=	MOUSE_NONE;




	return TRUE;
}
bool MToolMouseStatus::MoveMouse(DWORD dwFlag, int iMouseX, int iMouseY)
{
	if( mpGeometry == 0)
	{
		__asm int 3
		// ·£´õ·¯°¡ ¾ÆÁ÷ ¼³Á¤ ¾È‰ç´�?
	}
	if( miMouseX == -1 || miMouseY == -1)
	{
		miOldMouseX	=	iMouseX;
		miOldMouseY	=	iMouseY;
	}
	miMouseX	=	iMouseX;
	miMouseY	=	iMouseY;

	//	ÄÁÆ®·ÑÅ°.
	mbCtrlDown	=	false;
	if( dwFlag & MK_CONTROL)
	{
		mbCtrlDown	=	true;
	}
	//	½ÃÇÁÆ®Å°.
	mbShiftDown	=	false;
	if( dwFlag & MK_SHIFT)
	{
		mbShiftDown	=	true;
//		fMoveHorizonStep		=	5.0f;

//		fMoveVerticalStep		=	50.0f;
//		fRotateStep		=	0.5f;	// Shift¿¡ ÀÇÇØ »óÅÂ°¡ ´Þ¶óÁö´Â È¸Àü, ÀÌµ¿ ¼Óµµ°ª.

	}
	else
	{
//		fMoveHorizonStep		=	1.0f;
//		fMoveVerticalStep		=	10.0f;
//		fRotateStep		=	0.2f;	// Shift¿¡ ÀÇÇØ »óÅÂ°¡ ´Þ¶óÁö´Â È¸Àü, ÀÌµ¿ ¼Óµµ°ª.
	}

	//	¸¶¿ì½º ¿ÞÂÊ ¹öÆ°.
	mbLButtonDown	=	false;

	if( dwFlag & MK_LBUTTON)
	{
		mbLButtonDown	=	true;
	}
	//	¸¶¿ì½º Áß°£ ¹öÆ°.
	mbMButtonDown	=	false;
	if( dwFlag & MK_MBUTTON)
	{
		mbMButtonDown	=	true;
	}
	//	¸¶¿ì½º ¿À¸¥ÂÊ ¹öÆ°.
	mbRButtonDown	=	false;
	if( dwFlag & MK_RBUTTON)
	{
		mbRButtonDown	=	true;
	}
	return 0;
	// ¸ðµå¿¡ µû¸¥ ½ºÀ§Ä¡, ÇÔ¼ö ÄÝ.
	switch( mdwMouseMode)
	{
		case	MOUSE_NONE:
				miOldMouseX = iMouseX;
				miOldMouseY = iMouseY;
			// ¸¶¿ì½º ¿À¸¥ÂÊ ¹öÆ°¸¸ ´­¸° °æ¿ì.

			// È¸Àü ¸ðµå·Î.
			if( !mbLButtonDown && !mbMButtonDown && mbRButtonDown && !mbCtrlDown && !mbShiftDown)	
			{
				mdwMouseMode = MOUSE_ROTATE;	
			}
			// ¸¶¿ì½º ¿À¸¥ÂÊ ¹öÆ°°ú ÄÁÆ®·ÑÀÌ ´­·ÁÁ³À» °æ¿ì.

			//	¼öÆò ÀÌµ¿ ¸ðµå·Î.
			else if( !mbLButtonDown && !mbMButtonDown && mbRButtonDown && mbCtrlDown && !mbShiftDown)	
			{
				mdwMouseMode	=	MOUSE_MOVEHORIZON;
			}
			// ¸¶¿ì½º ¿À¸¥ÂÊ ¹öÆ°°ú ½ÃÇÁÆ®°¡ ´­·ÁÁø °æ¿ì.
			//	¼öÁ÷(³ôÀÌ) ÀÌµ¿ ¸ðµå·Î.
			else if( !mbLButtonDown && !mbMButtonDown && mbRButtonDown && !mbCtrlDown && mbShiftDown)	
			{
				mdwMouseMode	=	MOUSE_MOVEVERTICAL;
			}
			// ¸¶¿ì½º ¿À¸¥ÂÊ ¹öÆ°°ú ½ÃÇÁÆ®,ÄÁÆ®·ÑÀÌ µ¿½Ã¿¡ ´­·ÁÁø °æ¿ì.
			//	ÁÖ¾îÁø ÇÇº¿À» Áß½ÉÀ¸·Î È¸ÀüÇÑ´Ù.
			else if( !mbLButtonDown && !mbMButtonDown && mbRButtonDown && mbCtrlDown && mbShiftDown)	
			{
				mdwMouseMode	=	MOUSE_ROTATE_WITH_PIVOT;
			}

			return this->MouseNone();
			break;
		case	MOUSE_ROTATE:
			if( mbLButtonDown || mbMButtonDown || !mbRButtonDown || mbCtrlDown || mbShiftDown)	// ¾î¶»°Ôµç »óÅÂ°¡ ´Þ¶óÁö¸é ¸ðµå ÇØÁ¦.
			{
				mdwMouseMode	=	MOUSE_NONE;
			}
			else
			{
				return	this->MouseRotate();
			}

			break;
		case	MOUSE_MOVEHORIZON:
			if( mbLButtonDown || mbMButtonDown || !mbRButtonDown || !mbCtrlDown || mbShiftDown)	// ¾î¶»°Ôµç »óÅÂ°¡ ´Þ¶óÁö¸é ¸ðµå ÇØÁ¦.
			{
				mdwMouseMode	=	MOUSE_NONE;
			}
			else
			{
				return	this->MouseMoveHorizon();
			}
			break;
		case	MOUSE_MOVEVERTICAL:
			if( mbLButtonDown || mbMButtonDown || !mbRButtonDown || mbCtrlDown || !mbShiftDown)	// ¾î¶»°Ôµç »óÅÂ°¡ ´Þ¶óÁö¸é ¸ðµå ÇØÁ¦.
			{
				mdwMouseMode	=	MOUSE_NONE;
			}
			else
			{
				return	this->MouseMoveVertical();
			}

			break;
		case	MOUSE_ROTATE_WITH_PIVOT:
			if( mbLButtonDown || mbMButtonDown || !mbRButtonDown || !mbCtrlDown || !mbShiftDown)	// ¾î¶»°Ôµç »óÅÂ°¡ ´Þ¶óÁö¸é ¸ðµå ÇØÁ¦.

			{
				mdwMouseMode	=	MOUSE_NONE;
			}
			else
			{
				return	this->MouseRotateWithPivot();
			}
			break;
		default:
			break;
	}

return false;
}

bool MToolMouseStatus::MouseNone()
{
	// ¾Æ¹«°Íµµ ¾ÈÇÑ´Ù. -_-;
	return false;
}

bool MToolMouseStatus::MouseRotate()
{
	if( mbDoNotRotate == TRUE)	return true;		// ¸¶¿ì½º ·ÎÅ×ÀÌÆ® ±ÝÁö ¸ðµåÀÏ°æ¿ì...

	float	x = float(miMouseX - miOldMouseX) * mfRotateStep;
	float	y = float(miMouseY - miOldMouseY) * mfRotateStep;
	VECTOR3	Rotate;
	Rotate.x = -y;
	Rotate.y = x;
	Rotate.z = 0;


	mpGeometry->RotateCameraRad(&Rotate,0);

	miOldMouseX	=	miMouseX;
	miOldMouseY	=	miMouseY;

	return true;
}

bool MToolMouseStatus::MouseRotateWithPivot()
{

	if( mbDoNotRotate == TRUE)	return true;		// ¸¶¿ì½º ·ÎÅ×ÀÌÆ® ±ÝÁö ¸ðµåÀÏ°æ¿ì...
//	¿©±â

	mRotationPivot.x	=	50.0f;
	mRotationPivot.y	=	0.0f;
	mRotationPivot.z	=	0.0f;



	float	fRotateYaw		=	float(miMouseX - miOldMouseX) * mfRotateStep;
	float	fRotatePitch	=	-1.0f * float(miMouseY - miOldMouseY) * mfRotateStep;
	VECTOR3	Rotate;

	VECTOR3		NewPos, CurrentPos;
	CAMERA_DESC Desc;
	mpGeometry->GetCameraDesc( &Desc, 0);
	CurrentPos	=	Desc.v3From;

	VECTOR3		CameraAngle;

	mpGeometry->GetCameraAngleRad( &CameraAngle, 0);
/*
	if( CameraAngle.x >= (PI-0.01)/2.0f)	// Ä«¸Þ¶ó°¡ ¾Æ·¡¸¦ º¸°í ÀÖÀ» °æ¿ì.
	{
		_asm int 3;
	}
	else if( CameraAngle.x <= -1.0f * (PI-0.01)/2.0f)	// Ä«¸Þ¶ó°¡ ¾Æ·¡¸¦ º¸°í ÀÖÀ» °æ¿ì.
	{
		_asm int 3;
	}
*/
	// ÇÇº¿À» ±âÁØÀ¸·Î À§Ä¡¸¦ ¿Å±â°í,
	RotatePositionWithPivot( &NewPos, &mRotationPivot, &CurrentPos, &CameraAngle, fRotateYaw, fRotatePitch);
	mpGeometry->SetCameraPos( &NewPos, 0);


	// ¸¶¿ì½º¸¦ ·ÎÅ×ÀÌÆ®.	(À§Ä¡ ¿Å±â´Â°Í°úÀÇ ¼ø¼­´Â °ü°è ¾ø´Ù.)

	Rotate.x = fRotatePitch;
	Rotate.y = fRotateYaw;
	Rotate.z = 0;
	mpGeometry->RotateCameraRad(&Rotate,0);

	miOldMouseX	=	miMouseX;
	miOldMouseY	=	miMouseY;

	return true;
}

bool MToolMouseStatus::MouseMoveHorizon()
{
	float	x	=	float(miMouseX - miOldMouseX) * mfMoveHorizonStep * -1.0f;
	float	y	=	float(miMouseY - miOldMouseY) * mfMoveHorizonStep * -1.0f;
	
	VECTOR3	Rot;

	mpGeometry->GetCameraAngleRad(&Rot,0);

	VECTOR3	To;
	To.x	=	(-1 * x * (float)cosf(Rot.y) *10.0f + y * (float)sinf(Rot.y)*10.0f);
	To.y	=	0.0f;
	To.z	=	x * (float)sinf(Rot.y) *10.0f + y * (float)cosf(Rot.y)*10.0f;

	mpGeometry->MoveCamera( &To,0);

	miOldMouseX	=	miMouseX;
	miOldMouseY	=	miMouseY;
						
	return true;
}

bool MToolMouseStatus::MouseMoveVertical()
{
	VECTOR3		To;
	To.x	=	0;
	To.y	=	float(miMouseY - miOldMouseY) * mfMoveVerticalStep * -1;
	To.z	=	0;
	mpGeometry->MoveCamera(&To,0);

	miOldMouseX	=	miMouseX;
	miOldMouseY	=	miMouseY;

	return true;
}


void	MToolMouseStatus::DoNotRotate(void)
{
	mbDoNotRotate	=	TRUE;
}


void	MToolMouseStatus::AllowRotate(void)
{
	mbDoNotRotate	=	FALSE;
}

void	MToolMouseStatus::PushCameraAngleAndPos(DWORD dwStackNum)

{
	CAMERA_DESC		Desc;
	mpGeometry->GetCameraDesc(&Desc,0);
	mpCameraStackPos[dwStackNum]		=	Desc.v3From;
	mpfCameraStackFromTo[dwStackNum]	=	CalcDistance( &(Desc.v3From), &(Desc.v3To));
	mpGeometry->GetCameraAngleRad(&mpCameraStackAngle[dwStackNum],0);
}

void	MToolMouseStatus::PopCameraAngleAndPos(DWORD dwStackNum)
{
	VECTOR3	To	=	mpCameraStackPos[dwStackNum];
	To.z	=	To.z + mpfCameraStackFromTo[dwStackNum];
	VECTOR3 Up;
	Up.x = Up.z = 0;
	Up.y = 1;
		
	float fNear = DEFAULT_NEAR;
	float fFar = DEFAULT_FAR;
	

	mpGeometry->ResetCamera( &(mpCameraStackPos[dwStackNum]),fNear,fFar,DEFAULT_FOV,0);
//	mpGeometry->ResetCamera( &(mpCameraStackPos[dwStackNum]),fNear,5000.0f,DEFAULT_FOV,0);
	mpGeometry->SetCameraAngleRad( &(mpCameraStackAngle[dwStackNum]),0 );
}

void MToolMouseStatus::SetRotationPivot(VECTOR3 *pPivot)
{
	mRotationPivot	=	*pPivot;
}




