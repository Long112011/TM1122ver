








#include "stdafx.h"
#include "../4DyuchiGRX_Common/stdafx.h"
#include "MouseLib.h"
#include "../4DyuchiGXGFunc/global.h"

// ½ºÅ©¸°»óÀÇ x,yÁÂÇ¥¸¦ ½ÇÁ¦ ÁÂÇ¥ÀÇ x,0,z·Î ¹Ù²Û´Ù. y´Â ¾ðÁ¦³ª 0
VECTOR3		GetXYZFromScreenXY( I4DyuchiGXGeometry* pGeometry, DWORD dwX, DWORD dwY, DWORD dwScreenX, DWORD dwScreenY)
{

	VECTOR3	Result;

    VECTOR3 vPickRayDir;

    VECTOR3 vPickRayOrig;

    // Get the pick ray from the mouse position
        MATRIX4 matProj;
//        m_pd3dDevice->GetTransform( D3DTS_PROJECTION, &matProj );
		pGeometry->GetProjectionMatrix( &matProj,0);

//        POINT ptCursor;		ÀÌ°Å ¾²´Â°Ô ÆíÇÒ·Á³ª...
//        GetCursorPos( &ptCursor );
//	      ScreenToClient( m_hWnd, &ptCursor );

        // Compute the vector of the pick ray in screen space
        VECTOR3 v;


        v.x =  ( ( ( 2.0f * (float)dwX ) / (float)dwScreenX ) - 1 ) / matProj._11;
        v.y = -( ( ( 2.0f * (float)dwY ) / (float)dwScreenY ) - 1 ) / matProj._22;
        v.z =  1.0f;

        // Get the inverse view matrix
        MATRIX4 matView, m;
		pGeometry->GetViewMatrix( &matView,0);
        SetInverseMatrix(&m,&matView );

        // Transform the screen space pick ray into 3D space
        vPickRayDir.x  = v.x*m._11 + v.y*m._21 + v.z*m._31;
        vPickRayDir.y  = v.x*m._12 + v.y*m._22 + v.z*m._32;
        vPickRayDir.z  = v.x*m._13 + v.y*m._23 + v.z*m._33;
        vPickRayOrig.x = m._41;
        vPickRayOrig.y = m._42;
        vPickRayOrig.z = m._43;

		VECTOR3	From, To;
		From.x	=	vPickRayOrig.x;
		From.y	=	vPickRayOrig.y;

		From.z	=	vPickRayOrig.z;
		To.x	=	From.x + vPickRayDir.x;


		To.y	=	From.y + vPickRayDir.y;
		To.z	=	From.z + vPickRayDir.z;



		// x,z Æò¸é°ú ¸¸³ª´Â Á¡Àº ¾îµðÀÎ°¡.?
		{
			// ÀÏ´Ü Á÷¼±°ú Æò¸éÀÌ ÆòÇàÇÏ¸é ¿¡·¯.
//			VECTOR3	PlaneUp;
//			PlaneUp.x = 0;
//			PlaneUp.y = 1;
//			PlaneUp.z = 0;
//			if( vPickRayDir.y * PlaneUp.y == 0)
			if( vPickRayDir.y == 0)
			{
				// Æò¸é°ú º¸´Â Á÷¼±ÀÌ ÆòÇàÇÏ´Ù.
//				_asm int 3;

				Result.x = 1000000.0f;
				Result.y = 1000000.0f;
				Result.z = 1000000.0f;
				return	Result;

			}
			
			float	t = (-1*From.y) / (To.y - From.y);		// Á÷¼± ¹æÁ¤½Ä¿¡¼­ y°¡ 0ÀÏ¶§ t°ª.
			float	x = From.x+t * (To.x-From.x);
			float	z = From.z+t * (To.z-From.z);
			Result.x	=	x;
			Result.y	=	0;
			Result.z	=	z;
		}

		return	Result;

}

VECTOR3		GetXYZFromScreenXY2( I4DyuchiGXGeometry* pGeometry, DWORD dwX, DWORD dwY, DWORD dwScreenX, DWORD dwScreenY, float height)

