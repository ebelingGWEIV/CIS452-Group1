#include <stdio.h>
#include <dirent.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>

int main()
{
   DIR *dirPtr;
   struct dirent *entryPtr;
   struct stat statBuf;

   dirPtr = opendir (".");


   while ((entryPtr = readdir (dirPtr))) {
       if (stat (entryPtr->d_name, &statBuf) < 0) {
           perror ("huh?  there is ");
           exit(1);
       }
       printf("%-20s size: %ld bytes\n", entryPtr->d_name, statBuf.st_size);
   }

   closedir (dirPtr);
   return 0;
} 
