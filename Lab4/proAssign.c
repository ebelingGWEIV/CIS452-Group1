#include <pthread.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <time.h>
#include <poll.h>

#define NUM_THREADS 10
#define MAXBYTES 80

int status = 0;

void* workerFunction (void* arg);
int CheckTermination(int);
void sig_handler_parent(int);
// arguments :  arg is an untyped pointer pointing to a string
// returns :       a pointer to NULL
// side effects:  prints a greeting

///Number of currently running threads
int runningThreads = 0;

int main() {
    srand(time(0)); //give a seed to rand

    signal(SIGINT, sig_handler_parent);  //Register the signal handler
    pthread_t thread[NUM_THREADS]; //Create array of usable threads to keep track of

    int index = 0, requests = 0;

    fd_set readfds;
    int    num_readable;
    struct timeval tv;
    int    num_bytes;
    char   buf[MAXBYTES];
    int    fd_stdin;

    fd_stdin = fileno(stdin);

    FD_ZERO(&readfds);
    FD_SET(fileno(stdin), &readfds);

    tv.tv_sec = 5;
    tv.tv_usec = 0;

    printf("Please enter file name:");

    while (1) { //This is going to need to have a break condition
        char fileName[20]; //Hopefully, this can be a local variable. If there is undefined behavior occuring in the worker thread, this being deallocated and reallocated as a new filename might be the cause.

        fflush(stdout);

        num_readable = select(fd_stdin + 1, &readfds, NULL, NULL, &tv);
        if(num_readable <= 0){ //Happens when select is interrupted with a signal handler and when the timer has run out
            if(CheckTermination(0)) {
                break;
            }

        }
        else{
            /// Get the file name from the user
            scanf("%s", fileName); // Blocking
            requests++; // Another request was received
            /// Create a worker thread
            if ((status = pthread_create(&thread[index], NULL, workerFunction, &fileName)) != 0) {
                fprintf(stderr, "thread create error %d: %s\n", status, strerror(status));
                exit(1);
            }
            else{
                runningThreads++;
            }

            index = (index > 9) ? 0 : index + 1; // Increase index by 1 and reset to 0 once over 9

            printf("Please enter file name:");
        }
}
    printf("Waiting for worker threads to terminate...\n"); // Show that the program is closing

    while(runningThreads); //Wait for all created threads to exit

    printf("\nNumber of requests received: %d\n", requests);
    return 0;
}

int CheckTermination(int term) {
    static int terminate = 0;
    if(term != 0) {
        printf("terminate set\n");
        terminate = 1;
    }
    return terminate;
}

/* Signal Handler for the Parent Process */
void sig_handler_parent(int signum) {
    printf("got a thing\n");
   CheckTermination(1);
   return;
}

void* workerFunction (void* arg)
{
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

    // Thread has completed and can exit cleanly
    runningThreads--;
    pthread_exit(0);
}