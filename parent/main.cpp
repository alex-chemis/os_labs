#include <string.h>
#include <windows.h>
#include <stdio.h>
#include <iostream>

WCHAR Name[50];
WCHAR path[200];
WCHAR str[200];

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
	wcscpy_s(path, L"child.exe");
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

		std::wcout << L"Hello! My name is " << Name << L'\n';

		while (wcscmp(str, L"next"))
		{
			if (!wcscmp(str, L"exit"))
			{
				SetEvent(hEvent);

				CloseHandle(hEvent);

				return 0;
			}

			_getws_s(str);
			printf("User input %s\n", str);
		}

		printf("good buy!\n");
		SetEvent(hEvent);

	}

	return 0;

}