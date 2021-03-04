
#include <semaphore.h>
#include <fcntl.h>
#include <stdio.h>
#include <limits.h>

#define SEMNAME "mysem1"
/*
 * Initially, index was set to 0, and was incremented by 100,000,000
 * Then 2100000000, and 1000
 * Finally, 2147483000, and 1
 */
int main(int argc, char **argv)
{
    sem_t *sem;

    for(int long long index = 2147483000; index <= LONG_MAX - 1; index+= 1) {
        if((sem = sem_open(SEMNAME, O_CREAT | O_EXCL, 0666, index) )== SEM_FAILED)
        {
            printf("Max count %lld\n", index-1);
            break;
        }
        sem_destroy(sem);
        sem_close(sem);
        sem_unlink(SEMNAME);
    }
    sem_destroy(sem);
    sem_close(sem);
    sem_unlink(SEMNAME);
}

