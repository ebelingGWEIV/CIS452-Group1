#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/resource.h>

#define LINE_LENGTH 128

/***
 * Tokenizes string by spaces
 * @param string
 * @param parsedString
 * @return The number of tokens put into parsedString
 */
int Parser(char* string, char *parsedString[]){
    char *token;
    const char tok[3] = " \n";
    /* get the first token */
    token = strtok(string, tok);

    int index;
    /* walk through other tokens */
    for(index = 0; token != NULL; index++ ) {
        parsedString[index] = malloc(strlen(token) + 1); //Reserve space for the string
        strcpy(parsedString[index], token);
        token = strtok(NULL, tok);
    }
    parsedString[index++] = NULL;
    return index;
}

/***
 * This skeleton of this function came from Sample Program 3.
 * Creates a child that runs the command using execvp.
 * @param command The parsed command
 */
void Execute(char * command[])
{
    id_t pid, child;
    int status = 0;
    struct timeval start, end;
    long startSwitch, endSwitch;
    struct rusage usage;

    if ((pid = fork()) < 0) {
        perror("fork failure"); //This error seems big enough that exiting is okay
        exit(1);
    }
    else if (pid == 0) {
        //This was taken from Sample Program 4
        if (execvp(command[0], command) < 0) {
            //We don't want to throw a perror() because the program should continue running.
            //If execvp returns, the command failed to be executed.
            exit(1);
        }
    }
    else {
        ///4. Additional Requirement
        //Get start time
        getrusage(RUSAGE_CHILDREN, &usage);
        start = usage.ru_utime;

        child = wait(&status);

        //Get end time
        getrusage(RUSAGE_CHILDREN, &usage);
        end = usage.ru_utime;

        if(status > 0) printf("Command failed to execute %d\n", status);

        else {
            printf("User CPU time: %ld seconds and %ld micro-seconds\n", (end.tv_sec - start.tv_sec),
                   (end.tv_usec - start.tv_usec));
        }
        return;
    }
}

int main(int argc, char* argv[])
{
    char command[LINE_LENGTH];
    char* parsed[LINE_LENGTH]; //Array of strings

    struct rusage usage;
    long startSwitch, endSwitch;

    getrusage(RUSAGE_CHILDREN, &usage);
    startSwitch = usage.ru_nivcsw;

    ///5. (out of order, but this is where it happens) Repeat until the user enters quit
    while(1) {
        ///1. display a prompt to the user
        puts("Enter a command >> ");

        ///2. Get and parse the user input
        fgets(command, LINE_LENGTH, stdin);

        Parser(command, parsed);

        if(strcmp("quit", parsed[0]) == 0) break;
        ///3. Spawn a child process to execute the command
        Execute(parsed);


    }

    ///4. Additional Requirement
    getrusage(RUSAGE_CHILDREN, &usage);
    endSwitch = usage.ru_nivcsw;
    printf("Involuntary context switches %ld\n", (endSwitch - startSwitch));
    return 0;
}