#include <windows.h>
#include <stdio.h>
#include <string.h>

int main() {
    HANDLE hMutexWriter, hMutexReader;
    char message[256];

    // Create named mutexes for synchronization
    hMutexWriter = CreateMutex(NULL, TRUE, L"WriterMutex");
    hMutexReader = CreateMutex(NULL, TRUE, L"ReaderMutex");

    if (hMutexWriter == NULL || hMutexReader == NULL) {
        printf("Failed to create mutexes. Error: %d\n", GetLastError());
        return 1;
    }

    printf("Enter lines of text. Type 'next' to switch to the reader.\n");

    while (1) {
        gets_s(message);

        if (strcmp(message, "next") == 0) {
            // Release the writer mutex
            ReleaseMutex(hMutexWriter);
            printf("Waiting for reader...\n");
            // Wait for the reader mutex to be released
            WaitForSingleObject(hMutexReader, INFINITE);
        }
        else {
            // Open or create the file for writing
            HANDLE hFile = CreateFile(L"output.txt", GENERIC_WRITE, 0, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

            if (hFile != INVALID_HANDLE_VALUE) {
                DWORD bytesWritten;
                WriteFile(hFile, message, strlen(message), &bytesWritten, NULL);
                CloseHandle(hFile);
            }
        }
    }

    CloseHandle(hMutexWriter);
    CloseHandle(hMutexReader);

    return 0;
}
