#include <stdio.h>
#include <windows.h>
#include <iostream>

#define BUFSIZE 50

char chBuf[BUFSIZE];

void convertToUpper(char* str);

int main(int argc, char* argv[])
{
	DWORD dwRead{}, dwWritten{};

	HANDLE hStdin, hStdout;

	hStdin = GetStdHandle(STD_INPUT_HANDLE);// PIPE!!!
	hStdout = GetStdHandle(STD_OUTPUT_HANDLE);// PIPE!!!

	if ((hStdout == INVALID_HANDLE_VALUE))
		ExitProcess(1);

	while (dwRead == 0)
	{
		if (!ReadFile(hStdin, chBuf, BUFSIZE, &dwRead, NULL))
		{
			std::cerr << "Can'not read" << GetLastError() << '\n';
			break;
		}

		std::cerr << "In buffer " << chBuf << '\n';

		convertToUpper(chBuf);

		std::cerr << "Out buffer " << chBuf << '\n';

		Sleep(4000);

		if (!WriteFile(hStdout, chBuf, dwRead, &dwWritten, NULL))
		{
			std::cerr << "Can'not Write " << GetLastError() << '\n';
			break;
		}

	}

}

void convertToUpper(char* str)
{
	if (str == NULL)
	{
		return;  // Handle null input string
	}

	for (int i = 0; i < strlen(str); i++)
	{
		str[i] = toupper(str[i]);
	}
}
