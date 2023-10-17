#define _WIN32_WINNT 0x0400
#include <stdio.h>
#include <string.h>
#include <windows.h>

HANDLE readPipe, writePipe;

SECURITY_ATTRIBUTES pipeSA;

STARTUPINFO scrStartInfo, sinkStartInfo;

PROCESS_INFORMATION scrProcessInfo, sinkProcessInfo;

//HANDLE hSaveStdout;

//char saveStdout[20];
wchar_t path[200];

int main(int argc, char* argv[])
{
	pipeSA.nLength = sizeof(SECURITY_ATTRIBUTES);
	pipeSA.lpSecurityDescriptor = NULL;
	pipeSA.bInheritHandle = TRUE;

	if (!CreatePipe(&readPipe, &writePipe, &pipeSA, 0))
	{
		printf("Cann't create PIPE\n");
		ExitProcess(1);
	}

	printf("readPipe %d writePipe %d\n", readPipe, writePipe);

	// Create process to write the pipe
	// Make handles inheritable

	printf("Main: creating source process\n");

	memset(&scrStartInfo, 0, sizeof(STARTUPINFO));
	scrStartInfo.cb = sizeof(STARTUPINFO);
	scrStartInfo.hStdInput = GetStdHandle(STD_INPUT_HANDLE);
	scrStartInfo.hStdOutput = writePipe;
	scrStartInfo.hStdError = GetStdHandle(STD_ERROR_HANDLE);
	scrStartInfo.dwFlags = STARTF_USESTDHANDLES;

	wcscpy_s(path, wcslen(L"lab5_1A_wpipe.exe") + 1, L"lab5_1A_wpipe.exe");

	if (!CreateProcess(NULL, path, NULL, NULL, TRUE, CREATE_NEW_CONSOLE,
		NULL, NULL, &scrStartInfo, &scrProcessInfo))
	{
		printf("Cann't create source process\n");
		ExitProcess(1);
	}

	Sleep(1000);

	// Create process to read the pipe
	// Make handles inheritable

	printf("Main: creating sink process\n");

	memset(&sinkStartInfo, 0, sizeof(STARTUPINFO));
	sinkStartInfo.cb = sizeof(STARTUPINFO);
	sinkStartInfo.hStdInput = readPipe;
	sinkStartInfo.hStdOutput = GetStdHandle(STD_OUTPUT_HANDLE);
	sinkStartInfo.hStdError = GetStdHandle(STD_ERROR_HANDLE);
	sinkStartInfo.dwFlags = STARTF_USESTDHANDLES;

	//strcpy(path, "c:\\Users\\Admin\\source\\repos\\r_pipe\\debug\\r_pipe.exe");
	wcscpy_s(path, wcslen(L"lab5_1A_rpipe.exe") + 1, L"lab5_1A_rpipe.exe");

	if (!CreateProcess(NULL, path, NULL, NULL, TRUE, CREATE_NEW_CONSOLE,
		NULL, NULL, &sinkStartInfo, &sinkProcessInfo))
	{
		printf("Cann't create sink process\n");
		ExitProcess(1);
	}

	CloseHandle(readPipe);
	CloseHandle(writePipe);

}