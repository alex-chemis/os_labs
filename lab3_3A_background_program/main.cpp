#include <windows.h>
#include <stdio.h>
#include <string.h>

int main(int argc, char* argv[])
{
    if (argc != 3) {
        printf("Usage: %s <processID> <objectName>\n", argv[0]);
        return 1;
    }

    int processID = atoi(argv[1]);
    char* objectName = argv[2];

    // Simulate accessing the object (e.g., opening and writing to a file)
    FILE* file;
    if (!fopen_s(&file, objectName, "a+")) {
        fprintf(file, "Child process %d wrote to the file.\n", processID);
        fclose(file);
        printf("Child process %d managed to access the file.\n", processID);
    }
    else {
        printf("Child process %d failed to access the file.\n", processID);
    }

    return 0;
}