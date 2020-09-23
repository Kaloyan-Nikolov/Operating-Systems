#include <err.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>

int cmp(const void *a, const void *b)
{   
    uint32_t *A =(uint32_t*)a;
    uint32_t *B =(uint32_t*)b;
    if(*A > *B)
        return 1;
    else if(*A < *B)
        return -1;
    return 0;
}

int main(int argc, char *argv[])
{
    if(argc != 2)
        errx(1, "Invalid number of arguments! Usage: %s <filename>", argv[0]);

    struct stat st;
    if(stat(argv[1], &st) == -1)
        err(2, "Could not stat %s", argv[1]);

    off_t size = st.st_size;
    if(size % sizeof(uint32_t) != 0)
        errx(3, "%s does not contain only uint32_t numbers", argv[1]);
    if(size == 0)
    {
        write(1, "File is sorted\n",15);
        exit(0);
    }

    uint32_t numbers = size / 4;
    uint32_t firsthalf = numbers / 2;
    uint32_t memory_to_allocate = firsthalf * sizeof(uint32_t);
    uint32_t* buf = (uint32_t*)malloc(memory_to_allocate);
    if(buf == NULL)
        err(4, "Could not execute malloc");

    int fd = open(argv[1], O_RDONLY);
    if(fd == -1)
    {
        int myerrno = errno;
        free(buf);
        errno = myerrno;
        err(5, "Could not open %s for reading", argv[1]);
    }

    ssize_t rd = read(fd, buf, memory_to_allocate);
    if(rd != (ssize_t)memory_to_allocate)
    {
        int myerrno = errno;
        close(fd);
        free(buf);
        errno = myerrno;
        err(6, "Could not read from %s", argv[1]);
    }

    qsort(buf, firsthalf, sizeof(uint32_t), cmp);

    int fd1 = open("tmp1", O_CREAT | O_TRUNC | O_WRONLY, 0644);
    if(fd1 == -1)
    {
        int myerrno = errno;
        close(fd);
        free(buf);
        errno = myerrno;
        err(7, "Could not open file tmp1");
    }

    ssize_t wr = write(fd1, buf, memory_to_allocate);
    if(wr != (ssize_t)(memory_to_allocate))
    {
        int myerrno = errno;
        close(fd);
        free(buf);
        close(fd1);
        errno = myerrno;
        err(8, "Could not write in tmp1");
    }

    close(fd1);
    free(buf);

    uint32_t secondHalf = numbers - firsthalf;
    memory_to_allocate = secondHalf * sizeof(uint32_t);

    buf = (uint32_t*)malloc(memory_to_allocate);
    if(buf == NULL)
    {
        int myerrno = errno;
        close(fd);
        errno = myerrno;
        err(9, "Could not execute malloc");
    }

    if(read(fd, buf, memory_to_allocate) != (ssize_t)memory_to_allocate)
    {
        int myerrno = errno;
        close(fd);
        free(buf);
        errno = myerrno;
        err(10, "Could not read second part of file: %s!", argv[1]);

    }

    qsort(buf, secondHalf, sizeof(uint32_t), cmp);

    int fd2 = open("tmp2", O_CREAT | O_TRUNC | O_WRONLY, 0644);
    if(fd2 == -1)
    {
        int myerrno = errno;
        close(fd);
        free(buf);
        errno = myerrno;
        err(10, "Could not open file tmp2 for writing!");
    }

    if(write(fd2, buf, memory_to_allocate) != (ssize_t)memory_to_allocate)
    {
        int myerrno = errno;
        close(fd);
        close(fd2);
        free(buf);
        errno = myerrno;
        err(11, "Could not write in file tmp2");
    }

    close(fd);
    close(fd2);
    free(buf);

    fd = open(argv[1], O_TRUNC | O_WRONLY);
    if( fd == -1)
    {
        err(12, "Could not open %s for writing", argv[1]);
    }

    fd1 = open("tmp1", O_RDONLY);
    if(fd1 == -1)
    {
        int myerrno = errno;
        close(fd);
        errno = myerrno;
        err(13, "Could not open file tmp1 for reading");
    }

    fd2 = open("tmp2", O_RDONLY);
    if( fd2 == -1)
    {
        int myerrno = errno;
        close(fd);
        close(fd1);
        errno = myerrno;
        err(14, "Could not open file tmp2 for reading");
    }

    uint32_t a;
    uint32_t b;

    if( read(fd1, &a, sizeof(uint32_t)) != sizeof(uint32_t))
    {
        int myerrno = errno;
        close(fd);
        close(fd1);
        close(fd2);
        errno = myerrno;
        err(15, "Could not read from file tmp1");
    }

    if(read(fd2, &b, sizeof(uint32_t)) != sizeof(uint32_t))
    {
        int myerrno = errno;
        close(fd);
        close(fd1);
        close(fd2);
        errno = myerrno;
        err(16, "Could not read from file tmp2");
    }

    uint8_t fd1_finished = 0;
    uint8_t fd2_finished = 0;
    while(1)
    {
        if(a<b)
        {
            if(write(fd,&a, sizeof(uint32_t)) != sizeof(uint32_t))
            {
                int myerrno = errno;
                close(fd);
                close(fd1);
                close(fd2);
                errno = myerrno;
                err(17, "Could not write in %s", argv[1]);
            }
            ssize_t rd = read(fd1, &a, sizeof(uint32_t));
            if(rd == 0)
            {
                fd1_finished = 1;
                break;
            }
            if(rd < 0)
            {
                int myerrno = errno;
                close(fd);
                close(fd1);
                close(fd2);
                errno = myerrno;
                err(18, "Could not read from file tmp1");
            }
        }
        else
        {
            if(write(fd,&b, sizeof(uint32_t)) != sizeof(uint32_t))
            {
                int myerrno = errno;
                close(fd);
                close(fd1);
                close(fd2);
                errno = myerrno;
                err(19, "Could not write in %s", argv[1]);
            }
            ssize_t rd = read(fd2, &b, sizeof(uint32_t));
            if(rd == 0)
            {
                fd2_finished = 1;
                break;
            }
            if(rd < 0)
            {
                int myerrno = errno;
                close(fd);
                close(fd1);
                close(fd2);
                errno = myerrno;
                err(20, "Could not read from file tmp2");
            }
        }
    }

    ssize_t currd;
    while(!fd1_finished)
    {
        if(write(fd, &a, sizeof(uint32_t)) != sizeof(uint32_t))
        {
            int myerrno = errno;
            close(fd);
            close(fd1);
            close(fd2);
            errno = myerrno;
            err(21, "Could not write in %s", argv[1]);
        }
        currd = read(fd1, &a, sizeof(uint32_t));
        if(currd == 0)
        {
            fd1_finished=1;
        }
        else if(currd != sizeof(uint32_t))
        {
            int myerrno = errno;
            close(fd);
            close(fd1);
            close(fd2);
            errno = myerrno;
            err(22, "Could not read from file tmp1");
        }
    }

    while(!fd2_finished)
    {
        if(write(fd, &b, sizeof(uint32_t)) != sizeof(uint32_t))
        {
            int myerrno = errno;
            close(fd);
            close(fd1);
            close(fd2);
            errno = myerrno;
            err(23, "Could not write in %s", argv[1]);
        }
        currd = read(fd2, &b, sizeof(uint32_t));
        if(currd == 0)
        {
            fd2_finished=1;
        }
        else if(currd != sizeof(uint32_t))
        {
            int myerrno = errno;
            close(fd);
            close(fd1);
            close(fd2);
            errno = myerrno;
            err(24, "Could not read from file tmp2");
        }
    }

    close(fd);
    close(fd1);
    close(fd2);
    exit(0);
}
