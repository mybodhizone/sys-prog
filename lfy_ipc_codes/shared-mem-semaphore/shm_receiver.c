#include<stdio.h>
#include<string.h>
#include<unistd.h>
#include<stdlib.h>
#include<semaphore.h>
#include<errno.h>
#include<sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <fcntl.h>
#include<sys/stat.h>
int main (int argc, char *argv[])
{

  int shmid;
  char buffer[80];
  key_t key;
  char *virtualaddr;
  sem_t *get, *put;

  key = ftok("shm_receiver.c",'R');

  shmid = shmget(key,1024,0644|IPC_CREAT);

  if (-1 == shmid)
  {
        printf("Failed to Create Shared Memory\n");
        exit(1);
  }
  /*Access two  POSIX Named Semaphores, get and put*/

  get = sem_open("/get", O_RDWR);

  if (SEM_FAILED == get)
  {
		printf("I am not able to create/access Semaphore\n");
		if (ENOENT == errno)
		{
			printf("May be semaphore does not exists\n");
			printf("Try running the sender program first\n");
		}
			 
		exit(1);
  }

  put = sem_open("/put", O_RDWR);

  if (SEM_FAILED == put)
  {
		printf("I am not able to create/access Semaphore\n");
		if (ENOENT == errno)
		{
			printf("May be semaphore does not exists\n");
			printf("Try running the sender first\n");
		}
		exit(1);
  }
  /*Attaching the Shared memory segment to my address space*/
 
  virtualaddr = shmat(shmid,(void*)0,0);
  if (virtualaddr == (void*)-1)
  {
		printf("Failed To attach Shared Memory\n");
		exit(1);
  }


  /*In a Loop Receive Read data From the Shared Memory*/

  while (1)
  {
		//Waiting till the sender signals the semaphore             
		sem_wait(get);
		strcpy(buffer, virtualaddr);
                //Now signalling the semaphore put so that 
		//now the sender can write data on the 
		//shared memory
		sem_post(put);
		if (strcmp("exit\n", buffer) == 0)
		{
			printf("Receiver Received -%s", buffer);
			break;
		}
		printf("Receiver Received -%s", buffer);
  }
  return 0;

}

