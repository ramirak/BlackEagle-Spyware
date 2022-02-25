#pragma once

#include <windows.h>
#include <iostream>

int SaveToFile(HBITMAP hBitmap3, LPCTSTR lpszFileName);
int screenCapture(int x, int y, int w, int h, LPCSTR fname);
