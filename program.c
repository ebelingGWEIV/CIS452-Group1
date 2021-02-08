//Lab Assignment Three
//Riley Hernandez and George Ebeling

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <time.h>


/* Signal Handler for the Parent Process */
void sig_handler_parent(int signum) {
	if(signum == SIGUSR1) {
		printf("received a SIGUSR1 signal\n");
    	}

   	else if(signum == SIGUSR2) {
		printf("received a SIGUSR2 signal\n");
    	}

	else{
		printf(" recieved. That's it, I'm shutting you down...\n");
		CheckTermination(1);
	}
}

/* Signal Handler for the Child Process */
void sig_handler_child(int signum){
	CheckTermination(1);
}

/* Checks the Termination status of a signal handler in order to know when to exit gracefully */
int CheckTermination(int term) {
	static int terminate = 0;
	if(term != 0) {
		terminate = 1;
    	}
	return terminate;
}


int main() {
	int timeMin = 1;
	int timeMax = 5;
	pid_t pid;
	srand(time(0));
	
	pid = fork();

	/* checks if fork has failed */
	if (pid < 0){
		perror("Fork Failed");
		exit(1);
	}
	
	/* Child Process */
	if (pid == 0){
		signal(SIGUSR1, sig_handler_child);
		pid_t parent = getppid();

		while(1) {
			sleep(timeMin + rand() % (timeMax + 1 - timeMin)); //Child process waits for 1-5 seconds
			if (rand() % 2 == 0) { //SIGUSR1 = odd, SIGUSR2 = even
				kill (parent, SIGUSR1);
			}
			else {
				kill (parent, SIGUSR2);
			}
		}
	}
	
	/* Parent Process */
	else {
		signal(SIGUSR1, sig_handler_parent); //Register the signal handler
		signal(SIGUSR2, sig_handler_parent); //Register the signal handler
		signal(SIGINT, sig_handler_parent);  //Register the signal handler
		while(1){
			printf("waiting...        ");
			fflush(stdout); //buffers the wait print and the recieved print statements
			pause();
			if (CheckTermination(0)) { //kills program if signalled to terminate via ^C in the signal handler
				kill(pid, SIGUSR1);
				return(0);
			}
		}
	}

	return 0;
	
}
