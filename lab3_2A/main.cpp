#define _WIN32_WINNT 0x0400
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <vector>

#define _SECOND 10000000

std::vector< PROCESS_INFORMATION > CreateBackgroundProcesses(int N, int K);
void TerminateBackgroundProcesses(std::vector< PROCESS_INFORMATION >&& infos);

int main(int argc, char* argv[])
{
    if (argc != 3) {
        printf("Usage: %s <N> <K>\n", argv[0]);
        return 1;
    }

    int N = atoi(argv[1]);
    int K = atoi(argv[2]);

    HANDLE          wTimer;
    __int64         endTime;
    LARGE_INTEGER   quitTime;

    SYSTEMTIME now;

    wTimer = CreateWaitableTimer(NULL, FALSE, NULL);

    endTime = -(K * _SECOND);

    quitTime.LowPart = (DWORD)(endTime & 0xFFFFFFFF);
    quitTime.HighPart = (LONG)(endTime >> 32);

    SetWaitableTimer(wTimer, &quitTime, 0, NULL, NULL, FALSE);

    GetLocalTime(&now);
    printf("Local Time %d %d %d\n", now.wHour, now.wMinute, now.wSecond);

    auto infos = CreateBackgroundProcesses(N, K);

    DWORD dw = WaitForSingleObject(wTimer, INFINITE);
    switch (dw)
    {
    case WAIT_OBJECT_0: printf("Base thread wake up\n"); break;
    case WAIT_FAILED: printf("Wait failed\n"); return 1;
    }

    TerminateBackgroundProcesses(std::move(infos));

    printf("Waitable Timer sent signal\n");
    GetLocalTime(&now);
    printf("Local Time %d %d %d\n", now.wHour, now.wMinute, now.wSecond);

    CloseHandle(wTimer);

    printf("Terminate all other proccesses");

    return 0;
}

std::vector< PROCESS_INFORMATION > CreateBackgroundProcesses(int N, int K)
{
    std::vector< PROCESS_INFORMATION > infos;
    for (int i = 0; i < N; i++) {
        wchar_t commandLine[256];
        swprintf_s(commandLine, L"lab3_2A_background_program.exe %d", i);

        STARTUPINFO si;
        infos.push_back(PROCESS_INFORMATION{});
        ZeroMemory(&si, sizeof(si));
        si.cb = sizeof(si);

        if (CreateProcessW(NULL, commandLine, NULL, NULL, FALSE, 0, NULL, NULL, &si, &infos.back())) {
            printf("Process %d started\n", i);
        }
        else {
            printf("Failed to start process %d\n", i);
        }
    }
    return infos;
}

void TerminateBackgroundProcesses(std::vector< PROCESS_INFORMATION >&& infos)
{
    for (auto& info : infos)
    {
        TerminateProcess(info.hProcess, 0);
        CloseHandle(info.hProcess);
        CloseHandle(info.hThread);
    }
}