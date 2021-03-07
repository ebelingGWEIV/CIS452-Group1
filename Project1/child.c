#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>

#define READ 0
#define WRITE 1

#define FDWRITE 6
#define FDREAD 5

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

    printf("my vID: %d\nmy pid: %d\nreceiver ID: %d\nparent id: %d\n", myID, getpid(), receiverID, getppid());

    char str[] = "anyone there? - child";
    printf("child sending message\n");
    fflush(stdout);
    write (FDWRITE, (const void *) str, (size_t) strlen (str));
    printf("now child is waiting\n");
    int num = read (FDREAD, (void *) str, (size_t)  sizeof (str));
    printf("child received the message %s\n", str);
    printf("goodbye - child\n");
    fflush(stdout);

    return(0);

}