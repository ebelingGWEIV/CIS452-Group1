#include <stdio.h>
#include <sys/types.h>
#include <sys/resource.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include "SigHandler.h"
#include "Token.h"
#include <sys/ioctl.h>

#define READ 0
#define WRITE 1

#define FDWRITE 4200
#define FDREAD 4300

id_t StartChild(char **command, int procNum, int nextNum);

void StartChildren(int numProc);

void RunMessenger(int numProc);

void Send(struct token *tok, int dest);

void ClearString(const int maxLength, char *message);


int fd[20][2];

struct token *GetUserMessage(int numProc, const int maxLength);

void InitPipes(int numProc) {
    for (int index = 1; index <= numProc; index++) {
        if (pipe(fd[index]) < 0) {
            perror("plumbing problem");
            printf("index %d\n", index);
            exit(1);
        }
    }
}


int main(int argc, char *argv[]) {
    OverrideSIGNINT();

    printf("starting\n");
    int numProc;
    if (argc == 2) {
        numProc = atoi(argv[1]);
    } else {
        printf("Expected an argument for the number of processes to use\n");
        exit(1);
    }
    if (numProc <= 2) {
        printf("Number of processes must be more than 2\n");
        exit(1);
    }

    InitPipes(numProc);
    StartChildren(numProc);
    RunMessenger(numProc);
}

void StartChildren(int numProc) {
    for (int index = 1; index <= numProc; index++) //Start at index == 1 because we already have the parent process
    {
        if (index == 1) //Setup the parent
        {
            //Link up the pipes. Parent will always write to #1 and be writen to by numProc
            dup2(fd[index][READ], FDREAD); //Read on yours
            dup2(fd[index + 1][WRITE], FDWRITE); //Write on the next
            close(fd[index][READ]);
            close(fd[index + 1][WRITE]);
        } else {

            //Setup the correct arguments to the child
            //=========================================================================
            char *args[256];            // Array of pointers for each argument
            char childVirtualID[20];
            char receiverID[20];
            int nextProc = ((index + 1) <= numProc ? index + 1 : 1);

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
            if (access(file, F_OK) == 0) {
                StartChild(args, index, nextProc);
            } else {
                // file doesn't exist
                printf("%s could not be found\n", file);
                fflush(stdout);
                exit(1);
            }
        }
    }
}

/***
 * Creates a child that runs the command using execvp.
 * @param command The parsed command
 */
id_t StartChild(char **command, int procNum, int nextNum) {
    id_t pid;
    int status = 0;

    if ((pid = fork()) < 0) {
        perror("fork failure");
        exit(1);
    } else if (pid == 0) {
        //Link up the pipes
        dup2(fd[procNum][READ], FDREAD); //Read on yours
        dup2(fd[nextNum][WRITE], FDWRITE); //Write on the next
        close(fd[procNum][READ]);
        close(fd[nextNum][WRITE]);

        if (execvp(command[0], command) < 0) {
            fprintf(stderr, "%s\n", strerror(errno));
            exit(1);
        }
    } else {
        return pid;
    }
}

void RunMessenger(int numProc) {
    const int maxLength = 128;
    int myID = 1;
    int destID = 2;
    //Things needed for select() to work properly//
    fd_set readfds;
    struct timeval tv;
    int fd_stdin;

    fd_stdin = fileno(stdin);

    FD_ZERO(&readfds);
    FD_SET(0, &readfds);
    FD_SET(FDREAD, &readfds);//add the file descriptor to the set

    tv.tv_sec = 2;
    tv.tv_usec = 0;
    //                                          //

    struct token *newTok;
    //Get a message and send it before getting into the loop
    newTok = GetUserMessage(numProc, maxLength);

    Send(newTok, destID);

    while (CheckTermination(0) != 1) {
        select(fd_stdin + 1, &readfds, NULL, NULL, &tv);
        if (FD_ISSET(0, &readfds)) {

            //Check one more time before entering the blocking call
            if (CheckTermination(0)) break;

            newTok = GetUserMessage(numProc, maxLength);

            Send(newTok, destID);
        } else {
            int count;
            ioctl(FDREAD, FIONREAD, &count); //Check to see if there is anything to read
            if(count > 0) {
                int num = read(FDREAD, (void *) newTok, (size_t) sizeof(newTok) - 1);

                //Check that there is a message and that its from the right process
                if (num > 0 && newTok->src == numProc) {
                    //The message is for us
                    if (newTok->dest == myID) {
                        printf("Parent confirming message was received. Clearing message...\n");
                        printf("Enter a message: \n");
                        strcpy(newTok->message, "\n\0\0\0\0\0\0\0\0");
                        newTok->dest = 0;
                        fflush(stdout);
                        // Got the quit message back, time to quit
                    } else if (newTok->dest == -1) {
                        printf("Process %d is signing off\n", myID);
                        CheckTermination(1);
                    }
                        // Got a message for another process
                    else {
                        printf("Process %d received token meant for %d\n", myID, newTok->dest);
                    }
                    newTok->src = myID;
                    Send(newTok, destID);
                }
            }
        }
        FD_SET(FDREAD, &readfds); //place FDREAD back in the fd set
        FD_SET(0, &readfds); // place stdin back in the fd set

        if (CheckTermination(0))
            break;
    }

}

void ClearString(const int maxLength, char *message) {//reset the string
    for (int i = 0; i < maxLength; i++) {
        message[i] = '\0';
    }
}

void Send(struct token *tok, int dest) {
    printf("Passing token to %d\n", dest);
    fflush(stdout);
    write(FDWRITE, (const void *) tok, sizeof(struct token));
}


struct token *GetUserMessage(int numProc, const int maxLength) {
    char message[maxLength];
    ClearString(maxLength, message);

    //there is a left over \n
    printf("Enter a message: ");
    fflush(stdout);
    fflush(stdin);
    char c;

    while (strlen(message) <= 0)
        while ((c = getchar()) != '\n' && c != EOF)
            strncat(message, &c, sizeof(char));
//        fgets(message, maxLength, stdin);

    int dest;
    printf("Enter the destination (1-%d): ", numProc);
    fflush(stdout);
    fflush(stdin);
    scanf("%d", &dest);

    struct token myTok;
    struct token *newTok = &myTok;


    strcpy(newTok->message, message);
    newTok->dest = dest;
    newTok->src = 1;
    return newTok;
}