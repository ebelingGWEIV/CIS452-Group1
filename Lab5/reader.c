#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <signal.h>
#include "SharedStructure.h"
#include <unistd.h>

#define FOO 4096

int main () {
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
                printf("marking %d reader as: %d", i, sharedFile->readers[i]);
                hasReaderSpot = 1;
            }
        }
        printf("could not get a reader spot\n");
    }
    sharedFile = (struct messageStruct *) shmPtr;

	while(1) {

		if (sharedFile->readers[readerIndex] == 0){
			printf("Note: %s\n", sharedFile->message);
			sharedFile->readers[readerIndex] = 1; //set the reader display status
//			position++;
//			fflush(stdout);
		}

//		if (position > MESSAGE_LEN) {
//			position = 0;
//		}
		
		sharedFile = (struct messageStruct *) shmPtr;
	}

	if (shmdt (shmPtr) < 0) {
		perror ("Can't deattach\n");
		exit(1);
	}

	return 0;
}