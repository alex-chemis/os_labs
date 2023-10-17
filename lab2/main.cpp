#include <stdio.h>
#include <windows.h>
#include <string.h>
#include <conio.h>
#include <iostream>
#include <list>
#include <string>

HANDLE hNextThreadEvent;
std::list<std::string> lst;

DWORD WINAPI ThreadProc0(LPVOID lpParameter)
{

	BOOL bExit = FALSE;
	char str[50];
	int num;
	DWORD dw;

	num = *((int*)lpParameter);

	printf("Thread %d begins\n", num);

	while (!bExit)
	{
		dw = WaitForSingleObject(hNextThreadEvent, INFINITE);
		switch (dw)
		{
		case WAIT_OBJECT_0: printf("Thread %d catch Event\n", num); break;
		case WAIT_FAILED: printf("Wait failed\n"); break;
		}

		printf("Thread %d in while\n", num);

		gets_s(str, 50);
		printf("0 User input %s\n", str);
		lst.push_back(str);

		while ((strcmp(str, "next")) && (strcmp(str, "exit")))
		{
			gets_s(str, 50);
			printf("0 User input %s\n", str);
			lst.push_back(str);
		}

		if (!strcmp(str, "exit")) bExit = TRUE;

		printf("Thread %d release control\n", num);

		SetEvent(hNextThreadEvent);

	}

	printf("Thread %d finished\n", num);

	return 0;
}

DWORD WINAPI ThreadProc1(LPVOID lpParameter)
{

	BOOL bExit = FALSE;
	char str[50];
	int num;
	DWORD dw;

	num = *((int*)lpParameter);

	printf("Thread %d begins\n", num);

	while (!bExit)
	{
		dw = WaitForSingleObject(hNextThreadEvent, INFINITE);
		switch (dw)
		{
		case WAIT_OBJECT_0: printf("Thread %d catch Event\n", num); break;
		case WAIT_FAILED: printf("Wait failed\n"); break;
		}

		printf("Thread %d in while\n", num);

		gets_s(str, 50);
		printf("1 User input: ");
		if (lst.empty())
		{
			break;
		}
		auto st = lst.front();
		std::cout << st << '\n';
		lst.pop_front();

		while ((strcmp(str, "next")) && (strcmp(str, "exit")))
		{
			gets_s(str, 50);
			printf("1 User input: ");
			if (lst.empty())
			{
				break;
			}
			auto st = lst.front();
			std::cout << st << '\n';
			lst.pop_front();
		}

		if (!strcmp(str, "exit")) bExit = TRUE;

		printf("Thread %d release control\n", num);

		SetEvent(hNextThreadEvent);

	}

	printf("Thread %d finished\n", num);

	return 0;
}


int main(int argc, char* argv[])
{
	HANDLE hThreads[2];
	int thargs[2];

	printf("I am main! Hello!\n");

	hNextThreadEvent = CreateEvent(NULL, FALSE, FALSE, NULL);

	thargs[0] = 0;
	hThreads[0] = CreateThread(NULL, 0, ThreadProc0, thargs, 0, NULL);
	thargs[1] = 1;
	hThreads[1] = CreateThread(NULL, 0, ThreadProc1, thargs + 1, 0, NULL);

	Sleep(100);

	SetEvent(hNextThreadEvent);

	WaitForMultipleObjects(2, hThreads, TRUE, INFINITE);

	CloseHandle(hNextThreadEvent);
	CloseHandle(hThreads[0]);
	CloseHandle(hThreads[1]);

	return 0;

}