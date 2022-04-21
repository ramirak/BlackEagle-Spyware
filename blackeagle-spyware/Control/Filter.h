#pragma once

#include <windows.h>
#include <string>

#define DEFAULT_FILTER_PATH L"filters/"

#define FAKENEWS "FAKENEWS"
#define GAMBLING "GAMBLING"
#define PORN "PORN"
#define SOCIAL "SOCIAL"
#define ADDITIONAL_SITES "ADDITIONAL_SITES"

DWORD setFilters(LPCWSTR FilterType, LPCSTR AdditionalBlockedSites);
