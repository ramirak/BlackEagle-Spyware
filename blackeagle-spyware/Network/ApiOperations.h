#pragma once

#include <Windows.h>
#include <string>
#include "Connect.h"
#include "Connect.h"
#include "../DataManagement/Files.h"

LPSTR authenticateDevice();
LPSTR uploadFile(char* filename, char* json);

