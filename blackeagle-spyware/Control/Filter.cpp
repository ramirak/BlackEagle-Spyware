#include "Filter.h"


DWORD setFilters(LPCWSTR FilterType, LPCSTR AdditionalBlockedSites)
{
	HANDLE hFile;
	DWORD dBytesWritten;
	DWORD error;

	std::wstring path = L"C:\\Windows\\System32\\drivers\\etc";
	std::wstring pathWithOriginalFile = path + L"\\hosts";
	std::wstring pathWithFile = path + L"\\hosts-temp";
	std::wstring pathW(DEFAULT_FILTER_PATH);
	std::wstring fullPathW = pathW + FilterType + L"/hosts";
	LPCWSTR fullPathFinal = fullPathW.c_str();

	// Copy or replace the old file with the chosen filtered one 
	if (!CopyFile(fullPathFinal, pathWithFile.c_str(), FALSE)) {
		error = GetLastError();
		return 0;
	}

	// Open or create the file for appending
	hFile = CreateFile(pathWithFile.c_str(), FILE_APPEND_DATA, FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE, nullptr,
		OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr);
	if (hFile == INVALID_HANDLE_VALUE) {
		return 0;
	}

	// Set the file pointer to the end-of-file:
	DWORD dwMoved = ::SetFilePointer(hFile, 0l, nullptr, FILE_END);
	if (dwMoved == INVALID_SET_FILE_POINTER) {
		return 0;
	}

	// Append additional sites to the new host file
	if (!WriteFile(hFile, AdditionalBlockedSites, strlen(AdditionalBlockedSites), &dBytesWritten, NULL))
	{
		error = GetLastError();
		return 0;
	}

	// Must close the handle before replacing
	CloseHandle(hFile);

	// Try replacing the old host with the new one.
	if (!ReplaceFile(pathWithOriginalFile.c_str(), pathWithFile.c_str(), NULL, NULL, NULL, NULL))
	{
		error = GetLastError();
		return 0;
	}
	return 1;
}






