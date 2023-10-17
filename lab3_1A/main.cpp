#define _WIN32_WINNT 0x0400

#include <windows.h>
#include <stdio.h>

#define _SECOND 10000000
//#define _WIN32_WINNT 0x0400

int main(int argc, char* argv[])
{
    HANDLE          wTimer;
    __int64         endTime;
    LARGE_INTEGER   quitTime;

    SYSTEMTIME now;

    wTimer = CreateWaitableTimer(NULL, FALSE, NULL);


    // Create a negative 64-bit integer that will be used to
    // signal the timer 5 seconds from now ( relative )
    endTime = -(atoi(argv[1]) * _SECOND);

    // Copy the relative time into a LARGE_INTEGER.
    quitTime.LowPart = (DWORD)(endTime & 0xFFFFFFFF);
    quitTime.HighPart = (LONG)(endTime >> 32);

    SetWaitableTimer(wTimer, &quitTime, 0, NULL, NULL, FALSE);

    GetLocalTime(&now);
    printf("Local Time %d %d %d\n", now.wHour, now.wMinute, now.wSecond);

    //2000 Periodic timer interval of 2 seconds.
    DWORD dw = WaitForSingleObject(wTimer, INFINITE);
    switch (dw)
    {
    case WAIT_OBJECT_0: printf("Base thread wake up\n"); break;
    case WAIT_FAILED: printf("Wait failed\n"); return 1;
    }

    printf("Waitable Timer sent signal\n");
    GetLocalTime(&now);
    printf("Local Time %d %d %d\n", now.wHour, now.wMinute, now.wSecond);

    CloseHandle(wTimer);
    return 0;
}