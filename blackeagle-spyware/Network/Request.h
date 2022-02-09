#pragma once

#include <stdio.h>
#include <windows.h>
#include <winhttp.h>

#define HOST L"localhost"
#define PORT_NUM 8010
DWORD SendRequest(LPCWSTR additionalHeaders, char* optionalData, LPCWSTR apiUrl, LPCWSTR method);
