/*File name - select_file_pipe.c ***************************************
Purpose - Shows an example of use of select call.The select call is 
waiting over two descripors one a pipe and another a regular 
file called datafile.
PL. have a file named  datafile in your working directory**************/

#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#define MAXBUF 100

int main(void)
{
    fd_set rfds;
    int retval;
    int  ret;
    int maxdes = 0; //this is required for select's first argument
    int mypipe[2]; //descriptor array for the pipe.
    char buf[MAXBUF];

    int fd_file = open("datafile", O_RDONLY);
    if (0 > fd_file)
    {
		printf("Error opening file\n");
		perror("File Error:");
		exit(1);
    }


    /*Now let me create a pipe.
    The parent will try to read from the pipe, if data is available 
    The child will write something on the pipe after an 
    interval of say 3 secs*/
   ret = pipe(mypipe);
   if (0 > ret)
   {
		printf("Pipe Creation Failed\n");
		exit(1);
   }
   
   ret = fork();
   if (0 ==ret)
   {
	//Child - does not need the read end of the pipe
	close(mypipe[0]); //Closed read end
	while(1)
	{
		sleep(3);
		/*Ideally I should be handling errors too after writing. 
                If the below line is commented, then 
                select will not report readiness for the pipe*/
		write(mypipe[1], "Child-Hello\n", 12);
	}
    }
   else if (0 > ret)
   {
	printf("Could Not fork\n");
	perror("Fork ..");
	exit(1);
   }

   
   /*This is the parent
   Let me close the write end of the pipe, since I will be reading*/

   close(mypipe[1]);
    FD_ZERO(&rfds); //Clear the read set
    /*Need to find maximum of two descriptors 
   In this case though the pipe descriptor should be greater, still checking.*/
   if (mypipe[0] > fd_file)
   {
		maxdes = mypipe[0] + 1;
   }
   else
   {           
		 maxdes = fd_file + 1;
   }
		
   while (1)
   {
     
     //Pl. note the below two lines are important 
     FD_SET(fd_file, &rfds);//This is the file
     FD_SET(mypipe[0], &rfds);//This is the read end of the pipe.
     /*maxdes tell select how many descriptor slots at most it should
     check ..that should be the maximum descriptor + 1 since
     descriptors start from 0*/

     retval = select(maxdes, &rfds, NULL, NULL, NULL);
     if (retval == -1)
     {
        perror("select()");
	exit(1);
     }

     else if (retval > 0)
     {
	//Check only the descriptor referring the file opened
	if (FD_ISSET(fd_file, &rfds))
	{
		printf("select - showing ready for file descriptor %d\n", 
		fd_file);
	}
	if (FD_ISSET(mypipe[0], &rfds))
	{
		printf("select - showing ready for pipe\n");
		//Let me now read from the pipe
		ret = read(mypipe[0], buf, MAXBUF); 
		//Not taken care of buffer overflows
		buf[ret] ='\0';
		printf("Parent - Received over pipe - %s\n", buf);
		//Just to Halt so that the user sees this output
                sleep(1);
	}
     }
     else
     {
		/*Case for select's return 0 ..Seems this will not come
		I have not set time out, anyhow let me continue here*/
		continue;
     }

   } //End of While 
   //The line below will never be excuted in the currrent program
   exit(EXIT_SUCCESS);
}

