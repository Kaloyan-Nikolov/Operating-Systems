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
    if(argc != 5)
        errx(1, "Invalid number of arguments! Usage: %s <f1.dat> <f1.idx> <f2.dat> <f2.idx>", argv[0]);

    struct stat std;
    if(stat(argv[1], &std) == -1)
    {   
        err(2, "not stat");
    }

    if(!S_ISREG(std.st_mode) || !(std.st_mode & S_IRUSR))
    {   
        errx(3, "not file or not readable");
    }

    struct stat sti;
    if(stat(argv[2], &sti) == -1)
    {   
        err(4, "not stat");
    }

    if(!S_ISREG(sti.st_mode) || !(sti.st_mode & S_IRUSR))
    {   
        errx(5, "not file or not readable");
    }

    struct unit_t
    {
        uint16_t offst;
        uint8_t len;
        uint8_t saved;

    };//__attribute__((pached));

    if(sti.st_size % sizeof(struct unit_t) != 0)
        errx(6, "file is corrupted");

    int f1d = open(argv[1], O_RDONLY);
    if(f1d == -1)
        err(7, "open failed");

    int f1i = open(argv[2], O_RDONLY);
    if(f1i == -1)
    {
        int myerrno = errno;
        close(f1d);
        errno = myerrno;
        err(8, "open failed");
    }

    int f2d = open(argv[3], O_CREAT | O_TRUNC | O_WRONLY, 0644);
    if(f2d == -1)
    {
        int myerrno = errno;
        close(f1d);
        close(f1i);
        errno = myerrno;
        err(9, "open failed");
    }

    int f2i = open(argv[4], O_CREAT | O_TRUNC | O_WRONLY, 0644);
    if(f2i == -1)
    {
        int myerrno = errno;
        close(f1d);
        close(f1i);
        close(f2d);
        errno = myerrno;
        err(10, "open failed");
    }

    struct unit_t curunit;
    uint8_t lastlen = 0;
    uint16_t lastoffst = 0;
    while(read(f1i, &curunit, sizeof(curunit)) == sizeof(curunit))
    {
        if(curunit.len == 0)
        {
            continue;
        }

        off_t lsk = lseek(f1d, curunit.offst, SEEK_SET);
        if(lsk == -1)
        {
            int myerrno = errno;
            close(f1d);
            close(f1i);
            close(f2d);
            close(f2i);
            errno = myerrno;
            err(11, "lseek failed");
        }

        uint8_t b;
        if(read(f1d, &b, sizeof(b)) != sizeof(b))
        {
            int myerrno = errno;
            close(f1d);
            close(f1i);
            close(f2d);
            close(f2i);
            errno = myerrno;
            err(12, "read failed");
        }

        if(b > 'Z' || b < 'A')
        {
            continue;
        }

        for(uint8_t i = 1; i < curunit.len; i++)
        {
            if(write(f2d, &b, sizeof(b)) != 1)
            {
                int myerrno = errno;
                close(f1d);
                close(f1i);
                close(f2d);
                close(f2i);
                errno = myerrno;
                err(13, "write failed");
            }

            if(read(f1d, &b, sizeof(b)) != sizeof(b))
            {
                int myerrno = errno;
                close(f1d);
                close(f1i);
                close(f2d);
                close(f2i);
                errno = myerrno;
                err(14, "read failed");
            }
        }

        if(write(f2d, &b, sizeof(b)) != 1)
        {
            int myerrno = errno;
            close(f1d);
            close(f1i);
            close(f2d);
            close(f2i);
            errno = myerrno;
            err(15, "write failed");
        }

        struct unit_t n;
        n.offst = lastoffst + lastlen;
        n.len = curunit.len;
        n.saved = 0;

        lastlen = n.len;
        lastoffst = n.offst;

        if(write(f2i, &n, sizeof(n)) != sizeof(n))
        {
            int myerrno = errno;
            close(f1d);
            close(f1i);
            close(f2d);
            close(f2i);
            errno = myerrno;
            err(16, "write failed");
        }
    }

    close(f1d);
    close(f1i);
    close(f2d);
    close(f2i);
    exit(0);
}
