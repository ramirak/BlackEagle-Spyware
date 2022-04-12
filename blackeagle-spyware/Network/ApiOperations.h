#pragma once
#include <Windows.h>
#include <string>
#include "Connect.h"
#include "Connect.h"
#include <sstream>
#include <string>
#include "../DataManagement/Files.h"
#include "../DataManagement/converters.h"

#define AUTH_FILE_NAME "auth.json"

enum DownloadType {
	CONFIGURATION,
	REQUESTS
};

ResponseData authenticateDevice();
ResponseData uploadFile(char* filename, char* json);
ResponseData downloadFile(DownloadType dt);
wstring getUID();

using namespace std;
