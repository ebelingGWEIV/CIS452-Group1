#ifndef LAB6_SEMAPHORE_H
#define LAB6_SEMAPHORE_H

#include <stdio.h>
#include <semaphore.h>

struct semaphore{
    sig_atomic_t val;
    void (*nextCall) (); //a function pointer to next function to call
};



void Wait(struct  mySemaphore *);
void Signal(struct mySemaphore *);
struct mySemaphore1 compare_and_swap(struct mySemaphore1 *, int, int);

/*
 *
 */
struct mySemaphore1 compare_and_swap(struct  mySemaphore1 *value, int expected, int new_value)
{
    struct mySemaphore1 temp = *value;
    if(value->val == expected)
        value->val = new_value;
    return temp;
}

/**
 * Wait until a spot is available.
 */
void Wait(struct mySemaphore1 *sema)
{
    while (compare_and_swap(sema, 1, 0).val < 1)
        ;
    sema->nextCall = Signal;
}

/**
 * Signal that the spot is available again.
 */
void Signal(struct mySemaphore1 *sema)
{
    while(compare_and_swap(sema, 0, 1).val > 0);
    sema->nextCall = Wait;
}

#endif //LAB6_SEMAPHORE_H
