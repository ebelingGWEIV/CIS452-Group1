#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "SharedMem.h"
#include "Semaphore.h"

#define READ 0
#define WRITE 1
#define MAX 1024

#define SHMSIZE 512


struct tracker *CreateTracker(int numProc, struct tracker *pidTracker);
struct tracker *GetTracker(struct tracker *pidTracker);
int CheckRunningProcesses(struct tracker *pidTracker);
void SetupProcesses(struct tracker* pidTracker);

//Expect the user to enter the number of processes as a command line argument
int main(int argc, char * argv[])
{
    int numProc;
    if(argc == 2) {
        numProc = atoi(argv[1]);
    }
    else
    {
        printf("Expected an argument for the number of processes to use\n");
        return 1;
    }

    // Create a place to store PIDs of all the processes created
    pid_t procIDs[numProc];
    procIDs[0] = getpid(); //Add original process ID
    key_t memKey = ftok("Group4Project1", 1);
    long int *shmPtr;
    int shmId;
    InitSharedMem(&shmPtr, &shmId, SHMSIZE, memKey);
    clearMem(shmPtr, SHMSIZE);
    struct tracker *pidTracker = (struct  tracker *) shmPtr;
    pidTracker->numProc = numProc;
    pidTracker->processes = procIDs;

    SetupProcesses(pidTracker);

    puts("waiting");
    while (CheckRunningProcesses(pidTracker) > 1); //Wait until all other processes are closed

    //Shutdown only needs to happen with the parent process
    if(getppid() == pidTracker->processes[0]) {
        puts("quitting\n");
        CloseSharedMem(shmPtr, shmId);
    }
    return 0;
}

/**
 * Checks to see the state of all processes in the pidTracker
 * @param pidTracker Pointer to shared memory holding pids
 * @return The number of running processes
 */
int CheckRunningProcesses(struct tracker *pidTracker)
{
    int procRunning = 0;
    for(int index = 0; index < pidTracker->numProc; index++)
    {
        procRunning += (pidTracker->processes[index] == -1 ? 0 : 1);
    }
    return procRunning;
}

void SetupProcesses(struct tracker *pidTracker)
{
    int numProc = pidTracker->numProc;

    int fd[numProc][2]; //An array with numProc file descriptors

    //Setup each of the pipes
    for(int index = 0; index < numProc; index++)
    {
        if (pipe (fd[index]) < 0) {
            perror ("plumbing problem");
            exit(1);
        }
    }

    //Fork to create all of the processes, attach pipes to each process
    for(int index = 1; index < numProc; index++) //Start at index == 1 because we already have the parent process
    {
        pid_t child = fork();

        pid_t myPID = getpid();
        //Do something different with the child processes
        if(child == 0)
        {
            printf("Child process %d doing something different\n", myPID);
            pidTracker->processes[index] = -1;
            pause(); // Basically killing the process, there are probably better ways
        }
        else{
            printf("child spot %d\n", pidTracker->processes[index]);
            pidTracker->processes[index] = child;
            printf("my PID %d\n", myPID);
        }
    }
}