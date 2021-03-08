#ifndef CIS452_GROUP1_SIGHANDLER_H
#define CIS452_GROUP1_SIGHANDLER_H

#include <signal.h>

int CheckTermination(int term);
void MarkForTermination(int signum);

void OverrideSIGNINT()
{
    signal(SIGINT, MarkForTermination);  //Register the signal handler
}

/* Funciton to exit gracefully by marking when it is time to exit by the signal of ^C */
int CheckTermination(int term) {
    static int terminate = 0;
    if(term != 0) {
        terminate = 1;
    }
    return terminate;
}

/* Mark the process for termination */
void MarkForTermination(int signum) {
    CheckTermination(1);
}

#endif //CIS452_GROUP1_SIGHANDLER_H
