#include <stdio.h>
#include <dirent.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>
#include "string.h"

int main(int argc, char *argv[])
{
   DIR *dirPtr;
   struct dirent *entryPtr;
   int showInode = 0;
   int showID = 0;

   if (argc < 2) {
        printf ("Usage: filename required\n");
        exit(1);
    }
   if(argc >= 3)
   {
       // Figure out which arguments were given
       showInode = strchr(argv[2], 'n') != 0 ? 1 : 0;
       showID = strchr(argv[2], 'i') != 0 ? 1 : 0;
   }

    dirPtr = opendir (argv[1]);

    while ((entryPtr = readdir (dirPtr))) {
        //format the filename so that it points to a file in the specified directory
        char fileName[20];
        sprintf(fileName, "%s/%s", argv[1], entryPtr->d_name);

       //Get the new stat buffer
       struct stat statBuf;
       if (stat (fileName, &statBuf) < 0) {
           perror ("huh?  there is ");
           exit(1);
       }
       // (poorly) Handle the input conditions
       if(showInode && showID)
       {
           printf("%-20s inode #: %lu group ID: %d user ID: %d\n", entryPtr->d_name, statBuf.st_ino, statBuf.st_gid, statBuf.st_uid );
       }
       else if(showInode  && ! showID)
       {
           printf("%-20s inode #: %lu \n", entryPtr->d_name, statBuf.st_ino );
       }
       else if(!showInode && showID)
       {
           printf("%-20s group ID: %d user ID: %d\n", entryPtr->d_name, statBuf.st_gid, statBuf.st_uid );
       }
       else if(!showInode && !showID)
       {
           printf("%-20s\n", entryPtr->d_name);
       }
   }

   closedir (dirPtr);
   exit(0);
} 
