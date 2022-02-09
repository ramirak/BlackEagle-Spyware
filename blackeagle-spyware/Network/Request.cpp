#include "Request.h"
#include <errhandlingapi.h>
#include <string>

DWORD sendReq() {
	DWORD dwSize = 0;
	DWORD dwDownloaded = 0;
	LPSTR pszOutBuffer;
	BOOL  bResults = FALSE;
	HINTERNET  hSession = NULL,
		hConnect = NULL,
		hRequest = NULL;

	LPCWSTR additionalHeaders =
		L"Accept:application/json\r\nContent-Type:application/json\r\n\r\n";
	char postData[] = "{\"uid\":\"1c073427-04de-4289-bcb2-3175d7adbe09\",\"password\":\"Cpfxt7hMPUdXn6fna9IeKQDA9Fu+9Wyr9YUxwLa6qB4=\"}\r\n";

	// Use WinHttpOpen to obtain a session handle.
	hSession = WinHttpOpen(L"WinHTTP",
		WINHTTP_ACCESS_TYPE_DEFAULT_PROXY,
		WINHTTP_NO_PROXY_NAME,
		WINHTTP_NO_PROXY_BYPASS, 0);

	// Specify an HTTP server. 
	if (hSession)
		hConnect = WinHttpConnect(hSession, L"localhost",
			8010, 0);

	DWORD errorMessageID = ::GetLastError();

	// Create an HTTP request handle.
	if (hConnect)
		hRequest = WinHttpOpenRequest(hConnect, L"POST", L"/login",
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
	 
	// Send a request.
	if (hRequest)
		bResults = WinHttpSendRequest(hRequest,
			additionalHeaders, -1, 
			(LPVOID)postData, strlen(postData),
			strlen(postData), 0);

	errorMessageID = ::GetLastError();

	// WINHTTP_NO_ADDITIONAL_HEADERS
	// WINHTTP_NO_REQUEST_DATA

	// End the request.
	if (bResults)
		bResults = WinHttpReceiveResponse(hRequest, NULL);

	// Keep checking for data until there is nothing left.
	if (bResults)
	{
		do
		{
			// Check for available data.
			dwSize = 0;
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
				delete[] pszOutBuffer;
			}
		} while (dwSize > 0);
	}


	// Report any errors.
	if (!bResults)
		printf("Error %d has occurred.\n", GetLastError());

	// Close any open handles.
	if (hRequest) WinHttpCloseHandle(hRequest);
	if (hConnect) WinHttpCloseHandle(hConnect);
	if (hSession) WinHttpCloseHandle(hSession);

	return 0;
}