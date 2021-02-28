#ifndef LAB6_SEMAPHORE_H
#define LAB6_SEMAPHORE_H

#include <stdio.h>
#include <semaphore.h>

struct semaphore{
    sig_atomic_t val;
    void (*nextCall) (); //a function pointer to next function to call
};



void Wait(struct  semaphore *);
void Signal(struct semaphore *);
struct semaphore compare_and_swap(struct semaphore *, int, int);

/*
 *
 */
struct semaphore compare_and_swap(struct  semaphore *value, int expected, int new_value)
{
    struct semaphore temp = *value;
    if(value->val == expected)
        value->val = new_value;
    return temp;
}

/**
 * Wait until a spot is available.
 */
void Wait(struct semaphore *sema)
{
    while (compare_and_swap(sema, 1, 0).val < 1)
        ;
    sema->nextCall = Signal;
}

/**
 * Signal that the spot is available again.
 */
void Signal(struct semaphore *sema)
{
    while(compare_and_swap(sema, 0, 1).val > 0);
    sema->nextCall = Wait;
}

#endif //LAB6_SEMAPHORE_H
