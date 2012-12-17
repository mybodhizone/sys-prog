/*Filename - msg_sender.c ***************
Purpose - demonstration of System V Message Queue
Steps - Make sure you have the file msg_receiver.c in your 
current directory since that is used as one argument 
to ftok function.
Then compile both the msg_reciver.c and msg_sender.c giving 
name of the executables as say "r" and "s".
Run the sender "s" from one terminal and the receiver "c"
from another terminal.
you may use  ipcs command to see the Message Q created
*/
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<semaphore.h>
#include<sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <fcntl.h>
#include<sys/stat.h>
#define MAXBUF 80
struct msbuf {
        long mtype;
        char mstore[MAXBUF];
};

int main (int argc, char *argv[])
{

  char buffer[MAXBUF];
  key_t key;
  int msgid, ret;
  struct msbuf message;
  key = ftok("msg_receiver.c",'B');

  msgid = msgget(key,0644|IPC_CREAT);

  	if (0 > msgid)
	{
        	printf("Was Unable To Create/Access the MQ\n");
        	exit(1);
	}

  /*In a Loop get data from user/send it to the receiver, 
  through the Message queue, till the time user presses exit*/

  while (1)
  {
		printf("Pl. enter some data\n");
		fgets(buffer,MAXBUF, stdin);
		/*Create the Message, which is to be put  the queue*/

		message.mtype = 1;
		strcpy(message.mstore, buffer);

 		ret = msgsnd(msgid, &message, (sizeof(message)-sizeof(long)),0);
 		if (0 > ret)
 		{
         		printf("Failure in Sending Message\n");
         		perror("Message Send Error\n");
         		exit(1);
		 }
		if (strcmp("exit\n", buffer) == 0)
		{
				break;
		}
  }
  /*Delete the queue now*/
  ret = msgctl(msgid, IPC_RMID, NULL);
  if (0 > ret)
  {
		printf("Error in deleting message queue\n");
   }
	return 0;

}

