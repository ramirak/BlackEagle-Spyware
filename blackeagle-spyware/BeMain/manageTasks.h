#pragma once
#include <windows.h>
#include <tchar.h>
#include <strsafe.h>
#include <stdio.h>
#include "taskRoutines.h"

typedef struct MyData {
    int* args;
} MYDATA, * PMYDATA;


int runThreads();
void ErrorHandler(LPTSTR lpszFunction);
