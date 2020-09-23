#include <err.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdint.h>
#include <sys/stat.h>

int main(int argc, char *argv[])
{
    if(argc != 2)
        errx(1, "Invalid number of arguments! Usage: %s <filename>", argv[0]);

    const char *f1 = argv[1];
    struct stat st;
    if(stat(f1, &st) == -1)
        err(2, "Could not stat %s!", f1);

    if(!S_ISREG(st.st_mode))
        errx(3, "%s is not a regular file!", f1);

    uint8_t count[256] = { 0 };
    /*for(uint16_t i = 0; i < 256; i++)
        count[i]=0;*/
    uint8_t overflows[256];
    for(int16_t i = 0; i < 256; i++)
        overflows[i]=1;

    int fd = open(f1, O_RDONLY);
    if(fd == -1)
        err(4, "Could not open %s for reading!", f1);

    uint8_t c;
    while(read(fd, &c, sizeof(c)) > 0)
    {
        count[c]++;
        if(count[c] == 0xff)
        {
            count[c]=0;
            overflows[c]++;
        }
    }

    close(fd);
    fd = open(f1, O_TRUNC | O_WRONLY);
    if(fd == -1)
        err(5, "Could not open %s for writing!", f1);

    //for(int16_t i = 255; i >= 0 ; i--) // for decending order
    for(int16_t i = 0; i < 256; i++) // for ascending order
    {
        while(overflows[i]-- > 0)
        {
            while(count[i]-- > 0)
            {
                if(write(fd, &i, sizeof(uint8_t)) != sizeof(uint8_t))
                {
                    int myerrno = errno;
                    close(fd);
                    errno = myerrno;
                    errx(6, "Could not write in %s!", f1);
                }
            }
            count[i]=0xff;
        }
    }

    close(fd);
    exit(0);
}
