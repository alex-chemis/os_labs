#include <windows.h>
#include <stdio.h>

int main() {
    HANDLE hMutexWriter, hMutexReader;
    char buffer[256];
    DWORD bytesRead;

    // Create named mutexes for synchronization
    hMutexWriter = OpenMutex(SYNCHRONIZE, TRUE, L"WriterMutex");
    hMutexReader = OpenMutex(SYNCHRONIZE, TRUE, L"ReaderMutex");

    if (hMutexWriter == NULL || hMutexReader == NULL) {
        printf("Failed to open mutexes. Error: %d\n", GetLastError());
        return 1;
    }

    while (1) {
        // Wait for the writer mutex to be released
        printf("Waiting for writer...\n");
        WaitForSingleObject(hMutexWriter, INFINITE);

        // Open the file for reading
        HANDLE hFile = CreateFile(L"output.txt", GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

        if (hFile == INVALID_HANDLE_VALUE) {
            printf("Failed to open the file. Error: %d\n", GetLastError());
            break;
        }

        while (1) {
            if (ReadFile(hFile, buffer, sizeof(buffer), &bytesRead, NULL) && bytesRead > 0) {
                buffer[bytesRead] = '\0';
                printf("Received: %s\n", buffer);
            }
            else {
                break; // End of file
            }
        }

        // Close the file
        CloseHandle(hFile);

        // Release the reader mutex
        printf("Waiting for writer...\n");
        WaitForSingleObject(hMutexReader, INFINITE);
        ReleaseMutex(hMutexReader);
    }

    CloseHandle(hMutexWriter);
    CloseHandle(hMutexReader);

    return 0;
}
