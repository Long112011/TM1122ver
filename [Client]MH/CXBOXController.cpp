#include "stdafx.h"
#include "CXBOXController.h"
#include "MHTimeManager.h"
CXBOXController::CXBOXController(int playerNumber)
{
	// Set the Controller Number
	_controllerNum = playerNumber - 1;
}

XINPUT_STATE CXBOXController::GetState()
{
	// Zeroise the state
	ZeroMemory(&_controllerState, sizeof(XINPUT_STATE));

	// Get the state
	XInputGetState(_controllerNum, &_controllerState);

	return _controllerState;
}

bool CXBOXController::IsConnected()
{
	// Zeroise the state
	ZeroMemory(&_controllerState, sizeof(XINPUT_STATE));

	// Get the state
	DWORD Result = XInputGetState(_controllerNum, &_controllerState);

	if(Result == ERROR_SUCCESS)
	{
		//test = false;
		return true;
	}
	else
	{
		return false;
	}
}

void CXBOXController::Vibrate(int leftVal, int rightVal)
{
	// Create a Vibraton State
	XINPUT_VIBRATION Vibration;

	// Zeroise the Vibration
	ZeroMemory(&Vibration, sizeof(XINPUT_VIBRATION));

	// Set the Vibration Values
	Vibration.wLeftMotorSpeed = leftVal;
	Vibration.wRightMotorSpeed = rightVal;

	// Vibrate the controller
	XInputSetState(_controllerNum, &Vibration);
}
bool CXBOXController::button_pressed(DWORD dwKey)
{
	return ((_controllerState.Gamepad.wButtons & dwKey) != 0) && (_controllerState.Gamepad.wButtons == GetKeyPressed(dwKey));
}






void CXBOXController::UpdateKeyboardState()
{
	HRESULT hr;

	m_dwBufferedKeys = 32;

	DWORD Result = XInputGetState(_controllerNum, &_controllerState);

	if (Result == ERROR_SUCCESS)
	{
		m_dwBufferPos = m_dwBufferedKeys;
		test == false;
	}
	else
	{
		ClearKeyState();
	}
}


DWORD CXBOXController::GetKeyPressed(DWORD dwKey)
{
	return m_KeyPress[dwKey];
}





WORD CXBOXController::GetKeyPressedTest(DWORD dwKey)
{
	if (MHTIMEMGR->GetInstance()->CheckTimer2000())
		return dwKey;
	else
		return -1;
}




BOOL CXBOXController::GetKeyDown(DWORD dwKey)
{
	if (m_dwBufferedKeys == 0) return FALSE;
	if (m_dwBufferPos > m_dwBufferedKeys || m_dwBufferPos < 1)
		return FALSE;

	if ((_controllerState.Gamepad.wButtons == dwKey) && (_controllerState.Gamepad.wButtons & dwKey))
	{
		//test = true;
		return TRUE;
	}
	test = false;
	return FALSE;
}




BOOL CXBOXController::GetKeyUp(DWORD dwKey)
{
	if (m_dwBufferedKeys == 0) return FALSE;
	if (m_dwBufferPos > m_dwBufferedKeys || m_dwBufferPos < 1)
		return FALSE;

	//ASSERT(m_dwBufferPos <= m_dwBufferedKeys && m_dwBufferPos > 0);

	if (!(_controllerState.Gamepad.wButtons & dwKey) /*&& !(_controllerState.Gamepad.wButtons & dwKey)*/)
	{
		test = false;
		return TRUE;
	}


	return FALSE;
}


void CXBOXController::ClearKeyState()
{
	m_dwBufferedKeys = 0;
	m_dwBufferPos = 1;
	test = false;
	ZeroMemory(m_KeyPress, sizeof(m_KeyPress));

}


BOOL CXBOXController::IsEmptyKeyboardEvent()
{
	if (m_dwBufferPos > m_dwBufferedKeys)
		return TRUE;

	return FALSE;

}

BOOL CXBOXController::GetKeyTest(DWORD dwKey)
{ 
	if (test==false)
		test = GetKeyDown(dwKey);
	return (test && GetKeyUp(dwKey));
}