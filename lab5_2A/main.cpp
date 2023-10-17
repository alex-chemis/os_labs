#define _WIN32_WINNT 0x0400
#include <stdio.h>
#include <string.h>
#include <windows.h>

int main(int argc, char* argv[])
{
	HANDLE hSource, hFilter, hSink;

	SECURITY_ATTRIBUTES pipeSourceToFilter;
	SECURITY_ATTRIBUTES pipeFilterToSource;

	STARTUPINFO scrStartInfo, fltStartInfo, sinkStartInfo;

	PROCESS_INFORMATION scrProcessInfo, fltProcessInfo, sinkProcessInfo;

	const wchar_t* fSource = L"lab5_2A_source.exe";
	const wchar_t* fFilter = L"lab5_2A_filter.exe";
	const wchar_t* fSink = L"lab5_2A_sink.exe";
	wchar_t path[200];

	pipeSourceToFilter.nLength = sizeof(SECURITY_ATTRIBUTES);
	pipeSourceToFilter.lpSecurityDescriptor = NULL;
	pipeSourceToFilter.bInheritHandle = TRUE;

	if (!CreatePipe(&hSource, &hSink, &pipeSourceToFilter, 0))
	{
		printf("Cann't create PIPE\n");
		ExitProcess(1);
	}

	pipeFilterToSource.nLength = sizeof(SECURITY_ATTRIBUTES);
	pipeFilterToSource.lpSecurityDescriptor = NULL;
	pipeFilterToSource.bInheritHandle = TRUE;

	//if (!CreatePipe(&hFilter, &hSink, &pipeFilterToSource, 0))
	//{
	//	printf("Cann't create PIPE\n");
	//	ExitProcess(1);
	//}

	Sleep(1000);

	printf("Main: creating source process\n");

	memset(&scrStartInfo, 0, sizeof(STARTUPINFO));
	scrStartInfo.cb = sizeof(STARTUPINFO);
	scrStartInfo.hStdInput = GetStdHandle(STD_INPUT_HANDLE);
	scrStartInfo.hStdOutput = hSink;
	scrStartInfo.hStdError = GetStdHandle(STD_ERROR_HANDLE);
	scrStartInfo.dwFlags = STARTF_USESTDHANDLES;

	wcscpy_s(path, wcslen(fSource) + 1, fSource);

	if (!CreateProcess(NULL, path, NULL, NULL, TRUE, CREATE_NEW_CONSOLE,
		NULL, NULL, &scrStartInfo, &scrProcessInfo))
	{
		printf("Cann't create source process\n");
		ExitProcess(1);
	}

	Sleep(1000);

	printf("Main: creating filter process\n");

	memset(&fltStartInfo, 0, sizeof(STARTUPINFO));
	fltStartInfo.cb = sizeof(STARTUPINFO);
	fltStartInfo.hStdInput = hSource;
	fltStartInfo.hStdOutput = hSink;
	fltStartInfo.hStdError = GetStdHandle(STD_ERROR_HANDLE);
	fltStartInfo.dwFlags = STARTF_USESTDHANDLES;

	wcscpy_s(path, wcslen(fFilter) + 1, fFilter);

	if (!CreateProcess(NULL, path, NULL, NULL, TRUE, CREATE_NEW_CONSOLE,
		NULL, NULL, &fltStartInfo, &fltProcessInfo))
	{
		printf("Cann't create filter process\n");
		ExitProcess(1);
	}

	//Sleep(1000);

	printf("Main: creating sink process\n");

	memset(&sinkStartInfo, 0, sizeof(STARTUPINFO));
	sinkStartInfo.cb = sizeof(STARTUPINFO);
	sinkStartInfo.hStdInput = hSource;
	sinkStartInfo.hStdOutput = GetStdHandle(STD_OUTPUT_HANDLE);
	sinkStartInfo.hStdError = GetStdHandle(STD_ERROR_HANDLE);
	sinkStartInfo.dwFlags = STARTF_USESTDHANDLES;

	wcscpy_s(path, wcslen(fSink) + 1, fSink);

	if (!CreateProcess(NULL, path, NULL, NULL, TRUE, CREATE_NEW_CONSOLE,
		NULL, NULL, &sinkStartInfo, &sinkProcessInfo))
	{
		printf("Cann't create sink process\n");
		ExitProcess(1);
	}

	CloseHandle(hSource);
	//CloseHandle(hFilter);
	CloseHandle(hSink);
}