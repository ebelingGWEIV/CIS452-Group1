#ifndef LAB6_SHAREDMEM_H
#define LAB6_SHAREDMEM_H

#include <sys/stat.h>
#include <sys/ipc.h>
#include <sys/shm.h>

struct tracker{
    pid_t *processes; // ID of all processes
    int numProc; // Number of total processes
};

/**
 * Initialize shared memory
 * @param shmId memoryID
 * @param shmPtr Pointer for shared memory
 */
void OpenSharedMem(long **shmPtr, int *shmId, size_t size) {
    if (((*shmId) = shmget (IPC_PRIVATE, size, IPC_CREAT | S_IRUSR | S_IWUSR)) < 0) {
        perror ("i can't get no..\n");
        exit (1);
    }
    if (((*shmPtr) = shmat ((*shmId), 0, 0)) == (void*) -1) {
        perror ("can't attach\n");
        exit (1);
    }
}

/**
 * Detach from the shared memory
 * @param shmPtr pointer to shared memory
 * @param shmId id given by shmget
 */
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

#endif //LAB6_SHAREDMEM_H
