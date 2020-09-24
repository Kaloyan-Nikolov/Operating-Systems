#include <err.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <sys/stat.h>
#include <fcntl.h>

void readFromFile(const int fd, uint8_t toNumber)
{   
    static uint32_t lineCnt = 1;
    
    char buf[1024];
    char c;
    char prevC = '\n';
    ssize_t rd; 
    while((rd = read(fd, buf, sizeof(buf))) > 0)
    {   
        for(uint16_t i = 0; i < rd; i++)
        {   
            c=buf[i];
            if(toNumber == 1 && prevC == '\n')
            {   
                char tempbuf[20];
                sprintf(tempbuf, "%d: ", lineCnt);
                write(1, tempbuf, strlen(tempbuf));
                lineCnt++;
            }
            
            write(1, &c, sizeof(char));
            prevC = c;
        }
    }
}

int main(int argc, char* argv[])
{
    uint8_t toNumber = 0;
    uint16_t cnt = 1;
    if(argc == 1)
    {
        readFromFile(0, toNumber);
    }
    else if(strcmp(argv[1], "-n") == 0)
    {
        toNumber++;
        cnt++;
        if(argc == 2)
        {
            readFromFile(0, toNumber);
        }
    }

    for(; cnt < argc; cnt++)
    {
        if(strcmp(argv[cnt], "-") == 0)
        {
            readFromFile(0, toNumber);
            continue;
        }

        struct stat st;
        if(stat(argv[cnt], &st) == -1)
        {
            warn("Could not stat %s, skipping it!", argv[cnt]);
            continue;
        }

        if(!S_ISREG(st.st_mode) || !(st.st_mode & S_IRUSR))
        {
            warnx("%s is not a readable file, skipping it!", argv[cnt]);
            continue;
        }

        int fd = open(argv[cnt], O_RDONLY);
        if(fd == -1)
        {
            warn("Could not open file %s, skipping it!", argv[cnt]);
            continue;
        }

        readFromFile(fd, toNumber);

        if(close(fd) == -1)
        {
            warn("Close failed");
        }
    }

    exit(0);
}
