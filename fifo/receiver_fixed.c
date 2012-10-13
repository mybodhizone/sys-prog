#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#define MAX_LINE 80

int main()
{
        int ret = -1;
        int fd = -1;
        char buff[MAX_LINE];

        ret = mkfifo("Named_Pipe",S_IRWXU);


        if (0 == ret)
        {
                printf("Name Pipe Created Successfully...\n");
        }
        else
        {
                if (EEXIST == errno)
                {
                        printf("Named Pipe Already Exist...\n");
                }
                else
                {
                        printf("Can't create Pipe...\n");
                        exit(0);
                }
        }
	//Here the process will get blocked, till some one else
	//opens the Fifo for writing
        fd = open("Named_Pipe",O_RDONLY);
        if (0 > fd)
        {
                printf("Named Pipe Can't Opened\n");
                exit(0);
        }

        while ((ret = read(fd,buff,MAX_LINE))>0)
        {

                printf("Received : %d\n",ret);
                write(1,buff,ret);
        }

        return 0;

}

