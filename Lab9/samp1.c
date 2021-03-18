#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SIZE 16

char *GetUserInput(char * str);

int main()
{
    char *data1;

    data1 = malloc (SIZE);
    printf ("Please input username: ");
//    fgets(data1, SIZE, stdin); // This is another option, but is has the potential for not reading all of the given input
    data1 = GetUserInput(data1); //
    printf ("you entered: %s\n", data1);
    free (data1);
    return 0;
}

/**
 * Reads the user input and returns a string. Will allocate memory to match the size of the user input.
 * Has a lot of overhead, so it's probably not the best for high speed applications.
 * @return A character pointer to the string.
 * @param str Pointer to the string to read into.
 * @author George
 */
char *GetUserInput(char *str){

    size_t size = SIZE;
    int ch;
    size_t len;

    str = realloc(NULL, sizeof(char)*size);//size is start size

    //Continue looping and reading characters from the console until a newline is found or the end of the input stream.
    for(len = 0; (EOF!=(ch=getc(stdin)) && ch != '\n'); len++){
        str[len]=ch;

        if(len==size-1) //If the string has filled the array, allocate more memory
        {
            str = realloc(str, sizeof(char)*(size+=16));
            if(!str)return str;
        }
    }
    str[len++]='\0'; //add the terminating null character at the end of the string
    return realloc(str, sizeof(char)*len); //reallocate so there is only enough memory for storing the string.
}
