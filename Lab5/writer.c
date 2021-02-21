#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <signal.h>
#include <time.h>
#include "SharedStructure.h"

#define FOO 4096

void clearMem(const char *shmPtr, int length);
void Close(int shmId, const char *shmPtr);
int checkDisplayStatus(const struct messageStruct *sharedFile);
int CheckTermination(int term);
void resetDisplayFlags(struct messageStruct *sharedFile);
void sig_handler_parent(int);

int main ()
{
    signal(SIGINT, sig_handler_parent);  //Register the signal handler
    int shmId;
    char *shmPtr;
    int continueFlag = 1;

    //Get the shared memory pointer to the file
    key_t memKey = ftok("shared.txt", 101);

    if ((shmId = shmget (memKey, FOO, IPC_CREAT|S_IRUSR|S_IWUSR)) < 0) {
        perror ("I can't get no..\n");
        exit (1);
    }
    if ((shmPtr = shmat (shmId, 0, 0)) == (void*) -1) {
        perror ("can't attach\n");
        exit (1);
    }
    clearMem(shmPtr, FOO);

    fd_set readfds;
    struct timeval tv;
    int    fd_stdin;
    fd_stdin = fileno(stdin);
    FD_ZERO(&readfds);
    FD_SET(0, &readfds);
    tv.tv_sec = 5;
    tv.tv_usec = 0;

    // Get a usable pointer to the file
    struct messageStruct *sharedFile = (struct messageStruct *)shmPtr;

    // Check for input and send to shared memory until flag is reset
    while(continueFlag) {

        fflush(stdout);

        select(fd_stdin + 1, &readfds, NULL, NULL, &tv);
        if(FD_ISSET(0,&readfds)) {
            //Check one more time before entering the blocking call
            if(continueFlag = CheckTermination(0)) break;

            //Get user message
            char c;
            char message[MESSAGE_LEN];
            while((c = getchar()) != EOF && c != '\n')// Blocking
            {
                strncat(message, &c, sizeof(char));
                if(continueFlag = CheckTermination(0)) break; //Check before entering the blocking call again
            }
            // To break the main while-loop, need to check one more time
            if(continueFlag = CheckTermination(0)) break;

            //Copy the message to file
            strcpy(sharedFile->message, message);
        }

        //Wait until the message has been displayed by all readers
        while (1) {
            if (checkDisplayStatus(sharedFile)) break;
            if(continueFlag = CheckTermination(0)) break;
        }

        resetDisplayFlags(sharedFile);
    }

    free(sharedFile);
    Close(shmId, shmPtr);

    return 0;
}


/**
 * To assist with exiting gracefully by marking when it is time to exit by the signal of ^C
 * @param term 0 to check, 1 to set
 * @return 1 to terminate
 */
int CheckTermination(int term) {
    static int terminate = 0;
    if(term != 0) {
        terminate = 1;
        printf("term is 1");
    }
    return terminate;
}

/* Signal Handler for the Parent Process */
void sig_handler_parent(int signum) {
    CheckTermination(1);
    return;
}

/**
 * Reset all reader flags back to zero.
 * @param sharedFile Pointer to shared memory
 */
void resetDisplayFlags(struct messageStruct *sharedFile) {
    for(int i = 0; i < NUM_READERS; i++)
    {
        sharedFile->readers[i] = 0;
    }
}

/**
 * Check if all readers have set flags to 1.
 * @param sharedFile Pointer to shared memory
 * @return Return 1 is all readers have set their flags
 */
int checkDisplayStatus(const struct messageStruct *sharedFile) {
    //Check if message has been displayed
    for(int i = 0; i < NUM_READERS; i++)
    {
        if(sharedFile->readers[i] != 0)
            return 0;
    }
    return 1;
}

/**
 * Detach from the shared memory
 * @param shmId id given by shmget
 * @param shmPtr pointer to shared memory
 */
void Close(int shmId, const char *shmPtr) {
    if (shmdt (shmPtr) < 0) {

        perror ("just can't let go\n");
        exit (1);
    }
    if (shmctl (shmId, IPC_RMID, 0) < 0) {
        perror ("can't deallocate\n");
        exit(1);
    }
}

/**
 * Fills the shared memory with '\0'
 * @param shmPtr pointer to memory location
 * @param length number of char to clear
 */
void clearMem(const char *shmPtr, int length) {
    int i;
    for(i = 0; i < length; i++) {
        *(((char *)shmPtr)+i) = 0;
    }
}
