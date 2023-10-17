#include <conio.h>
#include <string.h>
#include <stdio.h>
#include <windows.h>

HANDLE hEvent;

WCHAR proc_name[50];
WCHAR str[50];

int wmain(int argc, WCHAR* argv[])
{
	if (argc >= 2)
	{
		wcscpy_s(proc_name, wcslen(argv[1]) + 1, argv[1]);
	}
	else
	{
		wcscpy_s(proc_name, wcslen(L"Default Proc_name") + 1, L"Default Proc_name");
	}

	hEvent = CreateEvent(NULL, FALSE, TRUE, L"exclusive_speech_event");
	if (GetLastError() == ERROR_ALREADY_EXISTS)
	{
		printf("Event was opened\n");
	}
	else
	{
		printf("Event was created\n");
	}

	while (TRUE)
	{
		wcscpy_s(str, wcslen(L"a") + 1, L"a");

		WaitForSingleObject(hEvent, INFINITE);

		printf("Hello! My name is %ls\n", proc_name);

		while (wcscmp(str, L"good buy"))
		{
			if (!wcscmp(str, L"exit"))
			{
				SetEvent(hEvent);

				CloseHandle(hEvent);

				return 0;
			}

			_getws_s(str, 10);
			printf("User input %ls\n", str);
		}

		printf("good buy!\n");
		SetEvent(hEvent);

	}

}