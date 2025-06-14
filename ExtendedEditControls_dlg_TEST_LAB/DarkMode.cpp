#include <windows.h>
#include "stdafx.h"
//#include "DarkMode.h"
#include "IatHook.h"
#include "resource.h"
#include "main.h"
enum IMMERSIVE_HC_CACHE_MODE
{
	IHCM_USE_CACHED_VALUE,
	IHCM_REFRESH
};

// 1903 18362
enum PreferredAppMode
{
	Default,
	AllowDark,
	ForceDark,
	ForceLight,
	Max
};

enum WINDOWCOMPOSITIONATTRIB
{
	WCA_UNDEFINED = 0,
	WCA_NCRENDERING_ENABLED = 1,
	WCA_NCRENDERING_POLICY = 2,
	WCA_TRANSITIONS_FORCEDISABLED = 3,
	WCA_ALLOW_NCPAINT = 4,
	WCA_CAPTION_BUTTON_BOUNDS = 5,
	WCA_NONCLIENT_RTL_LAYOUT = 6,
	WCA_FORCE_ICONIC_REPRESENTATION = 7,
	WCA_EXTENDED_FRAME_BOUNDS = 8,
	WCA_HAS_ICONIC_BITMAP = 9,
	WCA_THEME_ATTRIBUTES = 10,
	WCA_NCRENDERING_EXILED = 11,
	WCA_NCADORNMENTINFO = 12,
	WCA_EXCLUDED_FROM_LIVEPREVIEW = 13,
	WCA_VIDEO_OVERLAY_ACTIVE = 14,
	WCA_FORCE_ACTIVEWINDOW_APPEARANCE = 15,
	WCA_DISALLOW_PEEK = 16,
	WCA_CLOAK = 17,
	WCA_CLOAKED = 18,
	WCA_ACCENT_POLICY = 19,
	WCA_FREEZE_REPRESENTATION = 20,
	WCA_EVER_UNCLOAKED = 21,
	WCA_VISUAL_OWNER = 22,
	WCA_HOLOGRAPHIC = 23,
	WCA_EXCLUDED_FROM_DDA = 24,
	WCA_PASSIVEUPDATEMODE = 25,
	WCA_USEDARKMODECOLORS = 26,
	WCA_LAST = 27
};

struct WINDOWCOMPOSITIONATTRIBDATA
{
	WINDOWCOMPOSITIONATTRIB Attrib;
	PVOID pvData;
	SIZE_T cbData;
};

using fnRtlGetNtVersionNumbers = void (WINAPI *)(LPDWORD major, LPDWORD minor, LPDWORD build);
using fnSetWindowCompositionAttribute = BOOL (WINAPI *)(HWND hWnd, WINDOWCOMPOSITIONATTRIBDATA*);
// 1809 17763
using fnShouldAppsUseDarkMode = bool (WINAPI *)(); // ordinal 132
using fnAllowDarkModeForWindow = bool (WINAPI *)(HWND hWnd, bool allow); // ordinal 133
using fnAllowDarkModeForApp = bool (WINAPI *)(bool allow); // ordinal 135, in 1809
using fnFlushMenuThemes = void (WINAPI *)(); // ordinal 136
using fnRefreshImmersiveColorPolicyState = void (WINAPI *)(); // ordinal 104
using fnIsDarkModeAllowedForWindow = bool (WINAPI *)(HWND hWnd); // ordinal 137
using fnGetIsImmersiveColorUsingHighContrast = bool (WINAPI *)(IMMERSIVE_HC_CACHE_MODE mode); // ordinal 106
using fnOpenNcThemeData = HTHEME(WINAPI *)(HWND hWnd, LPCWSTR pszClassList); // ordinal 49
// 1903 18362
using fnShouldSystemUseDarkMode = bool (WINAPI *)(); // ordinal 138
using fnSetPreferredAppMode = PreferredAppMode (WINAPI *)(PreferredAppMode appMode); // ordinal 135, in 1903
using fnIsDarkModeAllowedForApp = bool (WINAPI *)(); // ordinal 139

fnSetWindowCompositionAttribute _SetWindowCompositionAttribute = nullptr;
fnShouldAppsUseDarkMode _ShouldAppsUseDarkMode = nullptr;
fnAllowDarkModeForWindow _AllowDarkModeForWindow = nullptr;
fnAllowDarkModeForApp _AllowDarkModeForApp = nullptr;
fnFlushMenuThemes _FlushMenuThemes = nullptr;
fnRefreshImmersiveColorPolicyState _RefreshImmersiveColorPolicyState = nullptr;
fnIsDarkModeAllowedForWindow _IsDarkModeAllowedForWindow = nullptr;
fnGetIsImmersiveColorUsingHighContrast _GetIsImmersiveColorUsingHighContrast = nullptr;
fnOpenNcThemeData _OpenNcThemeData = nullptr;
// 1903 18362
fnShouldSystemUseDarkMode _ShouldSystemUseDarkMode = nullptr;
fnSetPreferredAppMode _SetPreferredAppMode = nullptr;

