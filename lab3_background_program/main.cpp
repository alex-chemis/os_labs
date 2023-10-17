#include <windows.h>
#include <stdio.h>

int main(int argc, char* argv[])
{
	printf("Client % s beginning to run\n", argv[1]);
	while (TRUE)
	{
		if (getc(stdin) == 'y') break;
	}
	return 0;
}