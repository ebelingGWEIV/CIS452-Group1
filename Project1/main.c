#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define READ 0
#define WRITE 1
#define MAX 1024

//Expect the user to enter the number of processes as a command line argument
int main(int argc, char * argv[])
{
    int numProc;
    if((int) argv[0] >= 2)
        numProc = (int) argv[1];
    else
    {
        printf("Expected an argument for the number of processes to use\n");
        return 1;
    }



    return 0;
}

void SetupProcesses(int numProc, int procID[])
{
    int fd[numProc][2]; //An array with numProc file descriptors
    for(int index = 0; index < numProc; index++)
    {
        if (pipe (fd) < 0) {
            perror ("plumbing problem");
            exit(1);
        }
    }
}