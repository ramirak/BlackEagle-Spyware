#include "Connect.h"

HINTERNET  hSession = NULL;

ResponseData sendRequest(LPCWSTR additionalHeaders, RequestData* data, LPCWSTR apiUrl, LPCWSTR method) {
	DWORD dwSize = sizeof(DWORD);
	DWORD dwDownloaded = 0;
	LPSTR pszOutBuffer = NULL;
	BOOL  bResults = FALSE;

	HINTERNET hRequest = NULL, hConnect = NULL;
	DWORD dwStatusCode = 0;
	DWORD dwSupportedSchemes;
	DWORD dwFirstScheme;
	DWORD dwLastStatus = 0;
	BOOL  bDone = FALSE;

	// Use WinHttpOpen to obtain a session handle.
	if (!hSession)
		hSession = WinHttpOpen(L"Blackeagle-Service",
			WINHTTP_ACCESS_TYPE_DEFAULT_PROXY,
			WINHTTP_NO_PROXY_NAME,
			WINHTTP_NO_PROXY_BYPASS, 0);

	// Specify an HTTP server. 
	if (hSession)
		hConnect = WinHttpConnect(hSession, HOST,
			PORT_NUM, 0);

	DWORD errorMessageID = ::GetLastError();

	// Create an HTTP request handle.
	if (hConnect)
		hRequest = WinHttpOpenRequest(hConnect, method, apiUrl,
			NULL, WINHTTP_NO_REFERER,
			WINHTTP_DEFAULT_ACCEPT_TYPES,
			WINHTTP_FLAG_SECURE);

	errorMessageID = ::GetLastError();
	ULONG flags =
		SECURITY_FLAG_IGNORE_UNKNOWN_CA |
		SECURITY_FLAG_IGNORE_CERT_DATE_INVALID |
		SECURITY_FLAG_IGNORE_CERT_CN_INVALID |
		SECURITY_FLAG_IGNORE_CERT_WRONG_USAGE;

	WinHttpSetOption(hRequest, WINHTTP_OPTION_SECURITY_FLAGS, &flags, sizeof(flags));


	DWORD logonPolicy = WINHTTP_AUTOLOGON_SECURITY_LEVEL_LOW;
	BOOL fRet = WinHttpSetOption(hRequest,
		WINHTTP_OPTION_AUTOLOGON_POLICY,
		(LPVOID*)&logonPolicy,
		sizeof(logonPolicy));

	// Send a request.
	if (data->type == RequestType::WO_DATA) { // No data to send
		if (hRequest)
			bResults = WinHttpSendRequest(hRequest,
				additionalHeaders, -1,
				WINHTTP_NO_REQUEST_DATA, 0,
				0, 0);
	}
	else if (data->type == RequestType::BOUNDARY_ONLY) {
		if (hRequest)
			bResults = WinHttpSendRequest(hRequest,
				additionalHeaders, -1,
				data->part1, strlen(data->part1),
				strlen(data->part1), 0);
	}
	else if (data->type == RequestType::MULTIPART) {
		size_t filesize = 0;
		char* fileData = fileToString(data->filename, "rb", &filesize);

		WinHttpAddRequestHeaders(hRequest, additionalHeaders, -1L, WINHTTP_ADDREQ_FLAG_ADD);

		if (hRequest)
			bResults = WinHttpSendRequest(hRequest,
				WINHTTP_NO_ADDITIONAL_HEADERS,
				0, WINHTTP_NO_REQUEST_DATA, 0,
				lstrlenA(data->part1) + lstrlenA(data->part2) + filesize, NULL);

		DWORD dwBytesWritten = 0;
		if (bResults)
			bResults = WinHttpWriteData(hRequest, data->part1, lstrlenA(data->part1), &dwBytesWritten);
		if (bResults)
			bResults = WinHttpWriteData(hRequest, (LPCVOID)fileData, filesize, &dwBytesWritten);
		if (bResults)
			bResults = WinHttpWriteData(hRequest, data->part2, lstrlenA(data->part2), &dwBytesWritten);
	}

	errorMessageID = ::GetLastError();

	// End the request.
	if (bResults)
		bResults = WinHttpReceiveResponse(hRequest, NULL);


	// Check the status code.
	if (bResults)
		bResults = WinHttpQueryHeaders(hRequest,
			WINHTTP_QUERY_STATUS_CODE |
			WINHTTP_QUERY_FLAG_NUMBER,
			NULL,
			&dwStatusCode,
			&dwSize,
			NULL);

	if (bResults)
	{
		switch (dwStatusCode) 
		{
		case 200:
			// The resource was successfully retrieved.
			// You can use WinHttpReadData to read the 
			// contents of the server's response.

			// Keep checking for data until there is nothing left.

			if (!WinHttpQueryDataAvailable(hRequest, &dwSize))
				printf("Error %u in WinHttpQueryDataAvailable.\n",
					GetLastError());

			// Allocate space for the buffer.
			pszOutBuffer = new char[dwSize + 1];
			if (!pszOutBuffer)
			{
				printf("Out of memory\n");
				dwSize = 0;
			}
			else
			{
				// Read the data.
				ZeroMemory(pszOutBuffer, dwSize + 1);

				if (!WinHttpReadData(hRequest, (LPVOID)pszOutBuffer,
					dwSize, &dwDownloaded))
					printf("Error %u in WinHttpReadData.\n", GetLastError());
				else
					printf("%s", pszOutBuffer);

				// Free the memory allocated to the buffer.
				//delete[] pszOutBuffer;
			}

			printf("The resource was successfully retrieved.\n");
			break;

		case 401:
			// The server requires authentication.
			printf(" The server requires authentication. Sending credentials...\n");

			// Set the credentials before resending the request.
			if (bResults)
			{

			}
			break;
		default:
			// The status code does not indicate success.
			printf("Error. Status code %d returned.\n", dwStatusCode);
			bDone = TRUE;
		}
	}

	// Report any errors.
	if (!bResults)
		printf("Error %d has occurred.\n", GetLastError());

	// Close any open handles.
	if (hRequest) WinHttpCloseHandle(hRequest);
	if (hConnect) WinHttpCloseHandle(hConnect);

	ResponseData rd;
	rd.dwStatusCode = dwStatusCode;
	rd.response = pszOutBuffer;
	return rd;
}

BOOL closeSessionHandle() {
	if (hSession) {
		WinHttpCloseHandle(hSession);
		hSession = NULL;
		return TRUE;
	}
	return FALSE;
}