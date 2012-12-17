/*Filename - shm_sender.c
Purpose - Demonstration of Shared Memory and Semaphore***
Steps - Pl. make sure to have the file "shm_reciver.c" in your 
directory, since that is used as an argument to ftok function.
While compiling pl use the Linker flag -lrt that is 
compile like this gcc shm_sender -o sender -lrt 
(Pl. note run the sender first and then the reciver)
Run the sender first  in one window and reciver in another window*/

#include<stdio.h>
#include<string.h>
#include<unistd.h>
#include<stdlib.h>
#include<semaphore.h>
#include<sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <fcntl.h>
#include<sys/stat.h>
#define MAXBUF 80	
int main (int argc, char *argv[])
{

  int shmid,status ;
  char buffer[MAXBUF];
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
  /*Create two  POSIX Named Semaphores, get and put*/

  get = sem_open("/get", O_CREAT|O_RDWR, 0644, 0);

  if (SEM_FAILED == get)
  {
		printf("I am not able to create/access Semaphore\n");
		exit(1);
  }

  put = sem_open("/put", O_CREAT|O_RDWR, 0644, 1);

  if (SEM_FAILED == put)
  {
		printf("I am not able to create/access Semaphore\n");
		exit(1);
  }
  /*Attaching the Shared memory segment to my address space*/
 
  virtualaddr = shmat(shmid,(void*)0,0);
  if (virtualaddr == (void*)-1)
  {
		printf("Failed To attach Shared Memory\n");
		exit(1);
  }


  /*In a Loop get data from user/send it to the receiver, 
  through the Shared Memory, till the time user presses exit*/

  while (1)
  {
		printf("Pl. enter some data\n");
		fgets(buffer, MAXBUF, stdin);
                //Wait over the semaphore till the reciver has 
		//Received the earlier data, so that data
		//Which is not yet read is not overwritten
		sem_wait(put);
		strcpy(virtualaddr, buffer);
		sem_post(get);
		if (strcmp("exit\n", buffer) == 0)
		{
			break;
		}
  }
  status = sem_unlink("/get");
  if (0 > status)
  {
		printf("Deletion of Semaphore named get was unsucessful\n");
  }
  status = sem_unlink("/put");
  if (0 > status)
  {
		printf("Deletion of Semaphore named put was unsucessful\n");
  }
  status = shmctl(shmid, IPC_RMID, NULL);
  if (0 > status)
  {
		printf("Deletion of Shared Memory was unsucessful\n");
  }
  return 0;
  

}

