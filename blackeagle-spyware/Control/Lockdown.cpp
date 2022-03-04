#include "Lockdown.h"


BOOL lockUser(LPWSTR password) {
    DWORD dwLevel = 1003;
    USER_INFO_1003 ui;
    NET_API_STATUS nStatus;

    // Fill in the USER_INFO_1003 structure member.
    ui.usri1003_password = password;

    DWORD bufSize = MAX_PATH;
    TCHAR domainNameBuf[MAX_PATH];
    GetComputerNameEx(ComputerNameDnsDomain, domainNameBuf, &bufSize);

    TCHAR username[UNLEN + 1];
    DWORD size = UNLEN + 1;
    GetUserName((TCHAR*)username, &size);

    // Call the NetUserSetInfo function 
    //  to disable the account, specifying level 1003.
    nStatus = NetUserSetInfo(domainNameBuf,
        username,
        dwLevel,
        (LPBYTE)&ui,
        NULL);

    // Display the result of the call.
    if (nStatus == NERR_Success)
        return TRUE;

    if (!LockWorkStation())
        return TRUE;

    return FALSE;
}

char* generateRandomPass()
{
    char* newPass = (char*)malloc(sizeof(char) * (PASSWORD_LEN + 1));
    if (!newPass)
        return NULL;

    /* Seed number for rand() */
    srand((unsigned int)time(0));

    /*
    // With special chars
    if()
    {
        for (int i = 0; i < sizeof(newPass); i++)
            newPass += putchar(rand() % 94 + 33);
    }
    */

    /* Without special chars */
    const char alphanum[] =
        "0123456789"
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz";

    for (int i = 0; i < PASSWORD_LEN; i++)
        newPass[i] = alphanum[rand() % (sizeof(alphanum) - 1)];
    newPass[PASSWORD_LEN] = '\0';
    return newPass;
}
