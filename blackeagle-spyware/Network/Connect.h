#pragma once

#include <stdio.h>
#include <windows.h>
#include <winhttp.h>
#include <errhandlingapi.h>
#include <string>
#include "../DataManagement/files.h"

#define HOST L"localhost"
#define PORT_NUM 8010

enum class RequestType {
	WO_DATA,
	BOUNDARY_ONLY,
	MULTIPART
};

typedef struct {
	DWORD dwStatusCode;
	LPSTR response;
} ResponseData;

typedef struct {
	RequestType type;
	char* part1, *part2;
	char* filename;
} RequestData;

ResponseData sendRequest(LPCWSTR additionalHeaders, RequestData* data, LPCWSTR apiUrl, LPCWSTR method);
BOOL closeSessionHandle();