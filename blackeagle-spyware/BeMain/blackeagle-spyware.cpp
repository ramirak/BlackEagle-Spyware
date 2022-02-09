#include <windows.h>
#include <Aclapi.h>
#include "manageTasks.h"
#include "../Network/Request.h"

DWORD ProtectProcess(void);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    LPCWSTR additionalHeaders =
        L"Accept:application/json\r\nContent-Type:application/json\r\n\r\n";
    char postData[] = "{\"uid\":\"1c073427-04de-4289-bcb2-3175d7adbe09\",\"password\":\"Cpfxt7hMPUdXn6fna9IeKQDA9Fu+9Wyr9YUxwLa6qB4=\"}\r\n";
    LPCWSTR apiUrl = L"/login";
    LPCWSTR method = L"POST";
    
    SendRequest(additionalHeaders,postData,apiUrl,method);
          
    ProtectProcess();
   // runThreads();
    return 0;
}

// https://stackoverflow.com/questions/6185975/prevent-user-process-from-being-killed-with-end-process-from-process-explorer
DWORD ProtectProcess(void)
{
    HANDLE hProcess = GetCurrentProcess();
    PACL pEmptyDacl;
    DWORD dwErr;

    // using malloc guarantees proper alignment
    pEmptyDacl = (PACL)malloc(sizeof(ACL));

    if (!InitializeAcl(pEmptyDacl, sizeof(ACL), ACL_REVISION))
    {
        dwErr = GetLastError();
    }
    else
    {
        dwErr = SetSecurityInfo(hProcess, SE_KERNEL_OBJECT,
            DACL_SECURITY_INFORMATION, NULL, NULL, pEmptyDacl, NULL);
    }
    free(pEmptyDacl);
    return dwErr;
}
