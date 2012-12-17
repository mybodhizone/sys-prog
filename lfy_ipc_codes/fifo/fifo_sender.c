/*Filename - fifo_sender.c
Purpose - Demonstration of FIFO*********************
Steps - >Compile - gcc fifo_sender.c -o sender and then run sender
in one terminal. Then it waits for user input by using fgets.
Enter the string,terminated with enter from keyboard.
From another window run the sender.You need to enter "exit" to 
the receiver. Both the sender and receiver should terminate now.
Very Large strings will not be handled properly in this version*/

#include<stdio.h>
#include<string.h>
#include<unistd.h>
#include<errno.h>
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
        	printf("Fifo Already exists\n");
	}
	else
	{
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
		printf("Pl. enter some data\n");
		fgets(buffer, MAXBUF, stdin);
  		count = write(res, buffer, strlen(buffer) + 1);
		if (0 > count)
		{
			perror("Error in Writing\n");
			exit(1);
		}
		if (strcmp("exit\n", buffer) == 0)
		{
			break;
		}
  }
  //Delete the fifo
   res = unlink(argv[1]);
  if (0 > res)
  {
		printf("Fifo deletion was unsuccessful\n");
		perror("Fifo Deletion error\n");
  }
	  
return 0;
}

