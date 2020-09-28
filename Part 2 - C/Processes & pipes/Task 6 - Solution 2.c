#include <err.h>
#include <errno.h>
#include <time.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>

int main(int argc, char* argv[])
{
    if(argc < 3)
        errx(1, "Invalid number of argumnets! Usage: %s <num> <Q> [Q_args]", argv[0]);

    char *myargs[50]; // the max number of args or strings we can save to pass to the command 
    int cnt = 0;
    for(int i = 2; i < argc; i++)
    {
        myargs[cnt++]=argv[i];
    }
    myargs[cnt]=NULL;

    uint8_t minDur = argv[1][0] - '0';
    uint8_t lastExitCode = 0;
    uint8_t lastDuration = minDur + 5;
    while(1)
    {
        const pid_t Q = fork();
        if(Q == -1)
            err(2, "fork failed");
        time_t st = time(NULL);
        if(st == -1)
            err(3, "time failed");
        if(Q == 0)
        {
            if(execvp(argv[2], myargs) == -1)
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

        int fd = open("run.log", O_APPEND | O_CREAT | O_WRONLY, 0644);
        if(fd == -1)
            err(7, "open failed");

        char buf[128];
        sprintf(buf, "%ld %ld %d\n", st, end, exitCode);
        if(write(fd, buf, strlen(buf)) != (ssize_t)strlen(buf))
        {
            int myerrno = errno;
            close(fd);
            errno = myerrno;
            err(8, "write failed");
        }
        close(fd);

        uint8_t cond1 = exitCode && lastExitCode;
        lastExitCode = exitCode;
        time_t duration = end - st;
        uint8_t cond2 = duration < minDur && lastDuration < minDur;
        if(cond1 && cond2)
            break;
        lastDuration = duration;
    }

    exit(0);
}
