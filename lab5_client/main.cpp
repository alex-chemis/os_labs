#include <string.h>
#include <windows.h>
#include <stdio.h>
#include <iostream>

bool write_pipe(const wchar_t* pipe, const char* buf);
bool read_pipe(const wchar_t* hPipe, char* buf);

int main(int argc, char* argv[])
{
	if (argc != 2)
	{
		printf("Invalid parameter!\n");
		return 0;
	}

	char buffer[256];
	char message[200];

	HANDLE hEvent = (HANDLE)atoi(argv[1]);

	while (TRUE)
	{
		std::cout << "Client in work\n";

		std::cout << "Input file name: ";
		std::cin >> message;
		if (!write_pipe(L"request_pipe.txt", message))
		{
			break;
		}

		SetEvent(hEvent);
		std::cout << "Waiting for server...\n";
		WaitForSingleObject(hEvent, INFINITE);

		if (!read_pipe(L"response_pipe.txt", buffer))
		{
			break;
		}
		std::cout << "File contents:\n" << buffer << "\n\n";
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