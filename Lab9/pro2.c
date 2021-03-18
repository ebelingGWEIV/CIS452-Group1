#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SIZE 16

int myGlobal = 1;
int emptyGlobal;

int main(int argc, char *argv[])
{
    int mylocal = 2;
    int emptylocal;
    int *intPtr;
    int *empPtr;
    intPtr = malloc((size_t)SIZE);

    printf("\n----globals----\n");
    printf("global \t\t\t\t%p\n", &myGlobal);
    printf("empty global \t\t\t%p\n", &emptyGlobal);

    printf("----locals----\n");
    printf("local \t\t\t\t%p\n", &mylocal);
    printf("empty local \t\t\t%p\n", &emptylocal);

    printf("----local pointers----\n");
    printf("allocated int pointer \t\t%p\n", intPtr);
    printf("un allocated int pointer \t%p\n", empPtr);

    free(intPtr);
    puts("press any key...\n");
    char ch;
    ch = getc(stdin);
    return 0;
}

