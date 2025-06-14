#pragma once

class ColourStaticTextHooker
{
public:
	ColourStaticTextHooker() : _colour(RGB(0x00, 0x00, 0x00)) {};

	COLORREF setColour(COLORREF colour2Set) {
		COLORREF oldColour = _colour;
		_colour = colour2Set;
		return oldColour;
	};
	void hookOn(HWND staticHandle) {
		::SetWindowLongPtr(staticHandle, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(this));
		_oldProc = reinterpret_cast<WNDPROC>(::SetWindowLongPtr(staticHandle, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(staticProc)));
	};
private:
	COLORREF _colour = RGB(0xFF, 0xFF, 0xFF);
	WNDPROC _oldProc = nullptr;

	static LRESULT CALLBACK staticProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam){
		ColourStaticTextHooker *pColourStaticTextHooker = reinterpret_cast<ColourStaticTextHooker *>(::GetWindowLongPtr(hwnd, GWLP_USERDATA));
		return pColourStaticTextHooker->colourStaticProc(hwnd, message, wParam, lParam);
	};
	LRESULT CALLBACK colourStaticProc(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam);
};