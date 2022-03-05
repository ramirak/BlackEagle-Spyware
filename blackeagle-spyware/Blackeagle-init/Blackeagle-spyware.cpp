#include <windows.h>
#include <Aclapi.h>
#include <map>
#include "TaskManagement.h"
#include "../Network/ApiOperations.h"

DWORD ProtectProcess(void);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{

    authenticateDevice();
    char filename[] = "img.png";
    char json[] = "{\"dataType\":\"DEVICE_SETTINGS\"}";
    uploadFile(filename, json);

    /*
    std::map<std::string, std::string> dataMap = itemFromJson("data.json");
    
    std::map<std::string, std::string> ourMap{ std::make_pair("email", "liron"),std::make_pair("password", "1234") };
    std::string res = jsonFromItem(ourMap);
    */
  
    /*
    std::map<std::string, std::string> ourMap{
       std::make_pair("dataId", "b94418b7"),
       std::make_pair("dataType", "DEVICE_SETTINGS"),
       std::make_pair("createdTimestamp", "2019-10-11T13:33:05.673"),
       // std::make_pair("dataAttributes", "\n\"block\" : true,\n\"Log\" : false\n")
    };

    std::string res = jsonFromItem(ourMap, DATA);
    */

    ProtectProcess();
    //runThreads();
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
