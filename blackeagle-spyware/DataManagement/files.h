#pragma once
#include <windows.h>
#include <WinDef.h>
#include <string.h>
#include <stdio.h>

#pragma warning(disable:4996) // Disable "unsafe" warnings


#define ROUND_UP_SIZE(Value,Pow2) ((SIZE_T) ((((ULONG)(Value)) + (Pow2) - 1) & (~(((LONG)(Pow2)) - 1))))
#define ROUND_UP_PTR(Ptr,Pow2)  ((void *) ((((ULONG_PTR)(Ptr)) + (Pow2) - 1) & (~(((LONG_PTR)(Pow2)) - 1))))


DWORD writeToFile(wchar_t buffer[], LPCWSTR filename);
char* fileToString(const char* fileName);