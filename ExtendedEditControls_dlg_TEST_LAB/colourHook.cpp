#include "stdafx.h"
#include "main.h"
#include "colourHook.h"

LRESULT CALLBACK ColourStaticTextHooker::colourStaticProc(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam)
{
	switch(Message)
	{
		case WM_PAINT:
		{
			RECT rect{};
			::GetClientRect(hwnd, &rect);

			PAINTSTRUCT ps{};
			HDC hdc = ::BeginPaint(hwnd, &ps);

			::SetTextColor(hdc, _colour);

			if (theApp.GetGameAI()->sPage1.DarkMode)
			{
				::SetBkColor(hdc, RGB(34, 34, 34)/*NppDarkMode::getDarkerBackgroundColor()*/);
			}

			// Get the default GUI font
			LOGFONT lf/*{ NppParameters::getDefaultGUIFont() }*/;
			HFONT hf = ::CreateFontIndirect(&lf);

			HANDLE hOld = SelectObject(hdc, hf);

			// Draw the text!
			TCHAR text[MAX_PATH]{};
			::GetWindowText(hwnd, text, MAX_PATH);
			::DrawText(hdc, text, -1, &rect, DT_LEFT);

			::DeleteObject(::SelectObject(hdc, hOld));
			::EndPaint(hwnd, &ps);

			return TRUE;
		}
	}
	return ::CallWindowProc(_oldProc, hwnd, Message, wParam, lParam);
}