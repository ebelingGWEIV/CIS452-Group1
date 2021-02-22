#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <signal.h>
#include "SharedStructure.h"

#define FOO 4096

void sig_handler(int);
int CheckTermination(int term);

int main () {
    signal(SIGINT, sig_handler);  //Register the signal handler
	int shmId;
	int readerIndex = 0;
	int position = 0;
	char *shmPtr;
	
	//Get the shared memory pointer to the file
	key_t memKey = ftok("test", 1);

	if ((shmId = shmget(memKey, FOO, S_IRUSR)) < 0) {
		perror ("I can't find...\n");
		exit(1);
	}

	if ((shmPtr = shmat(shmId, 0, 0)) == (void*) -1) {
		perror ("Can't attach\n");
		exit(1);
	}

	//Usable pointer to the file
	struct messageStruct *sharedFile = (struct messageStruct *) shmPtr; 
	char c;
    int hasReaderSpot = 0;
    while(!hasReaderSpot) {
        for (int i = 0; i < NUM_READERS && !hasReaderSpot; i++) {
            if (sharedFile->readers[i] == 0) {
                readerIndex = i;
                sharedFile->readers[i] = i + 1; //tell other readers that this space is being used
                hasReaderSpot = 1;
            }
        }
        printf("Could not get a reader spot\n");

    }
    sharedFile = (struct messageStruct *) shmPtr;

	while(1) {

		if (sharedFile->readers[readerIndex] == 0){
			printf("Note: %s\n", sharedFile->message);
			sharedFile->readers[readerIndex] = 1; //set the reader display status
		}
		if(CheckTermination(0)) break;

		sharedFile = (struct messageStruct *) shmPtr;
	}

	if (shmdt (shmPtr) < 0) {
		perror ("Can't deattach\n");
		exit(1);
	}

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
    }
    return terminate;
}

/* Signal Handler for the Parent Process */
void sig_handler(int signum) {
    CheckTermination(1);
    return;
}