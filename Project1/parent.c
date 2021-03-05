#include <stdio.h>
#include <sys/types.h>
#include <sys/resource.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#define READ 0
#define WRITE 1

#define FDWRITE 6
#define FDREAD 5

id_t Execute(char * command[], int procNum, int nextNum);

void StartChildren(int numProc);

id_t *childID;


int fd[20][2];
void InitPipes(int numProc)
{
    for(int index = 0; index < numProc; index++)
    {
        if (pipe (fd[index]) < 0) {
            perror ("plumbing problem");
            printf("index %d\n", index);
            exit(1);
        }
    }
}

int main(int argc, char *argv[])
{
    printf("starting\n");
    int numProc;
    if(argc == 2) {
        numProc = atoi(argv[1]);
    }
    else
    {
        printf("Expected an argument for the number of processes to use\n");
        exit(1);
    }
    if(numProc <= 1) {
        printf("Number of processes must be more than 1\n");
        exit(1);
    }
    childID = malloc((numProc+1) * sizeof(id_t));

    InitPipes(numProc);
    StartChildren(numProc);
    free(childID);
}

void StartChildren(int numProc)
{
    //Link up the pipes. Parent will always write to #1 and be writen to by numProc
    dup2(fd[0][READ], FDREAD); //Read on yours
    dup2(fd[1][WRITE], FDWRITE); //Write on the next
    close(fd[0][READ]);
    close(fd[1][WRITE]);

    for(int index = 1; index < numProc; index++) //Start at index == 1 because we already have the parent process
    {
        //This is all to make sure we can send the correct arguments to the child
        //=========================================================================
        char *args[256];            // Array of pointers for each argument
        char childVirtualID[20];
        char receiverID[20];
        int nextProc = ((index + 1) < numProc ? index + 1 : 0 );

        sprintf(childVirtualID, "%d", index);
        //If the current process will be the last one, then make its receiver the original process
        sprintf(receiverID, "%d", nextProc);

        char file[] = "/home/kenny/CIS452/CIS452-Group1/Project1/child";
        //char file[] = "./child.out";

        args[0] = file;
        args[1] = childVirtualID; //myID
        args[2] = receiverID; //nextID
        args[3] = NULL;
        //=========================================================================
        //Run the child
        if( access( file, F_OK ) == 0 ) {
            childID[index] = Execute(args, index, nextProc);
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
id_t Execute(char * command[], int procNum, int nextNum)
{
    id_t pid;
    int status = 0;

    if ((pid = fork()) < 0) {
        perror("fork failure"); //This error seems big enough that exiting is okay
        exit(1);
    }
    else if (pid == 0) {
        //Link up the pipes
        dup2(fd[procNum][READ], FDREAD); //Read on yours
        dup2(fd[nextNum][WRITE], FDWRITE); //Write on the next
        close(fd[procNum][READ]);
        close(fd[nextNum][WRITE]);

        if (execvp(command[0], command) < 0) {
            fprintf(stderr, "%s\n", strerror(errno));
            exit(1);
        }
    }
    else {
        //This is all for testing the pipes
        //=========================================================================
        char str[128];
        printf("waiting for message\n");
        int num = read (FDREAD, (void *) str, (size_t)  sizeof (str));
        if (num > 128) {
            perror ("pipe read error\n");
            exit(1);
        }
        printf("parent received received: %s\n", str);

        char newstr[128] = "i am here";

        write (FDWRITE, (const void *) newstr, (size_t) strlen (newstr) + 1);
        //=========================================================================
        int child = wait(&status);
        if(child < 0) perror("child failed");

        return pid;
    }
}