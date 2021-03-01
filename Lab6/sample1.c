#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/shm.h>
#include "Semaphore.h"
#include "SharedMem.h"

void Swap(long *shmPtr);

#define SIZE 16

/**
 * We took inspiration from the dining philosophers problem to synchronize the processes.
 * The parent is polite and signals semaphore1 to let the child go first. When the child is done, it signals
 * semaphore2 to let the parent go. In this way, the parent and child are 'passing' ownership of the shared memory.
 */
int main (int argc, char *argv[])
{
    int status;
    long int i, loop;
    long int *shmPtr;
    int shmId;
    pid_t pid;

    // get value of loop variable (from command-line argument)
    if(argc == 2) {
        loop = atoi(argv[1]);
    }

    if(OpenSemaphores() != 0) { //Quit if this fails
        CloseSemaphores();
        return 1;
    }

    OpenSharedMem(&shmPtr, &shmId, SIZE);

    shmPtr[0] = 0;
    shmPtr[1] = 1;

    if (!(pid = fork())) {
        for (i=0; i<loop; i++) {
            // swap the contents of shmPtr[0] and shmPtr[1]
            sem_wait(mySemaphore1); //Wait for parent to give control of sem1
            Swap(shmPtr);
            sem_post(mySemaphore2); //Give control of sem2
        }
        if (shmdt (shmPtr) < 0) {
            perror ("just can't let go\n");
            exit (1);
        }
        sem_wait(mySemaphore1); //wait one last time to make sure the parent ends first
        exit(0);
    }
    else {
        for (i = 0; i < loop; i++) {
            // swap the contents of shmPtr[1] and shmPtr[0]
            sem_post(mySemaphore1); //Let child go first
            sem_wait(mySemaphore2); //Take control of sem2
            Swap(shmPtr);
        }
    }
    //Post so that each process can finish on their own time
    sem_post(mySemaphore1);

    //Wait for child
    wait (&status);

    //Close all the semaphores
    CloseSemaphores();

    //Display the final values
    printf ("values: %li\t%li\n", shmPtr[0], shmPtr[1]);

    //Close the shared memory
    CloseSharedMem(shmPtr, shmId);

    return 0;
}

/**
 * Swap the values of the two indices in shmPtr
 * @param shmPtr
 */
void Swap(long *shmPtr) {
    long int temp = shmPtr[0];
    shmPtr[0] = shmPtr[1];
    shmPtr[1] = temp;
}

