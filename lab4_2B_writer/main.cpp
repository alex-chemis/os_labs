#include <string.h>
#include <windows.h>
#include <stdio.h>
#include <iostream>

WCHAR Name[50];
WCHAR path[200];
WCHAR str[200];
char message[200];

HANDLE hEvent;

SECURITY_ATTRIBUTES SecurityAttributes;

PROCESS_INFORMATION ProcessInf;

STARTUPINFO StartInfo;

int main(int argc, char* argv[])
{
	wcscpy_s(Name, L"Parent");

	SecurityAttributes.bInheritHandle = TRUE;
	SecurityAttributes.lpSecurityDescriptor = NULL;
	SecurityAttributes.nLength = sizeof(SecurityAttributes);

	//printf("Enter path to child process:\n");
	//gets(path);
	wcscpy_s(path, L"lab4_2B_reader.exe");
	wcscat_s(path, L" ");

	hEvent = CreateEvent(&SecurityAttributes, FALSE, TRUE, NULL);

	_itow_s((int)hEvent, str, 10);
	wcscat_s(path, str);

	memset(&StartInfo, 0, sizeof(StartInfo));
	StartInfo.cb = sizeof(StartInfo);
	//StartInfo.lpTitle="Child";

	if (!CreateProcess(NULL, path, NULL, NULL, TRUE, CREATE_NEW_CONSOLE, NULL, NULL,
		&StartInfo, &ProcessInf))
	{
		printf("Could not create child process!\n");
		CloseHandle(hEvent);
		return 0;
	}

	std::wcout << path << L'\n';

	while (TRUE)
	{
		wcscpy_s(str, L"a");

		WaitForSingleObject(hEvent, INFINITE);

		std::cout << "Writer in work\n";

		HANDLE hFile = CreateFile(L"output.txt", GENERIC_WRITE, 0, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

		if (hFile == INVALID_HANDLE_VALUE) {
			printf("Failed to open the file. Error: %d\n", GetLastError());
			break;
		}

		while (wcscmp(str, L"next"))
		{
			gets_s(message);

			if (strcmp(message, "next") == 0) {
				break;
			}
			else {
				if (hFile != INVALID_HANDLE_VALUE) {
					DWORD bytesWritten;
					WriteFile(hFile, message, strlen(message), &bytesWritten, NULL);
				}
			}
		}

		CloseHandle(hFile);
		printf("Waiting for reader...\n");
		SetEvent(hEvent);

	}

	return 0;

}