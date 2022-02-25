
#include "files.h"

DWORD writeToFile(wchar_t buffer[], LPCWSTR filename) {

    HANDLE hFile;
    DWORD dwBytesToWrite = (DWORD)(wcslen(buffer)) * sizeof(wchar_t);
    DWORD dwBytesWritten = 0;
    BOOL bErrorFlag = FALSE;


    hFile = CreateFile(filename,                // name of the write
        FILE_APPEND_DATA,         // open for writing
        FILE_SHARE_READ,          // allow multiple readers
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
        buffer,      // start of data to write
        dwBytesToWrite,  // number of bytes to write
        &dwBytesWritten, // number of bytes that were written
        NULL);            // no overlapped structure

    if (FALSE == bErrorFlag)
    {
        //DisplayError(TEXT("WriteFile"));
        // printf("Terminal failure: Unable to write to file.\n");
    }
    else
    {
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

char* fileToString(const char* fileName)
{
    char* str;
    FILE* fp = fopen(fileName, "r");
    if (fp == NULL)
        return NULL;
    fseek(fp, 0, SEEK_END);
    size_t file_size = ftell(fp);
    str = (char*)malloc(sizeof(char) * file_size);
    fseek(fp, 0, SEEK_SET);
    fread(str, file_size, 1, fp);
    fclose(fp);
    return str; 
}
