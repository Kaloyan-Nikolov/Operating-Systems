#include <err.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

int main(int argc, char **argv)
{
    if(argc !=2)
        errx(1, "Invalid number of arguments. Usage: %s <command>", argv[0]);

    int fd = open("mypipe", O_RDONLY);
    if(fd == -1)
        err(2, "open failed");

    const pid_t child = fork();
    if(child == -1)
        err(3, "fork failed");

    if(child == 0)
    {
        if(dup2(fd, 0) == -1)
            err(3, "dup failed");

        if(execlp(argv[1], argv[1], (char *)NULL) == -1)
            err(4, "execlp failed");
    }

    if(wait(NULL) == -1)
        err(5, "wait failed");

    if(unlink("mypipe") == -1)
        err(6, "unlink failed");

    exit(0);
}
