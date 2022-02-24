#include <Windows.h>

int lockUser() {
    DWORD dwError = 0;
    NET_API_STATUS nStatus;
    //
    // All parameters are required.
    //
    if (argc != 5)
    {
        fwprintf(stderr, L"Usage: %s \\\\ServerName UserName OldPassword NewPassword\n", argv[0]);
        exit(1);
    }
    //
    // Call the NetUserChangePassword function.
    //
    nStatus = NetUserChangePassword(argv[1], argv[2], argv[3], argv[4]);
    //
    // If the call succeeds, inform the user.
    //
    if (nStatus == NERR_Success)
        fwprintf(stderr, L"User password has been changed successfully\n");
    //
    // Otherwise, print the system error.
    //
    else
        fprintf(stderr, "A system error has occurred: %d\n", nStatus);

    return 0;
}