{

	VECTOR3	Result;

    VECTOR3 vPickRayDir;
    VECTOR3 vPickRayOrig;

    // Get the pick ray from the mouse position
        MATRIX4 matProj;
//        m_pd3dDevice->GetTransform( D3DTS_PROJECTION, &matProj );
		pGeometry->GetProjectionMatrix( &matProj,0);

//        POINT ptCursor;		ÀÌ°Å ¾²´Â°Ô ÆíÇÒ·Á³ª...
//        GetCursorPos( &ptCursor );
//	      ScreenToClient( m_hWnd, &ptCursor );

        // Compute the vector of the pick ray in screen space
        VECTOR3 v;
        v.x =  ( ( ( 2.0f * (float)dwX ) / (float)dwScreenX ) - 1 ) / matProj._11;


        v.y = -( ( ( 2.0f * (float)dwY ) / (float)dwScreenY ) - 1 ) / matProj._22;
        v.z =  1.0f;

        // Get the inverse view matrix
        MATRIX4 matView, m;
		pGeometry->GetViewMatrix( &matView,0);
        SetInverseMatrix(&m,&matView );

        // Transform the screen space pick ray into 3D space
        vPickRayDir.x  = v.x*m._11 + v.y*m._21 + v.z*m._31;
        vPickRayDir.y  = v.x*m._12 + v.y*m._22 + v.z*m._32;
        vPickRayDir.z  = v.x*m._13 + v.y*m._23 + v.z*m._33;
        vPickRayOrig.x = m._41;
        vPickRayOrig.y = m._42;
        vPickRayOrig.z = m._43;

		VECTOR3	From, To;
		From.x	=	vPickRayOrig.x;
		From.y	=	vPickRayOrig.y;
		From.z	=	vPickRayOrig.z;
		To.x	=	From.x + vPickRayDir.x;
		To.y	=	From.y + vPickRayDir.y;
		To.z	=	From.z + vPickRayDir.z;


		// x,z Æò¸é°ú ¸¸³ª´Â Á¡Àº ¾îµðÀÎ°¡.?
		{
			// ÀÏ´Ü Á÷¼±°ú Æò¸éÀÌ ÆòÇàÇÏ¸é ¿¡·¯.
//			VECTOR3	PlaneUp;

//			PlaneUp.x = 0;
//			PlaneUp.y = 1;
//			PlaneUp.z = 0;
//			if( vPickRayDir.y * PlaneUp.y == 0)
			if( vPickRayDir.y == 0)
			{
				// Æò¸é°ú º¸´Â Á÷¼±ÀÌ ÆòÇàÇÏ´Ù.
//				_asm int 3;
				Result.x = 1000000.0f;
				Result.y = 1000000.0f;

				Result.z = 1000000.0f;
				return	Result;
			}
			
			float	t = (-1*(height+From.y)) / (To.y - From.y);		// Á÷¼± ¹æÁ¤½Ä¿¡¼­ y°¡ 0ÀÏ¶§ t°ª.
			float	x = From.x+t * (To.x-From.x);
			float	z = From.z+t * (To.z-From.z);
			Result.x	=	x;
			Result.y	=	0;
			Result.z	=	z;
		}

		return	Result;

}


void GetScreenXYFromXYZ( I4DyuchiGXGeometry* pGeometry, DWORD dwViewportIndex, VECTOR3* pInWhere, VECTOR3* pOutScreen)
{
	MATRIX4	proj, view, viewProj;


	pGeometry->GetProjectionMatrix( &proj, dwViewportIndex);

	pGeometry->GetViewMatrix(&view, dwViewportIndex);

	MatrixMultiply2( &viewProj, &view, &proj);			// view * projection 


	VECTOR4	where4, screen4;

	TransformV3TOV4(&where4,pInWhere,&viewProj,1);

	screen4.x = ( 1.0f + (where4.x / where4.w) ) / 2.0f;
	screen4.y = ( 1.0f - (where4.y / where4.w) ) / 2.08f;	//글�?크기가 커진 관계로 비율 수정
	screen4.z = where4.z / where4.w;

//	screen4.w = 1.0f / where4.w;


	pOutScreen->x	=	screen4.x;
	pOutScreen->y	=	screen4.y;
	pOutScreen->z	=	screen4.z;


	_asm nop;

}




