#include <stdio.h>
#include <windows.h>
#include <iostream>

#define BUFSIZE 50


int main(int argc, char* argv[])
{

	char chBuf[BUFSIZE];

	DWORD dwRead{}, dwWritten;

	HANDLE hStdin, hOutputFile;

	printf("Hello! I am reader\n");

	hStdin = GetStdHandle(STD_INPUT_HANDLE);

	if ((hStdin == INVALID_HANDLE_VALUE))
		ExitProcess(1);

	hOutputFile = CreateFile(L"output.txt", GENERIC_WRITE, 0, NULL, OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL, NULL);

	while (dwRead == 0)
	{
		for (int i = 0; i < 50; i++) chBuf[i] = '\0';

		if (!ReadFile(hStdin, chBuf, BUFSIZE, &dwRead, NULL))
		{
			std::cerr << "Can'not read" << GetLastError() << '\n';
			break;
		}


		printf("In buffer %s\n", chBuf);

		Sleep(1000);

		if (!WriteFile(hOutputFile, chBuf, dwRead, &dwWritten, NULL))
		{
			std::cerr << "Can'not Write " << GetLastError() << '\n';
			break;
		}

	}

	CloseHandle(hOutputFile);

}