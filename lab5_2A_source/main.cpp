#include <stdio.h>
#include <windows.h>
#include <iostream>

#define BUFSIZE 50

char chBuf[BUFSIZE];

int main(int argc, char* argv[])
{
	DWORD dwRead{}, dwWritten;

	HANDLE hStdout, hInputFile;

	hStdout = GetStdHandle(STD_OUTPUT_HANDLE);// PIPE!!!

	if ((hStdout == INVALID_HANDLE_VALUE))
		ExitProcess(1);

	hInputFile = CreateFile(L"input.txt", GENERIC_READ, 0, NULL, OPEN_EXISTING,
		FILE_ATTRIBUTE_READONLY, NULL);

	while (dwRead == 0)
	{

		if (!ReadFile(hInputFile, chBuf, BUFSIZE, &dwRead, NULL))
		{
			std::cerr << "Can'not read" << GetLastError() << '\n';
			break;
		}

		printf("In buffer %s\n", chBuf);

		Sleep(5000);

		if (!WriteFile(hStdout, chBuf, dwRead, &dwWritten, NULL))
		{
			std::cerr << "Can'not Write " << GetLastError() << '\n';
			break;
		}

	}

	CloseHandle(hInputFile);

}