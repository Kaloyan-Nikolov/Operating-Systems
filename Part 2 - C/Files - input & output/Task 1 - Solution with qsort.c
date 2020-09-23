#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <err.h>
#include <errno.h>
#include <sys/stat.h>
#include <stdint.h>

int cmp(const void *a, const void *b)
{   
    /*
    uint8_t* A = (uint8_t*)a;
    uint8_t* B = (uint8_t*)b;
    if(*A > *B)
        return 1;
    else if(*A < *B)
        return -1;
    return 0;
    */
    
    uint8_t A = *(uint8_t*)a;
    uint8_t B = *(uint8_t*)b;
    if(A > B)
        return 1;
    if(A < B)
        return -1;
    return 0;
}

int main(int argc, char *argv[])
{
    if(argc != 2)
        errx(1, "Invalid number of arguments! Usage: %s <bin_file>", argv[0]);

    struct stat st;
    if(stat(argv[1], &st) == -1)
        err(2, "Could not stat %s", argv[1]);

    off_t size = st.st_size;
    if(size == 0)
    {
        write(1, "There is nothing to sort!\n", 26);
        exit(0);
    }

    void *buf = malloc(size);
    if(buf == NULL)
        err(3, "Could not execute malloc!");

    int fd = open(argv[1], O_RDONLY);
    if(fd == -1)
        err(4, "Could not open file %s for reading", argv[1]);

    ssize_t rd = read(fd, buf, size);
    if( rd < 0)
    {
        int myerrno = errno;
        close(fd);
        free(buf);
        errno = myerrno;
        err(5, "Could not read from %s", argv[1]);
    }
    if(rd != size)
    {
        close(fd);
        free(buf);
        errx(6, "Error while trying to read from %s", argv[1]);
    }

    qsort(buf, size, 1, cmp);

    close(fd);
    fd = open(argv[1], O_WRONLY | O_TRUNC);
    if(fd == -1)
    {
        int myerrno = errno;
        free(buf);
        errno = myerrno;
        err(7, "Could not open file %s for writing!", argv[1]);
    }

    ssize_t wr = write(fd, buf, size);
    if(wr < 0)
    {
        int myerrno = errno;
        free(buf);
        close(fd);
        errno = myerrno;
        err(8, "Could not write in %s", argv[1]);
    }
    if( wr != size)
    {
        free(buf);
        close(fd);
        errx(9, "Error while trying to write in %s", argv[1]);
    }

    close(fd);
    free(buf);
    exit(0);
}
