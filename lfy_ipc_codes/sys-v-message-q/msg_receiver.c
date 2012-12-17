#include<stdio.h>
#include<string.h>
#include<unistd.h>
#include<stdlib.h>
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

  int retrecv ;
  struct msbuf message;
  int msgid;
  key_t key;

	key = ftok("msg_receiver.c",'B');
	if (0 > key)
	{
		printf("Error in Key Creation\n");
		exit(1);
	}
	/*Trying To get a Message Q descriptor*/
	/*if the Q is already there, it wont be recreated*/

	msgid = msgget(key,0644|IPC_CREAT);

	if (0 > msgid)
	{
		printf("Was Unable To Create/Access the MQ\n");
		perror("Message Q Error:\n");
		exit(1);
	}


  /*In a Loop Receive Read Message queue*/

  while (1)
  {             
	        retrecv = msgrcv(msgid, &message, (sizeof(message)-sizeof(long)),1,0);
		if (0 > retrecv)
		{
			printf("Receiver - Error in message receiving\n");
			exit(1);
		}
		if (strcmp("exit\n", message.mstore) == 0)
		{
			printf("Receiver Received -%s", message.mstore);
			break;
		}
		printf("Receiver Received -%s", message.mstore);
  }
	return 0;

}

