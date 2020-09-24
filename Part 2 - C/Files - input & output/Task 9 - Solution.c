#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdint.h>
#include <err.h>
#include <errno.h>

int cmp(const void* a, const void* b)
{
    const uint32_t* A = (uint32_t*)a;
    const uint32_t* B = (uint32_t*)b;
    if(*A > *B)
        return 1;
    if(*A < *B)
        return -1;
    return 0;
}

int main(int argc, char *argv[])
{
    if(argc != 3)
        errx(1, "Invalid number of arguments! Usage: %s <input.bin> <output.bin>", argv[0]);

    struct stat st;
    if(stat(argv[1], &st) == -1)
        err(2, "Could not stat");

    if(!S_ISREG(st.st_mode))
        errx(3, "%s is not a regular file!", argv[1]);

    if((st.st_mode & S_IRUSR) == 0)
        errx(4, "%s is not a readable file", argv[1]);

    if(st.st_size%sizeof(uint32_t)!=0)
        errx(5, "%s does not contain only uint32_t numbers", argv[1]);

    uint32_t numel = st.st_size/sizeof(uint32_t);
    uint32_t fh = numel / 2;
    uint32_t allocmem = fh * sizeof(uint32_t);

    uint32_t* buf = (uint32_t*)malloc(allocmem);
    if(buf == NULL)
        err(6, "Could not execute malloc");

    int fd = open(argv[1], O_RDONLY);
    if(fd == -1)
        err(7, "Could not open %s for reading", argv[1]);

    int fd1 = open("tmp1", O_CREAT | O_TRUNC | O_WRONLY, 0644);
    if(fd1 == -1)
    {
        int myerrno = errno;
        close(fd);
        errno = myerrno;
        err(8, "Could not open tmp1 for writing!");
    }

    ssize_t rd = read(fd, buf, allocmem);
    if(rd == -1)
    {
        int myerrno = errno;
        close(fd);
        close(fd1);
        errno = myerrno;
        err(9, "Could not read from %s", argv[1]);
    }
    if(rd != allocmem)
    {
        close(fd);
        close(fd1);
        errx(10, "Error while reading!");
    }

    qsort(buf, fh, sizeof(uint32_t), cmp);

    ssize_t wr = write(fd1, buf, allocmem);
    if(wr == -1)
    {
        int myerrno = errno;
        close(fd);
        close(fd1);
        errno = myerrno;
        err(11, "Could not write in tmp1");
    }
    if(wr != allocmem)
    {
        close(fd);
        close(fd1);
        errx(12, "Error while writing!");
    }

    close(fd1);
    free(buf);

    uint32_t sf = numel - fh;
    allocmem = sf * sizeof(uint32_t);
    buf = (uint32_t*)malloc(allocmem);
    if(buf == NULL)
    {
        int myerrno = errno;
        close(fd);
        errno = myerrno;
        err(13, "Could not execute malloc");
    }

    int fd2 = open("tmp2", O_CREAT | O_WRONLY | O_TRUNC, 0644);
    if(fd2 == -1)
    {
        int myerrno = errno;
        close(fd);
        close(fd2);
        errno = myerrno;
        err(14, "Could not open tmp2");
    }

    rd = read(fd, buf, allocmem);
    if(rd < 0)
    {
        int myerrno = errno;
        close(fd);
        close(fd2);
        errno = myerrno;
        err(15, "Could not read from %s!", argv[1]);
    }
    if(rd != allocmem)
    {
        close(fd);
        close(fd2);
        errx(16, "Error while reading!");
    }

    qsort(buf, sf, sizeof(uint32_t), cmp);

    wr = write(fd1, buf, allocmem);
    if(wr == -1)
    {
        int myerrno = errno;
        close(fd);
        close(fd2);
        errno = myerrno;
        err(17, "Could not write in tmp2");
    }
    if(wr != allocmem)
    {
        close(fd);
        close(fd2);
        errx(18, "Error while writing!");
    }

    close(fd2);
    close(fd);
    free(buf);

    int fdres = open(argv[2], O_CREAT | O_TRUNC | O_WRONLY, 0644);
    if(fdres == -1)
    {
        err(19, "Could not open %s for writing!", argv[2]);
    }

    fd1 = open("tmp1", O_RDONLY);
    if(fd1 == -1)
    {
        int myerrno = errno;
        close(fdres);
        errno = myerrno;
        err(20, "Could not open tmp1 for reading");
    }

    fd2 = open("tmp2", O_RDONLY);
    if(fd2 == -1)
    {
        int myerrno = errno;
        close(fdres);
        close(fd1);
        errno = myerrno;
        err(21, "Could not open tmp2 for reading");
    }

    uint32_t a;
    uint32_t b;

    ssize_t rd1 = read(fd1, &a, sizeof(uint32_t));
    if(rd1 < 0)
    {
        int myerrno = errno;
        close(fdres);
        close(fd1);
        close(fd2);
        errno = myerrno;
        err(22, "Could not read from tmp1");
    }
    if(rd1 != sizeof(uint32_t))
    {
        int myerrno = errno;
        close(fdres);
        close(fd1);
        errno = myerrno;
        errx(23, "Error while reading from tmp1");
    }

    ssize_t rd2 = read(fd2, &b, sizeof(uint32_t));
    if(rd2 < 0)
    {
        int myerrno = errno;
        close(fdres);
        close(fd1);
        close(fd2);
        errno = myerrno;
        err(23, "Could not read from tmp2");
    }
    if(rd2 != sizeof(uint32_t))
    {
        close(fdres);
        close(fd1);
        close(fd2);
        errx(24, "Error while reading from tmp2");
    }

    uint8_t tmp1_finish = 0;
    uint8_t tmp2_finish = 0;
    while(1)
    {
        if(a < b)
        {
            wr = write(fdres, &a, sizeof(uint32_t));
            if(wr < 0)
            {
                int myerrno = errno;
                close(fdres);
                close(fd1);
                close(fd2);
                errno = myerrno;
                err(25, "Could not write!");
            }

            if(wr != sizeof(uint32_t))
            {
                close(fdres);
                close(fd1);
                close(fd2);
                errx(26, "Could not write!");
            }

            rd1=read(fd1, &a, sizeof(uint32_t));
            if(rd1 < 0)
            {
                int myerrno = errno;
                close(fdres);
                close(fd1);
                close(fd2);
                errno = myerrno;
                err(27, "Could not read!");
            }
            if(rd1 == 0)
            {
                tmp1_finish = 1;
                // tmp1 has been read;
                break;
            }
            if(rd1 != sizeof(uint32_t))
            {
                close(fdres);
                close(fd1);
                close(fd2);
                errx(28, "Could not read!");
            }

        }
        else
        {
            wr = write(fdres, &b, sizeof(uint32_t));
            if(wr < 0)
            {
                int myerrno = errno;
                close(fdres);
                close(fd1);
                close(fd2);
                errno = myerrno;
                err(29, "Could not write!");
            }
            if(wr != sizeof(uint32_t))
            {
                close(fdres);
                close(fd1);
                close(fd2);
                errx(30, "Could not write!");
            }

            rd2=read(fd2, &b, sizeof(uint32_t));
            if(rd1 < 0)
            {
                int myerrno = errno;
                close(fdres);
                close(fd1);
                close(fd2);
                errno = myerrno;
                err(31, "Could not read!");
            }
            if(rd2 == 0)
            {
                tmp2_finish = 1;
                // tmp2 has been read;
                break;
            }
            if(rd2 != sizeof(uint32_t))
            {
                close(fdres);
                close(fd1);
                close(fd2);
                errx(32, "Could not read!");
            }
        }
    }

    while(!tmp1_finish)
    {
        wr = write(fdres, &a, sizeof(uint32_t));
        if(wr < 0)
        {
            int myerrno = errno;
            close(fdres);
            close(fd1);
            close(fd2);
            errno = myerrno;
            err(33, "Could not write!");
        }
        if(wr != sizeof(uint32_t))
        {
            close(fdres);
            close(fd1);
            close(fd2);
            errx(34, "Could not write!");
        }

        rd1=read(fd1, &a, sizeof(uint32_t));
        if(rd1 < 0)
        {
            int myerrno = errno;
            close(fdres);
            close(fd1);
            close(fd2);
            errno = myerrno;
            err(35, "Could not read!");
        }
        if(rd1 == 0)
        {
            tmp1_finish = 1;
            // tmp1 has been read;
            break;
        }
        if(rd1 != sizeof(uint32_t))
        {
            close(fdres);
            close(fd1);
            close(fd2);
            errx(36, "Could not read");
        }
    }

    while(!tmp2_finish)
    {
        wr = write(fdres, &b, sizeof(uint32_t));
        if(wr < 0)
        {
            int myerrno = errno;
            close(fdres);
            close(fd1);
            close(fd2);
            errno = myerrno;
            err(37, "Could not write!");
        }
        if(wr != sizeof(uint32_t))
        {
            close(fdres);
            close(fd1);
            close(fd2);
            errx(38, "Could not write!");
        }

        rd2=read(fd2, &b, sizeof(uint32_t));
        if(rd1 < 0)
        {
            int myerrno = errno;
            close(fdres);
            close(fd1);
            close(fd2);
            errno = myerrno;
            err(39, "Could not read!");
        }
        if(rd2 == 0)
        {
            tmp2_finish = 1;
            // tmp2 has been read;
            break;
        }
        if(rd2 != sizeof(uint32_t))
        {
            int myerrno = errno;
            close(fdres);
            close(fd1);
            close(fd2);
            errno = myerrno;
            err(40, "Could not read!");
        }
    }

    close(fdres);
    close(fd1);
    close(fd2);

    if(unlink("tmp1") == -1)
        err(12, "Could not remove tmp1");
    if(unlink("tmp2") == -1)
        err(13, "Could not remove tmp2");

    exit(0);
}
