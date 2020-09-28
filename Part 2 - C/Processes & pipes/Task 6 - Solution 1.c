#include <err.h>
#include <errno.h>
#include <time.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

int main(int argc, char *argv[])
{
    if(argc < 3)
        errx(1, "Invalid number of arguments! Usage: %s <num> <Q> [<Q_options>]", argv[0]);

    uint8_t cond = 0;
    uint32_t expectedDur = argv[1][0] - '0';
    while(cond < 2)
    {
        const pid_t child = fork();
        if(child == -1)
            err(2, "fork failed");

        time_t st = time(NULL);
        if(st == -1)
            err(3, "time failed");

        if(child == 0)
        {
            if(execvp(argv[2], argv + 2) == -1)
                err(4, "exec failed");
        }

        int status;
        if(wait(&status) == -1)
            err(5, "wait failed");

        time_t end = time(NULL);
        if(end == -1)
            err(6, "time failed");

        uint8_t exitCode;
        if(WIFEXITED(status) == 0)
            exitCode = 129;
        else
            exitCode = WEXITSTATUS(status);

        int fd = open("run.log", O_CREAT | O_APPEND | O_WRONLY, 0644);
        if(fd == -1)
            err(7, "open failed");

        char buf[128];
        sprintf(buf, "%ld %ld %d\n", st, end, exitCode);
        if(write(fd, buf, strlen(buf)) != (ssize_t)strlen(buf))
        {
            const int myerrno = errno;
            close(fd);
            errno = myerrno;
            err(8, "write failed");
        }
        close(fd);

        uint32_t dur = end - st;
        if(dur < expectedDur && exitCode != 0)
        {
            cond++;
        }
        else
        {
            cond = 0;
        }
    }

    exit(0);
}
