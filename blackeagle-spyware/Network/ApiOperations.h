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
ResponseData downloadFile();
ResponseData uploadFile(char* filename, char* json);
wstring getUID();

using namespace std;