bool g_darkModeSupported = false;
bool g_darkModeEnabled = false;
DWORD g_buildNumber = 0;
extern HWND g_hWndListView;
BOOL testInit;
bool AllowDarkModeForWindow(HWND hWnd, bool allow)
{
	if (g_darkModeSupported)
		return _AllowDarkModeForWindow(hWnd, allow);
	return false;
}

bool IsHighContrast()
{
	HIGHCONTRASTW highContrast = { sizeof(highContrast) };
	if (SystemParametersInfoW(SPI_GETHIGHCONTRAST, sizeof(highContrast), &highContrast, FALSE))
		return highContrast.dwFlags & HCF_HIGHCONTRASTON;
	return false;
}
void SetTitleBarThemeColor(HWND hWnd, BOOL dark)
{
	if (g_buildNumber < 18362)
		SetPropW(hWnd, L"UseImmersiveDarkModeColors", reinterpret_cast<HANDLE>(static_cast<intptr_t>(dark)));
	else if (_SetWindowCompositionAttribute)
	{
		WINDOWCOMPOSITIONATTRIBDATA data = { WCA_USEDARKMODECOLORS, &dark, sizeof(dark) };
		_SetWindowCompositionAttribute(hWnd, &data);
	}
}
void RefreshTitleBarThemeColor(HWND hWnd)
{
	/*BOOL dark = FALSE;
	if (_IsDarkModeAllowedForWindow(hWnd) &&
		_ShouldAppsUseDarkMode() &&
		!IsHighContrast())
	{
		dark = TRUE;
	}
	if (g_buildNumber < 18362)
		SetPropW(hWnd, L"UseImmersiveDarkModeColors", reinterpret_cast<HANDLE>(static_cast<INT_PTR>(dark)));
	else if (_SetWindowCompositionAttribute)
	{
		WINDOWCOMPOSITIONATTRIBDATA data = { WCA_USEDARKMODECOLORS, &dark, sizeof(dark) };
		_SetWindowCompositionAttribute(hWnd, &data);
	}*/
	BOOL dark = FALSE;
	if (_IsDarkModeAllowedForWindow && _ShouldAppsUseDarkMode)
	{
		if (_IsDarkModeAllowedForWindow(hWnd) && _ShouldAppsUseDarkMode() && !IsHighContrast())
		{
			dark = TRUE;
		}
	}
	SetTitleBarThemeColor(hWnd, dark);
}

bool IsColorSchemeChangeMessage(LPARAM lParam)
{
	/*bool is = false;
	if (lParam && CompareStringOrdinal(reinterpret_cast<LPCWCH>(lParam), -1, L"ImmersiveColorSet", -1, TRUE) == CSTR_EQUAL)
	{
		_RefreshImmersiveColorPolicyState();
		is = true;
	}
	_GetIsImmersiveColorUsingHighContrast(IHCM_REFRESH);
	return is;*/
	bool is = false;
	if (lParam && (0 == lstrcmpi(reinterpret_cast<LPCSTR>(lParam), "ImmersiveColorSet")) && _RefreshImmersiveColorPolicyState)
	{
		_RefreshImmersiveColorPolicyState();
		is = true;
	}
	if (_GetIsImmersiveColorUsingHighContrast)
		_GetIsImmersiveColorUsingHighContrast(IHCM_REFRESH);
	return is;
}

bool IsColorSchemeChangeMessage(UINT message, LPARAM lParam)
{
	if (message == WM_SETTINGCHANGE)
		return IsColorSchemeChangeMessage(lParam);
	return false;
}

void AllowDarkModeForApp(bool allow)
{
	if (_AllowDarkModeForApp)
		_AllowDarkModeForApp(allow);
	else if (_SetPreferredAppMode)
		_SetPreferredAppMode(allow ? AllowDark : Default);
}

