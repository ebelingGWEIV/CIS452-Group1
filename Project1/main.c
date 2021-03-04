#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "SharedMem.h"
#include "Semaphore.h"

#define READ 0
#define WRITE 1
#define MAX 1024

#define SHMSIZE 128


int CheckRunningProcesses(struct tracker *pidTracker);
void SetupProcesses(struct tracker *pidTracker);

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
    long int *shmPtr;
    int shmId;
    OpenSharedMem(&shmPtr, &shmId, SHMSIZE);
    struct tracker *pidTracker = (struct  tracker *) shmPtr;
    pidTracker->numProc = numProc;
    pidTracker->processes = procIDs;

    // Setup semaphores to protect the shared memory
    if(Open1Semaphore() != 0)
    {
        CloseSemaphores();
        return 1;
    }

    SetupProcesses(pidTracker);

//    while (CheckRunningProcesses(pidTracker) > 1); //Wait until all other processes are closed

    //Shutdown only needs to happen with the parent process
    if(getppid() == pidTracker->processes[0]) {
        puts("parent is closing up shop\n");
        CloseSemaphores();
        CloseSharedMem(shmPtr, shmId);
    }
    puts("quitting\n");
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
        procRunning += (pidTracker->processes[index] > 0 ? 1 : 0);
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
        sem_wait(mySemaphore1);
        pidTracker->processes[index] = child;
        sem_post(mySemaphore1);
//        dup2(fd[index][READ], STDIN_FILENO);
//        dup2(fd[index][WRITE], STDOUT_FILENO);
        // Close the fd
        close(fd[index][READ]);
        close(fd[index][WRITE]);
        pid_t myPID = getpid();
        //Do something different with the child processes
        printf("my PID %d tracker PID %d\n", myPID, pidTracker->processes[index]);
        if(child != 0)
        {
            printf("Child process %d doing something different\n", myPID);
            sem_wait(mySemaphore1);
            pidTracker->processes[index] = 0;
            pause(); // Basically killing the process, there are probably better ways
        }
        else{
            printf("my PID %d\n", myPID);
        }
    }
}