#pragma once
#include <Windows.h>
#include <string>
#include "Connect.h"
#include "Connect.h"
#include <sstream>
#include <string>
#include "../DataManagement/Files.h"
#include "../DataManagement/converters.h"
ResponseData authenticateDevice();
ResponseData uploadFile(char* filename, char* json);
wstring getUID();

#define AUTH_FILE_NAME "auth.json"

enum DownloadType {
	CONFIGURATION,
	REQUESTS
};
ResponseData downloadFile(DownloadType dt);

using namespace std;
