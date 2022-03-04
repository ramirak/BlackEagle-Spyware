#include "ApiOperations.h"

#define FILE_NAME "auth.json"

LPSTR authenticateDevice() {
	LPCWSTR additionalHeaders = L"Accept:application/json\r\nContent-Type:application/json\r\n\r\n";
	size_t size = 0;
	char* loginInfo = fileToString(FILE_NAME, "r", &size);
	LPCWSTR apiUrl = L"/login";
	LPCWSTR method = L"POST";
	RequestData rd;
	rd.part1 = loginInfo;
	rd.type = RequestType::BOUNDARY_ONLY;
	return sendRequest(additionalHeaders, &rd, apiUrl, method);
}

LPSTR uploadFile(char* filename, char* json) {
	char part1[] =
		"----346435246262465368257857\r\n"
		"Content-Disposition: form-data; name=\"file\"; filename=\"img.png\"\r\n"
		"Content-Type: image/png\r\n\r\n";
	char part2[] =
		"\r\n----346435246262465368257857\r\n"
		"Content-Disposition: form-data; name=\"newData\"\r\n"
		"Content-Type: application/json\r\n\r\n"
		"{\"dataType\":\"DEVICE_SETTINGS\"}"
		"\r\n----346435246262465368257857--\r\n";

	LPCWSTR method = L"POST";
	LPCWSTR apiUrl = L"/data/add/c424432e-cee1-45df-a3ed-20cd408dd56d";
	LPCWSTR additionalHeaders = L"Accept: application/json\r\nContent-Type: multipart/form-data; boundary=--346435246262465368257857\r\n";

	RequestData rd;
	rd.filename = filename;
	rd.part1 = part1;
	rd.part2 = part2;
	rd.type = RequestType::MULTIPART;
	return sendRequest(additionalHeaders, &rd, apiUrl, method);
}
/*
DWORD sendItem(LPVOID lpParam, LPSTR itemType)
{
	LPSTR success = NULL;
	LPCWSTR deviceID = L"1aa46bc6-4f58-4027-9980-c82badac16c9"; // TODO : Get real device ID
	while (success == NULL) {
		//Data newItem;
		//newItem.dataType = itemType;
		//newItem.dataAttributes = (char*)lpParam;
		//char* myJson = (char*) jsonFromItem(&newItem, DATA);

		LPCWSTR additionalHeaders = L"Accept:application/json\r\nContent-Type:application/json\r\n\r\n";
		//char* postData = myJson;

		std::wstring mywstring(deviceID);
		std::wstring concatted_stdstr = L"/data/add/" + mywstring;
		LPCWSTR apiUrl = concatted_stdstr.c_str();

		LPCWSTR method = L"POST";

		//success = SendRequest(additionalHeaders, postData, apiUrl, method);
	}
	return 0;
}

DWORD getItem(LPVOID lpParam, LPSTR itemType)
{
	LPSTR success = NULL;
	LPCWSTR deviceID = L"1aa46bc6-4f58-4027-9980-c82badac16c9"; // TODO : Get real device ID
	while (success == NULL) {

		LPCWSTR additionalHeaders = L"Accept:application/json\r\n\r\n";

		std::wstring mywstring(deviceID);
		std::wstring concatted_stdstr = L"/data/get/" + mywstring + L"/settings";
		LPCWSTR apiUrl = concatted_stdstr.c_str();

		LPCWSTR method = L"GET";

//		success = SendRequest(additionalHeaders, NULL, apiUrl, method);
		// Save response to disk
	}
	return 0;
}
*/