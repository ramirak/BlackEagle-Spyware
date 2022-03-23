#pragma once
#include <windows.h>
#include <iostream>

int SaveToFile(HBITMAP hBitmap3, LPCWSTR lpszFileName);
int screenCapture(int x, int y, int w, int h, LPCWSTR fname);
