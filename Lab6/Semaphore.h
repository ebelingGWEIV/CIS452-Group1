#ifndef LAB6_SEMAPHORE_H
#define LAB6_SEMAPHORE_H

#include <semaphore.h>
#include <fcntl.h>

#define LOCKED 0

sem_t *mySemaphore1 = NULL;
sem_t *mySemaphore2 = NULL;
#define MY_SEM_1 "/mySem1"
#define MY_SEM_2 "/mySem2"

/**
 * Close and unlink the semaphores.
 */
void CloseSemaphores() {
    sem_close(mySemaphore1);
    sem_close(mySemaphore2);
    sem_unlink(MY_SEM_1);
    sem_unlink(MY_SEM_2);
}

/**
 * Open 2 semaphores in the locked state.
 * @return
 */
int OpenSemaphores() {
    if((mySemaphore1 = sem_open(MY_SEM_1, O_CREAT | O_EXCL, 0666, LOCKED) )== SEM_FAILED)
    {
        printf("error creating semaphore1\n");
        return 1;
    }
    if((mySemaphore2 = sem_open(MY_SEM_2, O_CREAT | O_EXCL, 0666, LOCKED) )== SEM_FAILED)
    {
        printf("error creating semaphore2\n");
        return 1;
    }
    return 0;
}

#endif //LAB6_SEMAPHORE_H
