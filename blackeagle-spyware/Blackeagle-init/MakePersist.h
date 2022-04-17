#pragma once

#include <windows.h>
#include <Aclapi.h>
#include <stdlib.h>
#include "TasksHelper.h"
#include "../DataManagement/converters.h"

LPVOID install(BOOL reg);
DWORD createKey(LPCWSTR registryPath, LPWSTR registryName, LPWSTR value);
DWORD ProtectProcess(void);
 