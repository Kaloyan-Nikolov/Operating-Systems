#include <err.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdint.h>

int main(int argc, char *argv[])
{
    if(argc != 4)
        errx(1, "Invalid number of arguments! Usage: %s <patch.bin> <f1.bin> <f2.bin>", argv[0]);

    const char* const f1 = argv[2];
    const char* const f2 = argv[3];
    const char* const p = argv[1];

    struct stat pst;
    if(stat(p, &pst) == -1)
        err(2, "stat failed");

    if(!S_ISREG(pst.st_mode) || !(pst.st_mode & S_IRUSR))
        errx(3, "%s is not a readable file!", p);

    struct stat fst;
    if(stat(f1, &fst) == -1)
        err(4, "stat failed");

    if(!S_ISREG(fst.st_mode) || !(fst.st_mode & S_IRUSR))
        errx(5, "%s is not a readable file!", f1);

    if(pst.st_size % 4 != 0)
        errx(6, "%s is corrupted!", p);

    if(fst.st_size > UINT16_MAX)
        warnx("There might be changes which cannot be applied because file is too long");

    int fd1 = open(f1, O_RDONLY);
    if(fd1 == -1)
        err(7, "open failed");

    int fd2 = open(f2, O_RDWR | O_TRUNC | O_CREAT, 0644);
    if(fd2 == -1)
    {
        int myerrno = errno;
        close(fd1);
        errno = myerrno;
        err(8, "open failed");
    }

    int pfd = open(p, O_RDONLY);
    if(pfd == -1)
    {
        int myerrno = errno;
        close(fd1);
        close(fd2);
        errno = myerrno;
        err(9, "open failed");
    }

    char buf[4096];
    ssize_t rd;
    while((rd = read(fd1,buf, sizeof(buf))) > 0)
    {
        if(write(fd2, buf, rd) != rd)
        {
            int myerrno = errno;
            close(fd1);
            close(pfd);
            close(fd2);
            errno = myerrno;
            err(10, "write failed");
        }
    }

    struct tuple_t
    {
        uint16_t offst;
        uint8_t orig;
        uint8_t neww;
    }__attribute__((packed));

    struct tuple_t unit;

    while(read(pfd, &unit, sizeof(unit)) > 0)
    {
        off_t lsk = lseek(fd2, unit.offst, SEEK_SET);
        if(lsk == -1)
        {
            int myerrno = errno;
            close(fd1);
            close(pfd);
            close(fd2);
            errno = myerrno;
            err(11, "lseek failed");
        }

        uint8_t b;
        if(read(fd2, &b, sizeof(b)) != sizeof(b))
        {
            int myerrno = errno;
            close(fd1);
            close(pfd);
            close(fd2);
            errno = myerrno;
            err(12, "read failed");
        }

        if(b != unit.orig)
        {
            close(fd1);
            close(pfd);
            close(fd2);
            errx(13, "%s is corrupted", f1);
        }

        lsk = lseek(fd2, unit.offst, SEEK_SET);
        if(lsk == -1)
        {
            int myerrno = errno;
            close(fd1);
            close(pfd);
            close(fd2);
            errno = myerrno;
            err(14, "lseek failed");
        }

        if(write(fd2, &unit.neww, sizeof(unit.neww)) != sizeof(unit.neww))
        {
            int myerrno = errno;
            close(fd1);
            close(pfd);
            close(fd2);
            errno = myerrno;
            err(15, "write failed");
        }

    }

    close(fd1);
    close(fd2);
    close(pfd);
    exit(0);
}
