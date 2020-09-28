#include <err.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main(int argc, char *argv[])
{
    if(argc != 2)
        errx(1, "Invalid number of arguments! Usage: %s <dirname>", argv[0]);
    struct stat st;
    if(stat(argv[1],&st) == -1)
        err(2, "stat failed");

    if(!S_ISDIR(st.st_mode) || !(st.st_mode & S_IRUSR))
        errx(3, "Please enter a readable directory!");

    int a[2];
    if(pipe(a) == -1)
        err(4, "pipe failed");

    const pid_t pid_find = fork();
    if(pid_find == -1)
        err(5, "fork failed");

    if(pid_find == 0)
    {
        // we will write in a;
        close(a[0]);
        if(dup2(a[1],1) == -1)
            err(6, "dup2 failed");
        int fd = open("/dev/null", O_WRONLY);
        if(fd == -1)
            err(7, "could not open /dev/null");
        if(dup2(fd, 2) == -1)
            err(8, "dup2 failed");
        close(fd);
        if(execlp("find", "find", argv[1], "-printf", "%T@ %p\n", (char*)NULL) == -1)
            err(9, "exec failed");
    }

    close(a[1]);

    int b[2];
    if(pipe(b) == -1)
        err(10, "pipe failed");

    const pid_t pid_sort = fork();
    if(pid_sort == -1)
        err(11, "fork failed");

    if(pid_sort == 0)
    {
        // we will read from a;
        // we will write in b;
        if(dup2(a[0], 0) == -1)
            err(12, "dup2 failed");
        close(b[0]);
        if(dup2(b[1], 1) == -1)
            err(13, "dup2 failed");
        if(execlp("sort", "sort", "-n", (char*)NULL) == -1)
            err(14, "exec failed");
    }

    close(b[1]);

    int c[2];
    if(pipe(c) == -1)
        err(15, "pipe failed");

    const pid_t pid_tail = fork();
    if(pid_tail == -1)
        err(16, "fork failed");

    if(pid_tail == 0)
    {
        // we will read from b;
        // we will write in c;
        if(dup2(b[0], 0) == -1)
            err(17, "dup2 failed");
        close(c[0]);
        if(dup2(c[1], 1) == -1)
            err(18, "dup2 failed");

        if(execlp("tail", "tail", "-n1", (char*)NULL) == -1)
            err(19, "exec failed");
    }

    close(c[1]);

    // we will read from c;
    if(dup2(c[0], 0) == -1)
        err(20, "dup2 failed");
    close(c[0]);

    if(execlp("cut", "cut", "-d ", "-f2-", (char*)NULL) == -1)
        err(23, "exec failed");
}
