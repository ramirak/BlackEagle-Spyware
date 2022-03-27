#pragma once

#include <windows.h>;
#include <string>

#define DEFAULT_FILTER_PATH L"/filters/"

#define FAKENEWS "fakenews"
#define GAMBLING "gambling"
#define PORN "porn"
#define SOCIAL "social"

DWORD setFilters(LPCWSTR FilterType, LPCWSTR AdditionalBlockedSites);
