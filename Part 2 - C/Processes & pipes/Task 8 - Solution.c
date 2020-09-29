#include <err.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdint.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    if(argc != 3)
        errx(1, "Invalid number of arguments! Usage: %s <cmd1> <cmd2>", argv[0]);

    int a[2];
    if(pipe(a) == -1)
        err(2, "pipe failed");

    const pid_t child1 = fork();
    if(child1 == -1)
        err(3, "fork failed");

    if(child1 == 0)
    {
        // the child will read from pipe a
        close(a[1]);
        if(dup2(a[0],0) == -1)
            err(4, "dup2 failed");
        close(a[0]);
        if(execlp(argv[1], argv[1], (char*)NULL) == -1)
            err(5, "exec failed");
    }

    int b[2];
    if(pipe(b) == -1)
        err(6, "dup2 failed");

    const pid_t child2 = fork();
    if(child2 == -1)
        err(7, "fork failed");
    if(child2 == 0)
    {
        // the child will read from pipe b
        close(b[1]);
        if(dup2(b[0],0) == -1)
            err(8, "dup2 failed");
        close(b[0]);
        if(execlp(argv[2], argv[2], (char*)NULL) == -1)
            err(9, "exec failed");
    }

    uint8_t cnt = 0;
    char buf[256];
    ssize_t rd;
    while((rd = read(0, buf, sizeof(buf))) > 0)
    {
        if(cnt%2 == 1)
        {
            if(write(a[1], buf, rd) != rd)
                err(10, "write failed");
        }
        else
        {
            if(write(b[1], buf, rd) != rd)
                err(11, "write failed");
        }
        cnt++;
    }

    close(a[1]);
    close(b[1]);

    int status1;
    if(wait(&status1) == -1)
        err(12, "wait failed");

    if(WIFEXITED(status1) == 0)
        warnx("child did not finish normally!");

    if(WEXITSTATUS(status1) != 0)
        warnx("child did not exit with exit code 0");

    int status2;
    if(wait(&status2) == -1)
        err(13, "wait failed");

    if(WIFEXITED(status2) == 0)
        warnx("child did not finish normally!");

    if(WEXITSTATUS(status2) != 0)
        warnx("child did not exit with exit code 0");

    exit(0);
}
