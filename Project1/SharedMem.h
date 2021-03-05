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
 * Fills the shared memory with '\0'
 * @param shmPtr pointer to memory location
 * @param length number of char to clear
 */
void clearMem(const long int *shmPtr, int length) {
    int i;
    for(i = 0; i < length; i++) {
        *(((char *)shmPtr)+i) = 0;
    }
}

/**
 * Initialize shared memory
 * @param shmId memoryID
 * @param shmPtr Pointer for shared memory
 */
void InitSharedMem(long int **shmPtr, int *shmId, size_t size, key_t memKey) {//Get the shared memory pointer to the
    if (((*shmId) = shmget (memKey, size, IPC_CREAT | S_IRUSR | S_IWUSR)) < 0) {
        perror ("I can't get no..\n");
        exit (1);
    }
    if (((*shmPtr) = shmat ((*shmId), 0, 0)) == (void*) -1) {
        perror ("can't attach\n");
        exit (1);
    }
    clearMem((*shmPtr), size);
}

/**
 * Detach from the shared memory
 * @param shmPtr pointer to shared memory
 * @param shmId id given by shmget
 */
void CloseSharedMem(char *shmPtr, int shmId) {
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
