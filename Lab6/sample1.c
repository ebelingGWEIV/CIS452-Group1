#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include "Semaphore.h"

#define SIZE 16

void OpenSharedMem(long **shmPtr, int *shmId);
void CloseSharedMem(const long *shmPtr, int shmId);
void Swap(long temp, long *shmPtr, struct semaphore *sema);

sem_t *semaphore = NULL;

int main (int argc, char *argv[])
{
    int status;
    long int i, loop, temp = 0;
    long int *shmPtr, *semPtr;
    int shmId, semId;
    pid_t pid;

    // get value of loop variable (from command-line argument)
    if(argc == 2) {
        loop = atoi(argv[1]);
    }

    OpenSharedMem(&shmPtr, &shmId);
    OpenSharedMem(&semPtr, &semId);

    shmPtr[0] = 0;
    shmPtr[1] = 1;
    struct semaphore *sema = (struct semaphore *)semPtr;
    sema->val = 1;
    sema->nextCall = Wait;

    if (!(pid = fork())) {
        for (i=0; i<loop; i++) {
            // swap the contents of shmPtr[0] and shmPtr[1]
            Swap(temp, shmPtr, sema);
        }
        if (shmdt (shmPtr) < 0) {
            perror ("just can't let go\n");
            exit (1);
        }
        exit(0);
    }
    else {
        for (i = 0; i < loop; i++) {
            // swap the contents of shmPtr[1] and shmPtr[0]
            Swap(temp, shmPtr, sema);
        }
    }

    wait (&status);
    printf ("values: %li\t%li\n", shmPtr[0], shmPtr[1]);

    CloseSharedMem(shmPtr, shmId);
    CloseSharedMem(semPtr, semId);

    return 0;
}

void Swap(long temp, long *shmPtr, struct semaphore *sema) {
    sema->nextCall(sema);
    static int amount = 0;
    temp = shmPtr[0];
    shmPtr[0] = shmPtr[1];
    shmPtr[1] = temp;
    sema->nextCall(sema);
}

void CloseSharedMem(const long *shmPtr, int shmId) {
    if (shmdt (shmPtr) < 0) {
        perror ("just can't let go\n");
        exit (1);
    }
    if (shmctl (shmId, IPC_RMID, 0) < 0) {
        perror ("can't deallocate\n");
        exit(1);
    }
}

void OpenSharedMem(long **shmPtr, int *shmId) {
    if (((*shmId) = shmget (IPC_PRIVATE, SIZE, IPC_CREAT | S_IRUSR | S_IWUSR)) < 0) {
        perror ("i can't get no..\n");
        exit (1);
    }
    if (((*shmPtr) = shmat ((*shmId), 0, 0)) == (void*) -1) {
        perror ("can't attach\n");
        exit (1);
    }
}
