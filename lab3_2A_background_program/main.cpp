#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <random>

int main(int argc, char* argv[])
{
    int id = atoi(argv[1]);

    std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_int_distribution<std::mt19937::result_type> dist(0, 10);

    int runTime = dist(rng);

    printf("Client %d beginning to run with %d seconds run time\n", id, runTime);

    Sleep(runTime * 1000);

    printf("Client %d finished after %d seconds\n", id, runTime);

    return 0;
}
