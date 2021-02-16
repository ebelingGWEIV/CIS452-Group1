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
#define WRDLEN 20

int status = 0;

void* workerFunction (void* arg);
int CheckTermination(int);
void sig_handler_parent(int);
void ResetString(char * str, int len);
// arguments :  arg is an untyped pointer pointing to a string
// returns :       a pointer to NULL
// side effects:  prints a greeting

///Number of currently running threads
int runningThreads = 0;

int main() {
    srand(time(0)); //give a seed to rand

    signal(SIGINT, sig_handler_parent);  //Register the signal handler
    pthread_t thread[NUM_THREADS]; //Create array of usable threads to keep track of
    char words[NUM_THREADS][20];
    int index = 0, requests = 0;

    fd_set readfds;
    int    num_readable;
    struct timeval tv;
    int    fd_stdin;

    fd_stdin = fileno(stdin);

    FD_ZERO(&readfds);
    FD_SET(0, &readfds);

    tv.tv_sec = 5;
    tv.tv_usec = 0;

    printf("Please enter file name:");

    while (1) { //This is going to need to have a break condition

        fflush(stdout);

        select(fd_stdin + 1, &readfds, NULL, NULL, &tv);
        if(FD_ISSET(0,&readfds))
        {
            /// Get the file name from the user
            if(CheckTermination(0)) break; //Check one more time before entering the blocking call
            char c;

            while((c = getchar()) != EOF && c != '\n')// Blocking
            {
                strncat(words[index], &c, sizeof(char));
            }

            requests++; // Another request was received
            /// Create a worker thread
            if ((status = pthread_create(&thread[index], NULL, workerFunction, &words[index])) != 0) {
                fprintf(stderr, "thread create error %d: %s\n", status, strerror(status));
                exit(1);
            }
            else{
                runningThreads++;
            }

            index = (index > 9) ? 0 : index + 1; // Increase index by 1 and reset to 0 once over 9
            ResetString(words[index], WRDLEN);
            printf("Please enter file name:");
        }
        else
        {
            FD_SET(0, &readfds); // place stdin back in the fd set
        }
        if(CheckTermination(0)) break;
}
    printf("\nWaiting for worker threads to terminate..."); // Show that the program is closing
    fflush(stdout);
    while(runningThreads); //Wait for all created threads to exit

    printf("\nAll threads have been closed");
    printf("\nNumber of requests received: %d\n", requests);
    return 0;
}

void ResetString(char * str, int len)
{
    while(len > 0)
    {
        str[len-1] = '\0';
        len--;
    }
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
   return;
}

void* workerFunction (void* arg)
{
    char *val_ptr = (char *) arg;
    char word[WRDLEN];
    strcpy(word, val_ptr);
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
    printf("\nWorker thread found: %s", word);
    fflush(stdout);

    // Thread has completed and can exit cleanly
    runningThreads--;
    pthread_exit(0);
}