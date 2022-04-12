#pragma once

#include <windows.h>
#include <Aclapi.h>
#include <stdlib.h>

LPVOID install();
DWORD createKey(LPCWSTR registryPath, LPWSTR registryName, LPWSTR value);
DWORD ProtectProcess(void);
