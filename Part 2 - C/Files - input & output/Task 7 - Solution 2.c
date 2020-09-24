#include <err.h>
#include <errno.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>

int main(int argc, char *argv[])
{
    if(argc != 4)
        errx(1, "Invalid number of arguments! Usage: %s patch.bin f1.bin f2.bin", argv[0]);

    const char *p1 = argv[1];
    const char *f1 = argv[2];
    const char *f2 = argv[3];

    struct stat st;
    if(stat(f1, &st) == -1)
        err(2, "Could not stat %s", f1);

    uint32_t maxsize = 0xffff;
    if(maxsize < st.st_size)
        warnx("%s is too long and the last bytes cannot be checked in the patch file", f1);

    struct stat pst;
    if(stat(p1, &pst) == -1)
        err(3, "Could not stat %s", p1);

    struct mytuple_t
    {
        uint16_t offst;
        uint8_t curbyte;
        uint8_t newbyte;
    }curtuple;

    if(pst.st_size%sizeof(struct mytuple_t)!=0)
        errx(4, "%s is not correct", p1);

    int fd1 = open(f1, O_RDONLY);
    if(fd1 == -1)
        err(5, "Could not open file %s", f1);

    int pfd= open(p1, O_RDONLY);
    if(pfd == -1)
    {
        int myerrno = errno;
        close(fd1);
        errno = myerrno;
        err(6, "Could not open file %s", p1);
    }

    int fd2 = open(f2, O_CREAT | O_TRUNC | O_WRONLY, 0644);
    if(fd2 == -1)
    {
        int myerrno = errno;
        close(fd1);
        close(pfd);
        errno = myerrno;
        err(7, "Could not open %s for writing!", f2);
    }

    if(read(pfd, &curtuple, sizeof(curtuple)) != sizeof(curtuple))
    {
        int myerrno = errno;
        close(fd1);
        close(fd2);
        close(pfd);
        errno = myerrno;
        err(8, "Could not read from %s", p1);
    }

    uint16_t curoffst = curtuple.offst;
    uint32_t cnt = 0;

    uint8_t c;
    while(read(fd1, &c, sizeof(c)) > 0)
    {
        if(curoffst == cnt)
        {
            if(c==curtuple.curbyte)
                c=curtuple.newbyte;
            else
            {
                close(fd1);
                close(fd2);
                close(pfd);
                errx(9, "%s is not consistent with %s", p1, f1);
            }
            ssize_t rd = read(pfd, &curtuple, sizeof(curtuple));
            if(rd < 0)
            {
                int myerrno = errno;
                close(fd1);
                close(fd2);
                close(pfd);
                errno = myerrno;
                err(10, "Could not read from %s", p1);
            }
            if(rd == sizeof(curtuple))
            {
                curoffst = curtuple.offst;
            }
        }
        ssize_t wr = write(fd2, &c, sizeof(c));
        if(wr < 0)
        {
            int myerrno = errno;
            close(fd1);
            close(fd2);
            close(pfd);
            errno = myerrno;
            err(11, "Could not write in %s", f2);
        }
        cnt++;
    }

    close(fd1);
    close(fd2);
    close(pfd);
    exit(0);
}
