#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
int main()
{
        int ret = -1;
        int fd = -1;


        ret = mkfifo("Named_Pipe",S_IRWXU);

        if (0 == ret)
        {
                printf("Named Pipe Created Successfully....\n");

        }
        else
        {
                if (EEXIST == errno)
                {
                        printf("Named Pipe already exist....\n");
                }
                else
                {
                        printf("Can't create named pipe...");
                        exit(0);
                }
        }
	//Lets open in write only mode, specifying O_NONBLOCK
	//Means the return value of open, will indicate error
	//if no one has opened for reading
	//So the loop below will try till the reader is there

        while ( 0> (fd = open("Named_Pipe",O_WRONLY|O_NONBLOCK)));

        ret = write(fd,"Hi Hello",8);

        if (0 > ret)
        {
                printf("Can't Write in FIFO\n");
                exit(0);
        }
        else
        {
                printf("Bytes Written %d\n",ret);
        }

        return 0;
}
