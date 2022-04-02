#include "Filter.h"


DWORD setFilters(LPCWSTR FilterType, LPCWSTR AdditionalBlockedSites)
{
	HANDLE hFile;
	DWORD dBytesWritten;
	DWORD error;

	std::wstring path = L"%WinDir%/System32/Drivers/Etc/";
	std::wstring pathWithFile = path + L"hosts";
	
	// Delete old hostfile
	if (!DeleteFile(pathWithFile.c_str()))
		return 0;

	std::wstring pathW(DEFAULT_FILTER_PATH);
	std::wstring fullPathW = pathW + L"/" + FilterType + L"/hosts";
	LPCWSTR fullPathFinal = fullPathW.c_str();

	// Replace the old file with the chosen filtered one 
	if (!CopyFile(fullPathFinal, path.c_str(), FALSE)) {
		error = GetLastError();
		return 0;
	}

	// Try appending the addional sites to the new hostfile
	if ((hFile = CreateFile(fullPathFinal, FILE_GENERIC_WRITE, 0, NULL,
		OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL)) == INVALID_HANDLE_VALUE
		&& (GetLastError() == ERROR_FILE_NOT_FOUND))
	{
		if (!WriteFile(hFile, AdditionalBlockedSites, lstrlen(AdditionalBlockedSites), &dBytesWritten, NULL))
		{
			error = GetLastError();
			return 0;
		}
	}
	else {
		error = GetLastError();
		return 0;
	}
	return 1;
}







