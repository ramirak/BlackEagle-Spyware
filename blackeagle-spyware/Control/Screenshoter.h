#pragma once

#include <windows.h>
#include <iostream>

// x86_64-w64-mingw32-g++ screenshot2.c -o screen2.exe -lgdi32 -static-libgcc -static-libstdc++

int SaveToFile(HBITMAP hBitmap3, LPCTSTR lpszFileName);
int screenCapture(int x, int y, int w, int h, LPCSTR fname);
