#include <pthread.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <time.h>

#define NUM_THREADS 10

int status = 0;

void* workerFunction (void* arg);
int CheckTermination(int);
void sig_handler_parent(int);
// arguments :  arg is an untyped pointer pointing to a string
// returns :       a pointer to NULL
// side effects:  prints a greeting

int main() {
    srand(time(0)); //give a seed to rand

    signal(SIGINT, sig_handler_parent);  //Register the signal handler
    pthread_t thread[NUM_THREADS];


// create and start two threads executing the "workerFunction" function
// pass each thread a pointer to its respective argument

int index, requests;

    while (1) { //This is going to need to have a break condition
        char fileName[20]; //Hopefully, this can be a local variable. If there is undefined behavior occuring in the worker thread, this being deallocated and reallocated as a new filename might be the cause.

        if(CheckTermination(0)) break; //If marked for termination, exit the loop

        /// Get the file name from the user
        printf("Please enter file name: "); fflush(stdout);
        scanf("%s", fileName);
        requests++; //Another request was received

        /// Create a worker thread
        if ((status = pthread_create(&thread[index], NULL, workerFunction, &fileName)) != 0) {
            fprintf(stderr, "thread create error %d: %s\n", status, strerror(status));
            exit(1);
        }
        index = (index > 9) ? 0 : index + 1; //increase index by 1 and reset to 0 once over 9
}
    printf("Waiting for worker threads to terminate...\n"); // Show that the program is closing

    // join with the threads (wait for them to terminate);
    for(int i = 0; i <= 9; i++) {
        if ((status = pthread_join(thread[i], &thread[i])) != 0) {
            fprintf(stderr, "join error %d: %s\n", status, strerror(status));
            exit(1);
        }
    }
    printf("\nNumber of requests received: %d\n", requests);
    return 0;
}

int CheckTermination(int term) {
    static int terminate = 0;
    if(term != 0) {
        terminate = 1;
    }
    return terminate;
}
/* Signal Handler for the Parent Process */
void sig_handler_parent(int signum) {

   CheckTermination(1);
}

void* workerFunction (void* arg)
{
// note the cast of the void pointer to the desired data type
    char *val_ptr = (char *) arg;
    int quick = 8;
    int slow = 10;
    int timeMin = 1;
    int timeMax = 10;

    int sleepTime = rand() % (slow + 1 - timeMin);
    if(sleepTime <= quick)
    {
        sleep(timeMin);
    }
    else
    {
        int timeMin = 7; //min time is now 7
        int sleepTime = rand() % (timeMax + 1 - timeMin);
        sleep(sleepTime);
    }
    printf("Worker thread got %s\n", val_ptr);


// print out a message
    pthread_exit(0);
} 
