#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <fcntl.h>

void sigHandler (int);

int main()
{
	int fd = open("out.txt", O_APPEND|O_WRONLY);

	int pid = fork();

	if(pid == 0)
	{
		printf("child output\n");
		exit(0);
	}
	else{
		dup2(fd,1);
		printf("parent output\n");
	}


	close(fd);

	printf("test output\n");
    	return 0;
}