void FixDarkScrollBar()
{
	HMODULE hComctl = LoadLibraryExW(L"comctl32.dll", nullptr, LOAD_LIBRARY_SEARCH_SYSTEM32);
	if (hComctl)
	{
		auto addr = FindDelayLoadThunkInModule(hComctl, "uxtheme.dll", 49); // OpenNcThemeData
		if (addr)
		{
			DWORD oldProtect;
			if (VirtualProtect(addr, sizeof(IMAGE_THUNK_DATA), PAGE_READWRITE, &oldProtect))
			{
				auto MyOpenThemeData = [](HWND hWnd, LPCWSTR classList) -> HTHEME
				{
					if (wcscmp(classList, L"ScrollBar") == 0)
					{
						hWnd = nullptr;
						classList = L"Explorer::ScrollBar";
					}
					return _OpenNcThemeData(hWnd, classList);
				};

				addr->u1.Function = reinterpret_cast<ULONG_PTR>(static_cast<fnOpenNcThemeData>(MyOpenThemeData));
				VirtualProtect(addr, sizeof(IMAGE_THUNK_DATA), oldProtect, &oldProtect);
			}
		}
	}
}
void FixDarkScrollBar2()
{
	HMODULE hComctl = LoadLibraryExW(L"comctl32.dll", nullptr, LOAD_LIBRARY_SEARCH_SYSTEM32);
	if (hComctl)
	{
		auto addr = FindDelayLoadThunkInModule(hComctl, "uxtheme.dll", 49); // OpenNcThemeData
		if (addr)
		{
			DWORD oldProtect;
			if (VirtualProtect(addr, sizeof(IMAGE_THUNK_DATA), PAGE_READWRITE, &oldProtect))
			{
				auto MyOpenThemeData = [](HWND hWnd, LPCWSTR classList) -> HTHEME
				{
					if (wcscmp(classList, L"wstr") == 0)
					{
						hWnd = nullptr;
						classList = L"wstr";
					}
					return _OpenNcThemeData(hWnd, classList);
				};

				addr->u1.Function = reinterpret_cast<ULONG_PTR>(static_cast<fnOpenNcThemeData>(MyOpenThemeData));
				VirtualProtect(addr, sizeof(IMAGE_THUNK_DATA), oldProtect, &oldProtect);
			}
		}
	}
}
/*constexpr*/ bool CheckBuildNumber(DWORD buildNumber)
{
	return (buildNumber == 17763 || // 1809
		buildNumber == 18362 || // 1903
		buildNumber == 18363 || // 1909
		buildNumber == 19041 || // 2004
		buildNumber == 19042 || // 20H2
		buildNumber == 19043 || // 21H1
		buildNumber == 19044 || // 21H2
		(buildNumber > 19044 && buildNumber < 22000) || // Windows 10 any version > 21H2 
		buildNumber >= 22000);  // Windows 11 builds
}

