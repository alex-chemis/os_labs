#include <string.h>
#include <windows.h>
#include <stdio.h>

char Name[50];
char str[200];
char buffer[256];

HANDLE hEvent;
DWORD bytesRead;

int main(int argc, char* argv[])
{
	if (argc != 2)
	{
		printf("Invalid parameter!\n");
		return 0;
	}

	hEvent = (HANDLE)atoi(argv[1]);
	strcpy_s(Name, "child");

	while (TRUE)
	{
		strcpy_s(str, "a");

		WaitForSingleObject(hEvent, INFINITE);

		printf("Reader in work\n");

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

		printf("Waiting for writer...\n");
		SetEvent(hEvent);

	}
	return 0;
}