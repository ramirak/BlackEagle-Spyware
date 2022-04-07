#pragma once
#pragma comment(lib, "netapi32.lib")
#include <stdio.h>
#include <stdlib.h>
#include <windows.h> 
#include <time.h>
#include <lm.h>
#define PASSWORD_LEN 8

int lockUser(LPWSTR password);
char* generateRandomPass();

