#include "files.h"

DWORD writeToFile(wchar_t buffer[], LPCWSTR filename) {

    HANDLE hFile;
    DWORD dwBytesToWrite = (DWORD)(wcslen(buffer)) * sizeof(wchar_t);
    DWORD dwBytesWritten = 0;
    BOOL bErrorFlag = FALSE;

    hFile = CreateFile(filename,  // name of the write
        FILE_APPEND_DATA,         // open for writing
        0,                        // prevent other p from reading 
        NULL,                     // no security
        OPEN_ALWAYS,              // open or create
        FILE_ATTRIBUTE_NORMAL,    // normal file
        NULL);                    // no attr. template

    if (hFile == INVALID_HANDLE_VALUE)
    {
      //  DisplayError(TEXT("CreateFile"));
      //  _tprintf(TEXT("Terminal failure: Unable to open file \"%s\" for write.\n"), argv[1]);
        return -1;
    }

    bErrorFlag = WriteFile(
        hFile,           // open file handle
        buffer,          // start of data to write
        dwBytesToWrite,  // number of bytes to write
        &dwBytesWritten, // number of bytes that were written
        NULL);           // no overlapped structure

    if (FALSE == bErrorFlag)
    {
        //DisplayError(TEXT("WriteFile"));
        // printf("Terminal failure: Unable to write to file.\n");
    }
    else
    {
        DWORD err = GetLastError();
        int a = 1;
        if (dwBytesWritten != dwBytesToWrite)
        {
            
            // This is an error because a synchronous write that results in
            // success (WriteFile returns TRUE) should write all data as
            // requested. This would not necessarily be the case for
            // asynchronous writes.
          
          // printf("Error: dwBytesWritten != dwBytesToWrite\n");
        }
    }

    CloseHandle(hFile);
    return 1;
}

char* fileToString(const char *fileName, const char *mode, size_t *filesize)
{
    char* str;
    FILE* fp = fopen(fileName, mode);
    if (fp == NULL)
        return NULL;
    fseek(fp, 0, SEEK_END);
    *filesize = ftell(fp);
    str = (char*)malloc(sizeof(char) *  (*filesize));
    fseek(fp, 0, SEEK_SET);
    fread(str, *filesize, 1, fp);
    fclose(fp);
    return str; 
}
