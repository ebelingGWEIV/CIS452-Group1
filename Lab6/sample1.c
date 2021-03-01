#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <semaphore.h>
#include <fcntl.h>


#define SIZE 16

void OpenSharedMem(long **shmPtr, int *shmId);
void CloseSharedMem(const long *shmPtr, int shmId);
void Swap(long temp, long *shmPtr);
/**
 * @return 0 when failed
 */
int OpenSemaphores();

void CleanupMem(int *status, const long *shmPtr, int shmId);

sem_t *mySemaphore1 = NULL;
#define MY_SEM_1 "/mySem1"

int main (int argc, char *argv[])
{
    int status;
    long int i, loop, temp = 0;
    long int *shmPtr;
    int shmId;
    pid_t pid;

    // get value of loop variable (from command-line argument)
    if(argc == 2) {
        loop = atoi(argv[1]);
    }

    if(OpenSemaphores() != 0) { //quit if this fails
        return 1;
    }

    OpenSharedMem(&shmPtr, &shmId);

    shmPtr[0] = 0;
    shmPtr[1] = 1;

    if (!(pid = fork())) {
        for (i=0; i<loop; i++) {
            // swap the contents of shmPtr[0] and shmPtr[1]
            Swap(temp, shmPtr);
        }
        if (shmdt (shmPtr) < 0) {
            perror ("just can't let go\n");
            exit (1);
        }
        sem_wait(mySemaphore1); //wait one last time to let the parent end cleanly.
        exit(0);
    }
    else {
        for (i = 0; i < loop; i++) {
            // swap the contents of shmPtr[1] and shmPtr[0]
            Swap(temp, shmPtr);
        }
    }

    wait (status);

    sem_close(mySemaphore1);
    sem_unlink(MY_SEM_1);

    printf ("values: %li\t%li\n", shmPtr[0], shmPtr[1]);

    CloseSharedMem(shmPtr, shmId);

    return 0;
}

int OpenSemaphores() {
    if(mySemaphore1 = sem_open(MY_SEM_1, O_CREAT | S_IRUSR | S_IWUSR, 10) == SEM_FAILED)
    {
        printf("error creating semaphore1\n");
        return 1;
    }
}

void Swap(long temp, long *shmPtr) {
    sem_wait(mySemaphore1);
    temp = shmPtr[0];
    shmPtr[0] = shmPtr[1];
    shmPtr[1] = temp;
    sem_post(mySemaphore1);
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
