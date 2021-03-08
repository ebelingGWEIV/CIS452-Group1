#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include "Token.h"

#define READ 0
#define WRITE 1

#define FDWRITE 4200
#define FDREAD 4300

void Send(struct token *tok, int dest);

/**
 * Must be compiled with the output file as 'child'
 */
int main(int argc, char *argv[])
{
    int myID;
    int receiverID;
    if(argc >= 3) {
        myID = atoi(argv[1]);
        receiverID = atoi(argv[2]);
    }
    else
    {
        printf("Expected arguments for virtual IDs\n");
        return 1;
    }
    struct token myTok;
    struct token *newTok = &myTok;

    do{
        int num = read (FDREAD, (void *) newTok, (size_t)  sizeof (struct token));
        if(num > 0 && newTok->src == myID-1) {
            if (newTok->dest == myID) {
                newTok->dest = 1;
                printf("Child %d received message: %s\n", myID, newTok->message);
            } else if (newTok->dest == -1) {
                printf("Child %d is signing off\n", myID);
            } else {
                printf("Child %d received token meant for %d\n", myID, newTok->dest);
            }
            sleep(1); //Adding some delay to make it more readable for the user
            newTok->src = myID;
            Send(newTok, receiverID);
        }
        else if(num < 0){
            perror("failed to read");
            newTok->dest = -1;
            Send(newTok, receiverID);
        }

    }while(newTok->dest != -1);

    return(0);

}

void Send(struct token *tok, int dest)
{
    fflush(stdout);
    write (FDWRITE, (const void *) tok, sizeof(struct token));
}