void FlushMenuThemes()
{
	if (_FlushMenuThemes)
	{
		_FlushMenuThemes();
	}
}
bool ShouldAppsUseDarkMode()
{
	if (!_ShouldAppsUseDarkMode)
	{
		return false;
	}

	return _ShouldAppsUseDarkMode();
}
void InitDarkMode()
{
	auto RtlGetNtVersionNumbers = reinterpret_cast<fnRtlGetNtVersionNumbers>(GetProcAddress(GetModuleHandleW(L"ntdll.dll"), "RtlGetNtVersionNumbers"));
	if (RtlGetNtVersionNumbers)
	{
		DWORD major, minor;
		RtlGetNtVersionNumbers(&major, &minor, &g_buildNumber);
		g_buildNumber &= ~0xF0000000;
		if (major == 10 && minor == 0 && CheckBuildNumber(g_buildNumber))
		{
			HMODULE hUxtheme = LoadLibraryExW(L"uxtheme.dll", nullptr, LOAD_LIBRARY_SEARCH_SYSTEM32);
			if (hUxtheme)
			{
				_OpenNcThemeData = reinterpret_cast<fnOpenNcThemeData>(GetProcAddress(hUxtheme, MAKEINTRESOURCEA(49)));
				_RefreshImmersiveColorPolicyState = reinterpret_cast<fnRefreshImmersiveColorPolicyState>(GetProcAddress(hUxtheme, MAKEINTRESOURCEA(104)));
				_GetIsImmersiveColorUsingHighContrast = reinterpret_cast<fnGetIsImmersiveColorUsingHighContrast>(GetProcAddress(hUxtheme, MAKEINTRESOURCEA(106)));
				_ShouldAppsUseDarkMode = reinterpret_cast<fnShouldAppsUseDarkMode>(GetProcAddress(hUxtheme, MAKEINTRESOURCEA(132)));
				_AllowDarkModeForWindow = reinterpret_cast<fnAllowDarkModeForWindow>(GetProcAddress(hUxtheme, MAKEINTRESOURCEA(133)));

				auto ord135 = GetProcAddress(hUxtheme, MAKEINTRESOURCEA(135));
				if (g_buildNumber < 18362)
					_AllowDarkModeForApp = reinterpret_cast<fnAllowDarkModeForApp>(ord135);
				else
					_SetPreferredAppMode = reinterpret_cast<fnSetPreferredAppMode>(ord135);

				_FlushMenuThemes = reinterpret_cast<fnFlushMenuThemes>(GetProcAddress(hUxtheme, MAKEINTRESOURCEA(136)));
				_IsDarkModeAllowedForWindow = reinterpret_cast<fnIsDarkModeAllowedForWindow>(GetProcAddress(hUxtheme, MAKEINTRESOURCEA(137)));

				_SetWindowCompositionAttribute = reinterpret_cast<fnSetWindowCompositionAttribute>(GetProcAddress(GetModuleHandleW(L"user32.dll"), "SetWindowCompositionAttribute"));

				if (_OpenNcThemeData &&
					_RefreshImmersiveColorPolicyState &&
					_ShouldAppsUseDarkMode &&
					_AllowDarkModeForWindow &&
					(_AllowDarkModeForApp || _SetPreferredAppMode) &&
					_FlushMenuThemes &&
					_IsDarkModeAllowedForWindow)
				{
					g_darkModeSupported = true;

					AllowDarkModeForApp(true);
					_RefreshImmersiveColorPolicyState();

					g_darkModeEnabled = _ShouldAppsUseDarkMode() && !IsHighContrast();

					FixDarkScrollBar();
				}
			}
		}
	}
}
void SetDarkMode(bool useDark, bool fixDarkScrollbar)
{
	if (g_darkModeSupported)
	{
		AllowDarkModeForApp(useDark);
		//_RefreshImmersiveColorPolicyState();
		FlushMenuThemes();
		if (fixDarkScrollbar)
		{
			FixDarkScrollBar();
		}
		g_darkModeEnabled = ShouldAppsUseDarkMode() && !IsHighContrast();
	}
}
void SetDarkMode(HWND hWnd, bool useDark, bool fixDarkScrollbar)
{
	/*if (g_darkModeSupported)
	{
		InitDarkMode();
		//FlushMenuThemes();
		//_RefreshImmersiveColorPolicyState();		
		g_darkModeEnabled = true;// _ShouldAppsUseDarkMode() && !IsHighContrast();
	}
	else
	{
		g_darkModeEnabled = false;//
	}*/

	if(useDark)InitDarkMode();
	g_darkModeEnabled = useDark;//
	g_darkModeSupported = useDark;
	AllowDarkModeForApp(useDark);
	_AllowDarkModeForWindow(hWnd, useDark);
	FlushMenuThemes();
	if (fixDarkScrollbar)
	{
		FixDarkScrollBar();
	}
	RefreshTitleBarThemeColor(hWnd);
	//InitListView(g_hWndListView);
	//HWND hwndRoot = GetAncestor(hWnd, GA_ROOTOWNER);

	// wParam == true, will reset style and toolbar icon
	//::SendMessage(hwndRoot, NPPM_INTERNAL_REFRESHDARKMODE, static_cast<WPARAM>(!testInit), 0);
	testInit = !testInit;

	
}
void InitDarkButton(HWND hButton)
{
	int isChecked = (theApp.GetGameAI()->sPage1.DarkMode) ? BST_CHECKED : BST_UNCHECKED;

	LPCWSTR classList;
	if (theApp.GetGameAI()->sPage1.DarkMode)
	{
		classList = L"Explorer";
	}
	else
	{
		classList = L"wstr";
	}
	SetWindowTheme(hButton, L"Explorer", nullptr);
	AllowDarkModeForWindow(hButton, isChecked);
}
void SetTitleToDark(HWND hwnd)
{
	BOOL value = theApp.GetGameAI()->sPage1.DarkMode;// TRUE;
	::DwmSetWindowAttribute(hwnd,
		DWMWA_USE_IMMERSIVE_DARK_MODE,
		&value, sizeof(value));
}
void SetTitleToDark_TEST(HWND hwnd)
{
	int windowColor = 0;
	//HWND window = GetActiveWindow();
	LONG exStyle = GetWindowLong(hwnd, GWL_EXSTYLE);
	exStyle &= ~(WS_EX_DLGMODALFRAME | WS_EX_CLIENTEDGE | WS_EX_STATICEDGE);
	SetWindowLong(hwnd, GWL_EXSTYLE, exStyle);
	DwmSetWindowAttribute(hwnd, 21, &windowColor, sizeof(windowColor));
	SetWindowPos(hwnd, NULL, 0, 0, 0, 0, SWP_FRAMECHANGED | SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_NOOWNERZORDER);
}