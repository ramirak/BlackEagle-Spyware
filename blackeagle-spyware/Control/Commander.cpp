#include "Commander.h"

int runCommand(LPCWSTR cmd, LPCWSTR filename) {
	if (cmd != L"tasklist" &&
		cmd != L"taskkill" &&
		cmd != L"tree") {
		return 0; // Only these commands are allowed
	}
	// Output to a new file
	LPCWSTR o_cmd = L" > ";

	wstring stdResult = L"/C" + wstring(cmd) + o_cmd + filename;
	LPCWSTR final_cmd = stdResult.c_str();
	HINSTANCE res = ShellExecute(0, L"open", L"cmd.exe", final_cmd, 0, SW_HIDE);

	return ((INT_PTR)res) > 32;
}