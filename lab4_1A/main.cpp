#include <windows.h>
#include <stdio.h>
#include <stdbool.h>

int main() {
    // Create a named event to coordinate communication
    HANDLE event = CreateEvent(NULL, FALSE, TRUE, L"SharedEvent");

    if (event == NULL) {
        printf("Failed to create the event.\n");
        return 1;
    }

    bool hasControl = false;
    char message[256];

    while (true) {
        if (hasControl) {
            printf("You have control. Enter 'exit' to end or 'next' to pass control: ");
            gets_s(message);

            if (strcmp(message, "exit") == 0) {
                break;
            } else if (strcmp(message, "next") == 0) {
                SetEvent(event);
                hasControl = false;
            } else {
                printf("You said: %s\n", message);
            }
        } else {
            printf("Waiting for control...\n");
            WaitForSingleObject(event, INFINITE);
            hasControl = true;
        }
    }

    CloseHandle(event);
    return 0;
}