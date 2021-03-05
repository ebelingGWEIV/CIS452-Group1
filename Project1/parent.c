#include <stdio.h>
#include <sys/types.h>
#include <sys/resource.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

void Execute(char * command[]);

void StartChildren(int numProc);

int main(int argc, char *argv[])
{
    int numProc;
    pid_t child;
    if(argc == 2) {
        numProc = atoi(argv[1]);
    }
    else
    {
        printf("Expected an argument for the number of processes to use\n");
        return 1;
    }
    if(numProc <= 1) {
        printf("Number of processes must be more than 1\n");
        return 1;
    }
    StartChildren(numProc);
}

void StartChildren(int numProc) {//CLion needs a full path               // Pointer to the file name

    //Fork to create all of the processes, attach pipes to each process
    for(int index = 1; index < numProc; index++) //Start at index == 1 because we already have the parent process
    {
            char *args[256];            // Array of pointers for each argument
            char childVirtualID[20];
            char receiverID[20];

            sprintf(childVirtualID, "%d", index);
            sprintf(receiverID, "%d", ((index + 1) < numProc ? index + 1 : 0 )); //If the current process will be the
            // last one, then make its receiver the original process

            char file[] = "/home/kenny/CIS452/CIS452-Group1/Project1/child";
            //char file[] = "./child.out";

            args[0] = file;
            args[1] = childVirtualID; //myID
            args[2] = receiverID; //nextID
            args[3] = NULL;

            if( access( file, F_OK ) == 0 ) {
                Execute(args);
            } else {
                // file doesn't exist
                printf("%s could not be found\n", file);
                fflush(stdout);
                exit(1);
            }
    }
}


/***
 * Creates a child that runs the command using execvp.
 * @param command The parsed command
 */
void Execute(char * command[])
{
    id_t pid, child;
    int status = 0;

    if ((pid = fork()) < 0) {
        perror("fork failure"); //This error seems big enough that exiting is okay
        exit(1);
    }
    else if (pid == 0) {
        if (execvp(command[0], command) < 0) {
            fprintf(stderr, "%s\n", strerror(errno));
            exit(1);
        }
    }
    else {
        child = wait(&status);

        if(status > 0) printf("\nCommand failed to execute %d\n", status);

        else {
            printf("\nIt finally worked\n");
        }
        return;
    }
}