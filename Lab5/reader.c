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

int main () {
	int shmId;
	int readerIndex = 0;
	int position = 0;
	char *shmPtr;
	
	//Get the shared memory pointer to the file
	key_t memKey = ftok("shared.txt", 101);

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

	for (int i = 0; i < NUM_READERS; i++) {
		if (sharedFile->readers[i] == 1) {
			readerIndex = i;
		}
	}

	while(1) {
		sharedFile = (struct messageStruct *) shmPtr;
		if (sharedFile->readers[readerIndex] == 1){
			printf("Note: %s", sharedFile->message);
			sharedFile->readers[readerIndex] = 0;
			position++;
		}

		if (position > MESSAGE_LEN) {
			position = 0;
		}
		
		sharedFile = (struct messageStruct *) shmPtr;
		while(sharedFile->readers[readerIndex] != 1);
	}

	if (shmdt (shmPtr) < 0) {
		perror ("Can't deattach\n");
		exit(1);
	}

	return 0;
}