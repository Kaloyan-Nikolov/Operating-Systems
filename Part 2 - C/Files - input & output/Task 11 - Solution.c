#include <err.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdint.h>

int cmp(const void *a, const void *b)
{   
    uint16_t *A = (uint16_t*)a;
    uint16_t *B = (uint16_t*)b;
    if( *A > *B)
        return 1;
    if(*A < *B)
        return -1;
    return 0;
}

int main(int argc, char *argv[])
{
    if(argc != 3)
        errx(1, "Invalid numebr of arguments! Usage: %s <input.bin> <output.bin>", argv[0]);

    const char* const f1 = argv[1];
    const char* const f2 = argv[2];

    struct stat stf1;
    if(stat(f1, &stf1) == -1)
        err(2, "stat failed");

    if(!S_ISREG(stf1.st_mode) || !(stf1.st_mode & S_IRUSR))
        errx(3, "%s is not a readable file", f1);

    if(stf1.st_size % sizeof(uint16_t) != 0)
        errx(4, "%s does not contain only uint16_t numbers!", argv[1]);

    int fd1 = open(f1, O_RDONLY);
    if(fd1 == -1)
        err(5, "open failed");

    int fd2 = open(f2, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if(fd2 == -1)
    {
        int myerrno = errno;
        close(fd1);
        errno = myerrno;
        err(6, "open failed");
    }

    uint16_t *buf = (uint16_t*)malloc(stf1.st_size);
    if(buf == NULL)
    {
        int myerrno = errno;
        close(fd1);
        close(fd2);
        errno = myerrno;
        err(7, "no memory");
    }

    if(read(fd1, buf, stf1.st_size) != stf1.st_size)
    {
        int myerrno = errno;
        close(fd1);
        close(fd2);
        free(buf);
        errno = myerrno;
        err(8, "read failed");
    }

    qsort(buf, stf1.st_size / sizeof(uint16_t), sizeof(uint16_t), cmp);

    if(write(fd2, buf, stf1.st_size) != stf1.st_size)
    {
        int myerrno = errno;
        close(fd1);
        close(fd2);
        free(buf);
        errno = myerrno;
        err(8, "write failed");
    }

    free(buf);
    close(fd1);
    close(fd2);
    exit(0);
}
