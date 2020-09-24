#include <err.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdint.h>

int main(int argc, char *argv[])
{
    if(argc != 4)
        errx(1, "Invalid number of arguments! Usage: %s <f1.bin> <f2.bin> <patch.bin>", argv[0]);

    struct stat sf1;
    if(stat(argv[1], &sf1) == -1)
        err(2, "Could not stat");

    struct stat sf2;
    if(stat(argv[2], &sf2) == -1)
        err(3, "Could not stat");

    if(!S_ISREG(sf1.st_mode) || (sf1.st_mode & S_IRUSR) == 0)
        errx(4, "not file or not readable - %s", argv[1]);
    if(!S_ISREG(sf2.st_mode) || (sf2.st_mode & S_IRUSR) == 0)
        errx(4, "not file or not readable - %s", argv[2]);

    if(sf1.st_size != sf2.st_size)
        errx(5, "files have different length");

    struct unit_t
    {
        uint16_t offst;
        uint8_t orig;
        uint8_t neww;
    }__attribute__((packed));

    struct unit_t cur;

    int fd1 = open(argv[1], O_RDONLY);
    if(fd1 == -1)
        err(6, "open failed");

    int fd2 = open(argv[2], O_RDONLY);
    if(fd2 == -1)
    {
        int myerrno = errno;
        close(fd1);
        errno = myerrno;
        err(7, "open failed");
    }

    int fdp = open(argv[3], O_CREAT | O_TRUNC | O_WRONLY, 0644);
    if(fdp == -1)
    {
        int myerrno = errno;
        close(fd1);
        close(fd2);
        errno = myerrno;
        err(8, "open failed");
    }


    uint8_t a, b;
    uint16_t cnt = 0;
    while(read(fd1, &a, sizeof(a)) > 0 && read(fd2, &b, sizeof(b)) > 0)
    {
        if(a == b)
        {
            cnt++;
            continue;
        }

        cur.offst = cnt;
        cur.orig = a;
        cur.neww = b;
        cnt++;
        if(write(fdp, &cur, sizeof(cur)) != sizeof(cur))
        {
            int myerrno = errno;
            close(fd1);
            close(fd2);
            close(fdp);
            errno = myerrno;
            err(9, "write failed");
        }
    }

    close(fd1);
    close(fd2);
    close(fdp);
    exit(0);
}
