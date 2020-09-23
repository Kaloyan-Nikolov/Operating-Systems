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
    if(argc != 1)
        errx(1, "Invalid number of arguments! Usage: %s", argv[0]);

    struct stat st1;
    if(stat("f1", &st1) == -1)
        err(2, "Could not stat");

    if(!S_ISREG(st1.st_mode) || !(st1.st_mode & S_IRUSR))
        errx(3, "f1 is not a file or it is not readable!");

    struct stat st2;
    if(stat("f2", &st2) == -1)
        err(4, "Could not stat");

    if(!S_ISREG(st2.st_mode) || !(st2.st_mode & S_IRUSR))
        errx(5, "f2 is not a file or it is not readable!");

    if(st2.st_size % sizeof(uint32_t) != 0)
        errx(6, "f2 is corrupted!");

    if(st1.st_size % (sizeof(uint32_t)*2) != 0)
        errx(7, "f1 is corrupted");

    int fd1 = open("f1", O_RDONLY);
    if(fd1 == -1)
        err(8, "Could not open!");

    int fd2 = open("f2", O_RDONLY);
    if(fd2 == -1)
    {
        int myerrno = errno;
        close(fd1);
        errno = myerrno;
        err(9, "Could not open");
    }

    int fd3 = open("f3", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if(fd3 == -1)
    {
        int myerrno = errno;
        close(fd1);
        close(fd2);
        errno = myerrno;
        err(10, "Could not open");
    }

    uint32_t ppair[2];

    while(read(fd1, ppair, sizeof(ppair)) == sizeof(ppair))
    {
        off_t lsk = lseek(fd2, ppair[0], SEEK_SET);
        if(lsk == -1)
        {
            int myerrno = errno;
            close(fd1);
            close(fd2);
            close(fd3);
            errno = myerrno;
            err(11, "lseek failed");
        }
        else if (lsk > st2.st_size)
        {
            close(fd1);
            close(fd2);
            close(fd3);
            errx(12, "lseek failed");
        }

        for(uint32_t i = 0; i < ppair[1]; i++)
        {
            uint32_t cur;
            ssize_t rd = read(fd2, &cur, sizeof(uint32_t));
            if(rd != sizeof(uint32_t))
            {
                int myerrno = errno;
                close(fd1);
                close(fd2);
                close(fd3);
                errno = myerrno;
                err(13, "read");
            }

            ssize_t wr = write(fd3, &cur, sizeof(uint32_t));
            if(wr != sizeof(uint32_t))
            {
                int myerrno = errno;
                close(fd1);
                close(fd2);
                close(fd3);
                errno = myerrno;
                err(13, "write");
            }
        }
    }

    close(fd1);
    close(fd2);
    close(fd3);
    exit(0);
}
