#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>

/**
 * Must be compiled with the output file as 'child'
 */
int main(int argc, char *argv[])
{
    int numProc;
    int myID;
    int receiverID;
    if(argc >= 3) {
        myID = atoi(argv[1]);
        receiverID = atoi(argv[2]);
    }
    else
    {
        printf("Expected arguments for virtual IDs\n");
        return 1;
    }

    printf("my vID: %d\nmy pid: %d\nreceiver ID: %d\nparent id: %d", myID, getpid(), receiverID, getppid());
    fflush(stdout);
    return(0);

}