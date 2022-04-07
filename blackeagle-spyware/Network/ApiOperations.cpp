#include "ApiOperations.h"


wstring getUID() {
	string uid = itemFromJson(FALSE, AUTH_FILE_NAME, USER).find("uid")->second;
	std::wstring wuid = std::wstring(uid.begin(), uid.end()); // convert to wstring
	return wuid;
}

ResponseData authenticateDevice() {
	LPCWSTR additionalHeaders = L"Accept:application/json\r\nContent-Type:application/json\r\n\r\n";
	size_t size = 0;
	char* authenticationDetails = fileToString(AUTH_FILE_NAME, "r", &size);
	LPCWSTR apiUrl = L"/login";
	LPCWSTR method = L"POST";
	RequestData rd;
	rd.part1 = authenticationDetails;
	rd.type = RequestType::BOUNDARY_ONLY;
	return sendRequest(additionalHeaders, &rd, apiUrl, method);
}

ResponseData downloadFile(DownloadType dt)
{
	LPCWSTR additionalHeaders = L"Accept:application/json\r\n\r\n";
	LPCWSTR method = L"GET";
	wstring apiUrl;

	switch (dt)
	{
	case CONFIGURATION:
		apiUrl = L"/data/get/" + getUID() + L"/CONFIGURATION" + L"@" + getUID();
		break;
	case REQUESTS:
		apiUrl = L"/data/getAll/" + getUID() + L"/REQUEST";
		break;
	default:
		break;
	}
	RequestData rd;
	rd.type = RequestType::WO_DATA;
	return sendRequest(additionalHeaders, &rd, apiUrl.c_str(), method);
}

ResponseData uploadFile(char* filename, char* json) {
	LPCWSTR method = L"POST";
	wstring apiUrl = L"/data/upload";
	LPCWSTR additionalHeaders = L"Accept: application/json\r\nContent-Type: multipart/form-data; boundary=--346435246262465368257857\r\n";

	string part1str,part2str; 
	part1str.append(
		"----346435246262465368257857\r\n"
		"Content-Disposition: form-data; name=\"file\"; filename=\"")
		.append(filename)
		.append("\"\r\nContent-Type: application/octet-stream\r\n\r\n");
	//text/plain
	part2str.append(
		"\r\n----346435246262465368257857\r\n"
		"Content-Disposition: form-data; name=\"newData\"\r\n"
		"Content-Type: application/json\r\n\r\n")
		.append(json)
		.append("\r\n----346435246262465368257857--\r\n"); 
	
	char* part1 = (char*) malloc(sizeof(char) * (part1str.length() + 1));
	char* part2 = (char*) malloc(sizeof(char) * (part2str.length() + 1));

	strcpy(part1, part1str.c_str()); 
	strcpy(part2, part2str.c_str());

	RequestData rd;

	char fullPath[256];
	snprintf(fullPath, sizeof(fullPath), "%s%s", "temp/", filename);

	rd.filename = fullPath;
	rd.part1 = part1;
	rd.part2 = part2;
	rd.type = RequestType::MULTIPART;

	ResponseData response = sendRequest(additionalHeaders, &rd, apiUrl.c_str(), method);

	free(part1);
	free(part2);
	return response;
}
