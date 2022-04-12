#include "Commander.h"

int runCommand(LPCWSTR cmd, LPCWSTR filename) {
	// Output to a new file
	LPCWSTR o_cmd = L" > ";

	wstring stdResult = L"/C " + wstring(cmd) + o_cmd + filename;
	LPCWSTR final_cmd = (stdResult.append(L" 2>&1")).c_str();
	
	//HINSTANCE res = ShellExecute(0, L"open", L"cmd.exe", final_cmd, 0, SW_HIDE);

	SHELLEXECUTEINFO ShExecInfo = { 0 };
	ShExecInfo.cbSize = sizeof(SHELLEXECUTEINFO);
	ShExecInfo.fMask = SEE_MASK_NOCLOSEPROCESS;
	ShExecInfo.hwnd = NULL;
	ShExecInfo.lpVerb = L"open";
	ShExecInfo.lpFile = L"cmd.exe";
	ShExecInfo.lpParameters = final_cmd;
	ShExecInfo.lpDirectory = NULL;
	ShExecInfo.nShow = SW_HIDE;
	ShExecInfo.hInstApp = NULL;
	ShellExecuteEx(&ShExecInfo);
	WaitForSingleObject(ShExecInfo.hProcess, 30000);

	return 1;
}