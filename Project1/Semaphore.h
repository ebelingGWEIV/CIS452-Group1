#ifndef PROJECT1_SEMAPHORE_H
#define PROJECT1_SEMAPHORE_H

#include <semaphore.h>
#include <fcntl.h>

#define LOCKED 0
#define UNLOCKED 1

sem_t *mySemaphore1 = NULL;
sem_t *mySemaphore2 = NULL;
#define MY_SEM_1 "/mySem1"
#define MY_SEM_2 "/mySem2"

/**
 * Close and unlink the semaphores.
 */
void CloseSemaphores() {
    sem_close(mySemaphore1);
    sem_unlink(MY_SEM_1);
    sem_close(mySemaphore2);
    sem_unlink(MY_SEM_2);
}

/**
 * Open 2 semaphores in the locked state for dual process synchronization.
 * Proc1:
 *  sem_wait(mySemaphore1)
 *  do something
 *  sem_post(mySemaphore2)
 * proc2:
 *  sem_post(mySemaphore1)
 *  sem_wait(mySemaphore2)
 *  do something
 * @return
 */
int Open2Semaphores() {
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

/**
 * Open 1 semaphore for handling access to shared memory.
 * sem_wait() before changing shared memory
 * sem_post() once done changing shared memory
 * @return 0 for successful opening
 */
int Open1Semaphore()
{
    if ((mySemaphore1 = sem_open(MY_SEM_1, O_CREAT | O_EXCL, 0666, UNLOCKED)) == SEM_FAILED)
    {
        return 1;
    }
    return 0;
}

#endif //PROJECT1_SEMAPHORE_H
