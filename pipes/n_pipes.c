/*Filename - n_pipes.c**************************************************
Purpose - This code demonstrates, the use of pipe call. ****************
Here a circular ring is created.A parent process, accepts keyboard input.
It then reads data, and sends the data to its child1, child1, sends the 
data to child2 and so on. The nth  child send back the data to the parent.
This repeats, until the user presses exit.******************************
To compile use gcc n_pipes.c -o mypipe and then execute the code.
Version - 1.0 Date - 19.12.2012
************************************************************************/

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#define MAXCHILD 50
#define NO_OF_CHILDREN 20
#define MAXBUF 100

void child(int readend, int writeend);
int main()
{
	int pipefd[MAXCHILD + 1][2];
	int noofchildren = 0;
	int nchild = NO_OF_CHILDREN;
	int ret;
	int i ;
	int j ;
	char *retbuf;
	char buf[MAXBUF];
        for(i = 0; i <= nchild; i++)
	{
		ret = pipe(pipefd[i]);
		if (0 > ret)
		{
			printf("Pipe Creation failed\n");
			exit(1);
		}
	}
	printf("Pipes created\n");

        for(i = 0; i < nchild; i++)
	{
		
		ret = fork();
		if (0 == ret)
		{
			//In every child close all unused pipe descriptors
			for (j = 0; j <= nchild; j++)
			{
				if (j == i) 
				{ 
					close(pipefd[j][1]);
				}
				else if (j == i + 1)
				{
					close(pipefd[j][0]);
				}
				else
				{
					close(pipefd[j][0]);
					close(pipefd[j][1]);
				}
			}
			child(pipefd[i][0], pipefd[i + 1][1]);
		}
		//These ends are no longer needed in parent.
		close (pipefd[i][0]);
		close(pipefd[i + 1][1]);
	}

	printf("%d Children  Created\n", nchild);

	while(1) 
	{
		printf("Enter Some data -CTRL-D to exit:\n");
		//Now the parent/master reads from the standard input
		retbuf = fgets(buf, MAXBUF, stdin);
		if (NULL == retbuf)
		{
			printf("End of file or error\n");
			//To have a graceful exit, lets close the 
			//write end of my pipe.This allows
			//all other process to exit gracefully
			close (pipefd[0][1]);
			break;
		}
			
		//Write to the pipe which is connected to the first child.
		ret = write(pipefd[0][1],buf, strlen(buf) + 1);
		if (0 > ret)
		{
			perror("Error while writing to pipe:");
			exit(1);
		}
		//Read from the pipe, which is attached to the last child created
		ret = read(pipefd[nchild][0], buf, MAXBUF);
		if (0 > ret)
		{
			perror("Error while reading from pipe:");
			exit(1);
		}
		else if (ret == 0)
		{
			//Read returned 0, so lets exit now
			printf("Exiting - Got end of file\n");
			exit(0);
		}
		//Only print data if read returns some data
		printf("Master Read from the pipe - %s\n", buf);
		if (strcmp(buf, "exit\n") == 0)
		{
			printf("Exit received from the last process\n");
			break;
		}
	} 
	/*By this time all other children, would have exited
	Let's wait to see this******************************/ 
	while(1)
	{
		ret = wait(NULL);
		/*The wait call returns negative, when no more child is left*/
		if (0 > ret) //Returns -ve when no more child is left.
		{
			break;
		}
		noofchildren++;
	}
	printf("My %d children terminated! Now I am exiting", noofchildren);
	exit(0);
}
/**************************************************************************
Function - child - Acts as filter - reads from stdin and writes to stdout
***************************************************************************/
void child(int readend, int writeend)
{
	char buf[MAXBUF];
	int ret;
	//set up environment
	dup2(readend, 0);
	dup2(writeend,1);	
	while(1)
	{
		ret = read(0, buf, MAXBUF );
		if (strcmp(buf, "exit\n") == 0)
		{
			write(1, buf, ret);
			close(readend);
			close(writeend);
			exit(0);
		}
		else if (ret <= 0)
		{
			//If in any case end of file or error 
			close(readend);
			close(writeend);
			printf("Child exiting\n");
			exit(1);
		}
	 	write(1, buf, ret);
	}
}

