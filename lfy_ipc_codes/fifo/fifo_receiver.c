/*Filename - fifo_receiver.c
Purpose - Demonstration of FIFO**************************/
#include<stdio.h>
#include<string.h>
#include<errno.h>
#include<unistd.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#define MAXBUF 80
int main (int argc, char *argv[])
{

  int res, count;
  char buffer[MAXBUF];

  if ( argc < 2 )
  {
		printf("pL. provide the name of the FIFO\n");
		exit(1);
  }

  res = mkfifo (argv[1],S_IRWXU); /* to create FIFo */

  if (0 == res)
  {
   	 printf ("\nFIFO created successfully\n");
  }
  else
  {
	if (errno == EEXIST)
	{
		printf("Fifo Already exists proceeding\n");
	}
	else{
   		 printf ("\nFIFO not Created");
	 	 perror ("FIFO Error:\n");
		 exit(1);

	}
  }
  res = open(argv[1], O_RDWR);

  /*In a Loop get data from user/send it to the receiver, 
  through the FIFO, till the time user presses exit*/

  while (1)
  {
  		count = read(res, buffer, MAXBUF);
		if (0 > count)
		{
			perror("Error in Writing\n");
			exit(1);
		}

		if (strcmp("exit\n", buffer) == 0)
		{
			printf("Receiver: Received -%s", buffer);
			break;
		}
		printf("Receiver: Received -%s", buffer);
  }
return 0;

}

