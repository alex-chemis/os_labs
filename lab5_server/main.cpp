#include <string.h>
#include <windows.h>
#include <stdio.h>
#include <iostream>

bool write_pipe(const wchar_t* pipe, const char* buf);
bool read_pipe(const wchar_t* hPipe, char* buf);

int main(int argc, char* argv[])
{
	wchar_t path[200];
	wchar_t str[200];
	wchar_t w_message[200];
	char message[200];
	char buffer[200];

	SECURITY_ATTRIBUTES SecurityAttributes;

	PROCESS_INFORMATION ProcessInf;

	STARTUPINFO StartInfo;

	SecurityAttributes.bInheritHandle = TRUE;
	SecurityAttributes.lpSecurityDescriptor = NULL;
	SecurityAttributes.nLength = sizeof(SecurityAttributes);

	wcscpy_s(path, L"lab5_client.exe");
	wcscat_s(path, L" ");

	HANDLE hEvent = CreateEvent(&SecurityAttributes, FALSE, FALSE, NULL);

	_itow_s((int)hEvent, str, 10);
	wcscat_s(path, str);

	memset(&StartInfo, 0, sizeof(StartInfo));
	StartInfo.cb = sizeof(StartInfo);

	if (!CreateProcess(NULL, path, NULL, NULL, TRUE, CREATE_NEW_CONSOLE, NULL, NULL,
		&StartInfo, &ProcessInf))
	{
		printf("Could not create child process!\n");
		CloseHandle(hEvent);
		return 0;
	}

	while (TRUE)
	{
		std::cout << "Waiting for client...\n";
		WaitForSingleObject(hEvent, INFINITE);

		std::cout << "\nServer in work\n";

		if (!read_pipe(L"request_pipe.txt", message))
		{
			break;
		}
		mbstowcs_s(NULL, w_message, message, strlen(message) + 1);

		if (read_pipe(w_message, buffer))
		{
			if (!write_pipe(L"response_pipe.txt", buffer))
			{
				break;
			}
		}
		else
		{
			if (!write_pipe(L"response_pipe.txt", "Error: unable to read file"))
			{
				break;
			}
		}

		SetEvent(hEvent);
	}

	return 0;

}

bool write_pipe(const wchar_t* pipe, const char* buf)
{
	HANDLE hPipe = CreateFile(pipe, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

	if (hPipe == INVALID_HANDLE_VALUE) {
		printf("Failed to open the file. Error: %d\n", GetLastError());
		return false;
	}

	DWORD bytesWritten;
	if (!WriteFile(hPipe, buf, strlen(buf), &bytesWritten, NULL))
	{
		return false;
	}

	CloseHandle(hPipe);

	return true;
}

bool read_pipe(const wchar_t* pipe, char* buf)
{
	HANDLE hPipe = CreateFile(pipe, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	if (hPipe == INVALID_HANDLE_VALUE) {
		printf("Failed to open the file. Error: %d\n", GetLastError());
		return false;
	}

	DWORD bytesRead;
	if (!ReadFile(hPipe, buf, 200, &bytesRead, NULL)) {
		return false;
	}

	buf[bytesRead] = '\0';

	CloseHandle(hPipe);

	return true;
}